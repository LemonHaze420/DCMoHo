#include 	"Common.h"

#if TARGET == DC

#include 	"DCPlatform.h"

CDCMemoryPool	GLOBAL_PSX_POOL;

void	SetDCPool(CDCMemoryPool **p)
{
	*p = &GLOBAL_PSX_POOL;
}



#endif