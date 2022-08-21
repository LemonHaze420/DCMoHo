#ifndef CONTROLS_H
#define CONTROLS_H

#if TARGET == PC
#include	"PCController.h"
#elif TARGET == DC
#include	"DCController.h"
#elif	TARGET == PSX
#include	"PSXController.h"
#endif

#include	"Game.h"

#define NUM_CONTROLLERS		MAX_PLAYERS

class	CControls
{
public:
	CControls() {};
	~CControls() {};

	BOOL	Init();
	void	Shutdown() {};

	void	Update();	// for use outside the game
	void	Process();

	POINT	GetMousePos(SINT cn = 0);
	BOOL	IsButtonPressed(SINT button, SINT cn = 0);
	BOOL	IsButtonPressedDB(SINT button, SINT cn = 0); // debounced version
	BOOL	IsButtonReleasedDB(SINT button, SINT cn = 0); // debounced version
	GINT	GetAnalogueValue(SINT value, SINT cn = 0);
	void	DeActivateContoller(SINT controler_num) { mControllers[controler_num].DeActivate() ; }
	void	ActivateContoller(SINT controler_num) { mControllers[controler_num].Activate() ; }

	void			SetControllerMode(EControllerMode mode, SINT n = -1);
	EControllerMode	GetControllerMode();
	
	BOOL	AreAnyButtonsPressed();

//protected:
	CController		mControllers[NUM_CONTROLLERS];
};


//******************************************************************************************



#endif