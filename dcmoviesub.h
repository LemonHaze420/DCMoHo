#ifndef DCMovieSub_h
#define DCMovieSub_h
#include "common.h"
#include "globals.h"
#include "dcfmv.h"
#include	<shinobi.h>
#include	<sg_mw.h>
#include	<string.h>
#include 	<am.h>
#include 	<sg_chain.h>
#include	"usr.h"

#define bool BOOL
extern void PlayMovie(char *fname,int x,int y,int xsize,int ysize,BOOL Keypress);
extern void SubInitMovie(void);
#endif