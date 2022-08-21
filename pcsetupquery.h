// SetupQuery.cpp : Defines the entry point for the application.
//

#include "Common.h"

#if TARGET == PC
#ifndef _PCSETUPQUERY_H_
#define _PCSETUPQUERY_H_

#include "pcquery.h"
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "OkayToMap.h"
#include "PCSaveRoutines.h"


struct	tagConfigKeyMapping
{
	SINT			Button;
	bool			Matched;
};

struct tagControllerData
{
	bool			Valid;
	CInputDevice*	Device;
	char			Name[MAX_PATH];
	GUID			Guid;
	struct	tagConfigKeyMapping ConfigKeyMapping[10];
};

extern CQueryPlatform*		mySystem; 
extern tagControllerData*	WorkingController[2];
extern GUID					SelectedDisplayDevice;
extern GUID					SelectedSoundDevice;
extern int					SelectedWidth, SelectedHeight, SelectedDepth;
extern GUID					PlayerOneJoystickGUID;
extern GUID					PlayerTwoJoystickGUID;
extern bool					PlayerOneAssignedJoystick;
extern bool					PlayerTwoAssignedJoystick;
extern SINT					PCSetupQueryLanguage;
extern bool					OkayToUseStdNavKeys;

HWND getStartOptionByIndex(int _Index, HWND _Window);
HWND getMapJoystickComboByIndex(int _Index, HWND _Window);
HWND getMapKeyboardComboByIndex(int _Index, HWND _Window);

SINT assignKeyboardDefaultForAction(SINT _Action);
SINT assignJoystickDefaultForAction(SINT _Action);

SINT getMapJoystickIndexFromCombo(SINT _Window);
SINT getMapKeyboardIndexFromCombo(SINT _Window);




// START
int					RunGameDialog(HINSTANCE hInstance);
LRESULT CALLBACK	StartDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// INSTALL
// Nothing yet

// PLAY
// Not required

// DEVICE
int					RunDeviceDialog(HINSTANCE hInstance);
LRESULT CALLBACK	DeviceDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// CONTROLLER
int					RunControllerDialog(HINSTANCE hInstance);
LRESULT CALLBACK	ControllerDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// JOYSTICK
int					RunJoystickDialog(HINSTANCE hInstance);
LRESULT CALLBACK	JoystickDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// KEYBOARD
int					RunKeyboardDialog(HINSTANCE hInstance);
LRESULT CALLBACK	KeyboardDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// UNINSTALL
// Nothing yet.

// QUIT
// Not required

// 
void				LoadSettings();
bool				CheckCD();
SINT				MenuIndexToInGameFunction(SINT _Index);
HWND				GetMappingComboFromIndex(int _Index, HWND _Window);									// OLD - I think
char*				generateOptionFromDeviceButton(CInputDevice* _Device, SINT _Button, char* _Buffer);	// OLD
bool				getDeviceAndButtonFromOption(char* _Option, CInputDevice* _Device, SINT* _Button);	// OLD
bool				CorrectlyInstalled();

#endif
#endif