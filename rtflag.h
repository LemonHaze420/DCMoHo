#ifndef RTFlag_H
#define	RTFlag_H

//******************************************************************************************
#include	"RenderThing.h"
#include	"Texture.h"

//******************************************************************************************
DECLARE_RENDERTHING_CLASS(CRTFlag, CRenderThing)
public:
	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render();

protected:
	CTextureGTEX	*mTex;
};

#endif