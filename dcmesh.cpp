#define _KM_USE_VERTEX_MACRO_L5_ //important
#include <kamuix.h>
#include	"Common.h"

#if TARGET == DC
#include <string.h>
#include	"Globals.h"
#include	"DCMesh.h"
#include    "DC3dengine.h"
#include    <gtemac.h>
#include	<mw_util.h>
#include	"maincode.h"
#include	"dcjunk.h"
#include 	"profile.h"
#define kmyMemCopy memcpy
// Bad practise but just for now
tvector in[10000],out[10000];

BOOL	light_from_map; /// JCL - AAAAAAAAarrrrrrrrrghh, codewarrior is shit.

//******************************************************************************************
//******************************************************************************************
//** Render thing given this render info

SINT	light(float c)
{
	SINT	v = SINT(c * 255.f);
	
	if (v < 0)
		v = 0;
	else if (v > 255)
		v = 255;
	
	return v;
}

//*******************************************************************************
inline	BOOL	is_on_screen(FVector &v)
{
	if ((v.X < 0) ||
		(v.X > PLATFORM.ScreenWidth()) ||
		(v.Y < 0) ||
		(v.Y > PLATFORM.ScreenHeight()) ||
		(v.Z > 0))
		return FALSE;
		
	return TRUE;
}		

//*******************************************************************************

void    SDCMeshPart::Render(IRenderableThing *t, SRTInfo *ri, const FVector &pos, const FMatrix &ori)
{
	PROFILE_FN(RMMeshTotal);

	DWORD	rgb[1000];
	SINT 	skin = 0;	
	// JCL - quick bounding cull
	{
    PROFILE_FN(RMMeshDrawStartUp);	
	
	if (!GAME.mInFrontEnd)
	{
/*		FMatrix cori  = GMatrixToFMatrix(ENGINE.GetCamera()->mOrientation);
		FVector x = cori * FVector(GToF(mRadius), 0, 0);
		FVector y = cori * FVector(0, GToF(mRadius), 0);
	
		FVector	verts[4];
		
		verts[0] = -x - y;
		verts[1] =  x - y;
		verts[2] = -x + y;
		verts[3] =  x + y;*/
		
		FVector	verts[8];
		FVector x 		= ori * FVector(GToF(mCollisionVolume->mAxes.X), 0, 0);
		FVector y 		= ori * FVector(0, GToF(mCollisionVolume->mAxes.Y), 0);
		FVector z 		= ori * FVector(0, 0, GToF(mCollisionVolume->mAxes.Z));
		FVector origin 	= ori * GVectorToFVector(mCollisionVolume->mOrigin);
		
		// too far away?
		if ((pos + origin - GVectorToFVector(ENGINE.GetCamera()->mPos)).MagnitudeSq() >
		      (GAME.IsMultiPlayer() ? 5000.f : 16384.f))
			return;
		
		verts[0] = -x - y - z;
		verts[1] =  x - y - z;
		verts[2] = -x + y - z;
		verts[3] =  x + y - z;
		verts[4] = -x - y + z;
		verts[5] =  x - y + z;
		verts[6] = -x + y + z;
		verts[7] =  x + y + z;
		
		ENGINE.SwapYZ((FVector *)verts, 8);
		{
	    PROFILE_FN(RMMeshDrawTransWrapper1);
		PersTransWrapper(8,(tvector *)verts, (tvector *)verts, pos + origin, ID_FMATRIX);
		
		}
		SINT 	foo;
		BOOL	ok;
		
		BOOL	zvalid[8];
		
		// check each edge
		ok = FALSE;		
		for (foo = 0; foo < 8; foo ++)
		{
			if (verts[foo].Z < 0)
			{
				ok = TRUE;
				zvalid[foo] = TRUE;
			}
			else
				zvalid[foo] = FALSE;
		}
		if (!ok) return;
		
		ok = FALSE;		
		for (foo = 0; foo < 8; foo ++)
		{
			if ((verts[foo].Y < IY2 + 60) && zvalid[foo])
				ok = TRUE;
		}
		if (!ok) return;

		ok = FALSE;		
		for (foo = 0; foo < 8; foo ++)
		{
			if ((verts[foo].X >= IX1) && zvalid[foo])
				ok = TRUE;
		}
		if (!ok) return;
		
		ok = FALSE;		
		for (foo = 0; foo < 8; foo ++)
		{
			if ((verts[foo].Y >= IY1) && zvalid[foo])
				ok = TRUE;
		}
		if (!ok) return;
		
		ok = FALSE;		
		for (foo = 0; foo < 8; foo ++)
		{
			if ((verts[foo].X < IX2) && zvalid[foo])
				ok = TRUE;
		}
		if (!ok) return;
	
	}
	
	//*****************************************
	BOOL	rotate = TRUE;
	if (ri && ri->mWaveResponse == WR_DONT_ROTATE)
		rotate = FALSE;

	FVector     tpos = pos;

	// We're moving the object relative to the ground, so offset the Zpos...
	if(ri)
	{
		if(ri->mWaveResponse==WR_MOVE)
			tpos.Z=GToF(MAP.GetMapHeight(FVectorToGVector(tpos)));
	}
	GINT wh = GAME.GetLevelData()->mGVars[WATER_HEIGHT];

	// JCL temporary #2
	SINT	frame;
	SINT	f1 = 0, f2 = 0;
	SINT	frac = 0;

	if(t)
	{
		SINT	animind = t->GetRealAnimIndex();

		if ((mNumVFrames > 1) && (mBaseMesh->mNumAnimModes) && (animind >= 0))
		{
			GINT			vframe = t->GetRenderFrame();
			CDCAnimMode	*am   = &mBaseMesh->mAnimModes[animind];
			vframe *= am->mFrameScaler; // this fraction varies for looped and unlooped animations
								   	// see loader code

			SINT	vf1 = (vframe.Whole()+am->mStart) % mNumVFrames;		//### % shouldn't be needed
			SINT	vf2 = vf1 + 1;
			if(vf2 > am->mLast) vf2 = am->mStart;

			// now lookup into the vframes table
			GINT	af1 = mVFrames[vf1];
			GINT	af2 = mVFrames[vf2];

			if (af2 < af1) af2 = G((af1).Whole() + 1);

			GINT	aframe = (af2 * vframe.GFrac()) + (af1 * (G1 - vframe.GFrac()));

			f1 = aframe.Whole();
			f2 = f1 + 1;
			if	(f2 > (mVFrames[am->mLast]).Whole()) f2 = (mVFrames[am->mStart]).Whole();

			frac = aframe.Frac();
		}
	}
	frame = f1;
	
	if ((f1 < 0) || (f2 < 0) ||
		(f1 > 1000) || (f2 > 1000))
	{
//		printf("****** Silly frame number: %d, %d\n", f1, f2);
		return;
	}

	// Store result from camera offset inverse transform...

	ASSERT(f1 >= 0);
	ASSERT(f2 >= 0);
	
	FVector npos = pos;
	FMatrix nori = ori;
	
	GINT	scale;
	if(t && ((scale = t->GetRenderScale()) != G1))
	{
		float	fsc = GToF(scale);
		float	s_z = 2.f - fsc;
		float	s_xy = fsc;
		
		nori.Row[0].X *= s_xy;
		nori.Row[0].Y *= s_xy;
		nori.Row[0].Z *= s_xy;
		nori.Row[1].X *= s_xy;
		nori.Row[1].Y *= s_xy;
		nori.Row[1].Z *= s_xy;
		nori.Row[2].X *= s_z;
		nori.Row[2].Y *= s_z;
		nori.Row[2].Z *= s_z;
	}
	
	float sfrac = float(frac) / 65535.f;
	
	if((mNumVFrames==1)||(frac==0))
	{
		for (int tl=0;tl<mNumVertices;tl++)
		{
			in[tl].x=((mVertices[frame][tl].mVertex.X));//+npos.X;
			in[tl].z=((mVertices[frame][tl].mVertex.Y));//+npos.Y;
			in[tl].y=((mVertices[frame][tl].mVertex.Z));//+npos.Z;						
		}
	}
	else
	{
		// interpolation code ***********************
		for (int tl=0;tl<mNumVertices;tl++)
		{
			float vx1=((mVertices[f1][tl].mVertex.X));//+npos.X;
			float vx2=((mVertices[f2][tl].mVertex.X));//+npos.X;
			float vy1=((mVertices[f1][tl].mVertex.Y));//+npos.Y;
			float vy2=((mVertices[f2][tl].mVertex.Y));//+npos.Y;
			float vz1=((mVertices[f1][tl].mVertex.Z));//+npos.Z;
			float vz2=((mVertices[f2][tl].mVertex.Z));//+npos.Z;							
			
			in[tl].x=((vx2-vx1)*sfrac)+vx1;
			in[tl].z=((vy2-vy1)*sfrac)+vy1;
			in[tl].y=((vz2-vz1)*sfrac)+vz1;						
		}
	}
	PersTransWrapper(mNumVertices,(tvector *)&in,(tvector *)&out,npos,nori);
	

	if (!ri)
		skin = 0;
	else
	{
		if ((ri->i3 >= 0) && (ri->i3 < mNumSkins))
			skin = ri->i3;
	}

	if (t && (t->GetRenderThingMiscFlags() & RTMF_ONTOP))
	{
		SINT	c0;

		for (c0 = 0; c0 < mNumVertices; c0 ++)
			out[c0].z = -9.f;
	}
	}
	
	{
    PROFILE_FN(RMMeshDrawLights);
	
	// lighting!
	SINT 	c0;


	if ((t) && ((t->GetRenderThingMiscFlags() & RTMF_NOLIGHT) || (t->GetRenderThingMiscFlags() & RTMF_WHITE)))
	{
		for (c0 = 0; c0 < mNumNormals; c0 ++)
			rgb[c0] = 0xffffffff;
	}
	else
	{
		FVector lv1 = FVector( 1.f, -0.1f, 0);
		FVector lv2 = FVector(-1.f, -0.1f, 0);
		FVector	lv3 = FVector(0, 1.f, 0);

		float lr, lg, lb;
		
		if (t && light_from_map)		
		{
			DWORD col = MAP.GetCell(CCellRef(t->GetRenderPos()))->mColour;
			
			lr = float((col >> 16) & 0xff) / 512.f;
			lg = float((col >>  8) & 0xff) / 512.f;
			lb = float((col      ) & 0xff) / 512.f;
		}
		else
		{
			lr = lg = lb = 0.5f;
		}
		
		FVector	lc1 = FVector(lr * 4.f, lg * 2.f, lb);
		FVector lc2 = FVector(lr, lg * 2.f, lb * 4.f);
		FVector lc3 = FVector(2.f, 2.f, 2.f);
		
		lv1 = ori.Transpose() * lv1;
		lv2 = ori.Transpose() * lv2;
		lv3 = ori.Transpose() * lv3;
		
		for (c0 = 0; c0 < mNumNormals; c0 ++)
		{
			SINT	r, g, b;
	
			float v1 = mNormals[c0].mNormal * lv1;
			float v2 = mNormals[c0].mNormal * lv2;
			float v3 = mNormals[c0].mNormal * lv3;
			
			r  = light(v1 * lc1.X);
			g  = light(v1 * lc1.Y);
			b  = light(v1 * lc1.Z);
			r += light(v2 * lc2.X);
			g += light(v2 * lc2.Y);
			b += light(v2 * lc2.Z);
			r += light(v3 * lc3.X);
			g += light(v3 * lc3.Y);
			b += light(v3 * lc3.Z);
			
			r += 40;
			g += 40;
			b += 40;
			
			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;
			if (r < 0) r = 0;
			if (g < 0) g = 0;
			if (b < 0) b = 0;
			
			rgb[c0] = (r << 16) + (g << 8) + (b);
		}
	}
	}	

	// JCL - setup triangle headers
	{
    PROFILE_FN(RMMeshDrawHeaders);
    int c0;
	for (c0 = 0; c0 < mNumTexturesUsed; c0 ++)
	{
		SINT TN = mTexturesUsed[c0] + 1;
		
		if (IsTextureAlpha(TN))
		{
	  		kmChangeStripListType           ( &TextureHead[TN], KM_TRANS_POLYGON );	
	  		kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
  		}
  		else
  		{
	  		kmChangeStripListType           ( &TextureHead[TN], KM_OPAQUE_POLYGON );	
	  		kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
  		}
  		kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
	 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);
	}
	
}
	SINT	tl = -1;
	
//	for (int tl=0;tl<mNumTriangles;tl++)
{
    PROFILE_FN(RMMeshDrawMesh);
	kmxxGetCurrentPtr( &VertexBufferDesc );
	
	SINT	last_texture_page = -1;
	
	while (1)
	{
		// cache coherency stuff
top:			
		tl ++;
		if (tl >= mNumTriangles)
			break;
	
	    int f1=mGTriangles[tl].mVerts[0];
		int f2=mGTriangles[tl].mVerts[1];
		int f3=mGTriangles[tl].mVerts[2];
		
		// back face
		if (((out[f2].x - out[f1].x) * (out[f3].y - out[f1].y)) - 
		    ((out[f3].x - out[f1].x) * (out[f2].y - out[f1].y)) <= 0)
		    continue;

	    if ((out[f1].z > 0) ||
		    (out[f2].z > 0) ||
		    (out[f3].z > 0) ||
		    (out[f1].z <= -10.f) ||
		    (out[f2].z <= -10.f) ||
		    (out[f3].z <= -10.f))
		    goto top; // sorry!!!!!!!
		
		int n1=mGTriangles[tl].mNorms[0];
		int n2=mGTriangles[tl].mNorms[1];
		int n3=mGTriangles[tl].mNorms[2];
		
		SDCMeshTTriangle *tt = &mTTriangles[skin][tl];
		
		SINT  TN=mTTriangles[skin][tl].mTpage+1;
		
		if (TN != last_texture_page)
		{
 			kmxxStartStrip( &VertexBufferDesc, &TextureHead[TN] );
 			last_texture_page = TN;
 		}
 		kmxxSetVertex_3( KM_VERTEXPARAM_NORMAL,     out[f1].x, out[f1].y, -out[f1].z, tt->mU0,tt->mV0, rgb[n1],0x00000000 );
 		kmxxSetVertex_3( KM_VERTEXPARAM_NORMAL,     out[f2].x, out[f2].y, -out[f2].z, tt->mU1,tt->mV1, rgb[n2],0x00000000 );
 		kmxxSetVertex_3( KM_VERTEXPARAM_ENDOFSTRIP, out[f3].x, out[f3].y, -out[f3].z, tt->mU2,tt->mV2, rgb[n3],0x00000000 );
		         
	}
	
   	kmxxReleaseCurrentPtr(&VertexBufferDesc);	
    } 
}

//******************************************************************************************
#pragma dont_inline on

void	CDCMesh::RenderMeshHierarchy(IRenderableThing *t, SRTInfo *ri, FVector p, FMatrix ori, SDCMeshPart *mp)
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

	if (t)
	{
//		GVector po = t->GetRenderPos(mp->mPartNumber);
//		p += po;
		p+= GVectorToFVector(t->GetRenderPos(mp->mPartNumber));

//		GMatrix	m = t->GetRenderOrientation(mp->mPartNumber);
//		ori = ori * m;
		ori = ori * GMatrixToFMatrix(t->GetRenderOrientation(mp->mPartNumber));
	}

	// render this part
	if ((!t) || (t->RenderRenderPart(mp->mPartNumber)))
	{
		mp->Render(t, ri, p, ori);
	}

	// and the children
	SINT	c0;

	for (c0 = 0; c0 < mp->mNumChildren; c0 ++)
	{
		RenderMeshHierarchy(t, ri, p, ori, mp->mChildren[c0]);
	}
}

#pragma dont_inline off

//******************************************************************************************
// static access function
void CDCMesh::RenderAtPos(const FVector &pos,const FMatrix &ori)
{
	light_from_map = FALSE;
 	RenderMeshHierarchy(NULL,NULL,pos, ori, mParts[0]);
}




void	CDCMesh::RenderFromData(IRenderableThing *t, SRTInfo *ri, BOOL light)
{
	SINT	 	mesh_no;
	CDCMesh	*mesh;

	if (!t)
		return;
		
	light_from_map = light;

	// render top first?
	mesh_no = ri->i1;
	if (mesh_no >= 0)
	{
		mesh = ENGINE.GetMesh(mesh_no);
		if ((mesh->mNumParts > 0) && (mesh->mParts))
			RenderMeshHierarchy(t, ri, GVectorToFVector(t->GetRenderPos()), GMatrixToFMatrix(t->GetRenderOrientation(1)), mesh->mParts[0]);
	}

	// and bottom - bearing in mind that it might be a named mesh
	mesh_no = ri->i0;
	if (mesh_no >= 0)
	{
		// hack for front end
		if (mesh_no >= 10000)
		{
			mesh = ENGINE.GetNumberedMesh(mesh_no - 10000);
		}
		else
			mesh = ENGINE.GetMesh(mesh_no);
	}
	else
	{
		// named mesh - so get the mesh number from the thing.
		mesh_no = -1;

		if (t)
			mesh_no = t->GetRenderMesh();

		if (mesh_no >= 0)
			mesh = ENGINE.GetGlobalMesh(mesh_no);
		else
			mesh = ENGINE.GetDefaultMesh();
	}
	if ((mesh->mNumParts > 0) && (mesh->mParts))
	{
		FMatrix o = ID_FMATRIX;
		FVector p = ZERO_FVECTOR;
		RenderMeshHierarchy(t, ri, p, o, mesh->mParts[0]);
	}

	if(ri->mShadow==ST_SPRITE)
	{
		if (!(COORDS_INVALID(t->GetRenderPos())))
		{
			MAP.DrawShadow(t, ENGINE.GetOtherTexture(ri->i2));
		}
	}

}

//******************************************************************************************
GINT	CDCMesh::GetFrameIncrement(EAnimMode a, SINT* realindex)
{
//	printf("Anim Mode sought %d : num anims %d\n", a, mNumAnimModes);

	CDCAnimMode* am=mAnimModes;
	for(int i=0; i<mNumAnimModes; am++, i++)
	{
//		printf("Anim Mode %d\n", am->mMode);
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
GVector	CDCMesh::GetEmitter(SINT no, CThing *t)
{
	if ((mEmitters) && (no >= 0) && (no < mNumEmitters))
	{
		// traverse the hierarchy to rotate emitter into position
		SDCMeshPart	*mp		= mEmitters[no].mMeshPart;
		FVector			pos		= mEmitters[no].mPos;
		FMatrix			ori		= ID_FMATRIX;
		FMatrix			lori	= ID_FMATRIX;

		while (mp)
		{
			lori  = mp->mOrientation;

			if (t)
			{
				FMatrix	m = GMatrixToFMatrix(t->GetRenderOrientation(mp->mPartNumber));
				lori = lori * m;
			}
			pos  = lori * pos;
			pos += mp->mOffsetPos;
			ori = lori * ori;

			if (t && (!t->RenderUseHierarchy(mp->mPartNumber)))
				break; // we're done here

			mp = mp->mParent;
		};

		return FVectorToGVector(pos);
	}
	else
		return ZERO_GVECTOR;
}

//******************************************************************************************
GVector	SVectorToGVector(SVECTOR sv)
{
	GVector v;
	v.X.SetVal(sv.vx << 8);
	v.Y.SetVal(sv.vy << 8);
	v.Z.SetVal(sv.vz << 8);
	return v;
}

GVector	CDCMesh::GetEmitterAnimated(SINT no, CThing *t)
{
	GVector pos;
	GMatrix ori = ID_GMATRIX;

	if ((mEmitters) && (no >= 0) && (no < mNumEmitters))
	{
		SDCMeshPart	*mp = mEmitters[no].mMeshPart;

		// first, get the animated position of the vertex

		int		animind;
		if (t &&
			(mp->mNumVFrames >1) &&
			(mp->mBaseMesh->mNumAnimModes)&&
			((animind = t->GetRealAnimIndex())>= 0))
		{
			GINT		vframe = t->GetRenderFrame();
			CDCAnimMode	*am = &mp->mBaseMesh->mAnimModes[animind];

			vframe *= am->mFrameScaler; // this fraction varies for looped and unlooped animations
									// see loader code

			SINT	vf1 = (vframe.Whole()+am->mStart) % mp->mNumVFrames;		//### % shouldn't be needed
			SINT	vf2 = vf1 + 1;
			if(vf2 > am->mLast) vf2 = am->mStart;

			GINT	af1 = mp->mVFrames[vf1];
			GINT	af2 = mp->mVFrames[vf2];

			if (af2 < af1) af2 = G(af1.Whole() + 1);

			GINT	aframe = (af2 * vframe.GFrac()) + (af1 * (G1 - vframe.GFrac()));

			SINT	f1 = aframe.Whole();
			SINT	f2 = f1 + 1;
			if	(f2 > (mp->mVFrames[am->mLast]).Whole()) f2 = (mp->mVFrames[am->mStart]).Whole();

			SDCMeshVertex	*pverts1 = mp->mVertices[f1];
			SDCMeshVertex	*pverts2 = mp->mVertices[f2];

			GINT	frac2 = aframe - G(aframe.Whole());
			GINT	frac1 = G1 - frac2;

			pos = ((FVectorToGVector(pverts1[mEmitters[no].mTrackingVertex].mVertex) * frac1)) +
			      (FVectorToGVector(pverts2[mEmitters[no].mTrackingVertex].mVertex) * frac2);

			GMatrix			lori	= ID_GMATRIX;

			// now follow down the hierarchy
			while (mp)
			{
				lori  = FMatrixToGMatrix(mp->mOrientation);

				GMatrix	m = t->GetRenderOrientation(mp->mPartNumber);
				lori = lori * m;

				pos  = lori * pos;
				pos += FVectorToGVector(mp->mOffsetPos);

				if ((mp->mParent)) // not at top
				{
					pos += t->GetRenderPos(mp->mPartNumber);
				}

				ori = lori * ori;

				if (!t->RenderUseHierarchy(mp->mPartNumber))
					break; // we're done here

				mp = mp->mParent;
			};

			return pos;
		}
		else
			return GetEmitter(no, t);
	}
	return ZERO_GVECTOR;
}
//******************************************************************************************
void	CDCMesh::EvaluateCollisionVolume(CThing *t)
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
CCollisionVolume	*CDCMesh::GetCollisionVolume()
{
	if (mParts && mParts[0])
		return mParts[0]->mCollisionVolume;
	else
		return NULL;
}



//******************************************************************************************
void	SDCMeshPart::GenerateSimpleBoundingBox()
{
	if (mNumVertices == 0)
	{
		mCollisionVolume = NULL;
		return;
	}

	// get extremities
	GINT	minx = GINT_MAX;
	GINT	miny = GINT_MAX;
	GINT	minz = GINT_MAX;
	GINT	maxx = GINT_MIN;
	GINT	maxy = GINT_MIN;
	GINT	maxz = GINT_MIN;

	GINT	maxdist = G0;

	SINT			c0;
	SDCMeshVertex *v = mVertices[0];
	GVector			gv;

	for (c0 = 0; c0< mNumVertices; c0++)
	{
		gv = FVectorToGVector(v->mVertex);

		if (gv.X < minx)	minx = gv.X;
		if (gv.Y < miny)	miny = gv.Y;
		if (gv.Z < minz)	minz = gv.Z;
		if (gv.X > maxx)	maxx = gv.X;
		if (gv.Y > maxy)	maxy = gv.Y;
		if (gv.Z > maxz)	maxz = gv.Z;

		GINT	d = (gv + FVectorToGVector(mOffsetPos)).MagnitudeSq();
		if (d > maxdist)
			maxdist = d;

		v ++;
	}

	GVector gori  = (GVector(minx, miny, minz) + GVector(maxx, maxy, maxz)) * GINT_HALF;
	GVector gaxes = (GVector(minx, miny, minz) - GVector(maxx, maxy, maxz)) * GINT_HALF;
	gaxes.X = GABS(gaxes.X);
	gaxes.Y = GABS(gaxes.Y);
	gaxes.Z = GABS(gaxes.Z);

	mCollisionVolume = (CCollisionVolume *)PLATFORM.GetLevelPoolData(sizeof(CCollisionVolume));
	ASSERT(mCollisionVolume);

	// fill out stuff (can't use the constructor, I think...)
	mCollisionVolume->mType = VT_BOUNDING_BOX;
	mCollisionVolume->mOrigin = gori;
	mCollisionVolume->mAxes = gaxes;
	mCollisionVolume->mOrientation = ID_GMATRIX;
	mCollisionVolume->mTransformedOrigin = gori;
	mCollisionVolume->mTransformedOrientation = ID_GMATRIX;
	mCollisionVolume->mNext = NULL;
	mCollisionVolume->mNextPart = NULL;
	mCollisionVolume->mMeshPart = NULL;

	mRadius = GROOT(maxdist);
}

//******************************************************************************************
#pragma dont_inline on
void	SDCMeshPart::EvaluateCollisionVolumeHierarchy(FVector pos, FMatrix ori, CThing *t)
{
	pos += ori * mOffsetPos;
	ori = ori * mOrientation;

	if (t)
	{
		GMatrix	m = t->GetRenderOrientation(mPartNumber);
		ori = ori * GMatrixToFMatrix(m);
	}


	CCollisionVolume *cv = mCollisionVolume;

	while (cv)
	{
		GMatrix gori = FMatrixToGMatrix(ori);
		cv->mTransformedOrigin      =  (gori * cv->mOrigin) + FVectorToGVector(pos);
		cv->mTransformedOrientation = gori;

		cv = cv->mNext;
	}

	SINT	c0;

	for (c0 = 0; c0 < mNumChildren; c0 ++)
		if (mChildren[c0])
			mChildren[c0]->EvaluateCollisionVolumeHierarchy(pos, ori, t);
}
#pragma dont_inline off
//******************************************************************************************
void	SDCMeshPart::Read(CMEMBUFFER &mem, CDCMesh *parent)
{
    int 	i, j;

    // Get vertices
	mem.Read(&mNumVertices, 4);
	mem.Read(&mNumVFrames, 4);
	mem.Read(&mNumAFrames, 4);

	mVFrames = (GINT *)PLATFORM.GetLevelPoolData(sizeof(GINT) * mNumVFrames);
	ASSERT(mVFrames);
	mem.Read(mVFrames, sizeof(GINT) * mNumVFrames);

	mVertices = (SDCMeshVertex **)PLATFORM.GetLevelPoolData(sizeof(SDCMeshVertex *) * mNumAFrames);
	ASSERT(mVertices);

	for (i = 0; i < mNumAFrames; i ++)
	{
		mVertices[i] = (SDCMeshVertex *)PLATFORM.GetLevelPoolData(sizeof(SDCMeshVertex) * mNumVertices);
		ASSERT(mVertices[i]);
		SDCMeshVertex*v=mVertices[i];
		for(j=0;j<mNumVertices;j++)
			mem.Read(v++,12); // sds change from 6 to 12
	}

	// Get Normals
	mem.Read(&mNumNormals, 4);
	mNormals = (SDCMeshNormal *)PLATFORM.GetLevelPoolData(sizeof(SDCMeshNormal) * mNumNormals);
	ASSERT(mNormals);
	SDCMeshNormal*n=mNormals;
	for(int j=0;j<mNumNormals;j++)
			mem.Read(n++,12);

    // Get Triangles
	mem.Read(&mNumTriangles, 4);
	mem.Read(&mNumSkins, 4);

	mGTriangles = (SDCMeshGTriangle *)PLATFORM.GetLevelPoolData(sizeof(SDCMeshGTriangle) * mNumTriangles);
	ASSERT(mGTriangles);

    SDCMeshInputGTriangle	tri;
    for(i = 0; i < mNumTriangles;i ++)
    {
        mem.Read(&tri,sizeof(SDCMeshInputGTriangle));  // read an input triangle.
        mGTriangles[i].mVerts[0] = (tri.mVert0);// * sizeof(SVECTOR);
        mGTriangles[i].mVerts[1] = (tri.mVert1);// * sizeof(SVECTOR);
        mGTriangles[i].mVerts[2] = (tri.mVert2);// * sizeof(SVECTOR);
        mGTriangles[i].mNorms[0] = (tri.mNorm0);
        mGTriangles[i].mNorms[1] = (tri.mNorm1);
        mGTriangles[i].mNorms[2] = (tri.mNorm2);
    }

	mTTriangles = (SDCMeshTTriangle **)PLATFORM.GetLevelPoolData(sizeof(SDCMeshTTriangle *) * mNumSkins);
	ASSERT(mTTriangles);

    for (j = 0; j < mNumSkins; j++)
    {
		mTTriangles[j] = (SDCMeshTTriangle *)PLATFORM.GetLevelPoolData(sizeof(SDCMeshTTriangle) * mNumTriangles);
		ASSERT(mTTriangles[j]);

		SDCMeshInputTTriangle	tri;
	    SDCMeshTTriangle *tt = mTTriangles[j];
		for(i = 0; i < mNumTriangles;i ++)
		{
		    mem.Read(&tri,sizeof(SDCMeshInputTTriangle));  // read an input triangle.
		    tt->mU0 = tri.mU0;
		    tt->mV0 = tri.mV0;
		    tt->mU1 = tri.mU1;
		    tt->mV1 = tri.mV1;
		    tt->mU2 = tri.mU2;
		    tt->mV2 = tri.mV2;
		    tt->mTpage = tri.mTPageNo;
		    //sds tt->mTpage = getTPage(tri.mTexBitDepth,tri.mTexAlphaMode, tri.mTPageX,tri.mTPageY);
		    //sds tt->mClut = getClut(tri.mClutX, tri.mClutY);

//		    printf("Read u0: %d \n", tri.mU0);

		    tt ++;
		}
	}

    // OK - now read the rest of the mesh part information

	// part number
    mem.Read(&mPartNumber, 4);

	// children
    mem.Read(&mNumChildren, 4);
	SINT 	c;
	for (i = 0; i < mNumChildren; i ++)
  	{
   		mem.Read(&c, 4);

   		if (c == -1)
   			mChildren[i] = NULL;
   		else
   			mChildren[i] = parent->mParts[c];
   	}

	// parent
	mem.Read(&c, 4);

	if (c == -1)
		mParent = NULL;
	else
		mParent = parent->mParts[c];

	// offsets
	mem.Read(&mOffsetPos  , sizeof(mOffsetPos  ));
	mem.Read(&mOrientation, sizeof(mOrientation));

	mBaseMesh = parent;
	
	// JCL - check textures
	mNumTexturesUsed = 0;
	
	SINT	c0, c1, c2;
	for (c0 = 0; c0 < mNumSkins; c0 ++)
	{
		for (c1 = 0; c1 < mNumTriangles; c1 ++)
		{
			BOOL used = FALSE;
			
			for (c2 = 0; c2 < mNumTexturesUsed; c2 ++)
				if (mTexturesUsed[c2] == mTTriangles[c0][c1].mTpage)
					used = TRUE;
			
			if (!used)
				mTexturesUsed[mNumTexturesUsed ++] = mTTriangles[c0][c1].mTpage;
		}
	}
}

//******************************************************************************************
void	CDCMesh::Read(CMEMBUFFER &mem)
{
	mLastFrameCollisionGenerated = -2;

	// first read number of parts
	mem.Read(&mNumParts, 4);

	// and allocate memory
	mParts = (SDCMeshPart **)PLATFORM.GetLevelPoolData(sizeof(SDCMeshPart *) * mNumParts);
	ASSERT(mParts);

	SINT	c0;
	for (c0 = 0; c0 < mNumParts; c0 ++)
	{
		mParts[c0] = (SDCMeshPart *)PLATFORM.GetLevelPoolData(sizeof(SDCMeshPart));
		ASSERT(mParts[c0]);
	}

	// now read the parts
	for (c0 = 0; c0 < mNumParts; c0 ++)
		mParts[c0]->Read(mem, this);

	// Emitters
	mem.Read(&mNumEmitters, 4);
	if (mNumEmitters > 0)
	{
		mEmitters = (CDCEmitter *)PLATFORM.GetLevelPoolData(sizeof(CDCEmitter) * mNumEmitters);
		ASSERT(mEmitters);

		for (c0 = 0; c0 < mNumEmitters; c0 ++)
		{
			SINT	i;
			mem.Read(&i, 4);
			mEmitters[c0].mMeshPart = mParts[i];

			mem.Read(&mEmitters[c0].mPos, sizeof(GVector));

			mem.Read(&mEmitters[c0].mTrackingVertex, 4);
		}
	}
	else
		mEmitters = NULL;

	// Animation Stuff
	mem.Read(&mNumAnimModes, 4);

	if (mNumAnimModes > 0)
	{
		mAnimModes = (CDCAnimMode *)PLATFORM.GetLevelPoolData(sizeof(CDCAnimMode) * mNumAnimModes);
		ASSERT(mAnimModes);
		mem.Read(mAnimModes, sizeof(CDCAnimMode) * mNumAnimModes);
	}
	else
		mAnimModes = 0;

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

}




#endif
