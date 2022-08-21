// $Header$

// $Log$
// Revision 1.18  2000-10-14 14:10:43+01  asd
// removed outputDebugStrings ("the ones with the explitives")
//
// Revision 1.17  2000-10-06 16:02:00+01  asd
// <>
//
// Revision 1.16  2000-10-04 14:35:04+01  asd
// removed fog code to jclengine
//
// Revision 1.15  2000-10-04 11:16:37+01  are
// Added a debug line.
//
// Revision 1.14  2000-10-04 10:56:41+01  asd
// <>
//
// Revision 1.13  2000-10-04 10:46:10+01  asd
// added fog effect function
//
// Revision 1.12  2000-10-03 17:35:08+01  are
// <>
//
// Revision 1.11  2000-10-03 16:50:24+01  asd
// Added code to support multiple resolution
//
// Revision 1.10  2000-09-25 15:19:14+01  are
// Uses display device selected by the user.
//
// Revision 1.9  2000-09-22 10:33:26+01  are
// Removed CD checking code - now moved to part of configuration process. Also commented out LoadAccelerators due to the fact it relied on an handle declared in the old resource.h file, which has been updated.
//
// Revision 1.8  2000-09-14 16:55:26+01  jjs
// Fixes lightning.
//
// Revision 1.7  2000-09-14 10:33:50+01  are
// Checks for CD at start-up
//
// Revision 1.6  2000-08-31 11:50:17+01  are
// Correct a bug that caused the program to activate when minimised.
//
// Revision 1.5  2000-08-23 17:37:41+01  are
// Automatically rechecks save games when (re)activated.
//
// Revision 1.4  2000-08-14 11:54:53+01  are
// Minor change to message handling
//
// Revision 1.3  2000-08-11 17:33:18+01  are
// Changes to facilitate the restore the primary, back, and z-buffers.
// Modified message processing so that it processes all the messages in one go, rather than only one per time the method was called. Also added code to make the program 'sleep' when it is not active. Other changes include making the code slightly more 'defensive'.
//
// Revision 1.2  2000-07-11 10:52:09+01  jjs
// First DirectX7 version.
//
// Revision 1.1  2000-07-11 10:49:32+01  jjs
// Pre DirectX7 version.
//

#include	"Common.h"

#if TARGET == PC

#include	"Globals.h"
#include	"Resource.h"
#include	"TTShell.h"
#include	"PCSaveRoutines.h"
#include	"CLIParams.h"
#include	"PCSetupQuery.h"


#define SCREEN_BITS 16
#define	WIN_CLASS_NAME	"ENGINEWND"



//======================================================-==--=-- --  -
HDC TTWindow::GetDC()
{
	HDC hdc;
	if (!mpDSSurface)
		return NULL;
	if (mpDSSurface->GetDC(&hdc)==DD_OK)
		return hdc;
	return NULL;
}
//======================================================-==--=-- --  -
void TTWindow::ReleaseDC(HDC hdc)
{
	mpDSSurface->ReleaseDC(hdc);
	mpDSSurface->Blt(NULL,mpDSSurface,NULL,DDBLT_WAIT,NULL);
}
//= Z-buffer enumeration callback ======================-==--=-- --  -
HRESULT WINAPI TTWindow::EnumZBufferCallback(DDPIXELFORMAT* pddpf, LPVOID pddpfDesired)
{
    if (pddpf->dwFlags == DDPF_ZBUFFER)
    {
        memcpy(pddpfDesired, pddpf, sizeof(DDPIXELFORMAT));

        // Return with D3DENUMRET_CANCEL to end the search.
		return D3DENUMRET_CANCEL;
    }

    // Return with D3DENUMRET_OK to continue the search.
    return D3DENUMRET_OK;
}


// Restore the display surfaces and z-buffer, and 
// re-attach to the D3D object. - You should call this 
// method if you detect the loss of any of the display 
// surfaces.
HRESULT TTWindow::RestoreD3D()
{
	HRESULT result;

	result = this->PrimaryRestore();
	if (FAILED(result)) return result;

	// Back buffer (should have already been restored 
	// if it was created as a true back buffer.
	if (mpDSSurface)
	{
		if (FAILED(mpDSSurface->IsLost()))
		{
			result = mpDSSurface->Restore();
		}
		if (FAILED(result)) return result;
	}
	else
	{
		return E_FAIL;
	}

	if (mpZSurface)
	{
		if (FAILED(mpZSurface->IsLost()))
		{
			result = mpZSurface->Restore();
		}
		if (FAILED(result)) return result;
	}
	else
	{
		return E_FAIL;
	}

	result = mpDSSurface->AddAttachedSurface(mpZSurface);
	if ((result != D3D_OK) &&
		(result != DDERR_SURFACEALREADYATTACHED))
		return result;

	result = PLATFORM.GetD3DDevice()->SetRenderTarget(mpDSSurface, 0);
	return result;

}

//======================================================-==--=-- --  -
HRESULT TTWindow::DSRestore()
{
	if (!GetSurface())
		return E_FAIL;
	if (!GetSurface()->IsLost())
		return DD_OK;

	int result = GetSurface()->Restore();
	if (result != DD_OK)
		return result;
	
	
	// and the ZBuffer
	if (!GetZSurface())
		return E_FAIL;
	if (!GetZSurface()->IsLost())
		return DD_OK;

	return GetZSurface()->Restore();
}
 //======================================================-==--=-- --  -
// We have to clear the z-buffer as well
int TTWindow::Clear(D3DCOLOR color)
{
	if (!DSGetViewport())
		return E_FAIL;
	D3DRECT rect = {0, 0, DSWidth(), DSHeight()};
	return GetDevice()->Clear(1UL, &rect, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, color, 1.0f, 0L);
}

// We have to clear the z-buffer as well
int TTWindow::DSClearZ(D3DCOLOR color)
{
	if (!DSGetViewport())
		return E_FAIL;
	D3DRECT rect = {0, 0, DSWidth(), DSHeight()};
	return GetDevice()->Clear(1UL, &rect, D3DCLEAR_ZBUFFER, color, 1.0f, 0L);
}
//======================================================-==--=-- --  -
HRESULT TTWindow::SetTexture(CTextureSurface * pTexture)
{
	// Make sure the device exists
	if (!GetDevice())
		return E_FAIL;

	// Check if the user is trying to set no texture
	if (!pTexture)
		return GetDevice()->SetTexture(0,NULL);
	else
	{
		// We have a valid texture. Set it
		int result = GetDevice()->SetTexture(0,pTexture->GetSurface());
		return result;
	}
}

//####!!!!
//****************************************************************************************8
#define	MAX_DDRAW_DEVICES 30

SINT	dde_num_guids;
GUID	dde_guids[30];

BOOL	WINAPI	DDrawEnumCallback( GUID* guid , LPSTR desc , LPSTR name , void* param )
{
	if ( guid != NULL )
		dde_guids[dde_num_guids] = *guid;
	else
		dde_guids[dde_num_guids] = GUID_NULL;

	dde_num_guids++;

	if ( dde_num_guids >= MAX_DDRAW_DEVICES )
		return DDENUMRET_CANCEL;
	else
		return DDENUMRET_OK;
}

    //======================================================-==--=-- --  -
   //
  // TTWindow
 //
//======================================================-==--=-- --  -
LPDIRECTDRAWSURFACE7 TTWindow::GetPrimarySurface() 
{
	// Make this inline later.
	return this ? mpPrimarySurface : NULL;
}




HRESULT TTWindow::PrimaryRestore()
{
	if (!GetPrimarySurface())
		return E_FAIL;
	if (!GetPrimarySurface()->IsLost())
		return DD_OK;
	return GetPrimarySurface()->Restore();
}

//======================================================-==--=-- --  -
TTWindow::TTWindow()
{
// from CDrawPrimWindow's constructor
	m_pDD7 = NULL;
// from CWindow's
	m_hWnd = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_pHandler = NULL;

// surface stuff

	mpPrimarySurface = NULL;

// device surface

	mpDSDevice = NULL;
	memset(&mpDSViewport[0],0,sizeof(D3DVIEWPORT7));
	memset(&mpDSViewport[1],0,sizeof(D3DVIEWPORT7));
	memset(&mpDSViewport[2],0,sizeof(D3DVIEWPORT7));
	mpDSSurface = NULL;
	mpZSurface = NULL;
}
//======================================================-==--=-- --  -
LRESULT CALLBACK TTWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Find out which class is handling these messages
	TTWindow * pMe = (TTWindow *)GetWindowLong(hWnd, 0);

	// Call that classes message handler
	if (pMe)
		return pMe->GetHandler()->MessageProc(hWnd, uMsg, wParam, lParam);

	// If I didn't find the this pointer then just return the default value
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//======================================================-==--=-- --  -
BOOL TTWindow::InitDirectX(GUID * pDeviceGuid, const BOOL bFullScreen)
{
	//JCL - enumerate direct draw objects..
	/*if ( DirectDrawEnumerate( DDrawEnumCallback , NULL ) != DD_OK )
	{
		TRACE( "Unable to enumerate D3D devices\n" );
		return FALSE;
	}
	// Create the DirectDraw object
	// JCL - use the last enumerated device...

//	if (DirectDrawCreate(&dde_guids[dde_num_guids - 1], &m_pDD1, NULL) != DD_OK)
// bollox - it's lots of work to get 3dfx working....
	int dev_no;
	if(bFullScreen)	dev_no = dde_num_guids - 1;
	else dev_no = 0;

// to disable voodoo in fullscreen, uncomment the next line

	dev_no = 0;

*/
	if (DirectDrawCreateEx(	pDeviceGuid,
							(LPVOID *)&m_pDD7, 
							IID_IDirectDraw7, 
							NULL) != DD_OK)
		return FALSE;
	// Set the cooperative level. In debug mode we work in a window, in release mode we work full-screen.
	if (bFullScreen)
	{
		// Full-screen mode
		if (m_pDD7->SetCooperativeLevel(m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK)
			return FALSE;
		if (m_pDD7->SetDisplayMode(m_nWidth, m_nHeight, SCREEN_BITS, 0, 0) != DD_OK)
			return FALSE;
	}
	else
	{
		// Windowed mode
		if (m_pDD7->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL) != DD_OK)
			return FALSE;
	}
    DDSURFACEDESC2 ddsd;
	HRESULT ddrval;

    if(bFullScreen) //#################################################
	{
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;					//-
		ddsd.dwBackBufferCount = 1;											//-
		ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE |
			DDSCAPS_COMPLEX | DDSCAPS_FLIP | DDSCAPS_VIDEOMEMORY ;			//-
		m_pDD7->CreateSurface(&ddsd, &mpPrimarySurface, NULL);
		ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
		if((ddrval = mpPrimarySurface->GetAttachedSurface( &ddsd.ddsCaps, &mpDSSurface)) != DD_OK)
		{
			TRACE("Couldn't get attatched surface\n");
		}
		
		// If we succeeded then get the surface description
		ZeroMemory(&mDSSurfaceDesc, sizeof(mDSSurfaceDesc));
		mDSSurfaceDesc.dwSize = sizeof(mDSSurfaceDesc);
		if ((ddrval = mpDSSurface->GetSurfaceDesc(&mDSSurfaceDesc)) != DD_OK)
		{
			TRACE("Couldn't get a surface desciption\n");
		}
	}
	else //#############################################################
	{
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE;
		m_pDD7->CreateSurface(&ddsd, &mpPrimarySurface, NULL);

// this bit not in old fullscreen

		LPDIRECTDRAWCLIPPER pClipper;
		m_pDD7->CreateClipper(0, &pClipper, NULL);
		pClipper->SetHWnd(0, m_hWnd);
		mpPrimarySurface->SetClipper(pClipper);
		RELEASE(pClipper);
//
		ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		ddsd.dwFlags |= DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
		ddsd.dwFlags |= (DDSD_WIDTH | DDSD_HEIGHT);
		ddsd.dwWidth = m_nWidth;
		ddsd.dwHeight = m_nHeight;

		// Create the back buffer, Make sure everything I need exists
		if (!m_pDD7)
			return E_FAIL;

		// Try and create the actual surface
		if ((ddrval = m_pDD7->CreateSurface(&ddsd, &mpDSSurface, NULL)) != DD_OK)
		{
			TRACE("Couldn't create a surface\n");
		}
		// If we succeeded then get the surface description
		ZeroMemory(&mDSSurfaceDesc, sizeof(mDSSurfaceDesc));
		mDSSurfaceDesc.dwSize = sizeof(mDSSurfaceDesc);
		if ((ddrval = mpDSSurface->GetSurfaceDesc(&mDSSurfaceDesc)) != DD_OK)
		{
			TRACE("Couldn't get a surface desciption\n");
		}
	} //################################################################

	if (ddrval != DD_OK)
		return ddrval;
	// Get access to Direct3D object
	LPDIRECT3D7 pD3D = NULL;
    int result = m_pDD7->QueryInterface(IID_IDirect3D7, (LPVOID *)&pD3D);
    if (result != S_OK)
		return result;
	// Enumerate possible z-buffer formats
	ZeroMemory(&mZFormat, sizeof(mZFormat));
	pD3D->EnumZBufferFormats(IID_IDirect3DHALDevice, EnumZBufferCallback, (VOID*)&mZFormat);
	// If we found a good zbuffer format, then the dwSize field will be
	// properly set during enumeration. Else, we have a problem and will exit.
    if (sizeof(DDPIXELFORMAT) != mZFormat.dwSize)
        return E_FAIL;
	// Create the z-buffer
	// Set up the DDSURFACEDESC2 structure
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags |= (DDSD_CAPS | DDSD_PIXELFORMAT);
	ddsd.ddsCaps.dwCaps |= DDSCAPS_ZBUFFER;
	memcpy(&ddsd.ddpfPixelFormat, &mZFormat, sizeof(mZFormat));
	ddsd.dwFlags |= (DDSD_WIDTH | DDSD_HEIGHT);
	ddsd.dwWidth = DSWidth();
	ddsd.dwHeight = DSHeight();

	// Try and create the actual surface
	if ((ddrval = m_pDD7->CreateSurface(&ddsd, &mpZSurface, NULL)) != DD_OK)
	{
		TRACE("Couldn't create a surface\n");
		return ddrval;
	}

	// If we succeeded then get the surface description
	ZeroMemory(&mZSurfaceDesc, sizeof(mZSurfaceDesc));
	mZSurfaceDesc.dwSize = sizeof(mZSurfaceDesc);
	if ((ddrval = mpZSurface->GetSurfaceDesc(&mZSurfaceDesc)) != DD_OK)
	{
		TRACE("Couldn't get a surface desciption\n");
		return ddrval;
	}

	// Attach the z-buffer to me
	result = GetSurface()->AddAttachedSurface(GetZSurface());
	if (result != DD_OK)
	{
		TRACE("Couldn't attach z-buffer to the back buffer\n");
		return result;
	}

	// Create the device
	result = pD3D->CreateDevice(IID_IDirect3DHALDevice, GetSurface(), &mpDSDevice);
	if (result != D3D_OK)
	{
		TRACE("Couldn't create the device\n");
		return result;
	}

    // Set up the viewport data parameters
    D3DVIEWPORT7 vdData;

	// ********************
	// 0
    ZeroMemory(&vdData, sizeof(vdData));
	vdData.dwWidth      = DSWidth();
	vdData.dwHeight     = DSHeight();
    vdData.dvMaxZ       = 1.0f;

    // Create the viewport
    mpDSViewport[0] = vdData;

    // Associate the viewport with the D3DDEVICE object
    mpDSDevice->SetViewport(DSGetViewport(0));

	// ********************
	// 1
    ZeroMemory(&vdData, sizeof(vdData));
	vdData.dwWidth      = DSWidth() / 2;
	vdData.dwHeight     = DSHeight();
    vdData.dvMaxZ       = 1.0f;

    // Create the viewport
    mpDSViewport[1] = vdData;

	// ********************
	// 2
    ZeroMemory(&vdData, sizeof(vdData));
 	vdData.dwX = DSWidth() / 2;
	vdData.dwWidth      = DSWidth() / 2;
	vdData.dwHeight     = DSHeight();
    vdData.dvMaxZ       = 1.0f;

    // Create the viewport
    mpDSViewport[2] = vdData;

	pD3D->Release();

	TRACE("DirectX initialized correctly\n");

	return TRUE;
}
//======================================================-==--=-- --  -
/*
	The next method added by ASD.
	The game has different resolution settings but only for the actual levels.
	This method changes the reolution of the D3D buffer. The problem
	is that when you change the resolution you lose your primary surface
	so this has to be quick to avoid confusion.

*/
//********************************************************************
BOOL TTWindow::DeclareResolutionChange(DWORD theWidth, DWORD theHeight, DWORD theBBP)
{

			//I'm pretty sure that directx should delte the primary surface for me
			//but I want to make sure there are no mem leaks so I get rid of
			// all surfaces... start afresh as it were.
			if (mpDSDevice)
			{
				if (mpDSDevice->Release() > 0)
				{
					TRACE("*** Warning: device object ref count > 0 in CDeviceSurface::Destroy()\n");
				}
				mpDSDevice = NULL;
			}	
			
			if(mpPrimarySurface)
			{
				mpPrimarySurface->DeleteAttachedSurface( 0,	mpZSurface );
			}
			
			RELEASE(mpDSSurface);
			while(mpPrimarySurface->Release() > 0 )
			{
					
			}


			
			
			if (m_pDD7->SetDisplayMode(theWidth, theHeight, theBBP, 0, 0) != DD_OK)
			{
				
				OutputDebugString("couldn't set display mode");
				m_pDD7->SetDisplayMode(m_nWidth, m_nHeight, SCREEN_BITS, 0, 0);

			}
			
			m_pDD7->SetCooperativeLevel(m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
			
			//Now I have set the diplay mode I want to restor all my surfaces.
			// all the code here is copied from the init.
			DDSURFACEDESC2 ddsd;
			HRESULT ddrval;

			//#################################################
			
				ZeroMemory(&ddsd, sizeof(ddsd));
				ddsd.dwSize = sizeof(ddsd);
				ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;					//-
				ddsd.dwBackBufferCount = 1;											//-
				ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE |
					DDSCAPS_COMPLEX | DDSCAPS_FLIP | DDSCAPS_VIDEOMEMORY ;			//-
				m_pDD7->CreateSurface(&ddsd, &mpPrimarySurface, NULL);
				ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
				if((ddrval = mpPrimarySurface->GetAttachedSurface( &ddsd.ddsCaps, &mpDSSurface)) != DD_OK)
				{
					TRACE("Couldn't get attatched surface\n");
				}
				
				// If we succeeded then get the surface description
				ZeroMemory(&mDSSurfaceDesc, sizeof(mDSSurfaceDesc));
				mDSSurfaceDesc.dwSize = sizeof(mDSSurfaceDesc);
				if ((ddrval = mpDSSurface->GetSurfaceDesc(&mDSSurfaceDesc)) != DD_OK)
				{
					TRACE("Couldn't get a surface desciption\n");
				}
			
			
			//################################################################

			if (ddrval != DD_OK)
				return ddrval;
			// Get access to Direct3D object
			LPDIRECT3D7 pD3D = NULL;
			int result = m_pDD7->QueryInterface(IID_IDirect3D7, (LPVOID *)&pD3D);
			if (result != S_OK)
				return result;
			// Enumerate possible z-buffer formats
			ZeroMemory(&mZFormat, sizeof(mZFormat));
			pD3D->EnumZBufferFormats(IID_IDirect3DHALDevice, EnumZBufferCallback, (VOID*)&mZFormat);
			// If we found a good zbuffer format, then the dwSize field will be
			// properly set during enumeration. Else, we have a problem and will exit.
			if (sizeof(DDPIXELFORMAT) != mZFormat.dwSize)
				return E_FAIL;
			// Create the z-buffer
			// Set up the DDSURFACEDESC2 structure
			ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
			ddsd.dwSize = sizeof(DDSURFACEDESC2);
			ddsd.dwFlags |= (DDSD_CAPS | DDSD_PIXELFORMAT);
			ddsd.ddsCaps.dwCaps |= DDSCAPS_ZBUFFER;
			memcpy(&ddsd.ddpfPixelFormat, &mZFormat, sizeof(mZFormat));
			ddsd.dwFlags |= (DDSD_WIDTH | DDSD_HEIGHT);
			ddsd.dwWidth = DSWidth();
			ddsd.dwHeight = DSHeight();

			// Try and create the actual surface
			if ((ddrval = m_pDD7->CreateSurface(&ddsd, &mpZSurface, NULL)) != DD_OK)
			{
				TRACE("Couldn't create a surface\n");
				return ddrval;
			}

			// If we succeeded then get the surface description
			ZeroMemory(&mZSurfaceDesc, sizeof(mZSurfaceDesc));
			mZSurfaceDesc.dwSize = sizeof(mZSurfaceDesc);
			if ((ddrval = mpZSurface->GetSurfaceDesc(&mZSurfaceDesc)) != DD_OK)
			{
				TRACE("Couldn't get a surface desciption\n");
				return ddrval;
			}

			// Attach the z-buffer to me
			result = GetSurface()->AddAttachedSurface(GetZSurface());
			if (result != DD_OK)
			{
				TRACE("Couldn't attach z-buffer to the back buffer\n");
				return result;
			}

			// Create the device
			result = pD3D->CreateDevice(IID_IDirect3DHALDevice, GetSurface(), &mpDSDevice);
			if (result != D3D_OK)
			{
				TRACE("Couldn't create the device\n");
				return result;
			}

			// Set up the viewport data parameters
			D3DVIEWPORT7 vdData;

			// ********************
			// 0
			ZeroMemory(&vdData, sizeof(vdData));
			vdData.dwWidth      = DSWidth();
			vdData.dwHeight     = DSHeight();
			vdData.dvMaxZ       = 1.0f;

			// Create the viewport
			mpDSViewport[0] = vdData;

			// Associate the viewport with the D3DDEVICE object
			mpDSDevice->SetViewport(DSGetViewport(0));

			// ********************
			// 1
			ZeroMemory(&vdData, sizeof(vdData));
			vdData.dwWidth      = DSWidth() / 2;
			vdData.dwHeight     = DSHeight();
			vdData.dvMaxZ       = 1.0f;

			// Create the viewport
			mpDSViewport[1] = vdData;

			// ********************
			// 2
			ZeroMemory(&vdData, sizeof(vdData));
 			vdData.dwX = DSWidth() / 2;
			vdData.dwWidth      = DSWidth() / 2;
			vdData.dwHeight     = DSHeight();
			vdData.dvMaxZ       = 1.0f;

			// Create the viewport
			mpDSViewport[2] = vdData;
			
			pD3D->Release();

	OutputDebugString("DirectX initialized correctly\n");

	return TRUE;

}
//********************************************************************

//********************************************************************
void TTWindow::CleanUpDirectX()
{
	if (mpDSDevice)
	{
		if (mpDSDevice->Release() > 0)
		{
			TRACE("*** Warning: device object ref count > 0 in CDeviceSurface::Destroy()\n");
		}
		mpDSDevice = NULL;
	}

	if ((mpPrimarySurface) && (mpZSurface))
	{
		mpPrimarySurface->DeleteAttachedSurface( 0,	mpZSurface );
		mpZSurface = NULL;
	}
		
	RELEASE(mpDSSurface);

	if (mpPrimarySurface)
	{
		while(mpPrimarySurface->Release() > 0 )
		{
				
		}
		mpPrimarySurface = NULL;
	}

	RELEASE(m_pDD7);
}
//======================================================-==--=-- --  -
BOOL TTWindow::Init(	CMessageHandler * pHandler,
						const HINSTANCE hInstance,
						const int nWidth,
						const int nHeight,
						const BOOL bFullScreen)
{
	BOOL result;

	// Initialize the main window
	if (bFullScreen)
		result = WinInit(pHandler /*? pHandler : this*/,
			hInstance, nWidth, nHeight,	WS_POPUP, SW_SHOWNORMAL);
	else
		result = WinInit(pHandler /*? pHandler : this*/,
		hInstance, nWidth, nHeight,	WS_OVERLAPPEDWINDOW, SW_SHOWNORMAL);
	if (!result)
		return FALSE;

	// Set up DirectX, first Clean up anything we may have already created
	CleanUpDirectX();

	// Try to initialize with HAL
//	if (InitDirectX(&IID_IDirect3DHALDevice, bFullScreen))
//	return TRUE;
	
	// That didn't work, so let's try the HEL
//	CleanUpDirectX();
	if (InitDirectX(&SelectedDisplayDevice, bFullScreen))
	{
		mDInput.ShutDown(); // in case it's already active
		mDInput.Init(hInstance,GetHwnd());
		mDInput.SetAcquire(GetHwnd());
		return TRUE;
	}

	// Nope, that didn't work either. Nothing else we can do really.
	return FALSE;
}
//======================================================-==--=-- --  -
BOOL TTWindow::WinInit(CMessageHandler * pHandler, HINSTANCE hInstance, const int nWidth, const int nHeight, const DWORD dwStyle, const int nShow)
{

	// from CWindow
	m_pHandler = pHandler /*? pHandler : this*/;

// register the window class

	WNDCLASS wndclass;

	wndclass.style = 0;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = sizeof(this);
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(0,IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = WIN_CLASS_NAME;
	RegisterClass(&wndclass);

// create the window

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_hWnd = CreateWindow(	WIN_CLASS_NAME,GAME_NAME,dwStyle, CW_USEDEFAULT, 0,
							nWidth + (2 * GetSystemMetrics(SM_CXSIZEFRAME)), 
							nHeight + (2 * GetSystemMetrics(SM_CYSIZEFRAME))
							+ GetSystemMetrics(SM_CYCAPTION),
							NULL,0,hInstance,0);
	if (m_hWnd)
	{
		// Set the first window long to 'this'
		SetWindowLong(m_hWnd, 0, (LONG)this);
		ShowWindow(m_hWnd,nShow);
	}

	return (m_hWnd != NULL);

}
//======================================================-==--=-- --  -
void TTWindow::ShutDown()
{
	mDInput.ShutDown();
	
	//(JCL) added 27/5/00
	CleanUpDirectX();
	DestroyWindow(m_hWnd);
}

    //======================================================-==--=-- --  -
   //
  // TTApp
 //
//======================================================-==--=-- --  -
TTApp::TTApp()
{
	m_hInstance = NULL;		// from CWinapp's constructor
	m_pMainWindow = NULL;	// 4 out of 5 levels deep!
	m_bActive = TRUE;
	m_hAccel = 0;
}
//======================================================-==--=-- --  -
BOOL	TTApp::Init(HINSTANCE hInstance, LPSTR cmd_line)
{
	// If the display card supports hardware cursors then the cursor will
	// be visible. Normally I don't like
	// visible cursors, so I hide the cursor here.
#if !defined(_DEBUG)
	ShowCursor(FALSE);
#endif
	{
		m_hInstance = hInstance;
		BOOL result;
		{
			m_pMainWindow = m_pDpWin = new TTWindow;
			if (!m_pDpWin)
			return FALSE;

#ifdef _DEBUG
	result = m_pDpWin->Init(this, m_hInstance, 640, 480, FALSE);
	mFullScreen = FALSE;
#else
	result = m_pDpWin->Init(this, m_hInstance, 640, 480, TRUE);
//	result = m_pDpWin->Init(this, m_hInstance, 320, 200, TRUE);
//	result = m_pDpWin->Init(this, m_hInstance, 640, 400, TRUE);
	mFullScreen = TRUE;
#endif

		}

		// Load the accelerator table
//		m_hAccel = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

		if(!result) return FALSE;
	}

	CCLIParams	params;

	CCLIParams::GetParams(cmd_line, params);

	return GAME.Init(this, params);
}
//======================================================-==--=-- --  -
void TTApp::Flip(TTWindow &surface)
{
	LPDIRECTDRAWSURFACE7	refSurface;
	int result;
	POINT p0 = {0,0};
	POINT p1 = {surface.DSWidth(), surface.DSHeight()};
	ClientToScreen(GetWindow()->GetHwnd(), &p0);
	ClientToScreen(GetWindow()->GetHwnd(), &p1);
	RECT rect = {p0.x, p0.y, p1.x, p1.y};

	refSurface = GetDrawPrimWindow()->GetPrimarySurface();
	if (refSurface)
	{
		result = refSurface->IsLost();
		if (SUCCEEDED(result))
		{
			if(mFullScreen)
			{
				result = refSurface->Flip(surface.GetSurface(), DDFLIP_WAIT);
			}
			else
			{
				result = refSurface->Blt(&rect, surface.GetSurface(), NULL, DDBLT_WAIT, NULL);
			}
		}
	
		if (FAILED(result))
		{
			GetDPWin()->RestoreD3D();
		}
	}
}
//======================================================-==--=-- --  -
void TTApp::ProcessMessages()
{
	BOOL bGotMsg;
	MSG msg;

	// This loop has two states. One for when the 
	// application is active and another for when the app 
	// has been deactivated. The first basically peeks for 
	// messages and dispatches them to the WinProc function
	// for processing. The second state involves using the 
	// GetMessage function which blocks until the function 
	// can return a message. The loop can not be exited
	// when it is in the second state, unless a WM_QUIT 
	// message is received. The second state is a wait 
	// state, for when the program has been deactivated. 
	// The loop changes between states when it processes a 
	// WM_MAIN_THREAD_ACTIVATE message. This is 
	// effectively a custom name for the WM_ACTIVATE 
	// message typically sent directly to the WinProc 
	// function. It would be safe to process the message 
	// using the DispatchMessage function, however, there 
	// is no point doing so.
	bGotMsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	while (bGotMsg != 0)
	{
		// Activate and deactivate the application - should stop 
		// the program running when it is deactivated.
		if (msg.message == WM_MAIN_THREAD_ACTIVATE)
		{
			m_bActive = (	(LOWORD(msg.wParam) != WA_INACTIVE) && 
							(!HIWORD(msg.wParam))	// Minimised!
						); 
			if (m_bActive)
			{
				OutputDebugString("Activation message. \n");

				// Regenerate save file list, just in case 
				// the user changed the files in the save 
				// directory whilest we were deactivated.
				PCRefreshFileList();	
			}
			else
			{
				OutputDebugString("Deactivation message.\n");
			}
		}
		else
		{
			// Translate and dispatch the message
			if (!(m_pMainWindow && TranslateAccelerator(m_pMainWindow->GetHwnd(), m_hAccel, &msg)))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// Get next message. 
		if (m_bActive)
		{
			bGotMsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		}
		else
		{
			bGotMsg = GetMessage(&msg, NULL, 0, 0);
			if (!bGotMsg) // i.e. the message was WM_QUIT.
			{
				GAME.SetQuitFlag(GQ_QUIT);
			}
		}
    }
}
//======================================================-==--=-- --  -
int TTApp::Run()
{
	GAME.Run();
	return 1;
}
//======================================================-==--=-- --  -
void	TTApp::ShutDown()
{
	GAME.Shutdown();
	if (m_pMainWindow)
	{
		m_pMainWindow->ShutDown();
		delete m_pMainWindow;
		m_pMainWindow = NULL;
	}
#if !defined(_DEBUG)
	ShowCursor(TRUE);
#endif
}

#endif
