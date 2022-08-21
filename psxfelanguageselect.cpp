#include	"Common.h"

#if TARGET == PSX

#include	"PSXFELanguageSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
#include	"Globals.h"
#include	"PSXBuildType.h"


CPSXFELanguageSelect	LANGUAGESELECT;

#define	TITLE_BAR_Y	10

#define	SOUNDSELECT	SOUND.PlaySample(1)
#define	SOUNDMOVE	SOUND.PlaySample(4)


//******************************************************************************************
BOOL	CPSXFELanguageSelect::LoadData()
{
	SOUND.AllocSampleBank("MOHOFE");

	UINT	CID_FEPS	= MKID("FEPS");
	UINT	CID_TPAG	= MKID("TPAG");
	UINT	CID_MESH	= MKID("MESH");

	CMEMBUFFER	mem;

	PLATFORM.ClearPools(); // free up global pools

	char	name[256];
	sprintf(name, "FRONTEND\\PRISEL.DAT");

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
void	CPSXFELanguageSelect::Process()
{
	if (mQuit != 0)
		mQuit ++;

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

	if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_X))
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
void	CPSXFELanguageSelect::Render()
{
    int safe;
    do
    {
    	safe = PLATFORM.GeometryMarkBeginScene();
    } while (safe == -1);


	NTAG	*o = PLATFORM.GetOrderTable();
	
	if (mQuit == 0)
	{
		// Setup engine things
		PLATFORM.SetViewport(0);
		ENGINE.SetCamera(&mCamera);
	//	GouraudDepth=4096;      // Make sure all drawn polys are G3's....
		GouraudDepth=0x7fff7fff;      // Make sure all drawn polys are G3's....
	
		//************************
		// Background
	
		ENGINE.DrawSprite(FEPST_BACK, 0  , 0  , OT_LENGTH - 3, 127, 127, 127, FALSE, FALSE, G1, 0);
		ENGINE.DrawSprite(FEPST_BACK, 256, 0  , OT_LENGTH - 3, 127, 127, 127, FALSE, FALSE, G1, 1);
		ENGINE.DrawSprite(FEPST_BACK, 0  , 128, OT_LENGTH - 3, 127, 127, 127, FALSE, FALSE, G1, 2);
		ENGINE.DrawSprite(FEPST_BACK, 256, 128, OT_LENGTH - 3, 127, 127, 127, FALSE, FALSE, G1, 3);
	
		// draw title bar
		ENGINE.DrawSprite(FEPST_TITLEA, 0  , TITLE_BAR_Y, 2, 127, 127, 127, 2);
		ENGINE.DrawSprite(FEPST_TITLEB, 256, TITLE_BAR_Y, 2, 127, 127, 127, 2);
	
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
		ENGINE.DrawString(2, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25, 0x007f5f3f);
	
		switch (mLanguage)
		{
			case 0: sprintf(text, "[ To Confirm");			break;
			case 1: sprintf(text, "Appuyer sur [ pour confirmer");			break;
			case 2: sprintf(text, "Zur Best%ctigung [ dr%ccken", 228, 252);			break;
			case 3: sprintf(text, "Pulsa [ para confirmar la selecci%cn", 243);			break;
			case 4: sprintf(text, "Premere [ per confermare");			break;
		}
		sl = ENGINE.GetStringSize(0, text);
		ENGINE.DrawString(0, text, PLATFORM.ScreenWidth() / 2 - sl / 2, 220, 0x007f5f3f);
	
		//************************
		// draw flags
		
		// setup the camera
		mCamera.mPos = GVector(G0, -G(30), -G(2, 32768));
		mCamera.mMode = CCamera::LOOKAT;
		mCamera.mYaw = G0;
		mCamera.mPitch = G0;
		mCamera.FillOut();
	
		// aspect ratio
		mCamera.mOrientation.Row[0].Y=-(mCamera.mOrientation.Row[0].Y*G(0,50000));
	   	mCamera.mOrientation.Row[1].Y=-(mCamera.mOrientation.Row[1].Y*G(0,50000));
	   	mCamera.mOrientation.Row[2].Y=-(mCamera.mOrientation.Row[2].Y*G(0,50000));
	
		SPCameraGMatrix=SPCamera->mOrientation;
		ENGINE.GMatrixToMATRIX(&SPCameraGMatrix,&SPCameraMATRIX);
	
		// set mesh position and orientation
		SINT	c0;
		MATRIX lcol;
	
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
	
			SRTInfo	ri;
			ri.Clear();
			ri.i0 = FEPSM_ANGEL + c0 + 6 + 10000;
			ri.i1 = -1;
			ri.i2 = -2;
	
			CPSXMesh::RenderFromData(t, &ri, FALSE);
			
/*			t->mWhite = FALSE;
			t->mBlack = TRUE;
			
			t->mPos.X += G(1);
			t->mPos.Y += G(3);
			t->mPos.Z += G(1);
			CPSXMesh::RenderFromData(t, &ri, FALSE);
*/			
		}
	}

	//************************
	// draw ordertable

	int f=PLATFORM.GetFrame();
	NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f));
	NTAG_Rlink(PLATFORM.GetOrderTable(),OT_LENGTH,(long*)&POOL->mNTAGTerms[f][0].firstprim
												,(long*)&POOL->mNTAGTerms[f][0].lastprim);


	PLATFORM.GeometryMarkEndScene();


}

//******************************************************************************************
SINT	CPSXFELanguageSelect::Run()
{
	// Load stuff
	if (!LoadData())
		return 0; // oops

#if DISP_MODE == MODE_NTSC
	mLanguage = 0;	// well - we loaded some useful data anyway...
#else
	SOUND.DeclareVolumeChange();  // update volumes

	// misc engine setup
	mCamera.Reset(0);
	SPCamera=&mCamera;
	SPCameraGMatrix=SPCamera->mOrientation;
	ENGINE.GMatrixToMATRIX(&SPCameraGMatrix,&SPCameraMATRIX);
	PLATFORM.SetScreenWidth(512);
	int sx = PLATFORM.ScreenWidth()>>1;
	SetGeomOffset(sx, PLATFORM.ScreenHeight()>>1);
	OffScreenMask=0xff00fe00;

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
		CONTROLS.Update();
		SOUND.UpdateStatus();

//		if (CONTROLS.IsButtonPressedDB(BUTTON_SCREENSHOT))
//			GAME.TakeScreenShot();

		Process();
		Render();
	}
	
#endif	

	return mLanguage;
}

#endif
