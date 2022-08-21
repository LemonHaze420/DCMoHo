#include	"Common.h"

#ifdef LINK_EDITOR
#if EDITOR_VERSION == 1

#include	"LevelEditor.h"
#include	"Globals.h"
#include	"Thing.h"


// only here...
#define		LD	GAME.GetLevelData()

//******************************************************************************************
BOOL	CLevelEditor::Init()
{
	mActive = FALSE;
	mTextColour = RGB(255,255,255);

	mCurrentMenu = &mLEMMain;
	mCurrentItem = 0;
	mNumItems = 0;
	mHasCurrentLevelChanged = FALSE;

	return TRUE;
}

//******************************************************************************************
void	CLevelEditor::Process()
{
	// Get item list
	mNumItems = 0;
	mCurrentMenu->EnumerateItems();

	if (mCurrentItem < 0)
		mCurrentItem = 0;
	if (mCurrentItem > mNumItems - 1)
		mCurrentItem = mNumItems - 1;

	mCurrentMenu->Process(mCurrentItem); // if it needs to do anything exciting...

	// quit?
	if (CONTROLS.IsButtonPressedDB(BUTTON_EDITOR))
		GAME.ToggleEditor();
}

//******************************************************************************************
HRESULT	CLevelEditor::OnKeyChange(UBYTE ascii, UINT keydata)
{
	SINT change = 0;

	switch (ascii)
	{
	case '8': change = -1; break;
	case '2': change = +1; break;

	case '+': mCurrentMenu->OnSelect(mCurrentItem); break;

	case '4': mCurrentMenu->OnLeftRight(mCurrentItem, FALSE); break;
	case '6': mCurrentMenu->OnLeftRight(mCurrentItem, TRUE ); break;

	default:
		return 1;
		break;
	};	

	// security
	if ((!change) && (!(mItemsActive[mCurrentItem])))
		change = 1;

	while (change)
	{
		mCurrentItem += (change > 0) ? 1 : -1;

		if (mCurrentItem < 0)
			mCurrentItem = mNumItems - 1;
		if (mCurrentItem > mNumItems - 1)
			mCurrentItem = 0;

		if (mItemsActive[mCurrentItem])
			change -= (change > 0) ? 1 : -1;
	};

	return 0;
}

//******************************************************************************************
void	CLevelEditor::Activate()
{
	CONTROLS.Process(); //! hmm..  run once so editor doesn't quit straight away!
	mActive = TRUE;
//	MAP.ResetOffsets(); MAP.PrecalcCells(); // reset map
	GAME.LoadLevel(GAME.GetCurrentLevelNumber());
}

//******************************************************************************************
void	CLevelEditor::DeActivate()
{
	mActive = FALSE;
}

//******************************************************************************************

void	CLevelEditor::SetTextColour(SINT r, SINT g, SINT b)
{
	mTextColour = RGB(r, g, b);
}

//******************************************************************************************
void	CLevelEditor::DrawText(SINT x, SINT y, char *t)
{
	SINT	size = strlen(t);

	SetTextColor(mHDC, RGB(0,0,0));
	TextOut(mHDC, x + 2, y + 2, t, size);
	SetTextColor(mHDC, mTextColour);
	TextOut(mHDC, x    , y    , t, size);
}

//******************************************************************************************

#define LEM_Y	90
#define	LEM_YO	15

void	CLevelEditor::Render2D()
{
	mHDC = PLATFORM.GetDPWin()->GetDC(); // ####!!!!

	SetBkMode(mHDC, TRANSPARENT);

	// Title Stuff
	SetTextColour(255,255,255);
	DrawText(10, 50, "Level Editor!");
	DrawText(10, 60, "--------------------");
	DrawText(10, 460, "F6 to exit editor");

	// Draw Menu
	SINT c0;
	char	text[200];

	for (c0 = 0; c0 < mNumItems; c0++)
	{
		SetTextColour(0,100,150);
		sprintf(text, "%d)", c0 + 1);
		DrawText(5,  LEM_Y + (LEM_YO * c0), text);
		if (c0 == mCurrentItem)
			SetTextColour(255, 255, 255);
		else  if (mItemsActive[c0])
			SetTextColour(255, 150, 30 );
		else
			SetTextColour(100,100,100);
		DrawText(40, LEM_Y + (LEM_YO * c0), mItems[c0]);
	}

	PLATFORM.GetDPWin()->ReleaseDC(mHDC); // ####!!!!
}

//******************************************************************************************
void	CLevelEditor::AddMenuItem(char *text, BOOL active)
{
	if (mNumItems < MAX_LEM_ITEMS)
	{
		strcpy(mItems[mNumItems], text);
		mItemsActive[mNumItems] = active;
		mNumItems ++;
	}
}


//******************************************************************************************
//******************************************************************************************

void	CLEMMain::EnumerateItems()
{
	EDITOR.AddMenuItem("Landscape Menu");
	EDITOR.AddMenuItem("Thing Placement Editor");
	EDITOR.AddMenuItem("Level Variables Editor");
	char	text[200];
	sprintf(text, "Current Level: %02d ", GAME.GetCurrentLevelNumber());
	EDITOR.AddMenuItem(text, !EDITOR.mHasCurrentLevelChanged);
	EDITOR.AddMenuItem("Save", EDITOR.mHasCurrentLevelChanged);
	EDITOR.AddMenuItem("Discard", EDITOR.mHasCurrentLevelChanged);
}

//******************************************************************************************
void	CLEMMain::OnSelect(SINT item)
{
	switch (item)
	{
	case 0:		
		EDITOR.mHasCurrentLevelChanged = TRUE;
		EDITOR.ChangeMenu(&EDITOR.mLEMLandscapeMenu);
		break;
	case 1:
		EDITOR.mHasCurrentLevelChanged = TRUE;
		EDITOR.ChangeMenu(&EDITOR.mLEMThingEdit);
		break;
	case 2:
		EDITOR.mHasCurrentLevelChanged = TRUE;
		EDITOR.ChangeMenu(&EDITOR.mLEMVarEdit);
		break;
	case 4:
		EDITOR.ChangeMenu(&EDITOR.mLEMSaveOK);
		break;
	case 5:
		EDITOR.ChangeMenu(&EDITOR.mLEMDiscardOK);
		break;
	default:	break;
	};
}

//******************************************************************************************
void	CLEMMain::OnLeftRight(SINT item, BOOL right)
{
	switch (item)
	{
	case 3:
		// level selector
		SINT	l = GAME.GetCurrentLevelNumber() + ((right) ? 1 : -1);
		if (l < 0)  l = 99;
		if (l > 99) l = 0;

		BOOL	res = GAME.LoadLevel(l);

		char text[250];
		if (res)
			sprintf(text, "Level %02d loaded successfully.", l);
		else
			sprintf(text, "Level %02d failed to load.", l);

		EDITOR.mHasCurrentLevelChanged = FALSE;
		CONSOLE.AddString(text);

		break;
	};
}

//******************************************************************************************
//******************************************************************************************

void	CLEMLandscapeMenu::EnumerateItems()
{
	EDITOR.AddMenuItem("Landscape Generator");
	EDITOR.AddMenuItem("Landscape Editor");
	EDITOR.AddMenuItem("Tile Editor");
	EDITOR.AddMenuItem("Back");
}

//******************************************************************************************
void	CLEMLandscapeMenu::OnSelect(SINT item)
{
	switch (item)
	{
	case 0:		EDITOR.ChangeMenu(&EDITOR.mLEMLandscapeGen); break;
	case 1:		EDITOR.ChangeMenu(&EDITOR.mLEMLandscapeEditor); break;
	case 2:		EDITOR.ChangeMenu(&EDITOR.mLEMTileEditor); break;
	case 3:		EDITOR.ChangeMenu(&EDITOR.mLEMMain); break;
	default:	break;
	};
}
//******************************************************************************************
//******************************************************************************************

void	CLEMLandscapeEditor::EnumerateItems()
{
	EDITOR.AddMenuItem("Fine Tool");
	EDITOR.AddMenuItem("Medium Tool");
	EDITOR.AddMenuItem("Coarse Tool");
	EDITOR.AddMenuItem("Back");
}

//******************************************************************************************
void	CLEMLandscapeEditor::OnSelect(SINT item)
{
	switch (item)
	{
	case 3:		EDITOR.ChangeMenu(&EDITOR.mLEMLandscapeMenu); break;
	default:	break;
	};
}

//******************************************************************************************
void	CLEMLandscapeEditor::OnLeftRight(SINT item, BOOL right)
{
}

//******************************************************************************************
void	CLEMLandscapeEditor::Process(SINT item)
{
	if (PLATFORM.IsMouseButtonPressed(0) ||
		PLATFORM.IsMouseButtonPressed(1))
	{
		SINT size = 0;

		switch (item)
		{
		case 0: size = 1; break;
		case 1: size = 3; break;
		case 2: size = 10; break;
		}

		if (size != 0)
		{
			// cool.   alter the landscape

			SINT	c0, c1;
			SINT	px = MAP.GetCursor().X;
			SINT	py = MAP.GetCursor().Y;

			if (px != -1)
			{
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
								if (PLATFORM.IsMouseButtonPressed(0))
									val = -val; // up!

								MAP.GetCell(CCellRef(c0, c1))->AddHeight(FToG(val));
							}
						}
					}
				MAP.PrecalcCells();
			}
		}
	}

	// Flatten tool
	if (PLATFORM.IsKeyDown(VK_SPACE))
	{
		SINT size = 0;

		switch (item)
		{
		case 0: size = 1; break;
		case 1: size = 3; break;
		case 2: size = 10; break;
		}

		if (size != 0)
		{
			// cool.   alter the landscape

			SINT	c0, c1;
			SINT	px = MAP.GetCursor().X;
			SINT	py = MAP.GetCursor().Y;

			float	average = 0;
			SINT	num_cells = 0;

			if (px != -1)
			{
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
	}
}

//******************************************************************************************
//******************************************************************************************

void	CLEMLandscapeGen::EnumerateItems()
{
	char text[200];

	sprintf(text, "Pitch : %.2f", mPitch);
	EDITOR.AddMenuItem(text);
	sprintf(text, "Height : %.2f", mHeight);
	EDITOR.AddMenuItem(text);
	EDITOR.AddMenuItem("Texture : tex1");
	EDITOR.AddMenuItem("Back");
}

//******************************************************************************************
void	CLEMLandscapeGen::OnSelect(SINT item)
{
	switch (item)
	{
	case 3:		EDITOR.ChangeMenu(&EDITOR.mLEMLandscapeMenu); break;
	default:	break;
	};
}

//******************************************************************************************
void	CLEMLandscapeGen::OnLeftRight(SINT item, BOOL right)
{
	switch (item)
	{
	case 0: if (right) mPitch *= 1.1f; else mPitch /= 1.1f; break;
	case 1: if (right) mHeight += 1.f; else mHeight -= 1.f; break;
	default:	break;
	};

	if ((item == 0) || (item == 1))
		MAP.ClearMap(FToG(mPitch), FToG(mHeight));
}

//******************************************************************************************
//******************************************************************************************
void	CLEMTileEditor::OnActivate()
{
	mNumTextureNames = 0;

	// scan filenames!
	char			files[] = "data\\textures\\tiles\\*.*";
	WIN32_FIND_DATA data;

	HANDLE	handle = FindFirstFile(files, &data);

	if (handle != INVALID_HANDLE_VALUE)
	{
		while (TRUE)
		{
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				sprintf(mTextureNames[mNumTextureNames], "tiles\\%s", data.cFileName);
				mNumTextureNames ++;
			}

			if (mNumTextureNames == MAX_LE_TEXTURE_NAMES)
				break;

			if (!(FindNextFile(handle, &data)))
				break;
		}
		FindClose(handle);
	}

	if (mCurrentTextureNo >= mNumTextureNames)
		mCurrentTextureNo = 0;

	if (mNumTextureNames == 0)
		strcpy(mTextureNames[0], "NO TEXTURES FOUND!");
}

//******************************************************************************************
void	CLEMTileEditor::EnumerateItems()
{
	char text[200];
	sprintf(text, "Texture: %s", mTextureNames[mCurrentTextureNo]);
	EDITOR.AddMenuItem(text);
	EDITOR.AddMenuItem("Ice");
	EDITOR.AddMenuItem("Pit");
	EDITOR.AddMenuItem("BoostR");
	EDITOR.AddMenuItem("BoostL");
	EDITOR.AddMenuItem("BoostU");
	EDITOR.AddMenuItem("BoostD");
	EDITOR.AddMenuItem("Back");
}

//******************************************************************************************
void	CLEMTileEditor::OnSelect(SINT item)
{
	switch (item)
	{
	case 7:		EDITOR.ChangeMenu(&EDITOR.mLEMLandscapeMenu); break;
	default:	break;
	};
}

//******************************************************************************************
void	CLEMTileEditor::OnLeftRight(SINT item, BOOL right)
{
	SINT dir = 0;

	switch (item)
	{
	case 0:
		if (right)
			dir = 1;
		else
			dir = -1;
		
		mCurrentTextureNo += dir;
		while (mCurrentTextureNo < 0)
			mCurrentTextureNo += mNumTextureNames;

		while (mCurrentTextureNo >= mNumTextureNames)
			mCurrentTextureNo -= mNumTextureNames;

		break;
	}
}

//******************************************************************************************
void	CLEMTileEditor::Process(SINT item)
{
	CCellRef	c = MAP.GetCursor();

	if (c.X == -1)
		return; // no map pointer

	switch(item)
	{
	case 0:
		if (PLATFORM.IsMouseButtonPressed(0))
		{
			BOOL r;
			r = MAP.SetCellTexture(c, mTextureNames[mCurrentTextureNo]);

			if (!r)
				CONSOLE.AddString("Failed to load / allocate texture...");
		}
		if (PLATFORM.IsMouseButtonPressed(1))
			MAP.SetCellTextureDefault(c);
		break;

	case 1:
		if (PLATFORM.IsMouseButtonPressed(1))
			MAP.GetCell(c)->SetType(NORMAL);
		if (PLATFORM.IsMouseButtonPressed(0))
			MAP.GetCell(c)->SetType(ICE);
		break;

	case 2:
		if (PLATFORM.IsMouseButtonPressed(0))
			MAP.GetCell(c)->SetHeight(PIT_HEIGHT);
		if (PLATFORM.IsMouseButtonPressed(1))
			MAP.GetCell(c)->SetHeight(G0);
		break;

	case 3:
		if (PLATFORM.IsMouseButtonPressed(1))
			MAP.GetCell(c)->SetType(NORMAL);
		if (PLATFORM.IsMouseButtonPressed(0))
			MAP.GetCell(c)->SetType(BOOSTR);
		break;

	case 4:
		if (PLATFORM.IsMouseButtonPressed(1))
			MAP.GetCell(c)->SetType(NORMAL);
		if (PLATFORM.IsMouseButtonPressed(0))
			MAP.GetCell(c)->SetType(BOOSTL);
		break;

	case 5:
		if (PLATFORM.IsMouseButtonPressed(1))
			MAP.GetCell(c)->SetType(NORMAL);
		if (PLATFORM.IsMouseButtonPressed(0))
			MAP.GetCell(c)->SetType(BOOSTU);
		break;

	case 6:
		if (PLATFORM.IsMouseButtonPressed(1))
			MAP.GetCell(c)->SetType(NORMAL);
		if (PLATFORM.IsMouseButtonPressed(0))
			MAP.GetCell(c)->SetType(BOOSTD);
		break;
	};
}
//******************************************************************************************
//******************************************************************************************

void	CLEMThingEdit::EnumerateItems()
{
	char	text[250];

	// get thing name
	//! hmm.....
	CThing *t = SpawnThing(mThingID);
	if (t)
	{
		sprintf(text, "Thing: %s", t->_GetClassName());
		delete t;
	}
	else
		sprintf(text, "Invalid id: %d", mThingID);

	EDITOR.AddMenuItem(text);
	EDITOR.AddMenuItem("Back");
}

//******************************************************************************************
void	CLEMThingEdit::OnSelect(SINT item)
{
	switch (item)
	{
	case 1:		EDITOR.ChangeMenu(&EDITOR.mLEMMain); break;
	default:	break;
	};
}

//******************************************************************************************
void	CLEMThingEdit::OnLeftRight(SINT item, BOOL right)
{
	SINT dir = 0;

	switch (item)
	{
	case 0:
		if (right)
			dir = 1;
		else
			dir = -1;
		
		while (dir != 0)
		{
			mThingID += dir;
			if (mThingID < 0)
				mThingID = OID_EOL - 1;
			if (mThingID > OID_EOL - 1)
				mThingID = 0;

			//! hmm.....
			CThing *t = SpawnThing(mThingID);
			if (t)
			{
				if (t->IsPersistent())
					dir -= ((dir > 0) ? 1 : -1);

				delete t;
			}
		}
		break;
	}
}

//******************************************************************************************
void	CLEMThingEdit::Process(SINT item)
{
//	if (PLATFORM.IsMouseButtonPressed(0))
	if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
	{
		CCellRef c = MAP.GetCursor();

		if (c.X != -1)
		{
			// add thing here
//			if ((MAP.IsCellFree(px, py)) && (MAP.GetCell(px, py)->GetType() == NORMAL))

			// is there already something of this type here...
			if (MAP.GetCell(c)->GetType() == NORMAL)
			{
				CThing *m = MAP.GetFirstThing(c.ToGVector(), GINT_HALF);
				BOOL	v = TRUE;

				while (m)
				{
					if ((m->GetPos() - c.ToGVector()).Magnitude() < G(2))
						if (m->_GetClassID() == mThingID)
						{
							v = FALSE;
							break;
						}
					m = MAP.GetNextThing();
				}

				if (v)
				{
					CThing *t = SpawnThing(mThingID);
					if (t)
					{
						CInitThing i;
						i.mPos = GVector(G(c.X) + GINT_HALF, G(c.Y) + GINT_HALF, G0);
						t->Init(&i);
					};
				}
			}
		}
	}

//	if (PLATFORM.IsMouseButtonPressed(1))
	if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_SECONDARY))
	{
		CCellRef c = MAP.GetCursor();

		if (c.X != -1)
		{
			// delete thing here

			CThing *t;

			if ((t = MAP.GetFirstThing(c.ToGVector(), GINT_HALF)) != NULL)
			{
				WORLD.DestroyThing(t); // whatever...
			}
		}
	}
}

//******************************************************************************************
//******************************************************************************************

void	CLEMVarEdit::EnumerateItems()
{
	char text[200];
	SINT c0;

	for (c0 = 0; c0 < NUM_LEVEL_IVARS; c0 ++)
	{
		sprintf(text, "Int Variable %d : %d", c0, LD->mIVars[c0]);
		EDITOR.AddMenuItem(text);
	}
	for (c0 = 0; c0 < NUM_LEVEL_GVARS; c0 ++)
	{
		sprintf(text, "GINT Variable %d : %f", c0, GToF(LD->mGVars[c0]));
		EDITOR.AddMenuItem(text);
	}

	EDITOR.AddMenuItem("Back");
}

//******************************************************************************************
void	CLEMVarEdit::OnSelect(SINT item)
{
	switch (item)
	{
	case NUM_LEVEL_IVARS + NUM_LEVEL_GVARS:		EDITOR.ChangeMenu(&EDITOR.mLEMMain); break;
	default:	break;
	};
}

//******************************************************************************************
void	CLEMVarEdit::OnLeftRight(SINT item, BOOL right)
{
	if (item < NUM_LEVEL_IVARS)
	{
		if (right)
			 LD->mIVars[item] ++;
		else LD->mIVars[item] --;
	}
	else
	{
		item -= NUM_LEVEL_IVARS;
		if (item < NUM_LEVEL_GVARS)
		{
			if (right)
				 LD->mGVars[item] += GINT(0, 6554);
			else LD->mGVars[item] -= GINT(0, 6554);
		}
	}
}

//******************************************************************************************
//******************************************************************************************

void	CLEMSaveOK::EnumerateItems()
{
	EDITOR.AddMenuItem("Don't save");
	EDITOR.AddMenuItem("Save");
}

//******************************************************************************************
void	CLEMSaveOK::OnSelect(SINT item)
{
	switch (item)
	{
	case 0:		EDITOR.ChangeMenu(&EDITOR.mLEMMain); break;
	case 1:		
		{
			BOOL	res = GAME.SaveLevel();

			char text[250];
			if (res)
			{
				sprintf(text, "Level %02d saved successfully.", GAME.GetCurrentLevelNumber());
				EDITOR.mHasCurrentLevelChanged = FALSE;
			}
			else
				sprintf(text, "Level %02d failed to save.", GAME.GetCurrentLevelNumber());

			CONSOLE.AddString(text);
			
			
			EDITOR.ChangeMenu(&EDITOR.mLEMMain);
			break;
		}
	default:	break;
	};
}

//******************************************************************************************
//******************************************************************************************

void	CLEMDiscardOK::EnumerateItems()
{
	EDITOR.AddMenuItem("Don't Discard");
	EDITOR.AddMenuItem("Discard");
}

//******************************************************************************************
void	CLEMDiscardOK::OnSelect(SINT item)
{
	switch (item)
	{
	case 0:		EDITOR.ChangeMenu(&EDITOR.mLEMMain); break;
	case 1:		
		{
			BOOL	res = GAME.LoadLevel(GAME.GetCurrentLevelNumber());

			char text[250];
			if (res)
			{
				sprintf(text, "Level %02d discarded successfully.", GAME.GetCurrentLevelNumber());
			}
			else
				sprintf(text, "Level %02d failed to discard properly.", GAME.GetCurrentLevelNumber());

			CONSOLE.AddString(text);
			EDITOR.mHasCurrentLevelChanged = FALSE;		
			EDITOR.ChangeMenu(&EDITOR.mLEMMain);
			break;
		}
	default:	break;
	};
}

#endif
#endif