#include	"Common.h"

#include 	"Career.h"

CCareer				CAREER;
CMultiplayerCareer	MULTIPLAYER_CAREER;

//***************************************************
void	CLevelCareer::Clear()
{
	mFinished = ECL_NO;
	mBestTime = 0xffff;
	mNumAttempts = 0;
}
	
//***************************************************
void	CLevelCareer::DeclareTime(SINT time)
{
	//! Should check level type...??
	
	if (mBestTime > time)
		mBestTime = time;
}

//***************************************************
//***************************************************
void	CCharCareer::Clear()
{
	mStarted = FALSE;
	mStartedAlpha = FALSE;
	mWonGame = FALSE;

	mNumLevelsCompleted = 0;
	mNumGoldStars = 0;
	mNumLostToysTimes = 0;
	
	SINT	c0, c1;
	
	for (c0 = 0; c0 < 11; c0 ++)
		for (c1 = 0; c1 < MAX_LEVELS_PER_PRISON; c1 ++)
			mLevelCareers[c0][c1].Clear();
}

//***************************************************
void	CCareer::Clear()
{
	mVersion = CAREER_VERSION;

	SINT	c0;
	
	for (c0 = 0; c0 < 6; c0 ++)
		mCharCareers[c0].Clear();
		
	mCurrentCharacter = 0;
	mCurrentPrison = 0;
	mCurrentArena = 0;
	mCurrentDiskLevel = 0;
	
//	sprintf(mName, "TestSave");
	mName[0] = 0;
	
	mStarted = FALSE;
	
	mPendingVictory = FALSE;
}

//***************************************************
BOOL	CCareer::AllGold()
{
	return ((mCharCareers[0].mNumGoldStars >= 71) ||
			(mCharCareers[1].mNumGoldStars >= 71) ||
			(mCharCareers[2].mNumGoldStars >= 71) ||
			(mCharCareers[3].mNumGoldStars >= 71) ||
			(mCharCareers[4].mNumGoldStars >= 71) ||
			(mCharCareers[5].mNumGoldStars >= 72));
}

//***************************************************
void	CMultiplayerCareer::Clear()
{
	mCurrentTournament	= 0;
	mCurrentArena		= 0;
	mCurrentDiskLevel	= 0;
	
	mWhichPlayer[0] = 1;
	mWhichPlayer[1] = 3;

	SINT	c0;
	
	for (c0 = 0; c0 < MAX_LEVELS_IN_TOURNAMENT; c0 ++)
		mWhoWonWhichLevel[c0] = -1;	
}