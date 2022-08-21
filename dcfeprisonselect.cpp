#include	"Common.h"

#if TARGET == DC

#include	"DCFEPrisonSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
#include	"DCMemCard.h"
#include	"Career.h"
#include	"Globals.h"
#include	"DCFMV.h"
#include	"DCBuildType.h"
#include	"Credits.h"
#include	"DCCheats.h"
#include    "maincode.h"
#define		MENUTEXTOFFSET	9
#define 	MEDALOFFSET	(-8)
#define		ARROWOFFSET 5

CDCFEPrisonSelect	PRISONSELECT;

#define SLIDE_TIME	20

#define PS2GAME_COUNT	35
#define	PS2GAME_W2B		25

#define COL_GOLD   127, 96, 0
#define	COL_BLUE   64, 92, 128

#define	TITLE_BAR_Y	10

//#define	CREDITS_TIME 2250
#define	CREDITS_TIME 2000

#define	MAIN_TIMEOUT	(50 * 30)
#define	START_TIMEOUT	(50 * 30)  // runs in a frame!

//#define	MAIN_TIMEOUT	(25 * 30)
//#define	START_TIMEOUT	(50 * 30)  // runs in a frame!

#define BLOCKS_NEEDED_TO_SAVE	MCGetBlocksNeededToSave()

BOOL	vmus_in[8];

extern	KMBOOL SoftResetAllowed;

//******************************************************************************************
// Frontend data cache

#define 	FEDATASIZE  5100588  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

char	front_end_data[5200000];
SINT	front_end_data_loaded = -1;
SINT	front_end_data_size;

//******************************************************************************************
// Sound stuff

#define	SOUNDCANCEL	SOUND.PlaySample(SOUND_CANCEL)
#define	SOUNDSELECT	SOUND.PlaySample(SOUND_SELECT)
#define	SOUNDERROR	SOUND.PlaySample(SOUND_ERROR)
#define SOUNDNAME	SOUND.PlaySample(SOUND_NAME)
#define	SOUNDMOVE	SOUND.PlaySample(SOUND_MOVE)
#define	SOUNDCHEAT	SOUND.PlaySample(SOUND_CHEAT)
#define SOUNDSTART	SOUND.PlaySample(SOUND_START)


//******************************************************************************************
// Controller stuff
#define	CONTROLLER_SELECT	((!(CONTROLS.IsButtonPressed(BUTTON_DC_L))) && (CONTROLS.IsButtonPressedDB(BUTTON_DC_A) || CONTROLS.IsButtonPressedDB(BUTTON_DC_C) || CONTROLS.IsButtonPressedDB(BUTTON_DC_START)))
#define	CONTROLLER_CANCEL	((!(CONTROLS.IsButtonPressed(BUTTON_DC_L))) && (CONTROLS.IsButtonPressedDB(BUTTON_DC_B)))

//******************************************************************************************
// lighting values

SINT	title_lightning_places[][2]=
{
	{64,31}, {95, 15}, {118, 7}, {94, 47}, {118, 55}, {134, 31},
	{449, 31}, {419, 15}, {395, 7}, {419, 47}, {395, 55}, {376, 31}
};

#define NUM_TITLE_LIGHTING_PAIRS	20
SINT	title_lightning_pairs[NUM_TITLE_LIGHTING_PAIRS][2] =
{
	{0, 1}, {0, 3}, {1, 2}, {3, 4}, {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5},
	{6, 7}, {6, 9}, {7, 8}, {9, 10}, {6, 11}, {7, 11}, {8, 11}, {9, 11}, {10, 11},
	{2, 8}, {4, 10}
};


//******************************************************************************************

//******************************************************************************************

//******************************************************************************************
BOOL	can_select_doctor()
{
#ifdef PRESS_PREVIEW
	return FALSE;
#endif

#if DC_PREVIEW == DCP_FIRST
	return FALSE;
#endif

	if (GAME.IsCheatEnabled(CHEAT_DOCTOR))
		return TRUE;

	// has any character finished all levels?

	SINT	c0;

	for (c0 = 0; c0 < 5; c0 ++)
	{
		SINT	n = CAREER.mCharCareers[c0].mNumLevelsCompleted;

		if (n >= CLevelStructure::GetNumLevelsTotal() - 1)
			return TRUE;
	}
	return FALSE;
}

SINT	CDCFEPrisonSelect::EvaluateSingleLevelPerformance()
{
	// work out how well we did.

	//! more here
//	SINT	eval = 7; // 0xb111;

	SINT	eval = 1; // won at least!

	SINT 		time = CAREER.GetPendingVictory();
	SLevelInfo	*li = CLevelStructure::GetLevelInfo(mPrison, mArena);

	if (li->mType == LILT_GOLF)
	{
		SINT	par = CLevelStructure::GetLinksPar(mArena);

		if (time > par)
			return 0; // didn't win after all

		if (time < par)
			eval += 2;

		if (time < par - 2)
			eval += 4;

	}
	else
	{
		if (time < li->mGold)
			eval += 2;

		if (time < li->mLostToysTime)
			eval += 4;
	}

	CLevelCareer	*lc = &CAREER.mCharCareers[CAREER.mCurrentCharacter].mLevelCareers[CAREER.mCurrentPrison][CAREER.mCurrentArena];

	// are we this good already!

	if ((lc->mFinished == ECL_YES) || (lc->mFinished == ECL_GOLD) || (lc->mFinished == ECL_LT))
		eval &= (~1);

	if ((lc->mFinished == ECL_GOLD) || (lc->mFinished == ECL_LT))
		eval &= (~2);

	if (lc->mFinished == ECL_LT)
		eval &= (~4);

	mArenaNumMessages = 0;

	if (mPrison != 10)
	{
		// now work out if we unlocked anything...
		if (eval & 1)
		{
			// completed a level - did we unlock a new prison?
			SINT	n = CAREER.mCharCareers[CAREER.mCurrentCharacter].mNumLevelsCompleted + 1; // +1 - we won!

			SINT 	c0;
			for (c0 = 0; c0 < 10; c0 ++)
			{
				if (CLevelStructure::GetNumLevelsToAccess(c0) == n)
				{
					mArenaMessages[mArenaNumMessages ++] = c0;
					eval |= 8;
				}
			}

			// DOCTOR
			if ((CAREER.mCurrentCharacter != 5) && (!(can_select_doctor())))
			{
				if (n == CLevelStructure::GetNumLevelsTotal() - 1) // -1 - can't play yourself...
				{
					mArenaMessages[mArenaNumMessages ++] = 12; // unlocked doctor
					eval |= 8;
				}
			}
		}

		if (eval & 2)
		{
			// got a gold star! did we unlock a the bonus prison or a bonus level?
			SINT	n = CAREER.mCharCareers[CAREER.mCurrentCharacter].mNumGoldStars + 1; // +1 - we won!

			SINT 	c0;
			for (c0 = 0; c0 < CLevelStructure::GetNumBonusLevels(); c0 ++)
			{
				if (CLevelStructure::GetNumLevelsToAccessBonus(c0) == n)
				{
					if (c0 == 0)
						mArenaMessages[mArenaNumMessages ++] = 10; // unlocked prison
					else
						mArenaMessages[mArenaNumMessages ++] = 11; // unlocked new level

					eval |= 8;
				}
			}
			
			// ALL GOLD?
			if (CAREER.mCurrentCharacter == 5)
				n --;  // doctor needs one more...
				
			if (n == 71)
			{
				mArenaMessages[mArenaNumMessages ++] = 13; // unlocked prison
				eval |= 8;
			}			
		}
	}

	return eval;
}

//******************************************************************************************
BOOL	can_select_prison(SINT n, SINT ch)
{
	if (GAME.IsCheatEnabled(CHEAT_LEVELS))
		return TRUE;

	if (n == 10)
	{
		if (GAME.IsCheatEnabled(CHEAT_BONUS))
			return TRUE;
			
		return CAREER.mCharCareers[ch].mNumGoldStars >= CLevelStructure::GetNumLevelsToAccessBonus(0);
	}

#ifdef	PRESS_PREVIEW
	if (n == 9)
		return FALSE;
#endif


	SINT 	ncl = CAREER.mCharCareers[ch].mNumLevelsCompleted;
	SINT	nlp = CLevelStructure::GetNumLevelsToAccess(n);
	return (ncl >= nlp);
}

//******************************************************************************************
BOOL	can_select_bonus_prison(SINT ch)
{
#ifdef	PRESS_PREVIEW
	return FALSE;
#else
	if (GAME.IsCheatEnabled(CHEAT_BONUS))
		return TRUE;

	return CAREER.mCharCareers[ch].mNumGoldStars >= CLevelStructure::GetNumLevelsToAccessBonus(0);;
#endif
}

//******************************************************************************************
BOOL	can_select_arena(SINT p, SINT a, SINT ch)
{
#ifdef	PRESS_PREVIEW
	SLevelInfo	*li = CLevelStructure::GetLevelInfo(p, a);

//	printf("Checking: %d, %d: result %d\n", p, a, li->mAvailable);

	return li->mAvailable;
#else

	if (p == 9)
	{
		// alpha star
		if (a == 0)
			return TRUE;

		SINT	l4c;

		switch (ch)
		{
		case 0: l4c = 2; break;
		case 1: l4c = 4; break;
		case 2: l4c = 1; break;
		case 3: l4c = 5; break;
		case 4: l4c = 3; break;
		case 5:
		default:l4c = -1; break; // doctor has to do them all
		};

		if (a != 6)
		{
			if ((a != l4c) &&
		        (CAREER.mCharCareers[ch].mLevelCareers[9][0].mFinished != ECL_NO))
		        return TRUE;
		    else
		    	return FALSE;
		}
		else
		{
			SINT 	c0;
			for (c0 = 1; c0 < 6; c0 ++)
			{
				if ((c0 != l4c) &&
			        (CAREER.mCharCareers[ch].mLevelCareers[9][c0].mFinished == ECL_NO))
			        return FALSE;
			}

			return TRUE;
		}
	}

	if (p == 10)
	{
		// bonus
		if (GAME.IsCheatEnabled(CHEAT_BONUS))
			return TRUE;
		
		SINT	gs = CAREER.mCharCareers[ch].mNumGoldStars;

		return (gs >= CLevelStructure::GetNumLevelsToAccessBonus(a));
	}

	return TRUE;
#endif
}

//******************************************************************************************
SINT	get_levels_completed(SINT n, SINT ch)
{
	SINT 	nlp = CLevelStructure::GetNumLevelsInPrison(n);

	SINT	c0, tot = 0;
	for (c0 = 0; c0 < nlp; c0 ++)
	{
		if (CAREER.mCharCareers[ch].mLevelCareers[n][c0].mFinished)
			tot ++;
	}

	return tot;
}

//******************************************************************************************
SINT	get_char_stats(SINT c, SINT s)
{
	switch (c)
	{
	case 0:
		switch (s)
		{
			case 0: return 1;
			case 1: return 4;
			case 2: return 3;
			case 3: return 2;
		};
		break;

	case 1:
		switch (s)
		{
			case 0: return 2;
			case 1: return 3;
			case 2: return 3;
			case 3: return 2;
		};
		break;

	case 2:
		switch (s)
		{
			case 0: return 4;
			case 1: return 2;
			case 2: return 1;
			case 3: return 3;
		};
		break;

	case 3:
		switch (s)
		{
			case 0: return 4;
			case 1: return 1;
			case 2: return 2;
			case 3: return 3;
		};
		break;

	case 4:
		switch (s)
		{
			case 0: return 3;
			case 1: return 2;
			case 2: return 3;
			case 3: return 2;
		};
		break;

	case 5:
		switch (s)
		{
			case 0: return 3;
			case 1: return 2;
			case 2: return 1;
			case 3: return 4;
		};
		break;


	};
	return 0;
}

//******************************************************************************************
UINT	get_code2(SINT time, SINT level, SINT key, SINT pris)
{
	// Version 2 - uses the career name.
	
	char 	name[10];

	BOOL	done = FALSE;
	SINT	c0;
	
	for (c0 = 0; c0 < 8; c0 ++)
	{
		if (!done)
		{
			if (CAREER.mName[c0] == 0)
				done = TRUE;
			else
				name[c0] = CAREER.mName[c0];
		}
		
		if (done)
			name[c0] = '0' + c0;
	}	

	if ((time <= 0) || (time > 0xffff))
		time = 0xffff;
	
	key ^= pris;
	
	SINT	time_enc  = time ^ (key + (key << 8));
	SINT	level_enc = level ^ key;

	// checksum	
	SINT	sum = 0;
	
	for (c0 = 0; c0 < 16; c0 ++)
		sum += ((time >> c0) & 1);
		
	SINT	sum_enc = sum ^ key;
		
	key ^= pris;
	
	// extract bits
	
	SINT	timebits[16];
	SINT	keybits[8];
	SINT	sumbits[4];
	SINT	levelbits[4];
	
	for (c0 = 0; c0 < 16; c0 ++)
		timebits[c0] = (time_enc >> c0) & 1;

	for (c0 = 0; c0 < 8; c0 ++)
		keybits[c0] = (key >> c0) & 1;

	for (c0 = 0; c0 < 4; c0 ++)
		sumbits[c0] = (sum_enc >> c0) & 1;
	
	for (c0 = 0; c0 < 4; c0 ++)
		levelbits[c0] = (level_enc >> c0) & 1;
		
	// interleave and store
	SINT	code = 0;
	
	for (c0 = 0; c0 < 32; c0 ++)
	{
		if (c0 & 1)
		{
			code += timebits[c0 >> 1] << c0;
		}
		else
		{
			if (c0 < 16)
				code += keybits[c0 >> 1] << c0;
			else if (c0 < 24)
				code += sumbits[(c0 - 16) >> 1] << c0;
			else
				code += levelbits[(c0 - 24) >> 1] << c0;
		}
	}	
	
	// XOR on the name.
	code ^= name[0];
	code ^= name[1] << 8;
	code ^= name[2] << 16;
	code ^= name[3] << 24;
	code ^= name[4];
	code ^= name[5] << 8;
	code ^= name[6] << 16;
	code ^= name[7] << 24;

	return code;
}

//******************************************************************************************
BOOL	CDCFEPrisonSelect::LoadDataIntoCache(SINT n)
{
	//clear screen first
	SINT	c0;
	for (c0 = 0; c0 < 8; c0 ++)
	{
		DrawSync(0);
		VSync(0);
		SOUND.UpdateStatus();
	}
	
	CMEMBUFFER	mem;

	// from decrypt thing
	mem.DeclareFlippable(); // assuming the loading screen is setup OK!

	PLATFORM.ClearPools(); // free up global pools
	char	name[256];

	sprintf(name, "FRONTEND\\PRISEL%d.DAT", n);
	
	if (!(mem.InitFromFile(name)))
	{
		TRACE("File Load failed\n");

		mem.Close();
		return FALSE;
	}
	
	// and load into cache
	
	front_end_data_size = FEDATASIZE;
	mem.Read(front_end_data, front_end_data_size);
	front_end_data_loaded = n;
	
	mem.Close();

	return TRUE;

}

//******************************************************************************************
BOOL	CDCFEPrisonSelect::LoadDataFromCache()
{
	//clear screen first
	SINT	c0;
	for (c0 = 0; c0 < 8; c0 ++)
	{
		DrawSync(0);
		VSync(0);
		SOUND.UpdateStatus();
	}
	
	UINT	CID_FEPS	= MKID("FEPS");
	UINT	CID_TPAG	= MKID("TPAG");
	UINT	CID_MESH	= MKID("MESH");

	CMEMBUFFER	mem;


	PLATFORM.ClearPools(); // free up global pools
    SDSFreeAllTextures();

	mem.InitFromMemoryForReading(front_end_data, front_end_data_size);
	
	// check if file is valid
	UINT	chunk;
	mem.Read(&chunk, 4);

	if (chunk != CID_FEPS)
		return FALSE;

	// Read Texture page
	mem.Read(&chunk, 4);
	if (chunk != CID_TPAG)
		return FALSE;
	ENGINE.ReadPSXTextures(mem);

	// Read Meshes
	mem.Read(&chunk, 4);
	if (chunk != CID_MESH)
		return FALSE;
	ENGINE.ReadPSXMeshes(mem);

	mem.Close();

	return TRUE;
}

//******************************************************************************************
inline	SINT 	SIGN(SINT i)
{
	if (i > 0)
		return 1;
	if (i < 0)
		return -1;
	return 0;
}

//******************************************************************************************
#define PRCAM_MOMENTUM  G(0, 10000)

void	CDCFEPrisonSelect::Process()
{
	// JCL - scary quick VMU code
	// scan vmus
	if ((mMode != FEPS_QSAVE) && (mMode != FEPS_SELMEM))
	{
		SINT	c0;
		for (c0 = 0; c0 < 4; c0 ++)
		{
			if (MCCheck(c0 * 2) == VMU_EXISTS_FORMATTED)
	#ifdef JCLDCNOPSX  // ball breakers
	#endif
				SDSUpLoadLcd(c0 * 6 + 1, 6);
		}
	}

	if (mQuit == EPSXFE_NONE)
	{
		GINT xm = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT);
		GINT ym = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP);

		GINT xm2 = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, 1);
		GINT ym2 = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP, 1);


		switch (mMode)
		{

		// ****************************************************************************
		// ****************************************************************************
		// ** START

		case FEPS_START:
			if (CONTROLS.IsButtonPressedDB(BUTTON_DC_START))
			{
				if (!(CAREER.mStarted))
					CAREER.Start();		//always started in DC version
			
				mMainMenuSlide = 0;
				
				if (CAREER.mMemController == -2)
				{
					mMode = FEPS_START2SELMEM;
					mSelMemMenuSlide = 0;
					mSelMemWhereFrom = FEPS_START;
				}
				else
				{
					mMode = FEPS_START2MAIN;
					mMainMenuSlide = 0;
					mMainMenuFromSelMem = FALSE;
				}
				SOUNDSELECT;
			}
			if (mAttractCounter > START_TIMEOUT)
			{
				mQuit = EPSXFE_ATTRACT;
				mAttract ++;
				if (mAttract > 100000)
					mAttract = 1; // WHY??????????????????! ;)
			}
			break;

		case FEPS_START2MAIN:
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}
			break;

		case FEPS_START2SELMEM:
			mSelMemMenuSlide ++;
			if (mSelMemMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_SELMEM;
				mAttractCounter = 0;
				mCounter = 0;
				mSelMemMenuState = EMS_SELECT_CARD;
			}
			break;

		case FEPS_MAIN2START:
//			mMainMenuSlide --;
//			if (mMainMenuSlide == 0)
			{
				mMode = FEPS_START;
				mCounter = 0;
				mAttractCounter = 0;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** Main

		case FEPS_MAIN:
			if (ym != G0)
			{
				if (!mDBY)
				{
					if (ym > G0)
						 mMainMenuPos --;
					else mMainMenuPos ++;

#if DC_PREVIEW == DCP_FIRST
					mMainMenuPos = 0;
#endif
					mDBY = 6;
					SOUNDMOVE;
					mAttractCounter = 0;
				}
				else
					mDBY --;
			}
			else
				mDBY = FALSE;

			if (mMainMenuPos < 0)
				mMainMenuPos = 3;
			if (mMainMenuPos > 3)
				mMainMenuPos = 0;

			if ((mMainMenuPos == 1)	&& ((!(PLATFORM.IsControllerValid(1)))))
			{
				if (ym < G0)	// also deals with case where pad is pulled out while multiplayer is selected
					mMainMenuPos = 2;
				else
					mMainMenuPos = 0;
			}

			if (CONTROLLER_SELECT)
			{
				switch(mMainMenuPos)
				{
				case 0:
					mMode = FEPS_MAIN2CHAR;
					mMainMenuSlide = SLIDE_TIME;
					
					mCharMenuSlide = 0;
					mCharRotTimer = 0;
					mCharWait = -1;
					SINT	c0;
					for (c0 = 0; c0 < 6; c0 ++)
					{
						mCharThings[c0].Reset(c0);
					}
					break;

				case 1:
#ifndef PRESS_PREVIEW
					mMode = FEPS_MAIN2MCHAR;
					mMainMenuSlide = SLIDE_TIME;
					mMCharMenuSlide = 0;
					mMCharRotTimer = 0;
					mMCharSelected[0] = FALSE;
					mMCharSelected[1] = FALSE;
					mMCharWait = -1;
					{
						SINT	c0;
						for (c0 = 0; c0 < 6; c0 ++)
						{
							mMCharThings[0][c0].Reset(c0);
							mMCharThings[1][c0].Reset(c0);
						}
					}
#endif
					break;

				case 2:
					mMode = FEPS_MAIN2OPTIONS;
					mMainMenuSlide = SLIDE_TIME;
					mOptionsMenuSlide = 0;
					mOptionsMenuPos = 0;
					mOptionsStereo = CAREER.mStereo;
					break;

				case 3:
					mMode = FEPS_MAIN2CREDITS;
					mMainMenuSlide = SLIDE_TIME;
					mCreditsMenuSlide = 0;
					mCreditsPos = 0;
					break;

				default:
					break;
				};
				SOUNDSELECT;
			}
			if (CONTROLLER_CANCEL)
			{
				mMode = FEPS_MAIN2START;
				mMainMenuSlide = SLIDE_TIME;
				SOUNDCANCEL;
			}

			if (mAttractCounter > MAIN_TIMEOUT)
			{
				mMode = FEPS_MAIN2START;
				mMainMenuSlide = SLIDE_TIME;
			}
			
			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_VERSION)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_VERSION);
				}
			}

			break;

		case FEPS_OPTIONS2MAIN:
			mOptionsMenuSlide --;
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}
			break;

		case FEPS_MAIN2OPTIONS:
			mOptionsMenuSlide ++;
			mMainMenuSlide --;
			if (mMainMenuSlide == 0)
				mMode = FEPS_OPTIONS;
			break;

		case FEPS_CREDITS2MAIN:
			mCreditsMenuSlide --;
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}
			break;

		case FEPS_MAIN2CREDITS:
			mCreditsMenuSlide ++;
			mMainMenuSlide --;
			if (mMainMenuSlide == 0)
				mMode = FEPS_CREDITS;
			break;

		case FEPS_MCHAR2MAIN:
			mMCharMenuSlide --;
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}
			// Process Things
			{
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mMCharThings[0][c0].Process();
					mMCharThings[1][c0].Process();
				}
			}
			break;

		case FEPS_MAIN2CHAR:
			mMainMenuSlide --;
			mCharMenuSlide ++;
			if (mMainMenuSlide == 0)
			{
				mMode = FEPS_CHAR;
				mCounter = 0;
				mCharSlideStart = 0;
			}
			{
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mCharThings[c0].Process();
				}
			}
			break;
			

		case FEPS_MAIN2MCHAR:
			mMainMenuSlide --;
			mMCharMenuSlide ++;
			if (mMainMenuSlide == 0)
			{
				mMode = FEPS_MCHAR;
				mCounter = 0;
				mMCharSlideStart[0] = 0;
				mMCharSlideStart[1] = 0;
			}
			// Process Things
			{
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mMCharThings[0][c0].Process();
					mMCharThings[1][c0].Process();
				}
			}
			break;

		case FEPS_SELMEM2MAIN:
			mSelMemMenuSlide --;
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** SELMEM

		case FEPS_SELMEM:
			{
				SINT	c0, c1;
				BOOL	ok = FALSE;
				for (c0 = 0; c0 < 4; c0 ++)
				{
					for (c1 = 0; c1 < 2; c1 ++)
					{
						mSelMemVMS[c0][c1].mState 	    = MCCheck(c0 * 2 + c1);
						mSelMemVMS[c0][c1].mBlocksFree  = MCBlocksFree(c0 * 2 + c1);
						mSelMemVMS[c0][c1].mSavePresent = MCIsThereAGameSave(c0 * 2 + c1);
						
						if ((mSelMemVMS[c0][c1].mState == VMU_EXISTS_FORMATTED) &&
							((mSelMemVMS[c0][c1].mBlocksFree >= BLOCKS_NEEDED_TO_SAVE) ||
							 (mSelMemVMS[c0][c1].mSavePresent)))
							ok = TRUE;
							
/*						else // JCL HACK
						{
							mSelMemVMS[c0][c1].mState 	    = VMU_EXISTS_FORMATTED;
							mSelMemVMS[c0][c1].mBlocksFree  = 0;
							mSelMemVMS[c0][c1].mSavePresent = FALSE;
						}*/
					}
				}
						
	
				switch (mSelMemMenuState)
				{
				case EMS_SELECT_CARD:
					{
						if (!ok)
						{
							mSelMemCard	= -1;
							mSelMemPort = -1;
						}
						else
						{
							if (mSelMemCard == -1)
								mSelMemCard = 0;
							if (mSelMemPort == -1)
								mSelMemPort = 0;
								
							SINT 	dir = 0;
							
							if ((mSelMemVMS[mSelMemCard][mSelMemPort].mState != VMU_EXISTS_FORMATTED) ||
								((mSelMemVMS[mSelMemCard][mSelMemPort].mBlocksFree < BLOCKS_NEEDED_TO_SAVE) &&
								 (mSelMemVMS[mSelMemCard][mSelMemPort].mSavePresent == FALSE)))
								dir = 1;

							if (ym != G0)
							{
								if (!mDBY)
								{
									if (ym > G0)
									{
										dir = -1;
									}
									else
									{
										dir = 1;
									}
									SOUNDMOVE;
									mDBY = 6;
								}
								else
									mDBY --;
							}
							else
								mDBY = FALSE;

							if (dir != 0)
							{								
								SINT c0;
								for (c0 = 0; c0 < 8; c0 ++)
								{
									mSelMemPort += dir;
									if (mSelMemPort == 2)
									{
										mSelMemPort = 0;
										mSelMemCard ++;
									}
									if (mSelMemPort == -1)
									{
										mSelMemPort = 1;
										mSelMemCard --;
									}
									if (mSelMemCard == 4)
										mSelMemCard = 0;
									if (mSelMemCard == -1)
										mSelMemCard = 3;
										
									if ((mSelMemVMS[mSelMemCard][mSelMemPort].mState == VMU_EXISTS_FORMATTED) &&
										((mSelMemVMS[mSelMemCard][mSelMemPort].mBlocksFree >= BLOCKS_NEEDED_TO_SAVE) ||
										 (mSelMemVMS[mSelMemCard][mSelMemPort].mSavePresent)))
										break;
								}
							}						
						}

						if (CONTROLLER_SELECT)
						{
							if (mSelMemCard == -1)
							{
								CAREER.mMemController = -1;
								CAREER.mMemSlot = -1;
								
								switch (mSelMemWhereFrom)
								{
								case FEPS_START:
									mMode = FEPS_SELMEM2MAIN;
									mMainMenuSlide = 0;
									mMainMenuFromSelMem = TRUE;
									break;
								
								case FEPS_OPTIONS:
									mMode = FEPS_SELMEM2OPTIONS;
									mOptionsMenuSlide = 0;
									break;
									
								case FEPS_QSAVE:
									mMode = FEPS_SELMEM2ARENA;
									mArenaMenuSlide = 0;
									break;
								};
								SOUNDSELECT;
								break;
							}
							else
							{
								SOUNDSELECT;
							
								SINT	what_to_do = 0;
								
								switch (mSelMemWhereFrom)
								{
								case FEPS_START:
									if (mSelMemVMS[mSelMemCard][mSelMemPort].mSavePresent)
										what_to_do = 0;
									else
										what_to_do = 1;
									break;
								
								case FEPS_OPTIONS:
									if (mSelMemVMS[mSelMemCard][mSelMemPort].mSavePresent)
										what_to_do = 2;
									else
										what_to_do = 1;
									break;
									
								case FEPS_QSAVE:
									if (mSelMemVMS[mSelMemCard][mSelMemPort].mSavePresent)
										what_to_do = 3;
									else
										what_to_do = 1;
									break;
								}
								
								switch (what_to_do)
								{
								case 0:
									{
										// load
										SDSDisableSoftReset();
										SINT	rval = MCLoad(mSelMemCard * 2 + mSelMemPort);
										
										if (rval == BUD_ERR_OK)
											mSelMemMenuState = EMS_LOADING;
										else
										{
											mSelMemMenuState = EMS_LOAD_FAILED;
											mSelMemFailedPos = 0;
											SDSEnableSoftReset();
										}
									}
									break;
								case 1:
									{
										// save	
										SDSDisableSoftReset();
										SINT	rval = MCSave(mSelMemCard * 2 + mSelMemPort);
										
										if (rval == BUD_ERR_OK)
											mSelMemMenuState = EMS_SAVING;
										else
										{
											mSelMemMenuState = EMS_SAVE_FAILED;
											mSelMemFailedPos = 0;
											SDSEnableSoftReset();
										}
									}
									break;
								case 2:
									{
										// Save/load
										mSelMemMenuState = EMS_SAVE_LOAD;
										mSelMemSLPos = 0;
									}
									break;
								case 3:
									{
										// Save y/n
										mSelMemMenuState = EMS_SAVE_YN;
										mSelMemYNPos = 1;
									}
									break;
								};
							}
						}
			
						if ((mSelMemWhereFrom == FEPS_OPTIONS) && (CONTROLLER_CANCEL))
						{
							mMode = FEPS_SELMEM2OPTIONS;
							mSelMemMenuSlide = SLIDE_TIME;
							mOptionsMenuSlide = 0;
							SOUNDCANCEL;
							break;
						}

					}
					break;

				case EMS_SAVING:
					{
						SINT r = buStat(mSelMemCard * 2 + mSelMemPort);
						
						if (r == BUD_STAT_READY)
						{
							SDSEnableSoftReset();
							
							// did it fail?
							SINT	rval = buGetLastError(mSelMemCard * 2 + mSelMemPort);
							if (rval != BUD_ERR_OK)
							{
								mSelMemMenuState = EMS_SAVE_FAILED;
								mSelMemFailedPos = 0;
							}
							else
							{
								// done
								CAREER.mMemController = mSelMemCard;
								CAREER.mMemSlot = mSelMemPort;
								switch (mSelMemWhereFrom)
								{
								case FEPS_START:
									mMode = FEPS_SELMEM2MAIN;
									mMainMenuSlide = 0;
									mMainMenuFromSelMem = TRUE;
									break;
								
								case FEPS_OPTIONS:
									mMode = FEPS_SELMEM2OPTIONS;
									mOptionsMenuSlide = 0;
									break;
									
								case FEPS_QSAVE:
									mMode = FEPS_SELMEM2ARENA;
									mArenaMenuSlide = 0;
									break;
								};
							}
						}					
					}
					break;

				case EMS_LOADING:
					{
						SINT r = buStat(mSelMemCard * 2 + mSelMemPort);
						
						if (r == BUD_STAT_READY)
						{
							r = MCLoadFinish();
							
							SDSEnableSoftReset();
							
							if (r != BUD_ERR_OK)
							{
								mSelMemMenuState = EMS_LOAD_FAILED;
								mSelMemFailedPos = 0;
							}
							else
							{
								// done
								CAREER.mMemController = mSelMemCard;
								CAREER.mMemSlot = mSelMemPort;
								switch (mSelMemWhereFrom)
								{
								case FEPS_START:
									mMode = FEPS_SELMEM2MAIN;
									mMainMenuSlide = 0;
									mMainMenuFromSelMem = TRUE;
									break;
								
								case FEPS_OPTIONS:
									mMode = FEPS_SELMEM2OPTIONS;
									mOptionsMenuSlide = 0;
									break;
									
								case FEPS_QSAVE:
									mMode = FEPS_SELMEM2ARENA;
									mArenaMenuSlide = 0;
									break;
								};
							}
						}					
					}
					break;
					
				case	EMS_SAVE_FAILED:
				case	EMS_LOAD_FAILED:
					{
						if (ym != G0)
							{
								if (!mDBY)
								{
									mSelMemFailedPos = (mSelMemFailedPos == 1) ? 0 : 1;
								
									SOUNDMOVE;
									mDBY = 6;
								}
								else
									mDBY --;
							}
							else
								mDBY = FALSE;
					
						if (CONTROLLER_SELECT)
						{
							if (mSelMemFailedPos == 1)
							{
								// aborted
								CAREER.mMemController = -1;
								CAREER.mMemSlot = -1;
								switch (mSelMemWhereFrom)
								{
								case FEPS_START:
									mMode = FEPS_SELMEM2MAIN;
									mMainMenuSlide = 0;
									mMainMenuFromSelMem = TRUE;
									break;
								
								case FEPS_OPTIONS:
									mMode = FEPS_SELMEM2OPTIONS;
									mOptionsMenuSlide = 0;
									break;
									
								case FEPS_QSAVE:
									mMode = FEPS_SELMEM2ARENA;
									mArenaMenuSlide = 0;
									break;
								};
								SOUNDSELECT;
							}
							else
							{
								SOUNDCANCEL;
								mSelMemMenuState = EMS_SELECT_CARD;
							}
						}
					}
					break;
					
				case EMS_SAVE_LOAD:
					{
						if (ym != G0)
						{
							if (!mDBY)
							{
								mSelMemSLPos = (mSelMemSLPos == 1) ? 0 : 1;
							
								SOUNDMOVE;
								mDBY = 6;
							}
							else
								mDBY --;
						}
						else
							mDBY = FALSE;
					
					
						if (CONTROLLER_SELECT)
						{
							SOUNDSELECT;
							
							if (mSelMemSLPos == 0)
							{
								// Save y/n
								mSelMemMenuState = EMS_SAVE_YN;
								mSelMemYNPos = 1;
							}
							else
							{
								// Load y/n
								mSelMemMenuState = EMS_LOAD_YN;
								mSelMemYNPos = 1;
							}
						}					
						if (CONTROLLER_CANCEL)
						{
							SOUNDCANCEL;
							mSelMemMenuState = EMS_SELECT_CARD;
						}
					}
					break;	
					
				case EMS_LOAD_YN:
					{
						if (ym != G0)
						{
							if (!mDBY)
							{
								mSelMemYNPos = (mSelMemYNPos == 1) ? 0 : 1;
							
								SOUNDMOVE;
								mDBY = 6;
							}
							else
								mDBY --;
						}
						else
							mDBY = FALSE;
					
					
						if (CONTROLLER_SELECT)
						{
							
							if (mSelMemYNPos == 0)
							{
								// Load
								SOUNDSELECT;
								SDSDisableSoftReset();
								SINT	rval = MCLoad(mSelMemCard * 2 + mSelMemPort);
								
								if (rval == BUD_ERR_OK)
									mSelMemMenuState = EMS_LOADING;
								else
								{
									mSelMemMenuState = EMS_LOAD_FAILED;
									mSelMemFailedPos = 0;
									SDSEnableSoftReset();
								}
							}
							else
							{
								// Back
								mSelMemMenuState = EMS_SAVE_LOAD;
								SOUNDCANCEL;
							}
						}					
						if (CONTROLLER_CANCEL)
						{
							SOUNDCANCEL;
							mSelMemMenuState = EMS_SAVE_LOAD;
						}
					}
					break;	
					
				case EMS_SAVE_YN:
					{
						if (ym != G0)
						{
							if (!mDBY)
							{
								mSelMemYNPos = (mSelMemYNPos == 1) ? 0 : 1;
							
								SOUNDMOVE;
								mDBY = 6;
							}
							else
								mDBY --;
						}
						else
							mDBY = FALSE;
					
					
						if (CONTROLLER_SELECT)
						{
							
							if (mSelMemYNPos == 0)
							{
								// Save
								SOUNDSELECT;
								SDSDisableSoftReset();
								SINT	rval = MCSave(mSelMemCard * 2 + mSelMemPort);
								
								if (rval == BUD_ERR_OK)
									mSelMemMenuState = EMS_SAVING;
								else
								{
									mSelMemMenuState = EMS_SAVE_FAILED;
									mSelMemFailedPos = 0;
									SDSEnableSoftReset();
								}
							}
							else
							{
								// Back
								if (mSelMemWhereFrom == FEPS_OPTIONS)
									mSelMemMenuState = EMS_SAVE_LOAD;
								else
									mSelMemMenuState = EMS_SELECT_CARD;
								SOUNDCANCEL;
							}
						}					
						if (CONTROLLER_CANCEL)
						{
							SOUNDCANCEL;
							if (mSelMemWhereFrom == FEPS_OPTIONS)
								mSelMemMenuState = EMS_SAVE_LOAD;
							else
								mSelMemMenuState = EMS_SELECT_CARD;
						}
					}
					break;	
			
				};
		

			}
			break;
			
		case FEPS_SELMEM2ARENA:
			mSelMemMenuSlide --;
			mArenaMenuSlide ++;
			if (mSelMemMenuSlide == 0)
			{
				mMode = FEPS_ARENA;
				mCounter = 0;
				mArenaSlideStart = 0;
			}
			break;
			
		case FEPS_QSAVE2SELMEM:
			mSelMemMenuSlide ++;
			mQSaveMenuSlide --;
			mArenaMenuSlide --;
			if (mQSaveMenuSlide == 0)
			{
				mMode = FEPS_SELMEM;
				mCounter = 0;
				mSelMemMenuState = EMS_SAVE_FAILED;
				mSelMemFailedPos = 0;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** CHAR

		case FEPS_CHAR:
			if (mCharWait != -1)
			{
				// Process Character Things
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mCharThings[c0].Process();
				}

				// slide character
				mCharSlide = mCharSlide * G(0, 50000);
				if (GABS(mCharSlide) < G(0, 50))
					mCharSlide = G0;

				mCharWait ++;
				if (mCharWait == 25)
				{
					mMode = FEPS_CHAR2PSNEW;
					mCharMenuSlide = SLIDE_TIME;
					mCharSlide = G0;
					mPrisonMenuSlide = 0;
					mPrisonBonus = FALSE;
					mPrisonBonusSlide = G0;
					mPrison = 0;
					mCounter = 0;
					mCharWait = -1;
				}
			}
			else
			{
				if (xm != G0)
				{
					if (!mDBX)
					{
						if (xm > G0)
						{
							mCharacter ++;
							mCharSlide -= G1;
					 		mCharSlideStart = mCounter;
							if (!can_select_doctor())
							{
								if (mCharacter == 5)
							 	{
							 		mCharacter = 0;
							 		mCharSlide -= G1;
							 	}
							}
						}
						else
						{
							mCharacter --;
							mCharSlide += G1;
					 		mCharSlideStart = mCounter;
							if (!can_select_doctor())
							{
							 	if (mCharacter == -1)
							 	{
							 		mCharacter = 4;
							 		mCharSlide += G1;
							 		mCharSlideStart = mCounter;
							 	}
							}
						}
						SOUNDMOVE;

						mDBX = 6;
					}
					else
						mDBX --;
				}
				else
					mDBX = FALSE;

				if (mCharacter < 0)
					mCharacter = 5;

				if (mCharacter > 5)
					mCharacter = 0;

				// slide character
				mCharSlide = mCharSlide * G(0, 50000);
				if (GABS(mCharSlide) < G(0, 50))
					mCharSlide = G0;

				// Process Character Things
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mCharThings[c0].Process();
				}

				// buttons
				if (CONTROLLER_CANCEL)
				{
					mMode = FEPS_CHAR2MAIN;
					mMainMenuSlide = 0;
					mCharMenuSlide = SLIDE_TIME;
					mCharSlide = G0;
					SOUNDCANCEL;
				}
				if (CONTROLLER_SELECT)
				{
					mCharThings[mCharacter].Select();
					mCharWait = 0;
					SOUNDSELECT;
				}
			}
			
			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_DOCTOR)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_DOCTOR);
				}
			}

			break;

		case FEPS_CHAR2PSNEW:
			mCharMenuSlide --;
			mPrisonMenuSlide ++;
			if (mCharMenuSlide == 0)
			{
				mMode = FEPS_PSNEW;
				mCounter = 0;
				mPrisonSlideStart = 0;
				mPrisonAccessStart = 0;
			}
			{
				// Process Character Things
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mCharThings[c0].Process();
				}
			}
		break;

		case FEPS_PSNEW2CHAR:
			mCharMenuSlide ++;
			mPrisonMenuSlide --;
			if (mCharMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_CHAR;
				mCounter = 0;
				mCharSlideStart = 0;
			}
			{
				// Process Character Things
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mCharThings[c0].Process();
				}
			}
			break;

		case FEPS_CHAR2MAIN:
			mCharMenuSlide --;
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}

			{
				// Process Character Things
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mCharThings[c0].Process();
				}
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** PSNEW

		case FEPS_PSNEW:
			if (mPrisonBonusSlide == G0)
			{
				if (xm != G0)
				{
					if (!mDBX)
					{
						if (xm > G0)
						{
							if (mPrison < 9)
							{
								mPrison ++;
								BOOL	foo = can_select_prison(mPrison, mCharacter);
								if (foo != mPrisonAccessible)
									mPrisonAccessStart = mCounter;
								mPrisonAccessible = foo;
								mPrisonSlide -= G1;
						 		mPrisonSlideStart = mCounter;
						 	}
						}
						else
						{
							if (mPrison > 0)
							{
								mPrison --;
								BOOL	foo = can_select_prison(mPrison, mCharacter);
								if (foo != mPrisonAccessible)
									mPrisonAccessStart = mCounter;
								mPrisonAccessible = foo;
								mPrisonSlide += G1;
						 		mPrisonSlideStart = mCounter;
						 	}
						}

						mDBX = 6;
						SOUNDMOVE;
					}
					else
						mDBX --;
				}
				else
					mDBX = FALSE;
			}

			// slide prison
			mPrisonSlide = mPrisonSlide * G(0, 50000);
			if (GABS(mPrisonSlide) < G(0, 50))
				mPrisonSlide = G0;

			if (mPrisonBonusSlide == G0)
			{
				if (can_select_bonus_prison(mCharacter))
				{
					if (ym > 0)
					{
						mPrisonBonusSlide = G(0, 100);
						mPrisonBonus = TRUE;
				 		mPrisonSlideStart = mCounter;
				 		SOUNDMOVE;

					}
				}
			}

			if (mPrisonBonusSlide == G1)
			{
				if (ym < 0)
				{
					mPrisonBonusSlide = G(0, 65500);
					mPrisonBonus = FALSE;
			 		mPrisonSlideStart = mCounter;
			 		SOUNDMOVE;
				}
			}
			if ((mPrisonBonusSlide != G0) && (mPrisonBonusSlide != G1))
			{
				if (mPrisonBonus)
				{
					mPrisonBonusSlide += G(0, 10000);
					if (G1 - mPrisonBonusSlide < G(0, 1000))
						mPrisonBonusSlide = G1;
				}
				else
				{
					mPrisonBonusSlide -= G(0, 10000);
					if (mPrisonBonusSlide < G(0, 1000))
						mPrisonBonusSlide = G0;
				}
			}
			else
			{
				if (CONTROLLER_CANCEL)
				{
					mMode = FEPS_PSNEW2CHAR;
					mCharMenuSlide = 0;
					mPrisonSlide = G0;
					mCharRotTimer = 0;
					mCharWait = -1;
					SINT	c0;
					for (c0 = 0; c0 < 6; c0 ++)
					{
						mCharThings[c0].Reset(c0);
					}
					SOUNDCANCEL;

				}
				if (CONTROLLER_SELECT)
				{
					if (mPrisonBonus || can_select_prison(mPrison, mCharacter))
					{
						mMode = FEPS_PSNEW2ARENA;
						mArenaMenuSlide = 0;
						mArena = 0;
						mPrisonSlide = G0;
					}
					SOUNDSELECT;
				}
			}
			//CHEAT
			if (CONTROLS.IsButtonPressedDB(BUTTON_ADVANCE_LEVEL))
			{
				CAREER.mCharCareers[mCharacter].mNumLevelsCompleted = 100;
				CAREER.mCharCareers[mCharacter].mNumGoldStars = 100;
				CAREER.mCharCareers[mCharacter].mNumLostToysTimes = 100;
				SOUNDCHEAT;
			}

			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_LEVELS)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_LEVELS);
				}
			}
			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_BONUS)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_BONUS);
				}
			}

			break;

		case FEPS_ARENA2PSNEW:
			mArenaMenuSlide --;
			mPrisonMenuSlide ++;
			if (mArenaMenuSlide == 0)
			{
				mMode = FEPS_PSNEW;
				mCounter = 0;
				mPrisonSlideStart = 0;
				mPrisonAccessStart = 0;
			}
			break;

		case FEPS_PSNEW2ARENA:
			mArenaMenuSlide ++;
			mPrisonMenuSlide --;
			if (mArenaMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_ARENA;
				mCounter = 0;
				mArenaSlideStart = 0;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** ARENA

		case FEPS_ARENA:
			{
				SINT	prison;

				if (mPrisonBonus)
					prison = 10;
				else
					prison = mPrison;

				if (mArenaPendingVictory != -1)
				{
					mArenaPendingVictory --;

					if (mArenaPendingVictory == -1)
					{
						if (mArenaPendingVictoryType)
						{
							CCharCareer		*cc = &CAREER.mCharCareers[mCharacter];
							CLevelCareer	*lc = &CAREER.mCharCareers[mCharacter].mLevelCareers[prison][mArena];

							if (mArenaPendingVictoryType & 1)
							{
								// win level
								lc->mFinished = ECL_YES;
								if (!mPrisonBonus)
									cc->mNumLevelsCompleted ++;
								mArenaPendingVictoryType &= (~1);
								AddParticles(287, 182, NUM_PSXFE_PARTICLES / 2);
								mArenaPendingVictory = 8;
							}
							else if (mArenaPendingVictoryType & 2)
							{
								lc->mFinished = ECL_GOLD;
								if (!mPrisonBonus)
									cc->mNumGoldStars ++;
								mArenaPendingVictoryType &= (~2);
								AddParticles(225, 182, NUM_PSXFE_PARTICLES / 2);
								mArenaPendingVictory = 8;
							}
							else if (mArenaPendingVictoryType & 4)
							{
								lc->mFinished = ECL_LT;
								if (!mPrisonBonus)
									cc->mNumLostToysTimes ++;
								mArenaPendingVictoryType &= (~4);
								AddParticles(256 - 8, 182 - 8, NUM_PSXFE_PARTICLES / 2);
								mArenaPendingVictory = 8;
							}
							else if (mArenaPendingVictoryType & 8)
							{
								// Messages
								if (mArenaCurrentMessage < mArenaNumMessages)
								{
									mMode = FEPS_ARENA2MESSAGE;
									mMessageMenuSlide = 0;
									mMessage = mArenaMessages[mArenaCurrentMessage ++];
									mArenaPendingVictory = 0; // no delay
								}
								else
								{
									mArenaPendingVictoryType &= (~8);
									mArenaPendingVictory = 0; // no delay
								}
							}
						}
						else
						{
							if ((CAREER.mMemController >= 0) && (CAREER.mMemSlot >= 0))
							{
								mMode = FEPS_ARENA2QSAVE;
								mQSaveMenuSlide = 0;
								mQSaveMenuPos = 1;
							}
						}
					}
				}
				else
				{
					SINT	na = CLevelStructure::GetNumLevelsInPrison(prison);
					if (xm != G0)
					{
						if (!mDBX)
						{
							if (xm > G0)
							{
								mArena = (mArena + 1) % na;

								SINT	pr = mPrisonBonus ? 10 : mPrison;
								while (!(can_select_arena(pr, mArena, mCharacter)))
									mArena = (mArena + 1) % na;

						 		mArenaSlideStart = mCounter;
						 		mArrowCounter = 0;
							}
							else
							{
								mArena --;
								if (mArena < 0)
									mArena += na;
								SINT	pr = mPrisonBonus ? 10 : mPrison;
								while (!(can_select_arena(pr, mArena, mCharacter)))
								{
									mArena --;
									if (mArena < 0)
										mArena += na;
								}

						 		mArenaSlideStart = mCounter;
						 		mArrowCounter = 0;
							}

							mDBX = 6;
							SOUNDMOVE;
						}
						else
							mDBX --;
					}
					else
						mDBX = FALSE;

#ifdef	PRESS_PREVIEW
					while (!(can_select_arena(mPrison, mArena, mCharacter)))
					{
						mArena ++;
					}
#endif


//					if (mArena >= na)
//						mArena = 0;
//					if (mArena < 0)
//						mArena = na - 1;

					if (CONTROLLER_CANCEL)
					{
						mMode = FEPS_ARENA2PSNEW;
						mPrisonMenuSlide = 0;
		//				mPrisonSlide = G0;
						SOUNDCANCEL;
					}
					if (CONTROLLER_SELECT)
					{
						mCounter = 0;
						mMode = FEPS_PS2GAME;
						SOUNDSELECT;
						SOUNDSTART;
					}

					//! CHEAT
					if (CONTROLS.IsButtonPressedDB(BUTTON_ADVANCE_LEVEL))
					{
						// activate this level
						CCharCareer		*cc = &CAREER.mCharCareers[mCharacter];
						CLevelCareer	*lc = &CAREER.mCharCareers[mCharacter].mLevelCareers[prison][mArena];

						if (lc->mFinished == ECL_NO)
						{
							lc->mFinished = ECL_YES;
							lc->mBestTime = 2 * 60 * 25;
							lc->AddAttempt();
							cc->mNumLevelsCompleted ++;

							AddParticles(287, 182, NUM_PSXFE_PARTICLES / 2);
						}
						else if (lc->mFinished == ECL_YES)
						{
							lc->mFinished = ECL_GOLD;
							lc->mBestTime = 1 * 60 * 25;
							lc->AddAttempt();
							cc->mNumGoldStars ++;

							AddParticles(225, 182, NUM_PSXFE_PARTICLES / 2);
						}
						else if (lc->mFinished == ECL_GOLD)
						{
							lc->mFinished = ECL_LT;
							lc->mBestTime =  30 * 25;
							lc->AddAttempt();
							cc->mNumLostToysTimes ++;

							AddParticles(256, 182, NUM_PSXFE_PARTICLES / 2);
						}
						else
							lc->AddAttempt(); // helps debug

						SOUNDCHEAT;
					}

/*					// debug - save
					if (CONTROLS.IsButtonPressedDB(BUTTON_DC_X))
					{
						mMode = FEPS_ARENA2QSAVE;
						mQSaveMenuSlide = 0;
						mQSaveMenuPos = 1;
						SOUNDSELECT;
					};*/
				}
			}
/*			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_BOUNCY)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_BOUNCY);
				}
			}
			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_SLIPPERY)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_SLIPPERY);
				}
			}*/
			break;

		case FEPS_PS2GAME:
			if (mCounter == PS2GAME_COUNT)
				mQuit = EPSXFE_SINGLE;
			break;

		case FEPS_TS2GAME:
			if (mCounter == PS2GAME_COUNT)
				mQuit = EPSXFE_MULTI;
			break;

		case FEPS_QSAVE2ARENA:
			mQSaveMenuSlide --;
			mArenaMenuSlide ++;
			if (mArenaMenuSlide == SLIDE_TIME)
				mMode = FEPS_ARENA;
			break;

		case FEPS_ARENA2QSAVE:
			mQSaveMenuSlide ++;
			mArenaMenuSlide --;
			if (mArenaMenuSlide == 0)
			{
				mMode = FEPS_QSAVE;
				mQSaveCounter = 0;
			}
			break;

		case FEPS_MESSAGE2ARENA:
			mMessageMenuSlide --;
			mArenaMenuSlide ++;
			if (mArenaMenuSlide == SLIDE_TIME)
				mMode = FEPS_ARENA;
			break;

		case FEPS_ARENA2MESSAGE:
			mMessageMenuSlide ++;
			mArenaMenuSlide --;
			if (mArenaMenuSlide == 0)
				mMode = FEPS_MESSAGE;
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** QSAVE

		case FEPS_QSAVE:
			{
				SDSDisableSoftReset();
				
				SINT	save_error = 0;
				mQSaveCounter ++;
				
				VMUState		state = MCCheck(CAREER.mMemController * 2 + CAREER.mMemSlot);
				SINT			bfree = MCBlocksFree(CAREER.mMemController * 2 + CAREER.mMemSlot);
				BOOL			savep = MCIsThereAGameSave(CAREER.mMemController * 2 + CAREER.mMemSlot);
				
				if (mQSaveCounter == 3)
					if (state != VMU_EXISTS_FORMATTED)
						save_error = 1;
				
				if (save_error == 0)
				{
					if (mQSaveCounter == 5)
					{
						// initiate save
						SDSDisableSoftReset();
						SINT	rval = MCSave(CAREER.mMemController * 2 + CAREER.mMemSlot);
						
						if (rval != BUD_ERR_OK)
							save_error = 1;
					}
					else if (mQSaveCounter > 5)
					{
						// has save finished
						SINT r = buStat(CAREER.mMemController * 2 + CAREER.mMemSlot);
						
						if (r == BUD_STAT_READY)
						{
							SDSEnableSoftReset();
							
							SINT	rval = buGetLastError(mSelMemCard * 2 + mSelMemPort);
							if (rval != BUD_ERR_OK)
							{
								save_error = 1;
							}
							else						
								mMode = FEPS_QSAVE2ARENA;
						}
						
					}
				}
				
				if (save_error != 0)
				{
					mMode = FEPS_QSAVE2SELMEM;
					mSelMemMenuSlide = 0;
					mSelMemWhereFrom = FEPS_QSAVE;
					mSelMemCard = CAREER.mMemController;
					mSelMemPort = CAREER.mMemSlot;
					SDSEnableSoftReset();
				}		
			}	
			break;


		// ****************************************************************************
		// ****************************************************************************
		// ** MESSAGE

		case FEPS_MESSAGE:
			if ((CONTROLLER_SELECT) ||
			    (CONTROLLER_CANCEL))
			{
				mMode = FEPS_MESSAGE2ARENA;
				mMessageMenuSlide = SLIDE_TIME;
				mArenaMenuSlide = 0;
				SOUNDSELECT;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** MCHAR

		case FEPS_MCHAR:
			// PLAYER 1

			if ((!mMCharSelected[0]) && (xm != G0))
			{
				if (!mDBX)
				{
					if (xm > G0)
					{
						mMCharacter[0] ++;
						mMCharSlide[0] -= G1;
				 		if (mMCharacter[0] == 6)
				 			mMCharacter[0] = 0;
				 		mMCharSlideStart[0] = mCounter;
						if ((!can_select_doctor()) && (mMCharacter[0] == 5))
					 	{
					 		mMCharacter[0] = 0;
					 		mMCharSlide[0] -= G1;
					 	}
					 	if (mMCharacter[0] == mMCharacter[1])
					 	{
					 		mMCharacter[0] ++;
					 		mMCharSlide[0] -= G1;
					 		if (mMCharacter[0] == 6)
					 			mMCharacter[0] = 0;
					 	}
						if ((!can_select_doctor()) && (mMCharacter[0] == 5)) // again!
					 	{
					 		mMCharacter[0] = 0;
					 		mMCharSlide[0] -= G1;
					 	}
					}
					else
					{
						mMCharacter[0] --;
						mMCharSlide[0] += G1;
				 		if (mMCharacter[0] == -1)
				 			mMCharacter[0] = 5;
				 		mMCharSlideStart[0] = mCounter;
						if ((!can_select_doctor()) && (mMCharacter[0] == 5))
					 	{
					 		mMCharacter[0] = 4;
					 		mMCharSlide[0] += G1;
					 	}
					 	if (mMCharacter[0] == mMCharacter[1])
					 	{
					 		mMCharacter[0] --;
					 		mMCharSlide[0] += G1;
					 		if (mMCharacter[0] == -1)
					 			mMCharacter[0] = 5;
					 	}
						if ((!can_select_doctor()) && (mMCharacter[0] == 5)) // again!
					 	{
					 		mMCharacter[0] = 4;
					 		mMCharSlide[0] += G1;
					 	}
					}

					mDBX = 6;
					SOUNDMOVE;
				}
				else
					mDBX --;
			}
			else
				mDBX = FALSE;

			if (mMCharacter[0] < 0)
				mMCharacter[0] = 5;

			if (mMCharacter[0] > 5)
				mMCharacter[0] = 0;

			// PLAYER 1
			if ((!mMCharSelected[1]) && (xm2 != G0))
			{
				if (!mDBX2)
				{
					if (xm2 > G0)
					{
						mMCharacter[1] ++;
						mMCharSlide[1] -= G1;
				 		if (mMCharacter[1] == 6)
				 			mMCharacter[1] = 0;
				 		mMCharSlideStart[1] = mCounter;
						if ((!can_select_doctor()) && (mMCharacter[1] == 5))
					 	{
					 		mMCharacter[1] = 0;
					 		mMCharSlide[1] -= G1;
					 	}
					 	if (mMCharacter[1] == mMCharacter[0])
					 	{
					 		mMCharacter[1] ++;
					 		mMCharSlide[1] -= G1;
					 		if (mMCharacter[1] == 6)
					 			mMCharacter[1] = 0;
					 	}
						if ((!can_select_doctor()) && (mMCharacter[1] == 5)) // again!
					 	{
					 		mMCharacter[1] = 0;
					 		mMCharSlide[1] -= G1;
					 	}
					}
					else
					{
						mMCharacter[1] --;
						mMCharSlide[1] += G1;
				 		mMCharSlideStart[1] = mCounter;
				 		if (mMCharacter[1] == -1)
				 			mMCharacter[1] = 5;
						if ((!can_select_doctor()) && (mMCharacter[1] == 5))
					 	{
					 		mMCharacter[1] = 4;
					 		mMCharSlide[1] += G1;
					 	}
					 	if (mMCharacter[1] == mMCharacter[0])
					 	{
					 		mMCharacter[1] --;
					 		mMCharSlide[1] += G1;
					 		if (mMCharacter[1] == -1)
					 			mMCharacter[1] = 5;
					 	}
						if ((!can_select_doctor()) && (mMCharacter[1] == 5)) // again!
					 	{
					 		mMCharacter[1] = 4;
					 		mMCharSlide[1] += G1;
					 	}
					}

					mDBX2 = 6;
					SOUNDMOVE;
				}
				else
					mDBX2 --;
			}
			else
				mDBX2 = FALSE;

			if (mMCharacter[0] < 0)
				mMCharacter[0] = 5;

			if (mMCharacter[1] > 5)
				mMCharacter[1] = 0;

			// slide characters
			mMCharSlide[0] = mMCharSlide[0] * G(0, 50000);
			if (GABS(mMCharSlide[0]) < G(0, 50))
				mMCharSlide[0] = G0;

			mMCharSlide[1] = mMCharSlide[1] * G(0, 50000);
			if (GABS(mMCharSlide[1]) < G(0, 50))
				mMCharSlide[1] = G0;

			// buttons
			BOOL	goback 	= FALSE;
			BOOL	goon	= FALSE;

			if (mMCharSelected[0])
			{
				if (CONTROLS.IsButtonPressedDB(BUTTON_DC_B, 0))
				{
					mMCharSelected[0] = FALSE;
					SOUNDCANCEL;
				}

				if (CONTROLS.IsButtonPressedDB(BUTTON_DC_A, 0))
					if (mMCharSelected[1])
					{
						goon = TRUE;
						SOUNDSELECT;
					}
			}
			else
			{
				if (CONTROLS.IsButtonPressedDB(BUTTON_DC_B, 0))
				{
					goback = TRUE;
					SOUNDCANCEL;
				}

				if (CONTROLS.IsButtonPressedDB(BUTTON_DC_A, 0))
				{
					mMCharSelected[0] = TRUE;
					mMCharThings[0][mMCharacter[0]].Select();
					SOUNDSELECT;
				}

			}

			if (mMCharSelected[1])
			{
				if (CONTROLS.IsButtonPressedDB(BUTTON_DC_B, 1))
				{
					mMCharSelected[1] = FALSE;
					SOUNDCANCEL;
				}
			}
			else
			{
				if (CONTROLS.IsButtonPressedDB(BUTTON_DC_B, 1))
				{
					goback = TRUE;
					SOUNDCANCEL;
				}

				if (CONTROLS.IsButtonPressedDB(BUTTON_DC_A, 1))
				{
					mMCharSelected[1] = TRUE;
					mMCharThings[1][mMCharacter[1]].Select();
					SOUNDSELECT;
				}
			}

			if (goback)
			{
				mMode = FEPS_MCHAR2MAIN;
				mMainMenuSlide = 0;
				mMCharMenuSlide = SLIDE_TIME;
				mMCharSlide[0] = G0;
				mMCharSlide[1] = G0;
			}

			if (goon)
			{
				mMode = FEPS_MCHAR2MTOURN;;
				mMCharMenuSlide = SLIDE_TIME;
				mMCharSlide[0] = 0;
				mMCharSlide[1] = 0;
				mMTournamentMenuSlide = 0;
				MULTIPLAYER_CAREER.Clear();
				mMTournamentResultsTimer = -1;
				mMTournamentResultsShowLW = 0;
				mMTournamentResultsShowWinner = FALSE;
			}
			// Process Things
			{
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mMCharThings[0][c0].Process();
					mMCharThings[1][c0].Process();
				}
			}
			
			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_DOCTOR)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_DOCTOR);
				}
			}

			break;

		case FEPS_MCHAR2MTOURN:
			mMCharMenuSlide --;
			mMTournamentMenuSlide ++;
			if (mMTournamentMenuSlide == SLIDE_TIME)
				mMode = FEPS_MTOURN;
			// Process Things
			{
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mMCharThings[0][c0].Process();
					mMCharThings[1][c0].Process();
				}
			}

			break;

		case FEPS_MTOURN2MCHAR:
			mMTournamentMenuSlide --;
			mMCharMenuSlide ++;
			if (mMTournamentMenuSlide == 0)
			{
				mMode = FEPS_MCHAR;
				mCounter = 0;
				mMCharSlideStart[0] = 0;
				mMCharSlideStart[1] = 0;
			}
			// Process Things
			{
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mMCharThings[0][c0].Process();
					mMCharThings[1][c0].Process();
				}
			}
			break;


		// ****************************************************************************
		// ****************************************************************************
		// ** MTOURN

		case FEPS_MTOURN:
			// are we displaying results?
			if (mMTournamentResultsTimer >= 0)
			{
				STournamentInfo	*ti = CLevelStructure::GetTournamentInfo(mMTournament);

				mMTournamentResultsTimer ++;
				if (mMTournamentResultsTimer == 20)
				{
					mMTournamentResultsTimer = 0;
					if (mMTournamentResultsShowLW < ti->mNumLevels)
					{
						mMTournamentResultsShowLW ++;
					}
					else if (!mMTournamentResultsShowWinner)
					{
						mMTournamentResultsShowWinner = TRUE;
					}
					else
						mMTournamentResultsTimer = -2; // enable control!
				}
			}
			else if (mMTournamentResultsTimer == -2)
			{
				// Limited control - press X to resore
				if (CONTROLLER_SELECT)
				{
					MULTIPLAYER_CAREER.Clear();
					mMTournamentResultsTimer = -1;
					mMTournamentResultsShowLW = 0;
					mMTournamentResultsShowWinner = FALSE;
				}
			}
			else
			{
				if (xm != G0)
				{
					if (!mDBX)
					{
						if (xm > G0)
						{
							if (mMTournament < CLevelStructure::GetNumMultiplayerTournaments() - 1)
							{
								mMTournament ++;
								BOOL	foo = TRUE; //can_select_prison(mPrison, mCharacter);
								if (foo != mMTournamentAccessible)
									mMTournamentAccessStart = mCounter;
								mMTournamentAccessible = foo;
								mMTournamentSlide -= G1;
						 		mMTournamentSlideStart = mCounter;
						 	}
						}
						else
						{
							if (mMTournament > 0)
							{
								mMTournament --;
								BOOL	foo = TRUE; //can_select_prison(mPrison, mCharacter);
								if (foo != mMTournamentAccessible)
									mMTournamentAccessStart = mCounter;
								mMTournamentAccessible = foo;
								mMTournamentSlide += G1;
						 		mMTournamentSlideStart = mCounter;
						 	}
						}

						mDBX = 6;
						SOUNDMOVE;
					}
					else
						mDBX --;
				}
				else
					mDBX = FALSE;

				// slide prison
				mMTournamentSlide = mMTournamentSlide * G(0, 50000);
				if (GABS(mMTournamentSlide) < G(0, 50))
					mMTournamentSlide = G0;

				if (CONTROLLER_CANCEL)
				{
					mMode = FEPS_MTOURN2MCHAR;
					mMTournamentSlide = G0;
					mMCharMenuSlide = 0;
					mMCharRotTimer = 0;
					mMCharSelected[0] = FALSE;
					mMCharSelected[1] = FALSE;
					mMCharWait = -1;
					{
						SINT	c0;
						for (c0 = 0; c0 < 6; c0 ++)
						{
							mMCharThings[0][c0].Reset(c0);
							mMCharThings[1][c0].Reset(c0);
						}
					}
					SOUNDCANCEL;
				}
				if (CONTROLLER_SELECT)
				{
					mCounter = 0;
					mMode = FEPS_TS2GAME;
					SOUNDSELECT;
					SOUNDSTART;
				}
			}
/*			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_BOUNCY)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_BOUNCY);
				}
			}
			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_SLIPPERY)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_SLIPPERY);
				}
			}
			{
				SINT cheat = CONTROLS.mControllers[1].GetCheat();
				if (cheat == CHEAT_BOUNCY)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_BOUNCY);
				}
			}
			{
				SINT cheat = CONTROLS.mControllers[1].GetCheat();
				if (cheat == CHEAT_SLIPPERY)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_SLIPPERY);
				}
			}*/

			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** OPTIONS

		case FEPS_OPTIONS:
			if (ym != G0)
			{
				if (!mDBY)
				{
					if (ym > G0)
					{
						mOptionsMenuPos --;
					}
					else
					{
						mOptionsMenuPos ++;
					}

					mDBY = 6;
					SOUNDMOVE;
				}
				else
					mDBY --;
			}
			else
				mDBY = FALSE;

			if (xm != G0)
			{
				if (!mDBX)
				{
					if ((mOptionsMenuPos == 1) || (mOptionsMenuPos == 2))
					{
						SINT 	dir;
						if (xm > G0)
							dir = 1;
						else
							dir = -1;


						if (mOptionsMenuPos == 1)
						{
							CAREER.mSoundVolume += dir;
							if (CAREER.mSoundVolume < 0)
								CAREER.mSoundVolume = 0;
							if (CAREER.mSoundVolume > 10)
								CAREER.mSoundVolume = 10;
						}
						else
						{
							CAREER.mMusicVolume += dir;
							if (CAREER.mMusicVolume < 0)
								CAREER.mMusicVolume = 0;
							if (CAREER.mMusicVolume > 10)
								CAREER.mMusicVolume = 10;
						}
						SOUND.DeclareVolumeChange();
						SOUNDNAME;
					}
					else if (mOptionsMenuPos == 3)
					{
						if (xm > G0)
							mOptionsStereo = TRUE;
						else
							mOptionsStereo = FALSE;

						SOUND.DeclareVolumeChange();
						SOUNDNAME;
					}

					mDBX = 6;
				}
				else
					mDBX --;
			}
			else
				mDBX = FALSE;

			if (mOptionsMenuPos < 0)
				mOptionsMenuPos = 4;
			if (mOptionsMenuPos > 4)
				mOptionsMenuPos = 0;
			
			if (CONTROLLER_SELECT)
			{
				switch(mOptionsMenuPos)
				{
				case 0:
					mMode = FEPS_OPTIONS2CONTROLLER;
					mOptionsMenuSlide = SLIDE_TIME;
					mControllerMenuSlide = 0;
					mControllerSetting1 = CAREER.mControls1;
					mControllerSetting2	= CAREER.mControls2;
					SOUNDSELECT;
					break;
				case 4:
					mMode = FEPS_OPTIONS2SELMEM;
					mOptionsMenuSlide = SLIDE_TIME;
					mSelMemMenuSlide = 0;
					mSelMemWhereFrom = FEPS_OPTIONS;
					SOUNDSELECT;
					break;

				default:
					break;
				};
			}

			if (CONTROLLER_CANCEL)
			{
				mMode = FEPS_OPTIONS2MAIN;
				mOptionsMenuSlide = SLIDE_TIME;
				mMainMenuSlide = 0;
				CAREER.mStereo = mOptionsStereo;
				SOUND.DeclareVolumeChange();
				SDSSetBootRomSoundMode(CAREER.mStereo ? 0 : 1);
				SOUNDCANCEL;
			}
			break;

		case FEPS_OPTIONS2SELMEM:
			mSelMemMenuSlide ++;
			mOptionsMenuSlide --;
			if (mOptionsMenuSlide == 0)
			{
				mMode = FEPS_SELMEM;
				mCounter = 0;
				mSelMemMenuState = EMS_SELECT_CARD;
			}
			break;

		case FEPS_SELMEM2OPTIONS:
			mSelMemMenuSlide --;
			mOptionsMenuSlide ++;
			if (mSelMemMenuSlide == 0)
			{
				mMode = FEPS_OPTIONS;
			}
			break;

		case FEPS_OPTIONS2CONTROLLER:
			mOptionsMenuSlide --;
			mControllerMenuSlide ++;
			if (mOptionsMenuSlide == 0)
			{
				mMode = FEPS_CONTROLLER;
				mCounter = 0;
			}
			break;

			// ****************************************************************************
			// ****************************************************************************
			// ** CONTROLLER:

		case FEPS_CONTROLLER:
			// Player 1
			if (xm != G0)
			{
				if (!mDBX)
				{
					if (xm > G0)
					{
						mControllerSetting1 ++;
						if (mControllerSetting1 > 2)
							mControllerSetting1 = 0;
					}
					else
					{
						mControllerSetting1 --;
						if (mControllerSetting1 < 0)
							mControllerSetting1 = 2;
					}
					mDBX = 6;
					SOUNDMOVE;
				}
				else
					mDBX --;
			}
			else
				mDBX = FALSE;

			// Player 2
			if (xm2 != G0)
			{
				if (!mDBX2)
				{
					if (xm2 > G0)
					{
						mControllerSetting2 ++;
						if (mControllerSetting2 > 2)
							mControllerSetting2 = 0;
					}
					else
					{
						mControllerSetting2 --;
						if (mControllerSetting2 < 0)
							mControllerSetting2 = 2;
					}
					mDBX2 = 6;
					SOUNDMOVE;
				}
				else
					mDBX2 --;
			}
			else
				mDBX2 = FALSE;

			// save
			if (CONTROLLER_SELECT)
			{
				CAREER.mControls1 = mControllerSetting1;
				CAREER.mControls2 = mControllerSetting2;

				mMode = FEPS_CONTROLLER2OPTIONS;
				mControllerMenuSlide = SLIDE_TIME;
				mOptionsMenuSlide = 0;
				SOUNDSELECT;
			}

			// abort
			if (CONTROLLER_CANCEL)
			{
				mMode = FEPS_CONTROLLER2OPTIONS;
				mControllerMenuSlide = SLIDE_TIME;
				mOptionsMenuSlide = 0;
				SOUNDCANCEL;
			}

			break;

		case FEPS_CONTROLLER2OPTIONS:
			mControllerMenuSlide --;
			mOptionsMenuSlide ++;
			if (mControllerMenuSlide == 0)
			{
				mMode = FEPS_OPTIONS;
			}
			break;

			// ****************************************************************************
			// ****************************************************************************
			// ** CREDITS

		case FEPS_CREDITS:
			// roll the credits!
			mCreditsPos ++;

			if (mCreditsPos > CREDITS_TIME)
			{
				mMode = FEPS_CREDITS2MAIN;
				mCreditsMenuSlide = SLIDE_TIME;
				mMainMenuSlide = 0;
				SOUNDSELECT;
			}

			if ((CONTROLLER_SELECT) ||
			    (CONTROLLER_CANCEL))
			{
				mMode = FEPS_CREDITS2MAIN;
				mCreditsMenuSlide = SLIDE_TIME;
				mMainMenuSlide = 0;
				SOUNDSELECT;
			}
			break;

			// ****************************************************************************
			// ****************************************************************************
		};

		mCounter ++;
		mArrowCounter ++;
		mCharRotTimer ++;
		mAttractCounter ++;

		ProcessParticles();
	}
}	


//******************************************************************************************
void	CDCFEPrisonSelect::ProcessParticles()
{
	// lightning?
	if (mLightningTime == 0)
	{
		if ((rand() % 50) == 0)
		{
			mLightningTime = (rand() % 60) + 10;

			SINT	pair = rand() % NUM_TITLE_LIGHTING_PAIRS;
			SINT	p1 = title_lightning_pairs[pair][0];
			SINT	p2 = title_lightning_pairs[pair][1];
			mLightningSX = title_lightning_places[p1][0];
			mLightningSY = title_lightning_places[p1][1] + TITLE_BAR_Y;
			mLightningEX = title_lightning_places[p2][0];
			mLightningEY = title_lightning_places[p2][1] + TITLE_BAR_Y;

			mLightningPoint = p2;
		}
	}

	if ((mMode == FEPS_START) ||
	    (mMode == FEPS_START2MAIN) ||
	    (mMode == FEPS_START2SELMEM))
		mLightningTime = 0;

	if (mLightningTime)
	{
		mLightningTime --;

		AddParticles(mLightningSX, mLightningSY, 3, 1);
		AddParticles(mLightningEX, mLightningEY, 3, 1);

		// randomly move to another point
		if ((rand() % 10) == 0)
		{
			SINT c0, pair;

			pair = rand() % NUM_TITLE_LIGHTING_PAIRS;

			for (c0 = 0; c0 < NUM_TITLE_LIGHTING_PAIRS; c0 ++)
			{
				if ((title_lightning_pairs[pair][0] == mLightningPoint) ||
				    (title_lightning_pairs[pair][1] == mLightningPoint))
				{
					SINT	p1 = title_lightning_pairs[pair][0];
					SINT	p2 = title_lightning_pairs[pair][1];
					mLightningSX = title_lightning_places[p1][0];
					mLightningSY = title_lightning_places[p1][1] + TITLE_BAR_Y;
					mLightningEX = title_lightning_places[p2][0];
					mLightningEY = title_lightning_places[p2][1] + TITLE_BAR_Y;

					if (mLightningPoint == p1)
						mLightningPoint = p2;
					else
						mLightningPoint = p1;
					break;
				}

				pair = (pair + 1) % NUM_TITLE_LIGHTING_PAIRS;
			}
		}
	}

	SINT	c0;

	for (c0 = 0; c0 < NUM_PSXFE_PARTICLES; c0 ++)
	{
		SPSXFEParticle *p = &mParticles[c0];

		if (p->mVal != 0)
		{
			p->mVal -= 6;
			p->mX += p->mVX;
			p->mY += p->mVY;

			p->mVY += G(0, 40000);

			if (p->mVal < 0)
				p->mVal = 0;
		}
	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::LoadAndRenderStart()
{
	char	text[256];
	SINT	sl;

	//use the engine sprite draw to ensure consistency with loading screen....
	ENGINE.DrawSprite(FEDCPST_TITLE1,   0,   0, 256, 256, OT_LENGTH - 3);
	ENGINE.DrawSprite(FEDCPST_TITLE2, 256,   0, 256, 256, OT_LENGTH - 3);
	ENGINE.DrawSprite(FEDCPST_TITLE3, 512,   0, 128, 256, OT_LENGTH - 3);
	ENGINE.DrawSprite(FEDCPST_TITLE4,   0, 256, 256, 256, OT_LENGTH - 3);
	ENGINE.DrawSprite(FEDCPST_TITLE5, 256, 256, 256, 256, OT_LENGTH - 3);
	ENGINE.DrawSprite(FEDCPST_TITLE6, 512, 256, 128, 256, OT_LENGTH - 3);	

/*	DrawSprite(FEDCPST_TITLE1, 0    , 0     , OT_LENGTH - 3, 127, 127, 127);
	DrawSprite(FEDCPST_TITLE2, 204.8, 0     , OT_LENGTH - 3, 127, 127, 127);
	DrawSprite(FEDCPST_TITLE3, 409.6, 0     , OT_LENGTH - 3, 127, 127, 127);
	DrawSprite(FEDCPST_TITLE4, 0    , 136.53, OT_LENGTH - 3, 127, 127, 127);
	DrawSprite(FEDCPST_TITLE5, 204.8, 136.53, OT_LENGTH - 3, 127, 127, 127);
	DrawSprite(FEDCPST_TITLE6, 409.6, 136.53, OT_LENGTH - 3, 127, 127, 127);
*/

	if ((mCounter % 31) < 16)
	{
		// Draw some text
		CText::GetString(TEXT_PRESS_START, text);
		SINT 	font = GAME.GetLanguage() == 0 ? 0 : 2;
		sl = GetStringSize(font, text);
//		if (GAME.GetLanguage() == 1) // french is awkward
//			DrawString(font, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 160, 0x007f5f3f);
//		else
			DrawString(font, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 225, 0x007f5f3f);
	}

	CText::GetString(TEXT_COPYRIGHT, text);
	sl = GetStringSize(2, text);
	DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 180, 0x007f5f3f);

#if DC_PREVIEW == DCP_FIRST
	sprintf(text,"Press Preview Build - 25/9/00");
	sl = GetStringSize(2, text);
	DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 210, 0x007f5f3f);
#endif

	if (GAME.IsCheatEnabled(CHEAT_VERSION))
	{
//		sprintf(text, "v1.00");
//#if DISP_MODE == MODE_NTSC
//		sprintf(text, "v1.02b");
//#else
//		sprintf(text, "Beta v1.5");
		sprintf(text, "v1.0");
//#endif
		sl = GetStringSize(2, text);
		DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 215, 0x007f5f3f);
	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderMain()
{
	SINT	yps;
	GINT	gy = G1;

	if ((mMode == FEPS_START2MAIN) ||
	    (mMode == FEPS_MAIN2START))
	{
		gy = G(SLIDE_TIME - mMainMenuSlide);
		gy = (gy * gy * gy) / (SLIDE_TIME * SLIDE_TIME);
		yps = (G(TITLE_BAR_Y) - (gy * G(150)) / SLIDE_TIME + GINT_HALF).Whole();
		
		if (mMainMenuFromSelMem)
			yps = TITLE_BAR_Y;
		
		SINT	c0;
		for (c0 = 0; c0 < 5; c0 ++)
		{
			DrawSprite(FEDCPST_LCD1 + c0, 102.2 * float(c0), yps, 2, 127, 127, 127);
		}
	}

	SINT	c0;
	SINT	y = 100;

	for (c0 = 0; c0 < 4; c0 ++)
	{
		SINT 	c1;
		SINT	x = 112;

		GINT	gx;
		SINT	xoffs;

		BOOL	left = (mMode != FEPS_START2MAIN) && (mMode != FEPS_SELMEM2MAIN);

		gx = G(SLIDE_TIME - mMainMenuSlide) - (G(left ? c0 : (4 - c0)) * G(1, 30000));

		if (gx < G0)
			gx = G0;
		else
		{
			gx = (gx * gx) / SLIDE_TIME;
			gx = (gx * 2600) / SLIDE_TIME;

			if (left)
				gx = -gx;
		}
		xoffs = gx.Whole();

		if (xoffs < 1000)
		{
			x += xoffs;

			SINT	colr = 60, colg = 72, colb = 100;

			if (mMainMenuPos == c0)
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr / 4;
			}

			if ((c0 == 1) && ((!(PLATFORM.IsControllerValid(1)))))
			{
				colr = colg = colb = 64;
			}

#if DC_PREVIEW == DCP_FIRST
			if (c0 != 0)
			{
				colr = colg = colb = 64;
			}
#endif
			DrawSprite(FEDCPST_BOXL, x    , y    , 2, colr, colg, colb, 2);
			x += 48;

			for (c1 = 0; c1 < 4; c1 ++)
			{
				DrawSprite(FEDCPST_BOXM, x    , y    , 2, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEDCPST_BOXR, x    , y    , 2, colr, colg, colb, 2);

			// draw text
			//!! Temporary!
			char	s[100];
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_SINGLE_PLAYER, s); break;
			case 1:	CText::GetString(TEXT_MULTIPLAYER, s); break;
			case 2:	CText::GetString(TEXT_OPTIONS, s); break;
			case 3:	CText::GetString(TEXT_CREDITS, s); break;
			};

			SINT	tcol;

			if (mMainMenuPos == c0)
				tcol = 0x007f7f7f;
			else
				tcol = 0x005f5f5f;

			SINT	sl = GetStringSize(0, s);
			DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol);
			DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000);
		}
		y += 30;
	}

	if (mMode == FEPS_MAIN)
	{
		char text[200];

		CText::GetString(TEXT_HELPDC1, text);
		SINT	sl = GetStringSize(2, text);
		DrawString(2, text, 256 - sl / 2, 230 , 0x007f7f7f);
	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderSelMem()
{
	SINT	yps;
	GINT	gy = G1;

	if (mMode == FEPS_START2SELMEM)
	{
		gy = G(SLIDE_TIME - mSelMemMenuSlide);
		gy = (gy * gy * gy) / (SLIDE_TIME * SLIDE_TIME);
		yps = (G(TITLE_BAR_Y) - (gy * G(150)) / SLIDE_TIME + GINT_HALF).Whole();
		
		SINT	c0;
		for (c0 = 0; c0 < 5; c0 ++)
		{
			DrawSprite(FEDCPST_LCD1 + c0, 102.2 * float(c0), yps, 2, 127, 127, 127);
		}
	}


	// draw a text box
	GINT	gh = G(mSelMemMenuSlide) / SLIDE_TIME;
	SINT	col = (gh * 40).Whole();
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;

	SINT	w = (gh * 480).Whole();
	SINT	h = (gh * 150).Whole();

	DrawBox(256 - w / 2, 160 - h / 2, w, h, 3, col, col, col);
	

	if ((mMode == FEPS_SELMEM) && (mCounter > 2)) // let it check first...
	{	
		char	s[200], s2[200];
		SINT	sl;
		SINT	c0, c1;

		switch (mSelMemMenuState)
		{
		case EMS_SELECT_CARD:
			{
				SINT ty = 87;

				CText::GetString(TEXT_DC_CHOOSE_MEMORY_CARD, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 14;
				
				SINT	oty = ty;
				BOOL	ok = FALSE;
				BOOL	save_found = FALSE;
				
				for (c0 = 0; c0 < 4; c0 ++)
				{
					for (c1 = 0; c1 < 2; c1 ++)
					{
						if (mSelMemVMS[c0][c1].mState == VMU_EXISTS_FORMATTED)
						{		
							CText::GetString(TEXT_DC_PORT_SOCKET_BLOCKS, s2);
							sprintf(s, s2, (mSelMemVMS[c0][c1].mSavePresent) ? '{' : ' ' , 'A' + c0, c1 + 1, mSelMemVMS[c0][c1].mBlocksFree);
							sl = GetStringSize(2, s);
							
							if ((mSelMemVMS[c0][c1].mBlocksFree >= BLOCKS_NEEDED_TO_SAVE) ||
							    (mSelMemVMS[c0][c1].mSavePresent))
							{
								ok = TRUE;
								if ((c0 == mSelMemCard) && (c1 == mSelMemPort))
								{
									SINT	colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
									SINT	colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
									SINT	colb = colr / 4;
									SINT	col = (colb << 16) + (colg << 8) + colr;
									DrawString(2, s, 256 - sl / 2, ty , col);
								}
								else
									DrawString(2, s, 256 - sl / 2, ty , 0x001f4f7f);
							}
							else
								DrawString(2, s, 256 - sl / 2, ty , 0x003f3f3f);
							
							if (mSelMemVMS[c0][c1].mSavePresent)
								save_found = TRUE;
							
							ty += 9;
						}
					}
				}
				
				if (ty == oty)
				{
					CText::GetString(TEXT_DC_NO_CARDS, s);
					sl = GetStringSize(2, s);
					DrawString(2, s, 256 - sl / 2, ty , 0x003f7f3f);
					ty += 9;
				}
				
				ty += 4;
				
				CText::GetString(TEXT_DC_BLOCKS_REQUIRED, s2);
				sprintf(s, s2, BLOCKS_NEEDED_TO_SAVE);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 14;
				
				if (save_found)
				{
					CText::GetString(TEXT_DC_SAVE_PRESENT, s);
					sl = GetStringSize(0, s);
					DrawString(0, s, 256 - sl / 2, ty , 0x001f7f1f);
					ty += 14;
				}
				
				if (!ok)
				{
					CText::GetString(TEXT_DC_INSERT_CARD, s2);
					sprintf(s, s2, BLOCKS_NEEDED_TO_SAVE);
					sl = GetStringSize(2, s);
					DrawString(2, s, 256 - sl / 2, ty , 0x002f2f7f);

					ty += 14;

					SINT	colr = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(48)).Whole() + 48;
					SINT	colg = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(36)).Whole() + 48;
					SINT	colb = colr / 4;
					SINT	col = (colb << 16) + (colg << 8) + colr;

					CText::GetString(TEXT_DC_NO_SAVE1, s);
					sl = GetStringSize(0, s);
					DrawString(0, s, 256 - sl / 2, ty , col);
					
					ty += 9;
					CText::GetString(TEXT_DC_NO_SAVE2, s);
					sl = GetStringSize(0, s);
					DrawString(0, s, 256 - sl / 2, ty , col);
				}

				if (mSelMemWhereFrom != FEPS_OPTIONS)
					CText::GetString(TEXT_HELPDC5, s);
				else
					CText::GetString(TEXT_HELPDC1, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2,  220 , 0x007f7f7f);
			}
			break;
			
		case	EMS_SAVING:
			{
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_SAVING1, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 16;

				CText::GetString(TEXT_DC_SAVING2, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x002f2f7f);
				
				ty += 16;

				CText::GetString(TEXT_DC_SAVING3, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x002f2f7f);
				
				ty += 16;
			}
			break;
		
		case	EMS_LOADING:
			{
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_LOADING1, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 16;

				CText::GetString(TEXT_DC_LOADING2, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x002f2f7f);
				
				ty += 16;
			}
			break;

		case	EMS_SAVE_FAILED:
			{
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_SAVE_FAILED, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 20;
				
				SINT	colr = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(48)).Whole() + 48;
				SINT	colg = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(36)).Whole() + 48;
				SINT	colb = colr / 4;
				SINT	col = (colb << 16) + (colg << 8) + colr;
				
				DWORD	c1, c2;
				
				if (mSelMemFailedPos == 0)
				{
					c1 = col;
					c2 = 0x004f4f4f;
				}
				else
				{
					c1 = 0x004f4f4f;
					c2 = col;
				}
				
				CText::GetString(TEXT_DC_SELECT_AGAIN, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c1);
				ty += 16;

				CText::GetString(TEXT_DC_CONTINUE, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c2);
				ty += 30;
				
				CText::GetString(TEXT_DC_NO_SAVE1A, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);
				
				ty += 9;
				CText::GetString(TEXT_DC_NO_SAVE2, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);	
					
				CText::GetString(TEXT_HELPDC5, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, 220 , 0x007f7f7f);
			}
			break;

		case	EMS_LOAD_FAILED:
			{
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_LOAD_FAILED, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 20;
				
				SINT	colr = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(48)).Whole() + 48;
				SINT	colg = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(36)).Whole() + 48;
				SINT	colb = colr / 4;
				SINT	col = (colb << 16) + (colg << 8) + colr;
				
				DWORD	c1, c2;
				
				if (mSelMemFailedPos == 0)
				{
					c1 = col;
					c2 = 0x004f4f4f;
				}
				else
				{
					c1 = 0x004f4f4f;
					c2 = col;
				}
				
				CText::GetString(TEXT_DC_SELECT_AGAIN, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c1);
				ty += 16;

				CText::GetString(TEXT_DC_CONTINUE, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c2);
				ty += 30;
				
				CText::GetString(TEXT_DC_NO_SAVE1A, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);
				
				ty += 9;
				CText::GetString(TEXT_DC_NO_SAVE2, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);		
				
				CText::GetString(TEXT_HELPDC5, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, 220 , 0x007f7f7f);
				
			}
			break;
			
		case	EMS_SAVE_LOAD:
			{
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_SAVE_LOAD, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 16;

				CText::GetString(TEXT_DC_PORT_SOCKET, s2);
				sprintf(s, s2,  'A' + mSelMemCard, mSelMemPort + 1);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 24;
				
				SINT	colr = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(48)).Whole() + 48;
				SINT	colg = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(36)).Whole() + 48;
				SINT	colb = colr / 4;
				SINT	col = (colb << 16) + (colg << 8) + colr;
				
				DWORD	c1, c2;
				
				if (mSelMemSLPos == 0)
				{
					c1 = col;
					c2 = 0x004f4f4f;
				}
				else
				{
					c1 = 0x004f4f4f;
					c2 = col;
				}
				
				CText::GetString(TEXT_SAVE_GAME, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c1);
				ty += 16;

				CText::GetString(TEXT_LOAD_GAME, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c2);
				ty += 16;
				
				CText::GetString(TEXT_HELPDC1, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, 220 , 0x007f7f7f);
				
			}
			break;
			
		case	EMS_LOAD_YN:
			{
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_LOAD_YN1, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 16;

				CText::GetString(TEXT_DC_LOAD_YN2, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 24;
				
				SINT	colr = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(48)).Whole() + 48;
				SINT	colg = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(36)).Whole() + 48;
				SINT	colb = colr / 4;
				SINT	col = (colb << 16) + (colg << 8) + colr;
				
				DWORD	c1, c2;
				
				if (mSelMemYNPos == 0)
				{
					c1 = col;
					c2 = 0x004f4f4f;
				}
				else
				{
					c1 = 0x004f4f4f;
					c2 = col;
				}
				
				CText::GetString(TEXT_YES, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c1);
				ty += 16;

				CText::GetString(TEXT_NO, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c2);
				ty += 16;
				
				CText::GetString(TEXT_HELPDC1, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, 220 , 0x007f7f7f);
				
			}
			break;
		case	EMS_SAVE_YN:
			{
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_SAVE_YN1, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 16;

				CText::GetString(TEXT_DC_SAVE_YN2, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 24;
				
				SINT	colr = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(48)).Whole() + 48;
				SINT	colg = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(36)).Whole() + 48;
				SINT	colb = colr / 4;
				SINT	col = (colb << 16) + (colg << 8) + colr;
				
				DWORD	c1, c2;
				
				if (mSelMemYNPos == 0)
				{
					c1 = col;
					c2 = 0x004f4f4f;
				}
				else
				{
					c1 = 0x004f4f4f;
					c2 = col;
				}
				
				CText::GetString(TEXT_YES, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c1);
				ty += 16;

				CText::GetString(TEXT_NO, s);
				sl = GetStringSize(2, s);
				DrawString(2, s, 256 - sl / 2, ty , c2);
				ty += 16;
				
				CText::GetString(TEXT_HELPDC1, s);
				sl = GetStringSize(0, s);
				DrawString(0, s, 256 - sl / 2, 220 , 0x007f7f7f);
				
			}
			break;
		
		
		};
	}	
}


//******************************************************************************************
void	CDCFEPrisonSelect::RenderChar()
{
	SINT	c;
	c = (mCharMenuSlide * 128) / SLIDE_TIME;

//			c = 128;

//	DrawSprite(FEDCPST_CHARGRIDA, 50  , 180, OT_LENGTH - 4, c, c, c, 1);
//	DrawSprite(FEDCPST_CHARGRIDB, 306 , 180, OT_LENGTH - 4, c, c, c, 1);

	GINT	gy;
	SINT	yps;

	gy = G(SLIDE_TIME - mCharMenuSlide);
	gy = (gy * gy) / (SLIDE_TIME * SLIDE_TIME);
	yps = (gy * G(195)).Whole() + 190;

	DrawSprite(FEDCPST_CHARGRIDB, 154 , yps, OT_LENGTH - 4);
	DrawSprite(FEDCPST_CHARGRIDB, 256 , yps, OT_LENGTH - 4, 127, 127, 127, -1, FALSE, G1, 1);

//	if (mMode == FEPS_CHAR)
	{
		// draw the character meshes
		MATRIX lcol;

		// setup the camera
		GINT 	gy;

		gy = G(mCharMenuSlide);
		gy = (gy * gy) / (SLIDE_TIME * SLIDE_TIME);
		gy = -G(15 + 75) + (gy * 75);

		mCamera.mPos = GVector(G0, gy, -G(2, 32768));
		mCamera.mLookAt = GVector(G0, 0, -G(2, 32768));
		mCamera.mMode = CCamera::LOOKAT;
		mCamera.mYaw = G0;
		mCamera.mPitch = G0;
		mCamera.FillOut();

		// aspect ratio
		// set mesh position and orientation
		CDCMesh	*m;

		// which meshes to draw, and where
		SINT	character = mCharacter + mCharSlide.Whole();
//		if (mCharSlide < G0)
//			character --;

		while (character < 0)
		{
			character += 6;
		};

		while (character > 5)
		{
			character -= 6;
		};

		GINT	gpos = (mCharSlide + G(100)).GFrac();   // won't work with negative numbers
//		GINT	gpos = mCharSlide - mCharSlide.Whole();
//		if (gpos > G0)
//			gpos -= G1;
		gpos = -gpos;

		if (mCharSlide != G0)
		{
			m = ENGINE.GetNumberedMesh(FEPSM_ANGEL + character);
			if (m)
			{
#ifdef JCLDCNOPSX  // lighting
				if ((!can_select_doctor()) && (character == 5))
				{
					SetMLight(lcol.m,0, -2.0, -2.0, -2.0);
					SetMLight(lcol.m,1, -2.0, -2.0, -2.0);
					SetMLight(lcol.m,2, -2.0, -2.0, -2.0);
				}
				else
				{
					SetMLight(lcol.m,0, 1.99 , 0.40, 0.10);
					SetMLight(lcol.m,1, 0.20 , 0.20 , 1.20);
					SetMLight(lcol.m,2, 1.95 , 1.95 , 1.95);
				}
				SetColorMatrix(&lcol);
#endif

				CFrontEndThing	*t = &mCharThings[character];
				t->mPos = GVector(gpos * G(10), G0, G0);

				SRTInfo	ri;
				ri.Clear();
				ri.i0 = FEPSM_ANGEL + character + 10000;
				ri.i1 = -1;
				ri.i2 = -2;

				CDCMesh::RenderFromData(t, &ri, FALSE);
			}
			character = (character + 1) % 6;
			gpos = gpos + G1;
		}

		m = ENGINE.GetNumberedMesh(FEPSM_ANGEL + character);
		if (m)
		{
#ifdef JCLDCNOPSX // lighting
			if ((!can_select_doctor()) && (character == 5))
			{
				SetMLight(lcol.m,0, -2.0, -2.0, -2.0);
				SetMLight(lcol.m,1, -2.0, -2.0, -2.0);
				SetMLight(lcol.m,2, -2.0, -2.0, -2.0);
			}
			else
			{
				SetMLight(lcol.m,0, 1.99 , 0.40, 0.10);
				SetMLight(lcol.m,1, 0.20 , 0.20 , 1.20);
				SetMLight(lcol.m,2, 1.95 , 1.95 , 1.95);
			}
			SetColorMatrix(&lcol);
#endif

			CFrontEndThing	*t = &mCharThings[character];
			t->mPos = GVector(gpos * G(10), G0, G0);

			SRTInfo	ri;
			ri.Clear();
			ri.i0 = FEPSM_ANGEL + character + 10000;
			ri.i1 = -1;
			ri.i2 = -2;

			CDCMesh::RenderFromData(t, &ri, FALSE);
		}
	}

	if (mMode == FEPS_CHAR)
	{
		// draw arrows?
		if ((mArrowCounter & 31) < 20)
		{
			DrawSprite(FEDCPST_ARROW_L, 178, 119 + ARROWOFFSET, 0, 127, 127, 127, 2);
			DrawSprite(FEDCPST_ARROW_R, 318, 119 + ARROWOFFSET, 0, 127, 127, 127, 2);
		}
	}


	// draw some text boxes
	SINT	col = c / 3;
	GINT	gh = G(mCharMenuSlide) / SLIDE_TIME;
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;
	SINT	h = (gh * 130).Whole();

	DrawBox(16 , 100, 150, h, 3, col, col, col);
	DrawBox(346, 100, 150, h, 3, col, col, col);

	// and some assorted text
	char	text[100];
	SINT	sl;

	if (mMode == FEPS_CHAR)
	{
		//*****
		// Left

//		// Portrait
//		DrawSprite(FEDCPST_PORT_ANGEL + mCharacter, 51, 110, 0);

		// name
		CText::GetString(TEXT_ANGEL + mCharacter, text);
		sl = GetStringSize(0, text);
		DrawString(0, text, 93 - sl / 2, 107, 0x003f5f7f);

		// bio
		CText::GetString(TEXT_ANGEL_COMMENT + mCharacter, text);
		sl = GetStringSize(2, text);
		DrawDynamicText(2, text, 93 - sl / 2, 125, 64, 96, 128, (mCounter - mCharSlideStart) * 2);

		// levels & stars
		DrawSprite(FEDCPST_QUALIFY	,40, 142 + MEDALOFFSET, 0);
		DrawSprite(FEDCPST_GOLD   	,40, 171 + MEDALOFFSET, 0);
		DrawSprite(FEDCPST_LOST_TIME 	,40, 200 + MEDALOFFSET, 0);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumLevelsCompleted);
		DrawDynamicText(0, text, 100, 149, 64, 96, 128, (mCounter - mCharSlideStart) * 2);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumGoldStars);
		DrawDynamicText(0, text, 100, 178, 64, 96, 128, (mCounter - mCharSlideStart) * 2);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumLostToysTimes);
		DrawDynamicText(0, text, 100, 207, 64, 96, 128, (mCounter - mCharSlideStart) * 2);


		//******
		// Right

		CText::GetString(TEXT_STATS, text);
		sl = GetStringSize(0, text);
		DrawDynamicText(0, text, 421 - sl / 2, 110, 127, 80, 64, (mCounter * 2));

		// stats
		CText::GetString(TEXT_STRENGTH, text);
		DrawDynamicText(2, text, 355, 140, 48, 92, 48, (mCounter * 2)    );
		CText::GetString(TEXT_SPEED, text);
		DrawDynamicText(2, text, 355, 156, 48, 92, 48, (mCounter * 2) - 3);
		CText::GetString(TEXT_AGILITY, text);
		DrawDynamicText(2, text, 355, 172, 48, 92, 48, (mCounter * 2) - 6);
		CText::GetString(TEXT_STAMINA, text);
		DrawDynamicText(2, text, 355, 188, 48, 92, 48, (mCounter * 2) - 9);

		sprintf(text, "````");
		text[get_char_stats(mCharacter, 0)] = 0;
		DrawDynamicText(0, text, 450, 140, 128, 32, 32, (mCounter - mCharSlideStart));
		sprintf(text, "````");
		text[get_char_stats(mCharacter, 1)] = 0;
		DrawDynamicText(0, text, 450, 156, 128, 32, 32, (mCounter - mCharSlideStart));
		sprintf(text, "````");
		text[get_char_stats(mCharacter, 2)] = 0;
		DrawDynamicText(0, text, 450, 172, 128, 32, 32, (mCounter - mCharSlideStart));
		sprintf(text, "````");
		text[get_char_stats(mCharacter, 3)] = 0;
		DrawDynamicText(0, text, 450, 188, 128, 32, 32, (mCounter - mCharSlideStart));
	}

}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderPrisonNew()
{
	GINT	gy;
	SINT	yps;

	gy = G(SLIDE_TIME - mPrisonMenuSlide);
	gy = (gy * gy * gy) / (SLIDE_TIME * SLIDE_TIME);
	yps = (G(87) - (gy * G(195)) / SLIDE_TIME + GINT_HALF).Whole();

	// which prisons to draw, and where
	SINT	prison = mPrison + mPrisonSlide.Whole();

	prison -= 1;

/*	while (prison < 0)
	{
		prison += 10;
	};

	while (prison > 9)
	{
		prison -= 10;
	};*/

	GINT	gpos = (mPrisonSlide + G(100)).GFrac();   // won't work with negative numbers
	gpos = -gpos;

	gpos -= G1;

	SINT	number = (mPrisonSlide != G0) ? 4 : 3;
	SINT	c0;

	for (c0 = 0; c0 < number; c0 ++)
	{
		if ((prison >= 0) && (prison <= 9))
		{
			SINT	col = 32;

			if (prison == mPrison)
			{
				if (can_select_prison(prison, mCharacter) || ((mPrisonBonus) && (mPrisonBonusSlide > GINT_HALF)))
					col = 128;
				else
					col = 64;
			}

			#define PRIS_GAP	(160 + 150)

			if (mPrisonBonusSlide == G0)
			{
				DrawBorderClip((gpos * G(PRIS_GAP)).Whole() + 176, yps, 160, 2);
				if (((mMode != FEPS_PSNEW2ARENA) && (mMode != FEPS_ARENA2PSNEW)) || (prison != mPrison))
					DrawSprite(FEDCPST_PR_ECO + prison, (gpos * G(PRIS_GAP)).Whole() + 176, yps, 5, col, col, col, -1, !(can_select_prison(prison, mCharacter)));
			}
			else if (mPrisonBonusSlide == G1)
			{
				if ((mMode != FEPS_PSNEW2ARENA) && (mMode != FEPS_ARENA2PSNEW) && (prison == mPrison))
				{
					DrawBorderClip((gpos * G(PRIS_GAP)).Whole() + 176, yps, 160, 2);
					DrawSprite(FEDCPST_PR_BONUS, (gpos * G(PRIS_GAP)).Whole() + 176, yps, 5, col, col, col, -1, FALSE);
				}
			}
			else
			{
				if ((mPrisonBonusSlide < GINT_HALF) || (prison == mPrison))
				{
					DrawBorderClipRot(FEDCPST_PR_ECO + prison, FEDCPST_PR_BONUS, (gpos * G(PRIS_GAP)).Whole() + 176, yps, 160, 2, col, col, col, mPrisonBonusSlide, !(can_select_prison(prison, mCharacter)));
				}
			}
		}
		prison = (prison + 1);
		gpos = gpos + G1;
	}

	if ((mMode == FEPS_PSNEW) && (mPrisonSlide == G0) &&
	    ((mPrisonBonusSlide == G0) || (mPrisonBonusSlide == G1)))
	{
		// draw arrows?
		if ((mArrowCounter & 31) < 20)
		{
			if (mPrisonBonus)
			{
				DrawSprite(FEDCPST_ARROW_D, 256 - 7, 150 + ARROWOFFSET, 0, 127, 127, 127, 2);
			}
			else
			{
				if (mPrison != 0)
					DrawSprite(FEDCPST_ARROW_L, 138, 119 + ARROWOFFSET, 0, 127, 127, 127, 2);
				if (mPrison != 9)
					DrawSprite(FEDCPST_ARROW_R, 358, 119 + ARROWOFFSET, 0, 127, 127, 127, 2);
				if (can_select_bonus_prison(mCharacter))
					DrawSprite(FEDCPST_ARROW_U, 256 - 7, 90 + ARROWOFFSET, 0, 127, 127, 127, 2);
			}
		}
	}

	// draw text box
	SINT	col = (mPrisonMenuSlide * 128) / SLIDE_TIME / 3;
	GINT	gh = G(mPrisonMenuSlide) / SLIDE_TIME;
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;
	SINT	h = (gh * 76).Whole();

	if ((mMode != FEPS_PSNEW2ARENA) && (mMode != FEPS_ARENA2PSNEW))
	{
		DrawBox(16 , 180 + (76 - h), 232, h, 3, col, col, col);
		DrawBox(262, 180 + (76 - h), 232, h, 3, col, col, col);
	}

	// and some text
	char	text[100];

	prison = mPrison;
	if (mPrisonBonus)
		prison = 10;

	if (mMode == FEPS_PSNEW)
	{
		CText::GetString(TEXT_PRN_ECO + prison, text);
		DrawDynamicText(0, text , 25, 185, 32, 128, 32, (mCounter - mPrisonSlideStart) * 2);

		if (can_select_prison(prison, mCharacter))
		{
			// Prison Stats
			CText::GetString(TEXT_SECURITY_LEVEL, text);
			DrawDynamicText(2, text, 25, 200, COL_GOLD, (mCounter - mPrisonAccessStart) * 2);

			CText::GetString(TEXT_INMATES, text);
			DrawDynamicText(2, text, 25, 212, COL_GOLD, (mCounter - mPrisonAccessStart) * 2);

			CText::GetString(TEXT_ESCAPE_ATTEMPTS, text);
			DrawDynamicText(2, text, 25, 224, COL_GOLD, (mCounter - mPrisonAccessStart) * 2);

#if DISP_MODE == MODE_PAL
			CText::GetString(TEXT_SUCCESSFUL_ESCAPES, text);
			DrawDynamicText(2, text, 25, 236, COL_GOLD, (mCounter - mPrisonAccessStart) * 2);
#endif

			sprintf(text, "---");

			if (!mPrisonBonus)
				sprintf(text, "%d", 4 - (mPrison / 3));
			DrawDynamicText(2, text, 200, 200, COL_BLUE, (mCounter - mPrisonSlideStart));

			if (!mPrisonBonus)
				sprintf(text, "%d", (mPrison + 10) * 29);
			DrawDynamicText(2, text, 200, 212, COL_BLUE, (mCounter - mPrisonSlideStart));

			if (!mPrisonBonus)
				sprintf(text, "%d", (mPrison + 10) * 7);
			DrawDynamicText(2, text, 200, 224, COL_BLUE, (mCounter - mPrisonSlideStart));

#if DISP_MODE == MODE_PAL
			if (!mPrisonBonus)
				sprintf(text, "%d", ((mPrison + 10) * 7 * (9 - mPrison)) / 18);
			DrawDynamicText(2, text, 200, 236, COL_BLUE, (mCounter - mPrisonSlideStart));
#endif

			// Player Stats
			CText::GetString(TEXT_ARENAS_COMPLETED, text);
			DrawDynamicText(0, text, 270, 185, 32, 128, 32, (mCounter - mPrisonAccessStart) * 2);

			SINT nlip = CLevelStructure::GetNumLevelsInPrison(prison);
			if ((prison == 9) && (mCharacter != 5))
				nlip --;	// only doctor can access all levels in Alpha Star...
			sprintf(text, "%d / %d", get_levels_completed(prison, mCharacter), nlip);
			DrawDynamicText(0, text, 270, 199, COL_BLUE, (mCounter - mPrisonSlideStart));
		}
		else
		{
			CText::GetString(TEXT_ACCESS_DENIED, text);
			DrawDynamicText(2, text, 20, 200, 128, 32, 32, (mCounter - mPrisonAccessStart));

			CText::GetString(TEXT_ARENAS_NEEDED_TO_ACCESS, text);
			DrawDynamicText(2, text, 270, 185, 128, 32, 32, (mCounter - mPrisonAccessStart) * 2);

			sprintf(text, "%d", CLevelStructure::GetNumLevelsToAccess(mPrison));
			DrawDynamicText(0, text, 270, 199, COL_BLUE, (mCounter - mPrisonSlideStart));
		}

//		CText::GetString(TEXT_TOTAL_ARENAS_COMPLETED, text);
//		DrawDynamicText(0, text, 270, 215, 32, 128, 32, (mCounter) * 2);
//		sprintf(text, "%d", CAREER.mCharCareers[mCharacter].mNumLevelsCompleted);
//		DrawDynamicText(0, text, 270, 230, COL_BLUE, (mCounter));

		// levels & stars
		DrawSprite(FEDCPST_QUALIFY	,265, 215 + MEDALOFFSET, 0);
		DrawSprite(FEDCPST_GOLD   	,338, 215 + MEDALOFFSET, 0);
		DrawSprite(FEDCPST_LOST_TIME 	,410, 215 + MEDALOFFSET, 0);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumLevelsCompleted);
		DrawDynamicText(2, text, 311 + 10, 222, 64, 96, 128, mCounter);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumGoldStars);
		DrawDynamicText(2, text, 384 + 10, 222, 64, 96, 128, mCounter);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumLostToysTimes);
		DrawDynamicText(2, text, 456 + 10, 222, 64, 96, 128, mCounter);


	}
}

//******************************************************************************************
/*void	get_arena_pos(SINT num, SINT l, SINT *x, SINT *y, GINT offs)
{
	GINT	rad = (G_PI * G(2) * l) / G(num);

	GINT	sn = GSIN(rad + offs);
	GINT	cs = GCOS(rad + offs);

	// 145, 130
	// 170    65

	*x = 145 + (sn * G(100) * (G1 + offs)).Whole();
	*y = 170 - (cs * G(45 ) * (G1 + offs)).Whole();
}*/

void	get_arena_pos(SINT num, SINT l, SINT *x, SINT *y, GINT offs)
{
	*y = 95;

	SINT	total_width = (42 * (num - 1)) + (5 * (num - 1));
	SINT	basex = 256 - (total_width / 2) + (47 * l);

	GINT	d = G(1) - offs;
	if (d < G0)
		d = G0;
	if (d > G1)
		d = G1;

	d *= G(23);
	d *= d;

	*x = basex + d.Whole();
}

void	CDCFEPrisonSelect::RenderArena()
{
	SINT	prison;

	if (mPrisonBonus)
		prison = 10;
	else
		prison = mPrison;


	SINT	arena_slide;

	if ((mMode == FEPS_ARENA) ||
		(mMode == FEPS_ARENA2QSAVE) ||
		(mMode == FEPS_QSAVE) ||
		(mMode == FEPS_QSAVE2ARENA) ||
		(mMode == FEPS_MESSAGE) ||
		(mMode == FEPS_ARENA2MESSAGE) ||
		(mMode == FEPS_MESSAGE2ARENA))
	{
		arena_slide = SLIDE_TIME;
	}
	else
	{
		arena_slide = mArenaMenuSlide;
	}

	GINT	qsave_sc = G(arena_slide) / G(SLIDE_TIME);

	GINT	sc = G(arena_slide) / G(SLIDE_TIME);
	sc *= G(2);
	if (sc > G1)
		sc = G1;

	// hmm - fuck about with SIN/COS to smooth it.
	sc = (GSIN((sc * G_PI) - G_PI_2) / 2) + GINT_HALF;

	#define AGC		48

	// prison image
	if ((mMode == FEPS_ARENA) ||
		(mMode == FEPS_ARENA2QSAVE) ||
		(mMode == FEPS_QSAVE) ||
		(mMode == FEPS_QSAVE2ARENA) ||
		(mMode == FEPS_MESSAGE) ||
		(mMode == FEPS_ARENA2MESSAGE) ||
		(mMode == FEPS_MESSAGE2ARENA))
	{
		// simple render
		DrawBorder(176, 125, 160, 80, 2, 40, 40, 40);
//		DrawBorderClip(176, 125, 160, 2);
		DrawSprite(FEDCPST_PR_ECO + prison, 176, 125, 5, AGC, AGC, AGC, -1, TRUE);
	}
	else
	{
		// Cross fade to greyscale
		SINT	xpos;
		SINT	ypos;

		if ((mMode == FEPS_QSAVE2SELMEM) || (mMode == FEPS_SELMEM2ARENA))
		{
			xpos = 176;
			ypos = 125 + ((G1 - qsave_sc) * G(250)).Whole();
		}
		else
		{
			xpos = 176;
			ypos = 125 - ((G1 - sc) * G(38)).Whole();
		}

		SINT	cola = (G(AGC) *       sc ).Whole();
//		SINT	colb = (G(128) * (G1 - sc)).Whole();
		SINT	colb = ((G(128 - AGC) * (G1 - sc)) + G(AGC)).Whole();
		SINT	colc = (G( 40) *       sc ).Whole();

		DrawBorder(xpos, ypos, 160, 80, 2, colc, colc, colc);
//		DrawBorderClip(xpos, ypos, 160, 2);

//		DrawSprite(FEDCPST_PR_ECO + prison, xpos, ypos, 5, cola, cola, cola, 1 , TRUE);
		DrawSprite(FEDCPST_PR_ECO + prison, xpos, ypos, 5, colb, colb, colb, -1, FALSE);
	}

	// Arena icons
	SINT	n = CLevelStructure::GetNumLevelsInPrison(prison);

	SINT	c0;
	for (c0 = 0; c0 < n; c0 ++)
	{
		SINT		x, y;
		SLevelInfo	*li = CLevelStructure::GetLevelInfo(prison, c0);

		if ((mMode == FEPS_QSAVE2SELMEM) || (mMode == FEPS_SELMEM2ARENA))
			sc = qsave_sc;
		else
			sc = G(arena_slide) / G(SLIDE_TIME);

		sc *= G(3);
		sc -= G1;
		sc -= G(c0) * G(0, 7000);

		get_arena_pos(n, c0, &x, &y, sc);

		SINT	col = 92;
		GINT	scale = G1;

		x -= 21;
		y -= 14;

		if ((mMode == FEPS_ARENA) && (mArena == c0) && (mArenaPendingVictory == -1))
		{
			col = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(64)).Whole() + 64;
			scale *= (GSIN(G(mCounter) * G(0, 15000)) * G(0, 5000)) + G1;

			// draw arrows
			if ((mArrowCounter & 31) < 20)
			{
				// not on bonus / alpha star when only one available...
				BOOL	arrows = TRUE;

				if (prison == 10)
					if (!(can_select_arena(10, 1, mCharacter)))
						arrows = FALSE;

				if (prison == 9)
					if ((!(can_select_arena(9, 1, mCharacter))) &&
						(!(can_select_arena(9, 2, mCharacter))))
						arrows = FALSE;

				if (arrows)
				{
					DrawSprite(FEDCPST_ARROW_L, x - 13 + 4, y + 8 + ARROWOFFSET, 0, 127, 127, 127, 2);
					DrawSprite(FEDCPST_ARROW_R, x + 47 + 4, y + 8 + ARROWOFFSET, 0, 127, 127, 127, 2);
				}
			}
		}

		SINT	colr, colg, colb;


		CLevelCareer	*lc = &CAREER.mCharCareers[mCharacter].mLevelCareers[prison][c0];
		if (lc->mFinished == ECL_YES)
		{
			colg = col;
			colr = (col * 3) / 4;
			colb = (col    ) / 2;
		}
		else if	((lc->mFinished == ECL_GOLD) || (lc->mFinished == ECL_LT))
		{
			colg = (col * 5) / 6;
			colr = col;
			colb = col / 2;
		}
		else if (!(can_select_arena(prison, c0, mCharacter)))
		{
			colr = colg = colb = 16;
		}
		else
		{
			colr = col;
			colg = col;
			colb = col;
		}

		if (lc->mFinished == ECL_LT)
		{
			// draw halo
			DrawSprite(FEDCPST_HALO, x - 4, y - 3, 0, 127, 127, 127, 1);
		}

		DrawSprite(FEDCPST_LT_KING_OF_THE_HILL + li->mType, x, y, 1, colr, colg, colb, 2 , FALSE);
	}

	// text boxes

	sc = G(arena_slide) / G(SLIDE_TIME);

	// hmm - fuck about with SIN/COS to smooth it.
	sc = (GSIN((sc * G_PI) - G_PI_2) / 2) + GINT_HALF;

	SINT	xpos, ypos, wid, hgt;

	if ((mMode == FEPS_QSAVE2SELMEM) || (mMode == FEPS_SELMEM2ARENA))
	{
		xpos = 353;
		ypos = 125 + ((G1 - qsave_sc) * G(250)).Whole();
		wid  = 166;
		hgt	 =  80;
	}
	else
	{
		xpos = 262 + (G(91) * sc).Whole();
		ypos = 180 - (G(55) * sc).Whole();
		wid  = 232 - (G(66) * sc).Whole();
		hgt	 =  76 + (G( 4) * sc).Whole();
	}
	DrawBox(xpos, ypos, wid, hgt, 3, 40, 40, 40);

	if ((mMode == FEPS_QSAVE2SELMEM) || (mMode == FEPS_SELMEM2ARENA))
	{
		xpos = 16 - 24;
	}
	else
	{
		xpos = 16 - (G(24) * sc).Whole();
	}

	DrawBox(xpos, ypos, wid, hgt, 3, 40, 40, 40);

	if ((mMode == FEPS_QSAVE2SELMEM) || (mMode == FEPS_SELMEM2ARENA))
	{
		DrawBox(0 , 208 + ((G1 - qsave_sc) * G(250)).Whole(), 512, 300, 3, 40, 40, 40);
	}
	else
	{
		SINT	col = (arena_slide * 128) / SLIDE_TIME / 3;
		GINT	gh = G(arena_slide) / SLIDE_TIME;
		gh = G1 - gh;
		gh = (gh * gh);
		gh = G1 - gh;
		SINT	h = (gh * 300).Whole();
		DrawBox(0 , 208 + (300 - h), 512, h, 3, col, col, col);
	}

	// draw text

	char	text[100];
	SINT	sl;

	if ((mMode == FEPS_ARENA) ||
		(mMode == FEPS_ARENA2QSAVE) ||
		(mMode == FEPS_QSAVE) ||
		(mMode == FEPS_QSAVE2ARENA) ||
		(mMode == FEPS_MESSAGE) ||
		(mMode == FEPS_ARENA2MESSAGE) ||
		(mMode == FEPS_MESSAGE2ARENA))
	{
		SLevelInfo		*li = CLevelStructure::GetLevelInfo(prison, mArena);
		CLevelCareer	*lc = &CAREER.mCharCareers[mCharacter].mLevelCareers[prison][mArena];

		// Level Name
		CText::GetString(li->mName, text);
		sl = GetStringSize(2, text);
		DrawDynamicText(2, text, 256 - sl / 2, 130, COL_BLUE, (mCounter - mArenaSlideStart) * 3);

		// How to qualify
		CText::GetString(TEXT_QUALIFY, text);
		DrawDynamicText(2, text, 363, 130, COL_GOLD, (mCounter * 2));

		SINT	f = 2;
		switch (li->mQualify)
		{
			case LIFIN_SURVIVE:
				CText::GetString(TEXT_LIFIN_SURVIVE, text);
				break;
			case LIFIN_WIN:
				CText::GetString(TEXT_LIFIN_WIN, text);
				break;
			case LIFIN_WIN2:
				CText::GetString(TEXT_LIFIN_WIN2, text);
				break;
			case LIFIN_FINISH:
				CText::GetString(TEXT_LIFIN_FINISH, text);
				break;
			case LIFIN_PAR:
				{
					char	t2[100];
					CText::GetString(TEXT_LIFIN_PAR, t2);
					sprintf(text, "%s %d", t2, CLevelStructure::GetLinksPar(mArena));
				}
				break;
			default:
				{
					char	t2[100];
					CText::GetString(TEXT_LIFIN_BEAT, t2);

					SINT	minutes = li->mQualify / 60 / 25;
					SINT	seconds = (li->mQualify / 25) % 60;
					SINT	milli   = (li->mQualify % 25) * 4;

					sprintf(text, "%s %d:%02d.%02d", t2, minutes, seconds, milli);

					f = 2;
				}
				break;
		};
		DrawDynamicText(f, text, 363, 146, COL_BLUE, (mCounter - mArenaSlideStart) * 2);

		// Your Best Time

		if (li->mQualify == LIFIN_PAR)
		{
			// golf
			CText::GetString(TEXT_BEST_SCORE, text);
			sl = GetStringSize(2, text);
			DrawDynamicText(2, text, 144 - sl, 130, COL_GOLD, (mCounter * 2));

			if (lc->mBestTime == 0xffff)
				sprintf(text, "---");
			else
			{
				sprintf(text, "%d", lc->mBestTime);
			}
			sl = GetStringSize(2, text);
			DrawDynamicText(2, text, 144 - sl, 146, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}
		else
		{
			CText::GetString(TEXT_BEST_TIME, text);
			sl = GetStringSize(2, text);
			DrawDynamicText(2, text, 144 - sl, 130, COL_GOLD, (mCounter * 2));

			if (lc->mBestTime == 0xffff)
				sprintf(text, "--:--.--");
			else
			{
				SINT	minutes = lc->mBestTime / 60 / 25;
				SINT	seconds = (lc->mBestTime / 25) % 60;
				SINT	milli   = (lc->mBestTime % 25) * 4;

				sprintf(text, "%d:%02d.%02d", minutes, seconds, milli);
			}
			sl = GetStringSize(2, text);
			DrawDynamicText(2, text, 144 - sl, 146, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}

		// Number of Attempts
		CText::GetString(TEXT_NUM_ATTEMPTS, text);
		sl = GetStringSize(2, text);
		DrawDynamicText(2, text, 144 - sl, 162, COL_GOLD, (mCounter * 2) - 3);

		if (lc->mNumAttempts == 255)
		{
			CText::GetString(TEXT_LOTS, text);
			sl = GetStringSize(2, text);
			DrawDynamicText(2, text, 144 - sl, 178, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}
		else
		{
			sprintf(text, "%d", lc->mNumAttempts);
			sl = GetStringSize(2, text);
			DrawDynamicText(2, text, 144 - sl, 178, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}

		if ((mMode != FEPS_QSAVE) && (mMode != FEPS_MESSAGE))
		{
#define AR_BOT_Y	210

			// How to get gold
			CText::GetString(TEXT_GOLD, text);
			DrawDynamicText(2, text, 30, AR_BOT_Y, COL_GOLD, (mCounter * 2) - 6 );
			sl = GetStringSize(2, text); // for below

			f = 2;
			switch (li->mGold)
			{
				case LIFIN_SURVIVE:
					CText::GetString(TEXT_LIFIN_SURVIVE, text);
					break;
				case LIFIN_WIN:
					CText::GetString(TEXT_LIFIN_WIN, text);
					break;
				case LIFIN_WIN2:
					CText::GetString(TEXT_LIFIN_WIN2, text);
					break;
				case LIFIN_FINISH:
					CText::GetString(TEXT_LIFIN_FINISH, text);
					break;
				case LIFIN_PAR:
					CText::GetString(TEXT_LIFIN_UNDERPAR, text);
					break;
				default:
					{
						char	t2[100];
						CText::GetString(TEXT_LIFIN_BEAT, t2);

						SINT	minutes = li->mGold / 60 / 25;
						SINT	seconds = (li->mGold / 25) % 60;
						SINT	milli   = (li->mGold % 25) * 4;

						sprintf(text, "%s %d:%02d.%02d", t2, minutes, seconds, milli);

						f = 2;
					}
					break;
			};
			DrawDynamicText(f, text, 30 + sl + 20, AR_BOT_Y, COL_BLUE, (mCounter - mArenaSlideStart) * 2);


			// Lost Toys Best Time
			if ((lc->mFinished == ECL_GOLD)  || (lc->mFinished == ECL_LT))
			{
				if (li->mType == LILT_GOLF)
					CText::GetString(TEXT_LOST_SCORE, text);
				else
					CText::GetString(TEXT_LOST_TOYS_TIME, text);
				DrawDynamicText(2, text, 290, AR_BOT_Y, COL_GOLD, (mCounter * 2) - 6 );
				sl = GetStringSize(2, text); // for below

				f = 2;
				switch (li->mLostToysTime)
				{
					case LIFIN_SURVIVE:
						CText::GetString(TEXT_LIFIN_SURVIVE, text);
						break;
					case LIFIN_WIN:
						CText::GetString(TEXT_LIFIN_WIN, text);
						break;
					case LIFIN_WIN2:
						CText::GetString(TEXT_LIFIN_WIN2, text);
						break;
					case LIFIN_FINISH:
						CText::GetString(TEXT_LIFIN_FINISH, text);
						break;
					case LIFIN_PAR:
						CText::GetString(TEXT_LIFIN_3UNDERPAR, text);
						break;

					default:
						{
	//						char	t2[100];
	//						CText::GetString(TEXT_LIFIN_BEAT, t2);

							SINT	minutes = li->mLostToysTime / 60 / 25;
							SINT	seconds = (li->mLostToysTime / 25) % 60;
							SINT	milli   = (li->mLostToysTime % 25) * 4;

							sprintf(text, "%d:%02d.%02d", minutes, seconds, milli);

							f = 2;
						}
						break;
				};
				DrawDynamicText(f, text, 290 + sl + 20, AR_BOT_Y, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
			}
		}
			
		// CODE??
		if (lc->mFinished == ECL_LT)
		{
			sprintf(text, "%08X", get_code2(lc->mBestTime, (prison << 4) + mArena, CAREER.mCodeKey, prison));
			DrawDynamicText(f, text, 363, 178, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}				

		SINT	lev_text = TEXT_LT_KING_OF_THE_HILL;
		switch(li->mType)
		{
		// could do something with + !
		case LILT_KING_OF_THE_HILL:		lev_text = TEXT_LT_KING_OF_THE_HILL;	break;
		case LILT_LAST_MAN_ROLLING:		lev_text = TEXT_LT_LAST_MAN_ROLLING;	break;
		case LILT_RUN_THE_GAUNTLET:		lev_text = TEXT_LT_RUN_THE_GAUNTLET;	break;
		case LILT_PURSUIT:				lev_text = TEXT_LT_PURSUIT;				break;
		case LILT_RACE:					lev_text = TEXT_LT_RACE;				break;
		case LILT_TRICK:				lev_text = TEXT_LT_TRICK;				break;
		case LILT_POWERBALL:			lev_text = TEXT_LT_POWERBALL;			break;
		case LILT_GOLF:					lev_text = TEXT_LT_GOLF;				break;
		case LILT_PINBALL:				lev_text = TEXT_LT_PINBALL;				break;
		case LILT_SBALL:				lev_text = TEXT_LT_SBALL;				break;
		};

		CText::GetString(lev_text, text);
		sl = GetStringSize(2, text);
		DrawDynamicText(2, text, 256 - sl / 2, 146, COL_GOLD, (mCounter - mArenaSlideStart) * 3);

		// Draw other stuff
		if (lc->mFinished == ECL_LT)
		{
			DrawSprite(FEDCPST_LOST_TIME  , 231, 168 + MEDALOFFSET, 0.8f);
			DrawSprite(FEDCPST_QUALIFY	, 280, 168 + MEDALOFFSET, 0.8f);
			DrawSprite(FEDCPST_GOLD   	, 183, 168 + MEDALOFFSET, 0.8f);
		}
		else
		{
			if ((lc->mFinished == ECL_YES) || (lc->mFinished == ECL_GOLD))
				DrawSprite(FEDCPST_QUALIFY	, 262, 168 + MEDALOFFSET, 0.8f);
			if ((lc->mFinished == ECL_GOLD))
				DrawSprite(FEDCPST_GOLD   	, 200, 168 + MEDALOFFSET, 0.8f);
		}
	}

	if (mMode == FEPS_ARENA && (mArenaPendingVictory == -1))
	{
		char text[200];

		CText::GetString(TEXT_HELPDC1, text);
		SINT	sl = GetStringSize(2, text);
			DrawString(2, text, 256 - sl / 2, 225 , 0x007f7f7f);
	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderPS2Game()
{
#ifdef JCLDCNOPSX  // motion blur out

	// Motion-blur / feedback thingy.
	NTAG				*o  = PLATFORM.GetOrderTable();
	POLY_FT4			*p4 = PLATFORM.GetMiscPolyFT4s();
	POLY_F4				*f4;
	DR_MODE				*drm;

	SINT	sx, sy;

	PLATFORM.GetScreenPos(sx, sy);

	if (sy == 0)
		sy = 256;
	else
		sy = 0;

	f4 = (POLY_F4 *)p4;
	setPolyF4(f4);
	if (mCounter < PS2GAME_W2B)
		setRGB0(f4, 20, 20, 20);
	else
		setRGB0(f4, 50, 50, 50);
	setXYWH(f4, 0, 0, 512, 256);
	setSemiTrans(f4, TRUE);
	addPrim(&o[1], f4);
	f4 ++;

	drm = (DR_MODE *)f4;
	if (mCounter < PS2GAME_W2B)
		SetDrawMode(drm, 1, 1, getTPage(0, 1, 0, 0), NULL);
	else
		SetDrawMode(drm, 1, 1, getTPage(0, 2, 0, 0), NULL);
	addPrim(&o[1], drm);
	drm ++;

	p4 = (POLY_FT4 *)drm;
	setPolyFT4(p4);
	setRGB0(p4, 150, 150, 150);
	setXY4(p4, 0, 0, 259, 0, 0, 125, 256, 128);
	setUVWH(p4, 5, 5, 250, 123);
	setSemiTrans(p4, TRUE);
	setTPage(p4, 2, 0, sx, sy);
	addPrim(&o[1], p4);
	p4 ++;

	setPolyFT4(p4);
	setRGB0(p4, 150, 150, 150);
	setXY4(p4, 0, 125, 256, 128, 0, 256, 253, 256);
	setUVWH(p4, 5, 128, 250, 123);
	setSemiTrans(p4, TRUE);
	setTPage(p4, 2, 0, sx, sy);
	addPrim(&o[1], p4);
	p4 ++;

	setPolyFT4(p4);
	setRGB0(p4, 150, 150, 150);
	setXY4(p4, 259, 0, 512, 0, 256, 128, 512, 131);
	setUVWH(p4, 0, 5, 250, 123);
	setSemiTrans(p4, TRUE);
	setTPage(p4, 2, 0, sx + 256, sy);
	addPrim(&o[1], p4);
	p4 ++;

	setPolyFT4(p4);
	setRGB0(p4, 150, 150, 150);
	setXY4(p4, 256, 128, 512, 131, 253, 256, 512, 256);
	setUVWH(p4, 0, 128, 250, 123);
	setSemiTrans(p4, TRUE);
	setTPage(p4, 2, 0, sx + 256, sy);
	addPrim(&o[1], p4);
	p4 ++;

	PLATFORM.DeclareUsedMiscPolyFT4s(p4);
#endif
}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderQSave()
{
	SINT	yps;
	GINT	gy = G1;

	SINT	c0;
	SINT	y = 126;

	for (c0 = 0; c0 < 3; c0 ++)
	{
		SINT 	c1;
		SINT	x = 112 - 24;

		GINT	gx;
		SINT	xoffs;

		BOOL	left;

		left = (mMode == FEPS_QSAVE2SELMEM) || (mMode == FEPS_QSAVE2ARENA);

		gx = G(SLIDE_TIME - mQSaveMenuSlide) - (G(left ? c0 : (4 - c0)) * G(1, 30000));

		if (gx < G0)
			gx = G0;
		else
		{
			gx = (gx * gx) / SLIDE_TIME;
			gx = (gx * 2600) / SLIDE_TIME;

			if (left)
				gx = -gx;
		}
		xoffs = gx.Whole();

		if (xoffs < 1000)
		{
			x += xoffs;

			// draw text
			char	s[100];
			
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_DC_SAVING1, s); break;
			case 1:	CText::GetString(TEXT_DC_SAVING2, s); break;
			case 2:	CText::GetString(TEXT_DC_SAVING3, s); break;
			};

			SINT	tcol;

			if (c0 == 0)
				tcol = 0x007f7f7f;
			else
				tcol = 0x002f5f7f;

			SINT	sl = GetStringSize(0, s);
			DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol, 0);
			DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000, 0);

			// Draw Bars
			SINT	colr = 60, colg = 72, colb = 100;

			 if (c0 == 0)
			{
				colr = 80;
				colg = 100;
				colb = 60;
			}

			DrawSprite(FEDCPST_BOXL, x    , y    , 0.6f, colr, colg, colb, 2);
			x += 48;

			for (c1 = 0; c1 < 5; c1 ++)
			{
				DrawSprite(FEDCPST_BOXM, x    , y    , 0.6f, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEDCPST_BOXR, x    , y    , 0.6f, colr, colg, colb, 2);
		}
		y += 30;
	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderMessage()
{
	SINT	yps;
	GINT	gy = G1;

	SINT	c0;
	SINT	y = 126;

	for (c0 = 0; c0 < 3; c0 ++)
	{
		SINT 	c1;
		SINT	x = 64;

		GINT	gx;
		SINT	xoffs;

		BOOL	left;

		left = mMode == FEPS_MESSAGE2ARENA;

		gx = G(SLIDE_TIME - mMessageMenuSlide) - (G(left ? c0 : (4 - c0)) * G(1, 30000));

		if (gx < G0)
			gx = G0;
		else
		{
			gx = (gx * gx) / SLIDE_TIME;
			gx = (gx * 2600) / SLIDE_TIME;

			if (left)
				gx = -gx;
		}
		xoffs = gx.Whole();

		if (xoffs < 1000)
		{
			x += xoffs;

			// draw text
			char	s[100];
			switch (c0)
			{
			case 0:
				{
					SINT	tcol  = 0x007f7f7f;

					if (mMessage == 13)
						CText::GetString(TEXT_ALL_GOLD1, s);
					else
						CText::GetString(TEXT_CONGRATS1, s);

					SINT	sl = GetStringSize(2, s);
					DrawString(2, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol, 0);
					DrawString(2, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000, 0);
				}
				break;
			case 1:
				{
					SINT	tcol = 0x000f5f7f;

					if (mMessage < 11)
						CText::GetString(TEXT_PRN_ECO + mMessage, s);
					else if (mMessage == 11)
						CText::GetString(TEXT_BONUS_LEVEL, s);
					else if (mMessage == 12)
						CText::GetString(TEXT_BONUS_CHARACTER, s);
					else if (mMessage == 13)
						CText::GetString(TEXT_ALL_GOLD2, s);

					SINT	sl = GetStringSize(2, s);

					DrawString(2, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol, 0);
					DrawString(2, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000, 0);

				}
				break;
			case 2:
				{
					CText::GetString(TEXT_CONTINUE, s);
					SINT	tcol = 0x007f7f7f;

					SINT	sl = GetStringSize(0, s);
					DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol, 0);
					DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000, 0);
				}
				break;
			};


			// Draw Bars
			SINT	colr = 60, colg = 72, colb = 100;

			if ((c0 == 0) || (c0 == 1))
			{
				colr = 80;
				colg = 100;
				colb = 60;
			}
			else
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr / 4;
			}

			DrawSprite(FEDCPST_BOXL, x    , y    , 0.6f, colr, colg, colb, 2);
			x += 48;

			for (c1 = 0; c1 < 6; c1 ++)
			{
				DrawSprite(FEDCPST_BOXM, x    , y    , 0.6f, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEDCPST_BOXR, x    , y    , 0.6f, colr, colg, colb, 2);

		}
		y += 30;
	}

	if (mMode == FEPS_MESSAGE)
	{
		char text[200];

		CText::GetString(TEXT_HELPDC2, text);
		SINT	sl = GetStringSize(2, text);
		DrawString(2, text, 256 - sl / 2, 230 , 0x007f7f7f);
	}

}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderMChar()
{
	SINT	c;
	c = (mMCharMenuSlide * 128) / SLIDE_TIME;

	// draw the character meshes
	MATRIX lcol;

	// setup the camera
	GINT 	gy;

	gy = G(mMCharMenuSlide);
	gy = (gy * gy) / (SLIDE_TIME * SLIDE_TIME);
	gy = -G(15 + 75) + (gy * 75);

	mCamera.mPos = GVector(G0, gy, -G(2, 32768));
	mCamera.mLookAt = GVector(G0, 0, -G(2, 32768));
	mCamera.mMode = CCamera::LOOKAT;
	mCamera.mYaw = G0;
	mCamera.mPitch = G0;
	mCamera.FillOut();

	// Draw the Characters
	SINT 	cn;
	for (cn = 0; cn < 2; cn ++)
	{
		// set mesh position and orientation
		CDCMesh	*m;

		// which meshes to draw, and where
		SINT	character = mMCharacter[cn] + mMCharSlide[cn].Whole();

		while (character < 0)
		{
			character += 6;
		};

		while (character > 5)
		{
			character -= 6;
		};

		GINT	gpos = (mMCharSlide[cn] + G(100)).GFrac();   // won't work with negative numbers
		gpos = -gpos;

		if (mMCharSlide[cn] != G0)
		{
			m = ENGINE.GetNumberedMesh(FEPSM_ANGEL + character);
			if (m)
			{
				CFrontEndThing	*t = &mMCharThings[cn][character];

				t->mBlack = ((!can_select_doctor()) && (character == 5));
/*				if ((!can_select_doctor()) && (character == 5))
				{
					SetMLight(lcol.m,0, -2.0, -2.0, -2.0);
					SetMLight(lcol.m,1, -2.0, -2.0, -2.0);
					SetMLight(lcol.m,2, -2.0, -2.0, -2.0);
				}
				else
				{
					SetMLight(lcol.m,0, 1.99 , 0.40, 0.10);
					SetMLight(lcol.m,1, 0.20 , 0.20 , 1.20);
					SetMLight(lcol.m,2, 1.95 , 1.95 , 1.95);
				}
				SetColorMatrix(&lcol);
*/
				if (((cn == 0) && (gpos <= G0)) ||
					((cn == 1) && (gpos >= G0)))
					t->mPos = GVector(gpos * G(12) + ((cn == 0) ? -G(2) : G(2)), G(8, 0), G(0, 0));
				else if (cn == 0)
					t->mPos = GVector(-G(2), G(8, 0), G(0, 0) + gpos * G(15));
				else
					t->mPos = GVector( G(2), G(8, 0), G(0, 0) - gpos * G(15));

				SRTInfo	ri;
				ri.Clear();
				ri.i0 = FEPSM_ANGEL + character + 10000;
				ri.i1 = -1;
				ri.i2 = -2;

				CDCMesh::RenderFromData(t, &ri, FALSE);
			}
			character = (character + 1) % 6;
			gpos = gpos + G1;
		}

		m = ENGINE.GetNumberedMesh(FEPSM_ANGEL + character);
		if (m)
		{
#ifdef JCLDCNOPSX // lighting
			if ((!can_select_doctor()) && (character == 5))
			{
				SetMLight(lcol.m,0, -2.0, -2.0, -2.0);
				SetMLight(lcol.m,1, -2.0, -2.0, -2.0);
				SetMLight(lcol.m,2, -2.0, -2.0, -2.0);
			}
			else
			{
				SetMLight(lcol.m,0, 1.99 , 0.40, 0.10);
				SetMLight(lcol.m,1, 0.20 , 0.20 , 1.20);
				SetMLight(lcol.m,2, 1.95 , 1.95 , 1.95);
			}
			SetColorMatrix(&lcol);
#endif

			CFrontEndThing	*t = &mMCharThings[cn][character];

			if (((cn == 0) && (gpos <= G0)) ||
				((cn == 1) && (gpos >= G0)))
				t->mPos = GVector(gpos * G(12) + ((cn == 0) ? -G(2) : G(2)), G(8, 0), G(0, 0));
			else if (cn == 0)
				t->mPos = GVector(-G(2), G(8, 0), G(0, 0) + gpos * G(15));
			else
				t->mPos = GVector( G(2), G(8, 0), G(0, 0) - gpos * G(15));

			SRTInfo	ri;
			ri.Clear();
			ri.i0 = FEPSM_ANGEL + character + 10000;
			ri.i1 = -1;
			ri.i2 = -2;

			CDCMesh::RenderFromData(t, &ri, FALSE);
		}
	}

	if (mMode == FEPS_MCHAR)
	{
		// draw arrows?
		if ((mArrowCounter & 31) < 20)
		{
			if (!mMCharSelected[0])
			{
				DrawSprite(FEDCPST_ARROW_L, 183, 190 + ARROWOFFSET, 0, 127, 127, 127, 2);
				DrawSprite(FEDCPST_ARROW_R, 225, 190 + ARROWOFFSET, 0, 127, 127, 127, 2);
			}
			if (!mMCharSelected[1])
			{
				DrawSprite(FEDCPST_ARROW_L, 277, 190 + ARROWOFFSET, 0, 127, 127, 127, 2);
				DrawSprite(FEDCPST_ARROW_R, 319, 190 + ARROWOFFSET, 0, 127, 127, 127, 2);
			}
		}
	}

	// draw some text boxes
	SINT	col = c / 3;
	GINT	gh = G(mMCharMenuSlide) / SLIDE_TIME;
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;
	SINT	h = (gh * 130).Whole();

	DrawBox(16 , 100, 150, h, 3, col, col, col);
	DrawBox(346, 100, 150, h, 3, col, col, col);

	// and some assorted text
	char	text[100];
	SINT	sl;

	if (mMode == FEPS_MCHAR)
	{
		//************
		//** P1

		// name
		CText::GetString(TEXT_ANGEL + mMCharacter[0], text);
		sl = GetStringSize(0, text);
		DrawString(0, text, 83 - sl / 2, 110, 0x003f5f7f);

		// stats
		CText::GetString(TEXT_STRENGTH, text);
		DrawDynamicText(2, text, 35, 140, 48, 92, 48, (mCounter * 2)    );
		CText::GetString(TEXT_SPEED, text);
		DrawDynamicText(2, text, 35, 156, 48, 92, 48, (mCounter * 2) - 3);
		CText::GetString(TEXT_AGILITY, text);
		DrawDynamicText(2, text, 35, 172, 48, 92, 48, (mCounter * 2) - 6);
		CText::GetString(TEXT_STAMINA, text);
		DrawDynamicText(2, text, 35, 188, 48, 92, 48, (mCounter * 2) - 9);

		sprintf(text, "````");
		text[get_char_stats(mMCharacter[0], 0)] = 0;
		DrawDynamicText(0, text, 130, 140, 128, 32, 32, (mCounter - mMCharSlideStart[0]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[0], 1)] = 0;
		DrawDynamicText(0, text, 130, 156, 128, 32, 32, (mCounter - mMCharSlideStart[0]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[0], 2)] = 0;
		DrawDynamicText(0, text, 130, 172, 128, 32, 32, (mCounter - mMCharSlideStart[0]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[0], 3)] = 0;
		DrawDynamicText(0, text, 130, 188, 128, 32, 32, (mCounter - mMCharSlideStart[0]));


		//************
		//** P2

		// name
		CText::GetString(TEXT_ANGEL + mMCharacter[1], text);
		sl = GetStringSize(0, text);
		DrawString(0, text, 421 - sl / 2, 110, 0x003f5f7f);

		// stats
		CText::GetString(TEXT_STRENGTH, text);
		DrawDynamicText(2, text, 355, 140, 48, 92, 48, (mCounter * 2)    );
		CText::GetString(TEXT_SPEED, text);
		DrawDynamicText(2, text, 355, 156, 48, 92, 48, (mCounter * 2) - 3);
		CText::GetString(TEXT_AGILITY, text);
		DrawDynamicText(2, text, 355, 172, 48, 92, 48, (mCounter * 2) - 6);
		CText::GetString(TEXT_STAMINA, text);
		DrawDynamicText(2, text, 355, 188, 48, 92, 48, (mCounter * 2) - 9);

		sprintf(text, "````");
		text[get_char_stats(mMCharacter[1], 0)] = 0;
		DrawDynamicText(0, text, 450, 140, 128, 32, 32, (mCounter - mMCharSlideStart[1]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[1], 1)] = 0;
		DrawDynamicText(0, text, 450, 156, 128, 32, 32, (mCounter - mMCharSlideStart[1]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[1], 2)] = 0;
		DrawDynamicText(0, text, 450, 172, 128, 32, 32, (mCounter - mMCharSlideStart[1]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[1], 3)] = 0;
		DrawDynamicText(0, text, 450, 188, 128, 32, 32, (mCounter - mMCharSlideStart[1]));


		// Chosen?
		CText::GetString(TEXT_CHOSEN, text);
		sl = GetStringSize(2, text);
		if (mMCharSelected[0])
			DrawString(2, text, 256 - 45 - sl / 2, 175, 0x003f5f7f);
			
		// JCL - don't ask!  Fixes shadow bug...
		DrawString(2, " ", 256 - 45 - sl / 2, 175, 0x003f5f7f);
		
		if (mMCharSelected[1])
			DrawString(2, text, 256 + 45 - sl / 2, 175, 0x003f5f7f);
	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderMTourn()
{
	// which Tournaments to draw, and where
	SINT	tournament = mMTournament + mMTournamentSlide.Whole();

//	if (mMTournamentSlide >= G0) tournament -= 1;
	tournament -= 1;

	GINT	gpos = (mMTournamentSlide + G(100)).GFrac();   // won't work with negative numbers
	gpos = -gpos;
	gpos -= G1;

	GINT	gh = G(mMTournamentMenuSlide) / SLIDE_TIME;
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;

	SINT	number = 4;
	SINT	c0;

	for (c0 = 0; c0 < number; c0 ++)
	{
		if ((tournament >= 0) && (tournament < CLevelStructure::GetNumMultiplayerTournaments()))
		{
			#define TOURN_WIDTH		400
			#define TOURN_HEIGHT	150
			#define TOURN_GAP		20
			#define TOURN_Y			90

			SINT	tx = (gpos * G(TOURN_WIDTH + TOURN_GAP)).Whole() + ((PLATFORM.ScreenWidth() / 2) - (TOURN_WIDTH / 2));
			SINT	ty = 85 + ((G1 - gh) * 350).Whole();
			SINT	txc = tx + TOURN_WIDTH / 2;

			// Draw the border
			DrawBox(tx, ty, TOURN_WIDTH, TOURN_HEIGHT, 2, 40, 40, 40);

			// And the contents
			STournamentInfo		*ti = CLevelStructure::GetTournamentInfo(tournament);
			char	text[200];
			SINT	sl, c1;
			SINT	numl = ti->mNumLevels;

			// Name
			CText::GetString(ti->mName, text);
			sl = GetStringSize(0, text);
			DrawString(0, text, txc - sl / 2, ty + 10, 0x003f5f7f);

			// the Arenas
			SINT	total_width = (42 * (numl - 1)) + (7 * (numl - 1));
			SINT	x, y, x2, y2;

			x = txc - (total_width / 2) - 24;
			y = ty + 35 - 2 - 4;

			for (c1 = 0; c1 < numl; c1 ++)
			{

				DrawSprite(FEDCPST_LT_KING_OF_THE_HILL + ti->mLevels[c1].mType, x, y, 1, 127, 127, 127, 2 , FALSE);
				x += 49;
			}

			// draw some vertical lines
			x = txc - (total_width / 2) - 21 - 1;
			y = ty + 27;
			for (c1 = 0; c1 < numl + 1; c1 ++)
			{
				DrawLine(x, y - 5, x, y + 94, 2, 128, 128, 32);
				x += 49;
			}
			DrawLine(x, ty + 68 - 5, x, y + 94, 2, 128, 128, 32);

			// draw some horizontal lines
			x = txc - (total_width / 2) - 21 - 1 - 70;
			x2 = x + total_width + 49 + 49 + 70 + 5;

			DrawLine(x + 65, ty + 32 -  5, x2 - 49, ty + 32 -  5, 2, 128, 128, 32);
			DrawLine(x     , ty + 68     , x2     , ty + 68     , 2, 128, 128, 32);
			DrawLine(x     , ty + 68 + 24, x2     , ty + 68 + 24, 2, 128, 128, 32);
			DrawLine(x     , ty + 68 + 48, x2     , ty + 68 + 48, 2, 128, 128, 32);

			DrawLine(x + 5, ty + 68 - 5, x + 5, ty + 68 + 48 + 5, 2, 128, 128, 32);

			// more text
			CText::GetString(TEXT_1UP, text);
			sl = GetStringSize(0, text);
			DrawString(0, text, x + 60 - sl - 4, ty + 68 + 5      + 2, 0x007f3f5f);
			CText::GetString(TEXT_2UP, text);
			sl = GetStringSize(0, text);
			DrawString(0, text, x + 60 - sl - 4, ty + 68 + 5 + 24 + 2, 0x007f3f5f);


			// Hokay - do we need to show some results?
			if (tournament == mMTournament)
			{
				x = txc - (total_width / 2) - 21 + 2 - 4;

				for (c1 = 0; c1 < mMTournamentResultsShowLW; c1 ++)
				{
					SINT	res = MULTIPLAYER_CAREER.mWhoWonWhichLevel[c1];
					SINT	py = -1;
					switch (res)
					{
						case 0:
							py = ty + 68 - 4 + 4;
							break;
						case 1:
							py = ty + 68 + 24 - 4 + 4;
							break;
					};
					if (py != -1)
					{
						DrawSprite(FEDCPST_QUALIFY, x, py + MEDALOFFSET, 1);

						if (mMTournamentResultsTimer == 0)
						{
							if (c1 == mMTournamentResultsShowLW - 1)
							{
								if (!mMTournamentResultsShowWinner)
								{
									// emit some particles!
									AddParticles(x + 20, py + 12, NUM_PSXFE_PARTICLES / 2);
								}
							}
						}
					}
					else
					{
						// draw!
						DrawSprite(FEDCPST_DRAW, x, ty + 68      - 4  + 4 + MEDALOFFSET, 1.1f);
						DrawSprite(FEDCPST_DRAW, x, ty + 68 + 24 - 4  + 4 + MEDALOFFSET, 1);
					}
					x += 49;
				}

				if (mMTournamentResultsShowWinner)
				{
					SINT	sc1 = 0, sc2 = 0;

					for (c1 = 0; c1 < numl; c1 ++)
					{
						SINT	res = MULTIPLAYER_CAREER.mWhoWonWhichLevel[c1];
						if (res == 0) sc1 ++;
						if (res == 1) sc2 ++;
					}

					if (sc1 > sc2)
					{
						DrawSprite(FEDCPST_QUALIFY, x, ty + 68 - 4 + 4+ MEDALOFFSET, 1);
						// emit some particles!
						if (mMTournamentResultsTimer == 0)
							AddParticles(x + 20, ty + 68 - 4 + 12, NUM_PSXFE_PARTICLES / 2);

						CText::GetString(TEXT_P1WINS, text);
						sl = GetStringSize(0, text);
						DrawString(0, text, txc - sl / 2, ty + 68 + 48 + 10, 0x003f7f5f);
					}
					else if (sc1 < sc2)
					{
						DrawSprite(FEDCPST_QUALIFY, x, ty + 68 + 24 - 4 + 4 + MEDALOFFSET, 1);
						// emit some particles!
						if (mMTournamentResultsTimer == 0)
							AddParticles(x + 20, ty + 68 + 24 - 4 + 12, NUM_PSXFE_PARTICLES / 2);

						CText::GetString(TEXT_P2WINS, text);
						sl = GetStringSize(0, text);
						DrawString(0, text, txc - sl / 2, ty + 68 + 48 + 10, 0x003f7f5f);
					}
					else
					{
						// draw
						DrawSprite(FEDCPST_DRAW, x, ty + 68      - 4 + 4 + MEDALOFFSET, 1.1f);
						DrawSprite(FEDCPST_DRAW, x, ty + 68 + 24 - 4 + 4 + MEDALOFFSET, 1);

						CText::GetString(TEXT_TOURNAMENT_DRAWN, text);
						sl = GetStringSize(0, text);
						DrawString(0, text, txc - sl / 2, ty + 68 + 48 + 10, 0x003f7f5f);
					}
				}
			}
		}
		tournament = (tournament + 1);
		gpos = gpos + G1;
	}

	if ((mMode == FEPS_MTOURN) && (mMTournamentSlide == G0))
	{
		// draw arrows?
		if ((mArrowCounter & 31) < 20)
		{
			if (mMTournament != 0)
			{
				DrawSprite(FEDCPST_ARROW_L, 70, 100 + ARROWOFFSET, 0, 127, 127, 127, 2);
				DrawSprite(FEDCPST_ARROW_L, 70, 210 + ARROWOFFSET, 0, 127, 127, 127, 2);
			}
			if (mMTournament != CLevelStructure::GetNumMultiplayerTournaments() - 1)
			{
				DrawSprite(FEDCPST_ARROW_R, 432, 100 + ARROWOFFSET, 0, 127, 127, 127, 2);
				DrawSprite(FEDCPST_ARROW_R, 432, 210 + ARROWOFFSET, 0, 127, 127, 127, 2);
			}
		}
	}

}


//******************************************************************************************
void	CDCFEPrisonSelect::RenderOptions()
{
	SINT	yps;
	GINT	gy = G1;

	SINT	c0;

	for (c0 = 4; c0 >= 0; c0 --)
	{
		SINT	y = 74 + (c0 * 30);

		SINT 	c1;
		SINT	x = 112;

		GINT	gx;
		SINT	xoffs;

		BOOL	left;

		left = (mMode == FEPS_OPTIONS2SELMEM) || (mMode == FEPS_SELMEM2OPTIONS) ||
			   (mMode == FEPS_OPTIONS2CONTROLLER) || (mMode == FEPS_CONTROLLER2OPTIONS);

		gx = G(SLIDE_TIME - mOptionsMenuSlide) - (G(left ? c0 : (6 - c0)) * G(1, 00000));

		if (gx < G0)
			gx = G0;
		else
		{
			gx = (gx * gx) / SLIDE_TIME;
			gx = (gx * 2600) / SLIDE_TIME;

			if (left)
				gx = -gx;
		}
		xoffs = gx.Whole();

		if (xoffs < 1000)
		{
			x += xoffs;

			SINT	colr = 60, colg = 72, colb = 100;

			if (mOptionsMenuPos == c0)
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr / 4;
			}

			DrawSprite(FEDCPST_BOXL, x    , y    , 2, colr, colg, colb, 2);
			x += 48;

			for (c1 = 0; c1 < 4; c1 ++)
			{
				DrawSprite(FEDCPST_BOXM, x    , y    , 2, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEDCPST_BOXR, x    , y    , 2, colr, colg, colb, 2);

			// draw text
			char	s[100];
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_CONTROLLER_SETUP, s); break;
			case 1:	CText::GetString(TEXT_SOUND_VOLUME, s); break;
			case 2:	CText::GetString(TEXT_MUSIC_VOLUME, s); break;
			case 3:	CText::GetString(TEXT_STEREO_SOUND, s); break;
			case 4:	CText::GetString(TEXT_DC_SELECT_MEMORY_CARD, s); break;
			};

			SINT	tcol;

			if (mOptionsMenuPos == c0)
				tcol = 0x007f7f7f;
			else
				tcol = 0x005f5f5f;

			if ((c0 == 0) || (c0 == 4))
			{
				SINT font = 0;

				if ((c0 == 0) && (GAME.GetLanguage() != 0))
					font = 2;

				SINT	sl = GetStringSize(font, s);
				DrawString(font, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol);
				DrawString(font, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000);
			}
			else if ((c0 == 1) || (c0 == 2))
			{
				DrawString(2, s, 160 + xoffs    , y + 4 + MENUTEXTOFFSET, tcol, 0);
				DrawString(2, s, 160 + xoffs + 1, y + 5 + MENUTEXTOFFSET, 0, 0);

				SINT	val;
				if (c0 == 1)
					val = CAREER.mSoundVolume;
				else
					val = CAREER.mMusicVolume;

				sprintf(s, "``````````");
				s[val] = 0;

				SINT	sl = GetStringSize(0, s);
				DrawString(0, s, 290 + xoffs    , y + 4 + MENUTEXTOFFSET, 0x000f0f7f, 0);
				DrawString(0, s, 290 + xoffs + 1, y + 5 + MENUTEXTOFFSET, 0, 0);

				sprintf(s, "``````````");
				s[10 - val] = 0;
				DrawString(0, s, 290 + xoffs + sl    , y + 4 + MENUTEXTOFFSET, 0x002f2f2f, 0);
				DrawString(0, s, 290 + xoffs + sl + 1, y + 5 + MENUTEXTOFFSET, 0, 0);

				// draw arrows?
				if (mOptionsMenuPos == c0)
				{
	//				if ((mCounter & 31) < 20)
					{
						ENGINE.DrawSprite(FEDCPST_ARROW_L, 270 + xoffs, y + 3 + ARROWOFFSET, 0, 127, 127, 127, 2);
						ENGINE.DrawSprite(FEDCPST_ARROW_R, 370 + xoffs, y + 3 + ARROWOFFSET, 0, 127, 127, 127, 2);
					}
				}
			}
			else if (c0 == 3)
			{
				DrawString(2, s, 160 + xoffs    , y + 4 + MENUTEXTOFFSET, tcol, 0);
				DrawString(2, s, 160 + xoffs + 1, y + 5 + MENUTEXTOFFSET, 0, 0);

				DWORD	col;

				CText::GetString(TEXT_OFF, s);
				col = (!mOptionsStereo) ? 0x000f0f7f : 0x003f3f3f;

				DrawString(2, s, 290 + xoffs    , y + 4 + MENUTEXTOFFSET, col, 0);
				DrawString(2, s, 290 + xoffs + 1, y + 5 + MENUTEXTOFFSET, 0, 0);

				CText::GetString(TEXT_ON, s);
				col = mOptionsStereo ? 0x003f7f3f : 0x003f3f3f;

				DrawString(2, s, 290 + 35 + xoffs    , y + 4 + MENUTEXTOFFSET, col, 0);
				DrawString(2, s, 290 + 35 + xoffs + 1, y + 5 + MENUTEXTOFFSET, 0, 0);

				// draw arrows?
				if (mOptionsMenuPos == c0)
				{
	//				if ((mCounter & 31) < 20)
					{
						ENGINE.DrawSprite(FEDCPST_ARROW_L, 270 + xoffs, y + 3 + ARROWOFFSET, 0, 127, 127, 127, 2);
						ENGINE.DrawSprite(FEDCPST_ARROW_R, 370 + xoffs, y + 3 + ARROWOFFSET, 0, 127, 127, 127, 2);
					}
				}
			}
		}
	}

	if (mMode == FEPS_OPTIONS)
	{
		char text[200];

		CText::GetString(TEXT_HELPDC1, text);
		SINT	sl = GetStringSize(2, text);
		DrawString(2, text, 256 - sl / 2, 230 , 0x007f7f7f);
	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderCredits()
{
	if (mMode != FEPS_CREDITS)
		return;

	#define	LINE_SPACE	18
	#define SECTION_SPACE	15

	SINT	y = 260 - mCreditsPos;
	SINT	tn = 0;

	char	l[5][200];

	ECreditType type;
	SINT 		title;
	char		text[200];
	SINT		sl;
	DWORD		tcol;

#if TARGET == DC
	while ((y < 256) && (tn < 27))
#else
	while ((y < 256) && (tn < 32))
#endif
	{
		CCredits::GetCredit(tn, &type, &title, l[0], l[1], l[2], l[3], l[4]);

		// draw title
		CText::GetString(title, text);
		sl = GetStringSize(0, text);
		if (type == CRED_NORMAL)
			tcol = 0x007f7f7f;
		else
			tcol = 0x00005f7f;
		if (y > -50)
		{
			DrawString(0, text, 256 - sl / 2    , y    , tcol, 5);
			DrawString(0, text, 256 - sl / 2 + 1, y + 1, 0   , 5);
		}

		y += LINE_SPACE;

		SINT	c0;

		for (c0 = 0; c0 < 5; c0 ++)
		{
			if (l[c0][0])
			{
				SINT 	font = 0;
				if (tn == 10)	font = 2;
				sl = GetStringSize(font, l[c0]);
				if (y > -50)
				{
					DrawString(font, l[c0], 256 - sl / 2    , y    , 0x001f7f1f, 5);
					DrawString(font, l[c0], 256 - sl / 2 + 1, y + 1, 0         , 5);
				}
				y += LINE_SPACE;
			}
		}

		y += SECTION_SPACE;

		tn ++;
	};
}

//******************************************************************************************
char	get_controller_char(SINT config, SINT action)
{
	switch (config)
	{
		case 0:
			switch (action)
			{
			case 0:	return '^';
			case 1:	return ']';
			case 2:	return '\\';
			case 3:	return '[';
			case 4:	return '}';
			};
			break;
		case 1:
			switch (action)
			{
			case 0:	return ']';
			case 1:	return '^';
			case 2:	return '[';
			case 3:	return '\\';
			case 4:	return '}';
			};
			break;
		case 2:
			switch (action)
			{
			case 0:	return '}';
			case 1:	return '^';
			case 2:	return '[';
			case 3:	return '\\';
			case 4:	return ']';
			};
			break;
	};

	return '!';
}

void	CDCFEPrisonSelect::RenderController()
{
	SINT	c0;

	for (c0 = 0; c0 < 4; c0 ++)
	{
		SINT 	c1;
		SINT	x = (c0 & 1) ? 264 : 8;
		GINT	gx;
		SINT	xoffs;

		BOOL	left = FALSE;

		gx = G(SLIDE_TIME - mControllerMenuSlide) - (G(left ? c0 : (2 - (c0 / 2))) * G(1, 30000));

		if (gx < G0)
			gx = G0;
		else
		{
			gx = (gx * gx) / SLIDE_TIME;
			gx = (gx * 1600) / SLIDE_TIME;

			if (left)
				gx = -gx;
		}
		xoffs = gx.Whole();

		if (xoffs < 500)
		{
			SINT	y = 77 + (28 * (c0 / 2));

			x += xoffs;

			SINT	colr = 60, colg = 72, colb = 100;

			if ((c0 & 1) && ((!(PLATFORM.IsControllerValid(1)))))
			{
				colr = colg = colb = 64;
			}

			DrawSprite(FEDCPST_BOXL, x    , y    , 2, colr, colg, colb, 2);
			x += 48;

			for (c1 = 0; c1 < 3; c1 ++)
			{
				DrawSprite(FEDCPST_BOXM, x    , y    , 2, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEDCPST_BOXR, x    , y    , 2, colr, colg, colb, 2);

			// draw text
			char	s[100];
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_PLAYER_1, s); break;
			case 1:	CText::GetString(TEXT_PLAYER_2, s); break;
			case 2:	CText::GetString(TEXT_SETTING, s); break;
			case 3:	CText::GetString(TEXT_SETTING, s); break;
			};

			SINT	tcol;

			if (c0 & 2)
				tcol = 0x00005f7f;
			else
				tcol = 0x005f5f5f;

			if ((c0 & 1) && ((!(PLATFORM.IsControllerValid(1)))))
				tcol = 0x004f4f4f;

			SINT	sl;

			SINT	xpos;
			if (c0 & 1)
				xpos = ((PLATFORM.ScreenWidth() * 3) / 4);
			else
				xpos = ((PLATFORM.ScreenWidth()    ) / 4);

			if (c0 & 2)
				xpos -= 7; // centre

			sl = GetStringSize(2, s);
			DrawString(2, s, xpos - sl / 2 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol);
			DrawString(2, s, xpos - sl / 2 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000);

			if (c0 == 2)
			{
				sprintf(s, "%d", mControllerSetting1 + 1);
				DrawString(2, s, xpos + sl / 2 + 8 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, 0x001f7f3f);
				DrawString(2, s, xpos + sl / 2 + 8 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000);
			}
			if (c0 == 3)
			{
				sprintf(s, "%d", mControllerSetting2 + 1);
				if ((!(PLATFORM.IsControllerValid(1))))
				{
					DrawString(2, s, xpos + sl / 2 + 8 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol);
					DrawString(2, s, xpos + sl / 2 + 8 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000);
				}
				else
				{
					DrawString(2, s, xpos + sl / 2 + 8 + xoffs - 1, y + 5 - 1 + MENUTEXTOFFSET, 0x001f7f3f);
					DrawString(2, s, xpos + sl / 2 + 8 + xoffs    , y + 5     + MENUTEXTOFFSET, 0x001f0000);
				}
			}
		}
	}

	// draw some text boxes
	SINT	col = (mControllerMenuSlide * 128) / SLIDE_TIME / 3;
	GINT	gh = G(mControllerMenuSlide) / SLIDE_TIME;
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;
	SINT	h = (gh * 78).Whole();

	DrawBox(32 , 146, 192, h, 3, col, col, col);
	DrawBox(288, 146, 192, h, 3, col, col, col);


	if (mMode == FEPS_CONTROLLER)
	{
		if ((mArrowCounter & 31) < 20)
		{
			DrawSprite(FEDCPST_ARROW_L,  60, 108 + ARROWOFFSET + 8, 0, 127, 127, 127, 2);
			DrawSprite(FEDCPST_ARROW_R, 186, 108 + ARROWOFFSET + 8, 0, 127, 127, 127, 2);

			if (!((!(PLATFORM.IsControllerValid(1)))))
			{
				DrawSprite(FEDCPST_ARROW_L, 316, 108 + ARROWOFFSET + 8, 0, 127, 127, 127, 2);
				DrawSprite(FEDCPST_ARROW_R, 442, 108 + ARROWOFFSET + 8, 0, 127, 127, 127, 2);
			}
		}

		// and the controller info:
		char	text[200];

		text[1] = 0;

		SINT	c1, c2;
		for (c1 = 0; c1 < 2; c1 ++)
		{
			for (c2 = 0; c2 < 5; c2 ++)
			{
				text[0] = get_controller_char(c1 ? mControllerSetting2 : mControllerSetting1, c2);

				DrawString(0, text, c1 ? 336 : 80, 147 + (c2 * 15), 0x007f7f7f);
			}
		}

		CText::GetString(TEXT_JUMP, text);
		DrawDynamicText(2, text, 110, 147, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366, 147, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_ATTACK, text);
		DrawDynamicText(2, text, 110, 162, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366, 162, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_BLOCK, text);
		DrawDynamicText(2, text, 110, 177, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366, 177, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_THROW, text);
		DrawDynamicText(2, text, 110, 192, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366, 192, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_BRAKE, text);
		DrawDynamicText(2, text, 110, 207, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366, 207, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_HELPDC1, text);
		SINT	sl = GetStringSize(2, text);
		DrawString(2, text, 256 - sl / 2, 230 , 0x007f7f7f);

	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::RenderParticles()
{
	SINT	c0;

	for (c0 = 0; c0 < NUM_PSXFE_PARTICLES; c0 ++)
	{
		if (mParticles[c0].mVal != 0)
		{
			SINT	r;
			SINT	g;
			SINT	b;

			switch (mParticles[c0].mType)
			{
			case	0:
				r = mParticles[c0].mVal;
				g = mParticles[c0].mVal / 2;
				b = mParticles[c0].mVal / 5;
				break;
			case	1:
				r = mParticles[c0].mVal / 5;
				g = mParticles[c0].mVal / 2;
				b = mParticles[c0].mVal;
				break;
			}

			DrawSprite(FEDCPST_SPARK, mParticles[c0].mX.Whole() - 8, mParticles[c0].mY.Whole() - 8, 0, r, g, b, 0.7f);
		}
	}

	// lightning?
	if (mLightningTime)
	{
#define	FELIGHTNING_SIZE			10.f
#define FELIGHTNING_WIDTH			2.f
#define	FELIGHTNING_NUM_STREAMS		2
#define	FELIGHTNING_OFFSET			16

		SINT c0, c1, c2;

		// work out how many segments we need

		float	dx = float(mLightningEX - mLightningSX);
		float	dy = float(mLightningEY - mLightningSY);

		float	dist = float(sqrt((dx * dx) + (dy * dy)));

		SINT	num_segs = SINT(dist / FELIGHTNING_SIZE);

		float	ox = dx / num_segs;
		float	oy = dy / num_segs;

		float	perpx = 0;
		float	perpy = 0;

		if (dy > 0)
			perpx = 1;
		else if (dy < 0)
			perpx = -1;

		if (dx > 0)
			perpy = -1;
		else if (dx < 0)
			perpy = 1;



		kmChangeStripListType			( &GouraudHead, KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &GouraudHead, KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
	 	kmChangeStripTextureShadingMode ( &GouraudHead, KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &GouraudHead, KM_IMAGE_PARAM1, KM_TRUE );

		DWORD	col = 0xff204080;		
		
		for (c1 = 0; c1 < FELIGHTNING_NUM_STREAMS; c1++)
		{
			float	cx = mLightningSX;
			float	cy = mLightningSY;

			for	(c0 = 0; c0 < num_segs; c0++)
			{
				InitCZs();
				
				GouraudS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
			   	GouraudS[0].fX                   = (cx + perpx) * DCMULTX;
			   	GouraudS[0].fY                   = (cy + perpy) * DCMULTY;
			   	GouraudS[0].u.fZ                 = 10.f;
			   	GouraudS[0].uBaseRGB.dwPacked    = col;
			
			   	GouraudS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
			   	GouraudS[1].fX                   = (cx - perpx) * DCMULTX;
			   	GouraudS[1].fY                   = (cy - perpy) * DCMULTY;
			   	GouraudS[1].u.fZ                 = 10.f;
			   	GouraudS[1].uBaseRGB.dwPacked    = col;

				// nev verts
				if (c0 != num_segs - 1)
				{
					cx = mLightningSX + (ox * (c0 + 1)) + float((rand() % FELIGHTNING_OFFSET) - (FELIGHTNING_OFFSET / 2));
					cy = mLightningSY + (oy * (c0 + 1)) + float((rand() % FELIGHTNING_OFFSET) - (FELIGHTNING_OFFSET / 2));
				}
				else
				{
					cx = mLightningEX;
					cy = mLightningEY;
				}
							
			   	GouraudS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
			   	GouraudS[2].fX                   = (cx + perpx) * DCMULTX;
			   	GouraudS[2].fY                   = (cy + perpy) * DCMULTY;
			   	GouraudS[2].u.fZ                 = 10.f;
			   	GouraudS[2].uBaseRGB.dwPacked    = col;
			
			   	GouraudS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
			   	GouraudS[3].fX                   = (cx - perpx) * DCMULTX;
			   	GouraudS[3].fY                   = (cy - perpy) * DCMULTY;
			   	GouraudS[3].u.fZ                 = 10.f;
			   	GouraudS[3].uBaseRGB.dwPacked    = col;
			
				ExitCZs(4);			
			}
		}
	}
}

//******************************************************************************************
void	CDCFEPrisonSelect::Render()
{
	SDSSwitchClippingOff();				
	SDSRenderStart();			
	SDSSetClipRegion(0,0,640,540);			

	// Hacktastic
	ResetLastDrawnString();

	// Setup engine things
	PLATFORM.SetViewport(0);
	ENGINE.SetCamera(&mCamera);

	//************************
	// Background

	if ((mMode != FEPS_PS2GAME) && (mMode != FEPS_START) && (mMode != FEPS_TS2GAME))
	{
		DrawSprite(FEDCPST_BACK1, 0    , 0     , OT_LENGTH - 3, 80, 80, 80);
		DrawSprite(FEDCPST_BACK2, 204.8, 0     , OT_LENGTH - 3, 80, 80, 80);
		DrawSprite(FEDCPST_BACK3, 409.6, 0     , OT_LENGTH - 3, 80, 80, 80);
		DrawSprite(FEDCPST_BACK4, 0    , 136.53, OT_LENGTH - 3, 80, 80, 80);
		DrawSprite(FEDCPST_BACK5, 204.8, 136.53, OT_LENGTH - 3, 80, 80, 80);
		DrawSprite(FEDCPST_BACK6, 409.6, 136.53, OT_LENGTH - 3, 80, 80, 80);
	}

	if (PLATFORM.IsControllerValid(0) || mMode == FEPS_START)
	{
		// draw title bar
		if ((mMode != FEPS_START) &&
			(mMode != FEPS_START2MAIN) &&
			(mMode != FEPS_MAIN2START) &&
			(mMode != FEPS_PS2GAME) &&
			(mMode != FEPS_TS2GAME) &&
			(mMode != FEPS_START2SELMEM))
		{
			SINT	c0;
			for (c0 = 0; c0 < 5; c0 ++)
			{
				DrawSprite(FEDCPST_LCD1 + c0, 102.2 * float(c0), TITLE_BAR_Y, 2, 127, 127, 127);
			}
	
	
		}
	
		//**********************************************************************************
		//** Particles
	
		if ((mMode != FEPS_PS2GAME) && (mMode != FEPS_START) && (mMode != FEPS_TS2GAME))
			RenderParticles();
	
		//**********************************************************************************
		//** Start view
	
		if (mMode == FEPS_START)
			LoadAndRenderStart();
	
		//**********************************************************************************
		//** Main Menu view
	
		switch (mMode)
		{
		case 	FEPS_START2MAIN:
		case 	FEPS_MAIN2START:
		case	FEPS_MAIN:
		case 	FEPS_CHAR2MAIN:
		case	FEPS_OPTIONS2MAIN:
		case 	FEPS_MAIN2OPTIONS:
		case	FEPS_MAIN2MCHAR:
		case	FEPS_MCHAR2MAIN:
		case 	FEPS_CREDITS2MAIN:
		case	FEPS_MAIN2CREDITS:
		case	FEPS_SELMEM2MAIN:
		case	FEPS_MAIN2CHAR:
				RenderMain();
			break;
		};
	
		//**********************************************************************************
		//** SelMem Menu view
	
		switch (mMode)
		{
		case	FEPS_SELMEM:
		case	FEPS_SELMEM2MAIN:
		case	FEPS_SELMEM2OPTIONS:
		case	FEPS_START2SELMEM:
		case	FEPS_OPTIONS2SELMEM:
	
			RenderSelMem();
			break;
		}
	
		//**********************************************************************************
		//** Character Select view
	
		switch (mMode)
		{
		case 	FEPS_CHAR:
		case 	FEPS_CHAR2MAIN:
		case	FEPS_CHAR2PSNEW:
		case	FEPS_PSNEW2CHAR:
		case	FEPS_MAIN2CHAR:
			RenderChar();
			break;
		};
	
		//**********************************************************************************
		//** Prison Select new view
		switch (mMode)
		{
		case	FEPS_PSNEW:
		case	FEPS_CHAR2PSNEW:
		case	FEPS_PSNEW2CHAR:
		case	FEPS_PSNEW2ARENA:
		case	FEPS_ARENA2PSNEW:
			RenderPrisonNew();
			break;
	
		};
	
		//**********************************************************************************
		//** QSave Menu view
		switch (mMode)
		{
		case	FEPS_ARENA2QSAVE:
		case	FEPS_QSAVE:
		case	FEPS_QSAVE2ARENA:
		case	FEPS_QSAVE2SELMEM:
			RenderQSave();
			break;
		};
	
		//**********************************************************************************
		//** Message Menu view
	
		switch (mMode)
		{
		case	FEPS_ARENA2MESSAGE:
		case	FEPS_MESSAGE:
		case	FEPS_MESSAGE2ARENA:
			RenderMessage();
			break;
		};
	
		//**********************************************************************************
		//** Arena Select view
		switch (mMode)
		{
		case	FEPS_PSNEW2ARENA:
		case	FEPS_ARENA2PSNEW:
		case	FEPS_ARENA:
		case	FEPS_ARENA2QSAVE:
		case	FEPS_QSAVE:
		case	FEPS_QSAVE2ARENA:
		case	FEPS_QSAVE2SELMEM:
		case	FEPS_SELMEM2ARENA:
		case	FEPS_ARENA2MESSAGE:
		case	FEPS_MESSAGE:
		case	FEPS_MESSAGE2ARENA:
			RenderArena();
			break;
		};
	
		//**********************************************************************************
		//** Front end to Game
	
		if ((mMode == FEPS_PS2GAME) || (mMode == FEPS_TS2GAME))
			RenderPS2Game();
	
		//**********************************************************************************
		//** MCharacter Select view
	
		switch (mMode)
		{
		case	FEPS_MAIN2MCHAR:
		case	FEPS_MCHAR2MAIN:
		case	FEPS_MCHAR:
		case	FEPS_MCHAR2MTOURN:
		case	FEPS_MTOURN2MCHAR:
			RenderMChar();
			break;
		};
	
		//**********************************************************************************
		//** MTourn Select view
	
		switch (mMode)
		{
		case	FEPS_MCHAR2MTOURN:
		case	FEPS_MTOURN2MCHAR:
		case	FEPS_MTOURN:
			RenderMTourn();
			break;
		};
	
		//**********************************************************************************
		//** Options Menu view
	
		switch (mMode)
		{
		case 	FEPS_MAIN2OPTIONS:
		case 	FEPS_OPTIONS2MAIN:
		case	FEPS_OPTIONS:
		case	FEPS_OPTIONS2SELMEM:
		case 	FEPS_SELMEM2OPTIONS:
		case 	FEPS_OPTIONS2CONTROLLER:
		case 	FEPS_CONTROLLER2OPTIONS:
			RenderOptions();
			break;
		};
	
		//**********************************************************************************
		//** Controller Menu View
		switch (mMode)
		{
		case	FEPS_CONTROLLER:
		case 	FEPS_OPTIONS2CONTROLLER:
		case 	FEPS_CONTROLLER2OPTIONS:
			RenderController();
			break;
		};
	
		//**********************************************************************************
		//** Credits View
		switch(mMode)
		{
		case	FEPS_CREDITS2MAIN:
		case	FEPS_CREDITS:
		case	FEPS_MAIN2CREDITS:
			RenderCredits();
			break;
		};
	
		//**********************************************************************************
		//** Draw Title Text
	
		char	text[200];
		text[0] = 0;
		SINT	font = 2;
		switch (mMode)
		{
		case	FEPS_MAIN:
		case	FEPS_CHAR2MAIN:
		case	FEPS_OPTIONS2MAIN:
		case 	FEPS_MCHAR2MAIN:
		case	FEPS_SELMEM2MAIN:
		case	FEPS_CREDITS2MAIN:
			font = 0;
			CText::GetString(TEXT_TITLE, text);
			break;
	
		case 	FEPS_OPTIONS2SELMEM:
		case	FEPS_SELMEM:
		case	FEPS_QSAVE2SELMEM:
			CText::GetString(TEXT_DC_SELECT_MEMORY_CARD, text);
			break;
	
		case	FEPS_MAIN2CHAR:
		case	FEPS_CHAR:
		case	FEPS_PSNEW2CHAR:
			CText::GetString(TEXT_CHARACTER_SELECT, text);
			break;
	
		case	FEPS_CHAR2PSNEW:
		case	FEPS_PSNEW:
		case	FEPS_ARENA2PSNEW:
			CText::GetString(TEXT_PRISON_SELECT, text);
			break;
	
		case	FEPS_PSNEW2ARENA:
		case	FEPS_ARENA:
		case	FEPS_QSAVE2ARENA:
		case	FEPS_SELMEM2ARENA:
		case	FEPS_ARENA2MESSAGE:
		case	FEPS_MESSAGE:
		case 	FEPS_MESSAGE2ARENA:
			CText::GetString(TEXT_ARENA_SELECT, text);
			break;
	
		case	FEPS_ARENA2QSAVE:
		case	FEPS_QSAVE:
			CText::GetString(TEXT_SAVE_GAME, text);
			break;
	
		case	FEPS_MAIN2MCHAR:
		case 	FEPS_MCHAR:
		case	FEPS_MTOURN2MCHAR:
			CText::GetString(TEXT_CHARACTER_SELECT, text);
			break;
	
		case	FEPS_MCHAR2MTOURN:
		case	FEPS_MTOURN:
			CText::GetString(TEXT_TOURNAMENT_SELECT, text);
			break;
	
		case	FEPS_MAIN2OPTIONS:
		case	FEPS_OPTIONS:
		case	FEPS_SELMEM2OPTIONS:
		case	FEPS_CONTROLLER2OPTIONS:
			CText::GetString(TEXT_OPTIONS, text);
			break;
	
		case	FEPS_OPTIONS2CONTROLLER:
		case	FEPS_CONTROLLER:
			CText::GetString(TEXT_CONTROLLER_SETUP, text);
			break;
	
		case 	FEPS_MAIN2CREDITS:
		case 	FEPS_CREDITS:
			CText::GetString(TEXT_CREDITS, text);
			break;
	
		};
	
		if (text[0] != 0)
		{
			if (GAME.GetLanguage() == 0)
				font = 0; // always large in English
			SINT	sl = GetStringSize(font, text);
			DrawString(font, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25 + 1, 0x007f5f3f);
		}
	}
	else
	{
		// no controller - display warning...
		
		SINT	c0;
		SINT	y = 100;
	
		for (c0 = 0; c0 < 2; c0 ++)
		{
			SINT 	c1;
			SINT	x = 112 - 48;
	
			SINT	colr = 60, colg = 72, colb = 100;
	
			DrawSprite(FEDCPST_BOXL, x    , y    , 2, colr, colg, colb, 2);
			x += 48;
	
			for (c1 = 0; c1 < 6; c1 ++)
			{
				DrawSprite(FEDCPST_BOXM, x    , y    , 2, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEDCPST_BOXR, x    , y    , 2, colr, colg, colb, 2);
	
			// draw text
			//!! Temporary!
			char	s[100];
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_DC_CONTROLLER_REMOVED, s); break;
			case 1:	CText::GetString(TEXT_DC_MEMCARD_RECOGNISED, s); break;
			};
	
			SINT	tcol;
	
			tcol = 0x007f7f7f;
	
			if (SINT(GetSysTimeFloat() * 10.f) % 10 > 3)
			{	
				SINT	sl = GetStringSize(0, s);
				DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 - 1, y + 5 - 1 + MENUTEXTOFFSET, tcol);
				DrawString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2    , y + 5     + MENUTEXTOFFSET, 0x001f0000);
			}
			y += 30;
		}
	}

	PLATFORM.GeometryMarkEndScene();

}


//******************************************************************************************
void	CDCFEPrisonSelect::Reset(EPSEntryType entrytype)
{
	SINT	c0;

	// misc engine setup
	mCamera.Reset(0);
//	PLATFORM.SetScreenWidth(640);
	PLATFORM.SetScreenWidth(512); //! JCL - (sic)
#ifdef JCLNOPSX // misc setup stuff
	SPCamera=&mCamera;
	SPCameraGMatrix=SPCamera->mOrientation;
	ENGINE.GMatrixToMATRIX(&SPCameraGMatrix,&SPCameraMATRIX);
	int sx = PLATFORM.ScreenWidth()>>1;
	SetGeomOffset(sx, PLATFORM.ScreenHeight()>>1);
	OffScreenMask=0xff00fe00;
#endif

	mQuit = EPSXFE_NONE;
	mCounter = 0;
	mArrowCounter = 0;
	mAttractCounter = 0;

	mDBX  = FALSE;
	mDBY  = FALSE;
	mDBX2 = FALSE;
	mDBY2 = FALSE;

	// Main Menu init
	mMainMenuPos = 0;
	mMainMenuSlide = 0;

	// Sel Mem Menu
	mSelMemMenuSlide = 0;
	mSelMemMenuState = EMS_SELECT_CARD;
	mSelMemCard = 0;
	mSelMemPort = 0;

	// Char Menu init
	mCharacter = 0;
	mCharMenuSlide = 0;
	mCharSlide = G0;
	mCharSlideStart = 0;
	mCharRotTimer = 0;
	mCharWait = -1;
	for (c0 = 0; c0 < 6; c0 ++)
	{
		mCharThings[c0].Reset(c0);
	}

	// Prison select new init
	mPrison = 0;
	mPrisonSlide = G0;
	mPrisonSlideStart = 0;
	mPrisonAccessStart = 0;
	mPrisonMenuSlide = 0;
	mPrisonAccessible = TRUE;
	mPrisonBonusSlide = G0;
	mPrisonBonus = FALSE;

	// Arena Menu init
	mArenaMenuSlide = SLIDE_TIME;
	mArena = 0;
	mArenaSlideStart = 0;
	mArenaPendingVictory = -1;
	mArenaPendingVictoryType = 0;
	mArenaNumMessages = 0;
	mArenaCurrentMessage = 0;

	// QSave menu init
	mQSaveMenuSlide = 0;
	mQSaveMenuPos = 0;

	// Message Menu init
	mMessageMenuSlide = 0;
	mMessage = 0;

	// MChar Menu init
	mMCharacter[0] = 0;
	mMCharacter[1] = 1;
	mMCharMenuSlide = 0;
	mMCharSlide[0] = G0;
	mMCharSlide[1] = G0;
	mMCharSlideStart[0] = 0;
	mMCharSlideStart[1] = 0;
	mMCharRotTimer = 0;
	mMCharSelected[0] = FALSE;
	mMCharSelected[1] = FALSE;
	mMCharWait = -1;
	for (c0 = 0; c0 < 6; c0 ++)
	{
		mMCharThings[0][c0].Reset(c0);
		mMCharThings[1][c0].Reset(c0);
	}

	// Tournament Select Init
	mMTournament = 0;
	mMTournamentSlide = G0;
	mMTournamentSlideStart = 0;
	mMTournamentAccessStart = 0;
	mMTournamentMenuSlide = SLIDE_TIME;
	mMTournamentAccessible = TRUE;
	mMTournamentResultsTimer = -1;

	// Options Menu init
	mOptionsMenuPos = 0;
	mOptionsMenuSlide = 0;

	// Credits init
	mCreditsMenuSlide = 0;
	mCreditsPos = 0;

	// Controller Menu init
	mControllerMenuSlide = 0;

	// Initialise particles
	for (c0 = 0; c0 < NUM_PSXFE_PARTICLES; c0 ++)
		mParticles[c0].mVal = 0;

	mLightningTime = 0;

	if (GAME.IsCheatEnabled(CHEAT_BOUNCY) ||
		GAME.IsCheatEnabled(CHEAT_SLIPPERY))
	{
		// no cheating!
		if (entrytype == PSE_SINGLE_WIN)
			entrytype = PSE_SINGLE;
	}
	
		// Mode select
	switch (entrytype)
	{
	case PSE_TOP:
		mMode = FEPS_START;
		break;

	case PSE_SINGLE:
		{
			mMode = FEPS_ARENA;
			// pull some data out of the career file.
			mCharacter = CAREER.mCurrentCharacter;
			mPrison = CAREER.mCurrentPrison;
			mArena = CAREER.mCurrentArena;

			CLevelCareer	*lc = &CAREER.mCharCareers[CAREER.mCurrentCharacter].mLevelCareers[CAREER.mCurrentPrison][CAREER.mCurrentArena];
			lc->AddAttempt();

			// bonus prison
			if (mPrison == 10)
			{
				mPrison = 0;
				mPrisonBonus = TRUE;
				mPrisonBonusSlide = G1;
			}
	
			// always autosave on DC			
			mArenaPendingVictoryType = 0;
			mArenaPendingVictory = 5;
			
		}
		break;

	case PSE_MULTIPLAYER:
		mMode = FEPS_MTOURN;
		// pull some data out of the career file.
		mMCharacter[0] = MULTIPLAYER_CAREER.mWhichPlayer[0];
		mMCharacter[1] = MULTIPLAYER_CAREER.mWhichPlayer[1];
		mMTournament = MULTIPLAYER_CAREER.mCurrentTournament;
		break;

	case PSE_SINGLE_WIN:
		{
			mMode = FEPS_ARENA;
			// pull some data out of the career file.
			mCharacter = CAREER.mCurrentCharacter;
			mPrison = CAREER.mCurrentPrison;
			mArena = CAREER.mCurrentArena;
			mArenaPendingVictoryType = EvaluateSingleLevelPerformance();
//			if (mArenaPendingVictoryType)
				mArenaPendingVictory = 5;

			CLevelCareer	*lc = &CAREER.mCharCareers[CAREER.mCurrentCharacter].mLevelCareers[CAREER.mCurrentPrison][CAREER.mCurrentArena];
			lc->DeclareTime(CAREER.GetPendingVictory());
			lc->AddAttempt();

			// bonus prison
			if (mPrison == 10)
			{
				mPrison = 0;
				mPrisonBonus = TRUE;
				mPrisonBonusSlide = G1;
			}
		}
		break;

	case PSE_MULTIPLAYER_FINISHED:
		mMode = FEPS_MTOURN;
		// pull some data out of the career file.
		mMCharacter[0] = MULTIPLAYER_CAREER.mWhichPlayer[0];
		mMCharacter[1] = MULTIPLAYER_CAREER.mWhichPlayer[1];
		mMTournament = MULTIPLAYER_CAREER.mCurrentTournament;

		// trigger result displaying
		mMTournamentResultsTimer = 0;
		mMTournamentResultsShowLW = 0;
		mMTournamentResultsShowWinner = FALSE;

		break;
	}
}

//******************************************************************************************
SINT	CDCFEPrisonSelect::Run(EPSEntryType entrytype)
{
	SDSSwitchClippingOff();	
	kmSetRegionClipping((0)>>5,560,(640)>>5,(480)>>5);
	kmSetPixelClipping(0,0,640,560);	
	SDSRenderStart();
	SDSSetClipRegion(0,0,640,560);
	SDSRenderFlip();

	SINT c0;
	
	for (c0 = 0; c0 < 6; c0 ++)
	{
		DrawSync(0);
		VSync(0);
		SOUND.UpdateStatus();
	}
	
	PLATFORM.ClearScreen(0);
	PLATFORM.ClearScreen(1);


	SINT n = 0;
	
#ifdef	JCLDCNOPSX
	if ball breakers n += 2
#endif

	if (CAREER.AllGold())
		n += 1;
	
	// do we have the data in the cache?
	if (front_end_data_loaded != n)
//	if (1)
	{
		GAME.ShowLoadingScreen(n);

		if (!LoadDataIntoCache(n))
			return 0; // oops!
	}
	
//	GAME.ShowLoadingScreen(n);
	
	LoadDataFromCache();
	
	SOUND.DeclareVolumeChange();  // update volumes
	SOUND.PlayTrack(3, 2);
	
	SINT	reset_count = 0;

	Reset(entrytype);


	//!!!!!!!***********  CHECK GAME DATA (eg water) *******************
	GAME.DeclareSinglePlayer();		// for the engine

	// and run!
	while (mQuit == EPSXFE_NONE)
	{
		CONTROLS.Update();
		
		if ((CONTROLS.IsButtonPressedDB(BUTTON_DC_RESET, 0)) ||
			(CONTROLS.IsButtonPressedDB(BUTTON_DC_RESET, 1)))
		{
			if (SoftResetAllowed)
			{
				if (mMode != FEPS_START)
				{
					Reset(PSE_TOP);
					reset_count = 30;
				}
				else
					SDSSoftReset(); // Bye!
			}
		}
			
		
		SOUND.UpdateStatus();

		if (CONTROLS.IsButtonPressedDB(BUTTON_SCREENSHOT))
			GAME.TakeScreenShot();

		if (reset_count == 0)
		{
			// is controller plugged in?
			if (PLATFORM.IsControllerValid(0) || mMode == FEPS_START)
				Process();
		}
		else
			reset_count --;
			
		Render();
	}

	// clear some global game stuff
	GAME.ClearGolfShots();
	GAME.ClearBestResult();

	switch (mQuit)
	{
	case EPSXFE_SINGLE:
		{
			// write information into career.
			if (CAREER.mCharCareers[mCharacter].mStarted == FALSE)
			{
				CAREER.mCharCareers[mCharacter].mStarted = TRUE;

				// Play FMV - note: trashes memory...
				PlayFMV(EMovie(FMV_ANGEL + mCharacter));
			}
			CAREER.mCurrentCharacter = mCharacter;
			if (mPrisonBonus)
				CAREER.mCurrentPrison	 = 10;
			else
				CAREER.mCurrentPrison	 = mPrison;

			CAREER.mCurrentArena	 = mArena;

			SLevelInfo	*li = CLevelStructure::GetLevelInfo(CAREER.mCurrentPrison, mArena);
			CAREER.mCurrentDiskLevel = li->mDiskLevel;

			GAME.DeclareSinglePlayer();

			if ((CAREER.mCurrentPrison == 9) && (CAREER.mCharCareers[mCharacter].mStartedAlpha == FALSE))
			{
				CAREER.mCharCareers[mCharacter].mStartedAlpha = TRUE;

				// Play FMV - note: trashes memory...
				PlayFMV(FMV_ALPHASTAR_IN);
			}

			return li->mDiskLevel;
		}

	case EPSXFE_MULTI:
		{
			// write information into career
			MULTIPLAYER_CAREER.Clear();

			MULTIPLAYER_CAREER.mWhichPlayer[0] = mMCharacter[0];
			MULTIPLAYER_CAREER.mWhichPlayer[1] = mMCharacter[1];

			MULTIPLAYER_CAREER.mCurrentTournament = mMTournament;
			MULTIPLAYER_CAREER.mCurrentArena = 0;

			MULTIPLAYER_CAREER.mCurrentDiskLevel = CLevelStructure::GetTournamentInfo(mMTournament)->mLevels[0].mDiskLevel;

			GAME.DeclareMultiPlayer();

			return MULTIPLAYER_CAREER.mCurrentDiskLevel;
		}

	case EPSXFE_ATTRACT:
		{
			GAME.ClearCheats();		 // they fuck up demo replay.
			return -mAttract;
		}
	default:
		return -1;
	};
}

#endif
