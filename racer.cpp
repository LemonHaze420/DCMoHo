#include	"Common.h"

#include	"Globals.h"
#include	"Racer.h"
#include	"CheckPoint.h"


//******************************************************************************************
void	CRacer::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	GET_TARGET;

	GET_COUNTER(0);

	// if no one has set it then set it to 3
	if (mCounter0 == -1)
	{
		CONSOLE.AddString("Warning: Racer with no skill level set, Setting it to 3") ;
		mCounter0 = 3 ;
	}

	GET_COUNTER(1) ;
	if (mCounter1 == -1)
	{
	//	CONSOLE.AddString("Warning: Racer No Hit Frequency Set, Setting it to 5") ;
		mCounter1= 5;
	}

	// i love fixed point maths
	// get a number that varies from 0.7 to 1.15258  depending on skill level
	mRacingSpeed = G(0, 45875) + G(0, (2966*mCounter0));

	if (i->mAngleType == AT_YPR)
	{
		mYaw	= i->mYaw;
//		mPitch	= i->mPitch;
//		mRoll	= i->mRoll;
	}
	else
	{
		mYaw	= G0;
//		mPitch	= G0;
//		mRoll	= G0;
	}

	SitOnGround();
	mSetGoalPoint = FALSE ;
	mPos_b4= GVector(G0,G0,G0) ;
	mStartRevingTime = 25+(RANDOM()%50) ;
	mTimeTryingToGetToNextCheckPoint = 0 ;
	mBackTrackingToLastCheckpoint = FALSE ;

}

//******************************************************************************************
#define RACER_JUMP_POWER		(-G(0, 27000))
#define RACER_ALONG_POWER		(G(0, 1600))
#define RACER_ALONG_POWER2		(G(0, 800))

#define RACER_WAYPOINT_PROXIMITY (G(6) * G(6))

#define RACER_TURN_SPEED		(G(0, 3000))


//********************************************************************************************************
void	CRacer::CalcNewGoalPoint(SINT for_check_point)
{
	CCharacterLevelData* cld = &GetLevelData();
	CCheckPoint* goal_check_point = cld->GetShortestDistToCheckPointsSQ(for_check_point).GetCheckPoint() ;

	// stops it crashing if we ant set up any checkpoints
	if (!goal_check_point)
	{
		mGoalPoint = mPos;
		mSetGoalPoint = TRUE ;
		return ;
	}
	// calc point which is half way between this checkpoint and the checkpoints target checkpoint

	GVector p1 = goal_check_point->GetPos() ;
	GVector p2 = goal_check_point->GetTarget()->GetPos() ;

	GINT rand ;

	if (goal_check_point->IsJumpCheckPoint()) 
	{
		rand = G(0,32768) ;
	}
	else
	{
		rand = G(0,RANDOM()%16384) + G(0,24576)  ;
	}
	
	mGoalPoint.X = ((p2.X - p1.X) * rand ) + p1.X ;
	mGoalPoint.Y = ((p2.Y - p1.Y) * rand ) + p1.Y ;
	mGoalPoint.Z = G(0) ;

	mGoalPoint.Z = MAP.GetMapHeight(mGoalPoint) ;
	mSetGoalPoint = TRUE ;
}


//********************************************************************************************************
void	CRacer::ReachedCheckPoint(SINT check_point_num) 
{
	CCharacterLevelData* cld = &GetLevelData();

	// if we haven't arived at new checkpoint then do nothing
	if (cld->GetLastCheckPoint() == check_point_num)
	{
		if (!mBackTrackingToLastCheckpoint)
		{
			return ;
		}
		mBackTrackingToLastCheckpoint = FALSE ;
	}
	else
	{
		// ok update are level data stuff 
		SUPERTYPE::ReachedCheckPoint(check_point_num) ;
	}

	// calc which checkpoint number we want to goto next

	mTimeTryingToGetToNextCheckPoint = 0;
	mNumJumpsToNextCheckpoint = 0;
	SINT next_check_point ;
	if (cld->GetLastCheckPoint() == GAME.GetIVar(IV_NUM_CHECKPOINTS))
	{
		next_check_point = 0 ;
	}
	else
	{
		next_check_point = cld->GetLastCheckPoint() + 1 ;
	}

	CalcNewGoalPoint(next_check_point) ;
}


//********************************************************************************************************
void	CRacer::StateFinished()
{
	StateNormal() ;
}

//********************************************************************************************************
void	CRacer::StateNormal()
{

	// state normal is the race stuff

	// if we ant set a goal point (only happens at start of the race level) then
	// set it to the start/finish checkpoints
	if (!mSetGoalPoint) CalcNewGoalPoint(0) ;


	GVector d = (  mGoalPoint - GetPos());  // scale to prevent possible overflows.
	GINT dist  = d.BigMagnitude() ;
	CCharacterLevelData* cld = &GetLevelData();

	mTimeTryingToGetToNextCheckPoint++ ;

	// incase we miss a teleport
	if (dist > G(60) ) 
	{
		if (mTimeTryingToGetToNextCheckPoint > 200 )
		{
			CalcNewGoalPoint(cld->GetLastCheckPoint()) ;
			mBackTrackingToLastCheckpoint = TRUE ;
		}
		return ;
	}
	

	// don't attack right at start of level
	if (GAME.GetGameTurn() > 7*25)
	{
	
		// do a side punch 
		BOOL someone_near_by = FALSE ;
		CThing* a_thing_near_by_in_map_who  = FindFirstCollision(mPos, G(2)) ;
		while (a_thing_near_by_in_map_who && someone_near_by == FALSE)
		{
			CCharacter* a_racer_near_by_in_map_who = a_thing_near_by_in_map_who->IsCharacter() ;

			if ( a_racer_near_by_in_map_who && 
				 a_racer_near_by_in_map_who->GetState() != CS_DEAD && 
				 a_racer_near_by_in_map_who->GetLevelData().GetFinishedLevel() == FALSE &&
				 (a_racer_near_by_in_map_who->GetPos() - mPos).MagnitudeSq() < G(4)  )
			{
				someone_near_by = TRUE ;
			}
			a_thing_near_by_in_map_who = FindNextCollision() ;
		}

		if (someone_near_by)
		{
			SINT r=RANDOM()%100 ;
			if ((mCounter1 <<2) > r)
			{
				RacerAttack() ;
			}
		}
	}


	// ok move the racer

	d.Normalise() ;
	// accelerate if we are facing the right way, otherwise slow down.
	GVector power;
	GINT race_speed = mRacingSpeed ;

	// SRG ok this would be cool but brakes the AI so i am commenting it out
//	if (mState == CS_FINISHED) race_speed*=G(0,34000) ;

	if (mFlags & TF_ON_GROUND)
		Thrust(d * RACER_ALONG_POWER * race_speed ) ; 
	else
		Thrust(d * RACER_ALONG_POWER2 * race_speed ) ; 

		// incase we get stuck jump ??
	if (GAME.GetGameTurn()%8 ==0  )
	{
		if (mPos_b4!= GVector(G0,G0,G0) )
		{
			if ((mPos- mPos_b4).MagnitudeSq() < G(0,3034) )
			{
				Thrust(d * RACER_ALONG_POWER * race_speed * 8 ) ; 
				Jump() ;
				mNumJumpsToNextCheckpoint++ ;
			}
		}
		mPos_b4 = mPos;
	}


	// ok if we haven't reached the next checkpoint by 200 game cycles + we have tried jumping
	// to next checkpoint then back track to previous checkpoint

	if (mTimeTryingToGetToNextCheckPoint > 200 && mNumJumpsToNextCheckpoint != 0)
	{
		// ok fool the racer in thinking we have just arrived at the checkpoint
		// before the previous one.  This should make it head to the previous
		// checkpoint
		CalcNewGoalPoint(cld->GetLastCheckPoint()) ;
		mBackTrackingToLastCheckpoint = TRUE ;
	}

}


//******************************************************************************************
void   CRacer::Process()
{
	SUPERTYPE::Process() ;

	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER && 
		mRacerBoostVector != ZERO_GVECTOR && 
		GAME.IsGameRunning())
	{

		if (RANDOM()%30 ==1 )
		{
			mVelocity = mRacerBoostVector *2;
			mRacerBoostVector = ZERO_GVECTOR ;
		}
		else
		{
			mRacerBoostVector.Normalise() ;
			mRacerBoostVector*=G(0,16000) ;
			mVelocity = mRacerBoostVector ;
			mRacerBoostVector = ZERO_GVECTOR ;
		}
	}


	if (GAME.IsGameStarting() && GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER && !(GAME.IsHelp()))
	{
		if (GAME.GetGameTurn() > mStartRevingTime )
		{
			RacerBoost() ;
		}
	}
}

	

//******************************************************************************************
void	CRacer::RacerBoost() 
{
	GVector v = GVector(G0,G1,G0) ;
	v =mOrientation2 * v  ;

	SINT r = RANDOM()%1400 ;
	r-=700;
	v*=G(0,1500+r) ;


	mRacerBoostVector+=v ;

	
	if ((GABS(v.X) > G(0, 10)) || (GABS(v.Y) > G(0, 10)))
	{
		// yaw - face direction of travel
		GINT	ndy = GATAN2(v.Y, v.X) - G_PI_2;

		if (mDestYaw - ndy >  G_PI)
			ndy += G_PI * 2;
		if (mDestYaw - ndy < -G_PI)
			ndy -= G_PI * 2;

		mDestYaw = ndy;

		// Pitch
		GMatrix mat;

		mat.MakeRotationYaw(mYaw);

		GINT	dot = (mat * GVector(G0, G1, G0)) * v;

//		mDestPitchA = dot * 30;	   // play with this
		mDestPitchA = dot * mHowMuchILeanForwardWhenRunning;
	}
	//CONSOLE.AddString("here man !!") ;
}


//********************************************************************************************************
#if TARGET == PC
void	CRacer::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
	WRITE_COUNTER(0);
}
#endif
