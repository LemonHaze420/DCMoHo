// CharacterLevelData.cpp: implementation of the CCharacterLevelData class.
//
//////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Globals.h"
#include "CharacterLevelData.h"
#include "Character.h"
#include "Pickup.h"


//******************************************************************************************
void CCharacterLevelData::Init(CCharacter* for_char) 
{
	mCharacter= for_char ;
	mFinishedLevel = FALSE ;

	mCompletedLevelTime  = 0 ;

	// racer

	mLastCheckPoint = -1;
	mLap =-1;
	mPosition = 0 ;
	mFinishedPosition = -1 ;

	mFastedLapTime =-1 ;
	mStartedLapTime  = 0  ;
	mTotalRaceTime = 0 ;

	mNearestCheckPointReport.Clear() ; 

	//King of the hill

	mPoints =0 ;
	mLastFrameInKingHillArea = -1;
	mWhichKingHillArea = NULL ;

	// static member 
	mCurrentCrownHolder = NULL ;

	// static memeber 
	mMaxPointsReachedSoFar = 0 ;


	// tag

	mNumPickedUpTokens = 0 ;

}


//******************************************************************************************
void	CCharacterLevelData::SetFinishedLevel()
{
	mFinishedLevel = TRUE  ;

	mCompletedLevelTime = GAME.GetGameTimeSinceStart() ;

	 // ok we can never have a finish time which is after the max time allowed for
	 // level

	SINT	tl = GAME.GetLevelData()->mIVars[IV_TIME_LIMIT] * 25;
	
	if (tl > 0 )
	{
		if (mCompletedLevelTime > tl ) mCompletedLevelTime = tl ;
	}
}

//******************************************************************************************
SINT	CCharacterLevelData::GetTurnsTaken()  
{ 
	return GAME.GetGameTimeSinceStart(); 
} 





	

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// CHARACTER RACER STUFF


//******************************************************************************************
SINT	CCharacterLevelData::GetCurrentLapTime() 
{
	// have we started race
	if (mLap==-1) return 0 ;
	
	return GAME.GetGameTurn() - mStartedLapTime ;
}


//******************************************************************************************
void CCharacterLevelData::ReachedCheckPoint(SINT num) 
{

	// have we crossed the start finish point for the first time ? i.e. completed all the 
	// rolling start type stuff.

	if (mLap==-1) 
	{
		// ok we've not started the race and we're not at the first checkpoint, so do nothing
		if (num !=0) return ;
	
		// ok the race timers are now going
		mLap=0 ;
		mStartedLapTime = GAME.GetGameTurn();
		mLastCheckPoint = 0 ;
		return ;
	}

	// have we completed a lap ( only when we are in a racer mode )

	if (num == 0 && (mLastCheckPoint == GAME.GetIVar(IV_NUM_CHECKPOINTS) || mLastCheckPoint == GAME.GetIVar(IV_NUM_CHECKPOINTS) -1 ) &&
		GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER  )
	{
	 
		mLap++ ;
		SINT current_time = GAME.GetGameTurn() ;
		SINT lap_time = current_time - mStartedLapTime ;
		mTotalRaceTime +=lap_time ;
		mLastLapTime = lap_time ;

		if (lap_time < mFastedLapTime || mFastedLapTime == -1 )
		{
			mFastedLapTime = lap_time ;
		}

		mStartedLapTime = current_time;
		mLastCheckPoint = num ;

		// have we completed the race ?
		if ((mLap) >= GAME.GetIVar(IV_NUM_LAPS_FOR_RACE))
		{
			// this will then declare itself finish to GAME which will then store the result
			// position in its GameLevelData object
			mCharacter->FinishedLevel() ;

			// now get that position and store it in our character level data
			mFinishedPosition = GAME.GetGameLevelData().GetFinshedPositionForCharacter(mCharacter) ;
		}
	
	}

	// else have we reached the next checkpoint

	else if (num == mLastCheckPoint+1 || num == mLastCheckPoint+2)
	{
		mLastCheckPoint = num ;
	}

}


//******************************************************************************************
CNearestCheckPointReport&	CCharacterLevelData::GetShortestDistToCheckPointsSQ(SINT check_point_num)
{
	// this prevents the same calculation been done multiple times on the 
	// same game turn. 99% of time we will be asking for this with the same
	// check point number 

	if (mNearestCheckPointReport.mCompiledTime == GAME.GetGameTurn() &&
		mNearestCheckPointReport.mCheckPoint &&
		mNearestCheckPointReport.mCheckPoint->GetCheckPointNumber() == check_point_num) 
	{
		return mNearestCheckPointReport;
	}

	// distance is calculated as:
	// distance to next post + distance to next post->target (i.e. other post )
	// this is only an approx to work out who is closer to the next
	// checkpoint but works good enough

	const CheckPointList& items = WORLD.GetCheckPointNB();
	mNearestCheckPointReport.mDistToCheckPoint = G(120*120) * 2 ;
	GVector my_pos = mCharacter->GetPos() ;

	CCheckPoint *nt ;

	// loop through all checkpoints in world
	for (nt = items.First() ;
		 nt != NULL ;
		 nt = items.Next() )
	{
		// is the checkpont one we are looking for ?
		if (nt->GetCheckPointNumber() == check_point_num && nt->GetTarget() &&
			nt->GetActivated()) 
		{
			GVector v1 = nt->GetPos() - my_pos  ;
			GVector v2 = nt->GetTarget()->GetPos() - my_pos ;

			// make 2d
			v1.Z = G0;
			v2.Z = G0;

			GINT dist = v1.MagnitudeSq()  + v2.MagnitudeSq() ;
			if (dist < mNearestCheckPointReport.mDistToCheckPoint)
			{
				mNearestCheckPointReport.mDistToCheckPoint = dist ;
				mNearestCheckPointReport.mCheckPoint = nt ;
			
			}
		}
	}

	mNearestCheckPointReport.mCompiledTime = GAME.GetGameTurn() ;
	return  mNearestCheckPointReport ;
}





//******************************************************************************************
SINT	CCharacterLevelData::GetCurrentRacePosition()
{
	// if we have already finsihed then that finish position is our race position 

	if (mFinishedPosition !=-1) 
	{
		return mFinishedPosition+1 ;
	}

	CListArray<CCharacter, MAX_CHARACTERS_IN_WORLD>& items = WORLD.GetCharacterNB();

	SINT position = 1 ;

	// calculate next checkpoint we are trying to get to
	SINT next_checkpoint = GetLastCheckPoint() +1 ; 
	if (next_checkpoint > GAME.GetIVar(IV_NUM_CHECKPOINTS) )  
	{
		next_checkpoint = 0 ;
	}

	// find the shortest dist between us and all the checkpoint posts with that number
	GINT shortest_dist = GetShortestDistToCheckPointsSQ(next_checkpoint).mDistToCheckPoint ;

	// number of checkpoints we have passed including laps we say each lap is 2^10 checkpoints
	// !! it works !!
	SINT our_total_check_points = (GetLapNumber() << 10) + GetLastCheckPoint() ;

	CCharacter	*current = NULL;

	// loop through all characters in world

	CCharacter *nt ;
	for (nt = items.First() ;
		 nt != NULL ;
		 nt = items.Next() )
	{
		// is character actually part of the race ?
		if (nt->IsRacer() || nt->IsPrimary())
		{
			// get number of checkpoints the other racer has passed
			CCharacterLevelData* cld = &nt->GetLevelData();
			SINT total_check_points = (cld->GetLapNumber() << 10 )+ cld->GetLastCheckPoint() ;

			// if the other racer has passed more checkpoints than us then it must be in front
			if (total_check_points > our_total_check_points) position++ ;

			// if the other racer has passed same amount of checkpoints then find out who is
			// closer to the next checkpoint
			if (total_check_points == our_total_check_points) 
			{
				if (nt->GetLevelData().GetShortestDistToCheckPointsSQ(next_checkpoint).mDistToCheckPoint <  shortest_dist) 
				{
					// other racer is closer than we are so drop a place
					position++;
				}
			}
		}
	}
	return position ;
}



//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

// CHARACTER KING OF THE HILL

SINT CCharacterLevelData::mMaxPointsReachedSoFar = 0 ;
CCharacter* CCharacterLevelData::mCurrentCrownHolder = NULL ;

//******************************************************************************************
void CCharacterLevelData::InKingHillGoalPoint(CKingHillGoalPoint* king_hill_area) 
{
	if (GAME.GetGameState() >= GAME_STATE_GAME_FINISHED) return ;
	mPoints++ ;
	mWhichKingHillArea = king_hill_area ;

	GAME.GetGameLevelData().AddCharacterToKingHill(mCharacter) ;
	mLastFrameInKingHillArea = GAME.GetGameTurn() ; 	

	if (GAME.GetGameLevelData().NumCharactersOnHill() == 1 || (!mCharacter->IsPrimary()))
	{
		mCharacter->IncreaseScore(1) ;

	}
	// if other people on hill score at 25%
	else if ((GAME.GetGameTurn() & 3) == 0) // (JCL) - fixed
	{
		mCharacter->IncreaseScore(1) ;
	}

	// ok if we have taken the lead then start wearing the crown
	if ( mMaxPointsReachedSoFar < mCharacter->GetScore() && 
	        mCharacter != mCurrentCrownHolder)
	{
		if (mCurrentCrownHolder) mCurrentCrownHolder->StopWearingCrown() ;
		mMaxPointsReachedSoFar = mCharacter->GetScore() ;
		mCharacter->StartWearingCrown() ;
		mCurrentCrownHolder = mCharacter ;
	}

	if (mCharacter == mCurrentCrownHolder)
	{
		mMaxPointsReachedSoFar = mCharacter->GetScore() ;
	}

	// have we won the game
	if ( mCharacter->GetScore() >= GAME.GetIVar(IV_POINTS_REQUIRED_TO_WIN_LEVEL)  )
	{
		mCharacter->FinishedLevel() ;
	}
}

//********************************************************************************************************
// this function is called from character if that character dies and is the current crown holder.  
// This function will then work out who the next highest score character is and gives him the crown. 
// If no one else is alive the crown will disapear
//********************************************************************************************************
void	CCharacterLevelData::ReCalcCurrentCrownHolder()
{
	CharactersInWorldList& items = WORLD.GetCharacterNB() ;
	SINT highest_score = -2 ;
	CCharacter* highest_char = NULL ;

	for (CCharacter* nt = items.First() ;
					 nt != NULL ;
					 nt = items.Next() )
	{
		if (nt->GetState() != CS_DEAD)
		{
			SINT score = nt->GetScore() ;
			if (score > highest_score)
			{
				highest_score = score ;
				highest_char = nt ;
			}
		}
	}

	if (mCharacter != highest_char)
	{
		mCharacter->StopWearingCrown() ;
		if (highest_char)
		{
			highest_char->StartWearingCrown() ;
			mMaxPointsReachedSoFar = highest_char->GetScore() ;
			mCurrentCrownHolder = highest_char ;
		}
	}
}

//********************************************************************************************************
CCheckPoint*	CCharacterLevelData::IsInAPathLeadingUpTheHill()
{
	const CheckPointList& items = WORLD.GetCheckPointNB();

	CCheckPoint *nt ;

	// loop through all checkpoints in world
	for (nt = items.First() ;
		 nt != NULL ;
		 nt = items.Next() )
	{
		// if checkpoint has target and trigger then is part of a quad we can test to
		// see if we are in
		CCheckPoint* target  =(CCheckPoint*)nt->GetTarget() ;
		CCheckPoint* trigger = (CCheckPoint*)nt->GetChainTrigger() ;

		if (target && trigger && target->GetChainTrigger()) 
		{
			PointList poly ;
			poly.Add(&nt->GetPos()) ;
			poly.Add(&target->GetPos());
			poly.Add(&target->GetChainTrigger()->GetPos());
			poly.Add(&trigger->GetPos()) ;
			if (mCharacter->GetPos().InsideConvexPoly(poly))
			{
				return nt ;
			}
		}
	 }
	 return NULL ;
}

//******************************************************************************************
BOOL	CCharacterLevelData::AreWeInArea()
{ 
	return GetLastFrameInKingHillArea() >= GAME.GetGameTurn()-1 ; 
} 


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

// TAG

void	CCharacterLevelData::PickedUpToken()
{
	if (mCharacter->GetState()==CS_DEAD) return ;

	mNumPickedUpTokens++ ;

	if (GAME.IsMultiPlayer()) return ;

	// have we just picked up the last one 
	if (CPUPToken::GetNumberTokensInWorld() == 1 )
	{
		mCharacter->FinishedLevel() ;
	}
}

//******************************************************************************************
void	CCharacterLevelData::DiedOnTag()
{
	if (mNumPickedUpTokens >= 5 )
	{
		mNumPickedUpTokens-=5 ;
	}
	else
	{
		mNumPickedUpTokens=0 ;
	}
}



//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

// DROID BALL

void	CCharacterLevelData::HitDroidBallPole(CDroidBall* ball_which_hit)
{
	mCharacter->IncreaseScore(50) ;

	if (GAME.IsMultiPlayer()) return ;
	if ( mCharacter->GetScore() >= GAME.GetIVar(IV_POINTS_REQUIRED_TO_WIN_LEVEL)  )
	{
		mCharacter->FinishedLevel() ;
	}
}


//******************************************************************************************
void	CCharacterLevelData::DiedOnDroidBall()
{
	if (mCharacter->GetScore() >= 50)
	{
		mCharacter->DecreaseScore(50) ;
	}
}





