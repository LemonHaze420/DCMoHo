#include	"Common.h"

#if 0

#include	"CharNoBall.h"
#include	"Globals.h"

#define	CharNoBall_ACCEL	GINT(0, 1311)
#define CharNoBall_STUN_TIME 15

#define CharNoBall_TILT_DAMP GINT(0, 55000)
#define CharNoBall_TILT_MAG  GINT(0,  15000)

#define	CharNoBall_DIE_TILT	 GINT(1, 40000)
#define	CharNoBall_DIE_ACCEL GINT(0, 500)
#define	CharNoBall_DIE_DAMP  GINT(0, 26214)

//******************************************************************************************
void	CCharNoBall::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mTilt = G0;
	mDir = mOrientation * GVector(G0, G1, G0);
	SitOnGround();

	mTiltVelocity = G0;
}

//******************************************************************************************
void	CCharNoBall::StateNormal()
{
	// try to head towards person
	GVector d = WORLD.GetPrimaryPosition() - mPos;
	GINT	dist = d.Magnitude();
	d /= dist;
	mDir = d;

	GINT	force = G0;
	if ((dist < G(2)) &&
		(mVelocity * d < G(0)))
		force = G(-1); //go away
	else
		force = G( 1);

	if (mFlags & TF_ON_GROUND)
	{
		mVelocity += d * CharNoBall_ACCEL * force; 
	}

	force = d * mVelocity;

	mTilt = (mTilt * CharNoBall_TILT_DAMP) + (force * CharNoBall_TILT_MAG);
}

//******************************************************************************************
void	CCharNoBall::StateDead()
{
	mTiltVelocity += (mTiltTo > G0) ? CharNoBall_DIE_ACCEL : - CharNoBall_DIE_ACCEL;
	mTilt += mTiltVelocity;
	if (((mTiltTo > G0) && (mTilt > mTiltTo)) ||
		((mTiltTo < G0) && (mTilt < mTiltTo)))
	{
		mTilt = mTiltTo;
		mTiltVelocity = -mTiltVelocity * CharNoBall_DIE_DAMP;
	}
}

//******************************************************************************************
void	CCharNoBall::StateAsleep()
{
	// rock!
	mTilt = (GSIN(G(GAME.GetGameTurn() + mNumber * 43) / 10) + G1) / 10;

	SUPERTYPE::StateAsleep();
}

//******************************************************************************************
void	CCharNoBall::Process()
{
	SUPERTYPE::Process();

	// it's just about ok to do this after the process, because it only affects orientation.
	GINT	v = mDir.MagnitudeXY();

	if (v > GINT(0, 100))
	{
		GINT	sn = -mDir.X / v;
		GINT	cs =  mDir.Y / v;

		mOrientation.MakeRotationYawSC(sn, cs);
	}

	if (mTilt != G0)
	{
		// tilt in direction of force
		GMatrix m;
		GVector ax = mDir ^ GVector(G0, G0, G(1));
		if (ax.MagnitudeSq() > G(0, 100))
		{
			m.MakeGeneralAxisRotation(ax, mTilt);
			mOrientation = m * mOrientation;
		}
	}
}


//******************************************************************************************
void	CCharNoBall::StartDieProcess(BOOL hit_fan)
{
	ChangeState(CS_DEAD, CStateInit());

	mTiltVelocity = G0;
	mTiltTo = (mTilt > G0) ? (-CharNoBall_DIE_TILT) : CharNoBall_DIE_TILT;
}

//******************************************************************************************
void	CCharNoBall::Hit(CThing *that)
{
	/*if (mState != CS_DEAD)
	{
		if (that->IsFriendly())
		{
			GINT v = (mVelocity - that->GetVelocity()).MagnitudeXY() * G(2);

			if (v > G1)
				v = G1;

			that->Damage(v);
		}
	}*/
}

#endif