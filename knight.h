#ifndef	Knight_H
#define	Knight_H

#include	"BadGuys.h"

DECLARE_THING_CLASS(CKnight, CBadGuy)
public:
	virtual	void	Init(CInitThing *i);
};

#endif