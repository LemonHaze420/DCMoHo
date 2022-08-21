/*
 *	PS_Debug_Prefix.h	- Target #DEFINEs for the Sony PlayStation
 *
 *	Copyright © 1996 metrowerks inc. All Rights Reserved.
 *
 */

#ifndef __PS_Debug_prefix__
#define __PS_Debug_prefix__

/* general target hardware/os definitions */
#define __dest_os	__PSXOS
#define TARGET_LITTLE_ENDIAN
#define TARGET_HEAP_ALIGNMENT	16
#define TARGET_DEVICE		__PS_PSX
//#include "mwprof.h"

#define _PROFILE

#if __option(wchar_type)
	#define _WCHAR_T
#endif

#define VERPSX

#endif