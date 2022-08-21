#ifndef	Projectile_H
#define	Projectile_H

#include	"Actor.h"

#define PROJECTILE_MASS G(0, 8192) 

enum	EPJHitAction
{
	EPJH_NORMAL = 0,
	EPJH_RAINDROP = 1,
};

DECLARE_THING_CLASS(CProjectile, CActor)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();

	virtual ECollideShape		GetCollideShape()			{return CSH_SPHERE;};
	virtual	ECollideResponse	GetCollideResponse()		{ return CActor::mCurrentBCollideShape == CSH_FENCE ? CR_PASSIVE : CR_BOUNCE;};
	virtual	GINT				Radius() { return G(0, 16000);};
//	virtual	GINT				Radius() { return G(0, 32000);};

	virtual	GINT		CoE() {return GINT(0, 25000);};		// coefficient of elasticity

	virtual	CProjectile			*IsProjectile() {return this;};

	virtual	void				Hit(CThing *that);
	virtual	void				HitGround(GINT force);
	virtual	void				HitArena()	{mGravity = TRUE;};
	virtual	GINT				GetMass()	{ return PROJECTILE_MASS ; }
	virtual	BOOL				DoIHit(CThing *t) {return ((!(t->IsProjectile())) && (!(t->IsLaserBeam())));};

	virtual	GINT				Gravity() {return mGravity ? SUPERTYPE::Gravity() : G0;}

	virtual	EPJHitAction		GetHitAction() {return EPJH_NORMAL;};

	virtual	SINT				GetHitSample() {return SOUND_RICOCHET;};

protected:
	BOOL	mGravity;
};

DECLARE_THING_CLASS(CPJBullet, CProjectile)
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();

//	virtual	GINT		GetRenderRadius() {return GetRenderThingRadius() * G(1, 30000);};
	virtual	GINT		GetRenderRadius() {return GetRenderThingRadius() * G(0, 35000) * 2;};
//	virtual	GINT		GetRenderRadius() {return GetRenderThingRadius() * 2;};

	virtual	BOOL		GetRenderRotateShadow()			{return TRUE;};
	virtual	GINT		GetRenderYaw()					{return mYaw;};

	virtual GINT		GetRenderFrame() {return -G1;};

protected:
//	GINT	mRoll;
	GINT	mYaw;
};

DECLARE_THING_CLASS(CPJBulletRainDrop, CProjectile)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		Process();

	virtual	EPJHitAction		GetHitAction() {return EPJH_RAINDROP;};
	virtual	GINT				Radius() { return G(0, 32000);};
	virtual	BOOL		GetRenderRotateShadow()			{return TRUE;};
	virtual	GINT		GetRenderYaw()					{return mYaw;};

	virtual GINT		GetRenderFrame() {return -G1;};
protected:
	GINT	mYaw;
};


#endif