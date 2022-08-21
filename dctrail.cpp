#include	"Common.h"

#if TARGET == DC
#include	"Globals.h"
#include	"DCMesh.h"
#include    "DC3dengine.h"
#include	"DCTrail.h"
#include	"Globals.h"
#include	"Trail.h"
#include 	"MainCode.h"
#include	"DCJunk.h"


//******************************************************************************************
#define	PSXTrail_DESIRED_SEGMENT_SIZE	G(4)
#define	PSXTrail_HEIGHT					G(2, 1307)

//******************************************************************************************
void	CDCTrail::RenderFromData(CThing *t, const SRTInfo &ri)
{
	SINT c0, c1, c2;

	CTrail	*trail = t->GetRenderTrail();
	if (!trail)
		return;

	SINT	cs = trail->mStartPoint;
	SINT	cmag = 256 / MAX_TRAIL_POINTS;
	SINT	vc = 0;

	if (ri.i0 == -1)
		return;

	STextureRefOther *tr = ENGINE.GetOtherTexture(ri.i0);

	SINT	TN = tr->mTPage + 1;
	
	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
	

	for	(c0 = 0; c0 < MAX_TRAIL_POINTS - 1; c0++)
	{
		SINT	ns = (cs + 1) % MAX_TRAIL_POINTS;

		STrailPoint	*p1 = &trail->mPoints[cs];
		STrailPoint	*p2 = &trail->mPoints[ns];
		vc += cmag;

		if ((p1->mActive) && (p2->mActive))
		{

			FVector	verts[2][2];
			
			verts[0][0] = GVectorToFVector(p1->mStart);
			verts[0][1] = GVectorToFVector(p1->mEnd);
			verts[1][0] = GVectorToFVector(p2->mStart);
			verts[1][1] = GVectorToFVector(p2->mEnd);
			
			ENGINE.SwapYZ((FVector *)verts, 4);
			
			PersTransWrapper(4,(tvector *)verts, (tvector *)verts, ZERO_FVECTOR , ID_FMATRIX);

			if (!((verts[0][0].Z > 0.f) || (verts[0][1].Z > 0.f) || (verts[1][0].Z > 0.f) || (verts[1][1].Z > 0.f)))
			{
				InitCZTs(TN);
				
				float	u0 = tr->mX + 0.004f;
				float	u1 = tr->mX + tr->mW - 0.004f;
				float	v0 = tr->mY + 0.004f;
				float	v1 = tr->mY + tr->mH - 0.004f;
			
				DWORD	col = (vc << 16) + (vc << 8) + vc;
	
				
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
			}
		}		
		cs = ns;
	}

    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
}

//******************************************************************************************
#endif
