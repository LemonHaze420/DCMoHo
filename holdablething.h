#ifndef HOLDABLE_THING_H
#define HOLDABLE_THING_H

#include	"Actor.h"

class	CCharacter;
class	CDroidBall;

enum HTDropReason
{
	CHARACTER_WAS_HIT,
	CHARACTER_THREW_IT,
	GOT_SUCKED_IN_BY_DROID_BALL_POST
} ;

DECLARE_THING_CLASS(CHoldableThing, CActor)
public:
	virtual	void		Init(CInitThing *i);
	virtual		CHoldableThing*	IsHoldable() { return this ; }
	virtual		void		Process();
	virtual		void		Shutdown() ;
	virtual		BOOL		DoISeekCollisions() {return !mCarriedBy;};
	virtual		BOOL		DoIHit(CThing *t) {return !mCarriedBy;};
	virtual		BOOL		IsPersistent() {return TRUE;};  //!
	virtual		BOOL		DoICollideWithMap() {return !mCarriedBy;};
	virtual		CDroidBall* IsDroidBall() { return NULL ;} ;
	virtual		BOOL		AllowedToSpin() { return TRUE ; } 
	virtual		ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};
	virtual		GINT		Gravity() {return (mCarriedBy ? G0 : SUPERTYPE::Gravity());};
				BOOL		IsBeingCarried() { return mCarriedBy == NULL ? FALSE : TRUE ; } ;
				CCharacter* BeingCarriedBy() { return mCarriedBy ; }
	virtual		void		ValidateData();
	virtual		BOOL		Pickup(CCharacter *by);
	virtual		void		Drop(HTDropReason = CHARACTER_WAS_HIT );

	virtual		GVector		GetRenderPos(SINT no = 0);

protected:
	CCharacter	*mCarriedBy;
	GINT		mRoll;
};

#endif


