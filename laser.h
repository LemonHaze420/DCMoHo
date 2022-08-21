#ifndef LASER_H
#define LASER_H

#include	"Thing.h"

DECLARE_THING_CLASS(CLaserTurret, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Trigger(CThing *top);
	virtual	void		UnTrigger(CThing *top);


	virtual ECollideShape		GetCollideShape()		{return CSH_OBB2;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
	virtual	GINT		RadiusForMapWho() {return Radius()*4;};

			void		Shoot();
			void		DontShoot();
			void		ActivateLaserTurret();

	virtual	void		ValidateData() {VALIDATE(mBeam); SUPERTYPE::ValidateData();};

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif

	TED_COUNT0("Burst Speed")
	TED_COUNT1("Burst Duration")

	virtual	BOOL			RenderUseHierarchy(SINT pn)	{return (pn != 1);};

	virtual	GMatrix			GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 1:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	};

protected:
	GINT	mYaw, mPitch;

	SINT	mCounter0, mCounter1;

	SINT	mTimer;

	GMatrix	mOrientation2;

	BOOL mActivated;
	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;
	SINT mStartBurstNumber;

	CThing	*mBeam;
};



DECLARE_THING_CLASS(CLaserBeam, CThing)
public:
	virtual	void		Init(CInitThing *i);
			void		SetStartAndEnd(GVector &start, GVector &end);

	virtual ECollideShape		GetCollideShape()		{return CSH_FENCE;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};

	virtual	GVector		GetCollisionNormal() {return mNormal;};

	virtual	CLaserBeam	*IsLaserBeam() {return this;};

	virtual	GVector		GetRenderStartPos()		{ return mStart;};
	virtual	GVector		GetRenderEndPos()		{ return mEnd;};

	virtual	GINT		Radius() { return mRadius;};
	virtual void		Process() ;

	virtual	void		Hit(CThing *that);

protected:
	GVector	mStart, mEnd, mNormal;
	GVector mOriginalEnd;
	BOOL	mLastFrameSomeOneHitUs ;

	GINT	mRadius;
};




#endif
