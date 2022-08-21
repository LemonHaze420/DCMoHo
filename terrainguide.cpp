// CTerrainGuide.cpp: implementation of the CTerrainGuide class.
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "TerrainGuide.h"
#include "globals.h"
#include "Character.h"
#include "BadGuys.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerrainGuide::CTerrainGuide()
{
	mGoalPoint = GVector(G0,G0,G0);
	mPos_b4 = GVector(G0,G0,G0) ;
}

CTerrainGuide::~CTerrainGuide()
{

}

//******************************************************************************************
BOOL	CTerrainGuide::IsTerrainInDirectionSafe(GINT angle)
{

	#define NUM_CHECKS 3

	GVector pos = mForCharacter->GetPos() ;

	GINT current_height = pos.Z +mForCharacter->Radius();

	// get height from where it is going half a cell away
	GVector my_dir = GVector(GSIN(angle), GCOS(angle), G0) ;
	my_dir *= (mForCharacter->GetVelocity().Magnitude() *20);

	if (my_dir.MagnitudeSq() < G(16))
	{ 
		my_dir.Normalise() ;
		my_dir*=4 ;
	}

	GVector step = my_dir / NUM_CHECKS ;

	GVector to_check = pos+step ;

	for (SINT i = 0 ;i< NUM_CHECKS;i++)
	{

		if (!COORDS_INVALID(CCellRef(to_check) ))
		{
			GINT future_height = MAP.GetMapHeight(to_check) ;

			GINT height_diff = (future_height - current_height) ;
			if (height_diff > G(1,32768) ) return FALSE ;
		//	if (height_diff > G(0,400) ) return FALSE ;
	
		}
	
	
		to_check+=step ;
	}

	return TRUE ;
	
}
	
//******************************************************************************************
void CTerrainGuide::MoveMe(GINT CurrentAcell, BOOL MustTouchGoalPoint, BOOL CheckTerrain) 
{
	static GINT degs_5 = G(5) * ( G_PI / G(180) ) ;

	GVector d = ( mGoalPoint - mForCharacter->GetPos());  // scale to prevent possible overflows.

	GINT	dist = d.Magnitude() ;
	if ((dist > G(0,10)) && (dist < G(60)))
	{
		d /= dist;
	}
	else
	{
		d = ZERO_GVECTOR;
	}
 

	BOOL will_fall_off = FALSE;
	GVector vel = mForCharacter->GetVelocity() ;
	vel.Normalise() ;
	GINT vel_ang = GATAN2(vel.X, vel.Y) ;
	if ( CheckTerrain && IsTerrainInDirectionSafe(vel_ang) == FALSE )
	{
		if (GAME.GetIVar(IV_LEVEL_TYPE)==LT_KING)
		{
			mForCharacter->SetVelocity(mForCharacter->GetVelocity()*G(0,50000)) ;
		}
		will_fall_off=TRUE;
		d=-vel ;
	}

	GINT current_angle = GATAN2(d.X, d.Y) ;

	if ((!will_fall_off) && dist < G(2) && !MustTouchGoalPoint  ) return ;

	GINT	force = G1;

	SINT found_safe_path =FALSE;
	SINT attempts = 0;
	BOOL had_to_change_dir = FALSE;

	while (!found_safe_path && attempts < 3 && CheckTerrain)
	{
	
		if (IsTerrainInDirectionSafe(current_angle+degs_5) == FALSE ||
			IsTerrainInDirectionSafe(current_angle-degs_5) == FALSE)
		{	
			if (GAME.GetIVar(IV_LEVEL_TYPE)==LT_KING && (!will_fall_off) && attempts ==0)
			{
				mForCharacter->SetVelocity(mForCharacter->GetVelocity()*G(0,50000)) ;
			}
		
		//	CONSOLE.AddString("Shit!") ;
			current_angle+=(degs_5 *20) ;	
			had_to_change_dir = TRUE ;
		}
		else
		{
			found_safe_path = TRUE ;
		}
		attempts++ ;
	}

	if (attempts>=3 && !found_safe_path)
	{
		// couldn't find a way out just go with the flow and fall !!

	//	CONSOLE.AddString("arse no way out!!!!") ;

		return ;
	}


	if (had_to_change_dir)
	{
		d = GVector(GSIN(current_angle), GCOS(current_angle), G0) ;
	}
	mForCharacter->Thrust(d * CurrentAcell * force ) ; 
	

//	((CBadGuy*)mForCharacter)->GetCurrentAction() != ATTACK_TARGET &&
	// incase we get stuck jump ??
	if (
		GAME.GetGameTurn()%25 ==0 && 
		dist > G(3)  )
	{
		if (mPos_b4!= GVector(G0,G0,G0) )
		{
			if ((mForCharacter->GetPos() - mPos_b4).MagnitudeSq() < G(0,634) )
			{
				mForCharacter->Thrust(d * CurrentAcell * force *8 ) ; 
				mForCharacter->Jump() ;
				
			}
		}
		mPos_b4 = mForCharacter->GetPos();
	}

}



// although we don't want to go anywhere we may have some velocity caused by something
// so make sure the direction we are heading is safe else do something about it

//******************************************************************************************
void CTerrainGuide::ChewTerrainCheck(GINT CurrentAcell, BOOL CheckTerrain) 
{
	if (mForCharacter->GetVelocity().MagnitudeSq() < G(0,100) )
	{
		return ;
	}
	static GINT degs_5 = G(5) * ( G_PI / G(180) ) ;
	GVector vel = mForCharacter->GetVelocity() ;
	vel.Normalise() ;
	GINT vel_ang = GATAN2(vel.X, vel.Y) ;
	if ( (!CheckTerrain) || IsTerrainInDirectionSafe(vel_ang) == TRUE )
	{
		return ;
	}

	if (GAME.GetIVar(IV_LEVEL_TYPE)==LT_KING)
	{
		mForCharacter->SetVelocity(mForCharacter->GetVelocity()*G(0,60000)) ;
	}
	GINT current_angle = GATAN2(-vel.X, -vel.Y) ;

	GINT	force = G1;

	SINT found_safe_path =FALSE;
	SINT attempts = 0;
	BOOL had_to_change_dir = FALSE;

	while (!found_safe_path && attempts < 3 && CheckTerrain)
	{
	
		if (IsTerrainInDirectionSafe(current_angle+degs_5) == FALSE ||
			IsTerrainInDirectionSafe(current_angle-degs_5) == FALSE)
		{	
		//	CONSOLE.AddString("Shit!") ;
			current_angle+=(degs_5 *20) ;	
			had_to_change_dir = TRUE ;
		}
		else
		{
			found_safe_path = TRUE ;
		}
		attempts++ ;
	}

	if (attempts>=3 && !found_safe_path)
	{
		return ;
	}


	if (had_to_change_dir)
	{
		GVector to_thrust_dir = GVector(GSIN(current_angle), GCOS(current_angle), G0) ;
		mForCharacter->Thrust(to_thrust_dir * CurrentAcell * force ) ; 
	}
	else
	{
		mForCharacter->Thrust(-vel * CurrentAcell * force ) ; 
	}
}
