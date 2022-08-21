#include	"Common.h"

#if TARGET == PC

#include	"PCWorld.h"
#include	"Globals.h"
#include	"RenderThing.h"

#include	"PCRenderData.h"

//******************************************************************************************
// Load / save

UINT	WD_VER = (MKID("1.80"));

void	sort_out_id(SINT *i)
{
	//! HACK - turn primaries into StartP1's to save alex time.
	switch (*i)
	{
	case OID_CPrimaryAngel:
	case OID_CPrimaryApostle:
	case OID_CPrimaryBenny:
	case OID_CPrimaryLockdown:
	case OID_CPrimarySophie:
	case OID_CPrimaryDoctor:
		*i = OID_CStartP1;
		break;
	};
}

BOOL	CPCWorld::Read(CMEMBUFFER &mem)
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
				t->Init(&i);
			}

			mReadRef[c0] = t;

			if (mNumCachedThings < MAX_CACHED_THINGS)
			{
				mCachedIDs[mNumCachedThings] = id;
				mCachedInitThings[mNumCachedThings] = i;
				mNumCachedThings ++;
			}
			else
			{
				ASSERT(0);
//				printf("Too Many things in WORLD to cache.\n");
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
extern	BOOL	first_run_through;

void	CPCWorld::RestoreFromCache()
{
	// Instead of reading the things from a membuffer, read them from a cached list of things
	mReading = TRUE;
	mNumRelocates = 0;

	first_run_through = FALSE;

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
BOOL	CPCWorld::Write(CMEMBUFFER &mem, BOOL is_pc)
{
	// write out version
	mem.Write(&WD_VER, 4);

	// Count things.. and build reference array
	CThing *t = mFirstThing;
	SINT	n = 0;

	mWriting = TRUE;

	while (t)
	{
		mWriteRef[t->mNumber] = n;
		if (t->IsPersistent() && ((is_pc) || (!(t->IsPCOnly()))))
			n ++;
		t = t->mNextThing;
	};

	// write number of things
	mem.Write(&n, 4);
	
	// write out things
	t = mFirstThing;

	while (t)
	{
		if (t->IsPersistent() && ((is_pc) || (!(t->IsPCOnly()))))
		{
			SINT	id = t->_GetClassID();
			mem.Write(&id, 4);

			CInitThing	i;

			t->WriteData(i);

			mem.Write(&i, sizeof(i));
		}
		t = t->mNextThing;
	}

	// all done
	mWriting = FALSE;

	return TRUE;
}

//******************************************************************************************

// very odd that this function is here.... but it has different meanings depending if we are currently
// loading a level or not.

// if not, then just return the thing...

// otherwise, store the memory reference, so that we can go back later and patch it up with the proper value...

void	CPCWorld::GetThing(SINT no, CThing **thing)
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
		//! hmm.. not particularly fast...

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
void	CPCWorld::PatchUpReferences()
{
	SINT	c0;

	// ok - go through the list of pointers, and set them to the correct thing

	for (c0 = 0; c0 < mNumRelocates; c0 ++)
	{
		*mRelocates[c0] = mReadRef[SINT(*mRelocates[c0])];
	}
}

//******************************************************************************************
void	CPCWorld::RelocateGlobalMeshes()
{
	// tell the engine which meshes we're using.

	CThing *t = mFirstThing;

	while (t)
	{
		SINT n = t->GetRenderMesh();

		if ((n != -1) && (n != -2))
			ENGINE.AddRelocatedMesh(n);

		t = t->mNextThing;
	};
}

//******************************************************************************************
#ifdef LINK_EDITOR

//	Resource Accumulation
void	CPCWorld::AccumulateOIDResources(SINT oid)
{
	if ((oid == OID_INVALID) || (oid == OID_NONE))
		return;

	if (!(mOIDScanned[oid]))
	{
		mOIDScanned[oid] = TRUE;

		if (mAccumulatingPSX)
		mThingA->AddThing(oid);
		else
			mThingADC->AddThing(oid);
		CRenderThing	*rt = CreateAndGetRenderThing(oid, NULL); //! hope this is ok!

		if (rt)
		{
			CRenderThing	*p = rt;
			while (p)
			{
				if (mAccumulatingPSX)
				p->AccumulateResources(*mTexA, *mMeshA);    // add resources
				else
					p->AccumulateResourcesDC(*mTexADC, *mMeshADC);    // add resources
				p = p->GetNextRenderThing();
			}

			delete rt;
		}

	}
	CThing *t = SpawnThing(oid, FALSE);

	if (t)
	{
		t->DeclareSpawnees();							// and recursively add others

		delete t;
	}
}

//******************************************************************************************
void	CPCWorld::AccumulatePSXResources(CTextureAccumulator &ta, CMeshAccumulator &ma, CThingAccumulator &tha)
{
	// scan through each thing, and add resources
	mTexA	= &ta;
	mMeshA	= &ma;
	mThingA	= &tha;

	SINT	c0;

	// reset the scanned list.
	for (c0 = 0; c0 < OID_EOL; c0 ++)
	{
		mOIDScanned[c0] = FALSE;
	}

	// need to make sure each of the primaries is written out - so add the world!
	CThing	*pt[7];

	pt[0] = SpawnThing(OID_CPrimaryAngel);
	pt[1] = SpawnThing(OID_CPrimaryApostle);
	pt[2] = SpawnThing(OID_CPrimaryBenny);
	pt[3] = SpawnThing(OID_CPrimaryLockdown);
	pt[4] = SpawnThing(OID_CPrimarySophie);
	pt[5] = SpawnThing(OID_CPrimaryDoctor);

	pt[6] = SpawnThing(OID_CGolfArrow);   // and the golf arrow...

	for (c0 = 0; c0 < 7; c0 ++)
	{
		if (pt[c0])
		{
			CInitThing	i;
			pt[c0]->Init(&i);
		}
	}

	CThing	*t = mFirstThing;

	while (t)
	{
		if (!(t->mFlags & TF_RENDER_IN_EDITOR_ONLY))
		{
			if ((!(mOIDScanned[t->_GetClassID()])) || (t->DoesThingContainResources()))
			{
				if (!(mOIDScanned[t->_GetClassID()]))
				{
					mOIDScanned[t->_GetClassID()] = TRUE;

					tha.AddThing(t->_GetClassID());
				}

				CRenderThing	*rt = t->mRenderThing;
				while (rt)
				{
					rt->AccumulateResources(ta, ma);   // add resources
					rt = rt->GetNextRenderThing();
				}

			}
		}
		t->DeclareSpawnees();	// and recursively add others

		t = t->mNextThing;
	}

	// remove the primaries etc..
	for (c0 = 0; c0 < 7; c0 ++)
	{
		if (pt[c0])
			pt[c0]->SuicideNext();			// register thing as invalid					
	}
	ValidateAllData();	// and scan through, deleteing references
	for (c0 = 0; c0 < 7; c0 ++)
	{
		if (pt[c0])
			DestroyThing(pt[c0]);		// whatever..
	}


	// stuff from the player
	CPlayer::DeclareSpawnees();

	// from CWorld
	//! Hmmmmmmmmmmmmmmmmmmm.
//	AccumulateOIDResources(OID_CPrimary);

}

//******************************************************************************************
//******************************************************************************************
void	CPCWorld::AccumulateDCResources(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CThingAccumulatorDC &tha)
{
	// scan through each thing, and add resources
	mTexADC		= &ta;
	mMeshADC	= &ma;
	mThingADC	= &tha;

	SINT	c0;

	// reset the scanned list.
	for (c0 = 0; c0 < OID_EOL; c0 ++)
	{
		mOIDScanned[c0] = FALSE;
	}

	// need to make sure each of the primaries is written out - so add the world!
	CThing	*pt[7];

	pt[0] = SpawnThing(OID_CPrimaryAngel);
	pt[1] = SpawnThing(OID_CPrimaryApostle);
	pt[2] = SpawnThing(OID_CPrimaryBenny);
	pt[3] = SpawnThing(OID_CPrimaryLockdown);
	pt[4] = SpawnThing(OID_CPrimarySophie);
	pt[5] = SpawnThing(OID_CPrimaryDoctor);

	pt[6] = SpawnThing(OID_CGolfArrow);   // and the golf arrow...

	for (c0 = 0; c0 < 7; c0 ++)
	{
		if (pt[c0])
		{
			CInitThing	i;
			pt[c0]->Init(&i);
		}
	}

	CThing	*t = mFirstThing;

	while (t)
	{
		if (!(t->mFlags & TF_RENDER_IN_EDITOR_ONLY))
		{
			if ((!(mOIDScanned[t->_GetClassID()])) || (t->DoesThingContainResources()))
			{
				if (!(mOIDScanned[t->_GetClassID()]))
				{
					mOIDScanned[t->_GetClassID()] = TRUE;

					tha.AddThing(t->_GetClassID());
				}

				CRenderThing	*rt = t->mRenderThing;
				while (rt)
				{
					rt->AccumulateResourcesDC(ta, ma);   // add resources
					rt = rt->GetNextRenderThing();
				}

			}
		}
		t->DeclareSpawnees();	// and recursively add others

		t = t->mNextThing;
	}

	// remove the primaries etc..
	for (c0 = 0; c0 < 7; c0 ++)
	{
		if (pt[c0])
			pt[c0]->SuicideNext();			// register thing as invalid					
	}
	ValidateAllData();	// and scan through, deleteing references
	for (c0 = 0; c0 < 7; c0 ++)
	{
		if (pt[c0])
			DestroyThing(pt[c0]);		// whatever..
	}


	// stuff from the player
	CPlayer::DeclareSpawnees();

	// from CWorld
	//! Hmmmmmmmmmmmmmmmmmmm.
//	AccumulateOIDResources(OID_CPrimary);

}

#endif
//******************************************************************************************

#endif
