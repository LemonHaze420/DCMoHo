#ifndef PSXPARTICLE_SYSTEM_H
#define PSXPARTICLE_SYSTEM_H

//******************************************************************************************

#include "DCEngine.h"
//******************************************************************************************
class CDCParticleSystem
{
public:
	static	void	RenderFromData(CThing *t, const SRTInfo &ri);
	static	void	Render(CParticleSystem *p);

};

#endif



