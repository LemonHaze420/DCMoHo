#ifndef LEVELINFO_H
#define LEVELINFO_H

#define	MAX_LEVELS_PER_PRISON		10

#define	MAX_TOURNAMENTS				20
#define	MAX_LEVELS_PER_TOURNAMENT	5

enum	ELILevelType
{
	LILT_KING_OF_THE_HILL = 0,
	LILT_LAST_MAN_ROLLING,
	LILT_RUN_THE_GAUNTLET,
	LILT_PURSUIT,
	LILT_RACE,
	LILT_TRICK,
	
	LILT_POWERBALL,
	
	LILT_GOLF,
	LILT_PINBALL,
	LILT_SBALL,
};

#define LIFIN_SURVIVE		-1
#define LIFIN_WIN			-2
#define LIFIN_FINISH		-3
#define	LIFIN_PAR			-4
#define LIFIN_WIN2			-5

struct	SLevelInfo
{
	SINT			mDiskLevel;
	SINT			mName;
	ELILevelType	mType;
	SINT			mQualify;
	SINT			mGold;
	SINT			mLostToysTime;
	BOOL			mAvailable;
};

struct	SMultiplayerLevelInfo
{
	SINT			mDiskLevel;
	SINT			mName;
	ELILevelType	mType;
};

struct	STournamentInfo
{
	SINT					mName;
	SINT					mNumLevels;
	
	SMultiplayerLevelInfo	mLevels[MAX_LEVELS_PER_TOURNAMENT];
};

class	CLevelStructure
{
public:
	static	SINT	GetNumLevelsTotal()
	{
		return 72;
	}

	// Single Player Stuff
	static	SINT	GetNumLevelsInPrison(SINT p)
	{
		return mNumLevelsPerPrison[p];
	};
	
	static	SINT	GetNumLevelsToAccess(SINT p)
	{
		return mNumLevelsToAccess[p];
	};
	
	static	SLevelInfo	*GetLevelInfo(SINT l, SINT p)
	{
		return &mLevelInfo[l][p];
	};
	
	// Bonus prison stuff
	static	SINT	GetNumBonusLevels()
	{
		return 6;
	}

	static	SINT	GetNumLevelsToAccessBonus(SINT n)
	{
		switch (n)
		{
		case 0: return 4;
		case 1: return 12;
		case 2: return 20;
		case 3: return 30;
		case 4: return 43;
		case 5: return 56;
		};
		return 999;
	}
	
	static	SINT	GetPar(SINT n)
	{
		switch (n)
		{
		case	201:	return 5;
		case	202:	return 4;
		case	203:	return 4;
		case	204:	return 6;
		case	205:	return 4;
		case	206:	return 4;
		case	207:	return 3;
		case	208:	return 4;
		case	209:	return 3;
		};
		return 999;
	};
	
	static SINT		GetLinksPar(SINT n)
	{
		switch (n)
		{
		case 0:	return 13;
		case 2:	return 14;
		case 4:	return 10;
		}
		return 999;
	};

	// Multiplayer Stuff	
	static 	SINT	GetNumMultiplayerTournaments() {return mNumMultiplayerTournaments;};
	
	static	STournamentInfo	*GetTournamentInfo(SINT	t)
	{
		return &mTournamentInfo[t];
	}	
	
	static	SINT	GetPrisonForMultiplayerLevel(SINT t, SINT a)
	{
		SINT 	disk_level = mTournamentInfo[t].mLevels[a].mDiskLevel;
		
		SINT	p = 0;
		
		switch (disk_level)
		{
			case 101:	p = 0;	break;
			case 102:	p = 1;	break;
			case 103:	p = 8;	break;  
			case 104:	p = 5;	break;  
			case 105:	p = 5;	break;
			case 106:	p = 6;	break;
			case 107:	p = 2;	break;
			case 108:	p = 8;	break;
			case 109:	p = 8;	break;
			case 100:	p = 6;	break;
			case 111:	p = 4;	break;
			case 112:	p = 4;	break;	
			case 113:	p = 7;	break;
			case 114:	p = 2;	break;
			case 115:	p = 5;	break;	
			case 116:	p = 5;	break;	
			case 117:	p = 0;	break;	
			case 118:	p = 7;	break;
			case 119:	p = 5;	break;
		}			
		
		return p;
	}

protected:
	static	SINT				mNumLevelsPerPrison[11];
	static	SINT				mNumLevelsToAccess[10];
	static	SLevelInfo			mLevelInfo[11][MAX_LEVELS_PER_PRISON];
	
	static	SINT				mNumMultiplayerTournaments;
	static	STournamentInfo		mTournamentInfo[MAX_TOURNAMENTS];
};

#endif