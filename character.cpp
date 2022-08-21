#include	"Common.h"

#include	"Character.h"
#include	"Globals.h"
#include	"Weapon.h"
#include	"Profile.h"
#include    "Effect.h"
#include	"Crown.h"
#include    "Charball.h"
#include	"Droidball.h"

//******************************************************************************************
void	CCharacter::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	GET_INITIAL_MODE;
	if (mInitialMode == IM_INACTIVE)
		mState = CS_DEACTIVATED;
	else
		mState = CS_NORMAL;

	GET_TRIGGER_RESPONSE;
	if (mTriggerResponse == TR_UNUSED)
		mTriggerResponse = TR_ACTIVATE;
	GET_UNTRIGGER_RESPONSE;
	if (mUnTriggerResponse == TR_UNUSED)
		mUnTriggerResponse = TR_NONE;

	mEffectThing = NULL;
	mHoldingThing = NULL;

	if (i->mAngleType == AT_YPR)
		mYaw = i->mYaw;
	else
		mYaw = G0;

	// allowed to draw trail
	mParticleSystem.Activate();
	mParticleSystem.mOwner=this;
	
	mStateStackDepth = 0;
	mStateTimer = 0 ;
	mStateDest = GVector(G(32), G(32), G0);
	mStateStart = mStateDest;
	mStatePosStart = mStateDest;
	mStateTimerStart = 0;

	mIsBlocking = FALSE ;
	mCurrentPunchSequenceNum = 0;
	mLastPunchResult = PR_MISSED ;
	mLastPunchFrameNum = 0;
	mDefaultStunRecoveryTime = 6 ;
	mRecoverFromKnockoutTime = CHAR_KNOCK_OUT_TIME;
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER) 
		mRecoverFromKnockoutTime = mRecoverFromKnockoutTime >> 1 ;
	mLastFrameOnGround = GAME.GetGameTurn();
	mLastHeightOnGround = mPos.Z ;
	mLastFrameJumped = -1;
	mToAttackBF = 0 ;
	mScore=0;
	mCrown= NULL ;
	mDrawUsWhite = FALSE ;

	mRage = G0;

	mInvisible = FALSE;


	mMyAttackType = AT_NOTHING;
	mCurrentTarget = NULL ;

	mCharacterSight	 = G(25);

	mCanBlockBF = SI_SMALL_PUNCH | SI_CHARGE_PUNCH;

	ClipToGround();

// set default animation

	mAnimMode = AM_NONE; // JCL - wow!!!!! really obscure out of sync bug fixed here!!!!
	mRealAnimIndex = 0;
	SetAnimMode(AM_WALK);

	WORLD.GetCharacterNB().Add(this) ;

	mLevelData.Init(this) ;

	mInitialHoldingThingOID = OID_NONE ;

	if ((i->mOID != OID_INVALID) && (i->mOID != OID_NONE))
	{
		mInitialHoldingThingOID = i->mOID;
	}

	mLastFrameOnGroundPos = GVector(G(32), G(32), G0);

	mSpinStunRecoveryTime = 50;
	mAnimPauseCount = 0;

	mLastTrailUse = TU_NONE;
	mLastTrailUseTime = -32;

	mBoostTrailCount = 0;
	mOnFireFlames[0] = NULL ;
	mOnFireFlames[1] = NULL ;
	mFlameGoOutTime = -10 ;
	mLastHitSpikeTime = -10 ;

	mLastOrientation = mOrientation;
}

//******************************************************************************************
CCharacter::~CCharacter()
{

}


//******************************************************************************************
void	CCharacter::Shutdown()
{
	SUPERTYPE::Shutdown() ;
	WORLD.GetCharacterNB().Remove(this) ;
}


//******************************************************************************************
void	CCharacter::DeclareBoost()
{
	mBoostTrailCount = 40;
}

//******************************************************************************************
void	CCharacter::StartDieProcess(BOOL hit_fan)
{
	// kill water sound in case it's playing...
	SOUND.KillSample(this, SOUND_BUZZ);

	if (mCurrentTarget)
	{
		mCurrentTarget->NowNotBeenAttackedBy(this) ;
		mCurrentTarget= NULL ;
	}

	// ok we're dead but we are holding the crown
	if (mCrown)
	{
		GetLevelData().ReCalcCurrentCrownHolder() ;
	}

	if ((!GAME.IsMultiPlayer()) ||
			GAME.GetIVar(IV_LEVEL_TYPE) == LT_PURSUIT ||
			GAME.GetIVar(IV_LEVEL_TYPE) == LT_LASTMAN ||
			GAME.GetIVar(IV_LEVEL_TYPE) == LT_KING)
	{
		GAME.DeclareCharacterDead(this) ;
	}
	else
	{
		if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_DROID_BALL) mLevelData.DiedOnDroidBall() ;
		if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_TAG) mLevelData.DiedOnTag() ;
	}
}

//******************************************************************************************
SINT	CCharacter::WhichRTsToRender()
{
	if (mState == CS_TELEPORT)
	{
		if ((mStateTimer > 10) && (mStateTimer < 40))
			return 0;
	}

	SINT r = 1;

	if (mTrail.IsActive()) r += 2;
	if(mParticleSystem.mActive) r+= 4;
	return r;
}

//******************************************************************************************
BOOL	CCharacter::SetAnimMode(EAnimMode am, BOOL reset_frame)
{
	GINT	fi;
	
	mLastFrame = -G1; // reset interpolation.

	if ((am == AM_WALK) || (am == AM_STAND) || (am == AM_RUN))
	{
		GINT	v = mVelocity.MagnitudeXY();

//		if (v < G(0, 1000))
		if (v < G(0, 3500))
		{
//			if ((mAnimMode != AM_STAND) &&
//				(mAnimMode != AM_WALK2STAND) &&
//				(mAnimMode != AM_BORED) &&
//				(mAnimMode != AM_WAVE))
			if (mAnimMode == AM_WALK)
			{
				am = AM_WALK2STAND;
				reset_frame = TRUE;
			}
			else
			{
				if (((RANDOM()) % 200) == 11 && mAnimMode == AM_STAND && GAME.GetGameTurn() > 10*25)
					am = AM_BORED;
				else
					am = AM_STAND;
			}
		}
		else if (v < RUNNING_VELOCITY)
		{
//			if (mAnimMode != AM_WALK)
			am = AM_WALK;
		}
		else
		{
//			if (mAnimMode != AM_RUN)
			am = AM_RUN;
		}
	}

	if (am == AM_VICTORY)
		SetDestYaw(G_PI);  // face the camera!


	if (reset_frame)
		mFrame = G0;

	if (am == AM_RAGE_ATTACK)
	{
		fi = GetRenderThingFrameIncrement(AM_SPIN_ATTACK, &mRealAnimIndex);
	}
	else
	{
		fi = GetRenderThingFrameIncrement(am, &mRealAnimIndex);
	}

	if(fi != G0)
	{
		mFrameInc = fi;
		mAnimMode = am;
		return		TRUE;
	}

	return FALSE;
}



//******************************************************************************************
void	CCharacter::StateStunned()
{
	mStateTimer ++ ;
	if (mStateTimer < mCurrentStunRecoveryTime )
	{
	//	CONSOLE.AddString("Stunned") ;
		return ;
	}

	PopState() ;

	if ((mState == CS_ASLEEP) || (mState == CS_SCANNING))
			ChangeState(CS_NORMAL, CStateInit());

}

//******************************************************************************************

GINT	CCharacter::GroundFriction(EFrictionType friction_type)
{
//	if (mIsBlocking)
//	{
//		return SUPERTYPE::GroundFriction() ;
	//	return 	GINT(0, 65069);
//	}

	if (mState == CS_FINISHED && GAME.GetIVar(IV_LEVEL_TYPE) == LT_KING)
	{
		return G(0,55000) ;
	}
	else if (mState == CS_STUNNED )
	{
		return G(0,55000) ;
	}
	else if (mState == CS_KNOCKED_OUT)
	{
		return G(0,63000) ;
	}
	else
	{
		 return SUPERTYPE::GroundFriction(friction_type);
	}
}


//******************************************************************************************
void	CCharacter::Damage(GINT amount)
{
	if ((mStunned == 0) && (mState != CS_DEAD))
	{
		mLife -= (amount * 25).Whole();
	}

	// sample?
	if (IsPrimary())
	{
		SINT 	id = _GetClassID();
		SINT	s = -1;
		switch (id)
		{
		case OID_CPrimaryAngel: 	s = SOUND_1PUNCHED; break;
		case OID_CPrimaryApostle: 	s = SOUND_2PUNCHED; break;
		case OID_CPrimaryBenny: 	s = SOUND_3PUNCHED; break;
		case OID_CPrimaryDoctor: 	s = SOUND_4PUNCHED; break;
		case OID_CPrimaryLockdown: 	s = SOUND_5PUNCHED; break;
		case OID_CPrimarySophie: 	s = SOUND_6PUNCHED; break;
		};
		if (s != -1)
			SOUND.PlaySample(s, this, FALSE, TRUE);
	}
}


//******************************************************************************************
BOOL	CCharacter::IsStriking()
{
	switch(mAnimMode)
	{
	case AM_PUNCH:
	case AM_PUNCH2:
	case AM_PUNCH3:
	case AM_WEAPON_ATT:
	case AM_RAGE_ATTACK:
	case AM_CHARGE:
	case AM_SPIN_ATTACK:
	case AM_RUN_PUNCH_LEFT:
	case AM_RUN_PUNCH_RIGHT:
	case AM_JUMP_ATTACK:
	case AM_PUNCH_HIT:
	case AM_WEAPON_HIT:
	case AM_JUMP_HIT:
		return TRUE ;
	}
	return FALSE ;
}


//******************************************************************************************
BOOL	CCharacter::IsStrikingForward()
{
	switch(mAnimMode)
	{
	case AM_PUNCH:
	case AM_PUNCH2:
	case AM_PUNCH3:
	case AM_WEAPON_ATT:
	case AM_CHARGE:
	case AM_JUMP_ATTACK:
	case AM_PUNCH_HIT:
	case AM_WEAPON_HIT:
	case AM_JUMP_HIT:
		return TRUE ;
	}
	return FALSE ;
}

//******************************************************************************************
BOOL	CCharacter::IsLongRangeStriking()
{
	switch(mAnimMode)
	{
	case AM_CHARGE:
	case AM_JUMP_ATTACK:
	case AM_JUMP_HIT:
		return TRUE ;
	}
	return FALSE ;
}



//******************************************************************************************
void	CCharacter::Stun(SINT for_time)
{
	ChangeState(CS_STUNNED, CStateInit(0)) ;

	mCurrentStunRecoveryTime = for_time ;
}


//******************************************************************************************
BOOL	CCharacter::Strike(const CStrike& strike)
{
	// if in spin attack strike will fail

	if (mState ==CS_TELEPORT) return FALSE ;

	if ((mAnimMode == AM_SPIN_ATTACK ||
		mAnimMode == AM_RAGE_ATTACK) &&
		strike.mOverwriteSpin == FALSE)
	{
		return FALSE ;
	}

	GINT max_small_punch = SMALL_COMBAT_MAX_VELOCITY ;

	// move back so punch does not go through us
	CCharacter* originator = strike.mOriginator ;

	if (originator->mAnimMode == AM_PUNCH ||
		originator->mAnimMode == AM_PUNCH2 ||
		originator->mAnimMode == AM_PUNCH3 ||
		originator->mAnimMode == AM_PUNCH_HIT)
	{
		GVector them = strike.mOriginator->GetPos() ;
		GVector them_to_u = mPos- them ;
		GINT mag = them_to_u.MagnitudeXY() ;

		if (mag < strike.mShoulderToFistDist && strike.mStrikedWith == SI_SMALL_PUNCH && strike.mWillKnockOut == FALSE)
		{
			AddStrikeVelocity(strike.mAttackVelocity*2, SMALL_COMBAT_MAX_VELOCITY* 2) ;
			max_small_punch *=2 ;
		}
	}


	// (JCL) - if just been hit - don't get hit again
	if ((mState == CS_KNOCKED_OUT) && (mStateTimer < (CHAR_KNOCK_OUT_TIME  / 3)))
		return FALSE;

	// ok we can punch someone who is jump attacking us, but if we are jump attacking they win..
	if (IsStriking() && strike.mOriginator->GetRenderAnimation()  == AM_JUMP_HIT && mAnimMode != AM_JUMP_HIT && mAnimMode != AM_JUMP && (!IsGillam()) )
	{
		return FALSE ;
	}
	// check if we are blocking and we can block that type
	if (mIsBlocking && strike.mStrikedWith & mCanBlockBF)
	{
		// we are blocking so is our attacker facing me

		GINT angle_attacker_to_you = AngleDifference(mYaw, strike.mOriginator->GetYaw() ) ;

		BOOL successfull_block = FALSE ;

		// ok if spin attack then can't assume attack is facing us !! so special case
		if (strike.mStrikedWith == SI_SPIN_PUNCH)
		{
			GVector us_to_them = strike.mOriginator->GetPos() - mPos ;
			GINT angle_us_to_them = -GATAN2(us_to_them.X, us_to_them.Y) ;
			GINT ad =  GABS(AngleDifference(mYaw, angle_us_to_them)) ;

			if (ad < BLOCKING_DEFFENSE_ANGLE )
			{
				successfull_block = TRUE ;
			}
		}
		// assume attacker was facing us to hit us
		else if (GABS(angle_attacker_to_you) > ( G_PI - BLOCKING_DEFFENSE_ANGLE )  )
		{
			successfull_block = TRUE ;
		}

		if (successfull_block)
		{
		// SRG removed as primarys no longer have the flinch block animation

			// there is no flinch block animation for non primarys
		//	if (IsPrimary())
		//	{
		//		SetAnimMode(AM_FLINCH_BLOCK);
		//	}

		//	GVector half_vel = strike.mAttackVelocity * G(0,50000) ;

			if (strike.mStrikedWith == SI_CHARGE_PUNCH)
			{
				CStrike strike1 = strike ;
				strike1.mStrength /= 3 ;
				SUPERTYPE::Strike(strike1) ;
				AddStrikeVelocity(strike.mAttackVelocity / G(2) ,BIG_PUNCH_COMBAT_MAX_VELOCITY) ;
				return FALSE ;
			}

			AddStrikeVelocity(strike.mAttackVelocity, SMALL_COMBAT_MAX_VELOCITY) ;
			return FALSE ;
		}
	}

	TriggerVibration((strike.mStrength>11)? 4:2);
	BOOL result = SUPERTYPE::Strike(strike) ;


	// if stike has killed me and we are on a muliplayer tag level, award pther player
	// up to 5 points
	if (mLife <=0 &&
		GAME.IsMultiPlayer() &&
		GAME.GetIVar(IV_LEVEL_TYPE) == LT_TAG &&
		strike.mOriginator->IsPrimary())
	{
		SINT tokens= mLevelData.GetNumTokensPickedUp() ;
		if (tokens > 5) tokens = 5 ;
		strike.mOriginator->GetLevelData().AddTokens(tokens) ;
	}

	// if we we're blocking we ant now cos we've been clobbered.
	mIsBlocking = FALSE ;

	// if it won't knock you out then stun you at this level
	if (!strike.mWillKnockOut )
	{
		if (strike.mStrikedWith == SI_SPIN_PUNCH)
		{
			Stun(mSpinStunRecoveryTime) ;
			AddStrikeVelocity(strike.mAttackVelocity,BIG_PUNCH_COMBAT_MAX_VELOCITY) ;
		}
		else
		{
			Stun(mDefaultStunRecoveryTime) ;
			AddStrikeVelocity(strike.mAttackVelocity, max_small_punch) ;
		}
	}
	else
	{
		AddStrikeVelocity(strike.mAttackVelocity,BIG_PUNCH_COMBAT_MAX_VELOCITY) ;
	}

	// drop thing we are holding
	if (mHoldingThing) Drop() ;

	// change velocity of character

//	if (result == TRUE) mDrawUsWhite = TRUE ;

	return result;
}


//******************************************************************************************
void	CCharacter::ScoreStrike(CThing* thing_we_hit)
{
	// don't score for striking badguys in these levels
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_KING ||
		GAME.GetIVar(IV_LEVEL_TYPE) == LT_DROID_BALL)
	{
		return ;
	}

	CCharacter* character_we_hit = thing_we_hit->IsCharacter() ;
	// did we struck a non character ( eg a hov bot )
	if (character_we_hit == NULL)
	{
		mScore+=thing_we_hit->StrikeScore() ;
		return ;
	}

	switch (character_we_hit->GetState() )
	{
		case CS_KNOCKED_OUT:
		{
			mScore+=100;
			break ;
		}
		case CS_DEAD:
		{
			mScore+=400;
		}
		default:
		{
			mScore+=25 ;
		}
	}
}

//******************************************************************************************
void	CCharacter::ClearYourTarget()
{
	if (mCurrentTarget)
	{
		mCurrentTarget = NULL ;
	}
}

//******************************************************************************************
void	CCharacter::ProcessAnimation()
{
	if (mAnimMode == AM_MID_JUMP)
		if (mFlags & TF_ON_GROUND)
			SetAnimMode(GetNextAnimCharacter(mAnimMode));

	GINT	old_frame = mFrame;

	mFrame+=mFrameInc;


	switch (mAnimMode)
	{

	case AM_CHARGE:
	{
		AnimTrigger(ID_GMATRIX,TRUE) ;
		// stick on charge anim if we are still going fast enough

		GINT	v = mVelocity.MagnitudeXY();
		if (v> (RUNNING_VELOCITY-G(0,2000)))
		{
			mFrame=G0 ;
			mLastFrame = -G1; // reset interpolation.
		}

		break ;
	}


#define NUM_SPIN_CHECKS 6
#define START_HITTING_SPIN G(0, 30000)
#define STOP_HITTING_SPIN  G(0, 49536)
	case AM_RAGE_ATTACK:
	{

		GMatrix foo;
		GINT currentyaw = mYaw ;

		GINT step = (STOP_HITTING_SPIN -START_HITTING_SPIN)  / NUM_SPIN_CHECKS ;
		GINT radians_step = (G_2PI) / NUM_SPIN_CHECKS ;
		for (GINT check_fr = START_HITTING_SPIN ; check_fr <=STOP_HITTING_SPIN; check_fr+=step)
		{
			currentyaw-=radians_step;
			if (currentyaw < -G_PI) currentyaw+=(G_2PI) ;

			if (old_frame < check_fr && mFrame >=check_fr)
			{
				foo.MakeRotationYaw(currentyaw) ;
				AnimTrigger(foo, TRUE) ;
			}
		}

		SINT torsoid = ((CCharBall*)this)->GetTorsoID() ;

		// angel apostle
		if (torsoid == OID_CPrimaryAngelTorso || torsoid == OID_CPrimaryApostleTorso)
		{
			if (mFrame > STOP_HITTING_SPIN && mRage > G0)
			{
				mFrame = START_HITTING_SPIN - G(0,8000);
				mLastFrame = -G1; // reset interpolation.
			}
		}

		// benny
		if (torsoid == OID_CPrimaryBennyTorso )
		{
			if ((mFrame > STOP_HITTING_SPIN-G(0,2000)) && mRage > G0)
			{
				mFrame = START_HITTING_SPIN - G(0,2000);
				mLastFrame = -G1; // reset interpolation.
			}
		}

		// lockdown // doctor
		if (torsoid == OID_CPrimaryLockdownTorso || torsoid == OID_CPrimaryDoctorTorso)
		{
			if ((mFrame > STOP_HITTING_SPIN-G(0,3000)) && mRage > G0)
			{
				mFrame = START_HITTING_SPIN - G(0,7000);
				mLastFrame = -G1; // reset interpolation.
			}
		}

		// sophie
		if (torsoid == OID_CPrimarySophieTorso )
		{
			if ((mFrame > STOP_HITTING_SPIN-G(0,1000)) && mRage > G0)
			{
				mFrame = START_HITTING_SPIN - G(0,7000);
				mLastFrame = -G1; // reset interpolation.
			}
		}

		mRage-=G(0,850);
		break ;
	}

	case AM_SPIN_ATTACK:
	{

		GMatrix foo;
		GINT currentyaw = mYaw ;

		GINT step = (STOP_HITTING_SPIN -START_HITTING_SPIN)  / NUM_SPIN_CHECKS ;
		GINT radians_step = (G_2PI) / NUM_SPIN_CHECKS ;
		for (GINT check_fr = START_HITTING_SPIN ; check_fr <=STOP_HITTING_SPIN; check_fr+=step)
		{
			currentyaw-=radians_step;
			if (currentyaw < -G_PI) currentyaw+=(G_2PI) ;

			if (old_frame < check_fr && mFrame >=check_fr)
			{
				foo.MakeRotationYaw(currentyaw) ;
				AnimTrigger(foo, TRUE) ;
			}
		}
		break ;
	}

	case AM_THROW:
	{
#define THROW_ANIM_TRIGGER G(0,41000)

		if ((old_frame < THROW_ANIM_TRIGGER) && (mFrame >= THROW_ANIM_TRIGGER))
		{
			AnimTrigger(ID_GMATRIX, TRUE);
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

			if (mLastPunchResult == PR_HIT && mAnimPauseCount < 2 && ( mAnimMode == AM_PUNCH2 || mAnimMode == AM_PUNCH3))
			{
				mFrame-=mFrameInc ;
				mLastFrame = -G1; // reset interpolation.
				mAnimPauseCount++ ;
			}
		}
		else
		{
			if (old_frame < GINT_HALF && mAnimMode == AM_PUNCH_HIT)
			{
				SINT torsoid = ((CCharBall*)this)->GetTorsoID() ;
				if (torsoid == OID_CPrimaryBennyTorso || torsoid == OID_CPrimaryDoctorTorso)
				{
					GMatrix fred = ID_GMATRIX;
					AnimTrigger(fred, FALSE);
				}
			}
			mAnimPauseCount = 0 ;
		}
	}
	}




	if(mFrame>=G1)
	{
		if(IsAnimLooped(mAnimMode))
		{
			mFrame-=G1;
			mLastFrame = -G1; // reset interpolation.

			// sort out walking, running etc.
			if ((mAnimMode == AM_STAND) || (mAnimMode == AM_WALK) || (mAnimMode == AM_RUN))
				SetAnimMode(mAnimMode, FALSE);
		}
		else
		{
			EAnimMode nam = GetNextAnimCharacter(mAnimMode);
			if (nam != AM_INVALID)
				SetAnimMode(GetNextAnimCharacter(mAnimMode));
			else
			{
				mFrame = G1; // don't loop - pause at the end of the anim instead
				mLastFrame = -G1; // reset interpolation.
			}
		}
	}
}


//******************************************************************************************
//******************************************************************************************
//** State Stuff

void	CCharacter::Process()
{
//	PROFILE_FN(Character);

//	PROFILE_START(Character);
	mLastFrame = mFrame;
	mLastOrientation = mOrientation;

	ProcessAnimation();



	// if first time  and we have a weapon to attach to character then do it now
	if (mInitialHoldingThingOID> OID_NONE)
	{
		CThing *t = SpawnThing(mInitialHoldingThingOID);
		if (t)
		{
			CInitThing i;
			i.mPos = mPos;
			i.mYaw = G_PI_2 ;
			i.mPitch = G(0) ;
			i.mRoll = G(0) ;
			t->Init(&i);
		}
		CHoldableThing * item = t->IsHoldable();
		if (!t)
		{
			CONSOLE.AddString("Warning Character was given a non holdable as a initial object") ;
		}
		else
		{
			item->Pickup(this) ;
			mHoldingThing = item;
		}
		mInitialHoldingThingOID=OID_NONE;
	}



	if (GAME.IsGameRunning() || mState == CS_TELEPORT)  // teleporting in at start of game
	{
		switch (mState)
		{
		case CS_NORMAL:		StateNormal();		break;
		case CS_FINISHED:	StateFinished();	break;
		case CS_TELEPORT:	StateTeleport();	break;
		case CS_DEAD:		StateDead();		break;
		case CS_KNOCKED_OUT:StateKnockedOut();	break;
		case CS_ASLEEP:		StateAsleep();		break;
		case CS_SCANNING:	StateScanning();	break;
		case CS_STUNNED:	StateStunned();		break;
		case CS_LASERED:	StateBeingLasered() ; break ;
		case CS_WAITING:	break;  //!

		default: /*ASSERT(0);*/						break;
		};
	}


	GVector vel_before_hit = mVelocity;

	//hmm
//	if (mFlags & TF_ON_GROUND)
	if (MAP.GetMapHeight(mPos) - mPos.Z < Radius() * G(1, 10000))
	{
		mLastFrameOnGround = GAME.GetGameTurn() ;
		mLastFrameOnGroundPos = mPos;
	}

//	PROFILE_END(Character);

	if (mInvisible) return ;

	SUPERTYPE::Process();

//	PROFILE_START(Character);



	if (mFlags & TF_ON_GROUND)
	{
		//if (vel_before_hit.Z > G(0,32768) )
	//	{
	//		GVector map_normal = MAP.GetMapNormal(mPos);
	//		GVector vel = vel_before_hit ;
	//		GINT dot = map_normal * vel_before_hit  ;
	//		GINT damage = dot * vel_before_hit.Magnitude() ;
#if TARGET == PC
		//	char temp[50] ;
		//	sprintf(temp,"dot = %f  damage = %f", GToF(dot), GToF(damage)) ;
		//	CONSOLE.AddString(temp) ;
#endif
		
		// Vibrate the controller (if it's okay...)
	//	TriggerVibration(1);
	//	}

		mLastHeightOnGround = mPos.Z ;

	}

	if (mHoldingThing)
	{
		if (_GetClassID() != OID_CPrimaryDoctor)
		{
			GVector	e0 = GetRenderThingEmitterAnimated(0);
			GVector	e1 = GetRenderThingEmitterAnimated(1);
			GVector	e2 = GetRenderThingEmitterAnimated(2);

			GVector d1 = e0 - e1;
			GVector d2 = e2 - e1;

			GINT	m1 = d1.Magnitude();
			GINT	m2 = d2.Magnitude();

			if ((m1 > G(0, 100)) && (m2 > G(0, 100)))
			{
				d1 /= m1;
				d2 /= m2;

				// move to pos
				mHoldingThing->Move(mPos + e1 + (d1 * (m1 /** GINT_HALF*/)) + (d2 * (m2 * GINT_HALF)));
				// JCL use velocity to fix interpolation
//				GVector newpos = (mPos + e1 + (d1 * (m1 /** GINT_HALF*/)) + (d2 * (m2 * GINT_HALF)));
//				mHoldingThing->SetVelocity(newpos - mHoldingThing->GetPos());
								
				// well, it would be nice if these 2 vectors were perpendicular...
				// but it appears they aren't in general....
				GVector u = -d1 ^ d2;
				u.Normalise();
				GVector r = d1 ^ u;

				// orientation
				// like a camera look matrix!
				GMatrix	o;
				o.Row[2] = d1;
				o.Row[0] = u;
				o.Row[1] = r;

				o.TransposeInPlace();
				mHoldingThing->SetOrientation(o);
			}
			else
				mHoldingThing->Move(mPos + e1);
		}
		else
		{
			// ok the doctor can only pick up droid balls so just use emitter 3
			// to work out a position.  Orientation is not required
			mHoldingThing->Move(mPos + GetRenderThingEmitterAnimated(3));
		}

	}

	// Process Weapon Trail??

	CWeapon* weapon = NULL ;
	if (mHoldingThing)
	{
		weapon= mHoldingThing->IsWeapon() ;
	}


	if ((mBoostTrailCount > 0) && ((mLastTrailUse == TU_BALL) || (GAME.GetGameTurn() - mLastTrailUseTime > 32)))
	{
		GVector	e0;
		if (GABS(mVelocity.X) > GABS(mVelocity.Y))
		{
			e0 = GVector(-mVelocity.Z,G0,mVelocity.X ) ;
		}
		else
		{
			 e0 = GVector(-mVelocity.Y, mVelocity.X, G0) ;
		}
		e0.Normalise();
		GINT	scale = G(mBoostTrailCount) / 20;
		if (scale > G1)
			scale = G1;
//		e0=e0*(Radius() * G(0,55000) * scale) ;
		e0=e0*(Radius() * G(0,35000) * scale) ;
		mTrail.ProcessAdd(mPos - mVelocity + e0, mPos - mVelocity - e0);

		mLastTrailUse = TU_BALL;
		mLastTrailUseTime = GAME.GetGameTurn();
		mBoostTrailCount --;
	}
	else if (mDFlags&DFC_DrawTrail && mAnimMode == AM_PUNCH_HIT || mAnimMode == AM_RAGE_ATTACK || mAnimMode == AM_SPIN_ATTACK)
	{
		if ((mLastTrailUse == TU_ATTACK) || (GAME.GetGameTurn() - mLastTrailUseTime > 32))
		{
			GVector	e0 = GetRenderThingEmitterAnimated(1);
			GVector	e1 = GetRenderThingEmitterAnimated(2);
			mTrail.ProcessAdd(mPos - mVelocity + e0, mPos - mVelocity + e1);

			mLastTrailUse = TU_ATTACK;
			mLastTrailUseTime = GAME.GetGameTurn();
		}
	}
	else if ((mAnimMode == AM_WEAPON_HIT) && (weapon))
	{
		if ((mLastTrailUse == TU_ATTACK) || (GAME.GetGameTurn() - mLastTrailUseTime > 32))
		{
			mTrail.ProcessAdd(weapon->GetPos() + weapon->GetRenderThingEmitter(1),
							  weapon->GetPos() + weapon->GetRenderThingEmitter(0));

			mLastTrailUse = TU_ATTACK;
			mLastTrailUseTime = GAME.GetGameTurn();
		}
	}
	else
		mTrail.ProcessNone();


// Process the 'rings' around the character when in water...

	GINT w=GAME.GetLevelData()->mGVars[WATER_HEIGHT];
	if(w!= G0)
	{
		if(mPos.Z>(w - Radius()))
		{
//			if(((GAME.GetGameTurn()&15)==0)||((RANDOM()&7)==0))
			GINT	vsq = mVelocity.MagnitudeSqXY();
			if (((vsq > RUNNING_VELOCITY) &&
			     (((GAME.GetGameTurn() + mNumber) & 1) == 0)) ||
			    ((((GAME.GetGameTurn() + mNumber) & 3) == 0)))
			{
				// create ring
				CParticle *ptcl;
				if(mPos.Z<(w+Radius()))
				{
					ptcl	=AddParticleEffect(PART_WATERRING);
					if(ptcl)
					{
						ptcl->mPos.X=mPos.X;
						ptcl->mPos.Y=mPos.Y;
						ptcl->mPos.Z.mVal=w.mVal;
					}

					// and maybe some other stuff
					SINT 	np = ((vsq - G(0, 5000)) * 30).Whole();

					if (np > 0)
					{
						if (np > 10)
							np = 10;

						GVector	spos = mPos;
						spos.Z += Radius();

						GINT	baserad = GATAN2(mVelocity.Y, mVelocity.X) + G_PI;  // guaranteeed not 0/0...

						mParticleSystem.MakeSplash2(&spos, np, baserad, G(0, 16384));
					}
				}
				else
				{
					CMapCell*cell=MAP.GetCell(CCellRef(mPos));
					SINT z=cell->GetHeight().mVal;
					ptcl=mParticleSystem.CreateParticle(PART_WRING2);
					if(ptcl)
					{
						ptcl->mVelocity.Y.mVal=0;
						ptcl->mVelocity.Z.mVal=-0x100;
						ptcl->mDelay=0;
						//ptcl->mColour=0x00ffffff;
						ptcl->mPos.X.mVal=mPos.X.mVal+(((RANDOM()&0xffff)<<16)>>16);
						ptcl->mPos.Y.mVal=mPos.Y.mVal+(((RANDOM()&0xffff)<<16)>>16);
						ptcl->mPos.Z.mVal=z;
					}
				}

			}
		}

	}

#if TARGET == PSX

#if 0
	if((GAME.GetGameTurn()&0x1f)==0)
	{
		GVector spos = mPos;
		spos.Z -= G(2);

		mParticleSystem.MakeHitEffect(&spos, mYaw);
	}

#endif

/*	if((GAME.GetGameTurn()&0x3f)==0)
	{
		mParticleSystem.MakeExplosion2(&mPos);
	}*/

#endif



	if (mParticleSystem.IsActive())
	{
		BOOL	res = mParticleSystem.Process();
		if (res)
			mParticleSystem.DeActivate();
	}

	//! debug for Damo
//#if TARGET == PSX
#if 0
	if ((GAME.GetGameTurn() & 127) == 0)
	{
		CThing *t = SpawnThing(OID_CFXRageAttack);
		if (t)
		{
			CInitThing i;
			i.mPos = mPos;
			((CFXRageAttack*)t)->SetOwner(this);
			t->Init(&i);
		}
	}
	//if ((GAME.GetGameTurn() & 63) == 32)
	//{
	//	CThing *t = SpawnThing(OID_CFXRageAttack);
	//	if (t)
	//	{
	//		CInitThing i;
	//		i.mPos = mPos;
	//		t->Init(&i);
	//	}
	//}
#endif


// Mussed with this a bit.. DAMO!
// (JCL) and me!
	GINT wh;
	wh=GAME.GetLevelData()->mGVars[WATER_HEIGHT];
	if (wh != G0)
	{
		GINT	bottom = mPos.Z + Radius();
		// Do check for entering/exiting water....
		if((bottom > wh)&&(!(mDFlags&DFC_Inwater)))  // In water now, but wasnt...
		{
//			printf("Entering water\n");

			GVector	spos = mPos;
			spos.Z += Radius();
			mParticleSystem.MakeSplash(&spos);
			mDFlags|=DFC_Inwater;
			SOUND.PlaySample(SOUND_WATER1, this);
		}
		else if((bottom < wh)&&(mDFlags&DFC_Inwater))  // Wasn't in water but I am now..
		{
			GVector	spos = mPos;
			spos.Z += Radius();
			mParticleSystem.MakeSplash(&spos);
			mDFlags&=~DFC_Inwater;
			SOUND.PlaySample(SOUND_WATER1, this);
		}

		// drown???
//		if((DoIDrown())&&(mPos.Z > wh + Radius()))
		if((mPos.Z > wh + Radius()))
		{
			mLife -= 3;

			// Now show lightning effect on character that's underwater (unless dead).... DAMO!
//			if(((CCharBall*)this)->GetCollideShape()==CSH_DOUBLE_SPHERE)
			if (GetCollideShape() == CSH_DOUBLE_SPHERE)
			{
				if (!(GAME.GetGameTurn()&0x7))
				{
					{
						CParticle *p=mParticleSystem.CreateParticle(PART_LIGHTNING);
						if(p)
						{
							p->mLife=20;
							p->uPRing.mBrightness=16384;
							//p->mColour=0xffffff;
							p->mPos.X.mVal=
							p->mPos.Y.mVal=
							p->mPos.Z.mVal=0;
							p->mIsRelative=1;
						}
	
	
					}
				}
			}
			if (!(mDFlags & DFC_InDeepWater))
			{
//				if (_GetClassID() != OID_CPrimaryBall)
				if (GetCollideShape() == CSH_DOUBLE_SPHERE)
					SOUND.PlaySample(SOUND_BUZZ, this, GINT_HALF, TRUE, TRUE);
			}
			mDFlags |= DFC_InDeepWater;
		}
		else
		{
			if (mDFlags & DFC_InDeepWater)
			{
				SOUND.KillSample(this, SOUND_BUZZ);
			}
			mDFlags &= ~DFC_InDeepWater;
		}
	}




//	PROFILE_END(Character);
}


//******************************************************************************************
void	CCharacter::ChangeState(ECharState new_state, const CStateInit &i)
{
	// do I care?
	switch(mState)
	{
	case CS_TELEPORT:
		if (new_state == CS_TELEPORT)
			return; // no
		break;

	// if we are stunned then pop back to the state before we were stunned
	// then change into the new state.  stops state stack problems
	case CS_STUNNED:
	case CS_LASERED:
		PopState() ;
		break;

	case CS_DEAD:
		return; // no - I'm dead, leave me alone!!

	};

	// do I need to store state?
	switch(new_state)
	{
	case CS_TELEPORT:
		PushState();
		{
			// Hackity hack
			// don't do this at start of level (ie when we're waving)
			if (mAnimMode != AM_WAVE)
			{
				CThing *t = SpawnThing(OID_CFXTeleportIn);
				if (t)
				{
					CInitThing it;
					it.mPos = i.mStart - GVector(G0, G0, Radius());
					t->Init(&it);
				}
			}
		}
		break;

	case CS_LASERED:
		PushState();
		break;
	case CS_STUNNED:
	PushState();
		SetAnimMode(AM_FLINCH);
		break;
	default:
		break;
	}

	mState = new_state;

	mStateTimer = i.mTimer;
	mStateDest	= i.mDest  - GVector(G0, G0, Radius());
	mStateStart	= i.mStart - GVector(G0, G0, Radius());

	mStatePosStart		= mPos;
	mStateTimerStart	= mStateTimer;

	// kill state effect
	if (mEffectThing)
		mEffectThing->Die();

	mEffectThing = NULL;

	switch (mState)
	{
	case CS_ASLEEP:
		{
/*			mEffectThing = SpawnThing(OID_CFXZ);
			if (mEffectThing)
			{
				CInitThing	i;

				i.mPos = mPos + GVector(G0, G0, -G(2));
				mEffectThing->Init(&i);
			}*/
		}
		break;
	case CS_DEAD:
		{
			if (mIsBlocking) mIsBlocking = FALSE ;
			SetAnimMode(AM_DYING);
		}
		break ;
	case CS_KNOCKED_OUT:
		{
			if (mIsBlocking) mIsBlocking = FALSE ;
			SetAnimMode(AM_DYING);
		}
		break;

	default:
		break;
	};
}

//******************************************************************************************
void	CCharacter::PushState()
{
	if (mStateStackDepth < MAX_STATE_STACK_DEPTH)
	{
		mStateStack[mStateStackDepth].mState			= mState;
		mStateStack[mStateStackDepth].mStateDest		= mStateDest;
		mStateStack[mStateStackDepth].mStateTimer		= mStateTimer;
		mStateStack[mStateStackDepth].mStatePosStart	= mStatePosStart;
		mStateStack[mStateStackDepth].mStateTimerStart	= mStateTimerStart;

		mStateStackDepth ++;
	}
}

//******************************************************************************************
void	CCharacter::PopState()
{
	if (mStateStackDepth > 0)
	{
		mStateStackDepth --;

		mState				=	mStateStack[mStateStackDepth].mState;
		mStateDest			=	mStateStack[mStateStackDepth].mStateDest;
		mStateTimer			=	mStateStack[mStateStackDepth].mStateTimer;
		mStatePosStart		=	mStateStack[mStateStackDepth].mStatePosStart;
		mStateTimerStart	=	mStateStack[mStateStackDepth].mStateTimerStart;
	}
}



//******************************************************************************************
void	CCharacter::StateKnockedOut()
{


	mStateTimer++;
	if (mStateTimer > mRecoverFromKnockoutTime)
	{
		SetAnimMode(AM_WALK);
		ChangeState(CS_NORMAL, CStateInit());
		if (!IsPrimary() ) Jump() ;
	}
}


//******************************************************************************************
void	CCharacter::StateBeingLasered()
{
	if (GAME.GetGameTurn() & 1 )
	{
		mVelocity = GVector(G(0,4000) ,G(0,4000),G(0,4000)) ;
	}
	else
	{
		mVelocity = GVector(-G(0,4000) ,-G(0,4000),-G(0,4000)) ;
	}

	SetAnimMode(AM_FLINCH);
	Damage(G(0,4322)) ;
	TriggerVibration(3);
	if (mLastFrameBeingLasered < (GAME.GetGameTurn() -1 ))
	{
		PopState();
	}
}

//******************************************************************************************
#define TELPORT_RISE G(5)
void	CCharacter::StateTeleport()
{
	mStateTimer --;

	// comments?  what comments!
	GINT	f = G(mStateTimerStart - mStateTimer) / mStateTimerStart;

	if (mStateTimer < 40 && mStateTimer > 10)
	{
		if (mOnFireFlames[0] != NULL) mOnFireFlames[0]->Invisible() ;
		if (mOnFireFlames[1] != NULL) mOnFireFlames[1]->Invisible() ;
		if (mCrown) mCrown->Invisible() ;
	}

	// wait @ start & end
	f *= G(1, 30000);
	f -= G(0, 15000);

	if (f > G1)
		f = G1;

	if (f < G0)
	{
#if 1
//		f = (f + G(0, 15000)) / G(0, 15000);
		f += G(0, 15000);
//		f /= G(0, 15000);
		f *= G(4, 24187);
#endif

		Move(mStatePosStart + ((mStateStart - mStatePosStart) * f));
	}
	else
	{
		GINT	d = (GCOS((f * G_PI) - G_PI) + G1) / 2;
		GINT	u = (GCOS((f * G_PI * 2) - G_PI) + G1) / 2;

		Move(mStateStart + ((mStateDest - mStateStart) * d) + GVector(G0, G0, -u * TELPORT_RISE));
	}

	if (mStateTimer < 10)
	{
		// re activate flames
		if (mOnFireFlames[0] != NULL) mOnFireFlames[0]->Visible() ;
		if (mOnFireFlames[1] != NULL) mOnFireFlames[1]->Visible() ;
		if (mCrown) mCrown->Visible() ;
	}

	if (mStateTimer == 0)
	{
		// hack for start of level
		if (!(GAME.IsGameRunning()))
			SetAnimMode(AM_WAVE);

		PopState();
		
//		ChangeState(CS_NORMAL, CStateInit());
	}

	// create teleport in effect
	if (mStateTimer == 30)
	{
		CThing *t = SpawnThing(OID_CFXTeleportIn);
		if (t)
		{
			CInitThing it;
			it.mPos = mStateDest;
			t->Init(&it);
		}
	}

	mVelocity = ZERO_GVECTOR;
}

//******************************************************************************************
GINT	CCharacter::GetRenderScale()
{
	// scale the character during teleport effect.
	if (mState == CS_TELEPORT)
	{
		if ((mStateTimer < 11) || (mStateTimer > 39))
		{
			if (mStateTimer < 11)
				return G(10 - mStateTimer) / G(10);
			else
				return G(mStateTimer - 40) / G(10);
		}
	}

	return G1;
}

//******************************************************************************************
BOOL	CCharacter::DoICollideWithMap()
{
	switch (mState)
	{
	case CS_TELEPORT:
		return FALSE;


	default:
		return SUPERTYPE::DoICollideWithMap();
		break;
	};
}

//******************************************************************************************
BOOL	CCharacter::DoIHit(CThing *t)
{
	//! BOLLOX - should be in DoICollide()....

	switch (mState)
	{
	case CS_TELEPORT:
		return FALSE;


	default:
		return SUPERTYPE::DoIHit(t);
	};
}

//******************************************************************************************
void	CCharacter::StateAsleep()
{
	// check to see if the land underneath me has been wibbled
	//! should also react to being hit...

	if (GABS(MAP.GetMapHeightOffset(mPos)) > GINT(0, 2000))
		ChangeState(CS_NORMAL, CStateInit());

}

//******************************************************************************************
GINT	CCharacter::Gravity()
{
	switch (mState)
	{
	case CS_ASLEEP:
	case CS_SCANNING:
	case CS_LASERED:
		return G0;

	default:
	return SUPERTYPE::Gravity();

	}
}

//******************************************************************************************
void	CCharacter::Hit(CThing *that)
{
	if ((mState == CS_ASLEEP) || (mState == CS_SCANNING))
		ChangeState(CS_NORMAL, CStateInit());

	CWeapon *w = that->IsWeapon() ;

	// pickup weapon?
	if (!w || CanIPickupWeapons())
	{
		if (mHoldingThing == NULL)
		{
			CHoldableThing *t = that->IsHoldable();

			if (t)
			{
				CDroidBall* is_droid_ball = t->IsDroidBall() ;
				GINT mh = MAP.GetMapHeight(that->GetPos());
				GINT zdiff = (that->GetPos().Z - mh) + that->Radius() ;

				if ((is_droid_ball || (zdiff > -G(0,8000))) && mState != CS_STUNNED && mState != CS_KNOCKED_OUT && mState != CS_DEAD)
				{
					// only primarys can pick up droid balls
					if (is_droid_ball && ( is_droid_ball->IsStuckToPole() || (!IsPrimary()))) return ;

					if ( t->Pickup(this))
					{
						mHoldingThing = t;
						if (w)
						{
							// we can now block weapon attacks
							mCanBlockBF |= SI_WEAPON ;
						}
					}
				}
			}
		}
	}
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CCharacter::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

//	WORLD.AccumulateOIDResources(OID_CFXZ);
	WORLD.AccumulateOIDResources(mInitialHoldingThingOID);
	WORLD.AccumulateOIDResources(OID_CCrown) ;

	WORLD.AccumulateOIDResources(OID_CFXTeleportIn);
	WORLD.AccumulateOIDResources(OID_CFXRageAttack);

}
#endif

//********************************************************************************************************
#if TARGET == PC
void	CCharacter::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	if (mInitialHoldingThingOID != OID_NONE)
		 i.mOID = mInitialHoldingThingOID;
	else i.mOID = OID_NONE;

	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}
#endif

//******************************************************************************************
void	CCharacter::Trigger(CThing *top)
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		ChangeState(CS_NORMAL, CStateInit()); break;
	case TR_DEACTIVATE:
		ChangeState(CS_DEACTIVATED, CStateInit()); break;
	case TR_TOGGLE:
		{
			if (mState == CS_NORMAL)
			{
				ChangeState(CS_DEACTIVATED, CStateInit());
			}
			else
			{
				ChangeState(CS_NORMAL, CStateInit());
			}
			break ;
		}
	};

	SUPERTYPE::Trigger(top);
}

//******************************************************************************************
void	CCharacter::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		ChangeState(CS_NORMAL, CStateInit()); break;
	case TR_DEACTIVATE:
		ChangeState(CS_DEACTIVATED, CStateInit()); break;
	case TR_TOGGLE:
		{
			if (mState == CS_NORMAL)
			{
				ChangeState(CS_DEACTIVATED, CStateInit());
			}
			else
			{
				ChangeState(CS_NORMAL, CStateInit());
			}
			break ;
		}
	};

	SUPERTYPE::UnTrigger(top);
}


//******************************************************************************************
void	CCharacter::StartBlocking()
{
	if (!mIsBlocking && mFlags & TF_ON_GROUND && mAnimMode != AM_SPIN_ATTACK && mAnimMode != AM_RAGE_ATTACK)
	{
		SetAnimMode(AM_BLOCK_UP);
		mIsBlocking = TRUE ;
	}
}


//******************************************************************************************
void	CCharacter::StopBlocking()
{
	if (mIsBlocking)
	{
		mIsBlocking = FALSE ;
		SetAnimMode(AM_BLOCK_DOWN);
	}
}



//******************************************************************************************
BOOL	CCharacter::Attack(SINT no)
{
	if (no ==1 && mIsBlocking)
	{
		StopBlocking() ;
		return TRUE ;
	}
	if (IsAnimBusy(mAnimMode)) return FALSE ;

	return TRUE ;
}



//******************************************************************************************
void	CCharacter::StopFurthestAwayAttacker()
{
	CCharacter* furthest_target = NULL ;
	GINT furthest_dist = G(0) ;


	for (CCharacter* current_target = mBeingAttackedBy.First() ;
		 current_target != NULL ;
		 current_target = mBeingAttackedBy.Next())
	{
		GINT dist = (current_target->GetPos() - mPos).MagnitudeSq() ;
		if ( dist > furthest_dist )
		{
			furthest_dist = dist ;
			furthest_target = current_target ;
		}
	}
	if (furthest_target) furthest_target->ClearYourTarget() ;
}



//******************************************************************************************
void	CCharacter::ValidateData()
{
	VALIDATE(mEffectThing);
	VALIDATE(mHoldingThing);
	VALIDATE(mCurrentTarget);      //***  Stuart !!   // Not My fault !!
	SUPERTYPE::ValidateData();
}

//******************************************************************************************
void	CCharacter::HitParticle(EParticleType pt)
{
	switch(pt)
	{
	case	PART_FIRE1:
		if (DoIBurn())
		{
//			Stun(G(0, 1000));
		//	if ((GAME.GetGameTurn() & 1) == 0 )
		//	{
			mLife -=1 ;
		//	}
		   CatchOnFire() ;
		}
		break;

	case	PART_BODY_FIRE:
		if (DoIBurn())
		{
			// pass it on ;)
		//	if (!(mFlags & TF_ON_FIRE)) CatchOnFire() ;
			// Note the slowly dying part is done in the
			// charball part as you will hit many particles here
		}
		break;

	default:
		break;
	};
}

//******************************************************************************************
BOOL	CCharacter::CanIJump()
{
	return (((GAME.GetGameTurn() - mLastFrameOnGround) < 4) &&
		    ((GAME.GetGameTurn() - mLastFrameJumped  ) > 4));
}

//******************************************************************************************

void	CCharacter::Jump()
{
	if (CanIJump())
	{
		GVector norm = MAP.GetMapNormal(mLastFrameOnGroundPos);
		GINT	nz = norm.Z;


		if (nz < -G(0, 32768))
		{
			// add velocity proportional to speed already travelling
			GINT	z = -(mVelocity.Z * 6);
			if (z < G1)
				z = G1;

			z = G1 / (z * z);

			GINT	z2 = ((-nz) - G(0, 32768)) * G(2, 0);
			if (z2 < G0) z2 = G0;

			AddVelocity(GVector(G0, G0, DIRECT_JUMP_POWER * z * z2));

			mLastFrameJumped = GAME.GetGameTurn(); // prevent "double" jump
			SetAnimMode(AM_JUMP);
		}
	}
}


//******************************************************************************************
void	CCharacter::FinishedLevel()
{
	if (mLevelData.GetFinishedLevel() == TRUE) return ;

	GAME.DeclareCharacterFinishedLevel(this) ;
	mLevelData.SetFinishedLevel() ;
	mState=CS_FINISHED ;
}



//******************************************************************************************
void	CCharacter::ReachedCheckPoint(SINT check_point_num)
{
	// update the level data stuff
	mLevelData.ReachedCheckPoint(check_point_num) ;
}


//******************************************************************************************
void	CCharacter::StartThrowingAnimation()
{
	if (IsAnimBusy(mAnimMode) || mHoldingThing == NULL) return ;
	SetAnimMode(AM_THROW) ;
}


//******************************************************************************************
void	CCharacter::StartWearingCrown()
{
	if (mCrown) return ;
	mCrown  = (CCrown*) SpawnThing(OID_CCrown);
	if (mCrown)
	{
		CInitThing i;
		i.mPitch = G(0) ;
		i.mRoll = G(0) ;
		mCrown->Init(&i);
		mCrown->Pickup(this);
	}
}


//******************************************************************************************
void	CCharacter::StopWearingCrown()
{
	if (!mCrown) return ;
	mCrown->Die() ;
	mCrown= NULL ;
}


//******************************************************************************************
void	CCharacter::BeingLasered(GVector& hit_point)
{
	mLastFrameBeingLasered = GAME.GetGameTurn() ;

	// do special laser beam hit player effect here
	mParticleSystem.MakeHitEffect(&hit_point, mYaw + G_PI, HT_LASERED);

	if (mState != CS_LASERED && DoIGetLasered())
	{
		ChangeState(CS_LASERED, CStateInit()) ;
	}
}


//******************************************************************************************
void	CCharacter::Drop(HTDropReason reason )
{
	if (mHoldingThing)
	{
		mHoldingThing->Drop(reason) ;

		if (_GetClassID() != OID_CPrimaryDoctor) 
		{
			mCanBlockBF &= (~SI_WEAPON) ;
		}
		mHoldingThing = NULL ;
	}
}


#if TARGET == PC

//******************************************************************************************
void	CCharacter::DrawDebugText(HDC hdc, SINT* y_offset, char* buffer)
{
	SINT size = sprintf(buffer, "---------------------- CHARACTER -------------------");
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;
	size = sprintf(buffer, "this = %08x", this);
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;
	size = sprintf(buffer, "Number chars attacking us = %d", mBeingAttackedBy.Size());
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;
	switch (mState)
	{
	case CS_NORMAL: size = sprintf(buffer, "Character state = NORMAL"); break ;
	case CS_TELEPORT: size = sprintf(buffer, "Character state = TELEPORT"); break ;
	case CS_DEAD: size = sprintf(buffer, "Character state = DEAD"); break ;
	case CS_FINISHED: size = sprintf(buffer, "Character state = FINISHED"); break ;
	case CS_ASLEEP: size = sprintf(buffer, "Character state = ASLEEP"); break ;
	case CS_PATROL: size = sprintf(buffer, "Character state = PATROL"); break ;
	case CS_SCANNING: size = sprintf(buffer, "Character state = SCANNING"); break ;
	case CS_STATIONARY: size = sprintf(buffer, "Character state = STATIONARY"); break ;
	case CS_WAITING: size = sprintf(buffer, "Character state = WAITING"); break ;
	case CS_DEACTIVATED: size = sprintf(buffer, "Character state = DEACTIVETED"); break ;
	case CS_KNOCKED_OUT: size = sprintf(buffer, "Character state = KNOCKED OUT"); break ;
	case CS_STUNNED: size = sprintf(buffer, "Character state = STUNNED"); break ;
	default : size = sprintf(buffer, "Character state = %d", mState); break ;
	}
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;


	if (mCurrentTarget)
		ENGINE.RenderArrow(GVectorToFVector(GetRenderPos()), GVectorToFVector(mCurrentTarget->GetRenderPos()), 0xffff4010);


	size = sprintf(buffer, "Attack Bit Field = %d", mToAttackBF);
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;

	size = sprintf(buffer, "Block Bit Field = %d", mCanBlockBF);
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;

	if (mHoldingThing)
	{
		size = sprintf(buffer, "Holding a thing = TRUE");
	}
	else
	{
		size = sprintf(buffer, "Holding a thing = FALSE");
	}
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;


	if (mIsBlocking)
	{
		size = sprintf(buffer, "BLOCKING");
	}
	else
	{
		size = sprintf(buffer, "not blocking");
	}
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;

	switch (mAnimMode)
	{
	case AM_INVALID : size = sprintf(buffer, "Animation = INVALID"); break ;
	case AM_NONE : size = sprintf(buffer, "Animation = NONE"); break ;
	case AM_WALK: size = sprintf(buffer, "Animation = WALK"); break ;
	case AM_RUN: size = sprintf(buffer, "Animation = RUN"); break ;
	case AM_STAND: size = sprintf(buffer, "Animation = STAND"); break ;
	case AM_JUMP: size = sprintf(buffer, "Animation = JUMP"); break ;
	case AM_MID_JUMP: size = sprintf(buffer, "Animation = MID JUMP"); break ;
	case AM_PUNCH: size = sprintf(buffer, "Animation = PUNCH"); break ;
	case AM_FALLING: size = sprintf(buffer, "Animation = FALLING"); break ;
	case AM_LANDING: size = sprintf(buffer, "Animation = LANDING"); break ;
	case AM_FLINCH: size = sprintf(buffer, "Animation = FLINCH"); break ;
	case AM_DYING: size = sprintf(buffer, "Animation = DYING"); break ;
	case AM_WALK2STAND: size = sprintf(buffer, "Animation = WALK2STAND"); break ;
	case AM_TAILGRAB: size = sprintf(buffer, "Animation = TAILGRAB"); break ;
	case AM_SHOOT: size = sprintf(buffer, "Animation = SHOOT"); break ;
	case AM_SWITCH_ON: size = sprintf(buffer, "Animation = SWITCH ON"); break ;
	case AM_CHRISTAIR: size = sprintf(buffer, "Animation = CHRISTAIR"); break ;
	case AM_BORED: size = sprintf(buffer, "Animation = BORED"); break ;
	case AM_WAVE: size = sprintf(buffer, "Animation = WAVE"); break ;
	case AM_PUNCH2: size = sprintf(buffer, "Animation = PUNCH 2"); break ;
	case AM_PUNCH3: size = sprintf(buffer, "Animation = PUNCH 3"); break ;
	case AM_WEAPON_ATT: size = sprintf(buffer, "Animation = WEAPON ATTACK"); break ;
	case AM_JUMP_ATTACK: size = sprintf(buffer, "Animation = JUMP ATTCK"); break ;
	case AM_BLOCK: size = sprintf(buffer, "Animation = BLOCK"); break ;
	case AM_FLINCH_BLOCK: size = sprintf(buffer, "Animation = FLINCH BLOCK"); break ;
	case AM_FLINCH_BEHIND: size = sprintf(buffer, "Animation = FLINCH BEHIND"); break ;
	case AM_PUNCH_BEHIND: size = sprintf(buffer, "Animation = PUNCH BEHIND"); break ;
	case AM_BLOCK_UP: size = sprintf(buffer, "Animation = BLOCK UP"); break ;
	case AM_BLOCK_DOWN: size = sprintf(buffer, "Animation= BLOCK DOWN"); break ;
	case AM_GUARD_UP: size = sprintf(buffer, "Animation = GUARD UP"); break ;
	case AM_GUARD_DOWN: size = sprintf(buffer, "Animation = GUARD DOWN"); break ;
	case AM_RAGE_ATTACK: size = sprintf(buffer, "Animation = RAGE ATTACK"); break ;
	case AM_SPIN_ATTACK: size = sprintf(buffer, "Animation = SPIN ATTACK"); break ;
	case AM_RUN_PUNCH_RIGHT: size = sprintf(buffer, "Animation = RUN PUNCH RIGHT"); break ;
	case AM_RUN_PUNCH_LEFT: size = sprintf(buffer, "Animation = RUN PUNCH LEFT"); break ;
	case AM_CHARGE: size = sprintf(buffer, "Animation = CHARGE"); break ;
	case AM_THROW: size = sprintf(buffer, "Animation = THROW"); break ;
	case AM_VICTORY: size = sprintf(buffer, "Animation = VICTORY"); break ;
	case AM_PUNCH_HIT: size = sprintf(buffer, "Animation = PUNCH HIT"); break ;
	case AM_WEAPON_HIT: size = sprintf(buffer, "Animation = WEAPON HIT"); break ;
	case AM_JUMP_HIT: size = sprintf(buffer, "Animation = JUMP HIT"); break ;
	case AM_GATE: size = sprintf(buffer, "Animation = GATE"); break ;
	default : size = sprintf(buffer, "Animation = %d", mAnimMode); break ;
	}
	TextOut(hdc, 0, *y_offset, buffer, size);
	*y_offset+=12 ;
}

#endif

//***********************************************
// cheat!!!

#if TARGET == PC
#include	"PCCheats.h"
#elif TARGET == DC
#include	"DCCheats.h"
#elif TARGET == PSX
#include	"PSXCheats.h"
#endif


GINT	CCharacter::GetMapDamping()
{
	if (GAME.IsCheatEnabled(CHEAT_BOUNCY))
		return G(0, 50000);
	else
		return G(0, 1000);
};


//***********************************************
// Iterpolate!!!
GINT	CCharacter::GetRenderFrame()
{
	if (mLastFrame != -G1)
	{
		GINT	fr = mLastFrame + ((mFrame - mLastFrame) * GAME.GetFrameRenderFraction());
		
		// safety
		if (fr > G(0, 65500))
			fr = G(0, 65500);
		if (fr < G0)
			fr = G0;
			
		return fr;
	}
	else
		return mFrame;
}

//***********************************************
GVector	CCharacter::GetCarriedThingRenderPos()
{
	// ok - need to use interpolated position and animation to get the render position for the thing.
	// NB - could also be used to generate orientation.

	GVector	e0 = GetRenderThingEmitterAnimated(0);
	GVector	e1 = GetRenderThingEmitterAnimated(1);
	GVector	e2 = GetRenderThingEmitterAnimated(2);

	GVector d1 = e0 - e1;
	GVector d2 = e2 - e1;

	GINT	m1 = d1.Magnitude();
	GINT	m2 = d2.Magnitude();
	
	GVector dest;

	if ((m1 > G(0, 100)) && (m2 > G(0, 100)))
	{
		d1 /= m1;
		d2 /= m2;

		// move to pos
		dest = GVector(GetRenderPos() + e1 + (d1 * (m1 /** GINT_HALF*/)) + (d2 * (m2 * GINT_HALF)));
/*						
		// well, it would be nice if these 2 vectors were perpendicular...
		// but it appears they aren't in general....
		GVector u = -d1 ^ d2;
		u.Normalise();
		GVector r = d1 ^ u;

		// orientation
		// like a camera look matrix!
		GMatrix	o;
		o.Row[2] = d1;
		o.Row[0] = u;
		o.Row[1] = r;

		o.TransposeInPlace();
		mHoldingThing->SetOrientation(o);*/
	}
	else
		dest = GVector(GetRenderPos() + e1);	

	return dest;
}

//***********************************************
GMatrix	CCharacter::GetRenderOrientation(SINT no)
{
	if (no != 0)
		return SUPERTYPE::GetRenderOrientation(no);

	// interpolate!!
	GMatrix foo = mLastOrientation + ((mOrientation - mLastOrientation )* GAME.GetFrameRenderFraction());

	foo.Normalise();

	return foo;
}
