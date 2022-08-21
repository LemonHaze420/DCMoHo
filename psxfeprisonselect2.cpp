#include	"Common.h"

#if TARGET == PSX

#include	"PSXFEPrisonSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
#include	"PSXMemCard.h"
#include	"Career.h"
#include	"Globals.h"


// *** This file is a continuation of PSXFEPrisonSelect, and contains functions
//     I don't want inlined...


//******************************************************************************************
void	CPSXFEPrisonSelect::AddParticles(SINT x, SINT y, SINT num, SINT type)
{
	SINT	c0, c1;
	
	for (c0 = 0; c0 < num; c0 ++)
	{
		for (c1 = 0; c1 < NUM_PSXFE_PARTICLES; c1 ++)
		{
			SPSXFEParticle *p = &mParticles[c1];
			
			if (p->mVal == 0)
			{
				p->mType = type;
				
				switch (type)
				{
				case 0:
					p->mX = G(x + (rand() % 20) - 10);
					p->mY = G(y + (rand() % 20) - 10);
					p->mVX = G((rand() % 40) - 20) / 5;
					p->mVY = G((rand() % 40) - 30) / 5;
					p->mVal = rand() % 255;
					break;
				case 1:
					p->mX = G(x + (rand() % 10) - 5);
					p->mY = G(y + (rand() % 10) - 5);
					p->mVX = G((rand() % 40) - 20) / 5;
					p->mVY = G((rand() % 40) - 30) / 5;
					p->mVal = rand() % 128;
					break;
				};
				break;
			}
		}
	}
}

//******************************************************************************************
POLY_FT4	*CPSXFEPrisonSelect::DrawSprite(SINT n, SINT px, SINT py, SINT otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey, GINT scale, SINT flip)
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
void	CPSXFEPrisonSelect::DrawSpriteXY(SINT n, SINT px1, SINT py1, SINT px2, SINT py2, SINT px3, SINT py3, SINT px4, SINT py4, SINT otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey)
{
	NTAG				*o  = PLATFORM.GetOrderTable();
	POLY_FT4			*p4 = PLATFORM.GetMiscPolyFT4s();
	STextureRefOther 	*tr = ENGINE.GetNumberedTexture(n);

	// Split to prevent warping...
	
	SINT	c0;
	for (c0 = 0; c0 < 2; c0 ++)
	{
		setPolyFT4(p4);
		switch (c0)
		{
		case 0:
			setXY4(p4, px1, py1, px2, py2, px1 + ((px3 - px1) / 2), py1 + ((py3 - py1) / 2), px2 + ((px4 - px2) / 2), py2 + ((py4 - py2) / 2));
			break;
		case 1:
			setXY4(p4, px1 + ((px3 - px1) / 2), py1 + ((py3 - py1) / 2), px2 + ((px4 - px2) / 2), py2 + ((py4 - py2) / 2), px3, py3, px4, py4);
			break;
		};
		
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
	    
	    if (grey) p4->clut -= 1 << 6;  //! hope this works

		switch(c0)
		{
		case 0:				
			setUVWH(p4, tr->mX, tr->mY,  tr->mW - 1,  tr->mH / 2);
			break;
		case 1:	
			setUVWH(p4, tr->mX, tr->mY + tr->mH / 2,  tr->mW - 1,  tr->mH / 2  - 1);
			break;
		}			
				
		if (semitrans != -1)
			setSemiTrans(p4, TRUE);
		addPrim(&o[otdepth], p4);
		p4 ++;
	}
	PLATFORM.DeclareUsedMiscPolyFT4s(p4);
}

//******************************************************************************************
void	CPSXFEPrisonSelect::DrawDynamicText(SINT fn, char *text, SINT x, SINT y, SINT r, SINT g, SINT b, SINT time, SINT otdepth)
{
	while (*text)
	{
		if (time <= 0)
			break;
			
		if (time < 8)
		{
			r = g = b = time << 5;
		}
		else if (time < 12)
		{
			r = 128 - (((128 - r) * (time - 8)) >> 2);
			g = 128 - (((128 - g) * (time - 8)) >> 2);
			b = 128 - (((128 - b) * (time - 8)) >> 2);
		}
		// else do nothing
	
		DWORD col = (b << 16) + (g << 8) + r;
		
		switch (*text)
		{
		case ' ':
			x += 5;
			break;

		default:
			x += ENGINE.DrawChar(fn, *((unsigned char *)text) - 32, x, y, col, otdepth) + 2;
			break;
		};

		text++;
		time -= 3;
	};
}

//******************************************************************************************
void	CPSXFEPrisonSelect::DrawBorder(SINT x, SINT y, SINT w, SINT h, SINT otdepth, SINT r, SINT b, SINT g)
{	
	// cue the colour from the incoming r value.
	
	SINT	alphatype;
	SINT	col;
	
	if (r < 20)
	{
		alphatype = 2;
		col = (r * 12) - 120;
		if (col < 0)
			col = 0;
	}
	else
	{
		alphatype = -1;
		col = (r - 20) * 6 + 8;
	}
	
	// corners
	DrawSprite(FEPST_BORD_CTL, x - 6    , y - 6    , otdepth, col, col, col, alphatype);
	DrawSprite(FEPST_BORD_CTR, x - 6 + w, y - 6    , otdepth, col, col, col, alphatype);
	DrawSprite(FEPST_BORD_CBL, x - 6    , y - 6 + h, otdepth, col, col, col, alphatype);
	DrawSprite(FEPST_BORD_CBR, x - 6 + w, y - 6 + h, otdepth, col, col, col, alphatype);
	
	// edges
	SINT	c0;
	
	// top
	for (c0 = x + 8; c0 < x + w - 6; c0 += 14)
		DrawSprite(FEPST_BORD_PT, c0, y - 6, otdepth, col, col, col, alphatype);

	// right
	for (c0 = y + 6; c0 < y + h - 6; c0 += 12)
		DrawSprite(FEPST_BORD_PR, x - 6 + w, c0, otdepth, col, col, col, alphatype);
	
	// bottom
	for (c0 = x - 6 - 14 + w; c0 > x + 8 - 14; c0 -= 14)
		DrawSprite(FEPST_BORD_PB, c0, y + h - 6, otdepth, col, col, col, alphatype);

	// left
	for (c0 = y - 6 - 12 + h; c0 > y + 6 - 12; c0 -= 12)
		DrawSprite(FEPST_BORD_PL, x - 6, c0, otdepth, col, col, col, alphatype);	
}

//******************************************************************************************
void	CPSXFEPrisonSelect::DrawBorderClip(SINT x, SINT y, SINT w, SINT otdepth, SINT r, SINT g, SINT b)
{
	DrawSprite(FEPST_LEVEL_CLIP_L, x - 60, y - 6, otdepth, r, g, b, 2);	
	DrawSprite(FEPST_LEVEL_CLIP_R, x  + w - 11, y - 6, otdepth, r, g, b, 2);

//	DrawSprite(FEPST_LEVEL_CLIP_L, x - 60, y - 6, otdepth, r, g, b);	
//	DrawSprite(FEPST_LEVEL_CLIP_R, x  + w - 11, y - 6, otdepth, r, g, b);	
//	DrawSprite(FEPST_LEVEL_CLIP_L, x - 60 + 3, y - 6 + 3, otdepth, r, g, b, 2);	
//	DrawSprite(FEPST_LEVEL_CLIP_R, x  + w - 11 + 3, y - 6 + 3, otdepth, r, g, b, 2);	
}

//******************************************************************************************
#define FAKE_PERSPECTIVE(x, val)  ((x) = ((G((x) - 256) * (val)).Whole() + 256))

void	CPSXFEPrisonSelect::DrawBorderClipRot(SINT s1, SINT s2,SINT x, SINT y, SINT w, SINT otdepth, SINT r, SINT g, SINT b, GINT rot, BOOL grey)
{
	GINT	rot2 = rot;

	if (rot2 > GINT_HALF)
		rot2 = G1 - rot2;

	STextureRefOther 	*tr;
	GINT				cs = (G1 - GCOS(rot2 * G_PI)) * GINT_HALF;
	SINT				x1, x2, x3, x4;
	SINT				y1, y2, y3, y4;
	GINT				ps = GSIN(rot * G_PI);
	if (rot < GINT_HALF)
		ps = -ps * G(0, 13000);
	else
		ps =  ps * G(0, 13000);
	
	tr = ENGINE.GetNumberedTexture(FEPST_LEVEL_CLIP_L);
	
	x1 = x - 60;
	x2 = x - 60 + tr->mW;
	x3 = x - 60;
	x4 = x - 60 + tr->mW;
	y1 = y - 6 + (G(tr->mH) * cs).Whole();
	y2 = y1;
	y3 = y - 6 + tr->mH - (G(tr->mH) * cs).Whole();
	y4 = y3;

	FAKE_PERSPECTIVE(x1, G1 + ps);
	FAKE_PERSPECTIVE(x2, G1 + ps);
	FAKE_PERSPECTIVE(x3, G1 - ps);
	FAKE_PERSPECTIVE(x4, G1 - ps);
	
	DrawSpriteXY(FEPST_LEVEL_CLIP_L, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, 127, 127, 127, 2);
//	x1 += 3; x2 += 3; x3 += 3; x4 += 3;
//	y1 += 3; y2 += 3; y3 += 3; y4 += 3;
//	DrawSpriteXY(FEPST_LEVEL_CLIP_L, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, 127, 127, 127, 2);

	tr = ENGINE.GetNumberedTexture(FEPST_LEVEL_CLIP_R);
	x1 = x + w - 11;
	x2 = x + w - 11 + tr->mW;
	x3 = x + w - 11;
	x4 = x + w - 11 + tr->mW;
	y1 = y - 6 + (G(tr->mH) * cs).Whole();
	y2 = y1;
	y3 = y - 6 + tr->mH - (G(tr->mH) * cs).Whole();
	y4 = y3;

	FAKE_PERSPECTIVE(x1, G1 + ps);
	FAKE_PERSPECTIVE(x2, G1 + ps);
	FAKE_PERSPECTIVE(x3, G1 - ps);
	FAKE_PERSPECTIVE(x4, G1 - ps);
	
	DrawSpriteXY(FEPST_LEVEL_CLIP_R, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, 127, 127, 127, 2);
//	x1 += 3; x2 += 3; x3 += 3; x4 += 3;
//	y1 += 3; y2 += 3; y3 += 3; y4 += 3;
//	DrawSpriteXY(FEPST_LEVEL_CLIP_R, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, 127, 127, 127, 2);

	SINT 	texn;
	
	if (rot < GINT_HALF)
		texn = s1;
	else
		texn = s2;

	tr = ENGINE.GetNumberedTexture(texn);
	
	x1 = x;
	x2 = x + tr->mW;
	x3 = x;
	x4 = x + tr->mW;
	y1 = y + (G(tr->mH) * cs).Whole();
	y2 = y1;
	y3 = y + tr->mH - (G(tr->mH) * cs).Whole();
	y4 = y3;

	FAKE_PERSPECTIVE(x1, G1 + ps);
	FAKE_PERSPECTIVE(x2, G1 + ps);
	FAKE_PERSPECTIVE(x3, G1 - ps);
	FAKE_PERSPECTIVE(x4, G1 - ps);
	
	DrawSpriteXY(texn, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, r, g, b, -1, grey);
/*	x1 += 3; x2 += 3; x3 += 3; x4 += 3;
	y1 += 3; y2 += 3; y3 += 3; y4 += 3;
	DrawSpriteXY(texn, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, r, g, b, 2);*/
}

//******************************************************************************************
void	CPSXFEPrisonSelect::DrawBox(SINT x, SINT y, SINT w, SINT h, SINT otdepth, SINT r, SINT b, SINT g)
{
	// draw the borders
	DrawBorder(x, y, w, h, otdepth, r, g, b);

	// and the box

	NTAG				*o  = PLATFORM.GetOrderTable();
	POLY_F4				*p4 = (POLY_F4 *)PLATFORM.GetMiscPolyG4s();

	setPolyF4(p4);
	setXYWH(p4, x, y, w, h);
	setRGB0(p4, r, g, b);
	setSemiTrans(p4, TRUE);	
	addPrim(&o[otdepth], p4);
	p4 ++;

	DR_MODE		*drm = (DR_MODE *)p4;
	
	SetDrawMode(drm, 1, 1, getTPage(0, 2, 0, 0), NULL);
	addPrim(&o[otdepth], drm);
	
	drm ++;
	
	PLATFORM.DeclareUsedMiscPolyG4s((POLY_G4 *)drm);

}

//******************************************************************************************
void	CPSXFEPrisonSelect::DrawLine(SINT sx, SINT sy, SINT ex, SINT ey, SINT otdepth, SINT r, SINT g, SINT b)
{
	NTAG				*o  = PLATFORM.GetOrderTable();
	LINE_F2				*f2 = (LINE_F2 *)PLATFORM.GetMiscPolyG4s();

	setLineF2(f2);
	setXY2(f2, sx, sy, ex, ey);
	setRGB0(f2, r, g, b);
	addPrim(&o[otdepth], f2);
	f2 ++;
	
	PLATFORM.DeclareUsedMiscPolyG4s((POLY_G4 *)f2);
}


#endif