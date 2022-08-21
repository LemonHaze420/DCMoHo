#ifndef FlameThrower_H
#define FlameThrower_H

#include	"Thing.h"

DECLARE_THING_CLASS(CFlameThrower, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Trigger(CThing *top);
	virtual	void		UnTrigger(CThing *top);
			void		ActivateFlameThrower() ;
			void		Aim() ;
			void		DeActivateFlameThrower() ;
	virtual ECollideShape		GetCollideShape()		{return mMoving == FALSE ? CSH_OBB2 : CSH_OBBMOVING2;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
	virtual	GINT		RadiusForMapWho() {return Radius()*4;};

	virtual	void		ValidateData() {VALIDATE(mFlamer); SUPERTYPE::ValidateData();};

	virtual	BOOL			RenderUseHierarchy(SINT pn)	{return (pn != 1);};

	virtual	GMatrix			GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 1:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	};

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

protected:
			void		CreateFlamer();

	CThing	*mFlamer;
	GINT	mYaw;

	SINT	mCounter0, mCounter1, mCounter2;
	BOOL	mBool0,  mBool1, mBool2;

	SINT	mTimer;
	BOOL mActivated;
	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;
	SINT mStartBurstNumber ;
	BOOL mMoving ;

	GMatrix	mOrientation2;
};

DECLARE_THING_CLASS(CFlameThrowerGround, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};

	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Trigger(CThing *top);
	virtual	void		UnTrigger(CThing *top);
	
	virtual	void		ValidateData() {VALIDATE(mFlamer); SUPERTYPE::ValidateData();};

	virtual	BOOL		RenderUseHierarchy(SINT pn)	{return (pn != 1);};
			void		ActivateFlameThrowerGround();
			void		DeActivateFlameThrowerGround();

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

#ifdef LINK_EDITOR
	virtual	void		DeclareSpawnees();
#endif

	TED_COUNT0("Burst Speed")
	TED_COUNT1("Burst Duration")

protected:
			void		CreateFlamer();

	CThing	*mFlamer;

	SINT	mCounter0, mCounter1;
	SINT	mStartBurstNumber ;


	SINT	mTimer;
	BOOL mActivated;
	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;
};


#endif