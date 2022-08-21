#include	"Common.h"

#if TARGET == PSX

#include	"PSXDecal.h"
#include	"Globals.h"
#include "mwinline.h"
#include <gtemac.h>
#include "psx3dengine.h"
#include "character.h"



extern long Interpolcol (unsigned long *col,short mult);

void CommonOffsetCoord(int x,int y,int z)
{
	CCamera*	Gcam=SPCamera;
	// This flag is set during a particle system processing, since ALL the particles
	//	use the same camera matrix... (Saves quite a bit of loading)
	if(!CameraAlreadyLoaded)SetRotMatrix(&SPCameraMATRIX);
	gte_ldv0i(
			   (x-Gcam->mPos.X.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE),
			   (y-Gcam->mPos.Y.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE),
			   (z-Gcam->mPos.Z.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE));
	gte_rtv0();
}




void SetParticleCommonOffset(CParticle* p,CThing*mOwner)
{
	if((!p->mIsRelative)||(!mOwner))
		CommonOffsetCoord(p->mPos.X.mVal,p->mPos.Y.mVal,p->mPos.Z.mVal);
	else
	{
		/*
		 * Moves relative to the owner, so this particles position is an offset...
		 * This should NEVER occur if the owner has died....
		 */
		GVector tpos=mOwner->GetPos();
		//if(!mOwner)printf("This particle says it's relative, but it's owner is NOT VALID!!!\n");
		CommonOffsetCoord(p->mPos.X.mVal+tpos.X.mVal,p->mPos.Y.mVal+tpos.Y.mVal,p->mPos.Z.mVal+tpos.Z.mVal);
	}
}



#define RING_SEGS p->uPRing.mSegs
#define RING_STEP p->uPRing.mStep
#define RING_WIDTH p->uPRing.mWidth

// To render RINGS, use a height of 0, for TUBES, pass the HEIGHT of the tube....

// Can move relative to an owner, so need the Thing ptr too






inline ULONG dointerpxylong(SVECTOR *a,SVECTOR *b)
{
	int x=((a->vx+b->vx)>>1);
	int y=((a->vy+b->vy)>>1);
	ULONG retval=(ULONG)((y<<16)|(x&0xffff));

	//printf("A: 0x%08x B: 0x%08x  Ret: 0x%08x\n",*(ULONG*)a,*(ULONG*)b,retval);

	return retval;
}






void	ParticleRingRender(CParticle *p,CThing *mOwner)
{
	//return;


	long *cols=(long*)(0x1f800000);
	SVECTOR *vecs=(SVECTOR*)((0x1f800000)+RING_SEGS*16);  // Leave space for the colours
	long	width = ((p->mRadius));
	long width2 = (((p->mRadius))+RING_WIDTH);
	//printf("Col: 0x%08x    Bright: 0x%08x\n",p->mColour,p->uPRing.mBrightness);
	unsigned long col=p->mColour&0xffffff;
	col=Interpolcol(&col,p->uPRing.mBrightness);
	gte_ldrgb(&col);                    // Setup the passed colour for depthqueue
	SetMyDepthQueue();

	NTAG		*o = PLATFORM.GetOrderTable();


	SetParticleCommonOffset(p,mOwner);



	// Create the points for the ring...
	for(int sc=0;sc<RING_SEGS;sc++)
	{
		SWORD segsin=csin(sc*RING_STEP+p->uPRing.mOffs);
		SWORD segcos=ccos(sc*RING_STEP+p->uPRing.mOffs);
		vecs[sc].vx=(segsin*width)>>12;
		vecs[sc].vy=(segcos*width)>>12;
		vecs[sc].vz=0;
		if(p->uPRing.mHeight)
			vecs[sc+(RING_SEGS)].vz=-p->uPRing.mHeight;
		else
			vecs[sc+(RING_SEGS)].vz=0;
		vecs[sc+(RING_SEGS)].vx=(segsin*width2)>>12;
		vecs[sc+(RING_SEGS)].vy=(segcos*width2)>>12;

	}

	CommonSetTrans();


	register SINT poly_type;
	if(!p->uPRing.mTextureNum)
	{
			if(p->mColour>>24)
				poly_type=0x38000000;
			else
				poly_type=0x3a000000;
	}
	else
	{
		if(p->mColour>>24)
			poly_type=0x3c000000;
		else
			poly_type=0x3e000000;
	}


	SINT tilevert=p->uPRing.mTextureNum&PART_TILEV;
	for(int sc=0;sc<RING_SEGS;sc++)
	{
		register long temp,temp2;
		gte_ldv0(&vecs[sc]);
		gte_rtps();
		gte_stsxy(&vecs[sc]);
		gte_stszotz(&vecs[sc].vz);
		gte_dpcs();
		gte_ldv0(&vecs[sc+(RING_SEGS)]);
		asm volatile("	mfc2 %0,$22
   						.word 0x0000007f
						lui	%1,0x007f
						ori %1,%1,0x7f7f
						or	%0,%0,%3
						sw	%0,0(%2)
						srl	%0,%0,1
						and	%0,%0,%1
						or	%0,%0,%3
						sw	%0,0(%4)"

						: "=r"(temp),"=r"(temp2)
						: "r"(&cols[sc]),"r"(poly_type),"r"(&cols[sc+RING_SEGS])
						:"memory");
		// RTPS triggered above (.word 0x0000007f)
		//gte_rtps();
		gte_stsxy(&vecs[sc+RING_SEGS]);
		gte_stszotz(&vecs[sc+RING_SEGS].vz);
	}






	if(!p->uPRing.mTextureNum)
	{
		POLY_G4*g4=PLATFORM.GetMiscPolyG4s();


		SINT adder=63/RING_SEGS;
		for(int sc=0;sc<RING_SEGS;sc++)
		{
			int p1=(sc+1);
			if(p1==RING_SEGS)p1=0;
			int z;

			if(p->uPRing.mHeight)
			{
				z=((vecs[sc].vz)>>2)-16;
				*(long*)(&g4->x0)=*(long*)(&vecs[sc]);
				*(long*)(&g4->x1)=*(long*)(&vecs[p1]);
				*(long*)(&g4->x2)=*(long*)(&vecs[sc+(RING_SEGS)]);
				*(long*)(&g4->x3)=*(long*)(&vecs[p1+(RING_SEGS)]);

			}
			else
			{
				z=((vecs[sc].vz+vecs[p1].vz)>>3)-16;
				*(long*)(&g4->x2)=*(long*)(&vecs[sc]);
				*(long*)(&g4->x3)=*(long*)(&vecs[p1]);
				*(long*)(&g4->x0)=*(long*)(&vecs[sc+(RING_SEGS)]);
				*(long*)(&g4->x1)=*(long*)(&vecs[p1+(RING_SEGS)]);
			}

			if ((z >= 0) && (z <  OT_LENGTH))
			{
				*(long*)(&g4->r0)=cols[sc];
				*(long*)(&g4->r1)=cols[p1];
				*(long*)(&g4->r2)=
				*(long*)(&g4->r3)=0;
				NTAG_addPrimSize(&o[z], (long *)g4,8);
				//addPrim(&o[z], g4);
			}
			g4++;
		}
		PLATFORM.DeclareUsedMiscPolyG4s(g4);
	}
#if 1
	else
	{





		POLY_GT4*gt4=(POLY_GT4*)PLATFORM.GetMiscPolyG4s();
		SINT adder=63/RING_SEGS;

		SINT tnum=p->uPRing.mTextureNum;
		STextureRefOther *tr=ENGINE.GetParticleTexture(tnum&0x3ff);
		for(int sc=0;sc<RING_SEGS;sc++)
		{
			int p1=(sc+1);
			if(p1==RING_SEGS)p1=0;
			int z;
			ULONG *x0,*x1,*x2,*x3;

			if(p->uPRing.mHeight)
			{
				z=((vecs[sc].vz)>>2)-16;
				x0=(ULONG*)(&vecs[sc]);
				x1=(ULONG*)(&vecs[p1]);
				x2=(ULONG*)(&vecs[sc+RING_SEGS]);
				x3=(ULONG*)(&vecs[p1+RING_SEGS]);

			}
			else
			{
			    z=((vecs[sc].vz+vecs[p1].vz)>>3)-16;
				x2=(ULONG*)(&vecs[sc]);
				x3=(ULONG*)(&vecs[p1]);
				x0=(ULONG*)(&vecs[sc+RING_SEGS]);
				x1=(ULONG*)(&vecs[p1+RING_SEGS]);
			}
			if ((z >= 0) && (z <  OT_LENGTH))
			{
				if(tilevert)
				{
					ULONG xy1=dointerpxylong((SVECTOR*)x0,(SVECTOR*)x2);
					ULONG xy2=dointerpxylong((SVECTOR*)x1,(SVECTOR*)x3);
					*(ULONG*)(&gt4->x0)=*x0;
					*(ULONG*)(&gt4->x1)=*x1;
					*(ULONG*)(&gt4->x2)=xy1;
					*(ULONG*)(&gt4->x3)=xy2;
					*(ULONG*)(&gt4->r0)=cols[sc];
					*(ULONG*)(&gt4->r1)=cols[p1];
					*(ULONG*)(&gt4->r2)=cols[sc+RING_SEGS];
					*(ULONG*)(&gt4->r3)=cols[p1+RING_SEGS];
            		gt4->tpage = tr->mTPAGE_P1P1;
            		gt4->clut  =tr->mCLUT;
					if(tnum&PART_TILEH)
						setUVWH(gt4,tr->mX, tr->mY ,tr->mW -1, tr->mH -1);
					else
						setUVWH(gt4,tr->mX + (sc*adder), tr->mY ,adder, tr->mH);
					NTAG_addPrimSize(&o[z], (long *)gt4,12);
					gt4++;

					*(ULONG*)(&gt4->x0)=xy1;
					*(ULONG*)(&gt4->x1)=xy2;
					*(ULONG*)(&gt4->x2)=*x2;
					*(ULONG*)(&gt4->x3)=*x3;
					*(ULONG*)(&gt4->r0)=cols[sc+RING_SEGS];
					*(ULONG*)(&gt4->r1)=cols[p1+RING_SEGS];
					*(ULONG*)(&gt4->r2)=
					*(ULONG*)(&gt4->r3)=0;
            		gt4->tpage = tr->mTPAGE_P1P1;
            		gt4->clut  =tr->mCLUT;
					if(tnum&PART_TILEH)
						setUVWH(gt4,tr->mX , tr->mY ,tr->mW-1, tr->mH-1);
					else
						setUVWH(gt4,tr->mX + (sc*adder), tr->mY ,adder, tr->mH);
					NTAG_addPrimSize(&o[z], (long *)gt4,12);
					gt4++;


				}
				else
				{
					*(ULONG*)(&gt4->x0)=*x0;
					*(ULONG*)(&gt4->x1)=*x1;
					*(ULONG*)(&gt4->x2)=*x2;
					*(ULONG*)(&gt4->x3)=*x3;
					*(ULONG*)(&gt4->r0)=cols[sc];
					*(ULONG*)(&gt4->r1)=cols[p1];
					*(ULONG*)(&gt4->r2)=
					*(ULONG*)(&gt4->r3)=0;

            		gt4->tpage = tr->mTPAGE_P1P1;
            		gt4->clut  =tr->mCLUT;
					if(tnum&PART_TILEH)
						setUVWH(gt4,tr->mX, tr->mY ,tr->mW-1, tr->mH-1);
					else
						setUVWH(gt4,tr->mX + (sc*adder), tr->mY ,adder, tr->mH);
					NTAG_addPrimSize(&o[z], (long *)gt4,12);
					//addPrim(&o[z], gt4);
					gt4++;
				}
			}
		}
		PLATFORM.DeclareUsedMiscPolyGT4s(gt4);

	}
#endif
}

void	ParticleRayRender(CParticle *p)
{
	long *cols=(long*)(0x1f800000);
	SVECTOR *vecs=(SVECTOR*)(0x1f800000)+RING_SEGS;  // Leave space for the colours
	long	width = ((100-p->mLife)<<4);
	long width2 = (((100-p->mLife)<<4)+p->uPRay.mLength);
	SINT offs=-ENGINE.GetRenderGameTurn()<<5;
	//long ccol=0x00ffffff;
	long col=Interpolcol(&p->mColour,p->uPRing.mBrightness);

	//long ccol=0x00ffffff;
	gte_ldrgb(&col);                    // Setup the passed colour for depthqueue
	SetMyDepthQueue();

	NTAG		*o = PLATFORM.GetOrderTable();
	POLY_G3		*g3 =(POLY_G3*) PLATFORM.GetMiscPolyG4s();



	CommonOffsetCoord(p->mPos.X.mVal,p->mPos.Y.mVal,p->mPos.Z.mVal);


	// Create the points for the ring...
	for(int sc=0;sc<RING_SEGS;sc++)
	{
		SWORD segsin=csin(sc*RING_STEP+offs);
		SWORD segcos=ccos(sc*RING_STEP+offs);
		vecs[sc].vx=(segsin*width)>>12;
		vecs[sc].vy=(segcos*width)>>12;
		vecs[sc].vz=0;
			vecs[sc+RING_SEGS].vx=(segsin*width2)>>12;
			vecs[sc+RING_SEGS].vy=(segcos*width2)>>12;
			vecs[sc+RING_SEGS].vz=-p->uPRay.mHeight;
	}

	CommonSetTrans();


	for(int sc=0;sc<RING_SEGS;sc++)
	{
		gte_ldv0(&vecs[sc]);
		gte_rtps();
		gte_stsxy(&vecs[sc]);
		gte_stszotz(&vecs[sc].vz);
		gte_dpcs();
		gte_ldv0(&vecs[sc+RING_SEGS]);
		__evaluate (__arg0, (long)(&cols[sc]));
   		__asm_start();
   		__I_mfc2	(12,22);
   		__I_lui	(13,0x3200);            // Semi-trans G4
   		__I_or	(12,12,13);
   		__I_sw	(12, 0, __arg0);
   		__asm_end();
		gte_rtps();
		gte_stsxy(&vecs[sc+RING_SEGS]);
		gte_stszotz(&vecs[sc+RING_SEGS].vz);
	}

	for(int sc=0;sc<RING_SEGS;sc++)
	{
		int p1=(sc+1);
		if(p1==RING_SEGS)p1=0;
		{
			*(long*)(&g3->r0)=cols[sc];
			*(long*)(&g3->r1)=cols[p1];
			*(long*)(&g3->r2)=0;
			setlen(g3,6);
			int z=(vecs[sc].vz+vecs[p1].vz)>>3;
			*(long*)(&g3->x0)=*(long*)(&vecs[sc]);
			*(long*)(&g3->x1)=*(long*)(&vecs[p1]);
			g3->x2=((vecs[sc+RING_SEGS].vx+vecs[p1+RING_SEGS].vx)>>1);
			g3->y2=((vecs[sc+RING_SEGS].vy+vecs[p1+RING_SEGS].vy)>>1);
			setlen(g3,6);
			if ((z >= 0) && (z <  OT_LENGTH))
				addPrim(&o[z], g3);
			g3++;
		}
	}
	//dumpXY4(g4);

	PLATFORM.DeclareUsedMiscPolyG4s((POLY_G4*)g3);
}



#define ftoimv(x)  ((int)((x*32)/5.4017))

#define MESH_VERTEX(x,y,z) {ftoimv(x),ftoimv(y),-ftoimv(z)}
#define MESH_FACE(a,b,c) {a,b,c,0}

#define SPHERE_NUMVERTS 26
#define SPHERE_NUMFACES 40
		//MESH_NUMVERTEX 26
		//MESH_NUMFACES 40
struct SVECTOR hemiverts[26]= {
		MESH_VERTEX(0.0000 ,0.0000	,5.4017),

		MESH_VERTEX(2.8398 ,0.0000	,4.5949),
		MESH_VERTEX(0.8776 ,2.7008	,4.5949),
		MESH_VERTEX(-2.2975,1.6692	,4.5949),
		MESH_VERTEX(-2.2975,-1.6692,4.5949),
		MESH_VERTEX(0.8776 ,-2.7008,4.5949),

		MESH_VERTEX(4.8314 ,0.0000	,2.4157),
		MESH_VERTEX(1.4930 ,4.5949	,2.4157),
		MESH_VERTEX(-3.9087,2.8398	,2.4157),
		MESH_VERTEX(-3.9087,-2.8398,2.4157),
		MESH_VERTEX(1.4930 ,-4.5949,2.4157),

		MESH_VERTEX(3.7174 ,2.7008	,2.8398),
		MESH_VERTEX(-1.4199,4.3700	,2.8398),
		MESH_VERTEX(-4.5949,-0.0000,2.8398),
		MESH_VERTEX(-1.4199,-4.3700,2.8398),
		MESH_VERTEX(3.7174 ,-2.7008,2.8398),

		MESH_VERTEX(5.1373 ,1.6692	,0.0000),
		MESH_VERTEX(3.1750 ,4.3700	,0.0000),
		MESH_VERTEX(-0.0000,5.4017	,0.0000),
		MESH_VERTEX(-3.1750,4.3700	,0.0000),
		MESH_VERTEX(-5.1373,1.6692	,0.0000),
		MESH_VERTEX(-5.1373,-1.6692,0.0000),
		MESH_VERTEX(-3.1750,-4.3700,0.0000),
		MESH_VERTEX(0.0000 ,-5.4017,0.0000),
		MESH_VERTEX(3.1750 ,-4.3700,0.0000),
		MESH_VERTEX(5.1373 ,-1.6692,0.0000)
};

struct  {
	char v1,v2,v3,pad;
}MFaces[40]={
		MESH_FACE	(0  ,  1  ,  2 ),
		MESH_FACE	(0  ,  2  ,  3 ),
		MESH_FACE	(0  ,  3  ,  4 ),
		MESH_FACE	(0  ,  4  ,  5 ),
		MESH_FACE	(0  ,  5  ,  1 ),
		MESH_FACE	(11 ,  2  ,  1 ),
		MESH_FACE	(12 ,  3  ,  2 ),
		MESH_FACE	(13 ,  4  ,  3 ),
		MESH_FACE	(14 ,  5  ,  4 ),
		MESH_FACE	(15 ,  1  ,  5 ),
		MESH_FACE	(1  , 15  ,  6 ),
		MESH_FACE	(1  ,  6  , 11 ),
		MESH_FACE	(2  , 11  ,  7 ),
		MESH_FACE	(2  ,  7  , 12 ),
		MESH_FACE	(3  , 12  ,  8 ),
		MESH_FACE	(3  ,  8  , 13 ),
		MESH_FACE	(4  , 13  ,  9 ),
		MESH_FACE	(4  ,  9  , 14 ),
		MESH_FACE	(5  , 14  , 10 ),
		MESH_FACE	(5  , 10  , 15 ),
		MESH_FACE	(16 , 11  ,  6 ),
		MESH_FACE	(17 ,  7  , 11 ),
		MESH_FACE	(18 , 12  ,  7 ),
		MESH_FACE	(19 ,  8  , 12 ),
		MESH_FACE	(20 , 13  ,  8 ),
		MESH_FACE	(21 ,  9  , 13 ),
		MESH_FACE	(22 , 14  ,  9 ),
		MESH_FACE	(23 , 10  , 14 ),
		MESH_FACE	(24 , 15  , 10 ),
		MESH_FACE	(25 ,  6  , 15 ),
		MESH_FACE	(6  , 25  , 16 ),
		MESH_FACE	(11 , 16  , 17 ),
		MESH_FACE	(7  , 17  , 18 ),
		MESH_FACE	(12 , 18  , 19 ),
		MESH_FACE	(8  , 19  , 20 ),
		MESH_FACE	(13 , 20  , 21 ),
		MESH_FACE	(9  , 21  , 22 ),
		MESH_FACE	(14 , 22  , 23 ),
		MESH_FACE	(10 , 23  , 24 ),
		MESH_FACE	(15 , 24  , 25 )
};
#define NUM_LVERTS	106
#undef ftoimv
#undef MESH_VERTEX
#define ftoimv(x)  ((int)(((x)*32)/140))
#define MESH_VERTEX(x,y,z) {ftoimv((x)),ftoimv((y)),-ftoimv(z)}


// All lightning starts from point 0 in this list, then choose a random number (0-20) and use that as the base for a
//	meandering line.

struct SVECTOR lightning_verts[NUM_LVERTS]= {

			MESH_VERTEX(0.0000		,4.9533		,955.9452),



			MESH_VERTEX(-57.5190	,386.5674	,867.8605),
			MESH_VERTEX(-167.4463	,352.6594	,867.8605),
			MESH_VERTEX(-262.4953	,287.8561	,867.8605),
			MESH_VERTEX(-334.2204	,197.9157	,867.8605),
			MESH_VERTEX(-376.2486	,90.8297	,867.8605),
			MESH_VERTEX(-384.8454	,-23.8868	,867.8605),
			MESH_VERTEX(-359.2470	,-136.0407	,867.8605),
			MESH_VERTEX(-301.7280	,-235.6666	,867.8605),
			MESH_VERTEX(-217.3990	,-313.9124	,867.8605),
			MESH_VERTEX(-113.7533	,-363.8257	,867.8605),
			MESH_VERTEX(-0.0000		,-380.9712	,867.8605),
			MESH_VERTEX(113.7531	,-363.8257	,867.8605),
			MESH_VERTEX(217.3989	,-313.9125	,867.8605),
			MESH_VERTEX(301.7279	,-235.6667	,867.8605),
			MESH_VERTEX(359.2470	,-136.0408	,867.8605),
			MESH_VERTEX(384.8454	,-23.8869	,867.8605),
			MESH_VERTEX(376.2486	,90.8295	,867.8605),
			MESH_VERTEX(334.2205	,197.9155	,867.8605),
			MESH_VERTEX(262.4954	,287.8560	,867.8605),
			MESH_VERTEX(167.4464	,352.6593	,867.8605),
			MESH_VERTEX(57.5191		,386.5674	,867.8605),

			MESH_VERTEX(-90.1690	,603.1863	,718.5042),
			MESH_VERTEX(-262.4953	,550.0306	,718.5042),
			MESH_VERTEX(-411.4977	,448.4426	,718.5042),
			MESH_VERTEX(-523.9368	,307.4485	,718.5042),
			MESH_VERTEX(-589.8217	,139.5764	,718.5041),
			MESH_VERTEX(-603.2985	,-40.2575	,718.5041),
			MESH_VERTEX(-563.1694	,-216.0743	,718.5041),
			MESH_VERTEX(-473.0003	,-372.2518	,718.5041),
			MESH_VERTEX(-340.8031	,-494.9129	,718.5040),
			MESH_VERTEX(-178.3240	,-573.1587	,718.5040),
			MESH_VERTEX(-0.0000		,-600.0367	,718.5040),
			MESH_VERTEX(178.3238	,-573.1588	,718.5040),
			MESH_VERTEX(340.8029	,-494.9130	,718.5040),
			MESH_VERTEX(473.0002	,-372.2519	,718.5041),
			MESH_VERTEX(563.1694	,-216.0745	,718.5041),
			MESH_VERTEX(603.2984	,-40.2578	,718.5041),
			MESH_VERTEX(589.8218	,139.5762	,718.5041),
			MESH_VERTEX(523.9369	,307.4483	,718.5042),
			MESH_VERTEX(411.4979	,448.4424	,718.5042),
			MESH_VERTEX(262.4955	,550.0306	,718.5042),
			MESH_VERTEX(90.1692		,603.1862	,718.5042),


			MESH_VERTEX(-114.8071	,766.6495	,511.2126),
			MESH_VERTEX(-334.2204	,698.9695	,511.2126),
			MESH_VERTEX(-523.9368	,569.6230	,511.2126),
			MESH_VERTEX(-667.0991	,390.1033	,511.2125),
			MESH_VERTEX(-750.9866	,176.3613	,511.2125),
			MESH_VERTEX(-768.1458	,-52.6111	,511.2125),
			MESH_VERTEX(-717.0518	,-276.4686	,511.2125),
			MESH_VERTEX(-602.2446	,-475.3206	,511.2125),
			MESH_VERTEX(-433.9253	,-631.4980	,511.2124),
			MESH_VERTEX(-227.0499	,-731.1240	,511.2124),
			MESH_VERTEX(-0.0000		,-765.3463	,511.2124),
			MESH_VERTEX(227.0496	,-731.1241	,511.2124),
			MESH_VERTEX(433.9251	,-631.4982	,511.2124),
			MESH_VERTEX(602.2444	,-475.3208	,511.2125),
			MESH_VERTEX(717.0517	,-276.4689	,511.2125),
			MESH_VERTEX(768.1458	,-52.6114	,511.2125),
			MESH_VERTEX(750.9867	,176.3610	,511.2125),
			MESH_VERTEX(667.0992	,390.1031	,511.2125),
			MESH_VERTEX(523.9370	,569.6229	,511.2126),
			MESH_VERTEX(334.2206	,698.9694	,511.2126),
			MESH_VERTEX(114.8074	,766.6494	,511.2126),



			MESH_VERTEX(-129.2440	,862.4326	,264.4045),
			MESH_VERTEX(-376.2486	,786.2419	,264.4045),
			MESH_VERTEX(-589.8217	,640.6302	,264.4045),
			MESH_VERTEX(-750.9866	,438.5359	,264.4045),
			MESH_VERTEX(-845.4231	,197.9158	,264.4044),
			MESH_VERTEX(-864.7400	,-59.8498	,264.4044),
			MESH_VERTEX(-807.2209	,-311.8574	,264.4044),
			MESH_VERTEX(-677.9767	,-535.7150	,264.4044),
			MESH_VERTEX(-488.4913	,-711.5317	,264.4044),
			MESH_VERTEX(-255.6013	,-823.6857	,264.4044),
			MESH_VERTEX(-0.0000		,-862.2114	,264.4044),
			MESH_VERTEX(255.6010	,-823.6857	,264.4044),
			MESH_VERTEX(488.4911	,-711.5319	,264.4044),
			MESH_VERTEX(677.9766	,-535.7152	,264.4044),
			MESH_VERTEX(807.2208	,-311.8577	,264.4044),
			MESH_VERTEX(864.7399	,-59.8501	,264.4044),
			MESH_VERTEX(845.4232	,197.9155	,264.4044),
			MESH_VERTEX(750.9868	,438.5356	,264.4045),
			MESH_VERTEX(589.8220	,640.6300	,264.4045),
			MESH_VERTEX(376.2488	,786.2417	,264.4045),
			MESH_VERTEX(129.2444	,862.4326	,264.4045),


			MESH_VERTEX(-132.1971	,882.0250	,0.0000),
			MESH_VERTEX(-384.8454	,804.0934	,0.0000),
			MESH_VERTEX(-603.2984	,655.1547	,0.0000),
			MESH_VERTEX(-768.1458	,448.4427	,0.0000),
			MESH_VERTEX(-864.7400	,202.3248	,0.0000),
			MESH_VERTEX(-884.4982	,-61.3305	,0.0000),
			MESH_VERTEX(-825.6649	,-319.0961	,0.0000),
			MESH_VERTEX(-693.4677	,-548.0685	,0.0000),
			MESH_VERTEX(-499.6528	,-727.9025	,0.0000),
			MESH_VERTEX(-261.4415	,-842.6190	,0.0000),
			MESH_VERTEX(-0.0000		,-882.0250	,0.0000),
			MESH_VERTEX(261.4412	,-842.6191	,0.0000),
			MESH_VERTEX(499.6525	,-727.9026	,0.0000),
			MESH_VERTEX(693.4675	,-548.0687	,0.0000),
			MESH_VERTEX(825.6648	,-319.0964	,0.0000),
			MESH_VERTEX(884.4982	,-61.3308	,0.0000),
			MESH_VERTEX(864.7401	,202.3245	,0.0000),
			MESH_VERTEX(768.1459	,448.4424	,0.0000),
			MESH_VERTEX(603.2986	,655.1545	,0.0000),
			MESH_VERTEX(384.8457	,804.0933	,0.0000),
			MESH_VERTEX(132.1974	,882.0250	,0.0000)
};
//extern void SetLightMatrix(void);

#define LIGHTNING_NUMLINES	5
#define LIGHTNING_NUMVERTS	6

int Rval;

// Eg. Range 32 gives int from 0-31
int FXRand(SINT range)
{
	if (!(GAME.IsGamePanelActive()))Rval=((Rval*5)+1)&65535;
	return (Rval*range)>>16;
}

void RenderLightning(CParticle *p,CThing *mOwner)
{

	//MATRIX		mat;

	if(!mOwner)return;
    //CCamera     *cam = ENGINE.GetCamera();

	long col=Interpolcol(&p->mColour,p->uPRing.mBrightness);

	SetParticleCommonOffset(p,mOwner);



	CCharacter *c=mOwner->IsCharacter();
	if(!c)return;

	CommonSetTrans();

	GMatrix mtx;
	MATRIX objmt;
	mtx=c->GetRenderOrientation(1);
	ENGINE.GMatrixToTransposeMATRIX(&mtx,&objmt);

	SetMulRotMatrix(&objmt);

	CameraAlreadyLoaded=0;

	SetBackColor(80,80,80);
	long *cols=(long*)(0x1f800000);
	SVECTOR *vecs=(SVECTOR*)(0x1f800000)+LIGHTNING_NUMVERTS;  // Leave space for the colours
 	NTAG		*o = PLATFORM.GetOrderTable();
	//LINE_F2MODE		*g3 =(LINE_F2MODE*) PLATFORM.GetMiscPolyG4s();
	LINE_F2		*g3 =(LINE_F2*) PLATFORM.GetMiscPolyG4s();


	for(int zz=0;zz<5;zz++)
	{
		vecs[0]=lightning_verts[0];
		int i,r;
		r=FXRand(21);
		for(i=0;i<5;i++)
		{
			if(FXRand(2))r=r+1;
			else if(FXRand(2))r=r-1;
			if(r<0)r=20;
			if(r>20)r=0;
			//printf("R: %d\n",r);
			vecs[i+1]=lightning_verts[r+(i*21)+1];//*(1+(p->mRadius));
		}

		unsigned long c=0xffff00;
		c=Interpolcol(&c,p->uPRing.mBrightness);
		gte_ldrgb(&c);                    // Setup the passed colour for depthqueue
		SetMyDepthQueue();

		for(int i=0;i<6;i++)
		{
			gte_ldv0(&vecs[i]);
			gte_rtps();
			gte_stsxy(&vecs[i]);
			gte_stszotz(&vecs[i].vz);
			gte_dpcs();
			__evaluate (__arg0, (long)(&cols[i]));
   			__asm_start();
   			__I_mfc2	(12,22);
   			__I_lui	(13,0x4200);            // Semi-trans Line F2
   			__I_or	(12,12,13);
   			__I_sw	(12, 0, __arg0);
   			__asm_end();

		}


		ULONG xy0,xy1,xy2,clip;
		for(int i=0;i<5;i++)
		{
			xy0=*(ULONG*)(&vecs[i]);
			xy1=*(ULONG*)(&vecs[i+1]);



			{
				*(ULONG*)(&g3->x0)=xy0;
				*(ULONG*)(&g3->x1)=xy1;

				*(ULONG*)(&g3->r0)=cols[i];
				//g3->modeword=0xe1000620;  // Set ABR=1+1
				//setlen(g3,4);
				setlen(g3,3);
				int z;
				z=vecs[i].vz>>2;
				if ((z >= 0) && (z <  OT_LENGTH))
				{
					addPrim(&o[z], g3);
					g3++;
				}
			}
		}

	}


	PLATFORM.DeclareUsedMiscPolyG4s((POLY_G4*)g3);
}






void RenderAlphaShpere(CParticle *p)
{

	long col=Interpolcol(&p->mColour,p->uPRing.mBrightness);


	CommonOffsetCoord(p->mPos.X.mVal,p->mPos.Y.mVal,p->mPos.Z.mVal);


	SetBackColor(80,80,80);
	long *cols=(long*)(0x1f800000);
	SVECTOR *vecs=(SVECTOR*)(0x1f800000)+SPHERE_NUMVERTS;  // Leave space for the colours
 	NTAG		*o = PLATFORM.GetOrderTable();
	POLY_G3		*g3 =(POLY_G3*) PLATFORM.GetMiscPolyG4s();
	for(int i=0;i<SPHERE_NUMVERTS;i++)
	{
		vecs[i].vx=hemiverts[i].vx*(1+(p->mRadius));
		vecs[i].vy=hemiverts[i].vy*(1+(p->mRadius));
		vecs[i].vz=hemiverts[i].vz*(1+(p->mRadius));
	}
	CommonSetTrans();

	long c=Interpolcol(&p->mColour,p->uPHemi.mBrightness);
	gte_ldrgb(&c);                    // Setup the passed colour for depthqueue
	__I_addiu(12,0,0xD38f);
	__I_addiu(13,0,0x27c1);
	__I_sll(13,13,12);
	__I_ctc2(12,27);
	__I_ctc2(13,28);                    // Setup the depth queueing values DQA and DQB

	for(int i=0;i<SPHERE_NUMVERTS;i++)
	{
		gte_ldv0(&vecs[i]);
		gte_rtps();
		gte_stsxy(&vecs[i]);
		gte_stszotz(&vecs[i].vz);
		gte_dpcs();
		__evaluate (__arg0, (long)(&cols[i]));
   		__asm_start();
   		__I_mfc2	(12,22);
   		__I_lui	(13,0x3200);            // Semi-trans G3
   		__I_or	(12,12,13);
   		__I_sw	(12, 0, __arg0);
   		__asm_end();

	}


	ULONG xy0,xy1,xy2,clip;
	for(int i=0;i<SPHERE_NUMFACES;i++)
	{
		xy0=*(ULONG*)(&vecs[MFaces[i].v1]);
		xy1=*(ULONG*)(&vecs[MFaces[i].v2]);
		xy2=*(ULONG*)(&vecs[MFaces[i].v3]);


		gte_ldsxy3(xy0,xy1,xy2);
  		gte_nclip();


		*(ULONG*)(&g3->x0)=xy0;
		gte_stopz(&clip);
		if(clip>=0)
		{
			*(ULONG*)(&g3->x1)=xy1;
			*(ULONG*)(&g3->x2)=xy2;

			*(ULONG*)(&g3->r0)=cols[MFaces[i].v1];
			*(ULONG*)(&g3->r1)=cols[MFaces[i].v2];
			*(ULONG*)(&g3->r2)=cols[MFaces[i].v3];
			setlen(g3,6);
			int z;
			gte_AverageZ3(*(short*)(&vecs[MFaces[i].v1].vz),*(short*)(&vecs[MFaces[i].v2].vz),*(short*)(&vecs[MFaces[i].v3].vz),&z)
			if ((z >= 0) && (z <  OT_LENGTH))
			{
				addPrim(&o[z], g3);
				g3++;
			}
		}
	}
	PLATFORM.DeclareUsedMiscPolyG4s((POLY_G4*)g3);
}


void RenderAlphaSpark(CParticle *p, BOOL alpha, SINT Queuelength,CThing *mOwner)
{
	long *cols=(long*)(0x1f800000);
	SVECTOR *vecs=(SVECTOR*)(0x1f800000)+8;  // Leave space for the colours
	long col=Interpolcol(&p->mColour,p->uPSpark.mBrightness);

	SetMyDepthQueue();

	NTAG		*o = PLATFORM.GetOrderTable();
	LINE_G2		*g3 =(LINE_G2*) PLATFORM.GetMiscPolyG4s();

	SetParticleCommonOffset(p,mOwner);
	//CommonOffsetCoord(p->mPos.X.mVal,p->mPos.Y.mVal,p->mPos.Z.mVal);
	CommonSetTrans();


	int colscale=0x1000/(Queuelength-1);

	if (!alpha)
	{
		for(int sc=0;sc<Queuelength;sc++)
		{

			long col2;
			gte_ldrgb(&col);                    // Setup the passed colour for depthqueue
			gte_lddp((sc*colscale));
			gte_dpcs();
			gte_strgb(&col2);
			gte_ldv0(&p->uPSpark.mQueue[sc]);
			gte_ldrgb(&col2);
			gte_rtps();

			gte_stsxy(&vecs[sc]);
			gte_stszotz(&vecs[sc].vz);
			gte_dpcs();
			__evaluate (__arg0, (long)(&cols[sc]));
	   		__asm_start();
	   		__I_mfc2	(12,22);
	   		__I_lui	(13,0x5000);            // None-trans LINE_G2
			__I_sll(12,12,8);
			__I_srl(12,12,8);
	   		__I_or	(12,12,13);
	   		__I_sw	(12, 0, __arg0);
	   		__asm_end();
		}
	}
	else
	{
		for(int sc=0;sc<Queuelength;sc++)
		{
			long col2;
			gte_ldrgb(&col);                    // Setup the passed colour for depthqueue
			gte_lddp((sc*colscale));
			gte_dpcs();
			gte_strgb(&col2);

			gte_ldv0(&p->uPSpark.mQueue[sc]);
			gte_ldrgb(&col2);
			gte_rtps();
			gte_stsxy(&vecs[sc]);
			gte_stszotz(&vecs[sc].vz);
			gte_dpcs();
			__evaluate (__arg0, (long)(&cols[sc]));
	   		__asm_start();
	   		__I_mfc2	(12,22);
	   		__I_lui	(13,0x5200);            // Semi-trans LINE_G2
			__I_sll(12,12,8);
			__I_srl(12,12,8);
			__I_or	(12,12,13);
	   		__I_sw	(12, 0, __arg0);
	   		__asm_end();

		}
	}



	#if 1
	for(int c=0;c<Queuelength-1;c++)
	{
		ULONG xy0,xy1,rgb0,rgb1;
		SINT z=vecs[c].vz>>2;
		if((z>0)&&(z<OT_LENGTH))
		{
			xy0=*(ULONG*)(&vecs[c]);
			xy1=*(ULONG*)(&vecs[c+1]);
			rgb0=*(ULONG*)(&cols[c]);
			rgb1=*(ULONG*)(&cols[c+1]);
			*(ULONG*)(&g3->x0)=xy0;
			*(ULONG*)(&g3->x1)=xy1;
			*(ULONG*)(&g3->r0)=rgb0;
			*(ULONG*)(&g3->r1)=rgb1;
		//addPrim(&o[vecs[c].vz>>2],g3);
 			setlen(g3,4);
		  addPrim(&o[z],g3);
			g3++;
		}
	}
	#endif
	PLATFORM.DeclareUsedMiscPolyG4s((POLY_G4*)g3);


#if 0
extern void DecalRenderFromVerts(long col, SINT z_fudge, STextureRefOther *tr);
ENGINE.GetParticleTexture(TP_FIRE1);
	// Right... If it's a long trail (>3) then draw a 'star' at the end of it....
	// vecs[0] *already* contains the correct location, so we just need to re-translate to get the width...
	if(Queuelength>3)
	{
		gte_SetWidthDamo(32);
	}
#endif


}
extern void DecalRenderFromVerts(long col, SINT z_fudge, STextureRefOther *tr);


inline int gte_getDP(void)
{
	register int retval;
	asm volatile("	mfc2 %0,$8":"=r"(retval)::);
	return retval;;
}


void RenderStarTrail(CParticle *p, SINT Queuelength)
{
	SVECTOR *vecs=(SVECTOR*)(0x1f800000);

	long col=Interpolcol(&p->mColour,p->uPSpark.mBrightness);
	STextureRefOther *tr[2];
	tr[0]=ENGINE.GetParticleTexture(TP_SSTAR1);
	tr[1]=ENGINE.GetParticleTexture(TP_SSTAR2);
	//printf("+++++++++++++\n");
	int z;

	// Calculate the camera relative center of the effect...
	vecs[5].vx=((p->mPos.X.mVal-SPCamera->mPos.X.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE));
	vecs[5].vy=((p->mPos.Y.mVal-SPCamera->mPos.Y.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE));
	vecs[5].vz=((p->mPos.Z.mVal-SPCamera->mPos.Z.mVal)>>(GINT_SHIFT - PSXENGINE_SCALE));

	for(int sc=0;sc<Queuelength;sc++)
	{
		gte_SetWidthDamo(16<<4);               // Size of a star...
		gte_ldv0(&p->uPSpark.mQueue[sc]);

		gte_ldv0i(
			   		p->uPSpark.mQueue[sc].vx+vecs[5].vx,
			   		p->uPSpark.mQueue[sc].vy+vecs[5].vy,
			   		p->uPSpark.mQueue[sc].vz+vecs[5].vz
					);
		gte_rtps();
		z=gte_getopz()>>16;
		gte_stsxy(&vecs[4]);
		SetMyDepthQueue();
		CommonSetTrans();
		//if(!sc)printf("Z: 0x%08x\n",z);
		vecs[0].vx=vecs[4].vx-z;
		vecs[0].vz=vecs[4].vy-z;
		vecs[0].vy=0;
		vecs[1].vx=vecs[4].vx+z;
		vecs[1].vz=vecs[4].vy-z;
		vecs[1].vy=0;
		vecs[2].vx=vecs[4].vx-z;
		vecs[2].vz=vecs[4].vy+z;
		vecs[2].vy=0;
		vecs[3].vx=vecs[4].vx+z;
		vecs[3].vz=vecs[4].vy+z;
		vecs[3].vy=0;
		DecalRenderFromVerts(0x808080,0,tr[sc&1]);
	}
}



#endif //TARGET == PSX
