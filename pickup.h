#ifndef Pickup_H
#define Pickup_H

#include	"Actor.h"

DECLARE_THING_CLASS(CPickup, CActor)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};
	virtual	void		Hit(CThing *that);
	virtual	void		DoPickup(CThing *that);

	virtual	BOOL		IsPersistent() {return TRUE;};
//	virtual	BOOL		DoIHit(CThing *t) {return t->DoIAcceptPickups();};

	virtual	GINT		Radius() {return G(0, 50000);};

	virtual	BOOL		AmILimited() {return TRUE;};
	virtual	SINT		InitialLife() {return 200;};
	virtual EPickupType PickupType() { return PUP_AVERAGE ; };

	virtual	SINT		GetSample();

protected:
	GINT	mYaw;

};

//******************************************************************************************
//******************************************************************************************
// interesting...

DECLARE_THING_CLASS(CPickupStatic, CPickup)
public:
	DECLARE_ZERO_GRAVITY;
	virtual	BOOL		DoICollideWithMap() {return FALSE;};
	virtual	BOOL		DoIHit(CThing *t) {return t->DoIAcceptPickups();};
	virtual	BOOL		DoISeekCollisions() {return FALSE;};

protected:
	GINT	mYaw;
};

//******************************************************************************************
//******************************************************************************************

DECLARE_THING_CLASS(CPUPBrake, CPickup)
public:
	virtual EPickupType PickupType() { return PUP_BRAKE ; };
};

DECLARE_THING_CLASS(CPUPHealth, CPickup)
public:
	virtual EPickupType PickupType() { return PUP_HEALTH ; };
};

DECLARE_THING_CLASS(CPUPToken, CPickupStatic)
public:
	virtual	void		Init(CInitThing *i);
	virtual void		SuicideNext()	{ DeclarePickedUp(); SUPERTYPE::SuicideNext();} ;
	virtual void		Shutdown()		{ DeclarePickedUp(); SUPERTYPE::Shutdown() ; }

	virtual	GINT		Radius() {return G(1, 20000);};

	virtual	GINT		GetRenderRadius() {return G(0, 32000);};

	virtual EPickupType PickupType() { return PUP_TOKEN ; };
	virtual	void		DoPickup(CThing *that);
	virtual	BOOL		AmILimited() {return FALSE;};
	virtual void		Process() ;
	static	SINT		GetNumberTokensInWorld() { return  mTokensInWorld ; } ;
	virtual	SINT		WhichRTsToRender() { return mInvisible == FALSE ? 1 : 0 ;};
	virtual	SINT		GetSample();

	virtual ECollideShape		GetCollideShape()
	{
		return mInvisible ? CSH_NONE : SUPERTYPE::GetCollideShape();
	}

	void		DeclarePickedUp()
	{
		if (mListedInStatic)
		{
			mTokensInWorld--;
			mListedInStatic = FALSE;
		}
	}


	#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
	#endif


	TED_COUNT0("Regen secs(MP)")

private:
	static	SINT mTokensInWorld ;
	BOOL	mInvisible;
	SINT	mReGenTime;
	SINT	mCounter0;
	BOOL	mListedInStatic;
};
#endif