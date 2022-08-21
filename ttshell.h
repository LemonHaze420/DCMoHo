// $Header$

// $Log$
// Revision 1.10  2000-10-04 10:46:24+01  asd
// added fog effect function
//
// Revision 1.9  2000-10-03 17:34:28+01  are
// <>
//
// Revision 1.8  2000-10-03 16:50:43+01  asd
// added code to support multiple resolution
//
// Revision 1.7  2000-09-14 11:54:11+01  jjs
// Uses Dx8 if available.
//
// Revision 1.6  2000-09-12 13:13:28+01  asd
// Changed library links to work with directx 7
//
// Revision 1.5  2000-09-11 11:24:26+01  are
// Links against DSound and DErr8
//
// Revision 1.4  2000-09-01 17:15:19+01  are
// DirectX8
//
// Revision 1.3  2000-08-11 17:34:16+01  are
// Added method definition. Use DirectX7!!!
//
// Revision 1.2  2000-07-11 10:53:39+01  jjs
// DirectX7 version.
//
// Revision 1.1  2000-07-11 10:52:55+01  jjs
// Pre DirectX7 version.
//

#ifndef TTSHELL_H
#define TTSHELL_H
#include "messagehandler.h"
#include "textureSurface.h"
#include "ltdinput.h"

#pragma comment(lib, "ddraw.lib")
#if defined(USEDX8)
#pragma comment(lib, "d3d8.lib")
#else
#pragma comment(lib, "d3dim.lib")
#endif
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
//#pragma comment(lib, "dxerr7.lib")

    //======================================================-==--=-- --  -
   //
  // TTWindow
 //
//======================================================-==--=-- --  -
class	TTWindow : virtual public CMessageHandler
{
// devicesurface

public:
	inline LPDIRECTDRAWSURFACE7 GetZSurface() {return this ? mpZSurface : NULL;}

protected:
	DDPIXELFORMAT			mZFormat;
	LPDIRECTDRAWSURFACE7	mpZSurface;
	DDSURFACEDESC2			mZSurfaceDesc;

	LTDInput				mDInput;

//######

public:
	inline	LTDInput*	GetDInput() { return &mDInput;};
	inline	DWORD DSWidth() {return mDSSurfaceDesc.dwWidth;}
	inline	DWORD DSHeight() {return mDSSurfaceDesc.dwHeight;}
	inline	LPDIRECTDRAWSURFACE7 GetSurface() {return this ? mpDSSurface : NULL;} //####!!!!

	// Restore the display surfaces and z-buffer, and 
	// re-attach to the D3D object. - You should call this 
	// method if you detect the loss of any of the display 
	// surfaces.
	HRESULT RestoreD3D();
	
	//
	inline	LPDIRECT3DDEVICE7 GetDevice() {return mpDSDevice;} //####!!!!
	inline	LPD3DVIEWPORT7 DSGetViewport(int n = 0) {return &mpDSViewport[n];}

	HDC		GetDC();				//####!!!!
	void	ReleaseDC(HDC hdc);		//####!!!!
	HRESULT DSRestore();
	int		Clear(D3DCOLOR color=0);//####!!!!
	int		DSClearZ(D3DCOLOR color=0);
	HRESULT	SetTexture(CTextureSurface * pTexture = NULL);

protected:
	static HRESULT WINAPI EnumZBufferCallback(DDPIXELFORMAT* pddpf, LPVOID pddpfDesired);
	LPDIRECT3DDEVICE7 mpDSDevice;
	D3DVIEWPORT7 mpDSViewport[3];
	LPDIRECTDRAWSURFACE7 mpDSSurface;
	DDSURFACEDESC2 mDSSurfaceDesc;


public:
// primary surface stuff

	LPDIRECTDRAWSURFACE7 mpPrimarySurface;
	LPDIRECTDRAWSURFACE7 GetPrimarySurface(); // { return this ? mpPrimarySurface : NULL;} 

	HRESULT PrimaryRestore();
//
	TTWindow();
	~TTWindow() { m_pHandler = NULL; }
	BOOL Init(CMessageHandler * pHandler, const HINSTANCE hInstance, const int nWidth, const int nHeight, const BOOL bFullScreen);
	void	ShutDown();

	HWND GetHwnd() {return m_hWnd;}
	LPDIRECTDRAW7 GetDirectDraw7() {return m_pDD7;}

	BOOL DeclareResolutionChange(DWORD theWidth, DWORD theHeight, DWORD theBBP); //ASD added to faciltate resolution changes.
	

protected:
	LPDIRECTDRAW7 m_pDD7;
	CMessageHandler * m_pHandler;
	int m_nWidth, m_nHeight;
	HWND m_hWnd;

	BOOL WinInit(CMessageHandler * pHandler, const HINSTANCE hInstance, const int nWidth=CW_USEDEFAULT, const int nHeight=CW_USEDEFAULT, const DWORD dwStyle=WS_OVERLAPPEDWINDOW, const int nShow=SW_SHOWNORMAL);
	BOOL InitDirectX(GUID * pDeviceGuid, const BOOL bFullScreen);
	void CleanUpDirectX();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	inline CMessageHandler * GetHandler() {return m_pHandler;}
};
    //======================================================-==--=-- --  -
   //
  // TTApp
 //
//======================================================-==--=-- --  -
class	TTApp : virtual public CMessageHandler
{
public:
	TTApp();
	BOOL	Init(HINSTANCE hInstance, LPSTR cmd_line);
	int		Run();
	void	ShutDown();
	void Flip(TTWindow &surface);

	TTWindow	*GetDPWin() {return m_pDpWin;};
	void TTApp::ProcessMessages();
	inline TTWindow * GetWindow() {return this ? m_pMainWindow : NULL;}

protected:
	BOOL mFullScreen;
	TTWindow * m_pMainWindow;
	HACCEL m_hAccel;		
	BOOL m_bActive;			
	HINSTANCE m_hInstance;	
	TTWindow * m_pDpWin;
	inline TTWindow * GetDrawPrimWindow() {return this ? m_pDpWin : NULL;}
};
#endif