#include	"Common.h"

#include	"LevelProgression.h"

#define		LP_VERSION	1

//******************************************************************************************
BOOL	CLevelProgression::Read(CMEMBUFFER &mem)
{
	// get version number
	SINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson
	if (ver == 1)
	{
		SINT	c0;

		mem.Read(&mNumLevels, 4);

		if ((mNumLevels < 0) || (mNumLevels > MAX_LP_LEVELS))
			return FALSE;

		for (c0 = 0; c0 < mNumLevels; c0 ++)
		{
			mem.Read(&mLevels[c0].mX, 4);
			mem.Read(&mLevels[c0].mY, 4);
			mem.Read(&mLevels[c0].mLevel, 4);
			mLevels[c0].mActive = FALSE;
			mLevels[c0].mSelectable = FALSE;
		}

		mem.Read(&mNumLinks, 4);
		if ((mNumLinks < 0) || (mNumLinks > MAX_LP_LINKS))
			return FALSE;

		for (c0 = 0; c0 < mNumLinks; c0 ++)
		{
			mem.Read(&mLinks[c0].mA, 4);
			mem.Read(&mLinks[c0].mB, 4);
		}
	}

	return TRUE;
}

//******************************************************************************************
BOOL	CLevelProgression::Write(CMEMBUFFER &mem)
{
	SINT	c0;

	c0 = LP_VERSION;
	mem.Write(&c0, 4);

	mem.Write(&mNumLevels, 4);

	for (c0 = 0; c0 < mNumLevels; c0 ++)
	{
		mem.Write(&mLevels[c0].mX, 4);
		mem.Write(&mLevels[c0].mY, 4);
		mem.Write(&mLevels[c0].mLevel, 4);
	}

	mem.Write(&mNumLinks, 4);

	for (c0 = 0; c0 < mNumLinks; c0 ++)
	{
		mem.Write(&mLinks[c0].mA, 4);
		mem.Write(&mLinks[c0].mB, 4);
	}

	return TRUE;
}