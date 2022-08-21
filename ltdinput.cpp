#include "common.h"

#if TARGET == PC

#include "LTDInput.h"
#include "PCDirectXErrorToString.h"
#include "Globals.h"
#include "PCSetupQuery.h"

//======================================================-==--=-- --  -
LTDInput::LTDInput()
{
	mpDI				= NULL; 		
	mpJoystick1			= NULL; 	
	mpJoystick1Device2	= NULL;  // needed to poll joystick
	mpJoystick2			= NULL; 	
	mpJoystick2Device2	= NULL;  // needed to poll joystick
	mpKeyboard			= NULL;
	mpMouse				= NULL;
	mhInst				= NULL;
	mbActive			= TRUE;
	memset(&mMS,0,sizeof(mMS));
	memset(&mJS1,0,sizeof(mJS1));
	memset(&mJS2,0,sizeof(mJS2));
}
//======================================================-==--=-- --  -
LTDInput::~LTDInput()
{
}
//======================================================-==--=-- --  -
HRESULT LTDInput::Init(HINSTANCE hInstance, HWND hDlg )
{
	DIPROPRANGE	diprg; 
	HRESULT		ret = DI_OK;
	HRESULT		hr;

	// Hard coded size
	memset(&mKS, 0, 256);
	memset(&mMS, 0, sizeof(mMS));
	memset(&mJS1, 0, sizeof(mJS1));
	memset(&mJS2, 0, sizeof(mJS2));

	mhInst = hInstance;
	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use.

	// This function generates the following error message on Win2000. 
	// "Failed to Create default access regkey System\CurrentControlSet\Control\MediaResources\Joystick, lRc=0x5"
	// As far as I can see the key does not exist!!!
	hr = DirectInputCreate(mhInst, DIRECTINPUT_VERSION, &mpDI, NULL);
	if (FAILED(hr)) 
		return hr;

	// ----
	// Mouse

	// Obtain an interface to the system keyboard device.
	if (FAILED(hr = mpDI->CreateDevice(GUID_SysMouse, &mpMouse, NULL)))
		return hr;
	
	if (mpMouse)
	{
		// Set the data format to "mouse format" - a predefined data format 
		if (FAILED(hr = mpMouse->SetDataFormat(&c_dfDIMouse2)))
		{
			PrintDXError(ret = hr);
			goto doKeyboard;
		}
		
		// Set the cooperativity level to let DirectInput know how
		// this device should interact with the system and with other
		// DirectInput applications.
		hr = mpMouse->SetCooperativeLevel(	hDlg,	
#ifdef _DEBUG
												DISCL_NONEXCLUSIVE
#else
												DISCL_EXCLUSIVE
#endif
											|	DISCL_FOREGROUND);
		if (FAILED(hr))
		{
			PrintDXError(ret = hr);
			goto doKeyboard;
		}
	}


	// ----
	// Keyboard

doKeyboard:
	// Obtain an interface to the system keyboard device.
	if (FAILED(hr = mpDI->CreateDevice(GUID_SysKeyboard, &mpKeyboard, NULL)))
		return hr;
	
	if (mpKeyboard)
	{
		// Set the data format to "keyboard format" - a predefined data format 
		//
		// A data format specifies which controls on a device we
		// are interested in, and how they should be reported.
		//
		// This tells DirectInput that we will be passing an array
		// of 256 bytes to IDirectInputDevice::GetDeviceState.
		if (FAILED(hr = mpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			PrintDXError(ret = hr);
			goto enumerateJoysticks;
		}
		
		// Set the cooperativity level to let DirectInput know how
		// this device should interact with the system and with other
		// DirectInput applications.
		hr = mpKeyboard->SetCooperativeLevel(hDlg,		DISCL_NOWINKEY 
													|	DISCL_NONEXCLUSIVE
													|	DISCL_FOREGROUND);
		if (FAILED(hr))
		{
			PrintDXError(ret = hr);
			goto enumerateJoysticks;
		}
	}

	// ----
	// Joystick enumeration

enumerateJoysticks:

	// Look for two joysticks, setting mpJoystick1 and 2 if found.
	hr = mpDI->EnumDevices( DIDEVTYPE_JOYSTICK,
							EnumJoysticksCallback,
							this,
							DIEDFL_ATTACHEDONLY);
	if (FAILED(hr)) 
	{
		PrintDXError(ret = hr);
		return hr;
	}

	// ----
	// Joystick One

	// If joystick1 created..
	if (mpJoystick1)
	{
		// Set the data format to "simple joystick" - a predefined data format 
		//
		// A data format specifies which controls on a device we
		// are interested in, and how they should be reported.
		//
		// This tells DirectInput that we will be passing a
		// DIJOYSTATE structure to IDirectInputDevice::GetDeviceState.
		hr = mpJoystick1->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			ret = hr;
			goto doJoystick2;
		}

		// Set the cooperativity level to let DirectInput know how
		// this device should interact with the system and with other
		// DirectInput applications.
		hr = mpJoystick1->SetCooperativeLevel(	hDlg, 
												DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			ret = hr;
			goto doJoystick2;
		}

		// Determine how many axis the joystick has (so we don't error out setting
		// properties for unavailable axis)
		mdiDevCaps.dwSize = sizeof(DIDEVCAPS);
		hr = mpJoystick1->GetCapabilities(&mdiDevCaps);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			ret = hr;
			goto doJoystick2;
		}
		
		diprg.diph.dwSize		= sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= -1000; 
		diprg.lMax				= +1000; 

		diprg.diph.dwObj = DIJOFS_X;	// set the x-axis range
		hr = mpJoystick1->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			ret = hr;
			goto doJoystick2;
		}

		diprg.diph.dwObj = DIJOFS_Y;	// set the y-axis range
		hr = mpJoystick1->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			ret = hr;
			goto doJoystick2;
		}

		if (mdiDevCaps.dwAxes >= 3)
		{
			diprg.diph.dwObj = DIJOFS_Z;	// set the z-axis range
			hr = mpJoystick1->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (FAILED(hr)) 
			{
				PrintDXError(hr);
				ret = hr;
				goto doJoystick2;
			}
		}
		if (mdiDevCaps.dwAxes >= 4)
		{
			diprg.diph.dwObj = DIJOFS_RZ;	// set the rudder range
			hr = mpJoystick1->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (FAILED(hr)) 
			{
				PrintDXError(hr);
				ret = hr;
				goto doJoystick2;
			}
		}
	}
	

	// ----
	// Joystick Two
doJoystick2:

	// If joystick2 created..
	if (mpJoystick2)
	{
		// Set the data format to "simple joystick" - a predefined data format 
		//
		// A data format specifies which controls on a device we
		// are interested in, and how they should be reported.
		//
		// This tells DirectInput that we will be passing a
		// DIJOYSTATE structure to IDirectInputDevice::GetDeviceState.
		hr = mpJoystick2->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			return hr;
		}
		

		// Set the cooperativity level to let DirectInput know how
		// this device should interact with the system and with other
		// DirectInput applications.
		hr = mpJoystick2->SetCooperativeLevel(	hDlg, 
												DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			return hr;
		}

		// Determine how many axis the joystick has (so we don't error out setting
		// properties for unavailable axis)
		mdiDevCaps.dwSize = sizeof(DIDEVCAPS);
		hr = mpJoystick2->GetCapabilities(&mdiDevCaps);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			return hr;
		}
		
		memset((void*)&diprg, 0, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(DIPROPRANGE); 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= -1000; 
		diprg.lMax				= +1000; 

		diprg.diph.dwObj = DIJOFS_X;	// set the x-axis range
		hr = mpJoystick2->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			return hr;
		}

		diprg.diph.dwObj = DIJOFS_Y;	// set the y-axis range
		hr = mpJoystick2->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr)) 
		{
			PrintDXError(hr);
			return hr;
		}

		if (mdiDevCaps.dwAxes >= 3)
		{
			diprg.diph.dwObj = DIJOFS_Z;	// set the z-axis range
			hr = mpJoystick2->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (FAILED(hr)) 
			{
				PrintDXError(hr);
			//	return hr;
			}
		}
		if (mdiDevCaps.dwAxes >= 4)
		{
			diprg.diph.dwObj = DIJOFS_RZ;	// set the rudder range
			hr = mpJoystick2->SetProperty(DIPROP_RANGE, &diprg.diph);
			if (FAILED(hr)) 
			{
				PrintDXError(hr);
			//	return hr;
			}
		}
	}
	PrintDXError(ret);
	return ret;
}

//======================================================-==--=-- --  -
BOOL CALLBACK LTDInput::EnumJoysticksCallback(	LPCDIDEVICEINSTANCE pInst, 
												LPVOID				lpvContext )
{
	HRESULT 			hr;
	LPDIRECTINPUTDEVICE pDevice;
	LTDInput*			sneaky_this;

	sneaky_this = (LTDInput*)lpvContext;	// because callback can't have a real this
	if (!sneaky_this) return DIENUM_STOP;	// pointer, we use this


	// If we have (re) enumerated one of the selected joysticks...

	if(		(	(WorkingController[0])
			&&	(!strcmp(pInst->tszInstanceName, WorkingController[0]->Name))
			&&	(pInst->guidInstance == WorkingController[0]->Guid)
			)
		||	(	(WorkingController[1])
			&&	(!strcmp(pInst->tszInstanceName, WorkingController[1]->Name))
			&&	(pInst->guidInstance == WorkingController[1]->Guid)
			)
		)
	{
		// obtain an interface to the enumerated force feedback joystick.
		hr = sneaky_this->mpDI->CreateDevice(pInst->guidInstance, &pDevice, NULL);

		// if it failed, then we can't use this joystick for some
		// bizarre reason.	(Maybe the user unplugged it while we
		// were in the middle of enumerating it.)  So continue enumerating
		if (FAILED(hr)) 
			return DIENUM_CONTINUE;

		// we successfully created an IDirectInputDevice.  So stop looking 
		// for another one.
		if (pInst->guidInstance == WorkingController[0]->Guid)
		{
			// Joystick one
			sneaky_this->mpJoystick1 = pDevice;

			// query for IDirectInputDevice2 - we need this to poll the joystick 
			pDevice->QueryInterface(IID_IDirectInputDevice2, 
									(LPVOID*)&sneaky_this->mpJoystick1Device2);
			return DIENUM_CONTINUE;
		}
		else
		{
			// Joystick two
			sneaky_this->mpJoystick2 = pDevice;

			// query for IDirectInputDevice2 - we need this to poll the joystick 
			pDevice->QueryInterface(IID_IDirectInputDevice2, 
								(LPVOID*)&sneaky_this->mpJoystick2Device2);
			
			return DIENUM_STOP;
		}
	}
	else
	{
		return DIENUM_CONTINUE;
	}
	
}


//======================================================-==--=-- --  -
HRESULT LTDInput::SetAcquire( HWND hDlg )
{
	if (mpJoystick1)
	{
		if (mbActive) 
		{
			// acquire the input device 
			return mpJoystick1->Acquire();
		} 
		else 
		{
			// unacquire the input device 
			return mpJoystick1->Unacquire();
		}
	}

	if (mpJoystick2)
	{
		if (mbActive) 
		{
			// acquire the input device 
			return mpJoystick2->Acquire();
		} 
		else 
		{
			// unacquire the input device 
			return mpJoystick2->Unacquire();
		}
	}

	if (mpKeyboard)
	{
		if (mbActive) 
		{
			// acquire the input device 
			return mpKeyboard->Acquire();
		} 
		else 
		{
			// unacquire the input device 
			return mpKeyboard->Unacquire();
		}
	}

	if (mpMouse)
	{
		if (mbActive) 
		{
			// acquire the input device 
			return mpMouse->Acquire();
		} 
		else 
		{
			// unacquire the input device 
			return mpMouse->Unacquire();
		}
	}

	return S_FALSE;
}


//======================================================-==--=-- --  -
HRESULT LTDInput::UpdateInputState( HWND hDlg)
{
	HRESULT 	hr = 0;

	if(mpMouse) 
	{
		// Get the input's device state, and put the state in dims
		ZeroMemory(&mMS, sizeof(mMS));
		hr = mpMouse->GetDeviceState(sizeof(mMS), &mMS);
		if (FAILED(hr)) 
		{
			memset((void*)&mMS, 0, sizeof(mMS));

			// DirectInput may be telling us that the input stream has been
			// interrupted.  We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done.
			// We just re-acquire and try again.
		
			// If input is lost then acquire and keep trying 
			hr = mpMouse->Acquire();
			while (hr == DIERR_INPUTLOST) 
				hr = mpMouse->Acquire();

			if (	(hr == DIERR_OTHERAPPHASPRIO) 
				||	(hr == DIERR_NOTACQUIRED)
				)
			{
				OutputDebugString("DirectInput: Mouse: Failed to acquire device - is the app being minimised?\n");
				PrintDXError(hr);
			}
		}
	}
	
	if(mpKeyboard) 
	{
		// Get the input's device state, and put the state in dims
		memset((void*)&mKS, 0, sizeof(mKS));
		for (int i = 256; i--;)
		{
			if (mKS[i])
			{
				OutputDebugString("Error: Keyboard state memory is not clear!!!\n");
			}
		}

		hr = mpKeyboard->GetDeviceState(sizeof(mKS), &mKS);
		if (FAILED(hr)) 
		{
			memset((void*)&mKS, 0, sizeof(mKS));

			// DirectInput may be telling us that the input stream has been
			// interrupted.  We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done.
			// We just re-acquire and try again.
		
			// If input is lost then acquire and keep trying 
			hr = mpKeyboard->Acquire();
			while (hr == DIERR_INPUTLOST) 
				hr = mpKeyboard->Acquire();

			if (	(hr == DIERR_OTHERAPPHASPRIO) 
				||	(hr == DIERR_NOTACQUIRED)
				)
			{
				OutputDebugString("DirectInput: Keyboard: Failed to acquire device - is the app being minimised?\n");
				PrintDXError(hr);
			}
		}
	}

	if (mpJoystick1) 
	{
		hr = DIERR_INPUTLOST;
		memset((void*)&mJS1, 0, sizeof(mJS1)); 

		// if input is lost then acquire and keep trying 
		while (DIERR_INPUTLOST == hr) 
		{
			// poll the joystick to read the current state
			hr = mpJoystick1Device2->Poll();
			if (FAILED(hr))
			{
				OutputDebugString("Joystick1, polling FAILURE!\n");
				PrintDXError(hr);
				mpJoystick1Device2->Acquire();
				
				memset((void*)&mJS1, 0, sizeof(mJS1)); 
				goto doJoystick2;
			}
		
			// get the input's device state, and put the state in dims
			hr = mpJoystick1->GetDeviceState(sizeof(mJS1), &mJS1);
			if (FAILED(hr))	
				memset((void*)&mJS1, 0, sizeof(mJS1));
			if (hr == DIERR_INPUTLOST)
			{
				// DirectInput is telling us that the input stream has
				// been interrupted.  We aren't tracking any state
				// between polls, so we don't have any special reset
				// that needs to be done.  We just re-acquire and
				// try again.
				OutputDebugString("Joystick1, state FAILURE!\n");
				PrintDXError(hr);
				hr = mpJoystick1->Acquire();
				goto doJoystick2;
			}
		}
	} 
	
	// (If) joystick one failed it doesn't mean that 
	// we have to give up entirely. Thus poll device two.
doJoystick2:

	if (mpJoystick2) 
	{
		hr = DIERR_INPUTLOST;
		memset((void*)&mJS2, 0, sizeof(mJS2)); 
		OutputDebugString("Joystick2...\n");

		// if input is lost then acquire and keep trying 
		while (DIERR_INPUTLOST == hr) 
		{
			// poll the joystick to read the current state
			hr = mpJoystick2Device2->Poll();
			OutputDebugString("Polling...");
			if (FAILED(hr))
			{
				OutputDebugString("FAILURE!\n");
				PrintDXError(hr);
				mpJoystick2Device2->Acquire();

				memset((void*)&mJS2, 0, sizeof(mJS2)); 
				break;
			}
			OutputDebugString("okay\nGetting device state...");

			// get the input's device state, and put the state in dims
			hr = mpJoystick2->GetDeviceState(sizeof(mJS2), &mJS2);
			if (FAILED(hr))	
				memset((void*)&mJS2, 0, sizeof(mJS2));
			if (hr == DIERR_INPUTLOST)
			{
				// DirectInput is telling us that the input stream has
				// been interrupted.  We aren't tracking any state
				// between polls, so we don't have any special reset
				// that needs to be done.  We just re-acquire and
				// try again.
				OutputDebugString("FAILURE!\n");
				PrintDXError(hr);
				hr = mpJoystick2->Acquire();
				break;
			}
			OutputDebugString("okay\n");
		}
		OutputDebugString("Done\n");
	} 

	if (FAILED(hr))
	{
		OutputDebugString("Some error occured while updating controller information\n");
	}
	return hr;
}

//======================================================-==--=-- --  -
HRESULT LTDInput::ShutDown()
{
	// Unacquire and release any DirectInputDevice objects.
	if (NULL != mpJoystick1) 
	{
		// Unacquire the device one last time just in case 
		// the app tried to exit while the device is still acquired.
		mpJoystick1->Unacquire();

		mpJoystick1->Release();
		mpJoystick1 = NULL;
	}

	if (NULL != mpJoystick1Device2)
	{
		mpJoystick1Device2->Release();
		mpJoystick1Device2 = NULL;
	}

	// Unacquire and release any DirectInputDevice objects.
	if (NULL != mpJoystick2) 
	{
		// Unacquire the device one last time just in case 
		// the app tried to exit while the device is still acquired.
		mpJoystick2->Unacquire();

		mpJoystick2->Release();
		mpJoystick2 = NULL;
	}

	if (NULL != mpJoystick2Device2)
	{
		mpJoystick2Device2->Release();
		mpJoystick2Device2 = NULL;
	}

	// Release keyboard
	if (mpKeyboard)
	{
		mpKeyboard->Unacquire();
		mpKeyboard->Release();
		mpKeyboard = NULL;
	}

	// Release mouse
	if (mpMouse)
	{
		mpMouse->Unacquire();
		mpMouse->Release();
		mpMouse = NULL;
	}

	// Release any DirectInput objects.
	if (NULL != mpDI) 
	{
		mpDI->Release();
		mpDI = NULL;
	}

	return S_OK;
}


#endif
