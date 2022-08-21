#include	"Common.h"

#if TARGET == PC || DC

#include	"RTLightning.h"
#include	"Globals.h"

//******************************************************************************************
void	CRTLightning::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEXAdditive::GetTexture(i->mName);
}

//******************************************************************************************

#define	RTLIGHTNING_DESIRED_SEGMENT_SIZE	1.f
#define	RTLIGHTNING_WIDTH					0.1f
#define	RTLIGHTNING_OFFSET					0.5f
#define	RTLIGHTNING_NUM_STREAMS				3

//******************************************************************************************
void	CRTLightning::Render()
{
	FVector p;
	FMatrix ori;
	GetRenderPosition(&p);
	GetRenderOrientation(&ori);

	float	fr	= GToF(mThing->GetRenderTurn());
//	float	r	= GetRenderRadius();
	
	FVector	s, e;
	GetRenderStartPosition(&s);
//	s += - p + FVector(0, 0, -r);
	s += - p;

	GetRenderEndPosition(&e);
//	e += - p + FVector(0, 0, -r);
	e += - p;

	ENGINE.SetupTransforms(p, ori);

	SINT c0, c1;

	// work out how many segments we need

	float	dist = (e - s).Magnitude();

	SINT	num_segs = SINT(dist / RTLIGHTNING_DESIRED_SEGMENT_SIZE);
//	float	seg_size = dist / float(num_segs);
	FVector	seg_offset = (e - s) / float(num_segs);
	FVector	perp = (seg_offset ^ FVector(0, 0, -1.f)).NormaliseAndScale(RTLIGHTNING_WIDTH);

	srand(ENGINE.GetRenderGameTurn()); // stays constant on pause...

	CFVF_GTEX	vert;
	for (c1 = 0; c1 < RTLIGHTNING_NUM_STREAMS; c1++)
	{
		FVector	curr_pos = s;

		for	(c0 = 0; c0 < num_segs; c0++)
		{
			SINT i[4];

			// old verts
			vert.mDiffuse = 0xffffffff;
			vert.mPos = curr_pos - perp;
			vert.mU = 0.f;
			vert.mV = 0.f;
			i[0] = mTex->AddVertex(&vert);
			vert.mPos = curr_pos + perp;
			vert.mU = 1.f;
			i[1] = mTex->AddVertex(&vert);

			// nev verts
			if (c0 != num_segs - 1)
			{
				curr_pos = s + (seg_offset * float(c0 + 1));
				
				// add random offset
				curr_pos += FVector( ((float(rand()) / float(RAND_MAX)) - 0.5f) * RTLIGHTNING_OFFSET,
									 ((float(rand()) / float(RAND_MAX)) - 0.5f) * RTLIGHTNING_OFFSET,
									 ((float(rand()) / float(RAND_MAX)) - 1.0f) * RTLIGHTNING_OFFSET);
			}
			else
				curr_pos = e;

			vert.mPos = curr_pos - perp;
			vert.mU = 0.f;
			vert.mV = 1.f;
			i[2] = mTex->AddVertex(&vert);
			vert.mPos = curr_pos + perp;
			vert.mU = 1.f;
			i[3] = mTex->AddVertex(&vert);

			mTex->AddTri(i[0], i[3], i[2]);
			mTex->AddTri(i[0], i[1], i[3]);
			mTex->AddTri(i[0], i[2], i[3]);
			mTex->AddTri(i[0], i[3], i[1]);
		}
	}


//	ENGINE.EnableAdditiveAlpha();
//	mTex->DrawAll();
//	ENGINE.DisableAlpha();

	// not needed - additives drawn automatically...
}

//******************************************************************************************
#endif
