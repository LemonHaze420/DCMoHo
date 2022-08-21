#ifndef LEVELDATA_H
#define LEVELDATA_H

#include	"MemBuffer.h"

// don't change these

#define		LT_NORMAL		0
#define		LT_GOLF			1
#define		LT_SHUTTLE		2
#define		LT_RACER		3
#define		LT_DIRECT		4
#define		LT_LASTMAN		5
#define		LT_GAUNTLET		6
#define		LT_PURSUIT		7
#define		LT_TAG			8
#define		LT_KING			9
#define		LT_DROID_BALL	10 

#define		LT_MAX		11
#define		NO_LAPS_MAX 100

#define		TIME_LIMIT_MAX 2000

#define		NUM_PRISONS	11

enum	ELevelIVariables
{
	IV_RAINDROP_FREQUENCY,
	IV_LEVEL_TYPE,
	IV_TIME_LIMIT,
	IV_NUM_LAPS_FOR_RACE,
	IV_NUM_CHECKPOINTS,
	IV_NUMBER_PLAYERS,
	IV_POINTS_REQUIRED_TO_WIN_LEVEL,
	IV_PRISON_NUMBER,
	NUM_LEVEL_IVARS
};

enum	ELevelFVariables
{
	WATER_HEIGHT,

	NUM_LEVEL_GVARS
};

class	CLevelData
{
public:
	CLevelData();
	~CLevelData() {};

	void	Reset();

	// variables

	SINT	mIVars[NUM_LEVEL_IVARS];
	GINT	mGVars[NUM_LEVEL_GVARS];

	// object creators

	
	// read/write
	BOOL	Read(CMEMBUFFER &mem);
	BOOL	Write(CMEMBUFFER &mem);
	
	void	ToggleWater()
	{
		if (mGVars[WATER_HEIGHT] == mWaterBackup)
			mGVars[WATER_HEIGHT] = G0;
		else
			mGVars[WATER_HEIGHT] = mWaterBackup;
	};		
	
protected:
	GINT	mWaterBackup;

};

#endif