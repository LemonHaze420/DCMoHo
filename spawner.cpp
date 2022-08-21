#include	"Common.h"

#include	"Spawner.h"
#include	"Globals.h"
#include	"Thing.h"

//******************************************************************************************
void	CSpawner::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mOID = GetSpawnOID();
	mSpawned = FALSE;

	SitOnGround();

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;
}

//******************************************************************************************

void	CSpawner::Process()
{
	if (!mSpawned)
	{
		mSpawned = TRUE;

		CThing *t = SpawnThing(mOID);
		if (t)
		{
			CInitThing i;
			i.mPos = mPos;
			t->Init(&i);
		}
	}
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CThingSpawner::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	GET_COUNTER(0);
	GET_COUNTER(1);
	GET_COUNTER(2);
	GET_COUNTER(3);
	GET_BOOL(0);
	GET_BOOL(1);
	GET_BOOL(2);
	GET_BOOL(3);


	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	mOID = OID_NONE ;

	if ((i->mOID != OID_INVALID) && (i->mOID != OID_NONE))
	{
		mOID = i->mOID;
	}

	SitOnGround();
}

//******************************************************************************************
#if TARGET == PC
void	CThingSpawner::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	if (mOID != OID_NONE)
		 i.mOID = mOID;
	else i.mOID = OID_NONE;

	
	WRITE_COUNTER(0);
	WRITE_COUNTER(1);
	WRITE_COUNTER(2);
	WRITE_COUNTER(3);


	WRITE_BOOL(0);
	WRITE_BOOL(1);
	WRITE_BOOL(2);
	WRITE_BOOL(3);

}
#endif

//******************************************************************************************
void	CThingSpawner::Process()
{
	SUPERTYPE::Process();
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CThingSpawner::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	if (mOID !=OID_NONE)
		WORLD.AccumulateOIDResources(mOID);

}
#endif



//******************************************************************************************
void	CThingSpawner::Trigger  (CThing *top) 
{
	CThing* thing = SpawnThing(mOID);
	if (thing)
	{
		CInitThing i;
		i.mPos =GetPos();
		i.mCounters[0] = mCounter0 ;
		i.mCounters[1] = mCounter1 ;
		i.mCounters[2] = mCounter2 ;
		i.mCounters[3] = mCounter3 ;
		i.mBools[0] = mBool0 ;
		i.mBools[1] = mBool1 ;
		i.mBools[2] = mBool2 ;
		i.mBools[3] = mBool3 ;
		thing->Init(&i);
	}
}







