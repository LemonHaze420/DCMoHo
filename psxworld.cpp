#include	"Common.h"

#if TARGET == PSX

#include	"PSXWorld.h"
#include	"Globals.h"

//******************************************************************************************
// Load / save

UINT	WD_VER = (MKID("1.80"));

BOOL	CPSXWorld::Read(CMEMBUFFER &mem)
{
		// get version number
	UINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson
	if (ver == MKID("1.80"))
	{
		SINT	i;

		mNumCachedThings = 0;

		mReading = TRUE;
		mNumRelocates = 0;

		// read number of things
		mem.Read(&i, 4);

		SINT	c0;
		for (c0 = 0; c0 < i; c0 ++)
		{
			SINT id;
			mem.Read(&id, 4);

			CInitThing i;
			mem.Read(&i.mPos, sizeof(i.mPos));
			mem.Read(&i.mVelocity, sizeof(i.mVelocity));
			mem.Read(&i.mAngleType, sizeof(i.mAngleType));
			mem.Read(&i.mOrientation, sizeof(i.mOrientation));
			mem.Read(&i.mYaw, sizeof(i.mYaw));
			mem.Read(&i.mPitch, sizeof(i.mPitch));
			mem.Read(&i.mRoll, sizeof(i.mRoll));
			mem.Read(&i.mOID, sizeof(i.mOID));
			mem.Read(&i.mTarget, sizeof(i.mTarget));
			mem.Read(&i.mChainTrigger, sizeof(i.mChainTrigger));
			mem.Read(&i.mInitialMode, sizeof(i.mInitialMode));
			mem.Read(&i.mTriggerResponse, sizeof(i.mTriggerResponse));
			mem.Read(&i.mUnTriggerResponse, sizeof(i.mUnTriggerResponse));
			mem.Read(&i.mCounters, sizeof(SINT) * NUM_COUNTERS);
			mem.Read(&i.mBools, sizeof(BOOL) * NUM_BOOLS);
			mem.Read(&i.mMeshNo, sizeof(i.mMeshNo));

			CThing *t = SpawnThing(id);
			if (t)
			{
//				printf("Initialising thing %d\n", id);
				t->Init(&i);
			}

			mReadRef[c0] = t;
			
			// cache the thing
			if (mNumCachedThings < MAX_CACHED_THINGS)
			{
				mCachedIDs[mNumCachedThings] = id;
				mCachedInitThings[mNumCachedThings] = i;
				mNumCachedThings ++;
			}
			else
			{
				ASSERT(0);
				printf("Too Many things in WORLD to cache.\n");
			}
				
		}

		mReading = FALSE;

		// patch up references
		PatchUpReferences();

		return TRUE;
	}
	else return FALSE; // didn't recognise the version
}

//******************************************************************************************
void	CPSXWorld::RestoreFromCache()
{
	// Instead of reading the things from a membuffer, read them from a cached list of things
	SINT	i;

	mReading = TRUE;
	mNumRelocates = 0;

	SINT	c0;
	for (c0 = 0; c0 < mNumCachedThings; c0 ++)
	{
		CThing *t = SpawnThing(mCachedIDs[c0]);
		if (t)
		{
			t->Init(&mCachedInitThings[c0]);
		}

		mReadRef[c0] = t;
	}

	mReading = FALSE;

	// patch up references
	PatchUpReferences();
}


//******************************************************************************************

// very odd that this function is here.... but it has different meanings depending if we are currently
// loading a level or not.

// if not, then just return the thing...

// otherwise, store the memory reference, so that we can go back later and patch it up with the proper value...

void	CPSXWorld::GetThing(SINT no, CThing **thing)
{
	if (mReading)
	{
		if (no >= 0)
		{
			mRelocates[mNumRelocates ++] = thing;
			*thing = (CThing *)no;
		}
		else
			*thing = NULL;
	}
	else
	{
		CThing *t = mFirstThing;

		while (t)
		{
			if (t->mNumber == no)
			{
				*thing = t;
				return;
			}

			t = t->mNextThing;
		}

		*thing = NULL;
	}
}

//******************************************************************************************
void	CPSXWorld::PatchUpReferences()
{
	SINT	c0;

	// ok - go through the list of pointers, and set them to the correct thing

	for (c0 = 0; c0 < mNumRelocates; c0 ++)
	{
		*mRelocates[c0] = mReadRef[SINT(*mRelocates[c0])];
	}
}



#endif
