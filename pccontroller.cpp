#include	"Common.h"

#if TARGET == PC

#include	"Globals.h"
#include	"PCController.h"
#include	<time.h>
#include	"PCSetupQuery.h"
#include	"PCGamePanel.h"

#include 	"DCCheats.h"

struct	SCheat
{
	SINT	mNumKeys;
	SINT	mKeys[20];
}	CHEATS[] = 
	{
		{ 15, {3, 4, 3, 1, 3, 2, 3, 3, 4, 3, 1, 1, 3, 2, 3}},		// all prisons  (TTnP)
		{ 11, {3, 4, 1, 1, 3, 3, 4, 1, 2, 4, 4}},					// Bouncy   (Boing)
//		{ 13, {3, 3, 3, 4, 3, 1, 1, 1, 4, 3, 1, 1, 2}},				// Bonus   	(Bruce)
		{ 13, {1, 1, 1, 3, 1, 2, 2, 2, 3, 1, 2, 2, 4}},				// Bonus   	(Bruce)
//		{ 12, {3, 3, 4, 2, 1, 4, 2, 3, 1, 4, 4, 1}},				// Doctor  	(temp)
		{ 12, {2, 2, 1, 3, 4, 5, 1, 4, 3, 2, 2, 3}},				// Doctor  	(temp)
//		{ 10, {3, 3, 3, 4, 2, 3, 3, 3, 2, 1}},						// version	(LtL)
		{ 10, {2, 2, 2, 1, 3, 2, 2, 2, 3, 4}},						// version	(LtL)
		{ 16, {4, 3, 4, 3, 4, 3, 1, 2, 1, 3, 1, 3, 4, 3, 4, 2}},	// slippery (6:00)
	};


CControllerConfig::CControllerConfig()
{
	FunctionMappingList.clear();
	MappingList.clear();
}

CControllerConfig::~CControllerConfig()
{
	ClearUp();
}

// ----
// Deletes all internally referenced data
void CControllerConfig::ClearUp()
{
	// Okay. empty would appear to fail in debug and work in release.
	// I.E. In debug there will be memory leaks, and there shouldn't be in release.
	FunctionMappingList.erase(FunctionMappingList.begin(), FunctionMappingList.end());

/*	CFunctionMappingList::iterator i;
	CFunctionMapping* temp;

	i = FunctionMappingList.begin();
	while (i != FunctionMappingList.end())
	{
		temp = (*i);
		FunctionMappingList.erase(i);
		delete temp;
		
		i = FunctionMappingList.begin();
	}*/
}

// ----
// Make an association between a key and a function.
// False indicates failure.
bool CControllerConfig::MapKey(SINT _VKey, SINT _FunctionMapping)
{
	if ((_VKey >= 0) && (_FunctionMapping >= 0) &&
		(_VKey <= 999) && (_FunctionMapping <= 999))
	{
		CFunctionMapping* FM = new CFunctionMapping();
		if (FM) 
		{
			FM->Function = _FunctionMapping;
			FM->Mapping = _VKey;
			FunctionMappingList.push_front(FM);
			return true;
		}
	}
	return false;
}

// Remove all bindings for a particular key.
void CControllerConfig::UnMapKey(SINT _VKey)
{
	// I could almost grow to like this template linked list 
	CFunctionMappingList::iterator i;
	CFunctionMapping* temp;

	for (i = FunctionMappingList.begin(); i != FunctionMappingList.end(); ++i)
	{
		temp = (*i);
		if (temp->Mapping == _VKey)
		{
			FunctionMappingList.remove(temp);
			delete temp;
		}
	}
}

// Remove all bindings for a particular function.
void CControllerConfig::UnMapFunction(SINT _FunctionMapping)
{
	CFunctionMappingList::iterator i;
	CFunctionMapping* temp;

	for (i = FunctionMappingList.begin(); i != FunctionMappingList.end(); ++i)
	{
		temp = (*i);
		if (temp->Function == _FunctionMapping)
		{
			FunctionMappingList.remove(temp);
			delete temp;
		}
	}
}

// ----
// Return a reference to a list of keys that are 
// mapped to a particular function.
CMappingList* CControllerConfig::WhatKeysAreMappedTo(SINT _Function)
{
	CFunctionMappingList::iterator i;
	CFunctionMapping* temp;

	MappingList.clear();

	// Search master list and assign matching items.
	for (i = FunctionMappingList.begin(); i != FunctionMappingList.end(); ++i)
	{
		temp = (*i);
		if (temp->Function == _Function)
		{
			MappingList.push_front(temp->Mapping);
		}
	}

	return &MappingList;
}

// Return a reference to a list of functions that are 
// mapped to a particular key.
CMappingList* CControllerConfig::WhatFunctionsAreMappedBy(SINT _Key)
{
	CFunctionMappingList::iterator i;
	CFunctionMapping* temp;

	MappingList.clear();
	for (i = FunctionMappingList.begin(); i != FunctionMappingList.end(); ++i)
	{
		temp = (*i);
		if (temp->Mapping == _Key)
		{
			MappingList.push_front(temp->Function);
		}
	}

	return &MappingList;
}

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
void CControllerConfig::ProcessMappedKeys(MappedKeyIteraterCallbackType _MappedKeyIteraterCallback,
										  void* _This)
{
	CFunctionMappingList::iterator i;
	CFunctionMapping* temp;

	if (_MappedKeyIteraterCallback)
	{
		for (i = FunctionMappingList.begin(); i != FunctionMappingList.end(); ++i)
		{
			temp = (*i);
			_MappedKeyIteraterCallback(temp->Mapping, temp->Function, _This);
		}
	}
}


// ----
// Process data passed from start up process.
void CControllerConfig::AssignFromStartup(tagControllerData* _Controller)
{
	int i, j;
	
	j = 0;
	for (i = mySystem->GetNumberOfInputDevices(); i--;)
	{
		if (!strcmp(_Controller->Name, mySystem->GetInputDevice(i)->Info.tszInstanceName))
		{
			if (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(i)->Info.dwDevType) == DIDEVTYPE_JOYSTICK)
			{
				j = 256;
			}
			break;
		}
	}

	for (i = 10; i--;)
	{
		MapKey(		_Controller->ConfigKeyMapping[i].Button		// Value as passed
				+	j											// + 256 if a joystick button
				+	((j && (i<4))?256:0),						// + another 256 if j is set, AND i < 4 - i.e it is an axis. (Axis will come out in the range 512->. 
				MenuIndexToInGameFunction(i));
	}
}


//******************************************************************************************
CController::~CController()
{
	if (mRecording)
		StopRecording();

	if (mPlayingBack)
		StopPlayBack();
}

//******************************************************************************************
void	CController::Init()
{
	mButtons1 = mButtons1Old = 0;
	mButtons2 = mButtons2Old = 0;

	SINT	c0;
	for (c0 = 0; c0 < NUM_ANALOGUE_CONTROLS; c0 ++)
		mAnalogue[c0] = G0;

	mControllerMode = (EControllerMode)GAME.GetParams()->mController;


	if(mControllerMode>=CM_INVALID) mControllerMode = CM_MOUSE;

	mJoyMode = (EJoyMode)GAME.GetParams()->mJoyMode;
	if(mJoyMode>=JM_INVALID) mJoyMode = JM_NASTY;

	mRecording = NOT_ACTIVE;
	mPlayingBack = NOT_ACTIVE;
	mFileVersion = 0;
	fp = NULL;

	Joystick = NULL;

	mCheatStarted = FALSE;
	mCheatAvailable = -1;
	mCheatPos = 0;

}

void CController::AssignJoystickDataHandle(DIJOYSTATE* _JoyStick) 
{
	this->Joystick = _JoyStick;
}

//******************************************************************************************
#define DEMO_VERSON 1

//******************************************************************************************
void	CController::StartRecording(SINT no)
{
	ASSERT(mRecording != ACTIVE);
	ASSERT(mPlayingBack != ACTIVE);
	ASSERT(!fp);

	char	str[256];
	sprintf(str, "data\\Demos\\demo%d.dem", no);
	fp = fopen(str, "wb");
	ASSERT(fp);
	if (!fp)
		return;

	SINT	foo;

	foo = DEMO_VERSON;
	fwrite(&foo, 4, 1, fp);
	foo = GAME.GetCurrentLevelNumber();
	fwrite(&foo, 4, 1, fp);

/*	sprintf(str,"%d  ; Demo Version Number\n", DEMO_VERSON);
	fputs(str, fp);
	sprintf(str,"%d  ; Starting Level\n", GAME.GetCurrentLevelNumber());
	fputs(str, fp);*/

	mRecording = ACTIVE;

	sprintf(str, "Started Recording Demo %d", no);
	CONSOLE.AddString(str);

	BOOL res = GAME.LoadLevel(GAME.GetCurrentLevelNumber());
	if (!(res))
	{
		char text[200];
		sprintf(text, "Level %03d failed to load.", GAME.GetCurrentLevelNumber());
		CONSOLE.AddString(text);
	}
}

//******************************************************************************************
void	CController::RecordState()
{
	ASSERT(mRecording == ACTIVE);
	ASSERT(fp);

/*	char str[256];
	sprintf(str, "%d %d %u %u ", mMouseX, mMouseY, mButtons1, mButtons2);
	fputs(str, fp);

	SINT c0;
	for (c0 = 0; c0 < NUM_ANALOGUE_CONTROLS; c0 ++)
	{
		sprintf(str, "%d ", mAnalogue[c0]);   // note the GINT to INT conversion...
		fputs(str, fp);
	}

	sprintf(str, "\n");*/
//	fputs(str, fp);

	fwrite(&mButtons1, 4, 1, fp);
	fwrite(&mButtons2, 4, 1, fp);
	fwrite(&mAnalogue[0], 4, 1, fp);
	fwrite(&mAnalogue[1], 4, 1, fp);

}

//******************************************************************************************
void	CController::StopRecording()
{
	ASSERT(fp);
	ASSERT(mRecording == ACTIVE);

	fclose(fp);
	fp = NULL;
	mRecording = NOT_ACTIVE;

	CONSOLE.AddString("Stopped Recording.");
}

//******************************************************************************************
void	CController::StartPlayBack(SINT no)
{
	ASSERT(mRecording != ACTIVE);
	ASSERT(mPlayingBack != ACTIVE);
	ASSERT(!fp);

	char	str[256];
	sprintf(str, "data\\Demos\\demo%d.dem", no);
	fp = fopen(str, "rb");
	if (!fp)
	{
		CONSOLE.AddString("Demo not found");
		mPlayingBack = NOT_ACTIVE;
		return;
	}

//	fgets(str, 256, fp);
//	sscanf(str,"%d", &mFileVersion);
	fread(&mFileVersion, 4, 1, fp);
	ASSERT(mFileVersion == 1);

	SINT	l;
//	fgets(str, 256, fp);
//	sscanf(str,"%d", &l);
	fread(&l, 4, 1, fp);

	mPlayingBack = ACTIVE;
	sprintf(str, "Started Playing Back Demo %d", no);
	CONSOLE.AddString(str);

	BOOL res = GAME.LoadLevel(l);
	if (!(res))
	{
		char text[200];
		sprintf(text, "Level %03d failed to load.", l);
		CONSOLE.AddString(text);
	}
}

//******************************************************************************************
void	CController::PlayBackState()
{
	ASSERT(mPlayingBack == ACTIVE);
	ASSERT(fp);

	if (feof(fp))
	{
		StopPlayBack();
		return;
	}

//	char str[256];
//	fgets(str, 256, fp);

	//! hmm..
//	sscanf(str, "%d %d %u %u %d %d", &mMouseX, &mMouseY, &mButtons1, &mButtons2, &mAnalogue[0], &mAnalogue[1]); // note the GINT to int conversion


//	SINT c0;
//	for (c0 = 0; c0 < NUM_ANALOGUE_CONTROLS; c0 ++)
//	{
//		sprintf(str, "%d ", mAnalogue[c0]);
//		fputs(str, fp);
//	}

	fread(&mButtons1, 4, 1, fp);
	fread(&mButtons2, 4, 1, fp);
	fread(&mAnalogue[0], 4, 1, fp);
	fread(&mAnalogue[1], 4, 1, fp);
}

//******************************************************************************************
void	CController::StopPlayBack()
{
	ASSERT(fp);
	ASSERT(mPlayingBack == ACTIVE);

	fclose(fp);
	fp = NULL;
	mPlayingBack = NOT_ACTIVE;

	CONSOLE.AddString("Stopped PlayBack.");
}

// ----
SINT	CController::GetCheat()
{
	return mCheatAvailable;
}

//******************************************************************************************
void	CController::UpdateFromPlatform()
{

	POINT		p			= PLATFORM.GetMousePos();

	mMouseX = p.x;
	mMouseY = p.y;
#if 0

	// first, check controller change
	// are: Note the additional check to make sure that 
	// some other part of the program has registered an
	// interest in obtaining keyboard input. Obviously, 
	// if they are using the keyboard the user might hit 
	// 't', not realising the effect it has here. 
	if ((PLATFORM.mKeyChangeCallback == NULL) && 
		PLATFORM.IsKeyDown('T') && 
		(!(IsButtonSet(BUTTON_CHANGE_CONTROL_MODE))))
	{
		SetButton(BUTTON_CHANGE_CONTROL_MODE);

		switch(mControllerMode)
		{
		case CM_MOUSE:
			mControllerMode = CM_KEYBOARD;
			CONSOLE.AddString("Keyboard Controller Selected.");
			break;
		case CM_KEYBOARD:
			mControllerMode = CM_JOYPAD;
			CONSOLE.AddString("Joypad Controller Selected.");
			break;
		case CM_JOYPAD:
			mControllerMode = CM_KEY_ALT1;
			CONSOLE.AddString("Keyboard Alt 1 Controller Selected.");
			break;
		case CM_KEY_ALT1:
			mControllerMode = CM_JOY_ALT1;
			CONSOLE.AddString("Joypad Alt 1 Controller Selected.");
			break;
		case CM_JOY_ALT1:
			mControllerMode = CM_JOY_REL;
			CONSOLE.AddString("Relative Joystick Controller Selected.");
			break;
		case CM_JOY_REL:
			mControllerMode = CM_JOY_GOLF;
			CONSOLE.AddString("Joypad Golf Controller Selected.");
			break;
		case CM_JOY_GOLF:
			mControllerMode = CM_JOY_SHUTTLE;
			CONSOLE.AddString("Joypad Shuttle Controller Selected.");
			break;
		case CM_JOY_SHUTTLE:
			mControllerMode = CM_JOY_RACER;
			CONSOLE.AddString("Racer Controller Selected.");
			break;
		case CM_JOY_RACER:
			mControllerMode = CM_JOY_DIRECT;
			CONSOLE.AddString("Direct Joypad Controller Selected.");
			break;
		case CM_JOY_DIRECT:
			mControllerMode = CM_KEY_DIRECT;
			CONSOLE.AddString("Direct Keyboard Controller Selected.");
			break;
		case CM_KEY_DIRECT:
			mControllerMode = CM_MOUSE;
			CONSOLE.AddString("Mouse Controller Selected.");
			break;
		};
	}

	//! USE GetKeyboardState() in platform!!

	// controller specific stuff first

	switch (mControllerMode)
	{
	case CM_MOUSE:
		{
			if (PLATFORM.IsPrimaryFirePressed())
				SetButton(BUTTON_FIRE_PRIMARY);

			if (PLATFORM.IsSecondaryFirePressed())
				SetButton(BUTTON_FIRE_SECONDARY);

			if (PLATFORM.IsKeyDown('B'))
				SetButton(BUTTON_FLIPPER_RIGHT);

			if (PLATFORM.IsKeyDown('V'))
				SetButton(BUTTON_FLIPPER_LEFT);

			if (PLATFORM.IsKeyDown('I'))
				SetButton(BUTTON_NEXT_PRIMARY);

			if (PLATFORM.IsKeyDown('L'))
				SetButton(BUTTON_BRAKE);

		}
		break;

	case CM_KEYBOARD:
	case CM_KEY_ALT1:
	case CM_KEY_DIRECT:
		{
			
			mAnalogue[ANALOGUE_LEFT_RIGHT] = G0;
			mAnalogue[ANALOGUE_DOWN_UP] = G0;

			if (PLATFORM.IsKeyDown(VK_SPACE))
				SetButton(BUTTON_FIRE_PRIMARY);

			if (PLATFORM.IsKeyDown(VK_NUMPAD0))
				SetButton(BUTTON_FIRE_SECONDARY);

			if (PLATFORM.IsKeyDown('B'))
				SetButton(BUTTON_FLIPPER_RIGHT);

			if (PLATFORM.IsKeyDown('V'))
				SetButton(BUTTON_FLIPPER_LEFT);

			if (PLATFORM.IsKeyDown('I'))
				SetButton(BUTTON_NEXT_PRIMARY);

			if (PLATFORM.IsKeyDown('L'))
				SetButton(BUTTON_BRAKE);

			if (PLATFORM.IsKeyDown('S'))
				SetButton(BUTTON_BLOCK);
		}
		break;
	case CM_JOYPAD:
	case CM_JOY_ALT1:
	case CM_JOY_REL:
	case CM_JOY_GOLF:
	case CM_JOY_SHUTTLE:
	case CM_JOY_RACER:
	case CM_JOY_DIRECT:
		// nasty hard coded joystick stuff
		switch(mJoyMode)
		{
			case JM_NASTY:
				if (joy->rgbButtons[0] & 0x80)
					SetButton(BUTTON_FIRE_PRIMARY);

				if (joy->rgbButtons[1] & 0x80)
					SetButton(BUTTON_FIRE_SECONDARY);

				if (joy->rgbButtons[2] & 0x80)
					SetButton(BUTTON_FLIPPER_LEFT);

				if (joy->rgbButtons[3] & 0x80)
					SetButton(BUTTON_FLIPPER_RIGHT);

				if (joy->rgbButtons[4] & 0x80)
					SetButton(BUTTON_BLOCK);

				if (joy->rgbButtons[5] & 0x80)
					SetButton(BUTTON_THROW);

				if (joy->lZ < 0)
					SetButton(BUTTON_NEXT_PRIMARY);

				if (joy->lRz < 0)
					SetButton(BUTTON_BRAKE);
				break;
			case JM_XTERMINATOR:
				if (joy->rgbButtons[0] & 0x80)
					SetButton(BUTTON_FIRE_PRIMARY);
				if (joy->rgbButtons[3] & 0x80)
					SetButton(BUTTON_BLOCK);
				if (joy->rgbButtons[9] & 0x80)
					SetButton(BUTTON_FIRE_PRIMARY);

				if (joy->rgbButtons[1] & 0x80)
					SetButton(BUTTON_FIRE_SECONDARY);

				if (joy->rgbButtons[5] & 0x80)
					SetButton(BUTTON_THROW);

				if (joy->rgbButtons[6] & 0x80)
					SetButton(BUTTON_FLIPPER_LEFT);

				if (joy->rgbButtons[7] & 0x80)
					SetButton(BUTTON_FLIPPER_RIGHT);

				if (joy->rgbButtons[4] & 0x80)
					SetButton(BUTTON_TRICK);

				if (joy->lRz < -500)
					SetButton(BUTTON_NEXT_PRIMARY);

				if (joy->lRz > 500)
					SetButton(BUTTON_BRAKE);
				break;

		}
		break;
	};


	if (PLATFORM.IsKeyDown(VK_HOME))
		SetButton(BUTTON_ZOOM_OUT);

	if (PLATFORM.IsKeyDown(VK_END))
		SetButton(BUTTON_ZOOM_IN);

	if (PLATFORM.IsKeyDown(VK_RETURN))
		SetButton(BUTTON_FOLLOW_TOGGLE);

	if (PLATFORM.IsKeyDown(VK_F7))
		SetButton(BUTTON_WIDE_VIEW);

	if (PLATFORM.IsKeyDown(VK_F8))
		SetButton(BUTTON_TIGHT_VIEW);

	if (PLATFORM.IsKeyDown(VK_BACK))
		SetButton(BUTTON_CHANGE_WEAPON);

	if (PLATFORM.IsKeyDown('P'))
		SetButton(BUTTON_PAUSE);

	if (PLATFORM.IsKeyDown('Y')) 
		SetButton(BUTTON_NEXT_FRAME) ;

	if (PLATFORM.IsKeyDown('H'))
		SetButton(BUTTON_HELP);

	if (PLATFORM.IsKeyDown(VK_DELETE))
		SetButton(BUTTON_SPIN_CLOCKWISE);

	if (PLATFORM.IsKeyDown(VK_NEXT))
		SetButton(BUTTON_SPIN_ANTICLOCKWISE);

	if (PLATFORM.IsKeyDown(VK_INSERT))
		SetButton(BUTTON_SPIN_CENTRE);

	
	if (PLATFORM.IsKeyDown('J'))
		SetButton(BUTTON_TOGGLE_MAPDRAW);
	
	if (PLATFORM.IsKeyDown('F'))
		SetButton(BUTTON_CHANGE_GAMUT);

	if (PLATFORM.IsKeyDown(VK_F12))
		SetButton(BUTTON_SUSPEND);

	if (PLATFORM.IsKeyDown(VK_F3))
		SetButton(BUTTON_PROFILE_NEXT);

	if (PLATFORM.IsKeyDown(VK_F2))
		SetButton(BUTTON_PROFILE_PREV);

	if (PLATFORM.IsKeyDown(VK_F1))
		SetButton(BUTTON_TOGGLE_PROFILER);

	if (PLATFORM.IsKeyDown(VK_F4))
		SetButton(BUTTON_TOGGLE_DEBUG_TEXT);

	if (PLATFORM.IsKeyDown(VK_F6))
		SetButton(BUTTON_EDITOR);

	if (PLATFORM.IsKeyDown('O'))
		SetButton(BUTTON_TOGGLE_COLLISION_INFO);
	
	if (PLATFORM.IsKeyDown('C'))
		SetButton(BUTTON_TOGGLE_CHARACTER_INFO);

	if (PLATFORM.IsKeyDown('M'))
		SetButton(BUTTON_TOGGLE_MAPWHO_INFO);

	if (PLATFORM.IsKeyDown('D'))
		SetButton(BUTTON_TOGGLE_2D);

	if (PLATFORM.IsKeyDown('G'))
		SetButton(BUTTON_TELEPORT);

	if (PLATFORM.IsKeyDown('K'))
		SetButton(BUTTON_TRICK);
	
	if (PLATFORM.IsKeyDown(VK_CONTROL))
		SetButton(BUTTON_SHIFT);
	
	if (PLATFORM.IsKeyDown('9'))
		SetButton(BUTTON_RETREAT_LEVEL);

	if (PLATFORM.IsKeyDown('0'))
		SetButton(BUTTON_ADVANCE_LEVEL);

	if (PLATFORM.IsKeyDown('N'))
		SetButton(BUTTON_RELIGHT_LEVEL);

	if (PLATFORM.IsKeyDown('A'))
		SetButton(BUTTON_UNLIGHT_LEVEL);

	if (PLATFORM.IsKeyDown('U'))
		SetButton(BUTTON_UPLOAD_TEXTURE);
	if (PLATFORM.IsKeyDown('<'))
		SetButton(BUTTON_UPLOAD_TEXTURE);
	
	if (PLATFORM.IsKeyDown(VK_ESCAPE))
		SetButton(BUTTON_PSX_TRIANGLE);

	// Analogue Controls
	switch (mControllerMode)
	{
	case CM_MOUSE:
	case CM_KEYBOARD:
	case CM_KEY_ALT1:
	case CM_KEY_DIRECT:

		mAnalogue[ANALOGUE_LEFT_RIGHT] = G0;
		mAnalogue[ANALOGUE_DOWN_UP] = G0;
 
		if (PLATFORM.IsKeyDown(VK_LEFT))
			mAnalogue[ANALOGUE_LEFT_RIGHT] -= G1;
		if (PLATFORM.IsKeyDown(VK_RIGHT))
			mAnalogue[ANALOGUE_LEFT_RIGHT] += G1;

		if (PLATFORM.IsKeyDown(VK_DOWN))
			mAnalogue[ANALOGUE_DOWN_UP] -= G1;
		if (PLATFORM.IsKeyDown(VK_UP))
			mAnalogue[ANALOGUE_DOWN_UP] += G1;
		break;

	case CM_JOY_REL:
	case CM_JOY_ALT1:
	case CM_JOYPAD:
	case CM_JOY_GOLF:
	case CM_JOY_SHUTTLE:
	case CM_JOY_RACER:
	case CM_JOY_DIRECT:

		mAnalogue[ANALOGUE_LEFT_RIGHT] =  G(joy->lX) / 800;
		mAnalogue[ANALOGUE_DOWN_UP]    = -G(joy->lY) / 800;

		if (mAnalogue[ANALOGUE_LEFT_RIGHT] >  G1)  mAnalogue[ANALOGUE_LEFT_RIGHT] =  G1;
		if (mAnalogue[ANALOGUE_LEFT_RIGHT] < -G1)  mAnalogue[ANALOGUE_LEFT_RIGHT] = -G1;
		if (mAnalogue[ANALOGUE_DOWN_UP   ] >  G1)  mAnalogue[ANALOGUE_DOWN_UP   ] =  G1;
		if (mAnalogue[ANALOGUE_DOWN_UP   ] < -G1)  mAnalogue[ANALOGUE_DOWN_UP   ] = -G1;

		if (GABS(mAnalogue[ANALOGUE_LEFT_RIGHT]) < G(0, 16384)) mAnalogue[ANALOGUE_LEFT_RIGHT] = G0;
		if (GABS(mAnalogue[ANALOGUE_DOWN_UP   ]) < G(0, 16384)) mAnalogue[ANALOGUE_DOWN_UP   ] = G0;

		break;
	};
#endif
}

// - are --------------------------------------------------------------
// A callback function to test key states and set 
// Not a member function I know, but it is used in the method 
// below, thus this seems the logical place to put it.
void ProcKeyList(SINT _Key, SINT _Function, void* _This)
{
	CController*	This		= (CController*)_This;
	DIJOYSTATE*		Joystick	= This->AccessJoystickData();

	// Values in the range 0->255 are taken to denote keys, 
	// values outside that range are used for joystick and 
	// other input devices.
	if (	(_Key >= 0) 
		&& 	(_Key < 256))
	{
		// Basically, do I allow the user to type a
		// filename using the keyboard? Currently the case 
		// is yes, but it means disabling the controller 
		// keyboard mappings. Not really ideal, as the 
		// user has to use esc for exiting, and not the 
		// usual controller mapping!
		if (	(!PLATFORM.GetKeyChangeCallback()
			&&	(PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[_Key]) & 0x80)
			)
		{
			switch (_Function)
			{
			case BUTTON_ANALOGUE_LEFT:
				This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT, -G1);
				return;

			case BUTTON_ANALOGUE_RIGHT:
				This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT, G1);
				return;

			case BUTTON_ANALOGUE_DOWN:
				This->AdditiveAnalogue(ANALOGUE_DOWN_UP, -G1);
				return;
		
			case BUTTON_ANALOGUE_UP:
				This->AdditiveAnalogue(ANALOGUE_DOWN_UP, G1);
				return;
	
			default:
				This->SetButton(_Function);
			}
		}
		else
		{
			if (PLATFORM.IsKeyDown(VK_ESCAPE))
			This->SetButton(BUTTON_PSX_TRIANGLE);
		}
	}
	else
	{
		if (_Key >= 256)
		{
			SINT JoyButton = _Key - 256;
			if (JoyButton < 32)
			{
				if (	(Joystick) 
					&&	(Joystick->rgbButtons[JoyButton] & 0x80)
					)
				{
					This->SetButton(_Function);
				}
			}
			else
			{
				// Right, joystick buttons higher than 255 indicate an axis. 
				// So, JoyButton - 256 == axis number.
				if (JoyButton >= 255)
				{
					if (Joystick)
					{
						JoyButton -= 256;

						// JoyButton will equal negative one if no axis is defined. 
						// For a joystick this will always be the case for the 
						// down and right actions. (Up and left provide the 
						// associated mappings, respectively).

						switch (JoyButton)
						{
						case -1:
							// Can ignore this case.
							break;

						case 0:	// Axis 0
							if (_Function == BUTTON_ANALOGUE_UP)
								This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lX) / 800);
							if (_Function == BUTTON_ANALOGUE_LEFT)
								This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lX) / 800);
							break;

						case 1:	// Axis 1
							if (_Function == BUTTON_ANALOGUE_UP)
								This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lY) / 800);
							if (_Function == BUTTON_ANALOGUE_LEFT)
								This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lY) / 800);
							break;

						case 2:	// Axis 2
							if (_Function == BUTTON_ANALOGUE_UP)
								This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lZ) / 800);
							if (_Function == BUTTON_ANALOGUE_LEFT)
								This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lZ) / 800);
							break;

						case 3:	// Axis 3
							if (_Function == BUTTON_ANALOGUE_UP)
								This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lRx) / 800);
							if (_Function == BUTTON_ANALOGUE_LEFT)
								This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lRx) / 800);
							break;

						case 4:	// Axis 4
							if (_Function == BUTTON_ANALOGUE_UP)
								This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lRy) / 800);
							if (_Function == BUTTON_ANALOGUE_LEFT)
								This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lRy) / 800);
							break;

						case 5:	// Axis 5
							if (_Function == BUTTON_ANALOGUE_UP)
								This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lRz) / 800);
							if (_Function == BUTTON_ANALOGUE_LEFT)
								This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lRz) / 800);
							break;
						default:
							OutputDebugString("Axis out of range.\n");
							break;
						}
					}
				}
			}
		}
	}
}

// - are --------------------------------------------------------------
// A callback function to test key states and set 
// Not a member function I know, but it is used in the method 
// below, thus this seems the logical place to put it.
void FrontEndProcKeyList(SINT _Key, SINT _Function, void* _This)
{
	CController*	This		= (CController*)_This;
	DIJOYSTATE*		Joystick	= This->AccessJoystickData();

	if (_Key >= 256)
	{
		if (Joystick) 
		{
			if (Joystick->rgbButtons[0] & 0x80)
			{
				This->SetButton(BUTTON_FIRE_PRIMARY);
				This->SetButton(BUTTON_PSX_X);
			}

			if (Joystick->rgbButtons[1] & 0x80)
			{
				This->SetButton(BUTTON_PSX_TRIANGLE);
			}
		

			SINT JoyButton = _Key - 256;

			// Right, joystick buttons higher than 255 indicate an axis. 
			// So, JoyButton - 256 == axis number.
			if (JoyButton >= 255)
			{
				if (Joystick)
				{
					JoyButton -= 256;

					// JoyButton will equal negative one if no axis is defined. 
					// For a joystick this will always be the case for the 
					// down and right actions. (Up and left provide the 
					// associated mappings, respectively).

					switch (JoyButton)
					{
					case -1:
						// Can ignore this case.
						break;

					case 0:	// Axis 0
						if (_Function == BUTTON_ANALOGUE_UP)
							This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lX) / 800);
						if (_Function == BUTTON_ANALOGUE_LEFT)
							This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lX) / 800);
						break;

					case 1:	// Axis 1
						if (_Function == BUTTON_ANALOGUE_UP)
							This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lY) / 800);
						if (_Function == BUTTON_ANALOGUE_LEFT)
							This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lY) / 800);
						break;

					case 2:	// Axis 2
						if (_Function == BUTTON_ANALOGUE_UP)
							This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lZ) / 800);
						if (_Function == BUTTON_ANALOGUE_LEFT)
							This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lZ) / 800);
						break;

					case 3:	// Axis 3
						if (_Function == BUTTON_ANALOGUE_UP)
							This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lRx) / 800);
						if (_Function == BUTTON_ANALOGUE_LEFT)
							This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lRx) / 800);
						break;

					case 4:	// Axis 4
						if (_Function == BUTTON_ANALOGUE_UP)
							This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lRy) / 800);
						if (_Function == BUTTON_ANALOGUE_LEFT)
							This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lRy) / 800);
						break;

					case 5:	// Axis 5
						if (_Function == BUTTON_ANALOGUE_UP)
							This->AdditiveAnalogue(ANALOGUE_DOWN_UP,   -G(Joystick->lRz) / 800);
						if (_Function == BUTTON_ANALOGUE_LEFT)
							This->AdditiveAnalogue(ANALOGUE_LEFT_RIGHT,   G(Joystick->lRz) / 800);
						break;
					default:
						OutputDebugString("Axis out of range.\n");
						break;
					}
				}
			}
		}
	}
}

//******************************************************************************************
void	CController::Update()
{
	// Backup state
	mButtons1Old = mButtons1;
	mButtons2Old = mButtons2;

	// Reset to zero state
	mAnalogue[ANALOGUE_LEFT_RIGHT] = G0;
	mAnalogue[ANALOGUE_DOWN_UP] = G0;
		
	mButtons1 = 0;
	mButtons2 = 0;

	if (	(GAME.mInFrontEnd) 
		||	(GAMEPANEL.mState == GPS_COMING_ON) 
		||	(GAMEPANEL.mState == GPS_ON)
		)
	{
		// UP
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_UP]) & 0x80)
		{
			AdditiveAnalogue(ANALOGUE_DOWN_UP, G1);
		}

		// DOWN
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_DOWN]) & 0x80)
		{
			AdditiveAnalogue(ANALOGUE_DOWN_UP, -G1);
		}

		// LEFT
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_LEFT]) & 0x80)
		{
			AdditiveAnalogue(ANALOGUE_LEFT_RIGHT, -G1);
		}

		// RIGHT
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_RIGHT]) & 0x80)
		{
			AdditiveAnalogue(ANALOGUE_LEFT_RIGHT, G1);
		}

		// FIRE
		if (	((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_SPACE]) & 0x80)
			||	((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_RETURN]) & 0x80)
			)
		{
			SetButton(BUTTON_FIRE_PRIMARY);	// Some of the front-end only responds to this button
			SetButton(BUTTON_PSX_X);
		}

		// BACKUP
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_ESCAPE]) & 0x80)
		{
			SetButton(BUTTON_PSX_TRIANGLE);
		}
	
		mControllerConfig.ProcessMappedKeys(FrontEndProcKeyList, (void*)this);

		// ************************************************************
		// ************************************************************
		// Cheats  (JCL)
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_F1]) & 0x80)	SetButton(BUTTON_CHEAT_START);
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_F5]) & 0x80)	SetButton(BUTTON_CHEAT_1);
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_F6]) & 0x80)	SetButton(BUTTON_CHEAT_2);
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_F7]) & 0x80)	SetButton(BUTTON_CHEAT_3);
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_F8]) & 0x80)	SetButton(BUTTON_CHEAT_4);
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_F9]) & 0x80)	SetButton(BUTTON_CHEAT_5);
	
		#define	IDB(button) ((IsButtonSet(button)) && (!(IsButtonOldSet(button))))
			
		mCheatAvailable = -1; // only available for a frame...
		
		if (IsButtonSet(BUTTON_CHEAT_START))
		{
			if (!(mCheatStarted))
			{
				mCheatStarted = TRUE;
				mCheatPos = 0;
			}
			
			if (IDB(BUTTON_CHEAT_2))
			{
				if (mCheatPos < 19)
					mCheatKeys[mCheatPos ++] = 1;
			}
		
			if (IDB(BUTTON_CHEAT_1))
			{
				if (mCheatPos < 19)
					mCheatKeys[mCheatPos ++] = 2;
			}
		
			if (IDB(BUTTON_CHEAT_3))
			{
				if (mCheatPos < 19)
					mCheatKeys[mCheatPos ++] = 3;
			}
		
			if (IDB(BUTTON_CHEAT_4))
			{
				if (mCheatPos < 19)
					mCheatKeys[mCheatPos ++] = 4;
			}

			if (IDB(BUTTON_CHEAT_5))
			{
				if (mCheatPos < 19)
					mCheatKeys[mCheatPos ++] = 5;
			}
			
			SINT c0;
			
			for (c0 = 0; c0 < NUM_CHEATS; c0 ++)
			{
				SINT n = CHEATS[c0].mNumKeys;
				
				if (n == mCheatPos)
				{
					SINT	c1;
					SINT	ok = TRUE;
					
					for (c1 = 0; c1 < n; c1 ++)
					{
						if (mCheatKeys[c1] != CHEATS[c0].mKeys[c1])
							ok = FALSE;
					}
					if (ok)
					{
						if ((c0 != 1) && (c0 != 5))		// no slippery or bouncy.
						{
							mCheatAvailable = c0;
							mCheatStarted = FALSE;
						}
					}
				}
			}
		}
		else
		{
			mCheatStarted = FALSE;
		}
	}
	else
	{
		/*
		char buffer[2600];
		*buffer = 0;
		for (int i = 256; i--;)
		{
			if (PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[i] & 0x80)
				sprintf(buffer, "%s K%d[%d]", buffer, i, PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[i]);
		}
		if (*buffer)
		{
			strcat(buffer, "\n");
			OutputDebugString(buffer);
		}
		*/

		// Call method to process all mapped keys.
		mControllerConfig.ProcessMappedKeys(ProcKeyList, (void*)this);

		// GAME PANEL
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_ESCAPE]) & 0x80)
		{
			SetButton(BUTTON_GAME_PANEL);
		}

#ifdef _DEBUG
		// DEBUG
		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_BACK]) & 0x80)
		{
			SetButton(BUTTON_PAUSE);
		}

		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_HOME]) & 0x80)
			SetButton(BUTTON_ZOOM_OUT);

		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_END]) & 0x80)
			SetButton(BUTTON_ZOOM_IN);

		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_NEXT]) & 0x80)
			SetButton(BUTTON_SPIN_CLOCKWISE);

		if ((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_DELETE]) & 0x80)
			SetButton(BUTTON_SPIN_ANTICLOCKWISE);
#endif
	}	

	// Clamp values to bounds.
	if (mAnalogue[ANALOGUE_LEFT_RIGHT] >  G1)  mAnalogue[ANALOGUE_LEFT_RIGHT] =  G1;
	if (mAnalogue[ANALOGUE_LEFT_RIGHT] < -G1)  mAnalogue[ANALOGUE_LEFT_RIGHT] = -G1;
	if (mAnalogue[ANALOGUE_DOWN_UP   ] >  G1)  mAnalogue[ANALOGUE_DOWN_UP   ] =  G1;
	if (mAnalogue[ANALOGUE_DOWN_UP   ] < -G1)  mAnalogue[ANALOGUE_DOWN_UP   ] = -G1;


	GINT VectSqd =	(	(mAnalogue[ANALOGUE_DOWN_UP] * mAnalogue[ANALOGUE_DOWN_UP])
					+ 	(mAnalogue[ANALOGUE_LEFT_RIGHT] * mAnalogue[ANALOGUE_LEFT_RIGHT])
					);

	if ((GABS(mAnalogue[ANALOGUE_DOWN_UP]) + GABS(mAnalogue[ANALOGUE_LEFT_RIGHT])) > G1)
	{
		float factor = GToF(VectSqd);
		factor = sqrt(factor);
		GINT ffff = FToG(factor);

		mAnalogue[ANALOGUE_DOWN_UP]		/= ffff;
		mAnalogue[ANALOGUE_LEFT_RIGHT]	/= ffff;
	}

	if (GABS(mAnalogue[ANALOGUE_LEFT_RIGHT]) < G(0, 16384)) mAnalogue[ANALOGUE_LEFT_RIGHT] = G0;
	if (GABS(mAnalogue[ANALOGUE_DOWN_UP   ]) < G(0, 16384)) mAnalogue[ANALOGUE_DOWN_UP   ] = G0;
	
	UpdateFromPlatform();
	return;

#if 0
	// Recording/ playback checks
	if ((mPlayingBack == NOT_ACTIVE) && (mRecording == NOT_ACTIVE))
	{
		if (PLATFORM.IsKeyDown('Q'))
		{
			mRecording = WAITING_FOR_NUMBER;
			CONSOLE.AddString("Enter number to record, 'W' to cancel");
		}
		else if (PLATFORM.IsKeyDown('Z'))
		{
			mPlayingBack = WAITING_FOR_NUMBER;
			CONSOLE.AddString("Enter number to play back, 'X' to cancel");
		}
	}

	if (mRecording == WAITING_FOR_NUMBER)
	{
		SINT n = -1;
		if (PLATFORM.IsKeyDown('1')) n = 1;
		if (PLATFORM.IsKeyDown('2')) n = 2;
		if (PLATFORM.IsKeyDown('3')) n = 3;
		if (PLATFORM.IsKeyDown('4')) n = 4;
		if (PLATFORM.IsKeyDown('5')) n = 5;
		if (PLATFORM.IsKeyDown('6')) n = 6;
		if (PLATFORM.IsKeyDown('7')) n = 7;
		if (PLATFORM.IsKeyDown('8')) n = 8;

		if (n != -1)
			StartRecording(n);

		if (PLATFORM.IsKeyDown('W'))
		{
			mRecording = NOT_ACTIVE;
			CONSOLE.AddString("Cancelled...");
		}
	}

	if (mPlayingBack == WAITING_FOR_NUMBER)
	{
		SINT n = -1;
		if (PLATFORM.IsKeyDown('1')) n = 1;
		if (PLATFORM.IsKeyDown('2')) n = 2;
		if (PLATFORM.IsKeyDown('3')) n = 3;
		if (PLATFORM.IsKeyDown('4')) n = 4;
		if (PLATFORM.IsKeyDown('5')) n = 5;
		if (PLATFORM.IsKeyDown('6')) n = 6;
		if (PLATFORM.IsKeyDown('7')) n = 7;
		if (PLATFORM.IsKeyDown('8')) n = 8;

		if (n != -1)
			StartPlayBack(n);

		if (PLATFORM.IsKeyDown('X'))
		{
			mPlayingBack = NOT_ACTIVE;
			CONSOLE.AddString("Cancelled...");
		}
	}

	if (mPlayingBack == ACTIVE)
	{
		PlayBackState();
	}
	else
	{
		UpdateFromPlatform();
	
		if (mRecording == ACTIVE)
			RecordState();
	}

	if ((PLATFORM.IsKeyDown('W')) && (mRecording == ACTIVE))
		StopRecording();

	if ((PLATFORM.IsKeyDown('X')) && (mPlayingBack == ACTIVE))
		StopPlayBack();
#endif
}


#endif