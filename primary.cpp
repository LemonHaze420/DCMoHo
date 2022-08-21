#include	"Common.h"

#include	"Globals.h"
#include	"Primary.h"
#include	"Player.h"
#include	"Weapon.h"

#define BRAKE_UNIT_COST		G(0, 8000)
#define	BRAKE_FRICTION		G(0, 31000)

#define PRIMARY_TURN_SPEED		(G(0, 10000))

#define PRIMARY_NUM_FRAMES_END_COMBO 45

//******************************************************************************************
void	CPrimary::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	// allow players to block spin attacks
	mCanBlockBF |= SI_SPIN_PUNCH ;

	SitOnGround();
	mBrakePower = G1;

	mPlayer = WORLD.GetPlayerForPrimary(this);
	WORLD.SetPrimaryThing(this, mPlayer->mNumber);
	mPlayer->SetPrimary(this) ;

	SetMyAttackID(AT_PRIMARY) ;

//	SetAnimMode(AM_WAVE);
	mFuckedUpAtStartOfRaceCount = 0 ;
	mSpawnedBy = NULL ;
	mDoRacerRageBoost = FALSE ;

	// Teleport in
	CStateInit si;
	si.mDest  = mPos;
	si.mTimer = 50;
	si.mStart = mPos;
	ChangeState(CS_TELEPORT, si);
	mStateTimer = 31; //hmm!!
	mSpinStunRecoveryTime = 25 ;

}

//******************************************************************************************
BOOL	CPrimary::Strike(const CStrike& strike)
{
	BOOL	val = SUPERTYPE::Strike(strike) ;

	// can't increase rage whilst do rage
	if (mAnimMode != AM_RAGE_ATTACK)
	{
		mRage+=G(0,6554) ;
		if (mRage > G1) mRage = G1 ;
	}

	return val;
}

//******************************************************************************************
void	CPrimary::Process()
{

	// Brake?


	// ok, rage increase in muliplayer race levels if we are behind the other player


	if (GAME.IsMultiPlayer() &&
		GAME.GetIVar(IV_LEVEL_TYPE)==LT_RACER &&
		GAME.IsGameRunning())
	{
		CPrimary* apponent = PLAYER(0)->GetPrimary() ;

		if (apponent)
		{
			// arse,  we are player 0
			if (this == apponent)
			{
				apponent = PLAYER(1)->GetPrimary() ;
			}

			if (apponent->GetState() ==CS_NORMAL &&
				apponent->GetLevelData().GetCurrentRacePosition() < mLevelData.GetCurrentRacePosition())
			{
				mRage+=G(0,100) ;
				if (mRage > G1) mRage = G1;
			}
		}
	}


	//mAnimMode != AM_SPIN_ATTACK &&

	if ((mFlags & TF_ON_GROUND) &&
		 GetState() != CS_KNOCKED_OUT && // on ground?
		 mAnimMode != AM_SPIN_ATTACK &&
		(CONTROLS.IsButtonPressed(BUTTON_BRAKE, mPlayer->mNumber) /*||
		 CONTROLS.IsButtonPressed(BUTTON_BLOCK, mPlayer->mNumber)*/ ) &&	// brake pressed?
		(MAP.GetCell(CCellRef(mPos))->GetType() != CT_ICE) &&			// not ice
		(MAP.GetCell(CCellRef(mPos))->GetFriction() != FT_ICE) &&
		(WORLD.GetPrimaryThing(mPlayer->mNumber) == this) &&			// am I a primary?
		(GAME.GetIVar(IV_LEVEL_TYPE) != LT_GOLF) /*&&							// not playing golf
		(GAME.GetIVar(IV_LEVEL_TYPE) != LT_RACER) */)							// not playing race levels
	{
		// evaluate how much power we need to brake.
		GINT	p = mVelocity.MagnitudeXY() * BRAKE_UNIT_COST;

		if (p>G0 && mBrakePower > G0)
		{
			// can we afford it?
			GINT	brake_friction;
			if (p < mBrakePower)
			{
				// yup
				mBrakePower -= p;
				brake_friction = BRAKE_FRICTION;
			}
			else
			{
				// nope - just brake a bit.
				ASSERT(p > G0); // shouldn't be - otherwise conditional above would fail...
				GINT frac = mBrakePower / p;
				brake_friction = G1 - ((G1 - BRAKE_FRICTION) * frac);
				mBrakePower = G0;
			}
			mVelocity *= brake_friction;
		}
	}

	if (mIsBlocking || mAnimMode == AM_SPIN_ATTACK)
	{
		mBrakePower-=G(0,100) ;
		if (mBrakePower < G0)
		{
			mBrakePower = G0 ;
		}
	}

	if (mBrakePower==G0 && mIsBlocking)
	{
		StopBlocking() ;
	}

	// Golf levels have more friction, too

	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF)
	{
/*
		if ((mFlags & TF_ON_GROUND) &&								// on ground?
		(MAP.GetCell(CCellRef(mPos))->GetType() != CT_ICE))  // not ice
			mVelocity *= G(0, 58500);
*/
		// if staionary on golf level, stop.

		// GOLFNEW
		if (mPlayer)
		{
			if (GAME.GolfIsStationary(mPlayer->mNumber) && GAME.GolfGetShotNo(mPlayer->mNumber) != 1)
			{
				mVelocity = ZERO_GVECTOR;
				mFlags |= TF_NOT_ALLOWED_TO_MOVE ;
			}
			else
			{
				mFlags &= (~TF_NOT_ALLOWED_TO_MOVE) ;
			}
		}
	}

	// jump processing.
	if (mFlags & TF_ON_GROUND)
	{
		// reset camera if necessary
//		if (mLastFrameOnGround != GAME.GetGameTurn() - 1)
		{
			if (mVelocity.Z > -G(0, 2000)) // sometimes hits on the way up...
				if (mPlayer)
					GAME.UnlockCamera(mPlayer->mNumber);
		}
	}
	else
	{
		// if in the air for other reasons, trigger animation
//		if ((GAME.GetGameTurn() - mLastFrameOnGround > 20) && (!(IsAnimJumping(mAnimMode))))
		if ((GAME.GetGameTurn() - mLastFrameOnGround > 20) &&
			((!(IsAnimJumping(mAnimMode))) &&
			 (!(IsAnimBusy(mAnimMode)))))
			SetAnimMode(AM_JUMP);
	}


	if (IsStrikingForward())
	{
		CCharacter* char_near_me = NULL ;
		GINT smallest_ang_diff = G(1,30314);
		GINT angle_me_to_them = G0 ;
		CThing* a_thing_near_by_in_map_who  = FindFirstCollision(mPos, G(3)) ;
		while (a_thing_near_by_in_map_who )
		{
			CCharacter* a_char_near_by_in_map_who = a_thing_near_by_in_map_who->IsCharacter() ;

			if ( a_char_near_by_in_map_who && 
				 a_char_near_by_in_map_who->GetState() != CS_DEAD && 
				 (a_char_near_by_in_map_who->GetPos() - mPos).MagnitudeSq() < G(9)  )
			{

				// get angle
				GVector m_to_them = a_char_near_by_in_map_who->GetPos() - mPos ;
				GINT angle =-GATAN2(m_to_them.X, m_to_them.Y) ;
				GINT a_diff = GABS(AngleDifference(mYaw, angle)) ;
				if (a_diff < smallest_ang_diff)
				{
				//	char temp[50] ;
				//	sprintf(temp," ang diff = %2.2f", GToF(a_diff)) ;
				//	CONSOLE.AddString(temp) ;
					angle_me_to_them = angle ;
					smallest_ang_diff = a_diff;
					char_near_me = a_char_near_by_in_map_who ;
				}
			}
			a_thing_near_by_in_map_who = FindNextCollision() ;
		}

		if (char_near_me != NULL)
		{
			mDestYaw = angle_me_to_them ;
			// hack to stop you spinning the long way round !!
			if (mDestYaw > G_PI_2 && mYaw < -G_PI_2) mDestYaw-=(G_PI*2) ;
			if (mDestYaw < -G_PI_2 && mYaw > G_PI_2) mDestYaw+=(G_PI*2) ;
		}
	}


	SUPERTYPE::Process();

	// hack .. if we have fucked up the start of a race make ball roll a bit more..

	if (mFuckedUpAtStartOfRaceCount!=0)
	{
		GVector current_racer_spin_vector = GetRacerBoostVector() ;
		GVector ax;
		GVector::Cross3(mCurrentMapNormal,current_racer_spin_vector, ax);
		SetSpin(ax);
		mOrientation = mAngularVelocity * mOrientation;
		mOrientation.Normalise();
	//	SetRacerBoostVector(current_racer_spin_vector * G(0,64000) ) ;
		mFuckedUpAtStartOfRaceCount--;
		if (mFuckedUpAtStartOfRaceCount==0)
		{
			mRacerBoostVector = ZERO_GVECTOR ;
		}

		// create more smoke!

		CParticle *ptcl=GetRenderParticleSystem()->CreateParticle(PART_SMOKE);
		if(ptcl)
		{
			GINT	baserad = GATAN2(current_racer_spin_vector.Y, current_racer_spin_vector.X) + G_PI;  // guaranteeed not 0/0...

			ptcl->mPos=mPos;
			ptcl->mPos.Z += Radius();
			ptcl->mRadius=128;

			int 	r = (((G(rand() % 4095) * G(0,16384))) + (baserad / (G_PI * 2) * 4096) - (G(0,16384) * G(4096) / 2)).Whole();
			SINT	rad = (rand() % 7) + 8;
			ptcl->mVelocity.X.mVal=((rcos(r) * rad) >> (6 + 4))<<8;
			ptcl->mVelocity.Y.mVal=((rsin(r) * rad) >> (6 + 4))<<8;
			ptcl->mVelocity.Z.mVal=-0x20<<8;
		}
	}

	// racer boost

	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER &&
		mFuckedUpAtStartOfRaceCount == 0 &&
		mRacerBoostVector != ZERO_GVECTOR &&
		GAME.IsGameRunning())
	{
#define BOOST_MIN_SPEED G(0,20384)
#define BOOST_MAX_SPEED G(0,26000)
		GINT mag = mRacerBoostVector.Magnitude() ;
		if (mag > BOOST_MIN_SPEED && mag < BOOST_MAX_SPEED)
		{
			mVelocity = mRacerBoostVector *2;
			mRacerBoostVector = ZERO_GVECTOR ;
			DeclareBoost();
		}
		else if (mag >= BOOST_MAX_SPEED)
		{
			//CONSOLE.AddString("TTOOOOOOOOOOOOOO MUUCH ") ;
			mFuckedUpAtStartOfRaceCount = 24 ;
			mVelocity = mRacerBoostVector ;
			mVelocity.Normalise() ;
			mVelocity*=G(0,6000);
		}
		else
		{
			mRacerBoostVector.Normalise() ;
			mRacerBoostVector*=G(0,16000) ;
			mVelocity = mRacerBoostVector ;
			mRacerBoostVector = ZERO_GVECTOR ;
		}


	}

	//***********************************************************
	// trick stuff

	if (mFlags & TF_ON_GROUND)
		mTrickYaw = mYaw;

	if (GABS(mYaw - mTrickYaw) >= G_PI * 2)
	{
		CONSOLE.AddString("360!!!");
		mTrickYaw = mYaw;
	}


}



//#define DIRECT_JUMP_POWER_TO	(-G(0, 24000))
//#define DIRECT_JUMP_POWER_TO	(-G(0, 22000))
#define DIRECT_JUMP_POWER_TO	(-G(0, 18000))


//******************************************************************************************
// as we are primary do this for jump rather than generic part from character

void	CPrimary::Jump()
{
	if (CanIJump())
	{
		GVector norm = MAP.GetMapNormal(mLastFrameOnGroundPos);
		GINT	nz = norm.Z;
		
		BOOL	is_takeoff = MAP.GetCell(CCellRef(mLastFrameOnGroundPos))->IsTakeOff();
		
		// HACK - is this a take off or is it a ramp?
		
		if (is_takeoff)
		{
			if (nz < -G(0, 40000))
			{
				// not steep enough - so ramp...
				nz = ((G1 + nz) / 2) - G1; // make it possible to jump...
				is_takeoff = FALSE;
			}
		}

		if (is_takeoff)
		{
			// set Z velocity
			mVelocity.Z += DIRECT_JUMP_POWER_TO;

			// clip x & y velocity
			norm.Z = G0;
			norm.Normalise();
			mVelocity.X = mVelocity.X * (G1 - GABS(norm.X));// * G(0, 20000);
			mVelocity.Y = mVelocity.Y * (G1 - GABS(norm.Y));// * G(0, 20000);
			if (mPlayer)
				GAME.LockCamera(mPlayer->mNumber);

			mLastFrameJumped = GAME.GetGameTurn(); // prevent "double" jump
			SetAnimMode(AM_JUMP);
			SOUND.PlaySample(GetJumpSample(), this, G(0, 35000));
			mTrickYaw = mYaw;
		}
		else
		{
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
				SOUND.PlaySample(GetJumpSample(), this, G(0, 35000));
				mTrickYaw = mYaw;
			}
		}
	}
}



//******************************************************************************************
void	CPrimary::ApplyPickup(EPickupType pt, GINT amount)
{
	switch(pt)
	{
	case PUP_BRAKE:
		mBrakePower += amount;
		if (mBrakePower > G1)
			mBrakePower = G1;
		break;

	case PUP_HEALTH:
		{
			SINT	 i = InitialLife();
			mLife += (G(i) * amount).Whole();
			if (mLife > i)
				mLife = i;
		}
		break;
	case PUP_TOKEN:
		{
			GetLevelData().PickedUpToken() ;
			break ;
		}
	}
}

//******************************************************************************************
void	CPrimary::HitArena()
{
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF)
	{
		// OB
		mLife = InitialLife();
		mVelocity = ZERO_GVECTOR;

		if (mPlayer)
		{
			Move(GAME.GolfGetLastShotPosition(mPlayer->mNumber));
			GAME.GolfIncreaseShotNumber(mPlayer->mNumber);
			GAME.GolfDeclareStationary(mPlayer->mNumber);
			GAME.GolfDeclareOB(mPlayer->mNumber);
		}
	}
}

//******************************************************************************************
void	CPrimary::StartDieProcess(BOOL hit_fan)
{
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF)
	{
		// don't die!
		// instead - move back to where I started
		mFlags &= (~TF_DEAD);
		mDFlags&=~DFC_Inwater;

		mLife = InitialLife();
		mState = CS_NORMAL;
		mVelocity = ZERO_GVECTOR;

		if (mPlayer)
		{
			Move(GAME.GolfGetLastShotPosition(mPlayer->mNumber));
			GAME.GolfIncreaseShotNumber(mPlayer->mNumber);
			GAME.GolfDeclareStationary(mPlayer->mNumber);
		}
	}
	else
	{
		if (mState != CS_DEAD)
		{
			//!YYYYYYAAAAAAAAAARGhhh - this if carries through!

			// change game state if I'm the primary thing
			if (WORLD.GetPrimaryThing(mPlayer->mNumber) == this)
	//			GAME.GSDeclarePrimaryDead(this);
	//			GAME.ChangeGameState(GAME_STATE_PRIMARY_DEAD);

			SUPERTYPE::StartDieProcess(hit_fan);

			// Play Sample
			SINT 	id = _GetClassID();
			SINT	s = -1;
			switch (id)
			{
			case OID_CPrimaryAngel: 	s = SOUND_1DEATH; break;
			case OID_CPrimaryApostle: 	s = SOUND_2DEATH; break;
			case OID_CPrimaryBenny: 	s = SOUND_3DEATH; break;
			case OID_CPrimaryDoctor: 	s = SOUND_4DEATH; break;
			case OID_CPrimaryLockdown: 	s = SOUND_5DEATH; break;
			case OID_CPrimarySophie: 	s = SOUND_6DEATH; break;
			};
			if (s != -1)
				SOUND.PlaySample(s, this);

		}
	}
}





//******************************************************************************************
BOOL	CPrimary::Attack(SINT no)
{
	//! use number!

	if (no == 1 && GAME.GetIVar(IV_LEVEL_TYPE)==LT_RACER)
	{
		if (mRage!=G1) return FALSE;
		if (!SUPERTYPE::Attack(no)) return FALSE ;
	
		GVector dd = mVelocity ;
		dd.Normalise() ;
		dd*=G(1) ;
		mVelocity=dd ;
		mRage = G0 ;
		DeclareBoost() ;
		return FALSE;
	}

	if (!SUPERTYPE::Attack(no)) return FALSE ;

	if (mRage == G1 && no==1)
	{
		SetAnimMode(AM_RAGE_ATTACK);
		mCurrentPunchSequenceNum= 0 ;
		CThing *t = SpawnThing(OID_CFXRageAttack);
		if (t)
		{
			CInitThing i;
			i.mPos = mPos;
			((CFXRageAttack*)t)->SetOwner(this);
			t->Init(&i);
		}
		return TRUE ;
	}
	else
	{
		return MainCharacterAttack(no) ;
	}

}

//******************************************************************************************
GINT	CPrimary::GroundFriction(EFrictionType friction_type)
{

	if (mState == CS_FINISHED)
	{
		return G(0,55000) ;
	}
	else
	{
		 GINT friction = SUPERTYPE::GroundFriction(friction_type);
		 if (mAnimMode == AM_CHARGE) friction*=G(0,62000) ;
		 return friction ;
	}
}



//******************************************************************************************
void	CPrimary::FinishedLevel()
{
	if (mLevelData.GetFinishedLevel() == TRUE) return ;

	SUPERTYPE::FinishedLevel() ;

	if (mPlayer->GetLevelResult() == LR_WIN)
	{
		SetAnimMode(AM_VICTORY) ;
	}

}

#define COS_45_DEGS G(0,46340)

//******************************************************************************************
void	CPrimary::RacerBoost(GVector v)
{
	// ok to stop a faster boost when moving diagonal limit the magnitude of
	// the input vector to either v->x or v->y depending on which is bigger

	GINT new_mag = GABS(v.X) > GABS(v.Y) ? GABS(v.X) : GABS(v.Y) ;
	v.Normalise() ;
	v*=new_mag ;

	//make sure we are boosting where we are heaeding +- 45 degs
	GVector dir = GVector(GSIN(-mYaw), GCOS(-mYaw),G0) ;
	GVector v1 = v;
	v1.Normalise() ;
	GINT dot = v1 * dir ;
	if (dot < COS_45_DEGS ) return ;

	v*=2 ;
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


//******************************************************************************************
void	CPrimary::Thrust(const GVector& v)
{
   if (mState == CS_STUNNED) return ;
   if (mFuckedUpAtStartOfRaceCount==0)
   {
		 SUPERTYPE::Thrust(v) ;
   }
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CPrimaryBenny::Process()
{
	if (mAnimMode == AM_PUNCH || mAnimMode == AM_PUNCH_HIT)
	{
		mDFlags&= (~DFC_DrawTrail) ;
	}
	else
	{
		mDFlags|= DFC_DrawTrail ;
	}
	SUPERTYPE::Process() ;
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CPrimaryDoctor::Process()
{
	if (mAnimMode == AM_PUNCH || mAnimMode == AM_PUNCH_HIT)
	{
		mDFlags&= (~DFC_DrawTrail) ;
	}
	else
	{
		mDFlags|= DFC_DrawTrail ;
	}
	SUPERTYPE::Process() ;
}




CCharacterCombatProfile AngelCombatProfile = { 100,4,6,25,20,14,10,6,3 } ;
CCharacterCombatProfile ApostleCombatProfile = { 100,3,8,25,20,12,12,6,3 };
CCharacterCombatProfile LockdownCombatProfile = { 100,2,12, 25,20,11,13,6,3 };
CCharacterCombatProfile BennyCombatProfile = { 100,2,12,25,20,9,15,6,3 };
CCharacterCombatProfile SophieCombatProfile = { 100,3,8,25,20,13,11,6,3 };
CCharacterCombatProfile DoctorCombatProfile = { 100,1,12,30,0,12,12,6,3 } ;
CCharacterCombatProfile PrimaryBallCombatProfile = { 100,0,0,0,0,0,0,0,0 } ;

