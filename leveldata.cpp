#include	"Common.h"

#include	"Globals.h"
#include	"LevelData.h"


CLevelData::CLevelData()
{
	Reset();
}

//******************************************************************************************
void	CLevelData::Reset()
{
	mIVars[IV_RAINDROP_FREQUENCY] = 0;  // none

	mGVars[WATER_HEIGHT] = G0;  // no water

	mWaterBackup = G0;
}

//******************************************************************************************
//******************************************************************************************

UINT	LD_VER = (MKID("1.10"));

BOOL	CLevelData::Read(CMEMBUFFER &mem)
{
//#if TARGET == PSX
//	printf("About to read level data\n");
//#endif

	// clear variables
	SINT	c0;

	for (c0 = 0; c0 < NUM_LEVEL_IVARS; c0 ++)
		mIVars[c0] = 0;
	for (c0 = 0; c0 < NUM_LEVEL_GVARS; c0 ++)
		mGVars[c0] = G0;


	// set number players to 1 

	mIVars[IV_NUMBER_PLAYERS] = 1 ;

	// get version number
	UINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson
	if (ver == MKID("1.10"))
	{
		SINT	i;

		// read number of integer vars
		mem.Read(&i, 4);

		for (c0 = 0; c0 < i; c0 ++)
		{
			mem.Read(&mIVars[c0], 4);

//#if TARGET == PSX
//			printf("Int var %d : %d \n", c0, mIVars[c0]);
//#endif
		}

		// read number of GINT vars
		mem.Read(&i, 4);

		for (c0 = 0; c0 < i; c0 ++)
		{
			mem.Read(&mGVars[c0], 4);
			if (GABS(mGVars[c0]) > G(1000))
				mGVars[c0] = G0;
		}

		// Backup water
		mWaterBackup = mGVars[WATER_HEIGHT];

		return TRUE;
	}
	else	if (ver == MKID("1.00"))
	{
		GAME.DeclareLevelDataOld();
		SINT	i;

		// read number of vars
		mem.Read(&i, 4);

		for (c0 = 0; c0 < i; c0 ++)
			mem.Read(&mIVars[c0], 4);

		return TRUE;
	}
	else return FALSE; // didn't recognise the verson
}

//******************************************************************************************
BOOL	CLevelData::Write(CMEMBUFFER &mem)
{
	// write out version
	mem.Write(&LD_VER, 4);

	// write out number of int vars
	SINT	i = NUM_LEVEL_IVARS;
	mem.Write(&i,4);

	SINT	c0;

	for (c0 = 0; c0 < i; c0 ++)
		mem.Write(&mIVars[c0], 4);

	// write out number of GINT vars
	i = NUM_LEVEL_GVARS;
	mem.Write(&i,4);

	for (c0 = 0; c0 < i; c0 ++)
		mem.Write(&mGVars[c0], 4);

	// all done

	return TRUE;
}
