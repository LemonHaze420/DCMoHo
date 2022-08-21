// $Header$

// $Log$
// Revision 1.67  2000-10-20 17:19:29+01  are
// <>
//
// Revision 1.66  2000-10-20 10:22:14+01  are
// Hopefully shouldn't flash loading screen.
//
// Revision 1.65  2000-10-19 15:42:29+01  are
// Jeremy's optamisations.
//
// Revision 1.64  2000-10-18 15:44:04+01  are
// Water particles
//
// Revision 1.63  2000-10-18 11:55:54+01  are
// Press space message rather than press fire.
//
// Revision 1.62  2000-10-17 20:37:33+01  are
// Added ShowLoadingScreenWhilstWaitingForUserToPressSpace method.
//
// Revision 1.61  2000-10-17 15:27:40+01  are
// Loading screen now correctly draw in hi-res.
//
// Revision 1.60  2000-10-17 14:39:07+01  are
// <>
//
// Revision 1.59  2000-10-17 12:29:49+01  are
// Debug.
//
// Revision 1.58  2000-10-16 18:53:29+01  asd
// added stuff
//
// Revision 1.57  2000-10-16 16:52:27+01  are
// Change resolution before and after playing videos. (32bit display)
//
// Revision 1.56  2000-10-16 15:41:35+01  are
// Process map particles once per game loop.
//
// Revision 1.55  2000-10-16 14:28:37+01  are
// Draws loading message on loading screens. Works at high res.
//
// Revision 1.54  2000-10-15 16:25:29+01  asd
// took out help text for golf level
//
// Revision 1.53  2000-10-14 21:24:17+01  asd
// added text to loading screens
//
// Revision 1.52  2000-10-12 16:23:08+01  are
// Camera pan code.
//
// Revision 1.51  2000-10-11 15:58:39+01  are
// More frame interpolation code.
//
// Revision 1.50  2000-10-05 15:47:53+01  are
// Frame interpolation - incomplete (the character jumps about).
//
// Revision 1.49  2000-10-04 17:13:16+01  are
// <>
//
// Revision 1.48  2000-10-04 15:09:53+01  are
// <>
//
// Revision 1.47  2000-10-04 14:27:43+01  are
// stuff
//
// Revision 1.46  2000-10-04 13:49:50+01  asd
// Moved fog setting lines out of ifndef debug statements
//
// Revision 1.45  2000-10-04 11:17:15+01  are
// Changed fog type and turn off fog going into frontend.
//
// Revision 1.44  2000-10-04 10:45:50+01  asd
// added line to put in fog effect
//
// Revision 1.43  2000-10-04 10:18:42+01  are
// Guard around res change for debug and doesn't change resolution if it doesn't have to - i.e. in game is running at 640, 480, 16 bpp.
//
// Revision 1.42  2000-10-03 17:35:41+01  are
// Scale loading screen.
//
// Revision 1.41  2000-10-03 16:49:58+01  asd
// added stuff for multiple resolution
//
// Revision 1.40  2000-10-02 15:26:08+01  jjs
// Removes LINK_EDITOR if NO_LINK_EDITOR defined in settings.
//
// Revision 1.39  2000-10-02 14:15:42+01  jcl
// <>
//
// Revision 1.38  2000-09-29 11:20:49+01  jcl
// <>
//
// Revision 1.37  2000-09-28 17:26:06+01  are
// Collects controller info from setup process.
//
// Revision 1.36  2000-09-28 13:55:42+01  jcl
// <>
//
// Revision 1.35  2000-09-27 11:39:06+01  jcl
// <>
//
// Revision 1.34  2000-09-27 10:46:00+01  asd
// added line to change cd track for levels.
//
// Revision 1.33  2000-09-27 10:35:00+01  jcl
// <>
//
// Revision 1.32  2000-09-25 11:21:50+01  are
// Moved language detection (read from registry) code to PCSetupQuery.cpp
//
// Revision 1.31  2000-09-25 10:22:18+01  jjs
// Further EnginePanel changes.
//
// Revision 1.30  2000-09-20 17:00:10+01  jcl
// <>
//
// Revision 1.29  2000-09-20 15:12:22+01  jjs
// Removed Debug info.
//
// Revision 1.28  2000-09-19 10:03:07+01  jcl
// <>
//
// Revision 1.27  2000-09-19 10:01:55+01  jcl
// <>
//
// Revision 1.26  2000-09-18 11:59:10+01  jcl
// <>
//
// Revision 1.25  2000-09-18 11:11:15+01  jcl
// <>
//
// Revision 1.24  2000-09-16 18:38:04+01  jcl
// <>
//
// Revision 1.23  2000-09-15 16:33:57+01  are
// Stop sound effects when finishing level.
//
// Revision 1.22  2000-09-15 15:53:53+01  asd
// Added line to run update sound
//
// Revision 1.21  2000-09-15 12:31:59+01  are
// <>
//
// Revision 1.20  2000-09-15 11:08:50+01  are
// Sound commented out.
//
// Revision 1.19  2000-09-15 10:55:23+01  are
// Update sound in game loop and restart level.
//
// Revision 1.18  2000-09-15 10:05:16+01  jjs
// Debugging for GeForce
//
// Revision 1.17  2000-09-14 19:49:18+01  jcl
// <>
//
// Revision 1.16  2000-09-14 16:55:24+01  jjs
// Fixes lightning.
//
// Revision 1.15  2000-09-14 16:22:22+01  jcl
// <>
//
// Revision 1.14  2000-09-14 13:02:30+01  jcl
// <>
//
// Revision 1.13  2000-09-14 11:03:51+01  jcl
// <>
//
// Revision 1.12  2000-09-13 16:51:56+01  sds
// old editor stuff
//
// Revision 1.11  2000-09-13 16:50:02+01  jcl
// <>
//
// Revision 1.10  2000-09-12 14:10:24+01  are
// Temp implementation of GetFrameRenderFraction.
//
// Revision 1.9  2000-09-12 11:58:53+01  are
// Language setting read from the registry.
// Default key = "CURRENT_USER"
// Key = "\Software\Broadsword\MoHo"
// Value = "Language"
// The value for key is defined in PCSaveRoutines.h
//
// Revision 1.8  2000-09-11 15:03:33+01  are
// Pass character info into game
//
// Revision 1.7  2000-09-07 17:05:54+01  are
// Catches up on Windows events and initialises controller states.
//
// Revision 1.6  2000-08-31 17:36:37+01  are
// <>
//
// Revision 1.5  2000-08-25 17:27:58+01  are
// <>
//
// Revision 1.4  2000-08-24 10:54:40+01  are
// Load controller two config.
//
// Revision 1.3  2000-08-23 17:35:14+01  are
// Load custom controller config.
//
// Revision 1.2  2000-08-17 17:45:14+01  are
// Added support for loading and saving.
//
// Revision 1.1  2000-08-05 10:33:28+01  jjs
// First PC milestone.
//


#include	"Common.h"

#if TARGET == PC

#include	"Globals.h"
#include	"PCGame.h"
#include	"Profile.h"
#include	"Primary.h"

#include	"FrontEndCommon.h"
#include 	"GamePanel.h"
#include	"Career.h"
#include	"text.h"

#include	"PCSaveRoutines.h"

#include	"PCFEPrisonSelect.h"
#include	"PCFELanguageSelect.h"
#include	"PCFMV.h"

#include	"LevelProgression.h"

#include	"PCSetUpQuery.h"

//#define FORCE_LEVEL 9

#define		EMERGENCY_EXPORT_DC  -1		// nothing
//#define		EMERGENCY_EXPORT_DC  -2		// front end
//#define		EMERGENCY_EXPORT_DC  -3		// all levels
//#define		EMERGENCY_EXPORT_DC  -4		// 5 Levels from command line
//#define		EMERGENCY_EXPORT_DC  5

//#define		EXPORT_FONT_DATS

//******************************************************************************************
void	CPCGame::ShowSplashScreen()
{
	PLATFORM.ProcessMessages();

	ENGINE.Flip();

	SINT c = GetTickCount();
	SINT s = c;
	while (!(PLATFORM.IsKeyDown(VK_SPACE)))
	{
		SINT d;
		while ((d = GetTickCount()) == c)
			PLATFORM.ProcessMessages();

		c = d;

		ENGINE.DrawSplashScreen(c - s);
		ENGINE.Flip();
		
		PLATFORM.ProcessMessages();
		if (c - s > 10000)
			break;
	}

	c = s = GetTickCount();
	while (c - s < 1001)
	{
		SINT d;
		while ((d = GetTickCount()) == c)
			PLATFORM.ProcessMessages();

		c = d;

		SINT v = -1000 + (c - s);
		if (v > -1)
			v = -1;

		ENGINE.DrawSplashScreen(v);
		ENGINE.Flip();
		
		PLATFORM.ProcessMessages();
	}
}


#define FRAME_TIME	(0.04f)
GINT 	CPCGame::GetFrameRenderFraction() 
{
	return FToG((mFrameTime - mBaseTime) / FRAME_TIME);
};

//******************************************************************************************
//** Main game loop

EGQuitType	CPCGame::RunLevel(SINT levelno)
{
	// load level
	GAME.LoadLevel(levelno);

	GAMEPANEL.DeActivateNow();

	mBaseTime = ENGINE.GetSysTimeFloat(); 

	while (1)
	{
		while (mQuit == GQ_NONE)
		{
	#if ENABLE_PROFILER==1
			CProfiler::ResetAll();
	#endif
			float	start_time = ENGINE.GetSysTimeFloat();

			{
				PROFILE_FN(TotalModel);
				mFrameTime = mBaseTime;

				// handle windows messages
				PLATFORM.ProcessMessages();

				// process panel   ( needs to be before CONTROLS.Process() so it gets processed a turn later
				GAMEPANEL.Process();
				
				// handle controls
				CONTROLS.Process();

				// Process sound
				SOUND.UpdateStatus();
				
				
				// handle game
				if (mGameTurn == -1) // run a few turns so everything's initialised properly.
				{
					mGameTurn = 0;  // so that this doesn't keep happening in the editor...
					ProcessGame();
					ProcessGame(); 
				}
				ProcessGame();
//#if TARGET == PC
//				MAP.ProcessParticles();
//#endif
			}

			float	after_model_time = ENGINE.GetSysTimeFloat();
			if (after_model_time < start_time)
			{
				// oh dear - timer wrapped)
				after_model_time = start_time + 0.01f;  // approximate model time...
			}

			PROFILE_FN(TotalEngine);
			SINT	num_renders = 0;
			do
			{
				if (ENGINE.GetSysTimeFloat() < mBaseTime)
				{
					// oh dear - timer wrapped..
					mBaseTime = ENGINE.GetSysTimeFloat() - (after_model_time - start_time);
				}

				if (!mGamePanelActive)
					mFrameTime = ENGINE.GetSysTimeFloat() - (after_model_time - start_time);
				else
					mFrameTime = mBaseTime;

				if (mQuit == GQ_NONE)
				{
					// render
					if (!(mPlayer[1].IsActive()))
						ENGINE.Render(&mCameras[0], NULL);
					else
					{
						ENGINE.Render(&mCameras[0],&mCameras[1]);
					}
				}
				num_renders++;

				ENGINE.Flip(); // Here, yeah?
			//		FrameLimit();
			}
			while ((ENGINE.GetSysTimeFloat() - mBaseTime) < FRAME_TIME);
				
			// now adjust the base time
			if ((num_renders == 1) && ((ENGINE.GetSysTimeFloat() - start_time) > FRAME_TIME))
			{
				PROFILE_FN(RenderResetInt);
//				mBaseTime = GetSysTimeFloat(); 	// running slowly...
				mBaseTime += (ENGINE.GetSysTimeFloat() - start_time); 	// running slowly...
				if ((ENGINE.GetSysTimeFloat() - start_time) > FRAME_TIME * 1.3f)
					mBaseTime = ENGINE.GetSysTimeFloat(); // running *very* slowly...
			}
			else
				mBaseTime += FRAME_TIME;
		};


// are: FROM DREAMCAST VERSION
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
				break;

		}
	}

	SOUND.KillAllSamples();
	WORLD.Shutdown();


	return mQuit; // normal exit
}

//******************************************************************************************
SINT	CPCGame::Run()
{
	// Need to catch up with a few messages before going into the game.
	for (int i = 100; i--;)
	{
		PLATFORM.ProcessMessages();
	}

#ifndef DEMO_GAME
#ifdef FORCE_LEVEL
	mLanguage = 0;
#else

	// Language detection code has moved to PCSetupQuery.cpp::doConfig.
	mLanguage = PCSetupQueryLanguage;
	if (	(mLanguage < 0) 
		||	(mLanguage > MAX_LANGUAGE_INDEX)
		)
	{
		mLanguage = DEFAULT_LANGUAGE;
	}
#endif

#else
	mLanguage = 0;
#endif
	mInFrontEnd = true;
	CText::Load(mLanguage);
	mInFrontEnd = false;


#if EMERGENCY_EXPORT_DC  == -1
	
	FMVResChange(true);
	PlayFMV(FMV_ROCKSTAR);
	PlayFMV(FMV_LOSTTOYS);
	PlayFMV(FMV_BROADSWORD);
	PlayFMV(EMovie(FMV_INTRO_ENG + mLanguage));
	FMVResChange(false);
#endif

	EPSEntryType	fe_entry = PSE_TOP;

	// Get a list of saved games
	PCStartFileList();

	CONTROLS.mControllers[0].mControllerConfig.ClearUp();
	if (WorkingController[0])
		CONTROLS.mControllers[0].mControllerConfig.AssignFromStartup(WorkingController[0]);

	CONTROLS.mControllers[1].mControllerConfig.ClearUp();
	if (WorkingController[1])
		CONTROLS.mControllers[1].mControllerConfig.AssignFromStartup(WorkingController[1]);


#if EMERGENCY_EXPORT_DC != -1

#if EMERGENCY_EXPORT_DC == -4
	SINT	c0;

	for (c0 = mCLIParams.mLevel; c0 < mCLIParams.mLevel + 10; c0 ++)
	{
		BOOL res = GAME.LoadLevel(c0);
		if (res)
			GAME.SaveDCLevel(c0);
	}

	if (1)
	{}
	else	// sorry!		
#elif EMERGENCY_EXPORT_DC == -3
	// export all
	GAME.SaveDCFrontEndData();

	SINT	c0;
	for (c0 = 0; c0 < 1000; c0 ++)
	{
		BOOL res = GAME.LoadLevel(c0);
		if (res)
			GAME.SaveDCLevel(c0);
	}

#elif EMERGENCY_EXPORT_DC == -2
	GAME.SaveDCFrontEndData();
#else
	BOOL res = GAME.LoadLevel(EMERGENCY_EXPORT_DC);
	if (res)
		GAME.SaveDCLevel(EMERGENCY_EXPORT_DC);
	else		// sorry!!!!
#endif
#endif	

#ifdef	EXPORT_FONT_DATS
	WritePCFontDats();

	if (0) // hmm.
#endif

	//****************************************************************
	// MAIN LOOP
	while(1)
	{
		SINT	l;

#ifdef FORCE_LEVEL
		EGQuitType ret=RunLevel(FORCE_LEVEL);

#else

		/* // Test  tournement screen.
		fe_entry = PSE_MULTIPLAYER_FINISHED;
		MULTIPLAYER_CAREER.mCurrentTournament = 0;
		MULTIPLAYER_CAREER.mWhoWonWhichLevel[0] = 1;
		MULTIPLAYER_CAREER.mWhoWonWhichLevel[1] = 0;
		MULTIPLAYER_CAREER.mWhoWonWhichLevel[2] = 0;
		MULTIPLAYER_CAREER.mWhoWonWhichLevel[3] = -1;
		*/


		mInFrontEnd = TRUE;
		l = PRISONSELECT.Run(fe_entry);
		mInFrontEnd = FALSE;
		
		if (l == -999)
			break; // user wants to quit.....
		
#ifndef _DEBUG
		// Don't go through the process of changing resolution if there is no need. 
		if (	(SelectedWidth	!= 640)	
			||	(SelectedHeight	!= 480) 
			||	(SelectedDepth	!=  16)
			)
		{
			PLATFORM.GetDPWin()->DeclareResolutionChange(SelectedWidth, SelectedHeight, SelectedDepth);
		}
#endif
		
		// JCL - no attract in PC
/*		if (l < 0)
		{
			l = -l;
			// attract mode
			if (!(l & 1))
			{
				// FMV
				SINT 	ret;

				ret = PlayFMV(FMV_ROCKSTAR);
				if (ret != PLAYSTR_USER)
					ret = PlayFMV(FMV_LOSTTOYS);
				if (ret != PLAYSTR_USER)
					ret = PlayFMV(EMovie(FMV_INTRO_ENG + mLanguage));
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
					demo = 300;
					break;
				case 1:
					prison = 3;
					arena = 3;
					character = 0;
					demo = 301;
					break;
				case 2:
					prison = 2;
					arena = 1;
					character = 0;
					demo = 302;
					break;
				case 3:
					prison = 6;
					arena = 7;
					character = 1;
					demo = 303;
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
		else*/
		{
			while (1)
			{
				EGQuitType ret = RunLevel(l);

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
								FMVResChange(true);
								PlayFMV(EMovie(FMV_ALPHASTAR_ANGEL + CAREER.mCurrentCharacter));
								FMVResChange(false);
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
#ifndef _DEBUG
		if (	(SelectedWidth	!= 640)	
			||	(SelectedHeight	!= 480) 
			||	(SelectedDepth	!=  16)
			)
		{
			PLATFORM.GetDPWin()->DeclareResolutionChange(640, 480, 16);
		}
#endif
		ENGINE.DisableFogEffect();
	}

	return 1; // whatever
}

//******************************************************************************************
//******************************************************************************************
//** Load / Save levels

UINT	CID_BLV		= MKID("BLV!");
UINT	CID_LEVD	= MKID("LevD");
UINT	CID_MAP		= MKID("Map ");
UINT	CID_WRLD	= MKID("Wrld");
UINT	CID_GMSH	= MKID("GMsh");

extern	BOOL	first_run_through;

BOOL	CPCGame::LoadLevel(SINT levelno)
{
	CMEMBUFFER	mem;
	
	first_run_through = TRUE;

	// reset all internal data
	mLevelData.Reset();
	WORLD.Reset();
	MAP.Reset();  //! NB map gets reset in the PC loader....
	ENGINE.Reset();
	Reset();

	RANDOMSTREAM.Reset();

	SINT	pris;
	if (mMultiplayer)
		pris = CLevelStructure::GetPrisonForMultiplayerLevel(MULTIPLAYER_CAREER.mCurrentTournament, MULTIPLAYER_CAREER.mCurrentArena);
	else
		pris = CAREER.mCurrentPrison;

	if (CAREER.AllGold())
		pris += 11;
	ShowLoadingScreen(pris + 4);

	MAP.LoadPinMap(levelno);

	char	name[256];

	if (levelno >= 10000)  // undo levels for editor.
		sprintf(name, "data\\editor\\undo\\Level%03d.dat", levelno - 10000);
	else
	{
		sprintf(name, "data\\levels\\Level%03d.dat", levelno);
		mLevelNo = levelno;
	}

	// load engine stuff
	if (!ENGINE.LoadLevelTextures())
		return FALSE;

	if (!(mem.InitFromFile(name)))
	{
		mem.Close();
		ENGINE.PrepareLevel();
		return FALSE;
	}

	UINT	chunk;

	// check if file is valid
	mem.Read(&chunk, 4);
	if (chunk != CID_BLV)
	{
		mem.Close();
		ENGINE.PrepareLevel();
		return FALSE;
	}

	mLevelDataOld = FALSE;

	while (mem.IsMoreData())
	{
		mem.Read(&chunk, 4);

		     if (chunk == CID_LEVD) {if (!(mLevelData.Read(mem)))	{ASSERT(0);}    InitLevelType();}
		else if (chunk == CID_MAP ) {if (!(MAP.Read(mem)))			{ASSERT(0);}}
		else if (chunk == CID_WRLD) {if (!(WORLD.Read(mem)))		{ASSERT(0);}}
		else if (chunk == CID_GMSH) {if (!(ENGINE.Read(mem)))		{ASSERT(0);}}
		else ASSERT(0);
	};

	mem.Close();
#if defined(LINK_EDITOR)
	if (mLevelDataOld == TRUE)
	{
		char str[256];
		sprintf(str, "Level %03d is out of date - updating and saving,,,", mLevelNo);
		CONSOLE.AddString(str);
		SaveLevel(); // save level immediately if we've encountered an old level.
	}
#endif
//	InitLevelType();	// setup level type

	ENGINE.PrepareLevel();

	SOUND.PlayTrack(((levelno + 4) % 6), 2);               // Play from track 0, repeating all...

	// Sit here until user presses space.
	ShowLoadingScreenWhilstWaitingForUserToPressSpace(pris + 4);

	return TRUE;
}

//*****************************************************************************************
void	CPCGame::RestartLevel()
{
	WORLD.Reset();
	Reset();

	RANDOMSTREAM.Reset();

	InitLevelType();	// setup level type

	MAP.ResetOffsets();

	WORLD.RestoreFromCache();

	MAP.PostLoadProcess();

	mFirstTimeThrough = true;
}

#if defined(LINK_EDITOR)
//******************************************************************************************
void	CPCGame::RelocateGlobalMeshes()
{
	ENGINE.ClearRelocatedMeshes();
	WORLD.RelocateGlobalMeshes();
}

//******************************************************************************************
BOOL	CPCGame::SaveLevel(SINT levelno)
{
	CMEMBUFFER	mem;
	mem.InitFromMem();

	// header information
	mem.Write(&CID_BLV, 4);

	// relocate global meshes
	RelocateGlobalMeshes();

	// now ask each bit to write itself out
	mem.Write(&CID_LEVD, 4);
	mLevelData.Write(mem);

	mem.Write(&CID_MAP, 4);
	MAP.Write(mem);

	mem.Write(&CID_GMSH, 4);  // needs to be before things, so they can reference the meshes
	ENGINE.Write(mem);

	mem.Write(&CID_WRLD, 4);
	WORLD.Write(mem, TRUE);

	ENGINE.ClearRelocatedMeshes();

	// write out data
	char	name[256];

	if (levelno >= 10000)  // undo levels for editor.
		sprintf(name, "data\\editor\\undo\\Level%03d.dat", levelno - 10000);
	else 
	{
		if (levelno == -1) // use current level
			sprintf(name, "data\\levels\\Level%03d.dat", mLevelNo);
		else
			sprintf(name, "data\\levels\\Level%03d.dat", levelno);
	}

	return mem.Close(name);	
}
#endif

//******************************************************************************************
//******************************************************************************************

//******************************************************************************************
void	get_screen_name(SINT n, char *screen_name)
{
	switch (n)
	{
	case  0: strcpy(screen_name, "..\\screens\\ms");	break;
	case  1: strcpy(screen_name, "..\\screens\\mgs");	break;
	case  2: strcpy(screen_name, "..\\screens\\bbs");	break;
	case  3: strcpy(screen_name, "..\\screens\\bbgs");	break;
	case  4: strcpy(screen_name, "..\\screens\\eco");	break;
	case  5: strcpy(screen_name, "..\\screens\\zp");	break;
	case  6: strcpy(screen_name, "..\\screens\\ho");	break;
	case  7: strcpy(screen_name, "..\\screens\\sh");	break;
	case  8: strcpy(screen_name, "..\\screens\\re");	break;
	case  9: strcpy(screen_name, "..\\screens\\min");	break;
	case 10: strcpy(screen_name, "..\\screens\\dr");	break;
	case 11: strcpy(screen_name, "..\\screens\\sp");	break;
	case 12: strcpy(screen_name, "..\\screens\\ml");	break;
	case 13: strcpy(screen_name, "..\\screens\\al");	break;
	case 14: strcpy(screen_name, "..\\screens\\lm");	break;

	case 15: strcpy(screen_name, "..\\screens\\gold\\eco");		break;
	case 16: strcpy(screen_name, "..\\screens\\gold\\zep");		break;
	case 17: strcpy(screen_name, "..\\screens\\gold\\hosp");	break;
	case 18: strcpy(screen_name, "..\\screens\\gold\\ship");	break;
	case 19: strcpy(screen_name, "..\\screens\\gold\\rehab");	break;
	case 20: strcpy(screen_name, "..\\screens\\gold\\mine");	break;
	case 21: strcpy(screen_name, "..\\screens\\gold\\death");	break;
	case 22: strcpy(screen_name, "..\\screens\\gold\\sport");	break;
	case 23: strcpy(screen_name, "..\\screens\\gold\\mil");		break;
	case 24: strcpy(screen_name, "..\\screens\\gold\\alpha");	break;
	case 25: strcpy(screen_name, "..\\screens\\gold\\bonus");	break;

	case 26: strcpy(screen_name, "..\\screens\\prev");	break;
	};
}

#ifdef LINK_EDITOR

//******************************************************************************************
void	CPCGame::AddPSXGlobalTextures()
{
	// add font textures
	mTextureAccumulator.AddTexture("fonts\\fnt1eng.tga", TATT_FONT, 0, 256, 128);
	mTextureAccumulator.AddTexture("fonts\\fnt2eng.tga", TATT_FONT, 1, 128, 48);
	mTextureAccumulator.AddTexture("fonts\\fnt3eng.tga", TATT_FONT, 2, 256, 128);

	// stuff
	mTextureAccumulator.AddTexture("Water.tga" , TATT_NUMBERED, TN_WATER      , -1, -1, BD_4, 0, TRUE);
}
#endif
//******************************************************************************************
/*#define	GMN_SECT1	"sect1.msh"
#define	GMN_SECT2	"sect2.msh"
#define	GMN_SECT3	"sect3.msh"
#define	GMN_SECTC	"sectc.msh"
#define	GMN_SECT60	"sect60.msh"
#define	GMN_SECTE	"sectent.msh"*/

/*#define	GMN_SECT1	"as_sec1.msh"
#define	GMN_SECT2	"as_sec2.msh"
#define	GMN_SECT3	"as_sec3.msh"
#define	GMN_SECTC	"as_corn.msh"
#define	GMN_SECT60	"as_narr.msh"
#define	GMN_SECTE	"as_sec1.msh"*/

#define	GMN_SECT1	"mil_sec1.msh"
#define	GMN_SECT2	"mil_sec2.msh"
#define	GMN_SECT3	"mil_sec3.msh"
#define	GMN_SECT4	"mil_sec4.msh"
#define	GMN_SECTC	"mil_corn.msh"
#define	GMN_SECT60	"mil_nar.msh"

char	*get_stadium_name(SINT sn, SINT prison_no)
{
	switch(prison_no)
	{
	case 0:		// eco
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "eco_sec1.msh";
//		case NM_STADIUM_WALL2:		return "eco_sec2.msh";
//		case NM_STADIUM_WALL3:		return "eco_sec3.msh";
//		case NM_STADIUM_WALL4:		return "eco_sec4.msh";
//		case NM_STADIUM_CORNER:		return "eco_corn.msh";
//		case NM_STADIUM_WALL60:		return "eco_nar.msh";
		case NM_STADIUM_WALL1:		return "ec_sec1.msh";
		case NM_STADIUM_WALL2:		return "ec_sec2.msh";
		case NM_STADIUM_WALL3:		return "ec_sec3.msh";
		case NM_STADIUM_WALL4:		return "ec_sec4.msh";
		case NM_STADIUM_CORNER:		return "ec_corn.msh";
		case NM_STADIUM_WALL60:		return "ec_nar.msh";
		};
	case 1:		// zeppelin
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "zep_sec1.msh";
//		case NM_STADIUM_WALL2:		return "zep_sec2.msh";
//		case NM_STADIUM_WALL3:		return "zep_sec3.msh";
//		case NM_STADIUM_WALL4:		return "zep_sec4.msh";
//		case NM_STADIUM_CORNER:		return "zep_corn.msh";
//		case NM_STADIUM_WALL60:		return "zep_nar.msh";
		case NM_STADIUM_WALL1:		return "zp_sec1.msh";
		case NM_STADIUM_WALL2:		return "zp_sec2.msh";
		case NM_STADIUM_WALL3:		return "zp_sec3.msh";
		case NM_STADIUM_WALL4:		return "zp_sec4.msh";
		case NM_STADIUM_CORNER:		return "zp_corn.msh";
		case NM_STADIUM_WALL60:		return "zp_nar.msh";
		};
	case 2:		// hospital
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "hosp_sc1.msh";
//		case NM_STADIUM_WALL2:		return "hosp_sc2.msh";
//		case NM_STADIUM_WALL3:		return "hosp_sc3.msh";
//		case NM_STADIUM_WALL4:		return "hosp_sc4.msh";
//		case NM_STADIUM_CORNER:		return "hosp_cor.msh";
//		case NM_STADIUM_WALL60:		return "hosp_nar.msh";
		case NM_STADIUM_WALL1:		return "ho_sec1.msh";
		case NM_STADIUM_WALL2:		return "ho_sec2.msh";
		case NM_STADIUM_WALL3:		return "ho_sec3.msh";
		case NM_STADIUM_WALL4:		return "ho_sec4.msh";
		case NM_STADIUM_CORNER:		return "ho_corn.msh";
		case NM_STADIUM_WALL60:		return "ho_nar.msh";
		};
	case 3:		// ship
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "sea_sec1.msh";
//		case NM_STADIUM_WALL2:		return "sea_sec2.msh";
//		case NM_STADIUM_WALL3:		return "sea_sec3.msh";
//		case NM_STADIUM_WALL4:		return "sea_sec4.msh";
//		case NM_STADIUM_CORNER:		return "sea_corn.msh";
//		case NM_STADIUM_WALL60:		return "sea_nar.msh";
		case NM_STADIUM_WALL1:		return "sw_sec1.msh";
		case NM_STADIUM_WALL2:		return "sw_sec2.msh";
		case NM_STADIUM_WALL3:		return "sw_sec3.msh";
		case NM_STADIUM_WALL4:		return "sw_sec4.msh";
		case NM_STADIUM_CORNER:		return "sw_corn.msh";
		case NM_STADIUM_WALL60:		return "sw_nar.msh";
		};
	case 4:		// rehab
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "reh_sec1.msh";
//		case NM_STADIUM_WALL2:		return "reh_sec2.msh";
//		case NM_STADIUM_WALL3:		return "reh_sec3.msh";
//		case NM_STADIUM_WALL4:		return "reh_sec4.msh";
//		case NM_STADIUM_CORNER:		return "reh_corn.msh";
//		case NM_STADIUM_WALL60:		return "reh_nar.msh";
		case NM_STADIUM_WALL1:		return "re_sec1.msh";
		case NM_STADIUM_WALL2:		return "re_sec2.msh";
		case NM_STADIUM_WALL3:		return "re_sec3.msh";
		case NM_STADIUM_WALL4:		return "re_sec4.msh";
		case NM_STADIUM_CORNER:		return "re_corn.msh";
		case NM_STADIUM_WALL60:		return "re_nar.msh";
		};
	case 5:		// mining
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "mine_sc1.msh";
//		case NM_STADIUM_WALL2:		return "mine_sc2.msh";
//		case NM_STADIUM_WALL3:		return "mine_sc3.msh";
//		case NM_STADIUM_WALL4:		return "mine_sc4.msh";
//		case NM_STADIUM_CORNER:		return "mine_cor.msh";
//		case NM_STADIUM_WALL60:		return "mine_nar.msh";
		case NM_STADIUM_WALL1:		return "mi_sec1.msh";
		case NM_STADIUM_WALL2:		return "mi_sec2.msh";
		case NM_STADIUM_WALL3:		return "mi_sec3.msh";
		case NM_STADIUM_WALL4:		return "mi_sec4.msh";
		case NM_STADIUM_CORNER:		return "mi_corn.msh";
		case NM_STADIUM_WALL60:		return "mi_nar.msh";
		};
	case 6:		// deathrow
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "death_s1.msh";
//		case NM_STADIUM_WALL2:		return "death_s2.msh";
//		case NM_STADIUM_WALL3:		return "death_s3.msh";
//		case NM_STADIUM_WALL4:		return "death_s4.msh";
//		case NM_STADIUM_CORNER:		return "death_cn.msh";
//		case NM_STADIUM_WALL60:		return "death_n.msh";
		case NM_STADIUM_WALL1:		return "dr_sec1.msh";
		case NM_STADIUM_WALL2:		return "dr_sec2.msh";
		case NM_STADIUM_WALL3:		return "dr_sec3.msh";
		case NM_STADIUM_WALL4:		return "dr_sec4.msh";
		case NM_STADIUM_CORNER:		return "dr_corn.msh";
		case NM_STADIUM_WALL60:		return "dr_nar.msh";
		};
	case 7:		// sport
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "spt_sec1.msh";
//		case NM_STADIUM_WALL2:		return "spt_sec2.msh";
//		case NM_STADIUM_WALL3:		return "spt_sec3.msh";
//		case NM_STADIUM_WALL4:		return "spt_sec4.msh";
//		case NM_STADIUM_CORNER:		return "spt_corn.msh";
//		case NM_STADIUM_WALL60:		return "spt_nar.msh";
		case NM_STADIUM_WALL1:		return "sp_sec1.msh";
		case NM_STADIUM_WALL2:		return "sp_sec2.msh";
		case NM_STADIUM_WALL3:		return "sp_sec3.msh";
		case NM_STADIUM_WALL4:		return "sp_sec4.msh";
		case NM_STADIUM_CORNER:		return "sp_corn.msh";
		case NM_STADIUM_WALL60:		return "sp_nar.msh";
		};
	case 8:		// military
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "mil_sec1.msh";
//		case NM_STADIUM_WALL2:		return "mil_sec2.msh";
//		case NM_STADIUM_WALL3:		return "mil_sec3.msh";
//		case NM_STADIUM_WALL4:		return "mil_sec4.msh";
//		case NM_STADIUM_CORNER:		return "mil_corn.msh";
//		case NM_STADIUM_WALL60:		return "mil_nar.msh";
		case NM_STADIUM_WALL1:		return "ml_sec1.msh";
		case NM_STADIUM_WALL2:		return "ml_sec2.msh";
		case NM_STADIUM_WALL3:		return "ml_sec3.msh";
		case NM_STADIUM_WALL4:		return "ml_sec4.msh";
		case NM_STADIUM_CORNER:		return "ml_corn.msh";
		case NM_STADIUM_WALL60:		return "ml_nar.msh";
		};
	case 9:		// alpha star
		switch(sn)
		{
//		case NM_STADIUM_WALL1:		return "als_sec1.msh";
//		case NM_STADIUM_WALL2:		return "als_sec2.msh";
//		case NM_STADIUM_WALL3:		return "als_sec3.msh";
//		case NM_STADIUM_WALL4:		return "als_sec4.msh";
//		case NM_STADIUM_CORNER:		return "als_corn.msh";
//		case NM_STADIUM_WALL60:		return "als_narr.msh";
		case NM_STADIUM_WALL1:		return "as_sec1.msh";
		case NM_STADIUM_WALL2:		return "as_sec2.msh";
		case NM_STADIUM_WALL3:		return "as_sec3.msh";
		case NM_STADIUM_WALL4:		return "as_sec4.msh";
		case NM_STADIUM_CORNER:		return "as_corn.msh";
		case NM_STADIUM_WALL60:		return "as_nar.msh";
		};
	case 10:		// bonus
		switch(sn)
		{
		case NM_STADIUM_WALL1:		return "lm_sec1.msh";
		case NM_STADIUM_WALL2:		return "lm_sec2.msh";
		case NM_STADIUM_WALL3:		return "lm_sec3.msh";
		case NM_STADIUM_WALL4:		return "lm_sec4.msh";
		case NM_STADIUM_CORNER:		return "lm_corn.msh";
//		case NM_STADIUM_WALL60:		return "lm_secn.msh";
		case NM_STADIUM_WALL60:		return "lm_nar.msh";
		};
	}

	return "";
}

//******************************************************************************************
CMesh	*CPCGame::GetNumberedMesh(SINT n)
{
	SINT	prison_no = mLevelData.mIVars[IV_PRISON_NUMBER];

	switch (n)
	{
	case NM_STADIUM_WALL1:	
	case NM_STADIUM_WALL2:	
	case NM_STADIUM_WALL3:	
	case NM_STADIUM_WALL4:	
	case NM_STADIUM_CORNER:	
	case NM_STADIUM_WALL60:	
		return CMesh::GetMesh(get_stadium_name(n, prison_no), "");

	default:
		return NULL;
	}
}
#ifdef LINK_EDITOR
//******************************************************************************************
void	CPCGame::AddPSXNumberedMeshes()
{
	SINT	c0;
	
	for (c0 = 0; c0 < GetNumNumberedMeshes(); c0 ++)
	{
//		mMeshAccumulator.AddMesh(GetNumberedMesh(c0));
		GetNumberedMesh(c0)->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	}
}

//******************************************************************************************
void	CPCGame::WritePSXNumberedMeshReferences(CMEMBUFFER &mem, CMeshAccumulator &ma)
{
	// write out numbered mesh references
	SINT	i, c0, n;

	// number of meshes
	n = GetNumNumberedMeshes();
	mem.Write(&n, 4);

	// and the global meshes
	for (c0 = 0; c0 < n; c0 ++)
	{
		i = ma.GetMeshIndex(GetNumberedMesh(c0));
		mem.Write(&i, 4);
	}
}

//******************************************************************************************
// Write out PSX level file.
UINT	CID_BPLV	= MKID("BPLV");


BOOL	CPCGame::SavePSXLevel(SINT levelno)
{
	CMEMBUFFER	mem;
	mem.InitFromMem();

	// write out header
	mem.Write(&CID_BPLV, 4);

	// relocate global meshes
	RelocateGlobalMeshes();

	// write out the data that would be in the PC level
	mem.Write(&CID_LEVD, 4);
	mLevelData.Write(mem);

	mem.Write(&CID_MAP, 4);
	MAP.WritePSX(mem);

	// now the psx specific stuff


	// initialise the accumulators
	mTextureAccumulator.Init();
	mMeshAccumulator.Init();
	mThingAccumulator.Init();

	// ****************
	// First accumulate resources - textures, meshes, sounds  etc.

	MAP.AccumulatePSXResources(mTextureAccumulator, mMeshAccumulator);
	WORLD.AccumulatePSXResources(mTextureAccumulator, mMeshAccumulator, mThingAccumulator);
	ENGINE.AccumulatePSXResources(mTextureAccumulator, mMeshAccumulator);
	
	// add global stuff
	AddPSXGlobalTextures();
	AddPSXNumberedMeshes();
	
	// ****************
	// Then compile the relevant lists
	
	// Textures
	mTextureAccumulator.BuildTPage();	// Build the texture page
	mTextureAccumulator.Write(mem);		// write out the TPage

	// Precompile the meshes
	mMeshAccumulator.CompileAndWriteMeshes(mTextureAccumulator, mem);
	// write out default mesh reference.
	ENGINE.WritePSXMeshReferences(mem, mMeshAccumulator);
	GAME.WritePSXNumberedMeshReferences(mem, mMeshAccumulator);

	// Thing decriptions
	mThingAccumulator.CompileAndWrite(mTextureAccumulator, mMeshAccumulator, mem);
	
	// global mesh stuff
	ENGINE.ClearRelocatedMeshes();
	

	// ****************
	// Finally, the World.
	
	mem.Write(&CID_WRLD, 4);
	WORLD.Write(mem, FALSE);
	
	// ****************
	// Now write out data	
	
	
	// write out data
	char	base_name[256];
	SINT	ln;
	if (levelno == -1) // use current level
		ln = mLevelNo;
	else
		ln = levelno;

	SINT a, b;

	a = (ln / 100) * 100;
	b = (((ln - a) / 25) * 25) + a;

	sprintf(base_name, "MoHoData\\levels\\%03dTO%03d\\%03dTO%03d\\LEVEL%03d", a, a + 99, b, b + 24, ln);

	char	name[256];

	// write texture page dump
	sprintf(name, "MoHoData\\levelTGA\\%03d.tga", ln);
	mTextureAccumulator.WriteDump(name);
	
	sprintf(name, "%s.dat", base_name);
	return mem.Close(name);	

}

//******************************************************************************************
UINT	CID_FEPS	= MKID("FEPS");

BOOL	CPCGame::SavePSXPrisonSelectData()
{
	CMEMBUFFER	mem;
	mem.InitFromMem();

	// write out header
	mem.Write(&CID_FEPS, 4);

	mTextureAccumulator.Init();

	mTextureAccumulator.AddTexture("frontend\\1back.tga"		, TATT_NUMBERED, FEPST_BACK		, -1, -1, BD_8);
	mTextureAccumulator.AddTexture("frontend\\menpsxa.tga"		, TATT_NUMBERED, FEPST_TITLEA	, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\menpsxb.tga"		, TATT_NUMBERED, FEPST_TITLEB	, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\selchar.tga"		, TATT_NUMBERED, FEPST_CHARGRIDB, -1, -1, BD_8, 0, FALSE);
	mTextureAccumulator.AddTexture("frontend\\1butbitl.tga"		, TATT_NUMBERED, FEPST_BOXL		, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\1butbitm.tga"		, TATT_NUMBERED, FEPST_BOXM		, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\1butbitr.tga"		, TATT_NUMBERED, FEPST_BOXR		, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_eco.tga"		, TATT_NUMBERED, FEPST_PR_ECO		, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_zeps.tga"		, TATT_NUMBERED, FEPST_PR_ZEPPELIN	, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_hosp.tga"		, TATT_NUMBERED, FEPST_PR_HOSPITAL	, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_shipris.tga"	, TATT_NUMBERED, FEPST_PR_SHIP		, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_rehab.tga"		, TATT_NUMBERED, FEPST_PR_REHAB		, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_mine.tga"		, TATT_NUMBERED, FEPST_PR_MINING	, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_deaths.tga"	, TATT_NUMBERED, FEPST_PR_DEATHROW	, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_sport.tga"		, TATT_NUMBERED, FEPST_PR_SPORTS	, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_milis.tga"		, TATT_NUMBERED, FEPST_PR_MILITARY	, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_alpha.tga"		, TATT_NUMBERED, FEPST_PR_ALPHASTAR	, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ps_bonus.tga"		, TATT_NUMBERED, FEPST_PR_BONUS		, -1, -1, BD_8, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_king.tga"		, TATT_NUMBERED, FEPST_LT_KING_OF_THE_HILL	, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_rollin.tga"	, TATT_NUMBERED, FEPST_LT_LAST_MAN_ROLLING	, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_gaunt.tga"		, TATT_NUMBERED, FEPST_LT_RUN_THE_GAUNTLET	, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_purs.tga"		, TATT_NUMBERED, FEPST_LT_PURSUIT			, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_race.tga"		, TATT_NUMBERED, FEPST_LT_RACE				, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_trick.tga"		, TATT_NUMBERED, FEPST_LT_TRICK				, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_power.tga"		, TATT_NUMBERED, FEPST_LT_POWERBALL			, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_golf.tga"		, TATT_NUMBERED, FEPST_LT_GOLF				, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_pin.tga"		, TATT_NUMBERED, FEPST_LT_PINBALL			, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_speed.tga"		, TATT_NUMBERED, FEPST_LT_SBALL				, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\ar_shad.tga"		, TATT_NUMBERED, FEPST_LT_SHADOW			, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\qualify.tga"		, TATT_NUMBERED, FEPST_QUALIFY				, -1, -1, BD_4);
	mTextureAccumulator.AddTexture("frontend\\gold.tga"			, TATT_NUMBERED, FEPST_GOLD					, -1, -1, BD_4);
	mTextureAccumulator.AddTexture("frontend\\spark.tga"		, TATT_NUMBERED, FEPST_SPARK				, -1, -1, BD_4, 1, TRUE);
	mTextureAccumulator.AddTexture("frontend\\fram_pt.tga"		, TATT_NUMBERED, FEPST_BORD_PT		, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\fram_pb.tga"		, TATT_NUMBERED, FEPST_BORD_PB		, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\fram_pl.tga"		, TATT_NUMBERED, FEPST_BORD_PL		, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\fram_pr.tga"		, TATT_NUMBERED, FEPST_BORD_PR		, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\fram_ctl.tga"		, TATT_NUMBERED, FEPST_BORD_CTL		, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\fram_ctr.tga"		, TATT_NUMBERED, FEPST_BORD_CTR		, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\framcbl.tga"		, TATT_NUMBERED, FEPST_BORD_CBL		, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\framc_cbr.tga"	, TATT_NUMBERED, FEPST_BORD_CBR		, -1, -1, BD_4, 0, TRUE);
	mTextureAccumulator.AddTexture("frontend\\clip1.tga"		, TATT_NUMBERED, FEPST_LEVEL_CLIP_L		, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\clip2.tga"		, TATT_NUMBERED, FEPST_LEVEL_CLIP_R		, -1, -1, BD_8, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\arrow_l.tga"		, TATT_NUMBERED, FEPST_ARROW_L		, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\arrow_r.tga"		, TATT_NUMBERED, FEPST_ARROW_R		, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\arrow_u.tga"		, TATT_NUMBERED, FEPST_ARROW_U		, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\arrow_d.tga"		, TATT_NUMBERED, FEPST_ARROW_D		, -1, -1, BD_4, 2, TRUE, TRUE, TRUE);
	mTextureAccumulator.AddTexture("frontend\\qualify2.tga"		, TATT_NUMBERED, FEPST_QUALIFY2				, -1, -1, BD_4);
	mTextureAccumulator.AddTexture("frontend\\draw.tga"			, TATT_NUMBERED, FEPST_DRAW					, -1, -1, BD_4);
	mTextureAccumulator.AddTexture("frontend\\goldlt.tga"		, TATT_NUMBERED, FEPST_LOST_TIME			, -1, -1, BD_4);
	mTextureAccumulator.AddTexture("frontend\\halo.tga"			, TATT_NUMBERED, FEPST_HALO			, -1, -1, BD_8, 0,TRUE);

	mTextureAccumulator.AddTexture("fonts\\fnt1eng.tga", TATT_FONT, 0, 256, 128);
	mTextureAccumulator.AddTexture("fonts\\fnt2eng.tga", TATT_FONT, 1, 128, 48);
	mTextureAccumulator.AddTexture("fonts\\fnt3eng.tga", TATT_FONT, 2, 256, 128);

	mMeshAccumulator.Init();
	CMesh	*mesh_angel		= CMesh::GetMesh("Angel_FE.msh" , "");
	CMesh	*mesh_apostle	= CMesh::GetMesh("Apostle_FE.msh" , "");
	CMesh	*mesh_benny		= CMesh::GetMesh("Benny_FE.msh" , "");
	CMesh	*mesh_doctor	= CMesh::GetMesh("Doctor_FE.msh" , "");
	CMesh	*mesh_lockdown	= CMesh::GetMesh("Lockdown_FE.msh" , "");
	CMesh	*mesh_sophie	= CMesh::GetMesh("Sophie_FE.msh" , "");
	
	if (mesh_angel)		mesh_angel->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_apostle)	mesh_apostle->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_benny)		mesh_benny->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_doctor)	mesh_doctor->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_lockdown)	mesh_lockdown->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_sophie)	mesh_sophie->AccumulateResources(mTextureAccumulator, mMeshAccumulator);

	CMesh	*mesh_flag_uk = CMesh::GetMesh("flag_uk.msh", "");
	CMesh	*mesh_flag_fr = CMesh::GetMesh("flag_fr.msh", "");
	CMesh	*mesh_flag_gr = CMesh::GetMesh("flag_gr.msh", "");
	CMesh	*mesh_flag_sp = CMesh::GetMesh("flag_sp.msh", "");
	CMesh	*mesh_flag_it = CMesh::GetMesh("flag_it.msh", "");

	if (mesh_flag_uk)	mesh_flag_uk->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_flag_fr)	mesh_flag_fr->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_flag_gr)	mesh_flag_gr->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_flag_sp)	mesh_flag_sp->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
	if (mesh_flag_it)	mesh_flag_it->AccumulateResources(mTextureAccumulator, mMeshAccumulator);
		
	mTextureAccumulator.BuildTPage();	// Build the texture page
	mTextureAccumulator.Write(mem);		// write out the TPage

	// Write Mesh Stuff
	mMeshAccumulator.CompileAndWriteMeshes(mTextureAccumulator, mem);
	// write references
	SINT	i0 = 0, i1 = 11, mn;
	mem.Write(&i0, 4);	// default mesh
	mem.Write(&i0, 4);	// num global meshes

	mem.Write(&i1, 4);  // num numbered meshes

//	mn = mMeshAccumulator.GetMeshIndex(mesh_world);
//	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_angel);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_apostle);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_benny);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_lockdown);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_sophie);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_doctor);
	mem.Write(&mn, 4);

	mn = mMeshAccumulator.GetMeshIndex(mesh_flag_uk);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_flag_fr);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_flag_gr);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_flag_sp);
	mem.Write(&mn, 4);
	mn = mMeshAccumulator.GetMeshIndex(mesh_flag_it);
	mem.Write(&mn, 4);

	// write out data
	char	base_name[256];
	char	name[256];

	sprintf(base_name, "MoHoData\\FrontEnd\\PriSel");

	// write texture page dump
	sprintf(name, "%s.tga", base_name);
	mTextureAccumulator.WriteDump(name);

	sprintf(name, "%s.dat", base_name);
	return mem.Close(name);	
}

//******************************************************************************************
BOOL	CPCGame::SavePSXFrontEndData()
{
	BOOL	ret = TRUE;

	ret &= SavePSXPrisonSelectData();

	return ret;
}
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CPCGame::AddDCGlobalTextures()
{
	// add font textures
	mTextureAccumulatorDC.AddTexture("fonts\\fnt1eng.tga", TADCTT_FONT, 0, 256, 128);
	mTextureAccumulatorDC.AddTexture("fonts\\fnt2eng.tga", TADCTT_FONT, 1, 128, 64);
	mTextureAccumulatorDC.AddTexture("fonts\\fnt3eng.tga", TADCTT_FONT, 2, 256, 128);

	// stuff
	mTextureAccumulatorDC.AddTexture("Water.tga" , TADCTT_NUMBERED, TN_WATER      , -1, -1, 0, TRUE);
}

//******************************************************************************************
void	CPCGame::AddDCNumberedMeshes()
{
	SINT	c0;
	
	for (c0 = 0; c0 < GetNumNumberedMeshes(); c0 ++)
	{
//		mMeshAccumulator.AddMesh(GetNumberedMesh(c0));
		GetNumberedMesh(c0)->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	}
}

//******************************************************************************************
void	CPCGame::WriteDCNumberedMeshReferences(CMEMBUFFER &mem, CMeshAccumulatorDC &ma)
{
	// write out numbered mesh references
	SINT	i, c0, n;

	// number of meshes
	n = GetNumNumberedMeshes();
	mem.Write(&n, 4);

	// and the global meshes
	for (c0 = 0; c0 < n; c0 ++)
	{
		i = ma.GetMeshIndex(GetNumberedMesh(c0));
		mem.Write(&i, 4);
	}
}

//******************************************************************************************
// Write out DC level file.
//UINT	CID_BPLV	= MKID("BPLV");


BOOL	CPCGame::SaveDCLevel(SINT levelno)
{
	CMEMBUFFER	mem;
	mem.InitFromMem();

	// write out header
	mem.Write(&CID_BPLV, 4);

	// relocate global meshes
	RelocateGlobalMeshes();

	// write out the data that would be in the PC level
	mem.Write(&CID_LEVD, 4);
	mLevelData.Write(mem);

	mem.Write(&CID_MAP, 4);
	MAP.WriteDC(mem);

	// now the psx specific stuff


	// initialise the accumulators
	mTextureAccumulatorDC.Init();
	mMeshAccumulatorDC.Init();
	mThingAccumulatorDC.Init();

	// ****************
	// First accumulate resources - textures, meshes, sounds  etc.

	MAP.AccumulateDCResources(mTextureAccumulatorDC, mMeshAccumulatorDC);
	WORLD.AccumulateDCResources(mTextureAccumulatorDC, mMeshAccumulatorDC, mThingAccumulatorDC);
	ENGINE.AccumulateDCResources(mTextureAccumulatorDC, mMeshAccumulatorDC);
	
	// add global stuff
	AddDCGlobalTextures();
	AddDCNumberedMeshes();
	
	// ****************
	// Then compile the relevant lists
	
	// Textures
	mTextureAccumulatorDC.BuildTPage();	// Build the texture page
	mTextureAccumulatorDC.Write(mem);		// write out the TPage

	// Precompile the meshes
	mMeshAccumulatorDC.CompileAndWriteMeshes(mTextureAccumulatorDC, mem);
	// write out default mesh reference.
	ENGINE.WriteDCMeshReferences(mem, mMeshAccumulatorDC);
	GAME.WriteDCNumberedMeshReferences(mem, mMeshAccumulatorDC);

	// Thing decriptions
	mThingAccumulatorDC.CompileAndWrite(mTextureAccumulatorDC, mMeshAccumulatorDC, mem);
	
	// global mesh stuff
	ENGINE.ClearRelocatedMeshes();
	

	// ****************
	// Finally, the World.
	
	mem.Write(&CID_WRLD, 4);
	WORLD.Write(mem, FALSE);
	
	// ****************
	// Now write out data	
	
	
	// write out data
	char	base_name[256];
	SINT	ln;
	if (levelno == -1) // use current level
		ln = mLevelNo;
	else
		ln = levelno;

	SINT a, b;

	a = (ln / 100) * 100;
	b = (((ln - a) / 25) * 25) + a;

	sprintf(base_name, "DCData\\levels\\%03dTO%03d\\%03dTO%03d\\LEVEL%03d", a, a + 99, b, b + 24, ln);

	char	name[256];

	// write texture page dump
	sprintf(name, "DCData\\levelTGA\\%03d", ln);
	mTextureAccumulatorDC.WriteDump(name);
	
	sprintf(name, "%s.dat", base_name);
	return mem.Close(name);	

}

//******************************************************************************************
//UINT	CID_FEPS	= MKID("FEPS");

BOOL	CPCGame::SaveDCPrisonSelectData(SINT n)
{
	CMEMBUFFER	mem;
	mem.InitFromMem();

	// write out header
	mem.Write(&CID_FEPS, 4);

	mTextureAccumulatorDC.Init();

	mTextureAccumulatorDC.AddTexture("frontend\\back1.tga"		, TADCTT_NUMBERED, FEDCPST_BACK1			, -1, -1);
	mTextureAccumulatorDC.AddTexture("frontend\\back2.tga"		, TADCTT_NUMBERED, FEDCPST_BACK2			, -1, -1);
 	mTextureAccumulatorDC.AddTexture("frontend\\back3.tga"		, TADCTT_NUMBERED, FEDCPST_BACK3			, -1, -1);
	mTextureAccumulatorDC.AddTexture("frontend\\back4.tga"		, TADCTT_NUMBERED, FEDCPST_BACK4			, -1, -1);
	mTextureAccumulatorDC.AddTexture("frontend\\back5.tga"		, TADCTT_NUMBERED, FEDCPST_BACK5			, -1, -1);
	mTextureAccumulatorDC.AddTexture("frontend\\back6.tga"		, TADCTT_NUMBERED, FEDCPST_BACK6			, -1, -1);

	switch (n)
	{
	case 0:		// Moho, normal
		mTextureAccumulatorDC.AddTexture("frontend\\ms1.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE1		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\ms2.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE2		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\ms3.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE3		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\ms4.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE4		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\ms5.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE5		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\ms6.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE6		, -1, -1);
		break;
	case 1:		// Moho, gold
		mTextureAccumulatorDC.AddTexture("frontend\\mgs1.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE1		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\mgs2.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE2		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\mgs3.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE3		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\mgs4.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE4		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\mgs5.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE5		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\mgs6.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE6		, -1, -1);
		break;
	case 2:		// Ball Breakers, normal
		mTextureAccumulatorDC.AddTexture("frontend\\bbs1.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE1		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbs2.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE2		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbs3.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE3		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbs4.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE4		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbs5.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE5		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbs6.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE6		, -1, -1);
		break;
	case 3:		// Ball Breakers, gold
		mTextureAccumulatorDC.AddTexture("frontend\\bbgs1.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE1		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbgs2.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE2		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbgs3.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE3		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbgs4.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE4		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbgs5.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE5		, -1, -1);
		mTextureAccumulatorDC.AddTexture("frontend\\bbgs6.tga"		, TADCTT_NUMBERED, FEDCPST_TITLE6		, -1, -1);
		break;
	}

	
	mTextureAccumulatorDC.AddTexture("frontend\\lcd1.tga"		, TADCTT_NUMBERED, FEDCPST_LCD1				, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\lcd2.tga"		, TADCTT_NUMBERED, FEDCPST_LCD2				, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\lcd3.tga"		, TADCTT_NUMBERED, FEDCPST_LCD3				, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\lcd4.tga"		, TADCTT_NUMBERED, FEDCPST_LCD4				, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\lcd5.tga"		, TADCTT_NUMBERED, FEDCPST_LCD5				, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\selchar.tga"	, TADCTT_NUMBERED, FEDCPST_CHARGRIDB			, -1, -1, 0, FALSE);
	mTextureAccumulatorDC.AddTexture("frontend\\1butbitl.tga"	, TADCTT_NUMBERED, FEDCPST_BOXL				, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\1butbitm.tga"	, TADCTT_NUMBERED, FEDCPST_BOXM				, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\1butbitr.tga"	, TADCTT_NUMBERED, FEDCPST_BOXR				, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_eco.tga"		, TADCTT_NUMBERED, FEDCPST_PR_ECO				, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_zeps.tga"	, TADCTT_NUMBERED, FEDCPST_PR_ZEPPELIN		, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_hosp.tga"	, TADCTT_NUMBERED, FEDCPST_PR_HOSPITAL		, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_shipris.tga"	, TADCTT_NUMBERED, FEDCPST_PR_SHIP			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_rehab.tga"	, TADCTT_NUMBERED, FEDCPST_PR_REHAB			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_mine.tga"	, TADCTT_NUMBERED, FEDCPST_PR_MINING			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_deaths.tga"	, TADCTT_NUMBERED, FEDCPST_PR_DEATHROW		, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_sport.tga"	, TADCTT_NUMBERED, FEDCPST_PR_SPORTS			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_milis.tga"	, TADCTT_NUMBERED, FEDCPST_PR_MILITARY		, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_alpha.tga"	, TADCTT_NUMBERED, FEDCPST_PR_ALPHASTAR		, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ps_bonus.tga"	, TADCTT_NUMBERED, FEDCPST_PR_BONUS			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_king.tga"	, TADCTT_NUMBERED, FEDCPST_LT_KING_OF_THE_HILL, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_rollin.tga"	, TADCTT_NUMBERED, FEDCPST_LT_LAST_MAN_ROLLING, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_gaunt.tga"	, TADCTT_NUMBERED, FEDCPST_LT_RUN_THE_GAUNTLET, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_purs.tga"	, TADCTT_NUMBERED, FEDCPST_LT_PURSUIT			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_race.tga"	, TADCTT_NUMBERED, FEDCPST_LT_RACE			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_trick.tga"	, TADCTT_NUMBERED, FEDCPST_LT_TRICK			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_power.tga"	, TADCTT_NUMBERED, FEDCPST_LT_POWERBALL		, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_golf.tga"	, TADCTT_NUMBERED, FEDCPST_LT_GOLF			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_pin.tga"		, TADCTT_NUMBERED, FEDCPST_LT_PINBALL			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_speed.tga"	, TADCTT_NUMBERED, FEDCPST_LT_SBALL			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\ar_shad.tga"	, TADCTT_NUMBERED, FEDCPST_LT_SHADOW			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\qualify.tga"	, TADCTT_NUMBERED, FEDCPST_QUALIFY			, -1, -1);
	mTextureAccumulatorDC.AddTexture("frontend\\gold.tga"		, TADCTT_NUMBERED, FEDCPST_GOLD				, -1, -1);
	mTextureAccumulatorDC.AddTexture("frontend\\spark.tga"		, TADCTT_NUMBERED, FEDCPST_SPARK				, -1, -1, 1, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\frampt.tga"		, TADCTT_NUMBERED, FEDCPST_BORD_PT			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\frampb.tga"		, TADCTT_NUMBERED, FEDCPST_BORD_PB			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\frampl.tga"		, TADCTT_NUMBERED, FEDCPST_BORD_PL			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\frampr.tga"		, TADCTT_NUMBERED, FEDCPST_BORD_PR			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\framctl.tga"	, TADCTT_NUMBERED, FEDCPST_BORD_CTL			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\framctr.tga"	, TADCTT_NUMBERED, FEDCPST_BORD_CTR			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\framcbl.tga"	, TADCTT_NUMBERED, FEDCPST_BORD_CBL			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\framcbr.tga"	, TADCTT_NUMBERED, FEDCPST_BORD_CBR			, -1, -1, 0, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\clipl.tga"		, TADCTT_NUMBERED, FEDCPST_LEVEL_CLIP_L		, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\clipr.tga"		, TADCTT_NUMBERED, FEDCPST_LEVEL_CLIP_R		, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\arrow_l.tga"	, TADCTT_NUMBERED, FEDCPST_ARROW_L			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\arrow_r.tga"	, TADCTT_NUMBERED, FEDCPST_ARROW_R			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\arrow_u.tga"	, TADCTT_NUMBERED, FEDCPST_ARROW_U			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\arrow_d.tga"	, TADCTT_NUMBERED, FEDCPST_ARROW_D			, -1, -1, 2, TRUE, TRUE, TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\draw.tga"		, TADCTT_NUMBERED, FEDCPST_DRAW				, -1, -1);
	mTextureAccumulatorDC.AddTexture("frontend\\goldlt.tga"		, TADCTT_NUMBERED, FEDCPST_LOST_TIME			, -1, -1);
	mTextureAccumulatorDC.AddTexture("frontend\\halo.tga"		, TADCTT_NUMBERED, FEDCPST_HALO				, -1, -1, 0,TRUE);
	mTextureAccumulatorDC.AddTexture("frontend\\alphawhite.tga"	, TADCTT_NUMBERED, FEDCPST_ALPHAWHITE		, -1, -1, 0,TRUE);

	mTextureAccumulatorDC.AddTexture("fonts\\fnt1eng.tga", TADCTT_FONT, 0, 256, 128);
	mTextureAccumulatorDC.AddTexture("fonts\\fnt2eng.tga", TADCTT_FONT, 1, 128, 64);
	mTextureAccumulatorDC.AddTexture("fonts\\fnt3eng.tga", TADCTT_FONT, 2, 256, 128);

	mMeshAccumulatorDC.Init();
	CMesh	*mesh_angel		= CMesh::GetMesh("Angel_FE.msh" , "");
	CMesh	*mesh_apostle	= CMesh::GetMesh("Apostle_FE.msh" , "");
	CMesh	*mesh_benny		= CMesh::GetMesh("Benny_FE.msh" , "");
	CMesh	*mesh_doctor	= CMesh::GetMesh("Doctor_FE.msh" , "");
	CMesh	*mesh_lockdown	= CMesh::GetMesh("Lockdown_FE.msh" , "");
	CMesh	*mesh_sophie	= CMesh::GetMesh("Sophie_FE.msh" , "");
	
	if (mesh_angel)		mesh_angel->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_apostle)	mesh_apostle->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_benny)		mesh_benny->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_doctor)	mesh_doctor->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_lockdown)	mesh_lockdown->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_sophie)	mesh_sophie->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);

	CMesh	*mesh_flag_uk = CMesh::GetMesh("flag_uk.msh", "");
	CMesh	*mesh_flag_fr = CMesh::GetMesh("flag_fr.msh", "");
	CMesh	*mesh_flag_gr = CMesh::GetMesh("flag_gr.msh", "");
	CMesh	*mesh_flag_sp = CMesh::GetMesh("flag_sp.msh", "");
	CMesh	*mesh_flag_it = CMesh::GetMesh("flag_it.msh", "");

	if (mesh_flag_uk)	mesh_flag_uk->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_flag_fr)	mesh_flag_fr->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_flag_gr)	mesh_flag_gr->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_flag_sp)	mesh_flag_sp->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
	if (mesh_flag_it)	mesh_flag_it->AccumulateResourcesDC(mTextureAccumulatorDC, mMeshAccumulatorDC);
		
	mTextureAccumulatorDC.BuildTPage();	// Build the texture page
	mTextureAccumulatorDC.Write(mem, TRUE);		// write out the TPage

	// Write Mesh Stuff
	mMeshAccumulatorDC.CompileAndWriteMeshes(mTextureAccumulatorDC, mem);
	// write references
	SINT	i0 = 0, i1 = 11, mn;
	mem.Write(&i0, 4);	// default mesh
	mem.Write(&i0, 4);	// num global meshes

	mem.Write(&i1, 4);  // num numbered meshes

//	mn = mMeshAccumulator.GetMeshIndex(mesh_world);
//	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_angel);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_apostle);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_benny);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_lockdown);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_sophie);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_doctor);
	mem.Write(&mn, 4);

	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_flag_uk);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_flag_fr);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_flag_gr);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_flag_sp);
	mem.Write(&mn, 4);
	mn = mMeshAccumulatorDC.GetMeshIndex(mesh_flag_it);
	mem.Write(&mn, 4);

	// write out data
	char	base_name[256];
	char	name[256];

	sprintf(base_name, "DCData\\FrontEnd\\PriSel%d", n);

	// write texture page dump
	sprintf(name, "%s.tga", base_name);
	mTextureAccumulatorDC.WriteDump(name);

	sprintf(name, "%s.dat", base_name);
	return mem.Close(name);	
}

//******************************************************************************************
UINT	CID_SCRN	= MKID("SCRN");

BOOL	CPCGame::SaveDCScreen(SINT n)
{
	CMEMBUFFER	mem;
	mem.InitFromMem();

	// write out header
	mem.Write(&CID_SCRN, 4);

	mTextureAccumulatorDC.Init();

	char	screen_name[300];
	get_screen_name(n, screen_name);

	SINT	c0;
	for (c0 = 0; c0 < 6; c0 ++)
	{
		char tname[400];
		sprintf(tname, "%s%d.TGA", screen_name, c0 + 1);
		mTextureAccumulatorDC.AddTexture(tname, TADCTT_NUMBERED, SCREENT_MAIN1 + c0	, -1, -1);
	};

	mTextureAccumulatorDC.AddTexture("fonts\\fnt1eng.tga", TADCTT_FONT, 0, 256, 128);
	mTextureAccumulatorDC.AddTexture("fonts\\fnt2eng.tga", TADCTT_FONT, 1, 128, 64);
	mTextureAccumulatorDC.AddTexture("fonts\\fnt3eng.tga", TADCTT_FONT, 2, 256, 128);

	mTextureAccumulatorDC.BuildTPage();	// Build the texture page
	mTextureAccumulatorDC.Write(mem, TRUE);		// write out the TPage

	// write out data
	char	base_name[256];
	char	name[256];

	sprintf(base_name, "DCData\\Screens\\%d", n);

	sprintf(name, "%s.dat", base_name);
	BOOL	foo = mem.Close(name);	

	// write texture page dump
	sprintf(name, "%s.tga", base_name);
	mTextureAccumulatorDC.WriteDump(name);

	return foo;
}

//******************************************************************************************
BOOL	CPCGame::SaveDCFrontEndData()
{
	BOOL	ret = TRUE;

	SINT	c0;
	for (c0 = 0; c0 < 11 + 4 + 11 + 1; c0 ++)
		ret &= SaveDCScreen(c0);

//	ret &= SaveDCScreen(26);

	for (c0 = 0; c0 < 4; c0 ++)
		ret &= SaveDCPrisonSelectData(c0);

	return ret;
}


//******************************************************************************************
void	CPCGame::WritePCFontDats()
{
	mTextureAccumulatorDC.Init();

	mTextureAccumulatorDC.AddTexture("fonts\\fnt1eng.tga", TADCTT_FONT, 0, 256, 128);
	mTextureAccumulatorDC.AddTexture("fonts\\fnt2eng.tga", TADCTT_FONT, 1, 128, 64);
	mTextureAccumulatorDC.AddTexture("fonts\\fnt3eng.tga", TADCTT_FONT, 2, 256, 128);

	mTextureAccumulatorDC.BuildTPage();	// Build the texture page

	mTextureAccumulatorDC.WriteFontDats("data\\frontend\\font");
}

//******************************************************************************************


#endif


// ----------------------------------------------------------------------------
// Snappy name!
void	CPCGame::ShowLoadingScreenWhilstWaitingForUserToPressSpace(SINT n)
{
	bool	Continue = true;
	bool	ShowPress = true;
	float	Then;

	while (Continue)
	{
		ShowLoadingScreen(n, false, ShowPress);

		Then = ENGINE.GetSysTimeFloat();
		while ((ENGINE.GetSysTimeFloat() - Then) < 0.5f)
		{
			PLATFORM.ProcessMessages();
			if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_SPACE]) & 0x80)
			{
				Continue = false;
				break;
			}
		}
					
		// Flip state.
		ShowPress = !ShowPress;
	}
}

//******************************************************************************************
void	CPCGame::ShowLoadingScreen(SINT n, bool ShowLoading, bool ShowPress)
{
	float xs, ys;
	xs = PLATFORM.ScreenWidthScale();
	ys = PLATFORM.ScreenHeightScale();

	LPDIRECT3DDEVICE7 device = PLATFORM.GetDPWin()->GetDevice();
	if (!device) return;

	if (ShowLoading)
	{
		// clear screen first
		if (device->BeginScene() == D3D_OK)
		{
			PLATFORM.GetDPWin()->Clear(0x00000000);
			device->EndScene();
		}
	}
	else
	{
		PLATFORM.GetDPWin()->Clear(0x00000000);
	}

	
	CTextureTL	*tex[6];
	SINT		c0;
	char		screen_name[300];

	get_screen_name(n, screen_name);

	for (c0 = 0; c0 < 6; c0 ++)
	{
		char tname[400];
		sprintf(tname, "%s%d.TGA", screen_name, c0 + 1);

		tex[c0] = CTextureTL::GetTexture(tname);

		if (!tex[c0])
			return;
	}

	char s[200];

	if (device->BeginScene() == D3D_OK)
	{
		ENGINE.SetupInitialRenderstates();

		//void	DrawSpritePC(CTextureTL* n, float px, float py, float z, float width, float height, D3DCOLOR col=0xffffffff, float scalex=1.0f, float scaley=1.0f, SINT flip = 0);

		/*
		LPDIRECTDRAWSURFACE7 refSurface;
		//PLATFORM.GetDPWin()->GetDirectDraw7()->GetGDISurface(&refSurface);
		refSurface = PLATFORM.GetDPWin()->GetPrimarySurface();

		RECT dest;
		dest.top	= 0;
		dest.left	= 0;
		dest.bottom	= 256 * ys;
		dest.right	= 256 * xs;
		
		// Top row.
		refSurface->Blt(&dest, tex[0]->mTexSurface[0]->GetSurface(), NULL, DDBLT_WAIT, NULL);
		
		dest.left = dest.right;
		dest.right += 256 * xs;
		refSurface->Blt(&dest, tex[1]->mTexSurface[0]->GetSurface(), NULL, DDBLT_WAIT, NULL);

		dest.left = dest.right;
		dest.right += 128 * xs;
		refSurface->Blt(&dest, tex[2]->mTexSurface[0]->GetSurface(), NULL, DDBLT_WAIT, NULL);

		// Bottom row.
		dest.top	= dest.bottom;
		dest.left	= 0;
		dest.bottom	+= 256 * ys;
		dest.right	= 256 * xs;
		refSurface->Blt(&dest, tex[3]->mTexSurface[0]->GetSurface(), NULL, DDBLT_WAIT, NULL);

		dest.left = dest.right;
		dest.right += 256 * xs;
		refSurface->Blt(&dest, tex[4]->mTexSurface[0]->GetSurface(), NULL, DDBLT_WAIT, NULL);

		dest.left = dest.right;
		dest.right += 128 * xs;
		refSurface->Blt(&dest, tex[5]->mTexSurface[0]->GetSurface(), NULL, DDBLT_WAIT, NULL);
		*/

		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();

		ENGINE.DrawSpritePC2(tex[0],     0,   0, 0.9f, 256, 256, 0x0fffffff, xs, ys);
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();
		ENGINE.DrawSpritePC2(tex[1],   256,   0, 0.9f, 256, 256, 0x0fffffff, xs, ys);
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();
		ENGINE.DrawSpritePC2(tex[2],   512,   0, 0.9f, 128, 256, 0x0fffffff, xs, ys);
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();
		ENGINE.DrawSpritePC2(tex[3],     0, 256, 0.9f, 256, 256, 0x0fffffff, xs, ys);
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();
		ENGINE.DrawSpritePC2(tex[4],   256, 256, 0.9f, 256, 256, 0x0fffffff, xs, ys);
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();
		ENGINE.DrawSpritePC2(tex[5],   512, 256, 0.9f, 128, 256, 0x0fffffff, xs, ys);

	
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();

		
		SINT	sl1, sl2;
		
		
		if ((n > 3) && (n < 26))
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
				//t4 = TEXT_HELPPC_GOLF;
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
				y1 = 190 * PCMULTY * ys;
				y2 = 206 * PCMULTY * ys;
			}
			else if (s[0] == '.')
			{
				y1 = 174 * PCMULTY * ys;
				y2 = 190 * PCMULTY * ys;
				y4 = 206 * PCMULTY * ys;
			}
			else if (t4 == -1)
			{
				y1 = 174 * PCMULTY * ys;
				y2 = 190 * PCMULTY * ys;
				y3 = 206 * PCMULTY * ys;
			}
			else
			{
				y1 = 158 * PCMULTY * ys;
				y2 = 174 * PCMULTY * ys;
				y3 = 190 * PCMULTY * ys;
				y4 = 206 * PCMULTY * ys;
			}


			CText::GetString(TEXT_LEVEL_TYPE, s);
			sl1 = ENGINE.GetStringSize(0, s);
			ENGINE.DrawShadowedString(0, s, 20 * PCMULTX    , y1    , 0x001f4f7f,0.05f,FALSE);

			CText::GetString(TEXT_OBJECTIVE, s);
			sl2 = ENGINE.GetStringSize(0, s);
			ENGINE.DrawShadowedString(0, s, 20 * PCMULTX    , y2    , 0x001f4f7f,0.1f,FALSE);

			float	ty;
			if (sl1 > sl2)
				ty = sl1 + (30 * PCMULTX);
			else
				ty = sl2 + (30 * PCMULTX);

			CText::GetString(t1, s);
			ENGINE.DrawShadowedString(0, s, ty    , y1    , 0x007f5f1f,0.15f,FALSE);

			CText::GetString(t2, s);
			ENGINE.DrawShadowedString(2, s, ty    , y2    , 0x001f7f3f,0.2f,FALSE);

			CText::GetString(t3, s);
			if (s[0] != '.')
			{
				ENGINE.DrawShadowedString(2, s, ty    , y3    , 0x001f7f3f,0.3f,FALSE);
			}

			if (t4 != -1)
			{
				CText::GetString(t4, s);
				ENGINE.DrawShadowedString(2, s, ty    , y4    , 0x007f7f7f,0.4f,FALSE);
			}

			CText::GetString(TEXT_PRN_ECO + ((n - 4) % 11), s);
			SINT	sl = ENGINE.GetStringSize(0, s);
			ENGINE.DrawShadowedString(0, s, (PLATFORM.ScreenWidthFull() / 2) - (sl / 2)    , 20 * PCMULTY   , 0x007f7f7f,0.5f,FALSE);

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
			ENGINE.DrawShadowedString(0, s, (PLATFORM.ScreenWidthFull() / 2) - (sl / 2)    , 36 * PCMULTY    , 0x00FFBE5E,0.1f,FALSE);
		}
	}
	
	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();

	//if (draw == 0) // Draw some text on the first screen
	if (	(ShowLoading)
		&&	(n < 26)
		)
	{
		CText::GetString(TEXT_LOADING, s);
		SINT	sl = ENGINE.GetStringSize(0, s);
		ENGINE.DrawShadowedString(0, s, (PLATFORM.ScreenWidthFull() / 2) - (sl / 2), 222 * PCMULTY *ys, 0x00FFFFFF);
	}

	if (ShowPress)
	{
		// Draw some text
		char text[512];
		CText::GetString(TEXT_PC_PRESS_SPACE, text);
		SINT 	font = GAME.GetLanguage() == 0 ? 0 : 2;
		SINT sl = ENGINE.GetStringSize(font, text);
		ENGINE.DrawShadowedString(font, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 225 * PCMULTY * PLATFORM.ScreenHeightScale(), 0x00FFFFFF);
	}

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();

	device->EndScene();
	

	ENGINE.Flip();
	// could probably free them....
}

#endif