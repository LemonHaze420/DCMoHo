#ifndef DROIDBALLDISPENSER_INCLUDE
#define DROIDBALLDISPENSER_INCLUDE


#include "Actor.h"

DECLARE_THING_CLASS(CBallDispenser, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual ECollideShape		GetCollideShape()		{return CSH_OBB;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};

	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual	BOOL				DoICollideWithMap() {return FALSE;};
	virtual	GINT		RadiusForMapWho() {return Radius()*4;};
		DECLARE_ZERO_GRAVITY;
	virtual	GINT				Radius() { return GetRenderThingRadius();};
	

	virtual	EAnimMode	GetRenderAnimation(){return mAnimMode;};
			void		StartPrematureDispense() ;
			BOOL		SetAnimMode(EAnimMode am, BOOL reset_frame = TRUE);
	virtual	GINT		GetRenderFrame()				{return mFrame;};
	virtual SINT		GetRealAnimIndex()				{return mRealAnimIndex;};
			void		ProcessAnimation() ;
			void		AnimTrigger() ;
	virtual	void		Dispense() = 0 ;

	#if TARGET == PC
	virtual	void				WriteData(CInitThing &i);
	#endif

	TED_COUNT0("Depense Delay")

	void						StartNextBallTimer();

protected:

	SINT	mNextTurnToDepense ;
	SINT	mCounter0;  // represents a delay between the last time the ball got picked up and when
						// the next ball is dispensed.
	EAnimMode			mAnimMode;
	GINT				mFrame;
	GINT				mFrameInc;
	SINT				mRealAnimIndex;

};



DECLARE_THING_CLASS(CDroidBallDispenser, CBallDispenser)
	virtual	void		Dispense() ;
	virtual	BOOL				IsPersistent() {return TRUE;};
			void				DeclareSpawnees() ;
};

/*
DECLARE_THING_CLASS(CGrenadeDispenser, CBallDispenser)
	virtual	void		Dispense() ;
	virtual	BOOL		IsPersistent() {return TRUE;};
			void				DeclareSpawnees() ;

};
*/


#endif 




