#include	"Common.h"

#if TARGET == PSX
#include	"Globals.h"
#include    <mwinline.h>
#include    <gtereg_s.h>
#include    <gtemac.h>
#include	"PSXMesh.h"
#include    "psx3dengine.h"
#include	"PSXLightning.h"
#include	"Globals.h"




void asm InterpolatePoints(SVECTOR *src1,SVECTOR *src2,SVECTOR *dst,ULONG num_points, SINT ratio)
{
	lw		t9,16(sp)                   // Read the ratio for storage in the GTE....
	lh		t0,0(a1)
	lh		t1,2(a1)
	sra		t9,t9,4                     // Shift down to correct 1:19:12
	lh		t2,4(a1)                    // T0,T1,T2 are SRC2 points....
	mtc2	t9,C2_IR0                   // Setup the ratio register....
Loopy:
	lh		t3,0(a0)
	lh		t4,2(a0)
	lh		t5,4(a0)                    // T3,T4,T5 are SRC1 points....
	sub		t6,t0,t3
	sub		t7,t1,t4
	sub		t8,t2,t5
	mtc2	t6,C2_IR1
	mtc2	t7,C2_IR2
	mtc2	t8,C2_IR3                   // Setup the interpolation registers
	lh		t0,8(a1)
	addi	a1,a1,8                     // Update the SRC2 pointer
	GPF(1)
	addi	a2,a2,8                     // Update the DEST pointer
	addi	a0,a0,8                     // Update the SRC1 pointer
	addi	a3,a3,-1                    // Decrement point count
	lh		t1,2(a1)
	lh		t2,4(a1)                    // T0,T1,T2 are SRC2 points....
	mfc2	t6,C2_IR1
	mfc2	t7,C2_IR2
	mfc2	t8,C2_IR3                   // Retrieve the interpolation registers
	add		t3,t3,t6
	add		t4,t4,t7
	add		t5,t5,t8
	sh		t3,-8(a2)
	sh		t4,-6(a2)
	bnez	a3,Loopy
	sh		t5,-4(a2)
	jr		ra
	nop
}

 // 32bit signed * 8bit signed => 32bit signed
SINT asm TestMult(SINT value,SINT ratio)
{
	sra		a2,a0,16                    // Get whole part into a2
	andi	a0,a0,0xffff
	mtc2	a0,C2_IR1
	mtc2	a2,C2_IR2
	mtc2	a1,C2_IR0
	nop
	nop
	GPF(0)
	mfc2	v0,C2_MAC2
	mfc2	v1,C2_MAC1
	sll		v0,v0,16
	jr	ra
	add		v0,v0,v1
	nop
}



//******************************************************************************************
#define	PSXLIGHTNING_DESIRED_SEGMENT_SIZE	G(1)
#define	PSXLIGHTNING_WIDTH					G(0,3276)
#define	PSXLIGHTNING_OFFSET					6553
#define	PSXLIGHTNING_NUM_STREAMS			3
//******************************************************************************************
void	CPSXLightning::RenderFromData(CThing *t, const SRTInfo &ri)
{
	GMatrix ori=t->GetRenderOrientation(0);

	GINT	fr	=t->GetRenderTurn();

	GVector p=t->GetRenderPos();
	GVector s=t->GetRenderStartPos();
	GVector e=t->GetRenderEndPos();
	s += - p;
	e += - p;
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

	SINT	num_segs = (dist / PSXLIGHTNING_DESIRED_SEGMENT_SIZE).Whole();

//	float	seg_size = dist / float(num_segs);
	GVector	seg_offset = (e - s) / (num_segs);
	GVector	perp1 = (seg_offset ^ GVector(G(0), G(0), G(1)));
	GVector perp = perp1.NormaliseAndScale(PSXLIGHTNING_WIDTH);

	srand(ENGINE.GetRenderGameTurn()); // stays constant on pause...


	NTAG		*o = PLATFORM.GetOrderTable();
    POLY_FT4 	*o4 = (POLY_FT4*)PLATFORM.GetMapPolys();
	SVECTOR vert;
	STextureRefOther *tr=ENGINE.GetNumberedTexture(TN_BEAM1);

	CommonSetTrans();

	for (c1 = 0; c1 < PSXLIGHTNING_NUM_STREAMS; c1++)
	{
		GVector	curr_pos = s;

		for	(c0 = 0; c0 < num_segs; c0++)
		{
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
			if (c0 != num_segs - 1)
			{
				curr_pos = s + (seg_offset * (c0 + 1));
				// add random offset
				GVector Gr;
				Gr.X.SetVal(((rand()-16384)*PSXLIGHTNING_OFFSET)>>12);
				Gr.Y.SetVal(((rand()-16384)*PSXLIGHTNING_OFFSET)>>12);
				Gr.Z.SetVal(((rand()-32768)*PSXLIGHTNING_OFFSET)>>12);
				curr_pos += Gr;
			}
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
			if((z>0)&&(z<=850))
			{
				*(long*)(&o4->r0)=0x2e806060;
				int val=((tr->mX))+(((tr->mY+30+(rand()&31))<<8));
				*((UWORD *)(&(o4->u0)))  = val;
				*((UWORD *)(&(o4->u1)))  = val+0x003f;
				*((UWORD *)(&(o4->u2)))  = val+0x0100;
				*((UWORD *)(&(o4->u3)))  = val+0x013f;
				short clut=tr->mCLUT;
				short tpage=tr->mTPAGE_P1P1;

				//o4->clut  =tr->mCLUT;
            	//o4->tpage = tr->mTPAGE_P1P1;
				setlen(o4,9);
				addPrim(&o[z],o4);

				o4->clut=clut;
				o4->tpage=tpage;

				o4++;
			}
		}
	}
	PLATFORM.DeclareUsedMiscPolyGT4s((POLY_GT4*)o4);
}

//******************************************************************************************
#endif
