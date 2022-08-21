#include	"common.h"

#if TARGET == PC

#include	"vbuffer.h"

int	VBuffer::Init(LPDIRECT3DDEVICE3 d3ddev, int dwFlags, int dwCaps, int dwFVF, int dwNumVertices)
{
	    // Check parameters
    if( NULL==d3ddev)
        return 0;

	// Get a ptr to the ID3D object to create VB's, materials and/or lights.
    // Note: the Release() call just serves to decrease the ref count.
    LPDIRECT3D3 pD3D;
    if( FAILED( d3ddev->GetDirect3D( &pD3D ) ) )
        return 0;
    pD3D->Release();

	// Create and fill the vertex buffer.
	D3DVERTEXBUFFERDESC vbdesc;
	ZeroMemory( &vbdesc, sizeof(D3DVERTEXBUFFERDESC) );
	vbdesc.dwSize        = sizeof(D3DVERTEXBUFFERDESC);
	vbdesc.dwCaps        = dwCaps;
    vbdesc.dwFVF         = dwFVF;
    vbdesc.dwNumVertices = dwNumVertices;

	// Get the device's caps bits
	D3DDEVICEDESC ddHwDesc, ddSwDesc;
	ddHwDesc.dwSize = D3DDEVICEDESCSIZE;	// this is 
	ddSwDesc.dwSize = D3DDEVICEDESCSIZE;	// Bollocks!
	if( FAILED( d3ddev->GetCaps( &ddHwDesc, &ddSwDesc ) ) )
		return 0;

	// If the device is not hardware, make sure vertex buffers use system mem.
	if( 0L == ddHwDesc.dwFlags )
		vbdesc.dwCaps |= D3DVBCAPS_SYSTEMMEMORY;

	if( FAILED( pD3D->CreateVertexBuffer( &vbdesc, &VB, dwFlags,
		                                  NULL ) ) )
		return 0;
	return 1;
}

#endif
