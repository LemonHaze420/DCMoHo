#include	"Common.h"

#if TARGET == PC

#include	"RTFence.h"
#include	"Globals.h"

//******************************************************************************************
void	CRTFence::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEXAdditive::GetTexture(i->mName);
}

//******************************************************************************************

#define	RTFence_DESIRED_SEGMENT_SIZE	4.f
#define	RTFence_HEIGHT					2.2f

//******************************************************************************************
void	CRTFence::Render()
{
	FVector p;
	FMatrix ori;
	GetRenderPosition(&p);
	GetRenderOrientation(&ori);

	FVector	s, e;
	GetRenderStartPosition(&s);
	s -= p;

	GetRenderEndPosition(&e);
	e -= p;

	ENGINE.SetupTransforms(p, ori);

	SINT c0;

	// work out how many segments we need

	float	dist = (e - s).Magnitude();

	SINT	num_segs = SINT(dist / RTFence_DESIRED_SEGMENT_SIZE);
	FVector	seg_offset = (e - s) / float(num_segs);
	FVector	perp = FVector(0, 0, RTFence_HEIGHT);

	CFVF_GTEX	vert;

	FVector	curr_pos = s;

	for	(c0 = 0; c0 < num_segs; c0++)
	{
		SINT i[4];

		// old verts
		vert.mDiffuse = 0xffffffff;
		vert.mPos = curr_pos - perp;
		vert.mU = 0.f;
		vert.mV = 1.f;
		i[0] = mTex->AddVertex(&vert);
		vert.mPos = curr_pos;
		vert.mV = 0.f;
		i[1] = mTex->AddVertex(&vert);

		// nev verts
		if (c0 != num_segs - 1)
		{
			curr_pos = s + (seg_offset * float(c0 + 1));
		}
		else
			curr_pos = e;

		vert.mPos = curr_pos - perp;
		vert.mU = 1.f;
		vert.mV = 1.f;
		i[2] = mTex->AddVertex(&vert);
		vert.mPos = curr_pos;
		vert.mV = 0.f;
		i[3] = mTex->AddVertex(&vert);

		mTex->AddTri(i[0], i[3], i[2]);
		mTex->AddTri(i[0], i[1], i[3]);
		mTex->AddTri(i[0], i[2], i[3]);
		mTex->AddTri(i[0], i[3], i[1]);
	}
}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CRTFence::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	ta.AddTexture(mTex, TATT_OTHER, 1,  -1, -1, BD_4, 0, TRUE);
}

//******************************************************************************************
void	CRTFence::FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt)
{
	if (mTex)
		ri.i0 = ta.GetTextureIndex(TATT_OTHER, mTex);
	else
		ri.i0 = -1;
}

//******************************************************************************************
void	CRTFence::AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	ta.AddTexture(mTex, TADCTT_OTHER, 1,  -1, -1, 0, TRUE);

	SUPERTYPE::AccumulateResourcesDC(ta, ma);
}

//******************************************************************************************
void	CRTFence::FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt)
{
	if (mTex)
		ri.i0 = ta.GetTextureIndex(TADCTT_OTHER, mTex);
	else
		ri.i0 = -1;
}
#endif

//******************************************************************************************
#endif
