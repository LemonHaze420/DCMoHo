#ifndef RTFence_H
#define	RTFence_H

//******************************************************************************************
#include	"RenderThing.h"
#include	"Texture.h"

//******************************************************************************************
DECLARE_RENDERTHING_CLASS(CRTFence, CRenderThing)
public:
	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render();

#ifdef LINK_EDITOR
	virtual	void	AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma);
	virtual	void	FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt);

	virtual	void	AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma);
	virtual	void	FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt);
#endif

protected:
	CTextureGTEXAdditive	*mTex;
};

#endif
