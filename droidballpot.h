#ifndef DROIDBALLPOT_INCLUDE
#define DROIDBALLPOT_INCLUDE

#include "Thing.h"

#define BALL_POT_POLE_CHECK_RADIUS G(1,32345) 
#define BALL_POT_POLE_ACTUAL_RADIUS G(0,18345) 


DECLARE_THING_CLASS(CDroidBallPot, CThing)
public:

	virtual	void		Init(CInitThing *i);
	virtual	BOOL				DoICollideWithMap() {return FALSE;};
	virtual ECollideShape		GetCollideShape()		{return CSH_OBB3;};
	virtual	ECollideResponse	GetCollideResponse()		{return CR_STATIC;};
	virtual void		Shutdown() ;

	virtual void	Process() ;
	static	BOOL	CloseToDroidBallPot(CCharacter* for_character, GINT* return_angle) ;

	#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
	#endif

	DECLARE_ZERO_GRAVITY;
	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	GINT				Radius() { return GetRenderThingRadius();};

	TED_COUNT0("Mag rad x100")

protected:

	SINT	mCounter0;
	GINT	mMagneticRadius ;


};

#endif 