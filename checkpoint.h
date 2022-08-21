#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include	"Thing.h"

DECLARE_THING_CLASS(CCheckPoint, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	//! Hmmmmmmmmmmmm
	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};
//	virtual	GINT		Radius() { return GetRenderThingRadius();};

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	TED_COUNT0("CheckPoint No")
	TED_BOOL0("Not Jump?")
 

	virtual	void		ValidateData() {VALIDATE(mTarget); VALIDATE(mCheckPointSection); SUPERTYPE::ValidateData();};
	virtual	BOOL		IsPersistent() {return TRUE;};
			SINT		GetCheckPointNumber() { return mCounter0 ; }  
	virtual CCheckPoint *IsCheckPoint() { return this ;};
	virtual	void		Shutdown() ;
	virtual CThing*		GetTarget() { return mTarget ; };
	virtual	void		Trigger(CThing *top);
	virtual	void		UnTrigger(CThing *top);
			BOOL		IsJumpCheckPoint() { return !mBool0 ; } ;
			BOOL		GetActivated() { return mActivated ; } 
	
	static  CCheckPoint* GetACheckPointFromNumber(SINT num) ;

#ifdef LINK_EDITOR
	virtual	void		DeclareSpawnees();
#endif

protected:
	CThing	*mTarget;
	CThing	*mCheckPointSection;
	BOOL mActivated;
	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;
	SINT mCounter0; // check point number
	BOOL mBool0; // not jump.  tells racers to jump here 
};


DECLARE_THING_CLASS(CCheckPointSection, CThing)
public:
	virtual	void		Init(CInitThing *i);
			void		SetStartAndEnd(GVector &start, GVector &end);

	virtual ECollideShape		GetCollideShape()		{return CSH_FENCE;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};
	virtual	GVector				GetCollisionNormal()	{return mNormal;};

	virtual	GVector		GetRenderStartPos()		{ return mStart;};
	virtual	GVector		GetRenderEndPos()		{ return mEnd;};
	void		SetCheckPointNumber(SINT to_num)   { mCheckPointNum = to_num ; } ;
	void		SetJump(BOOL to)   { mJump = to ; } ;
	virtual	GINT		Radius() { return mRadius;};

	virtual	void		Hit(CThing *that);
	virtual	BOOL		DoIHit(CThing *t) {return (t->IsCharacter() != NULL);};

protected:
	GVector	mStart, mEnd, mNormal;

	GINT	mRadius;
	SINT	mCheckPointNum;
	BOOL    mJump ;
};



//******************************************************************************************



DECLARE_THING_CLASS(CRacerReSpawner, CThing)
	public:
		virtual	void		Init(CInitThing *i);
		virtual	BOOL		IsPersistent() {return TRUE;};
		virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};
				GINT		GetYaw() { return mYaw ;}
	protected:
	GINT mYaw ;
};


#endif