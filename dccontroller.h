#ifndef PSXCONTROLLER_H
#define PSXCONTROLLER_H

#include	"ControllerCommon.h"

extern void StopVibration();

class	CController
{
public:
	friend class CControls;

	CController() {mIsActive = FALSE;};
	~CController() {};

	void	Init()
	{
		mControllerMode = CM_JOY_ALT1;
		mCheatStarted = FALSE;
		mCheatAvailable = -1;
		mCheatPos = 0;
	mButtons1 = mButtons1Old = 0;
	mButtons2 = mButtons2Old = 0;

	SINT	c0;
	for (c0 = 0; c0 < NUM_ANALOGUE_CONTROLS; c0 ++)
		mAnalogue[c0] = G0;

	//mControllerMode = (EControllerMode)GAME.GetParams()->mController;


	//if(mControllerMode>=CM_INVALID) mControllerMode = CM_MOUSE;

	//mJoyMode = (EJoyMode)GAME.GetParams()->mJoyMode;
	//if(mJoyMode>=JM_INVALID) mJoyMode = JM_NASTY;

	mRecording = NOT_ACTIVE;
	mPlayingBack = NOT_ACTIVE;
	//mFileVersion = 0;
	//fp = NULL;

	//Joystick = NULL;		
		
		
		
	};
	void	Update();
	void	TriggerVibration(int Vib_Num);
	void	ProcessVibration();

	void	StartRecording(SINT no);
	void	RecordState();
	void	StopRecording();

	EStreamState	GetRecordState() {return mRecording;};
	EStreamState	GetPlaybackState() {return mPlayingBack;};

	void	StartPlayBack(SINT no);
	void	PlayBackState();
	void	StopPlayBack();

	void	UpdateFromPlatform() {};

	void			SetControllerMode(EControllerMode mode) {mControllerMode = mode;};
	EControllerMode	GetControllerMode() {return mControllerMode;};

	void	SetButton(SINT n)
	{
		if (n >= 32)
			mButtons2 |= (1 << (n - 32));
		else
			mButtons1 |= (1 <<  n      );
	}

	BOOL	IsButtonSet(SINT n)
	{
		if (n >= 32)
			return mButtons2 & (1 << (n - 32));
		else
			return mButtons1 & (1 <<  n      );
	}

	BOOL	IsButtonOldSet(SINT n)
	{
		if (n >= 32)
			return mButtons2Old & (1 << (n - 32));
		else
			return mButtons1Old & (1 <<  n      );
	}

	void	SetInputNumber(SINT n)
	{
		mInputNumber = n;
	}

	void	Activate() {mIsActive = TRUE;};
	void	DeActivate() {mIsActive = FALSE ;};
	BOOL	IsActive() {return mIsActive;};

	SINT	GetCheat();

	BOOL	AreAnyButtonsPressed();

protected:
	SINT	mMouseX;
	SINT	mMouseY;

	ULONG	mButtons1, mButtons2;
    GINT    mAnalogue[NUM_ANALOGUE_CONTROLS];

	// old for debouncing.

	ULONG	mButtons1Old, mButtons2Old;

	EControllerMode		mControllerMode;
	SINT				mInputNumber;

	BOOL	mIsActive;

    // record/playback stuff
	EStreamState	mRecording;
	EStreamState	mPlayingBack;

	char	*mPlaybackPtr;
	SINT	mPlaybackSize;

	// Cheat Stuff
	BOOL	mCheatStarted;
	SINT	mCheatKeys[20];
	SINT	mCheatAvailable;
	SINT	mCheatPos;
};

#endif
