#ifndef	Racer_H
#define	Racer_H

#include	"CharBall.h"

DECLARE_THING_CLASS(CRacer, CCharBall)
public:
	virtual	void		Init(CInitThing *i);
//	virtual	void		Process();
	virtual	void		StateNormal();
	virtual void		StateFinished() ;
	virtual void		Process() ;
			void		RacerBoost() ;

	virtual ECollideShape		GetCollideShape()			{return CSH_SPHERE;};
	virtual	ECollideResponse	GetCollideResponse()		{return CR_BOUNCE;};
	virtual	BOOL		IsRacer() { return TRUE ;} 
	virtual	BOOL				IsPersistent() {return TRUE;};
			void		CalcNewGoalPoint(SINT for_check_point);
	virtual	GINT		Radius2() { return G(1,0);};

	virtual	void		ReachedCheckPoint(SINT check_point_num) ;

//	virtual	GINT				Radius() { return GetRenderThingRadius();};
//	virtual	GINT				Radius() { return G(1, 32000);};

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	TED_COUNT0("Skill Level")
	TED_COUNT1("Hit Freq") 

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};

	// racing stuff


protected:
	CThing	*mTarget;
	GVector mGoalPoint;
	BOOL	mSetGoalPoint;
	GINT	mRacingSpeed ;
	SINT	mCounter0;  // represents skill level 
	SINT	mCounter1; // Represents hit frequency
	GINT	mYaw; //, mPitch, mRoll;
	GVector mPos_b4 ;
	SINT	mStartRevingTime;
	SINT	mTimeTryingToGetToNextCheckPoint ;
	SINT	mNumJumpsToNextCheckpoint;
	BOOL	mBackTrackingToLastCheckpoint;
};

extern CCharacterCombatProfile GuardCombatProfile ;
extern CCharacterCombatProfile SGuardCombatProfile ;

DECLARE_THING_CLASS(CRacerTank, CRacer)
	virtual	void		Init(CInitThing *i){ mCombatProfile = SGuardCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CSGuardATorso;};


};

DECLARE_THING_CLASS(CRacerGhost, CRacer)
	virtual	void		Init(CInitThing *i){ mCombatProfile = GuardCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetTorsoID() {return OID_CGuardCTorso;};


};

#endif