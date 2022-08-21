// $Header$

// $Log$
// Revision 1.7  2000-09-27 15:10:08+01  jjs
// AddedFASTFTOL to PC version.
//
// Revision 1.6  2000-09-24 11:44:47+01  jcl
// <>
//
// Revision 1.5  2000-09-20 11:47:00+01  jcl
// <>
//
// Revision 1.4  2000-09-18 11:12:02+01  sds
// <>
//
// Revision 1.3  2000-08-07 15:46:40+01  jjs
// DC and PC merged code.
//
// Revision 1.2  2000-07-10 15:18:27+01  sds
// SDS - #if TARGET == DC stuff e.t.c.
//
// Revision 1.1  2000-06-30 16:33:02+01  sds
// Added the Usual "if def" stuff
//

#ifndef FIXEDPT_H
#define FIXEDPT_H

#if TARGET == PC || TARGET == DC
#define INLINE __inline
#elif TARGET == PSX
#define INLINE inline
#include <asm.h>
//#include <mwinline.h>
//#include <gtemac.h>
#endif
#if TARGET == DC
#include <math.h>
#include <sg_xpt.h>
#endif

//**********************************************************************
// constanta
#define GINT_SHIFT      16
#define GINT_MULT       (1 << GINT_SHIFT)
#define	GINT_MASK		(GINT_MULT - 1)
#define GINT_MULT_HALF	(1 << (8))

#define	GINT_MIN GINT(0x80000000)
#define	GINT_MAX GINT(0x7fffffff)

#define	G0		GINT(0)
#define G1		GINT(65536)
#define GINT_HALF GINT(32768)

//**********************************************************************
//** Oooh! some machine code!!


#if TARGET == PC
INLINE	SLONG	DIV64(SLONG a,SLONG b)
{
	SLONG		ret_v;
	__asm
	{
		 mov	edx,a
		 mov	ebx,b
		 mov	eax,edx
		 shl	eax,16
		 sar	edx,16
		 idiv	ebx
		 mov	ret_v,eax
	}
	return(ret_v);
}

INLINE  SLONG   MUL64(SLONG a,SLONG b)
{
	SLONG		ret_v;
	__asm
	{
		 mov	eax,a
		 imul   b
		 mov	ax,dx
		 rol	eax,16
		 mov	ret_v,eax
	}
	return(ret_v);
}
#elif TARGET == PSX

#if 0

inline SLONG DIV64(SLONG a,SLONG b)
{
    __asm_start();
    SLONG   ret_v;
    __evaluate(-1,(long)a);
    __evaluate(-2,(long)b);
    __evaluate(-3,(long)ret_v);
    __I_sra(-2,-2,11);
    __I_sll(-1,-1,5);
    __I_div(-1,-2);
    __I_mflo(-3);
    __asm_end();
    return ret_v;
}
#endif

//#define MUL64(a,b) ((a) / GINT_MULT_HALF) * ((b) / GINT_MULT_HALF)

#if 0

inline SLONG MUL64(SLONG a,SLONG b)
{
	__asm_start();
	register long result,tmp;
	__evaluate(-1,(a));
	__evaluate(-2,(b));
	__evaluate(-3,result);
	__evaluate(-4,tmp);
	__I_nop();
	__I_mult(-1,-2);
	__I_nop();
	__I_mfhi(-4);
	__I_mflo(-3);
	__I_sll(-4,-4,16);
 	__I_srl(-3,-3,16);
 	__asm_end();
 	__I_or(-3,-3,-4);
 	return(result);
}
#endif

extern SLONG MUL64(SLONG a,SLONG b);

#if 0
inline SLONG MUL64(SLONG a,SLONG b)
{
	register UINT __z,high;
	__z=a*b;
	asm volatile ("	mfhi %0":"=r"(high):"r" (high) :);
	return (__z>>16)|(high<<16);
}
#endif

//#define DIV64(a,b) ((ABS(a) < (0x400000))?((a) <<8) / ((b) >>8 ):(a) / ((b)>>16))

extern SLONG DIV64(SLONG a,SLONG b);


//#define DIV64(a,b) ((ABS(a) < (GINT_MULT * GINT_MULT_HALF / 4))?((a) * GINT_MULT_HALF) / ((b) / GINT_MULT_HALF):(a) / ((b) / GINT_MULT))


#if 0
INLINE  SLONG MUL64(SLONG a, SLONG b)
{
	return (a / GINT_MULT_HALF) * (b / GINT_MULT_HALF);
}

INLINE	SLONG	DIV64(SLONG a,SLONG b)
{
	if (ABS(a) < (GINT_MULT * GINT_MULT_HALF / 4))
		return (a * GINT_MULT_HALF) / (b / GINT_MULT_HALF);
	else
		return a / (b / GINT_MULT);
}
#endif
#elif TARGET == DC

extern  SLONG MUL64(SLONG a, SLONG b);

INLINE	SLONG	DIV64(SLONG a,SLONG b)
{
	if (ABS(a) < (GINT_MULT * GINT_MULT_HALF / 4))
		return (a * GINT_MULT_HALF) / (b / GINT_MULT_HALF);
	else
		return a / (b / GINT_MULT);
}
#endif

//** That's enough of that!
//**********************************************************************

// definition for GINT class.

class GINT
{
//private:
    //union  {
    //    SINT    mVal;
    //    struct  {
    //        UWORD mFrac;
    //        SWORD mWhole;
    //            }w;
    //}u;
public:
	SINT		mVal;
	// *********************************************************************
	// ** Constructors
	INLINE	GINT() {} ;

	INLINE	GINT(const SINT v, const SINT q) : mVal((v << GINT_SHIFT) + q) {}
/*	{
		ASSERT(ABS(v) < (1 << (GINT_SHIFT - 1)));
		ASSERT(q < (1 << GINT_SHIFT));
		ASSERT(q >= 0);
        mVal = (v << GINT_SHIFT) + q;
	}*/

	INLINE	GINT(const GINT &g) : mVal(g.mVal) {}
/*	{
        mVal = g.mVal;
	}*/

//	INLINE	GINT(const SINT i) : mVal(i) {}
	INLINE	GINT(const SINT i)
	{
		mVal = i;
	}

	INLINE	void	SetVal(const SINT v)
	{
        mVal = v;
	}

	INLINE	SINT	GetVal() const
	{
        return mVal;
	}

	INLINE	BOOL	IsZero()
	{
        return mVal == 0;
	}
public:

	// *********************************************************************
	// ** Access

#ifdef USE_FLOATING_POINT
//	operator	float() const
//	{
//		return float(mVal) / float(GINT_MULT);
//	}
#endif

	INLINE	SINT	Whole() const
    {
        return mVal>>GINT_SHIFT;
    }

	INLINE	SINT	Frac() const
	{
        return mVal&GINT_MASK;
    }

	INLINE	GINT	GFrac() const
	{
//        return GINT(0, mVal&GINT_MASK);
        return GINT(mVal&GINT_MASK);
	}


	// *********************************************************************
	// ** Arithmetic stuff

	// Adding & Subtracting..
	INLINE	const	GINT &operator += (const GINT &other)
	{
        mVal += other.mVal;
		return *this;
	}

	INLINE	const	GINT operator + (const GINT &other) const
	{
//		GINT	g;
//        g.mVal = mVal + other.mVal;
//		return g;
		return GINT(mVal + other.mVal);
	}

	INLINE	const	GINT operator - (const GINT &other) const
	{
//		GINT	g;
//        g.mVal = mVal - other.mVal;
//		return g;
		return GINT(mVal - other.mVal);
	}
	INLINE	const	GINT operator - () const
	{
//		GINT	g;
//      g.mVal = -mVal;
//		return g;
		return GINT(-mVal);
	}

/*	INLINE	GINT AddSWORD(const SWORD x) const
	{
		GINT g;
		g.mVal=mVal+(x<<8);
		return g;
	}

	INLINE	void PlusEqSWORD(const SWORD x)
	{
		mVal+=(x<<8);
	}

	INLINE	GINT SubSWORD(const SWORD x) const
	{
		GINT g;
		g.mVal=mVal-(x<<8);
		return g;
	}

	INLINE	void MinusEqSWORD(const SWORD x)
	{
		mVal-=(x<<8);
	}*/

	INLINE	const	GINT &operator -= (const GINT &other)
	{
        mVal -= other.mVal;
		return *this;
	}

	INLINE	const	GINT operator -- (int notused)
	{
		GINT old = *this;
        mVal -= GINT_MULT;
		return old;
	}

	INLINE	const	GINT &operator -- ()
	{
        mVal -= GINT_MULT;
		return *this;
	}

	INLINE	const	GINT operator ++ (int notused)
	{
		GINT old = *this;
        mVal += GINT_MULT;
		return old;
	}

	INLINE	const	GINT &operator ++ ()
	{
        mVal += GINT_MULT;
		return *this;
	}

	// Multiplication etc..
	// with SINTS
	INLINE	const	GINT &operator *= (const SINT &other)
	{
        mVal = mVal * other;
		return *this;
	}

	INLINE	const	GINT &operator /= (const SINT &other)
	{
        mVal = mVal / other;
		return *this;
	}

	INLINE	const	GINT operator * (const SINT &other) const
	{
//		GINT	g;
//        g.mVal = mVal * other;
//		return g;
		return GINT(mVal * other);
	}

	INLINE	const	GINT operator / (const SINT &other) const
	{
//		GINT	g;
//      g.mVal = mVal / other;
//		return g;
		return GINT(mVal / other);
	}

	// and with GINTS
	INLINE	const	GINT &operator *= (const GINT &other)
	{
        //ASSERT(ABS((mVal / GINT_MULT) * (other.mVal / GINT_MULT)) < (GINT_MULT / 4));
//		mVal = (mVal / GINT_MULT_HALF) * (other.mVal / GINT_MULT_HALF);

        mVal = MUL64(mVal, other.mVal);

		return *this;
	}

	INLINE	const	GINT &operator /= (const GINT &other)
	{
/*		if (ABS(mVal) < (GINT_MULT * GINT_MULT_HALF / 4))
			mVal = (mVal * GINT_MULT_HALF) / (other.mVal / GINT_MULT_HALF);
		else
			mVal = mVal / (other.mVal / GINT_MULT);*/

        mVal = DIV64(mVal, other.mVal);

		return *this;
	}

	INLINE	const	GINT operator * (const GINT &other) const
	{
//		GINT	g;
/*        ASSERT(ABS((mVal / GINT_MULT) * (other.mVal / GINT_MULT)) < (GINT_MULT / 4));
		g.mVal = (mVal / GINT_MULT_HALF) * (other.mVal / GINT_MULT_HALF);*/

//        g.mVal = MUL64(mVal, other.mVal);
//        return g;
		return GINT(MUL64(mVal, other.mVal));
	}

	INLINE	const	GINT operator / (const GINT &other) const
	{
//		GINT	g;
/*		if (ABS(mVal) < (GINT_MULT * GINT_MULT_HALF / 4))
			g.mVal = (mVal * GINT_MULT_HALF) / (other.mVal / GINT_MULT_HALF);
		else
			g.mVal = mVal / (other.mVal / GINT_MULT);*/

//        g.mVal = DIV64(mVal, other.mVal);
//		return g;

		return GINT(DIV64(mVal, other.mVal));
	}

	INLINE	const	GINT operator >> (const SINT &other) const
	{
//		GINT g;
//        g.mVal = mVal >> other;
//		return g;
		return GINT(mVal >> other);
	}

	INLINE	const	GINT operator << (const SINT &other) const
	{
//		GINT g;
//	    g.mVal = mVal << other;
//		return g;
		return GINT(mVal << other);
	}


	// ****************************************************
	// comparison
	INLINE	const	int	operator == (const GINT &other) const
	{
        return mVal == other.mVal;
	}

	INLINE	const	int	operator != (const GINT &other) const
	{
        return mVal != other.mVal;
	}

	INLINE	const	int	operator > (const GINT &other) const
	{
        return mVal > other.mVal;
	}

	INLINE	const	int	operator >= (const GINT &other) const
	{
        return mVal >= other.mVal;
	}

	INLINE	const	int	operator < (const GINT &other) const
	{
        return mVal < other.mVal;
	}

	INLINE	const	int	operator <= (const GINT &other) const
	{
        return mVal <= other.mVal;
	}

	// **************************
	// Utility
	GINT	Abs() const
	{
//		GINT g;
//        if (mVal > 0)
//            g.SetVal(mVal);
//		else
//            g.SetVal(-mVal);
//  	return g;

#if TARGET==PSX
		return (mVal^(mVal>>31))-(mVal>>31);
#else
		return (mVal > 0) ? GINT(mVal) : GINT(-mVal);
#endif
	}
};

//*****************************************************************
// some utility functions

#ifdef USE_FLOATING_POINT
INLINE	GINT	FToG(const float f)
{
	GINT	g;

#if TARGET == PC
	SINT t;

	FASTFTOL(&t, f * float(GINT_MULT));
	g.SetVal(t);
#else
    g.SetVal(SINT(f * float(GINT_MULT)));
#endif
	return g;
}

INLINE	GINT	DToG(const double d)
{
	GINT	g;

#if TARGET == PC
	SINT t;
	
	FASTFTOL(&t, float(d * double(GINT_MULT)));
	g.SetVal(t);
#else
    g.SetVal(SINT(d * double(GINT_MULT)));
#endif
	return g;
}

INLINE	float	GToF(const GINT g)
{
    return float(g.GetVal()) / float(GINT_MULT);
}

#endif


#define d_2pi  (((3<<16)+9279)<<1)
#define d_pi  ((3<<16)+9279)
#define d_pi_2 (((3<<16)+9279)>>1)

INLINE GINT AngleDifference(GINT r1, GINT r2)
{

	if (r1.mVal < -d_pi_2 && r2.mVal > d_pi_2) r2.mVal-=(d_pi*2);
	if (r1.mVal > d_pi_2 && r2.mVal < -d_pi_2) r2.mVal+=(d_pi*2);
	return r1 - r2;

// For some reason, these statics DON'T just get one instance (PSX)!!! :o)
//	static GINT g_pi	=GINT(3, 9279) ;
//	static GINT g_pi_2	=(GINT(3, 9279) / 2) ;
//	static GINT tg_pi = g_pi * 2 ;
//	if (r1 < -g_pi_2 && r2 > g_pi_2) r2-=tg_pi ;
//	if (r1 > g_pi_2 && r2 < -g_pi_2) r2+=tg_pi  ;
//	return r1- r2 ;
}

/*INLINE	GINT	GABS(GINT g)
{
	return g.Abs();
}*/

#define GABS(g)  ((g).Abs())

INLINE	GINT	GABS2(GINT g1, GINT g2)
{
#if TARGET == PC
	return (g2.mVal > 0 ? g1 : - g1);
#else
	return (g1.mVal^(g2.mVal>>31))-(g2.mVal>>31);
#endif
}

INLINE	GINT	G(const SINT u)
{
//	return GINT(u, 0);
	return GINT(u << GINT_SHIFT);
}

INLINE	GINT	G(const SINT u, const SINT v)
{
//	return GINT(u, v);
	return GINT((u << GINT_SHIFT) + v);
}

INLINE	GINT	GSIGN(GINT g)
{
#if TARGET == PC
	return (g > G(0) ? G(1) : -G(1));
#else
	return(g.mVal>>31);
#endif
}

//#include	<libmath.h>

#if TARGET == PSX || TARGET == DC
/* The definitions below yield 2 integer bits, 30 fractional bits */
#define FPFRACBITS 16    /* Must be even! */
#define FPITERS    (15 + (FPFRACBITS >> 1))
#endif

#if TARGET == PC || TARGET == DC
INLINE  GINT    GROOT(const GINT g)
{
	return DToG(sqrt(GToF(g)));
}
#elif TARGET == PSX
GINT    GROOT(const GINT g);
#endif

//*****************************************************************

//const	GINT	G0 = G(0);
//const	GINT	G1 = G(1);
//const	GINT	GINT_HALF = GINT(0, 32768) ;

#endif

