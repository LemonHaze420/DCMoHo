#ifndef RTSpringMesh_H
#define	RTSpringMesh_H

//******************************************************************************************
#include	"RenderThing.h"
#include	"Mesh.h"

//******************************************************************************************
DECLARE_RENDERTHING_CLASS(CRTSpringMesh, CRenderThing)
public:
	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render();

protected:
	CTextureGTEX	*mBallTex;
	CTextureGTEX	*mWallTex;
};

#endif