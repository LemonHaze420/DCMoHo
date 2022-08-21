#include	"Common.h"

#if TARGET == PC || DC

#include	"RTLink.h"
#include	"Globals.h"

//******************************************************************************************
void	CRTLink::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEXAdditive::GetTexture(i->mName);
}

//******************************************************************************************
#define RTLINK_NUM_SEGS	4
#define RTLINK_SEG_SIZE 0.2f
//#define RTLINK_SEG_SIZE 0.5f

//******************************************************************************************
void	CRTLink::Render()
{
	if (!mThing)
		return;

	FVector p;
	FMatrix ori;
	GetRenderPosition(&p);
	ori = ID_FMATRIX;
	
	FVector	s, e;
	GetRenderStartPosition(&s);
	s -= p;

	GetRenderEndPosition(&e);
	e -= p;

	ENGINE.SetupTransforms(p, ori);

	SINT c0;
	GetRenderOrientation(&ori, 1);

	FVector	seg_offset = (e - s) / float(RTLINK_NUM_SEGS);
	FVector	x = ori * FVector(RTLINK_SEG_SIZE, 0, 0);
	FVector	y = ori * FVector(0, RTLINK_SEG_SIZE, 0);

	CFVF_GTEX	vert;
	SINT i[4];

	FVector	curr_pos = s;

	SINT	col = mThing->GetLife() << 1;
	if (col < 0)
		col = 0;
	if (col > 200)
		col = 200;

	vert.mDiffuse = (0xff << 24) + ((200 - col) << 16) + (col << 8);

	for	(c0 = 0; c0 < RTLINK_NUM_SEGS; c0++)
	{
		CTextureGTEXAdditive	*t = mTex;

		SINT	c1;
		for (c1 = 0; c1 < 4; c1 ++)
		{
			vert.mPos	= curr_pos + ((c1 & 1) ? x : -x) + ((c1 & 2) ? y : -y);
			vert.mU	= (c1&1)?1.f:0.f;
			vert.mV	= (c1&2)?1.f:0.f;
			i[c1] = t->AddVertex(&vert);
		}
		t->AddTri(i[0], i[3], i[2]);
		t->AddTri(i[0], i[1], i[3]);
		t->AddTri(i[0], i[2], i[3]);
		t->AddTri(i[0], i[3], i[1]);

		curr_pos += seg_offset;
	}
}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CRTLink::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	ta.AddTexture(mTex, TATT_OTHER, 1,  -1, -1, BD_4, 0, TRUE);
}

//******************************************************************************************
void	CRTLink::FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt)
{
	if (mTex)
		ri.i0 = ta.GetTextureIndex(TATT_OTHER, mTex);
	else
		ri.i0 = -1;
}


//******************************************************************************************
void	CRTLink::AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	ta.AddTexture(mTex, TADCTT_OTHER, 1,  -1, -1, 0, TRUE);

	SUPERTYPE::AccumulateResourcesDC(ta, ma);
}


//******************************************************************************************
void	CRTLink::FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt)
{
	if (mTex)
		ri.i0 = ta.GetTextureIndex(TADCTT_OTHER, mTex);
	else
		ri.i0 = -1;
}

#endif

//******************************************************************************************
#endif
