#ifndef FencePost_H
#define FencePost_H

#include	"Thing.h"

DECLARE_THING_CLASS(CFencePost, CThing)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Die();
	virtual	void		Process();

	//! Hmmmmmmmmmmmm
//	virtual ECollideShape		GetCollideShape()		{return CSH_COMPLEX;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
//	virtual	GINT		Radius() { return GetRenderThingRadius();};

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		ValidateData() {VALIDATE(mTarget); VALIDATE(mFenceSection); SUPERTYPE::ValidateData();};
	virtual	BOOL		IsPersistent() {return TRUE;};

#ifdef LINK_EDITOR
	virtual	void		DeclareSpawnees();
#endif

protected:
	CThing	*mTarget;
	CThing	*mFenceSection;
	CThing	*mFenceSectionC;
};


DECLARE_THING_CLASS(CFenceSection, CThing)
public:
	virtual	void		Init(CInitThing *i);
			void		SetStartAndEnd(GVector &start, GVector &end);

	virtual ECollideShape		GetCollideShape()		{return mCollideShape;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_BOUNCE;};
	virtual	GVector				GetCollisionNormal() {return mNormal;};

	virtual	GVector		GetRenderStartPos()		{ return mStart;};
	virtual	GVector		GetRenderEndPos()		{ return mEnd;};
			BOOL		AboveOrBelow(GVector& point);
			void		MoveStartZVal(GINT amount) { mStart.Z+=amount; }
	virtual void		Process() ;

	virtual	GINT		Radius() { return mRadius;};

	virtual	void		Hit(CThing *that);

protected:
	GVector	mStart, mEnd, mNormal;
	ECollideShape mCollideShape ;

	GINT	mRadius;
};
#endif