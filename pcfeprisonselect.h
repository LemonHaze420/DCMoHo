#ifndef PCFE_PRISON_SELECT_H
#define PCFE_PRISON_SELECT_H

#include	"Camera.h"
#include	"PCFrontEndThing.h"
#include	"pcplatform.h"

//******************************************************************************************

#define NUM_PSXFE_PARTICLES	200

enum	EPSEntryType
{
	PSE_TOP,
	PSE_SINGLE,
	PSE_SINGLE_WIN,
	PSE_MULTIPLAYER,
	PSE_MULTIPLAYER_FINISHED,
};

class	CPCFEPrisonSelect
{
public:
	friend class CPCGame;
	friend class CJCLEngine;

	CPCFEPrisonSelect() : mAttract(0) {}
	
	SINT	Run(EPSEntryType entrytype);

protected:
	BOOL	LoadData();
	
	SINT	EvaluateSingleLevelPerformance();
	
	void	Process();
	void	ProcessParticles();
	
	friend HRESULT __cdecl SaveGameKeyCaptureCallback(UBYTE ascii , UINT keydata);
	
	void	AddParticles(SINT x, SINT y, SINT num, SINT type = 0);
	
	void	DrawSpritePC(CTextureTL* n, float px, float py, float z, float width, float height, D3DCOLOR col, BOOL grey = FALSE, float scalex = PCMULTX, float scaley = PCMULTY, SINT flip = 0);
	void	DrawSpritePCAdditive(CTextureTLAdditive* n, float px, float py, float z, float width, float height, D3DCOLOR col, BOOL grey = FALSE, float scalex = PCMULTX, float scaley = PCMULTY, SINT flip = 0);
	void	DrawSpriteXY(SINT n, SINT px1, SINT py1, SINT px2, SINT py2, SINT px3, SINT py3, SINT px4, SINT py4, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127, SINT semitrans = -1, BOOL grey = FALSE);
	void	DrawBorder(float x, float y, float w, float h, float z, SINT r = 127, SINT g = 127, SINT b = 127);
	void	DrawBorderClip(float x, float y, float w, float z, SINT r = 127, SINT g = 127, SINT b = 127);
	void	DrawBorderClipRot(SINT s1, SINT s2, float x, float y, float w, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127, GINT rot = G0, BOOL grey = FALSE);
	void	DrawBox(float x, float y, float w, float h, float z, SINT r = 127, SINT g = 127, SINT b = 127);
	void	DrawLine(float sx, float sy, float ex, float ey, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127);
	void	DrawRect(float sx, float sy, float ex, float ey, SINT otdepth, SINT r = 255, SINT g = 255, SINT b = 255);
	
	void	DrawDynamicText(SINT fn, char *text, float x, float y, SINT r, SINT g, SINT b, SINT time, float z = 0.11f);
	
	void		Render();
	
	void	LoadAndRenderStart();
	
	void	RenderStart();
	void	RenderMain();
	void	RenderConfirmQuit();
	void	RenderSingle();
	void	RenderAbandon();
	void	RenderLoad();
	void	RenderChar();
	void	RenderPrisonNew();
	void	RenderArena();
	void	RenderPS2Game();
	void	RenderQSave();
	void	RenderSave();
	void	RenderMessage();
	void	RenderCredits();

	void	RenderMChar();
	void	RenderMTourn();

	void	RenderOptions();
	void	RenderController();
	
	void	RenderParticles();

	enum	EPSXFEQuitTypes
	{
		EPSXFE_NONE = 0,
		EPSXFE_SINGLE,
		EPSXFE_MULTI,
		EPSXFE_ATTRACT,
		EPSXFE_EXIT,
	}	mQuit;
	
	CCamera	mCamera;
	
	SINT	mCounter;
	
	BOOL	mDBX, mDBY;
	BOOL	mDBX2, mDBY2;
	
	enum	EFEPSMode
	{
		FEPS_START,
		FEPS_START2MAIN,
		FEPS_MAIN2START,
		FEPS_MAIN2OPTIONS,
		FEPS_OPTIONS2MAIN,

		FEPS_CONFIRMQUIT,
		FEPS_MAIN2CONFIRMQUIT,
		FEPS_CONFIRMQUIT2MAIN,

		FEPS_MAIN,
		FEPS_MAIN2SINGLE,
		FEPS_SINGLE2MAIN,
		
		FEPS_SINGLE,
		FEPS_SINGLE2CHAR,
		FEPS_SINGLE2LOAD,
		FEPS_SINGLE2ABANDON,
		
		FEPS_ABANDON,
		FEPS_ABANDON2MAIN,
		FEPS_ABANDON2CHAR,
		
		FEPS_LOAD,
		FEPS_LOAD2MAIN,
		FEPS_LOAD2CHAR,

		FEPS_CHAR,
		FEPS_CHAR2MAIN,
		FEPS_CHAR2PSNEW,		
		FEPS_PSNEW2CHAR,
		
		FEPS_PSNEW,	
		
		FEPS_ARENA,
		FEPS_PSNEW2ARENA,
		FEPS_ARENA2PSNEW,		
		
		FEPS_PS2GAME,
		
		FEPS_ARENA2QSAVE,
		FEPS_QSAVE,
		FEPS_QSAVE2ARENA,
		
		FEPS_QSAVE2SAVE,
		FEPS_SAVE,
		FEPS_SAVE2ARENA,
		
		FEPS_ARENA2MESSAGE,
		FEPS_MESSAGE,
		FEPS_MESSAGE2ARENA,
		
		FEPS_MAIN2MCHAR,
		FEPS_MCHAR2MAIN,
		FEPS_MCHAR,
		
		FEPS_MCHAR2MTOURN,
		FEPS_MTOURN2MCHAR,
		FEPS_MTOURN,
		
		FEPS_TS2GAME,
		
		FEPS_OPTIONS,
		FEPS_OPTIONS2SAVE,
		FEPS_SAVE2OPTIONS,
		
		FEPS_CONTROLLER,
		FEPS_OPTIONS2CONTROLLER,
		FEPS_CONTROLLER2OPTIONS,
		
		FEPS_CREDITS,
		FEPS_MAIN2CREDITS,
		FEPS_CREDITS2MAIN,
		
	} mMode;
	
	enum	ELoadState
	{
		ELS_START,
		ELS_SCANNING,
		ELS_NONE_FOUND,
		ELS_SAVES_FOUND,
		ELS_LOADING,
		ELS_LOADED,
		ELS_FAILED,
		ELS_INSERT,
		ELS_WRONG_VERSION,
		ELS_UNFORMATTED,
	};
	
	enum	ESaveState
	{
		ESS_ENTER_NAME,
		ESS_SCANNING,
		ESS_SAVING,
		ESS_NO_CARD,
		ESS_UNFORMATTED,
		ESS_FORMATTING,
		ESS_NO_SPACE,
		ESS_FAILED,
		ESS_SAVED,
		ESS_FORMAT_FAILED,
		ESS_NAME_TAKEN,
	};
	
	GMatrix	mTopMatrix;
	GMatrix mInMatrix;
	GVector	mTopPos;
	GVector mInPos;
	
	GINT	mZoomVal;
	GINT	mRadius;
	
	SINT	mArrowCounter;
	
	SINT	mMainMenuPos;
	SINT	mMainMenuSlide;

	SINT	mConfirmQuitMenuPos;
	SINT	mConfirmQuitMenuSlide;

	SINT	mSingleMenuPos;
	SINT	mSingleMenuSlide;
	
	SINT	mAbandonMenuPos;
	SINT	mAbandonMenuSlide;

	SINT		mLoadMenuSlide;
	SINT		mLoadMenuPos;
	SINT		mLoadMenuScrollPos;
	ELoadState	mLoadMenuState;

	SINT	mCharacter;
	SINT	mCharMenuSlide;
	GINT	mCharSlide;
	SINT	mCharSlideStart;
	SINT	mCharRotTimer;
	SINT	mCharWait;
	CFrontEndThing	mCharThings[6];
	
	SINT	mPrison;
	SINT	mPrisonMenuSlide;	
	GINT	mPrisonSlide;
	SINT	mPrisonSlideStart;
	BOOL	mPrisonAccessible;
	SINT	mPrisonAccessStart;
	GINT	mPrisonBonusSlide;
	BOOL	mPrisonBonus;
	
	SINT	mArenaMenuSlide;
	SINT	mArena;
	SINT	mArenaSlideStart;
	SINT	mArenaPendingVictory;
	SINT	mArenaPendingVictoryType;
	SINT	mArenaMessages[10];
	SINT	mArenaNumMessages;
	SINT	mArenaCurrentMessage;
	
	SINT	mQSaveMenuSlide;
	SINT	mQSaveMenuPos;
	
	bool		mNeedToUpdateSave;
	SINT		mSaveMenuSlide;
	SINT		mSaveMenuPos;
	ESaveState	mSaveMenuState;
	SINT		mSaveX;
	SINT		mSaveY;
	SINT		mSaveNameLen;
	BOOL		mSaveFromArena;
	BOOL		mSaveEnteredName;
	SINT		mSaveConfirmOverWriteMenuPos;
	
	SINT	mMessageMenuSlide;
	SINT	mMessage;
	
	SINT	mMCharacter[2];
	SINT	mMCharMenuSlide;
	GINT	mMCharSlide[2];
	SINT	mMCharSlideStart[2];
	SINT	mMCharRotTimer;
	BOOL	mMCharSelected[2];	
	SINT	mMCharWait;
	CFrontEndThing	mMCharThings[2][6];
	
	SINT	mMTournament;
	SINT	mMTournamentMenuSlide;	
	GINT	mMTournamentSlide;
	SINT	mMTournamentSlideStart;
	BOOL	mMTournamentAccessible;
	SINT	mMTournamentAccessStart;
	SINT	mMTournamentResultsTimer;
	SINT	mMTournamentResultsShowLW;
	BOOL	mMTournamentResultsShowWinner;

	SINT	mOptionsMenuPos;
	SINT	mOptionsMenuSlide;
	
	SINT	mControllerMenuSlide;
	SINT	mControllerSetting1;
	SINT	mControllerSetting2;
	
	SINT	mCreditsMenuSlide;
	SINT	mCreditsPos;
		
	struct	SPSXFEParticle
	{
		GINT	mX, mY;
		GINT	mVX, mVY;
		SINT	mVal;
		SINT	mType;
	}	mParticles[NUM_PSXFE_PARTICLES];
	
	SINT	mLightningSX;
	SINT	mLightningSY;
	SINT	mLightningEX;
	SINT	mLightningEY;
	SINT	mLightningPoint;
	SINT	mLightningTime;
	
	SINT	mAttract;
	SINT	mAttractCounter;
	
//	CTextureTL			*mTexSplash1;
//	CTextureTL			*mTexSplash2;
//	CTextureTL			*back;

	CTextureTL				*mTexSplash[6];
	CTextureTL				*mTexBack[6];
							
	CTextureTL				*title[5];
	CTextureTL				*boxl;
	CTextureTL				*boxm;
	CTextureTL				*boxr;
	CTextureTL				*arrow_l;
	CTextureTL				*arrow_r;
	CTextureTL				*arrow_u;
	CTextureTL				*arrow_d;
	CTextureTLAdditive		*spark;
	CTextureTL				*qualify;
	CTextureTLAdditive		*halo; // !!!!!!!!!!
	CTextureTL				*draw;
	CTextureTL				*gold;
	CTextureTL				*goldlt;
	CTextureTL				*selchar;
	CTextureTL				*framctl;
	CTextureTL				*framctr;
	CTextureTL				*framcbl;
	CTextureTL				*framcbr;
	CTextureTL				*frampt;
	CTextureTL				*frampr;
	CTextureTL				*frampb;
	CTextureTL				*frampl;
	CTextureTL				*clipl;
	CTextureTL				*clipr;
	CTextureTL				*ps[11];
	CTextureTL				*ar[11];
	CTextureGTEXAdditive	*lightning;
	CTextureTLAdditive		*felightning;
	CTextureTL				*white;
};

extern CPCFEPrisonSelect	PRISONSELECT;

#endif
