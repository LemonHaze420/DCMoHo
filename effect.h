#ifndef EFFECT_H
#define EFFECT_H

#include	"Actor.h"
#include	"SampleList.h"

DECLARE_THING_CLASS(CEffect, CActor)
public:

	virtual	BOOL				DoISeekCollisions() {return FALSE;};
	virtual ECollideShape		GetCollideShape()		{return CSH_NONE;};
	DECLARE_ZERO_GRAVITY;
	virtual	BOOL				DoICollideWithMap() {return FALSE;};

};

//******************************************************************************************
DECLARE_THING_CLASS(CFXLightning, CEffect)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	GVector		GetRenderStartPos()		{ return mStart;};
	virtual	GVector		GetRenderEndPos()		{ return mEnd;};

			void		SetStartAndEnd(GVector &start, GVector &end);


protected:
	GVector	mStart, mEnd;

};

//******************************************************************************************
DECLARE_THING_CLASS(CEffectParticle, CEffect)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	CParticleSystem	*GetRenderParticleSystem()	{return mParticleSystem;};
	virtual	CParticleSystem *GetParticleSystem()		{return mParticleSystem;};

	virtual	SINT		InitialLife() {return 100;};
	virtual	void		Die();
	virtual	void		CreateParticles() = 0;

	virtual ECollideShape		GetCollideShape()			{return CSH_PARTICLE_SYSTEM;};

	virtual	SINT		GetSample() {return -1;};

protected:
	CParticleSystem		*mParticleSystem;
};

/*
//******************************************************************************************
DECLARE_THING_CLASS(CFXZ, CEffectParticle)
public:
	virtual	void		CreateParticles();
};
*/

//******************************************************************************************
DECLARE_THING_CLASS(CFXStreamer, CEffectParticle)
public:
	virtual	void		Init(CInitThing *i);
	virtual	void		CreateParticles();

	virtual	GINT		Radius() {return G(6);}; //! hmmm.

	virtual	EParticleType	ParticleType() {return PART_FIRE1;};
	virtual	GINT			YParticleSpeed() {return G1;};
	virtual SINT			NewParticleFrequency() { return 2 ; };
			void			MoveStreamerAndParticles(GVector* np) ;
			void			SetXRandomStartOffset(GINT to) { mXRandomStartOffset = to ; }
			void			SetYRandomStartOffset(GINT to) { mYRandomStartOffset = to ; }
			void			SetZRandomStartOffset(GINT to) { mZRandomStartOffset = to ; }

protected:

	GINT	mXRandomStartOffset ;
	GINT	mYRandomStartOffset ;
	GINT	mZRandomStartOffset ;

};

//******************************************************************************************
#define FLAMER_SPEED	G(0, 15000)

DECLARE_THING_CLASS(CFXFlamer, CFXStreamer)
public:
	virtual	EParticleType	ParticleType() {return PART_FIRE1;};
	virtual	GINT			YParticleSpeed() {return FLAMER_SPEED;};

	virtual	SINT			GetSample() {return SOUND_FLAME1;};

};

//******************************************************************************************
DECLARE_THING_CLASS(CFXBodyFlamer, CFXStreamer)
public:
	virtual	void		Init(CInitThing *i) { mNewParticleFrequency = 5; SUPERTYPE::Init(i) ; mInvisible=FALSE; }
	virtual	EParticleType	ParticleType() {return PART_BODY_FIRE;};
	virtual SINT			NewParticleFrequency() { return mNewParticleFrequency ; }
	void			SetParticleFrequency(SINT amount) { mNewParticleFrequency = amount; }
	void			Invisible() { mInvisible = TRUE ; }
	void			Visible() {mInvisible = FALSE ; }

	virtual	SINT	WhichRTsToRender() { if (mInvisible) return 0 ; else return SUPERTYPE::WhichRTsToRender() ; } 
	virtual void		Process() ;
	virtual	GINT			YParticleSpeed() {return G(0, (rand() % 2000)) - G(0, 1000);};
protected:
	SINT mNewParticleFrequency ;
	BOOL mInvisible ;
};

//******************************************************************************************
DECLARE_THING_CLASS(CFXFlamer2, CFXStreamer)
public:
	virtual	EParticleType	ParticleType() {return PART_FIRE2;};
	virtual	GINT			YParticleSpeed() {return FLAMER_SPEED;};
};

//******************************************************************************************
DECLARE_THING_CLASS(CFXFlamer3, CFXStreamer)
public:
	virtual ECollideShape		GetCollideShape()			{return CSH_NONE;};
	virtual	EParticleType	ParticleType() {return PART_FIRE3;};
	virtual	GINT			YParticleSpeed() {return FLAMER_SPEED;};
};

//******************************************************************************************
DECLARE_THING_CLASS(CFXExplosion1, CEffectParticle)
public:
	virtual	void		CreateParticles();
};

//******************************************************************************************
DECLARE_THING_CLASS(CFXExplosion2, CEffectParticle)
public:
	virtual	void		Init(CInitThing *i) { mRippleLandscape = TRUE ;mAge = 0; SUPERTYPE::Init(i) ; }
	virtual	void		CreateParticles();
	virtual void		Process() ;
	virtual	GINT		Radius() {return G(6);}; //! hmmm.
	virtual void		Hit(CThing* that);
	void		SetRipple(BOOL to) { mRippleLandscape = to ; }
	virtual	BOOL				DoISeekCollisions() {return TRUE;};
	virtual	ECollideResponse	GetCollideResponse()		{return CR_PASSIVE;};
	virtual ECollideShape		GetCollideShape()		{return mAge != 10 ? CSH_NONE : CSH_SPHERE ;};
protected:
	SINT mAge ;
	BOOL mRippleLandscape ;
};

//******************************************************************************************
DECLARE_THING_CLASS(CFXTeleportIn, CEffectParticle)
public:
	virtual	void		CreateParticles();
	virtual void		Process() ;

};

//******************************************************************************************
DECLARE_THING_CLASS(CFXRageAttack, CEffectParticle)
public:
						CFXRageAttack() {mOwner=NULL;}
	virtual	void		CreateParticles();
	virtual void		Process() ;
			void		SetOwner(CThing *t) {mOwner=t;}
			CThing *	GetOwner() {return mOwner;}
	virtual	void		ValidateData() {VALIDATE(mOwner);SUPERTYPE::ValidateData();};

protected:
	CThing *mOwner;
};


#endif
