#ifndef MESH_ACCUMULATOR_H
#define MESH_ACCUMULATOR_H

#define MA_MAX_MESHES	256

#include	"MemBuffer.h"
#include	"TextureAccumulator.h"
#include	"MeshCommon.h"

class	CMesh;

#define	MA_MAX_MA_VERTICES		5000
#define	MA_MAX_MA_NORMALS		5000
#define	MA_MAX_MA_TRIANGLES		5000

struct	SMAVertex
{
	FVector	mVector;
};

struct	SMANormal
{
	FVector	mNormal;
};

struct	SMAGTriangle
{
	SWORD		mVert0, mVert1, mVert2;
	SWORD		mNorm0, mNorm1, mNorm2;

	ULONG		mDiffuse0, mDiffuse1, mDiffuse2;
};

struct	SMATTriangle
{
	SWORD		mTexBitDepth, mTexAlphaMode;
	SWORD		mTPageX, mTPageY;
	SWORD		mClutX, mClutY;

	UBYTE		mU0, mV0;
	UBYTE		mU1, mV1;
	UBYTE		mU2, mV2;

	UWORD		pad;
};

class	CMAMeshPart
{
public:
	void	Init()
	{
		mNumVertices = mNumNormals = mNumTriangles = 0;
	}

	SMAVertex		mVertices[MA_MAX_MA_VERTICES];
	SMANormal		mNormals[MA_MAX_MA_VERTICES];
	SMAGTriangle	mGTriangles[MA_MAX_MA_TRIANGLES];
	SMATTriangle	mTTriangles[MAX_SKINS][MA_MAX_MA_TRIANGLES];

	SINT		mNumVertices;
	SINT		mNumNormals;
	SINT		mNumTriangles;
};


class	CMeshAccumulator
{
public:
	void	Init() {mNumMeshes = 0;};

	void	AddMesh(CMesh *m);

	void	CompileAndWriteMeshes(CTextureAccumulator &ta, CMEMBUFFER &mem);

	SINT	GetMeshIndex(CMesh *m);

protected:
	void	AddVertex(FVector &v);
	void	AddNormal(FVector &v);

	SINT	GetVertex(FVector &v);
	SINT	GetNormal(FVector &v);

	SINT	mNumMeshes;

	CMesh	*mMeshes[MA_MAX_MESHES];

	CMAMeshPart	mMAMesh;
};

#endif
