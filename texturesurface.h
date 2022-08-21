// $Header$

// $Log$
// Revision 1.4  2000-09-27 11:50:23+01  jjs
// Fixed Managed Textures.
//
// Revision 1.3  2000-08-05 10:33:44+01  jjs
// First PC milestone.
//
// Revision 1.2  2000-07-11 10:46:29+01  jjs
// DirectX7 version.
//

    //======================================================-==--=-- --  -
   //
  // TextureSurface, made by combining CTextureSurface and CSurface
 //
//======================================================-==--=-- --  -
#ifndef TEXTURESURFACE_H
#define TEXTURESURFACE_H

#if TARGET == PC
class CImageLoader;

class CTextureSurface
{
public:
	void	qCopyUp();
	CTextureSurface();
	~CTextureSurface();

	inline	LPDIRECTDRAWSURFACE7 GetSurface() {return this ? m_pSurface : NULL;}

	int		PCreate(const LPDIRECTDRAW7 pDD, const int dwWidth, const int dwHeight);
	int		Create(const LPDIRECTDRAW7 pDD, const GUID * pDeviceGuid, const int dwWidth, const DDPIXELFORMAT* pPixelFormat=NULL);
	int		Create(const LPDIRECTDRAW7 pDD, const GUID * pDeviceGuid, CImageLoader & loader, const DDPIXELFORMAT* pPixelFormat=NULL, const DDPIXELFORMAT* pPixelFormatA=NULL, SINT *texw = NULL, SINT *texh = NULL);
	void	Destroy();
	HRESULT Lock(LPRECT lpDestRect,LPDDSURFACEDESC2 lpDDSurfaceDesc,DWORD dwFlags,HANDLE hEvent);
	HRESULT Unlock(LPVOID lpSurfaceData);
	HDC		GetDC();
	void	ReleaseDC(HDC hdc);

	// This class method replaces SetBits. 
	static	HRESULT WINAPI EnumSurfacesCallback2(	LPDIRECTDRAWSURFACE7	lpDDSurface, 
													LPDDSURFACEDESC2		lpDDSurfaceDesc,  
													LPVOID					lpContext);


protected:
	int		Create(const LPDIRECTDRAW7 pDD, const int dwWidth, const int dwHeight);
	BOOL	m_bDefPixelFormat;
	DDPIXELFORMAT m_PixelFormat;
	LPDIRECTDRAWSURFACE7 m_pSurface;
	LPDIRECTDRAW7 m_pDD;
	DDSURFACEDESC2 m_SurfaceDesc;
	const	GUID * m_pDeviceGuid;
};
#endif
#endif