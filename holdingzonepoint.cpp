
#include	"Common.h"

#include	"Globals.h"

#include "HoldingZonePoint.h"
#include "Character.h"


//******************************************************************************************
void		CHoldingZonePoint::Init(CInitThing *i) 
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	GET_TARGET;
	GET_COUNTER(0);
	if (mCounter0 ==-1)
	{
		CONSOLE.AddString("Warning: HoldingZonePoint not numbered") ;
	}
	if (mCounter0 ==0)
	{
		CONSOLE.AddString("Warning: HoldingZonePoint numbered 0 !!! ") ;
	}


	SitOnGround();
	mXBL = mYBL = mXTR = mYTR = G(0);
	WORLD.GetHoldingZonePointsNB().Add(this) ;	

};



//******************************************************************************************
void		CHoldingZonePoint::Shutdown()
{
	SUPERTYPE::Shutdown() ;
	WORLD.GetHoldingZonePointsNB().Remove(this) ;	
}


//******************************************************************************************
#if TARGET == PC
void	CHoldingZonePoint::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
	WRITE_COUNTER(0);
}	
#endif

//******************************************************************************************
GVector		CHoldingZonePoint::FindClosestPointInZoneTo(GVector& pos)
{
	GVector return_vec;
	// we are assuming the pos is not inside the holding zone !!!
	if (pos.X > mXTR)
	{
		return_vec.X = mXTR ;
	}
	else if (pos.X < mXBL)
	{
		return_vec.X = mXBL ;
	}
	else
	{
		return_vec.X = pos.X ;
	}

	if (pos.Y > mYTR)
	{
		return_vec.Y = mYTR ;
	}
	else if (pos.Y < mYBL)
	{
		return_vec.Y = mYBL ;
	}
	else
	{
		return_vec.Y = pos.Y ;
	}
	return_vec.Z = pos.Z ;
	return return_vec ;
}

	

//******************************************************************************************
BOOL		CHoldingZonePoint::IsPointInsideZone(GVector& point)
{	
	if (!mTarget) 
	{
		CONSOLE.AddString("Warning: you're calling IsPointInsideZone on wrong HoldingZonePoint") ;
		return FALSE;
	}

	// calc axis alligned box ( only done first time process called )

	if (mXBL == G(0) && mXTR == G(0))
	{
		GVector tarpos = mTarget->GetPos() ;
		if (tarpos.X > mPos.X)
		{	
			mXBL = mPos.X;
			mXTR = tarpos.X ;
		}
		else
		{
			mXTR = mPos.X;
			mXBL = tarpos.X ;
		}

		if (tarpos.Y > mPos.Y)
		{
			mYBL = mPos.Y;
			mYTR = tarpos.Y ;
		}
		else
		{
			mYTR = mPos.Y;
			mYBL = tarpos.Y ;
		}
	}


	if (point.X > mXBL &&
		point.Y > mYBL &&
		point.X < mXTR &&
		point.Y < mYTR) return TRUE ;

	return FALSE ;
}




