// $Header$

// $Log$
// Revision 1.3  2000-09-19 19:24:24+01  jcl
// <>
//
// Revision 1.2  2000-08-07 15:30:21+01  jjs
// Adds DC support.
//
// Revision 1.1  2000-08-05 10:31:02+01  jjs
// Adds PC support.
//

#include	"Common.h"

#include	"Text.h"

UBYTE	CText::mData[TEXT_BUFFER_SIZE];
UBYTE	*CText::mDataPtr;
SINT	CText::mNumStrings;

BOOL	loaded_already = FALSE;

//****************************************************************
void	CText::Load(SINT language)
{
	if (loaded_already)
		return;

	char	str[256];
	char	lang[10];
	
	switch(language)
	{
	case	LANG_ENGLISH:	sprintf(lang, "ENGLISH");	break;	
	case	LANG_FRENCH:	sprintf(lang, "FRENCH");	break;	
	case	LANG_GERMAN:	sprintf(lang, "GERMAN");	break;	
	case	LANG_SPANISH:	sprintf(lang, "SPANISH");	break;	
	case	LANG_ITALIAN:	sprintf(lang, "ITALIAN");	break;
	}
	
#if TARGET == PC
	DWORD bytesRead;

	sprintf(str, "data\\LANGUAGE\\%s.DAT", lang);

	HANDLE	size = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (size == INVALID_HANDLE_VALUE)
	{
//		printf("Can't open text file %s\n", str);
		return;
	}
	
//	return;

	ReadFile(size,(ULONG*)mData, TEXT_BUFFER_SIZE, &bytesRead,NULL);  // load it all in
	
	mNumStrings = *((SINT *)mData);
	mDataPtr = mData + (mNumStrings * 4) + 4;
	
	CloseHandle(size);
#elif TARGET == DC
	sprintf(str, "LANGUAGE\\%s.DAT", lang);

	SINT	size = DCOpenFile(str);
	if (size == -1)
	{
//		printf("Can't open text file %s\n", str);
		return;
	}
	
//	return;

	DCLoadPartFile((ULONG*)mData, size);  // load it all in
	
	mNumStrings = *((SINT *)mData);
	mDataPtr = mData + (mNumStrings * 4) + 4;
	
	DCCloseFile();
#else
	sprintf(str, "LANGUAGE\\%s.DAT", lang);

	SINT	size = PSXOpenFile(str);
	if (size == -1)
	{
//		printf("Can't open text file %s\n", str);
		return;
	}
	
//	return;

	PSXLoadPartFile((ULONG*)mData, size);  // load it all in
	
	mNumStrings = *((SINT *)mData);
	mDataPtr = mData + (mNumStrings * 4) + 4;
	
	PSXCloseFile();
#endif
}

//****************************************************************
void	CText::GetString(SINT number, char *buffer)
{
	SINT	*index = (SINT *)(mData + 4); // first is num strings

	UBYTE	*ptr;

	if ((number < 0) || (number >= mNumStrings))
		ptr = mDataPtr;
	else
		ptr = mDataPtr + index[number];
	
	sprintf(buffer, "%s", ptr);
}
