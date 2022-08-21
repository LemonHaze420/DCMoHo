#ifndef PLAYER_H
#define PLAYER_H

#include	"Action.h"
#include	"Map.h"

// ********************************************************8
enum ECursorState
{
	CS_NOT_ACTIVE,
	CS_ACTIVE_TOOL,
	CS_SHRINKING_TOOL,
	CS_ACTIVE_OTHER,
};


// ********************************************************8
enum ELevelResult
{
	LR_NONE,
	LR_WIN,
	LR_LOSE,
	LR_DRAW
};

// ********************************************************8
enum EPickupBeamState
{
	NO_BEAM,
	GROWING,
	FULL_BEAM,
	SHRINKING
};

// ********************************************************8
#define CGM_UP		0
#define CGM_DOWN	1
#define CGM_TOPPED  2

// ********************************************************8
class	CPlayer
{
public:
	friend class CPCMap;
	friend class CPSXMap;
	friend class CDCMap;

	CPlayer() {};
	~CPlayer() {};

	BOOL	Init();

	void	ProcessAction(EAction action, GINT v1 = G1, GINT v2 = G1);

	BOOL	IsActive() {return mActive;};
	void	Activate() {mActive = TRUE;};
	void	DeActivate() {mActive = FALSE;};

	void	Process();


#ifdef LINK_EDITOR
	static	void			DeclareSpawnees();
#endif

protected:
	SINT	mCurrentWeapon;
	SINT	mActive;


	// ********************************************************8
	// Cursor stuff
public:
	CCellRef	GetCursor()			{return mCursor;};
	GVector		GetAnalogCursor()	{return mAnalogCursor;};

	void		ActivateCursor();
	void		DeActivateCursor();

	void		EvaluateCursorPosition();

	void		RecentreCursor();

	static		void		PickupSubCell(const GVector &v, GINT power);

	SINT		mNumber;

	CPrimary*	GetPrimary() { return mPrimary ; } ;
	void		SetPrimary(CPrimary* to) { mPrimary = to ; } ;
	ELevelResult GetLevelResult() { return mLevelResult ; }
	void	    SetLevelResult(ELevelResult result);

	SINT		GetResultTime() {return mResultTime;};

	GINT				GetPickupBeamSize() {return mPickupBeamSize;};
	EPickupBeamState	GetPickupBeamState() {return mPickupBeamState;};

private:
	void		ProcessCursor();
	void		ProcessPickupTool();
	void		PlayShot();
	CThing		*FindThingUnderCursor();


	ECursorState		mCursorState;
	SINT				mCursorStateTime;

	EPickupBeamState	mPickupBeamState;
	GINT				mPickupBeamSize;
	SINT				mPickupBeamStartTime;

	CCellRef			mPickup;
	CCellRef			mCursor;

	GVector				mAnalogCursor;
	GVector				mAnalogCursorVelocity;
	BOOL				mHasAnalogCursorFoundPrimary;
	BOOL				mReleasedBlock;		
	ELevelResult		mLevelResult ;
	
	GVector				mAnalogPickup;

	GINT				mCursorYaw; // for golf things
	GINT				mCursorDist;
	SINT				mCursorGolfMode;

	GINT				mRelX;	// relative joystick mode
	GINT				mRelY;

	CPrimary*			mPrimary ;

	SINT				mResultTime; // JCL turn when levelresult was set.

	// ********************************************************8
	// racer stuff

public:
	GINT		GetBoostVal();

protected:
	SINT		mRacerBoost;
};

#endif