#ifndef PSXGAMEPANEL_H
#define PSXGAMEPANEL_H

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

class	CDCGamePanel
{
public:
	BOOL	Init();

	void	Process();
	void	Render();
	
	void	Activate(EGPType type, SINT player);
	void	DeActivate();
	void	DeActivateNow();
	
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
	
	SINT		mPlayer;
};

extern	CDCGamePanel GAMEPANEL;

#endif