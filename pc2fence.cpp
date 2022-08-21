#include	"Common.h"

#if TARGET == PSX
#include	"Globals.h"
#include    <mwinline.h>
#include    <gtereg_s.h>
#include    <gtemac.h>
#include	"PSXMesh.h"
#include    "psx3dengine.h"
#include	"PSXFence.h"
#include	"Globals.h"

//******************************************************************************************
#define	PSXFence_DESIRED_SEGMENT_SIZE	G(4)
#define	PSXFence_HEIGHT					G(2, 1307)



inline void gte_SetCol(SLONG col)
{
	asm volatile("	mtc2	%0,$6"::"r"(col):);
}

//******************************************************************************************
void	CPSXFence::RenderFromData(CThing *t, const SRTInfo &ri)
{
	GMatrix ori=t->GetRenderOrientation(0);

	GINT	fr	=t->GetRenderTurn();

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
	//CCamera	*Gcam = ENGINE.GetCamera();
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

	SINT	num_segs = (dist / PSXFence_DESIRED_SEGMENT_SIZE).Whole();
	GVector	seg_offset = (e - s) / (num_segs);
	GVector perp = GVector(G0, G0, PSXFence_HEIGHT);

	NTAG		*o = PLATFORM.GetOrderTable();
    POLY_GT4 	*o4 = (POLY_GT4*)PLATFORM.GetMapPolys();
	SVECTOR vert;

	if (ri.i0 == -1)
		return;

	CommonSetTrans();

	STextureRefOther *tr = ENGINE.GetOtherTexture(ri.i0);

	GVector	curr_pos = s;

	gte_SetCol(0x3e7f7f7f);              // Setup for the DPCS...
	SetMyDepthQueue();
	for	(c0 = 0; c0 < num_segs; c0++)
	{
		long z=0,dummy;
		// old verts
		vert.vx=(curr_pos.X.GetVal()-perp.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.GetVal()-perp.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.GetVal()-perp.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

		gte_ldv0(&vert);
        gte_rtps();

		vert.vx=(curr_pos.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

		//gte_stszotz(&dummy);
		gte_stsxy(&o4->x0);
		gte_dpcs();

		z=gte_Get_szotz();
		//z+=dummy;

		gte_strgb(&o4->r0);
		gte_ldv0(&vert);
        gte_rtps();

		// nev verts
		if (c0 != num_segs - 1)
			curr_pos = s + (seg_offset * (c0 + 1));
		else
			curr_pos = e;

		vert.vx=(curr_pos.X.GetVal()-perp.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.GetVal()-perp.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.GetVal()-perp.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

		//gte_stszotz(&dummy);
		gte_stsxy(&o4->x1);
		gte_dpcs();
		//z+=dummy;
		z+=gte_Get_szotz();
		gte_strgb(&o4->r1);

		gte_ldv0(&vert);
        gte_rtps();

		vert.vx=(curr_pos.X.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vy=(curr_pos.Y.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);
		vert.vz=(curr_pos.Z.GetVal())>>(GINT_SHIFT-PSXENGINE_SCALE);

		//gte_stszotz(&dummy);
		gte_stsxy(&o4->x2);
		gte_dpcs();
		//z+=dummy;
		z+=gte_Get_szotz();
		gte_strgb(&o4->r2);

		gte_ldv0(&vert);
        gte_rtps();

		//gte_stszotz(&dummy);
		gte_stsxy(&o4->x3);
		gte_dpcs();
		//z+=dummy;
		z+=gte_Get_szotz();
		gte_strgb(&o4->r3);

		z=(z-64)>>4;
        if (((z >= 0) && (z <  OT_LENGTH))&&(!(*(ULONG*)&o4->x0&*(ULONG*)&o4->x1&*(ULONG*)&o4->x2&*(ULONG*)&o4->x3&OffScreenMask)))
		{
			//*(long*)(&o4->r0)=0x2e7f7f7f;

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
			setlen(o4,12);
			addPrim(&o[z],o4);
			o4++;
		}
	}
	PLATFORM.DeclareUsedMiscPolyGT4s((POLY_GT4*)o4);
}

//******************************************************************************************
#endif
