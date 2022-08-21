#include "libetc.h"
#include <graph.h>
#include <conio.h>

long PadInit( u_long mode )
{
	if( mode != 0 )
		return -1;		// only type avaulable is the keyboard
	return 1;
}



long PadRead( u_short id )
{
	int c;

	if( id != 1 )	// only the keyboard is attached
		return 0;

	if( (kbhit()) != 0 )
	{
		c = (long)getch();
		switch( c )
		{
			case 'w':
				return PADLup;
				break;
			case 'x':
				return PADLdown;
				break;		
			case 'a':
				return PADLleft;
				break;		
			case 'd':
				return PADLright;
				break;		
			case 'i':
				return PADRup;
				break;		
			case 'm':
				return PADRdown;
				break;		
			case 'j':
				return PADRleft;
				break;		
			case 'l':
				return PADRright;
				break;		
			case '5':
				return PADi;
				break;		
			case '6':
				return PADj;
				break;	 
			case ' ':
				return PADk;
				break;	 
			case '7':
				return PADl;
				break;	 
			case '8':
				return PADm;
				break;	 
			case '4':
				return PADn;
				break;	 
			case '3':
				return PADo;
				break;	 
			case 'b':
				return PADh;
				break;		
		}
	}
	return 0;
}



long PadStatus( u_short mode )
{
}


