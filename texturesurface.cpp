// $Header$

// $Log$
// Revision 1.6  2000-10-06 15:09:01+01  are
// Enabled mip mapping. Replaced SetBits with EnumSurfacesCallback2 method. (Basically all the surfaces are enumerated and processed).
//
// Revision 1.5  2000-10-06 14:21:42+01  asd
// done stuff to it.
//
// Revision 1.4  2000-09-27 11:50:21+01  jjs
// Fixed Managed Textures.
//
// Revision 1.3  2000-08-11 17:35:30+01  are
// <>
//
// Revision 1.2  2000-08-05 10:33:42+01  jjs
// First PC milestone.
//
// Revision 1.1  2000-07-11 10:45:16+01  jjs
// First DirectX7 version.
//

    //======================================================-==--=-- --  -
   //
  // TextureSurface, made by combining CTextureSurface and CSurface
 //
//======================================================-==--=-- --  -
#include "Common.h"

#if TARGET == PC

#include "TextureSurface.h"
#include "ImageLoader.h"

//! hmmm..

#include	"Globals.h"

//======================================================-==--=-- --  -
CTextureSurface::CTextureSurface() : m_bDefPixelFormat(TRUE), m_pDeviceGuid(NULL),
									 m_pDD(NULL), m_pSurface(NULL)
{
}
CTextureSurface::~CTextureSurface()
{
//	this->Destroy();
}
//======================================================-==--=-- --  -
int CTextureSurface::PCreate(const LPDIRECTDRAW7 pDD, const int dwWidth, const int dwHeight)
{
	DDSURFACEDESC2 ddsd;

	m_pDD = pDD;

// Set up the DDSURFACEDESC2 structure
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags |= DDSD_CAPS | DDSD_MIPMAPCOUNT;
	ddsd.ddsCaps.dwCaps |= (DDSCAPS_TEXTURE | DDSCAPS_COMPLEX | DDSCAPS_MIPMAP);
	ddsd.ddsCaps.dwCaps2 |= (DDSCAPS2_TEXTUREMANAGE | DDSCAPS2_MIPMAPSUBLEVEL);
	ddsd.dwMipMapCount = 5;
	if (!m_bDefPixelFormat)
	{
		ddsd.dwFlags |= DDSD_PIXELFORMAT;
		memcpy(&ddsd.ddpfPixelFormat, &m_PixelFormat, sizeof(m_PixelFormat));
	}

	ddsd.dwFlags |= (DDSD_WIDTH | DDSD_HEIGHT);
	ddsd.dwWidth = dwWidth;
	ddsd.dwHeight = dwHeight;

// Create the surface as we normally would
	HRESULT ddrval;

// Make sure everything I need exists
	if (!m_pDD)
		ddrval = E_FAIL;

// Try and create the actual surface
	if ((ddrval = m_pDD->CreateSurface(&ddsd, &m_pSurface, NULL)) != DD_OK)
	{
		TRACE("Couldn't create a surface\n");
		ddrval = ddrval;
		return ddrval;
	}

// If we succeeded then get the surface description
	ZeroMemory(&m_SurfaceDesc, sizeof(m_SurfaceDesc));
	m_SurfaceDesc.dwSize = sizeof(m_SurfaceDesc);
	if ((ddrval = m_pSurface->GetSurfaceDesc(&m_SurfaceDesc)) != DD_OK)
	{
		TRACE("Couldn't get a surface desciption\n");
	}

	return ddrval;
}
//======================================================-==--=-- --  -
int CTextureSurface::Create(const LPDIRECTDRAW7 pDD, const GUID * pDeviceGuid, const int dwWidth, const DDPIXELFORMAT* pPixelFormat)
{
	if (pPixelFormat)
	{
		m_bDefPixelFormat = FALSE;
		memcpy(&m_PixelFormat, pPixelFormat, sizeof(m_PixelFormat));
	}
	else m_bDefPixelFormat = TRUE;
	m_pDeviceGuid = pDeviceGuid;
	return PCreate(pDD, dwWidth, dwWidth);
}
//======================================================-==--=-- --  -
int CTextureSurface::Create(const LPDIRECTDRAW7 pDD, const GUID * pDeviceGuid, CImageLoader & loader, const DDPIXELFORMAT* pPixelFormat, const DDPIXELFORMAT* pPixelFormatA, SINT *texw, SINT *texh)
{
	// Load the image
	if (!loader.Load())
		return DD_FALSE;

#if 0		 // JJS dont't care
	// check it's square
	if (loader.Width() != loader.Height())
		return DD_FALSE;
#endif
	// does it require an alpha channeled texture?
	BOOL	alpha = loader.NeedsAlpha();

	// Save the pixel format
	if (alpha)
	{
		if (pPixelFormatA)
		{
			m_bDefPixelFormat = FALSE;
			memcpy(&m_PixelFormat, pPixelFormatA, sizeof(m_PixelFormat));
		}
		else
			m_bDefPixelFormat = TRUE;
	}
	else
	{
		if (pPixelFormat)
		{
			m_bDefPixelFormat = FALSE;
			memcpy(&m_PixelFormat, pPixelFormat, sizeof(m_PixelFormat));
		}
		else
			m_bDefPixelFormat = TRUE;
	}

	m_pDeviceGuid = pDeviceGuid;

	// Create the surface
	int result = PCreate(pDD, loader.Width(), loader.Height());
	if (result != DD_OK)
	{
		char foo[200];
		sprintf(foo, "Level %d: Couldn't create surface for texture: %s\n", GAME.GetCurrentLevelNumber(), loader.GetFilename());
		CONSOLE.AddString(foo);
		OutputDebugString(foo);

		return result;
	}

	// Copy the memory to the texture

	DDSURFACEDESC2	desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.dwSize = sizeof(desc);
	m_pSurface->GetSurfaceDesc(&desc);

	EnumSurfacesCallback2(m_pSurface, &desc, (LPVOID)&loader);
	m_pSurface->EnumAttachedSurfaces((LPVOID)&loader, EnumSurfacesCallback2);

//	SetBits(loader.Width(), loader.Height(), loader.GetData(), loader.GetAlphaData());
//	SetBits(loader.Width(), loader.Height(), loader.GetData());

	if(texw)
		*texw = loader.Width();
	if(texh)
		*texh = loader.Height();
	return DD_OK;
}
//======================================================-==--=-- --  -
void CTextureSurface::Destroy()
{
	RELEASE(m_pSurface);
	m_pDD = NULL;
}
//======================================================-==--=-- --  -
HRESULT CTextureSurface::Lock(LPRECT lpDestRect,LPDDSURFACEDESC2 lpDDSurfaceDesc,DWORD dwFlags,HANDLE hEvent)
{
	if (!m_pSurface)
	{
		TRACE("Trying to lock a texture surface, but it's not created\n");
		return -1;
	}
	HRESULT result = m_pSurface->Lock(lpDestRect,lpDDSurfaceDesc,dwFlags,hEvent);
	if (result != DD_OK)
	{
		TRACE("Couldn't lock texture surface\n");
		return -1;
	}
	return result;
}
//======================================================-==--=-- --  -
HRESULT CTextureSurface::Unlock(LPVOID lpSurfaceData)
{
	return m_pSurface->Unlock((RECT *)lpSurfaceData);
}

//======================================================-==--=-- --  -
HDC CTextureSurface::GetDC()
{
	HDC hdc;

	if (!m_pSurface)
	{
		TRACE("Trying to GetDC a texture surface, but it's not created\n");
		return NULL;
	}
	if (m_pSurface->GetDC(&hdc)==DD_OK)
		return hdc;
	TRACE("Couldn't get texture surface DC\n");
	return NULL;
}

//======================================================-==--=-- --  -
void CTextureSurface::ReleaseDC(HDC hdc)
{
	if (!GetSurface())
		return;
	m_pSurface->ReleaseDC(hdc);
}

//======================================================-==--=-- --  -
// For TexureSurface.cpp
HRESULT WINAPI CTextureSurface::EnumSurfacesCallback2(	LPDIRECTDRAWSURFACE7	lpDDSurface, 
														LPDDSURFACEDESC2		lpDDSurfaceDesc,  
														LPVOID					lpContext)
{
	HRESULT hr;
	DDSURFACEDESC2	desc;
	CImageLoader* loader = (CImageLoader*)lpContext;

	SINT	nWidth		= loader->Width();
	SINT	nHeight		= loader->Width();
	LPVOID	pBits		= loader->GetData();
	LPVOID	pAlphaBits	= loader->GetAlphaData();
	
	int		yrat; 
	int		xrat; 

	// Lock the surface down
	ZeroMemory(&desc, sizeof(desc));
	desc.dwSize = sizeof(desc);
	hr = lpDDSurface->Lock(NULL, &desc, DDLOCK_WAIT, NULL); 
	if (hr != DD_OK)
		return hr;

	SINT	rat = nWidth / desc.dwWidth;
	// Loop through each pixel
	LPWORD pDest = (LPWORD)desc.lpSurface;
	SINT sH = SINT(desc.dwHeight);
	SINT sW = SINT(desc.dwWidth);
	SINT lP = (desc.lPitch/2);

	for (SINT y=0; y < sH; y++, pDest += lP)
	{
		for (SINT x=0; x < sW; x++)
		{
			
			DWORD	b = 0, g = 0, r = 0, a = 0;
			SINT	c = 0;
			SINT	px, py;
			yrat	= (y*rat) + rat;
			xrat	= (x*rat) + rat;
			for (py = y * rat; py < yrat; py ++)
				for (px = x * rat; px < xrat; px ++)
				{
					b += ((UBYTE *)pBits)[(py * nWidth * 3) + px * 3    ];
					g += ((UBYTE *)pBits)[(py * nWidth * 3) + px * 3 + 1];
					r += ((UBYTE *)pBits)[(py * nWidth * 3) + px * 3 + 2];
					a += pAlphaBits ? ((UBYTE *)pAlphaBits)[(py * nWidth) + px] : 0;

					c ++;
				}

			b /= c;
			g /= c;
			r /= c;
			a /= c;

			// Convert each component to the new format and mask
			b = (b * desc.ddpfPixelFormat.dwBBitMask / 255) & desc.ddpfPixelFormat.dwBBitMask;
			g = (g * desc.ddpfPixelFormat.dwGBitMask / 255) & desc.ddpfPixelFormat.dwGBitMask;
			r = (r * desc.ddpfPixelFormat.dwRBitMask / 255) & desc.ddpfPixelFormat.dwRBitMask;
			a = (a * desc.ddpfPixelFormat.dwRGBAlphaBitMask / 255) & desc.ddpfPixelFormat.dwRGBAlphaBitMask;

			// Merge components and save
			pDest[x] = (WORD)(r | g | b | a);
		}
	}
	lpDDSurface->Unlock(NULL);

	// Enumerate all sub surfaces.
	lpDDSurface->EnumAttachedSurfaces(lpContext, EnumSurfacesCallback2);

	return DD_OK;
}

#endif
