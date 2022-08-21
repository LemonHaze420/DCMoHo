#ifndef DCMEMBUFFER_H
#define DCMEMBUFFER_H

class	CThing;

//#define		MEM_BUFFER_SIZE		1000000
#define MEMBUFFSIZE  4096
//extern char psx_mem_buffer[MEM_BUFFER_SIZE];

class	CDCMemBuffer : public IMemBuffer
{
public:
	CDCMemBuffer() {mFlip = FALSE;};
	~CDCMemBuffer();

	BOOL	InitFromFile(char *fname);
	void	InitFromMemoryForReading(char *data, SINT size);

	void	Read(void *data, SINT size);  // stops a silly warning

	//BOOL	IsMoreData() {return (mPtr < psx_mem_buffer + mDataSize);};
	BOOL	IsMoreData() {return ((mFileSize+mInBuff)?1:0);};//mPtr < psx_mem_buffer + mDataSize);};
	BOOL	Close();

	//void	*GetDataPtr() {return mPtr;};

	void 	Skip(SINT size);

	void	DeclareFlippable() {mFlip = TRUE;};
	void	DeclareUnFlippable() {mFlip = FALSE;};
	
	SINT	GetSize() {return mFileSize;};

protected:

	BOOL	LoadNextPart(void);
	BOOL	mFirstChunk;                 // Flags if first 8 bytes to be skipped for CRC
	char	*mPtr;
	SINT	mInBuff;
	SINT	mFileSize;
	SINT	mFilePtr;

	BOOL	mFlip;
	
	BOOL	mReadingFromMemory;
};


#endif

