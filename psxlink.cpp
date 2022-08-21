#include	"Common.h"

#if TARGET == PSX
#include	"Globals.h"
#include    <mwinline.h>
#include    <gtereg_s.h>
#include    <gtemac.h>
#include	"PSXMesh.h"
#include    "psx3dengine.h"
#include	"PSXLink.h"
#include	"Globals.h"

//******************************************************************************************
#define PSXLINK_NUM_SEGS	4
#define PSXLINK_SEG_SIZE 	G(0, 10000)

//******************************************************************************************
void	CPSXLink::RenderFromData(CThing *t, const SRTInfo &ri)
{
	GMatrix ori = ID_GMATRIX;

	GVector p=t->GetRenderPos();
	GVector s=t->GetRenderStartPos();
	GVector e=t->GetRenderEndPos();
	s -= p;
	e -= p;

	//********************
	// setup matrices.
	MATRIX  cam;
    MATRIX objmt;
	ENGINE.GMatrixToTransposeMATRIX(&ori,&objmt);
	CCamera	*Gcam = ENGINE.GetCamera();
	ENGINE.GMatrixToMATRIX(&Gcam->mOrientation, &cam);
	GMatrix	foo = Gcam->mOrientation.Transpose();

	//GVector     tpos = p;

	//GVector offpos = tpos-Gcam->mPos;

	//cam.t[0] = (((foo.Row[0] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    //cam.t[1] = (((foo.Row[1] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    //cam.t[2] = (((foo.Row[2] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);


	SetRotMatrix(&cam);
	gte_ldv0i(
			   (p.X.mVal-Gcam->mPos.X.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE),
			   (p.Y.mVal-Gcam->mPos.Y.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE),
			   (p.Z.mVal-Gcam->mPos.Z.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE));
	gte_rtv0();
	// Store result from camera offset inverse transform...
	gte_MACtoTRANS();
	//gte_stlvnl(&cam.t[0]);








	gte_MulMatrix0(&cam,&objmt,&cam);
	SetRotMatrix(&cam);
	//SetTransMatrix(&cam);


	SINT c0, c1, c2;
	ori = t->GetRenderOrientation(1);


	// work out how many segments we need
	GVector	seg_offset = (e - s) / PSXLINK_NUM_SEGS;
	GVector	x = ori * GVector(PSXLINK_SEG_SIZE, G0, G0);
	GVector	y = ori * GVector(G0, PSXLINK_SEG_SIZE, G0);

	NTAG		*o = PLATFORM.GetOrderTable();
    POLY_FT4 	*o4 = (POLY_FT4*)PLATFORM.GetMapPolys();
	SVECTOR vert;

	if (ri.i0 == -1)
		return;

	STextureRefOther *tr = ENGINE.GetOtherTexture(ri.i0);

	SINT	col = t->GetLife();
	if (col < 0)
		col = 0;
	if (col > 100)
		col = 100;
	
	col <<= 1;

	DWORD	vcol = (0x2e << 24) + (col << 8) + (200 - col); // 2e = code


	GVector	curr_pos = s;

	for	(c0 = 0; c0 < PSXLINK_NUM_SEGS; c0++)
	{
		long z=0, dummy;

		vert.vx=(curr_pos.X.mVal - x.X.mVal - y.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.mVal - x.Y.mVal - y.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.mVal - x.Z.mVal - y.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

		gte_ldv0(&vert);
        gte_rtps();

		vert.vx=(curr_pos.X.mVal + x.X.mVal - y.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.mVal + x.Y.mVal - y.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.mVal + x.Z.mVal - y.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

		gte_stszotz(&dummy);
		gte_stsxy(&o4->x0);
		z+=dummy;

		gte_ldv0(&vert);
        gte_rtps();

		vert.vx=(curr_pos.X.mVal - x.X.mVal + y.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.mVal - x.Y.mVal + y.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.mVal - x.Z.mVal + y.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

		gte_stszotz(&dummy);
		gte_stsxy(&o4->x1);
		z+=dummy;

		gte_ldv0(&vert);
        gte_rtps();

		vert.vx=(curr_pos.X.mVal + x.X.mVal + y.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.mVal + x.Y.mVal + y.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.mVal + x.Z.mVal + y.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

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
			*(long*)(&o4->r0) = vcol;

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
			curr_pos += seg_offset;
		}
	}
	
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

	gte_ldv0(&vert);
    gte_rtps();

	vert.vx=(s.X.mVal + perp.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(s.Y.mVal + perp.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(s.Z.mVal + perp.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

	gte_stszotz(&dummy);
	gte_stsxy(&f4->x0);
	z+=dummy;

	gte_ldv0(&vert);
    gte_rtps();

	vert.vx=(e.X.mVal - perp.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(e.Y.mVal - perp.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(e.Z.mVal - perp.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

	gte_stszotz(&dummy);
	gte_stsxy(&f4->x1);
	z+=dummy;

	gte_ldv0(&vert);
    gte_rtps();

	vert.vx=(e.X.mVal + perp.X.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vy=(e.Y.mVal + perp.Y.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);
	vert.vz=(e.Z.mVal + perp.Z.mVal)>>(GINT_SHIFT-PSXENGINE_SCALE);

	gte_stszotz(&dummy);
	gte_stsxy(&f4->x2);
	z+=dummy;

	gte_ldv0(&vert);
    gte_rtps();

	gte_stszotz(&dummy);
	gte_stsxy(&f4->x3);
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

}

//******************************************************************************************
#endif
