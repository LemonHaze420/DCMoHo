#include	"Common.h"

#if TARGET == PSX

#include	"PSXUtility.h"
#include 	"PSXBuildType.h"
#include <libcd.h>

#ifdef USE_CD


CdlFILE OpenHandle;
SINT	StartedReading;
char fname2[256];
SINT	PSXOpenFile(char *fname)
{
	// Absolute safety...
	SINT 	c0;
	for (c0 = 0; c0 < 15; c0 ++)
	{
		DrawSync(0);
		VSync(0);
	}

	sprintf(fname2, "\\MOHODATA\\%s;1", fname);
	StartedReading=0;
//	printf("Finding file: %s\n",fname2);
	if(!CdSearchFile(&OpenHandle,fname2))
		return	-1;                     // Couldn't find the file
//	printf("Found file, Name \"%s\" , Size: %d\n",OpenHandle.name,OpenHandle.size);
	// load file
	return OpenHandle.size;
}

SINT PSXCloseFile(void)
{
	return	1;
}

/*-----------------28/10/1999 11:57-----------------
 * Reads 'len' bytes into array 'buffer' and returns
 * ACTUAL bytes read (if less than len...)
 * --------------------------------------------------*/
SINT PSXLoadPartFile(ULONG *buffer,SINT len)
{
	SINT result;
	if(len>OpenHandle.size)len=OpenHandle.size;
	if(StartedReading==0)
	{
		result=CdReadFile(fname2,buffer,(len+2047)&~2047);
		StartedReading=1;
	}
	else result=CdReadFile(NULL,buffer,(len+2047)&~2047);
//	printf("LoadPartFile result: %d\n",result);
	if(result == 0)return -1;
	
	if(result > len)result=len;  //! what the fuck is this???????????????????
	
	OpenHandle.size -= result;                   // Update 'bytes left'
	//printf("0x%08x\n",OpenHandle.size);
	CdReadSync(0, 0);                    // Wait for read to finish

	return result;                      // Return number of bytes read
}



#else

SINT OpenHandle=-1;
SINT OpenSize=-1;
inline void RedirectCDAccess()
{
	MWRedirectIO(__MWIO_CDROM);         // Make sure it's redirected.... Remove to stop redirection
}
SINT	PSXOpenFile(char *fname)
{
	// Absolute safety...
	SINT 	c0;
	for (c0 = 0; c0 < 15; c0 ++)
	{
		DrawSync(0);
		VSync(0);
	}

	char fname2[256];
	RedirectCDAccess();
	sprintf(fname2, "cdrom:\\MOHODATA\\%s", fname);
//	printf("Trying to open \"%s\"\n",fname2);
	// load file
	OpenHandle = MWopen(fname2,O_RDONLY);
	//printf("Handle: %d\n",OpenHandle);
	if(OpenHandle == -1)return -1;
	OpenSize = MWlseek(OpenHandle, 0, SEEK_END);
//	printf("Filesize: %d\n",OpenSize);
	MWlseek(OpenHandle, 0, 0);
	return OpenSize;
}

SINT PSXCloseFile(void)
{
	RedirectCDAccess();
	if(OpenHandle != -1)
	{
		SINT result = MWclose(OpenHandle);
		OpenHandle = -1;
		return result;
	}
	else return -1;
}

/*-----------------28/10/1999 11:57-----------------
 * Reads 'len' bytes into array 'buffer' and returns
 * ACTUAL bytes read (if less than len...)
 * --------------------------------------------------*/
SINT PSXLoadPartFile(ULONG *buffer,SINT len)
{
	if(OpenHandle == -1) return -1;        // Error - File Not Open
	if(len > OpenSize) len = OpenSize;       // Restrict so no read off end of file.
	SINT result = MWread(OpenHandle,buffer,len);
	if(result == -1) return -1;
	OpenSize -= result;                   // Update 'bytes left'
	return result;                      // Return number of bytes read
}

SINT	PSXLoadFile(char *fname, ULONG *buffer)
{
	char fname2[256];
	RedirectCDAccess();
	sprintf(fname2, "cdrom:%s", fname);
//	printf("Trying to open \"%s\"\n",fname2);
	// load file
	SINT	handle = MWopen(fname2,O_RDONLY);
//	printf("Returned handle: %d\n",handle);

	if (handle == -1) return -1;

	SINT	len;

	len = MWlseek(handle, 0, SEEK_END);
//	printf("Filesize: %d\n",len);
	MWlseek(handle, 0, 0);

	MWread(handle, (char *)buffer,len);
	MWclose(handle);
	return len;
}






#endif
























#if 0
BOOL	PSXLoadTimToVRAM(char *fname, ULONG *tempmem)
{
	if (PSXLoadFile(fname, tempmem) == -1)
		return FALSE;

	TIM_IMAGE	tim;

	OpenTIM(tempmem);
	while(ReadTIM(&tim))
	{
		if( tim.caddr )
		{
			*((UWORD *)(tim.caddr))=0;
			LoadImage( tim.crect, tim.caddr );
		}
		if( tim.paddr )
		{
			LoadImage( tim.prect, tim.paddr );
		}
	}
	DrawSync(0);

	return TRUE;
}
#endif


#if 0

#define FORM1_SIZE               2048 /* Size of an XA mode 1 form 1 sector. */
#define Sectors(x) ((x+FORM1_SIZE-1)/FORM1_SIZE)

SLONG	load_file_at(CBYTE * tname,UBYTE * p)
{
	SLONG	ERROR;
	ULONG	c0=100;
	while(1)	//c0<5)	  	//try 5 times
	{
		ERROR = load_file_at_do(tname,p);
		if(ERROR)
			break;
		c0++;
	}
	return(ERROR);
}

SLONG	load_file_at_do(UBYTE * tname,UBYTE * p)
{
/* Loadfile() loads the file specified by 'name' (full path) from CD into    */
/* memory pointed to by 'buffer'. It is the responsibility of the caller to  */
/* make sure that 'buffer' is large enough to hold the data loaded. The      */
/* function loads a whole number of sectors from CD (ie chunks of 2k) so     */
/* be careful about the size of the thing you're loading and the size of the */
/* buffer. This function is synchronous (blocking).                          */
/* The function returns 0 if it can't find the file, or if something goes    */
/* wrong with the CD commands (CD doesn't acknowledge, won't seek, won't     */
/* start reading, or alternatively the number of bytes in the file (*not*    */
/* the number of sectors) if it reads successfully.                          */

    CdlFILE fp;                  /* libcd file struct, including the length. */
    int fileLength;              /* Length of the file on CD in bytes.       */
    int numSectors;              /* Number of sectors the file takes up.     */
    int mode = 0;                /* The mode word for CdRead.                */
	UBYTE	name[128];

	DrawSync(0);

	sprintf(name,"\\%s;1",&tname[0]);

//	printf("load: %s\n",&name[0]);

    if(!CdSearchFile(&fp, name))             /* Find the stream file on CD. */
	{
		printf("search error\n");
		return(0);                               /* Return 0 if file not found. */
	}

    fileLength = fp.size;                     /* Get the file length too.    */

    /* Call Cd control (blocking) to seek to the logical file position.      */
    /* If the CD doesn't acknowledge, return 0 for an error code.            */

    if (!CdControlB(CdlSeekL,(unsigned char *)&fp.pos, 0))
	{
		printf("seek error\n");
      return 0;
	}

    /* Work out the number of sectors from the length of the file on CD.     */

    numSectors = Sectors(fileLength);

    /* Make up the CD mode word (just double speed flag).                    */

    mode |= CdlModeSpeed;

    /* Call CdRead() to read the sectors into the buffer at double speed.    */
    /* If the CD doesn't respond, then return 0.                             */

    if(!CdRead(numSectors,(unsigned long *)p,mode))
	{
		printf("read error\n");
      return(0);
	}

	 /* Block until the CD has finished reading (ie make the call block).     */
{
	SLONG	error;
    while (error=CdReadSync(1, 0) > 0 );
	if(error<0)
	{
		  printf("error\n");
	      return(0);
	}
}

//	printf("read %d\n",fileLength);

    return(fileLength);                     /* Return the length of the file. */
}

#endif
#endif
