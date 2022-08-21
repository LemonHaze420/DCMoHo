#ifndef CROWN_H
#define CROWN_H

#include	"Thing.h"
#include	"World.h"

DECLARE_THING_CLASS(CCrown, CThing)
public:
	virtual	void	Init(CInitThing *i) ;

	virtual	void		Process();
	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};
		void			Invisible() { mInvisible = TRUE ; }
	void			Visible() {mInvisible = FALSE ; }

	virtual	SINT	WhichRTsToRender() { if (mInvisible) return 0 ; else return SUPERTYPE::WhichRTsToRender() ; } 

	virtual	GVector			GetRenderPos(SINT no = 0);
	virtual	GMatrix			GetRenderOrientation(SINT no);

	virtual		BOOL		Pickup(CCharacter *by) {mBeingHeldBy = by; return TRUE;};
protected:

	BOOL 		mInvisible;
	CCharacter	*mBeingHeldBy;
};


#endif