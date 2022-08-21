#ifndef DCFE_PRISON_SELECT_H
#define DCFE_PRISON_SELECT_H

#include	"Camera.h"
#include	"DCFrontEndThing.h"
#include	"Maincode.h"
#include	"DCMemCard.h"

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

class	CDCFEPrisonSelect
{
public:
	friend class	CDCFELanguageSelect;

	CDCFEPrisonSelect() : mAttract(0) {};

	SINT	Run(EPSEntryType entrytype);
	void	Reset(EPSEntryType entrytype);

protected:
//	BOOL	LoadData();
	BOOL	LoadDataIntoCache(SINT n);
	BOOL	LoadDataFromCache();	
	
	SINT	EvaluateSingleLevelPerformance();
	
	void	Process();
	void	ProcessParticles();
	
	void	AddParticles(SINT x, SINT y, SINT num, SINT type = 0);
	
	void	DrawSprite(SINT n, SINT px, SINT py, float otdepth, SINT r = 127, SINT g = 127, SINT b = 127, SINT semitrans = -1, BOOL grey = FALSE, GINT scale = G1, SINT flip = 0);
	void	DrawSpriteXY(SINT n, SINT px1, SINT py1, SINT px2, SINT py2, SINT px3, SINT py3, SINT px4, SINT py4, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127, SINT semitrans = -1, BOOL grey = FALSE);
	void	DrawBorder(SINT x, SINT y, SINT w, SINT h, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127);
	void	DrawBorderClip(SINT x, SINT y, SINT w, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127);
	void	DrawBorderClipRot(SINT s1, SINT s2, SINT x, SINT y, SINT w, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127, GINT rot = G0, BOOL grey = FALSE);
	void	DrawBox(SINT x, SINT y, SINT w, SINT h, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127);
	void	DrawLine(SINT sx, SINT sy, SINT ex, SINT ey, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127);
	
	void	DrawDynamicText(SINT fn, char *text, float x, float y, SINT r, SINT g, SINT b, SINT time, SINT otdepth = 1);
	void	DrawString(SINT font, char *s, float x, float y, DWORD col, float otdepth = 1,BOOL HasBorder=FALSE);
	SINT	GetStringSize(SINT font, char *s);

	void		Render();
	
	void	LoadAndRenderStart();
	
	void	RenderMain();
	void	RenderSelMem();
	void	RenderChar();
	void	RenderPrisonNew();
	void	RenderArena();
	void	RenderPS2Game();
	void	RenderQSave();
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
	}	mQuit;
	
	CCamera	mCamera;
	
	SINT	mCounter;
	
	BOOL	mDBX, mDBY;
	BOOL	mDBX2, mDBY2;
	
	enum	EFEPSMode
	{
		FEPS_START,
		FEPS_START2MAIN,
		FEPS_START2SELMEM,
				
		FEPS_SELMEM,
		FEPS_SELMEM2MAIN,
		FEPS_SELMEM2OPTIONS,
		FEPS_SELMEM2ARENA,

		FEPS_MAIN,
		FEPS_MAIN2CHAR,
		FEPS_MAIN2START,
		FEPS_MAIN2OPTIONS,
		FEPS_OPTIONS2MAIN,
		
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
		FEPS_QSAVE2SELMEM,
		
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
		FEPS_OPTIONS2SELMEM,
		
		FEPS_CONTROLLER,
		FEPS_OPTIONS2CONTROLLER,
		FEPS_CONTROLLER2OPTIONS,
		
		FEPS_CREDITS,
		FEPS_MAIN2CREDITS,
		FEPS_CREDITS2MAIN,
		
	} mMode;
	
	enum	EMemSelState
	{
		EMS_SELECT_CARD,
		EMS_LOADING,
		EMS_SAVING,
		EMS_LOAD_FAILED,
		EMS_SAVE_FAILED,
		EMS_SAVE_LOAD,
		EMS_SAVE_YN,
		EMS_LOAD_YN,
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
	BOOL	mMainMenuFromSelMem;
	
	
	SINT			mSelMemMenuSlide;
	EMemSelState	mSelMemMenuState;
	SINT			mSelMemCard;
	SINT			mSelMemPort;
	EFEPSMode		mSelMemWhereFrom;
	SINT			mSelMemSLPos;
	SINT			mSelMemYNPos;
	SINT			mSelMemFailedPos;
	
	struct
	{
		VMUState	mState;
		SINT		mBlocksFree;
		BOOL		mSavePresent;
	} mSelMemVMS[4][2];
	
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
	SINT	mQSaveCounter;
	
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
	BOOL	mOptionsStereo;
	
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
	
	char	mLastDrawnString[1000];
	void	ResetLastDrawnString() {mLastDrawnString[0] = 0;};
};

extern CDCFEPrisonSelect	PRISONSELECT;

#endif