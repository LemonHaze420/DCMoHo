#ifndef Spikes_H
#define Spikes_H

#include	"Thing.h"
#include	"World.h"

DECLARE_THING_CLASS(CSpikes, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Hit(CThing *that);
};
/*
DECLARE_THING_CLASS(CSpikyDeath, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Hit(CThing *that);
};*/


enum EStingerMovement
{
	STINGER_NOT_MOVING,
	STINGER_MOVING_UP,
	STINGER_MOVING_DOWN,
};


DECLARE_THING_CLASS(CStinger, CThing) 
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	virtual void		Trigger(CThing *top) ;
	virtual	void		UnTrigger(CThing *top);
	virtual	BOOL		IsPersistent() {return TRUE;};
			void		MoveStingerUp() ;
			void		MoveStingerDown() ;
			void		ActivateStinger() ;
			void		DeActivateStinger() ;

	virtual	void		Hit(CThing *that);
	virtual	SINT		WhichRTsToRender()  ;
	virtual ECollideShape		GetCollideShape()			{return mSpikeAnim == 4 ?  CSH_SPHERE: CSH_NONE;}

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	
	TED_COUNT0("Burst Speed")
	TED_COUNT1("Burst Duration")
	TED_COUNT2("Offset") ;

protected:
	SINT mSpikeAnim ;
	SINT mSpikeAnimCount ;
	EStingerMovement mStingerMovement ;
	SINT	mCounter0, mCounter1, mCounter2;
	SINT	mStartBurstNumber ;

	BOOL				mActivated;
	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;


};

#endif