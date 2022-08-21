#ifndef PCCONTROLLER_H
#define PCCONTROLLER_H

#include "ControllerCommon.h"
#if defined(DEBUGMEM)
#undef new
#endif
#include <list>
#if defined(DEBUGMEM)
#define new DEBUG_NORMALBLOCK
#endif
#include "ltdinput.h"
#include "PCSetupQuery.h"

using namespace std;	// Required for list template.

#define StopVibration() /*nowt*/

// --------------------------------------------------
// Encapsulates a single key mapping entity-thing.
class CFunctionMapping
{
public:
	CFunctionMapping()
	{
		Function = NULL;
		Mapping = NULL;
	}

	CFunctionMapping(SINT _VKey, SINT _FunctionMapping)
	{
		Function = _FunctionMapping;
		Mapping = _VKey;
	}

	~CFunctionMapping()
	{
	}

	// Data.
	SINT Function;	// As in BUTTON_PSX_TRIANGLE
	SINT Mapping;	// and VK_TAB.
};

// - are --------------------------------------------
// Define a new storage class from the list template.
typedef list<CFunctionMapping*> CFunctionMappingList;
typedef list<SINT>				CMappingList;

// - are --------------------------------------------
// A callback type definition. 
typedef void (*MappedKeyIteraterCallbackType) (SINT, SINT, void*);

// - are --------------------------------------------
// Store controller settings.
class CControllerConfig
{
	friend class CControls;
	friend class CController;

public:
	CControllerConfig();
	~CControllerConfig();

	// ----
	// Deletes all internally referenced data
	void ClearUp();

	// ----
	// Make an association between a key and a function.
	// False indicates failure.
	bool MapKey(SINT _VKey, SINT _FunctionMapping);

	// Remove all bindings for a particular key or function.
	void UnMapKey(SINT _VKey);
	void UnMapFunction(SINT _FunctionMapping);

	// ----
	// Return a reference to a list of keys that are 
	// mapped to a particular function, and vice versa.
	CMappingList* WhatKeysAreMappedTo(SINT _Function);
	CMappingList* WhatFunctionsAreMappedBy(SINT _Key);

	// ----
	// Iterates through the list of mapped keys and 
	// functions, calling the _MappedKeyIteraterCallback 
	// function for each entry. 
	// NOTE:	This method does not test key states! That 
	//			is the task of the 
	//			_MappedKeyIteraterCallback function.
	// NOTE:	_This allows the user to pass a 'fake' this 
	//			object to the _MappedKeyIteraterCallback 
	//			function.
	void ProcessMappedKeys(	MappedKeyIteraterCallbackType _MappedKeyIteraterCallback,
							void* _This);

	// ----
	// Process data passed from start up process.
	void AssignFromStartup(tagControllerData* _Controller);

protected:
	CFunctionMappingList	FunctionMappingList;
	CMappingList			MappingList;
};

// **************************************************
class	CController
{
public:
	friend class CControls;
	friend class LTDInput;

	CController() {mIsActive = FALSE;};
	~CController();

	void	Init();
	void	Update();
	void	TriggerVibration(int Vib_Num) {};  // Only used on playstation forcefeedback stuff...

	void	StartRecording(SINT no);
	void	RecordState();
	void	StopRecording();

	void	StartPlayBack(SINT no);
	void	PlayBackState();
	void	StopPlayBack();

	SINT	GetCheat();

	void	UpdateFromPlatform();

	void			SetControllerMode(EControllerMode mode) {mControllerMode = mode;};
	EControllerMode	GetControllerMode() {return mControllerMode;};

	
	EStreamState GetPlaybackState() {return mPlayingBack;}
	
	// Allows a non member function to set the values.
	void	SetAnalogue(SINT _AnalogueEnumeration, GINT _Value)
	{
		ASSERT (_AnalogueEnumeration < NUM_ANALOGUE_CONTROLS);
		mAnalogue[_AnalogueEnumeration] = _Value;
	}

	// Allows a non member unction to modify the values.
	void	AdditiveAnalogue(SINT _AnalogueEnumeration, GINT _Value)
	{
		ASSERT (_AnalogueEnumeration < NUM_ANALOGUE_CONTROLS);
		mAnalogue[_AnalogueEnumeration] += _Value;
	}

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
	void	DeActivate() {mIsActive = FALSE;};
	BOOL	IsActive() {return mIsActive;};

	// Need to be able to store a reference to, and be 
	// able to access reference for, joystick data.
	// The reason being that there is no other 
	// relationship that allows me to access the joystick 
	// data directly. This is a simple solution that works.
	void AssignJoystickDataHandle(DIJOYSTATE* _JoyStick);
	DIJOYSTATE* AccessJoystickData() {return Joystick;}

	
protected:
	SINT	mMouseX;
	SINT	mMouseY;

	ULONG	mButtons1;
	ULONG	mButtons2;

	GINT	mAnalogue[NUM_ANALOGUE_CONTROLS];


	// old for debouncing.

	ULONG	mButtons1Old;
	ULONG	mButtons2Old;

	EControllerMode		mControllerMode;
	EJoyMode			mJoyMode;
	SINT				mInputNumber;

	// record/playback stuff

	FILE	*fp;
	SINT	mFileVersion;

	EStreamState	mRecording;
	EStreamState	mPlayingBack;

	BOOL	mIsActive;
	
	// Link to joystick data.
	DIJOYSTATE*		Joystick;

	// Cheat Stuff
	BOOL	mCheatStarted;
	SINT	mCheatKeys[20];
	SINT	mCheatAvailable;
	SINT	mCheatPos;

public:
	// Store controller config.
	CControllerConfig mControllerConfig;
};

#endif
