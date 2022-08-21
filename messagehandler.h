// MessageHandler.h: interface for the CMessageHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGEHANDLER_H__3DC41AA1_4273_11D2_BC4A_00A0C9A33ADF__INCLUDED_)
#define AFX_MESSAGEHANDLER_H__3DC41AA1_4273_11D2_BC4A_00A0C9A33ADF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Define a custom message.
#ifndef WM_MAIN_THREAD_ACTIVATE
#define WM_MAIN_THREAD_ACTIVATE (WM_USER+0)
#endif

class CMessageHandler  
{
public:
	CMessageHandler();
	virtual ~CMessageHandler();

	// This function dispatches messages to the appropriate functions below
	virtual LRESULT MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Window message handlers
	virtual LRESULT OnPaint(HWND hWnd);
	virtual LRESULT OnCreate(HWND hWnd);
	virtual LRESULT OnDestroy(HWND hWnd);
	virtual LRESULT OnClose(HWND hWnd);
	virtual LRESULT OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnEraseBkgnd(HWND hWnd, HDC hDC);
	virtual LRESULT OnGetMinMaxInfo(HWND hWnd, MINMAXINFO FAR* lpMMI);
	// End if window message handlers
};

#endif // !defined(AFX_MESSAGEHANDLER_H__3DC41AA1_4273_11D2_BC4A_00A0C9A33ADF__INCLUDED_)
