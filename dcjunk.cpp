#ifndef DCJUNK_C
#define DCJUNK_C

#include "common.h"
#include "Types.h"
#include "maincode.h"
#include "Fcoords.h"
#include <math.h>

#include "dcjunk.h"
#include "camera.h"
#include "globals.h"
//***********************************************************
// SDS Wrappers for all the PSX routines that we dont have
//***********************************************************

//struct SINCOS rcossin_tbl[];


//******************* Floating point Stuff *****************
#define DCscale 2
//#define DCscale 6
FMatrix 		FRTM;	
FVector			FTTM;

//******************* Fixed point Stuff**********************
MATRIX		LCM;			// local colour matrix
SVECTOR		LLV[3];			// local lighting vector
MATRIX		LLM;			// local lighting matrix

long			FFAR;		// fog far
long			FNEAR;		// fog near

long			xOffset;	// geometry offset
long			yOffset;
long			screenZ;	// geometry screen Z
long			SCR2;		// geometry screen Z / 2

MATRIX 		RTM;			// 1 current rotation matrix
long			gsxy[3];	// 3 current screen coordinates
long			gsz[4];		// 4 current screen z corrdinates

MATRIX		stack[20];		// 20 rotation matricies for stack
long			stackPtr;

long		MaxZValue;




typedef struct {
	u_char minute;		/* minute (BCD) */
	u_char second;		/* second (BCD) */
	u_char sector;		/* sector (BCD) */
	u_char track;		/* track (void) */
} CdlLOC;

typedef struct {
	CdlLOC	pos;		/* file location */
	u_long	size;		/* file size */
	char	name[16];	/* file name (body) */
} CdlFILE;



struct  SPSXMeshGTriangle
{
    SWORD       mVerts[3];
    SWORD       mNorms[3];
};


struct  SPSXMeshTTriangle
{
    UBYTE       mU0, mV0;

    SWORD       mClut;
    UBYTE       mU1, mV1;

    SWORD       mTpage;
    UBYTE       mU2, mV2;

        UWORD           pad;
};


typedef struct {
		char		*strName;					// Stream file name.
		short		mode;						// 24-Bit or 16-Bit streaming.
		short		drawBorders;				// != 0 if borders on.
		u_short		scrWidth;					// Screen res width.
		u_short		x;							// X,Y position.
		u_short		y;
		u_short		width;						// Stream width and height.
		u_short		height;
		u_long		endFrame;					// Last frame No.
		u_long		vlcBufSize;					// Size of each VLC buffer (including header).
		u_short		volume;						// Left and Right ADPCM volume.
} StrInfo;

typedef void (*CdlCB)(u_char,u_char *);


extern "C" 
{

#ifdef JCLDCNOPSX			// Don't need any of this...

__declspec(data) volatile CdlCB CD_cbready (u_char p1,u_char * p2) {return NULL;}
__declspec(data) volatile CdlCB CD_cbsync (u_char p1,u_char * p2) {return NULL;}


void InitCrypt(void) {}
u_long *BreakDraw(void) {return 0;}
int CdControlB(u_char com, u_char *param, u_char *result) {return 0;}
int CdControlF(u_char com, u_char *param) {return 0;}
int CdInit(void) {return 0;}
CdlLOC *CdIntToPos(int i, CdlLOC *p) {return p;}
int CdPosToInt(CdlLOC *p) {return 0;}
int CdGetSector(void *madr, int size) {return 0;}
int CdReadFile(char *file, u_long *addr, int nbyte) {return 0;}
CdlCB CdReadyCallback(CdlCB func)					{return NULL;}
CdlFILE *CdSearchFile(CdlFILE *fp, char *name)		{return NULL;}
void DrawOTag(u_long *p)						{}
int CdReadSync(int mode, u_char *result) {return 0;}
int ClearImage(RECT *rect, u_char r, u_char g, u_char b) {return 0;}
void Decrypt(unsigned char*buf) {}
int DrawSync(int mode) {return 0;}
u_long DrawSyncCallback(void (*func)()) {return 0;}
u_short GetTPage(int tp, int abr, int x, int y) {return 0;}
__declspec(text) int GouraudDepth() {return 0;}
void InitGeom() {}
void InitHeap3(unsigned long *, unsigned long) {}
int IsIdleGPU(int max_count) {return 0;}
int LoadImage(RECT *rect, u_long *p) {return 0;}
POLY_GT3 *MakePolysFromList(POLY_GT3 *p3,SPSXMeshGTriangle *tri,SVECTOR*vc,long *vn,int count,unsigned long *o, SPSXMeshTTriangle *ttri) {}
long MemCardAccept( long chan ) {return 0;}
long MemCardCreateFile( long chan, char* fnam, long blocks ){return 0;}
long MemCardFormat( long chan ) {return 0;}
void MemCardInit( long flg ) {}
long NormalClip(long sxy0,long sxy1,long sxy2) {return 0;}
__declspec(text) int OffScreenMask() {return 0;}
void MemCardStart(void) {}
void PadInitDirect(unsigned char *, unsigned char *) {}
void PadSetAct(int, unsigned char *, int) {}
int PadSetMainMode(int socket, int offs, int lock) {return 0;}
void PadStopCom(void) {}
short PlayStream(StrInfo *str, short (*keyHandler)(void)) {return 0;}
int PadGetState(int) {return 0;}
void PadStartCom(void) {}
DISPENV *PutDispEnv(DISPENV *env) {return NULL;}
int PadInfoMode(int, int, int) {return 0;}
DRAWENV *PutDrawEnv(DRAWENV *env) {return NULL;}
void ReadCrypt() {}
int ResetCallback(void) {return 0;}
void SetBackColor(long rbk,long gbk,long bbk){}
void SetColorMatrix(MATRIX *m){}
DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h) {return NULL;}
void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, RECT *tw) {}
void SetDrawMove(DR_MOVE *p, RECT *rect, int x, int y) {}
void SetFarColor(long rfc,long gfc,long bfc) {}
void SetGeomScreen(long h) {}
void SetLightMatrix(MATRIX *m) {}
long SetVideoMode (long mode) {return 0;}
void SpuGetAllKeysStatus (char *status) {}
void SpuSetCommonAttr (SpuCommonAttr *attr) {}
int PadSetActAlign(int, unsigned char *) {return 0;}
int ResetGraph(int mode) {return 0;}
DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h) {return NULL;}
int SetGraphDebug(int level) {return 0;}
void  SsInit(void) {}
void  SsQuit(void) {}
void  SsSeqCalledTbyT(void) {}
void  SsSetMVol(short, short) {}
void  SsSetMono(void) {}
void  SsSetSerialVol(char, short, short) {}
void  SsSetStereo(void) {}
void  SsSetTableSize(char*, short, short) {}
short SsUtKeyOffV(short voice) {return 0;}
short SsUtKeyOnV(short voice, short vabId, short prog, short tone,short note, short fine, short voll, short volr) {return 0;}
short SsUtSetVVol(short, short, short) {return 0;}
void  SsVabClose(short) {}
int StopCallback(void) {return 0;}
void SwEnterCriticalSection(void) {}
void SwExitCriticalSection(void) {}
void SetDispMask(int mask) {}
void  SsSetMute(char) {}
short SsVabOpenHeadSticky(unsigned char*, short, unsigned long) {return 0;}
int VSync(int mode) {return 0;}
int VSyncCallback(void (*f)()) {return 0;}
void SetFogNearFar(long a,long b,long h) {}
void  SsSetTickMode(long) {}

#endif


//*************************************
//***** PSX Geometry Functions ********
//*************************************


#define DCSHIFT 16

VECTOR *ApplyMatrix( MATRIX * m, SVECTOR * v0, VECTOR * v1 )
{
	long xt, yt, zt;
	long stage1, stage2, stage3;


	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	stage1 = (xt * (long)(m->m[0][0])) >> DCSHIFT;
	stage2 = (yt * (long)(m->m[0][1])) >> DCSHIFT;
	stage3 = (zt * (long)(m->m[0][2])) >> DCSHIFT;
	v1->vx = stage1+stage2+stage3+m->t[0];

	stage1 = (xt * (long)(m->m[1][0])) >> DCSHIFT;
	stage2 = (yt * (long)(m->m[1][1])) >> DCSHIFT;
	stage3 = (zt * (long)(m->m[1][2])) >> DCSHIFT;
	v1->vy = stage1+stage2+stage3+m->t[1];

	stage1 = (xt * (long)(m->m[2][0])) >> DCSHIFT;
	stage2 = (yt * (long)(m->m[2][1])) >> DCSHIFT;
	stage3 = (zt * (long)(m->m[2][2])) >> DCSHIFT;
	v1->vz = stage1+stage2+stage3+m->t[2];

	return v1;
}





short SsVabTransBodyPartly(unsigned char*, unsigned long, short) {return 0;}
SVECTOR *TransMultNormal_Light(SVECTOR *from,SVECTOR*to,int count) {return NULL;}
short SsVabTransCompleted(short) {return 0;}

};

MATRIX *TransMatrix(MATRIX *m,VECTOR *v)
{
m->t[0]+=v->vx;
m->t[1]+=v->vy;
m->t[2]+=v->vz;
}

MATRIX *SetMulRotMatrix(MATRIX *m0) {return NULL;}

long RotTransPers(SVECTOR *v0,long *sxy,long *p,long *flag)
{
#ifdef JCLDCNOPSX			// Don't need any of this...
	VECTOR 	r0;
	
	long x1,y1,z1;
	
	// SDS guessing that this is correct......	
	
	z1=RTM.t[2]+(RTM.m[2][0]*v0->vx+RTM.m[2][1]*v0->vy+RTM.m[2][2]*v0->vz)>> SHIFT;		
	
	sxy[0]=(((RTM.t[0]+(RTM.m[0][0]*v0->vx+RTM.m[0][1]*v0->vy+RTM.m[0][2]*v0->vz)>> 8) / z1) >> SHIFT) + 160;
	sxy[1]=(((RTM.t[1]+(RTM.m[1][0]*v0->vx+RTM.m[1][1]*v0->vy+RTM.m[1][2]*v0->vz)>> 8) / z1) >> SHIFT) + 100;
	
	return r0.vz >> 2;
#endif
}

SVECTOR *TransMultPoints(SVECTOR *from,SVECTOR*to,int count) 
{
long *DUMMY;
 for (int i=0;i<count;i++)
  { 
 	RotTransPers(from++,(long*)to++,DUMMY,DUMMY); 
  }
}

void SetRotMatrix(MATRIX *m) 
{
RTM.m[0][0]=m->m[0][0]; RTM.m[0][1]=m->m[0][1]; RTM.m[0][2]=m->m[0][2];
RTM.m[1][0]=m->m[1][0]; RTM.m[1][1]=m->m[0][1]; RTM.m[1][2]=m->m[1][2];
RTM.m[2][0]=m->m[2][0]; RTM.m[2][1]=m->m[0][1]; RTM.m[2][2]=m->m[2][2];
}
void SetTransMatrix(MATRIX *m) 
{
RTM.t[0]=m->t[0]; RTM.t[1]=m->t[1]; RTM.t[2]=m->t[2];
}
void SetGeomOffset(long ofx,long ofy) 
{
	xOffset = ofx;
	yOffset = ofy;
}

//*************************************
//***** SDS FGeometry Functions *******
//*************************************



VECTOR *FApplyMatrix( FMatrix * m, SVECTOR * v0, VECTOR * v1 )
{
	float xt, yt, zt;
	float stage1, stage2, stage3;


	xt = (float)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	stage1 = (xt *(m->Row[0].X)) ;
	stage2 = (yt *(m->Row[0].Y)) ;
	stage3 = (zt *(m->Row[0].Z)) ;
	v1->vx = stage1+stage2+stage3/*+m->t[0]*/;

	stage1 = (xt *(m->Row[1].X)) ;
	stage2 = (yt *(m->Row[1].Y)) ;
	stage3 = (zt *(m->Row[1].Z)) ;
	v1->vy = stage1+stage2+stage3/*+m->t[1]*/;

	stage1 = (xt *(m->Row[2].X)) ;
	stage2 = (yt *(m->Row[2].Y)) ;
	stage3 = (zt *(m->Row[2].Z)) ;
	v1->vz = stage1+stage2+stage3/*+m->t[2]*/;

	return v1;
}

FMatrix *FTransMatrix(FMatrix *m,VECTOR *v)
{
/*
m->t[0]+=v->vx;
m->t[1]+=v->vy;
m->t[2]+=v->vz;
*/
FTTM.X=v->vx;
FTTM.Y=v->vy;
FTTM.Z=v->vz;


}


FMatrix *FSetMulRotMatrix(FMatrix *m0) {return NULL;}

long FRotTransPers(FVector *v0,float *sxy,long *p,long *flag) {

	VECTOR 	r0;
	
	float x1,y1,z1;
	// SDS guessing that this is correct......	
	x1=FTTM.X+(FRTM.Row[0].X*v0->X+FRTM.Row[0].Y*v0->Y+FRTM.Row[0].Z*v0->Z);
	y1=FTTM.Y+(FRTM.Row[1].X*v0->X+FRTM.Row[1].Y*v0->Y+FRTM.Row[1].Z*v0->Z);
	z1=FTTM.Z+(FRTM.Row[2].X*v0->X+FRTM.Row[2].Y*v0->Y+FRTM.Row[2].Z*v0->Z);		
	
	sxy[0]=((x1*32 )/ z1)+320;
	sxy[1]=((y1*220)/ z1)+220;

	return r0.vz ;

}

SVECTOR *FTransMultPoints(FVector *from,FVector *to,int count) 
{
long *DUMMY;
 for (int i=0;i<count;i++)
  { 
 	FRotTransPers(from++,(float *)to++,DUMMY,DUMMY); 
  }
}

void FSetRotMatrix(FMatrix *m) 
{
FRTM.Row[0].X=m->Row[0].X; FRTM.Row[0].Y=m->Row[0].Y; FRTM.Row[0].Z=m->Row[0].Z;
FRTM.Row[1].X=m->Row[1].X; FRTM.Row[1].Y=m->Row[0].Y; FRTM.Row[1].Z=m->Row[1].Z;
FRTM.Row[2].X=m->Row[2].X; FRTM.Row[2].Y=m->Row[0].Y; FRTM.Row[2].Z=m->Row[2].Z;
}

void FSetTransMatrix(FMatrix *m) 
{
/*
FRTM.t[0]=m->t[0]; FRTM.t[1]=m->t[1]; FRTM.t[2]=m->t[2];
*/
//FTTM.X=v->vx;
//FTTM.Y=v->vy;
//FTTM.Z=v->vz
}

void FSetGeomOffset(long ofx,long ofy) 
{
	xOffset = ofx;
	yOffset = ofy;
}

void normalise(float *a, float *b,float *c)
{
float  nf=sqrt((*a)+(*b)+(*c));
*a=*a/nf;
*b=*b/nf;
*c=*c/nf;
}

void resetViewframe(viewframe *vf)
{
vf->iv[0]=1;  // x
vf->iv[1]=0;
vf->iv[2]=0;

vf->jv[0]=0;  // y - down
vf->jv[1]=1;
vf->jv[2]=0;

vf->kv[0]=0;  // z -
vf->kv[1]=0;
vf->kv[2]=1;
}

void InitViewFrame(viewframe *vf)
{
vf->x=1;
vf->y=1;
vf->z=1;

vf->iv[0]=1;  // x
vf->iv[1]=0;
vf->iv[2]=0;

vf->jv[0]=0;  // y - down
vf->jv[1]=1;
vf->jv[2]=0;

vf->kv[0]=0;  // z -
vf->kv[1]=0;
vf->kv[2]=1;
//rot_vx(2,vf);
}


void nrm_vec(float &n1, float &n2, float &n3)
{
 float mag_vec=1.0f/sqrt((n1*n1) + (n2*n2) + (n3*n3));
 n1=n1*mag_vec;
 n2=n2*mag_vec;
 n3=n3*mag_vec;
}

void  AxB(float *n,float *v1,float *v2)
{

float d0,d1,d2;

d0=(v1[1]*v2[2])-(v1[2]*v2[1]);
d1=(v1[2]*v2[0])-(v1[0]*v2[2]);
d2=(v1[0]*v2[1])-(v1[1]*v2[0]);

	//normalise the components
nrm_vec(d0,d1,d2);

n[0]=d0;
n[1]=d1;
n[2]=d2;
}

void regen_cosinesK(viewframe &vf)
{
	
 	nrm_vec(vf.kv[0],vf.kv[1],vf.kv[2]);	
	AxB(vf.iv,vf.jv,vf.kv);
	AxB(vf.jv,vf.kv,vf.iv);
}

void regen_cosinesI(viewframe &vf)
{
 nrm_vec(vf.iv[0],vf.iv[1],vf.iv[2]);
 AxB(vf.jv,vf.kv,vf.iv);
 AxB(vf.kv,vf.iv,vf.jv);
}


void regen_cosinesJ(viewframe &vf)
{
 nrm_vec(vf.jv[0],vf.jv[1],vf.jv[2]);
 AxB(vf.kv,vf.iv,vf.jv);
 AxB(vf.iv,vf.jv,vf.kv);
}

void regen_cosinesJI(viewframe &vf)
{
 nrm_vec(vf.jv[0],vf.jv[1],vf.jv[2]);
 AxB(vf.iv,vf.jv,vf.kv);
 AxB(vf.kv,vf.iv,vf.jv);
}
#pragma optimization_level 4
void PersTrans(int vn,viewframe vfCam, viewframe vf,tvector *in,tvector *out, SINT stride)
{
float n11,n12,n13,n21,n22,n23,n31,n32,n33;
float c11,c12,c13,c21,c22,c23,c31,c32,c33;
float f11,f12,f13,f21,f22,f23,f31,f32,f33;
float xAdd,yAdd,zAdd;
int lastvertex,pt;
float xw,yw,zw;
float xv,yv,zv;



n11=vf.iv[0];
n12=vf.iv[1];
n13=vf.iv[2];
n21=vf.jv[0];
n22=vf.jv[1];
n23=vf.jv[2];
n31=vf.kv[0];
n32=vf.kv[1];
n33=vf.kv[2];
c11=vfCam.iv[0];
c12=vfCam.iv[1];
c13=vfCam.iv[2];
c21=vfCam.jv[0];
c22=vfCam.jv[1];
c23=vfCam.jv[2];
c31=vfCam.kv[0];
c32=vfCam.kv[1];
c33=vfCam.kv[2];

f11=(c11*n11) + (c12*n12) + (c13*n13);
f12=(c11*n21) + (c12*n22) + (c13*n23);
f13=(c11*n31) + (c12*n32) + (c13*n33);
xAdd=((vf.x-vfCam.x)*c11) + ((vf.y-vfCam.y)*c12) + ((vf.z-vfCam.z)*c13);

f21=(c21*n11) + (c22*n12) + (c23*n13);
f22=(c21*n21) + (c22*n22) + (c23*n23);
f23=(c21*n31) + (c22*n32) + (c23*n33);
yAdd=((vf.x-vfCam.x)*c21) + ((vf.y-vfCam.y)*c22) + ((vf.z-vfCam.z)*c23);

f31=(c31*n11) + (c32*n12) + (c33*n13);
f32=(c31*n21) + (c32*n22) + (c33*n23);
f33=(c31*n31) + (c32*n32) + (c33*n33);
zAdd=((vf.x-vfCam.x)*c31) + ((vf.y-vfCam.y)*c32) + ((vf.z-vfCam.z)*c33);

pt = 0;

for (int c0=0;c0<vn;c0++)
 {
   xw=in[pt].x;
   yw=in[pt].y;
   zw=in[pt].z;
   xv=(xw*f11)  + (yw*f12) + (zw*f13)+xAdd;
   yv=(xw*f21)  + (yw*f22) + (zw*f23)+yAdd;
   zv=((xw*f31)  + (yw*f32) + (zw*f33)+zAdd);
   
   if (zv==0) zv=1;
   zv=1/zv;
   out[pt].x=(((-xv*320*(DCscale))*zv)+InternalViewOffset);
   out[pt].y=((yv*320*(DCscale))*zv)+240;
   out[pt].z=zv*10;
   pt += stride;
  }
}


void PersTrans2(int vn,viewframe vfCam, viewframe vf,tvector *in,tvector *out)
{
float n11,n12,n13,n21,n22,n23,n31,n32,n33;
float c11,c12,c13,c21,c22,c23,c31,c32,c33;
float f11,f12,f13,f21,f22,f23,f31,f32,f33;
float xAdd,yAdd,zAdd;
int lastvertex,pt;
float xw,yw,zw;
float xv,yv,zv;



n11=vf.iv[0];
n12=vf.iv[1];
n13=vf.iv[2];
n21=vf.jv[0];
n22=vf.jv[1];
n23=vf.jv[2];
n31=vf.kv[0];
n32=vf.kv[1];
n33=vf.kv[2];
c11=vfCam.iv[0];
c12=vfCam.iv[1];
c13=vfCam.iv[2];
c21=vfCam.jv[0];
c22=vfCam.jv[1];
c23=vfCam.jv[2];
c31=vfCam.kv[0];
c32=vfCam.kv[1];
c33=vfCam.kv[2];

f11=(c11*n11) + (c12*n12) + (c13*n13);
f12=(c11*n21) + (c12*n22) + (c13*n23);
f13=(c11*n31) + (c12*n32) + (c13*n33);
xAdd=((vf.x-vfCam.x)*c11) + ((vf.y-vfCam.y)*c12) + ((vf.z-vfCam.z)*c13);

f21=(c21*n11) + (c22*n12) + (c23*n13);
f22=(c21*n21) + (c22*n22) + (c23*n23);
f23=(c21*n31) + (c22*n32) + (c23*n33);
yAdd=((vf.x-vfCam.x)*c21) + ((vf.y-vfCam.y)*c22) + ((vf.z-vfCam.z)*c23);

f31=(c31*n11) + (c32*n12) + (c33*n13);
f32=(c31*n21) + (c32*n22) + (c33*n23);
f33=(c31*n31) + (c32*n32) + (c33*n33);
zAdd=((vf.x-vfCam.x)*c31) + ((vf.y-vfCam.y)*c32) + ((vf.z-vfCam.z)*c33);

for (int pt=0;pt<vn;pt++)
 {
   xw=in[pt].x;
   yw=in[pt].y;
   zw=in[pt].z;
   xv=(xw*f11)  + (yw*f12) + (zw*f13)+xAdd;
   yv=(xw*f21)  + (yw*f22) + (zw*f23)+yAdd;
   zv=((xw*f31)  + (yw*f32) + (zw*f33)+zAdd);
   
   if (zv==0) zv=1;
   //zv=1/zv;
   out[pt].x=xv;
   out[pt].y=yv;
   out[pt].z=zv;
  }
}


void FMatrixToViewFrame(FMatrix *in,viewframe *out)
{
out->iv[0]=in->Row[0].X;
out->iv[1]=in->Row[0].Y;
out->iv[2]=in->Row[0].Z;

out->jv[0]=-in->Row[1].X;
out->jv[1]=-in->Row[1].Y;
out->jv[2]=-in->Row[1].Z;

out->kv[0]=-in->Row[2].X;
out->kv[1]=-in->Row[2].Y;
out->kv[2]=-in->Row[2].Z;
}


void PersTransWrapper(int mVertexCount,tvector *in,tvector *out,const FVector LocalPos,const FMatrix LocalOrientation, SINT stride)
{
			
			viewframe cam;
			InitViewFrame(&cam);
			FVector CPos,DPos;
			// sds use looktat instead of the matrix
			CCamera *xcam =	ENGINE.GetCamera();
			CPos=GVectorToFVector(xcam->mPos);
			DPos=GVectorToFVector(xcam->mLookAt);
			
			cam.x=CPos.X;
			cam.z=CPos.Y;
			cam.y=CPos.Z;	
			
			cam.kv[0]=cam.x-DPos.X;
			cam.kv[1]=cam.y-DPos.Z;
			cam.kv[2]=cam.z-DPos.Y;
			
			float norm;
			norm=sqrt((cam.kv[0]*cam.kv[0])+(cam.kv[1]*cam.kv[1])+(cam.kv[2]*cam.kv[2]));
			
			cam.kv[0]=cam.kv[0]/norm;
			cam.kv[1]=cam.kv[1]/norm;
			cam.kv[2]=cam.kv[2]/norm;
			
			cam.jv[0]=cam.kv[2];
			cam.jv[1]=0;
			cam.jv[2]=-cam.kv[0];
			
			regen_cosinesK(cam);
			rot_vz(-3.14159265/2,cam);
			regen_cosinesK(cam);
		
			viewframe CharFrame;
			InitViewFrame(&CharFrame);
			
			
			CharFrame.kv[0]=LocalOrientation.Row[0].Y;
			CharFrame.kv[1]=LocalOrientation.Row[2].Y;			
			CharFrame.kv[2]=LocalOrientation.Row[1].Y;			

			CharFrame.jv[0]=LocalOrientation.Row[0].Z;
			CharFrame.jv[1]=LocalOrientation.Row[2].Z;			
			CharFrame.jv[2]=LocalOrientation.Row[1].Z;
			
			CharFrame.iv[0]=LocalOrientation.Row[0].X;
			CharFrame.iv[1]=LocalOrientation.Row[2].X;			
			CharFrame.iv[2]=LocalOrientation.Row[1].X;						

			CharFrame.x=LocalPos.X;
			CharFrame.y=LocalPos.Z;
			CharFrame.z=LocalPos.Y;			
			PersTrans(mVertexCount,cam,CharFrame,(tvector *)in,(tvector *)out, stride);

}

void PersTransWrapper2(int mVertexCount,tvector *in,tvector *out,const FVector LocalPos,const FMatrix LocalOrientation)
{
			
			viewframe cam;
			InitViewFrame(&cam);
			FVector CPos,DPos;
			// sds use looktat instead of the matrix
			CCamera *xcam =	ENGINE.GetCamera();
			CPos=GVectorToFVector(xcam->mPos);
			DPos=GVectorToFVector(xcam->mLookAt);
			
			cam.x=CPos.X;
			cam.z=CPos.Y;
			cam.y=CPos.Z;	
			
			cam.kv[0]=cam.x-DPos.X;
			cam.kv[1]=cam.y-DPos.Z;
			cam.kv[2]=cam.z-DPos.Y;
			
			float norm;
			norm=sqrt((cam.kv[0]*cam.kv[0])+(cam.kv[1]*cam.kv[1])+(cam.kv[2]*cam.kv[2]));
			
			cam.kv[0]=cam.kv[0]/norm;
			cam.kv[1]=cam.kv[1]/norm;
			cam.kv[2]=cam.kv[2]/norm;
			
			cam.jv[0]=cam.kv[2];
			cam.jv[1]=0;
			cam.jv[2]=-cam.kv[0];
			
			regen_cosinesK(cam);
			rot_vz(-3.14159265/2,cam);
			regen_cosinesK(cam);
		
			viewframe CharFrame;
			InitViewFrame(&CharFrame);
			
			
			CharFrame.kv[0]=LocalOrientation.Row[0].Y;
			CharFrame.kv[1]=LocalOrientation.Row[2].Y;			
			CharFrame.kv[2]=LocalOrientation.Row[1].Y;			

			CharFrame.jv[0]=LocalOrientation.Row[0].Z;
			CharFrame.jv[1]=LocalOrientation.Row[2].Z;			
			CharFrame.jv[2]=LocalOrientation.Row[1].Z;
			
			CharFrame.iv[0]=LocalOrientation.Row[0].X;
			CharFrame.iv[1]=LocalOrientation.Row[2].X;			
			CharFrame.iv[2]=LocalOrientation.Row[1].X;						

			CharFrame.x=LocalPos.X;
			CharFrame.y=LocalPos.Z;
			CharFrame.z=LocalPos.Y;			
			PersTrans2(mVertexCount,cam,CharFrame,(tvector *)in,(tvector *)out);

}


void rot_vx(float rotAng,viewframe *vf)
{
float sinT,cosT;

	sinT=sin(rotAng);
    cosT=cos(rotAng);
	vf->kv[0]=(-vf->jv[0]*sinT) + (vf->kv[0]*cosT);
	vf->kv[1]=(-vf->jv[1]*sinT) + (vf->kv[1]*cosT);
	vf->kv[2]=(-vf->jv[2]*sinT) + (vf->kv[2]*cosT);

	vf->jv[0]=(vf->jv[0]*cosT) + (vf->kv[0]*sinT);
	vf->jv[1]=(vf->jv[1]*cosT) + (vf->kv[1]*sinT);
	vf->jv[2]=(vf->jv[2]*cosT) + (vf->kv[2]*sinT);
}

void rot_vy(float rotAng,viewframe *vf)
{
float sinT,cosT;

	sinT=sin(rotAng);
    cosT=cos(rotAng);

	vf->iv[0]=(vf->iv[0]*cosT) - (vf->kv[0]*sinT);
	vf->iv[1]=(vf->iv[1]*cosT) - (vf->kv[1]*sinT);
	vf->iv[2]=(vf->iv[2]*cosT) - (vf->kv[2]*sinT);


	vf->kv[0]=(vf->iv[0]*sinT) + (vf->kv[0]*cosT);
	vf->kv[1]=(vf->iv[1]*sinT) + (vf->kv[1]*cosT);
	vf->kv[2]=(vf->iv[2]*sinT) + (vf->kv[2]*cosT);


}

void rot_vz(float rotAng,viewframe &vf)
{
float sinT,cosT;

	sinT=sin(rotAng);
    cosT=cos(rotAng);


	vf.jv[0]=-vf.iv[0]*sinT + vf.jv[0]*cosT;
	vf.jv[1]=-vf.iv[1]*sinT + vf.jv[1]*cosT;
	vf.jv[2]=-vf.iv[2]*sinT + vf.jv[2]*cosT;

	vf.iv[0]=vf.iv[0]*cosT + vf.jv[0]*sinT;
	vf.iv[1]=vf.iv[1]*cosT + vf.jv[1]*sinT;
	vf.iv[2]=vf.iv[2]*cosT + vf.jv[2]*sinT;
}

#pragma optimization_level 1
#endif
