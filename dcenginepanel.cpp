#include 	"Common.h"

#if TARGET == DC

//#pragma optimization_level 0

#include	"Globals.h"
#include	"Profile.h"
#include	"Primary.h" /// aaaaaaargh
#include	"Text.h"
#include	"CharacterLevelData.h"
#include	"Pickup.h"
#include	"LevelInfo.h"
#include	"Career.h"
#include 	"MainCode.h"
#include	"DCJunk.h"
#include	"FrontEndCommon.h"

// This file is essentially a continuation of PSXEngine.cpp, and contains CPSXEngine methods
// defined in PSXEngine.h

#define		MENUTEXTOFFSET	14

//******************************************************************************************
#define		CLOCK_W			146
#define		CLOCK_H			44
#define		STARTL_W		64
#define		STARTL_H		64
#define		ARM_W			64
#define		ARM_H			118
#define		BAR_W			64
#define		BAR_H			64
#define		PANEL_W			128
#define		PANEL_H			64
#define 	LEVELTYPE_W		46
#define 	LEVELTYPE_H		64
#define		HEALTH_W		128		
#define		HEALTH_H		128

//******************************************************************************************
void	swapf(float &a, float &b)
{
	float foo = a;
	a = b;
	b = foo;
}

//******************************************************************************************
DWORD	buildFadeARGB(SINT v)
{
//	return 0xff000000 + (v << 16) + (v << 8) + v;
//	return (v << 24) + (v << 16) + (v << 8) + v;
	return (v << 24);
}

//******************************************************************************************
BOOL	auto_shadow(SINT n)
{
	// do we try to shadow this?
	if (GAME.mInFrontEnd)
	{
		switch (n)
		{
		case FEDCPST_LCD1:
		case FEDCPST_LCD2:
		case FEDCPST_LCD3:
		case FEDCPST_LCD4:
		case FEDCPST_LCD5:
		case FEDCPST_CHARGRIDB:
		case FEDCPST_BOXL:
		case FEDCPST_BOXM:
		case FEDCPST_BOXR:
		case FEDCPST_PR_ECO:
		case FEDCPST_PR_ZEPPELIN:
		case FEDCPST_PR_HOSPITAL:
		case FEDCPST_PR_SHIP:
		case FEDCPST_PR_REHAB:
		case FEDCPST_PR_MINING:
		case FEDCPST_PR_DEATHROW:
		case FEDCPST_PR_SPORTS:
		case FEDCPST_PR_MILITARY:
		case FEDCPST_PR_ALPHASTAR:
		case FEDCPST_PR_BONUS:
		case FEDCPST_LT_KING_OF_THE_HILL:
		case FEDCPST_LT_LAST_MAN_ROLLING:
		case FEDCPST_LT_RUN_THE_GAUNTLET:
		case FEDCPST_LT_PURSUIT:
		case FEDCPST_LT_RACE:
		case FEDCPST_LT_TRICK:
		case FEDCPST_LT_POWERBALL:
		case FEDCPST_LT_GOLF:
		case FEDCPST_LT_PINBALL:
		case FEDCPST_LT_SBALL:
		case FEDCPST_LT_SHADOW:
		case FEDCPST_QUALIFY:
		case FEDCPST_GOLD:
		case FEDCPST_LEVEL_CLIP_L:
		case FEDCPST_LEVEL_CLIP_R:
		case FEDCPST_ARROW_L:
		case FEDCPST_ARROW_R:
		case FEDCPST_ARROW_U:
		case FEDCPST_ARROW_D:
		case FEDCPST_DRAW:
		case FEDCPST_LOST_TIME:
			return TRUE;	

		default:
			return FALSE;	
		};
	}
	else
	{
		switch (n)
		{
		case TN_HEALTH0:
		case TN_START1:
		case TN_START2:
		case TN_START3:
		case TN_START_ARM:
		case TN_PANEL:
		case TN_BOXL:
		case TN_BOXM:
		case TN_BOXR:
		case TN_LEVELTYPE:
		case TN_CLOCK:
		case TN_ARROW_L:
		case TN_ARROW_R:
		case TN_ARROW_U:
		case TN_ARROW_D:
			return TRUE;
		
		default:
			return FALSE;
		};
	}
}

//******************************************************************************************
void	CDCEngine::DrawSprite(SINT n, float px, float py, SINT sx, SINT sy, float otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey, GINT scale, SINT flip)
{
	// autoshadow????

	// swap rgb
/*	SINT	foo = r;
	r = b;
	b = foo;*/
	
	// auto shadow?
	if ((r != 0) || (g != 0) || (b != 0))
	{
		if (auto_shadow(n))
		{
			// draw again!
			if (GAME.mInFrontEnd)  // shadow more in frontend
				DrawSprite(n, px + 4, py + 4, sx, sy, otdepth + 0.5f, 0, 0, 0, semitrans, grey, scale, flip);
			else
				DrawSprite(n, px + 2, py + 2, sx, sy, otdepth + 0.5f, 0, 0, 0, semitrans, grey, scale, flip);
		}
	}

	STextureRefOther 	*tr = ENGINE.GetNumberedTexture(n);

	SINT	TN = tr->mTPage + 1;

	// No Auto shadows
	if (semitrans == 2)
		semitrans = -1;

	if (semitrans != -1)
	{
		kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
	 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
	}
	else
	{
		kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
	 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
	}
	
	InitCZTs(TN);

	float	x, y, w, h;

//	x = px * DCMULTX;
//	y = py * DCMULTY;
	x = px;
	y = py;
	w = sx;
	h = sy;

	if(grey)
			;// no idea yet
			
			
	float	us = tr->mX + 0.002f;
	float	vs = tr->mY + 0.002f;
	float	ue = tr->mX + (float(w) / float(256)) - 0.002f;  //!! Assume 256 texture page
	float	ve = tr->mY + (float(h) / float(256)) - 0.002f;  //!! Assume 256 texture page
	
	switch(flip)
	{
		case 0:
			break;
		case 1:
			swapf(us, ue);
			break;
		case 2:
			swapf(vs, ve);
			break;
		case 3:
			swapf(us, ue);
			swapf(vs, ve);
			break;
	}
	
	w *= GToF(scale);
	h *= GToF(scale);

	float	zdepth = 10.f - (float(otdepth) / 102.4);  //!!  check this

	FVector	verts[2][2];
	
	verts[0][0] = FVector(x    , y    , zdepth);
	verts[0][1] = FVector(x + w, y    , zdepth);
	verts[1][0] = FVector(x    , y + h, zdepth);
	verts[1][1] = FVector(x + w, y + h, zdepth);
	
	if (r > 127) r = 127;
	if (g > 127) g = 127;
	if (b > 127) b = 127;
	
	SINT	col = (0xff << 24) + ((r & 0x7f) << 17) + ((g & 0x7f) << 9) + ((b & 0x7f) << 1);
		
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
	
	if (semitrans != -1)
	{
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 		kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
 	}
}

//******************************************************************************************
void	CDCEngine::DrawBar(float x, float y, SINT num_bits, SINT colr, SINT colg, SINT colb, BOOL centre, float ot)
{
	if (centre)
	{
		SINT	size = BAR_W * (num_bits + 2);
		x -= size / 2;
	}

	SINT 	c1;
	DrawSprite(TN_BOXL, x    , y    , BAR_W, BAR_H, ot, colr, colg, colb);
	x += BAR_W;

	for (c1 = 0; c1 < num_bits; c1 ++)
	{
		DrawSprite(TN_BOXM, x    , y    , BAR_W, BAR_H, ot, colr, colg, colb);
		x += BAR_W;
	}
	DrawSprite(TN_BOXR, x    , y    , BAR_W, BAR_H, ot, colr, colg, colb);
}

//******************************************************************************************
UINT	eval_char(UINT c)
{
	if (c > 95)
	{
		switch (c + 32)
		{
			case 224:	c = 96; break;
			case 225:	c = 97; break;
			case 228:	c = 98; break;
			case 233:	c = 99; break;
			case 232:	c = 100; break;
			case 234:	c = 101; break;
			case 211:	c = 102; break;
			case 243:	c = 103; break;
			case 244:	c = 104; break;
			case 246:	c = 105; break;
			case 237:	c = 106; break;
			case 236:	c = 107; break;
			case 218:	c = 108; break;
			case 220:	c = 109; break;
			case 249:	c = 110; break;
			case 250:	c = 111; break;
			case 252:	c = 112; break;
			case 223:	c = 113; break;
			case 171:	c = 114; break;
			case 187:	c = 115; break;
			case 161:	c = 116; break;
			case 191:	c = 117; break;
			default:
			{
//				printf("Unknown character: %d\n", c);
				c = '!'; break;
			}
		};
	}

	return c;
}

//******************************************************************************************
SINT	CDCEngine::DrawChar(SINT font, UINT c, float x, float y, DWORD col, float otdepth)
{
	font = 2; //!!!JCL

	// Sort the colour out
	SINT	r = (col >> 16) & 0xff;
	SINT	g = (col >>  8) & 0xff;
	SINT	b = (col      ) & 0xff;
	
	if (r > 127)	r = 127;
	if (g > 127)	g = 127;
	if (b > 127)	b = 127;
	
	col = 0xff000000 + (b << 17) + (g << 9) + (r << 1); // note, swap RGB
//	col = 0xff000000 + (r << 17) + (g << 9) + (b << 1); // note, swap RGB??????
		
	// Push Y down...
//	y += 7;
//	y += 16;

	c = eval_char(c);

	SDCFont	*f  = &mFonts[font];
	STCFont		*fi = &f->f;  // nice clear code!

	SINT	TN = f->mTPage + 1;
	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );

	if (x >= 0)	// sorry - had to do this.
	{
		SINT	cw = fi->mCharWidth;
		SINT	ch = fi->mCharHeight;

		SINT	row    = c / fi->mCharsPerLine;
		SINT	column = c - row * fi->mCharsPerLine;
		SINT	bx = (column * cw);
		SINT	by = (row    * ch);
		
		//!  Note - assuming 256x256 texture page here...
		float 	us = f->mX + (float(bx) / 256.f) + 0.002f;
		float 	vs = f->mY + (float(by) / 256.f) + 0.002f;
		float	ue = us + float(cw) / 256.f - 0.002f;
		float	ve = vs + float(cw) / 256.f - 0.002f;

		InitCZTs(TN);
	
		float	fx, fy, fw, fh;
	
//		fx = float(x) * DCMULTX;
//		fy = float(y) * DCMULTY;
		fx = float(x);
		fy = float(y);
		fw = float(cw);
		fh = float(ch);
				
		float	zdepth = 10.f - (float(otdepth) / 102.4);  //!!  check this
	
		FVector	verts[2][2];
		
		verts[0][0] = FVector(fx     , fy     , zdepth);
		verts[0][1] = FVector(fx + fw, fy     , zdepth);
		verts[1][0] = FVector(fx     , fy + fh, zdepth);
		verts[1][1] = FVector(fx + fw, fy + fh, zdepth);
		
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

	return fi->mWidths[c];
}

#pragma dont_inline on
//******************************************************************************************
void	CDCEngine::DrawString(SINT font, char *s, float x, float y, DWORD col, float otdepth,BOOL HasBorder)
{
	// shadow hack
	if ((col == 0) || (col == 0x001f0000))
	{
		otdepth += 0.3f;
	}

	char *c=s;
	float xx=x;

	while (*c)
	{
		switch (*c)
		{
		case ' ':
			xx += 5.f;
			break;

		default:
			xx += DrawChar(font, *((unsigned char *)c) - 32, xx, y, col, otdepth) + 2;
			break;
		};

		c++;
	};

	if(HasBorder)
	{
		DrawString(font, s, x+1, y+1, 0 ,otdepth,FALSE);
		DrawString(font, s, x-1, y+1, 0 ,otdepth,FALSE);
		DrawString(font, s, x-1, y-1, 0 ,otdepth,FALSE);
		DrawString(font, s, x+1, y-1, 0 ,otdepth,FALSE);
	}
}
#pragma dont_inline off

//******************************************************************************************
SINT	CDCEngine::GetStringSize(SINT font, char *s)
{
	font = 2; //!!!JCL

	SINT 		l = 0;
	SDCFont	*f  = &mFonts[font];
	STCFont		*fi = &f->f;  // nice clear code!

	while (*s)
	{
		switch (*s)
		{
		case ' ':
			l += 5;
			break;

		default:
			l += fi->mWidths[eval_char(*((unsigned char *)s) - 32)] + 2;
			break;
		};

		s++;
	};

	return l;
}

//******************************************************************************************
SINT	get_panel_y_offset()
{
	SINT 	v = GAME.GetGameTimeSinceStart();
	SINT 	yo = -1;

	if (v > 20)
	{
		if (v > 50)
			yo = 0;
		else
		{
			yo = (50 - v);
			yo *= yo;
			yo >>= 2;
		}
	}

	return yo;
}

//******************************************************************************************
void	CDCEngine::RenderPlayerState(CPlayer* for_player, SINT left_pixels_offset, SINT screen_width)
{
	DWORD	text_col = 0x0000ffff;
	char 	text[200], text2[200];
	SINT 	sl;

	BOOL	right = (left_pixels_offset) || (screen_width == PLATFORM.ScreenWidthFull());

	// render panels and fades.
	CGameRenderInfo *gri = GAME.GetGameRenderInfo();

	CPrimary *pr = for_player->GetPrimary();
	if (!pr)
		return;

	CCharacterLevelData* cld = &pr->GetLevelData() ;

	// Golf shot no
	if ((gri->mActive & GRI_GOLF_SHOT_NO) && (for_player->GetLevelResult() != LR_WIN))
	{
		SINT 	yo = (GABS(gri->mGolfShotPos) * 150).Whole();
		SINT 	num_segs;
		float	x;
		float	y;
		SINT	font;

	 	num_segs = 2;
		x = 380 * DCMULTX;
		font = 2;
		y = 205 * DCMULTY;

		char ts[100];
		CText::GetString(TEXT_SHOT, ts);
		sprintf(text, "%s: %d", ts, gri->mGolfShotNo);

		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2)                  , y + float(yo + 4) * DCMULTY + MENUTEXTOFFSET, text_col, 2.f);
		DrawString(font, text, x - (sl / 2) + (1.f * DCMULTX), y + float(yo + 5) * DCMULTY + MENUTEXTOFFSET, 0, 2.f);

		ENGINE.DrawBar(x, y + float(yo) * DCMULTY, num_segs, 127, 127, 127, TRUE, 2.5f);

	}

	// Golf OB
	if ((gri->mActive & GRI_GOLF_OB) && (for_player->GetLevelResult() != LR_WIN))
	{
		SINT 	yo = (GABS(gri->mGolfOBPos) * 150).Whole();
		SINT 	num_segs;
		float	x;
		float	y;
		SINT	font;

	 	num_segs = 2;
		x = 115 * DCMULTX;
		font = 0;
		y = 205 * DCMULTY;

		CText::GetString(TEXT_OB, text);
		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2)                  , y + float(yo + 4) * DCMULTY + MENUTEXTOFFSET, text_col, 2.f);
		DrawString(font, text, x - (sl / 2) + (1.f * DCMULTX), y + float(yo + 5) * DCMULTY + MENUTEXTOFFSET, 0, 2.f);

		ENGINE.DrawBar(x, y + float(yo) * DCMULTY, num_segs, 127, 127, 127, TRUE, 2.5f);
	}

	if (gri->mActive & GRI_LIFE)
	{
		SINT	c0;

		GINT 	life  = G(pr->GetLife()) / 100;
		if (life < G0)
				life = G0;
		if (life > G1)
				life = G1;

		GINT	brake = pr->GetBrakePower();
		GINT	rage  = pr->GetRage();

		// draw bits
		for (c0 = 0; c0 < 4; c0 ++)
		{
			SINT	tn;
			GINT	val = G0;

			SINT	r = 127;
			SINT	g = 127;
			SINT	b = 127;

			switch (c0)
			{
			case 0:
				tn = TN_HEALTH0;
				val = G1;

				// colour?
				r = 80;
				g = 80;
				b = 80;

				// base colour
				if (brake <= G0)
				{
				 	b = 127;
				}
				if (life <= G0) // takes precedence
				{
					r = 127;
				}

				// now pulse if rage or life low...
				GINT	th = G(GAME.GetGameTurn()) * G(0, 30000);
				th.mVal = (th.mVal % ((G_2PI).mVal)); // slightly better mod...?

				if ((life > G0) && (life <= G(0, 13108)))
				{
					if ((th >= G_PI_2) && (th < G_PI))
					{
						r = r + (GSIN(th) * G(47)).Whole();

					}
				}

				th -= G_PI; // shift phase
				if ((brake > G0) && (brake <= G(0, 13108)))
				{
					if ((th >= G_PI_2) && (th < G_PI))
					{
						b = b + (GSIN(th) * G(47)).Whole();

					}
				}

				 break;
			case 1:
				tn = TN_HEALTH3;
				val = rage;
				if ((pr->GetRage() == G1) ||
					(pr->GetRenderAnimation() == AM_RAGE_ATTACK))
				{
					// flash
					r = ((GSIN(G(GAME.GetGameTurn()) * G(0, 30000)) + G1) * G(47)).Whole() + 80;
					g = r;
					b = r;
				}
				break;
			case 2: tn = TN_HEALTH2; val = brake;		break;
			case 3: tn = TN_HEALTH1; val = life;		break;
			};

			STextureRefOther 	*tr = ENGINE.GetNumberedTexture(tn);

			float	tx = tr->mX + 0.002f;
			float	ty = tr->mY + 0.002f;
			float	tw = tr->mW - 0.004f;
			float	th = tr->mH - 0.004f;

			float	x;
			if (right)
				x = screen_width - HEALTH_W - (21 * DCMULTX) + left_pixels_offset;
			else
				x = (20 * DCMULTX);
			float	y = PLATFORM.ScreenHeight() - HEALTH_H - (4 * DCMULTY);

			float	fval = GToF(val);

			if (val != G1)
			{
				tw  = (tr->mW * fval * 0.5f) + (5.f / 256.f);
				if (right)
				{
					x  += (float(HEALTH_W) * (1.f - fval) * 0.5f) + 5.f;
					tx += (tr->mW * (1.f - fval) * 0.5f) + (5.f / 256.f);
				}
				else
				{
					x  += (float(HEALTH_W) * (1.f      ) * 0.5f) - 8.f - 3.f;
					tx += (tr->mW * (1.f - fval) * 0.5f /*+ 0.5f*/) + (5.f / 256.f);   // hackity - hack!
				}
			}

			float	w = tw * 256.f;		//!! Assume 256x256 page
			float	h = th * 256.f;

/*			// swap rgb
			SINT	foo = r;
			r = b;
			b = foo;*/
		
		
			SINT	TN = tr->mTPage + 1;

			kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
		    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
		 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
		 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
		
			InitCZTs(TN);
		
			float	us, ue, vs, ve;
			
			if (right)
			{
				us = tx;
				ue = tx + tw;
			}
			else
			{
				us = tx + tw;
				ue = tx;
			}
			vs = ty;
			ve = ty + th;
		
			float	zdepth = 10.f - (float(5 - float(c0) / 2.f) / 102.4f);
		
			FVector	verts[2][2];
			
			verts[0][0] = FVector(x             , y             , zdepth);
			verts[0][1] = FVector(x + w + 0.004f, y             , zdepth);
			verts[1][0] = FVector(x             , y + h + 0.004f, zdepth);
			verts[1][1] = FVector(x + w + 0.004f, y + h + 0.004f, zdepth);
			
			if (r > 127) r = 127;
			if (g > 127) g = 127;
			if (b > 127) b = 127;
			
			SINT	col = (0xff << 24) + ((r & 0x7f) << 17) + ((g & 0x7f) << 9) + ((b & 0x7f) << 1);
				
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
		
		// health shadow
		float	x, y;
		if (right)
			x = screen_width - HEALTH_W - (21 * DCMULTX) + left_pixels_offset;
		else
			x = (20 * DCMULTX);
		y = PLATFORM.ScreenHeight() - HEALTH_H - (4 * DCMULTY);

		STextureRefOther 	*tr = ENGINE.GetNumberedTexture(TN_HEALTH0);
		float	tw = tr->mW - 0.004f;
		float	th = tr->mH - 0.004f;
		float	w = tw * 256.f;		//!! Assume 256x256 page
		float	h = th * 256.f;
		
		DrawSprite(TN_HEALTH0, x + 2.f, y + 2.f, w, h, 6.f, 0, 0, 0, 0, FALSE, G1, 1);
	}

	//******************
	
	SINT	yo = get_panel_y_offset();

	if (yo == -1)
		return; // all done here.

	float	panel_x = 0;
	float	panel_y = 0;

	STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_PANEL);
	if (tr)
	{
		if (right)
			panel_x = left_pixels_offset + screen_width - PANEL_W + 20;
		else
			panel_x = (20 + 10) * DCMULTX;
		panel_y = (16 + 10) * DCMULTY + 3;
	}
	tr = ENGINE.GetNumberedTexture(TN_LEVELTYPE);
	if (tr)
	{
		if (right)
			panel_x -= LEVELTYPE_W;
		else
			panel_x += LEVELTYPE_W;
	}

	panel_y -= yo * DCMULTY;

	// *************************************************************************
	// lifted from CJCLEngine::RenderPlayers2D

	// draw win lose draw
	SINT	result_time = GAME.GetGameTurn() - for_player->GetResultTime();

	if (for_player->GetLevelResult() == LR_DRAW)
	{
		SINT 	yo = 0;
		if (result_time > 100)
			result_time = 100;

		if (result_time < 10)
			yo = ((10 - result_time) * (10 - result_time));
		else if (result_time > 75)
			yo = ((result_time - 75) * (result_time - 75));


		SINT 	num_segs;
		float	x;
		float	y;
		SINT	font;

		// always multiplayer

		if (right)
			x = (387 + 24) * DCMULTX;
		else
			x = (125 - 24) * DCMULTX;

		if (GAME.GetLanguage() != 0)
			num_segs = 3;
		else
		 	num_segs = 2;

		font = 2;
		y = 170 * DCMULTY;

		char 	ts[100];
		CText::GetString(TEXT_LEVEL_DRAWN, ts);
		sprintf(text, "%s", ts);

		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2)                  , y + float(yo + 4) * DCMULTY + MENUTEXTOFFSET, text_col, 2);
		DrawString(font, text, x - (sl / 2) + (1.f * DCMULTX), y + float(yo + 5) * DCMULTY + MENUTEXTOFFSET, 0, 2);

		ENGINE.DrawBar(x, y + float(yo) * DCMULTY, num_segs, 127, 127, 127, TRUE, 2.5f);
	}

	if (for_player->GetLevelResult() == LR_WIN)
	{
		SINT 	yo = 0;
		if (result_time > 100)
			result_time = 100;

		if (result_time < 10)
			yo = ((10 - result_time) * (10 - result_time));
		else if (result_time > 75)
			yo = ((result_time - 75) * (result_time - 75));


		SINT 	num_segs;
		float	x;
		float	y;
		SINT	font;

		if (screen_width == PLATFORM.ScreenWidthFull())
		{
			if (GAME.GetLanguage() != 0)
				num_segs = 5;
			else
		 		num_segs = 3;
			x = screen_width / 2;
			font = 2;
		}
		else
		{
			if (right)
				x = (387 + 24) * DCMULTX;
			else
				x = (125 - 24) * DCMULTX;

			if (GAME.GetLanguage() != 0)
				num_segs = 3;
			else
			 	num_segs = 2;
			font = 2;
		}
		y = 170 * DCMULTY;

		char 	ts[100];

		SLevelInfo		*li = CLevelStructure::GetLevelInfo(CAREER.mCurrentPrison, CAREER.mCurrentArena);
		if (li->mType == LILT_GOLF)
		{
			SINT	par = CLevelStructure::GetPar(GAME.GetCurrentLevelNumber());
			SINT	score = GAME.GolfGetShotNo(0);

			SINT	res = score - par;

			switch(res)
			{
				case -2:	CText::GetString(TEXT_EAGLE, text); break;
				case -1:	CText::GetString(TEXT_BIRDIE, text); break;
				case  0:	CText::GetString(TEXT_PAR, text); break;
				case  1:	CText::GetString(TEXT_BOGIE, text); break;
				case  2:	CText::GetString(TEXT_DOUBLE_BOGIE, text); break;

				default:
					if (res < 0)
						sprintf(text, "%d", res);
					else
						sprintf(text, "+%d", res);
					break;
			};
		}
		else
		{
			CText::GetString(TEXT_LEVEL_WON, ts);

			if (screen_width == PLATFORM.ScreenWidthFull())
			{
				SINT	time = cld->GetCompletedLevelTime();

				SINT	minutes = time / 60 / 25;
				SINT	seconds = (time / 25) % 60;
				SINT	milli   = (time % 25) * 4;

				sprintf(text, "%s:  %02d:%02d:%02d", ts, minutes, seconds, milli);
			}
			else
				sprintf(text, "%s", ts);
		}

		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2)                  , y + float(yo + 4) * DCMULTY + MENUTEXTOFFSET, text_col, 2);
		DrawString(font, text, x - (sl / 2) + (1.f * DCMULTX), y + float(yo + 5) * DCMULTY + MENUTEXTOFFSET, 0, 2);
		ENGINE.DrawBar(x, y + float(yo) * DCMULTY, num_segs, 127, 127, 127, TRUE, 2.5f);
	}

	if (for_player->GetLevelResult() == LR_LOSE)
	{
		SINT 	yo = 0;
		if (result_time > 100)
			result_time = 100;

		if (result_time < 10)
			yo = ((10 - result_time) * (10 - result_time));
		else if (result_time > 75)
			yo = ((result_time - 75) * (result_time - 75));


		SINT 	num_segs;
		float	x;
		float	y;
		SINT	font;

		if (GAME.GetLanguage() != 0)
			num_segs = 3;
		else
		 	num_segs = 2;

		if (screen_width == PLATFORM.ScreenWidthFull())
		{
			x = screen_width / 2;
			font = 2;
		}
		else
		{
			if (right)
				x = (387 + 24) * DCMULTX;
			else
				x = (125 - 24) * DCMULTX;

			font = 2;
		}
		y = (170 * DCMULTY);

		char 	ts[100];
		CText::GetString(TEXT_LEVEL_LOST, ts);
		sprintf(text, "%s", ts);
			
		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2) + 1, y + float(yo + 5) * DCMULTY + MENUTEXTOFFSET, 0, 2);			
		DrawString(font, text, x - (sl / 2)    , y + float(yo + 4) * DCMULTY + MENUTEXTOFFSET, text_col, 2.f);
		
		ENGINE.DrawBar(x, y + float(yo) * DCMULTY, num_segs, 127, 127, 127, TRUE, 2.5f);
	}

	float left_side = (left_pixels_offset + screen_width)  - (100 * DCMULTX) ;
	

	// draw racer stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER && cld)
	{
		// print position
		SINT position =  (cld->GetCurrentRacePosition() );
		sprintf(text, "%d / %d", position, GAME.GetGameLevelData().GetStartCharsInGame() );
		DrawString(2, text, panel_x , panel_y, text_col);


		// OK - if we've just started a new lap, bring up a bar
		SINT current_lap_time = cld->GetCurrentLapTime() ;
		SINT	on_lap_number = cld->GetLapNumber() + 1;
		SINT	num_laps = GAME.GetIVar(IV_NUM_LAPS_FOR_RACE);
		
		if ((current_lap_time < 100) && (on_lap_number != 0) && (on_lap_number <= num_laps))
		{
			SINT 	yo = 0;
			if (current_lap_time < 10)
				yo = ((10 - current_lap_time) * (10 - current_lap_time));
			else if (current_lap_time > 75)
				yo = ((current_lap_time - 75) * (current_lap_time - 75));
				
			SINT 	num_segs;
			float	x;
			float	y;
			SINT	font;
			
			if (screen_width == PLATFORM.ScreenWidthFull())
			{
			 	num_segs = 1;
				x = 100 * DCMULTX;
				font = 2;
			}
			else
			{
				if (right)
					x = (387 + 24) * DCMULTX;
				else
					x = (125 - 24) * DCMULTX;  
					
			 	num_segs = 1;
				font = 2;
			}
			y = 205 * DCMULTY;
			
			if (on_lap_number == num_laps)
			{
				if (GAME.GetLanguage() != 0)
					num_segs = 2;
			
				CText::GetString(TEXT_FINAL_LAP, text);
			}
			else
			{	
				char ts[100];
				CText::GetString(TEXT_LAP_NUMBER, ts);
				sprintf(text, "%s: %d/%d", ts, on_lap_number, num_laps);
			}
				
			sl = ENGINE.GetStringSize(font, text);
			DrawString(font, text, x - (sl / 2) + 1, y + float(yo + 5) * DCMULTY + MENUTEXTOFFSET, 0, 2);			
			DrawString(font, text, x - (sl / 2)    , y + float(yo + 4) * DCMULTY + MENUTEXTOFFSET, text_col, 2.f);
			
			ENGINE.DrawBar(x, y + float(yo) * DCMULTY, num_segs, 127, 127, 127, TRUE, 2.5f);
		}
	}
				


	// display last man rolling stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_DROID_BALL)
	{
		SINT	score =	pr->GetScore();
		SINT 	needed = GAME.GetIVar(IV_POINTS_REQUIRED_TO_WIN_LEVEL);

		if (GAME.IsMultiPlayer())
			sprintf(text, "%d", score / 50);
		else
			sprintf(text, "%d / %d", score / 50, needed / 50) ;
		DrawString(2, text, panel_x, panel_y, text_col);
	}
	
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_KING)
	{
		SINT 	needed = GAME.GetIVar(IV_POINTS_REQUIRED_TO_WIN_LEVEL);
		SINT	score =	pr->GetScore();
		SINT	time = (needed - score);
		
		if (time < 0)
			time = 0;
		
		SINT	minutes = time / 60 / 25;
		SINT	seconds = (time / 25) % 60;
		SINT	milli   = (time % 25) * 4;

		sprintf(text, "%02d:%02d:%02d", minutes, seconds, milli);
		DrawString(2, text, panel_x, panel_y, text_col);
	}

	// display trick tag stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_TAG && cld)
	{
		if (GAME.IsMultiPlayer())
		{
			sprintf(text, "%d", cld->GetNumTokensPickedUp());
			DrawString(2, text, panel_x, panel_y, text_col);
		}
		else
		{
			sprintf(text, "%d / %d", cld->GetNumTokensPickedUp(), CPUPToken::GetNumberTokensInWorld() +  cld->GetNumTokensPickedUp());
			DrawString(2, text, panel_x, panel_y, text_col);
		}	
	}
	
	// display golf stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF )
	{
		if (GAME.IsMultiPlayer())
		{
		}
		else
		{
			sprintf(text, "%d", GAME.GolfGetShotNo(0) + GAME.GolfGetTotalShotNo(0));
			DrawString(2, text, panel_x, panel_y, text_col);
		}	
	}


	// ****************************************************************
	// ****************************************************************
	// JCL 8/4/00
	// New panel bits

	// draw panel
	float	xoffs = 0;
	
	float	x;
	if (right)
		x = left_pixels_offset + screen_width - LEVELTYPE_W - (20 * DCMULTX);
	else
		x = (20) * DCMULTX;
	SINT	y = (16 - yo) * DCMULTY;
	
	ENGINE.DrawSprite(TN_LEVELTYPE, x, y, LEVELTYPE_W, LEVELTYPE_H, 4, 128, 128, 128, -1, FALSE, G1, right ? 0 : 1);
	xoffs = LEVELTYPE_W;

	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
	case LT_KING:		
	case LT_RACER:		
	case LT_TAG:		
	case LT_DROID_BALL:	
	case LT_GOLF:
		{
			SINT	x;
			if (right)
				x = left_pixels_offset + screen_width - PANEL_W - (20 * DCMULTX) - xoffs;
			else
				x = (20 * DCMULTX) + xoffs;
			
			SINT	y = (16 - yo) * DCMULTY;
		
			ENGINE.DrawSprite(TN_PANEL, x, y, PANEL_W, PANEL_H, 4, 128, 128, 128, -1, FALSE, G1, right ? 0 : 1);
		}
		break;
	};
}


//******************************************************************************************
void	CDCEngine::RenderPanel()
{
	CGameRenderInfo *gri = GAME.GetGameRenderInfo();

	if (gri->mActive & GRI_PROFILE)
	{
#ifdef _PROFILE
		CProfiler::ShowProfile();
#endif
	}

	// render Fades.

	if ((gri->mActive & GRI_CIRCLE_FADE_IN)  ||
	    (gri->mActive & GRI_CIRCLE_FADE_OUT) ||
	    (gri->mActive & GRI_HELP))
	{
		SINT	c0, c1;

		if ((gri->mActive & GRI_CIRCLE_FADE_IN)  ||
		    (gri->mActive & GRI_CIRCLE_FADE_OUT))
		{
			// count goes from 0 to 50.
			SINT 	count = (50 - gri->mCircleFadeCount) - 10;

			// calculate fade values
			for (c0 = 0; c0 < FADE_X; c0 ++)
			{
				for (c1 = 0; c1 < FADE_Y; c1 ++)
				{
					UBYTE 	val;

					SINT 	v1 = (c0 - (FADE_Y / 2)) / 3;  // aspect ratio
					SINT 	v2 = (c1 - (FADE_X / 2));
					v1 += v1;

					SINT 	v3 = (v1 * v1 + v2 * v2) ;
					SINT 	v4 = (v3 + 80 - count * 11) * 2;

					if (v4 < 0)
						val = 0;
					else if (v4 > 255)
						val = 255;
					else val = UBYTE(v4);

					mFadeData[c1][c0] = val;
				}

			}
		}
		if (gri->mActive & GRI_HELP)
		{
			for (c0 = 0; c0 < FADE_X; c0 ++)
			{
				for (c1 = 0; c1 < FADE_Y; c1 ++)
				{
					if ((c0 > 0) && (c0 < FADE_X - 1) && (c1 > 1) && (c1 < FADE_Y - 2))
						mFadeData[c1][c0] = 100;
					else
						mFadeData[c1][c0] = 0;
				}
			}
		}
		// draw some polygons

		//! odd stuff here - spare lines @ right & bottom mean I have to stretch this slightly...
		
		float	sx, sy, dx, dy;
		
		sx = -4;
		sy = -FADE_Y / 2;
		dx = (PLATFORM.ScreenWidthFull() / (FADE_X - 1)) + 1;
		dy = (PLATFORM.ScreenHeight() / (FADE_Y - 1)) + 1;

		STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_WHITE);
		SINT	TN = tr->mTPage + 1;

		kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
//	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ZERO);
		SINT i = kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);
	
		float	u0 = tr->mX + 0.002f;
		float	u1 = tr->mX + tr->mW - 0.002f;
		float	v0 = tr->mY + 0.002f;
		float	v1 = tr->mY + tr->mH - 0.002f;
	
		for (c0 = 0; c0 < FADE_Y - 1; c0 ++)
		{
			for (c1 = 0; c1 < FADE_X - 1; c1 ++)
			{
				InitCZTs(TN);
		
		       	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
		
		       	TextureS[0].fX                   = sx;
		       	TextureS[0].fY                   = sy;
		       	TextureS[1].fX                   = sx + dx;
		       	TextureS[1].fY                   = sy;
		       	TextureS[2].fX                   = sx;
		       	TextureS[2].fY                   = sy + dy;
		       	TextureS[3].fX                   = sx + dx;
		       	TextureS[3].fY                   = sy + dy;

		  		TextureS[0].fU                   = u0;
		       	TextureS[0].fV                   = v1;
		       	TextureS[1].fU                   = u0;
		       	TextureS[1].fV                   = v0;
		       	TextureS[2].fU                   = u1;
		       	TextureS[2].fV                   = v1;
		       	TextureS[3].fU                   = u1;
		       	TextureS[3].fV                   = v0;
	
		       	TextureS[0].u.fZ                 = 9.9945f;
		       	TextureS[1].u.fZ                 = 9.9945f;
		       	TextureS[2].u.fZ                 = 9.9945f;
		       	TextureS[3].u.fZ                 = 9.9945f;
			
		       	TextureS[0].uBaseRGB.dwPacked    = buildFadeARGB(mFadeData[c1    ][c0    ]);
		       	TextureS[1].uBaseRGB.dwPacked    = buildFadeARGB(mFadeData[c1 + 1][c0    ]);
		       	TextureS[2].uBaseRGB.dwPacked    = buildFadeARGB(mFadeData[c1    ][c0 + 1]);
		       	TextureS[3].uBaseRGB.dwPacked    = buildFadeARGB(mFadeData[c1 + 1][c0 + 1]);
		
				ExitCZTs(4);
				sx += dx;
			}
			sx = -4;
			sy += dy;
		}
/*		kmChangeStripListType			( &GouraudHead, KM_TRANS_POLYGON );    
	 	kmChangeStripUseAlpha			( &GouraudHead, KM_IMAGE_PARAM1, KM_TRUE );
	    kmChangeStripBlendingMode       ( &GouraudHead, KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVDESTALPHA);
		SINT i = kmChangeStripTextureShadingMode ( &GouraudHead, KM_IMAGE_PARAM1, KM_MODULATE);	
	 	
	
		for (c0 = 0; c0 < FADE_Y - 1; c0 ++)
		{
			for (c1 = 0; c1 < FADE_X - 1; c1 ++)
			{
				InitCZs();
		
		       	GouraudS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	GouraudS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	GouraudS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		       	GouraudS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
		
		       	GouraudS[0].fX                   = sx;
		       	GouraudS[0].fY                   = sy;
		       	GouraudS[1].fX                   = sx + dx;
		       	GouraudS[1].fY                   = sy;
		       	GouraudS[2].fX                   = sx;
		       	GouraudS[2].fY                   = sy + dy;
		       	GouraudS[3].fX                   = sx + dx;
		       	GouraudS[3].fY                   = sy + dy;
	
		       	GouraudS[0].u.fZ                 = 10.f;
		       	GouraudS[1].u.fZ                 = 10.f;
		       	GouraudS[2].u.fZ                 = 10.f;
		       	GouraudS[3].u.fZ                 = 10.f;
			
		       	GouraudS[0].uBaseRGB.dwPacked    = buildFadeARGB(mFadeData[c1    ][c0    ]);
		       	GouraudS[1].uBaseRGB.dwPacked    = buildFadeARGB(mFadeData[c1 + 1][c0    ]);
		       	GouraudS[2].uBaseRGB.dwPacked    = buildFadeARGB(mFadeData[c1    ][c0 + 1]);
		       	GouraudS[3].uBaseRGB.dwPacked    = buildFadeARGB(mFadeData[c1 + 1][c0 + 1]);
		
				ExitCZs(4);
				sx += dx;
			}
			sx = -4;
			sy += dy;
		}
*/
	}


	// ********************************************************************
	// ** Player specific stuff..

 	// one player 
	if (GAME.GetNumPlayers() == 1 )
	{
		RenderPlayerState(GAME.GetPlayer(0), 0, PLATFORM.ScreenWidth() ) ;
	}
	// two players
	else
	{
		RenderPlayerState(GAME.GetPlayer(0), 0, PLATFORM.ScreenWidth()/* >> 1 */) ;
		RenderPlayerState(GAME.GetPlayer(1), PLATFORM.ScreenWidth()/* >> 1 */, PLATFORM.ScreenWidth()/* >> 1*/) ;
	}


	// ********************************************************************
	// ********************************************************************
	// ** More state stuff

	// time limit - assume that this is the same for both players!
	CPrimary	*pr = GAME.GetPlayer(0)->GetPrimary();
	char		text[100];
	if (pr)
	{
		float	midx = (80 * DCMULTX);
		if (GAME.GetNumPlayers() == 2 )
			midx = float(PLATFORM.ScreenWidthFull() / 2);
		
	
		SINT 	yo = get_panel_y_offset();
		
		if (yo != -1)
		{		
			STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_CLOCK);
			if (tr)
			{
				DrawSprite(TN_CLOCK, midx - (CLOCK_W / 2), float(20 - yo) * DCMULTY, CLOCK_W, CLOCK_H, 2);
			}			
	
			SINT	time = pr->GetLevelData().GetTurnsTaken();
			SINT	tl = GAME.GetLevelData()->mIVars[IV_TIME_LIMIT];
			if (tl == 0)
			{
				time = time / 25;
				
				if (time > 3599)
					time = 3599;
			}
			else
				time = tl - (time / 25);
		
			if (time < 0)
				time = 0;

			SINT	secs = time % 60;
			SINT	mins = (time % 3600) / 60;
			SINT	sl;
			
			DWORD 	col = 0xff7f7f7f;
			
			if (tl != 0)
			{
				if ((secs < 10) && (mins == 0))
					col = 0xff0f0f7f;
			}
			
			if ((tl == 0) ||
			    (mins != 0) ||
			    (secs > 10) ||
			    ((tl * 25) - pr->GetLevelData().GetTurnsTaken() <= 0) ||
			    ((pr->GetLevelData().GetTurnsTaken() % 25) < 13))
			{
				yo += 5; // Hmmm......
			
				sprintf(text, "%02d", mins);
				sl = GetStringSize(2, text);
				DrawString(2, text, midx - (4.f * DCMULTX) - sl, float(32 - yo) * DCMULTY, col);
				
				sl = GetStringSize(2, ":");
				DrawString(2, ":", midx - (sl / 2), float(32 - yo) * DCMULTY, col);
				
				sprintf(text, "%02d", secs);
				DrawString(2, text, midx + (4 * DCMULTX), float(32 - yo) * DCMULTY, col);
			}
		}
	}

	// ********************************************************************
	char	str[200];

	if (gri->mActive & GRI_START_LIGHTS)
	{
		SINT	sx = STARTL_W;
		SINT	sy = STARTL_H;
		float	x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);
		float 	y = float((gri->mStartLightsPos * G(sy + 25)).Whole() - sy - 5) * DCMULTY;

		SINT	cval = ((G1 - gri->mStartLightsOn.GFrac()) * 175).Whole();
		if (cval > 127) cval = 127;
		SINT 	cval2 = 128 - cval;

		SINT	c0;
		for (c0 = 0; c0 < 4; c0 ++)
		{
			SINT	sn = TN_START1;
			DrawSprite(sn, x, y, sx, sy, 1.f, cval2, cval2, cval2);
			x += sx;
		}
		x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);

		for (c0 = 0; c0 < 4; c0 ++)
		{
			SINT	sn = TN_START1;
			if (gri->mStartLightsOn >= G(c0 + 1))
			{
				if (c0 == 3)
					sn = TN_START3;
				else
					sn = TN_START2;
			}
			
			DrawSprite(sn, x, y, sx, sy, 1.f, cval, cval, cval, 10.f);
			x += sx;
		}
	
		// and the arms
		x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);
		
		DrawSprite(TN_START_ARM, x - sx      , y - (32 * DCMULTY), ARM_W, ARM_H, 10.f);
		DrawSprite(TN_START_ARM, x + (sx * 4), y - (32 * DCMULTY), ARM_W, ARM_H, 10.f, 127, 127, 127, -1, FALSE, G1, 1);
	}

	// DEMO - "press start"
#ifndef DEMO_GAME
	if (CONTROLS.mControllers[0].GetPlaybackState() == ACTIVE)
	{	
		// is there a controller plugged in?
		struct  SPadData *pad_data1 = PLATFORM.GetPadData(0);
		if (!(pad_data1->valid))
		{
			// flash		
			if ((GAME.GetGameTurn() % 32) < 20)
			{
				CText::GetString(TEXT_PRESS_START, text);
				SINT	sl = ENGINE.GetStringSize(0, text);
				
				DrawString(0, text, 320 - (sl / 2)    , 220 * DCMULTY   , 0x00ffffff, 0);
				DrawString(0, text, 320 - (sl / 2) + 1, 220 * DCMULTY + 1, 0, 0);			
			}
		}
	}
#endif

	// Level name - single player only...		
	SINT	game_turn = GAME.GetGameTurn();
	if ((game_turn > 10) && (game_turn < 110) && (GAME.GetNumPlayers() == 1))
	{
		game_turn -= 10;
		
		SINT 	yo = 0;
		if (game_turn < 10)
			yo = ((10 - game_turn) * (10 - game_turn));
		else if (game_turn > 75)
			yo = ((game_turn - 75) * (game_turn - 75));
			
		SINT 	num_segs;
		float	x;
		float	y;
		SINT	font;
		
	 	num_segs = 3;
		x = 148.f * DCMULTX;
		font = 2;
		y = 205.f * DCMULTY;
		
		SINT	sl;
		SLevelInfo		*li = CLevelStructure::GetLevelInfo(CAREER.mCurrentPrison, CAREER.mCurrentArena);
		
		if (li->mType == LILT_GOLF)
		{
			SINT	par = CLevelStructure::GetPar(GAME.GetCurrentLevelNumber());
			
			char	t2[200];
			CText::GetString(TEXT_LIFIN_PAR, t2);
			sprintf(text, "%s %d", t2, par);
		}
		else
			CText::GetString(li->mName, text);
		
		sl = ENGINE.GetStringSize(font, text);
		
		DrawString(font, text, x - (sl / 2) + 1, y + float(yo + 5) * DCMULTY + MENUTEXTOFFSET, 0, 9);			
		DrawString(font, text, x - (sl / 2)    , y + float(yo + 4) * DCMULTY + MENUTEXTOFFSET, 0x0000ffff, 9);
		
		ENGINE.DrawBar(x, y + float(yo) * DCMULTY, num_segs, 127, 127, 127, TRUE, 10);
	}	
}

#endif
