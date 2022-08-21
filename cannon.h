#ifndef CANNON_H
#define CANNON_H

#include	"Thing.h"

DECLARE_THING_CLASS(CCannon, CThing)
public:
	CCannon() {mOID = OID_NONE;};

	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};

	virtual	BOOL		IsPersistent() {return TRUE;};

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif

	virtual BOOL			DoesThingContainResources() {return TRUE;};

protected:
	GVector	CalcVelocity();

	SINT	mTimer;

	CThing	*mTarget;

	SINT	mOID;
};



#endif