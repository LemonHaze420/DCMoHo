#ifndef KINGHILLGOALPOINT_H
#define KINGHILLGOALPOINT_H

DECLARE_THING_CLASS(CKingHillGoalPoint, CThing)
public:

	virtual	void		Init(CInitThing *i) ;

	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};

	#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
	#endif

	
	virtual	void		Process();

	virtual	GINT				Radius() { return G(1);};
	virtual CThing*				GetTarget() { return mTarget ; };
	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual	BOOL				DoICollideWithMap() {return FALSE;};
			BOOL				IsPointInsideKingHillArea(GVector& point);
	virtual	BOOL				IsPersistent() {return TRUE;};
			GVector				GetCentrePoint() ;
	virtual void				Shutdown() ;

protected:

	CThing	*mTarget;
	GINT mXBL,mYBL, mXTR, mYTR;
};

#endif