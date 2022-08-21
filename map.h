#ifndef MAP_C
#define MAP_C

#include	"Camera.h"
#include	"MemBuffer.h"
#include	"Thing.h"
#include	"Friction.h"

//******************************************************************************************
//const	SINT	MAP_SIZE_SHIFT = 6;
//const	SINT	MAP_SIZE	= (1 << MAP_SIZE_SHIFT);

//const	SINT	MAP_WIDTH	= MAP_SIZE;
//const	SINT	MAP_HEIGHT	= MAP_SIZE;
//====================-==--=--- --  -

#define MAP_WIDTH	(MAP.mMapWidth)
#define MAP_HEIGHT	(MAP.mMapHeight)

#define MAP_WIDTH_SHIFT		(MAP.mMapWidthShift)
#define MAP_HEIGHT_SHIFT	(MAP.mMapHeightShift)

//******************************************************************************************
class	CCellRef
{
public:
	CCellRef() {};
//	CCellRef(const FVector &v) {X = SINT(v.X); Y = SINT(v.Y);};
	CCellRef(const GVector &v) {X = v.X.Whole(); Y = v.Y.Whole();};
	CCellRef(const SINT x, const SINT y) {X = x; Y = y;};

	CCellRef	operator+( const CCellRef& c ) const { return CCellRef(X+c.X, Y+c.Y); };

	void	Clip();

	GVector	ToGVector()
	{
		return GVector(G(X), G(Y), G0);
	};

	SINT	X, Y;
};

#define	GVECTOR_HALF	GVector(GINT_HALF, GINT_HALF, G0)

//******************************************************************************************

#if TARGET == PC
enum	ECellType
{
	// THESE NEVER EVER CHANGE!

	CT_INVALID = -1,
	CT_NORMAL = 0,
	CT_ICE,
	CT_TRIGGER_GUN,
	CT_BOOSTR,
	CT_BOOSTL,
	CT_BOOSTU,
	CT_BOOSTD,
	CT_TRIGGER_GUN_ON,
	CT_TRIGGER_BAD_GUN,
	CT_TRIGGER_WAVE,
	CT_TRIGGER_WAVE_ON,
	CT_HEALTH,
	CT_HEALTH_ON,

	NUM_CELL_TYPES,
};
#endif
#if TARGET == PSX || TARGET == DC
#define CT_INVALID			-1
#define CT_NORMAL			0
#define CT_ICE				1
#define CT_TRIGGER_GUN		2
#define	CT_BOOSTR			3
#define	CT_BOOSTL			4
#define	CT_BOOSTU			5
#define	CT_BOOSTD			6
#define CT_TRIGGER_GUN_ON	7
#define CT_TRIGGER_BAD_GUN	8
#define	CT_TRIGGER_WAVE		9
#define CT_TRIGGER_WAVE_ON	10
#define CT_HEALTH			11
#define CT_HEALTH_ON		12
#endif
//******************************************************************************************
#define		PIT_HEIGHT	G(30)  // the very bottom of the level
#define		PIT_HEIGHT_SINT (30<<8)
//******************************************************************************************
// #defines for mFlags member of CMapCell....

#define F_LAYER_MASK (15)

// free bit.... (16)

#define F_FIXED (32)
#define F_REVERSE_SPLIT	(64)
#define F_REVERSE_SPLIT_WATER	(128)
//******************************************************************************************

#define HEIGHT_SHIFT (11)



class	CMapCell
{
	friend class CMap;
	friend class CPCMap;
	friend class CPSXMap;
	friend class CDCMap;
	friend class CPlayer;
//	friend class SPSXMeshPart; // for map lighting
	friend class CPSXMesh; // for map lighting
	friend class CDCMesh;
	friend class SDCMeshPart;

public:

	inline GINT		GetHeight() {GINT rv;rv.SetVal((mHeight+mDisplacement+mOffset)<<(16-HEIGHT_SHIFT));return rv;};
	void		SetHeight(GINT v) {mHeight = v.GetVal()>>(16-HEIGHT_SHIFT);};
	void		AddHeight(GINT v) {mHeight += v.GetVal()>>(16-HEIGHT_SHIFT);};

	GINT		GetUnderlyingHeight() {GINT rv;rv.SetVal(mHeight<<(16-HEIGHT_SHIFT));return rv;};

	DWORD		GetColour();

	GINT		GetOffsetVelocity() {GINT g;g.SetVal(mOffsetVelocity<<(16-HEIGHT_SHIFT));return g;};
	void		SetOffsetVelocity(GINT v) {mOffsetVelocity = (v.GetVal()>>(16-HEIGHT_SHIFT));};
	void		AddOffsetVelocity(GINT v) {mOffsetVelocity += (v.GetVal()>>(16-HEIGHT_SHIFT));};

	void		AddDisplacement(GINT v) {mDisplacement += (v.GetVal()>>(16-HEIGHT_SHIFT));};
	GINT		GetDisplacement() {GINT rv;rv.SetVal(mDisplacement<<(16-HEIGHT_SHIFT));return rv;};
	void		SetDisplacement(GINT v) {mDisplacement=(v.GetVal()>>(16-HEIGHT_SHIFT));};

	GINT		GetOffset() {GINT rv;rv.SetVal(mOffset<<(16-HEIGHT_SHIFT));return rv;};
	void		SetOffset(GINT v) {mOffset=(v.GetVal()>>(16-HEIGHT_SHIFT));};
	void		AddOffset(GINT v) {mOffset += (v.GetVal()>>(16-HEIGHT_SHIFT));};

	GINT		GetCombinedOffset() {GINT rv;rv.SetVal((mDisplacement+mOffset)<<(16-HEIGHT_SHIFT));return rv;};

#if TARGET == PC
	void		SetType(SINT t) {mType = ECellType(t);};
	ECellType	GetType() {return mType;};
	static		ECellType	GetCellSaveType(ECellType t);

	void			SetFriction(EFrictionType f) {mFriction = UBYTE(f);};
	EFrictionType	GetFriction() {return EFrictionType(mFriction);	};
#endif
#if TARGET == PSX || TARGET == DC
	void		SetType(SBYTE t) {mType = t;};
	SBYTE		GetType() {return mType;};
	static		SBYTE	GetCellSaveType(SBYTE t);

	// EXTREMELY CHEEKY - encode friction in bits 0 and 8 of colour....
	void			SetFriction(EFrictionType f)
	{
		mColour &= 0xfffffefe;
		mColour |=  UBYTE(f) & 1;
		mColour |= (UBYTE(f) & 0x2) << 7;
	};

	EFrictionType	GetFriction()
	{
		UBYTE	v = 0;
		v +=  mColour & 1;
		v += (mColour & 0x100) >> 7;
		return EFrictionType(v);
	};
#endif
	void		SetFixedFlag() {mFlags|=F_FIXED;};
	void		ClearFixedFlag() {mFlags&=~F_FIXED;};

	BOOL		CanIMove() {return ((mFlags&F_FIXED)?FALSE:TRUE);};
	void		SetReverseSplitFlag() {mFlags|=F_REVERSE_SPLIT;};
	void		ClearReverseSplitFlag() {mFlags&=~F_REVERSE_SPLIT;};
	BOOL		ReverseSplit() {return ((mFlags&F_REVERSE_SPLIT)?TRUE:FALSE);};
	void		SetReverseSplitFlagWater() {mFlags|=F_REVERSE_SPLIT_WATER;};
	void		ClearReverseSplitFlagWater() {mFlags&=~F_REVERSE_SPLIT_WATER;};
	BOOL		ReverseSplitWater() {return ((mFlags&F_REVERSE_SPLIT_WATER)?TRUE:FALSE);};

	void		SetTakeOff() {mRamp |= 0x80;};
	void		ClearTakeOff() {mRamp &= 0x7f;};
	BOOL		IsTakeOff() {return (mRamp & 0x80);};

	void		SetRampNo(SINT n) {mRamp &= 0x80; mRamp += n & 0x8f;};
	SINT		GetRampNo() {return mRamp & 0x7f;};

#if TARGET==PC
	GINT		GetFrame() {return mFrame;};
	void		SetFrame(GINT frame) {mFrame = frame;};

	DWORD		mColour;
#endif
#if 0
	void		SetShadow(UBYTE	b)
	{
		if (mShadow > b)
			mShadow = b;
	};
#endif

protected:
	DWORD		EvaluateColour(CMapCell *other);
	DWORD		EvaluateOriginalColour(CMapCell *other);

#if TARGET == PC
	SWORD		mHeight;
	SWORD		mDisplacement;
	SWORD		mOffset;
	SWORD		mOffsetVelocity;

	DWORD		mPrecalcColour;
	

	ECellType	mType;

	SINT		mFlags;

	GINT		mFrame; // sic!
/*-----------------01/10/1999 12:05-----------------
 * This value (mTextureNo) is used by the playstation engine.
 *  I reference it as a BYTE and use it as my mix flags
 * so I know if there's a texture on each layer....
 * Glen, when you come to optimize this structure, let me know....
 *                   Damian
 * --------------------------------------------------*/
	SINT		mTextureNo;

	UBYTE		mShadow;

	UBYTE		mMix;			// Needed for both versions, the real mixer!

	UBYTE		mRamp;			// top bit = is it a takeoff point?  other bits = ramp no.

	UBYTE		tex1, tex2, tex3;

	UBYTE		mFriction;
#endif

#if TARGET == PSX || TARGET == DC
	SWORD		mHeight;
	SWORD		mDisplacement;
	SWORD		mOffset;
	SWORD		mOffsetVelocity;
	ULONG		mColour;  // Cheeky - encoding friction in bits 0 and 8 of colour.....
	UBYTE		mFlags;
	UBYTE		mType;
	UBYTE		mMix;			// Needed for both versions, the real mixer!
	UBYTE		mRamp;
	//UBYTE		tex1, tex2, tex3;
#endif
};
class	Pin
{
public:
	SWORD	U;									// 11.21 signed, only shifted this far to make it
	SWORD	V;									// wrap at 4k, vital
	Pin() {};
	Pin(SWORD u, SWORD v) : U(u), V(v) {};

	Pin		operator+( const Pin& a ) const { return Pin(U+a.U,V+a.V); };
	Pin		operator-( const Pin& a ) const { return Pin(U-a.U,V-a.V); };
	void	operator+=( const Pin& a ) {U+=a.U;V+=a.V;};
	void	operator-=( const Pin& a ) {U-=a.U;V-=a.V;};
	Pin		operator>>( int a ) const { return Pin(U>>a,V>>a);};

};
//******************************************************************************************

const	SINT	MAX_MAP_TEXTURES = 256;

#ifdef _DEBUG
#define VERIFY_COORDS(c) MAP.VerifyCoords(c)
#else
#define VERIFY_COORDS(c)
#endif

#define COORDS_INVALID(c) MAP.CoordsInvalid(c)

// **NB - can't change these now without changing PSXPlatform.h
#define		MAP_LAYER_MIN	1       // = 2x2 big
#define		MAP_LAYER_MAX	5		//

#define		MAP_LAYER_NUM (MAP_LAYER_MAX - MAP_LAYER_MIN + 1)
#define		MAP_LAYER_SIZE_X(l) (1 << (MAP_WIDTH_SHIFT - (l)))
#define		MAP_LAYER_SIZE_Y(l) (1 << (MAP_HEIGHT_SHIFT - (l)))


class	CMap
{
public:
	friend class CMapCell;

	CMap()
	{
		mParticleSystem.Activate();
		mEffectParticleSystem.Activate();
		// zero out pointers
		mCells = NULL;

		SINT	c0;
		for (c0 = MAP_LAYER_MIN; c0 <= MAP_LAYER_MAX; c0 ++)
			mMapWhos[c0-MAP_LAYER_MIN] = NULL;
		mPinAnim = FALSE;
	};
	~CMap() {};


	// pinmap stuff
	BOOL	mPinAnim;

	// ** Initialisation

	BOOL	InitCommon();
	void	Shutdown();

	void	Reset();

	void	ClearMap(GINT phase, GINT height);
	void	PrecalcCells();
	void	RaiseEdges();
	void	LockEdges();
	void	ResetOffsets();

#if TARGET == PC
	int		RayScan(GVector* pos, GVector* step, SINT steps);
	int		NRayScan(GVector* pos, GVector* step, SINT steps);
	GINT	GetCludgedComponent(SINT xc, SINT yc, SINT xoff, SINT yoff);
	void	RelightLevel();
	void	UnlightLevel();
	void	LightCells();
#endif

	// ** Access

	CMapCell	*GetCell(const CCellRef &c)
	{
		SINT X	= c.X;
		SINT Y	= c.Y;

		// Clip to nearest cell.
		if (X < 0)
			X = 0;

		if (X >= mMapWidth)
			X = mMapWidth - 1;

		if (Y < 0)
			Y = 0;

		if (Y >= mMapHeight)
			Y = mMapHeight - 1;

		//ASSERT((c.X >= 0) && (c.Y >= 0) && (c.X < mMapWidth) && (c.Y < mMapHeight));
        return &mCells[(Y << mMapWidthShift) + X];
	};

	// Using absolute CELL x/y's
	CMapCell	*GetCell(SINT x,SINT y)
	{
		// Clip to nearest cell.
		if (x < 0)
			x = 0;

		if (x >= mMapWidth)
			x = mMapWidth - 1;

		if (y < 0)
			y = 0;

		if (y >= mMapHeight)
			y = mMapHeight - 1;

		//ASSERT((x >= 0) && (y >= 0) && (x < mMapWidth) && (y < mMapHeight));
		return &mCells[(y << mMapWidthShift)+x];
	}

	// Using GINT WORLD x/y's
	CMapCell	*GetCell(GINT x,GINT y)
	{
		SINT X = (x.mVal>>16);
		SINT Y = (y.mVal>>16);

		// Clip to nearest cell.
		if (X < 0)
			X = 0;

		if (X >= mMapWidth)
			X = mMapWidth - 1;

		if (Y < 0)
			Y = 0;

		if (Y >= mMapHeight)
			Y = mMapHeight - 1;

		return &mCells[(Y<<mMapWidthShift)+X];
	}


	void		PinMap(Pin* pin, int pinmod, CMapCell* cell, int mapmod, int xpins, int ypins);
	void		NonSHMProcess();
	void		Process();
	GINT		GetMapHeightDifference(const GVector &v);
	GINT		GetMapHeightOriginal(const GVector &v);
	GINT		GetMapHeight(const GVector &v);
	GINT		GetMapHeightOffset(const GVector &v);
	GVector		GetMapNormal(const GVector &v);
	GINT		GetMapVelocity(const GVector &v);

	void		ProcessPursuit();

	void		ClipPosition(GVector &p);

	// Effects
	void		Raindrop(const CCellRef &c, GINT power = G1);

	GINT		GetLowestPoint()  {return mLowestPoint;};

	BOOL		FallenOffMap(GVector v);

	// shadow stuff
	void		AddShadowCuboid(GVector &pos, GVector &axes, GMatrix &ori);

	// ***********************
	// Mapwho stuff
	void		AddToMapWho(CThing *t);
	void		RemoveFromMapWho(CThing *t);
	void		RemoveFromMapWhoSizeUnknown(CThing *t); // needed, sadly
	void		MoveMapWho(CThing *t, GVector dest);

	// These commands operate similarly to DOS FindFirstFile()/FindNextFile() - ie they are static and not recursive.
	CThing		*GetFirstThing(const GVector &pos, GINT radius);
	CThing		*GetNextThing();

//	CThing		*GetFirstThing(CCellRef &c);

	SINT	mMapWidth;
	SINT	mMapHeight;
	SINT	mMapWidthShift;
	SINT	mMapHeightShift;

	void	VerifyCoords(const CCellRef &c) {ASSERT(((c.X) >= 0) && ((c.Y) >= 0) && ((c.X) < mMapWidth) && ((c.Y) < mMapHeight));};
	BOOL	CoordsInvalid(const CCellRef &c) {return (!(((c.X) >= 0) && ((c.Y) >= 0) && ((c.X) < mMapWidth - 1) && ((c.Y) < mMapHeight - 1)));};

	GVector		FindLineIntersection(GVector start, GVector end);
	CParticleSystem *GetEffectParticleSystem() {return &mEffectParticleSystem;};

protected:

	// ********************************************************8
	CMapCell	*mCells;

	// mapwho stuff
	CThing	**mMapWhos[MAP_LAYER_NUM];

	// For GetFirstThing()/GetNextThing()
	GVector	mGTPosition;
	GINT	mGTRadius;

	SINT	mGTCurrentLayer;
	CThing	*mGTCurrentThing;

	SINT	mXStart, mYStart;
	SINT	mXEnd, mYEnd;
	SINT	mCX, mCY;

	void	AddToMapWhoLayer     (SINT layer, CThing *t);
	BOOL	RemoveFromMapWhoLayer(SINT layer, CThing *t);
	SINT	ChooseLayer(GINT radius);

	// Handy ways of finding cells
	CThing		**GetMapWho(SINT layer, SINT x, SINT y)
	{
		return mMapWhos[layer - MAP_LAYER_MIN] + (y << (mMapWidthShift - layer)) + x;
	};

	CThing		**GetMapWho(SINT layer, CCellRef &c)
	{
#ifdef _DEBUG
		VerifyCoords(c);
#endif

		return mMapWhos[layer - MAP_LAYER_MIN] + ((c.Y >> layer) << (mMapWidthShift - layer)) + (c.X >> layer);
	};

	// ********************************************************8
	GINT			mLowestPoint;

	void			SetCellTNo(CCellRef &c, SINT n);
	// ********************************************************8
	SWORD			mFadeWave[4096];
	SWORD			mPinScroll;
	SWORD			mPinScrollPos;

	CParticleSystem mParticleSystem;
	CParticleSystem mEffectParticleSystem;


};

//******************************************************************************************


#endif
