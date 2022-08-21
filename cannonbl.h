#ifndef	CannonBall_H
#define	CannonBall_H

#include	"Actor.h"

DECLARE_THING_CLASS(CCannonBall, CActor)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	virtual	GINT		Radius() { return G1;};

	virtual	BOOL		DoICollideWithMap() {return FALSE;};

#ifdef LINK_EDITOR
	virtual	void		DeclareSpawnees();
#endif

	virtual	BOOL		DoIHit(CThing *t) {return (t->IsCharacter() != NULL);};

protected:
	BOOL	mLanded;

};

#endif