#include	"Common.h"

#include	"Flipper.h"
#include	"Globals.h"

//******************************************************************************************
void	CFlipper::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	mYaw = GetStartYaw();
	mOrientation2.MakeRotationYaw(mYaw);
	mOmega = G0;
}

//******************************************************************************************
//#define FLIPPER_ACCEL	G(0,2000)
//#define FLIPPER_ACCEL	G(0,3000)
#define FLIPPER_ACCEL	G(0,2000)
#define FLIPPER_DAMPING G(0,40000)

void	CFlipper::Process()
{
	StickToGround();

	GINT	startyaw, destyaw;
	//!CONTROLS not sure this should really be here..
	if (CONTROLS.IsButtonPressed(GetButton()))
	{
		startyaw = GetStartYaw();
		destyaw = GetEndYaw();
	}
	else
	{
		startyaw = GetEndYaw();
		destyaw = GetStartYaw();
	}

	if (destyaw > startyaw)
		mOmega += FLIPPER_ACCEL;
	else
		mOmega -= FLIPPER_ACCEL;

	mYaw += mOmega;
	
	if (destyaw > startyaw)
	{
		if (mYaw > destyaw)
		{
			mOmega = -mOmega * FLIPPER_DAMPING;
			mYaw = destyaw;
		}
		if (mYaw < startyaw)
		{
			mOmega = -mOmega * FLIPPER_DAMPING;
			mYaw = startyaw;
		}
	}
	else
	{
		if (mYaw < destyaw)
		{
			mOmega = -mOmega * FLIPPER_DAMPING;
			mYaw = destyaw;
		}
		if (mYaw > startyaw)
		{
			mOmega = -mOmega * FLIPPER_DAMPING;
			mYaw = startyaw;
		}
	}

	mOrientation2.MakeRotationYaw(mYaw);

	SUPERTYPE::Process();
}

//******************************************************************************************
GMatrix	CFlipper::GetAngularVelocity(SINT no)
{
	if (no == 0)
	{
		// build angular velocity matrix
		GMatrix m;
		m.MakeRotationYaw(mOmega);

		return m;  // see - easy wasn't it!
	}
	else return SUPERTYPE::GetAngularVelocity(no);
}
