// DroidBall.cpp: implementation of the CDroidBall class.
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "DroidBall.h"
#include "DroidBallDispenser.h"
#include "globals.h"
#include "Character.h"
#include "DroidBallPot.h"

#define SLOW_FLASH_TURNS 90
#define FAST_FLASH_TURNS 40

//******************************************************************************************
void	CDroidBall::Init(CInitThing* i)
{
	SUPERTYPE::Init(i) ;
	mBeenPickedUpYet = FALSE;
	mDispensedFrom = NULL;
	mFlashCount = 0 ;
	mLastCharToHoldMe = NULL ;
	mStuckToPoleCounter = 0 ;
	mStuckToPole = FALSE ;
	WORLD.GetDroidBallNB().Add(this) ;
	mAge = 0 ;
	mAgeAfterBeenThrown = 0 ;
}

//******************************************************************************************
void	CDroidBall::Shutdown()
{
	SUPERTYPE::Shutdown() ;
	WORLD.GetDroidBallNB().Remove(this) ;
}

//******************************************************************************************
BOOL	CDroidBall::Pickup(CCharacter *by)
{
	if (mFlags & TF_FLASHING ) return FALSE ;

	if (SUPERTYPE::Pickup(by))
	{
		mFlags &= ~TF_SLOW_FLASHING;
		mAgeAfterBeenThrown = 0;
		if (mBeenPickedUpYet == TRUE )
		{
			mLastCharToHoldMe = by ;
			return TRUE  ;
		}

		mBeenPickedUpYet = TRUE ;
		mLastCharToHoldMe = by ;
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
void	CDroidBall::Drop(HTDropReason reason)
{
	SUPERTYPE::Drop(reason) ;
	if (reason == CHARACTER_WAS_HIT)
	{
		mFlags |= TF_FLASHING;
		mFlashCount=SLOW_FLASH_TURNS ;
	}
}


//******************************************************************************************
SINT	CDroidBall::WhichRTsToRender()
{
	SINT r = 1;

	if (mTrail.IsActive()) r += 2;
	return r;
}

//******************************************************************************************
void	CDroidBall::Process()
{
	mAge++ ;

	SUPERTYPE::Process() ;

#define BLINK_TIME_AFTER_BEEN_THROWN 5 * 25  // 5 seconds

	if (mBeenPickedUpYet == TRUE && IsBeingCarried() == NULL) mAgeAfterBeenThrown++ ;

	// is already flashing or been stuck to the pole for a while or hit ground then
	// start flashing
	if (mBeenPickedUpYet == TRUE &&
		IsBeingCarried() == NULL &&
		!mStuckToPole && mAgeAfterBeenThrown >BLINK_TIME_AFTER_BEEN_THROWN &&
		mFlashCount <SLOW_FLASH_TURNS )
	{
		mFlags |= TF_SLOW_FLASHING;
		mFlashCount++ ;
	}

	if (mFlashCount >= SLOW_FLASH_TURNS ||
		mStuckToPoleCounter > 70 )
	{
		mFlags &= ~TF_SLOW_FLASHING;
		mFlags |= TF_FLASHING;
		mFlashCount++ ;

		// flashed enough ?  die then
		if (mFlashCount > SLOW_FLASH_TURNS+FAST_FLASH_TURNS)
		{
			if (!mStuckToPole)
			{
				mDispensedFrom->StartPrematureDispense() ;
			}
			Die() ;
			return ;
		}
	}

	if (mStuckToPole)
	{
		// make the ball stick to the pole
		Move(mStuckToPolePos) ;
		mVelocity = ZERO_GVECTOR ;
		mStuckToPoleCounter++ ;
	}

	// make the droid ball roll along the ground
	if (IsBeingCarried() == NULL && !mStuckToPole) Roll() ;

	#define SPEED_SQ_WHERE_DROID_BALLS_START_TRACING G(0,21234)*G(0,21234)
	// add trail points
	if (mVelocity.MagnitudeSq() > SPEED_SQ_WHERE_DROID_BALLS_START_TRACING )
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
		mTrail.ProcessAdd(mPos + e0 - mVelocity, mPos - e0 - mVelocity);
	}
	else
		mTrail.ProcessNone();

}


//******************************************************************************************
void	CDroidBall::HitPole(CDroidBallPot* pole)
{
	if (mStuckToPole) return ;

	// score the person who threw us
	if (mLastCharToHoldMe)
	{
		mLastCharToHoldMe->GetLevelData().HitDroidBallPole(this) ;
	}

	// find point where ball should hit
	GVector pole_pos =  pole->GetPos();
	GVector pole_point_up = GVector(pole_pos.X, pole_pos.Y, mPos.Z) ;
	GVector pole_to_ball = mPos - pole_point_up;
	pole_to_ball.Normalise() ;
	pole_to_ball = pole_to_ball * (BALL_POT_POLE_ACTUAL_RADIUS+ Radius());


	mStuckToPolePos  = pole_point_up + pole_to_ball;

	MAP.GetEffectParticleSystem()->MakeSparks(&mStuckToPolePos);
	MAP.GetEffectParticleSystem()->MakeSparks(&mStuckToPolePos);


	if ((pole_pos.Z - G(8)) > mPos.Z )
	{
		mStuckToPolePos = pole_pos ;


		mStuckToPolePos.Z = pole_pos.Z - G(8,22123) ;
	}
	mStuckToPole =TRUE ;

	SOUND.PlaySample(SOUND_GOAL2, this);
}

//******************************************************************************************
ECollideResponse	CDroidBall::GetCollideResponse()
{
	return (mFlags & TF_FLASHING ) ? CR_PASSIVE :CR_BOUNCE;
}
