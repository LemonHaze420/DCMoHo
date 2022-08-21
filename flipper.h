#ifndef Flipper_H
#define Flipper_H

#include	"Solid.h"
#include	"Controls.h"

DECLARE_THING_CLASS(CFlipper, CSolid)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();

	virtual	BOOL		DoIRotate() {return TRUE;};
	virtual GMatrix		GetAngularVelocity(SINT no); // for collision

	virtual	GINT		GetStartYaw() = 0;
	virtual	GINT		GetEndYaw() = 0;
	virtual	SINT		GetButton() = 0;

	virtual	GMatrix			GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 1:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	};

protected:
	GINT	mYaw;
	GINT	mOmega;
	GMatrix	mOrientation2;
};

DECLARE_THING_CLASS(CFlipperRight, CFlipper)
	virtual	GINT		GetStartYaw() {return  (G_PI * 5) / 4;};
	virtual	GINT		GetEndYaw() {return (G_PI * 3) / 4;};
	virtual	SINT		GetButton() {return BUTTON_FLIPPER_RIGHT;};
};

DECLARE_THING_CLASS(CFlipperLeft, CFlipper)
	virtual	GINT		GetStartYaw() {return -G_PI / 4;};
	virtual	GINT		GetEndYaw() {return G_PI / 4;};
	virtual	SINT		GetButton() {return BUTTON_FLIPPER_LEFT;};
};


#endif