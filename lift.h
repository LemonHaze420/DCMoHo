#ifndef Lift_H
#define Lift_H

#include	"Solid.h"

DECLARE_THING_CLASS(CLift, CSolid)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();

	virtual	GVector		GetEndPos() {return mPos + GVector(G0, G0, -G(10));};
	virtual	GINT		GetAccel()  {return GINT(0, 1200);};
	virtual	SINT		GetCounterMax() {return 120;};
	virtual	GINT		GetDamping() {return GINT(0, 26214);};

//	virtual	GINT		CoF() {return GINT(0, 5000);};		// coefficient of friction
//	virtual	GINT		CoE() {return GINT(0, 0);};		// coefficient of friction

	virtual	BOOL		DoIRotate() {return TRUE;};
	virtual GMatrix		GetAngularVelocity(SINT no);

	virtual	GINT		RollHitForwards() {return G0;};
	virtual	GINT		RollHitBackwards() {return G0;};
	
	virtual GINT		InitialYaw() {return G0;};

protected:
	GVector mStart, mDir;
	GINT	mDist, mEndDist;
	GINT	mSpeed;
	BOOL	mForwards;
	SINT	mCounter;
	
	GINT	mRoll, mRollVelocity;
	GINT	mYaw;
};

DECLARE_THING_CLASS(CLiftRight1, CLift)
	virtual	GVector		GetEndPos() {return mPos + GVector(G(15), G0, G0);};
	virtual	SINT		GetCounterMax() {return 120;};

	virtual	GINT		RollHitForwards() {return G(0, 6500);};
//	vi    rtual	GINT		RollHitForwards() {return G(0, 10000);};

	virtual GINT		InitialYaw() {return G_PI;};
};


#endif