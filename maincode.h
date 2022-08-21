#ifndef maincode_H
#define	maincode_H
//#include <shinobi.h>    /* Shinobi system routines. */
//#include <kamui2.h>     /* Kamui2 low-level graphics HAL. */
//#include <sn_fcntl.h>   /* LibCross file types. */
//#include <usrsnasm.h>   /* LibCross I/O routines. */
//#include <sg_syCbl.h>   /* NTSC/RGB/VGA Cable check interface. */
//#include <string.h>
//#include <stdlib.h>
//#define Align32Malloc   (*MallocPtr)
//#define Align32Free     (*FreePtr)
#include <kamui2.h>
#include "types.h"
#include "sg_pad.h"
#include "sg_pdkbd.h"

#define MaxTextures 200


#define MaxVerts 2000	
extern int FailedTextureCount;
extern int IX1,IX2,IY1,IY2;	
extern int InternalViewOffset;
extern KMVERTEX_00         GouraudS      [MaxVerts]; /* Strip of opaque color triangles. */
extern KMVERTEX_03         TextureS      [MaxVerts]; /* Strip of textured triangles.     */
extern KMVERTEX_01         TextureBlendS [MaxVerts]; /* Strip of opaque color triangles. */
extern KMVERTEX_05         TextureAlphaS [MaxVerts]; /* Strip of textured triangles.     */
extern KMSTRIPHEAD         GouraudHead;             /* Render params for oqaque color polys. */
extern KMSTRIPHEAD         TextureHead[MaxTextures];             /* Render params for oqaque color polys. */
extern KMVERTEXBUFFDESC	    VertexBufferDesc;		/* Vertex-buffer				*/	
extern char TextureLoadBucket[];
extern	SINT NumTPages;
float GetSysTimeFloat(void );
// STREAM CODe
BOOL ADXStillPlaying(void);
void SetStreamVolume(int Vol);
void PlayStream(int TrackNumber);
void PlayStream2(int TrackNumber);
Void StopStream();
//SSPadData *SGetPadData(void);
// Sounds Code
PDS_KEYBOARD *GetKeys(void);
BOOL SDSIsChannelPlaying(int channel);
void SDSVolume(int channel,int volume);
void SDSPlaySample(int channel,int number,int vol,BOOL Loop);
void SDSInitSound(void);
void SDSKillSample(int channel);
void SDSFreeAllTextures(void);

void InitCZTs(int Tpage);
void ExitCZTs(int Numv);
void InitCZs(void);
void ExitCZs(int Numv);
void CreateTexturePage(int TextureNumber,int flag,int xsize,int ysize);


void GTri(float x1,float y1,float x2,float y2,float x3,float y3);
void GTriC(float x1,float y1,float x2,float y2,float x3,float y3,float vc1,float vc2,float vc3);
void GTriCZ(float x1,float y1,float x2,float y2,float x3,float y3,int vc1,int vc2,int vc3,float z1,float z2,float z3);
void GTriCZT(int Tpage,float x1,float y1,float x2,float y2,float x3,float y3,float z1,float z2,float z3,float u1,float v1,float u2,float v2,float u3,float v3);
void GTriCCZT(int Tpage,float x1,float y1,float x2,float y2,float x3,float y3,float z1,float z2,float z3,float u1,float v1,float u2,float v2,float u3,float v3,DWORD vc1,DWORD vc2,DWORD vc3);
void GTriZT(int Tpage,float x1,float y1,float x2,float y2,float x3,float y3,float z1,float z2,float z3,float u1,float v1,float u2,float v2,float u3,float v3);

void GQuad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);
void GTQuadN(int n,float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,int a1,int a2,int a3,int a4);
void SubPlaySound(int number);

void SDSRenderFlip(void);
void SDSRenderNoFlip(void);
void SDSRenderNoFlipClip(int x1,int y1,int x2,int y2);
void SDSRenderStart(void);
void SDSFullScreenNoClipping(void);
void SDSSwitchClippingOn(void);
void SDSSwitchClippingOff(void);
//void *(*MallocPtr)(unsigned long);
void *_Align32Malloc (unsigned long Size);
void _Align32Free (void *Ptr);
void Init32Malloc (void);
void SDSChangeDir(char * dir);
void *InitAll(void);
void DCTimToScreen(char *fn);
void R555ToR565(UWORD *Dscreen,int size);
SINT sDCLoadPartFile(ULONG *buffer,SINT len);
SINT sDCOpenFile(char *fname);
SINT sDCCloseFile(void);
void showem(void);
void SDSRenderOn(void);
void SDSRenderOff(void);
void SDSClearBuffer(void);
void SDSNoClearBuffer(void);
void SDSContinuePass(void);
void SDSStereoMono(Sint32 SM);  // 0=stereo 1=mono

// Extended 60hz / 50hz modes

void SDSSwitchTo60hz(void);
void SDSSwitchTo50hz(void);
BOOL SDSCanDo60hz(void);
#include <ac.h>
//#include <a64thunk.h>
#include <am.h>

// SOFT RESET FUNCTIONS
void SDSSoftReset(void);
void SDSEnableSoftReset();
void SDSDisableSoftReset();
void SDSUpLoadLcd(int port,int index);
void SDSSetClipRegion(int x1,int y1,int x2,int y2);
Sint32 SDSGetNativeVertexBufferSize(int Ptype);

extern void PtrToScreen(void *Src,int x,int y);
extern BOOL PlayMovie(char *fname,int x,int y,int xsize,int ysize,BOOL Keypress);
extern void SubInitMovie(void);
PDS_PERIPHERAL      *GetSubPad(int PadNum);

Uint32 SDSFreeMemory(void);
Uint32 SDSGetFreeTextureMemory(void);

extern KMSURFACEDESC PrimarySurfaceDesc;		/* Primary frame buffer			*/
extern KMSURFACEDESC BackSurfaceDesc;		/* Back-buffer frame buffer		*/

// SDS MEMCARD STUFF
enum BootSoundMode
{
BOOT_STEREO,
BOOT_MONO
};


enum ControllerState
{
Conrtoller_NOT_THERE,
Conrtoller_EXISTS_STANDARD,
Conrtoller_EXISTS_ARCADE_STICK,
Controller_EXISTS_UNKNOWN
};

ControllerState SDSCheckController(int ControllerNumber); // SDS use the defines PDD_PORT_A0 e.t.c.

Sint32 	SDSGetBootRomSoundMode(void);
void 	SDSSetBootRomSoundMode(Sint32 SBootMode);

BOOL	IsTextureAlpha(SINT tn);

#endif