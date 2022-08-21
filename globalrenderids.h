#ifndef GLOBAL_RENDER_IDS_H
#define GLOBAL_RENDER_IDS_H

#define MAX_RENDER_THINGS_PER_THING	4

//******************************************************************************************
enum	EWaveResponse
{
	WR_NONE,
	WR_MOVE,
	WR_WARP,
	WR_DONT_ROTATE, // shouldn't really be here!
};

//******************************************************************************************
enum	EShadowType
{
	ST_NONE,
	ST_SPRITE,
	ST_MAP,
};

//******************************************************************************************
enum
{
	RTID_NONE				= -1,
	RTID_CRTBox				= 0,
	RTID_CRTDecal			= 1,
	RTID_CRTLightning		= 2,	
	RTID_CRTMesh			= 3,
	RTID_CRTSpikes			= 4,
	RTID_CRTSpringMesh		= 5,
	RTID_CRTFlag			= 6,
	RTID_CRTParticleSystem	= 7,
//	RTID_CRTNurbling		= 8,
	RTID_CRTFence			= 9,
	RTID_CRTTrail			= 10,
	RTID_CRTLaser			= 11,
	RTID_CRTLink			= 12,

	RTD_EOL,
};

#endif
