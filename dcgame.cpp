#include	"Common.h"

#if TARGET == DC
#include <sg_xpt.h>
#include	"Globals.h"
#include	"Profile.h"
#include 	"GamePanel.h"
#include	"Career.h"
#include	"Primary.h"
#include	"Text.h"

#include	"DCFEPrisonSelect.h"
#include	"DCFELanguageSelect.h"
#include	"DCFEDemo3.h"
#include	"DCFEDemo3End.h"
#include	"DCBuildType.h"
#include	"DCFMV.h"
#include 	"maincode.h"
#include 	"mw_util.h"

#include	"FrontEndCommon.h"

extern	int     InternalYClip;

//#pragma optimization_level 0

// If you define a level here, it will be run without enhuering the frontend...
//#define FORCE_LEVEL 5

//******************************************************************************************
void	CDCGame::ShowSplashScreen()
{
	//! nothing for now....
	
/*	PLATFORM.ClearScreen(0);
	PLATFORM.ClearScreen(1);
	PLATFORM.Flip(1);

	UBYTE *p = PLATFORM.GetLevelPoolData(512 * 384 * 2 + 1000); // +1000 for the header

	if (p)
		PSXLoadTimToVRAM("screens\\splash.tim", (ULONG *)p);

	PLATFORM.Flip(0);*/
	/*
	SDSChangeDir("\\mohodata\\mohodata\\screens");
	
	DCTimToScreen("splash.tim");	
	
  	for (int i=0;i<10;i++)
  	{
  	RenderStart();	  	
	GTQuad(0,0,639,0,0,479,639,479,1.0f);
	RenderFlip();		
	}
*/
}

//******************************************************************************************

char* loadnames[]=
{
	{"SCREENS\\ECO.TIM"},
	{"SCREENS\\1ZEPRIS.TIM"},
	{"SCREENS\\1HOSP.TIM"},
	{"SCREENS\\SPRISON1.TIM"},
	{"SCREENS\\REHAB.TIM"},
	{"SCREENS\\MINE.TIM"},
	{"SCREENS\\1DEATH.TIM"},
	{"SCREENS\\SPORTS.TIM"},
	{"SCREENS\\1MILI.TIM"},
	{"SCREENS\\1ALPHA.TIM"},
	{"SCREENS\\BONUS.TIM"}
};

char* loadnames_G[]=
{
	{"SCREENS\\GOLD\\ECO.TIM"},
	{"SCREENS\\GOLD\\1ZEPRIS.TIM"},
	{"SCREENS\\GOLD\\1HOSP.TIM"},
	{"SCREENS\\GOLD\\SPRISON1.TIM"},
	{"SCREENS\\GOLD\\REHAB.TIM"},
	{"SCREENS\\GOLD\\MINE.TIM"},
	{"SCREENS\\GOLD\\1DEATH.TIM"},
	{"SCREENS\\GOLD\\SPORTS.TIM"},
	{"SCREENS\\GOLD\\1MILI.TIM"},
	{"SCREENS\\GOLD\\1ALPHA.TIM"},
	{"SCREENS\\GOLD\\BONUS.TIM"}
};



BOOL CDCGame::ShowLoadingScreen(SINT num)
{
	//SDSSetClipRegion(0, 0, 640, InternalYClip);
	
	SDSSwitchClippingOn();		
	kmSetRegionClipping((0)>>5,InternalYClip,(640)>>5,(480)>>5);
	kmSetPixelClipping(0,0,640,InternalYClip);	

	PLATFORM.ClearPools();
	PLATFORM.InitFrames();

	// Clear Screen
	PLATFORM.GeometryMarkBeginScene();
	PLATFORM.GeometryMarkEndScene();


	UINT	CID_SCRN	= MKID("SCRN");
	UINT	CID_TPAG	= MKID("TPAG");
	UINT	CID_MESH	= MKID("MESH");

	CMEMBUFFER	mem;

	PLATFORM.ClearPools(); // free up global pools
	
    SDSFreeAllTextures();
	char	name[256];
	sprintf(name, "SCREENS\\%d.DAT", num);
	
	if (!(mem.InitFromFile(name)))
	{
		TRACE("File Load failed\n");

		mem.Close();
		return FALSE;
	}

	// check if file is valid
	UINT	chunk;
	mem.Read(&chunk, 4);

	if (chunk != CID_SCRN)
		return FALSE;

	// Read Texture page
	mem.Read(&chunk, 4);
	if (chunk != CID_TPAG)
		return FALSE;
	ENGINE.ReadPSXTextures(mem);

	mem.Close();

	PLATFORM.InitFrames(); // restore engine state

	// Now draw the screen!

//for (int draw=0;draw<2;draw++)
	{
	PLATFORM.GeometryMarkBeginScene();
	
	ENGINE.DrawSprite(SCREENT_MAIN1,   0,   0, 256, 256, 3);
	ENGINE.DrawSprite(SCREENT_MAIN2, 256,   0, 256, 256, 3);
	ENGINE.DrawSprite(SCREENT_MAIN3, 512,   0, 128, 256, 3);
	ENGINE.DrawSprite(SCREENT_MAIN4,   0, 256, 256, 256, 3);
	ENGINE.DrawSprite(SCREENT_MAIN5, 256, 256, 256, 256, 3);
	ENGINE.DrawSprite(SCREENT_MAIN6, 512, 256, 128, 256, 3);	

	char s[200];
	SINT	sl1, sl2;
	
	if (ENGINE.GetNumFonts() > 0)
	{
		if ((num > 3) && (num < 26))
		{
			ELILevelType lt;

			if (mMultiplayer)
			{
				lt = CLevelStructure::GetTournamentInfo(MULTIPLAYER_CAREER.mCurrentTournament)->mLevels[MULTIPLAYER_CAREER.mCurrentArena].mType;
			}
			else
			{
				lt = CLevelStructure::GetLevelInfo(CAREER.mCurrentPrison, CAREER.mCurrentArena)->mType;
			}

			SINT	t1 = -1, t2 = -1, t3 = -1, t4 = -1;

			switch (lt)
			{
			case LILT_KING_OF_THE_HILL:
				t1 = TEXT_LT_KING_OF_THE_HILL;
				t2 = TEXT_NOTES_KING_A;
				t3 = TEXT_NOTES_KING_B;
				break;
			case LILT_LAST_MAN_ROLLING:
				t1 = TEXT_LT_LAST_MAN_ROLLING;
				t2 = TEXT_NOTES_LAST_MAN_A;
				t3 = TEXT_NOTES_LAST_MAN_B;
				break;
			case LILT_RUN_THE_GAUNTLET:
				t1 = TEXT_LT_RUN_THE_GAUNTLET;
				t2 = TEXT_NOTES_GAUNTLET_A;
				t3 = TEXT_NOTES_GAUNTLET_B;
				break;
			case LILT_PURSUIT:
				t1 = TEXT_LT_PURSUIT;
				t2 = TEXT_NOTES_PURSUIT_A;
				t3 = TEXT_NOTES_PURSUIT_B;
				break;
			case LILT_RACE:
				t1 = TEXT_LT_RACE;
				t2 = TEXT_NOTES_RACE_A;
				t3 = TEXT_NOTES_RACE_B;
				break;
			case LILT_TRICK:
				t1 = TEXT_LT_TRICK;
				t2 = TEXT_NOTES_TRICK_A;
				t3 = TEXT_NOTES_TRICK_B;
				break;
			case LILT_POWERBALL:
				t1 = TEXT_LT_POWERBALL;
				t2 = TEXT_NOTES_BALL_A;
				t3 = TEXT_NOTES_BALL_B;
				break;
			case LILT_GOLF:
				t1 = TEXT_LT_GOLF;
				t2 = TEXT_NOTES_GOLF_A;
				t3 = TEXT_NOTES_GOLF_B;
				t4 = TEXT_HELPDC_GOLF;
				break;
			case LILT_SBALL:
				t1 = TEXT_LT_SBALL;
				t2 = TEXT_NOTES_SBALL_A;
				t3 = TEXT_NOTES_SBALL_B;
				t4 = TEXT_HELP_BEAM;
				break;
			};

			SINT 	y1, y2, y3, y4;

			CText::GetString(t3, s);

			if ((s[0] == '.') && (t4 == -1))
			{
				y1 = 190 * DCMULTY;
				y2 = 206 * DCMULTY;
			}
			else if (s[0] == '.')
			{
				y1 = 174 * DCMULTY;
				y2 = 190 * DCMULTY;
				y4 = 206 * DCMULTY;
			}
			else if (t4 == -1)
			{
				y1 = 174 * DCMULTY;
				y2 = 190 * DCMULTY;
				y3 = 206 * DCMULTY;
			}
			else
			{
				y1 = 158 * DCMULTY;
				y2 = 174 * DCMULTY;
				y3 = 190 * DCMULTY;
				y4 = 206 * DCMULTY;
			}


			CText::GetString(TEXT_LEVEL_TYPE, s);
			sl1 = ENGINE.GetStringSize(0, s);
			ENGINE.DrawString(0, s, 20 * DCMULTX    , y1    , 0x001f4f7f,1,TRUE);

			CText::GetString(TEXT_OBJECTIVE, s);
			sl2 = ENGINE.GetStringSize(0, s);
			ENGINE.DrawString(0, s, 20 * DCMULTX    , y2    , 0x001f4f7f,1,TRUE);

			float	ty;
			if (sl1 > sl2)
				ty = sl1 + (30 * DCMULTX);
			else
				ty = sl2 + (30 * DCMULTX);

			CText::GetString(t1, s);
			ENGINE.DrawString(0, s, ty    , y1    , 0x007f5f1f,1,TRUE);

			CText::GetString(t2, s);
			ENGINE.DrawString(2, s, ty    , y2    , 0x001f7f3f,1,TRUE);

			CText::GetString(t3, s);
			if (s[0] != '.')
			{
				ENGINE.DrawString(2, s, ty    , y3    , 0x001f7f3f,1,TRUE);
			}

			if (t4 != -1)
			{
				CText::GetString(t4, s);
				ENGINE.DrawString(2, s, ty    , y4    , 0x007f7f7f,1,TRUE);
			}

			CText::GetString(TEXT_PRN_ECO + ((num - 4) % 11), s);
			SINT	sl = ENGINE.GetStringSize(0, s);
			ENGINE.DrawString(0, s, (PLATFORM.ScreenWidthFull() / 2) - (sl / 2)    , 20 * DCMULTY   , 0x007f7f7f,1,TRUE);

			if (mMultiplayer)
			{
				STournamentInfo	*ti = CLevelStructure::GetTournamentInfo(MULTIPLAYER_CAREER.mCurrentTournament);
				CText::GetString(ti->mLevels[MULTIPLAYER_CAREER.mCurrentArena].mName, s);
			}
			else
			{
				SLevelInfo		*li = CLevelStructure::GetLevelInfo(CAREER.mCurrentPrison, CAREER.mCurrentArena);
				CText::GetString(li->mName, s);
			}
			sl = ENGINE.GetStringSize(0, s);
			ENGINE.DrawString(0, s, (PLATFORM.ScreenWidthFull() / 2) - (sl / 2)    , 36 * DCMULTY    , 0x007f5f2f,1,TRUE);
		}
	}
	//if (draw == 0) // Draw some text on the first screen
	if (num < 26)
	{
		CText::GetString(TEXT_LOADING, s);
		SINT	sl = ENGINE.GetStringSize(0, s);
		ENGINE.DrawString(0, s, (PLATFORM.ScreenWidthFull() / 2) - (sl / 2)    , 222 * DCMULTY    , 0x007f7f7f,1,TRUE);
	}

	PLATFORM.GeometryMarkEndScene();

	
	}

	return TRUE;
}

//******************************************************************************************
void	show_release_notes()
{
#ifdef JCLDCNOPSX			// Rewrite for DC - probably not needed, actually
	{
		CDCMemBuffer mem;

		mem.InitFromFile("SCREENS\\RELEASE.TIM");
		RECT	r = {512, 0, 512, 1};
		mem.Skip(20);                   // Skip TIM header...
		long templine[256];
		for(int c0=0;c0<256;c0++)
		{
			mem.Read((char*)templine,1024);
			//while(DrawSync(1)) /*Do Nothing!*/;
			LoadImage(&r,(ULONG*)templine);
			r.y++;
		}
	}
	PLATFORM.ClearPools(); // free up global pools
	PLATFORM.SetViewport(0);
	PLATFORM.InitFrames();

	for(int draws=0;draws<2;draws++)
	{
		int safe;
    	do
    	{
    		safe = PLATFORM.GeometryMarkBeginScene();
    	} while (safe == -1);

		NTAG	*o = PLATFORM.GetOrderTable();
		PLATFORM.SetViewport(0);

		POLY_FT4 	*p4 = PLATFORM.GetMiscPolyFT4s();

		setPolyFT4(p4);
		setRGB0(p4, 127, 127, 127);
		setXYWH(p4, 0, 0, 256, 256);
		setUVWH(p4, 0, 0, 255, 255);
		setTPage(p4, 2, 0, 512, 0);
		addPrim(&o[1], p4);
		p4 ++;

		setPolyFT4(p4);
		setRGB0(p4, 127, 127, 127);
		setXYWH(p4, 256, 0, 256, 256);
		setUVWH(p4, 0, 0, 255, 255);
		setTPage(p4, 2, 0, 768, 0);
		addPrim(&o[1], p4);
		p4 ++;

		PLATFORM.DeclareUsedMiscPolyFT4s(p4);

		int 	f = PLATFORM.GetFrame();
		//NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f + 2));
		NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f));
		NTAG_Rlink(PLATFORM.GetOrderTable(),OT_LENGTH,(long*)&POOL->mNTAGTerms[f][0].firstprim
													,(long*)&POOL->mNTAGTerms[f][0].lastprim);
		PLATFORM.GeometryMarkEndScene();
	}

	while (!(CONTROLS.IsButtonPressed(BUTTON_PSX_X)))
	{
		CONTROLS.Update();
	};
#endif
}

//******************************************************************************************
BOOL	CDCGame::HasMemoryCardBeenRemoved()
{
//	return (mMemoryCardRemoved != -1);
	if (CONTROLS.mControllers[0].GetPlaybackState() != ACTIVE)
		return (mMemoryCardRemoved > 10);
	else
		return FALSE;
}

//******************************************************************************************
//** Main game loop

#define			FRAME_TIME	0.04f

GINT	CDCGame::GetFrameRenderFraction(void)
{
	return FToG((mFrameTime - mBaseTime) / FRAME_TIME);
}

extern __declspec(data) volatile int	vbltick;
extern __declspec(data) volatile int	frames_to_screen_update;

//#include "PSXSmallParticle.h"
//CPSXSmallParticle ptcl;
EGQuitType	CDCGame::RunLevel(SINT level_no, SINT demo_number)
{
    if (!GAME.LoadLevel(level_no))
    {
//    	printf("Game Load Failed\n");
    	return GQ_FRONTEND;
    }

    //!DEMOTEST
#ifdef RECORD_GAME
	static int	wibble = 1;
    CONTROLS.mControllers[0].StartRecording(wibble ++);
#else
	if (demo_number != -1)
	{
		SOUND.StopTrack();                  // Reinit for double speed loading if I was playing CDA
	    CONTROLS.mControllers[0].StartPlayBack(demo_number);
		SOUND.PlayTrack(((CAREER.mCurrentPrison + 1) % 2), 2);               // Play from track 0, repeating all...
	}
#endif

	GAMEPANEL.DeActivateNow();



	PLATFORM.InitFrames();
	
	MAP.InitMappy();	
	
	mBaseTime = GetSysTimeFloat(); 
	
	mMemoryCardRemoved = -1;
	
	int drawme=0;
	while (1)
	{
		while (mQuit == GQ_NONE)
		{
			BOOL	memcard_ok = TRUE;
			
			// Check if memory card has been removed...
			if (CAREER.mMemController >= 0)
			{
				SINT	controller;
				switch(CAREER.mMemController)
				{
				case 0: controller =  PDD_PORT_A0; break;
				case 1: controller =  PDD_PORT_B0; break;
				case 2: controller =  PDD_PORT_C0; break;
				case 3: controller =  PDD_PORT_D0; break;
				}

				ControllerState cs = SDSCheckController(controller);
				
				SINT	mcs = MCCheck(CAREER.mMemController * 2 + CAREER.mMemSlot);
				
				if (mcs != VMU_EXISTS_FORMATTED)
				{
					if ((CAREER.mMemController != 0) ||
						((cs != Conrtoller_NOT_THERE) &&
						 (cs != Controller_EXISTS_UNKNOWN)))
					{
						if ((!(GAME.IsMultiPlayer())) ||
							(CAREER.mMemController != 1) ||
							((cs != Conrtoller_NOT_THERE) &&
							 (cs != Controller_EXISTS_UNKNOWN)))
						{
							memcard_ok = FALSE;
						}					
					}
				}
			}
			
			if (memcard_ok)
				mMemoryCardRemoved = -1;
			else
				mMemoryCardRemoved ++;
		
			if (mMemoryCardRemoved > 40)
			{
				if (CONTROLS.IsButtonPressed(BUTTON_DC_A, 0))
				{
					CAREER.mMemController = -1;
					CAREER.mMemSlot = -1;
					SOUND.PlaySample(SOUND_SELECT, G(0, 55000));
				}
			}
		
			float	start_time = GetSysTimeFloat();

//			if ((GetGameTurn() & 0x3f == 1))
			{
				SINT	c0;
				for (c0 = 0; c0 < 4; c0 ++)
				{
					if (MCCheck(c0 * 2) == VMU_EXISTS_FORMATTED)
					{
			#ifdef JCLDCNOPSX  // ball breakers
			#endif
						SINT icon = 6;
						
						if (GAME.IsMultiPlayer())
						{
							if (c0 == 0)
								icon = MULTIPLAYER_CAREER.mWhichPlayer[0];
							if (c0 == 1)
								icon = MULTIPLAYER_CAREER.mWhichPlayer[1];
						}
						else
							if (c0 == 0)
								icon = CAREER.mCurrentCharacter;					
					
						SDSUpLoadLcd(c0 * 6 + 1, icon);
					}
				}
			}
		
#if ENABLE_PROFILER==1
			CProfiler::ResetAll();
#endif
			if (GAME.GetGameState()==GAME_STATE_FADE_OUT)
				drawme=1;

			{
				PROFILE_FN(TotalModel);
				mFrameTime = mBaseTime;		// some things use GetRender*() in game code....

				// DC Reset
				if ((CONTROLS.IsButtonPressedDB(BUTTON_DC_RESET, 0)) ||
					(CONTROLS.IsButtonPressedDB(BUTTON_DC_RESET, 1)))
					GAME.SetQuitFlag(GQ_DC_RESET);
				
				PLATFORM.ProcessMessages();	// handle windows messages
				if (!HasMemoryCardBeenRemoved())
					GAMEPANEL.Process();		// process panel   ( needs to be before CONTROLS.Process() so it gets processed a turn later)
				CONTROLS.Process();			// handle controls

				if (mQuit != GQ_NONE)
					break;
				SOUND.UpdateStatus();
				// handle game
				if (!HasMemoryCardBeenRemoved())
				{
					if (mGameTurn == -1) // run a few turns so everything's initialised properly.
					{
						mGameTurn = 0;  // so that this doesn't keep happening in the editor...
						ProcessGame();
						ProcessGame();
					}
					ProcessGame();
				}
			}
			float	after_model_time = GetSysTimeFloat();
			if (after_model_time < start_time)
			{
				// oh dear - timer wrapped)
				after_model_time = start_time + 0.01f;  // approximate model time...
			}
			
			
			{
				PROFILE_FN(TotalEngine);
				SINT	num_renders = 0;
				do
				{
					if (GetSysTimeFloat() < mBaseTime)
					{
						// oh dear - timer wrapped..
						mBaseTime = GetSysTimeFloat() - (after_model_time - start_time);
					}
				
					if (!mGamePanelActive)
						mFrameTime = GetSysTimeFloat() - (after_model_time - start_time);
					else
						mFrameTime = mBaseTime;
						
					if (mQuit == GQ_NONE)
					{
						//if(drawme)
		    			//	PLATFORM.GeometryMarkEndScene();
						if (!(mPlayer[1].IsActive()))
							ENGINE.Render(&mCameras[0], NULL);
						else
						{
							ENGINE.Render(&mCameras[0],&mCameras[1]);
						}
					}
					num_renders ++;
					
					// ********************************************************************
					// Simple FPS
					// ********************************************************************
					//float et=(GetSysTimeFloat()-st)*38400;
					//GQuad(0,0,et,0,0,20,et,20);
					
				}
				
				while ((GetSysTimeFloat() - mBaseTime) < FRAME_TIME);
				
				// now adjust the base time
				if ((num_renders == 1) && ((GetSysTimeFloat() - start_time) > FRAME_TIME))
				{
					PROFILE_FN(RenderResetInt);
//					mBaseTime = GetSysTimeFloat(); 	// running slowly...
					mBaseTime += (GetSysTimeFloat() - start_time); 	// running slowly...
					if ((GetSysTimeFloat() - start_time) > FRAME_TIME * 1.3f)
						mBaseTime = GetSysTimeFloat(); // running *very* slowly...
				}
				else
					mBaseTime += FRAME_TIME;
					
				drawme=1;
			}
			
			// DOOR OPEN STUFF 
			//mQuit=GQ_DC_EXIT;
			
		}

		if (mQuit == GQ_DC_RESET)
			break;

		if (mQuit == GQ_DC_EXIT)
			break;

		if (mQuit == GQ_FRONTEND_WIN)
		{
			mQuit = GQ_FRONTEND_WIN;

			// fill out return structure
			CPrimary *pr = PLAYER(0)->GetPrimary();
			if (!pr)
			{
//				printf("Help! Couldn't find primary...\n");
			}
			else
			{
				if (GetIVar(IV_LEVEL_TYPE) == LT_GOLF)
				{
					mGolfTotalShotNo[0] += mGolfShotNo[0];
					CAREER.DeclarePendingVictory(mGolfTotalShotNo[0]);
				}
				else
				{
					CCharacterLevelData* cld = &pr->GetLevelData() ;
					SINT	time = cld->GetCompletedLevelTime();

					// did we beat this time in a previous attempt?
					if (mBestResult != -1)
					{
						if (mBestResult < time)
							time = mBestResult;
					}

					CAREER.DeclarePendingVictory(time);
				}
			}
			break;
		}

		if ((mQuit == GQ_FRONTEND) || (mQuit == GQ_QUIT))
		{
			SINT 	time = -1;

			// did we actually manage to win?
			if (!mMultiplayer)
			{
				ELevelResult res = PLAYER(0)->GetLevelResult();
				if (res == LR_WIN)
				{
					// yup!
					CPrimary *pr = PLAYER(0)->GetPrimary();

					if (pr)
					{
						CCharacterLevelData* cld = &pr->GetLevelData() ;
						time = cld->GetCompletedLevelTime();
					}
				}
			}

			if (mBestResult != -1) // if we finished in a previous attempt
			{
				if (time == -1)
					time = mBestResult;
				else
				{
					if (mBestResult < time)
						time = mBestResult;
					else
						mBestResult = time;
				}
			}

			if (time != -1)
			{
				CAREER.DeclarePendingVictory(time);
				mQuit = GQ_FRONTEND_WIN;
			}

			break;
		}

		if (mQuit == GQ_MULTIPLAYER_FINISHED)
		{
			// tell the career what happened.
			ELevelResult res0 = PLAYER(0)->GetLevelResult();
			ELevelResult res1 = PLAYER(1)->GetLevelResult();

			if (res0 == LR_WIN)
			{
				MULTIPLAYER_CAREER.mWhoWonWhichLevel[MULTIPLAYER_CAREER.mCurrentArena] = 0;
			}
			else if (res1 == LR_WIN)
			{
				MULTIPLAYER_CAREER.mWhoWonWhichLevel[MULTIPLAYER_CAREER.mCurrentArena] = 1;
			}
			else
			{
				// assume draw for now...
				MULTIPLAYER_CAREER.mWhoWonWhichLevel[MULTIPLAYER_CAREER.mCurrentArena] = -1;
			}
			break;
		}

		if (mQuit == GQ_QUIT)
			break;

		switch(mQuit)
		{
			case GQ_RESTART:
				// did we actually manage to win?
				if (!mMultiplayer)
				{
					ELevelResult res = PLAYER(0)->GetLevelResult();
					if (res == LR_WIN)
					{
						// yup!
						CPrimary *pr = PLAYER(0)->GetPrimary();

						if (pr)
						{
							CCharacterLevelData* cld = &pr->GetLevelData() ;
							SINT	time = cld->GetCompletedLevelTime();

							if ((mBestResult == -1) || (time < mBestResult))
								mBestResult = time; // store for later
						}
					}
				}

				CAREER.DeclareLevelAttempted();
				RestartLevel();
				mBaseTime = GetSysTimeFloat(); 
				break;

		}
	}

#ifdef RECORD_GAME
	// stop recording?
	if (CONTROLS.mControllers[0].GetRecordState() == ACTIVE)
		CONTROLS.mControllers[0].StopRecording();
#endif
	
	// stop playback just in case!
	if (CONTROLS.mControllers[0].GetPlaybackState() == ACTIVE)
		CONTROLS.mControllers[0].StopPlayBack();


	SOUND.KillAllSamples();
	WORLD.Shutdown();
	StopVibration();                // Make sure actuators stop...
	
	//** THING debug
	extern 	ULONG	lowest_thing_allocated;
	static	SINT	lta_val = 0x7fffffff;
	static	SINT	lta_level = -1;

	SINT	foo = SINT(lowest_thing_allocated - 0x70000000) - SINT(ULONG(&POOL->mThingPool) - 0x70000000);
	if (foo < lta_val)
	{
		lta_val = foo;
		lta_level = GetCurrentLevelNumber();
	}

//	printf("Min space free: %d - Min so far: %d on level %d\n", foo, lta_val, lta_level);
	lowest_thing_allocated = 0xffffffff;

	return mQuit;
}

//******************************************************************************************
void	CDCGame::RunAgeingTest()
{
	// perform ageing test
	SINT	l;
	SINT	character = 4;
	GAME.DeclareSinglePlayer();

	while (1)
	{
		SINT	prison = 0, arena = 0;

		for (prison = 0; prison < 10; prison ++)
		{
			for (arena = 0; arena < CLevelStructure::GetNumLevelsInPrison(prison); arena ++)
			{
				l = CLevelStructure::GetLevelInfo(prison, arena)->mDiskLevel;
//				printf("******* Ageing: Testing Prison: %d, Arena %d, Disk Level: %d\n", prison, arena, l);
				CAREER.mCurrentCharacter = character;
				RunLevel(l);
//				character = (character + 1) % 6;
			}
		}
	};
}

//******************************************************************************************
BOOL	CDCGame::ShowDemoScreen(SINT n)
{
#ifdef JCLDCNOPSX			// Rewrite for DC - if needed
	{
		CDCMemBuffer mem;

		char 	name[100];
#if	DEMO_GAME == 3 || DEMO_GAME == 4
		sprintf(name, "\\SCREENS\\DEM3%d.TIM", n + 1);
#else
		sprintf(name, "\\SCREENS\\DEMO%d.TIM", n + 1);
#endif

		mem.InitFromFile(name);
		RECT	r = {512, 0, 512, 1};
		mem.Skip(20);                   // Skip TIM header...
		long templine[256];
		for(int c0=0;c0<256;c0++)
		{
			mem.Read((char*)templine,1024);
			//while(DrawSync(1)) /*Do Nothing!*/;
			LoadImage(&r,(ULONG*)templine);
			r.y++;
		}
	}
	PLATFORM.ClearPools(); // free up global pools
	PLATFORM.SetViewport(0);
	PLATFORM.InitFrames();

	for(int draws=0;draws<2;draws++)
	{
		int safe;
    	do
    	{
    		safe = PLATFORM.GeometryMarkBeginScene();
    	} while (safe == -1);

		NTAG	*o = PLATFORM.GetOrderTable();
		PLATFORM.SetViewport(0);

		POLY_FT4 	*p4 = PLATFORM.GetMiscPolyFT4s();

		setPolyFT4(p4);
		setRGB0(p4, 127, 127, 127);
		setXYWH(p4, 0, 0, 256, 256);
		setUVWH(p4, 0, 0, 255, 255);
		setTPage(p4, 2, 0, 512, 0);
		addPrim(&o[1], p4);
		p4 ++;

		setPolyFT4(p4);
		setRGB0(p4, 127, 127, 127);
		setXYWH(p4, 256, 0, 256, 256);
		setUVWH(p4, 0, 0, 255, 255);
		setTPage(p4, 2, 0, 768, 0);
		addPrim(&o[1], p4);
		p4 ++;

		PLATFORM.DeclareUsedMiscPolyFT4s(p4);

		int 	f = PLATFORM.GetFrame();
		//NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f + 2));
		NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f));
		NTAG_Rlink(PLATFORM.GetOrderTable(),OT_LENGTH,(long*)&POOL->mNTAGTerms[f][0].firstprim
													,(long*)&POOL->mNTAGTerms[f][0].lastprim);
		PLATFORM.GeometryMarkEndScene();
	}

	if (n == 3)
	{
		SINT 	c0;

		for (c0 = 0; c0 < 50 * 4; c0 ++)
		{
			VSync(0);

#if (DEMO_GAME == 1) || (DEMO_GAME == 3)
			// HACK!!
			struct  SPadData *pad_data = PLATFORM.GetPadData(0);
		    unsigned long PadValue=(~pad_data->buttons)&0xffff;
		    PadValue=((PadValue>>8)&0x00ff)|((PadValue<<8)&0xff00);

			if (PadValue & PADselect)
				break;
#endif
		}
	}

#endif
	return TRUE;
}

//******************************************************************************************
void	CDCGame::RunDemo1()
{
	BOOL	rval = ShowDemoScreen(0);

#if (DEMO_GAME == 2)
	while (TRUE)	// loop forever
#else
	if (rval)
#endif
	{
		SINT 	c0;
		for (c0 = 0; c0 < 3; c0 ++)
		{
			SINT	prison, arena, character, demo;

			switch (c0)
			{
			case 0:
				prison = 7;
				arena = 0;
				character = 4;
				demo = 100;
				break;
			case 1:
				prison = 4;
				arena = 0;
				character = 0;
				demo = 101;
				break;
			case 2:
				prison = 6;
				arena = 4;
				character = 1;
				demo = 102;
				break;
			}

			SINT level = CLevelStructure::GetLevelInfo(prison, arena)->mDiskLevel;
			CAREER.mCurrentCharacter = character;
			CAREER.mCurrentPrison = prison;
			CAREER.mCurrentArena = arena;

			GAME.DeclareSinglePlayer();

			EGQuitType ret=RunLevel(level, demo);

			if (ret == GQ_QUIT)
			{
				// show the last screen
				ShowDemoScreen(3);
				break;
			}

			rval = ShowDemoScreen(c0 + 1);

			if (!rval)
				break;
		}
	};

//	printf("Returning from Demo...\n");
}

//******************************************************************************************
void	CDCGame::RunDemo3()
{
	SINT 	c0;

	//SHOW SPLASH 1

	ShowDemoScreen(0);
	for (c0 = 0; c0 < 50 * 2; c0 ++)
	{
		VSync(0);
	}

	ShowDemoScreen(1);

	SINT	rval = 0;
	BOOL	first = TRUE;

//	if (rval != -1)
	while (rval != -1)
	{
		rval = FEDEMO3.Run(first);
		first = FALSE;

		EGQuitType ret;

		if ((rval >= 0) && (rval <= 2))
		{
			GAME.DeclareSinglePlayer();
			ret = RunLevel(CAREER.mCurrentDiskLevel);

			// process ret...
			if (ret == GQ_FRONTEND_WIN)
				FEENDDEMO3.Run(CAREER.mPendingVictory, rval);
			else
				FEENDDEMO3.Run(-1, rval);

		}
		else
		{
			// run demo??
		}
	}

	//SHOW SPLASH2
	ShowDemoScreen(2);
	for (c0 = 0; c0 < 50 * 2; c0 ++)
		VSync(0);

	ShowDemoScreen(3);

//	printf("Returning from Demo...\n");
}



//******************************************************************************************
SINT	CDCGame::Run()
{
#if DC_PREVIEW == DCP_FIRST
	ShowLoadingScreen(26);
	
	// wait a bit
	float	t = GetSysTimeFloat();
	while (GetSysTimeFloat() < t + 5.f) {};	
#endif	

	// get stereo/mono from boot rom
	CAREER.mStereo = (SDSGetBootRomSoundMode() == 0);

	mInFrontEnd = FALSE;
	// get language?

#if DC_PREVIEW == DCP_NONE
#ifndef DEMO_GAME
#ifdef FORCE_LEVEL
	mLanguage = 0;
#else
	mLanguage =0;
	mInFrontEnd = TRUE;
	mLanguage = LANGUAGESELECT.Run();
	mInFrontEnd = FALSE;
#endif
#else
	mLanguage = 0;
#endif
#else
	mLanguage = 0;
#endif

	// load the correct language text
	CText::Load(mLanguage);

//	return 1;

#if 0
	// stuff for recording movies
	SINT level = CLevelStructure::GetLevelInfo(4, 0)->mDiskLevel;
	CAREER.mCurrentCharacter = 0;
	CAREER.mCurrentPrison = 4;
	CAREER.mCurrentArena = 0;
	GAME.DeclareSinglePlayer();
	RunLevel(level, 200);
#endif

#ifdef AGEING
	RunAgeingTest();

#elif (DEMO_GAME == 1) || (DEMO_GAME == 2)
	RunDemo1();
	return 1;
#elif DEMO_GAME == 3 || DEMO_GAME == 4
	RunDemo3();
	return 1;
#else

	// Intro FMV
#ifndef FORCE_LEVEL
	PlayFMV(FMV_ROCKSTAR);
	PlayFMV(FMV_LOSTTOYS);
	PlayFMV(FMV_BROADSWORD);
	
	// ADX screen
	ShowLoadingScreen(27);
	
	//wait a bit
	float 	tm = GetSysTimeFloat();
	while (GetSysTimeFloat() < tm + 2.f)
	{
		if (GetSysTimeFloat() < tm)
			break; // wrapped...
	};
	
	PlayFMV(EMovie(FMV_INTRO_ENG + mLanguage));
#endif

	SINT l;

//	show_release_notes();

	EPSEntryType	fe_entry = PSE_TOP;

	BOOL dc_exit = FALSE;
	
	while(!dc_exit)
	{
#ifdef FORCE_LEVEL
//		CAREER.mCurrentCharacter = 1;
		EGQuitType ret=RunLevel(FORCE_LEVEL);

#ifdef DEMODISK
		if(ret==GQ_QUIT)
		{
//			printf("Returning from Demo...\n");
			return 1;
		}
#endif
#else

		//!! force a multiplaye end
/*		fe_entry = PSE_MULTIPLAYER_FINISHED;
		MULTIPLAYER_CAREER.mCurrentTournament = 0;
		MULTIPLAYER_CAREER.mWhoWonWhichLevel[0] = 0;
		MULTIPLAYER_CAREER.mWhoWonWhichLevel[1] = 1;
		MULTIPLAYER_CAREER.mWhoWonWhichLevel[2] = -1;
		MULTIPLAYER_CAREER.mWhoWonWhichLevel[3] = 1;*/

		mInFrontEnd = TRUE;
		l = PRISONSELECT.Run(fe_entry);
		mInFrontEnd = FALSE;

		if (l < 0)
		{
			l = -l;
			// attract mode
			if (!(l & 1))
//			if (1)
			{
				// FMV
				SINT 	ret;

				ret = PlayFMV(FMV_ROCKSTAR);
				if (ret != PLAYSTR_USER)
					ret = PlayFMV(FMV_LOSTTOYS);
				if (ret != PLAYSTR_USER)
					ret = PlayFMV(FMV_BROADSWORD);
				if (ret != PLAYSTR_USER)
				{
					// ADX screen
					ShowLoadingScreen(27);
					
					//wait a bit
					float 	tm = GetSysTimeFloat();
					while (GetSysTimeFloat() < tm + 2.f)
					{
						if (GetSysTimeFloat() < tm)
							break; // wrapped...
					};
					ret = PlayFMV(EMovie(FMV_INTRO_ENG + mLanguage));
				}
			}
			else
			{
				SINT	prison, arena, character, demo;

				switch ((l >> 1) % 4)
				{
				default:
				case 0:
					prison = 4;
					arena = 0;
					character = 0;
					demo = 400;
					break;
				case 1:
					prison = 3;
					arena = 3;
					character = 0;
					demo = 401;
					break;
				case 2:
					prison = 2;
					arena = 1;
					character = 0;
					demo = 402;
					break;
				case 3:
					prison = 6;
					arena = 7;
					character = 1;
					demo = 403;
					break;
				}
				
				SINT level = CLevelStructure::GetLevelInfo(prison, arena)->mDiskLevel;
				CAREER.mCurrentCharacter = character;
				CAREER.mCurrentPrison = prison;
				CAREER.mCurrentArena = arena;

				GAME.DeclareSinglePlayer();

				EGQuitType ret=RunLevel(level, demo);

			}

			fe_entry = PSE_TOP;
		}
		else
		{
			while (1)
			{
				EGQuitType ret = RunLevel(l);

				if (ret == GQ_DC_EXIT)
				{	
					dc_exit = TRUE;
					break;
				}
				if (ret == GQ_DC_RESET)
				{
					fe_entry = PSE_TOP;
					break;
				}
				if (ret == GQ_MULTIPLAYER_FINISHED)
				{
					MULTIPLAYER_CAREER.mCurrentArena ++;

					if (MULTIPLAYER_CAREER.mCurrentArena == CLevelStructure::GetTournamentInfo(MULTIPLAYER_CAREER.mCurrentTournament)->mNumLevels)
					{
						fe_entry = PSE_MULTIPLAYER_FINISHED;
						break;
					}
					else
					{
						l = CLevelStructure::GetTournamentInfo(MULTIPLAYER_CAREER.mCurrentTournament)->mLevels[MULTIPLAYER_CAREER.mCurrentArena].mDiskLevel;
					}
				}
				else if (ret == GQ_FRONTEND_WIN)
				{
					if (l > 200)
					{
						if (((l - 200) % 3) == 0)
						{
							fe_entry = PSE_SINGLE_WIN;
							break;
						}
						else
						{
							// need to keep going..
							l ++;
						}
					}
					else
					{
						fe_entry = PSE_SINGLE_WIN;

						// play win animation??
						if (l == 80)
						{
							if (CAREER.mCharCareers[CAREER.mCurrentCharacter].mWonGame == FALSE)
							{
								CAREER.mCharCareers[CAREER.mCurrentCharacter].mWonGame = TRUE;
								PlayFMV(EMovie(FMV_ALPHASTAR_ANGEL + CAREER.mCurrentCharacter));
							}
						}

						break;
					}
				}
				else if (ret == GQ_QUIT)
				{
					return (0);
				}
				else if (IsMultiPlayer())
				{
					fe_entry = PSE_MULTIPLAYER;
					break;
				}
				else
				{
					fe_entry = PSE_SINGLE;
					break;
				}
			}
		}
#endif
	}

#endif
	return 1;
}

//******************************************************************************************
UINT	CID_BLV		= MKID("BLV!");
UINT	CID_LEVD	= MKID("LevD");
UINT	CID_MAP		= MKID("Map ");
UINT	CID_WRLD	= MKID("Wrld");

UINT	CID_BPLV	= MKID("BPLV");
UINT	CID_TPAG	= MKID("TPAG");
UINT	CID_MESH	= MKID("MESH");
UINT	CID_RTID	= MKID("RTID");

extern	BOOL	first_run_through;

BOOL	CDCGame::LoadLevel(SINT levelno)
{
	CMEMBUFFER	mem;

	first_run_through = TRUE;

	// reset all internal data
	SDSFreeAllTextures();
	PLATFORM.ClearPools(); // free up global pools

	mem.DeclareFlippable(); // assuming the loading screen is setup OK!

	SINT	pris;
	if (mMultiplayer)
		pris = CLevelStructure::GetPrisonForMultiplayerLevel(MULTIPLAYER_CAREER.mCurrentTournament, MULTIPLAYER_CAREER.mCurrentArena);
	else
		pris = CAREER.mCurrentPrison;

	if (CAREER.AllGold())
		pris += 11;
	ShowLoadingScreen(pris + 4);

    SDSFreeAllTextures();
	PLATFORM.ClearPools(); //! er.. free up pool again!  (loading screen uses it!)

	mLevelData.Reset();
	WORLD.Reset();
	MAP.Reset();  //! NB map gets reset in the PC loader....
	Reset();

	RANDOMSTREAM.Reset();

	char	name[256];

	//!Temp
	if ((levelno >= 100) && (levelno <= 199))
		DeclareMultiPlayer();


	// open the file

	SINT a, b;

	a = (levelno / 100) * 100;
	b = (((levelno - a) / 25) * 25) + a;

	sprintf(name, "LEVELS\\%03dTO%03d\\%03dTO%03d\\LEVEL%03d.DAT", a, a + 99, b, b + 24, levelno);
//	sprintf(name, "LEVELS\\LEVEL%03d.DAT", levelno);
	mLevelNo = levelno;

	//TRACE("************************\n");
	//TRACE("About to load level %03d\n", levelno);

	if (!(mem.InitFromFile(name)))
	{
		//mw_pr("File Load failed\n");

		mem.Close();
		return FALSE;
	}
	//TRACE("File Load successful\n");

	// First load in the PC specific stuff
	UINT	chunk;

	// check if file is valid
	mem.Read(&chunk, 4);

	if (chunk != CID_BPLV)
		return FALSE;

	mLevelDataOld = FALSE;

	//TRACE("About to read chunks\n");

	mem.Read(&chunk, 4);
	ASSERT(chunk == CID_LEVD);
	if (!(mLevelData.Read(mem)))
	{
		ASSERT(0);
		return FALSE;
	}

//	printf("Loaded LevelData OK\n");
	InitLevelType(); // Init level based on level data

	mem.Read(&chunk, 4);
	ASSERT(chunk == CID_MAP);
	if (!(MAP.Read(mem)))
	{
		//mw_pr("map load failed");
		ASSERT(0);
		return FALSE;
	}

	//printf("Loaded Map OK\n");
	//TRACE("Read PC chunks\n");

	//**********************************
	// Now load in the PSX specific stuff
	// Read Texture page

	
	mem.Read(&chunk, 4);
	if (chunk != CID_TPAG)
	{
		//mw_pr("Couldn't load TPage\n");
		return FALSE;
	}

	ENGINE.ReadPSXTextures(mem);

	// Read Meshes
	mem.Read(&chunk, 4);
	if (chunk != CID_MESH)
	{
		//mw_pr("Couldn't load Meshes\n");
		//return FALSE;
	}

//	printf("Loading Meshes\n");
	ENGINE.ReadPSXMeshes(mem);
//	printf("Loaded Meshes\n");

	// Read Thing Renderinfo
	mem.Read(&chunk, 4);
	if (chunk != CID_RTID)
	{
		//mw_pr("Couldn't load RenderInfo\n");
		//return FALSE;
	}

	ENGINE.ReadPSXRTInfo(mem);

//	printf("Loading World\n");

	// Finally, read the WORLD
	mem.Read(&chunk, 4);
	ASSERT(chunk == CID_WRLD);
	if (!(WORLD.Read(mem)))
	{
		//mw_pr("Couldn't load WORLD\n");
		ASSERT(0);
		//return FALSE;
	}

//	printf("Loaded World OK\n");

	InitLevelType();	// setup level type

	MAP.PostLoadProcess();

	//**DEBUG
	static	SINT	lps_check = 0;
	static	SINT	lps_lnum = -1;
	if (POOL->mLevelPoolSize > lps_check)
	{
		lps_check = POOL->mLevelPoolSize;
		lps_lnum = levelno;
	}

//	printf("Level Pool - %d / %d  used.  Maximum used so far: %d, on level %d\n", POOL->mLevelPoolSize, LEVEL_POOL_SIZE, lps_check, lps_lnum);

	mem.Close();

	SOUND.DeclareVolumeChange();  // update volumes

#if (DEMO_GAME == 1) || (DEMO_GAME == 2)
	// moved to after demo load.
//	SOUND.PlayTrack(((CAREER.mCurrentPrison + 1) % 2), 2);               // Play from track 0, repeating all...
#elif (DEMO_GAME == 3) || DEMO_GAME == 4
	SOUND.PlayTrack(0, 2);               // Play from track 0, repeating all...
#else
	SOUND.PlayTrack(((levelno + 4) % 6), 2);               // Play from track 0, repeating all...
#endif

	return TRUE;

}

//*****************************************************************************************
void	CDCGame::RestartLevel()
{
	WORLD.Reset();
	Reset();

	RANDOMSTREAM.Reset();

	InitLevelType();	// setup level type

	MAP.ResetOffsets();
	MAP.InitMappy();

	WORLD.RestoreFromCache();

	MAP.PostLoadProcess();
}


//*****************************************************************************************
//*****************************************************************************************

struct TGAHeader
{
	UBYTE	mIDLength;
	UBYTE	mColourMapType;
	UBYTE	mImageType;
	UBYTE	mColourMapSpec[5];
	UWORD	mOriginX;
	UWORD	mOriginY;
	UWORD	mSizeX;
	UWORD	mSizeY;
	UBYTE	mPixelDepth;
	UBYTE	mImageDescriptor;
};



//#pragma optimization_level 0
void	CDCGame::TakeScreenShot()
{
#if 0
	static 	int shot_no = 0;

/*	if (shot_no < 621)
	{
		shot_no ++;
		return;
	}
*/
	//!!  hmm.........

	DrawSync(0);
	DrawSync(0);
	VSync(0);
	VSync(0);
	VSync(0);
	VSync(0);

	// make sure we're done drawing

//	WORLD.Shutdown();
//	PLATFORM.ClearPools(); // free up global pools
//	PLATFORM.InitFrames();

	// get the image from the screen
	RECT	r = {0, 0, 512, 256};

	PLATFORM.GetScreenPos(r.x, r.y);
//	UBYTE	*p = POOL->mLevelPool;
	UBYTE	*p = (UBYTE *)0x80600000;
	StoreImage(&r,(ULONG*)p);
	DrawSync(0);

	TGAHeader	h;

	h.mIDLength = 0;
	h.mColourMapType = 0;
	h.mImageType = 2;
	h.mOriginX = 0;
	h.mOriginY = 0;
	h.mSizeX = 512;
	h.mSizeY = 256;
	h.mPixelDepth = 24;
	h.mImageDescriptor = 0;

	// now write out as 24 bit

	UBYTE 	*d = p + (512 * 256 * 2) + (512 * 255 * 3);
	UWORD	*s = ((UWORD *)p);

	SINT	c0, c1;

	for (c0 = 0; c0 < 256; c0 ++)
	{
		for (c1 = 0; c1 < 512; c1 ++)
		{
			SINT	r, g, b;
			b = ( (*s)        & 31) << 3;
			g = (((*s) >>  5) & 31) << 3;
			r = (((*s) >> 10) & 31) << 3;

			d[0] = r;
			d[1] = g;
			d[2] = b;

			s ++;
			d += 3;
		}
		d -= 512 * 6;
	}

	// and write it out!

	char	fname[100];
	sprintf(fname, "c:\\dev\\ballz\\shots\\psxscr%05d.tga", shot_no ++);
	char 	fname2[256];
	sprintf(fname2, "cdrom:%s", fname);
//	printf("Trying to open \"%s\"\n",fname2);
	// load file
	SINT	handle = MWopen(fname2,O_CREAT);
//	printf("Returned handle: %d\n",handle);

	if (handle != -1)
	{
		MWwrite(handle, (char *)(&h), sizeof(h));
		MWwrite(handle, p + (512 * 256 * 2), 512 * 256 * 3);
		MWclose(handle);
	}

//	mQuit = GQ_FRONTEND;
	// finally reload the level cause we've trashed all the buffers!!
//	LoadLevel(mLevelNo);
#endif
}
//#pragma optimization_level 4

#endif
