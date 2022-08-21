#include	"Common.h"

#if TARGET == DC

#include	"DCDecal.h"
#include	"Globals.h"
#include 	"Character.h"
#include 	"MainCode.h"
#include	"DCJunk.h"



extern long Interpolcol (unsigned long *col,short mult);

void CommonOffsetCoord(int x,int y,int z)
{
#ifdef JCLDCNOPSX   // Maybe replace this with a general camera setup function?

	CCamera*	Gcam=SPCamera;
	// This flag is set during a particle system processing, since ALL the particles
	//	use the same camera matrix... (Saves quite a bit of loading)
	if(!CameraAlreadyLoaded)SetRotMatrix(&SPCameraMATRIX);
	gte_ldv0i(
			   (x-Gcam->mPos.X.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE),
			   (y-Gcam->mPos.Y.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE),
			   (z-Gcam->mPos.Z.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE));
#if defined(JJS)
	gte_rtv0();
#endif
#endif
}




void SetParticleCommonOffset(CParticle* p,CThing*mOwner)
{
	if((!p->mIsRelative)||(!mOwner))
		CommonOffsetCoord(p->mPos.X.mVal,p->mPos.Y.mVal,p->mPos.Z.mVal);
	else
	{
		/*
		 * Moves relative to the owner, so this particles position is an offset...
		 * This should NEVER occur if the owner has died....
		 */
		GVector tpos=mOwner->GetPos();
		//if(!mOwner)printf("This particle says it's relative, but it's owner is NOT VALID!!!\n");
		CommonOffsetCoord(p->mPos.X.mVal+tpos.X.mVal,p->mPos.Y.mVal+tpos.Y.mVal,p->mPos.Z.mVal+tpos.Z.mVal);
	}
}

#define		SVECSCALE	(256.f)


#define RING_SEGS p->uPRing.mSegs
#define RING_STEP p->uPRing.mStep
#define RING_WIDTH p->uPRing.mWidth

// To render RINGS, use a height of 0, for TUBES, pass the HEIGHT of the tube....

// Can move relative to an owner, so need the Thing ptr too






inline ULONG dointerpxylong(SVECTOR *a,SVECTOR *b)
{
	int x=((a->vx+b->vx)>>1);
	int y=((a->vy+b->vy)>>1);
	ULONG retval=(ULONG)((y<<16)|(x&0xffff));

	//printf("A: 0x%08x B: 0x%08x  Ret: 0x%08x\n",*(ULONG*)a,*(ULONG*)b,retval);

	return retval;
}


void	SortOutParticleColour(DWORD &col)
{
	SINT	r = (col >> 16) & 0xff;
	SINT	g = (col >>  8) & 0xff;
	SINT	b = (col      ) & 0xff;
	
/*	if (r > 127) r = 127;
	if (g > 127) g = 127;
	if (b > 127) b = 127;
	col = (b << 17) + (g << 9) + (r << 1);*/

	col = (b << 16) + (g << 8) + r;
}




void	ParticleRingRender(CParticle *p,CThing *mOwner)
{
	long *cols=(long*)(0x1f800000);
	
	SVECTOR vecs[1000];
	FVector	fvecs[1000];
		
	long	width = ((p->mRadius));
	long 	width2 = (((p->mRadius))+RING_WIDTH);
	unsigned long col=p->mColour&0xffffff;
	col=Interpolcol(&col,p->uPRing.mBrightness);

	SortOutParticleColour(col);

	FVector	ppos;
	if((!p->mIsRelative)||(!mOwner))
		ppos = GVectorToFVector(p->mPos);
	else
		ppos = GVectorToFVector(p->mPos + mOwner->GetPos());


	// Create the points for the ring...
	for(int sc=0;sc<RING_SEGS;sc++)
	{
		SWORD segsin=csin(sc*RING_STEP+p->uPRing.mOffs);
		SWORD segcos=ccos(sc*RING_STEP+p->uPRing.mOffs);
		vecs[sc].vx=(segsin*width)>>12;
		vecs[sc].vy=(segcos*width)>>12;
		vecs[sc].vz=0;
		if(p->uPRing.mHeight)
			vecs[sc+(RING_SEGS)].vz=-p->uPRing.mHeight;
		else
			vecs[sc+(RING_SEGS)].vz=0;
		vecs[sc+(RING_SEGS)].vx=(segsin*width2)>>12;
		vecs[sc+(RING_SEGS)].vy=(segcos*width2)>>12;

	}

	SINT tilevert=p->uPRing.mTextureNum&PART_TILEV;

	// JCL - create real verts to transform
	SINT 	c0;
	for (c0 = 0; c0 < RING_SEGS * 2; c0 ++)
	{
		fvecs[c0].X = float(vecs[c0].vx) / SVECSCALE;
		fvecs[c0].Z = float(vecs[c0].vy) / SVECSCALE;
		fvecs[c0].Y = float(vecs[c0].vz) / SVECSCALE;
	}
	
	// transform them
	PersTransWrapper(RING_SEGS * 2,(tvector *)fvecs, (tvector *)fvecs, ppos , ID_FMATRIX);
	
	if(!p->uPRing.mTextureNum)
	{
		kmChangeStripListType			( &GouraudHead, KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &GouraudHead, KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
	 	kmChangeStripTextureShadingMode ( &GouraudHead, KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &GouraudHead, KM_IMAGE_PARAM1, KM_TRUE );
	
		for(int sc=0;sc<RING_SEGS;sc++)
		{
			int p1=(sc+1);
			if(p1==RING_SEGS)p1=0;

			if ((fvecs[sc].Z > 0.f) || (fvecs[p1].Z > 0.f) ||
			    (fvecs[sc + RING_SEGS].Z > 0.f) || (fvecs[p1 + RING_SEGS].Z > 0.f))
			    continue;

			InitCZs();

	       	GouraudS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	GouraudS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	GouraudS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	GouraudS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;


			if(p->uPRing.mHeight)
			{
		       	GouraudS[0].fX                   = fvecs[sc].X;
		       	GouraudS[0].fY                   = fvecs[sc].Y;
		       	GouraudS[0].u.fZ                 = -fvecs[sc].Z;
			
		       	GouraudS[1].fX                   = fvecs[p1].X;
		       	GouraudS[1].fY                   = fvecs[p1].Y;
		       	GouraudS[1].u.fZ                 = -fvecs[p1].Z;
			
		       	GouraudS[2].fX                   = fvecs[sc+(RING_SEGS)].X;
		       	GouraudS[2].fY                   = fvecs[sc+(RING_SEGS)].Y;
		       	GouraudS[2].u.fZ                 = -fvecs[sc+(RING_SEGS)].Z;
			
		       	GouraudS[3].fX                   = fvecs[p1+(RING_SEGS)].X;
		       	GouraudS[3].fY                   = fvecs[p1+(RING_SEGS)].Y;
		       	GouraudS[3].u.fZ                 = -fvecs[p1+(RING_SEGS)].Z;
		    }
		    else
		    {
		       	GouraudS[2].fX                   = fvecs[sc].X;
		       	GouraudS[2].fY                   = fvecs[sc].Y;
		       	GouraudS[2].u.fZ                 = -fvecs[sc].Z;
			
		       	GouraudS[3].fX                   = fvecs[p1].X;
		       	GouraudS[3].fY                   = fvecs[p1].Y;
		       	GouraudS[3].u.fZ                 = -fvecs[p1].Z;
			
		       	GouraudS[0].fX                   = fvecs[sc+(RING_SEGS)].X;
		       	GouraudS[0].fY                   = fvecs[sc+(RING_SEGS)].Y;
		       	GouraudS[0].u.fZ                 = -fvecs[sc+(RING_SEGS)].Z;
			
		       	GouraudS[1].fX                   = fvecs[p1+(RING_SEGS)].X;
		       	GouraudS[1].fY                   = fvecs[p1+(RING_SEGS)].Y;
		       	GouraudS[1].u.fZ                 = -fvecs[p1+(RING_SEGS)].Z;
		    }

	       	GouraudS[0].uBaseRGB.dwPacked    = col;
	       	GouraudS[1].uBaseRGB.dwPacked    = col;
	       	GouraudS[2].uBaseRGB.dwPacked    = 0;
	       	GouraudS[3].uBaseRGB.dwPacked    = 0;
	
			ExitCZs(4);
		}			
	}
	else
	{
		float	adder= (63.f / 256.f) / float(RING_SEGS);   //! assume 256x256 texture page
		SINT tnum=p->uPRing.mTextureNum;
		STextureRefOther *tr=ENGINE.GetParticleTexture(tnum&0x3ff);

		SINT	TN = tr->mTPage + 1;

		kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
	 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );

		for(int sc=0;sc<RING_SEGS;sc++)
		{
			int p1=(sc+1);
			if(p1==RING_SEGS)p1=0;

			if ((fvecs[sc].Z > 0.f) || (fvecs[p1].Z > 0.f) ||
			    (fvecs[sc + RING_SEGS].Z > 0.f) || (fvecs[p1 + RING_SEGS].Z > 0.f))
			    continue;

			FVector 	*x0,*x1,*x2,*x3;

			if(p->uPRing.mHeight)
			{
				x0 =&fvecs[sc];
				x1 =&fvecs[p1];
				x2 =&fvecs[sc+RING_SEGS];
				x3 =&fvecs[p1+RING_SEGS];

			}
			else
			{
				x2 = &fvecs[sc];
				x3 = &fvecs[p1];
				x0 = &fvecs[sc+RING_SEGS];
				x1 = &fvecs[p1+RING_SEGS];
			}

			if(tilevert)
			{
				// Draw 2.
				FVector	xy1 = ((*x0) + (*x2)) / 2.f;
				FVector	xy2 = ((*x1) + (*x3)) / 2.f;
				
				InitCZTs(TN);
				
				float	u0 = tr->mX + 0.002f;
				float	u1 = tr->mX + tr->mW - 0.002f;
				float	v0 = tr->mY + 0.002f;
				float	v1 = tr->mY + tr->mH - 0.002f;
				
		       	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[0].fX                   = x0->X;
		       	TextureS[0].fY                   = x0->Y;
		       	TextureS[0].u.fZ                 = -x0->Z;
		       	TextureS[0].uBaseRGB.dwPacked    = col;
			
		       	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[1].fX                   = x1->X;
		       	TextureS[1].fY                   = x1->Y;
		       	TextureS[1].u.fZ                 = -x1->Z;
		       	TextureS[1].uBaseRGB.dwPacked    = col;
			
		       	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[2].fX                   = xy1.X;
		       	TextureS[2].fY                   = xy1.Y;
		       	TextureS[2].u.fZ                 = -xy1.Z;
		       	TextureS[2].uBaseRGB.dwPacked    = col;
			
		       	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
		       	TextureS[3].fX                   = xy2.X;
		       	TextureS[3].fY                   = xy2.Y;
		       	TextureS[3].u.fZ                 = -xy2.Z;
		       	TextureS[3].uBaseRGB.dwPacked    = col;
				
				if(tnum&PART_TILEH)
				{
			       	TextureS[0].fU                   = u0;
			       	TextureS[0].fV                   = v0;
			       	TextureS[1].fU                   = u1;
			       	TextureS[1].fV                   = v0;
			       	TextureS[2].fU                   = u0;
			       	TextureS[2].fV                   = v1;
			       	TextureS[3].fU                   = u1;
			       	TextureS[3].fV                   = v1;				
		       	}
		       	else
		       	{
			       	TextureS[0].fU                   = u0 + (float(sc) * adder);
			       	TextureS[0].fV                   = v0;
			       	TextureS[1].fU                   = u0 + (float(sc) * adder) + adder;
			       	TextureS[1].fV                   = v0;
			       	TextureS[2].fU                   = u0 + (float(sc) * adder);
			       	TextureS[2].fV                   = v1;
			       	TextureS[3].fU                   = u0 + (float(sc) * adder) + adder;
			       	TextureS[3].fV                   = v1;				
				}
				
				ExitCZTs(4);	
				
				// and again
							
				InitCZTs(TN);
				
		       	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[0].fX                   = xy1.X;
		       	TextureS[0].fY                   = xy1.Y;
		       	TextureS[0].u.fZ                 = -xy1.Z;
		       	TextureS[0].uBaseRGB.dwPacked    = col;
			
		       	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[1].fX                   = xy2.X;
		       	TextureS[1].fY                   = xy2.Y;
		       	TextureS[1].u.fZ                 = -xy2.Z;
		       	TextureS[1].uBaseRGB.dwPacked    = col;
			
		       	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[2].fX                   = x2->X;
		       	TextureS[2].fY                   = x2->Y;
		       	TextureS[2].u.fZ                 = -x2->Z;
		       	TextureS[2].uBaseRGB.dwPacked    = 0;
			
		       	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
		       	TextureS[3].fX                   = x3->X;
		       	TextureS[3].fY                   = x3->Y;
		       	TextureS[3].u.fZ                 = -x3->Z;
		       	TextureS[3].uBaseRGB.dwPacked    = 0;
				
				if(tnum&PART_TILEH)
				{
			       	TextureS[0].fU                   = u0;
			       	TextureS[0].fV                   = v0;
			       	TextureS[1].fU                   = u1;
			       	TextureS[1].fV                   = v0;
			       	TextureS[2].fU                   = u0;
			       	TextureS[2].fV                   = v1;
			       	TextureS[3].fU                   = u1;
			       	TextureS[3].fV                   = v1;				
		       	}
		       	else
		       	{
			       	TextureS[0].fU                   = u0 + (float(sc) * adder);
			       	TextureS[0].fV                   = v0;
			       	TextureS[1].fU                   = u0 + (float(sc) * adder) + adder;
			       	TextureS[1].fV                   = v0;
			       	TextureS[2].fU                   = u0 + (float(sc) * adder);
			       	TextureS[2].fV                   = v1;
			       	TextureS[3].fU                   = u0 + (float(sc) * adder) + adder;
			       	TextureS[3].fV                   = v1;				
				}
				
				ExitCZTs(4);				
			}
			else
			{
				InitCZTs(TN);
				
				float	u0 = tr->mX + 0.002f;
				float	u1 = tr->mX + tr->mW - 0.002f;
				float	v0 = tr->mY + 0.002f;
				float	v1 = tr->mY + tr->mH - 0.002f;
				
		       	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[0].fX                   = x0->X;
		       	TextureS[0].fY                   = x0->Y;
		       	TextureS[0].u.fZ                 = -x0->Z;
		       	TextureS[0].uBaseRGB.dwPacked    = col;
			
		       	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[1].fX                   = x1->X;
		       	TextureS[1].fY                   = x1->Y;
		       	TextureS[1].u.fZ                 = -x1->Z;
		       	TextureS[1].uBaseRGB.dwPacked    = col;
			
		       	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[2].fX                   = x2->X;
		       	TextureS[2].fY                   = x2->Y;
		       	TextureS[2].u.fZ                 = -x2->Z;
		       	TextureS[2].uBaseRGB.dwPacked    = 0;
			
		       	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
		       	TextureS[3].fX                   = x3->X;
		       	TextureS[3].fY                   = x3->Y;
		       	TextureS[3].u.fZ                 = -x3->Z;
		       	TextureS[3].uBaseRGB.dwPacked    = 0;
				
				if(tnum&PART_TILEH)
				{
			       	TextureS[0].fU                   = u0;
			       	TextureS[0].fV                   = v0;
			       	TextureS[1].fU                   = u1;
			       	TextureS[1].fV                   = v0;
			       	TextureS[2].fU                   = u0;
			       	TextureS[2].fV                   = v1;
			       	TextureS[3].fU                   = u1;
			       	TextureS[3].fV                   = v1;				
		       	}
		       	else
		       	{
			       	TextureS[0].fU                   = u0 + (float(sc) * adder);
			       	TextureS[0].fV                   = v0;
			       	TextureS[1].fU                   = u0 + (float(sc) * adder) + adder;
			       	TextureS[1].fV                   = v0;
			       	TextureS[2].fU                   = u0 + (float(sc) * adder);
			       	TextureS[2].fV                   = v1;
			       	TextureS[3].fU                   = u0 + (float(sc) * adder) + adder;
			       	TextureS[3].fV                   = v1;				
				}
				
				ExitCZTs(4);	
			}
		}
	}
}

#define ftoimv(x)  ((int)((x*32)/5.4017))

#define MESH_VERTEX(x,y,z) {ftoimv(x),ftoimv(y),-ftoimv(z)}
#define MESH_FACE(a,b,c) {a,b,c,0}

#define SPHERE_NUMVERTS 26
#define SPHERE_NUMFACES 40
		//MESH_NUMVERTEX 26
		//MESH_NUMFACES 40
struct SVECTOR hemiverts[26]= {
		MESH_VERTEX(0.0000 ,0.0000	,5.4017),

		MESH_VERTEX(2.8398 ,0.0000	,4.5949),
		MESH_VERTEX(0.8776 ,2.7008	,4.5949),
		MESH_VERTEX(-2.2975,1.6692	,4.5949),
		MESH_VERTEX(-2.2975,-1.6692,4.5949),
		MESH_VERTEX(0.8776 ,-2.7008,4.5949),

		MESH_VERTEX(4.8314 ,0.0000	,2.4157),
		MESH_VERTEX(1.4930 ,4.5949	,2.4157),
		MESH_VERTEX(-3.9087,2.8398	,2.4157),
		MESH_VERTEX(-3.9087,-2.8398,2.4157),
		MESH_VERTEX(1.4930 ,-4.5949,2.4157),

		MESH_VERTEX(3.7174 ,2.7008	,2.8398),
		MESH_VERTEX(-1.4199,4.3700	,2.8398),
		MESH_VERTEX(-4.5949,-0.0000,2.8398),
		MESH_VERTEX(-1.4199,-4.3700,2.8398),
		MESH_VERTEX(3.7174 ,-2.7008,2.8398),

		MESH_VERTEX(5.1373 ,1.6692	,0.0000),
		MESH_VERTEX(3.1750 ,4.3700	,0.0000),
		MESH_VERTEX(-0.0000,5.4017	,0.0000),
		MESH_VERTEX(-3.1750,4.3700	,0.0000),
		MESH_VERTEX(-5.1373,1.6692	,0.0000),
		MESH_VERTEX(-5.1373,-1.6692,0.0000),
		MESH_VERTEX(-3.1750,-4.3700,0.0000),
		MESH_VERTEX(0.0000 ,-5.4017,0.0000),
		MESH_VERTEX(3.1750 ,-4.3700,0.0000),
		MESH_VERTEX(5.1373 ,-1.6692,0.0000)
};

struct  {
	char v1,v2,v3,pad;
}MFaces[40]={
		MESH_FACE	(0  ,  1  ,  2 ),
		MESH_FACE	(0  ,  2  ,  3 ),
		MESH_FACE	(0  ,  3  ,  4 ),
		MESH_FACE	(0  ,  4  ,  5 ),
		MESH_FACE	(0  ,  5  ,  1 ),
		MESH_FACE	(11 ,  2  ,  1 ),
		MESH_FACE	(12 ,  3  ,  2 ),
		MESH_FACE	(13 ,  4  ,  3 ),
		MESH_FACE	(14 ,  5  ,  4 ),
		MESH_FACE	(15 ,  1  ,  5 ),
		MESH_FACE	(1  , 15  ,  6 ),
		MESH_FACE	(1  ,  6  , 11 ),
		MESH_FACE	(2  , 11  ,  7 ),
		MESH_FACE	(2  ,  7  , 12 ),
		MESH_FACE	(3  , 12  ,  8 ),
		MESH_FACE	(3  ,  8  , 13 ),
		MESH_FACE	(4  , 13  ,  9 ),
		MESH_FACE	(4  ,  9  , 14 ),
		MESH_FACE	(5  , 14  , 10 ),
		MESH_FACE	(5  , 10  , 15 ),
		MESH_FACE	(16 , 11  ,  6 ),
		MESH_FACE	(17 ,  7  , 11 ),
		MESH_FACE	(18 , 12  ,  7 ),
		MESH_FACE	(19 ,  8  , 12 ),
		MESH_FACE	(20 , 13  ,  8 ),
		MESH_FACE	(21 ,  9  , 13 ),
		MESH_FACE	(22 , 14  ,  9 ),
		MESH_FACE	(23 , 10  , 14 ),
		MESH_FACE	(24 , 15  , 10 ),
		MESH_FACE	(25 ,  6  , 15 ),
		MESH_FACE	(6  , 25  , 16 ),
		MESH_FACE	(11 , 16  , 17 ),
		MESH_FACE	(7  , 17  , 18 ),
		MESH_FACE	(12 , 18  , 19 ),
		MESH_FACE	(8  , 19  , 20 ),
		MESH_FACE	(13 , 20  , 21 ),
		MESH_FACE	(9  , 21  , 22 ),
		MESH_FACE	(14 , 22  , 23 ),
		MESH_FACE	(10 , 23  , 24 ),
		MESH_FACE	(15 , 24  , 25 )
};
#define NUM_LVERTS	106
#undef ftoimv
#undef MESH_VERTEX
#define ftoimv(x)  ((int)(((x)*32)/140))
#define MESH_VERTEX(x,y,z) {ftoimv((x)),ftoimv((y)),-ftoimv(z)}


// All lightning starts from point 0 in this list, then choose a random number (0-20) and use that as the base for a
//	meandering line.

struct SVECTOR lightning_verts[NUM_LVERTS]= {

			MESH_VERTEX(0.0000		,4.9533		,955.9452),



			MESH_VERTEX(-57.5190	,386.5674	,867.8605),
			MESH_VERTEX(-167.4463	,352.6594	,867.8605),
			MESH_VERTEX(-262.4953	,287.8561	,867.8605),
			MESH_VERTEX(-334.2204	,197.9157	,867.8605),
			MESH_VERTEX(-376.2486	,90.8297	,867.8605),
			MESH_VERTEX(-384.8454	,-23.8868	,867.8605),
			MESH_VERTEX(-359.2470	,-136.0407	,867.8605),
			MESH_VERTEX(-301.7280	,-235.6666	,867.8605),
			MESH_VERTEX(-217.3990	,-313.9124	,867.8605),
			MESH_VERTEX(-113.7533	,-363.8257	,867.8605),
			MESH_VERTEX(-0.0000		,-380.9712	,867.8605),
			MESH_VERTEX(113.7531	,-363.8257	,867.8605),
			MESH_VERTEX(217.3989	,-313.9125	,867.8605),
			MESH_VERTEX(301.7279	,-235.6667	,867.8605),
			MESH_VERTEX(359.2470	,-136.0408	,867.8605),
			MESH_VERTEX(384.8454	,-23.8869	,867.8605),
			MESH_VERTEX(376.2486	,90.8295	,867.8605),
			MESH_VERTEX(334.2205	,197.9155	,867.8605),
			MESH_VERTEX(262.4954	,287.8560	,867.8605),
			MESH_VERTEX(167.4464	,352.6593	,867.8605),
			MESH_VERTEX(57.5191		,386.5674	,867.8605),

			MESH_VERTEX(-90.1690	,603.1863	,718.5042),
			MESH_VERTEX(-262.4953	,550.0306	,718.5042),
			MESH_VERTEX(-411.4977	,448.4426	,718.5042),
			MESH_VERTEX(-523.9368	,307.4485	,718.5042),
			MESH_VERTEX(-589.8217	,139.5764	,718.5041),
			MESH_VERTEX(-603.2985	,-40.2575	,718.5041),
			MESH_VERTEX(-563.1694	,-216.0743	,718.5041),
			MESH_VERTEX(-473.0003	,-372.2518	,718.5041),
			MESH_VERTEX(-340.8031	,-494.9129	,718.5040),
			MESH_VERTEX(-178.3240	,-573.1587	,718.5040),
			MESH_VERTEX(-0.0000		,-600.0367	,718.5040),
			MESH_VERTEX(178.3238	,-573.1588	,718.5040),
			MESH_VERTEX(340.8029	,-494.9130	,718.5040),
			MESH_VERTEX(473.0002	,-372.2519	,718.5041),
			MESH_VERTEX(563.1694	,-216.0745	,718.5041),
			MESH_VERTEX(603.2984	,-40.2578	,718.5041),
			MESH_VERTEX(589.8218	,139.5762	,718.5041),
			MESH_VERTEX(523.9369	,307.4483	,718.5042),
			MESH_VERTEX(411.4979	,448.4424	,718.5042),
			MESH_VERTEX(262.4955	,550.0306	,718.5042),
			MESH_VERTEX(90.1692		,603.1862	,718.5042),


			MESH_VERTEX(-114.8071	,766.6495	,511.2126),
			MESH_VERTEX(-334.2204	,698.9695	,511.2126),
			MESH_VERTEX(-523.9368	,569.6230	,511.2126),
			MESH_VERTEX(-667.0991	,390.1033	,511.2125),
			MESH_VERTEX(-750.9866	,176.3613	,511.2125),
			MESH_VERTEX(-768.1458	,-52.6111	,511.2125),
			MESH_VERTEX(-717.0518	,-276.4686	,511.2125),
			MESH_VERTEX(-602.2446	,-475.3206	,511.2125),
			MESH_VERTEX(-433.9253	,-631.4980	,511.2124),
			MESH_VERTEX(-227.0499	,-731.1240	,511.2124),
			MESH_VERTEX(-0.0000		,-765.3463	,511.2124),
			MESH_VERTEX(227.0496	,-731.1241	,511.2124),
			MESH_VERTEX(433.9251	,-631.4982	,511.2124),
			MESH_VERTEX(602.2444	,-475.3208	,511.2125),
			MESH_VERTEX(717.0517	,-276.4689	,511.2125),
			MESH_VERTEX(768.1458	,-52.6114	,511.2125),
			MESH_VERTEX(750.9867	,176.3610	,511.2125),
			MESH_VERTEX(667.0992	,390.1031	,511.2125),
			MESH_VERTEX(523.9370	,569.6229	,511.2126),
			MESH_VERTEX(334.2206	,698.9694	,511.2126),
			MESH_VERTEX(114.8074	,766.6494	,511.2126),



			MESH_VERTEX(-129.2440	,862.4326	,264.4045),
			MESH_VERTEX(-376.2486	,786.2419	,264.4045),
			MESH_VERTEX(-589.8217	,640.6302	,264.4045),
			MESH_VERTEX(-750.9866	,438.5359	,264.4045),
			MESH_VERTEX(-845.4231	,197.9158	,264.4044),
			MESH_VERTEX(-864.7400	,-59.8498	,264.4044),
			MESH_VERTEX(-807.2209	,-311.8574	,264.4044),
			MESH_VERTEX(-677.9767	,-535.7150	,264.4044),
			MESH_VERTEX(-488.4913	,-711.5317	,264.4044),
			MESH_VERTEX(-255.6013	,-823.6857	,264.4044),
			MESH_VERTEX(-0.0000		,-862.2114	,264.4044),
			MESH_VERTEX(255.6010	,-823.6857	,264.4044),
			MESH_VERTEX(488.4911	,-711.5319	,264.4044),
			MESH_VERTEX(677.9766	,-535.7152	,264.4044),
			MESH_VERTEX(807.2208	,-311.8577	,264.4044),
			MESH_VERTEX(864.7399	,-59.8501	,264.4044),
			MESH_VERTEX(845.4232	,197.9155	,264.4044),
			MESH_VERTEX(750.9868	,438.5356	,264.4045),
			MESH_VERTEX(589.8220	,640.6300	,264.4045),
			MESH_VERTEX(376.2488	,786.2417	,264.4045),
			MESH_VERTEX(129.2444	,862.4326	,264.4045),


			MESH_VERTEX(-132.1971	,882.0250	,0.0000),
			MESH_VERTEX(-384.8454	,804.0934	,0.0000),
			MESH_VERTEX(-603.2984	,655.1547	,0.0000),
			MESH_VERTEX(-768.1458	,448.4427	,0.0000),
			MESH_VERTEX(-864.7400	,202.3248	,0.0000),
			MESH_VERTEX(-884.4982	,-61.3305	,0.0000),
			MESH_VERTEX(-825.6649	,-319.0961	,0.0000),
			MESH_VERTEX(-693.4677	,-548.0685	,0.0000),
			MESH_VERTEX(-499.6528	,-727.9025	,0.0000),
			MESH_VERTEX(-261.4415	,-842.6190	,0.0000),
			MESH_VERTEX(-0.0000		,-882.0250	,0.0000),
			MESH_VERTEX(261.4412	,-842.6191	,0.0000),
			MESH_VERTEX(499.6525	,-727.9026	,0.0000),
			MESH_VERTEX(693.4675	,-548.0687	,0.0000),
			MESH_VERTEX(825.6648	,-319.0964	,0.0000),
			MESH_VERTEX(884.4982	,-61.3308	,0.0000),
			MESH_VERTEX(864.7401	,202.3245	,0.0000),
			MESH_VERTEX(768.1459	,448.4424	,0.0000),
			MESH_VERTEX(603.2986	,655.1545	,0.0000),
			MESH_VERTEX(384.8457	,804.0933	,0.0000),
			MESH_VERTEX(132.1974	,882.0250	,0.0000)
};
//extern void SetLightMatrix(void);


#define 	LINE_WIDTH	0.05f

//******************************************************************************
void	RenderLine(FVector s, FVector e, DWORD col1, DWORD col2, FVector &pos, FMatrix &ori, BOOL alpha = TRUE)
{
	// JCL this is probably the slowest line draw ever written

	SINT c0, c1, c2;
	
	SortOutParticleColour(col1);
	SortOutParticleColour(col2);

	// work out how many segments we need
	SINT	num_segs = 1;
	FVector	seg_offset = (e - s);
	FVector perp = seg_offset ^ (GMatrixToFMatrix(ENGINE.GetCamera()->mOrientation) * FVector(0, 0, 1.f));

	if (perp.Magnitude() > 0.0015)
	{
		perp.Normalise();
		perp *= LINE_WIDTH / 2.f;
	}
	else
		perp = FVector(0, 0, LINE_WIDTH / 2.f);

	FVector	curr_pos = s;

	if (alpha)
	{
		kmChangeStripListType			( &GouraudHead, KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &GouraudHead, KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
	 	kmChangeStripTextureShadingMode ( &GouraudHead, KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &GouraudHead, KM_IMAGE_PARAM1, KM_TRUE );
	}
	else
	{
		kmChangeStripListType			( &GouraudHead, KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &GouraudHead, KM_IMAGE_PARAM1, KM_ONE, KM_ZERO);
	 	kmChangeStripTextureShadingMode ( &GouraudHead, KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &GouraudHead, KM_IMAGE_PARAM1, KM_FALSE );
	}

	InitCZs();

	FVector	verts[2][2];
	
	verts[0][0] = curr_pos - perp;
	verts[0][1] = curr_pos + perp;
	
	curr_pos = e;
		
	verts[1][0] = curr_pos - perp;
	verts[1][1] = curr_pos + perp;
	
	ENGINE.SwapYZ((FVector *)verts, 4);
	
	PersTransWrapper(4,(tvector *)verts, (tvector *)verts, pos , ori);

   
	if ((verts[0][0].Z > 0.f) || (verts[0][1].Z > 0.f) || (verts[1][0].Z > 0.f) || (verts[1][1].Z > 0.f))
	    return;

   	GouraudS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	GouraudS[0].fX                   = verts[0][0].X;
   	GouraudS[0].fY                   = verts[0][0].Y;
   	GouraudS[0].u.fZ                 = -verts[0][0].Z;
   	GouraudS[0].uBaseRGB.dwPacked    = col1;

   	GouraudS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	GouraudS[1].fX                   = verts[0][1].X;
   	GouraudS[1].fY                   = verts[0][1].Y;
   	GouraudS[1].u.fZ                 = -verts[0][1].Z;
   	GouraudS[1].uBaseRGB.dwPacked    = col1;

   	GouraudS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	GouraudS[2].fX                   = verts[1][0].X;
   	GouraudS[2].fY                   = verts[1][0].Y;
   	GouraudS[2].u.fZ                 = -verts[1][0].Z;
   	GouraudS[2].uBaseRGB.dwPacked    = col2;

   	GouraudS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	GouraudS[3].fX                   = verts[1][1].X;
   	GouraudS[3].fY                   = verts[1][1].Y;
   	GouraudS[3].u.fZ                 = -verts[1][1].Z;
   	GouraudS[3].uBaseRGB.dwPacked    = col2;

	ExitCZs(4);
}

//****************************************************************************************

#define LIGHTNING_NUMLINES	5
#define LIGHTNING_NUMVERTS	6

int Rval;

// Eg. Range 32 gives int from 0-31
int FXRand(SINT range)
{
	if (!(GAME.IsGamePanelActive()))Rval=((Rval*5)+1)&65535;
	return (Rval*range)>>16;
}

void RenderLightning(CParticle *p,CThing *mOwner)
{
	if(!mOwner)return;

	long col=Interpolcol(&p->mColour,p->uPRing.mBrightness);

	FVector	ppos;
	if((!p->mIsRelative)||(!mOwner))
		ppos = GVectorToFVector(p->mPos);
	else
		ppos = GVectorToFVector(p->mPos + mOwner->GetPos());

	CCharacter *c=mOwner->IsCharacter();
	if(!c)return;


	FMatrix ori = GMatrixToFMatrix(c->GetRenderOrientation(1));

	SVECTOR vecs[20];

	for(int zz=0;zz<5;zz++)
	{
		vecs[0]=lightning_verts[0];
		int i,r;
		r=FXRand(21);
		for(i=0;i<5;i++)
		{
			if(FXRand(2))r=r+1;
			else if(FXRand(2))r=r-1;
			if(r<0)r=20;
			if(r>20)r=0;
			//printf("R: %d\n",r);
			vecs[i+1]=lightning_verts[r+(i*21)+1];//*(1+(p->mRadius));
		}

		unsigned long c=0xff7f00;

		FVector	fvecs[20];
		// JCL - create real verts to transform
		SINT 	c0;
		for (c0 = 0; c0 < 6; c0 ++)
		{
			fvecs[c0].X = float(vecs[c0].vx) / SVECSCALE;
			fvecs[c0].Y = float(vecs[c0].vy) / SVECSCALE;
			fvecs[c0].Z = float(vecs[c0].vz) / SVECSCALE;
		}

		for(int i=0;i<5;i++)
		{
			RenderLine(fvecs[i], fvecs[i + 1], c, c, ppos, ori);
		}
	}
}

void RenderAlphaShpere(CParticle *p)
{
	SVECTOR	vecs[100];
	for(int i=0;i<SPHERE_NUMVERTS;i++)
	{
		vecs[i].vx=hemiverts[i].vx*(1+(p->mRadius));
		vecs[i].vy=hemiverts[i].vy*(1+(p->mRadius));
		vecs[i].vz=hemiverts[i].vz*(1+(p->mRadius));
	}

	DWORD c=Interpolcol(&p->mColour,p->uPHemi.mBrightness);
	SortOutParticleColour(c);


	// JCL - create real verts to transform
	FVector	fvecs[100];
	SINT 	c0;
	for (c0 = 0; c0 < SPHERE_NUMVERTS; c0 ++)
	{
		fvecs[c0].X = float(vecs[c0].vx) / SVECSCALE;
		fvecs[c0].Z = float(vecs[c0].vy) / SVECSCALE;
		fvecs[c0].Y = float(vecs[c0].vz) / SVECSCALE;
	}
	
	// transform them
	PersTransWrapper(SPHERE_NUMVERTS, (tvector *)fvecs, (tvector *)fvecs, GVectorToFVector(p->mPos) , ID_FMATRIX);

	kmChangeStripListType			( &GouraudHead, KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &GouraudHead, KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	kmChangeStripTextureShadingMode ( &GouraudHead, KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &GouraudHead, KM_IMAGE_PARAM1, KM_TRUE );
	
	for(int i=0;i<SPHERE_NUMFACES;i++)
	{
		if ((fvecs[MFaces[i].v1].Z > 0.f) || (fvecs[MFaces[i].v2].Z > 0.f) || (fvecs[MFaces[i].v3].Z > 0.f))
		    continue;

		InitCZs();

       	GouraudS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	GouraudS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	GouraudS[2].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;

       	GouraudS[0].fX                   = fvecs[MFaces[i].v1].X;
       	GouraudS[0].fY                   = fvecs[MFaces[i].v1].Y;
       	GouraudS[0].u.fZ                 = -fvecs[MFaces[i].v1].Z;
	
       	GouraudS[1].fX                   = fvecs[MFaces[i].v2].X;
       	GouraudS[1].fY                   = fvecs[MFaces[i].v2].Y;
       	GouraudS[1].u.fZ                 = -fvecs[MFaces[i].v2].Z;
	
       	GouraudS[2].fX                   = fvecs[MFaces[i].v3].X;
       	GouraudS[2].fY                   = fvecs[MFaces[i].v3].Y;
       	GouraudS[2].u.fZ                 = -fvecs[MFaces[i].v3].Z;
	
       	GouraudS[0].uBaseRGB.dwPacked    = c;
       	GouraudS[1].uBaseRGB.dwPacked    = c;
       	GouraudS[2].uBaseRGB.dwPacked    = c;

		ExitCZs(3);
	}
}


void RenderAlphaSpark(CParticle *p, BOOL alpha, SINT Queuelength,CThing *mOwner)
{
	unsigned long col=Interpolcol(&p->mColour,p->uPSpark.mBrightness);

	FVector	ppos;
	if((!p->mIsRelative)||(!mOwner))
		ppos = GVectorToFVector(p->mPos);
	else
		ppos = GVectorToFVector(p->mPos + mOwner->GetPos());

	int colscale=0x1000/(Queuelength-1);

	FVector	fvecs[20];
	// JCL - create real verts to transform
	SINT 	c0;
	for (c0 = 0; c0 < Queuelength; c0 ++)
	{
		fvecs[c0].X = float(p->uPSpark.mQueue[c0].vx) / SVECSCALE;
		fvecs[c0].Y = float(p->uPSpark.mQueue[c0].vy) / SVECSCALE;
		fvecs[c0].Z = float(p->uPSpark.mQueue[c0].vz) / SVECSCALE;
	}

	for(int c=0;c<Queuelength-1;c++)
	{
		SINT	col1 = Interpolcol(&col, short(((Queuelength -  c     ) * 4096) / Queuelength));
		SINT	col2 = Interpolcol(&col, short(((Queuelength - (c + 1)) * 4096) / Queuelength));
	
		RenderLine(fvecs[c], fvecs[c + 1], col1, col2, ppos, ID_FMATRIX, alpha);
	}
}


#endif //TARGET == PSX
