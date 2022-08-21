#include	"Common.h"

#include	"Drawbridge.h"
#include	"Map.h"
/*
//******************************************************************************************
void	CDrawbridge::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mRoll = G0;
	mRollVelocity = G0;
	StickToGround();
	mTriggered = FALSE;
	mInitialOrientation = mOrientation;
}

//******************************************************************************************
void	CDrawbridge::Process()
{
	if (mTriggered)
	{
		mRollVelocity += G(0, 197);
		mRoll += mRollVelocity;

		if (mRoll > G_PI / 2)
		{
			mRoll = G_PI / 2;
			mRollVelocity = -mRollVelocity * GINT(0, 39321);
		}
	}
	GMatrix m;

	m.MakeRotationRoll(mRoll);
	mOrientation = mInitialOrientation * m;

	StickToGround();
	SUPERTYPE::Process();
}
*/