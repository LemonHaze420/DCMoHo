#ifndef DROIDBALL_INCLUDE
#define DROIDBALL_INCLUDE


class CDroidBallDispenser ;
class CDroidBallPot;
class CCharacter;

#include "HoldableThing.h"
#include "Trail.h"

DECLARE_THING_CLASS(CDroidBall, CHoldableThing)

public:
			void	SetDispensedBy(CDroidBallDispenser* d) { mDispensedFrom = d ;}
			void	Init(CInitThing *i) ;
	virtual void				Shutdown();
	virtual BOOL	Pickup(CCharacter *by);
	virtual	void	Process() ;
	virtual	BOOL	AllowedToSpin() { return mStuckToPole == TRUE ? FALSE : TRUE ; }
			BOOL	IsStuckToPole() { return mStuckToPole ; }
	virtual	BOOL	DoISeekCollisions() {return mAge < 15 ? FALSE : TRUE ;};
			SINT	WhichRTsToRender();
	virtual	CDroidBall* IsDroidBall() { return this; } ;
	virtual	CTrail		*GetRenderTrail()		{return &mTrail;};
	virtual		ECollideResponse	GetCollideResponse() ;
			void	HitPole(CDroidBallPot* pole) ;
	virtual GINT	GetMass() { return G(0,8543) ; } ;

	virtual	GINT	Radius() { return GetRenderThingRadius();};
	virtual void	Drop(HTDropReason = CHARACTER_WAS_HIT ) ;

protected:
		BOOL					mBeenPickedUpYet;
		CDroidBallDispenser*	mDispensedFrom;
		SINT					mFlashCount;
		CCharacter*				mLastCharToHoldMe;
		GVector					mStuckToPolePos ;
		BOOL					mStuckToPole ;
		SINT					mStuckToPoleCounter ;
		CTrail					mTrail;
		SINT					mAge;
		SINT					mAgeAfterBeenThrown;
	
};

#endif 