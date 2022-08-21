// $Header$

// $Log$
// Revision 1.12  2000-10-04 12:18:34+01  jjs
// Removes PC memory leaks.
//
// Revision 1.11  2000-10-02 15:26:03+01  jjs
// Removes LINK_EDITOR if NO_LINK_EDITOR defined in settings.
//
// Revision 1.10  2000-09-27 15:10:05+01  jjs
// AddedFASTFTOL to PC version.
//
// Revision 1.9  2000-09-25 08:58:25+01  jjs
// Changed include order.
//
// Revision 1.8  2000-09-14 13:02:33+01  jcl
// <>
//
// Revision 1.7  2000-09-12 10:12:45+01  sds
// Big Merge - Should work for DC and PC
//
// Revision 1.6  2000-08-07 15:46:36+01  jjs
// DC and PC merged code.
//
// Revision 1.5  2000-08-05 10:31:33+01  jjs
// PC changes.
//
// Revision 1.4  2000-07-11 11:02:33+01  jjs
// Redefined LINK_EDITOR for PC.
//
// Revision 1.3  2000-07-10 15:18:29+01  sds
// SDS - #if TARGET == DC stuff e.t.c.
//
// Revision 1.2  2000-06-30 15:28:22+01  sds
// Added Dreamcast defines.
//

#ifndef COMMON_H
#define COMMON_H

#define		PC		1
#define		PSX		2
#define 	DC		3

#ifdef VERPC
#define TARGET PC
#else
#ifdef VERPSX
#define TARGET PSX
#else
#ifdef __MWERKS__
#define TARGET DC
//#include <mw_util.h>
#else
#error no version specified...

#endif
#endif
#endif

//******************************************************************************************
//Platform specific
#if TARGET == PC
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define D3D_OVERLOADS
#define USE_FLOATING_POINT
//#if !defined(NO_LINK_EDITOR)
//#define LINK_EDITOR
//#endif
#define DIRECTINPUT_VERSION 0x0700
#include	<Windows.h>
#include	<ddraw.h>
#include	<d3d.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<memory.h>
#if defined(DEBUGMEM)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NORMALBLOCK   new( _NORMAL_BLOCK, __FILE__, __LINE__)

#endif

#elif TARGET == PSX
#include	<stdio.h>
#include	<sys/types.h>
#include	<libetc.h>
#include    <libpad.h>
#include    <libgte.h>
#include	<libgpu.h>
//#include	<gtemac.h>
//#include	<gtereg_s.h>	jjs
//#include	<inline_s.h>	jjs
#include 	<libspu.h>
#include 	<libsnd.h>
#include	<kernel.h>
#include	<libapi.h>
#include 	<libmcrd.h>
//#include  <libsn.h>
//#include	<MWDebugIO.h>	jjs

//#include	<strings.h>
#include	<rand.h>

#elif TARGET == DC
#include 	<SHC/stddef.h>
#include	<sys/types.h>
#include	<stdio.h>
/*#include	<libetc.h>
#include    <libpad.h>
#include    <libgte.h>
#include	<libgpu.h>
#include 	<libspu.h>
#include 	<libsnd.h>
#include	<kernel.h>
#include	<libapi.h>
#include 	<libmcrd.h>*/
#include	<rand.h>

#include	"JCLDCStuff.h"

#endif

#if TARGET == DC
#define USE_FLOATING_POINT
typedef int		BOOL;
//#include "dcjunk.h"
//#include "maincode.h"
#endif

//******************************************************************************************
// primary build settings
#if TARGET == PC
#define EDITOR_VERSION 2
#endif

//******************************************************************************************
//Warning stuff
#if TARGET == PC
#pragma warning( disable : 4710 4097 4100 4711)
//#pragma warning( disable : 4244)
#pragma warning( 3 : 4706)
__forceinline void FASTFTOL(long *result, float f)
{
        __asm {
                fld f
                mov eax,result
                fistp dword ptr [eax]
        }
}

__forceinline void FASTFTOL(int *result, float f)
{
        __asm {
                fld f
                mov eax,result
                fistp dword ptr [eax]
        }
}
#endif


//******************************************************************************************
//Debug stuff

#ifdef _DEBUG






#if TARGET == PC
#define ASSERT(e) 	\
if (!(e))			\
{					\
	__asm int 3		\
}
#define TRACE(x) OutputDebugString(x)
#elif TARGET == DC
extern void GoToDebugger();
#define ASSERT(e)  {if (!(e)) {GoToDebugger();}}
#define TRACE(x)
#elif TARGET == PSX
#define ASSERT(__Expression) \
  { \
   if (!(__Expression)) \
   { \
    printf("Assertion failed: \"%s\"\n\tFILE \"%s\", LINE %d\n\n", \
        #__Expression, \
        __FILE__, \
        __LINE__); \
    /*asm("break 8");*/ \
   } \
  }

#define TRACE printf
#endif
#else


#if TARGET == PC
#define ASSERT(e)
#define TRACE(x)
#elif TARGET == DC
#define ASSERT(e)
#define TRACE(x)
#elif TARGET == PSX
#define ASSERT(e)
#define TRACE printf
#endif

#endif

//******************************************************************************************

#include	"Types.h"
#include	"Utils.h"


#if TARGET == PSX
#include    "damogpu.h"                 // Need ASSERT in here...
#include	"PSXTypes.h"
#include	"PSXUtility.h" // utility functions (like string stuff)
#endif

#if TARGET == DC
//#include    "damogpu.h"                 // Need ASSERT in here...
#include	"DCTypes.h"
#include	"DCUtility.h" // utility functions (like string stuff)
#endif


#include	"FixedPt.h"
#include	"ListArray.h"
#include	"Coords.h"


#ifdef USE_FLOATING_POINT
#include	"FCoords.h"
#include	"Utils2.h"
//#error called
#endif


#define RELEASE(x) if (x) {x->Release(); x=NULL;}

//******************************************************************************************
#ifdef LINK_EDITOR
#define	DECLARE_THING_CLASS(a,b)   \
class a: public b  { typedef b SUPERTYPE; \
public: \
virtual const char *_GetClassName() { return #a ;}; \
virtual SINT _GetClassID() { return OID_##a ;}; \
private:
#else
#define	DECLARE_THING_CLASS(a,b)   \
class a: public b  { typedef b SUPERTYPE; \
public: \
virtual SINT _GetClassID() { return OID_##a ;}; \
private:
#endif

#define	DECLARE_RENDERTHING_CLASS(a,b)   \
class a: public b  { typedef b SUPERTYPE; \
public: \
virtual SINT _GetClassID() { return RTID_##a ;}; \
private:

//I wish...
//static void *_ThingFactory_##a()  { return new a;};
//or
//extern "C" CThing __declspec(dllexport) *ThingFactory_##a()	{return new ##a(); };
#if defined(DEBUGMEM)
#define new DEBUG_NORMALBLOCK
#endif
#endif
