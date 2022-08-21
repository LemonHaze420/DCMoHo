#include	"Common.h"

#if TARGET == PC

#include	"Mesh.h"
#include	"Texture.h"
#include	"MemBuffer.h"
#include	"Globals.h"
#include	"Console.h"

CMesh			*CMesh::mFirstMesh = NULL;
CTextureGTEX	*CMesh::mDefaultTexture = NULL;

//******************************************************************************************
CMeshPart::CMeshPart()
{
	mDVertices = NULL;
	mPVertices = NULL;
	mTriangles = NULL;

	mNumTriangles = 0;
	mNumDVertices = 0;
	mNumPVertices = 0;
	mNumVFrames	  = 0;
	mNumAFrames	  = 0;

	mOffsetPos = ZERO_FVECTOR;
	mOrientation = ID_FMATRIX;

	mCollisionVolume = NULL;
	mRadius	= GINT_HALF;

	mNumChildren = 0;
	mParent = NULL;
	
	mPartNumber = 0;
}

//******************************************************************************************
CMeshPart::~CMeshPart()
{
	if (mDVertices)
		delete [] mDVertices;

	if (mVFrames)
		delete [] mVFrames;

	if (mPVertices)
	{
		SINT	c0;
		for (c0 = 0; c0 < mNumAFrames; c0 ++)
		{
			if (mPVertices[c0])
				delete [] mPVertices[c0];
		}

		delete [] mPVertices;
	}

	if (mTriangles)
		delete [] mTriangles;

	while (mCollisionVolume)
	{
		CCollisionVolume *c = mCollisionVolume;
		mCollisionVolume = mCollisionVolume->mNext;
		delete c;
	}
}

//******************************************************************************************
BOOL	CMeshPart::InitMemory(SINT ndv, SINT npv, SINT nt, SINT nvf, SINT naf)
{
	mNumDVertices = ndv;
	mNumPVertices = npv;
	mNumTriangles = nt;
	mNumVFrames = nvf;
	mNumAFrames = naf;

	mDVertices = new CMeshDVertex[mNumDVertices];
	if (!mDVertices)
	{
		ASSERT(0);
		return FALSE;
	}

	mVFrames = new float[mNumVFrames];
	if (!mVFrames)
	{
		ASSERT(0);
		return FALSE;
	}

	mPVertices = new (FVector(*[mNumAFrames]));
	if (!mPVertices)
	{
		ASSERT(0);
		return FALSE;
	}

	SINT	c0;
	for (c0 = 0; c0 < mNumAFrames; c0 ++)
	{
		mPVertices[c0] = new FVector[mNumPVertices];
		if (!mPVertices[c0])
		{
			ASSERT(0);
			return FALSE;
		}
	}

	mTriangles = new CMeshTriangle[mNumTriangles];
	if (!mTriangles)
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************************
void	CMeshPart::GenerateSimpleBoundingBox()
{
	if (!mPVertices)
		return;

	// get extremities	
	float	minx =  999999.f;
	float	miny =  999999.f;
	float	minz =  999999.f;
	float	maxx = -999999.f;
	float	maxy = -999999.f;
	float	maxz = -999999.f;

	float	maxdist = 0;

	SINT		c0;
	FVector *v = mPVertices[0];
	for (c0 = 0; c0< mNumPVertices; c0++)
	{
		if (v->X < minx)	minx = v->X;
		if (v->Y < miny)	miny = v->Y;
		if (v->Z < minz)	minz = v->Z;
		if (v->X > maxx)	maxx = v->X;
		if (v->Y > maxy)	maxy = v->Y;
		if (v->Z > maxz)	maxz = v->Z;

		float	d = (*v + mOffsetPos).MagnitudeSq();
		if (d > maxdist)
			maxdist = d;

		v ++;
	}

	FVector ori  = (FVector(minx, miny, minz) + FVector(maxx, maxy, maxz)) / 2.f;
	FVector axes = (FVector(minx, miny, minz) - FVector(maxx, maxy, maxz)) / 2.f;
	axes.X = FABS(axes.X);
	axes.Y = FABS(axes.Y);
	axes.Z = FABS(axes.Z);

	GVector gori  = GVector(FToG(ori.X ), FToG(ori.Y ), FToG(ori.Z ));
	GVector gaxes = GVector(FToG(axes.X), FToG(axes.Y), FToG(axes.Z));

	mCollisionVolume = new CCollisionVolume(gori, gaxes, NULL, NULL, NULL, this);
	ASSERT(mCollisionVolume);

	mRadius = DToG(sqrt(maxdist));
}

//******************************************************************************************
void	CMeshPart::EvaluateNormals()
{
	SINT	c0;

	// evaluate normals. (potentially slow...)
	for (c0 = 0; c0 < mNumDVertices; c0 ++)
	{
		FVector n = ZERO_FVECTOR;
		SINT	nn = 0;

		SINT c1;

		for (c1 = 0; c1 < mNumTriangles; c1++)
		{
//			if ((mTriangles[c1].mA->mPVert == mDVertices[c0].mPVert) ||
//				(mTriangles[c1].mB->mPVert == mDVertices[c0].mPVert) ||
//				(mTriangles[c1].mC->mPVert == mDVertices[c0].mPVert))
			if ((mTriangles[c1].mA == &mDVertices[c0]) ||
				(mTriangles[c1].mB == &mDVertices[c0]) ||
				(mTriangles[c1].mC == &mDVertices[c0]))
			{
				// calc normal
				//! use frame 0...

				FVector normal = (mPVertices[0][mTriangles[c1].mA->mPVert] - mPVertices[0][mTriangles[c1].mB->mPVert]) ^
								 (mPVertices[0][mTriangles[c1].mA->mPVert] - mPVertices[0][mTriangles[c1].mC->mPVert]);

				normal.Normalise();

				n = n + normal;
				nn ++;
			}
		}

		if (nn == 0)
		{
			// eh?  vertex without face...
			mDVertices[c0].mNormal = FVector(1.f, 0, 0);
		}
		else
		{
			if (n.Magnitude() < 0.001f)
				mDVertices[c0].mNormal = FVector(1.f, 0, 0);  // very odd case...
			else
			{
				n.Normalise();
				mDVertices[c0].mNormal = n;
			}
		}
	}

}

//******************************************************************************************
void	CMeshPart::Read201(CMEMBUFFER &mem, CMesh *parent, CTextureGTEX **textures[], SINT num_textures, CTextureGTEX *dt, SINT num_skins)
{
	// load vertices
	SINT	c0, c1;

	for (c0 = 0; c0 < mNumDVertices; c0 ++)
	{
		mem.Read(&mDVertices[c0].mPVert, 4);
		mem.Read(&mDVertices[c0].mU, 4);
		mem.Read(&mDVertices[c0].mV, 4);
		mem.Read(&mDVertices[c0].mDiffuse, 4);

		SINT tex;
		mem.Read(&tex, 4);

		if ((tex < -2) || (tex >= num_textures))
			tex = -1; // could be 0 I suppose

		if (tex == -1)
		{
			for (c1 = 0; c1 < num_skins; c1 ++)
				mDVertices[c0].mTexture[c1] = dt;
		}
		else
		{
			for (c1 = 0; c1 < num_skins; c1 ++)
				mDVertices[c0].mTexture[c1] = *(textures[c1] + tex);
		}

		mDVertices[c0].mNormal = FVector(1.0f, 0, 0);
	}

	// set 
	for (c0 = 0; c0 < mNumVFrames; c0 ++)
		mVFrames[c0] = float(c0);

	// load PVerts
	for (c0 = 0; c0 < mNumAFrames; c0 ++)
	{
		mem.Read(mPVertices[c0], 12 * mNumPVertices);

		// transform points into local space
		SINT	c1;
		for (c1 = 0; c1 < mNumPVertices; c1 ++)
		{
			mPVertices[c0][c1] -= mOffsetPos;
			mPVertices[c0][c1] = mOrientation.Transpose() * mPVertices[c0][c1]; 
		}
	}

	// load Triangles
	for (c0 = 0; c0 < mNumTriangles; c0++)
	{
		SINT a, b, c;

		mem.Read(&a, 4);
		mem.Read(&b, 4);
		mem.Read(&c, 4);

		ASSERT(a < mNumDVertices);
		ASSERT(b < mNumDVertices);
		ASSERT(c < mNumDVertices);
		ASSERT(a >= 0);
		ASSERT(b >= 0);
		ASSERT(c >= 0);

		mTriangles[c0].mA = &mDVertices[a];
		mTriangles[c0].mB = &mDVertices[c];
		mTriangles[c0].mC = &mDVertices[b]; // inside out
	}

	EvaluateNormals();
}
//******************************************************************************************
void	CMeshPart::Read202(CMEMBUFFER &mem, CMesh *parent, CTextureGTEX **textures[], SINT num_textures, CTextureGTEX *dt, SINT num_skins)
{
	// load vertices
	SINT	c0, c1;

	for (c0 = 0; c0 < mNumDVertices; c0 ++)
	{
		mem.Read(&mDVertices[c0].mPVert, 4);
		mem.Read(&mDVertices[c0].mU, 4);
		mem.Read(&mDVertices[c0].mV, 4);
		mem.Read(&mDVertices[c0].mDiffuse, 4);

		SINT tex;
		mem.Read(&tex, 4);

		if ((tex < -2) || (tex >= num_textures))
			tex = -1; // could be 0 I suppose

		if (tex == -1)
		{
			for (c1 = 0; c1 < num_skins; c1 ++)
				mDVertices[c0].mTexture[c1] = dt;
		}
		else
		{
			for (c1 = 0; c1 < num_skins; c1 ++)
				mDVertices[c0].mTexture[c1] = *(textures[c1] + tex);
		}

		mDVertices[c0].mNormal = FVector(1.0f, 0, 0);
	}

	// load vframes
	for (c0 = 0; c0 < mNumVFrames; c0 ++)
		mem.Read(&mVFrames[c0], 4);

	// load PVerts
	for (c0 = 0; c0 < mNumAFrames; c0 ++)
	{
		mem.Read(mPVertices[c0], 12 * mNumPVertices);

		// transform points into local space
		SINT	c1;
		for (c1 = 0; c1 < mNumPVertices; c1 ++)
		{
			mPVertices[c0][c1] -= mOffsetPos;
			mPVertices[c0][c1] = mOrientation.Transpose() * mPVertices[c0][c1]; 
		}
	}

//	char	foo[100];
//	sprintf(foo, "Mesh %s: AFrames: %d\n", this->mBaseMesh->mName, mNumAFrames);
//	OutputDebugString(foo);

	// load Triangles
	for (c0 = 0; c0 < mNumTriangles; c0++)
	{
		SINT a, b, c;

		mem.Read(&a, 4);
		mem.Read(&b, 4);
		mem.Read(&c, 4);

		ASSERT(a < mNumDVertices);
		ASSERT(b < mNumDVertices);
		ASSERT(c < mNumDVertices);
		ASSERT(a >= 0);
		ASSERT(b >= 0);
		ASSERT(c >= 0);

		mTriangles[c0].mA = &mDVertices[a];
		mTriangles[c0].mB = &mDVertices[c];
		mTriangles[c0].mC = &mDVertices[b]; // inside out
	}

	EvaluateNormals();
}

//******************************************************************************************
void	CMeshPart::HierarchyAdjust(FVector pos, FMatrix ori)
{
	// sort out the hierarchy so that all transformations are relative to the parent node
	mOffsetPos -= pos;
	mOrientation = mOrientation * ori.Transpose();

	SINT	c0;

	for (c0 = 0; c0 < mNumChildren; c0 ++)
	{
		if (mChildren[c0])
			mChildren[c0]->HierarchyAdjust(mOffsetPos, mOrientation);
	}
}

//******************************************************************************************
SINT	CMeshPart::FindVertex(FVector pos)
{
	if ((!mPVertices) || (!mPVertices[0]))
		return 0;

	// Look for nearby vertex
	float	min = 99999.f;
	SINT	minn = 0;

	SINT	c0;

	for (c0 = 0; c0 < mNumPVertices; c0 ++)
	{
		float d = (mPVertices[0][c0] - pos).Magnitude();

		if (d < min)
		{
			min = d;
			minn = c0;
		}
	}

	return minn;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
CMesh::CMesh()
{
	mNumParts = 0;
	mParts = NULL;

	mNumEmitters = 0;
	mEmitters = NULL;

	mNumAnimModes = 0;
	mAnimModes = NULL;

	mRadius	= GINT_HALF;

	mLastFrameCollisionGenerated = -2;

	mNumSkins = 1;
}

//******************************************************************************************
CMesh::~CMesh()
{
	if (mParts)
	{
		SINT	c0;

		for (c0 = 0; c0 < mNumParts; c0 ++)
			delete mParts[c0];

		delete [] mParts;
	}

	if (mEmitters)
		delete [] mEmitters;
	if (mAnimModes)
		delete [] mAnimModes;

	// Note - assumes mesh is removed from linked-list elsewhere.
}

//******************************************************************************************

UINT	MESH	 = (MKID("MESH"));

BOOL	CMesh::InitFromFile(char *name, char *def_tex, SINT num_skins)
{
	mNumSkins = num_skins;

	char	fname[256] = "data\\Meshes\\";

	ASSERT(name[0]);
	strcat(fname, name);

	strcpy(mName, name);

	CTextureGTEX *dt;
	
	if (def_tex[0])
		dt = CTextureGTEX::GetTexture(def_tex);
	else
		dt = NULL;

	if (!dt)
		dt = mDefaultTexture;

	CMEMBUFFER	mem;
	if (!(mem.InitFromFile(fname)))
	{
		mem.Close();
		return FALSE;
	}

	UINT	ver;

	mem.Read(&ver, 4);
	if (ver != MESH)
	{
		mem.Close();
		return FALSE;
	}

	mem.Read(&ver, 4);

	// switch based on version
	//************************************************************
	if ((ver == MKID("2.01")) || (ver == MKID("2.02")))
	{
		// Full, hierarchical mesh.
		SINT c0, c1;

		// load textures
		CTextureGTEX		**textures[MAX_SKINS];
		SINT				numt;

		mem.Read(&numt, 4);
		if (numt)
		{
			for (c0 = 0; c0 < mNumSkins; c0 ++)
				textures[c0] = new (CTextureGTEX(*[numt]));

			ASSERT(textures);

			for (c0 = 0; c0 < numt; c0 ++)
			{
				char text[257], name[350];
				mem.Read(text, 256); // texture name

				sprintf(name, "meshtex\\%s", text);
				for (c1 = 0; c1 < mNumSkins; c1 ++)
				{
					if (mNumSkins != 1)
						name[strlen(name) - 5] = '1' + c1;
					*(textures[c1] + c0) = CTextureGTEX::GetTexture(name);
					ASSERT(*(textures[c1] + c0));
				}
			}
		}

		// and the parts
		mem.Read(&mNumParts, 4);
		ASSERT((mNumParts > 0) && (mNumParts < 100)); // whatever

		// allocate memory
		mParts = new (CMeshPart(*[mNumParts]));  // fucking C++ !
		if (!mParts)
		{
			mem.Close();
			return FALSE;
		}

		for (c0 = 0; c0 < mNumParts; c0 ++)
		{
			mParts[c0] = new CMeshPart;
			if (!mParts[c0])
			{
				mem.Close();
				return FALSE;
			}
		}

		// and read each part.
		for (c0 = 0; c0 < mNumParts; c0 ++)
		{
			CMeshPart	*mp = mParts[c0];

			mp->mPartNumber = c0;

			SINT	vd, vp, t, naf, nvf;
			mem.Read(&vd, 4);
			mem.Read(&vp, 4);
			mem.Read(&nvf, 4);

			if (ver == MKID("2.02"))
				mem.Read(&naf, 4);
			else
				naf = nvf;

			mem.Read(&t, 4);

			if (!(mp->InitMemory(vd, vp, t, nvf, naf)))
			{
				mem.Close();
				return FALSE;
			}
		
			// read offsets
			mem.Read(&mp->mOffsetPos  , sizeof(mp->mOffsetPos  ));
			mem.Read(&mp->mOrientation, sizeof(mp->mOrientation));

			if (c0 == 0)
				mp->mOrientation = ID_FMATRIX;	//!MESH - always set the base orientation to ID

			// read children
			mem.Read(&mp->mNumChildren, 4);

			ASSERT((mp->mNumChildren >= 0) && (mp->mNumChildren < MAX_CHILDREN));
			
			SINT	c;
			for (c1 = 0; c1 < mp->mNumChildren; c1 ++)
			{
				mem.Read(&c, 4);
				if ((c < 0) || (c >= mNumParts))
					mp->mChildren[c1] = NULL;
				else
					mp->mChildren[c1] = mParts[c];
			}

			// read parent
			mem.Read(&c, 4);
			if ((c < 0) || (c >= mNumParts))
				mp->mParent = NULL;
			else
				mp->mParent = mParts[c];


			mp->mBaseMesh = this;
			// read the mesh data
			if (ver == MKID("2.01"))
				mp->Read201(mem, this, textures, numt, dt, mNumSkins);
			else
				mp->Read202(mem, this, textures, numt, dt, mNumSkins);

			// read collision volumes
			SINT	nc;

			mem.Read(&nc, 4);

			ASSERT(nc == 0); //! Write this!!
		}


		// initialise emitters (here, not in the mesh part structure)

		mem.Read(&mNumEmitters, 4);
		if (mNumEmitters)
		{
			mEmitters = new CEmitter[mNumEmitters];
			if (!mEmitters)
			{
				mem.Close();
				return FALSE;
			}

			// load Emitters
			for (c0 = 0; c0 < mNumEmitters; c0++)
			{
				CEmitter	*e = &mEmitters[c0];
				SINT		c;

				// read part
				mem.Read(&c, 4);
				if ((c < 0) || (c >= mNumParts))
					e->mMeshPart = NULL;
				else
					e->mMeshPart = mParts[c];

				// and offset
				mem.Read(&e->mPos        , sizeof(FVector));
				mem.Read(&e->mOrientation, sizeof(FMatrix));

				// convert to local space
				if (e->mMeshPart)
				{
					e->mPos -= e->mMeshPart->mOffsetPos;
					e->mOrientation = e->mOrientation * e->mMeshPart->mOrientation.Transpose(); //! order??
					e->mTrackingVertex = e->mMeshPart->FindVertex(e->mPos);
				}
			}
		}
		if(!mem.IsMoreData()) mNumAnimModes = 0; // support for old files with no concept of animmodes
		else									// newer ones will just read a 0 for no anim data
		{
			char	animdesc[17];
			animdesc[16]=0;						// null termination for full 16 character names
			mem.Read(&mNumAnimModes, 4);
			if(mNumAnimModes)
			{
				mAnimModes = new CAnimMode[mNumAnimModes];
				CAnimMode* amode = mAnimModes;
				for(int i=0; i<mNumAnimModes; amode++, i++)
				{
					amode->mMode = AM_INVALID;
					mem.Read(animdesc,16);
					mem.Read(&amode->mStart,4);
					mem.Read(&amode->mLast,4);
					int	irate;
					mem.Read(&irate,4);

//					irate *= 3;

#define	AM_CHECK(X)	else if(!stricmp(animdesc,#X)) amode->mMode=AM_##X
					if(0) {}
					AM_CHECK(NONE);	
					AM_CHECK(WALK);	
					AM_CHECK(RUN);	
					AM_CHECK(STAND);	
					AM_CHECK(JUMP);	
					AM_CHECK(MID_JUMP);	
					AM_CHECK(PUNCH);	
					AM_CHECK(FALLING);	
					AM_CHECK(LANDING);
					AM_CHECK(FLINCH);
					AM_CHECK(DYING);
					AM_CHECK(WALK2STAND);
					AM_CHECK(TAILGRAB);
					AM_CHECK(SHOOT);
					AM_CHECK(SWITCH_ON);

					AM_CHECK(CHRISTAIR);
					AM_CHECK(BORED);
					AM_CHECK(WAVE);
					AM_CHECK(PUNCH2);
					AM_CHECK(PUNCH3);	
					AM_CHECK(WEAPON_ATT);
					AM_CHECK(JUMP_ATTACK);
					AM_CHECK(BLOCK);
					AM_CHECK(FLINCH_BLOCK);
					AM_CHECK(FLINCH_BEHIND);
					AM_CHECK(PUNCH_BEHIND);
					AM_CHECK(BLOCK_UP);
					AM_CHECK(BLOCK_DOWN);
					AM_CHECK(GUARD_UP);
					AM_CHECK(GUARD_DOWN);
					AM_CHECK(RAGE_ATTACK) ;
					AM_CHECK(SPIN_ATTACK) ;
					AM_CHECK(RUN_PUNCH_LEFT) ;
					AM_CHECK(RUN_PUNCH_RIGHT);
					AM_CHECK(CHARGE);
					AM_CHECK(THROW);
					AM_CHECK(VICTORY);

					AM_CHECK(PUNCH_HIT);
					AM_CHECK(WEAPON_HIT);
					AM_CHECK(JUMP_HIT);

					AM_CHECK(GATE);

					AM_CHECK(FRONT_END1);
					AM_CHECK(FRONT_END2);
					AM_CHECK(FRONT_END3);
		
					amode->mFrameScaler = amode->mLast-amode->mStart;
					if(IsAnimLooped(amode->mMode))
					{
						amode->mFrameScaler++;	// makes looped animations morph last frame to first
					}

					if(amode->mFrameScaler)
							amode->mRate = G1/(amode->mFrameScaler*irate);
					else	amode->mRate = G0;
				}
			}
		}
		// finally, go through the parts and convert the position and matrix offsets into local space
		// assumes that part 0 is the top node
		mParts[0]->HierarchyAdjust(ZERO_FVECTOR, ID_FMATRIX);

		for (c0 = 0; c0 < mNumSkins; c0 ++)
			delete [] textures[c0];
	}
	else
	{
		mem.Close();
		return FALSE;
	}
	
	mem.Close();

//	mWrap = TRUE; // probably


	// Initialise bounding volume;
	// Simple for now - generate an axis aligned bounding box.  Should author these in MAX or something...
	if (mParts)
	{
		SINT	c0;

		for (c0 = 0; c0 < mNumParts; c0 ++)
		{
			if (mParts[c0])
				mParts[c0]->GenerateSimpleBoundingBox();
		}
	}

	// chain up the collision volumes
	if (mParts)
	{
		SINT				c0;
		CCollisionVolume	*cv, *lcv = NULL;

		for (c0 = 0; c0 < mNumParts; c0 ++)
		{
			if (mParts[c0])
			{
				cv = mParts[c0]->mCollisionVolume;

				if (lcv)
					lcv->mNextPart = cv;
				
				while (cv)
				{
					if (cv->mNext == NULL)
						lcv = cv;
					cv = cv->mNext;
				}
			}
		}
	}

	// radius
	//! this isn't very sensible!
	if (mParts && mParts[0])
		mRadius = mParts[0]->mRadius;


	return TRUE;
}

//******************************************************************************************
void	CMeshPart::EvaluateCollisionVolumeHierarchy(FVector pos, FMatrix ori, CThing *t)
{
	pos += ori * mOffsetPos;
	ori = ori * mOrientation;
	
	if (t)
	{
		GMatrix	m = t->GetRenderOrientation(mPartNumber);
		ori = ori * GMatrixToFMatrix(m);
	}


	CCollisionVolume *cv = mCollisionVolume;

	GMatrix	gori = FMatrixToGMatrix(ori);
		
	while (cv)
	{
		cv->mTransformedOrigin      =  (gori * cv->mOrigin) + FVectorToGVector(pos);
		cv->mTransformedOrientation = cv->mOrientation * gori;

		cv = cv->mNext;
	}

	SINT	c0;

	for (c0 = 0; c0 < mNumChildren; c0 ++)
		if (mChildren[c0])
			mChildren[c0]->EvaluateCollisionVolumeHierarchy(pos, ori, t);
}

//******************************************************************************************
void	CMeshPart::EvaluatePointVelocity(GVector pos, GVector &velocity, CThing *t)
{
	ASSERT(t);  // shouldn't be NULL here, really

	GMatrix m = t->GetAngularVelocity(mPartNumber);

	// get velocity due to rotation
	GVector r = (m * pos) - pos;

	// add linear velocity
	velocity += t->GetVelocity(mPartNumber) + r;

	// rotate into frame of reference
	GMatrix go = FMatrixToGMatrix(mOrientation);
	pos		 = go * pos;
	velocity = go * velocity;

	// and add linear offset  (to position only)
	pos += FVectorToGVector(mOffsetPos);


	if (mParent)
		mParent->EvaluatePointVelocity(pos, velocity, t);  // tail recursion??
}
	
//******************************************************************************************
void	CMesh::EvaluateCollisionVolume(CThing *t)
{
	if (mParts && mParts[0])
	{
		if (mLastFrameCollisionGenerated != GAME.GetGameTurn())
		{
			mParts[0]->EvaluateCollisionVolumeHierarchy(ZERO_FVECTOR, ID_FMATRIX, t);
			mLastFrameCollisionGenerated = GAME.GetGameTurn();
		}
	}
}

//******************************************************************************************
void	CMesh::GetEmitter(SINT no, FVector &pos, FMatrix &ori, CThing *t)
{
	if ((mEmitters) && (no >= 0) && (no < mNumEmitters))
	{
		// traverse the hierarchy to rotate emitter into position
		CMeshPart	*mp	= mEmitters[no].mMeshPart;
		pos				= mEmitters[no].mPos;
		ori				= ID_FMATRIX;  //mEmitters[no].mOrientation;
		FMatrix	lori	= ID_FMATRIX;

		while (mp)
		{
			lori  = mp->mOrientation;

			if (t)
			{
				GMatrix	m = t->GetRenderOrientation(mp->mPartNumber);
				lori = lori * GMatrixToFMatrix(m);
			}
			pos  = lori * pos;
			pos += mp->mOffsetPos;  // suspension??

			ori = lori * ori;
			
			if (t && (!t->RenderUseHierarchy(mp->mPartNumber)))
				break; // we're done here

			mp = mp->mParent;
		};
	}
	else
	{
		pos = ZERO_FVECTOR;
		ori = ID_FMATRIX;
	}
}

//******************************************************************************************
void	CMesh::GetEmitterAnimated(SINT no, FVector &pos, FMatrix &ori, CThing *t)
{
	if ((mEmitters) && (no >= 0) && (no < mNumEmitters))
	{
		CMeshPart	*mp = mEmitters[no].mMeshPart;

		// first, get the animated position of the vertex

		int		animind;
		if (t &&
			(mp->mNumVFrames >1) &&
			(mp->mBaseMesh->mNumAnimModes)&&
			((animind = t->GetRealAnimIndex())>= 0))
		{
			GINT	vframe = t->GetRenderFrame();
			CAnimMode*	am = &mp->mBaseMesh->mAnimModes[animind];
			vframe*=am->mFrameScaler; // this fraction varies for looped and unlooped animations
									// see loader code

			SINT	vf1 = (vframe.Whole()+am->mStart) % mp->mNumVFrames;		//### % shouldn't be needed
			SINT	vf2 = vf1 + 1;
			if(vf2 > am->mLast) vf2 = am->mStart;

			// now lookup into the vframes table
			float	af1 = mp->mVFrames[vf1];
			float	af2 = mp->mVFrames[vf2];

			if (af2 < af1) 
#if TARGET == PC
				{
					SINT t1;
					FASTFTOL(&t1,af1);
					af2 = float(t1+1);
				}
#else
				af2 = float(SINT(af1) + 1);
#endif
			
			GINT	aframe = (FToG(af2) * vframe.GFrac()) + (FToG(af1) * (G1 - vframe.GFrac()));

			SINT	f1 = aframe.Whole();
			SINT	f2 = f1 + 1;
#if TARGET == PC
			SINT	t1;
			FASTFTOL(&t1,mp->mVFrames[am->mLast]);
			if	(f2 > t1) 
				FASTFTOL(&f2, mp->mVFrames[am->mStart]);
#else
			if	(f2 > SINT(mp->mVFrames[am->mLast])) f2 = SINT(mp->mVFrames[am->mStart]);
#endif
			FVector *pverts1 = mp->mPVertices[f1];
			FVector *pverts2 = mp->mPVertices[f2];

			float	frac2 = GToF(aframe) - float(aframe.Whole());
			float	frac1 = (1.f - frac2);

			pos = (pverts1[mEmitters[no].mTrackingVertex] * frac1) +
			      (pverts2[mEmitters[no].mTrackingVertex] * frac2);

			FMatrix	lori	= ID_FMATRIX;
			// now follow down the hierarchy
			while (mp)
			{
				lori  = mp->mOrientation;

				GMatrix	m = t->GetRenderOrientation(mp->mPartNumber);
				lori = lori * GMatrixToFMatrix(m);

				pos  = lori * pos;
				pos += mp->mOffsetPos;

				if ((mp->mParent)) // not at top
				{
					pos += GVectorToFVector(t->GetRenderPos(mp->mPartNumber));
				}

				ori = lori * ori;
				
				if (!t->RenderUseHierarchy(mp->mPartNumber))
					break; // we're done here

				mp = mp->mParent;
			};
		}
		else
			GetEmitter(no, pos, ori, t);
	}
}

//******************************************************************************************
CMesh	*CMesh::GetMesh(char *name, char *def_tex, SINT num_skins)
{
	// name is 'const', can't modify directly. 
	// Easiest to make a working copy here.
	char localName[256];	
	strcpy(localName, name);

	// Little bit of a hack here - check to see if we need to rename this mesh
	// because it is arena specific.
	if ((strlen(localName) > 3) && (localName[2] == '_'))
	{
		switch (GAME.GetLevelData()->mIVars[IV_PRISON_NUMBER])
		{
		case 0:	localName[0] = 'E'; localName[1] = 'C'; break;
		case 1:	localName[0] = 'Z'; localName[1] = 'P'; break;
		case 2:	localName[0] = 'H'; localName[1] = 'O'; break;
		case 3:	localName[0] = 'S'; localName[1] = 'W'; break;
		case 4:	localName[0] = 'R'; localName[1] = 'E'; break;
		case 5:	localName[0] = 'M'; localName[1] = 'I'; break;
		case 6:	localName[0] = 'D'; localName[1] = 'R'; break;
		case 7:	localName[0] = 'S'; localName[1] = 'P'; break;
		case 8:	localName[0] = 'M'; localName[1] = 'L'; break;
		case 9:	localName[0] = 'A'; localName[1] = 'S'; break;
		case 10:localName[0] = 'L'; localName[1] = 'M'; break;
		};
	}


	// do we have this mesh already?
	CMesh	*m = mFirstMesh;

	while (m)
	{
		if (!(stricmp(localName, m->mName)))
			return m;

		m = m->mNextMesh;
	}

	// nope - try to load it.
	CMesh	*n = new CMesh;

	if (n)
	{
		// try load from disk
		if (n->InitFromFile(localName, def_tex, num_skins))
		{
			// chain into linked list
			n->mNextMesh = mFirstMesh;
			mFirstMesh = n;

			return n;
		}

		delete n;
	}

	// return the default mesh, because I've broken some of the earlier data formats.
	return ENGINE.GetDefaultMesh();
//	return FALSE;
}

//******************************************************************************************
GINT	CMesh::GetFrameIncrement(EAnimMode a, SINT* realindex)
{
	CAnimMode* am=mAnimModes;
	for(int i=0; i<mNumAnimModes; am++, i++)
	{
		if(am->mMode == a)
		{
			*realindex = i; // nasty
			return am->mRate;
		}
	}
	*realindex = -1;
	return G0;
}

//******************************************************************************************
BOOL	CMesh::InitAll()
{
	ASSERT(mFirstMesh == NULL);

	mDefaultTexture = CTextureGTEX::GetTexture("stone.raw");
	
	return TRUE;
}

//******************************************************************************************
void	CMesh::ReleaseAll()
{
	CMesh	*m = mFirstMesh;
	
	while (m)
	{
		CMesh	*t = m->mNextMesh;
		delete	m;
		m = t;
	};

	mFirstMesh = NULL;
}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CMesh::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	// add the mesh
	ma.AddMesh(this);

	// add all textures

	SINT	c0, c1;

	if (mParts)
	{
		for (c0 = 0; c0 < mNumParts; c0 ++)
		{
			if (mParts[c0])
			{
				for (c1 = 0; c1 < mParts[c0]->mNumDVertices; c1 ++)
				{
					SINT	c2;
					for (c2 = 0; c2 < mNumSkins; c2 ++)
						ta.AddTexture(mParts[c0]->mDVertices[c1].mTexture[c2], TATT_MESH);
				}
			}
		}
	}

}

//******************************************************************************************
void	CMesh::AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	// add the mesh
	ma.AddMesh(this);

	// add all textures

	SINT	c0, c1;

	if (mParts)
	{
		for (c0 = 0; c0 < mNumParts; c0 ++)
		{
			if (mParts[c0])
			{
				for (c1 = 0; c1 < mParts[c0]->mNumDVertices; c1 ++)
				{
					SINT	c2;
					for (c2 = 0; c2 < mNumSkins; c2 ++)
						ta.AddTexture(mParts[c0]->mDVertices[c1].mTexture[c2], TADCTT_MESH);
				}
			}
		}
	}

}
#endif

//******************************************************************************************
SINT	CMesh::GetCharacterNumber()
{
	// OK, now this is a _REAL_ hack..

	// we need to establish a number for each mesh, so that the PSX Version can throw away the mesh
	// at load time if it needs to.

	if (!(stricmp(mName, "angel.msh")))		return 0;
	if (!(stricmp(mName, "apostle.msh")))	return 1;
	if (!(stricmp(mName, "benny.msh")))		return 2;
	if (!(stricmp(mName, "lockdown.msh")))	return 3;
	if (!(stricmp(mName, "sophie.msh")))	return 4;
	if (!(stricmp(mName, "doctor.msh")))	return 5;

	return -1;
}

#endif
