#include	"Common.h"

#include	"CharBall.h"
#include	"Globals.h"
#include	"Strike.h"
#include	"Torso.h"
#include	"HoldableThing.h"
#include	"effect.h"
#include	"Crown.h"
#include	"Profile.h"
#include	"Primary.h"
#include	"Weapon.h"
#include	"badguys.h"
#include	"start.h"
#include	"DroidBallPot.h"

#define	CHARBALL_DIE_TILT	 GINT(1, 40000)
#define	CHARBALL_DIE_ACCEL GINT(0, 500)
#define	CHARBALL_DIE_DAMP  GINT(0, 26214)

#define SUSPENSION_STRENGTH1	(G(6))
#define SUSPENSION_STRENGTH2	(G(2))
#define SUSPENSION_RANGE1		(G1)
#define SUSPENSION_RANGE2		(-G(2))
#define SUSPENSION_DAMPING		(G(0, 50000))
#define SUSPENSION_BALL_WEIGHT	(G(0, 2000))


#define S_TO_G G(2,32000)

//******************************************************************************************
void	CCharBall::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mOrientation2 = mOrientation;
	mDir = mOrientation * GVector(G0, G1, G0);
	SitOnGround();

	mPitchVelocity = G0;

	mSuspension = G0;
	mSuspensionVelocity = G0;

	mDestYaw	= mYaw;
	mPitch		= G0;
	mDestPitch	= G0;
	mRoll		= G0;
	mDestRoll	= G0;

	mDestPitchA = G0;
	mDestPitchL = G0;
	mGroundHeightBelowHead = G0 ;

	mTrickYaw = mYaw; // for tricks

	mSplit = FALSE;
	mHitSomethingDuringSpinAttack = FALSE ;

	mSpinVelocityWhenStruck  = G(0,42343) ;
	mHowMuchILeanForwardWhenRunning = G(20) ;
	mIceBallSpinVector = GVector(G0,G0,G0) ;
	mRacerBoostVector = GVector(G0,G0,G0) ;

	mLastOrientation2 = mOrientation2;
}

//******************************************************************************************
void	CCharBall::AddStrikeVelocity(GVector vel, GINT term_strike_vel)
{
	GINT vel1 = mVelocity.MagnitudeSq() ;
	if (vel1 > (term_strike_vel*term_strike_vel) ) return ;

	AddVelocity(vel) ;
}


//******************************************************************************************
void	CCharBall::Process()
{
	mLastOrientation2 = mOrientation2;

	// SRG speed up only needs to calc map normal once
	SINT game_turn = GAME.GetGameTurn() ;
	if (mCurrentMapNormalCalcTime != game_turn )
	{
		mCurrentMapNormalCalcTime = game_turn ;
		mCurrentMapNormal = MAP.GetMapNormal(mPos) ;
		if (!GAME.IsGameRunning() )
		{
			mCurrentMapNormal = GVector(G0,G0,-G1) ;
		}
	}

	if (mAnimMode != AM_SPIN_ATTACK) mHitSomethingDuringSpinAttack = FALSE ;

	// ****************************
	// ****************************
	// ****************************

	// handle rotation

	// ****************************

	// yaw


	// ok if we are throwing and we are close to droid ball pot then face it (helps the user
	// a bit )
	if (mAnimMode == AM_THROW && mFrame < GINT_HALF)
	{
		GINT yaw_you_to_pot;
		if (CDroidBallPot::CloseToDroidBallPot(this, &yaw_you_to_pot) )
		{
			if (GABS(AngleDifference(yaw_you_to_pot, mYaw)) < G_PI_2)
			{
				mDestYaw = yaw_you_to_pot ;
				// hack to stop you spinning the long way round !!
				if (mDestYaw > G_PI_2 && mYaw < -G_PI_2) mDestYaw-=(G_PI*2) ;
				if (mDestYaw < -G_PI_2 && mYaw > G_PI_2) mDestYaw+=(G_PI*2) ;
			}
		}
	}

	GINT	delta_yaw = mDestYaw - mYaw;

	//	while (delta_yaw > G_PI)
	//		delta_yaw -= G_PI * 2;
	//
	//	while (delta_yaw < -G_PI)
	//		delta_yaw += G_PI * 2;


	GINT	delta_yaw2 = delta_yaw;
	mYaw += delta_yaw2 * G(0, 10000);
//	mYaw += delta_yaw2 * G(0, 5000);

	if (mYaw > G_PI)
	{
		mYaw		-= G_PI * 2;
		mDestYaw	-= G_PI * 2;
		mTrickYaw	-= G_PI * 2;
	}
	if (mYaw < -G_PI)
	{
		mYaw		+= G_PI * 2;
		mDestYaw	+= G_PI * 2;
		mTrickYaw	+= G_PI * 2;
	}



	// if not knocked out or dead then update pitch value.
	// Note: if it is knocked out or dead then the state function
	// takes over pitch control.

	// only needs to calc this once

	BOOL is_fighting = FALSE ;
	if (IsBadGuy())
	{
		is_fighting = ((CBadGuy*)this)->IsCurrentActionAttackingTarget() ;
	}

	if (GAME.CanIBeSeen(mPos) || (!IsUpright()) || is_fighting)
	{
		PROFILE_START(Character);

		GMatrix m;
		m.MakeRotationYaw(mYaw);
		GVector	v = m * GVector(G0, G1, G0);
		GVector cross_of_normal_and_current_yaw;
		GVector::Cross3(v, mCurrentMapNormal, cross_of_normal_and_current_yaw);

		if ( mState != CS_DEAD && mState !=CS_KNOCKED_OUT)
		{

			// ****************************
			// Pitch
			// do a bit of processing first
			if ((mFlags & TF_ON_GROUND) && (!(mFlags & TF_ON_OBJECT)))
			{
				GVector p;
				GVector::Cross3(cross_of_normal_and_current_yaw, mCurrentMapNormal, p);
				p.Normalise();
				mDestPitchL = -p.Z;
			}

			mDestPitch = mDestPitchA + mDestPitchL;

			GINT	delta_pitch = mDestPitch - mPitch;

			while (delta_pitch > G_PI)
				delta_pitch -= G_PI * 2;

			while (delta_pitch < -G_PI)
				delta_pitch += G_PI * 2;

			GINT	delta_pitch2 = delta_pitch;
			mPitch += delta_pitch2 * G(0, 5000);


		}

		if (mPitch > G_PI)
			mPitch -= G_PI * 2;
		if (mPitch < -G_PI)
			mPitch += G_PI * 2;


		// ****************************
		// Roll

		// do a bit of processing first
		if ((mFlags & TF_ON_GROUND) && (!(mFlags & TF_ON_OBJECT)))
		{
			GVector p = cross_of_normal_and_current_yaw ;
			p.Normalise();
			mDestRoll = p.Z;
		}


		GINT	delta_roll = mDestRoll - mRoll;

		while (delta_roll > G_PI)
				delta_roll -= G_PI * 2;

		while (delta_roll < -G_PI)
				delta_roll += G_PI * 2;

		GINT	delta_roll2 = delta_roll;
		mRoll += delta_roll2 * G(0, 5000);

		if (mRoll > G_PI)
			mRoll -= G_PI * 2;
		if (mRoll < -G_PI)
			mRoll += G_PI * 2;


		// and build the matrices
	/*	GMatrix foo;

		// pitch
		mOrientation2.MakeRotationPitch(mPitch);

		// roll
		foo.MakeRotationRoll(mRoll);
		mOrientation2 = foo * mOrientation2;

		// yaw
		foo.MakeRotationYaw(mYaw);
		mOrientation2 = foo * mOrientation2;*/
		mOrientation2.MakeRotationYRP(mYaw, mRoll, mPitch);

		mDestPitchA = mDestPitchA * G(0, 60000);
		mDestPitchL = mDestPitchL * G(0, 60000);
		mDestRoll  = mDestRoll * G(0, 60000);

		PROFILE_END(Character);
	}

	//***********************

	SUPERTYPE::Process();

	//***********************

	PROFILE_START(BadGuy);
	if (GAME.CanIBeSeen(mPos))
	{
		Roll();
	}
	PROFILE_END(BadGuy);

	// process suspension
	mSuspension += mSuspensionVelocity;

	if (mSuspension > SUSPENSION_RANGE1)
	{
		mSuspension = SUSPENSION_RANGE1;
		mSuspensionVelocity = G0;
	}

	if (mSuspension < SUSPENSION_RANGE2)
	{
		mSuspension = SUSPENSION_RANGE2;
		mSuspensionVelocity = G0;
	}


	mSuspensionVelocity *= SUSPENSION_DAMPING;

	if (!(mFlags & TF_ON_GROUND))
		mSuspensionVelocity -= SUSPENSION_BALL_WEIGHT;

	mSuspensionVelocity -= mSuspension / SUSPENSION_STRENGTH1;

	// move flames if we are on fire
	if (mFlags & TF_ON_FIRE)
	{
		// are we in water
		if (mDFlags&DFC_Inwater)
		{
			mFlameGoOutTime = GAME.GetGameTurn() ;
			mFlags&=~TF_ON_FIRE ;
		}
		else
		{
			GVector pos = mPos  ;
			if (mOnFireFlames[0])
				mOnFireFlames[0]->MoveStreamerAndParticles(&pos);

			if (!mSplit)
			{
				pos =  mPos + (mOrientation2 * GVector(G0, G0, -(Radius() + Radius2()+G(1)))); ;
				if (mOnFireFlames[1])
					mOnFireFlames[1]->MoveStreamerAndParticles(&pos);
			}

			SINT game_time = GAME.GetGameTurn() ;
			if ((game_time & 3) == 0 && game_time < (mFlameGoOutTime ))
			{
				mLife-- ;
			}
		}

		// a really nasty hack..  To make flames go out after a while
		if (GAME.GetGameTurn() == mFlameGoOutTime)
		{
			// don't set mOnFireFlames to null !! let the validate  thing do that.
			// so we can still moves the partlces in the flame during die process
			if (mOnFireFlames[0]) mOnFireFlames[0]->Die() ;
			if (!mSplit && mOnFireFlames[1])
			{
				mOnFireFlames[1]->Die() ;
			}
		}

		if (	mOnFireFlames[0]== NULL )	mFlags&=~TF_ON_FIRE ;
	}

	// r we wearing a crown
	if (mCrown)
	{
		mCrown->SetOrientation(mOrientation2) ;
		mCrown->Move(mPos+ GetRenderThingEmitterAnimated(5) ) ;
	}
}


//******************************************************************************************
void	CCharBall::CatchOnFire()
{
    if ((mFlags & TF_ON_FIRE) &&
		mOnFireFlames[0] &&
		mOnFireFlames[0]->GetLife() == mOnFireFlames[0]->InitialLife() )
	{
		mFlameGoOutTime = GAME.GetGameTurn() + 50 ;

		if (mOnFireFlames[0]) mOnFireFlames[0]->SetParticleFrequency(5) ;
		if (mOnFireFlames[1]) mOnFireFlames[1]->SetParticleFrequency(5) ;
		return ;
	}

	// ok we are not on fire or we already told our last body flamer to die
	// so create a new set of body flamers
	mFlameGoOutTime = GAME.GetGameTurn() + 50 ;
	mFlags |= TF_ON_FIRE ;

	mOnFireFlames[0] = (CFXBodyFlamer*)SpawnThing(OID_CFXBodyFlamer);

	if (mOnFireFlames[0])
	{
		CInitThing	i;

		i.mPos = mPos  ;
		i.mOrientation.MakeRotationPitch(-G_PI_2);
		i.mAngleType = AT_ORI;
		mOnFireFlames[0]->Init(&i);
		mOnFireFlames[0]->SetXRandomStartOffset(G(0,16123)) ;
		mOnFireFlames[0]->SetYRandomStartOffset(G(0,16123)) ;
		mOnFireFlames[0]->SetZRandomStartOffset(G(0,16123)) ;
	}
	if (GetCollideShape() == CSH_DOUBLE_SPHERE)
	{
		mOnFireFlames[1] = (CFXBodyFlamer*)SpawnThing(OID_CFXBodyFlamer);

		if (mOnFireFlames[1])
		{
			CInitThing	i;
			i.mPos = GetPos(1) ;
			i.mOrientation.MakeRotationPitch(-G_PI_2);
			i.mAngleType = AT_ORI;
			mOnFireFlames[1]->Init(&i);
			mOnFireFlames[1]->SetXRandomStartOffset(G(0,16123)) ;
			mOnFireFlames[1]->SetYRandomStartOffset(G(0,16123)) ;
			mOnFireFlames[1]->SetZRandomStartOffset(G(0,16123)) ;
		}
	}
}


//******************************************************************************************
void	CCharBall::Thrust(const GVector &v)
{
	// ok - try and work out some sensible parameters here.

	if ((GABS(v.X) > G(0, 10)) || (GABS(v.Y) > G(0, 10)))
	{
		// yaw - face direction of travel
		GINT	ndy = GATAN2(v.Y, v.X) - G_PI_2;

		if (mDestYaw - ndy >  G_PI)
			ndy += G_PI * 2;
		if (mDestYaw - ndy < -G_PI)
			ndy -= G_PI * 2;

		if (mAnimMode != AM_RAGE_ATTACK &&
			mAnimMode != AM_SPIN_ATTACK)
		{
			mDestYaw = ndy;
		}

		// Pitch
		GVector dir = GVector(GSIN(-mYaw),GCOS(-mYaw),G0) ;
		GINT	dot = dir * v;

//		mDestPitchA = dot * 30;	   // play with this
		mDestPitchA = dot * mHowMuchILeanForwardWhenRunning;
	}

	// and do the thrust...
	CMapCell *cell = MAP.GetCell(CCellRef(mPos));
	if ((mFlags & TF_ON_GROUND) && (cell->GetFriction() == FT_ICE))  // ice??
	{
		AddVelocity(v /6);  //!Check this value!

		if (mIceBallSpinVector == GVector(G0,G0,G0) ) mIceBallSpinVector = mVelocity ;
		mIceBallSpinVector+=(v) ;

	    if (mIceBallSpinVector.Magnitude() > G(0,12234) )
		{
			mIceBallSpinVector.Normalise() ;
			mIceBallSpinVector*=G(0,12234) ;
		}
	}
	else
	{
		AddVelocity(v);
	}


	// (JCL) OK - take #2 - try and vary force dependant on current speed.
#define MAX_SPEED		1
#define MAX_SPEED_AIR	G(0, 55000);

/*	GVector v2 = v * G(2);

	GINT	speed	= mVelocity.Magnitude();
	if (speed > G(0, 100))
	{
		GINT	cap;

		if (mFlags & TF_ON_GROUND)
			cap = speed / MAX_SPEED;
		else
			cap = speed / MAX_SPEED_AIR;
		GINT	dot		= (v2 * mVelocity) / speed;
		GINT	sub		= G0;

		if (cap > G1)
			cap = G1;

		if (dot > G0)
			sub = (dot * cap) / speed;

		AddVelocity(v2 - (mVelocity * sub));
	}
	else
		AddVelocity(v2);
*/
}

//******************************************************************************************
BOOL	CCharBall::Strike(const CStrike& strike)
{
	ECharState	StateBeforeBeingStruck = mState;

	BOOL result = SUPERTYPE::Strike(strike);

	if (!result) return FALSE ;

	if (!strike.mWillKnockOut && strike.mStrikedWith == SI_SPIN_PUNCH && mState == CS_STUNNED)
	{
		mSpinVelocity = mSpinVelocityWhenStruck ;
	}
	else
	{
		mSpinVelocity = G(0) ;
	}

	if (strike.mWillKnockOut && StateBeforeBeingStruck != CS_KNOCKED_OUT  )
	{

		GINT angle_diff = AngleDifference(mYaw, strike.mOriginator->GetYaw() ) ;

		if (GABS(angle_diff) > G_PI_2  )
		{
			mPitchTo = -CHARBALL_DIE_TILT ;
			mPitchVelocity = -G(0,8192);
		}
		else
		{
			mPitchTo = CHARBALL_DIE_TILT ;
			mPitchVelocity = G(0,8192);
		}

		ChangeState(CS_KNOCKED_OUT, CStateInit(0));
		mRecoverFromKnockoutTime = CHAR_KNOCK_OUT_TIME ;
		if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER) mRecoverFromKnockoutTime = mRecoverFromKnockoutTime >> 1 ;

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
				SOUND.PlaySample(s, this);
		}
	}
	return result ;
}



//******************************************************************************************
void	CCharBall::StartDieProcess(BOOL hit_fan)
{
	if (mState != CS_DEAD)
	{
		// Crowd
		if (IsPrimary())
			GAME.CrowdEvent(-G(2), G1);
		else
			GAME.CrowdEvent(G(0, 40000), GINT_HALF);

		ChangeState(CS_DEAD, CStateInit(0));

		if (mHoldingThing) Drop() ;

//		mPitchVelocity = G0;
//		mPitchTo = (mPitch > G0) ? (-CHARBALL_DIE_TILT) : CHARBALL_DIE_TILT;
		mSplit = TRUE;

		mVelocity *= GINT_HALF;

		SINT	id = GetTorsoID();
		if (id != OID_NONE)
		{
			CThing *t = SpawnThing(id);
			{
				if (t)
				{
					CInitThing	i;

					// calc x,y pos
					BOOL spin = FALSE ;
					GVector f(G0,G0,-G(2,16123)) ;
					f= mOrientation2  * f ;
					f+=mPos;
					i.mPos = f;
					i.mAngleType = AT_YPR;
					i.mYaw = mYaw;
					i.mPitch = mPitch;
					i.mRoll = mRoll;

//					i.mVelocity = GVector(G0, G0, G0);

					if (!hit_fan)
					{
						i.mVelocity = mVelocity * 2;
					}
					else
					{
						i.mVelocity.X= G(0, (RANDOM()%40000) - 20000) ;
						i.mVelocity.Y= G(0, (RANDOM()%40000) - 20000) ;
						i.mVelocity.Z= -G(0, 28000) ;
						spin = TRUE ;
						CThing	*t1 = SpawnThing(OID_CFXExplosion2);
						if (t1)
						{
							CInitThing i;
							i.mPos = mPos;
							t1->Init(&i);
							((CFXExplosion2*)t1)->SetRipple(FALSE) ;
						}
					}


					t->Init(&i);
					if ((mFlags & TF_ON_FIRE) && mOnFireFlames[1] )
					{
						// pass ownership to torso
						((CTorso*)t)->SetBodyFlamer(mOnFireFlames[1]) ;
						mOnFireFlames[1] = NULL ;
					}
					((CTorso*)t)->SetSpin(spin) ;
				}
			}
		}

		SUPERTYPE::StartDieProcess(hit_fan) ;
	}
}



//******************************************************************************************
SINT	CCharBall::GetRenderThingMiscFlags()
{
	SINT res = SUPERTYPE::GetRenderThingMiscFlags() ;

	if (mState == CS_DEAD && mStateTimer > 100 && ((GAME.GetGameTurn() % 2) == 1))
	{
		res |= RTMF_WHITE;
	}
	return res ;
}



//******************************************************************************************
void	CCharBall::StateDead()
{
	if (mInvisible && mStateTimer >=150)  return ;

		mStateTimer++;


	// re spawn on multiplayer
	if (GAME.IsMultiPlayer() &&
		mStateTimer == 60 &&
		GAME.GetIVar(IV_LEVEL_TYPE) != LT_PURSUIT &&
		GAME.GetIVar(IV_LEVEL_TYPE) != LT_LASTMAN &&
		IsPrimary())
	{
		if (mFlags & TF_ON_FIRE )
		{
			if (mOnFireFlames[0])
			{
				mOnFireFlames[0]->Die() ;
				mOnFireFlames[0] = NULL;
			}

			if (mOnFireFlames[1])
			{
				mOnFireFlames[1]->Die() ;
				mOnFireFlames[1] = NULL;
			}
			mFlags&= ~TF_ON_FIRE ;
		}
			mFlags &= (~TF_DEAD);

		Heal(10000) ;
		mState = CS_NORMAL;
		mVelocity = ZERO_GVECTOR;
		mSplit = FALSE ;
		SetAnimMode(AM_WALK);

		if (GAME.GetIVar(IV_LEVEL_TYPE) != LT_RACER)
		{
			if (!mInvisible)
			{
				CThing	*t = SpawnThing(OID_CFXExplosion2);
				if (t)
				{
					CInitThing i;
					i.mPos = mPos;
					t->Init(&i);
				}
			}
			Move(((CPrimary*)this)->GetSpawnedBy()->GetPos()) ;
			mYaw = ((CPrimary*)this)->GetSpawnedBy()->GetYaw();
			mDestYaw = mYaw ;
		}
		else
		{
			SINT last_cp_num = mLevelData.GetLastCheckPoint() ;

			// ok re spawn player on last checkpoint visited.

			// if we have not reached a checkpoint yet, re spawn at start
			if (last_cp_num <0)
			{
				Move(((CPrimary*)this)->GetSpawnedBy()->GetPos()) ;
				mYaw = ((CPrimary*)this)->GetSpawnedBy()->GetYaw();
				mDestYaw = mYaw ;
			}
			else
			{
				// respawn at last checkpoint
				CCheckPoint* cp = CCheckPoint::GetACheckPointFromNumber(last_cp_num) ;
				CRacerReSpawner* racer_respawn = (CRacerReSpawner*)cp->GetChainTrigger() ;
				Move(racer_respawn->GetPos()) ;
				mYaw = racer_respawn->GetYaw();
				mDestYaw = mYaw ;
			}
		}
		mPitch = G0 ;
		mRage = G0 ;
		return ;
	}

	if (mStateTimer == 150)
	{
		if (mFlags & TF_ON_FIRE )
		{
			if (mOnFireFlames[0])
			{
				mOnFireFlames[0]->Die() ;
				mOnFireFlames[0] = NULL;
			}

			if (mOnFireFlames[1])
			{
				mOnFireFlames[1]->Die() ;
				mOnFireFlames[1] = NULL;
			}
			mFlags&= ~TF_ON_FIRE ;
		}
		if (!mInvisible)
		{
			CThing	*t = SpawnThing(OID_CFXExplosion2);
			if (t)
			{
				CInitThing i;
				i.mPos = mPos;
				t->Init(&i);
			}
		}
		if (!IsPrimary() && !IsRacer())
		{
			Die() ;
		}
		else
		{
			mInvisible = TRUE ;
		}
	}
}


//******************************************************************************************
SINT	CCharBall::WhichRTsToRender()
{
	if (mInvisible) return 0 ;

	SINT v = CCharacter::WhichRTsToRender() | (mState == CS_DEAD ? 0 : 8);

	if ((v & 1) == 0)
		return 0; // teleport?
	return v;
}


//******************************************************************************************
void	CCharBall::ProcessMapCollision()
{

	SUPERTYPE::ProcessMapCollision() ;

	GVector head_pos = mPos + (mOrientation2 * GVector(G0, G0, -(Radius() + Radius2() + G(1) )));
//	GVector head_pos = GetPos(1) ;

	GINT map_height ;


	GINT	r = Radius2();
	BOOL	hit_arena = FALSE;
	GINT	diff ;


	//*********************************************
	// collide head with stadium edges
	if (head_pos.X - r < G0 )
	{
		diff = -(head_pos.X - r);
		hit_arena = TRUE ;
	}
	else if (head_pos.Y - r < G0)
	{
		diff= -(head_pos.Y - r);
		hit_arena = TRUE ;
	}
	else if (	head_pos.X + r > G(MAP_WIDTH - 1) )
	{
		diff = ((head_pos.X + r) - G(MAP_WIDTH - 1));
		hit_arena = TRUE ;
	}
	else if (	head_pos.Y + r > G(MAP_HEIGHT - 1) )
	{
		diff= ((head_pos.Y + r) - G(MAP_HEIGHT - 1));
		hit_arena = TRUE ;
	}

	if (hit_arena)
	{
		// nice hack hear to stop it shaking
		diff.mVal = diff.mVal &  0xFFFF8000;

		// assume staduim on a gadient of 1/2
		map_height =-(diff*2 ) ;
	}
	else
	{
		//*********************************************
		// collide head with heightfield
		map_height = MAP.GetMapHeight(head_pos) - r;
	}


	mGroundHeightBelowHead = map_height ;
	if (head_pos.Z > map_height)
	{
		GINT ty = mPos.Z - map_height ;
		GINT tx = (mPos-head_pos).MagnitudeXY() ;
		GINT new_pitch = GATAN2(ty,tx) ;
		if (mPitch < G0)
		{
			mPitch = (-(G_PI >>1)) +new_pitch ;
		}
		else
		{
			mPitch = ((G_PI >>1)) -new_pitch ;
		}
	}
}


//******************************************************************************************
void	CCharBall::StateKnockedOut()
{

	mPitchVelocity += (mPitchTo > G0) ? CHARBALL_DIE_ACCEL : - CHARBALL_DIE_ACCEL;
	mPitch += mPitchVelocity;
	BOOL head_hit_height_field = GetPos(1).Z > mGroundHeightBelowHead ;

	if (((mPitchTo > G0) && (mPitch > mPitchTo)) ||
	((mPitchTo < G0) && (mPitch < mPitchTo)) ||
	head_hit_height_field)
	{
		if (!head_hit_height_field)
		{
			mPitch = mPitchTo;
		}

		mPitchVelocity = -mPitchVelocity * CHARBALL_DIE_DAMP;
		GVector head_impact_point = mPos + (mOrientation2 * GVector(G0, G0, -((Radius() + Radius2())*2)));
		if (!COORDS_INVALID(head_impact_point) )
		{
			MAP.Raindrop(CCellRef(head_impact_point), GABS(mPitchVelocity) >> 1);
		}
	}

	// ok if knocked out and upright cos of height field make ball roll away a bit

    static GINT ang = (G_PI>>1) - (G_PI>>2) ;

	if (head_hit_height_field && (GABS(mPitch) <ang) )
	{
		GVector b_to_h = GetPos(1) - mPos ;
		b_to_h.Z = G0;
		b_to_h.Normalise() ;
		b_to_h *= G(0,3000) ;
		AddStrikeVelocity(-b_to_h, SMALL_COMBAT_MAX_VELOCITY*2) ;
	}

	SUPERTYPE::StateKnockedOut() ;
}


//******************************************************************************************
void	CCharBall::StateStunned()
{

	// dizzy...
	if (mSpinVelocity != G(0) )
	{
		mYaw += mSpinVelocity ;
		mSpinVelocity*=G(0,63000) ;

		if (mYaw > G_PI) mYaw-=(G_2PI) ;
		mDestYaw = mYaw ;
	}

	SUPERTYPE::StateStunned() ;
}

//******************************************************************************************
void	CCharBall::SpinAnimTrigger(const GMatrix& current_anim_orientation)
{
	GVector hv = current_anim_orientation * GVector(G0, G1, G0);
	GVector our_movement_after_strike = GVector(G0,G0,G0);
	GINT our_term_velocity = SMALL_COMBAT_MAX_VELOCITY ;
	GINT size_of_me_factor = SizeofMeFactor() ;
	hv.Z = G0;
	if (hv.MagnitudeSqXY() > G(0, 10))
	{
		hv /= hv.Magnitude();
		hv *= ( G(1, 32768) * size_of_me_factor );

		GVector g1 = mOrientation2 * GVector(G0, G0, -G(1)) ;

		GVector hp = hv + mPos + g1; // height
		CThing *t = FindFirstCollision(hp, (G(2) * size_of_me_factor));

		mLastPunchResult = PR_MISSED ;
		mLastPunchFrameNum = GAME.GetGameTurn() ;

		while (t)
		{
			CStrike this_strike ;
			this_strike.mWillKnockOut = FALSE ;
			this_strike.mStrikedWith = SI_SPIN_PUNCH;
			this_strike.mStrength = mCombatProfile.mSpinAttackStrength ;
			this_strike.mShoulderToFistDist = S_TO_G * size_of_me_factor ;
			this_strike.mOverwriteSpin = TRUE ;
			this_strike.mOriginator = this ;


			CCharacter *c = t->IsCharacter();
			if (c && (c->GetState()== CS_DEAD || c->GetState() == CS_KNOCKED_OUT || (!((CCharBall*)c)->IsUpright())  )  )
			{
				t = FindNextCollision();
				continue ;
			}

			GINT angle_me_to_apponent = mYaw ;
			if (c)
			{
				GVector a1 = c->GetPos() - mPos ;
				angle_me_to_apponent = -GATAN2(a1.X, a1.Y) ;
			}
			GVector	dir = t->GetPos() - mPos;
			dir.Normalise();
			this_strike.mAttackVelocity = dir * G(0, 24000);

			BOOL res = t->Strike(this_strike);
			if (t->IsStrikeable())
			{
				if (!mHitSomethingDuringSpinAttack)
				{
					CPrimary *pr = this->IsPrimary();

					if (pr && mAnimMode != AM_RAGE_ATTACK)
					{
						GINT	br = pr->GetBrakePower();
						if (br > G(0, 6553))
							pr->SubBrakePower(G(0, 6553));
						else
							pr->SubBrakePower(br);
					}

					mHitSomethingDuringSpinAttack = TRUE ;
				}
				our_movement_after_strike = -(this_strike.mAttackVelocity*G(0,16124));

				if (res==TRUE)
				{
					ScoreStrike(t) ;
					mLastPunchResult = PR_HIT ;
					// $$$ HIT EFFECT HERE
				//	GVector hp1 =  mPos+	GetRenderThingEmitterAnimated(1);
					mParticleSystem.MakeHitEffect(&hp, angle_me_to_apponent + G_PI,HT_NORMAL);
				}
				else
				{
					// $$$ BLOCK EFFECT HERE
						mLastPunchResult = PR_BLOCKED ;
				//	GVector hp1 = mPos+	GetRenderThingEmitterAnimated(1);
					mParticleSystem.MakeHitEffect(&hp, angle_me_to_apponent + G_PI,HT_BLOCK);
				}

			}

			t = FindNextCollision();

		};

	}
	AddStrikeVelocity(our_movement_after_strike, our_term_velocity) ;

	if (mLastPunchResult == PR_HIT)	SOUND.PlaySample(SOUND_PUNCH2, this);
}

//******************************************************************************************
void	CCharBall::PunchLeftRightTrigger(GVector& lr)
{
	GVector hv = mOrientation2 * lr;

	GINT size_of_me_factor = SizeofMeFactor() ;
	hv.Z = G0;
	if (hv.MagnitudeSqXY() > G(0, 10))
	{
		hv /= hv.Magnitude();
		hv *= (G(2, 0) * size_of_me_factor);
		GVector g1 = mOrientation2 * GVector(G0, G0, -G(1)) ;
		GVector hp = hv + mPos + g1; // height

		CThing *t = FindFirstCollision(hp, (G(1,45000) * size_of_me_factor));

		mLastPunchResult = PR_MISSED ;
		mLastPunchFrameNum = GAME.GetGameTurn() ;

		while (t)
		{

			CStrike this_strike ;
			this_strike.mWillKnockOut = FALSE ;
			this_strike.mStrikedWith = SI_SMALL_PUNCH;
			this_strike.mStrength = mCombatProfile.mElbowAttackStrength ;
			this_strike.mOriginator = this ;
			this_strike.mShoulderToFistDist = S_TO_G * size_of_me_factor ;
			this_strike.mAttackVelocity = GVector(G0,G0,G0) ;
			CCharacter *c = t->IsCharacter();
			if (c && (c->GetState()== CS_DEAD || c->GetLevelData().GetFinishedLevel()))
			{
				t = FindNextCollision();
				continue ;
			}
			GVector	dir = t->GetPos() - mPos;
			dir.Normalise();

			if (t->Strike(this_strike) && t->IsStrikeable())
			{
				// $$$ HIT EFFECT HERE
				hp.Z-=G1;
				mParticleSystem.MakeHitEffect(&hp, mYaw + G_PI,HT_NORMAL);
				mLastPunchResult = PR_HIT ;
				ScoreStrike(t) ;
			}
			else if (c)
			{
				// $$$ BLOCK EFFECT HERE
				hp.Z-=G1;
				mParticleSystem.MakeHitEffect(&hp, mYaw + G_PI,HT_BLOCK);
				mLastPunchResult = PR_BLOCKED ;
			}


			t = FindNextCollision();

		};
	}

	if (mLastPunchResult == PR_MISSED)
	{
		SOUND.PlaySample(SOUND_PUNCHMIS, this);
	}
	else
	{
		SOUND.PlaySample(SOUND_PUNCH1, this);
	}

}
//******************************************************************************************
BOOL	CCharBall::Attack(SINT no)
{

	if (SUPERTYPE::Attack(no) == FALSE) return FALSE ;

	// move forward a bit

	GVector hv = mOrientation2 * GVector(G0, G1, G0);
	GVector our_movement = hv * SMALL_COMBAT_MAX_VELOCITY ;
	GINT our_term_velocity = SMALL_COMBAT_MAX_VELOCITY ;
	AddStrikeVelocity(our_movement, our_term_velocity) ;


	return TRUE ;
}


//******************************************************************************************
void	CCharBall::AnimTrigger(const GMatrix &current_anim_orientation, BOOL end_of_punch_hit)
{

	// THIS FUNCTION IS A MESS  !!!!!!
	GINT size_of_me_factor = SizeofMeFactor() ;

	// hits will fail if one of these
	if (mState == CS_KNOCKED_OUT || mState == CS_DEAD) return ;

	switch (mAnimMode)
	{
	case AM_SPIN_ATTACK:
	case AM_RAGE_ATTACK:
		{
			SpinAnimTrigger(current_anim_orientation) ;
			break ;
		}
	case AM_RUN_PUNCH_LEFT:
		{
			GVector l(-G1,G0,G0) ;
			PunchLeftRightTrigger(l) ;
			break ;
		}
	case AM_RUN_PUNCH_RIGHT:
		{
			GVector r(G1,G0,G0) ;
			PunchLeftRightTrigger(r) ;
			break ;
		}

	case AM_THROW:
		{
			Throw() ;
			break ;
		}

//	case AM_PUNCH:
	case AM_PUNCH_HIT:
	case AM_PUNCH2:  // should be different
	case AM_PUNCH3:
	case AM_CHARGE:
		{
			GVector hv = mOrientation2 * GVector(G0, G1, G0);
			GVector our_movement_after_strike = GVector(G0,G0,G0);
			GINT our_term_velocity = SMALL_COMBAT_MAX_VELOCITY ;

			hv.Z = G0;
			if (hv.MagnitudeSqXY() > G(0, 10))
			{
				hv /= hv.Magnitude();
			
				if (mAnimMode == AM_CHARGE) 
				{
					hv*=(G(3,32000) * size_of_me_factor);
				}
				else 
				{
					hv*=(G(2,0) * size_of_me_factor) ;
				}

				GVector g1 = mOrientation2 * GVector(G0, G0, -G(1)) ;
				GVector hp = hv + mPos + g1; // height


				CThing *t = FindFirstCollision(hp, (G(1) * size_of_me_factor));


				mLastPunchResult = PR_MISSED ;
				mLastPunchFrameNum = GAME.GetGameTurn() ;

				while (t)
				{
					CStrike this_strike ;
					this_strike.mWillKnockOut = FALSE ;
					this_strike.mStrikedWith = SI_SMALL_PUNCH;
					this_strike.mStrength = mCombatProfile.mPunchStrenth ;
					this_strike.mOriginator = this ;
					this_strike.mShoulderToFistDist = S_TO_G * size_of_me_factor;
					//this_strike.mAttackVelocity = GVector(G0,G0,G0) ;
					GVector	dir = t->GetPos() - mPos;
					dir.Normalise();
					this_strike.mAttackVelocity = dir * SMALL_COMBAT_MAX_VELOCITY;
					CCharacter *c = t->IsCharacter();
					if (c && c->GetState()== CS_DEAD)
					{
						t = FindNextCollision();
						continue ;
					}

					if (mAnimMode == AM_CHARGE && c && c->GetState() == CS_KNOCKED_OUT)
					{
					
						t = FindNextCollision();
						continue ;
					
					}

					if (mAnimMode == AM_CHARGE)
					{
						if (t->IsStrikeable())
						{
							this_strike.mAttackVelocity = dir * G(0, 24000);
							this_strike.mWillKnockOut = TRUE ;
							this_strike.mStrength = mCombatProfile.mChargeAttackStrength ;
							this_strike.mStrikedWith = SI_CHARGE_PUNCH;
							our_movement_after_strike = -(this_strike.mAttackVelocity) ;
							AddVelocity(-(this_strike.mAttackVelocity)*G(0,16124)) ;
						}
					}

					else if (mAnimMode == AM_PUNCH_HIT)
					{
										// hack to stop big robots knock you out !! change
						if (mCurrentPunchSequenceNum >= mCombatProfile.mNumAttacksForCombo)
						{
							// HACK CITY..

							if (end_of_punch_hit)
							{
							
								this_strike.mAttackVelocity = dir * G(0, 24000);
								this_strike.mWillKnockOut = TRUE ;
									
								SINT avc = _GetClassID() ;
								if (avc != OID_CPrimaryDoctor && avc != OID_CPrimaryBenny)
								{
									this_strike.mStrength = mCombatProfile.mBigPunchStrenth ;
								}
								else
								{
									this_strike.mStrength = 0 ;
								}
								this_strike.mStrikedWith = SI_SMALL_PUNCH;
							
							}
							else
							{
								// ok this is here for benny and doctor basicaly say whilst
								// there are drilling or whatever just stun the apponent
								this_strike.mStrength = mCombatProfile.mBigPunchStrenth / 9 ;
								this_strike.mShoulderToFistDist *= 2 ;
						
							//	this_strike.mAttackVelocity = GVector(G0,G0,G0) ;
							}


						}
					}


					// if strike successful

					if (t->Strike(this_strike) && t->IsStrikeable() )
					{
						mLastPunchResult = PR_HIT ;
						ScoreStrike(t) ;
						hp.Z -= G1;
						// $$$ HIT EFFECT HERE
						mParticleSystem.MakeHitEffect(&hp, mYaw + G_PI,HT_NORMAL);
						//GVector passpos=mPos;
						//passpos.Z-=G(2);
						//mParticleSystem.MakeHitEffect(&passpos, mYaw + G_PI,HT_NORMAL);
						if (this_strike.mWillKnockOut == FALSE)
						{
							our_movement_after_strike = this_strike.mAttackVelocity ;
						}

						if (!end_of_punch_hit)
						{
							if (c)
							{
								GVector a1 = c->GetVelocity() ;
								if (GAME.GetGameTurn() & 1 )
								{
									a1+= GVector(G(0,10000) ,G(0,10000),G(0,0)) ;
								}
								else
								{
									a1+= GVector(-G(0,10000) ,-G(0,10000),-G(0,0)) ;
								}
								c->SetVelocity(a1) ;
							}
						}
					}
					else if (c)
					{
						mLastPunchResult = PR_BLOCKED ;
						hp.Z -= G1;
						// $$$ BLOCK EFFECT HERE
						mParticleSystem.MakeHitEffect(&hp, mYaw + G_PI,HT_BLOCK);

						if (this_strike.mWillKnockOut == FALSE)
						{
							our_movement_after_strike = this_strike.mAttackVelocity ;
						}
					}


					// if your are a primary, rotate so you face the target ( helping the user
					// a bit ;) )
					if (c && IsUpright() && IsPrimary() && mAnimMode != AM_CHARGE)
					{

						GVector u_to_t =  (t->GetPos() - mPos ) ;
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
						SOUND.PlaySample(SOUND_PUNCH2, this); //!!!
					}
					else
						SOUND.PlaySample(SOUND_PUNCH1, this);
				}
			}
		}
		break;

//		case AM_WEAPON_ATT:
		case AM_WEAPON_HIT:
		{
			GVector hv = mOrientation2 * GVector(G0, G1, G0);
				GVector our_movement_after_strike = GVector(G0,G0,G0);
			GINT our_term_velocity = SMALL_COMBAT_MAX_VELOCITY ;
			hv.Z = G0;
			if (hv.MagnitudeSqXY() > G(0, 10))
			{
				hv /= hv.Magnitude();
				hv *= (G(2, 0) * size_of_me_factor);

				GVector g1 = mOrientation2 * GVector(G0, G0, -G(1) ) ;
				GVector hp = hv + mPos + g1; // height

				CThing *t = FindFirstCollision(hp, (G(1,32268) * size_of_me_factor));

				mLastPunchResult = PR_MISSED ;
				mLastPunchFrameNum = GAME.GetGameTurn() ;

				while (t)
				{

					CStrike this_strike ;
					this_strike.mWillKnockOut = TRUE ;
					this_strike.mStrikedWith = SI_WEAPON ;
					CCharacter *c = t->IsCharacter();
					if (c && c->GetState() == CS_DEAD)
					{
						t = FindNextCollision();
						continue ;
					}
					GINT angle_me_to_apponent = mYaw ;
					if (c)
					{
						GVector a1 = c->GetPos() - mPos ;
						angle_me_to_apponent = -GATAN2(a1.X, a1.Y) ;
					}

					GVector	dir = t->GetPos() - mPos;
					dir.Normalise();
				    this_strike.mAttackVelocity =  dir * G(0, 12000);

					this_strike.mOriginator = this ;
					this_strike.mShoulderToFistDist = S_TO_G * size_of_me_factor;
					this_strike.mStrength = mCombatProfile.mWeaponAttackStrength ;
					if (t->Strike(this_strike) && t->IsStrikeable())
					{
						mLastPunchResult = PR_HIT ;
						ScoreStrike(t) ;
						if (c && (!((CCharBall*)c)->IsUpright()) )
						{
							hp.Z = c->GetPos().Z - c->Radius() ;
						}
						// $$$ HIT SOMEONE WITH WEAPON EFFECT HERE
						mParticleSystem.MakeHitEffect(&hp, mYaw + G_PI,HT_WEAPON);
						//mParticleSystem.MakeHitEffect(&hp, angle_me_to_apponent + G_PI,HT_WEAPON);
						our_movement_after_strike = -(this_strike.mAttackVelocity*G(0,32123)) ;

					}
					else if (c)
					{
						mLastPunchResult = PR_BLOCKED ;
						hp.Z -= G1;
						// $$$ BLOCK EFFECT HERE
						mParticleSystem.MakeHitEffect(&hp, angle_me_to_apponent + G_PI,HT_BLOCK);
						our_movement_after_strike = this_strike.mAttackVelocity ;
					}

					t = FindNextCollision();

				};
			}
			AddStrikeVelocity(our_movement_after_strike, our_term_velocity) ;

			if (mLastPunchResult == PR_MISSED)
				SOUND.PlaySample(SOUND_WEAPMIS, this);
			else
			{
				SOUND.PlaySample(SOUND_PUNCH2, this);
				if (IsPrimary() && mHoldingThing )
				{
					CWeapon* w = mHoldingThing->IsWeapon() ;
					if (w)
					{
						w->DecrementHitsLeft() ;

						if (w->GetHitsLeft() <=0)
						{
							Drop();
							w->StartDying() ;
						}
					}
				}
			}
		}
		break;

//	case AM_JUMP_ATTACK:
	case AM_JUMP_HIT:
		{
			GVector hv = mOrientation2 * GVector(G0, G1, G0);
			hv.Z = G0;
			if (hv.MagnitudeSqXY() > G(0, 10))
			{
				hv /= hv.Magnitude();
		
				hv *= (G(1, 30000) * size_of_me_factor);

				GVector g1 = mOrientation2 * GVector(G0, G0, -G(1)) ;
				GVector hp = hv + mPos + g1; // height

				CThing *t = FindFirstCollision(hp, (G(2) * size_of_me_factor));

				mLastPunchResult = PR_MISSED ;
				mLastPunchFrameNum = GAME.GetGameTurn() ;

				while (t)
				{

					CStrike this_strike ;
					this_strike.mWillKnockOut = TRUE ;
					this_strike.mStrikedWith = SI_SMALL_PUNCH ;
					CCharacter *c = t->IsCharacter();
					if (c && c->GetState() == CS_DEAD)
					{
						t = FindNextCollision();
						continue ;
					}

					GINT angle_me_to_apponent = mYaw ;
					if (c)
					{
						GVector a1 = c->GetPos() - mPos ;
						angle_me_to_apponent = -GATAN2(a1.X, a1.Y) ;
					}

					GVector	dir = t->GetPos() - mPos;
					dir.Normalise();
					this_strike.mAttackVelocity = dir * G(0, 12000);
					this_strike.mOriginator = this ;
					this_strike.mShoulderToFistDist = S_TO_G * size_of_me_factor;
					this_strike.mStrength = mCombatProfile.mJumpAttackStrength ;

					if (t->Strike(this_strike) && t->IsStrikeable())
					{
						mLastPunchResult = PR_HIT ;
						ScoreStrike(t) ;
						if (c && (!((CCharBall*)c)->IsUpright()) )
						{
							hp.Z = c->GetPos().Z - c->Radius() ;
						}
						else
						{
								hp.Z -= G(0,32123) ;
						}

						// $$$ JUMP HIT EFFECT HERE
						mParticleSystem.MakeHitEffect(&hp, angle_me_to_apponent + G_PI,HT_JUMP);

					}
					else if (c)
					{
						hp.Z -= G(0,32123) ;
						// $$$ BLOCK EFFECT HERE
						mParticleSystem.MakeHitEffect(&hp, angle_me_to_apponent + G_PI,HT_BLOCK);
						mLastPunchResult = PR_BLOCKED ;
					}
					t = FindNextCollision();

				};
			}
			if (mLastPunchResult == PR_MISSED)
				SOUND.PlaySample(SOUND_PUNCHMIS, this);
			else
				SOUND.PlaySample(SOUND_PUNCH1, this);
		}
		break ;
	};
}


//******************************************************************************************
void	CCharBall::Trick(SINT no)
{
	//! use number!
	if ((mAnimMode == AM_JUMP) || (mAnimMode == AM_MID_JUMP))
		SetAnimMode(AM_TAILGRAB);
//		SetAnimMode(AM_CHRISTAIR);
}


//******************************************************************************************
GVector	CCharBall::GetPos(SINT n)
{
	if (n == 0)
		return mPos; // should supertype, but I can't be bothered

	// work out an approximate position for the top collision sphere

//	return mPos + (mOrientation2 * GVector(G0, G0, -((Radius() + Radius2()))));

	// JCL - fix for internal compiler error...
	GVector offset = GVector(G0, G0, -((Radius() + Radius2())));
	return mPos + (mOrientation2 * offset);
}

//******************************************************************************************
void	CCharBall::Throw()
{
	SUPERTYPE::Throw() ;
	if (mHoldingThing)
	{
		GMatrix new_mat = mOrientation2 ;


		GVector hv = mOrientation2 * GVector(G0, G1, -G(0,16232));
		if (hv.MagnitudeSqXY() > G(0, 10))
		{
				mHoldingThing->Drop(CHARACTER_THREW_IT) ;
				mCanBlockBF &= (~SI_WEAPON) ;
				hv /= hv.Magnitude();
				hv /= G(2) ;
				hv+=GetVelocity() ;
				mHoldingThing->SetVelocity(hv) ;
				mHoldingThing = NULL ;

		}
	}
}

//******************************************************************************************
BOOL	CCharBall::RacerAttack()
{

	if (IsAnimBusy(mAnimMode)) return FALSE ;

	// ok who's the closest racer to me ?

	CharactersInWorldList& items = WORLD.GetCharacterNB();

	CCharacter	*current = NULL;
	GINT		mind = mCharacterSight*mCharacterSight;

	CCharacter* nt;
	for (nt = items.First();
		 nt != NULL ;
		 nt = items.Next() )
	{
		if (nt!=this && nt->GetState() != CS_DEAD)
		{
			GINT	dist = (mPos - nt->GetPos()).MagnitudeSq();
			if (dist < mind)
			{
				mind = dist;
				current = nt;
			}
		}
	}

	if (current==NULL)
	{
			SetAnimMode(AM_RUN_PUNCH_RIGHT) ;
			return TRUE ;
	}

	// ok is current target left or right of me

	GVector otho = mOrientation2 * GVector(G1,G0,G0)  ;
	GVector u_to_target = current->GetPos() - mPos ;
	if ((u_to_target * otho) >=0)
	{
		SetAnimMode(AM_RUN_PUNCH_RIGHT) ;
	}
	else
	{
		SetAnimMode(AM_RUN_PUNCH_LEFT) ;
	}

	return TRUE ;
}

//******************************************************************************************

BOOL	CCharBall::MainCharacterAttack(SINT no)
{
	//! use number!

	if (no ==1 )
	{
		if (SpinAttack())
		{
			mCurrentPunchSequenceNum = 0 ;
			return TRUE ;
		}
		else
		{
			return FALSE ;
		}
	}

	if (mHoldingThing && mHoldingThing->IsWeapon())
	{
		SetAnimMode(AM_WEAPON_ATT);
		mCurrentPunchSequenceNum = 0 ;
	}
	else
	{
		if ((mAnimMode == AM_JUMP) || (mAnimMode == AM_MID_JUMP))
		{
			SetAnimMode(AM_JUMP_ATTACK);
			mCurrentPunchSequenceNum= 0 ;
		}
		else
		{
			if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER)
			{
				return RacerAttack() ;
			}

			if (mCurrentPunchSequenceNum >= mCombatProfile.mNumAttacksForCombo)
			{
				mCurrentPunchSequenceNum = 0 ;
			}

			SINT diff = GAME.GetGameTurn() - mLastPunchFrameNum ;
			// combo stuff
			#define PRIMARY_NUM_FRAMES_END_COMBO 25

			// SRG change if last punch was blocked still build up combo
			if (diff < PRIMARY_NUM_FRAMES_END_COMBO && mLastPunchResult != PR_MISSED)
			{
				mCurrentPunchSequenceNum++ ;
			}
			else
			{
				mCurrentPunchSequenceNum = 0 ;
			}


			BOOL do_charge_attack = FALSE;
			GINT v = mVelocity.MagnitudeXY();

			// this magic number comes for setanimmode
			if (v>G(0,20000)  && mCurrentPunchSequenceNum == 0 )
			{
				// check we are moving forward
				GVector v1 = mVelocity ;
				v1.Normalise() ;

				// do charge attack if we are facing velocity vector  + - 30 degrees
				if ( (v1 * (mOrientation2 *GVector(G0,G1,G0))) > G(0,32768)   )
				{
					do_charge_attack = TRUE ;
				}
			}

			if (do_charge_attack)
			{
				SetAnimMode(AM_CHARGE) ;

				// gives you a boost
				GVector v1=mVelocity ;
				v1.Normalise() ;
				AddVelocity(v1*G(0,23000)) ;
			}

			else if (mCurrentPunchSequenceNum >= mCombatProfile.mNumAttacksForCombo)
			{
				SetAnimMode(AM_PUNCH) ;
			}
			else
			{
				if (mCurrentPunchSequenceNum == 0)
				{
					SetAnimMode(AM_GUARD_UP);
				}
				else if (mCurrentPunchSequenceNum%2 ==0)
				{
					SetAnimMode(AM_PUNCH3);
				}
				else
				{
					SetAnimMode(AM_PUNCH2) ;
				}
			}
		}
	}
	return TRUE ;

}


//******************************************************************************************
BOOL	CCharBall::SpinAttack()
{
	if ((mAnimMode == AM_JUMP) || (mAnimMode == AM_MID_JUMP))
	{
		return FALSE;
	}

	CPrimary	*pr = this->IsPrimary();

	if (pr)
	{
		if (pr->GetBrakePower() <= G0)
			return FALSE;
	}

	SetAnimMode(AM_SPIN_ATTACK) ;

	return TRUE ;
}


//******************************************************************************************

void	CCharBall::HitGround(GINT force)
{
	mSuspensionVelocity += force / SUSPENSION_STRENGTH2;
}


//******************************************************************************************
void	CCharBall::StateBeingLasered()
{
	mDestYaw = mYaw ;
	mDestRoll	= mRoll;
	mDestPitch = mPitch;
	mPitch = G0 ;
	SUPERTYPE::StateBeingLasered() ;
}


//******************************************************************************************
#ifdef LINK_EDITOR
void	CCharBall::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	SINT	oid = GetTorsoID();
	if (oid != OID_NONE)
		WORLD.AccumulateOIDResources(oid);
	WORLD.AccumulateOIDResources(OID_CFXExplosion2);
	WORLD.AccumulateOIDResources(OID_CFXBodyFlamer);
}
#endif

//******************************************************************************************
// Stuff for the link

GVector	CCharBall::GetRenderStartPos()
{
	return GetRenderPos() + (mOrientation2 * GVector(G0, G0, -LinkRadius()));
}

//******************************************************************************************
GVector	CCharBall::GetRenderEndPos()
{
	return GetRenderPos() + GetRenderThingEmitterAnimated(6);
}

//******************************************************************************************
GMatrix	CCharBall::GetRenderOrientation(SINT no)
{
	switch (no)
	{
	case 1:
		{
			GMatrix foo = mLastOrientation2 + ((mOrientation2 - mLastOrientation2)* GAME.GetFrameRenderFraction());
			foo.Normalise();
			return foo;
		}
	default:	return SUPERTYPE::GetRenderOrientation(no);
	};
};
