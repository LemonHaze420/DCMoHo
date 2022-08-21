#include	"Common.h"

#if TARGET == DC
#include	"Globals.h"
//#include    <mwinline.h>
//#include    <gtereg_s.h>
#include    <gtemac.h>
#include	"DCMesh.h"
#include    "DC3dengine.h"
#include	"DCLink.h"
#include	"Globals.h"
#include 	"MainCode.h"
#include	"DCJunk.h"

//******************************************************************************************
#define PSXLINK_NUM_SEGS	4
#define PSXLINK_SEG_SIZE 	0.15f

//******************************************************************************************
void	CDCLink::RenderFromData(CThing *t, const SRTInfo &ri)
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
	FMatrix ori = GMatrixToFMatrix(t->GetRenderOrientation(1));


	// work out how many segments we need
	FVector	seg_offset = (e - s) / float(PSXLINK_NUM_SEGS);
	FVector	x = ori * FVector(PSXLINK_SEG_SIZE, 0, 0);
	FVector	y = ori * FVector(0, PSXLINK_SEG_SIZE, 0);

	if (ri.i0 == -1)
		return;

	STextureRefOther *tr = ENGINE.GetOtherTexture(ri.i0);

	SINT	col = t->GetLife();
	if (col < 0)
		col = 0;
	if (col > 100)
		col = 100;
	
	col <<= 1;

	DWORD	vcol = (col << 8) + ((200 - col) << 16);

	FVector	curr_pos = s;
	
	SINT	TN = tr->mTPage + 1;
	
	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
	

	for	(c0 = 0; c0 < PSXLINK_NUM_SEGS; c0++)
	{
		FVector	verts[2][2];
		
		verts[0][0] = curr_pos - x - y;
		verts[0][1] = curr_pos + x - y;
		verts[1][0] = curr_pos - x + y;
		verts[1][1] = curr_pos + x + y;
		
		ENGINE.SwapYZ((FVector *)verts, 4);
		
		PersTransWrapper(4,(tvector *)verts, (tvector *)verts, p , ID_FMATRIX);

		if (!((verts[0][0].Z > 0.f) || (verts[0][1].Z > 0.f) || (verts[1][0].Z > 0.f) || (verts[1][1].Z > 0.f)))
		{
			InitCZTs(TN);
			
			float	u0 = tr->mX + 0.004f;
			float	u1 = tr->mX + tr->mW - 0.004f;
			float	v0 = tr->mY + 0.004f;
			float	v1 = tr->mY + tr->mH - 0.004f;
			
	       	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[0].fX                   = verts[0][0].X;
	       	TextureS[0].fY                   = verts[0][0].Y;
	       	TextureS[0].u.fZ                 = -verts[0][0].Z;
	       	TextureS[0].uBaseRGB.dwPacked    = vcol;
	       	TextureS[0].fU                   = u0;
	       	TextureS[0].fV                   = v1;
		
	       	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[1].fX                   = verts[0][1].X;
	       	TextureS[1].fY                   = verts[0][1].Y;
	       	TextureS[1].u.fZ                 = -verts[0][1].Z;
	       	TextureS[1].uBaseRGB.dwPacked    = vcol;
	       	TextureS[1].fU                   = u0;
	       	TextureS[1].fV                   = v0;
		
	       	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[2].fX                   = verts[1][0].X;
	       	TextureS[2].fY                   = verts[1][0].Y;
	       	TextureS[2].u.fZ                 = -verts[1][0].Z;
	       	TextureS[2].uBaseRGB.dwPacked    = vcol;
	       	TextureS[2].fU                   = u1;
	       	TextureS[2].fV                   = v1;
		
	       	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
	       	TextureS[3].fX                   = verts[1][1].X;
	       	TextureS[3].fY                   = verts[1][1].Y;
	       	TextureS[3].u.fZ                 = -verts[1][1].Z;
	       	TextureS[3].uBaseRGB.dwPacked    = vcol;
	       	TextureS[3].fU                   = u1;
	       	TextureS[3].fV                   = v0;
		
			ExitCZTs(4);
		}
		
		curr_pos += seg_offset;
	}

    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );


	//********************
	//********************
	//********************
	
#ifdef JCLDCNOPSX			// Rewrite for DC
	
	
	// bit in the middle

	long z=0, dummy;
	POLY_F4		*f4 = (POLY_F4 *)o4;
	
	// get a perpendicular line
	GVector perp = (e - s) ^ (ENGINE.GetCamera()->mOrientation * GVector(G0, G0, G1));
	perp.Normalise();
	perp *= G(0, 3000);

	vert.vx=(s.X.mVal - perp.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(s.Y.mVal - perp.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(s.Z.mVal - perp.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

#if defined(JJS)
	gte_ldv0(&vert);
    gte_rtps();
#endif

	vert.vx=(s.X.mVal + perp.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(s.Y.mVal + perp.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(s.Z.mVal + perp.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

#if defined(JJS)
	gte_stszotz(&dummy);
	gte_stsxy(&f4->x0);
#endif
	z+=dummy;

#if defined(JJS)
	gte_ldv0(&vert);
    gte_rtps();
#endif
	vert.vx=(e.X.mVal - perp.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(e.Y.mVal - perp.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(e.Z.mVal - perp.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

#if defined(JJS)
	gte_stszotz(&dummy);
	gte_stsxy(&f4->x1);
#endif
	z+=dummy;

#if defined(JJS)
	gte_ldv0(&vert);
    gte_rtps();
#endif
	vert.vx=(e.X.mVal + perp.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(e.Y.mVal + perp.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(e.Z.mVal + perp.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

#if defined(JJS)
	gte_stszotz(&dummy);
	gte_stsxy(&f4->x2);
#endif
	z+=dummy;

#if defined(JJS)
	gte_ldv0(&vert);
    gte_rtps();

	gte_stszotz(&dummy);
	gte_stsxy(&f4->x3);
#endif
	z+=dummy;

	z=(z-64)>>4;
	if((z>0)&&(z<850))
	{
		*(long*)(&f4->r0) = vcol;
		setPolyF4(f4);
		setSemiTrans(f4, 1);
		addPrim(&o[z],f4);
		f4++;
		curr_pos += seg_offset;
	}

	PLATFORM.DeclareUsedMiscPolyGT4s((POLY_GT4*)f4);
#endif
}

//******************************************************************************************
#endif
