#include	"Common.h"

#include	"BadGuys.h"
#include	"Globals.h"
#include	"Weapon.h"
#include	"HoldingZonePoint.h"
#include	"HealthTile.h"

// the maximum height difference allowed between you and the weapon which would allow
// the bad guy to take an action to move to the weapon and pick it up 
#define MAX_HEIGHT_DIFF_TO_GET_WEAPON G(2)

#define	GIVE_UP_TRYING_TO_GET_WEAPON_TIME 200

#define MAX_HEIGHT_DIFF_TO_GET_TO_HEALTH_TILE G(2)

#define	GIVE_UP_TRYING_TO_GET_TO_HEALTH_TILE 200

// if a weapon is withing this fov of our current yaw then we can see it
#define WEAPON_SIGHT_FOV G_PI_4 

// if we are this close to the weapon then we say we can see it not matter if we ant facing it;)
#define MAX_DIST_SQ_WHEN_WE_CAN_SEE_WEAPON_NO_MATTER_WHAT_FOV G(16) 

//******************************************************************************************
void	CBadGuy::CurrentActionGoingForAWeapon()
{
	if (RANDOM()%2 == 1) mStateTimer ++;

	// check if weapon is still accessable
	if (mWeaponWeWantToGoAndPickUp)
	{
		GVector vec_to_weapon = mWeaponWeWantToGoAndPickUp->GetPos() - GetPos() ;
		GINT height_diff = ( GABS(vec_to_weapon.Z) ) ;

		if (height_diff > MAX_HEIGHT_DIFF_TO_GET_WEAPON)
		{
			ResetCurrentAction();
			return ;
		}
	}

	if (mIsBlocking == TRUE) StopBlocking() ;

	if (mWeaponWeWantToGoAndPickUp == NULL ||
		mWeaponWeWantToGoAndPickUp->IsBeingCarried() ||
		mStateTimer > GIVE_UP_TRYING_TO_GET_WEAPON_TIME)
	{
		ResetCurrentAction();
		return ;
	}

	mTerrainGuide.SetNewGoalPoint( mWeaponWeWantToGoAndPickUp->GetPos() );
	FaceGoToPoint() ;

	if (mFlags & TF_ON_GROUND && IsUpright())
	{
		// must touch goal point set to true cos we want to touch the goal
		// point to pick up the weapon 
		mTerrainGuide.MoveMe(mAccelSpeed,TRUE, mBool2) ;
	}
}


//******************************************************************************************
BOOL	CBadGuy::PickupAnyWeapon(GINT search_dist)
{
	// quick check to see if there is actualy any weapons left in the world which can
	// be picked up or if we already have a weapon
	if (CWeapon::GetNumWeaponsNotBeingHeld() <=0 || mHoldingThing || CanIPickupWeapons() == FALSE) return FALSE ;

	CWeapon* weapon = IsAWeaponLyingAroundNearBy(search_dist) ;
	if (weapon)
	{
		// can we see the weapon
		GVector vec_to_weapon = weapon->GetPos() - GetPos() ;
		GINT angle_weapon_to_you = - GATAN2(vec_to_weapon.X, vec_to_weapon.Y) ;
		GINT angle_diff = AngleDifference(mYaw, angle_weapon_to_you ) ;
		GINT height_diff = ( GABS(vec_to_weapon.Z) ) ;

		if (height_diff <= MAX_HEIGHT_DIFF_TO_GET_WEAPON && 
			(GABS(angle_diff) < WEAPON_SIGHT_FOV || vec_to_weapon.MagnitudeSq() < MAX_DIST_SQ_WHEN_WE_CAN_SEE_WEAPON_NO_MATTER_WHAT_FOV ) )
		{
			// go get it then !!!
			ChangeCurrentAction(GO_FOR_A_WEAPON) ;
			mWeaponWeWantToGoAndPickUp = weapon ;
			mStateTimer = 0 ;
			return TRUE ;
		}
	}
	return FALSE ;
}

//******************************************************************************************

CWeapon* CBadGuy::IsAWeaponLyingAroundNearBy(GINT search_dist)
{

	const WeaponList& items = WORLD.GetWeaponsNB();
	GINT nearest_weapon = G(120*120) * 2 ;
	GVector my_pos = GetPos() ;
	CWeapon	*current = NULL;

	// loop through all weapons in world

	CWeapon *nt;
	for (nt = items.First();
		 nt != NULL ;
		 nt = items.Next() )
	{
		// is the weapon not being carried by anyone ?
		if (!nt->IsBeingCarried() && nt->GetHitsLeft() > 0) 
		{
			GINT dist = (nt->GetPos() - my_pos).MagnitudeSq()  ;
			if (dist < nearest_weapon)
			{
				nearest_weapon = dist ;
				current = nt ;
			}
		}
	}

	if ((nearest_weapon < (search_dist*search_dist)) && (current != NULL)) return current ;
	return NULL ;
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CBadGuy::CurrentActionGoingToHealthTile()
{

	if (RANDOM()%2 == 1) mStateTimer ++;

	if (mIsBlocking == TRUE) StopBlocking() ;

	if (mLife == InitialLife() || mStateTimer >= GIVE_UP_TRYING_TO_GET_TO_HEALTH_TILE)
	{
		ResetCurrentAction();
		return ;
	}

	FaceGoToPoint() ;
	mTerrainGuide.SetNewGoalPoint( mHealthTileWeWantToGoTo->GetPos() );

	if (mFlags & TF_ON_GROUND && IsUpright())
	{
		mTerrainGuide.MoveMe(mAccelSpeed,FALSE, mBool2) ;
	}
}


//******************************************************************************************
BOOL	CBadGuy::GoToHealthTile(GINT search_dist)
{
	CHealthTile* health_tile = IsHealthTileLyingAroundNearBy(search_dist) ;
	if (health_tile)
	{
		// can we see the weapon
		GVector vec_to_health_tile = health_tile->GetPos() - GetPos() ;
		GINT height_diff = ( GABS(vec_to_health_tile.Z) ) ;

		if (height_diff <= MAX_HEIGHT_DIFF_TO_GET_TO_HEALTH_TILE  )
		{
			// go get it then !!!
			ChangeCurrentAction(GO_TO_HEALTH_TILE) ;
			mHealthTileWeWantToGoTo = health_tile ;
			mStateTimer = 0 ;
			return TRUE ;
		}
	}
	return FALSE ;
}


//******************************************************************************************
CHealthTile* CBadGuy::IsHealthTileLyingAroundNearBy(GINT search_dist)
{

	const HealthTileInWorldList& items = WORLD.GetHealthTileNB();
	GINT nearest_health_tile = G(120*120) * 2 ;
	GVector my_pos = GetPos() ;
	CHealthTile	*current = NULL;

	// loop through all weapons in world

	CHealthTile *nt;
	for (nt = items.First();
		 nt != NULL ;
		 nt = items.Next() )
	{
		GINT dist = (nt->GetPos() - my_pos).MagnitudeSq()  ;
		if (dist < nearest_health_tile)
		{
			nearest_health_tile = dist ;
			current = nt ;
		}
	}

	if ((nearest_health_tile < (search_dist*search_dist)) && (current != NULL)) return current ;
	return NULL ;
}