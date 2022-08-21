#ifndef Drawbridge_H
#define Drawbridge_H

#include	"Actor.h"
/*
DECLARE_THING_CLASS(CDrawbridge, CActor)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

//	virtual ECollideShape		GetCollideShape()		{return CSH_COMPLEX;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
	virtual	BOOL				IsPersistent() {return TRUE;};
	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual	BOOL				DoICollideWithMap() {return FALSE;};
	DECLARE_ZERO_GRAVITY;

	virtual	void				Trigger(CThing *top) {mTriggered = TRUE; SUPERTYPE::Trigger(top);};

	virtual	GINT				Radius() { return GetRenderThingRadius();};

protected:
	GINT	mRoll;
	GINT	mRollVelocity;
	BOOL	mTriggered;
	GMatrix	mInitialOrientation;
};
*/
#endif