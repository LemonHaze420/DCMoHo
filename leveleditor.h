#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#if EDITOR_VERSION == 1

#include	"OIDs.h"

//******************************************************************************************
class	CLEMenu
{
public:
	virtual	void	OnActivate() {};
	virtual	void	EnumerateItems() = 0;
	virtual	void	OnSelect(SINT item) {};
	virtual	void	OnLeftRight(SINT item, BOOL right) {};
	virtual	void	Process(SINT item) {};
};

//******************************************************************************************
class	CLEMMain : public CLEMenu
{
public:
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
	virtual	void	OnLeftRight(SINT item, BOOL right);
};

//******************************************************************************************
class	CLEMLandscapeEditor : public CLEMenu
{
public:
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
	virtual	void	OnLeftRight(SINT item, BOOL right);
	virtual void	Process(SINT item);
};

//******************************************************************************************
class	CLEMLandscapeMenu : public CLEMenu
{
public:
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
};

//******************************************************************************************

#define	MAX_LE_TEXTURE_NAMES 1000

class	CLEMTileEditor : public CLEMenu
{
public:
	CLEMTileEditor() {mNumTextureNames = mCurrentTextureNo = 0; mTextureNames[0][0] = 0;};
	virtual	void	OnActivate();
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
	virtual	void	OnLeftRight(SINT item, BOOL right);
	virtual void	Process(SINT item);
private:
	char	mTextureNames[MAX_LE_TEXTURE_NAMES][256];  // ooh!
	SINT	mNumTextureNames;
	SINT	mCurrentTextureNo;
};

//******************************************************************************************
class	CLEMLandscapeGen : public CLEMenu
{
public:
	CLEMLandscapeGen() {mPitch = 5.0f; mHeight = 5.0f;};
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
	virtual	void	OnLeftRight(SINT item, BOOL right);
protected:
	float	mPitch, mHeight;
};

//******************************************************************************************
class	CLEMThingEdit : public CLEMenu
{
public:
	CLEMThingEdit() {mThingID = OID_CEFRainDrop;};
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
	virtual	void	OnLeftRight(SINT item, BOOL right);
	virtual void	Process(SINT item);
protected:
	SINT	mThingID;
};

//******************************************************************************************
class	CLEMVarEdit : public CLEMenu
{
public:
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
	virtual	void	OnLeftRight(SINT item, BOOL right);
};

//******************************************************************************************
class	CLEMSaveOK : public CLEMenu
{
public:
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
};

//******************************************************************************************
class	CLEMDiscardOK : public CLEMenu
{
public:
	virtual	void	EnumerateItems();
	virtual	void	OnSelect(SINT item);
};


//******************************************************************************************
//******************************************************************************************

#define MAX_LEM_ITEMS 20

class	CLevelEditor
{
public:
	CLevelEditor() {};
	~CLevelEditor() {};

	BOOL	Init();
	void	Shutdown() {};


	BOOL	IsActive() {return mActive;};
	void	Activate();
	void	DeActivate();

	void	Process();
	HRESULT	OnKeyChange(UBYTE ascii, UINT keydata);

	void	Render2D(); // assume screen is locked.
	void	Render3D() {};

	void	ChangeMenu(CLEMenu *m) {mCurrentMenu = m; mCurrentItem = 0; m->OnActivate();};
	void	AddMenuItem(char *text, BOOL active = TRUE);

	// a bit shit, I know...
	CLEMMain			mLEMMain;
	CLEMLandscapeMenu	mLEMLandscapeMenu;
	CLEMLandscapeEditor mLEMLandscapeEditor;
	CLEMLandscapeGen	mLEMLandscapeGen;
	CLEMTileEditor		mLEMTileEditor;
	CLEMVarEdit			mLEMVarEdit;
	CLEMThingEdit		mLEMThingEdit;
	CLEMSaveOK			mLEMSaveOK;
	CLEMDiscardOK		mLEMDiscardOK;

	BOOL	mHasCurrentLevelChanged;

protected:
	BOOL		mActive;
	COLORREF	mTextColour;
	HDC			mHDC;

	void	SetTextColour(SINT r, SINT g, SINT b);
	void	DrawText(SINT x, SINT y, char *t);

	CLEMenu		*mCurrentMenu;
	SINT		mCurrentItem;

	SINT		mNumItems;

	char	mItems[MAX_LEM_ITEMS][200];
	BOOL	mItemsActive[MAX_LEM_ITEMS];

};

//******************************************************************************************

#endif

#endif