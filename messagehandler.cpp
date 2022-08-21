// MessageHandler.cpp: implementation of the CMessageHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "Common.h"

#if TARGET == PC

#include "MessageHandler.h"
#include "resource.h"

#include	"Globals.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessageHandler::CMessageHandler()
{
}

CMessageHandler::~CMessageHandler()
{
}

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL WM_MOUSELAST+1
#endif

LRESULT CMessageHandler::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
			OnCommand(hWnd, wParam, lParam);
			break;

		case WM_CLOSE:
			return OnClose(hWnd);
			break;

		case WM_CHAR:
			//!JCL
			return PLATFORM.OnChar(wParam, lParam);
//			return OnChar(hWnd, wParam, lParam);
			break;
		case WM_KEYDOWN:
			if ((wParam >= 0) && (wParam <= 255 ))
			{
				UBYTE	ascii = PLATFORM.vkey_to_ascii[wParam];
				if ( ascii != 0 )
					PLATFORM.OnChar(ascii, lParam);
			}
			break;

		case WM_LBUTTONDOWN:
			return PLATFORM.OnLButtonDown(wParam, lParam);
			break;

		case WM_LBUTTONUP:
			return PLATFORM.OnLButtonUp(wParam, lParam);
			break;

		case WM_RBUTTONDOWN:
			return PLATFORM.OnRButtonDown(wParam, lParam);
			break;

		case WM_RBUTTONUP:
			return PLATFORM.OnRButtonUp(wParam, lParam);
			break;

		case WM_MOUSEWHEEL:
			{
				int	deltaZ = int(signed short(HIWORD( wParam )));
				return PLATFORM.OnMouseWheel(deltaZ);
				break;
			}


		case WM_DESTROY:
			return OnDestroy(hWnd);

		case WM_PAINT:
			return OnPaint(hWnd);

		case WM_ERASEBKGND:
		{
			HDC hDC = (HDC)wParam; // handle of device context 
			return OnEraseBkgnd(hWnd, hDC);
		}

		case WM_GETMINMAXINFO:
			return OnGetMinMaxInfo(hWnd, (LPMINMAXINFO)lParam);

		case WM_ACTIVATE:
			// Repost the message (giving it a custom 
			// name) so that the main thread gets a 
			// chance to process it. 
			PostMessage(hWnd, WM_MAIN_THREAD_ACTIVATE, wParam, lParam);
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);


/*       case WM_ACTIVATE:   // sent when window changes active state
            if ( WA_INACTIVE == wParam )
            {
                PLATFORM.JoySetActive(FALSE);
            }
            else
            {
                PLATFORM.JoySetActive(TRUE);
            }
			break;
			PLATFORM.JoySetAcquire();
*/	}

	// Message was processed
	return 0;
}


LRESULT CMessageHandler::OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd,&ps);
	EndPaint(hWnd,&ps);
	return 0;
}

LRESULT CMessageHandler::OnCreate(HWND hWnd)
{
	return 0;
}

LRESULT CMessageHandler::OnDestroy(HWND hWnd)
{
	return 0;
}

LRESULT CMessageHandler::OnClose(HWND hWnd)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT CMessageHandler::OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_CHAR, wParam, lParam);
}

LRESULT CMessageHandler::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
/*	switch (LOWORD(wParam))
	{
	case ID_EXIT:
		PostQuitMessage(0);
		break;
	}
*/
	return 0;
}

LRESULT CMessageHandler::OnEraseBkgnd(HWND hWnd, HDC hDC)
{
	return DefWindowProc(hWnd, WM_ERASEBKGND, (WPARAM)hDC, 0);
}

LRESULT CMessageHandler::OnGetMinMaxInfo(HWND hWnd, MINMAXINFO FAR* lpMMI)
{
	return DefWindowProc(hWnd, WM_GETMINMAXINFO, 0, (LPARAM)lpMMI);
}

#endif
