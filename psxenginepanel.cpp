#include 	"Common.h"

#if TARGET == PSX

#include	"Globals.h"
#include	"Profile.h"
#include	"Primary.h" /// aaaaaaargh
#include	"Text.h"
#include	"CharacterLevelData.h"
#include	"Pickup.h"
#include	"LevelInfo.h"
#include	"Career.h"

// This file is essentially a continuation of PSXEngine.cpp, and contains CPSXEngine methods
// defined in PSXEngine.h

//******************************************************************************************
typedef struct 	__tsprt
		{
		u_long	tag;
		u_long	t_code;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	u0, v0;	u_short	clut;
		short	w,	h;
		}	TSPRT;					// Free size Sprite with TPage/ABR control

#define	setTSprt(p)				setTDrawTPageSize((p), 6),    ((p)->code)=0x64
#define	setTSprtTPageABR(p,t,a)	((p)->t_code) = _get_mode(0,1,(((t)&0x19f)|((a)<<5)))

//******************************************************************************************
void	CPSXEngine::DrawSprite(SINT n, SINT px, SINT py, SINT otdepth, SINT r, SINT g, SINT b, SINT semitrans, BOOL grey, GINT scale, SINT flip)
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

	return;
}

//******************************************************************************************
void	CPSXEngine::DrawBar(SINT x, SINT y, SINT num_bits, SINT colr, SINT colg, SINT colb, BOOL centre, SINT ot)
{
	if (centre)
	{
		SINT	size = 48 * (num_bits + 2);
		x -= size / 2;
	}

	SINT 	c1;
	DrawSprite(TN_BOXL, x    , y    , ot, colr, colg, colb, 2);
	x += 48;

	for (c1 = 0; c1 < num_bits; c1 ++)
	{
		DrawSprite(TN_BOXM, x    , y    , ot, colr, colg, colb, 2);
		x += 48;
	}
	DrawSprite(TN_BOXR, x    , y    , ot, colr, colg, colb, 2);
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
				printf("Unknown character: %d\n", c);
				c = '!'; break;
			}
		};
	}

	return c;
}

//******************************************************************************************
SINT	CPSXEngine::DrawChar(SINT font, UINT c, SINT x, SINT y, DWORD col, SINT otdepth)
{
	// remap international characters
	c = eval_char(c);

	// Now re-written to use the structure above. It's a SPRT primitive with TPage/ABR control
	// 4 longwords shorter with only x/y/w/h fields instead of 4 uv coords and 4 xy coords
	//!PSX (jcl) this code is ridiculous!  ah well..

	SPSXFont	*f  = &mFonts[font];
	STCFont		*fi = &f->f;  // nice clear code!

	if (x >= 0)	// sorry - had to do this.
	{
		NTAG		*o = PLATFORM.GetOrderTable();
		TSPRT		*p4=(TSPRT*)PLATFORM.GetMiscPolyFT4s();

		SINT	cw = fi->mCharWidth;
		SINT	ch = fi->mCharHeight;

		SINT	cwh = (ch<<16)|cw;
		SINT	cxy = (y<<16)|x;
		// char size

		*((SINT*)(&p4->w))=cwh;
		*((SINT*)(&p4->x0))=cxy;

		// colour
		col &= 0x00ffffff;
		col |= 0x64000000;
		*((SINT*)(&p4->r0)) = col;
		SINT	row    = c / fi->mCharsPerLine;
		SINT	column = c - row * fi->mCharsPerLine;
		SINT	bx = f->mX + (column * cw);
		SINT	by = f->mY + (row    * ch);
		SWORD bxy = (by<<8)|bx;
		*((SWORD*)(&p4->u0))=bxy;
		p4->clut  = f->mCLUT;
		setlen(p4,5);
		setTSprtTPageABR(p4,f->mTPAGE,1);
		addPrim(&o[otdepth], p4);
		p4++;

		PLATFORM.DeclareUsedMiscPolyFT4s((POLY_FT4*)p4);
	}
	return fi->mWidths[c];
}

#pragma dont_inline on
//******************************************************************************************
void	CPSXEngine::DrawString(SINT font, char *s, SINT x, SINT y, DWORD col, SINT otdepth,BOOL HasBorder)
{
	char *c=s;
	SINT xx=x;

	while (*c)
	{
		switch (*c)
		{
		case ' ':
			xx += 5;
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
SINT	CPSXEngine::GetStringSize(SINT font, char *s)
{
	SINT 		l = 0;
	SPSXFont	*f  = &mFonts[font];
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
void	add_health_quad(STextureRefOther *tr, NTAG *o, SINT x, SINT y, SINT w, SINT h, SINT tx, SINT ty, SINT tw, SINT th)
{
	POLY_FT4	*f4 = PLATFORM.GetMiscPolyFT4s();
	setPolyFT4(f4);
	setRGB0(f4, 128, 128, 128);
	setXYWH(f4, x, y, w, h);
    setUVWH(f4, tx, ty, tw, th);
    f4->tpage = tr->mTPAGE_P1P1;
    f4->clut  = tr->mCLUT;
	addPrim(&o[0], f4);
	f4 ++;
	PLATFORM.DeclareUsedMiscPolyFT4s(f4);
}

//******************************************************************************************
void	add_health_tri(STextureRefOther *tr, NTAG *o, SINT x0, SINT y0, SINT x1, SINT y1, SINT x2, SINT y2,
													  SINT u0, SINT v0, SINT u1, SINT v1, SINT u2, SINT v2)
{
	POLY_FT3	*f3 = (POLY_FT3 *)PLATFORM.GetMiscPolyFT4s();
	setPolyFT3(f3);
	setRGB0(f3, 128, 128, 128);
	setXY3(f3, x0, y0, x1, y1, x2, y2);
    setUV3(f3, u0, v0, u1, v1, u2, v2);
    f3->tpage = tr->mTPAGE_P1P1;
    f3->clut  = tr->mCLUT;
	addPrim(&o[0], f3);
	f3 ++;
	PLATFORM.DeclareUsedMiscPolyFT4s((POLY_FT4 *)f3);
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
void	CPSXEngine::RenderPlayerState(CPlayer* for_player, SINT left_pixels_offset, SINT screen_width)
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

	NTAG		*o = PLATFORM.GetOrderTable();
//	POLY_G4		*p4;

	// Golf shot no
	if ((gri->mActive & GRI_GOLF_SHOT_NO) && (for_player->GetLevelResult() != LR_WIN))
	{
		SINT 	yo = (GABS(gri->mGolfShotPos) * 150).Whole();
		SINT 	num_segs;
		SINT	x;
		SINT	y;
		SINT	font;

	 	num_segs = 2;
		x = 380;
		font = 2;
		y = 205;

		char ts[100];
		CText::GetString(TEXT_SHOT, ts);
		sprintf(text, "%s: %d", ts, gri->mGolfShotNo);

		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2)    , y + yo + 4, text_col, 0);
		DrawString(font, text, x - (sl / 2) + 1, y + yo + 5, 0, 0);

		ENGINE.DrawBar(x, y + yo, num_segs, 127, 127, 127, TRUE, 0);

	}

	// Golf OB
	if ((gri->mActive & GRI_GOLF_OB) && (for_player->GetLevelResult() != LR_WIN))
	{
		SINT 	yo = (GABS(gri->mGolfOBPos) * 150).Whole();
		SINT 	num_segs;
		SINT	x;
		SINT	y;
		SINT	font;

	 	num_segs = 2;
		x = 115;
		font = 0;
		y = 205;

		CText::GetString(TEXT_OB, text);
		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2)    , y + yo + 4, text_col, 0);
		DrawString(font, text, x - (sl / 2) + 1, y + yo + 5, 0, 0);

		ENGINE.DrawBar(x, y + yo, num_segs, 127, 127, 127, TRUE, 0);

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
		for (c0 = 3; c0 >= 0; c0 --)
		{
			STextureRefOther *tr;
			GINT	val = G0;

			SINT	r = 127;
			SINT	g = 127;
			SINT	b = 127;

			switch (c0)
			{
			case 0:
				tr = GetNumberedTexture(TN_HEALTH0);
				val = G1;

				// colour?

				// base colour
				if (brake <= G0)
				{
					r = 127;
				 	g = 127;
				 	b = 255;
				}
				if (life <= G0) // takes precedence
				{
					r = 255;
					g = 127;
					b = 127;
				}

				// now pulse if rage or life low...
				GINT	th = G(GAME.GetGameTurn()) * G(0, 30000);
//				th = th - (G((th / G_2PI).Whole()) * G_2PI);  // slow mod.
				th.mVal = (th.mVal % ((G_2PI).mVal)); // slightly better mod...?

				if ((life > G0) && (life <= G(0, 13108)))
				{
					if ((th >= G_PI_2) && (th < G_PI))
					{
						r = r + (GSIN(th) * G(128)).Whole();

					}
				}

				th -= G_PI; // shift phase
				if ((brake > G0) && (brake <= G(0, 13108)))
				{
					if ((th >= G_PI_2) && (th < G_PI))
					{
						b = b + (GSIN(th) * G(128)).Whole();

					}
				}

				 break;
			case 1:
				tr = GetNumberedTexture(TN_HEALTH3);
				val = rage;
				if ((pr->GetRage() == G1) ||
					(pr->GetRenderAnimation() == AM_RAGE_ATTACK))
				{
					// flash
					r = ((GSIN(G(GAME.GetGameTurn()) * G(0, 30000)) + G1) * G(64)).Whole() + 96;
					g = r;
					b = r;
				}
				break;
			case 2: tr = GetNumberedTexture(TN_HEALTH2); val = brake;		break;
			case 3: tr = GetNumberedTexture(TN_HEALTH1); val = life;		break;
			};

			SINT	tx = tr->mX;
			SINT	ty = tr->mY;
			SINT	tw = tr->mW;
			SINT	th = tr->mH;

			SINT	x;
			if (right)
				x = screen_width - tw - 20 + left_pixels_offset;
			else
				x = 20;
			SINT	y = PLATFORM.ScreenHeight() - th - 20;

			if (val != G1)
			{
				tw  = (G(tr->mW) * val * GINT_HALF).Whole() + 4;
				if (right)
				{
					x  += (G(tr->mW) * (G1 - val) * GINT_HALF).Whole() + 4;
					tx += (G(tr->mW) * (G1 - val) * GINT_HALF).Whole() + 4;
				}
				else
				{
					x  += (G(tr->mW) * (G1/* - val*/) * GINT_HALF).Whole() - 7;
					tx += (G(tr->mW) * (G1 - val) * GINT_HALF + GINT_HALF).Whole() + 4;   // hackity - hack!
				}
			}

			SINT	w = tw;
			SINT	h = th;

			POLY_FT4	*f4 = PLATFORM.GetMiscPolyFT4s();
			setPolyFT4(f4);
			setRGB0(f4, r, g, b);
			setXYWH(f4, x, y, w, h);
			if (right)
			    setUVWH(f4, tx, ty, tw, th);
			else
			    setUVWH(f4, tx + tw, ty, -tw, th);
		    f4->tpage = tr->mTPAGE_P1P1;
		    f4->clut  = tr->mCLUT;
			addPrim(&o[3], f4);
			f4 ++;
			PLATFORM.DeclareUsedMiscPolyFT4s(f4);
		}
	}

	//******************
	SINT	yo = get_panel_y_offset();

	if (yo == -1)
		return; // all done here.

	SINT	panel_x = 0;
	SINT	panel_y = 0;

	STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_PANEL);
	if (tr)
	{
		if (right)
			panel_x = left_pixels_offset + screen_width - tr->mW - 20 + 20;
		else
			panel_x = 20 + 10;
		panel_y = 16 + 10;
	}
	tr = ENGINE.GetNumberedTexture(TN_LEVELTYPE);
	if (tr)
	{
		if (right)
			panel_x -= tr->mW;
		else
			panel_x += tr->mW;
	}

	panel_y -= yo;

	// *************************************************************************
	// lifted from CJCLEngine::RenderPlayers2D

	// draw win lose draw
	SINT	result_time = GAME.GetGameTurn() - for_player->GetResultTime();

	if (for_player->GetLevelResult() == LR_DRAW)
/*	{
		CText::GetString(TEXT_LEVEL_DRAWN, text);
		sl = ENGINE.GetStringSize(0, text);
		DrawString(0, text, ((screen_width / 2) - (sl / 2))+left_pixels_offset, 60, text_col);
	}*/
	{
		SINT 	yo = 0;
		if (result_time > 100)
			result_time = 100;

		if (result_time < 10)
			yo = ((10 - result_time) * (10 - result_time));
		else if (result_time > 75)
			yo = ((result_time - 75) * (result_time - 75));


		SINT 	num_segs;
		SINT	x;
		SINT	y;
		SINT	font;

		// always multiplayer

		if (right)
			x = 387 + 24;
		else
			x = 125 - 24;

		if (GAME.GetLanguage() != 0)
			num_segs = 3;
		else
		 	num_segs = 2;

		font = 2;
		y = 170;

		char 	ts[100];
		CText::GetString(TEXT_LEVEL_DRAWN, ts);
		sprintf(text, "%s", ts);

		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2)    , y + yo + 4, text_col, 0);
		DrawString(font, text, x - (sl / 2) + 1, y + yo + 5, 0, 0);

		ENGINE.DrawBar(x, y + yo, num_segs, 127, 127, 127, TRUE, 0);
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
		SINT	x;
		SINT	y;
		SINT	font;

		if (screen_width == PLATFORM.ScreenWidthFull())
		{
			if (GAME.GetLanguage() != 0)
				num_segs = 4;
			else
		 		num_segs = 3;
			x = screen_width / 2;
			font = 2;
		}
		else
		{
			if (right)
				x = 387 + 24;
			else
				x = 125 - 24;

			if (GAME.GetLanguage() != 0)
				num_segs = 3;
			else
			 	num_segs = 2;
			font = 2;
		}
		y = 170;

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
		DrawString(font, text, x - (sl / 2)    , y + yo + 4, text_col, 0);
		DrawString(font, text, x - (sl / 2) + 1, y + yo + 5, 0, 0);

		ENGINE.DrawBar(x, y + yo, num_segs, 127, 127, 127, TRUE, 0);
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
		SINT	x;
		SINT	y;
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
				x = 387 + 24;
			else
				x = 125 - 24;

			font = 2;
		}
		y = 170;

		char 	ts[100];
		CText::GetString(TEXT_LEVEL_LOST, ts);
		sprintf(text, "%s", ts);
			
		sl = ENGINE.GetStringSize(font, text);
		DrawString(font, text, x - (sl / 2)    , y + yo + 4, text_col, 0);
		DrawString(font, text, x - (sl / 2) + 1, y + yo + 5, 0, 0);			
		
		ENGINE.DrawBar(x, y + yo, num_segs, 127, 127, 127, TRUE, 0);
	}
	
	SINT left_side = (left_pixels_offset + screen_width)  - 100 ;
	
//	#define	TEXT_Y	100
	#define	TEXT_Y	32
	
	
	// draw racer stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER && cld)
	{
		// print position
		SINT position =  (cld->GetCurrentRacePosition() );
//		CText::GetString(TEXT_POS, text2);
//		sprintf(text, "%s: %d/%d", text2, position, GAME.GetGameLevelData().GetStartCharsInGame() );
		sprintf(text, "%d / %d", position, GAME.GetGameLevelData().GetStartCharsInGame() );
		DrawString(2, text, panel_x , panel_y, text_col, 0);


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
			SINT	x;
			SINT	y;
			SINT	font;
			
			if (screen_width == PLATFORM.ScreenWidthFull())
			{
//			 	num_segs = 3;
//				x = 140;
//				font = 0;
			 	num_segs = 1;
				x = 100;
				font = 2;
			}
			else
			{
				if (right)
					x = 387 + 24;
				else
					x = 125 - 24;
					
			 	num_segs = 1;
				font = 2;
			}
			y = 205;
			
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
//				sprintf(text, "%s:   %d / %d", ts, on_lap_number, num_laps);
				sprintf(text, "%s: %d/%d", ts, on_lap_number, num_laps);
			}
				
			sl = ENGINE.GetStringSize(font, text);
			DrawString(font, text, x - (sl / 2)    , y + yo + 4, text_col, 0);
			DrawString(font, text, x - (sl / 2) + 1, y + yo + 5, 0, 0);			
			
			ENGINE.DrawBar(x, y + yo, num_segs, 127, 127, 127, TRUE, 0);
		}		
				

/*		// print what lap we are on
		SINT on_lap_number =  (cld->GetLapNumber()+1) ;
		if (on_lap_number > GAME.GetIVar(IV_NUM_LAPS_FOR_RACE)) on_lap_number = GAME.GetIVar(IV_NUM_LAPS_FOR_RACE) ;
		CText::GetString(TEXT_LAP_NUMBER, text2);
		sprintf(text, "%s: %d/%d", text2, on_lap_number, GAME.GetIVar(IV_NUM_LAPS_FOR_RACE) );
		DrawString(2, text, left_side , TEXT_Y + 32, text_col);

		// draw current lap time
		SINT current_lap_time = cld->GetCurrentLapTime() ;

		// if finsihed then print last lap time ;
		if (cld->GetLapNumber() == GAME.GetIVar(IV_NUM_LAPS_FOR_RACE) )
		{
			current_lap_time = cld->GetLastLapTime() ;
		}

		CText::GetString(TEXT_LAP_TIME, text2);
		sprintf(text, "%s: %02d:%02d", text2, current_lap_time / 25, ((current_lap_time *4) % 100));
		DrawString(2, text, left_side , TEXT_Y + 16, text_col);
		
		// draw best lap time
		CText::GetString(TEXT_BEST_LAP_TIME, text2);
		SINT best_lap_time = cld->GetFastedLapTime() ;
		if (best_lap_time != -1)
		{
			sprintf(text, "%s: %02d:%02d", text2, best_lap_time / 25, ((best_lap_time *4) % 100));
		}
		else
		{
			sprintf(text, "%s: - -:- -", text2);
		}
		DrawString(2, text, left_side , TEXT_Y, text_col);*/
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

	// ***************************************

/*	// Score
	if (gri->mActive & GRI_SCORE)
	{
		sprintf(text, "%06d", pr->GetScore());
		DrawString(2, text, left_side , 16, text_col);
	}
*/

	// ****************************************************************
	// ****************************************************************
	// JCL 8/4/00
	// New panel bits

	// draw panel
	SINT	xoffs = 0;
	
	tr = ENGINE.GetNumberedTexture(TN_LEVELTYPE);
	if (tr)
	{

		SINT	x;
		if (right)
			x = left_pixels_offset + screen_width - tr->mW - 20;
		else
			x = 20;
		SINT	y = 16 - yo;
		
		ENGINE.DrawSprite(TN_LEVELTYPE, x, y, 1, 128, 128, 128, -1, FALSE, G1, right ? 0 : 1);
		xoffs = tr->mW;
	}

	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
	case LT_KING:		
	case LT_RACER:		
	case LT_TAG:		
	case LT_DROID_BALL:	
	case LT_GOLF:
		tr = ENGINE.GetNumberedTexture(TN_PANEL);
		if (tr)
		{
	
			SINT	x;
			if (right)
				x = left_pixels_offset + screen_width - tr->mW - 20 - xoffs;
			else
				x = 20 + xoffs;
			
			SINT	y = 16 - yo;
		
			ENGINE.DrawSprite(TN_PANEL, x, y, 1, 128, 128, 128, -1, FALSE, G1, right ? 0 : 1);
		}
		break;
	};
}


//******************************************************************************************
void	CPSXEngine::RenderPanel()
{
	NTAG		*o = PLATFORM.GetOrderTable();
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

					SINT 	v1 = (c0 - (FADE_Y / 2));
					SINT 	v2 = c1 - (FADE_X / 2);
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
		POLY_G4 *p4 = PLATFORM.GetMiscPolyG4s();
		SINT	sx = -FADE_X / 2;
		SINT 	sy = -FADE_Y / 2;
		SINT	dx = (PLATFORM.ScreenWidthFull() / (FADE_X - 1)) + 1;
		SINT	dy = (PLATFORM.ScreenHeight() / (FADE_Y - 1)) + 1;

		for (c0 = 0; c0 < FADE_Y - 1; c0 ++)
		{
			for (c1 = 0; c1 < FADE_X - 1; c1 ++)
			{
				setPolyG4(p4);
				p4->x0	= sx;
				p4->y0	= sy;
				p4->r0	= mFadeData[c0][c1];
				p4->g0	= mFadeData[c0][c1];
				p4->b0	= mFadeData[c0][c1];

				p4->x1	= sx + dx;
				p4->y1	= sy;
				p4->r1	= mFadeData[c0][c1 + 1];
				p4->g1	= mFadeData[c0][c1 + 1];
				p4->b1	= mFadeData[c0][c1 + 1];

				p4->x2	= sx;
				p4->y2	= sy + dy;
				p4->r2	= mFadeData[c0 + 1][c1];
				p4->g2	= mFadeData[c0 + 1][c1];
				p4->b2	= mFadeData[c0 + 1][c1];

				p4->x3	= sx + dx;
				p4->y3	= sy + dy;
				p4->r3	= mFadeData[c0 + 1][c1 + 1];
				p4->g3	= mFadeData[c0 + 1][c1 + 1];
				p4->b3	= mFadeData[c0 + 1][c1 + 1];

			    setSemiTrans(p4, 1);

				addPrim(&o[0], p4);

				p4 ++;

				sx += dx;
			}
			sx = 0;
			sy += dy;
		}

		// make sure the right alpha mode is set
		PLATFORM.DeclareUsedMiscPolyG4s(p4);//(FADE_X - 1) * (FADE_Y - 1));
		PLATFORM.SetAlphaMode2(0);
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
		SINT	midx = 80;
		if (GAME.GetNumPlayers() == 2 )
			midx = PLATFORM.ScreenWidthFull() / 2;
		
	
		SINT 	yo = get_panel_y_offset();
		
		if (yo != -1)
		{		
			STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_CLOCK);
			if (tr)
			{
				DrawSprite(TN_CLOCK, midx - (tr->mW / 2), 20 - yo, 2);
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
				sprintf(text, "%02d", mins);
				sl = GetStringSize(2, text);
				DrawString(2, text, midx - 4 - sl, 23 - yo, col);
				
				sl = GetStringSize(2, ":");
				DrawString(2, ":", midx - (sl / 2), 23 - yo, col);
				
				sprintf(text, "%02d", secs);
				DrawString(2, text, midx + 4, 23 - yo, col);
			}
		}
	}
	
	// ********************************************************************
	
	char	str[200];

#if 0
	sprintf(str, "Level: %03d", GAME.GetCurrentLevelNumber());
	DrawString(0, str, 10, 15, 0xff7f7f7f);
#endif

//	sprintf(str, "^ for help");
//	DrawString(0, str, 10, 225, 0xff7f7f7f);

	if (gri->mActive & GRI_START_LIGHTS)
	{
		POLY_FT4	*f4 = PLATFORM.GetMiscPolyFT4s();

		SINT	sx = 48;
		SINT	sy = 32;
		SINT	x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);
		SINT	y = (gri->mStartLightsPos * G(sy + 25)).Whole() - sy - 5;

		SINT	cval = ((G1 - gri->mStartLightsOn.GFrac()) * 175).Whole();
		if (cval > 127) cval = 127;
		SINT 	cval2 = 128 - cval;

		SINT	c0;
		for (c0 = 0; c0 < 4; c0 ++)
		{
			STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_START1);

			if (gri->mStartLightsOn >= G(c0 + 1))
			{
				if (c0 == 3)
					tr = ENGINE.GetNumberedTexture(TN_START3);
				else
					tr = ENGINE.GetNumberedTexture(TN_START2);
			}

			setPolyFT4(f4);
			setRGB0(f4, cval, cval, cval);
			setXYWH(f4, x, y, sx, sy);
		    setUVWH(f4, tr->mX, tr->mY, tr->mW - 1, tr->mH - 1);
		    f4->tpage = tr->mTPAGE_P1P1;
		    f4->clut  = tr->mCLUT;
		    setSemiTrans(f4, TRUE);
			addPrim(&o[0], f4);
			f4 ++;

			x += sx;
		}

		x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);
		for (c0 = 0; c0 < 4; c0 ++)
		{
			STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_START1);

			setPolyFT4(f4);
			setRGB0(f4, cval2, cval2, cval2);
			setXYWH(f4, x, y, sx, sy);
		    setUVWH(f4, tr->mX, tr->mY, tr->mW - 1, tr->mH - 1);
		    f4->tpage = tr->mTPAGE_P1P1;
		    f4->clut  = tr->mCLUT;
			addPrim(&o[0], f4);
			f4 ++;

			x += sx;
		}

		// and the arms
		STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_START_ARM);
		x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);
		
		setPolyFT4(f4);
		setRGB0(f4, 127, 127, 127);
		setXYWH(f4, x - 48, y - 32, 48, 64);
	    setUVWH(f4, tr->mX, tr->mY, tr->mW - 1, tr->mH - 1);
	    f4->tpage = tr->mTPAGE_P1P1;
	    f4->clut  = tr->mCLUT;
		addPrim(&o[0], f4);
		f4 ++;

		setPolyFT4(f4);
		setRGB0(f4, 127, 127, 127);
		setXYWH(f4, x + (sx * 4), y - 32, 48, 64);
	    setUVWH(f4, tr->mX + tr->mW - 1, tr->mY, -(tr->mW - 1), tr->mH - 1);
	    f4->tpage = tr->mTPAGE_P1P1;
	    f4->clut  = tr->mCLUT;
		addPrim(&o[0], f4);
		f4 ++;
		
		
		
		x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);
		
		PLATFORM.DeclareUsedMiscPolyFT4s(f4);
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
				
				DrawString(0, text, 256 - (sl / 2)    , 220    , 0x00ffffff, 0);
				DrawString(0, text, 256 - (sl / 2) + 1, 220 + 1, 0, 0);			
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
		SINT	x;
		SINT	y;
		SINT	font;
		
	 	num_segs = 3;
		x = 148;
		font = 2;
		y = 205;
		
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
		
		DrawString(font, text, x - (sl / 2)    , y + yo + 4, 0x0000ffff, 0);
		DrawString(font, text, x - (sl / 2) + 1, y + yo + 5, 0, 0);			
		
		ENGINE.DrawBar(x, y + yo, num_segs, 127, 127, 127, TRUE, 0);
	}	
	
}

#endif
