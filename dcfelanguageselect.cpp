#include	"Common.h"

#if TARGET == DC

#include	"DCFELanguageSelect.h"
#include	"DCFEPrisonSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
#include	"Globals.h"
#include	"DCBuildType.h"
#include 	"Maincode.h"

CDCFELanguageSelect	LANGUAGESELECT;

#define	TITLE_BAR_Y	10

#define	SOUNDSELECT	SOUND.PlaySample(SOUND_SELECT)
#define	SOUNDMOVE	SOUND.PlaySample(SOUND_MOVE)

#define	CONTROLLER_SELECT	((CONTROLS.IsButtonPressedDB(BUTTON_DC_A) || CONTROLS.IsButtonPressedDB(BUTTON_DC_C) || CONTROLS.IsButtonPressedDB(BUTTON_DC_START)))

#define SLIDE_TIME	20

//******************************************************************************************
BOOL	CDCFELanguageSelect::LoadData()
{
	SOUND.AllocSampleBank("MOHOFE");

	UINT	CID_FEPS	= MKID("FEPS");
	UINT	CID_TPAG	= MKID("TPAG");
	UINT	CID_MESH	= MKID("MESH");

	CMEMBUFFER	mem;

	PLATFORM.ClearPools(); // free up global pools

	char	name[256];
	sprintf(name, "FRONTEND\\PRISEL0.DAT");

	//TRACE("************************\n");
	//TRACE("About to load front end data\n");

	if (!(mem.InitFromFile(name)))
	{
		TRACE("File Load failed\n");

		mem.Close();
		return FALSE;
	}

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

	PLATFORM.InitFrames(); // restore engine state

//	printf("Level Pool - %d / %d  used\n", POOL->mLevelPoolSize, LEVEL_POOL_SIZE);

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
void	CDCFELanguageSelect::Process()
{
	if (mQuit != 0)
		mQuit ++;

	GINT xm = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT);
	GINT ym = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP);

	switch (mState)
	{
		case FESS_LANGUAGE:
		{
			if (xm != G0)
			{
				if (!mDBX)
				{
					if (xm > G0)
					{
						switch(mLanguage)
						{
						case 0:	mLanguage = 1;	break;
						case 1:	mLanguage = 2;	break;
						};
					}
					else
					{
						switch(mLanguage)
						{
						case 1: mLanguage = 0; break;
						case 2: mLanguage = 1; break;
						};
					}
					mCounter = 0;
					SOUNDMOVE;
		
					mDBX = 6;
				}
				else
					mDBX --;
					
			}
			else
				mDBX = FALSE;
		
			if (CONTROLLER_SELECT)
			{
				SOUNDSELECT;
				if (SDSCanDo60hz())
				{
					CText::Load(mLanguage);
					mCounter = 0;
					mAskPos = 0;
					mAskMenuSlide = 0;
					mState = FESS_ASKON;
				}
				else
				{
					mQuit = 1;
					break;
				}
			}
		
		
			// Process Flag Things
			SINT	c0;
			for (c0 = 0; c0 < 5; c0 ++)
			{
				mFlags[c0].Process();
			}
			
			// timeout?
			if (mCounter == 50 * 30)
			{
				if (SDSCanDo60hz())
				{
					mLanguage = 0;
					CText::Load(mLanguage);
					mCounter = 0;
					mAskPos = 0;
					mAskMenuSlide = 0;
					mState = FESS_ASKON;
				}
				else
				{
					mQuit = 1;
					break;
				}
			}
		}
		break;
		
		case FESS_ASKON:
		{
			mAskMenuSlide ++;
			if (mAskMenuSlide == SLIDE_TIME)
				mState = FESS_ASK;
		}
		break;
		
		case FESS_ASK:
		{
			if (ym != G0)
			{
				if (!mDBY)
				{
					if (ym > G0)
					{
						mAskPos --;
					}
					else
					{
						mAskPos ++;
					}
					mCounter = 0;
					SOUNDMOVE;
		
					mDBY = 6;
				}
				else
					mDBY --;
					
				mCounter = 0;
			}
			else
				mDBY = FALSE;
				
			if (mAskPos == -1)
				mAskPos = 2;

			if (mAskPos == 3)
				mAskPos = 0;
		
			if (CONTROLLER_SELECT)
			{
				SOUNDSELECT;
				
				switch (mAskPos)
				{
				case 0:
					// in 50 already
					mQuit = 1;
					break;
					
				case 1:
					// switch to 60
					SDSSwitchTo60hz();
					mQuit = 1;
					break;
					
				case 2:
					mCounter = 0;
					mState = FESS_TEST1;
					break;				
				}
			}
		
			// timeout?
			if (mCounter == 50 * 30)
			{
				mQuit = 1;
			}
		}
		break;
	case FESS_TEST1:
		{
			// do nothing for 4 seconds!
			if (mCounter == 50 * 4)
			{
				mCounter = 0;
				mState = FESS_TEST2;
				SDSSwitchTo60hz();
			}
		};
		break;		
		
	case FESS_TEST2:
		{
			// do nothing for 5 seconds!
			if (mCounter == 60 * 5 - 1) // 60 hz mode!!
			{
				mCounter = 0;
				mState = FESS_ASK2;
				SDSSwitchTo50hz();
				mAsk2Pos = 0;
			}
		};
		break;		
		
	case FESS_ASK2:
		{
			if (ym != G0)
			{
				if (!mDBY)
				{
					if (ym > G0)
					{
						mAsk2Pos --;
					}
					else
					{
						mAsk2Pos ++;
					}
					mCounter = 0;
					SOUNDMOVE;
		
					mDBY = 6;
				}
				else
					mDBY --;
					
				mCounter = 0;
			}
			else
				mDBY = FALSE;
				
			if (mAsk2Pos == -1)
				mAsk2Pos = 1;

			if (mAsk2Pos == 2)
				mAsk2Pos = 0;
		
			if (CONTROLLER_SELECT)
			{
				SOUNDSELECT;
				
				switch (mAsk2Pos)
				{
				case 0:
					// switch to 60
					SDSSwitchTo60hz();
					mQuit = 1;
					break;
					
				case 1:
					// in 50 already
					mQuit = 1;
					break;
				}
			}
		
			// timeout?
			if (mCounter == 50 * 30)
			{
				mQuit = 1;
			}
		}
		break;

	};

	mCounter ++;
}

//******************************************************************************************
void	CDCFELanguageSelect::Render()
{
	PRISONSELECT.ResetLastDrawnString();
	PLATFORM.GeometryMarkBeginScene();

	char	text[200];
	char	s[200];

	if (mQuit == 0)
	{
		//************************
		// Background
	
		PRISONSELECT.DrawSprite(FEDCPST_BACK1, 0    , 0     , OT_LENGTH - 3, 80, 80, 80);
		PRISONSELECT.DrawSprite(FEDCPST_BACK2, 204.8, 0     , OT_LENGTH - 3, 80, 80, 80);
		PRISONSELECT.DrawSprite(FEDCPST_BACK3, 409.6, 0     , OT_LENGTH - 3, 80, 80, 80);
		PRISONSELECT.DrawSprite(FEDCPST_BACK4, 0    , 136.53, OT_LENGTH - 3, 80, 80, 80);
		PRISONSELECT.DrawSprite(FEDCPST_BACK5, 204.8, 136.53, OT_LENGTH - 3, 80, 80, 80);
		PRISONSELECT.DrawSprite(FEDCPST_BACK6, 409.6, 136.53, OT_LENGTH - 3, 80, 80, 80);
	
		// draw title bar
		SINT	c0;
		for (c0 = 0; c0 < 5; c0 ++)
		{
			PRISONSELECT.DrawSprite(FEDCPST_LCD1 + c0, 102.2 * float(c0), TITLE_BAR_Y, 2, 127, 127, 127);
		}
	
		switch (mState)
		{
		case FESS_LANGUAGE:
			{	
				// Setup engine things
				ENGINE.SetCamera(&mCamera);
			

				switch (mLanguage)
				{
					case 0: sprintf(text, "Choose Language");				break;
					case 1: sprintf(text, "S%clectionner langue", 233);		break;
					case 2: sprintf(text, "Sprache w%chlen", 228);				break;
				}
				SINT	sl = PRISONSELECT.GetStringSize(2, text);
				PRISONSELECT.DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25 + 1, 0x007f5f3f);
			
				switch (mLanguage)
				{
					case 0: sprintf(text, "^ To Confirm");			break;
					case 1: sprintf(text, "Appuyer sur ^ pour confirmer");			break;
					case 2: sprintf(text, "Zur Best%ctigung ^ dr%ccken", 228, 252);			break;
				}
				sl = PRISONSELECT.GetStringSize(0, text);
				PRISONSELECT.DrawString(0, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 220, 0xffffffff);
				
					//************************
				// draw flags
				
				// setup the camera
				mCamera.mPos = GVector(G0, -G(30), -G(2, 32768));
				mCamera.mLookAt = GVector(G0, 0, -G(2, 32768));
				mCamera.mMode = CCamera::LOOKAT;
				mCamera.mYaw = G0;
				mCamera.mPitch = G0;
				mCamera.FillOut();
			
				// set mesh position and orientation
				for (c0 = 0; c0 < 3; c0 ++)
				{
					CFrontEndThing	*t = &mFlags[c0];
					t->mWhite = (mLanguage == c0);
					t->mBlack = FALSE;
					
					#define	FLAG_Z1	(-G(0, 40000))
		//			#define	FLAG_Z1	(-G(3, 20000))
					
					#define	FXO		(-G(2))
					
					switch (c0)
					{
					case 0:	t->mPos = GVector(-G(9) + FXO, G0, FLAG_Z1);	break;
					case 1:	t->mPos = GVector( G0    + FXO, G0, FLAG_Z1);	break;
					case 2:	t->mPos = GVector( G(9) + FXO, G0, FLAG_Z1);	break;
					};
			
					SRTInfo	ri;
					ri.Clear();
					ri.i0 = FEPSM_ANGEL + c0 + 6 + 10000;
					ri.i1 = -1;
					ri.i2 = -2;
			
					CDCMesh::RenderFromData(t, &ri, FALSE);
					
				}
			}
			break;
			
		case FESS_ASKON:
			{
				GINT	gh = G(mAskMenuSlide) / SLIDE_TIME;
				SINT	col = (gh * 40).Whole();
				gh = G1 - gh;
				gh = (gh * gh);
				gh = G1 - gh;
			
				SINT	w = (gh * 480).Whole();
				SINT	h = (gh * 150).Whole();
			
				PRISONSELECT.DrawBox(256 - w / 2, 160 - h / 2, w, h, 3, col, col, col);

				CText::GetString(TEXT_DC_SEL_DISPLAY, text);
				SINT	sl = PRISONSELECT.GetStringSize(2, text);
				PRISONSELECT.DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25 + 1, 0x007f5f3f);

			}
			break;
		
		case FESS_ASK:
			{
				SINT	w = 480;
				SINT	h = 150;
				PRISONSELECT.DrawBox(256 - w / 2, 160 - h / 2, w, h, 3, 40, 40, 40);
				CText::GetString(TEXT_DC_SEL_DISPLAY, text);
				SINT	sl = PRISONSELECT.GetStringSize(2, text);
				PRISONSELECT.DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25 + 1, 0x007f5f3f);
				
				
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_SEL_DISPLAY, s);
				sl = PRISONSELECT.GetStringSize(2, s);
				PRISONSELECT.DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 20;
				
				SINT	colr = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(48)).Whole() + 48;
				SINT	colg = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(36)).Whole() + 48;
				SINT	colb = colr / 4;
				SINT	col = (colb << 16) + (colg << 8) + colr;
				
				DWORD	c1, c2, c3;
				
				if (mAskPos == 0)
				{
					c1 = col;
					c2 = 0x004f4f4f;
					c3 = 0x004f4f4f;
				}
				else if (mAskPos == 1)
				{
					c1 = 0x004f4f4f;
					c2 = col;
					c3 = 0x004f4f4f;
				}
				else
				{
					c1 = 0x004f4f4f;
					c2 = 0x004f4f4f;
					c3 = col;
				}
				
				CText::GetString(TEXT_DC_50HZ, s);
				sl = PRISONSELECT.GetStringSize(2, s);
				PRISONSELECT.DrawString(2, s, 256 - sl / 2, ty , c1);
				ty += 16;

				CText::GetString(TEXT_DC_60HZ, s);
				sl = PRISONSELECT.GetStringSize(2, s);
				PRISONSELECT.DrawString(2, s, 256 - sl / 2, ty , c2);
				ty += 16;

				CText::GetString(TEXT_DC_TEST, s);
				sl = PRISONSELECT.GetStringSize(2, s);
				PRISONSELECT.DrawString(2, s, 256 - sl / 2, ty , c3);
				ty += 25;
				
				CText::GetString(TEXT_DC_60MESS1, s);
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);
				ty += 9;
				
				CText::GetString(TEXT_DC_60MESS2, s);
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);	
				ty += 9;

				CText::GetString(TEXT_DC_60MESS3, s);
				sl =PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);	
				ty += 9;

				CText::GetString(TEXT_DC_60MESS4, s);
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);	
				ty += 9;
					
				CText::GetString(TEXT_HELPDC5, s);
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, 220 , 0x007f7f7f);
							
			}
			break;				

		case FESS_TEST1:
			{
				SINT	w = 480;
				SINT	h = 150;
				PRISONSELECT.DrawBox(256 - w / 2, 160 - h / 2, w, h, 3, 40, 40, 40);
				CText::GetString(TEXT_DC_SEL_DISPLAY, text);
				SINT	sl = PRISONSELECT.GetStringSize(2, text);
				PRISONSELECT.DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25 + 1, 0x007f5f3f);
				
				SINT ty = 95;

				CText::GetString(TEXT_DC_60TEST1, s);
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);
				ty += 9;

				CText::GetString(TEXT_DC_60TEST2, s);
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);
				ty += 9;
			}
			break;

		case FESS_TEST2:
			{
				SINT	w = 480;
				SINT	h = 150;
				PRISONSELECT.DrawBox(256 - w / 2, 160 - h / 2, w, h, 3, 40, 40, 40);
				CText::GetString(TEXT_DC_SEL_DISPLAY, text);
				SINT	sl = PRISONSELECT.GetStringSize(2, text);
				PRISONSELECT.DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25 + 1, 0x007f5f3f);
				
				SINT ty = 95;

				CText::GetString(TEXT_DC_60TESTING, s);
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);
				ty += 15;

				sprintf(s, "%d", 5 - (mCounter / 60));
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, ty , 0xffffffff);
				ty += 9;
			}
			break;
			
		case FESS_ASK2:
			{
				SINT	w = 480;
				SINT	h = 150;
				PRISONSELECT.DrawBox(256 - w / 2, 160 - h / 2, w, h, 3, 40, 40, 40);
				CText::GetString(TEXT_DC_SEL_DISPLAY, text);
				SINT	sl = PRISONSELECT.GetStringSize(2, text);
				PRISONSELECT.DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25 + 1, 0x007f5f3f);
				
				
				SINT ty = 95;
				
				CText::GetString(TEXT_DC_60RESPONSE, s);
				sl = PRISONSELECT.GetStringSize(2, s);
				PRISONSELECT.DrawString(2, s, 256 - sl / 2, ty , 0x007f7f7f);
				
				ty += 20;
				
				SINT	colr = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(48)).Whole() + 48;
				SINT	colg = ((GSIN(G(mCounter) * G(0, 10000)) + G1) * G(36)).Whole() + 48;
				SINT	colb = colr / 4;
				SINT	col = (colb << 16) + (colg << 8) + colr;
				
				DWORD	c1, c2;
				
				if (mAsk2Pos == 0)
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
				sl = PRISONSELECT.GetStringSize(2, s);
				PRISONSELECT.DrawString(2, s, 256 - sl / 2, ty , c1);
				ty += 16;

				CText::GetString(TEXT_NO, s);
				sl = PRISONSELECT.GetStringSize(2, s);
				PRISONSELECT.DrawString(2, s, 256 - sl / 2, ty , c2);
				ty += 16;

				CText::GetString(TEXT_HELPDC5, s);
				sl = PRISONSELECT.GetStringSize(0, s);
				PRISONSELECT.DrawString(0, s, 256 - sl / 2, 220 , 0x007f7f7f);
							
			}
			break;				
		};
	}			


	PLATFORM.GeometryMarkEndScene();

}

//******************************************************************************************
SINT	CDCFELanguageSelect::Run()
{
	// Load stuff
	if (!LoadData())
		return 0; // oops

#if DISP_MODE == MODE_NTSC
	mLanguage = 0;	// well - we loaded some useful data anyway...
#else
	SOUND.DeclareVolumeChange();  // update volumes
	SetStreamVolume(0);	
	PlayStream(1);
	SetStreamVolume(0);	
	// misc engine setup
	mCamera.Reset(0);
	PLATFORM.SetScreenWidth(512);
	
	SDSSetClipRegion(0,0,640,480);  // JCLDCNOPSX
	SDSRenderStart();
	SDSRenderFlip();

	mDBX  = FALSE;
	mDBY  = FALSE;

	SINT	c0;
	for (c0 = 0; c0 < 5; c0 ++)
	{
		mFlags[c0].Reset(c0 + 6);
	}
	
	mLanguage = 0;
	mQuit = 0;
	mCounter = 0;
	
	mState = FESS_LANGUAGE;

	//!!!!!!!***********  CHECK GAME DATA (eg water) *******************
	GAME.DeclareSinglePlayer();		// for the engine

	// and run!
	while (mQuit != 4)
	{
		CONTROLS.Update();
		SOUND.UpdateStatus();

		if ((CONTROLS.IsButtonPressedDB(BUTTON_DC_RESET, 0)) ||
			(CONTROLS.IsButtonPressedDB(BUTTON_DC_RESET, 1)))
			SDSSoftReset(); // Bye!

		Process();
		Render();
	}
	
#endif	

	return mLanguage;
}

#endif
