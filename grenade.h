#ifndef GRENADE_INCLUDE
#define GRENADE_INCLUDE

#if 0

class CBallDispenser ;

#include "HoldableThing.h"
#include "Trail.h"

DECLARE_THING_CLASS(CGrenade, CHoldableThing)

public:
			void	SetDispensedBy(CBallDispenser* d) { mDispensedFrom = d ;}
			void	Init(CInitThing *i) ;
	virtual BOOL	Pickup(CCharacter *by);
	virtual	void	Process() ;
	virtual	BOOL	DoISeekCollisions() {return mAge < 15 ? FALSE : TRUE ;};
			SINT	WhichRTsToRender();
	virtual	CTrail		*GetRenderTrail()		{return &mTrail;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_BOUNCE;};
	virtual GINT	GetMass() { return G(0,8543) ; } ;
	virtual	GINT	Radius() { return GetRenderThingRadius();};
	virtual void	Hit(CThing *that);
			void	Explode() ;

protected:
		BOOL					mBeenPickedUpYet;
		CBallDispenser*			mDispensedFrom;
		CTrail					mTrail;
		SINT					mAge;
		SINT					mAgeAfterBeenThrown;
	
};

#endif

#endif 