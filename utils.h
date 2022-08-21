// $Header$

// $Log$
// Revision 1.2  2000-07-10 15:18:31+01  sds
// SDS - #if TARGET == DC stuff e.t.c.
//
// Revision 1.1  2000-06-30 16:34:48+01  sds
// Added the Usual "if def" stuff
//

#ifndef UTILS_H
#define UTILS_H
#include "types.h"
#include "SeedUniformRandomNumberStream.h"
extern CSeedUniformRandomNumberStream RANDOMSTREAM;
// random useful functions

inline	char	*my_strncpy(char *d, const char *s, int n)
{
	char *r = d;
	while((n > 0) && ((*(d ++) = *(s ++)) != NULL)) n --;
	return r;
}


#if TARGET!=DC

inline	SINT	ABS(SINT i)
{
	return (i > 0) ? i : -i;
}
#endif
inline	SINT	IMAX(SINT a, SINT b)
{
	return (a > b) ? a : b;
}

inline	SINT	IMIN(SINT a, SINT b)
{
	return (a < b) ? a : b;
}


// seeded random generator?!
#if TARGET == PC
inline	SINT	RANDOM()
{
	return RANDOMSTREAM.Next();
	//return rand(); //! sort this out
}


#elif TARGET == DC
inline	SINT	RANDOM()
{
	//!PSX write seeded random number generator.
	return RANDOMSTREAM.Next();
//	return rand();
}

#elif TARGET == PSX
inline	SINT	RANDOM()
{
	//!PSX write seeded random number generator.
	return RANDOMSTREAM.Next();
//	return rand();
}

#endif
#endif