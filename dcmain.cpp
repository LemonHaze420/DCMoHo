#include	"Common.h"

#if TARGET == DC

#include	"Globals.h"
#include	"DCBuildType.h"
//#include 	"maincode.h"

//#include <__rts_info_t__.h>

//here it is!

//int __stdcall WinMain(int hInstance, int hPrevInstance, char *lpCmdLine, int nCmdShow)
/*
int main(int argc, char **argv)
{
#if DEMO_GAME == 1 || DEMO_GAME == 3
   int timeout, mode;             
   int start_track,			  
   
   
   

   
   
    num_tracks;

    mode = ((int *)argv)[0];
    timeout = ((int *)argv)[1];
    start_track = ((int *)argv)[2];
    num_tracks = ((int *)argv)[3];

	printf("*******************\n");
	printf("MOHO Playable Demo starting\n");
	printf("*******************\n");
   
#endif


	GAME.Init();
	GAME.Run();
	printf("Got back from game!\n");
	GAME.Shutdown();
	printf("Managed to shut down!\n");

#if DEMO_GAME == 1 || DEMO_GAME == 3
	printf("*******************\n");
	printf("MOHO Playable Demo returning control to Stub\n");
	printf("*******************\n");	
#endif
	
	return 0;
}
*/


#endif
