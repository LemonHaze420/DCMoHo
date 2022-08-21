//#define _KM_USE_VERTEX_MACRO_L5_ //important
//#include <kamuix.h>
#include <shinobi.h>    /* Shinobi system routines. */
#include <sg_mw.h>
#include <string.h>
#include <kamui2.h>     /* Kamui2 low-level graphics HAL. */
#include <am.h>
#include <sg_chain.h>
//#include <sn_fcntl.h>   /* LibCross file types. */
#include <sg_sd.h>
#include <ac.h>
#include <a64thunk.H>
//#include <usrsnasm.h>   /* LibCross I/O routines. */
#include <sg_syCbl.h>   /* NTSC/RGB/VGA Cable check interface. */
#include <sg_sytmr.h>
#include <sg_sycfg.h>
#include "maincode.h"
#include <stdarg.h>
#include "common.h"
#include "Globals.h"
#include "DCBuildType.h"
#include "DCPlatform.h"
//#include <mw_util.h>
#include <string.h>
#include "dcmembuffer.h"
#include <km2api_tex.h>
#include <sj.h>
#include "cri_adxf.h"
#include "cri_adxt.h"
#include "Dcbackup.h" // VMU - MEMCARD STUFF
#include <kmstatus.h>
#include "dcvmugfx.h"
#include "profile.h"
#define USR_KAMUI_NOWAITVSYNC
//#include "bpamstream.h"
 #define ADXCL 56
 #define ADXCH 63
int ChannelSample[64];
int ChannelPlaying[64];
unsigned long *GNextFreeSndRam;
KTBOOL GSB;
KMBOOL InternalResetCalled=KM_FALSE;
KMBOOL SoftResetAllowed=KM_TRUE;
int     InternalYClip  =480;
int     FailedTextureCount;
#define	USR_INT_PRI			(0x90)
int 	MemBlocksFree[12];
char    SysConfigBuf[16384];
int Abort;
//#define		
//#define kmyMemCopy memcpy
//#define _KM_USE_VERTEX_MACRO_L5_ //important
//#define _KM_USE_VERTEX_MACRO_
//#include <kamuix.h>
//#define kmyMemCopy memcpy

Sint32 	mode, frame, count, latency;
Bool 	ret;
KMDWORD	FBarea[24576 + 19456];
PKMSURFACEDESC		pFB[2];

	
typedef struct {
	Sint32	term_flag;	
	Sint32	disp_flag;	
	Sint32	pause;	
	char	*fname;
	MWPLY	ply;
	MWS_PLY_CPRM_SFD cprm;
} AP_OBJ, *AP;

AP_OBJ ap_obj = {
	OFF,
	OFF,
	OFF,
	"intusa1.SFD",
	NULL,
};

KMBOOL PALMODE=FALSE;
KMBOOL RENDERALLOWED = KM_TRUE;
void *vtxbuf;
AP ap=&ap_obj;
int IX1,IX2,IY1,IY2;
int InternalTrack;
BOOL InternalPlay;
int samplecount;
int sampleinfo[100][2];
unsigned long *SoundMemPointers[100];
 AM_BANK_PTR soundbank;
 AM_SOUND  mysound[24];
 KTU32     Voices[24];
 KTU32  *aicaBuffer;  
int kazam=0;
KMSTRIPHEAD			StripHead;
//#define JCLDCNOPSX
char TextureLoadBucket[512*512*2];
char TextureTwiddleBucket[512*512*2];
SINT NumTPages;
//********************************************************
// SDS Polygon Buckets for all the different Polygon types
//
//********************************************************
				/* hope this is enough tris per frame */
int InternalViewOffset;
//char TextureBucket[256*256*2];
KMSTRIPCONTEXT		StripContext;
KMVERTEX_00         GouraudS      [MaxVerts]; /* Strip of opaque color triangles. */
KMVERTEX_03         TextureS      [MaxVerts]; /* Strip of textured triangles.     */
KMVERTEX_01         TextureBlendS [MaxVerts]; /* Strip of opaque color triangles. */
KMVERTEX_05         TextureAlphaS [MaxVerts]; /* Strip of textured triangles.     */

KMSTRIPHEAD         GouraudHead;             /* Render params for oqaque color polys. */
KMSTRIPHEAD         TextureHead[MaxTextures];             /* Render params for oqaque color polys. */
KMSTRIPHEAD         TextureBlendHead;        /* Render params for oqaque color polys. */
KMSTRIPHEAD         TextureAlphaHead;        /* Render params for oqaque color polys. */

KMSURFACEDESC		TextureSurfaces[MaxTextures];

KMVERTEXBUFFDESC	    VertexBufferDesc;		/* Vertex-buffer				*/	


#define DIRECT  2                       /* Direct mode has been removed from Kamui2.. use 2V, it's faster.  */
#define VERTEXBUFFERSIZE    0x200000    /* Size of vertex buffer, in bytes (to be doubled).1mb */
#define TEXTUREMEMORYSIZE   0x420000    /* VRAM set aside for textures, in bytes (4 MB). */
#define MAXTEXTURES         4096          /* Maximum number of textures to be tracked. */
#define MAXSMALLVQ          1024           /* Maximum number of small VQ textures. */

/* Override Shinobi's syMalloc() / syFree() macros. */
#ifdef syMalloc
#undef syMalloc
#define syMalloc    malloc
#endif /* syMalloc */

#ifdef syFree
#undef syFree
#define syFree      free
#endif /* syFree */

void *(*MallocPtr)(unsigned long) = syMalloc;   /* Default allocation is syMalloc(). */
void (*FreePtr)(void *) = syFree;               /* Default de-allocation is syFree(). */
void PALExtCallbackFunc(PVOID pCallbackArguments); 

#define USE_AUDIO64AC
//#define USR_INT_PRI

#define FORCE_LEVEL
extern long *kmiDeviceExtension;    /* External reference to Kamui2 device driver. */
AC_ERROR_PTR bpAcError = KTNULL;
AM_ERROR *bpAmError = KTNULL;
KMSTRIPCONTEXT DefaultContext =     /* Standard opaque polygon. */
{
    0x90, {KM_OPAQUE_POLYGON, KM_USERCLIP_DISABLE, KM_NORMAL_POLYGON,
    KM_PACKEDCOLOR, KM_FALSE, KM_TRUE}, {KM_GREATEREQUAL, KM_CULLSMALL, KM_FALSE,
    KM_TRUE, 0}, {0, 0}, {KM_ONE, KM_ZERO, KM_FALSE, KM_FALSE, KM_NOFOG,
    KM_FALSE, KM_FALSE, KM_FALSE, KM_NOFLIP, KM_CLAMP_UV, KM_BILINEAR,
    KM_FALSE, KM_MIPMAP_D_ADJUST_1_25, KM_MODULATE, 0, 0}
};

KMPACKEDARGB    	Border;         		/* Border color packed ARGB value. */

KMSURFACEDESC		PrimarySurfaceDesc;		/* Primary frame buffer			*/
KMSURFACEDESC		BackSurfaceDesc;		/* Back-buffer frame buffer		*/
//KMVERTEXBUFFDESC	    VertexBufferDesc;		/* Vertex-buffer */
KMINT32             UsrRenderId[2];
PKMDWORD			dwDataPtr;
/* Kamui2 texture work area, must be aligned to 32-byte boundary. */
#ifdef  __MWERKS__
#pragma align (64);
KMDWORD
#else
#ifdef __GNUC__
KMDWORD __attribute__ ((aligned (32)))
#else
#pragma aligndata32(TextureWorkArea)
KMDWORD
#endif /* __GNUC__ */
#endif /* _MWERKS__ */
TextureWorkArea[MAXTEXTURES * 24 / 4 + MAXSMALLVQ * 76 / 4];

#ifdef  __MWERKS__
#pragma align (4);	/* Return Metrowerks alignment to 4-bytes. */
#endif

/* Set up 32-byte aligned malloc() / free() memory management. */
#define Align32Malloc   (*MallocPtr)
#define Align32Free     (*FreePtr)
/* Align a pointer to the nearest 32-byte aligned memory address. */
#define Align32Byte(ADR)        ((((long) ADR) + 0x1F) & 0xFFFFFFE0)
/* Reference physical memory through a cached memory address (SH4 P1 memory region). */
#define SH4_P1CachedMem(ADR)    ((((long) ADR) & 0x0FFFFFFF) | 0x80000000)
/* Reference physical memory through a non-cached memory address (SH4 P2 memory region). */
#define SH4_P2NonCachedMem(ADR) ((((long) ADR) & 0x0FFFFFFF) | 0xA0000000)
/* Pack red, green, blue, and alpha values into a 32-bit RGBA color word. */
#define PackRGBA(R, G, B, A)  ((unsigned long) (((A) << 24) | ((R) << 16) | ((G) << 8) | (B)))

#define _TWIDDLEADR(COORD)      ((COORD & 1)             | ((COORD >> 1) & 1) << 2   |  \
                                ((COORD >> 2) & 1) << 4  | ((COORD >> 3) & 1) << 6   |  \
                                ((COORD >> 4) & 1) << 8  | ((COORD >> 5) & 1) << 10  |  \
                                ((COORD >> 6) & 1) << 12 | ((COORD >> 7) & 1) << 14  |  \
                                ((COORD >> 8) & 1) << 16 | ((COORD >> 9) & 1) << 18)

#define TwiddledOffset(X, Y)    ((_TWIDDLEADR(X) << 1) | _TWIDDLEADR(Y))



int					VPointer;
KMSYSTEMCONFIGSTRUCT kmsc;                  /* Kamui2 system configuration structure. */
KMVERSIONINFO       VersionInfo;            /* Kamui2 version information. */
KMSURFACEDESC       TexSurfaceDesc;         /* Texture surface. */
PKMSURFACEDESC      FBSurfaces[2];          /* Array of pointers to FB surfaces. */
PKMDWORD            VertexBufferPtr;        /* Pointer to user-allocated vertex buffer. */
PKMDWORD            TexturePtr;             /* Pointer to texture. */
PKMDWORD            TwiddledPtr;            /* Pointer to twiddled texture. */
int                 li;
float               Red = 0.0f, Green = 0.0f, Blue = 0.0f;      /* Gradient colors. */
float               ColorDelta = 0.01f;


//*********************************************************************************************************************
BOOL	tex_alpha[MaxTextures];

BOOL	IsTextureAlpha(SINT tn)
{
	return tex_alpha[tn];
}

//*********************************************************************************************************************
//*********************************************************************************************************************
//*********************************************************************************************************************
// SDS GLOBAL VARS

char                TmpString[80];
UWORD				*Dscreen;
char				TmpTexture[640*480*2];
GDFS				IFH;  // Internal File Handle!
void SDSSwitchClippingOn(void)
{
 	kmChangeStripUserClipMode(&GouraudHead,KM_USERCLIP_INSIDE); 	
 	
 	for (int a=0;a<MaxTextures;a++)
 	{

 	kmChangeStripUserClipMode(&TextureHead[a],KM_USERCLIP_INSIDE); 	
 	}
}

void SDSSwitchClippingOff(void)
{
 	kmChangeStripUserClipMode(&GouraudHead,KM_USERCLIP_DISABLE); 	
 	
 	for (int a=0;a<MaxTextures;a++)
 	{

 	kmChangeStripUserClipMode(&TextureHead[a],KM_USERCLIP_DISABLE); 	
 	}
}

void SDSFullScreenNoClipping(void)
{
kmSetRegionClipping((0)>>5,0,(640)>>5,(InternalYClip)>>5);
kmSetPixelClipping(0,0,640,InternalYClip);
}

void SDSSetClipRegion(int x1,int y1,int x2,int y2)
{
 y2=InternalYClip;
 
 KMRECT MRECT= {x1>>5,y1>>5,x2>>5,y2>>5};
 
 //kmSetPixelClipping(x1,y1,x2,y2);
 kmSetUserClipping(&VertexBufferDesc,KM_USERCLIP_DISABLE,KM_TRANS_POLYGON,  &MRECT);
 kmSetUserClipping(&VertexBufferDesc,KM_USERCLIP_DISABLE,KM_OPAQUE_POLYGON, &MRECT);				



// kmSetRegionClipping(x1,y1,x2,y2);
 InternalViewOffset=((x2-x1)/2)+x1;

 IX1=x1;
 IY1=y1;
 IX2=x2;
 IY2=y2; 
 
// IX1 = IX2 = ((x2 - x1) / 2) + x1;
// IY1 = IY2 = ((y2 - y1) / 2) + y1;
}	
void SDSNoClearBuffer(void)
{
    kmsc.flags  =     
                 KM_CONFIGFLAG_NOWAITVSYNC                 /* Remove internal render wait. */
                | KM_CONFIGFLAG_ENABLE_2V_LATENCY;          /* Use 2V latency render model. */
  // kmSetSystemConfiguration (&kmsc);	

}

void SDSClearBuffer(void)
{
    kmsc.flags  = KM_CONFIGFLAG_ENABLE_CLEAR_FRAMEBUFFER    /* Clear FB at initialization. */
                | KM_CONFIGFLAG_NOWAITVSYNC                 /* Remove internal render wait. */
                | KM_CONFIGFLAG_ENABLE_2V_LATENCY;          /* Use 2V latency render model. */
   ///kmSetSystemConfiguration (&kmsc);	
}

void VSyncOn(void)
{
    kmsc.flags  = KM_CONFIGFLAG_ENABLE_CLEAR_FRAMEBUFFER    /* Clear FB at initialization. */
                //| KM_CONFIGFLAG_NOWAITVSYNC                 /* Remove internal render wait. */
                | KM_CONFIGFLAG_ENABLE_2V_LATENCY;          /* Use 2V latency render model. */
   kmSetSystemConfiguration (&kmsc);	
}

Void VSyncOff(void)
{
    kmsc.flags  = KM_CONFIGFLAG_ENABLE_CLEAR_FRAMEBUFFER    /* Clear FB at initialization. */
                | KM_CONFIGFLAG_NOWAITVSYNC                 /* Remove internal render wait. */
                | KM_CONFIGFLAG_ENABLE_2V_LATENCY;          /* Use 2V latency render model. */
   kmSetSystemConfiguration (&kmsc);	
}

SINT	sDCLoadPartFile(ULONG *buffer,SINT len)
{
  int FileBlocks;
//  gdFsGetFileSctSize (gdfs, &FileBlocks);
  gdFsRead (IFH, ((len+2047)&~2047)/2048, buffer);
  return len;

}
SINT	sDCOpenFile(char *fname)
{
char File[256];
char Path[256];
char nfname[256];
long fsize;
// Arrrgghhhh have to write a wrapper around this to handle the absolute path name problem
	size_t z;
	//sprintf(nfname,"Loading %s",fname);
	//mw_pr(nfname);
	z=strlen(fname);
	while (z!=0 && fname[z]!='\\')
		 z--;
	if (z!=0) 
	{
	strcpy(Path,"");
	int x;
	for (x=0;x<z;x++)
	 Path[x]=fname[x]; 
	 Path[x]='\0';
	//mw_pr(Path);
	int c=0;
	for (x=z+1;x<strlen(fname);x++)
	 File[c++]=fname[x];
	 File[c]='\0';
	// mw_pr(File);
	if (Path[0]!='\\')
	{
	for (x=strlen(Path);x>=0;x--)
	{ Path[x+1]=Path[x];}
	Path[0]='\\';
	}
	SDSChangeDir(Path);	  
	//mw_pr(Path);
	strcpy(fname,File);
	}	 



//mw_pr(fname);
IFH=gdFsOpen (fname, NULL); 
gdFsGetFileSize(IFH,&fsize);
return fsize;
}

SINT	sDCCloseFile(void)
{
gdFsClose(IFH);
}


void *_Align32Malloc (unsigned long Size)
{
    void *Ptr, *AlignedPtr;
    unsigned long MFree,MSize;
  

    
//	sprintf(name, "Malloc Called - %03d requested", Size);    
//	mw_pr(name);    
   /*
    syMallocStat(&MFree,&MSize);
    
	sprintf(name, "NFree %03d", MFree);
	mw_pr(name);
	
	sprintf(name, "BFree %03d", MSize);
	mw_pr(name);	
	*/
	/*
	*/
	
    Size = (Size + 0x1F) & 0xFFFFFFE0;
    Ptr = syMalloc (Size + 32);
    AlignedPtr = (void *) ((((long) Ptr) + 0x20) & 0xFFFFFFE0);
    *((char *) AlignedPtr - 1) = (char) ((long) AlignedPtr - (long) Ptr);
    
   // AlignedPtr = syMalloc (Size);
    return (AlignedPtr);
}

void _Align32Free (void *Ptr)
{
    char Diff;

	//sprintf(name, "Malloc Free Called ",0);    
	//mw_pr(name);

    /* Read cookie and adjust pointer back to original unaligned address before freeing. */
    
    Diff = *((char *) Ptr - 1);
    Ptr = (void *) ((long) Ptr - Diff);
    

    syFree (Ptr);
}

void Init32Malloc (void)
{
    void *Ptr1, *Ptr2;

    /* Use syMalloc() / syFree() by default. */
    MallocPtr = syMalloc;
    FreePtr = syFree;

    Ptr1 = syMalloc (1);
    Ptr2 = syMalloc (1);

    /* Test if either allocation was not 32-byte aligned. */
   // if (((long) Ptr1 & 0x1F) || ((long) Ptr2 & 0x1F))
    {
        MallocPtr = _Align32Malloc;
        FreePtr = _Align32Free;
    }

    syFree (Ptr1);
    syFree (Ptr2);
    //mw_pr("Init Malloc Called");
    
}

void Rectangle2Twiddled (char *Rectangle, char *Twiddled, int xD,int yD, int BitsPerPixel)
{
    int x, y;
    char *In = Rectangle, *Out;

    /* Convert BitsPerPixel into a shift value.. 8-bit = shift by 0, 16-bit = shift by 1. */
    BitsPerPixel = BitsPerPixel >> 4;

    for (y = 0; y < yD; y++)
    {
        for (x = 0; x < xD; x++)
        {
            /* TwiddledOffset macro converts coordinate into twiddled texture offset. */
            Out = Twiddled + ((TwiddledOffset (x, y)) << BitsPerPixel);

            /* Transfer pixel from Rectangle texture to Twiddled. */
            *Out = *In;
            if (BitsPerPixel > 0)           /* Account for 16-bit data. */
                *(Out + 1) = *(In + 1);
            In += BitsPerPixel + 1;
        }
    }
}
/*
LoadTextureFile (char *Filename)
{
}
*/
PKMDWORD LoadTextureFile (char *Filename)
{
    PKMDWORD    TPtr;
    GDFS        gdfs;
    long        FileBlocks;

    /* Open input file. */
    if (!(gdfs = gdFsOpen (Filename, NULL))) return NULL;

    /* Get file size (in blocks/sectors). */
    gdFsGetFileSctSize (gdfs, &FileBlocks);

    /* Allocate memory to nearest block size (2048 bytes). */
    TPtr = (unsigned long *)Align32Malloc (FileBlocks * 2048);

    /* Read file to memory region (Destination address must be 32-byte aligned). */
    gdFsReqRd32 (gdfs, FileBlocks, TPtr);

    /* Wait for file access to finish. */
    while (gdFsGetStat (gdfs) != GDD_STAT_COMPLETE)
        ;

    /* Close file. */
    gdFsClose (gdfs);

    return TPtr;
}

#pragma dont_inline on
VOID FatalErrorCallBack(PVOID pCallbackArguments)
{
Abort=1;
    //sbExitSystem();
}

void errfunc(void *obj, Sint32 err)
{
    if (err == GDD_ERR_TRAYOPEND || err == GDD_ERR_UNITATTENT)
    {
      Abort = 1;
      InternalResetCalled=KM_TRUE;
      if (SoftResetAllowed==KM_TRUE) SDSSoftReset();
    }
}


#pragma dont_inline off


VOID PALExtCallbackFunc(PVOID pCallbackArguments)
{
PKMPALEXTINFO	pInfo;

	pInfo = (PKMPALEXTINFO)pCallbackArguments;
	pInfo->nPALExtMode = KM_PALEXT_HEIGHT_RATIO_1_133;	   //for PAL extended mode
	return;
}


void CreateTexturePageFile(int TextureNumber,char *filename,int w,int h)
{
    /* Create a texture surface. */
    kmCreateTextureSurface (&TextureSurfaces[TextureNumber], w, h,(KM_TEXTURE_RECTANGLE | KM_TEXTURE_RGB565));

    TexturePtr = LoadTextureFile (filename);

    /* Load a texture file from disk. */
//    TexturePtr = LoadTextureFile ("FBI.pvr");
    /* Allocate memory for twiddled texture (256x256 16-bit). */
//    TwiddledPtr = (PKMDWORD) Align32Malloc (256 * 256 * 2);
    /* Run-time twiddle source rectangle texture + 16 bytes to skip PVRT chunk header. */
    //Rectangle2Twiddled ((char *) &TextureLoadBucket, (char *) TwiddledPtr, 256, 256, 16);
    /* Transfer twiddled texture from system mem to video mem using a DMA transfer. */
    
    kmLoadTexture (&TextureSurfaces[TextureNumber], (unsigned long *)TexturePtr);
    
    DefaultContext.ImageControl[KM_IMAGE_PARAM1].pTextureSurfaceDesc = &TextureSurfaces[TextureNumber];
   
    memset ((unsigned char *)&TextureHead[TextureNumber], 0, sizeof(TextureHead[TextureNumber]));
    kmGenerateStripHead03 (&TextureHead[TextureNumber], &DefaultContext);


    /* Set the texture surface of the opaque polygon context to the newly loaded texture. */
//_Align32Free (TwiddledPtr);
}

void CreateTexturePage(int TextureNumber,int flag,int xsize,int ysize)
{
	SINT	retval = KMSTATUS_INVALID_TEXTURE_TYPE;

	/* Create a texture surface. */
	if (((flag & 2)==0) && ((flag & 1)==0))  retval = kmCreateTextureSurface (&TextureSurfaces[TextureNumber], xsize, ysize,(KM_TEXTURE_TWIDDLED_RECTANGLE | KM_TEXTURE_RGB565));
	if (((flag & 2)==0) && ((flag & 1)==1))  retval = kmCreateTextureSurface (&TextureSurfaces[TextureNumber], xsize, ysize,(KM_TEXTURE_TWIDDLED_RECTANGLE | KM_TEXTURE_ARGB4444));
	if (((flag & 2)==2) && ((flag & 1)==0))  retval = kmCreateTextureSurface (&TextureSurfaces[TextureNumber], xsize, ysize,(KM_TEXTURE_VQ | KM_TEXTURE_RGB565));
	if (((flag & 2)==2) && ((flag & 1)==1))  retval = kmCreateTextureSurface (&TextureSurfaces[TextureNumber], xsize, ysize,(KM_TEXTURE_VQ | KM_TEXTURE_ARGB4444));
	if (((flag & 4)==4) && ((flag & 2)==2) && ((flag & 1)==0))  retval = kmCreateTextureSurface (&TextureSurfaces[TextureNumber], xsize, ysize,(KM_TEXTURE_VQ_MM | KM_TEXTURE_RGB565));
	if (((flag & 4)==4) && ((flag & 2)==2) && ((flag & 1)==1))  retval = kmCreateTextureSurface (&TextureSurfaces[TextureNumber], xsize, ysize,(KM_TEXTURE_VQ_MM | KM_TEXTURE_ARGB4444));

	if (retval != KMSTATUS_SUCCESS)
	{
		FailedTextureCount ++;
	}
	else
	{
		tex_alpha[TextureNumber] = (flag & 1);
		
		/* Run-time twiddle source rectangle texture + 16 bytes to skip PVRT chunk header. */
		if (!((flag & 2)==2) && !((flag & 4)==4))
		{
		Rectangle2Twiddled ((char *) &TextureLoadBucket[0], (char *) &TextureTwiddleBucket[0], xsize, ysize, 16);
		if ((kmLoadTexture (&TextureSurfaces[TextureNumber], (unsigned long *)&TextureTwiddleBucket[0])!=KMSTATUS_SUCCESS)) FailedTextureCount++;
		}
		else
		{
		//Rectangle2Twiddled ((char *) &TextureLoadBucket[0], (char *) &TextureTwiddleBucket[0], xsize, ysize, 16);
		if ((kmLoadTexture (&TextureSurfaces[TextureNumber], (unsigned long *)&TextureLoadBucket[16])!=KMSTATUS_SUCCESS)) FailedTextureCount++;	
		}
	}
	
	DefaultContext.ImageControl[KM_IMAGE_PARAM1].pTextureSurfaceDesc = &TextureSurfaces[TextureNumber];
	DefaultContext.ImageControl[KM_IMAGE_PARAM1].bUseAlpha = KM_TRUE;   
	memset ((unsigned char *)&TextureHead[TextureNumber], 0, sizeof(TextureHead[TextureNumber]));
	kmGenerateStripHead03 (&TextureHead[TextureNumber], &DefaultContext);
	
	if ((flag & 1)==1)
	{
	kmChangeStripBlendingMode       ( &TextureHead[TextureNumber], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
	kmChangeStripTextureShadingMode ( &TextureHead[TextureNumber], KM_IMAGE_PARAM1, KM_MODULATE);	
	kmChangeStripUseAlpha			( &TextureHead[TextureNumber], KM_IMAGE_PARAM1, KM_TRUE ); 	
	}
}


void SDSFreeAllTextures(void)
{
	//NOOOOOOOOOOO
	SINT	c0;
	for (c0 = 0; c0 < 10; c0 ++)
	{

	BOOL zam;
	
	SINT 	q = SDSGetFreeTextureMemory();
	SINT 	w = q;


	//kmResetRenderer();
	kmGarbageCollectTexture();

//	for (int a=0;a<MaxTextures;a++)
	for (int a = MaxTextures - 1; a >= 0; a --)
	{
 		if (TextureSurfaces[a].pSurface!=0)
 		{
 			SINT 	rval;
	  		rval = kmFreeTexture(&TextureSurfaces[a]);
  			if (rval != KMSTATUS_SUCCESS)
  			{
  				SINT a = rand();
  				SINT b = a;
  			}
  		}
 		
 		TextureSurfaces[a].pSurface = NULL;
	}
 	//kmGarbageCollectTexture();
 	
 	
	SINT 	i = SDSGetFreeTextureMemory();
	
	SINT j = i;
	
	} 	
}

void *InitAll(void)
{

#ifdef __GNUC__
	shinobi_workaround();
#endif


// Phew Lets try and start up this game!!!!!!!!!!!!!!!!!!!!!


    /* Check the cable for NTSC/PAL or VGA.. works properly for SCART. */
    switch (syCblCheck())
	{
        /* Initialize the display device and set the frame buffer based on the video mode. */
        case SYE_CBL_NTSC:  /* U.S./North America NTSC (hHz) and Brazil PAL-M (60Hz). */
        mode=KM_DSPMODE_NTSCNI640x480;
            sbInitSystem (KM_DSPMODE_NTSCNI640x480, KM_DSPBPP_RGB565, 1);
            break;
        case SYE_CBL_PAL:   /* Europe PAL (50Hz) and Argentina PAL-N (50Hz). */
        mode=KM_DSPMODE_PALNI640x480EXT;
            sbInitSystem (KM_DSPMODE_PALNI640x480EXT, KM_DSPBPP_RGB565, 1);

			/*	Change the PAL height ratio */
			kmSetPALEXTCallback(PALExtCallbackFunc,NULL);
			kmSetDisplayMode (KM_DSPMODE_PALNI640x480EXT, KM_DSPBPP_RGB565, TRUE, FALSE);
		mode=KM_DSPMODE_PALNI640x480EXT;

            break;
        case SYE_CBL_VGA:   /* Standard VGA. */
        mode=KM_DSPMODE_VGA640x480;
            sbInitSystem (KM_DSPMODE_VGA640x480, KM_DSPBPP_RGB565, 1);
            break;
        default:
            syBtExit();     /* Unknown video type, return to Dreamcast BootROM. */
	}

	//gdFsEntryErrFuncAll((void *) ApGdErrFunc,(void *) ap);

	kmSetFatalErrorCallback(FatalErrorCallBack,NULL);


    Init32Malloc();
	
    #if 1
        kmGetVersionInfo (&VersionInfo);
    #else
        VersionInfo.kmMajorVersion    = *kmiDeviceExtension;
        VersionInfo.kmLocalVersion    = ((*(kmiDeviceExtension + 1) & 0xFF) << 24);
        VersionInfo.kmLocalVersion   |= ((*(kmiDeviceExtension + 2) & 0xFF) << 16);
        VersionInfo.kmLocalVersion   |= (*(kmiDeviceExtension + 3) & 0xFFFF);
        VersionInfo.kmFrameBufferSize = *((long *) ((long) &kmiDeviceExtension + 24));
    #endif


    /* Set size of system configuration struct, required. */
    kmsc.dwSize = sizeof(KMSYSTEMCONFIGSTRUCT);

    /* Render control flags. */
    kmsc.flags  = KM_CONFIGFLAG_ENABLE_CLEAR_FRAMEBUFFER    /* Clear FB at initialization. */
                | KM_CONFIGFLAG_NOWAITVSYNC                 /* Remove internal render wait. */
                | KM_CONFIGFLAG_ENABLE_2V_LATENCY;          /* Use 2V latency render model. */
	/* VertexBufferPointer(set5) */
	dwDataPtr =	(PKMDWORD)(
		((KMDWORD)syMalloc(VERTEXBUFFERSIZE * 2) & 0x0FFFFFFFUL) | 0xA0000000 );

    /* Frame buffer surfaces information. */
    //FBSurfaces[0] = &PrimarySurfaceDesc;
    //FBSurfaces[1] = &BackSurfaceDesc;
    
   	pFB[1] = &PrimarySurfaceDesc;
	pFB[0] = &BackSurfaceDesc;
	
    kmsc.ppSurfaceDescArray = pFB;   /* Set frame buffer surface description array. */
    kmsc.fb.nNumOfFrameBuffer = 2;          /* Number of frame buffers (double buffered). */
    kmsc.fb.nStripBufferHeight = 32;        /* Strip buffer height = 32xN pixels, if used. */

    /* Texture handler setup. */
    kmsc.nTextureMemorySize = TEXTUREMEMORYSIZE;    /* Texture VRAM, divisible by 32 bytes. */
    kmsc.nNumOfTextureStruct = MAXTEXTURES;         /* Maximum number of textures tracked. */
    kmsc.nNumOfSmallVQStruct = MAXSMALLVQ;          /* Max. number of small VQ textures. */
    kmsc.pTextureWork = &FBarea[0];            /* Texture work area in system memory. */

    /* Generate 32-byte aligned vertex buffer (double-buffered). */
   // VertexBufferPtr = (PKMDWORD) Align32Malloc (2 * VERTEXBUFFERSIZE);
    //* Kamui2 requires the vertex buffer region to be 32-byte aligned and non-cacheable. */
    //kmsc.pVertexBuffer = (PKMDWORD) SH4_P2NonCachedMem (dwDataPtr);
	kmsc.pVertexBuffer		 = dwDataPtr;
    kmsc.nVertexBufferSize = VERTEXBUFFERSIZE ;  /* Size of vertex buffer X2, in bytes. */
    kmsc.pBufferDesc = &VertexBufferDesc;   /* Struct used to maintain vertex buffer info. */
    kmsc.nNumOfVertexBank = 2;              /* Number of vertex buffers (double buffered). */

    /* Set multi-pass rendering information. */
    kmsc.nPassDepth = 3;                                    /* Number of passes. */
    kmsc.Pass[0].dwRegionArrayFlag = KM_PASSINFO_AUTOSORT   /* Autosort translucent polys. */
        | KM_PASSINFO_ENABLE_Z_CLEAR;                       /* Clear Z buffer. */
    kmsc.Pass[0].nDirectTransferList = KM_OPAQUE_POLYGON;   /* Type sent direct in 2V mode. */

    /* Percent of vertex buffer used for each vertex type in a pass (must total 100%). */
    kmsc.Pass[0].fBufferSize[0] = 0.0f;        /* Opaque polygons (0% if sent direct). */
    kmsc.Pass[0].fBufferSize[1] =  0.0f;        /* Opaque modifier. */
    kmsc.Pass[0].fBufferSize[2] = 100.0f;        /* Translucent. */
    kmsc.Pass[0].fBufferSize[3] =  0.0f;        /* Translucent modifier. */
    kmsc.Pass[0].fBufferSize[4] = 0.0f;        /* Punchthrough. */

    /* Set system configuration. */
    kmSetSystemConfiguration (&kmsc);

    /* Set screen border color. */
    Border.dwPacked = 0x0000;
    kmSetBorderColor (Border);


    /* Generate a strip head array for opaque color (no texture) polygons. */
    memset ((unsigned char *)&GouraudHead, 0, sizeof(GouraudHead));
    kmGenerateStripHead00 (&GouraudHead, &DefaultContext);
    
//    memset ((unsigned char *)&TextureHead[254], 0, sizeof(TextureHead));
//    kmGenerateStripHead05 (&TextureHead[254], &DefaultContext);


    /* Set the texture surface of the opaque polygon context to the newly loaded texture. */
    DefaultContext.ImageControl[KM_IMAGE_PARAM1].pTextureSurfaceDesc = &TexSurfaceDesc;


    /* Modify the standard polygon context to support translucency. */
    DefaultContext.StripControl.nListType = KM_TRANS_POLYGON;
    DefaultContext.ImageControl[KM_IMAGE_PARAM1].bUseAlpha = KM_FALSE;
    DefaultContext.ImageControl[KM_IMAGE_PARAM1].nTextureShadingMode = KM_DECAL;
    
    
    return (void *)&VertexBufferDesc;

}





void GTQuad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,float alpha)
{

VPointer=0;

        kmStartStrip (&VertexBufferDesc, &TextureHead[0]);
      
        TextureS[0+VPointer].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[0+VPointer].fX                   = x1;
        TextureS[0+VPointer].fY                   = y1;
        TextureS[0+VPointer].u.fZ                 =  10.0f;


        TextureS[0+VPointer].fU         	      =   0.0f;
        TextureS[0+VPointer].fV            		  =   0.0f;        

        TextureS[1+VPointer].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[1+VPointer].fX                   = x2;
        TextureS[1+VPointer].fY                   = y2;
        TextureS[1+VPointer].u.fZ                 =  10.0f;

        TextureS[1+VPointer].fU         	      =   1.0f;
        TextureS[1+VPointer].fV            		  =   0.0f;           

        TextureS[2+VPointer].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[2+VPointer].fX                   = x3;
        TextureS[2+VPointer].fY                   = y3;
        TextureS[2+VPointer].u.fZ                 =  10.0f;
     

        TextureS[2+VPointer].fU         	      =   0.0f;
        TextureS[2+VPointer].fV            		  =   1.0f;           
        
        TextureS[3+VPointer].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
        TextureS[3+VPointer].fX                   = x4;
        TextureS[3+VPointer].fY                   = y4;
        TextureS[3+VPointer].u.fZ                 =  10.0f;
        

        TextureS[3+VPointer].fU         	      =   1.0f;
        TextureS[3+VPointer].fV            		  =   1.0f;   

        for (int i=0; i<=3; i++)
        	kmSetVertex (&VertexBufferDesc, &TextureS[i+VPointer], KM_VERTEXTYPE_03, sizeof (KMVERTEX_03));
		VPointer+=4;        	
		

}

void GTQuadN(int n,float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,int a1,int a2,int a3,int a4)
{
VPointer=0;

        kmStartStrip (&VertexBufferDesc, &TextureHead[n]);
      
        TextureS[0+VPointer].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[0+VPointer].fX                   = x1;
        TextureS[0+VPointer].fY                   = y1;
        TextureS[0+VPointer].u.fZ                 =  10.0f;
        TextureS[0+VPointer].uBaseRGB.dwPacked    =   (a1<<24)+0xffffff;        
     
        TextureS[0+VPointer].fU         	      =   0.0f;
        TextureS[0+VPointer].fV            		  =   0.0f;        

        TextureS[1+VPointer].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[1+VPointer].fX                   = x2;
        TextureS[1+VPointer].fY                   = y2;
        TextureS[1+VPointer].u.fZ                 =  10.0f;
        TextureS[1+VPointer].uBaseRGB.dwPacked    =  (a2<<24)+0xffffff;
        TextureS[1+VPointer].fU         	      =   1.0f;
        TextureS[1+VPointer].fV            		  =   0.0f;           

        TextureS[2+VPointer].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[2+VPointer].fX                   = x3;
        TextureS[2+VPointer].fY                   = y3;
        TextureS[2+VPointer].u.fZ                 =  10.0f;

        TextureS[2+VPointer].uBaseRGB.dwPacked    =   (a3<<24)+0xffffff;
        TextureS[2+VPointer].fU         	      =   0.0f;
        TextureS[2+VPointer].fV            		  =   1.0f;           
        
        TextureS[3+VPointer].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
        TextureS[3+VPointer].fX                   = x4;
        TextureS[3+VPointer].fY                   = y4;
        TextureS[3+VPointer].u.fZ                 =  10.0f;
        TextureS[3+VPointer].uBaseRGB.dwPacked    =   (a4<<24)+0xffffff;
        TextureS[3+VPointer].fU         	      =   1.0f;
        TextureS[3+VPointer].fV            		  =   1.0f;   

        for (int i=0; i<=3; i++)
        	kmSetVertex (&VertexBufferDesc, &TextureS[i+VPointer], KM_VERTEXTYPE_03, sizeof (KMVERTEX_03));
		VPointer+=4;        	
		

}


void GQuad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{


        kmStartStrip (&VertexBufferDesc, &GouraudHead);
      

        GouraudS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        GouraudS[0].fX                   = x1;
        GouraudS[0].fY                   = y1;
        GouraudS[0].u.fZ                 =  -1.0f;


        GouraudS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        GouraudS[1].fX                   = x2;
        GouraudS[1].fY                   = y2;
        GouraudS[1].u.fZ                 =  -1.0f;


        GouraudS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        GouraudS[2].fX                   = x3;
        GouraudS[2].fY                   = y3;
        GouraudS[2].u.fZ                 =  -1.0f;

        
        GouraudS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
        GouraudS[3].fX                   = x4;
        GouraudS[3].fY                   = y4;
        GouraudS[3].u.fZ                 =  -1.0f;
       

        for (int i=0; i<=3; i++)
        	kmSetVertex (&VertexBufferDesc, &GouraudS[i], KM_VERTEXTYPE_00, sizeof (KMVERTEX_00));
		VPointer+=4;        	
		

}

void GTriCZ(float x1,float y1,float x2,float y2,float x3,float y3,int vc1,int vc2,int vc3,float z1,float z2,float z3)
{


if (x1>-32 && x2>-32 && x3>-32) 
if (x1<640+32 && x2<640+32 && x3<640+32)
if (y1>-32 && y2>-32 && y3>-32)
if (y1<480+32 && y2<480+32 && y3<480+32)
{

        kmStartStrip (&VertexBufferDesc, &GouraudHead);
	    VPointer=0;      

        GouraudS[0+VPointer].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        GouraudS[0+VPointer].fX                   = x1;
        GouraudS[0+VPointer].fY                   = y1;
        GouraudS[0+VPointer].u.fZ                 = -z1;
        GouraudS[0+VPointer].uBaseRGB.dwPacked =vc1;        


        GouraudS[1+VPointer].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        GouraudS[1+VPointer].fX                   = x2;
        GouraudS[1+VPointer].fY                   = y2;
        GouraudS[1+VPointer].u.fZ                 =  -z2;
        GouraudS[1+VPointer].uBaseRGB.dwPacked  = vc2;            
        

        GouraudS[2+VPointer].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
        GouraudS[2+VPointer].fX                   = x3;
        GouraudS[2+VPointer].fY                   = y3;
        GouraudS[2+VPointer].u.fZ                 =  -z3;
        GouraudS[2+VPointer].uBaseRGB.dwPacked  = vc3;            
      

        for (int i=0; i<=2; i++)
        	//kmSetVertex (&VertexBufferDesc, &GouraudS[i+VPointer], KM_VERTEXTYPE_00, sizeof (KMVERTEX_00));
        	
		VPointer+=3;        	
		
		}

}


void InitCZTs(int Tpage)
{
 kmStartStrip (&VertexBufferDesc, &TextureHead[Tpage]);
}

void ExitCZTs(int Numv)
{
int size;
size =sizeof (KMVERTEX_03);

 for (int i=0; i<Numv; i++)
   	kmSetVertex (&VertexBufferDesc, &TextureS[i], KM_VERTEXTYPE_03, size);
   	
}


void InitCZs(void)
{
 kmStartStrip (&VertexBufferDesc, &GouraudHead);
}

void ExitCZs(int Tpage)
{

 for (int i=0; i<Tpage; i++)
   	kmSetVertex (&VertexBufferDesc, &GouraudS[i], KM_VERTEXTYPE_00, sizeof (KMVERTEX_00));
   	
}

void GTriZT(int Tpage,float x1,float y1,float x2,float y2,float x3,float y3,float z1,float z2,float z3,float u1,float v1,float u2,float v2,float u3,float v3)
{

if (Tpage>=0 && Tpage<(NumTPages+1))
//if (((x1>-32) && (x1<(640+32))) || ((x2>-32) && (x2<(640+32))) || ((x3>-32) && (x3<(640+32))))
//if (((y1>-32) && (y1<(480+32))) || ((y2>-32) && (y2<(480+32))) || ((y3>-32) && (y3<(480+32))))
//if ((z1<0) && (z2<0) && (z3<0))
{


        kmStartStrip (&VertexBufferDesc, &TextureHead[Tpage]);
      

        TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[0].fX                   = x1;
        TextureS[0].fY                   = y1;
        TextureS[0].u.fZ                 = -z1;
        TextureS[0].uBaseRGB.dwPacked           =   0xffffffff;        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        


        TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[1].fX                   = x2;
        TextureS[1].fY                   = y2;
        TextureS[1].u.fZ                 =  -z2;
        TextureS[1].uBaseRGB.dwPacked           =   0xffffffff;        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v2;         
       

        TextureS[2].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
        TextureS[2].fX                   = x3;
        TextureS[2].fY                   = y3;
        TextureS[2].u.fZ                 =  -z3;
        TextureS[2].uBaseRGB.dwPacked           =   0xffffffff;        
        TextureS[2].fU                   = u3;
        TextureS[2].fV                   = v3;         
       
		int y=sizeof(KMVERTEX_03);
        for (int i=0; i<=2; i++)
        	kmSetVertex (&VertexBufferDesc, &TextureS[i], KM_VERTEXTYPE_03, sizeof (KMVERTEX_03));
  	
        	
        	
      	
        	
        	
		}

}

void GTriCZT(int Tpage,float x1,float y1,float x2,float y2,float x3,float y3,float z1,float z2,float z3,float u1,float v1,float u2,float v2,float u3,float v3)
{

if (Tpage>=0 && Tpage<(NumTPages+1))
if (((x1>(IX1-32)) && (x1<(IX2+32))) || ((x2>(IX1-32)) && (x2<(IX2+32))) || ((x3>(IX1-32)) && (x3<(IX2+32))))
if (((y1>(IY1-32)) && (y1<(IY2+32))) || ((y2>(IY1-32)) && (y2<(IX2+32))) || ((y3>(IY1-32)) && (y3<(IY2+32))))
if ((z1<0) && (z2<0) && (z3<0))
{


        kmStartStrip (&VertexBufferDesc, &TextureHead[Tpage]);
      

        TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[0].fX                   = x1;
        TextureS[0].fY                   = y1;
        TextureS[0].u.fZ                 = -z1;
        TextureS[0].uBaseRGB.dwPacked           =   0xffffffff;        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        


        TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[1].fX                   = x2;
        TextureS[1].fY                   = y2;
        TextureS[1].u.fZ                 =  -z2;
        TextureS[1].uBaseRGB.dwPacked           =   0xffffffff;        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v2;         
       

        TextureS[2].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
        TextureS[2].fX                   = x3;
        TextureS[2].fY                   = y3;
        TextureS[2].u.fZ                 =  -z3;
        TextureS[2].uBaseRGB.dwPacked           =   0xffffffff;        
        TextureS[2].fU                   = u3;
        TextureS[2].fV                   = v3;         
       
		int y=sizeof(KMVERTEX_03);
        for (int i=0; i<=2; i++)
        	kmSetVertex (&VertexBufferDesc, &TextureS[i], KM_VERTEXTYPE_03, sizeof (KMVERTEX_03));
        	
        	
        	
      	
        	
        	
		}

}

void GTriCCZT(int Tpage,float x1,float y1,float x2,float y2,float x3,float y3,float z1,float z2,float z3,float u1,float v1,float u2,float v2,float u3,float v3,DWORD vc1,DWORD vc2,DWORD vc3)
{

if (Tpage>=0 && Tpage<(NumTPages+1))
if (((x1>-32) && (x1<(640+32))) || ((x2>-32) && (x2<(640+32))) || ((x3>-32) && (x3<(640+32))))
if (((y1>-32) && (y1<(480+32))) || ((y2>-32) && (y2<(480+32))) || ((y3>-32) && (y3<(480+32))))
//if ((z1<0) || (z2<0) || (z3<0))
{


        kmStartStrip (&VertexBufferDesc, &TextureHead[Tpage]);
      

        TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[0].fX                   = x1;
        TextureS[0].fY                   = y1;
        TextureS[0].u.fZ                 = -z1;
        TextureS[0].uBaseRGB.dwPacked           =   vc1;        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        


        TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[1].fX                   = x2;
        TextureS[1].fY                   = y2;
        TextureS[1].u.fZ                 =  -z2;
        TextureS[1].uBaseRGB.dwPacked           =   vc2;        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v2;         
       

        TextureS[2].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
        TextureS[2].fX                   = x3;
        TextureS[2].fY                   = y3;
        TextureS[2].u.fZ                 =  -z3;
        TextureS[2].uBaseRGB.dwPacked           =   vc3;        
        TextureS[2].fU                   = u3;
        TextureS[2].fV                   = v3;         
       

        for (int i=0; i<=2; i++)
        	kmSetVertex (&VertexBufferDesc, &TextureS[i], KM_VERTEXTYPE_03, sizeof (KMVERTEX_03));
        	
        	
        	
      	
        	
        	
		}

}




void GTriC(float x1,float y1,float x2,float y2,float x3,float y3,float vc1,float vc2,float vc3)
{




}

void GTri(float x1,float y1,float x2,float y2,float x3,float y3)
{

    	

}

void SDSChangeDir(char * dir)
{
gdFsChangeDir(dir);
//mw_pr(dir);
}

void SDSRenderNoFlipClip(int x1,int y1,int x2,int y2)
{
        		kmEndPass (&VertexBufferDesc);   
        		kmRender (KM_RENDER_NOFLIP);
        		kmEndScene (&kmsc);
        		
				kmSetRegionClipping((x1)>>5,y1,(x2)>>5,(y2)>>5);
				kmSetPixelClipping(x1,y1,x2,y2);
				        		
				kmBeginScene (&kmsc);
				kmBeginPass (&VertexBufferDesc);  
}

void SDSRenderStart(void)
{
	PROFILE_FN(RenderBeginScene);

//mw_pr("Render Start Called");
if (RENDERALLOWED==KM_TRUE)
 	{
	kmBeginScene (&kmsc);
	kmBeginPass (&VertexBufferDesc);
	}	
	VPointer=0;
	kazam=0;
}

#define TTSIZE 128
void SDSContinuePass(void)
{
kmContinuePass(&VertexBufferDesc);
}
void SDSRenderNoFlip(void)
{
PROFILE_FN(RenderFlip);

//mw_pr("Render Flip Called");
if (kazam==1)
//	if (NumTPages>0)
{
  	int x=0;
  	int y=0;
  
  	for (int z=0;z<NumTPages;z++)
  	 {
  	  kmChangeStripListType( &TextureHead[z+1], KM_TRANS_POLYGON );	
  	  kmChangeStripBlendingMode       ( &TextureHead[z+1], KM_IMAGE_PARAM1, KM_ONE, KM_ZERO);
  	  kmChangeStripTextureShadingMode ( &TextureHead[z+1], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
      kmChangeStripUseAlpha			( &TextureHead[z+1], KM_IMAGE_PARAM1, KM_TRUE ); 	

  	 
	  GTQuadN(z+1,x,y,x+TTSIZE,y,x,y+TTSIZE,x+TTSIZE,y+TTSIZE,1.0f,1,1,1);
	  x+=TTSIZE;
	  if (x>590)
	   {
	    x=0;
	    y+=TTSIZE;
	   }
	 }
}

if (RENDERALLOWED==KM_TRUE)
		{
		PLATFORM.CheckScreenSaver();

        kmSetBorderColor(Border);
        kmEndPass (&VertexBufferDesc);   
        kmRender (KM_RENDER_NOFLIP);
        kmEndScene (&kmsc);
	    }       
	    
        // Dodgy code here - must move somewhere else
        
        if (InternalPlay==TRUE)
         if ((ADXStillPlaying())==FALSE)
         {
         InternalTrack=(InternalTrack+1);
         if (InternalTrack>6) InternalTrack=1;
         PlayStream(InternalTrack);
         }

}        


void SDSRenderFlip(void)
{
	PROFILE_FN(RenderEndScene);


//mw_pr("Render Flip Called");
if (kazam==1)
//	if (NumTPages>0)
{
  	int x=0;
  	int y=0;
  
  	for (int z=0;z<NumTPages;z++)
  	 {
  	  kmChangeStripListType( &TextureHead[z+1], KM_TRANS_POLYGON );	
  	  kmChangeStripBlendingMode       ( &TextureHead[z+1], KM_IMAGE_PARAM1, KM_ONE, KM_ZERO);
  	  kmChangeStripTextureShadingMode ( &TextureHead[z+1], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
      kmChangeStripUseAlpha			( &TextureHead[z+1], KM_IMAGE_PARAM1, KM_TRUE ); 	

  	 
	  GTQuadN(z+1,x,y,x+TTSIZE,y,x,y+TTSIZE,x+TTSIZE,y+TTSIZE,1.0f,1,1,1);
	  x+=TTSIZE;
	  if (x>590)
	   {
	    x=0;
	    y+=TTSIZE;
	   }
	 }
}

if (RENDERALLOWED==KM_TRUE)
		{
		PLATFORM.CheckScreenSaver();
		

        kmSetBorderColor(Border);
        kmEndPass (&VertexBufferDesc);   
        kmRender (KM_RENDER_FLIP);
        kmEndScene (&kmsc);
	    }       
	    
        // Dodgy code here - must move somewhere else
        
        if (InternalPlay==TRUE)
         if ((ADXStillPlaying())==FALSE)
         {
         InternalTrack++;
         if (InternalTrack>5) InternalTrack=1;
         PlayStream(InternalTrack);
         }
         
        

}        

void R555ToR565(UWORD *Dscreen,int size)
{
int r,g,b,j;
UWORD dpp,spp;

         for (j= 0 ;j<size;j++)
         
           {
        	spp=*(Dscreen+j);
        	r=(spp & 0x1f);
        	g=((spp>>5) & 0x1f)<<1;
        	b=(spp>>10) & 0x1f;        	
        	dpp=(r<<11)+(g<<5)+(b);
            *(Dscreen+j ) = dpp;
           }
}

void DCTimToScreen(char *fn)
{
    long CurrentFB, i,j,sp,dp;
    UWORD *PixelAddress;
    UWORD spp;
    DWORD dpp;
    char r,g,b;
DWORD *fb;
PKMSURFACEDESC  pFrameBuff ;
GDFS mf;
long	FileBlocks;
long    *TimPtr;
long    *Twid;

	mf =gdFsOpen(fn,NULL);
        
    gdFsGetFileSctSize (mf, &FileBlocks);

    /* Allocate memory to nearest block size (2048 bytes). */
    TimPtr = (long *)Align32Malloc (FileBlocks * 2048);
//    Twid = (long *)Align32Malloc (FileBlocks * 2048);    
     
    /* Read file to memory region (Destination address must be 32-byte aligned). */
    gdFsReqRd32 (mf, FileBlocks, TimPtr);
   

    /* Wait for file access to finish. */
    while (gdFsGetStat (mf) != GDD_STAT_COMPLETE)    
     {}
	gdFsClose(mf);     
     
	//Rectangle2Twiddled ((char *) ((long) TimPtr + 16), (char *) TwiddledPtr, 256,256, 16);
	Dscreen=(UWORD*) TimPtr;	    
	TimPtr+=3;
			
         for (j= 0 ;j<FileBlocks*1024;j++)
         
           {
        	spp=*(Dscreen +(j));
        	r=(spp & 0x1f);
        	g=((spp>>5) & 0x1f)<<1;
        	b=(spp>>10) & 0x1f;        	
        	dpp=(r<<11)+(g<<5)+(b);
            *(Dscreen + (j)) = dpp;
           }
	


	kmCreateTextureSurface (&TextureSurfaces[0], 512, 256,( KM_TEXTURE_RECTANGLE | KM_TEXTURE_RGB565));
    kmLoadTexture (&TextureSurfaces[0], (unsigned long *)TimPtr); 

    DefaultContext.ImageControl[KM_IMAGE_PARAM1].pTextureSurfaceDesc = &TextureSurfaces[0];
    
    memset ((unsigned char *)&TextureHead[0], 0, sizeof(TextureHead[0]));
	
    DefaultContext.ImageControl[KM_IMAGE_PARAM1].bUseAlpha = KM_TRUE;
    // DefaultContext.ImageControl[KM_IMAGE_PARAM1].nTextureShadingMode = KM_MODULATE_ALPHA;   
    kmGenerateStripHead05 (&TextureHead[0], &DefaultContext);    
    
    _Align32Free(TimPtr);
}

void PtrToScreen(void *Src,int x,int y)
{
 kmFreeTexture(&TexSurfaceDesc);
 kmCreateTextureSurface (&TexSurfaceDesc, x, y,( KM_TEXTURE_RECTANGLE | KM_TEXTURE_RGB565)); 
 kmLoadTexture (&TexSurfaceDesc, (unsigned long *)Src);
}


//***********************************************************************************************
//***********************************************************************************************
//******************************** Movie Code ***************************************************
//***********************************************************************************************
//***********************************************************************************************






KTBOOL bpAcSetup(AC_DRIVER_TYPE driverType, KTBOOL usePolling,
	AC_CALLBACK_HANDLER theCallback)
{
	KTU32 *driverImage = KTNULL;
	KTU32 driverImageSize = 0;
	KTU32 driverSize = 0;
	KTSTRING driverName = KTNULL;


	bpAcError = acErrorGetLast();


	a64FileInit();


	acIntInstallCallbackHandler(theCallback);


	if (!acIntInit())
	{

		return (KTFALSE);
	}


	if (driverType == AC_DRIVER_DA)
	{
		driverName = AC_DRIVER_NAME_DA;
	}
	else if (driverType == AC_DRIVER_MIDI)
	{
		driverName = AC_DRIVER_NAME_MIDI;
	}
	else
	{
		acErrorSet(AC_OUT_OF_RANGE_PARAMETER,
						"bpAcSetup - driverType is invalid");
		return (KTFALSE);
	}


	if (!a64FileGetSize(driverName, &driverSize))
	{

		return (KTFALSE);
	}
	driverImageSize = SECTOR_ALIGN(driverSize);


	driverImage = (KTU32*)syMalloc(driverImageSize);
	if (!driverImage)
	{

		return (KTFALSE);
	}


	if (!a64FileLoad(driverName, driverImage, driverImageSize))
	{

		return (KTFALSE);
	}


	if (!acSystemInit(driverType, driverImage, driverSize, usePolling))
	{

		return (KTFALSE);
	}


	syFree(driverImage);

	return (KTTRUE);
}




  
void bpAcShutdown(void)
{
	acSystemShutdown();
}



KTBOOL bpAmSetup(AC_DRIVER_TYPE driverType, KTBOOL usePolling,
	AC_CALLBACK_HANDLER theCallback)
{
	// Initialize the AC layer
	if (!bpAcSetup(driverType, usePolling, theCallback))
	{
		// check acErrorGetLast() and/or amErrorGetLast()
		return (KTFALSE);
	}

	// Get error message pointers
	bpAmError = amErrorGetLast();

	// Initialize the AM layer
	if (!amInit())
	{
		// check acErrorGetLast() and/or amErrorGetLast()
		return (KTFALSE);
	}
		
	return (KTTRUE);
}






void bpAmShutdown(void)
{

	amShutdown();
}


#define	USR_INT_PRI			(0x90)
#define bool BOOL
#define NOT_FLAG(flag)		(((flag) == OFF) ? ON : OFF)
											


											








void ap_mw_err_func(void *obj, char *errmsg)
{
	AP ap=(AP)obj;

	for (;;);
}

#define CACHE_SHINOBI (SYD_CACHE_FORM_IC_ENABLE | SYD_CACHE_FORM_OC_ENABLE | SYD_CACHE_FORM_P1_CB )
#define CACHE_SOFDEC (SYD_CACHE_FORM_OC_INDEX | CACHE_SHINOBI)
void ApInitMw(AP ap, Sint32 mod, Sint32 frm, Sint32 cnt, Sint32 ltncy)
{
	MWS_PLY_INIT_SFD	iprm;

	memset(&iprm, 0, sizeof(iprm));	
	iprm.mode		= mod;
	iprm.frame		= frm;
	iprm.count		= cnt;
	iprm.latency	= ltncy;
	mwPlyInitSfdFx(&iprm);

	mwPlySetVertexBuffer(vtxbuf);
	//mwPlyEntryErrFunc(ap_mw_err_func, ap);
}

void ApFinishMw(AP ap)
{
	mwPlyFinishSfdFx();
}




void ApStart(AP ap,int x,int y,int xsize,int ysize)
{
	MWS_PLY_CPRM_SFD		*cprm=&ap->cprm;

	memset(cprm, 0, sizeof(cprm));
	cprm->compo_mode = MWD_PLY_COMPO_OPEQ;
	cprm->ftype	= MWD_PLY_FTYPE_SFD;
	cprm->dtype	= MWD_PLY_DTYPE_WND;
	cprm->max_bps = 450*1024*8;
	cprm->max_width = 320;
	cprm->max_height = 240;
	cprm->nfrm_pool_wk = 8;

	cprm->wksize = mwPlyCalcWorkCprmSfd(cprm);

	cprm->work =(signed char*) syMalloc(cprm->wksize);
	ap->ply = mwPlyCreateSofdec(cprm);
	
	if (ap->ply == NULL) 
	  {
	   for (;;) ;
	  }
	//syCacheInit(CACHE_SOFDEC);
	mwPlyStartFname(ap->ply, (signed char *)ap->fname);
	mwPlySetDispPos(ap->ply,x,y);		
	mwPlySetDispSize(ap->ply,xsize,ysize);	
	
}


void ApStop(AP ap)
{
	mwPlyStop(ap->ply);
	mwPlyDestroy(ap->ply);
	ap->ply = NULL;
	syFree(ap->cprm.work);
	ap->cprm.work = NULL;
}


void ap_disp_info(AP ap)
{
	Sint32	time, tunit, hh, mm, ss, ff;

	mwPlyGetTime(ap->ply, &time, &tunit);


}

BOOL	movie_was_quit_by_keypress;

Sint32 ApExec(AP ap, bool Keypress)
{
	MWPLY					ply=ap->ply;
	MWE_PLY_STAT			stat;
	const PDS_PERIPHERAL	*per;

	per = pdGetPeripheral(PDD_PORT_A0);
		
	if ((per->press & PDD_DGT_ST) && Keypress)
	{
		movie_was_quit_by_keypress = TRUE;
		return FALSE;
	}
	
	stat = mwPlyGetStat(ply);
	if ((stat == MWE_PLY_STAT_PLAYEND) || (stat == MWE_PLY_STAT_ERROR))
		return FALSE;

	return TRUE;
}


void ApGdErrFunc(void *obj, long errcode)
{
	AP ap=(AP)obj;
	if (errcode == GDD_ERR_TRAYOPEND || errcode == GDD_ERR_UNITATTENT)
		ap->term_flag = ON;	
}

void ApVsyncFunc(void *blam)
{
      //  UsrCheckDoorOpen();
}


void UsrBeginDraw(void)
{
        kmBeginScene(&kmsc);
        kmBeginPass(&VertexBufferDesc);
        pdExecPeripheralServer();

}


void UsrEndDraw(void)
{

        kmEndPass(&VertexBufferDesc);
		UsrRenderId[1] = UsrRenderId[0];
		UsrRenderId[0] = 
		kmRender(KM_RENDER_FLIP);	
        kmEndScene(&kmsc);

}


void UsrWaitVBlank(void)
{
/*
#ifdef USR_KAMUI_NOWAITVSYNC
	KMSTATUS	stat;

	for (;;) {
		stat = kmGetRenderStatus(UsrRenderId[1]);
		if (stat == KMSTATUS_UNDER_RENDER) {
			kmWaitVBlank();
		} else {
			return;
		}
	}
#endif*/
}

BOOL PlayMovie(char *fname,int x,int y,int xsize,int ysize,BOOL Keypress)
{
    // Does the file exist?
	movie_was_quit_by_keypress = FALSE;

    SDSChangeDir("\\mohodc\\movies");
    GDFS gf;
    gf=gdFsOpen(fname,NULL);
    if (gf!=NULL)
    {
	    gdFsClose(gf);
	 
		int FadeCount=255;
		StopStream();
	    VSyncOn();
		ApInitMw(ap, mode, frame, count, latency);	
	
    
	    for (int a=0;a<64;a++)
	    	SDSKillSample(a);
		
		ap->fname=fname;
		ApStart(ap,x,y,xsize,ysize);
		
		UsrBeginDraw();	
		UsrEndDraw();
		ret = TRUE;
			while (1) 
			{
	
				mwPlyExecTexSvr();
				UsrBeginDraw();
				mwPlyExecDrawSvr();
				
				if (ret!=FALSE) ret = ApExec(ap,Keypress);
				UsrEndDraw();
				if (ret==FALSE)
				 {
				 mwPlySetBright(ap->ply,0);			 
				 //mwPlySetSrfBright(ap->ply,0,1.0f,FadeCount/255,FadeCount/255,FadeCount/255);
				 FadeCount-=32;
				 }
				if ((FadeCount<0) || ( ap->term_flag == ON))
					break;
	
			}
		ApStop(ap);
	   	ApFinishMw(ap);		
	}
   	VSyncOff();
    SDSChangeDir("\\mohodc");  	

	return movie_was_quit_by_keypress;
}


void *UsrInitVideo(Sint32 *mode, Sint32 *frame, Sint32 *count, Sint32 *latency)
{
	KMPACKEDARGB BorderColor;
	InternalYClip=480;
	PALMODE=KM_FALSE;
    /* Check the cable for NTSC/PAL or VGA.. works properly for SCART. */
    switch (syCblCheck())
	{
        /* Initialize the display device and set the frame buffer based on the video mode. */
        case SYE_CBL_NTSC:  /* U.S./North America NTSC (60Hz) and Brazil PAL-M (60Hz). */
            *mode = KM_DSPMODE_NTSCNI640x480;
            break;
        case SYE_CBL_PAL:   /* Europe PAL (50Hz) and Argentina PAL-N (50Hz). */
            *mode = KM_DSPMODE_PALNI640x480EXT;
            PALMODE=KM_TRUE;
            break;
        case SYE_CBL_VGA:   /* Standard VGA. */
            *mode = KM_DSPMODE_VGA;
            break;
        default:
            syBtExit();     /* Unknown video type, return to Dreamcast BootROM. */
	}

	*frame = KM_DSPBPP_RGB565;
	*count = 1;
	*latency = 2;
	sbInitSystem(*mode, *frame, *count);

 	
	if (syCblCheck()==SYE_CBL_PAL)
  	    {
  	    	InternalYClip=560;
	 		kmSetPALEXTCallback(PALExtCallbackFunc,NULL);
			kmSetDisplayMode (KM_DSPMODE_PALNI640x480EXT, KM_DSPBPP_RGB565, TRUE, FALSE);
			}
	/*----------------------------------------------------------------- */
	/* Create frame buffer and vertex buffer */
	pFB[0] = &PrimarySurfaceDesc;
	pFB[1] = &BackSurfaceDesc;
	/* VertexBufferPointer(set5) */
	dwDataPtr =	(PKMDWORD)(
		((KMDWORD)syMalloc(VERTEXBUFFERSIZE) & 0x0FFFFFFFUL) | 0xA0000000 );
	kmsc.dwSize				 = sizeof(KMSYSTEMCONFIGSTRUCT);
	kmsc.flags				 = //KM_CONFIGFLAG_ENABLE_CLEAR_FRAMEBUFFER
									  KM_CONFIGFLAG_NOWAIT_FINISH_TEXTUREDMA
#ifdef USR_KAMUI_NOWAITVSYNC
									 | KM_CONFIGFLAG_NOWAITVSYNC
#endif
									 | KM_CONFIGFLAG_ENABLE_2V_LATENCY;
	/* for Frame buffer */
	kmsc.ppSurfaceDescArray	 = pFB;
	kmsc.fb.nNumOfFrameBuffer= 2;
	/* for Texture Memory */
	kmsc.nTextureMemorySize	 = TEXTUREMEMORYSIZE;
	kmsc.nNumOfTextureStruct = 4096;
	kmsc.nNumOfSmallVQStruct = 1024;
	kmsc.pTextureWork		 = &FBarea[0];
	/* for Vertex buffer */
	kmsc.pBufferDesc		 = &VertexBufferDesc;
	kmsc.nNumOfVertexBank	 = 1;
	kmsc.pVertexBuffer		 = dwDataPtr;
	kmsc.nVertexBufferSize	 = VERTEXBUFFERSIZE;
	kmsc.nPassDepth			 = 1;
	kmsc.Pass[0].dwRegionArrayFlag = KM_PASSINFO_AUTOSORT;
	
	kmsc.Pass[0].nDirectTransferList = KM_OPAQUE_POLYGON;
	kmsc.Pass[0].fBufferSize[0] =   0.0f;	
	kmsc.Pass[0].fBufferSize[1] =   0.0f;
	kmsc.Pass[0].fBufferSize[2] =  100.0f;	
	kmsc.Pass[0].fBufferSize[3] =   0.0f;
	kmsc.Pass[0].fBufferSize[4] =   0.0f;

	kmSetSystemConfiguration( &kmsc );

	/* Initialize Strip context */
	StripContext.nSize = sizeof(KMSTRIPCONTEXT);
	kmInitStripContext(KM_STRIPCONTEXT_SYS_GOURAUD|KM_OPAQUE_POLYGON,&StripContext);


	kmGenerateStripHead03(&StripHead, &StripContext);
	
	BorderColor.dwPacked = 0;
	kmSetBorderColor(BorderColor);
	
	//  txt_InitDisplayString();
    //	mwMngConfigUsingKmWaitVsyncCb(FALSE);

    memset ((unsigned char *)&GouraudHead, 0, sizeof(GouraudHead));
    kmGenerateStripHead00 (&GouraudHead, &DefaultContext);
    
    // memset ((unsigned char *)&TextureHead[254], 0, sizeof(TextureHead));
    // kmGenerateStripHead05 (&TextureHead[254], &DefaultContext);
    
    //KMVERTEXCONTEXT MVC;
    //kmSetVertexRenderState(VertexBufferDesc,);

    /* Set the texture surface of the opaque polygon context to the newly loaded texture. */
    DefaultContext.ImageControl[KM_IMAGE_PARAM1].pTextureSurfaceDesc = &TexSurfaceDesc;

    /* Modify the standard polygon context to support translucency. */
    DefaultContext.StripControl.nListType = KM_TRANS_POLYGON;
    DefaultContext.ImageControl[KM_IMAGE_PARAM1].bUseAlpha = KM_FALSE;
    DefaultContext.ImageControl[KM_IMAGE_PARAM1].nTextureShadingMode = KM_DECAL;
 	gdFsEntryErrFuncAll(errfunc, (void *) 0);
 	
 	// Setup the Clipping Stuff
 	kmChangeStripUserClipMode(&GouraudHead,KM_USERCLIP_INSIDE); 	
 	
 	for (int a=0;a<MaxTextures;a++)
 	{

 	kmChangeStripUserClipMode(&TextureHead[a],KM_USERCLIP_INSIDE); 	
 	}
 	
	return (void *)&VertexBufferDesc;
}


//#define	MW_MAX_CH		(8)
//#define	MW_SNDRAM_SIZE	(0x4040*MW_MAX_CH)

SYCHAIN                               usrg_chid = (SYCHAIN)NULL;

/*
void UsrSetVsyncFunc(void (*func)(void))
{
	if (usrg_chid != (SYCHAIN)NULL) {
		syChainDeleteHandler(usrg_chid);
	}
	usrg_chid = syChainAddHandler(
		(SY_CHAIN_EVENT) SYD_CHAIN_EVENT_IML6_VBLANK,
		(void *)func, (Uint32)USR_INT_PRI, (void *)NULL);
}
*/

PDS_PERIPHERAL      *GetSubPad(int PadNum)
{
	pdExecPeripheralServer();
 
	PDS_PERIPHERAL 	*per; 
	//CONTROLS.mControllers[0].ActivateController
	if (PadNum==0)
		per = (PDS_PERIPHERAL *) pdGetPeripheral (PDD_PORT_A0);
	if (PadNum==1)
		per = (PDS_PERIPHERAL *) pdGetPeripheral (PDD_PORT_B0);
		
 	return per;
}

float GetSysTimeFloat(void) 
{ 
return (syTmrCountToMicro(syTmrGetCount())/1000000.0f); 
}


GINT GetFrameRenderFraction() 
{


//syTmrCountToMicro(  ) / 1000.0f;
//return FToG((mFrameTime - mTime) / 0.04f);};
// range G(0) - G(1)
}

void showem(void)
{
//GTriCZT(0,0,0,640,0,480,0,1,1,1,-10,-10,-10,0,0,1,0,0,1);

	kazam=1;

}


void Test4x4(void)
{
}

void SubSetVolume(void)
{
}

void SubPlaySound(int number)
{
 BOOL zam;
// number=0;
 
 zam=amSoundFetchSample(aicaBuffer,number,&mysound[number]);
if (amSoundIsPlaying(&mysound[number])) return;
 zam=amSoundAllocateVoiceChannel(&mysound[number]); 
 zam=amSoundSetVolume(&mysound[number],127);
 zam=amSoundSetPan(&mysound[number], 64);
 zam=amSoundSetCurrentPlaybackRate(&mysound[number],2100); 
 zam=amSoundPlay(&mysound[number]);
 
 AM_ERROR *lasterr= amErrorGetLast();
}



void fuu(volatile KTU32 channel)
{
ChannelPlaying[channel]=0;
}

void SDSInitSound(void)
{
	CMEMBUFFER	mem;

if (!bpAcSetup(AC_DRIVER_DA,KTFALSE,KTNULL))
   { 
     sbExitSystem();
   }
acIntInstallCallbackHandler(fuu);
//mw_pr(fname);
for (int a=0;a<64;a++)
 ChannelPlaying[a]=0;
	SDSChangeDir("\\mohodc");
    IFH=gdFsOpen ("mohobank.bnk", NULL);
	SDSChangeDir("\\");
    
    long FileBlocks,FileSize;

    gdFsGetFileSctSize (IFH, &FileBlocks);
    gdFsGetFileSize (IFH, &FileSize);
    gdFsClose(IFH);
    /* Allocate memory to nearest block size (2048 bytes). */
    //gdFsReqRd32 (IFH, FileBlocks, SoundFile);
   
   
    	if (!(mem.InitFromFile("mohoBank.bnk")))
	{
		//mw_pr("File Load failed\n");

		mem.Close();

	}
	//TRACE("File Load successful\n");

	// First load in the PC specific stuff
	UINT	chunk;

	// check if file is valid
	mem.Read(&samplecount, 4);
	mem.Read(&sampleinfo,800);
	int lefttoread=FileSize-804;
    unsigned long *BankData = (unsigned long *)Align32Malloc (lefttoread);
	mem.Read(BankData,lefttoread);
    
    mem.Close();
    acSystemSetStereoOrMono(0); 
    acSystemSetMasterVolume(255);
    bool zam;
	  unsigned long *nextFree = acSystemGetFirstFreeSoundMemory(); //Get the beginning of free sound (AICA) memory //
	unsigned long *Cp = BankData;    
	// Some Dodgy Code to Handle the 32k hardware limit!!!!!
    for (int a=0;a<samplecount;a++)
     {
	  SoundMemPointers[a]=nextFree;
	  int templen=sampleinfo[a][1];
      //if (templen>20*1024) templen=20*1024;	 	  
	  //acG2FifoCheck();
 	  zam=acG2Write(nextFree,Cp, templen);// do a synchronous 32-bit aligned write to soundram
 	  nextFree+=templen>>2;
 	  Cp+=sampleinfo[a][1]>>2;
	 }
	 GNextFreeSndRam=nextFree;
Align32Free (BankData);
//acTransfer();


}

//0   1     2     3     4     5     6     7     8      9
int SampleFreq[] =
{
44100,44100,20822,44100,44100,38001,33401,39274,44100,44100,
22138,44100,44100,44100,28190,40494,44100,44100,44100,44100,
44100,22952,13581,36418,17177,32578,26859,27315,44100,44100,
44100,44100,44100,44100,37636,44100,44100,44100,44100,44100,
44100,44100,44100,44100,44100,44100,44100,44100,44100,44100,
44100,44100,19399,44100,44100,44100,44100,44100,44100,44100,
44100,36469,44100
};

KTU8 AICAVolumeConversionTable[] = 
{
	0x00,0x00,0x01,0x01,0x02,0x02,0x03,0x03,0x04,0x04,0x04,0x04,0x05,0x05,0x05,0x05,
	0x06,0x06,0x06,0x06,0x07,0x07,0x07,0x07,0x08,0x08,0x08,0x09,0x09,0x09,0x0a,0x0a,
	0x0a,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x0c,0x0d,0x0d,0x0d,0x0d,0x0e,0x0e,0x0e,0x0f,
	0x0f,0x0f,0x10,0x10,0x10,0x11,0x11,0x11,0x12,0x12,0x12,0x13,0x13,0x13,0x14,0x14,
	0x14,0x15,0x15,0x15,0x16,0x16,0x16,0x17,0x17,0x17,0x18,0x18,0x19,0x19,0x1a,0x1a,
	0x1a,0x1b,0x1b,0x1b,0x1c,0x1c,0x1c,0x1d,0x1d,0x1d,0x1e,0x1e,0x1f,0x1f,0x20,0x20,
	0x21,0x21,0x22,0x22,0x22,0x23,0x23,0x23,0x24,0x24,0x25,0x25,0x26,0x26,0x27,0x27,
	0x28,0x28,0x29,0x29,0x2a,0x2a,0x2b,0x2b,0x2c,0x2c,0x2d,0x2d,0x2e,0x2e,0x2f,0x2f,
	0x30,0x30,0x31,0x31,0x32,0x32,0x33,0x33,0x34,0x34,0x35,0x35,0x36,0x36,0x37,0x37,
	0x38,0x39,0x3a,0x3a,0x3b,0x3b,0x3c,0x3c,0x3d,0x3d,0x3e,0x3f,0x40,0x40,0x41,0x41,
	0x42,0x43,0x44,0x45,0x46,0x46,0x47,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4c,0x4d,0x4d,
	0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,
	0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x69,0x6b,0x6c,0x6d,0x6e,0x6f,
	0x71,0x73,0x74,0x75,0x77,0x79,0x7a,0x7b,0x7d,0x7f,0x81,0x83,0x85,0x87,0x89,0x8b,
	0x8d,0x8f,0x91,0x93,0x95,0x97,0x99,0x9d,0x9f,0xa3,0xa5,0xa9,0xad,0xb1,0xb3,0xb7,
	0xbb,0xc1,0xc5,0xcb,0xd1,0xd7,0xdb,0xdf,0xe3,0xe7,0xeb,0xef,0xf3,0xf7,0xfb,0xff
};


void SDSKillSample(int channel)
 {
if ((channel>=ADXCL) && (channel<=ADXCH)) return;
 acDigiStop(channel);
 ChannelPlaying[channel]=0;
 }

BOOL SDSIsChannelPlaying(int channel)
{
KTU32 pos;

if ((channel>=ADXCL) && (channel<=ADXCH)) return; FALSE;

if (ChannelPlaying[channel]==1)return TRUE; else return FALSE;

}

void SDSVolume(int channel,int volume)
{
// if (channel<4) return;
if ((channel>=ADXCL) && (channel<=ADXCH)) return;

// SDS - Ewwwwwww nastee hack for the crowd sounds!!!
if ((ChannelSample[channel]>=21) && (ChannelSample[channel]<=27)) volume=(volume<<1) & 0xff;

volume=0xff-AICAVolumeConversionTable[0xff-(volume*2)];
volume=0xff-AICAVolumeConversionTable[0xff-(volume)];
//volume=0xff-AICAVolumeConversionTable[0xff-(volume)];
//volume=0xff-AICAVolumeConversionTable[0xff-(volume)];
//volume=255;
if (volume>255) volume=255;
 GSB=acDigiSetVolume(channel,volume);
}

void SDSPlaySample(int channel,int number,int vol,bool Loop)
{
 //if (channel<4) return;
 if ((channel>=ADXCL) && (channel<=ADXCH)) return;
  //GSB=acDigiClose(channel); 
  ChannelSample[channel]=number;  
  GSB=acDigiOpen(channel, (unsigned long) SoundMemPointers[number], sampleinfo[number][1], AC_ADPCM,44100);// channel is 0-63 or 0-15
  SDSVolume(channel,vol);  

  if (Loop==KM_FALSE) 
  {
   acDigiRequestEvent(channel,sampleinfo[number][1]);
   GSB=acDigiPlayWithParameters(channel,255,15,0,0,255,SampleFreq[number],AC_PITCH_AS_SAMPLE_RATE,0,0,0);
   }
  else
   GSB=acDigiPlayWithParameters(channel,255,15,0,0,255,SampleFreq[number],AC_PITCH_AS_SAMPLE_RATE,0,0,sampleinfo[number][1]-1);

  SDSVolume(channel,vol);
  
  ChannelPlaying[channel]=1;
  GSB=acDigiClose(channel);   
}

PDS_KEYBOARD *GetKeys(void)
{
 pdExecPeripheralServer();
 PDS_KEYBOARD *kbd ;
 
  if(!(kbd = pdKbdGetData  (PDD_PORT_A0)))
   if(!(kbd = pdKbdGetData  (PDD_PORT_B0)))
    if(!(kbd = pdKbdGetData  (PDD_PORT_C0)))
     kbd = pdKbdGetData  (PDD_PORT_D0);
 return kbd;
 
}


void WaitforKey(void)
{
    PDS_KEYBOARD *key;
	do
	{
	
	key=GetKeys();
	} while (key->key[0]!=PDD_KEYUS_A);

}

void PlayDaTrack(int nTrack)
{

Sint32 status,timeout =0;
    /* Play the track */
 status=gdFsDaPlay(nTrack, nTrack, 15);

    /* Ensure it has started */
    do {
       status = gdFsGetDrvStat();
        /* calculate timeout here */
    } while((status != GDD_DRVSTAT_PLAY) && (!timeout));
}


/*
typedef struct
{
	KTBOOL play;
	KTU32 x;
	KTU32 y;
	CHAR *fileName;
	KTU32 playCount;
	KTU32 failCount;
	KTU32 volume;
	AM_STREAM *stream;
	KTU32 *headBuffer;
	KTU32 headBufferSize;
	KTU32 *xferBuffer;
	KTU32 xferBufferSize;
	KTU32 *playBuffer;
	KTU32 playBufferSize;
} ASYNC_STREAM_STATE;

ASYNC_STREAM_STATE strm =

	{
		KTTRUE, 5, 5,
		"TRACK1.STR", 0, 0, 127, KTNULL,
		KTNULL, AM_FILE_BLOCK_SIZE,
		KTNULL, (16384* 2 * 2),
		KTNULL, 16384*2
	};






KTBOOL StreamSetup(ASYNC_STREAM_STATE *stateStream)
{
	// ------------------------------------------------------------------------
	// get the necessary buffers lined up for the stream
	//
	// allocate memory for the play buffers.
	//
	// transfer buffer MUST! be in aica memory and 32 byte aligned or file
	// reads will fail with the GDFS file system.
	// ------------------------------------------------------------------------

	if (!amHeapAlloc(&stateStream->playBuffer,stateStream->playBufferSize,
			32, AM_PURGABLE_MEMORY, KTNULL))
	{
		return (KTFALSE);
	}

	// ------------------------------------------------------------------------
	// allocate memory for the transfer buffers.
	//
	// transfer buffer MUST! be in sh4 memory and 32 byte aligned or file reads
	// will fail with the GDFS file system.
	// ------------------------------------------------------------------------

	stateStream->xferBuffer =
				(volatile KTU32 *)syMalloc(stateStream->xferBufferSize);
	if (!stateStream->xferBuffer)
	{
		return (KTFALSE);
	}

	// ----------------------------------------------------------------------------------------------
	// allocate memory for the header buffers.
	//
	// header buffer MUST! be in sh4 memory and 32 byte aligned or file reads
	// will fail with the GDFS file system.
	// ----------------------------------------------------------------------------------------------

	stateStream->headBuffer =
				(volatile KTU32 *)syMalloc(stateStream->headBufferSize);
	if (!stateStream->headBuffer)
	  {
		return (KTFALSE);
	  }

	// ------------------------------------------------------------------------
	// prepare the stream
	// ------------------------------------------------------------------------

	stateStream->stream = bpAmStreamPrepareFile(stateStream->fileName,stateStream->volume, AM_PAN_CENTER, stateStream->headBuffer,
		stateStream->xferBuffer, stateStream->xferBufferSize,stateStream->playBuffer, stateStream->playBufferSize, KTNULL);
		
	if(!stateStream->stream)
	  {
		return (KTFALSE);
	  }

	return (KTTRUE);
}



void PlayStream(char *FName)
{
AM_STREAM  *MSP; 
amStreamInitFile(MSP,FName);
amStreamOpen(MSP);
amStreamStart(MSP);
StreamSetup(&strm);
bpAmStreamStart(strm.stream);

}
*/

//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************

#define ADXMAX_CH			(2)
#define	ADXMAX_CDSTM		(1)
#define	ADXMAX_SFREQ		(44100)
#define	ADXWKSIZE		(ADXT_CALC_WORK(ADXMAX_CH, 1, ADXMAX_CDSTM, ADXMAX_SFREQ))
#define NOT_FLAG(flag)		(((flag) == OFF) ? ON : OFF)
typedef struct {
	Sint32	term_flag;			//	Application terminate flag
	Sint32	disp_flag;			//	Display information flag
	Sint32	pause_flag;			//	Pause flag
	char	*fname;				//	File name
	ADXT	adxt;				//	ADXT handle
	char	work[ADXWKSIZE];		//	Work area
} ADXAP_OBJ, *ADXAP;

ADXAP_OBJ ADXap_obj = {
	OFF,						//	Execute App.
	ON,							//	Display Info.
	OFF,						//	not puase
        "moho1.ADX",                           //      File name
	NULL						//	ADXT handle
};


ADXAP ADXap=&ADXap_obj;
void ADXApGdErrFunc(void *obj, Sint32 errcode)
{
	ADXAP ADXap=(ADXAP)obj;
	if (errcode == GDD_ERR_TRAYOPEND || errcode == GDD_ERR_UNITATTENT)
		ADXap->term_flag = ON;			/*	アプリケーションの終了		*/
									/*	Terminate application		*/
}

void ADXap_adx_err_func(void *obj, char *msg)
{

}


BOOL ADXStillPlaying(void)
{
 long stat = ADXT_GetStat(ADXap->adxt);
 if (stat == ADXT_STAT_PLAYEND) return FALSE;
 return TRUE;
//if (TrackNumber==1) ADXap->fname="moho3.adx";	
}

void ADXApInitMw(ADXAP ap)
{

	

	unsigned long port[6];
	#define MW_MAX_CH 6
	#define	MW_SNDRAM_SIZE	(0x4040*MW_MAX_CH)
	KTU32 *sndram = acSystemGetFirstFreeSoundMemory();
	for (int i = 0; i < MW_MAX_CH; i++)
		port[i] = 56+i;	// Define the port numbers
	mwSetupAudio64(GNextFreeSndRam, MW_SNDRAM_SIZE, port, MW_MAX_CH);	//

	ADXT_Init();
	ADXT_EntryErrFunc(ADXap_adx_err_func, NULL);
}

void ADXApFinishMw(ADXAP ap)
{
	ADXT_Finish();
}

void ADXApStart(ADXAP ap)
{

//	if ((ap->adxt=ADXT_Create(2, ap->work, ADXWKSIZE)) == NULL)
//		for (;;) 
	ADXT_SetAutoRcvr(ap->adxt, ADXT_RMODE_REPLAY);
	ADXT_StartFname(ap->adxt, ap->fname);
}


void ADXApStop(ADXAP ap)
{

	ADXT_Stop(ap->adxt);
}

void ADXap_disp_info(ADXAP ap)
{

}

Sint32 ADXApExec(ADXAP ap)
{
	ADXT					adxt = ap->adxt;
	long					stat;
	const PDS_PERIPHERAL	*per;

	per = pdGetPeripheral(PDD_PORT_A0);
	if (per->press & PDD_DGT_ST) {
		ap->pause_flag = NOT_FLAG(ap->pause_flag);
		ADXT_Pause(adxt, ap->pause_flag);	/*	一時停止解除			*/
											/*	Release pause status	*/
	}
	if (per->press & PDD_DGT_TA)
		ap->disp_flag = NOT_FLAG(ap->disp_flag);
	if (per->press & PDD_DGT_TB)
		return FALSE;
	stat = ADXT_GetStat(adxt);
	if (stat == ADXT_STAT_PLAYEND)
		return FALSE;
	if (ap->disp_flag == ON)
		ADXap_disp_info(ap);
	return TRUE;
}

void StopStream(void)
{
if (InternalPlay==FALSE) return;
InternalPlay=FALSE; // Try and avoid a deadlock with renderflip!
ADXApStop(ADXap);


}

void PlayStream(int TrackNumber)
{


//if (InternalPlay==TRUE) return;
 	InternalTrack=TrackNumber;
	/*	Frame Work	*/
	//while ( ADXap->term_flag == OFF ) {
 	ADXap->fname="moho1.adx";
	//TrackNumber=5;
	if (TrackNumber<0) TrackNumber=0;
	if (TrackNumber>5) TrackNumber=5;
	if (TrackNumber==1)	ADXap->fname="moho1.adx";

	if (TrackNumber==2)	ADXap->fname="moho2.adx";	

	if (TrackNumber==3) ADXap->fname="moho3.adx";	

	if (TrackNumber==4)	ADXap->fname="moho4.adx";	

	if (TrackNumber==5) ADXap->fname="moho5.adx";	
				
	//if (TrackNumber==6)	ADXap->fname="moho6.adx";	
    gdFsChangeDir("\\mohodc");	
    ADXApStart(ADXap);
    SOUND.DeclareVolumeChange();
	InternalPlay=TRUE;	
}	

void SetStreamVolume(int Vol)
	{
		//Vol=(Vol-10)*96;
		Vol=0xff-AICAVolumeConversionTable[0xff-(Vol*10)];		
		Vol=(Vol*4)-999;
		if (ADXap->adxt!=0)
		ADXT_SetOutVol(ADXap->adxt,Vol);
	}
	
void SDSRenderOn(void)
{
RENDERALLOWED=KM_TRUE;
}
void SDSRenderOff(void)
{
RENDERALLOWED=KM_FALSE;
}

//************************************************************************************
//************************************************************************************
//************************************************************************************
//**************************************** VMU/MEMORY CARD STUFF *********************
//************************************************************************************
//************************************************************************************
//************************************************************************************

ControllerState SDSCheckController(int ControllerNumber) // SDS use the defines PDD_PORT_A0 e.t.c.
{
 
pdExecPeripheralServer();

PDS_PERIPHERAL *per = (PDS_PERIPHERAL *) pdGetPeripheral (ControllerNumber);
ControllerState RCS=Controller_EXISTS_UNKNOWN;
switch (per->id)
 {
 case 2047:
   RCS=Conrtoller_EXISTS_ARCADE_STICK;
   break;
 case 984830:
   RCS=Conrtoller_EXISTS_STANDARD;   
   break;
 case 0: 
   RCS=Conrtoller_NOT_THERE;
   break; 
 } 
return RCS;
}


Sint32 SDSGetBootRomSoundMode(void)
{
// returns 0 for Stereo 1 for Mono 
    Sint32 Smode;
    char pbuf[16384];
    Sint32 res;
    res=syCfgInit(&pbuf);
    res=syCfgGetSoundMode(&Smode);
    res=syCfgExit();
    return Smode;
}


void SDSSetBootRomSoundMode(Sint32 SBootMode)
{
    Sint32 Smode;
    char pbuf[16384];
    Sint32 res;
    res=syCfgInit(&pbuf);
    res=syCfgSetSoundMode(SBootMode);
    res=syCfgExit();
}


Uint32 SDSFreeMemory(void)
{
Uint32 MFree,MSize;
 syMallocStat(&MFree,&MSize);
 return MFree;

}

Uint32 SDSGetFreeTextureMemory(void)
{
 Uint32 MFree,MSize;
 kmGetFreeTextureMem((unsigned int *)&MFree,(unsigned int *)&MSize);
 return MFree;
}



KMBOOL SDSIsTheLidOpen(void)
{
 KMBOOL res=KM_FALSE;
 BOOL bTestPending;
 Sint32 stat;

SDSChangeDir("\\");
stat = gdFsGetDrvStat();
 if (stat == GDD_DRVSTAT_OPEN || stat == GDD_DRVSTAT_BUSY)
 {
  gdFsReqDrvStat();
 }

 if( Abort )
 {
   res=KM_TRUE;
 }
                            
 return res;
}
void VSYNCHANDLER(void)
{

//MyChainID = syChainAddHandler (SYD_CHAIN_EVENT_IML6_VBLANK, MyVsyncFunc,0x90, NULL);

}

void SDSSoftReset(void)
{
	//Nasty Bad code!!!!!!!!!!
	//************************
	GAME.Shutdown();  
	StopStream();
	ADXT_Finish();	
    kmGarbageCollectTexture();
    for (int a=0;a<MaxTextures;a++)
    {
    if (TextureSurfaces[a].pSurface!=0)
    kmFreeTexture(&TextureSurfaces[a]);
    }
	ADXApFinishMw(ADXap);
	BupExit();
	pdKbdExit(); 	
//    sbExitSystem();
    syBtExit();
}

void HardWareStartUp(void)
{
}


void SDSEnableSoftReset()
{
 SoftResetAllowed=KM_TRUE;
 if (InternalResetCalled==KM_TRUE) SDSSoftReset();
}

void SDSDisableSoftReset()
{
 SoftResetAllowed=KM_FALSE;
}


void SDSStereoMono(Sint32 SM)
{
 acSystemSetStereoOrMono(SM); 
}

void SDSSwitchTo60hz(void)
{
  kmSetDisplayMode(KM_DSPMODE_NTSCNI640x480,KM_DSPBPP_RGB565,TRUE,FALSE);
}

void SDSSwitchTo50hz(void)
{
  kmSetDisplayMode(KM_DSPMODE_PALNI640x480EXT,KM_DSPBPP_RGB565,TRUE,FALSE);
}

BOOL SDSCanDo60hz(void)
{
if (PALMODE==KM_TRUE) 
   return TRUE; 
   else 
   return FALSE;
}


void SDSUpLoadLcd(int port,int index)
{

char *dat;
switch (index)
{
case 0 : 
dat=&angelLCD[0];
break;
case 1 : 
dat=&aposLCD[0];
break;
case 2 : 
dat=&benLCD[0];
break;
case 3 : 
dat=&lockLCD[0];
break;
case 4 : 
dat=&sophLCD[0];
break;
case 5 : 
dat=&docLCD[0];
break;
case 6 : 
dat=&mohoLCD[0];
break;
case 7 : 
dat=&ballLCD[0];
break;


}
Sint32 flag=pdLcdGetDirection(port);
pdVmsLcdWrite(port,dat,flag);
}



void MiniSoundTestApp(void)
{
	//Mini Test App
	
	PlayStream(5);
	int fcount=0;
	while (1)
	{
	fcount++;
	if ((fcount % 60)==1) SDSPlaySample(1,16,255,FALSE);
	if ((fcount % 60)==8) SDSPlaySample(1,16,255,FALSE);
	if ((fcount % 60)==16) SDSPlaySample(1,16,255,FALSE);
	if ((fcount % 60)==24) SDSPlaySample(1,16,255,FALSE);			
	if ((fcount % 60)==32) SDSPlaySample(1,16,255,FALSE);
	if ((fcount % 60)==40) SDSPlaySample(1,16,255,FALSE);
	if ((fcount % 60)==48) SDSPlaySample(1,16,255,FALSE);			
	SDSRenderStart();
	SDSRenderFlip();
	}
	
}

Sint32 SDSGetNativeVertexBufferSize(int Ptype)
{
 KMSYSTEMMETRICS mets;
 memset(&mets,0,sizeof(mets));
 mets.flags = 0x1f;
 kmGetSystemMetrics(&mets);
 return mets.MaxVertexSize[0][Ptype];
}

void main (void)
{

  #ifdef __GNUC__
	shinobi_workaround();
  #endif
  
	mwPlyPreInitSofdec();
	InternalResetCalled=KM_FALSE;
    vtxbuf = UsrInitVideo(&mode, &frame, &count, &latency);
  
	frame = KM_DSPBPP_RGB565;
	count = 1;
	latency = 2;	
	InternalViewOffset=320;
	InternalPlay  = FALSE;
	RENDERALLOWED = KM_TRUE;
	
	SDSEnableSoftReset();
	BupInit();
    SDSInitSound();	
	VSyncOff();
	SDSRenderOn();

	for (int a=0;a<MaxTextures;a++)
	{
 		TextureSurfaces[a].pSurface = NULL;
	}
	
//	SDSSwitchTo60hz();
	
	ADXApInitMw(ADXap);	
	if ((ADXap->adxt=ADXT_Create(2, ADXap->work, ADXWKSIZE)) == NULL)
		for (;;) 
	ADXT_SetAutoRcvr(ADXap->adxt, ADXT_RMODE_REPLAY);	

 	GAME.Init();
	GAME.Run();			
	GAME.Shutdown();  
	ADXT_Destroy(ADXap->adxt);
	ADXap->adxt = NULL;	
	SDSSoftReset();
    
}
