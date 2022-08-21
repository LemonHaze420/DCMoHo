#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#if TARGET != PSX
#define csin(x) SINT(sin(float(x) * 0.0015339807878856412297180875894917f)*4096.f)
#define ccos(x) SINT(cos(float(x) * 0.0015339807878856412297180875894917f)*4096.f)
#endif

#define PART_TILEH 0x400
#define PART_TILEV 0x800
//************************************************************************
enum	EParticleType
{
	PART_NONE,
	PART_FIRE1,
	PART_FIRE2,
	PART_FIRE3,
	PART_BODY_FIRE,
	PART_Z,

	/*-----------------16/02/00 15:32-------------------
	 * All PART_'s from 100 onwards are special effects
	 * --------------------------------------------------*/
	PART_RING = 100,
	PART_COLUMN,
	PART_NECOLUMN,
	PART_WATERRING,
	PART_WATERRING_SMALL,
	PART_TELEPORTINCOLUMN,
	PART_TELEPORTINRING,
	PART_RAY,
	PART_STAR1,
	PART_STAR2,
	PART_ANIM,
	PART_SMOKE,
	PART_FLASH,
	PART_HEMI,
	PART_SPARK,
	PART_SPLASH,
	PART_SPINSPARK,
//	PART_TELEPORTSPARK,
	PART_WRING2,
	PART_SHOCKWAVE,
	PART_LIGHTNING,
	PART_LIGHT
};

//! GRrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
#if TARGET == PC
typedef struct {		/* short word type 3D vector */
	short	vx, vy;
	short	vz, pad;
} SVECTOR;
#endif

class CParticle;

struct _SSpark{
	SVECTOR mQueue[5]; //!!JCL - shortened
	SVECTOR mVel;
	short	mBrightness;
};

struct  _SZed{
};

struct  _SFire{
};

struct  _SRay{
	short	mSegs;
	short	mStep;
	short	mSpin;
	short	mOffs;
	short	mLength;
	short	mHeight;
	short	mBrightness;
};

struct  _SRing{
	short	mSegs;
	short	mStep;
	short	mSpin;
	short	mOffs;
	short	mWidth;
	short	mHeight;
	short	mBrightness;
	short	mTextureNum;                /* or 0 if non-textured */
};

struct  _SColumn{
	short	mSegs;
	short	mStep;
	short	mSpin;
	short	mOffs;
	short	mWidth;
	short	mHeight;
	short	mBrightness;
	short	mTextureNum;                /* or 0 if non-textured */
};
struct  _SHemi{
	short	mBrightness;
};
struct _SStar {
	short	mBrightness;                // a PSX style 12 bit number (0-1)
};

struct _SAnim {
	short mCs;
	short mSn;
	short mRate;
	short mTick;
	short mFrame;
	short mFrames;
	short mBrightness;
	char  mLoops;
	char  mHoriz;
};

//************************************************************************
class	CParticle
{
public:

	EParticleType	mType;
	short			mLife;
	short			mRadius;
	short			mDelay;
	short			mIsRelative;
	GVector			mPos;
	GVector			mVelocity;
#if TARGET == PC
	DWORD			mColour; //!Hmm.....
#else
	union
	{
		DWORD	mColour;
		CVECTOR mCVEC;
	};
#endif
	union
	{
		struct _SZed 	uPZed;
		struct _SFire 	uPFire;
		struct _SRay 	uPRay;
		struct _SRing 	uPRing;
		struct _SColumn uPColumn;
		struct _SStar	uPStar;
		struct _SAnim	uPAnim;
		struct _SHemi	uPHemi;
		struct _SSpark	uPSpark;
	};
};

// Pass as third parameter
enum EeHitType
{
	HT_NORMAL=1,
	HT_BLOCK,
	HT_WEAPON,
	HT_JUMP,
	HT_BULLETGROUND,
	HT_BULLETCHARACTER,
	HT_BALLGROUND,
	HT_LASERED,
};


//************************************************************************
#define		MAX_PARTICLES 32

class CThing;
class	CParticleSystem
{
public:
	CParticleSystem()	{mActive = FALSE;mOwner=NULL;};
	~CParticleSystem()	{};

	void		Activate();
	void		DeActivate()	{mActive = FALSE;};

	BOOL		IsActive()		{return mActive;};

	CParticle	*CreateParticle(EParticleType type);

	void		MoveAllParticles(GVector* by) ;
	void		MakeExplosion1(GVector *pos);
	void		MakeExplosion1Small(GVector *pos);
	void		MakeExplosion2(GVector *pos);

	void		MakeSplash(GVector *pos);
	void		MakeSplash2(GVector *pos, SINT num, GINT baserad, GINT spread);		// directional
	void		MakeSparks(GVector *pos);

	void		MakeHitEffect(GVector *pos, GINT baserad,EeHitType type);

	BOOL		Process();


	BOOL		mActive;

	CThing		*mOwner;
	CParticle	mParticles[MAX_PARTICLES];
};

#endif
