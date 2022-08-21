// Crown.cpp: implementation of the CCrown class.
//
//////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "globals.h"
#include "Crown.h"
#include "Character.h"


//****************************************************************************************************
void	CCrown::Init(CInitThing *i)
{
	mInvisible = FALSE ;
	SUPERTYPE::Init(i) ;
	
	mBeingHeldBy = NULL;
}

//****************************************************************************************************
void	CCrown::Process()
{
	VALIDATE(mBeingHeldBy);

	SUPERTYPE::Process() ;
}

//****************************************************************************************************
GVector	CCrown::GetRenderPos(SINT no)
{
	if (mBeingHeldBy)
		return (mBeingHeldBy->GetRenderPos(0) +
			    mBeingHeldBy->GetRenderThingEmitterAnimated(5));
	else
		return SUPERTYPE::GetRenderPos(no);
}

//****************************************************************************************************
GMatrix	CCrown::GetRenderOrientation(SINT no)
{
	if (mBeingHeldBy)
		return mBeingHeldBy->GetRenderOrientation(1);
	else
		return SUPERTYPE::GetRenderOrientation(no);
}
