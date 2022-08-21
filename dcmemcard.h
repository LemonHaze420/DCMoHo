#ifndef DCMEMCARD_H
#define DCMEMCARD_H

#include	"DCBackup.h"
#include	"Career.h"

enum VMUState
{
	VMU_NOT_THERE,
	VMU_NOT_READY,
	VMU_EXISTS_UNFORMATTED,
	VMU_EXISTS_FORMATTED,
	VMU_EXISTS_CORRUPT
};

/*
typedef struct 
{
	char VMCOMMENT      [16];
	char GUICOMMENT 	 [32];
	char GAMENAME   	 [16];
	int  NumberOfIcons;
	int  AnimationSpeed;
	int  VisualType;
	int  CRC;
	int  SaveDataSize;
	char Reserved       [20];
	char IconPalette    [32];
	char DATA           [4000];
} DCMEMFILE;
*/

VMUState 			MCCheck(int CardNum);
int 				MCBlocksFree(int CardNum);
const	BACKUPINFO 	*MCPollMemCard(int CardNum);
SINT 				MCLoad(int CardNum);
SINT				MCLoadFinish();
SINT 				MCSave(int CardNum);
int 				MCGetProgress(int CardNum);

BOOL				MCIsThereAGameSave(SINT card);

SINT				MCGetBlocksNeededToSave();

#endif