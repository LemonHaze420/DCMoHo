#ifndef Teleport_H
#define Teleport_H

#include	"Thing.h"

DECLARE_THING_CLASS(CTeleport, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	BOOL		IsPersistent() {return TRUE;};

//	virtual ECollideShape		GetCollideShape()		{return CSH_AABB;};
//	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};

	virtual	void		Trigger(CThing *top);
	virtual	void		UnTrigger(CThing *top);
			void		SwitchOnTeleportFlame() ; 
			void		SwitchOffTeleportFlame() ; 

	virtual	GINT		Radius() { return G1;};

	virtual	void		Hit(CThing *that);

	virtual	void		ValidateData() {VALIDATE(mTarget); VALIDATE(mEffect);SUPERTYPE::ValidateData();};

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif

protected:
	CThing	*mTarget;
	CThing	*mEffect;
	BOOL mActivated;
	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;
};

#endif