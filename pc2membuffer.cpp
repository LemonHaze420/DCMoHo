#include	"Common.h"
#include "profile.h"

#if TARGET == PSX

#include	"MemBuffer.h"  //(sic)
#include	"Globals.h"
#include 	"libcrypt.h"
#include	"PSXBuildType.h"
//!PSX aarrggh
//char	psx_mem_buffer[MEM_BUFFER_SIZE];

#define 	BUFFER_SIZE	102400

//******************************************************************************************
CPSXMemBuffer::~CPSXMemBuffer()
{
};

extern unsigned long ComputeCRC32Part(unsigned char *buf,int len,unsigned long  crc);


BOOL	CPSXMemBuffer::LoadNextPart()
{
	mPtr=(char*)&POOL->mPolyPool[0];
	mInBuff = PSXLoadPartFile((ULONG*)mPtr, BUFFER_SIZE);
	if(mInBuff==-1)
	{
		printf("LoadNextPart FAILED!\n");
		while(1);
		return FALSE;
	}
	else
	{
		mFileSize-=mInBuff;

		if(mFirstChunk)
		{
			mFirstChunk=0;              // Only do this if we're starting a file
			// See if we'er dealing with a CRC checksummed file....
			if(*(ULONG*)(mPtr)==MKID("DS32"))
			{
				// Yep! Got a CRC file!
				mDoCRC=TRUE;
				mFileCRC=*(ULONG*)(mPtr+4);
				printf("*file has a CRC* (0x%08x)\n",mFileCRC);
				mRunningCRC =0xffffffff;             // Initialize the CRC for checks...

#if DISP_MODE == MODE_PAL
				if(mIsCrypt)Decrypt((unsigned char*)(mPtr+2048));  // Second sector is encrypted using LibCrypt (SCEE)
#endif

				//Skip the chunk header
				mPtr+=8;
				mInBuff-=8;
				mCRCIsValid=FALSE;
			}
			else
			{
				mDoCRC=FALSE;
				mCRCIsValid=TRUE;
			}
		}

		// Now see if we're doing a CRC check on the loaded data....
		if(mDoCRC)
			mRunningCRC=ComputeCRC32Part((unsigned char*)mPtr,mInBuff,mRunningCRC);


		if(mFileSize==0)
		{
			PSXCloseFile();  // Finished with the file so close it
			if(mDoCRC)
			{
				if((~mRunningCRC)!=mFileCRC)
				{
					mCRCIsValid=FALSE;
					printf("CRC was INVALID!!!\n");
				}
				else
				{
					mCRCIsValid=TRUE;
					printf("CRC was OK!\n");
				}
			}
		}

		// Flip screen???
		if (mFlip)
		{
			SINT	safe;
			safe = PLATFORM.GeometryMarkBeginScene();
			if (safe != -1)
			{
				SINT	f = PLATFORM.GetFrame();
				NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f + 2));
				NTAG_Rlink(PLATFORM.GetOrderTable(),OT_LENGTH,(long*)&POOL->mNTAGTerms[f][0].firstprim
															,(long*)&POOL->mNTAGTerms[f][0].lastprim);
				PLATFORM.GeometryMarkEndScene();
			}
		}
		return TRUE;
	}
}
//******************************************************************************************
BOOL	CPSXMemBuffer::InitFromFile(char *fname,BOOL IsCrypt)
{
	STOP_PSX_PROF();
	SOUND.StopTrack();                  // Reinit for double speed loading if I was playing CDA
	printf("\"%s\"",fname);
	//VSync(0);
	//VSync(0);
	//VSync(0);
	//VSync(0);
	//my_strncpy(mFileName,fname,256);
#if DISP_MODE == MODE_PAL
	if(IsCrypt)printf("Requesting Decrypt!\n");
#endif
	mIsCrypt=IsCrypt;
	mFirstChunk=TRUE;                      // Flag for CRC initialize
	mFileSize = PSXOpenFile(fname);
	if (mFileSize == -1)
		return FALSE;
	return LoadNextPart(); //Pre-read the first part of the file
}

void asm mybcopy(UBYTE *from,UBYTE*to,SINT size)
{
Loop:
		lbu		t0,0(a0)
		addi	a0,a0,1
		addi	a2,a2,-1
		sb		t0,0(a1)
		bnez	a2,Loop
		addi	a1,a1,1
		jr	ra
		nop
}

//******************************************************************************************
void	CPSXMemBuffer::Read(void *data, SINT size)
{
	UBYTE *dest=(UBYTE*)data;
	SINT todo=size;
	SINT copysize;
	if(todo==-1)todo=mFileSize+mInBuff;  // Means read all data if size passed is NULL
	while(todo)
	{
		if(mInBuff==0)
			if(!LoadNextPart())
			{
				printf("ERROR LOADING DURING CPSXMemBuffer::Read!!!\n");
				mInBuff=0;
				Close();
				return;
			}
		if(todo>mInBuff)copysize=mInBuff;
			else copysize=todo;
		mybcopy((UBYTE*)mPtr,dest,copysize);   // Copy remainder of buffer
		todo-=copysize;
		dest+=copysize;
		mPtr+=copysize;
		mInBuff-=copysize;
	}
}

//******************************************************************************************
void	CPSXMemBuffer::Skip(SINT size)
{
	SINT copysize;
	while(size)
	{
		if(mInBuff==0)
			if(!LoadNextPart())printf("ERROR LOADING DURING CPSXMemBuffer::Skip!!!\n");
		if(size>mInBuff)copysize=mInBuff;
		else copysize=size;
		size-=copysize;
		mPtr+=copysize;
		mInBuff-=copysize;
	}
}

//******************************************************************************************
BOOL	CPSXMemBuffer::Close()
{
	START_PSX_PROF();
	if(mFileSize!=0)PSXCloseFile();     // Release the file if we've not already handled this...
	return TRUE;
}

#endif
