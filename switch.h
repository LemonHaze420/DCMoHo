#ifndef SWITCH_H
#define SWITCH_H

#include	"Thing.h"

DECLARE_THING_CLASS(CSwitch, CThing)
public:

	virtual	void		Init(CInitThing *i);

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

//	virtual	void		Process();

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};

	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};

	virtual	BOOL		IsPersistent() {return TRUE;};

			BOOL		IsSwitched() {return mSwitched;};

	virtual	CSwitch		*IsSwitch() {return this;};

protected:
	CThing	*mTarget;

	BOOL	mSwitched;
};

//******************************************************************************************
//******************************************************************************************
DECLARE_THING_CLASS(CSwitchDead, CSwitch)
	virtual	void		Process();
};

//******************************************************************************************
//******************************************************************************************
DECLARE_THING_CLASS(CSwitchAND, CSwitch)
	virtual	void		Process();
};

//******************************************************************************************
//******************************************************************************************
DECLARE_THING_CLASS(CSwitchOR, CSwitch)
	virtual	void		Process();
};

//******************************************************************************************
//******************************************************************************************
// JCL - ok - I'm lazy - this shouldn't be here..

DECLARE_THING_CLASS(CPit, CThing)
public:
	virtual	void		Init(CInitThing *i);

	virtual ECollideShape		GetCollideShape()		{return CSH_AABB;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_PASSIVE;};
	virtual void				Hit(CThing* that) ;
	virtual	GINT		RadiusForMapWho() {return Radius()*2;};
	virtual void				Process() ;
	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	GMatrix			GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 2:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	};
	virtual BOOL		GetRenderCanGoFlatShaded()		{return FALSE;};

	GMatrix mOrientation2 ;
};

#endif