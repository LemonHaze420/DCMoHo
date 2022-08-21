#ifndef PCFMV_H
#define PCFMV_H

enum	EMovie
{
	FMV_ROCKSTAR = 0,
	FMV_LOSTTOYS,
	FMV_BROADSWORD,
	FMV_INTRO_ENG,
	FMV_INTRO_FRE,
	FMV_INTRO_GER,
	FMV_INTRO_SPA,
	FMV_INTRO_ITA,
	FMV_ANGEL,
	FMV_APOSTLE,
	FMV_BENNY,
	FMV_LOCKDOWN,
	FMV_SOPHIE,
	FMV_DOCTOR,
	FMV_ALPHASTAR_IN,
	FMV_ALPHASTAR_ANGEL,
	FMV_ALPHASTAR_APOSTLE,
	FMV_ALPHASTAR_BENNY,
	FMV_ALPHASTAR_LOCKDOWN,
	FMV_ALPHASTAR_SOPHIE,
	FMV_ALPHASTAR_DOCTOR,
};

// PlayStream return values.
#define	PLAYSTR_END				1
#define	PLAYSTR_USER			2
#define	PLAYSTR_ERROR			3

// ----------------------------------------------------------------------------
// Attempts to obtain the resolution 640, 480, 32 if _Mode is set to true
// Setting _Mode to false returns to the default resolution.
void	FMVResChange(bool _Mode);

extern	SINT	PlayFMV(EMovie movie);

#endif