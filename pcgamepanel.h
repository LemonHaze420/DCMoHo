#include "Common.h"

#ifndef PPCGAMEPANEL_H	
#define PPCGAMEPANEL_H // was PSXGAMEPANEL_H, I am assuming that this is a mistake!!!

#include "Texture.h"

enum	EGPState
{
	GPS_OFF = 0,
	GPS_COMING_ON,
	GPS_ON,
	GPS_GOING_OFF
};

enum	EGPType
{
	GPT_INGAME,
	GPT_WON,
	GPT_LOST,
	GPT_INGAME2,
};



class	CPCGamePanel
{
	friend class CController;		// Need access to state, so I know whether to use cursor keys for navigation.
public:
	BOOL	Init();

	void	Process();
	void	Render();
	
	void	Activate(EGPType type);
	void	DeActivate();
	void	DeActivateNow();

	int		isGamePanelGoing;
	int		GetisGamePanelGoing();
	
protected:
	EGPState	mState;

	GINT		mPosX;
	SINT		mPosY;
	
	SINT		mSelected;
	BOOL		mDBx;
	BOOL		mDBy;
	
	EGPType		mType;
	
	BOOL		mFirstFrame;
	
	SINT		mCounter;
	SINT		mCounterOn;

	// Arrows	
	CTextureTL		*mArrowLeft;	
	CTextureTL		*mArrowRight;	
};

extern	CPCGamePanel GAMEPANEL;

#endif