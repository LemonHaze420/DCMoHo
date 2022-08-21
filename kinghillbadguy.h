// KingHillBadGuy.h: interface for the CKingHillBadGuy class.
//
//////////////////////////////////////////////////////////////////////

#ifndef KING_HILL_BAD_GUY_H
#define KING_HILL_BAD_GUY_H

#include "BadGuys.h"

enum eKingHillStatus
{
	TRING_TO_GET_TO_BOTTOM_OF_HILL,
	CLIMBING_HILL,
	REACHED_PLATO
} ;


class CWayPoint ;

DECLARE_THING_CLASS(CKingHillBadGuy, CBadGuy) 
public:

				void			Init(CInitThing *i) ;
		virtual	void			StateNormal();
				void			CurrentActionGoingToKingHillPoint() ;
				void			FindClosestPathFinderPoint();
				void			MoveMeUpTheHill() ;
				void			MoveMeToTheBottomOfTheHill() ;
		virtual	void			ReachedCheckPoint(SINT check_point_num) ;
		virtual void	    	StateFinished() ;
				void			CalcNewCheckPoint(SINT for_check_point);
				void			FaceCheckPointPos() ;
				BOOL			CheckIfFallenOffHill();
				void			UpdateProtectingHill();
				BOOL			CheckIfWeCanStartClimbingHillNow() ;
				void			StartProtectingHill();
		virtual void			ChooseTarget(); 
		virtual void			ResetCurrentAction() ;
		virtual	void			Hit(CThing *that);
		eKingHillStatus			GetKingHillStatus() { return mKingHillStatus; }

#if TARGET == PC
			virtual	void		DrawDebugText(HDC hdc, SINT *y_offset, char* buffer) ;
#endif

protected:


	eKingHillStatus mKingHillStatus;
	CPathFinderPoint* mNextPathFinderPoint;
	CCheckPoint* mNextCheckPoint;
	CCheckPoint* mCurrentCheckPoint;
	GVector mNextMidCheckPointPos;
	BOOL mGoingToFirstPathFinderPoint;
	SINT	mStateTimerKH;
} ; 





DECLARE_THING_CLASS(CPrisoner1AKingHill, CKingHillBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER1;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner1ATorso;};
	virtual BOOL		CanIPickupWeapons() { return FALSE ; }
};


DECLARE_THING_CLASS(CPrisoner2AKingHill, CKingHillBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	BOOL		DoIBurn() {return FALSE;}; 
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER2;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner2CTorso;};
};

DECLARE_THING_CLASS(CPrisoner3AKingHill, CKingHillBadGuy)
public:
	virtual	void		Init(CInitThing *i){ mCombatProfile = PrisonerCombatProfile; SUPERTYPE::Init(i) ; }
	virtual	SINT		GetInitialAttackID()	{return AT_PRISONER3;};
	virtual	SINT		GetTorsoID() {return OID_CPrisoner2BTorso;};
};

#endif 


