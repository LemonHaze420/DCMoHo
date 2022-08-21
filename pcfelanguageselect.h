#ifndef PCFE_LANGAUGE_SELECT_H
#define PCFE_LANGUAGE_SELECT_H

#include	"Camera.h"
#include	"PCFrontEndThing.h"

//******************************************************************************************

class	CPCFELanguageSelect
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
	CTextureTL		*back;
	CTextureTL		*titlea;
	CTextureTL		*titleb;
};

extern CPCFELanguageSelect	LANGUAGESELECT;

#endif