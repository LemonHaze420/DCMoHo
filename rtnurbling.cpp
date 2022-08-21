#include	"Common.h"

#if TARGET == PC

#include	"RTNurbling.h"
#include	"Globals.h"
#include	"RawLoader.h"
//======================================================-==--=-- --  -
void	CRTNurbling::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEX::GetTexture(i->mName);
	if (i->mName2[0])
		mTex2 = CTextureGTEX::GetTexture(i->mName2);
	else
		mTex2 = NULL;

	mShadow = i->mShadow;
}
//======================================================-==--=-- --  -
UWORD	arsemesh[]= {0,1,2, 0,3,1, 0,2,3, 0,3,2};
//======================================================-==--=-- --  -
void	CRTNurbling::Render()
{
	/*
	CFVF_TLDSUV	verts[4];		// uggh, braindead
	verts[0].mPos = FVector(0,0,0.1f);
	verts[0].mRHW = 1.0f;
	verts[0].mDiffuse = 0xffffffff;
	verts[0].mSpecular = 0;
	verts[0].mU = 0;
	verts[0].mV = 1;

	verts[1].mPos = FVector(100,0,0.1f);
	verts[1].mRHW = 1.0f;
	verts[1].mDiffuse = 0xffffffff;
	verts[1].mSpecular = 0;
	verts[1].mU = 1;
	verts[1].mV = 1;

	verts[2].mPos = FVector(0,100,0.1f);
	verts[2].mRHW = 1.0f;
	verts[2].mDiffuse = 0xffffffff;
	verts[2].mSpecular = 0;
	verts[2].mU = 0;
	verts[2].mV = 0;

	verts[3].mPos = FVector(100,100,0.1f);
	verts[3].mRHW = 1.0f;
	verts[3].mDiffuse = 0xffffffff;
	verts[3].mSpecular = 0;
	verts[3].mU = 1;
	verts[3].mV = 0;

	PLATFORM.GetD3DDevice()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	PLATFORM.GetD3DDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
	PLATFORM.GetD3DDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

	PLATFORM.GetD3DDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, FVF_TLDSUV_FLAGS,
			verts, 4, NULL);
*/
	FMatrix ori;
	ori = ID_FMATRIX;
	FVector pos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(pos, ori);

	float	radius = GetRenderRadius();

	CCamera	*cam = ENGINE.GetCamera();
	
	FVector p;
	GetRenderPosition(&p);
//==============================

//	p = FVector(0,0,0);

	CFVF_UVRGB	vertz[4];

	vertz[0].mPos = p+FVector(0,1,-1);
	vertz[0].mDiffuse = 0xffffffff;
	vertz[0].mU = 1;
	vertz[0].mV = 1;
	vertz[1].mPos = p+FVector(0,-1,-1);
	vertz[1].mDiffuse = 0xffffffff;
	vertz[1].mU = 0;
	vertz[1].mV = 0;
	vertz[2].mPos = p+FVector(1,0,-1);
	vertz[2].mDiffuse = 0xffffffff;
	vertz[2].mU = 0;
	vertz[3].mV = 1;
	vertz[3].mPos = p+FVector(1,0,1);
	vertz[3].mDiffuse = 0xffffffff;
	vertz[3].mU = 1;
	vertz[3].mV = 0;

	mTex->Activate();
	PLATFORM.GetD3DDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
		FVF_UVRGB_FLAGS, vertz, 4, arsemesh, 4*3, NULL);







//==============================
	FMatrix cori  = GMatrixToFMatrix(cam->mOrientation);
	FVector x = cori * FVector(radius, 0, 0);
	FVector y = cori * FVector(0, radius, 0);

	DWORD	col = mThing->GetRenderColour();

	SINT		i[4];
	CFVF_GTEX	vert[4];

	if ((mShadow == ST_SPRITE) && (!PLATFORM.IsKeyDown(VK_F11)))
	{
		if (!(COORDS_INVALID(CCellRef(FVectorToGVector(p)))))
		{	
			float dist = -(p.Z - GToF(MAP.GetMapHeight(FVectorToGVector(p))));
			if (dist > 0)
			{
				float shadow_scale = 8.f / (dist + 8.f);
				// draw shadow... (first - because of the sorting problems
				FVector x = FVector(radius * shadow_scale, 0, 0);
				FVector y = FVector(0, radius * shadow_scale, 0);

				for (int c1 = 0;c1 < 4; c1 ++)
				{
					vert[c1].mPos	= p + ((c1 & 1) ? x : -x) + ((c1 & 2) ? y : -y);
					vert[c1].mU	= (c1&1)?1.f:0.f;
					vert[c1].mV	= (c1&2)?1.f:0.f;
					vert[c1].mDiffuse = col;
				}

				MAP.ClipAndDrawShadowPoly(mTex, &vert[0], &vert[2], &vert[3]);
				MAP.ClipAndDrawShadowPoly(mTex, &vert[0], &vert[3], &vert[1]);
			}
		}
	}

	CTextureGTEX *t = mTex2 ? ((mThing->GetRenderFrame() >= G(1)) ? mTex2 : mTex) : mTex;

	for (int c1 = 0;c1 < 4; c1 ++)
	{
		vert[c1].mPos	= p + ((c1 & 1) ? x : -x) + ((c1 & 2) ? y : -y);
		vert[c1].mU	= (c1&1)?1.f:0.f;
		vert[c1].mV	= (c1&2)?1.f:0.f;
		vert[c1].mDiffuse = col;

		i[c1] = t->AddVertex(&(vert[c1]));
	}



	t->AddTri(i[0], i[2], i[3]);
	t->AddTri(i[0], i[3], i[1]);


//	DDCOLORKEY k;
//	k.dwColorSpaceLowValue = 0x00000000;
//	k.dwColorSpaceHighValue = 0x00000000;

//	mTex->GetSurface()->GetSurface()->SetColorKey(DDCKEY_SRCBLT, &k);
//	ENGINE.GetDevice()->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, TRUE );

	t->DrawAll();
	if (t != mTex)
		mTex->DrawAll();
//	ENGINE.GetDevice()->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, FALSE );
//	mTex->GetSurface()->GetSurface()->SetColorKey(DDCKEY_SRCBLT, NULL);



}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CRTNurbling::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	ta.AddTexture(mTex, TATT_OTHER);
	ta.AddTexture(mTex2, TATT_OTHER);
}

//******************************************************************************************
void	CRTNurbling::FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt)
{
	if (mTex)
		ri.i0 = ta.GetTextureIndex(TATT_OTHER, mTex);
	else
		ri.i0 = -1;

	if (mTex2)
		ri.i1 = ta.GetTextureIndex(TATT_OTHER, mTex2);
	else
		ri.i1 = -1;

	ri.mShadow = mShadow;
}
#endif
#endif
