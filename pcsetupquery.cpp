#include "Common.h"

#if TARGET == PC

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <dmusici.h>

#include <process.h>
#include <direct.h>

#include "PCSetupQuery.h"
#include "pcquery.h"
#include <stdio.h>
#include "resource.h"
#include "OkayToMap.h"
#include "PCSaveRoutines.h"
#include "game.h"
#include "controllercommon.h"
#include "text.h"
#include "Translate.h"

tagControllerData	LoadedController[2];	// Global
tagControllerData*	WorkingController[2];	// Extern

SINT				WorkingMapping[10];	// Temp list of mapped keys.(Could be made static in each proc)

struct tagWorkingControllerConfig			// Global
{
	tagControllerData Player[2];
}WorkingControllerConfig[16];

// Global data.
HINSTANCE		HInstance;
GUID			LoadedVideoGUID;
GUID			LoadedSoundGUID;
bool			Install;
bool			HaveDirectX7;
bool			CompleteInstall;
SINT			ProcessingPlayer;
char			LangArray[84][1000];

// Extern'd
CQueryPlatform*	mySystem; 
char			MoHoCDPath[MAX_PATH];
char			WhereIsSetup[MAX_PATH];
GUID			SelectedDisplayDevice;
int				SelectedWidth, SelectedHeight, SelectedDepth;
GUID			SelectedSoundDevice;
SINT			PCSetupQueryLanguage;
bool			OkayToUseStdNavKeys;

#define NUMBER_OF_ACTIONS	(20)
struct	tagConfigKeyMapping ConfigKeyMapping[NUMBER_OF_ACTIONS]; // This must go!




typedef HRESULT(WINAPI * DIRECTDRAWCREATE)( GUID*, LPDIRECTDRAW*, IUnknown* );
typedef HRESULT(WINAPI * DIRECTDRAWCREATEEX)( GUID*, VOID**, REFIID, IUnknown* );
typedef HRESULT(WINAPI * DIRECTINPUTCREATE)( HINSTANCE, DWORD, LPDIRECTINPUT*,
                                             IUnknown* );



//-----------------------------------------------------------------------------
// Name: GetDXVersion()
// Desc: This function returns two arguments:
//          dwDXVersion:
//            0x0000 = No DirectX installed
//            0x0100 = DirectX version 1 installed
//            0x0200 = DirectX 2 installed
//            0x0300 = DirectX 3 installed
//            0x0500 = At least DirectX 5 installed.
//            0x0600 = At least DirectX 6 installed.
//            0x0601 = At least DirectX 6.1 installed.
//            0x0700 = At least DirectX 7 installed.
//          dwDXPlatform:
//            0                          = Unknown (This is a failure case)
//            VER_PLATFORM_WIN32_WINDOWS = Windows 9X platform
//            VER_PLATFORM_WIN32_NT      = Windows NT platform
// 
//          Please note that this code is intended as a general guideline. Your
//          app will probably be able to simply query for functionality (via
//          QueryInterface) for one or two components.
//
//          Please also note:
//            "if (dxVer != 0x500) return FALSE;" is BAD. 
//            "if (dxVer < 0x500) return FALSE;" is MUCH BETTER.
//          to ensure your app will run on future releases of DirectX.
//-----------------------------------------------------------------------------
VOID GetDXVersion( DWORD* pdwDXVersion, DWORD* pdwDXPlatform )
{
    HRESULT              hr;
    HINSTANCE            DDHinst = 0;
    HINSTANCE            DIHinst = 0;
    LPDIRECTDRAW         pDDraw  = 0;
    LPDIRECTDRAW2        pDDraw2 = 0;
    DIRECTDRAWCREATE     DirectDrawCreate   = 0;
    DIRECTDRAWCREATEEX   DirectDrawCreateEx = 0;
    DIRECTINPUTCREATE    DirectInputCreate  = 0;
    OSVERSIONINFO        osVer;
    LPDIRECTDRAWSURFACE  pSurf  = 0;
    LPDIRECTDRAWSURFACE3 pSurf3 = 0;
    LPDIRECTDRAWSURFACE4 pSurf4 = 0;

    // First get the windows platform
    osVer.dwOSVersionInfoSize = sizeof(osVer);
    if( !GetVersionEx( &osVer ) )
    {
        (*pdwDXPlatform) = 0;
        (*pdwDXVersion)  = 0;
        return;
    }

    if( osVer.dwPlatformId == VER_PLATFORM_WIN32_NT )
    {
        (*pdwDXPlatform) = VER_PLATFORM_WIN32_NT;

        // NT is easy... NT 4.0 is DX2, 4.0 SP3 is DX3, 5.0 is DX5
        // and no DX on earlier versions.
        if( osVer.dwMajorVersion < 4 )
        {
            (*pdwDXVersion) = 0; // No DX on NT3.51 or earlier
            return;
        }

        if( osVer.dwMajorVersion == 4 )
        {
            // NT4 up to SP2 is DX2, and SP3 onwards is DX3, so we are at least DX2
            (*pdwDXVersion) = 0x200;

            // We're not supposed to be able to tell which SP we're on, so check for dinput
            DIHinst = LoadLibrary( "DINPUT.DLL" );
            if( DIHinst == 0 )
            {
                // No DInput... must be DX2 on NT 4 pre-SP3
                OutputDebugString( "Couldn't LoadLibrary DInput\r\n" );
                return;
            }

            DirectInputCreate = (DIRECTINPUTCREATE)GetProcAddress( DIHinst,
                                                                 "DirectInputCreateA" );
            FreeLibrary( DIHinst );

            if( DirectInputCreate == 0 )
            {
                // No DInput... must be pre-SP3 DX2
                OutputDebugString( "Couldn't GetProcAddress DInputCreate\r\n" );
                return;
            }

            // It must be NT4, DX2
            (*pdwDXVersion) = 0x300;  // DX3 on NT4 SP3 or higher
            return;
        }
        // Else it's NT5 or higher, and it's DX5a or higher: Drop through to
        // Win9x tests for a test of DDraw (DX6 or higher)
    }
    else
    {
        // Not NT... must be Win9x
        (*pdwDXPlatform) = VER_PLATFORM_WIN32_WINDOWS;
    }

    // Now we know we are in Windows 9x (or maybe 3.1), so anything's possible.
    // First see if DDRAW.DLL even exists.
    DDHinst = LoadLibrary( "DDRAW.DLL" );
    if( DDHinst == 0 )
    {
        (*pdwDXVersion)  = 0;
        (*pdwDXPlatform) = 0;
        FreeLibrary( DDHinst );
        return;
    }

    // See if we can create the DirectDraw object.
    DirectDrawCreate = (DIRECTDRAWCREATE)GetProcAddress( DDHinst, "DirectDrawCreate" );
    if( DirectDrawCreate == 0 )
    {
        (*pdwDXVersion)  = 0;
        (*pdwDXPlatform) = 0;
        FreeLibrary( DDHinst );
        OutputDebugString( "Couldn't LoadLibrary DDraw\r\n" );
        return;
    }

    hr = DirectDrawCreate( NULL, &pDDraw, NULL );
    if( FAILED(hr) )
    {
        (*pdwDXVersion)  = 0;
        (*pdwDXPlatform) = 0;
        FreeLibrary( DDHinst );
        OutputDebugString( "Couldn't create DDraw\r\n" );
        return;
    }

    // So DirectDraw exists.  We are at least DX1.
    (*pdwDXVersion) = 0x100;

    // Let's see if IID_IDirectDraw2 exists.
    hr = pDDraw->QueryInterface( IID_IDirectDraw2, (VOID**)&pDDraw2 );
    if( FAILED(hr) )
    {
        // No IDirectDraw2 exists... must be DX1
        pDDraw->Release();
        FreeLibrary( DDHinst );
        OutputDebugString( "Couldn't QI DDraw2\r\n" );
        return;
    }

    // IDirectDraw2 exists. We must be at least DX2
    pDDraw2->Release();
    (*pdwDXVersion) = 0x200;


    ///////////////////////////////////////////////////////////////////////////
    // DirectX 3.0 Checks
    ///////////////////////////////////////////////////////////////////////////

    // DirectInput was added for DX3
    DIHinst = LoadLibrary( "DINPUT.DLL" );
    if( DIHinst == 0 )
    {
        // No DInput... must not be DX3
        OutputDebugString( "Couldn't LoadLibrary DInput\r\n" );
        pDDraw->Release();
        FreeLibrary( DDHinst );
        return;
    }

    DirectInputCreate = (DIRECTINPUTCREATE)GetProcAddress( DIHinst,
                                                        "DirectInputCreateA" );
    if( DirectInputCreate == 0 )
    {
        // No DInput... must be DX2
        FreeLibrary( DIHinst );
        FreeLibrary( DDHinst );
        pDDraw->Release();
        OutputDebugString( "Couldn't GetProcAddress DInputCreate\r\n" );
        return;
    }

    // DirectInputCreate exists. We are at least DX3
    (*pdwDXVersion) = 0x300;
    FreeLibrary( DIHinst );

    // Can do checks for 3a vs 3b here


    ///////////////////////////////////////////////////////////////////////////
    // DirectX 5.0 Checks
    ///////////////////////////////////////////////////////////////////////////

    // We can tell if DX5 is present by checking for the existence of
    // IDirectDrawSurface3. First, we need a surface to QI off of.
    DDSURFACEDESC ddsd;
    ZeroMemory( &ddsd, sizeof(ddsd) );
    ddsd.dwSize         = sizeof(ddsd);
    ddsd.dwFlags        = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    hr = pDDraw->SetCooperativeLevel( NULL, DDSCL_NORMAL );
    if( FAILED(hr) )
    {
        // Failure. This means DDraw isn't properly installed.
        pDDraw->Release();
        FreeLibrary( DDHinst );
        (*pdwDXVersion) = 0;
        OutputDebugString( "Couldn't Set coop level\r\n" );
        return;
    }

    hr = pDDraw->CreateSurface( &ddsd, &pSurf, NULL );
    if( FAILED(hr) )
    {
        // Failure. This means DDraw isn't properly installed.
        pDDraw->Release();
        FreeLibrary( DDHinst );
        *pdwDXVersion = 0;
        OutputDebugString( "Couldn't CreateSurface\r\n" );
        return;
    }

    // Query for the IDirectDrawSurface3 interface
    if( FAILED( pSurf->QueryInterface( IID_IDirectDrawSurface3,
                                       (VOID**)&pSurf3 ) ) )
    {
        pDDraw->Release();
        FreeLibrary( DDHinst );
        return;
    }

    // QI for IDirectDrawSurface3 succeeded. We must be at least DX5
    (*pdwDXVersion) = 0x500;


    ///////////////////////////////////////////////////////////////////////////
    // DirectX 6.0 Checks
    ///////////////////////////////////////////////////////////////////////////

    // The IDirectDrawSurface4 interface was introduced with DX 6.0
    if( FAILED( pSurf->QueryInterface( IID_IDirectDrawSurface4,
                                       (VOID**)&pSurf4 ) ) )
    {
        pDDraw->Release();
        FreeLibrary( DDHinst );
        return;
    }

    // IDirectDrawSurface4 was create successfully. We must be at least DX6
    (*pdwDXVersion) = 0x600;
    pSurf->Release();
    pDDraw->Release();


    ///////////////////////////////////////////////////////////////////////////
    // DirectX 6.1 Checks
    ///////////////////////////////////////////////////////////////////////////

    // Check for DMusic, which was introduced with DX6.1
    LPDIRECTMUSIC pDMusic = NULL;
    CoInitialize( NULL );
    hr = CoCreateInstance( CLSID_DirectMusic, NULL, CLSCTX_INPROC_SERVER,
                           IID_IDirectMusic, (VOID**)&pDMusic );
    if( FAILED(hr) )
    {
        OutputDebugString( "Couldn't create CLSID_DirectMusic\r\n" );
        FreeLibrary( DDHinst );
        return;
    }

    // DirectMusic was created successfully. We must be at least DX6.1
    (*pdwDXVersion) = 0x601;
    pDMusic->Release();
    CoUninitialize();
    

    ///////////////////////////////////////////////////////////////////////////
    // DirectX 7.0 Checks
    ///////////////////////////////////////////////////////////////////////////

    // Check for DirectX 7 by creating a DDraw7 object
    LPDIRECTDRAW7 pDD7;
    DirectDrawCreateEx = (DIRECTDRAWCREATEEX)GetProcAddress( DDHinst,
                                                       "DirectDrawCreateEx" );
    if( NULL == DirectDrawCreateEx )
    {
        FreeLibrary( DDHinst );
        return;
    }

    if( FAILED( DirectDrawCreateEx( NULL, (VOID**)&pDD7, IID_IDirectDraw7,
                                    NULL ) ) )
    {
        FreeLibrary( DDHinst );
        return;
    }

    // DDraw7 was created successfully. We must be at least DX7.0
    (*pdwDXVersion) = 0x700;
    pDD7->Release();

    
    ///////////////////////////////////////////////////////////////////////////
    // End of checks
    ///////////////////////////////////////////////////////////////////////////

    // Close open libraries and return
    FreeLibrary( DDHinst );
    
    return;
}

// ----------------------------------------------------------------------------
// Entry point for dialogs.
int RunGameDialog(HINSTANCE hInstance)
{
	DWORD	DataType;
	char*	Value		= NULL;
	DWORD	DataLength	= 0;
		
	// Store hInstance value.
	HInstance = hInstance;
	
	// Assume that the game is not installed.
	Install			= true;
	CompleteInstall = false;
	
	// Load langauge settings, or assume default if unable to load.
	PCSetupQueryLanguage = DEFAULT_LANGUAGE;
	if (	!(PCLoadFromRegistry(	HKEY_CURRENT_USER,	// Existing key or default root.
									MOHO_REGISTRY_PATH,	// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
									"Language",			// Name of value to retrieve.
									&DataType,			// A pointer to a DWORD filled-in by the function.
									&Value,				// A pointer to buffer created within the function - you are responsible for the freeing of this resource.
									&DataLength			// A pointer to a DWORD filled-in by the function that indicates the length of the data (in bytes) returned for _Value.
								)
			)
		)
	{
		// Check that the data is valid
		if (	(Value)
			&&	(DataLength)
			)
		{
			// And of the correct type.
			if (DataType == REG_DWORD) 
			{
				PCSetupQueryLanguage = *((DWORD*)Value);
				Install = false;
			}
			
			delete[] Value;
			Value = NULL;
		}
	}
	else
	{
		switch (GetUserDefaultLangID())
		{
		case 0x0407:	// German (Standard) 
		case 0x0807:	// German (Swiss) 
		case 0x0c07:	// German (Austrian) 
		case 0x1007:	// German (Luxembourg) 
		case 0x1407:	// German (Liechtenstein) 
			PCSetupQueryLanguage = LANG_GERMAN;
			break;
		
		case 0x0409:	// English (United States) 
		case 0x0809:	// English (United Kingdom) 
		case 0x0c09:	// English (Australian) 
		case 0x1009:	// English (Canadian) 
		case 0x1409:	// English (New Zealand) 
		case 0x1809:	// English (Ireland) 
		case 0x1c09:	// English (South Africa) 
		case 0x2009:	// English (Jamaica) 
		case 0x2409:	// English (Caribbean) 
		case 0x2809:	// English (Belize) 
		case 0x2c09:	// English (Trinidad) 
			PCSetupQueryLanguage = LANG_ENGLISH;
			break;
		
		case 0x040a:	// Spanish (Traditional Sort) 
		case 0x080a:	// Spanish (Mexican) 
		case 0x0c0a:	// Spanish (Modern Sort) 
		case 0x100a:	// Spanish (Guatemala) 
		case 0x140a:	// Spanish (Costa Rica) 
		case 0x180a:	// Spanish (Panama) 
		case 0x1c0a:	// Spanish (Dominican Republic) 
		case 0x200a:	// Spanish (Venezuela) 
		case 0x240a:	// Spanish (Colombia) 
		case 0x280a:	// Spanish (Peru) 
		case 0x2c0a:	// Spanish (Argentina) 
		case 0x300a:	// Spanish (Ecuador) 
		case 0x340a:	// Spanish (Chile) 
		case 0x380a:	// Spanish (Uruguay) 
		case 0x3c0a:	// Spanish (Paraguay) 
		case 0x400a:	// Spanish (Bolivia) 
		case 0x440a:	// Spanish (El Salvador) 
		case 0x480a:	// Spanish (Honduras) 
		case 0x4c0a:	// Spanish (Nicaragua) 
		case 0x500a:	// Spanish (Puerto Rico) 
			PCSetupQueryLanguage = LANG_SPANISH;
			break;
		
		case 0x040c:	// French (Standard) 
		case 0x080c:	// French (Belgian) 
		case 0x0c0c:	// French (Canadian) 
		case 0x100c:	// French (Swiss) 
		case 0x140c:	// French (Luxembourg) 
			PCSetupQueryLanguage = LANG_FRENCH;
			break;
		
		case 0x0410:	// Italian (Standard) 
		case 0x0810:	// Italian (Swiss) 
			PCSetupQueryLanguage = LANG_ITALIAN;
			break;
			
		default:
			PCSetupQueryLanguage = DEFAULT_LANGUAGE;
			break;
		}
	}
	
	if (	(PCSetupQueryLanguage < 0) 
		||	(PCSetupQueryLanguage > MAX_LANGUAGE_INDEX)
		)
	{
		PCSetupQueryLanguage = DEFAULT_LANGUAGE;
	}

	switch (PCSetupQueryLanguage)
	{
	default:
	case LANG_ENGLISH:
		memcpy(&LangArray, &EnglishSetupLangArray, sizeof(EnglishSetupLangArray));
		break;
	case LANG_FRENCH:
		memcpy(&LangArray, &FrenchSetupLangArray, sizeof(FrenchSetupLangArray));
		break;
	case LANG_GERMAN:
		memcpy(&LangArray, &GermanSetupLangArray, sizeof(GermanSetupLangArray));
		break;
	case LANG_ITALIAN:
		memcpy(&LangArray, &ItalianSetupLangArray, sizeof(ItalianSetupLangArray));
		break;
	case LANG_SPANISH:
		memcpy(&LangArray, &SpanishSetupLangArray, sizeof(SpanishSetupLangArray));
		break;
	}

	// If can't read data path, force a re install.
	if (!PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "DataPath",	REG_SZ))
	{
		Install = true;
	}

	CompleteInstall = !CorrectlyInstalled();

	// Check whether the wrong version of DX was installed when the program was install.
	if (	!(PCLoadFromRegistry(	HKEY_CURRENT_USER,	// Existing key or default root.
									MOHO_REGISTRY_PATH,	// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
									"WrongDX",			// Name of value to retrieve.
									&DataType,			// A pointer to a DWORD filled-in by the function.
									&Value,				// A pointer to buffer created within the function - you are responsible for the freeing of this resource.
									&DataLength			// A pointer to a DWORD filled-in by the function that indicates the length of the data (in bytes) returned for _Value.
								)
			)
		)
	{
		// Check that the data is valid
		if (	(Value)
			&&	(DataLength)
			)
		{
			// And of the correct type.
			if (DataType == REG_DWORD) 
			{
				CompleteInstall = true;
			}
			
			delete[] Value;
			Value = NULL;
		}
	}

	DWORD DXVer, OSVer;
	GetDXVersion(&DXVer, &OSVer);
	HaveDirectX7 = (DXVer >= 0x700);

	if (HaveDirectX7) 
	{		
		// Query hardware
		mySystem = new CQueryPlatform();	// Collects and stores system data.
		mySystem->Query(hInstance);
	}
	else
	{
		// Set a reg item to indicate that the game has not completed install
		DWORD hh;
		hh = 0;
		PCSaveToRegistry(	HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH, 
							"WrongDX", 
							REG_DWORD, 
							(char*)&hh, 
							sizeof(DWORD));
	}

	// Start dialog
	OutputDebugString("Launching start game box.\n");
	HRESULT hr = DialogBox(hInstance, MAKEINTRESOURCE(IDD_START), GetDesktopWindow(), (DLGPROC)StartDlgProc);
	
	if (hr == TRUE)
	{
		return true;
	}
	else
	{
		OutputDebugString("Start Dialog: Cancelled or error.\n");
		return false;
	}
}

// ----------------------------------------------------------------------------
// Proc function for start dialog.
LRESULT CALLBACK StartDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int		i, j, k;
	char	buffer[256];
	
	// What are we processing?
	switch (uMsg)
    {
	case WM_INITDIALOG:

		if (	(Install) 
			||	(!CompleteInstall)
			||	(!HaveDirectX7)
			)
			SetWindowText(GetDlgItem(hDlg, IDC_START_INSTALL), LangArray[0]);	// Install
		else
			SetWindowText(GetDlgItem(hDlg, IDC_START_INSTALL), LangArray[1]);	// Complete install
		SetWindowText(GetDlgItem(hDlg, IDC_START_PLAY), LangArray[2]);
		SetWindowText(GetDlgItem(hDlg, IDC_START_DEVICE_OPTIONS), LangArray[3]);
		SetWindowText(GetDlgItem(hDlg, IDC_START_CONTROLLER_OPTIONS), LangArray[4]);
		SetWindowText(GetDlgItem(hDlg, IDC_START_UNINSTALL), LangArray[5]);
		SetWindowText(GetDlgItem(hDlg, IDC_START_QUIT), LangArray[6]);
		
		if (HaveDirectX7)
		{
			// Create all direct input devices
			mySystem->CreateAllDirectInput(hDlg);
		}
		
		if ((Install) || (CompleteInstall))
		{
			// Disable all buttons
			for (i = 4; i >= 1; i--)
			{
				EnableWindow(getStartOptionByIndex(i, hDlg), false);
			}
		}
		else
		{
			EnableWindow(getStartOptionByIndex(0, hDlg), false);
		}
		
		if (HaveDirectX7)
		{
			if (mySystem)
			{
				memset((void*)WorkingControllerConfig, 0, sizeof(tagWorkingControllerConfig) * mySystem->GetNumberOfInputDevices());
				LoadSettings();
				
				// For both players.
				for (i = 2; i--;)
				{
					WorkingController[i] = NULL;
					
					for (j = mySystem->GetNumberOfInputDevices(); j--;)
					{
						// All actions.
						for (k = 10; k--;)
						{
							WorkingControllerConfig[j].Player[i].ConfigKeyMapping[k].Button = -1;	// Unmapped
							
							switch (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(j)->Info.dwDevType))
							{
							case DIDEVTYPE_KEYBOARD:
								WorkingControllerConfig[j].Player[i].ConfigKeyMapping[k].Button = assignKeyboardDefaultForAction(k);
								break;
							case DIDEVTYPE_JOYSTICK:
								WorkingControllerConfig[j].Player[i].ConfigKeyMapping[k].Button = assignJoystickDefaultForAction(k);
								break;
							default:
								break;
							}
							//sprintf(buffer, "Player %d, device %d is a %s, and action %d is defined to be %d.\n", i, j, mySystem->GetInputDevice(j)->Info.tszInstanceName, k, WorkingControllerConfig[j].Player[i].ConfigKeyMapping[k].Button);
							//OutputDebugString(buffer);
						}
						
						strcpy(WorkingControllerConfig[j].Player[i].Name, mySystem->GetInputDevice(j)->Info.tszInstanceName);
						WorkingControllerConfig[j].Player[i].Guid = mySystem->GetInputDevice(j)->Info.guidInstance;
						
						// If the name of the loaded device matches the name of one of the iterated devices then copy its data over into that section position.
						if (!strcmp(LoadedController[i].Name, mySystem->GetInputDevice(j)->Info.tszInstanceName))
						{
							memcpy((void*)&WorkingControllerConfig[j].Player[i], (void*)&LoadedController[i], sizeof(LoadedController[i]));
							WorkingController[i] = &WorkingControllerConfig[j].Player[i];
						}
						
						// Update reference to device.
						WorkingControllerConfig[j].Player[i].Device = mySystem->GetInputDevice(j);
					}
				}
			}
			
			for (i = 2; i--;)
			{
				if (!WorkingController[i])
				{
					// Assign keyboard as default device
					for (j = mySystem->GetNumberOfInputDevices(); j--;)
					{
						if (i)
						{
							//		// Player one gets keyboard. This will always happen!!!
							//		if (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(j)->Info.dwDevType) == DIDEVTYPE_JOYSTICK)
							//		{
							//			WorkingController[i] = &WorkingControllerConfig[0].Player[i];
							//			//WorkingController[i]->Valid = true;
							//		}
						}
						else
						{
							// Player one gets keyboard. This will always happen!!!
							if (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(j)->Info.dwDevType) == DIDEVTYPE_KEYBOARD)
							{
								WorkingController[i] = &WorkingControllerConfig[j].Player[i];
								WorkingController[i]->Valid = true;
								break;
							}
							else
							{
								if (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(j)->Info.dwDevType) == DIDEVTYPE_JOYSTICK)
								{
									WorkingController[i] = &WorkingControllerConfig[j].Player[i];
									WorkingController[i]->Valid = true;
									//break;
								}
							}
						}
					}
				}
			}
		}
		
		return true;
		break;
		
	case WM_COMMAND:
		switch (HIWORD(wParam))	
		{
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case IDC_START_INSTALL:
				
				if (HaveDirectX7)
					mySystem->ReleaseDirectInputDevices();

				if ((Install) || (!HaveDirectX7))
				{
#ifndef IGNORE_INSTALL
					if (CheckCD())
					{
						ShowWindow(hDlg, false);
						strcat(WhereIsSetup, "setup.exe");
						_spawnl(_P_WAIT, WhereIsSetup, "setup.exe");

					}
					else
					{
						// Failed to find CD - so how are they running this?
						MessageBox(hDlg, LangArray[83], GAME_NAME, MB_OK);
						ShowWindow(hDlg, true);
						break;
					}
#else
//#if 0
					ShowWindow(hDlg, false);

					// Install code here!
					MessageBox(hDlg, "Install goes here", GAME_NAME, MB_OK);
					DWORD hh;
					hh = 0;
					PCSaveToRegistry(	HKEY_CURRENT_USER, 
										MOHO_REGISTRY_PATH, 
										"Language", 
										REG_DWORD, 
										(char*)&hh, 
										sizeof(DWORD));

					char datap[MAX_PATH];
					strcpy(datap, "C:\\MoHo PC\\");
					PCSaveToRegistry(	HKEY_CURRENT_USER, 
										MOHO_REGISTRY_PATH, 
										"DataPath", 
										REG_SZ, 
										(char*)datap, 
										strlen(datap) + 1);
//#endif
#endif
				}

				if (HaveDirectX7)
				{
					ShowWindow(hDlg, false);

					mySystem->CreateAllDirectInput(hDlg);
				
					RunDeviceDialog(HInstance);
					RunControllerDialog(HInstance);

					Install = false;
					
					for (i = 4; i >= 1; i--)
					{
						EnableWindow(getStartOptionByIndex(i, hDlg), true);
					}
				}

				if (CompleteInstall)
				{
					PCRemoveRegistryValue(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "WrongDX");

					SINT hhhh = 8;

					// Save out volume info
					PCSaveToRegistry(	HKEY_CURRENT_USER, 
										MOHO_REGISTRY_PATH, 
										"EffectsVolume", 
										REG_BINARY, 
										(char*)&hhhh, 
										sizeof(hhhh)
										);

					PCSaveToRegistry(	HKEY_CURRENT_USER, 
										MOHO_REGISTRY_PATH, 
										"MusicVolume", 
										REG_BINARY, 
										(char*)&hhhh, 
										sizeof(hhhh)
										);

					CompleteInstall = false;
				}

				EnableWindow(getStartOptionByIndex(0, hDlg), false);
				EnableWindow(getStartOptionByIndex(4, hDlg), true);
				ShowWindow(hDlg, true);
				
				break;
				
			case IDC_START_PLAY:
				
#ifndef IGNORE_INSTALL
				//CheckCD();
				if (CheckCD())
				{
#endif
					ShowWindow(hDlg, false);

					// All checks complete. Time to clear-up.
					if (mySystem)
					{
						mySystem->ReleaseDirectInputDevices();
					}
				
					EndDialog(hDlg, true);
					return true;
#ifndef IGNORE_INSTALL
				}
				else
				{
					// Failed to find CD - so how are they running this?
					MessageBox(hDlg, LangArray[83], GAME_NAME, MB_OK);
					break;
				}
#endif		
				
			case IDC_START_DEVICE_OPTIONS:
				ShowWindow(hDlg, SW_HIDE);		// Hide dialog.
				RunDeviceDialog(HInstance);		// Run device dialog
				ShowWindow(hDlg, SW_SHOW);		// Show dialog
				break;
				
			case IDC_START_CONTROLLER_OPTIONS:
				ShowWindow(hDlg, SW_HIDE);		// Hide dialog.
				RunControllerDialog(HInstance);	// Run controller dialog
				ShowWindow(hDlg, SW_SHOW);		// Show dialog
				break;
				
			case IDC_START_UNINSTALL:
				if (HaveDirectX7)
					mySystem->ReleaseDirectInputDevices();


				WinExec("control appwiz.cpl", SW_NORMAL);

				//MessageBox(hDlg, "Uninstall goes here", GAME_NAME, MB_OK);
				
	//			RegDeleteKey(	HKEY_CURRENT_USER,	// handle to open key
	//							MOHO_REGISTRY_PATH	// address of name of subkey to delete
	//						);
		
				EndDialog(hDlg, false);
				return false;
				
			case IDC_START_QUIT:
				mySystem->ReleaseDirectInputDevices();
				EndDialog(hDlg, false);
				return false;
			}
		}
	}
	
	return false;
}


// ----------------------------------------------------------------------------
int RunDeviceDialog(HINSTANCE hInstance)
{	
	if (mySystem)
	{
		OutputDebugString("Launching IDD_VIDEO_SOUND box.\n");
		HRESULT hr = DialogBox(hInstance, MAKEINTRESOURCE(IDD_VIDEO_SOUND), GetDesktopWindow(), (DLGPROC)DeviceDlgProc);
		if (hr == TRUE)
		{
			return true;
		}
		else
		{
			OutputDebugString("IDD_VIDEO_SOUND Dialog: Cancelled or error.\n");
			return false;
		}
	}
	
	return false;
}

// ----------------------------------------------------------------------------
LRESULT CALLBACK DeviceDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND						ComboBox;
	char						buffer[256];
	int							i;
	CVideoDevice*				VideoDeviceRef, *SelectThisVideoDevice;
	CSoundDevice*				SoundDeviceRef, *SelectThisSoundDevice;
	
	// What are we processing?
	switch (uMsg)
    {
	case WM_INITDIALOG:

		SetWindowText(GetDlgItem(hDlg, IDC_VIDEO_SOUND_VIDEO_OPTIONS), LangArray[7]);
		SetWindowText(GetDlgItem(hDlg, IDC_VIDEO_SOUND_VIDEO_DEVICE_CAPTION), LangArray[8]);
		SetWindowText(GetDlgItem(hDlg, IDC_VIDEO_SOUND_RESOLUTION_CAPTION), LangArray[9]);
		SetWindowText(GetDlgItem(hDlg, IDC_VIDEO_SOUND_SOUND_OPTIONS), LangArray[10]);
		SetWindowText(GetDlgItem(hDlg, IDC_VIDEO_SOUND_SOUND_DEVICE_CAPTION), LangArray[11]);

		SetWindowText(GetDlgItem(hDlg, IDC_VIDEO_SOUND_OK), LangArray[41]);
		SetWindowText(GetDlgItem(hDlg, IDC_VIDEO_SOUND_CANCEL), LangArray[42]);
		
		if ((Install) || (CompleteInstall))
		{
			EnableWindow(GetDlgItem(hDlg, IDC_VIDEO_SOUND_CANCEL), false);
		}
		
		if (mySystem)
		{
			// ----
			// VIDEO
			ComboBox = GetDlgItem(hDlg, IDC_VIDEO_SOUND_DISPLAY_DEVICE_LIST);
			*buffer = 0;
			SelectThisVideoDevice = NULL;
			for (i = mySystem->GetNumberOfVideoDevices(); i--;)
			{
				VideoDeviceRef = mySystem->GetVideoDevice(i);
				SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)VideoDeviceRef->Name);
				
				if (LoadedVideoGUID == VideoDeviceRef->Guid)
				{
					SelectThisVideoDevice = VideoDeviceRef;
				}
			}
			
			// Choose promoted mode, or last listed. (Most likely to be the primary device).
			if (SelectThisVideoDevice)
			{
				i = SendMessage(ComboBox, CB_FINDSTRINGEXACT, 0, (LPARAM)(LPCTSTR)SelectThisVideoDevice->Name);
			}
			SendMessage(ComboBox, CB_SETCURSEL, i, 0);
			
			
			// ----
			// RESOLUTION
			ComboBox = GetDlgItem(hDlg, IDC_VIDEO_SOUND_RESOLUTION_LIST);
			*buffer = 0;
			for (i = VideoDeviceRef->GetNumberOfResolutions(); i--;)
			{
				// Filter out un wanted resolutions here!
				if	(	(VideoDeviceRef->GetResolution(i)->ddpfPixelFormat.dwRGBBitCount >= 16)
					&&	(VideoDeviceRef->GetResolution(i)->dwWidth >= 640)
					&&	(VideoDeviceRef->GetResolution(i)->dwHeight >= 480)
					)
				{
					sprintf(buffer, 
						"%dx%dx%d", 
						VideoDeviceRef->GetResolution(i)->dwWidth,
						VideoDeviceRef->GetResolution(i)->dwHeight,
						VideoDeviceRef->GetResolution(i)->ddpfPixelFormat.dwRGBBitCount);
					
					SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buffer);
				}
			}

			sprintf(buffer, 
					"%dx%dx%d", 
					SelectedWidth,
					SelectedHeight,
					SelectedDepth);
			i = SendMessage(ComboBox, CB_FINDSTRINGEXACT, 0, (LPARAM)(LPCTSTR)buffer);
			SendMessage(ComboBox, CB_SETCURSEL, i, 0);
			
	
			// ----
			// SOUND
			ComboBox = GetDlgItem(hDlg, IDC_VIDEO_SOUND_SOUND_DEVICE_LIST);
			*buffer = 0;
			SelectThisSoundDevice = NULL;
			for (i = mySystem->GetNumberOfSoundDevices(); i--;)
			{
				SoundDeviceRef = mySystem->GetSoundDevice(i);
				SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)SoundDeviceRef->Name);
				if (LoadedSoundGUID == SoundDeviceRef->Guid)
				{
					SelectThisSoundDevice = SoundDeviceRef;
				}
			}
			
			// Choose promoted mode, or last listed. (Most likely to be the primary device).
			if (SelectThisSoundDevice)
			{
				i = SendMessage(ComboBox, CB_FINDSTRINGEXACT, 0, (LPARAM)(LPCTSTR)SelectThisSoundDevice->Name);
			}
			SendMessage(ComboBox, CB_SETCURSEL, i, 0);
		}
		return TRUE;
		
	case WM_COMMAND:
		
		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			if (mySystem)
			{
				switch ((int)LOWORD(wParam))
				{
				case IDC_VIDEO_SOUND_DISPLAY_DEVICE_LIST:
					// Get selected item index, and use to get 
					// back a reference to the selected string.
					i = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
					SendMessage((HWND)lParam, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer);
					
					// Locate name returned in list of display devices
					for (i = mySystem->GetNumberOfVideoDevices(); i--;)
					{
						VideoDeviceRef = mySystem->GetVideoDevice(i);
						if (!strcmp(buffer, VideoDeviceRef->Name))
						{
							// We have a match, and thus a reference to the selected device.
							// Remove old content, then add new resolution listings.
							ComboBox = GetDlgItem(hDlg, IDC_RESOLUTION_LIST);
							for (i = SendMessage(ComboBox, CB_GETCOUNT, 0, 0); i--;)
							{
								SendMessage(ComboBox, CB_DELETESTRING, i, 0);
							}
							
							*buffer = 0;
							for (i = VideoDeviceRef->GetNumberOfResolutions(); i--;)
							{
								// Filter out un wanted resolutions here!
								if (VideoDeviceRef->GetResolution(i)->ddpfPixelFormat.dwRGBBitCount > 8)
								{
									sprintf(buffer, 
										"%dx%dx%d", 
										VideoDeviceRef->GetResolution(i)->dwWidth,
										VideoDeviceRef->GetResolution(i)->dwHeight,
										VideoDeviceRef->GetResolution(i)->ddpfPixelFormat.dwRGBBitCount);
									
									SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buffer);
								}
							}
							i = SendMessage(ComboBox, CB_FINDSTRINGEXACT, 0, (LPARAM)(LPCTSTR)buffer);
							SendMessage(ComboBox, CB_SETCURSEL, i, 0);
							return true;
						}
					}
					break;
				}
			}
			
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case IDC_VIDEO_SOUND_OK:
				
				// Collect data from user interface. Pass onto game... (registry or globals).
				if (mySystem)
				{
					// ------------------------------------
					//	DISPLAY
					// Get selected display device index, 
					// and use to get back a reference to 
					// the selected string.
					ComboBox = GetDlgItem(hDlg, IDC_DISPLAY_DEVICE_LIST);
					i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
					SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer);
					
					// Locate name returned in list of display devices
					for (i = mySystem->GetNumberOfVideoDevices(); i--;)
					{
						VideoDeviceRef = mySystem->GetVideoDevice(i);
						if (!strcmp(buffer, VideoDeviceRef->Name))
						{
							LoadedVideoGUID = SelectedDisplayDevice = VideoDeviceRef->Guid;
							break;
						}
					}
					
					// ------------------------------------
					// RESOLUTION
					// Get selected resolution index, 
					// and use to get back a reference to 
					// the selected string.
					ComboBox = GetDlgItem(hDlg, IDC_RESOLUTION_LIST);
					i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
					SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer);
					
					// Read back video mode attributes. (Yes, I know this is not very nice).
					sscanf(buffer, "%dx%dx%d", &SelectedWidth, &SelectedHeight, &SelectedDepth);

					PCSaveToRegistry(	HKEY_CURRENT_USER, 
										MOHO_REGISTRY_PATH, 
										"Resolution", 
										REG_BINARY, 
										buffer, 
										strlen(buffer) + 1);
					
					
					// ------------------------------------
					// SOUND
					// Get selected sound device index, 
					// and use to get back a reference to 
					// the selected string.
					ComboBox = GetDlgItem(hDlg, IDC_SOUND_DEVICE_LIST);
					i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
					SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer);
					
					// Locate name returned in list of sound devices
					for (i = mySystem->GetNumberOfSoundDevices(); i--;)
					{
						SoundDeviceRef = mySystem->GetSoundDevice(i);
						if (!strcmp(buffer, SoundDeviceRef->Name))
						{
							LoadedSoundGUID = SelectedSoundDevice = SoundDeviceRef->Guid;
							break;
						}
					}
				}
				
				
				// ------------------------------------
				// SAVE DATA TO REGISTRY
				
				// ----
				// VIDEO
				
				memcpy((void*)buffer, &SelectedDisplayDevice, sizeof(SelectedDisplayDevice));
				PCSaveToRegistry(	HKEY_CURRENT_USER, 
					MOHO_REGISTRY_PATH, 
					"VideoDevice", 
					REG_BINARY, 
					buffer, 
					sizeof(SelectedDisplayDevice));
				
				// ----
				// SOUND
				memcpy((void*)buffer, &SelectedSoundDevice, sizeof(SelectedSoundDevice));
				PCSaveToRegistry(	HKEY_CURRENT_USER, 
					MOHO_REGISTRY_PATH, 
					"SoundDevice", 
					REG_BINARY, 
					buffer, 
					sizeof(SelectedSoundDevice));
				
				EndDialog(hDlg, TRUE);
				break;
				
			case IDC_VIDEO_SOUND_CANCEL:
				EndDialog(hDlg, FALSE);
				break;
			}
			break;
		}
		break;
    }
    return false;
}



// ----------------------------------------------------------------------------
int RunControllerDialog(HINSTANCE hInstance)
{	
	if (mySystem)
	{
		OutputDebugString("Launching IDD_CONTROLLER box.\n");
		HRESULT hr = DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONTROLLER), GetDesktopWindow(), (DLGPROC)ControllerDlgProc);
		if (hr == TRUE)
		{
			return true;
		}
		else
		{
			OutputDebugString("IDD_CONTROLLER Dialog: Cancelled or error.\n");
			return false;
		}
	}
	
	return false;
}

// ----------------------------------------------------------------------------
LRESULT CALLBACK ControllerDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND						ComboBox;
	char						buffer[10240], buffer2[256];
	int							i, j, k;
	CInputDevice*				InputDeviceRef, *HighlightThisController;
		
	// Data within this function should reference working data.
	// Working data should be initialised with loaded data.
	// Loaded data should only ever be saved over when the user clicks okay. 
	// Clicking cancel on this dialog therefore over rules clicking okay on the sub-dialogs.
	
	// In game changed to read from loaded data, and ignore the old registry method.
	
	/*
	GUID    guidInstance;
    DWORD   dwDevType;
    CHAR    tszInstanceName[MAX_PATH];
	*/
    
	// What are we processing?
	switch (uMsg)
    {
	case WM_INITDIALOG:

		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERONE_CAPTION), LangArray[12]);
		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERTWO_CAPTION), LangArray[13]);
		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERONE_CONTROL_DEVICE_CAPTION), LangArray[14]);
		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERTWO_CONTROL_DEVICE_CAPTION), LangArray[14]);
		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_CONFIGURE_PLAYERONE_DEVICE), LangArray[15]);
		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_CONFIGURE_PLAYERTWO_DEVICE), LangArray[15]);
		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_RUN_GAME_OPTIONS_PANEL), LangArray[16]);

		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_OK), LangArray[41]);
		SetWindowText(GetDlgItem(hDlg, IDC_CONTROLLER_CANCEL), LangArray[42]);

		if ((Install) || (CompleteInstall))
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CONTROLLER_CANCEL), false);
		}
		
		if (mySystem)
		{
			memset((void*)WorkingControllerConfig, 0, sizeof(tagWorkingControllerConfig) * mySystem->GetNumberOfInputDevices());
			
			// For both players.
			for (i = 2; i--;)
			{
				WorkingController[i] = NULL;//&WorkingControllerConfig[0].Player[i];
				
				// Paranoia? maybe. Clear all keys to undefined. Assume defaults and then over write if the user has defined the keys.
				for (j = mySystem->GetNumberOfInputDevices(); j--;)
				{
					// Un map all keys.
					for (k = 10; k--;)
					{
						WorkingControllerConfig[j].Player[i].ConfigKeyMapping[k].Button = -1;	// Unmapped
						
						switch (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(j)->Info.dwDevType))
						{
						case DIDEVTYPE_KEYBOARD:
							WorkingControllerConfig[j].Player[i].ConfigKeyMapping[k].Button = assignKeyboardDefaultForAction(k);
							break;
						case DIDEVTYPE_JOYSTICK:
							WorkingControllerConfig[j].Player[i].ConfigKeyMapping[k].Button = assignJoystickDefaultForAction(k);
							break;
						default:
							break;
						}
						//sprintf(buffer, "Player %d, device %d is a %s, and action %d is defined to be %d.\n", i, j, mySystem->GetInputDevice(j)->Info.tszInstanceName, k, WorkingControllerConfig[j].Player[i].ConfigKeyMapping[k].Button);
						//OutputDebugString(buffer);
					}
					
					strcpy(WorkingControllerConfig[j].Player[i].Name, mySystem->GetInputDevice(j)->Info.tszInstanceName);
					WorkingControllerConfig[j].Player[i].Guid = mySystem->GetInputDevice(j)->Info.guidInstance;
					
					// If the name of the loaded device matches the name of one of the iterated devices then copy its data over into that section position.
					if (!strcmp(LoadedController[i].Name, mySystem->GetInputDevice(j)->Info.tszInstanceName))
					{
						memcpy((void*)&WorkingControllerConfig[j].Player[i], (void*)&LoadedController[i], sizeof(LoadedController[i]));
						WorkingController[i] = &WorkingControllerConfig[j].Player[i];
					}
					
					// Update reference to device.
					WorkingControllerConfig[j].Player[i].Device = mySystem->GetInputDevice(j);
				}

				if (!WorkingController[i])
				{
					// Assign keyboard as default device
					for (j = mySystem->GetNumberOfInputDevices(); j--;)
					{
						if (i)
						{
							//		// Player one gets keyboard. This will always happen!!!
							//		if (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(j)->Info.dwDevType) == DIDEVTYPE_JOYSTICK)
							//		{
							//			WorkingController[i] = &WorkingControllerConfig[0].Player[i];
							//			//WorkingController[i]->Valid = true;
							//		}
						}
						else
						{
							// Player one gets keyboard. This will always happen!!!
							if (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(j)->Info.dwDevType) == DIDEVTYPE_KEYBOARD)
							{
								WorkingController[i] = &WorkingControllerConfig[j].Player[i];
								WorkingController[i]->Valid = true;
								break;
							}
							else
							{
								if (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(j)->Info.dwDevType) == DIDEVTYPE_JOYSTICK)
								{
									WorkingController[i] = &WorkingControllerConfig[j].Player[i];
									WorkingController[i]->Valid = true;
								}
							}
						}
					}
				}
			}
		
			// ----
			// INPUT
			
			// Loop through all the controller mapping combo 
			// boxes adding a reference to enumerated keys.
			for (i = 2; i--;)	// 2 players.
			{
				// Handle to appropriate player combo box.
				ComboBox = i?GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERTWO_CONTROLLER_LIST):GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERONE_CONTROLLER_LIST);
				
				// Go up throught the devices in order of enumeration.
				HighlightThisController = NULL;
				for (j = mySystem->GetNumberOfInputDevices(); j--;)
				{
					InputDeviceRef = mySystem->GetInputDevice(j);
					
					// Don't show mice or mouse like devices.
					if (GET_DIDEVICE_TYPE(InputDeviceRef->Info.dwDevType) == DIDEVTYPE_MOUSE)
					{
						continue;
					}
									
					if (	(InputDeviceRef->Caps.dwButtons	< 5) 
						||	(InputDeviceRef->Caps.dwAxes	< 2) 
						)
					{
						// Not a suitable joystick.
						continue;
					}
					
					// Prevent multiple names occuring. 
					if (SendMessage(ComboBox, CB_FINDSTRINGEXACT, 0, (LPARAM)(LPCTSTR)InputDeviceRef->Info.tszInstanceName) == CB_ERR)
					{
						SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)InputDeviceRef->Info.tszInstanceName);
					}
				}
				
				if (WorkingController[i])
				{
					j = SendMessage(ComboBox, CB_FINDSTRINGEXACT, 0, (LPARAM)(LPCTSTR)WorkingController[i]->Device->Info.tszInstanceName);
					SendMessage(ComboBox, CB_SETCURSEL, j, 0);
				}
			}
		}
		return TRUE;
		
	case WM_COMMAND:
		
		// Two main commands are processed.
		// 1. Change of selection 
		// 2. Button hit.
		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			if (mySystem)
			{
				switch ((int)LOWORD(wParam))
				{
				case IDC_CONTROLLER_PLAYERONE_CONTROLLER_LIST:
					
					ComboBox = GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERONE_CONTROLLER_LIST);
					i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
					j = SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer);
					
					ComboBox = GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERTWO_CONTROLLER_LIST);
					i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
					k = SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer2);
					
					if (	(j != CB_ERR)
						&&	(k != CB_ERR)
						)
					{
						if (!strcmp(buffer, buffer2))
						{
							SendMessage(ComboBox, CB_SETCURSEL, -1, 0);	// Deselect selection in player two's list box.
							WorkingController[1]	= NULL;
						}
					}
					
					// Now locate newly selected option, and set WorkingCongif to reference correct data.
					for (i = mySystem->GetNumberOfInputDevices(); i--;)
					{
						InputDeviceRef = mySystem->GetInputDevice(i);
						if (!strcmp(buffer, InputDeviceRef->Info.tszInstanceName))
						{
							// Found device in list. Reference that device.
							WorkingController[0] = &WorkingControllerConfig[i].Player[0];
							break;
						}
					}
					
					break;
					
				case IDC_CONTROLLER_PLAYERTWO_CONTROLLER_LIST:
					ComboBox = GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERTWO_CONTROLLER_LIST);
					i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
					k = SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer);
					
					ComboBox = GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERONE_CONTROLLER_LIST);
					i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
					j = SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer2);
					
					if (	(j != CB_ERR)
						&&	(k != CB_ERR)
						)
					{
						if (!strcmp(buffer, buffer2))
						{
							SendMessage(ComboBox, CB_SETCURSEL, -1, 0);	// Deselect selection in player one's list box
							WorkingController[0]	= NULL;
						}
					}
					
					// Now locate newly selected option, and set WorkingCongif to reference correct data.
					for (i = mySystem->GetNumberOfInputDevices(); i--;)
					{
						InputDeviceRef = mySystem->GetInputDevice(i);
						if (!strcmp(buffer, InputDeviceRef->Info.tszInstanceName))
						{
							// Found device in list. Reference that device.
							WorkingController[1] = &WorkingControllerConfig[i].Player[1];
							break;
						}
					}
					break;
					
				default:
					break;
				}
			}
			
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case IDC_CONTROLLER_RUN_GAME_OPTIONS_PANEL:
				WinExec("control joy.cpl", SW_NORMAL);
				break;
				
			case IDC_CONTROLLER_CONFIGURE_PLAYERONE_DEVICE:
			case IDC_CONTROLLER_CONFIGURE_PLAYERTWO_DEVICE:
				
				if (LOWORD(wParam) == IDC_CONTROLLER_CONFIGURE_PLAYERONE_DEVICE)
					ProcessingPlayer = 0;
				else
					ProcessingPlayer = 1;
				
				ComboBox = GetDlgItem(hDlg, ProcessingPlayer?IDC_CONTROLLER_PLAYERTWO_CONTROLLER_LIST:IDC_CONTROLLER_PLAYERONE_CONTROLLER_LIST);
				i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
				if (i != CB_ERR)
				{
					ShowWindow(hDlg, SW_HIDE);		// Hide dialog.					
					
					// Run appropriate dialog.
					if (GET_DIDEVICE_TYPE(WorkingController[ProcessingPlayer]->Device->Info.dwDevType) == DIDEVTYPE_JOYSTICK)
					{
						RunJoystickDialog(HInstance);
					}
					else
					{
						RunKeyboardDialog(HInstance);
					}
					
					ShowWindow(hDlg, SW_SHOW);		// Show dialog
				}
				else
				{
					MessageBox(hDlg, LangArray[17], GAME_NAME, MB_OK);
				}
				break;
				
				
			case IDC_CONTROLLER_OK:
				
				// Collect data from user interface. Pass onto game... (registry or globals).
				if (mySystem)
				{
					// Check that all keys are mapped 
					for (j = 2; j--;)
					{
						if (WorkingController[j])
						{
							for	(i = 9; i--;)
							{
								if (	(GET_DIDEVICE_TYPE(WorkingController[j]->Device->Caps.dwDevType) == DIDEVTYPE_JOYSTICK)
									&&	(	(i == 1) 
									||	(i == 3)
									)
									)
									continue;
								
								if (WorkingController[j]->ConfigKeyMapping[i].Button == -1)
								{
									MessageBox(	hDlg, 
										LangArray[32],
										GAME_NAME, 
										MB_OK);
									return false;
								}
							}
						}
					}
					
					if (WorkingController[0])
					{
						for	(i = 9; i--;)
						{
							if (	(GET_DIDEVICE_TYPE(WorkingController[0]->Device->Caps.dwDevType) == DIDEVTYPE_JOYSTICK)
								&&	(	(i == 1) 
									||	(i == 3)
									)
								)
								continue;
							
							if (WorkingController[0]->ConfigKeyMapping[i].Button == -1)
							{
								MessageBox(	hDlg, 
									LangArray[40],
									GAME_NAME, 
									MB_OK);
								return false;
							}
						}
						
						memcpy((void*)&LoadedController[0], (void*)WorkingController[0], sizeof(LoadedController[0]));
						LoadedController[0].Valid = true;
						
						// Save for later games.
						memcpy((void*)buffer, &LoadedController[0], sizeof(LoadedController[0]));
						PCSaveToRegistry(	HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH, 
							"ControllerOneDevice", 
							REG_BINARY, 
							buffer, 
							sizeof(LoadedController[0]));
						
					}
					else
					{
						MessageBox(	hDlg, 
							LangArray[18],
							GAME_NAME, 
							MB_OK	
							|	MB_ICONWARNING
							|	MB_SETFOREGROUND
							);
						LoadedController[0].Valid = false;
						return false;
					}
					
					
					
					// PLAYER TWO's DEVICE
					ComboBox = GetDlgItem(hDlg, IDC_CONTROLLER_PLAYERTWO_CONTROLLER_LIST);
					i = SendMessage(ComboBox, CB_GETCURSEL, 0, 0);
					if (SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)(LPCTSTR)buffer) != CB_ERR)
					{
						memcpy((void*)&LoadedController[1], (void*)WorkingController[1], sizeof(LoadedController[1]));
						LoadedController[1].Valid = true;
					}
					else
					{
						LoadedController[1].Device = NULL;
						*LoadedController[1].Name = NULL;
						memset((void*)&LoadedController[1].Guid, 0, sizeof(LoadedController[1].Guid));
						LoadedController[1].Valid = false;
					}
					
					// Save GUID for later games.
					memcpy((void*)buffer, &LoadedController[1], sizeof(LoadedController[1]));
					PCSaveToRegistry(	HKEY_CURRENT_USER, 
										MOHO_REGISTRY_PATH, 
										"ControllerTwoDevice", 
										REG_BINARY, 
										buffer, 
										sizeof(LoadedController[1]));
					
				}
				
				EndDialog(hDlg, TRUE);
				break;

			case IDC_CONTROLLER_CANCEL:
				EndDialog(hDlg, FALSE);
				break;
			}
			break;
		}
		break;
    }
	
    return false;
}


// ----------------------------------------------------------------------------
int RunJoystickDialog(HINSTANCE hInstance)
{	
	if (mySystem)
	{
		OutputDebugString("Launching IDD_MAP_JOYSTICK box.\n");
		HRESULT hr = DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAP_JOYSTICK), GetDesktopWindow(), (DLGPROC)JoystickDlgProc);
		if (hr == TRUE)
		{
			return true;
		}
		else
		{
			OutputDebugString("IDD_MAP_JOYSTICK Dialog: Cancelled or error.\n");
			return false;
		}
	}
	
	return false;
}

// ----------------------------------------------------------------------------
LRESULT CALLBACK JoystickDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int			i, j;
	long		AxesValue;
	char		buffer[10240];
	HWND		ComboBox;
	bool		Pressed;
	DIJOYSTATE	mJS1;
	HRESULT		hr;
	
	// What are we processing?
	switch (uMsg)
    {
	case WM_INITDIALOG:

		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_CAPTION), LangArray[34]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION_CAPTION1), LangArray[21]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION_CAPTION2), LangArray[21]);

		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_AXES_CAPTION), LangArray[35]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_BUTTON_CAPTION), LangArray[38]);
		
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION1), LangArray[36]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION2), LangArray[37]);

		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION5), LangArray[27]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION6), LangArray[28]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION7), LangArray[29]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION8), LangArray[30]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_ACTION9), LangArray[31]);
		
		for (i = 7; i--;)
		{
			SetWindowText(getMapJoystickComboByIndex(i, hDlg), LangArray[33]);
		}
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_RUN_GAME_OPTIONS_PANEL), LangArray[16]);

		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_OK), LangArray[41]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_JOYSTICK_CANCEL), LangArray[42]);


		if (mySystem)
		{
			// Make a working copy of the selected keys.
			for (i = 10; i--;)
			{
				WorkingMapping[i] = WorkingController[ProcessingPlayer]->ConfigKeyMapping[i].Button;
			}
			
			// Need to create a list of buttons.
			// Need to find selected keys, store reference, and highlight later.
			
			// For button combo boxes.
			for (i = 2; i--;)
			{
				if (WorkingMapping[i << 1] >= 0)	// -1 if undefined
				{
					ComboBox = getMapJoystickComboByIndex(i, hDlg);
					sprintf(buffer, "%s %d", LangArray[35], WorkingMapping[i << 1] + 1);
					SetWindowText(ComboBox, buffer);
				}
			}
			
			// For button combo boxes.
			for (i = 6; i >= 2; i--)
			{
				if (WorkingMapping[i + 2] >= 0)	// -1 indicates no key has been specified.
				{
					ComboBox = getMapJoystickComboByIndex(i, hDlg);
					generateOptionFromDeviceButton(	WorkingController[ProcessingPlayer]->Device, 
						WorkingMapping[i + 2], 
						buffer);
					OutputDebugString(buffer);
					if (*buffer != 0)
						SetWindowText(ComboBox, buffer);
				}
			}
		}
		break;
		
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case IDC_MAP_JOYSTICK_RUN_GAME_OPTIONS_PANEL:
				WinExec("control joy.cpl", SW_NORMAL);
				break;		
				
			case IDC_MAP_JOYSTICK_MAPPING1:	// Axis
			case IDC_MAP_JOYSTICK_MAPPING2:
				
			case IDC_MAP_JOYSTICK_MAPPING3:	// Buttons
			case IDC_MAP_JOYSTICK_MAPPING4:
			case IDC_MAP_JOYSTICK_MAPPING5:
			case IDC_MAP_JOYSTICK_MAPPING6:
			case IDC_MAP_JOYSTICK_MAPPING7:
				
				// Disable all buttons
				for (j = 7; j--;)
				{
					ComboBox = getMapJoystickComboByIndex(j, hDlg);
					EnableWindow(ComboBox, false);
				}
				EnableWindow(GetDlgItem(hDlg, LOWORD(wParam)), true);
				
				// Poll direct input device
				Pressed = false;
				while (!Pressed)
				{
					WorkingController[ProcessingPlayer]->Device->Device->Acquire();
					
					memset((void*)&mJS1, 0, sizeof(mJS1));
					hr = DIERR_INPUTLOST;
					while (DIERR_INPUTLOST == hr) 
					{
						// poll the joystick to read the current state
						hr = WorkingController[ProcessingPlayer]->Device->Device2->Poll();
						if (FAILED(hr))
						{
							OutputDebugString("Joystick, polling FAILURE!\n");
							//PrintDXError(hr);
							WorkingController[ProcessingPlayer]->Device->Device2->Acquire();
							
							memset((void*)&mJS1, 0, sizeof(mJS1)); 
							
							// Enable all buttons
							for (j = 7; j--;)
							{
								ComboBox = getMapJoystickComboByIndex(j, hDlg);
								EnableWindow(ComboBox, true);
							}
							
							return false;
						}
						
						// get the input's device state, and put the state in dims
						hr = WorkingController[ProcessingPlayer]->Device->Device->GetDeviceState(sizeof(mJS1), &mJS1);
						if (FAILED(hr))	
							memset((void*)&mJS1, 0, sizeof(mJS1));
						if (hr == DIERR_INPUTLOST)
						{
							// DirectInput is telling us that the input stream has
							// been interrupted.  We aren't tracking any state
							// between polls, so we don't have any special reset
							// that needs to be done.  We just re-acquire and
							// try again.
							OutputDebugString("Joystick, state FAILURE!\n");
							//PrintDXError(hr);
							hr = WorkingController[ProcessingPlayer]->Device->Device->Acquire();
							
							// Enable all buttons
							for (j = 7; j--;)
							{
								ComboBox = getMapJoystickComboByIndex(j, hDlg);
								EnableWindow(ComboBox, true);
							}
							
							return false;
						}
					}
					
					// Okay, we have valid data. 
					switch (LOWORD(wParam))
					{
					case IDC_MAP_JOYSTICK_MAPPING1:	// Axes
					case IDC_MAP_JOYSTICK_MAPPING2:
						for (i = WorkingController[ProcessingPlayer]->Device->Caps.dwAxes; i--;)
						{
							AxesValue = 0;
							switch (i)
							{
							case 0:
								AxesValue = mJS1.lX;
								break;
							case 1:
								AxesValue = mJS1.lY;
								break;
							case 2:
								AxesValue = mJS1.lZ;
								break;
							case 3:
								AxesValue = mJS1.lRx;
								break;
							case 4:
								AxesValue = mJS1.lRy;
								break;
							case 5:
								AxesValue = mJS1.lRz;
								break;
							default:
								OutputDebugString("This joystick has more than 6 axes!!! -> Can't use, don't know how to access data.\n");
							}
							
							if (	(AxesValue > 700) 
								||	(AxesValue < -700) 
								)
							{
								for (j = 2; j--;)
								{
									if (WorkingMapping[j << 1] == i)
									{
										ComboBox = getMapJoystickComboByIndex(j, hDlg);
										SetWindowText(ComboBox,  LangArray[33]);
										WorkingMapping[j << 1] = -1;
									}
								}
								
								sprintf(buffer, "%s %d", LangArray[35], i + 1);
								WorkingMapping[getMapJoystickIndexFromCombo(LOWORD(wParam))] = i;
								SetWindowText(GetDlgItem(hDlg, LOWORD(wParam)), buffer);
								
								Pressed = true;
								break;
							}
						}
						break;
						
					case IDC_MAP_JOYSTICK_MAPPING3:	// Buttons
					case IDC_MAP_JOYSTICK_MAPPING4:
					case IDC_MAP_JOYSTICK_MAPPING5:
					case IDC_MAP_JOYSTICK_MAPPING6:
					case IDC_MAP_JOYSTICK_MAPPING7:
						// Check the state of the buttons...
						for (i = WorkingController[ProcessingPlayer]->Device->Caps.dwButtons; i--;)
						{
							if (mJS1.rgbButtons[i] & 0x80)
							{
								// Check all others.
								for (j = 6; j >=2; j--)
								{
									if (WorkingMapping[j + 2] == i)
									{
										ComboBox = getMapJoystickComboByIndex(j, hDlg);
										SetWindowText(ComboBox, LangArray[33]);
										WorkingMapping[j + 2] = -1;
									}
								}
								// Found one that has been pressed.
								generateOptionFromDeviceButton(WorkingController[ProcessingPlayer]->Device, i, buffer);
								WorkingMapping[getMapJoystickIndexFromCombo(LOWORD(wParam))] = i;
								SetWindowText(GetDlgItem(hDlg, LOWORD(wParam)), buffer);
								
								Pressed = true;
								break;
							}
						}
						break;
					}
			
					BYTE					mKS[256];	

					// Look through all the devices for a keyboard
					for (int n = mySystem->GetNumberOfInputDevices(); n--;)
					{
						if (GET_DIDEVICE_TYPE(mySystem->GetInputDevice(n)->Info.dwDevType) == DIDEVTYPE_KEYBOARD)
						{
							mySystem->GetInputDevice(n)->Device->Acquire();
					
							ZeroMemory(&mKS, sizeof(mKS));
							hr = mySystem->GetInputDevice(n)->Device->GetDeviceState(sizeof(mKS), &mKS);
							if (SUCCEEDED(hr)) 
							{
								// Key pressed?
								if (mKS[DIK_ESCAPE] & 0x80)
								{
									Pressed = true;
									break;
								}
							}
						}
					}
							
							//if (	(GetAsyncKeyState(VK_ESCAPE) & 0x80)
					//	||	(GetAsyncKeyState(VK_ESCAPE) & 0x01)
					//	)
					//{
					//	Pressed = true;
					//}
			}
			// Enable all buttons
			for (j = 7; j--;)
			{
				ComboBox = getMapJoystickComboByIndex(j, hDlg);
				EnableWindow(ComboBox, true);
			}
			
			// Now remove all messages, or they simply get processed when we return. (And that basically voids the point of disabling the buttons.
			//while (PeekMessage(&Msg, hDlg, 0, 0, PM_REMOVE))	// Seemingly never exits.
			//{
			//	OutputDebugString(".");
			//}
			
			break;
			
		case IDC_MAP_JOYSTICK_OK:
			// Collect settings		// Settings are assigned as values are assigned
			// Verify				// Below
			// Save settings		// Below.
			// Exit					// Ditto.
			
			for (i = 9; i--;)
			{
				if (	(i == 1) 
					||	(i == 3))
					continue;
				
				if (WorkingMapping[i] == -1)
				{
					MessageBox(	hDlg, 
						LangArray[32],
						GAME_NAME, 
						MB_OK);
					return false;
				}
			}
			
			
			for (i = 10; i--;)
			{
				WorkingController[ProcessingPlayer]->ConfigKeyMapping[i].Button = WorkingMapping[i];
			}
			EndDialog(hDlg, true);
			break;
			
		case IDC_MAP_JOYSTICK_CANCEL:
			EndDialog(hDlg, false);
			break;
		}
	}
	}
	
	return false;//DefDlgProc(hDlg, uMsg, wParam, lParam);
}

// ----------------------------------------------------------------------------
int RunKeyboardDialog(HINSTANCE hInstance)
{	
	if (mySystem)
	{
		OutputDebugString("Launching IDD_MAP_KEYBOARD box.\n");
		HRESULT hr = DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAP_KEYBOARD), GetDesktopWindow(), (DLGPROC)KeyboardDlgProc);
		if (hr == TRUE)
		{
			return true;
		}
		else
		{
			OutputDebugString("IDD_MAP_KEYBOARD Dialog: Cancelled or error.\n");
			return false;
		}
	}
	
	return false;
}


// ----------------------------------------------------------------------------
LRESULT CALLBACK KeyboardDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int		i, j;
	char	buffer[10240];
	HWND	ComboBox;
	BYTE	mKS[256];
	bool	Pressed;
	HRESULT	hr;
	
	// What are we processing?
	switch (uMsg)
    {
	case WM_INITDIALOG:

		// Label everything in the correct language.
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_CAPTION), LangArray[20]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION_CAPTION), LangArray[21]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_KEY_CAPTION), LangArray[22]);
		
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION1), LangArray[23]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION2), LangArray[24]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION3), LangArray[25]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION4), LangArray[26]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION5), LangArray[27]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION6), LangArray[28]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION7), LangArray[29]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION8), LangArray[30]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_ACTION9), LangArray[31]);
		
		for (i = 9; i--;)
		{
			SetWindowText(getMapKeyboardComboByIndex(i, hDlg), LangArray[33]);
		}

		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_OK), LangArray[41]);
		SetWindowText(GetDlgItem(hDlg, IDC_MAP_KEYBOARD_CANCEL), LangArray[42]);
		

		if (mySystem)
		{
			// Make a working copy of the selected keys.
			for (i = 10; i--;)
			{
				WorkingMapping[i] = WorkingController[ProcessingPlayer]->ConfigKeyMapping[i].Button;
			}
			
			// For all combo boxes.
			for (i = 9; i--;)
			{
				if (WorkingMapping[i] >= 0)
				{
					ComboBox = getMapKeyboardComboByIndex(i, hDlg);
					generateOptionFromDeviceButton(	WorkingController[ProcessingPlayer]->Device,
						WorkingMapping[i], 
						buffer);
					if (*buffer)
						SetWindowText(ComboBox, buffer);
				}
			}
		}
		break;
		
	case WM_COMMAND:
		
		switch (HIWORD(wParam))
		{			
		case BN_CLICKED:
			switch (LOWORD(wParam))
			{
			case IDC_MAP_KEYBOARD_MAPPING1:	// Axes
			case IDC_MAP_KEYBOARD_MAPPING2:	
			case IDC_MAP_KEYBOARD_MAPPING3:	
			case IDC_MAP_KEYBOARD_MAPPING4:
			case IDC_MAP_KEYBOARD_MAPPING5:// Buttons
			case IDC_MAP_KEYBOARD_MAPPING6:
			case IDC_MAP_KEYBOARD_MAPPING7:
			case IDC_MAP_KEYBOARD_MAPPING8:
			case IDC_MAP_KEYBOARD_MAPPING9:
				
				// Disable all buttons
				for (j = 9; j--;)
				{
					ComboBox = getMapKeyboardComboByIndex(j, hDlg);
					EnableWindow(ComboBox, false);
				}
				EnableWindow(GetDlgItem(hDlg, LOWORD(wParam)), true);
				
				// Poll direct input device
				Pressed = false;
				while (!Pressed)
				{
					WorkingController[ProcessingPlayer]->Device->Device->Acquire();
					
					ZeroMemory(&mKS, sizeof(mKS));
					hr = WorkingController[ProcessingPlayer]->Device->Device->GetDeviceState(sizeof(mKS), &mKS);
					if (FAILED(hr)) 
					{
						memset((void*)&mKS, 0, sizeof(mKS));
						
						// If input is lost then acquire and keep trying 
						hr = WorkingController[ProcessingPlayer]->Device->Device->Acquire();
						while (hr == DIERR_INPUTLOST) 
							hr = WorkingController[ProcessingPlayer]->Device->Device->Acquire();
						
						for (j = 9; j--;)
						{
							ComboBox = getMapKeyboardComboByIndex(j, hDlg);
							EnableWindow(ComboBox, true);
						}
						return false;
					}
					else
					{
						// Valid data.
						for (i = 256; i--;)
						{
							// Key pressed?
							if (mKS[i] & 0x80)
							{
								// i == KeyScanCode
								// LOWORD(wParam) == button 
								
								generateOptionFromDeviceButton(WorkingController[ProcessingPlayer]->Device, i, buffer);
								if (*buffer)
								{
									// Check all other buttons
									for (j = 9; j--;)
									{
										if (WorkingMapping[j] == i)
										{
											
											ComboBox = getMapKeyboardComboByIndex(j, hDlg);
											// Then clear.
											SetWindowText(ComboBox, LangArray[33]);
											WorkingMapping[j] = -1;
										}
									}
									
									WorkingMapping[getMapKeyboardIndexFromCombo(LOWORD(wParam))] = i;
									SetWindowText(GetDlgItem(hDlg, LOWORD(wParam)), buffer);
								}
								Pressed = true;
								break;
							}
						}
					}
				}
				
				for (j = 9; j--;)
				{
					ComboBox = getMapKeyboardComboByIndex(j, hDlg);
					EnableWindow(ComboBox, true);
				}
				
				break;
				
				
			case IDC_MAP_KEYBOARD_OK:
				// Collect settings	// Done as we go.
				// Verify settings.	// Need to do this!!!
				// Save settings	// Below
				// Exit				// Below.
				
				for (i = 9; i--;)
				{
					if (WorkingMapping[i] == -1)
					{
						MessageBox(	hDlg, 
							LangArray[32],
							GAME_NAME, 
							MB_OK);
						return false;
					}
				}
				
				// Make a working copy of the selected keys.
				for (i = 10; i--;)
				{
					WorkingController[ProcessingPlayer]->ConfigKeyMapping[i].Button = WorkingMapping[i];
				}
				
				EndDialog(hDlg, true);
				break;
				
			case IDC_MAP_KEYBOARD_CANCEL:
				EndDialog(hDlg, false);
				break;
		}
	}
	}
	
    return false;//DefDlgProc(hDlg, uMsg, wParam, lParam);
}

// ----------------------------------------------------------------------------
SINT assignKeyboardDefaultForAction(SINT _Action)
{
	switch (_Action)
	{
	case 0:				
		return DIK_UP;
	case 1:				
		return DIK_DOWN;
	case 2:				
		return DIK_LEFT;
	case 3:				
		return DIK_RIGHT;
	case 4:				
		return DIK_X;			// JUMP
	case 5:				
		return DIK_SPACE;		// PUNCH
	case 6:				
		return DIK_Z;			// BLOCK
	case 7:				
		return DIK_LCONTROL;	// BRAKE
	case 8:				
		return DIK_LSHIFT;		// THROW
	default:
		return -1;	// undefined
	}
}

// ----------------------------------------------------------------------------
SINT assignJoystickDefaultForAction(SINT _Action)
{
	switch (_Action)
	{
	case 0:				
		return 1;	// AXES 1
	case 2:				
		return 0;	// AXES 0.
	case 4:				
		return 0;	// JUMP
	case 5:				
		return 1;	// PUNCH
	case 6:				
		return 2;	// BLOCK
	case 7:				
		return 3;	// BRAKE
	case 8:				
		return 4;	// THROW
	default:
		return -1;	// undefined
	}
}

bool	CorrectlyInstalled()
{
	bool ret;

	ret = true;

	ret &= PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "VideoDevice",			REG_BINARY);
	ret &= PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "Resolution",			REG_BINARY);
	ret &= PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "SoundDevice",			REG_BINARY);
	ret &= PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "EffectsVolume",		REG_BINARY);
	ret &= PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "MusicVolume",			REG_BINARY);
	ret &= PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "ControllerOneDevice", REG_BINARY);
	ret &= PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "ControllerTwoDevice", REG_BINARY);
	ret &= PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "DataPath",			REG_SZ);
	
	return ret;
}


// ----------------------------------------------------------------------------
// Load settings from registry
void	LoadSettings()
{
	// Registry loading.
	DWORD				DataType, DataLength;
	char*				Value;

	bool				ResOkay;

	// Temp handles
	CSoundDevice*		SoundDeviceRef;
	CVideoDevice*		VideoDeviceRef;
	
	// ----
	// VIDEO
	memset((void*)&LoadedVideoGUID, 0, sizeof(LoadedVideoGUID));
	PCLoadFromRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "VideoDevice", &DataType, &Value, &DataLength);
	if ((DataType == REG_BINARY) && 
		(Value) && 
		(DataLength > 0))
	{
		memcpy((void*)&LoadedVideoGUID, Value, DataLength);
		
		// Free memory
		delete[] Value;
		Value = NULL;
	}
	else
	{
		// Means by which to determine best video device.
	}

	ResOkay = false;
	PCLoadFromRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "Resolution", &DataType, &Value, &DataLength);
	if (	(DataType == REG_BINARY) 
		&& 	(Value) 
		&& 	(DataLength > 0)
		&& 	(*(Value + DataLength - 1) == 0)
		)
	{
		sscanf(Value, "%dx%dx%d", &SelectedWidth, &SelectedHeight, &SelectedDepth);
		
		// Validate resolution
		for (int i = mySystem->GetNumberOfVideoDevices(); i--;)
		{
			for (int j = mySystem->GetVideoDevice(i)->GetNumberOfResolutions(); j--;)
			{
				VideoDeviceRef = mySystem->GetVideoDevice(i);
				if (LoadedVideoGUID == VideoDeviceRef->Guid)
				{
					if (	(SelectedDepth	>= 16)
						&&	(SelectedWidth	>= 640)
						&&	(SelectedHeight >= 480)
						&&	(SelectedWidth	== VideoDeviceRef->GetResolution(j)->dwWidth)
						&&	(SelectedHeight	== VideoDeviceRef->GetResolution(j)->dwHeight)
						&&	(SelectedDepth	== VideoDeviceRef->GetResolution(j)->ddpfPixelFormat.dwRGBBitCount)
						)
					{
						ResOkay = true;
						break;
					}
				}
			}
		}
			

		// Free memory
		delete[] Value;
		Value = NULL;
	}

	// Default resolution.
	if (!ResOkay)
	{
		SelectedWidth	= 640;
		SelectedHeight	= 480;
		SelectedDepth	= 16;
	}

	// LOAD RESOLUTION!!!
	// VERIFY RES
	// Selected default if missing or invalid.
	
	// ----
	// SOUND
	memset((void*)&LoadedSoundGUID, 0, sizeof(LoadedSoundGUID));
	PCLoadFromRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "SoundDevice", &DataType, &Value, &DataLength);
	if ((DataType == REG_BINARY) && 
		(Value) && 
		(DataLength > 0))
	{
		// Get saved device GUID.
		memcpy((void*)&LoadedSoundGUID, Value, DataLength);
		
		// Free memory
		delete[] Value;
		Value = NULL;
	}
	else
	{
		// No preselected device, thus use determine the best available.
		for (int i = mySystem->GetNumberOfSoundDevices(); i--;)
		{
			SoundDeviceRef = mySystem->GetSoundDevice(i);
			
			if (	(SoundDeviceRef->Caps.dwFlags & DSCAPS_SECONDARYMONO) 
				||	(SoundDeviceRef->Caps.dwFlags & DSCAPS_SECONDARYSTEREO) 
				)
			{
				LoadedSoundGUID = SoundDeviceRef->Guid;
			}
		}
	}
	
	// Sound volume
	PCLoadFromRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "EffectsVolume", &DataType, &Value, &DataLength);
	if ((DataType == REG_BINARY) && 
		(Value) && 
		(DataLength > 0))
	{
		// Sound effects volume in bounds of 0->10
		CAREER.mSoundVolume = max(0, min(10, ((SINT)*Value)));

		// Free memory
		delete[] Value;
		Value = NULL;
	}
	
	// Music volume
	PCLoadFromRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "MusicVolume", &DataType, &Value, &DataLength);
	if ((DataType == REG_BINARY) && 
		(Value) && 
		(DataLength > 0))
	{
		// Music volume in bounds of 0->10
		CAREER.mMusicVolume = max(0, min(10, ((SINT)*Value)));

		// Free memory
		delete[] Value;
		Value = NULL;
	}


	// ----
	// INPUT		
	memset((void*)&LoadedController[0], 0, sizeof(LoadedController[0]));
	PCLoadFromRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "ControllerOneDevice", &DataType, &Value, &DataLength);
	if ((DataType == REG_BINARY) && 
		(Value) && 
		(DataLength > 0))
	{
		// Get saved device GUID.
		memcpy((void*)&LoadedController[0], Value, DataLength);
		
		// Free memory
		delete[] Value;
		Value = NULL;
	}
	
	memset((void*)&LoadedController[1], 0, sizeof(LoadedController[1]));
	PCLoadFromRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "ControllerTwoDevice", &DataType, &Value, &DataLength);
	if ((DataType == REG_BINARY) && 
		(Value) && 
		(DataLength > 0))
	{
		// Get saved device GUID.
		memcpy((void*)&LoadedController[1], Value, DataLength);
		
		// Free memory
		delete[] Value;
		Value = NULL;
	}
}



// ----------------------------------------------------------------------------
// Checks for the game CD.
bool	CheckCD()
{
	DWORD	Drives;
	FILE*	FH;
	
	// Check for MoHo CD - can be found in any [CD] drive.
	Drives = GetLogicalDrives();
	if (Drives)
	{
		for (int i = 0; i < 32; i++)
		{
			if (Drives & (1 << i))
			{
				sprintf(WhereIsSetup, "%c:\\", i + 65);
				if (GetDriveType(WhereIsSetup) == DRIVE_CDROM)
				{
					OutputDebugString("Drive ");
					OutputDebugString(WhereIsSetup);
					OutputDebugString(" is a CDROM.\n");
					
					strcat(WhereIsSetup, "MoHo.exe");
					
					// Check for a file on the disk.
					FH = fopen(WhereIsSetup, "r");
					if (FH)
					{
						fclose(FH);
						sprintf(WhereIsSetup, "%c:\\", i + 65);
						return true;
					}
				}
			}
		}
		
		// Ran out of drives to check.
		return false;
	}
	else
	{
		// There are no drives!!! 
		// Assume that the call to GetLogicalDrives failed. 
		// Let them off this one time?
		return true;
	}
}

// Convert from config menu index to in game control function code.
SINT MenuIndexToInGameFunction(SINT _Index)
{
	switch (_Index)
	{
	case 0:
	case 10:
		return BUTTON_ANALOGUE_UP;
	case 1:
	case 11:
		return BUTTON_ANALOGUE_DOWN;
	case 2:
	case 12:
		return BUTTON_ANALOGUE_LEFT;
	case 3:
	case 13:
		return BUTTON_ANALOGUE_RIGHT;
	case 4:
	case 14:
		return BUTTON_FIRE_PRIMARY;		// PUNCH
	case 5:
	case 15:
		return BUTTON_FIRE_SECONDARY;	// JUMP
	case 6:
	case 16:
		return BUTTON_BLOCK;			// BLOCK
	case 7:
	case 17:
		return BUTTON_BRAKE;			// BRAKE
	case 8:
	case 18:
		return BUTTON_THROW;			// THROW
	default:
		return -1;
	}	
}


HWND getStartOptionByIndex(int _Index, HWND _Window)
{
	switch (_Index)
	{
	case 0:
		return GetDlgItem(_Window, IDC_START_INSTALL);
	case 1:
		return GetDlgItem(_Window, IDC_START_PLAY);
	case 2:
		return GetDlgItem(_Window, IDC_START_DEVICE_OPTIONS);
	case 3:
		return GetDlgItem(_Window, IDC_START_CONTROLLER_OPTIONS);
	case 4:
		return GetDlgItem(_Window, IDC_START_UNINSTALL);
	case 5:
		return GetDlgItem(_Window, IDC_START_QUIT);
		
	default: 
		return NULL;
	}
}


HWND getMapJoystickComboByIndex(int _Index, HWND _Window)
{
	switch (_Index)
	{
	case 0:
		return GetDlgItem(_Window, IDC_MAP_JOYSTICK_MAPPING1);
	case 1:
		return GetDlgItem(_Window, IDC_MAP_JOYSTICK_MAPPING2);
	case 2:
		return GetDlgItem(_Window, IDC_MAP_JOYSTICK_MAPPING3);
	case 3:
		return GetDlgItem(_Window, IDC_MAP_JOYSTICK_MAPPING4);
	case 4:
		return GetDlgItem(_Window, IDC_MAP_JOYSTICK_MAPPING5);
	case 5:
		return GetDlgItem(_Window, IDC_MAP_JOYSTICK_MAPPING6);
	case 6:
		return GetDlgItem(_Window, IDC_MAP_JOYSTICK_MAPPING7);
		
	default: 
		return NULL;
	}
}

SINT getMapJoystickIndexFromCombo(SINT _Window)
{
	switch (_Window)
	{
	case IDC_MAP_JOYSTICK_MAPPING1:
		return 0;
	case IDC_MAP_JOYSTICK_MAPPING2:
		return 2;
		
	case IDC_MAP_JOYSTICK_MAPPING3:
		return 4;
	case IDC_MAP_JOYSTICK_MAPPING4:
		return 5;
	case IDC_MAP_JOYSTICK_MAPPING5:
		return 6;
	case IDC_MAP_JOYSTICK_MAPPING6:
		return 7;
	case IDC_MAP_JOYSTICK_MAPPING7:
		return 8;
		
	default: 
		return NULL;
	}
}

SINT getMapKeyboardIndexFromCombo(SINT _Window)
{
	switch (_Window)
	{
	case IDC_MAP_KEYBOARD_MAPPING1:
		return 0;
	case IDC_MAP_KEYBOARD_MAPPING2:
		return 1;
	case IDC_MAP_KEYBOARD_MAPPING3:
		return 2;
	case IDC_MAP_KEYBOARD_MAPPING4:
		return 3;
	case IDC_MAP_KEYBOARD_MAPPING5:
		return 4;
	case IDC_MAP_KEYBOARD_MAPPING6:
		return 5;
	case IDC_MAP_KEYBOARD_MAPPING7:
		return 6;
	case IDC_MAP_KEYBOARD_MAPPING8:
		return 7;
	case IDC_MAP_KEYBOARD_MAPPING9:
		return 8;
		
	default: 
		return NULL;
	}
}



HWND getMapKeyboardComboByIndex(int _Index, HWND _Window)
{
	switch (_Index)
	{
	case 0:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING1);
	case 1:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING2);
	case 2:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING3);
	case 3:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING4);
	case 4:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING5);
	case 5:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING6);
	case 6:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING7);
	case 7:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING8);
	case 8:
		return GetDlgItem(_Window, IDC_MAP_KEYBOARD_MAPPING9);
		
	default: 
		return NULL;
	}
}

// ----
// Generates a string to use as an option in the menu for a given device and button.
char* generateOptionFromDeviceButton(CInputDevice* _Device, SINT _Button, char* _Buffer)
{
	//char	shortName[MAX_PATH];
	//char*	index, 
	char*	keyNameBuffer;
	
	//strcpy(shortName, _Device->Info.tszInstanceName);
	//index = strchr(shortName, ' ');
	//if (index)
	//	*index = 0;
	
	if (GET_DIDEVICE_TYPE(_Device->Info.dwDevType) == DIDEVTYPE_KEYBOARD)
	{
		keyNameBuffer = DIScanCodeToKeyString(	_Button, 
												PCSetupQueryLanguage,
												_Device->Caps.dwDevType);
		
		if (keyNameBuffer)
		{
			//sprintf(_Buffer, "%s %s", shortName, keyNameBuffer);
			strcpy(_Buffer, keyNameBuffer);
		}
		else
		{
			*_Buffer = 0;
		}
	}
	else
	{	
		// LANGUAGE
		sprintf(_Buffer, "%s %d", LangArray[38], _Button + 1);	// The +1 means that users see 1 based values for buttons. - The DirectX Gaming Options Panel labels the buttons from one, too.
	}
	
	return _Buffer;
}


// -----
// Returns true if the option text could be tracked back to a device and button.
bool	getDeviceAndButtonFromOption(char* _Option, CInputDevice* _Device, SINT* _Button)
{
	SINT			KeyCount;
	CInputDevice*	InputDeviceRef;
	char			Buffer[MAX_PATH];
	
	if (mySystem)
	{
		// All enumerated devices
		for (int i = mySystem->GetNumberOfInputDevices(); i--;)
		{
			InputDeviceRef = mySystem->GetInputDevice(i);
			KeyCount = InputDeviceRef->Caps.dwButtons;
			if (GET_DIDEVICE_TYPE(InputDeviceRef->Info.dwDevType) == DIDEVTYPE_KEYBOARD)
			{
				KeyCount = MAX_KEY_INDEX;
			}
			
			// All buttons on the device, or the maxamum defined for a keyboard.
			for (int k = KeyCount; k--;)
			{
				generateOptionFromDeviceButton(InputDeviceRef, k, Buffer);
				if (!strcmp(Buffer, _Option))
				{
					// Matching pair!!!
					_Device		= InputDeviceRef;
					if (GET_DIDEVICE_TYPE(InputDeviceRef->Info.dwDevType) == DIDEVTYPE_JOYSTICK)
					{
						*_Button = (k + 256);	// Offset for joystick.
					}
					
					if (GET_DIDEVICE_TYPE(InputDeviceRef->Info.dwDevType) == DIDEVTYPE_KEYBOARD)
					{
						*_Button = DIKToString[k].DIK;
					}
					return true;
				}
			}
		}
	}
	return false;
}

#endif
