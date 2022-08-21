// $Header$

// $Log$
// Revision 1.4  2000-09-25 09:35:30+01  are
// Spanish is now Spanish again.
//
// Revision 1.3  2000-09-23 11:17:55+01  jjs
// Changed Spanish to OtherOne so it will compile.
//

#include "Common.h"

#if TARGET == PC
#ifndef _OKAYTOMAP_H_
#define _OKAYTOMAP_H_

#include <dinput.h>

struct tagDIKToString
{
	int		DIK;			// DirectInput Key - obviously!
	DWORD	KeyboardType;	// Keyboard type.
	char	English[32];	// These need reworking. (Minor change).
	char	French[32];
	char	German[32];
	char	Spanish[32];
	char	Italian[32];
};

#define		DIK7KEYLISTLENGTH	(144)
#define		MAX_KEY_INDEX		(256)
extern struct tagDIKToString DIKToString[DIK7KEYLISTLENGTH];
char* DIScanCodeToKeyString(int _DIScanCode, int _Language, DWORD _DevType);

#endif
#endif