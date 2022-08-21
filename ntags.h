/*
	NTAG header
*/

#define NTAG_BACK_TO_FRONT

typedef struct
{
	long *firstprim;
	long *lastprim;
}NTAG;

//#ifdef setaddr
//#undef setaddr
//#endif
//#define setaddr(_p0,_p1) {*(UWORD*)_p0=(UWORD)_p1;*(unsigned char *)((int)_p0+2)=(char)((int)_p1>>16);}


extern void NTAG_link(NTAG * ptr, int len, long *first, long *last);
extern void NTAG_Rlink(NTAG * ptr, int len, long *first, long *last);

static inline void NTAG_init(NTAG * ptr, int len)
{
	do
	{

#ifdef NTAG_BACK_TO_FRONT
		ptr[0].firstprim = NULL;
		ptr[1].firstprim = NULL;
		ptr[2].firstprim = NULL;
		ptr[3].firstprim = NULL;
#else
		ptr[0].lastprim = NULL;
		ptr[1].lastprim = NULL;
		ptr[2].lastprim = NULL;
		ptr[3].lastprim = NULL;
#endif
		ptr+=4;
		len-=4;
	}while(len>0);
}


#ifndef NTAG_BACK_TO_FRONT
// This is for adding prims to the END of a list.
// use this if drawing from FRONT to BACK into the list....
inline static void NTAG_addPrim(NTAG *ptr, long *prim)
{
	long *last=ptr->lastprim;
	ptr->lastprim = prim;
	if (last) {catPrim(last, prim);}
	else ptr->firstprim = prim;
}
#else
// This is for adding prims to the START of a list.
// use this if drawing from BACK to FRONT into the list....
inline static void NTAG_addPrim(NTAG *ptr, long *prim)
{
	long *first=ptr->firstprim;
	ptr->firstprim = prim;
	if (first) {catPrim(prim,first);}
	else ptr->lastprim = prim;
}

inline static void NTAG_addPrimSize(NTAG *ptr, long *prim,SINT size)
{
	long *first=ptr->firstprim;
	ptr->firstprim = prim;
	*(ULONG*)(prim)=(int)first|(size<<24);
	if(!first)ptr->lastprim = prim;
}




#endif


