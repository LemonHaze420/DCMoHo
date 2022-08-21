#include	"Common.h"

#if TARGET == PC

#include	"JCLEngine.h"
#include	"Globals.h"
#include	"Primary.h"
#include    "Editors.h"
#include	"Text.h"
#include	"Pickup.h"
#include	"LevelInfo.h"
#include	"Career.h"
#include	"PCFEPrisonSelect.h"
#include	"PCSetUpQuery.h"


//******************************************************************************************
//** This file is basically a continuation of JCLEngine.CPP - it shares JCLEngine.h

#define		MENUTEXTOFFSET	14

extern	void	sort_out_colour(DWORD &col);
extern	void	sort_out_colour2(DWORD &col);

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
#define NUMBER_SIZE		32.f
#define NUMBER_OFFSET	20.f
#define NUMBER_UV_SIZE	0.25f

//******************************************************************************************
enum	EIcons
{
	ICON_LIVES = 0,
	ICON_BOMB,
	ICON_AVERAGE,
};
/*
		case TN_BOXL:
		case TN_BOXM:
		case TN_BOXR:
		case TN_ARROW_L:
		case TN_ARROW_R:
		case TN_ARROW_U:
		case TN_ARROW_D:
*/

//******************************************************************************************
bool	CJCLEngine::auto_shadow(CTextureTL *tex)
{
	static CTextureTL**	shadTexFE[]={

		&PRISONSELECT.title[0],
		&PRISONSELECT.title[1],
		&PRISONSELECT.title[2],
		&PRISONSELECT.title[3],
		&PRISONSELECT.title[4],
		&PRISONSELECT.boxl,
		&PRISONSELECT.boxm,
		&PRISONSELECT.boxr,
		&PRISONSELECT.arrow_l,
		&PRISONSELECT.arrow_r,
		&PRISONSELECT.arrow_u,
		&PRISONSELECT.arrow_d,
		&PRISONSELECT.qualify,
		&PRISONSELECT.draw,
		&PRISONSELECT.gold,
		&PRISONSELECT.goldlt,
		&PRISONSELECT.selchar,
		&PRISONSELECT.clipl,
		&PRISONSELECT.clipr,
		&PRISONSELECT.ps[ 0],
		&PRISONSELECT.ps[ 1],
		&PRISONSELECT.ps[ 2],
		&PRISONSELECT.ps[ 3],
		&PRISONSELECT.ps[ 4],
		&PRISONSELECT.ps[ 5],
		&PRISONSELECT.ps[ 6],
		&PRISONSELECT.ps[ 7],
		&PRISONSELECT.ps[ 8],
		&PRISONSELECT.ps[ 9],
		&PRISONSELECT.ps[10],
		&PRISONSELECT.ar[ 0],
		&PRISONSELECT.ar[ 1],
		&PRISONSELECT.ar[ 2],
		&PRISONSELECT.ar[ 3],
		&PRISONSELECT.ar[ 4],
		&PRISONSELECT.ar[ 5],
		&PRISONSELECT.ar[ 6],
		&PRISONSELECT.ar[ 7],
		&PRISONSELECT.ar[ 8],
		&PRISONSELECT.ar[ 9],
		&PRISONSELECT.ar[10],
		NULL
	};

	static CTextureTL**	shadTex[]={
		&mTexHealth0,
		&mTexStart1,
		&mTexStart2,
		&mTexStart3,
		&mStartArm,
		&mPanel,
		&mLevelType[0],
		&mLevelType[1],
		&mLevelType[2],
		&mLevelType[3],
		&mLevelType[4],
		&mLevelType[5],
		&mLevelType[6],
		&mLevelType[7],
		&mClock,
		&mTexBarLeft,
		&mTexBarMiddle,
		&mTexBarRight,
		NULL
	};

	// do we try to shadow this?
	if (GAME.mInFrontEnd)
	{
		for(int i=0;shadTexFE[i];++i)
			if(tex == *shadTexFE[i])
				return true;
	}
	else
	{
		for(int i=0;shadTex[i];++i)
			if(tex == *shadTex[i])
				return true;
	}
	return false;
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
void	CJCLEngine::RenderPlayerState(CPlayer* for_player, SINT left_pixels_offset, SINT screen_width  ) 
{
#if defined(LINK_EDITOR)
	if (EDITOR.IsActive()) return ;
#endif
	SINT	i[4];

	DWORD	text_col = 0x0000ffff;
	char 	text[200];
	SINT 	sl;

	//CTexture::DrawAllTextures();
	//CTexture::DrawAllTexturesAdditive();

	float hiResOffsetY = PLATFORM.ScreenHeightScale();
	float hiResOffsetX = PLATFORM.ScreenWidthScale();


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
	
		x = PLATFORM.ScreenWidth() - (120);// * hiResOffsetX);
			
		font = 2;
		y = (PLATFORM.ScreenHeight()/10) * 9 - 24;
		
		char ts[100];
		CText::GetString(TEXT_SHOT, ts);
		sprintf(text, "%s: %d", ts, gri->mGolfShotNo);
		
		sl = ENGINE.GetStringSize(font, text);
		DrawShadowedString(font, text, x - (sl / 2),( y + float(yo + 4) * PCMULTY)  + MENUTEXTOFFSET , text_col, 0.03f);
		ENGINE.DrawBar(x,( y + float(yo + 0) * PCMULTY + 0 ), num_segs, 127, 127, 127, TRUE, 8);
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
		x = 120;// * hiResOffsetX;  
		font = 0;
		y = (PLATFORM.ScreenHeight()/10) * 9 - 24;
		
		CText::GetString(TEXT_OB, text);
		sl = ENGINE.GetStringSize(font, text);
		DrawShadowedString(font, text, (x - (sl / 2) ), ( y + float(yo + 4) * PCMULTY)  + MENUTEXTOFFSET, text_col, 0.03f);
		ENGINE.DrawBar(x,( y + float(yo + 0) * PCMULTY) + 0, num_segs, 127, 127, 127, TRUE, 8);
	}
	CTextureTL *tex;

	if (gri->mActive & GRI_LIFE)
	{
		SINT	c0;

		GINT life = G(pr->GetLife()) / 100;
		if (life < G0)
			life = G0;
		if (life > G1)
			life = G1;

		GINT	brake = pr->GetBrakePower();
		GINT	rage  = pr->GetRage();


		float	x, y;
		if (right)
			x = screen_width - HEALTH_W - (21 * PCMULTX) + left_pixels_offset;
		else
			x = (20 * PCMULTX);
		y = PLATFORM.ScreenHeight() - HEALTH_H - (4 * PCMULTY);
		
		CTextureTL *tex;
		tex = mTexHealth0; 
		float	tw = 1.0f - 0.004f;
		float	th = 1.0f - 0.004f;
		float	w = tw * 128.f;		//!! Assume 256x256 page
		float	h = th * 128.f;
		if(!right)
		{
			DrawSpritePC(tex, x + 2.f, y + 2.f,0.1f, w, h, 0x0, 1.0, 1.0f, 1);
		}
		else
		{
			DrawSpritePC(tex, x + 2.f, y + 2.f,0.1f, w, h, 0x0, 1.0, 1.0f, 0);
		}
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();

		// draw bits
		for (c0 = 0; c0 < 4 ; c0++)
		{
			CTextureTL *tex;

			GINT	val = G0;
			
			SINT	r = 127;
			SINT	g = 127;
			SINT	b = 127;
			switch (c0)
			{
			case 0:
				{
				tex = mTexHealth0; 
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
				}
				break;
			case 1:
				tex = mTexHealth3;
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
			case 2: tex = mTexHealth2; val = brake;		break;
			case 3: tex = mTexHealth1; val = life;		break;
			};

			float	tx = 0.002f;
			float	ty = 0.002f;
			float	tw = 1.0f - 0.005f;
			float	th = 1.0f- 0.005f;
			
			float	x;
			if (right)
				x = screen_width - HEALTH_W - (21 * PCMULTX) + left_pixels_offset;
			else
				x = (20 * PCMULTX);
			float	y = PLATFORM.ScreenHeight() - HEALTH_H - (4 * PCMULTY);
			
			float	fval = GToF(val);
			
			if (val != G1)
			{
				tw  = (fval * 0.5f) + (5.f / 128.f);
				if (right)
				{
					x  += (float(HEALTH_W) * (1.f - fval) * 0.5f) + 5.f;
					tx += ((1.f - fval) * 0.5f) + (5.f / 128.f);
				}
				else
				{
					x  += (float(HEALTH_W) * (1.f		) * 0.5f) - 10.f - 0.75f; // JCL
					tx += ((((1.f - fval) * 0.5f) ) + (5.f / 128.f)) ;   // hackity - hack!
				}
			}
			
			float	w = tw * 128.f;		//!! Assume 256x256 page
			float	h = th * 128.f;
			
//			float	frac = float(fmod(val * 8.f, 1.f));
//			float	frac2 = 1.f - frac;

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
			
			float	zdepth =  ((4.f - float(c0 / 100.f)) / 90.4f);

			if (r > 127) r = 127;
			if (g > 127) g = 127;
			if (b > 127) b = 127;

			SINT	col = (0xff << 24) + ((r & 0x7f) << 17) + ((g & 0x7f) << 9) + ((b & 0x7f) << 1);
			
			
			if(right)
			{
				i[0] = tex->AddVertex(&D3DTLVERTEX(D3DVECTOR(x     , y     , zdepth), 0.1f, col, D3DRGB(0,0,0), us, ve));
				i[1] = tex->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y     , zdepth), 0.1f, col, D3DRGB(0,0,0), ue, ve));
				i[2] = tex->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y + h, zdepth), 0.1f, col, D3DRGB(0,0,0), ue, vs));
				i[3] = tex->AddVertex(&D3DTLVERTEX(D3DVECTOR(x     , y + h, zdepth), 0.1f, col, D3DRGB(0,0,0), us, vs));
			}
			else
			{
				i[0] = tex->AddVertex(&D3DTLVERTEX(D3DVECTOR(x     , y     , zdepth), 0.1f, col, D3DRGB(0,0,0), us, ve));
				i[1] = tex->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y     , zdepth), 0.1f, col, D3DRGB(0,0,0), ue, ve));
				i[2] = tex->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + w, y + h, zdepth), 0.1f, col, D3DRGB(0,0,0), ue, vs));
				i[3] = tex->AddVertex(&D3DTLVERTEX(D3DVECTOR(x     , y + h, zdepth), 0.1f, col, D3DRGB(0,0,0), us, vs));
			}
			
			
			tex->AddTri(i[0], i[1], i[3]);
			tex->AddTri(i[1], i[2], i[3]);

			CTexture::DrawAllTextures();
			CTexture::DrawAllTexturesAdditive();

			// Shouldn't really render here! 
			// This code is dependent upon the order of rendering!
		//	tex->DrawAll();
		}
		// health shadow
		
		tex = mTexHealth0; 
			tw = 1.0f - 0.004f;
			th = 1.0f - 0.004f;
			w = tw * 128.f;		//!! Assume 256x256 page
			h = th * 128.f;
		if(!right)
		{
			DrawSpritePC(tex, x + 2.f, y + 2.f,0.1f, w, h, 0x0, 1.0, 1.0f, 1);
		}
		else
		{
			DrawSpritePC(tex, x + 2.f, y + 2.f,0.1f, w, h, 0x0, 1.0, 1.0f, 0);
		}
		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();
	}

	//******************
	SINT	yo = get_panel_y_offset();
	
	if (yo == -1)
		return; // all done here.
	

	float	panel_x = 0;
	float	panel_y = 0;
	
	if (right)
		panel_x = left_pixels_offset + screen_width - PANEL_W + 20;
	else
		panel_x = (20 + 10) * PCMULTX;
	panel_y = (16 + 10) * PCMULTY + 3;

	if (right)
		panel_x -= LEVELTYPE_W;
	else
		panel_x += LEVELTYPE_W;
	
	panel_y -= yo * PCMULTY;

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
				x = PLATFORM.ScreenWidth() - (150);// * hiResOffsetX);
			else
				x = 150;// * hiResOffsetX;  
		
		if (GAME.GetLanguage() != 0)
			num_segs = 3;
		else
			num_segs = 2;
		
		font = 2;
		y = (PLATFORM.ScreenHeight()/10) * 9 - 48;
		
		char 	ts[100];
		CText::GetString(TEXT_LEVEL_DRAWN, ts);
		sprintf(text, "%s", ts);
		
		sl = ENGINE.GetStringSize(font, text);
		DrawShadowedString(font, text, x - (sl / 2) + 1,(( y + float((yo + 4)) * PCMULTY)) + MENUTEXTOFFSET  , text_col, 0.03f);
		ENGINE.DrawBar(x - (sl / sl) + 1, (( y + float((yo + 0)) * PCMULTY))  + 0 , num_segs, 127, 127, 127, TRUE, 8);
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
				num_segs = 4;
			else
				num_segs = 3;
			x = screen_width / 2;
			font = 2;
		}
		else
		{
			if (right)
				x = PLATFORM.ScreenWidth() - (150);// * hiResOffsetX);
			else
				x = 150;// * hiResOffsetX;  
			
			if (GAME.GetLanguage() != 0)
				num_segs = 3;
			else
				num_segs = 2;
			font = 2;
		}
		y = (PLATFORM.ScreenHeight()/10) * 9 - 48;
		
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
		DrawShadowedString(font, text,x - (sl / 2) + 1,(( y + float((yo + 4)) * PCMULTY))  + MENUTEXTOFFSET  , text_col, 0.03f);
		ENGINE.DrawBar(x - (sl / sl) + 1,(( y + float((yo + 0)) * PCMULTY))  + 0 , num_segs, 127, 127, 127, TRUE, 8);
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
				x = PLATFORM.ScreenWidth() - (150);// * hiResOffsetX);
			else
				x = 150;// * hiResOffsetX;  
			
			font = 2;
		}
		y = (PLATFORM.ScreenHeight()/10) * 9 - 48;
		
		char 	ts[100];
		CText::GetString(TEXT_LEVEL_LOST, ts);
		sprintf(text, "%s", ts);
		
		sl = ENGINE.GetStringSize(font, text);
		DrawShadowedString(font, text, x - (sl / 2) + 1,(( y + float((yo + 4)) * PCMULTY))  + MENUTEXTOFFSET  , text_col, 0.03f);
		ENGINE.DrawBar(x - (sl / sl) + 1,(( y + float((yo + 0)) * PCMULTY))  + 0 , num_segs, 127, 127, 127, TRUE, 8);
	}
	
	float left_side = (left_pixels_offset + screen_width)  - (100 * PCMULTX) ;
	
	
	// draw racer stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER && cld)
	{
		// print position
		SINT position =  (cld->GetCurrentRacePosition() );

		sprintf(text, "%d / %d", position, GAME.GetGameLevelData().GetStartCharsInGame() );
		DrawShadowedString(2, text, panel_x , panel_y, text_col,0.08f);
		
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
			float scaleis = (PLATFORM.ScreenHeightScale());
			
			if (screen_width == PLATFORM.ScreenWidthFull())
			{

				num_segs = 1;
				x = 100 * PCMULTX;
				font = 2;
			}
			else
			{
				if (right)
					x = PLATFORM.ScreenWidth() - (120);// * hiResOffsetX);
				else
					x = 120;// * hiResOffsetX;  
				
				num_segs = 1;
				font = 2;
			}
			y = (PLATFORM.ScreenHeight()/10) * 9 - 48;
			
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
			DrawShadowedString(font, text, x - (sl / 2) + 1, ((y + float(yo + (4) * PCMULTY) + MENUTEXTOFFSET)), text_col, 0.034f);	
			ENGINE.DrawBar(x - (sl / sl) + 1, ((y + float(yo + (0) * PCMULTY) + 0)), num_segs, 127, 127, 127, TRUE, 9);
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

		DrawShadowedString(2, text, panel_x, panel_y, text_col, 0.1f);
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
		DrawShadowedString(2, text, panel_x, panel_y, text_col,0.1f);
	}
	
	// display trick tag stuff
	if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_TAG && cld)
	{
		if (GAME.IsMultiPlayer())
		{
			sprintf(text, "%d", cld->GetNumTokensPickedUp());
			DrawShadowedString(2, text, panel_x, panel_y, text_col,0.1f);
		}
		else
		{
			sprintf(text, "%d / %d", cld->GetNumTokensPickedUp(), CPUPToken::GetNumberTokensInWorld() +  cld->GetNumTokensPickedUp());
			DrawShadowedString(2, text, panel_x, panel_y, text_col,0.1f);
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
			DrawShadowedString(2, text, panel_x, panel_y, text_col,0.1f);
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
		x = left_pixels_offset + screen_width - LEVELTYPE_W - (20 * PCMULTX);
	else
		x = (20) * PCMULTX;
	SINT	y = (16 - yo) * PCMULTY;
	
	//CTextureTL *tex;

	switch (GAME.GetIVar(IV_LEVEL_TYPE))
	{
		case LT_GOLF:
		default:
			tex = mLevelType[0];
			break;
		case LT_RACER:
			tex = mLevelType[1];
			break;
		case LT_LASTMAN:
		case LT_GAUNTLET:
		case LT_PURSUIT:
		case LT_TAG:
		case LT_KING:
		case LT_DROID_BALL:
			tex = mLevelType[GAME.GetIVar(IV_LEVEL_TYPE)-3];
			break;				
	}
	
	if(right)
	{
		DrawSpritePC(tex,x,y,0.1f,LEVELTYPE_W,LEVELTYPE_H);
	}
	else
	{
		DrawSpritePC(tex,x,y,0.1f,LEVELTYPE_W,LEVELTYPE_H,0xFFFFFFFF, 1.0f, 1.0f, 1);
	}
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
				x = left_pixels_offset + screen_width - PANEL_W - (20 * PCMULTX) - xoffs;
			else
				x = (20 * PCMULTX) + xoffs;
			
			SINT	y = (16 - yo) * PCMULTY;
			if(right)
			{
				DrawSpritePC(mPanel,x,y,0.1f,PANEL_W,PANEL_H);
			}
			else
			{
				DrawSpritePC(mPanel,x,y,0.1f,PANEL_W,PANEL_H,0xFFFFFFFF,  1.0f, 1.0f, 1);
			}
		}
		break;
	};
}



//******************************************************************************************
void	CJCLEngine::RenderGameState()
{

	float	x, y;
	SINT	i[4];
	float xWidth = PLATFORM.ScreenWidth();
	float yHeight = PLATFORM.ScreenHeight();

	CGameRenderInfo *gri = GAME.GetGameRenderInfo();

	if (gri->mActive & GRI_PROFILE)
	{
#ifdef _PROFILE
		CProfiler::ShowProfile();
#endif
	}
	
	
	
	// one player 
	if (GAME.GetNumPlayers() == 1 )
	{
		RenderPlayerState(GAME.GetPlayer(0), 0, PLATFORM.ScreenWidth() ) ;
	}
	// two players
	else
	{
		RenderPlayerState(GAME.GetPlayer(0), 0, PLATFORM.ScreenWidth() >> 1 ) ;
		RenderPlayerState(GAME.GetPlayer(1), PLATFORM.ScreenWidth() >> 1 , PLATFORM.ScreenWidth() >> 1) ;
	}
	
// render Fades.
	if ((gri->mActive & GRI_CIRCLE_FADE_IN) || 
		(gri->mActive & GRI_CIRCLE_FADE_OUT)||
		(gri->mActive & GRI_HELP))
	{
		x = float(xWidth  / 2);
		y = float(yHeight / 2);
		
		float	s = ((53.f - float(gri->mCircleFadeCount)) * 400.f) * PLATFORM.ScreenWidthScale(); //53
		
		DWORD col = 0xffffffff;
		
//#define CIRCLE_Z	0.015f
#define CIRCLE_Z	0.03f

		CTexture::DrawAllTextures();
		CTexture::DrawAllTexturesAdditive();

		// Render a square
		if (gri->mCircleFadeCount > 46) // nearly finished? //46
		{
			// draw black screen
			i[0] = mTexDied->AddVertex(&D3DTLVERTEX(D3DVECTOR(0    , 0    , CIRCLE_Z),1, col,D3DRGB(0,0,0), 0.0f, 0.0f));
			i[1] = mTexDied->AddVertex(&D3DTLVERTEX(D3DVECTOR(x * 2, 0    , CIRCLE_Z),1, col,D3DRGB(0,0,0), 0.1f, 0.0f));
			i[2] = mTexDied->AddVertex(&D3DTLVERTEX(D3DVECTOR(x * 2, y * 2, CIRCLE_Z),1, col,D3DRGB(0,0,0), 0.1f, 0.1f));
			i[3] = mTexDied->AddVertex(&D3DTLVERTEX(D3DVECTOR(0    , y * 2, CIRCLE_Z),1, col,D3DRGB(0,0,0), 0.0f, 0.1f));
		}
		else
		{
			// normal fade
			i[0] = mTexDied->AddVertex(&D3DTLVERTEX(D3DVECTOR(x - s, y - s, CIRCLE_Z + 0.01f),1, col,D3DRGB(0,0,0), -8.f, -8.f));
			i[1] = mTexDied->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + s, y - s, CIRCLE_Z + 0.01f),1, col,D3DRGB(0,0,0),  9.f, -8.f));
			i[2] = mTexDied->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + s, y + s, CIRCLE_Z + 0.01f),1, col,D3DRGB(0,0,0),  9.f,  9.f));
			i[3] = mTexDied->AddVertex(&D3DTLVERTEX(D3DVECTOR(x - s, y + s, CIRCLE_Z + 0.01f),1, col,D3DRGB(0,0,0), -8.f,  9.f));
		}
		
		mTexDied->AddTri(i[0], i[1], i[3]);
		mTexDied->AddTri(i[1], i[2], i[3]);

		//CTexture::DrawAllTextures();
		//CTexture::DrawAllTexturesAdditive();
		
//		mDevice->SetTextureStageState(0, D3DTSS_ADDRESS , D3DTADDRESS_CLAMP);
//		mTexDied->DrawAll();
//		mDevice->SetTextureStageState(0, D3DTSS_ADDRESS , D3DTADDRESS_WRAP);
	}
	


	// ********************************************************************
	// ********************************************************************
	// ** More state stuff
	
	// time limit - assume that this is the same for both players!
	CPrimary	*pr = GAME.GetPlayer(0)->GetPrimary();
	char		text[100];
	if (pr)
	{
		float	midx = (80 * PCMULTX);
		if (GAME.GetNumPlayers() == 2 )
			midx = float(PLATFORM.ScreenWidthFull() / 2);
		
		
		SINT 	yo = get_panel_y_offset();
		
		if (yo != -1)
		{		
			CTextureTL *tex = mClock;
			if (tex)
			{
				DrawSpritePC(tex, midx - (CLOCK_W / 2), float(20 - yo) * PCMULTY, 0.1f, CLOCK_W, CLOCK_H);
//				tex->DrawAll();
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
			
			DWORD 	col = 0xffffffff;
			
			if (tl != 0)
			{
				if ((secs < 10) && (mins == 0))
					col = 0xff1f1fff;
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
				DrawShadowedString(2, text, midx - (4.f * PCMULTX) - sl, float(32 - yo) * PCMULTY, col, 0.05f);
				
				sl = GetStringSize(2, ":");
				DrawShadowedString(2, ":", midx - (sl / 2), float(32 - yo) * PCMULTY, col, 0.05f);
				
				sprintf(text, "%02d", secs);
				DrawShadowedString(2, text, midx + (4 * PCMULTX), float(32 - yo) * PCMULTY, col, 0.05f);
			}
		}
	}
	
	// ********************************************************************
	//char	str[200];
	
	if (gri->mActive & GRI_START_LIGHTS)
	{
		SINT sx = STARTL_W;
		SINT sy = STARTL_H;
		float	x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);
		float 	y = float((gri->mStartLightsPos * G(sy + 25)).Whole() - sy - 5) * PCMULTY;

		SINT	cval = ((G1 - gri->mStartLightsOn.GFrac()) * 350).Whole();
		if (cval > 255) cval = 255;

		DWORD	col1 = (cval << 16) + (cval << 8) + (cval) + 0xff000000;
		cval = 256 - cval;
		DWORD	col2 = (cval << 16) + (cval << 8) + (cval) + 0xff000000;	
		
		SINT	c0;
		for (c0 = 0; c0 < 4; c0 ++)
		{
			DrawSpritePC(mTexStart1, x, y, 0.1f, sx, sy,col2);

			x += sx;
		}

		mTexStart1->DrawAll();
		mTexStart2->DrawAll();
		mTexStart3->DrawAll();

		x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);

		for (c0 = 0; c0 < 4; c0 ++)
		{
			CTextureTL *tex = mTexStart1;

			if (gri->mStartLightsOn >= G(c0 + 1))
			{                 
				if (c0 == 3)
					tex = mTexStart3;
				else
					tex = mTexStart2;
			}

			DrawSpritePC(tex, x, y, 0.1f, sx, sy,col1);

			x += sx;
		}

		ENGINE.EnableAdditiveAlpha();
		mTexStart1->DrawAll();
		mTexStart2->DrawAll();
		mTexStart3->DrawAll();
		ENGINE.DisableAlpha();
		// and the arms
		x = (PLATFORM.ScreenWidthFull() / 2) - (sx * 2);
		
		DrawSpritePC(mStartArm, x - sx      , y - (32 * PCMULTY), 0.1f, ARM_W, ARM_H);
		DrawSpritePC(mStartArm, x + (sx * 4), y - (32 * PCMULTY), 0.1f, ARM_W, ARM_H, 0xffffffff, 1.0f, 1.0f, 1);
	}




	// draw Nurbling information panel

	x = PLATFORM.ScreenWidth() - 50.f;
	y = 100.f;

/*
	// golf
	if (gri->mActive & GRI_GOLF_SHOT_NO)
	{
		x = GToF(gri->mGolfShotPos) * float(PLATFORM.ScreenWidth() / 2) + float(PLATFORM.ScreenWidth() / 2);
		y = float(PLATFORM.ScreenHeight() / 2);

		float	sx = 64.f;
		float	sy = 64.f;

		x -= sx / 2.f;

		// Render a square
		i[0] = mTexGolfShot->AddVertex(&D3DTLVERTEX(D3DVECTOR(x     , y     , 0),1, 0xffffff2f, D3DRGB(0,0,0),0.f ,0.f));
		i[1] = mTexGolfShot->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + sx, y     , 0),1, 0xffffff2f, D3DRGB(0,0,0),1.f ,0.f));
		i[2] = mTexGolfShot->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + sx, y + sy, 0),1, 0xffffff2f, D3DRGB(0,0,0),1.f ,1.f));
		i[3] = mTexGolfShot->AddVertex(&D3DTLVERTEX(D3DVECTOR(x     , y + sy, 0),1, 0xffffff2f, D3DRGB(0,0,0),0.f ,1.f));

		mTexGolfShot->AddTri(i[0], i[1], i[3]);
		mTexGolfShot->AddTri(i[1], i[2], i[3]);

//		mTexGolfShot->DrawAll();

		// and some numbers
		x += sx + 10.f;

		char	str[10];
		SINT	c0;

		sprintf(str, "%d", gri->mGolfShotNo);

		for (c0 = 0; c0 < SINT(strlen(str)); c0 ++)
		{
			DrawNumber(x, y, str[c0] - '0', 0xffffff2f);
			x += NUMBER_OFFSET;
		}

//		mTexNumbers->DrawAll();
	}
	*/
}

//******************************************************************************************
void	CJCLEngine::DrawIconSprite(CTextureTL *t, float x, float y, SINT number, DWORD col)
{
	SINT	i[4];

	float	u = float(number % 4) * 0.25f + 0.005f;
	float	v = float(number / 4) * 0.25f;	

	i[0] = t->AddVertex(&D3DTLVERTEX(D3DVECTOR(x              , y              , 0),1 , col, D3DRGB(0,0,0),u          , v          ));
	i[1] = t->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + NUMBER_SIZE, y              , 0),1 , col, D3DRGB(0,0,0),u + NUMBER_UV_SIZE, v          ));
	i[2] = t->AddVertex(&D3DTLVERTEX(D3DVECTOR(x + NUMBER_SIZE, y + NUMBER_SIZE, 0),1 , col, D3DRGB(0,0,0),u + NUMBER_UV_SIZE, v + NUMBER_UV_SIZE));
	i[3] = t->AddVertex(&D3DTLVERTEX(D3DVECTOR(x              , y + NUMBER_SIZE, 0),1 , col, D3DRGB(0,0,0),u          , v + NUMBER_UV_SIZE));

	t->AddTri(i[0], i[1], i[3]);
	t->AddTri(i[1], i[2], i[3]);

	// note - no DrawAll()...
}

//******************************************************************************************
void	CJCLEngine::DrawNumber(float x, float y, SINT number, DWORD col)
{
	DrawIconSprite(mTexNumbers, x, y, number, col);
}

//******************************************************************************************
void	CJCLEngine::DrawIcon(float x, float y, SINT number, DWORD col)
{
	DrawIconSprite(mTexIcons, x, y, number, col);
}

//******************************************************************************************
void	swapf(float &a, float &b)
{
	float foo = a;
	a = b;
	b = foo;
}

//******************************************************************************************
void	CJCLEngine::DrawSpritePC(CTextureTL* n, float px, float py, float z, float width, float height, D3DCOLOR col, float scalex, float scaley, SINT flip)
{
	SINT	i[4];

	ULONG	spec = 0;
//	float u1,u2,v1,v2;
	float x,y,w,h;

	// auto shadow?
	if (col & 0xffffff)
	{
		if (auto_shadow(n))
		{
			// draw again!
			if (GAME.mInFrontEnd)  // shadow more in frontend
				DrawSpritePC(n, px + 3, py + 2, z + 0.001f, width, height, 0xff000000, scalex, scaley, flip);
			else
				DrawSpritePC(n, px + 2, py + 2, z + 0.001f, width, height, 0xff000000, scalex, scaley, flip);
		}
	}

	x = px * scalex;
	w = width * scalex;
	y = py * scaley;
	h = height * scaley;
	
	float	us = 0.005f;
	float	ue = (float(w) / float(n->texw +1)); 
	float	vs = 1.0f - 0.005f;
	float	ve = (float(n->texh - h + 1) / float(n->texh));

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

	float invz = 1.0f / (z * 2.f);


	i[0] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x,	y,		z),	invz, col, spec, us, vs));
	i[1] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x+w,	y,		z),	invz, col, spec, ue, vs));
	i[2] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x+w,	y+h,	z),	invz, col, spec, ue, ve));
	i[3] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x,	y+h,	z),	invz, col, spec, us, ve));
	
	n->AddTri(i[0], i[1], i[3]);
	n->AddTri(i[1], i[2], i[3]);
}
//******************************************************************************************
void	CJCLEngine::DrawSpritePC2(CTextureTL* n, float px, float py, float z, float width, float height, D3DCOLOR col, float scalex, float scaley, SINT flip)
{
	SINT	i[4];

	ULONG	spec = 0;
//	float u1,u2,v1,v2;
	float x,y,w,h;

	x = px * scalex;
	w = width * scalex;
	y = py * scaley;
	h = height * scaley;
	
	float	us = 0.005f;
	float	ue = 1.0f - 0.005f;
	float	vs = 1.0f - 0.005f;
	float	ve = 0.005f;

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

	
	i[0] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x, y, z),1.0f/z, col, spec, us, vs));
	i[1] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x+w, y, z),1.0f/z, col, spec, ue, vs));
	i[2] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x+w, y+h, z),1.0f/z, col, spec, ue, ve));
	i[3] = n->AddVertex(&D3DTLVERTEX(D3DVECTOR(x, y+h, z),1.0f/z, col, spec, us, ve));
	
	n->AddTri(i[0], i[1], i[3]);
	n->AddTri(i[1], i[2], i[3]);
}

//******************************************************************************************
void	CJCLEngine::DrawBar(float x, float y, SINT num_bits, SINT colr, SINT colg, SINT colb, BOOL centre, SINT ot)
{
	float Z = (((float)ot) / ((float)255));

	DWORD col = RGB_MAKE(colr * 2, colg * 2, colb * 2);
	
	if (centre)
	{
		SINT	size = 64 * (num_bits + 2);
		x -= size / 2;
	}

	// Left side of menu bar
	DrawSpritePC(mTexBarLeft, x  , y, Z, 64, 64, col, 1.0f, 1.0f, 0);
	x += 63;

	// 'num_bits' middle sections
	for (SINT c1 = 0; c1 < num_bits; c1 ++) 
	{
		DrawSpritePC(mTexBarMiddle, x  , y, Z, 64, 64, col, 1.0f, 1.0f, 0);
		x += 63;
	}

	// Right side of bar,
	DrawSpritePC(mTexBarRight, x  , y, Z, 64, 64, col, 1.0f, 1.0f, 0);

	// Force drawing of bar now.
	mTexBarLeft->DrawAll();
	mTexBarMiddle->DrawAll();
	mTexBarRight->DrawAll();

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
SINT	CJCLEngine::DrawChar(SINT font, UINT c, float x, float y, DWORD col, float z)
{
	font = 2;

	// remap international characters
	c = eval_char(c);

	if (GAME.mInFrontEnd)
	{
//		sort_out_colour(col);
	}
	else
		sort_out_colour2(col);


	// Now re-written to use the structure above. It's a SPRT primitive with TPage/ABR control
	// 4 longwords shorter with only x/y/w/h fields instead of 4 uv coords and 4 xy coords
	//!PSX (jcl) this code is ridiculous!  ah well..

	SPCFont	*f  = &mFonts[font];
	STCFont		*fi = &f->f;  // nice clear code!

	if (x >= 0)	// sorry - had to do this.
	{
		SINT	i[4];

		SINT	cw = fi->mCharWidth;
		SINT	ch = fi->mCharHeight;

		// colour
		col &= 0x00ffffff;
		col |= 0x64000000;
		SINT	row    = c / fi->mCharsPerLine;
		SINT	column = c - row * fi->mCharsPerLine;
		float	bx = (column * cw);
		float	by = (row    * ch);
		float	x1 = bx/f->mX + 0.002f;
		float	y1 = (f->mY-by)/f->mY;
		float	x2 = (bx+cw)/f->mX;
		float	y2 = (f->mY-(by+ch))/f->mY;
		

		DWORD spec = 0;
		i[0] = mFonts[font].font->AddVertex(&D3DTLVERTEX(D3DVECTOR(x   , y   , z),1, col, spec, x1, y1 - 0.002f));
		i[1] = mFonts[font].font->AddVertex(&D3DTLVERTEX(D3DVECTOR(x+cw, y   , z),1, col, spec, x2, y1 - 0.002f));
		i[2] = mFonts[font].font->AddVertex(&D3DTLVERTEX(D3DVECTOR(x+cw, y+ch, z),1, col, spec, x2, y2 + 0.002f));
		i[3] = mFonts[font].font->AddVertex(&D3DTLVERTEX(D3DVECTOR(x   , y+ch, z),1, col, spec, x1, y2 + 0.002f));

		/*
		i[0] = mFonts[font].font->AddVertex(&D3DTLVERTEX(D3DVECTOR(x, y, z),1, col, spec, x1, y1));
		i[1] = mFonts[font].font->AddVertex(&D3DTLVERTEX(D3DVECTOR(x+cw, y, z),1, col, spec, x2, y1));
		i[2] = mFonts[font].font->AddVertex(&D3DTLVERTEX(D3DVECTOR(x+cw, y+ch, z),1, col, spec, x2, y2));
		i[3] = mFonts[font].font->AddVertex(&D3DTLVERTEX(D3DVECTOR(x, y+ch, z),1, col, spec, x1, y2));
		*/
	
		mFonts[font].font->AddTri(i[0], i[1], i[3]);
		mFonts[font].font->AddTri(i[1], i[2], i[3]);
	}
	return fi->mWidths[c];
}
//******************************************************************************************
void	CJCLEngine::DrawString(SINT font, char *s, float x, float y, DWORD col, float z,BOOL HasBorder)
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
			xx += DrawChar(font, *((unsigned char *)c) - 32, xx, y, col, z) + 2;
			break;
		};

		c++;
	};

	if(HasBorder)
	{
		DrawString(font, s, x+1, y+1, 0 ,z-0.001f,FALSE);
		DrawString(font, s, x-1, y+1, 0 ,z-0.001f,FALSE);
		DrawString(font, s, x-1, y-1, 0 ,z-0.001f,FALSE);
		DrawString(font, s, x+1, y-1, 0 ,z-0.001f,FALSE);
	}
}

//******************************************************************************************
void	CJCLEngine::DrawShadowedString(SINT font, char *s, float x, float y, DWORD col, float z, BOOL HasBorder)
{
	DrawString(font, s, x+1, y+1, 0xff000000, z+0.001f, HasBorder);
	DrawString(font, s, x, y, col, z, HasBorder);
	//mFonts[font].font->DrawAll();
}

//******************************************************************************************
SINT	CJCLEngine::GetStringSize(SINT font, char *s)
{
	font = 2;

	SINT 		l = 0;
	SPCFont	*f  = &mFonts[font];
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
#endif
