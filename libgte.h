/*
 *  (C) Copyright 1993/1994 Sony Computer Entertainment ,Tokyo,Japan.
 *				All Rights Reserved
 *
 * 		libgte.h: Geometry Basic Structures Database
 *
 *$Id$
 */


//
// Converted to WATCOM C v10.0 by 
// Salim Siwani, Psygnosis Ltd
//       06/02/1995
//


#include "pragmas.h"

#ifndef LIBGTE_H
#define LIBGTE_H



/*
 * Geometry Structures:
 */

//#define ONE		4096	/* GTE regards 4096 as 1.0 */
//#define SHIFT		12		/* number of bits to shift for 4096 */
//#define SHIFT2	24		/*  */

//#define ONE		65536	/* GTE now regards 65536 as 1.0 */
//#define SHIFT		16		/* number of bits to shift for 4096 */
//#define SHIFT2	32		/*  */

#define	ONE			4096
#define ONE_FLOAT	4096.0
#define SHIFT		12		/* number of bits to shift for 4096 */
#define SHIFT2		24		/*  */

#define TMPVTXNUM	16		/* Clip Function Header */
#define OUTVTXNUM	10


#define MULT( a, b )		(((long)(a) * (long)(b)) >> SHIFT)



#ifdef ASSEMBLER
/*
* GTE read macros
*/
#define	read_sz_fifo3(r1,r2,r3)		mfc2	r1,$17; \
					mfc2	r2,$18; \
					mfc2	r3,$19; \
					nop

#define	read_sz_fifo4(r1,r2,r3,r4) 	mfc2	r1,$16; \
					mfc2	r2,$17; \
					mfc2	r3,$18; \
					mfc2	r4,$19;	\
					nop

#define	read_szx(r1) 			mfc2	r1,$16;	\
					nop
			
#define	read_sz0(r1)			mfc2	r1,$17;	\
					nop
			
#define	read_sz1(r1)			mfc2	r1,$18;	\
					nop
			
#define	read_sz2(r1)			mfc2	r1,$19;	\
					nop
			
#define	read_sxsy_fifo3(r1,r2,r3) 	mfc2	r1,$12; \
					mfc2	r2,$13; \
					mfc2	r3,$14;	\
					nop

#define	read_sxsy0(r1)			mfc2	r1,$12;	\
					nop

#define	read_sxsy1(r1)			mfc2	r1,$13;	\
					nop

#define	read_sxsy2(r1)			mfc2	r1,$14;	\
					nop

#define	read_rgb_fifo(r1,r2,r3)		mfc2	r1,$20; \
					mfc2	r2,$21; \
					mfc2	r3,$22;	\
					nop

#define	read_rgb0(r1)			mfc2	r1,$20; \
					nop

#define	read_rgb1(r1)			mfc2	r1,$21; \
					nop

#define	read_rgb2(r1)			mfc2	r1,$22; \
					nop

#define	read_flag(r1)			cfc2	r1,$31;	\
					nop

#define	read_p(r1)			mfc2	r1,$8;	\
					nop

#define	read_otz(r1)			mfc2	r1,$7;	\
					nop

#define	read_opz(r1)			mfc2	r1,$24;	\
					nop

#define read_mt(r1,r2,r3)		mfc2	r1,$25; \
					mfc2	r2,$26; \
					mfc2	r3,$27; \
					nop
/*
* GTE set macros
*/
#define set_trans_matrix(r1,r2,r3)	ctc2	r1,$5;	\
					ctc2	r2,$6;	\
					ctc2	r3,$7;	\
					nop
#endif











#ifndef ASSEMBLER
typedef struct  {
	short	m[3][3];	/* 3x3 rotation matrix */
    long    t[3];		/* transfer vector */
} MATRIX;

typedef struct {		/* long word type 3D vector */
	long	vx, vy;
	long	vz, pad;
} VECTOR;
	
typedef struct {
	short	vx,vy;
	short	vz,pad;
}SVECTOR;

typedef struct {		/* color type vector */	
	u_char	r, g, b, cd;
} CVECTOR;
	       
typedef struct {		/* 2D short vector */
	short vx, vy;
} DVECTOR;


typedef struct {		
	SVECTOR v;		/* Object(Local) 3D Vertex 	*/
	VECTOR sxyz;		/* Screen 3D Vertex		*/
	DVECTOR sxy;		/* Screen 2D Vertex		*/
	CVECTOR rgb;		/* Vertex Color Data	 	*/
	short txuv,pad;		/* Texture Mapping Data 	*/
	long chx,chy;		/* Clip Window Data 		*/
} EVECTOR;

typedef struct {
	SVECTOR v;
	u_char uv[2]; u_short pad;	/*　　*/  
	CVECTOR c;
	DVECTOR sxy;		
	u_long  sz;		/*　clip z-data　*/		
} RVECTOR;			/*　分割頂点情報ベクタ　*/


typedef struct {
	RVECTOR r01,r12,r20;
	RVECTOR	*r0,*r1,*r2;
	u_long *rtn;
} CRVECTOR3;			/*　３角形用再帰ベクタ　*/

typedef struct {
	u_long 	ndiv;		/*　分割数　*/
	u_long 	pih,piv;	/*　クリップエリア　*/
	u_short clut,tpage;
	CVECTOR	rgbc;
	u_long	*ot;
	RVECTOR r0,r1,r2;
	CRVECTOR3 cr[5];	
} DIVPOLYGON3;			/*　３角形用分割バッファ　*/

typedef struct {
	RVECTOR r01,r02,r31,r32,rc;
	RVECTOR	*r0,*r1,*r2,*r3;
	u_long *rtn;
} CRVECTOR4;			/*　４角形用再帰ベクタ　*/

typedef struct {
	u_long 	ndiv;		/*　分割数　*/
	u_long 	pih,piv;	/*　クリップエリア　*/
	u_short clut,tpage;
	CVECTOR	rgbc;
	u_long	*ot;
	RVECTOR r0,r1,r2,r3;
	CRVECTOR4 cr[5];	
} DIVPOLYGON4;			/*　４角形用分割バッファ　*/

typedef struct {
        short   xy[3];
        short   uv[2];
        short   rgb[3];
} SPOL;

                                        /*polygon: 41 bytes/1 polygon*/
typedef struct {
        short   sxy[4][2];              /*0..7*/
        short   sz[4][2];               /*8..15sz[][1] is dummy*/
        short   uv[4][2];               /*16..23*/
        short   rgb[4][3];              /*23..34*/
        short   code;                   /*35...  F4:5,TF4:6,G4:7,TG4:8*/
} POL4;

typedef struct {
        short   sxy[3][2];
        short   sz[3][2];               /*sz[][1] is dummy*/
        short   uv[3][2];
        short   rgb[3][3];
        short   code;                   /*F3:1,TF3:2,G3:3,TG3:4*/
} POL3;


typedef struct
{
	/* internal GTE color type vector */	
	u_long	r, g, b, cd;
} GTECVECTOR;


#endif
#include "libgpu.h"

/*
 *      ProtoTypes
 */
#ifndef NO_PROTOTYPE

void InitGeom();

MATRIX *MulMatrix0(MATRIX *m0,MATRIX *m1,MATRIX *m2);
MATRIX *MulRotMatrix0(MATRIX *m0,MATRIX *m1);
MATRIX *MulMatrix(MATRIX *m0,MATRIX *m1);
MATRIX *MulMatrix2(MATRIX *m0,MATRIX *m1);
MATRIX *MulRotMatrix(MATRIX *m0);
MATRIX *SetMulMatrix(MATRIX *m0,MATRIX *m1);

VECTOR *ApplyMatrix(MATRIX *m,SVECTOR *v0,VECTOR *v1);

long *DLSRotTransPers(long *sxy, VECTOR *r0, long *flag);


VECTOR *ApplyRotMatrix(SVECTOR *v0,VECTOR *v1);
VECTOR *ApplyMatrixLV(MATRIX *m,VECTOR *v0,VECTOR *v1);
MATRIX *RotMatrix(SVECTOR *r,MATRIX *m);
MATRIX *RotMatrixYXZ(SVECTOR *r,MATRIX *m);
MATRIX *RotMatrixX(long r,MATRIX *m);
MATRIX *RotMatrixY(long r,MATRIX *m);
MATRIX *RotMatrixZ(long r,MATRIX *m);
MATRIX *RotMatrixC(SVECTOR *r,MATRIX *m);
MATRIX *TransMatrix(MATRIX *m,VECTOR *v);
MATRIX *ScaleMatrix(MATRIX *m,VECTOR *v);
MATRIX *ScaleMatrixL(MATRIX *m,VECTOR *v);
MATRIX *TransposeMatrix(MATRIX *m0,MATRIX *m1);
MATRIX *CompMatrix(MATRIX *m0,MATRIX *m1,MATRIX *m2);
void SetRotMatrix(MATRIX *m); 
void SetLightMatrix(MATRIX *m);
void SetColorMatrix(MATRIX *m);
void SetTransMatrix(MATRIX *m);
void PushMatrix();
void PopMatrix();
void ReadRotMatrix(MATRIX *m);
void ReadLightMatrix(MATRIX *m);
void ReadColorMatrix(MATRIX *m);
void SetRGBcd(CVECTOR *v);
void SetBackColor(long rbk,long gbk,long bbk);
void SetFarColor(long rfc,long gfc,long bfc);
void SetGeomOffset(long ofx,long ofy);
void SetGeomScreen(long h);
void ReadSZfifo3(long *sz0,long *sz1,long *sz2);
void ReadSZfifo4(long *szx,long *sz0,long *sz1,long *sz2);
void ReadSXSYfifo(long *sxy0,long *sxy1,long *sxy2);
void ReadRGBfifo(CVECTOR *v0,CVECTOR *v1,CVECTOR *v2);
void ReadGeomOffset(long *ofx,long *ofy);
long ReadGeomScreen();

long RotTransPers(SVECTOR *v0,long *sxy,long *p,long *flag);
long RotTransPersForN(SVECTOR *v0,long *sxy,long *p,long *flag);
long RotTransPers3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			long *sxy0,long *sxy1,long *sxy2,long *p,long *flag);
void RotTrans(SVECTOR *v0,VECTOR *v1,long *flag);
void LocalLight(SVECTOR *v0,VECTOR *v1);
void LightColor(VECTOR *v0,VECTOR *v1);
void DpqColorLight(VECTOR *v0,CVECTOR *v1,long p,CVECTOR *v2);
void DpqColor(CVECTOR *v0,long p,CVECTOR *v1);
void DpqColor3(CVECTOR *v0,CVECTOR *v1,CVECTOR *v2,
			long p,CVECTOR *v3,CVECTOR *v4,CVECTOR *v5);
void Intpl(VECTOR *v0,long p,CVECTOR *v1);
VECTOR *Square12(VECTOR *v0,VECTOR *v1);
VECTOR *Square0(VECTOR *v0,VECTOR *v1);
void NormalColor(SVECTOR *v0,CVECTOR *v1);
void NormalColor3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			CVECTOR *v3,CVECTOR *v4,CVECTOR *v5);
void NormalColorDpq(SVECTOR *v0,CVECTOR *v1,long p,CVECTOR *v2);
void NormalColorDpq3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,CVECTOR *v3,
			long p,CVECTOR *v4,CVECTOR *v5,CVECTOR *v6);
void NormalColorCol(SVECTOR *v0,CVECTOR *v1,CVECTOR *v2);
void NormalColorCol3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,CVECTOR *v3,
			CVECTOR *v4,CVECTOR *v5,CVECTOR *v6);
void ColorDpq(VECTOR *v0,CVECTOR *v1,long p,CVECTOR *v2);
void ColorCol(VECTOR *v0,CVECTOR *v1,CVECTOR *v2);
long NormalClip(long sxy0,long sxy1,long sxy2);


long AverageZ3(long sz0,long sz1,long sz2);


long AverageZ4(long sz0,long sz1,long sz2,long sz3);


void OuterProduct12(VECTOR *v0,VECTOR *v1,VECTOR *v2);
void OuterProduct0(VECTOR *v0,VECTOR *v1,VECTOR *v2);
long Lzc(long data);


long RotTransPers4(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
			long *sxy0,long *sxy1,long *sxy2,long *sxy3,
			long *p,long *flag);
void RotTransPersN(SVECTOR *v0,DVECTOR *v1,u_short *sz,u_short *flag,long n, u_short *origz);
void RotTransPers3N(SVECTOR *v0,DVECTOR *v1,u_short *sz,u_short *flag,long n);
void RotMeshH(short *Yheight,DVECTOR *Vo,u_short *sz,u_short *flag,
			short Xoffset,short Zoffset,short m,short n,
			DVECTOR *base);
long RotAverage3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			long *sxy0,long *sxy1,long *sxy2,long *p,long *flag);
long RotAverage4(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
			long *sxy0,long *sxy1,long *sxy2,long *sxy3,
			long *p,long *flag);
long RotNclip3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			long *sxy0,long *sxy1,long *sxy2,long *p,long *otz,
			long *flag);
long RotNclip4(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
			long *sxy0,long *sxy1,long *sxy2,long *sxy3,
			long *p,long *otz,long *flag);
long RotAverageNclip3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			long *sxy0,long *sxy1,long *sxy2,
			long *p,long *otz,long *flag);
long RotAverageNclip4(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
			long *sxy0,long *sxy1,long *sxy2,long *sxy3,
			long *p,long *otz,long *flag);
long RotColorDpq(SVECTOR *v0,SVECTOR *v1,CVECTOR *v2,
			long *sxy,CVECTOR *v3,long *flag);
long RotColorDpq3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			SVECTOR *v3,SVECTOR *v4,SVECTOR *v5,CVECTOR *v6,
			long *sxy0,long *sxy1,long *sxy2,
			CVECTOR *v7,CVECTOR *v8,CVECTOR *v9,long *flag);
long RotAverageNclipColorDpq3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			SVECTOR *v3,SVECTOR *v4,SVECTOR *v5,CVECTOR *v6,
			long *sxy0,long *sxy1,long *sxy2,
			CVECTOR *v7,CVECTOR *v8,CVECTOR *v9,
			long *otz,long *flag);	 		   	      
long RotAverageNclipColorCol3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,SVECTOR *v4,SVECTOR *v5,CVECTOR *v6,
			long *sxy0,long *sxy1,long *sxy2,CVECTOR *v7,CVECTOR *v8,CVECTOR *v9,
			long *otz,long *flag);
long RotColorMatDpq(SVECTOR *v0,SVECTOR *v1,CVECTOR *v2,long *sxy,CVECTOR *v3,long matc,long *flag);
void ColorMatDpq(SVECTOR *v0,CVECTOR *v1,long p,CVECTOR *v2,long matc);
void ColorMatCol(SVECTOR *v0,CVECTOR *v1,CVECTOR *v2,long matc);
void InterpolByte(u_char *in1, u_char *in2, long p0, u_char *rtn);
void InterpolShort(SVECTOR *vi0, SVECTOR *vi1, long p0, SVECTOR *vt1);
void LoadAverage12(VECTOR *v0,VECTOR *v1,long p0,long p1,VECTOR *v2);
void LoadAverageShort12(SVECTOR *v0,SVECTOR *v1,long p0,long p1,SVECTOR *v2);
void LoadAverage0(VECTOR *v0,VECTOR *v1,long p0,long p1,VECTOR *v2);
void LoadAverageShort0(SVECTOR *v0,SVECTOR *v1,long p0,long p1,SVECTOR *v2);
void LoadAverageByte(u_char *v0,u_char *v1,long p0,long p1,u_char *v2);
void LoadAverageCol(u_char *v0,u_char *v1,long p0,long p1,u_char *v2);
void VectorNormal(VECTOR *v0, VECTOR *v1);
void VectorNormalS(VECTOR *v0, SVECTOR *v1);
long SquareRoot0(long a);
long SquareRoot12(long a);
void InvSquareRoot(long a, long *b, long *c);
void gteMIMefunc(SVECTOR *otp, SVECTOR *dfp, long n, long p);
void SetFogFar(long a,long h);
void SetFogNear(long a,long h);
void SubPol4(POL4 *p, SPOL *sp, int ndiv);
void SubPol3(POL3 *p, SPOL *sp, int ndiv);

long slow_rsin(long a);
long slow_rcos(long a);
long rcos(long a);
long rsin(long a);
long ccos(long a);
long csin(long a);
long csqrt(long a);
long cln(long a);
long catan(long a);
long ratan2(long y, long x);


void RotPMD_F3(long *pa,u_long *ot,int otlen,int id,int backc);
void RotPMD_G3(long *pa,u_long *ot,int otlen,int id,int backc);
void RotPMD_FT3(long *pa,u_long *ot,int otlen,int id,int backc);
void RotPMD_GT3(long *pa,u_long *ot,int otlen,int id,int backc);
void RotPMD_F4(long *pa,u_long *ot,int otlen,int id,int backc);
void RotPMD_G4(long *pa,u_long *ot,int otlen,int id,int backc);
void RotPMD_FT4(long *pa,u_long *ot,int otlen,int id,int backc);
void RotPMD_GT4(long *pa,u_long *ot,int otlen,int id,int backc);

void RotPMD_SV_F3(long *pa,long *va,u_long *ot,int otlen,int id,int backc);
void RotPMD_SV_G3(long *pa,long *va,u_long *ot,int otlen,int id,int backc);
void RotPMD_SV_FT3(long *pa,long *va,u_long *ot,int otlen,int id,int backc);
void RotPMD_SV_GT3(long *pa,long *va,u_long *ot,int otlen,int id,int backc);
void RotPMD_SV_F4(long *pa,long *va,u_long *ot,int otlen,int id,int backc);
void RotPMD_SV_G4(long *pa,long *va,u_long *ot,int otlen,int id,int backc);
void RotPMD_SV_FT4(long *pa,long *va,u_long *ot,int otlen,int id,int backc);
void RotPMD_SV_GT4(long *pa,long *va,u_long *ot,int otlen,int id,int backc);


void InitClip(EVECTOR *evbfad,long hw,long vw,long h,long Near,long Far);
long Clip3F(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,EVECTOR **evmx);
long Clip3FP(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,EVECTOR **evmx);
long Clip4F(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,EVECTOR **evmx);
long Clip4FP(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,EVECTOR **evmx);
long Clip3FT(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		short *uv0,short *uv1,short *uv2,EVECTOR **evmx);
long Clip3FTP(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		short *uv0,short *uv1,short *uv2,EVECTOR **evmx);
long Clip4FT(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		short *uv0,short *uv1,short *uv2,short *uv3,EVECTOR **evmx);
long Clip4FTP(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		short *uv0,short *uv1,short *uv2,short *uv3,EVECTOR **evmx);
long Clip3G(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,EVECTOR **evmx);
long Clip3GP(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,EVECTOR **evmx);
long Clip4G(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,CVECTOR *rgb3,
		EVECTOR **evmx);
long Clip4GP(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,CVECTOR *rgb3,
		EVECTOR **evmx);
long Clip3GT(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		short *uv0,short *uv1,short *uv2,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,
		EVECTOR **evmx);
long Clip3GTP(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		short *uv0,short *uv1,short *uv2,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,
		EVECTOR **evmx);
long Clip4GT(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		short *uv0,short *uv1,short *uv2,short *uv3,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,CVECTOR *rgb3,
		EVECTOR **evmx);
long Clip4GTP(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		short *uv0,short *uv1,short *uv2,short *uv3,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,CVECTOR *rgb3,
		EVECTOR **evmx);

void RotTransPers_nom(SVECTOR *v0);
void RotTransPers3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2);
void RotTransPers4_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3);
void RotTrans_nom(SVECTOR *v0);
void RotAverage3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2);
void RotNclip3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2);
void RotAverageNclip3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2);
void RotAverageNclipColorDpq3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			SVECTOR *v3,SVECTOR *v4,SVECTOR *v5,CVECTOR *v6);
void RotAverageNclipColorCol3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			SVECTOR *v3,SVECTOR *v4,SVECTOR *v5,CVECTOR *v6);
void RotColorDpq_nom(SVECTOR *v0,SVECTOR *v1,CVECTOR *v2);
long RotColorDpq3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			SVECTOR *v3,SVECTOR *v4,SVECTOR *v5,CVECTOR *v6);
void NormalColor_nom(SVECTOR *v0);
void NormalColor3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2);
void NormalColorDpq_nom(SVECTOR *v0,CVECTOR *v1,long p);
void NormalColorDpq3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
			CVECTOR *v3,long p);
void NormalColorCol_nom(SVECTOR *v0,CVECTOR *v1);
void NormalColorCol3_nom(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,CVECTOR *v3);

u_long *DivideF3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,CVECTOR *rgbc,
		POLY_F3 *s,u_long *ot,DIVPOLYGON3 *divp);

u_long *DivideF4(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		CVECTOR *rgbc,POLY_F4 *s,u_long *ot,DIVPOLYGON4 *divp);
u_long *DivideFT3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		u_long *uv0,u_long *uv1,u_long *uv2,CVECTOR *rgbc,
		POLY_FT3 *s,u_long *ot,DIVPOLYGON3 *divp);
u_long *DivideFT4(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		u_long *uv0,u_long *uv1,u_long *uv2,u_long *uv3,
		CVECTOR *rgbc,POLY_FT4 *s,u_long *ot,DIVPOLYGON4 *divp);
u_long *DivideG3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,
		POLY_G3 *s,u_long *ot,DIVPOLYGON3 *divp);
u_long *DivideG4(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,CVECTOR *rgb3,
		POLY_G4 *s,u_long *ot,DIVPOLYGON4 *divp);
u_long *DivideGT3(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,
		u_long *uv0,u_long *uv1,u_long *uv2,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,
		POLY_GT3 *s,u_long *ot,DIVPOLYGON3 *divp);
u_long *DivideGT4(SVECTOR *v0,SVECTOR *v1,SVECTOR *v2,SVECTOR *v3,
		u_long *uv0,u_long *uv1,u_long *uv2,u_long *uv3,
		CVECTOR *rgb0,CVECTOR *rgb1,CVECTOR *rgb2,CVECTOR *rgb3,
		POLY_GT4 *s,u_long *ot,DIVPOLYGON4 *divp);

u_long *RCpolyF3(POLY_F3 *s,DIVPOLYGON3 *divp);
u_long *RCpolyF4(POLY_F4 *s,DIVPOLYGON4 *divp);
u_long *RCpolyFT3(POLY_FT3 *s,DIVPOLYGON3 *divp);
u_long *RCpolyFT4(POLY_FT4 *s,DIVPOLYGON4 *divp);
u_long *RCpolyG3(POLY_G3 *s,DIVPOLYGON3 *divp);
u_long *RCpolyG4(POLY_G4 *s,DIVPOLYGON4 *divp);
u_long *RCpolyGT3(POLY_GT3 *s,DIVPOLYGON3 *divp);
u_long *RCpolyGT4(POLY_GT4 *s,DIVPOLYGON4 *divp);

/*
u_long *GsTMDfastF3L(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastF3LFG(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastF3NL(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastNF3(TMD_P_NF3 *primtop,SVECTOR *vertop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsTMDfastF4L(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastF4LFG(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastF4NL(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastNF4(TMD_P_NF4 *primtop,SVECTOR *vertop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsTMDfastTF3L(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTF3LFG(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTF3NL(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTNF3(TMD_P_TNF3 *primtop,SVECTOR *vertop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsTMDfastTF4L(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTF4LFG(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTF4NL(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTNF4(TMD_P_TNF4 *primtop,SVECTOR *vertop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsTMDfastG3L(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastG3LFG(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastG3NL(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastNG3(TMD_P_NG3 *primtop,SVECTOR *vertop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsTMDfastG4L(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastG4LFG(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastG4NL(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastNG4(TMD_P_NG4 *primtop,SVECTOR *vertop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsTMDfastTG3L(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTG3LFG(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTG3NL(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTNG3(TMD_P_TNG3 *primtop,SVECTOR *vertop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsTMDfastTG4L(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTG4LFG(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTG4NL(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsTMDfastTNG4(TMD_P_TNG4 *primtop,SVECTOR *vertop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsPrstF3L(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstF3LFG(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstF3NL(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstNF3(TMD_P_NF3 *primtop,SVECTOR *vertop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsPrstF4L(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstF4LFG(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstF4NL(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstNF4(TMD_P_NF4 *primtop,SVECTOR *vertop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsPrstTF3L(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTF3LFG(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTF3NL(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTNF3(TMD_P_TNF3 *primtop,SVECTOR *vertop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsPrstTF4L(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTF4LFG(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTF4NL(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTNF4(TMD_P_TNF4 *primtop,SVECTOR *vertop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsPrstG3L(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstG3LFG(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstG3NL(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstNG3(TMD_P_NG3 *primtop,SVECTOR *vertop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsPrstG4L(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstG4LFG(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstG4NL(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstNG4(TMD_P_NG4 *primtop,SVECTOR *vertop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsPrstTG3L(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTG3LFG(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTG3NL(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTNG3(TMD_P_TNG3 *primtop,SVECTOR *vertop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsPrstTG4L(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTG4LFG(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTG4NL(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp);
u_long *GsPrstTNG4(TMD_P_TNG4 *primtop,SVECTOR *vertop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp);

u_long *GsTMDdivF3L(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivF3LFG(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivF3NL(TMD_P_F3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivNF3(TMD_P_NF3 *primtop,SVECTOR *vertop,
		POLY_F3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);

u_long *GsTMDdivF4L(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivF4LFG(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivF4NL(TMD_P_F4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivNF4(TMD_P_NF4 *primtop,SVECTOR *vertop,
		POLY_F4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);

u_long *GsTMDdivTF3L(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivTF3LFG(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivTF3NL(TMD_P_TF3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivTNF3(TMD_P_TNF3 *primtop,SVECTOR *vertop,
		POLY_FT3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);

u_long *GsTMDdivTF4L(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivTF4LFG(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivTF4NL(TMD_P_TF4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivTNF4(TMD_P_TNF4 *primtop,SVECTOR *vertop,
		POLY_FT4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);

u_long *GsTMDdivG3L(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivG3LFG(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivG3NL(TMD_P_G3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivNG3(TMD_P_NG3 *primtop,SVECTOR *vertop,
		POLY_G3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);

u_long *GsTMDdivG4L(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivG4LFG(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivG4NL(TMD_P_G4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivNG4(TMD_P_NG4 *primtop,SVECTOR *vertop,
		POLY_G4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);

u_long *GsTMDdivTG3L(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivTG3LFG(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivTG3NL(TMD_P_TG3 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);
u_long *GsTMDdivTNG3(TMD_P_TNG3 *primtop,SVECTOR *vertop,
		POLY_GT3 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON3 *divp);

u_long *GsTMDdivTG4L(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivTG4LFG(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivTG4NL(TMD_P_TG4 *primtop,SVECTOR *vertop,SVECTOR *nortop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);
u_long *GsTMDdivTNG4(TMD_P_TNG4 *primtop,SVECTOR *vertop,
		POLY_GT4 *s,u_long n,u_long shift,GsOT *otp,DIVPOLYGON4 *divp);

*/

void RotSMD_F3(long *pa,u_long *ot,int otlen,int id);		
void RotSMD_G3(long *pa,u_long *ot,int otlen,int id);
void RotSMD_FT3(long *pa,u_long *ot,int otlen,int id);
void RotSMD_GT3(long *pa,u_long *ot,int otlen,int id);
void RotSMD_F4(long *pa,u_long *ot,int otlen,int id);
void RotSMD_G4(long *pa,u_long *ot,int otlen,int id);
void RotSMD_FT4(long *pa,u_long *ot,int otlen,int id);
void RotSMD_GT4(long *pa,u_long *ot,int otlen,int id);

void RotSMD_SV_F3(long *pa,long *va,u_long *ot,int otlen,int id);	
void RotSMD_SV_G3(long *pa,long *va,u_long *ot,int otlen,int id);
void RotSMD_SV_FT3(long *pa,long *va,u_long *ot,int otlen,int id);
void RotSMD_SV_GT3(long *pa,long *va,u_long *ot,int otlen,int id);
void RotSMD_SV_F4(long *pa,long *va,u_long *ot,int otlen,int id);
void RotSMD_SV_G4(long *pa,long *va,u_long *ot,int otlen,int id);
void RotSMD_SV_FT4(long *pa,long *va,u_long *ot,int otlen,int id);
void RotSMD_SV_GT4(long *pa,long *va,u_long *ot,int otlen,int id);



void RotRMD_F3(long *pa,u_long *ot,int otlen,int id);		
void RotRMD_G3(long *pa,u_long *ot,int otlen,int id);
void RotRMD_FT3(long *pa,u_long *ot,int otlen,int id);
void RotRMD_GT3(long *pa,u_long *ot,int otlen,int id);
void RotRMD_F4(long *pa,u_long *ot,int otlen,int id);
void RotRMD_G4(long *pa,u_long *ot,int otlen,int id);
void RotRMD_FT4(long *pa,u_long *ot,int otlen,int id);
void RotRMD_GT4(long *pa,u_long *ot,int otlen,int id);

void RotRMD_SV_F3(long *pa,long *va,u_long *ot,int otlen,int id);	
void RotRMD_SV_G3(long *pa,long *va,u_long *ot,int otlen,int id);
void RotRMD_SV_FT3(long *pa,long *va,u_long *ot,int otlen,int id);
void RotRMD_SV_GT3(long *pa,long *va,u_long *ot,int otlen,int id);
void RotRMD_SV_F4(long *pa,long *va,u_long *ot,int otlen,int id);
void RotRMD_SV_G4(long *pa,long *va,u_long *ot,int otlen,int id);
void RotRMD_SV_FT4(long *pa,long *va,u_long *ot,int otlen,int id);
void RotRMD_SV_GT4(long *pa,long *va,u_long *ot,int otlen,int id);

#endif
#endif
