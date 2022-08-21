// KingHillBadGuy.cpp: implementation of the CKingHillBadGuy class.
//
//////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Globals.h"
#include "KingHillBadGuy.h"
#include "BadGuys.h"
#include "WayPoint.h"
#include "KingHillGoalPoint.h"
#include "Checkpoint.h"

//******************************************************************************************
void	 CKingHillBadGuy::Init(CInitThing* i)
{

	SUPERTYPE::Init(i) ;

	// we don't call resetcurrentaction cause of the
	// annoying bug where u can't use points in the init function
	mDefaultObjective = GO_FOR_THE_KING_OF_THE_HILL ;
	ResetCurrentAction() ;
	mKingHillStatus = TRING_TO_GET_TO_BOTTOM_OF_HILL;
	mDefaultObjective = GO_FOR_THE_KING_OF_THE_HILL ;
	mCurrentAction = GO_FOR_THE_KING_OF_THE_HILL;
	mNextPathFinderPoint = NULL ;
	mStateTimerKH = 0;
	mGoingToFirstPathFinderPoint = TRUE ;
	mNextCheckPoint = NULL ;
	mCurrentCheckPoint = NULL ;
	GetLevelData().SetLastCheckPoint(-1) ;
}



//******************************************************************************************
void	CKingHillBadGuy::ResetCurrentAction()
{
	mDefaultObjective = GO_FOR_THE_KING_OF_THE_HILL ;
	SUPERTYPE::ResetCurrentAction();
	mKingHillStatus = TRING_TO_GET_TO_BOTTOM_OF_HILL;
	mStateTimerKH = 0;
	mDefaultObjective = GO_FOR_THE_KING_OF_THE_HILL ;
	mCurrentAction = GO_FOR_THE_KING_OF_THE_HILL;
	mNextPathFinderPoint = NULL ;
	mGoingToFirstPathFinderPoint = TRUE ;
	mNextCheckPoint = NULL ;
	mCurrentCheckPoint = NULL ;
	GetLevelData().SetLastCheckPoint(-1) ;
}

//********************************************************************************************************
void	CKingHillBadGuy::StateFinished()
{
	SUPERTYPE::StateFinished() ;

	// ok head for the centre of the hill
	CCharacterLevelData& cld = GetLevelData() ;
	CKingHillGoalPoint* khgp = cld.GetWhichKingHillArea() ;

	if (khgp && khgp->IsPointInsideKingHillArea(mPos))
	{
		mTerrainGuide.SetNewGoalPoint(khgp->GetCentrePoint()) ;
		mTerrainGuide.MoveMe(mAccelSpeed, FALSE, TRUE) ;
		return ;
	}

	// ok we have fallen off the hill make sure we don't roll into pit or something
	mTerrainGuide.ChewTerrainCheck(mAccelSpeed, mBool2) ;
	
}


//******************************************************************************************
void	CKingHillBadGuy::StateNormal()
{
	
	switch (mCurrentAction)
	{
		case GO_FOR_THE_KING_OF_THE_HILL:
		{
			if (mKingHillStatus == REACHED_PLATO)
			{
				CONSOLE.AddString("warning !!! arse !!!!") ;
			}

			CurrentActionGoingToKingHillPoint() ;
			return ;
		}
	}

	SUPERTYPE::StateNormal() ;

	if (mCurrentAction == GO_FOR_A_WEAPON ||
		mCurrentAction == GO_TO_HEALTH_TILE) return ;

	// must be attacking kinda modes if we get here

	// ok if we are attacking and if the guy we're attacking is not a serious threat
	// the legg it up the hill

	if (mCurrentAction == ATTACK_TARGET && mCurrentTarget && mKingHillStatus != REACHED_PLATO)
	{
		GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

		if (mCurrentTarget->GetState() == CS_KNOCKED_OUT ||
			mCurrentTarget->GetState() == CS_STUNNED ||
			dist_to_target > CLOSE_ENOUGH_TO_HIT)
		{
			if (CheckIfWeCanStartClimbingHillNow() )
			{
				ChangeCurrentAction(GO_FOR_THE_KING_OF_THE_HILL) ;
				mDefaultObjective = GO_FOR_THE_KING_OF_THE_HILL ;
			}
			else
			{
				// arse back to square 1
				ResetCurrentAction() ;
			}
			return ;
		}
	}

	// ok we stop actually attacking so go back on getting up the hill
	if (mCurrentAction != ATTACK_TARGET && mKingHillStatus != REACHED_PLATO)
	{
		if (CheckIfWeCanStartClimbingHillNow() )
		{
			ChangeCurrentAction(GO_FOR_THE_KING_OF_THE_HILL) ;
			mDefaultObjective = GO_FOR_THE_KING_OF_THE_HILL ;
		}
		else
		{
			// arse back to square 1
			ResetCurrentAction() ;
		}
		return ;
	}

	if (mKingHillStatus == REACHED_PLATO)
	{
		// must be protecting hill so check if we're not scoreing points
		UpdateProtectingHill() ;
	}


}



//******************************************************************************************
void	CKingHillBadGuy::CurrentActionGoingToKingHillPoint()
{

	if (mIsBlocking == TRUE) StopBlocking() ;


	if (mKingHillStatus == CLIMBING_HILL)
	{
		MoveMeUpTheHill() ;
		return ;
	}
	else
	{
		// if we're scoreing points anyway then change to attacking mode ;
		if( GetLevelData().AreWeInArea() )
		{
			StartProtectingHill() ;
			return ;
		}
	}

	MoveMeToTheBottomOfTheHill() ;
}


//********************************************************************************************************
void	CKingHillBadGuy::MoveMeToTheBottomOfTheHill()
{
	if (mLife < (InitialLife() - (InitialLife() >>2)  ) )
	{
		GoToHealthTile(mScanDistForAWeapon) ;
	}
	// could be worst while going for a weapon instead
	PickupAnyWeapon(mScanDistForAWeapon ) ;



	// incase we get stuck
	if (mStateTimerKH > 120 )
	{
		mNextPathFinderPoint= NULL ;
	}

	if (mKingHillStatus == TRING_TO_GET_TO_BOTTOM_OF_HILL && mNextPathFinderPoint == NULL)
	{
		FindClosestPathFinderPoint() ;
		mStateTimerKH = 0 ;
	}

	mStateTimerKH++;

	if (mNextPathFinderPoint && mFlags & TF_ON_GROUND && IsUpright())
	{
		// switch off terrain height check if we're going to
		// our first PathFinderPoint, basicaly who knows what kinda mess we are in (like stuck
		// on top off a peek or something)
		BOOL terrain_check = mBool2 ;
		if (mGoingToFirstPathFinderPoint) terrain_check = FALSE ;
		mTerrainGuide.MoveMe(mAccelSpeed, FALSE, terrain_check) ;

		// are we close to PathFinderPoint.
		if ((mPos - mNextPathFinderPoint->GetPos()).MagnitudeSq() < G(9) )
		{
			mStateTimerKH = 0 ;
			mGoingToFirstPathFinderPoint = FALSE ;
			mNextPathFinderPoint = (CPathFinderPoint*) mNextPathFinderPoint->GetChainTrigger() ;
			if (mNextPathFinderPoint != NULL)
			{
				mTerrainGuide.SetNewGoalPoint(mNextPathFinderPoint->GetPos()) ;
			}
			else
			{
				// we should be now at the bottom of the hill
				mKingHillStatus = CLIMBING_HILL ;
				CalcNewCheckPoint(0) ;
			}

		}
	}
}



//********************************************************************************************************
void	CKingHillBadGuy::MoveMeUpTheHill()
{
	// ok if someone is already on the hill and we have just entered the scoring zone then don't
	// bother going for the last checkpoint instead start fighting now
	if (GAME.GetGameLevelData().NumCharactersOnHill() > 1 && GetLevelData().AreWeInArea())
	{
		StartProtectingHill() ;
		return ;
	}

	if (CheckIfFallenOffHill() )
	{
		// ok we're no longer on the checkpoint path but if we've reached the plato at the top ( i.e.
		// we are scoring points then just start protecting the hill )
		if( GetLevelData().AreWeInArea() )
		{
			StartProtectingHill() ;
		}
		else
		{
			ResetCurrentAction() ;
		}
		return ;
	}

	mStateTimerKH++;

	// incase we get stuck
	if (mStateTimerKH > 120 )
	{
		ResetCurrentAction() ;
		return ;
	}

	GVector u_to_checkpoint = (  mNextMidCheckPointPos - GetPos());  // scale to prevent possible overflows.
	GINT mag = u_to_checkpoint.Magnitude() ;
	GVector velocity = GetVelocity() ;
	velocity.Normalise() ;
	GINT velocity_a =   GATAN2(velocity.X, velocity.Y);
	GINT angle_you_to_checkpoint = GATAN2(u_to_checkpoint.X, u_to_checkpoint.Y);
	GINT angle_diff = GABS(AngleDifference(velocity_a, angle_you_to_checkpoint ) ) ;
	GINT angle_diff1 = GABS(AngleDifference(angle_you_to_checkpoint, velocity_a ) ) ;

//	angle_diff ;

	GINT new_angle1;
	GINT new_angle2 ;

	if (angle_diff > G_PI_2)
	{
	    new_angle1= velocity_a+G_PI ;
		if (new_angle1 < G_PI) new_angle1-=(G_2PI) ;
		new_angle2 = velocity_a ;
	}
	else
	{
		new_angle1 = angle_you_to_checkpoint-angle_diff ;
		if (new_angle1 < -G_PI) new_angle1+=(G_2PI) ;

		new_angle2 = angle_you_to_checkpoint+angle_diff ;
		if (new_angle2 > G_PI) new_angle2-=(G_2PI) ;
	}

	GINT angle_diff_new_angle1= GABS(AngleDifference(velocity_a, new_angle1 ) ) ;
	GINT angle_diff_new_angle2= GABS(AngleDifference(velocity_a, new_angle2 ) ) ;


	GVector to_thrust_dir ;

	GINT vel = GetVelocity().Magnitude() ;

	if (vel < G(0,4000))
	{
		to_thrust_dir = u_to_checkpoint ;
	}
	else
	{
		if (angle_diff_new_angle1 > angle_diff_new_angle2)
		{
			to_thrust_dir = GVector(GSIN(new_angle1), GCOS(new_angle1), G0) ;
		}
		else
		{
			to_thrust_dir = GVector(GSIN(new_angle2), GCOS(new_angle2), G0) ;
		}
	}

	to_thrust_dir.Normalise() ;

	mTerrainGuide.SetNewGoalPoint(mPos+(to_thrust_dir*4)) ;

	GINT boost = G(1,16444) ;


	if (mag < G(6) && vel > G(0, 18107)  )
	{
		boost = G(1) - ( mag * G(0, 15384)) ;
	}

	// accelerate if we are facing the right way, otherwise slow down.
	GVector power;
	if (mFlags & TF_ON_GROUND) Thrust(to_thrust_dir * mAccelSpeed * boost ) ;
	FaceCheckPointPos() ;

}


//******************************************************************************************
void	CKingHillBadGuy::FindClosestPathFinderPoint()
{
	CPathFinderPoint* closest_path_finder_point = NULL ;
	GINT	closest_distanceSQ = G(120*120) ;

	PathFinderPointsInWorldList& items = WORLD.GetPathFinderPointsNB() ;
	for (CPathFinderPoint* current = items.First() ;
					current != NULL ;
					current = items.Next() )
	{
		GINT dist = (mPos - current->GetPos()).MagnitudeSq() ;
		if ( dist < closest_distanceSQ)
		{
			closest_distanceSQ = dist ;
			closest_path_finder_point = current ;
		}
	}

	if (closest_path_finder_point)
	{
		// ok, is this PathFinderPoints trigger PathFinderPoint closer to us than the distance between this
		// PathFinderPoint and its trigger.  If it is, go there instead and save doubling back on our self

		CPathFinderPoint* next_path_finder_point = (CPathFinderPoint*)closest_path_finder_point->GetChainTrigger() ;
		if (next_path_finder_point)
		{
			// calc dist to next PathFinderPoint
			GINT us_to_nextwp = (mPos - next_path_finder_point->GetPos()).MagnitudeSq() ;
			GINT closestwp_to_nextwp = (closest_path_finder_point->GetPos() - next_path_finder_point->GetPos()).MagnitudeSq() ;
			if (us_to_nextwp < closestwp_to_nextwp)
			{
				closest_path_finder_point = next_path_finder_point ;
			}
		}
		mNextPathFinderPoint = closest_path_finder_point;
		mTerrainGuide.SetNewGoalPoint(mNextPathFinderPoint->GetPos()) ;
		mGoingToFirstPathFinderPoint = TRUE ;
	}
}


//******************************************************************************************
void	CKingHillBadGuy::StartProtectingHill()
{
	ChangeCurrentAction(WAIT_FOR_A_TARGET) ;
	mDefaultObjective = WAIT_FOR_A_TARGET;
	mKingHillStatus = REACHED_PLATO ;
	mCurrentTarget = NULL ;
}



//******************************************************************************************
BOOL	CKingHillBadGuy::CheckIfWeCanStartClimbingHillNow()
{
	CCheckPoint* checkpoint = GetLevelData().IsInAPathLeadingUpTheHill() ;

	if (checkpoint)
	{
		mKingHillStatus = CLIMBING_HILL ;

		// ok pretend that we were trying to head toward the checkpoint we
		// are in now and call the ReachedCheckpoint function, this should
		// then have us on the way moving up the hill to the next checkpoint.

		mNextCheckPoint = checkpoint;
		GetLevelData().SetLastCheckPoint(checkpoint->GetCheckPointNumber() -1 ) ;
		ReachedCheckPoint(checkpoint->GetCheckPointNumber()) ;
		return TRUE ;
	}
	return FALSE ;
}



//******************************************************************************************
void	CKingHillBadGuy::UpdateProtectingHill()
{
	CCharacterLevelData& cld = GetLevelData() ;

	// are we still scoring
	if (!cld.AreWeInArea())
	{
		ResetCurrentAction() ;
		CheckIfWeCanStartClimbingHillNow() ;
		return ;
	}


	CKingHillGoalPoint* khgp = cld.GetWhichKingHillArea() ;

	// if know one around head toward middle of king hill point
	if (mCurrentTarget == NULL)
	{
		mTerrainGuide.SetNewGoalPoint(khgp->GetCentrePoint()) ;
		mTerrainGuide.MoveMe(mAccelSpeed, FALSE, TRUE) ;
	}

	// stop us setting any goal points set by are parent fighting stuff outside
	// of the king hill area.  Bit crapy this, but see how it goes..

	GVector goal_point = mTerrainGuide.GoalPoint() ;

	if (khgp && !khgp->IsPointInsideKingHillArea(goal_point) )
	{
		ResetCurrentAction() ;
    	StartProtectingHill();
	}
}


//********************************************************************************************************
void	CKingHillBadGuy::CalcNewCheckPoint(SINT for_check_point)
{
	CCharacterLevelData* cld = &GetLevelData();
	mCurrentCheckPoint = mNextCheckPoint ;
	mNextCheckPoint = cld->GetShortestDistToCheckPointsSQ(for_check_point).GetCheckPoint() ;

	// calc point which is half way between this checkpoint and the checkpoints target checkpoint

	GVector p1 = mNextCheckPoint->GetPos() ;
	GVector p2 = mNextCheckPoint->GetTarget()->GetPos() ;


	mNextMidCheckPointPos.X = ((p2.X - p1.X) * G(0,32768) ) + p1.X ;
	mNextMidCheckPointPos.Y = ((p2.Y - p1.Y) * G(0,32768) ) + p1.Y ;
	mNextMidCheckPointPos.Z = G(0) ;

	mNextMidCheckPointPos.Z = MAP.GetMapHeight(mNextMidCheckPointPos) ;
}


//********************************************************************************************************
void	CKingHillBadGuy::ReachedCheckPoint(SINT check_point_num)
{
	CCharacterLevelData* cld = &GetLevelData();
	if (mKingHillStatus != CLIMBING_HILL) return  ;

	// if we haven't arived at new checkpoint then do nothing
	if (cld->GetLastCheckPoint() == check_point_num) return ;


	mStateTimerKH = 0;
	// ok update are level data stuff
	SUPERTYPE::ReachedCheckPoint(check_point_num) ;

	// calc which checkpoint number we want to goto next

	SINT next_check_point ;
	if (cld->GetLastCheckPoint() == GAME.GetIVar(IV_NUM_CHECKPOINTS))
	{
		// no more checkpoints we must be at the top
		StartProtectingHill() ;
		return ;
	}
	else
	{
		next_check_point = cld->GetLastCheckPoint() + 1 ;
	}

	CalcNewCheckPoint(next_check_point) ;
}


//********************************************************************************************************
BOOL	CKingHillBadGuy::CheckIfFallenOffHill()
{
	if (!mCurrentCheckPoint ||
		!mCurrentCheckPoint->GetChainTrigger() ||
		!mCurrentCheckPoint->GetTarget() ||
		!mCurrentCheckPoint->GetTarget()->GetChainTrigger() )
	{
		return FALSE ;
	}

	GVector cp1_to_u = mPos - mCurrentCheckPoint->GetPos() ;
	cp1_to_u.Z = G(0) ;
	GVector cp1_to_cpt1 = mCurrentCheckPoint->GetChainTrigger()->GetPos() - mCurrentCheckPoint->GetPos() ;
	GVector cp2_to_u = mPos - mCurrentCheckPoint->GetTarget()->GetPos() ;
	cp2_to_u.Z = G(0) ;
	GVector cp2_to_cpt2 = mCurrentCheckPoint->GetTarget()->GetChainTrigger()->GetPos() - mCurrentCheckPoint->GetTarget()->GetPos() ;


	// calculate perpendicular to vector cp1 to cpt1
	GVector perp_cp1_cpt1 = GVector(-cp1_to_cpt1.Y, cp1_to_cpt1.X,G(0) ) ;
	GVector perp_cp2_cpt2 = GVector(cp2_to_cpt2.Y, -cp2_to_cpt2.X,G(0) ) ;

	GINT dot1 = perp_cp1_cpt1* cp1_to_u  ;
	GINT dot2 =	perp_cp2_cpt2* cp2_to_u  ;

	if (dot1 < G(0) || dot2 < G(0)) return TRUE ;

	return FALSE ;
}





//******************************************************************************************
void	CKingHillBadGuy::FaceCheckPointPos()
{
	if (!IsUpright()) return ;

	GVector u_to_t =  (mNextMidCheckPointPos - mPos ) ;
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
void	CKingHillBadGuy::Hit(CThing *that)
{
	SUPERTYPE::Hit(that) ;
	CCharacter* character = that->IsCharacter() ;

	if (!character ||
		mDefaultObjective != GO_FOR_THE_KING_OF_THE_HILL ||
		mState != CS_NORMAL ||
		character->GetState() == CS_KNOCKED_OUT ||
		character->GetState() == CS_STUNNED ||
		(!(character->GetAttackID() & mToAttackBF))) return;

	// do we currently have a target ?
	if (mCurrentTarget !=NULL)
	{
		// if the guy who hit us is already our target then make sure we attack it
		if (mCurrentTarget == character)
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
				if ((RANDOM()%2)==1) return ;

				// stop attacking out current target
				StopAttackingTarget() ;
			}

			// start attacking the guy who struck us
			mCurrentTarget = character;
			StartAttackingTarget() ;
		}
	}

	// we have no target
	else
	{
		// start attacking the guy who struck us
		mCurrentTarget = character ;
		StartAttackingTarget() ;
	}
}

//******************************************************************************************
void	CKingHillBadGuy::ChooseTarget()
{
	if (mKingHillStatus != REACHED_PLATO)
	{
		SUPERTYPE::ChooseTarget() ;
		return ;
	}

	BOOL no_primarys = FALSE ;
	if (GAME.GetGameState() >= GAME_STATE_GAME_FINISHED) 
	{
		no_primarys = TRUE ;
	}
	// get a new target ( i.e. get the guy who has the most points if we're on
	// the plato !!!

	SINT highest_score = -2 ;

	CListArray<CCharacter, MAX_CHARACTERS_IN_WORLD>& items = WORLD.GetCharacterNB();

	CCharacter	*current = NULL;
	GINT		mind = mCharacterSight*mCharacterSight;

	CCharacter* nt;
	for (nt = items.First();
		 nt != NULL ;
		 nt = items.Next() )
	{
		if (nt && nt->GetAttackID() & mToAttackBF && nt != this  &&
			( no_primarys == FALSE || nt->IsPrimary() == FALSE) )
		{
			CCharacterLevelData* tcld = &nt->GetLevelData() ;
			GINT height_diff = GABS( nt->GetPos().Z - mPos.Z ) ;

			if (nt->GetState() != CS_DEAD &&
				height_diff <= G(2) &&
				nt->GetScore() > highest_score)
			{
				highest_score = nt->GetScore() ;
				current = nt ;
			}
		}

	}

	mCurrentTarget = current;

	if (mCurrentTarget != NULL)
	{	
		// fuck it !! if you are too far away from the winner do the normal choose target shit
		if ((mCurrentTarget->GetPos()-mPos).MagnitudeSq() > G(36))
		{
			SUPERTYPE::ChooseTarget() ;
		}
		else
		{
			SetHoldingPoint(mMinHoldingPointDist,mMaxHoldingPointDist) ;
			ChangeCurrentAction(GO_TO_HOLDING_POINT) ;
		}
	}
}



#if TARGET == PC

//******************************************************************************************
void	CKingHillBadGuy::DrawDebugText(HDC hdc, SINT* y_offset, char* buffer)
{
	SUPERTYPE::DrawDebugText(hdc, y_offset, buffer) ;
	SINT size ;
	size = sprintf(buffer, "-----------------KING HILL BAD GUY -------------------");
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;
	switch (mKingHillStatus)
	{
	case TRING_TO_GET_TO_BOTTOM_OF_HILL:	size = sprintf(buffer, "Current king hill status = TRYING TO GET TO BOTTOM OFF HILL"); break;
	case CLIMBING_HILL:size = sprintf(buffer, "Current king hill status = CLIMBING HILL"); break;
	case REACHED_PLATO:size = sprintf(buffer, "Current king hill status = ON PLATO"); break;
	default: size = sprintf(buffer, "Current king hill status = %d", mKingHillStatus); break;
	}

	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;


    if (mGoingToFirstPathFinderPoint)
	{
		size = sprintf(buffer, "Heading To First PathFinderPoint");
	}
	else
	{
		size = sprintf(buffer, "Follwing PathFinderPoints or checkpoints");
	}
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;

	if (mCurrentCheckPoint)
	{
		size = sprintf(buffer, "Current checkPoint number = %d", mCurrentCheckPoint->GetCheckPointNumber());
		TextOut(hdc, 0, *y_offset, buffer, size);
		*y_offset+=12 ;
	}
	if (mNextCheckPoint)
	{
		size = sprintf(buffer, "Next checkPoint number = %d", mNextCheckPoint->GetCheckPointNumber());
		TextOut(hdc, 0, *y_offset, buffer, size);
		*y_offset+=12 ;
	}

	size = sprintf(buffer, "king hill state timer = %d", mStateTimerKH);
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;


	size = sprintf(buffer, "score = %d", GetScore());
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;

	if (mNextPathFinderPoint)
	{
		ENGINE.RenderArrow(GVectorToFVector(GetRenderPos()), GVectorToFVector(mNextPathFinderPoint->GetRenderPos()), 0xff4080ff);
	}
	else if (mNextCheckPoint)
	{
		ENGINE.RenderArrow(GVectorToFVector(GetRenderPos()), GVectorToFVector(mNextMidCheckPointPos), 0xff4080ff);
	}



} ;


#endif
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

