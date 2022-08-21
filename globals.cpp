#include	"Common.h"

#include	"Globals.h"

CConsole		CONSOLE;
CControls		CONTROLS;

#if TARGET == PC
	CPCSoundManager SOUND;
	CPCMap			MAP;
	CPCGame			GAME;
	CJCLEngine		ENGINE;
	CPCWorld		WORLD;
	CPlatform		PLATFORM;
	CSeedUniformRandomNumberStream	RANDOMSTREAM(123456);
#elif TARGET == PSX
	CPSXSoundManager SOUND;
	CPSXMap			MAP;
	CPSXGame		GAME;
	CPSXEngine		ENGINE;
	CPSXWorld		WORLD;
	CPSXPlatform	PLATFORM;
	CSeedUniformRandomNumberStream	RANDOMSTREAM(123456);
#elif TARGET == DC
	CDCSoundManager SOUND;
	CDCMap			MAP;
	CDCGame		GAME;
	CDCEngine		ENGINE;
	CDCWorld		WORLD;
	CDCPlatform	PLATFORM;
	CSeedUniformRandomNumberStream	RANDOMSTREAM(123456);
#endif
