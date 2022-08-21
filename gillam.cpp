// Gillam.cpp: implementation of the CGillam class.
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "globals.h"
#include "Gillam.h"
#include "HoldingZonePoint.h"

#define GIVE_UP_TRYING_TO_ATTACK_COUNT 80

#define GILLAM_CHARGE_SPEED G(0,30000)

#define RE_CALC_HOLDING_POINT_DIST G(13)

#define HOLDING_POINT_MIN_DIST 9
#define HOLDING_POINT_MAX_DIST 10

#define MAX_HOLDING_ANGLE_CHANGE G_PI_4
#define PUNCH_ZENNITH_PAUSE_TIME 60

CCharacterCombatProfile GillamCombatProfile = { 130,0,0,30,0,0,15,0,0 } ;

//******************************************************************************************
void	CGillam::Init(CInitThing* i)
{
	mCombatProfile = GillamCombatProfile ;
	SUPERTYPE::Init(i) ;
	mPunchTime = 31 - (mCounter0 << 1)  ;
	mHoldingPointDelay = 40 - ((10 - mCounter0)*5);
	mMinHoldingPointDist = 13 ;
	mMaxHoldingPointDist = 14 ;
	mBadGuyProximityRange = G(30) ;
	mSpinStunRecoveryTime = 15 ;
	mSpinVelocityWhenStruck = G(0,42343) ;
	mAccelSpeed *= G(0,55000) ;
	mHowMuchILeanForwardWhenRunning = G(10) ;
	mChargesSinceLastHit = 0 ;
	mPlayVictoryAtEndOfAttack = FALSE ;
	mTimeAllowedToSlamTheGround = -2 ;
}

#define CLOSE_ENOUGH_TO_HIT_GILLAM G(7)

//******************************************************************************************
void	CGillam::CurrentActionAttackTarget()
{
//	SUPERTYPE::CurrentActionAttackTarget() ;
//	return ;

	if (RANDOM()%2 == 1) mStateTimer ++;

	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

//	SetAttackingPoint() ;


	mTerrainGuide.SetNewGoalPoint( mCurrentTarget->GetPos());

	if (mFlags & TF_ON_GROUND && dist_to_target > CLOSE_ENOUGH_TO_HIT)
	{
		if (mAnimMode != AM_CHARGE)
		{
			mTerrainGuide.MoveMe(mAccelSpeed*G(2,23232), FALSE, mBool2) ;
		}
	}

	if (mAnimMode != AM_CHARGE)
	{
		FaceTarget() ;
	}

	if (mCurrentPunchSequenceNum != 0 && !IsStriking() )
	{
		mCurrentPunchSequenceNum = 0 ;
		StopAttackingTarget() ;
		ResetCurrentAction() ;
		return ;
	}


	SINT next_time_allowed_to_attack = mPunchTime ;
	if (mCurrentTarget->GetState() == CS_KNOCKED_OUT) next_time_allowed_to_attack += (mPunchTime<<1);
	GINT	v = mVelocity.MagnitudeXY();

	if ((v>GILLAM_CHARGE_SPEED && dist_to_target < CLOSE_ENOUGH_TO_HIT_GILLAM ) ||
		dist_to_target < (CLOSE_ENOUGH_TO_HIT*G(1,32123)) ) //&& mStateTimer > next_time_allowed_to_attack )
	{
		if (mStateTimer > next_time_allowed_to_attack)
		{
			// is target knocked out then only stike if we are skill level 8 or above
			if ( mCounter0 >= 8 && (!IsStriking()))
			{
				// if attack was allowed
				if (DoAnAttack())
				{
					mStateTimer = 0;
					mCurrentPunchSequenceNum++;
				}
			}
			else
			{
				mStateTimer = 0 ;
			}
		}
	}

	if (mStateTimer > GIVE_UP_TRYING_TO_ATTACK_COUNT)
	{
		StopAttackingTarget() ;
		ResetCurrentAction() ;
	}
}


//******************************************************************************************
void	CGillam::CurrentActionGoToHoldingPoint()
{

//	SUPERTYPE::CurrentActionGoToHoldingPoint() ;
//	return ;

	// if target moved along away from the position when we calculated the holding point then reset
	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;


	if ((mCurrentTarget->GetPos() - mTerrainGuide.GoalPoint()).MagnitudeSq() < G(49) )
	{
	//	SINT temp = mStateTimer ;
	//	ResetCurrentAction();
	//	mStateTimer = temp ;

		if (!IsStriking() && GAME.GetGameTurn() > mTimeAllowedToSlamTheGround )
		{
			FaceTarget() ;
			SetAnimMode(AM_PUNCH) ;
		//	CONSOLE.AddString("punch 111111111111111111111111111") ;
			GVector sp = mCurrentTarget->GetPos() - mPos ;
			sp/=2 ;
			sp+=mPos ;
			mChargesSinceLastHit= 0 ;
			mTimeAllowedToSlamTheGround = GAME.GetGameTurn() + 5*25 ;

		}
	//	return ;
	}

//	if ((mTargetPosWhenSettingHoldingPoint - mCurrentTarget->GetPos()).MagnitudeSq() > RE_CALC_HOLDING_POINT_DIST * RE_CALC_HOLDING_POINT_DIST )
//	{
//		SINT temp = mStateTimer ;
//		ResetCurrentAction();
//		mStateTimer = temp ;
//			FaceTarget() ;
		//	CONSOLE.AddString("prem") ;
//		return ;
//	}

	if (RANDOM()%2 == 1) mStateTimer ++;

	GINT accell_speed = mAccelSpeed ;


	if (mFlags & TF_ON_GROUND && IsUpright())
	{
		mTerrainGuide.MoveMe(accell_speed, FALSE, mBool2) ;
	}


	GINT dist_to_goal_point = (mTerrainGuide.GoalPoint() - mPos).MagnitudeSq() ;
	if (dist_to_goal_point > G(4) )
	{
	//	CONSOLE.AddString("face gt point") ;
		FaceGoToPoint() ;
	}
	else
	{
	//	CONSOLE.AddString("face target") ;
		FaceTarget() ;
	}

	BOOL allowed_to_attack = TRUE;
	if (UsesNonProximitryHoldingZones() && !mHoldingZonePoint->IsPointInsideZone(mCurrentTarget->GetPos()))
	{
	//	allowed_to_attack = FALSE ;
	}

//	char temp[50] ;
//	sprintf(temp,"st = %d   hpd = %d", mStateTimer, mHoldingPointDelay) ;
//	CONSOLE.AddString(temp) ;

	if (mStateTimer > mHoldingPointDelay )
	{
		mStateTimer = 0;
		SINT pcwhole = 80 ;

		if (pcwhole > (RANDOM())%100 &&
			allowed_to_attack)
		{
			if (RANDOM()%3==0 && dist_to_goal_point < G(4) && mChargesSinceLastHit !=0 )
			{
				SetAnimMode(AM_PUNCH) ;
			//	CONSOLE.AddString("punch 2222222222222222222222!!!");
				mChargesSinceLastHit= 0 ;
				mTimeAllowedToSlamTheGround = GAME.GetGameTurn() + 5*25 ;
			}
			else
			{
				StartAttackingTarget() ;
			}
		}
		else
		{
			ResetCurrentAction() ;
			return ;
		}
	}
}


//******************************************************************************************
void	CGillam::StateKnockedOut()
{

	if (GAME.GetGameTurn() %2 == 0)
	{
		ReduceKnockoutTime(2) ;
	}
	else
	{
		ReduceKnockoutTime(1) ;
	}
	SUPERTYPE::StateKnockedOut() ;

}

//******************************************************************************************

void	CGillam::Defense()
{
	/*
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
	}*/
}


//******************************************************************************************
EAnimMode	CGillam::GetNextAnimCharacter(EAnimMode am)
{
	switch (am)
	{

	case AM_PUNCH:
		return AM_PUNCH_HIT;

	case AM_VICTORY:
	case AM_CHARGE:
	case AM_FLINCH:
	case AM_PUNCH_HIT:
		return AM_WALK;

	case AM_DYING:		return AM_INVALID; //  stop

	default:			return AM_NONE;
	};
}


//******************************************************************************************
BOOL	CGillam::Attack(SINT no)
{
	GINT	v = mVelocity.MagnitudeXY();
	if (v>GILLAM_CHARGE_SPEED )
	{
		SetAnimMode(AM_CHARGE) ;
		// gives you a boost
		GVector v1=mVelocity ;
		v1.Normalise() ;
		AddVelocity(v1*G(0,23000)) ;
		mChargesSinceLastHit++ ;
	}
	else
	{
		SetAnimMode(AM_PUNCH) ;
		mTimeAllowedToSlamTheGround = GAME.GetGameTurn() + 5*25 ;
	//	CONSOLE.AddString("punch 33333333333333333333333333") ;
	}

	return TRUE ;
}

//******************************************************************************************
GINT	CGillam::GroundFriction(EFrictionType friction_type)
{
	if (mAnimMode == AM_PUNCH || mAnimMode == AM_PUNCH_HIT)
	{
		return  G(0,55000) ;
	}
	GINT friction = SUPERTYPE::GroundFriction(friction_type);
	if (mAnimMode == AM_CHARGE) friction*=G(0,62000) ;
	return friction ;
}

//******************************************************************************************
BOOL	CGillam::Strike(const CStrike &strike)
{
	if (IsStriking() && mAnimPauseCount ==0) return FALSE ;
	// ok we have to modify stike cos the gillam is 'ard...
	CStrike new_strike = strike ;
	new_strike.mAttackVelocity/=2 ;


	SINT temp_spin_rc = mSpinStunRecoveryTime;
	SINT temp_default_rc = mDefaultStunRecoveryTime ;

	if (mAnimPauseCount!=0 || (mAnimMode == AM_PUNCH_HIT && mFrame > GINT_HALF) )
	{
		mSpinStunRecoveryTime *=2 ;
		mDefaultStunRecoveryTime *=5 ;
	}
	else
	{
		new_strike.mWillKnockOut = FALSE ;
	}

	BOOL result = SUPERTYPE::Strike(new_strike) ;

	mSpinStunRecoveryTime = temp_spin_rc;
	mDefaultStunRecoveryTime =temp_default_rc;
	return result ;
}


//******************************************************************************************
void	CGillam::Damage(GINT amount)
{
//	amount/=2 ;
	SUPERTYPE::Damage(amount) ;
}


#define S_TO_G G(4,32000)

//******************************************************************************************
void	CGillam::AnimTrigger(const GMatrix &current_anim_orientation, BOOL end_of_punch_hit)
{

	// THIS FUNCTION IS A MESS  !!!!!!

	// hits will fail if one of these
	if (mState == CS_KNOCKED_OUT || mState == CS_DEAD) return ;

	switch (mAnimMode)
	{


	case AM_PUNCH_HIT:
	case AM_CHARGE:
		{
			GVector hv = mOrientation2 * GVector(G0, G1, G0);
			GVector our_movement_after_strike = GVector(G0,G0,G0);
			GINT our_term_velocity = SMALL_COMBAT_MAX_VELOCITY ;

			hv.Z = G0;
			if (hv.MagnitudeSqXY() > G(0, 10))
			{
				hv /= hv.Magnitude();

				GVector hp;
				CThing *t ;
				if (mAnimMode == AM_CHARGE)
				{
					hp = mPos+ (mOrientation2 * GVector(G(0,22937), G(3,58982), -G(2,39321) ));
					t = FindFirstCollision(hp, G(2));
					//ENGINE.RenderArrow(GVectorToFVector(mPos), GVectorToFVector(hp), 0xff4080ff);
				}
				else
				{
					hv *= G(2, 0);
					hp = hv + mPos + GVector(G0, G0, -G(1)); // height
					t = FindFirstCollision(hp, G(4));
				}

				if (mAnimMode == AM_PUNCH_HIT)
				{
					// $$$ HIT GROUND HARD EFFECT HERE
				CCharacter* c=IsCharacter();
				if(c)
				{
					CParticleSystem *ps=c->GetParticleSystem();
					if(ps)
					{

						for(int i=0;i<4;i++)
						{
							CParticle *p=ps->CreateParticle(PART_SMOKE);
							if(p)
							{
								p->mPos=mPos;
								p->mRadius=128;

								int 	r = rand()&4095;
								SINT	rad = (rand() % 15) + 8;
								p->mVelocity.X.mVal=((rcos(r) * rad) >> (6 + 4))<<8;
								p->mVelocity.Y.mVal=((rsin(r) * rad) >> (6 + 4))<<8;
								p->mVelocity.Z.mVal=-0x20<<8;

							}
						}
					}


				}

					MAP.Raindrop(CCellRef(hv + mPos), G(0,28000));
				}

				mLastPunchResult = PR_MISSED ;
				mLastPunchFrameNum = GAME.GetGameTurn() ;

				while (t)
				{
					CStrike this_strike ;
					this_strike.mWillKnockOut = TRUE ;
					this_strike.mStrikedWith = SI_BIG_PUNCH;
					this_strike.mStrength = mCombatProfile.mBigPunchStrenth ;
					this_strike.mOriginator = this ;
					this_strike.mOverwriteSpin = TRUE ;
					this_strike.mShoulderToFistDist = S_TO_G ;
					//this_strike.mAttackVelocity = GVector(G0,G0,G0) ;
					GVector	dir = t->GetPos() - mPos;
					dir.Normalise();
						this_strike.mAttackVelocity = dir * G(0, 24000);
					CCharacter *c = t->IsCharacter();
					if (c && c->GetState()== CS_DEAD)
					{
						t = FindNextCollision();
						continue ;
					}

					if (mAnimMode == AM_CHARGE)
					{
						if (c && c->GetState() == CS_KNOCKED_OUT)
						{
							t = FindNextCollision();
							continue ;
						}

						if (t->IsStrikeable())
						{
							this_strike.mAttackVelocity = dir * G(0, 24000);
							this_strike.mWillKnockOut = TRUE ;
							this_strike.mStrength = mCombatProfile.mChargeAttackStrength ;
							this_strike.mOverwriteSpin = TRUE ;
							this_strike.mStrikedWith = SI_BIG_PUNCH;
							our_movement_after_strike = -(this_strike.mAttackVelocity) ;
							AddVelocity(-(this_strike.mAttackVelocity)*G(0,16124)) ;
						}
					}


					// if strike successful

					if (t->Strike(this_strike) && t->IsStrikeable() )
					{
						mLastPunchResult = PR_HIT ;
						ScoreStrike(t) ;
						hp.Z -= G1;
						mParticleSystem.MakeHitEffect(&hp, mYaw + G_PI,HT_NORMAL);
						if (this_strike.mStrikedWith != SI_BIG_PUNCH)
						{
							our_movement_after_strike = this_strike.mAttackVelocity ;
						}
						if (RANDOM()%4 == 0)
						{
							mPlayVictoryAtEndOfAttack = TRUE ;
						}
					}
					else if (c)
					{
						mLastPunchResult = PR_BLOCKED ;
						if (this_strike.mStrikedWith != SI_BIG_PUNCH)
						{
							our_movement_after_strike = this_strike.mAttackVelocity ;
						}
					}


					t = FindNextCollision();

				};
			}

			AddStrikeVelocity(our_movement_after_strike, our_term_velocity) ;

			if (mLastPunchResult == PR_MISSED)
			{
				if (mAnimMode != AM_CHARGE)
					SOUND.PlaySample(SOUND_PUNCHMIS, this);
			}
			else
			{
				if (mAnimMode == AM_PUNCH_HIT)
					SOUND.PlaySample(SOUND_PUNCH2, this);
				else
				{
					if ((mAnimMode == AM_CHARGE) && (mLastPunchResult != PR_HIT))
					{
					}
					else
						SOUND.PlaySample(SOUND_PUNCH1, this);
				}
			}
		}
		break;

	}
}



//******************************************************************************************
void	CGillam::CurrentActionAmbushing()
{
	mTerrainGuide.SetNewGoalPoint(mCurrentTarget->GetPos()) ;

	// Ignore terrain check when ambushing
	mTerrainGuide.MoveMe(mAccelSpeed, FALSE, FALSE) ;

	FaceTarget() ;
	GINT dist_to_target = (mCurrentTarget->GetPos() - mPos).Magnitude() ;

	if (dist_to_target < CLOSE_ENOUGH_TO_HIT_GILLAM )
	{
		CONSOLE.AddString("fuking close enough !!!!");
		StartAttackingTarget() ;
	}

}

//******************************************************************************************
void	CGillam::StateNormal()
{
	if (mAnimMode == AM_PUNCH || mAnimMode == AM_PUNCH_HIT)
	{

		if (mAnimPauseCount != 0 )
		{
			mDestYaw =mYaw ;
			// stop drawing trails
			mDFlags&= (~DFC_DrawTrail) ;

			// make gillam shake when hit ground
			SINT a_num = ((PUNCH_ZENNITH_PAUSE_TIME) - mAnimPauseCount) ;
			a_num /=2 ;
			GINT amount = G(0, (a_num*a_num*a_num))-G(0,1000) ;
			if (amount < G0) amount = G0 ;
			if (GAME.GetGameTurn() & 1 )
			{
				mVelocity = GVector(amount ,amount,amount) ;
			}
			else
			{
				mVelocity = GVector(-amount ,-amount,-amount) ;
			}
		}
	}
	else
	{
		// allow trails to be drawn
		mDFlags|=DFC_DrawTrail ;
		SUPERTYPE::StateNormal() ;
	}
}

//******************************************************************************************
void	CGillam::ProcessAnimation()
{
	GINT	old_frame = mFrame;

	mFrame+=mFrameInc;

	switch (mAnimMode)
	{

	case AM_CHARGE:
	{
		AnimTrigger(ID_GMATRIX, TRUE) ;
		// stick on charge anim if we are still going fast enough

		GINT	v = mVelocity.MagnitudeXY();
		if (v> (RUNNING_VELOCITY-G(0,2000)))
		{
			mFrame=G0 ;
		}

		break ;
	}



	default:
	{
		if ((old_frame < GINT_HALF) && (mFrame >= GINT_HALF))
		{
			// trigger actions??
			if (mAnimPauseCount == 0)
			{
//				printf("About to anim trigger\n");
				GMatrix fred = ID_GMATRIX;
				AnimTrigger(fred, TRUE);
//				printf("Done anim triggering\n");
			}

			if (mAnimPauseCount < PUNCH_ZENNITH_PAUSE_TIME && mAnimMode == AM_PUNCH_HIT)
			{
				mFrame-=mFrameInc ;
				mAnimPauseCount++ ;
				if (mAnimPauseCount == PUNCH_ZENNITH_PAUSE_TIME)
				{
					mFrameInc/=2;
				}
			}
		}
		else
		{
			mAnimPauseCount = 0 ;
		}
	}
	}

	if(mFrame>=G1)
	{
		if(IsAnimLooped(mAnimMode))
		{
			mFrame-=G1;

			// sort out walking, running etc.
			if ((mAnimMode == AM_STAND) || (mAnimMode == AM_WALK) || (mAnimMode == AM_RUN))
				SetAnimMode(mAnimMode, FALSE);
		}
		else
		{
			if (mPlayVictoryAtEndOfAttack == TRUE)
			{
				mFrame = G0;
				GINT fi = GetRenderThingFrameIncrement(AM_VICTORY, &mRealAnimIndex);
				if(fi != G0)
				{
					mFrameInc = fi;
					mAnimMode = AM_VICTORY;
				}
				mPlayVictoryAtEndOfAttack = FALSE ;
			}
			else
			{
				EAnimMode nam = GetNextAnimCharacter(mAnimMode);
				if (nam != AM_INVALID)
					SetAnimMode(GetNextAnimCharacter(mAnimMode));
				else
					mFrame = G1; // don't loop - pause at the end of the anim instead
			}

		}
	}
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CGillam::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();
	WORLD.AccumulateOIDResources(OID_CFXExplosion1);

}
#endif



#if TARGET == PC

//******************************************************************************************
void	CGillam::DrawDebugText(HDC hdc, SINT* y_offset, char* buffer)
{
	SUPERTYPE::DrawDebugText(hdc, y_offset, buffer) ;
	SINT size ;
	size = sprintf(buffer, "---------------------- GILLAM -------------------");
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

	size = sprintf(buffer, "state timer = %d", mStateTimer) ;
	TextOut(hdc, 0, *y_offset, buffer, size) ;
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

