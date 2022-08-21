#include	"Common.h"

#include	"Globals.h"

#include "KingHillGoalPoint.h"
#include "Character.h"


//******************************************************************************************
void		CKingHillGoalPoint::Init(CInitThing *i) 
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	GET_TARGET;
	SitOnGround();
	mXBL = mYBL = mXTR = mYTR = G(0);

};



//******************************************************************************************
void		CKingHillGoalPoint::Shutdown()
{
	SUPERTYPE::Shutdown() ;
}


//******************************************************************************************
#if TARGET == PC
void	CKingHillGoalPoint::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
}	
#endif


//******************************************************************************************
BOOL		CKingHillGoalPoint::IsPointInsideKingHillArea(GVector& point)
{
	if (!mTarget) return FALSE ;

	if (point.X > mXBL &&
		point.Y > mYBL &&
		point.X < mXTR &&
		point.Y < mYTR) return TRUE ;
	return FALSE ;
}


//******************************************************************************************
GVector		CKingHillGoalPoint::GetCentrePoint()
{
	if (!mTarget) return mPos;

//	return GVector( ((mXTR - mXBL)/2)+mXBL, ((mYTR - mYBL)/2)+mYBL, mPos.Z) ;

	// JCL - fix for internal compiler error.
	GINT	x = ((mXTR - mXBL)/2)+mXBL;
	GINT	y = ((mYTR - mYBL)/2)+mYBL;
	
	return GVector(x, y, mPos.Z) ;
}

//******************************************************************************************
void		CKingHillGoalPoint::Process()
{
	if (!mTarget) return ;

	// calc axis alligned box ( only done first time process called )
	// from this King Hill point to it's target

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

	CharactersInWorldList& items = WORLD.GetCharacterNB() ;
	for (CCharacter* current = items.First() ;
					 current != NULL;
					 current = items.Next() )
	{
		// is character inside axis alligned enclosing box around vector (TarPos - mPos)  
						 
		GVector cPos = current->GetPos() ;

		if (current->GetState() != CS_DEAD && IsPointInsideKingHillArea(cPos) == TRUE)
		{
			current->GetLevelData().InKingHillGoalPoint(this) ;
		}
	}	
}




