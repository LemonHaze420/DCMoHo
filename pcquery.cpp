
#include "Common.h"

#if TARGET == PC

#include "pcquery.h"
#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

// ----------------------------------------------------------------------------
// Store resolution, etc, data for a single video adapter device.

// ----
CVideoDevice::CVideoDevice()
{
	this->NumberOfResolutions = 0;
}

// ----
// Free any referenced objects.
CVideoDevice::~CVideoDevice()
{
}

// ----
bool			CVideoDevice::AddResolution(LPDDSURFACEDESC2 _NewItem)
{
	if (this->NumberOfResolutions < MAX_RESOLUTIONS)
	{
		this->EnumeratedResolutions[this->NumberOfResolutions] = *_NewItem;
		this->NumberOfResolutions++;
		return true;
	}
	return false;
}

// ----
int				CVideoDevice::GetNumberOfResolutions() 
{
	return NumberOfResolutions;
}

// ----
LPDDSURFACEDESC2	CVideoDevice::GetResolution(int _Index)
{
	if (	(_Index >= 0) 
		||	(_Index < this->NumberOfResolutions)
		)
	{
		return &this->EnumeratedResolutions[_Index];
	}
	return NULL;
}

// ----
// compare function for use with qsorting resolutions.
int ResDecend(const void* _Left, const void* _Right)
{
	int ret = 0;
	LPDDSURFACEDESC2 Left, Right;
	Left	= (LPDDSURFACEDESC2)_Left;
	Right	= (LPDDSURFACEDESC2)_Right;

	if (Left->dwWidth < Right->dwWidth)
		ret += 100;
	if (Left->dwWidth > Right->dwWidth)
		ret -= 100;

	if (Left->dwHeight < Right->dwHeight)
		ret += 10;
	if (Left->dwHeight > Right->dwHeight)
		ret -= 10;

	if (Left->ddpfPixelFormat.dwRGBBitCount < Right->ddpfPixelFormat.dwRGBBitCount)
		ret += 1;
	if (Left->ddpfPixelFormat.dwRGBBitCount > Right->ddpfPixelFormat.dwRGBBitCount)
		ret -= 1;
	
	return ret;
}

// ----
// Sorts the resolutions into accending order. 
void	CVideoDevice::SortResolutions()
{
	qsort(	(void*)EnumeratedResolutions, 
			NumberOfResolutions, 
			sizeof(DDSURFACEDESC2), 
			ResDecend);
}

// ----------------------------------------------------------------------------
CSoundDevice::CSoundDevice()
{
	memset((void*)&Caps, 0, sizeof(Caps));
	Caps.dwSize = sizeof(DSCAPS);
}

// ----
CSoundDevice::~CSoundDevice()
{
	// Nothing
}

// ----------------------------------------------------------------------------
CInputDevice::CInputDevice()
{
	Device = NULL;
	Device2 = NULL;
	memset((void*)&Info, 0, sizeof(Info));
	memset((void*)&Caps, 0, sizeof(Caps));
}

CInputDevice::~CInputDevice()
{
	// Nothing.
}


// - CQueryPlatform -----------------------------------------------------------
// STATIC ITEMS
int				CQueryPlatform::NumberOfEnumeratedVideoDevices;
CVideoDevice	CQueryPlatform::VideoDeviceArray[MAX_VIDEO_DEVICES];
int				CQueryPlatform::NumberOfEnumeratedSoundDevices;
CSoundDevice	CQueryPlatform::SoundDeviceArray[MAX_SOUND_DEVICES];
int				CQueryPlatform::NumberOfEnumeratedInputDevices;
CInputDevice	CQueryPlatform::InputDeviceArray[MAX_INPUT_DEVICES];
HWND			CQueryPlatform::TestWindow;
HINSTANCE		CQueryPlatform::HInstance;
LPDIRECTDRAW7	CQueryPlatform::DDrawObject;
LPDIRECTSOUND	CQueryPlatform::DSoundObject;
LPDIRECTINPUT7	CQueryPlatform::DInputObject;

// ----------------------------------------------------------------------------
// Get all data for the platform.
CQueryPlatform::CQueryPlatform()
{
	TestWindow		= 0;
	HInstance		= 0;
	DDrawObject		= NULL;
	DSoundObject	= NULL;
	DInputObject	= NULL;

	NumberOfEnumeratedVideoDevices	= 0;
	NumberOfEnumeratedSoundDevices	= 0;
	NumberOfEnumeratedInputDevices	= 0;
}

CQueryPlatform::~CQueryPlatform()
{
}

// ----------------------------------------------------------------------------
// Enumerate screen modes for a particular device.
HRESULT WINAPI CQueryPlatform::EnumModesCallback2(LPDDSURFACEDESC2 lpDDSurfaceDesc,  LPVOID lpContext)
{
	if (VideoDeviceArray[NumberOfEnumeratedVideoDevices].AddResolution(lpDDSurfaceDesc))
	{
		return DDENUMRET_OK;
	}
	return DDENUMRET_CANCEL;
}

// ----
// Video devices
BOOL	WINAPI	CQueryPlatform::DDrawEnumCallback(GUID* _Guid, LPSTR _Desc, LPSTR _Name, void* _Param)
{
	if (NumberOfEnumeratedVideoDevices < MAX_VIDEO_DEVICES)
	{
		if (_Guid)
		{
			VideoDeviceArray[NumberOfEnumeratedVideoDevices].Guid	= *_Guid;
		}
		else
		{
			VideoDeviceArray[NumberOfEnumeratedVideoDevices].Guid	= GUID_NULL;
		}
		strcpy(VideoDeviceArray[NumberOfEnumeratedVideoDevices].Name, _Desc);

		// Enumerate available video modes.
		if (SUCCEEDED(DirectDrawCreateEx(	&VideoDeviceArray[NumberOfEnumeratedVideoDevices].Guid, 
											(LPVOID*)&DDrawObject,
											IID_IDirectDraw7,
											NULL
										)
						)
			)
		{
			DDrawObject->EnumDisplayModes(	NULL, 
											NULL,
											NULL, 
											EnumModesCallback2);
			DDrawObject->Release();
		}

		NumberOfEnumeratedVideoDevices++;
		return DDENUMRET_OK;
	}
	return DDENUMRET_CANCEL;
}

// ----------------------------------------------------------------------------
// Enumerate sound devices
BOOL CQueryPlatform::DSoundEnumCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
	if (NumberOfEnumeratedSoundDevices < MAX_SOUND_DEVICES)
	{
		if (lpGuid)
		{
			SoundDeviceArray[NumberOfEnumeratedSoundDevices].Guid	= *lpGuid;
		}
		else
		{
			SoundDeviceArray[NumberOfEnumeratedSoundDevices].Guid	= GUID_NULL;
		}
		strcpy(SoundDeviceArray[NumberOfEnumeratedSoundDevices].Name, lpcstrDescription);

		// Get the devices caps.
		if (SUCCEEDED(DirectSoundCreate(&SoundDeviceArray[NumberOfEnumeratedVideoDevices].Guid, 
										&DSoundObject,
										NULL
										)
						)
			)
		{
			DSoundObject->GetCaps(&SoundDeviceArray[NumberOfEnumeratedVideoDevices].Caps);
			DSoundObject->Release();
		}
		
		NumberOfEnumeratedSoundDevices++;
		return true;
	}

	// Stop enumeration process as we have run out of storage space.
	return false;
}

// ----------------------------------------------------------------------------
// Input device enumeration callback.
BOOL	WINAPI		CQueryPlatform::DInputEnumCallback(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
	LPDIRECTINPUTDEVICE	pDevice;
	DIPROPRANGE			diprg; 
	//DIDEVCAPS			mdiDevCaps;

	if (	(NumberOfEnumeratedInputDevices < MAX_INPUT_DEVICES) 
		&&	(pInst)
		)
	{
		OutputDebugString("Enumerated input device: ");
		OutputDebugString(pInst->tszProductName);
		OutputDebugString("\n");

		// Attempt to create device
		if (FAILED(DInputObject->CreateDevice(pInst->guidInstance, &pDevice, NULL))) 
		{
			OutputDebugString("FAILED TO CREATE DEVICE!!!\n");
			return DIENUM_CONTINUE;
		}

		InputDeviceArray[NumberOfEnumeratedInputDevices].Info = *pInst;
		InputDeviceArray[NumberOfEnumeratedInputDevices].Device = pDevice;

		switch (GET_DIDEVICE_TYPE(pInst->dwDevType))
		{
		case DIDEVTYPE_MOUSE:
			pDevice->SetDataFormat(&c_dfDIMouse2);
			pDevice->SetCooperativeLevel(	(HWND)lpvContext,
												DISCL_NONEXCLUSIVE
											|	DISCL_BACKGROUND);
			InputDeviceArray[NumberOfEnumeratedInputDevices].Caps.dwSize = sizeof(DIDEVCAPS);
			pDevice->GetCapabilities(&InputDeviceArray[NumberOfEnumeratedInputDevices].Caps);
			break;

		case DIDEVTYPE_KEYBOARD:
			pDevice->SetDataFormat(&c_dfDIKeyboard);
			pDevice->SetCooperativeLevel(	(HWND)lpvContext,
												DISCL_NONEXCLUSIVE
											|	DISCL_BACKGROUND);
			InputDeviceArray[NumberOfEnumeratedInputDevices].Caps.dwSize = sizeof(DIDEVCAPS);
			pDevice->GetCapabilities(&InputDeviceArray[NumberOfEnumeratedInputDevices].Caps);
			break;

		case DIDEVTYPE_JOYSTICK:
			pDevice->QueryInterface(IID_IDirectInputDevice2, 
									(LPVOID*)&InputDeviceArray[NumberOfEnumeratedInputDevices].Device2 );
	
			pDevice->SetDataFormat(&c_dfDIJoystick);
			pDevice->SetCooperativeLevel(	(HWND)lpvContext, 
											DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		
			// Determine how many axis the joystick has (so we don't 
			// error out setting properties for unavailable axis).
			InputDeviceArray[NumberOfEnumeratedInputDevices].Caps.dwSize = sizeof(DIDEVCAPS);
			if (SUCCEEDED(pDevice->GetCapabilities(&InputDeviceArray[NumberOfEnumeratedInputDevices].Caps))) 
			{
				memset((void*)&diprg, 0, sizeof(diprg));
				diprg.diph.dwSize		= sizeof(DIPROPRANGE); 
				diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
				diprg.diph.dwHow		= DIPH_BYOFFSET; 
				diprg.lMin				= -1000; 
				diprg.lMax				= +1000; 

				diprg.diph.dwObj = DIJOFS_X;	// set the x-axis range
				pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
				diprg.diph.dwObj = DIJOFS_Y;	// set the y-axis range
				pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
				
				if (InputDeviceArray[NumberOfEnumeratedInputDevices].Caps.dwAxes >= 3)
				{
					diprg.diph.dwObj = DIJOFS_Z;	// set the z-axis range
					pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
				}
		
				if (InputDeviceArray[NumberOfEnumeratedInputDevices].Caps.dwAxes >= 4)
				{
					diprg.diph.dwObj = DIJOFS_RZ;	// set the rudder range
					pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
				}
			}
			break;
		
		default:
			OutputDebugString("WARNING: Unknown device type - ignoring [won't show up in list of enumerated devices].\n");
			return DIENUM_CONTINUE;
			break;
		}
		
		NumberOfEnumeratedInputDevices++;
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

// ----
// Creates a DirectInput object and queries for ATTACHED devices.
bool				CQueryPlatform::CreateAllDirectInput(HWND _HWnd)
{
	// Clean up previous run.
	if (NumberOfEnumeratedInputDevices > 0)
	{
		ReleaseDirectInputDevices();
	}

	NumberOfEnumeratedInputDevices = 0;

	// Create DirectInput object
	if (SUCCEEDED(DirectInputCreateEx(HInstance, DIRECTINPUT_VERSION, IID_IDirectInput7, (void**)&DInputObject, NULL)))
	{
		// Enumerate and create devices.
		DInputObject->EnumDevices(	0,					// All device types.
									DInputEnumCallback,
									_HWnd, 
									DIEDFL_ATTACHEDONLY);
		return true;
	}
	return false;
}

// ----
// Release all DirectInput devices.
void				CQueryPlatform::ReleaseDirectInputDevices()
{
	if (DInputObject)
	{
		for (int i = NumberOfEnumeratedInputDevices; i--;)
		{
			if (InputDeviceArray[i].Device)
			{
				InputDeviceArray[i].Device->Release();
				InputDeviceArray[i].Device = NULL;
			}

			if (InputDeviceArray[i].Device2)
			{
				InputDeviceArray[i].Device2->Release();
				InputDeviceArray[i].Device2 = NULL;
			}
		}
		DInputObject->Release();
		DInputObject = NULL;
	}
}

// ----------------------------------------------------------------------------
// Returns false to indicate that a problem occured 
// during the querying of the system - you should not 
// rely on all the data to be accurate.
bool CQueryPlatform::Query(HINSTANCE _HInstance)
{
	OutputDebugString("Starting query process.\n");
	HInstance = _HInstance;
	bool	ret = true;

	// Query DirectDraw.	(Should generate a list of devices and associated video resolutions).
	ret &= SUCCEEDED(DirectDrawEnumerate(DDrawEnumCallback, NULL));

	// Sort video resolutions into accending order.
	for (int i = NumberOfEnumeratedVideoDevices; i--;)
	{
		VideoDeviceArray[i].SortResolutions();
	}

	// Query DirectSound.	(Should generate a list of devices and associated capabilities).
	ret &= SUCCEEDED(DirectSoundEnumerate(DSoundEnumCallback, NULL));

	OutputDebugString("Finished querying process.\n");
	return ret;
}

// ----------------------------------------------------------------------------
// Methods to call once you have performed a query.

// ----------------------------------------------------------------------------
// VIDEO
int				CQueryPlatform::GetNumberOfVideoDevices()
{
	return NumberOfEnumeratedVideoDevices;
}

// ----
CVideoDevice*	CQueryPlatform::GetVideoDevice(int _Index)
{
	if (	(_Index >= 0) 
		&&	(_Index < NumberOfEnumeratedVideoDevices)
		)
	{
		return &VideoDeviceArray[_Index];
	}
	return NULL;
}

// ----------------------------------------------------------------------------
// SOUND
int				CQueryPlatform::GetNumberOfSoundDevices() 
{
	return NumberOfEnumeratedSoundDevices;
}

// ----
CSoundDevice*	CQueryPlatform::GetSoundDevice(int _Index)
{
	if (	(_Index >= 0) 
		&&	(_Index < NumberOfEnumeratedSoundDevices)
		)
	{
		return &SoundDeviceArray[_Index];
	}
	return NULL;
}

// ----------------------------------------------------------------------------
// INPUT
int				CQueryPlatform::GetNumberOfInputDevices() 
{
	return NumberOfEnumeratedInputDevices;
}

// ----
CInputDevice*	CQueryPlatform::GetInputDevice(int _Index)
{
	if (	(_Index >= 0) 
		&&	(_Index < NumberOfEnumeratedInputDevices)
		)
	{
		return &InputDeviceArray[_Index];
	}
	return NULL;
}

#endif