// $Header$

// $Log$
// Revision 1.5  2000-10-01 16:32:17+01  are
// Modified ScreenWidth, ScreenHeight methods to return 640 and 480, respectively, when in the frontend and return the selected in game width and height at all other times.
//
// Revision 1.4  2000-09-29 15:42:22+01  are
// Very minor code layout change.
//
// Revision 1.3  2000-09-15 16:36:29+01  are
// Now updates controllers irrespective of whether the calls to DirectInput succeeded. The data is all zero'ed in such an event.
//
// Revision 1.2  2000-09-07 17:06:48+01  are
// Only updates controller states if successfully obtained updated input from direct input.
//
// Revision 1.1  2000-07-11 10:32:59+01  jjs
// DirectX7 version.
//

#include	"Common.h"

#if TARGET == PC

#include	"ttshell.h"
#include	"PCPlatform.h"
#include	"Globals.h"

//******************************************************************************************

#define	CHECK_CAP(a, b)  \
if (!((a) & (b))) {char text[400]; sprintf(text,"Caps test failed: " #a "\n"); TRACE(text); retval = FALSE;}

BOOL	CPlatform::CheckCaps()
{
	// simple checking of caps to help me out.

	BOOL	retval = TRUE;

	DWORD	texture_blend_caps = mD3DDevDesc.dpcTriCaps.dwTextureBlendCaps;

	CHECK_CAP(texture_blend_caps, D3DPTBLENDCAPS_MODULATEALPHA );

//	ASSERT(retval);
	
//	return retval; 
	return TRUE; //! ...
}

//******************************************************************************************

BOOL	CPlatform::Init(TTApp *a)
{
	mApp = a;
	mKeyChangeCallback = NULL;
	mMouseWheelDelta = 0;

	InitVkeyToAscii();

	// get caps
	LPDIRECT3DDEVICE7	dev = GetD3DDevice();
	ASSERT(dev);

	dev->GetCaps(&mD3DDevDesc);

	TRACE("Caps checking passed successfully!\n");

	return CheckCaps();
};
//******************************************************************************************
void	CPlatform::ProcessMessages()
{
	PLATFORM.JoyUpdateInput();
	CONTROLS.Update();

	mApp->ProcessMessages();
}

//*************************************************************************************************
BOOL	CPlatform::IsKeyDown(UINT key)
{
	return (GetAsyncKeyState(key) < 0);
}

//*************************************************************************************************
BOOL	CPlatform::IsMouseButtonPressed(SINT button_no)
{
	return mButtons & (1 << button_no);
}

//*************************************************************************************************
POINT	CPlatform::GetMousePos()
{
	POINT	p;
	
	GetCursorPos(&p);
	ScreenToClient(PLATFORM.GetApp()->GetWindow()->GetHwnd(), &p);
	
	return p;
}

//*************************************************************************************************
HRESULT	CPlatform::OnChar(WPARAM wParam, LPARAM lParam)
{
	if (mKeyChangeCallback)
		return mKeyChangeCallback(wParam, lParam);

	return 0;
}

// ----
// Variable in game resolution on PC.
SINT	CPlatform::ScreenWidth()
{
#if TARGET == PC
	return GAME.mInFrontEnd?640: SelectedWidth;
#else
	return 640;
#endif
}

// Variable in game resolution on PC.
SINT	CPlatform::ViewWidth()
{
#if TARGET == PC
	return GAME.mInFrontEnd?640: (GAME.IsMultiPlayer() ? SelectedWidth / 2 : SelectedWidth);
#else
	return 640;
#endif
}

// ----
// Variable in game resolution on PC.
SINT	CPlatform::ScreenWidthFull()
{
#if TARGET == PC
	return GAME.mInFrontEnd?640:SelectedWidth;
#else
	return 640;
#endif
}

// ----
// Variable in game resolution on PC.
SINT	CPlatform::ScreenHeight()	
{
#if TARGET == PC
	return GAME.mInFrontEnd?480:SelectedHeight;
#else
	return 480;
#endif
}



//*************************************************************************************************
void	CPlatform::InitVkeyToAscii()
{
	memset(vkey_to_ascii, 0 ,sizeof(vkey_to_ascii));

	vkey_to_ascii[VK_F1]		= ASCII_F1;
	vkey_to_ascii[VK_F2]		= ASCII_F2;
	vkey_to_ascii[VK_F3]		= ASCII_F3;
	vkey_to_ascii[VK_F4]		= ASCII_F4;
	vkey_to_ascii[VK_F5]		= ASCII_F5;
	vkey_to_ascii[VK_F6]		= ASCII_F6;
	vkey_to_ascii[VK_F7]		= ASCII_F7;
	vkey_to_ascii[VK_F8]		= ASCII_F8;
	vkey_to_ascii[VK_F9]		= ASCII_F9;
	vkey_to_ascii[VK_F10]		= ASCII_F10;
	vkey_to_ascii[VK_F11]		= ASCII_F11;
	vkey_to_ascii[VK_F12]		= ASCII_F12;
	vkey_to_ascii[VK_HOME]		= ASCII_HOME;
	vkey_to_ascii[VK_LEFT]		= ASCII_LEFT;
	vkey_to_ascii[VK_RIGHT]		= ASCII_RIGHT;
	vkey_to_ascii[VK_UP]		= ASCII_UP;
	vkey_to_ascii[VK_DOWN]		= ASCII_DOWN;
	vkey_to_ascii[VK_END]		= ASCII_END;
	vkey_to_ascii[VK_NUMPAD0]	= ASCII_PAD0;
	vkey_to_ascii[VK_NUMPAD1]	= ASCII_PAD1;
	vkey_to_ascii[VK_NUMPAD2]	= ASCII_PAD2;
	vkey_to_ascii[VK_NUMPAD3]	= ASCII_PAD3;
	vkey_to_ascii[VK_NUMPAD4]	= ASCII_PAD4;
	vkey_to_ascii[VK_NUMPAD5]	= ASCII_PAD5;
	vkey_to_ascii[VK_NUMPAD6]	= ASCII_PAD6;
	vkey_to_ascii[VK_NUMPAD7]	= ASCII_PAD7;
	vkey_to_ascii[VK_NUMPAD8]	= ASCII_PAD8;
	vkey_to_ascii[VK_NUMPAD9]	= ASCII_PAD9;
	vkey_to_ascii[VK_PRIOR]		= ASCII_PGUP;
	vkey_to_ascii[VK_NEXT]		= ASCII_PGDN;
	vkey_to_ascii[VK_DELETE]	= ASCII_DEL;
	vkey_to_ascii[VK_INSERT]	= ASCII_INS;
}

#endif