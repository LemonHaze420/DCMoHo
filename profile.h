// 23/1 (JCL) Profiler!

#if TARGET == PC

#include "cpu.h"
#define TIMER (RDTSC32())

#elif TARGET == DC
#include	"maincode.h"
#define TIMER (SINT(GetSysTimeFloat() * 1000000.f))
#elif TARGET == PSX

#include <libapi.h>
#include <kernel.h>
extern __declspec(data) volatile int ProfilerTimerTick;
#define TimerToUse RCntCNT2
#define TIMER ProfilerTimerTick
#endif

class PE
{
public:
	enum	EProfile
	{
		// Leave these globals ALONE!!!
		TotalModel = 0,
		TotalEngine,

		// Rendering Stuff
		RenderSky,
		RenderMap,
		RenderThings,
		RenderQSort,
		RenderTextures,
		RenderBeginScene,
		RenderEndScene,
		Render2D,
		RenderFlip,
		RenderDIP,
		RenderGCGamut,
		RenderResetInt,

		// Render Map Stuff
		RMInnerLoop,
		RMDeltas,
		RMTransform,
		RMDraw1,
		RMDraw2,
		RMDrawPanel,
		RMDraw4x4,
		RMStripGeometry,
		RMStripSetup,
		RMExitStrip,
		RMTriggers,
		RMDrawParticles,
		RMDrawPickupBeam,
		RMDrawWater,
		
		// RenderMesh Stuff
		RMMeshTotal,
		RMMeshDrawTransWrapper1,
		RMMeshDrawStartUp,			
		RMMeshDrawMesh,		
		RMMeshDrawHeaders,
		RMMeshDrawLights,				
		RMDrawStadium,

		// Game Stuff
		ProcessMap,
		ProcessMapSt1,
		ProcessMapSt2,
		ProcessWorld,
		ThingProcess,
		GameCollision,
		Character,
		BadGuy,
		FenceCollision,

		// Leave this at the end
		ProfileCount
	};

	enum	EProfilePage
	{
		// Leave this global page ALONE!
		Globals = 0,

		// Custiom pages here
		RenderOverview,
		RMap,
		RMesh,
		GameOverview,

		// Leave this at the end
		ProfilePageCount
	};
};

class	CProfile
{
public:
	ULONG	mStartTime;
	ULONG	mTotalTime, mLastTotalTime;
	SINT	mPage;
	char	mName[50];
	SINT	mCalls, mLastCalls;
};

class	CProfiler
{
public:
	static	void	AddCount(PE::EProfile timer, ULONG count)	{mProfiles[timer].mTotalTime+=count;mProfiles[timer].mCalls++;};
	static	void	Start(PE::EProfile timer)					{mProfiles[timer].mStartTime =TIMER;};
	static	void	End(PE::EProfile timer)						{mProfiles[timer].mTotalTime+=TIMER-mProfiles[timer].mStartTime;mProfiles[timer].mCalls++;};

	static	void	InitTimer(PE::EProfile timer, PE::EProfilePage page, char *name);

	static	void	Init();
	static	void	ResetAll();
	static	void	NextPage();
	static	void	PrevPage();
	static	CProfile*	Fetch();
	static	void	ResetFetch();

#if TARGET == PC
	static	void	DrawProfile(HDC hDC, CProfile *p, SINT y);
#elif TARGET == PSX
	static	void	DrawProfile(CProfile *p, SINT y);
#elif TARGET == DC
	static	void	DrawProfile(CProfile *p, SINT y);
#endif

	static	void	ShowProfile();

private:
	static	CProfile	mProfiles[PE::ProfileCount];
	static	SINT		mCurrentPage;
	static	SINT		mFetcher;
};

class	CProfileTimer
{
public:
	CProfileTimer(PE::EProfile timer)
	{
		mTimer = timer;
		mStartTime = TIMER;
	};
	~CProfileTimer()
	{
		CProfiler::AddCount(mTimer,TIMER - mStartTime);
	};

protected:
	PE::EProfile	mTimer;
	ULONG		mStartTime;
};


// Macros

#if TARGET == PC /*|| TARGET == DC*/
#define _PROFILE
#endif

#ifdef _PROFILE
#define ENABLE_PROFILER 1
#else
#define ENABLE_PROFILER 0
#endif


#if ENABLE_PROFILER==1

#define STOP_PSX_PROF() StopRCnt(TimerToUse)
#define START_PSX_PROF() StartRCnt(TimerToUse)
#define PROFILE_FN(x)		CProfileTimer temp##x(PE::x)
#define PROFILE_START(x)	CProfiler::Start(PE::x)
#define PROFILE_END(x)		CProfiler::End(PE::x)

#else

#define STOP_PSX_PROF()
#define START_PSX_PROF()
#define PROFILE_FN(x)
#define PROFILE_START(x)
#define PROFILE_END(x)

#endif
