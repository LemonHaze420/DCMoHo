#include	"Common.h"

#include	"HoldableThing.h"
#include	"Character.h"
#include	"globals.h"

//******************************************************************************************
void	CHoldableThing::Init(CInitThing *i)
{
	mCarriedBy = NULL;
	SUPERTYPE::Init(i);
	mRoll = G(0) ;
}

//******************************************************************************************
void	CHoldableThing::Shutdown()
{
	SUPERTYPE::Shutdown();

	if (mCarriedBy) Drop() ;
}


//******************************************************************************************
BOOL	CHoldableThing::Pickup(CCharacter *by)
{
	if ((!mCarriedBy) && by)
	{
		mCarriedBy = by;
		return TRUE;
	}
	else
		return FALSE;
}

//******************************************************************************************
void	CHoldableThing::Drop(HTDropReason reason)
{
	if (mCarriedBy != NULL)
	{
		mCarriedBy = NULL;
	}
}


//******************************************************************************************
void	CHoldableThing::Process()
{
	if (mCarriedBy)
		mVelocity = ZERO_GVECTOR;

	SUPERTYPE::Process();

	// make the thing roll a bit in the air if been throw or droped etc.

	if ( AllowedToSpin() && (!(mFlags & TF_ON_GROUND)) &&  !mCarriedBy)
	{
		mRoll+=G(0,12123) ;
		mOrientation.MakeRotationRoll(mRoll) ;
	}
}


//******************************************************************************************
void	CHoldableThing::ValidateData() 
{
	VALIDATE(mCarriedBy);
	SUPERTYPE::ValidateData();
}

//******************************************************************************************
GVector	CHoldableThing::GetRenderPos(SINT n)
{
	if ((n != 0) || (!mCarriedBy))
	{
		return SUPERTYPE::GetRenderPos(n);
	}
	else
	{
		return mCarriedBy->GetCarriedThingRenderPos();
	}
}
