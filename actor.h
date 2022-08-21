#ifndef ACTOR_H
#define ACTOR_H

#include	"Thing.h"
#include	"Friction.h"

DECLARE_THING_CLASS(CActor, CThing)
public:

	virtual	void		Init(CInitThing *i);
	virtual	void		Process();

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	GVector		GetRenderPos(SINT no = 0);

	virtual	GVector		GetOldPos() {return mOldPos;};
	virtual	GVector		GetVelocity(SINT no = 0) {if (no == 0) return mVelocity; else return ZERO_GVECTOR;};
	virtual	void		SetVelocity(GVector v) {mVelocity = v;};
	virtual	void		AddVelocity(GVector v) {mVelocity += v;};
			void		CollideSphereAABB(CActor *a, CThing *b, BOOL passive) ;
			void		CollideSphereOBBStatic(CActor *a, CThing *b, SINT colision_volume) ;
			void		CollideSphereOBBMovingStatic(CActor *a, CThing *b) ;

	virtual	GINT		GroundFriction(EFrictionType friction_type);

	virtual	GINT		Gravity();
	virtual	BOOL		DoICollideWithMap() {return TRUE;};
	virtual	BOOL		DoISeekCollisions() {return TRUE;};
	virtual GINT		GetMass() { return G(1) ; } ;

	virtual	GINT		GetMapDamping()	{return G(0, 32768);};

	virtual	void		HitGround(GINT force)	{};
	virtual	void		HitArena()	{};

	virtual	ECollideResponse	GetCollideResponse()	{return CR_BOUNCE;};

			void		SetSpin(GVector ax);
			void		Roll(); // guess what!

			CThing		*FindFirstCollision(GVector &v, GINT r);
			CThing		*FindNextCollision();


	virtual	void		DeclareBoost() {};


protected:
	GVector		mVelocity;
	GVector		mOldPos;
	GMatrix		mAngularVelocity;
	SINT		mStunned;

	void		ProcessCollision();
	void		ProcessObjectCollision();
	virtual     void		ProcessMapCollision();

	BOOL		DoSpheresCollide(CThing *a, CThing *b, GVector &dir);
	void		CollideSphereSphereBounce(CThing *a, CThing *b);
	void		CollideSphereSphereStatic(CActor *a, CThing *b);
	void		CollideSphereSpherePassive(CThing *a, CThing *b);
	void		CollideSphereComplexStatic(CActor *a, CThing *b);
	void		CollideSphereParticleSystem(CActor *a, CThing *b);
	void		CollideSphereFence(CActor *a, CThing *b, BOOL passive);
	void		CollideSphereFenceXAllign(CActor *a, CThing *b, BOOL passive);
	void		CollideSphereFenceYAllign(CActor *a, CThing *b, BOOL passive);
	static		CThing	*mCachedCollisionThing;
	static		GVector	mCachedCollisionPos;
	static		GINT	mCachedCollisionRadiusSq;

	static		CThing* mLastAThing ;
	static		SINT    mLastAThingTime ;
	static		ECollideShape mCurrentACollideShape ;
	static	    ECollideShape mCurrentBCollideShape ;

	GVector		mCurrentMapNormal ;
	SINT		mCurrentMapNormalCalcTime ;
	ULONG	mDFlags;


};


#endif