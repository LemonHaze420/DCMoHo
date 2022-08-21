#ifndef PSXDECAL_H
#define PSXDECAL_H

#include	"PSXEngine.h"

class	CPSXDecal
{
public:
	static	void	RenderFromData(CThing *t, const SRTInfo &ri);

	static 	void	CommonDecalRender(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT z_fudge = 0);
	static 	void	CommonDecalRenderHoriz(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT z_fudge = 0, GINT yaw = G0);
	static 	void	CommonDecalRenderHoriz2(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT sn,SINT cs);
};






#endif
