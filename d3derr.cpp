#include "common.h"

#if TARGET == PC

#include "d3derr.h"
//
// Traces the appropriate direct draw error.
//

void trace_dd_error(HRESULT dd_err)
{
	TRACE("Direct Draw Error: ");

	switch(dd_err)
	{
		case	DDERR_ALREADYINITIALIZED:			TRACE("DDERR_ALREADYINITIALIZED");break;
		case	DDERR_CANNOTATTACHSURFACE:			TRACE("DDERR_CANNOTATTACHSURFACE");break;
		case	DDERR_CANNOTDETACHSURFACE:			TRACE("DDERR_CANNOTDETACHSURFACE");break;
		case	DDERR_CURRENTLYNOTAVAIL:			TRACE("DDERR_CURRENTLYNOTAVAIL");break;
		case	DDERR_EXCEPTION:					TRACE("DDERR_EXCEPTION");break;
		case	DDERR_GENERIC:						TRACE("DDERR_GENERIC");break;
		case	DDERR_HEIGHTALIGN:					TRACE("DDERR_HEIGHTALIGN");break;
		case	DDERR_INCOMPATIBLEPRIMARY:			TRACE("DDERR_INCOMPATIBLEPRIMARY");break;
		case	DDERR_INVALIDCAPS:					TRACE("DDERR_INVALIDCAPS");break;
		case	DDERR_INVALIDCLIPLIST:				TRACE("DDERR_INVALIDCLIPLIST");break;
		case	DDERR_INVALIDMODE:					TRACE("DDERR_INVALIDMODE");break;
		case	DDERR_INVALIDOBJECT:				TRACE("DDERR_INVALIDOBJECT");break;
		case	DDERR_INVALIDPARAMS:				TRACE("DDERR_INVALIDPARAMS");break;
		case	DDERR_INVALIDPIXELFORMAT:			TRACE("DDERR_INVALIDPIXELFORMAT");break;
		case	DDERR_INVALIDRECT:					TRACE("DDERR_INVALIDRECT");break;
		case	DDERR_LOCKEDSURFACES:				TRACE("DDERR_LOCKEDSURFACES");break;
		case	DDERR_NO3D:							TRACE("DDERR_NO3D");break;
		case	DDERR_NOALPHAHW:					TRACE("DDERR_NOALPHAHW");break;
		case	DDERR_NOCLIPLIST:					TRACE("DDERR_NOCLIPLIST");break;
		case	DDERR_NOCOLORCONVHW:				TRACE("DDERR_NOCOLORCONVHW");break;
		case	DDERR_NOCOOPERATIVELEVELSET:		TRACE("DDERR_NOCOOPERATIVELEVELSET");break;
		case	DDERR_NOCOLORKEY:					TRACE("DDERR_NOCOLORKEY");break;
		case	DDERR_NOCOLORKEYHW:					TRACE("DDERR_NOCOLORKEYHW");break;
		case	DDERR_NODIRECTDRAWSUPPORT:			TRACE("DDERR_NODIRECTDRAWSUPPORT");break;
		case	DDERR_NOEXCLUSIVEMODE:				TRACE("DDERR_NOEXCLUSIVEMODE");break;
		case	DDERR_NOFLIPHW:						TRACE("DDERR_NOFLIPHW");break;
		case	DDERR_NOGDI:						TRACE("DDERR_NOGDI");break;
		case	DDERR_NOMIRRORHW:					TRACE("DDERR_NOMIRRORHW");break;
		case	DDERR_NOTFOUND:						TRACE("DDERR_NOTFOUND");break;
		case	DDERR_NOOVERLAYHW:					TRACE("DDERR_NOOVERLAYHW");break;
		case	DDERR_NORASTEROPHW:					TRACE("DDERR_NORASTEROPHW");break;
		case	DDERR_NOROTATIONHW:					TRACE("DDERR_NOROTATIONHW");break;
		case	DDERR_NOSTRETCHHW:					TRACE("DDERR_NOSTRETCHHW");break;
		case	DDERR_NOT4BITCOLOR:					TRACE("DDERR_NOT4BITCOLOR");break;
		case	DDERR_NOT4BITCOLORINDEX:			TRACE("DDERR_NOT4BITCOLORINDEX");break;
		case	DDERR_NOT8BITCOLOR:					TRACE("DDERR_NOT8BITCOLOR");break;
		case	DDERR_NOTEXTUREHW:					TRACE("DDERR_NOTEXTUREHW");break;
		case	DDERR_NOVSYNCHW:					TRACE("DDERR_NOVSYNCHW");break;
		case	DDERR_NOZBUFFERHW:					TRACE("DDERR_NOZBUFFERHW");break;
		case	DDERR_NOZOVERLAYHW:					TRACE("DDERR_NOZOVERLAYHW");break;
		case	DDERR_OUTOFCAPS:					TRACE("DDERR_OUTOFCAPS");break;
		case	DDERR_OUTOFMEMORY:					TRACE("DDERR_OUTOFMEMORY");break;
		case	DDERR_OUTOFVIDEOMEMORY:				TRACE("DDERR_OUTOFVIDEOMEMORY");break;
		case	DDERR_OVERLAYCANTCLIP:				TRACE("DDERR_OVERLAYCANTCLIP");break;
		case	DDERR_OVERLAYCOLORKEYONLYONEACTIVE:	TRACE("DDERR_OVERLAYCOLORKEYONLYONEACTIVE");break;
		case	DDERR_PALETTEBUSY:					TRACE("DDERR_PALETTEBUSY");break;
		case	DDERR_COLORKEYNOTSET:				TRACE("DDERR_COLORKEYNOTSET");break;
		case	DDERR_SURFACEALREADYATTACHED:		TRACE("DDERR_SURFACEALREADYATTACHED");break;
		case	DDERR_SURFACEALREADYDEPENDENT:		TRACE("DDERR_SURFACEALREADYDEPENDENT");break;
		case	DDERR_SURFACEBUSY:					TRACE("DDERR_SURFACEBUSY");break;
		case	DDERR_CANTLOCKSURFACE:				TRACE("DDERR_CANTLOCKSURFACE");break;
		case	DDERR_SURFACEISOBSCURED:			TRACE("DDERR_SURFACEISOBSCURED");break;
		case	DDERR_SURFACELOST:					TRACE("DDERR_SURFACELOST");break;
		case	DDERR_SURFACENOTATTACHED:			TRACE("DDERR_SURFACENOTATTACHED");break;
		case	DDERR_TOOBIGHEIGHT:					TRACE("DDERR_TOOBIGHEIGHT");break;
		case	DDERR_TOOBIGSIZE:					TRACE("DDERR_TOOBIGSIZE");break;
		case	DDERR_TOOBIGWIDTH:					TRACE("DDERR_TOOBIGWIDTH");break;
		case	DDERR_UNSUPPORTED:					TRACE("DDERR_UNSUPPORTED");break;
		case	DDERR_UNSUPPORTEDFORMAT:			TRACE("DDERR_UNSUPPORTEDFORMAT");break;
		case	DDERR_UNSUPPORTEDMASK:				TRACE("DDERR_UNSUPPORTEDMASK");break;
		case	DDERR_VERTICALBLANKINPROGRESS:		TRACE("DDERR_VERTICALBLANKINPROGRESS");break;
		case	DDERR_WASSTILLDRAWING:				TRACE("DDERR_WASSTILLDRAWING");break;
		case	DDERR_XALIGN:						TRACE("DDERR_XALIGN");break;
		case	DDERR_INVALIDDIRECTDRAWGUID:		TRACE("DDERR_INVALIDDIRECTDRAWGUID");break;
		case	DDERR_DIRECTDRAWALREADYCREATED:		TRACE("DDERR_DIRECTDRAWALREADYCREATED");break;
		case	DDERR_NODIRECTDRAWHW:				TRACE("DDERR_NODIRECTDRAWHW");break;
		case	DDERR_PRIMARYSURFACEALREADYEXISTS:	TRACE("DDERR_PRIMARYSURFACEALREADYEXISTS");break;
		case	DDERR_NOEMULATION:					TRACE("DDERR_NOEMULATION");break;
		case	DDERR_REGIONTOOSMALL:				TRACE("DDERR_REGIONTOOSMALL");break;
		case	DDERR_CLIPPERISUSINGHWND:			TRACE("DDERR_CLIPPERISUSINGHWND");break;
		case	DDERR_NOCLIPPERATTACHED:			TRACE("DDERR_NOCLIPPERATTACHED");break;
		case	DDERR_NOHWND:						TRACE("DDERR_NOHWND");break;
		case	DDERR_HWNDSUBCLASSED:				TRACE("DDERR_HWNDSUBCLASSED");break;
		case	DDERR_HWNDALREADYSET:				TRACE("DDERR_HWNDALREADYSET");break;
		case	DDERR_NOPALETTEATTACHED:			TRACE("DDERR_NOPALETTEATTACHED");break;
		case	DDERR_NOPALETTEHW:					TRACE("DDERR_NOPALETTEHW");break;
		case	DDERR_BLTFASTCANTCLIP:				TRACE("DDERR_BLTFASTCANTCLIP");break;
		case	DDERR_NOBLTHW:						TRACE("DDERR_NOBLTHW");break;
		case	DDERR_NODDROPSHW:					TRACE("DDERR_NODDROPSHW");break;
		case	DDERR_OVERLAYNOTVISIBLE:			TRACE("DDERR_OVERLAYNOTVISIBLE");break;
		case	DDERR_NOOVERLAYDEST:				TRACE("DDERR_NOOVERLAYDEST");break;
		case	DDERR_INVALIDPOSITION:				TRACE("DDERR_INVALIDPOSITION");break;
		case	DDERR_NOTAOVERLAYSURFACE:			TRACE("DDERR_NOTAOVERLAYSURFACE");break;
		case	DDERR_EXCLUSIVEMODEALREADYSET:		TRACE("DDERR_EXCLUSIVEMODEALREADYSET");break;
		case	DDERR_NOTFLIPPABLE:					TRACE("DDERR_NOTFLIPPABLE");break;
		case	DDERR_CANTDUPLICATE:				TRACE("DDERR_CANTDUPLICATE");break;
		case	DDERR_NOTLOCKED:					TRACE("DDERR_NOTLOCKED");break;
		case	DDERR_CANTCREATEDC:					TRACE("DDERR_CANTCREATEDC");break;
		case	DDERR_NODC:							TRACE("DDERR_NODC");break;
		case	DDERR_WRONGMODE:					TRACE("DDERR_WRONGMODE");break;
		case	DDERR_IMPLICITLYCREATED:			TRACE("DDERR_IMPLICITLYCREATED");break;
		case	DDERR_NOTPALETTIZED:				TRACE("DDERR_NOTPALETTIZED");break;
		case	DDERR_UNSUPPORTEDMODE:				TRACE("DDERR_UNSUPPORTEDMODE");break;
		case	DDERR_NOMIPMAPHW:					TRACE("DDERR_NOMIPMAPHW");break;
		case	DDERR_INVALIDSURFACETYPE:			TRACE("DDERR_INVALIDSURFACETYPE");break;
		case	DDERR_NOOPTIMIZEHW:					TRACE("DDERR_NOOPTIMIZEHW");break;
		case	DDERR_NOTLOADED:					TRACE("DDERR_NOTLOADED");break;
		case	DDERR_DCALREADYCREATED:				TRACE("DDERR_DCALREADYCREATED");break;
		case	DDERR_NONONLOCALVIDMEM:				TRACE("DDERR_NONONLOCALVIDMEM");break;
		case	DDERR_CANTPAGELOCK:					TRACE("DDERR_CANTPAGELOCK");break;
		case	DDERR_CANTPAGEUNLOCK:				TRACE("DDERR_CANTPAGEUNLOCK");break;
		case	DDERR_NOTPAGELOCKED:				TRACE("DDERR_NOTPAGELOCKED");break;
		case	DDERR_MOREDATA:						TRACE("DDERR_MOREDATA");break;
		case	DDERR_VIDEONOTACTIVE:				TRACE("DDERR_VIDEONOTACTIVE");break;
		case	DDERR_DEVICEDOESNTOWNSURFACE:		TRACE("DDERR_DEVICEDOESNTOWNSURFACE");break;
		case	DDERR_NOTINITIALIZED:				TRACE("DDERR_NOTINITIALIZED");break;
		default:									
			{
				char str[400];
				sprintf(str, "Unknown - %ld",dd_err&0xffff);
				TRACE(str);
			}
	}

	TRACE("\n");
}


//
// Traces the appropriate direct 3d error.
//

void trace_d3d_error(HRESULT dd_err)
{
	TRACE("Direct Draw Error: ");

	if (dd_err) 
	{
		TRACE("oh:");
	}
	switch(dd_err)
	{
		case	D3DERR_BADMAJORVERSION:			TRACE("D3DERR_BADMAJORVERSION");break;
		case	D3DERR_BADMINORVERSION:			TRACE("D3DERR_BADMINORVERSION");break;
		case	D3DERR_INVALID_DEVICE:			TRACE("D3DERR_INVALID_DEVICE");break;
		case	D3DERR_EXECUTE_CREATE_FAILED:	TRACE("D3DERR_EXECUTE_CREATE_FAILED");break;
		case	D3DERR_EXECUTE_DESTROY_FAILED:	TRACE("D3DERR_EXECUTE_DESTROY_FAILED");break;
		case	D3DERR_EXECUTE_LOCK_FAILED:		TRACE("D3DERR_EXECUTE_LOCK_FAILED");break;
		case	D3DERR_EXECUTE_UNLOCK_FAILED:	TRACE("D3DERR_EXECUTE_UNLOCK_FAILED");break;
		case	D3DERR_EXECUTE_LOCKED:			TRACE("D3DERR_EXECUTE_LOCKED");break;
		case	D3DERR_EXECUTE_NOT_LOCKED:		TRACE("D3DERR_EXECUTE_NOT_LOCKED");break;
		case	D3DERR_EXECUTE_FAILED:			TRACE("D3DERR_EXECUTE_FAILED");break;
		case	D3DERR_EXECUTE_CLIPPED_FAILED:	TRACE("D3DERR_EXECUTE_CLIPPED_FAILED");break;
		case	D3DERR_TEXTURE_NO_SUPPORT:		TRACE("D3DERR_TEXTURE_NO_SUPPORT");break;
		case	D3DERR_TEXTURE_CREATE_FAILED:	TRACE("D3DERR_TEXTURE_CREATE_FAILED");break;
		case	D3DERR_TEXTURE_DESTROY_FAILED:	TRACE("D3DERR_TEXTURE_DESTROY_FAILED");break;
		case	D3DERR_TEXTURE_LOCK_FAILED:		TRACE("D3DERR_TEXTURE_LOCK_FAILED");break;
		case	D3DERR_TEXTURE_UNLOCK_FAILED:	TRACE("D3DERR_TEXTURE_UNLOCK_FAILED");break;
		case	D3DERR_TEXTURE_LOAD_FAILED:		TRACE("D3DERR_TEXTURE_LOAD_FAILED");break;
		case	D3DERR_TEXTURE_SWAP_FAILED:		TRACE("D3DERR_TEXTURE_SWAP_FAILED");break;
		case	D3DERR_TEXTURE_LOCKED:			TRACE("D3DERR_TEXTURE_LOCKED");break;
		case	D3DERR_TEXTURE_NOT_LOCKED:		TRACE("D3DERR_TEXTURE_NOT_LOCKED");break;
		case	D3DERR_TEXTURE_GETSURF_FAILED:	TRACE("D3DERR_TEXTURE_GETSURF_FAILED");break;
		case	D3DERR_MATRIX_CREATE_FAILED:	TRACE("D3DERR_MATRIX_CREATE_FAILED");break;
		case	D3DERR_MATRIX_DESTROY_FAILED:	TRACE("D3DERR_MATRIX_DESTROY_FAILED");break;
		case	D3DERR_MATRIX_SETDATA_FAILED:	TRACE("D3DERR_MATRIX_SETDATA_FAILED");break;
		case	D3DERR_MATRIX_GETDATA_FAILED:	TRACE("D3DERR_MATRIX_GETDATA_FAILED");break;
		case	D3DERR_SETVIEWPORTDATA_FAILED:	TRACE("D3DERR_SETVIEWPORTDATA_FAILED");break;
		case	D3DERR_MATERIAL_CREATE_FAILED:	TRACE("D3DERR_MATERIAL_CREATE_FAILED");break;
		case	D3DERR_MATERIAL_DESTROY_FAILED:	TRACE("D3DERR_MATERIAL_DESTROY_FAILED");break;
		case	D3DERR_MATERIAL_SETDATA_FAILED:	TRACE("D3DERR_MATERIAL_SETDATA_FAILED");break;
		case	D3DERR_MATERIAL_GETDATA_FAILED:	TRACE("D3DERR_MATERIAL_GETDATA_FAILED");break;
		case	D3DERR_LIGHT_SET_FAILED:		TRACE("D3DERR_LIGHT_SET_FAILED");break;
		case	D3DERR_SCENE_IN_SCENE:			TRACE("D3DERR_SCENE_IN_SCENE");break;
		case	D3DERR_SCENE_NOT_IN_SCENE:		TRACE("D3DERR_SCENE_NOT_IN_SCENE");break;
		case	D3DERR_SCENE_BEGIN_FAILED:		TRACE("D3DERR_SCENE_BEGIN_FAILED");break;
		case	D3DERR_SCENE_END_FAILED:		TRACE("D3DERR_SCENE_END_FAILED");break;
		case	D3DERR_INBEGIN:					TRACE("D3DERR_INBEGIN");break;
		case	D3DERR_NOTINBEGIN:				TRACE("D3DERR_NOTINBEGIN");break;
		case	D3DERR_NOVIEWPORTS:				TRACE("D3DERR_NOVIEWPORTS");break;
		case	D3DERR_VIEWPORTDATANOTSET:		TRACE("D3DERR_VIEWPORTDATANOTSET");break;
		case	D3DERR_INVALIDCURRENTVIEWPORT:	TRACE("D3DERR_INVALIDCURRENTVIEWPORT");break;
		case	D3DERR_INVALIDPRIMITIVETYPE:	TRACE("D3DERR_INVALIDPRIMITIVETYPE");break;
		case	D3DERR_INVALIDVERTEXTYPE:		TRACE("D3DERR_INVALIDVERTEXTYPE");break;
		case	D3DERR_TEXTURE_BADSIZE:			TRACE("D3DERR_TEXTURE_BADSIZE");break;
		default:								trace_dd_error(dd_err);
	}

	TRACE("\n");
}

#endif
