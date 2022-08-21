#ifndef DCPLATFORM_H
#define DCPLATFORM_H

#include	"Map.h"



//extern __declspec(data) volatile int	vbltick=0;

//******************************************************************************************
#define	HFFOR 28
#define	HFFOG 52
#define	HFFOB 68

//#define SCR_Z	192
//#define SCR_Z	256
#define SCR_Z   512

#define PSXENGINE_SCALE 8

//******************************************************************************************
#define OT_BIT_DEPTH    10                  /* Bit length of order table */
#define	OT_LENGTH		(1<<OT_BIT_DEPTH)	/* Number of entries in order table. */

//#define NUM_MAP_POLYS	(128*64)
//#define NUM_OVL_POLYS   (128*64)
//#define	NUM_MISC_POLY_G4S	1024
//#define	NUM_MISC_POLY_FT4S	1024
//#define   NUM_MESH_POLY_GT3S  4096
//#define	NUM_MESH_POLY_GT3S	(8192 * 2)

#define POLYPOOL_SIZE	(2500*sizeof(POLY_GT4))
//#define POLYPOOL_SIZE	(5120*sizeof(POLY_GT4))

//#define LEVEL_POOL_SIZE (384*1024)
//#define LEVEL_POOL_SIZE (448*1024)
//#define LEVEL_POOL_SIZE (500*1024)
//#define LEVEL_POOL_SIZE (470*1024)
//#define LEVEL_POOL_SIZE (450*1024)
//#define LEVEL_POOL_SIZE (455*1024) // extremely tight.  Max usage is 461700 bytes
#define LEVEL_POOL_SIZE (1024*1024) // OTT for safety

//#define POLYPOOL_SIZE	(6144*sizeof(POLY_GT4))
//#define LEVEL_POOL_SIZE (256*1024)

#define	THING_POOL_SIZE		65536
#define MAX_MAPCELLS		((128 * 64) + 1)
#define MAPWHO_LAYER1_SIZE	(MAX_MAPCELLS / 4)
#define MAPWHO_LAYER2_SIZE	(MAX_MAPCELLS / 16)
#define MAPWHO_LAYER3_SIZE	(MAX_MAPCELLS / 64)
#define MAPWHO_LAYER4_SIZE	(MAX_MAPCELLS / 256)
#define MAPWHO_LAYER5_SIZE	(MAX_MAPCELLS / 1024)

#define NUM_MESH_CACHED_VERTICES (1536)



struct SPadData {
			UBYTE   comflag;
            UBYTE   count:4;
            UBYTE   type:4;
            UWORD   buttons;
            UBYTE   rx;
            UBYTE   ry;
            UBYTE   lx;
            UBYTE   ly;
            UBYTE   multitapspaceincaseofcrash[34-8];
			UBYTE	Motor0;
			UBYTE	Motor1;
			UBYTE	*vib_profile;
			UBYTE	valid;
			UBYTE	Send;
			UBYTE	ticker;
};

class	CDCMemoryPool
{
public:
	void	Clear();
	// Memory stuff

	// largely temporary

#ifdef JCLDCNOPSX  // PSX memory pools - see comment in DCMemBuffer.CPP, though, about the PolyPool
	NTAG		mNTAGTable[OT_LENGTH];
	char		mPolyPool[2][POLYPOOL_SIZE];
	NTAG		mNTAGTerms[2][2];
	NTAG		mNTAGPanel[2];

	struct  SMeshVertexCache
    {
        SWORD   x, y;
        SWORD   z;
        SWORD   pad;
    }   mMeshVertexCache[NUM_MESH_CACHED_VERTICES];

    struct  SMeshNormalCache
    {
        SWORD   x, y;
        SWORD   z;
        SWORD   pad;
    }   mMeshNormalCache[NUM_MESH_CACHED_VERTICES];

#endif

	UBYTE		mLevelPool[LEVEL_POOL_SIZE];
	SINT		mLevelPoolSize;

	UBYTE		mThingPool[THING_POOL_SIZE];

	CMapCell	mMapCells[MAX_MAPCELLS];

	ULONG		mMapWhoLayer1[MAPWHO_LAYER1_SIZE];
	ULONG		mMapWhoLayer2[MAPWHO_LAYER2_SIZE];
	ULONG		mMapWhoLayer3[MAPWHO_LAYER3_SIZE];
	ULONG		mMapWhoLayer4[MAPWHO_LAYER4_SIZE];
	ULONG		mMapWhoLayer5[MAPWHO_LAYER5_SIZE];

	ULONG		*GetMapWhoLayerMemory(SINT n)
	{
		switch (n)
		{
			case 1: return mMapWhoLayer1;
			case 2: return mMapWhoLayer2;
			case 3: return mMapWhoLayer3;
			case 4: return mMapWhoLayer4;
			case 5: return mMapWhoLayer5;
			default:
//				printf("Tried to get memory for a layer that doesn't exist...\n");
				return NULL;
		};
	}
};

extern CDCMemoryPool *POOL;
extern void	SetDCPool(CDCMemoryPool **p);

//******************************************************************************************

class	CDCPlatform
{
	friend class CController;
public:
	CDCPlatform()	{};
	~CDCPlatform() {};

	BOOL			Init();

	void			Shutdown();
	
#ifdef JCLDCNOPSX			// Not needed - no Libcrypt
	void			LoadCrypt();
#endif

#ifdef JCLDCNOPSX			// Replace with controller detection?
#endif
	BOOL			HaveDualShock()  {return TRUE;};

	BOOL			IsKeyDown(UINT key) {return TRUE;};
	BOOL			IsMouseButtonPressed(SINT button_no) {return FALSE;};

	void			ProcessMessages() {};		// deal with windows message queue
	//SDS Stereo Mono Setting from the Boot rom/firmware ram e.t.c..
	
	// JCL screensaver stuff
	void			CheckScreenSaver();
	void			ResetScreenSaver();
	float			mScreenSaverTime;	
	
	BOOL			IsControllerValid(SINT n);
	
	// some globals

	void	SetScreenWidth(SINT w) {mScreenWidth=w;};
	SINT	ScreenWidth() {return mScreenWidth;};
	SINT	ScreenWidthFull() {return 640;}; // SDS
	SINT	ScreenHeight() {return 480;};    // SDS

	// user control stuff
	BOOL	IsPrimaryFirePressed() {return FALSE;};
	BOOL	IsSecondaryFirePressed() {return FALSE;};

	// Flip screen
	//void	Flip(SINT frame = -1);
	void	ClearScreen(SINT screen, SINT r = 0, SINT g = 0, SINT b = 0);
	void	InitFrames();

	// Viewport select code... (needed for NTAG routines)
	void	SetViewport(SINT pnum)		{mViewport=pnum;}
	SINT	GetViewport()				{return mViewport;}

	UBYTE		*GetLevelPoolData(SINT size);
	void		ClearPools() {POOL->Clear();};

	int			GeometryMarkBeginScene();
    void        GeometryMarkEndScene();
    void        GeometryDrawSyncCallback();
    void        GeometryVBlankHandler();

	void		SetAlphaMode2(SINT ot);

	SINT		GetFrame() {return mFrame;};
	void		GetScreenPos(SINT &x, SINT &y);

	struct	SPadData	*GetPadData(SINT n);

	void	*AllocateThingMemory(ULONG size);
	void	FreeThingMemory(void *mem);

	SINT	GetVSyncCount() {return mVSyncCount;};

	void	ClearDebugVSync();
	SINT	GetDebugVSync();

protected:
#ifdef JCLDCNOPSX		// Not used
	void		ResetPolyPointers()
	{
		mPolyPtr=(char*)((int)(&POOL->mPolyPool[mFrame][0])&0x7fffff);
	}

	DRAWENV		mDrawEnv[2];
	DISPENV		mDispEnv[2];

	DR_MODE		mDMAlpha2[2];

	DR_AREAOFFSET	mDrawPolys[2][2];
	volatile SINT	sbGPUDrawing;
    volatile SINT	otselect;
    volatile SINT	last_mFrame;
    volatile SINT	sActivePrimLists;
    struct  SPadData mPadData1;
    struct  SPadData mPadData2;
	DR_AREAOFFSET	mScreenPolys[4];
	volatile char		*mPolyPtr;
#endif
	int		mScreenWidth;
    volatile SINT	mFrame;
	SINT	mViewport;
    volatile SINT	last_mFrame;
	volatile SINT	mVSyncCount;
};

#endif
