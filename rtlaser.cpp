#include	"Common.h"

#if TARGET == PC

#include	"RTLaser.h"
#include	"Globals.h"

//******************************************************************************************
void	CRTLaser::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex1 = CTextureGTEXAdditive::GetTexture(i->mName);
	mTex2 = CTextureGTEXAdditive::GetTexture(i->mName2);
	mTex3 = CTextureGTEXAdditive::GetTexture(i->mName3);
	mTex4 = CTextureGTEXAdditive::GetTexture(i->mName4);
}

//******************************************************************************************

#define	RTLaser_START_AND_END_SIZE		1.f
#define	RTLaser_DESIRED_SEGMENT_SIZE	4.f
#define	RTLaser_HEIGHT					0.5f
#define	RTLaser_FLARE_SIZE				1.2f
//******************************************************************************************
void	CRTLaser::Render()
{
	FVector p;
	FMatrix ori;
	GetRenderPosition(&p);
//	GetRenderOrientation(&ori);
	ori = ID_FMATRIX;
	
	FVector	s, e;
	GetRenderStartPosition(&s);
	s -= p;

	GetRenderEndPosition(&e);
	e -= p;

	ENGINE.SetupTransforms(p, ori);

	SINT c0;

	// work out how many segments we need

	float	dist = (e - s).Magnitude();

	SINT	num_segs = SINT((dist - (RTLaser_START_AND_END_SIZE * 2)) / RTLaser_DESIRED_SEGMENT_SIZE);
	if (num_segs < 1)
		num_segs = 1;
	FVector	seg_offset = (e - s) / float(num_segs) * ((dist - (RTLaser_START_AND_END_SIZE * 2) ) / dist);
	FVector	seg_offsetse = (e - s) * RTLaser_START_AND_END_SIZE / dist;
	FVector perp = seg_offset ^ GVectorToFVector(ENGINE.GetCamera()->mOrientation * GVector(G0, G0, G1));
	num_segs += 2;

	if (perp.Magnitude() > 0.01f)
	{
		perp.Normalise();
		perp *= RTLaser_HEIGHT / 2;
	}
	else
		perp = FVector(0, 0, RTLaser_HEIGHT / 2);

	CFVF_GTEX	vert;
	SINT i[4];

	FVector	curr_pos = s;

	for	(c0 = 0; c0 < num_segs; c0++)
	{
		CTextureGTEXAdditive	*t = mTex1;

		if (c0 == 0)
			t = mTex2;
		else if (c0 == num_segs - 1)
			t = mTex3;

		// old verts
		vert.mDiffuse = 0xffffffff;
		vert.mPos = curr_pos - perp;
		vert.mU = 0.05f;
		vert.mV = 1.f;
		i[0] = t->AddVertex(&vert);
		vert.mPos = curr_pos + perp;
		vert.mV = 0.f;
		i[1] = t->AddVertex(&vert);

		// nev verts
		if (c0 == 0)
		{
			curr_pos = s + seg_offsetse;
		}
		else if (c0 != num_segs - 1)
		{
			curr_pos = s + seg_offsetse + (seg_offset * float(c0/* + 1*/));
		}
		else
			curr_pos = e;

		vert.mPos = curr_pos - perp;
		vert.mU = 0.95f;
		vert.mV = 1.f;
		i[2] = t->AddVertex(&vert);
		vert.mPos = curr_pos + perp;
		vert.mV = 0.f;
		i[3] = t->AddVertex(&vert);

		t->AddTri(i[0], i[3], i[2]);
		t->AddTri(i[0], i[1], i[3]);
		t->AddTri(i[0], i[2], i[3]);
		t->AddTri(i[0], i[3], i[1]);
	}

	// Render Decal bit
	ori = ID_FMATRIX;
	FVector pos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);

	float	radius = RTLaser_FLARE_SIZE;

	CCamera	*cam = ENGINE.GetCamera();
	
	GetRenderStartPosition(&p);

	FVector		a = p - GVectorToFVector(cam->mPos);
	float	ad = a.Magnitude();
	p -= a * 2.f / ad;

	// OK - now some fucked up hacks to rotate the flare
	FVector b = GVectorToFVector(cam->mLookAt - cam->mPos);
	float	bd = b.Magnitude();
	float	nbd = (a * b) / bd;
	b = b / bd * nbd;
	
	FVector x = b - a;
	FVector y = x ^ b;

	x.Normalise();
	y.Normalise();

	x *= radius;
	y *= -radius;

	for (int c1 = 0;c1 < 4; c1 ++)
	{
		vert.mPos	= p + ((c1 & 1) ? x : -x) + ((c1 & 2) ? y : -y);
		vert.mU	= (c1&1)?1.f:0.f;
		vert.mV	= (c1&2)?1.f:0.f;
		vert.mDiffuse = 0xffffffff;

		i[c1] = mTex4->AddVertex(&vert);
	}

	mTex4->AddTri(i[0], i[2], i[3]);
	mTex4->AddTri(i[0], i[3], i[1]);

}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CRTLaser::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	ta.AddTexture(mTex1, TATT_OTHER, 1,  -1, -1, BD_4, 0, TRUE);
	ta.AddTexture(mTex2, TATT_OTHER, 1,  -1, -1, BD_4, 0, TRUE);
	ta.AddTexture(mTex3, TATT_OTHER, 1,  -1, -1, BD_4, 0, TRUE);
	ta.AddTexture(mTex4, TATT_OTHER, 1,  -1, -1, BD_4, 0, TRUE);
}

//******************************************************************************************
void	CRTLaser::FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt)
{
	if (mTex1)
		ri.i0 = ta.GetTextureIndex(TATT_OTHER, mTex1);
	else
		ri.i0 = -1;

	if (mTex2)
		ri.i1 = ta.GetTextureIndex(TATT_OTHER, mTex2);
	else
		ri.i1 = -1;
	
	if (mTex3)
		ri.i2 = ta.GetTextureIndex(TATT_OTHER, mTex3);
	else
		ri.i2 = -1;

	if (mTex4)
		ri.i3 = ta.GetTextureIndex(TATT_OTHER, mTex4);
	else
		ri.i3 = -1;
}
//******************************************************************************************
void	CRTLaser::AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	ta.AddTexture(mTex1, TADCTT_OTHER, 1,  -1, -1, 0, TRUE);
	ta.AddTexture(mTex2, TADCTT_OTHER, 1,  -1, -1, 0, TRUE);
	ta.AddTexture(mTex3, TADCTT_OTHER, 1,  -1, -1, 0, TRUE);
	ta.AddTexture(mTex4, TADCTT_OTHER, 1,  -1, -1, 0, TRUE);

	SUPERTYPE::AccumulateResourcesDC(ta, ma);
}

//******************************************************************************************
void	CRTLaser::FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt)
{
	if (mTex1)
		ri.i0 = ta.GetTextureIndex(TADCTT_OTHER, mTex1);
	else
		ri.i0 = -1;

	if (mTex2)
		ri.i1 = ta.GetTextureIndex(TADCTT_OTHER, mTex2);
	else
		ri.i1 = -1;
	
	if (mTex3)
		ri.i2 = ta.GetTextureIndex(TADCTT_OTHER, mTex3);
	else
		ri.i2 = -1;

	if (mTex4)
		ri.i3 = ta.GetTextureIndex(TADCTT_OTHER, mTex4);
	else
		ri.i3 = -1;
}
#endif

//******************************************************************************************
#endif
