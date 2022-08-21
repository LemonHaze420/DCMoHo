// $Header$

// $Log$
// Revision 1.2  2000-07-11 10:22:51+01  jjs
// Seans hacked version - This will change
//

#include	"Common.h"

#ifdef LINK_EDITOR

#if EDITOR_VERSION == 2

#include	"LevelEd2.h"
#include	"Globals.h"
#include	"Thing.h"
#include	"TGALoader.h"
#include	"Editors.h"
#include	"Friction.h"

// only here...
#define		LD	GAME.GetLevelData()

// SDS I really shouldn't do this - but time is short!!

//******************************************************************************************
BOOL	CLevelEditor2::LoadAndScaleTGA(char *fname, UWORD *data, SINT size)
{

#if TARGET != DC

	char	name[256] = "data\\Textures\\";

	ASSERT(fname[0]);
	strcat(name, fname);

	
	SINT dummy;
	CTGALoader loader(name, &dummy);

	if (!loader.Load())
	{
		ASSERT(0);
		return FALSE;
	}

	SINT w = loader.Width();
	SINT h = loader.Height();

	// is height a multiple of width?
	if (((h / w) * w) != h)
	{
		ASSERT(0);
		return FALSE;
	}

	// is width a multiple of size?
	if (((w / size) * size) != w)
	{
		ASSERT(0);
		return FALSE;
	}

	SINT	ps = w / size;
	SINT	c0, c1, c2, c3;

	UBYTE	*sp = loader.GetData();

	// scale with mip-map
	for (c0 = 0; c0 < size; c0++)
		for (c1 = 0; c1 < size; c1++)
		{
			SINT	r = 0, g = 0, b = 0;
			for (c2 = 0; c2 < ps; c2 ++)
				for (c3 = 0; c3 < ps; c3 ++)
				{
					UBYTE	*p = &sp[(((size - c0 - 1) * w * ps) + (c1 * ps) + (c2 * w) + c3) * 3]; // TGAs are upside down
					b += p[0];
					g += p[1];
					r += p[2];
				}

			r /= ps * ps;
			g /= ps * ps;
			b /= ps * ps;

			data[(c0 * size) + c1] = RGBToWord(r, g, b);
		}

#endif
	return TRUE;
}

//******************************************************************************************
//** CIconBank
//******************************************************************************************
CIconBank::CIconBank(char *fname)
{

#if TARGET != DC
	mData = NULL;
	mNumIcons = -1;

	char	name[256] = "data\\Editor\\";

	ASSERT(fname[0]);
	strcat(name, fname);

	
	SINT dummy;
	CTGALoader loader(name, &dummy);

	if (!loader.Load())
	{
		ASSERT(0);
		return;
	}

	SINT w = loader.Width();
	SINT h = loader.Height();

	// is height a multiple of width?
	if (((h / w) * w) != h)
	{
		ASSERT(0);
		return;
	}

	mIconSize = w;

	// ok - create buffer
	mData = new UWORD[w * h];

	// and copy data
	SINT	c0, c1;
	UWORD	*dp = mData + w * (h - 1);
	UBYTE	*sp = loader.GetData();

	// annoyingly, TGAs come in upside down...
	for (c0 = 0; c0 < h; c0 ++)
	{
		for (c1 = 0; c1 < w; c1 ++)
		{
			*(dp++) = RGBToWord(sp[2], sp[1], sp[0]);
			sp += 3;
		}
		dp -= w * 2;
	}

	mNumIcons = h / w;
	#endif
}

//******************************************************************************************
//** Alternate loader allowing arrays of icons
CIconBank::CIconBank(char *fname, SINT across)
{

#if TARGET != DC
	mData = NULL;
	mNumIcons = -1;

	char	name[256] = "data\\Editor\\";

	ASSERT(fname[0]);
	strcat(name, fname);

	
	SINT dummy;
	CTGALoader loader(name, &dummy);

	if (!loader.Load())
	{
		ASSERT(0);
		return;
	}

	SINT w = loader.Width();
	SINT h = loader.Height();

	// is width a multiple of 'across'?
	if (((w / across) * across) != w)
	{
		ASSERT(0);
		return;
	}
	mIconSize = w / across;

	// is height a multiple of 'icon size'?
	if (((h / mIconSize) * mIconSize) != h)
	{
		ASSERT(0);
		return;
	}

	// ok - create buffer
	mData = new UWORD[w * h];

	// and pull out the icons one by one
	SINT	c0, c1, c2, c3;
	SINT	down = h / mIconSize;

	for (c0 = 0; c0 < down; c0 ++)
		for (c1 = 0; c1 < across; c1 ++)
		{
			UWORD	*dp = mData + ((c0 * across) + c1) * mIconSize * mIconSize;
			// TGAs are upside down...
			UBYTE	*sp = loader.GetData() + (((down - c0 - 1) * w * mIconSize) + (c1 * mIconSize) + (w * (mIconSize - 1))) * 3;
		
			for (c2 = 0; c2 < mIconSize; c2 ++)
			{
				for (c3 = 0; c3 < mIconSize; c3 ++)
				{
					*(dp++) = RGBToWord(sp[2], sp[1], sp[0]);
					sp += 3;
				}
				sp -= (w + mIconSize) * 3;
			}
		}

	mNumIcons = across * down;
#endif	
}

//******************************************************************************************
CIconBank::~CIconBank()
{
#if TARGET != DC
	delete [] mData;
	mData = NULL;
#endif	
}

//******************************************************************************************
void	CIconBank::GetIcon(SINT no, UWORD *dest, SINT pitch)
{
#if TARGET != DC
	if ((no < 0) || (no >= mNumIcons))
		return;

	SINT	c0, c1;
	UWORD	*s = mData + no * (mIconSize * mIconSize);

	for (c0 = 0; c0 < mIconSize; c0 ++)
	{
		for (c1 = 0; c1 < mIconSize; c1 ++)
		{
			*(dest++) = *(s++);
		}
		dest += pitch - mIconSize;
	}
#endif	
}

//******************************************************************************************
SINT	CIconBank::GetIconWidth(SINT num)
{

#if TARGET != DC
	// for proportional fonts
	UWORD *p = GetIconData(num);

	SINT	x, y;
	SINT	lx = 0;

	for (x = 0; x < mIconSize; x ++)
		for (y = 0; y < mIconSize; y ++)
		{
			if (p[(y * mIconSize) + x] != 0)
				lx = x;
		}

	return lx + 1;
#endif	
}

//******************************************************************************************
//** CListText
//******************************************************************************************
int		entry_comp(const void *e1, const void *e2)
{
#if TARGET != DC
	return strcmp(((const CListText::SEntry *)e1)->mText, ((const CListText::SEntry *)e2)->mText);
#endif	
}

void	CListText::AddEntry(const char *text, SINT data, SINT type)
{
#if TARGET != DC
	if (mNumEntries == MAX_LISTTEXT_ENTRIES)
		return;

	strcpy(mEntries[mNumEntries].mText, text);
	mEntries[mNumEntries].mData = data;
	mEntries[mNumEntries].mType = type;

	mNumEntries ++;

	qsort(mEntries, mNumEntries, sizeof(SEntry), entry_comp);
#endif	
}

void	CListText::EvaluateDisplaySlots()
{
#if TARGET != DC
	mNumDisplaySlots = (mH - 2) / TEXT_HEIGHT;
#endif	
}

//******************************************************************************************
BOOL	CListText::Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT &mw)
{
#if TARGET != DC
	if ((x < mX) || (x >= mX + mW) ||
		(y < mY) || (y >= mY + mH))
		return FALSE;

	SINT	size = EDITOR.mScrollIcons->GetSize();

	if (x > mX + mW - size)
	{
		// on scroll bar

		// process arrows
		if (PLATFORM.IsMouseButtonPressed(0))
		{
			if (y < mY + size)
			{
				// top arrow
				if (mScrollPos > 0)
				{
					mScrollPos --;
					mParent->Redraw();
				}
			}
			else if (y > mY + mH - size)
			{
				// bottom arrow
				if (mScrollPos < mNumEntries - mNumDisplaySlots)
				{
					mScrollPos ++;
					mParent->Redraw();
				}
			}
		}
		
		//! pickup bar here...
	}
	else
	{
		// on text
		if (b1)
		{
			SINT line = (y - mY - 2) / TEXT_HEIGHT;
			SINT item = line + mScrollPos;

			if (item < mNumEntries)
			{
				mCurrentEntry = item;
				mParent->Redraw();
			}
		}
	}

	// mouse wheel
	if (mw)
	{
		mScrollPos += -(mw / 60);
		if (mScrollPos < 0)
			mScrollPos = 0;
		if (mScrollPos > mNumEntries - mNumDisplaySlots)
			mScrollPos = mNumEntries - mNumDisplaySlots;
		
		mParent->Redraw();
	}
#endif
	return TRUE;
}

//******************************************************************************************
//** CListIcon
//******************************************************************************************
int		entry_icon_comp(const void *e1, const void *e2)
{
#if TARGET != DC
	return strcmp(((const CListIcon::SEntryIcon *)e1)->mText, ((const CListIcon::SEntryIcon *)e2)->mText);
#endif	
}

void	CListIcon::AddEntry(UWORD *data, const char *text)
{
#if TARGET != DC
	if (mNumEntries == MAX_LISTICON_ENTRIES)
		return;

	mEntries[mNumEntries].mData = new UWORD [mIconSize * mIconSize];
	ASSERT(mEntries[mNumEntries].mData);
	memcpy(mEntries[mNumEntries].mData, data, mIconSize * mIconSize * sizeof(UWORD));

	strcpy(mEntries[mNumEntries].mText, text);

	mNumEntries ++;

	qsort(mEntries, mNumEntries, sizeof(SEntryIcon), entry_icon_comp);
#endif	
}

//******************************************************************************************
CListIcon::~CListIcon()
{
#if TARGET != DC
	while(mNumEntries --)
	{
		delete [] mEntries[mNumEntries].mData;
	};
#endif	
}

//******************************************************************************************
void	CListIcon::EvaluateDisplaySlots()
{
#if TARGET != DC
	// work out width
	SINT	scroll_size = EDITOR.mScrollIcons->GetSize();
	SINT	width = mW - scroll_size + (ICON_SEPARATION / 2);

	mNumAcross = width / (mIconSize + ICON_SEPARATION);

	mNumDisplaySlots = (mH - ICON_SEPARATION) / (mIconSize + ICON_SEPARATION);
#endif	
}

//******************************************************************************************
BOOL	CListIcon::Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT &mw)
{
#if TARGET != DC
	if ((x < mX) || (x >= mX + mW) ||
		(y < mY) || (y >= mY + mH))
		return FALSE;


	SINT	size = EDITOR.mScrollIcons->GetSize();

	if (x > mX + mW - size)
	{
		// on scroll bar

		// process arrows
		if (PLATFORM.IsMouseButtonPressed(0))
		{
			if (y < mY + size)
			{
				// top arrow
				if (mScrollPos > 0)
				{
					mScrollPos --;
					mParent->Redraw();
				}
			}
			else if (y > mY + mH - size)
			{
				// bottom arrow
				if (mScrollPos < ((mNumEntries / mNumAcross) + 1) - mNumDisplaySlots)
				{
					mScrollPos ++;
					mParent->Redraw();
				}
			}
		}
		
		//! pickup bar here...
	}
	else
	{
		// on icons
		SINT line	= (y - mY) / (LISTICON_ICON_SIZE + ICON_SEPARATION);
		SINT column = (x - mX) / (LISTICON_ICON_SIZE + ICON_SEPARATION);
		if ((column >= 0) && (column < mNumAcross))
		{
			SINT item = (line + mScrollPos) * mNumAcross + column;

			if (item < mNumEntries)
			{
				char	text[200];
				sprintf(text, "Texture: %s", mEntries[item].mText);
				EDITOR.SetTipText(text);

				if (b1)
				{
					mCurrentEntry = item;
					mParent->Redraw();
				}
			}
		}
	}

	// mouse wheel
	if (mw)
	{
		mScrollPos += -(mw / 120);
		if (mScrollPos < 0)
			mScrollPos = 0;
		if (mScrollPos > ((mNumEntries / mNumAcross) + 1) - mNumDisplaySlots)
			mScrollPos = ((mNumEntries / mNumAcross) + 1) - mNumDisplaySlots;
		
		mParent->Redraw();
	}
#endif
	return TRUE;
}

//******************************************************************************************
//** CPane
//******************************************************************************************
CPane	*CPane::mFirstPane = NULL;

CPane::CPane(SINT sizex, SINT sizey, SINT scrx, SINT scry, EPaneSide side, DWORD rop, BOOL transparent, SINT sizeys)
{
#if TARGET != DC
	mNextPane = mFirstPane;
	mFirstPane = this;

	mSizeX = sizex;
	mSizeY = sizey;
	mScrX  = scrx;
	mScrY  = scry;

	if (sizeys == -1)
		mSizeYS = sizey;
	else
		mSizeYS = sizeys;
	mYPos = 0;

	mRop = rop;
	mTransparent = transparent;

	mData = new UWORD[mSizeX * mSizeY];
	ASSERT(mData);

	// amusing.
	switch (side)
	{
	case EPS_LEFT:		mDScrX = -mSizeX;					mDScrY =  mScrY;					break;
	case EPS_RIGHT:		mDScrX =  PLATFORM.ScreenWidth();	mDScrY =  mScrY;					break;
	case EPS_TOP:		mDScrX =  mScrX;					mDScrY = -mSizeY;					break;
	case EPS_BOTTOM:	mDScrX =  mScrX;					mDScrY =  PLATFORM.ScreenHeight();	break;
	case EPS_NONE:		mDScrX =  -1;						mDScrY =  -1;						break;
	default: ASSERT(0);
	}

	mSlideCount = SLIDE_COUNT_MAX;

	// contents
	mNumButtons = 0;
	mCurrentButton = 0;
	mListText = NULL;
	mListIcon = NULL;

	// draw
	DeActivate();

	Redraw();
#endif	
}

//******************************************************************************************
CPane::~CPane()
{
#if TARGET != DC
	// delete contents
	SINT	c0;
	for (c0 = 0; c0 < mNumButtons; c0++)
	{
		delete mButtons[c0].mData;
		mButtons[c0].mData = NULL;
	}

	delete mListText;
	mListText = NULL;

	delete mListIcon;
	mListText = NULL;

	// delete screen
	delete [] mData;
	mData = NULL;

	// delete from linked list
	if (mFirstPane == this)
	{
		mFirstPane = mNextPane;
	}
	else
	{
		CPane *p = mFirstPane;
		while (p->mNextPane != this)
		{
			ASSERT(p);
			p = p->mNextPane;
		}
		p->mNextPane = mNextPane;
	}
#endif	
}

//******************************************************************************************
void	CPane::ClearToBackground(SINT sx, SINT sy, SINT w, SINT h, SINT no)
{
#if TARGET != DC
	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;
	if (sx >= mSizeX) sx = mSizeX - 1;
	if (sy >= mSizeY) sy = mSizeY - 1;
	if (sx + w > mSizeX) w = mSizeX - sx;
	if (sy + h > mSizeY) h = mSizeY - sy;

	SINT	c0, c1;
	UWORD	*p = mData + sx + sy * mSizeX;
	UWORD	*d = EDITOR.mSystemIcons->GetIconData(no);
	SINT	size = EDITOR.mSystemIcons->GetSize();

	for (c0 = 0; c0 < h; c0 ++)
	{
		for (c1 = 0; c1 < w; c1 ++)
			*(p++) = *(d + ((sx + c1) % size) + (((sy + c0) % size) * size));

		p += mSizeX - w;
	}
#endif	
}

//******************************************************************************************
void	CPane::ClearToColour(SINT sx, SINT sy, SINT w, SINT h, UWORD col)
{
#if TARGET != DC
	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;
	if (sx >= mSizeX) sx = mSizeX - 1;
	if (sy >= mSizeY) sy = mSizeY - 1;
	if (sx + w > mSizeX) w = mSizeX - sx;
	if (sy + h > mSizeY) h = mSizeY - sy;

	SINT	c0, c1;
	UWORD	*p = mData + sx + sy * mSizeX;
	UWORD	*d = EDITOR.mSystemIcons->GetIconData(1);

	for (c0 = 0; c0 < h; c0 ++)
	{
		for (c1 = 0; c1 < w; c1 ++)
			*(p++) = col;

		p += mSizeX - w;
	}
#endif	
}

//******************************************************************************************
void	CPane::Clear()
{
#if TARGET != DC
	ClearToBackground(0, 0, mSizeX, mSizeY, GetBackground());

	DrawOutlineRect(0, 0, mSizeX    , mSizeY    , RGBToWord(255, 255, 255));
	DrawOutlineRect(1, 1, mSizeX - 2, mSizeY - 2, RGBToWord(0  , 0  ,   0));
#endif	
}

//******************************************************************************************
void	CPane::DrawOutlineRect(SINT x, SINT y, SINT w, SINT h, UWORD col)
{
#if TARGET != DC
	SINT	c0;
	UWORD	*p1, *p2;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= mSizeX) x = mSizeX - 1;
	if (y >= mSizeY) y = mSizeY - 1;
	if (x + w > mSizeX) w = mSizeX - x;
	if (y + h > mSizeY) h = mSizeY - y;

	// draw top & bottom
	p1 = mData + x + (mSizeX * y);
	p2 = mData + x + (mSizeX * (y + h - 1));

	for (c0 = 0; c0 < w; c0++)
	{
		*(p1++) = col;
		*(p2++) = col;
	}

	// draw left & right
	p1 = mData + x + (mSizeX * y);
	p2 = mData + x + (w - 1) + (mSizeX * y);

	for (c0 = 0; c0 < h; c0++)
	{
		*p1 = col;
		*p2 = col;
		p1 += mSizeX;
		p2 += mSizeX;
	}
#endif	

}

//******************************************************************************************
void	CPane::GetRenderPos(SINT &sx, SINT &sy)
{
#if TARGET != DC
	// work out screen position

	sx = mScrX + (((mDScrX - mScrX) * mSlideCount * mSlideCount) / (SLIDE_COUNT_MAX * SLIDE_COUNT_MAX));
	sy = mScrY + (((mDScrY - mScrY) * mSlideCount * mSlideCount) / (SLIDE_COUNT_MAX * SLIDE_COUNT_MAX));

	// unless we don't slide
	if ((mDScrX == -1) && (mDScrY == -1))
	{
		sx = mScrX;
		sy = mScrY;
	}
#endif	
}

//******************************************************************************************
void	CPane::RenderListText(HDC hdc, SINT x, SINT y)
{
#if TARGET != DC
	//! this function shouldn't really exist...
	if (!mListText)
		return;

	// Draw the text - the rest is handled in the pane...
	SINT px = mListText->mX + 2;
	SINT py = mListText->mY + 2;
	
	SINT c0 = mListText->mScrollPos;

	while ((c0 < mListText->mNumEntries) && (py + TEXT_HEIGHT < mListText->mY + mListText->mH))
	{
		switch (mListText->mEntries[c0].mType)
		{
		case 0:
			if (c0 == mListText->mCurrentEntry)
				EDITOR.SetTextColour(255, 255, 255);
			else
				EDITOR.SetTextColour(255, 200, 100);
			break;
		case 1:
			if (c0 == mListText->mCurrentEntry)
				EDITOR.SetTextColour(200, 220, 255);
			else
				EDITOR.SetTextColour(100, 200, 255);
			break;
		}

		EDITOR.DrawText(x + px, y + py, mListText->mEntries[c0].mText);
		c0 ++;
		py += TEXT_HEIGHT;
	};
#endif	
}

//******************************************************************************************
void	CPane::Render(HDC hdc)
{
#if TARGET != DC
	// am I active?
	if ((!mActive) && (mSlideCount == SLIDE_COUNT_MAX))
		return;

	if (!mActive)
	{
		if (mSlideCount < SLIDE_COUNT_MAX)
			mSlideCount ++;
	}
	else
	{
		if (mSlideCount > 0)
			mSlideCount --;
	}

	SINT	sx, sy;
	GetRenderPos(sx, sy);

	// Assumes screen is locked and hdc is valid...
	
	BITMAPINFO	bmi;

	memset(&bmi.bmiHeader, 0, sizeof(bmi.bmiHeader));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);

	bmi.bmiHeader.biWidth = mSizeX;
	bmi.bmiHeader.biHeight = -mSizeYS; // don't ask why...
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 16;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = mSizeX * mSizeYS * 2;

	StretchDIBits(hdc,
				  sx, sy,
				  mSizeX, mSizeYS,
				  0, 0,
				  mSizeX, mSizeYS,
				  (void *)(mData + (mSizeX * mYPos)), 
				  &bmi,
				  DIB_RGB_COLORS,
				  mRop);
				  

	RenderListText(hdc, sx, sy);
	
#endif	
}

//******************************************************************************************
//** Contents
void	CPane::AddIconButton(SINT x, SINT y, CIconBank *icons, SINT no, EEdAction action, char *text, BOOL selectable, BOOL hold_down)
{
#if TARGET != DC
	ASSERT(mNumButtons < MAX_BUTTONS);

	SINT	size = icons->GetSize();

	mButtons[mNumButtons].mX = x;
	mButtons[mNumButtons].mY = y;
	mButtons[mNumButtons].mW = size;
	mButtons[mNumButtons].mH = size;

	mButtons[mNumButtons].mAction = action;

	strcpy(mButtons[mNumButtons].mText, text);

	mButtons[mNumButtons].mSelectable = selectable;
	mButtons[mNumButtons].mHoldDown = hold_down;

	// copy data
	mButtons[mNumButtons].mData = new UWORD[size * size];
	ASSERT(mButtons[mNumButtons].mData);
	icons->GetIcon(no, mButtons[mNumButtons].mData, size);

	mNumButtons ++;

	// activate
	ActivateButton(action);
#endif	
	
}

//******************************************************************************************
void	CPane::DrawBitmap(SINT sx, SINT sy, UWORD *data, SINT size)
{
#if TARGET != DC
	// copy up.
	SINT	x, y;
	UWORD	*dest = GetAddr(sx, sy);

	for (y = 0; y < size; y ++)
	{
		for (x = 0; x < size; x ++)
		{
			*(dest++) = *(data++);
		}
		dest += mSizeX - size;
	}
#endif	
}

//******************************************************************************************
void	CPane::DrawMaskedBitmap(SINT sx, SINT sy, UWORD *data, SINT size, SINT r, SINT g, SINT b)
{
#if TARGET != DC
	// copy up.
	SINT	x, y;
	UWORD	*dest = GetAddr(sx, sy);

	for (y = 0; y < size; y ++)
	{
		for (x = 0; x < size; x ++)
		{
			if (*data)
//				*dest = Colourize(*data, r, g, b);
				*dest = Blend(Colourize(*data, r, g, b), *dest, *data);
			dest ++;
			data ++;
		}
		dest += mSizeX - size;
	}
#endif	
}

//******************************************************************************************
void	CPane::DrawButtonHighlight(CButton *b)
{
#if TARGET != DC
	DrawOutlineRect(b->mX - 1, b->mY - 1, b->mW + 2, b->mH + 2, RGBToWord(255, 255, 100));
	DrawOutlineRect(b->mX - 2, b->mY - 2, b->mW + 4, b->mH + 4, RGBToWord(200, 100,  50));
//	DrawOutlineRect(b->mX - 3, b->mY - 3, b->mW + 6, b->mH + 6, RGBToWord(200,   0,   0));
#endif
}

//******************************************************************************************
void	CPane::DrawActiveButton(CButton *b)
{
#if TARGET != DC
	// copy up.
	SINT	x, y;
	UWORD	*s = b->mData;
	UWORD	*dest = GetAddr(b->mX, b->mY);

	for (y = 0; y < b->mH; y ++)
	{
		for (x = 0; x < b->mW; x ++)
		{
			*(dest++) = *(s++);
		}
		dest += mSizeX - b->mW;
	}
#endif	
}

//******************************************************************************************
void	CPane::DrawInActiveButton(CButton *b)
{
#if TARGET != DC
//	ClearToBackground(b->mX, b->mY, b->mW + 2, b->mH + 2, GetBackground());

	// copy up.
	SINT	x, y;
	UWORD	*dest = GetAddr(b->mX + 2, b->mY + 2);

	UWORD	*s = b->mData;
	for (y = 0; y < b->mH; y ++)
	{
		for (x = 0; x < b->mW; x ++)
		{
			// do greyscale version.
			*(dest++) = ToGreyScale(*(s++));
		}
		dest += mSizeX - b->mW;
	}
#endif	
}

//******************************************************************************************
void	CPane::ActivateButton(EEdAction action)
{
#if TARGET != DC
	// find the button
	SINT	c0;

	for (c0 = 0; c0 < mNumButtons; c0++)
	{
		if (mButtons[c0].mAction == action)
		{
			CButton *b = &mButtons[c0];
			b->mActive = TRUE;
			Redraw();
		}
	}
#endif	
}

//******************************************************************************************
void	CPane::DeActivateButton(EEdAction action)
{
#if TARGET != DC
	// find the button
	SINT	c0;

	for (c0 = 0; c0 < mNumButtons; c0++)
	{
		if (mButtons[c0].mAction == action)
		{
			CButton *b = &mButtons[c0];
			b->mActive = FALSE;
			Redraw();
		}
	}
#endif	
}

//******************************************************************************************
BOOL	CPane::IsButtonSelected(EEdAction action)
{
#if TARGET != DC
	// find the button
	SINT	c0;

	//! who the fuck wrote this code - it's appalling!

	for (c0 = 0; c0 < mNumButtons; c0++)
	{
		if (mButtons[c0].mAction == action)
		{
			return (c0 == mCurrentButton);
		}
	}
#endif	
	return FALSE;
}

//******************************************************************************************
void	CPane::SelectButton(EEdAction action)
{
#if TARGET != DC
	// find the button
	SINT	c0;

	for (c0 = 0; c0 < mNumButtons; c0++)
	{
		if (mButtons[c0].mAction == action)
		{
			mCurrentButton = c0;
			Redraw();
		}
	}
#endif	
}

//******************************************************************************************
CListText	*CPane::CreateAndGetListText()
{
#if TARGET != DC
	ASSERT(!mListText)
	mListText = new CListText;
	ASSERT(mListText);

	mListText->mParent = this;
#endif
	return mListText;
}

//******************************************************************************************
CListIcon	*CPane::CreateAndGetListIcon()
{
#if TARGET != DC
	ASSERT(!mListIcon)
	mListIcon = new CListIcon(LISTICON_ICON_SIZE);
	ASSERT(mListIcon);

	mListIcon->mParent = this;
#endif
	return mListIcon;
}

//******************************************************************************************
SINT	CPane::DrawVScrollBar(SINT x, SINT y, SINT h, float bar_pos, BOOL align_right)
{
#if TARGET != DC
	CIconBank	*i = EDITOR.mScrollIcons;
	SINT		size = i->GetSize();
	SINT		px, py;

	// draw line
	if (align_right)
		px = x + 1 - size;
	else
		px = x;
	py = y + size;

	while (py < y + h - size)
	{
		i->GetIcon(2, GetAddr(px, py), mSizeX);
		py += size;
	};

	// draw bar
	SINT	bpos = SINT(float(h - size * 3) * bar_pos);
	i->GetIcon(3, GetAddr(px, y + size + bpos), mSizeX);

	// draw top + bottom
	i->GetIcon(0, GetAddr(px, y), mSizeX);
	i->GetIcon(1, GetAddr(px, y + h - size), mSizeX);
#elif TARGET == DC
	SINT size =0;
#endif
	return size;
}

//******************************************************************************************
void	CPane::DrawListText(CListText *l)
{
#if TARGET != DC
//	ClearToColour(l->mX, l->mY,
//				  l->mW, l->mH,
//				  RGBToWord(50, 50, 50));
	ClearToBackground(l->mX, l->mY,
					  l->mW, l->mH,
					  2);

	// draw scrollbar
	float	bar_pos;

	if (l->mNumEntries <= l->mNumDisplaySlots)
			bar_pos = 0.f;
	else	bar_pos = float(l->mScrollPos) / float(l->mNumEntries - l->mNumDisplaySlots);
	
	SINT size = DrawVScrollBar(l->mX + l->mW, l->mY, l->mH, bar_pos ,TRUE);

	// clear text space
	DrawOutlineRect(l->mX    , l->mY    , l->mW - size    , l->mH    , RGBToWord(255, 255, 255));
	DrawOutlineRect(l->mX + 1, l->mY + 1, l->mW - size - 2, l->mH - 2, RGBToWord(  0,   0,   0));
#endif
}

//******************************************************************************************
void	CPane::DrawListIcon(CListIcon *l)
{
#if TARGET != DC
	ClearToBackground(l->mX, l->mY,
					  l->mW, l->mH,
					  2);

	// draw scrollbar
	float	bar_pos;

	if (l->mNumEntries <= l->mNumDisplaySlots)
			bar_pos = 0.f;
	else	bar_pos = float(l->mScrollPos) / float(((l->mNumEntries / l->mNumAcross) + 1) - l->mNumDisplaySlots);
	
	SINT size = DrawVScrollBar(l->mX + l->mW, l->mY, l->mH, bar_pos ,TRUE);

	// clear icon space
	DrawOutlineRect(l->mX    , l->mY    , l->mW - size    , l->mH    , RGBToWord(255, 255, 255));
	DrawOutlineRect(l->mX + 1, l->mY + 1, l->mW - size - 2, l->mH - 2, RGBToWord(  0,   0,   0));

	// Draw icons!!
	SINT	ct = l->mScrollPos * l->mNumAcross;
	SINT	c0, c1;

	SINT	px = l->mX + ICON_SEPARATION / 2;
	SINT	py = l->mY + ICON_SEPARATION / 2;

	for (c0 = 0; c0 < l->mNumDisplaySlots; c0 ++)
	{
		for (c1 = 0; c1 < l->mNumAcross; c1 ++)
		{
			DrawBitmap(px, py, l->mEntries[ct].mData, LISTICON_ICON_SIZE);

			EFrictionType	ft = CFriction::GetFrictionForTexture(l->mEntries[ct].mText);

			switch (ft)
			{
			case FT_FAST:
				DrawOutlineRect(px - 1, py - 1, LISTICON_ICON_SIZE + 2, LISTICON_ICON_SIZE + 2, RGBToWord(255, 0, 0));
				DrawOutlineRect(px - 2, py - 2, LISTICON_ICON_SIZE + 4, LISTICON_ICON_SIZE + 4, RGBToWord(255, 0, 0));
				break;
			case FT_SLOW:
				DrawOutlineRect(px - 1, py - 1, LISTICON_ICON_SIZE + 2, LISTICON_ICON_SIZE + 2, RGBToWord(0, 255, 0));
				DrawOutlineRect(px - 2, py - 2, LISTICON_ICON_SIZE + 4, LISTICON_ICON_SIZE + 4, RGBToWord(0, 255, 0));
				break;
			case FT_ICE:
				DrawOutlineRect(px - 1, py - 1, LISTICON_ICON_SIZE + 2, LISTICON_ICON_SIZE + 2, RGBToWord(0, 0, 255));
				DrawOutlineRect(px - 2, py - 2, LISTICON_ICON_SIZE + 4, LISTICON_ICON_SIZE + 4, RGBToWord(0, 0, 255));
				break;
			}

			if (ct == l->mCurrentEntry)
			{
				DrawOutlineRect(px - 1, py - 1, LISTICON_ICON_SIZE + 2, LISTICON_ICON_SIZE + 2, RGBToWord(255, 255, 100));
				DrawOutlineRect(px - 2, py - 2, LISTICON_ICON_SIZE + 4, LISTICON_ICON_SIZE + 4, RGBToWord(200, 100,  50));
			}
			px += LISTICON_ICON_SIZE + ICON_SEPARATION;
			ct ++;

			if (ct >= l->mNumEntries)
				break;
		}

		px = l->mX + ICON_SEPARATION / 2;
		py += LISTICON_ICON_SIZE + ICON_SEPARATION;

		if (ct >= l->mNumEntries)
			break;
	}
#endif	
}

//******************************************************************************************
SINT	CPane::DrawFontText(SINT x, SINT y, char *text, SINT r, SINT g, SINT b)
{
#if TARGET != DC
	SINT	font_size = EDITOR.mFont->GetSize();
	SINT	ox = x;

	while (*text)
	{
		char c = *text;
		if ((c >= 32) && (c <= 127))
		{
			SINT offset = 0;

/*			switch (c)
			{
			case 'g':
			case 'p':
			case 'q':
			case 'y':
				offset = 3; 
				break;
			};*/

			// draw shadow
			DrawMaskedBitmap(x + 2, y + 2 + offset, EDITOR.mFont->GetIconData(c - 32), font_size, 20, 20, 20);
			
			// and coloured text
			DrawMaskedBitmap(x, y + offset, EDITOR.mFont->GetIconData(c - 32), font_size, r, g, b);
		}

		x += EDITOR.mFont->GetIconWidth(c - 32) + 1;
		text++;
	}
#elif TARGET == DC
 int ox;	
#endif
	return x - ox;
}

//******************************************************************************************
void	CPane::Redraw()
{
#if TARGET != DC
	Clear();

	SINT c0;
	for (c0 = 0; c0 < mNumButtons; c0 ++)
	{
		if (mButtons[c0].mSelectable)   // is this a proper button
			DrawInActiveButton(&mButtons[c0]);
		if (mButtons[c0].mActive)
			DrawActiveButton(&mButtons[c0]);
	}

	if ((mNumButtons > 0) && (mCurrentButton != -1))
		DrawButtonHighlight(&mButtons[mCurrentButton]);

	if (mListText)
		DrawListText(mListText);

	if (mListIcon)
		DrawListIcon(mListIcon);
#endif		
}

//******************************************************************************************
BOOL	CPane::Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT mw)
{
#if TARGET != DC
	// am I active?
	if (!mActive)
		return FALSE;

	// am I transparent
	if (mTransparent)
		return FALSE;

	// is the cursor on this menu?
	if ((x < mScrX) || (x >= mScrX + mSizeX ) ||
		(y < mScrY) || (y >= mScrY + mSizeYS))
		return FALSE; // no

	// ok - check the buttons
	SINT	c0;

	for (c0 = 0; c0 < mNumButtons; c0 ++)
	{
		if (mButtons[c0].mActive)
		{
			if ((x >= mScrX          + mButtons[c0].mX                  ) &&
				(x <  mScrX          + mButtons[c0].mX + mButtons[c0].mW) &&
				(y >= mScrY - mYPos  + mButtons[c0].mY                  ) &&
				(y <  mScrY - mYPos  + mButtons[c0].mY + mButtons[c0].mH))
			{
				// process buttons.
				if (((!mButtons[c0].mHoldDown) && (b1)) ||
					(( mButtons[c0].mHoldDown) && (CONTROLS.IsButtonPressed(BUTTON_FIRE_PRIMARY))))
				{
					if (mButtons[c0].mSelectable)
					{
						mCurrentButton = c0;
						Redraw();
					}
					ProcessAction(mButtons[c0].mAction);
				}
				if (((!mButtons[c0].mHoldDown) && (b2)) ||
					(( mButtons[c0].mHoldDown) && (CONTROLS.IsButtonPressed(BUTTON_FIRE_SECONDARY))))
				{
					if (mButtons[c0].mSelectable)
					{
						mCurrentButton = c0;
						Redraw();
					}
					ProcessAction2(mButtons[c0].mAction);
				}

				// and text
				EDITOR.SetTipText(mButtons[c0].mText);

//				return TRUE;
			}
		}
	}

	// check listtext
	if (mListText)
		if (mListText->Process(x - mScrX, y - mScrY + mYPos, b1, b2, mw))
			return TRUE;

	// check listicon
	if (mListIcon)
		if (mListIcon->Process(x - mScrX, y - mScrY + mYPos, b1, b2, mw))
			return TRUE;

	// process mouse wheel
	// note - mw may have been cleared by the listicon/box above.
	if (mw)
	{
		mYPos -= mw / 4;

		if (mYPos < 0)
			mYPos = 0;
		if (mYPos > mSizeY - mSizeYS)
			mYPos = mSizeY - mSizeYS;

	}
#endif	
	return TRUE;
}

//******************************************************************************************
void	CPane::ChangeIconTexture(EEdAction a, char *fname)
{
#if TARGET != DC
	// first, find the icon
	SINT	c0;

	for (c0 = 0; c0 < mNumButtons; c0++)
	{
		if (mButtons[c0].mAction == a)
		{
			SINT size = mButtons[c0].mW;

			// load the texture
			UWORD	*data = new UWORD[size * size];

			if (CLevelEditor2::LoadAndScaleTGA(fname, data, size))
			{
				memcpy(mButtons[c0].mData, data, size * size * 2);
			}

			delete [] data;
		}
	}
#endif	
}

//******************************************************************************************
void	CPane::ChangeIconTexture(EEdAction a, CIconBank *icons, SINT no)
{
#if TARGET != DC
	// first, find the icon
	SINT	c0;

	for (c0 = 0; c0 < mNumButtons; c0++)
	{
		if (mButtons[c0].mAction == a)
		{
			SINT size = mButtons[c0].mW;

			icons->GetIcon(no, mButtons[c0].mData, size);
		}
	}
#endif	
}

//******************************************************************************************
//** CPaneTip
//******************************************************************************************
void	CPaneTip::Init()
{
#if TARGET != DC
	CPane::Init();

	ClearText();
#endif	
}

//******************************************************************************************
void	CPaneTip::Render(HDC hdc)
{
#if TARGET != DC
	CPane::Render(hdc);

	SINT	sx, sy;
	GetRenderPos(sx, sy);

//	EDITOR.SetTextColour(255,255,100);
//	EDITOR.DrawText(sx + 5, sy + 5, mText);

	EDITOR.SetTextColour(10,255,50);
	EDITOR.DrawText(sx + mSizeX - 120, sy + 5, "F6 to exit editor");
#endif	
}

//******************************************************************************************
void	CPaneTip::Redraw()
{
#if TARGET != DC
	CPane::Redraw();

	// ok - now this *really* is silly.
	mCounter += 0.03f;

	char	*t = mText;
	float	f = mCounter;
	SINT	x = 5;

	char	text[2] = "a";

	while (*t)
	{
		SINT r = 192 + SINT(sin(f       ) * 63.f);
		SINT g = 192 + SINT(sin(f * 1.4f) * 63.f);
		SINT b = 192 + SINT(sin(f * 0.6f) * 63.f);

		text[0] = *t;
		x += DrawFontText(x, 8, text, r, g, b);

		t++;
		f += 0.2f;
	};

//	DrawFontText(5, 10, mText, r, g, b);
	//	EDITOR.SetTextColour(255,255,100);
	//	EDITOR.DrawText(sx + 5, sy + 5, mText);
#endif	
}

//******************************************************************************************
//** CPaneLandscapeEd
//******************************************************************************************
void	CPaneLandscapeEd::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("IconsLED.tga");
	ASSERT(icons->IsValid());

	AddIconButton( 8,  10, icons, 0, ED_LED_SMALL			, "Small Landscape Tool");
	AddIconButton( 8,  50, icons, 1, ED_LED_MEDIUM			, "Medium Landscape Tool");
	AddIconButton( 8,  90, icons, 2, ED_LED_LARGE			, "Large Landscape Tool");
	AddIconButton( 8, 130, icons, 3, ED_LED_SELECT			, "Region Select");
	AddIconButton( 8, 170, icons, 4, ED_LED_CLIP_UP			, "Clip up to point");
	AddIconButton( 8, 210, icons, 5, ED_LED_CLIP_DOWN		, "Clip down to point");
	AddIconButton( 8, 250, icons, 6, ED_LED_SLOPE			, "Interpolate slope");
	AddIconButton( 8, 290, icons, 9, ED_LED_PARABOLA		, "Interpolate slope parabolically");
	AddIconButton(45, 210, icons, 7, ED_LED_RAISE_SELECTION	, "Raise Selection", TRUE, TRUE);
	AddIconButton(45, 250, icons, 8, ED_LED_LOWER_SELECTION	, "Lower Selection", TRUE, TRUE);
	AddIconButton(45, 290, icons, 10, ED_LED_PIT				, "Pit!");

	AddIconButton(45,  10, icons, 11, ED_LED_TAKEOFF			, "Flag as Take-off point");
	AddIconButton(45,  50, icons, 12, ED_LED_RAMP				, "Flag as Ramp");
	AddIconButton(45,  90, icons, 13, ED_LED_CLEAR				, "Clear Ramp & Take-off flags");
	AddIconButton(45, 130, icons, 14, ED_LED_UNIFY				, "Unify Ramp Flags");

	SelectButton(ED_LED_MEDIUM);

	mSelecting = LED_NOT_SELECTING;
	mStart.X = -1; 
	EDITOR.DisableSelection();

	delete icons;
#endif	
}

//******************************************************************************************
void	CPaneLandscapeEd::Activate()   
{
#if TARGET != DC
	CPane::Activate();
	EDITOR.RegisterActionPane(this);
#endif	
}

//******************************************************************************************
void	CPaneLandscapeEd::DeActivate()
{
#if TARGET != DC
	CPane::DeActivate(); 
	EDITOR.ReleaseActionPane( this);
#endif	
}

//******************************************************************************************
void	CPaneLandscapeEd::ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2)
{
#if TARGET != DC
	BOOL	changed_selection = FALSE;

	if ((IsButtonSelected(ED_LED_SMALL )) ||
		(IsButtonSelected(ED_LED_MEDIUM)) ||
	    (IsButtonSelected(ED_LED_LARGE )))
	{
		// modify land directly
		// different sense here.
		b1 = PLATFORM.IsMouseButtonPressed(0);
		b2 = PLATFORM.IsMouseButtonPressed(1);
			
		SINT size = 0;
		EEdAction a = ED_LED_SMALL;

		if (IsButtonSelected(ED_LED_SMALL )) {size = 1;		a = ED_LED_SMALL;};
		if (IsButtonSelected(ED_LED_MEDIUM)) {size = 3;;	a = ED_LED_MEDIUM;};
		if (IsButtonSelected(ED_LED_LARGE )) {size = 10;;	a = ED_LED_LARGE;};

		if (size == 0)
			return;

		if (b1 || b2)
		{
			// cool.   alter the landscape

			SINT	c0, c1;
			SINT	px = PLAYER(0)->GetCursor().X;
			SINT	py = PLAYER(0)->GetCursor().Y;

			if (px != -1)
			{
				EDITOR.RegisterLevelChange(a);
				for (c0 = px - size; c0 <= px + size; c0 ++)
					for (c1 = py - size; c1 <= py + size; c1 ++)
					{
						if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH) && (c1 < MAP_HEIGHT))
						{
							float val = float(cos(float(fabs(float(c0 - px))) / float(size) * PI / 2.f)) * 
										float(cos(float(fabs(float(c1 - py))) / float(size) * PI / 2.f));

							val *= 0.3f;

							if (val > 0)
							{
								if (b1)
									val = -val; // up!

								MAP.GetCell(CCellRef(c0, c1))->AddHeight(FToG(val));
							}
						}
					}
//				MAP.PrecalcCells();
			}
		}

		// Flatten tool
		if (PLATFORM.IsKeyDown(VK_SPACE))
		{
			// cool.   alter the landscape

			SINT	c0, c1;
			SINT	px = PLAYER(0)->GetCursor().X;
			SINT	py = PLAYER(0)->GetCursor().Y;

			float	average = 0;
			SINT	num_cells = 0;

			if (px != -1)
			{
				EDITOR.RegisterLevelChange(a);
				for (c0 = px - size; c0 <= px + size; c0 ++)
					for (c1 = py - size; c1 <= py + size; c1 ++)
					{
						if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH) && (c1 < MAP_HEIGHT))
						{
							float val = float(cos(float(fabs(float(c0 - px))) / float(size) * PI / 2.f)) * 
										float(cos(float(fabs(float(c1 - py))) / float(size) * PI / 2.f));

							if (val > 0)
							{
								average += GToF(MAP.GetCell(CCellRef(c0, c1))->GetUnderlyingHeight());
								num_cells ++;
							}
						}
					}

				float	dest = average / float(num_cells);

				for (c0 = px - size; c0 <= px + size; c0 ++)
					for (c1 = py - size; c1 <= py + size; c1 ++)
					{
						if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH) && (c1 < MAP_HEIGHT))
						{
							float val = float(cos(float(fabs(float(c0 - px))) / float(size) * PI / 2.f)) * 
										float(cos(float(fabs(float(c1 - py))) / float(size) * PI / 2.f));

							if (val > 0)
							{
								float h = GToF(MAP.GetCell(CCellRef(c0, c1))->GetUnderlyingHeight());
								MAP.GetCell(CCellRef(c0, c1))->AddHeight(FToG((dest - h) * 0.2f));
							}
						}
					}

				MAP.PrecalcCells();
			}
		}
		// Smooth
		if (PLATFORM.IsKeyDown(VK_TAB))
		{
			SINT	c0, c1;
			SINT	px = PLAYER(0)->GetCursor().X;
			SINT	py = PLAYER(0)->GetCursor().Y;

			if (px != -1)
			{
				EDITOR.RegisterLevelChange(a);
				for (c0 = px - size; c0 <= px + size; c0 ++)
					for (c1 = py - size; c1 <= py + size; c1 ++)
					{
						if ((c0 > 0) && (c1 > 0) && (c0 < MAP_WIDTH - 1) && (c1 < MAP_HEIGHT - 1))
						{
							float val = (GToF(MAP.GetCell(CCellRef(c0    , c1    ))->GetUnderlyingHeight()) * 10.f) +
									    (GToF(MAP.GetCell(CCellRef(c0 - 1, c1    ))->GetUnderlyingHeight()) *  4.f) +
									    (GToF(MAP.GetCell(CCellRef(c0 + 1, c1    ))->GetUnderlyingHeight()) *  4.f) +
									    (GToF(MAP.GetCell(CCellRef(c0    , c1 + 1))->GetUnderlyingHeight()) *  4.f) +
									    (GToF(MAP.GetCell(CCellRef(c0    , c1 - 1))->GetUnderlyingHeight()) *  4.f) +
									    (GToF(MAP.GetCell(CCellRef(c0 - 1, c1 - 1))->GetUnderlyingHeight()) *  2.f) +
									    (GToF(MAP.GetCell(CCellRef(c0 - 1, c1 + 1))->GetUnderlyingHeight()) *  2.f) +
									    (GToF(MAP.GetCell(CCellRef(c0 + 1, c1 - 1))->GetUnderlyingHeight()) *  2.f) +
									    (GToF(MAP.GetCell(CCellRef(c0 + 1, c1 + 1))->GetUnderlyingHeight()) *  2.f);

							MAP.GetCell(CCellRef(c0, c1))->SetHeight(FToG(val / 34.f));
						}
					}

				MAP.PrecalcCells();
			}
		}
	}
	else if ((IsButtonSelected(ED_LED_SELECT)))
	{
		switch (mSelecting)
		{
		case LED_NOT_SELECTING:
			if (b1)
			{
				mStart = PLAYER(0)->GetCursor();
				if (mStart.X == -1)
					break; // not on map...
				mEnd = mStart;
				mSelecting = LED_SELECTING;
				changed_selection = TRUE;
			}
			break;

		case LED_SELECTING:
			changed_selection = TRUE;
			if (CONTROLS.IsButtonReleasedDB(BUTTON_FIRE_PRIMARY))
			{
				// have we actually selected anything?
				if ((mEnd.X == mStart.X) || (mEnd.Y == mStart.Y))
				{
					mSelecting = LED_SELECTING;
					break;
				}

				// reorder

				SINT	foo;
				if (mEnd.X < mStart.X)
				{
					foo = mEnd.X;
					mEnd.X = mStart.X;
					mStart.X = foo;
				}
				if (mEnd.Y < mStart.Y)
				{
					foo = mEnd.Y;
					mEnd.Y = mStart.Y;
					mStart.Y = foo;
				}

				mSelecting = LED_SELECTED;
			}
			else
			{
				if (PLAYER(0)->GetCursor().X != -1)
					mEnd = PLAYER(0)->GetCursor();
			}
			break;
		case LED_SELECTED:
			if (b2)
			{
				// clear rectangle
				changed_selection = TRUE;
				mStart.X = -1;
				mSelecting = LED_NOT_SELECTING;
			}
			break;
		default:
			ASSERT(0); // ???!
			break;
		};
	}
	else if ((IsButtonSelected(ED_LED_CLIP_UP)))
	{
		if (b1)
		{
			ASSERT(EDITOR.IsSelectionActive());

			SINT	px = PLAYER(0)->GetCursor().X;
			SINT	py = PLAYER(0)->GetCursor().Y;

			if (px != -1)
			{
				EDITOR.RegisterLevelChange(ED_LED_CLIP_UP);
				CCellRef start, end;
				EDITOR.GetSelection(start, end);

				SINT	c0, c1;
				GINT	clip_height = MAP.GetCell(CCellRef(px, py))->GetUnderlyingHeight();

				for (c0 = start.Y; c0 <= end.Y; c0++)
					for (c1 = start.X; c1 <= end.X; c1++)
					{
						GINT h = MAP.GetCell(CCellRef(c1, c0))->GetUnderlyingHeight();

						if (h > clip_height)
							MAP.GetCell(CCellRef(c1, c0))->SetHeight(clip_height);
					}
				MAP.PrecalcCells();
			}
		}
	}
	else if ((IsButtonSelected(ED_LED_CLIP_DOWN)))
	{
		if (b1)
		{
			ASSERT(EDITOR.IsSelectionActive());

			SINT	px = PLAYER(0)->GetCursor().X;
			SINT	py = PLAYER(0)->GetCursor().Y;

			if (px != -1)
			{
				EDITOR.RegisterLevelChange(ED_LED_CLIP_DOWN);
				CCellRef start, end;
				EDITOR.GetSelection(start, end);

				SINT	c0, c1;
				GINT	clip_height = MAP.GetCell(CCellRef(px, py))->GetUnderlyingHeight();

				for (c0 = start.Y; c0 <= end.Y; c0++)
					for (c1 = start.X; c1 <= end.X; c1++)
					{
						GINT h = MAP.GetCell(CCellRef(c1, c0))->GetUnderlyingHeight();

						if (h < clip_height)
							MAP.GetCell(CCellRef(c1, c0))->SetHeight(clip_height);
					}
				MAP.PrecalcCells();
			}
		}
	}

	// tell the editor what's selected
	if (changed_selection)
	{
		if (mStart.X == -1)
			EDITOR.DisableSelection();
		else
		{
			// give it a sorted list
			CCellRef s, e;

			if (mStart.X > mEnd.X)
			{
				s.X = mEnd.X;
				e.X = mStart.X;
			}
			else
			{
				s.X = mStart.X;
				e.X = mEnd.X;
			}

			if (mStart.Y > mEnd.Y)
			{
				s.Y = mEnd.Y;
				e.Y = mStart.Y;
			}
			else
			{
				s.Y = mStart.Y;
				e.Y = mEnd.Y;
			}

			EDITOR.DeclareSelection(s, e);
		}
	}
#endif	
}

//******************************************************************************************
void	CPaneLandscapeEd::UpdateState()
{
#if TARGET != DC
	if (EDITOR.IsSelectionActive())
	{
		ActivateButton(ED_LED_CLIP_UP);
		ActivateButton(ED_LED_CLIP_DOWN);
		ActivateButton(ED_LED_SLOPE);
		ActivateButton(ED_LED_PARABOLA);
		ActivateButton(ED_LED_PIT);
		ActivateButton(ED_LED_RAISE_SELECTION);
		ActivateButton(ED_LED_LOWER_SELECTION);
		ActivateButton(ED_LED_TAKEOFF);
		ActivateButton(ED_LED_RAMP);
		ActivateButton(ED_LED_CLEAR);
		ActivateButton(ED_LED_UNIFY);
	}
	else
	{
		DeActivateButton(ED_LED_CLIP_UP);
		DeActivateButton(ED_LED_CLIP_DOWN);
		DeActivateButton(ED_LED_SLOPE);
		DeActivateButton(ED_LED_PARABOLA);
		DeActivateButton(ED_LED_PIT);
		DeActivateButton(ED_LED_RAISE_SELECTION);
		DeActivateButton(ED_LED_LOWER_SELECTION);
		DeActivateButton(ED_LED_TAKEOFF);
		DeActivateButton(ED_LED_RAMP);
		DeActivateButton(ED_LED_CLEAR);
		DeActivateButton(ED_LED_UNIFY);
	}
#endif	
}

//******************************************************************************************
void	CPaneLandscapeEd::ProcessAction(EEdAction action)
{
#if TARGET != DC
	switch (action)
	{
	case ED_LED_SLOPE:
		// ok - interpolate a slope based on the 4 corners.
		{
			ASSERT(EDITOR.IsSelectionActive());
			EDITOR.RegisterLevelChange(action);

			CCellRef start, end;
			EDITOR.GetSelection(start, end);
			// get the four heights
			GINT	xy = MAP.GetCell(start)->GetUnderlyingHeight();
			GINT	Xy = MAP.GetCell(CCellRef(end.X, start.Y))->GetUnderlyingHeight();
			GINT	xY = MAP.GetCell(CCellRef(start.X, end.Y))->GetUnderlyingHeight();
			GINT	XY = MAP.GetCell(end)->GetUnderlyingHeight();

			SINT	c0, c1;
			for (c0 = start.Y; c0 <= end.Y; c0++)
				for (c1 = start.X; c1 <= end.X; c1++)
				{
					float	f0 = float(c0 - start.Y) / float(end.Y - start.Y);
					float	f1 = float(c1 - start.X) / float(end.X - start.X);
					
					GINT	h1 = (FToG(1.f - f1) * xy) + (FToG(f1) * Xy);
					GINT	h2 = (FToG(1.f - f1) * xY) + (FToG(f1) * XY);

					GINT	height = (FToG(1.f - f0) * h1) + (FToG(f0) * h2);

					MAP.GetCell(CCellRef(c1, c0))->SetHeight(height);
				}

			MAP.PrecalcCells();
			SelectButton(ED_LED_SELECT);
		}
		break;
	case ED_LED_PARABOLA:
		// ok - interpolate a slope based on the 4 corners, parabolically!
		{
			ASSERT(EDITOR.IsSelectionActive());
			EDITOR.RegisterLevelChange(action);

			CCellRef start, end;
			EDITOR.GetSelection(start, end);
			// get the four heights
			GINT	xy = MAP.GetCell(start)->GetUnderlyingHeight();
			GINT	Xy = MAP.GetCell(CCellRef(end.X, start.Y))->GetUnderlyingHeight();
			GINT	xY = MAP.GetCell(CCellRef(start.X, end.Y))->GetUnderlyingHeight();
			GINT	XY = MAP.GetCell(end)->GetUnderlyingHeight();

			GINT	lowest = xy;
			if (Xy > lowest) lowest = Xy;
			if (xY > lowest) lowest = xY;
			if (XY > lowest) lowest = XY;

			GINT	highest = xy;
			if (Xy < highest) highest = Xy;
			if (xY < highest) highest = xY;
			if (XY < highest) highest = XY;

			SINT	c0, c1;
			for (c0 = start.Y; c0 <= end.Y; c0++)
				for (c1 = start.X; c1 <= end.X; c1++)
				{
					float	f0 = float(c0 - start.Y) / float(end.Y - start.Y);
					float	f1 = float(c1 - start.X) / float(end.X - start.X);
					
					GINT	h1 = (FToG(1.f - f1) * xy) + (FToG(f1) * Xy);
					GINT	h2 = (FToG(1.f - f1) * xY) + (FToG(f1) * XY);

					GINT	height = (FToG(1.f - f0) * h1) + (FToG(f0) * h2);

					// parabola
					GINT	dh = height - lowest;
					GINT	dhl = highest - lowest;
					if (GABS(dhl) > G(0, 1000))
						height = (dh * dh * dh * dh) / (dhl * dhl * dhl);

					MAP.GetCell(CCellRef(c1, c0))->SetHeight(height + lowest);
				}

			MAP.PrecalcCells();
			SelectButton(ED_LED_SELECT);
		}
		break;
	case ED_LED_TAKEOFF:
		{
			ASSERT(EDITOR.IsSelectionActive());
			EDITOR.RegisterLevelChange(action);

			CCellRef start, end;
			EDITOR.GetSelection(start, end);

			SINT	c0, c1;
			for (c0 = start.Y; c0 < end.Y; c0++)
				for (c1 = start.X; c1 < end.X; c1++)
				{
					MAP.GetCell(CCellRef(c1, c0))->SetTakeOff();
				}

			MAP.PrecalcCells();
			SelectButton(ED_LED_SELECT);
		}
		break;
	case ED_LED_CLEAR:
		{
			ASSERT(EDITOR.IsSelectionActive());
			EDITOR.RegisterLevelChange(action);

			CCellRef start, end;
			EDITOR.GetSelection(start, end);

			SINT	c0, c1;
			for (c0 = start.Y; c0 < end.Y; c0++)
				for (c1 = start.X; c1 < end.X; c1++)
				{
					MAP.GetCell(CCellRef(c1, c0))->ClearTakeOff();
					MAP.GetCell(CCellRef(c1, c0))->SetRampNo(0);
				}

			MAP.PrecalcCells();
			SelectButton(ED_LED_SELECT);
		}
		break;
	case ED_LED_PIT:
		// Turn into pit
		{
			ASSERT(EDITOR.IsSelectionActive());
			EDITOR.RegisterLevelChange(action);

			CCellRef start, end;
			EDITOR.GetSelection(start, end);

			SINT	c0, c1;
			for (c0 = start.Y; c0 <= end.Y; c0++)
				for (c1 = start.X; c1 <= end.X; c1++)
				{
					MAP.GetCell(CCellRef(c1, c0))->SetHeight(PIT_HEIGHT);
				}

			MAP.PrecalcCells();
			SelectButton(ED_LED_SELECT);
		}

		break;

	case ED_LED_RAISE_SELECTION:
	case ED_LED_LOWER_SELECTION:
		EDITOR.RegisterLevelChange(action);

		GINT delta = (action == ED_LED_RAISE_SELECTION) ? (-G(0, 20000)) : G(0, 20000);

		CCellRef start, end;
		EDITOR.GetSelection(start, end);

		SINT	c0, c1;
		for (c0 = start.Y; c0 <= end.Y; c0++)
			for (c1 = start.X; c1 <= end.X; c1++)
			{
				MAP.GetCell(CCellRef(c1, c0))->AddHeight(delta);
			}

		MAP.PrecalcCells();
	};
#endif	
}
//******************************************************************************************
//** CPaneTileDetail1
//******************************************************************************************
void	CPaneTileDetail1::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("tb1.tga");

	AddIconButton( 5,    10, icons, 0, ED_BLEND_ICON_SET, "Set texture");
	AddIconButton( 5,    50, icons, 1, ED_BLEND_ICON_ADD, "Add to texture");
	AddIconButton( 5,    90, icons, 2, ED_BLEND_ICON_SUB, "Subtract from texture");

	delete icons;
#endif	
}
//******************************************************************************************
//** CPaneTileDetail2
//******************************************************************************************
void	CPaneTileDetail2::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("tb2.tga");

	AddIconButton( 5,   10, icons, 0, ED_BLEND_ICON_FLAT, "small tool");
	AddIconButton( 5,   50, icons, 1, ED_BLEND_ICON_STEEP, "medium tool");
	AddIconButton( 5,   90, icons, 2, ED_BLEND_ICON_SMALL, "big Tool");
	AddIconButton( 5,   130,icons, 3, ED_BLEND_ICON_SMALL, "Airbrush");

	delete icons;
#endif	
}

//******************************************************************************************
//** CPaneTileDetail3
//******************************************************************************************
void	CPaneTileDetail3::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("tb3.tga");

	AddIconButton( 5,    10, icons, 0, ED_BLEND_ICON_SET, "Set texture 0");
	AddIconButton( 5,    50, icons, 1, ED_BLEND_ICON_SUB, "Set texture 1");
	AddIconButton( 5,    90, icons, 2, ED_BLEND_ICON_LARGE, "Set texture 2");

	delete icons;
#endif	
}

//******************************************************************************************
//** CPaneTileDetail4
//******************************************************************************************
void	CPaneTileDetail4::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("tb4.tga");

	AddIconButton( 5,   10, icons, 0, ED_BLEND_ICON_STEEP, "edit texture");
	AddIconButton( 5,   50, icons, 1, ED_BLEND_ICON_MEDIUM, "edit mix");
	AddIconButton( 5,   90, icons, 2, ED_BLEND_ICON_LARGE, "wimpy, 'I promise not to fuck anything up' mode");
	AddIconButton( 5,   130,icons, 3, ED_BLEND_ICON_LARGE, "Hard mother, stomp over everything mode");
	AddIconButton( 5,   170,icons, 4, ED_BLEND_ICON_LARGE, "Clean cell");

	delete icons;
#endif	
}

//******************************************************************************************
//** CPaneTileEd
//******************************************************************************************
void	CPaneTileEd::Init()
{
#if TARGET != DC
	CPane::Init();

	// set up list box
	CListIcon	*l = CreateAndGetListIcon();

	l->Resize(7, 7, 92, 323);
	// ok, now we need to load and add the textures...

	// scan filenames!
	char			files[] = "data\\textures\\tiles\\*.tga";
	WIN32_FIND_DATA data;
	char			name[256];
	UWORD			tiledata[LISTICON_ICON_SIZE * LISTICON_ICON_SIZE];

	HANDLE	handle = FindFirstFile(files, &data);

	if (handle != INVALID_HANDLE_VALUE)
	{
		while (TRUE)
		{
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				// cool - we got a texture
				sprintf(name, "tiles\\%s", data.cFileName);
				if (CLevelEditor2::LoadAndScaleTGA(name, tiledata, LISTICON_ICON_SIZE))
				{
					// it loaded ok!
					
					l->AddEntry(tiledata, data.cFileName);
				}
			}

			if (!(FindNextFile(handle, &data)))
				break;
		}
		FindClose(handle);
	}

	UpdateState();

	Redraw();

	// setup PaneTileDetail
	mPaneTileDetail1 = new CPaneTileDetail1(44, 170, 111, 57, EPS_LEFT);
	mPaneTileDetail1->Init();
	mPaneTileDetail2 = new CPaneTileDetail2(44, 170, 111, 230, EPS_BOTTOM);
	mPaneTileDetail2->Init();
	mPaneTileDetail3 = new CPaneTileDetail3(44, 130, 158, 57, EPS_TOP);
	mPaneTileDetail3->Init();
	mPaneTileDetail4 = new CPaneTileDetail4(44, 210, 158, 190, EPS_RIGHT);
	mPaneTileDetail4->Init();
#endif	
}

//******************************************************************************************
void	CPaneTileEd::UpdateState()
{
}

//******************************************************************************************
void	CPaneTileEd::Activate()   
{
#if TARGET != DC
	CPane::Activate();
	EDITOR.RegisterActionPane(this);
	mPaneTileDetail1->Activate();
	mPaneTileDetail2->Activate();
	mPaneTileDetail3->Activate();
	mPaneTileDetail4->Activate();
#endif	
}

//******************************************************************************************
void	CPaneTileEd::DeActivate()
{
#if TARGET != DC
	CPane::DeActivate(); 
	EDITOR.ReleaseActionPane( this);
	mPaneTileDetail1->DeActivate();
	mPaneTileDetail2->DeActivate();
	mPaneTileDetail3->DeActivate();
	mPaneTileDetail4->DeActivate();
#endif	
}

//******************************************************************************************
void	CPaneTileEd::ProcessAction(EEdAction action)
{
#if TARGET != DC
	Redraw();
#endif	
}

//******************************************************************************************
void	CPaneTileEd::ProcessAction2(EEdAction action)
{
}

//******************************************************************************************
BOOL	CPaneTileEd::Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT mw)
{
#if TARGET != DC
	BOOL ret = CPane::Process(x, y, b1, b2, mw);

	return ret;
#endif	
}


//******************************************************************************************
void	CPaneTileEd::ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2)
{
#if TARGET != DC
	CCellRef c = PLAYER(0)->GetCursor();

	EDITOR.Set442Draw(-1);
	if (c.X != -1)
	{
		int		blendmode	= mPaneTileDetail1->GetCurrentButton();
		int		brushsize	= mPaneTileDetail2->GetCurrentButton();
		int		stage		= mPaneTileDetail3->GetCurrentButton();
		int		mode		= mPaneTileDetail4->GetCurrentButton();
		int		add_this_much = 16;

		if(blendmode==1) add_this_much = 2;
		else if(blendmode==2) add_this_much = -2;

		if (CONTROLS.IsButtonPressed(BUTTON_FIRE_PRIMARY))
		{
			char	fname[256];
			sprintf(fname, "tiles\\%s", mListIcon->mEntries[mListIcon->mCurrentEntry].mText);
			int	tn = MAP.GetTexNo(fname);

			EDITOR.RegisterLevelChange((blendmode<<14)+(brushsize<<12)+(stage<<10)+(mode<<8)+tn+999999);

			switch(mode)
			{
			case 0:		// set texture in selected channel and on Jez-o-map
				BOOL	r;

				if(stage==0)
				{
					r = MAP.SetCellTexture(c, fname);
					if (!r)	CONSOLE.AddString("Failed to load / allocate texture...");
				}
				tn = MAP.GetTexNo(fname);
				if (!tn)	CONSOLE.AddString("Failed to load / allocate 44texture...");
				else
				{	
					MAP.Set44Texture(stage,c,tn);
				}
				break;
			case 1:
			case 2:
			case 3:
				int	xc, yc;
				for(yc=-brushsize; yc<=brushsize; yc++)
				{
					for(xc=-brushsize; xc<=brushsize; xc++)
					{
						CCellRef	shadespot;
						shadespot.Y = c.Y+yc;
						shadespot.X = c.X+xc;
						if(sqrt(xc*xc+yc*yc)<=brushsize)
						{
							if(	(shadespot.X>=0)&&(shadespot.X<MAP.mMapWidth) &&
								(shadespot.Y>=0)&&(shadespot.Y<MAP.mMapHeight))
							{
								if(mode==1)		// set vertex
								{
									MAP.TweakVertex(stage,shadespot,add_this_much);
								}
								MAP.Create44Masks(shadespot,shadespot);
								if(mode==2) // wimp mode, will only do anything if it won't upset anyone
								{
									tn = MAP.GetTexNo(fname);
									if (!tn)	CONSOLE.AddString("Failed to load / allocate 44texture...");
									else
									{
										if(MAP.TentativeSetTex(stage, tn, shadespot))	MAP.TweakVertex(stage,shadespot,add_this_much);
									}
								}
								if(mode==3) // hard bastard mode
								{
									tn = MAP.GetTexNo(fname);
									if (!tn)	CONSOLE.AddString("Failed to load / allocate 44texture...");
									else
									{
										MAP.BrutalSetTex(stage, tn, shadespot);
										MAP.TweakVertex(stage, shadespot, add_this_much);
									}
								}
							}
						}
					}
				}
				break;
			case 4: // trash the other passes

				r = MAP.SetCellTexture(c, fname);
				if (!r)	CONSOLE.AddString("Failed to load / allocate texture...");
				tn = MAP.GetTexNo(fname);
				if (!tn)	CONSOLE.AddString("Failed to load / allocate 44texture...");
					MAP.Set44Texture(0,c,tn);
				MAP.Clear44Channel(1, c);
				MAP.Clear44Channel(2, c);
				MAP.Create44Masks(c,c);
				break;
			}
		}

		else if (CONTROLS.IsButtonPressed(BUTTON_FIRE_SECONDARY))
		{
			EDITOR.RegisterLevelChange(ED_TILE_EDIT);
			MAP.SetCellTextureDefault(c);
			if((mode==1)||(mode==2))
			{
				MAP.ClearChannel(stage,c);
			}
			MAP.Create44Masks(c,c);
		}
		else
		{
			EDITOR.Set442Draw((c.Y>>2)*(MAP_WIDTH>>2)+(c.X>>2));
		}
	}

	// upload texture
	if (CONTROLS.IsButtonPressed(BUTTON_UPLOAD_TEXTURE))
	{
		SINT	fftd = EDITOR.Get442Draw();

		if (fftd >= 0)
		{
			C4x4	*ff = &MAP.m4x4s[fftd];
			SINT	stage = mPaneTileDetail3->GetCurrentButton();

			CTexture	*t = MAP.GetMapTexture(ff->GetTexture(stage));

			if (t)
			{
				// try and select this
				SINT	c0;

				for (c0 = 0; c0 < mListIcon->mNumEntries; c0 ++)
				{
					if (!(stricmp(mListIcon->mEntries[c0].mText, t->GetName() + 6)))  // +6 to skip the "tiles\\"
					{
						mListIcon->mCurrentEntry = c0;
						Redraw();

						break;
					}
				}
			}
		}
	}
#endif	
}	

//******************************************************************************************
//** CPaneTriggerEd
//******************************************************************************************
void	CPaneTriggerEd::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("IconsTrigger.tga");


	ASSERT(icons->IsValid());

	AddIconButton(8,  10, icons, 0, ED_TRIGGER_TRIGGER	, "Basic Trigger");
	AddIconButton(8,  50, icons, 1, ED_TRIGGER_ICE   	, "Ice");
	AddIconButton(8,  90, icons, 2, ED_TRIGGER_BOOSTU	, "Boost Up");
	AddIconButton(8, 130, icons, 3, ED_TRIGGER_BOOSTD	, "Boost Down");
	AddIconButton(8, 170, icons, 4, ED_TRIGGER_BOOSTR	, "Boost Right");
	AddIconButton(8, 210, icons, 5, ED_TRIGGER_BOOSTL	, "Boost Left");

	DeActivateButton(ED_TRIGGER_TRIGGER);
	SelectButton(ED_TRIGGER_ICE);

	delete icons;
#endif	
}

//******************************************************************************************
void	CPaneTriggerEd::Activate()   
{
#if TARGET != DC
	CPane::Activate();
	EDITOR.RegisterActionPane(this);
#endif	
}

//******************************************************************************************
void	CPaneTriggerEd::DeActivate()
{
#if TARGET != DC
	CPane::DeActivate(); 
	EDITOR.ReleaseActionPane( this);
#endif	
}

//******************************************************************************************
void	CPaneTriggerEd::ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2)
{
#if TARGET != DC
	CCellRef c = PLAYER(0)->GetCursor();

	if (c.X != -1)
	{
		if (CONTROLS.IsButtonPressed(BUTTON_FIRE_PRIMARY))
		{
			if (IsButtonSelected(ED_TRIGGER_TRIGGER))
			{
			}
			else
			{
				EDITOR.RegisterLevelChange(ED_TRIGGER_EDIT);
				ECellType type = CT_NORMAL;

				SINT size = 1;

				if (IsButtonSelected(ED_TRIGGER_ICE))		{type = CT_ICE;	size = 4;}
				if (IsButtonSelected(ED_TRIGGER_BOOSTU))	{type = CT_BOOSTU; size = 2;}
				if (IsButtonSelected(ED_TRIGGER_BOOSTD))	{type = CT_BOOSTD; size = 2;}
				if (IsButtonSelected(ED_TRIGGER_BOOSTR))	{type = CT_BOOSTR; size = 2;}
				if (IsButtonSelected(ED_TRIGGER_BOOSTL))	{type = CT_BOOSTL; size = 2;}

				c.X &= (~(size - 1));
				c.Y &= (~(size - 1));

				SINT	c0, c1;
				for (c0 = 0; c0 < size; c0 ++, c.Y ++)
				{
					for (c1 = 0; c1 < size; c1 ++, c.X ++)
						MAP.GetCell(c)->SetType(type);
					c.X -= size;
				}

			}
		}

		if (CONTROLS.IsButtonPressed(BUTTON_FIRE_SECONDARY))
		{
			EDITOR.RegisterLevelChange(ED_TRIGGER_EDIT);

			SINT size = 1;
			switch (MAP.GetCell(c)->GetType())
			{
			case CT_ICE:
				size = 4;	break;
			case CT_BOOSTU:
			case CT_BOOSTD:
			case CT_BOOSTR:
			case CT_BOOSTL:
				size = 2;	break;
			}

				c.X &= (~(size - 1));
				c.Y &= (~(size - 1));

				SINT	c0, c1;
				for (c0 = 0; c0 < size; c0 ++, c.Y ++)
				{
					for (c1 = 0; c1 < size; c1 ++, c.X ++)
						MAP.GetCell(c)->SetType(CT_NORMAL);
					c.X -= size;
				}
		}
	}
#endif	
}
//******************************************************************************************
//** CPaneMeshSelect
//******************************************************************************************
void	CPaneMeshSelect::Init()
{
#if TARGET != DC
	CPane::Init();

	// set up list box
	CListText	*l = CreateAndGetListText();

	l->Resize(10, 10, 164, 313);

	// scan filenames!
	char			files[] = "data\\meshes\\*.msh";
	WIN32_FIND_DATA data;

	HANDLE	handle = FindFirstFile(files, &data);

	if (handle != INVALID_HANDLE_VALUE)
	{
		while (TRUE)
		{
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				// cool - we got a mesh
				char	name[256];
				strcpy(name, data.cFileName);
				strlwr(name);
				l->AddEntry(name, 0, 0);
			}

			if (!(FindNextFile(handle, &data)))
				break;
		}
		FindClose(handle);
	}
#endif	
}

//******************************************************************************************
//** CPaneThingDetail
//******************************************************************************************

void	CPaneThingDetail::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons;
	
	//***********************
	// Yaw, Pitch, Roll icons
	icons = new CIconBank("scroll16.tga");
	ASSERT(icons->IsValid());

	AddIconButton(50 , 120, icons, 0, ED_TD_YAW_L90		, "Yaw 90 Degrees Left"		,FALSE);
	AddIconButton(68 , 120, icons, 1, ED_TD_YAW_L		, "Yaw Left"				,FALSE, TRUE);
	AddIconButton(130, 120, icons, 2, ED_TD_YAW_R		, "Yaw Right"				,FALSE, TRUE);
	AddIconButton(148, 120, icons, 3, ED_TD_YAW_R90		, "Yaw 90 Degrees Right"	,FALSE);

	AddIconButton(50 , 140, icons, 0, ED_TD_PITCH_L90	, "Pitch 90 Degrees Left"	,FALSE);
	AddIconButton(68 , 140, icons, 1, ED_TD_PITCH_L		, "Pitch Left"				,FALSE, TRUE);
	AddIconButton(130, 140, icons, 2, ED_TD_PITCH_R		, "Pitch Right"				,FALSE, TRUE);
	AddIconButton(148, 140, icons, 3, ED_TD_PITCH_R90	, "Pitch 90 Degrees Right"	,FALSE);

	AddIconButton(50 , 160, icons, 0, ED_TD_ROLL_L90	, "Roll 90 Degrees Left"	,FALSE);
	AddIconButton(68 , 160, icons, 1, ED_TD_ROLL_L		, "Roll Left"				,FALSE, TRUE);
	AddIconButton(130, 160, icons, 2, ED_TD_ROLL_R		, "Roll Right"				,FALSE, TRUE);
	AddIconButton(148, 160, icons, 3, ED_TD_ROLL_R90	, "Roll 90 Degrees Right"	,FALSE);

	delete icons;

	//***********************
	// thing trigger icons
	icons = new CIconBank("things.tga");
	ASSERT(icons->IsValid());

	AddIconButton(130,  182, icons, 1, ED_TD_TARGET_THING	, "Target Thing");
	AddIconButton(130,  222, icons, 1, ED_TD_OID			, "Object ID");
	AddIconButton(130,  262, icons, 1, ED_TD_CHAIN_TRIGGER	, "Chain Trigger Thing");

	delete icons;

	//***********************
	// initial state + trigger response stuff

	icons = new CIconBank("scroll16.tga");
	ASSERT(icons->IsValid());

	AddIconButton(110,  50, icons, 1, ED_TD_XDOWN					, "Decrease X"			,FALSE, FALSE);
	AddIconButton(148,  50, icons, 2, ED_TD_XUP						, "Increase X"			,FALSE, FALSE);
	AddIconButton(110,  70, icons, 1, ED_TD_YDOWN					, "Decrease Y"			,FALSE, FALSE);
	AddIconButton(148,  70, icons, 2, ED_TD_YUP						, "Increase Y"			,FALSE, FALSE);
	AddIconButton(110,  90, icons, 1, ED_TD_ZDOWN					, "Decrease Z"			,FALSE, TRUE);
	AddIconButton(148,  90, icons, 2, ED_TD_ZUP						, "Increase Z"			,FALSE, TRUE);

	AddIconButton(86 , 320, icons, 1, ED_TD_INITIAL_MODE_DOWN		, "Initial Mode Down"	,FALSE, TRUE);
	AddIconButton(148, 320, icons, 2, ED_TD_INITIAL_MODE_UP			, "Initial Mode Up"		,FALSE, TRUE);

	AddIconButton(86 , 350, icons, 1, ED_TD_TRIGGER_RESPONSE_DOWN	, "Trigger Mode Down"	,FALSE, TRUE);
	AddIconButton(148, 350, icons, 2, ED_TD_TRIGGER_RESPONSE_UP		, "Trigger Mode Up"		,FALSE, TRUE);
	AddIconButton(86 , 370, icons, 1, ED_TD_UNTRIGGER_RESPONSE_DOWN	, "UnTrigger Mode Down"	,FALSE, TRUE);
	AddIconButton(148, 370, icons, 2, ED_TD_UNTRIGGER_RESPONSE_UP	, "UnTrigger Mode Up"	,FALSE, TRUE);

	AddIconButton(86 , 400, icons, 1, ED_TD_COUNT0_DOWN				, "Counter 0 Down"		,FALSE, TRUE);
	AddIconButton(148, 400, icons, 2, ED_TD_COUNT0_UP				, "Counter 0 Up"		,FALSE, TRUE);
	AddIconButton(86 , 420, icons, 1, ED_TD_COUNT1_DOWN				, "Counter 1 Down"		,FALSE, TRUE);
	AddIconButton(148, 420, icons, 2, ED_TD_COUNT1_UP				, "Counter 1 Up"		,FALSE, TRUE);
	AddIconButton(86 , 440, icons, 1, ED_TD_COUNT2_DOWN				, "Counter 2 Down"		,FALSE, TRUE);
	AddIconButton(148, 440, icons, 2, ED_TD_COUNT2_UP				, "Counter 2 Up"		,FALSE, TRUE);
	AddIconButton(86 , 460, icons, 1, ED_TD_COUNT3_DOWN				, "Counter 3 Down"		,FALSE, TRUE);
	AddIconButton(148, 460, icons, 2, ED_TD_COUNT3_UP				, "Counter 3 Up"		,FALSE, TRUE);

	AddIconButton(86 , 490, icons, 1, ED_TD_BOOL0_DOWN				, "Bool 0 Down"			,FALSE, TRUE);
	AddIconButton(148, 490, icons, 2, ED_TD_BOOL0_UP				, "Bool 0 Up"			,FALSE, TRUE);
	AddIconButton(86 , 510, icons, 1, ED_TD_BOOL1_DOWN				, "Bool 1 Down"			,FALSE, TRUE);
	AddIconButton(148, 510, icons, 2, ED_TD_BOOL1_UP				, "Bool 1 Up"			,FALSE, TRUE);
	AddIconButton(86 , 530, icons, 1, ED_TD_BOOL2_DOWN				, "Bool 2 Down"			,FALSE, TRUE);
	AddIconButton(148, 530, icons, 2, ED_TD_BOOL2_UP				, "Bool 2 Up"			,FALSE, TRUE);
	AddIconButton(86 , 550, icons, 1, ED_TD_BOOL3_DOWN				, "Bool 3 Down"			,FALSE, TRUE);
	AddIconButton(148, 550, icons, 2, ED_TD_BOOL3_UP				, "Bool 3 Up"			,FALSE, TRUE);

	delete icons;

	//***********************
	// Mesh Select icon
	icons = new CIconBank("things.tga");
	ASSERT(icons->IsValid());

	AddIconButton(130,  580, icons, 1, ED_TD_MESH_SELECT	, "Select Mesh");

	delete icons;

	//***********************
	// mesh select pane
	mPaneMeshSelect = new CPaneMeshSelect(180, 343, 280, 57, EPS_BOTTOM);
	mPaneMeshSelect->Init();

	mPaneMeshSelect->DeActivate();
	mPaneMeshSelect->GetListText()->mCurrentEntry = -1;

	mCachedThing = NULL;
	UpdateState();
#endif	
}

//******************************************************************************************
void	CPaneThingDetail::Activate()   
{
#if TARGET != DC
	CPane::Activate();
	EDITOR.RegisterActionPane(this);

	mPaneMeshSelect->GetListText()->mCurrentEntry = -1;

	UpdateState();

	DeselectButton();
#endif	
}

//******************************************************************************************
void	CPaneThingDetail::DeActivate()
{
#if TARGET != DC
	CPane::DeActivate(); 
	EDITOR.ReleaseActionPane( this);
	mPaneMeshSelect->DeActivate();
#endif	
}

//******************************************************************************************
void	CPaneThingDetail::ProcessAction(EEdAction action)
{
#if TARGET != DC
	CThing		*s = EDITOR.GetSelectedThing();

	if (s)
	{
		CInitThing	i;
		s->WriteData(i);

		BOOL	recreate = TRUE;

		switch(action)
		{
		case ED_TD_XUP:
			EDITOR.RegisterLevelChange(ED_TD_XUP);
			i.mPos.X += GINT_HALF;
			break;

		case ED_TD_XDOWN:
			EDITOR.RegisterLevelChange(ED_TD_XDOWN);
			i.mPos.X -= GINT_HALF;
			break;

		case ED_TD_YUP:
			EDITOR.RegisterLevelChange(ED_TD_YUP);
			i.mPos.Y += GINT_HALF;
			break;

		case ED_TD_YDOWN:
			EDITOR.RegisterLevelChange(ED_TD_YDOWN);
			i.mPos.Y -= GINT_HALF;
			break;

		case ED_TD_ZUP:
			EDITOR.RegisterLevelChange(ED_TD_ZUP);
			i.mPos.Z += GINT_HALF;
			break;

		case ED_TD_ZDOWN:
			EDITOR.RegisterLevelChange(ED_TD_ZDOWN);
			i.mPos.Z -= GINT_HALF;
			break;

		case ED_TD_YAW_L90:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mYaw -= G_PI / 2;
			break;

		case ED_TD_YAW_L:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mYaw -= G_PI / 180;
			break;

		case ED_TD_YAW_R:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mYaw += G_PI / 180;
			break;

		case ED_TD_YAW_R90:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mYaw += G_PI / 2;
			break;


		case ED_TD_PITCH_L90:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mPitch -= G_PI / 2;
			break;

		case ED_TD_PITCH_L:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mPitch -= G_PI / 180;
			break;

		case ED_TD_PITCH_R:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mPitch += G_PI / 180;
			break;

		case ED_TD_PITCH_R90:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mPitch += G_PI / 2;
			break;


		case ED_TD_ROLL_L90:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mRoll -= G_PI / 2;
			break;

		case ED_TD_ROLL_L:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mRoll -= G_PI / 180;
			break;

		case ED_TD_ROLL_R:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mRoll += G_PI / 180;
			break;

		case ED_TD_ROLL_R90:
			EDITOR.RegisterLevelChange(ED_TD_YAW_L);
			i.mRoll += G_PI / 2;
			break;

		case ED_TD_INITIAL_MODE_UP:
			EDITOR.RegisterLevelChange(ED_TD_INITIAL_MODE_UP);
			i.mInitialMode =  EInitialMode(int(i.mInitialMode) + 1);
			if (i.mInitialMode == IM_MAX)
				i.mInitialMode = EInitialMode(int(IM_MAX) - 1);
			break;

		case ED_TD_INITIAL_MODE_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_INITIAL_MODE_DOWN);
			i.mInitialMode =  EInitialMode(int(i.mInitialMode) - 1);
			if (int(i.mInitialMode) < 0)
				i.mInitialMode = EInitialMode(0);
			break;

		case ED_TD_TRIGGER_RESPONSE_UP:
			EDITOR.RegisterLevelChange(ED_TD_TRIGGER_RESPONSE_UP);
			i.mTriggerResponse =  ETriggerResponse(int(i.mTriggerResponse) + 1);
			if (i.mTriggerResponse == TR_MAX)
				i.mTriggerResponse = ETriggerResponse(int(TR_MAX) - 1);
			break;

		case ED_TD_TRIGGER_RESPONSE_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_TRIGGER_RESPONSE_DOWN);
			i.mTriggerResponse =  ETriggerResponse(int(i.mTriggerResponse) - 1);
			if (int(i.mTriggerResponse) < 0)
				i.mTriggerResponse = ETriggerResponse(0);
			break;

		case ED_TD_UNTRIGGER_RESPONSE_UP:
			EDITOR.RegisterLevelChange(ED_TD_UNTRIGGER_RESPONSE_UP);
			i.mUnTriggerResponse =  ETriggerResponse(int(i.mUnTriggerResponse) + 1);
			if (i.mUnTriggerResponse == TR_MAX)
				i.mUnTriggerResponse = ETriggerResponse(int(TR_MAX) - 1);
			break;

		case ED_TD_UNTRIGGER_RESPONSE_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_UNTRIGGER_RESPONSE_DOWN);
			i.mUnTriggerResponse =  ETriggerResponse(int(i.mUnTriggerResponse) - 1);
			if (int(i.mUnTriggerResponse) < 0)
				i.mUnTriggerResponse = ETriggerResponse(0);
			break;

		case ED_TD_COUNT0_UP:
			EDITOR.RegisterLevelChange(ED_TD_COUNT0_UP);
			i.mCounters[0] ++;
			break;

		case ED_TD_COUNT0_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_COUNT0_DOWN);
			i.mCounters[0] --;
			if (i.mCounters[0] < -1) i.mCounters[0] = -1;
			break;

		case ED_TD_COUNT1_UP:
			EDITOR.RegisterLevelChange(ED_TD_COUNT1_UP);
			i.mCounters[1] ++;
			break;

		case ED_TD_COUNT1_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_COUNT1_DOWN);
			i.mCounters[1] --;
			if (i.mCounters[1] < -1) i.mCounters[1] = -1;
			break;

		case ED_TD_COUNT2_UP:
			EDITOR.RegisterLevelChange(ED_TD_COUNT2_UP);
			i.mCounters[2] ++;
			break;

		case ED_TD_COUNT2_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_COUNT2_DOWN);
			i.mCounters[2] --;
			if (i.mCounters[2] < -1) i.mCounters[2] = -1;
			break;

		case ED_TD_COUNT3_UP:
			EDITOR.RegisterLevelChange(ED_TD_COUNT3_UP);
			i.mCounters[3] ++;
			break;

		case ED_TD_COUNT3_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_COUNT3_DOWN);
			i.mCounters[3] --;
			if (i.mCounters[3] < -1) i.mCounters[3] = -1;
			break;

		case ED_TD_BOOL0_UP:
			EDITOR.RegisterLevelChange(ED_TD_BOOL0_UP);
			i.mBools[0] = TRUE;
			break;

		case ED_TD_BOOL0_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_BOOL0_DOWN);
			i.mBools[0] = FALSE;
			break;

		case ED_TD_BOOL1_UP:
			EDITOR.RegisterLevelChange(ED_TD_BOOL1_UP);
			i.mBools[1] = TRUE;
			break;

		case ED_TD_BOOL1_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_BOOL1_DOWN);
			i.mBools[1] = FALSE;
			break;

		case ED_TD_BOOL2_UP:
			EDITOR.RegisterLevelChange(ED_TD_BOOL2_UP);
			i.mBools[2] = TRUE;
			break;

		case ED_TD_BOOL2_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_BOOL2_DOWN);
			i.mBools[2] = FALSE;
			break;

		case ED_TD_BOOL3_UP:
			EDITOR.RegisterLevelChange(ED_TD_BOOL3_UP);
			i.mBools[3] = TRUE;
			break;

		case ED_TD_BOOL3_DOWN:
			EDITOR.RegisterLevelChange(ED_TD_BOOL3_DOWN);
			i.mBools[3] = FALSE;
			break;


		case ED_TD_MESH_SELECT:
			mPaneMeshSelect->Activate();
			mPaneMeshSelect->GetListText()->mCurrentEntry = -1;
			recreate = FALSE;
			break;

		default:
			recreate = FALSE;
			break;
		};

		if (recreate)
		{
			// clip stuff
			while (i.mYaw >  G_PI)
				i.mYaw -= G_PI * 2;
			while (i.mYaw < -G_PI)
				i.mYaw += G_PI * 2;
			while (i.mPitch >  G_PI)
				i.mPitch -= G_PI * 2;
			while (i.mPitch < -G_PI)
				i.mPitch += G_PI * 2;
			while (i.mRoll >  G_PI)
				i.mRoll -= G_PI * 2;
			while (i.mRoll < -G_PI)
				i.mRoll += G_PI * 2;

			s->Shutdown();
			s->Init(&i);

			DeselectButton();
			UpdateState();
		}
	}
#endif	
}

//******************************************************************************************
void	CPaneThingDetail::ProcessAction2(EEdAction action)
{
#if TARGET != DC
	CThing		*s = EDITOR.GetSelectedThing();

	if (s)
	{
		CInitThing	i;
		s->WriteData(i);

		BOOL	recreate = FALSE;

		switch(action)
		{
		case	ED_TD_TARGET_THING:
			i.mTarget = -1;
			recreate = TRUE;
			break;
		case	ED_TD_CHAIN_TRIGGER:
			i.mChainTrigger = -1;
			recreate = TRUE;
			break;
		case	ED_TD_OID:
			i.mOID = OID_NONE;
			recreate = TRUE;
			break;
		};

		if (recreate)
		{
			s->Shutdown();
			s->Init(&i);

			DeselectButton();
			UpdateState();
		}
	}
#endif	
}

//******************************************************************************************
CThing	*GetNearestThing(CCellRef &c)
{
#if TARGET != DC
	CThing *t = MAP.GetFirstThing(c.ToGVector(), GINT_HALF);
	GINT	min_dist = GINT_MAX;
	CThing	*min_thing = NULL;
	GVector p = GVector(G(c.X, 32768), G(c.Y, 32768), G0);
	p.Z = MAP.GetMapHeight(p);

	while (t)
	{
		if (t->IsPersistent())
		{
			if ((p - t->GetPos()).Magnitude() < min_dist)
			{
				min_dist = (p - t->GetPos()).Magnitude();
				min_thing = t;
			}
		}
		t = MAP.GetNextThing();
	};
#elif TARGET == DC
	CThing	*min_thing = NULL;
#endif	
	return min_thing;
}
//**********

void	CPaneThingDetail::ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2)
{
#if TARGET != DC
	CCellRef c = PLAYER(0)->GetCursor();

	if (c.X != -1)
	{
		CThing *s = EDITOR.GetSelectedThing();
		if (IsButtonSelected(ED_TD_TARGET_THING) && b1 && s)
		{
			CThing *t = GetNearestThing(c);

			EDITOR.RegisterLevelChange(ED_TD_TARGET_THING);

			CInitThing i;
			s->WriteData(i);

			if (t)
				i.mTarget = t->GetThingNumber();
			else
				i.mTarget = -1;

			s->Shutdown();
			s->Init(&i);

			DeselectButton();

			UpdateState();
		}
		else	if (IsButtonSelected(ED_TD_CHAIN_TRIGGER) && b1 && s)
		{
			CThing *t = GetNearestThing(c);

			EDITOR.RegisterLevelChange(ED_TD_CHAIN_TRIGGER);

			CInitThing i;
			s->WriteData(i);

			if (t)
				i.mChainTrigger = t->GetThingNumber();
			else
				i.mChainTrigger = -1;

			s->Shutdown();
			s->Init(&i);

			EDITOR.RegisterSelectedThing(s);

			DeselectButton();

			UpdateState();
		}
		else if (b2)
		{
			if (CONTROLS.IsButtonPressed(BUTTON_SHIFT))
			{
				// move thing
				if (c.X != -1)
				{
					CThing *s = EDITOR.GetSelectedThing();
					if (s)
					{
						EDITOR.RegisterLevelChange(ED_THING_EDIT);

						CInitThing i;
						s->WriteData(i);
						i.mPos = GVector(G(c.X) + GINT_HALF, G(c.Y) + GINT_HALF, G0);
						i.mPos.Z = MAP.GetMapHeight(i.mPos);

						s->Shutdown();
						s->Init(&i);
					}
				}
			}
			else
			{
				// Select thing.
				CThing *t = GetNearestThing(c);

				EDITOR.RegisterSelectedThing(t);
				UpdateState();
				DeselectButton();
			}
		}
		else
			mParent->ProcessOtherActions(x, y, b1, b2);
	}
#endif	
}

//******************************************************************************************
void	CPaneThingDetail::UpdateState()
{
#if TARGET != DC
	if (EDITOR.GetSelectedThing())
	{
		CInitThing	i;
		EDITOR.GetSelectedThing()->WriteData(i);

		if (i.mTarget != -2)
			 ActivateButton(ED_TD_TARGET_THING);
		else DeActivateButton(ED_TD_TARGET_THING);
		
		if (i.mOID != OID_INVALID)
			 ActivateButton(ED_TD_OID);
		else DeActivateButton(ED_TD_OID);

		if (i.mChainTrigger != -2)
			 ActivateButton(ED_TD_CHAIN_TRIGGER);
		else DeActivateButton(ED_TD_CHAIN_TRIGGER);

		if (i.mMeshNo != -2)
			 ActivateButton(ED_TD_MESH_SELECT);
		else DeActivateButton(ED_TD_MESH_SELECT);

	}
	else
	{
		DeActivateButton(ED_TD_TARGET_THING);
		DeActivateButton(ED_TD_OID);
		DeActivateButton(ED_TD_CHAIN_TRIGGER);
		DeActivateButton(ED_TD_MESH_SELECT);
		DeselectButton();
	}
	
	Redraw();
#endif	
}
	
//******************************************************************************************
BOOL	CPaneThingDetail::ChainOID(SINT new_oid)
{
#if TARGET != DC
	// handle selection of an OID from the ThingEd Pane

	if (IsButtonSelected(ED_TD_OID))
	{
		CThing *s = EDITOR.GetSelectedThing();

		ASSERT(s);

		if (s)
		{

			EDITOR.RegisterLevelChange(ED_TD_OID);

			CInitThing i;
			s->WriteData(i);
			i.mOID = new_oid;

			s->Shutdown();
			s->Init(&i);

			DeselectButton();
			UpdateState();
		}

		return TRUE;
	}
	else
	
#endif	
		return FALSE;
}

//******************************************************************************************
void	CPaneThingDetail::Redraw()
{
#if TARGET != DC
	CPane::Redraw();

	mCachedThing = EDITOR.GetSelectedThing();  // update state

	CThing *st = EDITOR.GetSelectedThing();
	CThing *t;

	char	text[200];

	if (st)
		strcpy(text, st->_GetClassName() + 1);
	else
		strcpy(text, "Nothing Selected");
		
	DrawFontText(5, 7, text, 255, 200, 30);

	if (!st)
		return;

	CInitThing	i;
	st->WriteData(i);

#define NAME_COLOUR  200, 130, 130
#define NONAME_COLOUR  130, 130, 130
#define ITEM_COLOUR  150, 250, 100
#define ITEM_COLOUR2 150, 100, 250

	SINT	cy = 40;
	
	DrawFontText(5, cy, "Position:", NAME_COLOUR);
	cy += 17;
	sprintf(text, "%02.1f, %02.1f, %02.1f", GToF(i.mPos.X), GToF(i.mPos.Y), GToF(i.mPos.Z));
	DrawFontText(5, cy, text, 150, 250, 100);
	cy += 23;

	DrawFontText(130, 50, "X", NAME_COLOUR);
	DrawFontText(130, 70, "Y", NAME_COLOUR);
	DrawFontText(130, 90, "Z", NAME_COLOUR);

	DrawFontText(5, cy, "Velocity:", NAME_COLOUR);
	cy += 17;
	sprintf(text, "%02.1f, %02.1f, %02.1f", GToF(i.mVelocity.X), GToF(i.mVelocity.Y), GToF(i.mVelocity.Z));
	DrawFontText(5, cy, text, 150, 250, 100);
	cy += 23;

	// ok - get yaw, pitch & roll back out of the matrix!  rather dodgy...

	SINT	iyaw   = ((i.mYaw   * 180) / G_PI).Whole();
	SINT	ipitch = ((i.mPitch * 180) / G_PI).Whole();
	SINT	iroll  = ((i.mRoll  * 180) / G_PI).Whole();

	DrawFontText(5, cy, "Yaw:", NAME_COLOUR);
	sprintf(text, "%02d", iyaw);
	DrawFontText(95, cy, text, ITEM_COLOUR2);
	cy += 20;

	DrawFontText(5, cy, "Pitch:", NAME_COLOUR);
	sprintf(text, "%02d", ipitch);
	DrawFontText(95, cy, text, ITEM_COLOUR2);
	cy += 20;

	DrawFontText(5, cy, "Roll:", NAME_COLOUR);
	sprintf(text, "%02d", iroll);
	DrawFontText(95, cy, text, ITEM_COLOUR2);
	cy += 20;

	DrawFontText(5, cy, "Target Thing:", NAME_COLOUR);
	cy += 17;
	if (i.mTarget == -2)
	{
		strcpy(text, "** N/A **");
	}
	else
	{
		WORLD.GetThing(i.mTarget, &t);
		if (t)
			strcpy(text, t->_GetClassName() + 1);
		else
			strcpy(text, "No Thing selected");
	}
	DrawFontText(5, cy, text, 150, 250, 100);
	cy += 23;

	DrawFontText(5, cy, "Object ID:", NAME_COLOUR);
	cy += 17;
	// hmm - have to spawn a thing to get its name
	if (i.mOID == OID_INVALID)
	{
		strcpy(text, "** N/A **");
	}
	else
	{
		t = SpawnThing(i.mOID, FALSE);
		if (t)
		{
			strcpy(text, t->_GetClassName() + 1);
			delete t;
		}
		else
			strcpy(text, "No OID selected");
	}
	DrawFontText(5, cy, text, 150, 250, 100);
	cy += 23;

	DrawFontText(5, cy, "Chain Trigger Thing:", NAME_COLOUR);
	cy += 17;
	if (i.mChainTrigger == -2)
	{
		strcpy(text, "** N/A **");
	}
	else
	{
		WORLD.GetThing(i.mChainTrigger, &t);
		if (t)
			strcpy(text, t->_GetClassName() + 1);
		else
			strcpy(text, "No Thing selected");
	}
	DrawFontText(5, cy, text, 150, 250, 100);
	cy += 23;

	//****************************************
	// Initial Mode
	if (i.mInitialMode == IM_UNUSED)
		    DrawFontText(5, 320, "Initial Mode", NONAME_COLOUR);
	else	DrawFontText(5, 320, "Initial Mode", NAME_COLOUR);

	switch (i.mInitialMode)
	{
	case	IM_UNUSED:		strcpy(text, "n/a");		break;
	case	IM_ACTIVE:		strcpy(text, "Active");		break;
	case	IM_INACTIVE:	strcpy(text, "InActive");	break;
	};
	
	DrawFontText(107, 320, text, ITEM_COLOUR);

	//****************************************
	// Trigger
	if (i.mTriggerResponse == IM_UNUSED)
			DrawFontText(5, 350, "Trigger", NONAME_COLOUR);
	else	DrawFontText(5, 350, "Trigger", NAME_COLOUR);

	switch (i.mTriggerResponse)
	{
	case	TR_UNUSED:		strcpy(text, "n/a");		break;
	case	TR_NONE:		strcpy(text, "None");		break;
	case	TR_ACTIVATE:	strcpy(text, "Activate");	break;
	case	TR_DEACTIVATE:	strcpy(text, "DeActivate");	break;
	case	TR_TOGGLE:		strcpy(text, "Toggle");		break;
	}
	DrawFontText(107, 350, text, ITEM_COLOUR);

	//****************************************
	// UnTrigger
	if (i.mUnTriggerResponse == IM_UNUSED)
			DrawFontText(5, 370, "UnTrigger", NONAME_COLOUR);
	else	DrawFontText(5, 370, "UnTrigger", NAME_COLOUR);

	switch (i.mUnTriggerResponse)
	{
	case	TR_UNUSED:		strcpy(text, "n/a");		break;
	case	TR_NONE:		strcpy(text, "None");		break;
	case	TR_ACTIVATE:	strcpy(text, "Activate");	break;
	case	TR_DEACTIVATE:	strcpy(text, "DeActivate");	break;
	case	TR_TOGGLE:		strcpy(text, "Toggle");		break;
	}
	DrawFontText(107, 370, text, ITEM_COLOUR);

	//****************************************
	// Counters
	if (st->GetTEDCount0() == NULL)
		DrawFontText(5, 400, "Unused"         , NONAME_COLOUR);
	else
	{
		DrawFontText(5, 400, st->GetTEDCount0(), NAME_COLOUR  );

		if (i.mCounters[0] == -1)
			strcpy(text, "Infinite");
		else
			sprintf(text, "%d", i.mCounters[0]);

		DrawFontText(107, 400, text, ITEM_COLOUR2);
	}

	if (st->GetTEDCount1() == NULL)
		DrawFontText(5, 420, "Unused"         , NONAME_COLOUR);
	else
	{
		DrawFontText(5, 420, st->GetTEDCount1(), NAME_COLOUR  );

		if (i.mCounters[1] == -1)
			strcpy(text, "Infinite");
		else
			sprintf(text, "%d", i.mCounters[1]);

		DrawFontText(107, 420, text, ITEM_COLOUR2);
	}

	if (st->GetTEDCount2() == NULL)
		DrawFontText(5, 440, "Unused"         , NONAME_COLOUR);
	else
	{
		DrawFontText(5, 440, st->GetTEDCount2(), NAME_COLOUR  );

		if (i.mCounters[2] == -1)
			strcpy(text, "Infinite");
		else
			sprintf(text, "%d", i.mCounters[2]);

		DrawFontText(107, 440, text, ITEM_COLOUR2);
	}

	if (st->GetTEDCount3() == NULL)
		DrawFontText(5, 460, "Unused"         , NONAME_COLOUR);
	else
	{
		DrawFontText(5, 460, st->GetTEDCount3(), NAME_COLOUR  );

		if (i.mCounters[3] == -1)
			strcpy(text, "Infinite");
		else
			sprintf(text, "%d", i.mCounters[3]);

		DrawFontText(107, 460, text, ITEM_COLOUR2);
	}

	//****************************************
	// Bools
	if (st->GetTEDBOOL0() == NULL)
		DrawFontText(5, 490, "Unused"         , NONAME_COLOUR);
	else
	{
		DrawFontText(5, 490, st->GetTEDBOOL0(), NAME_COLOUR  );

		if (i.mBools[0] == TRUE)
			strcpy(text, "True");
		else
			strcpy(text, "False");

		DrawFontText(107, 490, text, ITEM_COLOUR2);
	}

	if (st->GetTEDBOOL1() == NULL)
		DrawFontText(5, 510, "Unused"         , NONAME_COLOUR);
	else
	{
		DrawFontText(5, 510, st->GetTEDBOOL1(), NAME_COLOUR  );

		if (i.mBools[1] == TRUE)
			strcpy(text, "True");
		else
			strcpy(text, "False");

		DrawFontText(107, 510, text, ITEM_COLOUR2);
	}

	if (st->GetTEDBOOL2() == NULL)
		DrawFontText(5, 530, "Unused"         , NONAME_COLOUR);
	else
	{
		DrawFontText(5, 530, st->GetTEDBOOL2(), NAME_COLOUR  );

		if (i.mBools[2] == TRUE)
			strcpy(text, "True");
		else
			strcpy(text, "False");

		DrawFontText(107, 530, text, ITEM_COLOUR2);
	}

	if (st->GetTEDBOOL3() == NULL)
		DrawFontText(5, 550, "Unused"         , NONAME_COLOUR);
	else
	{
		DrawFontText(5, 550, st->GetTEDBOOL3(), NAME_COLOUR  );

		if (i.mBools[3] == TRUE)
			strcpy(text, "True");
		else
			strcpy(text, "False");

		DrawFontText(107, 550, text, ITEM_COLOUR2);
	}

	//******************************************************************************************
	DrawFontText(5, 580, "Mesh Name:", NAME_COLOUR);

	// hmm - have to spawn a thing to get its name
	if (i.mMeshNo == -2)
	{
		strcpy(text, "** N/A **");
	}
	else if (i.mMeshNo == -1)
	{
		strcpy(text, "None selected");
	}
	else
	{
		CMesh	*m = ENGINE.GetGlobalMesh(i.mMeshNo);
		strcpy(text, m->mName);
	}
	DrawFontText(5, 597, text, 150, 250, 100);
#endif	
}

//******************************************************************************************
BOOL	CPaneThingDetail::Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT mw)
{
#if TARGET!=DC
	// have we selected a mesh?
	if (mPaneMeshSelect->IsActive())
	{
		SINT mn = mPaneMeshSelect->GetListText()->mCurrentEntry;

		if (mn != -1)
		{
			mPaneMeshSelect->DeActivate();

			CInitThing i;
			CThing *s = EDITOR.GetSelectedThing();

			if (s)
			{
				s->WriteData(i);

				if (i.mMeshNo != -2)
				{
					i.mMeshNo = ENGINE.GetGlobalMeshNumber(mPaneMeshSelect->GetListText()->mEntries[mn].mText);

					s->Shutdown();
					s->Init(&i);

					DeselectButton();

					UpdateState();
				}
			}
		}
	}
	// (done before processing the pane)

	BOOL	ret = CPane::Process(x, y, b1, b2, mw);

	if (mCachedThing != EDITOR.GetSelectedThing())
		UpdateState();

	if (!(EDITOR.GetSelectedThing()))
		DeActivate();
#elif TARGET == DC
BOOL ret;		
#endif
	return ret;
}

//******************************************************************************************
BOOL	CPaneThingDetail::ProcessKeys()
{
#if TARGET != DC
	CPane::ProcessKeys();

	if (CONTROLS.IsButtonPressedDB(BUTTON_CHANGE_WEAPON))
	{
		CThing *t = EDITOR.GetSelectedThing();

		if (t)
		{
			EDITOR.RegisterLevelChange(ED_THING_SELECT);

			EDITOR.RegisterSelectedThing(NULL);
			t->SuicideNext();			// register thing as invalid					
			WORLD.ValidateAllData();	// and scan through, deleteing references
			WORLD.DestroyThing(t);		// whatever...
		}
	}
#endif
	return FALSE;
}

//******************************************************************************************
//** CPaneThingEd
//******************************************************************************************
void	CPaneThingEd::Init()
{
#if TARGET != DC
	CPane::Init();

	// set up list box
	CListText	*l = CreateAndGetListText();

	l->Resize(10, 10, 164, 313);

	SINT	id = 0;
	while (id < OID_EOL)
	{
		//! hmm.....
		if (CanThisOIDBeInstantiated(id))
		{
			CThing *t = SpawnThing(id, FALSE);  // don't care if they don't exist...
			if (t)
			{
				if (t->IsPersistent())
				{
					l->AddEntry(t->_GetClassName() + 1, id, 0);  // + 1 loses the 'C'
				}
				else
				{
					l->AddEntry(t->_GetClassName() + 1, id, 1);  // + 1 loses the 'C'
				}

				delete t;
			}
		}
		id ++;
	}

	mPaneThingDetail = new CPaneThingDetail(170, 700, 465, 5, EPS_RIGHT, SRCCOPY, FALSE, 395);
	mPaneThingDetail->Init();
	DeActivateThingWindow();
	mPaneThingDetail->mParent = this;

	Redraw();
#endif	
}

//******************************************************************************************
void	CPaneThingEd::Activate()   
{
#if TARGET != DC
	CPane::Activate();
	EDITOR.RegisterActionPane(this);
	if (EDITOR.GetSelectedThing())
		ActivateThingWindow();
#endif		
}


//******************************************************************************************
void	CPaneThingEd::DeActivate()
{
#if TARGET != DC
	CPane::DeActivate(); 
	EDITOR.ReleaseActionPane(this);
	DeActivateThingWindow();
#endif	
}

//******************************************************************************************
void	CPaneThingEd::ProcessAction(EEdAction action)
{
/*witch(action)
	{
	case ED_THING_SELECT: 

		ActivateThingWindow();
		
		mListText->Deselect();
		break;
	};*/
}

//******************************************************************************************
BOOL	CPaneThingEd::Process(SINT x, SINT y, BOOL b1, BOOL b2, SINT mw)
{
#if TARGET != DC
	BOOL	ret = CPane::Process(x, y, b1, b2, mw);

	if (mListText->mCurrentEntry != -1)
	{
		// do we need to chain this to the Detail window?
		if (mPaneThingDetail->ChainOID(mListText->mEntries[mListText->mCurrentEntry].mData))
		{
			mListText->mCurrentEntry = -1;
		}
	}
#elif TARGET == DC
 BOOL ret;	
#endif
	return ret;
}

//******************************************************************************************
void	CPaneThingEd::ActivateThingWindow()
{
#if TARGET != DC
	mPaneThingDetail->Activate();
#endif	
}

//******************************************************************************************
void	CPaneThingEd::DeActivateThingWindow()
{
#if TARGET != DC
	mPaneThingDetail->DeActivate();
#endif	
}

//******************************************************************************************
void	CPaneThingEd::ProcessOtherActions(SINT x, SINT y, BOOL b1, BOOL b2)
{
#if TARGET != DC
	CCellRef c = PLAYER(0)->GetCursor();

	if (mListText->mCurrentEntry != -1)
	{
		if (b1)
		{

			if (c.X != -1)
			{
				// add thing here

				// is there already something of this type here...
//				if (MAP.GetCell(c)->GetType() == CT_NORMAL)
				{
					CThing *m = MAP.GetFirstThing(c.ToGVector(), GINT_HALF);
					BOOL	v = TRUE;

					while (m)
					{
						if ((m->GetPos() - c.ToGVector() - GVECTOR_HALF).Magnitude() < G(1))
							if (m->_GetClassID() == mListText->mEntries[mListText->mCurrentEntry].mData)
							{
								v = FALSE;
								break;
							}
						m = MAP.GetNextThing();
					}

					if (v)
					{
						EDITOR.RegisterLevelChange(ED_THING_EDIT);
						CThing *t = SpawnThing(mListText->mEntries[mListText->mCurrentEntry].mData);
						if (t)
						{
							CInitThing i;
							i.mPos = GVector(G(c.X) + GINT_HALF, G(c.Y) + GINT_HALF, G0);
							i.mPos.Z = MAP.GetMapHeight(i.mPos);
							// raise lights initially
//							if (t->IsLight())
//								i.mPos.Z -= G(5);
							t->Init(&i);
						};
					}
				}
			}
		}

		if (b2)
		{
			// Select thing.
			CThing *t = GetNearestThing(c);

			EDITOR.RegisterSelectedThing(t);

			if (t)
				ActivateThingWindow();
			else
				DeActivateThingWindow();
		}
	}
#endif	
}	
	
//******************************************************************************************
//** CPaneVarEd
//******************************************************************************************
void	CPaneVarEd::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("scroll16.tga");
	ASSERT(icons->IsValid());


	AddIconButton(10, 40, icons, 5, ED_DECREASE_NO_PLAYERS	, "Decrease num players", FALSE);
	AddIconButton(75, 40, icons, 4, ED_INCREASE_NO_PLAYERS	, "Increase num players", FALSE);

	AddIconButton(10,  80, icons, 5, ED_VAR_WATER_DOWN	, "Lower Water"				, FALSE);
	AddIconButton(75,  80, icons, 4, ED_VAR_WATER_UP	, "Raise Water"				, FALSE);
	AddIconButton(10, 120, icons, 5, ED_LEVEL_TYPE_DOWN	, "Previous Level Type"		, FALSE);
	AddIconButton(75, 120, icons, 4, ED_LEVEL_TYPE_UP	, "Next Level Type"			, FALSE);
	AddIconButton(10, 160, icons, 5, ED_TIME_LIMIT_DOWN	, "Lower Time Limit"		, FALSE, TRUE);
	AddIconButton(75, 160, icons, 4, ED_TIME_LIMIT_UP	, "Raise Time Limit"		, FALSE, TRUE);

	AddIconButton(10, 200, icons, 5, ED_DECREASE_NO_LAPS	, "Decrease no laps for race", FALSE);
	AddIconButton(75, 200, icons, 4, ED_INCREASE_NO_LAPS	, "Increase no laps for race", FALSE);

	AddIconButton(10, 240, icons, 5, ED_DECREASE_NO_CHECKPOINTS	, "Decrease total number of checkpoints", FALSE);
	AddIconButton(75, 240, icons, 4, ED_INCREASE_NO_CHECKPOINTS	, "Increase total number of checkpoints", FALSE);

	AddIconButton(10, 280, icons, 5, ED_DECREASE_NO_POINTS_REQ_LMR	, "Decrease points required to complete last man rolling", FALSE);
	AddIconButton(75, 280, icons, 4, ED_INCREASE_NO_POINTS_REQ_LMR	, "Increase points required to complete last man rolling", FALSE);

	AddIconButton(10, 320, icons, 5, ED_DECREASE_PRISON_NO, "Decrease Prison Facitily Number", FALSE);
	AddIconButton(75, 320, icons, 4, ED_INCREASE_PRISON_NO, "Increase Prison Facitily Number", FALSE);




	delete icons;

	DeselectButton();
#endif	
}

void	CPaneVarEd::Redraw()
{
#if TARGET != DC
	CPane::Redraw();

	char	text[200];

	SINT	cy = 5;
	
	DrawFontText(5, cy, "Level Variables", 255, 150, 100);
	cy += 15;

	DrawFontText(10, cy, "Num Players", NAME_COLOUR);
	cy += 20;
	sprintf(text, "%d", LD->mIVars[IV_NUMBER_PLAYERS]);
	DrawFontText(40, cy, text, 150, 250, 100);
	cy += 20;

	DrawFontText(10, cy, "Water Height", NAME_COLOUR);
	cy += 20;
	sprintf(text, "%02.1f", GToF(LD->mGVars[WATER_HEIGHT]));
	DrawFontText(40, cy, text, 150, 250, 100);
	cy += 20;

	DrawFontText(10, cy, "Level Type", NAME_COLOUR);
	cy += 20;
	switch(LD->mIVars[IV_LEVEL_TYPE])
	{
	case LT_NORMAL:		strcpy(text, "Beam");					break;
	case LT_GOLF:		strcpy(text, "Golf");					break;
	case LT_SHUTTLE:	strcpy(text, "Shuttle");				break;
	case LT_RACER:		strcpy(text, "Racer");					break;
	case LT_DIRECT:		strcpy(text, "Direct");					break;
	case LT_LASTMAN:	strcpy(text, "Last Man Rolling");		break;
	case LT_GAUNTLET:	strcpy(text, "Run The Gauntlet");		break;
	case LT_PURSUIT:	strcpy(text, "'Pursuit'");				break;
	case LT_TAG:		strcpy(text, "Trick 'n Tag");			break;
	case LT_KING:		strcpy(text, "King of the Hill");		break;
	case LT_DROID_BALL:	strcpy(text, "Droid Ball");		break;
	default:			sprintf(text, "%d", LD->mIVars[IV_LEVEL_TYPE]); break;
	};
	DrawFontText(30, cy, text, 150, 250, 100);
	cy += 20;

	DrawFontText(10, cy, "Time Limit", NAME_COLOUR);
	cy += 20;
	if (LD->mIVars[IV_TIME_LIMIT] == 0)
		strcpy(text, "None");
	else
		sprintf(text, "%02d:%02d", LD->mIVars[IV_TIME_LIMIT] / 60 , LD->mIVars[IV_TIME_LIMIT] % 60);
	DrawFontText(34, cy, text, 150, 250, 100);
	cy += 20;


//	if (LD->mIVars[IV_LEVEL_TYPE] == LT_RACER)
//	{
		DrawFontText(10, cy, "No. Laps", NAME_COLOUR);
		cy += 20;
		sprintf(text, "%d", LD->mIVars[IV_NUM_LAPS_FOR_RACE]);
		DrawFontText(40, cy, text, 150, 250, 100);
		cy += 20;

		DrawFontText(10, cy, "No. Checkpoints", NAME_COLOUR);
		cy += 20;
		sprintf(text, "%d", LD->mIVars[IV_NUM_CHECKPOINTS]);
		DrawFontText(40, cy, text, 150, 250, 100);
		cy += 20;
//	}

		DrawFontText(10, cy, "Points req", NAME_COLOUR);
		cy += 20;
		sprintf(text, "%d", LD->mIVars[IV_POINTS_REQUIRED_TO_WIN_LEVEL]);
		DrawFontText(40, cy, text, 150, 250, 100);
		cy += 20;

		DrawFontText(10, cy, "Prison No", NAME_COLOUR);
		cy += 20;
		sprintf(text, "%d", LD->mIVars[IV_PRISON_NUMBER]);
		DrawFontText(40, cy, text, 150, 250, 100);
		cy += 20;
#endif		

}

void	CPaneVarEd::ProcessAction(EEdAction action)
{
#if TARGET != DC
	switch(action)
	{
	case ED_VAR_WATER_DOWN:
		EDITOR.RegisterLevelChange(ED_VAR_WATER_DOWN);
		LD->mGVars[WATER_HEIGHT] += FToG(0.1f);
		break;

	case ED_VAR_WATER_UP:
		EDITOR.RegisterLevelChange(ED_VAR_WATER_UP);
		LD->mGVars[WATER_HEIGHT] -= FToG(0.1f);
		break;

	case ED_LEVEL_TYPE_DOWN:
		if (LD->mIVars[IV_LEVEL_TYPE] > 0)
		{
			EDITOR.RegisterLevelChange(ED_LEVEL_TYPE_DOWN);
			LD->mIVars[IV_LEVEL_TYPE] --;
		}
		break;

	case ED_LEVEL_TYPE_UP:
		if (LD->mIVars[IV_LEVEL_TYPE] < LT_MAX - 1)
		{
			EDITOR.RegisterLevelChange(ED_LEVEL_TYPE_UP);
			LD->mIVars[IV_LEVEL_TYPE] ++;
		}
		break;


	case ED_DECREASE_NO_PLAYERS:
		if (LD->mIVars[IV_NUMBER_PLAYERS] > 1)
		{
			LD->mIVars[IV_NUMBER_PLAYERS] --;
		}
		break;

	case ED_INCREASE_NO_PLAYERS:
		if (LD->mIVars[IV_NUMBER_PLAYERS] < 2)
		{
			LD->mIVars[IV_NUMBER_PLAYERS] ++;
		}
		break;

	case ED_DECREASE_NO_LAPS:
		if (LD->mIVars[IV_NUM_LAPS_FOR_RACE] > 0)
		{
			LD->mIVars[IV_NUM_LAPS_FOR_RACE] --;
		}
		break;

	case ED_INCREASE_NO_LAPS:
		if (LD->mIVars[IV_NUM_LAPS_FOR_RACE] < NO_LAPS_MAX - 1)
		{
			LD->mIVars[IV_NUM_LAPS_FOR_RACE] ++;
		}
		break;

	case ED_DECREASE_NO_CHECKPOINTS:
		if (LD->mIVars[IV_NUM_CHECKPOINTS] > 0)
		{
			LD->mIVars[IV_NUM_CHECKPOINTS] --;
		}
		break;

//#pragma message ("***********  STUART : fix this!  ***********\n")
//#define NO_CHECKPOINTS_MAX 20

	case ED_INCREASE_NO_CHECKPOINTS:
		if (LD->mIVars[IV_NUM_CHECKPOINTS] < NO_CHECKPOINTS_MAX - 1)
		{
			LD->mIVars[IV_NUM_CHECKPOINTS] ++;
		}
		break;


	case ED_DECREASE_NO_POINTS_REQ_LMR:
		if (LD->mIVars[IV_POINTS_REQUIRED_TO_WIN_LEVEL] > 0)
		{
			LD->mIVars[IV_POINTS_REQUIRED_TO_WIN_LEVEL] -=100;
		}
		break;

	case ED_INCREASE_NO_POINTS_REQ_LMR:
		if (LD->mIVars[IV_POINTS_REQUIRED_TO_WIN_LEVEL] < 100000)
		{
			LD->mIVars[IV_POINTS_REQUIRED_TO_WIN_LEVEL]+=100;
		}
		break;

	case ED_DECREASE_PRISON_NO:
		if (LD->mIVars[IV_PRISON_NUMBER] > 0)
		{
			LD->mIVars[IV_PRISON_NUMBER] -= 1;
		}
		break;

	case ED_INCREASE_PRISON_NO:
		if (LD->mIVars[IV_PRISON_NUMBER] < NUM_PRISONS - 1)
		{
			LD->mIVars[IV_PRISON_NUMBER] += 1;
		}
		break;

	case ED_TIME_LIMIT_UP:
		if (LD->mIVars[IV_TIME_LIMIT] < TIME_LIMIT_MAX - 15)
		{
			EDITOR.RegisterLevelChange(ED_TIME_LIMIT_UP);
			LD->mIVars[IV_TIME_LIMIT] += 15;
		}
		break;

	case ED_TIME_LIMIT_DOWN:
		if (LD->mIVars[IV_TIME_LIMIT] > 0)
		{
			EDITOR.RegisterLevelChange(ED_TIME_LIMIT_DOWN);
			LD->mIVars[IV_TIME_LIMIT] -= 15;
			if (LD->mIVars[IV_TIME_LIMIT] < 0)
				LD->mIVars[IV_TIME_LIMIT] = 0;
		}
		break;
	};
	// clip to 0
	if (GABS(LD->mGVars[WATER_HEIGHT]) < G(0, 500))
		LD->mGVars[WATER_HEIGHT] = G0;

	Redraw();
#endif	
}

//******************************************************************************************
//** CPaneMain
//******************************************************************************************
void	CPaneMain::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("icons1.tga");
	ASSERT(icons->IsValid());

	AddIconButton( 10, 8, icons, 0, ED_LANDSCAPE_EDIT		, "Landscape Editor");
	AddIconButton( 50, 8, icons, 1, ED_TILE_EDIT			, "Tile Editor");
	AddIconButton( 90, 8, icons, 2, ED_TRIGGER_EDIT			, "Trigger Editor");
	AddIconButton(130, 8, icons, 3, ED_THING_EDIT			, "Thing Placement Editor");
	AddIconButton(170, 8, icons, 4, ED_VAR_EDIT				, "Level Variables Editor");

	AddIconButton(210, 8, icons, 5, ED_EXPORT_THIS			, "Export this level to PSX");
	AddIconButton(250, 8, icons, 6, ED_EXPORT_ALL			, "Export all levels to PSX");
	AddIconButton(290, 8, icons, 7, ED_EXPORT_FE			, "Export front end data to PSX");
	AddIconButton(330, 8, icons, 8, ED_EXPORTDC_THIS			, "Export this level to DC");
	AddIconButton(370, 8, icons, 9, ED_EXPORTDC_ALL			, "Export all levels to DC");
	AddIconButton(410, 8, icons,10, ED_EXPORTDC_FE			, "Export front end data to DC");

//	DeActivateButton(ED_TILE_EDIT);
//	DeActivateButton(ED_VAR_EDIT);

	delete icons;

	// load dependant panes
	mPaneLandscapeEd = NULL;
	mPaneTileEd = NULL;
	mPaneTriggerEd = NULL;
	mPaneThingEd = NULL;
	mPaneVarEd = NULL;

	mPaneLandscapeEd = new CPaneLandscapeEd(88, 343, 5, 57, EPS_TOP);
	mPaneLandscapeEd->Init();
	mPaneLandscapeEd->Activate();

	mPaneTileEd = new CPaneTileEd(104, 343, 5, 57, EPS_LEFT);
	mPaneTileEd->Init();

	mPaneTriggerEd = new CPaneTriggerEd(50, 343, 5, 57, EPS_TOP);
	mPaneTriggerEd->Init();

	mPaneThingEd = new CPaneThingEd(180, 343, 5, 57, EPS_BOTTOM);
	mPaneThingEd->Init();

	mPaneVarEd = new CPaneVarEd(100, 800, 5, 57, EPS_LEFT, SRCCOPY, FALSE, 343);
	mPaneVarEd->Init();
#endif	
}

//******************************************************************************************
void	CPaneMain::ProcessAction(EEdAction action)
{
#if TARGET != DC
	if (action == ED_EXPORT_THIS)
	{
		// Save this level as PSX
		Redraw();

		GAME.SavePSXLevel();

		ActivateButton(ED_LANDSCAPE_EDIT);
//		mPaneLandscapeEd->Init();
	}
	else	if (action == ED_EXPORT_ALL)
	{
		// load and save all levels!
		Redraw();

		GAME.SavePSXFrontEndData();

		SINT	c0;

		for (c0 = 0; c0 < 1000; c0 ++)
		{
			EDITOR.ClearSelections();
			BOOL res = GAME.LoadLevel(c0);
			if (res)
				GAME.SavePSXLevel(c0);

			CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!
		}

		ActivateButton(ED_LANDSCAPE_EDIT);
//		mPaneLandscapeEd->Init();
	}
	else	if (action == ED_EXPORT_FE)
	{
		GAME.SavePSXFrontEndData();
	}
	if (action == ED_EXPORTDC_THIS)
	{
		// Save this level as PSX
		Redraw();

 		GAME.SaveDCLevel();

		ActivateButton(ED_LANDSCAPE_EDIT);
//		mPaneLandscapeEd->Init();
	}
	else	if (action == ED_EXPORTDC_ALL)
	{
		// load and save all levels!
		Redraw();

		GAME.SaveDCFrontEndData();

		SINT	c0;

		for (c0 = 0; c0 < 1000; c0 ++)
		{
			EDITOR.ClearSelections();
			BOOL res = GAME.LoadLevel(c0);
			if (res)
				GAME.SaveDCLevel(c0);

			CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!
		}

		ActivateButton(ED_LANDSCAPE_EDIT);
//		mPaneLandscapeEd->Init();
	}
	else	if (action == ED_EXPORTDC_FE)
	{
		GAME.SaveDCFrontEndData();
	}
	else
	{
		// disable all
		mPaneLandscapeEd->DeActivate();
		mPaneTileEd->DeActivate();
		mPaneTriggerEd->DeActivate();
		mPaneThingEd->DeActivate();
		mPaneVarEd->DeActivate();

		switch(action)
		{
		case ED_LANDSCAPE_EDIT: mPaneLandscapeEd->Activate();		break;
		case ED_TILE_EDIT:		mPaneTileEd->Activate();		break;
		case ED_TRIGGER_EDIT:	mPaneTriggerEd->Activate();		break;
		case ED_THING_EDIT:		mPaneThingEd->Activate();		break;
		case ED_VAR_EDIT:		mPaneVarEd->Activate();		break;
		}
	}
#endif	
}

//******************************************************************************************
//** CPaneDisk
//******************************************************************************************
void	CPaneDisk::Init()
{
#if TARGET != DC
	CPane::Init();

	CIconBank *icons = new CIconBank("disk.tga");
	ASSERT(icons->IsValid());

	AddIconButton(  4,  5, EDITOR.mDataEntryIcons, 0, ED_PREV_LEV, "Previous Level");
//	AddIconButton( 48,  5, EDITOR.mDataEntryIcons, 2, ED_LEV_NO	, "Level No");
	AddIconButton( 44,  5, EDITOR.mDataEntryIcons, 3, ED_LEV_NO	, "Level No");
	AddIconButton( 66,  5, EDITOR.mDataEntryIcons, 4, ED_LEV_NO	, "Level No");
	AddIconButton(106,  5, EDITOR.mDataEntryIcons, 1, ED_NEXT_LEV, "Next Level");

	AddIconButton(147,  5, icons, 0, ED_SAVE			, "Save Level");
	AddIconButton(185,  5, icons, 1, ED_DISCARD			, "Discard Level");
	AddIconButton(223,  5, icons, 2, ED_UNDO			, "Undo!");
	AddIconButton(261,  5, icons, 3, ED_REDO			, "Redo!");
	AddIconButton(301,  5, icons, 4, ED_REFRESH_LEVEL	, "Refresh Level");

	delete icons;

	DeselectButton();

	UpdateState();
#endif	
}

//******************************************************************************************
void	CPaneDisk::UpdateState()
{
#if TARGET != DC
	if (EDITOR.HasCurrentLevelChanged())
	{
		DeActivateButton(ED_PREV_LEV);
		DeActivateButton(ED_NEXT_LEV);
//		ActivateButton(ED_SAVE);
		ActivateButton(ED_DISCARD);
//		ActivateButton(ED_REFRESH_LEVEL);
	}
	else
	{
		ActivateButton(ED_PREV_LEV);
		ActivateButton(ED_NEXT_LEV);
//		DeActivateButton(ED_SAVE);
		DeActivateButton(ED_DISCARD);
//		DeActivateButton(ED_REFRESH_LEVEL);
	}

	if (EDITOR.CanIUndo())
	{
		ActivateButton(ED_UNDO);
	}
	else
	{
		DeActivateButton(ED_UNDO);
	}

	if (EDITOR.CanIRedo())
	{
		ActivateButton(ED_REDO);
	}
	else
	{
		DeActivateButton(ED_REDO);
	}

	EDITOR.	UpdateAllStates();
#endif	
}

//******************************************************************************************
void	CPaneDisk::Redraw()
{
#if TARGET != DC
	CPane::Redraw();

	char	l[10];

	sprintf(l, "%03d", GAME.GetCurrentLevelNumber());

	DrawFontText(53, 14, l, 100, 255, 100);
#endif	
}

//******************************************************************************************
void	CPaneDisk::ProcessAction(EEdAction action)
{
#if TARGET != DC
	switch (action)
	{
	case ED_PREV_LEV:
		{
			SINT	l = GAME.GetCurrentLevelNumber() -1;
			if (l < 0)  l = 999;
			EDITOR.ClearSelections();
			BOOL	res = GAME.LoadLevel(l);
			CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!
			EDITOR.RegisterSelectedThing(NULL);
			// print a message?
			DeselectButton();
			EDITOR.UpdateAllStates();
			break;
		}
	case ED_NEXT_LEV:
		{
			SINT	l = GAME.GetCurrentLevelNumber() + 1;
			if (l > 999)  l = 0;
			EDITOR.ClearSelections();
			BOOL	res = GAME.LoadLevel(l);
			CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!
			EDITOR.RegisterSelectedThing(NULL);
			// print a message?
			DeselectButton();
			EDITOR.UpdateAllStates();
			break;
		}
	case ED_SAVE:
		EDITOR.EnableDialogBox("Save Level?", DIALOG_YESNO, this);
		break;

	case ED_DISCARD:
		EDITOR.EnableDialogBox("Discard Level?", DIALOG_NOYES, this);
		break;


	case ED_YES:
		switch (mButtons[mCurrentButton].mAction)
		{
		case ED_SAVE:
			{
				// save!
				BOOL	res = GAME.SaveLevel();
				res = GAME.SavePSXLevel();
				//! do something with res..
				EDITOR.RegisterLevelUnchanged();
				EDITOR.ResetUndo();
				EDITOR.UpdateAllStates();
			}
			break;
			case ED_DISCARD:
			{
				// discard!
				EDITOR.ClearSelections();
				BOOL	res = GAME.LoadLevel(GAME.GetCurrentLevelNumber());
				CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!
				EDITOR.RegisterSelectedThing(NULL);
				//! do something with res..
				EDITOR.RegisterLevelUnchanged();
				EDITOR.ResetUndo();
				EDITOR.UpdateAllStates();
			}
			break;

		default:
			ASSERT(0);
		}

		DeselectButton();
		break;
	case ED_NO:

		DeselectButton();
		break;

	case ED_UNDO:
		EDITOR.Undo();
		DeselectButton();
		break;

	case ED_REDO:
		EDITOR.Redo();
		DeselectButton();
		break;

	case ED_REFRESH_LEVEL:
		EDITOR.RefreshLevel();
		DeselectButton();
		break;

	default:
		DeselectButton();
		break;
	};
#endif	
}
//******************************************************************************************
//** CPaneDialog
//******************************************************************************************
void	CPaneDialog::InitDialog(EEdDialogType type, CPane *caller, char *text)
{
#if TARGET != DC
	strcpy(mText, text);
	mCaller = caller;
	mType = type;

	switch(type)
	{
	case DIALOG_YESNO:
		AddIconButton( 30,  5, EDITOR.mDialogIcons, 0, ED_YES, "Yes!");
		AddIconButton( 70,  5, EDITOR.mDialogIcons, 1, ED_NO , "No!");
		break;
	case DIALOG_NOYES:
		AddIconButton( 30,  5, EDITOR.mDialogIcons, 1, ED_NO , "No!");
		AddIconButton( 70,  5, EDITOR.mDialogIcons, 0, ED_YES, "Yes!");
		break;
	default:
		ASSERT(0);
	};

	DeselectButton();

	// draw text
	DrawFontText(120, 15, text, 255, 200, 30);
#endif	
}

void	CPaneDialog::ProcessAction(EEdAction action)
{
#if TARGET != DC
	// pass to caller!
	mCaller->ProcessAction(action);
	EDITOR.RequestDialogDisable();
#endif	
}

//******************************************************************************************
//******************************************************************************************
//** CLevelEditor2
//******************************************************************************************
BOOL	CLevelEditor2::Init()
{
#if TARGET != DC
	mActive = FALSE;
	mActionPane = NULL;

	mSystemIcons = new CIconBank("SystemIcons.tga");
	ASSERT(mSystemIcons->IsValid());

	mScrollIcons = new CIconBank("Scrollbar.tga");
	ASSERT(mScrollIcons->IsValid());

	mDataEntryIcons = new CIconBank("DataEntry.tga");
	ASSERT(mDataEntryIcons->IsValid());

	mDialogIcons = new CIconBank("Dialog.tga");
	ASSERT(mDialogIcons->IsValid());

	mFont = new CIconBank("font.tga", 16);
	ASSERT(mFont->IsValid());


	SINT size = mSystemIcons->GetSize();
	mPaneMouse = new CPane(size, size, 0, 0, EPS_NONE, SRCPAINT, TRUE);
	mSystemIcons->GetIcon(0, mPaneMouse->GetAddr(0, 0), size);
	mPaneMouse->Activate();

	mPaneTip = new CPaneTip(630, 30, 5, 446, EPS_BOTTOM);
	mPaneTip->Init();
	mPaneTip->Activate();

	mPaneDisk = new CPaneDisk(340, 42, 5, 402, EPS_RIGHT);
	mPaneDisk->Init();
	mPaneDisk->Activate();

//	mPaneMain = new CPaneMain(340, 50, 5, 5, EPS_LEFT);
	mPaneMain = new CPaneMain(460, 50, 5, 5, EPS_LEFT);
	mPaneMain->Init();
	mPaneMain->Activate();

	mDialogActive = FALSE;
	mDialogPane = NULL;
	mPleaseDisableDialog = FALSE;

	mSelectionStart.X = -1; // no selection

	mUpdating = FALSE;
#endif
	return TRUE;
}

//******************************************************************************************
void	CLevelEditor2::Shutdown()
{
#if TARGET != DC
	// delete all panes
	CPane	*p = CPane::mFirstPane;

	while (p)
	{
		CPane *t = p->mNextPane;
		delete p;
		p = t;
	};

	delete mFont;
	mFont = NULL;

	delete mSystemIcons;
	mSystemIcons = NULL;

	delete mScrollIcons;
	mScrollIcons = NULL;

	delete mDataEntryIcons;
	mDataEntryIcons = NULL;

	delete mDialogIcons;
	mDialogIcons = NULL;
#endif	
}

//******************************************************************************************
void	CLevelEditor2::Process()
{
#if TARGET != DC
	mPaneTip->ClearText(); // clear tool tip bar

	POINT	pt = CONTROLS.GetMousePos();
	CPane	*p = CPane::mFirstPane;

	SINT	mw = PLATFORM.GetMouseWheelData();

	if (mDialogActive)
	{
		// MODAL!  just process Dialog
		mDialogPane->Process(pt.x, pt.y,
						   CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY),
						   CONTROLS.IsButtonPressedDB(BUTTON_FIRE_SECONDARY), mw);

		// did it finish?
		if (mPleaseDisableDialog)
		{
			// yes!
			mPleaseDisableDialog = FALSE;
			mDialogActive = FALSE;
//			delete mDialogPane;
//			mDialogPane = NULL;
			//! NOTE WELL - don't delete this now.  and the next time, create a new dialog pane
			// using this pointer. They'll all get cleaned up at shutdown, honest!
			mDialogPane->DeActivate();

		}
	}
	else
	{
		// normal process
		while (p)
		{
			if (p->Process(pt.x, pt.y,
						   CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY),
						   CONTROLS.IsButtonPressedDB(BUTTON_FIRE_SECONDARY), mw))

				break; // hit this pane
			p = p->mNextPane;
		}

		if (p == NULL)
		{
			if (mActionPane)
				mActionPane->ProcessOtherActions(pt.x, pt.y,
												   CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY),
												   CONTROLS.IsButtonPressedDB(BUTTON_FIRE_SECONDARY));
		}
	
		// let the action pane get keys first
		BOOL stop = FALSE;

		if (mActionPane)
			stop = mActionPane->ProcessKeys();
		
		if (!stop)
		{
			p = CPane::mFirstPane;

			// process the other panes
			while (p)
			{
				if ((p != mActionPane) && (p->IsActive()))
					p->ProcessKeys();

				p = p->mNextPane;
			};
		}
	}

	// quit?
	if (CONTROLS.IsButtonPressedDB(BUTTON_EDITOR))
		GAME.ToggleEditor();
#endif		
}

//******************************************************************************************
void	CLevelEditor2::SetTipText(char *t)
{
#if TARGET != DC
	mPaneTip->SetText(t);
	
	mPaneTip->Redraw();
#endif	
}

//******************************************************************************************
void	CLevelEditor2::Activate()
{
#if TARGET != DC
	CONTROLS.Process(); //! hmm..  run once so editor doesn't quit straight away!

	mActive = TRUE;
	ClearSelections();
	GAME.LoadLevel(GAME.GetCurrentLevelNumber());
		
	mLastControllerMode = CONTROLS.GetControllerMode();
	CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!

	// mmm..
	CPane *p = CPane::mFirstPane;
	while (p)
	{
		p->mSlideCount = SLIDE_COUNT_MAX;
		// redraw all panes
		p->Redraw();

		p = p->mNextPane;
	}

	// bollox - the mouse pain is a hack anyway - recreate here...
	mSystemIcons->GetIcon(0, mPaneMouse->GetAddr(0, 0), mSystemIcons->GetSize());

	mHasCurrentLevelChanged = FALSE;
	ResetUndo();

	mSelectedThing = NULL;
#endif	
}

//******************************************************************************************
void	CLevelEditor2::DeActivate()
{
#if TARGET != DC
	mActive = FALSE;
	RegisterSelectedThing(NULL);

	// assume we've saved!
	ClearSelections();
	GAME.LoadLevel(GAME.GetCurrentLevelNumber());

//	// restore control mode
//	CONTROLS.SetControllerMode(mLastControllerMode);
#endif
}

//******************************************************************************************
void	CLevelEditor2::EnableDialogBox(char *text, EEdDialogType type, CPane *caller)
{
#if TARGET != DC
	ASSERT(!mDialogActive); //!hmmmm.

	// create dialog pane
	mDialogPane = new CPaneDialog(286, 42, 350, 402, EPS_RIGHT);
	mDialogPane->Init();
	mDialogPane->InitDialog(type, caller, text);
	mDialogPane->Activate();

	mDialogActive = TRUE;
#endif	
}

//******************************************************************************************
void	CLevelEditor2::SetTextColour(SINT r, SINT g, SINT b)
{
#if TARGET != DC
	mTextColour = RGB(r, g, b);
#endif	
}

//******************************************************************************************
void	CLevelEditor2::DrawText(SINT x, SINT y, char *t)
{
#if TARGET != DC
	SINT	size = strlen(t);

	SetTextColor(mHDC, RGB(0,0,0));
	TextOut(mHDC, x + 2, y + 2, t, size);
	SetTextColor(mHDC, mTextColour);
	TextOut(mHDC, x    , y    , t, size);
#endif	
}
	
//******************************************************************************************
void	CLevelEditor2::Render2D()
{
#if TARGET != DC
	mHDC = PLATFORM.GetDPWin()->GetDC(); // ####!!!!

	SetBkMode(mHDC, TRANSPARENT);

	// Title Stuff
	SetTextColour(255,255,255);
	DrawText(410, 20, "Level Editor!");
	DrawText(410, 30, "--------------------");

	// Move cursor
	POINT	pt = CONTROLS.GetMousePos();
	mPaneMouse->Move(pt.x, pt.y);

	// Draw Panes...
	CPane *p = CPane::mFirstPane;
	while (p)
	{
		p->Render(mHDC);
		p = p->mNextPane;
	}

	PLATFORM.GetDPWin()->ReleaseDC(mHDC); // ####!!!!
#endif	
}

//******************************************************************************************
//** Undo Stuff

void	CLevelEditor2::RegisterLevelChange(int action)
{
#if TARGET != DC
	mHasCurrentLevelChanged = TRUE;
	if (mUndoLastAction != action)
	{
		// save undo
		GAME.SaveLevel(10000 + mUndoCount);
		mUndoCount ++;
		mUndoMax = mUndoCount;

		mUndoLastAction = action;

	}

	mPaneDisk->UpdateState();
#endif	

}

//******************************************************************************************
void	CLevelEditor2::RegisterLevelUnchanged()
{
#if TARGET != DC
	mHasCurrentLevelChanged = FALSE;

	UpdateAllStates();
#endif	

}

//******************************************************************************************
void	CLevelEditor2::ResetUndo()
{
#if TARGET != DC
	mUndoCount = 0;
	mUndoMax = 0;
	mUndoLastAction = ED_NONE;
#endif	
}

//******************************************************************************************
void	CLevelEditor2::Undo()
{
#if TARGET != DC
	if (mUndoCount != 0)
	{
		if (mUndoCount == mUndoMax)
			GAME.SaveLevel(10000 + mUndoCount); // save current state
		mUndoCount --;
		ClearSelections();
		GAME.LoadLevel(10000 + mUndoCount); // load undo state
		CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!
		EDITOR.RegisterSelectedThing(NULL);
	}
	UpdateAllStates();
	mUndoLastAction = ED_NONE;
#endif	
}

//******************************************************************************************
void	CLevelEditor2::Redo()
{
#if TARGET != DC
	if (mUndoCount < mUndoMax)
	{
		mUndoCount ++;
		ClearSelections();
		GAME.LoadLevel(10000 + mUndoCount);
		CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!
		EDITOR.RegisterSelectedThing(NULL);
	}
	UpdateAllStates();
	mUndoLastAction = ED_NONE;
#endif	
}

//******************************************************************************************
void	CLevelEditor2::RefreshLevel()
{
#if TARGET != DC
	// save & load to sort out heights etc...
	GAME.SaveLevel(10000 + 999);
	ClearSelections();
	GAME.LoadLevel(10000 + 999);
	CONTROLS.SetControllerMode(CM_MOUSE);  // so we can edit!!

	EDITOR.UpdateAllStates();
	// hope undo doesn't get this far.....
#endif	
}	

//******************************************************************************************
//** Selection Stuff

void	CLevelEditor2::DeclareSelection(CCellRef &start, CCellRef &end)
{
#if TARGET != DC
	mSelectionStart = start;
	mSelectionEnd = end;

	UpdateAllStates();
#endif	
}

//******************************************************************************************
void	CLevelEditor2::DisableSelection()
{
#if TARGET != DC
	mSelectionStart.X = -1;
	UpdateAllStates();
#endif	
}

//******************************************************************************************
BOOL	CLevelEditor2::IsPointInSelectedRegion(CCellRef &c)
{
#if TARGET != DC
	if (mSelectionStart.X == -1)
		return FALSE; // no region selected

	return ((c.X >= mSelectionStart.X) &&
			(c.Y >= mSelectionStart.Y) &&
			(c.X <= mSelectionEnd.X) &&
			(c.Y <= mSelectionEnd.Y));
#endif
}

//******************************************************************************************
CThing	*CLevelEditor2::GetSelectedTargetThing()
{
#if TARGET != DC
	if (mSelectedThing)
	{
		CInitThing	i;
		mSelectedThing->WriteData(i);

		CThing *t;
		WORLD.GetThing(i.mTarget, &t);
		return t;
	}
#endif
	return NULL;
}

//******************************************************************************************

#endif
#endif