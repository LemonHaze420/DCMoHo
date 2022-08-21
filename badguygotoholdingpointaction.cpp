#include	"Common.h"

#include	"BadGuys.h"
#include	"Globals.h"
#include	"Weapon.h"
#include	"HoldingZonePoint.h"

//  If the target moves this far away from the point where this badguy calculated its holding point
//  then it will recalc a new holding point
#define RE_CALC_HOLDING_POINT_DIST G(8) 


#define MAX_HOLDING_ANGLE_CHANGE G_PI_4

//******************************************************************************************
void	CBadGuy::CurrentActionGoToHoldingPoint()
{

	// shall we go and pick a weapon instead ?
	if (PickupAnyWeapon(mScanDistForAWeapon ) ) return ;

	// if target moved along away from the position when we calculated the holding point then reset

	if ( (mTargetPosWhenSettingHoldingPoint - mCurrentTarget->GetPos()).MagnitudeSq() > RE_CALC_HOLDING_POINT_DIST * RE_CALC_HOLDING_POINT_DIST )
	{
		ResetCurrentAction();

		return ;
	}


	if (GAME.IsMultiPlayer())
	{
		if (RANDOM()%100== 12)
		{
			ResetCurrentAction() ;
		}
	}

	if (RANDOM()%2 == 1) mStateTimer ++;

	GINT accell_speed = mAccelSpeed ;
	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

	if (mFlags & TF_ON_GROUND && IsUpright())
	{
		mTerrainGuide.MoveMe(accell_speed, FALSE, mBool2) ;
	}


	// face target
	FaceTarget() ;

	BOOL allowed_to_attack = TRUE;
	if (UsesNonProximitryHoldingZones() && !mHoldingZonePoint->IsPointInsideZone(mCurrentTarget->GetPos()))
	{
		allowed_to_attack = FALSE ;
	}

	if (mStateTimer > mHoldingPointDelay )
	{	
		mStateTimer = 0;

		// do we attack or go to another holding point

		// work out a percentage chance of attacking from number of people aready attacking and 
		// skill level 

		int people_not_attacking = NUM_OF_GUYS_ALLOWED_TO_ATTACK_ONE_GUY_AT_SAME_TIME - mCurrentTarget->GetBeingAttackedBy().Size() ;
		GINT pc = G(50) / NUM_OF_GUYS_ALLOWED_TO_ATTACK_ONE_GUY_AT_SAME_TIME ;
		pc = pc * people_not_attacking ;
		pc += G(mCounter0*4) ;
		SINT pcwhole = pc.Whole() ;
		
		if (!mCurrentTarget->TooManyPeopleAttacking() &&
			pcwhole > (RANDOM())%100 &&
			allowed_to_attack)
		{
			StartAttackingTarget() ;
		}
		else
		{
			ResetCurrentAction() ;
			return ;
		}
	}
	else
	{
		// if no one attacking it and it is a primary then attack it anyway

		if ((mCurrentTarget->GetAttackID() == AT_PRIMARY || 
			GAME.GetIVar(IV_LEVEL_TYPE) == LT_KING ||
			GAME.GetIVar(IV_LEVEL_TYPE) == LT_LASTMAN )  &&
			mCurrentTarget->GetBeingAttackedBy().Size() == 0 &&
			allowed_to_attack)
		{
			StartAttackingTarget() ;
		}
	}
}



//******************************************************************************************
void	CBadGuy::SetHoldingPoint(SINT min_holding_point,
								 SINT max_holding_point)
{
	GVector target_pos = GetCurrentTarget()->GetPos() ;

	GINT angle_target_to_candidate ;

	GVector candidate_pos ;
	GVector t_to_u = (mPos - target_pos) ;
	GINT angle_target_to_you = GATAN2(t_to_u.X, t_to_u.Y) ;

	BOOL found_point = FALSE ;
	SINT attempt = 0 ;

	// ok quick speed up check if we are using holding zones and out target is so far
	// away, then no holding point will be inside the holding zone
	if (UsesNonProximitryHoldingZones())
	{
		GVector closest_point = mHoldingZonePoint->FindClosestPointInZoneTo(target_pos) ;
		if ((closest_point-target_pos).MagnitudeSqXY() > G(max_holding_point * max_holding_point ))
		{
			return ;
		}
	}


	// we also check that it may not be able to find a point so break out of 50 attempts
	while (!found_point && attempt< 50)
	{

		// Find a candidate point by selection a random point inside an imaginary 
		// box, where the target would be in the centre of it.
		candidate_pos = target_pos ;
		static SINT mod1 = (max_holding_point)*1024 ;
		static SINT mod2 = (max_holding_point)*1024;
		GINT rand_x_dist = G(RANDOM()%mod1) ;
		GINT rand_y_dist = G(RANDOM()%mod2) ;
		rand_x_dist/=G(512) ;
		rand_y_dist/=G(512) ;
		candidate_pos.X+=rand_x_dist- G(max_holding_point);
		candidate_pos.Y+=rand_y_dist- G(max_holding_point);

		// now we check that the point is in within the holding point distance window (i.e.
		// not too far away and not too close)

		GINT dist_target_to_cand = (candidate_pos - target_pos).MagnitudeSq() ;

		if (dist_target_to_cand > G(min_holding_point * min_holding_point) &&
			dist_target_to_cand < G(max_holding_point * max_holding_point ) &&
			!COORDS_INVALID(CCellRef(candidate_pos)) )
		{	
			
			// is point inside our holding zone if we have a holding zone
			if(!UsesNonProximitryHoldingZones() || mHoldingZonePoint->IsPointInsideZone(candidate_pos))
			{
				// finally check that the angle between the target and the candidate point
				// and your point is less than a given amount ( stops you wandering too far
				// away from your current point.

				GVector t_to_c = (candidate_pos - target_pos) ;
				angle_target_to_candidate = GATAN2(t_to_c.X, t_to_c.Y) ;
				GINT angle_diff = AngleDifference(angle_target_to_you , angle_target_to_candidate) ;

				if (GABS(angle_diff) < MAX_HOLDING_ANGLE_CHANGE  )
				{
					found_point = TRUE ;
					mTerrainGuide.SetNewGoalPoint(candidate_pos) ;
					mTargetPosWhenSettingHoldingPoint = target_pos ; 
				}
			}
			
		}
		attempt++;
	}	
}


