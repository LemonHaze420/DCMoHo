#include	"Common.h"

#if TARGET == PC

#include	"MemBuffer.h"  //(sic)
#include	"Globals.h"

//******************************************************************************************
//#define		MEM_BUFFER_SIZE		1024000
//#define		MEM_BUFFER_SIZE		(1024000 * 2)
#if defined(LINK_EDITOR)
#define		MEM_BUFFER_SIZE		(1024000 * 16)
#else
#define		MEM_BUFFER_SIZE		(1024000 * 1)
#endif
//******************************************************************************************
CPCMemBuffer::~CPCMemBuffer()
{
//	ASSERT(!(mData)); // should be closed by now...
	delete mData;
};

//******************************************************************************************
BOOL	CPCMemBuffer::InitFromMem()
{
	mData =	new char[MEM_BUFFER_SIZE];

	if (!mData)
		return FALSE;

	mPtr = mData;
	mSize = MEM_BUFFER_SIZE;
	mReading = FALSE;
	mDataSize = 0;

	return TRUE;
}

//******************************************************************************************
BOOL	CPCMemBuffer::InitFromFile(char *fname)
{
	mData =	new char[MEM_BUFFER_SIZE];

	if (!mData)
		return FALSE;

	mPtr = mData;
	mSize = MEM_BUFFER_SIZE;
	mReading = TRUE;
	mDataSize = 0;

	// load file
	FILE	*fp = fopen(fname, "rb");

	if (!fp)
	{
		delete mData;
		mData = NULL;
		return FALSE;
	}

	SINT t;
	do
	{
		t = fread(mPtr, 1, 10000, fp);
		mPtr += t;
		mDataSize += t;
	} while (t == 10000);

	fclose(fp);

	mPtr = mData;

	return TRUE;
}
//******************************************************************************************
SINT	CPCMemBuffer::ConvertFromWritingToReading()
{
	mReading	= TRUE;
	mPtr		= mData;

	return mDataSize;
}

//******************************************************************************************
void	CPCMemBuffer::Read(void *data, SINT size)
{
	ASSERT(mReading);

	memcpy(data, mPtr, size);
	mPtr += size;

	ASSERT(mPtr <= mData + mDataSize);
}

//******************************************************************************************
void	CPCMemBuffer::Write(void *data, SINT size)
{
	ASSERT(!mReading);
	ASSERT(mPtr + size <= mData + mSize);

	memcpy(mPtr, data, size);
	mPtr += size;
	mDataSize += size;
}

//******************************************************************************************
BOOL	CPCMemBuffer::Close(char *fname)
{
	if ((mReading) || (fname == NULL))
	{
		// just free up - easy
		delete [] mData;
		mData = NULL;
		return TRUE;
	}

	// otherwise write out the stream.

	FILE	*fp;

	fp = fopen(fname, "wb");

	if (!fp)
	{
		// can't open - clean up and go home
		delete [] mData;
		mData = NULL;
		return FALSE;
	}

	fwrite(mData, mDataSize, 1, fp);  //! hope this works!
	fclose(fp);

	// free up
	delete [] mData;
	mData = NULL;

	return TRUE;
}

//******************************************************************************************
void	CPCMemBuffer::DeclareInvalidData(CThing *t)
{
	char	text[300];

	sprintf(text, "Invalid data in Thing");  // can't link _GetClassName() without editor...
	CONSOLE.AddString(text);
}

//******************************************************************************************

#endif