// DroidBall.cpp: implementation of the CDroidBall class.
//
//////////////////////////////////////////////////////////////////////

#include "common.h"

#if 0


#include "Grenade.h"
#include "globals.h"
#include "Character.h"
#include "DroidBallDispenser.h"



//******************************************************************************************
void	CGrenade::Init(CInitThing* i)
{
	SUPERTYPE::Init(i) ;
	mBeenPickedUpYet = FALSE;
	mDispensedFrom = NULL;
	mAge = 0 ;
	mAgeAfterBeenThrown = 0 ;
}


//******************************************************************************************
BOOL	CGrenade::Pickup(CCharacter *by)
{
	if (SUPERTYPE::Pickup(by))
	{
		mBeenPickedUpYet = TRUE ;
		if (mDispensedFrom)
		{
			// kick off the next ball from the depsenser
			mDispensedFrom->StartNextBallTimer() ;
		}
		else
		{
			ASSERT(0) ;
		}

		return TRUE ;
	}
	return FALSE ;
}



//******************************************************************************************
SINT	CGrenade::WhichRTsToRender()
{
	SINT r = 1;

	if (mTrail.IsActive()) r += 2;
	return r;
}

//******************************************************************************************
void	CGrenade::Hit(CThing *that)
{
	if (mBeenPickedUpYet == TRUE && IsBeingCarried() == NULL)
	{
		Explode() ;
	}
}


//******************************************************************************************
void	CGrenade::Explode()
{
	CThing	*t = SpawnThing(OID_CFXExplosion1);
	if (t)
	{
		CInitThing i;
		i.mPos = mPos;
		t->Init(&i);
	}
	MAP.Raindrop(CCellRef(mPos), G(0 , 30000));
	Die() ;
}


//******************************************************************************************
void	CGrenade::Process()
{
	mAge++ ;

	SUPERTYPE::Process() ;

#define FUSE_GAME_TICKS 25

	if (mBeenPickedUpYet == TRUE && IsBeingCarried() == NULL) mAgeAfterBeenThrown++ ;

	// is already flashing or been stuck to the pole for a while or hit ground then
	// start flashing
	if (mBeenPickedUpYet == TRUE && IsBeingCarried() == NULL && mAgeAfterBeenThrown > FUSE_GAME_TICKS )
	{
		Explode() ;
		return ;

	}

	#define SPEED_SQ_WHERE_GRENADE_START_TRACING G(0,21234)*G(0,21234) 
	// add trail points
	if (mVelocity.MagnitudeSq() > SPEED_SQ_WHERE_GRENADE_START_TRACING )
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
		e0=e0*(Radius()*G(0,55000)) ;
		mTrail.ProcessAdd(mPos + e0, mPos - e0);
	}
	else
		mTrail.ProcessNone();

}


#endif