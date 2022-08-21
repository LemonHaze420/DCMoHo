// $Header$

// $Log$
// Revision 1.75  2000-10-20 11:25:53+01  asd
// pretty things made prettier
//
// Revision 1.74  2000-10-20 10:43:33+01  asd
// fixed alignment probs in french multiplayer select
//
// Revision 1.73  2000-10-20 10:26:27+01  are
// Black box in save.
//
// Revision 1.72  2000-10-19 17:34:39+01  asd
// <>
//
// Revision 1.71  2000-10-19 14:20:15+01  are
// Fixed potential problem if user changes saves during program execution.
//
// Revision 1.70  2000-10-19 11:11:14+01  are
// Arrows position correct for German.
//
// Revision 1.69  2000-10-19 10:13:36+01  are
// Load / save boxes and scroll bar.
//
// Revision 1.68  2000-10-18 20:07:38+01  asd
// added box round save
//
// Revision 1.67  2000-10-18 18:32:21+01  asd
// added new code to support voodoo
//
// Revision 1.66  2000-10-18 13:48:47+01  are
// Cheats
//
// Revision 1.65  2000-10-18 11:55:32+01  are
// Changes for new credits and changed colour of text in confirm quit.
//
// Revision 1.64  2000-10-18 11:27:43+01  are
// Cheat code enabled. I have not tested this code it may be broke.
//
// Revision 1.63  2000-10-18 10:04:30+01  are
// Sorted z problem on char screen and spaced out icons on arena select.
//
// Revision 1.62  2000-10-17 20:36:41+01  are
// Corrected black line problem.
//
// Revision 1.61  2000-10-17 17:36:43+01  are
// Have to press fire on MoHo intro screen to begin menu.
//
// Revision 1.60  2000-10-17 15:25:53+01  are
// Corrected offset in confirm overwrite dialog.
//
// Revision 1.59  2000-10-17 14:48:59+01  are
// Tournament icons and particles are now correct. Made message bar larger.
//
// Revision 1.58  2000-10-17 12:24:38+01  are
// Load button shadowed if no saves.
//
// Revision 1.57  2000-10-17 11:35:47+01  are
// Changed mesasage if no saves found. (Don't forget to change single so that load is shadowed out if no saves).
//
// Revision 1.56  2000-10-16 17:11:25+01  are
// Uses langauge resource for Overwrite message.
//
// Revision 1.55  2000-10-16 16:51:42+01  are
// Res change around video playback.
//
// Revision 1.54  2000-10-16 14:49:28+01  are
// Lightning additive.
//
// Revision 1.53  2000-10-16 14:45:04+01  asd
// changed stuff for tourn select
//
// Revision 1.52  2000-10-16 10:23:51+01  are
// Hack around failed to save state.
//
// Revision 1.51  2000-10-15 14:03:30+01  are
// Fixed problem with message being incorrectly rendered.
//
// Revision 1.50  2000-10-14 21:24:01+01  asd
// added cancel sound and some other bits
//
// Revision 1.49  2000-10-14 15:42:16+01  are
// Removed 'scanning for memory card' during loading.
//
// Revision 1.48  2000-10-13 17:35:47+01  asd
// updated draw for multiplayer, icons now in right place. Increased brightness of particles,
//
// Revision 1.47  2000-10-13 16:21:22+01  asd
// added jeremies changes
//
// Revision 1.46  2000-10-12 16:41:37+01  are
// Doesn't 'flash' during auto save.
//
// Revision 1.45  2000-10-12 16:21:47+01  are
// Quit button. Quit dialog. Changed order of confirm over write buttons - now in keeping with the rest of the program. Halo now additive. Other bits and bobs. :o)
//
// Revision 1.44  2000-10-12 13:43:26+01  asd
// updated colour values for text
//
// Revision 1.43  2000-10-12 10:37:35+01  are
// Prevented sounds from being played twice in multiplayer select.
//
// Revision 1.42  2000-10-11 17:09:55+01  asd
// moved one line
//
// Revision 1.41  2000-10-10 15:15:16+01  asd
// removed all dc help text
//
// Revision 1.40  2000-10-10 12:21:07+01  are
// Removed all calls to alpha sorting routines.
//
// Revision 1.39  2000-10-10 11:59:40+01  are
// Z-sorting has been removed, thus this code has been updated to force the correct rendering order. Much, much faster. No noticable problems.
//
// Revision 1.38  2000-10-10 09:47:28+01  asd
// bug fixes for arrows on options menu
//
// Revision 1.37  2000-10-06 17:31:12+01  asd
// changed keys to select player 2's character to up / down
//
// Revision 1.36  2000-10-06 11:34:27+01  are
// Updated load screen. Now shows eight (8) saves at a time. Verisons 1.9 and 1.4 of PCSaveRoutines.cpp and .h, respectively, are required.
//
// Revision 1.35  2000-10-06 09:14:41+01  are
// Auto save works.
// Removed redundent features from the options menu.
//
// Revision 1.34  2000-10-01 18:35:36+01  are
// <>
//
// Revision 1.33  2000-10-01 15:03:40+01  are
// Disabled start screen and attract mode. Removed controller option from options screen.
//
// Revision 1.32  2000-09-29 16:59:41+01  are
// Maintains volume settings when career loaded.
//
// Revision 1.31  2000-09-29 11:20:41+01  jcl
// <>
//
// Revision 1.30  2000-09-28 13:55:35+01  jcl
// <>
//
// Revision 1.29  2000-09-27 11:39:07+01  jcl
// <>
//
// Revision 1.28  2000-09-27 10:35:02+01  jcl
// <>
//
// Revision 1.27  2000-09-26 10:20:18+01  are
// Commented out unreferenced variables
//
// Revision 1.26  2000-09-26 10:12:06+01  are
// Removed code that saves controller config at game shutdown. No longer required as the play has no way of modifying the controller setup in game, and the setup is saved during the config process.
//
// Revision 1.25  2000-09-25 15:25:30+01  are
// Corrected registry path references.
//
// Revision 1.24  2000-09-18 16:05:38+01  asd
// replaced sound code
//
// Revision 1.23  2000-09-15 15:56:43+01  asd
// added soundupdate line
//
// Revision 1.22  2000-09-15 10:17:13+01  jcl
// <>
//
// Revision 1.21  2000-09-14 17:30:33+01  jcl
// <>
//
// Revision 1.20  2000-09-14 17:15:02+01  jcl
// Remapped front end sounds to new mapping
//
// Revision 1.19  2000-09-14 16:55:21+01  jjs
// Fixes lightning.
//
// Revision 1.18  2000-09-14 13:25:19+01  are
// <>
//
// Revision 1.17  2000-09-14 11:29:55+01  are
// <>
//
// Revision 1.16  2000-09-14 11:03:46+01  jcl
// <>
//
// Revision 1.15  2000-09-13 10:51:01+01  are
// <>
//
// Revision 1.14  2000-09-11 15:04:50+01  are
// Force alpha textures to be drawn last.
//
// Revision 1.13  2000-09-07 16:44:44+01  are
// Unified controller update and fixed a few bugs / unreferenced variables.
//
// Revision 1.12  2000-09-05 17:46:46+01  are
// Cleaned up code, and partially implemented RenderMessage.
//
// Revision 1.11  2000-08-31 17:36:04+01  are
// Quick save screen and sorted out some z-buffer issues in existing screens.
//
// Revision 1.10  2000-08-30 14:37:35+01  are
// Unified many of the shadowed text draws (ENGINE.DrawShadowedString) into calls to ENGINE.DrawShadowedString. This is essential as the old method used the same z-depth value for both strings, which proved problamatic when sorting the triangles for rendering.
//
// Revision 1.9  2000-08-25 17:24:03+01  are
// Few fixes, attempting to get the particles to work.
//
// Revision 1.8  2000-08-24 11:36:25+01  are
// Renders characters correctly.
//
// Revision 1.7  2000-08-24 10:55:31+01  are
// Save both controller configs.
//
// Revision 1.6  2000-08-23 17:34:37+01  are
// Partial conversion of multiplay screens and additional code to support better save game file handling. Few other fixes.
//
// Revision 1.5  2000-08-18 16:36:30+01  are
// Texture clamping and more screens, etc.
//
// Revision 1.4  2000-08-17 17:44:14+01  are
// Loading and Saving implemented
//
// Revision 1.3  2000-08-16 13:17:29+01  are
// More screens implemented. Changed text to look more like PS version / standardised across implementation. Modified text layout calculations to avoid float rounding problems. Can now use keyboard to enter a save file name. Recovers surfaces and D3D.
//
// Revision 1.2  2000-08-11 17:23:42+01  are
// Restores surfaces / D3D and processes Windows messages.
//
// Revision 1.1  2000-08-05 10:33:16+01  jjs
// First PC milestone.
//

#include	"Common.h"

#if TARGET == PC

#include	"PCFEPrisonSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
//#include	"PSXMemCard.h"
#include	"PCSaveRoutines.h"
#include	"Career.h"
#include	"Globals.h"
#include	"PCFMV.h"
#include	"PCBuildType.h"
#include	"Credits.h"
#include	"PCCheats.h"


#define		MENUTEXTOFFSET	14
#define		ARROWOFFSETX 2
#define		ARROWOFFSETY 4


CPCFEPrisonSelect	PRISONSELECT;

#define SLIDE_TIME	20

#define PS2GAME_COUNT	35
#define	PS2GAME_W2B		25

#define COL_GOLD   127, 96, 0
#define	COL_BLUE   64, 92, 128

#define	TITLE_BAR_Y	10

#define	CREDITS_TIME 2000

#define	MAIN_TIMEOUT	(25 * 30)
#define	START_TIMEOUT	(50 * 30)  // runs in a frame!
//#define	START_TIMEOUT	(30)  // runs in a frame!

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

SINT	CPCFEPrisonSelect::EvaluateSingleLevelPerformance()
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
BOOL	CPCFEPrisonSelect::LoadData()
{
	// load engine stuff
	if (!ENGINE.LoadLevelTextures())
		return FALSE;
	
	CInitRenderThing ri;
	CFrontEndThing	*t;
	
	t = &mCharThings[0];
	strcpy(ri.mName,"Angel_FE.msh");
	t->mMesh.Init(&ri);
	t = &mMCharThings[0][0];
	t->mMesh.Init(&ri);
	t = &mMCharThings[1][0];
	t->mMesh.Init(&ri);

	t = &mCharThings[1];
	strcpy(ri.mName,"Apostle_FE.msh");
	t->mMesh.Init(&ri);
	t = &mMCharThings[0][1];
	t->mMesh.Init(&ri);
	t = &mMCharThings[1][1];
	t->mMesh.Init(&ri);

	t = &mCharThings[2];
	strcpy(ri.mName,"Benny_FE.msh");
	t->mMesh.Init(&ri);
	t = &mMCharThings[0][2];
	t->mMesh.Init(&ri);
	t = &mMCharThings[1][2];
	t->mMesh.Init(&ri);

	t = &mCharThings[3];
	strcpy(ri.mName,"Lockdown_FE.msh");
	t->mMesh.Init(&ri);
	t = &mMCharThings[0][3];
	t->mMesh.Init(&ri);
	t = &mMCharThings[1][3];
	t->mMesh.Init(&ri);

	t = &mCharThings[4];
	strcpy(ri.mName,"Sophie_FE.msh");
	t->mMesh.Init(&ri);
	t = &mMCharThings[0][4];
	t->mMesh.Init(&ri);
	t = &mMCharThings[1][4];
	t->mMesh.Init(&ri);

	t = &mCharThings[5];
	strcpy(ri.mName,"Doctor_FE.msh");
	t->mMesh.Init(&ri);
	t = &mMCharThings[0][5];
	t->mMesh.Init(&ri);
	t = &mMCharThings[1][5];
	t->mMesh.Init(&ri);

	SINT	n = 0;

	if (CAREER.AllGold())
		n += 1;

#ifdef BALL_BREAKERS	// JCL
	n += 2;
#endif
	
	char	screen_name[400];
	char	name[400];

	switch (n)
	{
	case  0: strcpy(screen_name, "..\\screens\\ms");	break;
	case  1: strcpy(screen_name, "..\\screens\\mgs");	break;
	case  2: strcpy(screen_name, "..\\screens\\bbs");	break;
	case  3: strcpy(screen_name, "..\\screens\\bbgs");	break;
	};

	SINT	c0;
	for (c0 = 0; c0 < 6; c0 ++)
	{
		sprintf(name, "%s%d.tga", screen_name, c0 + 1);
		mTexSplash[c0] = CTextureTL::GetTexture(name);
	}

	sprintf(screen_name, "frontend\\back");
	for (c0 = 0; c0 < 6; c0 ++)
	{
		sprintf(name, "%s%d.tga", screen_name, c0 + 1);
		mTexBack[c0] = CTextureTL::GetTexture(name);
	}

	title[0]	= CTextureTL::GetTexture("frontend\\lcd1.tga");
	title[1]	= CTextureTL::GetTexture("frontend\\lcd2.tga");
	title[2]	= CTextureTL::GetTexture("frontend\\lcd3.tga");
	title[3]	= CTextureTL::GetTexture("frontend\\lcd4.tga");
	title[4]	= CTextureTL::GetTexture("frontend\\lcd5.tga");
	boxl		= CTextureTL::GetTexture("frontend\\1butbitl.tga");
	boxm		= CTextureTL::GetTexture("frontend\\1butbitm.tga");
	boxr		= CTextureTL::GetTexture("frontend\\1butbitr.tga");
	arrow_l		= CTextureTL::GetTexture("frontend\\arrow_l.tga");
	arrow_r		= CTextureTL::GetTexture("frontend\\arrow_r.tga");
	arrow_u		= CTextureTL::GetTexture("frontend\\arrow_u.tga");
	arrow_d		= CTextureTL::GetTexture("frontend\\arrow_d.tga");
	spark		= CTextureTLAdditive::GetTexture("frontend\\spark.tga");
	qualify		= CTextureTL::GetTexture("frontend\\qualify.tga");
	halo		= CTextureTLAdditive::GetTexture("frontend\\halo.tga");
	draw		= CTextureTL::GetTexture("frontend\\draw.tga");
	gold		= CTextureTL::GetTexture("frontend\\gold.tga");
	goldlt		= CTextureTL::GetTexture("frontend\\goldlt.tga");
	selchar		= CTextureTL::GetTexture("frontend\\selchar.tga");
	framctr		= CTextureTL::GetTexture("frontend\\framctr.tga");
	framctl		= CTextureTL::GetTexture("frontend\\framctl.tga");
	framcbr		= CTextureTL::GetTexture("frontend\\framcbr.tga");
	framcbl		= CTextureTL::GetTexture("frontend\\framcbl.tga");
	frampt		= CTextureTL::GetTexture("frontend\\frampt.tga");
	frampl		= CTextureTL::GetTexture("frontend\\frampl.tga");
	frampb		= CTextureTL::GetTexture("frontend\\frampb.tga");
	frampr		= CTextureTL::GetTexture("frontend\\frampr.tga");
	clipl		= CTextureTL::GetTexture("frontend\\clipl.tga");
	clipr		= CTextureTL::GetTexture("frontend\\clipr.tga");
	ps[0]		= CTextureTL::GetTexture("frontend\\ps_eco.tga");
	ps[1]		= CTextureTL::GetTexture("frontend\\ps_zeps.tga");
	ps[2]		= CTextureTL::GetTexture("frontend\\ps_hosp.tga");
	ps[3]		= CTextureTL::GetTexture("frontend\\ps_shipris.tga");
	ps[4]		= CTextureTL::GetTexture("frontend\\ps_rehab.tga");
	ps[5]		= CTextureTL::GetTexture("frontend\\ps_mine.tga");
	ps[6]		= CTextureTL::GetTexture("frontend\\ps_deaths.tga");
	ps[7]		= CTextureTL::GetTexture("frontend\\ps_sport.tga");
	ps[8]		= CTextureTL::GetTexture("frontend\\ps_milis.tga");
	ps[9]		= CTextureTL::GetTexture("frontend\\ps_alpha.tga");
	ps[10]		= CTextureTL::GetTexture("frontend\\ps_bonus.tga");
	ar[0]		= CTextureTL::GetTexture("frontend\\ar_king.tga");
	ar[1]		= CTextureTL::GetTexture("frontend\\ar_rollin.tga");
	ar[2]		= CTextureTL::GetTexture("frontend\\ar_gaunt.tga");
	ar[3]		= CTextureTL::GetTexture("frontend\\ar_purs.tga");
	ar[4]		= CTextureTL::GetTexture("frontend\\ar_race.tga");
	ar[5]		= CTextureTL::GetTexture("frontend\\ar_trick.tga");
	ar[6]		= CTextureTL::GetTexture("frontend\\ar_power.tga");
	ar[7]		= CTextureTL::GetTexture("frontend\\ar_golf.tga");
	ar[8]		= CTextureTL::GetTexture("frontend\\ar_pin.tga");
	ar[9]		= CTextureTL::GetTexture("frontend\\ar_speed.tga");
	ar[10]		= CTextureTL::GetTexture("frontend\\ar_shad.tga");
	lightning	= CTextureGTEXAdditive::GetTexture("lightning.tga");
	felightning	= CTextureTLAdditive::GetTexture("frontend\\felightning.tga");
	white		= CTextureTL::GetTexture("frontend\\alphawhite.tga");

	SOUND.AllocSampleBank("MOHOFE");

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
// WARNING: This implementation is bound to the PRISONSELECT object!!!
HRESULT __cdecl SaveGameKeyCaptureCallback(UBYTE ascii , UINT keydata)
{
	int xpos, ypos;

	// Alpha keys.
	if (((ascii > 64) && (ascii < 91)) || 
		((ascii > 96) && (ascii < 123)))
	{
		if (ascii < 91)
		{
			xpos = (ascii - 65)	% 13;
			ypos = (ascii - 65) / 13;
		}
		else
		{
			xpos = (ascii - 97)	% 13;
			ypos = (ascii - 97) / 13 + 2;
		}

		PRISONSELECT.mSaveX = xpos;
		PRISONSELECT.mSaveY = ypos;

		if (PRISONSELECT.mSaveNameLen < 8)
		{
			CAREER.mName[PRISONSELECT.mSaveNameLen] = ascii;
			PRISONSELECT.mSaveNameLen ++;
			CAREER.mName[PRISONSELECT.mSaveNameLen] = 0;
			
			SOUNDNAME;
		}
		else
		{
			SOUNDERROR;
		}
		return 0;
	}


	// Backspace
	if (ascii == 0x08) 
	{
		PRISONSELECT.mSaveX = 0;
		PRISONSELECT.mSaveY	= 4;

		if 	(PRISONSELECT.mSaveNameLen > 0)
		{
			PRISONSELECT.mSaveNameLen--;
			CAREER.mName[PRISONSELECT.mSaveNameLen] = 0;	
			SOUNDNAME;
		}
		else
		{
			SOUNDERROR;
		}
		return 0;
	}

	// Enter
	if (ascii == 0x0D)
	{
		PRISONSELECT.mSaveX = 1;
		PRISONSELECT.mSaveY	= 4;

		if (PRISONSELECT.mSaveNameLen > 0)
		{
			PRISONSELECT.mSaveMenuState = CPCFEPrisonSelect::ESS_SAVING;
			PRISONSELECT.mCounter = 0;
			SOUNDSELECT;
		}
		else
		{
			SOUNDERROR;
		}
		return 0;
	}

	return 0;
}


//******************************************************************************************
#define PRCAM_MOMENTUM  G(0, 10000)

void	CPCFEPrisonSelect::Process()
{
	// buttons
	BOOL	goback 	= FALSE;
	BOOL	goon	= FALSE;
	SINT	EffectVolume;
	SINT	MusicVolume;
	
	// Process platform messages. 
	PLATFORM.ProcessMessages();

	if (mQuit == EPSXFE_NONE)
	{
		GINT xm = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT);
		GINT ym = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP);

		GINT xm2 = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, 1);
		GINT ym2 = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP, 1);


	//	char* ControllerConf;
	//	DWORD DataLength;

		switch (mMode)
		{

		// ****************************************************************************
		// ****************************************************************************
		// ** START

		case FEPS_START:
			mMode = FEPS_START2MAIN;


			/*
			//if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
			//{
			//	mQuit = EPSXFE_EXIT;
			//}

			if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
			{
				mMainMenuSlide = 0;
				mMode = FEPS_START2MAIN;
				SOUNDSELECT;
			}
			
			if (mAttractCounter > START_TIMEOUT)
			{
				mQuit = EPSXFE_ATTRACT;
				mAttract ++;
				if (mAttract > 100000)
					mAttract = 1; // WHY??????????????????! ;)
			}*/
			break;

		case FEPS_START2MAIN:
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
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
		// ** ConfirmQuit

		case FEPS_CONFIRMQUIT:
			if (ym != G0)
			{
				if (!mDBY)
				{
					if (mConfirmQuitMenuPos == 0)
						mConfirmQuitMenuPos = 1;
					else
						mConfirmQuitMenuPos = 0;

					mDBY = 6;
					SOUNDMOVE;
				}
				else
					mDBY --;
			}
			else
				mDBY = FALSE;

			if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
			{
				switch(mConfirmQuitMenuPos)
				{
				case 0:
					mMode = FEPS_CONFIRMQUIT2MAIN;
					mConfirmQuitMenuSlide = SLIDE_TIME;
					mMainMenuSlide = 0;
					break;

				case 1:
					mQuit = EPSXFE_EXIT;
					break;
				default:
					break;
				};
				SOUNDSELECT;
			}
		
			if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
			{
				mMode = FEPS_CONFIRMQUIT2MAIN;
				mConfirmQuitMenuSlide = SLIDE_TIME;
				mMainMenuSlide = 0;
				SOUNDCANCEL;
			}
			break;

		case FEPS_CONFIRMQUIT2MAIN:
			mConfirmQuitMenuSlide --;
			mMainMenuSlide ++;
			if (mConfirmQuitMenuSlide == 0)
			{
				mMode = FEPS_MAIN;
				//mCounter = 0;
				//mMainSlideStart = 0;
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
				mMainMenuPos = 4;
			if (mMainMenuPos > 4)
				mMainMenuPos = 0;

#ifdef JJS	// Controller two check code.
			if ((mMainMenuPos == 1)	&& (PLATFORM.GetPadData(1)->valid))
			{
				if (ym < G0)	// also deals with case where pad is pulled out while multiplayer is selected
					mMainMenuPos = 2;
				else
					mMainMenuPos = 0;
			}
#endif
			if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
			{
				switch(mMainMenuPos)
				{
				case 0:
					mMode = FEPS_MAIN2SINGLE;
					mMainMenuSlide = SLIDE_TIME;
					mSingleMenuSlide = 0;
					if (!(CAREER.mStarted))
						mSingleMenuPos = 1;
					else
						mSingleMenuPos = 0;
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
					break;

				case 3:
					mMode = FEPS_MAIN2CREDITS;
					mMainMenuSlide = SLIDE_TIME;
					mCreditsMenuSlide = 0;
					mCreditsPos = 0;
					break;

				case 4:
					mMode = FEPS_MAIN2CONFIRMQUIT;
					mMainMenuSlide = SLIDE_TIME;
					mConfirmQuitMenuSlide = 0;
					mConfirmQuitMenuPos = 0;
					break;

				default:
					break;
				};
				SOUNDSELECT;
			}

			// Could have an option on the menu too...
			if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
			{
				mMode = FEPS_MAIN2CONFIRMQUIT;
				mMainMenuSlide = SLIDE_TIME;
				mConfirmQuitMenuSlide = 0;
				mConfirmQuitMenuPos = 0;
				SOUNDCANCEL;
			}

			/*
			if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
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
			*/
			

			{
				SINT cheat = CONTROLS.mControllers[0].GetCheat();
				if (cheat == CHEAT_VERSION)
				{
					SOUNDCHEAT;
					GAME.EnableCheat(CHEAT_VERSION);
				}
			}

			break;


		case FEPS_SINGLE2MAIN:
			mSingleMenuSlide --;
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}
			break;

		case FEPS_MAIN2CONFIRMQUIT:
			mConfirmQuitMenuSlide ++;
			mMainMenuSlide --;

			if (mMainMenuSlide == 0)
				mMode = FEPS_CONFIRMQUIT;
			break;

		case FEPS_MAIN2SINGLE:
			mSingleMenuSlide ++;
			mMainMenuSlide --;
			if (mMainMenuSlide == 0)
				mMode = FEPS_SINGLE;
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

		case FEPS_LOAD2MAIN:
			mLoadMenuSlide --;
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}
			break;

		case FEPS_ABANDON2MAIN:
			mAbandonMenuSlide --;
			mMainMenuSlide ++;
			if (mMainMenuSlide == SLIDE_TIME)
			{
				mMode = FEPS_MAIN;
				mAttractCounter = 0;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** SINGLE

		case FEPS_SINGLE:
			mCharacter = 0; // bugfix
			if (ym != G0)
			{
				if (!mDBY)
				{
					if (ym > G0)
					{
						 mSingleMenuPos --;
					//	 if ((mSingleMenuPos == 0) && (!(CAREER.mStarted)))
					//	 	mSingleMenuPos --;
					}
					else
					{
						mSingleMenuPos ++;
					//	if ((mSingleMenuPos == 3) && (!(CAREER.mStarted)))
					//		mSingleMenuPos ++;
					}

					SOUNDMOVE;

					mDBY = 6;
				}
				else
					mDBY --;
			}
			else
				mDBY = FALSE;

			// Going up...
			if ((mSingleMenuPos == 0) && (!(CAREER.mStarted)))
			 	mSingleMenuPos --;

			if ((mSingleMenuPos == -1) && (!(PCNumFiles())))
			 	mSingleMenuPos --;

			// Going down...
			if ((mSingleMenuPos == 2) && (!(PCNumFiles())))
			 	mSingleMenuPos ++;

			if ((mSingleMenuPos == 3) && (!(CAREER.mStarted)))
			 	mSingleMenuPos ++;

			if (mSingleMenuPos < 0)
				mSingleMenuPos += 3;
			if (mSingleMenuPos > 2)
				mSingleMenuPos -= 3;

			if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
			{
				switch(mSingleMenuPos)
				{
				case 1:
					if (CAREER.mStarted)
					{
						mMode = FEPS_SINGLE2ABANDON;
						mSingleMenuSlide = SLIDE_TIME;
						mAbandonMenuSlide = 0;
						mAbandonMenuPos = 0;
						break;
					}
					//NB - No Break!
				case 0:
					mMode = FEPS_SINGLE2CHAR;
					mSingleMenuSlide = SLIDE_TIME;
					mCharMenuSlide = 0;
					mCharRotTimer = 0;
					mCharWait = -1;
					SINT	c0;
					for (c0 = 0; c0 < 6; c0 ++)
					{
						mCharThings[c0].Reset(c0);
					}
					break;

				case 2:
					mMode = FEPS_SINGLE2LOAD;
					mSingleMenuSlide = SLIDE_TIME;
					mLoadMenuSlide = 0;
					mLoadMenuPos = 0;
					mLoadMenuScrollPos = 0;
					mLoadMenuState = ELS_START;
					break;

				default:
					break;
				};
				SOUNDSELECT;
			}

			if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
			{
				mMode = FEPS_SINGLE2MAIN;
				mSingleMenuSlide = SLIDE_TIME;
				mMainMenuSlide = 0;
				SOUNDCANCEL;
			}
			break;

		case FEPS_SINGLE2CHAR:
			mSingleMenuSlide --;
			mCharMenuSlide ++;
			if (mSingleMenuSlide == 0)
			{
				mMode = FEPS_CHAR;
				mCounter = 0;
				mCharSlideStart = 0;
				if (mSingleMenuPos == 1)
				{
					CAREER.Start();
					GAME.ClearCheats();
				}

			}
			{
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mCharThings[c0].Process();
				}
			}
			break;

		case FEPS_SINGLE2LOAD:
			mSingleMenuSlide --;
			mLoadMenuSlide ++;
			if (mSingleMenuSlide == 0)
			{
				mMode = FEPS_LOAD;
				mCounter = 0;
			}
			break;

		case FEPS_SINGLE2ABANDON:
			mSingleMenuSlide --;
			mAbandonMenuSlide ++;
			if (mSingleMenuSlide == 0)
			{
				mMode = FEPS_ABANDON;
				mCounter = 0;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** ABANDON

		case FEPS_ABANDON:
			if (ym != G0)
			{
				if (!mDBY)
				{
					if (mAbandonMenuPos == 0)
						mAbandonMenuPos = 1;
					else
						mAbandonMenuPos = 0;

					mDBY = 6;
					SOUNDMOVE;
				}
				else
					mDBY --;
			}
			else
				mDBY = FALSE;

			if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
			{
				switch(mAbandonMenuPos)
				{
				case 0:
					mMode = FEPS_ABANDON2MAIN;
					mAbandonMenuSlide = SLIDE_TIME;
					mMainMenuSlide = 0;
					break;

				case 1:
					mMode = FEPS_ABANDON2CHAR;
					mAbandonMenuSlide = SLIDE_TIME;
					mCharMenuSlide = 0;
					mCharRotTimer = 0;
					mCharWait = -1;
					SINT	c0;
					for (c0 = 0; c0 < 6; c0 ++)
					{
						mCharThings[c0].Reset(c0);
					}
					break;
				default:
					break;
				};
				SOUNDSELECT;
			}
			if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
			{
				mMode = FEPS_ABANDON2MAIN;
				mAbandonMenuSlide = SLIDE_TIME;
				mMainMenuSlide = 0;
				SOUNDCANCEL;
			}
			break;

		case FEPS_ABANDON2CHAR:
			mAbandonMenuSlide --;
			mCharMenuSlide ++;
			if (mAbandonMenuSlide == 0)
			{
				mMode = FEPS_CHAR;
				mCounter = 0;
				mCharSlideStart = 0;
				CAREER.Start();
				GAME.ClearCheats();
			}
			{
				SINT	c0;
				for (c0 = 0; c0 < 6; c0 ++)
				{
					mCharThings[c0].Process();
				}
			}
			break;


		// ****************************************************************************
		// ****************************************************************************
		// ** LOAD


		case FEPS_LOAD:
			SINT n;

			switch (mLoadMenuState)
			{
				case ELS_START:
					if (mCounter == 3)
						mLoadMenuState = ELS_SCANNING;
					break;

				case ELS_SCANNING:
					{
						// Work out how many save files there are.
						if (PCNumFiles())
						{
							mLoadMenuState = ELS_SAVES_FOUND;
							mLoadMenuPos = 0;
							mLoadMenuScrollPos = 0;
						}
						else
						{
							mLoadMenuState = ELS_NONE_FOUND;
						}
					}
					break;

				case ELS_UNFORMATTED:
				case ELS_NONE_FOUND:
					
					n = PCNumFiles();
					if (n)
					{
						mLoadMenuState = ELS_SCANNING;
					}
					else
					{
						mMode = FEPS_LOAD2MAIN;
						mLoadMenuSlide = SLIDE_TIME;
						mMainMenuSlide = 0;
						SOUNDCANCEL;
					}

					if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
					{
						mMode = FEPS_LOAD2MAIN;
						mLoadMenuSlide = SLIDE_TIME;
						mMainMenuSlide = 0;
						SOUNDCANCEL;
					}
					break;

				case ELS_SAVES_FOUND:
					{
					
				 		n = PCNumFiles();
						if (!n)
						{
							mLoadMenuState = ELS_SCANNING;
						}
						
						if (ym != G0)
						{
							if (!mDBY)
							{
								if (ym > G0)
								{
									mLoadMenuPos --;
								}
								else
								{
									mLoadMenuPos ++;
								}

								mDBY = 6;
								SOUNDMOVE;
							}
							else
								mDBY --;
						}
						else
							mDBY = FALSE;

						if (mLoadMenuPos < 0)
							mLoadMenuPos = 0;
						if (mLoadMenuPos >= n )
							mLoadMenuPos = n - 1;

						if (mLoadMenuPos < mLoadMenuScrollPos)
							mLoadMenuScrollPos = mLoadMenuPos;
						if (mLoadMenuPos > mLoadMenuScrollPos + 5)
							mLoadMenuScrollPos = mLoadMenuPos - 5;

						if (mLoadMenuScrollPos > n - 6)
							mLoadMenuScrollPos = n - 6;
						if (mLoadMenuScrollPos < 0)
							mLoadMenuScrollPos = 0;

					}

					if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
					{
						mMode = FEPS_LOAD2MAIN;
						mLoadMenuSlide = SLIDE_TIME;
						mMainMenuSlide = 0;
						SOUNDCANCEL;
					}

					if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
					{
						mLoadMenuState = ELS_LOADING;
						mCounter = 0;
						SOUNDSELECT;
					}

					break;

				case ELS_LOADING:
					{
						if (mCounter == 3)
						{
							SINT	ret;

//							ret = PCSaveCareer("JCLTest", GAME.GetCareer(), TRUE);
//							printf("Save returned %d\n", ret);

							char	name[9];

							my_strncpy(name, PCGetSaveNameByIndex(mLoadMenuPos)->name, 8);
							name[8] = 0;
							
							// Store volume settings so that we can transfer the 
							// info from the current career to loaded career. 
							EffectVolume	= CAREER.mSoundVolume;
							MusicVolume		= CAREER.mMusicVolume;
	
							ret = PCLoadCareer(name, &CAREER);

							CAREER.mSoundVolume = EffectVolume;
							CAREER.mMusicVolume = MusicVolume;

//							printf("Load returned %d\n", ret);

							switch (ret)
							{
								case McErrNone:
									// pull some data out of the career file.
									mCharacter = CAREER.mCurrentCharacter;
									mPrison = CAREER.mCurrentPrison;
									mArena = CAREER.mCurrentArena;

									// set the volumes
									SOUND.DeclareVolumeChange();

									mLoadMenuState = ELS_LOADED;
									break;

								case McWrongVersion:
									mLoadMenuState = ELS_WRONG_VERSION;
									break;

								default:
									mLoadMenuState = ELS_FAILED;
									break;
							};
						}
					}
					break;

				case ELS_LOADED:
					GAME.ClearCheats();
				//	if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
				//	{
						mMode = FEPS_LOAD2CHAR;
						mLoadMenuSlide = SLIDE_TIME;
						mCharMenuSlide = 0;
						mCharRotTimer = 0;
						mCharWait = -1;
						SINT	c0;
						for (c0 = 0; c0 < 6; c0 ++)
						{
							mCharThings[c0].Reset(c0);
						}
				//		SOUNDSELECT;
				//	}
					break;

				case ELS_FAILED:
				case ELS_WRONG_VERSION:
					if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
					{
						mMode = FEPS_LOAD2MAIN;
						mLoadMenuSlide = SLIDE_TIME;
						mMainMenuSlide = 0;
						SOUNDCANCEL;
					}
					break;

				case ELS_INSERT:
					{
/*
						if ((mCounter & 3) == 0)
						{
							SINT	ret = MCAcquireCard();

							if (ret == 0)
							{
								mCounter = 0;
								mLoadMenuState = ELS_START;
								break;
							}
							if (ret == McErrNotFormat)
							{
							 	mLoadMenuState = ELS_UNFORMATTED;
							 	break;
							}
						}
*/

						if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
						{
							mMode = FEPS_LOAD2MAIN;
							mLoadMenuSlide = SLIDE_TIME;
							mMainMenuSlide = 0;
							SOUNDCANCEL;
						}
					}
					break;
			};


#ifdef JJS	// Load game code?
			switch (mLoadMenuState)
			{
				case ELS_START:
					if (mCounter == 3)
						mLoadMenuState = ELS_SCANNING;
					break;

				case ELS_SCANNING:
					{
						SINT	ret;

						ret = MCAcquireCard();
						ret = MCAcquireCard();

						switch (ret)
						{
							 case 	McErrNone:
							 	{
							 		SINT n = MCNumFiles();

							 		if (n == 0)
							 		{
							 			mLoadMenuState = ELS_NONE_FOUND;
							 		}
							 		else
							 		{
										mLoadMenuState = ELS_SAVES_FOUND;
										mLoadMenuPos = 0;
										mLoadMenuScrollPos = 0;
							 		};
							 	}
							 	break;
							 case	McErrCardNotExist:
							 	mLoadMenuState = ELS_INSERT;
							 	break;

							 case 	McErrNotFormat:
							 	mLoadMenuState = ELS_UNFORMATTED;
							 	break;

							 default:
								mLoadMenuState = ELS_NONE_FOUND;
								break;
						};
					}
					break;

				case ELS_UNFORMATTED:
				case ELS_NONE_FOUND:
					if ((mCounter & 3) == 0)
					{
						SINT	ret = MCAcquireCard();

						if (ret == McErrCardNotExist)
						{
							mLoadMenuState = ELS_INSERT;
							break;
						}
					}

					if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
					{
						mMode = FEPS_LOAD2MAIN;
						mLoadMenuSlide = SLIDE_TIME;
						mMainMenuSlide = 0;
						SOUNDCANCEL;
					}
					break;

				case ELS_SAVES_FOUND:
					{
						if ((mCounter & 3) == 0)
						{
							SINT	ret = MCGetState(1);

							if (ret == McErrCardNotExist)
							{
								mLoadMenuState = ELS_INSERT;
								break;
							}
						}

				 		SINT n = MCNumFiles();

						if (ym != G0)
						{
							if (!mDBY)
							{
								if (ym > G0)
								{
									mLoadMenuPos --;
								}
								else
								{
									mLoadMenuPos ++;
								}

								mDBY = 6;
								SOUNDMOVE;
							}
							else
								mDBY --;
						}
						else
							mDBY = FALSE;

						if (mLoadMenuPos < 0)
							mLoadMenuPos = 0;
						if (mLoadMenuPos >= n )
							mLoadMenuPos = n - 1;

						if (mLoadMenuPos < mLoadMenuScrollPos)
							mLoadMenuScrollPos = mLoadMenuPos;
						if (mLoadMenuPos > mLoadMenuScrollPos + 3)
							mLoadMenuScrollPos = mLoadMenuPos - 3;

						if (mLoadMenuScrollPos > n - 4)
							mLoadMenuScrollPos = n - 4;
						if (mLoadMenuScrollPos < 0)
							mLoadMenuScrollPos = 0;

					}

					if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
					{
						mMode = FEPS_LOAD2MAIN;
						mLoadMenuSlide = SLIDE_TIME;
						mMainMenuSlide = 0;
						SOUNDCANCEL;
					}

					if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
					{
						mLoadMenuState = ELS_LOADING;
						mCounter = 0;
						SOUNDSELECT;
					}

					break;

				case ELS_LOADING:
					{
						if (mCounter == 3)
						{
							SINT	ret;

//							ret = MCSaveCareer("JCLTest", GAME.GetCareer(), TRUE);
//							printf("Save returned %d\n", ret);

							char	name[9];
							my_strncpy(name, McFileList[mLoadMenuPos].name + 12, 8);
							name[8] = 0;
							ret = MCLoadCareer(name, &CAREER);

//							printf("Load returned %d\n", ret);

							switch (ret)
							{
								case McErrNone:
									// pull some data out of the career file.
									mCharacter = CAREER.mCurrentCharacter;
									mPrison = CAREER.mCurrentPrison;
									mArena = CAREER.mCurrentArena;

									// set the volumes
									SOUND.DeclareVolumeChange();

									mLoadMenuState = ELS_LOADED;
									break;

								case McWrongVersion:
									mLoadMenuState = ELS_WRONG_VERSION;
									break;

								default:
									mLoadMenuState = ELS_FAILED;
									break;
							};
						}
					}
					break;

				case ELS_LOADED:
					GAME.ClearCheats();
					if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
					{
						mMode = FEPS_LOAD2CHAR;
						mLoadMenuSlide = SLIDE_TIME;
						mCharMenuSlide = 0;
						mCharRotTimer = 0;
						mCharWait = -1;
						SINT	c0;
						for (c0 = 0; c0 < 6; c0 ++)
						{
							mCharThings[c0].Reset(c0);
						}
						SOUNDSELECT;
					}
					break;

				case ELS_FAILED:
				case ELS_WRONG_VERSION:
					if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
					{
						mMode = FEPS_LOAD2MAIN;
						mLoadMenuSlide = SLIDE_TIME;
						mMainMenuSlide = 0;
						SOUNDCANCEL;
					}
					break;

				case ELS_INSERT:
					{

						if ((mCounter & 3) == 0)
						{
							SINT	ret = MCAcquireCard();

							if (ret == 0)
							{
								mCounter = 0;
								mLoadMenuState = ELS_START;
								break;
							}
							if (ret == McErrNotFormat)
							{
							 	mLoadMenuState = ELS_UNFORMATTED;
							 	break;
							}
						}

						if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
						{
							mMode = FEPS_LOAD2MAIN;
							mLoadMenuSlide = SLIDE_TIME;
							mMainMenuSlide = 0;
							SOUNDCANCEL;
						}
					}
					break;
			};
#endif
			break;

		case FEPS_LOAD2CHAR:
			mLoadMenuSlide --;
			mCharMenuSlide ++;
			if (mLoadMenuSlide == 0)
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
				if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
				{
					mMode = FEPS_CHAR2MAIN;
					mMainMenuSlide = 0;
					mCharMenuSlide = SLIDE_TIME;
					mCharSlide = G0;
					SOUNDCANCEL;
				}
				if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
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
				if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
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
				if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
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
							// Used to quick save here.
						}
					}
				}
				else
				{
					// NeedToUpdateSave
					if (mNeedToUpdateSave)
					{
						mMode = FEPS_ARENA2QSAVE;
						mNeedToUpdateSave = false;
						mQSaveMenuSlide = 0;
						mQSaveMenuPos = 1;
					}

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

					if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
					{
						mMode = FEPS_ARENA2PSNEW;
						mPrisonMenuSlide = 0;
		//				mPrisonSlide = G0;
						SOUNDCANCEL;
					}
					if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
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

					// debug - save
			//		if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_SQUARE))
			//		{
			//			mMode = FEPS_ARENA2QSAVE;
			//			mQSaveMenuSlide = 0;
			//			mQSaveMenuPos = 1;
			//			SOUNDSELECT;
			//		};
				}
			}
#ifdef JJS	// Cheat code.
			{
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
#endif
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
				mMode = FEPS_QSAVE;
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

		case FEPS_SAVE2ARENA:
			mSaveMenuSlide --;
			mArenaMenuSlide ++;
			if (mSaveMenuSlide == 0)
			{
				mMode = FEPS_ARENA;
				mCounter = 0;
				mArenaSlideStart = 0;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** QSAVE

		case FEPS_QSAVE:
			if (CAREER.mName[0] == 0)
			{
				mMode = FEPS_QSAVE2SAVE;
				mQSaveMenuSlide = SLIDE_TIME;
				mSaveMenuSlide = 0;
				mArenaMenuSlide = SLIDE_TIME;
			}
			else
			{
				// Got a name - so save.
				PCSaveCareer(CAREER.mName, &CAREER, true);
				mMode = FEPS_QSAVE2ARENA;
				//mMode = FEPS_ARENA;
				break;
			}
			break;

#if 0	// No menu any more - auto save.
			if (ym != G0)
			{
				if (!mDBY)
				{
					if (ym > G0)
					{
						mQSaveMenuPos --;
					}
					else
					{
						mQSaveMenuPos ++;
					}

					mDBY = 6;
					SOUNDMOVE;
				}
				else
					mDBY --;
			}
			else
				mDBY = FALSE;

			if (mQSaveMenuPos < 1)
				mQSaveMenuPos = 2;
			if (mQSaveMenuPos > 2)
				mQSaveMenuPos = 1;

			if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
			{
				switch(mQSaveMenuPos)
				{
				case 1:
					mMode = FEPS_QSAVE2SAVE;
					mQSaveMenuSlide = SLIDE_TIME;
					mSaveMenuSlide = 0;
					mArenaMenuSlide = SLIDE_TIME;
					break;

				case 2:
					mMode = FEPS_QSAVE2ARENA;
					mQSaveMenuSlide = SLIDE_TIME;
					mArenaMenuSlide = 0;
					break;

				default:
					break;
				};
				SOUNDSELECT;
			}

			if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
			{
				mMode = FEPS_QSAVE2ARENA;
				mQSaveMenuSlide = SLIDE_TIME;
				mArenaMenuSlide = 0;
				SOUNDCANCEL;
			}
			break;
#endif

		case FEPS_QSAVE2SAVE:
			mSaveMenuSlide ++;
			mQSaveMenuSlide --;
			mArenaMenuSlide --;
			if (mQSaveMenuSlide == 0)
			{
				mMode = FEPS_SAVE;
				mSaveFromArena = TRUE;

				if (CAREER.mName[0] == 0)
				{
					mSaveMenuState = ESS_ENTER_NAME;
					mSaveX = 0;
					mSaveY = 0;
					mSaveNameLen = 0;
					mSaveEnteredName = TRUE;
				}
				else
				{
					// Changed from scanning as there is no
					// need to scan for memory cards - I am 
					// making the assumption that a hard 
					// disk is available.
					mSaveMenuState = ESS_SAVING;	
					mSaveEnteredName = FALSE;
				}

				mCounter = 0;
			}
			break;

		// ****************************************************************************
		// ****************************************************************************
		// ** SAVE

		case FEPS_SAVE:
		{
			if (mSaveMenuState == ESS_ENTER_NAME)
			{
				PLATFORM.SetKeyChangeCallback(SaveGameKeyCaptureCallback);
			}
			else
			{
				PLATFORM.SetKeyChangeCallback(NULL);
			}

			BOOL	process_triangle = TRUE;
			switch (mSaveMenuState)
			{
				case ESS_ENTER_NAME:
					{
#if 0
						if (xm != G0)
						{
							if (!mDBX)
							{
								if (xm > G0)
								{
									mSaveX ++;
								}
								else
								{
									mSaveX --;
								}

								mDBX = 6;
								SOUNDMOVE;
							}
							else
								mDBX --;
						}
						else
							mDBX = FALSE;

						SINT	oy = mSaveY;

						if (ym != G0)
						{
							if (!mDBY)
							{
								if (ym > G0)
								{
									mSaveY --;
								}
								else
								{
									mSaveY ++;
								}

								mDBY = 6;
								SOUNDMOVE;
							}
							else
								mDBY --;
						}
						else
							mDBY = FALSE;

						if (mSaveY < 0)
							mSaveY += 5;
						if (mSaveY > 4)
							mSaveY -= 5;

						if ((mSaveY == 4) && (oy != 4))
						{
							if (mSaveX < 7)
								mSaveX = 0;
							else
								mSaveX = 1;
						}

						if (mSaveY != 4)
						{
							if (mSaveX < 0)
								mSaveX += 13;
							if (mSaveX > 12)
								mSaveX -= 13;
						}
						else
						{
							if (mSaveX < 0)
								mSaveX += 2;
							if (mSaveX > 1)
								mSaveX -= 2;
						}

						if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
						{

							if (mSaveY != 4)
							{
								char	c;

								switch (mSaveY)
								{
								case 0:	c = mSaveX + 'A'; break;
								case 1:	c = mSaveX + 'N'; break;
								case 2:	c = mSaveX + 'a'; break;
								case 3:	c = mSaveX + 'n'; break;
								}

								if (mSaveNameLen < 8)
								{
									CAREER.mName[mSaveNameLen] = c;
									mSaveNameLen ++;
									CAREER.mName[mSaveNameLen] = 0;

									SOUNDNAME;
								}
								else
								{
									SOUNDERROR;
								}

							}
							else
							{
								if (mSaveX == 0)
								{
									// backspace
									if (mSaveNameLen > 0)
									{
										mSaveNameLen --;
										CAREER.mName[mSaveNameLen] = 0;
										SOUNDNAME;
									}
									else
									{
										SOUNDERROR;
									}
								}
								else
								{
									// enter
									if (mSaveNameLen > 0)
									{
										mSaveMenuState = ESS_SAVING;	// No need for ESS_SCANNING.
										mCounter = 0;
										SOUNDSELECT;
									}
									else
									{
										SOUNDERROR;
									}
								}
							}
						}
#endif
					}
					break;

				case ESS_SCANNING:
					// There is no comparision on the 
					// PC for the functionality 
					// delivered by this case on the 
					// PSX. Jumping straight to save...
					mSaveMenuState = ESS_SAVING;
					break;

				case ESS_SAVING:
					if (mCounter == 3)
					{
						SINT	ret;
						ret = PCSaveCareer(CAREER.mName, &CAREER, (!mSaveEnteredName));
						switch (ret)
						{
						case 	McErrNone:
							mSaveMenuState = ESS_SAVED;
							break;
						//case 	McErrBlockFull:
						//	mSaveMenuState = ESS_NO_SPACE;
						//		break;
								
						case	McErrAlreadyExist:
							mSaveConfirmOverWriteMenuPos = 1;
							mSaveMenuState = ESS_NAME_TAKEN;
							break;

						default:
							mSaveMenuState = ESS_SAVED;
							//mSaveMenuState = ESS_FAILED;
							break;
						}
					}
					break;

				case ESS_SAVED:
					
				//	if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
				//	{
						if (mSaveFromArena)
						{
							mMode = FEPS_SAVE2ARENA;
							mArenaMenuSlide = 0;
						}
						else
						{
							mMode = FEPS_SAVE2OPTIONS;
							mOptionsMenuSlide = 0;
						}
						mSaveMenuSlide = SLIDE_TIME;
				//		SOUNDSELECT;
				//	}
					break;

				case ESS_FORMAT_FAILED:
				case ESS_FAILED:
					if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
					{
						// Not sure what to do here.
						mSaveMenuState = ESS_SAVED; // Say it saved and continue.
					}
					break;

				case ESS_UNFORMATTED:
					if (ym != G0)
					{
						if (!mDBY)
						{
							if (mSaveY == 0)
								mSaveY = 1;
							else
								mSaveY = 0;

							mDBY = 6;
							SOUNDMOVE;
						}
						else
							mDBY --;
					}
					else
						mDBY = FALSE;

					if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
					{
						if (mSaveY == 0)
						{
							// return
							if (mSaveFromArena)
							{
								mMode = FEPS_SAVE2ARENA;
								mArenaMenuSlide = 0;
							}
							else
							{
								mMode = FEPS_SAVE2OPTIONS;
								mOptionsMenuSlide = 0;
							}
							mSaveMenuSlide = SLIDE_TIME;
						}
						else
						{
							// format memory card
							mCounter = 0;
							mSaveMenuState = ESS_FORMATTING;
						}
						SOUNDSELECT;
					}

					break;

				case ESS_FORMATTING:
					// No need on PC.
					break;

				case ESS_NO_SPACE:
#ifdef JJS		// No space - this should be thought about. This doesn't really have the same meaning as it does PSX.
					if ((mCounter & 3) == 0)
					{
						SINT	ret = MCAcquireCard();

						if (ret == McErrCardNotExist)
						{
							mSaveMenuState = ESS_NO_CARD;
							break;
						}
					}
#endif
					break;
				case ESS_NO_CARD:
#ifdef JJS		// This state (no card during saving) can't happen on a PC, surely.
					if ((mCounter & 3) == 0)
					{
						SINT	ret = MCAcquireCard();

						if (ret != McErrCardNotExist)
						{
							mCounter = 0;
							mSaveMenuState = ESS_SCANNING;
							break;
						}
					}
#endif					
				case ESS_NAME_TAKEN:

					if (ym != G0)
					{
						if (!mDBY)
						{
							if (ym > G0)
							{
								mSaveConfirmOverWriteMenuPos --;
							}
							else
							{
								mSaveConfirmOverWriteMenuPos ++;
							}

							mDBY = 6;
							SOUNDMOVE;
						}
						else
							mDBY --;
					}
					else
						mDBY = FALSE;

					if (mSaveConfirmOverWriteMenuPos < 1)
						mSaveConfirmOverWriteMenuPos = 2;
					if (mSaveConfirmOverWriteMenuPos > 2)
						mSaveConfirmOverWriteMenuPos = 1;

					if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
					{
						switch(mSaveConfirmOverWriteMenuPos)
						{
						case 1:
							mSaveMenuState = ESS_ENTER_NAME;
							mCounter = 0;
							mSaveX = 0;
							mSaveY = 0;
							mSaveNameLen = 0;
							CAREER.mName[0] = 0;
							break;

						case 2:
							mCounter = 0;
							mSaveMenuState = ESS_SAVING;
							mSaveEnteredName = false;
							break;

						default:
							break;
						};
						SOUNDSELECT;
					}
					break;
				
			};
			if ((mSaveMenuState != ESS_SCANNING) &&
			    (mSaveMenuState != ESS_SAVING) &&
			    (process_triangle))
			{
				/*
				if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
				{
					// No longer take an interest in the keyboard input.
					PLATFORM.SetKeyChangeCallback(NULL);

					if (mSaveFromArena)
					{
						mMode = FEPS_SAVE2ARENA;
						mArenaMenuSlide = 0;
					}
					else
					{
						mMode = FEPS_SAVE2OPTIONS;
						mOptionsMenuSlide = 0;
					}
					mSaveMenuSlide = SLIDE_TIME;
					SOUNDCANCEL;

					if (mSaveMenuState == ESS_ENTER_NAME)
						CAREER.mName[0] = 0;   // reenter next time...
				}
				*/
			}

			break;
		}
		// ****************************************************************************
		// ****************************************************************************
		// ** MESSAGE

		case FEPS_MESSAGE:
			if ((CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY)) ||
			    (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE)))
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
					if (xm < G0)
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

			// PLAYER 1 (2, surely?)
			if ((!mMCharSelected[1]) && (ym != G0))
			{
				if (!mDBX2)
				{
					if (ym < G0)
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

			if (mMCharacter[1] < 0)
				mMCharacter[1] = 5;

			if (mMCharacter[1] > 5)
				mMCharacter[1] = 0;

			// slide characters
			mMCharSlide[0] = mMCharSlide[0] * G(0, 50000);
			if (GABS(mMCharSlide[0]) < G(0, 50))
				mMCharSlide[0] = G0;

			mMCharSlide[1] = mMCharSlide[1] * G(0, 50000);
			if (GABS(mMCharSlide[1]) < G(0, 50))
				mMCharSlide[1] = G0;

			if (mMCharSelected[0])
			{
				if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE, 0))
				{
					mMCharSelected[0] = FALSE;
					SOUNDCANCEL;
				}

				if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY, 0))
					if (mMCharSelected[1])
					{
						goon = TRUE;
						SOUNDSELECT;
					}
			}
			else
			{
				if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE, 0))
				{
					goback = TRUE;
				//	SOUNDCANCEL;
				}

				if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY, 0))
				{
					mMCharSelected[0] = TRUE;
					mMCharThings[0][mMCharacter[0]].Select();
				//	SOUNDSELECT;
				}

			}

			if (mMCharSelected[1])
			{
				if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE, 0))
				{
					mMCharSelected[1] = FALSE;
					//SOUNDCANCEL;
				}
			}
			else
			{
				if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE, 0))
				{
					goback = TRUE;
					SOUNDCANCEL;
				}

				if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY, 0))
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
				if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
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

				if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
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
				if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
				{
					mCounter = 0;
					mMode = FEPS_TS2GAME;
					SOUNDSELECT;
					SOUNDSTART;
				}
			}
#ifdef JJS	// Cheat code
			{
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
			}
#endif
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
					//if ((mOptionsMenuPos == 1) || (mOptionsMenuPos == 2))
					if ((mOptionsMenuPos == 0) || (mOptionsMenuPos == 1))
					{
						SINT 	dir;
						if (xm > G0)
							dir = 1;
						else
							dir = -1;


						//if (mOptionsMenuPos == 1)
						if (mOptionsMenuPos == 0)
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

#if 0
					//else if (mOptionsMenuPos == 3)
					else if (mOptionsMenuPos == 2)
					{
						if (xm > G0)
							CAREER.mStereo = TRUE;
						else
							CAREER.mStereo = FALSE;

						SOUND.DeclareVolumeChange();

						SOUNDNAME;
					}
					//else if (mOptionsMenuPos == 4)
					else if (mOptionsMenuPos == 3)
					{
						if (xm > G0)
							CAREER.mForceFeedback = TRUE;
						else
							CAREER.mForceFeedback = FALSE;
						SOUNDNAME;
					}
#endif

					mDBX = 6;
				}
				else
					mDBX --;
			}
			else
				mDBX = FALSE;

			if (mOptionsMenuPos < 0)
				mOptionsMenuPos = 1;
			if (mOptionsMenuPos > 1)
				mOptionsMenuPos = 0;

#if 0
			if (CAREER.mStarted)
			{
				if (mOptionsMenuPos < 0)
					mOptionsMenuPos = 3;
				if (mOptionsMenuPos > 3)
					mOptionsMenuPos = 0;
			}
			else
			{
				if (mOptionsMenuPos < 0)
					mOptionsMenuPos = 2;
				if (mOptionsMenuPos > 2)
					mOptionsMenuPos = 0;
			}
#endif
#ifdef JJS	// FORCE FEEDBACK!!!
			if ((!(PLATFORM.HaveDualShock())) && (mOptionsMenuPos == 4))
			{
				if (ym < G0)
				{
					if (CAREER.mStarted)
						mOptionsMenuPos = 5;
					else
						mOptionsMenuPos = 0;
				}
				else
					mOptionsMenuPos = 3;
			}	
#endif
			if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY)) //BUTTON_PSX_X))
			{
#if 0
				switch(mOptionsMenuPos)
				{
				/*case 0:
					mMode = FEPS_OPTIONS2CONTROLLER;
					mOptionsMenuSlide = SLIDE_TIME;
					mControllerMenuSlide = 0;
					mControllerSetting1 = CAREER.mControls1;
					mControllerSetting2	= CAREER.mControls2;
					SOUNDSELECT;
					break;
					*/
				case 3:
					mMode = FEPS_OPTIONS2SAVE;
					mOptionsMenuSlide = SLIDE_TIME;
					mSaveMenuSlide = 0;
					SOUNDSELECT;
					break;

				default:
					break;
				};
#endif
			}

			if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
			{
				mMode = FEPS_OPTIONS2MAIN;
				mOptionsMenuSlide = SLIDE_TIME;
				mMainMenuSlide = 0;
				SOUNDCANCEL;
			}
			break;

		case FEPS_OPTIONS2SAVE:
			mSaveMenuSlide ++;
			mOptionsMenuSlide --;
			if (mOptionsMenuSlide == 0)
			{
				mMode = FEPS_SAVE;
				mSaveFromArena = FALSE;

				if (CAREER.mName[0] == 0)
				{
					mSaveMenuState = ESS_ENTER_NAME;
					mSaveX = 0;
					mSaveY = 0;
					mSaveNameLen = 0;
					mSaveEnteredName = TRUE;					
				}
				else
				{
					mSaveMenuState = ESS_SCANNING;
					mSaveEnteredName = FALSE;
				}

				mCounter = 0;
			}
			break;

		case FEPS_SAVE2OPTIONS:
			mSaveMenuSlide --;
			mOptionsMenuSlide ++;
			if (mSaveMenuSlide == 0)
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
			if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
			{
				CAREER.mControls1 = mControllerSetting1;
				CAREER.mControls2 = mControllerSetting2;

				mMode = FEPS_CONTROLLER2OPTIONS;
				mControllerMenuSlide = SLIDE_TIME;
				mOptionsMenuSlide = 0;
				SOUNDSELECT;
			}

			// abort
			if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE))
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

			if ((CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY)) ||
			    (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE)))
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
void	CPCFEPrisonSelect::ProcessParticles()
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
	    (mMode == FEPS_LOAD) ||
	    (mMode == FEPS_SAVE))
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
void	CPCFEPrisonSelect::LoadAndRenderStart()
{
	char	text[256];
	SINT	sl;


	ENGINE.DrawSpritePC(mTexSplash[0],     0,   0, .999f, 256, 256);
	ENGINE.DrawSpritePC(mTexSplash[1],   256,   0, .999f, 256, 256);
	ENGINE.DrawSpritePC(mTexSplash[2],   512,   0, .999f, 128, 256);
	ENGINE.DrawSpritePC(mTexSplash[3],     0, 256, .999f, 256, 256);
	ENGINE.DrawSpritePC(mTexSplash[4],   256, 256, .999f, 256, 256);
	ENGINE.DrawSpritePC(mTexSplash[5],   512, 256, .999f, 128, 256);


	// Draw some text
	//CText::GetString(TEXT_PRESS_START, text);
	//SINT 	font = GAME.GetLanguage() == 0 ? 0 : 2;
	//sl = ENGINE.GetStringSize(font, text);
	//if (GAME.GetLanguage() == 1) // french is awkward
	//	ENGINE.DrawShadowedString(font, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 160 * PCMULTY, 0x007fbfff);
	//else
	//	ENGINE.DrawShadowedString(font, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 225 * PCMULTY, 0x007fbfff);
	
	
	//CText::GetString(TEXT_COPYRIGHT, text);
	//sl = ENGINE.GetStringSize(2, text);
	//ENGINE.DrawShadowedString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 180 * PCMULTY, 0x007fbfff);
	
#if 1
	if (GAME.IsCheatEnabled(CHEAT_VERSION))
	{
		sprintf(text, "Beta v1.2");


		sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 205 * PCMULTY, 0x007fbfff);
	}
#endif
	
	//sprintf(text,"** Alpha Build - Not for distribution outside Take 2 **");
	//sl = ENGINE.GetStringSize(2, text);
	//ENGINE.DrawShadowedString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 190 * PCMULTY, 0x007fbfff);
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderMain()
{

	SINT	yps;
	GINT	gy = G1;

	if ((mMode == FEPS_START2MAIN) ||
	    (mMode == FEPS_MAIN2START))
	{
		gy = G(SLIDE_TIME - mMainMenuSlide);
		gy = (gy * gy * gy) / (SLIDE_TIME * SLIDE_TIME);
		yps = (G(TITLE_BAR_Y) - (gy * G(150)) / SLIDE_TIME + GINT_HALF).Whole();

		SINT	c0;
		for (c0 = 0; c0 < 5; c0 ++)
		{
			DrawSpritePC(title[c0], 102.4 * float(c0), yps, .99f, 129, 128, 0xffffffff);
		}
	}

	SINT	c0;
	SINT	y = 100 - 13; // are: Added 'quit' option.

	for (c0 = 0; c0 < 5; c0 ++)
	{
		SINT 	c1;
		SINT	x = 112;

		GINT	gx;
		SINT	xoffs;

		BOOL	left = (mMode != FEPS_START2MAIN) &&
			           (mMode != FEPS_MAIN2CONFIRMQUIT) &&
					   (mMode != FEPS_CONFIRMQUIT2MAIN);

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
				colb = colr >> 2;
				if(colr > 127)
					colr = 127;
				if(colg > 127)
					colg = 127;
			}
#if 0
			if ((c0 == 1) && (PLATFORM.GetPadData(1)->valid))
			{
				colr = colg = colb = 64;
			}
#endif
			DWORD col = RGB_MAKE(colr*2,colg*2,colb*2);

			// Left side of menu bar
			DrawSpritePC(boxl, x  , y, .99f, 64, 64, col);
			x += 47;

			// Four middle sections
			for (c1 = 0; c1 < 4; c1 ++) // 4
			{
				DrawSpritePC(boxm, x  , y, .99f, 64, 64, col);
				x += 47;
			}

			// Right side of bar,
			DrawSpritePC(boxr, x  , y, .99f, 64, 64, col);

			//boxl->DrawAll();
			//boxm->DrawAll();
			//boxr->DrawAll();

			// draw text
			//!! Temporary!
			char	s[100];
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_SINGLE_PLAYER, s); break;
			case 1:	CText::GetString(TEXT_MULTIPLAYER, s); break;
			case 2:	CText::GetString(TEXT_OPTIONS, s); break;
			case 3:	CText::GetString(TEXT_CREDITS, s); break;
			case 4:	
				//CText::GetString(TEXT_QUIT, s); 
				strcpy(s, "Quit");
				break;
			};

			SINT	tcol;

			if (mMainMenuPos == c0)
				tcol = 0x00ffffff;
			else
				tcol = 0x00bfbfbf;

			SINT	sl = ENGINE.GetStringSize(0, s);
			ENGINE.DrawShadowedString(	0, 
										s, 
										PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs, 
										(y + 5) * PCMULTY + MENUTEXTOFFSET,
										tcol);
/*
			ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , (y + 5) * PCMULTY    , 0x0000003f);
			ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, (y + 5) * PCMULTY - 1, tcol);
*/
		}
		y += 30;
	}
	/*
	if (mMode == FEPS_MAIN)
	{
		char text[200];

		CText::GetString(TEXT_HELP1, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 220 * PCMULTY  + MENUTEXTOFFSET, 0x00ffffff);
	}
	*/

	if (GAME.IsCheatEnabled(CHEAT_VERSION))
	{
		char	text[256];
		SINT	sl;

		sprintf(text, "Beta v1.2");

		sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 463, 0x007fbfff);
	}
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderSingle()
{
	GINT	gy = G1;

	SINT	c0;
	SINT	y = 110;

	for (c0 = 0; c0 < 3; c0 ++)
	{
		SINT 	c1;
		SINT	x = 112;

		GINT	gx;
		SINT	xoffs;

		BOOL	left;

		left = (mMode == FEPS_SINGLE2CHAR) || (mMode == FEPS_SINGLE2LOAD) || (mMode == FEPS_SINGLE2ABANDON);

		gx = G(SLIDE_TIME - mSingleMenuSlide) - (G(left ? c0 : (4 - c0)) * G(1, 30000));

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

			if (mSingleMenuPos == c0)
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr >> 2;
				if(colr > 127)
					colr = 127;
				if(colg > 127)
					colg = 127;
			}


			if (	(	(c0 == 0) 
					&&	(!(CAREER.mStarted))
					)
				||	(	(c0 == 2) 
					&&	(!(PCNumFiles()))
					)
				)
			{
				colr = colg = colb = 64;
			}

			DWORD col = RGB_MAKE(colr*2,colg*2,colb*2);

			DrawSpritePC(boxl, x  , y, .99f, 64, 64, col);
			x += 47;

			for (c1 = 0; c1 < 4; c1 ++)
			{
				DrawSpritePC(boxm, x  , y, .99f, 64, 64, col);
				x += 47;
			}
			DrawSpritePC(boxr, x  , y, .99f, 64, 64, col);

			// draw text
			//!! Temporary!
			char	s[100];
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_CONTINUE_GAME, s); break;
			case 1:	CText::GetString(TEXT_NEW_GAME, s); break;
			case 2:	CText::GetString(TEXT_LOAD_GAME, s); break;
			};

			SINT	tcol;

			if (mSingleMenuPos == c0)
				tcol = 0x00ffffff;
			else
				tcol = 0x00bfbfbf;

			SINT	sl = ENGINE.GetStringSize(0, s);
			ENGINE.DrawShadowedString(	0, 
										s, 
										PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs, 
										(y + 5) * PCMULTY + MENUTEXTOFFSET,
										tcol);			// Foreground colour.

			/*
			ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs, (y + 5) * PCMULTY    , 0x0000003f);
			ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs, (y + 5) * PCMULTY - 1, tcol);
			*/
		}
		y += 30;
	}
/*
	if (mMode == FEPS_SINGLE)
	{
		char text[200];

		CText::GetString(TEXT_HELP1, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, (220 * PCMULTY) , 0x00ffffff);
	}
	*/
}


//******************************************************************************************
void	CPCFEPrisonSelect::RenderConfirmQuit()
{
	GINT	gy = G1;

	SINT	c0;
	SINT	y = 110;

	for (c0 = 0; c0 < 3; c0 ++)
	{
		SINT 	c1;
		SINT	x = 112;

		GINT	gx;
		SINT	xoffs;

		BOOL	left;

		left = TRUE;

		gx = G(SLIDE_TIME - mConfirmQuitMenuSlide) - (G(left ? c0 : (4 - c0)) * G(1, 30000));

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

			if (mConfirmQuitMenuPos + 1 == c0)
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr >> 2;
				if(colr > 127)
					colr = 127;
				if(colg > 127)
					colg = 127;
			}
			else if (c0 == 0)
			{
				colr = 80;
				colg = 100;
				colb = 60;
			}

			SINT iii = (GAME.GetLanguage()==0)?0:2;
			// Draw the button background.
			DWORD col = RGB_MAKE(colr*2,colg*2,colb*2);
			DrawSpritePC(boxl, x  - 24 * iii , y, .99f, 64, 64, col);	// Left side of menu bar
			x += 48;
			for (c1 = 0; c1 < (4 + iii); c1 ++)						// Four middle sections
			{
				DrawSpritePC(boxm, x - 24 * iii , y, .99f, 64, 64, col);
				x += 48;
			}
			DrawSpritePC(boxr, x - 24 * iii, y, .99f, 64, 64, col);	// Right side of bar.


			// draw text
			//!! Temporary!
			char	s[100];
			SINT	font = 0;

			switch (c0)
			{
			case 0:	
				CText::GetString(TEXT_PC_CONFIRM_QUIT_GAME, s); 
				font = 2; 
				break;
			case 1:	CText::GetString(TEXT_NO, s); break;
			case 2:	CText::GetString(TEXT_YES, s); break;
			};

			SINT	tcol;

			if ((mConfirmQuitMenuPos + 1 == c0) || (c0 == 0))
				tcol = 0x00ffffff;
			else
				tcol = 0x00bfbfbf;
			
			/*tcol = 0x007f7f7f;
			else
				tcol = 0x005f5f5f;*/

			SINT	sl = ENGINE.GetStringSize(font, s);
			ENGINE.DrawShadowedString(	font, 
										s, 
										PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs, 
										(y + 5) * PCMULTY + MENUTEXTOFFSET,
										tcol);
/*
			ENGINE.DrawShadowedString(font, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , (y + 5) * PCMULTY    , 0x0000003f);//0x001f0000);
			ENGINE.DrawShadowedString(font, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, (y + 5) * PCMULTY - 1, tcol);
*/
		}
		y += 30;
	}
}


//******************************************************************************************
void	CPCFEPrisonSelect::RenderAbandon()
{
	GINT	gy = G1;

	SINT	c0;
	SINT	y = 110;

	for (c0 = 0; c0 < 3; c0 ++)
	{
		SINT 	c1;
		SINT	x = 112;

		GINT	gx;
		SINT	xoffs;

		BOOL	left;

		left = (mMode == FEPS_ABANDON2CHAR);

		gx = G(SLIDE_TIME - mAbandonMenuSlide) - (G(left ? c0 : (4 - c0)) * G(1, 30000));

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

			if (mAbandonMenuPos + 1 == c0)
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr >> 2;
				if(colr > 127)
					colr = 127;
				if(colg > 127)
					colg = 127;
			}
			else if (c0 == 0)
			{
				colr = 80;
				colg = 100;
				colb = 60;
			}

			// Draw the button background.
			DWORD col = RGB_MAKE(colr*2,colg*2,colb*2);
			DrawSpritePC(boxl, x  , y, .99f, 64, 64, col);	// Left side of menu bar
			x += 48;
			for (c1 = 0; c1 < 4; c1 ++)						// Four middle sections
			{
				DrawSpritePC(boxm, x  , y, .99f, 64, 64, col);
				x += 48;
			}
			DrawSpritePC(boxr, x  , y, .99f, 64, 64, col);	// Right side of bar.


			// draw text
			//!! Temporary!
			char	s[100];
			SINT	font = 0;

			switch (c0)
			{
			case 0:	CText::GetString(TEXT_ABANDON_GAME, s); font = 2; break;
			case 1:	CText::GetString(TEXT_NO, s); break;
			case 2:	CText::GetString(TEXT_YES, s); break;
			};

			SINT	tcol;

			if ((mAbandonMenuPos + 1 == c0) || (c0 == 0))
				tcol = 0x00ffffff;
			else
				tcol = 0x00bfbfbf;
			
			/*tcol = 0x007f7f7f;
			else
				tcol = 0x005f5f5f;*/

			SINT	sl = ENGINE.GetStringSize(font, s);
			ENGINE.DrawShadowedString(	font, 
										s, 
										PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs, 
										(y + 5) * PCMULTY + MENUTEXTOFFSET,
										tcol);
/*
			ENGINE.DrawShadowedString(font, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , (y + 5) * PCMULTY    , 0x0000003f);//0x001f0000);
			ENGINE.DrawShadowedString(font, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, (y + 5) * PCMULTY - 1, tcol);
*/
		}
		y += 30;
	}
/*
	if (mMode == FEPS_ABANDON)
	{
		char text[200];

		CText::GetString(TEXT_HELP1, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, 256 * PCMULTX - sl / 2, 220 * PCMULTY , 0x007f7f7f);
	}
	*/
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderLoad()
{

	// draw a text box
	GINT	gh = G(mLoadMenuSlide) / SLIDE_TIME;
	SINT	col = (gh * 40).Whole();
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;

	SINT	w = (gh * 480).Whole();
	SINT	h = (gh * 150).Whole();

	//DrawBox(256 - w / 2, 160 - h / 2, w, h, 0.89f, col, col, col);

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();
	DrawBox(256 - w / 2, 160 - h / 2, w, h, 0.89f, col, col, col);

	int asl = 256;
	float astartTX = (256) - (asl / 2);
	float astartTY =  130;

	
	
	if (mMode == FEPS_LOAD)
	{
		char	s[100];
		SINT	sl;

		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();

		
		DrawRect(astartTX, astartTY, astartTX + asl + 12, astartTY + 73, 50, 0, 0, 0);
			
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();


		switch (mLoadMenuState)
		{
			case ELS_START:
				{
		//			CText::GetString(TEXT_LOAD_LOOKING_A, s);
		//			sl = ENGINE.GetStringSize(2, s);
		//			ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);

		//			ty = 120;

		//			CText::GetString(TEXT_LOAD_LOOKING_B, s);
		//			if (s[0] != '.')
		//			{
		//				sl = ENGINE.GetStringSize(2, s);
		//				ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, ty * PCMULTY , 0x003E9EFE);//0x001f4f7f);
		//				ty += 20;
		//			}

		//			CText::GetString(TEXT_LOAD_REMOVE, s);
		//			sl = ENGINE.GetStringSize(2, s);
		//			ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, ty * PCMULTY , 0x003E9EFE);//0x001f4f7f);
				}
				break;

			case ELS_SCANNING:
				break;

			case ELS_NONE_FOUND:
		//		{
		//			CText::GetString(TEXT_LOAD_NONE, s);
		//			sl = ENGINE.GetStringSize(2, s);
		//			ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);

					//CText::GetString(TEXT_LOAD_INSERT2_A, s);
					//sl = ENGINE.GetStringSize(2, s);
					//ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY , 0x003E9EFE);//0x001f4f7f);

					//CText::GetString(TEXT_LOAD_INSERT2_B, s);
					//if (s[0] != '.')
					//{
					//	sl = ENGINE.GetStringSize(2, s);
					//	ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 140 * PCMULTY , 0x003E9EFE);//0x001f4f7f);
					//}
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
		//		  }

				break;

			case ELS_SAVES_FOUND:
				{
					char	s[100];
					SINT	sl;

					CText::GetString(TEXT_LOAD_SELECT, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);

					SINT 	c0, nf = PCNumFiles();
					SINT	y = 130; // 120

					for (c0 = mLoadMenuScrollPos; c0 < mLoadMenuScrollPos + 6; c0 ++)
					{
						if (c0 < nf)
						{
							sl = ENGINE.GetStringSize(0, PCGetSaveNameByIndex(c0)->name);

							if (c0 == mLoadMenuPos)
							{
								SINT	colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
								SINT	colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
								SINT	colb = colr >> 2;

								if (colr > 127)
									colr = 127;
								if (colg > 127)
									colg = 127;
								if (colb > 127)
									colb = 127;

								SINT col = RGB_MAKE(colr*2, colg*2, colb*2);

								//ENGINE.DrawShadowedString(0, PCFileList[c0].name + 12, 256 * PCMULTX - sl / 2, y * PCMULTY, col);
								ENGINE.DrawShadowedString(0, PCGetSaveNameByIndex(c0)->name, 256 * PCMULTX - sl / 2, y * PCMULTY + 2, col);
							}
							else
							{
								//ENGINE.DrawShadowedString(0, PCFileList[c0].name + 12, 256 * PCMULTX - sl / 2, y * PCMULTY, 0x007f4f1f);
								ENGINE.DrawShadowedString(0, PCGetSaveNameByIndex(c0)->name, 256 * PCMULTX - sl / 2, y * PCMULTY + 2, 0x00FE9E3E);
							}
						}
						
						sl = 256;
						float startTX = (256) - (sl / 2);
						DrawLine(startTX,	y,	startTX + sl,	y,	5, 255, 255, 255);
						
						y += 12;
					}

					sl = 256;
					for (int rep = 2; rep --;)
					{
						float startTX = (256) - (sl / 2);
						float startTY =  130;
						DrawLine(startTX,	startTY,	startTX + sl,	startTY,	5, 255, 255, 255);
						DrawLine(startTX,	y,			startTX + sl,	y,			5, 255, 255, 255);
	
						DrawLine(startTX,			startTY,	startTX,			y, 5, 255, 255, 255);
						DrawLine(startTX + sl,		startTY,	startTX + sl,		y, 5, 255, 255, 255);

						// Scroll bar!
						DrawLine(startTX + sl + 12,	startTY,	startTX + sl + 12,	y, 5, 255, 255, 255);

						DrawLine(startTX + sl,		startTY,	startTX + sl + 12,	startTY,	5, 255, 255, 255);
						DrawLine(startTX + sl,		y,			startTX + sl + 12,	y,			5, 255, 255, 255);

						// Centre section.
						SINT h = y - startTY - 4;
						SINT sbdepth	= SINT(float(h) * (6.0f / float(max(PCNumFiles(), 6))) );
						SINT sbtop		= SINT(float(h - sbdepth) * float(mLoadMenuScrollPos) / float(max(PCNumFiles() - 6, 1)));
						DrawRect(startTX + sl + 2, startTY + 2 + sbtop, startTX + sl + 10, startTY + 2 + sbtop + sbdepth, 5, 255, 255, 255);
					}

					// arrows?
					if ((mArrowCounter & 31) < 20)
					{
						if (mLoadMenuScrollPos != 0)
						{
							//DrawSpritePC(arrow_u, 156 + ARROWOFFSETX, 120 + ARROWOFFSETY, 0.8f, 18, 14, 0x00ffffff);
							//DrawSpritePC(arrow_u, 356 + ARROWOFFSETX, 120 + ARROWOFFSETY, 0.8f, 18, 14, 0x00ffffff);
							DrawSpritePC(arrow_u, 156 + ARROWOFFSETX, 115 + ARROWOFFSETY, 0.8f, 18, 14, 0x00ffffff);
							DrawSpritePC(arrow_u, 356 + ARROWOFFSETX, 115 + ARROWOFFSETY, 0.8f, 18, 14, 0x00ffffff);
						}
						if (mLoadMenuScrollPos < PCNumFiles() - 6)
						{
							DrawSpritePC(arrow_d, 156 + ARROWOFFSETX, 203 + ARROWOFFSETY, 0.8f, 18, 14, 0x00ffffff);
							DrawSpritePC(arrow_d, 356 + ARROWOFFSETX, 203 + ARROWOFFSETY, 0.8f, 18, 14, 0x00ffffff);

							//DrawSpritePC(arrow_d, 156 + ARROWOFFSETX, 168 + ARROWOFFSETY, 0.8f, 18, 14, 0x00ffffff);
							//DrawSpritePC(arrow_d, 356 + ARROWOFFSETX, 168 + ARROWOFFSETY, 0.8f, 18, 14, 0x00ffffff);
						}
					}

					/*
					CText::GetString(TEXT_HELP1, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ELS_LOADING:
				{
				//	CText::GetString(TEXT_LOAD_LOADING, s);
				//	sl = ENGINE.GetStringSize(2, s);
				//	ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);

				//	CText::GetString(TEXT_LOAD_REMOVE, s);
				//	sl = ENGINE.GetStringSize(2, s);
				//	ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY , 0x003E9EFE);//0x001f4f7f);
				}
				break;

			case ELS_LOADED:

				{
				//	CText::GetString(TEXT_LOAD_LOADED, s);
				//	sl = ENGINE.GetStringSize(2, s);
				//	ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);
					/*
					CText::GetString(TEXT_HELP2, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ELS_FAILED:
				{
					CText::GetString(TEXT_LOAD_FAILED, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ELS_WRONG_VERSION:
				{
					CText::GetString(TEXT_LOAD_WRONG_VERSION, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ELS_INSERT:
				{
					CText::GetString(TEXT_LOAD_INSERT_A, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);

					CText::GetString(TEXT_LOAD_INSERT_B, s);
					if (s[0] != '.')
					{
						sl = ENGINE.GetStringSize(2, s);
						ENGINE.DrawShadowedString(2, s, 256 - sl / 2, 120 * PCMULTY , 0x003E9EFE);//0x001f4f7f);
					}
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ELS_UNFORMATTED:
				{
					CText::GetString(TEXT_SAVE_UNFORMATTED, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EFE);//0x001f4f7f);

					CText::GetString(TEXT_LOAD_INSERT2_A, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY , 0x003E9EFE);//0x001f4f7f);

					CText::GetString(TEXT_LOAD_INSERT2_B, s);
					if (s[0] != '.')
					{
						sl = ENGINE.GetStringSize(2, s);
						ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 140 * PCMULTY , 0x003E9EFE);//0x001f4f7f);
					}
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;
		};
	}
}
#define SetMLight(lm,n,r,g,b) (lm[0][n]=FtoPSX(r),lm[1][n]=FtoPSX(g),lm[2][n]=FtoPSX(b))
//******************************************************************************************
void	CPCFEPrisonSelect::RenderChar()
{
	SINT	c;
	c = (mCharMenuSlide * 128) / SLIDE_TIME;

	GINT	gy;
	SINT	yps;

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();

	gy = G(SLIDE_TIME - mCharMenuSlide);
	gy = (gy * gy) / (SLIDE_TIME * SLIDE_TIME);
	yps = (gy * G(195)).Whole() + 190;

	//	if (mMode == FEPS_CHAR)
	{
		// draw the character meshes
//		MATRIX lcol;

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

		// JCL - reset engine transforms
		ENGINE.SetupTransforms(ZERO_FVECTOR, ID_FMATRIX);

		if (mCharSlide != G0)
		{

//			m = ENGINE.GetNumberedMesh(FEPSM_ANGEL + character);
//			if (m)
			{
#if 0
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
				t->mPos = GVector(gpos * G(10), G0, G0); // G0

				t->Render();
			}
			character = (character + 1) % 6;
			gpos = gpos + G1;
		}

//		m = ENGINE.GetNumberedMesh(FEPSM_ANGEL + character);
//		if (m)
		{
#if 0
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
			t->Render();
		}
	}

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();

	DrawSpritePC(selchar, 154, yps - 5, .99f, 128, 64, 0x00ffffff, FALSE);
	DrawSpritePC(selchar, 256, yps - 5, .99f, 128, 64, 0x00ffffff, FALSE, PCMULTX, PCMULTY, 1);

	if (mMode == FEPS_CHAR)
	{
		// draw arrows?
		if ((mArrowCounter & 31) < 20)
		{
			DrawSpritePC(arrow_l, 178 + ARROWOFFSETX, 119 + ARROWOFFSETY, .99f, 14, 18, 0x00ffffff);
			DrawSpritePC(arrow_r, 318 + ARROWOFFSETX, 119 + ARROWOFFSETY, .99f, 14, 18, 0x00ffffff);
		}
	}


	// draw some text boxes
	SINT	col = c / 3;
	GINT	gh = G(mCharMenuSlide) / SLIDE_TIME;
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;
	SINT	h = (gh * 130).Whole();

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();

	DrawBox(16 , 100, 150, h, 0.95f, col, col, col);
	DrawBox(346, 100, 150, h, 0.95f, col, col, col);

	// and some assorted text
	char	text[100];
	SINT	sl;

	if (mMode == FEPS_CHAR)
	{
		//*****
		// Left

		// name
		CText::GetString(TEXT_ANGEL + mCharacter, text);
		sl = ENGINE.GetStringSize(0, text);
		ENGINE.DrawShadowedString(0, text, (93) * PCMULTX  - sl / 2, 107 * PCMULTY, 0x00ffbf7f);

		// bio
		CText::GetString(TEXT_ANGEL_COMMENT + mCharacter, text);
		sl = ENGINE.GetStringSize(2, text);
		DrawDynamicText(2, text, (93 ) * PCMULTX - sl / 2, 125 * PCMULTY, 64, 96, 128, (mCounter - mCharSlideStart) * 2);

		// levels & stars
		DrawSpritePC(qualify, 40  , 142 - 8, .9f, 64, 64, 0x00ffffff);
		DrawSpritePC(gold   , 40  , 171 - 8, .9f, 64, 64, 0x00ffffff);
		DrawSpritePC(goldlt , 40  , 200 - 8, .9f, 64, 64, 0x00ffffff);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumLevelsCompleted);
		DrawDynamicText(0, text, 90 * PCMULTX + 8, 149 * PCMULTY - 2, 64, 96, 128, (mCounter - mCharSlideStart) * 2);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumGoldStars);
		DrawDynamicText(0, text, 90 * PCMULTX + 8, 178 * PCMULTY - 2, 64, 96, 128, (mCounter - mCharSlideStart) * 2);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumLostToysTimes);
		DrawDynamicText(0, text, 90 * PCMULTX + 8, 207 * PCMULTY - 2, 64, 96, 128, (mCounter - mCharSlideStart) * 2);


		//******
		// Right

		CText::GetString(TEXT_STATS, text);
		sl = ENGINE.GetStringSize(0, text);
		DrawDynamicText(0, text, (421)  * PCMULTX - sl / 2, 110 * PCMULTY, 127, 80, 64, (mCounter * 2));

		// stats
		CText::GetString(TEXT_STRENGTH, text);
		DrawDynamicText(2, text, 355 * PCMULTX, 140 * PCMULTY, 48, 92, 48, (mCounter * 2)    );
		CText::GetString(TEXT_SPEED, text);
		DrawDynamicText(2, text, 355 * PCMULTX, 156 * PCMULTY, 48, 92, 48, (mCounter * 2) - 3);
		CText::GetString(TEXT_AGILITY, text);
		DrawDynamicText(2, text, 355 * PCMULTX, 172 * PCMULTY, 48, 92, 48, (mCounter * 2) - 6);
		CText::GetString(TEXT_STAMINA, text);
		DrawDynamicText(2, text, 355 * PCMULTX, 188 * PCMULTY, 48, 92, 48, (mCounter * 2) - 9);

		sprintf(text, "````");
		text[get_char_stats(mCharacter, 0)] = 0;
		DrawDynamicText(0, text, 450 * PCMULTX, 140 * PCMULTY, 128, 32, 32, (mCounter - mCharSlideStart));
		sprintf(text, "````");
		text[get_char_stats(mCharacter, 1)] = 0;
		DrawDynamicText(0, text, 450 * PCMULTX, 156 * PCMULTY, 128, 32, 32, (mCounter - mCharSlideStart));
		sprintf(text, "````");
		text[get_char_stats(mCharacter, 2)] = 0;
		DrawDynamicText(0, text, 450 * PCMULTX, 172 * PCMULTY, 128, 32, 32, (mCounter - mCharSlideStart));
		sprintf(text, "````");
		text[get_char_stats(mCharacter, 3)] = 0;
		DrawDynamicText(0, text, 450 * PCMULTX, 188 * PCMULTY, 128, 32, 32, (mCounter - mCharSlideStart));
	}
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderPrisonNew()
{
	GINT	gy;
	SINT	yps;

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();	


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
			DWORD col = 0x00404040;

			if (prison == mPrison)
			{
				if (can_select_prison(prison, mCharacter) || ((mPrisonBonus) && (mPrisonBonusSlide > GINT_HALF)))
					col = 0x00ffffff;
				else
					col = 0x00808080;
			}

			#define PRIS_GAP	(160 + 150)

			if (mPrisonBonusSlide == G0)
			{
				DrawBorderClip((gpos * G(PRIS_GAP)).Whole() + 176, yps, 160, 0.90f);
				if (((mMode != FEPS_PSNEW2ARENA) && (mMode != FEPS_ARENA2PSNEW)) || (prison != mPrison))
					DrawSpritePC(ps[prison],(gpos * G(PRIS_GAP)).Whole() + 176, yps, 0.91f, 200, 150, col);
			}
			else if (mPrisonBonusSlide == G1)
			{
				if ((mMode != FEPS_PSNEW2ARENA) && (mMode != FEPS_ARENA2PSNEW) && (prison == mPrison))
				{
					DrawBorderClip((gpos * G(PRIS_GAP)).Whole() + 176, yps, 160, 0.90f);
					DrawSpritePC(ps[FEPST_PR_BONUS - FEPST_PR_ECO],(gpos * G(PRIS_GAP)).Whole() + 176, yps, 0.91f, 200, 150, col);
				}
			}
			else
			{
				if ((mPrisonBonusSlide < GINT_HALF) || (prison == mPrison))
				{
					DrawBorderClipRot(FEPST_PR_ECO + prison, FEPST_PR_BONUS, (gpos * G(PRIS_GAP)).Whole() + 176, yps, 160, 2, col, col, col, mPrisonBonusSlide, !(can_select_prison(prison, mCharacter)));
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
				DrawSpritePC(arrow_d, 256 - 7  + ARROWOFFSETX , 150 + ARROWOFFSETY, .89f, 18, 14, 0x00ffffff);
			}
			else
			{
				if (mPrison != 0)
					DrawSpritePC(arrow_l, 138  + ARROWOFFSETX , 119 + ARROWOFFSETY, .89f, 14, 18, 0x00ffffff);
				if (mPrison != 9)
					DrawSpritePC(arrow_r, 358  + ARROWOFFSETX , 119 + ARROWOFFSETY, .89f, 14, 18, 0x00ffffff);
				if (can_select_bonus_prison(mCharacter))
					DrawSpritePC(arrow_u, 256 - 7 + ARROWOFFSETX , 90 + ARROWOFFSETY, .89f, 18, 14, 0x00ffffff);
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
		DrawBox(16 , 180 + (76 - h), 232, h, 0.99f, col, col, col);
		DrawBox(262, 180 + (76 - h), 232, h, 0.99f, col, col, col);
	}

	white->DrawAll();

	// and some text
	char	text[100];

	prison = mPrison;
	if (mPrisonBonus)
		prison = 10;

	if (mMode == FEPS_PSNEW)
	{
		CText::GetString(TEXT_PRN_ECO + prison, text);
		DrawDynamicText(0, text , 25 * PCMULTX, 185 * PCMULTY, 32, 128, 32, (mCounter - mPrisonSlideStart) * 2);

		if (can_select_prison(prison, mCharacter))
		{
			// Prison Stats
			CText::GetString(TEXT_SECURITY_LEVEL, text);
			DrawDynamicText(2, text, 25 * PCMULTX, 200 * PCMULTY, COL_GOLD, (mCounter - mPrisonAccessStart) * 2);

			CText::GetString(TEXT_INMATES, text);
			DrawDynamicText(2, text, 25 * PCMULTX, 212 * PCMULTY, COL_GOLD, (mCounter - mPrisonAccessStart) * 2);

			CText::GetString(TEXT_ESCAPE_ATTEMPTS, text);
			DrawDynamicText(2, text, 25 * PCMULTX, 224 * PCMULTY, COL_GOLD, (mCounter - mPrisonAccessStart) * 2);

#if DISP_MODE == MODE_PAL
			CText::GetString(TEXT_SUCCESSFUL_ESCAPES, text);
			DrawDynamicText(2, text, 25 * PCMULTX, 236 * PCMULTY, COL_GOLD, (mCounter - mPrisonAccessStart) * 2);
#endif

			sprintf(text, "---");

			if (!mPrisonBonus)
				sprintf(text, "%d", 4 - (mPrison / 3));
			DrawDynamicText(2, text, 200 * PCMULTX, 200 * PCMULTY, COL_BLUE, (mCounter - mPrisonSlideStart));

			if (!mPrisonBonus)
				sprintf(text, "%d", (mPrison + 10) * 29);
			DrawDynamicText(2, text, 200 * PCMULTX, 212 * PCMULTY, COL_BLUE, (mCounter - mPrisonSlideStart));

			if (!mPrisonBonus)
				sprintf(text, "%d", (mPrison + 10) * 7);
			DrawDynamicText(2, text, 200 * PCMULTX, 224 * PCMULTY, COL_BLUE, (mCounter - mPrisonSlideStart));

#if DISP_MODE == MODE_PAL
			if (!mPrisonBonus)
				sprintf(text, "%d", ((mPrison + 10) * 7 * (9 - mPrison)) / 18);
			DrawDynamicText(2, text, 200 * PCMULTX, 236 * PCMULTY, COL_BLUE, (mCounter - mPrisonSlideStart));
#endif

			// Player Stats
			CText::GetString(TEXT_ARENAS_COMPLETED, text);
			DrawDynamicText(0, text, 270 * PCMULTX, 185 * PCMULTY, 32, 128, 32, (mCounter - mPrisonAccessStart) * 2);

			SINT nlip = CLevelStructure::GetNumLevelsInPrison(prison);
			if ((prison == 9) && (mCharacter != 5))
				nlip --;	// only doctor can access all levels in Alpha Star...
			sprintf(text, "%d / %d", get_levels_completed(prison, mCharacter), nlip);
			DrawDynamicText(0, text, 270 * PCMULTX, 199 * PCMULTY, COL_BLUE, (mCounter - mPrisonSlideStart));
		}
		else
		{
			CText::GetString(TEXT_ACCESS_DENIED, text);
			DrawDynamicText(2, text, 25 * PCMULTX, 200 * PCMULTY, 128, 32, 32, (mCounter - mPrisonAccessStart));

			CText::GetString(TEXT_ARENAS_NEEDED_TO_ACCESS, text);
			DrawDynamicText(2, text, 270 * PCMULTX, 185 * PCMULTY, 128, 32, 32, (mCounter - mPrisonAccessStart) * 2);

			sprintf(text, "%d", CLevelStructure::GetNumLevelsToAccess(mPrison));
			DrawDynamicText(0, text, 270 * PCMULTX, 199 * PCMULTY, COL_BLUE, (mCounter - mPrisonSlideStart));
		}

//		CText::GetString(TEXT_TOTAL_ARENAS_COMPLETED, text);
//		DrawDynamicText(0, text, 270, 215, 32, 128, 32, (mCounter) * 2);
//		sprintf(text, "%d", CAREER.mCharCareers[mCharacter].mNumLevelsCompleted);
//		DrawDynamicText(0, text, 270, 230, COL_BLUE, (mCounter));

		// levels & stars
		DrawSpritePC(qualify,	265  , 215, .9f, 64, 64, 0x00ffffff);
		DrawSpritePC(gold,		338  , 215, .9f, 64, 64, 0x00ffffff);
		DrawSpritePC(goldlt,	410  , 215, .9f, 64, 64, 0x00ffffff);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumLevelsCompleted);
		DrawDynamicText(2, text, 311 * PCMULTX + 4, 222 * PCMULTY + 10, 64, 96, 128, mCounter);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumGoldStars);
		DrawDynamicText(2, text, 384 * PCMULTX + 4, 222 * PCMULTY + 10, 64, 96, 128, mCounter);

		sprintf(text,"%d", CAREER.mCharCareers[mCharacter].mNumLostToysTimes);
		DrawDynamicText(2, text, 456 * PCMULTX + 4, 222 * PCMULTY + 10, 64, 96, 128, mCounter);


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

	SINT	total_width = (47 * (num - 1)) + (5 * (num - 1));
	SINT	basex = 256 - (total_width / 2) + (52 * l);

	GINT	d = G(1) - offs;
	if (d < G0)
		d = G0;
	if (d > G1)
		d = G1;

	d *= G(23);
	d *= d;

	*x = basex + d.Whole();
}

void	CPCFEPrisonSelect::RenderArena()
{
	SINT	prison;

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();


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

	// 96
	#define AGC		0x60

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
		DrawBorder(176, 125, 160, 80, 0.91f, 40, 40, 40);
		DrawSpritePC(ps[prison],176, 125, 0.95f, 200, 150, RGB_MAKE(AGC,AGC,AGC));
	}
	else
	{
		// Cross fade to greyscale
		SINT	xpos;
		SINT	ypos;

		if ((mMode == FEPS_QSAVE2SAVE) || (mMode == FEPS_SAVE2ARENA))
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
		SINT	colb = ((G(255 - AGC) * (G1 - sc)) + G(AGC)).Whole();
		SINT	colc = (G( 40) *       sc ).Whole();

		DrawBorder(xpos, ypos, 160, 80, 0.91f, colc, colc, colc);

		//JCLPCTODO
		DrawSpritePC(ps[prison],xpos, ypos, 0.95f, 200, 150, RGB_MAKE(colb, colb, colb));
	}

	// Arena icons
	SINT	n = CLevelStructure::GetNumLevelsInPrison(prison);

	SINT	c0;
	for (c0 = 0; c0 < n; c0 ++)
	{
		SINT		x, y;
		SLevelInfo	*li = CLevelStructure::GetLevelInfo(prison, c0);

		if ((mMode == FEPS_QSAVE2SAVE) || (mMode == FEPS_SAVE2ARENA))
			sc = qsave_sc;
		else
			sc = G(arena_slide) / G(SLIDE_TIME);

		sc *= G(3);
		sc -= G1;
		sc -= G(c0) * G(0, 7000);

		get_arena_pos(n, c0, &x, &y, sc);

		SINT	col = 92;
		GINT	scale = G1;

		x -= 26;
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
					DrawSpritePC(arrow_l, x - 13  + ARROWOFFSETX , y + 8 + ARROWOFFSETY, .85f, 14, 18, 0x00ffffff);
					DrawSpritePC(arrow_r, x + 47  + ARROWOFFSETX , y + 8 + ARROWOFFSETY, .85f, 14, 18, 0x00ffffff);
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
			DrawSpritePCAdditive(halo, x - 4, y - 3, .88f - 0.0005f * c0, 75, 75, 0x00FFFFFF);
		}

		if(colr > 127)
			colr = 127;
		if(colg > 127)
			colg = 127;
		if(colb > 127)
			colb = 127;

		DWORD colpc = RGB_MAKE(colr*2,colg*2,colb*2);
		DrawSpritePC(ar[li->mType], x  , y, .89f - 0.0005f * c0, 64, 64, colpc);

//		CTexture::DrawAllTextures();
//		CTexture::DrawAllTexturesAdditive();
	}

	// text boxes

	sc = G(arena_slide) / G(SLIDE_TIME);

	// hmm - fuck about with SIN/COS to smooth it.
	sc = (GSIN((sc * G_PI) - G_PI_2) / 2) + GINT_HALF;

	SINT	xpos, ypos, wid, hgt;

	if ((mMode == FEPS_QSAVE2SAVE) || (mMode == FEPS_SAVE2ARENA))
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
	DrawBox(xpos, ypos, wid, hgt, 0.98f, 40, 40, 40);

	if ((mMode == FEPS_QSAVE2SAVE) || (mMode == FEPS_SAVE2ARENA))
	{
		xpos = 16 - 24;
	}
	else
	{
		xpos = 16 - (G(24) * sc).Whole();
	}

	DrawBox(xpos, ypos, wid, hgt, 0.98f, 40, 40, 40);

	if ((mMode == FEPS_QSAVE2SAVE) || (mMode == FEPS_SAVE2ARENA))
	{
		DrawBox(0 , 208 + ((G1 - qsave_sc) * G(250)).Whole(), 512, 300, 0.98f, 40, 40, 40);
	}
	else
	{
		SINT	col = (arena_slide * 128) / SLIDE_TIME / 3;
		GINT	gh = G(arena_slide) / SLIDE_TIME;
		gh = G1 - gh;
		gh = (gh * gh);
		gh = G1 - gh;
		SINT	h = (gh * 300).Whole();
		DrawBox(-30 , 208 + (300 - h), 512 + 30, 100, 0.98f, col, col, col);
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
		sl = ENGINE.GetStringSize(2, text);
		DrawDynamicText(2, text, 256 * PCMULTX - sl / 2, 130 * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 3);

		// How to qualify
		CText::GetString(TEXT_QUALIFY, text);
		DrawDynamicText(2, text, 363 * PCMULTX, 130 * PCMULTY, COL_GOLD, (mCounter * 2));

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
		DrawDynamicText(f, text, 363 * PCMULTX, 146 * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 2);

		// Your Best Time

		if (li->mQualify == LIFIN_PAR)
		{
			// golf
			CText::GetString(TEXT_BEST_SCORE, text);
			sl = ENGINE.GetStringSize(2, text);
			DrawDynamicText(2, text, 144 * PCMULTX - sl, 130 * PCMULTY, COL_GOLD, (mCounter * 2));

			if (lc->mBestTime == 0xffff)
				sprintf(text, "---");
			else
			{
				sprintf(text, "%d", lc->mBestTime);
			}
			sl = ENGINE.GetStringSize(2, text);
			DrawDynamicText(2, text, 144 * PCMULTX - sl, 146 * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}
		else
		{
			CText::GetString(TEXT_BEST_TIME, text);
			sl = ENGINE.GetStringSize(2, text);
			DrawDynamicText(2, text, 144 * PCMULTX - sl, 130 * PCMULTY, COL_GOLD, (mCounter * 2));

			if (lc->mBestTime == 0xffff)
				sprintf(text, "--:--.--");
			else
			{
				SINT	minutes = lc->mBestTime / 60 / 25;
				SINT	seconds = (lc->mBestTime / 25) % 60;
				SINT	milli   = (lc->mBestTime % 25) * 4;

				sprintf(text, "%d:%02d.%02d", minutes, seconds, milli);
			}
			sl = ENGINE.GetStringSize(2, text);
			DrawDynamicText(2, text, 144 * PCMULTX - sl, 146 * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}

		// Number of Attempts
		CText::GetString(TEXT_NUM_ATTEMPTS, text);
		sl = ENGINE.GetStringSize(2, text);
		DrawDynamicText(2, text, 144 * PCMULTX - sl, 162 * PCMULTY, COL_GOLD, (mCounter * 2) - 3);

		if (lc->mNumAttempts == 255)
		{
			CText::GetString(TEXT_LOTS, text);
			sl = ENGINE.GetStringSize(2, text);
			DrawDynamicText(2, text, 144 * PCMULTX - sl, 178 * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}
		else
		{
			sprintf(text, "%d", lc->mNumAttempts);
			sl = ENGINE.GetStringSize(2, text);
			DrawDynamicText(2, text, 144 * PCMULTX - sl, 178 * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
		}

		//if ((mMode != FEPS_QSAVE) && (mMode != FEPS_MESSAGE))
		if (mMode != FEPS_MESSAGE)
		{
#define AR_BOT_Y	213

			// How to get gold
			CText::GetString(TEXT_GOLD, text);
			DrawDynamicText(2, text, 30 * PCMULTX, AR_BOT_Y * PCMULTY, COL_GOLD, (mCounter * 2) - 6 );
			sl = ENGINE.GetStringSize(2, text); // for below

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
			DrawDynamicText(f, text, (30 + 20) * PCMULTX + sl, AR_BOT_Y * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 2);


			// Lost Toys Best Time
			if ((lc->mFinished == ECL_GOLD)  || (lc->mFinished == ECL_LT))
			{
				if (li->mType == LILT_GOLF)
					CText::GetString(TEXT_LOST_SCORE, text);
				else
					CText::GetString(TEXT_LOST_TOYS_TIME, text);
				DrawDynamicText(2, text, 290 * PCMULTX, AR_BOT_Y * PCMULTY, COL_GOLD, (mCounter * 2) - 6 );
				sl = ENGINE.GetStringSize(2, text); // for below

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
				DrawDynamicText(f, text, (290 + 20) * PCMULTX + sl, AR_BOT_Y * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
			}
		}
			
		// CODE??
		if (lc->mFinished == ECL_LT)
		{
			sprintf(text, "%08X", get_code2(lc->mBestTime, (prison << 4) + mArena, CAREER.mCodeKey, prison));
			DrawDynamicText(f, text, 363 * PCMULTX, 178 * PCMULTY, COL_BLUE, (mCounter - mArenaSlideStart) * 2);
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
		sl = ENGINE.GetStringSize(2, text);
		DrawDynamicText(2, text, 256 * PCMULTX - (sl / 2), 146 * PCMULTY, COL_GOLD, (mCounter - mArenaSlideStart) * 3);

		// Draw other stuff
		if (lc->mFinished == ECL_LT)
		{
			DrawSpritePC(goldlt , 235, 160, .89f, 64, 64, 0x00FFFFFF);
			DrawSpritePC(qualify, 284, 160, .89f, 64, 64, 0x00FFFFFF);
			DrawSpritePC(gold   , 187, 160, .89f, 64, 64, 0x00FFFFFF);
		}
		else
		{
			if ((lc->mFinished == ECL_YES) || (lc->mFinished == ECL_GOLD))
			{
				DrawSpritePC(qualify, 266, 160, .89f, 64, 64, 0x00FFFFFF);
			}
			if ((lc->mFinished == ECL_GOLD))
			{
				DrawSpritePC(gold, 204, 160, .89f, 64, 64, 0x00FFFFFF);
			}
		}
	}
	/*
	if (mMode == FEPS_ARENA && (mArenaPendingVictory == -1))
	{
		char text[200];
		
		CText::GetString(TEXT_HELP4, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
			ENGINE.DrawShadowedString(2, text, 256 * PCMULTX - (sl / 2), 225 * PCMULTY, 0x00ffffff);
	}
	*/	
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderPS2Game()
{
#ifdef JJS	// RenderPS2Game?
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
void	CPCFEPrisonSelect::RenderQSave()
{
	return; 


#if 0
	//SINT	yps;
	GINT	gy = G1;

	SINT	c0;
	SINT	y = 126;

	for (c0 = 0; c0 < 3; c0 ++)
	{
		SINT 	c1;
		SINT	x = 112;

		GINT	gx;
		SINT	xoffs;

		BOOL	left;

		left = mMode == FEPS_QSAVE2SAVE;

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
			case 0:	CText::GetString(TEXT_QSAVE_GAME, s); break;
			case 1:	CText::GetString(TEXT_YES, s); break;
			case 2:	CText::GetString(TEXT_NO, s); break;
			};

			SINT	tcol;

			if ((mQSaveMenuPos == c0) || (c0 == 0))
				tcol = 0x00FEFEFE;
			else
				tcol = 0x00BEBEBE;

			SINT	sl = ENGINE.GetStringSize(0, s);
			ENGINE.DrawShadowedString(	0, 
										s, 
										PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs, 
										(y + 5) * PCMULTY,
										tcol,			// Foreground colour.
										0.05f);			// Z

			/*
			ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1, tcol, 0);
			ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5    , 0x001f0000, 0);
			*/

			// Draw Bars
			SINT	colr = 60, colg = 72, colb = 100;

			if (mQSaveMenuPos == c0)
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr / 4;
			}
			else if (c0 == 0)
			{
				colr = 80;
				colg = 100;
				colb = 60;
			}

			if(colr > 127)
				colr = 127;
			if(colg > 127)
				colg = 127;
			if(colb > 127)
				colb = 127;

			// Left side of menu bar
			SINT col = RGB_MAKE(colr*2, colg*2, colb*2);
			DrawSpritePC(boxl, x  , y, .09f, 64, 64, col);
			x += 47;
			// Four middle sections
			for (c1 = 0; c1 < 4; c1 ++) // 4
			{
				DrawSpritePC(boxm, x  , y, .09f, 64, 64, col);
				x += 47;
			}
			// Right side of bar,
			DrawSpritePC(boxr, x  , y, .09f, 64, 64, col);
		}
		y += 30;
	}
/*
	if (mMode == FEPS_QSAVE)
	{
		char text[200];

		CText::GetString(TEXT_HELP1, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, 256 *PCMULTX - sl / 2, 220 *PCMULTY  + MENUTEXTOFFSET, 0x00FFFFFF);//7f7f7f);
	}
	*/
#endif
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderSave()
{
	// draw a text box
	GINT	gh = G(mSaveMenuSlide) / SLIDE_TIME;
	SINT	col = (gh * 40).Whole();
	gh = G1 - gh;
	gh = (gh * gh);
	gh = G1 - gh;

	SINT	w = (gh * 480).Whole();
	SINT	h = (gh * 150).Whole();

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();
	
	DrawBox(256 - w / 2, 160 - h / 2, w, h, 0.79f, col, col, col);

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();

	SINT 	ty;
	

	if (mMode == FEPS_SAVE)
	{
		char	s[100];
		SINT	sl;
		
		switch (mSaveMenuState)
		{
			case ESS_ENTER_NAME:
				{
					CTexture::DrawAllTextures();
					CTexture::DrawAllTexturesAdditive();
					
					sl = ENGINE.GetStringSize(0, "W");
					sl = ((sl + 10) * 9)/PCMULTX;
					float startTX = (256) - (sl/2);
					float startTY =  130;

					DrawRect((startTX), (startTY) - 8, startTX + (sl), startTY + 8, 50, 0, 0, 0);
					//DrawRect((startTX), (startTY) - 8, startTX + (sl), startTY + 8, 50, 0, 0, 0);

					CText::GetString(TEXT_ENTER_NAME, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY,  0x00FE9E3E);//0x007f4f1f); //90
					/*
					CText::GetString(TEXT_HELP1, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY, 0x007f7f7f);
					*/
					// draw current name
					if (((mCounter & 31) < 20) && (mSaveNameLen < 8))
						sprintf(s, "%s_", CAREER.mName);
					else
						sprintf(s, "%s", CAREER.mName);

					sl = ENGINE.GetStringSize(0, CAREER.mName);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 126 * PCMULTY, 0x00DEDE3E); //106
					
					sl = ENGINE.GetStringSize(0, "W");
					sl = ((sl + 10) * 9)/PCMULTX;
					startTX = (256) - (sl/2);
					startTY =  130;
					
				
					DrawLine((startTX), (startTY) - 8, startTX + (sl), startTY - 8, 5, 255, 255, 255);
					DrawLine((startTX), (startTY) + 8, startTX + (sl), startTY + 8, 5, 255, 255, 255);

					DrawLine((startTX), (startTY) - 8, startTX, startTY + 8, 5, 255, 255, 255);
					DrawLine((startTX + (sl)), (startTY) - 8, startTX + (sl), startTY + 8, 5, 255, 255, 255);

					DrawLine((startTX), (startTY) - 8, startTX + (sl), startTY - 8, 5, 255, 255, 255);
					DrawLine((startTX), (startTY) + 8, startTX + (sl), startTY + 8, 5, 255, 255, 255);

					DrawLine((startTX), (startTY) - 8, startTX, startTY + 8, 5, 255, 255, 255);
					DrawLine((startTX + (sl)), (startTY) - 8, startTX + (sl), startTY + 8, 5, 255, 255, 255);

					CTexture::DrawAllTextures();
					CTexture::DrawAllTexturesAdditive();

#if 0
					// draw character grid
					SINT	cy = 122;
					SINT	c0, c1;
					SINT	colr, colg, colb;

					s[1] = 0; // fake string

					for (c1 = 0; c1 < 4; c1 ++)
					{
						SINT	cx = 152;
						char 	c;

						switch (c1)
						{
						case 0:	c = 'A'; break;
						case 1:	c = 'N'; break;
						case 2:	c = 'a'; break;
						case 3:	c = 'n'; break;
						}

						for (c0 = 0; c0 < 13; c0 ++)
						{
							if ((mSaveX == c0) && (mSaveY == c1))
							{
								colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
								colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
								colb = colr / 4;
							}
							else
							{
								colr = 60;
								colg = 72;
								colb = 100;
							}
							
							if(colr > 127)
								colr = 127;
							if(colg > 127)
								colg = 127;
							if(colb > 127)
								colb = 127;

							col = RGB_MAKE(colr*2,colg*2,colb*2);

							//col = (colb << 16) + (colg << 8) + colr;

							s[0] = c + c0;

							ENGINE.DrawShadowedString(2, s, cx * PCMULTX, cy * PCMULTY, col);

							cx += 16;
						}
						cy += 13;
					}
					cy ++;

					// and del & enter
					if ((mSaveX == 0) && (mSaveY == 4))
					{
						colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
						colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
						colb = colr / 4;
					}
					else
					{
						colr = 60;
						colg = 72;
						colb = 100;
					}
					
					if(colr > 127)
						colr = 127;
					if(colg > 127)
						colg = 127;
					if(colb > 127)
						colb = 127;

					col = RGB_MAKE(colr*2,colg*2,colb*2);

					CText::GetString(TEXT_BACKSPACE, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 170 * PCMULTX - sl / 2, cy * PCMULTY, col);

					if ((mSaveX == 1) && (mSaveY == 4))
					{
						colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
						colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
						colb = colr / 4;
					}
					else
					{
						colr = 60;
						colg = 72;
						colb = 100;
					}
					
					if(colr > 127)
						colr = 127;
					if(colg > 127)
						colg = 127;
					if(colb > 127)
						colb = 127;

					col = RGB_MAKE(colr*2,colg*2,colb*2);

					CText::GetString(TEXT_ENTER, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 340 * PCMULTX - sl / 2, cy * PCMULTY , col);
#endif
				}
				break;

			case ESS_SCANNING:
				{
					CText::GetString(TEXT_LOAD_LOOKING_A, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE);//0x001f4f7f);

					ty = 120;

					CText::GetString(TEXT_LOAD_LOOKING_B, s);
					if (s[0] != '.')
					{
						sl = ENGINE.GetStringSize(2, s);
						ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, ty * PCMULTY , 0x003E9EfE);
						ty += 20;
					}

					CText::GetString(TEXT_LOAD_REMOVE, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, ty * PCMULTY , 0x003E9EfE);
				}
				break;

			case ESS_SAVING:
				{
				//	CText::GetString(TEXT_SAVE_SAVING, s);
				//	sl = ENGINE.GetStringSize(2, s);
				//	ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE);

				//	CText::GetString(TEXT_LOAD_REMOVE, s);
				//	sl = ENGINE.GetStringSize(2, s);
				//	ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY , 0x003E9EfE);

				}
				break;

			case ESS_SAVED:
				{
				//	CText::GetString(TEXT_SAVE_SAVED, s);
				//	sl = ENGINE.GetStringSize(2, s);
				//	ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY, 0x003E9EfE);
					/*
					CText::GetString(TEXT_HELP2, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ESS_FAILED:
				{
					CText::GetString(TEXT_SAVE_FAILED, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE);
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ESS_FORMAT_FAILED:
				{
					CText::GetString(TEXT_FORMAT_FAILED, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE);
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/	
				}
				break;

			case ESS_UNFORMATTED:
				{
					CText::GetString(TEXT_SAVE_UNFORMATTED, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE);

					CText::GetString(TEXT_SAVE_UNFORMATTED2_A, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY , 0x003E9EfE);

					ty = 140;

					CText::GetString(TEXT_SAVE_UNFORMATTED2_B, s);
					if (s[0] != '.')
					{
						sl = ENGINE.GetStringSize(2, s);
						ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 140 * PCMULTY , 0x003E9EfE);
						ty += 20;
					}

					// no/ yes
					DWORD col1, col2;

					SINT	colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
					SINT	colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
					SINT	colb = colr / 4;

					if (mSaveY == 0)
					{
						col1 = (colb << 16) + (colg << 8) + colr;
						col2 = 0x007f4f1f;
					}
					else
					{
						col1 = 0x007f4f1f;
						col2 = (colb << 16) + (colg << 8) + colr;
					}

					CText::GetString(TEXT_NO, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, ty * PCMULTY , col1);

					CText::GetString(TEXT_YES, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, (ty + 20) * PCMULTY , col2);
					/*
					CText::GetString(TEXT_HELP1, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/	
				}
				break;

			case ESS_FORMATTING:
				{
					CText::GetString(TEXT_SAVE_FORMATTING, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE);

					CText::GetString(TEXT_LOAD_REMOVE, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY , 0x003E9EfE);

				}
				break;

			case ESS_NO_SPACE:
				{
					CText::GetString(TEXT_SAVE_NO_SPACE, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE);

					CText::GetString(TEXT_LOAD_INSERT2_A, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY , 0x003E9EfE);

					CText::GetString(TEXT_LOAD_INSERT2_B, s);
					if (s[0] != '.')
					{
						sl = ENGINE.GetStringSize(2, s);
						ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 140 * PCMULTY , 0x003E9EfE);
					}
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ESS_NO_CARD:
				{
					CText::GetString(TEXT_LOAD_INSERT_A, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE);

					CText::GetString(TEXT_LOAD_INSERT_B, s);
					if (s[0] != '.')
					{
						sl = ENGINE.GetStringSize(2, s);
						ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY ,0x003E9EfE);
					}
					/*
					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
					*/
				}
				break;

			case ESS_NAME_TAKEN:
				{
					GINT	gy = G1;

					SINT	c0;
					SINT	y = 126;

					for (c0 = 0; c0 < 3; c0 ++)
					{
						SINT 	c1;
						SINT	x = 112;

						GINT	gx;
						/*
						//SINT	xoffs;

						BOOL	left;

						//left = mMode == FEPS_QSAVE2SAVE;
						left = false;

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

						//if (xoffs < 1000)
						//{
						
							x += xoffs;*/

							// draw text
							char	s[100];
							switch (c0)
							{
							case 0:	CText::GetString(TEXT_PC_OVERWRITE, s); break;
							case 1:	CText::GetString(TEXT_NO, s); break;
							case 2:	CText::GetString(TEXT_YES, s); break;
							};

							SINT	tcol;

							if ((mSaveConfirmOverWriteMenuPos == c0) || (c0 == 0))
								tcol = 0x00FFFFFF;
							else
								tcol = 0x00BEBEBE;

							SINT	sl = ENGINE.GetStringSize(0, s);
							ENGINE.DrawShadowedString(	0, 
														s, 
														PLATFORM.ScreenWidth() / 2 - sl / 2,// + xoffs, 
														(y + 5) * PCMULTY + MENUTEXTOFFSET,
														tcol,			// Foreground colour.
														0.05f);			// Z

							/*
							ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1, tcol, 0);
							ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5    , 0x001f0000, 0);
							*/

							// Draw Bars
							SINT	colr = 60, colg = 72, colb = 100;

							if (mSaveConfirmOverWriteMenuPos == c0)
							{
								colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
								colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
								colb = colr / 4;
							}
							else if (c0 == 0)
							{
								colr = 80;
								colg = 100;
								colb = 60;
							}

							if(colr > 127)
								colr = 127;
							if(colg > 127)
								colg = 127;
							if(colb > 127)
								colb = 127;

							// Left side of menu bar
							SINT col = RGB_MAKE(colr*2, colg*2, colb*2);
							DrawSpritePC(boxl, x  , y, .09f, 64, 64, col);
							x += 47;
							// Four middle sections
							for (c1 = 0; c1 < 4; c1 ++) // 4
							{
								DrawSpritePC(boxm, x  , y, .09f, 64, 64, col);
								x += 47;
							}
							// Right side of bar,
							DrawSpritePC(boxr, x  , y, .09f, 64, 64, col);
						//}
						y += 30;
					}
#if 0
					if (mMode == FEPS_QSAVE)
					{
						char text[200];

						CText::GetString(TEXT_HELP1, text);
						SINT	sl = ENGINE.GetStringSize(2, text);
						ENGINE.DrawShadowedString(2, text, 256 *PCMULTX - sl / 2, 220 *PCMULTY  + MENUTEXTOFFSET, 0x00FFFFFF);//7f7f7f);
					}

					CText::GetString(TEXT_NAME_ERROR1, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 100 * PCMULTY , 0x003E9EfE););

					CText::GetString(TEXT_NAME_ERROR2, s);
					sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(2, s, 256 * PCMULTX - sl / 2, 120 * PCMULTY , 0x003E9EfE););

					CText::GetString(TEXT_HELP3, s);
					sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(0, s, 256 * PCMULTX - sl / 2, 210 * PCMULTY , 0x007f7f7f);
#endif
				}
				break;
		};
	}

}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderMessage()
{
//	SINT	yps;
	GINT	gy = G1;

	SINT	c0;
	SINT	y = 126;

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();

	for (c0 = 0; c0 < 3; c0 ++)
	{
		SINT 	c1;
		SINT	x = 112;

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
					SINT	tcol  = 0x00FFFFFF;

					if (mMessage == 13)
						CText::GetString(TEXT_ALL_GOLD1, s);
					else
						CText::GetString(TEXT_CONGRATS1, s);

					SINT	sl = ENGINE.GetStringSize(2, s);
					ENGINE.DrawShadowedString(	2, 
												s, 
												PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs, 
												(y + 5) * PCMULTY + MENUTEXTOFFSET,
												tcol,			// Foreground colour.
												0.005f);			
					/*
					ENGINE.DrawShadowedString(2, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1, tcol, 0);
					ENGINE.DrawShadowedString(2, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5    , 0x001f0000, 0);
					*/
				}
				break;
			case 1:
				{
					SINT	tcol = 0x00B8FF;

					if (mMessage < 11)
						CText::GetString(TEXT_PRN_ECO + mMessage, s);
					else if (mMessage == 11)
						CText::GetString(TEXT_BONUS_LEVEL, s);
					else if (mMessage == 12)
						CText::GetString(TEXT_BONUS_CHARACTER, s);
					else if (mMessage == 13)
						CText::GetString(TEXT_ALL_GOLD2, s);

					SINT	sl = ENGINE.GetStringSize(2, s);

					ENGINE.DrawShadowedString(	2, 
												s, 
												PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs * PCMULTX, 
												(y + 5) * PCMULTY + MENUTEXTOFFSET,
												tcol,			// Foreground colour.
												0.005f);
					/*
					ENGINE.DrawShadowedString(2, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1, tcol, 0);
					ENGINE.DrawShadowedString(2, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5    , 0x001f0000, 0);
					*/

				}
				break;
			case 2:
				{
					CText::GetString(TEXT_CONTINUE, s);
					SINT	tcol = 0x00FFFFFF;

					SINT	sl = ENGINE.GetStringSize(0, s);
					ENGINE.DrawShadowedString(	0, 
												s, 
												PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs * PCMULTX, 
												(y + 5) * PCMULTY + MENUTEXTOFFSET,
												tcol,			// Foreground colour.
												0.005f);
					/*
					ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs - 1, y + 5 - 1, tcol, 0);
					ENGINE.DrawShadowedString(0, s, PLATFORM.ScreenWidth() / 2 - sl / 2 + xoffs    , y + 5    , 0x001f0000, 0);
					*/
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

			if(colr > 127)
				colr = 127;
			if(colg > 127)
				colg = 127;
			if(colb > 127)
				colb = 127;

			SINT lsc = (GAME.GetLanguage() == LANG_ENGLISH)?0:2;

			// Draw the button background.
			DWORD col = RGB_MAKE(colr*2,colg*2,colb*2);
			DrawSpritePC(boxl, x - 24 - 24 * lsc, y, .008f, 64, 64, col); // Left side of menu bar
			x += 48;
			for (c1 = 0; c1 < 5 + lsc; c1 ++) // Four middle sections
			{
				DrawSpritePC(boxm, x - 24 - 24 * lsc , y, .008f, 64, 64, col);
				x += 48;
			}
			DrawSpritePC(boxr, x - 24 - 24 * lsc , y, .008f, 64, 64, col); // Right side of bar.


//			boxl->DrawAll();
//			boxm->DrawAll();
//			boxr->DrawAll();

			/*
			// Left side of menu bar
			SINT col = RGB_MAKE(colr*2, colg*2, colb*2);
			DrawSpritePC(boxl, x , y, .09f, 64, 64, col);
			x += 47;
			// Four middle sections
			for (c1 = 0; c1 < 4; c1 ++) // 4
			{
				DrawSpritePC(boxm, x  , y, .09f, 64, 64, col);
				x += 47;
			}
			// Right side of bar,
			DrawSpritePC(boxr, x  , y, .09f, 64, 64, col);
			*/
		}
		y += 30;
	}
/*
	if (mMode == FEPS_MESSAGE)
	{
		char text[200];

		CText::GetString(TEXT_HELP2, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, 256 * PCMULTX - sl / 2, 220 * PCMULTY + MENUTEXTOFFSET, 0x00FFFFFF);
	}
	*/

	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderMChar()
{
	SINT	c		= (mMCharMenuSlide * 128) / SLIDE_TIME;
	GINT 	gy;

	
	gy = G(mMCharMenuSlide);
	gy = (gy * gy) / (SLIDE_TIME * SLIDE_TIME);
	gy = -G(15 + 75) + (gy * 75);

	// setup the camera
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
		// Which meshes to draw, and where
		SINT	character = mMCharacter[cn] + mMCharSlide[cn].Whole();

		if (character < 0) 
		{
			character %= 6;
			character += 6;
		}

		if (character > 5)
		{
			character %= 6;
		}

		GINT	gpos = -((mMCharSlide[cn] + G(100)).GFrac());   // won't work with negative numbers

		if (mMCharSlide[cn] != G0)
		{
			CFrontEndThing	*t = &mMCharThings[cn][character];
			t->mBlack = ((!can_select_doctor()) && (character == 5));

			if (((cn == 0) && (gpos <= G0)) ||
				((cn == 1) && (gpos >= G0)))
				t->mPos = GVector(gpos * G(12) + ((cn == 0) ? -G(2) : G(2)), G(8, 0), G(2, 0));
			else if (cn == 0)
				t->mPos = GVector(-G(2), G(8, 0), G(2, 0) + gpos * G(15));
			else
				t->mPos = GVector( G(2), G(8, 0), G(2, 0) - gpos * G(15));
				
			t->Render();
			character = (character + 1) % 6;
			gpos = gpos + G1;
		}

		CFrontEndThing	*t = &mMCharThings[cn][character];
		t->mBlack = ((!can_select_doctor()) && (character == 5));

		if (((cn == 0) && (gpos <= G0)) ||
			((cn == 1) && (gpos >= G0)))
			t->mPos = GVector(gpos * G(12) + ((cn == 0) ? -G(2) : G(2)), G(8, 0), G(2, 0));
		else if (cn == 0)
			t->mPos = GVector(-G(2), G(8, 0), G(2, 0) + gpos * G(15));
		else
			t->mPos = GVector( G(2), G(8, 0), G(2, 0) - gpos * G(15));

		t->Render();
	}
	
	if (mMode == FEPS_MCHAR)
	{
		// draw arrows?
		if ((mArrowCounter & 31) < 20)
		{
			if (!mMCharSelected[0])
			{
				DrawSpritePC(arrow_l, 187 + ARROWOFFSETX, 190 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
				DrawSpritePC(arrow_r, 229 + ARROWOFFSETX, 190 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
			}
			if (!mMCharSelected[1])
			{
				DrawSpritePC(arrow_u, 267 + ARROWOFFSETX, 191 + ARROWOFFSETY, .9f, 18, 14, 0x00ffffff);
				DrawSpritePC(arrow_d, 309 + ARROWOFFSETX, 191 + ARROWOFFSETY, .9f, 18, 14, 0x00ffffff);
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

	
	if(GAME.GetLanguage() != 1)
	{
		DrawBox(16 , 100, 150, h, .95f, col, col, col);
		DrawBox(346, 100, 150, h, .95f, col, col, col);
	}
	else
	{
		DrawBox(16 , 100, 145, h, .95f, col, col, col);
		DrawBox(346, 100, 145, h, .95f, col, col, col);
	}

	// and some assorted text
	char	text[100];
	SINT	sl;

	if (mMode == FEPS_MCHAR)
	{
		//************
		//** P1

		// name
		CText::GetString(TEXT_ANGEL + mMCharacter[0], text);
		sl = ENGINE.GetStringSize(0, text);
		ENGINE.DrawShadowedString(0, text, 93 * PCMULTX - sl / 2, 110 * PCMULTY, 0x00FEBE7E);//0x00FEBE7E);

		// stats
		CText::GetString(TEXT_STRENGTH, text);
		DrawDynamicText(2, text, 35 * PCMULTX, 140 * PCMULTY, 48, 92, 48, (mCounter * 2)    );
		CText::GetString(TEXT_SPEED, text);
		DrawDynamicText(2, text, 35 * PCMULTX, 156 * PCMULTY, 48, 92, 48, (mCounter * 2) - 3);
		CText::GetString(TEXT_AGILITY, text);
		DrawDynamicText(2, text, 35 * PCMULTX, 172 * PCMULTY, 48, 92, 48, (mCounter * 2) - 6);
		CText::GetString(TEXT_STAMINA, text);
		DrawDynamicText(2, text, 35 * PCMULTX, 188 * PCMULTY, 48, 92, 48, (mCounter * 2) - 9);

		sprintf(text, "````");
		text[get_char_stats(mMCharacter[0], 0)] = 0;
		DrawDynamicText(0, text, 130 * PCMULTX, 140 * PCMULTY, 128, 32, 32, (mCounter - mMCharSlideStart[0]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[0], 1)] = 0;
		DrawDynamicText(0, text, 130 * PCMULTX, 156 * PCMULTY, 128, 32, 32, (mCounter - mMCharSlideStart[0]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[0], 2)] = 0;
		DrawDynamicText(0, text, 130 * PCMULTX, 172 * PCMULTY, 128, 32, 32, (mCounter - mMCharSlideStart[0]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[0], 3)] = 0;
		DrawDynamicText(0, text, 130 * PCMULTX, 188 * PCMULTY, 128, 32, 32, (mCounter - mMCharSlideStart[0]));


		//************
		//** P2

		// name
		CText::GetString(TEXT_ANGEL + mMCharacter[1], text);
		sl = ENGINE.GetStringSize(0, text);
		ENGINE.DrawShadowedString(0, text, 421 * PCMULTX - sl / 2, 110 * PCMULTY, 0x00FEBE7E);

		// stats
		CText::GetString(TEXT_STRENGTH, text);
		DrawDynamicText(2, text, 365 * PCMULTX, 140 * PCMULTY, 48, 92, 48, (mCounter * 2)    );
		CText::GetString(TEXT_SPEED, text);
		DrawDynamicText(2, text, 365 * PCMULTX, 156 * PCMULTY, 48, 92, 48, (mCounter * 2) - 3);
		CText::GetString(TEXT_AGILITY, text);
		DrawDynamicText(2, text, 365 * PCMULTX, 172 * PCMULTY, 48, 92, 48, (mCounter * 2) - 6);
		CText::GetString(TEXT_STAMINA, text);
		DrawDynamicText(2, text, 365 * PCMULTX, 188 * PCMULTY, 48, 92, 48, (mCounter * 2) - 9);

		sprintf(text, "````");
		text[get_char_stats(mMCharacter[1], 0)] = 0;
		DrawDynamicText(0, text, 460 * PCMULTX, 140 * PCMULTY, 128, 32, 32, (mCounter - mMCharSlideStart[1]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[1], 1)] = 0;
		DrawDynamicText(0, text, 460 * PCMULTX, 156 * PCMULTY, 128, 32, 32, (mCounter - mMCharSlideStart[1]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[1], 2)] = 0;
		DrawDynamicText(0, text, 460 * PCMULTX, 172 * PCMULTY, 128, 32, 32, (mCounter - mMCharSlideStart[1]));
		sprintf(text, "````");
		text[get_char_stats(mMCharacter[1], 3)] = 0;
		DrawDynamicText(0, text, 460 * PCMULTX, 188 * PCMULTY, 128, 32, 32, (mCounter - mMCharSlideStart[1]));


		// Chosen?
		CText::GetString(TEXT_CHOSEN, text);
		sl = ENGINE.GetStringSize(2, text);
		if (mMCharSelected[0])
			ENGINE.DrawShadowedString(2, text, (256 - 45) * PCMULTX - sl / 2, 175 * PCMULTY, 0x00FEBE7E);
		if (mMCharSelected[1])
			ENGINE.DrawShadowedString(2, text, (256 + 45) * PCMULTX - sl / 2, 175 * PCMULTY, 0x00FEBE7E);
	}
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderMTourn()
{
	
	CTexture::DrawAllTextures();
	CTexture::DrawAllTexturesAdditive();

	// Which Tournaments to draw, and where...
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

			// Doesn't take screen scaling into account!!! SINT	tx = ((gpos * G(TOURN_WIDTH + TOURN_GAP)).Whole() + ((PLATFORM.ScreenWidth() / 2) - (TOURN_WIDTH / 2)));
			SINT	tx = (gpos * G(TOURN_WIDTH + TOURN_GAP)).Whole() + ((SINT)(PLATFORM.ScreenWidth() / PCMULTX - TOURN_WIDTH) >> 1);
			SINT	ty = 85 + ((G1 - gh) * 350).Whole();			
			SINT	txc = tx + TOURN_WIDTH / 2;

			// Draw the border
			
			DrawBox(tx, ty, TOURN_WIDTH, TOURN_HEIGHT, 0.95f, 40, 40, 40);
		
			// And the contents
			STournamentInfo		*ti = CLevelStructure::GetTournamentInfo(tournament);
			char	text[200];
			SINT	sl, c1;
			SINT	numl = ti->mNumLevels;

			// Name
			CText::GetString(ti->mName, text);
			sl = ENGINE.GetStringSize(0, text);
			ENGINE.DrawShadowedString(0, text, txc * PCMULTX - sl / 2, (ty + 10) * PCMULTY, 0x00FEBE7E);

			// the Arenas
			SINT	total_width = (42 * (numl - 1)) + (7 * (numl - 1));
			SINT	x, y, x2;//, y2;

			x = txc - (total_width / 2) - 24; //-21
			y = ty + 32; //35

			for (c1 = 0; c1 < numl; c1 ++)
			{
				DrawSpritePC(ar[ti->mLevels[c1].mType], x, y, 0.93f, 64, 64, 0x00FFFFFF);
				x += 49;
			}

			// draw some vertical lines
			x = txc - (total_width / 2) - 21 - 1;
			y = ty + 27;
			for (c1 = 0; c1 < numl + 1; c1 ++)
			{
				DrawLine(x, y, x, y + 94, 240, 128, 128, 32);
				x += 49;
			}
			DrawLine(x, ty + 68 - 5, x, y + 94, 240, 128, 128, 32);

			// draw some horizontal lines
			x = txc - (total_width / 2) - 21 - 1 - 70;
			x2 = x + total_width + 49 + 49 + 70 + 5;

			DrawLine(x + 65, ty + 32     , x2 - 49, ty + 32     , 240, 128, 128, 32);
			DrawLine(x     , ty + 68     , x2     , ty + 68     , 240, 128, 128, 32);
			DrawLine(x     , ty + 68 + 24, x2     , ty + 68 + 24, 240, 128, 128, 32);
			DrawLine(x     , ty + 68 + 48, x2     , ty + 68 + 48, 240, 128, 128, 32);

			DrawLine(x + 5, ty + 68 - 5, x + 5, ty + 68 + 48 + 5, 240, 128, 128, 32);
			
			CTexture::DrawAllTextures();
			CTexture::DrawAllTexturesAdditive();

			// more text
			CText::GetString(TEXT_1UP, text);
			sl = ENGINE.GetStringSize(0, text);
			ENGINE.DrawShadowedString(0, text, (x + 53) * PCMULTX - sl, (ty + 68 + 8) * PCMULTY     , 0x00BE7EFE);
			CText::GetString(TEXT_2UP, text);
			sl = ENGINE.GetStringSize(0, text);
			ENGINE.DrawShadowedString(0, text, (x + 53) * PCMULTX - sl, (ty + 68 + 8 + 24) * PCMULTY, 0x00BE7EFE);


			// Hokay - do we need to show some results?
			if (tournament == mMTournament)
			{
				x = txc - (total_width / 2) - 21 + 2 -5;

				for (c1 = 0; c1 < mMTournamentResultsShowLW; c1 ++)
				{
					SINT	res = MULTIPLAYER_CAREER.mWhoWonWhichLevel[c1];
					SINT	py = -1;
					switch (res)
					{
						case 0:
							py = ty + 68 - 4;
							break;
						case 1:
							py = ty + 68 + 24 - 4;
							break;
					};
					if (py != -1)
					{

						// NOT TESTED!!!
						DrawSpritePC(qualify, x, py, 0.93f, 70, 60, 0x00FFFFFF);

						if (mMTournamentResultsTimer == 0)
						{
							if (c1 == mMTournamentResultsShowLW - 1)
							{
								if (!mMTournamentResultsShowWinner)
								{
									// emit some particles!
									AddParticles(x + 30, py + 12, NUM_PSXFE_PARTICLES / 2);
								}
							}
						}
					}
					else
					{
						// draw!
						// NOT TESTED!!!
						DrawSpritePC(draw, x, (ty + 68      - 4), 0.93f, 70, 60, 0x00FFFFFF);
						DrawSpritePC(draw, x, (ty + 68 + 24 - 4), 0.93f, 70, 60, 0x00FFFFFF);
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
						// NOT TESTED!!!
						DrawSpritePC(qualify, x-5, ty + 68 - 4, 0.05f, 70, 60, 0x00FFFFFF);
						// emit some particles!
						if (mMTournamentResultsTimer == 0)
							AddParticles(x + 30, ty + 68 - 4 + 12, NUM_PSXFE_PARTICLES / 2);

						CText::GetString(TEXT_P1WINS, text);
						sl = ENGINE.GetStringSize(0, text);
						ENGINE.DrawShadowedString(0, text, txc * PCMULTX - sl / 2, (ty + 68 + 48 + 10) * PCMULTY, 0x00BEFF7E);//3f7f5f);
					}
					else if (sc1 < sc2)
					{
						// NOT TESTED!!!
						DrawSpritePC(qualify, x, ty + 68 + 24 - 4, 0.05f, 70, 60, 0x00FFFFFF);
						// emit some particles!
						if (mMTournamentResultsTimer == 0)
							AddParticles(x + 30, ty + 68 + 24 - 4 + 12, NUM_PSXFE_PARTICLES / 2);

						CText::GetString(TEXT_P2WINS, text);
						sl = ENGINE.GetStringSize(0, text);
						ENGINE.DrawShadowedString(0, text, txc * PCMULTX - sl / 2, (ty + 68 + 48 + 10) * PCMULTY, 0x00BEFF7E);// are: 0x005F7F3F //3f7f5f);
					}
					else
					{
						// draw
						// NOT TESTED!!!
						DrawSpritePC(draw, x-5, (ty + 68      - 4), 0.05f, 70, 60, 0x00FFFFFF);
						DrawSpritePC(draw, x-5, (ty + 68 + 24 - 4), 0.05f, 70, 60, 0x00FFFFFF);

						CText::GetString(TEXT_TOURNAMENT_DRAWN, text);
						sl = ENGINE.GetStringSize(0, text);
						ENGINE.DrawShadowedString(0, text, txc * PCMULTX - sl / 2, (ty + 68 + 48 + 10) * PCMULTY, 0x00BEFF7E);//3f7f5f);
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
				DrawSpritePC(arrow_l, 70 + ARROWOFFSETX, 100 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
				DrawSpritePC(arrow_l, 70 + ARROWOFFSETX, 210 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
			}
			if (mMTournament != CLevelStructure::GetNumMultiplayerTournaments() - 1)
			{
				DrawSpritePC(arrow_r, 432 + ARROWOFFSETX, 100 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
				DrawSpritePC(arrow_r, 432 + ARROWOFFSETX, 210 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
			}
		}
	}
}


//******************************************************************************************
void	CPCFEPrisonSelect::RenderOptions()
{
	GINT	gy = G1;
	SINT	c0;
	
	for (c0 = 1; c0 >= 0; c0 --)
	{
		//SINT	y = 100 + (c0 * 30);
		SINT	y = 120 + (c0 * 30); 

		SINT 	c1;
		SINT	x = 112;

		GINT	gx;
		SINT	xoffs;

		BOOL	left;

		left = (mMode == FEPS_OPTIONS2SAVE) || (mMode == FEPS_SAVE2OPTIONS) ||
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
				colb = colr >> 2;
				if(colr > 127)
					colr = 127;
				if(colg > 127)
					colg = 127;
			}
				
			if ((c0 == 3) && (!(CAREER.mStarted))) 
			{
				colr = colg = colb = 64;
			}

			// Draw the button background.
			DWORD col = RGB_MAKE(colr*2,colg*2,colb*2);
			DrawSpritePC(boxl, x  , y, .99f, 64, 64, col);	// Left side of menu bar
			x += 47;
			for (c1 = 0; c1 < 4; c1 ++)						// Four middle sections
			{
				DrawSpritePC(boxm, x  , y, .99f, 64, 64, col);
				x += 47;
			}
			DrawSpritePC(boxr, x  , y, .99f, 64, 64, col);	// Right side of bar.

	
			// draw text
			char	s[100];
			switch (c0)
			{
			case 0:	CText::GetString(TEXT_SOUND_VOLUME, s); break;
			case 1:	CText::GetString(TEXT_MUSIC_VOLUME, s); break;
//			case 2:	CText::GetString(TEXT_STEREO_SOUND, s); break;
//			case 3:	CText::GetString(TEXT_SAVE_GAME, s); break;
			};

			SINT	tcol;
			if (mOptionsMenuPos == c0)
				tcol = 0x00ffffff;
			else
				tcol = 0x00bfbfbf;

#if 0
			if (c0 == 3)
			{
				SINT font = 0;

				// Positioning
				SINT	sl		= ENGINE.GetStringSize(font, s);
				SINT	SW		= PLATFORM.ScreenWidth();
				SINT	xPos	= (SW >> 1) - (sl >> 1) + xoffs;

				ENGINE.DrawShadowedString(	0, 
											s, 
											xPos, 
											(y + 5) * PCMULTY + MENUTEXTOFFSET,
											tcol);			// Foreground colour.
											
			}
			else if ((c0 == 0) || (c0 == 1))
			{
#endif
			ENGINE.DrawShadowedString(	0, 
										s, 
										160 * PCMULTX + xoffs, 
										(y + 5) * PCMULTY + MENUTEXTOFFSET,
										tcol);			// Foreground colour.

			
			SINT	val;
	
			if (c0 == 0)
				val = CAREER.mSoundVolume;
			else
				val = CAREER.mMusicVolume;

			sprintf(s, "``````````");
			s[val] = 0;

			SINT	sl = ENGINE.GetStringSize(0, s);
			ENGINE.DrawShadowedString(	0, 
										s, 
										290 * PCMULTX + xoffs, 
										(y + 5) * PCMULTY + MENUTEXTOFFSET,
										0x07ff0f00);			// Foreground colour.

		
			sprintf(s, "``````````");
			s[10 - val] = 0;
			ENGINE.DrawShadowedString(	0, 
										s, 
										290 * PCMULTX + xoffs + sl, 
										(y + 5) * PCMULTY + MENUTEXTOFFSET,
										0x002f2f2f);

			
			// draw arrows?
			if (mOptionsMenuPos == c0)
			{
//				if ((mCounter & 31) < 20)
				{
					DrawSpritePC(arrow_l, 259 + xoffs  + ARROWOFFSETX +13, (y + 8) + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
					DrawSpritePC(arrow_r, 359 + xoffs  + ARROWOFFSETX -13, (y + 8) + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
				}
			}
#if 0
			}
			else if (c0 == 2)
			{
				ENGINE.DrawShadowedString(	2, 
											s, 
											160 * PCMULTX + xoffs, 
											(y + 5) * PCMULTY + MENUTEXTOFFSET,
											tcol);
				
				DWORD	col;

				if (c0 == 2)
				{
					CText::GetString(TEXT_OFF, s);
					col = (!CAREER.mStereo) ? 0x07ff0f00 : 0x003f3f3f;
				}
				ENGINE.DrawShadowedString(	2, 
											s, 
											290 * PCMULTX + xoffs, 
											(y + 5) * PCMULTY + MENUTEXTOFFSET,
											col);
												
				
				if (c0 == 2)
				{
					CText::GetString(TEXT_ON, s);
					col = CAREER.mStereo ? 0x003f7f3f : 0x003f3f3f;
				}
				ENGINE.DrawShadowedString(	2, 
											s, 
											(290 + 35) * PCMULTX + xoffs, 
											(y + 5) * PCMULTY + MENUTEXTOFFSET,
											col);

				
				// draw arrows?
				if (mOptionsMenuPos == c0)
				{
	//				if ((mCounter & 31) < 20)
					{
						DrawSpritePC(arrow_l, 270 + xoffs + ARROWOFFSETX, (y + 3) + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
						DrawSpritePC(arrow_r, 370 + xoffs + ARROWOFFSETX, (y + 3) + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
					}
				}
			}
#endif
		}
	}
/*
	if (mMode == FEPS_OPTIONS)
	{
		char text[200];

		CText::GetString(TEXT_HELP1, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, 256 * PCMULTX - sl / 2, 220 * PCMULTY  + MENUTEXTOFFSET, 0x00FFFFFF);
	}
	*/
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderCredits()
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

	while ((y < 256) && (tn < 29))
	{
		CCredits::GetCredit(tn, &type, &title, l[0], l[1], l[2], l[3], l[4]);

		// draw title
		CText::GetString(title, text);
		sl = ENGINE.GetStringSize(0, text);
		if (type == CRED_NORMAL)
			tcol = 0x00ffffff;
		else
			tcol = 0x00ffbf00;
		if (y > -50)
		{
			ENGINE.DrawShadowedString(0, text, 320 - sl / 2    , y * PCMULTY    , tcol, 0.995f);
		}

		y += LINE_SPACE;

		SINT	c0;

		for (c0 = 0; c0 < 5; c0 ++)
		{
			if (l[c0][0])
			{
				SINT 	font = 0;
				if (tn == 10)	font = 2;
				sl = ENGINE.GetStringSize(font, l[c0]);
				if (y > -50)
				{
					ENGINE.DrawShadowedString(font, l[c0], 320 - sl / 2    , (y) * PCMULTY    , 0x003fff3f, 0.995f);
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
			case 0:	return '[';
			case 1:	return '\\';
			case 2:	return ']';
			case 3:	return '^';
			case 4:	return '}';
			};
			break;
		case 1:
			switch (action)
			{
			case 0:	return '\\';
			case 1:	return '[';
			case 2:	return '^';
			case 3:	return ']';
			case 4:	return '}';
			};
			break;
		case 2:
			switch (action)
			{
			case 0:	return '}';
			case 1:	return '[';
			case 2:	return '^';
			case 3:	return ']';
			case 4:	return '\\';
			};
			break;
	};

	return '!';
}

void	CPCFEPrisonSelect::RenderController()
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

			
			// Draw the button background.
			DWORD col = RGB_MAKE(colr*2,colg*2,colb*2);
			DrawSpritePC(boxl, x  , y, .99f, 64, 64, col);	// Left side of menu bar
			x += 47;
			for (c1 = 0; c1 < 3; c1 ++)						// Three middle sections
			{
				DrawSpritePC(boxm, x  , y, .99f, 64, 64, col);
				x += 47;
			}
			DrawSpritePC(boxr, x  , y, .99f, 64, 64, col);	// Right side of bar.

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
				tcol = 0x007F5f00 * 2;//tcol = 0x00005f7f;
			else
				tcol = 0x005f5f5f * 2;

//			if ((c0 & 1) && (PLATFORM.GetPadData(1)->valid))
			//if (c0 & 1)
			//	tcol = 0x00bfbfbf;//0x004f4f4f;

			SINT	sl;

			SINT	xpos;
			if (c0 & 1)
				xpos = ((PLATFORM.ScreenWidth() * 3) / 4);
			else
				xpos = ((PLATFORM.ScreenWidth()    ) / 4);

			if (c0 & 2)
				xpos -= 7; // centre

			sl = ENGINE.GetStringSize(2, s);
			ENGINE.DrawShadowedString(	2, 
										s, 
										xpos - sl / 2 + xoffs,
										(y + 5) * PCMULTY + MENUTEXTOFFSET, 
										tcol);
			/*
			ENGINE.DrawShadowedString(2, s, xpos - sl / 2 + xoffs    , (y + 5) * PCMULTY    , 0x001f0000);
			ENGINE.DrawShadowedString(2, s, xpos - sl / 2 + xoffs - 1, (y + 5) * PCMULTY - 1, tcol);
			*/
			
			if (c0 == 2)
			{
				sprintf(s, "%d", mControllerSetting1 + 1);
				ENGINE.DrawShadowedString(	2, 
											s, 
											xpos + sl / 2 + 8 + xoffs,
											(y + 5) * PCMULTY + MENUTEXTOFFSET, 
											0x001f7f3f * 2);

				/*
				ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs    , (y + 5) * PCMULTY    , 0x001f0000);
				ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs - 1, (y + 5) * PCMULTY - 1, 0x001f7f3f);
				*/
				
			}
			if (c0 == 3)
			{
				sprintf(s, "%d", mControllerSetting2 + 1);
//				if (PLATFORM.GetPadData(1)->valid)
				{
					ENGINE.DrawShadowedString(	2, 
												s, 
												xpos + sl / 2 + 8 + xoffs,
												(y + 5) * PCMULTY + MENUTEXTOFFSET, 
												0x001f7f3f * 2);

					/*
					ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs    , (y + 5) * PCMULTY    , 0x001f0000);
					ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs - 1, (y + 5) * PCMULTY - 1, tcol);
					*/
				}
			//	else
				{
					ENGINE.DrawShadowedString(	2, 
												s, 
												xpos + sl / 2 + 8 + xoffs,
												(y + 5) * PCMULTY + MENUTEXTOFFSET, 
												0x001f7f3f * 2);
					/*
					ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs    , (y + 5) * PCMULTY    , 0x001f0000);
					ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs - 1, (y + 5) * PCMULTY - 1, 0x001f7f3f);
					*/
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

	DrawBox(32 , 136, 192, h, 0.99f, col, col, col);
	DrawBox(288, 136, 192, h, 0.99f, col, col, col);


	if (mMode == FEPS_CONTROLLER)
	{
		if ((mArrowCounter & 31) < 20)
		{
			DrawSpritePC(arrow_l, 60   + ARROWOFFSETX , 108 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
			DrawSpritePC(arrow_r, 186  + ARROWOFFSETX , 108 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);

//			if (!(PLATFORM.GetPadData(1)->valid))
			{
				DrawSpritePC(arrow_l, 316  + ARROWOFFSETX , 108 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
				DrawSpritePC(arrow_r, 442  + ARROWOFFSETX , 108 + ARROWOFFSETY, .9f, 14, 18, 0x00ffffff);
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

				ENGINE.DrawShadowedString(0, text, (c1 ? 336 : 80) * PCMULTX, (137 + (c2 * 15)) * PCMULTY, 0x007f7f7f*2);
			}
		}

		CText::GetString(TEXT_JUMP, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 137 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 137 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_ATTACK, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 152 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 152 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_BLOCK, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 167 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 167 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_THROW, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 182 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 182 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_BRAKE, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 197 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 197 * PCMULTY, COL_GOLD, mCounter * 2);
/*
		CText::GetString(TEXT_HELP1, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, 256 * PCMULTX - sl / 2, 220 * PCMULTY, 0x007f7f7f * 2);
*/
	}

/*
#ifdef JJS
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

			if ((c0 & 1) && (PLATFORM.GetPadData(1)->valid))
			{
				colr = colg = colb = 64;
			}

			DrawSprite(FEPST_BOXL, x    , y    , 2, colr, colg, colb, 2);
			x += 48;

			for (c1 = 0; c1 < 3; c1 ++)
			{
				DrawSprite(FEPST_BOXM, x    , y    , 2, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEPST_BOXR, x    , y    , 2, colr, colg, colb, 2);

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

			if ((c0 & 1) && (PLATFORM.GetPadData(1)->valid))
				tcol = 0x004f4f4f;

			SINT	sl;

			SINT	xpos;
			if (c0 & 1)
				xpos = ((PLATFORM.ScreenWidth() * 3) / 4);
			else
				xpos = ((PLATFORM.ScreenWidth()    ) / 4);

			if (c0 & 2)
				xpos -= 7; // centre

			sl = ENGINE.GetStringSize(2, s);
			ENGINE.DrawShadowedString(2, s, xpos - sl / 2 + xoffs - 1, y + 5 - 1, tcol);
			ENGINE.DrawShadowedString(2, s, xpos - sl / 2 + xoffs    , y + 5    , 0x001f0000);

			if (c0 == 2)
			{
				sprintf(s, "%d", mControllerSetting1 + 1);
				ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs - 1, y + 5 - 1, 0x001f7f3f);
				ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs    , y + 5    , 0x001f0000);
			}
			if (c0 == 3)
			{
				sprintf(s, "%d", mControllerSetting2 + 1);
				if (PLATFORM.GetPadData(1)->valid)
				{
					ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs - 1, y + 5 - 1, tcol);
					ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs    , y + 5    , 0x001f0000);
				}
				else
				{
					ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs - 1, y + 5 - 1, 0x001f7f3f);
					ENGINE.DrawShadowedString(2, s, xpos + sl / 2 + 8 + xoffs    , y + 5    , 0x001f0000);
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

	DrawBox(32 , 136, 192, h, 3, col, col, col);
	DrawBox(288, 136, 192, h, 3, col, col, col);


	if (mMode == FEPS_CONTROLLER)
	{
		if ((mArrowCounter & 31) < 20)
		{
			DrawSprite(FEPST_ARROW_L,  60, 108, 0, 127, 127, 127, 2);
			DrawSprite(FEPST_ARROW_R, 186, 108, 0, 127, 127, 127, 2);

			if (!(PLATFORM.GetPadData(1)->valid))
			{
				DrawSprite(FEPST_ARROW_L, 316, 108, 0, 127, 127, 127, 2);
				DrawSprite(FEPST_ARROW_R, 442, 108, 0, 127, 127, 127, 2);
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

				ENGINE.DrawShadowedString(0, text, c1 ? 336 : 80, 137 + (c2 * 15), 0x007f7f7f);
			}
		}

		CText::GetString(TEXT_JUMP, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 137 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 137 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_ATTACK, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 152 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 152 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_BLOCK, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 167 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 167 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_THROW, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 182 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 182 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_BRAKE, text);
		DrawDynamicText(2, text, 110 * PCMULTX, 197 * PCMULTY, COL_GOLD, mCounter * 2);
		DrawDynamicText(2, text, 366 * PCMULTX, 197 * PCMULTY, COL_GOLD, mCounter * 2);

		CText::GetString(TEXT_HELP1, text);
		SINT	sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawShadowedString(2, text, 256 - sl / 2, 220 , 0x007f7f7f);

	}
#endif
*/
}

//******************************************************************************************
void	CPCFEPrisonSelect::RenderParticles()
{
	SINT	c0;
	SINT	i[4];

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
				g = mParticles[c0].mVal / 1.5f;//2;
				b = mParticles[c0].mVal / 4;//5;
				break;
			case	1:
				r = mParticles[c0].mVal / 4; // 5
				g = mParticles[c0].mVal / 1.5f;// 2
				b = mParticles[c0].mVal;
				break;
			}

			r = r*2;
			g = g*2;
			b = b*2;
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;

			SINT col = 0xff + (r << 16) + (g << 8) + b;
			
			DrawSpritePCAdditive(	spark, 
							mParticles[c0].mX.Whole() - 8, 
							mParticles[c0].mY.Whole() - 8, 
							0.005f,
							16, 
							16,
							col);
		}
	}

//#ifdef JJS	 // Lightning
	// lightning?
	if (mLightningTime)
	{
#define	FELIGHTNING_SIZE			10
#define FELIGHTNING_WIDTH			4
#define	FELIGHTNING_NUM_STREAMS		2
#define	FELIGHTNING_OFFSET			16

		SINT c0, c1;//, c2;
		POINT a, b, c, d;


		// work out how many segments we need

		SINT	dx = mLightningEX - mLightningSX;
		SINT	dy = mLightningEY - mLightningSY;

		GINT	dist = GROOT(G((dx * dx) + (dy * dy)));

		SINT	num_segs = (dist / FELIGHTNING_SIZE).Whole();

		SINT	ox = dx / num_segs;
		SINT	oy = dy / num_segs;

//		SINT	perpx =  oy / (FELIGHTNING_SIZE / FELIGHTNING_WIDTH);
//		SINT	perpy = -ox / (FELIGHTNING_SIZE / FELIGHTNING_WIDTH);

//		SINT	perpx =   (G(dy * FELIGHTNING_WIDTH) / dist).Whole();
//		SINT	perpy = -((G(dx * FELIGHTNING_WIDTH) / dist).Whole());

		// take 3
		SINT	perpx = 0;
		SINT	perpy = 0;

		if (dy > 0)
			perpx = 2;
		else if (dy < 0)
			perpx = -2;

		if (dx > 0)
			perpy = -2;
		else if (dx < 0)
			perpy = 2;

	//	NTAG		*o = PLATFORM.GetOrderTable();
	//    POLY_F4 	*o4 = (POLY_F4*)PLATFORM.GetMapPolys();

		for (c1 = 0; c1 < FELIGHTNING_NUM_STREAMS; c1++)
		{
			SINT	cx = mLightningSX;
			SINT	cy = mLightningSY;

			for	(c0 = 0; c0 < num_segs; c0++)
			{
				a.x = (cx + perpx) * PCMULTX;
				a.y	= (cy + perpy) * PCMULTY;
				b.x = (cx - perpx) * PCMULTX;
				b.y	= (cy - perpy) * PCMULTY;

				// nev verts
				if (c0 != num_segs - 1)
				{
					cx = mLightningSX + (ox * (c0 + 1)) + ((rand() % FELIGHTNING_OFFSET) - (FELIGHTNING_OFFSET / 2));
					cy = mLightningSY + (oy * (c0 + 1)) + ((rand() % FELIGHTNING_OFFSET) - (FELIGHTNING_OFFSET / 2));
				}
				else
				{
					cx = mLightningEX;
					cy = mLightningEY;
				}

				c.x = (cx + perpx) * PCMULTX;
				c.y	= (cy + perpy) * PCMULTY;
				d.x = (cx - perpx) * PCMULTX;
				d.y	= (cy - perpy) * PCMULTY;

				i[0] = felightning->AddVertex(&D3DTLVERTEX(D3DVECTOR(a.x, a.y, 0.003f), 1, 0x99AABBFF, 0x00000000, 0.0f, 0.0f));
				i[1] = felightning->AddVertex(&D3DTLVERTEX(D3DVECTOR(b.x, b.y, 0.003f), 1, 0x99AABBFF, 0x00000000, 1.0f, 0.0f));
				i[2] = felightning->AddVertex(&D3DTLVERTEX(D3DVECTOR(c.x, c.y, 0.003f), 1, 0x99AABBFF, 0x00000000, 0.0f, 1.0f));
				i[3] = felightning->AddVertex(&D3DTLVERTEX(D3DVECTOR(d.x, d.y, 0.003f), 1, 0x99AABBFF, 0x00000000, 1.0f, 1.0f));
			
				felightning->AddTri(i[0], i[2], i[3]);
				felightning->AddTri(i[0], i[3], i[1]);
			}
		}

		// semi-trans mode
		//DR_MODE		*drm = (DR_MODE *)o4;

		//SetDrawMode(drm, 1, 1, getTPage(0, 1, 0, 0), NULL);
		//addPrim(&o[0], drm);

		//drm ++;

		//PLATFORM.DeclareUsedMiscPolyGT4s((POLY_GT4*)drm);
	}
//#endif
}

//******************************************************************************************
void	CPCFEPrisonSelect::Render()
{
	HRESULT result;

	PLATFORM.GetDPWin()->Clear(0x00000000); // ####!!!!
	result = PLATFORM.GetBackSurface()->GetDevice()->BeginScene();
	if (SUCCEEDED(result))
	{
		// Setup engine things
		ENGINE.SetupInitialRenderstates();
		ENGINE.GetDevice()->SetViewport(PLATFORM.GetDPWin()->DSGetViewport(0));
		ENGINE.SetCamera(&mCamera);
		
		//************************
		// Background
		
		if ((mMode != FEPS_PS2GAME) && (mMode != FEPS_START) && (mMode != FEPS_TS2GAME))
		{
			// draw title bar
			//ENGINE.DrawSpritePC(back, 0  , 0  , .999f, 257, 129, 0xff808080, PCMULTX, PCMULTY, 0);

/*			ENGINE.DrawSpritePC(back, 0  , 0  , .999f, 256, 128, 0xffFFFFFF, PCMULTX, PCMULTY, 0);
			ENGINE.DrawSpritePC(back, 256, 0  , .998f, 256, 128, 0xffFFFFFF, PCMULTX, PCMULTY, 1);
			ENGINE.DrawSpritePC(back, 0  , 128, .997f, 256, 128, 0xffFFFFFF, PCMULTX, PCMULTY, 2);
			ENGINE.DrawSpritePC(back, 256, 128, .996f, 256, 128, 0xffFFFFFF, PCMULTX, PCMULTY, 3);*/

			ENGINE.DrawSpritePC(mTexBack[0],     0,   0, .999f, 256, 256);
			ENGINE.DrawSpritePC(mTexBack[1],   256,   0, .999f, 256, 256);
			ENGINE.DrawSpritePC(mTexBack[2],   512,   0, .999f, 128, 256);
			ENGINE.DrawSpritePC(mTexBack[3],     0, 256, .999f, 256, 256);
			ENGINE.DrawSpritePC(mTexBack[4],   256, 256, .999f, 256, 256);
			ENGINE.DrawSpritePC(mTexBack[5],   512, 256, .999f, 128, 256);
		}
		
		
		
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
		case	FEPS_MAIN2CONFIRMQUIT:
		case	FEPS_CONFIRMQUIT2MAIN:
		case	FEPS_MAIN2SINGLE:
		case 	FEPS_SINGLE2MAIN:
		case 	FEPS_CHAR2MAIN:
		case	FEPS_OPTIONS2MAIN:
		case 	FEPS_MAIN2OPTIONS:
		case	FEPS_MAIN2MCHAR:
		case	FEPS_MCHAR2MAIN:
		case	FEPS_LOAD2MAIN:
		case 	FEPS_ABANDON2MAIN:
		case 	FEPS_CREDITS2MAIN:
		case	FEPS_MAIN2CREDITS:
			RenderMain();
			break;
		};
		
		//**********************************************************************************
		//** ConfirmQuit Menu view
		
		switch (mMode)
		{
		case	FEPS_MAIN2CONFIRMQUIT:
		case	FEPS_CONFIRMQUIT2MAIN:
		case	FEPS_CONFIRMQUIT:
			RenderConfirmQuit();
			break;
		};

		//**********************************************************************************
		//** Single Menu view
		
		switch (mMode)
		{
		case 	FEPS_MAIN2SINGLE:
		case 	FEPS_SINGLE2MAIN:
		case	FEPS_SINGLE:
		case	FEPS_SINGLE2CHAR:
		case	FEPS_SINGLE2LOAD:
		case	FEPS_SINGLE2ABANDON:
			
			RenderSingle();
			break;
		};
		
		//**********************************************************************************
		//** Abandon Menu view
		
		switch (mMode)
		{
		case	FEPS_SINGLE2ABANDON:
		case	FEPS_ABANDON:
		case	FEPS_ABANDON2MAIN:
		case	FEPS_ABANDON2CHAR:
			
			RenderAbandon();
			break;
		}
		
		//**********************************************************************************
		//** Load Menu view
		
		switch (mMode)
		{
		case 	FEPS_SINGLE2LOAD:
		case 	FEPS_LOAD:
		case	FEPS_LOAD2MAIN:
		case	FEPS_LOAD2CHAR:
			RenderLoad();
			break;
		};
		
		//**********************************************************************************
		//** Character Select view
		
		switch (mMode)
		{
		case 	FEPS_CHAR:
		case 	FEPS_CHAR2MAIN:
		case	FEPS_SINGLE2CHAR:
		case	FEPS_CHAR2PSNEW:
		case	FEPS_PSNEW2CHAR:
		case	FEPS_LOAD2CHAR:
		case	FEPS_ABANDON2CHAR:
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
		case	FEPS_QSAVE2SAVE:
			RenderQSave();
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
		case	FEPS_QSAVE2SAVE:
		case	FEPS_SAVE2ARENA:
		case	FEPS_ARENA2MESSAGE:
		case	FEPS_MESSAGE:
		case	FEPS_MESSAGE2ARENA:
			RenderArena();
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
		//** Save Menu view
		
		switch (mMode)
		{
		case 	FEPS_QSAVE2SAVE:
		case 	FEPS_SAVE:
		case	FEPS_SAVE2ARENA:
		case	FEPS_OPTIONS2SAVE:
		case	FEPS_SAVE2OPTIONS:
			RenderSave();
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
		case	FEPS_OPTIONS2SAVE:
		case 	FEPS_SAVE2OPTIONS:
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
	
		// are: Can see this failing when there are overlays!!!
		//boxl->DrawAll();
		//boxm->DrawAll();
		//boxr->DrawAll();


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
		
		
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();	

		// draw title bar
		if ((mMode != FEPS_START) &&
			(mMode != FEPS_START2MAIN) &&
			(mMode != FEPS_MAIN2START) &&
			(mMode != FEPS_PS2GAME) &&
			(mMode != FEPS_TS2GAME))
		{
			SINT	c0;
			for (c0 = 0; c0 < 5; c0 ++)
			{
				DrawSpritePC(title[c0], 102.4 * float(c0), TITLE_BAR_Y, .95f, 129, 128, 0xffffffff);
				title[c0]->DrawAll();
			}
//			DrawSpritePC(titlea, 0  , TITLE_BAR_Y, .95f, 256, 72, 0xFFFFFFFF);//0xff7f7f7f);
//			DrawSpritePC(titleb, 256, TITLE_BAR_Y, .94f, 256, 72, 0xFFFFFFFF);//0xff7f7f7f);
		}

		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();	


		//**********************************************************************************
		//** Draw Title Text
		
		char	text[200];
		text[0] = 0;
		SINT	font = 2;
		switch (mMode)
		{
		case	FEPS_MAIN:
		case	FEPS_CONFIRMQUIT:
		case	FEPS_CONFIRMQUIT2MAIN:
		case	FEPS_MAIN2CONFIRMQUIT:
		case	FEPS_SINGLE2MAIN:
		case	FEPS_CHAR2MAIN:
			//	case 	FEPS_START2MAIN:
		case	FEPS_OPTIONS2MAIN:
		case 	FEPS_MCHAR2MAIN:
		case	FEPS_ABANDON2MAIN:
		case	FEPS_CREDITS2MAIN:
			font = 0;
			CText::GetString(TEXT_TITLE, text);
			break;
			
		case	FEPS_MAIN2SINGLE:
		case	FEPS_SINGLE:
			CText::GetString(TEXT_SINGLE_PLAYER, text);
			break;
			
		case	FEPS_SINGLE2ABANDON:
		case 	FEPS_ABANDON:
			CText::GetString(TEXT_NEW_GAME, text);
			break;
			
		case	FEPS_SINGLE2LOAD:
		case	FEPS_LOAD:
			CText::GetString(TEXT_LOAD_GAME, text);
			break;
			
		case	FEPS_SINGLE2CHAR:
		case	FEPS_CHAR:
		case	FEPS_PSNEW2CHAR:
		case	FEPS_LOAD2CHAR:
		case	FEPS_ABANDON2CHAR:
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
		case	FEPS_SAVE2ARENA:
		case	FEPS_ARENA2MESSAGE:
		case	FEPS_MESSAGE:
		case 	FEPS_MESSAGE2ARENA:
			CText::GetString(TEXT_ARENA_SELECT, text);
			break;
			
		case	FEPS_ARENA2QSAVE:
		case	FEPS_QSAVE:
		case	FEPS_QSAVE2SAVE:
		case	FEPS_SAVE:
		case	FEPS_OPTIONS2SAVE:
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
		case	FEPS_SAVE2OPTIONS:
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
			SINT	sl = ENGINE.GetStringSize(font, text);
			ENGINE.DrawShadowedString(font, text, PLATFORM.ScreenWidth() / 2 - sl / 2, (TITLE_BAR_Y + 25 + 1) * PCMULTY, 0x007fbfff);
		}
		
		//*********************
		// Set STP Update
		// (for motion-blur)
		//! Is this any slower?  I don't know....
#ifdef JJS	// Motion blur, apparently.
		DR_STP	*stp = (DR_STP *)PLATFORM.GetMiscPolyFT4s();
		SetDrawStp(stp, 1);
		addPrim(&o[OT_LENGTH - 3], stp);
		stp ++;
		PLATFORM.DeclareUsedMiscPolyFT4s((POLY_FT4 *)stp);
#endif	

		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();	

		//**********************************************************************************
		//** Particles
		
		if ((mMode != FEPS_PS2GAME) && (mMode != FEPS_START) && (mMode != FEPS_TS2GAME))
			RenderParticles();

		CTexture::DrawAllTextures();
		spark->DrawAll();
		CTexture::DrawAllTexturesAdditive();	

		PLATFORM.GetBackSurface()->GetDevice()->EndScene();
		
		ENGINE.Flip();
		PLATFORM.GetDPWin()->GetDirectDraw7()->WaitForVerticalBlank(DDWAITVB_BLOCKEND,NULL);
	}
	else
	{
		PLATFORM.GetDPWin()->RestoreD3D();
	}
}

//******************************************************************************************
SINT	CPCFEPrisonSelect::Run(EPSEntryType entrytype)
{
	SINT c0;
	
	// set engine into single player
	ENGINE.mNumDisplays = 1;

	SINT n = 0;
	
#ifdef	JCLDCNOPSX
	if ball breakers n += 2
#endif

	if (CAREER.AllGold())
		n += 1;
	
	GAME.ShowLoadingScreen(n);

	// Load stuff
	if (!LoadData())
		return 0; // oops

	SOUND.DeclareVolumeChange();  // update volumes
	SOUND.PlayTrack(2, 2);

	// misc engine setup
	mCamera.Reset(0);

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

	// Single Menu init
	mSingleMenuPos = 0;
	mSingleMenuSlide = 0;

	// Abandon Menu init
	mAbandonMenuPos = 0;
	mAbandonMenuSlide = 0;

	// Load Menu init
	mLoadMenuPos = 0;
	mLoadMenuScrollPos = 0;
	mLoadMenuSlide = 0;
	mLoadMenuState = ELS_START;

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
	
	// Save Menu init
	mNeedToUpdateSave = false;
	mSaveMenuPos = 0;
	mSaveMenuSlide = 0;
	mSaveMenuState = ESS_ENTER_NAME;
	mSaveFromArena = FALSE;

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
			mNeedToUpdateSave = true;

			// bonus prison
			if (mPrison == 10)
			{
				mPrison = 0;
				mPrisonBonus = TRUE;
				mPrisonBonusSlide = G1;
			}
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
			mNeedToUpdateSave = true;

			// pull some data out of the career file.
			mCharacter = CAREER.mCurrentCharacter;
			mPrison = CAREER.mCurrentPrison;
			mArena = CAREER.mCurrentArena;
			mArenaPendingVictoryType = EvaluateSingleLevelPerformance();
			if (mArenaPendingVictoryType)
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


	//!!!!!!!***********  CHECK GAME DATA (eg water) *******************
	GAME.DeclareSinglePlayer();		// for the engine


	// and run!
	while (mQuit == EPSXFE_NONE)
	{
		float time = ENGINE.GetSysTimeFloat();

//		CONTROLS.Update();// Now processed as part of the Process method.
		SOUND.UpdateStatus();

		if (CONTROLS.IsButtonPressedDB(BUTTON_SCREENSHOT))
			GAME.TakeScreenShot();

		Process();
		Render();

		while (ENGINE.GetSysTimeFloat() < time + (0.02f)) {};
	}

	// 'Bug' test
//	if (mQuit == EPSXFE_ATTRACT)
//	{
//		MessageBox(PLATFORM.GetDPWin()->GetHwnd(), "Attract mode activated", "Warning", MB_OK);
//	}

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

				FMVResChange(true);
				// Play FMV - note: trashes memory...
				PlayFMV(EMovie(FMV_ANGEL + mCharacter));
				FMVResChange(false);
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
				FMVResChange(true);
				PlayFMV(FMV_ALPHASTAR_IN);
				FMVResChange(false);
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
	case EPSXFE_EXIT:
		{
			return -999;  // let me out of here!
		}
	default:
		return -1;
	};
}

#endif
