#ifndef RTLightning_H
#define	RTLightning_H

//******************************************************************************************
#include	"RenderThing.h"
#include	"Texture.h"

//******************************************************************************************
DECLARE_RENDERTHING_CLASS(CRTLightning, CRenderThing)
public:
	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render();

protected:
	CTextureGTEXAdditive	*mTex;
};

#endif