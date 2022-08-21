#ifndef HOLDINGZONEPOINT_H
#define HOLDINGZONEPOINT_H

DECLARE_THING_CLASS(CHoldingZonePoint, CThing)
public:

	virtual	void		Init(CInitThing *i) ;

	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};

	#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
	#endif
	TED_COUNT0("HoldZoneP No")

	virtual	GINT				Radius() { return G(1);};
	virtual CThing*				GetTarget() { return mTarget ; };
	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual	BOOL				DoICollideWithMap() {return FALSE;};
			BOOL				IsPointInsideZone(GVector& point);
			SINT				GetHoldingZoneNumber() { return mCounter0 ; }
			GVector				FindClosestPointInZoneTo(GVector& pos) ;
	virtual	BOOL				IsPersistent() {return TRUE;}; 
	virtual void				Shutdown() ;

protected:

	SINT mCounter0; // holding zone point number
	CThing	*mTarget;
	GINT mXBL,mYBL, mXTR, mYTR;
};

#endif