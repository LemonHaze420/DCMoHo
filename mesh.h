#ifndef MESH_H
#define MESH_H

#include	"Texture.h"
#include	"CollisionVolume.h"

#include	"MeshCommon.h"

#include	"TextureAccumulator.h"
#include	"MeshAccumulator.h"
#include	"TextureAccumulatorDC.h"
#include	"MeshAccumulatorDC.h"
#include	"Animation.h"

class	CMeshDVertex
{
public:
	SINT			mPVert;
	float			mU, mV;
	DWORD			mDiffuse;
	CTextureGTEX	*mTexture[MAX_SKINS];
	FVector			mNormal;

	SINT			mIndex; // temp for CTexture::AddVertex;
};

class	CMeshTriangle
{
public:
	CMeshDVertex	*mA, *mB, *mC;
};

class	CMeshPart : public IMeshPart
{
public:
	CMeshPart();
	~CMeshPart();

	BOOL			InitMemory(SINT ndv, SINT npv, SINT nt, SINT nvf, SINT naf);
	void			Read201(CMEMBUFFER &mem, CMesh *parent, CTextureGTEX **textures[], SINT num_textures, CTextureGTEX *dt, SINT num_skins);
	void			Read202(CMEMBUFFER &mem, CMesh *parent, CTextureGTEX **textures[], SINT num_textures, CTextureGTEX *dt, SINT num_skins);
	void			GenerateSimpleBoundingBox();
	void			HierarchyAdjust(FVector pos, FMatrix ori);
	void			EvaluateCollisionVolumeHierarchy(FVector pos, FMatrix ori, CThing *t);
	void			EvaluateNormals();

	virtual	void	EvaluatePointVelocity(GVector pos, GVector &velocity, CThing *t);

	SINT			FindVertex(FVector pos);

	SINT			mPartNumber;  // which part I am

	SINT			mNumChildren;
	CMeshPart		*mChildren[MAX_CHILDREN];
	CMeshPart		*mParent;

	FVector			mOffsetPos;
	FMatrix			mOrientation;

	SINT			mNumDVertices;
	SINT			mNumPVertices;
	SINT			mNumTriangles;
	SINT			mNumAFrames;
	SINT			mNumVFrames;

	CMeshDVertex	*mDVertices;
	FVector			**mPVertices;
	CMeshTriangle	*mTriangles;
	float			*mVFrames;

	CCollisionVolume	*mCollisionVolume;

	GINT			mRadius;

	CMesh			*mBaseMesh;
};

class	CEmitter
{
public:
	CMeshPart		*mMeshPart;
	FVector			mPos;
	FMatrix			mOrientation;
	SINT			mTrackingVertex;
};

class	CAnimMode
{
public:
	EAnimMode		mMode;
	int				mStart;
	int				mLast;
	int				mFrameScaler; // last-start for normal anims, last-start+1 for loops
	GINT			mRate;
};

class	CMesh
{
public:
	friend class CMeshAccumulator;
	friend class CMeshAccumulatorDC;

	CMesh();
	~CMesh();

	BOOL				InitFromFile(char *name, char *def_tex, SINT num_skins);

	SINT				mNumParts;
	CMeshPart			**mParts;

	SINT				mNumEmitters;
	CEmitter			*mEmitters;

	SINT				mNumAnimModes;
	CAnimMode*			mAnimModes;

	static	CMesh		*GetMesh(char *name, char *def_tex, SINT num_skins = 1);

	static	BOOL		InitAll();
	static	void		ReleaseAll();

	CCollisionVolume	*GetCollisionVolume();
	void				EvaluateCollisionVolume(CThing *t);

	void				GetEmitter(SINT no, FVector &pos, FMatrix &ori, CThing *t);
	void				GetEmitterAnimated(SINT no, FVector &pos, FMatrix &ori, CThing *t);
	virtual	GINT		GetFrameIncrement(EAnimMode a, SINT* realindex);

	GINT				Radius() {return mRadius;};

#ifdef LINK_EDITOR
	void				AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma);
	void				AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma);
#endif

	char				mName[300];
	SINT				GetCharacterNumber(); // nice hack

private:
	static	CMesh		*mFirstMesh;
	CMesh				*mNextMesh;

	GINT				mRadius;

	static	CTextureGTEX *mDefaultTexture;

	SINT				mLastFrameCollisionGenerated;

	SINT				mNumSkins;
};

#endif