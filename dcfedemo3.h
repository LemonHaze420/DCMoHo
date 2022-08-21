#ifndef PSXFEDEMO3_H
#define PSXFEDEMO3_H

enum	EDemoEntryType
{
	DET_TOP,
	DET_SINGLE,
	DET_SINGLE_WIN,
};

//******************************************************************************************

class	CPSXFEDemo3
{
public:
	SINT	Run(BOOL load);

protected:
	BOOL	LoadData();
	
	void	Process();
	void	DrawSprite(SINT n, SINT px, SINT py, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127, SINT semitrans = -1, BOOL grey = FALSE, GINT scale = G1, SINT flip = 0);
	void	Render();
	
	BOOL	mDBX, mDBY;
	SINT	mQuit;
	SINT	mCounter;
	
	SINT	mLevel;

	CCamera	mCamera;
};

extern CPSXFEDemo3	FEDEMO3;

#endif

