#include	"Common.h"

#if TARGET == DC
#include	"Globals.h"
//#include    <mwinline.h>
//#include    <gtereg_s.h>
#include    <gtemac.h>
#include	"DCMesh.h"
#include    "DC3dengine.h"
#include	"DCLaser.h"
#include	"Globals.h"
#include 	"MainCode.h"
#include	"DCJunk.h"


//******************************************************************************************
#define	PSXLaser_START_AND_END_SIZE		1.f
#define	PSXLaser_DESIRED_SEGMENT_SIZE	4.f
#define	PSXLaser_HEIGHT					0.5f
#define	PSXLaser_FLARE_SIZE				1.2f

//******************************************************************************************
void	CDCLaser::RenderFromData(CThing *t, const SRTInfo &ri)
{

	GVector gp=t->GetRenderPos();
	GVector gs=t->GetRenderStartPos();
	GVector ge=t->GetRenderEndPos();
	gs -= gp;
	ge -= gp;

	FVector	p = GVectorToFVector(gp);
	FVector	s = GVectorToFVector(gs);
	FVector	e = GVectorToFVector(ge);


	SINT c0, c1, c2;

	// work out how many segments we need
	float	dist = (e - s).Magnitude();
	SINT	num_segs = SINT((dist - (PSXLaser_START_AND_END_SIZE * 2.f)) / PSXLaser_DESIRED_SEGMENT_SIZE);
	if (num_segs < 1)
		num_segs = 1;
	FVector	seg_offset = (e - s) / float(num_segs) * ((dist - (PSXLaser_START_AND_END_SIZE * 2.f) ) / dist);
	FVector	seg_offsetse = (e - s) * PSXLaser_START_AND_END_SIZE / dist;
	FVector perp = seg_offset ^ (GMatrixToFMatrix(ENGINE.GetCamera()->mOrientation) * FVector(0, 0, 1.f));
	num_segs += 2;

	if (perp.Magnitude() > 0.0015)
	{
		perp.Normalise();
		perp *= PSXLaser_HEIGHT / 2.f;
	}
	else
		perp = FVector(0, 0, PSXLaser_HEIGHT / 2.f);

	if (ri.i0 == -1)
		return;

	STextureRefOther *tr;

	FVector	curr_pos = s;

	for	(c0 = 0; c0 < num_segs; c0++)
	{
		if (c0 == 0)
			tr = ENGINE.GetOtherTexture(ri.i1);
		else if (c0 == num_segs - 1)
			tr = ENGINE.GetOtherTexture(ri.i2);
		else
			tr = ENGINE.GetOtherTexture(ri.i0);

		SINT	TN = tr->mTPage + 1;
		kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
	 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
		kmChangeStripMipmapAdjust		( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MIPMAP_D_ADJUST_0_25 ); 	

		FVector	verts[2][2];
		
		verts[0][0] = curr_pos - perp;
		verts[0][1] = curr_pos + perp;
		
		if (c0 == 0)
			curr_pos = s + seg_offsetse;
		else if (c0 != num_segs - 1)
			curr_pos = s + seg_offsetse + (seg_offset * (c0));
		else
			curr_pos = e;
			
		verts[1][0] = curr_pos - perp;
		verts[1][1] = curr_pos + perp;
		
		ENGINE.SwapYZ((FVector *)verts, 4);
		
		PersTransWrapper(4,(tvector *)verts, (tvector *)verts, p , ID_FMATRIX);

		if ((verts[0][0].Z > 0.f) || (verts[0][1].Z > 0.f) || (verts[1][0].Z > 0.f) || (verts[1][1].Z > 0.f))
		    continue;

		InitCZTs(TN);

		float	u0 = tr->mX + 0.004f;
		float	u1 = tr->mX + tr->mW - 0.004f;
		float	v0 = tr->mY + 0.004f;
		float	v1 = tr->mY + tr->mH - 0.004f;
		
       	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[0].fX                   = verts[0][0].X;
       	TextureS[0].fY                   = verts[0][0].Y;
       	TextureS[0].u.fZ                 = -verts[0][0].Z;
       	TextureS[0].uBaseRGB.dwPacked    = 0xffffffff;
       	TextureS[0].fU                   = u0;
       	TextureS[0].fV                   = v1;
	
       	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[1].fX                   = verts[0][1].X;
       	TextureS[1].fY                   = verts[0][1].Y;
       	TextureS[1].u.fZ                 = -verts[0][1].Z;
       	TextureS[1].uBaseRGB.dwPacked    = 0xffffffff;
       	TextureS[1].fU                   = u0;
       	TextureS[1].fV                   = v0;
	
       	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[2].fX                   = verts[1][0].X;
       	TextureS[2].fY                   = verts[1][0].Y;
       	TextureS[2].u.fZ                 = -verts[1][0].Z;
       	TextureS[2].uBaseRGB.dwPacked    = 0xffffffff;
       	TextureS[2].fU                   = u1;
       	TextureS[2].fV                   = v1;
	
       	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
       	TextureS[3].fX                   = verts[1][1].X;
       	TextureS[3].fY                   = verts[1][1].Y;
       	TextureS[3].u.fZ                 = -verts[1][1].Z;
       	TextureS[3].uBaseRGB.dwPacked    = 0xffffffff;
       	TextureS[3].fU                   = u1;
       	TextureS[3].fV                   = v0;
	
		ExitCZTs(4);
		
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
		kmChangeStripMipmapAdjust		( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MIPMAP_D_ADJUST_1_25 ); 	
	}


	// Render Decal - this should be fun...

	p = GVectorToFVector(t->GetRenderStartPos());

	FVector		a = p - GVectorToFVector(ENGINE.GetCamera()->mPos);
	float		ad = a.Magnitude();
	p -= a * 2.f / ad;

	float	radius = PSXLaser_FLARE_SIZE;

	// OK - now some fucked up hacks to rotate the flare
	FVector b = GVectorToFVector(ENGINE.GetCamera()->mLookAt) - GVectorToFVector(ENGINE.GetCamera()->mPos);
	float	bd = b.Magnitude();
	float	nbd = (a * b) / bd;
	b = (b * nbd) / bd ;

	FVector x = b - a;
	x.Normalise();
	FVector y = x ^ b;
	y.Normalise();

	x *=  radius;
	y *= -radius;

	// and render the poly
	tr = ENGINE.GetOtherTexture(ri.i3);

	SINT	TN = tr->mTPage + 1;
	kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );

	InitCZTs(TN);

	FVector	verts[2][2];
	
	verts[0][0] = -x - y;
	verts[0][1] = x - y;
	verts[1][0] = -x + y;
	verts[1][1] = x + y;
	
	ENGINE.SwapYZ((FVector *)verts, 4);
	
	PersTransWrapper(4,(tvector *)verts, (tvector *)verts, p , ID_FMATRIX);

	if ((verts[0][0].Z > 0.f) || (verts[0][1].Z > 0.f) || (verts[1][0].Z > 0.f) || (verts[1][1].Z > 0.f))
	    return;

	float	u0 = tr->mX + 0.004f;
	float	u1 = tr->mX + tr->mW - 0.004f;
	float	v0 = tr->mY + 0.004f;
	float	v1 = tr->mY + tr->mH - 0.004f;
	
   	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[0].fX                   = verts[0][0].X;
   	TextureS[0].fY                   = verts[0][0].Y;
   	TextureS[0].u.fZ                 = -verts[0][0].Z;
   	TextureS[0].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[0].fU                   = u0;
   	TextureS[0].fV                   = v1;

   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].fX                   = verts[0][1].X;
   	TextureS[1].fY                   = verts[0][1].Y;
   	TextureS[1].u.fZ                 = -verts[0][1].Z;
   	TextureS[1].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[1].fU                   = u0;
   	TextureS[1].fV                   = v0;

   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].fX                   = verts[1][0].X;
   	TextureS[2].fY                   = verts[1][0].Y;
   	TextureS[2].u.fZ                 = -verts[1][0].Z;
   	TextureS[2].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[2].fU                   = u1;
   	TextureS[2].fV                   = v1;

   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	TextureS[3].fX                   = verts[1][1].X;
   	TextureS[3].fY                   = verts[1][1].Y;
   	TextureS[3].u.fZ                 = -verts[1][1].Z;
   	TextureS[3].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[3].fU                   = u1;
   	TextureS[3].fV                   = v0;

	ExitCZTs(4);
	
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
}

//******************************************************************************************
#endif
