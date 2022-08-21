#ifndef NamedMesh_H
#define NamedMesh_H

#include	"Actor.h"

DECLARE_THING_CLASS(CNamedMesh, CThing)
public:
	virtual	void			Init(CInitThing *i);

	virtual ECollideShape		GetCollideShape()		{return CSH_OBB;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};
	virtual	BOOL				IsPersistent() {return TRUE;};
	virtual	GINT				Radius() { return GetRenderThingRadius();};

	virtual	SINT			GetRenderMesh()				{return mMesh;};

#ifdef	LINK_EDITOR
	virtual BOOL			DoesThingContainResources() {return TRUE;};
#endif

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

protected:

	SINT	mMesh;
};

#endif