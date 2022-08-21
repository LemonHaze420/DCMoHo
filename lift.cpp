#include	"Common.h"

#include	"Lift.h"
#include	"Map.h"

#if TARGET == PSX
//!PSX
#pragma optimization_level 0
#endif

//******************************************************************************************
void	CLift::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	mStart = mPos; // + GVector(G0, G0, G(3));   //!!!
	mDir = GetEndPos() - mPos;
	mEndDist = mDir.Magnitude();
	mDir /= mEndDist; // Normalise
	mDist = G0;
	mSpeed = G0;
	mForwards = TRUE;
	mCounter = 0;

	mRoll = G0;
	mRollVelocity = G0;

	mYaw = InitialYaw();

	mOrientation.MakeRotation(mYaw, G0, mRoll);
}

//******************************************************************************************
void	CLift::Process()
{
	// movement
	mSpeed += GetAccel() * ((mForwards) ? 1 : -1);
	mDist += mSpeed;

	if (mForwards)
	{
		if (mDist > mEndDist)
		{
			mDist = mEndDist;
			mRollVelocity += RollHitForwards() * GABS(mSpeed);
			mSpeed = -mSpeed * GetDamping();
		}
	}
	else
	{
		if (mDist < G0)
		{
			mDist = G0;
			mRollVelocity += RollHitBackwards() * GABS(mSpeed);
			mSpeed = -mSpeed * GetDamping();
		}
	}

	// rotation
	mRollVelocity -= GINT(0, 393);
	mRoll += mRollVelocity;
	if (mRoll < G0)
	{
		mRoll = G0;
		mRollVelocity = -mRollVelocity * GINT(0, 26214);
	}

	mOrientation.MakeRotation(mYaw, G0, mRoll);
	// timer

	mCounter ++;
	if (mCounter == GetCounterMax())
	{
		mCounter = 0;
		mForwards = !mForwards;
	}

	mVelocity = (mStart + mDir * mDist) - mPos;

	SUPERTYPE::Process();
}

//******************************************************************************************
GMatrix	CLift::GetAngularVelocity(SINT no)
{
	// build angular velocity matrix
	GMatrix m;
	m.MakeRotationRoll(mRollVelocity);

	return m;  // see - easy wasn't it!
}
