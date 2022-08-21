#include	"Common.h"

#include	"BadGuys.h"
#include	"Globals.h"
#include	"Weapon.h"
#include	"HoldingZonePoint.h"

// if the counter reaches this without the character still being close enough to attack then give up
// it's attack
#define GIVE_UP_TRYING_TO_ATTACK_COUNT 80


//******************************************************************************************
void	CBadGuy::CurrentActionAttackTarget()
{
	if (RANDOM()%2 == 1) mStateTimer ++;

	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

	SetAttackingPoint() ;

	if (mFlags & TF_ON_GROUND && IsUpright())
	{
		if (dist_to_target > CLOSE_ENOUGH_TO_HIT)
		{
			mTerrainGuide.MoveMe(mAccelSpeed, FALSE, mBool2) ;

			// as we are not close enough to hit have a quick search around to see if there is any weapons
			// lying around nearby
			if (PickupAnyWeapon( mScanDistForAWeapon ) ) return  ;


			// if muliplayer re evaluate more often
			if (GAME.IsMultiPlayer())
			{
				if (RANDOM()%100 == 12)
				{
					ResetCurrentAction() ;
				}
			}
		}
		else
		{
			mTerrainGuide.ChewTerrainCheck(mAccelSpeed, mBool2) ;
		}
	}

	FaceGoToPoint() ;

	if (!mCurrentTarget)
		return;

	// if last hit was blocked decrease the rate in which the bad guy can attack

	SINT next_time_allowed_to_attack = mPunchTime ;
	
	if (mLastPunchResult==PR_BLOCKED)
	{
			next_time_allowed_to_attack = 18 ;
			next_time_allowed_to_attack -= (mCounter0>>1) ;
	} 

	if (mCurrentTarget->GetState() == CS_KNOCKED_OUT) next_time_allowed_to_attack += (mPunchTime<<1);

	if (dist_to_target < CLOSE_ENOUGH_TO_HIT && mStateTimer > next_time_allowed_to_attack && IsUpright() )
	{	
		// is target knocked out then only stike if we are skill level 8 or above
		if (mCurrentTarget->GetState() != CS_KNOCKED_OUT || mCounter0 >= 8 )
		{
			// if attack was allowed
			if (DoAnAttack()) 
			{
				mStateTimer = 0;
				// have we reached then end of our attack
				// SRG Think this needs to be changed
				if (mCurrentPunchSequenceNum != 0 && (RANDOM()%4)==1)
				{
					// extra check so only stop attacking if out last attack missed
					SINT diff = GAME.GetGameTurn() - mLastPunchFrameNum ; 
					if (diff > 110 || mLastPunchResult != PR_HIT) 
					{
						mStopAttackingWhenPunchIsOver = TRUE ;
					}
				}
			}
		}
		else
		{
			mStateTimer = 0 ;
		}
	}

	if (mStateTimer > GIVE_UP_TRYING_TO_ATTACK_COUNT ||
		( GAME.GetGameState() >= GAME_STATE_GAME_FINISHED && mCurrentTarget->IsPrimary() ) )
	{
		StopAttackingTarget() ;
		ResetCurrentAction() ;
	}
}


//******************************************************************************************
BOOL	CBadGuy::DoAnAttack()
{
	// at this level they only no how do do a punch
	return Attack(0) ; 
}


//******************************************************************************************
void	CBadGuy::SetAttackingPoint()
{
	
	// calc angle between u and t

	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

	GINT dist_to_stop_and_hit = CLOSE_ENOUGH_TO_HIT ;

	if (mCurrentTarget->GetState() == CS_KNOCKED_OUT && mCounter0 < 8)
	{
		dist_to_stop_and_hit *= G(1,52643);
	}

	if (dist_to_target < dist_to_stop_and_hit )
	{
		GVector offset = mPos - mCurrentTarget->GetPos() ;
		offset/= 128 ; 
		mTerrainGuide.SetNewGoalPoint( mPos - offset ) ;
		return ;
	}

	// try to head towards where the target is going.

	GVector t_to_u =  (mPos - mCurrentTarget->GetPos()  ) ;
	GINT angle_you_to_target = GATAN2(t_to_u.X, t_to_u.Y);

	GVector velocity = mCurrentTarget->GetVelocity() ;
	velocity.Normalise() ;

	GINT velocity_a =   GATAN2(t_to_u.X, t_to_u.Y);
	
	GINT angle_diff = GABS(AngleDifference(velocity_a, angle_you_to_target ) ) ;


	GINT angle_mul = G_PI_2 / G(1, 16384) ;

	if (angle_diff > G_PI_2  )
	{
		velocity*= (dist_to_target * 2) ;
	}
	else
	{
		velocity*= (dist_to_target * ((angle_mul*angle_diff)+G(0,49152) ) );

	}


	mTerrainGuide.SetNewGoalPoint( mCurrentTarget->GetPos() + velocity );
}