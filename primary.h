#ifndef PRIMARY_H
#define PRIMARY_H

#include	"CharBall.h"

class	CPlayer;
class	CStart;

DECLARE_THING_CLASS(CPrimary, CCharBall)
public:

	virtual	void		Init(CInitThing *i);
	virtual	CPrimary	*IsPrimary() {return this;};

	virtual	void		Process();

	virtual void		TriggerVibration(SINT number)
	{
		if((mPlayer))
			CONTROLS.mControllers[mPlayer->mNumber].TriggerVibration(number);
	}
	virtual	BOOL		DoIAcceptPickups()	{return TRUE;};
	virtual	void		ApplyPickup(EPickupType pt, GINT amount);
	virtual	BOOL		Attack(SINT no) ;
	virtual	GINT		GroundFriction(EFrictionType friction_type);
	virtual void		Heal(SINT amount) { SUPERTYPE::Heal(amount) ;
							if ((mBrakePower+G(0,amount<<9) ) <= G(1) ) mBrakePower+=G(0,amount<<9) ; else mBrakePower = G1 ; }



			CPlayer*	GetPlayer() { return mPlayer ;}
	virtual void		FinishedLevel() ;
			void		RacerBoost(GVector d) ;
			void		SpawnedBy(CStart* start) {mSpawnedBy = start ; } 
			CStart*     GetSpawnedBy() { return mSpawnedBy ; } 

	virtual	void		HitArena();

//	virtual	GINT		Radius() { return GINT_HALF;};

//	virtual	SINT		GetRenderThingNumber() {return (mState == CS_TELEPORT) ? 1 : 0;};

	virtual	void		StartDieProcess(BOOL hit_fan=FALSE);

	virtual	GINT		GetYaw() {return mYaw;};
	virtual	BOOL		Strike(const CStrike& strike);


			GINT		GetBrakePower() {return mBrakePower;};

	virtual	void		Jump();
	virtual void		Thrust(const GVector& v);


			void		SubBrakePower(GINT v) {mBrakePower -= v;};

	virtual	SINT		GetJumpSample() {return SOUND_MHUP2;};

protected:

	GINT				mBrakePower;  // range 0 - 1

	CPlayer				*mPlayer;
	SINT mFuckedUpAtStartOfRaceCount ;
	CStart				*mSpawnedBy;
	BOOL				mDoRacerRageBoost;

};

extern CCharacterCombatProfile AngelCombatProfile ;
extern CCharacterCombatProfile ApostleCombatProfile ;
extern CCharacterCombatProfile LockdownCombatProfile ;
extern CCharacterCombatProfile BennyCombatProfile ;
extern CCharacterCombatProfile SophieCombatProfile ;
extern CCharacterCombatProfile DoctorCombatProfile ;
extern CCharacterCombatProfile PrimaryBallCombatProfile ;



DECLARE_THING_CLASS(CPrimaryAngel, CPrimary)
public:
	virtual	void		Init(CInitThing *i) { mCombatProfile = AngelCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryAngelTorso;};
	virtual GINT		LinkRadius() {return G(0, 27000);};  // for link
	virtual	SINT		GetJumpSample() {return SOUND_FHUP;};
};

DECLARE_THING_CLASS(CPrimaryApostle, CPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = ApostleCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryApostleTorso;};
};

DECLARE_THING_CLASS(CPrimaryLockdown, CPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = LockdownCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryLockdownTorso;};
	virtual	GINT	Radius2() { return G(1);};
	virtual GINT		SizeofMeFactor() {return G(1,16384) ; }
};

DECLARE_THING_CLASS(CPrimaryBenny, CPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = BennyCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryBennyTorso;};
	virtual GINT		LinkRadius() {return G(0, 45000);};  // for link
	virtual void		Process() ;
	virtual	GINT		Radius2() { return G(1);};
	virtual GINT		SizeofMeFactor() {return G(1,16384) ; }
};

DECLARE_THING_CLASS(CPrimarySophie, CPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = SophieCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimarySophieTorso;};
	virtual	SINT		GetJumpSample() {return SOUND_FHUP;};
};

DECLARE_THING_CLASS(CPrimaryDoctor, CPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = DoctorCombatProfile; SUPERTYPE::Init(i) ; mCanBlockBF |= SI_WEAPON ;}
	virtual	SINT		GetTorsoID() {return OID_CPrimaryDoctorTorso;};
	virtual BOOL		CanIPickupWeapons() {return FALSE;};
	virtual void		Process() ;
};

// er...
DECLARE_THING_CLASS(CPrimaryBall, CPrimary)
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrimaryBallCombatProfile; SUPERTYPE::Init(i) ; }
	virtual BOOL			RenderRenderPart(SINT pn)	{return pn == 0;};

	virtual ECollideShape		GetCollideShape()	{return mInvisible ? CSH_NONE : CSH_SPHERE;};
};


//******************************************************************************************
//******************************************************************************************

#endif
