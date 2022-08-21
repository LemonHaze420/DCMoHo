#include	"Common.h"

#if TARGET == PSX

#include	"PSXDecal.h"
#include	"Globals.h"
#include "mwinline.h"
#include "psx3dengine.h"

//******************************************************************************************
// Deacl Size will be HALF the PIXEL WIDTH of the sprite...
//#define DECAL_SIZE  12
#define DECAL_SIZE  15  //JCL - new with GetRenderRadius()







void	CPSXDecal::CommonDecalRender(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT z_fudge)
{
	long	width = (render_radius * DECAL_SIZE).Whole();


	NTAG		*o = PLATFORM.GetOrderTable();
	POLY_FT4	*p4 = PLATFORM.GetMiscPolyFT4s();

	//MATRIX		mat;

    //CCamera     *cam = ENGINE.GetCamera();

	//ENGINE.GMatrixToGTE(&cam->mOrientation);


	//GMatrix	foo = cam->mOrientation.Transpose();


	//GVector offpos = *pos - cam->mPos;
    //SINT    tx = (foo.Row[0] * offpos).GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);
	//SINT	ty = (foo.Row[1] * offpos).GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);
	//SINT	tz = (foo.Row[2] * offpos).GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);


	//VECTOR	hack = {tx, ty, tz};


	//TransMatrix(&mat,&hack);
	//SetTransMatrix(&mat);


	CommonOffsetCoord(pos->X.mVal,pos->Y.mVal,pos->Z.mVal);


	long flags;
	asm volatile("cfc2 %0,$31":"=r"(flags)::);



	CommonSetTrans();

	if(flags<0)return;                  // overflow!

	SVECTOR     point;

	// JCLHACK
//	point.vx = (pos->X.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE));
//	point.vy = (pos->Y.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE));
//	point.vz = (pos->Z.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE));
	point.vx = 0;
	point.vy = 0;
	point.vz = 0;


    struct {
		SWORD	sx;
		SWORD	sy;
    }           sp;

    long    dummy,xsize,dpq;


    gte_SetWidthDamo(width);       // Use the GTE to calculate a depth-queued width!!!!

    long    z = (RotTransPers(&point, (long *)(&sp), &xsize, &dummy)-32)>>2;



	z += z_fudge;

	// Do onscreen check using calculated width....
	if(((UINT)(sp.sx+xsize)<(UINT)(PLATFORM.ScreenWidth()+(xsize<<1)))&&
		((UINT)(sp.sy+xsize)<(UINT)(256+(xsize<<1)))&&(z > 0) && (z <  850))
	{

		SetMyDepthQueue();
		gte_rtps();                         // Do the point again to get the DPQ value...


    	//sp.sy =  PLATFORM.ScreenHeight() - sp.sy; //!PSX !!!!!!!

		// fill out the decal





		p4->x0  = sp.sx - xsize;
    	p4->y0  = sp.sy - xsize;

    	p4->x1  = sp.sx + xsize;
    	p4->y1  = sp.sy - xsize;


		asm volatile("cfc2 %0,$31":"=r"(flags)::);
		gte_ldrgb(&col);
		if(flags<0)
		{
			//printf("Fecked up!\n");
			return;
		}

		gte_dpcs();




		p4->x2  = sp.sx - xsize;
    	p4->y2  = sp.sy + xsize;

    	p4->x3  = sp.sx + xsize;
    	p4->y3  = sp.sy + xsize;


   	__evaluate (__arg0, (long)(&(p4->r0)));
   	__asm_start();
   	__I_mfc2	(12,22);
   	__I_lui	(13,0x2c00);
   	__I_or	(12,12,13);
   	__I_sw	(12, 0, __arg0);
   	__asm_end();

		//*(SLONG*)(&(p4->r0))=col|0x2c000000;


		setlen(p4,9);
    	p4->tpage = tr->mTPAGE_P1P1;
    	p4->clut  = tr->mCLUT;
    	setUVWH(p4, tr->mX, tr->mY, tr->mW - 1, tr->mH - 1);

		setSemiTrans(p4, 1);

		addPrim(&o[z], p4);

		p4 ++;
		PLATFORM.DeclareUsedMiscPolyFT4s(p4);
	}
}


inline SINT gte_GetMAC1(void)
{
	register retval;
	asm volatile("	mfc2 %0,$25":"=r"(retval)::);
	return retval;
}
inline SINT gte_GetMAC2(void)
{
	register retval;
	asm volatile("	mfc2 %0,$26":"=r"(retval)::);
	return retval;
}

#define gte_ldreg( r0 , r1 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_mtc2 (__arg0, r1);\
   __asm_end();


void DecalRenderFromVerts(long col, SINT z_fudge, STextureRefOther *tr)
{

	SVECTOR     *vecs=(SVECTOR*)0x1f800000;
	NTAG		*o = PLATFORM.GetOrderTable();
	POLY_FT4	*p4 = PLATFORM.GetMiscPolyFT4s();
	long    z,dpq;

	gte_ldv3(&vecs[0],&vecs[1],&vecs[2]);
	gte_rtpt();



	gte_stsxy3(&p4->x0,&p4->x1,&p4->x2);
	gte_ldv0(&vecs[3]);
	gte_rtps();
	gte_ldrgb(&col);
	gte_stsxy(&p4->x3);
	z=(gte_Get_szotz()>>2)+z_fudge;


	//printf("Z: %d\n",z);
	//printf("x0: %04x  y0: %04x\n",p4->x0,p4->y0);
	//printf("x1: %04x  y1: %04x\n",p4->x1,p4->y1);
	//printf("x2: %04x  y2: %04x\n",p4->x2,p4->y2);
	//printf("x3: %04x  y3: %04x\n",p4->x3,p4->y3);




	//gte_setrgb(col);
	gte_dpcs();

	if(((z >= 0) && (z <  850))&&(!(*(ULONG*)&p4->x0&*(ULONG*)&p4->x1&*(ULONG*)&p4->x2&*(ULONG*)&p4->x3&OffScreenMask)))
	{
		__evaluate (__arg0, (long)(&(p4->r0)));
		__asm_start();
		__I_mfc2	(12,22);
		__I_lui	(13,0x2c00);
		__I_or	(12,12,13);
		__I_sw	(12, 0, __arg0);
		__asm_end();

		setlen(p4,9);
		p4->tpage = tr->mTPAGE_P1P1;
		p4->clut  = tr->mCLUT;
		setUVWH(p4, tr->mX, tr->mY, tr->mW - 1, tr->mH - 1);

		setSemiTrans(p4, 1);

		if ((z > 0) && (z <  OT_LENGTH))
			addPrim(&o[z], p4);

		p4 ++;

		PLATFORM.DeclareUsedMiscPolyFT4s(p4);
	}


}


















void	CPSXDecal::CommonDecalRenderHoriz(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT z_fudge, GINT yaw)
{
	long	width = (render_radius * DECAL_SIZE).Whole()<<4;


	CommonOffsetCoord(pos->X.mVal,pos->Y.mVal,pos->Z.mVal);



	__I_addiu(12,0,0xD38f);
	__I_addiu(13,0,0x27c1);
	__I_sll(13,13,12);
	__I_ctc2(12,27);
	__I_ctc2(13,28);

	CommonSetTrans();

	SVECTOR     *vecs=(SVECTOR*)0x1f800000;

	if (yaw != G0)
	{
		//GMatrix	m;
		//m.MakeRotationYaw(yaw);

		//GVector x = GVector(G(width), G0, G0);
		//GVector	y = GVector(G0, G(width), G0);

		//x = m * x;
		//y = m * y;

		//SINT	xx = x.X.Whole();
		//SINT	xy = x.Y.Whole();
		//SINT	yx = y.X.Whole();
		//SINT	yy = y.Y.Whole();


		SINT angle=(yaw*GINT(652)).mVal;
		SINCOS *sc=rcossin_tbl+(angle&4095);
		SINT sn=sc->sin;
		SINT cs=sc->cos;

		gte_ldreg(width,8);
		gte_ldreg(sn,9);
		gte_ldreg(cs,10);
		gte_gpf12();

		sn=gte_GetMAC1();
		cs=gte_GetMAC2();




		SINT	xx = cs;
		SINT	xy = sn;
		SINT	yx = -sn;
		SINT	yy = cs;





		vecs[0].vx= -xx - yx;
		vecs[0].vy= -xy - yy;

		vecs[1].vx= +xx - yx;
		vecs[1].vy= +xy - yy;

		vecs[2].vx= -xx + yx;
		vecs[2].vy= -xy + yy;

		vecs[3].vx= +xx + yx;
		vecs[3].vy= +xy + yy;

		vecs[0].vz=0;
		vecs[1].vz=0;
		vecs[2].vz=0;
		vecs[3].vz=0;
	}
	else
	{
		vecs[0].vx=-width;
		vecs[0].vy=-width;
		vecs[0].vz=0;
		vecs[1].vx=+width;
		vecs[1].vy=-width;
		vecs[1].vz=0;
		vecs[2].vx=-width;
		vecs[2].vy=+width;
		vecs[2].vz=0;
		vecs[3].vx=+width;
		vecs[3].vy=+width;
		vecs[3].vz=0;
	}
	DecalRenderFromVerts(col,z_fudge,tr);
}



void	CPSXDecal::CommonDecalRenderHoriz2(GVector *pos,GINT render_radius,STextureRefOther *tr,long col, SINT sn,SINT cs)
{
	long	width = (render_radius * DECAL_SIZE).Whole()<<4;



	CommonOffsetCoord(pos->X.mVal,pos->Y.mVal,pos->Z.mVal);



	SetMyDepthQueue();

	CommonSetTrans();

	SVECTOR     *vecs=(SVECTOR*)0x1f800000;

		gte_ldreg(width,8);
		gte_ldreg(sn,9);
		gte_ldreg(cs,10);
		gte_gpf12();

		sn=gte_GetMAC1();
		cs=gte_GetMAC2();




		SINT	xx = cs;
		SINT	xy = sn;
		SINT	yx = -sn;
		SINT	yy = cs;





		vecs[0].vx= -xx - yx;
		vecs[0].vy= -xy - yy;

		vecs[1].vx= +xx - yx;
		vecs[1].vy= +xy - yy;

		vecs[2].vx= -xx + yx;
		vecs[2].vy= -xy + yy;

		vecs[3].vx= +xx + yx;
		vecs[3].vy= +xy + yy;

		vecs[0].vz=0;
		vecs[1].vz=0;
		vecs[2].vz=0;
		vecs[3].vz=0;
		DecalRenderFromVerts(col,0,tr);

}






















extern int ShadowTexNumber;

#define return_gtereg(r,v) {__evaluate(-1,v);__I_mfc2(-1,r);}






void    CPSXDecal::RenderFromData(CThing *t, const SRTInfo &ri)
{
	if (ri.i3)
	{
		// special case for bullets
		SINT	frame = t->GetRenderFrame().Whole();
		STextureRefOther *tr;
		GVector		pos = t->GetRenderPos();
		SINT		tno;

		BOOL		shadow = (ri.mShadow == TRUE);

		if (frame == -1)
		{
			tno = ri.i0;
			if (tno != -1)
			{
				STextureRefOther *tr = ENGINE.GetOtherTexture(tno);
				CommonDecalRenderHoriz(&pos,t->GetRenderRadius(),tr,0x00808080, 0, t->GetRenderYaw());
			}

			// render the other one as well!
			tno = ri.i1;
			if (tno != -1)
			{
				STextureRefOther *tr = ENGINE.GetOtherTexture(tno);
				CommonDecalRenderHoriz(&pos,t->GetRenderRadius(),tr,0x00808080, 0, -t->GetRenderYaw());
			}

			tno = ri.i0;
			if((tno != -1) && (ri.mShadow==ST_SPRITE))
			{
				ShadowTexNumber = tno;                    // Shadow Texture
				MAP.DrawShadow(t);
			}
		}
		else
		{
			tno = (frame == 0) ? ri.i0 : ri.i1;
			if (tno != -1)
			{
				STextureRefOther *tr = ENGINE.GetOtherTexture(tno);
				CommonDecalRenderHoriz(&pos,t->GetRenderRadius(),tr,0x00808080, 0, t->GetRenderYaw());
			}

			if (frame != -1)
			{
				if(shadow)
				{
					ShadowTexNumber = tno;                    // Shadow Texture
					MAP.DrawShadow(t);
				}
			}
		}
	}
	else
	{
		SINT	frame = t->GetRenderFrame().Whole();
		SINT	tno = (frame == 0) ? ri.i0 : ri.i1;
		if (tno == -1)
			return;

		STextureRefOther *tr = ENGINE.GetOtherTexture(tno);
		if(ri.mShadow==ST_SPRITE)
		{
			ShadowTexNumber = tno;                    // Shadow Texture
			MAP.DrawShadow(t);
		}

		GVector		pos = t->GetRenderPos();
		CommonDecalRender(&pos,t->GetRenderRadius(),tr,0x00808080);
	}
}


#endif
