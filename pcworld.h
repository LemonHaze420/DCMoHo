#ifndef PCWORLD_H
#define PCWORLD_H

#include	"World.h"

#include	"TextureAccumulator.h"
#include	"MeshAccumulator.h"
#include	"ThingAccumulator.h"

#include	"TextureAccumulatorDC.h"
#include	"MeshAccumulatorDC.h"
#include	"ThingAccumulatorDC.h"

#define MAX_CACHED_THINGS	128

class	CPCWorld : public CWorld
{
public:
	CPCWorld() {mReading = FALSE; mWriting = FALSE;};


	BOOL	Read(CMEMBUFFER &mem);
	BOOL	Write(CMEMBUFFER &mem, BOOL is_pc);

	SINT	GetWriteRef(CThing *t)
	{
		if (mWriting)
			return mWriteRef[t->mNumber];
		else
			return t->mNumber;
	}

	BOOL	IsReading() {return mReading;};
	
	void	GetThing(SINT no, CThing **t);

	void	RestoreFromCache();
	void	RelocateGlobalMeshes();

#ifdef LINK_EDITOR
	void	AccumulateOIDResources(SINT oid);
	void	AccumulatePSXResources(CTextureAccumulator &ta, CMeshAccumulator &ma, CThingAccumulator &tha);
	void	AccumulateDCResources(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CThingAccumulatorDC &tha);

protected:
	
	BOOL					mOIDScanned[OID_EOL];
	BOOL	mAccumulatingPSX;
	CTextureAccumulator		*mTexA;
	CMeshAccumulator		*mMeshA;
	CThingAccumulator		*mThingA;
	CTextureAccumulatorDC	*mTexADC;
	CMeshAccumulatorDC		*mMeshADC;
	CThingAccumulatorDC		*mThingADC;
#endif

private:
	void	PatchUpReferences();

	SINT	mWriteRef[2048];

	CThing	*mReadRef[2048];
	CThing	**mRelocates[2048];

	BOOL	mReading;
	BOOL	mWriting;
	SINT	mNumRelocates;

	SINT		mNumCachedThings;
	SINT		mCachedIDs[MAX_CACHED_THINGS];
	CInitThing	mCachedInitThings[MAX_CACHED_THINGS];	

};

#endif
