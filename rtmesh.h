#ifndef RTMesh_H
#define	RTMesh_H

//******************************************************************************************
#include	"RenderThing.h"
#include	"Mesh.h"

//******************************************************************************************
DECLARE_RENDERTHING_CLASS(CRTMesh, CRenderThing)
	friend class CFrontEndThing;
public:
	virtual	void	Init(CInitRenderThing *i);
	virtual	void	Render();

	static	void	RenderMeshPart(FVector p, FMatrix &ori, CMeshPart *mesh_part, CThing *t, EWaveResponse wr, SINT skin = 0);
			void	RenderMeshHierarchy(FVector p, FMatrix ori, CMeshPart *mp);
			void	RenderMesh(FVector &p, FMatrix &ori, CMesh *mesh);
	static	void	RenderFEMeshPart(FVector p, FMatrix &ori, CMeshPart *mesh_part, IRenderableThing *t, EWaveResponse wr, SINT skin);
			void	RenderFEMeshHierarchy(FVector p, FMatrix ori, CMeshPart *mp, IRenderableThing *t);
			void	RenderFEMesh(FVector &p, FMatrix &ori,IRenderableThing *t);
			void	RenderShadow(FVector &p);

	virtual	GINT	GetRTRadius() {return mRadius;};

	virtual	CCollisionVolume	*GetCollisionVolume()
	{
		mMesh->EvaluateCollisionVolume(mThing);
		if (mMesh->mParts && mMesh->mParts[0])
			return mMesh->mParts[0]->mCollisionVolume;
		else
			return NULL;
	};

	virtual	void	RenderCollisionVolume(SINT i);

	virtual	GVector	GetRTEmitter(SINT no);
	virtual	GVector	GetRTEmitterAnimated(SINT no);
	virtual	GINT	GetFrameIncrement(EAnimMode a, SINT* realindex);

#ifdef LINK_EDITOR
	virtual	void	AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma);
	virtual	void	FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt);

	virtual	void	AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma);
	virtual	void	FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt);
#endif

protected:
	CMesh			*mMesh, *mMesh2;
	CTextureGTEX	*mTexShadow;
	EWaveResponse	mWaveResponse;

	GINT			mRadius;
	EShadowType		mShadow;

	SINT			mSkin;
};

#endif