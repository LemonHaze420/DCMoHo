#include	"Common.h"

#include	"BadGuys.h"
#include	"Globals.h"
#include	"Weapon.h"
#include	"HoldingZonePoint.h"

#define	BADGUY_ACCEL	GINT(0, 1311)
#define BADGUY_STUN_TIME 15

#define BADGUY_TILT_DAMP GINT(0, 55000)
#define BADGUY_TILT_MAG  GINT(0,  15000)

// if your target is less than this distance and not retreating then it will start attacking
#define START_ATTACKING_DISTANCE G(4)

// the distance away from the target when the character will change it state to GO_TO_HOLDING_POINT
#define REACHED_RETREAT_DISTANCE G(5)

#define BAD_GUY_CHECK_DEFENSE 4

#define MIN_SKILL_LEVEL_REQUIRED_TO_START_BLOCKING 4

//#define CLOSE_ENOUGH_TO_HIT G(2,64000)

#define FAILED_TO_RETREAT_TIME 100
#define BADGUY_NUM_FRAMES_END_COMBO 30



//******************************************************************************************
void	CBadGuy::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);
	SetMyAttackID(AT_BADGUY) ;

	SetMyAttackID(EAttackCharactersBF(GetInitialAttackID()));

	// skill level
	GET_COUNTER(0);

	if (mCounter0 >= 9)
	{
			mCanBlockBF |= SI_SPIN_PUNCH ;
	}

	// if no one has set it then set it to 3
	if (mCounter0 == -1)
	{
		CONSOLE.AddString("Warning: BadGuy with no skill level set, Setting it to 3") ;
		mCounter0 = 3 ;
	}

	// ambush ??
	GET_BOOL(0);
	mDefaultObjective = WAIT_FOR_A_TARGET ;

	if (mBool0)
	{
		mCurrentAction = WAIT_FOR_A_TARGET;
	}
	else
	{
		mCurrentAction = AMBUSH ;
	}

	// who do we attack bit field
	GET_COUNTER(1);


	// if no one has set it so we attack primarys
	if (mCounter1 == -1)
	{
		CONSOLE.AddString("Warning: BadGuy with attack bf not set, Setting it to attack primarys") ;
		mCounter1 = 1 ;
	}
	mToAttackBF = mCounter1 ;


	// stick to holding zones
	GET_BOOL(1);

	// do terrain checking 
	GET_BOOL(2) ;

	// holding zone number
	GET_COUNTER(2);

	mTerrainGuide.SetForCharacter(this) ;
	mHoldingPointDelay = 120 - ((10 - mCounter0)*5);

//		mPunchTime = 15 - (mCounter0 )  ;
//	mPunchTime = 21 - (mCounter0 << 1)  ;
	mPunchTime = 11 - mCounter0 ;
	mAccelSpeed = G(1) / G(84-(mCounter0<<2)) ;

	// scan distance for weapon is 10 to 15 away depeding on skill ;
	mScanDistForAWeapon= G(10) + G(mCounter0>>1) ;
	mWeaponWeWantToGoAndPickUp = NULL;

	mDefaultStunRecoveryTime = 10 - (mCounter0 >> 1) ;
	mDefenseCheckTimer = RANDOM()%7 ;
	mStopAttackingWhenPunchIsOver = FALSE ;
	mHoldingZonePoint = NULL;
	mHealthTileWeWantToGoTo = NULL ;

	mMinHoldingPointDist = 7 ;
	mMaxHoldingPointDist = 8 ;
	mBadGuyProximityRange = G(15) ;
	mSpinStunRecoveryTime = 50 - (mCounter0 << 2);
//	ChangeState(CS_SCANNING, CStateInit());

}


//********************************************************************************************************
#if TARGET == PC
void	CBadGuy::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
	WRITE_BOOL(0);
	WRITE_COUNTER(1);
	WRITE_COUNTER(2);
	WRITE_BOOL(1);
	WRITE_BOOL(2);
}
#endif


//******************************************************************************************
void	CBadGuy::Hit(CThing* that)
{
	SUPERTYPE::Hit(that);
	if (mState == CS_DEACTIVATED)
	{
		if (that->IsCharacter())
		{
			ChangeState(CS_NORMAL, CStateInit()) ;
		}
	}

	CCharacter* c = that->IsCharacter() ;

	if (!c) return ;

		// on our list of charachters to fight with, then return
	if (mState != CS_NORMAL  ||
		mState == CS_DEAD ||
		mCurrentAction == RETREAT_TO_HOLDING_POINT ||
		(!(c->GetAttackID() & mToAttackBF))) return ;


	// do we currently have a target ?
	if (mCurrentTarget !=NULL)
	{
		// if the guy who struck us is already our target then make sure we attack it
		if (mCurrentTarget == c)
		{
			if (mCurrentAction != ATTACK_TARGET)
			{
				StartAttackingTarget() ;
			}
		}
			// our target is not the guy who hit us
		else
		{
			// if we aready in attack mode then mabey change target
			if (mCurrentAction == ATTACK_TARGET)
			{
				// carry on attacking our current target
				if ((RANDOM()%3)==1) return ;

				// stop attacking out current target
				StopAttackingTarget() ;
			}

			// start attacking the guy who struck us
			mCurrentTarget = c;
			StartAttackingTarget() ;
		}
	}

	// we have no target
	else
	{
		// start attacking the guy who struck us
		mCurrentTarget = c ;
		StartAttackingTarget() ;
	}

	return ;
}

//******************************************************************************************
void	CBadGuy::ChangeCurrentAction(eBadGuyAction new_action)
{
	// clean up first

	if (mCurrentAction == ATTACK_TARGET)
	{
		if (mCurrentTarget)
		{
			mCurrentTarget->NowNotBeenAttackedBy(this) ;
		}
		else
		{
			CONSOLE.AddString("Warning: current action was attacking but had no target!!") ;
		}
	}

	mCurrentAction = new_action ;
	mStateTimer = 0;
}


//******************************************************************************************
void	CBadGuy::StateNormal()  // THIS IS THE BAD GUY AI
{
	if (UsesNonProximitryHoldingZones() && mHoldingZonePoint== NULL)
	{
		mHoldingZonePoint = WORLD.LocatePrimaryHoldingZonePoint(mCounter2) ;
	}

	// normal state for a bad guy is the fighting stuff

	// probably don't need this, but just to be safe.  if we are not attacking then we don't need
	// to say to stop attacking when the punch reaches is trigger point.
	if (mCurrentAction != ATTACK_TARGET) mStopAttackingWhenPunchIsOver = FALSE ;


	// first process all the objectives that don't required us to have a target

	switch (mCurrentAction)
	{
		case GO_FOR_A_WEAPON:
		{
			CurrentActionGoingForAWeapon() ;
			return ;
		}
		case GO_TO_HEALTH_TILE:
		{
			CurrentActionGoingToHealthTile() ;
			return ;
		}
		case RUN_AWAY:
		{
			CurrentActionRunningAway() ;
			return ;
		}
		case GO_TO_HOLDING_ZONE:
		{
			CurrentActionGoToHoldingZone() ;
			return ;
		}
	}


	// if we have a holding zone and we are outside of it then head back to it
	if (UsesNonProximitryHoldingZones() && !mHoldingZonePoint->IsPointInsideZone(mPos))
	{
		ChangeCurrentAction(GO_TO_HOLDING_ZONE) ;
		mTerrainGuide.SetNewGoalPoint(mHoldingZonePoint->FindClosestPointInZoneTo(mPos)) ;
		return ;
	}

	// get a target if we not got one

	if (mCurrentAction == AMBUSH && mCurrentTarget == NULL)
	{
		ChooseTarget() ;
		if (mCurrentTarget == NULL) return ;

	}
	if (mCurrentTarget==NULL || mCurrentAction == WAIT_FOR_A_TARGET )
	{
		ChangeCurrentAction(WAIT_FOR_A_TARGET) ;
		ChooseTarget() ;
	}

	// if we still haven't got a target then return
	if (mCurrentTarget == NULL || mCurrentAction == WAIT_FOR_A_TARGET)
	{
		// as we're doing nothing check to see if a weapon lying around
		PickupAnyWeapon(mScanDistForAWeapon ) ;

		// if we have bugger all to do make sure we don't slowly roll off somewhere we don't want to go
		if (mCurrentAction == WAIT_FOR_A_TARGET) 
		{
			mTerrainGuide.ChewTerrainCheck(mAccelSpeed, mBool2) ;
		}
		return ;
	}


	// if are target is dead then stop attacking it
	if (mCurrentTarget->GetState() == CS_DEAD ||
		mCurrentTarget->GetLevelData().GetFinishedLevel())
	{
		ResetCurrentAction() ;
		return ;
	}


	// do we do nothing if we have a proximity zone
	if (mBool1 &&
		mCurrentAction != ATTACK_TARGET &&
		!UsesNonProximitryHoldingZones() &&
		GAME.GetIVar(IV_LEVEL_TYPE) != LT_LASTMAN)
	{
		GVector p = mCurrentTarget->GetPos() ;
		GVector d = ( p - mPos);  // scale to prevent possible overflows.

		GINT	dist = d.BigMagnitude() ;

		if ( dist > mBadGuyProximityRange ) return ;
	}


	// defense first before attack unless we are ambushing
	if (mCurrentAction != AMBUSH)
	{
		Defense() ;
		if (mIsBlocking || mCurrentAction == RUN_AWAY || mCurrentAction == WAIT_FOR_A_TARGET)	return ;
	}

	switch (mCurrentAction)
	{
		case AMBUSH:						CurrentActionAmbushing() ; break;
		case GO_TO_HOLDING_POINT:			CurrentActionGoToHoldingPoint(); break ;
		case RETREAT_TO_HOLDING_POINT:		CurrentActionGoToRetreatingPoint() ; break ;
		case ATTACK_TARGET:					CurrentActionAttackTarget() ; break ;
		default:
		{
#if TARGET == PC
			char temp[50] ;
			sprintf(temp,"Unknown bad guy action %d", mCurrentAction) ;
			CONSOLE.AddString(temp) ;
#endif
		}
	}
}



//******************************************************************************************
void	CBadGuy::ResetCurrentAction()
{
	switch (mDefaultObjective)
	{
		case GO_FOR_THE_KING_OF_THE_HILL:
		{
			ChangeCurrentAction(GO_FOR_THE_KING_OF_THE_HILL) ;
			mCurrentTarget = NULL ;
			break ;
		}

		case WAIT_FOR_A_TARGET:
		{
			ChangeCurrentAction(WAIT_FOR_A_TARGET);
			ChooseTarget() ;
			break ;
		}

		default:
		{
			CONSOLE.AddString("Warning: Unknown default attack state for bad guy") ;
			break ;
		}
	}

	mStateTimer = 0;
}



//******************************************************************************************
void	CBadGuy::ChooseTarget()
{

	// get a new target
	BOOL no_primarys = FALSE ;
	if (GAME.GetGameState() >= GAME_STATE_GAME_FINISHED) 
	{
		no_primarys = TRUE ;
	}

	CListArray<CCharacter, MAX_CHARACTERS_IN_WORLD>& items = WORLD.GetCharacterNB();

	CCharacter	*current = NULL;
	GINT		mind = mCharacterSight*mCharacterSight;

	CCharacter* nt;
	for (nt = items.First();
		 nt != NULL ;
		 nt = items.Next() )
	{
		if (nt && nt->GetAttackID() & mToAttackBF && nt != this &&
			( no_primarys == FALSE || nt->IsPrimary() == FALSE))
		{

			ECharState cs = nt->GetState();

			// if we can't ambush then don't select targets which are below us

			GINT height_diff = ( nt->GetPos().Z - mPos.Z ) ;

			if (mCurrentAction == AMBUSH|| height_diff <= G(2))
			{

				// cant select targets above us
				if (cs != CS_DEAD && height_diff >= -G(2) )
				{

					GINT	dist = (mPos - nt->GetPos()).MagnitudeSq();
					if (dist < mind)
					{
						mind = dist;
						current = nt;
					}
				}
			}
		}

	}
	mCurrentTarget = current;

	// no need to do anymore if ambush
	if (mCurrentAction == AMBUSH) return ;

	if (mCurrentTarget != NULL)
	{
		SetHoldingPoint(mMinHoldingPointDist,mMaxHoldingPointDist) ;
		ChangeCurrentAction(GO_TO_HOLDING_POINT) ;
	}

}



//******************************************************************************************
void	CBadGuy::SetRunAwayPoint()
{
	GVector target_pos = GetCurrentTarget()->GetPos() ;


	GVector t_to_u = (mPos - target_pos) ;
	t_to_u.Z = G(0) ;

	GVector point_half_way = target_pos - (t_to_u/2) ;
	t_to_u.Normalise() ;


	GVector up(G(0),G(0),G(-1) ) ;

	GVector cross = up ^ t_to_u ;
	GVector candidate_point = point_half_way +( cross *12);

	if (!COORDS_INVALID(CCellRef(candidate_point) ))
	{
		mTerrainGuide.SetNewGoalPoint(candidate_point) ;
		return ;
	}

	GVector down(G(0),G(0),G(1) ) ;
	cross = down ^ t_to_u ;
	candidate_point = point_half_way +( cross *12);

	if (!COORDS_INVALID(CCellRef(candidate_point) ))
	{
		mTerrainGuide.SetNewGoalPoint(candidate_point) ;
		return ;
	}
}



//******************************************************************************************

void	CBadGuy::Defense()
{
	mDefenseCheckTimer++;

	// only check defense ever BAD_GUY_CHECK_DEFENSE cycles

	SINT df = BAD_GUY_CHECK_DEFENSE ;
	if (mCounter0 >6) df = df >> 1 ;

	if (mDefenseCheckTimer < df) return ;

	mDefenseCheckTimer=0 ;

	// check if we need to stop blocking
	if (mIsBlocking == TRUE  &&
		!mCurrentTarget->IsStriking() )
	{
		StopBlocking() ;
	}


	// runaway ?? ***** have to think about this one SRG!!! cos it produces weird results
/*
//	if (mCurrentAction != RETREAT_TO_HOLDING_POINT)
	{
		// ok lets see who is attacking us and evulate it as a threat score
		const CharacterList& items = GetBeingAttackedBy();

		SINT  overall_apponents_score = 0 ;
		CCharacter* highestscore_apponent = NULL ;
		SINT  highest_apponent_score = 0 ;
		BOOL  included_our_target = FALSE ;
		// loop through all checkpoints in world

		CCharacter* apponent;
		for (apponent = items.First();
			 apponent != NULL ;
			 apponent = items.Next() )
		{
			SINT score = apponent->GetCharacterThreatScore();
			if (apponent == mCurrentTarget) included_our_target = TRUE ;
			overall_apponents_score += score;
			if (score > highest_apponent_score)
			{
				highestscore_apponent = apponent ;
				highest_apponent_score = score ;
			}
		}

		// if our attacker is not attacking us then still add it in
		if (mCurrentTarget && !included_our_target )
		{
			SINT score = mCurrentTarget->GetCharacterThreatScore();
			overall_apponents_score += score;
			if (score > highest_apponent_score)
			{
				highestscore_apponent = mCurrentTarget ;
				highest_apponent_score = score ;
			}
		}

		// if apponents score is 3 more than are threat score the run away !!!!
		if (overall_apponents_score - 3 >= GetCharacterThreatScore() )
		{
			// easiest way to run away is make an apponent our target and set the retreat to holding point state

			if (mCurrentTarget && mCurrentAction == ATTACK_TARGET)
			{
				StopAttackingTarget() ;
			}


			mCurrentTarget = highestscore_apponent ;
			ChangeCurrentAction(RUN_AWAY) ;
			mStateTimer = 0 ;
			SetRunAwayPoint() ;

			return ;
		}
	}
*/


	// ok defence stuff with out current appoponent

	// are we close enough to bother with other checks
	if ((!mCurrentTarget->IsLongRangeStriking()) && (mCurrentTarget->GetPos() - GetPos()).MagnitudeSq() >= START_ATTACKING_DISTANCE*START_ATTACKING_DISTANCE )
	{
		return ;
	}

	// check if we need to block now!;
	if (mCounter0 >= MIN_SKILL_LEVEL_REQUIRED_TO_START_BLOCKING &&
		mIsBlocking == FALSE &&
		!IsStriking() &&
		mCurrentTarget->IsStriking() )
	{

		// check if blocking actualy gonna work
	

		// ok try and block if they are facing us and we are facing them 
		GINT angle_attacker_to_you = AngleDifference(mYaw, mCurrentTarget->GetYaw() ) ;
		if (GABS(angle_attacker_to_you) > ( G_PI - BLOCKING_DEFFENSE_ANGLE )  )
		{
		//	shall we block ? throw a dice depending on are skill level

			if (mCounter0 >= 6 )
			{
				if ( RANDOM()%2 == 0 )
				{
					BOOL res = CounterAttack(TRUE) ;
					if (res) return ;
				}
					
			}
			SINT percent = (mCounter0 >> 1) * 16 ;
			if (RANDOM()%100 <= percent)
			{
				StartBlocking() ;
				return ;
			}
		}
		else
		{
			// ok are we facing them
			GVector a1 = mCurrentTarget->GetPos() - mPos;
			GINT angle1 = -GATAN2(a1.X, a1.Y) ;
			BOOL facing_them = FALSE ;
			if (GABS(AngleDifference(mYaw, angle1)) < BLOCKING_DEFFENSE_ANGLE) facing_them = TRUE ;
			BOOL res = CounterAttack(facing_them) ;
			if (res) return ;
		}
	}

	// sometimes it is best to attack back

	// as are target is very close go straight into attack mode if we are not attacking
	if (mCurrentAction == GO_TO_HOLDING_POINT )
	{
		// if for ever reason your target is very close go straight into attack mode
		// odds on attacking are based on skill level

		SINT percent = 20 + (mCounter0*5) ;

		// if no one attacking it though then make it 100%
		if (mCurrentTarget->GetBeingAttackedBy().Size() < 1)
		{
			percent = 100 ;
		}

		if (percent > RANDOM()%100)
		{
			StartAttackingTarget() ;
			// make sure we punch straight away
			mStateTimer = mPunchTime ;
		}
	}
}





//******************************************************************************************
void	CBadGuy::CurrentActionRunningAway()
{
	if (RANDOM()%2 == 1) mStateTimer ++;

	if (mIsBlocking == TRUE) StopBlocking() ;

	if (!mCurrentTarget)
	{
		ResetCurrentAction();
		return ;
	}

	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

	// is guy we running awat from still near by
	if (dist_to_target < G(8) )
	{
		SetRunAwayPoint() ;
		FaceGoToPoint() ;

		if (mFlags & TF_ON_GROUND )
		{
			GINT accell_speed = mAccelSpeed  * G(1, 32768) ;
			mTerrainGuide.MoveMe(accell_speed, FALSE, mBool2) ;

		}
		return ;
	}

	// we've managed to run away
	ResetCurrentAction();
}


//******************************************************************************************
BOOL CBadGuy::CounterAttack(BOOL facing_target) 
{
	if (facing_target) 
	{
		Attack(0) ;
		return TRUE ;
	}
	return FALSE ;
}

//******************************************************************************************
void	CBadGuy::CurrentActionGoToRetreatingPoint()
{
	if (RANDOM()%2 == 1) mStateTimer ++;

	GINT accell_speed = mAccelSpeed  * G(1, 32768) ;

	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

	if (mFlags & TF_ON_GROUND && IsUpright())
	{
		mTerrainGuide.MoveMe(accell_speed, FALSE, mBool2) ;
	}

	if (dist_to_target > REACHED_RETREAT_DISTANCE ||
		mStateTimer > FAILED_TO_RETREAT_TIME)
	{
		ResetCurrentAction() ;
	}
}

//******************************************************************************************
void	CBadGuy::CurrentActionGoToHoldingZone()
{
	if (RANDOM()%2 == 1) mStateTimer ++;

	GINT accell_speed = mAccelSpeed  * G(1, 32768) ;

	if (mFlags & TF_ON_GROUND && IsUpright())
	{
		mTerrainGuide.MoveMe(accell_speed, TRUE, mBool2) ;
	}

	if (mHoldingZonePoint->IsPointInsideZone(mPos))
	{
		ResetCurrentAction() ;
	}
}





//******************************************************************************************
void	CBadGuy::CurrentActionAmbushing()
{
	mTerrainGuide.SetNewGoalPoint(mCurrentTarget->GetPos()) ;

	// set the ignore height difference to true when moving
	mTerrainGuide.MoveMe(mAccelSpeed, FALSE, FALSE) ;

	FaceTarget() ;
	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

	if (dist_to_target < CLOSE_ENOUGH_TO_HIT )
	{
		StartAttackingTarget() ;
	}

}

//******************************************************************************************

void	CBadGuy::StartAttackingTarget()
{
	// quick check to make sure we ant already attacking
	if (mCurrentAction != ATTACK_TARGET)
	{
		if (mCurrentTarget->TooManyPeopleAttacking()) mCurrentTarget->StopFurthestAwayAttacker() ;
		mCurrentTarget->NowBeenAttackedBy(this) ;
		ChangeCurrentAction(ATTACK_TARGET) ;
		mStateTimer = 0 ;
		SetAttackingPoint() ;

		// stop too many people attacking at once
		if (mCurrentTarget->GetBeingAttackedBy().Size() > 2 )
		{
			if (RANDOM()%2 == 0 )
			{
				mCurrentTarget->StopFurthestAwayAttacker() ;
			}
		}
		return ;
	}

	CONSOLE.AddString("Warning: Could not start attacking target cos already was") ;
}



//******************************************************************************************

void	CBadGuy::StopAttackingTarget()
{
	// quick check to make sure we are attacking
	if (mCurrentAction == ATTACK_TARGET)
	{
		ChangeCurrentAction(RETREAT_TO_HOLDING_POINT) ;
		mStateTimer = 0 ;
		SetHoldingPoint(mMinHoldingPointDist,mMaxHoldingPointDist) ;
		return ;
	}

	CONSOLE.AddString("Warning : Could not stop attacking target cos i wasn't attacking") ;
}





//******************************************************************************************
//  This function gets called whenever this character has been struct ( normally from another
//  character ).  The strike characteristics are passed in. At this level we decided whether or
//  not retaliate

BOOL	CBadGuy::Strike(const CStrike& strike)
{
	BOOL result;
	if (mState == CS_DEACTIVATED) ChangeState(CS_NORMAL, CStateInit()) ;
		
	ECharState	StateBeforeBeingStruck = mState;

	result = SUPERTYPE::Strike(strike);

	// Set up so we attack the guy who struck us,
	CCharacter* guy_attacking_me = strike.mOriginator ;

	// if we are not fighting or if we are retreating or they guy who struck us is not
	// on our list of charachters to fight with, then return
	BOOL hit_now=FALSE ;
	if (StateBeforeBeingStruck == CS_KNOCKED_OUT && result == TRUE && mCounter0 > 7)
	{
		SetAnimMode(AM_WALK);
		ChangeState(CS_NORMAL, CStateInit());
		StateBeforeBeingStruck = CS_NORMAL ;
		hit_now = TRUE ;
		
	}

	if (StateBeforeBeingStruck != CS_NORMAL  ||
		mState == CS_DEAD ||
		mCurrentAction == RETREAT_TO_HOLDING_POINT ||
		(!(guy_attacking_me->GetAttackID() & mToAttackBF))) return result;


	if (mState == CS_KNOCKED_OUT)
	{
		// quick check to make sure we are attacking
		if (mCurrentAction == ATTACK_TARGET)
		{
			StopAttackingTarget() ;
		}
		return result;
	}


	// do we currently have a target ?
	if (mCurrentTarget !=NULL)
	{
		// if the guy who struck us is already our target then make sure we attack it
		if (mCurrentTarget == guy_attacking_me)
		{
			if (mCurrentAction != ATTACK_TARGET)
			{
				StartAttackingTarget() ;
			}
		}
			// our target is not the guy who hit us
		else
		{
			// if we aready in attack mode then mabey change target
			if (mCurrentAction == ATTACK_TARGET)
			{
				// carry on attacking our current target
				if ((RANDOM()%2)==1) return result;

				// stop attacking out current target
				StopAttackingTarget() ;
			}

			// start attacking the guy who struck us
			mCurrentTarget = guy_attacking_me;
			StartAttackingTarget() ;
		}
	}

	// we have no target
	else
	{
		// start attacking the guy who struck us
		mCurrentTarget = guy_attacking_me ;
		StartAttackingTarget() ;
	}

	if (hit_now) 	mStateTimer = mPunchTime ;
	return result;
}


//******************************************************************************************
void	CBadGuy::ClearYourTarget()
{
	if (mCurrentAction != ATTACK_TARGET)
	{
		CONSOLE.AddString("Warning:  could not clear my target as i wasn't attacking!") ;
	}
	ChangeCurrentAction(mDefaultObjective);

	SUPERTYPE::ClearYourTarget() ;

}



//******************************************************************************************
void	CBadGuy::FaceTarget()
{


	// if pitch over 30 degs then can't face target

	if (!IsUpright()) return;

	GVector u_to_t =  (mCurrentTarget->GetPos() - mPos ) ;
	GINT angle_you_to_target = GATAN2(u_to_t.X, u_to_t.Y);
	mDestYaw = -angle_you_to_target ;

	// hmmmm  !!
	if (mDestYaw > G_PI_2 && mYaw < -G_PI_2 )
	{
		mYaw+=G_2PI ;
	}

	if (mDestYaw < -G_PI_2 && mYaw > G_PI_2 )
	{
		mYaw-=G_2PI ;
	}
}



//******************************************************************************************
void	CBadGuy::FaceGoToPoint()
{
	if (!IsUpright()) return ;

	GVector u_to_t =  (mTerrainGuide.GoalPoint() - mPos ) ;
	GINT angle_you_to_target = GATAN2(u_to_t.X, u_to_t.Y);
	mDestYaw = -angle_you_to_target ;

	// hmmmm  !!
	if (mDestYaw > G_PI_2 && mYaw < -G_PI_2 )
	{
		mYaw+=G_2PI ;
	}

	if (mDestYaw < -G_PI_2 && mYaw > G_PI_2 )
	{
		mYaw-=G_2PI ;
	}

}




//******************************************************************************************
void	CBadGuy::StateStunned()
{
	SUPERTYPE::StateStunned() ;

	// if we have now recovered from stunned state and we are attacking set next strike time.

	if (mState == CS_NORMAL && mCurrentAction == ATTACK_TARGET)
	{
		// this will cause it to launch a punch on next frame
		mStateTimer = mPunchTime  ;
	}

	if (GAME.GetIVar(IV_LEVEL_TYPE)==LT_KING)
	{
		mTerrainGuide.ChewTerrainCheck(mAccelSpeed, mBool2) ;
	}
}


//******************************************************************************************
void	CBadGuy::StateAsleep()
{
	// rock!

	// mtilt no longer exists

	mPitch = (GSIN(G(GAME.GetGameTurn() + mNumber * 43) / 10) + G1) / 10;

	SUPERTYPE::StateAsleep();
}


//******************************************************************************************
void	CBadGuy::StateKnockedOut()
{

	// recover depending on how are skill level is

	if (RANDOM()%100 <= (50 +(mCounter0*5)))
	{
		ReduceKnockoutTime(1) ;
	}

	if (mCounter0 > 7)
	{
		ReduceKnockoutTime(1) ;
	}
	SUPERTYPE::StateKnockedOut() ;

}


//******************************************************************************************
#define SCAN_THETA	G(0, 10000)

void	CBadGuy::StateScanning()
{
	// scan around in a circle.  wake up when the player is near and we see him.

	SitOnGround();

	mYaw += G(0, 1000);

	if (mYaw > G_PI  - SCAN_THETA)
		mYaw -= G_PI * 2;
	mOrientation2.MakeRotationYaw(-mYaw);


	// and the search.

	GVector r	   = WORLD.GetPrimaryPosition() - mPos;

	GINT	l = r.MagnitudeSq();

	if (l < G(12 * 12))
	{
		GINT	th = GATAN2(r.X, r.Y);

		th =th - mYaw  ;

#if TARGET == PC
//		dont fookin work
//		char text[200];
//		sprintf(text, "ry = %f, x=%f y=%f  th=%f", GToF(mYaw),  GToF(r.X), GToF(r.Y), GToF(th));
//		CONSOLE.AddString(text);
#endif

	//	if (GABS(th) < G(0,51472) )
	//		ChangeState(CS_NORMAL, CStateInit());
	}
}



//******************************************************************************************
BOOL	CBadGuy::Attack(SINT no)
{
	//! use number!

	if (!SUPERTYPE::Attack(no)) return FALSE ;

	if (mHoldingThing && mHoldingThing->IsWeapon())
		SetAnimMode(AM_WEAPON_ATT);
	else
	{
		if ((mAnimMode == AM_JUMP) || (mAnimMode == AM_MID_JUMP))
			SetAnimMode(AM_JUMP_ATTACK);
		else
		{

			if (mCurrentPunchSequenceNum >= mCombatProfile.mNumAttacksForCombo)
			{
				mCurrentPunchSequenceNum = 0 ;
			}

			SINT diff = GAME.GetGameTurn() - mLastPunchFrameNum ;
				// combo stuff

			// SRG combo still builds up if blocked
			if (diff < BADGUY_NUM_FRAMES_END_COMBO && mLastPunchResult != PR_MISSED)
			{
				mCurrentPunchSequenceNum++ ;
			}
			else
			{
				mCurrentPunchSequenceNum = 0 ;
			}

			SetAnimMode(AM_PUNCH) ;
		}
	}

	return TRUE ;
}


//******************************************************************************************
void	CBadGuy::AnimTrigger(const GMatrix& current_anim_orientation, BOOL end_of_punch_hit)
{

	SUPERTYPE::AnimTrigger(current_anim_orientation, end_of_punch_hit) ;
	if ( IsFinalAttackAnim(mAnimMode) &&
		 mStopAttackingWhenPunchIsOver &&
		 mCurrentTarget &&
		 mCurrentAction == ATTACK_TARGET)
	{
		StopAttackingTarget() ;
		mStopAttackingWhenPunchIsOver=FALSE ;
	}
}




//******************************************************************************************
void	CBadGuy::Process()
{
	SUPERTYPE::Process();

}

//********************************************************************************************************
void	CBadGuy::StateFinished()
{
	SUPERTYPE::StateFinished() ;

	//if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_KING)
//	{
	//	/mDestYaw = G_PI ;
//	}
}

#if TARGET == PC

//******************************************************************************************
void	CBadGuy::DrawDebugText(HDC hdc, SINT* y_offset, char* buffer)
{
	SUPERTYPE::DrawDebugText(hdc, y_offset, buffer) ;
	SINT size ;
	size = sprintf(buffer, "---------------------- BAD GUY -------------------");
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;
	switch (mCurrentAction)
	{
	case GO_TO_HOLDING_POINT:	size = sprintf(buffer, "Current Action = GO TO HOLDING POINT"); break;
	case ATTACK_TARGET:size = sprintf(buffer, "Current Action = ATTACKING ATRGET"); break;
	case RETREAT_TO_HOLDING_POINT:size = sprintf(buffer, "Current Action = RETREAT TO HOLDING POINT"); break;
	case AMBUSH:size = sprintf(buffer, "Current Action = AMBUSH"); break;
	case WAIT_FOR_A_TARGET:size = sprintf(buffer, "Current Action = WAIT FOR TARGET"); break;
	case RUN_AWAY:size = sprintf(buffer, "Current Action = RUN AWAY"); break;
	case GO_FOR_A_WEAPON:size = sprintf(buffer, "Current Action = GO FOR A WEAPON"); break;
	case GO_FOR_THE_KING_OF_THE_HILL:size = sprintf(buffer, "Current Action = GO FOR KING OF THE HILL"); break;
	case GO_TO_HOLDING_ZONE:size = sprintf(buffer, "Current Action = GO TO HOLDING ZONE"); break;
	case GO_TO_HEALTH_TILE:size = sprintf(buffer, "Current Action = GO TO HEALTH TILE"); break;
	default: size = sprintf(buffer, "Current Action = %d", mCurrentAction); break;
	}

	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;

		switch (mDefaultObjective)
	{
	case WAIT_FOR_A_TARGET:size = sprintf(buffer, "Default objective = WAIT FOR TARGET"); break;
	case GO_FOR_THE_KING_OF_THE_HILL:size = sprintf(buffer, "Default objective = GO FOR KING OF THE HILL"); break;
	default: size = sprintf(buffer, "Current Action = %d", mCurrentAction); break;
	}
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;

	size = sprintf(buffer, "Skill level = %d", mCounter0);
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;

	ENGINE.RenderArrow(GVectorToFVector(GetRenderPos()), GVectorToFVector(mTerrainGuide.GoalPoint()), 0xff40ff20);
	size = sprintf(buffer, "Dist to Goal point = %f", GToF((mPos-mTerrainGuide.GoalPoint()).Magnitude()));
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;
}

#endif


CCharacterCombatProfile PrisonerCombatProfile = { 80, 5,4,15,16,0,0,0,2 } ;
CCharacterCombatProfile GuardCombatProfile = { 120,3,8,22,20,0,0,0,3 };
CCharacterCombatProfile SGuardCombatProfile = { 100,4,5,20,18,0,0,0,3 };
CCharacterCombatProfile DeputyCombatProfile = { 120,3,8,25,22,0,0,0,0 };
CCharacterCombatProfile MarshallCombatProfile = { 150,3,9,25,25,0,0,0,0 };

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************


