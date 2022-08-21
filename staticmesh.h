#ifndef StaticMesh_H
#define StaticMesh_H

#include	"Actor.h"

DECLARE_THING_CLASS(CStaticMesh, CActor)
public:
	virtual	void			Init(CInitThing *i);

	virtual	void			Process();

	virtual ECollideShape		GetCollideShape()		{return CSH_AABB;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
	virtual	BOOL				IsPersistent() {return TRUE;};
	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual	BOOL				DoICollideWithMap() {return FALSE;};
	DECLARE_ZERO_GRAVITY;
	virtual	GINT				Radius() { return GetRenderThingRadius();};
protected:
};

//******************************************************************************************
DECLARE_THING_CLASS(CSMArrowL, CStaticMesh) };
DECLARE_THING_CLASS(CSMArrowR, CStaticMesh) };


DECLARE_THING_CLASS(CSMBillBoard, CStaticMesh) 
	virtual ECollideShape		GetCollideShape()		{return CSH_OBB3;};
};

DECLARE_THING_CLASS(CSMWaterTower, CStaticMesh) 
	virtual ECollideShape		GetCollideShape()		{return CSH_OBB;};
};

DECLARE_THING_CLASS(CSMGolfBoard, CStaticMesh) 
	virtual ECollideShape		GetCollideShape()		{return CSH_OBB;};
};

DECLARE_THING_CLASS(CSMLitTower, CStaticMesh) 
	virtual ECollideShape		GetCollideShape()		{return CSH_OBB3;};
};


DECLARE_THING_CLASS(CSMIcon, CStaticMesh) };
DECLARE_THING_CLASS(CSMStatue, CStaticMesh) };
DECLARE_THING_CLASS(CSMVase, CStaticMesh) };

DECLARE_THING_CLASS(CSMChevron, CStaticMesh) 
	virtual ECollideShape		GetCollideShape()		{return CSH_OBB;};
};
DECLARE_THING_CLASS(CSMChevrons, CStaticMesh) 
	virtual ECollideShape		GetCollideShape()		{return CSH_OBB;};
};


DECLARE_THING_CLASS(CTVCamera, CStaticMesh)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();
	virtual	BOOL		RenderUseHierarchy(SINT pn)	{return (pn != 1);};
	virtual	GMatrix		GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 1:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	};

	GINT	mYaw;
	GINT	mPitch ;
	GMatrix	mOrientation2;
};

#endif