#include	"Common.h"

#if TARGET == PC || DC

#include	"RTFlag.h"
#include	"Globals.h"

//******************************************************************************************
void	CRTFlag::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEX::GetTexture(i->mName);
}

//******************************************************************************************

#define		RTFLAG_SIZE_X	6.0f
//#define		RTFLAG_SIZE_X	4.0f
#define		RTFLAG_SIZE_Z	1.0f
#define		RTFLAG_OFFSET_Z	5.0f
#define		RTFLAG_SIZE_Y	0.1f
#define		RTFLAG_SPEED	2.f
#define		RTFLAG_FREQ		1.f

#define		RTFLAG_NUM_SEGS	20

//******************************************************************************************
void	CRTFlag::Render()
{
	FVector p;
	FMatrix ori;
	GetRenderPosition(&p);
	GetRenderOrientation(&ori);
	float	fr	= GToF(mThing->GetRenderTurn());

	ENGINE.SetupTransforms(p, ori);

	SINT c0;

	// work out offsets.
	FVector sx = FVector(-RTFLAG_SIZE_X / 2.f, 0, 0) - FVector(0, 0, RTFLAG_OFFSET_Z);
	FVector dx = FVector(RTFLAG_SIZE_X / float(RTFLAG_NUM_SEGS), 0, 0);
	FVector dy = FVector(0, RTFLAG_SIZE_Y, 0);
	FVector dz = FVector(0, 0, RTFLAG_SIZE_Z);

	SINT i[4];

	CFVF_GTEX	vert;

//	for (c0 = 0; c0 <  1; c0++)
	for (c0 = 0; c0 < RTFLAG_NUM_SEGS + 1; c0++)
	{
		float	theta = float(c0) / RTFLAG_FREQ + fr / RTFLAG_SPEED;
		float	offy = float(sin(theta));
		UBYTE	c = UBYTE((cos(theta) + 2.f) * (0x3e));
		vert.mDiffuse = c + (c << 8) + (c << 16) + (c << 24);
		
		vert.mPos = sx + dy * offy;
//		vert.mPos = FVector(0, 0, -2.f);
		vert.mU = 0;
		vert.mV = 0;
		i[0] = mTex->AddVertex(&vert);

		vert.mPos = sx + dy * offy + dz;
//		vert.mPos = FVector(0, 0, -1.f);
		vert.mU = 0;
		vert.mV = 1.f;
		i[1] = mTex->AddVertex(&vert);

		sx += dx;
		theta = float(c0+1) / RTFLAG_FREQ + fr / RTFLAG_SPEED;
		offy = float(sin(theta));
		c = UBYTE(cos(theta) + 1.f * 0x7e);
		vert.mDiffuse = c + (c << 8) + (c << 16) + (c << 24);

		vert.mPos = sx + dy * offy;
//		vert.mPos = FVector(1.0f, 0, -2.f);
		vert.mU = 0.5f;
		vert.mV = 0;
		i[2] = mTex->AddVertex(&vert);

		vert.mPos = sx + dy * offy + dz;
//		vert.mPos = FVector(1.0f, 0, -1.f);
		vert.mU = 0.5f;
		vert.mV = 1.f;
		i[3] = mTex->AddVertex(&vert);

		mTex->AddTri(i[0], i[3], i[2]);
		mTex->AddTri(i[0], i[1], i[3]);

		
		mTex->AddTri(i[0], i[2], i[3]);
		mTex->AddTri(i[0], i[3], i[1]);
	}


	mTex->DrawAll();
}

//******************************************************************************************
#endif
