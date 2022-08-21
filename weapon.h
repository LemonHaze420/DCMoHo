#ifndef Weapon_H
#define Weapon_H

#include "HoldableThing.h"


DECLARE_THING_CLASS(CWeapon, CHoldableThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual void		Shutdown() ;
	static	SINT		GetNumWeaponsNotBeingHeld() { return mNumWeaponsNotBeingHeld ; }
	virtual	CWeapon		*IsWeapon() {return this;};
	virtual	BOOL		Pickup(CCharacter *by);
	virtual	void		Drop(HTDropReason = CHARACTER_WAS_HIT );
	virtual	void		Process() ;
	virtual	SINT		InitalNumberHits() { return 40000 ; }
			void		StartDying() {mFlashCount++ ; }
			SINT		GetHitsLeft() { return mNumHitsLeft; }
			void		DecrementHitsLeft() { mNumHitsLeft -- ; if (mNumHitsLeft==1) mFlags |= TF_SLOW_FLASHING ; }
	virtual	SINT		WhichRTsToRender();
	
	virtual	SINT		GetSample() {return SOUND_PICKUP3;};
protected:

	GMatrix		mOriginalOrientation;
	static SINT	mNumWeaponsNotBeingHeld ;
	SINT	mNumHitsLeft; 
	SINT	mFlashCount;

};



DECLARE_THING_CLASS(CWeaponAngelSword, CWeapon) 
	virtual	SINT	InitalNumberHits() { return 25; }
	virtual	SINT		GetSample() {return SOUND_PICKUPSW;};
};

DECLARE_THING_CLASS(CWeaponApostleSword, CWeapon)
	virtual	SINT	InitalNumberHits() { return 25; }
	virtual	SINT		GetSample() {return SOUND_PICKUPSW;};
};
DECLARE_THING_CLASS(CWeaponClub, CWeapon) 
	virtual	SINT	InitalNumberHits() { return 12; }
};
DECLARE_THING_CLASS(CWeaponCowProd, CWeapon)
 	virtual	SINT	InitalNumberHits() { return 20; }
};
DECLARE_THING_CLASS(CWeaponDagger, CWeapon) 
	virtual	SINT	InitalNumberHits() { return 10; }
	virtual	SINT		GetSample() {return SOUND_PICKUPSW;};
};
DECLARE_THING_CLASS(CWeaponHammer, CWeapon) 
	virtual	SINT	InitalNumberHits() { return 15; }
};


#endif


