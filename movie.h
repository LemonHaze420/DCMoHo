// $Header$

// $Log$
// Revision 1.2  2000-08-08 10:36:46+01  sds
// Latest build fix
//
// Revision 1.1  2000-08-05 10:33:05+01  jjs
// First PC milestone.
//

/************************************************************
 *                                                          *
 *                       movie.h                            *
 *                                                          *
 *                                                          *
 *               Vince Diesi     13/02/97                   *
 *                                                          *
 *   Copyright (C) 1997 Sony Computer Entertainment Inc.    *
 *                  All Rights Reserved                     *
 *                                                          *
 ***********************************************************/

#ifndef __MOVIE_H
#define __MOVIE_H

/* ---------------------------------------------------------------------------
 * - CONSTANTS
 * ---------------------------------------------------------------------------
 */

// Streaming modes.
#define STR_MODE24				1
#define STR_MODE16				0


// Border modes.
#define STR_BORDERS_OFF			0
#define STR_BORDERS_ON			1


// PlayStream return values.
#define	PLAYSTR_END				1
#define	PLAYSTR_USER			2
#define	PLAYSTR_ERROR			3

/* ---------------------------------------------------------------------------
 * - DATA TYPE AND STRUCTURE DECLARATIONS
 * ---------------------------------------------------------------------------
 */
#if TARGET == PSX || TARGET == DC
typedef struct {
		char		*strName;					// Stream file name.
		short		mode;						// 24-Bit or 16-Bit streaming.
		short		drawBorders;				// != 0 if borders on.
		u_short		scrWidth;					// Screen res width.
		u_short		x;							// X,Y position.
		u_short		y;
		u_short		width;						// Stream width and height.
		u_short		height;
		u_long		endFrame;					// Last frame No.
		u_long		vlcBufSize;					// Size of each VLC buffer (including header).
		u_short		volume;						// Left and Right ADPCM volume.
#if TARGET == DC
		BOOL		KeyQuit;
#endif		
} StrInfo;
#elif TARGET == PC
typedef struct {
		char		*strName;					// Stream file name.
		short		x;							// X,Y position.
		short		y;
		short		width;						// Stream width and height.
		short		height;
} StrInfo;
#endif
/* ---------------------------------------------------------------------------
 * - PUBLIC FUNCTION PROTOTYPES
 * ---------------------------------------------------------------------------
 */

extern "C" short PlayStream(StrInfo *str, short (*keyHandler)(void));

/* ------------------------------------------------------------------------ */

#endif // __MOVIE_H
