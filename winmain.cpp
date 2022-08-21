// $Header$

// $Log$
// Revision 1.12  2000-10-23 09:08:47+01  are
// <>
//
// Revision 1.11  2000-10-20 12:21:27+01  are
// CHDIR to path as specified by registry.
//
// Revision 1.10  2000-10-06 11:35:46+01  are
// Call to free memory used to store save game names.
//
// Revision 1.9  2000-10-04 12:18:45+01  jjs
// Removes PC memory leaks.
//
// Revision 1.8  2000-09-29 16:58:19+01  are
// Saves out volume settings to registry.
//
// Revision 1.7  2000-09-29 15:45:58+01  are
// Delete mySystem object when finished.
//
// Revision 1.6  2000-09-28 17:29:55+01  are
// Run start game dialog instead of old setup dialog.
//
// Revision 1.5  2000-09-25 09:35:39+01  are
// <>
//
// Revision 1.4  2000-09-25 05:16:54+01  jcl
// <>
//
// Revision 1.3  2000-09-23 12:36:06+01  jjs
// Commented out the DialogBox cos it creashes.
//

#include	"Common.h"

#if TARGET == PC

#include	"ttshell.h"
#include	"Game.h"
#include	"Globals.h"
#include	<time.h>
#include	<crtdbg.h>
#include	<direct.h>
#include	<process.h> 

#include	"PCSetupQuery.h"
#include	"OkayToMap.h"

#ifdef _DEBUG
FILE *logFile;

const char lineStr[] = { "---------------------------------------\
--------------------------------------\n"  };

 

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Memory allocation checks
#ifdef _DEBUG
	char timeStr[10], dateStr[10];         // Used to set up log file
		
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF );
	
	// Open a log file for the hook functions to use 
	logFile = fopen( "MEM-LOG.TXT", "w" );

	_strtime( timeStr );
	_strdate( dateStr );
	fprintf( logFile, 
		"Memory Allocation Log File for MoHo, run at %s on %s.\n",
		timeStr, dateStr );
	fputs( lineStr, logFile );
	
#endif                     // End of #ifdef _DEBUG

	DWORD DataLength;
	DWORD DataType;
	char* Value;


	if (PCValidateRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "DataPath",	REG_SZ))
	{
		char cwd[MAX_PATH];
		_getcwd(cwd, MAX_PATH);

		if (!(PCLoadFromRegistry(	HKEY_CURRENT_USER,	// Existing key or default root.
									MOHO_REGISTRY_PATH,	// Sub key, relative to _ExistingKey. (Must not start with a '/' char!)
									"DataPath",			// Name of value to retrieve.
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
				if (DataType == REG_SZ) 
				{
					if (strlen(cwd) > 0)
					{
						if (cwd[strlen(cwd) - 1] != '\\')
						{
							strcat(cwd, "\\");
						}
					}

					if (_stricmp(cwd, Value))
					{
						// Paths are different! (i.e. running from CD) 
						// Will see if I can find the moho exe in DataPath, 
						// and run that if it exists.

						_chdir(Value);
						char buffer[MAX_PATH];
						sprintf(buffer, "%sMoHo.exe", Value);
						if (_execl(buffer, buffer, NULL) != -1)
						{
							delete[] Value;
							Value = NULL;

							// Close down this instance of the game.
							return false;
						}
					}
				}
				
				delete[] Value;
				Value = NULL;
			}
		}
	}


	// Run start game dialog / config process.
	if (!RunGameDialog(hInstance))
	{
		return true; // Early exit.
	}
	else
	{
		DWORD	DataType;
		char*	Value;
		DWORD	DataLength;

		PCLoadFromRegistry(HKEY_CURRENT_USER, MOHO_REGISTRY_PATH, "DataPath", &DataType, &Value, &DataLength);
		if (	(DataType == REG_SZ) 
			&& 	(Value) 
			&& 	(DataLength > 0)
			&& 	(*(Value + DataLength - 1) == 0)
			)
		{
			_chdir(Value);
		}
	}
	
	// Now start the game.
	TTApp theApp;
	if (theApp.Init(hInstance, lpCmdLine))
	{
		// A most definate 'hack' to allow the controller objects to have access to the joystick data.
		CONTROLS.mControllers[0].AssignJoystickDataHandle(theApp.GetDPWin()->GetDInput()->GetJoyState(0));
		CONTROLS.mControllers[1].AssignJoystickDataHandle(theApp.GetDPWin()->GetDInput()->GetJoyState(1));

		// Lets go!
		theApp.Run();		

		// Save out volume info
		PCSaveToRegistry(	HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH, 
							"EffectsVolume", 
							REG_BINARY, 
							(char*)&CAREER.mSoundVolume, 
							sizeof(CAREER.mSoundVolume)
							);

		PCSaveToRegistry(	HKEY_CURRENT_USER, 
							MOHO_REGISTRY_PATH, 
							"MusicVolume", 
							REG_BINARY, 
							(char*)&CAREER.mMusicVolume, 
							sizeof(CAREER.mMusicVolume)
							);
	
		// Clean up
		theApp.ShutDown();	
		if (mySystem)
		{
			delete mySystem;
			mySystem = NULL;
		}

		// Free file list memory.
		PCEndFileList();
	}
	else
	{
		MessageBox(NULL, "Initialization failure!", NULL, MB_OK | MB_ICONEXCLAMATION);
	}
	CONTROLS.mControllers[0].mControllerConfig.ClearUp();
	CONTROLS.mControllers[1].mControllerConfig.ClearUp();
#ifdef _DEBUG
	if ( _CrtDumpMemoryLeaks() )
		::MessageBox( NULL , "Memory leaks detected on exit!\nCheck debugging traces for details." ,
					  GAME_NAME , MB_ICONEXCLAMATION|MB_OK );
#endif
	
	return 0;
}

#endif
