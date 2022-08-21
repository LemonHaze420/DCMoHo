#include	"Common.h"

#if TARGET == DC

#include	"DCDecal.h"
#include	"Globals.h"
#include "dc3dengine.h"
#include 	"MainCode.h"
#include	"DCJunk.h"

void	swapRGB(DWORD &col)
{
	SINT	r = (col >> 16) & 0xff;
	SINT	g = (col >>  8) & 0xff;
	SINT	b = (col      ) & 0xff;
	
	col = (b << 16) + (g << 8) + r;
}

//******************************************************************************************
void	CDCDecal::CommonDecalRender(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT z_fudge)
{
	swapRGB(col);

	SINT	TN = tr->mTPage + 1;
	
	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );

	FMatrix cori  = GMatrixToFMatrix(ENGINE.GetCamera()->mOrientation);
	float	radius = GToF(render_radius);
	FVector x = cori * FVector(radius, 0, 0);
	FVector y = cori * FVector(0, radius, 0);

	FVector	verts[2][2];
	
	verts[0][0] = -x - y;
	verts[0][1] =  x - y;
	verts[1][0] = -x + y;
	verts[1][1] =  x + y;
	
	ENGINE.SwapYZ((FVector *)verts, 4);
	
	FVector	p = GVectorToFVector(*pos);
	PersTransWrapper(4,(tvector *)verts, (tvector *)verts, p , ID_FMATRIX);

	if ((verts[0][0].Z > 0.f) || (verts[0][1].Z > 0.f) || (verts[1][0].Z > 0.f) || (verts[1][1].Z > 0.f))
	    return;

	InitCZTs(TN);

	float	u0 = tr->mX + 0.004f;
	float	u1 = tr->mX + tr->mW - 0.004f;
	float	v0 = tr->mY + 0.004f;
	float	v1 = tr->mY + tr->mH - 0.004f;
	
   	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[0].fX                   = verts[0][0].X;
   	TextureS[0].fY                   = verts[0][0].Y;
   	TextureS[0].u.fZ                 = -verts[0][0].Z;
   	TextureS[0].uBaseRGB.dwPacked    = col;
   	TextureS[0].fU                   = u0;
   	TextureS[0].fV                   = v1;

   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].fX                   = verts[0][1].X;
   	TextureS[1].fY                   = verts[0][1].Y;
   	TextureS[1].u.fZ                 = -verts[0][1].Z;
   	TextureS[1].uBaseRGB.dwPacked    = col;
   	TextureS[1].fU                   = u0;
   	TextureS[1].fV                   = v0;

   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].fX                   = verts[1][0].X;
   	TextureS[2].fY                   = verts[1][0].Y;
   	TextureS[2].u.fZ                 = -verts[1][0].Z;
   	TextureS[2].uBaseRGB.dwPacked    = col;
   	TextureS[2].fU                   = u1;
   	TextureS[2].fV                   = v1;
   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	TextureS[3].fX                   = verts[1][1].X;
   	TextureS[3].fY                   = verts[1][1].Y;
   	TextureS[3].u.fZ                 = -verts[1][1].Z;
   	TextureS[3].uBaseRGB.dwPacked    = col;
   	TextureS[3].fU                   = u1;
   	TextureS[3].fV                   = v0;
	
	ExitCZTs(4);

    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
}

//******************************************************************************************
void DecalRenderFromVerts(long col, GVector *pos, FVector *verts, STextureRefOther *tr)
{
	swapRGB(col);

	SINT	TN = tr->mTPage + 1;
	
	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );


	ENGINE.SwapYZ((FVector *)verts, 4);
	
	FVector	p = GVectorToFVector(*pos);
	PersTransWrapper(4,(tvector *)verts, (tvector *)verts, p , ID_FMATRIX);

	if ((verts[0].Z > 0.f) || (verts[1].Z > 0.f) || (verts[2].Z > 0.f) || (verts[3].Z > 0.f))
	    return;

	InitCZTs(TN);

	float	u0 = tr->mX + 0.004f;
	float	u1 = tr->mX + tr->mW - 0.004f;
	float	v0 = tr->mY + 0.004f;
	float	v1 = tr->mY + tr->mH - 0.004f;
	
   	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[0].fX                   = verts[0].X;
   	TextureS[0].fY                   = verts[0].Y;
   	TextureS[0].u.fZ                 = -verts[0].Z;
   	TextureS[0].uBaseRGB.dwPacked    = col;
   	TextureS[0].fU                   = u0;
   	TextureS[0].fV                   = v1;

   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].fX                   = verts[1].X;
   	TextureS[1].fY                   = verts[1].Y;
   	TextureS[1].u.fZ                 = -verts[1].Z;
   	TextureS[1].uBaseRGB.dwPacked    = col;
   	TextureS[1].fU                   = u0;
   	TextureS[1].fV                   = v0;

   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].fX                   = verts[2].X;
   	TextureS[2].fY                   = verts[2].Y;
   	TextureS[2].u.fZ                 = -verts[2].Z;
   	TextureS[2].uBaseRGB.dwPacked    = col;
   	TextureS[2].fU                   = u1;
   	TextureS[2].fV                   = v1;

   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	TextureS[3].fX                   = verts[3].X;
   	TextureS[3].fY                   = verts[3].Y;
   	TextureS[3].u.fZ                 = -verts[3].Z;
   	TextureS[3].uBaseRGB.dwPacked    = col;
   	TextureS[3].fU                   = u1;
   	TextureS[3].fV                   = v0;
	
	ExitCZTs(4);

    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
}

//******************************************************************************************
void	CDCDecal::CommonDecalRenderHoriz(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT z_fudge, GINT yaw)
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
void	CDCDecal::CommonDecalRenderHoriz2(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT sn,SINT cs)
{
	// sn & cs here are 4.12 ints.
	
	float	snf = float(cs) / 4096.f * GToF(render_radius);
	float	csf = float(sn) / 4096.f * GToF(render_radius);
//	float	snf = float(sn) / 4096.f;
//	float	csf = float(cs) / 4096.f;

	FVector	verts[4];

	float	xx = csf;
	float	xy = -snf;
	float	yx = -snf;
	float	yy = -csf;

	verts[0].X = -xx - yx;
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

//******************************************************************************************************
void    CDCDecal::RenderFromData(CThing *t, const SRTInfo &ri)
{
	if (ri.i3)
	{
		// special case for bullets
		SINT				frame = t->GetRenderFrame().Whole();
		STextureRefOther 	*tr;
		GVector				pos = t->GetRenderPos();
		SINT				tno;

		BOOL				shadow = (ri.mShadow == TRUE);

		if (frame == -1)
		{
			tno = ri.i0;
			if (tno != -1)
			{
				STextureRefOther *tr = ENGINE.GetOtherTexture(tno);
				CommonDecalRenderHoriz(&pos,t->GetRenderRadius(),tr,0xffffffff, 0, t->GetRenderYaw());
			}

			// render the other one as well!
			tno = ri.i1;
			if (tno != -1)
			{
				STextureRefOther *tr = ENGINE.GetOtherTexture(tno);
				CommonDecalRenderHoriz(&pos,t->GetRenderRadius(),tr,0xffffffff, 0, -t->GetRenderYaw());
			}

			tno = ri.i2;
			if((tno != -1) && (ri.mShadow==ST_SPRITE))
			{
				MAP.DrawShadow(t, ENGINE.GetOtherTexture(tno));
			}
		}
		else
		{
			tno = (frame == 0) ? ri.i0 : ri.i1;
			if (tno != -1)
			{
				STextureRefOther *tr = ENGINE.GetOtherTexture(tno);
				CommonDecalRenderHoriz(&pos,t->GetRenderRadius(),tr,0xffffffff, 0, t->GetRenderYaw());
			}

			if (frame != -1)
			{
				if(shadow && (ri.i2 != -1))
				{
					MAP.DrawShadow(t, ENGINE.GetOtherTexture(ri.i2));
				}
			}
		}
	}
	else
	{
		SINT	frame = t->GetRenderFrame().Whole();
		SINT	tno = (frame == 0) ? ri.i0 : ri.i1;
		if (tno == -1)
			return;

		STextureRefOther *tr = ENGINE.GetOtherTexture(tno);
		if ((ri.mShadow==ST_SPRITE) && (ri.i2 != -1))
		{
			MAP.DrawShadow(t, ENGINE.GetOtherTexture(ri.i2));
		}

		GVector		pos = t->GetRenderPos();
		CommonDecalRender(&pos,t->GetRenderRadius(),tr,0xffffffff);
	}
}


#endif
