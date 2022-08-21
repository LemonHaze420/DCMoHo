#ifndef GOLF_H
#define GOLF_H

#include	"Thing.h"
#include	"World.h"

DECLARE_THING_CLASS(CGolfArrow, CThing)
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual SINT		GetRenderThingMiscFlags() {return RTMF_ONTOP | RTMF_NOLIGHT;}

	virtual GINT		GetRenderScale();
	virtual	DWORD		GetRenderColour();

protected:

			void		Align();

};

#endif

