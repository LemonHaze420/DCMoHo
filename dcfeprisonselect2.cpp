#include	"Common.h"

#if TARGET == DC

#include	<string.h>
#include	"DCFEPrisonSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
#include	"DCMemCard.h"
#include	"Career.h"
#include	"Globals.h"
#include 	"MainCode.h"
#include	"DCJunk.h"

// *** This file is a continuation of PSXFEPrisonSelect, and contains functions
//     I don't want inlined...


//******************************************************************************************
void	CDCFEPrisonSelect::AddParticles(SINT x, SINT y, SINT num, SINT type)
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
void	CDCFEPrisonSelect::DrawString(SINT font, char *s, float x, float y, DWORD col, float otdepth,BOOL HasBorder)
{
	// wrapper...
	
	// shadow bodge
	if ((col == 0) || (col == 0x001f0000))
	{
		// otdepth ++; - moved to ENGINE.DrawString
	}
	else
	{
		// check out this mammoth hack!
		if (((strcmp(mLastDrawnString, s)) && (!HasBorder)))
		{
			// draw a shadow, if it wasn't the last thing we did!
			ENGINE.DrawString(font, s, (x + 1.f) * DCMULTX, (y + 1.f) * DCMULTY, 0x001f0000, otdepth + 0.4f, HasBorder);
		}
	}
	
	strcpy(mLastDrawnString, s);			
	
	ENGINE.DrawString(font, s, x * DCMULTX, y * DCMULTY, col, otdepth, HasBorder);

}
//******************************************************************************************
SINT	CDCFEPrisonSelect::GetStringSize(SINT font, char *s)
{
	// wrapper
	float	size = ENGINE.GetStringSize(font, s);
	size /= DCMULTX;
	
	return SINT(size);
}

//******************************************************************************************
// bodge stuff for sprite sizes
SINT	GetSpriteWidth(SINT n)
{
	switch(n)
	{
	case	FEDCPST_PR_ECO:
	case	FEDCPST_PR_ZEPPELIN:
	case	FEDCPST_PR_HOSPITAL:
	case	FEDCPST_PR_SHIP:
	case	FEDCPST_PR_REHAB:
	case	FEDCPST_PR_MINING:
	case	FEDCPST_PR_DEATHROW:
	case	FEDCPST_PR_SPORTS:
	case	FEDCPST_PR_MILITARY:
	case	FEDCPST_PR_ALPHASTAR:
	case	FEDCPST_PR_BONUS:
		return 200;
		
	default:
	{
		STextureRefOther *tr = ENGINE.GetNumberedTexture(n);
		return SINT(tr->mW * 256.f);
	}
	
	};
}

SINT	GetSpriteHeight(SINT n)
{
	switch(n)
	{
	case	FEDCPST_PR_ECO:
	case	FEDCPST_PR_ZEPPELIN:
	case	FEDCPST_PR_HOSPITAL:
	case	FEDCPST_PR_SHIP:
	case	FEDCPST_PR_REHAB:
	case	FEDCPST_PR_MINING:
	case	FEDCPST_PR_DEATHROW:
	case	FEDCPST_PR_SPORTS:
	case	FEDCPST_PR_MILITARY:
	case	FEDCPST_PR_ALPHASTAR:
	case	FEDCPST_PR_BONUS:
		return 150;

	default:
	{
		STextureRefOther *tr = ENGINE.GetNumberedTexture(n);
		return SINT(tr->mH * 256.f);
	}
	
	};
}




//******************************************************************************************
void	CDCFEPrisonSelect::DrawSprite(SINT n, SINT px, SINT py, float otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey, GINT scale, SINT flip)
{
	// Bodge for now
	STextureRefOther *tr = ENGINE.GetNumberedTexture(n);

	float	x = float(px) * DCMULTX;
	float	y = float(py) * DCMULTY;

	SINT	sx = GetSpriteWidth(n);
	SINT	sy = GetSpriteHeight(n);

	ENGINE.DrawSprite(n, x, y, sx, sy, otdepth, r, g, b, semitrans, grey, scale, flip);
}

//******************************************************************************************
void	CDCFEPrisonSelect::DrawSpriteXY(SINT n, SINT px1, SINT py1, SINT px2, SINT py2, SINT px3, SINT py3, SINT px4, SINT py4, SINT otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey)
{
#ifdef JCLDCNOPSX			// Rewrite for DC
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
		//sds case  1: p4->tpage = tr->mTPAGE_P1P1;	break;
		//sds case  2: p4->tpage = tr->mTPAGE_P1M1;	break;
		//sds case  3: p4->tpage = tr->mTPAGE_P1P025;	break;
		default:
		case  0: p4->tpage = tr->mTPage;	break;
		};
	    //sds p4->clut  = tr->mCLUT;
	    
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
#endif
}

//******************************************************************************************
void	CDCFEPrisonSelect::DrawDynamicText(SINT fn, char *text, float x, float y, SINT r, SINT g, SINT b, SINT time, SINT otdepth)
{
	x *= DCMULTX;
	y *= DCMULTY;
	
	
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
//		DWORD col = (r << 16) + (g << 8) + b;
		
		switch (*text)
		{
		case ' ':
			x += 5.f * DCMULTX;
			break;

		default:
			// always drop shadow...
			     ENGINE.DrawChar(fn, *((unsigned char *)text) - 32, x + (1.f * DCMULTX), y + (1.f * DCMULTY), 0x001f0000, otdepth + 1);
			x += ENGINE.DrawChar(fn, *((unsigned char *)text) - 32, x, y, col, otdepth) + 2;
			break;
		};

		text++;
		time -= 3;
	};
}

//******************************************************************************************
void	CDCFEPrisonSelect::DrawBorder(SINT x, SINT y, SINT w, SINT h, SINT otdepth, SINT r, SINT b, SINT g)
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
	DrawSprite(FEDCPST_BORD_CTL, x - 6    , y - 6    , otdepth, col, col, col, alphatype);
	DrawSprite(FEDCPST_BORD_CTR, x - 6 + w, y - 6    , otdepth, col, col, col, alphatype);
	DrawSprite(FEDCPST_BORD_CBL, x - 6    , y - 6 + h, otdepth, col, col, col, alphatype);
	DrawSprite(FEDCPST_BORD_CBR, x - 6 + w, y - 6 + h, otdepth, col, col, col, alphatype);
	
	// edges
	SINT	c0;
	
	// top
	for (c0 = x + 6; c0 < x + w - 6; c0 += 11)
		DrawSprite(FEDCPST_BORD_PT, c0, y - 6, otdepth + 1, col, col, col, alphatype);

	// right
	for (c0 = y + 2; c0 < y + h - 6; c0 += 8)
		DrawSprite(FEDCPST_BORD_PR, x - 6 + w, c0, otdepth + 1, col, col, col, alphatype);
	
	// bottom
	for (c0 = x - 6 - 11 + w; c0 > x + 8 - 11; c0 -= 11)
		DrawSprite(FEDCPST_BORD_PB, c0, y + h - 6, otdepth + 2, col, col, col, alphatype);

	// left
	for (c0 = y - 6 - 8 + h; c0 > y + 6 - 12; c0 -= 8)
		DrawSprite(FEDCPST_BORD_PL, x - 6, c0, otdepth + 2, col, col, col, alphatype);	

/*	// top
	for (c0 = x + 8; c0 < x + w - 6; c0 += 14)
		DrawSprite(FEDCPST_BORD_PT, c0, y - 6, otdepth, col, col, col, alphatype);

	// right
	for (c0 = y + 6; c0 < y + h - 6; c0 += 12)
		DrawSprite(FEDCPST_BORD_PR, x - 6 + w, c0, otdepth, col, col, col, alphatype);
	
	// bottom
	for (c0 = x - 6 - 14 + w; c0 > x + 8 - 14; c0 -= 14)
		DrawSprite(FEDCPST_BORD_PB, c0, y + h - 6, otdepth, col, col, col, alphatype);

	// left
	for (c0 = y - 6 - 12 + h; c0 > y + 6 - 12; c0 -= 12)
		DrawSprite(FEDCPST_BORD_PL, x - 6, c0, otdepth, col, col, col, alphatype);	*/
}

//******************************************************************************************
void	CDCFEPrisonSelect::DrawBorderClip(SINT x, SINT y, SINT w, SINT otdepth, SINT r, SINT g, SINT b)
{
	DrawSprite(FEDCPST_LEVEL_CLIP_L, x - 60, y - 6, otdepth, r, g, b, 2);	
	DrawSprite(FEDCPST_LEVEL_CLIP_R, x  + w - 11, y - 6, otdepth, r, g, b, 2);

//	DrawSprite(FEDCPST_LEVEL_CLIP_L, x - 60, y - 6, otdepth, r, g, b);	
//	DrawSprite(FEDCPST_LEVEL_CLIP_R, x  + w - 11, y - 6, otdepth, r, g, b);	
//	DrawSprite(FEDCPST_LEVEL_CLIP_L, x - 60 + 3, y - 6 + 3, otdepth, r, g, b, 2);	
//	DrawSprite(FEDCPST_LEVEL_CLIP_R, x  + w - 11 + 3, y - 6 + 3, otdepth, r, g, b, 2);	
}

//******************************************************************************************
#define FAKE_PERSPECTIVE(x, val)  ((x) = ((G((x) - 256) * (val)).Whole() + 256))

void	CDCFEPrisonSelect::DrawBorderClipRot(SINT s1, SINT s2,SINT x, SINT y, SINT w, SINT otdepth, SINT r, SINT g, SINT b, GINT rot, BOOL grey)
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
	
	tr = ENGINE.GetNumberedTexture(FEDCPST_LEVEL_CLIP_L);
	
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
	
	DrawSpriteXY(FEDCPST_LEVEL_CLIP_L, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, 127, 127, 127, 2);
//	x1 += 3; x2 += 3; x3 += 3; x4 += 3;
//	y1 += 3; y2 += 3; y3 += 3; y4 += 3;
//	DrawSpriteXY(FEDCPST_LEVEL_CLIP_L, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, 127, 127, 127, 2);

	tr = ENGINE.GetNumberedTexture(FEDCPST_LEVEL_CLIP_R);
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
	
	DrawSpriteXY(FEDCPST_LEVEL_CLIP_R, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, 127, 127, 127, 2);
//	x1 += 3; x2 += 3; x3 += 3; x4 += 3;
//	y1 += 3; y2 += 3; y3 += 3; y4 += 3;
//	DrawSpriteXY(FEDCPST_LEVEL_CLIP_R, x1, y1, x2, y2, x3, y3, x4, y4, otdepth, 127, 127, 127, 2);

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
void	CDCFEPrisonSelect::DrawBox(SINT x, SINT y, SINT w, SINT h, SINT otdepth, SINT r, SINT b, SINT g)
{
	// draw the borders
	DrawBorder(x, y, w, h, otdepth, r, g, b);

	// and the box
	STextureRefOther 	*tr = ENGINE.GetNumberedTexture(FEDCPST_ALPHAWHITE);
	SINT	TN = tr->mTPage + 1;

	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
	
	InitCZTs(TN);

	float	fx, fy, fw, fh;

	fx = float(x) * DCMULTX;
	fy = float(y) * DCMULTY;
	fw = float(w) * DCMULTX;
	fh = float(h) * DCMULTY;

	float	us = tr->mX + 0.002f;
	float	vs = tr->mY + 0.002f;
	float	ue = tr->mX + tr->mW - 0.002f;
	float	ve = tr->mY + tr->mH - 0.002f;
	
	float	zdepth = 10.f - (float(otdepth + 3) / 102.4);

	FVector	verts[2][2];
	
	verts[0][0] = FVector(fx     , fy     , zdepth);
	verts[0][1] = FVector(fx + fw, fy     , zdepth);
	verts[1][0] = FVector(fx     , fy + fh, zdepth);
	verts[1][1] = FVector(fx + fw, fy + fh, zdepth);
	
	SINT	col = 0xff000000;
		
   	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[0].fX                   = verts[0][0].X;
   	TextureS[0].fY                   = verts[0][0].Y;
   	TextureS[0].u.fZ                 = verts[0][0].Z;
   	TextureS[0].uBaseRGB.dwPacked    = col;
   	TextureS[0].fU                   = us;
   	TextureS[0].fV                   = vs;

   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].fX                   = verts[0][1].X;
   	TextureS[1].fY                   = verts[0][1].Y;
   	TextureS[1].u.fZ                 = verts[0][1].Z;
   	TextureS[1].uBaseRGB.dwPacked    = col;
   	TextureS[1].fU                   = ue;
   	TextureS[1].fV                   = vs;

   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].fX                   = verts[1][0].X;
   	TextureS[2].fY                   = verts[1][0].Y;
   	TextureS[2].u.fZ                 = verts[1][0].Z;
   	TextureS[2].uBaseRGB.dwPacked    = col;
   	TextureS[2].fU                   = us;
   	TextureS[2].fV                   = ve;

   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	TextureS[3].fX                   = verts[1][1].X;
   	TextureS[3].fY                   = verts[1][1].Y;
   	TextureS[3].u.fZ                 = verts[1][1].Z;
   	TextureS[3].uBaseRGB.dwPacked    = col;
   	TextureS[3].fU                   = ue;
   	TextureS[3].fV                   = ve;
	
	ExitCZTs(4);
}

#define	LINE_WIDTH 1.f

//******************************************************************************************
void	CDCFEPrisonSelect::DrawLine(SINT sx, SINT sy, SINT ex, SINT ey, SINT otdepth, SINT r, SINT g, SINT b)
{
#ifdef JCLDCNOPSX			// Rewrite for DC  (not sure if this is used in the frontend anymore?)
	NTAG				*o  = PLATFORM.GetOrderTable();
	LINE_F2				*f2 = (LINE_F2 *)PLATFORM.GetMiscPolyG4s();

	setLineF2(f2);
	setXY2(f2, sx, sy, ex, ey);
	setRGB0(f2, r, g, b);
	addPrim(&o[otdepth], f2);
	f2 ++;
	
	PLATFORM.DeclareUsedMiscPolyG4s((POLY_G4 *)f2);
#endif

	// JCL this is probably the slowest line draw ever written

	SINT c0, c1, c2;
	
	if (r > 127) r = 127;
	if (g > 127) g = 127;
	if (b > 127) b = 127;
	
	DWORD col = (r << 16) + (g << 8) + (b << 0);

	float	zdepth = 10.f - (float(otdepth) / 102.4);  //!!  check this

	FVector	s = FVector(float(sx) * DCMULTX, float(sy) * DCMULTY, zdepth);
	FVector	e = FVector(float(ex) * DCMULTX, float(ey) * DCMULTY, zdepth);

	// work out how many segments we need
	SINT	num_segs = 1;
	FVector	seg_offset = (e - s);
	FVector perp = seg_offset ^ FVector(0, 0, 1.f);

	if (perp.Magnitude() > 0.0015)
	{
		perp.Normalise();
		perp *= LINE_WIDTH / 2.f;
	}
	else
		perp = FVector(0, 0, LINE_WIDTH / 2.f);

	FVector	curr_pos = s;

	kmChangeStripListType			( &GouraudHead, KM_TRANS_POLYGON );    
	kmChangeStripBlendingMode       ( &GouraudHead, KM_IMAGE_PARAM1, KM_ONE, KM_ZERO);
	kmChangeStripTextureShadingMode ( &GouraudHead, KM_IMAGE_PARAM1, KM_MODULATE);	
	kmChangeStripUseAlpha			( &GouraudHead, KM_IMAGE_PARAM1, KM_FALSE );

	InitCZs();

	FVector	verts[2][2];
	
	verts[0][0] = curr_pos - perp;
	verts[0][1] = curr_pos + perp;
	
	curr_pos = e;
		
	verts[1][0] = curr_pos - perp;
	verts[1][1] = curr_pos + perp;
	
   	GouraudS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	GouraudS[0].fX                   = verts[0][0].X;
   	GouraudS[0].fY                   = verts[0][0].Y;
   	GouraudS[0].u.fZ                 = verts[0][0].Z;
   	GouraudS[0].uBaseRGB.dwPacked    = col;

   	GouraudS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	GouraudS[1].fX                   = verts[0][1].X;
   	GouraudS[1].fY                   = verts[0][1].Y;
   	GouraudS[1].u.fZ                 = verts[0][1].Z;
   	GouraudS[1].uBaseRGB.dwPacked    = col;

   	GouraudS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	GouraudS[2].fX                   = verts[1][0].X;
   	GouraudS[2].fY                   = verts[1][0].Y;
   	GouraudS[2].u.fZ                 = verts[1][0].Z;
   	GouraudS[2].uBaseRGB.dwPacked    = col;

   	GouraudS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	GouraudS[3].fX                   = verts[1][1].X;
   	GouraudS[3].fY                   = verts[1][1].Y;
   	GouraudS[3].u.fZ                 = verts[1][1].Z;
   	GouraudS[3].uBaseRGB.dwPacked    = col;

	ExitCZs(4);
}

#endif