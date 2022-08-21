#include	"Common.h"
#include "profile.h"

#if TARGET == DC

#include	"MemBuffer.h"  //(sic)
#include	"Globals.h"
#include	"DCBuildType.h"
#include 	<string.h>

#define 	BUFFER_SIZE	102400

//!DC aarrggh
char	DC_mem_buffer[BUFFER_SIZE];

//******************************************************************************************
CDCMemBuffer::~CDCMemBuffer()
{
};


BOOL	CDCMemBuffer::LoadNextPart()
{
#ifdef JCLDCNOPSX			// AAAAARgggggh!! - hokay, PSX version used the primitive pool as temporary storage for the membuffer.  I've allocated 100K above for this temporarily - this should probably go somewhere else!
	mPtr=(char*)&POOL->mPolyPool[0];
#else
	mPtr = DC_mem_buffer;
#endif
	
	mInBuff = DCLoadPartFile((ULONG*)mPtr, BUFFER_SIZE);
	if(mInBuff==-1)
	{
		//mw_pr("LoadNextPart FAILED");
		while(1);
		return FALSE;
	}
	else
	{
		mFileSize-=mInBuff;

		if(mFirstChunk)
		{
			mFirstChunk=0;              // Only do this if we're starting a file
		}


		if(mFileSize==0)
		{
			DCCloseFile();  // Finished with the file so close it
		}
/*
		// Flip screen???
		SDSNoClearBuffer();
		if (mFlip)
		{
			SINT	safe;
			safe = PLATFORM.GeometryMarkBeginScene();
			if (safe != -1)
			{

				PLATFORM.GeometryMarkEndScene();
			}
		}
		*/

		return TRUE;
		
	}
}
//******************************************************************************************
BOOL	CDCMemBuffer::InitFromFile(char *fname)
{
	SOUND.StopTrack();                  // Reinit for double speed loading if I was playing CDA
	PLATFORM.ResetScreenSaver();		// reset screen saver value.	

	mFirstChunk=TRUE;                      // Flag for CRC initialize
	mFileSize = DCOpenFile(fname);
	if (mFileSize == -1)
		return FALSE;
		
	mReadingFromMemory = FALSE;
	return LoadNextPart(); //Pre-read the first part of the file
}

//******************************************************************************************
void	CDCMemBuffer::InitFromMemoryForReading(char *data, SINT size)
{
	SOUND.StopTrack();                  // Reinit for double speed loading if I was playing CDA
	PLATFORM.ResetScreenSaver();		// reset screen saver value.
	
	mFileSize = size;
	mPtr = data;
	mInBuff = 0;

	mReadingFromMemory = TRUE;
}

//******************************************************************************************
void mybcopy(UBYTE *from,UBYTE*to,SINT size)
{
//memcpy(from,to,size); // SDS - doh! forgot to add this essential function...
for (int x=0;x<size;x++)
to[x]=from[x];
#if defined(JJS)
Loop:
		lbu		t0,0(a0)
		addi	a0,a0,1
		addi	a2,a2,-1
		sb		t0,0(a1)
		bnez	a2,Loop
		addi	a1,a1,1
		jr	ra
		nop
#endif
}

//******************************************************************************************
void	CDCMemBuffer::Read(void *data, SINT size)
{
	if (mReadingFromMemory)
	{
		mybcopy((UBYTE*)mPtr,(UBYTE *)data,size);
		mPtr += size;
		mFileSize -= size;	
	}
	else
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
	//				printf("ERROR LOADING DURING CPSXMemBuffer::Read!!!\n");
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
}

//******************************************************************************************
void	CDCMemBuffer::Skip(SINT size)
{
	if (mReadingFromMemory)
	{
		mPtr += size;
		mFileSize -= size;	
	}
	else
	{
		SINT copysize;
		while(size)
		{
			if(mInBuff==0)
				LoadNextPart();
	//			if(!LoadNextPart())
	//			   printf("ERROR LOADING DURING CPSXMemBuffer::Skip!!!\n");
			if(size>mInBuff)copysize=mInBuff;
			else copysize=size;
			size-=copysize;
			mPtr+=copysize;
			mInBuff-=copysize;
		}
	}
}

//******************************************************************************************
BOOL	CDCMemBuffer::Close()
{
	if (!mReadingFromMemory)
		if(mFileSize!=0)DCCloseFile();     // Release the file if we've not already handled this...
	SDSClearBuffer();	
	return TRUE;
}

#endif
