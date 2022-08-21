#include	"Common.h"

#if TARGET == DC

#include	"Globals.h"
#include	"DCFMV.h"
#include  	"maincode.h"
//*****************************************************************************

#define STR_VOL	64

StrInfo STR_ROCKSTAR =
{
		"ROCKSTAR.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		158,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_TAKE2 =
{
		"T2INTRO.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		43,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_LOSTTOYS =
{
		"LOSTLOGO.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		147,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_BROADSWORD =
{
		"BSLOGO.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		147,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_INTRO_ENG =
{
		"INTENG.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_INTRO_FRE =
{
		"INTFREN.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_INTRO_GER=
{
		"INTGERM.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_INTRO_SPA =
{
		"INTSPAN.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_INTRO_ITA =
{
		"INTITA.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_ANGEL =
{
		"ANGELS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		254,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_APOSTLE =
{
		"APOSTS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		265,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_BENNY =
{
		"BENNS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		267,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_LOCKDOWN =
{
		"LOCKS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		278,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_SOPHIE =
{
		"SOPHS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		257,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_DOCTOR =
{
		"DOCS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		298,
		100000,
		STR_VOL,
		TRUE
};

StrInfo STR_ALPHASTAR_IN =
{
		"ALPHINT.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		227,
		100000,
		STR_VOL,
		FALSE
};

StrInfo STR_ALPHASTAR_ANGEL =
{
		"ANGELOS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL,
		FALSE
};

StrInfo STR_ALPHASTAR_APOSTLE =
{
		"APOSTOS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL,
		FALSE
};

StrInfo STR_ALPHASTAR_BENNY =
{
		"BENNOS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL,
		FALSE
};

StrInfo STR_ALPHASTAR_LOCKDOWN =
{
		"LOCKOS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL,
		FALSE
};

StrInfo STR_ALPHASTAR_SOPHIE =
{
		"SOPHOS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL,
		FALSE
};

StrInfo STR_ALPHASTAR_DOCTOR =
{
		"DOCOS.SFD",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL,
		FALSE
};



//*****************************************************************************
short test_key(void)
{
	struct  SPadData *pad_data = PLATFORM.GetPadData(0);

    unsigned long PadValue=(~pad_data->buttons)&0xffff;
    PadValue=((PadValue>>8)&0x00ff)|((PadValue<<8)&0xff00);

	return (short)PadValue;
}

short dont_test_key(void)
{
	return 0;
}

//*****************************************************************************
SINT	PlayFMV(EMovie movie)
{
	StrInfo	*stream = NULL;
	SINT	ret = PLAYSTR_ERROR;



	switch(movie)
	{
//	case FMV_ROCKSTAR:				stream = &STR_ROCKSTAR;				break;
	case FMV_ROCKSTAR:				stream = &STR_TAKE2;				break;
	case FMV_LOSTTOYS:				stream = &STR_LOSTTOYS;				break;
	case FMV_BROADSWORD:			stream = &STR_BROADSWORD;			break;
	case FMV_INTRO_ENG:				stream = &STR_INTRO_ENG;			break;
	case FMV_INTRO_FRE:				stream = &STR_INTRO_FRE;			break;
	case FMV_INTRO_GER:				stream = &STR_INTRO_GER;			break;
	case FMV_INTRO_SPA:				stream = &STR_INTRO_SPA;			break;
	case FMV_INTRO_ITA:				stream = &STR_INTRO_ITA;			break;
	case FMV_ANGEL:					stream = &STR_ANGEL;				break;
	case FMV_APOSTLE:				stream = &STR_APOSTLE;				break;
	case FMV_BENNY:					stream = &STR_BENNY;				break;
	case FMV_LOCKDOWN:				stream = &STR_LOCKDOWN;				break;
	case FMV_SOPHIE:				stream = &STR_SOPHIE;				break;
	case FMV_DOCTOR:				stream = &STR_DOCTOR;				break;
	case FMV_ALPHASTAR_IN:			stream = &STR_ALPHASTAR_IN;			break;
	case FMV_ALPHASTAR_ANGEL:		stream = &STR_ALPHASTAR_ANGEL;		break;
	case FMV_ALPHASTAR_APOSTLE:		stream = &STR_ALPHASTAR_APOSTLE;	break;
	case FMV_ALPHASTAR_BENNY:		stream = &STR_ALPHASTAR_BENNY;		break;
	case FMV_ALPHASTAR_LOCKDOWN:	stream = &STR_ALPHASTAR_LOCKDOWN;	break;
	case FMV_ALPHASTAR_SOPHIE:		stream = &STR_ALPHASTAR_SOPHIE;		break;
	case FMV_ALPHASTAR_DOCTOR:		stream = &STR_ALPHASTAR_DOCTOR;		break;
	};

	if (stream)
	{
		if (PlayMovie(stream->strName,0,48,640,384,stream->KeyQuit))
			return PLAYSTR_USER;
		else
			return PLAYSTR_END;			
	}
	return ret;
}

//*****************************************************************************
#endif

