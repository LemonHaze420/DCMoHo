#ifndef	CharBall_H
#define	CharBall_H

#include	"Character.h"
#include	"effect.h"

// say if less than 30 degs then we are upright

#define BALL_UPRIGHT_TOLERANCE G(0,34314)


DECLARE_THING_CLASS(CCharBall, CCharacter)
public:
	virtual	void	Init(CInitThing *i);

	virtual	void	Process();
//	virtual	GINT	Radius() { GINT_HALF };
	virtual	GINT	Radius() { return G(0, 40000);};
//	virtual	GINT	Radius2() { return GINT(1, 0);};
	virtual	GINT	Radius2() { return G(0, 55000);};

	virtual GINT	LinkRadius() {return Radius();};  // for link
	void	SetYaw(GINT to) { mYaw = to ; }

	virtual	GINT	RadiusForMapWho() {return Radius() + (Radius2() * 2);};

	virtual ECollideShape		GetCollideShape()
	{
		return mInvisible ? CSH_NONE : (mSplit ? CSH_SPHERE :CSH_DOUBLE_SPHERE);
	}

	void		SetInvisible(BOOL to) { mInvisible = to; }
	virtual	GVector		GetPos(SINT n = 0);
	virtual	GVector		GetRenderPos(SINT no = 0)
	{
		if (no == 0)
			 return SUPERTYPE::GetRenderPos(no);
		else return GVector(G0, G0, mSuspension);
	};

	virtual	GMatrix			GetRenderOrientation(SINT no);
	
	virtual void		AddStrikeVelocity(GVector vel, GINT term_vel) ;
	virtual	BOOL		Strike(const CStrike& strike);
	virtual void		CatchOnFire() ;
	virtual SINT		GetRenderThingMiscFlags();

	virtual	GVector		GetRenderStartPos();
	virtual	GVector		GetRenderEndPos();

	virtual	void		HitGround(GINT force);

	virtual	void		StateDead();
	virtual SINT		WhichRTsToRender();

	virtual void		ValidateData() { VALIDATE(mOnFireFlames[0]); VALIDATE(mOnFireFlames[1]); SUPERTYPE::ValidateData();};

	virtual void		StateKnockedOut();
	virtual void		StateStunned() ;
	virtual	void		StateBeingLasered() ;
	virtual void		ProcessMapCollision();
	virtual void		Throw() ;
	virtual void		StartDieProcess(BOOL hit_fan = FALSE);
	virtual BOOL		Attack(SINT no);

	virtual	void		Thrust(const GVector &v);
			BOOL		MainCharacterAttack(SINT no);
			BOOL		RacerAttack() ;

	virtual	BOOL		RenderUseHierarchy(SINT pn)	{return (pn != 1);};

	virtual	void		AnimTrigger(const GMatrix& current_anim_orientation,BOOL end_of_punch_hit);
			BOOL		SpinAttack() ;
			void		SpinAnimTrigger(const GMatrix& current_anim_orientation) ;
			void		PunchLeftRightTrigger(GVector& lr) ;

			BOOL		IsUpright() { return GABS(mPitch) < BALL_UPRIGHT_TOLERANCE ; };

			void		Trick(SINT no);
			void		SetSpinVelocity(GINT spin_val) { mSpinVelocity = spin_val ; } ;

			GINT		GetSuspension() {return mSuspension;};
			GVector		GetIceBallSpinVector() { return mIceBallSpinVector ;};
			void		SetIceBallSpinVector(GVector v) { mIceBallSpinVector = v;};
			GVector		GetRacerBoostVector() { return mRacerBoostVector ;};
			void		SetRacerBoostVector(GVector v) { mRacerBoostVector = v;};


	// split stuff
	virtual BOOL		RenderRenderPart(SINT pn)	{return ( (pn == 0) || (!mSplit));};
	virtual	SINT		GetTorsoID() {return OID_NONE;};

#ifdef LINK_EDITOR
	virtual	void		DeclareSpawnees();
#endif

	virtual void		SetDestYaw(GINT	yaw) {mDestYaw = yaw;};

protected:
	GMatrix		mOrientation2, mLastOrientation2;
	GINT		mGroundHeightBelowHead ;
	GVector		mDir;

	GINT		mPitchVelocity, mPitchTo;

	GINT		mSpinVelocity ;
	BOOL		mHitSomethingDuringSpinAttack;

	GINT		mSuspension, mSuspensionVelocity;

	GINT		mDestYaw;
	GINT		mPitch, mDestPitch;
	GINT		mRoll, mDestRoll;
	

	GINT		mDestPitchA, mDestPitchL;
	GINT		mHowMuchILeanForwardWhenRunning ;

	GINT		mTrickYaw; // for tricks

	BOOL		mSplit;	// have I split in two?


	GINT mSpinVelocityWhenStruck;

	//
	GVector mIceBallSpinVector ;
	GVector mRacerBoostVector ;

};

#endif