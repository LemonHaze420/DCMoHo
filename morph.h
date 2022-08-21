#ifndef Morph_H
#define Morph_H

#include	"Thing.h"

DECLARE_THING_CLASS(CMorph, CThing)
public:
	virtual	ECollideShape	GetCollideShape()	{return CSH_NONE;};
	virtual	void			Init(CInitThing *i);

	virtual void			Die() {}; //! they don't die, yet!

protected:
	SINT	mLife;
};

//******************************************************************************************
DECLARE_THING_CLASS(CMPBomb, CMorph)
public:
	virtual	void			Process();
};

//******************************************************************************************
#define	MPAVERAGE_SIZE 5

DECLARE_THING_CLASS(CMPAverage, CMorph)
public:
	virtual	void			Init(CInitThing *i);
	virtual	void			Process();

protected:
	GINT	mAverageHeight;
};

//******************************************************************************************
#define	MPLOWER_SIZE 5

DECLARE_THING_CLASS(CMPLower, CMorph)
public:
	virtual	void			Init(CInitThing *i);
	virtual	void			Process();

protected:
	GINT	mLowerHeight;
};




#endif