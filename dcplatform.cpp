#include	"Common.h"
#include    "globals.h"

#if TARGET == DC
#include	"DCPlatform.h"
#include	"DCBuildType.h"
#include	<malloc.h>
#include 	"DCMemCard.h"
#include	"FrontEndCommon.h"
#include	"Profile.h"

CDCMemoryPool	*POOL;

#include	"DCFMV.h"
#include	"Career.h" // hmmmm
#include 	"maincode.h"
//#include	<shinobi.h>
// Use to offset the draw position within VRAM

#if DISP_MODE == MODE_PAL
#define SCOFFS 0
#else
#define SCOFFS 8
#endif



//******************************************************************************************
//******************************************************************************************
//CCareer dummycareer;

void GeomVBlankHandler(void);
void GeomDrawSyncCallback(void);
/*
PKMCALLBACKFUNC EORCallback (PVOID pCallbackArguments)
{
vbltick++;
vbltick = vbltick % 50;
   
}
*/


//******************************************************************************************

extern void SoundTest(void);

volatile int fartyfart;

//******************************************************************************************
void	CDCMemoryPool::Clear()
{
	mLevelPoolSize = 0;

#ifdef JCLDCNOPSX			// PSX version had a separate heap - you may want to do the same thing?
	// Initialise thing memory
	InitHeap3((ULONG *)&mThingPool, 65536);
#endif

};

//******************************************************************************************

inline SINT gte_GetDQA(void)
{
	SINT retval;
#if defined(JJS)
	asm volatile("cfc2 %0,$27":"=r"(retval)::);
#endif
	return retval;
}

inline SINT gte_GetDQB(void)
{
	SINT retval;
#if defined(JJS)
	asm volatile("cfc2 %0,$28":"=r"(retval)::);
#endif
	return retval;
}


SPadData spad;




struct	SPadData *CDCPlatform::GetPadData(SINT n)
{
//return (SPadData *)SGetPadData();
spad.valid=0;
spad.buttons=1;
return &spad;

}
	
	
	
#ifdef JCLDCNOPSX			//  Maybe replace with alternate controller detection for DC
BOOL CDCPlatform::HaveDualShock()
{

	struct SPadData *pad0 = &mPadData1;
	struct SPadData *pad1 = &mPadData2;
	 if((!pad0->comflag)&&(pad0->type==7))return TRUE;
	 else if((!pad1->comflag)&&(pad1->type==7))return TRUE;
	 return FALSE;
}
#endif

#ifdef JCLDCNOPSX			// Not needed - no libcrypt for DC

BOOL mDoneLoadCrypt=0;

void CDCPlatform::LoadCrypt()
{
	u_char result[4];
//	unsigned char b[2048];
/*
	if (CAREER.AllGold())
		GAME.ShowLoadingScreen("SCREENS\\GOLD\\SPLASH.TIM");
	else
		GAME.ShowLoadingScreen("SCREENS\\SPLASH.TIM");
*/
#if DISP_MODE == MODE_NTSC
	return;
#endif

	if(!mDoneLoadCrypt)
	{
		InitCrypt();
#ifndef DISABLE_LIBCRYPT
		mDoneLoadCrypt=1;
		result[0]=CdlModeSize1;
		CdControlB( CdlSetmode,result,result);
		VSync(25);
		ReadCrypt();                        // Start reading the crypt data...
		// SDS while(!GOTKEY)
		{
			VSync(25);
//			printf("Waiting...\n");
			SINT	safe;
			safe = PLATFORM.GeometryMarkBeginScene();
			if (safe != -1)
			{
				SINT	f = PLATFORM.GetFrame();
				NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f + 2));
				NTAG_Rlink(PLATFORM.GetOrderTable(),OT_LENGTH,(long*)&POOL->mNTAGTerms[f][0].firstprim
															,(long*)&POOL->mNTAGTerms[f][0].lastprim);
				PLATFORM.GeometryMarkEndScene();
			}
		}
#endif

//	b[0]=b[1]=0;
//	Decrypt(b);
//	printf("Decrypt code = 0x%02x%02x\n",b[1]&0xff,b[0]&0xff);
	}


}
#endif


BOOL    CDCPlatform::Init()
{
	u_char		param[4], result[8];
	fartyfart=1;

//	kmSetEORCallback ((PKMCALLBACKFUNC) EORCallback, NULL); // SDS set up the VBL ticker!!!
	
	SetDCPool(&POOL);

	SetScreenWidth(640);

#ifdef JCLDCNOPSX			// Lots of unnecessary PSX init stuff - might be worth checking through this to see if there's anything you should be initialising eg. geometry setup, CD, sound etc...

	ResetCallback();
#ifdef DEMODISK
	printf("It's a DEMODISK!!!!!\n");
#endif



	PadInitDirect(&mPadData1.comflag,&mPadData2.comflag);                     /* initialize PAD */
#ifndef DEMODISK
	MemCardInit(0);
	MemCardStart();
#endif
	ResetGraph(0);					/* reset graphic subsystem (0:cold,1:warm) */
    SetVideoMode(DISP_MODE);

	SetScreenWidth(512);                 // Default screen width, changes to 256 for two player mode...
	SetGraphDebug(0);				/* set debug mode (0:off, 1:monitor, 2:dump) */

	InitGeom();						/* initialize geometry subsystem */
    SetGeomOffset(ScreenWidth()/2, ScreenHeight()/2);//+80);   /* set geometry origin as (160, 120) */
    SetGeomScreen(SCR_Z);                               /* distance to viewing-screen */

	//int i,j;
	//for(i=32768;i<65536;i+=256)
	//{
	//	for(j=32768;j<65536;j+=256)
	//	{
	//		//SetFogNearFar(i,j,SCR_Z);
	//		SetFogNearFar(2048<<2,3600<<2,SCR_Z);
	//		int dqb=gte_GetDQB();
	//		int dqa=gte_GetDQA();
	//		printf("I: %05d  J: %05d  DQA: 0x%08x  DQB: 0x%08x\n",i,j,dqa,dqb);
	//	}
	//}


	// clear the screen!
	ClearScreen(0);
	ClearScreen(1);


	NTAG_init(GetOrderTable(),OT_LENGTH);
	SsInit();
	CdInit();                         // Initialize low level CD access libray.
	param[0] = CdlModeSpeed|CdlModeRT|CdlModeSF;
	CdControlB(CdlSetmode, param, 0);
#if 0

	SetScreenWidth(512);                 // Default screen width, changes to 256 for two player mode...
	SetGraphDebug(0);				/* set debug mode (0:off, 1:monitor, 2:dump) */

	InitGeom();						/* initialize geometry subsystem */
    SetGeomOffset(ScreenWidth()/2, ScreenHeight()/2);//+80);   /* set geometry origin as (160, 120) */
    SetGeomScreen(SCR_Z);                               /* distance to viewing-screen */

	//int i,j;
	//for(i=32768;i<65536;i+=256)
	//{
	//	for(j=32768;j<65536;j+=256)
	//	{
	//		//SetFogNearFar(i,j,SCR_Z);
	//		SetFogNearFar(2048<<2,3600<<2,SCR_Z);
	//		int dqb=gte_GetDQB();
	//		int dqa=gte_GetDQA();
	//		printf("I: %05d  J: %05d  DQA: 0x%08x  DQB: 0x%08x\n",i,j,dqa,dqb);
	//	}
	//}


	// clear the screen!
	ClearScreen(0);
	ClearScreen(1);
#endif
//!	PCinit();						// init SN PC stuff



	SetDefDrawEnv(&mDrawEnv[0], 0, 256, ScreenWidth(), ScreenHeight());
    SetDefDrawEnv(&mDrawEnv[1], 0, 0  , ScreenWidth(), ScreenHeight());
    SetDefDispEnv(&mDispEnv[0], 0, 0  +SCOFFS, ScreenWidth(), ScreenHeight());
    SetDefDispEnv(&mDispEnv[1], 0, 256+SCOFFS, ScreenWidth(), ScreenHeight());
    //SetDefDrawEnv(&mDrawEnv[0], 0, 0             , 320, 256);
    //SetDefDrawEnv(&mDrawEnv[1], 0, 256, 320, 256);
    //SetDefDispEnv(&mDispEnv[0], 0, 256, 320, 256);
    //SetDefDispEnv(&mDispEnv[1], 0, 0             , 320, 256);

    mDrawEnv[0].isbg = 1;//1;
    mDrawEnv[1].isbg = 1;//1;
    mDrawEnv[0].dtd = 1;
    mDrawEnv[1].dtd = 1;
    mDrawEnv[0].r0 = 0;//HFFOR;
	mDrawEnv[0].g0 = 0;//HFFOG;
	mDrawEnv[0].b0 = 0;//HFFOB;

	mDrawEnv[1].r0 = 0;//HFFOR;
	mDrawEnv[1].g0 = 0;//HFFOG;
	mDrawEnv[1].b0 = 0;//HFFOB;

	mDispEnv[1].screen.x = mDispEnv[0].screen.x = 0;

#if DISP_MODE == MODE_PAL
	mDispEnv[1].screen.y = mDispEnv[0].screen.y = 18;
#else
	mDispEnv[1].screen.y = mDispEnv[0].screen.y = 0;
#endif


	mDispEnv[1].screen.h = mDispEnv[0].screen.h = ScreenHeight();
	mDispEnv[1].pad0 = mDispEnv[0].pad0 = 1;


/**///Now set the Primitives for the view ports (p1, p2, full screen)



	DR_AREAOFFSET *p00=&mDrawPolys[1][0];
	DR_AREAOFFSET *p01=&mDrawPolys[1][1];
	DR_AREAOFFSET *p10=&mDrawPolys[0][0];
	DR_AREAOFFSET *p11=&mDrawPolys[0][1];
	SetPolyAreaOffset(p00,  0,256,(ScreenWidth()>>1)-1,256,  0,256);  /* Player 1 frame 0 */
	SetPolyAreaOffset(p01,257,256,(ScreenWidth()>>1)-1,256,256,256);  /* Player 2 frame 0 */
	SetPolyAreaOffset(p10,  0,  0,(ScreenWidth()>>1)-1,256,  0,  0);  /* Player 1 frame 1 */
	SetPolyAreaOffset(p11,257,  0,(ScreenWidth()>>1)-1,256,256,  0);  /* Player 2 frame 1 */

	DR_AREAOFFSET *v0=&mScreenPolys[1];
	DR_AREAOFFSET *v1=&mScreenPolys[0];
	DR_AREAOFFSET *v2=&mScreenPolys[3];
	DR_AREAOFFSET *v3=&mScreenPolys[2];
	SetPolyAreaOffset(v0,  0,256,512,ScreenHeight(),  0,256);  /* frame 0 Fullscreen*/
	SetPolyAreaOffset(v1,  0,  0,512,ScreenHeight(),  0,  0);  /* frame 1 Fullscreen*/
	SetPolyAreaOffset(v2,  0,256,512,ScreenHeight(),  0,256);  /* frame 0 Fullscreen*/
	SetPolyAreaOffset(v3,  0,  0,512,ScreenHeight(),  0,  0);  /* frame 1 Fullscreen*/

	/*-----------------01/02/00 13:00-------------------
	 * Now set up the dtd, dfe and clear screen polys for the
	 * display environments that need them, and reduce the
	 * DMA size for those that don't.
	 * --------------------------------------------------*/
	for(int c0=0;c0<2;c0++)
	{
		mScreenPolys[c0].excode[0]=0xe1000600;
		mScreenPolys[c0].excode[1]=0x60000000;
		mScreenPolys[c0].excode[2]=0x00000000;
		mScreenPolys[c0].excode[3]=0x01000200;
		mScreenPolys[c0+2].excode[0]=0xe1000600;
		PT(&mScreenPolys[c0+2])->len = 5;  // Don't want to clear the screen with these ones, just set dtd/dfe

		for(int c1=0;c1<2;c1++)
		{
			mDrawPolys[c0][c1].excode[0]=0xe1000600;
			mDrawPolys[c0][c1].excode[1]=0x60000000;
			mDrawPolys[c0][c1].excode[2]=0x00000000;
			mDrawPolys[c0][c1].excode[3]=0x01000100;
		}
	}

	//DrawPrim(&mScreenPolys[0]);
	VSync(0);
	PutDrawEnv(&mDrawEnv[0]); /* update drawing environment */
	PutDispEnv(&mDispEnv[0]); /* update display environment */

    //SDS SetFogNearFar(2048<<2,3200<<2,SCR_Z);
	//while(fartyfart);
	InitFrames();
	SetDispMask(1);     /* enable to display (0:inhibit, 1:enable) */

	// setup 1 * bg - 1 * tx drawmode for alpha polys
//	RECT	dummy;
//	SetDrawMode(&mDMAlpha2[0], 1, 1, getTPage(0, 2, 0, 0), &dummy);
//	SetDrawMode(&mDMAlpha2[1], 1, 1, getTPage(0, 2, 0, 0), &dummy);
	SetDrawMode(&mDMAlpha2[0], 1, 1, getTPage(0, 2, 0, 0), NULL);
	SetDrawMode(&mDMAlpha2[1], 1, 1, getTPage(0, 2, 0, 0), NULL);

	SetFarColor(HFFOR,HFFOG,HFFOB);

	// This was moved out of SPSXMeshPart::Render for speed....
#endif

	ClearPools();
	
#ifdef JCLDCNOPSX			// More PSX Setup - see above note
	
	ResetPolyPointers();

    VSyncCallback(GeomVBlankHandler);
    DrawSyncCallback(GeomDrawSyncCallback);

	PadStartCom();


	VSync(0);
	VSync(0);
	VSync(0);
	VSync(0);
	int done0=0;
	int done1=0;
	for(int i=0;i<20;i++)
	{
	VSync(0);
		if((!done0))
		{
			int f;
			while(((f=PadGetState(0x00))==PadStateStable)&&(!done0))
			{
				//printf("State: %d\n",f);
				done0=PadSetMainMode(0x00,1,1);
				//printf("Checking port 0 (%d)\n",done0);
			}
		}
		if((!done1))
		{
			int f;
			while(((f=PadGetState(0x10))==PadStateStable)&&(!done1))
			{
				//printf("State: %d\n",f);
				done1=PadSetMainMode(0x10,1,1);
				//printf("Checking port 1 (%d)\n",done1);
			}

	}    }


	mVSyncCount = 0;

#if FORCE_CARD_UNFORMAT == 1
	printf("Acquire returned: %d\n",MCAcquireCard());
	int ret=MemCardUnformat(0);
	printf("Card UNFormat returned: %d\n",ret);
#endif


	//printf("Save returned: %d\n",MCSaveCareer("DAMOC1",&dummycareer,1));
	//printf("Load returned: %d\n",MCLoadCareer("DAMOC1",&dummycareer));


	//while(1)
	//{
	//	int r=MCGetState(1);
	//	if(r==McErrNone)MCEchoFiles();
	//}

		//for(int dl=0;dl<80;dl++)VSync(1);
//	PlayStream(&TEST_STREAM,&TK);

	//!Shouldn't be here...

	extern	void	show_release_notes();
//	show_release_notes();
//	PlayFMV(FMV_ROCKSTAR);


	//DoPadTest();
	
#endif
	
	return TRUE;
}


void CDCPlatform::Shutdown()
{
#ifdef JCLDCNOPSX			// Shutdown code - only necessary if you need to quit the game (eg return to demo stub?)
	DrawSync(0);
	DrawSync(0);
	DrawSync(0);
	VSync(0);
	VSync(0);
	VSync(0);
	DrawSync(0);
	DrawSync(0);
	DrawSync(0);
	VSync(0);
	VSync(0);
	VSync(0);
	DrawSync(0);
	DrawSync(0);
	DrawSync(0);
	VSync(0);
	VSync(0);
	VSync(0);

   BreakDraw();                         // Make sure the GPU has stopped
   while(IsIdleGPU(1)!=0);              // Wait for BreakDraw to complete..
   PadStopCom();           /* Stop pad reading. */
   SsQuit();
   VSyncCallback(NULL);
   DrawSyncCallback(NULL);
   StopCallback();      /* Stop the CD callbacks. */

   ResetGraph(0);
//   ResetGraph(3);       /* GPU warm reset */

   SetGraphDebug(1);
   
#endif
   return;
}



//******************************************************************************************
void	CDCPlatform::GetScreenPos(SINT &x, SINT &y)
{
	x = 0;
//	y = (mFrame == 0) ? 256 : 0;
	y = (mFrame == 1) ? 256 : 0;
}


//******************************************************************************************
void	CDCPlatform::SetAlphaMode2(SINT ot)
{
#ifdef JCLDCNOPSX			// Inserts a PSX primitive to change the alpha blending mode - you'll need to do this a different way on the PSX
	addPrim(&POOL->mNTAGTable[ot], &mDMAlpha2[mFrame]); // FOR NOW.... (NTAG WORK)
	//addPrim(&POOL->mOrderTable[mFrame][ot], &mDMAlpha2[mFrame]); // FOR NOW.... (NTAG WORK)
#endif
}
//******************************************************************************************
#if 0
void	CDCPlatform::Flip(SINT frame)
{
#if 0
	// a few checks
	if (mMeshPolyGT3sUsed > NUM_MESH_POLY_GT3S)
	{
		ASSERT(0);
		printf("Bugger. Out of poly GT3s");
	}

	if (mMiscPolyG4sUsed > NUM_MISC_POLY_G4S)
	{
		ASSERT(0);
		printf("Bugger. Out of poly G4s");
	}

	if (mMiscPolyFT4sUsed > NUM_MISC_POLY_FT4S)
	{
		ASSERT(0);
		printf("Bugger. Out of poly FT4s");
	}
#endif

//#if 0
    DrawSync(0);                  /* wait for end of drawing */
    VSync(0);                     /* wait for the next V-BLNK */

    if (frame != -1)
	    mFrame = frame;  		// set frame manually if desired

	PutDrawEnv(&mDrawEnv[mFrame]); /* update drawing environment */
	PutDispEnv(&mDispEnv[mFrame]); /* update display environment */

	if (mFrame == 1)
		mFrame = 0;
	else
		mFrame = 1;

//#endif
	// reset poly pointers
    ResetPolyPointers();
    RenderFlip();
}
#endif
//******************************************************************************************
void	CDCPlatform::ClearScreen(SINT screen, SINT r, SINT g, SINT b)
{
#ifdef JCLDCNOPSX			// Rewrite for DC
	RECT 	rect;

	if (screen == 0)
		rect.y = 0;
	else
		rect.y = 256;

	rect.x = 0;
	rect.w = ScreenWidthFull();
	rect.h = 256;

	ClearImage(&rect, r, g, b);
#endif
}

//******************************************************************************************
UBYTE	*CDCPlatform::GetLevelPoolData(SINT size)
{
	// get some space on the level pool

	if (POOL->mLevelPoolSize + size >= LEVEL_POOL_SIZE)
	{
		// oops! no memory left
		ASSERT(0);
		return NULL;
	}

    UBYTE   *t = POOL->mLevelPool + POOL->mLevelPoolSize;
	POOL->mLevelPoolSize += size;
	//printf("Pool used: 0x%08x\n",POOL->mLevelPoolSize);

	return t;
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
__declspec(data) volatile char otq[2]={0};
__declspec(data) volatile char ots[2]={0};
__declspec(data) volatile char dispframe=0;

__declspec(data) volatile int	drawnext=0;
__declspec(data) volatile int	time_to_dec_lim=0;
__declspec(data) volatile int	SCREENFRAME_LIMITER=2;

__declspec(data) volatile int	vbltick=0;
__declspec(data) volatile int	debug_vbltick=0;
__declspec(data) volatile int	screenframe_tick=0;
__declspec(data) volatile int	frames_to_game_update=0;
__declspec(data) volatile int	frames_to_screen_update=0;

void	CDCPlatform::ClearDebugVSync()
{
	debug_vbltick = 0;
}

SINT	CDCPlatform::GetDebugVSync()
{
	return debug_vbltick;
}


int CDCPlatform::GeometryMarkBeginScene( void )
{
	PROFILE_FN(RenderBeginScene);
	SDSRenderStart();
	return TRUE;
}

static	int	frames_to_wait = 2;

void CDCPlatform::GeometryMarkEndScene( void )
{
	PROFILE_FN(RenderEndScene);
	SDSRenderFlip();
}

void CDCPlatform::GeometryVBlankHandler( void )
{
}

void CDCPlatform::GeometryDrawSyncCallback( void )
{
}

void	CDCPlatform::InitFrames(void)
{
}

void GeomVBlankHandler(void)
{
    PLATFORM.GeometryVBlankHandler();
}

void GeomDrawSyncCallback(void)
{
    PLATFORM.GeometryDrawSyncCallback();
}



//******************************************************************************************
//******************************************************************************************

// thing memory stuff

unsigned long	lowest_thing_allocated = 0xffffffff;

//******************************************************************************************
void	*CDCPlatform::AllocateThingMemory(ULONG size)
{
	void	*mem = _Align32Malloc(size);
//	if (!mem)
//		printf("Couldn't allocate thing of size %d\n", size);
//	else
	if (mem)
	{
//		printf("Allocated Thing of size %d at %X\n", size, mem);
		ULONG	foo = (ULONG)mem;
		if (ULONG(mem) < lowest_thing_allocated)
			lowest_thing_allocated = ULONG(mem);
	}
	return (mem) ? mem : NULL;
}

//******************************************************************************************
void	CDCPlatform::FreeThingMemory(void *mem)
{
//	printf("About to free memory at %X\n", mem);
	_Align32Free(mem);
}

#endif


#if TARGET == DC

void	GoToDebugger()
{
	int i = 0;
	int j;
	j = i;
}

#endif


//******************************************************************************************
//#define SCREENSAVER_TIME	60.f
#define SCREENSAVER_TIME	300.f
//#define SCREENSAVER_TIME	10.f

static float	last_diff = 0;

void	CDCPlatform::CheckScreenSaver()
{
	if (GetSysTimeFloat() < mScreenSaverTime - 1.0f)
	{
		// timer has wrapped!
		mScreenSaverTime = GetSysTimeFloat() + last_diff;
	}

	if (CONTROLS.AreAnyButtonsPressed())
		ResetScreenSaver();

	float	diff = GetSysTimeFloat() - mScreenSaverTime;
	last_diff = diff;
	
	float	val = 0;
	
	if (diff > SCREENSAVER_TIME)
		val = diff - SCREENSAVER_TIME;
	
	if (diff < 0)
		val = -diff;
	
	if (val == 0)
		return;
		
	val *= 2.f;
	
	if (val > 1.f)
		val = 1.f;
		
	// render screen saver.
	
	STextureRefOther *tr;
	
	if (GAME.mInFrontEnd)
		tr = ENGINE.GetNumberedTexture(FEDCPST_LCD3);
	else
		tr = ENGINE.GetNumberedTexture(TN_WHITE);

	SINT	TN = tr->mTPage + 1;

	kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);

	float	u0, u1, v0, v1;

	if (GAME.mInFrontEnd)
	{
		u0 = tr->mX + (tr->mW * 0.3f) + 0.002f;
		u1 = tr->mX + (tr->mW * 0.6f) - 0.002f;
		v0 = tr->mY + (tr->mH * 0.3f) + 0.002f;
		v1 = tr->mY + (tr->mH * 0.6f) - 0.002f;
	}
	else
	{
		u0 = tr->mX + 0.002f;
		u1 = tr->mX + tr->mW - 0.002f;
		v0 = tr->mY + 0.002f;
		v1 = tr->mY + tr->mH - 0.002f;
	}
	
	SINT	ival;
	if (GAME.mInFrontEnd)
		ival = SINT(val * 255);
	else
		ival = SINT(val * 192);
		
	DWORD	col = (ival << 24);	
	
	InitCZTs(TN);

   	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;

   	TextureS[0].fX                   = 0;
   	TextureS[0].fY                   = 0;
   	TextureS[1].fX                   = 640;
   	TextureS[1].fY                   = 0;
   	TextureS[2].fX                   = 0;
   	TextureS[2].fY                   = 480;
   	TextureS[3].fX                   = 640;
   	TextureS[3].fY                   = 480;

	TextureS[0].fU                   = u0;
   	TextureS[0].fV                   = v1;
   	TextureS[1].fU                   = u0;
   	TextureS[1].fV                   = v0;
   	TextureS[2].fU                   = u1;
   	TextureS[2].fV                   = v1;
   	TextureS[3].fU                   = u1;
   	TextureS[3].fV                   = v0;

   	TextureS[0].u.fZ                 = 11.0f;
   	TextureS[1].u.fZ                 = 11.0f;
   	TextureS[2].u.fZ                 = 11.0f;
   	TextureS[3].u.fZ                 = 11.0f;

   	TextureS[0].uBaseRGB.dwPacked    = col;
   	TextureS[1].uBaseRGB.dwPacked    = col;
   	TextureS[2].uBaseRGB.dwPacked    = col;
   	TextureS[3].uBaseRGB.dwPacked    = col;

	ExitCZTs(4);
}

//******************************************************************************************
void	CDCPlatform::ResetScreenSaver()
{
	float	diff = GetSysTimeFloat() - mScreenSaverTime;
	
	if (diff < 0)
		return; 	// fading in
		
	mScreenSaverTime = GetSysTimeFloat();
	
	if (diff > SCREENSAVER_TIME)
		mScreenSaverTime += 0.5f; // fade in
}

//******************************************************************************************
BOOL	CDCPlatform::IsControllerValid(SINT n)
{
	SINT	controller = (n == 0) ? PDD_PORT_A0 : PDD_PORT_B0;

	ControllerState cs = SDSCheckController(controller);
	
	// JCLDCNOPSX - unsupported???
	
	if ((cs == Conrtoller_NOT_THERE) || (cs == Controller_EXISTS_UNKNOWN))
		return FALSE;
		
	return TRUE;
}
