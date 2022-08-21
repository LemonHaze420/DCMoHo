// $Header$

// $Log$
// Revision 1.19  2000-10-19 15:42:22+01  are
// Jeremy's optamisations.
//
// Revision 1.18  2000-10-18 13:51:12+01  asd
// Took out lighting for multiplayer
//
// Revision 1.17  2000-10-17 19:06:58+01  are
// Some optamisation.
//
// Revision 1.16  2000-10-17 14:12:49+01  are
// Lighting model complete.
//
// Revision 1.15  2000-10-17 12:29:09+01  are
// new shadow.
//
// Revision 1.14  2000-10-16 14:12:34+01  asd
// updated mesh lighting
//
// Revision 1.13  2000-10-16 10:18:49+01  are
// Bounding box.
//
// Revision 1.12  2000-10-15 15:06:38+01  asd
// updated lightA
//
// Revision 1.11  2000-10-13 17:36:28+01  asd
// updated lighting effects
//
// Revision 1.10  2000-10-13 15:08:34+01  asd
// MESH LIGHTING IS FIXED!!! YEY
//
// Revision 1.9  2000-10-13 14:42:10+01  asd
// removed some debug strings, mesh lighting still not working
//
// Revision 1.8  2000-10-12 17:36:15+01  asd
// added lighting for char select, mesh lighting still not working properly.....
//
// Revision 1.7  2000-10-05 15:08:04+01  asd
// added code to activate mesh lighting
//
// Revision 1.6  2000-09-12 17:19:23+01  sds
// Big Merge II - all of jeremy's link editor stuff and andrews TARGET == DC stuff
//
// Revision 1.5  2000-09-12 14:09:21+01  are
// #if TARGET == DC'd out some code so that it compiles on PC.
//
// Revision 1.4  2000-09-12 10:12:52+01  sds
// Big Merge - Should work for DC and PC
//
// Revision 1.3  2000-08-07 15:47:08+01  jjs
// DC and PC merged code.
//
// Revision 1.2  2000-08-05 10:33:36+01  jjs
// First PC milestone.
//

#include	"Common.h"

#if TARGET == PC

#include	"RTMesh.h"
#include	"Globals.h"


BOOL	light_from_map;

//******************************************************************************************
void	CRTMesh::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	// get skinning info
	mSkin = i->mSkin;

	if (i->mName[0])
		mMesh = CMesh::GetMesh(i->mName, i->mName2, i->mNumSkins);
	else
	{
		if (mThing)
		{
			SINT mn = mThing->GetRenderMesh();

			if ((mn != -1) && (mn != -2))
				mMesh = ENGINE.GetGlobalMesh(mn);
			else
				mMesh = ENGINE.GetDefaultMesh();
		}
		else
			mMesh = ENGINE.GetDefaultMesh();

	}

	ASSERT(mMesh);
	if (!mMesh)
		mMesh = ENGINE.GetDefaultMesh();

	if (i->mName4[0] != 0)
	{
		mMesh2 = CMesh::GetMesh(i->mName4, i->mName2);
		ASSERT(mMesh2);
	}
	else
		mMesh2 = NULL;

	mShadow = i->mShadow;

	if (i->mShadow == ST_SPRITE)
	{
		mTexShadow = CTextureGTEX::GetTexture(i->mName3);
		ASSERT(mTexShadow);
	}
	else
		mTexShadow = NULL;

	mWaveResponse = i->mWaveResponse;

	// fill out radius
	mRadius = mMesh->Radius();

	if (mMesh2)
	{
		GINT r = mMesh2->Radius();
		if (r > mRadius)
			mRadius = r;
	}
}

//******************************************************************************************
inline SINT	lightA(float c)
{
	if (c < 0.0f)
	{
		return 0.0f;
	}
	
	if (c > 1.0f)
	{
		return 255.0f;
	}

	return (c * 255.0f);
}

//********************************************************************************
DWORD	light(DWORD &d, FVector &n, FMatrix &m)
{
	FVector	norm = m * n; //rotate normal into world space;

	// erm.. just use x for now...

//	float	val = (norm.X * 0.4f) + 0.6f; // 0.2 - 1.0
	
	float	val = (norm.X * 0.5f) + 0.5f; // 0.0 - 1.0

	UBYTE	mul = UBYTE(val * 255);

	UBYTE r = UBYTE((((d & 0x00ff0000) >> 16 ) * mul) >> 8 );
	UBYTE g = UBYTE((((d & 0x0000ff00) >> 8  ) * mul) >> 8 );
	UBYTE b = UBYTE((((d & 0x000000ff) >> 0  ) * mul) >> 8 );

	DWORD l = (d & 0xff000000) + (r << 16) + (g << 8) + b;

	return l;
}


//******************************************************************************************
float	get_wibble_scale(FVector p)
{
	// wibble more nearer surface.
	// get worldspace difference
#define WIBBLE_DAMP  0.2f

	GVector g = FVectorToGVector(p);

	if (!(COORDS_INVALID(CCellRef(g))))
	{
		float ws = p.Z - GToF(MAP.GetMapHeightOriginal(g));
		float wibble = (1.f / (1.f + ((ws < - 4.f) ? (-ws + 4.f) : 0) * WIBBLE_DAMP));

		return wibble;
	}
	else
		return 0;
}



//******************************************************************************************
void	CRTMesh::Render()
{


	FVector p;
//	GetRenderPosition(&p);
	p = ZERO_FVECTOR;

	FMatrix ori;
	GetRenderOrientation(&ori);
//	ori = ID_FMATRIX;

	FVector pos;
	GetRenderPosition(&pos);

	RenderMesh(p, ID_FMATRIX, mMesh);

	if(mShadow)
	{
		GetRenderPosition(&p);
		RenderShadow(p);
	}
	if (mMesh2)
	{
//		GetRenderPosition(&p);
		p = ZERO_FVECTOR;
		GetRenderOrientation(&ori, 1);

		RenderMesh(p, ori, mMesh2);
	}	
}

//SINT maxvertcount = 0;

//******************************************************************************************
void	CRTMesh::RenderMeshPart(FVector p, FMatrix &ori, CMeshPart *mesh_part, CThing *t, EWaveResponse wr, SINT skin)
{
//	p += mesh_part->mOffsetPos;

	// Count those verticies!
	/*
	// 390 highest number counted
	maxvertcount = max(maxvertcount, mesh_part->mNumDVertices);
	char buffer[256];
	sprintf(buffer, "Max verts processed: %d\n", maxvertcount);
	OutputDebugString(buffer);
	*/

	DWORD	rgb[512];

	if (wr == WR_MOVE)
		p += FVector(0, 0, GToF(MAP.GetMapHeightOffset(FVectorToGVector(p))) * get_wibble_scale(p));

	//*********************************************************************
	//*********************************************************************
	//*********************************************************************
	//** JCL moved clipping here...

	int IX1 = 0;
	int IY1	= 0;
	int IX2 = PLATFORM.ViewWidth();	// JCL
	int	IY2 = PLATFORM.ScreenHeight();

	if (GAME.IsMultiPlayer() && (ENGINE.RenderingView() == 1))
	{
		IX1 += PLATFORM.ViewWidth();
		IX2 += PLATFORM.ViewWidth();
	}


	if (!GAME.mInFrontEnd)
	{
		//FVector		verts[8];
		CFVF_GTEX	verts[8];
		FVector x 		= ori * FVector(GToF(mesh_part->mCollisionVolume->mAxes.X), 0, 0);
		FVector y 		= ori * FVector(0, GToF(mesh_part->mCollisionVolume->mAxes.Y), 0);
		FVector z 		= ori * FVector(0, 0, GToF(mesh_part->mCollisionVolume->mAxes.Z));
		FVector origin 	= ori * GVectorToFVector(mesh_part->mCollisionVolume->mOrigin);
		
		// too far away?
		if ((p + origin - GVectorToFVector(ENGINE.GetCamera()->mPos)).MagnitudeSq() >
		      (GAME.IsMultiPlayer() ? 5000.f : 16384.f))
			return;

		verts[0].mPos = -x - y - z + p + origin;
		verts[1].mPos =  x - y - z + p + origin;
		verts[2].mPos = -x + y - z + p + origin;
		verts[3].mPos =  x + y - z + p + origin;
		verts[4].mPos = -x - y + z + p + origin;
		verts[5].mPos =  x - y + z + p + origin;
		verts[6].mPos = -x + y + z + p + origin;
		verts[7].mPos =  x + y + z + p + origin;
		
//		ENGINE.SwapYZ((FVector *)verts, 8);
//		{
//	    PROFILE_FN(RMMeshDrawTransWrapper1);
//		PersTransWrapper(8,(tvector *)verts, (tvector *)verts, pos + origin, ID_FMATRIX);
//		}

		//CFVF_GTEX *vs = (CFVF_GTEX*)mVerts;
		CFVF_GTEX *lpun;
		D3DTLVERTEX *lptr;

		static LPDIRECT3DVERTEXBUFFER7 rtuntrans=NULL;
		static LPDIRECT3DVERTEXBUFFER7 rttrans=NULL;

		// Only executed once, hopefully!
		if(!rtuntrans)
		{
			LPDIRECT3D7 lpD3D;
			D3DVERTEXBUFFERDESC vbdesc;
			ZeroMemory(&vbdesc,sizeof(D3DVERTEXBUFFERDESC));
			vbdesc.dwSize = sizeof(D3DVERTEXBUFFERDESC);
			vbdesc.dwFVF = FVF_GTEX_FLAGS;
			vbdesc.dwNumVertices = 8;
			vbdesc.dwCaps = D3DVBCAPS_SYSTEMMEMORY;
			ENGINE.GetDevice()->GetDirect3D(&lpD3D);
			lpD3D->CreateVertexBuffer(&vbdesc,&rtuntrans,0L);
			vbdesc.dwFVF = D3DFVF_TLVERTEX;
			lpD3D->CreateVertexBuffer(&vbdesc,&rttrans,0L);
		}

		// Copy all vertex data.
		rtuntrans->Lock(DDLOCK_WRITEONLY,(LPVOID *)&lpun,NULL);
		memcpy(lpun, verts, 8 * sizeof(CFVF_GTEX));
		rtuntrans->Unlock();

		ENGINE.SetupTransforms(	ZERO_FVECTOR, ID_FMATRIX);

		if (SUCCEEDED(rttrans->ProcessVertices(	D3DVOP_TRANSFORM,
												0,					// Destination start index
												8,					// Number
												rtuntrans,			// Untransformed vertices
												0,					// Source start index
												ENGINE.GetDevice(),	// Direct3D
												0					// Flags
											)
						)
			)
		{
			rttrans->Lock(DDLOCK_READONLY, (LPVOID*)&lptr, NULL);

			SINT 	foo;
			BOOL	ok;

			/*
			for (foo = 0; foo < 8; foo++)
			{
				char where[256];
				sprintf(where, 
						"%d %f %f %f\n", 
						foo,
						lptr[foo].sx,
						lptr[foo].sy,
						lptr[foo].sz
						);
				OutputDebugString(where);
			}
			*/
			
			BOOL	zvalid[8];
			
			// check each edge
			ok = FALSE;		
			for (foo = 0; foo < 8; foo ++)
			{
				if (lptr[foo].sz > 0) 
				{
					ok = TRUE;
					zvalid[foo] = TRUE;
				}
				else
					zvalid[foo] = FALSE;
			}
			if (!ok)
			{
				rttrans->Unlock();
				return;
			}
			
			ok = FALSE;		
			for (foo = 0; foo < 8; foo ++)
			{
				if ((lptr[foo].sy < IY2 + 60) && zvalid[foo])
					ok = TRUE;
			}
			if (!ok)
			{
				rttrans->Unlock();
				return;
			}

			ok = FALSE;		
			for (foo = 0; foo < 8; foo ++)
			{
				if ((lptr[foo].sx >= IX1) && zvalid[foo])
					ok = TRUE;
			}
			if (!ok)
			{
				rttrans->Unlock();
				return;
			}

			ok = FALSE;		
			for (foo = 0; foo < 8; foo ++)
			{
				if ((lptr[foo].sy >= IY1) && zvalid[foo])
					ok = TRUE;
			}
			if (!ok)
			{
				rttrans->Unlock();
				return;
			}
			
			ok = FALSE;		
			for (foo = 0; foo < 8; foo ++)
			{
				if ((lptr[foo].sx < IX2) && zvalid[foo])
					ok = TRUE;
			}
			if (!ok) 
			{
				rttrans->Unlock();
				return;
			}

			rttrans->Unlock();
		}
		else
		{
			OutputDebugString("Failed to transform.\n");
			return;
		}
	}
	//*********************************************************************





	FMatrix iro = ori.Transpose();
	
	GINT	scale;
	if(t && ((scale = t->GetRenderScale()) != G1))
	{
		float	fsc = GToF(scale);
		float	s_z = 2.f - fsc;
		float	s_xy = fsc;
		
		ori.Row[0].X *= s_xy;
		ori.Row[0].Y *= s_xy;
		ori.Row[0].Z *= s_xy;
		ori.Row[1].X *= s_xy;
		ori.Row[1].Y *= s_xy;
		ori.Row[1].Z *= s_xy;
		ori.Row[2].X *= s_z;
		ori.Row[2].Y *= s_z;
		ori.Row[2].Z *= s_z;
	}
	
	ENGINE.SetupTransforms(p, ori);
	SINT		c0;	

	//***********************
	// lighting!

//	if(!GAME.IsMultiPlayer())//don't light players if game is multiplayer.. speed up.
	{
		if ((t) && ((t->GetRenderThingMiscFlags() & RTMF_NOLIGHT) || (t->GetRenderThingMiscFlags() & RTMF_WHITE)))
		{
			//for (c0 = 0; (c0 < mesh_part->mNumDVertices); c0 ++)
			//	rgb[c0] = 0xffffffff;
			memset((void*)rgb, 0xFF, mesh_part->mNumDVertices * sizeof(DWORD));
		}
		else
		{
			FVector lv1 = FVector( 1.f, -0.1f, 0);
			FVector lv2 = FVector(-1.f, -0.1f, 0);
			FVector	lv3 = FVector(0, 1.f, 0);

			float lr, lg, lb;
			SINT  sr, sg, sb;

				if (t)
				{
					GVector thePos = t->GetRenderPos();
					
					GINT gX = thePos.X;
					GINT gY = thePos.Y;

					//CMapCell theMapCell;
						
					//DWORD col = (MAP.GetCell(gX, gY)).mColour;

					DWORD col = MAP.GetCell(CCellRef(t->GetRenderPos()))->mColour;
					
					//DWORD col = theMapCell.GetColour();
					
					sr = ((col >> 16) & 0xff);
					
					lr = (sr/512.0f);
					
					sg = ((col >>  8) & 0xff);
					
					lg = (sg/512.0f);
					
					sb = ((col      ) & 0xff);
					
					lb = (sb/512.0f);

				
				}
				else
				{
					lr = lg = lb = 0.5f;
				}

			FVector	lc1, lc2, lc3;
			
			lc1 = FVector( (float)(lr * 4.0f) , (float)(lg * 2.0f), (float)lb);
			lc2 = FVector((float)lr, (float)(lg * 2.0f), (float)(lb * 4.0f));
			lc3 = FVector((float)2.0f, (float)2.0f, (float)2.0f);
						
			lv1 = ((iro * lv1));
			lv2 = ((iro * lv2));
			lv3 = ((iro * lv3));
			
			
			
			for (c0 = 0; c0 < mesh_part->mNumDVertices; c0 ++)
			{
				SINT	r, g, b;
		
				float v1 = ((float)((mesh_part->mDVertices[c0].mNormal) * lv1));
				float v2 = ((float)((mesh_part->mDVertices[c0].mNormal) * lv2));
				float v3 = ((float)((mesh_part->mDVertices[c0].mNormal) * lv3));
				
				r  = lightA((float)(v1 * lc1.X));
				g  = lightA((float)(v1 * lc1.Y));
				b  = lightA((float)(v1 * lc1.Z));
				r += lightA((float)(v2 * lc2.X));
				g += lightA((float)(v2 * lc2.Y));
				b += lightA((float)(v2 * lc2.Z));
				r += lightA((float)(v3 * lc3.X));
				g += lightA((float)(v3 * lc3.Y));
				b += lightA((float)(v3 * lc3.Z));
				
				r += 40;
				g += 40;
				b += 40;
				
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;

				// Can never be less that 0! (lightA returns in bound 0-255)
				//if (r < 0) r = 0;
				//if (g < 0) g = 0;
				//if (b < 0) b = 0;	
			
				
				(DWORD)rgb[c0] = ((r << 16) + (g << 8) + (b));
			}
		
		}
//		mesh_part->mDVertices[c0].mDiffuse = rgb[c0];
	}
	//********************************
	int		animind;
	if (t &&
		(mesh_part->mNumVFrames >1) &&
		(mesh_part->mBaseMesh->mNumAnimModes)&&
		((animind = t->GetRealAnimIndex())>= 0))
	{
		GINT	vframe = t->GetRenderFrame();
		CAnimMode*	am = &mesh_part->mBaseMesh->mAnimModes[animind];
		vframe*=am->mFrameScaler; // this fraction varies for looped and unlooped animations
								// see loader code

		SINT	vf1 = (vframe.Whole()+am->mStart) % mesh_part->mNumVFrames;		//### % shouldn't be needed
		SINT	vf2 = vf1 + 1;
		if(vf2 > am->mLast) vf2 = am->mStart;

		// now lookup into the vframes table
		float	af1 = mesh_part->mVFrames[vf1];
		float	af2 = mesh_part->mVFrames[vf2];

		if (af2 < af1) af2 = float(SINT(af1) + 1);

		GINT	aframe = (FToG(af2) * vframe.GFrac()) + (FToG(af1) * (G1 - vframe.GFrac()));

		SINT	f1 = aframe.Whole();
		SINT	f2 = f1 + 1;
		if	(f2 > SINT(mesh_part->mVFrames[am->mLast])) f2 = SINT(mesh_part->mVFrames[am->mStart]);

		FVector *pverts1 = mesh_part->mPVertices[f1];
		FVector *pverts2 = mesh_part->mPVertices[f2];

		float	frac2 = GToF(aframe) - float(aframe.Whole());
		float	frac1 = (1.f - frac2);

		CFVF_GTEX	vert;

		for (c0 = 0; c0 < mesh_part->mNumDVertices; c0++)
		{
			CMeshDVertex *v	= &mesh_part->mDVertices[c0];

			if (wr == WR_WARP)
			{
				FVector wp		= (pverts1[v->mPVert] * frac1) + (pverts2[v->mPVert] * frac2);

				//! ARRRRGh!
				vert.mPos		= wp + ((iro * FVector(0, 0, GToF(MAP.GetMapHeightOffset(FVectorToGVector((ori * wp ) + p))))) * get_wibble_scale((ori * wp ) + p));
			}
			else
				vert.mPos		= (pverts1[v->mPVert] * frac1) + (pverts2[v->mPVert] * frac2);

			vert.mU			= v->mU;
			vert.mV			= v->mV;
//			if(!GAME.IsMultiPlayer())
			{
				vert.mDiffuse	= rgb[c0];
			}
//			else
//			{
//				vert.mDiffuse	= 0xFF999999;
//			}

			v->mIndex = v->mTexture[skin]->AddVertex(&vert);
		}
	}
	else
	{
		FVector *pverts1 = mesh_part->mPVertices[0];

		CFVF_GTEX	vert;

		for (c0 = 0; c0 < mesh_part->mNumDVertices; c0++)
		{
			CMeshDVertex *v	= &mesh_part->mDVertices[c0];

			if (wr == WR_WARP)
			{
				FVector wp		= pverts1[v->mPVert];

				//! ARRRRGh!
				vert.mPos		= wp + ((iro * FVector(0, 0, GToF(MAP.GetMapHeightOffset(FVectorToGVector((ori * wp ) + p))))) * get_wibble_scale((ori * wp ) + p));
			}
			else
				vert.mPos		= pverts1[v->mPVert];

			vert.mU			= v->mU;
			vert.mV			= v->mV;
//			if(!GAME.IsMultiPlayer())
			{
				vert.mDiffuse	= rgb[c0];
			}
//			else
//			{
//				vert.mDiffuse	= 0xFF999999;
//			}

			v->mIndex = v->mTexture[skin]->AddVertex(&vert);
		}
	}

	for (c0 = 0; c0 < mesh_part->mNumTriangles; c0++)
	{
		CMeshTriangle *t = &mesh_part->mTriangles[c0];

		ASSERT(t->mA->mTexture[skin]);
		ASSERT(t->mA->mTexture[skin] == t->mB->mTexture[skin]);
		ASSERT(t->mA->mTexture[skin] == t->mC->mTexture[skin]);
		t->mA->mTexture[skin]->AddTri(t->mA->mIndex, t->mB->mIndex, t->mC->mIndex);
	}
	CTexture::DrawAllTextures(); // AAARSE...
	
	if (t && (GAME.GetGameRenderInfo()->mActive & GRI_COLLISION_INFO))
		ENGINE.DrawCollisionVolume(mesh_part->mCollisionVolume, t->GetRenderHitFlags());
}

//******************************************************************************************
void	CRTMesh::RenderMeshHierarchy(FVector p, FMatrix ori, CMeshPart *mp)
{
	if (!mp)
		return;

	// evaluate transforms
	if ((!mThing) || (mThing->RenderUseHierarchy(mp->mPartNumber)))
	{
		p  += ori * mp->mOffsetPos;
		ori = ori * mp->mOrientation;
	}
	else
	{
		p += mp->mOffsetPos;
		ori = mp->mOrientation;
	}

	if (mThing)
	{
		FVector po;
		GetRenderPosition(&po, mp->mPartNumber);
		p += po;

		FMatrix	m;
		GetRenderOrientation(&m, mp->mPartNumber);
		ori = ori * m;
	}

	// render this part
	if ((!mThing) || (mThing->RenderRenderPart(mp->mPartNumber)))
		RenderMeshPart(p, ori, mp, mThing, mWaveResponse, mSkin);

	// and the children
	SINT	c0;

	for (c0 = 0; c0 < mp->mNumChildren; c0 ++)
	{
		RenderMeshHierarchy(p, ori, mp->mChildren[c0]);
	}
}

//******************************************************************************************
void	CRTMesh::RenderMesh(FVector &p, FMatrix &ori, CMesh *mesh)
{
	if (mesh && mesh->mParts)
	{
		ASSERT(mesh->mParts[0]->mParent == NULL); // first part should be the parent
		light_from_map = TRUE;	
		RenderMeshHierarchy(p, ori, mesh->mParts[0]);
	}
	light_from_map = FALSE;
}

//******************************************************************************************
void	CRTMesh::RenderFEMeshPart(FVector p, FMatrix &ori, CMeshPart *mesh_part, IRenderableThing *t, EWaveResponse wr, SINT skin)
{
//	p += mesh_part->mOffsetPos;
	DWORD rgb, col = 0xFFFFFFFF;
	SINT  sr, sg, sb, r, g, b;
	FVector	lc1, lc2, lc3, lc4;
	float	v1, v2, v3, v4, lr, lg, lb;

	if (wr == WR_MOVE)
		p += FVector(0, 0, GToF(MAP.GetMapHeightOffset(FVectorToGVector(p))) * get_wibble_scale(p));
	
	FMatrix iro = ori.Transpose();
	
	FVector lv1 = iro * FVector( 1.f, -0.1f, 0);
	FVector lv2 = iro * FVector(-1.f, -0.1f, 0);
	FVector	lv3 = iro * FVector(0, 1.f, 0);
	
	sr = ((col >> 16) & 0xff);
			
	lr = (sr/512.0f);
	
	sg = ((col >>  8) & 0xff);
	
	lg = (sg/512.0f);
	
	sb = ((col      ) & 0xff);
	
	lb = (sb/512.0f);

	lc1 = FVector( (float)(lr * 4.0f) , (float)(lg * 2.0f), (float)lb);
	lc2 = FVector((float)lr, (float)(lg * 2.0f), (float)(lb * 4.0f));
	lc3 = FVector((float)2.0f, (float)2.0f, (float)2.0f);
	
	ENGINE.SetupTransforms(p, ori);
	SINT		c0;	

	int		animind;
	if (t &&
		(mesh_part->mNumVFrames >1) &&
		(mesh_part->mBaseMesh->mNumAnimModes)&&
		((animind = t->GetRealAnimIndex())>= 0))
	{
		GINT	vframe = t->GetRenderFrame();
		CAnimMode*	am = &mesh_part->mBaseMesh->mAnimModes[animind];
		vframe*=am->mFrameScaler; // this fraction varies for looped and unlooped animations
								// see loader code

		SINT	vf1 = (vframe.Whole()+am->mStart) % mesh_part->mNumVFrames;		//### % shouldn't be needed
		SINT	vf2 = vf1 + 1;
		if(vf2 > am->mLast) vf2 = am->mStart;

		// now lookup into the vframes table
		float	af1 = mesh_part->mVFrames[vf1];
		float	af2 = mesh_part->mVFrames[vf2];

		if (af2 < af1) af2 = float(SINT(af1) + 1);

		GINT	aframe = (FToG(af2) * vframe.GFrac()) + (FToG(af1) * (G1 - vframe.GFrac()));

		SINT	f1 = aframe.Whole();
		SINT	f2 = f1 + 1;
		if	(f2 > SINT(mesh_part->mVFrames[am->mLast])) f2 = SINT(mesh_part->mVFrames[am->mStart]);

		FVector *pverts1 = mesh_part->mPVertices[f1];
		FVector *pverts2 = mesh_part->mPVertices[f2];

		float	frac2 = GToF(aframe) - float(aframe.Whole());
		float	frac1 = (1.f - frac2);

		CFVF_GTEX	vert;

		for (c0 = 0; c0 < mesh_part->mNumDVertices; c0++)
		{
			CMeshDVertex *v	= &mesh_part->mDVertices[c0];

			if (wr == WR_WARP)
			{
				FVector wp		= (pverts1[v->mPVert] * frac1) + (pverts2[v->mPVert] * frac2);

				//! ARRRRGh!
				vert.mPos		= wp + ((iro * FVector(0, 0, GToF(MAP.GetMapHeightOffset(FVectorToGVector((ori * wp ) + p))))) * get_wibble_scale((ori * wp ) + p));
			}
			else
				vert.mPos		= (pverts1[v->mPVert] * frac1) + (pverts2[v->mPVert] * frac2);

			vert.mU			= v->mU;
			vert.mV			= v->mV;
			
				
			v1 = ((mesh_part->mDVertices[c0].mNormal) * lv1);
			v2 = ((mesh_part->mDVertices[c0].mNormal) * lv2);
			v3 = ((mesh_part->mDVertices[c0].mNormal) * lv3);
			
			r  = lightA((v1 * lc1.X)) + lightA((v2 * lc2.X)) + lightA((v3 * lc3.X)) + 40;
			g  = lightA((v1 * lc1.Y)) + lightA((v2 * lc2.Y)) + lightA((v3 * lc3.Y)) + 40;
			b  = lightA((v1 * lc1.Z)) + lightA((v2 * lc2.Z)) + lightA((v3 * lc3.Z)) + 40;
			
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			rgb = ((r << 16) + (g << 8) + (b));
				
			vert.mDiffuse	= rgb;//light(v->mDiffuse, v->mNormal, ori);

			v->mIndex = v->mTexture[skin]->AddVertex(&vert);
		}
	}
	else
	{
		FVector *pverts1 = mesh_part->mPVertices[0];

		CFVF_GTEX	vert;

		for (c0 = 0; c0 < mesh_part->mNumDVertices; c0++)
		{
			CMeshDVertex *v	= &mesh_part->mDVertices[c0];

			if (wr == WR_WARP)
			{
				FVector wp		= pverts1[v->mPVert];

				//! ARRRRGh!
				vert.mPos		= wp + ((iro * FVector(0, 0, GToF(MAP.GetMapHeightOffset(FVectorToGVector((ori * wp ) + p))))) * get_wibble_scale((ori * wp ) + p));
			}
			else
				vert.mPos		= pverts1[v->mPVert];

			vert.mU			= v->mU;
			vert.mV			= v->mV;

			v1 = ((mesh_part->mDVertices[c0].mNormal) * lv1);
			v2 = ((mesh_part->mDVertices[c0].mNormal) * lv2);
			v3 = ((mesh_part->mDVertices[c0].mNormal) * lv3);
			
			r  = lightA((v1 * lc1.X)) + lightA((v2 * lc2.X)) + lightA((v3 * lc3.X)) + 40;
			g  = lightA((v1 * lc1.Y)) + lightA((v2 * lc2.Y)) + lightA((v3 * lc3.Y)) + 40;
			b  = lightA((v1 * lc1.Z)) + lightA((v2 * lc2.Z)) + lightA((v3 * lc3.Z)) + 40;
			
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			rgb = ((r << 16) + (g << 8) + (b));

			vert.mDiffuse	= rgb; //light(v->mDiffuse, v->mNormal, ori);

			v->mIndex = v->mTexture[skin]->AddVertex(&vert);
		}
	}

	for (c0 = 0; c0 < mesh_part->mNumTriangles; c0++)
	{
		CMeshTriangle *t = &mesh_part->mTriangles[c0];

		ASSERT(t->mA->mTexture[skin]);
		ASSERT(t->mA->mTexture[skin] == t->mB->mTexture[skin]);
		ASSERT(t->mA->mTexture[skin] == t->mC->mTexture[skin]);
		t->mA->mTexture[skin]->AddTri(t->mA->mIndex, t->mB->mIndex, t->mC->mIndex);
	}
	CTexture::DrawAllTextures(); // AAARSE...
	
}

//******************************************************************************************
void	CRTMesh::RenderFEMeshHierarchy(FVector p, FMatrix ori, CMeshPart *mp, IRenderableThing *t)
{
	if (!mp)
		return;

	// evaluate transforms
	if ((!t) || (t->RenderUseHierarchy(mp->mPartNumber)))
	{
		p  += ori * mp->mOffsetPos;
		ori = ori * mp->mOrientation;
	}
	else
	{
		p += mp->mOffsetPos;
		ori = mp->mOrientation;
	}

	// render this part
	if ((!t) || (t->RenderRenderPart(mp->mPartNumber)))
		RenderFEMeshPart(p, ori, mp, t, mWaveResponse, mSkin);

	// and the children
	SINT	c0;

	for (c0 = 0; c0 < mp->mNumChildren; c0 ++)
	{
		RenderFEMeshHierarchy(p, ori, mp->mChildren[c0],t);
	}
}
//******************************************************************************************
void	CRTMesh::RenderFEMesh(FVector &p, FMatrix &ori,IRenderableThing *t)
{
	if (mMesh && mMesh->mParts)
	{
		ASSERT(mMesh->mParts[0]->mParent == NULL); // first part should be the parent
		RenderFEMeshHierarchy(p, ori, mMesh->mParts[0],t);
	}
}

//******************************************************************************************
void	CRTMesh::RenderShadow(FVector &p)
{
	if ((!PLATFORM.IsKeyDown(VK_F11)))
	{
		if (mShadow == ST_SPRITE)
		{
			if (mTexShadow) 		// render shadow as texture?
			{
				MAP.DrawShadow(mThing, mTexShadow);

			}
		}


	}
}

//******************************************************************************************
void	CRTMesh::RenderCollisionVolume(SINT i)
{
	FVector p;
	FMatrix m;

	GetRenderPosition(&p);
	GetRenderOrientation(&m);

	ENGINE.SetupTransforms(p, m);

	if (mMesh)
		ENGINE.DrawCollisionVolume(GetCollisionVolume(), i);
}

//******************************************************************************************
GVector	CRTMesh::GetRTEmitter(SINT no)
{
	FVector v = ZERO_FVECTOR;
	FMatrix m = ID_FMATRIX;

	if (mMesh)
	{
		if ((no < mMesh->mNumEmitters) && (no >= 0))
		{
			mMesh->GetEmitter(no, v, m, mThing);
		}
	}

	return FVectorToGVector(v);
}

//******************************************************************************************
GVector	CRTMesh::GetRTEmitterAnimated(SINT no)
{
	FVector v = ZERO_FVECTOR;
	FMatrix m = ID_FMATRIX;

	if (mMesh)
	{
		if ((no < mMesh->mNumEmitters) && (no >= 0))
		{
			mMesh->GetEmitterAnimated(no, v, m, mThing);
		}
	}

	return FVectorToGVector(v);
}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CRTMesh::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	SUPERTYPE::AccumulateResources(ta, ma);

	if (mMesh)
		mMesh->AccumulateResources(ta, ma);

	if (mMesh2)
		mMesh2->AccumulateResources(ta, ma);

	if (mTexShadow)
		ta.AddTexture(mTexShadow, TATT_OTHER, 1,  -1, -1, BD_4, 2, TRUE);
}

//******************************************************************************************
void	CRTMesh::FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt)
{
	// is this a named mesh?
	if (pcirt.mName[0] == 0)
		ri.i0 = -1;
	else
		ri.i0 = ma.GetMeshIndex(mMesh);

	ri.i1 = ma.GetMeshIndex(mMesh2);
	
	if (mTexShadow)
		ri.i2 = ta.GetTextureIndex(TATT_OTHER, mTexShadow);
	else
		ri.i2 = -1;

	ri.mShadow = mShadow;
	ri.mWaveResponse = mWaveResponse;

	ri.i3 = mSkin;
}

//******************************************************************************************
void	CRTMesh::AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	SUPERTYPE::AccumulateResourcesDC(ta, ma);

	if (mMesh)
		mMesh->AccumulateResourcesDC(ta, ma);

	if (mMesh2)
		mMesh2->AccumulateResourcesDC(ta, ma);

	if (mTexShadow)
		ta.AddTexture(mTexShadow, TADCTT_OTHER, 1,  -1, -1, 2, TRUE);
}

//******************************************************************************************
void	CRTMesh::FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt)
{
	// is this a named mesh?
	if (pcirt.mName[0] == 0)
		ri.i0 = -1;
	else
		ri.i0 = ma.GetMeshIndex(mMesh);

	ri.i1 = ma.GetMeshIndex(mMesh2);
	
	if (mTexShadow)
		ri.i2 = ta.GetTextureIndex(TADCTT_OTHER, mTexShadow);
	else
		ri.i2 = -1;

	ri.mShadow = mShadow;
	ri.mWaveResponse = mWaveResponse;

	ri.i3 = mSkin;
}

#endif

//******************************************************************************************
GINT	CRTMesh::GetFrameIncrement(EAnimMode a, SINT* realindex) 
{
	if(mMesh)
		return mMesh->GetFrameIncrement(a, realindex);
	else
		return G0;
}

//******************************************************************************************

#endif

