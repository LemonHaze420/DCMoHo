#ifndef PC_RENDER_DATA_H
#define PC_RENDER_DATA_H

#include	"OIDs.h"
#include	"RenderThing.h"

#include	"PCRTIDs.h"
class	CThing;

#define MAX_RENDER_THINGS_PER_THING	4

class	CRenderData
{
public:
	CRenderData() {mNumRenderThings = 1;};
	SINT				mOID;
	SINT				mRTID[MAX_RENDER_THINGS_PER_THING];
	CInitRenderThing	mInit[MAX_RENDER_THINGS_PER_THING];
	SINT				mNumRenderThings;
};

void			InitRenderData();

CRenderData		*GetRenderData(SINT oid);
CRenderThing	*CreateAndGetRenderThing(SINT oid, CThing *t);

#endif