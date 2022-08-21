#include	"Common.h"

#if TARGET == PSX
#include	"Globals.h"
#include    <mwinline.h>
#include    <gtereg_s.h>
#include    <gtemac.h>
#include	"PSXMesh.h"
#include    "psx3dengine.h"
#include	"PSXLaser.h"
#include	"Globals.h"

//******************************************************************************************
#define	PSXLaser_START_AND_END_SIZE		G1
#define	PSXLaser_DESIRED_SEGMENT_SIZE	G(4)
#define	PSXLaser_HEIGHT					GINT_HALF
#define	PSXLaser_FLARE_SIZE				G(1, 13107)

//******************************************************************************************
void	CPSXLaser::RenderFromData(CThing *t, const SRTInfo &ri)
{
	GMatrix ori = ID_GMATRIX;

	GVector p=t->GetRenderPos();
	GVector s=t->GetRenderStartPos();
	GVector e=t->GetRenderEndPos();
	s -= p;
	e -= p;

	//********************
	// setup matrices.
	//MATRIX  cam;
    //MATRIX objmt;
	//ENGINE.GMatrixToTransposeMATRIX(&ori,&objmt);
	CCamera	*Gcam = ENGINE.GetCamera();  // Still need this later...
	//ENGINE.GMatrixToMATRIX(&Gcam->mOrientation, &cam);
	//GMatrix	foo = Gcam->mOrientation.Transpose();
    //GVector     tpos = p;

	//GVector offpos = tpos-Gcam->mPos;

	//cam.t[0] = (((foo.Row[0] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    //cam.t[1] = (((foo.Row[1] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    //cam.t[2] = (((foo.Row[2] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
	//gte_MulMatrix0(&cam,&objmt,&cam);
	//SetRotMatrix(&cam);
	//SetTransMatrix(&cam);

	CommonOffsetCoord(p.X.mVal,p.Y.mVal,p.Z.mVal);

	SINT c0, c1, c2;

	// work out how many segments we need
	GINT	dist = (e - s).Magnitude();
	SINT	num_segs = ((dist - (PSXLaser_START_AND_END_SIZE * 2)) / PSXLaser_DESIRED_SEGMENT_SIZE).Whole();
	if (num_segs < 1)
		num_segs = 1;
	GVector	seg_offset = (e - s) / (num_segs) * ((dist - (PSXLaser_START_AND_END_SIZE * 2) ) / dist);
	GVector	seg_offsetse = (e - s) * PSXLaser_START_AND_END_SIZE / dist;
	GVector perp = seg_offset ^ (ENGINE.GetCamera()->mOrientation * GVector(G0, G0, G1));
	num_segs += 2;

	if (perp.Magnitude() > G(0, 100))
	{
		perp.Normalise();
		perp *= PSXLaser_HEIGHT / 2;
	}
	else
		perp = GVector(0, 0, PSXLaser_HEIGHT / 2);

	NTAG		*o = PLATFORM.GetOrderTable();
    POLY_FT4 	*o4 = (POLY_FT4*)PLATFORM.GetMapPolys();
	SVECTOR vert;

	if (ri.i0 == -1)
		return;

	STextureRefOther *tr;

	GVector	curr_pos = s;

	CommonSetTrans();
	for	(c0 = 0; c0 < num_segs; c0++)
	{
		if (c0 == 0)
			tr = ENGINE.GetOtherTexture(ri.i1);
		else if (c0 == num_segs - 1)
			tr = ENGINE.GetOtherTexture(ri.i2);
		else
			tr = ENGINE.GetOtherTexture(ri.i0);

		long z=0,dummy;
		// old verts
		gte_ldv0i(
		/*vert.vx=*/(curr_pos.X.GetVal()-perp.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE),//;
		/*vert.vy=*/(curr_pos.Y.GetVal()-perp.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE),//;
		/*vert.vz=*/(curr_pos.Z.GetVal()-perp.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE));

		//gte_ldv0(&vert);
        gte_rtps();

		vert.vx=(curr_pos.X.GetVal()+perp.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.GetVal()+perp.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.GetVal()+perp.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

		gte_stszotz(&dummy);
		gte_stsxy(&o4->x0);
		z+=dummy;

		gte_ldv0(&vert);
        gte_rtps();

		// nev verts
		if (c0 == 0)
			curr_pos = s + seg_offsetse;
		else if (c0 != num_segs - 1)
			curr_pos = s + seg_offsetse + (seg_offset * (c0));
		else
			curr_pos = e;

		vert.vx=(curr_pos.X.GetVal()-perp.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.GetVal()-perp.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.GetVal()-perp.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

		gte_stszotz(&dummy);
		gte_stsxy(&o4->x1);
		z+=dummy;

		gte_ldv0(&vert);
        gte_rtps();

		vert.vx=(curr_pos.X.GetVal()+perp.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.GetVal()+perp.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.GetVal()+perp.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

		gte_stszotz(&dummy);
		gte_stsxy(&o4->x2);
		z+=dummy;

		gte_ldv0(&vert);
        gte_rtps();

		gte_stszotz(&dummy);
		gte_stsxy(&o4->x3);
		z+=dummy;

		z=(z-64)>>4;
    	if ((z >= 0) && (z <  OT_LENGTH))
		{
			*(long*)(&o4->r0)=0x2e7f7f7f;

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

	// Render Decal - this should be fun...

	p = t->GetRenderStartPos();

	GVector		a = p - Gcam->mPos;
	GINT		ad = a.Magnitude();
	p -= a * G(2) / ad;

	//offpos = p - Gcam->mPos;

	//cam.t[0] = (((foo.Row[0] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    //cam.t[1] = (((foo.Row[1] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    //cam.t[2] = (((foo.Row[2] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
	//gte_MulMatrix0(&cam,&objmt,&cam);
	//SetRotMatrix(&cam);
	//SetTransMatrix(&cam);


	CommonOffsetCoord(p.X.mVal,p.Y.mVal,p.Z.mVal);


	GINT	radius = PSXLaser_FLARE_SIZE;

	// OK - now some fucked up hacks to rotate the flare
	GVector b = Gcam->mLookAt - Gcam->mPos;
	GINT	bd = b.Magnitude();
	GINT	nbd = (a * b) / bd;
	b = (b * nbd) / bd ;

	GVector x = b - a;
	x.Normalise();
	GVector y = x ^ b;
	y.Normalise();

	x *=  radius;
	y *= -radius;

	// and render the poly
	tr = ENGINE.GetOtherTexture(ri.i3);


	CommonSetTrans();


	long z=0,dummy;
	GVector tv;

	tv = -x - y;
	vert.vx=(tv.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(tv.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(tv.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

	gte_ldv0(&vert);
    gte_rtps();

	tv = x - y;
	vert.vx=(tv.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(tv.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(tv.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

	gte_stszotz(&dummy);
	gte_stsxy(&o4->x0);
	z+=dummy;

	gte_ldv0(&vert);
    gte_rtps();

	tv = -x + y;
	vert.vx=(tv.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(tv.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(tv.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

	gte_stszotz(&dummy);
	gte_stsxy(&o4->x1);
	z+=dummy;

	gte_ldv0(&vert);
    gte_rtps();

	tv = x + y;
	vert.vx=(tv.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(tv.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(tv.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

	gte_stszotz(&dummy);
	gte_stsxy(&o4->x2);
	z+=dummy;

	gte_ldv0(&vert);
    gte_rtps();

	gte_stszotz(&dummy);
	gte_stsxy(&o4->x3);
	z+=dummy;

	z=(z-64)>>4;
    if ((z >= 0) && (z <  OT_LENGTH))
	{
		*(long*)(&o4->r0)=0x2e7f7f7f;

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
	PLATFORM.DeclareUsedMiscPolyGT4s((POLY_GT4*)o4);

}

//******************************************************************************************
#endif
