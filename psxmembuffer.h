#ifndef PSXMEMBUFFER_H
#define PSXMEMBUFFER_H

class	CThing;

//#define		MEM_BUFFER_SIZE		1000000
#define MEMBUFFSIZE  4096
//extern char psx_mem_buffer[MEM_BUFFER_SIZE];

class	CPSXMemBuffer : public IMemBuffer
{
public:
	CPSXMemBuffer() {mFlip = FALSE;};
	~CPSXMemBuffer();

	BOOL	InitFromFile(char *fname,BOOL IsCrypt=FALSE);

	void	Read(void *data, SINT size);  // stops a silly warning

	//BOOL	IsMoreData() {return (mPtr < psx_mem_buffer + mDataSize);};
	BOOL	IsMoreData() {return ((mFileSize+mInBuff)?1:0);};//mPtr < psx_mem_buffer + mDataSize);};
	BOOL	IsCRCValid() {return mCRCIsValid;};
	BOOL	Close();

	//void	*GetDataPtr() {return mPtr;};

	void 	Skip(SINT size);

	void	DeclareFlippable() {mFlip = TRUE;};
	void	DeclareUnFlippable() {mFlip = FALSE;};

protected:

	BOOL	LoadNextPart(void);
	BOOL	mFirstChunk;                 // Flags if first 8 bytes to be skipped for CRC
	ULONG	mRunningCRC;                // Stores running CRC value for LoadNextPart
	ULONG	mFileCRC;
	BOOL	mIsCrypt;                   // Do I decrypt the second sector of this file?
	BOOL	mDoCRC;                      // TRUE if file has a CRC header
	BOOL	mCRCIsValid;
	char	*mPtr;
	SINT	mInBuff;
	SINT	mFileSize;
	SINT	mFilePtr;

	BOOL	mFlip;
};


#endif

