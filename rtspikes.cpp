#include	"Common.h"

#if TARGET == PC

#include	"RTSpikes.h"
#include	"Globals.h"
#include	"RawLoader.h"

//******************************************************************************************
void	CRTSpikes::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEX::GetTexture(i->mName);
}

//******************************************************************************************

#define	RTSPIKES_BASE_SIZE 0.3f
#define RTSPIKES_TOP_SEPARATION 0.5f

//******************************************************************************************
void	CRTSpikes::DrawSpike(FVector top, FVector bottom)
{
	SINT i[3];
	CFVF_GTEX	vert;

	FVector p[3];

	FVector v = top - bottom;

	FVector n1 = v ^ FVector(1.f, 0, 0);
	n1.Normalise();
	FVector n2 = v ^ n1;
	n2.Normalise();
	
	p[0] = bottom +  n1              * RTSPIKES_BASE_SIZE;
	p[1] = bottom - (n1 * 0.5f + n2) * RTSPIKES_BASE_SIZE;
	p[2] = bottom - (n1 * 0.5f - n2) * RTSPIKES_BASE_SIZE;

	vert.mDiffuse = 0xffffffff;
	vert.mU = 0;
	vert.mV = 0;
	vert.mPos = top;
	i[0] = mTex->AddVertex(&vert);
	vert.mU = 1.f;
	vert.mV = 0;
	vert.mPos = p[1];
	i[1] = mTex->AddVertex(&vert);
	vert.mU = 0;
	vert.mV = 1.f;
	vert.mPos = p[0];
	i[2] = mTex->AddVertex(&vert);
	mTex->AddTri(i[0], i[1], i[2]);

	vert.mU = 0;
	vert.mV = 0;
	vert.mPos = top;
	i[0] = mTex->AddVertex(&vert);
	vert.mU = 1.f;
	vert.mV = 0;
	vert.mPos = p[2];
	i[1] = mTex->AddVertex(&vert);
	vert.mU = 0;
	vert.mV = 1.f;
	vert.mPos = p[1];
	i[2] = mTex->AddVertex(&vert);
	mTex->AddTri(i[0], i[1], i[2]);

	vert.mU = 0;
	vert.mV = 0;
	vert.mPos = top;
	i[0] = mTex->AddVertex(&vert);
	vert.mU = 1.f;
	vert.mV = 0;
	vert.mPos = p[0];
	i[1] = mTex->AddVertex(&vert);
	vert.mU = 0;
	vert.mV = 1.f;
	vert.mPos = p[2];
	i[2] = mTex->AddVertex(&vert);
	mTex->AddTri(i[0], i[1], i[2]);

}

//******************************************************************************************
void	CRTSpikes::Render()
{
	FVector p;
	FMatrix ori;
	GetRenderPosition(&p);
	GetRenderOrientation(&ori);

	float	fr	= GToF(mThing->GetRenderTurn());

	ENGINE.SetupTransforms(p, ori);

	FVector bottom = FVector(0, 0,  1.f);
	FVector top    = FVector(0, 0, -1.f);

	DrawSpike(top + FVector( RTSPIKES_TOP_SEPARATION,  RTSPIKES_TOP_SEPARATION, 0), bottom);
	DrawSpike(top + FVector(-RTSPIKES_TOP_SEPARATION,  RTSPIKES_TOP_SEPARATION, 0), bottom);
	DrawSpike(top + FVector( RTSPIKES_TOP_SEPARATION, -RTSPIKES_TOP_SEPARATION, 0), bottom);
	DrawSpike(top + FVector(-RTSPIKES_TOP_SEPARATION, -RTSPIKES_TOP_SEPARATION, 0), bottom);
	DrawSpike(top																  , bottom);

	mTex->DrawAll();
}

//******************************************************************************************

#endif
