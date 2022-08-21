#ifndef THING_ACCUMULATOR_H
#define THING_ACCUMULATOR_H

#include	"ThingAccumulator.h"
#include	"MeshAccumulator.h"

#include	"OIDs.h"

class	CThingAccumulator
{
public:
	void	Init() {mNumThings = 0;};

	void	AddThing(SINT oid);

	void	CompileAndWrite(CTextureAccumulator &ta, CMeshAccumulator &ma, CMEMBUFFER &mem);

private:
	
	SINT	mThings[OID_EOL];			// can't ever be more than this!
	SINT	mNumThings;
};




#endif