// $Header$

// $Log$
// Revision 1.5  2000-10-06 15:27:43+01  jcl
// <>
//
// Revision 1.4  2000-10-04 12:18:42+01  jjs
// Removes PC memory leaks.
//
// Revision 1.3  2000-08-07 15:47:11+01  jjs
// DC and PC merged code.
//
// Revision 1.2  2000-07-11 10:48:28+01  jjs
// Commented out CSH_COMPLEX as per Lost Toys version.
//

#ifndef THING_H
#define THING_H

//******************************************************************************************

#include	"OIDs.h"
#include	"PickupTypes.h"
#include	"MemBuffer.h"
#include	"CollisionVolume.h"
#include	"ParticleSystem.h"
#include	"animation.h"
#include	"Strike.h"

class	CWorld;
class	CRenderThing;
class	CSpringArray;
class	CParticleSystem;
class	CProjectile;
class	CCheckPoint ;

enum	EAnimMode;

//******************************************************************************************

#define	TF_ON_GROUND				(1<<0)
#define	TF_RENDER_IN_EDITOR_ONLY	(1<<1)
#define	TF_FLASHING					(1<<2)
#define	TF_SUICIDE					(1<<3) // about to go
#define	TF_SUICIDE_NEXT				(1<<4) // nearly about to go
#define	TF_DEBUG_SUICIDE			(1<<5) // debug for suicide check
#define	TF_HIT						(1<<6) // was hit in last frame
#define TF_HIT_CHECK				(1<<7) // was checked in last frame
#define TF_IN_MAPWHO				(1<<8) // in the mapwho system
#define TF_DEAD						(1<<9) // I'm dead and I know I am!
#define	TF_ON_OBJECT				(1<<10) // standing on a object that behaves like the ground
#define TF_ON_FIRE					(1<<11) // On fire ??
#define TF_SLOW_FLASHING			(1<<12) // Pre Flashing kinda thing
#define TF_NOT_ALLOWED_TO_MOVE      (1<<13) // so child of actor's can stop things (i.e. take control over gravity etc) 

//******************************************************************************************
#define	VALIDATE(p) {if (p && (!((p)->IsValid()))) p = NULL;}

//******************************************************************************************

enum	ECollideShape
{
	CSH_NONE = 0,
	CSH_SPHERE = 1,
//	CSH_COMPLEX = 2,
	CSH_PARTICLE_SYSTEM = 3,
	CSH_DOUBLE_SPHERE = 4,
	CSH_FENCE = 5,
	CSH_FENCE_Y_ALLIGN = 6,
	CSH_FENCE_X_ALLIGN = 7,
	CSH_AABB = 8,
	CSH_OBB = 9,
	CSH_OBB2 = 10,
	CSH_OBB3 = 11,
	CSH_OBBMOVING2 = 12

};

#define BYTE_PAIR(a, b) ((SINT(a) << 8) + (b))

enum	ECollideResponse
{
	CR_BOUNCE,
	CR_STATIC,
	CR_PASSIVE,
};


enum	EAngleType
{
	AT_YPR,
	AT_ORI,
};

enum	EInitialMode
{
	IM_UNUSED = 0,
	IM_ACTIVE,
	IM_INACTIVE,

	IM_MAX,
};

enum	ETriggerResponse
{
	TR_UNUSED = 0,
	TR_NONE,
	TR_ACTIVATE,
	TR_DEACTIVATE,
	TR_TOGGLE,

	TR_MAX
};

//******************************************************************************************
#define	NUM_COUNTERS	4
#define	NUM_BOOLS		4

class	CInitThing
{
public:
	CInitThing();

	// if this is changed - increase version number in load/save
	GVector				mPos;
	GVector				mVelocity;

	EAngleType			mAngleType;
	GMatrix				mOrientation;
	GINT				mYaw, mPitch, mRoll;

	SINT				mOID;

	SINT				mTarget, mChainTrigger;

	EInitialMode		mInitialMode;
	ETriggerResponse	mTriggerResponse, mUnTriggerResponse;

	SINT				mCounters[NUM_COUNTERS];
	SINT				mBools[NUM_BOOLS];

	SINT				mMeshNo;

};

//******************************************************************************************
// some interesting macros
#define DECLARE_ZERO_GRAVITY	virtual	GINT Gravity() {return G0;}

//******************************************************************************************
// some more interesting macros for the editor

#ifdef LINK_EDITOR

#define TED_COUNT0(s)	virtual char	*GetTEDCount0() {return s;}
#define TED_COUNT1(s)	virtual char	*GetTEDCount1() {return s;}
#define TED_COUNT2(s)	virtual char	*GetTEDCount2() {return s;}
#define TED_COUNT3(s)	virtual char	*GetTEDCount3() {return s;}

#define TED_BOOL0(s)	virtual char	*GetTEDBOOL0() {return s;};
#define TED_BOOL1(s)	virtual char	*GetTEDBOOL1() {return s;};
#define TED_BOOL2(s)	virtual char	*GetTEDBOOL2() {return s;};
#define TED_BOOL3(s)	virtual char	*GetTEDBOOL3() {return s;};

#else

#define TED_COUNT0(s)
#define TED_COUNT1(s)
#define TED_COUNT2(s)
#define TED_COUNT3(s)

#define TED_BOOL0(b)
#define TED_BOOL1(b)
#define TED_BOOL2(b)
#define TED_BOOL3(b)

#endif

//******************************************************************************************
// Macros to help object properties
#define GET_OID						mOID = i->mOID
#define GET_TARGET					WORLD.GetThing(i->mTarget, &mTarget)
#define	GET_COUNTER(x)				mCounter##x = i->mCounters[x]
#define	GET_BOOL(x)					mBool##x = i->mBools[x]
#define	GET_INITIAL_MODE			{mInitialMode = i->mInitialMode; if (mInitialMode == IM_UNUSED) mInitialMode = IM_ACTIVE;}
#define GET_TRIGGER_RESPONSE		{mTriggerResponse = i->mTriggerResponse;}
#define GET_UNTRIGGER_RESPONSE		{mUnTriggerResponse = i->mUnTriggerResponse;}

#define WRITE_OID					{if (mOID == OID_INVALID) i.mOID = OID_NONE;	else i.mOID = mOID;}
#define WRITE_TARGET				{if (mTarget) i.mTarget = WORLD.GetWriteRef(mTarget); else i.mTarget = -1;}
#define	WRITE_COUNTER(x)			{i.mCounters[x] = mCounter##x;}
#define	WRITE_BOOL(x)				{i.mBools[x] = mBool##x;}
#define WRITE_INITIAL_MODE			{i.mInitialMode = mInitialMode;}
#define WRITE_TRIGGER_RESPONSE		{i.mTriggerResponse = mTriggerResponse;}
#define WRITE_UNTRIGGER_RESPONSE	{i.mUnTriggerResponse = mUnTriggerResponse;}

//******************************************************************************************
// forward definitions
class	CCharacter;
class	CPrimary;
class	CParticleSystem;
class	CSwitch;
class	CWeapon;
class	CHoldableThing;
class	CTrail;
class	CEditorLight;
class   CCharBall;
class	CLaserBeam;

//******************************************************************************************

#define	RTMF_WHITE		1
#define	RTMF_ONTOP		2
#define	RTMF_NOLIGHT	4
#define RTMF_BLACK		8

class	IRenderableThing
{
public:
	virtual	GVector			GetRenderPos(SINT no = 0) = 0;
	virtual	GMatrix			GetRenderOrientation(SINT no) = 0;
	virtual	BOOL			GetRenderSelected() = 0;
	virtual	SINT			GetRenderColourOffset() = 0;
	virtual	DWORD			GetRenderColour() = 0;
	virtual GINT			GetRenderTurn() = 0;
	virtual GINT			GetRenderFrame() = 0;
	virtual SINT			GetRealAnimIndex() = 0;
	virtual	GINT			GetRenderRadius() = 0;
	virtual	GVector			GetRenderStartPos() = 0;
	virtual	GVector			GetRenderEndPos() = 0;
	virtual SINT			GetRenderThingMiscFlags() = 0;
	virtual	CSpringArray	*GetRenderSpringArray() = 0;
	virtual	CParticleSystem	*GetRenderParticleSystem() = 0;
	virtual	SINT			GetRenderMesh() = 0;
	virtual	EAnimMode		GetRenderAnimation() = 0;
	virtual	CTrail			*GetRenderTrail() = 0;
	virtual	BOOL			GetRenderRotateShadow() = 0;
	virtual BOOL			GetRenderCanGoFlatShaded() = 0;
	virtual	BOOL			RenderUseHierarchy(SINT pn) = 0;
	virtual BOOL			RenderRenderPart(SINT pn) = 0;
	virtual	GINT			GetRenderYaw() = 0;
	virtual GINT			GetRenderScale() = 0;
};

//******************************************************************************************
DECLARE_THING_CLASS(CThing, IRenderableThing)
public:
	friend class	CWorld;
	friend class	CPCWorld;
	friend class	CPSXWorld;
	friend class	CDCWorld;
	friend class	CMap;
	friend class	CJCLEngine;
	friend class	CPSXEngine;
	friend class	CDCEngine;
	friend class	CPlayer;

	CThing() {};
	virtual	~CThing();
#if TARGET != PC
	void	*operator	new(size_t size);
	void	operator	delete(void *mem);
#endif	

	virtual	void		Init(CInitThing *i);
			void		InitRenderThing();
	virtual	void		Shutdown();
			void		Suicide();
	virtual void		SuicideNext();
	virtual void		Die() {SuicideNext();};

#if TARGET == PC
	virtual	void		WriteData(CInitThing &i);
#endif

	virtual	void		ValidateData() {VALIDATE(mChainTrigger);};

	virtual	void		Process();
			void		Render();
			void		RenderCollisionVolume(SINT i);

	virtual	SINT		WhichRTsToRender() {return 1;};

			SINT		GetThingNumber() {return mNumber;};

	// collision
	virtual	GINT		Radius() { return GINT_HALF;};
	virtual	GINT		Radius2() { return GINT_HALF;};  // could make this parametric, I suppose
	virtual	GINT		RadiusForMapWho() {return Radius();};
	virtual	BOOL		DoIHit(CThing *t) {return TRUE;};
	virtual	void		Hit(CThing *that) {};
	virtual	BOOL		DoISeekCollisions() {return FALSE;};
			void		SetHitFlag()		{mFlags |= TF_HIT;};
			void		SetHitCheckFlag()	{mFlags |= TF_HIT_CHECK;};
	virtual	void		HitParticle(EParticleType pt) {};
	virtual	BOOL		Strike(const CStrike& strike) { Damage(G(0,strike.mStrength*2632)) ; return TRUE; };

	virtual ECollideShape		GetCollideShape()			{return CSH_SPHERE;};
	virtual	ECollideResponse	GetCollideResponse()		{return CR_STATIC;};
	virtual CCollisionVolume	*GetComplexCollisionVolume();

	virtual	GVector		GetCollisionNormal() {return GVector(G0, G1, G0);}; // for fences etc

	virtual	GINT		CoE() {return G(0, 1000);};		// coefficient of elasticity
	virtual	GINT		CoF() {return G(0, 1967);};					// coefficient of friction

	// application
	virtual	void		ApplyPickup(EPickupType pt, GINT amount) {};
	virtual	void		Damage(GINT amount) {};

	virtual	GVector		GetOldPos() {return mPos;};
	virtual	GVector		GetPos(SINT n = 0) {return mPos;};
			CThing		*GetNextThing() { return mNextThing;};
			const		GMatrix		&GetOrientation() {return mOrientation;};
			void		SetOrientation(GMatrix &m) {mOrientation = m;};

	virtual	GVector		GetVelocity(SINT no = 0) {return ZERO_GVECTOR;};
	virtual	void		SetVelocity(GVector v) {};
	virtual	void		Move(GVector dest);

	virtual	BOOL		DoIRotate() {return FALSE;};
	virtual GMatrix		GetAngularVelocity(SINT no) {return ID_GMATRIX;};

	virtual	void		Activate() {};
	virtual	BOOL		CanIBeActivated() {return FALSE;};

	virtual	BOOL		IsPersistent() {return FALSE;};
	virtual	BOOL		IsPCOnly() {return FALSE;};
	virtual	BOOL		IsValid()	{return (!(mFlags & TF_SUICIDE)) && (!(mFlags & TF_SUICIDE_NEXT));};

			void		StickToGround();
			void		SitOnGround();
			void		ClipToGround();

			UINT		GetFlags() { return mFlags ; } ;
	// triggering
	virtual	void		Trigger  (CThing *top) {if (mChainTrigger && mChainTrigger != top) mChainTrigger->Trigger(top);};
	virtual	void		UnTrigger(CThing *top) {if (mChainTrigger && mChainTrigger != top) mChainTrigger->UnTrigger(top);};
			CThing		*GetChainTrigger() {return mChainTrigger;};

	virtual	CThing		*GetTarget() {return NULL;};
	virtual	void		SetTarget(CThing *t) {};

	virtual	CParticleSystem *GetParticleSystem() {return NULL;};

	// type properties
	virtual	CCharacter		*IsCharacter() {return NULL;};
	virtual	CPrimary		*IsPrimary() {return NULL;};
	virtual	CProjectile		*IsProjectile() {return NULL;};
	virtual	CSwitch			*IsSwitch() {return NULL;};
	virtual	CWeapon			*IsWeapon() {return NULL;};
	virtual	CHoldableThing	*IsHoldable() {return NULL;};
	virtual CCheckPoint		*IsCheckPoint() { return NULL;};
	virtual	CLaserBeam		*IsLaserBeam() {return NULL;};
	virtual	CEditorLight	*IsLight()	{return NULL;};
	virtual BOOL			 IsStrikeable() { return FALSE ; }

	virtual	BOOL		DoIAcceptPickups()	{return FALSE;};
	virtual	SINT		StrikeScore() {return 0;};

	// other properties
	virtual	SINT		InitialLife() {return 100;};
			SINT		GetLife() {return mLife;};

			SINT		GetNumber()	{return mNumber;};

	// for rendering
	virtual	GVector			GetRenderPos(SINT no = 0)		{if (no == 0) return mPos; else return ZERO_GVECTOR;};
	virtual	GMatrix			GetRenderOrientation(SINT no)	{if (no == 0) return mOrientation; else return ID_GMATRIX;};
	virtual	BOOL			GetRenderSelected()				{return FALSE;};
	virtual	SINT			GetRenderColourOffset()			{return 0;};
	virtual	DWORD			GetRenderColour()				{return 0xffffffff;};
	virtual GINT			GetRenderTurn();
	virtual GINT			GetRenderFrame()				{return G0;};
	virtual SINT			GetRealAnimIndex()				{return 0;};
	virtual	GINT			GetRenderRadius()				{return Radius();};
	virtual	GVector			GetRenderStartPos()				{return mPos;};
	virtual	GVector			GetRenderEndPos()				{return mPos;};
	virtual	CSpringArray	*GetRenderSpringArray()			{return NULL;};
			SINT			GetRenderHitFlags()				{return mFlags & (TF_HIT | TF_HIT_CHECK | TF_ON_GROUND);};
	virtual	CParticleSystem	*GetRenderParticleSystem()		{return NULL;};
	virtual	SINT			GetRenderMesh()					{return -1;};
	virtual	EAnimMode		GetRenderAnimation()			{return (enum EAnimMode)0;};
	virtual	CTrail			*GetRenderTrail()				{return NULL;};
	virtual	BOOL			GetRenderRotateShadow()			{return FALSE;};
	virtual BOOL			GetRenderCanGoFlatShaded()		{return TRUE;};
	virtual	GINT			GetRenderYaw()					{return G0;};
	virtual SINT			GetRenderThingMiscFlags() 		{return 0;}
	virtual GINT			GetRenderScale()				{return G1;};


	//! temporary!
	virtual	BOOL			RenderUseHierarchy(SINT pn)		{return TRUE;};
	virtual BOOL			RenderRenderPart(SINT pn)		{return TRUE;};

	// animation stuff GC

			GINT			GetRenderThingFrameIncrement(EAnimMode am, SINT* realindex);

	// from renderthing
			GINT			GetRenderThingRadius();
			GVector			GetRenderThingEmitter(SINT no);
			GVector			GetRenderThingEmitterAnimated(SINT no);

	// for interpolating the position of carried things.
	virtual	GVector			GetCarriedThingRenderPos() {return mPos;};

#ifdef LINK_EDITOR
	// data accumulation stuff
	virtual	void			DeclareSpawnees() {};
	virtual BOOL			DoesThingContainResources() {return FALSE;};
#endif

	// editor string stuff
	TED_COUNT0(NULL)
	TED_COUNT1(NULL)
	TED_COUNT2(NULL)
	TED_COUNT3(NULL)
	TED_BOOL0(NULL)
	TED_BOOL1(NULL)
	TED_BOOL2(NULL)
	TED_BOOL3(NULL)

protected:
	CThing			*mNextThing;
	CThing			*mPrevThing;

	CThing			*mMapWhoNext;

	GVector			mPos;
	GMatrix			mOrientation;

	UINT			mFlags;

	SINT			mNumber;

	SINT			mLife;

	CThing			*mChainTrigger; // when triggered, also trigger this

private:
#if TARGET == PC
	CRenderThing	*mRenderThing;
#endif

#ifdef LINK_EDITOR
	// shit - need to do this for the editor
	GINT	mEditorYaw, mEditorPitch, mEditorRoll;
#endif
};


//******************************************************************************************

#endif
