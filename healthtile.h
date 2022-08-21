#ifndef HEALTH_TILE_H
#define HEALTH_TILE_H

#include	"Thing.h"


DECLARE_THING_CLASS(CHealthTile, CThing)
public:

	virtual	void		Init(CInitThing *i);
	virtual ECollideShape		GetCollideShape()		{return CSH_SPHERE;};
	virtual GINT		Radius() { return G(2) ; } 
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};
	virtual	void		Hit(CThing *that);
	virtual void		Shutdown() ;

	virtual	void		Process();

	virtual	BOOL		IsPersistent() {return TRUE;};

protected:
	SINT	mOnTimer;

};

#endif
