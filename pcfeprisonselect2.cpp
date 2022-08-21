#include	"Common.h"

#if TARGET == PC

#include	"PCFEPrisonSelect.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"FrontEndCommon.h"
#include	"Text.h"
#include	"LevelInfo.h"
//#include	"PSXMemCard.h"
#include	"Career.h"
#include	"Globals.h"


// *** This file is a continuation of PSXFEPrisonSelect, and contains functions
//     I don't want inlined...

void	sort_out_colour(DWORD &col)
{
	SINT	r = (col >> 16) & 0xff;
	SINT	g = (col >>  8) & 0xff;
	SINT	b = (col      ) & 0xff;
	
	if (r > 127) r = 127;
	if (g > 127) g = 127;
	if (b > 127) b = 127;
	
	col = (0xff << 24) + ((r & 0x7f) << 17) + ((g & 0x7f) << 9) + ((b & 0x7f) << 1);
}

void	sort_out_colour2(DWORD &col)
{
	SINT	r = (col      ) & 0xff;
	SINT	g = (col >>  8) & 0xff;
	SINT	b = (col >> 16) & 0xff;
	
	if (r > 127) r = 127;
	if (g > 127) g = 127;
	if (b > 127) b = 127;
	
	col = (0xff << 24) + ((r & 0x7f) << 17) + ((g & 0x7f) << 9) + ((b & 0x7f) << 1);
}

//******************************************************************************************
void	CPCFEPrisonSelect::AddParticles(SINT x, SINT y, SINT num, SINT type)
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
void	CPCFEPrisonSelect::DrawSpritePC(CTextureTL* n, float px, float py, float z, float width, float height, D3DCOLOR col, BOOL grey, float scalex, float scaley, SINT flip)
{
	// use the one in PCEnginePanel.cpp
	ENGINE.DrawSpritePC(n, px, py, z, width / scalex, height / scaley, col, scalex, scaley, flip);

/*	SINT	i[4];
	
	ULONG	spec = 0;
	float u1,u2,v1,v2;
	float x,y,w,h;

	x = px * scalex;
	y = py * scaley;

//	w = width * scalex;
//	h = height * scaley;
	w = width;
	h = height;

//	sort_out_colour(col);

	if(grey)
			;// no idea yet
	
	// Quit if the texture is not valid.
	if (!n) {return;}
	
	switch(flip)
	{
		case 0:
			u1 = 0.005f;
			u2 = (float)width / (float)(n->texw+1.0f);
			v1 = 0.995f;
			v2 = (float)(n->texh-height+1) / (float)n->texh;
			break;
		case 1:
			u1 = (float)width / (float)(n->texw+1.0f);
			u2 = 0.005f;
			v1 = 0.995f;
			v2 = (float)(n->texh-height+1) / (float)n->texh;
			break;
		case 2:
			u1 = 0.005f;
			u2 = (float)width / (float)(n->texw+1.0f);
			v1 = (float)(n->texh-height+1) / (float)n->texh;
			v2 = 0.995f;
			break;
		case 3:
			u1 = (float)width / (float)(n->texw+1.0f);
			u2 = 0.005f;
			v1 = (float)(n->texh-height+1) / (float)n->texh;
			v2 = 0.995f;
			break;
	}
	
	i[0] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x    , y    , z), 1.0f, col, spec, u1, v1));
	i[1] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y    , z), 1.0f, col, spec, u2, v1));
	i[2] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y + h, z), 1.0f, col, spec, u2, v2));
	i[3] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x    , y + h, z), 1.0f, col, spec, u1, v2));
	
	n->AddTri(i[0], i[1], i[3]);
	n->AddTri(i[1], i[2], i[3]);*/
}

//******************************************************************************************
void	CPCFEPrisonSelect::DrawSpritePCAdditive(CTextureTLAdditive* n, float px, float py, float z, float width, float height, D3DCOLOR col, BOOL grey, float scalex, float scaley, SINT flip)
{
	SINT	i[4];
	
	ULONG	spec = 0;
	float u1,u2,v1,v2;
	float x,y,w,h;

	x = px * scalex;
	y = py * scaley;

//	w = width * scalex;
//	h = height * scaley;
	w = width;
	h = height;

//	sort_out_colour(col);

	if(grey)
			;// no idea yet
	
	// Quit if the texture is not valid.
	if (!n) {return;}
	
	switch(flip)
	{
		case 0:
			u1 = 0.005f;
			u2 = (float)width / (float)(n->texw+1.0f);
			v1 = 0.995f;
			v2 = (float)(n->texh-height+1) / (float)n->texh;
			break;
		case 1:
			u1 = (float)width / (float)(n->texw+1.0f);
			u2 = 0.005f;
			v1 = 0.995f;
			v2 = (float)(n->texh-height+1) / (float)n->texh;
			break;
		case 2:
			u1 = 0.005f;
			u2 = (float)width / (float)(n->texw+1.0f);
			v1 = (float)(n->texh-height+1) / (float)n->texh;
			v2 = 0.995f;
			break;
		case 3:
			u1 = (float)width / (float)(n->texw+1.0f);
			u2 = 0.005f;
			v1 = (float)(n->texh-height+1) / (float)n->texh;
			v2 = 0.995f;
			break;
	}
	
	i[0] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x    , y    , z), 1.0f, col, spec, u1, v1));
	i[1] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y    , z), 1.0f, col, spec, u2, v1));
	i[2] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y + h, z), 1.0f, col, spec, u2, v2));
	i[3] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x    , y + h, z), 1.0f, col, spec, u1, v2));
	
	n->AddTri(i[0], i[1], i[3]);
	n->AddTri(i[1], i[2], i[3]);
}

//******************************************************************************************
void	CPCFEPrisonSelect::DrawSpriteXY(SINT n, SINT px1, SINT py1, SINT px2, SINT py2, SINT px3, SINT py3, SINT px4, SINT py4, SINT otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey)
{
#if 0
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
#endif
}

//******************************************************************************************
void	CPCFEPrisonSelect::DrawDynamicText(SINT fn, char *text, float x, float y, SINT r, SINT g, SINT b, SINT time, float z)
{
	if ((r | b | g) != 0)
		// auto shadow
		DrawDynamicText(fn, text, x + 1, y + 1, 0, 0, 0, time, z + 0.03f);

	while (*text)
	{
		if (time <= 0)
			break;
			
		if ((r | b | g) != 0)	// are: Never fade if black. - Hack to stopping fading of shadow.
		{
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
			if(r>127)
				r=127;
			if(g>127)
				g=127;
			if(b>127)
				b=127;
		}
	
		DWORD col = RGB_MAKE(r*2,g*2,b*2);
		
		switch (*text)
		{
		case ' ':
			x += 5;
			break;

		default:
			x += ENGINE.DrawChar(fn, *((unsigned char *)text) - 32, x, y, col, z) + 2;
			break;
		};

		text++;
		time -= 3;
	};
}

//******************************************************************************************
void	CPCFEPrisonSelect::DrawBorder(float x, float y, float w, float h, float z, SINT r, SINT b, SINT g)
{	
	// cue the colour from the incoming r value.
	
	SINT	alphatype;
	DWORD	col;
	
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

	if(col>127)
		col = 127;
	col = RGB_MAKE(col*2,col*2,col*2);
	
	// corners
	DrawSpritePC(framctl, x-4,    y-4, z, 16, 16, col);
	DrawSpritePC(framctr, x-4+w,  y-4, z, 16, 16, col);
	DrawSpritePC(framcbl, x-4,    y-4+h, z, 16, 16, col);
	DrawSpritePC(framcbr, x-4+w,  y-4+h, z, 16, 16, col);
	
	// edges
	SINT	c0;
	
	// top
	for (c0 = x + 4; c0 < x + w - 4; c0 += 12)
		DrawSpritePC(frampt, c0,  y-4, z, 16, 16, col);

	// right
	for (c0 = y + 4; c0 < y + h - 4; c0 += 8)
		DrawSpritePC(frampr, x - 4 + w,  c0, z, 16, 16, col);
	
	// bottom
	for (c0 = x - 4 - 12 + w; c0 > x + 8 - 12; c0 -= 12)
		DrawSpritePC(frampb, c0,  y + h - 4, z, 16, 16, col);

	// left
	for (c0 = y - 4 - 8 + h; c0 > y + 4 - 8; c0 -= 8)
		DrawSpritePC(frampl, x - 4,  c0, z, 16, 16, col);
}

//******************************************************************************************
void	CPCFEPrisonSelect::DrawBorderClip(float x, float y, float w, float z, SINT r, SINT g, SINT b)
{
	if(r > 127)
		r = 127;
	if(g > 127)
		g = 127;
	if(b > 127)
		b = 127;

	DWORD col = RGB_MAKE(r * 2,g * 2,b * 2);
	DrawSpritePC(clipl, x - 60,  y - 6, z, 94, 174, col);
	DrawSpritePC(clipr, x  + w - 11,  y - 6, z, 94, 174, col);
}

//******************************************************************************************
#define FAKE_PERSPECTIVE(x, val)  ((x) = ((G((x) - 256) * (val)).Whole() + 256))

void	CPCFEPrisonSelect::DrawBorderClipRot(SINT s1, SINT s2,float x, float y, float w, SINT otdepth, SINT r, SINT g, SINT b, GINT rot, BOOL grey)
{
	GINT	rot2 = rot;

	if (rot2 > GINT_HALF)
		rot2 = G1 - rot2;
#if 0
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
#endif
}

//******************************************************************************************
void	CPCFEPrisonSelect::DrawBox(float x, float y, float w, float h, float z, SINT r, SINT b, SINT g)
{
	// draw the borders
	DrawBorder(x, y, w, h, z, r, g, b);

	// and the box
	SINT	i[4];

	DWORD	col = 0xff000000;
	DWORD	spec = 0;
	float	u1 = 0;
	float	v1 = 0;
	float	u2 = 1.f;
	float	v2 = 1.f;

	x *= PCMULTX;
	w *= PCMULTX;
	y *= PCMULTY;
	h *= PCMULTY;

	i[0] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(x    , y    , z + 0.001f), 1.0f, col, spec, u1, v1));
	i[1] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y    , z + 0.001f), 1.0f, col, spec, u2, v1));
	i[2] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y + h, z + 0.001f), 1.0f, col, spec, u2, v2));
	i[3] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(x    , y + h, z + 0.001f), 1.0f, col, spec, u1, v2));
	
	white->AddTri(i[0], i[1], i[3]);
	white->AddTri(i[1], i[2], i[3]);

#if 0
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
#endif
}

//******************************************************************************************
void	CPCFEPrisonSelect::DrawLine(float sx, float sy, float ex, float ey, SINT otdepth, SINT r, SINT g, SINT b)
{
	sx *= PCMULTX;
	ex *= PCMULTX;
	sy *= PCMULTY;
	ey *= PCMULTY;

	float	z		= ((float)otdepth) / 255.0f;
	DWORD	col		= RGB_MAKE(r, g, b);
	SINT	i[4];

	i[0] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(sx,			sy,			z), 1.0f, col, 0x00000000, 0.0f, 0.0f));
	i[1] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(sx + 1.0f,	sy + 1.0f,	z), 1.0f, col, 0x00000000, 1.0f, 0.0f));
	i[2] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(ex,			ey,			z), 1.0f, col, 0x00000000, 1.0f, 1.0f));
	i[3] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(ex + 1.0f,	ey + 1.0f,	z), 1.0f, col, 0x00000000, 0.0f, 1.0f));

	white->AddTri(i[0], i[2], i[1]);
	white->AddTri(i[1], i[2], i[3]);

	white->AddTri(i[0], i[1], i[2]);
	white->AddTri(i[1], i[3], i[2]);
}

//******************************************************************************************
void	CPCFEPrisonSelect::DrawRect(float sx, float sy, float ex, float ey, SINT otdepth, SINT r, SINT g, SINT b)
{
	sx *= PCMULTX;
	ex *= PCMULTX;
	sy *= PCMULTY;
	ey *= PCMULTY;

	float	z		= ((float)otdepth) / 255.0f;
	DWORD	col		= RGB_MAKE(r, g, b);
	SINT	i[4];

	i[0] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(sx,	sy,		z), 1.0f, col, 0x00000000, 0.0f, 0.0f));
	i[1] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(ex,	sy,		z), 1.0f, col, 0x00000000, 1.0f, 0.0f));
	i[2] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(ex,	ey,		z), 1.0f, col, 0x00000000, 1.0f, 1.0f));
	i[3] = white->AddVertex(&D3DTLVERTEX(D3DVECTOR(sx,	ey,		z), 1.0f, col, 0x00000000, 0.0f, 1.0f));

	white->AddTri(i[0], i[1], i[3]);
	white->AddTri(i[1], i[2], i[3]);
}

#endif