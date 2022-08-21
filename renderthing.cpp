#include	"Common.h"

#if TARGET == PC 

#include	"RenderThing.h"
#include	"Thing.h"
#include	"Globals.h"


//******************************************************************************************
void	CRenderThing::Init(CInitRenderThing *i)
{
	mThing = i->mThing;

};

//******************************************************************************************
void	CRenderThing::RenderCollisionVolume(SINT i)
{
	FVector p;
	FMatrix m;

	GetRenderPosition(&p);
	GetRenderOrientation(&m);

	ENGINE.SetupTransforms(p, m);

	ENGINE.DrawCollisionSphere(GetRenderRadius(), i);		
}








//******************************************************************************************

#endif
