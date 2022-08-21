#ifndef Wall_H
#define Wall_H

#include	"Actor.h"
#include	"SpringArray.h"

/*
DECLARE_THING_CLASS(CWall, CActor)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Shutdown();

	virtual	void		Process();

	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
	virtual	BOOL				IsPersistent() {return TRUE;};
	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual	BOOL				DoICollideWithMap() {return FALSE;};
	DECLARE_ZERO_GRAVITY;

	virtual	CSpringArray		*GetRenderSpringArray() {return mSpringArray;};

protected:
	CSpringArray	*mSpringArray;
};
*/

#endif