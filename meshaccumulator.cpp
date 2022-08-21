#include	"Common.h"

#ifdef LINK_EDITOR

#include	"MeshAccumulator.h"
#include	"Mesh.h"
#include	"Globals.h"

//******************************************************************************************
void	CMeshAccumulator::AddMesh(CMesh *m)
{
	// have we got this already?
	SINT	c0;

	for (c0 = 0; c0 < mNumMeshes; c0 ++)
	{
		if (!(stricmp(mMeshes[c0]->mName, m->mName)))  // better not just compare pointers...
			return; // yup - so don't add
	}


	if (mNumMeshes < MA_MAX_MESHES)
	{
		mMeshes[mNumMeshes] = m;
		mNumMeshes ++;
	}
}

//******************************************************************************************
SINT	CMeshAccumulator::GetMeshIndex(CMesh *m)
{
	if (!m)
		return -1;

	SINT	c0;

	for (c0 = 0; c0 < mNumMeshes; c0 ++)
	{
		if (!(stricmp(mMeshes[c0]->mName, m->mName)))  // better not just compare pointers...
			return c0;
	}

	return -1; // didn't find it - oops
}

//******************************************************************************************
void	CMeshAccumulator::AddVertex(FVector &v)
{
	if (mMAMesh.mNumVertices < MA_MAX_MA_VERTICES)
	{
		SINT	c0;

		for (c0 = 0; c0 < mMAMesh.mNumVertices; c0 ++)
		{
			if ((v - mMAMesh.mVertices[c0].mVector).Magnitude() < 0.01f)
				return;	// don't need to add - already close
		}

		mMAMesh.mVertices[mMAMesh.mNumVertices].mVector = v;
		mMAMesh.mNumVertices ++;
	}
}

//******************************************************************************************
void	CMeshAccumulator::AddNormal(FVector &v)
{
	if (mMAMesh.mNumNormals < MA_MAX_MA_NORMALS)
	{
		SINT	c0;

		for (c0 = 0; c0 < mMAMesh.mNumNormals; c0 ++)
		{
			if ((v - mMAMesh.mNormals[c0].mNormal).Magnitude() < 0.01f)
				return;	// don't need to add - already close
		}

		mMAMesh.mNormals[mMAMesh.mNumNormals].mNormal = v;
		mMAMesh.mNumNormals ++;
	}
}

//******************************************************************************************
SINT	CMeshAccumulator::GetVertex(FVector &v)
{
	SINT	c0;

	for (c0 = 0; c0 < mMAMesh.mNumVertices; c0 ++)
	{
		if ((v - mMAMesh.mVertices[c0].mVector).Magnitude() < 0.01f)
			return c0; // that'll do!
	}

	ASSERT(0);
	return 0; // ah well.
}

//******************************************************************************************
SINT	CMeshAccumulator::GetNormal(FVector &v)
{
	SINT	c0;

	for (c0 = 0; c0 < mMAMesh.mNumNormals; c0 ++)
	{
		if ((v - mMAMesh.mNormals[c0].mNormal).Magnitude() < 0.01f)
			return c0; // that'll do!
	}

	ASSERT(0);
	return 0; // ah well.
}

//******************************************************************************************
void	snap_uv(float &uv)
{
	SINT count = 0;

	if (uv > 0)
	{
		while (uv > 0)
		{
			uv -= 1.f;
			count ++;
		}
	}
	else
	{
		while (uv < 0)
		{
			uv += 1.f;
			count --;
		}
	}

	if (FABS(uv) < 0.01f)
		uv = 0;
	if (FABS(uv - 1.f) < 0.01f)
		uv = 1.f;
	if (FABS(uv + 1.f) < 0.01f)
		uv = -1.f;

	uv += float(count);
}

void	fix_max_uvs(float &uv0, float &uv1, float &uv2)
{
	/////// I hate functions like this

	// some polygons are mapped from 3.9999 to 5.0001
	// try to convert this to 0.0 to 1.0

	// first snap to nearest unit if close
	snap_uv(uv0);
	snap_uv(uv1);
	snap_uv(uv2);

	// then find the lowest value and shift up / down.
	float l = uv0;

	if (uv1 < l) l = uv1;
	if (uv2 < l) l = uv2;

	float fl = float(floor(l));

	uv0 -= fl;
	uv1 -= fl;
	uv2 -= fl;

//	ASSERT(uv0 >= 0);
//	ASSERT(uv1 >= 0);
//	ASSERT(uv2 >= 0);

//	ASSERT(uv0 <= 1.f);
//	ASSERT(uv1 <= 1.f);
//	ASSERT(uv2 <= 1.f);

	// bollox

	if (uv0 > 1.f) uv0 = 1.f;
	if (uv1 > 1.f) uv1 = 1.f;
	if (uv2 > 1.f) uv2 = 1.f;
}


//******************************************************************************************
void	CMeshAccumulator::CompileAndWriteMeshes(CTextureAccumulator &ta, CMEMBUFFER &mem)
{
	// ok - here's the fun bit
	// take a CMesh, and convert it into a sensible PSX mesh format,
	// read texture information directly from the TA and store native UV, TPage and blending information

	// write out header stuff
	UINT	CID_MESH	= MKID("MESH");
	UINT	MESH_VER	= (MKID("1.00"));

	mem.Write(&CID_MESH, 4);	// write out chunk header
	mem.Write(&MESH_VER, 4);	// write out verson number

	mem.Write(&mNumMeshes, 4);  // write out number of meshes

	SINT	c0, c1, c2, c3;

	// convert each mesh in turn
	for (c0 = 0; c0 < mNumMeshes; c0 ++)
	{
		// OK - new stuff - we need to establish the length of the mesh so that the PSX reader
		// can skip over the mesh if we don't want to read it.
		// SO - create a temporary membuffer, and write the data to that, and then we know the length,
		// then copy the temporary file back to the real membuffer

		CMEMBUFFER	tempmem;
		tempmem.InitFromMem();

		CMesh	*m = mMeshes[c0];

		// write out number of parts
		tempmem.Write(&m->mNumParts, 4);

		// write out each individual part
		for (c2 = 0; c2 < m->mNumParts; c2 ++)
		{
			mMAMesh.Init();  // reset destination mesh buffer
			CMeshPart *mp = m->mParts[c2];

			// Compile vertex list
			// not necessary now...
			//! back in for test
			for (c1 = 0; c1 < mp->mNumPVertices; c1 ++)
				AddVertex(mp->mPVertices[0][c1]);

			// Compile normal list
			for (c1 = 0; c1 < mp->mNumDVertices; c1 ++)
				AddNormal(mp->mDVertices[c1].mNormal);

			// Great!  Now build the trangle list, pulling out all the relevant information.
			for (c1 = 0; c1 < mp->mNumTriangles; c1 ++)
			{
				SMAGTriangle		*gt  = &mMAMesh.mGTriangles[c1];
				CMeshTriangle		*mt = &mp->mTriangles[c1];

				// first, find the vertices
				gt->mVert0 = mt->mA->mPVert;
				gt->mVert1 = mt->mB->mPVert;
				gt->mVert2 = mt->mC->mPVert;

				// Then, the normals
				gt->mNorm0 = GetNormal(mt->mA->mNormal);
				gt->mNorm1 = GetNormal(mt->mB->mNormal);
				gt->mNorm2 = GetNormal(mt->mC->mNormal);

				// diffuse colours
				gt->mDiffuse0 = mt->mA->mDiffuse;
				gt->mDiffuse1 = mt->mB->mDiffuse;
				gt->mDiffuse2 = mt->mC->mDiffuse;

				// and the texture information
				SINT	sk;
				for (sk = 0; sk < m->mNumSkins; sk ++)
				{
					SMATTriangle		*tt  = &mMAMesh.mTTriangles[sk][c1];

					// get texture reference from texture accumulator
					STATexture	*tex = ta.GetTextureRef(TATT_MESH, mt->mA->mTexture[sk]);

					if (tex)
					{
						// first. generic texture information
						tt->mTexBitDepth = tex->mDepth;
						tt->mTexAlphaMode = 1;			//! whatever...
						tt->mTPageX = tex->mTPageX;
						tt->mTPageY = tex->mTPageY;
						tt->mClutX  = tex->mClutX;
						tt->mClutY  = tex->mClutY;

						// and now the uv information.

						// aaaaaaaargh - need to fix up dodgy UVs from MAX...

						float	u0 = mt->mA->mU;
						float	u1 = mt->mB->mU;
						float	u2 = mt->mC->mU;
						float	v0 = mt->mA->mV;
						float	v1 = mt->mB->mV;
						float	v2 = mt->mC->mV;

						fix_max_uvs(u0, u1, u2);
						fix_max_uvs(v0, v1 ,v2);

						SINT	texw = tex->mW - 1;
						SINT	texh = tex->mH - 1;

						tt->mU0 = UBYTE(SINT((tex->mX << 2) +        SINT(float(texw) * u0)));
						tt->mV0 = UBYTE(SINT((tex->mY     ) + texh - SINT(float(texh) * v0)));
						tt->mU1 = UBYTE(SINT((tex->mX << 2) +        SINT(float(texw) * u1)));
						tt->mV1 = UBYTE(SINT((tex->mY     ) + texh - SINT(float(texh) * v1)));
						tt->mU2 = UBYTE(SINT((tex->mX << 2) +        SINT(float(texw) * u2)));
						tt->mV2 = UBYTE(SINT((tex->mY     ) + texh - SINT(float(texh) * v2)));
					}
					else
					{
						//!er.. god knows what to do here
						ASSERT(0);
					}
				}

				mMAMesh.mNumTriangles ++;
			}

			// marvellous - all converted.  Now we need to write out the mesh

			// first the vertices
			tempmem.Write(&mp->mNumPVertices, 4);

			tempmem.Write(&mp->mNumVFrames, 4);
			tempmem.Write(&mp->mNumAFrames, 4);

			for (c1 = 0; c1 < mp->mNumVFrames; c1 ++)
			{
				GINT	g = FToG(mp->mVFrames[c1]);

				tempmem.Write(&g, 4);
			}

			for (c3 = 0; c3 < mp->mNumAFrames; c3 ++)
			{
				for (c1 = 0; c1 < mp->mNumPVertices; c1 ++)
				{
					// convert to dest type
					//!PSX think about this type!
					GVector	gv = FVectorToGVector(mp->mPVertices[c3][c1]);
					WORD	x = gv.X.GetVal() >> 8;
					WORD	y = gv.Y.GetVal() >> 8;
					WORD	z = gv.Z.GetVal() >> 8;

					tempmem.Write(&x, 2);
					tempmem.Write(&y, 2);
					tempmem.Write(&z, 2);
					//mem.Write(&z, 2); // dummy...
				}
			}

			// then the normals
			tempmem.Write(&mMAMesh.mNumNormals, 4);

			for (c1 = 0; c1 < mMAMesh.mNumNormals; c1 ++)
			{
				// convert to dest type
				//!PSX think about this type!
				GVector	gv = FVectorToGVector(mMAMesh.mNormals[c1].mNormal);
				WORD	x = gv.X.GetVal() >> 4;
				WORD	y = gv.Y.GetVal() >> 4;
				WORD	z = gv.Z.GetVal() >> 4;

				tempmem.Write(&x, 2);
				tempmem.Write(&y, 2);
				tempmem.Write(&z, 2);
				//mem.Write(&z, 2); // dummy...
			}

			// finally the triangles
			tempmem.Write(&mMAMesh.mNumTriangles, 4);
			tempmem.Write(&m->mNumSkins, 4);

			for (c1 = 0; c1 < mMAMesh.mNumTriangles; c1 ++)
			{
				tempmem.Write(&mMAMesh.mGTriangles[c1], sizeof(SMAGTriangle));
			}

			SINT	sk;
			for (sk = 0; sk < m->mNumSkins; sk ++)
			{
				for (c1 = 0; c1 < mMAMesh.mNumTriangles; c1 ++)
				{
					tempmem.Write(&mMAMesh.mTTriangles[sk][c1], sizeof(SMATTriangle));
				}
			}

			// OK - vertex data written.
			// now write out the remaining stuff

			tempmem.Write(&mp->mPartNumber, 4);					// part number (not necessary, but there for safety)

			tempmem.Write(&mp->mNumChildren, 4);				// number of children

			SINT	pn;
			for (c1 = 0; c1 < mp->mNumChildren; c1 ++)		// children nodes
			{
				if (mp->mChildren[c1])
					pn = mp->mChildren[c1]->mPartNumber;
				else
					pn = -1;

				tempmem.Write(&pn, 4);
			}

			if (mp->mParent)								// parent node
				pn = mp->mParent->mPartNumber;
			else
				pn = -1;

			tempmem.Write(&pn, 4);

			GVector	gv = FVectorToGVector(mp->mOffsetPos);
			tempmem.Write(&gv, sizeof(gv));						// Offset Position

			GMatrix gm = FMatrixToGMatrix(mp->mOrientation);
			tempmem.Write(&gm, sizeof(gm));						// Offset Orientation

			//!MESH Collision volume to come
		}

		// Emitters
		tempmem.Write(&m->mNumEmitters, 4);
		for (c1 = 0; c1 < m->mNumEmitters; c1++)
		{
			tempmem.Write(&m->mEmitters[c1].mMeshPart->mPartNumber, 4);

			GVector v = FVectorToGVector(m->mEmitters[c1].mPos);
			tempmem.Write(&v, sizeof(v));

			tempmem.Write(&m->mEmitters[c1].mTrackingVertex, 4);
		}

		// Animation stuff
		tempmem.Write(&m->mNumAnimModes, 4);

		tempmem.Write(m->mAnimModes, sizeof(CAnimMode) * m->mNumAnimModes);


		// OK - all done.  now write out the real data.
		SINT	len = tempmem.ConvertFromWritingToReading();
		mem.Write(&len, 4);
		SINT	foo = m->GetCharacterNumber();
		mem.Write(&foo, 4);

		mem.Write(tempmem.GetData(), len);

//		char text[200];
//		sprintf(text, "Output Mesh: %s, size %d\n", m->mName, len);
//		OutputDebugString(text);
	}

}


#endif
