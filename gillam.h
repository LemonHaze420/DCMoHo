// Gillam.h: interface for the CGillam class.
//
//////////////////////////////////////////////////////////////////////

#ifndef GILLAM_H
#define GILLAM_H

#include "BadGuys.h"


DECLARE_THING_CLASS(CGillam, CBadGuy) 
public:
	virtual void		Init(CInitThing* i) ;
	virtual	SINT		GetInitialAttackID()	{return AT_GILLAM;};
	virtual	SINT		GetTorsoID() {return OID_CGillamTorso;};
	virtual void		CurrentActionAttackTarget() ;
	virtual void		CurrentActionGoToHoldingPoint() ;
	virtual void		CurrentActionAmbushing();
	virtual void        StateKnockedOut();
	virtual void		StateNormal()	;
	virtual	void		ProcessAnimation();
	virtual BOOL		IsGillam() { return TRUE ; }
	virtual BOOL		Attack(SINT no) ;
	virtual void		Defense() ;
	virtual GINT		GroundFriction(EFrictionType friction_type);
	virtual	BOOL		Strike(const CStrike &strike) ;
	virtual	void		Damage(GINT amount);
	virtual BOOL		CanIPickupWeapons() { return FALSE ; }
	virtual	GINT		Radius2() { return G(2,0);};
	virtual EAnimMode	GetNextAnimCharacter(EAnimMode am) ;
	virtual void		AnimTrigger(const GMatrix &current_anim_orientation,BOOL end_of_punch_hit);
	virtual	BOOL		DoIBurn() {return FALSE;}; 
	virtual BOOL		DoIGetLasered() { return FALSE ; }

#if TARGET == PC
	virtual void		DrawDebugText(HDC hdc, SINT* y_offset, char* buffer);
#endif

#ifdef LINK_EDITOR

	virtual void		DeclareSpawnees();
#endif

protected:
	SINT mChargesSinceLastHit;
	BOOL mPlayVictoryAtEndOfAttack;
	SINT mTimeAllowedToSlamTheGround;


} ;

#endif 
