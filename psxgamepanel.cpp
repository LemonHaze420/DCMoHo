#include	"Common.h"

#if TARGET == PSX

#include	"Globals.h"
#include	"PSXGamePanel.h"
#include	"Text.h"
#include 	"PSXBuildType.h"
#include	"Career.h"

CPSXGamePanel	GAMEPANEL;


#if DEMO_GAME == 3 || DEMO_GAME == 4
#define	GAMEPANEL_ON_TIMEOUT	750
#define GAME_TIMEOUT			15000

//#define DEMO_TIMEOUT	(mCounter > (GAME_TIMEOUT))
#define DEMO_TIMEOUT 	(FALSE)

#else
#define DEMO_TIMEOUT 	(FALSE)
#endif

#define	SOUNDCANCEL	SOUND.PlaySample(SOUND_CANCEL, G(0, 55000))
#define	SOUNDSELECT	SOUND.PlaySample(SOUND_SELECT, G(0, 55000))
#define	SOUNDMOVE	SOUND.PlaySample(SOUND_MOVE, G(0, 55000))


#define	GP_WIDTH	500

#define	PANEL_Y	75
//******************************************************************************************
BOOL	CPSXGamePanel::Init()
{
	mState = GPS_OFF;
	mPosX = 	-G(GP_WIDTH);
	mSelected = 0;
	mCounter = 0;
	mCounterOn = 0;
	mDBx = FALSE;
	mDBy = FALSE;

	return TRUE;
}

//******************************************************************************************
void	CPSXGamePanel::Process()
{
	static SINT	c1_valid = 0;
	static SINT	c2_valid = 0;

	if (CONTROLS.mControllers[0].GetPlaybackState() != ACTIVE)
	{
		struct  SPadData *pad_data1 = PLATFORM.GetPadData(0);
		struct  SPadData *pad_data2 = PLATFORM.GetPadData(1);

		if (pad_data1->valid)
			c1_valid ++;
		else
			c1_valid = 0;
		
		if ((GAME.IsMultiPlayer())&&(pad_data2->valid))
			c2_valid ++;
		else
			c2_valid = 0;
			
		
		if ((c1_valid > 5) || (c2_valid > 5))
		{
			if ((mState != GPS_COMING_ON) && (mState != GPS_ON))
				GAME.ToggleGamePanel();
		}
	}

	mCounter ++;

#ifdef AGEING
	if (mState != GPS_OFF)
	{
		EGQuitType	qt = GQ_FRONTEND;
		GAME.SetQuitFlag(qt);
		GAME.ToggleGamePanel();
		mState = GPS_OFF;
	}
#elif (DEMO_GAME == 1) || (DEMO_GAME == 2)
	if (mState != GPS_OFF)
	{
		EGQuitType	qt = GQ_FRONTEND;
		GAME.SetQuitFlag(qt);
		GAME.ToggleGamePanel();
		mState = GPS_OFF;
	}
#else
	// window position
	switch(mState)
	{
	case	GPS_COMING_ON:
		{
			GINT	dest = G0;
			mPosX += (dest - mPosX) * G(0, 20000);

			if (GABS(mPosX - dest) < G1)
			{
				mPosX = dest;
				mState = GPS_ON;
			}
		}
		break;

	case	GPS_GOING_OFF:
		{
			GINT	dest = -G(GP_WIDTH);
			mPosX += (dest - mPosX) * G(0, 20000);

			if (GABS(mPosX - dest) < G1)
			{
				mPosX = dest;
				mState = GPS_OFF;
			}
		}
		break;

	case	GPS_ON:
	case 	GPS_OFF:
	default:
		break;
	};

	if ((mState == GPS_OFF) || (mState == GPS_GOING_OFF))
		return;

	mCounterOn ++;

	// cursor
	GINT xm = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP);
	if (GAME.IsMultiPlayer())
		xm += CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP, 1);

	if (xm != G0)
	{
		if (!mDBy)
		{
			SOUNDMOVE;
			
			mCounterOn = 0;
			mDBy = TRUE;

			SINT 	dir;
			if (xm < G0)
				dir = 1;
			else
				dir = -1;

			mSelected += dir;

			if (mSelected < 0)
				mSelected = 4;
			else if (mSelected > 4)
				mSelected = 0;

			// nice and hacky....
			if (mType == GPT_LOST)
			{
				if (mSelected == 0)
				{
					if (dir == 1)
					{
						if (DEMO_TIMEOUT)
							mSelected = 2;
						else
							mSelected = 1;
					}
					else
						mSelected = 4;
				}
			}
			if ((mType == GPT_INGAME2) || (DEMO_TIMEOUT))
			{
				if (mSelected == 1)
				{
					if (dir == 1)
						mSelected = 2;
					else
					{
						if (mType == GPT_LOST)
							mSelected = 4;
						else
							mSelected = 0;
					}
				}
			}
		}
	}
	else
		mDBy = FALSE;

	// demo check
	if 	((DEMO_TIMEOUT) && (mSelected == 1))
		mSelected = 2;

	xm = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT);
	if (GAME.IsMultiPlayer())
		xm += CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, 1);

	if (xm != G0)
	{
		if (!mDBx)
		{
			mCounterOn = 0;
			mDBx = TRUE;

			if ((mSelected == 3) || (mSelected == 4))
			{
				SINT 	dir;
				if (xm < G0)
					dir = -1;
				else
					dir = 1;

				if (mSelected == 3)
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
				SOUND.PlaySample(SOUND_PICKUP3);
				
				SOUNDMOVE;
			}
		}
	}
	else
		mDBx = FALSE;

	// exit?
	if (!mFirstFrame)
	{
		if ((mType == GPT_INGAME) || (mType == GPT_INGAME2))
			if (CONTROLS.IsButtonPressedDB(BUTTON_GAME_PANEL))
			{
				GAME.ToggleGamePanel();
				SOUNDSELECT;
			}

		if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_X) ||
		    (GAME.IsMultiPlayer() && (CONTROLS.IsButtonPressedDB(BUTTON_PSX_X, 1))))
		{
			mCounterOn = 0;
			switch(mSelected)
			{
				case 0:
					GAME.ToggleGamePanel();
					SOUNDSELECT;
					break;
				case 1:
					GAME.SetQuitFlag(GQ_RESTART);
					GAME.ToggleGamePanel();
					mState = GPS_OFF; // otherwise the panel slides off at the start of the new level!
					mPosX = 	-G(GP_WIDTH);
					SOUNDSELECT;
					break;
				case 2:
					GAME.SetQuitFlag(GQ_FRONTEND);
					GAME.ToggleGamePanel();
					mState = GPS_OFF; // otherwise the panel slides off at the start of the new level!
					mPosX = 	-G(GP_WIDTH);
					SOUNDSELECT;
					break;
			}
		}
		else if (((CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE)) ||
	  		      (GAME.IsMultiPlayer() && (CONTROLS.IsButtonPressedDB(BUTTON_PSX_TRIANGLE, 1))))
		         && (mType != GPT_LOST))
		    {
				GAME.ToggleGamePanel();
				SOUNDCANCEL;
			}

#if	DEMO_GAME == 3 || DEMO_GAME == 4
		if (mCounterOn > GAMEPANEL_ON_TIMEOUT)
		{
			GAME.SetQuitFlag(GQ_FRONTEND);
			GAME.ToggleGamePanel();
			mState = GPS_OFF; // otherwise the panel slides off at the start of the new level!
			mPosX = -G(GP_WIDTH);
		}
#endif

	}
	else
		mFirstFrame = FALSE;
#endif
}


#define GP_P1CONT_X		20
#define GP_P1CONT_Y		100
#define GP_P1CONT_W		300
#define GP_P1CONT_H		20
#define GP_P2CONT_X		20
#define GP_P2CONT_Y		130
#define GP_P2CONT_W		300
#define GP_P2CONT_H		20

//******************************************************************************************
void	CPSXGamePanel::Render()
{


	struct  SPadData *pad_data1 = PLATFORM.GetPadData(0);
	struct  SPadData *pad_data2 = PLATFORM.GetPadData(1);
	POLY_G4	*p4;

	char	text[100];
	SINT	sl;


	NTAG	*o = PLATFORM.GetOrderTable();

	if(GAME.IsMultiPlayer())
	{
		// Draw the separating bar down midle of screen...
		POLY_F4*f4=(POLY_F4*)PLATFORM.GetMiscPolyG4s();
		setPolyF4(f4);
		f4->x0=255;
		f4->y0=0;
		f4->x1=257;
		f4->y1=0;
		f4->x2=255;
		f4->y2=256;
		f4->x3=257;
		f4->y3=256;
		f4->r0=f4->g0=f4->b0=0x00;
		addPrim(&o[15],f4);
		f4++;
		PLATFORM.DeclareUsedMiscPolyG4s((POLY_G4*)f4);
	}


	if (mState == GPS_OFF)
		return;


	SINT	px = (mPosX + GINT_HALF).Whole();  // stop end pop

	if (CONTROLS.mControllers[0].GetPlaybackState() != ACTIVE)
	{
		if((pad_data1->valid))
		{
			CText::GetString(TEXT_INSERT1, text);
			sl = ENGINE.GetStringSize(2, text);

			if((mCounter & 15) < 11)
			{
				ENGINE.DrawString(2, text, PLATFORM.ScreenWidthFull() / 2 - sl / 2 + px    , 15 + 4, 0xff00a0a0, 0);
				ENGINE.DrawString(2, text, PLATFORM.ScreenWidthFull() / 2 - sl / 2 + px + 1, 15 + 5, 0, 0);
			}

			ENGINE.DrawBar(PLATFORM.ScreenWidthFull() / 2 + px, 15, 6, 60, 127, 15, TRUE, 0);
		}
		if((GAME.IsMultiPlayer())&&(pad_data2->valid))
		{
			CText::GetString(TEXT_INSERT2, text);
			sl = ENGINE.GetStringSize(2, text);

			if((mCounter & 15) < 11)
			{
				ENGINE.DrawString(2, text, PLATFORM.ScreenWidthFull() / 2 - sl / 2 + px    , 45 + 4, 0xff00a0a0, 0);
				ENGINE.DrawString(2, text, PLATFORM.ScreenWidthFull() / 2 - sl / 2 + px + 1, 45 + 5, 0, 0);
			}

			ENGINE.DrawBar(PLATFORM.ScreenWidthFull() / 2 + px, 45, 6, 60, 127, 15, TRUE, 0);
		}
	}

	SINT	c0, py = PANEL_Y;

	for (c0 = 0; c0 < 5; c0 ++)
	{
		char	text[100];
		SINT 	tn = -1;

		switch (c0)
		{
		case 0:	tn = TEXT_CONTINUE; 			break;

		case 1:
#if DEMO_GAME == 3 || DEMO_GAME == 4
			if (DEMO_TIMEOUT)
				tn = TEXT_DEMO_TIMEOUT;
			else
				tn = TEXT_RESTART_ARENA;
			break;
#else
			tn = TEXT_RESTART_ARENA; 			break;
#endif

//#if DEMO_GAME == 3
//		case 2: tn = TEXT_QUIT_DEMO;			break;
//#else
		case 2:	tn = TEXT_EXIT_ARENA; 			break;
//#endif
		case 3:	tn = TEXT_SOUND_VOLUME; 		break;
		case 4:	tn = TEXT_MUSIC_VOLUME; 		break;
		}

		CText::GetString(tn, text);
		SINT	sl = ENGINE.GetStringSize(0, text);

		SINT	tcol;
		if (mSelected == c0)
			tcol = 0x007f7f7f;
		else
			tcol = 0x005f5f5f;

		if (c0 < 3)
		{
			ENGINE.DrawString(0, text, (PLATFORM.ScreenWidthFull() / 2) - (sl / 2) + px    , py + 4, tcol, 0);
			ENGINE.DrawString(0, text, (PLATFORM.ScreenWidthFull() / 2) - (sl / 2) + px + 1, py + 5, 0, 0);
		}
		else
		{
			ENGINE.DrawString(2, text, 160 + px    , py + 4, tcol, 0);
			ENGINE.DrawString(2, text, 160 + px + 1, py + 5, 0, 0);

			SINT	val;
			if (c0 == 3)
				val = CAREER.mSoundVolume;
			else
				val = CAREER.mMusicVolume;

			sprintf(text, "``````````");
			text[val] = 0;

			SINT	sl = ENGINE.GetStringSize(0, text);
			ENGINE.DrawString(0, text, 290 + px    , py + 4, 0x000f0f7f, 0);
			ENGINE.DrawString(0, text, 290 + px + 1, py + 5, 0, 0);

			sprintf(text, "``````````");
			text[10 - val] = 0;
			ENGINE.DrawString(0, text, 290 + px + sl    , py + 4, 0x002f2f2f, 0);
			ENGINE.DrawString(0, text, 290 + px + sl + 1, py + 5, 0, 0);

			// draw arrows?
			if (mSelected == c0)
			{
//				if ((mCounter & 31) < 20)
				{
					ENGINE.DrawSprite(TN_ARROW_L, 270 + px, py + 3, 0, 127, 127, 127, 2);
					ENGINE.DrawSprite(TN_ARROW_R, 370 + px, py + 3, 0, 127, 127, 127, 2);
				}
			}
		}

		SINT	colr = 60, colg = 72, colb = 100;

		if (mSelected == c0)
		{
			colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
			colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
			colb = colr / 4;
		}
		if ((c0 == 0) && (mType == GPT_LOST))
		{
			colr = colg = colb = 48;
		}
		if ((c0 == 1) && ((mType == GPT_INGAME2) || (DEMO_TIMEOUT)))
		{
			colr = colg = colb = 48;
		}
		ENGINE.DrawBar(PLATFORM.ScreenWidthFull() / 2 + px, py, 4, colr, colg, colb, TRUE, 0);

		py += 30;
	}

	if (px == 0)
	{
		if (mType != GPT_LOST)
			CText::GetString(TEXT_HELP1, text);
		else
			CText::GetString(TEXT_HELP5, text);

		sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawString(2, text, PLATFORM.ScreenWidthFull() / 2 - sl / 2    , py    , 0x007f7f7f, 0);
		ENGINE.DrawString(2, text, PLATFORM.ScreenWidthFull() / 2 - sl / 2 + 1, py + 1, 0, 0);
	}
}

//******************************************************************************************
void	CPSXGamePanel::Activate(EGPType type)
{
	mDBy = FALSE;
	mState = GPS_COMING_ON;
	if (type != GPT_LOST)
		mSelected = 0;
	else
		mSelected = 1;
	mType = type;
	mFirstFrame = TRUE;

	mCounterOn = 0;

	mDBy = TRUE;  // stops Darran getting annoyed

	SOUND_SELECT; // well, might as well go here...

	StopVibration();                // Make sure actuators stop...
	
#ifdef RECORD_GAME
	// DEMO Recording
	if (CONTROLS.mControllers[0].GetRecordState() == ACTIVE)
	{
		CONTROLS.mControllers[0].StopRecording();
	}
#endif

	if (CONTROLS.mControllers[0].GetPlaybackState() == ACTIVE)
		CONTROLS.mControllers[0].StopPlayBack();

}

//******************************************************************************************
void	CPSXGamePanel::DeActivate()
{
	mState = GPS_GOING_OFF;
}

//******************************************************************************************
void	CPSXGamePanel::DeActivateNow()
{
	mState = GPS_OFF;
	mPosX = 	-G(GP_WIDTH);
}

#endif
