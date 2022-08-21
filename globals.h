#ifndef GLOBALS_H
#define GLOBALS_H

#include	"Console.h"
#include	"Controls.h"

extern	CConsole		CONSOLE;
extern	CControls		CONTROLS;

#if TARGET == PC

	#include	"PCPlatform.h"
	extern	CPlatform		PLATFORM;

	#include	"PCGame.h"
	extern	CPCGame			GAME;

	#include	"PCMap.h"
	extern	CPCMap			MAP;

	#include	"JCLEngine.h"
	extern	CJCLEngine		ENGINE;

	#include	"PCWorld.h"
	extern	CPCWorld		WORLD;

	#include	"PCSoundManager.h"
	extern	CPCSoundManager SOUND;

	#include	"SeedUniformRandomNumberStream.h"
	extern CSeedUniformRandomNumberStream	RANDOMSTREAM;
#elif TARGET == DC

	#include	"DCPlatform.h"
	extern	CDCPlatform	PLATFORM;

	#include	"DCGame.h"
	extern	CDCGame		GAME;

	#include	"DCMap.h"
	extern	CDCMap			MAP;

	#include	"DCEngine.h"
	extern	CDCEngine		ENGINE;

	#include	"DCWorld.h"
	extern	CDCWorld		WORLD;

	#include	"DCSoundManager.h"
	extern	CDCSoundManager SOUND;

	#include	"SeedUniformRandomNumberStream.h"
	extern CSeedUniformRandomNumberStream	RANDOMSTREAM;

#elif TARGET == PSX

	#include	"PSXPlatform.h"
	extern	CPSXPlatform	PLATFORM;

	#include	"PSXGame.h"
	extern	CPSXGame		GAME;

	#include	"PSXMap.h"
	extern	CPSXMap			MAP;

	#include	"PSXEngine.h"
	extern	CPSXEngine		ENGINE;

	#include	"PSXWorld.h"
	extern	CPSXWorld		WORLD;

	#include	"PSXSoundManager.h"
	extern	CPSXSoundManager SOUND;

	#include	"SeedUniformRandomNumberStream.h"
	extern CSeedUniformRandomNumberStream	RANDOMSTREAM;

#endif

#define PLAYER(n)	GAME.GetPlayer(n)

#endif

