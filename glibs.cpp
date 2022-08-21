/******************************************************************************
 Name     : KMBLEND
 Title    : KAMUI test program "Blend" from VL
 Author   : VideoLogic Carlos Sarria / NEC Software Takeshi Muto
 Created  : Jan 1998 by VideoLogic
 Modified : June     09. 1998 by Takeshi Muto for KAMUI
			January  27. 1999 by T.Kamei 	 for KAMUI2
			February 15. 1999 by Masayuki Imanishi for Shinobi
 Description :
 Copyright : 1998 by VideoLogic Limited. All rights reserved.
******************************************************************************/
#define		_DISPLAY_STRING_						/* if defined then display strings on screen	*/
#define		_DISP_PERFORMANCE_						/* Display performance							*/

#include <stdio.h>
#include <math.h>
#include <Shinobi.h>
#include <kamui2.h>

#pragma warning (disable : 4244) /* Disables float to double conversion warning */
#define _STRICT_UNION_
#define _CLX2_
#define _CLX_
#include	"kamui2.h"				/* for Kamui2 Call 									*/
#include	"dispstr.h"				/* for DisplayString								*/
#include	"vltex.h"				/* for vl sample texture							*/
#include    "dcmembuffer.h"
#if 0
#define _KM_USE_VERTEX_MACRO_
#define _KM_USE_VERTEX_MACRO_L3_
#include    "kamuix.h"
#endif
#if TARGET!= DC
#define 	PI	(3.1416f)
#endif
#define		RGBAColour(r, g, b, a)   ((unsigned long) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define		RGBColour(r, g, b)	((KMUINT32) (((r) << 16) | ((g) <<  8) | (b)))
#define		RGBA(r, g, b, a)	((KMUINT32) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

/* Macros for kamui */
#define		GetRED(col)			((KMFLOAT) ( ((col) & 0x00ff0000) >> 16 ) / 256.0f)
#define		GetGREEN(col)		((KMFLOAT) ( ((col) & 0x0000ff00) >>  8 ) / 256.0f)
#define		GetBLUE(col)		((KMFLOAT) ( ((col) & 0x000000ff)       ) / 256.0f)
#define		GetALPHA(col)		((KMFLOAT) ( ((col) & 0xff000000) >> 24 ) / 256.0f)

typedef float		pvr_vector[3];	/* type of pvr_vector */

/* Peripheral */
PDS_PERIPHERAL *per;



typedef struct tagPVRVERTEX
{
	/* Projected screen coordinates: */
	float		fX;		/* 0 <= fX < screen width (up to 1024)					*/
	float		fY;		/* 0 <= fY < screen height (up to 1024)					*/
	float		fZ;		/* 0.01 <= fZ <= 1, where 1 is infinite distance away	*/
	/* 1/w for the perspective projection (should be set to 1.0/fZ; fZ is */
	/* actually ignored): */
	float		fInvW;
	KMUINT32	u32Colour;			/* Material colour (D3DCOLOR format):	*/
	KMUINT32	u32Specular;		/* Specular colour (D3DCOLOR format):	*/
	float		fUOverW, fVOverW;	/* Texture coordinates:					*/
} PVRVERTEX, *PPVRVERTEX;

/****************************************************************************
 *	Valuables for KAMUI
 ****************************************************************************/
KMSTATUS			status;					/* return status of KAMUI API	*/
KMVERSIONINFO		VersionInfo;			/* KAMUI Version information	*/
KMSURFACEDESC		PrimarySurfaceDesc;		/* Primary frame buffer			*/
KMSURFACEDESC		BackSurfaceDesc;		/* Back-buffer frame buffer		*/
KMVERTEXBUFFDESC	VertexBufferDesc;		/* Vertex-buffer				*/
KMDWORD				FogDensity;				/* Fog density					*/
KMDWORD				FogD = 1;
KMSTRIPCONTEXT		StripContext00;
KMSTRIPCONTEXT		StripContext03;
KMSTRIPHEAD			StripHead00_Gold;
KMSTRIPHEAD			StripHead00_Trans;
KMSTRIPHEAD			StripHead03_Gold;
KMSTRIPHEAD			StripHead03_Trans;
PKMSURFACEDESC			pFB[2];
KMDWORD					FBarea[24576 + 19456];
								/* FBstruct 4096 Entry (= 4096 * 24 / 4) *
								 * VQstruct 1024 Entry (= 1024 * 76 / 4) */
KMSYSTEMCONFIGSTRUCT	SystemConfig;
PKMDWORD				dwDataPtr;

/*------------------------------- Global Variables -----------------------------------*/
/* Globals */
KMSURFACEDESC		TexOpaq, TexTrans, TexBack, TexGold;
static unsigned		Frame			= 0;
static int			BlendSrc		= 4;	/* SRCA				*/
static int			BlendDst		= 5;	/* INVSRCA			*/
static int			BlendModulate	= TRUE;	/* MODULATE			*/
static int			AlphaSource		= 0;	/* TEXTURE ALPHA	*/
static int			PosZ			= 800;

/* Info Structure */
struct
{
	int		VertexAlpha;
	int		AlphaBlend;

} Info = { TRUE, TRUE };

/* Prototypes */
void NextFrame		(void);
void ReadMaple			(void);		/* Maple data read routine */

/* Static Prototypes */
static void SetContext			(void);
static void SetTextures			(void);


/* Macros for kamui */
#define		GetRED(col)			((KMFLOAT) ( ((col) & 0x00ff0000) >> 16 ) / 256.0f)
#define		GetGREEN(col)		((KMFLOAT) ( ((col) & 0x0000ff00) >>  8 ) / 256.0f)
#define		GetBLUE(col)		((KMFLOAT) ( ((col) & 0x000000ff)       ) / 256.0f)
#define		GetALPHA(col)		((KMFLOAT) ( ((col) & 0xff000000) >> 24 ) / 256.0f)

/****************************************************************************
 *	Check KAMUI function status
 ****************************************************************************/
void	Check_Status(KMSTATUS status, char *func)
{
	switch(status)
	{
		case KMSTATUS_SUCCESS:
			/* No message is displayed when SUCCESS */
			break;
		case KMSTATUS_INVALID_DISPLAY_MODE:
			PRINTOUT(("%s:KMSTATUS_INVALID_DISPLAY_MODE\n",func));
			break;
		case KMSTATUS_INVALID_VIDEO_MODE:
			PRINTOUT(("%s:KMSTATUS_INVALID_VIDEO_MODE\n",func));
			break;
		case KMSTATUS_NOT_ENOUGH_MEMORY:
			PRINTOUT(("%s:KMSTATUS_NOT_ENOUGH_MEMORY\n",func));
			break;
		case KMSTATUS_INVALID_TEXTURE_TYPE:
			PRINTOUT(("%s:KMSTATUS_INVALID_TEXTURE_TYPE\n",func));
			break;
		case KMSTATUS_INVALID_MIPMAPED_TEXTURE:
			PRINTOUT(("%s:KMSTATUS_INVALID_MIPMAPED_TEXTURE\n",func));
			break;
		case KMSTATUS_CANT_FLIP_SURFACE:
			PRINTOUT(("%s:KMSTATUS_CANT_FLIP_SURFACE\n",func));
			break;
		case KMSTATUS_INVALID_ADDRESS:
			PRINTOUT(("%s:KMSTATUS_INVALID_ADDRESS\n",func));
			break;
		case KMSTATUS_HARDWARE_NOT_PRESENTED:
			PRINTOUT(("%s:KMSTATUS_HARDWARE_NOT_PRESENTED\n",func));
			break;
		case KMSTATUS_INVALID_SETTING:
			PRINTOUT(("%s:KMSTATUS_INVALID_SETTING\n",func));
			break;
		case KMSTATUS_INVALID_VERTEX_TYPE:
			PRINTOUT(("%s:KMSTATUS_INVALID_VERTEX_TYPE\n",func));
			break;
		case KMSTATUS_NOT_IMPLEMENTED:
			PRINTOUT(("%s:KMSTATUS_NOT_IMPREMENTED\n",func));
			break;
		case KMSTATUS_INVALID_PARAMETER:
			PRINTOUT(("%s:KMSTATUS_INVALID_PARAMETER\n",func));
			break;
		default:
			PRINTOUT(("%s:???Undefined status : %d\n", func, status));
			break;
	}
}

typedef struct
{
 float x1,y1,z1;
 float x2,y2,z2;
 float x3,y3,z3; 
 KMUINT32 vc1,vc2,vc3; 
} sdsTriangle;

void DrawGouraudTriangle(sdsTriangle tri)
{
	
KMVERTEX_00 Ver00;	
		    kmStartStrip(&VertexBufferDesc,&StripHead03_Trans);
		
			Ver00.ParamControlWord	= KM_VERTEXPARAM_NORMAL;
			Ver00.fX				= tri.x1;
			Ver00.fY				= tri.y1;
			Ver00.u.fZ				= tri.z1;
			Ver00.uBaseRGB.dwPacked	= tri.vc1;
			status = kmSetVertex(&VertexBufferDesc,&Ver00,KM_VERTEXTYPE_00,sizeof(KMVERTEX_00) );
			Check_Status(status, "kmSetVertex");

			Ver00.ParamControlWord	= KM_VERTEXPARAM_NORMAL;
			Ver00.fX				= tri.x2;
			Ver00.fY				= tri.y2;
			Ver00.u.fZ				= tri.z2;
			Ver00.uBaseRGB.dwPacked	= tri.vc2;
			status = kmSetVertex(&VertexBufferDesc,&Ver00,KM_VERTEXTYPE_00,sizeof(KMVERTEX_00) );
			Check_Status(status, "kmSetVertex");

			Ver00.ParamControlWord	= KM_VERTEXPARAM_ENDOFSTRIP;
			Ver00.fX				= tri.x3;
			Ver00.fY				= tri.y3;
			Ver00.u.fZ				= tri.z3;
			Ver00.uBaseRGB.dwPacked	= tri.vc3;
		
			status = kmSetVertex(&VertexBufferDesc,&Ver00,KM_VERTEXTYPE_00,sizeof(KMVERTEX_00) );
			Check_Status(status, "kmSetVertex");
			
	kmEndStrip(&VertexBufferDesc);
}



/***************************************
 *	VertexType00用KMSTRIPHEADの構築
 ***************************************/
void	InitStripContext00(
								PKMSTRIPCONTEXT pStripContext,
								PKMSTRIPHEAD	pStripHead
)
{
	pStripContext->nSize = sizeof(KMSTRIPCONTEXT);
	pStripContext->StripControl.nListType		 					= KM_OPAQUE_POLYGON;
	pStripContext->StripControl.nUserClipMode	 					= KM_USERCLIP_DISABLE;
	pStripContext->StripControl.nShadowMode		 					= KM_NORMAL_POLYGON;
	pStripContext->StripControl.bGouraud		 					= KM_TRUE;
	pStripContext->ObjectControl.nDepthCompare			 			= KM_GREATER;
	pStripContext->ObjectControl.nCullingMode			 			= KM_NOCULLING;
	pStripContext->ObjectControl.bZWriteDisable			 			= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nSRCBlendingMode	= KM_ONE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nDSTBlendingMode	= KM_ONE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bSRCSelect			= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bDSTSelect			= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nFogMode			= KM_NOFOG;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bColorClamp		= KM_TRUE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bUseAlpha			= KM_TRUE;

	kmGenerateStripHead00( pStripHead,pStripContext );
}

/***************************************
 *	VertexType03用KMSTRIPHEADの構築
 ***************************************/
void	InitStripContext03(
								PKMSTRIPCONTEXT pStripContext,
								PKMSTRIPHEAD	pStripHead,
								PKMSURFACEDESC	pTextureDesc
)
{
	pStripContext->nSize = sizeof(KMSTRIPCONTEXT);
	pStripContext->StripControl.nListType		 						= KM_OPAQUE_POLYGON;
	pStripContext->StripControl.nUserClipMode	 						= KM_USERCLIP_DISABLE;
	pStripContext->StripControl.nShadowMode		 						= KM_NORMAL_POLYGON;
	pStripContext->StripControl.bOffset									= KM_FALSE;
	pStripContext->StripControl.bGouraud		 						= KM_TRUE;
	pStripContext->ObjectControl.nDepthCompare			 				= KM_GREATER;
	pStripContext->ObjectControl.nCullingMode			 				= KM_NOCULLING;
	pStripContext->ObjectControl.bZWriteDisable							= KM_FALSE;
	pStripContext->ObjectControl.bDCalcControl			 				= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nSRCBlendingMode		= KM_ONE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nDSTBlendingMode		= KM_ZERO;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bSRCSelect				= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bDSTSelect				= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nFogMode				= KM_NOFOG;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bColorClamp			= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bUseAlpha				= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bIgnoreTextureAlpha	= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nFlipUV				= KM_NOFLIP;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nClampUV				= KM_NOCLAMP;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nFilterMode			= KM_BILINEAR;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].bSuperSampleMode		= KM_FALSE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].dwMipmapAdjust			= KM_MIPMAP_D_ADJUST_1_00;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].nTextureShadingMode	= KM_MODULATE;
	pStripContext->ImageControl[KM_IMAGE_PARAM1].pTextureSurfaceDesc	= pTextureDesc;

	kmGenerateStripHead03( pStripHead,pStripContext );
}

/*-------------------------------------------------------------------------------------*
 *                                       MAIN                                          *
 *-------------------------------------------------------------------------------------*/
int Kamui_Init(void)
{
	int Control = 0, Device, Val = 0;

	txt_ClearScreen();		/* Clear text screen */
	PRINTOUT(("** KMBLEND(KAMUI2) : Blending mode test **\n"));

	kmGetVersionInfo(&VersionInfo);
	PRINTOUT(("MajorVersion      0x0%08x\n",VersionInfo.kmMajorVersion));
	PRINTOUT(("LocalVersion      0x0%08x\n",VersionInfo.kmLocalVersion));
	PRINTOUT(("kmFrameBufferSize 0x0%08x\n",VersionInfo.kmFrameBufferSize));

	/*----------------------------------------------------------------------------------- */
	/* Create frame buffer and vertex buffer */
	pFB[0] = &PrimarySurfaceDesc;
	pFB[1] = &BackSurfaceDesc;

	/* VertexBufferPointer(set5) */
	dwDataPtr = (PKMDWORD)(((KMDWORD)syMalloc(0x200000) & 0x0FFFFFFFUL) | 0xA0000000);

	SystemConfig.dwSize					=	sizeof(KMSYSTEMCONFIGSTRUCT);
	SystemConfig.flags					=	KM_CONFIGFLAG_ENABLE_CLEAR_FRAMEBUFFER;
	/* for Frame buffer */
	SystemConfig.ppSurfaceDescArray		=	pFB;		/* Array of SurfaceDesc	pointer	*/
	SystemConfig.fb.nNumOfFrameBuffer	=	2;			/* Number Of Frame Buffer		*/
	/* for Texture Memory */
	SystemConfig.nTextureMemorySize		=	0x100000*2;	/* Texture 2MB					*/
	SystemConfig.nNumOfTextureStruct	=	4096;		/* テクスチャ管理構造体の数		*/
	SystemConfig.nNumOfSmallVQStruct	=	1024;		/* VQテクスチャ管理構造体の数	*/
	SystemConfig.pTextureWork			=	&FBarea[0];	/* テクスチャ管理構造体領域		*/
	/* for Vertex buffer */
	SystemConfig.pBufferDesc			=	&VertexBufferDesc;	/* pointer to VERTEXBUFFDESC	*/
	SystemConfig.nNumOfVertexBank		=	2;
	SystemConfig.pVertexBuffer			=	dwDataPtr;	/* pointer to Vertex buffer		*/
	SystemConfig.nVertexBufferSize		=	0x100000*2;	/* 2MB 							*/
	SystemConfig.nPassDepth				=	1;
	SystemConfig.Pass[0].dwRegionArrayFlag = KM_PASSINFO_AUTOSORT | KM_PASSINFO_ENABLE_Z_CLEAR;
	SystemConfig.Pass[0].fBufferSize[0] =  50.0f;
	SystemConfig.Pass[0].fBufferSize[1] =   0.0f;
	SystemConfig.Pass[0].fBufferSize[2] =  50.0f;
	SystemConfig.Pass[0].fBufferSize[3] =   0.0f;
	SystemConfig.Pass[0].fBufferSize[4] =   0.0f;

	status = kmSetSystemConfiguration( &SystemConfig );
	Check_Status(status, "kmSetSystemConfiguration");

	/*-----------------------------------------------------------------------------------*/
	/* Initialize DisplayString routine */
	txt_InitDisplayString();

	/*----------------------------------------------------------------------------------- */
	/* some initializes */
	SetTextures ();
	//SetBackground (&TexBack);
	SetContext();


    return 0;
}

/*----------------------------------------------------------------------*
 *  Function Name   : ReadMaple                                         *
 *  Inputs          :                                                   *
 *  Outputs         :                                                   *
 *  Returns         :                                                   *
 *  Globals Used    : PosZ, BlendSrc, BlendDst, AlphaSource,            *
 *                    BlendModulate                                     *
 *  Description     : Read MAPLE status                                 *
 *----------------------------------------------------------------------*/
void ReadMaple (void)
{

	unsigned long	Pad[2];		/* maple data								*/

	per = pdGetPeripheral(PDD_PORT_A0);

	/* UP cross key */
	if ( per->on & PDD_DGT_KU )
	{
	}

	/* DOWN cross key */
	if ( per->on & PDD_DGT_KD )
	{
	}

	if ( per->press & PDD_DGT_TA )
	{
	}

		/* B button */
	if ( per->press & PDD_DGT_TB )
	{
	}

	/* X button */
	if ( per->press & PDD_DGT_TX )
	{
	}

	/* Y button */
	if ( per->press & PDD_DGT_TY )
	{
	}
}

/*----------------------------------------------------------------------*
 *  Function Name   : SetContext                                        *
 *  Inputs          :                                                   *
 *  Outputs         :                                                   *
 *  Returns         :                                                   *
 *  Globals Used    :                                                   *
 *  Description     :                                                   *
 *----------------------------------------------------------------------*/
void SetContext (void)
{
	/**************************
	 * KMSTRIPHEADの構築
	 **************************/
//#if 0
	/* VertexType00用 */
//	memset( &StripContext00		, 	0, sizeof(StripContext00));
//	memset( &StripHead00_Gold	,	0, sizeof(StripHead00_Gold));
//	memset( &StripHead00_Trans	,	0, sizeof(StripHead00_Trans));

	InitStripContext00(&StripContext00, &StripHead00_Gold	);
	InitStripContext00(&StripContext00, &StripHead00_Trans	);
//#endif

	/* VertexType03用 */
//	memset( &StripContext03		, 	0, sizeof(StripContext03));
//	memset( &StripHead03_Gold	,	0, sizeof(StripHead03_Gold));
//	memset( &StripHead03_Trans	,	0, sizeof(StripHead03_Trans));

	InitStripContext03(&StripContext03, &StripHead03_Gold, 	&TexGold	);
	InitStripContext03(&StripContext03, &StripHead03_Trans,	&TexTrans	);
}

/*----------------------------------------------------------------------*
 *  Function Name   : SetTextures                                       *
 *  Inputs          :                                                   *
 *  Outputs         :                                                   *
 *  Returns         :                                                   *
 *  Globals Used    : TexOpaq TexTrans TexGold Texback                  *
 *  Description     : Loads BMP files                                   *
 *----------------------------------------------------------------------*/
void SetTextures (void)
{
	PKMDWORD			pTexture01;				/* Pointer to texture		*/
	KMTEXTURETYPE		nTextureType;			/* texture type definition	*/

	/*----------------------------------------------------------------------------------- */
	/* Create texture surface */
	nTextureType = KM_TEXTURE_TWIDDLED | KM_TEXTURE_RGB565;	/* Set texture type */
	status = kmCreateTextureSurface(
		&TexOpaq,				/* OUT	PKMSURFACEDESC	pSurfaceDesc	*/
		256,					/*	IN	KMINT32			nWidth			*/
		256,					/*	IN	KMINT32			nHeight			*/
		nTextureType			/*	IN	KMTEXTURETYPE	nTextureType	*/
	);
	Check_Status(status, "kmCreateTextureSurface #1");
	/*----------------------------------------------------------------------------------- */
	/* Create texture surface */
	nTextureType = KM_TEXTURE_TWIDDLED | KM_TEXTURE_ARGB4444;	/* Set texture type */
	status = kmCreateTextureSurface(
		&TexTrans,				/* OUT	PKMSURFACEDESC	pSurfaceDesc	*/
		256,					/*	IN	KMINT32			nWidth			*/
		256,					/*	IN	KMINT32			nHeight			*/
		nTextureType			/*	IN	KMTEXTURETYPE	nTextureType	*/
	);
	Check_Status(status, "kmCreateTextureSurface #2");
	/*----------------------------------------------------------------------------------- */
	/* Create texture surface */
	nTextureType = KM_TEXTURE_TWIDDLED | KM_TEXTURE_RGB565;	/* Set texture type */
	status = kmCreateTextureSurface(
		&TexBack,				/* OUT	PKMSURFACEDESC	pSurfaceDesc	*/
		256,					/*	IN	KMINT32			nWidth			*/
		256,					/*	IN	KMINT32			nHeight			*/
		nTextureType			/*	IN	KMTEXTURETYPE	nTextureType	*/
	);
	Check_Status(status, "kmCreateTextureSurface #3");
	/*----------------------------------------------------------------------------------- */
	/* Create texture surface */
	nTextureType = KM_TEXTURE_TWIDDLED | KM_TEXTURE_RGB565;	/* Set texture type */
	status = kmCreateTextureSurface(
		&TexGold,				/* OUT	PKMSURFACEDESC	pSurfaceDesc	*/
		128,					/*	IN	KMINT32			nWidth			*/
		128,					/*	IN	KMINT32			nHeight			*/
		nTextureType			/*	IN	KMTEXTURETYPE	nTextureType	*/
	);
	Check_Status(status, "kmCreateTextureSurface #4");
	/*----------------------------------------------------------------------------------- */
	/* Load and set texture */
	pTexture01 = (unsigned long *)&ble_opaq;
	status = kmLoadTexture(	&TexOpaq,							/* PKMSURFACEDESC pSurfaceDesc	*/
							pTexture01 + (16 / sizeof(KMDWORD))	/* PKMDWORD pTexture			*/
							);
	Check_Status(status, "kmLoadTexture #1");
	/*----------------------------------------------------------------------------------- */
	/* Load and set texture */
	pTexture01 = (unsigned long *)&ble_trans;
	status = kmLoadTexture(	&TexTrans,							/* PKMSURFACEDESC pSurfaceDesc	*/
							pTexture01 + (16 / sizeof(KMDWORD))	/* PKMDWORD pTexture			*/
							);
	Check_Status(status, "kmLoadTexture #2");
	/*----------------------------------------------------------------------------------- */
	/* Load and set texture */
	pTexture01 = (unsigned long *)&ble_back;
	status = kmLoadTexture(	&TexBack,							/* PKMSURFACEDESC pSurfaceDesc	*/
							pTexture01 + (16 / sizeof(KMDWORD))	/* PKMDWORD pTexture			*/
							);
	Check_Status(status, "kmLoadTexture #3");
	/*----------------------------------------------------------------------------------- */
	/* Load and set texture */
	pTexture01 = (unsigned long *)&ble_gold;
	status = kmLoadTexture(&TexGold,							/* PKMSURFACEDESC pSurfaceDesc	*/
							pTexture01 + (16 / sizeof(KMDWORD))	/* PKMDWORD pTexture			*/
							);
	Check_Status(status, "kmLoadTexture #4");
}

/*----------------------------------------------------------------------*
 *  Function Name   : NextFrame                                         *
 *  Inputs          :                                                   *
 *  Outputs         :                                                   *
 *  Returns         :                                                   *
 *  Globals Used    :                                                   *
 *  Description     : Render a scene.                                   *
 *----------------------------------------------------------------------*/
void NextFrame (void)
{
	int         tmpSrc;
	int 		tmpDst;
    sdsTriangle MyTri;

	kmBeginScene(&SystemConfig);
	kmBeginPass(&VertexBufferDesc);

	MyTri.x1=0.0f;
	MyTri.y1=0.0f;
	MyTri.z1=0.1f;
	MyTri.vc1= RGBAColour (255,255,0, 128);

	MyTri.x2=640.0f;
	MyTri.y2=0.0f;
	MyTri.z2=0.1f;
	MyTri.vc2= RGBAColour (255,0,255, 255);

	MyTri.x3=640.0f;
	MyTri.y3=480.0f;
	MyTri.z3=0.1f;
	MyTri.vc3= RGBAColour (0,255,255, 128);
	
    kmChangeStripBlendingMode		( &StripHead03_Trans, KM_IMAGE_PARAM1, KM_ZERO,KM_ZERO );	
	kmChangeStripTextureShadingMode	( &StripHead03_Trans, KM_IMAGE_PARAM1, KM_DECAL );	
    DrawGouraudTriangle(MyTri);

	
	kmEndPass(&VertexBufferDesc);
	
	kmRender(KM_RENDER_FLIP);
	
	kmEndScene(&SystemConfig);
	
	Check_Status(status, "kmRender");

	/*----------------------------------------------------------------------------------- */
	Frame++;
}









