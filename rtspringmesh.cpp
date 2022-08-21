#include	"Common.h"

#if TARGET == PC || DC

#include	"RTSpringMesh.h"
#include	"Globals.h"
#include	"SpringArray.h"

//******************************************************************************************
void	CRTSpringMesh::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mBallTex = CTextureGTEX::GetTexture("ball3_1.raw");
	mWallTex = CTextureGTEX::GetTexture("copper.raw");
}

//******************************************************************************************
DWORD	tlight(DWORD &d, FVector &p1, FVector &p2, FVector &p3)
{
	FVector norm = -((p1 - p2) ^ (p1 - p3));
	norm.Normalise();

	// erm.. just use x for now...

	float	val = (norm.X * 0.4f) + 0.6f; // 0.2 - 1.0
	UBYTE	mul = UBYTE(val * 255);

	UBYTE r = UBYTE((((d & 0x00ff0000) >> 16 ) * mul) >> 8 );
	UBYTE g = UBYTE((((d & 0x0000ff00) >> 8  ) * mul) >> 8 );
	UBYTE b = UBYTE((((d & 0x000000ff) >> 0  ) * mul) >> 8 );

	DWORD l = (d & 0xff000000) + (r << 16) + (g << 8) + b;

	return l;
}


//******************************************************************************************
#define	RAD	0.1f

void	CRTSpringMesh::Render()
{
	FMatrix ori = ID_FMATRIX;
	FVector pos;
	GetRenderPosition(&pos);
	ENGINE.SetupTransforms(pos, ori);

	CCamera	*cam = ENGINE.GetCamera();
	FMatrix cori  = GMatrixToFMatrix(cam->mOrientation);
	FVector x = cori * FVector(RAD, 0, 0);
	FVector y = cori * FVector(0, RAD, 0);

	CSpringArray	*a = mThing->GetRenderSpringArray();
	if (!a)
		return;

	CSpring	*s = a->mSprings;

#if 0
	if (0) // Draw Balls
	{
		SINT	c0;
		SINT	n = a->mXSize * a->mYSize * a->mZSize;

		for	(c0 = 0; c0 < n; c0 ++)
		{
			FVector		p = s->mPos;
			DWORD		col = 0xffffffff;

			SINT		i[4];
			CFVF_GTEX	vert;

			for (int c1 = 0;c1 < 4; c1 ++)
			{
				vert.mPos	= p + ((c1 & 1) ? x : -x) + ((c1 & 2) ? y : -y);
				vert.mU	= (c1&1)?1.f:0.f;
				vert.mV	= (c1&2)?0.f:1.f;
				vert.mDiffuse = col;

				i[c1] = mBallTex->AddVertex(&vert);
			}

			mBallTex->AddTri(i[0], i[2], i[3]);
			mBallTex->AddTri(i[0], i[3], i[1]);

			s++;
		}

		mBallTex->DrawAll();
	}
#endif

	if (1) // Draw Walls
	{
		SINT	c0;
		
//		for (c0 = 0; c0 < 6; c0 ++)
		for (c0 = 0; c0 < 5; c0 ++)
		{
			SINT	ix1 = 0, iy1 = 0, iz1 = 0;
			SINT	ix2 = 0, iy2 = 0, iz2 = 0;
			SINT	sx, sy, sz;
			SINT	n1, n2;
			BOOL	clock = TRUE;

			switch(c0)
			{
			case 0:
				sx = 0;
				sy = 0;
				sz = 0;
				ix1 = 1;
				iz2 = 1;
				n1 = a->mXSize - 1;
				n2 = a->mZSize - 1;
				break;
			case 1:
				sx = 0;
				sy = 0;
				sz = a->mZSize - 1;
				ix1 = 1;
				iy2 = 1;
				n1 = a->mXSize - 1;
				n2 = a->mYSize - 1;
				break;
			case 2:
				sx = 0;
				sy = 0;
				sz = 0;
				iz1 = 1;
				iy2 = 1;
				n1 = a->mZSize - 1;
				n2 = a->mYSize - 1;
				break;
			case 3:
				sx = a->mXSize - 1;
				sy = 0;
				sz = 0;
				iz1 = 1;
				iy2 = 1;
				n1 = a->mZSize - 1;
				n2 = a->mYSize - 1;
				clock = FALSE;
				break;
			case 4:
				sx = 0;
				sy = a->mYSize-1;
				sz = 0;
				ix1 = 1;
				iz2 = 1;
				n1 = a->mXSize - 1;
				n2 = a->mZSize - 1;
				clock = FALSE;
				break;

			};

			SINT	c1, c2;

			for (c1 = 0; c1 < n1; c1 ++)
				for (c2 = 0; c2 < n2; c2 ++)
				{
					SINT		i[4];
					CFVF_GTEX	vert;

					SINT	px = sx + c1*ix1 + c2*ix2;
					SINT	py = sy + c1*iy1 + c2*iy2;
					SINT	pz = sz + c1*iz1 + c2*iz2;

					FVector p1 = GVectorToFVector(a->mSprings[a->GetSpring(px, py, pz)].mPos);
					FVector p2 = GVectorToFVector(a->mSprings[a->GetSpring(px + ix1, py + iy1, pz + iz1)].mPos);
					FVector p3 = GVectorToFVector(a->mSprings[a->GetSpring(px + ix2, py + iy2, pz + iz2)].mPos);
					FVector	p4 = GVectorToFVector(a->mSprings[a->GetSpring(px + ix1 + ix2, py + iy1 + iy2, pz + iz1 + iz2)].mPos);

					DWORD	tcol = 0xffffffff;
					DWORD	col;
					if (clock)
						col = tlight(tcol, p1, p2, p3);
					else
						col = tlight(tcol, p2, p1, p3);

					vert.mPos	= p1;
					vert.mU	= 0.f;
					vert.mV	= 0.f;
					vert.mDiffuse = col;
					i[0] = mWallTex->AddVertex(&vert);

					vert.mPos	= p2;
					vert.mU	= 1.f;
					vert.mV	= 0.f;
					vert.mDiffuse = col;
					i[1] = mWallTex->AddVertex(&vert);

					vert.mPos	= p3;
					vert.mU	= 0.f;
					vert.mV	= 1.f;
					vert.mDiffuse = col;
					i[2] = mWallTex->AddVertex(&vert);

					s = &a->mSprings[a->GetSpring(px + ix1 + ix2, py + iy1 + iy2, pz + iz1 + iz2)];
					vert.mPos	= GVectorToFVector(s->mPos);
					vert.mU	= 1.f;
					vert.mV	= 1.f;
					vert.mDiffuse = col;
					i[3] = mWallTex->AddVertex(&vert);

					if (clock)
					{
						mWallTex->AddTri(i[0], i[2], i[1]);
						mWallTex->AddTri(i[2], i[3], i[1]);
					}
					else
					{
						mWallTex->AddTri(i[0], i[1], i[2]);
						mWallTex->AddTri(i[2], i[1], i[3]);
					}


				}
		}

		mWallTex->DrawAll();

	}
}

//******************************************************************************************
#endif
