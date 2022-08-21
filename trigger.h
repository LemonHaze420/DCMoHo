#ifndef Trigger_H
#define Trigger_H

#include	"Thing.h"

#define		TRIG_OID		(1 << 0)
#define		TRIG_THING		(1 << 1)

DECLARE_THING_CLASS(CTrigger, CThing)
public:
	CTrigger() {mOID = OID_NONE;};
		
	virtual	void		Init(CInitThing *i);

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		Process();

	virtual	void		ValidateData() {VALIDATE(mTarget); VALIDATE(mSpawnee); SUPERTYPE::ValidateData();};

	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};

	virtual	BOOL		IsPersistent() {return TRUE;};

			void		Action();
			void		UnAction();

	virtual	void		TurnOn()  {};
	virtual	void		TurnOff() {};

	virtual	SINT		GetTriggerTime() {return -1;}; // infinite

#ifdef LINK_EDITOR
	virtual	void		DeclareSpawnees();
#endif
	virtual BOOL			DoesThingContainResources() {return TRUE;};

protected:

	SINT	mOID;
	CThing	*mTarget, *mSpawnee;

	SINT	mType;
	SINT	mTimer;
};

//********************************************************************************
//********************************************************************************

DECLARE_THING_CLASS(CTriggerTile, CTrigger)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Shutdown();

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		Process();

	virtual ECollideShape		GetCollideShape()		{return CSH_SPHERE;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};
	virtual	GINT		Radius() { return G(1, 20000);};

	virtual	void		Hit(CThing *that);

	virtual	void		TurnOn();
	virtual	void		TurnOff();

	virtual	SINT		GetTriggerTime() {return mCounter0 * 25;}; 

			SINT		GetTileType(BOOL on);

	TED_COUNT0("Seconds")
	TED_COUNT1("Type")

protected:
	SINT	mFrame;

	SINT	mCounter0;	// time active
	SINT	mCounter1;	// type
};



//********************************************************************************
//********************************************************************************

DECLARE_THING_CLASS(CTriggerDead, CTrigger)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();

protected:
	BOOL	mTriggered;

};



//********************************************************************************
//********************************************************************************

DECLARE_THING_CLASS(CTriggerSwitch, CTrigger)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();

protected:
	BOOL	mTriggered;

};


//********************************************************************************
//********************************************************************************

DECLARE_THING_CLASS(CTriggerLap, CTrigger)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	
	#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
	#endif

	TED_COUNT0("Lap to trigger")

protected:
	BOOL	mTriggered;

	// lap to trigger
	SINT	mCounter0;
};



//********************************************************************************
//********************************************************************************

DECLARE_THING_CLASS(CTriggerScore, CTrigger)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	
	#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
	#endif

	TED_COUNT0("Score x100")

protected:
	BOOL	mTriggered;

	// score x100 to trigger
	SINT	mCounter0;
};



//********************************************************************************
//********************************************************************************

DECLARE_THING_CLASS(CTriggerOnePrimaryOnHill, CTrigger)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	virtual	SINT		GetTriggerTime() {return mCounter0 * 25;}; 

	TED_COUNT0("Seconds")

		
#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif



protected:

	SINT	mCounter0;	// time active

	
protected:
	BOOL	mTriggered;

};



#endif