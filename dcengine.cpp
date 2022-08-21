
#include	"Common.h"

#if TARGET == DC

#include	"Globals.h"
#include	"DCDecal.h"
#include	"DCParticleSystem.h"
#include 	"DCLightning.h"
#include	"DCFence.h"
#include	"DCTrail.h"
#include	"DCLaser.h"
#include	"DCLink.h"
#include	"GamePanel.h"
#include	"LevelInfo.h"
#include	"Career.h"

#include	"Profile.h"
#include 	"maincode.h"
#include	<mw_util.h>
#include	"utils2.h"
#include	"Fcoords.h"
#include	"dcjunk.h"
#include	"Text.h"

#include	"lzss.h"
//#pragma optimization_level 0

UBYTE	texture_decompression_buffer[512*512*2];
//UBYTE	texture_decompression_buffer[1];

extern	int	InternalYClip;	

//******************************************************************************************
GVector	CDCEngine::GetEmitter(CThing *t, SINT no)
{
	SINT 	id = t->_GetClassID();
	SINT 	index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SDCRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CDCMesh	*mesh = ENGINE.GetMesh(mesh_no);

				return mesh->GetEmitter(no, t);
			}
		}
	}

	return ZERO_GVECTOR;
}

//******************************************************************************************
GVector	CDCEngine::GetEmitterAnimated(CThing *t, SINT no)
{
	SINT 	id = t->_GetClassID();
	SINT 	index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SDCRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CDCMesh	*mesh = ENGINE.GetMesh(mesh_no);

				return mesh->GetEmitterAnimated(no, t);
			}
		}
	}

	return ZERO_GVECTOR;
}

GINT	CDCEngine::GetFrameIncrement(SINT id, EAnimMode a, SINT* realindex)
{
	SINT index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SDCRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CDCMesh	*mesh = ENGINE.GetMesh(mesh_no);

				return mesh->GetFrameIncrement(a, realindex);
			}
		}
	}

	*realindex = 0;
	return G0;
}

//******************************************************************************************
CCollisionVolume 	*CDCEngine::GetCollisionVolume(CThing *t)
{
	if (!t)
		return NULL;

	SINT	id = t->_GetClassID();
	SINT 	index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SDCRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CDCMesh	*mesh = ENGINE.GetMesh(mesh_no);

				mesh->EvaluateCollisionVolume(t);
				return mesh->GetCollisionVolume();
			}
		}
	}

	return NULL;
}

//******************************************************************************************
GINT 	CDCEngine::GetRenderThingRadius(CThing *t)
{
	if (!t)
		return G1;

	SINT	id = t->_GetClassID();
	SINT 	index = mThingRef[id];

	ASSERT(index != -1);

	if (index != -1)
	{
		SDCRenderData	*rd = &mRenderData[index];

		if (rd->mRTIDs[0] == RTID_CRTMesh)
		{
			SINT	mesh_no = rd->mRTInfos[0].i0;

			if (mesh_no >= 0)
			{
				CDCMesh	*mesh = ENGINE.GetMesh(mesh_no);

				return mesh->mRadius;
			}
		}
	}

	return G1;
}

//******************************************************************************************

void	CDCEngine::RenderThings()
{

    // JCL - dodgy gamut - do a rectangle around a point "behind" the camera
//#define	BEHIND_DIST		5
//#define	GAMUT_RADIUS	18
#define	BEHIND_DIST		10
#define	GAMUT_RADIUS	23

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
						SDCRenderData	*rd = &mRenderData[index];

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
										CDCMesh::RenderFromData(t, &rd->mRTInfos[c0]);
			                            break;
									
			                        case RTID_CRTDecal:
										CDCDecal::RenderFromData(t, rd->mRTInfos[c0]);
			                             break;
									case RTID_CRTParticleSystem :
										CDCParticleSystem::RenderFromData(t, rd->mRTInfos[c0]);
										break;										
									case RTID_CRTLightning:
										CDCLightning::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									case RTID_CRTFence:
										CDCFence::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									case RTID_CRTTrail:
										CDCTrail::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									case RTID_CRTLaser:
										CDCLaser::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									case RTID_CRTLink:
										CDCLink::RenderFromData(t, rd->mRTInfos[c0]);
										break;
									

									default:
										{
										
											// SDS put some Default sprite drawing code in here...
											//!PSX just draw a small decal
											// SDS also convert G's to F's, just make the whole damn thing FPU
											// We have a camera
											// we have an object - just a vector representing a point
																						
											GVector	pos = t->GetRenderPos();
											FVector npos = GVectorToFVector(pos);
											
											tvector in,out;
											
											in.x=npos.X;
											in.y=npos.Z;
											in.z=npos.Y;
											//PersTrans(1,cam,world,&in,&out);
											
											FVector WorldPos = FVector(0.0f,0.0f,0.0f);
											FMatrix WorldOri = ID_FMATRIX;

											PersTransWrapper(1,(tvector *)&in,(tvector *)&out,WorldPos,WorldOri);
											
											#define DECAL_SIZE	5
											//if (out.z<0)
											GTri(out.x- DECAL_SIZE,out.y- DECAL_SIZE,out.x+ DECAL_SIZE,out.y- DECAL_SIZE,out.x,out.y+ DECAL_SIZE);
											

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
#define SKYSIZE	512.f
//#define SKYSIZE	16.f

void	CDCEngine::RenderSkyCube()
{
#if 1
	SINT	c0;
	
	SINT	pris;
	if (GAME.IsMultiPlayer())
		pris = CLevelStructure::GetPrisonForMultiplayerLevel(MULTIPLAYER_CAREER.mCurrentTournament, MULTIPLAYER_CAREER.mCurrentArena);
	else
		pris = CAREER.mCurrentPrison;

	BOOL	flip = (!((pris == 1) || (pris == 4)));
	
	for (c0 = 0; c0 < 5; c0 ++)
	{
		STextureRefOther *tr;
		FVector x, y, o;

		switch (c0)
		{
		case 0:
			tr = GetNumberedTexture(flip ? TN_SKY_UP : TN_SKY_DOWN);
			x = FVector(SKYSIZE, 0, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(0, SKYSIZE, 0);
			break;
		case 1:
			tr = GetNumberedTexture(flip ? TN_SKY_DOWN: TN_SKY_UP);
			x = FVector(-SKYSIZE, 0, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(0, -SKYSIZE, 0);
			break;
		case 2:
			tr = GetNumberedTexture(flip ? TN_SKY_RIGHT : TN_SKY_LEFT);
			x = FVector(0, -SKYSIZE, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(SKYSIZE, 0, 0);
			break;
		case 3:
			tr = GetNumberedTexture(flip ? TN_SKY_LEFT: TN_SKY_RIGHT);
			x = FVector(0, SKYSIZE, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(-SKYSIZE, 0, 0);
			break;
		case 4:
			tr = GetNumberedTexture(TN_SKY_CENT);
			x = FVector(-SKYSIZE, 0, 0);
			y = FVector(0, SKYSIZE, 0);
			o = FVector(0, 0, -SKYSIZE);
			
			if (!flip)
			{
				x = -x;
				y = -y;
			}
			break;
		};
		
		if (!tr)
			return;
	
		SINT	TN = tr->mTPage + 1;
		
		kmChangeStripMipmapAdjust		( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MIPMAP_D_ADJUST_0_25 );    		
		kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
	 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
	

		FVector	p = GVectorToFVector(GetCamera()->mPos);
	
		FVector	verts[2][2];
		
		verts[0][0] = -x - y + o;
		verts[0][1] =  x - y + o;
		verts[1][0] =  x + y + o;
		verts[1][1] = -x + y + o;
		
		ENGINE.SwapYZ((FVector *)verts, 4);
		
		//PersTransWrapper(4,(tvector *)verts, (tvector *)verts, p , ID_FMATRIX);
		PersTransWrapper2(4,(tvector *)verts, (tvector *)verts, p , ID_FMATRIX);
	
		float	u0 = tr->mX + 0.004f;
		float	u1 = tr->mX + tr->mW - 0.004f;
		float	v0 = tr->mY + 0.004f;
		float	v1 = tr->mY + tr->mH - 0.004f;
		
		DWORD	col = 0xffffff;
		
	   	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	   	TextureS[0].fX                   = verts[0][0].X;
	   	TextureS[0].fY                   = verts[0][0].Y;
	   	TextureS[0].u.fZ                 = verts[0][0].Z;
	   	TextureS[0].uBaseRGB.dwPacked    = col;
	   	TextureS[0].fU                   = u0;
	   	TextureS[0].fV                   = v1;
	
	   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	   	TextureS[1].fX                   = verts[0][1].X;
	   	TextureS[1].fY                   = verts[0][1].Y;
	   	TextureS[1].u.fZ                 = verts[0][1].Z;
	   	TextureS[1].uBaseRGB.dwPacked    = col;
	   	TextureS[1].fU                   = u1;
	   	TextureS[1].fV                   = v1;
	
	   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	   	TextureS[2].fX                   = verts[1][0].X;
	   	TextureS[2].fY                   = verts[1][0].Y;
	   	TextureS[2].u.fZ                 = verts[1][0].Z;
	   	TextureS[2].uBaseRGB.dwPacked    = col;
	   	TextureS[2].fU                   = u1;
	   	TextureS[2].fV                   = v0
	   	;
	   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
	   	TextureS[3].fX                   = verts[1][1].X;
	   	TextureS[3].fY                   = verts[1][1].Y;
	   	TextureS[3].u.fZ                 = verts[1][1].Z;
	   	TextureS[3].uBaseRGB.dwPacked    = col;
	   	TextureS[3].fU                   = u0;
	   	TextureS[3].fV                   = v0;
		
		
		//*********************************************
		// SDS Clip Away
			
		KMVERTEX_03 TempArray[10];
			
		int lc=4;
		int nvc=0;
		float ZV = -0.1f;
		for (int lc=0;lc<4;lc++)
		{
			KMVERTEX_03 V1=TextureS[(lc)]; 
			KMVERTEX_03 V2=TextureS[(lc+1) % 4];		 
	
			// aaaaaaarghhh
			extern BOOL ClipToZ(float z,KMVERTEX_03 *lp1,KMVERTEX_03 *lp2);
			
			ClipToZ(ZV,&V1,&V2);
			
			if (V1.u.fZ<=ZV)
			{
				if (nvc==0)
				{	    
					TempArray[nvc]=V1;
					nvc++;
				}
				else
				{
					if ((TempArray[nvc-1].u.fZ!=V1.u.fZ) || (TempArray[nvc-1].fX!=V1.fX) || (TempArray[nvc-1].fY!=V1.fY))
					{
						TempArray[nvc]=V1;
						nvc++;
					}
				}
			}
			if (V2.u.fZ<=ZV)
			{
				if (nvc==0)
				{		    
					TempArray[nvc]=V2;
					nvc++;
				}
				else
				{
					if ((TempArray[nvc-1].u.fZ!=V2.u.fZ) || (TempArray[nvc-1].fX!=V2.fX) || (TempArray[nvc-1].fY!=V2.fY))
					{		   
						TempArray[nvc]=V2;
						nvc++;
					}
				}
			}
		}
			
		// SDS Perspective Transform	 
		
		for (int lc=0;lc<nvc;lc++)
		{
			TempArray[lc].u.fZ=(1/TempArray[lc].u.fZ);
			TempArray[lc].fX= (((-TempArray[lc].fX*320*(2))*TempArray[lc].u.fZ)+InternalViewOffset);
			TempArray[lc].fY=((TempArray[lc].fY*320*(2))*TempArray[lc].u.fZ)+240;		 
			TempArray[lc].u.fZ=(TempArray[lc].u.fZ)*10;		 
		}
		
		// SDS Fan the bastard!!!!
		
		if (nvc>=3)
		{
			for (int a=1;a<=nvc-2;a++)
			{
				GTriZT(TN,
				TempArray[0].fX,TempArray[0].fY,
				TempArray[a].fX,TempArray[a].fY,
				TempArray[a+1].fX,TempArray[a+1].fY,
				TempArray[0].u.fZ,
				TempArray[a].u.fZ,
				TempArray[a+1].u.fZ,
				TempArray[0].fU,TempArray[0].fV,
				TempArray[a].fU,TempArray[a].fV,
				TempArray[a+1].fU,TempArray[a+1].fV);
			}
		}
			
	//	ExitCZTs(4);
	
	 }
#endif
}

//******************************************************************************************
void	render_mem_card_removed()
{
	char 	s[500], s2[500];
	SINT	sl;
	
	SINT	y = 160;
	SINT	c0;
	
	for (c0 = 0; c0 < 7; c0 ++)
	{
		SINT col = 0xffffffff;
		
		switch (c0)
		{
		case 0:	
			CText::GetString(TEXT_DC_CARD_REMOVED1, s);
			break;
		case 1:
			CText::GetString(TEXT_DC_CARD_REMOVED2, s);
			break;
		case 2:
			CText::GetString(TEXT_DC_PORT_SOCKET2, s2);
			sprintf(s, s2, CAREER.mMemController + 'A', CAREER.mMemSlot + 1);
			break;
		case 3:
			CText::GetString(TEXT_DC_CARD_REMOVED3, s);
			y += 30;
			break;
		case 4:
			CText::GetString(TEXT_DC_CARD_REMOVED4M, s);	  // JCLDCNOPSX - BALLBREAKERS
			break;
		case 5:
			CText::GetString(TEXT_DC_NO_SAVE1, s);
			y += 30;
			break;
		case 6:
			CText::GetString(TEXT_DC_NO_SAVE2, s);
			break;
		};
		
		sl = ENGINE.GetStringSize(2, s);
		ENGINE.DrawString(2, s, 320 - sl / 2, y, col, 0);
		y += 18;
	}
}

//******************************************************************************************
void	CDCEngine::Render(CCamera *cam , CCamera *cam2)
{
	CCamera	interpolated_cameras[2];
	
	if (cam)
	{
		interpolated_cameras[0] = *cam;
		interpolated_cameras[0].Interpolate();	
		mViewportCameras[0] = &interpolated_cameras[0];
	}
	else
		mViewportCameras[0] = NULL;

	if (cam2)
	{
		interpolated_cameras[1] = *cam2;
		interpolated_cameras[1].Interpolate();	
		mViewportCameras[1] = &interpolated_cameras[1];
	}
	else
		mViewportCameras[1] = NULL;


	// setup data
//	mViewportCameras[0] = cam;
//	mViewportCameras[1] = cam2;


	int views=1;
	if(cam2)
	{
		views++;
		PLATFORM.SetScreenWidth(320);
	}
	else
	{
		PLATFORM.SetScreenWidth(640);
	}

	// Clear Ordertable
    int safe;

	{
		PLATFORM.ClearDebugVSync();
		do
		{
			if (PLATFORM.GetDebugVSync() > 200)
			{
//				printf("*** Drawsync missed.  Aborting...***\n");
				GAME.SetQuitFlag(GQ_FRONTEND);
				return;
			}
			//safe = PLATFORM.GeometryMarkBeginScene();
			safe = 0;
		} while (safe == -1);
	}

	if(safe!=-1)
	{
		if (GAME.HasMemoryCardBeenRemoved())
		{
			SDSSwitchClippingOff();				
			SDSRenderStart();			
			SDSSetClipRegion(0,0,640,InternalYClip);			

			render_mem_card_removed();
		
			SDSRenderFlip();	
		}
		else
		{
			if (views==1)
			{
				int f=safe;
				//SDSSetClipRegion(0,0,640,480);	
				SDSSwitchClippingOff();				
				SDSRenderStart();			
				SDSSetClipRegion(0,0,640,InternalYClip);			
			
	
				{
					PROFILE_FN(RMDrawPanel);
					GAMEPANEL.Render();
	     			RenderPanel();	
	     		}
				int cnt=0;
				{
					mCurrentCamera=mViewportCameras[cnt];      // Setup for correct camera view
					PLATFORM.SetViewport(cnt);          // Only used to indicate which DMA, Camera etc...
					//SDSRenderNoFlip();
				}
				{
					PROFILE_FN(RenderThings);
					RenderThings();
				}
				{
					PROFILE_FN(RenderMap);
					MAP.Render();
				}
				RenderSkyCube();
				SDSRenderFlip();	
			}
			
			if (views==2)
			{
				int f=safe;
				SDSSwitchClippingOn();		
				kmSetRegionClipping((0)>>5,InternalYClip,(320)>>5,(480)>>5);
				kmSetPixelClipping(0,0,320,InternalYClip);
				SDSRenderStart();
				SDSSetClipRegion(0,0,320,InternalYClip);	
				mCurrentCamera=mViewportCameras[0];      // Setup for correct camera view
				PLATFORM.SetViewport(0);          // Only used to indicate which DMA, Camera etc...
				
	
				{
				PROFILE_FN(RMDrawPanel);
				GAMEPANEL.Render();
		     	RenderPanel();	
		     	}
					
				{
				PROFILE_FN(RenderThings);
				RenderThings();
				}
					
				{
				PROFILE_FN(RenderMap);
				MAP.Render();
				}
				RenderSkyCube();
				
				SDSRenderNoFlipClip(320,0,640,InternalYClip);
				SDSSetClipRegion(320,0,640,InternalYClip);
				mCurrentCamera=mViewportCameras[1];      // Setup for correct camera view
				PLATFORM.SetViewport(1);          // Only used to indicate which DMA, Camera etc...
				
				{
				PROFILE_FN(RMDrawPanel);
				GAMEPANEL.Render();
		     	RenderPanel();	
		     	}
					
				{
				PROFILE_FN(RenderThings);
				RenderThings();
				}
					
				{
				PROFILE_FN(RenderMap);
				MAP.Render();
				}
				RenderSkyCube();
	
	
	
				SDSRenderFlip();
				
		
			}
		}
	}

		//SDSSetClipRegion(0,0,640,480);	
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
#ifdef JCLDCNOPSX			// Not Needed - MATRIX Type should be redundant
void    CDCEngine::GMatrixToMATRIX(const GMatrix *gmat, MATRIX *mat)
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
#endif



asm void GMatrixToGTELight(const GMatrix *gmat)
{
#if defined(JJS)
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
#endif
}




asm void CDCEngine::GMatrixToGTE(const GMatrix *gmat)
{

//Aaaaaaarrrrrrrrghhhhhhhhhhhhhhhhhhhhh




#if defined(JJS)
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
#endif
}

asm void CDCEngine::GMatrixTransposeToGTE(const GMatrix *gmat)
{
#if defined(JJS)
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
#endif
}


#ifdef JCLDCNOPSX			// Not Needed - MATRIX Type should be redundant
void    CDCEngine::GMatrixToTransposeMATRIX(const GMatrix *gmat, MATRIX *mat)
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
#endif

//******************************************************************************************
void	CDCEngine::ReadPSXTextures(CMEMBUFFER &mem)
{

	//mw_pr("Started Loading Textures");
	// get version number
	UINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson

	if (ver == MKID("1.00"))
	{
		SINT 	c0;
        UINT    trmTPX,trmTPY,trmClutX,trmClutY;
        
		//******************************************************************************************
		//***************************** Get Map textures *******************************************
		//******************************************************************************************
		
		for (c0 = 0; c0 < MAX_MAP_TEXTURES; c0 ++)
		{
			// read data
			STextureRefMap	*tr = &mMapTextures[c0];

            mem.Read(&tr->mX, 4);
            mem.Read(&tr->mY, 4);
            mem.Read(&tr->mW, 4);
            mem.Read(&tr->mH, 4);
        	mem.Read(&tr->mTPage, 4);            
        	
            //SDS mem.Read(&trmTPX, 4);
            //SDS mem.Read(&trmTPY, 4);
            //SDS mem.Read(&trmClutX, 4);
            //SDS mem.Read(&trmClutY, 4);

	        //SDS tr->mX <<= 2;
            //SDS tr->mTPAGE = getTPage(0, 1, trmTPX, trmTPY);
            //SDS tr->mCLUT  = getClut(trmClutX, trmClutY);
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

            /* SDS
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
            */

			            
            mem.Read(&tr->mX, 4);
            mem.Read(&tr->mY, 4);
            mem.Read(&tr->mW, 4);
            mem.Read(&tr->mH, 4);
        	mem.Read(&tr->mTPage, 4);            
        	            
			/*
	        tr->mX <<= (2 - bd);
            tr->mTPAGE_P05P05 = getTPage(bd, 0, trmTPX, trmTPY);
            tr->mTPAGE_P1P1 = getTPage(bd, 1, trmTPX, trmTPY);
            tr->mTPAGE_P1M1 = getTPage(bd, 2, trmTPX, trmTPY);
            tr->mTPAGE_P1P025 = getTPage(bd, 3, trmTPX, trmTPY);
            tr->mCLUT  = getClut(trmClutX, trmClutY);
            */
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
        	mem.Read(&tr->mTPage, 4); 
        	/*				

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
            */
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
        	mem.Read(&tr->mTPage, 4); 				
			/*
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
            */
        }


        // read fonts

		mem.Read(&mNumFonts, 4);

		for (c0 = 0; c0 < mNumFonts; c0 ++)
		{
			// Read font stuff
			SINT 	tpx, tpy, clx, cly, TPage;

			mem.Read(&mFonts[c0].mX, 4);
			mem.Read(&mFonts[c0].mY, 4);
			mem.Read(&mFonts[c0].mTPage, 4);
			mem.Read(&mFonts[c0].f, sizeof(STCFont));

		}
		long templine[256];

		//SDS New Format Loader
		
		FailedTextureCount=0;
		mem.Read(&NumTPages,4);
		//mw_pr("Loading Textures");
		
		SINT sizex,sizey,flags;
		
		for (int Ntl=0;Ntl<NumTPages;Ntl++)
		{
			mem.Read(&sizex,4);
			mem.Read(&sizey,4);
			mem.Read(&flags,4);
			// Stuff for VQ e.t.c.
			int ByteSize=sizex*sizey*2;
			if ((flags & 2) ==2 ) 
			{
	          	mem.Read(&ByteSize,4);						
	 	    	mem.Read((char*)&TextureLoadBucket,ByteSize);          		
	        }
	        else
	        {	
	          	if (flags & 8)
	          	{
	          		// compressed!
	          		SINT	size;
	          		mem.Read(&size, 4);
	          		mem.Read(&texture_decompression_buffer, size);
					
					lzss_decompress((unsigned char *)texture_decompression_buffer,
								    (unsigned char *)TextureLoadBucket, size);
	          	}
	          	else	          	
	 	    		mem.Read((char*)&TextureLoadBucket,sizex*sizey*2);
	 	    }
	 	    CreateTexturePage(Ntl+1,flags, sizex, sizey);
		}
		
		// and blit the texture memory.
		/*
		RECT	r = {512, 0, 512, 1};

		
		for(c0=0;c0<512;c0++)
		{
			r.y=c0;
			mem.Read((char*)templine,1024);  // SDS
			LoadImage(&r,(ULONG*)templine);
		}
		*/
		/*
		char *SDSTemp;

		SDSTemp=(char*)_Align32Malloc(1024*512);
		
		mem.Read((char*)SDSTemp,1024*512);

		R555ToR565((UWORD *)SDSTemp,1024*256);
		PtrToScreen(SDSTemp,1024,256);
		for (int j=0;j<1000;j++)
		{
		for (int i=0;i<10;i++)
  						{
  						RenderStart();	  	
						GTQuad(0,0,639,0,0,479,639,479,1.0f);
												RenderFlip();		
						}
								
		}				
		for	(int j=1;j<1000;j++)
		{			
		RenderStart();    	
		float sx,sy;
    	for (int loop=0;loop<=5;loop++)
        	{
 			sx=(float)(rand()%640);
			sy=(float)(rand()%480);        
    		GTri(320,240,sx+50,sy,sx+50,sy+50);
        	}
       	
		RenderFlip();						
		
		Align32Free(SDSTemp);
		
		}
		*/
				
		// SDS
		
	}
}
SINT    CDCEngine::GetRenderGameTurn()
{
	return GAME.GetGameTurn();  //! change this for sub-turn interpolation...
}

//******************************************************************************************
void	CDCEngine::ReadPSXMeshes(CMEMBUFFER &mem)
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
void	CDCEngine::ReadPSXRTInfo(CMEMBUFFER &mem)
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
		mRenderData = (SDCRenderData *)PLATFORM.GetLevelPoolData(sizeof(SDCRenderData) * mNumRenderData);
		ASSERT(mRenderData);

		// and read the data
		mem.Read(mRenderData, sizeof(SDCRenderData) * mNumRenderData);
	}
}

//*******************************************************************8
void	CDCEngine::SwapYZ(FVector *v, SINT num)
{
	SINT 	c0;
	
	for (c0 = 0; c0 < num; c0 ++)
	{
		float foo = v[c0].Z;
		v[c0].Z = v[c0].Y;
		v[c0].Y = foo;
	}
}


#endif
