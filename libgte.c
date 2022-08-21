//
// LIBGTE.C for WATCOM C v10.0 by 
//  Salim Siwani and Dave Smith, 
//		     Psygnosis Ltd
//          07/02/1995
//

#include <math.h>
#include <string.h>
#define _SIZE_T
#include <sys\types.h>
#include "libgte.h"

static float tmp;

extern int					WinPitch;
extern short				WinWidth;
extern short				WinHeight;
extern short				WinHeightX2;

// Global Variables for GTE...

MATRIX		LCM;			// local colour matrix
SVECTOR		LLV[3];			// local lighting vector
MATRIX		LLM;			// local lighting matrix

GTECVECTOR	BK;				// background colour
GTECVECTOR	FC;				// far colour

long			FFAR;		// fog far
long			FNEAR;		// fog near

long			xOffset;	// geometry offset
long			yOffset;
long			screenZ;	// geometry screen Z
long			SCR2;		// geometry screen Z / 2

MATRIX 		RTM;			// 1 current rotation matrix
GTECVECTOR 	grgb[3];		// 3 current colour values
long			gsxy[3];	// 3 current screen coordinates
long			gsz[4];		// 4 current screen z corrdinates

MATRIX		stack[20];		// 20 rotation matricies for stack
long			stackPtr;

long		MaxZValue;
long		HalfZValue;

#define	TableSize	4096
double	SineTable[TableSize];





long slow_rsin( long a )
{
	return (long)( sin( ((double)a / 4096.0) * 6.28318530718 ) * 4096.0);
}



#ifndef USE_ATI

long slow_rcos( long a )
{
	return (long)( cos( ((double)a / 4096.0) * 6.28318530718 ) * 4096.0);
}

#endif //USE_ATI

long ratan2(long y, long x)
{
	return((long)((atan2((double) y, (double) x)) * 652));
}


void CreateSineTable(void)
{
	long	angle;


	for(angle = 0; angle < TableSize; angle++)
		SineTable[angle] = slow_rsin(angle);
}	



long rsin(long a)
{
	long	index, result;


	index = a;

	if(abs(index) > TableSize)
		index %= TableSize;

	if(index < 0)
		index = -index;

	if(a < 0.0)
		result = -SineTable[index];
	else
		result = SineTable[index];

	return(result);
}	



long rcos(long a)
{
	long	index, result;


	index = a + 1024;

	if(abs(index) > TableSize)
		index %= TableSize;

	if(index < 0)
	{
		index = -index;
		result = -SineTable[index];
	}
	else
		result = SineTable[index];

	return(result);
}	


#if 0  //JJS
long lnFOG( long a )
{
	double u;


	u = (double)(a - FNEAR) / (double)(FFAR - FNEAR) * 256.0;

	if( u < 1 )
		return 0;

	if( u > 256 )
		return ONE;

	return (long)( log10( u ) * 1700.827 );
}
#endif


#ifndef USE_ATI

VECTOR *ApplyMatrixCC( MATRIX * m, SVECTOR * v0, VECTOR * v1 )
{
	long xt, yt, zt;
	long stage1, stage2, stage3;


	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	stage1 = (xt * (long)(m->m[0][0])) >> SHIFT;
	stage2 = (yt * (long)(m->m[0][1])) >> SHIFT;
	stage3 = (zt * (long)(m->m[0][2])) >> SHIFT;
	v1->vx = stage1+stage2+stage3+m->t[0];

	stage1 = (xt * (long)(m->m[1][0])) >> SHIFT;
	stage2 = (yt * (long)(m->m[1][1])) >> SHIFT;
	stage3 = (zt * (long)(m->m[1][2])) >> SHIFT;
	v1->vy = stage1+stage2+stage3+m->t[1];

	stage1 = (xt * (long)(m->m[2][0])) >> SHIFT;
	stage2 = (yt * (long)(m->m[2][1])) >> SHIFT;
	stage3 = (zt * (long)(m->m[2][2])) >> SHIFT;
	v1->vz = stage1+stage2+stage3+m->t[2];

	return v1;
}
#endif //USE_ATI



#ifndef USE_ATI


long AverageZ3CC( long sz0, long sz1, long sz2 )
{
	long a, b;

	a = (sz0+sz1+sz2) >> 2;
	b = a;					  //  1/4
	b >>= 2;
	a += b;					  // +1/16
	b >>= 2;
	a += b;					  // +1/64
	b >>= 2;
	a += b;					  // +1/256
	b >>= 2;
	a += b;					  // +1/1024
	return (a>>2);
}

#endif //USE_ATI


#ifndef USE_ATI

long AverageZ4CC( long sz0, long sz1, long sz2, long sz3 )
{
	return (sz0 + sz1 + sz2 + sz3)/16;
}
#endif //USE_ATI


#ifndef USE_ATI


void ColorCol( VECTOR * v0, CVECTOR * v1, CVECTOR * v2 )
{
	long xt, yt, zt;

	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT ) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT ) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT ) + ( BK.b );

	v2->r = grgb[2].r = MULT(v1->r, grgb[2].r);
	v2->g = grgb[2].g = MULT(v1->g, grgb[2].g);
	v2->b = grgb[2].b = MULT(v1->b, grgb[2].b);
}
#endif //USE_ATI
#ifndef USE_ATI




void ColorDpq( VECTOR * v0, CVECTOR * v1, long p, CVECTOR * v2 )
{
	long xt, yt, zt;
	long p2 = 4096 - p;

	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT ) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT ) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT ) + ( BK.b );

	v2->r = ( (p2 * v1->r * grgb[2].r) + (p * FC.r) ) >> SHIFT;
	v2->g = ( (p2 * v1->g * grgb[2].g) + (p * FC.g) ) >> SHIFT;
	v2->b = ( (p2 * v1->b * grgb[2].b) + (p * FC.b) ) >> SHIFT;
}
#endif //USE_ATI

#ifndef USE_ATI


void ColorMatCol( SVECTOR * v0, CVECTOR * v1, CVECTOR * v2, long matc )
{
	long xt, yt, zt;

	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	LLV[2].vx = pow(LLV[2].vx, (pow(2,matc)));
	LLV[2].vy = pow(LLV[2].vy, (pow(2,matc)));
	LLV[2].vz = pow(LLV[2].vz, (pow(2,matc)));

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	xt = (long)(grgb[2].r);
	yt = (long)(grgb[2].g);
	zt = (long)(grgb[2].b);

	v2->r = ( (xt * (long)(v1->r)) + (yt * (long)(v1->r)) + (zt * (long)(v1->r)) ) >> SHIFT;
	v2->g = ( (xt * (long)(v1->g)) + (yt * (long)(v1->g)) + (zt * (long)(v1->g)) ) >> SHIFT;
	v2->b = ( (xt * (long)(v1->b)) + (yt * (long)(v1->b)) + (zt * (long)(v1->b)) ) >> SHIFT;
}
#endif //USE_ATI


#ifndef USE_ATI


void ColorMatDpq( SVECTOR * v0, CVECTOR * v1, long p, CVECTOR * v2, long matc )
{
	long xt, yt, zt;
	long p2 = 4096 - p;

	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	LLV[2].vx = pow(LLV[2].vx, (pow(2,matc)));
	LLV[2].vy = pow(LLV[2].vy, (pow(2,matc)));
	LLV[2].vz = pow(LLV[2].vz, (pow(2,matc)));

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	v2->r = grgb[2].r = ( (p2 * v1->r * grgb[2].r) + (p * FC.r) ) >> SHIFT2;
	v2->g = grgb[2].g = ( (p2 * v1->g * grgb[2].g) + (p * FC.g) ) >> SHIFT2;
	v2->b = grgb[2].b = ( (p2 * v1->b * grgb[2].b) + (p * FC.b) ) >> SHIFT2;
}
#endif //USE_ATI



#ifndef USE_ATI

long	csqrt(long a)
{
unsigned long b;
	b = (unsigned long ) a;
	return (long)(sqrt((double)b) * 64);
}
#endif //USE_ATI


#ifndef USE_ATI


void DpqColor( CVECTOR * v0, long p, CVECTOR * v1 )
{
	long p2 = 4096 - p;


	v1->r = ( (p2 * v0->r) + (p * FC.r) ) >> SHIFT;
	v1->g = ( (p2 * v0->g) + (p * FC.g) ) >> SHIFT;
	v1->b = ( (p2 * v0->b) + (p * FC.b) ) >> SHIFT;
}
#endif //USE_ATI



#ifndef USE_ATI

void DpqColor3( CVECTOR * v0, CVECTOR * v1, CVECTOR * v2, long p, 
					 CVECTOR * v3, CVECTOR * v4, CVECTOR * v5 )
{
	long p2 = 4096 - p;


	v3->r = grgb[2].r = ( (p2 * v0->r) + (p * FC.r) ) >> SHIFT;
	v3->g = grgb[2].g = ( (p2 * v0->g) + (p * FC.g) ) >> SHIFT;
	v3->b = grgb[2].b = ( (p2 * v0->b) + (p * FC.b) ) >> SHIFT;
	
	v4->r = grgb[2].r = ( (p2 * v1->r) + (p * FC.r) ) >> SHIFT;
	v4->g = grgb[2].g = ( (p2 * v1->g) + (p * FC.g) ) >> SHIFT;
	v4->b = grgb[2].b = ( (p2 * v1->b) + (p * FC.b) ) >> SHIFT;
	
	v5->r = grgb[2].r = ( (p2 * v2->r) + (p * FC.r) ) >> SHIFT;
	v5->g = grgb[2].g = ( (p2 * v2->g) + (p * FC.g) ) >> SHIFT;
	v5->b = grgb[2].b = ( (p2 * v2->b) + (p * FC.b) ) >> SHIFT;
}
#endif //USE_ATI



#ifndef USE_ATI

void DpqColorLight( VECTOR * v0, CVECTOR * v1, long p, CVECTOR * v2 )
{
	long p2 = 4096 - p;


	v2->r = grgb[2].r = ( (p2 * v1->r * v1->r) + (p * FC.r) ) >> SHIFT2;
	v2->g = grgb[2].g = ( (p2 * v1->g * v1->g) + (p * FC.g) ) >> SHIFT2;
	v2->b = grgb[2].b = ( (p2 * v1->b * v1->b) + (p * FC.b) ) >> SHIFT2;
}
#endif //USE_ATI



#ifndef USE_ATI

void FlipRotMatrixX()
{
	MATRIX	RMat;


	ReadRotMatrix(&RMat);

	RMat.m[0][0] = -RMat.m[0][0];
	RMat.m[0][1] = -RMat.m[0][1];
	RMat.m[0][2] = -RMat.m[0][2];

	SetRotMatrix(&RMat);
}

#endif //USE_ATI

#ifndef USE_ATI


void FlipTRX()
{
	MATRIX	RMat;


	ReadRotMatrix(&RMat);
	RMat.t[0] = -RMat.t[0];
	SetTransMatrix(&RMat);
}
#endif //USE_ATI



#ifndef USE_ATI

void gteMIMefunc( SVECTOR * otp, SVECTOR * dfp, long n, long p )
{
	int i;


	for(i=0; i<n; i++)
	{
		(otp+i)->vx += ( (int)( (dfp+i)->vx ) * p ) >> SHIFT;
		(otp+i)->vy += ( (int)( (dfp+i)->vy ) * p ) >> SHIFT;
		(otp+i)->vz += ( (int)( (dfp+i)->vz ) * p ) >> SHIFT;
	}
}
#endif //USE_ATI



void InitGeom( void )
{
	// initalise stack pointer for rotation matricies
	stackPtr = 0;

	RTM.m[0][0] =  16652;
	RTM.m[0][1] = -31805;
	RTM.m[0][2] = -30657;
	RTM.m[1][0] =  17100;
	RTM.m[1][1] =  26345;
	RTM.m[1][2] =  25272;
	RTM.m[2][0] =   6882;
	RTM.m[2][1] = -11327;
	RTM.m[2][2] =  20672;

	RTM.t[0] = 0x40000401;
	RTM.t[1] = 0x40000401;
	RTM.t[2] = 0x40000401;

	LCM.m[0][0] = ONE;
	LCM.m[0][1] =   0;
	LCM.m[0][2] =   0;
	LCM.m[1][0] =   0;
	LCM.m[1][1] = ONE;
	LCM.m[1][2] =   0;
	LCM.m[2][0] =   0;
	LCM.m[2][1] =   0;
	LCM.m[2][2] = ONE;

	LLM.m[0][0] = -28320;
	LLM.m[0][1] = -29804;
	LLM.m[0][2] =   9801;
	LLM.m[1][0] =    782;
	LLM.m[1][1] = -23358;
	LLM.m[1][2] = -29799;
	LLM.m[2][0] =  21952;
	LLM.m[2][1] = -22357;
	LLM.m[2][2] =  31393;

	xOffset = 0;
	yOffset = 0;
	screenZ = 0x3e8;
	SCR2 = screenZ >> 1;

	BK.r = 0;
	BK.g = 0;
	BK.b = 255;

	FC.r = 0;
	FC.g = 255;
	FC.b = 0;

	MaxZValue = 65535;
	HalfZValue = MaxZValue >> 1;
}



#ifndef USE_ATI

void Intpl( VECTOR * v0, long p, CVECTOR * v1 )
{
	long p2 = 4096 - p;

	v1->r = grgb[2].r = ( (p2 * v0->vx) + (p * FC.r) ) >> SHIFT;
	v1->g = grgb[2].g = ( (p2 * v0->vy) + (p * FC.g) ) >> SHIFT;
	v1->b = grgb[2].b = ( (p2 * v0->vz) + (p * FC.b) ) >> SHIFT;
}

#endif //USE_ATI


#ifndef USE_ATI

void InterpolShort(SVECTOR *vi0, SVECTOR *vi1, long p0, SVECTOR *vt1)
{
	float p1 = (4096 - (float)p0) / 4096;


	vt1->vx = ((vi1->vx - vi0->vx) * p1) + vi0->vx;
	vt1->vy = ((vi1->vy - vi0->vy) * p1) + vi0->vy;
	vt1->vz = ((vi1->vz - vi0->vz) * p1) + vi0->vz;
}
#endif //USE_ATI



#ifndef USE_ATI

void InvSquareRoot(long a, long *b, long *c)
{
	double 	mantissa;
	int		temp;


	mantissa = frexp(ONE/(sqrt(a)), &temp);
	*b = (long)(mantissa);
	*c = (long)(temp);
}
#endif //USE_ATI



#ifndef USE_ATI

void LightColor( VECTOR * v0, VECTOR * v1 )
{
	long xt, yt, zt;


	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	v1->vx = ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT;
	v1->vy = ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT;
	v1->vz = ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT;

	if( v1->vx < 0 )	v1->vx = 0;
	if( v1->vy < 0 )	v1->vy = 0;
	if( v1->vz < 0 )	v1->vz = 0;
}
#endif //USE_ATI



#ifndef USE_ATI

void LoadAverage0( VECTOR * v0, VECTOR * v1, long p0, long p1, VECTOR * v2 )
{
	v2->vx = (v0->vx * p0) + (v1->vx * p1);
	v2->vy = (v0->vy * p0) + (v1->vy * p1);
	v2->vz = (v0->vz * p0) + (v1->vz * p1);
}
#endif //USE_ATI



#ifndef USE_ATI

void LoadAverage12( VECTOR * v0, VECTOR * v1, long p0, long p1, VECTOR * v2 )
{
	v2->vx = ( ( (v0->vx) * p0 ) + ( (v1->vx) * p1) ) >> SHIFT;
	v2->vy = ( ( (v0->vy) * p0 ) + ( (v1->vy) * p1) ) >> SHIFT;
	v2->vz = ( ( (v0->vz) * p0 ) + ( (v1->vz) * p1) ) >> SHIFT;
}

#endif //USE_ATI


#ifndef USE_ATI

void LoadAverageShort0( SVECTOR * v0, SVECTOR * v1, long p0, long p1, SVECTOR * v2 )
{
	v2->vx = (v0->vx * p0) + (v1->vx * p1);
	v2->vy = (v0->vy * p0) + (v1->vy * p1);
	v2->vz = (v0->vz * p0) + (v1->vz * p1);
}
#endif //USE_ATI


#ifndef USE_ATI

void LoadAverageShort12( SVECTOR * v0, SVECTOR * v1, long p0, long p1, SVECTOR * v2 )

{
	v2->vx = (short)( ( ( (long)(v0->vx) * p0 ) + ( (long)(v1->vx) * p1) ) >> SHIFT );
	v2->vy = (short)( ( ( (long)(v0->vy) * p0 ) + ( (long)(v1->vy) * p1) ) >> SHIFT );
	v2->vz = (short)( ( ( (long)(v0->vz) * p0 ) + ( (long)(v1->vz) * p1) ) >> SHIFT );
}
#endif //USE_ATI



#ifndef USE_ATI

void LocalLight( SVECTOR * v0, VECTOR * v1 )
{
	long xt, yt, zt;


	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	v1->vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	v1->vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	v1->vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	if( v1->vx < 0 )	v1->vx = 0;
	if( v1->vy < 0 )	v1->vy = 0;
	if( v1->vz < 0 )	v1->vz = 0;
}
#endif //USE_ATI



#ifndef USE_ATI

long Lzc( long data )
{
	u_long	tmp;
	long 		cnt = 0;		// set up bit counter
	

	if( data < 0 )
		tmp = (u_long)(-data);
	else
		tmp = (u_long)(+data);

	while( tmp <= (u_long)0x7FFFFFFF )
	{
		tmp <<= 1;
		cnt++;
	}

	return cnt;
}
#endif //USE_ATI



#ifndef USE_ATI

MATRIX * MulMatrix( MATRIX * m0, MATRIX * m1 )
{
	RTM.m[0][0] = (((long)m0->m[0][0] * (long)m1->m[0][0]) + ((long)m0->m[0][1] * (long)m1->m[1][0]) + ((long)m0->m[0][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[0][1] = (((long)m0->m[0][0] * (long)m1->m[0][1]) + ((long)m0->m[0][1] * (long)m1->m[1][1]) + ((long)m0->m[0][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[0][2] = (((long)m0->m[0][0] * (long)m1->m[0][2]) + ((long)m0->m[0][1] * (long)m1->m[1][2]) + ((long)m0->m[0][2] * (long)m1->m[2][2])) >> SHIFT;

	RTM.m[1][0] = (((long)m0->m[1][0] * (long)m1->m[0][0]) + ((long)m0->m[1][1] * (long)m1->m[1][0]) + ((long)m0->m[1][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[1][1] = (((long)m0->m[1][0] * (long)m1->m[0][1]) + ((long)m0->m[1][1] * (long)m1->m[1][1]) + ((long)m0->m[1][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[1][2] = (((long)m0->m[1][0] * (long)m1->m[0][2]) + ((long)m0->m[1][1] * (long)m1->m[1][2]) + ((long)m0->m[1][2] * (long)m1->m[2][2])) >> SHIFT;

	RTM.m[2][0] = (((long)m0->m[2][0] * (long)m1->m[0][0]) + ((long)m0->m[2][1] * (long)m1->m[1][0]) + ((long)m0->m[2][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[2][1] = (((long)m0->m[2][0] * (long)m1->m[0][1]) + ((long)m0->m[2][1] * (long)m1->m[1][1]) + ((long)m0->m[2][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[2][2] = (((long)m0->m[2][0] * (long)m1->m[0][2]) + ((long)m0->m[2][1] * (long)m1->m[1][2]) + ((long)m0->m[2][2] * (long)m1->m[2][2])) >> SHIFT;

#ifdef WIPEOUTPC
	RTM.t[0] = m0->t[0];
	RTM.t[1] = m0->t[1];
	RTM.t[2] = m0->t[2];
#else
	RTM.t[0] = 0;
	RTM.t[1] = 0;
	RTM.t[2] = 0;
#endif

	(*m0) = RTM;

	return m0;
}
#endif //USE_ATI



MATRIX * MulMatrix0( MATRIX * m0, MATRIX * m1, MATRIX * m2 )
{
	RTM.m[0][0] = (((long)m0->m[0][0] * (long)m1->m[0][0]) + ((long)m0->m[0][1] * (long)m1->m[1][0]) + ((long)m0->m[0][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[0][1] = (((long)m0->m[0][0] * (long)m1->m[0][1]) + ((long)m0->m[0][1] * (long)m1->m[1][1]) + ((long)m0->m[0][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[0][2] = (((long)m0->m[0][0] * (long)m1->m[0][2]) + ((long)m0->m[0][1] * (long)m1->m[1][2]) + ((long)m0->m[0][2] * (long)m1->m[2][2])) >> SHIFT;

	RTM.m[1][0] = (((long)m0->m[1][0] * (long)m1->m[0][0]) + ((long)m0->m[1][1] * (long)m1->m[1][0]) + ((long)m0->m[1][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[1][1] = (((long)m0->m[1][0] * (long)m1->m[0][1]) + ((long)m0->m[1][1] * (long)m1->m[1][1]) + ((long)m0->m[1][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[1][2] = (((long)m0->m[1][0] * (long)m1->m[0][2]) + ((long)m0->m[1][1] * (long)m1->m[1][2]) + ((long)m0->m[1][2] * (long)m1->m[2][2])) >> SHIFT;

	RTM.m[2][0] = (((long)m0->m[2][0] * (long)m1->m[0][0]) + ((long)m0->m[2][1] * (long)m1->m[1][0]) + ((long)m0->m[2][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[2][1] = (((long)m0->m[2][0] * (long)m1->m[0][1]) + ((long)m0->m[2][1] * (long)m1->m[1][1]) + ((long)m0->m[2][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[2][2] = (((long)m0->m[2][0] * (long)m1->m[0][2]) + ((long)m0->m[2][1] * (long)m1->m[1][2]) + ((long)m0->m[2][2] * (long)m1->m[2][2])) >> SHIFT;

#ifdef WIPEOUTPC
	RTM.t[0] = m2->t[0];
	RTM.t[1] = m2->t[1];
	RTM.t[2] = m2->t[2];
#else
	RTM.t[0] = 0;
	RTM.t[1] = 0;
	RTM.t[2] = 0;
#endif

	(*m2) = RTM;

	return m2;
}


#ifndef USE_ATI


MATRIX *MulMatrix2( MATRIX * m0, MATRIX * m1 )
{
/*
	long stage1, stage2, stage3;

	stage1 = ((long)(m0->m[0][0]) * (long)(m1->m[0][0])) >> SHIFT;
	stage2 = ((long)(m0->m[0][1]) * (long)(m1->m[1][0])) >> SHIFT;
	stage3 = ((long)(m0->m[0][2]) * (long)(m1->m[2][0])) >> SHIFT;
	RTM.m[0][0] = stage1+stage2+stage3;

	stage1 = ((long)(m0->m[0][0]) * (long)(m1->m[0][1])) >> SHIFT;
	stage2 = ((long)(m0->m[0][1]) * (long)(m1->m[1][1])) >> SHIFT;
	stage3 = ((long)(m0->m[0][2]) * (long)(m1->m[2][1])) >> SHIFT;
	RTM.m[0][1] = stage1+stage2+stage3;

	stage1 = ((long)(m0->m[0][0]) * (long)(m1->m[0][2])) >> SHIFT;
	stage2 = ((long)(m0->m[0][1]) * (long)(m1->m[1][2])) >> SHIFT;
	stage3 = ((long)(m0->m[0][2]) * (long)(m1->m[2][2])) >> SHIFT;
	RTM.m[0][2] = stage1+stage2+stage3;


	stage1 = ((long)(m0->m[1][0]) * (long)(m1->m[0][0])) >> SHIFT;
	stage2 = ((long)(m0->m[1][1]) * (long)(m1->m[1][0])) >> SHIFT;
	stage3 = ((long)(m0->m[1][2]) * (long)(m1->m[2][0])) >> SHIFT;
	RTM.m[1][0] = stage1+stage2+stage3;

	stage1 = ((long)(m0->m[1][0]) * (long)(m1->m[0][1])) >> SHIFT;
	stage2 = ((long)(m0->m[1][1]) * (long)(m1->m[1][1])) >> SHIFT;
	stage3 = ((long)(m0->m[1][2]) * (long)(m1->m[2][1])) >> SHIFT;
	RTM.m[1][1] = stage1+stage2+stage3;

	stage1 = ((long)(m0->m[1][0]) * (long)(m1->m[0][2])) >> SHIFT;
	stage2 = ((long)(m0->m[1][1]) * (long)(m1->m[1][2])) >> SHIFT;
	stage3 = ((long)(m0->m[1][2]) * (long)(m1->m[2][2])) >> SHIFT;
	RTM.m[1][2] = stage1+stage2+stage3;


	stage1 = ((long)(m0->m[2][0]) * (long)(m1->m[0][0])) >> SHIFT;
	stage2 = ((long)(m0->m[2][1]) * (long)(m1->m[1][0])) >> SHIFT;
	stage3 = ((long)(m0->m[2][2]) * (long)(m1->m[2][0])) >> SHIFT;
	RTM.m[2][0] = stage1+stage2+stage3;

	stage1 = ((long)(m0->m[2][0]) * (long)(m1->m[0][1])) >> SHIFT;
	stage2 = ((long)(m0->m[2][1]) * (long)(m1->m[1][1])) >> SHIFT;
	stage3 = ((long)(m0->m[2][2]) * (long)(m1->m[2][1])) >> SHIFT;
	RTM.m[2][1] = stage1+stage2+stage3;

	stage1 = ((long)(m0->m[2][0]) * (long)(m1->m[0][2])) >> SHIFT;
	stage2 = ((long)(m0->m[2][1]) * (long)(m1->m[1][2])) >> SHIFT;
	stage3 = ((long)(m0->m[2][2]) * (long)(m1->m[2][2])) >> SHIFT;
	RTM.m[2][2] = stage1+stage2+stage3;
*/

	RTM.m[0][0] = (((long)m0->m[0][0] * (long)m1->m[0][0]) + ((long)m0->m[0][1] * (long)m1->m[1][0]) + ((long)m0->m[0][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[0][1] = (((long)m0->m[0][0] * (long)m1->m[0][1]) + ((long)m0->m[0][1] * (long)m1->m[1][1]) + ((long)m0->m[0][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[0][2] = (((long)m0->m[0][0] * (long)m1->m[0][2]) + ((long)m0->m[0][1] * (long)m1->m[1][2]) + ((long)m0->m[0][2] * (long)m1->m[2][2])) >> SHIFT;

	RTM.m[1][0] = (((long)m0->m[1][0] * (long)m1->m[0][0]) + ((long)m0->m[1][1] * (long)m1->m[1][0]) + ((long)m0->m[1][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[1][1] = (((long)m0->m[1][0] * (long)m1->m[0][1]) + ((long)m0->m[1][1] * (long)m1->m[1][1]) + ((long)m0->m[1][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[1][2] = (((long)m0->m[1][0] * (long)m1->m[0][2]) + ((long)m0->m[1][1] * (long)m1->m[1][2]) + ((long)m0->m[1][2] * (long)m1->m[2][2])) >> SHIFT;

	RTM.m[2][0] = (((long)m0->m[2][0] * (long)m1->m[0][0]) + ((long)m0->m[2][1] * (long)m1->m[1][0]) + ((long)m0->m[2][2] * (long)m1->m[2][0])) >> SHIFT;
	RTM.m[2][1] = (((long)m0->m[2][0] * (long)m1->m[0][1]) + ((long)m0->m[2][1] * (long)m1->m[1][1]) + ((long)m0->m[2][2] * (long)m1->m[2][1])) >> SHIFT;
	RTM.m[2][2] = (((long)m0->m[2][0] * (long)m1->m[0][2]) + ((long)m0->m[2][1] * (long)m1->m[1][2]) + ((long)m0->m[2][2] * (long)m1->m[2][2])) >> SHIFT;

#ifdef WIPEOUTPC
	RTM.t[0] = m1->t[0];
	RTM.t[1] = m1->t[1];
	RTM.t[2] = m1->t[2];
#else
	RTM.t[0] = 0;
	RTM.t[1] = 0;
	RTM.t[2] = 0;
#endif

	(*m1) = RTM;

	return m1;
}
#endif //USE_ATI


short OnScreen3(long xy0, long xy1, long xy2)
{
	DVECTOR *vxy0, *vxy1, *vxy2;

	//return 1;

	vxy0 = (DVECTOR *)&xy0;
	vxy1 = (DVECTOR *)&xy1;
	vxy2 = (DVECTOR *)&xy2;

	if (vxy0->vx > 320 || vxy0->vx < 0) return 0;
	if (vxy1->vx > 320 || vxy1->vx < 0) return 0;
	if (vxy2->vx > 320 || vxy2->vx < 0) return 0;

	if (vxy0->vy > 240 || vxy0->vy < 0) return 0;
	if (vxy1->vy > 240 || vxy1->vy < 0) return 0;
	if (vxy2->vy > 240 || vxy2->vy < 0) return 0;

	return 1;
}

short OnScreen4(long xy0, long xy1, long xy2, long xy3)
{
	DVECTOR *vxy0, *vxy1, *vxy2, *vxy3;

	//return 1;

	vxy0 = (DVECTOR *)&xy0;
	vxy1 = (DVECTOR *)&xy1;
	vxy2 = (DVECTOR *)&xy2;
	vxy3 = (DVECTOR *)&xy3;

	if (vxy0->vx > 320 || vxy0->vx < 0) return 0;
	if (vxy1->vx > 320 || vxy1->vx < 0) return 0;
	if (vxy2->vx > 320 || vxy2->vx < 0) return 0;
	if (vxy3->vx > 320 || vxy3->vx < 0) return 0;

	if (vxy0->vy > 240 || vxy0->vy < 0) return 0;
	if (vxy1->vy > 240 || vxy1->vy < 0) return 0;
	if (vxy2->vy > 240 || vxy2->vy < 0) return 0;
	if (vxy3->vy > 240 || vxy3->vy < 0) return 0;

	return 1;
}

long NormalClip( long sxy0, long sxy1, long sxy2 )
{
	long		sx0, sy0, 
				dx1, dy1, 
				dx2, dy2;
	DVECTOR 	*xy0, *xy1, *xy2;


	xy0 = (DVECTOR *)&sxy0;			// AndyY mod for signed x
	xy1 = (DVECTOR *)&sxy1;
	xy2 = (DVECTOR *)&sxy2;

	dx1 = xy1->vx - xy0->vx;
	dy1 = xy1->vy - xy0->vy;

	dx2 = xy2->vx - xy0->vx;
	dy2 = xy2->vy - xy0->vy;

	return ((dx1*dy2) - (dy1*dx2));
}



#ifndef USE_ATI

void NormalColor (SVECTOR * v0, CVECTOR * v1)
{
/*	
	long xt, yt, zt;

	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	v1->r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	v1->g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	v1->b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );
*/
}
#endif //USE_ATI



#ifndef USE_ATI

void NormalColor3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2,
						 CVECTOR * v3, CVECTOR * v4, CVECTOR * v5 )
{
	long xt, yt, zt;

	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	LLV[0].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[0].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[0].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(v1->vx);
	yt = (long)(v1->vy);
	zt = (long)(v1->vz);

	LLV[1].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[1].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[1].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(v2->vx);
	yt = (long)(v2->vy);
	zt = (long)(v2->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(LLV[0].vx);
	yt = (long)(LLV[0].vy);
	zt = (long)(LLV[0].vz);

	v3->r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	v3->g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	v3->b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	xt = (long)(LLV[1].vx);
	yt = (long)(LLV[1].vy);
	zt = (long)(LLV[1].vz);

	v4->r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	v4->g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	v4->b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	v5->r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	v5->g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	v5->b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );
}
#endif //USE_ATI



void NormalColorCol( SVECTOR * v0, CVECTOR * v1, CVECTOR * v2 )
{
	long xt, yt, zt;


	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	v2->r = grgb[2].r = MULT(v1->r, grgb[2].r);
	v2->g = grgb[2].g = MULT(v1->g, grgb[2].g);
 	v2->b = grgb[2].b = MULT(v1->b, grgb[2].b);
}



void NormalColorCol3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2, 
							 CVECTOR * v3, 
							 CVECTOR * v4, CVECTOR * v5, CVECTOR * v6 )
{
	long xt, yt, zt;


	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	LLV[0].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[0].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[0].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(v1->vx);
	yt = (long)(v1->vy);
	zt = (long)(v1->vz);

	LLV[1].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[1].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[1].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(v2->vx);
	yt = (long)(v2->vy);
	zt = (long)(v2->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(LLV[0].vx);
	yt = (long)(LLV[0].vy);
	zt = (long)(LLV[0].vz);

	grgb[0].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[0].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[0].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	xt = (long)(LLV[1].vx);
	yt = (long)(LLV[1].vy);
	zt = (long)(LLV[1].vz);

	grgb[1].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[1].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[1].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	v4->r = grgb[0].r = MULT(v3->r, grgb[0].r);
	v4->g = grgb[0].g = MULT(v3->g, grgb[0].g);
	v4->b = grgb[0].b = MULT(v3->b, grgb[0].b);
	
	v5->r = grgb[1].r = MULT(v3->r, grgb[1].r);
	v5->g = grgb[1].g = MULT(v3->g, grgb[1].g);
	v5->b = grgb[1].b = MULT(v3->b, grgb[1].b);
	
	v6->r = grgb[2].r = MULT(v3->r, grgb[2].r);
	v6->g = grgb[2].g = MULT(v3->g, grgb[2].g);
	v6->b = grgb[2].b = MULT(v3->b, grgb[2].b);
}



#ifndef USE_ATI

void NormalColorDpq( SVECTOR * v0, CVECTOR * v1, long p, CVECTOR * v2 )
{
	long xt, yt, zt;
	long p2 = 4096 - p;


	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	v2->r = ( (p2 * v1->r * grgb[2].r) + (p * FC.r) ) >> SHIFT;
	v2->g = ( (p2 * v1->g * grgb[2].g) + (p * FC.g) ) >> SHIFT;
	v2->b = ( (p2 * v1->b * grgb[2].b) + (p * FC.b) ) >> SHIFT;
}
#endif //USE_ATI




void NormalColorDpq3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2, 
							 CVECTOR * v3, long p, 
							 CVECTOR * v4, CVECTOR * v5, CVECTOR * v6 )
{
	long xt, yt, zt;
	long p2 = 4096 - p;


	xt = (long)(v0->vx);
	yt = (long)(v0->vy);
	zt = (long)(v0->vz);

	LLV[0].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[0].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[0].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(v1->vx);
	yt = (long)(v1->vy);
	zt = (long)(v1->vz);

	LLV[1].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[1].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[1].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(v2->vx);
	yt = (long)(v2->vy);
	zt = (long)(v2->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(LLV[0].vx);
	yt = (long)(LLV[0].vy);
	zt = (long)(LLV[0].vz);

	grgb[0].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[0].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[0].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	xt = (long)(LLV[1].vx);
	yt = (long)(LLV[1].vy);
	zt = (long)(LLV[1].vz);

	grgb[1].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[1].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[1].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	v4->r = grgb[0].r = ( (p2 * v3->r * grgb[0].r) + (p * FC.r) ) >> SHIFT;
	v4->g = grgb[0].g = ( (p2 * v3->g * grgb[0].g) + (p * FC.g) ) >> SHIFT;
	v4->b = grgb[0].b = ( (p2 * v3->b * grgb[0].b) + (p * FC.b) ) >> SHIFT;

	v5->r = grgb[1].r = ( (p2 * v3->r * grgb[1].r) + (p * FC.r) ) >> SHIFT;
	v5->g = grgb[1].g = ( (p2 * v3->g * grgb[1].g) + (p * FC.g) ) >> SHIFT;
	v5->b = grgb[1].b = ( (p2 * v3->b * grgb[1].b) + (p * FC.b) ) >> SHIFT;

	v6->r = grgb[2].r = ( (p2 * v3->r * grgb[2].r) + (p * FC.r) ) >> SHIFT;
	v6->g = grgb[2].g = ( (p2 * v3->g * grgb[2].g) + (p * FC.g) ) >> SHIFT;
	v6->b = grgb[2].b = ( (p2 * v3->b * grgb[2].b) + (p * FC.b) ) >> SHIFT;
}



#ifndef USE_ATI

void OuterProduct0( VECTOR * v0, VECTOR * v1, VECTOR * v2 )
{
	v2->vx = ((v0->vy*v1->vz) - (v0->vz*v1->vy));
	v2->vy = ((v0->vz*v1->vx) - (v0->vx*v1->vz));
	v2->vz = ((v0->vx*v1->vy) - (v0->vy*v1->vx));
}

#endif //USE_ATI


#ifndef USE_ATI

void OuterProduct12( VECTOR * v0, VECTOR * v1, VECTOR * v2 )
{
	v2->vx = ((v0->vy*v1->vz) - (v0->vz*v1->vy)) >> SHIFT;
	v2->vy = ((v0->vz*v1->vx) - (v0->vx*v1->vz)) >> SHIFT;
	v2->vz = ((v0->vx*v1->vy) - (v0->vy*v1->vx)) >> SHIFT;
}

#endif //USE_ATI


#ifndef USE_ATI

void PopMatrix( void )
{
	if(stackPtr == 0 )
		printf("Error: Can't pop matrix, stack is empty!\n");
	else
	{
		stackPtr--;
		RTM = stack[stackPtr];
	}
}

#endif //USE_ATI

#ifndef USE_ATI


void PushMatrix( void )
{
	if(stackPtr == 20 )
		printf("Error: Can't push matrix, stack (max 20) is full!\n");
	else
	{
		stack[stackPtr] = RTM;
		stackPtr++;
	}
}
#endif //USE_ATI


#ifndef USE_ATI


void ReadColorMatrix( MATRIX * m )
{
	*m = LCM;
}

#endif //USE_ATI


#ifndef USE_ATI

void ReadLightMatrix( MATRIX * m )
{
	*m = LLM;
}
#endif //USE_ATI



#ifndef USE_ATI

void ReadRGBfifo( CVECTOR * v0, CVECTOR * v1, CVECTOR * v2 )
{
	v0->r = grgb[0].r;
	v0->g = grgb[0].g;
	v0->b = grgb[0].b;

	v1->r = grgb[1].r;
	v1->g = grgb[1].g;
	v1->b = grgb[1].b;
	
	v2->r = grgb[2].r;
	v2->g = grgb[2].g;
	v2->b = grgb[2].b;
}
#endif //USE_ATI



void ReadRotMatrix( MATRIX * m )
{
	*m = RTM;
}



#ifndef USE_ATI

void ReadSXSYfifo( long * sxy0, long * sxy1, long * sxy2 )
{
	*sxy0 = gsxy[0];
	*sxy1 = gsxy[1];
	*sxy2 = gsxy[2];
}
#endif //USE_ATI



#ifndef USE_ATI

void ReadSZfifo3( long * sz0, long * sz1, long * sz2 )
{
	*sz0 = gsz[1];
	*sz1 = gsz[2];
	*sz2 = gsz[3];
}
#endif //USE_ATI



#ifndef USE_ATI

void ReadSZfifo4( long * sz, long * sz0, long * sz1, long * sz2 )
{
	*sz  = gsz[0];
	*sz0 = gsz[1];
	*sz1 = gsz[2];
	*sz2 = gsz[2];
}

#endif //USE_ATI


#ifndef USE_ATI

long RotAverage3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2, 
						long * sxy0, long * sxy1, long * sxy2,
						long * p, long * flag )
{
	long z, ave;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	VECTOR r0, r1, r2;


	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		*flag |= 0x80004000;	xt0 = -1024;	}
	if( (xt0 >  1023) )					{		*flag |= 0x80004000;	xt0 =  1023;	}
	if( (yt0 < -1024) )					{		*flag |= 0x80002000;	yt0 = -1024;	}
	if( (yt0 >  1023) )					{		*flag |= 0x80002000;	yt0 =  1023;	}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x01000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x00800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);

	ave = (r0.vz + r1.vz + r2.vz)/12;

	if( (FNEAR<SCR2) && (z<SCR2 ) )
		*p = lnFOG( SCR2 );
	else
		*p = lnFOG( z );

	return ave;
}
#endif //USE_ATI



#ifndef USE_ATI

long RotAverage4( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2, SVECTOR * v3,
						long * sxy0, long * sxy1, long * sxy2, long * sxy3,
						long * p, long * flag )
{
	long z, ave;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	long xt3, yt3;
	VECTOR r0, r1, r2, r3;


	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );
	ApplyMatrix( &RTM, v3, &r3 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		*flag |= 0x80004000;	xt0 = -1024;	}
	if( (xt0 >  1023) )					{		*flag |= 0x80004000;	xt0 =  1023;	}
	if( (yt0 < -1024) )					{		*flag |= 0x80002000;	yt0 = -1024;	}
	if( (yt0 >  1023) )					{		*flag |= 0x80002000;	yt0 =  1023;	}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x81000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x80800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	z = r3.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r3.vx > 32767 )					{		*flag |= 0x81000000;	r3.vx = 32767;	}
	if( r3.vy > 32767 )					{		*flag |= 0x80800000;	r3.vy = 32767;	}
	if( r3.vx < -32767 )					{		*flag |= 0x81000000;	r3.vx = -32767;	}
	if( r3.vy < -32767 )					{		*flag |= 0x80800000;	r3.vy = -32767;	}
	xt3 = ((screenZ * r3.vx) / z) + xOffset;
	yt3 = ((screenZ * r3.vy) / z) + yOffset;
	if( (xt3 < -1024) )					{		*flag |= 0x80004000;	xt3 = -1024;	}
	if( (xt3 >  1023) )					{		*flag |= 0x80004000;	xt3 =  1023;	}
	if( (yt3 < -1024) )					{		*flag |= 0x80002000;	yt3 = -1024;	}
	if( (yt3 >  1023) )					{		*flag |= 0x80002000;	yt3 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);
	*sxy3 = (yt3 << 16) | (xt3 & 0x0000FFFF);

	ave = (r0.vz + r1.vz + r2.vz + r3.vz) >> 4;

	if( (FNEAR<SCR2) && (z<SCR2 ) )
		*p = lnFOG( SCR2 );
	else
		*p = lnFOG( z );

	return ave;
}
#endif //USE_ATI



long RotAverageNclip3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2,
							  long * sxy0, long * sxy1, long * sxy2,
							  long * p, long * otz, long * flag )
{
	long z, ave;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	VECTOR r0, r1, r2;


	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		*flag |= 0x80004000;	xt0 = -1024;	}
	if( (xt0 >  1023) )					{		*flag |= 0x80004000;	xt0 =  1023;	}
	if( (yt0 < -1024) )					{		*flag |= 0x80002000;	yt0 = -1024;	}
	if( (yt0 >  1023) )					{		*flag |= 0x80002000;	yt0 =  1023;	}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x01000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x00800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);

	*otz = ((r0.vz + r1.vz + r2.vz)/12); // >> 2;

#if 0	//JJS
	if( (FNEAR<SCR2) && (z<SCR2 ) )
		*p = lnFOG( SCR2 );
	else
		*p = lnFOG( z );

#endif
	return (NormalClip(*sxy0,*sxy1,*sxy2));
}



long RotAverageNclip4( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2, SVECTOR * v3,
							  long * sxy0, long * sxy1, long * sxy2, long * sxy3,
							  long * p, long * otz, long * flag )
{
	long z, ave;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	long xt3, yt3;
	VECTOR r0, r1, r2, r3;

	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );
	ApplyMatrix( &RTM, v3, &r3 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		*flag |= 0x80004000;	xt0 = -1024;	}
	if( (xt0 >  1023) )					{		*flag |= 0x80004000;	xt0 =  1023;	}
	if( (yt0 < -1024) )					{		*flag |= 0x80002000;	yt0 = -1024;	}
	if( (yt0 >  1023) )					{		*flag |= 0x80002000;	yt0 =  1023;	}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x01000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x00800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	z = r3.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r3.vx > 32767 )					{		*flag |= 0x81000000;	r3.vx = 32767;	}
	if( r3.vy > 32767 )					{		*flag |= 0x80800000;	r3.vy = 32767;	}
	if( r3.vx < -32767 )					{		*flag |= 0x81000000;	r3.vx = -32767;	}
	if( r3.vy < -32767 )					{		*flag |= 0x80800000;	r3.vy = -32767;	}
	xt3 = ((screenZ * r3.vx) / z) + xOffset;
	yt3 = ((screenZ * r3.vy) / z) + yOffset;
	if( (xt3 < -1024) )					{		*flag |= 0x80004000;	xt3 = -1024;	}
	if( (xt3 >  1023) )					{		*flag |= 0x80004000;	xt3 =  1023;	}
	if( (yt3 < -1024) )					{		*flag |= 0x80002000;	yt3 = -1024;	}
	if( (yt3 >  1023) )					{		*flag |= 0x80002000;	yt3 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);
	*sxy3 = (yt3 << 16) | (xt3 & 0x0000FFFF);

	*otz = ((r0.vz + r1.vz + r2.vz + r3.vz) >> 4); // >> 2;
#if 0 //JJS
	if( (FNEAR<SCR2) && (z<SCR2 ) )
		*p = lnFOG( SCR2 );
	else
		*p = lnFOG( z );
#endif
	return (NormalClip(*sxy0,*sxy1,*sxy2));
}



#ifndef USE_ATI

long RotAverageNclipColorCol3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2,
										 SVECTOR * v3, SVECTOR * v4, SVECTOR * v5,
										 CVECTOR * v6,
										 long * sxy0, long * sxy1, long * sxy2,
										 CVECTOR * v7, CVECTOR * v8, CVECTOR * v9,
										 long * otz, long * flag )
{
	long z, ave;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	VECTOR r0, r1, r2;

	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		*flag |= 0x80004000;	xt0 = -1024;	}
	if( (xt0 >  1023) )					{		*flag |= 0x80004000;	xt0 =  1023;	}
	if( (yt0 < -1024) )					{		*flag |= 0x80002000;	yt0 = -1024;	}
	if( (yt0 >  1023) )					{		*flag |= 0x80002000;	yt0 =  1023;	}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x01000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x00800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);

	*otz = (r0.vz + r1.vz + r2.vz)/12;
	NormalColorCol3(v3,v4,v5,v6,v7,v8,v9);

	return (NormalClip(*sxy0,*sxy1,*sxy2));
}
#endif //USE_ATI


#ifndef USE_ATI


long RotAverageNclipColorDpq3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2,
										 SVECTOR * v3, SVECTOR * v4, SVECTOR * v5,
										 CVECTOR * v6,
										 long * sxy0, long * sxy1, long * sxy2,
										 CVECTOR * v7, CVECTOR * v8, CVECTOR * v9,
										 long * otz, long * flag )
{
	long z, ave;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	VECTOR r0, r1, r2;
	long p;

	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		*flag |= 0x80004000;	xt0 = -1024;	}
	if( (xt0 >  1023) )					{		*flag |= 0x80004000;	xt0 =  1023;	}
	if( (yt0 < -1024) )					{		*flag |= 0x80002000;	yt0 = -1024;	}
	if( (yt0 >  1023) )					{		*flag |= 0x80002000;	yt0 =  1023;	}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x01000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x00800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);

	*otz = (r0.vz + r1.vz + r2.vz)/12;

	if( (FNEAR<SCR2) && (z<SCR2 ) )
		p = lnFOG( SCR2 );
	else
		p = lnFOG( z );
	
	NormalColorDpq3(v3,v4,v5,v6,p,v7,v8,v9);

	return (NormalClip(*sxy0,*sxy1,*sxy2));
}
#endif //USE_ATI


#ifndef USE_ATI


long RotColorDpq( SVECTOR * v0, SVECTOR * v1, CVECTOR * v2, 
						long * sxy, CVECTOR * v3, long * flag )
{
	long xt, yt, zt;
	VECTOR r0, r1, r2;
	long z, p, p2;

	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt = ((screenZ * r0.vx) / z) + xOffset;
	yt = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt < -1024) )					{		*flag |= 0x80004000;	xt = -1024;	}
	if( (xt >  1023) )					{		*flag |= 0x80004000;	xt =  1023;	}
	if( (yt < -1024) )					{		*flag |= 0x80002000;	yt = -1024;	}
	if( (yt >  1023) )					{		*flag |= 0x80002000;	yt =  1023;	}

	*sxy = (yt << 16) | (xt & 0x0000FFFF);

	if( (FNEAR<SCR2) && (z<SCR2 ) )
		p = lnFOG( SCR2 );
	else
		p = lnFOG( z );

	p2 = 4096-p;

	xt = (long)(v1->vx);
	yt = (long)(v1->vy);
	zt = (long)(v1->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	v3->r = grgb[2].r = ( (p2 * v2->r * grgb[2].r) + (p * FC.r) ) >> SHIFT2;
	v3->g = grgb[2].g = ( (p2 * v2->g * grgb[2].g) + (p * FC.g) ) >> SHIFT2;
	v3->b = grgb[2].b = ( (p2 * v2->b * grgb[2].b) + (p * FC.b) ) >> SHIFT2;

	return (r0.vz >> 2);
}
#endif //USE_ATI



#ifndef USE_ATI

long RotColorDpq3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2,
						 SVECTOR * v3, SVECTOR * v4, SVECTOR * v5,
						 CVECTOR * v6,
						 long * sxy0, long * sxy1, long * sxy2,
						 CVECTOR * v7, CVECTOR * v8, CVECTOR * v9,
						 long * flag )
{
	long p, z;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	VECTOR r0, r1, r2;

	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		*flag |= 0x80004000;	xt0 = -1024;	}
	if( (xt0 >  1023) )					{		*flag |= 0x80004000;	xt0 =  1023;	}
	if( (yt0 < -1024) )					{		*flag |= 0x80002000;	yt0 = -1024;	}
	if( (yt0 >  1023) )					{		*flag |= 0x80002000;	yt0 =  1023;	}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x01000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x00800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);


	if( (FNEAR<SCR2) && (z<SCR2 ) )
		p = lnFOG( SCR2 );
	else
		p = lnFOG( z );
	
	NormalColorDpq3(v3,v4,v5,v6,p,v7,v8,v9);

	return ((r0.vz + r1.vz + r2.vz)/12) >> 2;
}
#endif //USE_ATI


#ifndef USE_ATI


long RotColorMatDpq( SVECTOR * v0, SVECTOR * v1, CVECTOR * v2, 
							long * sxy, CVECTOR * v3, long matc, long * flag )
{
	long xt, yt, zt;
	VECTOR r0, r1, r2;
	long z, p, p2;

	*flag = 0;
				   
	ApplyMatrix( &RTM, v0, &r0 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt = ((screenZ * r0.vx) / z) + xOffset;
	yt = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt < -1024) )					{		*flag |= 0x80004000;	xt = -1024;	}
	if( (xt >  1023) )					{		*flag |= 0x80004000;	xt =  1023;	}
	if( (yt < -1024) )					{		*flag |= 0x80002000;	yt = -1024;	}
	if( (yt >  1023) )					{		*flag |= 0x80002000;	yt =  1023;	}

	*sxy = (yt << 16) | (xt & 0x0000FFFF);

	if( (FNEAR<SCR2) && (z<SCR2 ) )
		p = lnFOG( SCR2 );
	else
		p = lnFOG( z );

	p2 = 4096-p;

	xt = (long)(v1->vx);
	yt = (long)(v1->vy);
	zt = (long)(v1->vz);

	LLV[2].vx = ( ( xt * (long)(LLM.m[0][0]) ) + ( yt * (long)(LLM.m[0][1]) ) + ( zt * (long)(LLM.m[0][2]) ) ) >> SHIFT;
	LLV[2].vy = ( ( xt * (long)(LLM.m[1][0]) ) + ( yt * (long)(LLM.m[1][1]) ) + ( zt * (long)(LLM.m[1][2]) ) ) >> SHIFT;
	LLV[2].vz = ( ( xt * (long)(LLM.m[2][0]) ) + ( yt * (long)(LLM.m[2][1]) ) + ( zt * (long)(LLM.m[2][2]) ) ) >> SHIFT;

	LLV[2].vx = pow(LLV[2].vx, (pow(2,matc)));
	LLV[2].vy = pow(LLV[2].vy, (pow(2,matc)));
	LLV[2].vz = pow(LLV[2].vz, (pow(2,matc)));

	xt = (long)(LLV[2].vx);
	yt = (long)(LLV[2].vy);
	zt = (long)(LLV[2].vz);

	grgb[2].r = ( ( ( xt * (long)(LCM.m[0][0]) ) + ( yt * (long)(LCM.m[0][1]) ) + ( zt * (long)(LCM.m[0][2]) ) ) >> SHIFT) + ( BK.r );
	grgb[2].g = ( ( ( xt * (long)(LCM.m[1][0]) ) + ( yt * (long)(LCM.m[1][1]) ) + ( zt * (long)(LCM.m[1][2]) ) ) >> SHIFT) + ( BK.g );
	grgb[2].b = ( ( ( xt * (long)(LCM.m[2][0]) ) + ( yt * (long)(LCM.m[2][1]) ) + ( zt * (long)(LCM.m[2][2]) ) ) >> SHIFT) + ( BK.b );

	v3->r = grgb[2].r = ( (p2 * v2->r * grgb[2].r) + (p * FC.r) ) >> SHIFT2;
	v3->g = grgb[2].g = ( (p2 * v2->g * grgb[2].g) + (p * FC.g) ) >> SHIFT2;
	v3->b = grgb[2].b = ( (p2 * v2->b * grgb[2].b) + (p * FC.b) ) >> SHIFT2;

	return (r0.vz >> 2);
}
#endif //USE_ATI



MATRIX * RotMatrix( SVECTOR * r, MATRIX * m )
{
	long	cx, sx;
	long	cy, sy;
	long	cz, sz;

	sx = rsin( r->vx );	cx = rcos( r->vx );
	sy = rsin( r->vy );	cy = rcos( r->vy );
	sz = rsin( r->vz );	cz = rcos( r->vz );

	m->m[0][0] =  MULT(cy,cz);
	m->m[0][1] = -MULT(cy,sz);
	m->m[0][2] =  sy;
	m->m[1][0] =  ((sz*cx) + (MULT(sy,sx)*cz)) >> SHIFT;
	m->m[1][1] =  ((cx*cz) - (MULT(sy,sx)*sz)) >> SHIFT;	
	m->m[1][2] = -MULT(cy,sx);
	m->m[2][0] =  ((sz*sx) - (MULT(cx,cz)*sy)) >> SHIFT;
	m->m[2][1] =  ((sx*cz) + (MULT(cx,sz)*sy)) >> SHIFT; 	
	m->m[2][2] =  MULT(cy,cx);
	
	return m;
}



#ifndef USE_ATI

MATRIX * RotMatrixC( SVECTOR * r, MATRIX * m )
{
	//As RotMatrixC is slower than RotMatrix we can safely do the following

	return RotMatrix( r, m );
}
#endif //USE_ATI



#ifndef USE_ATI

long RotNClip3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2,
					 long * sxy0, long * sxy1, long * sxy2,
					 long *p, long * otz, long * flag )
{
	long	result;

	result = RotAverageNClip3(v0,v1,v2,sxy0,sxy1,sxy2,p,otz,flag);
	return(result);
}

#endif //USE_ATI

#ifndef USE_ATI


long RotNClip4( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2, SVECTOR * v3,
					 long * sxy0, long * sxy1, long * sxy2, long * sxy3,
					 long *p, long * otz, long * flag )
{
	long	result;

	result = RotAverageNClip4(v0,v1,v2,v3,sxy0,sxy1,sxy2,sxy3,p,otz,flag);
	return(result);
}
#endif //USE_ATI



void RotTrans( SVECTOR * v0, VECTOR * v1, long * flag )
{
	register long z;

	ApplyMatrix( &RTM, v0, v1 );

	z = v1->vz;
	if( z < 0 )							{	*flag |= 0x00040000;					}
	if( z > 32768 )						{	*flag |= 0x00400000;					}
	if( z < SCR2 )						{	*flag |= 0x00020000; z = SCR2;			}
	if( z > 65536 )						{						 z = 65536;			}
	//if( (z < FNEAR) || (z > FFAR) )		{	*flag |= 0x00001000;					}
	if( v1->vx > 32767 )				{	*flag |= 0x81000000; v1->vx = 32767;	}
	if( v1->vy > 32767 )				{	*flag |= 0x80800000; v1->vy = 32767;	}
	if( v1->vx < -32767 )				{	*flag |= 0x81000000; v1->vx = -32767;	}
	if( v1->vy < -32767 )				{	*flag |= 0x80800000; v1->vy = -32767;	}
}

long RotTransPersForN(SVECTOR *v0, long *sxy, long *p, long *flag)
{
	VECTOR 	r0;


	ApplyMatrix(&RTM, v0, &r0);
	DLSRotTransPers(sxy, &r0, flag);
/*
	if (r0.vz < FNEAR)
		*p = 0;
	else if (r0.vz > FFAR)
		*p = 4095;
	else
		tmp = (((r0.vz - FNEAR) * 4095.0) / (float)(FFAR-FNEAR));
		*p = (long)tmp;
*/
	return r0.vz;
}


void RotTransPersN(SVECTOR *v0,DVECTOR *v1,u_short *sz,u_short *flag,long n, u_short *origz)
{
	int loop;
	SVECTOR *ip;
	//DVECTOR v2;
	DVECTOR *op;
	//SVECTOR *stmp;
	long	crap;
	long	long_flag=0;

// printf("\n\n!!!!!!!!! RotTransPersN !!!!!!!!!!!\n\n");


	ip = v0;
	//op = &v2;
	crap = NULL;
	op = v1;

	for (loop =0; loop < n; loop++)
	{
		*sz = (u_short)RotTransPersForN(ip,(long *)op,&crap,&long_flag);
		sz++;
 		*flag = (u_short)(long_flag >> 12);
		flag++;
		*origz = ip->vz;

		//stmp->vx = op->vx;
		//stmp->vy = op->vy;
		//stmp->vz = ip->vz;
		ip++;
		op++;
		origz++;
		//stmp++;

	}
}	  



long RotTransPers(SVECTOR *v0, long *sxy, long *p, long *flag)
{
	VECTOR 	r0;
	//DVECTOR sxy1;
	//long v1[3];
	//long *v2;

	//v2 = &v1[0];

	ApplyMatrix(&RTM, v0, &r0);
	DLSRotTransPers(sxy, &r0, flag);

/*
	if (r0.vz < FNEAR)
		*p = 0;
	else if (r0.vz > FFAR)
		*p = 4095;
	else
		*p = (long)(((r0.vz - FNEAR) * 4095.0) / (float)(FFAR-FNEAR));
*/
	//sxy->vx = v1[0];
	//sxy->vy = v1[1];
	//sxy->vz = v1[2];

	return r0.vz >> 2;
}



#ifndef USE_ATI

long RotTransPersx( SVECTOR * v0, long * sxy, long * p, long * flag )
{
	long 	z;
	long 	xt0, yt0;
	VECTOR 	r0;

	*flag = 0;

	ApplyMatrix(&RTM, v0, &r0);

	z = r0.vz;

	if( z < 0 )	
		*flag |= 0x00040000;

	if( z > 32768 )
		*flag |= 0x00400000;

	if( z < SCR2 )
	{
		*flag |= 0x00020000;	
		z = SCR2;
	}

	if( z > 65536 )
		z = 65536;

	//if( (z < FNEAR) || (z > FFAR) )
		//*flag |= 0x00001000;

	if( r0.vx > 32767 )
	{
		*flag |= 0x81000000;
		r0.vx = 32767;
	}

	if( r0.vy > 32767 )
	{		
		*flag |= 0x80800000;	
		r0.vy = 32767;
	}

	if( r0.vx < -32767 )
	{
		*flag |= 0x81000000;
		r0.vx = -32767;
	}

	if( r0.vy < -32767 )
	{
		*flag |= 0x80800000;	
		r0.vy = -32767;
	}

	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;

	if( (xt0 < -1024) )
	{
		*flag |= 0x80004000;
		xt0 = -1024;
	}

	if( (xt0 >  1023) )
	{
		*flag |= 0x80004000;
		xt0 =  1023;
	}

	if( (yt0 < -1024) )
	{
		*flag |= 0x80002000;
		yt0 = -1024;
	}

	if( (yt0 >  1023) )
	{
		*flag |= 0x80002000;
		yt0 =  1023;
	}

	*sxy = (yt0 << 16) | (xt0 & 0x0000FFFF);

	return r0.vz >> 2;

//	if( (FNEAR<SCR2) && (z<SCR2 ) )
//		*p = lnFOG( SCR2 );
//	else
//		*p = lnFOG( z );
}
#endif //USE_ATI



#ifndef USE_ATI

long RRRotTransPers2(SVECTOR *v0, SVECTOR *v1,
					 long *sxy0, long *sxy1)
{
	VECTOR 	r0, r1;
	long 	*flag;
	long 	z;
	long 	xt0, yt0;
	long 	xt1, yt1;


	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );

	z = r0.vz;
	if( z < SCR2 )						{		z = SCR2;		}
	if( z > 65536 )						{		z = 65536;		}
	if( r0.vx > 32767 )					{		r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		r0.vy = 32767;	}
	if( r0.vx < -32767 )				{		r0.vx = -32767;	}
	if( r0.vy < -32767 )				{		r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		xt0 = -1024;	}
	if( (xt0 >  1023) )					{		xt0 =  1023;	}
	if( (yt0 < -1024) )					{		yt0 = -1024;	}
	if( (yt0 >  1023) )					{		yt0 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);


	z = r1.vz;
	if( z < SCR2 )						{		z = SCR2;		}
	if( z > 65536 )						{		z = 65536;		}
	if( r1.vx > 32767 )					{		r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		r1.vy = 32767;	}
	if( r1.vx < -32767 )				{		r1.vx = -32767;	}
	if( r1.vy < -32767 )				{		r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		xt1 = -1024;	}
	if( (xt1 >  1023) )					{		xt1 =  1023;	}
	if( (yt1 < -1024) )					{		yt1 = -1024;	}
	if( (yt1 >  1023) )					{		yt1 =  1023;	}

	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);

	return r0.vz >> 2;
}
#endif //USE_ATI



#ifndef USE_ATI

long RotTransPers3( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2,
						  long * sxy0, long * sxy1, long * sxy2,
						  long * p, long * flag )
{
	long z;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	VECTOR r0, r1, r2;

	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );

	z = r0.vz;
	if( z < 0 )							{	*flag |= 0x00040000;					}
	if( z > 32768 )						{	*flag |= 0x00400000;					}
	if( z < SCR2 )						{	*flag |= 0x00020000; z = SCR2;			}
	if( z > 65536 )						{						 z = 65536;			}
	//if( (z < FNEAR) || (z > FFAR) )		{	*flag |= 0x00001000;					}
	if( r0.vx > 32767 )					{	*flag |= 0x81000000; r0.vx = 32767;		}
	if( r0.vy > 32767 )					{	*flag |= 0x80800000; r0.vy = 32767;		}
	if( r0.vx < -32767 )				{	*flag |= 0x81000000; r0.vx = -32767;	}
	if( r0.vy < -32767 )				{	*flag |= 0x80800000; r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{	*flag |= 0x80004000; xt0 = -1024;		}
	if( (xt0 >  1023) )					{	*flag |= 0x80004000; xt0 =  1023;		}
	if( (yt0 < -1024) )					{	*flag |= 0x80002000; yt0 = -1024;		}
	if( (yt0 >  1023) )					{	*flag |= 0x80002000; yt0 =  1023;		}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x01000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x00800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);

//	if( (FNEAR<SCR2) && (z<SCR2 ) )
//		*p = lnFOG( SCR2 );
//	else
//		*p = lnFOG( z );

	return r2.vz >> 2;
}
#endif //USE_ATI


#ifndef USE_ATI


long RotTransPers4( SVECTOR * v0, SVECTOR * v1, SVECTOR * v2, SVECTOR * v3,
						  long * sxy0, long * sxy1, long * sxy2, long * sxy3,
						  long * p, long * flag )
{
	long z;
	long xt0, yt0;
	long xt1, yt1;
	long xt2, yt2;
	long xt3, yt3;
  	VECTOR r0, r1, r2, r3;

	*flag = 0;

	ApplyMatrix( &RTM, v0, &r0 );
	ApplyMatrix( &RTM, v1, &r1 );
	ApplyMatrix( &RTM, v2, &r2 );
	ApplyMatrix( &RTM, v3, &r3 );

	z = r0.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r0.vx > 32767 )					{		*flag |= 0x81000000;	r0.vx = 32767;	}
	if( r0.vy > 32767 )					{		*flag |= 0x80800000;	r0.vy = 32767;	}
	if( r0.vx < -32767 )					{		*flag |= 0x81000000;	r0.vx = -32767;	}
	if( r0.vy < -32767 )					{		*flag |= 0x80800000;	r0.vy = -32767;	}
	xt0 = ((screenZ * r0.vx) / z) + xOffset;
	yt0 = ((screenZ * r0.vy) / z) + yOffset;
	if( (xt0 < -1024) )					{		*flag |= 0x80004000;	xt0 = -1024;	}
	if( (xt0 >  1023) )					{		*flag |= 0x80004000;	xt0 =  1023;	}
	if( (yt0 < -1024) )					{		*flag |= 0x80002000;	yt0 = -1024;	}
	if( (yt0 >  1023) )					{		*flag |= 0x80002000;	yt0 =  1023;	}

	
	z = r1.vz;
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r1.vx > 32767 )					{		*flag |= 0x81000000;	r1.vx = 32767;	}
	if( r1.vy > 32767 )					{		*flag |= 0x80800000;	r1.vy = 32767;	}
	if( r1.vx < -32767 )					{		*flag |= 0x81000000;	r1.vx = -32767;	}
	if( r1.vy < -32767 )					{		*flag |= 0x80800000;	r1.vy = -32767;	}
	xt1 = ((screenZ * r1.vx) / z) + xOffset;
	yt1 = ((screenZ * r1.vy) / z) + yOffset;
	if( (xt1 < -1024) )					{		*flag |= 0x80004000;	xt1 = -1024;	}
	if( (xt1 >  1023) )					{		*flag |= 0x80004000;	xt1 =  1023;	}
	if( (yt1 < -1024) )					{		*flag |= 0x80002000;	yt1 = -1024;	}
	if( (yt1 >  1023) )					{		*flag |= 0x80002000;	yt1 =  1023;	}

	
	z = r2.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r2.vx > 32767 )					{		*flag |= 0x01000000;	r2.vx = 32767;	}
	if( r2.vy > 32767 )					{		*flag |= 0x00800000;	r2.vy = 32767;	}
	if( r2.vx < -32767 )					{		*flag |= 0x81000000;	r2.vx = -32767;	}
	if( r2.vy < -32767 )					{		*flag |= 0x80800000;	r2.vy = -32767;	}
	xt2 = ((screenZ * r2.vx) / z) + xOffset;
	yt2 = ((screenZ * r2.vy) / z) + yOffset;
	if( (xt2 < -1024) )					{		*flag |= 0x80004000;	xt2 = -1024;	}
	if( (xt2 >  1023) )					{		*flag |= 0x80004000;	xt2 =  1023;	}
	if( (yt2 < -1024) )					{		*flag |= 0x80002000;	yt2 = -1024;	}
	if( (yt2 >  1023) )					{		*flag |= 0x80002000;	yt2 =  1023;	}

	z = r3.vz;
	if( z < 0 )								{		*flag |= 0x00040000;						}
	if( z > 32768 )						{		*flag |= 0x00400000;						}
	if( z < SCR2 )							{		*flag |= 0x00020000;	z = SCR2;			}
	if( z > 65536 )						{									z = 65536;		}
	//if( (z < FNEAR) || (z > FFAR) )	{		*flag |= 0x00001000;						}
	if( r3.vx > 32767 )					{		*flag |= 0x81000000;	r3.vx = 32767;	}
	if( r3.vy > 32767 )					{		*flag |= 0x80800000;	r3.vy = 32767;	}
	if( r3.vx < -32767 )					{		*flag |= 0x81000000;	r3.vx = -32767;	}
	if( r3.vy < -32767 )					{		*flag |= 0x80800000;	r3.vy = -32767;	}
	xt3 = ((screenZ * r3.vx) / z) + xOffset;
	yt3 = ((screenZ * r3.vy) / z) + yOffset;
	if( (xt3 < -1024) )					{		*flag |= 0x80004000;	xt3 = -1024;	}
	if( (xt3 >  1023) )					{		*flag |= 0x80004000;	xt3 =  1023;	}
	if( (yt3 < -1024) )					{		*flag |= 0x80002000;	yt3 = -1024;	}
	if( (yt3 >  1023) )					{		*flag |= 0x80002000;	yt3 =  1023;	}

	*sxy0 = (yt0 << 16) | (xt0 & 0x0000FFFF);
	*sxy1 = (yt1 << 16) | (xt1 & 0x0000FFFF);
	*sxy2 = (yt2 << 16) | (xt2 & 0x0000FFFF);
	*sxy3 = (yt3 << 16) | (xt3 & 0x0000FFFF);

	if( (FNEAR<SCR2) && (z<SCR2 ) )
		*p = lnFOG( SCR2 );
	else
		*p = lnFOG( z );

	return r3.vz >> 2;
}
#endif //USE_ATI



#ifndef USE_ATI

MATRIX * ScaleMatrix( MATRIX * m, VECTOR * v )
{
	m->m[0][0] = MULT(m->m[0][0], v->vx);	
	m->m[0][1] = MULT(m->m[0][1], v->vx);	
	m->m[0][2] = MULT(m->m[0][2], v->vx);
	m->m[1][0] = MULT(m->m[1][0], v->vy);	
	m->m[1][1] = MULT(m->m[1][1], v->vy);	
	m->m[1][2] = MULT(m->m[1][2], v->vy);
	m->m[2][0] = MULT(m->m[2][0], v->vz);	
	m->m[2][1] = MULT(m->m[2][1], v->vz);	
	m->m[2][2] = MULT(m->m[2][2], v->vz);

	return m;
}
#endif //USE_ATI



void SetBackColor( long rbk, long gbk, long bbk )
{
	BK.r = rbk;
	BK.g = gbk;
	BK.b = bbk;
}



void SetColorMatrix( MATRIX * m )
{
	LCM = *m;
}



void SetDrawDistance(long Dist)
{
	MaxZValue = Dist;
	HalfZValue = Dist >> 1;
}


#ifndef USE_ATI


void SetFarColor( long rfc, long gfc, long bfc )
{
	FC.r = rfc;
	FC.g = gfc;
	FC.b = bfc;
}

#endif //USE_ATI

#ifndef USE_ATI


void SetFogFar( long a, long h )
{
	FFAR = a;
	FNEAR = a / 5;
}

#endif //USE_ATI


#ifndef USE_ATI

void SetFogNear( long a, long h )
{
	FNEAR = a;
	FFAR = a * 5;
}

#endif //USE_ATI


void SetGeomOffset( long ofx, long ofy)
{					
	xOffset = ofx;
	yOffset = ofy;
}



void SetGeomScreen( long h )
{
	screenZ = h;
	SCR2 = h >> 1;
}



void SetLightMatrix( MATRIX * m )
{
	LLM = *m;
}



void SetRotMatrix( MATRIX * m )
{
	memcpy( &(RTM.m), &(m->m), sizeof(short)*9 );
}



void SetTransMatrix( MATRIX * m )
{
	memcpy( &(RTM.t), &(m->t), sizeof(long)*3 );
}



#ifndef USE_ATI

VECTOR * Square0( VECTOR * v0, VECTOR * v1 )
{
	v1->vx = (v0->vx * v0->vx);
	v1->vy = (v0->vy * v0->vy);
	v1->vz = (v0->vz * v0->vz);

	return v1;
}
#endif //USE_ATI



#ifndef USE_ATI

VECTOR * Square12( VECTOR * v0, VECTOR * v1 )
{
	v1->vx = (v0->vx * v0->vx) >> SHIFT;
	v1->vy = (v0->vy * v0->vy) >> SHIFT;
	v1->vz = (v0->vz * v0->vz) >> SHIFT;

	return v1;
}

#endif //USE_ATI


long SquareRoot0( long a )
{
unsigned long b;
	b = (unsigned long)a ;
	return sqrt((unsigned long) b );
}


#ifndef USE_ATI


long SquareRoot12( long a )
{
unsigned long b;
	b = (unsigned long)a;
	return (long)(sqrt( (double)b / ONE_FLOAT ) * ONE_FLOAT);
}

#endif //USE_ATI


#ifndef USE_ATI

void SubPol3( POL3 * p, SPOL * sp, int ndiv )
{
}
#endif //USE_ATI



#ifndef USE_ATI

void SubPol4( POL4 * p, SPOL * sp, int ndiv )
{
}

#endif //USE_ATI


MATRIX * TransMatrix( MATRIX * m, VECTOR * v )
{
	m->t[0] = v->vx;
	m->t[1] = v->vy;
	m->t[2] = v->vz;

	return m;	
}



#ifndef USE_ATI

void VectorNormal( VECTOR * v0, VECTOR * v1 )
{
	long	xSquare,ySquare,zSquare;
	long	Length;

	xSquare = v0->vx*v0->vx;
	ySquare = v0->vy*v0->vy;
	zSquare = v0->vz*v0->vz;

	Length = sqrt(xSquare+ySquare+zSquare);

	v1->vx = v0->vx/Length;
	v1->vy = v0->vy/Length;
	v1->vz = v0->vz/Length;
}
#endif //USE_ATI
