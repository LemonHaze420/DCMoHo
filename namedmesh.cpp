#include	"Common.h"

#include	"NamedMesh.h"
#include	"Globals.h"

//******************************************************************************************
void	CNamedMesh::Init(CInitThing *i)
{
	mMesh = i->mMeshNo;  // has to be done first else the renderthing creation fucks up

	SUPERTYPE::Init(i);

	StickToGround();
}

//******************************************************************************************
#if TARGET == PC
void	CNamedMesh::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	i.mMeshNo = ENGINE.GetRelocatedMesh(mMesh);  // post-packed meshes.
}

#endif
//******************************************************************************************
