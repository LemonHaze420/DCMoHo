#ifndef BAD_GUY_PRIMARY_INCLUDE
#define BAD_GUY_PRIMARY_INCLUDE

#include "BadGuys.h"
#include "KingHillBadGuy.h"
#include "Primary.h"

DECLARE_THING_CLASS(CBadGuyPrimary, CBadGuy)
public:
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER1;};
	virtual BOOL		Attack(SINT no) ;
			void		Init(CInitThing *i);
	virtual GINT		GroundFriction(EFrictionType friction_type);
	virtual BOOL		DoAnAttack() ;
	virtual BOOL		CounterAttack(BOOL facing_target); 
protected:
	SINT	mOddsOfDoingSpinAttack ;
};


DECLARE_THING_CLASS(CBadGuyLockDown, CBadGuyPrimary)
	virtual	void		Init(CInitThing *i){ mCombatProfile = LockdownCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryLockdownTorso;};
};

DECLARE_THING_CLASS(CBadGuyAngel, CBadGuyPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = AngelCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryAngelTorso;};
	virtual GINT		LinkRadius() {return GINT_HALF;};  // for link
};

DECLARE_THING_CLASS(CBadGuyApostle, CBadGuyPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = ApostleCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryApostleTorso;};
};


DECLARE_THING_CLASS(CBadGuyBenny, CBadGuyPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = BennyCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryBennyTorso;};
	virtual GINT		LinkRadius() {return G(0, 45000);};  // for link
	virtual void		Process() ;
};

DECLARE_THING_CLASS(CBadGuySophie, CBadGuyPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = SophieCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimarySophieTorso;};
};

DECLARE_THING_CLASS(CBadGuyDoctor, CBadGuyPrimary)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = DoctorCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CPrimaryDoctorTorso;};
	virtual BOOL		CanIPickupWeapons() {return FALSE;};
	virtual void		Process() ;
};



/*
DECLARE_THING_CLASS(CKingHillBadGuyPrimary, CKingHillBadGuy)
public:
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER1;};
	virtual BOOL		Attack(SINT no) ;
			void		Init(CInitThing *i);
	virtual GINT		GroundFriction(EFrictionType friction_type);
	virtual BOOL		DoAnAttack() ;
protected:
	SINT	mOddsOfDoingSpinAttack ;
};


DECLARE_THING_CLASS(CKingHillBadGuyLockDown, CKingHillBadGuyPrimary)
	virtual	SINT		GetTorsoID() {return OID_CPrimaryLockdownTorso;};
};

DECLARE_THING_CLASS(CKingHillBadGuyAngel, CKingHillBadGuyPrimary)
public:
	virtual	SINT		GetTorsoID() {return OID_CPrimaryAngelTorso;};
	virtual GINT		LinkRadius() {return GINT_HALF;};  // for link
};

DECLARE_THING_CLASS(CKingHillBadGuyApostle, CKingHillBadGuyPrimary)
public:
	virtual	SINT		GetTorsoID() {return OID_CPrimaryApostleTorso;};
};


DECLARE_THING_CLASS(CKingHillBadGuyBenny, CKingHillBadGuyPrimary)
public:
	virtual	SINT		GetTorsoID() {return OID_CPrimaryBennyTorso;};
	virtual GINT		LinkRadius() {return G(0, 45000);};  // for link
};

DECLARE_THING_CLASS(CKingHillBadGuySophie, CKingHillBadGuyPrimary)
public:
	virtual	SINT		GetTorsoID() {return OID_CPrimarySophieTorso;};
};

DECLARE_THING_CLASS(CKingHillBadGuyDoctor, CKingHillBadGuyPrimary)
public:
	virtual	SINT		GetTorsoID() {return OID_CPrimaryDoctorTorso;};
	virtual BOOL		CanIPickupWeapons() {return FALSE;};
};

  */


#endif

