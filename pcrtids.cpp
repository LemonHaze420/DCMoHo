#include	"Common.h"

#if TARGET == PC

#include	"PCRTIDs.h"

#include	"RenderThing.h"
#include	"RTBox.h"
#include	"RTDecal.h"
#include	"RTLightning.h"
#include	"RTMesh.h"
#include	"RTSpikes.h"
#include	"RTSpringMesh.h"
#include	"RTFlag.h"
#include	"RTParticleSystem.h"
//#include	"RTNurbling.h"
#include	"RTFence.h"
#include	"RTTrail.h"
#include	"RTLaser.h"
#include	"RTLink.h"

CRenderThing	*SpawnRenderThing(SINT id)
{
	switch (id)
	{
	case RTID_CRTBox:				return new CRTBox;
	case RTID_CRTDecal:				return new CRTDecal;
	case RTID_CRTLightning:			return new CRTLightning;
	case RTID_CRTMesh:				return new CRTMesh;
	case RTID_CRTSpikes:			return new CRTSpikes;
	case RTID_CRTSpringMesh:		return new CRTSpringMesh;
	case RTID_CRTFlag:				return new CRTFlag;
	case RTID_CRTParticleSystem:	return new CRTParticleSystem;
//	case RTID_CRTNurbling:			return new CRTNurbling;
	case RTID_CRTFence:				return new CRTFence;
	case RTID_CRTTrail:				return new CRTTrail;
	case RTID_CRTLaser:				return new CRTLaser;
	case RTID_CRTLink:				return new CRTLink;

	default: ASSERT(0);		return NULL;
	};
}


#endif