#ifndef Solid_H
#define Solid_H

#include	"Actor.h"

DECLARE_THING_CLASS(CSolid, CActor)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual ECollideShape		GetCollideShape()		{return CSH_OBB;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
	virtual	BOOL				IsPersistent() {return TRUE;};
	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual	BOOL				DoICollideWithMap() {return FALSE;};
	DECLARE_ZERO_GRAVITY;
	virtual	GINT		CoE() {return GINT(0, 10000);};		// coefficient of elasticity
//	virtual	GINT		CoF() {return GINT(0, 20000);};		// coefficient of friction

	virtual	GINT		Radius() { return GetRenderThingRadius();};

protected:
};

#endif