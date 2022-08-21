#include	"Common.h"

#if TARGET == PSX

#include	"Globals.h"
#include    <mwinline.h>
#include	"PSXMesh.h"
#include    <gtereg_s.h>
#include    "psx3dengine.h"
#include    <gtemac.h>

#define __asm__ asm
//#include <inline_c.h>
//#define __arg0	-1
//#define __arg1	-2
//#define __arg2	-3
//#define __arg3	-4
#ifdef _MAP_OPTIMISED
#pragma optimization_level 4
#endif

#define MESH_GOURAUD_DEPTH
#define PullLong(addr,offs) (*(ULONG*)(((char*)(addr))+(offs)))
#define gte_RotTransPersXYP(r1,r2,r3)            \
				{	gte_ldv0(r1);   	\
                    gte_rtps();         \
                    gte_stsxy(r2);      \
                    gte_stszotz(r3);    }

// Set Global matrix+translate simultaneously from {struct MATRIX Matrix}
#define gte_SetGlobalMatrix( Matrix )  \
   __evaluate (__arg0, (long)(Matrix));\
   __asm_start();\
   __I_lw (12, 0, __arg0);\
   __I_lw (13, 4, __arg0);\
   __I_ctc2 (12, 0);\
   __I_ctc2 (13, 1);\
   __I_lw (12, 8, __arg0);\
   __I_lw (13, 12, __arg0);\
   __I_lw (14, 16, __arg0);\
   __I_ctc2 (12, 2);\
   __I_ctc2 (13, 3);\
   __I_ctc2 (14, 4);\
   __I_lw (12, 20, __arg0);\
   __I_lw (13, 24, __arg0);\
   __I_ctc2 (12, 5);\
   __I_lw (14, 28, __arg0);\
   __I_ctc2 (13, 6);\
   __I_ctc2 (14, 7);\
   __asm_end();


//******************************************************************************************
//******************************************************************************************

//** Render thing given this render info


//VECTOR DisplayAspect={FtoPSX(1.0),FtoPSX(1.0),FtoPSX(0.666)};


#define gte_stlvz( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_swc2 (27, 0, __arg0);\
   __asm_end();

#define gte_stlvzh( r0 ) \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_mfc2 (13,27);\
   __I_nop();\
   __I_sh (13,0,__arg0);\
   __asm_end();


#define gte_SetTransZonly( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_lw(14,0,__arg0);\
   __I_nop();\
   __I_ctc2(14, 7);\
   __asm_end();


#define gte_ldv0scale( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_lh (12,0,__arg0);\
   __I_lh (13,2,__arg0);\
   __I_lw (14,4,__arg0);\
   __I_sll(12,12,2);\
   __I_sll(13,13,18);\
   __I_mtc2(14,1);\
   __I_or(12,12,13);\
   __I_mtc2(12,0);\
   __asm_end();




//SPSXMeshTriangle tribuff[1536];

/*-----------------01/10/1999 11:09-----------------
 * Copies longwords from src to dst for a BYTE length of length
 * --------------------------------------------------*/
extern void asm lcopy(long *src,long *dst,int length);


/*-----------------18/10/1999 12:51-----------------
 * This function calculates (4096/divisor)
 * --------------------------------------------------*/

#define gte_initforratiocalc()\
		__asm_start();\
		__I_addiu(12,0,1);\
		__I_mtc2(0,0);\
		__I_mtc2(0,1);\
		__I_ctc2(12,26);\
		__I_ctc2(12,27);\
		__I_ctc2(0,28);\
		__asm_end();

#define gte_restoreforratiocalc()\
    SetGeomOffset(PLATFORM.ScreenWidth()/2, PLATFORM.ScreenHeight()/2);\
    SetGeomScreen(SCR_Z);\




// Returns the ratio in retval, also sets it up in the GTE (as per gte_lddp(x))
#define gte_ratiocalc(arg1,arg2,retval)\
		__evaluate(__arg0,long(arg1));\
		__evaluate(__arg1,long(arg2));\
		__evaluate(__arg2,long(retval));\
		__asm_start();\
		__I_sub(12,__arg0,__arg1);\
		__I_ctc2(12,7);\
		gte_rtps();\
		__I_mfc2(12,24);\
		__I_mtc2(__arg0,9);\
		__I_mtc2(12,8);\
		gte_gpf0();\
		__I_mfc2(12,25);\
		__I_mtc2(12,8);\
		__I_sra(12,12,4);\
		__I_sh(12,0,__arg2);\
		__asm_end();

inline SWORD GetRatio(SWORD z1,SWORD z2)
{
	//if(z1==z2)return 0;
	SINT retval;
	if(z1>z2)
	{
	 	gte_ratiocalc(z1,z2,&retval);
		return retval;
	}
	else
	{
		gte_ratiocalc(z2,z1,&retval);
		return 4096-retval;
	}


	//if(z1>z2) return (gte_ratiocalc(z1,z2));
	//else return 4096- (gte_ratiocalc(z2,z1));

	//if(z1>z2)return ((z1<<12)/(z1-z2));
	//else return 4096- ((z2<<12)/(z2-z1));

	//if(z1>z2)return gte_4096divide(z1-z2)*z1;
	//else	return	4096-(gte_4096divide(z2-z1)*z2);

}

/*********************************************************************************************************
 * This function does the following:
 *
 * 1. Reads two points
 * 2. Interpolates between them using the ratio value supplied
 * 3. Rotates and translates the resulting point
 * 4. Sets the .pad element to reflect above/below waterline
 *
 * NOTES:
 * 	There is *NO PERSPECTIVE TRANSFORM* performed.
 * 	This function is (by source-reading only) only 3 cycles per point slower than the interpolation itself.
 */
#if 0
void asm InterpolateRotTransPersPoints(SVECTOR *src1,SVECTOR *src2,SVECTOR *dst,ULONG num_points, SINT ratio)//, SLONG zpos)
{
//	lw		v0,GAME.mLevelData.mGVars+WATER_HEIGHT  // V0 is the water height
	//lw		v1,20(sp)                   // Load up the zpos of the object

	lw		t9,16(sp)                   // Read the ratio for storage in the GTE....


	lh		t0,0(a1)
	lh		t1,2(a1)
	sra		t9,t9,4                     // Shift down to correct 1:19:12
	lh		t2,4(a1)                    // T0,T1,T2 are SRC2 points....
	lh		t3,0(a0)
	lh		t4,2(a0)
	lh		t5,4(a0)                    // T3,T4,T5 are SRC1 points....
	sub		t6,t0,t3
	sub		t7,t1,t4
	sub		t8,t2,t5
//	sra		v0,v0,GINT_SHIFT-PSXENGINE_SCALE  // V0 is the water height shifted for PSX
Loopyit:
	mtc2	t9,C2_IR0                   // Setup the ratio register....
	mtc2	t3,C2_MAC1
	mtc2	t4,C2_MAC2
	mtc2	t5,C2_MAC3
	mtc2	t6,C2_IR1
	mtc2	t7,C2_IR2
	mtc2	t8,C2_IR3                   // Setup the interpolation registers
	lh		t0,8(a1)
	GPL(1)
	addi	a0,a0,8                     // Update the SRC1 pointer
	addi	a1,a1,8                     // Update the SRC2 pointer
	addi	a2,a2,8                     // Update the DEST pointer
	addi	a3,a3,-1                    // Decrement point count
   	lh		t1,2(a1)
	//MVMVA(1,0,3,3, 0)                    // Rotate the resulting vector (gte_rt)

	mfc2	t2,C2_MAC1
	mfc2	t3,C2_MAC2
	mfc2	t4,C2_MAC3
	sll		t3,t3,16                    // Get Y into top half
	andi	t2,t2,0xffff                // Mask X into bottom
	or		t2,t2,t3
	mtc2	t2,C2_VXY0
	mtc2	t4,C2_VZ0

	lh		t2,4(a1)                    // T0,T1,T2 are SRC2 points....

	RTPS                                // Rotate/Translate/Perspective resulting vector

	// Use this spare time to read the next point for the GPL instruction
	lh		t3,0(a0)
	lh		t4,2(a0)
	lh		t5,4(a0)                    // T3,T4,T5 are SRC1 points....
	sub		t6,t0,t3
	sub		t7,t1,t4
	sub		t8,t2,t5


	mfc2    t0,C2_SXY2
	mfc2    t1,C2_SZ3

	sw		t0,-8(a2)
	bnez	a3,Loopyit
	sh		t1,-4(a2)
	jr		ra
	nop
}
#else

#ifndef t10
#define	t10 v0
#define t11 v1
#define t12 ra
#define t13 at
#endif

void asm InterpolateRotTransPersPoints(SVECTOR *src1,SVECTOR *src2,SVECTOR *dst,ULONG num_points, SINT ratio)//, SLONG zpos)
{

	lw		t13,16(sp)                   // Read the ratio for storage in the GTE....




	lh		t0,0(a1)
	lh		t1,2(a1)
	lh		t2,4(a1)
	sw		ra,16(sp)

	sra		t13,t13,4                     // Shift down to correct 1:19:12
	andi	t13,t13,0xffff                // Mask to store in Ratio register

	lh		t3,0(a0)
	lh		t4,2(a0)
	lh		t5,4(a0)

	mtc2	t13,C2_IR0                  // Restore ratio after RTPT


	lh		t6,8(a0)
	lh		t7,10(a0)
	lh		t8,12(a0)
	lh		t9,16(a0)
	lh		t10,18(a0)
	lh		t11,20(a0)
	b		In
	nop

Loopyit:
	lh		t0,0(a1)
	lh		t1,2(a1)
	lh		t2,4(a1)

	lh		t3,0(a0)
	lh		t4,2(a0)
	lh		t5,4(a0)
	lh		t6,8(a0)
	lh		t7,10(a0)
	lh		t8,12(a0)
	lh		t9,16(a0)
	lh		t10,18(a0)
	lh		t11,20(a0)


	mfc2	t12,C2_SXY0
	mfc2	t13,C2_SZ1
	mtc2	t13,C2_IR0                  // Restore ratio after RTPT
	sw		t12,0(a2)
	sh		t13,4(a2)
	mfc2	t12,C2_SXY1
	mfc2	t13,C2_SZ2
	sw		t12,8(a2)
	sh		t13,12(a2)
	mfc2	t12,C2_SXY2
	mfc2	t13,C2_SZ3
	sw		t12,16(a2)
	sh		t13,20(a2)
	addi	a2,a2,24                    // Point to next output trio

In:

	sub		t0,t0,t3
	sub		t1,t1,t4
	sub		t2,t2,t5



	mtc2	t0,C2_IR1
	mtc2	t1,C2_IR2
	mtc2	t2,C2_IR3
	mtc2	t3,C2_MAC1
	mtc2	t4,C2_MAC2
	mtc2	t5,C2_MAC3

	lh		t0, 8(a1)
	GPL(1)

	lh		t1,10(a1)
	lh		t2,12(a1)
	sub		t0,t0,t6
	sub		t1,t1,t7
	sub		t2,t2,t8
	mfc2	t3,C2_MAC1
	mfc2	t4,C2_MAC2
	mfc2	t5,C2_MAC3
	mtc2	t0,C2_IR1
	mtc2	t1,C2_IR2
	mtc2	t2,C2_IR3
	mtc2	t6,C2_MAC1
	mtc2	t7,C2_MAC2
	mtc2	t8,C2_MAC3
	lh		t0,16(a1)
	GPL(1)
	lh		t1,18(a1)
	lh		t2,20(a1)
	sub		t0,t0,t9
	sub		t1,t1,t10
	sub		t2,t2,t11
	mfc2	t6,C2_MAC1
	mfc2	t7,C2_MAC2
	mfc2	t8,C2_MAC3
	mtc2	t0,C2_IR1
	mtc2	t1,C2_IR2
	mtc2	t2,C2_IR3
	mtc2	t9,C2_MAC1
	mtc2	t10,C2_MAC2
	mtc2	t11,C2_MAC3
	andi	t3,t3,0xffff
	GPL(1)

	sll		t4,t4,16
	or		t3,t3,t4
	mtc2	t3,C2_VXY0
	mtc2	t5,C2_VZ0

	andi	t6,t6,0xffff

	mfc2	t9,C2_MAC1
	mfc2	t10,C2_MAC2
	mfc2	t11,C2_MAC3

	sll		t7,t7,16
	or		t6,t6,t7
	mtc2	t6,C2_VXY1
	mtc2	t8,C2_VZ1

	andi	t9,t9,0xffff
	sll		t10,t10,16
	or		t9,t9,t10
	mtc2	t9,C2_VXY2
	mtc2	t11,C2_VZ2

	mfc2	t13,C2_IR0                  // Preserve ratio during rtpt


	addi	a3,a3,-3                    // Check for next group of three...


	RTPT                                // RTP the three points in the GTE

	addi	a0,a0,24

	addi	a1,a1,24


	bgtz	a3,Loopyit
	nop


	mfc2	t12,C2_SXY0
	mfc2	t13,C2_IR0                  // Restore ratio after RTPT
	sw		t12,0(a2)
	sh		t13,4(a2)
	mfc2	t12,C2_SXY1
	mfc2	t13,C2_SZ2
	sw		t12,8(a2)
	sh		t13,12(a2)
	mfc2	t12,C2_SXY2
	mfc2	t13,C2_SZ3
	sw		t12,16(a2)
	sh		t13,20(a2)
	addi	a2,a2,24                    // Point to next output trio

	nop

	lw	ra,16(sp)
	nop
	jr		ra
	nop
}


#endif


struct  SDuv{
	UBYTE mU;
	UBYTE mV;
};

struct SSubDivVert {
	UBYTE mU;
	UBYTE mV;
	UWORD mNorm;
	UWORD mVnum;
};


#define gte_SetBackColorRatio( r0 )  \
   __evaluate (__arg0, (long)(r0<<4));\
   __asm_start();\
   __I_ctc2 (__arg0, 13);\
   __I_ctc2 (__arg0, 14);\
   __I_ctc2 (__arg0, 15);\
   __asm_end();


struct SInd {
	SWORD mVind;                        // Vertex index in vertex array...
	SWORD mTind;                        // Vertex index in TRIANGLE DEFINITION...
};
int subdivided_flag;
int vertices_are_world;

#define gte_setir0(x) \
		__asm_start();\
		__evaluate(-1,(long)x);\
		__I_lh(12,0,-1);\
		__I_nop();\
		__I_nop();\
		/*__I_sll(12,12,4);*/\
		__I_mtc2(12,8);\
		__asm_end();



// Load V0 in GTE from column 0 in a matrix...
#define gte_loadv0m(r1)	\
{\
	__evaluate(-1,(long)r1);\
	__I_lhu(13,6,-1);\
	__I_lhu(12,0,-1);\
	__I_lhu(14,12,-1);\
	__I_sll(13,13,16);\
	__I_or(12,13,12);\
	__I_mtc2(12,0);\
	__I_mtc2(14,1);\
}

// Load V1 in GTE from column 1 in a matrix...
#define gte_loadv1m(r1)  \
{\
	__evaluate(-1,(long)r1);\
	__I_lhu(13,8,-1);\
	__I_lhu(12,2,-1);\
	__I_lhu(14,14,-1);\
	__I_sll(13,13,16);\
	__I_or(12,13,12);\
	__I_mtc2(12,2);\
	__I_mtc2(14,3);\
}
// Load V1 in GTE from column 2 in a matrix...
#define gte_loadv2m(r1)   \
{\
	__evaluate(-1,(long)r1);\
	__I_lhu(13,10,-1);\
	__I_lhu(12,4,-1);\
	__I_lhu(14,16,-1);\
	__I_sll(13,13,16);\
	__I_or(12,13,12);\
	__I_mtc2(12,4);\
	__I_mtc2(14,5);\
}


#define gte_MulMatrixQuick(r1,r2,r3)				\
				{	gte_SetRotMatrix(r1);	\
					__asm_start();\
					gte_loadv0m(r2);		\
					__I_nop();\
					__I_word(0x0000013f);\
					gte_loadv1m(r2);\
					gte_stclmv(r3);		\
					__I_word(0x0000017f);\
					gte_loadv2m(r2);\
					gte_stclmv((char*)r3+2);\
					__I_word(0x000001bf);\
					gte_stclmv((char*)r3+4);\
					__asm_end();}


inline void gte_SetLightMatrixQuick(long *where)
{
	asm volatile("	lw		$12,0(%0)
					lw  	$13,4(%0)
					ctc2	$12,$8
					ctc2	$13,$9
					lw  	$12,8(%0)
					lw  	$13,12(%0)
					lw  	$14,16(%0)
					ctc2	$12,$10
					ctc2	$13,$11
					ctc2	$14,$12"::"r"(where):"t4","t5","t6");
}


#define gte_ZeroTrans() {\
		__asm_start();\
		__I_ctc2(0,5);\
		__I_ctc2(0,6);\
		__I_ctc2(0,7);\
		__asm_end();\
}

MATRIX LIGHT_DIR=
{
	{
		{0x1000,0x0000,0x0000},
		{-0x1000,0x0000,0x0000},
		{0x1000,0x0000,0x0000}
	},
	{0,0,0}
};
long maximum_tris=0;
long maximum_normals=0;

void    SPSXMeshPart::Render(IRenderableThing *t, SRTInfo *ri, const GVector &pos, const GMatrix &ori)
{
	//return;
	int subdivided_flag=0;
	int vertices_are_world=0;
	int	subdiv_index=0;


	//********************
	// setup matrices.
	MATRIX  cam;
    MATRIX objmt;
    MATRIX ldir;
	CCamera	*Gcam = SPCamera;

	BOOL	rotate = TRUE;
	if (ri && ri->mWaveResponse == WR_DONT_ROTATE)
		rotate = FALSE;

	if (rotate)
	{
		ENGINE.GMatrixToTransposeMATRIX(&ori,&objmt);
	    gte_MulMatrixQuick(&LIGHT_DIR,&objmt,(MATRIX*)0x1f800000);
	    gte_SetLightMatrixQuick((long*)0x1f800000);
	}
	else
	{
		gte_SetLightMatrixQuick((long *)(&LIGHT_DIR));
	}

	SetBackColor(80,80,80);


	GVector     tpos = pos;

	// We're moving the object relative to the ground, so offset the Zpos...
	if(ri)
	{
		if(ri->mWaveResponse==WR_MOVE)
			tpos.Z=MAP.GetMapHeight(tpos);
	}
	GINT wh = GAME.GetLevelData()->mGVars[WATER_HEIGHT];
	int zpos=(tpos.Z-wh).GetVal()>>(GINT_SHIFT-PSXENGINE_SCALE);


	SetRotMatrix(&SPCameraMATRIX);
	gte_ldv0i(
			   (tpos.X.mVal-Gcam->mPos.X.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE),
			   (tpos.Y.mVal-Gcam->mPos.Y.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE),
			   (tpos.Z.mVal-Gcam->mPos.Z.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE));
	gte_rtv0();



	// JCL temporary #2
	SINT	frame;
	SINT	f1 = 0, f2 = 0;
	SINT	frac = 0;

	if(t)
	{
		SINT	animind = t->GetRealAnimIndex();

		if ((mNumVFrames > 1) && (mBaseMesh->mNumAnimModes) && (animind >= 0))
		{
			GINT			vframe = t->GetRenderFrame();
			CPSXAnimMode	*am   = &mBaseMesh->mAnimModes[animind];
			vframe *= am->mFrameScaler; // this fraction varies for looped and unlooped animations
								   	// see loader code

			SINT	vf1 = (vframe.Whole()+am->mStart) % mNumVFrames;		//### % shouldn't be needed
			SINT	vf2 = vf1 + 1;
			if(vf2 > am->mLast) vf2 = am->mStart;

			// now lookup into the vframes table
			GINT	af1 = mVFrames[vf1];
			GINT	af2 = mVFrames[vf2];

			if (af2 < af1) af2 = G((af1).Whole() + 1);

			GINT	aframe = (af2 * vframe.GFrac()) + (af1 * (G1 - vframe.GFrac()));

			f1 = aframe.Whole();
			f2 = f1 + 1;
			if	(f2 > (mVFrames[am->mLast]).Whole()) f2 = (mVFrames[am->mStart]).Whole();

			frac = aframe.Frac();
		}
	}
	frame = f1;
	
	if ((f1 < 0) || (f2 < 0) ||
		(f1 > 1000) || (f2 > 1000))
	{
		printf("****** Silly frame number: %d, %d\n", f1, f2);
		return;
	}

	// Store result from camera offset inverse transform...
	gte_MACtoTRANS();

	long dpq;

	SetMyDepthQueue();

	__asm_start();

	__evaluate(-1,(long)&dpq);
	__I_mtc2(0,0);
	__I_mtc2(0,1);
//	__I_lui(12,0x009f);
//	__I_addi(12,12,0x9f9f);
	__I_lui(12,0x002f);
	__I_addi(12,12,0x2f2f);
	__I_mtc2(12,6);                     // Setup the RGB values
	gte_rtps();                         // Setup the IR0 register for depthqueueing
	__I_nop();
	__I_nop();
	__I_mfc2(12,8);
	__I_nop();
	__I_nop();
	__I_sw(12,0,-1);
	__asm_end();






	//gte_stlvnl(&cam.t[0]);
	if(dpq<(930<<3))
	{


		// end JCL temporary

		ASSERT(f1 >= 0);
		ASSERT(f2 >= 0);

    	SVECTOR *svecs;// = &mVertices[frame][0].mVertex;

		SVECTOR *vc =(SVECTOR*)POOL->mMeshVertexCache;
		SVECTOR *vn =(SVECTOR*)POOL->mMeshNormalCache;

	#if 1
		// Had to reduce this because of the Scratchpad copies of the main matrices...
		if(mNumVertices<114)
		{
			vc=(SVECTOR*)0x1f800000;

			if(mNumNormals+mNumVertices<114)
			{
				vn=vc+mNumVertices+2;
				//printf("Both fitted!\n");
			}
			//printf("Got <128!\n");
		}

	#endif

		GINT	scale;
		if(t && ((scale = t->GetRenderScale()) != G1))
		{
			int s_z  = 0x20000 - scale.mVal;
			int s_xy = scale.mVal;

			objmt.m[0][0]=MUL64(objmt.m[0][0],s_xy);
			objmt.m[0][1]=MUL64(objmt.m[0][1],s_xy);
			objmt.m[0][2]=MUL64(objmt.m[0][2],s_xy);
			objmt.m[1][0]=MUL64(objmt.m[1][0],s_xy);
			objmt.m[1][1]=MUL64(objmt.m[1][1],s_xy);
			objmt.m[1][2]=MUL64(objmt.m[1][2],s_xy);
			objmt.m[2][0]=MUL64(objmt.m[2][0],s_z);
			objmt.m[2][1]=MUL64(objmt.m[2][1],s_z);
			objmt.m[2][2]=MUL64(objmt.m[2][2],s_z);

		}
		if (rotate)
			SetMulRotMatrix(&objmt);

		if((mNumVFrames==1)||(frac==0))
		{
			//frame = ABS(GAME.GetGameTurn()) % mNumFrames;
    		svecs = &mVertices[frame][0].mVertex;
			TransMultPoints(svecs,vc,mNumVertices);
		}
		else
		{
			// Animating mesh, so do interpolation now...
			ASSERT(mNumVertices > 0);

			InterpolateRotTransPersPoints(&mVertices[f1][0].mVertex,&mVertices[f2][0].mVertex,vc,mNumVertices,frac);//,cam.t[2]);
			svecs=vc;
		}



		//********************
    	// Now Calculate the lighting from the normals (rotated lighting matrix)


		gte_SetBackColorRatio(180);

		svecs=&mNormals[0].mNormal;
		TransMultNormal_Light(svecs,vn,mNumNormals);

		//********************
		// Set the 'pad' element of the SVECTOR struct so the tribuilder knows to darken the vert

		// Now add all the triangles

		POLY_GT3		*p3 = PLATFORM.GetMeshPolyGT3s();
		POLY_GT3		*op3 = p3;

//		GouraudDepth=1536;

		SINT 	skin = -1;
		if (!ri)
			skin = 0;
		else
		{
//			printf("ri->i3: %d, mNumSkins: %d\n", ri->i3, mNumSkins);
			if ((ri->i3 >= 0) && (ri->i3 < mNumSkins))
				skin = ri->i3;
		}

		if (t && (t->GetRenderThingMiscFlags() & RTMF_ONTOP))
		{
			SINT	c0;

			for (c0 = 0; c0 < mNumVertices; c0 ++)
				vc[c0].vz = 1;
		}

		if (t && (t->GetRenderThingMiscFlags() & RTMF_NOLIGHT))
		{
			SINT	c0;
			
			DWORD	col = t->GetRenderColour();
			col &= 0x00ffffff;

			for (c0 = 0; c0 < mNumNormals; c0 ++)
				*((DWORD *)(&vn[c0])) = col;
		}



		if(mNumTriangles>maximum_tris)
		{
			maximum_tris=mNumTriangles;
//			printf("Max Tris: %d\n",maximum_tris);
		}
		if(mNumNormals>maximum_normals)
		{
			maximum_normals=mNumNormals;
//			printf("Max Norms: %d\n",maximum_normals);
		}
		if (skin != -1)
		{
			p3=MakePolysFromList(p3, mGTriangles, vc, (long *)vn,mNumTriangles,
			                    (unsigned long *)PLATFORM.GetOrderTable(), mTTriangles[skin]);//,OffScreenMask,MESH_GOURAUD_DEPTH);
		}
		PLATFORM.DeclareUsedMeshPolyGT3s(p3);
	}
}

//******************************************************************************************
#pragma dont_inline on

void	CPSXMesh::RenderMeshHierarchy(IRenderableThing *t, SRTInfo *ri, GVector p, GMatrix ori, SPSXMeshPart *mp)
{
	if (!mp)
		return;

	// evaluate transforms
	if ((!t) || (t->RenderUseHierarchy(mp->mPartNumber)))
	{
		p  += ori * mp->mOffsetPos;
		ori = ori * mp->mOrientation;
	}
	else
	{
		p += mp->mOffsetPos;
		ori = mp->mOrientation;
	}

	if (t)
	{
//		GVector po = t->GetRenderPos(mp->mPartNumber);
//		p += po;
		p+= t->GetRenderPos(mp->mPartNumber);

//		GMatrix	m = t->GetRenderOrientation(mp->mPartNumber);
//		ori = ori * m;
		ori = ori * t->GetRenderOrientation(mp->mPartNumber);
	}

	// render this part
	if ((!t) || (t->RenderRenderPart(mp->mPartNumber)))
	{
		mp->Render(t, ri, p, ori);
	}

	// and the children
	SINT	c0;

	for (c0 = 0; c0 < mp->mNumChildren; c0 ++)
	{
		RenderMeshHierarchy(t, ri, p, ori, mp->mChildren[c0]);
	}
}

#pragma dont_inline off

//******************************************************************************************
// static access function

int ShadowTexNumber;


void CPSXMesh::RenderAtPos(const GVector &pos,const GMatrix &ori)
{
	GouraudDepth=4096;

	// DAMO! Moved here to speed up the render. Most meshes will have all parts in the same place...
	// JCL - hmmmmmm
	MATRIX 	lcol;

#define tlcol (0x5f<<5)

	lcol.m[0][0] = tlcol * 4;
	lcol.m[1][0] = tlcol * 3;
	lcol.m[2][0] = tlcol;
	lcol.m[0][1] = tlcol;
	lcol.m[1][1] = tlcol * 2;
	lcol.m[2][1] = tlcol * 4;
	lcol.m[0][2] = 8191;
	lcol.m[1][2] = 8191;
	lcol.m[2][2] = 8191;

    SetColorMatrix(&lcol);

	// End JCL


 	RenderMeshHierarchy(NULL,NULL,pos, ori, mParts[0]);


}




void	CPSXMesh::RenderFromData(IRenderableThing *t, SRTInfo *ri, BOOL light)
{
	SINT	 	mesh_no;
	CPSXMesh	*mesh;

	if (!t)
		return;

	// Gouraud depth?
	if (t->GetRenderCanGoFlatShaded())
		GouraudDepth=1536;
	else
		GouraudDepth=4096;

	#if 1
// DAMO! Moved here to speed up the render. Most meshes will have all parts in the same place...
	// JCL - hmmmmmm
	MATRIX 	lcol;
	DWORD	col;

	SINT	tflags = t->GetRenderThingMiscFlags();

	if (tflags & RTMF_WHITE)
	{
		lcol.m[0][0] = 0x7fff;
		lcol.m[1][0] = 0x7fff;
		lcol.m[2][0] = 0x7fff;
		lcol.m[0][1] = 0x7fff;
		lcol.m[1][1] = 0x7fff;
		lcol.m[2][1] = 0x7fff;
		lcol.m[0][2] = 0x7fff;
		lcol.m[1][2] = 0x7fff;
		lcol.m[2][2] = 0x7fff;
	}
	else if (tflags & RTMF_BLACK)
	{
		lcol.m[0][0] = -0x7ffe;
		lcol.m[1][0] = -0x7ffe;
		lcol.m[2][0] = -0x7ffe;
		lcol.m[0][1] = -0x7ffe;
		lcol.m[1][1] = -0x7ffe;
		lcol.m[2][1] = -0x7ffe;
		lcol.m[0][2] = -0x7ffe;
		lcol.m[1][2] = -0x7ffe;
		lcol.m[2][2] = -0x7ffe;
	}
	else	
	{
		if (light)
		{
			col = MAP.GetCell(CCellRef(t->GetRenderPos()))->mColour;
	//		printf("Lighing - col = %X\n", col);
		}
		else
			col = 0x5f5f5f5f;

		SINT	colr = ((col      ) & 0xff) << 5;
		SINT	colg = ((col >>  8) & 0xff) << 5;
		SINT	colb = ((col >> 16) & 0xff) << 5;

		lcol.m[0][0] = colr * 4;
		lcol.m[1][0] = colg * 3;
		lcol.m[2][0] = colb;
		lcol.m[0][1] = colr;
		lcol.m[1][1] = colg * 2;
		lcol.m[2][1] = colb * 4;
		lcol.m[0][2] = 8191;
		lcol.m[1][2] = 8191;
		lcol.m[2][2] = 8191;
	}

//	if (ri->i0 < 10000)			// not in frontend
	    SetColorMatrix(&lcol);
	#endif
	// End JCL


	// render top first?
	mesh_no = ri->i1;
	if (mesh_no >= 0)
	{
		mesh = ENGINE.GetMesh(mesh_no);
		if ((mesh->mNumParts > 0) && (mesh->mParts))
			RenderMeshHierarchy(t, ri, t->GetRenderPos(), t->GetRenderOrientation(1), mesh->mParts[0]);
	}

	// and bottom - bearing in mind that it might be a named mesh
	mesh_no = ri->i0;
	if (mesh_no >= 0)
	{
		// hack for front end
		if (mesh_no >= 10000)
		{
			mesh = ENGINE.GetNumberedMesh(mesh_no - 10000);
		}
		else
			mesh = ENGINE.GetMesh(mesh_no);
	}
	else
	{
		// named mesh - so get the mesh number from the thing.
		mesh_no = -1;

		if (t)
			mesh_no = t->GetRenderMesh();

		if (mesh_no >= 0)
			mesh = ENGINE.GetGlobalMesh(mesh_no);
		else
			mesh = ENGINE.GetDefaultMesh();
	}
	if ((mesh->mNumParts > 0) && (mesh->mParts))
	{
		GMatrix o = ID_GMATRIX;
		GVector p = ZERO_GVECTOR;
		RenderMeshHierarchy(t, ri, p, o, mesh->mParts[0]);
	}

	if(ri->mShadow==ST_SPRITE)
	{
		if (!(COORDS_INVALID(t->GetRenderPos())))
		{
			ShadowTexNumber=ri->i2;                    // Shadow Texture
			MAP.DrawShadow(t);
		}
	}

}

//******************************************************************************************
GINT	CPSXMesh::GetFrameIncrement(EAnimMode a, SINT* realindex)
{
//	printf("Anim Mode sought %d : num anims %d\n", a, mNumAnimModes);

	CPSXAnimMode* am=mAnimModes;
	for(int i=0; i<mNumAnimModes; am++, i++)
	{
//		printf("Anim Mode %d\n", am->mMode);
		if(am->mMode == a)
		{
			*realindex = i; // nasty
			return am->mRate;
		}
	}
	*realindex = -1;
	return G0;
}

//******************************************************************************************
GVector	CPSXMesh::GetEmitter(SINT no, CThing *t)
{
	if ((mEmitters) && (no >= 0) && (no < mNumEmitters))
	{
		// traverse the hierarchy to rotate emitter into position
		SPSXMeshPart	*mp		= mEmitters[no].mMeshPart;
		GVector			pos		= mEmitters[no].mPos;
		GMatrix			ori		= ID_GMATRIX;
		GMatrix			lori	= ID_GMATRIX;

		while (mp)
		{
			lori  = mp->mOrientation;

			if (t)
			{
				GMatrix	m = t->GetRenderOrientation(mp->mPartNumber);
				lori = lori * m;
			}
			pos  = lori * pos;
			pos += mp->mOffsetPos;
			ori = lori * ori;

			if (t && (!t->RenderUseHierarchy(mp->mPartNumber)))
				break; // we're done here

			mp = mp->mParent;
		};

		return pos;
	}
	else
		return ZERO_GVECTOR;
}

//******************************************************************************************
GVector	SVectorToGVector(SVECTOR sv)
{
	GVector v;
	v.X.SetVal(sv.vx << 8);
	v.Y.SetVal(sv.vy << 8);
	v.Z.SetVal(sv.vz << 8);
	return v;
}

GVector	CPSXMesh::GetEmitterAnimated(SINT no, CThing *t)
{
	GVector pos;
	GMatrix ori = ID_GMATRIX;

	if ((mEmitters) && (no >= 0) && (no < mNumEmitters))
	{
		SPSXMeshPart	*mp = mEmitters[no].mMeshPart;

		// first, get the animated position of the vertex

		int		animind;
		if (t &&
			(mp->mNumVFrames >1) &&
			(mp->mBaseMesh->mNumAnimModes)&&
			((animind = t->GetRealAnimIndex())>= 0))
		{
			GINT		vframe = t->GetRenderFrame();
			CPSXAnimMode	*am = &mp->mBaseMesh->mAnimModes[animind];

			vframe *= am->mFrameScaler; // this fraction varies for looped and unlooped animations
									// see loader code

			SINT	vf1 = (vframe.Whole()+am->mStart) % mp->mNumVFrames;		//### % shouldn't be needed
			SINT	vf2 = vf1 + 1;
			if(vf2 > am->mLast) vf2 = am->mStart;

			GINT	af1 = mp->mVFrames[vf1];
			GINT	af2 = mp->mVFrames[vf2];

			if (af2 < af1) af2 = G(af1.Whole() + 1);

			GINT	aframe = (af2 * vframe.GFrac()) + (af1 * (G1 - vframe.GFrac()));

			SINT	f1 = aframe.Whole();
			SINT	f2 = f1 + 1;
			if	(f2 > (mp->mVFrames[am->mLast]).Whole()) f2 = (mp->mVFrames[am->mStart]).Whole();

			SPSXMeshVertex	*pverts1 = mp->mVertices[f1];
			SPSXMeshVertex	*pverts2 = mp->mVertices[f2];

			GINT	frac2 = aframe - G(aframe.Whole());
			GINT	frac1 = G1 - frac2;

			pos = (SVectorToGVector(pverts1[mEmitters[no].mTrackingVertex].mVertex) * frac1) +
			      (SVectorToGVector(pverts2[mEmitters[no].mTrackingVertex].mVertex) * frac2);

			GMatrix			lori	= ID_GMATRIX;

			// now follow down the hierarchy
			while (mp)
			{
				lori  = mp->mOrientation;

				GMatrix	m = t->GetRenderOrientation(mp->mPartNumber);
				lori = lori * m;

				pos  = lori * pos;
				pos += mp->mOffsetPos;

				if ((mp->mParent)) // not at top
				{
					pos += t->GetRenderPos(mp->mPartNumber);
				}

				ori = lori * ori;

				if (!t->RenderUseHierarchy(mp->mPartNumber))
					break; // we're done here

				mp = mp->mParent;
			};

			return pos;
		}
		else
			return GetEmitter(no, t);
	}
	return ZERO_GVECTOR;
}
//******************************************************************************************
void	CPSXMesh::EvaluateCollisionVolume(CThing *t)
{
	if (mParts && mParts[0])
	{
		if (mLastFrameCollisionGenerated != GAME.GetGameTurn())
		{
			mParts[0]->EvaluateCollisionVolumeHierarchy(ZERO_GVECTOR, ID_GMATRIX, t);
			mLastFrameCollisionGenerated = GAME.GetGameTurn();
		}
	}
}

//******************************************************************************************
CCollisionVolume	*CPSXMesh::GetCollisionVolume()
{
	if (mParts && mParts[0])
		return mParts[0]->mCollisionVolume;
	else
		return NULL;
}



//******************************************************************************************
void	SPSXMeshPart::GenerateSimpleBoundingBox()
{
	if (mNumVertices == 0)
	{
		mCollisionVolume = NULL;
		return;
	}

	// get extremities
	GINT	minx = GINT_MAX;
	GINT	miny = GINT_MAX;
	GINT	minz = GINT_MAX;
	GINT	maxx = GINT_MIN;
	GINT	maxy = GINT_MIN;
	GINT	maxz = GINT_MIN;

	GINT	maxdist = G0;

	SINT			c0;
	SPSXMeshVertex *v = mVertices[0];
	GVector			gv;

	for (c0 = 0; c0< mNumVertices; c0++)
	{
		gv.X.SetVal(SINT(v->mVertex.vx) <<8);
		gv.Y.SetVal(SINT(v->mVertex.vy) <<8);
		gv.Z.SetVal(SINT(v->mVertex.vz) <<8);

		if (gv.X < minx)	minx = gv.X;
		if (gv.Y < miny)	miny = gv.Y;
		if (gv.Z < minz)	minz = gv.Z;
		if (gv.X > maxx)	maxx = gv.X;
		if (gv.Y > maxy)	maxy = gv.Y;
		if (gv.Z > maxz)	maxz = gv.Z;

		GINT	d = (gv + mOffsetPos).MagnitudeSq();
		if (d > maxdist)
			maxdist = d;

		v ++;
	}

	GVector gori  = (GVector(minx, miny, minz) + GVector(maxx, maxy, maxz)) * GINT_HALF;
	GVector gaxes = (GVector(minx, miny, minz) - GVector(maxx, maxy, maxz)) * GINT_HALF;
	gaxes.X = GABS(gaxes.X);
	gaxes.Y = GABS(gaxes.Y);
	gaxes.Z = GABS(gaxes.Z);

	mCollisionVolume = (CCollisionVolume *)PLATFORM.GetLevelPoolData(sizeof(CCollisionVolume));
	ASSERT(mCollisionVolume);

	// fill out stuff (can't use the constructor, I think...)
	mCollisionVolume->mType = VT_BOUNDING_BOX;
	mCollisionVolume->mOrigin = gori;
	mCollisionVolume->mAxes = gaxes;
	mCollisionVolume->mOrientation = ID_GMATRIX;
	mCollisionVolume->mTransformedOrigin = gori;
	mCollisionVolume->mTransformedOrientation = ID_GMATRIX;
	mCollisionVolume->mNext = NULL;
	mCollisionVolume->mNextPart = NULL;
	mCollisionVolume->mMeshPart = NULL;

	mRadius = GROOT(maxdist);
}

//******************************************************************************************
void	SPSXMeshPart::EvaluateCollisionVolumeHierarchy(GVector pos, GMatrix ori, CThing *t)
{
	pos += ori * mOffsetPos;
	ori = ori * mOrientation;

	if (t)
	{
		GMatrix	m = t->GetRenderOrientation(mPartNumber);
		ori = ori * m;
	}


	CCollisionVolume *cv = mCollisionVolume;

	while (cv)
	{
		cv->mTransformedOrigin      =  (ori * cv->mOrigin) + pos;
//		cv->mTransformedOrientation = cv->mOrientation * ori;
		cv->mTransformedOrientation = ori;

		cv = cv->mNext;
	}

	SINT	c0;

	for (c0 = 0; c0 < mNumChildren; c0 ++)
		if (mChildren[c0])
			mChildren[c0]->EvaluateCollisionVolumeHierarchy(pos, ori, t);
}

//******************************************************************************************
void	SPSXMeshPart::Read(CMEMBUFFER &mem, CPSXMesh *parent)
{
    int 	i, j;

    // Get vertices
	mem.Read(&mNumVertices, 4);
	mem.Read(&mNumVFrames, 4);
	mem.Read(&mNumAFrames, 4);

	mVFrames = (GINT *)PLATFORM.GetLevelPoolData(sizeof(GINT) * mNumVFrames);
	ASSERT(mVFrames);
	mem.Read(mVFrames, sizeof(GINT) * mNumVFrames);

	mVertices = (SPSXMeshVertex **)PLATFORM.GetLevelPoolData(sizeof(SPSXMeshVertex *) * mNumAFrames);
	ASSERT(mVertices);

	for (i = 0; i < mNumAFrames; i ++)
	{
		mVertices[i] = (SPSXMeshVertex *)PLATFORM.GetLevelPoolData(sizeof(SPSXMeshVertex) * mNumVertices);
		ASSERT(mVertices[i]);
		SPSXMeshVertex*v=mVertices[i];
		for(j=0;j<mNumVertices;j++)
			mem.Read(v++,6);
	}

	// Get Normals
	mem.Read(&mNumNormals, 4);
	mNormals = (SPSXMeshNormal *)PLATFORM.GetLevelPoolData(sizeof(SPSXMeshNormal) * mNumNormals);
	ASSERT(mNormals);
	SPSXMeshNormal*n=mNormals;
	for(int j=0;j<mNumNormals;j++)
			mem.Read(n++,6);

    // Get Triangles
	mem.Read(&mNumTriangles, 4);
	mem.Read(&mNumSkins, 4);

	mGTriangles = (SPSXMeshGTriangle *)PLATFORM.GetLevelPoolData(sizeof(SPSXMeshGTriangle) * mNumTriangles);
	ASSERT(mGTriangles);

    SPSXMeshInputGTriangle	tri;
    for(i = 0; i < mNumTriangles;i ++)
    {
        mem.Read(&tri,sizeof(SPSXMeshInputGTriangle));  // read an input triangle.
        mGTriangles[i].mVerts[0] = (tri.mVert0);// * sizeof(SVECTOR);
        mGTriangles[i].mVerts[1] = (tri.mVert1);// * sizeof(SVECTOR);
        mGTriangles[i].mVerts[2] = (tri.mVert2);// * sizeof(SVECTOR);
        mGTriangles[i].mNorms[0] = (tri.mNorm0) * sizeof(SLONG);
        mGTriangles[i].mNorms[1] = (tri.mNorm1) * sizeof(SLONG);
        mGTriangles[i].mNorms[2] = (tri.mNorm2) * sizeof(SLONG);
    }

	mTTriangles = (SPSXMeshTTriangle **)PLATFORM.GetLevelPoolData(sizeof(SPSXMeshTTriangle *) * mNumSkins);
	ASSERT(mTTriangles);

    for (j = 0; j < mNumSkins; j++)
    {
		mTTriangles[j] = (SPSXMeshTTriangle *)PLATFORM.GetLevelPoolData(sizeof(SPSXMeshTTriangle) * mNumTriangles);
		ASSERT(mTTriangles[j]);

		SPSXMeshInputTTriangle	tri;
	    SPSXMeshTTriangle *tt = mTTriangles[j];
		for(i = 0; i < mNumTriangles;i ++)
		{
		    mem.Read(&tri,sizeof(SPSXMeshInputTTriangle));  // read an input triangle.
		    tt->mU0 = tri.mU0;
		    tt->mV0 = tri.mV0;
		    tt->mU1 = tri.mU1;
		    tt->mV1 = tri.mV1;
		    tt->mU2 = tri.mU2;
		    tt->mV2 = tri.mV2;
		    tt->mTpage = getTPage(tri.mTexBitDepth,
		                        		   tri.mTexAlphaMode,
		                         		   tri.mTPageX,
		                         		   tri.mTPageY);
		    tt->mClut = getClut(tri.mClutX, tri.mClutY);

//		    printf("Read u0: %d \n", tri.mU0);

		    tt ++;
		}
	}

    // OK - now read the rest of the mesh part information

	// part number
    mem.Read(&mPartNumber, 4);

	// children
    mem.Read(&mNumChildren, 4);
	SINT 	c;
	for (i = 0; i < mNumChildren; i ++)
  	{
   		mem.Read(&c, 4);

   		if (c == -1)
   			mChildren[i] = NULL;
   		else
   			mChildren[i] = parent->mParts[c];
   	}

	// parent
	mem.Read(&c, 4);

	if (c == -1)
		mParent = NULL;
	else
		mParent = parent->mParts[c];

	// offsets
	mem.Read(&mOffsetPos  , sizeof(mOffsetPos  ));
	mem.Read(&mOrientation, sizeof(mOrientation));

	mBaseMesh = parent;
}

//******************************************************************************************
void	CPSXMesh::Read(CMEMBUFFER &mem)
{
	mLastFrameCollisionGenerated = -2;

	// first read number of parts
	mem.Read(&mNumParts, 4);

	// and allocate memory
	mParts = (SPSXMeshPart **)PLATFORM.GetLevelPoolData(sizeof(SPSXMeshPart *) * mNumParts);
	ASSERT(mParts);

	SINT	c0;
	for (c0 = 0; c0 < mNumParts; c0 ++)
	{
		mParts[c0] = (SPSXMeshPart *)PLATFORM.GetLevelPoolData(sizeof(SPSXMeshPart));
		ASSERT(mParts[c0]);
	}

	// now read the parts
	for (c0 = 0; c0 < mNumParts; c0 ++)
		mParts[c0]->Read(mem, this);

	// Emitters
	mem.Read(&mNumEmitters, 4);
	if (mNumEmitters > 0)
	{
		mEmitters = (CPSXEmitter *)PLATFORM.GetLevelPoolData(sizeof(CPSXEmitter) * mNumEmitters);
		ASSERT(mEmitters);

		for (c0 = 0; c0 < mNumEmitters; c0 ++)
		{
			SINT	i;
			mem.Read(&i, 4);
			mEmitters[c0].mMeshPart = mParts[i];

			mem.Read(&mEmitters[c0].mPos, sizeof(GVector));

			mem.Read(&mEmitters[c0].mTrackingVertex, 4);
		}
	}
	else
		mEmitters = NULL;

	// Animation Stuff
	mem.Read(&mNumAnimModes, 4);

	if (mNumAnimModes > 0)
	{
		mAnimModes = (CPSXAnimMode *)PLATFORM.GetLevelPoolData(sizeof(CPSXAnimMode) * mNumAnimModes);
		ASSERT(mAnimModes);
		mem.Read(mAnimModes, sizeof(CPSXAnimMode) * mNumAnimModes);
	}
	else
		mAnimModes = 0;

	// Initialise bounding volume;
	// Simple for now - generate an axis aligned bounding box.  Should author these in MAX or something...
	if (mParts)
	{
		SINT	c0;

		for (c0 = 0; c0 < mNumParts; c0 ++)
		{
			if (mParts[c0])
				mParts[c0]->GenerateSimpleBoundingBox();
		}
	}

	// chain up the collision volumes
	if (mParts)
	{
		SINT				c0;
		CCollisionVolume	*cv, *lcv = NULL;

		for (c0 = 0; c0 < mNumParts; c0 ++)
		{
			if (mParts[c0])
			{
				cv = mParts[c0]->mCollisionVolume;

				if (lcv)
					lcv->mNextPart = cv;

				while (cv)
				{
					if (cv->mNext == NULL)
						lcv = cv;
					cv = cv->mNext;
				}
			}
		}
	}

	// radius
	//! this isn't very sensible!
	if (mParts && mParts[0])
		mRadius = mParts[0]->mRadius;

}




#endif
