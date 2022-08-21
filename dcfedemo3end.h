#ifndef PSXFEDEMO3END_H
#define PSXFEDEMO3END_H

//******************************************************************************************

class	CPSXFEDemo3End
{
public:
	SINT	Run(SINT time, SINT level);

protected:
	BOOL	LoadData();
	
	void	Process();
	void	DrawSprite(SINT n, SINT px, SINT py, SINT otdepth, SINT r = 127, SINT g = 127, SINT b = 127, SINT semitrans = -1, BOOL grey = FALSE, GINT scale = G1, SINT flip = 0);
	void	Render();
	
	BOOL	mDBX, mDBY;
	SINT	mQuit;
	SINT	mCounter;
	
	SINT	mTime, mLevel, mKey;

	CCamera	mCamera;
};

extern CPSXFEDemo3End	FEENDDEMO3;

#endif

