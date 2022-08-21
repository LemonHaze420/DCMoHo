#ifndef DCFE_LANGAUGE_SELECT_H
#define DcFE_LANGUAGE_SELECT_H

#include	"Camera.h"
#include	"DCFrontEndThing.h"

//******************************************************************************************

enum	EFELSS
{
	FESS_LANGUAGE,
	FESS_ASKON,
	FESS_ASK,
	FESS_TEST1,
	FESS_TEST2,
	FESS_ASK2,
};

class	CDCFELanguageSelect
{
public:
	SINT	Run();

protected:
	BOOL	LoadData();
	
	SINT	EvaluateSingleLevelPerformance();
	
	void	Process();
	void	Render();
	
	CCamera	mCamera;
	
	SINT	mLanguage;
	BOOL	mDBX, mDBY;
	SINT	mQuit;
	SINT	mCounter;
	
	CFrontEndThing	mFlags[5];
	
	EFELSS	mState;
	SINT	mAskPos;
	SINT	mAsk2Pos;
	
	SINT	mAskMenuSlide;
};

extern CDCFELanguageSelect	LANGUAGESELECT;

#endif