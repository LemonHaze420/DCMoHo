// $Header$

// $Log$
// Revision 1.20  2000-10-18 15:43:46+01  are
// Water particles should finally be correct.
//
// Revision 1.19  2000-10-18 13:50:51+01  asd
// added random number code, it now builds a buffer of 250 for each level for random numbers.. should speed up water code..
//
// Revision 1.18  2000-10-18 11:51:35+01  asd
// no water particle cals are done if there si no water
//
// Revision 1.17  2000-10-17 16:24:12+01  are
// Animated pads working.
//
// Revision 1.16  2000-10-17 14:53:10+01  are
// UVs on map tile animations correct.
//
// Revision 1.15  2000-10-17 12:25:28+01  are
// New shadow code.
//
// Revision 1.14  2000-10-17 11:36:52+01  are
// Animated pads half way there.
//
// Revision 1.13  2000-10-17 09:51:47+01  are
// Middle of the way through putting in RenderCellType methods.
//
// Revision 1.12  2000-10-16 16:24:39+01  are
// Using z-bias to fix z problems.
//
// Revision 1.11  2000-10-16 15:42:00+01  are
// Correctly process map particles.
//
// Revision 1.10  2000-10-14 17:21:32+01  are
// Rendering some map particles. Greater z offset to reduce artifacting.
//
// Revision 1.9  2000-10-12 17:32:15+01  are
// Corrected decal problem caused by 4x4, 2x2 and 1x1 overlap.
//
// Revision 1.8  2000-10-12 16:32:54+01  are
// Offset to 'pads' - removes decal problem.
//
// Revision 1.7  2000-10-12 11:18:56+01  are
// Draws 'pads'. - although there is a decal issue.
//
// Revision 1.6  2000-10-11 16:18:29+01  are
// Fixed oversight in drawing 3x3.
//
// Revision 1.5  2000-10-11 15:57:18+01  are
// 4x4, 2x2, 1x1, 4x3, 3x4, 3x3, methods implemented. Not entirely sure they do what they are supposed to, but does look very similar to DC version.
//
// Revision 1.4  2000-10-10 14:41:09+01  asd
// added addwaterParticles function. not yet fully working..
//
// Revision 1.3  2000-09-19 10:51:07+01  jjs
// Added Texture Alpha for Shadow polys.
//

#include	"Common.h"

#if TARGET == PC
#include	"PCMap.h"
#include	"Globals.h"
#include	"Profile.h"
#include	"Gamut.h"
#include	"gcgamut.h"
#include	"RTParticleSystem.h"
#include	"Editors.h"

struct	Vertemp	// only used here for temporary vertex storage
{
	FVector	mPos;
	SINT	mShade;
	SINT	mMix[3];
	BOOL	CanIMove;
	DWORD	mLightColour; // JCL
};
//******************************************************************************************
BOOL	CPCMap::Resize(SINT xs, SINT ys)
{
	//!PSX this stuff should be platform specific.
	//!PSX - DEFINITLY!

	// clear old mapwhos
	SINT	c0;
	for (c0 = MAP_LAYER_MIN; c0 <= MAP_LAYER_MAX; c0 ++)
	{
		delete [] mMapWhos[c0 - MAP_LAYER_MIN];
		mMapWhos[c0-MAP_LAYER_MIN] = NULL;
	}

	delete [] mCells;
	mCells = NULL;

	// create new ones

	// default values.
	mMapWidthShift = xs;
	mMapHeightShift = ys;
	mMapWidth  = 1 << mMapWidthShift;
	mMapHeight = 1 << mMapHeightShift;

	ASSERT(!((mMapWidth&3)||(mMapHeight&3))) // check if map is divisible by four
	// initialise the heightfield
	mCells = new CMapCell[mMapWidth * mMapHeight + 1];  //! +1 so I can do (*this +1)..
	if (!mCells) return FALSE;

	delete [] m4x4s;
	m4x4s = NULL;
	m4x4s = new C4x4[(mMapWidth>>2)*(mMapHeight>>2)];
	if (!m4x4s) return FALSE;

	// Allocate memory for MapWhos
	for (c0 = MAP_LAYER_MIN; c0 <= MAP_LAYER_MAX; c0++)
	{
		// Cells
		SINT size_x = MAP_WIDTH / (1 << c0);
		SINT size_y = MAP_HEIGHT / (1 << c0);
		mMapWhos[c0 - MAP_LAYER_MIN]=new (CThing (*([size_x * size_y])));
		ASSERT(mMapWhos[c0-MAP_LAYER_MIN]);
	}

	MAP.InitPlatform();

	Reset(); //! Overkill??

	return TRUE;
}

// ----------------------------------------------------------------------------
void		CPCMap::ProcessParticles()
{
	return;

	// Add water particles.
	for (int a = 0; a < (mMapHeight >> 2) - 1; a++)
	{
		for (int b = 0; b < (mMapWidth >> 2) - 1; b++)
		{
			FVector l1 = GVectorToFVector(ENGINE.GetCamera()->mLookAt);
			FVector l2 = FVector(float(b) * 4.f, float(a) * 4.f, 0);
			
			float dist = (l1 - l2).MagnitudeSq();

			if (dist < 400.0f)
				AddWaterParticles(b << 2, a << 2);
		}
	}
}



//======================================================-==--=-- --  -
void	CPCMap::Create44Masks(const CCellRef &start, const CCellRef &end)
{
	int		ox, oy, xc, yc;
	int		startx, starty, endx, endy;
	CMapCell* cell;

	startx = (start.X-5)&0xfffffffc;
	if(startx<0) startx=0;
	starty = (start.Y-5)&0xfffffffc;
	if(starty<0) starty=0;
	endx = (end.X+8)&0xfffffffc;
	if(endx>MAP_WIDTH) endx=MAP_WIDTH;
	endy = (end.Y+8)&0xfffffffc;
	if(endy>MAP_HEIGHT) endy=MAP_HEIGHT;

	for(oy=starty; oy<endy; oy+=4)
	{
		for(ox=startx; ox<endx; ox+=4)
		{
			int	ycm, xcm;
			SWORD	max = -32768;
			SWORD	min = 32767;
			C4x4* p4x4 = &m4x4s[((oy*MAP_WIDTH)>>4)+(ox>>2)];

			ycm = (oy<(MAP_HEIGHT-4))+3; // cludge for bottom edge to make it only 3
			xcm = (ox<(MAP_WIDTH-4))+3; // cludge for right edge to make it only 3

			UWORD	mask0 = 0;
			UWORD	mask1 = 0;
			UWORD	mask2 = 0;
			UWORD	or_me = 1;
			for(yc=0; yc<ycm; yc++)
			{
				for(xc=0; xc<xcm; or_me <<=1, xc++)
				{
					int	p0, p1, p2, t, tp0, tp1;
					cell = &mCells[(oy+yc)*MAP_WIDTH+ox+xc];

					SWORD a = cell->mHeight;
					if(a<min) min=a;
					if(a>max) max=a;

					t = cell[0].mMix;
					tp0 = t & 0xf;
					tp1 = (t>>4) & 0xf;
					p0 = tp0;
					p1 = tp1;
					p2 = 15 - tp0 - tp1;

					t = cell[1].mMix;
					tp0 = t & 0xf;
					tp1 = (t>>4) & 0xf;
					p0 += tp0;
					p1 += tp1;
					p2 += 15 - tp0 - tp1;

					t = cell[MAP_WIDTH].mMix;
					tp0 = t & 0xf;
					tp1 = (t>>4) & 0xf;
					p0 += tp0;
					p1 += tp1;
					p2 += 15 - tp0 - tp1;

					t = cell[MAP_WIDTH+1].mMix;
					tp0 = t & 0xf;
					tp1 = (t>>4) & 0xf;
					p0 += tp0;
					p1 += tp1;
					p2 += 15 - tp0 - tp1;


					if(p0) mask0 |= or_me;
					if(p1) mask1 |= or_me;
					if(p2) mask2 |= or_me;
				}
				if(ycm==3)
				{
					or_me <<=1;			// ensure the mask is the same for narrow right/top blocks
				}
			}
			p4x4->SetTextureMask(0,mask0);
			p4x4->SetTextureMask(1,mask1);
			p4x4->SetTextureMask(2,mask2);
			UBYTE	addafter = 0;
			if(!mask0)
			{
				p4x4->SetTexture(0,0);
				addafter = 1;
			}
			if(!mask1)
			{
				p4x4->SetTexture(1,0);
				if(addafter !=0) addafter = 2;
			}
			if(!mask2) p4x4->SetTexture(2,0);
			p4x4->SetAditiveAfter(addafter);
			p4x4->mMin = (((float)min)/8192.0f)-1;
			p4x4->mMax = (((float)max)/8192.0f)+1;
		}
	}
}
//======================================================-==--=-- --  -
void	CPCMap::InitPlatform(void)
{
}
//======================================================-==--=-- --  -
void	CPCMap::ShutdownPlatform(void)
{
	if(m4x4s)
		delete [] m4x4s;
}

//******************************************************************************************
void	CPCMap::SetupDefaultTexture()
{
	strcpy(mMapTextureNames[0], "unknown.tga");
//	strcpy(mMapTextureNames[0], "land1.tga");
	mMapTextures[0] = CTextureGTEX::GetTexture(mMapTextureNames[0]);
}

//******************************************************************************************
// **  MAP RENDERING
BOOL	CPCMap::Init()
{
	// init top level stuff
	if (!(InitCommon()))
		return FALSE;

	SINT	c0;

	// Type Textures
	mTypeTextures[CT_ICE]				= CTextureGTEX::GetTexture("IceRock.tga");
	mTypeTextures[CT_TRIGGER_GUN]		= CTextureGTEX::GetTexture("tiles\\switguna.tga");
	mTypeTextures[CT_BOOSTR]			= CTextureGTEX::GetTexture("BoostR.tga");
	mTypeTextures[CT_BOOSTL]			= CTextureGTEX::GetTexture("BoostL.tga");
	mTypeTextures[CT_BOOSTU]			= CTextureGTEX::GetTexture("BoostU.tga");
	mTypeTextures[CT_BOOSTD]			= CTextureGTEX::GetTexture("BoostD.tga");
	mTypeTextures[CT_TRIGGER_GUN_ON]	= CTextureGTEX::GetTexture("tiles\\switgun1.tga");
	mTypeTextures[CT_TRIGGER_BAD_GUN]	= CTextureGTEX::GetTexture("tiles\\switgun.tga");
	mTypeTextures[CT_TRIGGER_WAVE]		= CTextureGTEX::GetTexture("tiles\\switwav.tga");
	mTypeTextures[CT_TRIGGER_WAVE_ON]	= CTextureGTEX::GetTexture("tiles\\switwav1.tga");
	mTypeTextures[CT_HEALTH]			= CTextureGTEX::GetTexture("tiles\\healthso.tga");
	mTypeTextures[CT_HEALTH_ON]			= CTextureGTEX::GetTexture("tiles\\healths.tga");


	for (c0 = 1; c0 < NUM_CELL_TYPES; c0 ++)
	{
		ASSERT(mTypeTextures[c0]);
	}

	mTexPickupBeam1 = CTextureGTEXAdditive::GetTexture("Beam1.tga");
	mTexPickupBeam2 = CTextureGTEXAdditive::GetTexture("Beam2.tga");
	mTexLaserBeam   = CTextureGTEXAdditive::GetTexture("Laser.tga");
	ASSERT(mTexPickupBeam1);
	ASSERT(mTexPickupBeam2);
	ASSERT(mTexLaserBeam);

	// map texture stuff
	for (c0 = 0; c0 < NUM_CELL_TYPES; c0++)
		mMapTextures[c0] = NULL;

	// default texture
	SetupDefaultTexture();
	ASSERT(mMapTextures[0]);
// initialise vertex buffers

	CreateMixTable();
	CreateEffectTable();

	// New code to fill up random number buffer
	for(int i = 0; i < 250; i++)
	{
		mRandomNumbers[i] = rand();
	}

	RandPos = 0;

	return TRUE;
}

//======================================================-==--=-- --  -
void	CPCMap::CreateEffectTable(void)
{
	int	m, shade;
	float d;
	for(d = 0.0, shade=0; shade<EFFECT_SHADES; d+=(PI*2.0f/(float)EFFECT_SHADES), shade++)
	{
		SINT	r = 230+(int)(25.0f*sin(d));
		SINT	g = 230+(int)(25.0f*sin(d+(PI/3.0f)));
		SINT	b = 230+(int)(25.0f*sin(d-(PI/3.0f)));
		for(int c = 0; c<16; c++)
		{
			m = c *0x11;
		// should go 0,11,22,33,44,55,66,77,88,99,aa,bb,cc,dd,ee,ff in hex
			mEffects[shade][c] =  ((((r*m)/256)<<16)+
									(((g*m)/256)<<8)+
									(((b*m)/256)));
		}
	}

}
//======================================================-==--=-- --  -
void	CPCMap::CreateMixTable(void)
{
	int	m, shade;
	float d;
	for(d = 1.0, shade=0; shade<MIX_SHADES; d-= 2.0/MIX_SHADES, shade++)
	{

// taken from EvaluateColour in map.cpp

		SINT g = (d < 0) ? 170 - SINT((-d) * 130.f) : 170 - SINT(d * 0.f);
		SINT r = (d < 0) ? 170 - SINT((-d) * 130.f) : 170 + SINT(d * 80.f);
		SINT b = (d < 0) ? 170                      : 170 - SINT(d * 40.f) ;

		for(int c = 0; c<16; c++)
		{
			m = c *0x11;
		// should go 0,11,22,33,44,55,66,77,88,99,aa,bb,cc,dd,ee,ff in hex
			mMixers[shade][c] =  ((((r*m)/256)<<16)+
									(((g*m)/256)<<8)+
									(((b*m)/256)));
		}
	}
}
//======================================================-==--=-- --  -
SINT	CPCMap::GetMixerShade(GINT a, GINT b)
{
	SINT ret = ((a-b)*(MIX_SHADES/2)).Whole()+(MIX_SHADES/2);
	if(ret<0) ret = 0;
	else if(ret>=MIX_SHADES) ret=MIX_SHADES-1;
	return ret;
}
//******************************************************************************************
void	CPCMap::DrawMapWhoDebug()
{
	// scan through mapwhos, flagging which mapwhos are used
	FMatrix ori = ID_FMATRIX;
	FVector ps  = FVector(0, 0, 0);
	ENGINE.SetupTransforms(ps, ori);

	SINT layer, x, y;

	for (layer = MAP_LAYER_MIN; layer <= MAP_LAYER_MAX; layer++)
	{
		SINT size_x = MAP_WIDTH  / (1 << layer);
		SINT size_y = MAP_HEIGHT / (1 << layer);

		for (y = 0; y < size_y; y ++)
			for (x = 0; x < size_x; x ++)
			{
				// is there something here?
				if (*(GetMapWho(layer, x, y)))
				{
					// yup - draw a debug cuboid
					float	l = float(1 << layer);
					FVector pos  = FVector(float(x) + 0.5f, float(y) + 0.5f, 0) * l;
					FVector axes = FVector(l * 0.5f, l * 0.5f, 10.f);

					DWORD	col; // pick a colour!
					switch(layer)
					{
					case 1:  col = 0xff101080; break;
					case 2:  col = 0xffa05030; break;
					case 3:  col = 0xff20f080; break;
					case 4:  col = 0xff602040; break;
					default: col = 0xffffffff; break;
					};

					ENGINE.DrawDebugCuboid(col, axes, pos);
				}
			}
	}
}

//******************************************************************************************
void	CPCMap::DrawPickupBeam(CPlayer *pl)
{
	PROFILE_FN(RMDrawPickupBeam);

	if (pl->mPickupBeamState == NO_BEAM)
		return;

	// draw central cylinder.
//#define	BEAM_LONG_VERTS			10
#define	BEAM_LONG_VERTS			30
#define BEAM_ROTATE_SPEED		0.1f
#define	BEAM_RADIUS				0.4f
#define BEAM_LENGTH				20.f
#define	BEAM_GROUND_PENETRATION	3.f

	ASSERT(pl->mPickup.X != -1);

	FMatrix ori;
	ori.MakeRotationYawF(float(ENGINE.GetRenderGameTurn() - pl->mPickupBeamStartTime) * BEAM_ROTATE_SPEED);

	FVector pos;

	//! Hmmmm
	GVector ac = pl->mAnalogCursor;
	ClipPosition(ac);

	switch (CONTROLS.GetControllerMode())
	{
	case CM_KEY_ALT1:
	case CM_JOY_ALT1:
	case CM_JOY_REL:
	case CM_JOY_GOLF:
		pos = FVector(GToF(ac.X), GToF(ac.Y) , GToF(GetCell(CCellRef(ac))->GetUnderlyingHeight()) + BEAM_GROUND_PENETRATION);
		break;
	case CM_JOY_SHUTTLE:
	case CM_JOY_RACER:
	case CM_JOY_DIRECT:
		return; // no beam in shuttle mode.

	default:
		pos = FVector(float(pl->mPickup.X), float(pl->mPickup.Y), GToF(GetCell(pl->mPickup)->GetUnderlyingHeight()) + BEAM_GROUND_PENETRATION);
		break;
	}

	ENGINE.SetupTransforms(pos, ori);

	float	beam_radius = BEAM_RADIUS * GToF(pl->mPickupBeamSize) * 0.1f;
	SINT	intensity = pl->mPickupBeamSize.Whole() * (0xff / 10);
	DWORD	col = RGBA_MAKE(intensity, intensity, intensity, 0xff);

	CFVF_GTEX	vert;
	SINT		i[BEAM_LONG_VERTS * 2];

	SINT	c0, c1, cv = 0;

	for (c0 = 0; c0 < 2; c0 ++)
	{
		for (c1 = 0; c1 < BEAM_LONG_VERTS; c1 ++)
		{
			vert.mPos.X = float(sin((c1 / float(BEAM_LONG_VERTS)) * (2 * PI))) * beam_radius;
			vert.mPos.Y = float(cos((c1 / float(BEAM_LONG_VERTS)) * (2 * PI))) * beam_radius;
			vert.mPos.Z = (float(c0 - 1)) * BEAM_LENGTH;

			vert.mU = (float(c1) / float(BEAM_LONG_VERTS)); // arse... wrapping problems
			vert.mV = float(1 - c0) * 0.9f + 0.05f; // prevent leakage
			vert.mDiffuse = col;

			i[cv] = mTexPickupBeam1->AddVertex(&vert);

			cv ++;
		}
	}

	// and draw the polys
	//! AAAAAAAARRGH

	SINT	ind[6];

	for (c1 = 0; c1 < BEAM_LONG_VERTS; c1++)
	{
		if (c1 != BEAM_LONG_VERTS - 1)
		{
			ind[0] = ((0    ) * BEAM_LONG_VERTS) + c1;
			ind[1] = ((0    ) * BEAM_LONG_VERTS) + c1 + 1;
			ind[2] = ((0 + 1) * BEAM_LONG_VERTS) + c1;

			ind[3] = ((0 + 1) * BEAM_LONG_VERTS) + c1;
			ind[4] = ((0    ) * BEAM_LONG_VERTS) + c1 + 1;
			ind[5] = ((0 + 1) * BEAM_LONG_VERTS) + c1 + 1;
		}
		else
		{
			ind[0] = ((0    ) * BEAM_LONG_VERTS) + c1;
			ind[1] = ((0    ) * BEAM_LONG_VERTS) + c1 - BEAM_LONG_VERTS + 1;
			ind[2] = ((0 + 1) * BEAM_LONG_VERTS) + c1;

			ind[3] = ((0 + 1) * BEAM_LONG_VERTS) + c1;
			ind[4] = ((0    ) * BEAM_LONG_VERTS) + c1 - BEAM_LONG_VERTS + 1;
			ind[5] = ((0 + 1) * BEAM_LONG_VERTS) + c1 - BEAM_LONG_VERTS + 1;
		}
		mTexPickupBeam1->AddTri(i[ind[0]], i[ind[2]], i[ind[1]]);
		mTexPickupBeam1->AddTri(i[ind[3]], i[ind[5]], i[ind[4]]);

		// draw both sides
//		mTexPickupBeam1->AddTri(i[ind[0]], i[ind[1]], i[ind[2]]);
//		mTexPickupBeam1->AddTri(i[ind[3]], i[ind[4]], i[ind[5]]);
	}

//	ENGINE.EnableUWrapping();
//	ENGINE.EnableAdditiveAlpha();

//	mTexPickupBeam1->DrawAll();

//	ENGINE.DisableAlpha();
//	ENGINE.DisableUWrapping();

	// Draw the pulsing circle things..

//#define	RING_SEPARATION	10
//#define	RING_COUNT_MAX	50

#define	RING_SEPARATION	5
#define	RING_COUNT_MAX	80

//#define	RING_SEPARATION	3
//#define	RING_COUNT_MAX	200

#define	RING_BRIGHTNESS_PEAK 10
#define	RING_RADIUS	(BEAM_RADIUS * 2.f)
#define	RING_START_OFFSET (BEAM_GROUND_PENETRATION + 2.2f)

	SINT	count = (ENGINE.GetRenderGameTurn() - pl->mPickupBeamStartTime) % RING_SEPARATION;

	while (count < RING_COUNT_MAX)
	{
		// use same geometry as above.
//		float	ring_height = -(float(count * count) * (BEAM_LENGTH / float(RING_COUNT_MAX * RING_COUNT_MAX)) + BEAM_GROUND_PENETRATION);
		float	ring_height = -(float(count * count * count * count) * (BEAM_LENGTH / float(RING_COUNT_MAX * RING_COUNT_MAX * RING_COUNT_MAX * RING_COUNT_MAX)) + RING_START_OFFSET);
		float	ring_size = (float(RING_COUNT_MAX * RING_COUNT_MAX) - float((RING_COUNT_MAX - count) * (RING_COUNT_MAX - count))) * (RING_RADIUS / float(RING_COUNT_MAX * RING_COUNT_MAX)) * GToF(pl->mPickupBeamSize) * 0.1f;

		SINT	intensity2;

		if (count < RING_BRIGHTNESS_PEAK)
			intensity2 = count * (0xff / RING_BRIGHTNESS_PEAK);
		else
			intensity2 = (RING_COUNT_MAX - count) * (0xff / (RING_COUNT_MAX - RING_BRIGHTNESS_PEAK));

		intensity2 = (intensity * intensity2) / 0xff;
//		intensity = intensity2;

		col = RGB(intensity2, intensity2, intensity2) + 0xff000000;

		vert.mDiffuse = col;

		vert.mPos = FVector(-ring_size, -ring_size, ring_height);
		vert.mU = 0;
		vert.mV = 0;
		i[0] = mTexPickupBeam2->AddVertex(&vert);

		vert.mPos = FVector( ring_size, -ring_size, ring_height);
		vert.mU = 1.f;
		vert.mV = 0;
		i[1] = mTexPickupBeam2->AddVertex(&vert);

		vert.mPos = FVector(-ring_size,  ring_size, ring_height);
		vert.mU = 1.f;
		vert.mV = 0;
		i[2] = mTexPickupBeam2->AddVertex(&vert);

		vert.mPos = FVector( ring_size,  ring_size, ring_height);
		vert.mU = 1.f;
		vert.mV = 1.f;
		i[3] = mTexPickupBeam2->AddVertex(&vert);

		mTexPickupBeam2->AddTri(i[0], i[2], i[3]);
		mTexPickupBeam2->AddTri(i[0], i[3], i[1]);

		count += RING_SEPARATION;
	}

//	ENGINE.EnableAdditiveAlpha();

//	mTexBeam2->DrawAll();

//	ENGINE.DisableAlpha();

}

//******************************************************************************************
void	CPCMap::DrawLaserBeam(CPlayer *pl)
{
#define	LASER_RADIUS				0.05f
#define LASER_LENGTH				20.f
#define	LASER_GROUND_PENETRATION	3.f

	if (pl->mCursor.X == -1)
		return; // not on map.

	// setup transformations
	FMatrix ori = ID_FMATRIX;
	FVector pos;

	GVector ac = pl->mAnalogCursor;
	ClipPosition(ac);

	switch (CONTROLS.GetControllerMode())
	{
	case CM_KEY_ALT1:
	case CM_JOY_ALT1:
	case CM_JOY_REL:
	case CM_JOY_GOLF:
//		pos = FVector(GToF(mAnalogCursor.X) - 0.5f, GToF(mAnalogCursor.Y) - 0.5f, GToF(GetCell(CCellRef(mAnalogCursor))->GetUnderlyingHeight()) + LASER_GROUND_PENETRATION);
		pos = FVector(GToF(ac.X), GToF(ac.Y), GToF(GetCell(CCellRef(ac))->GetUnderlyingHeight()) + LASER_GROUND_PENETRATION);
		break;
	case CM_JOY_DIRECT:
		return; // no beam in shuttle mode

	default:
		pos = FVector(float(pl->mCursor.X), float(pl->mCursor.Y), GToF(GetCell(pl->mCursor)->GetUnderlyingHeight()) + LASER_GROUND_PENETRATION);
		break;
	};

	ENGINE.SetupTransforms(pos, ori);

	float	laser_radius = LASER_RADIUS;
	SINT	intensity = 0x7f;
	DWORD	col = RGBA_MAKE(intensity, 0, 0, 0xff);

	CFVF_GTEX	vert;
	SINT		i[8];

//	lots of beams here!
	float	v1 = float((sin(float(ENGINE.GetRenderGameTurn()) / 80.f))) * 0.4f + 0.5f;
	float	v2 = float((cos(float(ENGINE.GetRenderGameTurn()) / 80.f))) * 0.4f + 0.5f;

	SINT	c2, c0, c1, cv = 0;

	for (c2 = 0; c2 < 2; c2 ++)
	{
		for (c0 = 0; c0 < 2; c0 ++)
		{
			for (c1 = 0; c1 < 2; c1 ++)
			{
				vert.mPos.X = (laser_radius * float(c1)) - (laser_radius / 2.f);
				vert.mPos.Y = 0;
				vert.mPos.Z = (float(c0 - 1)) * LASER_LENGTH;

				vert.mU = float(c0) * 0.9f  + 0.05f;
				vert.mV = float(c1) * 0.02f + ((c2 == 0) ? v1 : v2); // prevent leakage
				vert.mDiffuse = col;

				i[cv] = mTexLaserBeam->AddVertex(&vert);

				cv ++;
			}
		}
	}

	mTexLaserBeam->AddTri(i[0], i[1], i[2]);
	mTexLaserBeam->AddTri(i[2], i[1], i[3]);
	mTexLaserBeam->AddTri(i[4], i[5], i[6]);
	mTexLaserBeam->AddTri(i[6], i[5], i[7]);

#ifdef LINK_EDITOR
	mTexLaserBeam->AddTri(i[0], i[2], i[1]);
	mTexLaserBeam->AddTri(i[2], i[3], i[1]);
	mTexLaserBeam->AddTri(i[4], i[6], i[5]);
	mTexLaserBeam->AddTri(i[6], i[7], i[5]);
#endif

}

//******************************************************************************************
inline	DWORD	get_avg_colour(DWORD c1, DWORD c2)
{
	return ((c1 >> 1) & 0x7f7f7f) + ((c2 >> 1) & 0x7f7f7f);

}

//******************************************************************************************

#define	MAP_TILE_SIZE	4
#define	MAP_TILE_UV		(1.f / float(MAP_TILE_SIZE))

void	CPCMap::Render()
{
	PROFILE_FN(RenderMap);

	// OK - testbed for the D3D Geometry.  Not meant to be too fast.  Yet.
	// setup all the matrices, and let's see what happens...

	FMatrix ori = ID_FMATRIX;
	FVector pos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);

	CFVF_GTEX	vert;

//	WORD	index[] = {4,0,1,2,3,0};
	SINT	index[5];

	SINT c0, c1;

	// evaluate colour for flashing cursor.
	UBYTE	c = ENGINE.GetRenderGameTurn() % 15;
	if (c > 7) c = 15 - c;
	c *= 16;
	c += 127;
	UBYTE d = c / 4;

	// Different coloured cursors
	ULONG	specval = d + (c << 8) + (d << 16) + (0xff << 24);

	switch (CONTROLS.GetControllerMode())
	{
	case CM_KEY_ALT1:
	case CM_JOY_ALT1:
	case CM_JOY_REL:
	case CM_JOY_GOLF:
	case CM_JOY_DIRECT:
		specval = 0;
		break;
	default:
		break;
	};

	SINT	curx = PLAYER(0)->mCursor.X;
	SINT	cury = PLAYER(0)->mCursor.Y;

	if (curx != -1)
	{
		VERIFY_COORDS(PLAYER(0)->mCursor);

		// red cursor for immovable square
		if (!(GetCell(PLAYER(0)->mCursor)->CanIMove()))
		{
			specval = d + (d << 8) +  (c << 16) + (0xff << 24);
		}

		// yellow cursor when something is selectable
		if (PLAYER(0)->FindThingUnderCursor())
		{
			specval = d + (c << 8) +  (c << 16) + (0xff << 24);
		}
	}

	BOOL	tex_drawn[MAX_MAP_TEXTURES + NUM_CELL_TYPES]; // see which textures have been drawn...
	for (c1 = 0; c1 < MAX_MAP_TEXTURES + NUM_CELL_TYPES; c1++)
		tex_drawn[c1] = FALSE;

	CFVF_GTEX	*v1 = new CFVF_GTEX[MAP_WIDTH];
	CFVF_GTEX	*v2 = new CFVF_GTEX[MAP_WIDTH];
	CFVF_GTEX	*ov = v1, *nv = v2, *tv, *cov, *cnv;

	BOOL	use_gamut;
	CGamut *gamut;
	switch (ENGINE.GetGamutType())
	{
	case GAMUT_NORMAL:
	case GAMUT_LOWERED:
		use_gamut = TRUE;
		break;
	default:
		use_gamut = FALSE;
	};

	SINT	start_y, end_y;

	if (use_gamut)
	{
		gamut= ENGINE.GetGamut();
		start_y = gamut->mFirstY;
		end_y = gamut->mLastY;
	}
	else
	{
		start_y = 0;
		end_y = MAP_HEIGHT;
	}

	for (c0 = start_y; c0 < end_y; c0++)
	{
		SINT start_x, end_x;

		if (use_gamut)
		{
			if (c0 != gamut->mLastY - 1)
			{
				// look at the gamut line above, too
				start_x = IMIN(gamut->mGamutLines[c0].Min, gamut->mGamutLines[c0 + 1].Min);
				end_x   = IMAX(gamut->mGamutLines[c0].Max, gamut->mGamutLines[c0 + 1].Max);
				if (end_x < MAP_WIDTH) end_x ++;
			}
			else
			{
				// just use the current line
				start_x = gamut->mGamutLines[c0].Min;
				end_x   = gamut->mGamutLines[c0].Max;
				if (end_x < MAP_WIDTH) end_x ++;
			}
		}
		else
		{
			start_x = 0; end_x = MAP_WIDTH;
		}

		// swap old and new lists
		tv  = ov;	ov  = nv;	nv = tv;
		cov = ov + start_x;	cnv = nv + start_x;

		for (c1 = start_x; c1 < end_x; c1++)
		{
			PROFILE_FN(RMInnerLoop);

			CCellRef	c  = CCellRef(c1, c0);
			CMapCell	*cell = GetCell(c);

			// fill out the geometry.
			cnv->mPos.X		= float(c.X);
			cnv->mPos.Y		= float(c.Y);
			cnv->mPos.Z		= GToF(cell->GetHeight());
//			if (cell->IsSelfIlluminating())
//					cnv->mDiffuse = 0xffffffff;
//			else	cnv->mDiffuse = cell->GetColour();
			cnv->mDiffuse = cell->GetColour();
			cell->mShadow = 255; //! Shouldnt be here!!!!!
			if ((c.X == curx) && (c.Y == cury)) cnv->mSpecular = specval; else cnv->mSpecular = 0;

			// editor highlight region
#ifdef LINK_EDITOR
			if ((EDITOR.IsActive()) && (EDITOR.IsPointInSelectedRegion(c)))
			{
				if (((c.X & 3) == 0) || ((c.Y & 3) == 0))
					cnv->mSpecular = (70 << 8);
				else
					cnv->mSpecular = 70;
			}
			// mark ramps
			if (cell->IsTakeOff())
				cnv->mSpecular += (70 << 16);
			else if ((c0 > 0) && (c1 > 0) && GetCell(CCellRef(c1 - 1, c0 - 1))->IsTakeOff())
				cnv->mSpecular += (35 << 16);

			if (cell->GetRampNo() != 0)
				cnv->mSpecular += (70 << 8) + 70;
			else if ((c0 > 0) && (c1 > 0) && GetCell(CCellRef(c1 - 1, c0 - 1))->GetRampNo() != 0)
				cnv->mSpecular += (35 << 8) + 35;

#endif

			// do we actually want to draw?
			if ((use_gamut &&
			     ((c0 >  gamut->mFirstY) &&
			 	  (c1 >  gamut->mGamutLines[c0].Min) &&
				  (c1 <= gamut->mGamutLines[c0].Max))) ||
				((!use_gamut) &&
				  ((c0 != 0) && (c1 != 0))))
			{
				// looks like it!

				c  = CCellRef(c1 - 1, c0 - 1); // this is the base cell...
				cell = GetCell(c);
				// set up the texture
				CTextureGTEX *ct;
				if (cell->mType == CT_NORMAL)
				{
					// use default texture
					SINT	tn = cell->mTextureNo;
					ct = mMapTextures[tn];
					tex_drawn[tn] = TRUE;
				}
				else
				{
					// use cell type texture
					ct = mTypeTextures[cell->mType];
					tex_drawn[MAX_MAP_TEXTURES + cell->mType] = TRUE;
				}

				ct->SetFrame(GToF(cell->GetFrame()));

				// and the uvs
				float		u = float(c.X % MAP_TILE_SIZE) / float(MAP_TILE_SIZE);
				float		v = float(c.Y % MAP_TILE_SIZE) / float(MAP_TILE_SIZE);

				 cnv     ->mU = u + MAP_TILE_UV;
				 cnv     ->mV = v + MAP_TILE_UV;
				(cnv - 1)->mU = u;
				(cnv - 1)->mV = v + MAP_TILE_UV;
				 cov     ->mU = u + MAP_TILE_UV;
				 cov     ->mV = v;
				(cov - 1)->mU = u;
				(cov - 1)->mV = v;

				// add the first 4 vertices
				index[0] = ct->AddVertex(cov - 1);
				index[1] = ct->AddVertex(cnv - 1);
				index[2] = ct->AddVertex(cnv    );
				index[3] = ct->AddVertex(cov    );

				// sort out the 5th vertex
				FVector		av;
				ULONG		ad1, ad2;


				av  = (cov - 1)->mPos;
				ad1 = (cov - 1)->mDiffuse;

				av += (cnv - 1)->mPos;
				ad1 = get_avg_colour(ad1, (cnv - 1)->mDiffuse);

				av += cnv->mPos;
				ad2 = cnv->mDiffuse;

				av += cov->mPos;
				ad2 = get_avg_colour(ad2, cov->mDiffuse);

				vert.mPos		= av * 0.25f;
				vert.mDiffuse	= get_avg_colour(ad1, ad2);
				vert.mU = cnv->mU - (MAP_TILE_UV / 2.f);
				vert.mV = cnv->mV - (MAP_TILE_UV / 2.f);
				index[4] = ct->AddVertex(&vert);

				// add the triangles!
				ct->AddTri(index[4], index[0], index[1]);
				ct->AddTri(index[4], index[1], index[2]);
				ct->AddTri(index[4], index[2], index[3]);
				ct->AddTri(index[4], index[3], index[0]);

			}
			cov ++;
			cnv ++;
		}

		if (!(c0 & 7))
		{
			PROFILE_FN(RMDraw1);
			// Don't draw TypeTextures here (probably infrequent?)
			for (c1 = 0; c1 < MAX_MAP_TEXTURES; c1 ++)
			{
				if (tex_drawn[c1])
				{
					mMapTextures[c1]->DrawAll();
					tex_drawn[c1] = FALSE;
				}
			}
		}
	}

	{
		PROFILE_FN(RMDraw2);
		for (c1 = 0; c1 < MAX_MAP_TEXTURES + NUM_CELL_TYPES; c1 ++)
		{
			if (tex_drawn[c1])
			{
				if (c1 >= MAX_MAP_TEXTURES)
					mTypeTextures[c1 - MAX_MAP_TEXTURES]->DrawAll();
				else
					mMapTextures[c1]->DrawAll();
				tex_drawn[c1] = FALSE;
			}
		}
	}

	//free up the vertex arrays
	delete [] v2;
	delete [] v1;

	// Draw beam effects if necessary
	CPlayer	*pl = PLAYER(0);
	DrawPickupBeam(pl);
	DrawLaserBeam(pl);

	// Draw MapWho debug information if necessary
	if (GAME.GetGameRenderInfo()->mActive & GRI_MAPWHO_INFO)
		DrawMapWhoDebug();

}

//******************************************************************************************
// draw shadow polygon.. this is hard (ish...)

#define SIGN(a) (((a) >= 0) ? (1.f) : (-1.f))

#define	SHADOW_COL 0x7f000000

void	CPCMap::CDSP(CTextureGTEX *tex, FVector p1, FVector p2, FVector p3, CFVF_GTEX *v1, CFVF_GTEX *v2, CFVF_GTEX *v3)
{
	CFVF_GTEX	b1[20], b2[20];
	CFVF_GTEX	*in = b1, *out = b2;

	b1[0] = *v1;
	b1[1] = *v2;
	b1[2] = *v3;

	// first push all points onto the plane
	SINT c0;

	FVector normal	= (p2 - p1) ^ (p3 - p1);
	FVector h		= normal ^ FVector(0, 0, 1.f); // a "Horizonal" vector
	if (h.MagnitudeSq() < 0.0001)
		h = FVector(1.f, 0, 0); // oops - nearly horizontal anyway...
	FVector s		= h ^ normal;  // this vector points down the slope
	s = s / s.MagnitudeXY();

	for (c0 = 0; c0 < 3; c0 ++)
	{
		FVector s1 = b1[c0].mPos - p1;
		b1[c0].mPos.Z = p1.Z + s.Z * (s.X * s1.X + s.Y * s1.Y) - 0.01f;
		b1[c0].mDiffuse = SHADOW_COL; //! Ahhh...
	}

	CFVF_GTEX	*curr_in, *curr_out, *end = in + 3, *prev;


	for (c0 = 0; c0 < 3; c0++)
	{
		FVector	d;
		FVector pa;

		switch (c0)
		{
		case 0:
			d = p2 - p1;
			pa = p1;
			curr_in = in;
			curr_out = out;
			break;
		case 1:
			d = p3 - p2;
			pa = p2;
			curr_in = out;
			curr_out = in;
			break;
		case 2:
			d = p1 - p3;
			pa = p3;
			curr_in = in;
			curr_out = out;
			break;
		};
		prev = end - 1;

		while(curr_in < end)
		{
			FVector d1 = curr_in->mPos - pa;
			FVector d2 = prev   ->mPos - pa;
			float a = d.X * d1.Y - d.Y * d1.X;
			float b = d.X * d2.Y - d.Y * d2.X;

			if (SIGN(a) != SIGN(b))
			{
				// different sides - need to do something!
				float	r = FABS(a) + FABS(b);
				float	ra = FABS(a) / r;
				float	rb = FABS(b) / r;
				curr_out->mPos		= prev->mPos + ((curr_in->mPos - prev->mPos) * rb);
				curr_out->mU	= (prev->mU * ra) + (curr_in->mU * rb);
				curr_out->mV	= (prev->mV * ra) + (curr_in->mV * rb);
//				curr_out->mDiffuse = 0xffffffff; //! Ahhh...
				curr_out->mDiffuse = SHADOW_COL; //! Ahhh...

//				//work out height value here.
//				float rz = (d2 + ((curr_in->mPos - prev->mPos) * rb)).MagnitudeXY() / d.MagnitudeXY();
//				curr_out->mPos.Z = pa.Z + rz * d.Z;

				curr_out++;
			}

			if (SIGN(a) == 1.f)
				*curr_out ++ = *curr_in;

			prev = curr_in;
			curr_in ++;
		}

		end = curr_out;
	}

	if (end < out + 3)
		// all clipped - go away
		return;

	SINT i[20];

	ASSERT(end < out + 20);

	SINT n = end - out;
	for (c0 = 0; c0 < n; c0++)
		i[c0] = tex->AddVertex(&out[c0]);

	for (c0 = 0; c0 < n - 2; c0++)
		tex->AddTri(i[0], i[c0 + 1], i[c0 + 2]);
}

//******************************************************************************************
void	CPCMap::ClipAndDrawShadowPoly(CTextureGTEX *tex, CFVF_GTEX *v1, CFVF_GTEX *v2, CFVF_GTEX *v3)
{
	// get footprint

	float	minx = 999999.f, maxx = -9999999.f;
	float	miny = 999999.f, maxy = -9999999.f;

	if (v1->mPos.X < minx) minx = v1->mPos.X;
	if (v2->mPos.X < minx) minx = v2->mPos.X;
	if (v3->mPos.X < minx) minx = v3->mPos.X;
	if (v1->mPos.Y < miny) miny = v1->mPos.Y;
	if (v2->mPos.Y < miny) miny = v2->mPos.Y;
	if (v3->mPos.Y < miny) miny = v3->mPos.Y;

	if (v1->mPos.X > maxx) maxx = v1->mPos.X;
	if (v2->mPos.X > maxx) maxx = v2->mPos.X;
	if (v3->mPos.X > maxx) maxx = v3->mPos.X;
	if (v1->mPos.Y > maxy) maxy = v1->mPos.Y;
	if (v2->mPos.Y > maxy) maxy = v2->mPos.Y;
	if (v3->mPos.Y > maxy) maxy = v3->mPos.Y;

	SINT	startx	= SINT(minx);
	SINT	starty	= SINT(miny);
	SINT	endx	= SINT(maxx);
	SINT	endy	= SINT(maxy);

	if (startx < 0) startx = 0;
	if (starty < 0) starty = 0;
	if (endx > MAP_WIDTH  - 2) endx = MAP_WIDTH  - 2;
	if (endy > MAP_HEIGHT - 2) endy = MAP_HEIGHT - 2;

	SINT c0, c1;
	PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	for (c0 = startx; c0 <= endx; c0++)
	{
		for (c1 = starty; c1 <= endy; c1++)
		{
			FVector p[5];

			p[0] = FVector(float(c0    ), float(c1    ), GToF(MAP.GetCell(CCellRef(c0    , c1    ))->GetHeight()));
			p[1] = FVector(float(c0 + 1), float(c1    ), GToF(MAP.GetCell(CCellRef(c0 + 1, c1    ))->GetHeight()));
			p[2] = FVector(float(c0 + 1), float(c1 + 1), GToF(MAP.GetCell(CCellRef(c0 + 1, c1 + 1))->GetHeight()));
			p[3] = FVector(float(c0    ), float(c1 + 1), GToF(MAP.GetCell(CCellRef(c0    , c1 + 1))->GetHeight()));

			p[4] = (p[0] + p[1] + p[2] + p[3]) / 4.0f;

			CDSP(tex, p[4], p[0], p[1], v1, v2, v3);
			CDSP(tex, p[4], p[1], p[2], v1, v2, v3);
			CDSP(tex, p[4], p[2], p[3], v1, v2, v3);
			CDSP(tex, p[4], p[3], p[0], v1, v2, v3);
		}
	}
	tex->DrawAll();
	PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
// ** New shadow code JCL 16-10-00
void	CPCMap::CDSQ(CTextureGTEX *tex, const FVector &p1, const FVector &p2, const FVector &p3,
					 float sx, float sy, float ex, float ey)
{
	#define Z_FUDGE2 (0.003f)
	// take #2
	// map uvs.
	// !! assumes single texture on page
	
	float	u0, u1, u2, v0, v1, v2;
	
	u0 = (p1.X - sx) / (ex - sx);
	u1 = (p2.X - sx) / (ex - sx);
	u2 = (p3.X - sx) / (ex - sx);

	v0 = (p1.Y - sy) / (ey - sy);
	v1 = (p2.Y - sy) / (ey - sy);
	v2 = (p3.Y - sy) / (ey - sy);	

	// render

	SINT		i[3];
	CFVF_GTEX	vert;

	vert.mPos		= p1;
	vert.mU			= u0;
	vert.mV			= v0;
	vert.mDiffuse	= SHADOW_COL;
	i[0] = tex->AddVertex(&vert);

	vert.mPos		= p2;
	vert.mU			= u1;
	vert.mV			= v1;
	vert.mDiffuse	= SHADOW_COL;
	i[1] = tex->AddVertex(&vert);

	vert.mPos		= p3;
	vert.mU			= u2;
	vert.mV			= v2;
	vert.mDiffuse	= SHADOW_COL;
	i[2] = tex->AddVertex(&vert);

	tex->AddTri(i[0], i[1], i[2]);
}

//*****************************************************************************
void	CPCMap::ClipAndDrawShadowQuad(CTextureGTEX *tex, float sx, float sy, float ex, float ey)
{
	SINT	startx	= SINT(sx);
	SINT	starty	= SINT(sy);
	SINT	endx	= SINT(ex);
	SINT	endy	= SINT(ey);

	if (startx < 0) startx = 0;
	if (starty < 0) starty = 0;
	if (endx > MAP_WIDTH  - 2) endx = MAP_WIDTH  - 2;
	if (endy > MAP_HEIGHT - 2) endy = MAP_HEIGHT - 2;

	PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	PLATFORM.GetD3DDevice()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE);

	SINT c0, c1;
	for (c0 = startx; c0 <= endx; c0++)
	{
		for (c1 = starty; c1 <= endy; c1++)
		{
			FVector p[4];

			p[0] = FVector(float(c0    ), float(c1    ), GToF(MAP.GetCell(CCellRef(c0    , c1    ))->GetHeight()));
			p[1] = FVector(float(c0 + 1), float(c1    ), GToF(MAP.GetCell(CCellRef(c0 + 1, c1    ))->GetHeight()));
			p[2] = FVector(float(c0 + 1), float(c1 + 1), GToF(MAP.GetCell(CCellRef(c0 + 1, c1 + 1))->GetHeight()));
			p[3] = FVector(float(c0    ), float(c1 + 1), GToF(MAP.GetCell(CCellRef(c0    , c1 + 1))->GetHeight()));

//			CDSQ(tex, p[0], p[1], p[3], sx, sy, ex, ey);
//			CDSQ(tex, p[3], p[1], p[2], sx, sy, ex, ey);

			CDSQ(tex, p[0], p[3], p[1], sx, sy, ex, ey);
			CDSQ(tex, p[3], p[2], p[1], sx, sy, ex, ey);
		}
	}

	tex->DrawAll();

	PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	PLATFORM.GetD3DDevice()->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
}

//*****************************************************************************
void	CPCMap::DrawShadow(IRenderableThing *t, CTextureGTEX *tex)
{
	if ((t) && (tex))
	{
		FVector	v = ZERO_FVECTOR;
		FMatrix m = ID_FMATRIX;
		ENGINE.SetupTransforms(v, m);

		GVector	pos = t->GetRenderPos();
		if (!(COORDS_INVALID(CCellRef(pos))))
		{
			float dist = - GToF(pos.Z - (MAP.GetMapHeight(pos)));
			if (dist >= -0.5f)
			{
				GINT	gr = t->GetRenderRadius();
				float	r = GToF(gr);
				
				float shadow_scale = 8.f / (dist + 8.f);
				r *= shadow_scale;

				MAP.ClipAndDrawShadowQuad(tex, GToF(pos.X) - r, GToF(pos.Y) - r, GToF(pos.X) + r, GToF(pos.Y) + r);
			}
		}
	}
}
//*****************************************************************************
//*****************************************************************************


//** Map texture stuff
//======================================================-==--=-- --  -
void	CPCMap::TweakVertex(int channel,CCellRef &c,int addthis)
{
	CMapCell*	cell = &mCells[c.Y*MAP_WIDTH+c.X];
	int			ch[3];
	ch[0]= cell->mMix& 0xf;
	ch[1]= (cell->mMix& 0xf0)>>4;
	ch[2]= 15-ch[0]-ch[1];

	ASSERT((ch[0]+ch[1]+ch[2])==15);
	int othera, otherb;
	if(channel==0)
	{
		othera = 1;
		otherb = 2;
	}
	else if(channel==1)
	{
		othera = 2;
		otherb = 0;
	}
	else
	{
		othera = 0;
		otherb = 1;
	}

	int final=ch[channel]+addthis;
	if(final>15) final=15;
	else if(final<0) final=0;

	ch[channel]=final;
	int	sum;
	while((sum=ch[0]+ch[1]+ch[2])!=15)
	{
		if(sum>16)
		{
			if(ch[othera]>0) ch[othera]--;
			if(ch[otherb]>0) ch[otherb]--;
		}
		else if(sum>15)
		{
			if(ch[othera]>0) ch[othera]--;
			else if(ch[otherb]>0) ch[otherb]--;
		}
		else if(sum<14)
		{
			if(ch[othera]<15) ch[othera]++;
			if(ch[otherb]<15) ch[otherb]++;
		}
		else if(sum<15)
		{
			if(ch[othera]<15) ch[othera]++;
			else if(ch[otherb]<15) ch[otherb]++;
		}
	}
	cell->mMix = ch[0]+(ch[1]<<4);
}
//======================================================-==--=-- --  -
void	CPCMap::ClearChannel(int channel, CCellRef &c)
{
	CMapCell*	cell = &mCells[c.Y*MAP_WIDTH+c.X];
	int			ch[3];
	ch[0]= cell->mMix& 0xf;
	ch[1]= (cell->mMix>>4)&0xf;
	ch[2]= 15-ch[0]-ch[1];

	int othera, otherb;
	if(channel==0)
	{
		othera = 1;
		otherb = 2;
	}
	else if(channel==1)
	{
		othera = 2;
		otherb = 0;
	}
	else
	{
		othera = 0;
		otherb = 1;
	}

	ch[channel] = 0;
	if((ch[othera])&&(ch[otherb]))	// both have something in, so dish out spare
	{
		if(ch[othera]>ch[otherb])	// ensure b is greater than a
		{
			int t = othera;	othera = otherb; otherb = t;
		}
		ch[otherb] = (15*ch[otherb])/(ch[othera]+ch[otherb]);
		ch[othera] = 15-ch[otherb];
	}
	else if(ch[othera])	ch[othera]=15;	// only one of these has any in
	else if(ch[otherb])	ch[otherb]=15;	// so make it the 'winner'
	else if(channel==0) ch[1]=15;		// bodge so that channel 0 can be cleared
	else ch[0]=15;

	ASSERT((ch[0]+ch[1])<=15);

	cell->mMix = ch[0]+(ch[1]<<4);
}
 //======================================================-==--=-- --  -
// Can I set the channel(s) associated with this point without screwing anything up?
BOOL	CPCMap::TentativeSetTex(int channel, int tex, const CCellRef &c)
{
	int		firstx, lastx, firsty, lasty;
	int		mod=MAP_WIDTH>>2;

	lastx = c.X>>2;
	if(c.X&3)		firstx = lastx;			// if it's the first verex in a 44
	else			firstx = lastx-1;		// we need do screw around with the previous
	if(firstx<0)	firstx = 0;				// unless we are on the left

	lasty = c.Y>>2;
	if(c.Y&3)		firsty = lasty;			// same shit with the y
	else			firsty = lasty-1;
	if(firsty<0)	firsty = 0;

	int	xc, yc;

//	check that all textures around this point are either ==tex or 0

	for(yc=firsty; yc<=lasty; yc++)
	{
		for(xc=firstx; xc<=lastx; xc++)
		{
			UWORD	t = m4x4s[yc*mod+xc].GetTexture(channel);
			if((t!=0)&&(t!=tex))	return	FALSE;
		}
	}
//	OK, they were, we better set them all to tex;

	for(yc=firsty; yc<=lasty; yc++)
	{
		for(xc=firstx; xc<=lastx; xc++)
		{
			m4x4s[yc*mod+xc].SetTexture(channel,tex);
		}
	}
	return	TRUE;
}
  //======================================================-==--=-- --  -
 // Set the texture but clear this channel's usage first in associated
// 44cell(s)
void CPCMap::BrutalSetTex(int channel, int tex, CCellRef &c)
{
	int		firstx, lastx, firsty, lasty;
	int		mod=MAP_WIDTH>>2;

	lastx = c.X>>2;
	if(c.X&3)		firstx = lastx;			// as function above
	else			firstx = lastx-1;
	if(firstx<0)	firstx = 0;

	lasty = c.Y>>2;
	if(c.Y&3)		firsty = lasty;
	else			firsty = lasty-1;
	if(firsty<0)	firsty = 0;

	int	xc, yc;

	for(yc=firsty; yc<=lasty; yc++)
	{
		for(xc=firstx; xc<=lastx; xc++)
		{
			UWORD	t = m4x4s[yc*mod+xc].GetTexture(channel);
			if(t!=tex)
				Clear44ChannelByXY(channel,xc<<2,yc<<2);
			m4x4s[yc*mod+xc].SetTexture(channel,tex);
		}
	}
}
 //======================================================-==--=-- --  -
// remove channel from 44 cell by map XY
void	CPCMap::Clear44ChannelByXY(int channel, int x, int y)
{
	ASSERT((x<MAP_WIDTH)&&(y<MAP_HEIGHT));
	int xcs = (x!=(MAP_WIDTH-4))+4;					// ensure only 4 verts wide
	int ycs = (y!=(MAP_HEIGHT-4))+4;				// on left and top

	C4x4*	ff = &m4x4s[(y>>2)*(MAP_WIDTH>>2)+(x>>2)];
	ff->SetTexture(channel,0);

	CCellRef	cc;
	for(cc.Y=y; cc.Y<(y+ycs); cc.Y++)
	{
		for(cc.X=x; cc.X<(x+xcs); cc.X++)
		{
			ClearChannel(channel,cc);
		}
	}
}
//======================================================-==--=-- --  -
void	CPCMap::Clear44Channel(int channel, const CCellRef &c)
{
	int	x = c.X&0xfffffffc;
	int	y = c.Y&0xfffffffc;

	ASSERT((x<MAP_WIDTH)&&(y<MAP_HEIGHT));
	int xcs = (x!=(MAP_WIDTH-4))+4;				// ensure only 4 verts
	int ycs = (y!=(MAP_HEIGHT-4))+4;			// wide on left and top

	C4x4*	ff = &m4x4s[(y>>2)*(MAP_WIDTH>>2)+(x>>2)];
	ff->SetTexture(channel,0);

	CCellRef	cc;
	for(cc.Y=y; cc.Y<(y+ycs); cc.Y++)
	{
		for(cc.X=x; cc.X<(x+xcs); cc.X++)
		{
			ClearChannel(channel,cc);
		}
	}
}
 //======================================================-==--=-- --  -
// ensure texture is loaded and get index in maptextures array
int		CPCMap::GetTexNo(char *tname)
{
	// is this texture already loaded?
	SINT c0;
	for (c0 = 0; c0 < (MAX_MAP_TEXTURES); c0 ++)
	{
		if (!(stricmp(tname, mMapTextureNames[c0])))
		{
			goto qaz;
		}
	}

	// nope - so look for a blank texture and load it in.

	for (c0 = 1; c0 < MAX_MAP_TEXTURES; c0 ++)
	{
		if(!mMapTextureNames[c0][0])	break;
	}
	if(c0==(MAX_MAP_TEXTURES)) return 0;


	// nothing here!
	mMapTextures[c0] = CTextureGTEX::GetTexture(tname);
	if (mMapTextures[c0])
	{
		strcpy(mMapTextureNames[c0], tname);
	}
	// couldn't load file.  or something
	else return 0;

qaz:

//	m4x4s[((c.Y>>2)*(MAP_WIDTH>>2))+(c.X>>2)].SetTexture(channel, c0);
	return c0;//TRUE;
}
  //======================================================-==--=-- --  -
 // Not like set cell texture, _must_ be used in conjunction with
// GetTexNo
void	CPCMap::Set44Texture(int channel, CCellRef &c, int tnum)
{
	m4x4s[((c.Y>>2)*(MAP_WIDTH>>2))+(c.X>>2)].SetTexture(channel, tnum);
}
  //======================================================-==--=-- --  -
 // Changed by GC to do less checking, makes system more reliant on
// ReconcileMapTextures, seems to work fine
BOOL	CPCMap::SetCellTexture(CCellRef &c, char *tname)
{
	// is this texture already loaded?
	SINT c0;
	for (c0 = 0; c0 < (MAX_MAP_TEXTURES); c0 ++)
	{
		if (!(stricmp(tname, mMapTextureNames[c0])))
		{
			goto if_jez_can_use_goto_so_can_i;
		}
	}

	// nope - so look for a blank texture and load it in.

	for (c0 = 1; c0 < MAX_MAP_TEXTURES; c0 ++)
	{
		if(!mMapTextureNames[c0][0])	break;
	}
	if(c0==(MAX_MAP_TEXTURES)) return FALSE;


	// nothing here!
	mMapTextures[c0] = CTextureGTEX::GetTexture(tname);
	if (mMapTextures[c0])
	{
		strcpy(mMapTextureNames[c0], tname);
	}
	// couldn't load file.  or something
	else return FALSE;

if_jez_can_use_goto_so_can_i:

	SetCellTNo(c, c0);

	return TRUE;
}
//******************************************************************************************
void	CPCMap::SetCellTextureDefault(CCellRef &c)
{
	SetCellTNo(c, 0);
}

//******************************************************************************************
void	CPCMap::ReconcileMapTextures()
{
	// scan through ensuring that each texture is really used...
	SINT	c0;
	for (c0 = 1; c0 < (MAX_MAP_TEXTURES); c0 ++)  // leave in default texture
	{
		if (mMapTextureNames[c0][0] != 0)
		{
			SINT	cx, cy;
			BOOL	found = FALSE;

			for (cx = 0; cx < MAP_WIDTH; cx++)
				for (cy = 0; cy < MAP_HEIGHT; cy++)
					if (GetCell(CCellRef(cx, cy))->mTextureNo == c0)
						found = TRUE;

// and check the three passes of the 44 map

			C4x4*	ff=m4x4s;
			for (cy = 0; cy < MAP_HEIGHT; cy+=4)
				for (cx = 0; cx < MAP_WIDTH; ff++, cx+=4)
					if((ff->GetTexture(0) == c0)||(ff->GetTexture(1) == c0)||(ff->GetTexture(2) == c0))
						found = TRUE;


			if (!found)
			{
				// oh dear - this texture shouldn't be here

				// first copy down textures
				SINT	c1;

				for (c1 = c0; c1 < MAX_MAP_TEXTURES - 1; c1 ++)
				{
					mMapTextures[c1] = mMapTextures[c1 + 1];
					strcpy(mMapTextureNames[c1], mMapTextureNames[c1 + 1]);
				}
				mMapTextures[MAX_MAP_TEXTURES - 1] = NULL;
				mMapTextureNames[MAX_MAP_TEXTURES - 1][0] = 0;

				// and then change the references
				for (cx = 0; cx < MAP_WIDTH; cx++)
					for (cy = 0; cy < MAP_HEIGHT; cy++)
					{
						SINT t = GetCell(CCellRef(cx, cy))->mTextureNo;

						if (t > c0)
							GetCell(CCellRef(cx, cy))->mTextureNo --;
					}
				ff=m4x4s;
				for (cy = 0; cy < MAP_HEIGHT; cy+=4)
					for (cx = 0; cx < MAP_WIDTH; ff++, cx+=4)
						for(int	cc=0; cc<3; cc++)
						{
							SINT	t = ff->GetTexture(cc);
							if(t> c0) ff->SetTexture(cc,t-1);
						}

				//#### and the other map

				c0 --;  // because the numbers have changed..
			}
		}
	}
}

 //******************************************************************************************
// ** save/load

UINT	MAPVER = MKID("1.70");

BOOL	CPCMap::Read(CMEMBUFFER &mem)
{
	// get version number
	UINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson
	if (ver == MKID("1.70"))
	{
		SINT c0, c1;
		SINT w, h;
		mem.Read(&w, 4);
		mem.Read(&h, 4);

		// quick bodge for size
		SINT xs = 0, ys = 0;
		while ((w & 1) == 0)
		{
			w >>= 1;
			xs ++;
		}
		while ((h & 1) == 0)
		{
			h >>= 1;
			ys ++;
		}

		if (!(MAP.Resize(xs, ys)))
			return FALSE;

		// read texture names
		mem.Read(&mMapTextureNames, MAX_MAP_TEXTURES * 256); 

		// allocate textures...
		for (c0 = 0; c0 < MAX_MAP_TEXTURES; c0++)
		{
			if (mMapTextureNames[c0][0] != 0)
			{
				// Little bit of a hack here - check to see if we need to rename this texture
				// because it is arena specific.
				if ((strlen(mMapTextureNames[c0]) > 9) && (mMapTextureNames[c0][8] == '_'))
				{
					if ((toupper(mMapTextureNames[c0][6]) != 'H') ||
						(toupper(mMapTextureNames[c0][7]) != 'P'))
					{
						switch (GAME.GetLevelData()->mIVars[IV_PRISON_NUMBER])
						{
						case 0:	mMapTextureNames[c0][6] = 'E'; mMapTextureNames[c0][7] = 'C'; break;
						case 1:	mMapTextureNames[c0][6] = 'Z'; mMapTextureNames[c0][7] = 'P'; break;
						case 2:	mMapTextureNames[c0][6] = 'H'; mMapTextureNames[c0][7] = 'O'; break;
						case 3:	mMapTextureNames[c0][6] = 'S'; mMapTextureNames[c0][7] = 'W'; break;
						case 4:	mMapTextureNames[c0][6] = 'R'; mMapTextureNames[c0][7] = 'E'; break;
						case 5:	mMapTextureNames[c0][6] = 'M'; mMapTextureNames[c0][7] = 'I'; break;
						case 6:	mMapTextureNames[c0][6] = 'D'; mMapTextureNames[c0][7] = 'R'; break;
						case 7:	mMapTextureNames[c0][6] = 'S'; mMapTextureNames[c0][7] = 'P'; break;
						case 8:	mMapTextureNames[c0][6] = 'M'; mMapTextureNames[c0][7] = 'L'; break;
						case 9:	mMapTextureNames[c0][6] = 'A'; mMapTextureNames[c0][7] = 'S'; break;
						};
					}
				}


				mMapTextures[c0] = CTextureGTEX::GetTexture(mMapTextureNames[c0]);
				if (!mMapTextures[c0])
				{
					char text[200];
					sprintf(text, "Unable to load maptex - %s",mMapTextureNames[c0]);
					CONSOLE.AddString(text);
				}
			}
		}

		for (c0 = 0; c0 < MAP_HEIGHT; c0++)
		{
			for (c1 = 0; c1 < MAP_WIDTH; c1++)
			{
				CCellRef	c = CCellRef(c1, c0);

				GINT	h;
				mem.Read(&h, 4);
				GetCell(c)->SetHeight(h);

				SINT	*t = (SINT *)(&GetCell(c)->mType);
				mem.Read(t, 4);

				t = (SINT *)(&GetCell(c)->mTextureNo);

				mem.Read(t, 4);
				// check that texture is actually there!
				if (!(mMapTextures[*t]))
				{
					*t = 0;  // no - use default.
					GAME.DeclareLevelDataOld();
				}

				mem.Read(&GetCell(c)->mColour, 4);
				mem.Read(&GetCell(c)->mMix, 1);
				mem.Read(&GetCell(c)->mRamp, 1);

			}
		}
// and the 4x4 map

		for(c0=0; c0<(MAP_HEIGHT>>2); c0++)
		{
			for(c1=0; c1<(MAP_WIDTH>>2); c1++)
			{
				EFrictionType	ft[3];

				C4x4*	ff = &m4x4s[c0*(MAP_WIDTH>>2)+c1];
				UBYTE bt;
				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(0,bt);
				ft[0] = CFriction::GetFrictionForTexture(mMapTextures[bt]->GetName() + 6); // +6 to skip header

				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(1,bt);
				ft[1] = CFriction::GetFrictionForTexture(mMapTextures[bt]->GetName() + 6); // +6 to skip header

				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(2,bt);
				ft[2] = CFriction::GetFrictionForTexture(mMapTextures[bt]->GetName() + 6); // +6 to skip header

				// and evaluate the friction for the cells in the 4x4
				// chose the friction of the texture with the largest mix value...
				SINT		c2, c3, mix0, mix1, mix2;
				CMapCell	*cell;
				for (c2 = c0 * 4; c2 < (c0 + 1) * 4; c2 ++)
					for (c3 = c1 * 4; c3 < (c1 + 1) * 4; c3 ++)
					{
						cell = GetCell(CCellRef(c3, c2));

						mix0 = (cell->mMix&0xf);
						mix1 = ((cell->mMix>>4)&0xf);
						mix2 = 15 - mix0 - mix1;

						if (mix0 > mix1)
						{
							if (mix0 > mix2)
								cell->SetFriction(ft[0]);
							else
								cell->SetFriction(ft[2]);
						}
						else
						{
							if (mix1 > mix2)
								cell->SetFriction(ft[1]);
							else
								cell->SetFriction(ft[2]);
						}
					}			
			}
		}
		Create44Masks(CCellRef(0,0),CCellRef(MAP_WIDTH-1,MAP_HEIGHT-1));
		SetupDefaultTexture();
		PrecalcCells();
		return TRUE;
	}
	else if (ver == MKID("1.60"))
	{
		GAME.DeclareLevelDataOld();

		SINT c0, c1;
		SINT w, h;
		mem.Read(&w, 4);
		mem.Read(&h, 4);

		// quick bodge for size
		SINT xs = 0, ys = 0;
		while ((w & 1) == 0)
		{
			w >>= 1;
			xs ++;
		}
		while ((h & 1) == 0)
		{
			h >>= 1;
			ys ++;
		}

		if (!(MAP.Resize(xs, ys)))
			return FALSE;

		// read texture names
		mem.Read(&mMapTextureNames, MAX_MAP_TEXTURES * 256);

		// allocate textures...
		for (c0 = 0; c0 < MAX_MAP_TEXTURES; c0++)
		{
			if (mMapTextureNames[c0][0] != 0)
			{
				mMapTextures[c0] = CTextureGTEX::GetTexture(mMapTextureNames[c0]);
				if (!mMapTextures[c0])
				{
					char text[200];
					sprintf(text, "Unable to load maptex - %s",mMapTextureNames[c0]);
					CONSOLE.AddString(text);
				}
			}
		}

		for (c0 = 0; c0 < MAP_HEIGHT; c0++)
		{
			for (c1 = 0; c1 < MAP_WIDTH; c1++)
			{
				CCellRef	c = CCellRef(c1, c0);

				GINT	h;
				mem.Read(&h, 4);
				GetCell(c)->SetHeight(h);

				SINT	*t = (SINT *)(&GetCell(c)->mType);
				mem.Read(t, 4);

				t = (SINT *)(&GetCell(c)->mTextureNo);

				mem.Read(t, 4);
				// check that texture is actually there!
				if (!(mMapTextures[*t]))
				{
					*t = 0;  // no - use default.
					GAME.DeclareLevelDataOld();
				}

				mem.Read(&GetCell(c)->mColour, 4);

				mem.Read(&GetCell(c)->mMix, 1);

			}
		}
// and the 4x4 map

		for(c0=0; c0<(MAP_HEIGHT>>2); c0++)
		{
			for(c1=0; c1<(MAP_WIDTH>>2); c1++)
			{
				C4x4*	ff = &m4x4s[c0*(MAP_WIDTH>>2)+c1];
				UBYTE bt;
				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(0,bt);
				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(1,bt);
				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(2,bt);
			}
		}
		Create44Masks(CCellRef(0,0),CCellRef(MAP_WIDTH-1,MAP_HEIGHT-1));
		SetupDefaultTexture();
		PrecalcCells();
		return TRUE;
	}
	else if (ver == MKID("1.50"))
	{
		GAME.DeclareLevelDataOld();

		SINT c0, c1;
		SINT w, h;
		mem.Read(&w, 4);
		mem.Read(&h, 4);

		// quick bodge for size
		SINT xs = 0, ys = 0;
		while ((w & 1) == 0)
		{
			w >>= 1;
			xs ++;
		}
		while ((h & 1) == 0)
		{
			h >>= 1;
			ys ++;
		}

		if (!(MAP.Resize(xs, ys)))
			return FALSE;

		// read texture names
		mem.Read(&mMapTextureNames, MAX_MAP_TEXTURES * 256);

		// allocate textures...
		for (c0 = 0; c0 < MAX_MAP_TEXTURES; c0++)
		{
			if (mMapTextureNames[c0][0] != 0)
			{
				mMapTextures[c0] = CTextureGTEX::GetTexture(mMapTextureNames[c0]);
				if (!mMapTextures[c0])
				{
					char text[200];
					sprintf(text, "Unable to load maptex - %s",mMapTextureNames[c0]);
					CONSOLE.AddString(text);
				}
			}
		}

		for (c0 = 0; c0 < MAP_HEIGHT; c0++)
		{
			for (c1 = 0; c1 < MAP_WIDTH; c1++)
			{
				CCellRef	c = CCellRef(c1, c0);

				GINT	h;
				mem.Read(&h, 4);
				GetCell(c)->SetHeight(h);

				SINT	*t = (SINT *)(&GetCell(c)->mType);
				mem.Read(t, 4);

				t = (SINT *)(&GetCell(c)->mTextureNo);

				mem.Read(t, 4);
				// check that texture is actually there!
				if (!(mMapTextures[*t]))
				{
					*t = 0;  // no - use default.
					GAME.DeclareLevelDataOld();
				}

				mem.Read(&GetCell(c)->mMix, 1);

			}
		}
// and the 4x4 map

		for(c0=0; c0<(MAP_HEIGHT>>2); c0++)
		{
			for(c1=0; c1<(MAP_WIDTH>>2); c1++)
			{
				C4x4*	ff = &m4x4s[c0*(MAP_WIDTH>>2)+c1];
				UBYTE bt;
				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(0,bt);
				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(1,bt);
				mem.Read(&bt,1);
				if(!mMapTextures[bt]) bt=0;
				ff->SetTexture(2,bt);
			}
		}
		Create44Masks(CCellRef(0,0),CCellRef(MAP_WIDTH-1,MAP_HEIGHT-1));
		SetupDefaultTexture();
		PrecalcCells();
		return TRUE;
	}

	else return FALSE; // didn't recognise the verson
}

//******************************************************************************************
BOOL	CPCMap::Write(CMEMBUFFER &mem)
{
	// write out verson number
	mem.Write(&MAPVER, 4);

	// write out map size
	SINT a = MAP_WIDTH;
	SINT b = MAP_HEIGHT;

	mem.Write(&a, 4);
	mem.Write(&b, 4);

	SINT c0, c1;

	// write out textures
	ReconcileMapTextures(); // remove any spare spaces...
	mem.Write(&mMapTextureNames, MAX_MAP_TEXTURES * 256);

	// and Write out map heights

	for (c0 = 0; c0 < MAP_HEIGHT; c0++)
	{
		for (c1 = 0; c1 < MAP_WIDTH; c1++)
		{
			CCellRef	c = CCellRef(c1, c0);

			GINT h= GetCell(c)->GetUnderlyingHeight();
			mem.Write(&h, 4);
			SINT	t = SINT(CMapCell::GetCellSaveType(GetCell(c)->mType));
			mem.Write(&t, 4);

			t = GetCell(c)->mTextureNo;			// can go when engine unifies
			mem.Write(&t, 4);

			mem.Write(&GetCell(c)->mColour, 4);

			mem.Write(&GetCell(c)->mMix,1);

			mem.Write(&GetCell(c)->mRamp, 1);
		}
	}
// and the 4x4 map

	for(c0=0; c0<(MAP_HEIGHT>>2); c0++)
	{
		for(c1=0; c1<(MAP_WIDTH>>2); c1++)
		{
			C4x4*	ff = &m4x4s[c0*(MAP_WIDTH>>2)+c1];
			UBYTE bt;
			bt=ff->GetTexture(0);
			mem.Write(&bt,1);
			bt=ff->GetTexture(1);
			mem.Write(&bt,1);
			bt=ff->GetTexture(2);
			mem.Write(&bt,1);
		}
	}
	// all done

	return TRUE;
}



BOOL	CPCMap::WritePSX(CMEMBUFFER &mem)
{
	// write out verson number
	mem.Write(&MAPVER, 4);

	// write out map size
	SWORD a = MAP_WIDTH;
	SWORD b = MAP_HEIGHT;

	mem.Write(&a, 2);
	mem.Write(&b, 2);

	SINT c0, c1;

	// write out textures
	ReconcileMapTextures(); // remove any spare spaces...
	//mem.Write(&mMapTextureNames, MAX_MAP_TEXTURES * 256);

	// and Write out map heights

	for (c0 = 0; c0 < MAP_HEIGHT; c0++)
	{
		for (c1 = 0; c1 < MAP_WIDTH; c1++)
		{
			CCellRef	c = CCellRef(c1, c0);

			SWORD h= GetCell(c)->mHeight;
			mem.Write(&h, 2);
			UBYTE t=UBYTE(CMapCell::GetCellSaveType(GetCell(c)->mType));
			mem.Write(&t, 1);

			//= GetCell(c)->mTextureNo;			// can go when engine unifies
			//m.Write(&t, 4);
			mem.Write(&GetCell(c)->mColour, 4);

			mem.Write(&GetCell(c)->mMix,1);
			mem.Write(&GetCell(c)->mRamp,1);

			UBYTE	f = (UBYTE)GetCell(c)->GetFriction();
			mem.Write(&f, 1);
		}
	}
// and the 4x4 map

	for(c0=0; c0<(MAP_HEIGHT>>2); c0++)
	{
		for(c1=0; c1<(MAP_WIDTH>>2); c1++)
		{
			C4x4*	ff = &m4x4s[c0*(MAP_WIDTH>>2)+c1];
			UBYTE bt;
			bt=ff->GetTexture(0);
			mem.Write(&bt,1);
			bt=ff->GetTexture(1);
			mem.Write(&bt,1);
			bt=ff->GetTexture(2);
			mem.Write(&bt,1);
		}
	}
	// all done

	return TRUE;
}




BOOL	CPCMap::WriteDC(CMEMBUFFER &mem)
{
	// write out verson number
	mem.Write(&MAPVER, 4);

	// write out map size
	SWORD a = MAP_WIDTH;
	SWORD b = MAP_HEIGHT;

	mem.Write(&a, 2);
	mem.Write(&b, 2);

	SINT c0, c1;

	// write out textures
	ReconcileMapTextures(); // remove any spare spaces...
	//mem.Write(&mMapTextureNames, MAX_MAP_TEXTURES * 256);

	// and Write out map heights

	for (c0 = 0; c0 < MAP_HEIGHT; c0++)
	{
		for (c1 = 0; c1 < MAP_WIDTH; c1++)
		{
			CCellRef	c = CCellRef(c1, c0);

			SWORD h= GetCell(c)->mHeight;
			mem.Write(&h, 2);
			UBYTE t=UBYTE(CMapCell::GetCellSaveType(GetCell(c)->mType));
			mem.Write(&t, 1);

			//= GetCell(c)->mTextureNo;			// can go when engine unifies
			//m.Write(&t, 4);
			mem.Write(&GetCell(c)->mColour, 4);

			mem.Write(&GetCell(c)->mMix,1);
			mem.Write(&GetCell(c)->mRamp,1);

			UBYTE	f = (UBYTE)GetCell(c)->GetFriction();
			mem.Write(&f, 1);
		}
	}
// and the 4x4 map

	for(c0=0; c0<(MAP_HEIGHT>>2); c0++)
	{
		for(c1=0; c1<(MAP_WIDTH>>2); c1++)
		{
			C4x4*	ff = &m4x4s[c0*(MAP_WIDTH>>2)+c1];
			UBYTE bt;
			bt=ff->GetTexture(0);
			mem.Write(&bt,1);
			bt=ff->GetTexture(1);
			mem.Write(&bt,1);
			bt=ff->GetTexture(2);
			mem.Write(&bt,1);
		}
	}
	// all done

	return TRUE;
}

// --------------------------------------------------------
void	CPCMap::PostLoadProcess()
{
	// Nothing yet
}





           //======================================================-==--=-- --  -
          // define a triangle list for four*four Jeremy 'pyramid' cells
         // that's 4*4*4=64 triangles and 64*3=192 indices, which is sizeof(tilemesh)
        //
       // numbering convention for Jeremificated mesh:
      //
     //  0---1---2---3---4
    //   |\ /|\ /|\ /|\ /|					This is the case for the 4*4 map units
   //    | 5 | 6 | 7 | 8 |					The macros can build these with
  //     |/ \|/ \|/ \|/ \|					different Moduli and Offsets.
 //      9---10--11--12--13 etc....
//======================================================-==--=-- --  -
#define		FLEXIBLE_PCELL(X,M,O) X+M,X+O,X+0, X+0,X+O,X+1, X+M+1,X+O,X+M, X+1,X+O,X+M+1

#define		PYRAMID_CELL(X) FLEXIBLE_PCELL(X,9,5)

UWORD tilemesh[] = {PYRAMID_CELL(0), PYRAMID_CELL(1), PYRAMID_CELL(2), PYRAMID_CELL(3),
					PYRAMID_CELL(9), PYRAMID_CELL(10),PYRAMID_CELL(11),PYRAMID_CELL(12),
					PYRAMID_CELL(18),PYRAMID_CELL(19),PYRAMID_CELL(20),PYRAMID_CELL(21),
					PYRAMID_CELL(27),PYRAMID_CELL(28),PYRAMID_CELL(29),PYRAMID_CELL(30)};

// And a right clipped version, still uses full size vertex buffer
UWORD rc_tilemesh[] = { PYRAMID_CELL(0), PYRAMID_CELL(1), PYRAMID_CELL(2),
						PYRAMID_CELL(9), PYRAMID_CELL(10),PYRAMID_CELL(11),
						PYRAMID_CELL(18),PYRAMID_CELL(19),PYRAMID_CELL(20),
						PYRAMID_CELL(27),PYRAMID_CELL(28),PYRAMID_CELL(29)};

#define		FLEXIBLE_PCELLNEW(X, M) X,X+M,X+1, X+1,X+M,X+M+1
#define		PYRAMID_CELL44(X) FLEXIBLE_PCELLNEW(X, 5)
#define		PYRAMID_CELL22(X) FLEXIBLE_PCELLNEW(X, 3)
#define		PYRAMID_CELL11(X) FLEXIBLE_PCELLNEW(X, 2)

UWORD tilemesh44[] = {	PYRAMID_CELL44(0),	PYRAMID_CELL44(1),	PYRAMID_CELL44(2),	PYRAMID_CELL44(3),
						PYRAMID_CELL44(5),	PYRAMID_CELL44(6),	PYRAMID_CELL44(7),	PYRAMID_CELL44(8),
						PYRAMID_CELL44(10),	PYRAMID_CELL44(11), PYRAMID_CELL44(12), PYRAMID_CELL44(13),
						PYRAMID_CELL44(15),	PYRAMID_CELL44(16), PYRAMID_CELL44(17), PYRAMID_CELL44(18)};

UWORD tilemesh43[] = {	PYRAMID_CELL44(0),	PYRAMID_CELL44(1),	PYRAMID_CELL44(2),	PYRAMID_CELL44(3),
						PYRAMID_CELL44(5),	PYRAMID_CELL44(6),	PYRAMID_CELL44(7),	PYRAMID_CELL44(8),
						PYRAMID_CELL44(10),	PYRAMID_CELL44(11), PYRAMID_CELL44(12), PYRAMID_CELL44(13)};

UWORD tilemesh34[] = {	PYRAMID_CELL44(0),	PYRAMID_CELL44(1),	PYRAMID_CELL44(2),
						PYRAMID_CELL44(5),	PYRAMID_CELL44(6),	PYRAMID_CELL44(7),
						PYRAMID_CELL44(10),	PYRAMID_CELL44(11), PYRAMID_CELL44(12),
						PYRAMID_CELL44(15),	PYRAMID_CELL44(16), PYRAMID_CELL44(17)};

UWORD tilemesh33[] = {	PYRAMID_CELL44(0),	PYRAMID_CELL44(1),	PYRAMID_CELL44(2),
						PYRAMID_CELL44(5),	PYRAMID_CELL44(6),	PYRAMID_CELL44(7),
						PYRAMID_CELL44(10),	PYRAMID_CELL44(11), PYRAMID_CELL44(12)};

UWORD tilemesh22[] = {	PYRAMID_CELL22(0),	PYRAMID_CELL22(1),
						PYRAMID_CELL22(3),	PYRAMID_CELL22(4)};

UWORD tilemesh11[] = {	PYRAMID_CELL11(0)};


// and some for drawing things like booster squares
#define	PC_1W(X) FLEXIBLE_PCELL(X,3,2)
UWORD pmesh1[] = {	PC_1W(0),
					PC_1W(3),
					PC_1W(6),
					PC_1W(9)};
#define PC_2W(X) FLEXIBLE_PCELL(X,5,3)
UWORD pmesh2[] = {	PC_2W(0), PC_2W(1),
					PC_2W(5), PC_2W(6),
					PC_2W(10),PC_2W(11),
					PC_2W(15),PC_2W(16)};

//======================================================-==--=-- --  -
int	myrandom()
{
	static seed = 4997;
	return (seed = (seed*4997)+1223);
}

//***********************************************
DWORD	jcl_light(DWORD a, DWORD b)
{
	// rather odd function = assume a is range 0 - 0xffffffff, b is range 0 - 0x7f7f7f7f, and clip...
	SINT	ar, ag, ab, aa;
	SINT	br, bg, bb, ba;
	SINT	cr, cg, cb, ca;

	aa = (a >> 24) & 0xff;
	ar = (a >> 16) & 0xff;
	ag = (a >> 8 ) & 0xff;
	ab = (a      ) & 0xff;

	if (b & (1 << 31))
		 ba = 0xff;
	else ba = ((b >> 23)) & 0xfe;
	if (b & (1 << 23))
		 br = 0xff;
	else br = ((b >> 15)) & 0xfe;
	if (b & (1 << 15))
		 bg = 0xff;
	else bg = ((b >> 7 )) & 0xfe;
	if (b & (1 << 7))
		 bb = 0xff;
	else bb = ((b << 1 )) & 0xfe;

	ca = (aa * ba) >> 8;
	cr = (ar * br) >> 8;
	cg = (ag * bg) >> 8;
	cb = (ab * bb) >> 8;

	return (ca << 24) + (cr << 16) + (cg << 8) + cb;
	
}

void	CPCMap::GCRender(C3DGamut* gamut)
{
	PROFILE_FN(RenderMap);

	CFVF_UVRGB	mapverts[5*5+4*4];

	int	which_vb = 0;

//	CGamut	*gamut;
	FMatrix ori = ID_FMATRIX;
	FVector pos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);

	SINT oxc, oyc, ixc, iyc; // outer and inner x and y counters

//	MAP.ReGenMix(); for amusment purposes only

// Jez's engine does a bit of cursor specular stuff here, I should do the same really.

// Gamut stuff here, Gamut needs to work at 4*4 res of course.

//
// this routine is all very temporary and will change with DX7 shell

	Vertemp	tv[5*5]; // temporary buffer for verts

	SINT	last_x = (MAP_WIDTH>>2)-1;
	SINT	last_y = (MAP_HEIGHT>>2)-1;
	SINT	start_y = 0>>2;
	SINT	end_y = MAP_HEIGHT>>2;
	SINT	clip_this_y = end_y-1;
	SINT	scroller = GAME.GetGameTurn();
	SINT	start_x, end_x;
	int		extraindex=0;

	start_x = 0>>2;
	end_x = MAP_WIDTH>>2;
	SINT	clip_this_x = end_x-1;


	int	tt = gamut->XMin;
	if(start_x<tt) start_x = tt;
	tt = gamut->YMin;
	if(start_y<tt) start_y = tt;
	tt = gamut->XMax;
	if(end_x>tt) end_x = tt;
	tt = gamut->YMax;
	if(end_y>tt) end_y = tt;

	
	for(oyc = start_y; oyc < end_y; oyc++)
	{

		for(oxc = start_x; oxc < end_x; oxc++)
		{
			C4x4*		fftemp = &m4x4s[oyc*(MAP_WIDTH>>2)+oxc];
				AddWaterParticles(start_x >> 2, start_y >> 2);	
			if(gamut->NoClipInGamut3d(oxc,oyc,fftemp->mMin,fftemp->mMax))
			{
// build a 5*5 temporary copy of the tile's vertices
				UWORD*	the_trilist = tilemesh;
				SINT	the_tricount = 4*4*4*3;
				SINT	xclip = 0;
				SINT	yclip = 0;

				if(oxc==clip_this_x)
				{
					xclip = 1;
					the_trilist = rc_tilemesh;
					the_tricount = 4*4*3*3;
				}
				if(oyc==clip_this_y)
				{
					yclip = 1;
					the_tricount = (3*the_tricount)>>2;
				}

				SINT		basex = oxc<<2;
				SINT		basey = oyc<<2;
				Vertemp*	vert = tv;
				CMapCell*	bcell = GetCell(CCellRef(basex,basey));
//				C4x4*		fftemp = &m4x4s[oyc*(MAP_WIDTH>>2)+oxc];
				SINT		tn = bcell->mTextureNo;
				for(iyc=0; iyc<=(4-yclip); vert+= xclip, iyc++)
				{
					for(ixc=0; ixc<=(4-xclip); vert++, ixc++)
					{
						CCellRef	c	= CCellRef(basex+ixc, basey+iyc);
						CMapCell*	cell= GetCell(c);
						vert->mPos.X = float(c.X);
						vert->mPos.Y = float(c.Y);
						vert->mPos.Z = GToF(cell->GetHeight());
	// self illuminating stuff?...
						vert->mShade = /*MIX_SHADES/2;//*/GetMixerShade(cell[0].GetHeight(),cell[1].GetHeight());
	//					vert->mLightColour = cell->GetColour();
						vert->mLightColour = cell->mColour;
	// is it the cursor point?... (will have to be done on another pass
	//					for(int cc=0; cc<UO_MIXMAX; cc++)	vert->mMix[cc] = cell->GetCheapMix(cc);

						vert->mMix[0] = (cell->mMix&0xf);
						vert->mMix[1] = ((cell->mMix>>4)&0xf);
						vert->mMix[2] = 15-vert->mMix[0]-vert->mMix[1];
						vert->CanIMove = cell->CanIMove();
	// editor highlight region?...
					}
				}
	#ifdef LINK_EDITOR
				if(ENGINE.GetMapDrawMode()==2)//EDITOR.IsActive())
				{

					int	ec;
					vert = tv;
					for(ec=4; ec; vert++, ec--)
					{
						vert->mShade += 32;
						if(vert->mShade>=MIX_SHADES) vert->mShade=MIX_SHADES-1;
					}
					vert = &tv[5];
					for(ec=3; ec; vert+=5, ec--)
					{
						vert->mShade += 32;
						if(vert->mShade>=MIX_SHADES) vert->mShade=MIX_SHADES-1;
					}
					vert = &tv[9];
					for(ec=3; ec; vert+=5, ec--)
					{
						vert->mShade -= 32;
						if(vert->mShade<0) vert->mShade=0;
					}
					vert = &tv[21];
					for(ec=4; ec; vert++, ec--)
					{
						vert->mShade -= 32;
						if(vert->mShade<0) vert->mShade=0;
					}
				}
	#endif

	// up to UO_MIXMAX passes of texture
				for(SINT pass = 0; pass<3; pass++)
				{
					if(fftemp->GetTextureMask(pass))							// is this cell used?
					{
						CFVF_UVRGB*		dvp;	
						if(pass<=fftemp->GetAditiveAfter())
						{
							int q;
							q = fftemp->GetTexture(pass);
							CTextureGTEX* t = mMapTextures[q];
							t->Activate();
							dvp = mapverts;
						}
						else
						{
							int q;
							q = fftemp->GetTexture(pass);
							mExtras[extraindex].Texture = mMapTextures[q];
							mExtras[extraindex].TriList = the_trilist;
							mExtras[extraindex].TriCount = the_tricount;
							dvp = mExtras[extraindex].Verts;
							extraindex++;
							if(extraindex==NO_EXTRA_PASSES) extraindex--;
						}
						CFVF_UVRGB*		dvpp5 = dvp+5;
						
						Vertemp*	svp = tv;						// source
						float u, v;
						for(iyc=0, v=0; iyc<=(4-yclip); v+=0.25, dvp+=(4+xclip),svp+=xclip, iyc++)			// dvp+4 because rel vb has four extra
						{
							for(ixc=0, u=0; ixc<=(4-xclip); u+=0.25, dvp++, svp++, ixc++)
							{
								dvp->mPos		= svp->mPos;
								dvp->mDiffuse	= jcl_light(mMixers[svp->mShade][svp->mMix[pass]], svp->mLightColour);;
								dvp->mU			= u;
								dvp->mV			= v;
							}
						}

						dvp = dvpp5;					// destination
						svp = tv;						// source

						for(iyc=0, v=0.125; iyc<(4-yclip); v+=0.25, dvp+=(5+xclip), svp+=(1+xclip), iyc++)			// dvp+4 because rel vb has four extra
						{
							for(ixc=0, u=0.125; ixc<(4-xclip); u+=0.25, dvp++, svp++, ixc++)
							{
								dvp->mPos		= (svp[0].mPos+svp[1].mPos+svp[5].mPos+svp[6].mPos)*0.25f;
								dvp->mDiffuse	= jcl_light(mMixers[(svp[0].mShade + svp[1].mShade +
																	 svp[5].mShade + svp[6].mShade)>>2]
																   [(svp[0].mMix[pass] + svp[1].mMix[pass] +
																	 svp[5].mMix[pass]+svp[6].mMix[pass])>>2], 
															get_avg_colour(get_avg_colour(svp[0].mLightColour, svp[1].mLightColour),
																		   get_avg_colour(svp[5].mLightColour, svp[6].mLightColour)));

								dvp->mU			= u;
								dvp->mV			= v;
							}
						}
						if(pass<=fftemp->GetAditiveAfter())
						{
							PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
								mapverts,5*5+4*4,the_trilist,the_tricount,0);
						}
					}
//					if(pass==fftemp->GetAditiveAfter())	ENGINE.EnableAdditiveAlpha(); // enable additive for further passes
				}
//				ENGINE.DisableAlpha();
	// now do a nasty cludge for non CT_NORMALs in the mTypes of the high res map
				if(1) // #### should be flagged in the 4x4
				{
					ECellType	lasttex = CT_INVALID;
					CTextureGTEX* tex;

					CFVF_GTEX	verts[5];		// uggh, braindead
					int	xc, yc, xcm, ycm;
					xcm = (oxc != last_x)+3;
					ycm = (oyc != last_y)+3;

					CMapCell*	cell = bcell;
					float		u, v;
					for(yc=0, v=0; yc<ycm; v+=0.25, cell+=MAP_WIDTH-xcm, yc++)
					{
						for(xc=0, u=0; xc<xcm; u+=0.25, cell++, xc++)
						{
							ECellType thistex = cell->mType;
							if((thistex != CT_NORMAL))//&&(thistex != CT_ICE)) // fuck ice, draw that some other way
							{
								Vertemp*	vt = &tv[yc*5+xc];

								verts[2].mPos		= (vt[0].mPos+vt[1].mPos+vt[5].mPos+vt[6].mPos)*0.25;
								verts[2].mDiffuse	= mMixers[(vt[0].mShade+vt[1].mShade+vt[5].mShade+vt[6].mShade)>>2][15];
								verts[2].mU			= u+0.125f;
								verts[2].mV			= v+0.125f;

								verts[0].mPos		= vt->mPos;
								verts[0].mDiffuse	= mMixers[vt->mShade][15];
								verts[0].mU			= u;
								verts[0].mV			= v;

								vt++;

								verts[1].mPos		= vt->mPos;
								verts[1].mDiffuse	= mMixers[vt->mShade][15];
								verts[1].mU			= u+0.25f;
								verts[1].mV			= v;

								vt+=4;

								verts[3].mPos		= vt->mPos;
								verts[3].mDiffuse	= mMixers[vt->mShade][15];
								verts[3].mU			= u;
								verts[3].mV			= v+0.25f;

								vt++;

								verts[4].mPos		= vt->mPos;
								verts[4].mDiffuse	= mMixers[vt->mShade][15];
								verts[4].mU			= u+0.25f;
								verts[4].mV			= v+0.25f;

								tex = mTypeTextures[thistex];
								SINT vis[5];

								for(int i=0; i<5; i++)
								{
									verts[i].mSpecular = 0;
									vis[i]=tex->AddVertex(&verts[i]);
								}
								
								
								tex->AddTri(vis[2],vis[1],vis[0]);
								tex->AddTri(vis[2],vis[4],vis[1]);
								tex->AddTri(vis[2],vis[3],vis[4]);
								tex->AddTri(vis[2],vis[0],vis[3]);
							}
						}
					}
				}
			}
		}
	}
	ENGINE.EnableAdditiveAlpha();

	ExtraPass* epp = mExtras;
	for(;extraindex; epp++, extraindex--)
	{
		epp->Texture->Activate();
		PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
			epp->Verts,5*5+4*4,epp->TriList,epp->TriCount,0);
	}
	ENGINE.DisableAlpha();

	for(int i=0; i<NUM_CELL_TYPES; i++)
	{
		CTextureGTEX* ty =	mTypeTextures[i];
		if(ty) ty->DrawAll();
	}

#ifdef LINK_EDITOR
	if(EDITOR.IsActive()&&(EDITOR.Get442Draw()>=0)) // show the layers under the cursor for the editor
	{
		C4x4*	ff = &m4x4s[EDITOR.Get442Draw()];
		CFVF_TLUV	verts[4];		// uggh, braindead
		verts[0].mPos = FVector(600,70,0.1f);
		verts[0].mRHW = 1.0f;
		verts[0].mU = 0;
		verts[0].mV = 1;

		verts[1].mPos = FVector(640,80,0.1f);
		verts[1].mRHW = 1.0f;
		verts[1].mU = 1;
		verts[1].mV = 1;

		verts[2].mPos = FVector(550,90,0.1f);
		verts[2].mRHW = 1.0f;
		verts[2].mU = 0;
		verts[2].mV = 0;

		verts[3].mPos = FVector(610,100,0.1f);
		verts[3].mRHW = 1.0f;
		verts[3].mU = 1;
		verts[3].mV = 0;

		for(int c=0; c<3; c++)
		{
			CTextureGTEX* t = mMapTextures[ff->GetTexture(c)];
			if(t)
			{
				t->Activate();
				PLATFORM.GetD3DDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, FVF_TLUV_FLAGS,
								verts, 4, NULL);
			}
			for(int c = 0; c<4; c++)
			{
				verts[c].mPos.Y-=12;
			}
		}
	}
#endif


		
	

	// Draw beam effects if necessary
	DrawPickupBeam(PLAYER(0));
	DrawLaserBeam(PLAYER(0));

	// Draw MapWho debug information if necessary
	if (GAME.GetGameRenderInfo()->mActive & GRI_MAPWHO_INFO)
		DrawMapWhoDebug();

}


// ----------------------------------------------------------------------------
void	CPCMap::PCRender(C3DGamut* gamut)
{
	PROFILE_FN(RenderMap);

	FMatrix ori = ID_FMATRIX;
	FVector pos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);


	SINT	start_y = 0>>2;
	SINT	end_y = MAP_HEIGHT>>2;
	SINT	start_x, end_x;
	start_x = 0>>2;
	end_x = MAP_WIDTH>>2;

	int	tt = gamut->XMin;
	if(start_x<tt) start_x = tt;
	tt = gamut->YMin;
	if(start_y<tt) start_y = tt;
	tt = gamut->XMax;
	if(end_x>tt) end_x = tt;
	tt = gamut->YMax;
	if(end_y>tt) end_y = tt;

	int a, b;

	for(a = start_y; a < end_y; a++)
	{

		for(b = start_x; b < end_x; b++)
		{
			C4x4*		fftemp = &m4x4s[a*(MAP_WIDTH>>2)+b];
			if(gamut->NoClipInGamut3d(b,a,fftemp->mMin,fftemp->mMax))

			{

				FVector l1 = GVectorToFVector(ENGINE.GetCamera()->mLookAt);
				FVector l2 = FVector(float(b) * 4.f, float(a) * 4.f, 0);
				
				float dist = (l1 - l2).MagnitudeSq();

				if (dist < 1600.f)
					PCRender4x4(b, a, 0);

				if ((dist > 900.5f) && (dist < 4900.f))
					PCRender2x2(b, a);
				
				if (!GAME.IsMultiPlayer())
				{
					if (dist > 4140.f)	
						if (dist < 16384.f)		
							PCRender1x1(b, a);
				}
			}
		}
	}

	// Draw edge sections.
	for (b = 0; b < (mMapWidth >> 2) - 1; b++)
	{
		FVector l1 = GVectorToFVector(ENGINE.GetCamera()->mLookAt);
		FVector l2 = FVector(float(b) * 4.f, float((mMapHeight / 4) - 1) * 4.f, 0);
		
		float dist = (l1 - l2).MagnitudeSq();
		if (dist < 16384.f)			
			PCRender4x3(b, (mMapHeight / 4) - 1);
	}
	
	for (b = 0; b < (mMapHeight >> 2) - 1; b++)
	{
		FVector l1 = GVectorToFVector(ENGINE.GetCamera()->mLookAt);
		FVector l2 = FVector(float((mMapWidth / 4) - 1) * 4.f, float(b) * 4.f, 0);
		
		float dist = (l1 - l2).MagnitudeSq();
		if (dist < 16384.f)			
			PCRender3x4((mMapWidth / 4) - 1,b);
	}  
	
	PCRender3x3((mMapWidth / 4) - 1, (mMapHeight / 4) - 1);

	
	// Draw beam effects if necessary
	DrawPickupBeam(PLAYER(0));
	DrawLaserBeam(PLAYER(0));

	CRTParticleSystem::Render(&mParticleSystem);
	CRTParticleSystem::Render(&mEffectParticleSystem);

	// Draw MapWho debug information if necessary
	if (GAME.GetGameRenderInfo()->mActive & GRI_MAPWHO_INFO)
		DrawMapWhoDebug();
}

// --------------------------------------------------------------------
void	CPCMap::PCRender4x4(SINT x, SINT y, SINT mode)
{
	// Mode indicates whether or not the code should draw a 4x4, 4x3, 3x4, or 3x3.
	// Mode values:
	// 1 = 4x3
	// 2 = 3x4
	// 3 = 3x3
	// all others values are assumed to indicate 4x4

	PROFILE_FN(RenderMap);

	CFVF_UVRGB	mapverts[5*5];
	Vertemp		tv[5*5]; 

	SINT		the_tricount;
	UWORD*		the_trilist;
	
	switch (mode)
	{
	case 1:
		the_tricount	= 4*3*2*3;
		the_trilist		= tilemesh43;
		break;
	case 2:
		the_tricount	= 3*4*2*3;
		the_trilist		= tilemesh34;
		break;
	case 3:
		the_tricount	= 3*3*2*3;
		the_trilist		= tilemesh33;
		break;
	default:
		the_tricount	= 4*4*2*3;
		the_trilist		= tilemesh44;
	}

	int			extraindex		= 0;
	C4x4*		fftemp			= &m4x4s[y * (MAP_WIDTH >> 2) + x];

	SINT		basex			= x << 2;
	SINT		basey			= y << 2;
	Vertemp*	vert			= tv;
	CMapCell*	bcell			= GetCell(CCellRef(basex, basey));
	SINT		tn				= bcell->mTextureNo;

	// High z bias
	ENGINE.GetDevice()->SetRenderState(D3DRENDERSTATE_ZBIAS, 12);

	AddWaterParticles(x << 2, y << 2);

	for (int i = 0; i <= 4; i++)
	{
		for (int j = 0; j <= 4; j++)
		{
			CCellRef	c		= CCellRef(basex + j, basey + i);
			CMapCell*	cell	= GetCell(c);

			vert->mPos.X		= float(c.X);
			vert->mPos.Y		= float(c.Y);
			vert->mPos.Z		= GToF(cell->GetHeight());
			vert->mShade		= GetMixerShade(cell[0].GetHeight(), cell[1].GetHeight());
			vert->mLightColour	= cell->mColour;
			vert->mMix[0]		= (cell->mMix&0xf);
			vert->mMix[1]		= ((cell->mMix>>4)&0xf);
			vert->mMix[2]		= 15-vert->mMix[0]-vert->mMix[1];
			vert->CanIMove		= cell->CanIMove();

			// Next vertex please.
			vert++;
		}
	}
		
	for (int pass = 0; pass < 3; pass++)
	{
		if (fftemp->GetTextureMask(pass))		// is this cell used?
		{
			CFVF_UVRGB*		dvp;	
			if (pass <= fftemp->GetAditiveAfter())
			{
				int q;
				q = fftemp->GetTexture(pass);
				CTextureGTEX* t = mMapTextures[q];
				t->Activate();
				dvp = mapverts;
			}
			else
			{
				int q;
				q = fftemp->GetTexture(pass);
				mExtras[extraindex].Texture		= mMapTextures[q];
				mExtras[extraindex].TriList		= the_trilist;
				mExtras[extraindex].TriCount	= the_tricount;
				dvp = mExtras[extraindex].Verts;
				extraindex++;
				if (extraindex == NO_EXTRA_PASSES) extraindex--;
			}
		
			Vertemp*	svp = tv;						// source
			float u, v;

			u = 0.0f;
			v = 0.0f;
			for (int i = 0; i <= 4; i++)
			{
				u = 0.0f;
				for (int j = 0; j <= 4; j++)
				{
					dvp->mPos		= svp->mPos;
					dvp->mDiffuse	= jcl_light(mMixers[svp->mShade][svp->mMix[pass]], svp->mLightColour);
					dvp->mU			= u;
					dvp->mV			= v;
					
					// Setup for next loop.
					u += 0.25f;
					dvp++;
					svp++;
				}

				// Setup for next loop.
				u = 0.0f;
				v += 0.25f;
			}

			if (pass <= fftemp->GetAditiveAfter())
			{
				PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
					mapverts, 5*5, the_trilist, the_tricount, 0);
			}
		}
	}


	ECellType	lasttex = CT_INVALID;
	CTextureGTEX* tex;

	CFVF_GTEX	verts[5];		// uggh, braindead
	int	xc, yc;

	// Higher z bias still
	ENGINE.GetDevice()->SetRenderState(D3DRENDERSTATE_ZBIAS, 14);

	CMapCell*	cell = bcell;
	float		u, v;
	for(yc = 0, v = 0.0f; yc < 4; v += 0.25f, cell += MAP_WIDTH - 4, yc++)
	{
		for(xc = 0, u = 0.0f; xc < 4; u += 0.25f, cell++, xc++)
		{
			ECellType thistex = cell->mType;
			if((thistex != CT_NORMAL))
			{
				SINT	sqx, sqy;
				sqx = xc & 3;
				sqy = (yc & 3);
			
				SINT type = cell->GetType();
				if ((type >= CT_BOOSTR) && (type <= CT_BOOSTD))
				{
					// animate
					SINT turn = GAME.GetGameTurn();

					if ((turn & 2) && (!(turn & 8)))
						sqx |= 2;
					else
						sqx &= ~2;

					if ((turn & 4) && (!(turn & 8)))
						sqy &= ~2;
					else
						sqy |= 2;
				}

				float	us, vs, tw, th;
				float	u0, u1, v0, v1;

				us = 0.0f;//tr->mX + 0.002f;
				vs = 0.0f;//tr->mY + 0.002f;
				tw = 0.25f;//(tr->mW - 0.002f) / 4.f;
				th = 0.25f;//(tr->mH - 0.002f) / 4.f;

				u0 = us + (tw * sqx);
				u1 = us + (tw * (sqx + 1));
				v0 = vs + (th * sqy);
				v1 = vs + (th * (sqy + 1));

				Vertemp*	vt = &tv[yc*5+xc];

			//	verts[2].mPos		= (vt[0].mPos+vt[1].mPos+vt[5].mPos+vt[6].mPos)*0.25;
			//	verts[2].mDiffuse	= mMixers[(vt[0].mShade+vt[1].mShade+vt[5].mShade+vt[6].mShade)>>2][15];
			//	verts[2].mU			= (u0 + u1) / 2.0f;//u+0.125f;
			//	verts[2].mV			= (v0 + v1) / 2.0f;//v+0.125f;

				verts[0].mPos		= vt->mPos;
				verts[0].mDiffuse	= mMixers[vt->mShade][15];
				verts[0].mU			= u0;//u;
				verts[0].mV			= v0;//v;

				vt++;

				verts[1].mPos		= vt->mPos;
				verts[1].mDiffuse	= mMixers[vt->mShade][15];
				verts[1].mU			= u1;//u+0.25f;
				verts[1].mV			= v0;//v;

				vt+=4;

				verts[2].mPos		= vt->mPos;
				verts[2].mDiffuse	= mMixers[vt->mShade][15];
				verts[2].mU			= u0;//u;
				verts[2].mV			= v1;//v+0.25f;

				vt++;

				verts[3].mPos		= vt->mPos;
				verts[3].mDiffuse	= mMixers[vt->mShade][15];
				verts[3].mU			= u1;//u+0.25f;
				verts[3].mV			= v1;//v+0.25f;

				tex = mTypeTextures[thistex];
				SINT vis[5];

				for(int i=0; i<4; i++)
				{
					verts[i].mPos.Z -= 0.03f;	// Higher z-bias is not enough.
					verts[i].mSpecular = 0;
					vis[i]=tex->AddVertex(&verts[i]);
				}
				
				tex->AddTri(vis[0],vis[3],vis[1]);
				tex->AddTri(vis[0],vis[2],vis[3]);
		//		
		//		tex->AddTri(vis[2],vis[1],vis[0]);
		//		tex->AddTri(vis[2],vis[4],vis[1]);
		//		tex->AddTri(vis[2],vis[3],vis[4]);
		//		tex->AddTri(vis[2],vis[0],vis[3]);
				tex->DrawAll();
			}
		}
	}

	// High z bias
	ENGINE.GetDevice()->SetRenderState(D3DRENDERSTATE_ZBIAS, 12);


	ENGINE.EnableAdditiveAlpha();
	ExtraPass* epp = mExtras;
	for(;extraindex; epp++, extraindex--)
	{
		epp->Texture->Activate();
		PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
			epp->Verts,5*5, epp->TriList, epp->TriCount, 0);
	}

	ENGINE.DisableAlpha();

	for (i=0; i<NUM_CELL_TYPES; i++)
	{
		CTextureGTEX* ty =	mTypeTextures[i];
		if(ty) ty->DrawAll();
	}
}

// --------------------------------------------------------------------
void	CPCMap::PCRender2x2(SINT x, SINT y)
{
	PROFILE_FN(RenderMap);

	CFVF_UVRGB	mapverts[3*3];
	Vertemp		tv[3*3]; 
	SINT		the_tricount	= 2*2*2*3;
	UWORD*		the_trilist		= tilemesh22;
	int			extraindex		= 0;

	C4x4*		fftemp			= &m4x4s[y * (MAP_WIDTH >> 2) + x];

	SINT		basex			= x << 2;
	SINT		basey			= y << 2;
	Vertemp*	vert			= tv;
	CMapCell*	bcell			= GetCell(CCellRef(basex, basey));
	SINT		tn				= bcell->mTextureNo;

	// Middle z bias
	ENGINE.GetDevice()->SetRenderState(D3DRENDERSTATE_ZBIAS, 8);

	for (int i = 0; i <= 2; i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			CCellRef	c		= CCellRef(basex + (j << 1), basey + (i << 1));
			CMapCell*	cell	= GetCell(c);

			vert->mPos.X		= float(c.X);
			vert->mPos.Y		= float(c.Y);
			vert->mPos.Z		= GToF(cell->GetHeight());
			vert->mShade		= GetMixerShade(cell[0].GetHeight(), cell[1].GetHeight());
			vert->mLightColour	= cell->mColour;
			vert->mMix[0]		= (cell->mMix&0xf);
			vert->mMix[1]		= ((cell->mMix>>4)&0xf);
			vert->mMix[2]		= 15-vert->mMix[0]-vert->mMix[1];
			vert->CanIMove		= cell->CanIMove();

			// Next vertex please.
			vert++;
		}
	}
		
	for (int pass = 0; pass < 3; pass++)
	{
		if (fftemp->GetTextureMask(pass))		// is this cell used?
		{
			CFVF_UVRGB*		dvp;	
			if (pass <= fftemp->GetAditiveAfter())
			{
				int q;
				q = fftemp->GetTexture(pass);
				CTextureGTEX* t = mMapTextures[q];
				t->Activate();
				dvp = mapverts;
			}
			else
			{
				int q;
				q = fftemp->GetTexture(pass);
				mExtras[extraindex].Texture		= mMapTextures[q];
				mExtras[extraindex].TriList		= the_trilist;
				mExtras[extraindex].TriCount	= the_tricount;
				dvp = mExtras[extraindex].Verts;
				extraindex++;
				if (extraindex == NO_EXTRA_PASSES) extraindex--;
			}
		
			Vertemp*	svp = tv;						// source
			float u, v;

			u = 0.0f;
			v = 0.0f;
			for (int i = 0; i <= 2; i++)
			{
				u = 0.0f;
				for (int j = 0; j <= 2; j++)
				{
					dvp->mPos		= svp->mPos;
					dvp->mDiffuse	= jcl_light(mMixers[svp->mShade][svp->mMix[pass]], svp->mLightColour);
					dvp->mU			= u;
					dvp->mV			= v;
					
					// Setup for next loop.
					u += 0.5f;
					dvp++;
					svp++;
				}

				// Setup for next loop.
				u = 0.0f;
				v += 0.5f;
			}

			if (pass <= fftemp->GetAditiveAfter())
			{
				PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
					mapverts, 3*3, the_trilist, the_tricount, 0);
			}
		}
	}

	ENGINE.EnableAdditiveAlpha();

	ExtraPass* epp = mExtras;
	for(;extraindex; epp++, extraindex--)
	{
		epp->Texture->Activate();
		PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
			epp->Verts, 3*3, epp->TriList, epp->TriCount, 0);
	}
	ENGINE.DisableAlpha();

	for (i=0; i<NUM_CELL_TYPES; i++)
	{
		CTextureGTEX* ty =	mTypeTextures[i];
		if(ty) ty->DrawAll();
	}
}

// --------------------------------------------------------------------
void	CPCMap::PCRender1x1(SINT x, SINT y)
{
	PROFILE_FN(RenderMap);

	CFVF_UVRGB	mapverts[2*2];
	Vertemp		tv[2*2]; 
	SINT		the_tricount	= 1*1*2*3;
	UWORD*		the_trilist		= tilemesh11;

	int			extraindex		= 0;
	
	C4x4*		fftemp			= &m4x4s[y * (MAP_WIDTH >> 2) + x];

	SINT		basex			= x << 2;
	SINT		basey			= y << 2;
	Vertemp*	vert			= tv;
	CMapCell*	bcell			= GetCell(CCellRef(basex, basey));
	SINT		tn				= bcell->mTextureNo;

	// Low z bias
	ENGINE.GetDevice()->SetRenderState(D3DRENDERSTATE_ZBIAS, 0);
	for (int i = 0; i <= 1; i++)
	{
		for (int j = 0; j <= 1; j++)
		{
			CCellRef	c		= CCellRef(basex + (j << 2), basey + (i << 2));
			CMapCell*	cell	= GetCell(c);

			vert->mPos.X		= float(c.X);
			vert->mPos.Y		= float(c.Y);
			vert->mPos.Z		= GToF(cell->GetHeight());
			vert->mShade		= GetMixerShade(cell[0].GetHeight(), cell[1].GetHeight());
			vert->mLightColour	= cell->mColour;
			vert->mMix[0]		= (cell->mMix&0xf);
			vert->mMix[1]		= ((cell->mMix>>4)&0xf);
			vert->mMix[2]		= 15-vert->mMix[0]-vert->mMix[1];
			vert->CanIMove		= cell->CanIMove();

			// Next vertex please.
			vert++;
		}
	}
		
	for (int pass = 0; pass < 3; pass++)
	{
		if (fftemp->GetTextureMask(pass))		// is this cell used?
		{
			CFVF_UVRGB*		dvp;	
			if (pass <= fftemp->GetAditiveAfter())
			{
				int q;
				q = fftemp->GetTexture(pass);
				CTextureGTEX* t = mMapTextures[q];
				t->Activate();
				dvp = mapverts;
			}
			else
			{
				int q;
				q = fftemp->GetTexture(pass);
				mExtras[extraindex].Texture		= mMapTextures[q];
				mExtras[extraindex].TriList		= the_trilist;
				mExtras[extraindex].TriCount	= the_tricount;
				dvp = mExtras[extraindex].Verts;
				extraindex++;
				if (extraindex == NO_EXTRA_PASSES) extraindex--;
			}

			Vertemp*	svp = tv;						// source
			float u, v;

			u = 0.0f;
			v = 0.0f;
			for (int i = 0; i <= 1; i++)
			{
				u = 0.0f;
				for (int j = 0; j <= 1; j++)
				{
					dvp->mPos		= svp->mPos;
					dvp->mDiffuse	= jcl_light(mMixers[svp->mShade][svp->mMix[pass]], svp->mLightColour);
					dvp->mU			= u;
					dvp->mV			= v;
					
					// Setup for next loop.
					u += 1.0f;
					dvp++;
					svp++;
				}

				// Setup for next loop.
				u = 0.0f;
				v += 1.0f;
			}

			if (pass <= fftemp->GetAditiveAfter())
			{
				PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
					mapverts, 2*2, the_trilist, the_tricount, 0);
			}
		}
	}
	

	ENGINE.EnableAdditiveAlpha();
	ExtraPass* epp = mExtras;
	for(;extraindex; epp++, extraindex--)
	{
		epp->Texture->Activate();
		PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
			epp->Verts, 2*2, epp->TriList, epp->TriCount, 0);
	}
	ENGINE.DisableAlpha();

	for (i=0; i<NUM_CELL_TYPES; i++)
	{
		CTextureGTEX* ty =	mTypeTextures[i];
		if(ty) ty->DrawAll();
	}
}


// --------------------------------------------------------------------
void		CPCMap::PCRender4x3(SINT x, SINT y)
{
	PCRender4x4(x, y, 1);
}

// --------------------------------------------------------------------
void		CPCMap::PCRender3x4(SINT x, SINT y)
{
	PCRender4x4(x, y, 2);
}

// --------------------------------------------------------------------
void		CPCMap::PCRender3x3(SINT x, SINT y)
{
	PCRender4x4(x, y, 3);
}

//======================================================-==--=-- --  -
void	CPCMap::GCRender22(C3DGamut* gamut)
{
#if ARE
	PROFILE_FN(RenderMap);

	CFVF_UVRGB	mapverts[3*3];

	int	which_vb = 0;

//	CGamut	*gamut;
	FMatrix ori = ID_FMATRIX;
	FVector pos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);

	SINT oxc, oyc, ixc, iyc; // outer and inner x and y counters

//	MAP.ReGenMix(); for amusment purposes only

// Jez's engine does a bit of cursor specular stuff here, I should do the same really.

// Gamut stuff here, Gamut needs to work at 4*4 res of course.

//
// this routine is all very temporary and will change with DX7 shell

	Vertemp	tv[3*3]; // temporary buffer for verts

	SINT	last_x = (MAP_WIDTH>>2)-1;
	SINT	last_y = (MAP_HEIGHT>>2)-1;
	SINT	start_y = 0>>2;
	SINT	end_y = MAP_HEIGHT>>2;
	SINT	clip_this_y = end_y-1;
	SINT	scroller = GAME.GetGameTurn();
	SINT	start_x, end_x;
	int		extraindex=0;

	start_x	= 0>>2;
	end_x	= MAP_WIDTH>>2;
	SINT	clip_this_x = end_x-1;

	int	tt = gamut->XMin;
	if(start_x<tt) start_x = tt;
	tt = gamut->YMin;
	if(start_y<tt) start_y = tt;
	tt = gamut->XMax;
	if(end_x>tt) end_x = tt;
	tt = gamut->YMax;
	if(end_y>tt) end_y = tt;

	
	for(oyc = start_y; oyc < end_y; oyc++)				// Y map grid squares
	{
		for(oxc = start_x; oxc < end_x; oxc++)			// X map grid squares
		{
			C4x4*		fftemp = &m4x4s[oyc*(MAP_WIDTH>>2)+oxc];

			//AddWaterParticles(start_x >> 2, start_y >> 2);	
			if (true)//gamut->NoClipInGamut3d(oxc, oyc, fftemp->mMin, fftemp->mMax))
			{
				// build a 3*3 temporary copy of the tile's vertices
				UWORD*	the_trilist		= tilemesh22;
				SINT	the_tricount	= 2*2*2*3;
				SINT	xclip = 0;
				SINT	yclip = 0;

				/*
				if(oxc==clip_this_x)
				{
					xclip = 1;
					the_trilist		= rc_tilemesh22;
					the_tricount	= 2*2*2*3; // ?
				}

				if(oyc==clip_this_y)
				{
					yclip = 1;
					the_tricount = the_tricount >> 2;
				}
*/

				// 
				SINT		basex = oxc << 2;
				SINT		basey = oyc << 2;
				Vertemp*	vert = tv;
				CMapCell*	bcell = GetCell(CCellRef(basex,basey));
//				C4x4*		fftemp = &m4x4s[oyc*(MAP_WIDTH>>2)+oxc];
				SINT		tn = bcell->mTextureNo;
				for(iyc = 0; iyc <= (2 - yclip); vert += xclip, iyc++)
				{
					for(ixc = 0; ixc <= (2 - xclip); vert++, ixc++)
					{
						CCellRef	c	= CCellRef(basex + ixc, basey + iyc);
						CMapCell*	cell= GetCell(c);
						vert->mPos.X = float(c.X);
						vert->mPos.Y = float(c.Y);
						vert->mPos.Z = GToF(cell->GetHeight());
	// self illuminating stuff?...
						vert->mShade = /*MIX_SHADES/2;//*/GetMixerShade(cell[0].GetHeight(),cell[1].GetHeight());
	//					vert->mLightColour = cell->GetColour();
						vert->mLightColour = cell->mColour;
	// is it the cursor point?... (will have to be done on another pass
	//					for(int cc=0; cc<UO_MIXMAX; cc++)	vert->mMix[cc] = cell->GetCheapMix(cc);

						vert->mMix[0] = (cell->mMix&0xf);
						vert->mMix[1] = ((cell->mMix>>4)&0xf);
						vert->mMix[2] = 15-vert->mMix[0]-vert->mMix[1];
						vert->CanIMove = cell->CanIMove();
	// editor highlight region?...
					}
				}
	
	// up to UO_MIXMAX passes of texture
				for(SINT pass = 0; pass<3; pass++)
				{
					if(fftemp->GetTextureMask(pass))		// is this cell used?
					{
						CFVF_UVRGB*		dvp;	

						if(pass <= fftemp->GetAditiveAfter())
						{
							int q;
							q = fftemp->GetTexture(pass);
							CTextureGTEX* t = mMapTextures[q];
							t->Activate();
							dvp = mapverts;
						}
						else
						{
							int q;
							q = fftemp->GetTexture(pass);
							mExtras[extraindex].Texture = mMapTextures[q];
							mExtras[extraindex].TriList = the_trilist;
							mExtras[extraindex].TriCount = the_tricount;
							dvp = mExtras[extraindex].Verts;
							extraindex++;
							if(extraindex==NO_EXTRA_PASSES) extraindex--;
						}

//						CFVF_UVRGB*		dvpp5 = dvp+5;
						
						Vertemp*	svp = tv;						// source
						float u, v;
						for(iyc=0, v=0.0f; iyc <= (2-yclip); v+=0.5f, svp+=xclip, iyc++)	//dvp+=(2+xclip),svp+=xclip, iyc++)			// dvp+4 because rel vb has four extra
						{
							for(ixc=0, u=0.0f; ixc<=(2-xclip); u+=0.5f, dvp++, svp++, ixc++)
							{
								dvp->mPos		= svp->mPos;
								dvp->mDiffuse	= jcl_light(mMixers[svp->mShade][svp->mMix[pass]], svp->mLightColour);;
								dvp->mU			= u;
								dvp->mV			= v;
							}
						}
						

						/*
						dvp = dvpp5;					// destination
						svp = tv;						// source

						for(iyc=0, v=0.125; iyc<(4-yclip); v+=0.25, dvp+=(5+xclip), svp+=(1+xclip), iyc++)			// dvp+4 because rel vb has four extra
						{
							for(ixc=0, u=0.125; ixc<(4-xclip); u+=0.25, dvp++, svp++, ixc++)
							{
								dvp->mPos		= (svp[0].mPos+svp[1].mPos+svp[5].mPos+svp[6].mPos)*0.25f;
								dvp->mDiffuse	= jcl_light(mMixers[(svp[0].mShade + svp[1].mShade +
																	 svp[5].mShade + svp[6].mShade)>>2]
																   [(svp[0].mMix[pass] + svp[1].mMix[pass] +
																	 svp[5].mMix[pass]+svp[6].mMix[pass])>>2], 
															get_avg_colour(get_avg_colour(svp[0].mLightColour, svp[1].mLightColour),
																		   get_avg_colour(svp[5].mLightColour, svp[6].mLightColour)));

								dvp->mU			= u;
								dvp->mV			= v;
							}
						}
						*/
						if(pass<=fftemp->GetAditiveAfter())
						{
							PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
								mapverts, 3*3, the_trilist, the_tricount, 0); // tv == mapverts
						}
					}
//					if(pass==fftemp->GetAditiveAfter())	ENGINE.EnableAdditiveAlpha(); // enable additive for further passes
				}


//				ENGINE.DisableAlpha();
	// now do a nasty cludge for non CT_NORMALs in the mTypes of the high res map
				if(1) // #### should be flagged in the 4x4
				{
					ECellType	lasttex = CT_INVALID;
					CTextureGTEX* tex;

					CFVF_GTEX	verts[4];		// uggh, braindead
					int	xc, yc, xcm, ycm;
					xcm = (oxc != last_x)+3; // +3???
					ycm = (oyc != last_y)+3;

					CMapCell*	cell = bcell;
					float		u, v;
					for(yc=0, v=0; yc<ycm; v+=0.25, cell+=MAP_WIDTH-xcm, yc++)
					{
						for(xc=0, u=0; xc<xcm; u+=0.25, cell++, xc++)
						{
							ECellType thistex = cell->mType;
							if((thistex != CT_NORMAL))//&&(thistex != CT_ICE)) // fuck ice, draw that some other way
							{
								Vertemp*	vt = &tv[yc*2+xc];

								verts[0].mPos		= vt->mPos;
								verts[0].mDiffuse	= mMixers[vt->mShade][15];
								verts[0].mU			= u;
								verts[0].mV			= v;
								vt++;

								verts[1].mPos		= vt->mPos;
								verts[1].mDiffuse	= mMixers[vt->mShade][15];
								verts[1].mU			= u+0.25f;
								verts[1].mV			= v;
								vt+=2;

								verts[2].mPos		= vt->mPos;
								verts[2].mDiffuse	= mMixers[vt->mShade][15];
								verts[2].mU			= u;
								verts[2].mV			= v+0.25f;
								vt++;

								verts[3].mPos		= vt->mPos;
								verts[3].mDiffuse	= mMixers[vt->mShade][15];
								verts[3].mU			= u+0.25f;
								verts[4].mV			= v+0.25f;

								tex = mTypeTextures[thistex];
								SINT vis[4];

								for(int i=0; i<4; i++)
								{
									verts[i].mSpecular = 0;
									vis[i]=tex->AddVertex(&verts[i]);
								}
								
								tex->AddTri(vis[0],vis[1],vis[3]);
								tex->AddTri(vis[0],vis[2],vis[3]);
								
								/*
								tex->AddTri(vis[2],vis[1],vis[0]);
								tex->AddTri(vis[2],vis[4],vis[1]);
								tex->AddTri(vis[2],vis[3],vis[4]);
								tex->AddTri(vis[2],vis[0],vis[3]);
								*/
							}
						}
					}
				}
			}
		}
	}
	ENGINE.EnableAdditiveAlpha();

	ExtraPass* epp = mExtras;
	for(;extraindex; epp++, extraindex--)
	{
		epp->Texture->Activate();
	//	PLATFORM.GetD3DDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, FVF_UVRGB_FLAGS,
	//		epp->Verts,5*5+4*4,epp->TriList,epp->TriCount,0);
	}
	ENGINE.DisableAlpha();

	for(int i=0; i<NUM_CELL_TYPES; i++)
	{
		CTextureGTEX* ty =	mTypeTextures[i];
		if(ty) ty->DrawAll();
	}


	// Draw beam effects if necessary
	DrawPickupBeam(PLAYER(0));
	DrawLaserBeam(PLAYER(0));

	// Draw MapWho debug information if necessary
	if (GAME.GetGameRenderInfo()->mActive & GRI_MAPWHO_INFO)
		DrawMapWhoDebug();
#endif
}

//======================================================-==--=-- --  -
void CPCMap::LoadPinMap(int levelno)
{
	char	pinname[2000];
	mPinAnim = FALSE;
	sprintf(pinname,"data\\levels\\splin%03d.dat", levelno);
	FILE* in;
	if((in = fopen(pinname,"rb")) != NULL)
	{
		int		version;
		int		loops;
		fread(pinname,4,1,in);					// skip	"BMAP"
		fread(&version,4,1,in);					// version
		fread(&loops,4,1,in);					// loop counter
		int		splinenodes;
		fread(&splinenodes,4,1,in);				// loop counter
		fread(pinname,4+(splinenodes*13), 1, in);	// skip selectednode and spline data
		fread(&mPinMapXSize,4,1,in);			// xsize
		fread(&mPinMapYSize,4,1,in);			// ysize
		int nverts =(mPinMapXSize+1)*(mPinMapYSize+1);
		if((nverts>0)&&(nverts<=(5*129)))
		{
			Pin* pp = mPinMap;
			for(;nverts; pp++, nverts--)
			{
				fread(&pp->U,2,1,in);
				fread(&pp->V,2,1,in);
				pp->V*=loops;
			}
			fread(pinname,2,1,in);				// a couple of editor only bytes
			fread(&MAP.mPinScroll,2,1,in);		// scroller

			fread(MAP.mFadeWave,4096,2,in);
			mPinAnim = TRUE;
		}
		fclose(in);
	}
}
//********************************************************************
int		CPCMap::GetRandNum()
{
	int num;
	
	num = mRandomNumbers[RandPos];

	RandPos++;
	
	if(RandPos > 250)
	{
		RandPos = 0;
	}

	return num;

}
//********************************************************************
void	CPCMap::AddWaterParticles(SINT start_x, SINT start_y)
{
	SINT	wh=GAME.GetLevelData()->mGVars[WATER_HEIGHT].GetVal();

	float	v = GToF(GAME.GetLevelData()->mGVars[WATER_HEIGHT]);

	if (fabs(v) < 0.0001)
		return; // no water.

	if (!(GAME.IsGamePanelActive()))
	{
		if((wh) && ((GetRandNum() & 0xff) < 8))       // Do we want a water decal?
		{
			SINT 	wx, wy;
			wx = start_x + (GetRandNum() & 3);
			wy = start_y + (GetRandNum() & 3);

 			SINT 	h1 = GetCell(CCellRef(wx, wy))->GetHeight().mVal;
			if(h1 > wh)
			{
				h1-=wh;
				SINT 	h2 = GetCell(CCellRef(wx+1, wy))->GetHeight().mVal-wh;
				SINT 	h3 = GetCell(CCellRef(wx+1, wy+1))->GetHeight().mVal-wh;
				SINT 	h4 = GetCell(CCellRef(wx, wy+1))->GetHeight().mVal-wh;
				if((h1|h2|h3|h4)<0)  // Only on water edge...
				{
					//printf("Edge!\n");
					//if((h1<0x10000)&&(h2<0x10000)&&(h3<0x10000)&&(h4<0x10000))  // Only shallow water/water edge.
					{
						CParticle *ptcl;
						ptcl	=mParticleSystem.CreateParticle(PART_ANIM);
						if(ptcl)
						{
							ptcl->mColour=0x00606050;
							ptcl->mPos.X.mVal=(wx << GINT_SHIFT) + (GetRandNum() & 0xffff);
							ptcl->mPos.Y.mVal=(wy << GINT_SHIFT) + (GetRandNum() & 0xffff);
							ptcl->mPos.Z.mVal=wh;
							//ptcl->mVelocity.X.mVal=0;
							//ptcl->mVelocity.Y.mVal=0x2000;

							GVector tgv=GetMapNormal(ptcl->mPos);
							ptcl->mVelocity.X.mVal=tgv.X.mVal>>3;
							ptcl->mVelocity.Y.mVal=tgv.Y.mVal>>3;
							ptcl->mVelocity.Z.mVal=0;

							GINT	scale = tgv.MagnitudeXY();
							if(scale.mVal>(0x8000))
							{
								ptcl->mVelocity.X.mVal>>=2;
								ptcl->mVelocity.Y.mVal>>=2;
							}
							if (!scale.IsZero())
							{
								scale = G1 / scale;
								tgv.X *= scale;
								tgv.Y *= scale;
							}
							ptcl->uPAnim.mSn=-tgv.X.mVal>>4;
							ptcl->uPAnim.mCs=tgv.Y.mVal>>4;
							ptcl->uPAnim.mFrame=TP_WAT00;
							ptcl->uPAnim.mRate=3;
							ptcl->uPAnim.mTick=3;
							ptcl->mRadius=180;
							ptcl->uPAnim.mFrames=6;
							ptcl->mDelay=1+(GetRandNum()&5);
							ptcl->uPAnim.mLoops=0;
							ptcl->uPAnim.mHoriz=2;  // Flags to use cs,sn instead of yaw....
						}
					}
				}
				else
				{
					SINT x=(wx<<GINT_SHIFT)+(GetRandNum()&0xffff);
					SINT y=(wy<<GINT_SHIFT)+(GetRandNum()&0xffff);
					CMapCell*cell=&mCells[((wy) << mMapWidthShift) + wx];
					SINT z=cell->GetHeight().mVal;
					CParticle *ptcl;
					for(int i=0;i<2;i++)
					{
						ptcl=mParticleSystem.CreateParticle(PART_WRING2);
						if(ptcl)
						{
							ptcl->mVelocity.Y.mVal=0;
							ptcl->mVelocity.Z.mVal=-0x100;
							ptcl->mDelay=i<<4;
							ptcl->mColour=0x00606050;
							ptcl->mPos.X.mVal=x;
							ptcl->mPos.Y.mVal=y;
							ptcl->mPos.Z.mVal=z;
						}
					}
				}
			}

		}
	}
}
//********************************************************************
//======================================================-==--=-- --  -
#ifdef LINK_EDITOR

void	CPCMap::AccumulatePSXResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	// pass map textures to Texture Accumulator
	SINT	c0;

	for (c0 = 0; c0 < MAX_MAP_TEXTURES; c0 ++)
	{
		if ((mMapTextureNames[c0][0]) && (mMapTextures[c0]))
		
			ta.AddTexture(mMapTextureNames[c0], TATT_MAP, c0);
	}

	// also add the stuff for the beam
	ta.AddTexture(mTexPickupBeam1, TATT_NUMBERED, TN_BEAM1, 64, 64, BD_4, 1, TRUE);
	ta.AddTexture(mTexPickupBeam2, TATT_NUMBERED, TN_BEAM2, -1, -1, BD_4, 1, TRUE);
	ta.AddTexture(mTexLaserBeam  , TATT_NUMBERED, TN_LASER, -1, -1, BD_4, 1, TRUE);

	// and some tiles
	ta.AddTexture("IceRock.tga"						, TATT_NUMBERED, TN_ICE);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_GUN]		, TATT_NUMBERED, TN_TRIGGER_GUN);
	ta.AddTexture(mTypeTextures[CT_BOOSTR]			, TATT_NUMBERED, TN_BOOSTR);
	ta.AddTexture(mTypeTextures[CT_BOOSTL]			, TATT_NUMBERED, TN_BOOSTL);
	ta.AddTexture(mTypeTextures[CT_BOOSTU]			, TATT_NUMBERED, TN_BOOSTU);
	ta.AddTexture(mTypeTextures[CT_BOOSTD]			, TATT_NUMBERED, TN_BOOSTD);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_GUN_ON]	, TATT_NUMBERED, TN_TRIGGER_GUN_ON);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_BAD_GUN]	, TATT_NUMBERED, TN_TRIGGER_BAD_GUN);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_WAVE]	, TATT_NUMBERED, TN_TRIGGER_WAVE);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_WAVE_ON]	, TATT_NUMBERED, TN_TRIGGER_WAVE_ON);
	ta.AddTexture(mTypeTextures[CT_HEALTH]			, TATT_NUMBERED, TN_HEALTH);
	ta.AddTexture(mTypeTextures[CT_HEALTH_ON]		, TATT_NUMBERED, TN_HEALTH_ON);
}

//******************************************************************************************
void	CPCMap::AccumulateDCResources(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	// pass map textures to Texture Accumulator
	SINT	c0;

	for (c0 = 0; c0 < MAX_MAP_TEXTURES; c0 ++)
	{
		if ((mMapTextureNames[c0][0]) && (mMapTextures[c0]))
		
			ta.AddTexture(mMapTextureNames[c0], TADCTT_MAP, c0);
	}

	// also add the stuff for the beam
	ta.AddTexture(mTexPickupBeam1, TADCTT_NUMBERED, TN_BEAM1, 64, 64, 1, TRUE);
	ta.AddTexture(mTexPickupBeam2, TADCTT_NUMBERED, TN_BEAM2, -1, -1, 1, TRUE);
	ta.AddTexture(mTexLaserBeam  , TADCTT_NUMBERED, TN_LASER, -1, -1, 1, TRUE);

	// and some tiles
	ta.AddTexture("IceRock.tga"						, TADCTT_NUMBERED, TN_ICE);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_GUN]		, TADCTT_NUMBERED, TN_TRIGGER_GUN);
	ta.AddTexture(mTypeTextures[CT_BOOSTR]			, TADCTT_NUMBERED, TN_BOOSTR);
	ta.AddTexture(mTypeTextures[CT_BOOSTL]			, TADCTT_NUMBERED, TN_BOOSTL);
	ta.AddTexture(mTypeTextures[CT_BOOSTU]			, TADCTT_NUMBERED, TN_BOOSTU);
	ta.AddTexture(mTypeTextures[CT_BOOSTD]			, TADCTT_NUMBERED, TN_BOOSTD);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_GUN_ON]	, TADCTT_NUMBERED, TN_TRIGGER_GUN_ON);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_BAD_GUN]	, TADCTT_NUMBERED, TN_TRIGGER_BAD_GUN);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_WAVE]	, TADCTT_NUMBERED, TN_TRIGGER_WAVE);
	ta.AddTexture(mTypeTextures[CT_TRIGGER_WAVE_ON]	, TADCTT_NUMBERED, TN_TRIGGER_WAVE_ON);
	ta.AddTexture(mTypeTextures[CT_HEALTH]			, TADCTT_NUMBERED, TN_HEALTH);
	ta.AddTexture(mTypeTextures[CT_HEALTH_ON]		, TADCTT_NUMBERED, TN_HEALTH_ON);
}

//******************************************************************************************

#endif

#endif
