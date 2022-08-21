#include	"Common.h"

#include	"SampleList.h"

BOOL	IsSampleLooping(SINT n)
{
	switch (n)
	{
	case SOUND_FLAME1:
	case SOUND_GENERATOR:
	case SOUND_RUMBLE:
	case SOUND_CROWD1:	
	case SOUND_CROWD2:	
	case SOUND_CROWD3:	
	case SOUND_CROWD4:	
	case SOUND_CROWD5:	
	case SOUND_CROWD6:	
	case SOUND_CROWD7:
	case SOUND_BUZZ:
		
		return TRUE;

	default:
		return FALSE;

	};
}
