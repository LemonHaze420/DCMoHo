#ifndef START_H
#define START_H

#include	"Thing.h"

class CStart : public CThing
{
public:
	GINT	GetYaw() { return mYaw; }
	virtual ECollideShape		GetCollideShape()			{return CSH_NONE;};
	virtual	BOOL				IsPersistent() {return TRUE;}; 

protected:
	BOOL	mStarted;
	GINT	mYaw;

};


DECLARE_THING_CLASS(CStartP1, CStart)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();
};

DECLARE_THING_CLASS(CStartP2, CStart)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();
};




#endif