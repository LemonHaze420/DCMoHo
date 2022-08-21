#include	"Common.h"

#if TARGET == PC || DC

#include	"RTDecal.h"
#include	"Globals.h"
#include	"RawLoader.h"

//******************************************************************************************
void	CRTDecal::Init(CInitRenderThing *i)
{
	SUPERTYPE::Init(i);

	mTex = CTextureGTEXAdditive::GetTexture(i->mName);
	if (i->mName2[0])
		mTex2 = CTextureGTEXAdditive::GetTexture(i->mName2);
	else
		mTex2 = NULL;

	if (i->mName3[0])
		mTex3 = CTextureGTEX::GetTexture(i->mName3);
	else
		mTex3 = NULL;

	mShadow = i->mShadow;
	mAlpha = i->mAlpha;
	mHoriz = i->mHoriz; // this means always render both texutures, second one as horizontal
}


//******************************************************************************************
void	CRTDecal::CommonDecalRender(GVector *pos, GINT render_radius, CTextureGTEXAdditive *tr, long col, SINT z_fudge)
{
	FMatrix ori = ID_FMATRIX;
	FVector fpos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(fpos, ori);

	float	radius = GToF(render_radius);

	CCamera	*cam = ENGINE.GetCamera();
	
	FVector p = GVectorToFVector(*pos);

	FMatrix cori  = GMatrixToFMatrix(cam->mOrientation);
	FVector x = cori * FVector(radius, 0, 0);
	FVector y = cori * FVector(0, radius, 0);

	SINT		i[4];
	CFVF_GTEX	vert[4];

	for (int c1 = 0;c1 < 4; c1 ++)
	{
		vert[c1].mPos	= p + ((c1 & 1) ? x : -x) + ((c1 & 2) ? y : -y);
		vert[c1].mU	= (c1&1)?1.0f:0.0f; // this line needs correcting.
		vert[c1].mV	= (c1&2)?1.0f:0.0f; // as does this. texture uv's?
		vert[c1].mDiffuse = col;

		i[c1] = tr->AddVertex(&(vert[c1]));
	}

	tr->AddTri(i[0], i[2], i[3]);
	tr->AddTri(i[0], i[3], i[1]);

	tr->DrawAll();
}

//******************************************************************************************
void	CRTDecal::DrawShadow()
{
	if (!mTex3)
		return;

	MAP.DrawShadow(mThing, mTex3);
}


//******************************************************************************************
void DecalRenderFromVerts(long col, GVector *pos, FVector *verts, CTextureGTEXAdditive *tr)
{
	if (!tr)
		return;

	FMatrix ori = ID_FMATRIX;
	FVector fpos = FVector(0, 0, 0);
	ENGINE.SetupTransforms(fpos, ori);

	FVector p = GVectorToFVector(*pos);

	SINT		i[4];
	CFVF_GTEX	vert[4];

	for (int c1 = 0;c1 < 4; c1 ++)
	{
		vert[c1].mPos	= p + verts[c1];
		vert[c1].mU	= (c1&1)?1.f:0.f;
		vert[c1].mV	= (c1&2)?1.f:0.f;
		vert[c1].mDiffuse = col;

		i[c1] = tr->AddVertex(&(vert[c1]));
	}

	tr->AddTri(i[0], i[2], i[3]);
	tr->AddTri(i[0], i[3], i[1]);

	tr->DrawAll();
}

//******************************************************************************************
void	CRTDecal::CommonDecalRenderHoriz(GVector *pos, GINT render_radius, CTextureGTEXAdditive *tr, long col, SINT z_fudge, GINT yaw)
{
	FVector	verts[4];

	if (yaw != G0)
	{
		GMatrix	m;
		m.MakeRotationYaw(yaw);

		GVector x = GVector(render_radius, G0, G0);
		GVector	y = GVector(G0, render_radius, G0);

		x = m * x;
		y = m * y;
		
		FVector xf = GVectorToFVector(x);
		FVector yf = GVectorToFVector(y);
		
		verts[0] = -xf - yf;
		verts[1] =  xf - yf;
		verts[2] = -xf + yf;
		verts[3] =  xf + yf;
	}
	else
	{
		float	radius = GToF(render_radius);

		verts[0] = FVector(-radius, -radius, 0);
		verts[1] = FVector( radius, -radius, 0);
		verts[2] = FVector(-radius,  radius, 0);
		verts[3] = FVector( radius,  radius, 0);
	}
	DecalRenderFromVerts(col, pos, verts, tr);
}


//******************************************************************************************
void	CRTDecal::CommonDecalRenderHoriz2(GVector *pos, GINT render_radius, CTextureGTEXAdditive *tr, long col, SINT sn, SINT cs)
{
	// sn & cs here are 4.12 ints.
	
	float	snf = float(sn) / 4096.f;
	float	csf = float(cs) / 4096.f;

	FVector	verts[4];

	float	xx = csf;
	float	xy = snf;
	float	yx = -snf;
	float	yy = csf;

	verts[0].X= -xx - yx;
	verts[0].Y = -xy - yy;

	verts[1].X = +xx - yx;
	verts[1].Y = +xy - yy;

	verts[2].X = -xx + yx;
	verts[2].Y = -xy + yy;

	verts[3].X = +xx + yx;
	verts[3].Y = +xy + yy;

	verts[0].Z = 0;
	verts[1].Z = 0;
	verts[2].Z = 0;
	verts[3].Z = 0;

	DecalRenderFromVerts(col, pos, verts, tr);
}


//******************************************************************************************
void	CRTDecal::Render()
{
	if (!mThing)
		return;

	if (mHoriz)
	{
		// special case for bullets
		SINT				frame = mThing->GetRenderFrame().Whole();
		GVector				pos = mThing->GetRenderPos();

		if (frame == -1)
		{
			if (mTex)
			{
				CommonDecalRenderHoriz(&pos,mThing->GetRenderRadius(), mTex, 0xffffffff, 0, mThing->GetRenderYaw());
			}

			// render the other one as well!
			if (mTex2)
			{
				CommonDecalRenderHoriz(&pos,mThing->GetRenderRadius(), mTex2, 0xffffffff, 0, -mThing->GetRenderYaw());
			}

			if (mShadow == ST_SPRITE)
			{
				DrawShadow();
			}
		}
		else
		{
			CTextureGTEXAdditive *tex;
			tex = (frame == 0) ? mTex : mTex2;
			if (tex)
			{
				CommonDecalRenderHoriz(&pos, mThing->GetRenderRadius(),tex,0xffffffff, 0, mThing->GetRenderYaw());
			}

			if (frame != -1)
			{
				if(mShadow == ST_SPRITE)
				{
					DrawShadow();
				}
			}
		}
	}
	else
	{
		SINT	frame = mThing->GetRenderFrame().Whole();
		CTextureGTEXAdditive *tex;
		tex = (frame == 0) ? mTex : mTex2;

		if (!tex)
			return;

		if(mShadow == ST_SPRITE)
		{
			DrawShadow();
		}

		GVector		pos = mThing->GetRenderPos();
		CommonDecalRender(&pos,mThing->GetRenderRadius(),tex,0xffffffff);
	}
}

//******************************************************************************************
#ifdef LINK_EDITOR
void	CRTDecal::AccumulateResources(CTextureAccumulator &ta, CMeshAccumulator &ma)
{
	if (mHoriz)
	{
/*		ta.AddTexture(mTex , TATT_OTHER, -1, -1, -1, BD_4, 1, FALSE);
		if (mAlpha)
			ta.AddTexture(mTex2, TATT_OTHER, -1, -1, -1, BD_4, 1, TRUE);
		else
			ta.AddTexture(mTex2, TATT_OTHER, -1, -1, -1, BD_4, 1, FALSE);*/
		if (mAlpha)
		{
			ta.AddTexture(mTex , TATT_OTHER, -1, -1, -1, BD_8, 1, TRUE); // why not...
			ta.AddTexture(mTex2, TATT_OTHER, -1, -1, -1, BD_8, 1, TRUE); 
		}
		else
		{
			ta.AddTexture(mTex , TATT_OTHER, -1, -1, -1, BD_4, 1, FALSE);
			ta.AddTexture(mTex2, TATT_OTHER, -1, -1, -1, BD_4, 1, FALSE);
		}
	}
	else if (mAlpha)
	{
		// er... assume 8 bit ?!
		ta.AddTexture(mTex , TATT_OTHER, -1, -1, -1, BD_8, 1, TRUE);
		ta.AddTexture(mTex2, TATT_OTHER, -1, -1, -1, BD_8, 1, TRUE);
	}
	else
	{
		ta.AddTexture(mTex , TATT_OTHER);
		ta.AddTexture(mTex2, TATT_OTHER);
	}
}

//******************************************************************************************
void	CRTDecal::FillOutPSXRenderInfo(SRTInfo &ri, CTextureAccumulator &ta, CMeshAccumulator &ma, CInitRenderThing &pcirt)
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

	ri.i3 = mHoriz;
}

//******************************************************************************************
void	CRTDecal::AccumulateResourcesDC(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma)
{
	if (mHoriz)
	{
/*		ta.AddTexture(mTex , TATT_OTHER, -1, -1, -1, BD_4, 1, FALSE);
		if (mAlpha)
			ta.AddTexture(mTex2, TATT_OTHER, -1, -1, -1, BD_4, 1, TRUE);
		else
			ta.AddTexture(mTex2, TATT_OTHER, -1, -1, -1, BD_4, 1, FALSE);*/
		if (mAlpha)
		{
			ta.AddTexture(mTex , TADCTT_OTHER, -1, -1, -1, 1, TRUE); // why not...
			ta.AddTexture(mTex2, TADCTT_OTHER, -1, -1, -1, 1, TRUE); 
		}
		else
		{
			ta.AddTexture(mTex , TADCTT_OTHER, -1, -1, -1, 1, FALSE);
			ta.AddTexture(mTex2, TADCTT_OTHER, -1, -1, -1, 1, FALSE);
		}
	}
	else if (mAlpha)
	{
		// er... assume 8 bit ?!
		ta.AddTexture(mTex , TADCTT_OTHER, -1, -1, -1, 1, TRUE);
		ta.AddTexture(mTex2, TADCTT_OTHER, -1, -1, -1, 1, TRUE);
	}
	else
	{
		ta.AddTexture(mTex , TADCTT_OTHER);
		ta.AddTexture(mTex2, TADCTT_OTHER);
	}

	//JCL????????
	ta.AddTexture(mTex3, TADCTT_OTHER, 1,  -1, -1, 2, TRUE);

	SUPERTYPE::AccumulateResourcesDC(ta, ma);
}

//******************************************************************************************
void	CRTDecal::FillOutDCRenderInfo(SRTInfo &ri, CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma, CInitRenderThing &pcirt)
{
	if (mTex)
		ri.i0 = ta.GetTextureIndex(TADCTT_OTHER, mTex);
	else
		ri.i0 = -1;

	if (mTex2)
		ri.i1 = ta.GetTextureIndex(TADCTT_OTHER, mTex2);
	else
		ri.i1 = -1;

	if (mTex3)
		ri.i2 = ta.GetTextureIndex(TADCTT_OTHER, mTex3);
	else
		ri.i2 = -1;

	ri.mShadow = mShadow;

	ri.i3 = mHoriz;
}

#endif
#endif
