#include	"Common.h"

#if TARGET == PSX

#include	"PSXFELevelSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"

CPSXFELevelSelect	LEVELSELECT;

//******************************************************************************************
BOOL	CPSXFELevelSelect::LoadData()
{
	UINT	CID_FED		= MKID("FED!");
	UINT	CID_TPAG	= MKID("TPAG");

	CMEMBUFFER	mem;

	PLATFORM.ClearPools(); // free up global pools

	char	name[256];
	sprintf(name, "FRONTEND\\LEVSEL.DAT");

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

	if (chunk != CID_FED)
		return FALSE;

	// Read Texture page
	mem.Read(&chunk, 4);
	if (chunk != CID_TPAG)
		return FALSE;

	ENGINE.ReadPSXTextures(mem);

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
void	CPSXFELevelSelect::Process()
{
	if (!mQuit)
	{
		GINT xm = CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT);
		GINT ym = CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP);

	}
}

//******************************************************************************************
POLY_FT4	*CPSXFELevelSelect::DrawSprite(SINT n, SINT px, SINT py, SINT xs, SINT ys, SINT uvm)
{
	NTAG				*o  = PLATFORM.GetOrderTable();
	POLY_FT4			*p4 = PLATFORM.GetMiscPolyFT4s();
	STextureRefOther 	*tr = ENGINE.GetNumberedTexture(n);

	setPolyFT4(p4);
	setXYWH(p4, px, py, (tr->mW * xs) / 256, (tr->mH * ys) / 256);
	setRGB0(p4, 128, 128, 128);
    p4->tpage = tr->mTPAGE_P1M1;
    p4->clut  = tr->mCLUT;
    switch(uvm)
    {
    case 0:		setUVWH(p4, tr->mX         , tr->mY         ,  tr->mW,  tr->mH);	break;
    case 1:		setUVWH(p4, tr->mX + tr->mW, tr->mY         , -tr->mW,  tr->mH);	break;
    case 2:		setUVWH(p4, tr->mX         , tr->mY + tr->mH,  tr->mW, -tr->mH);	break;
    case 3:		setUVWH(p4, tr->mX + tr->mW, tr->mY + tr->mH, -tr->mW, -tr->mH);	break;
    }
	addPrim(&o[1], p4);
	PLATFORM.DeclareUsedMiscPolyFT4s(p4+1);

	return p4;
}

//******************************************************************************************
void	CPSXFELevelSelect::Render()
{
	DrawSync(0);
	// Clear Ordertable
	//ClearOTagR(PLATFORM.GetOrderTable(), OT_LENGTH);
	NTAG	*o = PLATFORM.GetOrderTable();
	NTAG_init(o,OT_LENGTH);


	RECT 	r;
	SINT 	x, y;
	PLATFORM.GetScreenPos(x, y);

	r.x = 512;
	r.y = 0;
	r.w = 512;
	r.h = 256;

	MoveImage(&r, x, y);

	//************************
	// and some text
	DWORD col = 0xff0000ff;
	ENGINE.DrawString(0, "Level Select!", 20, 20, col);


	//************************
	// draw ordertable

	int f=PLATFORM.GetFrame();
	//DrawOTag(PLATFORM.GetOrderTable() + OT_LENGTH - 1); // Queue polys for next frame.

	// Now we need to 'link' the NTAG chain to the correct DMA header.
	// This frees up the NTAG array for the next time around....
	// *NOTE* this doesn't end the DMA chain, it leaves a 'NULL' in the corrrct mNTAGTerms.lastprim

	NTAG_Rlink(PLATFORM.GetOrderTable(),OT_LENGTH,(long*)&POOL->mNTAGTerms[f][0].firstprim
												,(long*)&POOL->mNTAGTerms[f][0].lastprim);
	DrawSync(0);
	DrawOTag((unsigned long *)(&POOL->mNTAGTerms[f][0].firstprim));


}

//******************************************************************************************
SINT	CPSXFELevelSelect::Run()
{
	// Load stuff
	if (!LoadData())
		return 0; // oops

	SOUND.PlayTrack(0,2);


	mQuit = FALSE;

	// and run!
	while (!mQuit)
	{
		CONTROLS.Update();

		Process();
		Render();
		PLATFORM.Flip();
	}

	return 501;
}




#endif
