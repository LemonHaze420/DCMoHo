#ifndef CLIPARAMS_H
#define CLIPARAMS_H

class	CCLIParams
{
public:
	CCLIParams()
	{
		mLevel = 0;
		mController = 0;
		mJoyMode = 0;
		mStartCharacter = 0;
	};
	~CCLIParams() {};

	static	void	GetParams(char *text, CCLIParams &p);

	SINT	mLevel;
	SINT	mController;
	SINT	mJoyMode;
	SINT	mStartCharacter;

};


#endif