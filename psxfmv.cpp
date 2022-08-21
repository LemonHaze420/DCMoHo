#include	"Common.h"

#if TARGET == PSX

#include	"Globals.h"
#include	"PSXFMV.h"

//*****************************************************************************

#define STR_VOL	64

StrInfo STR_ROCKSTAR =
{
		"\\MOHODATA\\VIDEO\\ROCKSTAR.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		158,
		100000,
		STR_VOL
};

StrInfo STR_TAKE2 =
{
		"\\MOHODATA\\VIDEO\\T2INTRO.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		43,
		100000,
		STR_VOL
};

StrInfo STR_LOSTTOYS =
{
		"\\MOHODATA\\VIDEO\\LOSTLOGO.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		147,
		100000,
		STR_VOL
};

StrInfo STR_INTRO_ENG =
{
		"\\MOHODATA\\VIDEO\\INTENG.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL
};

StrInfo STR_INTRO_FRE =
{
		"\\MOHODATA\\VIDEO\\INTFREN.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL
};

StrInfo STR_INTRO_GER=
{
		"\\MOHODATA\\VIDEO\\INTGERM.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL
};

StrInfo STR_INTRO_SPA =
{
		"\\MOHODATA\\VIDEO\\INTSPAN.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL
};

StrInfo STR_INTRO_ITA =
{
		"\\MOHODATA\\VIDEO\\INTITA.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		960,
		100000,
		STR_VOL
};

StrInfo STR_ANGEL =
{
		"\\MOHODATA\\VIDEO\\ANGELS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		254,
		100000,
		STR_VOL
};

StrInfo STR_APOSTLE =
{
		"\\MOHODATA\\VIDEO\\APOSTS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		265,
		100000,
		STR_VOL
};

StrInfo STR_BENNY =
{
		"\\MOHODATA\\VIDEO\\BENNS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		267,
		100000,
		STR_VOL
};

StrInfo STR_LOCKDOWN =
{
		"\\MOHODATA\\VIDEO\\LOCKS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		278,
		100000,
		STR_VOL
};

StrInfo STR_SOPHIE =
{
		"\\MOHODATA\\VIDEO\\SOPHS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		257,
		100000,
		STR_VOL
};

StrInfo STR_DOCTOR =
{
		"\\MOHODATA\\VIDEO\\DOCS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		298,
		100000,
		STR_VOL
};

StrInfo STR_ALPHASTAR_IN =
{
		"\\MOHODATA\\VIDEO\\ALPHINT.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		227,
		100000,
		STR_VOL
};

StrInfo STR_ALPHASTAR_ANGEL =
{
		"\\MOHODATA\\VIDEO\\ANGELOS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL
};

StrInfo STR_ALPHASTAR_APOSTLE =
{
		"\\MOHODATA\\VIDEO\\APOSTOS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL
};

StrInfo STR_ALPHASTAR_BENNY =
{
		"\\MOHODATA\\VIDEO\\BENNOS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL
};

StrInfo STR_ALPHASTAR_LOCKDOWN =
{
		"\\MOHODATA\\VIDEO\\LOCKOS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL
};

StrInfo STR_ALPHASTAR_SOPHIE =
{
		"\\MOHODATA\\VIDEO\\SOPHOS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL
};

StrInfo STR_ALPHASTAR_DOCTOR =
{
		"\\MOHODATA\\VIDEO\\DOCOS.STR;1",
		STR_MODE24,
		STR_BORDERS_OFF,
		320,
		0,32,
		320,192,
		371,
		100000,
		STR_VOL
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
		DrawSync(0);
		VSync(0);
		DrawSync(0);
		VSync(0);
		DrawSync(0);
		VSync(0);

		PLATFORM.ClearPools();
		SOUND.StopTrack();

/*		RECT	r;
		r.x = 0;
		r.y = 0;
		r.w = 513;
		r.h = 513;
		ClearImage(&r, 0, 0, 0);*/
		PLATFORM.ClearScreen(0);
		PLATFORM.ClearScreen(1);
		

		DrawSync(0);
		VSync(0);
		VSync(0);
		VSync(0);
		VSync(0);

		printf("About to play movie: %s\n", stream->strName);
		
		switch(movie)
		{
		case FMV_ALPHASTAR_IN:
		case FMV_ALPHASTAR_ANGEL:
		case FMV_ALPHASTAR_APOSTLE:
		case FMV_ALPHASTAR_BENNY:
		case FMV_ALPHASTAR_LOCKDOWN:
		case FMV_ALPHASTAR_SOPHIE:
		case FMV_ALPHASTAR_DOCTOR:
			ret = PlayStream(stream, &dont_test_key);
			break;
			
		default:
			ret = PlayStream(stream, &test_key);
			break;
		};

		PLATFORM.ClearPools();
		PLATFORM.InitFrames();
	}
	return ret;
}

//*****************************************************************************
#endif

