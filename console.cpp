#include	"Common.h"

#include	"Globals.h"

//******************************************************************************************
CConsole::CConsole()
{
	SINT	c0;

	for(c0 = 0; c0 < CONSOLE_MAX_LINES; c0++)
	{
		mStrings[c0][0] = 0;
	}
	mCurrentString = 0;
	mLastTextTime = -100;
}

//******************************************************************************************
void	CConsole::AddString(char *s)
{
	mCurrentString++;
	if (mCurrentString > CONSOLE_MAX_LINES - 1)
		mCurrentString = 0;

	my_strncpy(mStrings[mCurrentString], s, CONSOLE_LINE_SIZE);
	mStrings[mCurrentString][CONSOLE_LINE_SIZE - 1] = 0; // security

	mLastTextTime = GAME.GetGameTurn();
}

//******************************************************************************************
char	*CConsole::GetString(SINT n)
{
	SINT v = mCurrentString - n;
	while (v < 0)
	{
		v += CONSOLE_MAX_LINES;
	};

	return mStrings[v];
}

//******************************************************************************************
//******************************************************************************************

void	CConsole::Render()
{
	return;

#if TARGET == PC
	// draw?

	if (mLastTextTime < GAME.GetGameTurn() - 50)
	{
		mLastTextTime = -100;
		return;
	}

	HDC		hdc = PLATFORM.GetDPWin()->GetDC(); // ####!!!!
	SetBkMode(hdc, TRANSPARENT);

	SINT	c0;
	for (c0 = 0; c0 < 4; c0++)
	{
		SINT	size = strlen(CONSOLE.GetString(c0));

		SetTextColor(hdc, RGB(0,0,0));
		TextOut(hdc, 10 + 2, 440 - 15*c0 + 2, CONSOLE.GetString(c0), size);
		SetTextColor(hdc, RGB(30, 200, 100));
		TextOut(hdc, 10    , 440 - 15*c0    , CONSOLE.GetString(c0), size);
	}
	PLATFORM.GetDPWin()->ReleaseDC(hdc); //####!!!!
#elif TARGET == PSX
	//!PSX draw console here

#endif
}
