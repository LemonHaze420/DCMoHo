// $Header$

// $Log$
// Revision 1.7  2000-10-03 17:34:51+01  are
// Methods to return scaling factors for in game res.
//
// Revision 1.6  2000-10-01 16:34:02+01  are
// Moved ScreenWidth implementations to cpp file to allow access to GAME object.
//
// Revision 1.5  2000-09-20 16:27:56+01  jjs
// More EnginePanel changes.
//
// Revision 1.4  2000-09-07 17:09:50+01  are
// DirectInput / multiple controllers
//
// Revision 1.3  2000-08-16 13:20:09+01  are
// Formally specified the calling convention in KeyChangeCallback typedef.
//
// Revision 1.2  2000-08-05 10:33:30+01  jjs
// First PC milestone.
//
// Revision 1.1  2000-07-11 10:33:58+01  jjs
// DirectX7 version.
//


#ifndef PCPLATFORM_H
#define PCPLATFORM_H

#include	"ttshell.h"
#include	"TextureSurface.h"

#define OT_BIT_DEPTH    10                  /* Bit length of order table */
#define	OT_LENGTH		(1<<OT_BIT_DEPTH)	/* Number of entries in order table. */

//******************************************************************************************
typedef	HRESULT	(__cdecl* KeyChangeCallback) ( UBYTE ascii , UINT keydata );

//******************************************************************************************

#define		CLEAR_STRUCT(a) {ZeroMemory((a), sizeof(*a)); (a)->dwSize = sizeof(*a);}

//******************************************************************************************
class	CPlatform
{
	friend class CController;
public:
	CPlatform()	{};
	~CPlatform() {};

//	BOOL			Init(CEngineApp *a);
	BOOL			Init(TTApp *a);
	void			Shutdown() {};
	void			InitVkeyToAscii();

	BOOL			IsKeyDown(UINT key);
	BOOL			IsMouseButtonPressed(SINT button_no);
	
	void				SetKeyChangeCallback(KeyChangeCallback c) {mKeyChangeCallback = c;};
	KeyChangeCallback	GetKeyChangeCallback() {return mKeyChangeCallback;}
	HRESULT				OnChar(WPARAM wParam, LPARAM lParam);
	HRESULT				OnLButtonDown(WPARAM wParam, LPARAM lParam) {mButtons |=  1; return 1;};
	HRESULT				OnLButtonUp(WPARAM wParam, LPARAM lParam)   {mButtons &= ~1; return 1;};
	HRESULT				OnRButtonDown(WPARAM wParam, LPARAM lParam) {mButtons |=  2; return 1;};
	HRESULT				OnRButtonUp(WPARAM wParam, LPARAM lParam)   {mButtons &= ~2; return 1;};

	HRESULT			OnMouseWheel(SINT	v) {mMouseWheelDelta += v; return 1;};

	SINT			GetMouseWheelData()
	{
		SINT i = mMouseWheelDelta;
		mMouseWheelDelta = 0;
		return i;
	}

	void		ProcessMessages();		// deal with windows message queue

	TTWindow	*GetBackSurface() { return GetDPWin();}; // very badly named function!!

	TTWindow	*GetDPWin() {return mApp->GetDPWin();};
	TTApp		*GetApp()   {return mApp;};
	LPDIRECT3DDEVICE7  GetD3DDevice() {return GetDPWin() ? GetDPWin()->GetDevice() :  NULL;};

	// Dinput stuff
//	HRESULT	JoySetAcquire()	{ TTWindow* ttw = GetDPWin(); return ttw->GetDInput()->SetAcquire(ttw->GetHwnd()); };
//	void	JoySetActive(bool tf) { GetDPWin()->GetDInput()->SetActive(tf); };
	HRESULT	JoyUpdateInput()	{ TTWindow* ttw = GetDPWin(); return ttw->GetDInput()->UpdateInputState(ttw->GetHwnd()); };
	DIJOYSTATE* JoyGetState(SINT n = 0) { return GetDPWin()->GetDInput()->GetJoyState(n);}
	// some globals

	SINT	ScreenWidth();		// are: Implementations moved to PCPlatform.cpp
	SINT	ViewWidth();
	SINT	ScreenWidthFull();
	SINT	ScreenHeight();

	float	ScreenWidthScale() {return ScreenWidth() / 640.0f;}		// Slow, I know, will add a means of performing and storing the calculation when the resolution is set.
	float	ScreenHeightScale() {return ScreenHeight() / 480.0f;}

	float	GetFOV()  {return 1.f;};  // in radians  (along x axis)
	float	GetFOVX() {return GetFOV();};
	float	GetFOVY() {return GetFOV() / ScreenWidth() * ScreenHeight();};

	// user control stuff
	BOOL	IsPrimaryFirePressed() {return IsMouseButtonPressed(0);};
	BOOL	IsSecondaryFirePressed() {return IsMouseButtonPressed(1);};

	UBYTE	vkey_to_ascii[256];

	// Mouse position is no longer protected as this method has no direct side effect.
	POINT			GetMousePos();

protected:	

//	CEngineApp	*mApp;
	TTApp *mApp;
	DWORD		mButtons;		// bit field  1 = down
	
	KeyChangeCallback	mKeyChangeCallback;

	// caps.
	BOOL	CheckCaps();

	D3DDEVICEDESC7	mD3DDevDesc;

	SINT	mMouseWheelDelta;
};

//******************************************************************************************
enum	AsciiKeys
{
	// Faked key codes
	ASCII_F1 = 131,
	ASCII_F2 = 132,
	ASCII_F3 = 133,
	ASCII_F4 = 134,
	ASCII_F5 = 135,
	ASCII_F6 = 136,
	ASCII_F7 = 137,
	ASCII_F8 = 138,
	ASCII_F9 = 139,
	ASCII_F10 = 140,
	ASCII_F11 = 141,
	ASCII_F12 = 142,
	ASCII_HOME = 150,
	ASCII_PGUP = 151,
	ASCII_PGDN = 152,
	ASCII_LEFT = 153,
	ASCII_RIGHT = 154,
	ASCII_UP = 155,
	ASCII_DOWN = 156,
	ASCII_INS = 157,
	ASCII_DEL = 158,
	ASCII_END = 159,

	ASCII_PAD0 = 160,
	ASCII_PAD1 = 161,
	ASCII_PAD2 = 162,
	ASCII_PAD3 = 163,
	ASCII_PAD4 = 164,
	ASCII_PAD5 = 165,
	ASCII_PAD6 = 166,
	ASCII_PAD7 = 167,
	ASCII_PAD8 = 168,
	ASCII_PAD9 = 169,

	ASCII_PRINT = 170,

	ASCII_TAB = 9,
	ASCII_BKSP = 8,
	ASCII_ESCAPE = 27,
};
//******************************************************************************************

#endif