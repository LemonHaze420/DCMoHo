#ifndef	Character_H
#define	Character_H

#include	"Actor.h"
#include	"ListArray.h"
#include	"Trail.h"
#include	"CharacterLevelData.h"
class CFXBodyFlamer ;

#include "HoldableThing.h"
class CCrown ;

#define NUM_OF_GUYS_ALLOWED_TO_ATTACK_ONE_GUY_AT_SAME_TIME 3

#define MAX_NUM_ATTACKING_CHARS 10
#define CHAR_KNOCK_OUT_TIME 80
#define BLOCKING_DEFFENSE_ANGLE G(0,62910) // around 55 degs

class CHoldableThing;
class CCharacter ;

#define RUNNING_VELOCITY G(0,20000)

typedef  CListArray<CCharacter, MAX_NUM_ATTACKING_CHARS>  CharacterList ;

//#define DIRECT_JUMP_POWER		(-G(0, 27000))
#define DIRECT_JUMP_POWER		(-G(0, 18000))

#define SMALL_COMBAT_MAX_VELOCITY G(0,6000)
#define BIG_PUNCH_COMBAT_MAX_VELOCITY G(10)

enum	ECharState
{
	CS_NORMAL,
	CS_TELEPORT,
	CS_DEAD,
	CS_FINISHED,
	CS_ASLEEP,
	CS_PATROL,
	CS_SCANNING,
	CS_STATIONARY,
	CS_WAITING,
	CS_DEACTIVATED,
	CS_KNOCKED_OUT,
	CS_STUNNED,
	CS_LASERED
};


enum EAttackCharactersBF
{
	AT_NOTHING = 0,
	AT_PRIMARY = 1,
	AT_BADGUY = 2,
//	AT_ZBOT = 4,
//	AT_SBOT = 8,
//	AT_CBOT = 16,
	AT_GUARD = 4,
	AT_SGUARD = 8,

	AT_MARSHALL = 32,
	AT_DEPUTY	= 64,
	AT_PRISONER1 = 128,
	AT_PRISONER2 = 256,
	AT_PRISONER3 = 512,
	AT_GILLAM	=1024,
	AT_EVERYTHING=2<<16
};


class CCharacterCombatProfile
{
public:
	SINT	mInitialHealth ;
	SINT	mNumAttacksForCombo;
	SINT	mPunchStrenth;
	SINT	mBigPunchStrenth;
	SINT	mWeaponAttackStrength;
	SINT	mJumpAttackStrength;
	SINT    mChargeAttackStrength;
	SINT	mSpinAttackStrength;
	SINT	mElbowAttackStrength;
};


enum EPunchResult
{
	PR_MISSED,
	PR_HIT,
	PR_BLOCKED
};


class	CStateInit
{
public:
	CStateInit(SINT Timer = 100,
		       GVector &Dest  = ZERO_GVECTOR,
		       GVector &start = ZERO_GVECTOR)
	{
		mDest = Dest;
		mTimer = Timer;
		mStart = start;
	};

	GVector	mDest, mStart;
	SINT	mTimer;
};

#define DFC_Inwater		1
#define DFC_DrawTrail	2
#define DFC_InDeepWater 4

DECLARE_THING_CLASS(CCharacter, CActor)
public:
	virtual	void		Init(CInitThing *i);

	~CCharacter() ;


/*-----------------16/02/00 17:29-------------------
 * Added by Damo!
 * --------------------------------------------------*/
			CParticle* AddParticleEffect(EParticleType type)	{ return mParticleSystem.CreateParticle(type); };
/* --------------------------------------------------*/


	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	GINT		Radius() { return GINT(0, 49152);};

	virtual	SINT		WhichRTsToRender();
	virtual	CTrail		*GetRenderTrail()		{return &mTrail;};
	virtual	CParticleSystem	*GetRenderParticleSystem()		{return &mParticleSystem;};
	virtual	CParticleSystem *GetParticleSystem()		{return &mParticleSystem;};

	virtual	CCharacter	*IsCharacter() {return this;};

	virtual void		TriggerVibration(SINT number){};
	virtual	void		Damage(GINT amount);
	virtual void		AddStrikeVelocity(GVector vel, GINT max_vel) {} ;
	virtual SINT		InitialLife() { return mCombatProfile.mInitialHealth ; }
	virtual void		Heal(SINT amount) { if ((mLife+amount) <= InitialLife()) mLife+=amount ; else mLife = InitialLife() ;}
	virtual BOOL		Strike(const CStrike& strike);
			void		Stun(SINT for_time);

	virtual	BOOL				DoICollideWithMap();
	virtual	BOOL		DoIHit(CThing *t);
//	virtual	GINT		GetMapDamping()	{return G(0, 1000);};
	virtual	GINT		GetMapDamping();

	virtual	void		ValidateData() ;
	virtual	void		FinishedLevel() ;

	// state functions
	virtual	void		ProcessAnimation();
	virtual	void		Process();
	virtual void		StartDieProcess(BOOL hit_fan = FALSE);
			void		ScoreStrike(CThing* thing_we_hit);
	virtual void		StateNormal()		{};
	virtual	void		StateDead()			{};
	virtual void		StateKnockedOut() ;
	virtual	void		StateTeleport();
	virtual	void		StateAsleep();
	virtual void		StateFinished() {} ;
	virtual	void		StateScanning()		{};
	virtual void		StateStunned()	;
	virtual	void		StateBeingLasered() ;
	virtual	void		Shutdown() ;
	virtual	void		ChangeState(ECharState new_state, const CStateInit &i);
	virtual	void		ReachedCheckPoint(SINT check_point_num) ;
	virtual void		CatchOnFire() {} ;
	virtual BOOL		 IsStrikeable() { return TRUE ; }
	virtual	GINT		SizeofMeFactor() { return G1 ; }


			void		PushState();
			void		PopState();
			BOOL		IsStriking();
			BOOL		IsStrikingForward() ;
			BOOL		IsLongRangeStriking() ;

	virtual	void		HitParticle(EParticleType pt);
//	virtual	BOOL		DoIDrown() {return TRUE;};
	virtual	BOOL		DoIBurn() {return TRUE;};
	virtual BOOL		DoIGetLasered() { return TRUE ; }
	virtual BOOL		CanIPickupWeapons() {return TRUE;};

	virtual	GINT		Gravity();
	virtual	void		Hit(CThing *that);
	virtual	EAnimMode	GetRenderAnimation(){return mAnimMode;};
			BOOL		SetAnimMode(EAnimMode am, BOOL reset_frame = TRUE);
	virtual	GINT		GetRenderFrame();
	virtual SINT		GetRealAnimIndex()				{return mRealAnimIndex;};
	virtual BOOL		GetRenderCanGoFlatShaded()		{return FALSE;};
			CCharacter* GetCurrentTarget()				{return mCurrentTarget;};
			SINT		GetAttackID()			{return mMyAttackType;};
			void		SetMyAttackID(EAttackCharactersBF type)	{mMyAttackType |=type;};
	virtual SINT		GetRenderThingMiscFlags() { if( mDrawUsWhite) { mDrawUsWhite = FALSE ; return RTMF_WHITE ; } return 0 ; }
	virtual	SINT		GetInitialAttackID()	{return 0;};
	virtual	GINT		GroundFriction(EFrictionType friction_type);
	virtual	void		AnimTrigger(const GMatrix& current_anim_orientation,BOOL end_of_punch_hit) {};
	virtual	void		Thrust(const GVector& v) {} ;
//	virtual	SINT		GetCharacterThreatScore() { return 5 + (mWeapon==NULL ? 0 : 5) ; } ;
	virtual	BOOL		IsRacer() { return FALSE ;}
	virtual	BOOL		IsBadGuy() { return FALSE ; }
	virtual BOOL		IsGillam() { return FALSE ; }
	virtual	BOOL		Attack(SINT	no);
	virtual void		Throw() {} ;
			void		Drop(HTDropReason = CHARACTER_WAS_HIT ) ;
	virtual EAnimMode	GetNextAnimCharacter(EAnimMode am) { return GetNextAnim(am) ; }
			void		StartThrowingAnimation() ;

			void		StartBlocking() ;
			void		ReduceKnockoutTime(SINT amount) { if (mRecoverFromKnockoutTime > amount ) mRecoverFromKnockoutTime-=amount ; };

			void		StopBlocking();
			BOOL		IsBlocking() { return mIsBlocking ; }
			GINT		GetYaw() { return mYaw ; }
			SINT		GetScore() { return mScore ; }
			void		IncreaseScore(SINT amount) { mScore+=amount ; } ;
			void		DecreaseScore(SINT amount) { mScore-=amount ; } ;
						BOOL		CanIJump();
						GINT		GetRage() {return mRage;};
	virtual	void		Jump();

	virtual	void		Trigger(CThing *top);
	virtual	void		UnTrigger(CThing *top);

			void		NowBeenAttackedBy(CCharacter* character) {mBeingAttackedBy.Add(character) ; };
			void		NowNotBeenAttackedBy(CCharacter* character) {mBeingAttackedBy.Remove(character) ; };
	virtual	void		ClearYourTarget() ;
			BOOL		TooManyPeopleAttacking() { return mBeingAttackedBy.Size() >= NUM_OF_GUYS_ALLOWED_TO_ATTACK_ONE_GUY_AT_SAME_TIME ; } ;
		    const		CharacterList& GetBeingAttackedBy() { return mBeingAttackedBy ; };
			void		StopFurthestAwayAttacker() ;
			CCharacterLevelData& GetLevelData() { return mLevelData ; } ;
			void		StartWearingCrown() ;
			void		StopWearingCrown() ;
			CCrown*		GetCrown() { return mCrown ; }
			void		BeingLasered(GVector& hit_point) ;
			SINT		GetLastSpikeHitTime() { return mLastHitSpikeTime ; }
			void		SetLastSpikeHitTime(SINT time) { mLastHitSpikeTime = time ; }

	virtual	GMatrix		GetRenderOrientation(SINT no);


#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
	virtual	void		DrawDebugText(HDC hdc, SINT *y_offset, char* buffer) ;
#endif

#ifdef LINK_EDITOR
	virtual	void			DeclareSpawnees();
#endif

			ECharState	GetState() {return mState;};

	virtual void		SetDestYaw(GINT	yaw) {};

	virtual GINT		GetRenderScale();

	virtual	void		DeclareBoost();

	virtual	GVector			GetCarriedThingRenderPos();

protected:
	// State data
	SINT				mScore;
	ECharState			mState;
	EAnimMode			mAnimMode;
	GINT				mFrame;
	GINT				mFrameInc;
	SINT				mRealAnimIndex;

	GVector				mStateDest;
	GVector				mStateStart;
	SINT				mStateTimer;

	GVector				mStatePosStart;
	SINT				mStateTimerStart;

	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse;
	ETriggerResponse	mUnTriggerResponse;

	SINT				mAnimPauseCount ;


#define	MAX_STATE_STACK_DEPTH 10
	struct		SStateStack
	{
		ECharState	mState;

		GVector		mStateDest;
		SINT		mStateTimer;

		GVector		mStatePosStart;
		SINT		mStateTimerStart;
	}	mStateStack[MAX_STATE_STACK_DEPTH];

	SINT	mStateStackDepth;

	CThing		*mEffectThing;		// generic state effect

	GINT	mYaw;

	SINT	mInitialHoldingThingOID;

	CCharacter* mCurrentTarget ;
	SINT mToAttackBF;
	GINT mCharacterSight;
	SINT mLastFrameOnGround;
	SINT mLastFrameJumped;
	GINT mLastHeightOnGround;
	GINT				mRage ;

	SINT mCanBlockBF;

	CHoldableThing *mHoldingThing;

	SINT mMyAttackType ;

	SINT mCurrentPunchSequenceNum ;
	EPunchResult mLastPunchResult ;
	SINT mLastPunchFrameNum ;
	SINT mRecoverFromKnockoutTime;
	SINT mDefaultStunRecoveryTime ;
	SINT mCurrentStunRecoveryTime ;
	BOOL mIsBlocking ;

	CharacterList mBeingAttackedBy ;

	CCharacterLevelData mLevelData;

	CTrail	mTrail;
	CParticleSystem mParticleSystem;
	BOOL	mInvisible;

	enum	ETrailUse
	{
		TU_NONE = -1,
		TU_ATTACK = 0,
		TU_BALL = 1,
	} mLastTrailUse;
	SINT	mLastTrailUseTime;
	SINT	mBoostTrailCount;

	GVector	mLastFrameOnGroundPos;
	SINT	mLastHitSpikeTime;

	CCrown *mCrown;
	SINT	mLastFrameBeingLasered ;
	SINT mSpinStunRecoveryTime ;
	BOOL mDrawUsWhite;
	CCharacterCombatProfile mCombatProfile;
	CFXBodyFlamer* mOnFireFlames[2] ;
	SINT		mFlameGoOutTime ;
	
	
	GINT	mLastFrame;

	GMatrix	mLastOrientation;

};

#endif
