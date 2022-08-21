#include	"Common.h"

#include	"Map.h"
#include	"Globals.h"
#include	"Thing.h"
#include	"Profile.h"
#include	"Primary.h"
#include	"Light.h"

#if TARGET == PSX
#include <mwinline.h>
#include <gtemac.h>
#ifdef _MAP_OPTIMISED
#pragma optimization_level 4
#endif
#endif

//******************************************************************************************
//******************************************************************************************
void	CCellRef::Clip()
{
	if (X < 0)				X = 0;
	if (Y < 0)				Y = 0;
	if (X >= MAP_WIDTH )	X = MAP_WIDTH  - 1;
	if (Y >= MAP_HEIGHT)	Y = MAP_HEIGHT - 1;
}

//******************************************************************************************
//******************************************************************************************
#if TARGET == PSX
#define gte_SetBackColorRatio( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_sll (12,__arg0, 4);\
   __I_ctc2 (12, 13);\
   __I_ctc2 (12, 14);\
   __I_ctc2 (12, 15);\
   __asm_end();

#endif


DWORD	CMapCell::EvaluateColour(CMapCell *other)
{
	DWORD col;

#if TARGET == PC

	float d = GToF(other->GetHeight() - GetHeight());
	if (d >  1.f) d =  1.f;
	if (d < -1.f) d = -1.f;

	SINT g;
	if(d < 0) 
	{
		g = (170 - SINT((-d) * 130.f));
	}
	else
	{
		g = (170 - SINT(d * 0.f));
	}
	SINT r;
	if(d < 0) 
	{
		r = (170 - SINT((-d) * 130.f));
	}
	else 
	{
		r = (170 + SINT(d * 80.f));
	}
	SINT b;
	if(d < 0) 
	{
		b = 170;
	}
	else
	{
		b = (170 - SINT(d * 40.f));
	}

	col = (r << 16) + (g << 8) + b;

//	col = (rand() << 16) + rand();

#elif TARGET == PSX
	//!PSX YAAAAAAAAARRGh
	GINT d = other->GetHeight() - GetHeight();
	if (d >  G1) d =  G1;
	if (d < -G1) d = -G1;

	SVECTOR nv;
	CVECTOR cv;
	nv.vx=(d.GetVal())>>4;
	nv.vy=1024;
	nv.vz=-(d.GetVal())>>4;
	gte_ldv0(&nv);
	gte_ldrgb(&mColour);
	gte_ncs();

	//!PSX
	// cheap water
    GINT wh = GAME.GetLevelData()->mGVars[WATER_HEIGHT];
	gte_strgb(&cv);
    if ((wh != G0) && (GetHeight() > GAME.GetLevelData()->mGVars[WATER_HEIGHT]))
    {
		col = ((cv.b << 16) + ((cv.g << 7)&0x007f00) + (cv.r>>1));
	}
	else
	{
		col=*((ULONG*)(&cv.r));
	}



#endif


	return col;
}

//******************************************************************************************
DWORD	CMapCell::EvaluateOriginalColour(CMapCell *other)
{
	return EvaluateColour(other);
}

//******************************************************************************************

#if TARGET == PC
 DWORD shadow(DWORD col, SINT shad)
{
	SINT r = col & 0xff0000;
	SINT g = col & 0x00ff00;
	SINT b = col & 0x0000ff;

	r = (r * shad) / 256;
	g = (g * shad) / 256;
	b = (b * shad) / 256;

	return (r & 0xff0000) + (g & 0xff00) + (b & 0xff);

}
#endif
//******************************************************************************************

 DWORD	CMapCell::GetColour()
{
#if TARGET == PC
	//!!!!!!!!!!!!!  near zero, please...
	if ((mOffset == 0) && (mDisplacement == 0))
		return shadow(mPrecalcColour, mShadow);
	// otherwise we've moved...
	return shadow(EvaluateColour(this + 1), mShadow);  //! Zoiks!
#endif
#if TARGET == PSX
	return EvaluateColour(this+1);
#endif

}


//******************************************************************************************
#if TARGET == PC
ECellType	CMapCell::GetCellSaveType(ECellType t)
#endif
#if TARGET==PSX || TARGET == DC
SBYTE	CMapCell::GetCellSaveType(SBYTE t)
#endif
{
	// restore cell types modified by objects

	switch (t)
	{
	case CT_TRIGGER_GUN:
	case CT_TRIGGER_GUN_ON:
	case CT_TRIGGER_BAD_GUN:
	case CT_TRIGGER_WAVE:
	case CT_TRIGGER_WAVE_ON:
	case CT_HEALTH:
	case CT_HEALTH_ON:
		return CT_NORMAL;
	default:
		return t;
	}
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
inline	GINT	get_height(const CCellRef &c, GINT phase, GINT height)
{
	return G0;

	// nah!
/*	GINT s0 = GSIN(G(c.X) / phase);
	GINT s1 = GSIN(G(c.Y) / phase);

	return -((s0 * s1) * height); // + 5.f;*/
}
//******************************************************************************************
void	CMap::ClearMap(GINT phase, GINT height)
{
	SINT	c0,c1;

	CMapCell	*cell = mCells;
	for (c0 = 0; c0 < MAP_HEIGHT; c0++)
		for (c1 = 0; c1 < MAP_WIDTH; c1++)
		{
			// set height
			cell->SetHeight(get_height(CCellRef(c0, c1), phase, height));
			cell->mType = CT_NORMAL;

			cell->mColour = 0x7f7f7f7f;

#if TARGET==PC
			cell->mFrame = G0;
			cell->mTextureNo = 0;
#endif
			cell->mMix = 15;			// all in channel 0;
			cell->mRamp = 0;
			cell->SetFriction(FT_NORMAL);
			cell ++ ;
		}

#if TARGET==PC
	C4x4*	ff = MAP.m4x4s;
	for(c0 = 0; c0< MAP_HEIGHT; c0+=4)
		for(c1 = 0; c1< MAP_WIDTH; c1+=4)
		{
			ff->SetTexture(0,0);
			ff->SetTexture(1,0);
			ff->SetTexture(2,0);
			ff->SetTextureMask(0,0xffff);
			ff->SetTextureMask(1,0);
			ff->SetTextureMask(2,0);
			ff++;
		}
#endif
//	RaiseEdges();
	PrecalcCells();
}

//******************************************************************************************
#if TARGET == PC
struct	ShitLight
{
	GVector	Pos;
	GVector Colour;
	GINT	OSRad;			// squared radiuses
	GINT	ISRad;

};

//******************************************************************************************
GINT CMap::GetCludgedComponent(SINT xc, SINT yc, SINT xoff, SINT yoff)
{
	SINT	checkx = xc+xoff;
	if((checkx<0) || (checkx>=MAP_WIDTH)) checkx = xc;
	SINT	checky = yc+yoff;
	if((checky<0) || (checky>=MAP_HEIGHT)) checky = yc;
	CMapCell* cell =  GetCell(CCellRef(checkx, checky));
	GINT	ret = cell->GetUnderlyingHeight();

	checkx = xc-xoff;
	if((checkx<0) || (checkx>=MAP_WIDTH)) checkx = xc;
	checky = yc-yoff;
	if((checky<0) || (checky>=MAP_HEIGHT)) checky = yc;
	cell =  GetCell(CCellRef(checkx, checky));
	ret -= cell->GetUnderlyingHeight();
	return ret;
}
//******************************************************************************************
int	CMap::RayScan(GVector* pos, GVector* step, int steps)
{
	GVector	scan = *pos;
	for(;steps>0;steps--)
	{
		scan-=*step;
		int x, y;
		x = scan.X.Whole();
		y = scan.Y.Whole();
		if((x<0)||(x>=MAP_WIDTH)||(y<0)||(y>=MAP_HEIGHT)) return 0;
 		if((GetCell(CCellRef(x, y))->GetUnderlyingHeight()<scan.Z)) return 1;
	}
	return	0;
}
//******************************************************************************************
int	CMap::NRayScan(GVector* pos, GVector* step, int steps)
{
	int ret=0;
	GVector n =*pos;
	n.X-=G(0,32768);
	n.Y-=G(0,32768);
	for(int yc=0; yc<4; yc++)
	{
		for(int xc=0; xc<4; xc++)
		{
			n.X+=G(0,16384);
			ret+=RayScan(&n, step, steps);
		}
		n.X-=G1;
		n.Y+=G(0,16384);
	}
	return 16-ret;
}
//******************************************************************************************
#define	LCSIZE	32
void	CMap::RelightLevel()
{
#if TARGET == PC

	SINT xc, yc;
/*	int	no_lights = (MAP_WIDTH/LCSIZE)*(MAP_HEIGHT/LCSIZE);
	ShitLight* shitlights = new ShitLight[no_lights];
	ShitLight* sl = shitlights;
	for(yc=0;yc<MAP_HEIGHT;yc+=LCSIZE)
	{
		for(xc=0;xc<MAP_WIDTH;sl++, xc+=LCSIZE)
		{
			sl->Pos = GVector(G(xc+8+(rand()%15),0),G(yc+8+(rand()%15),0),G0);
			sl->Pos.Z = MAP.GetMapHeight(sl->Pos) - G(25);
			sl->Colour = GVector(G(0,20000+(rand() % 32000)),G(0,20000+(rand() % 32000)),G(0,20000+(rand() % 32000)));
			sl->OSRad = G(35*35,0);
			sl->ISRad = G(25*25,0);
		}
	}*/
	int	no_lights = 0;

	CThing *t = WORLD.GetFirstThing();
	while (t)
	{
		if (t->IsLight())
			no_lights ++;
		t = t->mNextThing;
	}

	ShitLight* shitlights = new ShitLight[no_lights];
	ShitLight* sl = shitlights;

	t = WORLD.GetFirstThing();
	CEditorLight	*tl;
	while (t)
	{
		if ((tl = t->IsLight()) != NULL)
		{
			sl->Pos = tl->GetPos();
			sl->Pos.Z = MAP.GetMapHeight(sl->Pos) - G(25);
//			sl->Colour = GVector(G(0,20000+(rand() % 32000)),G(0,20000+(rand() % 32000)),G(0,20000+(rand() % 32000)));
			switch (tl->GetColour())
			{
			case 0:	sl->Colour = GVector(G1, G(0, 30000), G(0, 30000));		break;
			case 1:	sl->Colour = GVector(G(0, 30000), G1, G(0, 30000));		break;
			case 2:	sl->Colour = GVector(G(0, 30000), G(0, 30000), G1);		break;
			case 3:	sl->Colour = GVector(G1, G1, G(0, 30000));				break;
			case 4:	sl->Colour = GVector(G1, G(0, 30000), G1);		break;
			case 5:	sl->Colour = GVector(G(0, 30000), G1, G1);		break;
			case 6:	sl->Colour = GVector(G1, G1, G1);		break;
			};
			sl->OSRad = G(35*35,0);
			sl->ISRad = G(25*25,0);
			sl ++;
		}
		t = t->mNextThing;
	}


	/*
	shitlights[0].Pos = GVector(G(0,0),G(64,0),G(-30,0));
	shitlights[0].Colour = GVector(G(0,33000),G(0,33000),G(0,00000));
	shitlights[0].OSRad = G(50*50,0);
	shitlights[0].ISRad = G(10*50,0);
	*/



	GVector	scan;
	for(yc=0, scan.Y =G0; yc<MAP_HEIGHT; scan.Y+=G1, yc++)
	{
		for(xc=0, scan.X=G0; xc<MAP_WIDTH; scan.X+=G1, xc++)
		{
			GVector	col = GVector(G0,G0,G0);
			CMapCell*	cell = GetCell(CCellRef(xc, yc));
			cell->mColour = 0;
			scan.Z = cell->GetUnderlyingHeight();
			GVector surf;
			surf.Z = G1*2;
			surf.X = GetCludgedComponent(xc,yc,-1,0);
			surf.Y = GetCludgedComponent(xc,yc,0,-1);
			surf.Normalise();
			for(int lc=0; lc<no_lights; lc++)
			{
				GVector	rel = scan-shitlights[lc].Pos;
				GINT	ds = rel.MagnitudeSq();
				if(ds<=shitlights[lc].OSRad)
				{
					GINT	falloff;
					if(ds<=shitlights[lc].ISRad) falloff = G1;
					else	falloff = (shitlights[lc].OSRad-ds)/(shitlights[lc].OSRad-shitlights[lc].ISRad);
					GINT	mag = rel.Magnitude();
					BOOL inshadow = FALSE;
					if(mag!=G0)
					{
						rel /=mag;
						inshadow = NRayScan( &scan, &rel, mag.Whole());
					}
					GINT	light = rel*surf*falloff;
					int		l = (light.GetVal()*inshadow>>12); //### change to 11 for psx?

					col += shitlights[lc].Colour*l;
				}
			}
			int r = col.X.Whole();
			if (r<0)r=0; else if(r>192) r=192;
			int g = col.Y.Whole();
			if (g<0)g=0; else if(g>192) g=192;
			int b = col.Z.Whole();
			if (b<0)b=0; else if(b>192) b=192;

			// water?
			if (GAME.GetLevelData()->mGVars[WATER_HEIGHT] != G0)
				if (cell->GetUnderlyingHeight() > GAME.GetLevelData()->mGVars[WATER_HEIGHT])
				{
					r >>= 1;
					g >>= 1;

					if (b < 64) b = 64;
				}

			// map edges
			if ((xc == 0) || (yc == 0) || (xc == MAP_WIDTH - 1) || (yc == MAP_HEIGHT - 1))
			{
				r = g = b = 0;
			}

			cell->mColour = (r<<16)+(g<<8)+b;
		}
	}

	for(yc = 0; yc < MAP_HEIGHT - 1; yc++)
	{
		for(xc = 0; xc < MAP_WIDTH - 1; xc++)
		{
			// even more bodges
			// lower the resolution of the lights so that when we snap to 2x2s there is less change.
			SINT		scol1, scol2, r1, g1, b1, r2, g2, b2, r3, g3, b3;
			CCellRef	ref1, ref2;

			if ((xc | yc) & 1)
			{
				if (xc & 1)
				{
					if (yc & 1)
					{
						ref1 = CCellRef(xc - 1, yc - 1);
						ref2 = CCellRef(xc + 1, yc + 1);
					}
					else
					{
						ref1 = CCellRef(xc - 1, yc);
						ref2 = CCellRef(xc + 1, yc);
					}
				}
				else if (yc & 1)
				{
					ref1 = CCellRef(xc, yc - 1);
					ref2 = CCellRef(xc, yc + 1);
				}

				scol1 = MAP.GetCell(ref1)->mColour;
				scol2 = MAP.GetCell(ref2)->mColour;

				r1 = (scol1 >> 16) & 0xff;
				g1 = (scol1 >>  8) & 0xff;
				b1 = (scol1      ) & 0xff;

				r2 = (scol2 >> 16) & 0xff;
				g2 = (scol2 >>  8) & 0xff;
				b2 = (scol2      ) & 0xff;

				r3 = (r1 + r2) / 2;
				g3 = (g1 + g2) / 2;
				b3 = (b1 + b2) / 2;

				MAP.GetCell(CCellRef(xc, yc))->mColour = (r3 << 16) + (g3 << 8) + b3;
			}
		}
	}

	delete[] shitlights;

#endif
}

//******************************************************************************************
void	CMap::UnlightLevel()
{
	SINT	c0, c1;

	for (c0 = 0; c0 < MAP_HEIGHT; c0 ++)
	{
		for (c1 = 0; c1 < MAP_WIDTH; c1 ++)
		{
			GetCell(CCellRef(c1, c0))->mColour = 0x7f7f7f7f;
		}
	}
}

//******************************************************************************************
void	CMap::LightCells()
{
	SINT	c0, c1, l0, l1;

	// simple for now - scan through map, adding light.

	for (l0 = 1; l0 < MAP_HEIGHT - 1; l0++)
	{
		for (l1 = 1; l1 < MAP_WIDTH - 1; l1++)
		{
			//! errr.....
			BOOL	light = FALSE;
			SINT	r, g, b;

			if (((l0 & 0x1f) == 3) && ((l1 & 0x1f) == 3))
//			if (((l0 & 0xf) == 0) && ((l1 & 0xf) == 0))
			{
				light = TRUE;
				r = g = b = 0x7f;
			}
			if (((l0 & 0x1f) == 0x13) && ((l1 & 0x1f) == 0x13))
//			if (((l0 & 0xf) == 0x7) && ((l1 & 0xf) == 0x7))
			{
				light = TRUE;
				r = (rand() % 0x7f);
				g = (rand() % 0x7f);
				b = (rand() % 0x7f);
//				r = (rand() % 0x3f) + 0x3f;
//				g = (rand() % 0x3f) + 0x3f;
//				b = (rand() % 0x3f) + 0x3f;
			}

			if (light)
			{
				GVector	lpos = GVector(G(l1), G(l0), -G(0));
				for (c0 = 0; c0 < MAP_HEIGHT; c0++)
				{
					for (c1 = 0; c1 < MAP_WIDTH; c1++)
					{
						CMapCell	*cell = GetCell(CCellRef(c1, c0));

						GVector	cpos = GVector(G(c1), G(c0), cell->GetUnderlyingHeight());

						GINT	d = ((cpos - lpos) / G(10)).Magnitude();
						if (d < G(0, 1000)) d = G(0, 1000);

						GINT	v = G(0, 20000) / (d * d);
//						GINT	v = G(0, 10000) / (d * d);

						GINT	nl = (((lpos - cpos) / G(10)) * MAP.GetMapNormal(cpos)) / d;
						if (nl > G0)
							v *= nl;

						if (v > G1) v = G1;

						SINT	dr = (v * r).Whole();
						SINT	dg = (v * g).Whole();
						SINT	db = (v * b).Whole();

						DWORD	a = cell->mColour;

						SINT	aa = (a >> 24) & 0xff;
						SINT	ar = (a >> 16) & 0xff;
						SINT	ag = (a >> 8 ) & 0xff;
						SINT	ab = (a      ) & 0xff;

						ar += dr;
						ag += dg;
						ab += db;

//						if (ar > 0x7f) ar = 0x7f;
//						if (ag > 0x7f) ag = 0x7f;
//						if (ab > 0x7f) ab = 0x7f;
//						if (ar > 0xff) ar = 0xff;
//						if (ag > 0xff) ag = 0xff;
//						if (ab > 0xff) ab = 0xff;
						if (ar > 0xbf) ar = 0xbf;
						if (ag > 0xbf) ag = 0xbf;
						if (ab > 0xbf) ab = 0xbf;

						cell->mColour = (aa << 24) + (ar << 16) + (ag << 8) + ab;
					}
				}
			}
		}
	}
}

#endif

//******************************************************************************************
void	CMap::PrecalcCells()
{
	SINT	c0,c1;
	CMapCell	*cell = mCells;

	mLowestPoint = GINT_MIN;

	LockEdges();

	for (c0 = 0; c0 < MAP_HEIGHT; c0++)
		for (c1 = 0; c1 < MAP_WIDTH; c1++)
		{
			// initial colour
#if TARGET==PC
			cell->mPrecalcColour = cell->EvaluateOriginalColour(cell + 1);
#endif
			cell->ClearFixedFlag();
			//cell->mFixed = FALSE;
			if ((cell->GetUnderlyingHeight() > mLowestPoint) && (cell->GetUnderlyingHeight() != PIT_HEIGHT))
				mLowestPoint=cell->GetUnderlyingHeight();

			cell ++ ;
		}

	cell = mCells;
	for (c0 = 0; c0 < MAP_HEIGHT; c0++)
		for (c1 = 0; c1 < MAP_WIDTH; c1++)
		{
			// fixed?
			if ((cell->mType == CT_ICE   ) ||
				(cell->mType == CT_BOOSTR) ||
				(cell->mType == CT_BOOSTL) ||
				(cell->mType == CT_BOOSTU) ||
				(cell->mType == CT_BOOSTD))
			{
				cell->SetFixedFlag();
				//cell->mFixed = TRUE;

				if (c0 < MAP_HEIGHT - 1)
					(cell + MAP_WIDTH)->SetFixedFlag();
					//(cell + MAP_WIDTH)->mFixed = TRUE;

				if (c1 < MAP_WIDTH - 1)
					(cell + 1)->SetFixedFlag();
					//(cell + 1)->mFixed = TRUE;
				if ((c0 < MAP_HEIGHT - 1) && (c1 < MAP_WIDTH - 1))
					(cell + MAP_WIDTH + 1)->SetFixedFlag();
					//(cell + MAP_WIDTH + 1)->mFixed = TRUE;
			}

			cell ++ ;
		}

#if TARGET == PC
//	QuickLightCells();
#endif
	mPinScrollPos = 0;
}

//******************************************************************************************

#define MAP_BOUNDARY_HEIGHT1	-GINT(1,  39321)
#define MAP_BOUNDARY_HEIGHT2	-GINT(1,  0    )
#define MAP_BOUNDARY_HEIGHT3	-GINT(0,  26214)

void	CMap::RaiseEdges()
{
	// redundant
	SINT	c0,c1;
	CMapCell	*cell = mCells;

	for (c0 = 0; c0 < MAP_HEIGHT; c0++)
		for (c1 = 0; c1 < MAP_WIDTH; c1++)
		{
			if ((c0 == 2) || (c1 == 2) || (c0 == MAP_HEIGHT - 3) || (c1 == MAP_WIDTH - 3))
				cell->SetHeight(MAP_BOUNDARY_HEIGHT3);
			if ((c0 == 1) || (c1 == 1) || (c0 == MAP_HEIGHT - 2) || (c1 == MAP_WIDTH - 2))
				cell->SetHeight(MAP_BOUNDARY_HEIGHT2);
			if ((c0 == 0) || (c1 == 0) || (c0 == MAP_HEIGHT - 1) || (c1 == MAP_WIDTH - 1))
				cell->SetHeight(MAP_BOUNDARY_HEIGHT1);

			cell++;
		}
}

void	CMap::LockEdges()
{
	SINT	c0,c1;
	CMapCell	*cell = mCells;

	for (c0 = 0; c0 < MAP_HEIGHT; c0++)
		for (c1 = 0; c1 < MAP_WIDTH; c1++)
		{
			if ((c0 == 0) || (c1 == 0) || (c0 == MAP_HEIGHT - 1) || (c1 == MAP_WIDTH - 1))
				cell->SetHeight(G0);

			cell++;
		}
}

//******************************************************************************************
void	CMap::ResetOffsets()
{
	SINT	c0;

	CMapCell	*cell = mCells;
	for (c0 = 0; c0 < MAP_WIDTH * MAP_HEIGHT; c0++)
	{
		cell->mOffset = 0;
		cell->mOffsetVelocity = 0;
		cell->mDisplacement = 0;
		//cell->mShadow = 255;
		cell ++ ;
	}

#if TARGET == PSX
	MAP.ClearOtherFlags();
#endif

	// setup particle systems
	mParticleSystem.Activate();
	mEffectParticleSystem.Activate();
}

//******************************************************************************************
//******************************************************************************************
BOOL	CMap::InitCommon()
{
	MAP.Resize(6, 6);

	Reset();

	return TRUE;
}

//******************************************************************************************
void	CMap::Reset()
{
	ResetOffsets();
	ClearMap(G(5), G(5));

	// clear mapwhos
	SINT	c0;
	for (c0 = MAP_LAYER_MIN; c0 <= MAP_LAYER_MAX; c0++)
	{
		SINT size_x = MAP_WIDTH / (1 << c0);
		SINT size_y = MAP_HEIGHT / (1 << c0);
#if TARGET == PSX
		jcl_memset(mMapWhos[c0 - MAP_LAYER_MIN], 0, sizeof(CThing *) * size_x * size_y);
#elif TARGET == PC
		memset(mMapWhos[c0 - MAP_LAYER_MIN], 0, sizeof(CThing *) * size_x * size_y);
#endif
	}
}

//******************************************************************************************
void	CMap::Shutdown()
{
	SINT	c0;
	for (c0=MAP_LAYER_MIN; c0<=MAP_LAYER_MAX; c0++)
	{
		if (mMapWhos[c0-MAP_LAYER_MIN])
			delete [] mMapWhos[c0-MAP_LAYER_MIN];
		mMapWhos[c0-MAP_LAYER_MIN] = NULL;
	}

	if (mCells)
		delete [] mCells;
	mCells = NULL;

	MAP.ShutdownPlatform();
}

//******************************************************************************************
BOOL	CMap::FallenOffMap(GVector v)
{
	return (((COORDS_INVALID(CCellRef(v))) && (v.Z > G(5))) ||
			(v.Z > mLowestPoint + GINT(10, 0)));
}


//******************************************************************************************
//#define	FADEWAVE_SIZE 16384
//SWORD	fadewave[FADEWAVE_SIZE];
//******************************************************************************************
#define	PINSHIFT	2
#define	PINSIZE		(1<<PINSHIFT)
void CMap::PinMap(Pin*	pin, int pinmod, CMapCell* cell, int mapmod, int xpins, int ypins)
{
	SWORD*	fadewave=MAP.mFadeWave;
	SWORD	scroll=MAP.mPinScrollPos+=MAP.mPinScroll;
	for(int	yc=ypins; yc; pin+=pinmod-xpins, cell+=(mapmod*PINSIZE)-(xpins*PINSIZE), yc--)
	{
		for(int xc=xpins; xc; pin++, cell+=PINSIZE-mapmod*PINSIZE, xc--)
		{
			Pin		left= pin[0];
			Pin		dx	= (pin[1]-left)>>PINSHIFT;
			Pin		dy	= (pin[pinmod]-left)>>PINSHIFT;
			Pin		ddx	= (((pin[pinmod+1]-pin[pinmod])>>PINSHIFT)-dx)>>PINSHIFT;

			for(int iyc=PINSIZE; iyc; cell+=mapmod-PINSIZE, left+=dy, dx+=ddx, iyc--)
			{
				Pin	pos = left;
				for(int ixc=PINSIZE; ixc; cell++, pos+=dx, ixc--)
				{
					SWORD	newoff;
					int		angle = ((pos.V>>4)+scroll)&4095;

					newoff =(((fadewave[angle])>>8)*(pos.U>>4))>>6;
					*((SWORD*)&cell->mOffsetVelocity) = ((newoff-*((SWORD*)&cell->mOffset)));
					*((SWORD*)&cell->mOffset) = newoff;
				}
			}
		}
	}
}
//******************************************************************************************
//static	BOOL pinmapsetup=FALSE; // boj boj boj
//Pin	pinmap[5*129];		// cos it's slightly bigger than 9*65 or 17*33
void CMap::NonSHMProcess()
{
/*	if(!pinmapsetup)
	{
		Pin*	pin = MAP.mPinMap;
		for(int yc = 33, y=-8<<8; yc; y+=1<<8, yc--)
		{
			for(int xc = 17, x=-8<<8; xc; x+=1<<8, xc--)
			{
				GINT	g;
				g.SetVal(x*x+y*y);
				int r = GROOT(g).GetVal();    // between 0  and 32<<16
				pin->V = (BATAN(x,y)<<5);//-(r>>3);

				r=BSIN((r/180)-512)/2;



				if(r<0) r=0;
				pin->U = r;
				pin++;
			}
		}
		for(int i=0; i<FADEWAVE_SIZE; i++)
		{
			fadewave[i]=BSIN(i*2/4)/2;///((FADEWAVE_SIZE+800-i)/256);
		}


		pinmapsetup = TRUE;
	}
	*/
#if TARGET==PC
	int		xps = MAP_WIDTH/4;
	if(MAP.mPinMapXSize<xps) xps = MAP.mPinMapXSize;
	int		yps = MAP_HEIGHT/4;
	if(MAP.mPinMapYSize<yps) yps = MAP.mPinMapYSize;

	PinMap(MAP.mPinMap, MAP.mPinMapXSize+1, mCells, MAP_WIDTH, xps, yps);
#endif
}

//******************************************************************************************
#define MAP_VELOCITY_SCALAR	GINT(0, 19660)
#define MAP_VELOCITY_SCALAR_SINT (19660>>(16-HEIGHT_SHIFT))
#define MAP_VELOCITY_DAMPING GINT(0, 62259)
#define MAP_VELOCITY_DAMPING_SINT (62259>>(16-HEIGHT_SHIFT))

#define SHABS(a) (((a)^((a) >>31))-((a) >>31))




void	CMap::Process()
{
	mParticleSystem.Process();
	mEffectParticleSystem.Process();

	if(mPinAnim)
	{
		NonSHMProcess();
		return;
	}

	SINT	mpsx, mpsy, mpex, mpey;

#if TARGET == PC || TARGET == DC
	mpsx = 1;
	mpsy = 1;
	mpex = MAP_WIDTH - 1;
	mpey = MAP_HEIGHT - 1;
#elif TARGET == PSX

	#define MPW		32
	GVector cpos = GAME.GetCamera(0)->mLookAt;  //!!!!!!!AAAAAAAAAAAAAAAAAARGHHHHHHHHHH
	mpsx = cpos.X.Whole() - MPW / 2;
	mpsy = cpos.Y.Whole() - MPW / 2;

	if (mpsx > MAP_WIDTH  - 1 - MPW)  mpsx = MAP_WIDTH  - 1 - MPW;
	if (mpsy > MAP_HEIGHT - 1 - MPW)  mpsy = MAP_HEIGHT - 1 - MPW;
	if (mpsx < 1) mpsx = 1;
	if (mpsy < 1) mpsy = 1;

	mpex = mpsx + MPW;
	mpey = mpsy + MPW;

	if (mpex > MAP_WIDTH  - 1)  mpex = MAP_WIDTH  - 1;
	if (mpey > MAP_HEIGHT - 1)  mpey = MAP_HEIGHT - 1;
#endif

	CMapCell	*cell = mCells + MAP_WIDTH + 1;
    SINT        c0, c1;

#if TARGET == PSX
	if (!(GAME.IsMultiPlayer()))
#endif
	{
		PROFILE_FN(ProcessMapSt1);
		for (c0 = mpsy; c0 < mpey; c0++)
		{
            cell = GetCell(CCellRef(mpsx, c0));
#if 1
            CMapCell *cellmwid=cell-MAP_WIDTH;
            CMapCell *cellpwid=cell+MAP_WIDTH;
#endif
#if TARGET == PSX

			register SINT tl,t,tr,l,m,r,bl,b,br;

			//Pre-read the 9 values...
			tl=(cellmwid-1)->mOffset;
			 t=(cellmwid  )->mOffset;
			tr=(cellmwid+1)->mOffset;
			 l=(cell    -1)->mOffset;
			 m=(cell      )->mOffset;
			 r=(cell    +1)->mOffset;
			bl=(cellpwid-1)->mOffset;
			 b=(cellpwid  )->mOffset;
			br=(cellpwid+1)->mOffset;

#endif                                  // TARGET == PSX

            for (c1 = mpsx; c1 < mpex; c1++)
			{
                if (cell->CanIMove())
				{
#if TARGET == PSX
//					__asm_start();
					register SINT d = t+b+l+r;
					start_mult((6062>>(16-HEIGHT_SHIFT)),d);
					register SINT d2 = (tl+bl+tr+br)>>4;
					tl=t;
					t=tr;
					l=m;
					d=get_mult();
					start_mult((24248>>(16-HEIGHT_SHIFT)),m);
					d=(d>>HEIGHT_SHIFT)+d2;
					m=r;
					bl=b;
					b=br;
					tr=(cellmwid+2)->mOffset;
					r=(cell+2)->mOffset;
					br=(cellpwid+2)->mOffset;
					d2=get_mult();
					d=d+(d2>>HEIGHT_SHIFT);
//					__asm_end();
#else                                  // TARGET == PSX
					SINT d = (((((cellmwid    )->mOffset +
                                 (cellpwid    )->mOffset +
								 (cell - 1            )->mOffset +
								 (cell + 1            )->mOffset  ) * (6062>>(16-HEIGHT_SHIFT) ))>>HEIGHT_SHIFT) +

                              ((SINT)((cellmwid - 1)->mOffset +
                                (cellmwid + 1)->mOffset +
                                (cellpwid - 1)->mOffset +
                                (cellpwid + 1)->mOffset  ) >> 4) +

							  (((SINT)  (cell          ->mOffset)    * (24248>>(16-HEIGHT_SHIFT)))>>HEIGHT_SHIFT));
#endif
					SINT mov=cell->mOffsetVelocity;
					SINT mo=cell->mOffset;

					cell->mOffsetVelocity = SWORD(((mov*MAP_VELOCITY_DAMPING_SINT)+((d-mo)*MAP_VELOCITY_SCALAR_SINT))>>(HEIGHT_SHIFT));

					//cell->mOffsetVelocity *= MAP_VELOCITY_DAMPING;
					//cell->mOffsetVelocity += (d - cell->mOffset) * MAP_VELOCITY_SCALAR;


				}
				cell++;
#if 1
                cellmwid++;
                cellpwid++;
#endif
            }
//			cell += 2;
		}
	}

#if TARGET == PSX
	if (!GAME.IsMultiPlayer())
	{

		// clear outer ring.

		// left edge
		cell = GetCell(CCellRef(mpsx - 1, mpsy - 1));
		for (c0 = mpsy - 1; c0 <= mpey; c0 ++)
		{
			cell->mOffset = 0;
			cell->mOffsetVelocity = 0;
			cell += MAP_WIDTH;
		}

		// right edge
		cell = GetCell(CCellRef(mpex, mpsy - 1));
		for (c0 = mpsy - 1; c0 <= mpey; c0 ++)
		{
			cell->mOffset = 0;
			cell->mOffsetVelocity = 0;
			cell += MAP_WIDTH;
		}

		// bottom edge
		cell = GetCell(CCellRef(mpsx - 1, mpsy - 1));
		for (c0 = mpsx - 1; c0 <= mpex; c0 ++)
		{
			cell->mOffset = 0;
			cell->mOffsetVelocity = 0;
			cell ++;
		}

		// top edge
		cell = GetCell(CCellRef(mpsx - 1, mpey));
		for (c0 = mpsx - 1; c0 <= mpex; c0 ++)
		{
			cell->mOffset = 0;
			cell->mOffsetVelocity = 0;
			cell ++;
		}
	}
#endif



//	cell = mCells + MAP_WIDTH + 1;
	{
		PROFILE_FN(ProcessMapSt2);
		for (c0 = mpsy; c0 < mpey; c0++)
		{
			cell = GetCell(CCellRef(mpsx, c0));
			for (c1 = mpsx; c1 < mpex; c1++)
			{
				if (cell->CanIMove())
				{
					cell->mOffset+=cell->mOffsetVelocity;

					if ((SHABS(cell->mOffset) < (655>>(16-HEIGHT_SHIFT))) && (SHABS(cell->mOffsetVelocity) <  (655>>(16-HEIGHT_SHIFT))))
					{
						cell->mOffset=0;
						cell->mOffsetVelocity=0;
					}
//#endif
				}
				else
					cell->mOffsetVelocity = 0;
//				cell->mShadow = 255; // reset - objects write into this every frame...
				// bugger - can't do it here..

				cell++;
			}
//			cell += 2;
		}
	}


#define	DROP_SIZE_SINT	(-4<<HEIGHT_SHIFT)
#define	DROP_SIZE  G(-4)
//#define	DROP_SIZE_SINT	(-5<<HEIGHT_SHIFT)
//#define	DROP_SIZE  G(-5)

#define DROP_SHAPE G(0, 45875)
#define DROP_SHAPE_SINT (45875>>(GINT_SHIFT-HEIGHT_SHIFT))
#define DROP_SIZE2  ((DROP_SIZE) * DROP_SHAPE)
#define DROP_SIZE2_SINT  (((DROP_SIZE_SINT) * DROP_SHAPE_SINT)>>(HEIGHT_SHIFT))

	// raindrops
	SINT v = GAME.GetLevelData()->mIVars[IV_RAINDROP_FREQUENCY];

	if (v != 0)
	{
		if (RANDOM() % v == 0)
		{
			SINT x = (RANDOM() % (MAP_WIDTH  - 4)) + 2;
			SINT y = (RANDOM() % (MAP_HEIGHT - 4)) + 2;

			Raindrop(CCellRef(x, y), G1);
		}
	}

	// Pursuit?
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_PURSUIT)
		ProcessPursuit();
}

//******************************************************************************************

#define	PURSUIT_SPEED	10
#define	PURSUIT_SIZE	8

void	CMap::ProcessPursuit()
{
	if (!(GAME.IsGameRunning()))
		return;

	// progressively lower map based on gameturn
	SINT	i = GAME.GetGameTimeSinceStart();   // UNBODGE

	SINT	start = i / PURSUIT_SPEED;
	SINT	m = i - (start * PURSUIT_SPEED);

	GINT	val = G(m) * G_PI / PURSUIT_SIZE / PURSUIT_SPEED;
//	GINT	val = G0;

	GINT	wh = GAME.GetLevelData()->mGVars[WATER_HEIGHT];
	GINT	desth = wh + G(3);

	SINT	c0, c1;

	//! NEED TO SET MAX & MIN cells, and change water flag on PSX.

	for (c0 = 0; c0 < PURSUIT_SIZE + 1; c0 ++)
	{
		GINT	mod;

		if (val < G_PI)
			mod = GCOS(val);
		else
			mod = -G1;

		mod = GINT_HALF - (mod * GINT_HALF);

		SINT	row = start - c0;
		if ((row > c0) && (row < MAP_HEIGHT - 1 - c0))
		{
			for (c1 = c0 + 1; c1 < MAP_WIDTH - c0 - 1; c1 ++)
			{
				CMapCell *cell = GetCell(CCellRef(c1, row));

				cell->SetDisplacement((desth - cell->GetUnderlyingHeight()) * mod);
			}
		}
		val += G_PI / PURSUIT_SIZE;
	}

#if TARGET == PSX
	// need to set flags in the PSX 4x4 map for water and visibility
	if (start < MAP_HEIGHT - 1)
		MAP.SetRowWCCheckAndMaxHeight(start>>2 , desth);

	SINT	v = start - PURSUIT_SIZE - 1;
	if ((v > PURSUIT_SIZE) && (v < MAP_HEIGHT - PURSUIT_SIZE - 1))
		MAP.SetRowWCAlwaysAndMinHeight(v>>2, desth);
#endif

	// sound!  Rummmmmmmble.

	// ensure the sample is playing
	SOUND.PlaySample(SOUND_RUMBLE, G(1), TRUE, G(0, 1));

	// work out distance.
	GINT	dist = WORLD.GetPrimaryPosition(0).Y - (G(start + PURSUIT_SIZE));
	SINT	vol = 20 - dist.Whole();
	if (vol > 20) vol = 20;
	if (vol < 0 ) vol = 0;
	vol *= 6;

	SOUND.FadeTo(SOUND_RUMBLE, G(vol) / G(127), G(0, 5000));
}

//******************************************************************************************
// return interpolated height value

#if TARGET == PSX
SINT intpl(SINT v1,SINT v2,SINT rx)
{
	VECTOR vec1,vec2;
	vec1.vx=v1>>4;
	vec2.vx=v2>>4;
	LoadAverage0(&vec1,&vec2,rx>>4,4096-(rx>>4),&vec1);
	return vec1.vx;
}
#endif


// return z value of input vector if map is not moving
//******************************************************************************************
GINT	CMap::GetMapHeightDifference(const GVector &v)
{
	CCellRef c = CCellRef(v);

//	VERIFY_COORDS(c);
	if (COORDS_INVALID(c))
		return G0;

	CMapCell* mc = GetCell(c) ;

	if (mc->GetOffsetVelocity() == G0 &&
		mc->GetDisplacement() == G0 )
	{
		return v.Z ;
	}

	return GetMapHeight(v) ;
}

//******************************************************************************************
GINT	CMap::GetMapHeight(const GVector &v)
{
	CCellRef c = CCellRef(v);

	if (COORDS_INVALID(c))
		return G0;


	GINT	rx = v.X - G(c.X);
	GINT	ry = v.Y - G(c.Y);

	SINT	x1 = c.X + 1;
	SINT	y1 = c.Y + 1;

	if (x1 >= MAP_WIDTH ) x1 = MAP_WIDTH  - 1;
	if (y1 >= MAP_HEIGHT) y1 = MAP_HEIGHT - 1;

	GINT	v1 = (GetCell(CCellRef(c.X, c.Y ))->GetHeight() * (G1 - rx)) + (GetCell(CCellRef(x1, c.Y))->GetHeight() * rx);
	GINT	v2 = (GetCell(CCellRef(c.X, y1  ))->GetHeight() * (G1 - rx)) + (GetCell(CCellRef(x1, y1 ))->GetHeight() * rx);

	//SINT v1=intpl(GetCell(CCellRef(c.X, c.Y ))->GetHeight().GetVal(),GetCell(CCellRef(x1, c.Y))->GetHeight().GetVal(),rx.GetVal());
	//SINT v2=intpl(GetCell(CCellRef(c.X, y1  ))->GetHeight().GetVal(),GetCell(CCellRef(x1, y1 ))->GetHeight().GetVal(),rx.GetVal());

	return	(v1 * (G1 - ry)) + (v2 * ry);
}

//******************************************************************************************
GINT	CMap::GetMapHeightOffset(const GVector &v)
{
	CCellRef c = CCellRef(v);

//	VERIFY_COORDS(c);
	if (COORDS_INVALID(c))
		return G0;

	GINT	rx = v.X - G(c.X);
	GINT	ry = v.Y - G(c.Y);

	GINT	v1 = (GetCell(CCellRef(c.X, c.Y    ))->GetCombinedOffset() * (G1 - rx)) + (GetCell(CCellRef(c.X + 1, c.Y    ))->GetCombinedOffset() * rx);
	GINT	v2 = (GetCell(CCellRef(c.X, c.Y + 1))->GetCombinedOffset() * (G1 - rx)) + (GetCell(CCellRef(c.X + 1, c.Y + 1))->GetCombinedOffset() * rx);

	return	(v1 * (G1 - ry)) + (v2 * ry);
}

//******************************************************************************************
GINT	CMap::GetMapHeightOriginal(const GVector &v)
{
	CCellRef c = CCellRef(v);

	if (COORDS_INVALID(c))
	return G0;


	GINT	rx = v.X - G(c.X);
	GINT	ry = v.Y - G(c.Y);

	GINT	v1 = (GetCell(CCellRef(c.X, c.Y    ))->GetUnderlyingHeight() * (G1 - rx)) + (GetCell(CCellRef(c.X + 1, c.Y    ))->GetUnderlyingHeight() * rx);
	GINT	v2 = (GetCell(CCellRef(c.X, c.Y + 1))->GetUnderlyingHeight() * (G1 - rx)) + (GetCell(CCellRef(c.X + 1, c.Y + 1))->GetUnderlyingHeight() * rx);

	return	(v1 * (G1 - ry)) + (v2 * ry);
}

//******************************************************************************************
// return map cell normal
#if TARGET == PSX
inline void gte_SaveMatrix(void)
{
	register tmp1,tmp2,tmp3;
}

inline void gte_RestoreMatrix(void)
{
	register tmp1,tmp2,tmp3;
	asm volatile("	mfc2	%0,$0
					mfc2	%1,$2
					mfc2	%2,$4
					ctc2	%0,$0
					ctc2	%1,$2
					ctc2	%2,$4":"r"(tmp1),"r"(tmp2),"r"(tmp3)::);
}


void 	GNormal(GVector *a,GVector *b,GVector *out)
{
			//VECTOR me,you;
			register int tmp1,tmp2,tmp3,tmp4,tmp5,tmp6;
			//me.vx=a->X.mVal>>4;
			//me.vy=a->Y.mVal>>4;
			//me.vz=a->Z.mVal>>4;
			//you.vx=b->X.mVal>>4;
			//you.vy=b->Y.mVal>>4;
			//you.vz=b->Z.mVal>>4;
		asm volatile("	cfc2	%0,$0
						cfc2	%1,$2
						cfc2	%2,$4":"r"(tmp4),"r"(tmp5),"r"(tmp6)::);

			//gte_ldopv1(a);

		asm volatile("
						lw		%1,0(%0)
						lw		%2,4(%0)
						lw		%3,8(%0)
						sra		%1,%1,4
						sra		%2,%2,4
						sra		%3,%3,4
						ctc2	%1,$0
						ctc2	%2,$2
						ctc2	%3,$4
						"::"r"(a),"r"(tmp1),"r"(tmp2),"r"(tmp3):"memory");
			//gte_ldopv2(b);
		asm volatile("
						lw		%1,0(%0)
						lw		%2,4(%0)
						lw		%3,8(%0)
						sra		%1,%1,4
						sra		%2,%2,4
						sra		%3,%3,4
						mtc2	%1,$9
						mtc2	%2,$10
						mtc2	%3,$11
						"::"r"(b),"r"(tmp1),"r"(tmp2),"r"(tmp3):"memory");
		gte_op12();
		asm volatile("	mfc2	%1,$25
						mfc2	%2,$26
						mfc2	%3,$27
						sll		%1,%1,4
						sll		%2,%2,4
						sll		%3,%3,4
						sw		%1,0(%0)
						sw		%2,4(%0)
						sw		%3,8(%0)"::"r"(out),"r"(tmp1),"r"(tmp2),"r"(tmp3):"memory");

			//gte_OuterProduct12(&me,&you,out)
			//out->X.mVal<<=4;
			//out->Y.mVal<<=4;
			//out->Z.mVal<<=4;
		asm volatile("	ctc2	%0,$0
						ctc2	%1,$2
						ctc2	%2,$4"::"r"(tmp4),"r"(tmp5),"r"(tmp6):);
}
#endif

GVector	CMap::GetMapNormal(const GVector &v)
{
	//! hmmm. kinda hard to return the normal of a non co-planar quad.
	// so I'll take the average of the two triangle normals; whatever that means...

#if TARGET==PSX
	CCellRef c = CCellRef(v);

	if (COORDS_INVALID(c))
		return GVector(G0, G0, -G1);

	GVector v1, v2, n1, n2, n3;

	v1 = GVector(G(-1), G0, GetCell(CCellRef(c.X, c.Y))->GetHeight()-GetCell(CCellRef(c.X + 1, c.Y    ))->GetHeight());
	v2 = GVector(G0, G(-1), GetCell(CCellRef(c.X, c.Y))->GetHeight()-GetCell(CCellRef(c.X    , c.Y + 1))->GetHeight());

	//sniff...
	v1.Normalise();
	v2.Normalise();
	GNormal(&v2,&v1,&n1);

	//n1 = v2 ^ v1;

	n1.Normalise();

	v1 = GVector(G1 , G0 , GetCell(CCellRef(c.X + 1, c.Y + 1))->GetHeight()-GetCell(CCellRef(c.X    , c.Y + 1))->GetHeight());
	v2 = GVector(G0 , G1 , GetCell(CCellRef(c.X + 1, c.Y + 1))->GetHeight()-GetCell(CCellRef(c.X + 1, c.Y    ))->GetHeight());
	v1.Normalise();
	v2.Normalise();


	//n2 = v2 ^ v1;

	GNormal(&v2,&v1,&n2);

	n2.Normalise();

	n3 = n1 + n2;  // hyeah, right...

	n3.Normalise();

	return n3;
#else
	CCellRef c = CCellRef(v);

	if (COORDS_INVALID(c))
		return GVector(G0, G0, -G1);

	GVector v1, v2, n1, n2, n3;

	v1 = GVector(G(-1), G0, GetCell(CCellRef(c.X, c.Y))->GetHeight()-GetCell(CCellRef(c.X + 1, c.Y    ))->GetHeight());
	v2 = GVector(G0, G(-1), GetCell(CCellRef(c.X, c.Y))->GetHeight()-GetCell(CCellRef(c.X    , c.Y + 1))->GetHeight());

	n1 = v2 ^ v1;

	n1.Normalise();

	v1 = GVector(G1 , G0 , GetCell(CCellRef(c.X + 1, c.Y + 1))->GetHeight()-GetCell(CCellRef(c.X    , c.Y + 1))->GetHeight());
	v2 = GVector(G0 , G1 , GetCell(CCellRef(c.X + 1, c.Y + 1))->GetHeight()-GetCell(CCellRef(c.X + 1, c.Y    ))->GetHeight());

	n2 = v2 ^ v1;

	n2.Normalise();

	n3 = n1 + n2;  // hyeah, right...

	n3.Normalise();

	return n3;

#endif
}

//******************************************************************************************
// return map cell velocity

GINT	CMap::GetMapVelocity(const GVector &v)
{
	// Similar to the normals - just take the average of all the cell velocities
	CCellRef c = CCellRef(v);

//	VERIFY_COORDS(c);
		if (COORDS_INVALID(c))
		return G0;


	GINT vel;

	vel  = GetCell(CCellRef(c.X    , c.Y    ))->GetOffsetVelocity();
	vel += GetCell(CCellRef(c.X + 1, c.Y    ))->GetOffsetVelocity();
	vel += GetCell(CCellRef(c.X    , c.Y + 1))->GetOffsetVelocity();
	vel += GetCell(CCellRef(c.X + 1, c.Y    ))->GetOffsetVelocity();

	return vel >> 2;
}

//******************************************************************************************
//**  Clip submitted position

#define MAP_BOUNDARY G(2)

void	CMap::ClipPosition(GVector &p)
{
	if (p.X < MAP_BOUNDARY) p.X = MAP_BOUNDARY;
	if (p.Y < MAP_BOUNDARY) p.Y = MAP_BOUNDARY;

	if (p.X > G(MAP_WIDTH ) - MAP_BOUNDARY) p.X = G(MAP_WIDTH ) - MAP_BOUNDARY;
	if (p.Y > G(MAP_HEIGHT) - MAP_BOUNDARY) p.Y = G(MAP_HEIGHT) - MAP_BOUNDARY;
}

//******************************************************************************************
GVector CMap::FindLineIntersection(GVector start, GVector end)
{
	// OK - rev one - step through the map by moving one map square's length at a time.

#define INT_ACCURACY 4

	GVector dir = (end - start) / G(10); // probably too big...

	GINT	dmag = dir.MagnitudeXY();

	if (dmag < G(0, 1000))
		return start;	// close enough

	dir = dir / dmag / G(INT_ACCURACY);

	GINT	z_for_xy = dir.Z / GROOT(dir.X * dir.X + dir.Y * dir.Y);

	SINT c0;
	GINT	x = start.X;
	GINT	y = start.Y;


	for (c0 = 0; c0 < (MAP_WIDTH + MAP_HEIGHT) * INT_ACCURACY; c0++)
	{
		x += dir.X;
		y += dir.Y;

//		if ((x < MAP_BOUNDARY) || (y < MAP_BOUNDARY) || (x > G(MAP_WIDTH) - MAP_BOUNDARY) || (y > G(MAP_HEIGHT) - MAP_BOUNDARY))
		if ((x < G0) || (y < G0) || (x >= G(MAP_WIDTH)) || (y >= G(MAP_HEIGHT)))
			continue; // not on the map yet...

		// ok - we got a cell. let's test it.
		// at what height does out line hit this cell?
		GINT h = start.Z + (z_for_xy * G(c0) / G(INT_ACCURACY));

		if (h > GetMapHeight(GVector(x, y, G0)))
		{
			// this'll do for now...

			return GVector(x, y, G0);
		}
	}


	return GVector(G(-1), G0, G0);
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

// ** Map Effects

void	CMap::Raindrop(const CCellRef &c,  GINT power)
{
	//TRACE("Raindrop!\n");
#if TARGET == PSX
	if (GAME.IsMultiPlayer())
		return;
#endif

	if (COORDS_INVALID(c))
		return;
	SINT pow=power.GetVal()>>(16-HEIGHT_SHIFT);

	GetCell(c                   )->mOffsetVelocity -= ((DROP_SIZE_SINT  * pow)>>(HEIGHT_SHIFT));

	SINT mw = mMapWidth-1;
	SINT mh = mMapHeight-1 ;
	SINT x =c.X;
	SINT y =c.Y;
	if (x < mw )			GetCell(c + CCellRef( 1,  0))->mOffsetVelocity -= ((DROP_SIZE2_SINT * pow)>>(HEIGHT_SHIFT));
	if (x > 0 )				GetCell(c + CCellRef(-1,  0))->mOffsetVelocity -= ((DROP_SIZE2_SINT * pow)>>(HEIGHT_SHIFT));
	if (x < mw && y < mh)	GetCell(c + CCellRef( 1,  1))->mOffsetVelocity -= ((DROP_SIZE2_SINT * pow)>>(HEIGHT_SHIFT));
	if (y < mh)				GetCell(c + CCellRef( 0,  1))->mOffsetVelocity -= ((DROP_SIZE2_SINT * pow)>>(HEIGHT_SHIFT));
	if (x > 0 && y < mh)	GetCell(c + CCellRef(-1,  1))->mOffsetVelocity -= ((DROP_SIZE2_SINT * pow)>>(HEIGHT_SHIFT));
	if (x < mw && y > 0)	GetCell(c + CCellRef( 1, -1))->mOffsetVelocity -= ((DROP_SIZE2_SINT * pow)>>(HEIGHT_SHIFT));
	if (y > 0)				GetCell(c + CCellRef( 0, -1))->mOffsetVelocity -= ((DROP_SIZE2_SINT * pow)>>(HEIGHT_SHIFT));
	if (x > 0 && y > 0)		GetCell(c + CCellRef(-1, -1))->mOffsetVelocity -= ((DROP_SIZE2_SINT * pow)>>(HEIGHT_SHIFT));
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CMap::SetCellTNo(CCellRef &c, SINT n)
{
#if TARGET == PC
	// set group of cells.
	SINT	sx = (c.X / 4) * 4;
	SINT	sy = (c.Y / 4) * 4;

	SINT	c0, c1;

	for (c0 = 0; c0 < 4; c0 ++)
		for (c1 = 0; c1 < 4; c1 ++)
			GetCell(CCellRef(sx + c0, sy + c1))->mTextureNo = n;
#endif
}


//******************************************************************************************
//******************************************************************************************
//** Shadow Stuff

void	CMap::AddShadowCuboid(GVector &pos, GVector &axes, GMatrix &ori)
{
#if 0
	// very simple for now...
	GINT	maxx = GINT_MIN, maxy = GINT_MIN;
	GINT	minx = GINT_MAX, miny = GINT_MAX;

	GVector r;

	//! er... think about this you fuckwit...  still, looks cool for the windmill
	r = pos + ori * GVector( axes.X, G0, G0);
	if (r.X > maxx) maxx = r.X;
	if (r.Y > maxy) maxy = r.Y;
	if (r.X < minx) minx = r.X;
	if (r.Y < miny) miny = r.Y;
	r = pos + ori * GVector(-axes.X, G0, G0);
	if (r.X > maxx) maxx = r.X;
	if (r.Y > maxy) maxy = r.Y;
	if (r.X < minx) minx = r.X;
	if (r.Y < miny) miny = r.Y;
	r = pos + ori * GVector(G0,  axes.Y, G0);
	if (r.X > maxx) maxx = r.X;
	if (r.Y > maxy) maxy = r.Y;
	if (r.X < minx) minx = r.X;
	if (r.Y < miny) miny = r.Y;
	r = pos + ori * GVector(G0, -axes.Y, G0);
	if (r.X > maxx) maxx = r.X;
	if (r.Y > maxy) maxy = r.Y;
	if (r.X < minx) minx = r.X;
	if (r.Y < miny) miny = r.Y;
	r = pos + ori * GVector(G0, G0,  axes.Z);
	if (r.X > maxx) maxx = r.X;
	if (r.Y > maxy) maxy = r.Y;
	if (r.X < minx) minx = r.X;
	if (r.Y < miny) miny = r.Y;
	r = pos + ori * GVector(G0, G0, -axes.Z);
	if (r.X > maxx) maxx = r.X;
	if (r.Y > maxy) maxy = r.Y;
	if (r.X < minx) minx = r.X;
	if (r.Y < miny) miny = r.Y;

	SINT c0, c1;
	SINT l = minx.Whole() + 1;
	SINT m = maxx.Whole() + 1;
	SINT n = miny.Whole() + 1;
	SINT o = maxy.Whole() + 1;

	if (l < 1) l = 1;
	if (m < 1) m = 1;
	if (n < 1) n = 1;
	if (o < 1) o = 1;
	if (l > MAP_WIDTH  - 2) l = MAP_WIDTH  - 2;
	if (m > MAP_WIDTH  - 2) m = MAP_WIDTH  - 2;
	if (n > MAP_HEIGHT - 2) n = MAP_HEIGHT - 2;
	if (o > MAP_HEIGHT - 2) o = MAP_HEIGHT - 2;

	for (c0 = l; c0 < m; c0++)
	{
		for (c1 = n; c1 < o; c1++)
		{
			GetCell(CCellRef(c0, c1))->SetShadow(128);
		}
	}

	// do edges
	SINT f1, f2, f3, f4;

	f1 = 128 + (miny.Frac() / 512);
	for (c0 = l; c0 < m; c0++)
		GetCell(CCellRef(c0, n - 1 ))->SetShadow(f1);

	f2 = 255 - (maxy.Frac() / 512);
	for (c0 = l; c0 < m; c0++)
		GetCell(CCellRef(c0, o))->SetShadow(f2);

	f3 = 128 + (minx.Frac() / 512);
	for (c0 = n; c0 < o; c0++)
		GetCell(CCellRef(l - 1, c0))->SetShadow(f3);

	f4 = 255 - (maxx.Frac() / 512);
	for (c0 = n; c0 < o; c0++)
		GetCell(CCellRef(m, c0))->SetShadow(f4);

	// and the corners
	SINT f;
	f = 255 - (((255 - f1) * (255 - f3)) / 128);
	GetCell(CCellRef(l - 1, n - 1))->SetShadow(f);

	f = 255 - (((255 - f2) * (255 - f3)) / 128);
	GetCell(CCellRef(l - 1, o))->SetShadow(f);

	f = 255 - (((255 - f1) * (255 - f4)) / 128);
	GetCell(CCellRef(m, n - 1))->SetShadow(f);

	f = 255 - (((255 - f2) * (255 - f4)) / 128);
	GetCell(CCellRef(m, o))->SetShadow(f);

#endif
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

// ** Mapwho stuff.

//***************************************************************************************
SINT	CMap::ChooseLayer(GINT radius)
{
	// select mapwho layer to insert thing into.
	// choose such that radius < size/2;
	//! slightly shit code here - but will be reasonably quick if the number of layers is small (<=5)

	SINT	r=(radius * G(1, 65500)).Whole() + 1; // just a little smaller than it should be
	SINT	c0;

	for (c0=MAP_LAYER_MIN; c0<MAP_LAYER_MAX; c0++)
		if (r < (1 << c0))
			return c0;

	return c0;	// largest layer if too big.
}

//***************************************************************************************
void	CMap::AddToMapWhoLayer(SINT layer, CThing* t)
{
	CCellRef	c = CCellRef(t->mPos);
	c.Clip();

	CThing **m = GetMapWho(layer, c);

	t->mMapWhoNext = *m;
	*m = t;

	t->mFlags |= TF_IN_MAPWHO;
}

//***************************************************************************************
void	CMap::AddToMapWho(CThing *t)
{
	ASSERT(t);
//	ASSERT(!(t->mFlags & TF_IN_MAPWHO));

	AddToMapWhoLayer(ChooseLayer(t->RadiusForMapWho()), t);
}

//***************************************************************************************
BOOL	CMap::RemoveFromMapWhoLayer(SINT layer, CThing *t)
{
	CCellRef	c = CCellRef(t->mPos);
	c.Clip();

	CThing **m = GetMapWho(layer, c);

	if (*m == t)
	{
		// 1st one - special case
		*m = t->mMapWhoNext;
		t->mMapWhoNext = NULL;
		t->mFlags &= ~TF_IN_MAPWHO;
		return TRUE;
	}

	CThing	*ptr = *m;
	CThing	*optr;

	while (ptr)
	{
		optr = ptr;
		ptr = ptr->mMapWhoNext;

		if (ptr == t)
		{
			optr->mMapWhoNext = t->mMapWhoNext;
			t->mMapWhoNext = NULL;
			t->mFlags &= ~TF_IN_MAPWHO;
			return TRUE;
		}
	}

	return FALSE; // not necessarily an error

}

//***************************************************************************************
// two functions provided here so we don't have to call the virtual Radius() function
// if this is being called in part of the thing's destructor.

void	CMap::RemoveFromMapWhoSizeUnknown(CThing *t)
{
	ASSERT(t);
	ASSERT(t->mFlags & TF_IN_MAPWHO);

	// look for it!
	SINT	c0;
	for (c0 = MAP_LAYER_MIN; c0 <= MAP_LAYER_MAX; c0 ++)
	{
		if (RemoveFromMapWhoLayer(c0, t))
		{
			// found it!
			return;
		}
	}
	// Bugger...

	//ASSERT(0);
}
//***************************************************************************************
void	CMap::RemoveFromMapWho(CThing *t)
{
	ASSERT(t);
	ASSERT(t->mFlags & TF_IN_MAPWHO);

	SINT	layer=ChooseLayer(t->RadiusForMapWho());
	if (!(RemoveFromMapWhoLayer(layer, t)))
	{
		// can't find it - look elsewhere...
		RemoveFromMapWhoSizeUnknown(t);
	}
}

//***************************************************************************************
void	CMap::MoveMapWho(CThing *t, GVector dest)
{
	ASSERT(t);

	RemoveFromMapWho(t);
	t->mPos = dest;
	AddToMapWho(t);
}

/*//******************************************************************************************
CThing *CMap::GetFirstThing(CCellRef &c)
{
	// use mapwhos now

	if (COORDS_INVALID(c))
		return NULL;

	return *GetMapWho(MAP_LAYER_MIN, c);
}
*/

//***************************************************************************************
CThing	*CMap::GetFirstThing(const GVector &pos, GINT radius)
{
	// setup map scanning parameters

	mGTPosition	= pos;
	mGTRadius	= radius;

	mGTCurrentLayer = MAP_LAYER_MIN;

	GINT	size = G(1 << mGTCurrentLayer);
	GINT	rad=mGTRadius;
	if (rad < size / 2) rad = size / 2;  // Must search at least 4 cells.

	mXStart=((mGTPosition.X - rad) / size).Whole();
	mYStart=((mGTPosition.Y - rad) / size).Whole();
	mXEnd  =((mGTPosition.X + rad) / size).Whole();
	mYEnd  =((mGTPosition.Y + rad) / size).Whole();

	SINT	maxsize_x = MAP_LAYER_SIZE_X(mGTCurrentLayer) - 1;
	SINT	maxsize_y = MAP_LAYER_SIZE_Y(mGTCurrentLayer) - 1;
	if (mXStart < 0) mXStart = 0;
	if (mYStart < 0) mYStart = 0;
	if (mXEnd   < 0) mXEnd   = 0;
	if (mYEnd   < 0) mYEnd   = 0;
	if (mXStart > maxsize_x) mXStart = maxsize_x - 1;
	if (mYStart > maxsize_y) mYStart = maxsize_y - 1;
	if (mXEnd   > maxsize_x) mXEnd   = maxsize_x - 1;
	if (mYEnd   > maxsize_y) mYEnd   = maxsize_y - 1;

	mCX = mXStart;
	mCY = mYStart;

	mGTCurrentThing = *GetMapWho(mGTCurrentLayer, mCX, mCY);

	return GetNextThing();
}

//***************************************************************************************
CThing	*CMap::GetNextThing()
{
	while (TRUE) // repeat until we find a thing
	{
		// First, if we are currently in a mapwho cell, return the next thing
		if (mGTCurrentThing)
		{
			CThing *t = mGTCurrentThing;
			mGTCurrentThing = mGTCurrentThing->mMapWhoNext;
			return t;
		}

		// next, look for the next x cell
		if (mCX < mXEnd)
		{
			mCX ++;
			mGTCurrentThing = *GetMapWho(mGTCurrentLayer, mCX, mCY);
			continue;
		}
		// ok, the next y...
		if (mCY < mYEnd)
		{
			mCY ++;
			mCX = mXStart;
			mGTCurrentThing = *GetMapWho(mGTCurrentLayer, mCX, mCY);
			continue;
		}
		// ok, the next layer of mapwho cells.
		if (mGTCurrentLayer < MAP_LAYER_MAX)
		{
			mGTCurrentLayer ++;

			GINT	size = G(1 << mGTCurrentLayer);
			GINT	rad=mGTRadius;
			if (rad < size / 2) rad = size / 2;  // Must search at least 4 cells.

			mXStart=((mGTPosition.X - rad) / size).Whole();
			mYStart=((mGTPosition.Y - rad) / size).Whole();
			mXEnd  =((mGTPosition.X + rad) / size).Whole();
			mYEnd  =((mGTPosition.Y + rad) / size).Whole();

			SINT	maxsize_x = MAP_LAYER_SIZE_X(mGTCurrentLayer) - 1;
			SINT	maxsize_y = MAP_LAYER_SIZE_Y(mGTCurrentLayer) - 1;
			if (mXStart < 0) mXStart = 0;
			if (mYStart < 0) mYStart = 0;
			if (mXEnd   < 0) mXEnd   = 0;
			if (mYEnd   < 0) mYEnd   = 0;
			if (mXStart > maxsize_x) mXStart = maxsize_x - 1;
			if (mYStart > maxsize_y) mYStart = maxsize_y - 1;
			if (mXEnd   > maxsize_x) mXEnd   = maxsize_x - 1;
			if (mYEnd   > maxsize_y) mYEnd   = maxsize_y - 1;

			mCX = mXStart;
			mCY = mYStart;

			mGTCurrentThing = *GetMapWho(mGTCurrentLayer, mCX, mCY);

			continue;
		}
		// all done
		return NULL;
	}
}
