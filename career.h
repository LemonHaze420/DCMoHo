#ifndef CAREER_H
#define CAREER_H

// everything you've ever done, on the record!

#include	"LevelInfo.h"

//***************************************************
#define	CAREER_VERSION	2

//***************************************************
#define	ECL_NO		0
#define	ECL_YES		1
#define	ECL_GOLD	2
#define	ECL_LT		3

//***************************************************
class	CLevelCareer
{
public:
	void	Clear();
	void	AddAttempt()
	{
		if (mNumAttempts < 255)
			mNumAttempts ++;
	}

	void	DeclareTime(SINT time);

 	UBYTE	mFinished;
	UBYTE	mNumAttempts;
	UWORD	mBestTime;
};

//***************************************************
class	CCharCareer
{
public:
	void	Clear();

	BOOL			mStarted;
	SINT			mNumLevelsCompleted;
	SINT			mNumGoldStars;
	SINT			mNumLostToysTimes;
	BOOL			mStartedAlpha;
	BOOL			mWonGame;

	CLevelCareer	mLevelCareers[11][MAX_LEVELS_PER_PRISON];
};

//***************************************************
class	CCareer
{
public:
	CCareer()
	{
		mSoundVolume = 8;
		mMusicVolume = 8;
		mControlSet = 0;
		mStereo = FALSE;
		mForceFeedback = TRUE;
		mControls1 = 0;
		mControls2 = 0;
		mCodeKey = rand() & 0xff;
		
#if TARGET == DC
		// DC stuff
		mMemController = -2;		// -2 = not selected yet, -1 = chosen no mem card
		mMemSlot = -2;
#endif
	};

	void	Clear();
	void	Start()
	{
		Clear();
		mStarted = TRUE;
		mCodeKey = rand() & 0xff;
	};

	void	DeclareLevelAttempted()
	{
		if  (mCharCareers[mCurrentCharacter].mLevelCareers[mCurrentPrison][mCurrentArena].mNumAttempts < 255)
			mCharCareers[mCurrentCharacter].mLevelCareers[mCurrentPrison][mCurrentArena].mNumAttempts ++;
	};

	void	DeclarePendingVictory(SINT time)
	{
		mPendingVictory = time;
	};

	SINT	GetPendingVictory()
	{
		return mPendingVictory;
	};
	
	BOOL	AllGold();

	SINT		mVersion;

	CCharCareer	mCharCareers[6];

	SINT		mCurrentCharacter;
	SINT		mCurrentPrison;
	SINT		mCurrentArena;
	SINT		mCurrentDiskLevel;

	BOOL		mStarted;

	char		mName[9];

	SINT		mPendingVictory;

	//	options
	SINT		mSoundVolume;
	SINT		mMusicVolume;
	SINT		mControlSet;
	BOOL		mStereo;
	BOOL		mForceFeedback;
	SINT		mControls1;
	SINT		mControls2;
	
	SINT		mCodeKey;
	
#if TARGET == DC
	SINT		mMemController;
	SINT		mMemSlot;
#endif
	
};

//***************************************************
//***************************************************
#define MAX_LEVELS_IN_TOURNAMENT 10

class	CMultiplayerCareer
{
public:
	void	Clear();

	SINT	mCurrentTournament;
	SINT	mCurrentArena;
	SINT	mCurrentDiskLevel;

	SINT	mWhichPlayer[2];

	SINT	mWhoWonWhichLevel[MAX_LEVELS_IN_TOURNAMENT];
};

// Singleton classes

extern	CCareer				CAREER;
extern	CMultiplayerCareer	MULTIPLAYER_CAREER;

#endif
