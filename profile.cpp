#include	"Common.h"

#include	"Profile.h"
#include	"Globals.h"

#if TARGET == dc
#include "maincode.h"
#endif
#if TARGET == PC
#include	<string.h>
#endif

#if TARGET == PSX
#if ENABLE_PROFILER == 1

/*-----------------20/01/00 16:20-------------------
 * PSX SPECIFIC PROFILER TIMER ROUTINES......
 * --------------------------------------------------*/

__declspec(data) volatile int ProfilerTimerTick;

long __ProfilerTimerRoutine(void)
{
	ProfilerTimerTick += 1L; /*Just add one to the current count...*/
	//ProfilerTimerTick &= 0x7FFFFFFF;
	//printf("%d\n",ProfilerTimerTick);
	return 0;
}
/***************************************************************************/
#define PROFILER_TIMER_RATE_HORZ_TIMER		20000
#define	PROFILER_TIMER_RATE_SYSTEM_CLOCK	20000

void __InstallProfilerTimer(void)
{
	long TimerEvent;
	long TimerRate;

	ProfilerTimerTick = 0L;

	/**********************************
	 * Figure out which rate to use...
	 **********************************/
	if(TimerToUse == RCntCNT1)
		TimerRate = PROFILER_TIMER_RATE_HORZ_TIMER;
	else
		TimerRate = PROFILER_TIMER_RATE_SYSTEM_CLOCK;

	/************************************************************
	 * Install our timer callback routine...
	 ************************************************************/
	EnterCriticalSection();

	/************************************************************
	 * Get our timer going...
	 ************************************************************/
	TimerEvent = OpenEvent(TimerToUse,EvSpINT,EvMdINTR,__ProfilerTimerRoutine);
	if(!EnableEvent(TimerEvent))
		printf("Unable to enable timer event!\n");

	SetRCnt(TimerToUse, TimerRate, RCntMdINTR | RCntMdSC);
	StartRCnt(TimerToUse);
	ExitCriticalSection();
}

#endif
#endif

#if ENABLE_PROFILER == 1

//****************************************************************************************************
CProfile	CProfiler::mProfiles[PE::ProfileCount];
SINT		CProfiler::mCurrentPage=1;
SINT		CProfiler::mFetcher=0;

//****************************************************************************************************
void	CProfiler::InitTimer(PE::EProfile timer, PE::EProfilePage page, char *name)
{
	mProfiles[timer].mPage=page;
	my_strncpy(mProfiles[timer].mName, name, 1000); //! Don't ask me..  strcpy doesn't seem to exist?
};

//****************************************************************************************************
void	CProfiler::Init()
{
#if TARGET == PSX
	__InstallProfilerTimer();
#endif
	InitTimer(PE::TotalModel			, PE::Globals			, "Total Model");
	InitTimer(PE::TotalEngine			, PE::Globals			, "Total Engine");

	InitTimer(PE::RenderSky				, PE::RenderOverview	, "Draw Sky");
	InitTimer(PE::RenderMap				, PE::RenderOverview	, "Draw Map");
	InitTimer(PE::RenderThings			, PE::RenderOverview	, "Draw Things");
	InitTimer(PE::RenderQSort			, PE::RenderOverview	, "QSort");
	InitTimer(PE::RenderTextures		, PE::RenderOverview	, "DrawAll()");
	InitTimer(PE::RenderBeginScene		, PE::RenderOverview	, "BeginScene()");
	InitTimer(PE::RenderEndScene		, PE::RenderOverview	, "EndScene()");
	InitTimer(PE::Render2D				, PE::RenderOverview	, "Draw 2D");
	InitTimer(PE::RenderFlip			, PE::RenderOverview	, "Flip()");
	InitTimer(PE::RenderDIP				, PE::RenderOverview	, "DrawIndexedPrim()");
	InitTimer(PE::RenderGCGamut			, PE::RenderOverview	, "GC GamutCalc");
	InitTimer(PE::RenderResetInt		, PE::RenderOverview	, "Reset Int");

	InitTimer(PE::RMInnerLoop			, PE::RMap				, "Inner Loop");
	InitTimer(PE::RMDeltas				, PE::RMap				, "Deltas");
	InitTimer(PE::RMTransform			, PE::RMap				, "Transform");	
	InitTimer(PE::RMDraw1				, PE::RMap				, "Draw 1");
	InitTimer(PE::RMDraw2				, PE::RMap				, "Draw 2");
	InitTimer(PE::RMDrawPanel			, PE::RMap				, "DrawPanel");	
	InitTimer(PE::RMDraw4x4				, PE::RMap				, "4x4 Total");	
	InitTimer(PE::RMStripGeometry		, PE::RMap				, "Strip Geom");
	InitTimer(PE::RMStripSetup			, PE::RMap				, "Strip Light");
	InitTimer(PE::RMExitStrip			, PE::RMap				, "Exit Strip");	
	InitTimer(PE::RMTriggers			, PE::RMap				, "Triggers");	
	InitTimer(PE::RMDrawParticles		, PE::RMap				, "Map Particles");	
	InitTimer(PE::RMDrawPickupBeam		, PE::RMap				, "Draw Pickup Beam");
	InitTimer(PE::RMDrawWater			, PE::RMap				, "Water");

	InitTimer(PE::RMMeshTotal				, PE::RMesh				, "TotalMesh");	
	InitTimer(PE::RMMeshDrawStartUp			, PE::RMesh				, "DMCull");	
	InitTimer(PE::RMMeshDrawTransWrapper1	, PE::RMesh				, "DMTrans1");	
	InitTimer(PE::RMMeshDrawMesh			, PE::RMesh				, "DrawMesh");		
	InitTimer(PE::RMMeshDrawHeaders			, PE::RMesh				, "DrawHeaders");	
	InitTimer(PE::RMMeshDrawLights			, PE::RMesh				, "DrawLights");			
	InitTimer(PE::RMDrawStadium				, PE::RMesh				, "Stadium");	


	InitTimer(PE::ProcessMap			, PE::GameOverview		, "Process Map");
	InitTimer(PE::ProcessMapSt1			, PE::GameOverview		, "-> Stage 1");
	InitTimer(PE::ProcessMapSt2			, PE::GameOverview		, "-> Stage 2");
	InitTimer(PE::ProcessWorld			, PE::GameOverview		, "Process World");
	InitTimer(PE::ThingProcess			, PE::GameOverview		, "Thing Process()");
	InitTimer(PE::GameCollision			, PE::GameOverview		, "Collision");
	InitTimer(PE::Character				, PE::GameOverview		, "Character Process()");
	InitTimer(PE::BadGuy				, PE::GameOverview		, "BadGuy Process()");
	InitTimer(PE::FenceCollision		, PE::GameOverview		, "Fence Collision");
}

//****************************************************************************************************
void	CProfiler::ResetAll()
{
	SINT c0;

	for (c0=0;c0<PE::ProfileCount;c0++)
	{
		mProfiles[c0].mLastTotalTime = mProfiles[c0].mTotalTime;
		mProfiles[c0].mLastCalls = mProfiles[c0].mCalls;

		mProfiles[c0].mTotalTime=0;
		mProfiles[c0].mCalls=0;
	}
}

//****************************************************************************************************
void	CProfiler::NextPage()
{
	if (mCurrentPage<PE::ProfilePageCount-1)
		mCurrentPage++;
}

//****************************************************************************************************
void	CProfiler::PrevPage()
{
	if (mCurrentPage>0)
		mCurrentPage--;
}

//****************************************************************************************************
CProfile	*CProfiler::Fetch()
{
	while (mFetcher<PE::ProfileCount)
	{
		if (mProfiles[mFetcher].mPage==mCurrentPage)
			return &mProfiles[mFetcher++];
		mFetcher++;
	}
	return NULL; // no more left
}

//****************************************************************************************************
void	CProfiler::ResetFetch()
{
	mFetcher=0;
}

//****************************************************************************************************
//****************************************************************************************************
//****************************************************************************************************

#if TARGET == PC

#define PC_XT1	120
#define PC_XT2	200
#define PC_XT3	240
#define PC_XL	320

#define PC_SCALE 16

//****************************************************************************************************
void	CProfiler::DrawProfile(HDC hDC, CProfile *p, SINT y)
{
	char text[200];
	SINT	size;

	size = sprintf(text, "%s: ", p->mName);
	SetTextColor(hDC, RGB(0,0,0));
	TextOut(hDC, 0 + 2, y + 2, text, size);
	SetTextColor(hDC, RGB(200,130,50));
	TextOut(hDC, 0, y, text, size);

	size = sprintf(text, "%d", p->mLastTotalTime);
	SetTextColor(hDC, RGB(0,0,0));
	TextOut(hDC, PC_XT1 + 2, y + 2, text, size);
	SetTextColor(hDC, RGB(100,200,50));
	TextOut(hDC, PC_XT1, y, text, size);

	size = sprintf(text, "%d", p->mLastCalls);
	SetTextColor(hDC, RGB(0,0,0));
	TextOut(hDC, PC_XT2 + 2, y + 2, text, size);
	SetTextColor(hDC, RGB(100,200,50));
	TextOut(hDC, PC_XT2, y, text, size);

	if (p->mLastCalls)
	{
		size = sprintf(text, "%d", p->mLastTotalTime / p->mLastCalls);
		SetTextColor(hDC, RGB(0,0,0));
		TextOut(hDC, PC_XT3 + 2, y + 2, text, size);
		SetTextColor(hDC, RGB(100,200,50));
		TextOut(hDC, PC_XT3, y, text, size);
	}

	SINT	v = (p->mLastTotalTime)>>PC_SCALE;

	if (v > 150) v = 150;
	memset(text, '.', v);
	SetTextColor(hDC, RGB(100,100,255));
	TextOut(hDC, PC_XL, y - 5, text, v);
}

//****************************************************************************************************
void	CProfiler::ShowProfile()
{
	CProfile	*p;
	SINT		y = 150;

	HDC hDC = PLATFORM.GetDPWin()->GetDC(); // ####!!!!

	SetBkMode(hDC, TRANSPARENT);

	// draw totals
	p = &mProfiles[0];
	DrawProfile(hDC, p, y);
	y += 16;

	p = &mProfiles[1];
	DrawProfile(hDC, p, y);
	y += 16;

	y += 16;
	ResetFetch();
	while ((p=Fetch())!=NULL)
	{
		DrawProfile(hDC, p, y);
		y += 16;

		if (y > 450)
			break;
	}
	PLATFORM.GetDPWin()->ReleaseDC(hDC); // ####!!!!
}

//****************************************************************************************************
//****************************************************************************************************
//****************************************************************************************************
#elif TARGET == PSX

#define PSX_XT1	120
#define PSX_XT2	200
#define PSX_XT3	240
#define PSX_XL	320

#define PSX_SCALE 0

//****************************************************************************************************
void	CProfiler::DrawProfile(CProfile *p, SINT y)
{
	char text[200];

	sprintf(text, "%d", p->mLastTotalTime);
	ENGINE.DrawString(0, text, PSX_XT1, y , 0xff308010);

	sprintf(text, "%d", p->mLastCalls);
//	sprintf(text, "%d", GetRCnt(RCntCNT1));
	ENGINE.DrawString(0, text, PSX_XT2, y , 0xff308010);

	if (p->mLastCalls)
	{
		sprintf(text, "%d", p->mLastTotalTime / p->mLastCalls);
		ENGINE.DrawString(0, text, PSX_XT3, y , 0xff308010);
	}

	sprintf(text, "%s: ", p->mName);
	ENGINE.DrawString(0, text, 10, y , 0xff804010);

	SINT	v = (p->mLastTotalTime)>>PSX_SCALE;

	v /= 2;

	if (v > 150) v = 150;
	char *f;
	for(f=text;v;v--)*f++='.';
	*f=0;
	//memset(text, '.', v);
	ENGINE.DrawString(0, text, PSX_XL, y - 5 , 0xff303080);
}

//****************************************************************************************************

void	CProfiler::ShowProfile()
{
	CProfile	*p;
	SINT		y = 50;

	// draw totals
	p = &mProfiles[0];
	DrawProfile(p, y);
	y += 16;

	p = &mProfiles[1];
	DrawProfile(p, y);
	y += 16;

	y += 16;
	ResetFetch();
	while ((p=Fetch())!=NULL)
	{
		DrawProfile(p, y);
		y += 16;

		if (y > 250)
			break;
	}
}

//****************************************************************************************************
//****************************************************************************************************
//****************************************************************************************************
#elif TARGET == DC

#define DC_XT1	120
#define DC_XT2	200
#define DC_XT3	240
#define DC_XL	320

#define DC_SCALE 10

//****************************************************************************************************
void	CProfiler::DrawProfile(CProfile *p, SINT y)
{
	char text[200];

	sprintf(text, "%d", p->mLastTotalTime);
	ENGINE.DrawString(0, text, DC_XT1, y , 0xff308010);
	ENGINE.DrawString(0, text, DC_XT1 + 1, y + 1, 0);

	sprintf(text, "%d", p->mLastCalls);
	ENGINE.DrawString(0, text, DC_XT2, y , 0xff308010);
	ENGINE.DrawString(0, text, DC_XT2 + 1, y + 1, 0);

	if (p->mLastCalls)
	{
		sprintf(text, "%d", p->mLastTotalTime / p->mLastCalls);
		ENGINE.DrawString(0, text, DC_XT3, y , 0xff308010);
		ENGINE.DrawString(0, text, DC_XT3 + 1, y + 1, 0);
	}

	sprintf(text, "%s: ", p->mName);
	ENGINE.DrawString(0, text, 10, y , 0xff804010);
	ENGINE.DrawString(0, text, 10 + 1 , y + 1 , 0);

	SINT	v = (p->mLastTotalTime)>>DC_SCALE;

	v /= 2;

	if (v > 150) v = 150;
	char *f;
	for(f=text;v;v--)*f++='.';
	*f=0;
	//memset(text, '.', v);
	ENGINE.DrawString(0, text, DC_XL, y - 5 , 0xff303080);
}

//****************************************************************************************************

void	CProfiler::ShowProfile()
{
	CProfile	*p;
	SINT		y = 50;

	// draw totals
	p = &mProfiles[0];
	DrawProfile(p, y);
	y += 16;

	p = &mProfiles[1];
	DrawProfile(p, y);
	y += 16;

	y += 16;
	ResetFetch();
	while ((p=Fetch())!=NULL)
	{
		DrawProfile(p, y);
		y += 16;                                                                                                                                                   

		if (y > 450)
			break;
	}
	
#if TARGET==DC
	char text[200];
	for (int a=0;a<5;a++)
	{
	sprintf(text,"Max Vertex Size %d",SDSGetNativeVertexBufferSize(a));
	ENGINE.DrawString(0,text,10,300+(a*20), 0xff303080);		
	}
	sprintf(text,"Failed Texture Load %d",FailedTextureCount);
	ENGINE.DrawString(0,text,10,420, 0xff303080);	
	sprintf(text,"Texture Memory Free %d",SDSGetFreeTextureMemory());
	ENGINE.DrawString(0,text,10,440, 0xff303080);
	sprintf(text,"System Memory Free %d",SDSFreeMemory());
	ENGINE.DrawString(0,text,10,460, 0xff303080);	
#endif	

}

#endif


#endif
