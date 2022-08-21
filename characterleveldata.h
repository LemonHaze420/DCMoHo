// CharacterLevelData.h: interface for the CCharacterLevelData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CHARACTER_LEVEL_DATA_H
#define CHARACTER_LEVEL_DATA_H

class CKingHillGoalPoint ;
class CDroidBall ;

// don't laugh this is deadly serious 
class CNearestCheckPointReport
{
	// classes that will be writing the report so give them write access
	friend class CCharacterLevelData ;

public:

		void				Clear() { mCheckPoint= NULL ; mCompiledTime = -2 ; } ;
		CCheckPoint*		GetCheckPoint() { return mCheckPoint ; } ;
		SINT				GetCompiledTime() { return mCompiledTime ; } ;
		GINT&				GetDistToCheckPointt() { return mDistToCheckPoint ; } ;

private:
	GINT			mDistToCheckPoint ;
	CCheckPoint*	mCheckPoint ;
	SINT			mCompiledTime;
};



class CCharacter ; 

class CCharacterLevelData 
{

	// GENERIC part

public:

	void	SetFinishedLevel()  ; 
	BOOL	GetFinishedLevel() { return mFinishedLevel ; } ;
	SINT	GetCompletedLevelTime() { return mCompletedLevelTime ; } ;
	SINT	GetTurnsTaken()  ;


protected:

	CCharacter* mCharacter ;
	BOOL		mFinishedLevel ;
	SINT		mCompletedLevelTime  ;


	// RACER
public:
	void	ReachedCheckPoint(SINT num) ;
	CNearestCheckPointReport&	GetShortestDistToCheckPointsSQ(SINT check_point_num) ;
	SINT	GetCurrentRacePosition();
	SINT	GetLapNumber() { return mLap ; } ;
	SINT	GetLastCheckPoint() { return mLastCheckPoint ; }
	void	SetLastCheckPoint(SINT to) { mLastCheckPoint = to ; }
	SINT	GetFastedLapTime()	{ return mFastedLapTime ; } 
	SINT	GetTotalRaceTime() { return mTotalRaceTime ; } 
	SINT	GetCurrentLapTime() ;
	SINT	GetLastLapTime() { return mLastLapTime ; } 



private:

	SINT		mLastCheckPoint;
	SINT		mLap ;
	SINT		mPosition;
	SINT		mFinishedPosition;
	SINT		mFastedLapTime  ;
	SINT		mStartedLapTime  ;
	SINT		mLastLapTime ;
	SINT		mTotalRaceTime ;

	// lazy evaluation data to speed up calcs

	CNearestCheckPointReport		mNearestCheckPointReport;

	// KING OF THE HILL

public:

	void InKingHillGoalPoint(CKingHillGoalPoint* king_hill_area) ;
	void Init(CCharacter* for_char) ;
	SINT GetLastFrameInKingHillArea() { return mLastFrameInKingHillArea ; }
	CCheckPoint* IsInAPathLeadingUpTheHill() ;
	CKingHillGoalPoint* GetWhichKingHillArea() { return mWhichKingHillArea ; }
	BOOL AreWeInArea() ; 
	void ReCalcCurrentCrownHolder() ;

private:
	
	SINT mPoints ;
	SINT mLastFrameInKingHillArea;
	CKingHillGoalPoint* mWhichKingHillArea ;
	static	SINT mMaxPointsReachedSoFar ; 
	static  CCharacter* mCurrentCrownHolder ;



	// TAG


public:
	void	PickedUpToken() ;
	SINT	GetNumTokensPickedUp() { return mNumPickedUpTokens ; }
	void	AddTokens( SINT amount) { mNumPickedUpTokens+=amount ; }
	void	DiedOnTag() ;

private:

	SINT mNumPickedUpTokens  ;


	// DROID BALL

public:

	void	HitDroidBallPole(CDroidBall* ball_which_hit) ;
	void	DiedOnDroidBall() ;


};


#endif
