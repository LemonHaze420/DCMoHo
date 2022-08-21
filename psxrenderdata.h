#ifndef PSX_RENDER_DATA_H
#define PSX_RENDER_DATA_H

// ** NB this file is in the PC Project as well!

#include	"GlobalRenderIDs.h"

// generic structure that each render thing can use to retrieve data
struct	SRTInfo
{
	// OK, OK - so this should be a class.. shuddup already.
	void	Clear()
	{
		i0 = i1 = i2 = i3 = 0;

		mShadow = ST_NONE;
		mWaveResponse = WR_NONE;
	};

	SINT	i0;
	SINT	i1;
	SINT	i2;
	SINT	i3;

	EShadowType		mShadow;
	EWaveResponse	mWaveResponse;
};

struct	SPSXRenderData
{
	// How to render an object

	SINT	mNumRenderThings;
	SINT	mRTIDs[MAX_RENDER_THINGS_PER_THING];
	SRTInfo	mRTInfos[MAX_RENDER_THINGS_PER_THING];
};



#endif