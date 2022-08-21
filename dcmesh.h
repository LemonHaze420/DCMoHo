#ifndef DC_MESH_H
#define DC_MESH_H

#include	"MemBuffer.h"
#include	"DCRenderData.h"
#include	"MeshCommon.h"
#include	"Animation.h"
#include 	"dcengine.h"

struct	SDCMeshVertex
{
//	GVector	mVertex;
//SDS	SVECTOR	mVertex;
	FVector	mVertex;
	
};

struct	SDCMeshNormal
{
//	GVector	mNormal;
//SDS	SVECTOR	mNormal;
	FVector	mNormal;
};

struct	SDCMeshInputGTriangle
{
	SWORD		mVert0, mVert1, mVert2;
	SWORD		mNorm0, mNorm1, mNorm2;

	ULONG		mDiffuse0, mDiffuse1, mDiffuse2;
};

struct	SDCMeshInputTTriangle
{
	/* SDS
	SWORD		mTexBitDepth, mTexAlphaMode;
	SWORD		mTPageX, mTPageY;
	SWORD		mClutX, mClutY;

	UBYTE		mU0, mV0;
	UBYTE		mU1, mV1;
	UBYTE		mU2, mV2;
	*/
	SINT 		mTPageNo;
	float		mU0, mV0;
	float		mU1, mV1;
	float		mU2, mV2;
	
	UWORD		pad;
};

struct  SDCMeshGTriangle
{
    SWORD		mVerts[3];
	SWORD		mNorms[3];
};

struct	SDCMeshTTriangle
{
    float       mU0, mV0;               // Pull as long and write to POLY_GT3.u0
    SWORD       mClut;
    float       mU1, mV1;               // Pull as long and write to POLY_GT3.u1
    SINT       mTpage;
    float       mU2, mV2;               // Pull as long and write to POLY_GT3.u2
	UWORD		pad;
};

class	CDCMesh;

struct	SDCMeshPart
{
	void	Read(CMEMBUFFER &mem, CDCMesh	*parent);
	void	GenerateSimpleBoundingBox();
	void	EvaluateCollisionVolumeHierarchy(FVector pos, FMatrix ori, CThing *t);

	void	Render(IRenderableThing *t, SRTInfo *ri, const FVector &pos, const FMatrix &ori);

	SDCMeshVertex		**mVertices;
	SDCMeshNormal		*mNormals;
	SDCMeshGTriangle	*mGTriangles;
	SDCMeshTTriangle	**mTTriangles;
	GINT				*mVFrames;

	SINT				mNumVertices;
	SINT				mNumNormals;
	SINT				mNumTriangles;
	SINT				mNumVFrames;
	SINT				mNumAFrames;
	SINT				mNumSkins;

	SINT				mPartNumber;

	SINT				mNumChildren;
	SDCMeshPart		*mChildren[MAX_CHILDREN];
	SDCMeshPart	 	*mParent;

	//GVector				mOffsetPos;
	//GMatrix				mOrientation;
	
	FVector				mOffsetPos;
	FMatrix				mOrientation;

	CDCMesh			*mBaseMesh;

	//!MESH	 Complex Collision Volume stuff to follow.
	CCollisionVolume	*mCollisionVolume;
	GINT				mRadius;
	
	// JCL - texture list
	SINT				mTexturesUsed[100]; // 100 = safety!
	SINT				mNumTexturesUsed;
};

class	CDCAnimMode
{
public:
	EAnimMode		mMode;
	int				mStart;
	int				mLast;
	int				mFrameScaler; // last-start for normal anims, last-start+1 for loops
	GINT			mRate;
};

class	CDCEmitter
{
public:
	SDCMeshPart	*mMeshPart;
	//SDS GVector			mPos;
	FVector			mPos;	
	
	SINT			mTrackingVertex;
};

class	CDCMesh
{
public:
	friend struct SDCMeshPart;

	void	Read(CMEMBUFFER &mem);

	static	void	RenderMeshHierarchy(IRenderableThing *t, SRTInfo *ri, FVector p, FMatrix ori, SDCMeshPart *mp);
	static	void	RenderFromData(IRenderableThing *t, SRTInfo *ri, BOOL light = TRUE);
	CCollisionVolume	*GetCollisionVolume();
	void				EvaluateCollisionVolume(CThing *t);

	void 	RenderAtPos(const FVector &pos,const FMatrix &ori);

	GINT	GetFrameIncrement(EAnimMode a, SINT* realindex);

	GVector	GetEmitter(SINT no, CThing *t);
	GVector	GetEmitterAnimated(SINT no, CThing *t);

	void	Clear()
	{
		mNumParts = 0;
		mNumEmitters = 0;
		mNumAnimModes = 0;
		mLastFrameCollisionGenerated = -2;
	};

	GINT				mRadius;

private:
	SDCMeshPart		**mParts;
	SINT				mNumParts;

	CDCEmitter			*mEmitters;
	SINT				mNumEmitters;

	CDCAnimMode		*mAnimModes;
	SINT				mNumAnimModes;


	SINT		mLastFrameCollisionGenerated;

};


#endif
