// $Header$

// $Log$
// Revision 1.2  2000-09-27 14:57:29+01  jjs
// Added FASTFTOL to PC version.
//

#include	"Common.h"

#if TARGET == PC || TARGET == DC

#include	"Gamut.h"
#include	"Camera.h"

//**********************************************************************************
void	CGamut::SetSize(int minx, int maxx, int miny, int maxy)
{
	MinX = minx;
	MaxX = maxx;
	MinY = miny;
	MaxY = maxy; // + 1; //! hmmm
}

//**********************************************************************************

#if 0
#ifdef DEBUG
void	CGamut::DrawDebug()
{	
#define SCL 5
	int c1,c2;
	for (c1=0;c1<mNumP;c1++)
	for (c2=c1+1;c2<mNumP;c2++)
	{		
		LbDraw_Line(int(SCL*mP[c1].X),int(SCL*mP[c1].Y),int(SCL*mP[c2].X),int(SCL*mP[c2].Y),TbColour(255,255,255,255));
	}
	for (c1=0;c1<mNumO;c1++)
	{
		c2=(c1+1)%mNumO;
		LbDraw_Line(int(SCL*mOut[c1].X),int(SCL*mOut[c1].Y),int(SCL*mOut[c2].X),int(SCL*mOut[c2].Y),TbColour(255,000,255,255));
	}
	for (c1=mFirstY;c1<mLastY;c1++)
	{
		//LbDraw_Line(mGamutLines[c1].Min*SCL,c1*SCL,mGamutLines[c1].Max*SCL,c1*SCL,TbColour(000,100,000,000));
		LbDraw_Line(1+mGamutLines[c1].Min*SCL,c1*SCL,1+mGamutLines[c1].Min*SCL,c1*SCL+SCL,TbColour(000,000,255,000));
		LbDraw_Line(1+mGamutLines[c1].Max*SCL,c1*SCL,1+mGamutLines[c1].Max*SCL,c1*SCL+SCL,TbColour(255,255,255,000));
	}
	LbDraw_Line(0,1+mFirstY*SCL,640,1+mFirstY*SCL,TbColour(128,000,000,000));
	LbDraw_Line(0,1+mLastY*SCL,640,1+mLastY*SCL,TbColour(255,000,000,000));
}

#endif
#endif

int CompareY(const void *arg1, const void *arg2)
{
	float f=((FVector*)arg1)->Y-((FVector*)arg2)->Y;
	return (f<0)?-1:(f==0)?0:1;
}

BOOL CompareAngles(const FVector &a,const FVector &b)
{
	return(a.X*b.Y>a.Y*b.X);
}


#if TARGET!=DC
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

void	CGamut::AddLine(int x, int y, int x2)
{
	if (y<MinY || y>=MaxY) return;
	if (x<MinX) x=MinX;
	if (x2>MaxX) x2=MaxX;
	if (x2<=x) return;
	if (mFirstY>y) mFirstY=y;
	if (mLastY<y) mLastY=y;
	if (mGamutLines[y].Min>x) mGamutLines[y].Min=x;
	if (mGamutLines[y].Max<x2) mGamutLines[y].Max=x2;
}

void	CGamut::Calculate(const CCamera *c, SINT scale)
{	
	// reset the scan conversion
	mFirstY=mLastY=0;

	// work out the points of the view volume
	mNumP=5;
//	FVector xvec=(GVectorToFVector(c->mOrientation.Column0()))*MAXDISTANCE; // this should have some lens factor in it.
//	FVector yvec=(GVectorToFVector(c->mOrientation.Column1()))*MAXDISTANCE;
	FVector xvec=(GVectorToFVector(c->mOrientation.Column0()))*MAXDISTANCE / 1.9f; // this should have some lens factor in it.
	FVector yvec=(GVectorToFVector(c->mOrientation.Column1()))*MAXDISTANCE / 2.5f;
	FVector zvec=(GVectorToFVector(c->mPos) / float(scale)) - GVectorToFVector(c->mOrientation.Column2()) * SHAGDISTANCE;
	mP[0]=zvec;
	zvec += GVectorToFVector(c->mOrientation.Column2()) * MAXDISTANCE;
	mP[1]=zvec+xvec+yvec;
	mP[2]=zvec+xvec-yvec;
	mP[3]=zvec-xvec+yvec;
	mP[4]=zvec-xvec-yvec;
	
	// clip the points to the ground plane
	int c1,c2;
	for (c1=0;c1<5;c1++)
	for (c2=c1+1;c2<5;c2++)
	{
		if ((mP[c1].Z<GROUNDZ)^(mP[c2].Z<GROUNDZ))
		{
			// clip this line
			float f=(mP[c1].Z-GROUNDZ)/(mP[c1].Z-mP[c2].Z);
			mP[mNumP]=mP[c1]+(mP[c2]-mP[c1])*f;
			mP[mNumP++].Z=GROUNDZ;
		}
	}
	
	// remove points under the ground	
	for (c1=0;c1<mNumP;c1++)
	{
		while (mNumP>c1 && mP[c1].Z>GROUNDZ) mP[c1]=mP[--mNumP];
	}
	
	if (mNumP<3) return;	// nothing!
	
	// calculate convex hull
	// sort by height
	qsort(mP,mNumP,sizeof(FVector),CompareY);
	// run up one edge
	FVector *p,*curp,*out,*endp;
	out=mOut;
	curp=mP;
	*out++=*curp;
	p=curp+1;
	endp=mP+mNumP;
	do
	{		
		FVector bestdelta=*p-*curp;
		FVector *bestp=p;
		for (p++;p<endp;p++)
		{
			FVector delta=*p-*curp;
			if (CompareAngles(delta,bestdelta)) 
			{
				bestdelta=delta;
				bestp=p;
			}
		}
		*out++=*bestp;
		curp=bestp;
		p=bestp+1;
	} while (p<endp);

	// now go down the other side
	curp=endp-1;
	p=curp-1;
	do
	{
		FVector bestdelta=*curp-*p;
		FVector *bestp=p;
		for (p--;p>=mP;p--)
		{
			FVector delta=*curp-*p;
			if (CompareAngles(delta,bestdelta)) 
			{
				bestdelta=delta;
				bestp=p;
			}
		}
		*out++=*bestp;
		curp=bestp;
		p=bestp-1;
	} while (p>=mP);
	mNumO=out-mOut;

	if (mNumO<3) 
		return;	// nothing!

	// scan convert
#if TARGET == PC
	FASTFTOL(&mFirstY, mOut->Y);
#else
	mFirstY=(int)mOut->Y;
#endif
	if (mFirstY<0) 
		mFirstY=0;
	mLastY=mFirstY;

	FVector *lo=mOut+mNumO-1;
	FVector *o=mOut;
	float x,dx;
	int sy,ey;
	GamutLine *line;
	//memset(mGamutLines,0,sizeof(GamutLine)*256);
	for (c1=0;c1<mNumO;c1++,lo=o++)
	{		
		if (o->Y>lo->Y)
		{			
			// downward edge	
#if TARGET == PC
			int t1,t2;

			FASTFTOL(&t1, lo->X + 1);
			FASTFTOL(&t2, o->X + 1);
			int minx=MIN(t1,t2);
			if (minx<MinX) 
				minx=MinX;
			int maxx=MAX(t1,t2);
			if (maxx>MaxX) 
				maxx=MaxX;
#else
			int minx=MIN(int(lo->X)+1,int(o->X)+1);
			if (minx<MinX) 
				minx=MinX;
			int maxx=MAX(int(lo->X)+1,int(o->X)+1);
			if (maxx>MaxX) 
				maxx=MaxX;
#endif
			dx=(o->X-lo->X)/(o->Y-lo->Y);
#if TARGET == PC
			FASTFTOL(&sy,lo->Y);
#else
			sy=(int)lo->Y;
#endif
			if (sy<MinY) 
				sy=MinY;
			if (sy>=MaxY) 
				continue;
#if TARGET == PC
			FASTFTOL(&ey, o->Y+1);
#else
			ey=(int)o->Y+1;
#endif
			if (ey>MaxY) 
				ey=MaxY;
			if (ey<=MinY) 
				continue;
			x=lo->X+(sy-lo->Y)*dx;		
			line=mGamutLines+sy;
			if (sy<mFirstY) 
				mFirstY=sy;
			if (ey>mLastY) 
				mLastY=ey;		
			x+=1;
			for (;sy<ey;sy++,x+=dx,line++)
			{
				if (x<minx) 
					line->Max=minx; 
				else 
					if (x>maxx) 
						line->Max=maxx; 
					else 
#if TARGET == PC
						FASTFTOL(&line->Max, x);
#else
						line->Max=(int)x;
#endif

//				ASSERT(line->Max<=MaxX);
//				if (line->Max<MaxX) line->Max=MaxX;  //! aaaaaaargh  - why is the gamut soooooooooo broken.... :( :(
			}
		}
		else
		if (o->Y<lo->Y)
		{
			// upward edge
#if TARGET == PC
			int t1,t2;

			FASTFTOL(&t1, lo->X);
			FASTFTOL(&t2, o->X);

			int minx=MIN(t1,t2);
			if (minx<MinX) 
				minx=MinX;
			int maxx=MAX(t1,t2);
			if (maxx>MaxX) 
				maxx=MaxX;
#else
			int minx=MIN(int(lo->X),int(o->X));
			if (minx<MinX) 
				minx=MinX;
			int maxx=MAX(int(lo->X),int(o->X));
			if (maxx>MaxX) 
				maxx=MaxX;
#endif
			dx=(o->X-lo->X)/(o->Y-lo->Y);
#if TARGET == PC
			FASTFTOL(&sy,o->Y);
#else
			sy=(int)o->Y;
#endif
			if (sy<MinY) 
				sy=MinY;
			if (sy>=MaxY) 
				continue;
#if TARGET == PC
			FASTFTOL(&ey,lo->Y+1);
#else
			ey=(int)lo->Y+1;
#endif
			if (ey>MaxY) 
				ey=MaxY;
			if (ey<=MinY) 
				continue;
			x=o->X;
			if (sy>o->Y) 
				x+=(sy-o->Y)*dx;					
			line=mGamutLines+sy;
			if (sy<mFirstY) 
				mFirstY=sy;
			if (ey>mLastY) 
				mLastY=ey;			
			for (;sy<ey;sy++,x+=dx,line++)
			{				
				if (x<minx) 
					line->Min=minx; 
				else 
					if (x>maxx) 
						line->Min=maxx; 
					else
#if TARGET == PC
						FASTFTOL(&line->Min, x);
#else
						line->Min=(int)x;				
#endif
			}
		}
		
	}

	// make sure the 4 squares around the player are always in view... 
#if TARGET == PC
	int cx,cy;
	FASTFTOL(&cx,(GToF(c->mPos.X) / float(scale)));
	FASTFTOL(&cy,(GToF(c->mPos.Y) / float(scale)));
#else
	int cx=(int)(GToF(c->mPos.X) / float(scale));
	int cy=(int)(GToF(c->mPos.Y) / float(scale));
#endif
	AddLine(cx,cy,cx+2);
	AddLine(cx,cy+1,cx+2);

	ASSERT(mFirstY >= 0);

	// JCL - ok, this is really shite, but the gamut..well.. has bugs in it (thanks alex)....

	SINT c0;
	for(c0=mFirstY; c0<mLastY;c0++) // prolly should be <=mLastY but, that's buggered too...
	{
		if (mGamutLines[c0].Min<MinX) mGamutLines[c0].Max=MinX;
		if (mGamutLines[c0].Max>MaxX) mGamutLines[c0].Max=MaxX;
	}
	// bugger....
}

#endif
