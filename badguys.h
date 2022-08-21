#ifndef	BAD_GUYS_H
#define	BAD_GUYS_H

#include	"CharBall.h"
#include	"CharNoBall.h"
#include	"TerrainGuide.h"
#include	"Weapon.h"
class CHoldingZonePoint;
class CHealthTile;
enum eBadGuyAction
{
	GO_TO_HOLDING_POINT,
	ATTACK_TARGET,
	RETREAT_TO_HOLDING_POINT,
	AMBUSH,
	WAIT_FOR_A_TARGET,
	RUN_AWAY,
	GO_FOR_A_WEAPON,
	GO_FOR_THE_KING_OF_THE_HILL,
	GO_TO_HOLDING_ZONE,
	GO_TO_HEALTH_TILE
};

#define CLOSE_ENOUGH_TO_HIT G(3)

DECLARE_THING_CLASS(CBadGuy, CCharBall)
public:

	virtual void		Process();

	virtual	void		StateNormal();
	virtual void		StateFinished() ;
	virtual	void		StateAsleep();
	virtual void		StateStunned() ;
	virtual	void		StateScanning();
	virtual	void		Init(CInitThing *i);
	virtual BOOL		Strike(const CStrike& strike) ;
	virtual	void		ClearYourTarget() ;
	virtual	void		AnimTrigger(const GMatrix& current_anim_orientation, BOOL end_of_punch_hit);
	virtual BOOL		Attack(SINT no) ;
	virtual BOOL		CounterAttack(BOOL facing_target) ;
	virtual	BOOL		IsBadGuy() { return TRUE ; }
	virtual	void		ResetCurrentAction() ;
	virtual	GINT		Radius2() { return G(1,0);};
	virtual BOOL		DoAnAttack() ;
	virtual void		Hit(CThing *that) ;
	void		ChangeCurrentAction(eBadGuyAction new_action);

			BOOL		PickupAnyWeapon(GINT search_dist);
			CWeapon*	IsAWeaponLyingAroundNearBy(GINT search_dist);
			BOOL		GoToHealthTile(GINT search_dist);
			CHealthTile*IsHealthTileLyingAroundNearBy(GINT search_dist);

	virtual void		ValidateData() { VALIDATE(mWeaponWeWantToGoAndPickUp) ; SUPERTYPE::ValidateData();};
			BOOL		UsesNonProximitryHoldingZones() { return mCounter2 > 0 ; };
//	virtual	SINT		GetCharacterThreatScore() { return mCounter0 + (mWeapon==NULL ? 0 : 5) ; } ;

	virtual	void		StateKnockedOut();
	virtual void		ChooseTarget(); 
			eBadGuyAction		GetCurrentAction() { return mCurrentAction ; } ;
			BOOL		IsCurrentActionAttackingTarget() { if (mCurrentAction==ATTACK_TARGET) return TRUE; else return FALSE;}

	#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
	virtual	void		DrawDebugText(HDC hdc, SINT *y_offset, char* buffer) ;
	#endif

	TED_COUNT0("Skill Level")
	TED_COUNT1("To Attack BF")
	TED_COUNT2("Hold Zone Num")
	TED_BOOL0("Not Ambush?")
	TED_BOOL1("Holding zones")
	TED_BOOL2("Check Terrain") 

			void		SetHoldingPoint(SINT min, SINT max);
			void		SetRunAwayPoint();
			void		SetAttackingPoint();
			void		CurrentActionGoToRetreatingPoint() ;
	virtual void		CurrentActionGoToHoldingPoint() ;
	virtual	void		CurrentActionAttackTarget() ;
			void		CurrentActionGoingForAWeapon() ;
			void		CurrentActionRunningAway() ;
			void		CurrentActionGoToHoldingZone() ;
	virtual void		CurrentActionAmbushing();
			void		CurrentActionGoingToHealthTile() ;
			void		StartAttackingTarget();
			void		StopAttackingTarget();
			void		FaceTarget();
			void		FaceGoToPoint();
	virtual void		Defense();
	
protected:

	// the point that the bad guy wants to go to in normal state
	CTerrainGuide mTerrainGuide;
	eBadGuyAction mCurrentAction;
	eBadGuyAction mDefaultObjective;

	CHoldingZonePoint* mHoldingZonePoint;
	SINT mHoldingPointDelay ;

	SINT mCounter0; // Represents skill level
	SINT mCounter1; // Represents who to attack bf
	SINT mCounter2; // Represents holzing zone number
	BOOL	mBool0; // Represents whether they can ambush or not
	BOOL	mBool1; // Represents whether they stick to there holding zones
	BOOL	mBool2; // Represents whether the AI should do Terrain checking when moving ( i.e look out for drops etc )

	// Number of cycles between each time the badguy punches 
	SINT mPunchTime;
	SINT mDefenseCheckTimer ;
	GINT mAccelSpeed;
	GINT mScanDistForAWeapon;
	CWeapon* mWeaponWeWantToGoAndPickUp ;
	CHealthTile* mHealthTileWeWantToGoTo;

	GVector mTargetPosWhenSettingHoldingPoint ;
	BOOL mStopAttackingWhenPunchIsOver ;

	SINT mMinHoldingPointDist ;
	SINT mMaxHoldingPointDist;
	GINT mBadGuyProximityRange ;

};

extern CCharacterCombatProfile PrisonerCombatProfile ;
extern CCharacterCombatProfile GuardCombatProfile ;
extern CCharacterCombatProfile SGuardCombatProfile ;
extern CCharacterCombatProfile DeputyCombatProfile ;
extern CCharacterCombatProfile MarshallCombatProfile ;


DECLARE_THING_CLASS(CGuardA, CBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = GuardCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_GUARD;};
	virtual	SINT		GetTorsoID() {return OID_CGuardATorso;};
	virtual	BOOL		DoIBurn() {return FALSE;}; 
};

DECLARE_THING_CLASS(CGuardB, CBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = GuardCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_GUARD;};
	virtual	SINT		GetTorsoID() {return OID_CGuardBTorso;};
};

DECLARE_THING_CLASS(CGuardC, CBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = GuardCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_GUARD;};
	virtual	SINT		GetTorsoID() {return OID_CGuardCTorso;};
	virtual BOOL		DoIGetLasered() { return FALSE ; }
};

DECLARE_THING_CLASS(CSGuardA, CBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = SGuardCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_SGUARD;};
	virtual	SINT		GetTorsoID() {return OID_CSGuardATorso;};
	virtual	BOOL		DoIBurn() {return FALSE;}; 
};

DECLARE_THING_CLASS(CSGuardB, CBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = SGuardCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_SGUARD;};
	virtual	SINT		GetTorsoID() {return OID_CSGuardBTorso;};
};

DECLARE_THING_CLASS(CSGuardC, CBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = SGuardCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_SGUARD;};
	virtual	SINT		GetTorsoID() {return OID_CSGuardCTorso;};
	virtual BOOL		DoIGetLasered() { return FALSE ; }
};

DECLARE_THING_CLASS(CMarshall, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = MarshallCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_MARSHALL;};
	virtual	SINT		GetTorsoID() {return OID_CMarshallTorso;};
};


DECLARE_THING_CLASS(CDeputy, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = DeputyCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_DEPUTY;};
	virtual	SINT		GetTorsoID() {return OID_CDeputyTorso;};
};

DECLARE_THING_CLASS(CPrisoner1A, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER1;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner1ATorso;};
	virtual BOOL		CanIPickupWeapons() { return FALSE ; }
};

DECLARE_THING_CLASS(CPrisoner1B, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER1;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner1BTorso;};
	virtual BOOL		CanIPickupWeapons() { return FALSE ; }
};

DECLARE_THING_CLASS(CPrisoner1C, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER1;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner1CTorso;};
	virtual BOOL		CanIPickupWeapons() { return FALSE ; }
};

DECLARE_THING_CLASS(CPrisoner2A, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER2;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner2ATorso;};
};

DECLARE_THING_CLASS(CPrisoner2B, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER2;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner2BTorso;};
};

DECLARE_THING_CLASS(CPrisoner2C, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER2;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner2CTorso;};
};

DECLARE_THING_CLASS(CPrisoner3A, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER2;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner3ATorso;};
};

DECLARE_THING_CLASS(CPrisoner3B, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER2;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner3BTorso;};
};

DECLARE_THING_CLASS(CPrisoner3C, CBadGuy) 
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER2;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner3CTorso;};
};


#endif