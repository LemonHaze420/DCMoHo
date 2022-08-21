#include "Common.h"

// are: 
// Classes to collect and store enumerated DirectX device information.

#if TARGET == PC
#ifndef _PCQUERY_H_
#define _PCQUERY_H_

#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

// ----------------------------------------------------------------------------
// Store resolution, etc, data for a single video adapter device.
class CVideoDevice
{
	#define MAX_RESOLUTIONS				256
	#define MAX_LEN_VIDEO_DEVICE_NAME	256

	friend class CQueryPlatform;
public:
	CVideoDevice();
	~CVideoDevice();

	bool				AddResolution(LPDDSURFACEDESC2 _NewItem);	// Returns true if successfully added.

	int					GetNumberOfResolutions();
	LPDDSURFACEDESC2	GetResolution(int _Index);	// Returns null if _Index is out of bounds.

	// Sorts the resolutions into accending order. 
	void			SortResolutions();

	// ////////////////////////
	// INSTANCE VARIABLES
	GUID				Guid;
	char				Name[MAX_LEN_VIDEO_DEVICE_NAME];

protected:
	int					NumberOfResolutions;
	DDSURFACEDESC2		EnumeratedResolutions[MAX_RESOLUTIONS];
};

// ----------------------------------------------------------------------------
class CSoundDevice
{
	#define MAX_LEN_SOUND_DEVICE_NAME	256

	friend class CQueryPlatform;
public:
	CSoundDevice();
	~CSoundDevice();


	GUID	Guid;
	char	Name[MAX_LEN_SOUND_DEVICE_NAME];
	DSCAPS	Caps;
protected:

};

// ----------------------------------------------------------------------------
class CInputDevice
{
	friend class CQueryPlatform;
public:
	CInputDevice();
	~CInputDevice();

	DIDEVICEINSTANCE		Info;
	DIDEVCAPS				Caps;
	IDirectInputDevice*     Device;
	IDirectInputDevice2*    Device2;
protected:
};

// ----------------------------------------------------------------------------
// Get all data for the platform.
class CQueryPlatform
{
public:
	CQueryPlatform();
	~CQueryPlatform();

// ----
	// Returns false to indicate that a problem occured 
	// during the querying of the system - you should not 
	// rely on all the data to be accurate.
	static	bool Query(HINSTANCE _HInstance);

	// Creates all DirectInput devices for querying.
	static bool				CreateAllDirectInput(HWND _HWnd);

	// Release DirecInput resources.
	static void				ReleaseDirectInputDevices();

// ----
	// Methods to call once you have performed a query.

	// VIDEO
	static	int				GetNumberOfVideoDevices();
	static	CVideoDevice*	GetVideoDevice(int _Index);
	
	// SOUND
	static	int				GetNumberOfSoundDevices();
	static	CSoundDevice*	GetSoundDevice(int _Index);

	// INPUT
	static	int				GetNumberOfInputDevices();
	static	CInputDevice*	GetInputDevice(int _Index);

protected:

	// Callbacks.
	static BOOL		WINAPI		DDrawEnumCallback(GUID*	_Guid, LPSTR _Desc, LPSTR _Name, void* _Param);
	static HRESULT	WINAPI		EnumModesCallback2(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext);
	static BOOL		WINAPI		DSoundEnumCallback(LPGUID	lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext);
	static BOOL		WINAPI		DInputEnumCallback(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext);
		
	// GENERAL
	static	HWND			TestWindow;
	static	HINSTANCE		HInstance;
	static	LPDIRECTDRAW7	DDrawObject;
	static	LPDIRECTSOUND	DSoundObject;
	static	LPDIRECTINPUT7	DInputObject;

	// VIDEO
	#define					MAX_VIDEO_DEVICES	256
	static	int				NumberOfEnumeratedVideoDevices;
	static	CVideoDevice	VideoDeviceArray[MAX_VIDEO_DEVICES];

	// SOUND
	#define					MAX_SOUND_DEVICES	256
	static	int				NumberOfEnumeratedSoundDevices;
	static	CSoundDevice	SoundDeviceArray[MAX_SOUND_DEVICES];

	// INPUT
	#define					MAX_INPUT_DEVICES	256
	static	int				NumberOfEnumeratedInputDevices;
	static	CInputDevice	InputDeviceArray[MAX_INPUT_DEVICES];
};
#endif
#endif