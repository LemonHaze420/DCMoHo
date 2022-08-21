#ifndef RTDECAL_H
#define	RTDECAL_H

//******************************************************************************************
#include	"RenderThing.h"
#include	"Texture.h"

//******************************************************************************************
DECLARE_RENDERTHING_CLASS(CRTDecal, CRenderThing)
public:
	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render();

	virtual	void	DrawShadow();

	static 	void	CommonDecalRender(GVector *pos, GINT render_radius, CTextureGTEXAdditive *tr, long col, SINT z_fudge = 0);
	static 	void	CommonDecalRenderHoriz(GVector *pos, GINT render_radius, CTextureGTEXAdditive *tr, long col, SINT z_fudge = 0, GINT yaw = G0);
	static 	void	CommonDecalRenderHoriz2(GVector *pos, GINT render_radius, CTextureGTEXAdditive *tr, long col, SINT sn,SINT cs);

#ifdef LINK_EDITOR
	virtual	void	AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma);
	virtual	void	FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt);

	virtual	void	AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma);
	virtual	void	FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt);
#endif

protected:
	CTextureGTEXAdditive	*mTex;
	CTextureGTEXAdditive	*mTex2;
	CTextureGTEX			*mTex3;
	EShadowType				mShadow;

	BOOL					mAlpha;
	BOOL					mHoriz;
};

#endif