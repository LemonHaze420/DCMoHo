#include	"Common.h"

#if TARGET == PSX

#include	"Globals.h"
#include	"PSXFEDemo3End.h"

#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
#include	"Career.h"

CPSXFEDemo3End	FEENDDEMO3;

#define	TITLE_BAR_Y	10

//******************************************************************************************
BOOL	CPSXFEDemo3End::LoadData()
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

	printf("Level Pool - %d / %d  used\n", POOL->mLevelPoolSize, LEVEL_POOL_SIZE);

	return TRUE;
}

//******************************************************************************************
void	CPSXFEDemo3End::Process()
{
	if (mQuit != 0)
		mQuit ++;

	if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_X))
	{
		mQuit = 1;
		SOUND.PlaySample(1);
	}

	if (CONTROLS.IsButtonPressedDB(BUTTON_PSX_SELECT))
	{
		mQuit = 1;
		SOUND.PlaySample(1);
	}

	mCounter ++;
	
	// timeout?
	if (mCounter == 25 * 60)
	{
		mQuit = 1;
		mLevel = 3;
	}
}
//******************************************************************************************
POLY_FT4	*CPSXFEDemo3End::DrawSprite(SINT n, SINT px, SINT py, SINT otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey, GINT scale, SINT flip)
{
	NTAG				*o  = PLATFORM.GetOrderTable();
	POLY_FT4			*p4 = PLATFORM.GetMiscPolyFT4s();
	STextureRefOther 	*tr = ENGINE.GetNumberedTexture(n);

	setPolyFT4(p4);
	setXYWH(p4, px, py, (G(tr->mW) * scale).Whole(), (G(tr->mH) * scale).Whole() );
	setRGB0(p4, r, g, b);
	switch (semitrans)
	{
	case  1: p4->tpage = tr->mTPAGE_P1P1;	break;
	case  2: p4->tpage = tr->mTPAGE_P1M1;	break;
	case  3: p4->tpage = tr->mTPAGE_P1P025;	break;
	default:
	case  0: p4->tpage = tr->mTPAGE_P05P05;	break;
	};
    p4->clut  = tr->mCLUT;
    
    if (grey)
    	p4->clut -= 1 << 6;  //! hope this works

	SINT 	w = tr->mW;
	SINT	h = tr->mH;

	if (tr->mX + w == 256)
		w --;

	if (tr->mY + h == 256)
		h --;

	switch (flip)
	{
	case 0:
		setUVWH(p4, tr->mX    , tr->mY    ,  w,  h);
		break;
	case 1:
		setUVWH(p4, tr->mX + w, tr->mY    , -w,  h);
		break;
	case 2:
		setUVWH(p4, tr->mX    , tr->mY + h - 1,  w, -h + 1);
		break;
	case 3:
		setUVWH(p4, tr->mX + w, tr->mY + h - 1, -w, -h + 1);
		break;
	};		
	
	if (semitrans != -1)
		setSemiTrans(p4, TRUE);
	addPrim(&o[otdepth], p4);
	PLATFORM.DeclareUsedMiscPolyFT4s(p4 + 1);

	return p4;
}

//******************************************************************************************
UINT	get_code(SINT time, SINT level, SINT key)
{
	SINT	c0;
	if ((time <= 0) || (time > 0xffff))
		time = 0xffff;
	
	SINT	time_enc  = time ^ (key + (key << 8));
	SINT	level_enc = level ^ key;

	// checksum	
	SINT	sum = 0;
	
	for (c0 = 0; c0 < 16; c0 ++)
		sum += ((time >> c0) & 1);
		
	SINT	sum_enc = sum ^ key;
		
	
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

	return code;
}

//******************************************************************************************
void	CPSXFEDemo3End::Render()
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
	
		CText::GetString(TEXT_MOHO_DEMO, text);
		SINT	sl = ENGINE.GetStringSize(0, text);
		ENGINE.DrawString(0, text, PLATFORM.ScreenWidth() / 2 - sl / 2, TITLE_BAR_Y + 25, 0x007f5f3f);
	
		CText::GetString(TEXT_HELP2, text);
		sl = ENGINE.GetStringSize(2, text);
		ENGINE.DrawString(2, text, 256 - sl / 2, 220 , 0x007f7f7f);
	
		//**********************
		// Menu
		SINT	yps;
		GINT	gy = G1;
	
		SINT	c0;
		SINT	y = 95;
	
		for (c0 = 0; c0 < 4; c0 ++)
		{
			SINT 	c1;
			SINT	x = 112;
	
			GINT	gx;

			SINT	colr = 60, colg = 72, colb = 100;

			if (c0 == 3)
			{
				colr = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(48)).Whole() + 48;
				colg = ((GSIN(G(mCounter) * G(0, 15000)) + G1) * G(36)).Whole() + 48;
				colb = colr / 4;
			}
			
			if (c0 == 0)
			{
				colr = 80;
				colg = 100;
				colb = 60;
			}

			DrawSprite(FEPST_BOXL, x    , y    , 2, colr, colg, colb, 2);
			x += 48;

			for (c1 = 0; c1 < 4; c1 ++)
			{
				DrawSprite(FEPST_BOXM, x    , y    , 2, colr, colg, colb, 2);
				x += 48;
			}
			DrawSprite(FEPST_BOXR, x    , y    , 2, colr, colg, colb, 2);

			// draw text
			char	s[300], s2[200], f = 0;
			switch (c0)
			{
			case 0:
				switch (mLevel)
				{
				case 0:	CText::GetString(TEXT_LNAME_031, s2); break;
				case 1:	CText::GetString(TEXT_LNAME_005, s2); break;
				case 2:	CText::GetString(TEXT_LNAME_003, s2); break;
				};
				sprintf(s, "%s:", s2);
				break;
				
			case 1:
				CText::GetString(TEXT_BEST_TIME, s2);
				if (mTime == -1)
					sprintf(s, "%s  --:--.--", s2);
				else
				{
					SINT	minutes = mTime / 60 / 25;
					SINT	seconds = (mTime / 25) % 60;
					SINT	milli   = (mTime % 25) * 4;
		
					sprintf(s, "%s  %d:%02d.%02d", s2, minutes, seconds, milli);
				}
				f = 2;
				break;
				
			case 2:
				CText::GetString(TEXT_YOUR_CODE, s2);
				if (mTime == -1)
					sprintf(s, "%s:  --------",s2);
				else
					sprintf(s, "%s:  %08X", s2, get_code(mTime, mLevel, mKey));
				f = 2;
				break;
				
			case 3:
				CText::GetString(TEXT_OK, s);
				break;
			};

			SINT	tcol;

//			if (mLevel == c0 - 1)
				tcol = 0x007f7f7f;
//			else
//				tcol = 0x005f5f5f;

			SINT	sl = ENGINE.GetStringSize(f, s);
			ENGINE.DrawString(f, s, PLATFORM.ScreenWidth() / 2 - sl / 2 - 1, y + 5 - 1, tcol);
			ENGINE.DrawString(f, s, PLATFORM.ScreenWidth() / 2 - sl / 2    , y + 5    , 0x001f0000);

			y += 28;
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
SINT	CPSXFEDemo3End::Run(SINT time, SINT level)
{
	// Load stuff
	if (!LoadData())
		return 0; // oops

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

	mQuit = 0;
	mCounter = 0;
	mLevel = 0;
	mTime = time;
	mLevel = level;
	mKey = rand() & 0xff;

	//!!!!!!!***********  CHECK GAME DATA (eg water) *******************
	GAME.DeclareSinglePlayer();		// for the engine

	// and run!
	while (mQuit != 4)
	{
		CONTROLS.Update();
		SOUND.UpdateStatus();

		Process();
		Render();
	}
}

#endif
