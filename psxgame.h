#ifndef PSXGAME_H
#define PSXGAME_H

#include	"Game.h"
#include	"MemBuffer.h"

//!PSX write the game!

class	CPSXGame : public CGame
{
public:
	void		ShowSplashScreen();
	BOOL		ShowLoadingScreen(char *nameptr=NULL);

	EGQuitType	RunLevel(SINT level_no, SINT demo_number = -1);
	SINT		Run();

	void		RunAgeingTest();
	void		RunDemo1();
	void		RunDemo3();
	BOOL		ShowDemoScreen(SINT n);

	BOOL		LoadLevel(SINT levelno);
	void		RestartLevel();

	void		TakeScreenShot();
};


#endif
