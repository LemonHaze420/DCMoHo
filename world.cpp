#include	"Common.h"
#include	"World.h"
#include	"Actor.h"
#include	"Effector.h"
#include	"Map.h"
#include	"Game.h"
#include	"Globals.h"
#include	"Primary.h"
#include	"HoldingZonePoint.h"
#include	"Profile.h"

//******************************************************************************************
//#define	WORLD_GRAVITY	0.05f;
#define	WORLD_GRAVITY	GINT(0, 1966)
//#define	WORLD_GRAVITY	0.01f;

#define	FOLLOW_DISTANCE G(4)

//******************************************************************************************
BOOL	CWorld::Init()
{
	mFirstThing = NULL;

	SINT	c0;
	for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
		mPrimaryThings[c0] = NULL;
	
	mThingNumber = 0;

	mPrimaryFollowPoint = GVector(G1, G1, G0);
	mFinishPos = GVector(G(32), G(32), -G(1000)); // out of the way somewhere!

	// clear particle systems
	for (c0 = 0; c0 < NUM_PARTICLE_SYSTEMS; c0 ++)
	{
		mParticleSystems[c0].DeActivate();
		mParticleSystems[c0].mOwner = NULL;
	}

	// golf stuff
	mGolfArrow = NULL;


	//!Clear lists?????????????/

	return TRUE;
}

//******************************************************************************************
void	CWorld::Shutdown()
{
	while (mFirstThing)
	{
		CThing *t = mFirstThing;
		mFirstThing = mFirstThing->mNextThing;
		DestroyThing(t);
	};

	SINT	c0;
	for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)

		mPrimaryThings[c0] = NULL;

	mGolfArrow = NULL;
}

//******************************************************************************************
void	CWorld::Reset()
{
	Shutdown();
	Init();
}

//******************************************************************************************
void	CWorld::AddDefaultObjects()
{
}

//******************************************************************************************
void	CWorld::AddThing(CThing *t)
{
	ASSERT(t);

	// bizarre  - if a thing seeks collisions, put it at the end of the list, otherwise add to the front...
	if ((t->DoISeekCollisions()) && (mFirstThing)) // if 0 things, do other half
	{
		CThing *p = mFirstThing;
		while (p->mNextThing)
			p = p->mNextThing;

		p->mNextThing = t;
		t->mNextThing = NULL;
		t->mPrevThing = p;
	}
	else
	{
		if (mFirstThing)
			mFirstThing->mPrevThing = t;
		t->mNextThing = mFirstThing;
		t->mPrevThing = NULL;
		mFirstThing = t;
	}

	t->mNumber = mThingNumber++;  // give unique number
}

//******************************************************************************************
void	CWorld::RemoveThing(CThing *t)
{
	if (t->mPrevThing)
		t->mPrevThing->mNextThing = t->mNextThing;
	else
		mFirstThing = t->mNextThing;

	if (t->mNextThing)
		t->mNextThing->mPrevThing = t->mPrevThing;
		
}

//******************************************************************************************
void	CWorld::Process()
{
	if (!mFirstThing)
	{
		// just loaded off disk with no objects - so add a few...
		AddDefaultObjects();
	}

	if ((GAME.GetGameTurn() > 5) && (!mPrimaryThings[0]))
	{
		// oops - better create a default one...
		CThing	*th = SpawnThing(OID_CPrimary);
		if (th)
		{
			CInitThing i;
			i.mPos = GVector(G(8), G(8), G(-3));
			th->Init(&i);
		}

	}

	// Process things
	CThing *t, *u;

	//! This should go!
	t = mFirstThing;
	while (t)
	{
		// clear some flags
		t->mFlags &= ~(TF_HIT | TF_HIT_CHECK);
		t = t->mNextThing;
	};

	t = mFirstThing;
	while (t)
	{
		PROFILE_FN(ThingProcess);
		{
			if (t->mFlags & TF_SUICIDE)
			{
				ASSERT(!(t->mFlags & TF_DEBUG_SUICIDE)); // ensure suicide is not being set at the wrong time...
				// kill thing
				u = t->mNextThing;
				DestroyThing(t);
				t = u;
			}
			else
			{
				t->ValidateData();
				t->Process();
				t = t->mNextThing;
			}
		}
	};

	// primary related functions
	SINT	c0;

	for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
	{
		VALIDATE(mPrimaryThings[c0]); // check it's still there
		if (mPrimaryThings[c0])
		{
//			// have we fallen off the map?
//			if (MAP.FallenOffMap(mPrimaryThings[c0]->GetPos()))
//				GAME.ChangeGameState(GAME_STATE_FALLEN_OFF_MAP);

			// Process Primary follow point...
			if (c0 == 0)
				mPrimaryFollowPoint = (mPrimaryFollowPoint - mPrimaryThings[0]->mPos).NormaliseAndScale(FOLLOW_DISTANCE) + mPrimaryThings[c0]->mPos;

		}
	}
}

//******************************************************************************************
void	CWorld::DestroyThing(CThing *t)
{
	if (t)
	{
		t->Shutdown();
		delete t;
	}
}

//******************************************************************************************
GINT	CWorld::Gravity()
{
	return WORLD_GRAVITY;
}

//******************************************************************************************
GVector CWorld::GetPrimaryPosition(SINT n)
{
	return mPrimaryThings[n] ? mPrimaryThings[n]->mPos : GVector(G(16), G(16), G0);
}

//******************************************************************************************
GVector CWorld::GetPrimaryFollowPoint()
{
	return mPrimaryFollowPoint;
}

//******************************************************************************************
void	CWorld::ValidateAllData()
{
	// first, the WORLD stuff
	SINT	c0;

	for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
		VALIDATE(mPrimaryThings[c0]);

	CThing *t = mFirstThing;

	while (t)
	{
		t->ValidateData();
		t = t->mNextThing;
	};
}

//******************************************************************************************
//******************************************************************************************

CParticleSystem	*CWorld::GetParticleSystem()
{
	// try to find a free particle system

	SINT c0;

	for (c0 = 0; c0 < NUM_PARTICLE_SYSTEMS; c0 ++)
	{
		if (!(mParticleSystems[c0].IsActive()))
		{
			mParticleSystems[c0].Activate();
			mParticleSystems[c0].mOwner = NULL;   // just make sure....
			return &mParticleSystems[c0];
		}
	}

	return NULL;
}

//******************************************************************************************
void	CWorld::NextPrimary()
{
	// cycle through primary balls

	//! yeah

	PLAYER(0)->RecentreCursor();
}

//******************************************************************************************
CPlayer	*CWorld::GetPlayerForPrimary(CPrimary *p)
{
	//! should be more cunning

/*	SINT	c0;
	for (c0 = 0; c0 < MAX_PLAYERS; c0 ++)
	{
		if ((PLAYER(c0)->IsActive()) && ((!mPrimaryThings[c0]) || (!(mPrimaryThings[c0]->IsValid()))))
			return PLAYER(c0);
	}

	return PLAYER(0);*/

	SINT	id = p->_GetClassID();
	SINT	pr_char = -1;

	switch (id)
	{
	case OID_CPrimaryAngel:		pr_char = 0;	break;
	case OID_CPrimaryApostle:	pr_char = 1;	break;
	case OID_CPrimaryBenny:		pr_char = 2;	break;
	case OID_CPrimaryLockdown:	pr_char = 3;	break;
	case OID_CPrimarySophie:	pr_char = 4;	break;
	case OID_CPrimaryDoctor:	pr_char = 5;	break;
	}

	if (pr_char != -1)
	{
		if (GAME.GetPlayerCharacter(0) == pr_char)
			return PLAYER(0);
		if (GAME.GetPlayerCharacter(1) == pr_char)
			return PLAYER(1);
	}

	return PLAYER(0);  // hmm....
}

//******************************************************************************************
//******************************************************************************************
// Golf stuff

void	CWorld::GolfArrowEnable()
{
	// create an arrow.
	if (!mGolfArrow)
	{
		mGolfArrow = SpawnThing(OID_CGolfArrow);

		if (mGolfArrow)
		{
			CInitThing	i;
			i.mPos = ZERO_GVECTOR;
			mGolfArrow->Init(&i);
		}
	}
	else
	{
//		ASSERT(0);  // shouldn't happen
	}
}

//******************************************************************************************
void	CWorld::GolfArrowDisable()
{
	if (mGolfArrow)
	{
		mGolfArrow->SuicideNext();
		mGolfArrow = NULL;
	}
	else
	{
		ASSERT(0); // shouldn't happen??
	}
}



//******************************************************************************************
CHoldingZonePoint*	CWorld::LocatePrimaryHoldingZonePoint(SINT for_holding_zone_point_num)
{
	for (CHoldingZonePoint* current = mHoldingZonePointsNB.First() ;
							current != NULL ;
							current = mHoldingZonePointsNB.Next() )
	{
		if (current->GetHoldingZoneNumber() == for_holding_zone_point_num &&
			current->GetTarget() )
		{
			return current ;
		}
	}
	ASSERT(0) ;
	return NULL;
}



