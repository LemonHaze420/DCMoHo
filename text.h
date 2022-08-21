// $Header$

// $Log$
// Revision 1.1  2000-08-05 10:32:00+01  jjs
// Fixes for PC code.
//

#ifndef TEXT_H
#define TEXT_H

// international text manager (JCL) 21/2/00

//#include	"TextNumbers.h"
#include	"TextList.h"
#if defined(LANG_ENGLISH)
#undef	LANG_ENGLISH
#undef	LANG_FRENCH
#undef	LANG_GERMAN
#undef	LANG_SPANISH
#undef	LANG_ITALIAN
#endif

#define	LANG_ENGLISH	0
#define	LANG_FRENCH		1
#define	LANG_GERMAN		2
#define	LANG_SPANISH	3
#define	LANG_ITALIAN	4

#define DEFAULT_LANGUAGE	LANG_ENGLISH	
#define MAX_LANGUAGE_INDEX	LANG_ITALIAN

#define 	TEXT_BUFFER_SIZE	10000

class	CText
{
public:

	static	void	Load(SINT language);
	static	void	GetString(SINT number, char *buffer);

protected:
	static	UBYTE	mData[TEXT_BUFFER_SIZE];
	static	UBYTE	*mDataPtr;
	static	SINT	mNumStrings;
};

#endif