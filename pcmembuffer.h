#ifndef PCMEMBUFFER_H
#define PCMEMBUFFER_H

class	CThing;

class	CPCMemBuffer : public IMemBuffer
{
public:
	CPCMemBuffer() {mData = NULL;};
	~CPCMemBuffer();

	BOOL	InitFromMem();
	BOOL	InitFromFile(char *fname);

	SINT	ConvertFromWritingToReading();	// returns length

	void	Read(void *data, SINT size);
	void	Write(void *data, SINT size);

	BOOL	Close(char *fname = NULL);

	BOOL	IsMoreData() {ASSERT(mReading); return (mPtr < mData + mDataSize);};

	void	DeclareInvalidData(CThing *t);

	char	*GetData() {return mData;};

	SINT	GetDataSize() {return mDataSize;};

protected:
	char	*mData;
	char	*mPtr;

	SINT	mSize, mDataSize;
	BOOL	mReading;
};


#endif