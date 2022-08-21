#include	"Common.h"

#if TARGET == DC
#include	"Globals.h"
//#include    <mwinline.h>
//#include    <gtereg_s.h>
#include    <gtemac.h>
#include	"DCMesh.h"
#include    "DC3dengine.h"
#include	"DCFence.h"
#include	"Globals.h"
#include 	"MainCode.h"
#include	"DCJunk.h"

//******************************************************************************************
#define	PSXFence_DESIRED_SEGMENT_SIZE	4.f
#define	PSXFence_HEIGHT					2.02f

//******************************************************************************************
void	CDCFence::RenderFromData(CThing *t, const SRTInfo &ri)
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

	SINT	num_segs = SINT(dist / PSXFence_DESIRED_SEGMENT_SIZE);
	FVector	seg_offset = (e - s) / float(num_segs);
	FVector  perp = FVector(0, 0, PSXFence_HEIGHT);

	if (ri.i0 == -1)
		return;

	STextureRefOther *tr = ENGINE.GetOtherTexture(ri.i0);

	FVector	curr_pos = s;
	
	SINT	TN = tr->mTPage + 1;
	
	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
	

	for	(c0 = 0; c0 < num_segs; c0++)
	{

		FVector	verts[2][2];
		
		verts[0][0] = curr_pos;
		verts[0][1] = curr_pos - perp;
		
		if (c0 != num_segs - 1)
			curr_pos = s + (seg_offset * float((c0 + 1)));
		else
			curr_pos = e;
			
		verts[1][0] = curr_pos;
		verts[1][1] = curr_pos - perp;
		
		ENGINE.SwapYZ((FVector *)verts, 4);
		
		PersTransWrapper(4,(tvector *)verts, (tvector *)verts, p , ID_FMATRIX);

		if ((verts[0][0].Z > 0.f) || (verts[0][1].Z > 0.f) || (verts[1][0].Z > 0.f) || (verts[1][1].Z > 0.f))
		    continue;

		InitCZTs(TN);

		float	u0 = tr->mX + 0.002f;
		float	u1 = tr->mX + tr->mW - 0.002f;
		float	v0 = tr->mY + 0.002f;
		float	v1 = tr->mY + tr->mH - 0.002f;
		
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
		
	}

    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
}

//******************************************************************************************
#endif
