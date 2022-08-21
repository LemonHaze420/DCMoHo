#ifndef	CharNoBall_H
#define	CharNoBall_H

#if 0

#include	"Character.h"

DECLARE_THING_CLASS(CCharNoBall, CCharacter)
public:
	virtual	void	Init(CInitThing *i);

	virtual	void	Process();
	virtual	GINT	Radius() { return GINT(0, 40000);};

	virtual	void			Hit(CThing *that);

	virtual	void			StateNormal();
	virtual	void			StateDead();
	virtual	void			StateAsleep();


	virtual void	StartDieProcess(BOOL hit_fan=FALSE);

protected:
	GINT		mTilt;

	GVector		mDir;

	GINT		mTiltVelocity, mTiltTo;

};

#endif

#endif