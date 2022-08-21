#ifndef DCGAME_H
#define DCGAME_H

#include	"Game.h"
#include	"MemBuffer.h"

//!PSX write the game!

class	CDCGame : public CGame
{
public:
	float mFrameTime ,mBaseTime;

	void		ShowSplashScreen();
	BOOL		ShowLoadingScreen(SINT num);

	EGQuitType	RunLevel(SINT level_no, SINT demo_number = -1);
	SINT		Run();

	void		RunAgeingTest();
	void		RunDemo1();
	void		RunDemo3();
	BOOL		ShowDemoScreen(SINT n);

	BOOL		LoadLevel(SINT levelno);
	void		RestartLevel();

	void		TakeScreenShot();
	GINT        GetFrameRenderFraction(void);
	
	BOOL		mInFrontEnd;
	
	SINT		mMemoryCardRemoved;
	
	BOOL		HasMemoryCardBeenRemoved();
};


#endif
