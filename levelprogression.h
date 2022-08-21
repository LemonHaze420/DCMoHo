#ifndef LEVEL_PROGRESSION_H
#define LEVEL_PROGRESSION_H

#include	"MemBuffer.h"

#define MAX_LP_LEVELS	256
#define MAX_LP_LINKS	512

class	CLPLevel
{
public:
	SINT	mX, mY;
	SINT	mLevel;
	BOOL	mActive;
	BOOL	mSelectable;
};

class	CLPLink
{
public:
	SINT	mA, mB;
};


class	CLevelProgression
{
public:
	CLevelProgression() {mNumLevels = 0; mNumLinks = 0;};

	BOOL		Read(CMEMBUFFER &mem);
	BOOL		Write(CMEMBUFFER &mem);

	CLPLevel	mLevels[MAX_LP_LEVELS];
	CLPLink		mLinks[MAX_LP_LINKS];

	SINT		mNumLevels;
	SINT		mNumLinks;
};

#endif