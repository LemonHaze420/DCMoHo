#include	"Common.h"

#include	"Globals.h"
#include	"GunTurret.h"
#include	"Primary.h"

//test -  hi mum...

//******************************************************************************************
void	CGunTurret::Init(CInitThing *i)
{

	SUPERTYPE::Init(i);

	StickToGround();
	
	if (i->mAngleType == AT_YPR)
	{
		mYaw = i->mYaw;
		mPitch = i->mPitch;
		mOrientation.MakeRotation(G0, G0, i->mRoll);
	}		
	else		
	{
		mYaw = G0;
		mPitch = G0;
	}

	mStartBurstNumber = mNumber * 43 ;
	mMoving = FALSE ;

	mOrientation2.MakeRotation(mYaw, mPitch, G0);

	GET_COUNTER(0);
	GET_COUNTER(1);
	GET_COUNTER(2);
	GET_BOOL(0);
	GET_BOOL(1);
	GET_BOOL(2);
	GET_BOOL(3);


	GET_INITIAL_MODE;
	if (mInitialMode == IM_INACTIVE)
		mActivated = FALSE;
	else
		mActivated = TRUE;

	GET_TRIGGER_RESPONSE;
	if (mTriggerResponse == TR_UNUSED)
		mTriggerResponse = TR_ACTIVATE;
	GET_UNTRIGGER_RESPONSE;
	if (mUnTriggerResponse == TR_UNUSED)
		mUnTriggerResponse = TR_NONE;
	mTarget = NULL ;
	mFireAtWayPoint = FALSE;

}

//********************************************************************************************************
#if TARGET == PC
void	CGunTurret::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
	WRITE_COUNTER(1);
	WRITE_COUNTER(2);
	WRITE_BOOL(0);
	WRITE_BOOL(1);
	WRITE_BOOL(2);
	WRITE_BOOL(3);
	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}
#endif


//******************************************************************************************
void	CGunTurret::ActivateGunTurret()
{
	if (mActivated== FALSE)
	{
		mActivated= TRUE ;
		// make the turret fire now !! 
		mStartBurstNumber = (mCounter0 - ((GAME.GetGameTurn() /3) % mCounter0)) - 1;
	}
}

//******************************************************************************************
void	CGunTurret::Trigger(CThing *top)
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateGunTurret(); break;
	case TR_DEACTIVATE:
		mActivated = FALSE; break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateGunTurret();
			}
			else
			{
				mActivated = FALSE;
			}
			break ;
		}
	};

	SUPERTYPE::Trigger(top);
}



//******************************************************************************************
void	CGunTurret::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateGunTurret(); break;
	case TR_DEACTIVATE:
		mActivated = FALSE; break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateGunTurret();
			}
			else
			{
				mActivated = FALSE;
			}
			break ;
		}
	};
	mTimer = 0;

	SUPERTYPE::UnTrigger(top);
}


//******************************************************************************************
#define GT_BULLET_SPEED G(1)
//#define GT_BULLET_SPEED G(0, 30000)

void	CGunTurret::Shoot()
{
	// do we shoot?
	if (mBool2)
		if (!GAME.IsGameRunning())
			return;


	CThing	*t;
	
	if (!mBool3)
		t = SpawnThing(OID_CPJBulletRainDrop);
	else
		t = SpawnThing(OID_CPJBullet);

	if (t)
	{
		CInitThing i;
		
		GVector start = mPos;

		GMatrix	m;
		m.MakeRotation(mYaw, mPitch, G0);

		GVector v = m * GVector(G0, GT_BULLET_SPEED, G0);

		i.mVelocity = v;

		i.mVelocity += GVector(G(0, (RANDOM() % 2000)) - G(0, 1000),
							   G(0, (RANDOM() % 2000)) - G(0, 1000),
							   G(0, (RANDOM() %  500)) - G(0,  250));

		i.mPos = start + GetRenderThingEmitter(0); // + v * G(2);

		t->Init(&i);
		
		SOUND.PlaySample(SOUND_SHOT1, this);
	
	
		// Muzzle Flash
		BOOL seen=GAME.CanIBeSeen(mPos);
		if(seen)
		{
			CParticle *p=MAP.GetEffectParticleSystem()->CreateParticle(PART_ANIM);
			if(p)
			{
				p->mPos = i.mPos;

				p->mColour=0x00ffffff;
				p->uPAnim.mFrame=TP_FLASH_SM;
				p->uPAnim.mRate=1;
				p->mRadius = 128;
				p->uPAnim.mFrames=1;
				p->mDelay = 0;
				p->uPAnim.mLoops=0;
				p->uPAnim.mHoriz=0;
			}
			p=MAP.GetEffectParticleSystem()->CreateParticle(PART_ANIM);
			if(p)
			{
				p->mPos = i.mPos;

				p->mColour=0x00808080;
				p->uPAnim.mFrame=TP_FLASH_SM;
				p->uPAnim.mRate=1;
				p->mRadius = 256;
				p->uPAnim.mFrames=1;
				p->mDelay = 0;
				p->uPAnim.mLoops=0;
				p->uPAnim.mHoriz=0;
			}
		}			
	}
}

//******************************************************************************************
void	CGunTurret::Aim()
{
	if (!mTarget) return ;
	GVector d = (mTarget->GetPos() - mPos);

	GINT	v = d.MagnitudeXY();

	if (mCounter2 > 0)
	{
	
		d += (mTarget->GetVelocity() * G(mCounter2) / G(4)) * (d.Magnitude() / GT_BULLET_SPEED);
	}


	// do yaw ;

	GINT RequiredYaw = GATAN2(d.Y, d.X) - G_PI_2;

	if (GABS(AngleDifference(mYaw, RequiredYaw)) < G(0,3000) )
	{
		mYaw = RequiredYaw;
	}
	else
	{
		// go left or right ?
		GVector otho1 = mOrientation2 * GVector(G1,G0,G0)  ;
		GINT dot = d * otho1 ;

		if (dot >=0)
		{
			mYaw-=G(0,3000) ;
			if (mYaw < -G_PI) mYaw+=(G_PI*2); 
		}
		else
		{
			mYaw+=G(0,3000) ;
			if (mYaw > G_PI) mYaw-=(G_PI*2); 
		}
	}

	// do pitch

	GINT RequiredPitch = GATAN2(v, -d.Z) - G_PI_2;
	{
		if (GABS(RequiredPitch-mPitch) < G(0,1000) )
		{
			mPitch = RequiredPitch ;
		}
		else
		{
			if (RequiredPitch > mPitch)
			{
				mPitch+=G(0,1000) ;
			}
			else
			{
				mPitch-=G(0,1000) ;
			}
		}
	}

	mOrientation2.MakeRotation(mYaw, mPitch, G0);

}


//******************************************************************************************
void	CGunTurret::GetPrimaryTarget()
{
	mTarget = PLAYER(0)->GetPrimary() ;
	if (!GAME.IsMultiPlayer()) return ;

	GINT dist_to_player_1 ;
	if (mTarget)
	{
		dist_to_player_1 = (mPos-mTarget->GetPos()).MagnitudeSq() ;
		CThing* p2 = PLAYER(1)->GetPrimary() ; 
		if (p2)
		{
			GINT dist_to_player_2 = (mPos-p2->GetPos()).MagnitudeSq() ;
			if (dist_to_player_2 < dist_to_player_1)
			{
				mTarget = p2 ;
			}
		}
	}
	else
	{
		mTarget = PLAYER(1)->GetPrimary() ;
	}
}


//******************************************************************************************
void	CGunTurret::Process()
{
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;
	mMoving=FALSE ;

	if (!mFireAtWayPoint)
	{
		GetPrimaryTarget() ;
	}

	if (!mBool2)
	{
		
		if (mBool0) // spin?
		{
			mYaw += mBool1 ? -G(0, 3000) : G(0, 3000);   // direction
			mOrientation2.MakeRotationYaw(mYaw);
			mMoving=TRUE ;
		}
	}
	else if (mActivated)
	{
		Aim();
		mMoving=TRUE ;
	}

	if (mActivated && mTarget)
	{
		SINT	a = GAME.GetGameTurn();
		if (a == ((a / 3) * 3))
		{
			SINT	num = (GAME.GetGameTurn() / 3 )+ mStartBurstNumber;

 			if ((mCounter0 == -1) ||
				((num % mCounter0) < mCounter1))
			{
				GVector d = (mTarget->GetPos() - mPos);
				if (d.MagnitudeSq() < G(4000))
				{
					if (!mTarget->IsPrimary())
					{
						Shoot() ;
					}
					else
					{
						if(!((CPrimary*)mTarget)->GetLevelData().GetFinishedLevel()) Shoot();
					}
				}
			}
		}
	}

	SUPERTYPE::Process();
}




//******************************************************************************************
#ifdef LINK_EDITOR

void	CGunTurret::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CPJBullet);
	WORLD.AccumulateOIDResources(OID_CPJBulletRainDrop);
}
#endif

//******************************************************************************************


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

#define CANNON_GROUND_SPEED_MAX	GINT(0, 52428)
#define CANNON_GROUND_SPEED_MIN	GINT(0, 10000)
#define CANNON_GROUND_SPEED_DEF	GINT(0, 25000)

void	CCannon2::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	GET_OID;
	GET_TARGET;

	if (mTarget != NULL) mFireAtWayPoint = TRUE ;
	mCachedVelocity = ZERO_GVECTOR;
	mOrientation3 = ID_GMATRIX;
	mPitch = -G_PI_2 ;
	mOrientation3.MakeRotation(G0, mPitch, G0);
}

//******************************************************************************************
void	CCannon2::Aim()
{
	GVector	v;
	GVector	targ_pos;

	if (mTarget)
		targ_pos = mTarget->GetPos();
	else
		targ_pos = WORLD.GetPrimaryPosition();

	v = (targ_pos - mPos);

	GINT	speed	 = v.MagnitudeXY() / 64;
	if (speed > CANNON_GROUND_SPEED_MAX) speed = CANNON_GROUND_SPEED_MAX;
	if (speed < CANNON_GROUND_SPEED_MIN) speed = CANNON_GROUND_SPEED_MIN;

	v.Normalise();
	v *= speed;
//	v.Z = -WORLD.Gravity() * (WORLD.GetPrimaryPosition() - mPos).MagnitudeXY() / speed / 2;
	v.Z = -WORLD.Gravity() * (targ_pos - mPos).MagnitudeXY() / speed / G(2, 10000);

	mCachedVelocity = v;

	// work out pitch * yaw

	mYaw = GATAN2(v.Y, v.X) - G_PI_2;
	
	GINT	l = v.MagnitudeXY();
	if (l > G(0,100))
		mPitch = GATAN2(l, -v.Z) - G_PI_2;

	mOrientation2.MakeRotation(mYaw, G0, G0);
	mOrientation3.MakeRotation(G0, mPitch, G0);
}

//******************************************************************************************
void	CCannon2::Shoot()
{
	SINT	oid;

	if ((mOID != OID_NONE) && (mOID != OID_INVALID))
		oid = mOID;
	else
		oid = OID_CCannonBall;

	CThing *t = SpawnThing(oid);

	if (t)
	{
		CInitThing i;
	
		// aiming?
		if (mBool2)
		{
			i.mVelocity = mCachedVelocity;
		}
		else
		{		
			GMatrix	m;
			m.MakeRotation(mYaw, mPitch, G0);

			GVector v = m * GVector(G0, CANNON_GROUND_SPEED_DEF, G0);

			i.mVelocity = v;
		}

		i.mVelocity += GVector(G(0, (RANDOM() % 2000)) - G(0, 1000),
							   G(0, (RANDOM() % 2000)) - G(0, 1000),
							   G(0, (RANDOM() %  500)) - G(0,  250));

		i.mPos = mPos + GetRenderThingEmitter(0); // + v * G(2);

		t->Init(&i);
	}
}

//******************************************************************************************
#if TARGET == PC
void	CCannon2::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_OID;
	WRITE_TARGET;
}	
#endif

//******************************************************************************************

#ifdef LINK_EDITOR

void	CCannon2::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	SINT	oid;

	if ((mOID != OID_NONE) && (mOID != OID_INVALID))
		oid = mOID;
	else
		oid = OID_CCannonBall;

	WORLD.AccumulateOIDResources(oid);
}
#endif
