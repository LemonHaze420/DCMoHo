#ifndef GAMEPANEL_H
#define	GAMEPANEL_H

#if TARGET == PC
#include	"PCGamePanel.h"
#elif TARGET ==DC
#include	"DCGamePanel.h"
#elif TARGET == PSX
#include	"PSXGamePanel.h"
#endif

#endif
