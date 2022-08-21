#include "ntags.h"
/*
 * My version of the addPrim Macro.
 *
 * Reasons:
 * Quicker due to less memory read/writes.
 * Less instructions (less chance of cache overflow).
 */

#undef addPrim

#define AddPrim addPrim

//#define addPrim(ot,p)\
//{\
//        u_long ptag=(*(u_long*)(p));\
//        u_long ottag=(*(u_long*)(ot));\
//        (*(u_long*)(p))=ptag&0xff000000|(ottag&0x00ffffff);\
//        (*(u_long*)(ot))=((u_long)(p))&0x00ffffff;\
//}

inline void addPrimF(unsigned long *ot,POLY_GT3*p)
{
#ifdef _DEBUG
	if(!(((getcode(p)==0x20)&&( getlen(p)==4 ))||\
		   ((getcode(p)==0x24)&&( getlen(p)==7 ))|| \
		   ((getcode(p)==0x30)&&( getlen(p)==6 ))||  \
		   ((getcode(p)==0x34)&&( getlen(p)==9 ))||   \
		   ((getcode(p)==0x28)&&( getlen(p)==5 ))||    \
		   ((getcode(p)==0x2c)&&( getlen(p)==9 ))||     \
		   ((getcode(p)==0x38)&&( getlen(p)==8 ))||      \
		   ((getcode(p)==0x3c)&&( getlen(p)==12))||       \
		   ((getcode(p)==0x22)&&( getlen(p)==4 ))||        \
		   ((getcode(p)==0x26)&&( getlen(p)==7 ))||         \
		   ((getcode(p)==0x32)&&( getlen(p)==6 ))||          \
		   ((getcode(p)==0x36)&&( getlen(p)==9 ))||           \
		   ((getcode(p)==0x2a)&&( getlen(p)==5 ))||            \
		   ((getcode(p)==0x2e)&&( getlen(p)==9 ))||             \
		   ((getcode(p)==0x3a)&&( getlen(p)==8 ))||              \
		   ((getcode(p)==0x3e)&&( getlen(p)==12 ))||              \
		   ((getcode(p)==0x40)&&( getlen(p)==3  ))||    /*jcl*/    \
		   ((getcode(p)==0xe1))))printf("Illegal entry in OT, Code: 0x%02x  Length: 0x%02x\n",getcode(p),getlen(p));
#endif
#if defined(ONDC)
	__evaluate(-1,(long)ot);
	__evaluate(-2,(long)p);
	__asm_start();
	__I_lwl(12,2,-1);
	__I_sll(13,-2,8);
	__I_swl(13,2,-1);
	__I_swl(12,2,-2);
	__asm_end();
#endif
}

#if 0
	ASSERT(((getcode(p)==0x20)&&( getlen(p)==4 ))||\
		   ((getcode(p)==0x24)&&( getlen(p)==7 ))|| \
		   ((getcode(p)==0x30)&&( getlen(p)==6 ))||  \
		   ((getcode(p)==0x34)&&( getlen(p)==9 ))||   \
		   ((getcode(p)==0x28)&&( getlen(p)==5 ))||    \
		   ((getcode(p)==0x2c)&&( getlen(p)==9 ))||     \
		   ((getcode(p)==0x38)&&( getlen(p)==8 ))||      \
		   ((getcode(p)==0x3c)&&( getlen(p)==12))||       \
		   ((getcode(p)==0x22)&&( getlen(p)==4 ))||        \
		   ((getcode(p)==0x26)&&( getlen(p)==7 ))||         \
		   ((getcode(p)==0x32)&&( getlen(p)==6 ))||          \
		   ((getcode(p)==0x36)&&( getlen(p)==9 ))||           \
		   ((getcode(p)==0x2a)&&( getlen(p)==5 ))||            \
		   ((getcode(p)==0x2e)&&( getlen(p)==9 ))||             \
		   ((getcode(p)==0x3a)&&( getlen(p)==8 ))||              \
		   ((getcode(p)==0x3e)&&( getlen(p)==12 ))||              \
		   ((getcode(p)==0xe1)))					               \

#endif


// NEW!!! Using NTAGS now....
#define addPrim(ot,p) (NTAG_addPrim((ot),(long*)(p)))
#define addPrimSize(ot,p,size)	\
{\
	NTAG_addPrim((ot),(long*)(p));			\
	(*(char*)(((char*)p)+3)=size;			\
}
#if 0

#ifdef _DEBUG
#define addPrim(ot,p)\
{\
	ASSERT(((getcode(p)==0x20)&&( getlen(p)==4 ))||\
		   ((getcode(p)==0x24)&&( getlen(p)==7 ))|| \
		   ((getcode(p)==0x30)&&( getlen(p)==6 ))||  \
		   ((getcode(p)==0x34)&&( getlen(p)==9 ))||   \
		   ((getcode(p)==0x28)&&( getlen(p)==5 ))||    \
		   ((getcode(p)==0x2c)&&( getlen(p)==9 ))||     \
		   ((getcode(p)==0x38)&&( getlen(p)==8 ))||      \
		   ((getcode(p)==0x3c)&&( getlen(p)==12))||       \
		   ((getcode(p)==0x22)&&( getlen(p)==4 ))||        \
		   ((getcode(p)==0x26)&&( getlen(p)==7 ))||         \
		   ((getcode(p)==0x32)&&( getlen(p)==6 ))||          \
		   ((getcode(p)==0x36)&&( getlen(p)==9 ))||           \
		   ((getcode(p)==0x2a)&&( getlen(p)==5 ))||            \
		   ((getcode(p)==0x2e)&&( getlen(p)==9 ))||             \
		   ((getcode(p)==0x3a)&&( getlen(p)==8 ))||              \
		   ((getcode(p)==0x3e)&&( getlen(p)==12 ))||              \
		   ((getcode(p)==0x40)&&( getlen(p)==3  ))||   /*jcl*/     \
		   ((getcode(p)==0xe1)))					               \
	addPrimF((unsigned long*)ot,(POLY_GT3*)p);                      \
}
#else
#define addPrim(ot,p) (addPrimF((unsigned long *)ot,(POLY_GT3*)p))
#endif

#endif
// Use to cast pointers to other types. eg:

// *ULONGPTR(p4->x0)=0x12345678;
// Sets p4->x0 to 0x1234 amd p4->y0 to 0x5678 with one memory write.

#define ULONGPTR(a) ( (ULONG*) (&(a)) )
#define UBYTEPTR(a) ( (UBYTE*) (&(a)) )
#define UWORDPTR(a) ( (UWORD*) (&(a)) )
#define SLONGPTR(a) ( (SLONG*) (&(a)) )
#define SBYTEPTR(a) ( (SBYTE*) (&(a)) )
#define SWORDPTR(a) ( (SWORD*) (&(a)) )


#undef csin
#undef ccos
#ifndef rcossin_tbl
extern struct SINCOS { short sin, cos; } rcossin_tbl[];
#endif
#if TARGET == PSX
#define  csin(x)  (rcossin_tbl[(x)&0xfff].sin)
#define  ccos(x)  (rcossin_tbl[(x)&0xfff].cos)
#endif


/*-----------------11/01/2000 18:37-----------------
 * Okay, here follows some new prim types....
 *
 * 	These prims allow you to reset the ABR mode after
 * drawing (for silly stuf like shadows that make stuff
 * drawn after appear in the wrong mode....)
 * 	Just use the following macro and remember to add
 * one longword to the packet size when setting the tag...
 * --------------------------------------------------*/

#define setpostabr(p,abr) p->modeword=((0xe1<<24)|((abr)<<5));

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	x1,	y1;
	short	x2,	y2;
	u_long	modeword;
} POLY_F3MODE;				/* Flat Triangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	x1,	y1;
	short	x2,	y2;
	short	x3,	y3;
	u_long	modeword;
} POLY_F4MODE;				/* Flat Quadrangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	u_short	clut;
	short	x1,	y1;
	u_char	u1, v1;	u_short	tpage;
	short	x2,	y2;
	u_char	u2, v2;	u_short	pad1;
	u_long	modeword;
} POLY_FT3MODE;				/* Flat Textured Triangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	u_short	clut;
	short	x1,	y1;
	u_char	u1, v1;	u_short	tpage;
	short	x2,	y2;
	u_char	u2, v2;	u_short	pad1;
	short	x3,	y3;
	u_char	u3, v3;	u_short	pad2;
	u_long	modeword;
} POLY_FT4MODE;				/* Flat Textured Quadrangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	r1, g1, b1, pad1;
	short	x1,	y1;
	u_char	r2, g2, b2, pad2;
	short	x2,	y2;
	u_long	modeword;
} POLY_G3MODE;				/* Gouraud Triangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	r1, g1, b1, pad1;
	short	x1,	y1;
	u_char	r2, g2, b2, pad2;
	short	x2,	y2;
	u_char	r3, g3, b3, pad3;
	short	x3,	y3;
	u_long	modeword;
} POLY_G4MODE;				/* Gouraud Quadrangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	u_short	clut;
	u_char	r1, g1, b1, p1;
	short	x1,	y1;
	u_char	u1, v1;	u_short	tpage;
	u_char	r2, g2, b2, p2;
	short	x2,	y2;
	u_char	u2, v2;	u_short	pad2;
	u_long	modeword;
} POLY_GT3MODE;				/* Gouraud Textured Triangle */

typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	u_short	clut;
	u_char	r1, g1, b1, p1;
	short	x1,	y1;
	u_char	u1, v1;	u_short	tpage;
	u_char	r2, g2, b2, p2;
	short	x2,	y2;
	u_char	u2, v2;	u_short	pad2;
	u_char	r3, g3, b3, p3;
	short	x3,	y3;
	u_char	u3, v3;	u_short	pad3;
	u_long	modeword;
} POLY_GT4MODE;				/* Gouraud Textured Quadrangle */

typedef struct {
	u_long	tag;
	u_long	modeword;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	short	x1,	y1;
} LINE_F2MODE;				/* Unconnected Flat Line */





// -------------------------------------------------------------------------
// Now: define all the GPU non-drawing opcodes as pure binary numbers...
// -------------------------------------------------------------------------


#define	DR_NOP_CODE			0x00		// does nothing
#define	DR_AREASTART_CODE	0xe3
#define	DR_AREAEND_CODE	0xe4
#define	DR_OFFSET_CODE		0xe5

typedef struct {
	u_long	tag;
	u_long	dacode[2];
	u_long	ofcode[2];
	u_long	excode[4];                  // Set DTD + DFE and clear screen (TILE)
} DR_AREAOFFSET;				/* Drawing Area and offset*/


#define PT(_P) ((P_TAG *)_P)

#define tt_DrawNop()		(DR_NOP_CODE << 24)

#define tt_DrawAreaStart(_x,_y)	\
	(	(DR_AREASTART_CODE << 24)	\
	| (((_y) & 0x3FF) << 10)		\
	| ((_x) & 0xFFF)	)

#define tt_DrawAreaEnd(_x,_y)		\
	(	(DR_AREAEND_CODE << 24)		\
	| (((_y) & 0x3FF) << 10)		\
	| ((_x) & 0xFFF)	)

#define tt_DrawOffset(_x,_y)		\
	(	(DR_OFFSET_CODE << 24)		\
	| (((_y) & 0x7FF) << 11)		\
	| ((_x) & 0x7FF) )

#define dosetDrawArea(_p,_x,_y,_w,_h)			\
	(_p)->dacode[0] = tt_DrawAreaStart(_x, _y),	\
	(_p)->dacode[1] = tt_DrawAreaEnd((_x)+(_w)-1, (_y)+(_h)-1)\

#define dosetDrawOffset(_p,_x,_y)			\
	(_p)->ofcode[0] = tt_DrawOffset(_x,_y),	\
	(_p)->ofcode[1] = tt_DrawNop()


#define SetPolyAreaOffset(_p,_x,_y,_w,_h,_ofx,_ofy)	\
	(_p)->dacode[0] = tt_DrawAreaStart(_x, _y),	\
	(_p)->dacode[1] = tt_DrawAreaEnd((_x)+(_w)-1, (_y)+(_h)-1),\
	(_p)->ofcode[0] = tt_DrawOffset(_ofx,_ofy),	\
	(_p)->ofcode[1] = tt_DrawNop(),\
	PT(_p)->len = 8;


#define FtoPSX(x) ((SWORD)((x)*(1<<12)))
#define SetMLight(lm,n,r,g,b) (lm[0][n]=FtoPSX(r),lm[1][n]=FtoPSX(g),lm[2][n]=FtoPSX(b))

// these initiate and pull the result from the multipliers. !!!!USE WITH CARE!!!!

inline void  start_mult(SINT x,SINT y)
{
#if defined(ONDC)
	asm volatile("	mult %0,%1"::"r"(x),"r"(y):);
#endif
}


inline SINT get_mult(void)
{
	register SINT retval;
#if defined(ONDC)
	asm volatile("mflo %0":"=r"(retval)::);
#endif
	return retval;
}

// Load immediate values into vector 0 of the GTE.
   // NO MEMORY WRITE/READ!!! Saves some time!!

inline void gte_ldv0i(int x,int y,int z)
{
#if defined(ONDC)
	asm volatile("	sll		%1,%1,16
					andi	%0,%0,0xffff
					or		%0,%0,%1
					mtc2	%2,$1
					mtc2	%0,$0"::"r"(x),"r"(y),"r"(z):);
#endif
}

inline void gte_ldv1i(int x,int y,int z)
{
#if defined(ONDC)
	asm volatile("	sll		%1,%1,16
					andi	%0,%0,0xffff
					or		%0,%0,%1
					mtc2	%2,$3
					mtc2	%0,$2"::"r"(x),"r"(y),"r"(z):);
#endif
}
inline void gte_ldv2i(int x,int y,int z)
{
#if defined(ONDC)
	asm volatile("	sll		%1,%1,16
					andi	%0,%0,0xffff
					or		%0,%0,%1
					mtc2	%2,$5
					mtc2	%0,$4"::"r"(x),"r"(y),"r"(z):);
#endif
}


inline int gte_getsz0(void)
{
	register int retval;
#if defined(ONDC)
	asm volatile("	mfc2 %0,$1":"=r"(retval)::);
#endif
	return retval;
}

inline int gte_getopz(void)
{
	register int retval;
#if defined(ONDC)
	asm volatile("	mfc2 %0,$24":"=r"(retval)::);
#endif
	return retval;;
}

inline void gte_MACtoTRANS(void)
{
#if defined(ONDC)
	asm volatile("	mfc2	$12,$9
					mfc2    $13,$10
					mfc2	$14,$11
					ctc2	$12,$5
					ctc2	$13,$6
					ctc2	$14,$7":::"t4","t5","t6");
#endif
}



// This sets the DQA/DQB registers in the GTE to the correct value for depthqueueing....
inline void SetMyDepthQueue(void)// {SetFogNearFar(2800<<2,3400<<2,SCR_Z);}
{
	register int dqa,dqb;
#if defined(ONDC)
	asm volatile("	addi	%0,$0,-31733
					addi	%1,$0,0x5aaa
					sll		%1,%1,12
					ctc2	%0,$27
					ctc2	%1,$28"::"=r"(dqa),"=r"(dqb):);
#endif
}



inline SINT gte_Get_szotz(void)
{
	register SINT retval;
#if defined(ONDC)
	asm volatile("	mfc2 %0,$19
					nop
					sra	%0,%0,2":"=r"(retval)::);
#endif
	return retval;

}
inline SINT gte_Get_sz(void)
{
	register SINT retval;
#if defined(ONDC)
	asm volatile("	mfc2 %0,$19":"=r"(retval)::);
#endif
	return retval;

}

inline void gte_setrgb(SINT col)
{
#if defined(ONDC)
	asm volatile(" mtc2 %0,$6"::"r"(col):);
#endif
}

#define gte_stszotz( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_mfc2 (12, 19);\
   __I_nop ();\
   __I_sra (12, 12, 2);\
   __I_sw (12, 0, __arg0);\
   __asm_end();

// This Macro sets the DQA/DQB pair to the width of the sprite we want to scale.
// the P value returned by RotTransPers is the sprite width after perspective calculation
// according to the Z position of the sprite... Nifty Huh?

#define gte_SetWidthDamo( r0 )  \
   __evaluate (-1, (long)(r0));\
   __asm_start();\
   __I_ctc2(-1,27);\
   __I_addi(12,0,0);\
   __I_ctc2(12,28);\
   __asm_end();\


