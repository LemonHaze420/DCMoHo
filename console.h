#ifndef CONSOLE_H
#define	CONSOLE_H

#define CONSOLE_MAX_LINES	30
#define CONSOLE_LINE_SIZE	80

class	CConsole
{
public:
	CConsole();
	~CConsole() {};

	void	AddString(char *s);
	char	*GetString(SINT n);

	void	Render();

protected:
	char	mStrings[CONSOLE_MAX_LINES][CONSOLE_LINE_SIZE];
	SINT	mCurrentString;

	SINT	mLastTextTime;
};

#endif