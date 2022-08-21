#ifndef SPAWNER_H
#define SPAWNER_H

#include	"Thing.h"
#include	"OIDs.h"

#include	"Globals.h"

DECLARE_THING_CLASS(CSpawner, CThing)
public:
	CSpawner() {mOID = OID_NONE;};

	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	GINT		Radius() { return GINT_HALF;};

	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	ECollideShape	GetCollideShape()	{return CSH_NONE;};
	virtual BOOL			DoesThingContainResources() {return TRUE;};

protected:
	virtual	SINT		GetSpawnOID() {return OID_CThing;};

	SINT	mOID;
	BOOL	mSpawned;
};

//******************************************************************************************



DECLARE_THING_CLASS(CThingSpawner, CThing)
	public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
		
	TED_COUNT0("Counter 0")
	TED_COUNT1("Counter 1")
	TED_COUNT2("Counter 2")
	TED_COUNT3("Counter 3")


	TED_BOOL0("Bool 0")
	TED_BOOL1("Bool 1")
	TED_BOOL2("Bool 2")
	TED_BOOL3("Bool 3")

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

			void		DeclareSpawnees();

	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Trigger  (CThing *top) ;
	
protected:
	SINT	mOID;

	SINT mCounter0; 
	SINT mCounter1; 
	SINT mCounter2; 
	SINT mCounter3; 

	BOOL	mBool0; 
	BOOL	mBool1; 
	BOOL	mBool2; 
	BOOL	mBool3; 


};



#endif

