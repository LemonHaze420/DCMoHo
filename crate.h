#ifndef CRATE_H
#define CRATE_H

#include	"Thing.h"
#include	"World.h"

DECLARE_THING_CLASS(CCrate, CThing)
public:
	virtual	void		Init(CInitThing *i);

#if TARGET == PC || DC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		Process();
	virtual	BOOL		IsPersistent() {return TRUE;};

	virtual	void		Hit(CThing *that);

	virtual ECollideShape		GetCollideShape()		{return CSH_AABB;};
	virtual	ECollideResponse	GetCollideResponse()	{return CR_STATIC;};

	virtual	GINT				Radius() { return GetRenderThingRadius();};

	virtual	BOOL		Strike(const CStrike& strike);

	virtual	void		Damage(GINT amount);
	virtual BOOL		IsStrikeable() { return TRUE ; }


	virtual	void		Smash();
			void		Rock();

	virtual	SINT		StrikeScore() {return 0;};  
	
	virtual void		Die() {Smash(); SUPERTYPE::Die();};

#ifdef LINK_EDITOR
	virtual	void		DeclareSpawnees();
#endif

	virtual	SINT		HitDamage() {return 51;};
	virtual	BOOL		DoIRock() {return TRUE;};

	virtual	void		ValidateData() {VALIDATE(mTarget); SUPERTYPE::ValidateData();};

	virtual	SINT		SmashSample() {return SOUND_CRATE;};

	virtual BOOL		DoesThingContainResources() {return TRUE;};

protected:
	GINT	mZBase;
	GINT	mZVel;

	SINT	mOID;
	CThing	*mTarget;

	BOOL	mSmashed;

};

DECLARE_THING_CLASS(CGenerator, CCrate)
 	virtual	void		Init(CInitThing *i);

	virtual	void		ValidateData() {VALIDATE(mEffect); SUPERTYPE::ValidateData();};

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif
	virtual	void		Smash();

	virtual	SINT		HitDamage() {return 34;};
	virtual	SINT		StrikeScore() {return 0;};  
	virtual	BOOL		DoIRock() {return FALSE;};
	
	virtual	SINT		SmashSample() {return SOUND_EXPLODE1;};

	virtual void		Die();
	virtual void		Process();

protected:
	CThing	*mEffect;

	SINT	mDeathCount;
};

#endif