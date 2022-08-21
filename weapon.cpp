#include	"Common.h"

#include	"globals.h"
#include	"Weapon.h"
#include	"Character.h"


SINT CWeapon::mNumWeaponsNotBeingHeld = 0 ;


//******************************************************************************************
void	CWeapon::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mNumWeaponsNotBeingHeld++ ;
	mOriginalOrientation = mOrientation ;
	mNumHitsLeft = InitalNumberHits() ;
	mFlashCount = 0 ;

	WORLD.GetWeaponsNB().Add(this) ;
}

//******************************************************************************************
void	CWeapon::Shutdown()
{
	SUPERTYPE::Shutdown();

	//!DOES THIS WORK AT END LEVEL TIME?
	if (mCarriedBy) Drop() ;

	mNumWeaponsNotBeingHeld-- ;

	WORLD.GetWeaponsNB().Remove(this) ;
}


//******************************************************************************************
BOOL	CWeapon::Pickup(CCharacter *by)
{
	BOOL res = SUPERTYPE::Pickup(by) ;

	if (res && mFlashCount == 0 )
	{
		mNumWeaponsNotBeingHeld--;
		SOUND.PlaySample(GetSample(), this);
		if (mNumHitsLeft ==1 && by->IsPrimary() ) mFlags |= TF_SLOW_FLASHING ; 
		return TRUE;
	}
	else
		return FALSE;
}


//******************************************************************************************
void	CWeapon::Process()
{
	SUPERTYPE::Process() ;
	if (mFlashCount > 0 )
	{
		mFlags |= TF_FLASHING ;
		mFlashCount++ ;
		if (mFlashCount == 80)
		{
			Die() ;
			return ;
		}
	}
}

//******************************************************************************************
SINT	CWeapon::WhichRTsToRender()
{
	if (mCarriedBy && ((mCarriedBy->WhichRTsToRender() & 1) == 0))
		return 0; // he's not visible;
		
	return SUPERTYPE::WhichRTsToRender();
}

//******************************************************************************************
void	CWeapon::Drop(HTDropReason reason)
{
	if (mCarriedBy != NULL)
	{
		mNumWeaponsNotBeingHeld++ ;
	}

	mFlags &= (~TF_SLOW_FLASHING) ;
	SUPERTYPE::Drop(reason);
}

