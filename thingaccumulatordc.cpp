#include	"Common.h"

#ifdef LINK_EDITOR

#include	"ThingAccumulatorDC.h"
#include	"PCRenderData.h"
#include	"PSXRenderData.h"
#include	"RenderThing.h"

//******************************************************************************************
void	CThingAccumulatorDC::AddThing(SINT oid)
{
	if (oid < 0) 
	{
		ASSERT(0);
		return;
	}

	if (mNumThings >= OID_EOL)
		return;			// shouldn't really happen..

	SINT	c0;

	// scan through to check we've not already go this

	for	 (c0 = 0; c0 < mNumThings; c0 ++)
	{
		if (mThings[c0] == oid)
			return;
	}

	mThings[mNumThings] = oid;
	mNumThings ++;
}

//******************************************************************************************
void	CThingAccumulatorDC::CompileAndWrite(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CMEMBUFFER &mem)
{
	SINT			master_index[MAX_OIDS];  //!PSX can probably store this as SWORD or maybe a UBYTE??
	SPSXRenderData	rd;

	// write out the header
	UINT	CID_RTID	= MKID("RTID");
	UINT	RTID_VER	= (MKID("1.00"));

	mem.Write(&CID_RTID, 4);	// write out chunk header
	mem.Write(&RTID_VER, 4);	// write out verson number

	mem.Write(&mNumThings, 4);  // write out number of thing ids

	SINT	c0;

	// clear the index
	for (c0 = 0; c0 < MAX_OIDS; c0 ++)
		master_index[c0] = -1;

	// fill in the index
	for (c0 = 0; c0 < mNumThings; c0 ++)
		master_index[mThings[c0]] = c0;

	// write out the index
	mem.Write(master_index, sizeof(master_index));

	//*****************************
	// compile RenderData structures

	for (c0 = 0; c0 < mNumThings; c0 ++)
	{
		rd.mNumRenderThings = 0;

		// scan through each renderthing associated with the oid
		// have to scan through the PCRenderData stuff as well.....

		CRenderThing	*rt = CreateAndGetRenderThing(mThings[c0], NULL);
		CRenderData		*pcrd = GetRenderData(mThings[c0]);

		if (!pcrd)
		{
			//! do something like a default decal
			delete rt;
		}
		else
		{
			if (rt)
			{
				CRenderThing	*p = rt;
				while (p)
				{
					rd.mRTIDs[rd.mNumRenderThings] = pcrd->mRTID[rd.mNumRenderThings];
					rd.mRTInfos[rd.mNumRenderThings].Clear();
					p->FillOutDCRenderInfo(rd.mRTInfos[rd.mNumRenderThings], ta, ma, pcrd->mInit[rd.mNumRenderThings]);

					rd.mNumRenderThings ++;
					p = p->GetNextRenderThing();
				}

				delete rt;
			}
		}

		mem.Write(&rd, sizeof(rd));
	}
}

#endif
