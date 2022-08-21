// $Header$

// $Log$
// Revision 1.2  2000-09-07 17:08:21+01  are
// Two joysticks, at once!!!
//
// Revision 1.1  2000-07-11 10:25:13+01  jjs
// Added define so that the new DX8 headers work.
//

#ifndef	LTDINPUT_H
#define	LTDINPUT_H

#define DIRECTINPUT_VERSION 0x0700

#include <dinput.h>
#pragma comment(lib, "dinput.lib")
//======================================================-==--=-- --  -
#define	LTDI_JOYSTICK	(1<<0)
//======================================================-==--=-- --  -
class LTDInput
{
	IDirectInput*           mpDI;
	IDirectInputDevice*     mpMouse;
	IDirectInputDevice*     mpKeyboard;
	IDirectInputDevice*     mpJoystick1;
	IDirectInputDevice2*    mpJoystick1Device2;
	IDirectInputDevice*     mpJoystick2;
	IDirectInputDevice2*    mpJoystick2Device2;
	HINSTANCE               mhInst;
	BOOL                    mbActive;
	DIDEVCAPS               mdiDevCaps;
	DIMOUSESTATE2			mMS;
	DIJOYSTATE				mJS1;
	DIJOYSTATE				mJS2;
	BYTE					mKS[256];	

	static BOOL CALLBACK	EnumJoysticksCallback(LPCDIDEVICEINSTANCE pInst, 
												LPVOID lpvContext );
public:
	LTDInput();
	~LTDInput();
	HRESULT					Init(HINSTANCE hInstance, HWND hDlg );
	HRESULT					ShutDown();

	HRESULT					SetAcquire( HWND hDlg );
	HRESULT					UpdateInputState( HWND hDlg);
	inline DIJOYSTATE*		GetJoyState(SINT n = 0) {if (n){return &mJS2;}else{return &mJS1;}}
	inline BYTE*			GetKeyState() { return mKS; };
	inline DIMOUSESTATE2*	GetMouseState() { return &mMS; };
	inline void				SetActive(BOOL tf) { mbActive = tf; };
};
#endif
