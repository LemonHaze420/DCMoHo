#ifndef GAME_H
#define GAME_H

#define MAX_CHARS_IN_WORLD 20


#if TARGET == PC
#include	"ttshell.h"
#include	"JCLEngine.h"
#elif TARGET == DC
#include	"DCEngine.h"
#elif TARGET == PSX
#include	"PSXEngine.h"
#endif

#include	"Camera.h"
#include	"LevelData.h"
#include	"Action.h"
#include	"Player.h"
#include	"CLIParams.h"

#include	"GameLevelData.h"

#if TARGET==PSX
#define MAX_PLAYERS 2
#endif

#if TARGET==DC
#define MAX_PLAYERS 2
#endif

#if TARGET==PC
#define MAX_PLAYERS 4
#endif


class CCharacterLevelData ;


//******************************************************************************************

#define	GRI_LEVEL_COMPLETE		(1 << 0)
#define	GRI_CIRCLE_FADE_IN		(1 << 1)
#define	GRI_DEBUG_TEXT			(1 << 2)
#define	GRI_PROFILE				(1 << 3)
#define	GRI_SCORE				(1 << 4)
#define	GRI_LIFE				(1 << 5)
#define	GRI_BRAKE				(1 << 6)
#define	GRI_LEVEL_NUMBER		(1 << 7)
#define	GRI_2D					(1 << 8)
#define GRI_COLLISION_INFO		(1 << 9)
#define GRI_MAPWHO_INFO			(1 << 10)
#define	GRI_CIRCLE_FADE_OUT		(1 << 11)
#define	GRI_HELP				(1 << 12)
#define GRI_GOLF_SHOT_NO		(1 << 13)
#define	GRI_GOLF_OB				(1 << 14)
#define	GRI_START_LIGHTS		(1 << 15)
#define GRI_CHARACTER_INFO      (1 << 16)



// number frames before we actualy start ??

#define NUM_LS_FRAMES	150


class	CGameRenderInfo
{
public:
	ULONG	mActive;

	SINT	mCircleFadeCount;
	GINT	mStartLightsOn;
	GINT	mStartLightsPos;
	SINT	mLNNumber;
	DWORD	mLNLeftColour;
	DWORD	mLNRightColour;

	GINT	mGolfShotPos;
	SINT	mGolfShotNo;

	GINT	mGolfOBPos;
};

//******************************************************************************************

enum	EGameState
{
	GAME_STATE_RUNNING = 0,
	GAME_STATE_STARTING,
	GAME_STATE_WAITING_FOR_PANEL,

	// anything after this then games objects get switched off (i.e. don't attack etc ) 
	GAME_STATE_GAME_FINISHED,
	GAME_STATE_FADE_OUT,
};

enum	EGQuitType
{
	GQ_NONE = 0,
	GQ_RESTART,
	GQ_ADVANCE,
	GQ_QUIT,
	GQ_FRONTEND,
	GQ_FRONTEND_WIN,
	GQ_MULTIPLAYER_FINISHED,
	GQ_DC_RESET,
	GQ_DC_EXIT,
};

class	CPrimary;



class	CGame
{
public:
	CGame() {mLanguage = 0;};
	~CGame() {};

#if TARGET == PC
	BOOL		Init(TTApp *app, CCLIParams &params);
	CCLIParams*	GetParams() {return	&mCLIParams;};

#elif TARGET == DC
	BOOL		Init();
#elif TARGET == PSX
	BOOL		Init();
#endif

	void		Shutdown();
	void		Reset();

	void		SetQuitFlag(EGQuitType f) {mQuit = f;};

	CLevelData	*GetLevelData() {return &mLevelData;};

	SINT		GetCurrentLevelNumber() {return mLevelNo;};

	SINT		GetGameTurn()			{return mGameTurn;};
	SINT		GetGameTimeSinceStart()	{return mGameTurn - mGameStartTime;};

	void		ProcessGame();
	void		ProcessGameState();
	void		SetGameRenderInfo();
	void		ProcessCrowdSounds();

	EGameState	GetGameState() {return mGameState;};
	void		SetGameState(EGameState new_state);
	SINT		GetGameStateCounter() {return mGameStateCounter;};

	void		GSDeclarePrimaryDead(CPrimary *p);
	void		DeclareCharacterDead(CCharacter* c) ;
	void		DeclareCharacterFinishedLevel(CCharacter *p);

	BOOL		IsGameRunning();
	BOOL		IsGameStarting() ;

	CGameRenderInfo	*GetGameRenderInfo() {return &mGRI;};

	void		PlayerAction(SINT player_no, EAction action, GINT v1 = G1, GINT v2 = G1);
	CPlayer		*GetPlayer(SINT no) {ASSERT((no >= 0) && (no < MAX_PLAYERS)); return &mPlayer[no];};

	void		FrameLimit();

	void		Toggle2D()				{mDraw2D			= (!mDraw2D           );};
	void		ToggleCollisionInfo()	{mDrawCollisionInfo = (!mDrawCollisionInfo);};
	void		ToggleMapWhoInfo()		{mDrawMapWhoInfo	= (!mDrawMapWhoInfo   );};
	void		ToggleDebugText()		{mDrawDebug			= (!mDrawDebug		  );};
	void		ToggleProfiler()		{mDrawProfiler		= (!mDrawProfiler     );};
	void		ToggleCharacterInfo() ;
	CCharacter* GetCurrentCharacterInfo() { return mDrawCharacterInfo ; }

	void		TogglePause();
	void		FrameIncrease() ;
	BOOL		IsPaused() {return mPaused;};

	void		ToggleHelp();
	BOOL		IsHelp() {return mHelp;};

	void		ToggleGamePanel(SINT pn);
	BOOL		IsGamePanelActive() {return mGamePanelActive;};

	void		ToggleEditor();

	void		DeclareLevelDataOld() {mLevelDataOld = TRUE;}; // Automatically rewrite when loading.

	void		InitLevelType();

	SINT		GetIVar(SINT i) {return mLevelData.mIVars[i];};
	GINT		GetGVar(SINT i) {return mLevelData.mGVars[i];};
	SINT		GetNumPlayers() { return mLevelData.mIVars[IV_NUMBER_PLAYERS] ; };
	CCamera		*GetCamera(SINT n) {return &mCameras[n];};


	// GOLFNEW
	void		GolfDeclareStationary(SINT pn);
	BOOL		GolfCanIPlay(SINT pn);
	void		GolfDeclarePlay(SINT pn);
	void		GolfDeclareLastShot(SINT pn, const GVector &v);
	BOOL		GolfIsStationary(SINT pn) {return mGolfIsStationary[pn];};
	GVector&	GolfGetLastShotPosition(SINT for_player) { return mGolfLastShot[for_player] ; }
	void		GolfIncreaseShotNumber(SINT for_player) { mGolfShotNo[for_player]++ ; }
	SINT		GolfGetCurrentPlayer() { return mGolfCurrentPlayer ; }
	SINT		GolfGetShotNo(SINT pn) {return mGolfShotNo[pn];};
	SINT		GolfGetTotalShotNo(SINT pn) {return mGolfTotalShotNo[pn];};
	void		GolfDeclareOB(SINT pn) {mGolfOBCount[pn] = 0;};

	void		LockCamera(SINT n);
	void		UnlockCamera(SINT n);
	CGameLevelData& GetGameLevelData() { return mGameLevelData ; } ;

	BOOL		DoWeNeedThisCharacter(SINT cn);

	void		DeclareSinglePlayer() {mMultiplayer = FALSE;};
	void		DeclareMultiPlayer()  {mMultiplayer = TRUE;};
	BOOL		IsMultiPlayer()  {return mMultiplayer ;};


	SINT		GetPlayerCharacter(SINT n);

	BOOL		CanIBeSeen(const GVector &p);
	
	void		ClearGolfShots() {SINT	c0; for (c0 = 0; c0 < MAX_PLAYERS; c0 ++) mGolfTotalShotNo[c0] = 0;};
	void		ClearBestResult() {mBestResult = -1;};
	
	SINT		GetLanguage() {return mLanguage;};
	
	void		ClearCheats() {SINT c0; for (c0 = 0; c0 < 20; c0 ++) mCheats[c0] = FALSE;};
	void		EnableCheat(SINT n) {mCheats[n] = TRUE;};
	BOOL		IsCheatEnabled(SINT n) {return mCheats[n];};
	
protected:
	SINT		mLanguage;

	CGameLevelData mGameLevelData ;

	EGQuitType	mQuit;

	CCamera		mCameras[2];
	CLevelData	mLevelData;

	SINT		mLevelNo;
	BOOL		mFirstTimeThrough;

	SINT		mGameTurn;
	SINT		mGameStartTime;

	EGameState	mGameState;
	SINT		mGameStateCounter;
	BOOL		mFrameIncrease;

	CGameRenderInfo	mGRI ;

	CPlayer		mPlayer[MAX_PLAYERS];

	BOOL		mPaused, mGamePanelActive, mHelp;

	BOOL		mDraw2D;
	BOOL		mDrawDebug;
	BOOL		mDrawProfiler;
	BOOL		mDrawCollisionInfo;
	CCharacter* mDrawCharacterInfo;
	BOOL		mDrawMapWhoInfo;

	SINT		mTickCount;

	SINT		mLevelDataOld;

	CCLIParams	mCLIParams;

	BOOL		mMultiplayer;

	// GOLFNEW
	// golf stuff
	SINT		mGolfShotNo[MAX_PLAYERS];
	SINT		mGolfTotalShotNo[MAX_PLAYERS];

	BOOL		mGolfIsStationary[MAX_PLAYERS];
	SINT		mGolfStationaryCount[MAX_PLAYERS];
	SINT		mGolfMovingCount[MAX_PLAYERS];
	GVector		mGolfLastShot[MAX_PLAYERS];
	SINT		mGolfOBCount[MAX_PLAYERS];

	SINT		mGolfCurrentPlayer;
	
	
	SINT		mBestResult; // track scores across restarting the level
	
	// crowd stuff
public:
	void		CrowdEvent(GINT happiness, GINT intensity);
protected:
	GINT		mCrowdHappiness;
	GINT		mCrowdIntensity;
	
	BOOL		mCheats[20];
};

#define GAME_NAME "Moho"

//******************************************************************************************
#endif