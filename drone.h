#ifndef	DRONE_H
#define	DRONE_H

#include	"Character.h"

DECLARE_THING_CLASS(CDrone, CCharacter)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();
};

#endif