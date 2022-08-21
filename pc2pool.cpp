#include 	"Common.h"

#if TARGET == PSX

#include 	"PSXPlatform.h"

CPSXMemoryPool	GLOBAL_PSX_POOL;

void	SetPSXPool(CPSXMemoryPool **p)
{
	*p = &GLOBAL_PSX_POOL;
}



#endif