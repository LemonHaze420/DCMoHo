#ifndef DCWORLD_H
#define DCWORLD_H

#include	"World.h"
#include	"Thing.h"

#define MAX_CACHED_THINGS	128

class	CDCWorld : public CWorld
{
public:
	BOOL	Read(CMEMBUFFER &mem);

	void	GetThing(SINT no, CThing **t);

	BOOL	IsReading() {return mReading;};
	
	void	RestoreFromCache();

private:
	void	PatchUpReferences();

	CThing	*mReadRef[2048];
	CThing	**mRelocates[2048];

	SINT	mNumRelocates;
	BOOL	mReading;
	
	SINT		mNumCachedThings;
	SINT		mCachedIDs[MAX_CACHED_THINGS];
	CInitThing	mCachedInitThings[MAX_CACHED_THINGS];	
};


#endif



