#include	"Common.h"
#if TARGET == PSX

#include    "Globals.h"
#include 	"PsxMemCard.h"
#include	"Career.h"

#include 	<libcd.h>
#include	"PSXPlatform.h"
#include	"PSXBuildType.h"
#include 	<libapi.h>
#include	<malloc.h>

#include	"asc2sjis.cpp"
#include 	"crc.cpp"
#include <string.h>
extern void asm mybcopy(UBYTE *from,UBYTE*to,SINT size);

struct DIRENTRY McFileList[BLOCK_MAX];


#if DISP_MODE == MODE_NTSC
char GAMESEARCHNAME[]={"BASLUS-01249*"};
#else
char GAMESEARCHNAME[]={"BESLES-02830*"};
#endif


#define SOCareer  (((sizeof(CCareer)+3)/4)*4)
typedef struct
{
	unsigned long CRC1[2];
	unsigned char Copy1[SOCareer];
	unsigned long CRC2[2];
	unsigned char Copy2[SOCareer];

} CAREER_FILE;


typedef struct
{
	char			magic[2];
	char			type;
	char			blockEntry;
	unsigned char	title[64];
	char 			reserved[28];
	short			clut[16];
	long			icons[3][32];
} FILE_HEADERPART;


typedef struct
{
	char			magic[2];
	char			type;
	char			blockEntry;
	unsigned char	title[64];
	char 			reserved[28];
	short			clut[16];
	long			icons[3][32];
	char			Buffer[7680];  // For the Remaining data in the save.
} FILE_HEADER;

#define mkcol(b,g,r) (((r>>3)&0x1f)|(((g>>3)&0x1f)<<5)|(((b>>3)&0x1f)<<10))

FILE_HEADERPART MCHeaderPart=
{
	'S','C',
	0x11,
	1,
	{0},
	{0},
#if DISP_MODE == MODE_PAL
	{
		mkcol(0x0,0x0,0x0),
		mkcol(0x0,0x13,0xb),
		mkcol(0x0,0xb,0x1b),
		mkcol(0x13,0x2b,0x4b),
		mkcol(0x13,0x2b,0x7b),
		mkcol(0x3b,0x4b,0xff),
		mkcol(0x0,0x13,0x53),
		mkcol(0x23,0x33,0xc7),
		mkcol(0xb,0x1b,0x2b),
		mkcol(0x0,0x0,0xb),
		mkcol(0x3b,0x53,0x83),
		mkcol(0x2b,0x33,0x33),
		mkcol(0x4b,0x6b,0xc7),
		mkcol(0xff,0x55,0xff),
		mkcol(0x55,0xff,0xff),
		mkcol(0xff,0xff,0xff)
	},
	{
		{
			0x00000000	,0x000bb0a8	,
			0x103c30ac	,0x0006c07c	,
			0xccc5c075	,0x00065045	,
			0x55555a55	,0x1caacac5	,
			0xa7555555	,0x4c5c5c57	,
			0xb375a555	,0x5a475a77	,
			0x31776175	,0x5a177145	,
			0xa877b045	,0x7a077137	,
			0xac778047	,0x4ca7c147	,
			0x9b478047	,0x8477a844	,
			0x00672047	,0x06684844	,
			0x00378037	,0x00003843	,
			0x00042044	,0x00000841	,
			0x00089148	,0x00000990	,
			0x00000039	,0x00000000	,
			0x00000920	,0x00000000	,
		},
		{0},
		{0}

	},
#else
 	{

        mkcol(0x73,0x62,0x52),
        mkcol(0x94,0x7b,0x6a),
        mkcol(0xac,0x94,0x7b),
        mkcol(0xa4,0x94,0x83),
        mkcol(0x5a,0x52,0x41),
        mkcol(0xbd,0xa4,0x83),
        mkcol(0x52,0x41,0x39),
        mkcol(0x9c,0x83,0x6a),
        mkcol(0xac,0x9c,0x83),
        mkcol(0x00,0x00,0x00),
        mkcol(0xa4,0x8b,0x73),
        mkcol(0x83,0x73,0x5a),
        mkcol(0xcd,0xb4,0x94),
        mkcol(0x99,0x99,0xc9),
        mkcol(0x58,0x95,0x99),
        mkcol(0x99,0xa9,0xc5)
	},
	{
		{
			0x32a99999,0x99999222,
			0xa17aa999,0x999237a2,
			0x2a771a99,0x99217a28,
			0x823131a9,0x92112a85,
			0x81bb0139,0x91bbbbb5,
			0xb444407a,0x81b64448,
			0x47824012,0xa14a3a6c,
			0x864440aa,0xaa164448,
			0x4ba140aa,0xab4ba168,
			0x61a74b22,0x2b61ab45,
			0xb4646b8a,0xc304666a,
			0x22855829,0x95855322,
			0x8558cca9,0x95ac8585,
			0x58ccca99,0x99aacc58,
			0xccc5a999,0x999c7cc5,
			0x55c99999,0x99999558,

		},
		{	0	},
		{	0	}

	},
#endif



};

long NumFilesOnCard,files,FormattedFlag;
long McCurrState=McErrCardNotExist;


#define macromemclr(x,y) {for(__cnt=0;__cnt<(y),__cnt++)*x++=0;}


void MCStart(void)
{
	MemCardStart();
	McCurrState=McErrCardNotExist;
}

void MCStop(void)
{
	MemCardStop();
	McCurrState=McErrCardNotExist;
}

void MCClearFiles(void)
{
	for(int i=0;i<15;i++)
	{
		McFileList[i].size=0;
		McFileList[i].name[0]=0;
	}
}

// !!BEWARE!! This returns NON-ZERO if the file exists or there's an error....
//				It returns ZERO if the filename doesn't exist....


int mystrcmp ( char *p1, char *p2 )
{
	register unsigned char *s1 = ( unsigned char *) p1;
	register unsigned char *s2 = ( unsigned char *) p2;
	register unsigned char c1, c2;

	do
	{
		c1 = ( unsigned char) *s1++;
		c2 = ( unsigned char) *s2++;
		if (c1 == '\0')
		return c1 - c2;
	}
	while (c1 == c2);

	return c1 - c2;
}






int MCFileExist(char *name)
{
	if(files)
	for(int i=0;i<files;i++)
	{
		if(McFileList[i].name[0])
		{
//			printf("--------------------------\n%s\n%s\n",McFileList[i].name,name);
			int r=mystrcmp(McFileList[i].name,name);
//			printf("strcmp: %d\n",r);
			if(!r)
			{
//				printf("MATCH!\n");
				return McErrAlreadyExist;
			}
		}
	}
	return 0;
}

int MCGetState(int check)
{
		long cmds,rslt,i;
		int ret = MemCardSync(1,&cmds,&rslt);

	switch(ret)
	{
		case -1:
			if(check)
			{
				MemCardExist( 0x00 );	//Execute connection test
				McCurrState=McBusy;
			}
		break;

		case 0:
			McCurrState=McBusy;
		break;

		case 1:
			McCurrState=McErrNone;
		// this is where all the interesting, if such things exist with Memory cards happen
			switch(cmds)
			{
				case McFuncExist:		// the Memory card Exists.
					switch(rslt)
						{
							// McErrNone means there's a card in the slot, but it could be bad/unformatted
							case McErrNone:
								if (FormattedFlag == 0)
									McCurrState = McErrNotFormat;
//								else if(McCurrState!=McErrNotFormat)
								else
									McCurrState=McErrNone;  // Flag we've got a valid card....
							break;

							case McErrNewCard:
//								printf("Card Has Been Inserted\n");
								files=0;
								MemCardAccept(0x00);	// Get further detailed information
							break;

							default:
//								printf("Please Enter A Memory card Into port 1\n");
								files=0;
								McCurrState=McErrCardNotExist;
							break;
						}
				break;	// end of McFuncExist

				case McFuncAccept:
					switch(rslt)
						{
							case McErrNone:
							case McErrNewCard:
								// read the directory information.... all 15 blocks on the cards.
								// With no specific file name.
								// First clear the filelist.....
								MCClearFiles();
								MemCardGetDirentry( 0x00, GAMESEARCHNAME , McFileList, &files, 0, 15 );
								for( i=0, NumFilesOnCard=0; i<files; i++ )
								NumFilesOnCard += McFileList[i].size/8192 + (McFileList[i].size%8192 ? 1:0);
								FormattedFlag=1;
							break;

							case McErrNotFormat:	// Find unformatted card
								MCClearFiles();
//								printf("This card is not formatted\n");
								McCurrState=McErrNotFormat;
								FormattedFlag=0;
							break;

							case McErrCardNotExist:
								MCClearFiles();
//								printf("Card removed at an awkward time!\n");
								McCurrState=McErrCardNotExist;
								break;

							default:
//								printf("ERROR(%d)",(int)rslt);
							break;
						}
						break;
				default:                // Any other functions need to set return code....
					McCurrState=rslt;
					break;
			}
	}
//	if(McCurrState!=McBusy)printf("State: %d\n",McCurrState);
	return (McCurrState);
}

/*void MCEchoFiles(void)
{
	if(McCurrState==McErrNone)
	for(int i=0;i<15;i++)
		if(McFileList[i].size)
			printf("Name: \"%s\" Size: %d bytes\n",McFileList[i].name,McFileList[i].size);
}*/


int MCAcquireCard(void)
{
	while(MCGetState(1)==McBusy)/*Wait*/;
	while(MCGetState(1)==McBusy)/*Wait*/;
	return McCurrState;
}


int MCNumFiles(void)
{
	return files;
}


int MCSaveCareer(char *name,CCareer*career,int overwrite)
{
	FILE_HEADER	*MCHeader = (FILE_HEADER *)POOL->mMapCells;
//	FILE_HEADER	MCHeader;

	long cmds,rslt,ret;
	mybcopy((unsigned char*)(&MCHeaderPart), (unsigned char*)(MCHeader),sizeof(FILE_HEADERPART));

#if DISP_MODE == MODE_NTSC
	unsigned char SJISNAME[]="Ball Breakers    :             \0";
#else
	unsigned char SJISNAME[]="MoHo Career File :             \0";
#endif

	char FileName[32];
	my_strncpy(FileName,GAMESEARCHNAME,12);
	my_strncpy(&FileName[12], name, 999);
	my_strncpy((char*)&SJISNAME[19],name, 999);
	CAREER_FILE *car=(CAREER_FILE*)(MCHeader->Buffer);

/*	if((SOCareer+12)>(7680/2))
	{
		printf("Career size is too large to fit one block!!!! (%d)\n",SOCareer);
		while(1);
	}
	else
		printf("Career size: %d\n",SOCareer);
*/
	// (JCL) clear the buffer area to prevent CRC inconsistencies
	SINT	c0;
	for (c0 = 0; c0 < 7680; c0 ++)
		MCHeader->Buffer[c0] = 0;

	mybcopy((unsigned char*)career, car->Copy1, SOCareer);
	mybcopy((unsigned char*)career, car->Copy2, SOCareer);

	unsigned long CRC=ComputeCRC32(car->Copy1,SOCareer);
	for(int i=0;i<2;i++)
	{
		car->CRC1[i]=car->CRC2[i]=CRC;
	}

//	printf("Filename: %s\n CRC-32 : 0x%08x\n",FileName,CRC);

	AsciiStringToSJIS(SJISNAME,MCHeader->title);


	while(MCGetState(1)==McBusy)/*Wait*/;
	while(MCGetState(1)==McBusy)/*Wait*/;



	if(!FormattedFlag)return McErrNotFormat;

	// Open the file depending upon overwrtite flag...
	if(MCFileExist(FileName))
	{
		if(!overwrite)return McErrAlreadyExist;
	}
	else
	{
//		printf("Didn't exist.... Creating\n");
		ret=MemCardCreateFile(0x00,FileName,1);
	}

	if(ret!=0)return ret;

//	printf("Level 0 best time: %d\n", career->mCharCareers[0].mLevelCareers[0][0].mBestTime);
//	printf("Level 0 best time: %d\n", ((CCareer *)car->Copy1)->mCharCareers[0].mLevelCareers[0][0].mBestTime);

	ret = MemCardSync(0,&cmds,&rslt);
	MemCardWriteFile(0x00,FileName,(unsigned long*)MCHeader,0,8192);
	ret = MemCardSync(0,&cmds,&rslt);
	MemCardAccept(0x00);	// Get further detailed information
	return rslt;
}






int MCLoadCareer(char *name,CCareer*career)
{
	FILE_HEADER	*MCHeader = (FILE_HEADER *)POOL->mMapCells;
//	FILE_HEADER	MCHeader;	
	
	long cmds,rslt;
	char FileName[32];
	int ret;

	my_strncpy(FileName,GAMESEARCHNAME,12);
	my_strncpy(&FileName[12],name, 999);

//	printf("About to load %s\n", FileName);

	CAREER_FILE *car=(CAREER_FILE*)(MCHeader->Buffer);

	for(int i=0;i<7680;i++)MCHeader->Buffer[i]=0;  // Clear the load buffer

/*	if((SOCareer+12)>(7680/2))
	{
		printf("Career size is too large to fit one block!!!! \n");
		while(1);
	}
*/
	while(MCGetState(1)==McBusy)/*Wait*/;
	while(MCGetState(1)==McBusy)/*Wait*/;
	if(!FormattedFlag)return McErrNotFormat;


	ret = MemCardSync(0,&cmds,&rslt);
	MemCardReadFile(0x00,FileName,(unsigned long*)MCHeader,0,8192);
	ret = MemCardSync(0,&cmds,&rslt);
	if(rslt!=0)return rslt;


	// Force a 1 bit error in the first buffer area
//	MCHeader.Buffer[150]^=1;



	unsigned long CRC1=ComputeCRC32(car->Copy1,SOCareer);
	unsigned long CRC2=ComputeCRC32(car->Copy2,SOCareer);

//	printf("Filename: %s\n CRC-32(1) : 0x%08x CRC-32(2) : 0x%08x\n",FileName,CRC1,CRC2);

	int firstok=1;
	int secondok=1;

	if(car->CRC1[0]!=car->CRC1[1])
		firstok=0;
	if(car->CRC1[0]!=CRC1)
		firstok=0;

	if(car->CRC2[0]!=car->CRC2[1])
		secondok=0;
	if(car->CRC2[0]!=CRC2)
		secondok=0;


//	printf("Filename: %s\n CRC-32 : 0x%08x\n",FileName,CRC1);

	if(firstok)
	{
//		printf("Using first copy of data\n");

		mybcopy(car->Copy1, (unsigned char*)career, SOCareer);
		return McCurrState;
	}
	else
	if(secondok)
	{
//		printf("Using SECOND copy of data\n");

		mybcopy(car->Copy2, (unsigned char*)career, SOCareer);
		return McCurrState;
	}
	else
	{
		if (((CCareer *)car->Copy1)->mVersion != CAREER_VERSION)
		{
//			printf("Wrong version of Career structure\n");
			return McWrongVersion;
		}
		return McFileCorrupt;
	}
}

int MCFormat(void)
{

	long cmds,rslt;
	int ret;


	while(MCGetState(0)==McBusy)/*wait*/;

	ret=-1;
	while(ret==-1)
	{
		ret=MemCardFormat(0);
	}
	MemCardAccept(0x00);	// Get further detailed information
	return ret;
}

#endif




