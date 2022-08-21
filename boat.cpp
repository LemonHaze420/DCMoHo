#include	"Common.h"

#include	"Globals.h"
#include	"Boat.h"

//******************************************************************************************
void	CBoat::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	GET_TARGET;

	// float!
	if (mPos.Z > GAME.GetLevelData()->mGVars[WATER_HEIGHT])
		mPos.Z = GAME.GetLevelData()->mGVars[WATER_HEIGHT];

	if (i->mAngleType == AT_YPR)
	{
		mYaw	= i->mYaw;
		mPitch	= i->mPitch;
		mRoll	= i->mRoll;
	}
	else
	{
		mYaw	= G0;
		mPitch	= G0;
		mRoll	= G0;
	}

//	mDestYaw = invalid value..

	mOriginalZ = mPos.Z;
}

//******************************************************************************************
void	CBoat::Process()
{
	// move?
	if (mTarget)
	{








	}

	// rotate a bit 
	GINT	v = G((GAME.GetGameTurn() + mNumber * 3547) % 20000);

	mPitch = GSIN(v / 23) / 15;
	mRoll  = GCOS(v / 15) / 12;

	mOrientation.MakeRotation(mYaw, mPitch, mRoll);

	// Bob up & down
	mPos.Z = mOriginalZ + GSIN((v / 15)) / 5;  // OK not to use MOVE  (maybe...)

	SUPERTYPE::Process();
}

//********************************************************************************************************
#if TARGET == PC
void	CBoat::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
}
#endif
