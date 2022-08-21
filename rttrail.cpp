#include	"Common.h"

#if TARGET == PC

#include	"RTTrail.h"
#include	"Globals.h"
#include	"Trail.h"

//******************************************************************************************
void	CRTTrail::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEXAdditive::GetTexture(i->mName);
}

//******************************************************************************************
void	CRTTrail::Render()
{
	FVector p = ZERO_FVECTOR;
	FMatrix ori = ID_FMATRIX;;

	ENGINE.SetupTransforms(p, ori);

	if (!mThing)
		return;

	CTrail	*trail = mThing->GetRenderTrail();
	if (!trail)
		return;

	SINT	c0;
	SINT	cs = trail->mStartPoint;
	SINT	i[4];
	DWORD	col;
	SINT	cmag = 256 / MAX_TRAIL_POINTS;
	SINT	vc = 0;

	CFVF_GTEX	vert;

	for	(c0 = 0; c0 < MAX_TRAIL_POINTS - 1; c0++)
	{
		SINT	ns = (cs + 1) % MAX_TRAIL_POINTS;
		
		STrailPoint	*p1 = &trail->mPoints[cs];
		STrailPoint	*p2 = &trail->mPoints[ns];
		vc += cmag;

		if ((p1->mActive) && (p2->mActive))
		{
			col = 0xff000000 + (vc << 16) + (vc << 8) + vc;
			// old verts
			vert.mDiffuse = col;
			vert.mPos = GVectorToFVector(p1->mStart);
			vert.mU = 0.f;
			vert.mV = 1.f;
			i[0] = mTex->AddVertex(&vert);
			vert.mPos = GVectorToFVector(p1->mEnd);
			vert.mV = 0.1f;
			i[1] = mTex->AddVertex(&vert);

			// new verts
			vert.mPos = GVectorToFVector(p2->mStart);
			vert.mU = 1.f;
			vert.mV = 1.f;
			i[2] = mTex->AddVertex(&vert);
			vert.mPos = GVectorToFVector(p2->mEnd);
			vert.mV = 0.1f;
			i[3] = mTex->AddVertex(&vert);

			mTex->AddTri(i[0], i[3], i[2]);
			mTex->AddTri(i[0], i[1], i[3]);
			mTex->AddTri(i[0], i[2], i[3]);
			mTex->AddTri(i[0], i[3], i[1]);
		}

		cs = ns;
	}
}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CRTTrail::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	ta.AddTexture(mTex, TATT_OTHER, 1,  -1, -1, BD_4, 0, TRUE);
}

//******************************************************************************************
void	CRTTrail::FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt)
{
	if (mTex)
		ri.i0 = ta.GetTextureIndex(TATT_OTHER, mTex);
	else
		ri.i0 = -1;
}

//******************************************************************************************
void	CRTTrail::AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	ta.AddTexture(mTex, TADCTT_OTHER, 1,  -1, -1, 0, TRUE);

	SUPERTYPE::AccumulateResourcesDC(ta, ma);
}

//******************************************************************************************
void	CRTTrail::FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt)
{
	if (mTex)
		ri.i0 = ta.GetTextureIndex(TADCTT_OTHER, mTex);
	else
		ri.i0 = -1;
}
#endif

//******************************************************************************************
#endif
