#include	"Common.h"

#include	"CLIParams.h"

#define MAX_PARMS 30
#define PARM_LENGTH 256


void	CCLIParams::GetParams(char *text, CCLIParams &p)
{
#if TARGET == PC
	char	parms[MAX_PARMS][PARM_LENGTH];

	SINT	num_parms = 0;
	SINT	x = 0;
	parms[num_parms][x] = 0;


	// break into parms	
	while ((parms[num_parms][x] = *text) != NULL)
	{
		if (*text == ' ')
		{
			if (x != 0) // skip multiple whitespace
			{
				if (num_parms < MAX_PARMS)
				{
					parms[num_parms][x] = 0;
					num_parms ++;
					x = 0;
				}
				else 
					break;
			}
		}
		else
		{
			if (x < PARM_LENGTH)
				x ++;
			else
			{
				if (num_parms < MAX_PARMS)
				{
					parms[num_parms][x] = 0;
					num_parms ++;
					x = 0;
				}
				else 
					break;
			}
		}
		text ++;
	}

	// scan parms
	SINT current_parm = 0;

	while (current_parm < num_parms)
	{
		if (!(stricmp(parms[current_parm], "-level")))
		{
			current_parm ++;
			sscanf(parms[current_parm], "%d", &p.mLevel);
		}
		else if (!(stricmp(parms[current_parm], "-controller")))
		{
			current_parm ++;
			sscanf(parms[current_parm], "%d", &p.mController);
		}
		else if (!(stricmp(parms[current_parm], "-joymode")))
		{
			current_parm ++;
			sscanf(parms[current_parm], "%d", &p.mJoyMode);
		}
		else if (!(stricmp(parms[current_parm], "-character")))
		{
			current_parm ++;
			sscanf(parms[current_parm], "%d", &p.mStartCharacter);
		}

		// else if ...

		current_parm++;
	};	

#elif TARGET == PSX
	//!PSX write this!

#endif
}
