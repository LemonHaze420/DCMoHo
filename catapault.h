#ifndef Catapault_H
#define Catapault_H

#include	"Solid.h"

DECLARE_THING_CLASS(CCatapault, CSolid)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	BOOL		DoIRotate() {return TRUE;};
	virtual GMatrix		GetAngularVelocity(SINT no);
	
	virtual	GINT		CoF() {return GINT(0, 20000);};		// coefficient of friction

protected:
	GINT	mRoll, mRollVelocity;
	BOOL	mUp;
	SINT	mCounter;
};

//***********************************************
DECLARE_THING_CLASS(CMechCatapault, CSolid)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	BOOL		DoIRotate() {return TRUE;};
	virtual GMatrix		GetAngularVelocity(SINT no);
	virtual	GMatrix		GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 1:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	}

protected:
	GINT	mPitch, mPitchVelocity;
	BOOL	mUp;
	SINT	mCounter;

	GMatrix	mOrientation2;
};

//***********************************************
DECLARE_THING_CLASS(CSteamHammer, CSolid)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	BOOL		DoIRotate() {return TRUE;};
	virtual GMatrix		GetAngularVelocity(SINT no);
	virtual	GMatrix		GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 1:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	}

protected:
	GINT	mPitch, mPitchVelocity;
	BOOL	mUp;
	SINT	mCounter;

	GMatrix	mOrientation2;
};

#endif