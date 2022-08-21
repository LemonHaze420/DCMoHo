#ifndef WAYPOINT_H
#define WAYPOINT_H


DECLARE_THING_CLASS(CWayPoint, CThing)
public:

	virtual	void		Init(CInitThing *i) ;
	virtual void		Shutdown() ;

	virtual ECollideShape		GetCollideShape()			{return CSH_NONE;};
	virtual	BOOL				IsPersistent() {return TRUE;}; 
};


DECLARE_THING_CLASS(CPathFinderPoint, CThing)
public:

	virtual	void		Init(CInitThing *i) ;
	virtual void		Shutdown() ;

	virtual ECollideShape		GetCollideShape()			{return CSH_NONE;};
	virtual	BOOL				IsPersistent() {return TRUE;}; 
};


#endif