#ifndef	Blimp_H
#define	Blimp_H

#include	"Actor.h"

DECLARE_THING_CLASS(CBlimp, CActor)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	virtual	BOOL		DoICollideWithMap() {return FALSE;};
	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	GMatrix		GetRenderOrientation(SINT no);

	DECLARE_ZERO_GRAVITY;

protected:
	GINT	mYaw;
	GMatrix	mBladeOri;
	GMatrix mEngineOri;
};


//***********************************************
DECLARE_THING_CLASS(CFlyDroid, CActor)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();
	virtual	BOOL		DoICollideWithMap() {return FALSE;};
	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	GMatrix		GetRenderOrientation(SINT no);

	DECLARE_ZERO_GRAVITY;

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif

protected:
	GINT	mYaw;
	GMatrix	mFan1Ori;
	GMatrix	mFan2Ori;
	GMatrix mRadarOri;

};

#endif