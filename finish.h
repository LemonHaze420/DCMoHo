#ifndef Finish_H
#define Finish_H

#include	"Thing.h"

DECLARE_THING_CLASS(CFinish, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	ECollideShape	GetCollideShape()	{return CSH_NONE;};
	virtual	void		Process();
	virtual	GINT		Radius() { return G1;};

protected:
	GINT	mInitialZ;
	GINT	mSize;

};

DECLARE_THING_CLASS(CFinishPin, CFinish)
	virtual	void		Init(CInitThing *i);

	virtual	EAnimMode	GetRenderAnimation(){return mAnimMode;};
			BOOL		SetAnimMode(EAnimMode am, BOOL reset_frame = TRUE);
	virtual	GINT		GetRenderFrame()				{return mFrame;};
	virtual SINT		GetRealAnimIndex()				{return mRealAnimIndex;};
			void		ProcessAnimation() ;

	virtual	void		Process();
	
	SINT	GetRenderThingMiscFlags()
	{
		SINT res = SUPERTYPE::GetRenderThingMiscFlags() ;
		res |= RTMF_WHITE;
		return res;
	};
	

protected:
	EAnimMode			mAnimMode;
	GINT				mFrame;
	GINT				mFrameInc;
	SINT				mRealAnimIndex;

};


DECLARE_THING_CLASS(CFinishTrigger, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};

	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Trigger(CThing *top);

protected:
};

#endif