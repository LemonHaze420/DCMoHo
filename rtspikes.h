#ifndef RTSpikes_H
#define	RTSpikes_H

//******************************************************************************************
#include	"RenderThing.h"
#include	"Texture.h"

//******************************************************************************************
DECLARE_RENDERTHING_CLASS(CRTSpikes, CRenderThing)
public:
	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render();

protected:
	void	DrawSpike(FVector top, FVector bottom);


	CTextureGTEX	*mTex;
};

#endif