#ifndef RTPARTICLE_SYSTEM_H
#define RTPARTICLE_SYSTEM_H

//******************************************************************************************
#include	"RenderThing.h"
#include	"Texture.h"

//******************************************************************************************
DECLARE_RENDERTHING_CLASS(CRTParticleSystem, CRenderThing)
public:
	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render();//CParticleSystem *p);
	
	static	void	Render(CParticleSystem *ps);

#ifdef LINK_EDITOR
	virtual	void	AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma);
//	virtual	void	FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt);

	virtual	void	AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma);
//	virtual	void	FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt);
#endif

	static CTextureGTEXAdditive		*GetAnimTexture(SINT n);
	static CTextureGTEXAdditive		*mTexWhite; 

protected:
	
	// static particle textures
	static	CTextureGTEXAdditive	*mTexFire1; 
	static	CTextureGTEXAdditive	*mTexFire2; 
	static	CTextureGTEXAdditive	*mTexFire3; 
	static	CTextureGTEXAdditive	*mTexStar1; 
	static	CTextureGTEXAdditive	*mTexStar2; 
	static	CTextureGTEXAdditive	*mTexSStar1; 
	static	CTextureGTEXAdditive	*mTexSStar2;
	static	CTextureGTEXAdditive	*mTexExp00;
	static	CTextureGTEXAdditive	*mTexExp01;
	static	CTextureGTEXAdditive	*mTexExp02;
	static	CTextureGTEXAdditive	*mTexExp03;
	static	CTextureGTEXAdditive	*mTexExp04;
	static	CTextureGTEXAdditive	*mTexExp05;
	static	CTextureGTEXAdditive	*mTexExp06;
	static	CTextureGTEXAdditive	*mTexExp07;
	static	CTextureGTEXAdditive	*mTexExp08;
	static	CTextureGTEXAdditive	*mTexExp09;
	static	CTextureGTEXAdditive	*mTexExp10;
	static	CTextureGTEXAdditive	*mTexExp11;
	static	CTextureGTEXAdditive	*mTexExp12;
	static	CTextureGTEXAdditive	*mTexExp13;
	static	CTextureGTEXAdditive	*mTexExp14;
	static	CTextureGTEXAdditive	*mTexExp15;
	static	CTextureGTEXAdditive	*mTexWat00;
	static	CTextureGTEXAdditive	*mTexWat01;
	static	CTextureGTEXAdditive	*mTexWat02;
	static	CTextureGTEXAdditive	*mTexWat03;
	static	CTextureGTEXAdditive	*mTexWat04;
	static	CTextureGTEXAdditive	*mTexWat05;
	static	CTextureGTEXAdditive	*mTexBubble;
	static	CTextureGTEXAdditive	*mTexZ;
	static	CTextureGTEXAdditive	*mTexRing;
	static	CTextureGTEXAdditive	*mTexSmoke;
	static	CTextureGTEXAdditive	*mTexShockwave;
	static	CTextureGTEXAdditive	*mTexHit1;
	static	CTextureGTEXAdditive	*mTexHit2;
	static	CTextureGTEXAdditive	*mTexInner;
	static	CTextureGTEXAdditive	*mTexMiddle;
	static	CTextureGTEXAdditive	*mTexOuter;
	static	CTextureGTEXAdditive	*mTexFlashSM;
};

#endif



