#ifndef PSXPARTICLE_SYSTEM_H
#define PSXPARTICLE_SYSTEM_H

//******************************************************************************************

#include "PSXEngine.h"
//******************************************************************************************
class CPSXParticleSystem
{
public:
	static	void	RenderFromData(CThing *t, const SRTInfo &ri);
	static	void	Render(CParticleSystem *p);

};

#endif



