#include	"Common.h"

#if TARGET == PSX

#include	"Globals.h"
#include	"PSXFEDemo3.h"

#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
#include	"Career.h"
#include	"PSXBuildType.h"

CPSXFEDemo3	FEDEMO3;

#define	TITLE_BAR_Y	10

//hi mum

//******************************************************************************************
BOOL	CPSXFEDemo3::LoadData()
{
	SOUND.AllocSampleBank("MOHOFE");

	UINT	CID_FEPS	= MKID("FEPS");
	UINT	CID_TPAG	= MKID("TPAG");
	UINT	CID_MESH	= MKID("MESH");

	CMEMBUFFER	mem;

	PLATFORM.ClearPools(); // free up global pools

	char	name[256];
	sprintf(name, "FRONTEND\\PRISEL.DAT");

	//TRACE("************************\n");
	//TRACE("About to load front end data\n");

	if (!(mem.InitFromFile(name)))
	{
		TRACE("File Load failed\n");

		mem.Close();
		return FALSE;
	}

	// check if file is valid
	UINT	chunk;
	mem.Read(&chunk, 4);

	if (chunk != CID_FEPS)
		return FALSE;

	// Read Texture page
	mem.Read(&chunk, 4);
	if (chunk != CID_TPAG)
		return FALSE;
	ENGINE.ReadPSXTextures(mem);

	// Read Meshes
	mem.Read(&chunk, 4);
	if (chunk != CID_MESH)
		return FALSE;
	ENGINE.ReadPSXMeshes(mem);

	mem.Close();

	PLATFORM.InitFrames(); // restore engine state
	SOUND.DeclareVolumeChange();  // update volumes

	printf("Level Pool - %d / %d  used\n", POOL->mLevelPoolSize, LEVEL_POOL_SIZE);

	return TRUE;
}

//******************************************************************************************
void	CPSXFEDemo3::Process()
{
	if (mQuit != 0)
		mQuit ++;

	GINT ym = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP);
	GINT xm = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT);


	if (ym != G0)
	{
		if (!mDBY)
		{
			if (ym > G0)
			{
				mLevel --;
				if (mLevel == -1)
					mLevel = 3;
			}
			else
			{
				mLevel ++;
				if (mLevel == 4)
					mLevel = 0;
			}
			mCounter = 0;

			SOUND.PlaySample(4);

			mDBY = 6;
		}
		else
			mDBY --;
	}
	else
		mDBY = FALSE;
		
	if (xm != G0)
	{
		if (!mDBX)
		{
			if (mLevel == 3)
			{
				if (xm > G0)
					CAREER.mForceFeedback = TRUE;
				else
					CAREER.mForceFeedback = FALSE;
				SOUND.PlaySample(4);
			}

			mDBX = 6;
		}
		else
			mDBX --;
	}
	else
		mDBX = FALSE;
		

	if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_X))
	{
		if (mLevel != 3)
		{
			mQuit = 1;
			SOUND.PlaySample(1);
		}
	}

#if DEMO_GAME == 3  
	if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE) ||
	    CONTROLS.IsButtonPressedDB(BUTTON_PSX_SELECT))
	{
		mQuit = 1;
		mLevel = -1;
		SOUND.PlaySample(1);
	}
#endif

	mCounter ++;
	
#if DEMO_GAME == 3  
	// timeout?
	if (mCounter == 25 * 60)
	{
		mQuit = 1;
		mLevel = -1;
	}
#endif
}
//******************************************************************************************
POLY_FT4	*CPSXFEDemo3::DrawSprite(SINT n, SINT px, SINT py, SINT otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey, GINT scale, SINT flip)
{
	POLY_FT4			*p4 = PLATFORM.GetMiscPolyFT4s();
	NTAG				*o  = PLATFORM.GetOrderTable();
	STextureRefOther 	*tr = ENGINE.GetNumberedTexture(n);

	setPolyFT4(p4);
	setXYWH(p4, px, py, (G(tr->mW) * scale).Whole(), (G(tr->mH) * scale).Whole() );
	setRGB0(p4, r, g, b);
	switch (semitrans)
	{
	case  1: p4->tpage = tr->mTPAGE_P1P1;	break;
	case  2: p4->tpage = tr->mTPAGE_P1M1;	break;
	case  3: p4->tpage = tr->mTPAGE_P1P025;	break;
	default:
	case  0: p4->tpage = tr->mTPAGE_P05P05;	break;
	};
    p4->clut  = tr->mCLUT;
    
    if (grey)
    	p4->clut -= 1 << 6;  //! hope this works

	SINT 	w = tr->mW;
	SINT	h = tr->mH;

	if (tr->mX + w == 256)
		w --;

	if (tr->mY + h == 256)
		h --;

	switch (flip)
	{
	case 0:
		setUVWH(p4, tr->mX    , tr->mY    ,  w,  h);
		break;
	case 1:
		setUVWH(p4, tr->mX + w, tr->mY    , -w,  h);
		break;
	case 2:
		setUVWH(p4, tr->mX    , tr->mY + h - 1,  w, -h + 1);
		break;
	case 3:
		setUVWH(p4, tr->mX + w, tr->mY + h - 1, -w, -h + 1);
		break;
	};		
	
	if (semitrans != -1)
		setSemiTrans(p4, TRUE);
	addPrim(&o[otdepth], p4);
	PLATFORM.DeclareUsedMiscPolyFT4s(p4 + 1);

	return p4;
}

//******************************************************************************************
void	CPSXFEDemo3::Render()
{
    int safe;
    do
    {
    	safe = PLATFORM.GeometryMarkBeginScene();
    } while (safe == -1);


	NTAG	*o = PLATFORM.GetOrderTable();
	
	if (mQuit == 0)
	{
		// Setup engine things
		PLATFORM.SetViewport(0);
	
		//************************
		// Background
	
		ENGINE.DrawSprite(FEPST_BACK, 0  , 0  , OT_LENGTH - 3, 127, 127, 127, FALSE, FALSE, G1, 0);
		ENGINE.DrawSprite(FEPST_BACK, 256, 0  , OT_LENGTH - 3, 127, 127, 127, FALSE, FALSE, G1, 1);
		ENGINE.DrawSprite(FEPST_BACK, 0  , 128, OT_LENGTH - 3, 127, 127, 127, FALSE, FALSE, G1, 2);
		ENGINE.DrawSprite(FEPST_BACK, 256, 128, OT_LENGTH - 3, 127, 127, 127, FALSE, FALSE, G1, 3);
	
		// draw title bar
		ENGINE.DrawSprite(FEPST_TITLEA, 0  , TITLE_BAR_Y, 2, 127, 127, 127, 2);
		ENGINE.DrawSprite(FEPST_TITLEB, 256, TITLE_BAR_Y, 2, 127, 127, 127, 2);
	
		char	text[200];
	
		CText::GetString(TEXT_MOHO_DEMO, text);
		SINT	sl = ENGINE.GetStringSize(0, text);
		ENGINE.DrawString(0, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25, 0x007f5f3f);
	
#if DEMO_GAME == 4
		CText::GetString(TEXT_DEMO_HELP2, text);
#else		
		CText::GetString(TEXT_DEMO_HELP1, text);
#endif
		sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawString(2, text, 256 - sl / 2, 220 , 0x007f7f7f);
	
		//**********************
		// Menu
		SINT	yps;
		GINT	gy = G1;
	
		SINT	c0;
		SINT	y = 80;
	
		for (c0 = 0; c0 < 5; c0 ++)
		{
			SINT 	c1;
			SINT	x = 112;
	
			GINT	gx;

			SINT	colr = 60, colg = 72, colb = 100;

			if (mLevel == c0 - 1)
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr / 4;
			}
			
			if (c0 == 0)
			{
				colr = 80;
				colg = 100;
				colb = 60;
			}

			DrawSprite(FEPST_BOXL, x    , y    , 2, colr, colg, colb, 2);
			x += 48;

			for (c1 = 0; c1 < 4; c1 ++)
			{
				DrawSprite(FEPST_BOXM, x    , y    , 2, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEPST_BOXR, x    , y    , 2, colr, colg, colb, 2);

			// draw text
			char	s[100];
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_ARENA_SELECT, s); break;
			case 1:	CText::GetString(TEXT_LNAME_031, s); break;
			case 2:	CText::GetString(TEXT_LNAME_005, s); break;
			case 3:	CText::GetString(TEXT_LNAME_003, s); break;
			case 4:	CText::GetString(TEXT_VIBRATION, s); break;
			};

			SINT	tcol;

			if ((mLevel == c0 - 1) || (c0 == 0))
				tcol = 0x007f7f7f;
			else
				tcol = 0x005f5f5f;

			if (c0 == 4)
			{
				ENGINE.DrawString(2, s, 160     , y + 4, tcol, 0);
				ENGINE.DrawString(2, s, 160  + 1, y + 5, 0, 0);
				
				DWORD	col;

				CText::GetString(TEXT_OFF, s);
				col = (!CAREER.mForceFeedback) ? 0x003f7f3f : 0x003f3f3f;
				ENGINE.DrawString(2, s, 290     , y + 4, col, 0);
				ENGINE.DrawString(2, s, 290  + 1, y + 5, 0, 0);

				CText::GetString(TEXT_ON, s);
				col = CAREER.mForceFeedback ? 0x003f7f3f : 0x003f3f3f;
				ENGINE.DrawString(2, s, 290 + 35     , y + 4, col, 0);
				ENGINE.DrawString(2, s, 290 + 35  + 1, y + 5, 0, 0);

				// draw arrows?
				if (mLevel == c0)
				{
	//				if ((mCounter & 31) < 20)
					{
						ENGINE.DrawSprite(FEPST_ARROW_L, 270 , y + 3, 0, 127, 127, 127, 2);
						ENGINE.DrawSprite(FEPST_ARROW_R, 370 , y + 3, 0, 127, 127, 127, 2);
					}
				}
			}
			else
			{
				SINT	sl = ENGINE.GetStringSize(0, s);
				ENGINE.DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 - 1, y + 5 - 1, tcol);
				ENGINE.DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2    , y + 5    , 0x001f0000);
			}

			y += 28;
		}
	}

	//************************
	// draw ordertable

	int f=PLATFORM.GetFrame();
	NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f));
	NTAG_Rlink(PLATFORM.GetOrderTable(),OT_LENGTH,(long*)&POOL->mNTAGTerms[f][0].firstprim
												,(long*)&POOL->mNTAGTerms[f][0].lastprim);


	PLATFORM.GeometryMarkEndScene();


}

//******************************************************************************************
SINT	CPSXFEDemo3::Run(BOOL load)
{
	// Load stuff
	
	if (load)			// is it all already here?
		if (!LoadData())
			return 0; // oops

	SOUND.DeclareVolumeChange();  // update volumes

	// misc engine setup
	mCamera.Reset(0);
	SPCamera=&mCamera;
	SPCameraGMatrix=SPCamera->mOrientation;
	ENGINE.GMatrixToMATRIX(&SPCameraGMatrix,&SPCameraMATRIX);
	PLATFORM.SetScreenWidth(512);
	int sx = PLATFORM.ScreenWidth()>>1;
	SetGeomOffset(sx, PLATFORM.ScreenHeight()>>1);
	OffScreenMask=0xff00fe00;

	mDBX  = FALSE;
	mDBY  = FALSE;

	mQuit = 0;
	mCounter = 0;
	mLevel = 0;

	//!!!!!!!***********  CHECK GAME DATA (eg water) *******************
	GAME.DeclareSinglePlayer();		// for the engine

	// and run!
	while (mQuit != 4)
	{
		CONTROLS.Update();
		SOUND.UpdateStatus();

		Process();
		Render();
	}

	// clear some global game stuff
	GAME.ClearGolfShots();
	GAME.ClearBestResult();
	
	CAREER.mCurrentCharacter = 0;
	switch (mLevel)
	{
		case 0:
			CAREER.mCurrentPrison	 = 7;		
			CAREER.mCurrentArena	 = 0;
			break;
		case 1:
			CAREER.mCurrentPrison	 = 0;		
			CAREER.mCurrentArena	 = 2;
			break;
		case 2:
			CAREER.mCurrentPrison	 = 3;		
			CAREER.mCurrentArena	 = 1;
			break;
	};
	
	SLevelInfo	*li = CLevelStructure::GetLevelInfo(CAREER.mCurrentPrison, CAREER.mCurrentArena);
	CAREER.mCurrentDiskLevel = li->mDiskLevel;

	GAME.DeclareSinglePlayer();
	
	if (mLevel == -1)
		return -1;
	else
		return mLevel;
}

#endif
