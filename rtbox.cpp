#include	"Common.h"

#if TARGET == PC || DC

#include	"RTBox.h"
#include	"Globals.h"

//******************************************************************************************
void	CRTBox::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEX::GetTexture(i->mName);
	ASSERT(mTex);
}

//******************************************************************************************
void	CRTBox::Render()
{
	FVector p;
	FMatrix ori;
	GetRenderPosition(&p);
	GetRenderOrientation(&ori);

	BOOL	s	= mThing->GetRenderSelected();
	SINT	o	= mThing->GetRenderColourOffset();

	ENGINE.SetupTransforms(p, ori);

	CTextureGTEX	*st = ENGINE.GetSkyTexture();

	SINT c0;

	CFVF_GTEX	vert;
	for (c0 = 0; c0 < 5; c0++)
	{
		SINT		i[4];
		SINT		si[4];

		DWORD		col;
		FVector		ldir;

		if (s)
		{
//			col = 0xffffffff;
			col = (UINT(rand())) << o;
		}
		else
		{
			// flat shading
			switch (c0)
			{
			case 0: ldir = FVector(    0,     0, 1.0f); break;
			case 1: ldir = FVector( 1.0f,     0,    0); break;
			case 2: ldir = FVector(-1.0f,     0,    0); break;
			case 3: ldir = FVector(    0,  1.0f,    0); break;
			case 4: ldir = FVector(    0, -1.0f,    0); break;
			};
			col = ENGINE.CalcLight(ori * ldir );
		}

		for (int c1 = 0;c1 < 4; c1 ++)
		{
			switch (c0)
			{
			case 0:	vert.mPos	= FVector(((c1 & 1) ?  0.5f : -0.5f), ((c1 & 2) ? -0.5f :  0.5f), -1.f		   			  ); break;
			case 1:	vert.mPos	= FVector( 0.5f					    , ((c1 & 1) ?  0.5f : -0.5f), ((c1 & 2) ?  1.f : -1.f)); break;
			case 2:	vert.mPos	= FVector(-0.5f					    , ((c1 & 1) ? -0.5f :  0.5f), ((c1 & 2) ?  1.f : -1.f)); break;
			case 3:	vert.mPos	= FVector(((c1 & 1) ? -0.5f :  0.5f),  0.5f					    , ((c1 & 2) ?  1.f : -1.f)); break;
			case 4:	vert.mPos	= FVector(((c1 & 1) ?  0.5f : -0.5f), -0.5f					    , ((c1 & 2) ?  1.f : -1.f)); break;
			};

			// err..
			if ((c0 != 0) && (c1 & 2))
			{
				vert.mPos.X /= 5.f;
				vert.mPos.Y /= 5.f;
			}

			vert.mU	= (c1&1)?1.f:0.f;
			vert.mV	= (c1&2)?0.f:1.f;
			vert.mDiffuse = col;

			i[c1] = mTex->AddVertex(&vert);

			// how about a sky reflection?
			if ((c0 == 0) && (st))
			{
				FVector d = (p - GVectorToFVector(ENGINE.GetCamera()->mPos));
				FVector v = (ori * vert.mPos) / d.Magnitude() * 3.f + d * 0.1f;
				vert.mU = (v.X / 2.f) + 0.5f ;
				vert.mV = (v.Y / 2.f) + 0.5f;
				vert.mDiffuse = 0xafafafaf;
				si[c1] = st->AddVertex(&vert);
			}
		}

		if ((c0 == 0) && (st))
		{
			st->AddTri(si[0], si[3], si[2]);
			st->AddTri(si[0], si[1], si[3]);
		}
//		else
		{
			mTex->AddTri(i[0], i[3], i[2]);
			mTex->AddTri(i[0], i[1], i[3]);
		}


	}

	mTex->DrawAll();

	ENGINE.EnableAdditiveAlpha();
	st->DrawAll();
	ENGINE.DisableAlpha();
}

//******************************************************************************************
#endif
