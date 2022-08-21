#ifndef	Clown_H
#define	Clown_H

#include	"BadGuys.h"

DECLARE_THING_CLASS(CClown, CBadGuy)
public:
	virtual	void	Init(CInitThing *i);

	virtual void	Process();
	virtual	void	StateNormal();

#ifdef LINK_EDITOR
	virtual	void	DeclareSpawnees();
#endif

	virtual	SINT		GetTorsoID() {return OID_CClownTorso;};
};

#endif