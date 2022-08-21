#include	"Common.h"

#include	"Globals.h"
#include	"Profile.h"
#include    <libmath.h>
#if TARGET == PSX
#include    <mwinline.h>
#include <gtemac.h>
#include <gtereg_s.h>
#include <inline_s.h>
#include "psx3dengine.h"
#elif TARGET == DC
#include	"dcengine.h"
#endif
/*
	NTAG routines...
*/





SLONG get3(int r0)
{
	unsigned long t;
#if defined(JJS)
	asm volatile ( "lwl %0, 2( %1 )" : "=r"(t) : "r"( r0) : "memory" );
#endif
	return t;
}



void set3(int r0,int r1)
{
#if defined(JJS)
	asm volatile ( "swl %1, 2( %0 )" : : "r"( r0 ), "r"( r1 ) : "memory" );
#endif
}

#define fsetaddr(_p0,_p1)	\
	set3((_p0), ((u_long) (_p1)) << 8)

#define fgetaddr(_p)	\
	(get3(_p) >> 8)

#define fcatPrim(p0, p1)		fsetaddr((int)p0, (int)p1)

 #if 0
 void NTAG_link(NTAG * ptr, int len, long *first, long *last)
{
	NTAG *start=ptr;
	NTAG * end;
	long * prim;
	long * next = ptr->lastprim;

	*first=NULL;
	*last=NULL;
	for (end=&ptr[len]; ptr!=end; ptr++)
	{
		prim=ptr->lastprim;
		if (prim == NULL)
			continue;
		catPrim(first, ptr->firstprim);  // see above
		first = prim;
	}

	catPrim(first, last);
	termPrim(last);                   // DO THIS MANUALLY... So I can build mre than one list...

}
#endif

#if 0
		#define fcatPrim(a,b)	\
{\
	__asm_start();\
	__evaluate(-1,(long)a);\
	__evaluate(-2,(long)b);\
	__I_sll(12,-2,8);\
	__I_swl(12,2,-1);\
	__asm_end();\
}
#endif

void NTAG_Rlink(NTAG * ptr,int len, long *first, long *last)
{
	NTAG * end=(NTAG*)ptr-1;
	NTAG * cur=end+len;
	long * lprim;
	long * next=cur->firstprim;
	long * fprim;
	*first=0;
	*last= 0;
	for (; cur!=end; cur--)
	{
		fprim=next;
		next=(cur-1)->firstprim;
		if	(fprim == NULL)
			continue;
		cur->firstprim=NULL;            // Clear for next loop round...
		lprim=cur->lastprim;
		fcatPrim(first,fprim);
		first=lprim;
	}
	fcatPrim(first, last);
	fsetaddr((long)last,(long)0xffffff);

}

