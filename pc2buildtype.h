// This file chooses the type of build to create. This has been separated off from the usual PSX Prefix file so
//	that a re-build can occur without recompiling the entire bladdy projectg! :o)

// (JCL) - this activates debug controls
//#define ALLOW_DEBUG_CONTROLS

// (JCL) - this allows the select + R2 cheat
//#define	ALLOW_SIMPLE_CHEAT

// This define will make the file system access the CD drive for game data files...
#define USE_CD

// This define will allow CD-A / XA to play
#define ENABLE_CDA

// This define selects which screen MODE_ to use  (PAL/NTSC)
#define DISP_MODE MODE_PAL
//#define DISP_MODE MODE_NTSC

// This define (if 1) will make the game UNformat the memory card in slot 1 when the game is run
#define FORCE_CARD_UNFORMAT 0

// (JCL) - this will force an ageing test, running through all the levels.
//#define AGEING

// (JCL) - this will cause each level played to be recorded.
//#define RECORD_GAME

// (JCL) - this is for demos -- note: this should be derived from the build settings
//#define DEMO_GAME 1			// 1 = ROLLING DEMO FOR 17/4/00
//#define DEMO_GAME 2			// 2 = ROLLING DEMO FOR 17/4/00 - continuous
//#define DEMO_GAME 3			// 3 = Playable demo with competition support
//#define DEMO_GAME 4			// 4 = Playable demo with competition support - standalone

// (JCL) - this is for a press preview build with limited levels available.
//#define PRESS_PREVIEW

// (JCL) - this lets the camera move freely in pause mode
#define FREE_LOOK

//Define this to stop that annoying 6 second wait before you debug!
//#define DISABLE_LIBCRYPT
