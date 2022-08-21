#ifndef	Boat_H
#define	Boat_H

#include	"Actor.h"

DECLARE_THING_CLASS(CBoat, CActor)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();
	virtual	BOOL		DoICollideWithMap() {return FALSE;};

	virtual ECollideShape		GetCollideShape()		{return CSH_COMPLEX;};
	virtual	ECollideResponse	GetCollideResponse()		{return CR_STATIC;};
	DECLARE_ZERO_GRAVITY;
	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	GINT				Radius() { return GetRenderThingRadius();};

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};

protected:
	CThing	*mTarget;

	GINT	mYaw, mPitch, mRoll;
	GINT	mOriginalZ;

};

DECLARE_THING_CLASS(CBoatViking, CBoat) };
DECLARE_THING_CLASS(CBoatPirate, CBoat) };

#endif