#include	"Common.h"

#include	"FixedPt.h"
#if TARGET == DC

#pragma optimization_level 0
SLONG MUL64(SLONG a, SLONG b)
{
	asm 
	{
		.set reorder
		DMULS.L r4,r5
		STS MACH,r1
		STS MACL,r0
		XTRCT r1,r0
		RTS
	}   
}
#pragma optimization_level 4

#endif
#if TARGET == PSX
GINT    GROOT(const GINT g)
{
    unsigned long remLo=(unsigned long)(g.GetVal());
    unsigned long root, remHi,testDiv, count;
    //GINT d;

    root = 0;         /* Clear root */
    remHi = 0;        /* Clear high part of partial remainder */
    count = FPITERS;    /* Load loop counter */

    do {
        remHi = (remHi << 2) | (remLo >> 30); remLo <<= 2;  /* get 2 bits of arg */
        root <<= 1;   /* Get ready for the next bit in the root */
        testDiv = (root << 1) + 1;    /* Test radical */
        if (remHi >= testDiv) {
            remHi -= testDiv;
            root += 1;
        }
    } while (count-- != 0);
    //d.SetVal(root);
    return GINT(root);
}

#if 1
inline asm SLONG MUL64(SLONG a,SLONG b)
{
    mult    a0,a1
    mfhi     v1
    mflo     v0
    sll     v1,v1,16
    srl     v0,v0,16
    jr       ra
    or      v0,v1,v0
}
#endif

#if 1

inline asm SLONG DIV64(SLONG a,SLONG b)
{
	mtc2	a0,C2_LZCS
	addi	t1,zero,16
	sll		t0,a0,16
	mfc2	v0,C2_LZCR
	nop
	bgt		v0,16,_sl16div
	addi	v0,v0,-1
	sub		v1,t1,v0
	sllv	t0,a0,v0
	srav	a1,a1,v1
_sl16div:
	div		t0,a1
	mflo	v0
	jr	ra
	nop
}
#else

SLONG DIV64(SLONG a,SLONG b)
{
	return (ABS(a) < (GINT_MULT * GINT_MULT_HALF / 4))?((a) * GINT_MULT_HALF) / ((b) / GINT_MULT_HALF):(a) / ((b) / GINT_MULT);
}

#endif



#endif
