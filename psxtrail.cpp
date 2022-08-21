#include	"Common.h"

#if TARGET == PSX
#include	"Globals.h"
#include    <mwinline.h>
#include    <gtereg_s.h>
#include    <gtemac.h>
#include	"PSXMesh.h"
#include    "psx3dengine.h"
#include	"PSXTrail.h"
#include	"Globals.h"
#include	"Trail.h"

//******************************************************************************************
#define	PSXTrail_DESIRED_SEGMENT_SIZE	G(4)
#define	PSXTrail_HEIGHT					G(2, 1307)

//******************************************************************************************
void	CPSXTrail::RenderFromData(CThing *t, const SRTInfo &ri)
{
	GMatrix ori = ID_GMATRIX;

	//********************
	// setup matrices.
	MATRIX  cam;
    MATRIX objmt;
	ENGINE.GMatrixToTransposeMATRIX(&ori,&objmt);
	CCamera	*Gcam = ENGINE.GetCamera();
	ENGINE.GMatrixToMATRIX(&Gcam->mOrientation, &cam);
	GMatrix	foo = Gcam->mOrientation.Transpose();

	GVector offpos = -Gcam->mPos;

	cam.t[0] = (((foo.Row[0] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    cam.t[1] = (((foo.Row[1] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    cam.t[2] = (((foo.Row[2] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
	gte_MulMatrix0(&cam,&objmt,&cam);
	SetRotMatrix(&cam);
	SetTransMatrix(&cam);


	SINT c0, c1, c2;


	CTrail	*trail = t->GetRenderTrail();
	if (!trail)
		return;

	SINT	cs = trail->mStartPoint;
	SINT	cmag = 256 / MAX_TRAIL_POINTS;
	SINT	vc = 0;

	NTAG		*o = PLATFORM.GetOrderTable();
    POLY_FT4 	*o4 = (POLY_FT4*)PLATFORM.GetMapPolys();
	SVECTOR vert;

	if (ri.i0 == -1)
		return;

	STextureRefOther *tr = ENGINE.GetOtherTexture(ri.i0);

	for	(c0 = 0; c0 < MAX_TRAIL_POINTS - 1; c0++)
	{
		SINT	ns = (cs + 1) % MAX_TRAIL_POINTS;

		STrailPoint	*p1 = &trail->mPoints[cs];
		STrailPoint	*p2 = &trail->mPoints[ns];
		vc += cmag;

		if ((p1->mActive) && (p2->mActive))
		{
			long z=0,dummy;
			// old verts
			vert.vx=(p1->mStart.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
			vert.vy=(p1->mStart.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
			vert.vz=(p1->mStart.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

			gte_ldv0(&vert);
	        gte_rtps();

			vert.vx=(p1->mEnd.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
			vert.vy=(p1->mEnd.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
			vert.vz=(p1->mEnd.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

			gte_stszotz(&dummy);
			gte_stsxy(&o4->x0);
			z+=dummy;

			gte_ldv0(&vert);
	        gte_rtps();

			vert.vx=(p2->mStart.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
			vert.vy=(p2->mStart.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
			vert.vz=(p2->mStart.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

			gte_stszotz(&dummy);
			gte_stsxy(&o4->x1);
			z+=dummy;

			gte_ldv0(&vert);
	        gte_rtps();

			vert.vx=(p2->mEnd.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
			vert.vy=(p2->mEnd.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
			vert.vz=(p2->mEnd.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

			gte_stszotz(&dummy);
			gte_stsxy(&o4->x2);
			z+=dummy;

			gte_ldv0(&vert);
	        gte_rtps();

			gte_stszotz(&dummy);
			gte_stsxy(&o4->x3);
			z+=dummy;

			z=(z-64)>>4;
			if((z>0)&&(z<850))
			{
				*(long*)(&o4->r0)=0x2e000000 + (vc << 16) + (vc << 8) + vc;

				o4->u0 = tr->mX;
				o4->v0 = tr->mY;
				o4->u1 = tr->mX;
				o4->v1 = tr->mY + tr->mH - 1;
				o4->u2 = tr->mX + tr->mW - 1;
				o4->v2 = tr->mY;
				o4->u3 = tr->mX + tr->mW - 1;
				o4->v3 = tr->mY + tr->mH - 1;


	        	o4->clut  =tr->mCLUT;
	        	o4->tpage = tr->mTPAGE_P1P1;
				setlen(o4,9);
				addPrim(&o[z],o4);
				o4++;
			}
		}
		cs = ns;
	}
	PLATFORM.DeclareUsedMiscPolyGT4s((POLY_GT4*)o4);
}

//******************************************************************************************
#endif
