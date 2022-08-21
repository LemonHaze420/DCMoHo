#ifndef GUN_TURRET_H
#define GUN_TURRET_H

#include	"Thing.h"

DECLARE_THING_CLASS(CGunTurret, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Trigger(CThing *top);
	virtual	void		UnTrigger(CThing *top);
	virtual ECollideShape		GetCollideShape()		{return mMoving==FALSE ? CSH_OBB2: CSH_OBBMOVING2;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};

	virtual void		Aim();
			void		ActivateGunTurret();
	virtual	GINT		RadiusForMapWho() {return Radius()*4;};
			void		GetPrimaryTarget() ;

	virtual	void		Shoot();

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif

	TED_COUNT0("Burst Speed")
	TED_COUNT1("Burst Duration")
	TED_COUNT2("Accuracy")
	TED_BOOL0("Spin?")
	TED_BOOL1("Clockwise?")
	TED_BOOL2("Track Player?")
	TED_BOOL3("Projectile A?")

	virtual	BOOL			RenderUseHierarchy(SINT pn)	{return (pn != 1);};

	virtual	GMatrix			GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 1:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	};

protected:

	GINT	mYaw, mPitch;

	SINT	mCounter0, mCounter1, mCounter2;
	BOOL	mBool0,  mBool1, mBool2, mBool3;

	SINT	mTimer;

	GMatrix	mOrientation2;

	BOOL mActivated;
	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;
	SINT mStartBurstNumber ;
	BOOL mMoving ;
	CThing*	mTarget ;
	BOOL	mFireAtWayPoint ;

};

DECLARE_THING_CLASS(CCannon2, CGunTurret)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Shoot();
	virtual void		Aim();

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

#ifdef LINK_EDITOR
	virtual	void		DeclareSpawnees();
#endif
	virtual	GINT		RadiusForMapWho() {return Radius()*4;};
	virtual ECollideShape		GetCollideShape()			{return CSH_AABB;};
	virtual	ECollideResponse	GetCollideResponse()		{return CR_STATIC;};
	virtual	GMatrix			GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 2:		return mOrientation3;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	};

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};
	virtual BOOL			DoesThingContainResources() {return TRUE;};

protected:
	SINT	mOID;
	GVector	mCachedVelocity;
	GMatrix	mOrientation3;

};


#endif