// $Header$

// $Log$
// Revision 1.2  2000-07-11 10:23:47+01  jjs
// Sean added "common.h"
//

#ifndef LEVEL_ED2_H
#define LEVEL_ED2_H

#if EDITOR_VERSION == 2

#include	"Map.h"
#include	"Controls.h"
#include     "common.h"

//******************************************************************************************
inline	UWORD	RGBToWord(SINT r, SINT g, SINT b)
{
	//! hm..
//	return ((b >> 3) << 11) + ((g >> 2) << 5) + (r >> 3);   //565
	return ((r >> 3) << 10) + ((g >> 3) << 5) + (b >> 3);   //555
}

//******************************************************************************************
inline	UWORD	ToGreyScale(UWORD c)
{
	SINT v = ((c >> 10) & 31) + ((c >> 5) & 31) + (c & 31);
	v /= 3;

	return (v << 10) + (v << 5) + v;
}

//******************************************************************************************
inline	UWORD	Colourize(UWORD c, SINT r, SINT g, SINT b)
{
	// colourize pixel based on intensity.
	SINT v = ((c >> 10) & 31) + ((c >> 5) & 31) + (c & 31);
	v /= 3;

	return (((v * r) / 256) << 10) + (((v * g) / 256) << 5) + ((v * b) / 256);
}

//******************************************************************************************
inline	UWORD	Blend(UWORD c, UWORD d, UWORD ori)
{
	// get intensity
	SINT v = ((ori >> 10) & 31) + ((ori >> 5) & 31) + (ori & 31);
	v /= 3;
	SINT u = 32 - v;

	// Blend pixel based on intensity.
	SINT	sr = ((c >> 10) & 31);
	SINT	sg = ((c >> 5) & 31);
	SINT	sb = (c & 31);

	SINT	dr = ((d >> 10) & 31);
	SINT	dg = ((d >> 5) & 31);
	SINT	db = (d & 31);

	return ((((sr * v) + (dr * u)) / 32)  << 10) + ((((sg * v) + (dg * u)) / 32) << 5) + (((sb * v) + (db * u)) / 32);
}

//******************************************************************************************

class	CIconBank
{
public:
	CIconBank(char *fname);
	CIconBank(char *fname, SINT across); // specify that there is more than 1 icon across
	~CIconBank();

	BOOL	IsValid() {return mNumIcons != -1;};

	void	GetIcon(SINT no, UWORD *dest, SINT pitch);
	UWORD	*GetIconData(SINT no)
	{
		return mData + no * mIconSize * mIconSize;
	};

	SINT	GetSize() {return mIconSize;};

	SINT	GetIconWidth(SINT num);

protected:
	UWORD	*mData;
	SINT	mNumIcons;
	SINT	mIconSize;
};

//******************************************************************************************
//******************************************************************************************

enum EEdAction
{
	ED_NONE,

	ED_LANDSCAPE_EDIT,
	ED_TILE_EDIT,
	ED_TRIGGER_EDIT,
	ED_THING_EDIT,
	ED_VAR_EDIT,
	ED_EXIT,
	ED_EXPORT_THIS,
	ED_EXPORT_ALL,
	ED_EXPORT_FE,
	ED_EXPORTDC_THIS,
	ED_EXPORTDC_ALL,
	ED_EXPORTDC_FE,

	ED_SAVE,
	ED_DISCARD,
	ED_UNDO,
	ED_REDO,

	ED_PREV_LEV,
	ED_LEV_NO,
	ED_NEXT_LEV,
	ED_REFRESH_LEVEL,

	ED_LED_SMALL,
	ED_LED_MEDIUM,
	ED_LED_LARGE,
	ED_LED_SELECT,
	ED_LED_CLIP_UP,
	ED_LED_CLIP_DOWN,
	ED_LED_SLOPE,
	ED_LED_RAISE_SELECTION,
	ED_LED_LOWER_SELECTION,
	ED_LED_PARABOLA,
	ED_LED_PIT,
	ED_LED_TAKEOFF,
	ED_LED_RAMP,
	ED_LED_CLEAR,
	ED_LED_UNIFY,

	ED_TRIGGER_TRIGGER,
	ED_TRIGGER_ICE,
	ED_TRIGGER_BOOSTU,
	ED_TRIGGER_BOOSTD,
	ED_TRIGGER_BOOSTL,
	ED_TRIGGER_BOOSTR,

	ED_THING_SELECT,

	ED_TD_XUP,
	ED_TD_XDOWN,
	ED_TD_YUP,
	ED_TD_YDOWN,
	ED_TD_ZUP,
	ED_TD_ZDOWN,

	ED_TD_TARGET_THING,
	ED_TD_OID,
	ED_TD_CHAIN_TRIGGER,

	ED_TD_INITIAL_MODE_UP,
	ED_TD_INITIAL_MODE_DOWN,

	ED_TD_TRIGGER_RESPONSE_UP,
	ED_TD_TRIGGER_RESPONSE_DOWN,
	ED_TD_UNTRIGGER_RESPONSE_UP,
	ED_TD_UNTRIGGER_RESPONSE_DOWN,

	ED_TD_COUNT0_UP,
	ED_TD_COUNT1_UP,
	ED_TD_COUNT2_UP,
	ED_TD_COUNT3_UP,
	ED_TD_COUNT0_DOWN,
	ED_TD_COUNT1_DOWN,
	ED_TD_COUNT2_DOWN,
	ED_TD_COUNT3_DOWN,

	ED_TD_BOOL0_UP,
	ED_TD_BOOL1_UP,
	ED_TD_BOOL2_UP,
	ED_TD_BOOL3_UP,
	ED_TD_BOOL0_DOWN,
	ED_TD_BOOL1_DOWN,
	ED_TD_BOOL2_DOWN,
	ED_TD_BOOL3_DOWN,

	ED_TD_MESH_SELECT,

	ED_BLEND_TEX_0,
	ED_BLEND_TEX_1,
	ED_BLEND_TEX_2,
	ED_BLEND_TEX_3,
	ED_BLEND_TEX_4,
	ED_BLEND_TEX_5,
	ED_BLEND_TEX_6,
	ED_BLEND_TEX_7,

	ED_BLEND_ICON_SET,
	ED_BLEND_ICON_ADD,
	ED_BLEND_ICON_SUB,
	ED_BLEND_ICON_FLAT,
	ED_BLEND_ICON_STEEP,
	ED_BLEND_ICON_SMALL,
	ED_BLEND_ICON_MEDIUM,
	ED_BLEND_ICON_LARGE,

	ED_TD_YAW_L,
	ED_TD_YAW_L90,
	ED_TD_YAW_R,
	ED_TD_YAW_R90,
	ED_TD_PITCH_L,
	ED_TD_PITCH_L90,
	ED_TD_PITCH_R,
	ED_TD_PITCH_R90,
	ED_TD_ROLL_L,
	ED_TD_ROLL_L90,
	ED_TD_ROLL_R,
	ED_TD_ROLL_R90,

	ED_INCREASE_NO_PLAYERS,
	ED_DECREASE_NO_PLAYERS,

	ED_VAR_WATER_DOWN,
	ED_VAR_WATER_UP,

	ED_LEVEL_TYPE_DOWN,
	ED_LEVEL_TYPE_UP,

	ED_TIME_LIMIT_DOWN,
	ED_TIME_LIMIT_UP,

	ED_DECREASE_NO_LAPS,
	ED_INCREASE_NO_LAPS,

	ED_DECREASE_NO_CHECKPOINTS,
	ED_INCREASE_NO_CHECKPOINTS,

	ED_DECREASE_NO_POINTS_REQ_LMR,
	ED_INCREASE_NO_POINTS_REQ_LMR,

	ED_DECREASE_PRISON_NO,
	ED_INCREASE_PRISON_NO,

	ED_YES,
	ED_NO,
};

enum EEdDialogType
{
	DIALOG_YESNO,
	DIALOG_NOYES,
};

//******************************************************************************************
#define MAX_BUTTONS	50

class	CButton
{
public:
	CButton() {mActive = TRUE; mData = NULL;};
	SINT		mX, mY, mW, mH;
	EEdAction	mAction;
	BOOL		mActive;
	UWORD		*mData;
	char		mText[256];
	BOOL		mSelectable;
	BOOL		mHoldDown;
};

//******************************************************************************************
#define MAX_LISTTEXT_ENTRIES	800
#define TEXT_HEIGHT	13

class	CPane;

class	CListText
{
public:
	CListText() {mNumEntries = 0; mCurrentEntry = 0; mScrollPos = 0;};

	void	AddEntry(const char *text, SINT data, SINT type);
	void	EvaluateDisplaySlots();
	BOOL	Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT &mw);

	void	Resize(SINT x, SINT y, SINT w, SINT h)
	{
		mX = x;
		mY = y;
		mW = w;
		mH = h;
		EvaluateDisplaySlots();
	};

	void	Deselect() {mCurrentEntry = -1;};
	
	SINT	mX, mY, mW, mH;
	SINT	mNumEntries;
	SINT	mCurrentEntry;
	SINT	mScrollPos;
	SINT	mNumDisplaySlots;
	struct	SEntry
	{
		char	mText[256];
		SINT	mData;
		SINT	mType;
	}	mEntries[MAX_LISTTEXT_ENTRIES];

	CPane	*mParent;
};

//******************************************************************************************
#define MAX_LISTICON_ENTRIES	800
#define ICON_SEPARATION	6
#define LISTICON_ICON_SIZE	32

class	CListIcon
{
public:
	CListIcon(SINT icon_size) {mIconSize = icon_size; mNumEntries = 0; mCurrentEntry = 0; mScrollPos = 0; mNumAcross = 1;};
	~CListIcon();

	void	AddEntry(UWORD *data, const char *text);
	void	EvaluateDisplaySlots();
	BOOL	Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT &mw);
	
	void	Resize(SINT x, SINT y, SINT w, SINT h)
	{
		mX = x;
		mY = y;
		mW = w;
		mH = h;
		EvaluateDisplaySlots();
	};

	
	SINT	mX, mY, mW, mH;
	SINT	mNumEntries;
	SINT	mCurrentEntry;
	SINT	mScrollPos;
	SINT	mNumDisplaySlots;
	SINT	mNumAcross;
	SINT	mIconSize;
	struct	SEntryIcon
	{
		UWORD	*mData;
		char	mText[256];
	}	mEntries[MAX_LISTTEXT_ENTRIES];

	CPane	*mParent;
};

//******************************************************************************************
enum	EPaneSide
{
	EPS_LEFT,
	EPS_RIGHT,
	EPS_TOP,
	EPS_BOTTOM,
	EPS_NONE,
};

#define SLIDE_COUNT_MAX	10

class	CPane
{
public:
	CPane(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE, SINT sizeys = -1);
	virtual	~CPane();
	
	virtual void	Init() {};

	void	ClearToBackground(SINT sx, SINT sy, SINT w, SINT h, SINT no);
	void	ClearToColour    (SINT sx, SINT sy, SINT w, SINT h, UWORD col);
	void	Clear();

	void	RenderListText(HDC hdc, SINT x, SINT y);
	virtual	void	Render(HDC hdc);
	virtual	void	Redraw();

	virtual	BOOL	Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT mw);
	virtual	void	ProcessAction(EEdAction action) {};
	virtual	void	ProcessAction2(EEdAction action) {};
	virtual	void	ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2) {};
	virtual	BOOL	ProcessKeys() {return FALSE;};

	virtual void	UpdateState() {};

	void	DrawOutlineRect(SINT x, SINT y, SINT w, SINT h, UWORD col);

	BOOL	IsActive() {return mActive;};
	
	virtual	void	Activate() {mActive = TRUE;};
	virtual	void	DeActivate() {mActive = FALSE;};

	UWORD	*GetAddr(SINT x, SINT y) {return &mData[x + y * mSizeX];};

	void	Move(SINT x, SINT y) {mScrX = x; mScrY = y;};

	// contents
	void	AddIconButton(SINT x, SINT y, CIconBank *icons, SINT no, EEdAction action, char *text, BOOL	selectable = TRUE, BOOL hold_down = FALSE);
	void	ActivateButton(EEdAction action);
	void	DeActivateButton(EEdAction action);

	CListText	*CreateAndGetListText();
	CListIcon	*CreateAndGetListIcon();

	CListText	*GetListText() {return mListText;};

	virtual	SINT	GetBackground() {return 1;};

	void	ChangeIconTexture(EEdAction a, char *fname);
	void	ChangeIconTexture(EEdAction a, CIconBank *icons, SINT no);

	CButton	*GetButtons() {return mButtons;};
	SINT	GetCurrentButton() {return mCurrentButton;};

	static	CPane	*mFirstPane;
	CPane	*mNextPane;
	SINT	mSlideCount;
protected:
	BOOL	mActive;
	UWORD	*mData;
	SINT	mSizeX, mSizeY;
	SINT	mScrX, mScrY;
	SINT	mDScrX, mDScrY;
	SINT	mSizeYS, mYPos;
	DWORD	mRop;
	BOOL	mTransparent;
	
	// Contents data
	SINT	mNumButtons;
	CButton	mButtons[MAX_BUTTONS];
	SINT	mCurrentButton;
	BOOL	IsButtonSelected(EEdAction action);
	void	SelectButton(EEdAction action);
	void	DeselectButton() {mCurrentButton = -1; Redraw();};

	CListText	*mListText;

	CListIcon	*mListIcon;

	void	GetRenderPos(SINT &sx, SINT &sy);
	void	DrawBitmap(SINT sx, SINT sy, UWORD *data, SINT size);
	void	DrawMaskedBitmap(SINT sx, SINT sy, UWORD *data, SINT size, SINT r, SINT g, SINT b);
	void	DrawActiveButton(CButton *b);
	void	DrawInActiveButton(CButton *b);
	void	DrawButtonHighlight(CButton *b);

	SINT	DrawFontText(SINT x, SINT y, char *text, SINT r = 255, SINT g = 255, SINT b = 255);

	SINT	DrawVScrollBar(SINT x, SINT y, SINT h, float bar_pos, BOOL align_right = FALSE);
	
	void	DrawListText(CListText *l);
	void	DrawListIcon(CListIcon *l);
};

//******************************************************************************************
//** tip toolbar
class	CPaneTip : public CPane
{
public:
	CPaneTip(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY) :
			CPane(sizex, sizey, scrx, scry, side, rop) {mText[0] = 0; mCounter = 0;};

	virtual void	Init();
	virtual	void	Render(HDC hdc);
	virtual	void	Redraw();
	virtual	SINT	GetBackground() {return 2;};

	void	SetText(char *p)	{strcpy(mText, p); Redraw();};
	void	ClearText()			{mText[0] = 0; Redraw();}; 
private:
	char	mText[256];

	float	mCounter;
};

//******************************************************************************************
//** LandscapeEd toolbar
class	CPaneLandscapeEd : public CPane
{
public:
	CPaneLandscapeEd(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
	virtual	void	Activate();
	virtual	void	DeActivate();
	virtual	void	ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2);
	virtual void	UpdateState();
	virtual	void	ProcessAction(EEdAction action);

	CCellRef	mStart;
	CCellRef	mEnd;

	enum
	{
		LED_NOT_SELECTING,
		LED_SELECTING,
		LED_SELECTED,
	} mSelecting;
};

//******************************************************************************************
//** Tile Detail Toolbar
class	CPaneTileDetail1 : public CPane
{
public:
	CPaneTileDetail1(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
};

//******************************************************************************************
//** Tile Detail Toolbar
class	CPaneTileDetail2 : public CPane
{
public:
	CPaneTileDetail2(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
};

//******************************************************************************************
//** Tile Detail Toolbar
class	CPaneTileDetail3 : public CPane
{
public:
	CPaneTileDetail3(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
};

//******************************************************************************************
//** Tile Detail Toolbar
class	CPaneTileDetail4 : public CPane
{
public:
	CPaneTileDetail4(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
};

//******************************************************************************************
//** TileEd toolbar
class	CPaneTileEd : public CPane
{
public:
	CPaneTileEd(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
	virtual	void	Activate();
	virtual	void	DeActivate();
	virtual	void	ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2);
	virtual	void	ProcessAction(EEdAction action);
	virtual	void	ProcessAction2(EEdAction action);
	virtual	BOOL	Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT mw);
	virtual void	UpdateState();

protected:
	CPaneTileDetail1	*mPaneTileDetail1;
	CPaneTileDetail2	*mPaneTileDetail2;
	CPaneTileDetail3	*mPaneTileDetail3;
	CPaneTileDetail4	*mPaneTileDetail4;
	
};

//******************************************************************************************
//** TriggerEd toolbar
class	CPaneTriggerEd : public CPane
{
public:
	CPaneTriggerEd(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
	virtual	void	Activate();
	virtual	void	DeActivate();
	virtual	void	ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2);
};

//******************************************************************************************
//** ThingDetail toolbar

class	CPaneMeshSelect : public CPane
{
public:
	CPaneMeshSelect(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
};

//******************************************************************************************
//** ThingDetail toolbar
class	CPaneThingEd;

class	CPaneThingDetail : public CPane
{
public:
	CPaneThingDetail(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE, SINT sizeys = -1) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent, sizeys) {};
	virtual void	Init();
	virtual	void	Activate();
	virtual	void	DeActivate();
	virtual	void	ProcessAction(EEdAction action);
	virtual	void	ProcessAction2(EEdAction action);
	virtual	void	ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2);
	virtual	void	Redraw();
	virtual	BOOL	Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT mw);
	virtual	SINT	GetBackground() {return 2;};
	virtual void	UpdateState();
	virtual	BOOL	ProcessKeys();

			BOOL	ChainOID(SINT new_oid);

	CPaneThingEd	*mParent;
protected:
	CThing *mCachedThing;

	CPaneMeshSelect	*mPaneMeshSelect;
};

//******************************************************************************************
//** ThingEd toolbar
class	CPaneThingEd : public CPane
{
public:
	CPaneThingEd(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
	virtual	void	Activate();
	virtual	void	DeActivate();
	virtual	void	ProcessAction(EEdAction action);
	virtual	void	ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2);
	virtual	BOOL	Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT mw);

protected:
	void			ActivateThingWindow();
	void			DeActivateThingWindow();

	CPaneThingDetail	*mPaneThingDetail;
};

//******************************************************************************************
//** VarEd toolbar
class	CPaneVarEd : public CPane
{
public:
	CPaneVarEd(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE, SINT sizeys = -1) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent, sizeys) {};
	virtual void	Init();
	virtual	void	Redraw();
	virtual	SINT	GetBackground() {return 2;};
	virtual	void	ProcessAction(EEdAction action);

	virtual void	UpdateState() {Redraw();};
};

//******************************************************************************************
//** Disk toolbar
class	CPaneDisk : public CPane
{
public:
	CPaneDisk(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
	virtual	void	Redraw();

	virtual	void	ProcessAction(EEdAction action);
	virtual void	UpdateState();
};

//******************************************************************************************
//** Dialog
class	CPaneDialog : public CPane
{
public:
	CPaneDialog(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {mCaller = NULL; mType = DIALOG_YESNO;};
	void	InitDialog(EEdDialogType type, CPane *caller, char *text);

	virtual	void	ProcessAction(EEdAction action);

	virtual	SINT	GetBackground() {return 3;};
protected:
	EEdDialogType	mType;
	CPane			*mCaller;
	char			mText[256];

};

//******************************************************************************************
//** main toolbar
class	CPaneMain : public CPane
{
public:
	friend class CLevelEditor2;

	CPaneMain(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop = SRCCOPY, BOOL transparent = FALSE) :
			CPane(sizex, sizey, scrx, scry, side, rop, transparent) {};
	virtual void	Init();
	virtual	void	ProcessAction(EEdAction action);

	virtual void	UpdateState()
	{
		if (mPaneLandscapeEd)
			mPaneLandscapeEd->UpdateState();
		if (mPaneTileEd)
			mPaneTileEd->UpdateState();
		if (mPaneVarEd)
			mPaneVarEd->UpdateState();
	};

protected:
	CPaneLandscapeEd	*mPaneLandscapeEd;
	CPaneTileEd			*mPaneTileEd;
	CPaneTriggerEd		*mPaneTriggerEd;
	CPaneThingEd		*mPaneThingEd;
	CPaneVarEd			*mPaneVarEd;
};

//******************************************************************************************
//******************************************************************************************

//******************************************************************************************
class	CLevelEditor2
{

public:
	friend class CPane;
	friend class CListText;
	friend class CListIcon;
	friend class CPaneDisk;
	friend class CPaneDialog;
	friend class CPaneTip;

	CLevelEditor2() {};
	~CLevelEditor2() {};

	BOOL	Init();
	void	Shutdown();


	BOOL	IsActive() {return mActive;};

	VOID	Set442Draw(SINT x) {m442Draw = x;};	
	SINT	Get442Draw() {return m442Draw;};	

	void	Activate();
	void	DeActivate();

	void	Process();

	void	Render2D(); // assume screen is locked.
	void	Render3D() {};

	void	SetTextColour(SINT r, SINT g, SINT b);
	void	DrawText(SINT x, SINT y, char *t);

	void	SetTipText(char *t);

	void	RegisterActionPane(CPane *p) {mOldActionPane = mActionPane; mActionPane = p;};  // 1 stack
	void	ReleaseActionPane(CPane *p) {if (mActionPane == p) {mActionPane = mOldActionPane; mOldActionPane = NULL;}};

	void	EnableDialogBox(char *text, EEdDialogType type, CPane *caller);

	void	RequestDialogDisable() {mPleaseDisableDialog = TRUE;};

	void	DeclareSelection(CCellRef &start, CCellRef &end);
	void	DisableSelection();
	BOOL	IsSelectionActive() {return (mSelectionStart.X != -1);};
	BOOL	IsPointInSelectedRegion(CCellRef &c);
	void	GetSelection(CCellRef &start, CCellRef &end) {start = mSelectionStart; end = mSelectionEnd;};

	BOOL	HasCurrentLevelChanged() {return mHasCurrentLevelChanged;};
	BOOL	CanIUndo() {return mUndoCount > 0;};
	BOOL	CanIRedo() {return mUndoCount < mUndoMax;};

	void	RegisterLevelChange(int action);
	void	RegisterLevelUnchanged();
	void	ResetUndo();

	void	Undo();
	void	Redo();
	void	RefreshLevel();

	void	RegisterSelectedThing(CThing *t) {mSelectedThing = t;};
	CThing	*GetSelectedThing() {return mSelectedThing;};
	CThing	*GetSelectedTargetThing();

	void	UpdateAllStates()
	{
		if (!mUpdating)
		{
			mUpdating = TRUE;

			if (mPaneMain)
				mPaneMain->UpdateState();
			if (mPaneDisk)
				mPaneDisk->UpdateState();

			mUpdating = FALSE;
		}
	}

	// static utility functions
	static	BOOL	LoadAndScaleTGA(char *fname, UWORD *data, SINT size);

	void	ClearSelections() {mSelectedThing = NULL;};

protected:
	BOOL		mActive;
	SINT	m442Draw;
	HDC			mHDC;
	COLORREF	mTextColour;

	CPaneMain	*mPaneMain;
	CPaneTip	*mPaneTip;
	CPaneDisk	*mPaneDisk;

	CPane		*mPaneMouse;
	CIconBank	*mSystemIcons;
	CIconBank	*mScrollIcons;
	CIconBank	*mDataEntryIcons;
	CIconBank	*mDialogIcons;
	CIconBank	*mFont;

	CCellRef	mSelectionStart;
	CCellRef	mSelectionEnd;
	
	CPane		*mActionPane, *mOldActionPane; // Pane to which spare events get sent

	// dialog stuff
	BOOL			mDialogActive;
	CPaneDialog		*mDialogPane;
	BOOL			mPleaseDisableDialog;

	// level state stuff
	BOOL			mHasCurrentLevelChanged;
	SINT			mUndoCount;
	SINT			mUndoMax;
	SINT			mUndoLastAction;

	CThing			*mSelectedThing;
	
	void	RenderCursor();

	BOOL	mUpdating;


	EControllerMode	mLastControllerMode;
};

//******************************************************************************************

#endif
#endif