#include	"Common.h"

#if TARGET == PSX

#include	"Globals.h"
#include	"PSXDecal.h"
#include	"PSXParticleSystem.h"
#include 	"PSXLightning.h"
#include	"PSXFence.h"
#include	"PSXTrail.h"
#include	"PSXLaser.h"
#include	"PSXLink.h"
#include	"GamePanel.h"

#include	"Profile.h"


//#pragma optimization_level 0

//******************************************************************************************
GVector	CPSXEngine::GetEmitter(CThing *t, SINT no)
{
	SINT 	id = t->_GetClassID();
	SINT 	index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SPSXRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CPSXMesh	*mesh = ENGINE.GetMesh(mesh_no);

				return mesh->GetEmitter(no, t);
			}
		}
	}

	return ZERO_GVECTOR;
}

//******************************************************************************************
GVector	CPSXEngine::GetEmitterAnimated(CThing *t, SINT no)
{
	SINT 	id = t->_GetClassID();
	SINT 	index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SPSXRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CPSXMesh	*mesh = ENGINE.GetMesh(mesh_no);

				return mesh->GetEmitterAnimated(no, t);
			}
		}
	}

	return ZERO_GVECTOR;
}

GINT	CPSXEngine::GetFrameIncrement(SINT id, EAnimMode a, SINT* realindex)
{
	SINT index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SPSXRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CPSXMesh	*mesh = ENGINE.GetMesh(mesh_no);

				return mesh->GetFrameIncrement(a, realindex);
			}
		}
	}

	*realindex = 0;
	return G0;
}

//******************************************************************************************
CCollisionVolume 	*CPSXEngine::GetCollisionVolume(CThing *t)
{
	if (!t)
		return NULL;

	SINT	id = t->_GetClassID();
	SINT 	index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SPSXRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CPSXMesh	*mesh = ENGINE.GetMesh(mesh_no);

				mesh->EvaluateCollisionVolume(t);
				return mesh->GetCollisionVolume();
			}
		}
	}

	return NULL;
}

//******************************************************************************************
GINT 	CPSXEngine::GetRenderThingRadius(CThing *t)
{
	if (!t)
		return G1;

	SINT	id = t->_GetClassID();
	SINT 	index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SPSXRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CPSXMesh	*mesh = ENGINE.GetMesh(mesh_no);

				return mesh->mRadius;
			}
		}
	}

	return G1;
}

//******************************************************************************************

void	CPSXEngine::RenderThings()
{
	// Set default lighting for meshes...
	MATRIX lcol;
    SetMLight(lcol.m,0, 1.99 , 0.40, 0.10);
    SetMLight(lcol.m,1, 0.20 , 0.20 , 1.20);
    SetMLight(lcol.m,2, 1.95 , 1.95 , 1.95);
    SetColorMatrix(&lcol);

    // JCL - dodgy gamut - do a rectangle around a point "behind" the camera
#define	BEHIND_DIST		5
//#define	GAMUT_RADIUS	15
#define	GAMUT_RADIUS	18

    GINT	gminx, gminy, gmaxx, gmaxy;

    GVector	gpos = mCurrentCamera->mLookAt;
    GVector	behind = mCurrentCamera->mLookAt - mCurrentCamera->mPos;
    behind.Z = G0;
    behind.Normalise();
    behind = behind * BEHIND_DIST;
    gpos += behind;

	CThing *t = WORLD.GetFirstThing();
	while (t)
	{
		if ((  t->mFlags & TF_IN_MAPWHO) && //! well - this list should be created by scanning through the mapwhos...
		    (!(t->mFlags & TF_RENDER_IN_EDITOR_ONLY)) &&		//! combine these??
			(!(t->mFlags & TF_SUICIDE)) &&
			(!(t->mFlags & TF_SUICIDE_NEXT)))
		{
			//!PSX

			//! mmm nice gamut...
			GINT	rad = t->GetRenderRadius();

			if ((GABS(t->mPos.X - gpos.X) < G(GAMUT_RADIUS) + rad) &&
				(GABS(t->mPos.Y - gpos.Y) < G(GAMUT_RADIUS) + rad))
			{
//				if (!(t->IsPrimary()))
				if (((!(t->mFlags & TF_FLASHING)) || (GAME.GetGameTurn() & 1)) &&
					((!(t->mFlags & TF_SLOW_FLASHING)) || ((GAME.GetGameTurn() & 2) ==0)))
				{

					// OK - now look up the render type
					SINT index = mThingRef[t->_GetClassID()];

					ASSERT(index != -1);

					if (index != -1)
					{
						SPSXRenderData	*rd = &mRenderData[index];

						// and render based on render data

						//!PSX just use render thing 0 for the moment...
						SINT	c0;
						SINT 	whichrt = t->WhichRTsToRender();

						for (c0 = 0; c0 < rd->mNumRenderThings; c0 ++)
						{
							if (whichrt & (1 << c0))
							{
								switch (rd->mRTIDs[c0])
								{
									case RTID_CRTMesh:
										CPSXMesh::RenderFromData(t, &rd->mRTInfos[c0]);
			                            break;

			                        case RTID_CRTDecal:
										CPSXDecal::RenderFromData(t, rd->mRTInfos[c0]);
			                            break;
									case RTID_CRTParticleSystem :
										CPSXParticleSystem::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									case RTID_CRTLightning:
										CPSXLightning::RenderFromData(t, rd->mRTInfos[c0]);
										break;

									case RTID_CRTFence:
										CPSXFence::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									case RTID_CRTTrail:
										CPSXTrail::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									case RTID_CRTLaser:
										CPSXLaser::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									case RTID_CRTLink:
										CPSXLink::RenderFromData(t, rd->mRTInfos[c0]);
										break;


									default:
										{
											//!PSX just draw a small decal
											NTAG		*o = PLATFORM.GetOrderTable();
											POLY_G4		*p4 = PLATFORM.GetMiscPolyG4s();

											MATRIX	mat;

											ENGINE.GMatrixToMATRIX(&mCurrentCamera->mOrientation, &mat);
											SetRotMatrix(&mat);

											GMatrix	foo = mCurrentCamera->mOrientation.Transpose();

											SINT	tx = (-foo.Row[0] * mCurrentCamera->mPos).GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);
											SINT	ty = (-foo.Row[1] * mCurrentCamera->mPos).GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);
											SINT	tz = (-foo.Row[2] * mCurrentCamera->mPos).GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);

											VECTOR	hack = {tx, ty, tz};

											TransMatrix(&mat,&hack);
											SetTransMatrix(&mat);

											SVECTOR		point;
											GVector		pos = t->GetPos();
											point.vx = (pos.X.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE));
											point.vy = (pos.Y.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE));
											point.vz = (pos.Z.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE));
											struct
											{
												SWORD	sx;
												SWORD	sy;
											}			sp;

											long	dummy;

											long	z = RotTransPers(&point, (long *)(&sp), &dummy, &dummy)>>2;
			                                //sp.sy =  PLATFORM.ScreenHeight() - sp.sy; //!PSX !!!!!!!

											// fill out the decal
								#define DECAL_SIZE	5
											setPolyG4(p4);

											p4->x0	= sp.sx - DECAL_SIZE;
											p4->y0	= sp.sy - DECAL_SIZE;
											p4->r0	= 255;
											p4->g0	= 0;
											p4->b0	= 0;

											p4->x1	= sp.sx + DECAL_SIZE;
											p4->y1	= sp.sy - DECAL_SIZE;
											p4->r1	= 0;
											p4->g1	= 255;
											p4->b1	= 0;

											p4->x2	= sp.sx - DECAL_SIZE;
											p4->y2	= sp.sy + DECAL_SIZE;
											p4->r2	= 0;
											p4->g2	= 0;
											p4->b2	= 255;

											p4->x3	= sp.sx + DECAL_SIZE;
											p4->y3	= sp.sy + DECAL_SIZE;
											p4->r3	= 255;
											p4->g3	= 0;
											p4->b3	= 255;

											setSemiTrans(p4, 0);

											if ((z >= 0) && (z <  OT_LENGTH))
												addPrim(&o[z], p4);

											p4 ++;
											PLATFORM.DeclareUsedMiscPolyG4s(p4);
										}
								};
							}
						}
					}
				}
			}
		}
		t = t->GetNextThing();
	}
}



//******************************************************************************************
//extern MATRIX SPCam,SPTCam;
//#include "PSXSmallParticle.h"
//extern CPSXSmallParticle ptcl;

/*****************************/
/*!!!!!!!!!!!NOTE!!!!!!!!!!!!*/
/* OffScreenMask NEEDS to be */
/*         global...         */
/*****************************/
//extern __declspec(data) volatile char dispframe;


inline int Gmul50000(GINT x)
{
	int v=x.GetVal()>>1;
	v=(v>>1)+v;
	return(v+(v>>6));

}
inline int Gmul50000(int x)
{
	int v=x>>1;
	v=(v>>1)+v;
	return(v+(v>>6));

}

//#define Gmul50000(x) ((x.GetVal()>>1)+(x.GetVal()>>2)+(x.GetVal()>>7)+(x.GetVal()>>8))
//#define Gmul50000(x) ((x>>1)+(x>>2)+(x>>7)+(x>>8))


void	CPSXEngine::Render(CCamera *cam , CCamera *cam2)
{




	// setup data
	mViewportCameras[0] = cam;
	mViewportCameras[1] = cam2;
	int views=1;
	if(cam2)
	{
		views++;
		PLATFORM.SetScreenWidth(256);
		OffScreenMask=0xff00ff00;
	}
	else
	{
		PLATFORM.SetScreenWidth(512);
		OffScreenMask=0xff00fe00;
	}

	// Clear Ordertable
    int safe;

	{
		PROFILE_FN(RenderFlip);
		PLATFORM.ClearDebugVSync();
		do
		{
			if (PLATFORM.GetDebugVSync() > 200)
			{
				printf("*** Drawsync missed.  Aborting...***\n");
				GAME.SetQuitFlag(GQ_FRONTEND);
				return;
			}
			safe = PLATFORM.GeometryMarkBeginScene();
		} while (safe == -1);
	}

	if(safe!=-1)
	{
#ifdef _DEBUG
		POLY_GT4*oldgt4=PLATFORM.GetMapPolys();
		POLY_GT4*newgt4;
#endif
		int tt1=mViewportCameras[0]->mOrientation.Row[0].Y.GetVal();
		int tt2=mViewportCameras[0]->mOrientation.Row[1].Y.GetVal();
		int tt3=mViewportCameras[0]->mOrientation.Row[2].Y.GetVal();
		tt1=-Gmul50000(tt1);
		tt2=-Gmul50000(tt2);
		tt3=-Gmul50000(tt3);
		mViewportCameras[0]->mOrientation.Row[0].Y.SetVal(tt1);
		mViewportCameras[0]->mOrientation.Row[1].Y.SetVal(tt2);
		mViewportCameras[0]->mOrientation.Row[2].Y.SetVal(tt3);

		//mViewportCameras[0]->mOrientation.Row[0].Y.SetVal(-(Gmul50000(mViewportCameras[0]->mOrientation.Row[0].Y)));
    	//mViewportCameras[0]->mOrientation.Row[1].Y.SetVal(-(Gmul50000(mViewportCameras[0]->mOrientation.Row[1].Y)));
    	//mViewportCameras[0]->mOrientation.Row[2].Y.SetVal(-(Gmul50000(mViewportCameras[0]->mOrientation.Row[2].Y)));

		if(views-1)
		{
 			int tt1=mViewportCameras[1]->mOrientation.Row[0].Y.GetVal();
			int tt2=mViewportCameras[1]->mOrientation.Row[1].Y.GetVal();
			int tt3=mViewportCameras[1]->mOrientation.Row[2].Y.GetVal();
			tt1=-Gmul50000(tt1);
			tt2=-Gmul50000(tt2);
			tt3=-Gmul50000(tt3);
			mViewportCameras[1]->mOrientation.Row[0].Y.SetVal(tt1);
			mViewportCameras[1]->mOrientation.Row[1].Y.SetVal(tt2);
			mViewportCameras[1]->mOrientation.Row[2].Y.SetVal(tt3);
			//mViewportCameras[1]->mOrientation.Row[0].Y.SetVal(-(Gmul50000(mViewportCameras[1]->mOrientation.Row[0].Y)));
    		//mViewportCameras[1]->mOrientation.Row[1].Y.SetVal(-(Gmul50000(mViewportCameras[1]->mOrientation.Row[1].Y)));
    		//mViewportCameras[1]->mOrientation.Row[2].Y.SetVal(-(Gmul50000(mViewportCameras[1]->mOrientation.Row[2].Y)));
		}

		//GMatrixToMATRIX(&mCamera->mOrientation, &SPCam);
		//GMatrixToTransposeMATRIX(&mCamera->mOrientation, &SPTCam);
		//SPCam.t[0]= cam->mPos.X.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);
		//SPCam.t[1]= cam->mPos.Y.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);
		//SPCam.t[2]= cam->mPos.Z.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);

		// render stuff
		// ** Remember to draw front to back!

		int f=safe;

		GAMEPANEL.Render();

		RenderPanel();
		NTAG_addPrim(&PLATFORM.GetOrderTable()[15],PLATFORM.GetScreenPrim(f+2));
		NTAG_Rlink(PLATFORM.GetOrderTable(),16,(long*)&POOL->mNTAGPanel[f].firstprim
														,(long*)&POOL->mNTAGPanel[f].lastprim);


		CameraAlreadyLoaded=0;          // Make sure CommonOffsetCoord knows to load the matrix...
		//printf("Views %d\n",views);
		for(int cnt=0;cnt<views;cnt++)
		{
			mCurrentCamera=mViewportCameras[cnt];      // Setup for correct camera view
			PLATFORM.SetViewport(cnt);          // Only used to indicate which DMA, Camera etc...
			scratchpadwh = GAME.GetLevelData()->mGVars[WATER_HEIGHT].GetVal();  // Waterheight in scratchpad
			SPCamera=GetCamera();
			SPCameraGMatrix=SPCamera->mOrientation;
			GMatrixToMATRIX(&SPCameraGMatrix,&SPCameraMATRIX);

			//ptcl.Process();

//			if (views == 2)
			{
				int sx = PLATFORM.ScreenWidth()>>1;
				SetGeomOffset(sx, PLATFORM.ScreenHeight()>>1);//+80);   /* set geometry origin as (160, 120) */
			}

			{
				PROFILE_FN(RenderThings);
				RenderThings();
			}

			{
				PROFILE_FN(RenderMap);
				MAP.Render();
			}
			// Now we need to 'link' the NTAG chain to the correct DMA header.
			// This frees up the NTAG array for the next time around....
			if(views-1)                 // if in two player mode, use the half-screen width drawenv
				NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetDrawPrim(f));
			else                        // Otherwise use the FULLscreenprim...
				NTAG_addPrim(&(PLATFORM.GetOrderTable()[OT_LENGTH-1]),PLATFORM.GetScreenPrim(f));

			// Link the current view (common for 1 and 2 player mode....
			NTAG_Rlink(PLATFORM.GetOrderTable(),OT_LENGTH,(long*)&POOL->mNTAGTerms[f][cnt].firstprim
														,(long*)&POOL->mNTAGTerms[f][cnt].lastprim);
		}


		if(views-1)                     // If in two player mode, link the two playlists together and add the panel last...
		{
			POOL->mNTAGTerms[f][0].lastprim=POOL->mNTAGTerms[f][1].firstprim;
			POOL->mNTAGTerms[f][1].lastprim=POOL->mNTAGPanel[f].firstprim;
		}
		else                            // Otherwisr just link the panel to the end of the single-player drawlist
			POOL->mNTAGTerms[f][0].lastprim=POOL->mNTAGPanel[f].firstprim;


#ifdef _DEBUG
		newgt4=PLATFORM.GetMapPolys();
		printf("Used GT4's: %05d   Memused: 0x%08x\n",newgt4-oldgt4,(int)newgt4-(int)oldgt4);
#endif




	}
    //PLATFORM.GeometryMarkEndScene();
}

//******************************************************************************************
//void	CPSXEngine::Flip()
//{
//	PLATFORM.Flip();
//}

//******************************************************************************************

#if 0
void CPSXEngine::CameraToPSX(MATRIX *mat)
{
	GMatrixToMatrix(&CAMERA.mCurrentCamera.mOrientation,mat);




}
#endif


//******************************************************************************************
void    CPSXEngine::GMatrixToMATRIX(const GMatrix *gmat, MATRIX *mat)
{
	// Convert a GMatrix to a PSX MATRIX
    SLONG t0,t1,t2,t3,t4,t5,t6,t7,t8;

    t0= (gmat->Row[0].X).GetVal() >> (GINT_SHIFT - 12);
    t1= (gmat->Row[0].Y).GetVal() >> (GINT_SHIFT - 12);
    t2= (gmat->Row[0].Z).GetVal() >> (GINT_SHIFT - 12);
    t3= (gmat->Row[1].X).GetVal() >> (GINT_SHIFT - 12);
    t4= (gmat->Row[1].Y).GetVal() >> (GINT_SHIFT - 12);
    t5= (gmat->Row[1].Z).GetVal() >> (GINT_SHIFT - 12);
    t6= (gmat->Row[2].X).GetVal() >> (GINT_SHIFT - 12);
    t7= (gmat->Row[2].Y).GetVal() >> (GINT_SHIFT - 12);
    t8= (gmat->Row[2].Z).GetVal() >> (GINT_SHIFT - 12);


	*(ULONG*)(&mat->m[0][0]) = (ULONG) (UWORD)t0|(((UWORD)(t3))<<16);
	*(ULONG*)(&mat->m[0][2]) = (ULONG) (UWORD)t6|(((UWORD)(t1))<<16);
	*(ULONG*)(&mat->m[1][1]) = (ULONG) (UWORD)t4|(((UWORD)(t7))<<16);
	*(ULONG*)(&mat->m[2][0]) = (ULONG) (UWORD)t2|(((UWORD)(t5))<<16);
	*(ULONG*)(&mat->m[2][2]) = (ULONG) (UWORD)t8;



#if 0
	*(UWORD*)(&mat->m[0][0]) = (UWORD)t0;
    *(UWORD*)(&mat->m[0][1]) = (UWORD)t3;
    *(UWORD*)(&mat->m[0][2]) = (UWORD)t6;

	*(UWORD*)(&mat->m[1][0]) = (UWORD)t1;
    *(UWORD*)(&mat->m[1][1]) = (UWORD)t4;
    *(UWORD*)(&mat->m[1][2]) = (UWORD)t7;

	*(UWORD*)(&mat->m[2][0]) = (UWORD)t2;
    *(UWORD*)(&mat->m[2][1]) = (UWORD)t5;
    *(UWORD*)(&mat->m[2][2]) = (UWORD)t8;
#endif
}



asm void GMatrixToGTELight(const GMatrix *gmat)
{
	lui		t9,0xffff                       // T7 is mask for  coords...

	lw		t0,0(a0)                        // Load row[0].X
	lw		t1,4(a0)                        // Load row[0].Y
	lw		t2,8(a0)                        // Load row[0].Z
	lw		t3,12(a0)                        // Load row[1].X
	lw		t4,16(a0)                        // Load row[1].Y
	lw		t5,20(a0)                        // Load row[1].Z
	lw		t6,24(a0)                        // Load row[2].X
	lw		t7,28(a0)                        // Load row[2].Y
	lw		t8,32(a0)                        // Load row[2].Z

	sra		t0,t0,GINT_SHIFT-12  // Shift row[0].X



	sra		t2,t2,GINT_SHIFT-12  // Shift row[0].X
	sra		t4,t4,GINT_SHIFT-12  // Shift row[0].X
	sra		t6,t6,GINT_SHIFT-12  // Shift row[0].X
	sra		t8,t8,GINT_SHIFT-12  // Shift row[0].X
	sll		t1,t1,(16-(GINT_SHIFT-12))
	sll		t3,t3,(16-(GINT_SHIFT-12))
	sll		t5,t5,(16-(GINT_SHIFT-12))
	sll		t7,t7,(16-(GINT_SHIFT-12))
	and		t1,t1,t9                        // Mask t1
	and		t3,t3,t9                        // Mask t3
	and		t5,t5,t9                        // Mask t5
	and		t7,t7,t9                        // Mask t7
	andi	t0,t0,0xffff                // Mask t0
	andi	t2,t2,0xffff                // Mask t0
	andi	t4,t4,0xffff                // Mask t0
	andi	t6,t6,0xffff                // Mask t0
// This section is the only difference between GMatrixTransposeToGTE and GMatrixToGTE......
	or		t0,t0,t3
	or		t2,t2,t5
	or		t4,t4,t7
	or		t6,t6,t1
//............Section ends.
	ctc2	t0,C2_L11L12
	ctc2	t6,C2_L13L21
	ctc2	t4,C2_L22L23
	ctc2	t2,C2_L31L32
	jr	ra
	ctc2	t8,C2_R33
}




asm void CPSXEngine::GMatrixToGTE(const GMatrix *gmat)
{
	lui		t9,0xffff                       // T7 is mask for  coords...

	lw		t0,0(a1)                        // Load row[0].X
	lw		t1,4(a1)                        // Load row[0].Y
	lw		t2,8(a1)                        // Load row[0].Z
	lw		t3,12(a1)                        // Load row[1].X
	lw		t4,16(a1)                        // Load row[1].Y
	lw		t5,20(a1)                        // Load row[1].Z
	lw		t6,24(a1)                        // Load row[2].X
	lw		t7,28(a1)                        // Load row[2].Y
	lw		t8,32(a1)                        // Load row[2].Z


	sra		t0,t0,GINT_SHIFT-12  // Shift row[0].X
	sra		t2,t2,GINT_SHIFT-12  // Shift row[0].Z
	sra		t4,t4,GINT_SHIFT-12  // Shift row[0].X
	sra		t6,t6,GINT_SHIFT-12  // Shift row[0].Z
	sra		t8,t8,GINT_SHIFT-12  // Shift row[0].X
	sll		t1,t1,(16-(GINT_SHIFT-12))
	sll		t3,t3,(16-(GINT_SHIFT-12))
	sll		t5,t5,(16-(GINT_SHIFT-12))
	sll		t7,t7,(16-(GINT_SHIFT-12))
	and		t1,t1,t9                        // Mask t1
	and		t3,t3,t9                        // Mask t3
	and		t5,t5,t9                        // Mask t5
	and		t7,t7,t9                        // Mask t7
	andi	t0,t0,0xffff                // Mask t0
	andi	t2,t2,0xffff                // Mask t0
	andi	t4,t4,0xffff                // Mask t0
	andi	t6,t6,0xffff                // Mask t0
// This section is the only difference between GMatrixTransposeToGTE and GMatrixToGTE......
	or		t0,t0,t3
	or		t2,t2,t5
	or		t4,t4,t7
	or		t6,t6,t1
//............Section ends.
	ctc2	t0,C2_R11R12
	ctc2	t6,C2_R13R21
	ctc2	t4,C2_R22R23
	ctc2	t2,C2_R31R32
	jr	ra
	ctc2	t8,C2_R33
}

asm void CPSXEngine::GMatrixTransposeToGTE(const GMatrix *gmat)
{
	lui		t9,0xffff                       // T7 is mask for  coords...

	lw		t0,0(a1)                        // Load row[0].X
	lw		t3,4(a1)                        // Load row[0].Y
	lw		t6,8(a1)                        // Load row[0].Z
	lw		t1,12(a1)                        // Load row[1].X
	lw		t4,16(a1)                        // Load row[1].Y
	lw		t7,20(a1)                        // Load row[1].Z
	lw		t2,24(a1)                        // Load row[2].X
	lw		t5,28(a1)                        // Load row[2].Y
	lw		t8,32(a1)                        // Load row[2].Z
	sra		t0,t0,GINT_SHIFT-12  // Shift row[0].X



	sra		t2,t2,GINT_SHIFT-12  // Shift row[0].X
	sra		t4,t4,GINT_SHIFT-12  // Shift row[0].X
	sra		t6,t6,GINT_SHIFT-12  // Shift row[0].X
	sra		t8,t8,GINT_SHIFT-12  // Shift row[0].X
	sll		t1,t1,(16-(GINT_SHIFT-12))
	sll		t3,t3,(16-(GINT_SHIFT-12))
	sll		t5,t5,(16-(GINT_SHIFT-12))
	sll		t7,t7,(16-(GINT_SHIFT-12))
	and		t1,t1,t9                        // Mask t1
	and		t3,t3,t9                        // Mask t3
	and		t5,t5,t9                        // Mask t5
	and		t7,t7,t9                        // Mask t7
	andi	t0,t0,0xffff                // Mask t0
	andi	t2,t2,0xffff                // Mask t0
	andi	t4,t4,0xffff                // Mask t0
	andi	t6,t6,0xffff                // Mask t0
// This section is the only difference between GMatrixTransposeToGTE and GMatrixToGTE......
	or		t0,t0,t3
	or		t2,t2,t5
	or		t4,t4,t7
	or		t6,t6,t1
//............Section ends.
	ctc2	t0,C2_R11R12
	ctc2	t6,C2_R13R21
	ctc2	t4,C2_R22R23
	ctc2	t2,C2_R31R32
	jr	ra
	ctc2	t8,C2_R33
}


void    CPSXEngine::GMatrixToTransposeMATRIX(const GMatrix *gmat, MATRIX *mat)
{
    // Convert a GMatrix to a PSX MATRIX with Transpose

	//!PSX ?? god only knows if this is right...
    SINT t0,t1,t2;

    t0= (gmat->Row[0].X).GetVal() >> (GINT_SHIFT - 12);
    t1= (gmat->Row[0].Y).GetVal() >> (GINT_SHIFT - 12);
    t2= (gmat->Row[0].Z).GetVal() >> (GINT_SHIFT - 12);
    mat->m[0][0] = t0;
    mat->m[0][1] = t1;
    mat->m[0][2] = t2;
    t0= (gmat->Row[1].X).GetVal() >> (GINT_SHIFT - 12);
    t1= (gmat->Row[1].Y).GetVal() >> (GINT_SHIFT - 12);
    t2= (gmat->Row[1].Z).GetVal() >> (GINT_SHIFT - 12);
    mat->m[1][0] = t0;
    mat->m[1][1] = t1;
    mat->m[1][2] = t2;
    t0= (gmat->Row[2].X).GetVal() >> (GINT_SHIFT - 12);
    t1= (gmat->Row[2].Y).GetVal() >> (GINT_SHIFT - 12);
    t2= (gmat->Row[2].Z).GetVal() >> (GINT_SHIFT - 12);
    mat->m[2][0] = t0;
    mat->m[2][1] = t1;
    mat->m[2][2] = t2;
}
//******************************************************************************************
void	CPSXEngine::ReadPSXTextures(CMEMBUFFER &mem)
{


	// get version number
	UINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson

	if (ver == MKID("1.00"))
	{
		SINT 	c0;
        UINT    trmTPX,trmTPY,trmClutX,trmClutY;

		// Get Map textures
		for (c0 = 0; c0 < MAX_MAP_TEXTURES; c0 ++)
		{
			// read data
			STextureRefMap	*tr = &mMapTextures[c0];

            mem.Read(&tr->mX, 4);
            mem.Read(&tr->mY, 4);
            mem.Read(&tr->mW, 4);
            mem.Read(&tr->mH, 4);
            mem.Read(&trmTPX, 4);
            mem.Read(&trmTPY, 4);
            mem.Read(&trmClutX, 4);
            mem.Read(&trmClutY, 4);

	        tr->mX <<= 2;
            tr->mTPAGE = getTPage(0, 1, trmTPX, trmTPY);
            tr->mCLUT  = getClut(trmClutX, trmClutY);
        }

		// Get Other textures
		SINT 	num;
		mem.Read(&num, 4);
		if (num > MAX_OTHER_TEXTURES)
		{
			ASSERT(0);
		}

		for (c0 = 0; c0 < num; c0 ++)
		{
			// read data
			STextureRefOther	*tr;
			SINT				bd, at;

			if (c0 < MAX_OTHER_TEXTURES)
				tr = &mOtherTextures[c0];
			else
				tr = &mOtherTextures[0];

            mem.Read(&tr->mX, 4);
            mem.Read(&tr->mY, 4);
            mem.Read(&tr->mW, 4);
            mem.Read(&tr->mH, 4);
            mem.Read(&trmTPX, 4);
            mem.Read(&trmTPY, 4);
            mem.Read(&trmClutX, 4);
            mem.Read(&trmClutY, 4);
            mem.Read(&bd, 4);
            mem.Read(&at, 4);

	        tr->mX <<= (2 - bd);
            tr->mTPAGE_P05P05 = getTPage(bd, 0, trmTPX, trmTPY);
            tr->mTPAGE_P1P1 = getTPage(bd, 1, trmTPX, trmTPY);
            tr->mTPAGE_P1M1 = getTPage(bd, 2, trmTPX, trmTPY);
            tr->mTPAGE_P1P025 = getTPage(bd, 3, trmTPX, trmTPY);
            tr->mCLUT  = getClut(trmClutX, trmClutY);
        }

		// Get Numbered textures
		mem.Read(&num, 4);
		if (num > MAX_NUMBERED_TEXTURES)
		{
			ASSERT(0);
		}

//		printf("Found %d numbered textures\n", num);

		for (c0 = 0; c0 < num; c0 ++)
		{
			// read data
			STextureRefOther	*tr;
			SINT				bd, at;

			if (c0 < MAX_NUMBERED_TEXTURES)
				tr = &mNumberedTextures[c0];
			else
				tr = &mNumberedTextures[0];

            mem.Read(&tr->mX, 4);
            mem.Read(&tr->mY, 4);
            mem.Read(&tr->mW, 4);
            mem.Read(&tr->mH, 4);
            mem.Read(&trmTPX, 4);
            mem.Read(&trmTPY, 4);
            mem.Read(&trmClutX, 4);
            mem.Read(&trmClutY, 4);
            mem.Read(&bd, 4);
            mem.Read(&at, 4);

	        tr->mX <<= (2 - bd);
            tr->mTPAGE_P05P05 = getTPage(bd, 0, trmTPX, trmTPY);
            tr->mTPAGE_P1P1 = getTPage(bd, 1, trmTPX, trmTPY);
            tr->mTPAGE_P1M1 = getTPage(bd, 2, trmTPX, trmTPY);
            tr->mTPAGE_P1P025 = getTPage(bd, 3, trmTPX, trmTPY);
            tr->mCLUT  = getClut(trmClutX, trmClutY);
        }

		// Get Particle textures
		mem.Read(&num, 4);
		if (num > MAX_PARTICLE_TEXTURES)
		{
			ASSERT(0);
		}

		for (c0 = 0; c0 < num; c0 ++)
		{
			// read data
			STextureRefOther	*tr;
			SINT				bd, at;

			if (c0 < MAX_PARTICLE_TEXTURES)
				tr = &mParticleTextures[c0];
			else
				tr = &mParticleTextures[0];

            mem.Read(&tr->mX, 4);
            mem.Read(&tr->mY, 4);
            mem.Read(&tr->mW, 4);
            mem.Read(&tr->mH, 4);
            mem.Read(&trmTPX, 4);
            mem.Read(&trmTPY, 4);
            mem.Read(&trmClutX, 4);
            mem.Read(&trmClutY, 4);
            mem.Read(&bd, 4);
            mem.Read(&at, 4);

	        tr->mX <<= (2 - bd);
            tr->mTPAGE_P05P05 = getTPage(bd, 0, trmTPX, trmTPY);
            tr->mTPAGE_P1P1 = getTPage(bd, 1, trmTPX, trmTPY);
            tr->mTPAGE_P1M1 = getTPage(bd, 2, trmTPX, trmTPY);
            tr->mTPAGE_P1P025 = getTPage(bd, 3, trmTPX, trmTPY);
            tr->mCLUT  = getClut(trmClutX, trmClutY);
        }


        // read fonts

		mem.Read(&mNumFonts, 4);

		for (c0 = 0; c0 < mNumFonts; c0 ++)
		{
			// Read font stuff
			SINT 	tpx, tpy, clx, cly;

			mem.Read(&mFonts[c0].mX, 4);
			mem.Read(&mFonts[c0].mY, 4);

			mem.Read(&tpx, 4);
			mem.Read(&tpy, 4);
			mem.Read(&clx, 4);
			mem.Read(&cly, 4);

			mFonts[c0].mTPAGE = getTPage(0, 1, tpx, tpy);
			mFonts[c0].mCLUT  = getClut(clx, cly);

			mem.Read(&mFonts[c0].f, sizeof(STCFont));

		}

		// and blit the texture memory.

		RECT	r = {512, 0, 512, 1};
		long templine[256];
		for(c0=0;c0<512;c0++)
		{
			r.y=c0;
			mem.Read((char*)templine,1024);
			//while(DrawSync(1)) /*Do Nothing!*/;
			LoadImage(&r,(ULONG*)templine);
		}
	}
}
SINT    CPSXEngine::GetRenderGameTurn()
{
	return GAME.GetGameTurn();  //! change this for sub-turn interpolation...
}

//******************************************************************************************
void	CPSXEngine::ReadPSXMeshes(CMEMBUFFER &mem)
{
	// get version number
	UINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson

	if (ver == MKID("1.00"))
	{
		SINT	c0;

		mem.Read(&mNumMeshes, 4);

		for (c0 = 0; c0 < mNumMeshes; c0 ++)
		{
			SINT	len, character_number;
			mem.Read(&len, 4);
			mem.Read(&character_number, 4);
			if (GAME.DoWeNeedThisCharacter(character_number))
				mMeshes[c0].Read(mem);
			else
			{
				mMeshes[c0].Clear();
				mem.Skip(len);
			}
		}

		// now read references for default mesh and global meshes
		UINT tmp;
		mem.Read(&mDefaultMesh, 4);
		//mem.Read(&mNumGlobalMeshes, 4);
		mem.Read(&tmp,4);
		mNumGlobalMeshes=tmp;

		mem.Read(mGlobalMeshes, 4 * mNumGlobalMeshes);

		//mem.Read(&mNumNumberedMeshes, 4);
		mem.Read(&tmp,4);
		mNumNumberedMeshes=tmp;
		mem.Read(mNumberedMeshes, 4 * mNumNumberedMeshes);

		ASSERT(mNumGlobalMeshes < MAX_GLOBAL_MESHES);
		ASSERT(mNumNumberedMeshes < MAX_NUMBERED_MESHES)
	}
}

//******************************************************************************************
void	CPSXEngine::ReadPSXRTInfo(CMEMBUFFER &mem)
{
	// get version number
	UINT	ver;
	mem.Read(&ver, 4);




	// and load, based on the verson

	if (ver == MKID("1.00"))
	{
		// read number of refs
		mem.Read(&mNumRenderData, 4);

		// read master index
		//mem.Read(&mThingRef, sizeof(mThingRef));
		for(int i=0;i<MAX_OIDS;i++)
		{
			mem.Read(&ver,4);
			mThingRef[i]=ver;
		}

		// allocate memory for renderdata
		mRenderData = (SPSXRenderData *)PLATFORM.GetLevelPoolData(sizeof(SPSXRenderData) * mNumRenderData);
		ASSERT(mRenderData);

		// and read the data
		mem.Read(mRenderData, sizeof(SPSXRenderData) * mNumRenderData);
	}
}

#endif
