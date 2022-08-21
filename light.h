#ifndef LIGHT_H
#define LIGHT_H

DECLARE_THING_CLASS(CEditorLight, CThing)
public:

	virtual	void		Init(CInitThing *i) ;

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	BOOL		IsPCOnly() {return TRUE;};
	virtual ECollideShape		GetCollideShape()			{return CSH_NONE;};

	virtual	CEditorLight	*IsLight()	{return this;};

			SINT		GetColour() {return mCounter0;};

	TED_COUNT0("Burst Speed")

protected:
	SINT	mCounter0;
};
#endif