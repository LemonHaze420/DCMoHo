#ifndef	Vampire_H
#define	Vampire_H

#include	"BadGuys.h"
#include	"Nurbling.h"

DECLARE_THING_CLASS(CVampire, CBadGuy)
public:
	virtual	void	Init(CInitThing *i);

	virtual	void	StateNormal();

	virtual	void			Hit(CThing *that);

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};

protected:
	CNurbling	*mTarget;

};

#endif