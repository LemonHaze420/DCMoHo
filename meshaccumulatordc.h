#ifndef MESH_ACCUMULATOR_DC_H
#define MESH_ACCUMULATOR_DC_H

#define MADC_MAX_MESHES	256

#include	"MemBuffer.h"
#include	"TextureAccumulatorDC.h"
#include	"MeshCommon.h"

class	CMesh;

#define	MADC_MAX_MA_VERTICES		5000
#define	MADC_MAX_MA_NORMALS			5000
#define	MADC_MAX_MA_TRIANGLES		5000

struct	SMADCVertex
{
	FVector	mVector;
};

struct	SMADCNormal
{
	FVector	mNormal;
};

struct	SMADCGTriangle
{
	SWORD		mVert0, mVert1, mVert2;
	SWORD		mNorm0, mNorm1, mNorm2;

	ULONG		mDiffuse0, mDiffuse1, mDiffuse2;
};

struct	SMADCTTriangle
{
	SINT		mTPageNo;

	float		mU0, mV0;
	float		mU1, mV1;
	float		mU2, mV2;

	UWORD		pad;
};

class	CMADCMeshPart
{
public:
	void	Init()
	{
		mNumVertices = mNumNormals = mNumTriangles = 0;
	}

	SMADCVertex		mVertices[MADC_MAX_MA_VERTICES];
	SMADCNormal		mNormals[MADC_MAX_MA_VERTICES];
	SMADCGTriangle	mGTriangles[MADC_MAX_MA_TRIANGLES];
	SMADCTTriangle	mTTriangles[MAX_SKINS][MADC_MAX_MA_TRIANGLES];

	SINT		mNumVertices;
	SINT		mNumNormals;
	SINT		mNumTriangles;
};


class	CMeshAccumulatorDC
{
public:
	void	Init() {mNumMeshes = 0;};

	void	AddMesh(CMesh *m);

	void	CompileAndWriteMeshes(CTextureAccumulatorDC &ta, CMEMBUFFER &mem);

	SINT	GetMeshIndex(CMesh *m);

protected:
	void	AddVertex(FVector &v);
	void	AddNormal(FVector &v);

	SINT	GetVertex(FVector &v);
	SINT	GetNormal(FVector &v);

	SINT	mNumMeshes;

	CMesh	*mMeshes[MADC_MAX_MESHES];

	CMADCMeshPart	mMAMesh;
};






#endif