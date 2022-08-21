// $Header$

// $Log$
// Revision 1.59  2000-10-19 15:42:27+01  are
// Jeremy's optamisations.
//
// Revision 1.58  2000-10-18 19:08:21+01  are
// Water working, at last.
//
// Revision 1.57  2000-10-18 17:39:04+01  asd
// bob
//
// Revision 1.56  2000-10-18 15:55:36+01  asd
// not really any changes
//
// Revision 1.55  2000-10-18 13:48:15+01  are
// Fixed bug in water.
//
// Revision 1.54  2000-10-18 10:03:47+01  are
// Water has high z bias.
//
// Revision 1.53  2000-10-17 12:30:31+01  are
// Initial renderstates modified.
//
// Revision 1.52  2000-10-16 18:54:09+01  asd
// changed stuff
//
// Revision 1.51  2000-10-16 16:24:48+01  are
// Set initial z bias.
//
// Revision 1.50  2000-10-16 14:55:39+01  are
// Water now has two passes.
//
// Revision 1.49  2000-10-16 14:29:21+01  are
// Loads fonts before they are used. :o)
//
// Revision 1.48  2000-10-16 10:18:14+01  are
// Dulled water and corrected horizontal flip.
//
// Revision 1.47  2000-10-14 21:14:30+01  are
// Water working as DC.
//
// Revision 1.46  2000-10-14 20:42:35+01  are
// Clamp textures. Rough new water.
//
// Revision 1.45  2000-10-12 14:19:39+01  are
// Nothing much.
//
// Revision 1.44  2000-10-11 15:59:57+01  are
// Camera interpolation.
//
// Revision 1.43  2000-10-10 12:21:21+01  are
// Removed all calls to alpha sorting routines.
//
// Revision 1.42  2000-10-10 11:57:14+01  are
// Sky cube should never be inside map area.
//
// Revision 1.41  2000-10-10 09:18:53+01  are
// Corrected sky cube - the top wasn't using the correct u-v's.
//
// Revision 1.40  2000-10-06 17:34:32+01  asd
// removed some comments and setRenderStates that were unnessesarily rpeated
//
// Revision 1.39  2000-10-06 15:29:06+01  are
// Set texture states in ChangeTexture.
//
// Revision 1.38  2000-10-06 14:49:27+01  asd
// enabled mip-mapping
//
// Revision 1.37  2000-10-06 09:51:31+01  asd
// changes made but commented out.
//
// Revision 1.36  2000-10-05 15:30:14+01  are
// SkyCube locked to Camera - disabled fogging in order to see said SkyCube.
//
// Revision 1.35  2000-10-05 13:46:30+01  are
// Cleaned up fogging and made water duller.
//
// Revision 1.34  2000-10-05 11:28:05+01  asd
// added then removed fuctions for setting lighting
//
// Revision 1.33  2000-10-04 17:12:56+01  are
// Fogging working - performance is hit!!!
//
// Revision 1.32  2000-10-04 15:17:45+01  asd
// set fog partially implimented
//
// Revision 1.31  2000-10-04 14:36:32+01  asd
// added fog code
//
// Revision 1.30  2000-10-04 11:37:52+01  are
// <>
//
// Revision 1.29  2000-10-03 12:01:27+01  are
// Larger cube size. 1024
//
// Revision 1.28  2000-10-02 17:14:20+01  are
// SkyCube in. Haven't checked roof as I can't get an angle to see it, but the walls are definately correct.
//
// Revision 1.27  2000-10-02 15:04:52+01  are
// SkyCube. You'll need a new copy of JCLEngine.h too.
//
// Revision 1.26  2000-10-02 13:53:54+01  jjs
// Fixed LINK_EDITOR dependancies.
//
// Revision 1.25  2000-10-02 13:49:38+01  are
// RenderSkyCube started.
//
// Revision 1.24  2000-09-30 14:24:35+01  jcl
// <>
//
// Revision 1.23  2000-09-29 11:20:52+01  jcl
// <>
//
// Revision 1.22  2000-09-28 13:55:34+01  jcl
// <>
//
// Revision 1.21  2000-09-27 11:39:12+01  jcl
// <>
//
// Revision 1.20  2000-09-27 09:05:40+01  jjs
// Added GetSysTimeFloat method.
//
// Revision 1.19  2000-09-25 05:16:39+01  jcl
// <>
//
// Revision 1.18  2000-09-23 12:35:38+01  jjs
// More EnginePanel changes.
//
// Revision 1.17  2000-09-22 14:36:45+01  jjs
// Added new textures.
//
// Revision 1.16  2000-09-22 14:10:04+01  jcl
// <>
//
// Revision 1.15  2000-09-21 15:10:07+01  jjs
// More EnginePanel changes.
//
// Revision 1.14  2000-09-20 16:27:52+01  jjs
// More EnginePanel changes.
//
// Revision 1.12  2000-09-14 17:48:18+01  jcl
// <>
//
// Revision 1.11  2000-09-14 16:32:21+01  are
// <>
//
// Revision 1.10  2000-09-14 13:24:58+01  are
// <>
//
// Revision 1.9  2000-09-14 11:37:00+01  are
// Corrected file name.
//
// Revision 1.8  2000-09-13 17:05:01+01  sds
// Editor Mods
//
// Revision 1.7  2000-09-13 16:07:58+01  are
// <>
//
// Revision 1.6  2000-09-13 14:15:53+01  are
// Added game panel into rendering loop.
//
// Revision 1.5  2000-09-11 15:06:32+01  are
// Water is now transparent
//
// Revision 1.4  2000-09-07 16:14:20+01  are
// Attempted to get in game water effect to work. As the water should be semi-transparent (i.e. it is an alphaed texture) it needs to be rendered in the correct order. Made changes to facilitate the correct rendering order.
//
// Revision 1.3  2000-08-05 10:33:01+01  jjs
// First PC milestone.
//
// Revision 1.2  2000-07-11 10:18:51+01  jjs
// First DirectX7 version.
//
// Revision 1.1  2000-07-11 10:14:21+01  jjs
// Pre Direct7 version.
//

#include	"Common.h"

#if TARGET == PC

#include	"JCLEngine.h"
#include	"Globals.h"
#include	"RawLoader.h"
#include	"Thing.h"
#include	"Profile.h"
#include	"Mesh.h"
#include	"Editors.h"
#include	"CharacterLevelData.h"
#include	"gcgamut.h"
#include	"Primary.h"
#include	"Pickup.h"
#include	"Player.h"
#include	"PCGamePanel.h"

#include	"RTMesh.h"

#include	"d3derr.h"

#include	<stdlib.h>

//******************************************************************************************
BOOL	CJCLEngine::Init()
{
	mDevice = PLATFORM.GetDPWin()->GetDevice();

	// intialise textures
	if (!(CTexture::InitAll()))
		return FALSE;

	// and Meshes
	if (!(CMesh::InitAll()))
		return FALSE;

	// and get how many processor ticks there are in a second
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);

	mFrameCount = 0;
	mTickCount = 0;
	mFPS = 0;
	mLastTexture = NULL;

	mTransformPosition = ZERO_FVECTOR;
	mTransformOrientation = ID_FMATRIX;
	mTransformCount = 0;

	PLATFORM.GetBackSurface()->Clear(0x00000000);

	// load default  stuff
	mDefaultMesh = CMesh::GetMesh("default.msh", "");
//	mDefaultMesh = CMesh::GetMesh("soldmor.msh", "");
	ASSERT(mDefaultMesh);

	mDefaultTextureGTEX = CTextureGTEX::GetTexture("default.tga");
	ASSERT(mDefaultTextureGTEX);

	// clear numbered meshes
	SINT	c0;
	
	for (c0 = 0; c0 < GAME.GetNumNumberedMeshes(); c0 ++)
	{
		mNumberedMeshes[c0] = NULL;
	}

	Reset();

	mAltMapDraw = 1; // sorry about the magic number

	m3DGamut = new C3DGamut(34); // 26

	// Load the fonts before they are used. :o)
	mFonts[0].font		= CTextureTL::GetTexture("fonts\\fnt1eng.tga");
	mFonts[1].font		= CTextureTL::GetTexture("fonts\\fnt2eng.tga");
	mFonts[2].font		= CTextureTL::GetTexture("fonts\\fnt3eng.tga");

	CMEMBUFFER	mem;

	mem.InitFromFile("data\\frontend\\font1.dat");
	mem.Read(&mFonts[0].f, sizeof(STCFont));
	mem.Close();

	mem.InitFromFile("data\\frontend\\font2.dat");
	mem.Read(&mFonts[1].f, sizeof(STCFont));
	mem.Close();

	mem.InitFromFile("data\\frontend\\font3.dat");
	mem.Read(&mFonts[2].f, sizeof(STCFont));
	mem.Close();

	mFonts[0].mX = 256;
	mFonts[0].mY = 128;
	
	mFonts[1].mX = 128;
	mFonts[1].mY = 64;
	
	mFonts[2].mX = 256;
	mFonts[2].mY = 128;

	mRenderingView = 0;

	return TRUE;
}

//******************************************************************************************
void	CJCLEngine::Reset()
{
	mGamutType = GAMUT_LOWERED;
	mNumGlobalMeshes = 0;
	mNumRelocatedMeshes = 0;
}


//******************************************************************************************
BOOL	CJCLEngine::LoadLevelTextures()
{
	// load some textures

	mTexCursor			= CTextureTL::GetTexture("cursor_1.raw");
	mTexLevelComplete	= CTextureTL::GetTexture("LevelComplete_1.raw");
	mTexDied			= CTextureTL::GetTexture("FadeCircle_2.raw");
	mTexGolfShot		= CTextureTL::GetTexture("shot_1.raw");

	mTexNumbers			= CTextureTL::GetTexture("Numbers_3.raw");
	mTexIcons			= CTextureTL::GetTexture("Icons_3.raw");

	mSkyTexture			= CTextureGTEX::GetTexture("sky.tga");
	mTexWater			= CTextureGTEXAdditive::GetTexture("water.tga");

//	mTexSplash			= CTextureTL::GetTexture("splash.tga");
	mTexSplash			= mTexCursor; // safety...

	mTexOutline			= CTextureGTEXAdditive::GetTexture("outline.tga");

	// Box stuff
	mTexBarLeft			= CTextureTL::GetTexture("frontend\\1butbitl.tga");
	mTexBarMiddle		= CTextureTL::GetTexture("frontend\\1butbitm.tga");
	mTexBarRight		= CTextureTL::GetTexture("frontend\\1butbitr.tga");

	// Panel Stuff
	mTexPanelBar		= CTextureTL::GetTexture("panel\\colourbar.tga");
	mTexPanelBar2		= CTextureTL::GetTexture("panel\\colourbar2.tga");
	mTexPanelBackground	= CTextureTLAdditive::GetTexture("panel\\outlinebox.tga");

	mTexStart1			= CTextureTL::GetTexture("startl1.tga");
	mTexStart2			= CTextureTL::GetTexture("startl2.tga");
	mTexStart3			= CTextureTL::GetTexture("startl3.tga");

	mTexHealth0			= CTextureTL::GetTexture("frontend\\health0.tga");
	mTexHealth1			= CTextureTL::GetTexture("frontend\\health1.tga");
	mTexHealth2			= CTextureTL::GetTexture("frontend\\health2.tga");
	mTexHealth3			= CTextureTL::GetTexture("frontend\\health3.tga");

	// Fonts are loaded in init - they are referenced in the loading 
	// screen, ironically to print the message 'Loading...'.
//	mFonts[0].font		= CTextureTL::GetTexture("fonts\\fnt1eng.tga");
//	mFonts[1].font		= CTextureTL::GetTexture("fonts\\fnt2eng.tga");
//	mFonts[2].font		= CTextureTL::GetTexture("fonts\\fnt3eng.tga");

	mClock				= CTextureTL::GetTexture("frontend\\s_clock.tga");
	mPanel				= CTextureTL::GetTexture("frontend\\s_stat.tga");

	mLevelType[0]		= CTextureTL::GetTexture("frontend\\s_golf.tga");
	mLevelType[1]		= CTextureTL::GetTexture("frontend\\s_race.tga");
	mLevelType[2]		= CTextureTL::GetTexture("frontend\\s_last.tga");
	mLevelType[3]		= CTextureTL::GetTexture("frontend\\s_gaunt.tga");
	mLevelType[4]		= CTextureTL::GetTexture("frontend\\s_pursuit.tga");
	mLevelType[5]		= CTextureTL::GetTexture("frontend\\s_trick.tga");
	mLevelType[6]		= CTextureTL::GetTexture("frontend\\s_king.tga");
	mLevelType[7]		= CTextureTL::GetTexture("frontend\\s_droid.tga");

	mStartArm			= CTextureTL::GetTexture("frontend\\arm.tga");
	
	/* loaded in init
	CMEMBUFFER	mem;

	mem.InitFromFile("data\\frontend\\font1.dat");
	mem.Read(&mFonts[0].f, sizeof(STCFont));
	mem.Close();

	mem.InitFromFile("data\\frontend\\font2.dat");
	mem.Read(&mFonts[1].f, sizeof(STCFont));
	mem.Close();

	mem.InitFromFile("data\\frontend\\font3.dat");
	mem.Read(&mFonts[2].f, sizeof(STCFont));
	mem.Close();

	mFonts[0].mX = 256;
	mFonts[0].mY = 128;
	
	mFonts[1].mX = 128;
	mFonts[1].mY = 64;
	
	mFonts[2].mX = 256;
	mFonts[2].mY = 128;
	*/

	// ----
	// sky cube stuff
	char	bn[3];
	
	switch (CAREER.mCurrentPrison)
	{
	case  0:	strcpy(bn, "EC"); break;
	case  1:	strcpy(bn, "ZP"); break;
	case  2:	strcpy(bn, "HO"); break;
	case  3:	strcpy(bn, "SW"); break;
	case  4:	strcpy(bn, "RE"); break;
	case  5:	strcpy(bn, "MI"); break;
	case  6:	strcpy(bn, "DR"); break;
	case  7:	strcpy(bn, "SP"); break;
	case  8:	strcpy(bn, "ML"); break;
	case  9:	strcpy(bn, "AS"); break;
	case 10:	strcpy(bn, "LM"); break;
	}

	char	fn[200];
	sprintf(fn, "meshtex\\%s_cent.tga", bn);
	mTexSkyCentre			= CTextureGTEX::GetTexture(fn);

	sprintf(fn, "meshtex\\%s_up.tga", bn);
	mTexSkyUp				= CTextureGTEX::GetTexture(fn);
	
	sprintf(fn, "meshtex\\%s_down.tga", bn);
	mTexSkyDown				= CTextureGTEX::GetTexture(fn);

	sprintf(fn, "meshtex\\%s_left.tga", bn);
	mTexSkyLeft				= CTextureGTEX::GetTexture(fn);

	sprintf(fn, "meshtex\\%s_right.tga", bn);
	mTexSkyRight			= CTextureGTEX::GetTexture(fn);

#ifdef LINK_EDITOR
	mTexEdArrow			= CTextureGTEXAdditive::GetTexture("EdArrow.tga");
#endif
	return TRUE;
}

//******************************************************************************************


//******************************************************************************************
void	CJCLEngine::Shutdown()
{
	CMesh::ReleaseAll();
	CTexture::ReleaseAll();
	delete m3DGamut;
}

float	CJCLEngine::GetSysTimeFloat(void)
{
	LARGE_INTEGER ts;
	
	static	BOOL	fs_done = FALSE;
	static LARGE_INTEGER first_seen;

	if(frequency.QuadPart)
	{
		QueryPerformanceCounter(&ts);
		if (!fs_done)
		{
			first_seen.QuadPart = ts.QuadPart;
			fs_done = TRUE;
		}
		return float(ts.QuadPart- first_seen.QuadPart) / float(frequency.QuadPart);
	}

	return float(timeGetTime()) / 1000.0f;
	
}
//******************************************************************************************
void	CJCLEngine::PrepareLevel()
{
	// load numbered meshes
	SINT	c0;
	
	for (c0 = 0; c0 < GAME.GetNumNumberedMeshes(); c0 ++)
	{
		mNumberedMeshes[c0] = GAME.GetNumberedMesh(c0);
	}
}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CJCLEngine::AccumulatePSXResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	ta.AddTexture(mTexHealth0, TATT_NUMBERED, TN_HEALTH0, -1, -1, BD_8);
	ta.AddTexture(mTexHealth1, TATT_NUMBERED, TN_HEALTH1, -1, -1, BD_8);
	ta.AddTexture(mTexHealth2, TATT_NUMBERED, TN_HEALTH2, -1, -1, BD_8);
	ta.AddTexture(mTexHealth3, TATT_NUMBERED, TN_HEALTH3, -1, -1, BD_8); 

	ta.AddTexture(mTexStart1, TATT_NUMBERED, TN_START1, -1, -1, BD_8, 1, TRUE); 
	ta.AddTexture(mTexStart2, TATT_NUMBERED, TN_START2, -1, -1, BD_8, 1, TRUE); 
	ta.AddTexture(mTexStart3, TATT_NUMBERED, TN_START3, -1, -1, BD_8, 1, TRUE); 

	ta.AddTexture("ARM.TGA" , TATT_NUMBERED, TN_START_ARM, -1, -1, BD_8, 1, TRUE); 

	ta.AddTexture("frontend\\s_stat.tga"		, TATT_NUMBERED, TN_PANEL, -1, -1, BD_8, 1, TRUE); 
	ta.AddTexture("frontend\\s_clock.tga"		, TATT_NUMBERED, TN_CLOCK, -1, -1, BD_8, 1, TRUE); 
	ta.AddTexture("frontend\\1butbitl.tga"	, TATT_NUMBERED, TN_BOXL, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\1butbitm.tga"	, TATT_NUMBERED, TN_BOXM, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\1butbitr.tga"	, TATT_NUMBERED, TN_BOXR, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);

	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
	case LT_KING:		ta.AddTexture("frontend\\s_king.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	case LT_LASTMAN:	ta.AddTexture("frontend\\s_last.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	case LT_GAUNTLET:	ta.AddTexture("frontend\\s_gaunt.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	case LT_PURSUIT:	ta.AddTexture("frontend\\s_pursuit.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	case LT_RACER:		ta.AddTexture("frontend\\s_race.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	case LT_TAG:		ta.AddTexture("frontend\\s_trick.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	case LT_DROID_BALL:	ta.AddTexture("frontend\\s_droid.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	case LT_GOLF:		ta.AddTexture("frontend\\s_golf.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	default:			ta.AddTexture("frontend\\s_golf.tga"		, TATT_NUMBERED, TN_LEVELTYPE, -1, -1, BD_8);	break;
	};

	ta.AddTexture("frontend\\arrow_l.tga"	, TATT_NUMBERED, TN_ARROW_L, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\arrow_r.tga"	, TATT_NUMBERED, TN_ARROW_R, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\arrow_u.tga"	, TATT_NUMBERED, TN_ARROW_U, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\arrow_d.tga"	, TATT_NUMBERED, TN_ARROW_D, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);


	ASSERT(mDefaultMesh);
	mDefaultMesh->AccumulateResources(ta, ma);
}

//******************************************************************************************
void	CJCLEngine::AccumulateDCResources(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	ta.AddTexture(mTexHealth0, TADCTT_NUMBERED, TN_HEALTH0, -1, -1);
	ta.AddTexture(mTexHealth1, TADCTT_NUMBERED, TN_HEALTH1, -1, -1);
	ta.AddTexture(mTexHealth2, TADCTT_NUMBERED, TN_HEALTH2, -1, -1);
	ta.AddTexture(mTexHealth3, TADCTT_NUMBERED, TN_HEALTH3, -1, -1); 

	ta.AddTexture(mTexStart1 , TADCTT_NUMBERED, TN_START1 , -1, -1, 1, TRUE); 
	ta.AddTexture(mTexStart2 , TADCTT_NUMBERED, TN_START2 , -1, -1, 1, TRUE); 
	ta.AddTexture(mTexStart3 , TADCTT_NUMBERED, TN_START3 , -1, -1, 1, TRUE); 

	ta.AddTexture("ARM.TGA" , TADCTT_NUMBERED, TN_START_ARM, -1, -1, 1, TRUE); 

	ta.AddTexture("frontend\\s_stat.tga"	, TADCTT_NUMBERED, TN_PANEL, -1, -1, 1, TRUE); 
	ta.AddTexture("frontend\\s_clock.tga"	, TADCTT_NUMBERED, TN_CLOCK, -1, -1, 1, TRUE); 
	ta.AddTexture("frontend\\1butbitl.tga"	, TADCTT_NUMBERED, TN_BOXL , -1, -1, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\1butbitm.tga"	, TADCTT_NUMBERED, TN_BOXM , -1, -1, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\1butbitr.tga"	, TADCTT_NUMBERED, TN_BOXR , -1, -1, 2, TRUE, TRUE, TRUE);

	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
	case LT_KING:		ta.AddTexture("frontend\\s_king.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	case LT_LASTMAN:	ta.AddTexture("frontend\\s_last.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	case LT_GAUNTLET:	ta.AddTexture("frontend\\s_gaunt.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	case LT_PURSUIT:	ta.AddTexture("frontend\\s_pursuit.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	case LT_RACER:		ta.AddTexture("frontend\\s_race.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	case LT_TAG:		ta.AddTexture("frontend\\s_trick.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	case LT_DROID_BALL:	ta.AddTexture("frontend\\s_droid.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	case LT_GOLF:		ta.AddTexture("frontend\\s_golf.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	case LT_NORMAL:		ta.AddTexture("frontend\\s_beam.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	default:			ta.AddTexture("frontend\\s_golf.tga"		, TADCTT_NUMBERED, TN_LEVELTYPE, -1, -1);	break;
	};

	ta.AddTexture("frontend\\arrow_l.tga"	, TADCTT_NUMBERED, TN_ARROW_L, -1, -1, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\arrow_r.tga"	, TADCTT_NUMBERED, TN_ARROW_R, -1, -1, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\arrow_u.tga"	, TADCTT_NUMBERED, TN_ARROW_U, -1, -1, 2, TRUE, TRUE, TRUE);
	ta.AddTexture("frontend\\arrow_d.tga"	, TADCTT_NUMBERED, TN_ARROW_D, -1, -1, 2, TRUE, TRUE, TRUE);

	ta.AddTexture("white.tga", TADCTT_NUMBERED, TN_WHITE, -1, -1);

	// sky cube stuff
	char	bn[3];

	switch (CAREER.mCurrentPrison)
	{
	case  0:	strcpy(bn, "EC"); break;
	case  1:	strcpy(bn, "ZP"); break;
	case  2:	strcpy(bn, "HO"); break;
	case  3:	strcpy(bn, "SW"); break;
	case  4:	strcpy(bn, "RE"); break;
	case  5:	strcpy(bn, "MI"); break;
	case  6:	strcpy(bn, "DR"); break;
	case  7:	strcpy(bn, "SP"); break;
	case  8:	strcpy(bn, "ML"); break;
	case  9:	strcpy(bn, "AS"); break;
	case 10:	strcpy(bn, "LM"); break;
	}

	char	fn[200];

	sprintf(fn, "meshtex\\%s_cent.tga", bn);
	ta.AddTexture(fn, TADCTT_NUMBERED, TN_SKY_CENT);
	sprintf(fn, "meshtex\\%s_up.tga", bn);
	ta.AddTexture(fn, TADCTT_NUMBERED, TN_SKY_UP);
	sprintf(fn, "meshtex\\%s_down.tga", bn);
	ta.AddTexture(fn, TADCTT_NUMBERED, TN_SKY_DOWN);
	sprintf(fn, "meshtex\\%s_left.tga", bn);
	ta.AddTexture(fn, TADCTT_NUMBERED, TN_SKY_LEFT);
	sprintf(fn, "meshtex\\%s_right.tga", bn);
	ta.AddTexture(fn, TADCTT_NUMBERED, TN_SKY_RIGHT);

	ASSERT(mDefaultMesh);
	mDefaultMesh->AccumulateResourcesDC(ta, ma);
}
#endif


//******************************************************************************************
void CJCLEngine::RenderPlayers2D(CPlayer* for_player, SINT left_pixels_offset, SINT screen_width  )
{
	CGameRenderInfo *gri = GAME.GetGameRenderInfo();

	CPrimary *pr = for_player->GetPrimary();

	if (!pr)
		return;

	HDC hDC = PLATFORM.GetDPWin()->GetDC(); // ####!!!!
	
	SetTextColor(hDC, RGB(255,255,0));
	SetBkMode(hDC, TRANSPARENT);

	char text[200];
	SINT size;

	// draw win lose draw

	if (for_player->GetLevelResult() == LR_DRAW)
	{
		size = sprintf(text, "GAME DRAWN");
		TextOut(hDC, ((screen_width/2) - 20)+left_pixels_offset, PLATFORM.ScreenHeight() - 250, text, size);
	}

	if (for_player->GetLevelResult() == LR_WIN)
	{
		size = sprintf(text, "GAME WON");
		TextOut(hDC, ((screen_width/2) - 20)+left_pixels_offset, PLATFORM.ScreenHeight() - 250, text, size);
	}

	if (for_player->GetLevelResult() == LR_LOSE)
	{
		size = sprintf(text, "GAME LOST");
		TextOut(hDC, ((screen_width/2) - 20)+left_pixels_offset, PLATFORM.ScreenHeight() - 250, text, size);
	}



	size = sprintf(text, "Level: %d", GAME.GetCurrentLevelNumber());
	TextOut(hDC, left_pixels_offset + 10, PLATFORM.ScreenHeight() - 20, text, size);

	SINT	time;
	SINT	tl = GAME.GetLevelData()->mIVars[IV_TIME_LIMIT];


	SINT turns_taken = pr->GetLevelData().GetTurnsTaken() ;

	if (tl == 0)
		time = turns_taken / 25;
	else
		time = tl - (turns_taken / 25);

	if (time < 0)
		time = 0;

	if (time < 3600)
		size = sprintf(text, "Time: %02d:%02d", time / 60, time % 60);
	else
		size = sprintf(text, "Time: %d:%02d:%02d", time / 3600, (time % 3600) / 60, time % 60);
	TextOut(hDC, left_pixels_offset + 10, PLATFORM.ScreenHeight() - 40, text, size);


	
	// draw racer stuff

	CCharacterLevelData* cld = &pr->GetLevelData() ;
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER && cld)
	{
		SINT left_side = (left_pixels_offset + screen_width)  -80 ;

		// print position

		SINT position =  (cld->GetCurrentRacePosition() ) ;
		size = sprintf(text, "Pos: %d/%d", position, GAME.GetGameLevelData().GetStartCharsInGame() );
		TextOut(hDC, left_side , PLATFORM.ScreenHeight() - 220, text, size);


		// print what lap we are on

		SINT on_lap_number =  (cld->GetLapNumber()+1) ;
		if (on_lap_number > GAME.GetIVar(IV_NUM_LAPS_FOR_RACE)) on_lap_number = GAME.GetIVar(IV_NUM_LAPS_FOR_RACE) ;
		size = sprintf(text, "Lap %d/%d", on_lap_number, GAME.GetIVar(IV_NUM_LAPS_FOR_RACE) );
		
		TextOut(hDC, left_side , PLATFORM.ScreenHeight() - 200, text, size);

		// draw current lap time

		SINT current_lap_time = cld->GetCurrentLapTime() ;

		// if finsihed then print last lap time ;

		if (cld->GetLapNumber() == GAME.GetIVar(IV_NUM_LAPS_FOR_RACE) )
		{
			current_lap_time = cld->GetLastLapTime() ;
		}

		size = sprintf(text, "Time: %02d:%02d", current_lap_time / 25, ((current_lap_time *4) % 100));
		TextOut(hDC, left_side, PLATFORM.ScreenHeight() - 180, text, size);
		

		// draw best lap time

		SINT best_lap_time = cld->GetFastedLapTime() ;
		if (best_lap_time!=-1)
		{
			size = sprintf(text, "Best: %02d:%02d", best_lap_time / 25, ((best_lap_time *4) % 100));
		}
		else
		{
			size = sprintf(text, "Best: - -:- -");
		}
		TextOut(hDC, left_side, PLATFORM.ScreenHeight() - 160, text, size);
		

	}



	// display last man rolling stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_LASTMAN ||
		GAME.GetIVar(IV_LEVEL_TYPE) == LT_DROID_BALL)
	{
		if (!GAME.IsMultiPlayer())
		{
			SINT left_side = (left_pixels_offset + screen_width)  -100 ;
			size = sprintf(text, "Required %d", GAME.GetIVar(IV_POINTS_REQUIRED_TO_WIN_LEVEL) ) ;
			TextOut(hDC, left_side, PLATFORM.ScreenHeight() - 160, text, size);
		}
	}


	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_KING )
	{
		SINT left_side = (left_pixels_offset + screen_width)  -100 ;
		size = sprintf(text, "Required %d", GAME.GetIVar(IV_POINTS_REQUIRED_TO_WIN_LEVEL) ) ;
		TextOut(hDC, left_side, PLATFORM.ScreenHeight() - 160, text, size);
	}



	// display trick tag stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_TAG && cld)
	{
		if (GAME.IsMultiPlayer())
		{
			SINT left_side = (left_pixels_offset + screen_width)  -80 ;
			size = sprintf(text, "Tokens %d", cld->GetNumTokensPickedUp());
			TextOut(hDC, left_side, PLATFORM.ScreenHeight() - 160, text, size);
		}
		else
		{
			SINT left_side = (left_pixels_offset + screen_width)  -80 ;
			size = sprintf(text, "Tokens %d/%d", cld->GetNumTokensPickedUp(), CPUPToken::GetNumberTokensInWorld() +  cld->GetNumTokensPickedUp());
			TextOut(hDC, left_side, PLATFORM.ScreenHeight() - 160, text, size);
	}	}


	// debug stuff

	if (gri->mActive & GRI_DEBUG_TEXT)
	{
		size = sprintf(text, "%d FPS", mFPS);
		TextOut(hDC, left_pixels_offset, 0, text, size);
		size = sprintf(text, "%d Polys", mPolyCount);
		TextOut(hDC, left_pixels_offset, 16, text, size);

		SetTextColor(hDC, RGB(30,150,255));
		size = sprintf(text, "Mouse Pos ");
		TextOut(hDC, left_pixels_offset+100, 0, text, size);
		size = sprintf(text, "Active Cell");
		TextOut(hDC, left_pixels_offset+100, 16, text, size);
		size = sprintf(text, "Camera Pos");
		TextOut(hDC, left_pixels_offset+100, 32, text, size);
		size = sprintf(text, "Primary Pos");
		TextOut(hDC, left_pixels_offset+100, 48, text, size);

		SetTextColor(hDC, RGB(30,255,80));
		POINT	p = CONTROLS.GetMousePos();
		size = sprintf(text, "%d, %d", p.x, p.y);
		TextOut(hDC, left_pixels_offset+180, 0, text, size);
		size = sprintf(text, "%d, %d", for_player->GetCursor().X, for_player->GetCursor().Y);
		TextOut(hDC, left_pixels_offset+180, 16, text, size);
		size = sprintf(text, "%.2f, %.2f, %.2f", GToF(mCamera->mPos.X), GToF(mCamera->mPos.Y), GToF(mCamera->mPos.Z));
		TextOut(hDC, left_pixels_offset+180, 32, text, size);
		GVector v = pr->GetPos();
		size = sprintf(text, "%.2f, %.2f, %.2f", GToF(v.X), GToF(v.Y), GToF(v.Z));
		TextOut(hDC, left_pixels_offset+180, 48, text, size);
	}	

	if (gri->mActive & GRI_CHARACTER_INFO)
	{
		CCharacter* c = GAME.GetCurrentCharacterInfo() ;
		if (c)
		{
			SINT y =0 ;
			c->DrawDebugText(hDC,&y, text) ;
		}
	}


	PLATFORM.GetDPWin()->ReleaseDC(hDC); //####!!!!
}


//******************************************************************************************
void	CJCLEngine::Render2D()
{
	PROFILE_FN(Render2D);
#if defined(LINK_EDITOR)
	if (EDITOR.IsActive()) return ;
#endif
	// calc framerate.

	SINT new_tick = GetTickCount();
	if ((new_tick / 1000) != (mTickCount / 1000))
	{
		mFPS = (mFrameCount * 1000) / (new_tick - mTickCount);
		mTickCount = new_tick;
		mFrameCount = 0;
	}

	CGameRenderInfo *gri = GAME.GetGameRenderInfo();

	if (!(gri->mActive & GRI_2D))
		return;

	CONSOLE.Render();

#if ENABLE_PROFILER == 1
	if (gri->mActive & GRI_PROFILE)
		CProfiler::ShowProfile();
#endif

	
#if 0
	// one player 
	if (GAME.GetNumPlayers() == 1 )
	{
		RenderPlayers2D(GAME.GetPlayer(0), 0, PLATFORM.ScreenWidth() ) ;
	}
	// two players
	else
	{
		RenderPlayers2D(GAME.GetPlayer(0), 0, PLATFORM.ScreenWidth() >> 1 ) ;
		RenderPlayers2D(GAME.GetPlayer(1), PLATFORM.ScreenWidth() >> 1 , PLATFORM.ScreenWidth() >> 1) ;
	}
#endif
}

//******************************************************************************************
void	CJCLEngine::ChangeTexture(CTextureSurface *t)
{
	// don't change if this surface is already active, because it's slow!
	if (t != mLastTexture)
	{
		mLastTexture = t;
		PLATFORM.GetDPWin()->SetTexture(t); //####!!!!
	}
}

//******************************************************************************************
BOOL	CJCLEngine::AddFogEffect(	DWORD dwColour,
									DWORD dwMode,
									float fStart, 
									float fEnd, 
									float fDensity, 
									BOOL fUseRange)
{	
	HRESULT ddrval;
	
	// Enable fog blending.
	if((ddrval = mDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE)) != DD_OK)
	{
		OutputDebugString("Couldn't set fog as a render state\n");
		return FALSE;
	}
 
	// Set the fog color.
	if ((ddrval = mDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, dwColour)) != DD_OK)
	{
		OutputDebugString("Couldn't set the fog colour\n");
		return FALSE;
	}
	
	// Set fog parameters.
	if(D3DFOG_LINEAR == dwMode)
	{
		if ((ddrval = mDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, dwMode)) != DD_OK)
		{
			OutputDebugString("Couldn't set fog mode \n");
			return FALSE;
		}
		if ((ddrval = mDevice->SetRenderState(D3DRENDERSTATE_FOGSTART, *(DWORD *)(&fStart))) != DD_OK)
		{
			OutputDebugString("Couldn't set fog mode \n");
			return FALSE;
		}
		if ((ddrval = mDevice->SetRenderState(D3DRENDERSTATE_FOGEND,   *(DWORD *)(&fEnd))) != DD_OK)
		{
			OutputDebugString("Couldn't set fog mode \n");
			return FALSE;
		}
	}
	else
	{
		mDevice->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, dwMode);
		mDevice->SetRenderState(D3DRENDERSTATE_FOGDENSITY, *(DWORD *)(&fDensity));
	}

	// Enable range-based fog if desired (only supported for vertex fog).
	if(fUseRange == TRUE)
	{
		if (( ddrval = mDevice->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, TRUE)) != DD_OK)
		{
			OutputDebugString("Couldn't set range fog\n");
			return FALSE;
		}
	}
	return TRUE;
}

//******************************************************************************************
void	CJCLEngine::DisableFogEffect()
{
	mDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, false);
}

//******************************************************************************************
void	CJCLEngine::EnableAdditiveAlpha()
{
	mDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	mDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE );
	mDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	mDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE , FALSE );
}

//******************************************************************************************
void	CJCLEngine::EnableMultiplicativeAlpha()
{
	mDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

//	mDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE , TRUE );
//	mDevice->SetRenderState( D3DRENDERSTATE_ALPHAFUNC , D3DCMP_GREATEREQUAL );
//	mDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF  , 0x1000 );
	
	mDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
	mDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

//	mDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE , FALSE );
}

//******************************************************************************************
void	CJCLEngine::DisableAlpha()
{
	mDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
	mDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE , TRUE );
}

//******************************************************************************************
void	CJCLEngine::EnableUWrapping()
{
	mDevice->SetRenderState(D3DRENDERSTATE_WRAP0, D3DWRAPCOORD_0);
}

//******************************************************************************************
void	CJCLEngine::DisableUWrapping()
{
	mDevice->SetRenderState(D3DRENDERSTATE_WRAP0, 0);
}

//******************************************************************************************
void	ZeroMatrix(D3DMATRIX &mat)
{
	mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = mat._41 = 0.0f;
	mat._21 = mat._23 = mat._24 = mat._42 = 0.0f;
	mat._31 = mat._32 = mat._34 = mat._43 = 0.0f;
}

//******************************************************************************************
D3DMATRIX ProjectionMatrix(float near_plane, float far_plane, float fov_horiz, float fov_vert)
{
    float    h, w, Q; 
	w = 1.f / float(tan(fov_horiz * 0.5f));
    h = 1.f / float(tan(fov_vert  * 0.5f));
    Q = far_plane/(far_plane - near_plane); 

    D3DMATRIX ret;
	ZeroMatrix(ret);
    ret(0, 0) = w;
	ret(1, 1) = h;
    ret(2, 2) = Q;
    ret(3, 2) = -Q*near_plane;
    ret(2, 3) = 1;
	ret(3, 3) = 0;
    return ret;
}

//******************************************************************************************
void	CJCLEngine::SetupInitialRenderstates()
{
    mDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
    mDevice->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_CCW );

	mDevice->SetRenderState( D3DRENDERSTATE_ZBIAS, 0);
	mDevice->SetRenderState( D3DRENDERSTATE_ANTIALIAS , D3DANTIALIAS_NONE );

	mDevice->SetTextureStageState(0, D3DTSS_ADDRESS , D3DTADDRESS_CLAMP );
//	mDevice->SetTextureStageState(0, D3DTSS_ADDRESS , D3DTADDRESS_WRAP );

	mDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	mDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
	mDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );// D3DTFP_LINEAR D3DTFP_NONE

	mDevice->SetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE , TRUE );
	mDevice->SetRenderState( D3DRENDERSTATE_WRAP0 , 0 );
	mDevice->SetRenderState( D3DRENDERSTATE_FILLMODE , D3DFILL_SOLID );
	mDevice->SetRenderState( D3DRENDERSTATE_SHADEMODE , D3DSHADE_GOURAUD );
	mDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE , TRUE );

	mDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE , TRUE );

	mDevice->SetRenderState( D3DRENDERSTATE_LASTPIXEL , FALSE );
	mDevice->SetRenderState( D3DRENDERSTATE_ZFUNC , D3DCMP_LESSEQUAL );
	mDevice->SetRenderState( D3DRENDERSTATE_DITHERENABLE , TRUE );
	mDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF  , 0x00 );
	mDevice->SetRenderState( D3DRENDERSTATE_ALPHAFUNC  , D3DCMP_GREATER );

	mDevice->SetRenderState( D3DRENDERSTATE_EDGEANTIALIAS , FALSE );
	mDevice->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE , FALSE );
	mDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE , FALSE );	
	mDevice->SetTexture( 0 , NULL );

	mDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	

	D3DMATRIX m;
	
	if (mNumDisplays == 1)
		m= ProjectionMatrix(0.5f, 1000.0f, PLATFORM.GetFOVX(), PLATFORM.GetFOVY());
	else
		m= ProjectionMatrix(0.5f, 1000.0f, PLATFORM.GetFOVX() / (2.f), PLATFORM.GetFOVY()); //1.6f
		//m= ProjectionMatrix(0.5f, 1000.0f, PLATFORM.GetFOVX(), PLATFORM.GetFOVY());

    mDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m );
	
}

//******************************************************************************************
HRESULT SetViewMatrix( D3DMATRIX& mat, FVector& vFrom, FVector& vAt,
					   FVector& vWorldUp )
{
    FVector vView = vAt - vFrom;

    FLOAT fLength = vView.Magnitude();
    if( fLength < 1e-6f )
        return E_INVALIDARG;

    // Normalize the z basis vector
    vView /= fLength;

    FLOAT fDotProduct = vWorldUp * vView;

    FVector vUp = vWorldUp - vView * fDotProduct;

    // If this vector has near-zero length because the input specified a
    // bogus up vector, let's try a default up vector
    if( 1e-6f > ( fLength = vUp.Magnitude() ) )
    {
        vUp = FVector( 0.0f, 1.0f, 0.0f ) - vView * vView.Y;

        // If we still have near-zero length, resort to a different axis.
        if( 1e-6f > ( fLength = vUp.Magnitude() ) )
        {
            vUp = FVector( 0.0f, 0.0f, 1.0f ) - vView * vView.Z;

            if( 1e-6f > ( fLength = vUp.Magnitude() ) )
                return E_INVALIDARG;
        }
    }

    // Normalize the y basis vector
    vUp /= fLength;

    FVector vRight = vUp ^ vView;

/*	vView.Normalise();
	vWorldUp.Normalise();
	FVector vRight = -vView ^ vWorldUp;
	FVector vUp = vView ^ vRight;
*/    
    // Start building the matrix. The first three rows contains the basis
    // vectors used to rotate the view to point at the lookat point
    mat._11 = vRight.X;  mat._12 = vUp.X;  mat._13 = vView.X;  mat._14 = 0.0f;
    mat._21 = vRight.Y;  mat._22 = vUp.Y;  mat._23 = vView.Y;  mat._24 = 0.0f;
    mat._31 = vRight.Z;  mat._32 = vUp.Z;  mat._33 = vView.Z;  mat._34 = 0.0f;

    // Do the translation values (rotations are still about the eyepoint)
    mat._41 = - vFrom * vRight;
    mat._42 = - vFrom * vUp;
    mat._43 = - vFrom * vView;
    mat._44 = 1.0f;

    return S_OK;
}

//******************************************************************************************
void	CJCLEngine::FMatToD3DMat(FMatrix *f, D3DMATRIX *d)
{
	d->_11 = f->Row[0].X;
	d->_12 = f->Row[0].Y;
	d->_13 = f->Row[0].Z;
	d->_14 = 0;
	d->_21 = f->Row[1].X;
	d->_22 = f->Row[1].Y;
	d->_23 = f->Row[1].Z;
	d->_24 = 0;
	d->_31 = f->Row[2].X;
	d->_32 = f->Row[2].Y;
	d->_33 = f->Row[2].Z;
	d->_34 = 0;
	d->_41 = 0;
	d->_42 = 0;
	d->_43 = 0;
	d->_44 = 1.f;
}

//******************************************************************************************
void	CJCLEngine::SetViewMatrixFromCamera(D3DMATRIX &mat)
{
	// set D3D Matrix from camera orientation matrix
	GMatrix m = mCamera->mOrientation;

    mat._11 = GToF(m.Row[0].X);  mat._12 = GToF(m.Row[0].Y);  mat._13 = GToF(m.Row[0].Z);  mat._14 = 0.0f;
    mat._21 = GToF(m.Row[1].X);  mat._22 = GToF(m.Row[1].Y);  mat._23 = GToF(m.Row[1].Z);  mat._24 = 0.0f;
    mat._31 = GToF(m.Row[2].X);  mat._32 = GToF(m.Row[2].Y);  mat._33 = GToF(m.Row[2].Z);  mat._34 = 0.0f;
	
	m.TransposeInPlace();

    mat._41 = - GToF(mCamera->mPos * m.Row[0]);
    mat._42 = - GToF(mCamera->mPos * m.Row[1]);
    mat._43 = - GToF(mCamera->mPos * m.Row[2]);
    mat._44 = 1.0f;

}

//******************************************************************************************
void	CJCLEngine::SetupTransforms(FVector pos, FMatrix ori)
{
	// store the transformations
	mTransformPosition = pos;
	mTransformOrientation = ori;
	mTransformCount ++; // so we can tell that it's changed

	// World Matrix
    D3DMATRIX matWorld;
	matWorld._11 = ori.Row[0].X; matWorld._12 = ori.Row[1].X; matWorld._13 = ori.Row[2].X;
	matWorld._21 = ori.Row[0].Y; matWorld._22 = ori.Row[1].Y; matWorld._23 = ori.Row[2].Y;
	matWorld._31 = ori.Row[0].Z; matWorld._32 = ori.Row[1].Z; matWorld._33 = ori.Row[2].Z;
	matWorld._14 = matWorld._24 = matWorld._34 = 0.f;
	matWorld._41 = pos.X;
	matWorld._42 = pos.Y;
	matWorld._43 = pos.Z;
	matWorld._44 = 1.f;
    mDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	// View Matrix
	D3DMATRIX matView;
	
	FVector up = FVector(0, 0, -1.f);
//	FVector buggerme = FVector(-0.5f, 0.5f, 0.4f);
	FVector buggerme = ZERO_FVECTOR;

	FVector cam_pos, cam_at;
	GVectorToFVector(&mCamera->mPos, &cam_pos);
	GVectorToFVector(&mCamera->mLookAt, &cam_at);
	SetViewMatrix(matView, cam_pos + buggerme, cam_at + buggerme, up);

//	SetViewMatrixFromCamera(matView);

    mDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, &matView );
	
}
//******************************************************************************************
SINT	CJCLEngine::GetRenderGameTurn()
{
	return GAME.GetGameTurn();  //! change this for sub-turn interpolation...
}
//******************************************************************************************
DWORD	CJCLEngine::CalcLight(FVector v)
{
	DWORD col;

	float d = v.X;
	if (d >  1.f) d =  1.f;
	if (d < -1.f) d = -1.f;

	SINT r = ((d > -0.3f) ? SINT( (d + 0.3f) * 130) : 0) + 80;
	SINT b = ((d <  0.3f) ? SINT(-(d - 0.3f) * 130) : 0) ;
	SINT g = 100;
	
	col = (r << 16) + (g << 8) + b;

	return col;
}



//******************************************************************************************
void	CJCLEngine::RenderSky()
{
	PROFILE_FN(RenderSky);

#define SKY_LAT_VERTS		10
#define	SKY_LONG_VERTS		10
#define	SKY_DIST			100.f
#define	SKY_HEIGHT_OFFSET	(-1.f)

	FMatrix ori = ID_FMATRIX;
	FVector pos;
	GVectorToFVector(&mCamera->mPos, &pos);
	pos	-= FVector(0, 0, -30.f);

//	FVector pos = mCamera->mPos - FVector(0, .3f * SKY_DIST, 0);
//	FVector pos = FVector(32.f, -30.f, 32.f);

	SetupTransforms(pos, ori);

	CFVF_GTEX	vert;
	SINT		i[SKY_LAT_VERTS * SKY_LONG_VERTS];

	SINT	c0, c1, cv = 0;

	float	so = float(GetRenderGameTurn()) * 0.005f;

	for (c0 = 0; c0 < SKY_LAT_VERTS; c0 ++)
	{
		for (c1 = 0; c1 < SKY_LONG_VERTS; c1 ++)
		{
			vert.mPos.X = float(sin((c1 / float(SKY_LONG_VERTS)) * (2 * PI) + so)) * SKY_DIST * float(cos( float(c0) / float(SKY_LAT_VERTS) * PI / 2.f));
			vert.mPos.Y = float(cos((c1 / float(SKY_LONG_VERTS)) * (2 * PI) + so)) * SKY_DIST * float(cos( float(c0) / float(SKY_LAT_VERTS) * PI / 2.f));
//			vert.mPos.Z = -float(c0) * (SKY_DIST  / float(SKY_LAT_VERTS)) ;
			vert.mPos.Z = float(c0) * (SKY_DIST  / float(SKY_LAT_VERTS))  + SKY_HEIGHT_OFFSET;

			vert.mU = float(c1) / float(SKY_LONG_VERTS); // * 2.f;

			vert.mV = /*1.f - */(float(c0) / float(SKY_LAT_VERTS));

			vert.mDiffuse = 0xffffffff;

			i[cv] = mSkyTexture->AddVertex(&vert);

			cv ++;
		}
	}

	// and draw the polys
	//! AAAAAAAARRGH

	SINT	ind[6];

	for (c0 = 0; c0 < SKY_LAT_VERTS - 1; c0++)
	{
		for (c1 = 0; c1 < SKY_LONG_VERTS; c1++)
		{
			if (c1 != SKY_LONG_VERTS - 1)
			{
				ind[0] = ((c0    ) * SKY_LONG_VERTS) + c1;
				ind[1] = ((c0    ) * SKY_LONG_VERTS) + c1 + 1;
				ind[2] = ((c0 + 1) * SKY_LONG_VERTS) + c1;

				ind[3] = ((c0 + 1) * SKY_LONG_VERTS) + c1;
				ind[4] = ((c0    ) * SKY_LONG_VERTS) + c1 + 1;
				ind[5] = ((c0 + 1) * SKY_LONG_VERTS) + c1 + 1;
			}
			else
			{
				ind[0] = ((c0    ) * SKY_LONG_VERTS) + c1;
				ind[1] = ((c0    ) * SKY_LONG_VERTS) + c1 - SKY_LONG_VERTS + 1;
				ind[2] = ((c0 + 1) * SKY_LONG_VERTS) + c1;

				ind[3] = ((c0 + 1) * SKY_LONG_VERTS) + c1;
				ind[4] = ((c0    ) * SKY_LONG_VERTS) + c1 - SKY_LONG_VERTS + 1;
				ind[5] = ((c0 + 1) * SKY_LONG_VERTS) + c1 - SKY_LONG_VERTS + 1;
			}
			mSkyTexture->AddTri(i[ind[0]], i[ind[1]], i[ind[2]]);
			mSkyTexture->AddTri(i[ind[3]], i[ind[4]], i[ind[5]]);
		}
	}

	//mDevice->SetRenderState(D3DRENDERSTATE_WRAP0, D3DWRAPCOORD_0);

	//mSkyTexture->DrawAll();

	//mDevice->SetRenderState(D3DRENDERSTATE_WRAP0, 0);
}


#define SKYSIZE	(512.0f)
//******************************************************************************************
//#define SKYSIZE	(max(MAP_WIDTH, MAP_HEIGHT) * 1.2f)
void	CJCLEngine::RenderSkyCube()
{
	SINT	c0;
	
	SINT	pris;
	if (GAME.IsMultiPlayer())
		pris = CLevelStructure::GetPrisonForMultiplayerLevel(MULTIPLAYER_CAREER.mCurrentTournament, MULTIPLAYER_CAREER.mCurrentArena);
	else
		pris = CAREER.mCurrentPrison;

	BOOL	flip = (!((pris == 1) || (pris == 4)));

	for (c0 = 0; c0 < 5; c0 ++)
	{
		CTextureGTEX *tr;

		FVector x, y, o;

		switch (c0)
		{
		case 0:
			tr = flip ? mTexSkyUp : mTexSkyDown;
			x = FVector(SKYSIZE, 0, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(0, SKYSIZE, 0);
			break;
		case 1:
			tr = flip ? mTexSkyDown : mTexSkyUp;
			x = FVector(-SKYSIZE, 0, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(0, -SKYSIZE, 0);
			break;
		case 2:
			tr = flip ? mTexSkyRight : mTexSkyLeft;
			x = FVector(0, -SKYSIZE, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(SKYSIZE, 0, 0);
			break;
		case 3:
			tr = flip ? mTexSkyLeft : mTexSkyRight;
			x = FVector(0, SKYSIZE, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(-SKYSIZE, 0, 0);
			break;
		case 4:
			tr = mTexSkyCentre;
			x = FVector(-SKYSIZE, 0, 0);
			y = FVector(0, -SKYSIZE, 0);
			o = FVector(0, 0, -SKYSIZE);
			if (!flip)
			{
				x = -x;
				y = -y;
			}
			break;
		};
		
		if (!tr)
			return;
	
		
		FVector	p = GVectorToFVector(GetCamera()->mPos);
//		FMatrix ori = GMatrixToFMatrix(GetCamera()->mOrientation);
		
//		ENGINE.SetupTransforms(p, ori);
		ENGINE.SetupTransforms(p, ID_FMATRIX);

		FVector	verts[2][2];
		
		verts[0][0] = -x - y + o;
//		verts[0][0].X += p.X;
//		verts[0][0].Y += p.Y;

		verts[0][1] =  x - y + o;
//		verts[0][1].X += p.X;
//		verts[0][1].Y += p.Y;

		verts[1][0] =  x + y + o;
//		verts[1][0].X += p.X;
//		verts[1][0].Y += p.Y;

		verts[1][1] = -x + y + o;
//		verts[1][1].X += p.X;
//		verts[1][1].Y += p.Y;
		
		DWORD	col = 0xffffff;
		
		// work out how many segments we need
		CFVF_GTEX	vert;
		SINT	i[4];

		// old verts
		vert.mDiffuse = col;
		vert.mPos = verts[0][0];
		vert.mU = (c0 == 4)?0.0f:0.f;
		vert.mV = (c0 == 4)?1.0f:0.f;
		i[0] = tr->AddVertex(&vert);

		vert.mPos = verts[0][1];
		vert.mU = (c0 == 4)?1.0f:1.f;
		vert.mV = (c0 == 4)?1.0f:0.f;
		i[1] = tr->AddVertex(&vert);

		vert.mPos = verts[1][0];
		vert.mU = (c0 == 4)?1.0f:1.f;
		vert.mV = (c0 == 4)?0.0f:1.f;
		i[2] = tr->AddVertex(&vert);

		vert.mPos = verts[1][1];
		vert.mU = (c0 == 4)?0.0f:0.f;
		vert.mV = (c0 == 4)?0.0f:1.f;
		i[3] = tr->AddVertex(&vert);

		if (c0 < 4)
		{
			tr->AddTri(i[0], i[3], i[1]);
			tr->AddTri(i[3], i[2], i[1]);
		}
		else
		{	tr->AddTri(i[0], i[1], i[3]);
			tr->AddTri(i[3], i[1], i[2]);
		}
		tr->DrawAll();
	}
}



//******************************************************************************************

#define SKY_HEIGHT (50.f)
#define	SKY_SIZE (200.f)

inline	void	calc_water_uv(FVector &pos, float &u, float &v)
{
	FVector c;
	GVectorToFVector(&ENGINE.GetCamera()->mPos, &c);

	float	d =	pos.Z - c.Z;
	float	e = (pos - c).MagnitudeXY();

	if ((fabs(e) < 0.0001f) || (fabs(d) < 0.0001f))
	{
		// bastard case
		u = v = 0.f;
		return;
	}

	float	f = ((e / d) * SKY_HEIGHT) + e;

	u =  ((f * (pos - c).X) / e - float(GAME.GetGameTurn()) / 3.f) / SKY_SIZE ;
	v = -((f * (pos - c).Y) / e                                  ) / SKY_SIZE;
}

// ----------------------------------------------------------------------------
BOOL ClipToZ(float z,CFVF_GTEX *lp1,CFVF_GTEX *lp2)
{
	float du,dv,dx,dy,dz,incr;
	BOOL clippy;
	
	clippy=FALSE;
	
	if ((lp1->mPos.Z>=z)  && (lp2->mPos.Z>=z ))  return FALSE;
	if ((lp1->mPos.Z<z)  && (lp2->mPos.Z<z ))	return TRUE;
	
	dx=lp2->mPos.X - lp1->mPos.X;
	dy=lp2->mPos.Y - lp1->mPos.Y;
	dz=lp2->mPos.Z - lp1->mPos.Z;
	du=lp2->mU - lp1->mU;
	dv=lp2->mV - lp1->mV;
	
	if (dz==0) return FALSE;
	
	dz=1/dz;
	
	incr=z - lp1->mPos.Z;
	
	clippy=TRUE;
	
	if (lp1->mPos.Z>lp2->mPos.Z)
	{
		lp1->mPos.X = (dx * dz * incr) + lp1->mPos.X;
		lp1->mPos.Y = (dy * dz * incr) + lp1->mPos.Y;
		lp1->mU 	= (du * dz * incr) + lp1->mU;
		lp1->mV 	= (dv * dz * incr) + lp1->mV;
		lp1->mPos.Z = z;
	}
	else
	{
		lp2->mPos.X = (dx * dz * incr) + lp1->mPos.X; 
		lp2->mPos.Y = (dy * dz * incr) + lp1->mPos.Y;
		lp2->mU 	= (du * dz * incr) + lp1->mU;
		lp2->mV 	= (dv * dz * incr) + lp1->mV;
		lp2->mPos.Z = z;
	}
	return clippy;	  
}


//#define	WATER_COL	0x7f6f6fff;
#define	WATER_COL	0x101F1F44
//0x5f6f6fff;

#define	WATER_SPLITSY_SP 	 4
#define	WATER_SPLITY_SIZE_SP (0.1f)
#define	WATER_SPLITSX_SP 	 4
#define	WATER_SPLITX_SIZE_SP (0.25f)

#define	WATER_SPLITSY_MP 	 2
#define	WATER_SPLITY_SIZE_MP (0.2f)
#define	WATER_SPLITSX_MP 	 2
#define	WATER_SPLITX_SIZE_MP (0.5f)

SINT	InternalViewOffset = 320;
void	CJCLEngine::RenderWater()
{
	ENGINE.GetDevice()->SetRenderState(D3DRENDERSTATE_ZBIAS, 6);

	// JCL render the sky cube upside down... then bodge the z's
	float	water_height = GToF( GAME.GetLevelData()->mGVars[WATER_HEIGHT]);

	if (fabs(water_height) < 0.0001)
		return; // no water.

	if (ENGINE.GetCamera()->mPos.Z > water_height)
		return; // don't draw!

	CTexture::DrawAllTextures();		
	CTexture::DrawAllTexturesAdditive();

	SINT	WATER_SPLITSY;
	float	WATER_SPLITY_SIZE;
	SINT	WATER_SPLITSX;
	float	WATER_SPLITX_SIZE;
	
	if (GAME.IsMultiPlayer())
	{
		WATER_SPLITSY		= WATER_SPLITSY_MP;
		WATER_SPLITY_SIZE	= WATER_SPLITY_SIZE_MP;
		WATER_SPLITSX		= WATER_SPLITSX_MP;
		WATER_SPLITX_SIZE	= WATER_SPLITX_SIZE_MP;
	}
	else
	{
		WATER_SPLITSY		= WATER_SPLITSY_SP;
		WATER_SPLITY_SIZE	= WATER_SPLITY_SIZE_SP;
		WATER_SPLITSX		= WATER_SPLITSX_SP;
		WATER_SPLITX_SIZE	= WATER_SPLITX_SIZE_SP;
	}

	SINT	pris;
	if (GAME.IsMultiPlayer())
		pris = CLevelStructure::GetPrisonForMultiplayerLevel(MULTIPLAYER_CAREER.mCurrentTournament, MULTIPLAYER_CAREER.mCurrentArena);
	else
		pris = CAREER.mCurrentPrison;
	BOOL		rot = (!(pris == 7));
	
	ENGINE.EnableAdditiveAlpha();
	for (SINT c1 = 0; c1 < (WATER_SPLITSY * WATER_SPLITSX * 4) + 1; c1 ++)
	{
		SINT		spl = (c1 % (WATER_SPLITSY * WATER_SPLITSX));
		SINT		splitx = spl % WATER_SPLITSX;
		SINT		splity = spl / WATER_SPLITSX;
		
		CTextureGTEX *tr;
		FVector x, y, o;

		tr = NULL;

		switch (c1 / (WATER_SPLITSY * WATER_SPLITSX))
		{
		case 0:
			tr = rot?mTexSkyUp:mTexSkyDown;
			x = FVector(SKYSIZE, 0, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(0, SKYSIZE, 0);
			break;

		case 1:
			tr = rot?mTexSkyDown:mTexSkyUp;
			x = FVector(-SKYSIZE, 0, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(0, -SKYSIZE, 0);
			break;

		case 2:
			tr = rot?mTexSkyLeft:mTexSkyRight;
			x = FVector(0, -SKYSIZE, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(SKYSIZE, 0, 0);
			break;

		case 3:			
			tr = rot?mTexSkyRight:mTexSkyLeft;
			x = FVector(0, SKYSIZE, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(-SKYSIZE, 0, 0);
			break;

		case 4:
			tr = mTexSkyCentre;
			x = FVector(-SKYSIZE, 0, 0);
			y = FVector(0, SKYSIZE, 0);
			o = FVector(0, 0,  SKYSIZE);

			if (!rot)
			{
				x = -x; y = -y;
			}
			break;

		default:
			return;
		};
		
		if (!tr)
			return;
		
		x = -x;

		FVector	cpos = GVectorToFVector(GetCamera()->mPos);
		FMatrix ori = GMatrixToFMatrix(GetCamera()->mOrientation);
		ENGINE.SetupTransforms(cpos, ori);

//		FMatrix ori = ID_FMATRIX;
//		FVector pos = ZERO_FVECTOR;
//		SetupTransforms(pos, ori);

		FVector	WaterVerts[4];
		if (c1 == (WATER_SPLITSY * WATER_SPLITSX * 4))
		{		
			WaterVerts[0] = -x - y + o + cpos;
			WaterVerts[1] =  x - y + o + cpos; 
			WaterVerts[2] =  x + y + o + cpos;
			WaterVerts[3] = -x + y + o + cpos;
		}
		else
		{
			WaterVerts[0] = - x + (x * float(splitx    ) * WATER_SPLITX_SIZE * 2)
			                - y + (y * float(splity    ) * WATER_SPLITY_SIZE * 2)
			                + o + cpos;
			WaterVerts[1] = - x + (x * float(splitx + 1) * WATER_SPLITX_SIZE * 2)
						    - y + (y * float(splity    ) * WATER_SPLITY_SIZE * 2)
						    + o + cpos;
			WaterVerts[2] = - x + (x * float(splitx + 1) * WATER_SPLITX_SIZE * 2)
			  				- y + (y * float(splity + 1) * WATER_SPLITY_SIZE * 2)
			  				+ o + cpos;
			WaterVerts[3] = - x + (x * float(splitx    ) * WATER_SPLITX_SIZE * 2)
							- y + (y * float(splity + 1) * WATER_SPLITY_SIZE * 2)
							+ o + cpos;
		}

		FVector WorldPos = FVector(0.0f,0.0f,0.0f);
		FMatrix WorldOri = ID_FMATRIX;	
		ENGINE.SetupTransforms(WorldPos, WorldOri);

		SINT 	c0;
		BOOL	failed = FALSE;
		
		
		for (c0 = 0; c0 < 4; c0 ++)
		{
			
			FVector	wv  = WaterVerts[c0];
			
			FVector	d  = wv - cpos;
		
			float	l = d.Z;
			float	r = d.Z / (water_height - cpos.Z);
			
			if (r <= 0)
			{
				failed = TRUE;
				break;
			}
			
			FVector	nv = cpos + (d / r);
	
			WaterVerts[c0] = nv;
		}
		if (failed)
			continue;
		
		for (int ii = 4; ii--;)
		{
			WaterVerts[ii].Z = water_height;
		}
	
		float	u0, u1, v0, v1;
		if (c1 == (WATER_SPLITSY * WATER_SPLITSX * 4))
		{
			u0 = 1.0f;
			u1 = 0.0f;
			v0 = 1.0f;
			v1 = 0.0f;
		}
		else
		{
			u0 = 1.0f - (float(splitx    ) * WATER_SPLITX_SIZE);
			u1 = 1.0f - (float(splitx + 1) * WATER_SPLITX_SIZE);
			v1 = 1.0f - (float(splity    ) * WATER_SPLITY_SIZE);
			v0 = 1.0f - (float(splity + 1) * WATER_SPLITY_SIZE);
		}

		DWORD		col = 0x00333345;
		CFVF_GTEX	vert[4];
		SINT		i[4];

		vert[0].mDiffuse = col;
		vert[0].mPos = WaterVerts[0];
		vert[0].mU = u0;//(c0 == 4)?0.0f:0.f;
		vert[0].mV = v1;//(c0 == 4)?1.0f:0.f;

		vert[1].mDiffuse = col;
		vert[1].mPos = WaterVerts[1];
		vert[1].mU = u1;//(c0 == 4)?1.0f:1.f;
		vert[1].mV = v1;//(c0 == 4)?1.0f:0.f;

		vert[2].mDiffuse = col;
		vert[2].mPos = WaterVerts[2];
		vert[2].mU = u1;//(c0 == 4)?1.0f:1.f;
		vert[2].mV = v0;//(c0 == 4)?0.0f:1.f;
		
		vert[3].mDiffuse = col;
		vert[3].mPos = WaterVerts[3];
		vert[3].mU = u0;//(c0 == 4)?0.0f:0.f;
		vert[3].mV = v0;//(c0 == 4)?0.0f:1.f;
		
		// Draw the ****ing triangles. 
		i[0] = tr->AddVertex(&vert[0]);
		i[1] = tr->AddVertex(&vert[1]);
		i[2] = tr->AddVertex(&vert[2]);
		i[3] = tr->AddVertex(&vert[3]);

		tr->AddTri(i[0], i[3], i[1]);
		tr->AddTri(i[3], i[2], i[1]);

		tr->AddTri(i[0], i[1], i[3]);
		tr->AddTri(i[3], i[1], i[2]);

		tr->DrawAll();
	}

	CFVF_GTEX	vert;
	SINT	i[4];

	vert.mPos		= FVector(0, 0, water_height);
	calc_water_uv(vert.mPos, vert.mU, vert.mV);
	vert.mDiffuse	= WATER_COL;
	i[0] = mTexWater->AddVertex(&vert);

	vert.mPos		= FVector(float(MAP_WIDTH), 0, water_height);
	calc_water_uv(vert.mPos, vert.mU, vert.mV);
	vert.mDiffuse	= WATER_COL;
	i[1] = mTexWater->AddVertex(&vert);
	
	vert.mPos		= FVector(0, float(MAP_HEIGHT), water_height);
	calc_water_uv(vert.mPos, vert.mU, vert.mV);
	vert.mDiffuse	= WATER_COL;
	i[2] = mTexWater->AddVertex(&vert);
	
	vert.mPos		= FVector(float(MAP_WIDTH), float(MAP_HEIGHT), water_height);
	calc_water_uv(vert.mPos, vert.mU, vert.mV);
	vert.mDiffuse	= WATER_COL;
	i[3] = mTexWater->AddVertex(&vert);

	mTexWater->AddTri(i[0], i[2], i[1]);
	mTexWater->AddTri(i[1], i[2], i[3]);
	mTexWater->DrawAll();

	CTexture::DrawAllTextures();		
	CTexture::DrawAllTexturesAdditive();

	ENGINE.DisableAlpha();
}

#if are
void	CJCLEngine::RenderWater()
{
	//PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	float	v = GToF(GAME.GetLevelData()->mGVars[WATER_HEIGHT]);

	if (fabs(v) < 0.0001)
		return; // no water.

	if (GToF(ENGINE.GetCamera()->mPos.Z) > v)
		return; // underwater //! do something else!

	FMatrix ori = ID_FMATRIX;
	FVector pos = ZERO_FVECTOR;
	SetupTransforms(pos, ori);

	D3DTLVERTEX newVert;

	CFVF_GTEX	vert;
	SINT		i[4];

	vert.mPos		= FVector(0, 0, v);
	calc_water_uv(vert.mPos, vert.mU, vert.mV);
	vert.mDiffuse	= WATER_COL;
	i[0] = mTexWater->AddVertex(&vert);

	vert.mPos		= FVector(float(MAP_WIDTH), 0, v);
	calc_water_uv(vert.mPos, vert.mU, vert.mV);
	vert.mDiffuse	= WATER_COL;
	i[1] = mTexWater->AddVertex(&vert);
	
	vert.mPos		= FVector(0, float(MAP_HEIGHT), v);
	calc_water_uv(vert.mPos, vert.mU, vert.mV);
	vert.mDiffuse	= WATER_COL;
	i[2] = mTexWater->AddVertex(&vert);
	
	vert.mPos		= FVector(float(MAP_WIDTH), float(MAP_HEIGHT), v);
	calc_water_uv(vert.mPos, vert.mU, vert.mV);
	vert.mDiffuse	= WATER_COL;
	i[3] = mTexWater->AddVertex(&vert);

	mTexWater->AddTri(i[0], i[2], i[1]);
	mTexWater->AddTri(i[1], i[2], i[3]);
	

	//PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	//mTexWater->DrawAll();
	//PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//PLATFORM.GetD3DDevice()->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_CURRENT);


//	mDevice->SetRenderState(D3DRENDERSTATE_WRAPU, TRUE);

	//ENGINE.EnableMultiplicativeAlpha();
	//mTexWater->DrawAll();
	//ENGINE.DisableAlpha();

//	mDevice->SetRenderState(D3DRENDERSTATE_WRAPU, FALSE);
}
#endif

//******************************************************************************************
void	CJCLEngine::RenderCursor()
{
	if (CONTROLS.GetControllerMode() == CM_MOUSE)
	{
		// Render a square
		POINT	p = CONTROLS.GetMousePos();
		
		SINT	i[4];
			
		i[0] = mTexCursor->AddVertex(&(D3DTLVERTEX(D3DVECTOR(float(p.x      ), float(p.y      ), 0),1,D3DRGB(1,1,1),D3DRGB(0,0,0),0,0)));
		i[1] = mTexCursor->AddVertex(&(D3DTLVERTEX(D3DVECTOR(float(p.x + 16 ), float(p.y      ), 0),1,D3DRGB(1,1,1),D3DRGB(0,0,0),1,0)));
		i[2] = mTexCursor->AddVertex(&(D3DTLVERTEX(D3DVECTOR(float(p.x + 16 ), float(p.y + 16 ), 0),1,D3DRGB(1,1,1),D3DRGB(0,0,0),1,1)));
		i[3] = mTexCursor->AddVertex(&(D3DTLVERTEX(D3DVECTOR(float(p.x      ), float(p.y + 16 ), 0),1,D3DRGB(1,1,1),D3DRGB(0,0,0),0,1)));

		mTexCursor->AddTri(i[0], i[1], i[3]);
		mTexCursor->AddTri(i[1], i[2], i[3]);

		//mTexCursor->DrawAll();
	}
}

//******************************************************************************************
void	CJCLEngine::DrawCollisionSphere(float radius, SINT hit_flags)
{
	// this looks rather like a cuboid to me....
	DWORD	col;
	
	if (hit_flags & TF_HIT)
		col =  0xffff0000;
	else if (hit_flags & TF_HIT_CHECK)
		col = 0xff808080;
	else
		col = 0xff00ff80;

	FVector pos  = ZERO_FVECTOR;
	FVector axes = FVector(radius, radius, radius);
	FMatrix ori  = ID_FMATRIX;

	DrawDebugCuboid(col, axes, pos, ori);
}

//******************************************************************************************
void	CJCLEngine::DrawCollisionVolume(CCollisionVolume *cv, SINT hit_flags)
{
	if (!cv)
		return;

	while (cv)
	{
		DWORD	col;
		
		if (hit_flags & TF_HIT)
			col =  0xffff0000;
		else if (hit_flags & TF_HIT_CHECK)
		{
			if (hit_flags & TF_ON_GROUND)
				 col = 0xff808080;
			else col = 0xff0000ff;
		}
		else 
			col = 0xff00ff00;

		FVector pos  = GVectorToFVector(cv->mOrigin);
		FVector axes = GVectorToFVector(cv->mAxes);
		FMatrix ori  = GMatrixToFMatrix(cv->mOrientation);

		DrawDebugCuboid(col, axes, pos, ori);

		cv = cv->mNext;
	}
}

//******************************************************************************************
void	CJCLEngine::DrawDebugCuboid(DWORD col, FVector &axes, FVector &pos, FMatrix ori)
{
	// Draw polys representing cuboid.
	// ** ASSUMES local orientation already set up...

	FVector ax   = ori * FVector(axes.X, 0, 0);
	FVector ay   = ori * FVector(0, axes.Y, 0);
	FVector az   = ori * FVector(0, 0, axes.Z);


	SINT		c0;
	CFVF_GTEX	vert;

	for (c0 = 0; c0 < 6; c0++)
	{
		SINT		i[4];

		for (int c1 = 0;c1 < 4; c1 ++)
		{
			switch (c0)
			{
			case 0:	vert.mPos	=   ax                  + ((c1 & 1) ? ay : -ay) + ((c1 & 2) ? az : -az) ; break;
			case 1:	vert.mPos	= - ax                  + ((c1 & 1) ? ay : -ay) + ((c1 & 2) ? az : -az) ; break;
			case 2:	vert.mPos	= ((c1 & 1) ? ax : -ax) + ay                    + ((c1 & 2) ? az : -az) ; break;
			case 3:	vert.mPos	= ((c1 & 1) ? ax : -ax) - ay                    + ((c1 & 2) ? az : -az) ; break;
			case 4:	vert.mPos	= ((c1 & 1) ? ax : -ax) + ((c1 & 2) ? ay : -ay) + az		   			; break;
			case 5:	vert.mPos	= ((c1 & 1) ? ax : -ax) + ((c1 & 2) ? ay : -ay) - az		   			; break;
			};

			vert.mPos += pos;

			vert.mU	= (c1&1)?1.f:0.f;
			vert.mV	= (c1&2)?0.f:1.f;
			vert.mDiffuse = col;

			i[c1] = mTexOutline->AddVertex(&vert);
		}

		// double-sided...
		mTexOutline->AddTri(i[0], i[3], i[2]);
		mTexOutline->AddTri(i[0], i[1], i[3]);
		mTexOutline->AddTri(i[0], i[2], i[3]);
		mTexOutline->AddTri(i[0], i[3], i[1]);
	}

//	mTexOutline->DrawAll();  // unnecessary
}

//******************************************************************************************
void	CJCLEngine::RenderArrow(FVector v1, FVector v2, DWORD col)
{
#if defined(LINK_EDITOR)
	// draw arrow between 2 points

	FMatrix ori = ID_FMATRIX;
	FVector pos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);

	FVector l = v2 - v1;
	FVector s = l ^ FVector(0, 0, -1.f);
	s.Normalise();
	s *= 2.f;


	SINT		c1;
	CFVF_GTEX	vert;

	SINT		i[4];

	for (c1 = 0;c1 < 4; c1 ++)
	{
		vert.mPos = ((c1 & 1) ? ZERO_FVECTOR : l) + ((c1 & 2) ? s : -s);
		vert.mPos += v1 + FVector(0, 0, -0.5f);

		vert.mU	= (c1&2)?1.f:0.f;
		vert.mV	= (c1&1)?0.f:1.f;
		vert.mDiffuse = col;

		i[c1] = mTexEdArrow->AddVertex(&vert);
	}

		// double-sided...
	mTexEdArrow->AddTri(i[0], i[3], i[2]);
	mTexEdArrow->AddTri(i[0], i[1], i[3]);
#endif
}

//******************************************************************************************
#define	MAX_THINGS_TO_RENDER 2048

CThing	*rt_things[MAX_THINGS_TO_RENDER];

//******************************************************************************************
int		rtt_comp(const void *e1, const void *e2)
{
	CThing *t1 = *((CThing **)e1);
	CThing *t2 = *((CThing **)e2);

	float	v1 = GToF(t1->GetPos().Y + (t1->GetPos().Z /*/ 2.f*/));
	float	v2 = GToF(t2->GetPos().Y + (t2->GetPos().Z /*/ 2.f*/));

	if (v1 > v2)
		 return -1;
	else return  1;
}

//******************************************************************************************
#define GAMUT_SCALER (1.0f/(float)(1<<18))
void	CJCLEngine::RenderThings()
{
	PROFILE_FN(RenderThings);

	// render things
	//! GAMUT???
//	CThing *t = WORLD.GetFirstThing();
//	while (t)
//	{
//		t->Render();
//		t = t->GetNextThing();
//	};


	//! dodgy - sort things..

// for quick gamut rejection sorry about the native shifts and mval refs but...
// the '18's are 16 (for the gint) plus 2 (as in 1<<2==4) because of the 4x4 nature
// of the gamut

	GINT	xmin, xmax, ymin, ymax;
		
	xmin.mVal = m3DGamut->XMin<<18;
	xmax.mVal = m3DGamut->XMax<<18;
	ymin.mVal = m3DGamut->YMin<<18;
	ymax.mVal = m3DGamut->YMax<<18;


	SINT	n = 0;
	CThing *t = WORLD.GetFirstThing();
	while (t)
	{
		if (t->mFlags & TF_IN_MAPWHO)  //! well - this list should be created by scanning through the mapwhos...
		{
// a bunch of extra checks before rt_things[n++] = t;
			
			GINT irad=t->GetRenderRadius();

			if(irad.mVal >=(1<<18)) // big stuff
			{
				GVector p = t->GetRenderPos();
				p.X-=irad;
				p.Y-=irad;
				if((p.X>=xmin)&&(p.X<xmax)&&(p.Y>=ymin)&&(p.Y<ymax)) rt_things[n++] = t;
				else
				{
					p.X = irad*2;
					p.Y = irad*2;
					if((p.X>=xmin)&&(p.X<xmax)&&(p.Y>=ymin)&&(p.Y<ymax)) rt_things[n++] = t;
				}
			}
			else		 // everything else
			{
				float rad = (float)irad.mVal*GAMUT_SCALER;
				GVector p = t->GetRenderPos();
				if((p.X>=xmin)&&(p.X<xmax)&&(p.Y>=ymin)&&(p.Y<ymax))
				{
					float zz = (float)p.Z.mVal*GAMUT_SCALER;
					if(m3DGamut->NoClipInGamut3d( p.X.mVal>>18, p.Y.mVal>>18,zz-rad-1.0f,zz+rad+2.0f))
					{
						rt_things[n++] = t;
					}
				}
			}
		}
		t = t->GetNextThing();

		ASSERT(n < MAX_THINGS_TO_RENDER);
	};

	{
		PROFILE_FN(RenderQSort);
		qsort(rt_things, n, sizeof(CThing *), rtt_comp);
	}

	SINT	c0;

	for (c0 = 0; c0 < n; c0 ++)
	{
		rt_things[c0]->Render();
	}

#ifdef LINK_EDITOR
	CThing	*a, *b, *c, *d;
	a = EDITOR.GetSelectedThing(); // could support multiple things??
	if (a)
		a->RenderCollisionVolume(0);

	b = EDITOR.GetSelectedTargetThing();
	if (b)
		b->RenderCollisionVolume(TF_HIT_CHECK);

	if ((a) && (b))
		RenderArrow(GVectorToFVector(a->GetRenderPos()), GVectorToFVector(b->GetRenderPos()), 0xff4080ff);

	if (a)
	{
		d = a;
		while ((c = d->GetChainTrigger()) != NULL)
		{
			c->RenderCollisionVolume(TF_HIT);
			RenderArrow(GVectorToFVector(d->GetRenderPos()), GVectorToFVector(c->GetRenderPos()), 0xffff8040);
			d = c;
			if (d == a)
				break;  // loop
		}
	}

	if (b)
	{
		d = b;
		while ((c = d->GetChainTrigger()) != NULL)
		{
			c->RenderCollisionVolume(TF_HIT);
			RenderArrow(GVectorToFVector(d->GetRenderPos()), GVectorToFVector(c->GetRenderPos()), 0xffff8040);
			d = c;
			if (d == b)
				break; // loop
		}
	}
#endif
}

//******************************************************************************************
SINT	get_stadium_part(SINT i)
{
	return NM_STADIUM_WALL4 - ((i + 1) % 4);
}

SINT	get_stadium_part2(SINT i)
{
	return NM_STADIUM_WALL1;
}


void	CJCLEngine::RenderStadium()
{
	FVector p;
	FMatrix o;
	SINT	c0;
	float	pos;

	// bottom
	o = ID_FMATRIX;
	pos = 0;
	p = FVector(pos, 0, 0);
	CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[NM_STADIUM_WALL60]->mParts[0], NULL, WR_NONE);
	pos += 7.f;
	for (c0 = 0; c0 < MAP_WIDTH / 8 - 1; c0 ++)
	{
		p = FVector(pos, 0, 0);
		CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[get_stadium_part(c0)]->mParts[0], NULL, WR_NONE);
		pos += 8.f;
	}
	p = FVector(pos, 0, 0);
	CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[NM_STADIUM_CORNER]->mParts[0], NULL, WR_NONE);

	// top
	o.MakeRotationYawF(PI);
	pos = float(MAP_WIDTH - 1);
	p = FVector(pos, float(MAP_HEIGHT - 1), 0);
	CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[NM_STADIUM_WALL60]->mParts[0], NULL, WR_NONE);
	pos -= 7.f;
	for (c0 = 0; c0 < MAP_WIDTH / 8 - 1; c0 ++)
	{
		p = FVector(pos, float(MAP_HEIGHT - 1), 0);
		CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[get_stadium_part(c0)]->mParts[0], NULL, WR_NONE);
		pos -= 8.f;
	}
	p = FVector(pos, float(MAP_HEIGHT - 1), 0);
	CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[NM_STADIUM_CORNER]->mParts[0], NULL, WR_NONE);

	// right
	o.MakeRotationYawF(PI / 2.f);
	pos = 0;
	p = FVector(float(MAP_WIDTH - 1), pos, 0);
	CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[NM_STADIUM_WALL60]->mParts[0], NULL, WR_NONE);
	pos += 7.f;
	for (c0 = 0; c0 < MAP_HEIGHT / 8 - 1; c0 ++)
	{
		p = FVector(float(MAP_WIDTH - 1), pos, 0);
		CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[get_stadium_part(c0)]->mParts[0], NULL, WR_NONE);
		pos += 8.f;
	}
	p = FVector(float(MAP_WIDTH - 1), pos, 0);
	CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[NM_STADIUM_CORNER]->mParts[0], NULL, WR_NONE);

	// left
	o.MakeRotationYawF(PI * 1.5f);
	pos = float(MAP_HEIGHT - 1);
	p = FVector(0, pos, 0);
	CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[NM_STADIUM_WALL60]->mParts[0], NULL, WR_NONE);
	pos -= 7.f;
	for (c0 = 0; c0 < MAP_HEIGHT / 8 - 1; c0 ++)
	{
		p = FVector(0, pos, 0);
		CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[get_stadium_part(c0)]->mParts[0], NULL, WR_NONE);
		pos -= 8.f;
	}
	p = FVector(0, pos, 0);
	CRTMesh::RenderMeshPart(p, o, mNumberedMeshes[NM_STADIUM_CORNER]->mParts[0], NULL, WR_NONE);
}

//******************************************************************************************
void	CJCLEngine::Render(CCamera *cam1, CCamera *cam2)
{
	// should we render?
	if (	(!PLATFORM.GetDPWin())
		||	(!PLATFORM.GetDPWin()->GetPrimarySurface())
		||	(!PLATFORM.GetDPWin()->GetSurface())
		)
		return;

	// OK let's do it!

	CCamera	interpolated_cameras[2];
	if (cam1)
	{
		interpolated_cameras[0] = *cam1;
		interpolated_cameras[0].Interpolate();	
		mViewportCameras[0] = &interpolated_cameras[0];
	}
	else
		mViewportCameras[0] = NULL;

	if (cam2)
	{
		interpolated_cameras[1] = *cam2;
		interpolated_cameras[1].Interpolate();	
		mViewportCameras[1] = &interpolated_cameras[1];
	}
	else
		mViewportCameras[1] = NULL;

	mNumDisplays = (cam2 ? 2 : 1);


	// Get the Direct3D device
	LPDIRECT3DDEVICE7 device = PLATFORM.GetDPWin()->GetDevice();
	if (!device) return;

	// setup the globals
	mDevice = device;

	// Basic Init
	mFrameCount ++;
	mPolyCount = 0;
	mLastTexture = NULL;

	// Clear the device
	PLATFORM.GetDPWin()->Clear(0x00000000); // ####!!!!
	//	mSurface->Clear(0x000000ff);
	//!	mSurface->ClearZ(0x000000ff);
	
	SINT	split_number;

	for (split_number = 0; split_number < mNumDisplays; split_number ++)
	{
		mRenderingView = split_number;

		//!!!!!!!!!!!!!
		if (mNumDisplays == 2)
			mDevice->SetViewport(PLATFORM.GetDPWin()->DSGetViewport(split_number + 1)); //+1
		else
			mDevice->SetViewport(PLATFORM.GetDPWin()->DSGetViewport(0));


		mCamera = mViewportCameras[split_number];
		/*
		if (split_number == 0)
			mCamera = cam1;
		if (split_number == 1)
			mCamera = cam2;
		*/

		// calculate the gamut
		mGamut.SetSize(0, MAP_WIDTH, 0, MAP_HEIGHT);

		switch(mGamutType)
		{
		case GAMUT_NORMAL:
			mGamut.Calculate(mCamera, 1);
			break;
		case GAMUT_LOWERED:
			{
				// pretend the camera is higher than it is
				CCamera cam;
				cam.mPos = mCamera->mPos - GVector(G0, G0, MAP.GetLowestPoint());
				cam.mOrientation = mCamera->mOrientation;
				mGamut.Calculate(&cam , 1); 
			}
			break;
		case GAMUT_NONE: // don't do anything
			break;
		};

	// 3D gamut

		FMatrix	gamutmat;
		FVector gamutpos;
		GMatrixToFMatrix(&mCamera->mOrientation, &gamutmat);
		GVectorToFVector(&mCamera->mPos,&gamutpos);
		gamutpos /=4;			// to scale to 4*4 size

// note that the game will get faster as the 26 below decreases, 26*4 is
// 'just beyond the sky', 20*4 is a huge distance but sometimes you can see further
// flying in halfpipe levels
//		float howfar = 20;
//		float howfar = 16;
		float howfar = 32;  // JCL 19-10-00  (amongst other things near here..)
#ifdef LINK_EDITOR
		if(EDITOR.IsActive()) howfar=26;
#endif
		{
			PROFILE_FN(RenderGCGamut);
			m3DGamut->MakeIt(&gamutmat.Transpose(), &gamutpos, howfar, GAME.IsMultiPlayer() ? 0.354f : 0.707f, 0.6f);
		}
	// Start the scene
		if (device->BeginScene() == D3D_OK)
		{
			SetupInitialRenderstates();

			// - Disabled to see the SkyCube. AddFogEffect(0x00000000, D3DFOG_LINEAR, 56.0f, 60.0f, 0.0f, FALSE);

			// RenderSky(); JCL - will replace this with a skycube shortly
			RenderSkyCube();
			if (mAltMapDraw > 0)
			{
				RenderStadium();
				MAP.PCRender(m3DGamut);
			}
			else
				MAP.Render();

			RenderThings();							// these two calls swapped, is there a good
			RenderWater();							// reason why they were the other way round?

	#ifdef LINK_EDITOR
			if (EDITOR.IsActive())
				EDITOR.Render3D();
	#endif

			CTexture::DrawAllTextures();			// draw any leftover polys
			CTexture::DrawAllTexturesAdditive();	// and additive alphas go last...

	#ifdef LINK_EDITOR
			if (!(EDITOR.IsActive()))
	#endif
				RenderCursor();	// have to use different cursor for editor..

			{
				// All done with the scene
				PROFILE_FN(RenderEndScene);
				device->EndScene();
			}
		}
	}

	//!!!!!!!!!!!!!
	mDevice->SetViewport(PLATFORM.GetDPWin()->DSGetViewport(0));
	if (device->BeginScene() == D3D_OK)
	{
		SetupInitialRenderstates();
//		AddFogEffect(0x00FFFFFF, 3, 0.4f, 0.8f, 0.5f, FALSE);
		RenderGameState();
		GAMEPANEL.Render(); // I think

		CTexture::DrawAllTextures();			// draw any leftover polys
		CTexture::DrawAllTexturesAdditive();	// and additive alphas go last...

		device->EndScene();
	}

	Render2D();	// Maybe some 2D stuff?

#ifdef LINK_EDITOR
	if (EDITOR.IsActive())
		EDITOR.Render2D();

	if(GAME.GetGameRenderInfo()->mActive & GRI_CHARACTER_INFO)
	{
		CCharacter* c = GAME.GetCurrentCharacterInfo() ;
		if (c)
		{
			c->RenderCollisionVolume(0);
		}
	}
#endif

}

void	CJCLEngine::Flip()
{
	PROFILE_FN(RenderFlip);

	// The base functions flips the back buffer to the primary
	PLATFORM.GetApp()->Flip(*PLATFORM.GetBackSurface());
}


	/*******************************************************************************************\
	// This code shows how you can manually set the bits in a surface yourself. It works
	// for back buffers, primary surfaces and textures. I wouldn't try it on a z-buffer.
	BYTE image[128][128][3];
	for (int y=0; y<128; y++)
		for (int x=0; x<128; x++)
		{
			image[y][x][0] = 0;
			image[y][x][1] = x*2;
			image[y][x][2] = y*2;
		}
	surface->SetBits(128, 128, image);
	\*******************************************************************************************/

/*
*/


//******************************************************************************************
//** Convert screen xy to worldspace xyz.

GMatrix	CJCLEngine::GetCursorOffsetMatrix(SINT x, SINT y)
{
	GMatrix m	= ID_GMATRIX;
	SINT	sx	= PLATFORM.ScreenWidth();
	SINT	sy	= PLATFORM.ScreenHeight();

	if (x < 0)		x = 0;
	if (y < 0)		y = 0;
	if (x > sx - 1)	x = sx - 1;
	if (y > sy - 1)	y = sy - 1;

	x -= sx / 2;
	y -= sy / 2;

	GINT rx = G(x) / G(sx / 2) * FToG(PLATFORM.GetFOVX());
	GINT ry = G(y) / G(sy / 2) * FToG(PLATFORM.GetFOVY());

//	m.MakeRotation((rx >> 1), G0/*(rx >> 1)*/, G0 );
//	m.MakeRotation(G0, G0, (rx >> 1));
//	m.MakeRotation(G0, (ry >> 1), G0);
	m.MakeRotation(G0, (ry >> 1), (rx >> 1));

	return m;
}

//******************************************************************************************
void	CJCLEngine::DrawSplashScreen(SINT t)
{
//	mSurface = PLATFORM.GetBackSurface();
	mDevice = PLATFORM.GetDPWin()->GetDevice();

	if (PLATFORM.GetBackSurface()->GetDevice()->BeginScene() == D3D_OK)
	{
		SetupInitialRenderstates();

		SINT	i[4];

		ULONG	col  = 0x01010101;
		ULONG	spec = 0x01010101;

		if (t < 0)
		{
			col  = col * (-(t * 255) / 1000);
			spec = 0;
		}
		else if (t < 500)
		{
			col  = 0;
			spec = spec * ((t * 255) / 500);
		}
		else if (t < 1500)
		{
			col  = 0xffffffff;
			spec = spec * (((1500 - t) * 255) / 1000);
		}
		else 
		{
			col  = 0xffffffff;
			spec = 0;
		}

		i[0] = mTexSplash->AddVertex(&D3DTLVERTEX(D3DVECTOR(0  , 0  , 0),1, col, spec, 0.f, 1.f));
		i[1] = mTexSplash->AddVertex(&D3DTLVERTEX(D3DVECTOR(639, 0  , 0),1, col, spec, 1.f, 1.f));
		i[2] = mTexSplash->AddVertex(&D3DTLVERTEX(D3DVECTOR(639, 479, 0),1, col, spec, 1.f, 0.25f));
		i[3] = mTexSplash->AddVertex(&D3DTLVERTEX(D3DVECTOR(0  , 479, 0),1, col, spec, 0.f, 0.25f));

		mTexSplash->AddTri(i[0], i[1], i[3]);
		mTexSplash->AddTri(i[1], i[2], i[3]);

		CTexture::DrawAllTextures();		// draw any leftover polys
		PLATFORM.GetBackSurface()->GetDevice()->EndScene();
	}
}

//******************************************************************************************
void	CJCLEngine::DrawLoadingScreen()
{
	mDevice = PLATFORM.GetDPWin()->GetDevice();

	if (PLATFORM.GetBackSurface()->GetDevice()->BeginScene() == D3D_OK)
	{
		float h = PLATFORM.ScreenHeightScale();
		float w = PLATFORM.ScreenWidthScale();

		SetupInitialRenderstates();

		SINT	i[4];

		ULONG	col  = 0xffffffff;
		ULONG	spec = 0;
		
		i[3] = mTexLoading->AddVertex(&D3DTLVERTEX(D3DVECTOR(220 * w, 340 * h, 0),1, col, spec, 0.f, 0.f));
		i[2] = mTexLoading->AddVertex(&D3DTLVERTEX(D3DVECTOR(420 * w, 340 * h, 0),1, col, spec, 1.f, 0.f));
		i[1] = mTexLoading->AddVertex(&D3DTLVERTEX(D3DVECTOR(420 * w, 140 * h, 0),1, col, spec, 1.f, 1.f));
		i[0] = mTexLoading->AddVertex(&D3DTLVERTEX(D3DVECTOR(220 * w, 140 * h, 0),1, col, spec, 0.f, 1.f));
		
		mTexLoading->AddTri(i[0], i[1], i[3]);
		mTexLoading->AddTri(i[1], i[2], i[3]);

		CTexture::DrawAllTextures();		// draw any leftover polys
		PLATFORM.GetBackSurface()->GetDevice()->EndScene();
	}
	Flip();
}

//******************************************************************************************
void	CJCLEngine::ChangeGamut()
{
	switch (mGamutType)
	{
	case GAMUT_NORMAL:
		mGamutType = GAMUT_LOWERED;
		CONSOLE.AddString("Gamut mode set to 'Lowered'");
		break;
	case GAMUT_LOWERED:
		mGamutType = GAMUT_NONE;
		CONSOLE.AddString("Gamut disabled");
		break;
	case GAMUT_NONE:
		mGamutType = GAMUT_NORMAL;
		CONSOLE.AddString("Gamut mode set to 'Normal'");
		break;
	};
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// GLOBAL MESHES

//******************************************************************************************
SINT	CJCLEngine::GetGlobalMeshNumber(char *name)
{
	// do we have this already
	SINT	c0;

	for (c0 = 0; c0 < mNumGlobalMeshes; c0 ++)
	{
		if (mGlobalMeshes[c0])
			if (!(stricmp(name, mGlobalMeshes[c0]->mName)))
				return c0;
	}

	// nope - need to create a new resource
	if (mNumGlobalMeshes == MAX_GLOBAL_MESHES - 1)
	{
		// bugger - no space
		CONSOLE.AddString("No space for new mesh...");

		return -1;
	}

	mGlobalMeshes[mNumGlobalMeshes] = CMesh::GetMesh(name, "");

	if (!mGlobalMeshes[mNumGlobalMeshes])
	{
		CONSOLE.AddString("Couldn't load mesh...");
		return -1;
	}

	mNumGlobalMeshes ++;

	return mNumGlobalMeshes - 1;
}

//******************************************************************************************
void	CJCLEngine::AddRelocatedMesh(SINT num)
{
	if (num < 0)
		return;

	// do we have this already?
	SINT	c0;

	for (c0 = 0; c0 < mNumRelocatedMeshes; c0 ++)
		if (mGMRelocates[c0] == num)
			return;

	// nope - so add it.
	if (mNumRelocatedMeshes < MAX_GLOBAL_MESHES)
	{
		mGMRelocates[mNumRelocatedMeshes ++] = num;
	}
	else
		CONSOLE.AddString("Can't add relocated mesh...");
}

//******************************************************************************************
SINT	CJCLEngine::GetRelocatedMesh(SINT num)
{
	if ((num == -1) || (num == -2))
		return -1;

	if (mNumRelocatedMeshes == 0)
		return num;  // not currently relocating
	
	SINT	c0;

	for (c0 = 0; c0 < mNumRelocatedMeshes; c0 ++)
	{
		if (num == mGMRelocates[c0])
			return c0;
	}

	ASSERT(0);  // oops
	return -1;  // couldn't find it.
}

//******************************************************************************************
BOOL	CJCLEngine::Read(CMEMBUFFER &mem)
{
	SINT	c0;

	// get version number
	UINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson
	if ((ver == MKID("1.00")) || (ver == 0))  // cause dev studio fucked some levels up...
	{
		mem.Read(&mNumGlobalMeshes, 4);

		for (c0 = 0; c0 < mNumGlobalMeshes; c0 ++)
		{
			char name[256];

			mem.Read(name, 256);

			if (name[0] == 0)
				mGlobalMeshes[c0] = NULL;
			else
				mGlobalMeshes[c0] = CMesh::GetMesh(name, "");
		}

		return TRUE;
	}
	else
		return FALSE;
}

//******************************************************************************************
BOOL	CJCLEngine::Write(CMEMBUFFER &mem)
{
	UINT	ENG_VER = (MKID("1.00"));

	// write out version
	mem.Write(&ENG_VER, 4);

	// write out number of meshes
	mem.Write(&mNumRelocatedMeshes, 4);

	// and the mesh names

	SINT	c0;

	for (c0 = 0; c0 < mNumRelocatedMeshes; c0 ++)
	{
		char	name[256];
		SINT num = mGMRelocates[c0];
		if (num >= 0)
			strcpy(name, mGlobalMeshes[num]->mName);
		else
			strcpy(name, "");

		name[255] = 0; // safety
		mem.Write(name, 256);
	}

	return TRUE;
}

//******************************************************************************************

#ifdef LINK_EDITOR
//******************************************************************************************
void	CJCLEngine::WritePSXMeshReferences(CMEMBUFFER &mem, CMeshAccumulator &ma)
{
	// write out default mesh index, and indices for global meshes.

	SINT	i, c0;
	
	// default mesh
	i = ma.GetMeshIndex(GetDefaultMesh());
	mem.Write(&i, 4);

	// number of meshes
	mem.Write(&mNumRelocatedMeshes, 4);

	// and the global meshes
	for (c0 = 0; c0 < mNumRelocatedMeshes; c0 ++)
	{
		i = ma.GetMeshIndex(GetGlobalMesh(mGMRelocates[c0]));
		mem.Write(&i, 4);
	}
}
//******************************************************************************************
void	CJCLEngine::WriteDCMeshReferences(CMEMBUFFER &mem, CMeshAccumulatorDC &ma)
{
	// write out default mesh index, and indices for global meshes.
	SINT	i, c0;
	
	// default mesh
	i = ma.GetMeshIndex(GetDefaultMesh());
	mem.Write(&i, 4);

	// number of meshes
	mem.Write(&mNumRelocatedMeshes, 4);

	// and the global meshes
	for (c0 = 0; c0 < mNumRelocatedMeshes; c0 ++)
	{
		i = ma.GetMeshIndex(GetGlobalMesh(mGMRelocates[c0]));
		mem.Write(&i, 4);
	}
}


//******************************************************************************************

#endif

#endif
