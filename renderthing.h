#ifndef RENDERTHING_H
#define	RENDERTHING_H

//******************************************************************************************
#include	"PCRTIDs.h"
#include	"Thing.h"
#include	"JCLEngine.h"

#include	"CollisionVolume.h"

#include	"TextureAccumulator.h"
#include	"MeshAccumulator.h"

#include	"TextureAccumulatorDC.h"
#include	"MeshAccumulatorDC.h"

#include	"PSXRenderData.h"

class	CThing;
class	CParticleSystem;

//******************************************************************************************

class	CInitRenderThing
{
public:
	CInitRenderThing()
	{
		mName[0]  = 0;
		mName2[0] = 0;
		mName3[0] = 0;
		mName4[0] = 0;
		mThing = NULL;
		mShadow = ST_NONE;
		mWaveResponse = WR_NONE;
		mSkin = 0;
		mNumSkins = 1;
		mAlpha = FALSE;
		mHoriz = FALSE;
	};
		
	char			mName[256];
	char			mName2[256];
	char			mName3[256];
	char			mName4[256];
	CThing			*mThing;
	EShadowType		mShadow;
	EWaveResponse	mWaveResponse;
	SINT			mSkin;
	SINT			mNumSkins;
	BOOL			mAlpha;
	BOOL			mHoriz;
};

//******************************************************************************************

class	CRenderThing
{
public:
	CRenderThing() {mNextRenderThing = NULL;};
	virtual	~CRenderThing() {if (mNextRenderThing) delete mNextRenderThing;};

	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render() = 0;

	void	GetRenderPosition(FVector *f, SINT no = 0)
	{
		ASSERT(mThing);
		GVector g = mThing->GetRenderPos(no);
		GVectorToFVector(&g, f);
	}

	void	GetRenderOrientation(FMatrix *f, SINT no = 0)
	{
		ASSERT(mThing);
		GMatrix g = mThing->GetRenderOrientation(no);
		GMatrixToFMatrix(&g, f);
	}


	void	GetRenderStartPosition(FVector *f)
	{
		ASSERT(mThing);
		GVector g = mThing->GetRenderStartPos();
		GVectorToFVector(&g, f);
	}

	void	GetRenderEndPosition(FVector *f)
	{
		ASSERT(mThing);
		GVector g = mThing->GetRenderEndPos();
		GVectorToFVector(&g, f);
	}

	float	GetRenderRadius()
	{
		ASSERT(mThing);
		return GToF(mThing->GetRenderRadius());
	}

	CParticleSystem	*GetRenderParticleSystem()
	{
		ASSERT(mThing);
		return mThing->GetRenderParticleSystem();
	}

	//! careful - don't confuse with above
	// this is the radius that the render thing thinks it is...
	virtual	GINT	GetRTRadius() {return GINT_HALF;};

	virtual	GVector	GetRTEmitter(SINT no) {return ZERO_GVECTOR;};
	virtual	GVector	GetRTEmitterAnimated(SINT no) {return ZERO_GVECTOR;};

	virtual	CCollisionVolume	*GetCollisionVolume() {return &UNIT_BOX;};

	virtual	GINT	GetFrameIncrement(EAnimMode a, SINT* realindex) {return G0;};

	// chaining renderthings
	void	ChainRenderThing(CRenderThing *rt)
	{
		ASSERT(mNextRenderThing == NULL);  // shouldn't be overwriting stuff...	
		mNextRenderThing = rt;
	}

	CRenderThing *GetChainedRenderThing(SINT no)
	{
		if ((no == 0) || (mNextRenderThing == NULL))
		{
//			ASSERT((no == 0) || (mNextRenderThing)); // shouldn't really happen....
			return this;
		}
		else
			return mNextRenderThing->GetChainedRenderThing(no - 1);
	}

	CRenderThing	*GetNextRenderThing() {return mNextRenderThing;};

	virtual	void	RenderCollisionVolume(SINT i);

#ifdef LINK_EDITOR
	virtual	void	FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt) {};
#endif


#ifdef	LINK_EDITOR
	virtual	void	AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
	{
		if (mNextRenderThing)
			mNextRenderThing->AccumulateResources(ta, ma);
	};
	
	virtual	void	FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt) {};
	virtual	void	AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
	{
		if (mNextRenderThing)
			mNextRenderThing->AccumulateResourcesDC(ta, ma);
	};
#endif

protected:
	CThing	*mThing;

	CRenderThing *mNextRenderThing;

};

//******************************************************************************************

#endif