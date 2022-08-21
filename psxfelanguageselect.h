#ifndef PSXFE_LANGAUGE_SELECT_H
#define PSXFE_LANGUAGE_SELECT_H

#include	"Camera.h"
#include	"PSXFrontEndThing.h"

//******************************************************************************************

class	CPSXFELanguageSelect
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
};

extern CPSXFELanguageSelect	LANGUAGESELECT;

#endif