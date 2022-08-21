#include	"Common.h"
#if TARGET == DC

#include    "Globals.h"
#include 	"DCMemCard.h"
#include	"Career.h"

#include	"DCPlatform.h"
#include	"DCBuildType.h"

#include	"sg_pad.h"

#include	<string.h>

SINT	InternalMemCardBuffer1[15000];
SINT	InternalMemCardBuffer2[15000];

char	MC_GAME_NAME[] = "MOHOSAVEGAME";
char	MC_BTR_COMMENT[] = "MOHO";
char	MC_VMS_COMMENT[] = "MOHO";

#ifdef	JCLDCNOPSX		// ball breakers
char	MC_GAME_NAME[] = "BALLBREAKERS";
char	MC_BTR_COMMENT[] = "BALL BREAKERS CAREER";
char	MC_VMS_COMMENT[] = "BALL BREAKERS";
#endif


//UBYTE	MC_ICON_PALETTE[10000];
//UBYTE	MC_ICON_DATA[10000];


short int BB_MC_ICON_PALETTE[] = {64563,63232,64614,64512,61440,62208,62464,64000,62976,61696,63744,0,0,0,0,0,};
char BB_MC_ICON_DATA[] = {34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,32,35,51,51,51,51,
51,51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,
35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,
51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,
51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,
17,17,17,17,26,51,51,58,17,17,17,17,23,51,49,35,68,68,68,68,68,154,51,56,68,68,68,
68,68,99,49,35,68,68,68,68,68,68,115,56,68,68,68,68,68,72,49,35,68,68,88,133,68,68,
19,56,68,68,136,100,68,68,49,35,68,68,19,51,68,68,131,56,68,73,51,49,68,68,49,35,
68,68,19,55,68,68,163,56,68,73,51,56,68,69,49,35,68,68,149,84,68,72,51,56,68,68,85,
84,68,151,49,35,68,68,68,68,68,87,51,56,68,68,68,68,68,19,49,35,68,68,68,68,68,68,
163,56,68,68,68,68,68,69,49,35,68,68,19,55,100,68,147,56,68,73,51,58,68,68,49,35,
68,68,19,51,164,68,71,56,68,73,51,51,84,68,49,35,68,68,23,122,84,68,71,56,68,73,119,
113,68,68,49,35,68,68,68,68,68,68,83,56,68,68,68,68,68,68,49,35,68,68,68,68,68,68,
163,56,68,68,68,68,68,70,49,35,68,68,68,68,69,103,51,56,68,68,68,68,149,19,49,35,
51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,
51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,
51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,51,
51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,
51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,17,17,17,17,17,17,17,17,
17,17,17,17,17,17,17,17,};

short int MC_ICON_PALETTE[] = {64563,63232,64614,64512,61440,62464,63744,64000,62208,61696,62976,63488,0,0,0,0,};
char MC_ICON_DATA[] = {34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,32,35,51,51,51,51,
51,51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,
35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,51,102,102,102,102,99,51,51,51,
102,102,102,102,99,49,35,51,68,68,68,68,70,51,51,59,68,68,68,68,147,49,35,51,68,68,
68,68,74,51,51,53,68,68,68,68,147,49,35,51,68,68,68,68,72,51,51,57,68,68,68,68,147,
49,35,51,68,68,68,68,68,51,51,116,68,68,68,68,147,49,35,51,68,68,68,68,68,99,51,20,
68,68,68,68,147,49,35,51,68,68,73,68,68,163,51,84,68,73,68,68,147,49,35,51,68,68,
69,68,68,131,51,148,68,152,68,68,147,49,35,51,68,68,69,132,68,67,55,68,68,88,68,68,
147,49,35,51,68,68,69,164,68,70,49,68,68,24,68,68,147,49,35,51,68,68,69,100,68,74,
53,68,68,120,68,68,147,49,35,51,68,68,69,52,68,72,57,68,73,56,68,68,147,49,35,51,
68,68,69,56,68,68,116,68,69,56,68,68,147,49,35,51,68,68,69,58,68,68,84,68,65,56,68,
68,147,49,35,51,68,68,69,54,68,68,68,68,71,56,68,68,147,49,35,51,68,68,69,51,68,68,
68,68,147,56,68,68,147,49,35,51,68,68,69,51,132,68,68,68,83,56,68,68,147,49,35,51,
68,68,69,51,164,68,68,68,19,56,68,68,147,49,35,51,68,68,69,51,100,68,68,68,115,56,
68,68,147,49,35,51,68,68,69,51,52,68,68,73,51,56,68,68,147,49,35,51,68,68,69,51,56,
68,68,69,51,56,68,68,147,49,35,51,68,68,69,51,58,68,68,65,51,56,68,68,147,49,35,51,
68,68,69,51,54,68,68,71,51,56,68,68,147,49,35,51,51,51,51,51,51,51,51,51,51,51,51,
51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,
51,51,51,51,51,51,51,49,35,51,51,51,51,51,51,51,51,51,51,51,51,51,51,49,17,17,17,
17,17,17,17,17,17,17,17,17,17,17,17,17,};
#define	MC_NUM_ICONS 	1
#define	MC_ICON_SPEED	1

UBYTE	MC_VISUAL_DATA[10000];  // safety
#define	MC_VISUAL_TYPE 	BUD_VISUALTYPE_NONE


//*************************************************************************************
inline	char	*my_strcpy(char *d, const char *s)
{
	char *r = d;
	while(((*(d ++) = *(s ++)) != NULL));
	return r;
}

//*************************************************************************************
VMUState MCCheck(int CardNum)
{
	const	BACKUPINFO		*binfo;
	const	BUS_DISKINFO	*diskinfo;

	binfo = BupGetInfo(CardNum);
	diskinfo = &binfo->DiskInfo;

	if (binfo->Ready)
	{
		if (binfo->IsFormat) 
			return VMU_EXISTS_FORMATTED;
		else 
			return VMU_EXISTS_UNFORMATTED;
	}
	else
	{
		if (binfo->Operation == BUD_OP_MOUNT) 
			return VMU_NOT_READY;
		else 
			return VMU_NOT_THERE;
	}
}

//*************************************************************************************
int 	MCBlocksFree(int CardNum)
{
	const 	BACKUPINFO		*binfo;
	const	BUS_DISKINFO	*diskinfo;

	binfo = BupGetInfo(CardNum);
	diskinfo = &binfo->DiskInfo;

	if (binfo->Ready)
	{
		if (binfo->IsFormat) 
			return	diskinfo->free_user_blocks;
		else
			return -1;
	}
	else
		return -1;
}

//*************************************************************************************
// Use this to get the memcards current Save/Load state = binfo->ready
const BACKUPINFO *MCPollMemCard(int CardNum)
{
	const BACKUPINFO	*binfo = BupGetInfo(CardNum);
	return binfo;
}

//*************************************************************************************
SINT 	MCLoad(int CardNum)
{
//	BupLoad(CardNum, fname, &InternalMemCardBuffer);

	memset(InternalMemCardBuffer1, 0, sizeof(InternalMemCardBuffer1));

	SINT	blocks = MCGetBlocksNeededToSave();

	return buLoadFile(CardNum, MC_GAME_NAME, InternalMemCardBuffer1, blocks);
}

//*************************************************************************************
SINT	MCLoadFinish()
{
	BUS_BACKUPFILEHEADER bfh;
	
	SINT ret = buAnalyzeBackupFileImage(&bfh, InternalMemCardBuffer1);
	
	if (ret != BUD_ERR_OK)
		return ret; // failed
		
	// backup some stuff from the career
	BOOL	stereo 		= CAREER.mStereo;
	SINT	controller 	= CAREER.mMemController;
	SINT	mem_slot	= CAREER.mMemSlot;
	
	memcpy(&CAREER, bfh.save_data, sizeof(CAREER));
	
	// restore
	CAREER.mStereo 			= stereo;
	CAREER.mMemController 	= controller;
	CAREER.mMemSlot			= mem_slot;
	
	SOUND.DeclareVolumeChange();
	
	return BUD_ERR_OK;
}

//*************************************************************************************
SINT	MCPrepareSave()
{
	BUS_BACKUPFILEHEADER	bfh;

	memset(&bfh, 0, sizeof(bfh));

	my_strcpy((char *)bfh.vms_comment, MC_VMS_COMMENT);
	my_strcpy((char *)bfh.btr_comment, MC_BTR_COMMENT);
	my_strcpy((char *)bfh.game_name, MC_GAME_NAME);
	
	bfh.icon_palette = (void *)MC_ICON_PALETTE;
	bfh.icon_data 	 = (void *)MC_ICON_DATA;
	bfh.icon_num   	 = MC_NUM_ICONS;
	bfh.icon_speed 	 = MC_ICON_SPEED;
	
	bfh.visual_data  = MC_VISUAL_DATA;
	bfh.visual_type  = MC_VISUAL_TYPE;
	
	bfh.save_data	 = (void *)(&CAREER);
	bfh.save_size    = sizeof(CAREER);
	
	return buMakeBackupFileImage(InternalMemCardBuffer1, &bfh);
}

//*************************************************************************************
SINT 	MCSave(int CardNum)
{
	SINT blocks = MCPrepareSave();
	
	if (blocks > 0)
		return BupSave(CardNum, MC_GAME_NAME, InternalMemCardBuffer1, blocks);
	else
		return -10000; // mine
}

//*************************************************************************************
void 	MCDeleteMemCard(int CardNum)
{
}

//*************************************************************************************
int 	MCGetProgress(int CardNum)
{
	const BACKUPINFO *binfo;
	binfo = BupGetInfo(CardNum);
	int res=(binfo->ProgressCount * 100 / binfo->ProgressMax);	
	if (buStat(CardNum) == BUD_STAT_READY)
		res=100;
	return res;		
}

//*************************************************************************************
BOOL	MCIsThereAGameSave(SINT card)
{
	// check to see if there is a MoHo save on this card

	SINT 	ret;
	char	fname[20];
	
	ret = buFindFirstFile(card, fname);
	while (ret >= 0)
	{
		if (!(strncmp(fname, MC_GAME_NAME, 12)))
		{
			return TRUE;	
		}
		
		ret = buFindNextFile(card, fname);
	};
	
	return FALSE;
}

//*************************************************************************************
SINT	MCGetBlocksNeededToSave()
{
	static int needed = -1;
	
	if (needed == -1)
		needed = MCPrepareSave();
		
	return needed;
}

#endif
