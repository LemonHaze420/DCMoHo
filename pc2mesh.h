#ifndef PSX_MESH_H
#define PSX_MESH_H

#include	"MemBuffer.h"
#include	"PSXRenderData.h"
#include	"MeshCommon.h"
#include	"Animation.h"
#include 	"psxengine.h"

struct	SPSXMeshVertex
{
//	GVector	mVertex;
	SVECTOR	mVertex;
};

struct	SPSXMeshNormal
{
//	GVector	mNormal;
	SVECTOR	mNormal;
};

struct	SPSXMeshInputGTriangle
{
	SWORD		mVert0, mVert1, mVert2;
	SWORD		mNorm0, mNorm1, mNorm2;

	ULONG		mDiffuse0, mDiffuse1, mDiffuse2;
};

struct	SPSXMeshInputTTriangle
{
	SWORD		mTexBitDepth, mTexAlphaMode;
	SWORD		mTPageX, mTPageY;
	SWORD		mClutX, mClutY;

	UBYTE		mU0, mV0;
	UBYTE		mU1, mV1;
	UBYTE		mU2, mV2;

	UWORD		pad;
};

struct  SPSXMeshGTriangle
{
    SWORD		mVerts[3];
	SWORD		mNorms[3];
};

struct	SPSXMeshTTriangle
{
    UBYTE       mU0, mV0;               // Pull as long and write to POLY_GT3.u0
    SWORD       mClut;
    UBYTE       mU1, mV1;               // Pull as long and write to POLY_GT3.u1
    SWORD       mTpage;
    UBYTE       mU2, mV2;               // Pull as long and write to POLY_GT3.u2
	UWORD		pad;
};

class	CPSXMesh;

struct	SPSXMeshPart
{
	void	Read(CMEMBUFFER &mem, CPSXMesh	*parent);
	void	GenerateSimpleBoundingBox();
	void	EvaluateCollisionVolumeHierarchy(GVector pos, GMatrix ori, CThing *t);

	void	Render(IRenderableThing *t, SRTInfo *ri, const GVector &pos, const GMatrix &ori);

	SPSXMeshVertex		**mVertices;
	SPSXMeshNormal		*mNormals;
	SPSXMeshGTriangle	*mGTriangles;
	SPSXMeshTTriangle	**mTTriangles;
	GINT				*mVFrames;

	SINT				mNumVertices;
	SINT				mNumNormals;
	SINT				mNumTriangles;
	SINT				mNumVFrames;
	SINT				mNumAFrames;
	SINT				mNumSkins;

	SINT				mPartNumber;

	SINT				mNumChildren;
	SPSXMeshPart		*mChildren[MAX_CHILDREN];
	SPSXMeshPart	 	*mParent;

	GVector				mOffsetPos;
	GMatrix				mOrientation;

	CPSXMesh			*mBaseMesh;

	//!MESH	 Complex Collision Volume stuff to follow.
	CCollisionVolume	*mCollisionVolume;
	GINT				mRadius;
};

class	CPSXAnimMode
{
public:
	EAnimMode		mMode;
	int				mStart;
	int				mLast;
	int				mFrameScaler; // last-start for normal anims, last-start+1 for loops
	GINT			mRate;
};

class	CPSXEmitter
{
public:
	SPSXMeshPart	*mMeshPart;
	GVector			mPos;
	SINT			mTrackingVertex;
};

class	CPSXMesh
{
public:
	friend struct SPSXMeshPart;

	void	Read(CMEMBUFFER &mem);

	static	void	RenderMeshHierarchy(IRenderableThing *t, SRTInfo *ri, GVector p, GMatrix ori, SPSXMeshPart *mp);
	static	void	RenderFromData(IRenderableThing *t, SRTInfo *ri, BOOL light = TRUE);
	CCollisionVolume	*GetCollisionVolume();
	void				EvaluateCollisionVolume(CThing *t);

	void 	RenderAtPos(const GVector &pos,const GMatrix &ori);

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
	SPSXMeshPart		**mParts;
	SINT				mNumParts;

	CPSXEmitter			*mEmitters;
	SINT				mNumEmitters;

	CPSXAnimMode		*mAnimModes;
	SINT				mNumAnimModes;


	SINT		mLastFrameCollisionGenerated;

};


#endif
