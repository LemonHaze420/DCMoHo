#include	"Common.h"

#if TARGET == PC

#include	"RTDecal.h"
#include	"RTParticleSystem.h"
#include	"Globals.h"
#include	"ParticleSystem.h"
#include	"Character.h"

// remember to add these to the resource accumulators at the bottom of the file.
CTextureGTEXAdditive	*CRTParticleSystem::mTexFire1 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexFire2 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexFire3 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexZ	  = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexStar1 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexStar2 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp00 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp01 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp02 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp03 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp04 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp05 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp06 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp07 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp08 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp09 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp10 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp11 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp12 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp13 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp14 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexExp15 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexWat00 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexWat01 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexWat02 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexWat03 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexWat04 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexWat05 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexBubble = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexRing = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexSStar1 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexSStar2 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexSmoke = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexShockwave = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexHit1 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexHit2 = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexInner = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexMiddle = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexOuter = NULL;
CTextureGTEXAdditive	*CRTParticleSystem::mTexFlashSM = NULL;

CTextureGTEXAdditive			*CRTParticleSystem::mTexWhite = NULL;

//******************************************************************************************
void	CRTParticleSystem::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	// check static textures
	if (mTexFire1 == NULL)		mTexFire1 = CTextureGTEXAdditive::GetTexture("fire1.tga");
	if (mTexFire2 == NULL)		mTexFire2 = CTextureGTEXAdditive::GetTexture("fire2.tga");
	if (mTexFire3 == NULL)		mTexFire3 = CTextureGTEXAdditive::GetTexture("fire3.tga");

	if (mTexZ	  == NULL)		mTexZ     = CTextureGTEXAdditive::GetTexture("z.tga");

	if(mTexStar1 == NULL)		mTexStar1 = CTextureGTEXAdditive::GetTexture("particles\\star.tga");
	if(mTexStar2 == NULL)		mTexStar2 = CTextureGTEXAdditive::GetTexture("particles\\star2.tga");

	if(mTexExp00==NULL)			mTexExp00 = CTextureGTEXAdditive::GetTexture("particles\\exxp0000.tga");
	if(mTexExp01==NULL)			mTexExp01 = CTextureGTEXAdditive::GetTexture("particles\\exxp0001.tga");
	if(mTexExp02==NULL)			mTexExp02 = CTextureGTEXAdditive::GetTexture("particles\\exxp0002.tga");
	if(mTexExp03==NULL)			mTexExp03 = CTextureGTEXAdditive::GetTexture("particles\\exxp0003.tga");
	if(mTexExp04==NULL)			mTexExp04 = CTextureGTEXAdditive::GetTexture("particles\\exxp0004.tga");
	if(mTexExp05==NULL)			mTexExp05 = CTextureGTEXAdditive::GetTexture("particles\\exxp0005.tga");
	if(mTexExp06==NULL)			mTexExp06 = CTextureGTEXAdditive::GetTexture("particles\\exxp0006.tga");
	if(mTexExp07==NULL)			mTexExp07 = CTextureGTEXAdditive::GetTexture("particles\\exxp0007.tga");
	if(mTexExp08==NULL)			mTexExp08 = CTextureGTEXAdditive::GetTexture("particles\\exxp0008.tga");
	if(mTexExp09==NULL)			mTexExp09 = CTextureGTEXAdditive::GetTexture("particles\\exxp0009.tga");

	if(mTexExp10==NULL)			mTexExp10 = CTextureGTEXAdditive::GetTexture("particles\\exxp0010.tga");
	if(mTexExp11==NULL)			mTexExp11 = CTextureGTEXAdditive::GetTexture("particles\\exxp0011.tga");
	if(mTexExp12==NULL)			mTexExp12 = CTextureGTEXAdditive::GetTexture("particles\\exxp0012.tga");
	if(mTexExp13==NULL)			mTexExp13 = CTextureGTEXAdditive::GetTexture("particles\\exxp0013.tga");
	if(mTexExp14==NULL)			mTexExp14 = CTextureGTEXAdditive::GetTexture("particles\\exxp0014.tga");
	if(mTexExp15==NULL)			mTexExp15 = CTextureGTEXAdditive::GetTexture("particles\\exxp0015.tga");
	if(mTexWat00==NULL)			mTexWat00 = CTextureGTEXAdditive::GetTexture("particles\\watt_1.tga");
	if(mTexWat01==NULL)			mTexWat01 = CTextureGTEXAdditive::GetTexture("particles\\watt_2.tga");
	if(mTexWat02==NULL)			mTexWat02 = CTextureGTEXAdditive::GetTexture("particles\\watt_3.tga");
	if(mTexWat03==NULL)			mTexWat03 = CTextureGTEXAdditive::GetTexture("particles\\watt_4.tga");
	if(mTexWat04==NULL)			mTexWat04 = CTextureGTEXAdditive::GetTexture("particles\\watt_5.tga");
	if(mTexWat05==NULL)			mTexWat05 = CTextureGTEXAdditive::GetTexture("particles\\watt_6.tga");
	if(mTexBubble==NULL)		mTexBubble = CTextureGTEXAdditive::GetTexture("particles\\Bubble.tga");
	if(mTexRing==NULL)			mTexRing = CTextureGTEXAdditive::GetTexture("particles\\smallring.tga");
	if(mTexSStar1 == NULL)		mTexSStar1 = CTextureGTEXAdditive::GetTexture("particles\\sstar.tga");
	if(mTexSStar2 == NULL)		mTexSStar2 = CTextureGTEXAdditive::GetTexture("particles\\sstar2.tga");
	if(mTexSStar2 == NULL)		mTexSStar2 = CTextureGTEXAdditive::GetTexture("particles\\sstar2.tga");
	if(mTexSmoke == NULL)		mTexSmoke = CTextureGTEXAdditive::GetTexture("particles\\smoke.tga");
	if(mTexShockwave == NULL)	mTexShockwave = CTextureGTEXAdditive::GetTexture("particles\\shock.tga");
	if(mTexHit1 == NULL)		mTexHit1 = CTextureGTEXAdditive::GetTexture("h-blast.tga");
	if(mTexHit2 == NULL)		mTexHit2 = CTextureGTEXAdditive::GetTexture("h-blast2.tga");
	if(mTexInner == NULL)		mTexInner = CTextureGTEXAdditive::GetTexture("particles\\inner.tga");
	if(mTexMiddle == NULL)		mTexMiddle = CTextureGTEXAdditive::GetTexture("particles\\middle.tga");
	if(mTexOuter == NULL)		mTexOuter = CTextureGTEXAdditive::GetTexture("particles\\Outer.tga");
	if(mTexFlashSM == NULL)		mTexFlashSM = CTextureGTEXAdditive::GetTexture("particles\\flash_sm.tga");

	if(mTexWhite == NULL)		mTexWhite = CTextureGTEXAdditive::GetTexture("white.tga");
}


#define	SVECSCALE	(256.f)
#define RING_SEGS	p->uPRing.mSegs
#define RING_STEP	p->uPRing.mStep
#define RING_WIDTH	p->uPRing.mWidth

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

//******************************************************************************************
long Interpolcol(unsigned long *col,short mult)
{
	// interpolate colour with 4096 multiplier
	
	SINT	r = ((*col) >> 16) & 0xff;
	SINT	g = ((*col) >>  8) & 0xff;
	SINT	b = ((*col)      ) & 0xff;
	
	if (mult > 4096) mult = 4096;
	
	r = (r * mult) >> 12;
	g = (g * mult) >> 12;
	b = (b * mult) >> 12;
	
	return (r << 16) + (g << 8) + b;
}	

//******************************************************************************************
void ParticleRingRender(CParticle *p,CThing *mOwner) 
{
	long *cols=(long*)(0x1f800000);
	
	SVECTOR vecs[1000];
	FVector	fvecs[1000];

	CFVF_GTEX		vert;
	SINT			i[4];
		
	long	width = ((p->mRadius));
	long 	width2 = (((p->mRadius))+RING_WIDTH);
	unsigned long col=p->mColour&0xffffff;
	//col=p->uPRing.mBrightness;//
	col = Interpolcol(&col,p->uPRing.mBrightness);
	SortOutParticleColour(col);

	FVector	ppos;
	if((!p->mIsRelative)||(!mOwner))
		ppos = GVectorToFVector(p->mPos);
	else
		ppos = GVectorToFVector(p->mPos + mOwner->GetRenderPos());


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
		fvecs[c0].Y = float(vecs[c0].vy) / SVECSCALE;
		fvecs[c0].Z = float(vecs[c0].vz) / SVECSCALE;
	}
	
	ENGINE.SetupTransforms(ppos, ID_FMATRIX);
	
	if(!p->uPRing.mTextureNum)
	{
		CTextureGTEXAdditive	*tr = CRTParticleSystem::mTexWhite;
		if (!tr)
			return;

		for(int sc=0;sc<RING_SEGS;sc++)
		{
			int p1=(sc+1);
			if(p1==RING_SEGS)
				p1=0;

//			if ((fvecs[sc].Z > 0.f) || (fvecs[p1].Z > 0.f) ||
//			    (fvecs[sc + RING_SEGS].Z > 0.f) || (fvecs[p1 + RING_SEGS].Z > 0.f))
//			    continue;
		
			/*
	       	GouraudS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	GouraudS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	GouraudS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	GouraudS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
			*/

			if(p->uPRing.mHeight)
			{
				vert.mPos		= fvecs[sc];
				vert.mDiffuse	= col;
				vert.mU			= 0.0f;
				vert.mV			= 0.0f;
				i[0] = tr->AddVertex(&vert);

				vert.mPos		= fvecs[p1];
				vert.mDiffuse	= col;
				vert.mU			= 0.0f;
				vert.mV			= 1.0f;
				i[1] = tr->AddVertex(&vert);

				vert.mPos		= fvecs[sc + (RING_SEGS)];
				vert.mDiffuse	= 0;
				vert.mU			= 1.0f;
				vert.mV			= 0.0f;
				i[2] = tr->AddVertex(&vert);

				vert.mPos		= fvecs[p1 + (RING_SEGS)];
				vert.mDiffuse	= 0;
				vert.mU			= 1.0f;
				vert.mV			= 1.0f;
				i[3] = tr->AddVertex(&vert);
			
				tr->AddTri(i[0], i[3], i[1]);
				tr->AddTri(i[0], i[2], i[3]);

				tr->AddTri(i[0], i[1], i[3]);
				tr->AddTri(i[0], i[3], i[2]);
		
				/*
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
				*/
		    }
		    else
		    {
				vert.mPos		= fvecs[sc];
				vert.mDiffuse	= 0;
				i[2] = tr->AddVertex(&vert);

				vert.mPos		= fvecs[p1];
				vert.mDiffuse	= 0;
				i[3] = tr->AddVertex(&vert);

				vert.mPos		= fvecs[sc + (RING_SEGS)];
				vert.mDiffuse	= col;
				i[0] = tr->AddVertex(&vert);

				vert.mPos		= fvecs[p1 + (RING_SEGS)];
				vert.mDiffuse	= col;
				i[1] = tr->AddVertex(&vert);

				tr->AddTri(i[0], i[3], i[1]);
				tr->AddTri(i[0], i[2], i[3]);

				tr->AddTri(i[0], i[1], i[3]);
				tr->AddTri(i[0], i[3], i[2]);

				/*
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
				*/
		    }

			/*
	       	GouraudS[0].uBaseRGB.dwPacked    = col;
	       	GouraudS[1].uBaseRGB.dwPacked    = col;
	       	GouraudS[2].uBaseRGB.dwPacked    = 0;
	       	GouraudS[3].uBaseRGB.dwPacked    = 0;
			*/
			
		}	
	}
	else
	{
		float	adder= 1.f / float(RING_SEGS);//(63.f / 256.f) / float(RING_SEGS);   //! assume 256x256 texture page
		SINT tnum=p->uPRing.mTextureNum;
//		STextureRefOther *tr=ENGINE.GetParticleTexture(tnum&0x3ff);
//		SINT	TN = tr->mTPage + 1;

//		kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
//	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
//	 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
//	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );

		CTextureGTEXAdditive	*tr = CRTParticleSystem::GetAnimTexture(tnum&0x3ff);
		if (!tr)
			return;

		for(int sc=0;sc<RING_SEGS;sc++)
		{
			int p1=(sc+1);
			if(p1==RING_SEGS)
				p1=0;

//			if ((fvecs[sc].Z > 0.f) || (fvecs[p1].Z > 0.f) ||
//			    (fvecs[sc + RING_SEGS].Z > 0.f) || (fvecs[p1 + RING_SEGS].Z > 0.f))
//			    continue;

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
				
//				InitCZTs(TN);
				
								
				float	u0 = 0.0f;//tr->mX + 0.002f;
				float	u1 = 1.0f;//tr->texw;//tr->mX + tr->mW - 0.002f;
				float	v0 = 0.0f;//tr->mY + 0.002f;
				float	v1 = 1.0f;//tr->texh;//tr->mY + tr->mH - 0.002f;
				
				vert.mPos		= *x0;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u0;
				else
					vert.mU		= u0 + (float(sc) * adder);
				vert.mV			= v0;
				i[0] = tr->AddVertex(&vert);

				vert.mPos		= *x1;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u1;
				else
					vert.mU		= u1 + (float(sc) * adder) + adder;
				vert.mV			= v0;
				i[1] = tr->AddVertex(&vert);

				vert.mPos		= xy1;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u0;
				else
					vert.mU		= u0 + (float(sc) * adder);
				vert.mV			= v1;
				i[2] = tr->AddVertex(&vert);

				vert.mPos		= xy2;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u1;
				else
					vert.mU		= u0 + (float(sc) * adder) + adder;
				vert.mV			= v1;
				i[3] = tr->AddVertex(&vert);

				tr->AddTri(i[0], i[3], i[1]);
				tr->AddTri(i[0], i[2], i[3]);

				tr->AddTri(i[0], i[1], i[3]);
				tr->AddTri(i[0], i[3], i[2]);
				

				/*
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
				*/
				// and again
					
				
				//InitCZTs(TN);

				vert.mPos		= xy1;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u0;
				else
					vert.mU		= u0 + (float(sc) * adder);
				vert.mV			= v0;
				i[0] = tr->AddVertex(&vert);

				vert.mPos		= xy2;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u1;
				else
					vert.mU		= u1 + (float(sc) * adder) + adder;
				vert.mV			= v0;
				i[1] = tr->AddVertex(&vert);

				vert.mPos		= *x2;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u0;
				else
				vert.mU			= u0 + (float(sc) * adder);
				vert.mV			= v1;
				i[2] = tr->AddVertex(&vert);

				vert.mPos		= *x3;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u1;
				else
					vert.mU		= u0 + (float(sc) * adder) + adder;
				vert.mV			= v1;
				i[3] = tr->AddVertex(&vert);

				tr->AddTri(i[0], i[3], i[1]);
				tr->AddTri(i[0], i[2], i[3]);

				tr->AddTri(i[0], i[1], i[3]);
				tr->AddTri(i[0], i[3], i[2]);


				/*
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
				*/
			}
			else
			{
				//InitCZTs(TN);
				float	u0 = 0.0f;
				float	u1 = 1.0f;
				float	v0 = 0.0f;
				float	v1 = 1.0f;

				vert.mPos		= *x0;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u0;
				else
					vert.mU		= u0 + (float(sc) * adder);
				vert.mV			= v0;
				i[0] = tr->AddVertex(&vert);

				vert.mPos		= *x1;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u1;
				else
					vert.mU		= u1 + (float(sc) * adder) + adder;
				vert.mV			= v0;
				i[1] = tr->AddVertex(&vert);

				vert.mPos		= *x2;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u0;
				else
				vert.mU			= u0 + (float(sc) * adder);
				vert.mV			= v1;
				i[2] = tr->AddVertex(&vert);

				vert.mPos		= *x3;
				vert.mDiffuse	= col;
				if (tnum&PART_TILEH)
					vert.mU		= u1;
				else
					vert.mU		= u0 + (float(sc) * adder) + adder;
				vert.mV			= v1;
				i[3] = tr->AddVertex(&vert);

				tr->AddTri(i[0], i[3], i[1]);
				tr->AddTri(i[0], i[2], i[3]);

				tr->AddTri(i[0], i[1], i[3]);
				tr->AddTri(i[0], i[3], i[2]);
			

				/*
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
				*/
			}
		}
	}
}

//******************************************************************************
#define 	LINE_WIDTH	0.05f
void	RenderLine(FVector s, FVector e, DWORD col1, DWORD col2, FVector &pos, FMatrix &ori, BOOL alpha = TRUE)
{
	// JCL this is probably the slowest line draw ever written

//	SINT		c0, c1, c2;
	CFVF_GTEX	vert;
	SINT		i[4];
	
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


	FVector	verts[2][2];
	
	verts[0][0] = curr_pos - perp;
	verts[0][1] = curr_pos + perp;
	
	curr_pos = e;
		
	verts[1][0] = curr_pos - perp;
	verts[1][1] = curr_pos + perp;
	
	// Set up transform.
	ENGINE.SetupTransforms(pos, ori);

	CTextureGTEXAdditive	*tr = CRTParticleSystem::mTexWhite;

	vert.mPos = verts[0][0];
	vert.mDiffuse	= col1;
	i[0] = tr->AddVertex(&vert);

	vert.mPos = verts[0][1];
	vert.mDiffuse	= col1;
	i[1] = tr->AddVertex(&vert);

	vert.mPos = verts[1][0];
	vert.mDiffuse	= col2;
	i[2] = tr->AddVertex(&vert);

	vert.mPos = verts[1][1];
	vert.mDiffuse	= col2;
	i[3] = tr->AddVertex(&vert);

	tr->AddTri(i[0], i[3], i[1]);
	tr->AddTri(i[0], i[2], i[3]);

	tr->AddTri(i[0], i[1], i[3]);
	tr->AddTri(i[0], i[3], i[2]);
}

#define ftoimv(x)  ((int)((x*32)/5.4017))

#define MESH_VERTEX(x,y,z) {ftoimv(x),ftoimv(y),-ftoimv(z)}
#define MESH_FACE(a,b,c) {a,b,c,0}

#define SPHERE_NUMVERTS 26
#define SPHERE_NUMFACES 40
		//MESH_NUMVERTEX 26
		//MESH_NUMFACES 40
SVECTOR hemiverts[26]= {
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

SVECTOR lightning_verts[NUM_LVERTS]= {

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


void RenderAlphaShpere(CParticle *p) 
{
	SVECTOR		vecs[100];
	int			i;
	CFVF_GTEX	vert;
	SINT		id[4];

	for(i=0;i<SPHERE_NUMVERTS;i++)
	{
		vecs[i].vx=hemiverts[i].vx*(1+(p->mRadius));
		vecs[i].vy=hemiverts[i].vy*(1+(p->mRadius));
		vecs[i].vz=hemiverts[i].vz*(1+(p->mRadius));
	}

	DWORD col = Interpolcol(&p->mColour,p->uPHemi.mBrightness);
	SortOutParticleColour(col);

	// JCL - create real verts to transform
	FVector	fvecs[100];
	SINT 	c0;
	for (c0 = 0; c0 < SPHERE_NUMVERTS; c0 ++)
	{
		fvecs[c0].X = float(vecs[c0].vx) / SVECSCALE;
		fvecs[c0].Y = float(vecs[c0].vy) / SVECSCALE;
		fvecs[c0].Z = float(vecs[c0].vz) / SVECSCALE;
	}
	
	// transform them
	//PersTransWrapper(SPHERE_NUMVERTS, (tvector *)fvecs, (tvector *)fvecs, GVectorToFVector(p->mPos) , ID_FMATRIX);
	ENGINE.SetupTransforms(GVectorToFVector(p->mPos), ID_FMATRIX);

    //kmChangeStripListType			( &GouraudHead, KM_TRANS_POLYGON );    
    //kmChangeStripBlendingMode       ( &GouraudHead, KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	//kmChangeStripTextureShadingMode ( &GouraudHead, KM_IMAGE_PARAM1, KM_MODULATE);	
 	//kmChangeStripUseAlpha			( &GouraudHead, KM_IMAGE_PARAM1, KM_TRUE );
	
	CTextureGTEXAdditive	*tr = CRTParticleSystem::mTexWhite;
	if (!tr)
		return;

	for (i = 0; i < SPHERE_NUMFACES; i++)
	{
	//	if ((fvecs[MFaces[i].v1].Z > 0.f) || (fvecs[MFaces[i].v2].Z > 0.f) || (fvecs[MFaces[i].v3].Z > 0.f))
	//	    continue;

		vert.mPos		= fvecs[MFaces[i].v1];
		vert.mDiffuse	= col;
		id[0] = tr->AddVertex(&vert);

		vert.mPos		= fvecs[MFaces[i].v2];
		vert.mDiffuse	= col;
		id[1] = tr->AddVertex(&vert);

		vert.mPos		= fvecs[MFaces[i].v3];
		vert.mDiffuse	= col;
		id[2] = tr->AddVertex(&vert);
			
		tr->AddTri(id[0], id[1], id[2]);
		tr->AddTri(id[0], id[2], id[1]);
	}
}

void RenderAlphaSpark(CParticle *p, BOOL alpha, SINT Queuelength,CThing *mOwner) 
{
	unsigned long col=Interpolcol(&p->mColour,p->uPSpark.mBrightness);
	//DWORD col = p->mColour;

	FVector	ppos;
	if((!p->mIsRelative)||(!mOwner))
		ppos = GVectorToFVector(p->mPos);
	else
		ppos = GVectorToFVector(p->mPos + mOwner->GetRenderPos());

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
		//SINT	col1 = col;//Interpolcol(&col, short(((Queuelength -  c     ) * 4096) / Queuelength));
		//SINT	col2 = col;//Interpolcol(&col, short(((Queuelength - (c + 1)) * 4096) / Queuelength));

		SINT	col1 = Interpolcol(&col, short(((Queuelength -  c     ) * 4096) / Queuelength));
		SINT	col2 = Interpolcol(&col, short(((Queuelength - (c + 1)) * 4096) / Queuelength));
	
		RenderLine(fvecs[c], fvecs[c + 1], col1, col2, ppos, ID_FMATRIX, alpha);
	}
}

//void RenderStarTrail(CParticle *p, SINT Queuelength) {}

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
//	DWORD col = p->mColour;

	FVector	ppos;
	if((!p->mIsRelative)||(!mOwner))
		ppos = GVectorToFVector(p->mPos);
	else
		ppos = GVectorToFVector(p->mPos + mOwner->GetRenderPos());

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

		for(i=0;i<5;i++)
		{
			RenderLine(fvecs[i], fvecs[i + 1], c, c, ppos, ori);
		}
	}
}




//******************************************************************************************
CTextureGTEXAdditive	*CRTParticleSystem::GetAnimTexture(SINT n)
{
	switch(n)
	{					
	case TP_FIRE1:		return mTexFire1;
	case TP_FIRE2:		return mTexFire2; 
	case TP_FIRE3:		return mTexFire3; 
	case TP_STAR1:		return mTexStar1; 
	case TP_STAR2:		return mTexStar2; 
	case TP_SSTAR1:		return mTexSStar1; 
	case TP_SSTAR2:		return mTexSStar2;
	case TP_EXP00:		return mTexExp00;
	case TP_EXP01:		return mTexExp01;
	case TP_EXP02:		return mTexExp02;
	case TP_EXP03:		return mTexExp03;
	case TP_EXP04:		return mTexExp04;
	case TP_EXP05:		return mTexExp05;
	case TP_EXP06:		return mTexExp06;
	case TP_EXP07:		return mTexExp07;
	case TP_EXP08:		return mTexExp08;
	case TP_EXP09:		return mTexExp09;
	case TP_EXP10:		return mTexExp10;
	case TP_EXP11:		return mTexExp11;
	case TP_EXP12:		return mTexExp12;
	case TP_EXP13:		return mTexExp13;
	case TP_EXP14:		return mTexExp14;
	case TP_EXP15:		return mTexExp15;
	case TP_WAT00:		return mTexWat00;
	case TP_WAT01:		return mTexWat01;
	case TP_WAT02:		return mTexWat02;
	case TP_WAT03:		return mTexWat03;
	case TP_WAT04:		return mTexWat04;
	case TP_WAT05:		return mTexWat05;
	case TP_BUBBLE:		return mTexBubble;
	case TP_Z:			return mTexZ;
	case TP_RING:		return mTexRing;
	case TP_SMOKE:		return mTexSmoke;
	case TP_SHOCKWAVE:	return mTexShockwave;
	case TP_HIT1:		return mTexHit1;
	case TP_HIT2:		return mTexHit2;
	case TP_INNER:		return mTexInner;
	case TP_MIDDLE:		return mTexMiddle;
	case TP_OUTER:		return mTexOuter;
	case TP_FLASH_SM:	return mTexFlashSM;

	default:
		return mTexFire1;
	};
}

//******************************************************************************************
void	CRTParticleSystem::Render()//CParticleSystem *p)
{
	SINT	c0;
	
	// Camera 
	FMatrix				ori		= ID_FMATRIX;
	FVector				pos		= FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);
	CCamera				*cam	= ENGINE.GetCamera();

	// Particles
	CParticleSystem		*ps		= GetRenderParticleSystem();
	if ((!ps) || (!(ps->IsActive())))
		return;
	CParticle			*part	= ps->mParticles;

	for (c0 = 0; c0 < MAX_PARTICLES; c0 ++)
	{
		if (part->mType != PART_NONE)
		{
			FVector		p		= GVectorToFVector(part->mPos);
			DWORD		col;
						
			FMatrix		cori	= GMatrixToFMatrix(cam->mOrientation);
			float		radius	= float(part->mRadius) / 128.f;
			FVector		x		= cori * FVector(radius, 0, 0);
			FVector		y		= cori * FVector(0, radius, 0);
	
			if(part->mDelay==0)
			{
				switch(part->mType)
				{
				case	PART_FIRE1				:
				case	PART_BODY_FIRE			:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexFire1,		part->mColour);	break;
				case	PART_FIRE2				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexFire2,		part->mColour);	break;
				case	PART_FIRE3				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexFire3,		part->mColour);	break;
				case	PART_Z					:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexZ,			part->mColour);	break;
				case	PART_WRING2				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexBubble,	part->mColour);	break;
					
				case	PART_TELEPORTINRING		:
				case	PART_TELEPORTINCOLUMN	:
				case	PART_NECOLUMN 			:
				case	PART_WATERRING_SMALL	:
				case	PART_WATERRING 			:
				case	PART_COLUMN				:
				case	PART_RING				:	ParticleRingRender(part, ps->mOwner); break;
					
					
// are: No longer used (see dcparticlesystem.cpp)				case	PART_RAY				:	ParticleRayRender(part); break;
				case	PART_STAR1				:	
					col =  Interpolcol(&part->mColour, part->uPStar.mBrightness) & 0x00ffffff;
					CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexStar1, col); break;
				case	PART_STAR2				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexStar1, part->mColour); break;//|(0x02<<24)); break;
				case	PART_SHOCKWAVE			:
				case	PART_FLASH				:
				case	PART_ANIM				:
					{
						col = Interpolcol(&part->mColour, part->uPAnim.mBrightness) & 0x00ffffff;

						if(part->uPAnim.mHoriz==1)
						{
							SINT fudge;
							if(part->mType==PART_SHOCKWAVE)SINT fudge=-20;
							else fudge=0;
							CRTDecal::CommonDecalRenderHoriz(	&part->mPos,
																part->mRadius << 9,
																GetAnimTexture(part->uPAnim.mFrame),
																col,//|(2<<24),
																fudge);
						}
						else
							if(part->uPAnim.mHoriz==2)
								CRTDecal::CommonDecalRenderHoriz2(	&part->mPos, 
																	part->mRadius << 9, 
																	GetAnimTexture(part->uPAnim.mFrame),
																	col, //|(2<<24),
																	part->uPAnim.mSn,
																	part->uPAnim.mCs);
							else
								CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, GetAnimTexture(part->uPAnim.mFrame), col);//|(2<<24));
					}
					break;
				case	PART_SMOKE				:	
					col = Interpolcol(&part->mColour, part->uPAnim.mBrightness) & 0x00ffffff;
					CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexSmoke, col); break;

				case	PART_HEMI				:	RenderAlphaShpere(part);				break;
				case	PART_SPARK				:	RenderAlphaSpark(part, FALSE,2,NULL);	break;
				case	PART_SPLASH				:	RenderAlphaSpark(part, TRUE,2,NULL);	break;
				case	PART_SPINSPARK			:	RenderAlphaSpark(part, TRUE,4,NULL);	break;
				case	PART_LIGHTNING			:	RenderLightning(part, ps->mOwner);		break;
				case	PART_LIGHT				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexHit1,	part->mColour); break;//|(0x02<<24)); break;
				default 	 					:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexFire1,	part->mColour); break;
				}
			}
		}
		part++;
	}
}

//******************************************************************************************
void	CRTParticleSystem::Render(CParticleSystem *ps)
{
	SINT	c0;
	
	// Camera 
	FMatrix				ori		= ID_FMATRIX;
	FVector				pos		= FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);
	CCamera				*cam	= ENGINE.GetCamera();
	FVector l1 = GVectorToFVector(ENGINE.GetCamera()->mLookAt);

	// Particles
	//CParticleSystem		*ps		= GetRenderParticleSystem();
	if ((!ps) || (!(ps->IsActive())))
		return;
	CParticle			*part	= ps->mParticles;

	for (c0 = 0; c0 < MAX_PARTICLES; c0 ++)
	{
		if (part->mType != PART_NONE)
		{
			FVector	p	= GVectorToFVector(part->mPos);
			float dist = (float)((l1 - p).MagnitudeSq());
			if(dist < 2000.f)
			{
				DWORD		col;
							
				FMatrix		cori	= GMatrixToFMatrix(cam->mOrientation);
				float		radius	= float(part->mRadius) / 128.f;
				FVector		x		= cori * FVector(radius, 0, 0);
				FVector		y		= cori * FVector(0, radius, 0);
		
				if(part->mDelay==0)
				{
					switch(part->mType)
					{
					case	PART_FIRE1				:
					case	PART_BODY_FIRE			:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexFire1,		part->mColour);	break;
					case	PART_FIRE2				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexFire2,		part->mColour);	break;
					case	PART_FIRE3				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexFire3,		part->mColour);	break;
					case	PART_Z					:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexZ,			part->mColour);	break;
					case	PART_WRING2				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexBubble,	part->mColour);	break;
						
					case	PART_TELEPORTINRING		:
					case	PART_TELEPORTINCOLUMN	:
					case	PART_NECOLUMN 			:
					case	PART_WATERRING_SMALL	:
					case	PART_WATERRING 			:
					case	PART_COLUMN				:
					case	PART_RING				:	
						ParticleRingRender(part, ps->mOwner); 
						break;
						
					case	PART_STAR1				:	
						col =  Interpolcol(&part->mColour, part->uPStar.mBrightness) & 0x00ffffff;
						CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexStar1, col); break;
					case	PART_STAR2				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexStar1, part->mColour); break;//|(0x02<<24)); break;
					case	PART_SHOCKWAVE			:
					case	PART_FLASH				:
					case	PART_ANIM				:
						{
							col = Interpolcol(&part->mColour, part->uPAnim.mBrightness) & 0x00ffffff;

							if(part->uPAnim.mHoriz==1)
							{
								SINT fudge;
								if(part->mType==PART_SHOCKWAVE)SINT fudge=-20;
								else fudge=0;
								CRTDecal::CommonDecalRenderHoriz(	&part->mPos,
																	part->mRadius << 9,
																	GetAnimTexture(part->uPAnim.mFrame),
																	col,//|(2<<24),
																	fudge);
							}
							else
								if(part->uPAnim.mHoriz==2)
									CRTDecal::CommonDecalRenderHoriz2(	&part->mPos, 
																		part->mRadius << 9, 
																		GetAnimTexture(part->uPAnim.mFrame),
																		col, //|(2<<24),
																		part->uPAnim.mSn,
																		part->uPAnim.mCs);
								else
									CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, GetAnimTexture(part->uPAnim.mFrame), col);//|(2<<24));
						}
						break;
					case	PART_SMOKE				:	
						col = Interpolcol(&part->mColour, part->uPAnim.mBrightness) & 0x00ffffff;
						CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexSmoke, col); break;

					case	PART_HEMI				:	RenderAlphaShpere(part);				break;
					case	PART_SPARK				:	RenderAlphaSpark(part, FALSE,2,NULL);	break;
					case	PART_SPLASH				:	RenderAlphaSpark(part, TRUE,2,NULL);	break;
					case	PART_SPINSPARK			:	RenderAlphaSpark(part, TRUE,4,NULL);	break;
					case	PART_LIGHTNING			:	RenderLightning(part, ps->mOwner);		break;
					case	PART_LIGHT				:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexHit1,	part->mColour); break;//|(0x02<<24)); break;
					default 	 					:	CRTDecal::CommonDecalRender(&part->mPos, part->mRadius << 9, mTexFire1,	part->mColour); break;
					}
				}
			}
		}
		part++;
	}
}


//******************************************************************************************
#ifdef LINK_EDITOR
void	CRTParticleSystem::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
//	ta.AddTexture(mTexFire1, TATT_OTHER);
//	ta.AddTexture(mTexZ, TATT_OTHER);

	ta.AddTexture(mTexFire1,		TATT_PARTICLE, TP_FIRE1, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexFire2,		TATT_PARTICLE, TP_FIRE2, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexFire3, 		TATT_PARTICLE, TP_FIRE3, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexZ,	 		TATT_PARTICLE, TP_Z    , 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexStar1,		TATT_PARTICLE, TP_STAR1, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexStar2,		TATT_PARTICLE, TP_STAR2, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp00,		TATT_PARTICLE, TP_EXP00, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp01,		TATT_PARTICLE, TP_EXP01, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp02,		TATT_PARTICLE, TP_EXP02, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp03,		TATT_PARTICLE, TP_EXP03, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp04,		TATT_PARTICLE, TP_EXP04, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp05,		TATT_PARTICLE, TP_EXP05, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp06,		TATT_PARTICLE, TP_EXP06, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp07,		TATT_PARTICLE, TP_EXP07, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp08,		TATT_PARTICLE, TP_EXP08, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp09,		TATT_PARTICLE, TP_EXP09, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp10,		TATT_PARTICLE, TP_EXP10, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp11,		TATT_PARTICLE, TP_EXP11, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp12,		TATT_PARTICLE, TP_EXP12, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp13,		TATT_PARTICLE, TP_EXP13, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp14,		TATT_PARTICLE, TP_EXP14, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexExp15,		TATT_PARTICLE, TP_EXP15, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexWat00,		TATT_PARTICLE, TP_WAT00, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexWat01,		TATT_PARTICLE, TP_WAT01, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexWat02,		TATT_PARTICLE, TP_WAT02, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexWat03,		TATT_PARTICLE, TP_WAT03, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexWat04,		TATT_PARTICLE, TP_WAT04, 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexWat05,		TATT_PARTICLE, TP_WAT05, 32, 32, BD_4, 0, TRUE);

	ta.AddTexture(mTexBubble,		TATT_PARTICLE, TP_BUBBLE, 16, 16, BD_4, 0, TRUE);
	ta.AddTexture(mTexRing,			TATT_PARTICLE, TP_RING , 32, 32, BD_4, 0, TRUE);
	ta.AddTexture(mTexSStar1,		TATT_PARTICLE, TP_SSTAR1, 16, 16, BD_4);
	ta.AddTexture(mTexSStar2,		TATT_PARTICLE, TP_SSTAR2, 16, 16, BD_4);

	ta.AddTexture(mTexSmoke,		TATT_PARTICLE, TP_SMOKE, 16, 16, BD_4, 0, TRUE);
	ta.AddTexture(mTexShockwave,	TATT_PARTICLE, TP_SHOCKWAVE, 16, 16, BD_8, 0, TRUE);
	ta.AddTexture(mTexHit1,			TATT_PARTICLE, TP_HIT1, 16, 16, BD_8, 0, TRUE);
	ta.AddTexture(mTexHit2,			TATT_PARTICLE, TP_HIT2, 16, 16, BD_8, 0, TRUE);
	ta.AddTexture(mTexInner,		TATT_PARTICLE, TP_INNER, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexMiddle,		TATT_PARTICLE, TP_MIDDLE, 64, 64, BD_4, 0, TRUE);
	ta.AddTexture(mTexOuter,		TATT_PARTICLE, TP_OUTER, 64, 64, BD_4, 0, TRUE);

	ta.AddTexture(mTexFlashSM,		TATT_PARTICLE, TP_FLASH_SM, 32, 32, BD_4, 0, TRUE);
}

//*****************************************************************************************************
void	CRTParticleSystem::AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	ta.AddTexture(mTexFire1,		TADCTT_PARTICLE, TP_FIRE1		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexFire2,		TADCTT_PARTICLE, TP_FIRE2		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexFire3, 		TADCTT_PARTICLE, TP_FIRE3		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexZ,	 		TADCTT_PARTICLE, TP_Z			, 32, 32, 0, TRUE);
	ta.AddTexture(mTexStar1,		TADCTT_PARTICLE, TP_STAR1		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexStar2,		TADCTT_PARTICLE, TP_STAR2		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexExp00,		TADCTT_PARTICLE, TP_EXP00		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp01,		TADCTT_PARTICLE, TP_EXP01		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp02,		TADCTT_PARTICLE, TP_EXP02		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp03,		TADCTT_PARTICLE, TP_EXP03		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp04,		TADCTT_PARTICLE, TP_EXP04		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp05,		TADCTT_PARTICLE, TP_EXP05		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp06,		TADCTT_PARTICLE, TP_EXP06		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp07,		TADCTT_PARTICLE, TP_EXP07		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp08,		TADCTT_PARTICLE, TP_EXP08		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp09,		TADCTT_PARTICLE, TP_EXP09		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp10,		TADCTT_PARTICLE, TP_EXP10		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp11,		TADCTT_PARTICLE, TP_EXP11		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp12,		TADCTT_PARTICLE, TP_EXP12		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp13,		TADCTT_PARTICLE, TP_EXP13		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp14,		TADCTT_PARTICLE, TP_EXP14		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexExp15,		TADCTT_PARTICLE, TP_EXP15		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexWat00,		TADCTT_PARTICLE, TP_WAT00		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexWat01,		TADCTT_PARTICLE, TP_WAT01		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexWat02,		TADCTT_PARTICLE, TP_WAT02		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexWat03,		TADCTT_PARTICLE, TP_WAT03		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexWat04,		TADCTT_PARTICLE, TP_WAT04		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexWat05,		TADCTT_PARTICLE, TP_WAT05		, 32, 32, 0, TRUE);

	ta.AddTexture(mTexBubble,		TADCTT_PARTICLE, TP_BUBBLE	, 16, 16, 0, TRUE);
	ta.AddTexture(mTexRing,			TADCTT_PARTICLE, TP_RING		, 32, 32, 0, TRUE);
	ta.AddTexture(mTexSStar1,		TADCTT_PARTICLE, TP_SSTAR1	, 16, 16);
	ta.AddTexture(mTexSStar2,		TADCTT_PARTICLE, TP_SSTAR2	, 16, 16);

	ta.AddTexture(mTexSmoke,		TADCTT_PARTICLE, TP_SMOKE		, 16, 16, 0, TRUE);
	ta.AddTexture(mTexShockwave,	TADCTT_PARTICLE, TP_SHOCKWAVE	, 16, 16, 0, TRUE);
	ta.AddTexture(mTexHit1,			TADCTT_PARTICLE, TP_HIT1		, 16, 16, 0, TRUE);
	ta.AddTexture(mTexHit2,			TADCTT_PARTICLE, TP_HIT2		, 16, 16, 0, TRUE);
	ta.AddTexture(mTexInner,		TADCTT_PARTICLE, TP_INNER		, 64, 64, 0, TRUE);
	ta.AddTexture(mTexMiddle,		TADCTT_PARTICLE, TP_MIDDLE	, 64, 64, 0, TRUE);
	ta.AddTexture(mTexOuter,		TADCTT_PARTICLE, TP_OUTER		, 64, 64, 0, TRUE);

	ta.AddTexture(mTexFlashSM,		TADCTT_PARTICLE, TP_FLASH_SM	, 32, 32, 0, TRUE);
}

#endif
#endif
