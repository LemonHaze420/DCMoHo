// $Header$

// $Log$
// Revision 1.5  2000-09-14 17:15:59+01  jcl
// Remapped samples
//
// Revision 1.4  2000-09-11 15:05:12+01  are
// Draw alpha textures last.
//
// Revision 1.3  2000-09-07 16:43:48+01  are
// Unified controller update
//
// Revision 1.2  2000-08-11 17:22:33+01  are
// Now processes Windows messages and restores surfaces / D3D.
//
// Revision 1.1  2000-08-05 10:33:13+01  jjs
// First PC milestone.
//

#include	"Common.h"

#if TARGET == PC

#include	"PCFELanguageSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
#include	"Globals.h"
#include	"PCBuildType.h"
#include	"rtmesh.h"


CPCFELanguageSelect	LANGUAGESELECT;

#define	TITLE_BAR_Y	10

#define	SOUNDSELECT	SOUND.PlaySample(SOUND_SELECT)
#define	SOUNDMOVE	SOUND.PlaySample(SOUND_MOVE)


//******************************************************************************************
BOOL	CPCFELanguageSelect::LoadData()
{
	CInitRenderThing ri;
	CFrontEndThing	*t;
	
	t = &mFlags[0];
	strcpy(ri.mName,"flag_uk.msh");
	t->mMesh.Init(&ri);

	t = &mFlags[1];
	strcpy(ri.mName,"flag_fr.msh");
	t->mMesh.Init(&ri);

	t = &mFlags[2];
	strcpy(ri.mName,"flag_gr.msh");
	t->mMesh.Init(&ri);

	t = &mFlags[3];
	strcpy(ri.mName,"flag_sp.msh");
	t->mMesh.Init(&ri);

	t = &mFlags[4];
	strcpy(ri.mName,"flag_it.msh");
	t->mMesh.Init(&ri);

	back		= CTextureTL::GetTexture("frontend\\1back.tga");
	titlea		= CTextureTL::GetTexture("frontend\\menpsxa.tga");
	titleb		= CTextureTL::GetTexture("frontend\\menpsxb.tga");

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
void	CPCFELanguageSelect::Process()
{
	if (mQuit != 0)
		mQuit ++;

	PLATFORM.ProcessMessages();
	
	GINT xm = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT);
	GINT ym = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP);

	if (ym != G0)
	{
		if (!mDBY)
		{
			if (ym < G0)
			{
				switch(mLanguage)
				{
				case 0:	mLanguage = 3;	break;
				case 1:	mLanguage = 3;	break;
				case 2:	mLanguage = 4;	break;
				};
			}
			else
			{
				switch(mLanguage)
				{
				case 3: mLanguage = 0; break;
				case 4: mLanguage = 2; break;
				};
			}
			mCounter = 0;
			SOUNDMOVE;

			mDBY = 6;
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
			if (xm > G0)
			{
				switch(mLanguage)
				{
				case 0:	mLanguage = 1;	break;
				case 1:	mLanguage = 2;	break;
				case 3:	mLanguage = 4;	break;
				};
			}
			else
			{
				switch(mLanguage)
				{
				case 1: mLanguage = 0; break;
				case 2: mLanguage = 1; break;
				case 4: mLanguage = 3; break;
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

	if (CONTROLS.IsButtonPressedDB(BUTTON_FIRE_PRIMARY))
	{
		mQuit = 1;
		SOUNDSELECT;
	}


	// Process Flag Things
	SINT	c0;
	for (c0 = 0; c0 < 5; c0 ++)
	{
		mFlags[c0].Process();
	}
	
	mCounter ++;
	
	// timeout?
	if (mCounter == 25 * 30)
	{
		mLanguage = 0;
		mQuit = 1;
	}
}

//******************************************************************************************
void	CPCFELanguageSelect::Render()
{
	PLATFORM.GetDPWin()->Clear(0x00000000); // ####!!!!
	if (PLATFORM.GetBackSurface()->GetDevice()->BeginScene() == D3D_OK)
	{
		if (mQuit == 0)
		{
			ENGINE.GetDevice()->SetViewport(PLATFORM.GetDPWin()->DSGetViewport(0));
			ENGINE.SetCamera(&mCamera);
			//************************
			// Background
		
			// draw title bar
			ENGINE.DrawSpritePC(back, 0  , 0  , .999f, 256, 128,0xffffffff, PCMULTX, PCMULTY, 0);
			ENGINE.DrawSpritePC(back, 256, 0  , .999f, 256, 128, 0xffffffff, PCMULTX, PCMULTY, 1);
			ENGINE.DrawSpritePC(back, 0  , 128, .999f, 256, 128, 0xffffffff, PCMULTX, PCMULTY, 2);
			ENGINE.DrawSpritePC(back, 256, 128, .999f, 256, 128, 0xffffffff, PCMULTX, PCMULTY, 3);
			back->DrawAll();
			ENGINE.DrawSpritePC(titlea, 0, TITLE_BAR_Y, .99f, 256, 72, 0xffffffff, PCMULTX, PCMULTY, 0);
			ENGINE.DrawSpritePC(titleb, 256, TITLE_BAR_Y, .99f, 256, 72, 0xffffffff, PCMULTX, PCMULTY, 0);
		
			char	text[200];
		
			switch (mLanguage)
			{
				case 0: sprintf(text, "Choose Language");				break;
				case 1: sprintf(text, "S%clectionner langue", 233);		break;
				case 2: sprintf(text, "Sprache w%chlen", 228);				break;
				case 3: sprintf(text, "Selecci%cn del idioma", 243);			break;
				case 4: sprintf(text, "Selezionare la lingua");			break;
			}
			SINT	sl = ENGINE.GetStringSize(2, text);
			ENGINE.DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, (TITLE_BAR_Y + 25) * PCMULTY, 0x007fbfff);
		
			switch (mLanguage)
			{
				case 0: sprintf(text, "[ To Confirm");			break;
				case 1: sprintf(text, "Appuyer sur [ pour confirmer");			break;
				case 2: sprintf(text, "Zur Best%ctigung [ dr%ccken", 228, 252);			break;
				case 3: sprintf(text, "Pulsa [ para confirmar la selecci%cn", 243);			break;
				case 4: sprintf(text, "Premere [ per confermare");			break;
			}
			sl = ENGINE.GetStringSize(0, text);
			ENGINE.DrawString(0, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 220 * PCMULTY, 0x007fbfff);
	
			//************************
			// draw flags	
			// setup the camera
			mCamera.mPos = GVector(G0, -G(30), -G(2, 32768));
			mCamera.mMode = CCamera::LOOKAT;
			mCamera.mYaw = G0;
			mCamera.mPitch = G0;
			mCamera.FillOut();
			mCamera.mLookAt = mCamera.mOrientation * GVector(G0,G1,G0);	// Needed for PC version

			// aspect ratio
//			mCamera.mOrientation.Row[0].Y=-(mCamera.mOrientation.Row[0].Y*G(0,50000));
//		   	mCamera.mOrientation.Row[1].Y=-(mCamera.mOrientation.Row[1].Y*G(0,50000));
//		   	mCamera.mOrientation.Row[2].Y=-(mCamera.mOrientation.Row[2].Y*G(0,50000));
	
			// set mesh position and orientation
			SINT	c0;
		
			for (c0 = 0; c0 < 5; c0 ++)
			{
				CFrontEndThing	*t = &mFlags[c0];
				t->mWhite = (mLanguage == c0);
				t->mBlack = FALSE;
				
				#define	FLAG_Z1	(-G(3, 20000))
				#define	FLAG_Z2	( G(2, 30000))
				
				#define	FXO		(-G(2))
				
				switch (c0)
				{
				case 0:	t->mPos = GVector(-G(9) + FXO, G0, FLAG_Z1);	break;
				case 1:	t->mPos = GVector( G0    + FXO, G0, FLAG_Z1);	break;
				case 2:	t->mPos = GVector( G(9) + FXO, G0, FLAG_Z1);	break;
				case 3:	t->mPos = GVector(-G( 5) + FXO, G0, FLAG_Z2);	break;
				case 4:	t->mPos = GVector( G( 5) + FXO, G0, FLAG_Z2);	break;
				};

				t->Render();
			}
		}
		CTexture::DrawAllTextures();
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
SINT	CPCFELanguageSelect::Run()
{
	// Load stuff
	if (!LoadData())
		return 0; // oops

	CONTROLS.SetControllerMode(CM_KEYBOARD);  // so we can edit!!
#if DISP_MODE == MODE_NTSC
	mLanguage = 0;	// well - we loaded some useful data anyway...
#else

	SOUND.DeclareVolumeChange();  // update volumes

	// misc engine setup
	mCamera.Reset(0);

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

	//!!!!!!!***********  CHECK GAME DATA (eg water) *******************
	GAME.DeclareSinglePlayer();		// for the engine

	// and run!
	while (mQuit != 4)
	{
	//	CONTROLS.Update(); // Now processed as part of the Process method.
//		SOUND.UpdateStatus();

//		if (CONTROLS.IsButtonPressedDB(BUTTON_SCREENSHOT))
//			GAME.TakeScreenShot();

		Process();
		Render();
	}

#endif	

	return mLanguage;
}

#endif
