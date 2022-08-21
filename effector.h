#ifndef Effector_H
#define Effector_H

#include	"Thing.h"
#include	"World.h"

DECLARE_THING_CLASS(CEffector, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	virtual	void		Activate() = 0;

	virtual	BOOL		GetRenderSelected();

	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	BOOL		CanIBeActivated() {return TRUE;};

	virtual	void		Trigger(CThing *top) {Activate(); SUPERTYPE::Trigger(top);};

protected:
	GINT	mYaw;
};

DECLARE_THING_CLASS(CEFRainDrop, CEffector)
public:
	virtual	void		Activate();
};

DECLARE_THING_CLASS(CEFLineWavePair, CEffector)
public:
	virtual	void		Init(CInitThing *i);
#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};

	virtual	void		Activate();
	virtual	void		Process();

	virtual	SINT		GetRenderColourOffset()	{ return 8;};

	virtual	CThing		*GetTarget() {return mTarget;};
	virtual	void		SetTarget(CThing *t) {mTarget = t; mType = LWP_SECONDARY;}; //security - in case both have targets...

	virtual ECollideShape		GetCollideShape()		{return CSH_AABB;};
	virtual	GINT				Radius() { return GetRenderThingRadius();};

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif

protected:
	SINT	mActive;
	SINT	mDelayCounter;
	SINT	mDistance;

	CThing	*mTarget;
	enum
	{
		LWP_NOT_INITIALISED,
		LWP_PRIMARY,
		LWP_SECONDARY,
	}	mType;
};

DECLARE_THING_CLASS(CEFLineWavePair2, CEffector)
public:
	virtual	void		Init(CInitThing *i);
#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};

	virtual	void		Activate();
	virtual	void		Process();
	virtual	void		UnTrigger(CThing *top);

	virtual	SINT		GetRenderColourOffset()	{ return 8;};

	virtual	CThing		*GetTarget() {return mTarget;};
	virtual	void		SetTarget(CThing *t) {mTarget = t; mType = LWP_SECONDARY;}; //security - in case both have targets...

	virtual ECollideShape		GetCollideShape()		{return CSH_AABB;};
	virtual	GINT				Radius() { return GetRenderThingRadius();};

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif

protected:
	SINT	mActive;
	SINT	mDelayCounter;
	SINT	mDistance;
	SINT	mTriggerOnFrame;

	BOOL	mActivated;

	CThing	*mTarget;
	enum
	{
		LWP_NOT_INITIALISED,
		LWP_PRIMARY,
		LWP_SECONDARY,
	}	mType;

	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;

};

#endif