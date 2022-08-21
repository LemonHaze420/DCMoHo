// $Header$

// $Log$
// Revision 1.12  2000-10-02 01:50:50+01  jcl
// <>
//
// Revision 1.11  2000-10-01 12:45:43+01  jcl
// <>
//
// Revision 1.10  2000-09-29 11:20:51+01  jcl
// <>
//
// Revision 1.9  2000-09-25 10:21:45+01  jjs
// Uncommented the Sound Init.
//
// Revision 1.8  2000-09-25 05:16:42+01  jcl
// <>
//
// Revision 1.7  2000-09-19 12:12:13+01  jcl
// <>
//
// Revision 1.6  2000-09-12 10:13:58+01  sds
// Big Merge - Should work for DC and PC
//
// Revision 1.5  2000-09-11 11:51:26+01  asd
// Line added to return a handle to the active window to sound init
//
// Revision 1.4  2000-09-01 17:10:58+01  are
// Can get out of the game now (if you win :o).
//
// Revision 1.3  2000-08-07 15:46:45+01  jjs
// DC and PC merged code.
//
// Revision 1.2  2000-07-11 10:11:43+01  jjs
// Put code back on seperate lines for ease of debugging.
//

#include	"Common.h"

#include	"Game.h"
#include	"Globals.h"
#include	"MemBuffer.h"
#include	"Profile.h"
#include	"Editors.h"
#include	"Primary.h"
#include	"Career.h"

#include	"GamePanel.h"


#if TARGET == PC
#include	"PCRenderData.h"
#elif TARGET == PSX
//!PSX #include "PSXRenderData.h"
#include	"PSXBuildType.h"
#elif TARGET == DC
#include	"DCBuildType.h"
#include   "maincode.h"
#endif


//******************************************************************************************

#if TARGET == PC
BOOL	CGame::Init(TTApp *app, CCLIParams &params)
#elif TARGET == PSX || TARGET == DC
BOOL	CGame::Init()
#endif

{

#if TARGET == PC
		mCLIParams = params;
#endif

#if TARGET == PC
	if (!(PLATFORM.Init(app))) 
		return FALSE;
#elif TARGET == PSX || TARGET == DC
	if (!(PLATFORM.Init()))	
		return FALSE;
#endif



	if (!(ENGINE.Init()))	    
		return FALSE;
	if (!(MAP.Init()))			
		return FALSE;
	if (!(WORLD.Init()))		
		return FALSE;
	if (!(GAMEPANEL.Init()))	
		return FALSE;
#if TARGET == PC		
	if (!(SOUND.Init(PLATFORM.GetDPWin()->GetHwnd())))	
	return FALSE;	
#endif
#if TARGET == DC
  if	(!(SOUND.Init()))	
		return FALSE;
#endif		


#if TARGET == PC
	InitRenderData();
#endif

#ifdef LINK_EDITOR
	if (!(EDITOR.Init()))
		return FALSE;
#endif

	if (!(CONTROLS.Init()))
		return FALSE;

#if ENABLE_PROFILER==1
	CProfiler::Init();
#endif

	Reset();
	mDraw2D	= TRUE;
	mDrawDebug = FALSE;
	mDrawProfiler = FALSE;
	mDrawCollisionInfo = FALSE;
	mDrawMapWhoInfo = FALSE;
	mTickCount = 0;
	mFirstTimeThrough = TRUE ;
	mDrawCharacterInfo = NULL ;

#if TARGET == PSX || TARGET == DC
	GAME.ShowSplashScreen();
#endif

	// init players
	SINT	c0;
	for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
		mPlayer[c0].mNumber = c0;

	mGameLevelData.Init() ;
	CAREER.Clear();
	MULTIPLAYER_CAREER.Clear();
	mMultiplayer = FALSE;
	ClearBestResult();
	ClearGolfShots();
	return TRUE;
}

//******************************************************************************************
void	CGame::Shutdown()
{
	CONTROLS.Shutdown();
#ifdef LINK_EDITOR
	EDITOR.Shutdown();
#endif
	WORLD.Shutdown();
	MAP.Shutdown();
	ENGINE.Shutdown();
	SOUND.Shutdown();
	PLATFORM.Shutdown();
}

//******************************************************************************************
void	CGame::Reset()
{
	mQuit = GQ_NONE;
	mGameTurn = -1;
	mGameStartTime = -1;

	SetGameState(GAME_STATE_STARTING) ;
	mGameStateCounter = 0;
	mGRI.mStartLightsOn = G0;
	mPaused = FALSE;
	mHelp = FALSE;
	mGamePanelActive = FALSE;
	mDrawCharacterInfo = NULL ;
	mFirstTimeThrough = TRUE ;
	mFrameIncrease = FALSE ;

	// golf
	SINT	c0;
	for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
	{
		mGolfShotNo[c0]				= 0;
		mGolfIsStationary[c0]		= TRUE;
		mGolfStationaryCount[c0]	= 0;
		mGolfOBCount[c0]			= -1;
		mGolfMovingCount[c0]		= 0;
		mGolfLastShot[c0]			= GVector(G(32), G(32), G0);
	}


	mCameras[0].Reset(0);
	mCameras[1].Reset(1);

	for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
		mPlayer[c0].Init();

	mGameLevelData.Init() ;

	//!CONTROLS - eertheilrulhgaieurlga
	mPlayer[0].Activate();
//	mPlayer[1].Activate();

	mCrowdHappiness = G0;
	mCrowdIntensity = -G1;
	SOUND.Reset();
}

//******************************************************************************************
void	CGame::TogglePause()
{
	mPaused = (!mPaused);

	if (mPaused)
		CONSOLE.AddString("Game paused");
	else
		CONSOLE.AddString("Game un-paused");
}


//******************************************************************************************
void	CGame::FrameIncrease()
{
	if (!mPaused) return ;
	mFrameIncrease = TRUE ;

	CONSOLE.AddString("Frame increase");
}

//******************************************************************************************
void	CGame::ToggleHelp()
{
	mHelp = (!mHelp);
}

//******************************************************************************************
void	CGame::ToggleGamePanel(SINT pn)
{
	if (!mGamePanelActive && ((GAMEPANEL.GetisGamePanelGoing()) == 0 || GAMEPANEL.GetisGamePanelGoing() > 25 ))
	{
/*		if ((mGameState == GAME_STATE_FADE_OUT) ||
			(mGameState == GAME_STATE_GAME_FINISHED))
		{
			if (!mMultiplayer)
			{
				ELevelResult res = PLAYER(0)->GetLevelResult();
				if (res == LR_WIN)
					return; // no gamepanel here...
			}
		}
*/
		mGamePanelActive = TRUE;
		if ((GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF) ||
			(mMultiplayer))
#if TARGET == DC
			GAMEPANEL.Activate(GPT_INGAME2, pn);
#else
			GAMEPANEL.Activate(GPT_INGAME2);
#endif
		else
#if TARGET == DC
			GAMEPANEL.Activate(GPT_INGAME, pn);
#else
			GAMEPANEL.Activate(GPT_INGAME);
#endif
	}
	else
	{
		GAMEPANEL.DeActivate();
		mGamePanelActive = FALSE;
	}
}

//******************************************************************************************

//!aaaaaaargh  this shouldn't be here!

// Had a fiddle with this.. (quicker method) DAMO! (Shoot me if it's wrong)

DWORD	interp_colour(DWORD c1, DWORD c2, float v)
{
	SINT a1 = (c1 & 0xff000000) >> 24;
	SINT r1 = (c1 & 0x00ff0000) >> 16;
	SINT g1 = (c1 & 0x0000ff00) >> 8;
	SINT b1 = c1 & 0x000000ff;

	// Calculate deltas...
	SINT ad = ((c2 & 0xff000000) >> 24)-a1;
	SINT rd = ((c2 & 0x00ff0000) >> 16)-r1;
	SINT gd = ((c2 & 0x0000ff00) >> 8)-g1;
	SINT bd = (c2 & 0x000000ff)-b1;

	//SINT a3 = SINT(float(a1) * (1.f - v)) + SINT(float(a2) * v);
	//SINT r3 = SINT(float(r1) * (1.f - v)) + SINT(float(r2) * v);
	//SINT g3 = SINT(float(g1) * (1.f - v)) + SINT(float(g2) * v);
	//SINT b3 = SINT(float(b1) * (1.f - v)) + SINT(float(b2) * v);

	// Add fraction of deltas to initial points... (half the number of mults)
	SINT a3 = SINT(float(a1) + (float(ad)*v));
	SINT r3 = SINT(float(r1) + (float(rd)*v));
	SINT g3 = SINT(float(g1) + (float(gd)*v));
	SINT b3 = SINT(float(b1) + (float(bd)*v));

	//return colour
	return (a3 << 24) + (r3 << 16) + (g3 << 8) + b3;
}


//******************************************************************************************
void	CGame::ProcessGameState()
{

#define NUM_LS_FRAMES	150

	switch(mGameState)
	{

	case GAME_STATE_RUNNING:
		{
			mGameLevelData.ProcessLevel() ;
			break;
		}

	case GAME_STATE_GAME_FINISHED:
		mGameStateCounter++;
		if (mGameStateCounter > 70)
		{

			mGameState = GAME_STATE_FADE_OUT;
			mGameStateCounter = 0;
		}
		break;

	case GAME_STATE_FADE_OUT:
		if (mGameStateCounter == 0)
			SOUND.FadeAllSamples();

		mGameStateCounter += 2;

		if (mGameStateCounter > 50)
		{
			// level is done..  go to stats screen or something
//#if TARGET == PC

//#elif TARGET == PSX || TARGET == DC

			// Hokay - so what happens now?

			if (!mMultiplayer)
			{
				// did we win?
				ELevelResult res = PLAYER(0)->GetLevelResult();

				if (res == LR_WIN)
				{
#if DEMO_GAME == 3 || DEMO_GAME == 4
					// ask the player what to do
					mGamePanelActive = TRUE;
					GAMEPANEL.Activate(GPT_LOST);
					mGameState = GAME_STATE_WAITING_FOR_PANEL;
#else
					// cool.
					mQuit = GQ_FRONTEND_WIN;
#endif
				}
				else
				{
//					GAME.LoadLevel(GetCurrentLevelNumber() ); //! Yikes
//					return;

					// ask the player what to do
					mGamePanelActive = TRUE;
#if TARGET == DC
					GAMEPANEL.Activate(GPT_LOST, 0);
#else
					GAMEPANEL.Activate(GPT_LOST);
#endif
					mGameState = GAME_STATE_WAITING_FOR_PANEL;
				}
			}
			else
			{
				mQuit = GQ_MULTIPLAYER_FINISHED;
			}

//#endif
		}
		break;

		/*
	case GAME_STATE_PRIMARY_DEAD:
		mGameStateCounter += 2;

		if (mGameStateCounter > 50)
		{
			// level is done..  go to stats screen or something
#if TARGET == PC
			GAME.LoadLevel(GetCurrentLevelNumber()); //! Yikes
#elif TARGET == PSX || TARGET == DC
//			mQuit = TRUE; // quit to
			mGamePanelActive = TRUE;
			GAMEPANEL.Activate(GPT_LOST);

			mGameState = GAME_STATE_WAITING_FOR_PANEL;
#endif
		}
		*/


		break;
	case GAME_STATE_STARTING:
		mGameStateCounter ++;
		if (mGameStateCounter > NUM_LS_FRAMES)
		{
			mGameState = GAME_STATE_RUNNING;
			mGameStateCounter = 0;
		}

		break;

	case GAME_STATE_WAITING_FOR_PANEL:
		break;
	};


	switch (GetIVar(IV_LEVEL_TYPE))
	{

	// GOLFNEW
	case LT_GOLF:
		if (IsGameRunning())
		{
			mGolfStationaryCount[mGolfCurrentPlayer] ++;		// always count up...

			SINT	c0;

			for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
			{
				if (mGolfOBCount[c0] != -1)
				{
					mGolfOBCount[c0] ++;
					if (mGolfOBCount[c0] > 150)
						mGolfOBCount[c0] = -1;
				}
			}
		}

		if (!mGolfIsStationary[mGolfCurrentPlayer])
		{
			// check to see if the ball has stopped
			CPrimary *p = PLAYER(mGolfCurrentPlayer)->GetPrimary();

			if (p)
			{
				BOOL under_water = FALSE ;
				GINT wh;
				wh=GAME.GetLevelData()->mGVars[WATER_HEIGHT];
				if (wh != G0 && p->GetPos().Z > wh + p->Radius() )
				{
					under_water=TRUE ;
				}

				if (p->GetVelocity().MagnitudeSq() < G(0, 50) && under_water == FALSE)
				{

					mGolfMovingCount[mGolfCurrentPlayer] ++;
					if (mGolfMovingCount[mGolfCurrentPlayer] > 25)
					{
						GolfDeclareStationary(mGolfCurrentPlayer);
					}
				}
				else
					mGolfMovingCount[mGolfCurrentPlayer] = 0;
			}
		}


		break;

	};

	ProcessCrowdSounds();
}

//******************************************************************************************
void	CGame::CrowdEvent(GINT happiness, GINT intensity)
{
	mCrowdHappiness += happiness,
	mCrowdIntensity += intensity;

	if (mCrowdHappiness > G(1, 32768))
		mCrowdHappiness = G(1, 32768);
	if (mCrowdHappiness < -G1)
		mCrowdHappiness = -G1;

	if (mCrowdIntensity > G1)
		mCrowdIntensity = G1;
}

//******************************************************************************************
int	get_crowd_sound(SINT n)
{
	switch (n)
	{
	case 0: return SOUND_CROWD2;
	case 1: return SOUND_CROWD4;
	case 2: return SOUND_CROWD3;
	case 3: return SOUND_CROWD7;
	case 4: return SOUND_CROWD5;
	case 5: return SOUND_CROWD6;
	}
	return SOUND_SHOT1;
};

void	CGame::ProcessCrowdSounds()
{
	if (mCrowdIntensity == -G1) // game start
	{
		// kick the crowd off
		mCrowdHappiness = G(1, 32768);
		mCrowdIntensity = G1;
	}

	// basic sound
	SOUND.PlaySample(SOUND_CROWD1, G(0, 50000), TRUE, G(0, 1000));

	// other sounds
	SINT	c0;
	for (c0 = 0; c0 < 6; c0 ++)
		SOUND.PlaySample(get_crowd_sound(c0), G1, TRUE, G(0, 1));

	if (mGameTurn > 20)
	{
		// adjust intensities
		mCrowdIntensity *= G(0, 65000);

		if (mCrowdHappiness > G0)
			mCrowdHappiness -= G(0, 150);
		else
			mCrowdHappiness += G(0, 150);

		GINT	inten = mCrowdIntensity * G(0, 60535) + G(0, 5000);

		// basic sound
		SOUND.FadeTo(SOUND_CROWD1, inten);

		// work out which other sounds to use
		GINT	happy = mCrowdHappiness;
		if (happy > G1)
			happy = G1;
		if (happy < -G1)
			happy = -G1;

		GINT	split = (happy + G1) * G(2, 32700);
		SINT	s1 = split.Whole();
		SINT	s2 = s1 + 1;
		GINT	frac = split.GFrac();

		// and fade
		SOUND.FadeTo(get_crowd_sound(s1), inten * (G1 - frac) + G(0, 1), G(0, 5000));
		SOUND.FadeTo(get_crowd_sound(s2), inten * (     frac) + G(0, 1), G(0, 5000));

		// kill the others
		SINT	c0;
		for (c0 = 0; c0 < 6; c0 ++)
		{
			if ((c0 != s1) && (c0 != s2))
				SOUND.FadeTo(get_crowd_sound(c0), G(0, 1));
		}


//		if ((mGameTurn & 15) == 0)
//		{
//			printf("Intensity: %d       Happiness: %d\n", mCrowdIntensity.mVal, mCrowdHappiness.mVal);
//			printf("Samples: %d & %d, Frac : %d\n", s1, s2, frac.mVal);
//		}
	}
}

//******************************************************************************************
void	CGame::SetGameRenderInfo()
{
	mGRI.mActive = 0;  // reset game render stuff
	mGRI.mActive |= GRI_SCORE ;

	if (mDraw2D)
		mGRI.mActive |= GRI_2D;

	if (mDrawDebug)
		mGRI.mActive |= GRI_DEBUG_TEXT;

	if (mDrawProfiler)
		mGRI.mActive |= GRI_PROFILE;

	if (mDrawCollisionInfo)
		mGRI.mActive |= GRI_COLLISION_INFO;

	if (mDrawCharacterInfo!=NULL)
		mGRI.mActive |= GRI_CHARACTER_INFO;

	if (mDrawMapWhoInfo)
		mGRI.mActive |= GRI_MAPWHO_INFO;

	// Game State Stuff
	if (IsHelp())
		mGRI.mActive |= GRI_HELP;


#ifdef LINK_EDITOR
	if (!EDITOR.IsActive())
	{
#endif

	// Game State Specific stuff
	switch(mGameState)
	{
//	case GAME_STATE_LEVEL_COMPLETE:

//		mGRI.mActive |= GRI_LEVEL_COMPLETE;

//		break;

	case GAME_STATE_FADE_OUT:
//	case GAME_STATE_PRIMARY_DEAD:
	case GAME_STATE_WAITING_FOR_PANEL:		// well, leave the screen black
		mGRI.mActive |= GRI_CIRCLE_FADE_OUT;
		mGRI.mCircleFadeCount = mGameStateCounter;
		break;

	case GAME_STATE_STARTING:
		{
#define	NUM_LS_COLS		7
			DWORD	ls_cols[NUM_LS_COLS] =
			{
				0x00000000,
				0xffffaf20,
				0xffffff00,
				0xff00ff7f,
				0x000000ff,
				0x00000000,
				0x00000000
			};

			if (mGameStateCounter < 25)
			{
				mGRI.mActive |= GRI_CIRCLE_FADE_IN;
				mGRI.mCircleFadeCount = 50 - ((mGameStateCounter * 2) + 1 - 4); // hack for extra gameturns at start of level.
				if (mGRI.mCircleFadeCount > 50) mGRI.mCircleFadeCount = 50;
			}

			mGRI.mActive |= GRI_LEVEL_NUMBER;

#if TARGET == PC
//!PSX code fixed point version of this!
			float v1 = (float(mGameStateCounter    )  / float(NUM_LS_FRAMES)) * float(NUM_LS_COLS - 2);
			float v2 = (float(mGameStateCounter + 10) / float(NUM_LS_FRAMES)) * float(NUM_LS_COLS - 2);

			SINT	i;
			float	r;

			i = SINT(v1);
			r = float(fmod(v1, 1.f));
			ASSERT(i < NUM_LS_COLS - 1);
			mGRI.mLNLeftColour = interp_colour(ls_cols[i], ls_cols[i + 1], r);

			i = SINT(v2);
			r = float(fmod(v2, 1.f));
			ASSERT(i < NUM_LS_COLS - 1);
			mGRI.mLNRightColour = interp_colour(ls_cols[i], ls_cols[i + 1], r);

			mGRI.mLNNumber = mLevelNo;
#endif

			// Start Lights
			mGRI.mActive |= GRI_START_LIGHTS;
#define LIGHTS_SLIDE_TIME	20

			mGRI.mStartLightsPos = G1;
			if (mGameStateCounter < LIGHTS_SLIDE_TIME)
				mGRI.mStartLightsPos = G(mGameStateCounter) / LIGHTS_SLIDE_TIME;
			if (mGameStateCounter > NUM_LS_FRAMES - LIGHTS_SLIDE_TIME)
				mGRI.mStartLightsPos = G(NUM_LS_FRAMES - mGameStateCounter) / LIGHTS_SLIDE_TIME;

			mGRI.mStartLightsPos = G1 - ((G1 - mGRI.mStartLightsPos) * (G1 - mGRI.mStartLightsPos));

			mGRI.mStartLightsOn = (G(mGameStateCounter * 7) / NUM_LS_FRAMES) - G1;
			if (mGRI.mStartLightsOn < G0)
				mGRI.mStartLightsOn = G0;
			if (mGRI.mStartLightsOn > G(4))
				mGRI.mStartLightsOn = G(4);
		}
		break;
	};

#ifdef LINK_EDITOR
	}
#endif


	mGRI.mActive |= GRI_LIFE;


	// game type stuff

/*	if (mGRI.mCharacterLevelDetails == NULL)
	{
		mGRI.mCharacterLevelDetails = &WORLD.GetPrimaryThing(0)->GetLevelData() ;
	}
*/
	switch (GetIVar(IV_LEVEL_TYPE))
	{
	case LT_GOLF:
#define	GOLF_MAX_COUNT	75
#define	GOLF_MAX_COUNT2	50
#define	GOLF_MAX_COUNT3	25
		{
			SINT	foo = mGolfStationaryCount[mGolfCurrentPlayer];
			if (foo < GOLF_MAX_COUNT)
			{
				if (foo > GOLF_MAX_COUNT2)
					foo -= GOLF_MAX_COUNT3;
				else if (foo > GOLF_MAX_COUNT3)
					foo = GOLF_MAX_COUNT3;

				mGRI.mActive |= GRI_GOLF_SHOT_NO;

				GINT	s = G(foo - (GOLF_MAX_COUNT2 / 2));
				s = (s * s) / G(GOLF_MAX_COUNT2 / 2) / G(GOLF_MAX_COUNT2 / 2) * ((s > G0) ? G1 : -G1);

				mGRI.mGolfShotPos = -s;
				mGRI.mGolfShotNo = mGolfShotNo[mGolfCurrentPlayer];
			}

			foo = mGolfOBCount[mGolfCurrentPlayer];
			if ((foo != -1) && (foo < GOLF_MAX_COUNT))
			{
				if (foo > GOLF_MAX_COUNT2)
					foo -= GOLF_MAX_COUNT3;
				else if (foo > GOLF_MAX_COUNT3)
					foo = GOLF_MAX_COUNT3;

				mGRI.mActive |= GRI_GOLF_OB;

				GINT	s = G(foo - (GOLF_MAX_COUNT2 / 2));
				s = (s * s) / G(GOLF_MAX_COUNT2 / 2) / G(GOLF_MAX_COUNT2 / 2) * ((s > G0) ? G1 : -G1);

				mGRI.mGolfOBPos = -s;
			}
		}

		break;
	/*
	case LT_RACER:
		{
			mGRI.mActive |= GRI_RACE_DETAILS;
		}
		break;
	case LT_TAG:
		{
			mGRI.mActive |= GRI_TAG_DETAILS;
		}

	*/

		break;
	}

}

//******************************************************************************************
void	CGame::ToggleEditor()
{
#ifdef LINK_EDITOR
	if (EDITOR.IsActive())
	{
		EDITOR.DeActivate();
		mCameras[0].PanIn();
		mCameras[1].PanIn();
	}
	else
	{
		EDITOR.Activate();
		mCameras[0].PanOut();
		mCameras[1].PanOut();
	}
#endif
}


//******************************************************************************************
BOOL	CGame::IsGameStarting()
{
	if (mPaused && (!mFrameIncrease))
		return FALSE;

	if (mGameState == GAME_STATE_STARTING)
	{
		if (mGRI.mStartLightsOn < G(4))
			return TRUE;
	}
	return FALSE;
}

//******************************************************************************************
BOOL	CGame::IsGameRunning()
{
	if (mPaused && (!mFrameIncrease))
		return FALSE;

	if (mGameState == GAME_STATE_STARTING)
	{
		if (mGRI.mStartLightsOn != G(4))
			return FALSE;
	}
	return TRUE;
}

//******************************************************************************************
void	CGame::ProcessGame()
{
	mCameras[0].Update();
	if (mPlayer[1].IsActive())
		mCameras[1].Update();

	SetGameRenderInfo();



	if ( (mFrameIncrease || !mPaused) && (!mGamePanelActive) && (!mHelp) && (!(CONTROLS.IsButtonPressed(BUTTON_SUSPEND))))
	{
#ifdef LINK_EDITOR
		if (EDITOR.IsActive())
		{
			EDITOR.Process();
			mPlayer[0].EvaluateCursorPosition();
		}
		else
#endif
		{
//			if (mGameState != GAME_STATE_START)
			{
				// process Players
				SINT c0;
				for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
					mPlayer[c0].Process();

				// process Map
				{
					PROFILE_FN(ProcessMap);
					MAP.Process();
				}

				// process World
				{
					PROFILE_FN(ProcessWorld);
					WORLD.Process();
				}

				if (mFirstTimeThrough)
				{
					mGameLevelData.DeclareCharsThatWillAppearInThisGame() ;
					mFirstTimeThrough = FALSE ;
				}
//				if (IsGameRunning())
					mGameTurn++;

				if (!(IsGameRunning()))
					mGameStartTime = mGameTurn;
			}
			ProcessGameState();
		}
		mFrameIncrease = FALSE ;
	}

}



//******************************************************************************************
void	CGame::DeclareCharacterDead(CCharacter *c)
{
	mGameLevelData.CharacterHasDied(c) ;
}

//******************************************************************************************
void	CGame::DeclareCharacterFinishedLevel(CCharacter *p)
{
	mGameLevelData.CharacterFinishedLevel(p) ;
}

//******************************************************************************************
void	CGame::FrameLimit()
{
#if TARGET == PC
	//!PSX move this to PLATFORM

	// limit to 25 fps
	SINT t = GetTickCount();
	while ((t = GetTickCount()) < mTickCount + (1000/25))
	{
		PLATFORM.ProcessMessages();
	};

	mTickCount = t;
#endif
}

//******************************************************************************************
void	CGame::PlayerAction(SINT player_no, EAction action, GINT v1, GINT v2)
{
	// pass action to the player
	mPlayer[player_no].ProcessAction(action, v1, v2);
}

//******************************************************************************************
// setup level stuff according to level type

void	CGame::InitLevelType()
{

	// get the old chainsaw out
	if ((GetIVar(IV_NUMBER_PLAYERS) == 2)
#ifdef LINK_EDITOR
	 && (!(EDITOR.IsActive()))
#endif
	    )
	{
		mPlayer[1].Activate();
	}

	switch (GetIVar(IV_LEVEL_TYPE))
	{
	case LT_NORMAL:
		{
/*#if TARGET == PC
			EControllerMode cm = (EControllerMode)GAME.GetParams()->mController;
			if (cm >= CM_INVALID)
				cm = CM_MOUSE;
			CONTROLS.SetControllerMode(cm);
#elif TARGET == PSX || TARGET == DC*/
			CONTROLS.SetControllerMode(CM_JOY_ALT1);
//#endif

		}
		break;

	case LT_GOLF:
		{
			// GOLFNEW
			CONTROLS.SetControllerMode(CM_JOY_GOLF);

			// Init state stuff
			mGolfCurrentPlayer = 0;

			SINT c0;
			for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
			{
				mGolfShotNo[c0] = 0;
				GolfDeclareStationary(c0);
			}
		}
		break;

	case LT_SHUTTLE:
		CONTROLS.SetControllerMode(CM_JOY_SHUTTLE);
		break;

//	case LT_RACER:
//		CONTROLS.SetControllerMode(CM_JOY_RACER);
//		break;

	case LT_RACER:
	case LT_DIRECT:
	case LT_LASTMAN:
	case LT_GAUNTLET:
	case LT_PURSUIT:
	case LT_TAG:
	case LT_KING:
	case LT_DROID_BALL:
#if TARGET == PC
		{
			EControllerMode cm = (EControllerMode)GAME.GetParams()->mController;
			if (cm == CM_KEY_DIRECT)
				CONTROLS.SetControllerMode(CM_KEY_DIRECT);
			else
			{
				CONTROLS.SetControllerMode(CM_JOY_DIRECT, 0);
				CONTROLS.SetControllerMode(CM_KEY_DIRECT, 1);
			}
		}
#elif TARGET == PSX || TARGET == DC
		CONTROLS.SetControllerMode(CM_JOY_DIRECT);
#endif
		break;
	};
}

//******************************************************************************************
void		CGame::ToggleCharacterInfo()
{
	CharactersInWorldList& items = WORLD.GetCharacterNB() ;
	for (CCharacter* current = items.First();
					 current != NULL ;
					 current = items.Next() )
	{
		if ( mDrawCharacterInfo == current )
		{
			mDrawCharacterInfo = items.Next() ;
			return ;
		}
	}
	mDrawCharacterInfo = items.First() ;
}


//******************************************************************************************
void		CGame::SetGameState(EGameState new_state)
{
	mGameState = new_state;
	mGameStateCounter = 0 ;

	// ok disable game controls if game finished
	if (new_state == GAME_STATE_GAME_FINISHED)
	{
		for (SINT i =0;i<MAX_PLAYERS;i++)
		{
			if (PLAYER(i)->IsActive())
			{
				// good job contoller number matches player num.
				CONTROLS.DeActivateContoller(i) ;
			}
		}
	}

	// ok enable game controls if game starting
	if (new_state == GAME_STATE_STARTING)
	{
		for (SINT i =0;i<MAX_PLAYERS;i++)
		{
			if (PLAYER(i)->IsActive())
			{
				// good job contoller number matches player num.
				CONTROLS.ActivateContoller(i) ;
			}
		}
	}
}


// GOLFNEW
//******************************************************************************************
//******************************************************************************************
// Golf stuff
void	CGame::GolfDeclareStationary(SINT pn)
{
	if ((mGameState != GAME_STATE_GAME_FINISHED) && (mGameState != GAME_STATE_FADE_OUT))
	{
		mGolfIsStationary[pn] = TRUE;
		mGolfStationaryCount[pn] = 0;
		mGolfOBCount[pn] = -1;
		mGolfShotNo[pn] ++;
		WORLD.GolfArrowEnable();
	}
}

//******************************************************************************************
BOOL	CGame::GolfCanIPlay(SINT pn)
{
	return mGolfIsStationary[pn];
}

//******************************************************************************************
void	CGame::GolfDeclarePlay(SINT pn)
{
	mGolfIsStationary[pn] = FALSE;
	mGolfMovingCount[pn] = 0;
	WORLD.GolfArrowDisable();
}

//******************************************************************************************
void	CGame::GolfDeclareLastShot(SINT pn, const GVector &v)
{
	mGolfLastShot[pn] = v;
}

//******************************************************************************************
//******************************************************************************************
void	CGame::LockCamera(SINT n)
{
	mCameras[n].Lock();
}

//******************************************************************************************
void	CGame::UnlockCamera(SINT n)
{
	mCameras[n].Unlock();
}

//******************************************************************************************
BOOL	CGame::DoWeNeedThisCharacter(SINT cn)
{
	// is this primary played by a player?
	//!! Or is this a badguy?

	if (cn == -1)
		return TRUE; // not a primary

	// which career?

	if (mMultiplayer)
	{
		if (MULTIPLAYER_CAREER.mWhichPlayer[0] == cn)	return TRUE;
		if (MULTIPLAYER_CAREER.mWhichPlayer[1] == cn)	return TRUE;
		return FALSE;
	}
	else
	{
		// GOLF HACK
		if ((GetIVar(IV_LEVEL_TYPE) == LT_GOLF) || (GetIVar(IV_LEVEL_TYPE) == LT_NORMAL))
			return cn == 0;
		else
		{
			//!!!! VIOLENT HACK
			SINT	hack_char = -1;
			switch (mLevelNo)
			{
			case 75: hack_char = 2; break;
			case 76: hack_char = 0; break;
			case 77: hack_char = 4; break;
			case 78: hack_char = 1; break;
			case 79: hack_char = 3; break;
//			case 80: hack_char = 5; break;
			}
			if (hack_char != -1)
				if (cn == hack_char)
					return TRUE;

			return (cn == CAREER.mCurrentCharacter);
		}
	}

}

//******************************************************************************************
SINT	CGame::GetPlayerCharacter(SINT n)
{
	// which character is played by player n
	if (mMultiplayer)
	{
		ASSERT((n == 0) || (n == 1));
		return MULTIPLAYER_CAREER.mWhichPlayer[n];
	}
	else
	{
//		ASSERT(n == 0);
		return CAREER.mCurrentCharacter;
	}

}

//******************************************************************************************
#define SEENSIZE	30

BOOL	CGame::CanIBeSeen(const GVector &p)
{
	if ((GABS(p.X - mCameras[0].mLookAt.X) < G(SEENSIZE)) &&
		(GABS(p.Y - mCameras[0].mLookAt.Y) < G(SEENSIZE)))
		return TRUE;

	if (mMultiplayer)
	{
		if ((GABS(p.X - mCameras[1].mLookAt.X) < G(SEENSIZE)) &&
			(GABS(p.Y - mCameras[1].mLookAt.Y) < G(SEENSIZE)))
			return TRUE;
	}

	return FALSE;
}
