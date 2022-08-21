#ifndef THING_ACCUMULATOR_DC_H
#define THING_ACCUMULATOR_DC_H

#include	"TextureAccumulatorDC.h"
#include	"MeshAccumulatorDC.h"

#include	"OIDs.h"

class	CThingAccumulatorDC
{
public:
	void	Init() {mNumThings = 0;};

	void	AddThing(SINT oid);

	void	CompileAndWrite(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CMEMBUFFER &mem);

private:
	
	SINT	mThings[OID_EOL];			// can't ever be more than this!
	SINT	mNumThings;
};




#endif