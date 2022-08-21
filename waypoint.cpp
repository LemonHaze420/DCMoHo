
#include "Common.h"
#include "Globals.h"
#include "WayPoint.h"
#include "Character.h"



//******************************************************************************************
void	CWayPoint::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	SitOnGround();
}
 


//******************************************************************************************
void	CWayPoint::Shutdown() 
{
	SUPERTYPE::Shutdown() ;

}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************


void	CPathFinderPoint::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	SitOnGround();
	WORLD.GetPathFinderPointsNB().Add(this) ;
}
 


//******************************************************************************************
void	CPathFinderPoint::Shutdown() 
{
	SUPERTYPE::Shutdown() ;
	WORLD.GetPathFinderPointsNB().Remove(this) ;
}



