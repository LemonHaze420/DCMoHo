#include	"Common.h"

#if TARGET == DC

#include	"Globals.h"
#include	"Profile.h"
#include 	"maincode.h"
#pragma peephole on

#include 	"DC3dengine.h"
#include	"DCParticleSystem.h"
#include 	"dcjunk.h"
#include 	"LevelInfo.h"
#include	"Career.h"

#include	<memory.h>

void asm MakeColoursCommon(int count);

#define WATERZ (-10)
#define LANDZ	(+10)
#define SHADOWZ (-4)

void	CachedPersTrans(SINT vn, tvector *in, tvector *out, SINT stride);

//******************************************************************************************
FVector 	WaterVerts[4];
tvector     Mappy[8192];
tvector     Trans[8192];
int			MixEE[8192][3];
int			Flaggy[8192];
float     	sheight[8192];
int 		TextureNumber[8192][3];
int  		DColours[8192];
BOOL		FFlags[64 * 128];

//******************************************************************************************
//** Preprepared strip
KMVERTEX_03         MapVertices[50];
KMVERTEX_03         MapVertices2[12];
KMVERTEX_03         MapVertices3[4];

//******************************************************************************************

inline	DWORD	modulate_colour(DWORD col1, DWORD col2)
{
	//return col1;
	//return col1;
	SINT r = (((col1 >> 16) & 0xff) * ((col2 >> 16) & 0xff)) >> 8;
	SINT g = (((col1 >>  8) & 0xff) * ((col2 >>  8) & 0xff)) >> 8;
	SINT b = (((col1      ) & 0xff) * ((col2      ) & 0xff)) >> 8;
	
	return (r << 16) + (g << 8) + b;

}
	

void MapDrawBottomEdge(void)
{
	CDCMesh *meshes[4] = { ENGINE.GetNumberedMesh(NM_STADIUM_WALL1),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL2),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL3),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL4)};

	FVector pos=ZERO_FVECTOR;
	FMatrix ori=ID_FMATRIX;
	int x, pv = 0;
    SINT lookx = ENGINE.GetCamera()->mLookAt.X.Whole();
	//if(MAP_WIDTH==32)lookx=16;
	//if(lookx<=(MAP_WIDTH>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	pv += 7;
	for(x=(MAP_WIDTH>>3) - 1;x>0;x--)
	{
		pos.X=(pv);
		meshes[(x - 1) & 3]->RenderAtPos(pos, ori);
		pv += 8;
	}
	pos.X=(pv);
	//if(lookx>=(MAP_WIDTH>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_CORNER)->RenderAtPos(pos,ori);
}

void MapDrawTopEdge(void)
{
	CDCMesh *meshes[4] = { ENGINE.GetNumberedMesh(NM_STADIUM_WALL1),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL2),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL3),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL4)};

	FVector pos=ZERO_FVECTOR;
	FMatrix ori;
	ori.MakeRotationYaw(PI /*G_PI*/);
	int x, pv = MAP_WIDTH - 1;
	pos.Y=((MAP_HEIGHT - 1));

	pos.X=(pv);
    SINT lookx = ENGINE.GetCamera()->mLookAt.X.Whole();
	//if(MAP_WIDTH==32)lookx=16;
	//if(lookx>=(MAP_WIDTH>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	pv -= 7;

	for(x=(MAP_WIDTH>>3) - 1;x>0;x--)
	{
		pos.X=(pv);
		meshes[(x - 1) & 3]->RenderAtPos(pos, ori);
		pv -= 8;
	}
	pos.X=(pv);
	//if(lookx<=(MAP_WIDTH>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_CORNER)->RenderAtPos(pos,ori);
}

void MapDrawRightEdge(void)
{
	CDCMesh *meshes[4] = { ENGINE.GetNumberedMesh(NM_STADIUM_WALL1),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL2),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL3),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL4)};

	FVector pos=ZERO_FVECTOR;
	FMatrix ori;
	ori.MakeRotationYaw(PI/2 /*G_PI_2*/);
	int y, pv = 0;
	pos.X=((MAP_WIDTH - 1));

    SINT looky = ENGINE.GetCamera()->mLookAt.Y.Whole();
	//if(MAP_HEIGHT==32)looky=16;
	//if(looky<=(MAP_HEIGHT>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	pv += 7;

	for(y=(MAP_HEIGHT>>3) - 1;y>0;y--)
	{
		pos.Y=(pv);
		meshes[(y - 1) & 3]->RenderAtPos(pos, ori);
		pv += 8;
	}
	pos.Y=(pv);
	//if(looky>=(MAP_HEIGHT>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_CORNER)->RenderAtPos(pos,ori);
}


void MapDrawLeftEdge(void)
{
	CDCMesh *meshes[4] = {  ENGINE.GetNumberedMesh(NM_STADIUM_WALL1),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL2),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL3),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL4)};

	FVector pos=ZERO_FVECTOR;
	FMatrix ori;
	
	ori.MakeRotationYaw(-PI/2 /*-G_PI_2*/);
	int y, pv = MAP_HEIGHT - 1;

	pos.Y=(pv);
    SINT looky = ENGINE.GetCamera()->mLookAt.Y.Whole();
	//if(MAP_HEIGHT==32)looky=16;
	//if(looky>=(MAP_HEIGHT>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	pv -= 7;

	for(y=(MAP_HEIGHT>>3) - 1;y>0;y--)
	{
		pos.Y=(pv);
		meshes[(y - 1) & 3]->RenderAtPos(pos, ori);
		pv -= 8;
	}
	pos.Y=(pv);
	//if(looky<=(MAP_HEIGHT>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_CORNER)->RenderAtPos(pos,ori);
}

//*************************************************************************
void	CDCMap::DrawCellType1(SINT x, SINT y, SINT type)
{
	// JCL - draw booster square or whatever.
	// Slow - but doesn't happen that often
	
	STextureRefOther 	*tr =ENGINE.GetNumberedTexture(type + TN_ICE-1);

	SINT	sqx, sqy;
	
	sqx = x & 3;
	sqy = 3 - (y & 3);
	
	if ((type >= CT_BOOSTR) && (type <= CT_BOOSTD))
	{
		// animate
		SINT turn = GAME.GetGameTurn();

		if ((turn & 2) && (!(turn & 8)))
			sqx |= 2;
		else
			sqx &= ~2;

		if ((turn & 4) && (!(turn & 8)))
			sqy |= 2;
		else
			sqy &= ~2;
	}

	float	us, vs, tw, th;
	float	u0, u1, v0, v1;
	
	us = tr->mX + 0.002f;
	vs = tr->mY + 0.002f;
	tw = (tr->mW - 0.002f) / 4.f;
	th = (tr->mH - 0.002f) / 4.f;

	u0 = us + (tw * sqx);
	u1 = us + (tw * (sqx + 1));
	v0 = vs + (th * sqy);
	v1 = vs + (th * (sqy + 1));
	
	SINT	TN = tr->mTPage + 1;
	
	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
	kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );

	InitCZTs(TN);
	
	#define Z_FUDGE (0.002f)
	
	SINT	ref = x + (y * mMapWidth);

	CachedPersTrans(1, &Mappy[ref], &Trans[ref], 1);
  	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[0].fX                   =  Trans[ref].x;
   	TextureS[0].fY                   =  Trans[ref].y;
   	TextureS[0].u.fZ                 = -Trans[ref].z + Z_FUDGE;
   	TextureS[0].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[0].fU                   = u0;
   	TextureS[0].fV                   = v1;
   	
	ref += mMapWidth ;
	CachedPersTrans(1, &Mappy[ref], &Trans[ref], 1);
   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].fX                   =  Trans[ref].x;
   	TextureS[1].fY                   =  Trans[ref].y;
   	TextureS[1].u.fZ                 = -Trans[ref].z + Z_FUDGE;
   	TextureS[1].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[1].fU                   = u0;
   	TextureS[1].fV                   = v0;   	

	ref -=mMapWidth -1;
	CachedPersTrans(1, &Mappy[ref], &Trans[ref], 1);
   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].fX                   =  Trans[ref].x;
   	TextureS[2].fY                   =  Trans[ref].y;
   	TextureS[2].u.fZ                 = -Trans[ref].z + Z_FUDGE;
   	TextureS[2].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[2].fU                   = u1;
   	TextureS[2].fV                   = v1;

	ref +=mMapWidth;
	CachedPersTrans(1, &Mappy[ref], &Trans[ref], 1);
   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	TextureS[3].fX                   =  Trans[ref].x;
   	TextureS[3].fY                   =  Trans[ref].y;
   	TextureS[3].u.fZ                 = -Trans[ref].z + Z_FUDGE;
   	TextureS[3].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[3].fU                   = u1;
   	TextureS[3].fV                   = v0;


	ExitCZTs(4);
}

void	CDCMap::DrawCellType2(SINT x, SINT y, SINT type)
{
	// JCL - draw booster square or whatever.
	// Slow - but doesn't happen that often
	
	STextureRefOther 	*tr =ENGINE.GetNumberedTexture(type + TN_ICE-1);

	SINT	sqx, sqy;
	
	sqx = x & 3;
	sqy = 3 - (y & 3);
	
	if ((type >= CT_BOOSTR) && (type <= CT_BOOSTD))
	{
		// animate
		SINT turn = GAME.GetGameTurn();

		if ((turn & 2) && (!(turn & 8)))
			sqx |= 2;
		else
			sqx &= ~2;

		if ((turn & 4) && (!(turn & 8)))
			sqy |= 2;
		else
			sqy &= ~2;
	}

	float	us, vs, tw, th;
	float	u0, u1, v0, v1;
	
	us = tr->mX + 0.002f;
	vs = tr->mY + 0.002f;
	tw = (tr->mW - 0.002f) / 4.f;
	th = (tr->mH - 0.002f) / 4.f;

	u0 = us + (tw * sqx);
	u1 = us + (tw * (sqx + 1));
	v0 = vs + (th * sqy);
	v1 = vs + (th * (sqy + 1));
	
	SINT	TN = tr->mTPage + 1;
	
	kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );    
	kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );

	InitCZTs(TN);
	
	#define Z_FUDGE (0.002f)
	
	SINT	ref = x + ((y + 1) * mMapWidth);

	CachedPersTrans(1, &Mappy[ref], &Trans[ref], 1);
  	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[0].fX                   =  Trans[ref].x;
   	TextureS[0].fY                   =  Trans[ref].y;
   	TextureS[0].u.fZ                 = -Trans[ref].z + Z_FUDGE;
   	TextureS[0].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[0].fU                   = u0;
   	TextureS[0].fV                   = v0;
   	
	ref -= mMapWidth ;
	CachedPersTrans(1, &Mappy[ref], &Trans[ref], 1);
   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].fX                   =  Trans[ref].x;
   	TextureS[1].fY                   =  Trans[ref].y;
   	TextureS[1].u.fZ                 = -Trans[ref].z + Z_FUDGE;
   	TextureS[1].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[1].fU                   = u0;
   	TextureS[1].fV                   = v1;   	

	ref +=mMapWidth + 1;
	CachedPersTrans(1, &Mappy[ref], &Trans[ref], 1);
   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].fX                   =  Trans[ref].x;
   	TextureS[2].fY                   =  Trans[ref].y;
   	TextureS[2].u.fZ                 = -Trans[ref].z + Z_FUDGE;
   	TextureS[2].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[2].fU                   = u1;
   	TextureS[2].fV                   = v0;

	ref -=mMapWidth;
	CachedPersTrans(1, &Mappy[ref], &Trans[ref], 1);
   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	TextureS[3].fX                   =  Trans[ref].x;
   	TextureS[3].fY                   =  Trans[ref].y;
   	TextureS[3].u.fZ                 = -Trans[ref].z + Z_FUDGE;
   	TextureS[3].uBaseRGB.dwPacked    = 0xffffffff;
   	TextureS[3].fU                   = u1;
   	TextureS[3].fV                   = v1;


	ExitCZTs(4);
}


//*************************************************************************
//*************************************************************************
//Check out the 'function' dropdown menu in CodeWarrior (the one with the two braces) for this file....

#pragma mark --ooo==============ooo--
#pragma mark !!!!SUPRISE SURPRISE!!!!
#pragma mark --ooo==============ooo--

extern void	Trigger_Vib(int Vib_Num);

#define SSCF 30
//#define SSCF 0
//#define SSCF (-100)

//#pragma optimization_level 4
//#pragma dont_inline on
void CDCMap::DcRender4x4(int x,int y)
{
	PROFILE_FN(RMDraw4x4);

    SINT	c0, c1;
	// Trivial Reject
	
	int		zam = (x<<2) +((y<<2)*mMapWidth);
	int		ozam=zam; 
	int		flags=FFlags[x + (y * (mMapWidth >> 2))];

	float 	x1 = Trans[zam].x;
	float 	y1 = Trans[zam].y;
	float	z1 = Trans[zam].z;
	
	float 	x2 = Trans[zam+4].x;
	float 	y2 = Trans[zam+4].y;
	float  	z2 = Trans[zam+4].z;   
	
	float 	x3 = Trans[zam+(mMapWidth*4)].x;
	float 	y3 = Trans[zam+(mMapWidth*4)].y;
	float  	z3 = Trans[zam+(mMapWidth*4)].z;   
	
	float 	x4 = Trans[zam+4+(mMapWidth*4)].x;
	float 	y4 = Trans[zam+4+(mMapWidth*4)].y;
	float  	z4 = Trans[zam+4+(mMapWidth*4)].z;
	
	//***************************************************
	// Some Extra stuff to stop the  bad clipping problem
	//***************************************************
	
	float 	x5 = Trans[zam+2].x;
	float 	y5 = Trans[zam+2].y;
	
//	float 	x6 = Trans[zam+(mMapWidth*2)].x;
//	float 	y6 = Trans[zam+(mMapWidth*2)].y;	
	
//	float 	x7 = Trans[zam+4+(mMapWidth*2)].x;
//	float 	y7 = Trans[zam+4+(mMapWidth*2)].y;
	
//	float 	x8 = Trans[zam+2+(mMapWidth*4)].x;
//	float 	y8 = Trans[zam+2+(mMapWidth*4)].y;	
	
	if ((x5>(IX1-SSCF) && x5<(IX2+SSCF)) ||  (x1>(IX1-SSCF) && x1<(IX2+SSCF)) || (x2>(IX1-SSCF) && x2<(IX2+SSCF)) || (x3>(IX1-SSCF) && x3<(IX2+SSCF)) || (x4>(IX1-SSCF) && x4<(IX2+SSCF)))
	  if ((y5>(-SSCF) && y5<(479+SSCF))    || (y1>(-SSCF) && y1<(479+SSCF))   || (y2>-SSCF && y2<(479+SSCF))      || (y3>-SSCF && y3<(479+SSCF)) || (y4>-SSCF && y4<(479+SSCF)))
		if (z1<0 || z2<0 || z3<0 || z4<0)
	{
		TransformSome((x << 2), (y << 2), 5, 5, 1);
	
		AddWaterParticles(x << 2, y << 2);
	
		{
			PROFILE_FN(RMStripGeometry);
			
			KMVERTEX_03	*mv = &MapVertices[0];
			tvector		*tv = &Trans[zam];

#define	COPY_TRANSFORM { mv->fX = tv->x; mv->fY = tv->y; mv->u.fZ = -tv->z;}

			// 4x1 - 1 **************************************************** 
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv += mMapWidth - 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv += mMapWidth - 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv += mMapWidth - 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv += mMapWidth - 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	
			
			// 4x1 - 2 ****************************************************       	       	
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv -= mMapWidth + 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv -= mMapWidth + 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv -= mMapWidth + 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv -= mMapWidth + 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	

			// 4x1 - 3 **************************************************** 
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv += mMapWidth - 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv += mMapWidth - 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv += mMapWidth - 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv += mMapWidth - 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	
			
			// 4x1 - 4 ****************************************************       	       	
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv -= mMapWidth + 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv -= mMapWidth + 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv -= mMapWidth + 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	tv -= mMapWidth + 1;
			COPY_TRANSFORM;	mv ++;	tv ++;
			COPY_TRANSFORM;	mv ++;	

		}

		BOOL	drawn_first_pass = FALSE;
		
		for (c0 = 0; c0 < 3; c0 ++)
		{
			if (flags & (1 << (c0 + 1)))
			{
				{
					PROFILE_FN(RMStripSetup); 
					zam = ozam;
		    
					SINT	TN = ENGINE.GetMapTexture(TextureNumber[zam][c0])->mTPage + 1;
					
					if (drawn_first_pass == FALSE)
					{
						kmChangeStripListType			( &TextureHead[TN], KM_OPAQUE_POLYGON );    
						kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
						kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
						kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
						drawn_first_pass = TRUE;
					}
					else
					{
						kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );	
						kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
						kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
						kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
					}
					
					InitCZTs(TN);
								
					//*************************************************************
					//********************** 4x1 - 1 ******************************
					//*************************************************************
					
					DWORD	*cp		= &MapVertices[0].uBaseRGB.dwPacked;
					SINT	*meep	= &MixEE[zam][c0];
					SINT	*dcp	= &DColours[zam];

					#define	CPA		(sizeof(MapVertices[0]) / sizeof(DWORD))
					#define MEEPA	(sizeof(MixEE[0]) / sizeof(SINT))

					#define LIGHT_VERTEX {*cp = modulate_colour(*meep, *dcp);}

					// 4x1 - 1 ****************************************************       	       	
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA * (mMapWidth - 1);	dcp += mMapWidth - 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA * (mMapWidth - 1);	dcp += mMapWidth - 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA * (mMapWidth - 1);	dcp += mMapWidth - 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA * (mMapWidth - 1);	dcp += mMapWidth - 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		

					// 4x1 - 2 ****************************************************       	       	
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep -= MEEPA * (mMapWidth + 1);	dcp -= mMapWidth + 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep -= MEEPA * (mMapWidth + 1);	dcp -= mMapWidth + 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep -= MEEPA * (mMapWidth + 1);	dcp -= mMapWidth + 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep -= MEEPA * (mMapWidth + 1);	dcp -= mMapWidth + 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		

					// 4x1 - 3 ****************************************************       	       	
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA * (mMapWidth - 1);	dcp += mMapWidth - 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA * (mMapWidth - 1);	dcp += mMapWidth - 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA * (mMapWidth - 1);	dcp += mMapWidth - 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA * (mMapWidth - 1);	dcp += mMapWidth - 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		

					// 4x1 - 4 ****************************************************       	       	
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep -= MEEPA * (mMapWidth + 1);	dcp -= mMapWidth + 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep -= MEEPA * (mMapWidth + 1);	dcp -= mMapWidth + 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep -= MEEPA * (mMapWidth + 1);	dcp -= mMapWidth + 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		meep -= MEEPA * (mMapWidth + 1);	dcp -= mMapWidth + 1;
					LIGHT_VERTEX;	cp += CPA;		meep += MEEPA;						dcp ++;
					LIGHT_VERTEX;	cp += CPA;		

				}
				{
					PROFILE_FN(RMExitStrip);     	
		
					for (int i=0; i < 40; i++)
					{
					   	kmSetVertex(&VertexBufferDesc, &MapVertices[i], KM_VERTEXTYPE_03, sizeof(KMVERTEX_03));
					}
				}
			}
		}
		
		{
			PROFILE_FN(RMTriggers);
		
	        // JCL - switches and booster squares.
	        for (c0 = x << 2; c0 < (x << 2) + 4; c0 ++)
		        for (c1 = y << 2; c1 < (y << 2) + 4; c1 ++)
		        {
		        	CMapCell *cell = GetCell(CCellRef(c0, c1));
		        	
					SINT type = cell->GetType();
		        	if (type != CT_NORMAL)
		        	 if (c0 & 1)
		        		DrawCellType2(c0, c1, type);
		        		else
		        		DrawCellType1(c0, c1, type);		        		
		        
		        }		        
		 }
    }
}
//#pragma dont_inline off
//****************************************************************************************************
//****************************************************************************************************
#define ZBODGE2x2	(-0.0001f)

void CDCMap::DcRender2x2(int x,int y)
{
    SINT	c0, c1;
	// Trivial Reject
	
	int		zam = (x<<2) +((y<<2)*mMapWidth);
	int		ozam=zam; 
	int		flags=FFlags[x + (y * (mMapWidth >> 2))];

	float 	x1 = Trans[zam].x;
	float 	y1 = Trans[zam].y;
	float	z1 = Trans[zam].z;
	
	float 	x2 = Trans[zam+4].x;
	float 	y2 = Trans[zam+4].y;
	float  	z2 = Trans[zam+4].z;   
	
	float 	x3 = Trans[zam+(mMapWidth*4)].x;
	float 	y3 = Trans[zam+(mMapWidth*4)].y;
	float  	z3 = Trans[zam+(mMapWidth*4)].z;   
	
	float 	x4 = Trans[zam+4+(mMapWidth*4)].x;
	float 	y4 = Trans[zam+4+(mMapWidth*4)].y;
	float  	z4 = Trans[zam+4+(mMapWidth*4)].z;
	
	if ((x1>(IX1-SSCF) && x1<(IX2+SSCF)) || (x2>(IX1-SSCF) && x2<(IX2+SSCF)) || (x3>(IX1-SSCF) && x3<(IX2+SSCF)) || (x4>(IX1-SSCF) && x4<(IX2+SSCF)))
	if ((y1>(-SSCF) && y1<(479+SSCF)) || (y2>-SSCF && y2<(479+SSCF)) || (y3>-SSCF && y3<(479+SSCF)) || (y4>-SSCF && y4<(479+SSCF)))
	if (z1<0 || z2<0 || z3<0 || z4<0)
	{
		TransformSome((x << 2), (y << 2), 3, 3, 2);

		AddWaterParticles(x << 2, y << 2);
	
		{
			PROFILE_FN(RMStripGeometry);
		
			MapVertices2[0].fX                   = Trans[zam].x;;
			MapVertices2[0].fY                   =     Trans[zam].y;
			MapVertices2[0].u.fZ                 =    -Trans[zam].z + ZBODGE2x2;
			MapVertices2[1].fX                   = Trans[zam+2].x;
			MapVertices2[1].fY                   =     Trans[zam+2].y;
			MapVertices2[1].u.fZ                 =    -Trans[zam+2].z + ZBODGE2x2;

			zam+=mMapWidth * 2;
			MapVertices2[2].fX                   = Trans[zam].x;
			MapVertices2[2].fY                   =     Trans[zam].y;
			MapVertices2[2].u.fZ                 =    -Trans[zam].z + ZBODGE2x2;
			MapVertices2[3].fX                   = Trans[zam+2].x;
			MapVertices2[3].fY                   =     Trans[zam+2].y;
			MapVertices2[3].u.fZ                 =    -Trans[zam+2].z + ZBODGE2x2;
		
			zam+=mMapWidth * 2;
			MapVertices2[4].fX                   = Trans[zam].x;
			MapVertices2[4].fY                   =     Trans[zam].y;
			MapVertices2[4].u.fZ                 =    -Trans[zam].z + ZBODGE2x2;
			MapVertices2[5].fX                   = Trans[zam+2].x;
			MapVertices2[5].fY                   =     Trans[zam+2].y;
			MapVertices2[5].u.fZ                 =    -Trans[zam+2].z + ZBODGE2x2;
			
			zam += 2;
			MapVertices2[6].fX                   = Trans[zam].x;
			MapVertices2[6].fY                   =     Trans[zam].y;
			MapVertices2[6].u.fZ                 =    -Trans[zam].z + ZBODGE2x2;
			MapVertices2[7].fX                   = Trans[zam+2].x;
			MapVertices2[7].fY                   =     Trans[zam+2].y;
			MapVertices2[7].u.fZ                 =    -Trans[zam+2].z + ZBODGE2x2;
			
			zam-=mMapWidth * 2;         
			MapVertices2[8].fX                   = Trans[zam].x;
			MapVertices2[8].fY                   =     Trans[zam].y;
			MapVertices2[8].u.fZ                 =    -Trans[zam].z + ZBODGE2x2;
			MapVertices2[9].fX                   = Trans[zam+2].x;
			MapVertices2[9].fY                   =     Trans[zam+2].y;
			MapVertices2[9].u.fZ                 =    -Trans[zam+2].z + ZBODGE2x2;
			
			zam-=mMapWidth * 2;         
			MapVertices2[10].fX                   = Trans[zam].x;;
			MapVertices2[10].fY                   =     Trans[zam].y;
			MapVertices2[10].u.fZ                 =    -Trans[zam].z + ZBODGE2x2;
			MapVertices2[11].fX                   = Trans[zam+2].x;
			MapVertices2[11].fY                   =     Trans[zam+2].y;
			MapVertices2[11].u.fZ                 =    -Trans[zam+2].z + ZBODGE2x2;

		}

		BOOL	drawn_first_pass = FALSE;
		
		for (c0 = 0; c0 < 3; c0 ++)
		{
			if (flags & (1 << (c0 + 1)))
			{
				{
					PROFILE_FN(RMStripSetup); 
					zam = ozam;
		    
					SINT	TN = ENGINE.GetMapTexture(TextureNumber[zam][c0])->mTPage + 1;
					
					if (drawn_first_pass == FALSE)
					{
						kmChangeStripListType			( &TextureHead[TN], KM_OPAQUE_POLYGON );    
						kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
						kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
						kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
						drawn_first_pass = TRUE;
					}
					else
					{
						kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );	
						kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
						kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
						kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
					}
					
					InitCZTs(TN);
								
					//*************************************************************
					//********************** 4x1 - 1 ******************************
					//*************************************************************
					
					MapVertices2[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][c0], DColours[zam]);        
					MapVertices2[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+2][c0], DColours[zam+2]);
					zam+=mMapWidth * 2;        
					MapVertices2[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][c0], DColours[zam]);        
					MapVertices2[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+2][c0], DColours[zam+2]);        
					zam+=mMapWidth * 2;         
					MapVertices2[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][c0], DColours[zam]);        
					MapVertices2[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+2][c0], DColours[zam+2]);
					zam += 2;         
					MapVertices2[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][c0], DColours[zam]);        
					MapVertices2[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+2][c0], DColours[zam+2]);        
					zam-=mMapWidth * 2;         
					MapVertices2[8].uBaseRGB.dwPacked       =   modulate_colour(MixEE[zam][c0], DColours[zam]);        
					MapVertices2[9].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+2][c0], DColours[zam+2]);
					zam-=mMapWidth * 2;         
					MapVertices2[10].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][c0], DColours[zam]);        
					MapVertices2[11].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+2][c0], DColours[zam+2]);   

				}
				{
					PROFILE_FN(RMExitStrip);     	
		
					for (int i=0; i < 12; i++)
					{
					   	kmSetVertex(&VertexBufferDesc, &MapVertices2[i], KM_VERTEXTYPE_03, sizeof(KMVERTEX_03));
					}
				}
			}
		}
		
		{
			PROFILE_FN(RMTriggers);
		
	        // JCL - switches and booster squares.
	        for (c0 = x << 2; c0 < (x << 2) + 4; c0 ++)
		        for (c1 = y << 2; c1 < (y << 2) + 4; c1 ++)
		        {
		        	CMapCell *cell = GetCell(CCellRef(c0, c1));
		        	
					SINT type = cell->GetType();
		        	if (type != CT_NORMAL)
		        	 if (c0 & 1)
		        		DrawCellType2(c0, c1, type);
		        		else
		        		DrawCellType1(c0, c1, type);		        	
		        		
		        
		        }		        
		 }
    }
}
//****************************************************************************************************
//****************************************************************************************************
#define ZBODGE1x1	(-0.0002f)

void CDCMap::DcRender1x1(int x,int y)
{
    SINT	c0, c1;
	// Trivial Reject
	
	int		zam = (x<<2) +((y<<2)*mMapWidth);
	int		ozam=zam; 
	int		flags=FFlags[x + (y * (mMapWidth >> 2))];

	float 	x1 = Trans[zam].x;
	float 	y1 = Trans[zam].y;
	float	z1 = Trans[zam].z;
	
	float 	x2 = Trans[zam+4].x;
	float 	y2 = Trans[zam+4].y;
	float  	z2 = Trans[zam+4].z;   
	
	float 	x3 = Trans[zam+(mMapWidth*4)].x;
	float 	y3 = Trans[zam+(mMapWidth*4)].y;
	float  	z3 = Trans[zam+(mMapWidth*4)].z;   
	
	float 	x4 = Trans[zam+4+(mMapWidth*4)].x;
	float 	y4 = Trans[zam+4+(mMapWidth*4)].y;
	float  	z4 = Trans[zam+4+(mMapWidth*4)].z;
	
	if ((x1>(IX1-SSCF) && x1<(IX2+SSCF)) || (x2>(IX1-SSCF) && x2<(IX2+SSCF)) || (x3>(IX1-SSCF) && x3<(IX2+SSCF)) || (x4>(IX1-SSCF) && x4<(IX2+SSCF)))
	if ((y1>(-SSCF) && y1<(479+SSCF)) || (y2>-SSCF && y2<(479+SSCF)) || (y3>-SSCF && y3<(479+SSCF)) || (y4>-SSCF && y4<(479+SSCF)))
	if (z1<0 || z2<0 || z3<0 || z4<0)
	{
		AddWaterParticles(x << 2, y << 2);
	
		{
			PROFILE_FN(RMStripGeometry);
		
			MapVertices3[0].fX                   = Trans[zam].x;;
			MapVertices3[0].fY                   =     Trans[zam].y;
			MapVertices3[0].u.fZ                 =    -Trans[zam].z + ZBODGE1x1;
			MapVertices3[1].fX                   = Trans[zam+4].x;
			MapVertices3[1].fY                   =     Trans[zam+4].y;
			MapVertices3[1].u.fZ                 =    -Trans[zam+4].z + ZBODGE1x1;

			zam+=mMapWidth * 4;
			MapVertices3[2].fX                   = Trans[zam].x;
			MapVertices3[2].fY                   =     Trans[zam].y;
			MapVertices3[2].u.fZ                 =    -Trans[zam].z + ZBODGE1x1;
			MapVertices3[3].fX                   = Trans[zam+4].x;;
			MapVertices3[3].fY                   =     Trans[zam+4].y;
			MapVertices3[3].u.fZ                 =    -Trans[zam+4].z + ZBODGE1x1;
		}

		BOOL	drawn_first_pass = FALSE;
		
		for (c0 = 0; c0 < 3; c0 ++)
		{
			if (flags & (1 << (c0 + 1)))
			{
				{
					PROFILE_FN(RMStripSetup); 
					zam = ozam;
		    
					SINT	TN = ENGINE.GetMapTexture(TextureNumber[zam][c0])->mTPage + 1;
					
					if (drawn_first_pass == FALSE)
					{
						kmChangeStripListType			( &TextureHead[TN], KM_OPAQUE_POLYGON );    
						kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
						kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
						kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
						drawn_first_pass = TRUE;
					}
					else
					{
						kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );	
						kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
						kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
						kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
					}
					
					InitCZTs(TN);
								
					//*************************************************************
					//********************** 4x1 - 1 ******************************
					//*************************************************************
					
					MapVertices3[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][c0], DColours[zam]);        
					MapVertices3[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+4][c0], DColours[zam+4]);
					zam+=mMapWidth * 4;        
					MapVertices3[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][c0], DColours[zam]);        
					MapVertices3[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+4][c0], DColours[zam+4]);        

				}
				{
					PROFILE_FN(RMExitStrip);     	
		
					for (int i=0; i < 4; i++)
					{
					   	kmSetVertex(&VertexBufferDesc, &MapVertices3[i], KM_VERTEXTYPE_03, sizeof(KMVERTEX_03));
					}
				}
			}
		}
		
		{
			PROFILE_FN(RMTriggers);
		
	        // JCL - switches and booster squares.
	        for (c0 = x << 2; c0 < (x << 2) + 4; c0 ++)
		        for (c1 = y << 2; c1 < (y << 2) + 4; c1 ++)
		        {
		        	CMapCell *cell = GetCell(CCellRef(c0, c1));
		        	
					SINT type = cell->GetType();
		        	if (type != CT_NORMAL)
		        	 if (c0 & 1)
		        		DrawCellType2(c0, c1, type);
		        		else
		        		DrawCellType1(c0, c1, type);		        	
		    
		        
		        }		        
		 }
    }
}

//****************************************************************************************
//****************************************************************************************

void CDCMap::DcRender4x3(int x,int y)
{

// if (x<0 || x>(mMapWidth>>2)) return;
// if (y<0 || y>(mMapHeight>>2)) return;

 // Trivial Reject

 int zam = (x<<2) +((y<<2)*mMapWidth);
 int ozam=zam; 
 int flags=Flaggy[zam];
 float x1=Trans[zam].x;
 float y1=Trans[zam].y;

 float x2=Trans[zam+4].x;
 float y2=Trans[zam+4].y;

 float x3=Trans[zam+(mMapWidth*3)].x;
 float y3=Trans[zam+(mMapWidth*3)].y;

 float x4=Trans[zam+4+(mMapWidth*3)].x;
 float y4=Trans[zam+4+(mMapWidth*3)].y;

 float 	x5 = Trans[zam+2].x;
 float 	y5 = Trans[zam+2].y;

  float  z1=    Trans[zam].z;
  float  z2=    Trans[zam+4].z;   
  float  z3=    Trans[zam+(mMapWidth*3)].z;
  float  z4=    Trans[zam+4+(mMapWidth*3)].z;   


if ((x5>(IX1-SSCF) && x5<(IX2+SSCF)) || (x1>(IX1-SSCF) && x1<(IX2+SSCF)) || (x2>(IX1-SSCF) && x2<(IX2+SSCF)) || (x3>(IX1-SSCF) && x3<(IX2+SSCF)) || (x4>(IX1-SSCF) && x4<(IX2+SSCF)))
if ((y5>(-SSCF) && y5<(479+SSCF)) || (y1>(-SSCF) && y1<(479+SSCF)) || (y2>-SSCF && y2<(479+SSCF)) || (y3>-SSCF && y3<(479+SSCF)) || (y4>-SSCF && y4<(479+SSCF)))
 if (z1<0 || z2<0 || z3<0 || z4<0)
  {
  TransformSome((x << 2), (y << 2), 5, 4, 1);


  int TN = TextureNumber[zam][0];


  
  float du=ENGINE.GetMapTexture(TN)->mW /4;
  float dv=ENGINE.GetMapTexture(TN)->mH /4;
  
  float u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
  float u2 = ENGINE.GetMapTexture(TN)->mX+(du);
  float u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
  float u4 = ENGINE.GetMapTexture(TN)->mX+(du*3);
  float u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
  	
  float v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
  float v4 = ENGINE.GetMapTexture(TN)->mY+(dv)+0.00390635f;
  float v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
  float v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
  float v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
 
  //TN=TN-1;
   
	kmChangeStripListType( &TextureHead[TN], KM_OPAQUE_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
    
        InitCZTs(TN);
   
   		//*************************************************************
   		//********************** 4x1 - 1 ******************************
   		//*************************************************************
  		
        TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[0].fX                   = Trans[zam].x;;
        TextureS[0].fY                   =     Trans[zam].y;
        TextureS[0].u.fZ                 =    -Trans[zam].z;
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        


        TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[1].fX                   = Trans[zam+1].x;
        TextureS[1].fY                   =     Trans[zam+1].y;
        TextureS[1].u.fZ                 =    -Trans[zam+1].z;
        TextureS[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        

        TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[2].fX                   = Trans[zam].x;
        TextureS[2].fY                   =     Trans[zam].y;
        TextureS[2].u.fZ                 =    -Trans[zam].z;
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
    
       
        TextureS[3].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[3].fX                   = Trans[zam+1].x;;
        TextureS[3].fY                   =     Trans[zam+1].y;
        TextureS[3].u.fZ                 =    -Trans[zam+1].z;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        
        zam+=mMapWidth;         
        
       	TextureS[4].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[4].fX                   = Trans[zam].x;
       	TextureS[4].fY                   =     Trans[zam].y;
       	TextureS[4].u.fZ                 =    -Trans[zam].z;
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	
       	
       	TextureS[5].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[5].fX                   = Trans[zam+1].x;
       	TextureS[5].fY                   =     Trans[zam+1].y;
       	TextureS[5].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        

        
        zam+=mMapWidth;         
        
       	TextureS[6].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[6].fX                   = Trans[zam].x;
       	TextureS[6].fY                   =     Trans[zam].y;
       	TextureS[6].u.fZ                 =    -Trans[zam].z;
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	
       	
       	TextureS[7].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[7].fX                   = Trans[zam+1].x;
       	TextureS[7].fY                   =     Trans[zam+1].y;
       	TextureS[7].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
       	
        	
   		// 4x1 - 2 ****************************************************       	       	
       	
        zam+=1;
        TextureS[8].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[8].fX                   = Trans[zam].x;;
        TextureS[8].fY                   =     Trans[zam].y;
        TextureS[8].u.fZ                 =    -Trans[zam].z;
        TextureS[8].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[8].fU                   = u2;
        TextureS[8].fV                   = v4;        


        TextureS[9].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[9].fX                   = Trans[zam+1].x;
        TextureS[9].fY                   =     Trans[zam+1].y;
        TextureS[9].u.fZ                 =    -Trans[zam+1].z;
        TextureS[9].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[9].fU                   = u3;
        TextureS[9].fV                   = v4;         
        zam-=mMapWidth;        

        TextureS[10].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[10].fX                   = Trans[zam].x;
        TextureS[10].fY                   =     Trans[zam].y;
        TextureS[10].u.fZ                 =    -Trans[zam].z;
        TextureS[10].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[10].fU                   = u2;
        TextureS[10].fV                   = v3;
    
       
        TextureS[11].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[11].fX                   = Trans[zam+1].x;;
        TextureS[11].fY                   =     Trans[zam+1].y;
        TextureS[11].u.fZ                 =    -Trans[zam+1].z;
        TextureS[11].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[11].fU                   = u3;
        TextureS[11].fV                   = v3; 
        
        zam-=mMapWidth;         
        
       	TextureS[12].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[12].fX                   = Trans[zam].x;
       	TextureS[12].fY                   =     Trans[zam].y;
       	TextureS[12].u.fZ                 =    -Trans[zam].z;
       	TextureS[12].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[12].fU                   = u2;
       	TextureS[12].fV                   = v2;
       	
       	
       	TextureS[13].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[13].fX                   = Trans[zam+1].x;
       	TextureS[13].fY                   =     Trans[zam+1].y;
       	TextureS[13].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[13].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[13].fU                   = u3;
       	TextureS[13].fV                   = v2;
        
        zam-=mMapWidth;         
        
       	TextureS[14].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[14].fX                   = Trans[zam].x;
       	TextureS[14].fY                   =     Trans[zam].y;
       	TextureS[14].u.fZ                 =    -Trans[zam].z;
       	TextureS[14].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v1;
       	
       	
       	TextureS[15].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[15].fX                   = Trans[zam+1].x;
       	TextureS[15].fY                   =     Trans[zam+1].y;
       	TextureS[15].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[15].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);       
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v1;
       	
       	
    	// 4x1 - 3 ****************************************************
       	
       	zam+=1;
        TextureS[16].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[16].fX                   = Trans[zam].x;;
        TextureS[16].fY                   =     Trans[zam].y;
        TextureS[16].u.fZ                 =    -Trans[zam].z;
        TextureS[16].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);       
        TextureS[16].fU                   = u3;
        TextureS[16].fV                   = v1;        


        TextureS[17].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[17].fX                   = Trans[zam+1].x;
        TextureS[17].fY                   =     Trans[zam+1].y;
        TextureS[17].u.fZ                 =    -Trans[zam+1].z;
        TextureS[17].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[17].fU                   = u4;
        TextureS[17].fV                   = v1;         
        zam+=mMapWidth;        

        TextureS[18].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[18].fX                   = Trans[zam].x;
        TextureS[18].fY                   =     Trans[zam].y;
        TextureS[18].u.fZ                 =    -Trans[zam].z;
        TextureS[18].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[18].fU                   = u3;
        TextureS[18].fV                   = v2;
    
       
        TextureS[19].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[19].fX                   = Trans[zam+1].x;;
        TextureS[19].fY                   =     Trans[zam+1].y;
        TextureS[19].u.fZ                 =    -Trans[zam+1].z;
        TextureS[19].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[19].fU                   = u4;
        TextureS[19].fV                   = v2; 
        
        zam+=mMapWidth;         
        
       	TextureS[20].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[20].fX                   = Trans[zam].x;
       	TextureS[20].fY                   =     Trans[zam].y;
       	TextureS[20].u.fZ                 =    -Trans[zam].z;
       	TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);       
       	TextureS[20].fU                   = u3;
       	TextureS[20].fV                   = v3;
       	
       	
       	TextureS[21].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[21].fX                   = Trans[zam+1].x;
       	TextureS[21].fY                   =     Trans[zam+1].y;
       	TextureS[21].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[21].fU                   = u4;
       	TextureS[21].fV                   = v3;
        
        zam+=mMapWidth;         
        
       	TextureS[22].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[22].fX                   = Trans[zam].x;
       	TextureS[22].fY                   =     Trans[zam].y;
       	TextureS[22].u.fZ                 =    -Trans[zam].z;
       	TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[22].fU                   = u3;
       	TextureS[22].fV                   = v4;
       	
       	
       	TextureS[23].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[23].fX                   = Trans[zam+1].x;
       	TextureS[23].fY                   =     Trans[zam+1].y;
       	TextureS[23].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[23].fU                   = u4;
       	TextureS[23].fV                   = v4;
       	

    	// 4x1 - 4 ****************************************************
       	
       	zam+=1;
        TextureS[24].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[24].fX                   = Trans[zam].x;;
        TextureS[24].fY                   =     Trans[zam].y;
        TextureS[24].u.fZ                 =    -Trans[zam].z;
        TextureS[24].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[24].fU                   = u4;
        TextureS[24].fV                   = v4;        


        TextureS[25].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[25].fX                   = Trans[zam+1].x;
        TextureS[25].fY                   =     Trans[zam+1].y;
        TextureS[25].u.fZ                 =    -Trans[zam+1].z;
        TextureS[25].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[25].fU                   = u5;
        TextureS[25].fV                   = v4;         
        zam-=mMapWidth;        

        TextureS[26].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[26].fX                   = Trans[zam].x;
        TextureS[26].fY                   =     Trans[zam].y;
        TextureS[26].u.fZ                 =    -Trans[zam].z;
        TextureS[26].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[26].fU                   = u4;
        TextureS[26].fV                   = v3;
    
       
        TextureS[27].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[27].fX                   = Trans[zam+1].x;;
        TextureS[27].fY                   =     Trans[zam+1].y;
        TextureS[27].u.fZ                 =    -Trans[zam+1].z;
        TextureS[27].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[27].fU                   = u5;
        TextureS[27].fV                   = v3; 
        
        zam-=mMapWidth;         
        
       	TextureS[28].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[28].fX                   = Trans[zam].x;
       	TextureS[28].fY                   =     Trans[zam].y;
       	TextureS[28].u.fZ                 =    -Trans[zam].z;
       	TextureS[28].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[28].fU                   = u4;
       	TextureS[28].fV                   = v2;
       	
       	
       	TextureS[29].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[29].fX                   = Trans[zam+1].x;
       	TextureS[29].fY                   =     Trans[zam+1].y;
       	TextureS[29].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[29].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[29].fU                   = u5;
       	TextureS[29].fV                   = v2;
        
        zam-=mMapWidth;         
        
       	TextureS[30].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[30].fX                   = Trans[zam].x;
       	TextureS[30].fY                   =     Trans[zam].y;
       	TextureS[30].u.fZ                 =    -Trans[zam].z;
       	TextureS[30].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[30].fU                   = u4;
       	TextureS[30].fV                   = v1;
       	
       	
       	TextureS[31].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
       	TextureS[31].fX                   = Trans[zam+1].x;
       	TextureS[31].fY                   =     Trans[zam+1].y;
       	TextureS[31].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[31].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]); 
       	TextureS[31].fU                   = u5;
       	TextureS[31].fV                   = v1;
       	
       	
	ExitCZTs(32);

flags>>=1;
//if (flags<1) return;
		
//    kmChangeStripBlendingMode       ( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
// 	kmChangeStripTextureShadingMode ( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_MODULATE);	
// 	kmChangeStripUseAlpha			( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_TRUE );
 	
zam=ozam;
  TN = TextureNumber[zam][1];
  du=ENGINE.GetMapTexture(TN)->mW /4;
  dv=ENGINE.GetMapTexture(TN)->mH /4;
  
  u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
  u2 = ENGINE.GetMapTexture(TN)->mX+(du);
  u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
  u4 = ENGINE.GetMapTexture(TN)->mX+(du*3);
  u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
  v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
  v4 = ENGINE.GetMapTexture(TN)->mY+(dv)+0.00390635f;
  v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
  v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
  v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
  kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );	
//  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
//  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
  
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        
        TextureS[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]); ;        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);         
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	TextureS[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
   		// 4x1 - 2 ****************************************************       	       	
        zam+=1;
        TextureS[8].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[8].fU                   = u2;
        TextureS[8].fV                   = v4;        
        TextureS[9].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);       
        TextureS[9].fU                   = u3;
        TextureS[9].fV                   = v4;         
        zam-=mMapWidth;        
        TextureS[10].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[10].fU                   = u2;
        TextureS[10].fV                   = v3;
        TextureS[11].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);    
        TextureS[11].fU                   = u3;
        TextureS[11].fV                   = v3; 
        zam-=mMapWidth;         
       	TextureS[12].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[12].fU                   = u2;
       	TextureS[12].fV                   = v2;
       	TextureS[13].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);   
       	TextureS[13].fU                   = u3;
       	TextureS[13].fV                   = v2;
        zam-=mMapWidth;         
       	TextureS[14].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v1;
       	TextureS[15].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]); 
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v1;
    	// 4x1 - 3 ****************************************************
       	zam+=1;
        TextureS[16].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);       
        TextureS[16].fU                   = u3;
        TextureS[16].fV                   = v1;        
        TextureS[17].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);     
        TextureS[17].fU                   = u4;
        TextureS[17].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[18].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[18].fU                   = u3;
        TextureS[18].fV                   = v2;
        TextureS[19].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);   
        TextureS[19].fU                   = u4;
        TextureS[19].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);       
       	TextureS[20].fU                   = u3;
       	TextureS[20].fV                   = v3;
       	TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);     
       	TextureS[21].fU                   = u4;
       	TextureS[21].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[22].fU                   = u3;
       	TextureS[22].fV                   = v4;
       	TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]); 
       	TextureS[23].fU                   = u4;
       	TextureS[23].fV                   = v4;

    	// 4x1 - 4 ****************************************************
       	zam+=1;
        TextureS[24].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[24].fU                   = u4;
        TextureS[24].fV                   = v4;        
        TextureS[25].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]); 
        TextureS[25].fU                   = u5;
        TextureS[25].fV                   = v4;         
        zam-=mMapWidth;        
        TextureS[26].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[26].fU                   = u4;
        TextureS[26].fV                   = v3;
        TextureS[27].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]); 
        TextureS[27].fU                   = u5;
        TextureS[27].fV                   = v3; 
        zam-=mMapWidth;         
       	TextureS[28].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[28].fU                   = u4;
       	TextureS[28].fV                   = v2;
       	TextureS[29].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[29].fU                   = u5;
       	TextureS[29].fV                   = v2;
        zam-=mMapWidth;         
       	TextureS[30].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[30].fU                   = u4;
       	TextureS[30].fV                   = v1;
       	TextureS[31].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]); 
       	TextureS[31].fU                   = u5;
       	TextureS[31].fV                   = v1;

 	
    InitCZTs(TN);	
    ExitCZTs(32);  

flags>>=1;
//if (flags<1) return;
  zam=ozam;
   TN = TextureNumber[zam][2];
   du=ENGINE.GetMapTexture(TN)->mW /4;
   dv=ENGINE.GetMapTexture(TN)->mH /4;
  
   u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
   u2 = ENGINE.GetMapTexture(TN)->mX+(du);
   u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
   u4 = ENGINE.GetMapTexture(TN)->mX+(du*3);
   u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
   v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
   v4 = ENGINE.GetMapTexture(TN)->mY+(dv)+0.00390635f;
   v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
   v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
   v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
  kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );	
//  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
//  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
  
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        
        TextureS[1].uBaseRGB.dwPacked          =  modulate_colour(MixEE[zam+1][2], DColours[zam+1]);     
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);            
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);            
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	TextureS[7].uBaseRGB.dwPacked           =  modulate_colour(MixEE[zam+1][2], DColours[zam+1]);       
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
   		// 4x1 - 2 ****************************************************       	       	
        zam+=1;
        TextureS[8].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[8].fU                   = u2;
        TextureS[8].fV                   = v4;        
        TextureS[9].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);            
        TextureS[9].fU                   = u3;
        TextureS[9].fV                   = v4;         
        zam-=mMapWidth;        
        TextureS[10].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[10].fU                   = u2;
        TextureS[10].fV                   = v3;
        TextureS[11].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);           
        TextureS[11].fU                   = u3;
        TextureS[11].fV                   = v3; 
        zam-=mMapWidth;         
       	TextureS[12].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[12].fU                   = u2;
       	TextureS[12].fV                   = v2;
       	TextureS[13].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);           
       	TextureS[13].fU                   = u3;
       	TextureS[13].fV                   = v2;
        zam-=mMapWidth;         
       	TextureS[14].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v1;
       	TextureS[15].uBaseRGB.dwPacked          =  modulate_colour(MixEE[zam+1][2], DColours[zam+1]);          
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v1;
    	// 4x1 - 3 ****************************************************
       	zam+=1;
        TextureS[16].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);       
        TextureS[16].fU                   = u3;
        TextureS[16].fV                   = v1;        
        TextureS[17].uBaseRGB.dwPacked           =  modulate_colour(MixEE[zam+1][2], DColours[zam+1]);            
        TextureS[17].fU                   = u4;
        TextureS[17].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[18].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[18].fU                   = u3;
        TextureS[18].fV                   = v2;
        TextureS[19].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);           
        TextureS[19].fU                   = u4;
        TextureS[19].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);       
       	TextureS[20].fU                   = u3;
       	TextureS[20].fV                   = v3;
       	TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);             
       	TextureS[21].fU                   = u4;
       	TextureS[21].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[22].fU                   = u3;
       	TextureS[22].fV                   = v4;
       	TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);          
       	TextureS[23].fU                   = u4;
       	TextureS[23].fV                   = v4;
    	// 4x1 - 4 ****************************************************
       	zam+=1;
        TextureS[24].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[24].fU                   = u4;
        TextureS[24].fV                   = v4;        
        TextureS[25].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);         
        TextureS[25].fU                   = u5;
        TextureS[25].fV                   = v4;         
        zam-=mMapWidth;        
        TextureS[26].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[26].fU                   = u4;
        TextureS[26].fV                   = v3;
        TextureS[27].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);         
        TextureS[27].fU                   = u5;
        TextureS[27].fV                   = v3; 
        zam-=mMapWidth;         
       	TextureS[28].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[28].fU                   = u4;
       	TextureS[28].fV                   = v2;
       	TextureS[29].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);         
       	TextureS[29].fU                   = u5;
       	TextureS[29].fV                   = v2;
        zam-=mMapWidth;         
       	TextureS[30].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[30].fU                   = u4;
       	TextureS[30].fV                   = v1;
       	TextureS[31].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);     
       	TextureS[31].fU                   = u5;
       	TextureS[31].fV                   = v1;


 	
    InitCZTs(TN);	
    ExitCZTs(32); 
    
    
    		{
			PROFILE_FN(RMTriggers);
		
	        // JCL - switches and booster squares.
	        SINT 	c0, c1;
	        for (c0 = x << 2; c0 < (x << 2) + 4; c0 ++)
		        for (c1 = y << 2; c1 < (y << 2) + 3; c1 ++)
		        {
		        	CMapCell *cell = GetCell(CCellRef(c0, c1));
		        	
					SINT type = cell->GetType();
		        	if (type != CT_NORMAL)
		        	 if (c0 & 1)
		        		DrawCellType2(c0, c1, type);
		        		else
		        		DrawCellType1(c0, c1, type);		        		
		        
		        }		        
			}
}
}


void CDCMap::DcRender3x4(int x,int y)
{

// if (x<0 || x>(mMapWidth>>2)) return;
// if (y<0 || y>(mMapHeight>>2)) return;

 // Trivial Reject

 int zam = (x<<2) +((y<<2)*mMapWidth);
 int ozam=zam; 
 int flags=Flaggy[zam];
 float x1=Trans[zam].x;
 float y1=Trans[zam].y;

 float x2=Trans[zam+3].x;
 float y2=Trans[zam+3].y;

 float x3=Trans[zam+(mMapWidth*4)].x;
 float y3=Trans[zam+(mMapWidth*4)].y;

 float x4=Trans[zam+3+(mMapWidth*4)].x;
 float y4=Trans[zam+3+(mMapWidth*4)].y;

 float  z1=    Trans[zam].z;
 float  z2=    Trans[zam+3].z;   
 float  z3=    Trans[zam+(mMapWidth*4)].z;   
 float  z4=    Trans[zam+3+(mMapWidth*4)].z;   

if ((x1>(IX1-SSCF) && x1<(IX2+SSCF)) || (x2>(IX1-SSCF) && x2<(IX2+SSCF)) || (x3>(IX1-SSCF) && x3<(IX2+SSCF)) || (x4>(IX1-SSCF) && x4<(IX2+SSCF)))
if ((y1>(-SSCF) && y1<(479+SSCF)) || (y2>-SSCF && y2<(479+SSCF)) || (y3>-SSCF && y3<(479+SSCF)) || (y4>-SSCF && y4<(479+SSCF)))
 if (z1<0 || z2<0 || z3<0 || z4<0)
  {
	TransformSome((x << 2), (y << 2), 4, 5, 1);

  int TN = TextureNumber[zam][0];


  
  float du=ENGINE.GetMapTexture(TN)->mW /4;
  float dv=ENGINE.GetMapTexture(TN)->mH /4;
  
  float u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
  float u2 = ENGINE.GetMapTexture(TN)->mX+(du);
  float u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
  float u4 = ENGINE.GetMapTexture(TN)->mX+(du*3)-0.00390635f;
  float u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
  	
  float v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
  float v4 = ENGINE.GetMapTexture(TN)->mY+(dv);
  float v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
  float v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
  float v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
 
  //TN=TN-1;
    
	kmChangeStripListType( &TextureHead[TN], KM_OPAQUE_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
    
        InitCZTs(TN);
   
   		//*************************************************************
   		//********************** 4x1 - 1 ******************************
   		//*************************************************************
  		
        TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[0].fX                   = Trans[zam].x;;
        TextureS[0].fY                   =     Trans[zam].y;
        TextureS[0].u.fZ                 =    -Trans[zam].z;
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        


        TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[1].fX                   = Trans[zam+1].x;
        TextureS[1].fY                   =     Trans[zam+1].y;
        TextureS[1].u.fZ                 =    -Trans[zam+1].z;
        TextureS[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        

        TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[2].fX                   = Trans[zam].x;
        TextureS[2].fY                   =     Trans[zam].y;
        TextureS[2].u.fZ                 =    -Trans[zam].z;
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
    
       
        TextureS[3].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[3].fX                   = Trans[zam+1].x;;
        TextureS[3].fY                   =     Trans[zam+1].y;
        TextureS[3].u.fZ                 =    -Trans[zam+1].z;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        
        zam+=mMapWidth;         
        
       	TextureS[4].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[4].fX                   = Trans[zam].x;
       	TextureS[4].fY                   =     Trans[zam].y;
       	TextureS[4].u.fZ                 =    -Trans[zam].z;
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	
       	
       	TextureS[5].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[5].fX                   = Trans[zam+1].x;
       	TextureS[5].fY                   =     Trans[zam+1].y;
       	TextureS[5].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        

        
        zam+=mMapWidth;         
        
       	TextureS[6].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[6].fX                   = Trans[zam].x;
       	TextureS[6].fY                   =     Trans[zam].y;
       	TextureS[6].u.fZ                 =    -Trans[zam].z;
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	
       	
       	TextureS[7].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[7].fX                   = Trans[zam+1].x;
       	TextureS[7].fY                   =     Trans[zam+1].y;
       	TextureS[7].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
       	
        zam+=mMapWidth;         
        
       	TextureS[8].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[8].fX                   = Trans[zam].x;
       	TextureS[8].fY                   =     Trans[zam].y;
       	TextureS[8].u.fZ                 =    -Trans[zam].z;
       	TextureS[8].uBaseRGB.dwPacked       =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[8].fU                   = u1;
       	TextureS[8].fV                   = v5;
       	
       	
       	TextureS[9].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[9].fX                   = Trans[zam+1].x;
       	TextureS[9].fY                   =     Trans[zam+1].y;
       	TextureS[9].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[9].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[9].fU                   = u2;
       	TextureS[9].fV                   = v5;
       	
   		// 4x1 - 2 ****************************************************       	       	
       	
        zam+=1;
        TextureS[10].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[10].fX                   = Trans[zam].x;;
        TextureS[10].fY                   =     Trans[zam].y;
        TextureS[10].u.fZ                 =    -Trans[zam].z;
        TextureS[10].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[10].fU                   = u2;
        TextureS[10].fV                   = v5;        


        TextureS[11].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[11].fX                   = Trans[zam+1].x;
        TextureS[11].fY                   =     Trans[zam+1].y;
        TextureS[11].u.fZ                 =    -Trans[zam+1].z;
        TextureS[11].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[11].fU                   = u3;
        TextureS[11].fV                   = v5;         
        zam-=mMapWidth;        

        TextureS[12].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[12].fX                   = Trans[zam].x;
        TextureS[12].fY                   =     Trans[zam].y;
        TextureS[12].u.fZ                 =    -Trans[zam].z;
        TextureS[12].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[12].fU                   = u2;
        TextureS[12].fV                   = v4;
    
       
        TextureS[13].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[13].fX                   = Trans[zam+1].x;;
        TextureS[13].fY                   =     Trans[zam+1].y;
        TextureS[13].u.fZ                 =    -Trans[zam+1].z;
        TextureS[13].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[13].fU                   = u3;
        TextureS[13].fV                   = v4; 
        
        zam-=mMapWidth;         
        
       	TextureS[14].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[14].fX                   = Trans[zam].x;
       	TextureS[14].fY                   =     Trans[zam].y;
       	TextureS[14].u.fZ                 =    -Trans[zam].z;
       	TextureS[14].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v3;
       	
       	
       	TextureS[15].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[15].fX                   = Trans[zam+1].x;
       	TextureS[15].fY                   =     Trans[zam+1].y;
       	TextureS[15].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[15].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v3;
        
        zam-=mMapWidth;         
        
       	TextureS[16].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[16].fX                   = Trans[zam].x;
       	TextureS[16].fY                   =     Trans[zam].y;
       	TextureS[16].u.fZ                 =    -Trans[zam].z;
       	TextureS[16].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[16].fU                   = u2;
       	TextureS[16].fV                   = v2;
       	
       	
       	TextureS[17].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[17].fX                   = Trans[zam+1].x;
       	TextureS[17].fY                   =     Trans[zam+1].y;
       	TextureS[17].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[17].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);       
       	TextureS[17].fU                   = u3;
       	TextureS[17].fV                   = v2;
       	
        zam-=mMapWidth;         
        
       	TextureS[18].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[18].fX                   = Trans[zam].x;
       	TextureS[18].fY                   =     Trans[zam].y;
       	TextureS[18].u.fZ                 =    -Trans[zam].z;
       	TextureS[18].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[18].fU                   = u2;
       	TextureS[18].fV                   = v1;
       	
       	
       	TextureS[19].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[19].fX                   = Trans[zam+1].x;
       	TextureS[19].fY                   =     Trans[zam+1].y;
       	TextureS[19].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[19].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);       
       	TextureS[19].fU                   = u3;
       	TextureS[19].fV                   = v1;  
       	
    	// 4x1 - 3 ****************************************************
       	
       	zam+=1;
        TextureS[20].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[20].fX                   = Trans[zam].x;;
        TextureS[20].fY                   =     Trans[zam].y;
        TextureS[20].u.fZ                 =    -Trans[zam].z;
        TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);       
        TextureS[20].fU                   = u3;
        TextureS[20].fV                   = v1;        


        TextureS[21].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[21].fX                   = Trans[zam+1].x;
        TextureS[21].fY                   =     Trans[zam+1].y;
        TextureS[21].u.fZ                 =    -Trans[zam+1].z;
        TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[21].fU                   = u4;
        TextureS[21].fV                   = v1;         
        zam+=mMapWidth;        

        TextureS[22].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[22].fX                   = Trans[zam].x;
        TextureS[22].fY                   =     Trans[zam].y;
        TextureS[22].u.fZ                 =    -Trans[zam].z;
        TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[22].fU                   = u3;
        TextureS[22].fV                   = v2;
    
       
        TextureS[23].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[23].fX                   = Trans[zam+1].x;;
        TextureS[23].fY                   =     Trans[zam+1].y;
        TextureS[23].u.fZ                 =    -Trans[zam+1].z;
        TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[23].fU                   = u4;
        TextureS[23].fV                   = v2; 
        
        zam+=mMapWidth;         
        
       	TextureS[24].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[24].fX                   = Trans[zam].x;
       	TextureS[24].fY                   =     Trans[zam].y;
       	TextureS[24].u.fZ                 =    -Trans[zam].z;
       	TextureS[24].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);       
       	TextureS[24].fU                   = u3;
       	TextureS[24].fV                   = v3;
       	
       	
       	TextureS[25].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[25].fX                   = Trans[zam+1].x;
       	TextureS[25].fY                   =     Trans[zam+1].y;
       	TextureS[25].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[25].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[25].fU                   = u4;
       	TextureS[25].fV                   = v3;
        
        zam+=mMapWidth;         
        
       	TextureS[26].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[26].fX                   = Trans[zam].x;
       	TextureS[26].fY                   =     Trans[zam].y;
       	TextureS[26].u.fZ                 =    -Trans[zam].z;
       	TextureS[26].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[26].fU                   = u3;
       	TextureS[26].fV                   = v4;
       	
       	
       	TextureS[27].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[27].fX                   = Trans[zam+1].x;
       	TextureS[27].fY                   =     Trans[zam+1].y;
       	TextureS[27].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[27].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[27].fU                   = u4;
       	TextureS[27].fV                   = v4;
       	
        zam+=mMapWidth;         
        
       	TextureS[28].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[28].fX                   = Trans[zam].x;
       	TextureS[28].fY                   =     Trans[zam].y;
       	TextureS[28].u.fZ                 =    -Trans[zam].z;
       	TextureS[28].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);       
       	TextureS[28].fU                   = u3;
       	TextureS[28].fV                   = v5;
       	
       	
       	TextureS[29].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
       	TextureS[29].fX                   = Trans[zam+1].x;
       	TextureS[29].fY                   =     Trans[zam+1].y;
       	TextureS[29].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[29].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[29].fU                   = u4;
       	TextureS[29].fV                   = v5; 

     	
	ExitCZTs(30);

flags>>=1;
//if (flags<1) return;
		
//    kmChangeStripBlendingMode       ( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
// 	kmChangeStripTextureShadingMode ( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_MODULATE);	
// 	kmChangeStripUseAlpha			( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_TRUE );
 	
zam=ozam;
  TN = TextureNumber[zam][1];
  du=ENGINE.GetMapTexture(TN)->mW /4;
  dv=ENGINE.GetMapTexture(TN)->mH /4;
  
  u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
  u2 = ENGINE.GetMapTexture(TN)->mX+(du);
  u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
  u4 = ENGINE.GetMapTexture(TN)->mX+(du*3)-0.00390635f;
  u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
  v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
  v4 = ENGINE.GetMapTexture(TN)->mY+(dv);
  v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
  v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
  v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
  kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );	
//  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
//  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
  
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        
        TextureS[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	TextureS[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
        zam+=mMapWidth;         
       	TextureS[8].uBaseRGB.dwPacked       =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[8].fU                   = u1;
       	TextureS[8].fV                   = v5;
       	TextureS[9].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[9].fU                   = u2;
       	TextureS[9].fV                   = v5;
   		// 4x1 - 2 ****************************************************       	       	
        zam+=1;
        TextureS[10].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[10].fU                   = u2;
        TextureS[10].fV                   = v5;        
        TextureS[11].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[11].fU                   = u3;
        TextureS[11].fV                   = v5;         
        zam-=mMapWidth;        
        TextureS[12].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[12].fU                   = u2;
        TextureS[12].fV                   = v4;
        TextureS[13].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[13].fU                   = u3;
        TextureS[13].fV                   = v4; 
        zam-=mMapWidth;         
       	TextureS[14].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v3;
       	TextureS[15].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v3;
        zam-=mMapWidth;         
       	TextureS[16].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[16].fU                   = u2;
       	TextureS[16].fV                   = v2;
       	TextureS[17].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);       
       	TextureS[17].fU                   = u3;
       	TextureS[17].fV                   = v2;
        zam-=mMapWidth;         
       	TextureS[18].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[18].fU                   = u2;
       	TextureS[18].fV                   = v1;
       	TextureS[19].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);       
       	TextureS[19].fU                   = u3;
       	TextureS[19].fV                   = v1;  
    	// 4x1 - 3 ****************************************************
       	zam+=1;
        TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);       
        TextureS[20].fU                   = u3;
        TextureS[20].fV                   = v1;        
        TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[21].fU                   = u4;
        TextureS[21].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[22].fU                   = u3;
        TextureS[22].fV                   = v2;
        TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[23].fU                   = u4;
        TextureS[23].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[24].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);       
       	TextureS[24].fU                   = u3;
       	TextureS[24].fV                   = v3;
       	TextureS[25].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[25].fU                   = u4;
       	TextureS[25].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[26].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[26].fU                   = u3;
       	TextureS[26].fV                   = v4;
       	TextureS[27].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[27].fU                   = u4;
       	TextureS[27].fV                   = v4;
        zam+=mMapWidth;         
       	TextureS[28].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);       
       	TextureS[28].fU                   = u3;
       	TextureS[28].fV                   = v5;
       	TextureS[29].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[29].fU                   = u4;
       	TextureS[29].fV                   = v5; 


 	
    InitCZTs(TN);	
    ExitCZTs(30);  

flags>>=1;
//if (flags<1) return;
  zam=ozam;
   TN = TextureNumber[zam][2];
   du=ENGINE.GetMapTexture(TN)->mW /4;
   dv=ENGINE.GetMapTexture(TN)->mH /4;
  
   u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
   u2 = ENGINE.GetMapTexture(TN)->mX+(du);
   u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
   u4 = ENGINE.GetMapTexture(TN)->mX+(du*3)-0.00390635f;
   u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
   v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
   v4 = ENGINE.GetMapTexture(TN)->mY+(dv);
   v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
   v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
   v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
  kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );	
//  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
//  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
  
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        
        TextureS[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	TextureS[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
        zam+=mMapWidth;         
       	TextureS[8].uBaseRGB.dwPacked       =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[8].fU                   = u1;
       	TextureS[8].fV                   = v5;
       	TextureS[9].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[9].fU                   = u2;
       	TextureS[9].fV                   = v5;
   		// 4x1 - 2 ****************************************************       	       	
        zam+=1;
        TextureS[10].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[10].fU                   = u2;
        TextureS[10].fV                   = v5;        
        TextureS[11].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[11].fU                   = u3;
        TextureS[11].fV                   = v5;         
        zam-=mMapWidth;        
        TextureS[12].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[12].fU                   = u2;
        TextureS[12].fV                   = v4;
        TextureS[13].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[13].fU                   = u3;
        TextureS[13].fV                   = v4; 
        zam-=mMapWidth;         
       	TextureS[14].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v3;
       	TextureS[15].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v3;
        zam-=mMapWidth;         
       	TextureS[16].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[16].fU                   = u2;
       	TextureS[16].fV                   = v2;
       	TextureS[17].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);       
       	TextureS[17].fU                   = u3;
       	TextureS[17].fV                   = v2;
        zam-=mMapWidth;         
       	TextureS[18].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[18].fU                   = u2;
       	TextureS[18].fV                   = v1;
       	TextureS[19].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);       
       	TextureS[19].fU                   = u3;
       	TextureS[19].fV                   = v1;  
    	// 4x1 - 3 ****************************************************
       	zam+=1;
        TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);       
        TextureS[20].fU                   = u3;
        TextureS[20].fV                   = v1;        
        TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[21].fU                   = u4;
        TextureS[21].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[22].fU                   = u3;
        TextureS[22].fV                   = v2;
        TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[23].fU                   = u4;
        TextureS[23].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[24].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);       
       	TextureS[24].fU                   = u3;
       	TextureS[24].fV                   = v3;
       	TextureS[25].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[25].fU                   = u4;
       	TextureS[25].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[26].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[26].fU                   = u3;
       	TextureS[26].fV                   = v4;
       	TextureS[27].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[27].fU                   = u4;
       	TextureS[27].fV                   = v4;
        zam+=mMapWidth;         
       	TextureS[28].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);       
       	TextureS[28].fU                   = u3;
       	TextureS[28].fV                   = v5;
       	TextureS[29].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[29].fU                   = u4;
       	TextureS[29].fV                   = v5; 



 	
    InitCZTs(TN);	
    ExitCZTs(30);  
    }
}

void CDCMap::DcRender3x3(int x,int y)
{

// if (x<0 || x>(mMapWidth>>2)) return;
// if (y<0 || y>(mMapHeight>>2)) return;

 // Trivial Reject

 int zam = (x<<2) +((y<<2)*mMapWidth);
 int ozam=zam; 
 int flags=Flaggy[zam];
 float x1=Trans[zam].x;
 float y1=Trans[zam].y;

 float x2=Trans[zam+3].x;
 float y2=Trans[zam+3].y;

 float x3=Trans[zam+(mMapWidth*3)].x;
 float y3=Trans[zam+(mMapWidth*3)].y;

 float x4=Trans[zam+3+(mMapWidth*3)].x;
 float y4=Trans[zam+3+(mMapWidth*3)].y;

 float  z1=    Trans[zam].z;
 float  z2=    Trans[zam+3].z;   
 float  z3=    Trans[zam+(mMapWidth*3)].z;   
 float  z4=    Trans[zam+3+(mMapWidth*3)].z;   

if ((x1>(IX1-SSCF) && x1<(IX2+SSCF)) || (x2>(IX1-SSCF) && x2<(IX2+SSCF)) || (x3>(IX1-SSCF) && x3<(IX2+SSCF)) || (x4>(IX1-SSCF) && x4<(IX2+SSCF)))
if ((y1>(-SSCF) && y1<(479+SSCF)) || (y2>-SSCF && y2<(479+SSCF)) || (y3>-SSCF && y3<(479+SSCF)) || (y4>-SSCF && y4<(479+SSCF)))
 if (z1<0 || z2<0 || z3<0 || z4<0)
  {
	TransformSome((x << 2), (y << 2), 4, 4, 1);
 
  int TN = TextureNumber[zam][0];


  
  float du=ENGINE.GetMapTexture(TN)->mW /4;
  float dv=ENGINE.GetMapTexture(TN)->mH /4;
  
  float u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
  float u2 = ENGINE.GetMapTexture(TN)->mX+(du);
  float u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
  float u4 = ENGINE.GetMapTexture(TN)->mX+(du*3)-0.00390635f;
  float u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
  	
  float v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
  float v4 = ENGINE.GetMapTexture(TN)->mY+(dv);
  float v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
  float v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
  float v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
 
  //TN=TN-1;
    
	kmChangeStripListType( &TextureHead[TN], KM_OPAQUE_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ZERO);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE );
    
        InitCZTs(TN);
   
   		//*************************************************************
   		//********************** 4x1 - 1 ******************************
   		//*************************************************************
  		
        TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[0].fX                   = Trans[zam].x;;
        TextureS[0].fY                   =     Trans[zam].y;
        TextureS[0].u.fZ                 =    -Trans[zam].z;
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        


        TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[1].fX                   = Trans[zam+1].x;
        TextureS[1].fY                   =     Trans[zam+1].y;
        TextureS[1].u.fZ                 =    -Trans[zam+1].z;
        TextureS[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        

        TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[2].fX                   = Trans[zam].x;
        TextureS[2].fY                   =     Trans[zam].y;
        TextureS[2].u.fZ                 =    -Trans[zam].z;
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
    
       
        TextureS[3].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[3].fX                   = Trans[zam+1].x;;
        TextureS[3].fY                   =     Trans[zam+1].y;
        TextureS[3].u.fZ                 =    -Trans[zam+1].z;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        
        zam+=mMapWidth;         
        
       	TextureS[4].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[4].fX                   = Trans[zam].x;
       	TextureS[4].fY                   =     Trans[zam].y;
       	TextureS[4].u.fZ                 =    -Trans[zam].z;
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	
       	
       	TextureS[5].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[5].fX                   = Trans[zam+1].x;
       	TextureS[5].fY                   =     Trans[zam+1].y;
       	TextureS[5].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        

        
        zam+=mMapWidth;         
        
       	TextureS[6].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[6].fX                   = Trans[zam].x;
       	TextureS[6].fY                   =     Trans[zam].y;
       	TextureS[6].u.fZ                 =    -Trans[zam].z;
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	
       	
       	TextureS[7].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[7].fX                   = Trans[zam+1].x;
       	TextureS[7].fY                   =     Trans[zam+1].y;
       	TextureS[7].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
       	
  		// 4x1 - 2 ****************************************************       	       	
       	
        zam+=1;

        TextureS[8].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[8].fX                   = Trans[zam].x;
        TextureS[8].fY                   =     Trans[zam].y;
        TextureS[8].u.fZ                 =    -Trans[zam].z;
        TextureS[8].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[8].fU                   = u2;
        TextureS[8].fV                   = v4;
    
       
        TextureS[9].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[9].fX                   = Trans[zam+1].x;;
        TextureS[9].fY                   =     Trans[zam+1].y;
        TextureS[9].u.fZ                 =    -Trans[zam+1].z;
        TextureS[9].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[9].fU                   = u3;
        TextureS[9].fV                   = v4; 
        
        zam-=mMapWidth;         
        
       	TextureS[10].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[10].fX                   = Trans[zam].x;
       	TextureS[10].fY                   =     Trans[zam].y;
       	TextureS[10].u.fZ                 =    -Trans[zam].z;
       	TextureS[10].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[10].fU                   = u2;
       	TextureS[10].fV                   = v3;
       	
       	
       	TextureS[11].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[11].fX                   = Trans[zam+1].x;
       	TextureS[11].fY                   =     Trans[zam+1].y;
       	TextureS[11].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[11].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[11].fU                   = u3;
       	TextureS[11].fV                   = v3;
        
        zam-=mMapWidth;         
        
       	TextureS[12].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[12].fX                   = Trans[zam].x;
       	TextureS[12].fY                   =     Trans[zam].y;
       	TextureS[12].u.fZ                 =    -Trans[zam].z;
       	TextureS[12].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[12].fU                   = u2;
       	TextureS[12].fV                   = v2;
       	
       	
       	TextureS[13].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[13].fX                   = Trans[zam+1].x;
       	TextureS[13].fY                   =     Trans[zam+1].y;
       	TextureS[13].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[13].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);       
       	TextureS[13].fU                   = u3;
       	TextureS[13].fV                   = v2;
       	
        zam-=mMapWidth;         
        
       	TextureS[14].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[14].fX                   = Trans[zam].x;
       	TextureS[14].fY                   =     Trans[zam].y;
       	TextureS[14].u.fZ                 =    -Trans[zam].z;
       	TextureS[14].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v1;
       	
       	
       	TextureS[15].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[15].fX                   = Trans[zam+1].x;
       	TextureS[15].fY                   =     Trans[zam+1].y;
       	TextureS[15].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[15].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);       
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v1;  
       	
    	// 4x1 - 3 ****************************************************
       	
       	zam+=1;
        TextureS[16].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[16].fX                   = Trans[zam].x;;
        TextureS[16].fY                   =     Trans[zam].y;
        TextureS[16].u.fZ                 =    -Trans[zam].z;
        TextureS[16].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);       
        TextureS[16].fU                   = u3;
        TextureS[16].fV                   = v1;        


        TextureS[17].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[17].fX                   = Trans[zam+1].x;
        TextureS[17].fY                   =     Trans[zam+1].y;
        TextureS[17].u.fZ                 =    -Trans[zam+1].z;
        TextureS[17].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[17].fU                   = u4;
        TextureS[17].fV                   = v1;         
        zam+=mMapWidth;        

        TextureS[18].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[18].fX                   = Trans[zam].x;
        TextureS[18].fY                   =     Trans[zam].y;
        TextureS[18].u.fZ                 =    -Trans[zam].z;
        TextureS[18].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
        TextureS[18].fU                   = u3;
        TextureS[18].fV                   = v2;
    
       
        TextureS[19].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
        TextureS[19].fX                   = Trans[zam+1].x;;
        TextureS[19].fY                   =     Trans[zam+1].y;
        TextureS[19].u.fZ                 =    -Trans[zam+1].z;
        TextureS[19].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
        TextureS[19].fU                   = u4;
        TextureS[19].fV                   = v2; 
        
        zam+=mMapWidth;         
        
       	TextureS[20].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[20].fX                   = Trans[zam].x;
       	TextureS[20].fY                   =     Trans[zam].y;
       	TextureS[20].u.fZ                 =    -Trans[zam].z;
       	TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);       
       	TextureS[20].fU                   = u3;
       	TextureS[20].fV                   = v3;
       	
       	
       	TextureS[21].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[21].fX                   = Trans[zam+1].x;
       	TextureS[21].fY                   =     Trans[zam+1].y;
       	TextureS[21].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[21].fU                   = u4;
       	TextureS[21].fV                   = v3;
        
        zam+=mMapWidth;         
        
       	TextureS[22].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
       	TextureS[22].fX                   = Trans[zam].x;
       	TextureS[22].fY                   =     Trans[zam].y;
       	TextureS[22].u.fZ                 =    -Trans[zam].z;
       	TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][0], DColours[zam]);        
       	TextureS[22].fU                   = u3;
       	TextureS[22].fV                   = v4;
       	
       	
       	TextureS[23].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
       	TextureS[23].fX                   = Trans[zam+1].x;
       	TextureS[23].fY                   =     Trans[zam+1].y;
       	TextureS[23].u.fZ                 =    -Trans[zam+1].z;
       	TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][0], DColours[zam+1]);        
       	TextureS[23].fU                   = u4;
       	TextureS[23].fV                   = v4;
       	

     	
	ExitCZTs(24);

flags>>=1;
//if (flags<1) return;
		
//    kmChangeStripBlendingMode       ( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
// 	kmChangeStripTextureShadingMode ( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_MODULATE);	
// 	kmChangeStripUseAlpha			( &TextureHead[TN-1], KM_IMAGE_PARAM1, KM_TRUE );
 	
zam=ozam;
  TN = TextureNumber[zam][1];
  du=ENGINE.GetMapTexture(TN)->mW /4;
  dv=ENGINE.GetMapTexture(TN)->mH /4;
  
  u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
  u2 = ENGINE.GetMapTexture(TN)->mX+(du);
  u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
  u4 = ENGINE.GetMapTexture(TN)->mX+(du*3)-0.00390635f;
  u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
  v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
  v4 = ENGINE.GetMapTexture(TN)->mY+(dv);
  v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
  v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
  v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
  kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );	
//  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
//  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
  
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        
        TextureS[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	TextureS[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
   		// 4x1 - 2 ****************************************************       	       	
        zam+=1;
        TextureS[8].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[8].fU                   = u2;
        TextureS[8].fV                   = v4;
        TextureS[9].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[9].fU                   = u3;
        TextureS[9].fV                   = v4; 
        zam-=mMapWidth;         
       	TextureS[10].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[10].fU                   = u2;
       	TextureS[10].fV                   = v3;
       	TextureS[11].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[11].fU                   = u3;
       	TextureS[11].fV                   = v3;
        zam-=mMapWidth;         
       	TextureS[12].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[12].fU                   = u2;
       	TextureS[12].fV                   = v2;
       	TextureS[13].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);       
       	TextureS[13].fU                   = u3;
       	TextureS[13].fV                   = v2;
        zam-=mMapWidth;         
       	TextureS[14].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v1;
       	TextureS[15].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);       
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v1;  
    	// 4x1 - 3 ****************************************************
       	zam+=1;
        TextureS[16].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);       
        TextureS[16].fU                   = u3;
        TextureS[16].fV                   = v1;        
        TextureS[17].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[17].fU                   = u4;
        TextureS[17].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[18].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
        TextureS[18].fU                   = u3;
        TextureS[18].fV                   = v2;
        TextureS[19].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
        TextureS[19].fU                   = u4;
        TextureS[19].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);       
       	TextureS[20].fU                   = u3;
       	TextureS[20].fV                   = v3;
       	TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[21].fU                   = u4;
       	TextureS[21].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][1], DColours[zam]);        
       	TextureS[22].fU                   = u3;
       	TextureS[22].fV                   = v4;
       	TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][1], DColours[zam+1]);        
       	TextureS[23].fU                   = u4;
       	TextureS[23].fV                   = v4;
 	
    InitCZTs(TN);	
    ExitCZTs(24);  

flags>>=1;
//if (flags<1) return;
  zam=ozam;
   TN = TextureNumber[zam][2];
   du=ENGINE.GetMapTexture(TN)->mW /4;
   dv=ENGINE.GetMapTexture(TN)->mH /4;
  
   u1 = ENGINE.GetMapTexture(TN)->mX+0.00390635f;
   u2 = ENGINE.GetMapTexture(TN)->mX+(du);
   u3 = ENGINE.GetMapTexture(TN)->mX+(du*2);
   u4 = ENGINE.GetMapTexture(TN)->mX+(du*3)-0.00390635f;
   u5 = ENGINE.GetMapTexture(TN)->mX+(du*4)-0.00390635f;        
   v5 = ENGINE.GetMapTexture(TN)->mY+0.00390635f;
   v4 = ENGINE.GetMapTexture(TN)->mY+(dv);
   v3 = ENGINE.GetMapTexture(TN)->mY+(dv*2);
   v2 = ENGINE.GetMapTexture(TN)->mY+(dv*3);
   v1 = ENGINE.GetMapTexture(TN)->mY+(dv*4)-0.00390635f; 	
  TN = ENGINE.GetMapTexture(TN)->mTPage+1;
  kmChangeStripListType( &TextureHead[TN], KM_TRANS_POLYGON );	
//  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
//  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE, KM_ONE);
  kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);	
  kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE ); 	
  
        TextureS[0].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[0].fU                   = u1;
        TextureS[0].fV                   = v1;        
        TextureS[1].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[1].fU                   = u2;
        TextureS[1].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[2].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[2].fU                   = u1;
        TextureS[2].fV                   = v2;
        TextureS[3].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[3].fU                   = u2;
        TextureS[3].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[4].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[4].fU                   = u1;
       	TextureS[4].fV                   = v3;
       	TextureS[5].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[5].fU                   = u2;
       	TextureS[5].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[6].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[6].fU                   = u1;
       	TextureS[6].fV                   = v4;
       	TextureS[7].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[7].fU                   = u2;
       	TextureS[7].fV                   = v4;
   		// 4x1 - 2 ****************************************************       	       	
        zam+=1;
        TextureS[8].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[8].fU                   = u2;
        TextureS[8].fV                   = v4;
        TextureS[9].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[9].fU                   = u3;
        TextureS[9].fV                   = v4; 
        zam-=mMapWidth;         
       	TextureS[10].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[10].fU                   = u2;
       	TextureS[10].fV                   = v3;
       	TextureS[11].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[11].fU                   = u3;
       	TextureS[11].fV                   = v3;
        zam-=mMapWidth;         
       	TextureS[12].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[12].fU                   = u2;
       	TextureS[12].fV                   = v2;
       	TextureS[13].uBaseRGB.dwPacked          =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);       
       	TextureS[13].fU                   = u3;
       	TextureS[13].fV                   = v2;
        zam-=mMapWidth;         
       	TextureS[14].uBaseRGB.dwPacked         =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[14].fU                   = u2;
       	TextureS[14].fV                   = v1;
       	TextureS[15].uBaseRGB.dwPacked            =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);       
       	TextureS[15].fU                   = u3;
       	TextureS[15].fV                   = v1;  
    	// 4x1 - 3 ****************************************************
       	zam+=1;
        TextureS[16].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);       
        TextureS[16].fU                   = u3;
        TextureS[16].fV                   = v1;        
        TextureS[17].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[17].fU                   = u4;
        TextureS[17].fV                   = v1;         
        zam+=mMapWidth;        
        TextureS[18].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
        TextureS[18].fU                   = u3;
        TextureS[18].fV                   = v2;
        TextureS[19].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
        TextureS[19].fU                   = u4;
        TextureS[19].fV                   = v2; 
        zam+=mMapWidth;         
       	TextureS[20].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);       
       	TextureS[20].fU                   = u3;
       	TextureS[20].fV                   = v3;
       	TextureS[21].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[21].fU                   = u4;
       	TextureS[21].fV                   = v3;
        zam+=mMapWidth;         
       	TextureS[22].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam][2], DColours[zam]);        
       	TextureS[22].fU                   = u3;
       	TextureS[22].fV                   = v4;
       	TextureS[23].uBaseRGB.dwPacked           =   modulate_colour(MixEE[zam+1][2], DColours[zam+1]);        
       	TextureS[23].fU                   = u4;
       	TextureS[23].fV                   = v4;
 	
    InitCZTs(TN);	
    ExitCZTs(24);  
    }
}




//********************************************************************
ULONG	JCLBlend(ULONG c, SINT blend)
{
	// Blend Colour
	SINT	r, g, b;
	
	r = (c >>  0) & 0xff;
	g = (c >>  8) & 0xff;
	b = (c >> 16) & 0xff;
	
	r = (r * blend) / 15;
	g = (g * blend) / 15;
	b = (b * blend) / 15;
	
	return b + (g << 8) + (r << 16) + (0xff << 24);  //swap RGB as well....
}

//********************************************************************
float	get_map_u(SINT c0)
{
	c0 ++;

	switch (c0)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
		return 0.002f;
	case 2:
	case 4:
	case 6:
	case 8:
	case 10:
	case 11:
	case 13:
	case 15:
	case 17:
	case 19:
		return 0.25f;
	case 12:
	case 14:
	case 16:
	case 18:
	case 20:
	case 21:
	case 23:
	case 25:
	case 27:
	case 29:
		return 0.5f;
	case 22:
	case 24:
	case 26:
	case 28:
	case 30:
	case 31:
	case 33:
	case 35:
	case 37:
	case 39:
		return 0.75f;
	case 32:
	case 34:
	case 36:
	case 38:
	case 40:
		return 0.998f;
	};
	return 0.f;
}

//********************************************************************
float	get_map_v(SINT c0)
{
	c0 ++;
	
	switch(c0)
	{
	case 1:
	case 2:
	case 19:
	case 20:
	case 21:
	case 22:
	case 39:
	case 40:
		return 0.998f;
	case 3:
	case 4:
	case 17:
	case 18:
	case 23:
	case 24:
	case 37:
	case 38:
		return 0.75f;
	case 5:
	case 6:
	case 15:
	case 16:
	case 25:
	case 26:
	case 35:
	case 36:
		return 0.5f;
	case 7:
	case 8:
	case 13:
	case 14:
	case 27:
	case 28:
	case 33:
	case 34:
		return 0.25f;
	case 9:
	case 10:
	case 11:
	case 12:
	case 29:
	case 30:
	case 31:
	case 32:
		return 0.002f;
	};
	return 0.f;
}

//********************************************************************
float	get_map_u2(SINT c0)
{
	c0 ++;
	
	switch (c0)
	{
		case 1:
		case 3:
		case 5:
			return 0.002f;
		case 2:
		case 4:
		case 6:
		case 7:
		case 9:
		case 11:
			return 0.5f;
		case 8:
		case 10:
		case 12:
			return 0.998f;
	};
	return 0;
	
}

//********************************************************************
float	get_map_v2(SINT c0)
{
	c0 ++;
	
	switch (c0)
	{
		case 1:
		case 2:
		case 11:
		case 12:
			return 0.998f;
		case 3:
		case 4:
		case 9:
		case 10:
			return 0.5f;
		case 5:
		case 6:
		case 7:
		case 8:
			return 0.002f;
	};
	return 0;
}

//********************************************************************
float	get_map_u3(SINT c0)
{
	c0 ++;
	
	switch (c0)
	{
	case 1:
	case 3:
		return 0.002f;
	case 2:
	case 4:
		return 0.998f;
	};
	return 0;
}		

//********************************************************************
float	get_map_v3(SINT c0)
{
	c0 ++;
	
	switch (c0)
	{
	case 1:
	case 2:
		return 0.998f;
	case 3:
	case 4:
		return 0.002f;
	};
	return 0;
}		

//********************************************************************
void CDCMap::InitMappy()
{
	int 	zam=0;
	int 	cx,cy;
	int 	startX,startY,endX,endY;
	
	for (int y=0;y<mMapHeight;y++)
	{
		for (int x=0;x<mMapWidth;x++)
		{
			CCellRef c1 = CCellRef(x, y);
			CMapCell *tc = GetCell(c1);		
		
			float d = GToF((tc + 1)->GetHeight() - tc->GetHeight());
			
			int col;
			
			if (d >  1.f) d =  1.f;
			if (d < -1.f) d = -1.f;
			
			SINT g = (d < 0) ? 170 - SINT((-d) * 130.f) : 170 - SINT(d * 0.f);
			SINT r = (d < 0) ? 170 - SINT((-d) * 130.f) : 170 + SINT(d * 80.f);
			SINT b = (d < 0) ? 170                      : 170 - SINT(d * 40.f) ;
			
			col = (r << 16) + (g << 8) + b;
			DColours[x+(y*mMapWidth)]=col;
			Mappy[x+(y*mMapWidth)].y=GToF(tc->GetHeight());   // Damn forgot to do the GToF();
			tc++;
		}
	}
	for (int a=0;a<mMapHeight;a++)
 	{
		for (int b=0;b<mMapWidth;b++)
  		{
			CCellRef c = CCellRef(b,a);
			c.X=b;
			c.Y=a;
			CMapCell *tc=GetCell(c);
			GINT h=tc->GetUnderlyingHeight();
			
			sheight[zam]=(h.GetVal() >> 16);
			
			// JCL - different vertex colours for Additive blend.
			SINT 	mix0 = (tc->mMix     ) & 0xf;
			SINT 	mix1 = (tc->mMix >> 4) & 0xf;
			SINT		mix2 = 15 - mix0 - mix1;
			
			MixEE[zam][0] = JCLBlend(tc->mColour, mix0);
			MixEE[zam][1] = JCLBlend(tc->mColour, mix1);
			MixEE[zam][2] = JCLBlend(tc->mColour, mix2);
			
			Flaggy[zam]=Texnums[((a>>2)*(mMapWidth>>2))+(b>>2)].mFlags;
			TextureNumber[zam][0] = (Texnums[((a>>2)*(mMapWidth>>2))+(b>>2)].tex1); //cell->mTextureNo;
			TextureNumber[zam][1] = (Texnums[((a>>2)*(mMapWidth>>2))+(b>>2)].tex2); //cell->mTextureNo;
			TextureNumber[zam][2] = (Texnums[((a>>2)*(mMapWidth>>2))+(b>>2)].tex3); //cell->mTextureNo;      
			Mappy[zam].x=(c.X);
			Mappy[zam].y=GToF(tc->GetHeight());   // Damn forgot to do the GToF();
			Mappy[zam].z=(c.Y);    
			zam++;
		}
	}
 
	// JCL - evaluate flags on 4x4 basis
	SINT	c0, c1, c2, c3;
	for (c0 = 0; c0 < mMapWidth >> 2; c0 ++)
	{
		for (c1 = 0; c1 < mMapHeight >> 2; c1 ++)
		{
			SINT	flags = 0;
			
			for (c2 = 0; c2 < 4; c2 ++)
			{
				for (c3 = 0; c3 < 4; c3 ++)
				{
					flags |= MAP.GetCell(CCellRef(c0 * 4 + c2, c1 * 4 + c3))->mFlags & 0xf;
				}
			}
			
			FFlags[c0 + (c1 * (mMapWidth >> 2))] = flags;
		}
	}
	
	// JCL - prebuild the vertex buffers
	for (c1 = 0; c1 < 40; c1 ++)
	{
		if (c1 != 39)
			MapVertices[c1].ParamControlWord = KM_VERTEXPARAM_NORMAL;
		else
			MapVertices[c1].ParamControlWord = KM_VERTEXPARAM_ENDOFSTRIP;

	    MapVertices[c1].fU = get_map_u(c1);
	    MapVertices[c1].fV = get_map_v(c1);
	}       	
	for (c1 = 0; c1 < 12; c1 ++)
	{
		if (c1 != 11)
			MapVertices2[c1].ParamControlWord = KM_VERTEXPARAM_NORMAL;
		else
			MapVertices2[c1].ParamControlWord = KM_VERTEXPARAM_ENDOFSTRIP;

	    MapVertices2[c1].fU = get_map_u2(c1);
	    MapVertices2[c1].fV = get_map_v2(c1);
	}       	
	
	for (c1 = 0; c1 < 4; c1 ++)
	{
		if (c1 != 3)
			MapVertices3[c1].ParamControlWord = KM_VERTEXPARAM_NORMAL;
		else
			MapVertices3[c1].ParamControlWord = KM_VERTEXPARAM_ENDOFSTRIP;

	    MapVertices3[c1].fU = get_map_u3(c1);
	    MapVertices3[c1].fV = get_map_v3(c1);
	}       	
}


// SDS GGrrrr I hate clipping code - it does my head in!!!
//********************************************************

BOOL ClipToZ(float z,KMVERTEX_03 *lp1,KMVERTEX_03 *lp2)
{
float du,dv,x1,y1,x2,y2,dx,dy,dz,incr;
BOOL clippy;

 clippy=FALSE;

 if ((lp1->u.fZ>=z)  && (lp2->u.fZ>=z ))  return FALSE;
 if ((lp1->u.fZ<z)  && (lp2->u.fZ<z ))  return TRUE;
  
 dx=lp2->fX-lp1->fX;
 dy=lp2->fY-lp1->fY;
 dz=lp2->u.fZ-lp1->u.fZ;
 du=lp2->fU-lp1->fU;
 dv=lp2->fV-lp1->fV;
 
 if (dz==0) return FALSE;
 
 dz=1/dz;

 incr=z-lp1->u.fZ;

 clippy=TRUE;

 if (lp1->u.fZ>lp2->u.fZ)
 	{
    lp1->fX=(dx*dz*incr)+lp1->fX;
    lp1->fY=(dy*dz*incr)+lp1->fY;
    lp1->fU=(du*dz*incr)+lp1->fU;
    lp1->fV=(dv*dz*incr)+lp1->fV;
    lp1->u.fZ=z;
   }
  else
   {
    lp2->fX=(dx*dz*incr)+lp1->fX;
    lp2->fY=(dy*dz*incr)+lp1->fY;
    lp2->fU=(du*dz*incr)+lp1->fU;
    lp2->fV=(dv*dz*incr)+lp1->fV;
    lp2->u.fZ=z;
    }
return clippy;    
   
}




void CDCMap::CalcDeltaColours(void)
{
	int zam=0;
	
	GVector Cam = ENGINE.GetCamera()->mLookAt;
	
	int cx,cy;
	int startX,startY,endX,endY;
	
	cx=Cam.X.Whole();
	cy=Cam.Y.Whole();
	
	SINT	LSize;
	
	if (GAME.IsMultiPlayer())
		LSize = 20;
	else
		LSize = 30;
	
	startX=(cx-LSize);
	endX=(cx+LSize);
	startY=(cy-LSize);
	endY=(cy+LSize);
	
	if (startX < 0) startX = 0;
	if (startY < 0) startY = 0;
	if (endX > mMapWidth) endX = mMapWidth;
	if (endY > mMapHeight) endY = mMapHeight;
	
	
	for (int y=startY;y<endY;y++)
	{
		CCellRef c1 = CCellRef(startX, y);
		  
		CMapCell *tc = GetCell(c1);
		for (int x=startX;x<endX;x++)
		{
			float d = GToF((tc + 1)->GetHeight() - tc->GetHeight());
			
			int col;
			
			if (d >  1.f) d =  1.f;
			if (d < -1.f) d = -1.f;
			
			SINT r, g, b;
			
			if (d < 0)
			{
				g = 170 - SINT((-d) * 130.f);
				r = 170 - SINT((-d) * 130.f);
				b = 170                     ;
			}
			else
			{
				g = 170 - SINT(d * 0.f);
				r = 170 + SINT(d * 80.f);
				b = 170 - SINT(d * 40.f) ;
			}
			
			col = (r << 16) + (g << 8) + b;
			DColours[x+(y*mMapWidth)]=col;
			Mappy[x+(y*mMapWidth)].y=GToF(tc->GetHeight());   // Damn forgot to do the GToF();
			tc ++;			
		}
	}
  
}

void	CDCMap::AddWaterParticles(SINT start_x, SINT start_y)
{
	SINT	wh=GAME.GetLevelData()->mGVars[WATER_HEIGHT].GetVal();

	if (!(GAME.IsGamePanelActive()))
	{
		if((wh) && ((rand() & 0xff) < 8))       // Do we want a water decal?
		{
			SINT 	wx, wy;
			wx = start_x + (rand() & 3);
			wy = start_y + (rand() & 3);

			SINT 	h1 = GetCell(CCellRef(wx, wy))->GetHeight().mVal;
			if(h1 > wh)
			{
				h1-=wh;
				SINT 	h2 = GetCell(CCellRef(wx+1, wy))->GetHeight().mVal-wh;
				SINT 	h3 = GetCell(CCellRef(wx+1, wy+1))->GetHeight().mVal-wh;
				SINT 	h4 = GetCell(CCellRef(wx, wy+1))->GetHeight().mVal-wh;
				if((h1|h2|h3|h4)<0)  // Only on water edge...
				{
					//printf("Edge!\n");
					//if((h1<0x10000)&&(h2<0x10000)&&(h3<0x10000)&&(h4<0x10000))  // Only shallow water/water edge.
					{
						CParticle *ptcl;
						ptcl	=mParticleSystem.CreateParticle(PART_ANIM);
						if(ptcl)
						{
							ptcl->mColour=0x00606050;
							ptcl->mPos.X.mVal=(wx << GINT_SHIFT) + (rand() & 0xffff);
							ptcl->mPos.Y.mVal=(wy << GINT_SHIFT) + (rand() & 0xffff);
							ptcl->mPos.Z.mVal=wh;
							//ptcl->mVelocity.X.mVal=0;
							//ptcl->mVelocity.Y.mVal=0x2000;

							GVector tgv=GetMapNormal(ptcl->mPos);
							ptcl->mVelocity.X.mVal=tgv.X.mVal>>3;
							ptcl->mVelocity.Y.mVal=tgv.Y.mVal>>3;
							ptcl->mVelocity.Z.mVal=0;

							GINT	scale = tgv.MagnitudeXY();
							if(scale.mVal>(0x8000))
							{
								ptcl->mVelocity.X.mVal>>=2;
								ptcl->mVelocity.Y.mVal>>=2;
							}
							if (!scale.IsZero())
							{
								scale = G1 / scale;
								tgv.X *= scale;
								tgv.Y *= scale;
							}
							ptcl->uPAnim.mSn=-tgv.X.mVal>>4;
							ptcl->uPAnim.mCs=tgv.Y.mVal>>4;
							ptcl->uPAnim.mFrame=TP_WAT00;
							ptcl->uPAnim.mRate=3;
							ptcl->uPAnim.mTick=3;
							ptcl->mRadius=180;
							ptcl->uPAnim.mFrames=6;
							ptcl->mDelay=1+(rand()&5);
							ptcl->uPAnim.mLoops=0;
							ptcl->uPAnim.mHoriz=2;  // Flags to use cs,sn instead of yaw....
						}
					}
				}
				else
				{
					SINT x=(wx<<GINT_SHIFT)+(rand()&0xffff);
					SINT y=(wy<<GINT_SHIFT)+(rand()&0xffff);
					CMapCell*cell=&mCells[((wy) << mMapWidthShift) + wx];
					SINT z=cell->GetHeight().mVal;
					CParticle *ptcl;
					for(int i=0;i<2;i++)
					{
						ptcl=mParticleSystem.CreateParticle(PART_WRING2);
						if(ptcl)
						{
							ptcl->mVelocity.Y.mVal=0;
							ptcl->mVelocity.Z.mVal=-0x100;
							ptcl->mDelay=i<<4;
							ptcl->mColour=0x00606050;
							ptcl->mPos.X.mVal=x;
							ptcl->mPos.Y.mVal=y;
							ptcl->mPos.Z.mVal=z;
						}
					}
				}
			}

		}
	}
}




//**************************************************************************************************
float f11,f12,f13,f21,f22,f23,f31,f32,f33;
float xAdd,yAdd,zAdd;

void SetupPersTrans(viewframe vfCam, viewframe vf)
{
float n11,n12,n13,n21,n22,n23,n31,n32,n33;
float c11,c12,c13,c21,c22,c23,c31,c32,c33;
int lastvertex,pt;
float xw,yw,zw;
float xv,yv,zv;



n11=vf.iv[0];
n12=vf.iv[1];
n13=vf.iv[2];
n21=vf.jv[0];
n22=vf.jv[1];
n23=vf.jv[2];
n31=vf.kv[0];
n32=vf.kv[1];
n33=vf.kv[2];
c11=vfCam.iv[0];
c12=vfCam.iv[1];
c13=vfCam.iv[2];
c21=vfCam.jv[0];
c22=vfCam.jv[1];
c23=vfCam.jv[2];
c31=vfCam.kv[0];
c32=vfCam.kv[1];
c33=vfCam.kv[2];

f11=(c11*n11) + (c12*n12) + (c13*n13);
f12=(c11*n21) + (c12*n22) + (c13*n23);
f13=(c11*n31) + (c12*n32) + (c13*n33);
xAdd=((vf.x-vfCam.x)*c11) + ((vf.y-vfCam.y)*c12) + ((vf.z-vfCam.z)*c13);

f21=(c21*n11) + (c22*n12) + (c23*n13);
f22=(c21*n21) + (c22*n22) + (c23*n23);
f23=(c21*n31) + (c22*n32) + (c23*n33);
yAdd=((vf.x-vfCam.x)*c21) + ((vf.y-vfCam.y)*c22) + ((vf.z-vfCam.z)*c23);

f31=(c31*n11) + (c32*n12) + (c33*n13);
f32=(c31*n21) + (c32*n22) + (c33*n23);
f33=(c31*n31) + (c32*n32) + (c33*n33);
zAdd=((vf.x-vfCam.x)*c31) + ((vf.y-vfCam.y)*c32) + ((vf.z-vfCam.z)*c33);
}

//***************************************************************************************************
void SetupPersTransWrapper(const FVector LocalPos,const FMatrix LocalOrientation)
{
	viewframe cam;
	InitViewFrame(&cam);
	FVector CPos,DPos;
	// sds use looktat instead of the matrix
	CCamera *xcam =	ENGINE.GetCamera();
	CPos=GVectorToFVector(xcam->mPos);
	DPos=GVectorToFVector(xcam->mLookAt);
	
	cam.x=CPos.X;
	cam.z=CPos.Y;
	cam.y=CPos.Z;	
	
	cam.kv[0]=cam.x-DPos.X;
	cam.kv[1]=cam.y-DPos.Z;
	cam.kv[2]=cam.z-DPos.Y;
	
	float norm;
	norm=sqrt((cam.kv[0]*cam.kv[0])+(cam.kv[1]*cam.kv[1])+(cam.kv[2]*cam.kv[2]));
	
	cam.kv[0]=cam.kv[0]/norm;
	cam.kv[1]=cam.kv[1]/norm;
	cam.kv[2]=cam.kv[2]/norm;
	
	cam.jv[0]=cam.kv[2];
	cam.jv[1]=0;
	cam.jv[2]=-cam.kv[0];
	
	regen_cosinesK(cam);
	rot_vz(-3.14159265/2,cam);
	regen_cosinesK(cam);

	viewframe CharFrame;
	InitViewFrame(&CharFrame);
	
	
	CharFrame.kv[0]=LocalOrientation.Row[0].Y;
	CharFrame.kv[1]=LocalOrientation.Row[2].Y;			
	CharFrame.kv[2]=LocalOrientation.Row[1].Y;			

	CharFrame.jv[0]=LocalOrientation.Row[0].Z;
	CharFrame.jv[1]=LocalOrientation.Row[2].Z;			
	CharFrame.jv[2]=LocalOrientation.Row[1].Z;
	
	CharFrame.iv[0]=LocalOrientation.Row[0].X;
	CharFrame.iv[1]=LocalOrientation.Row[2].X;			
	CharFrame.iv[2]=LocalOrientation.Row[1].X;						

	CharFrame.x=LocalPos.X;
	CharFrame.y=LocalPos.Z;
	CharFrame.z=LocalPos.Y;			
	SetupPersTrans(cam,CharFrame);
}
//***************************************************************************************************
#define DCscale 2

void	CachedPersTrans(SINT vn, tvector *in, tvector *out, SINT stride)
{
	for (int c0=0;c0<vn;c0++)
	{
	   float	xw = in->x;
	   float	yw = in->y;
	   float	zw = in->z;
	 
	   float	zv=(xw*f31)  + (yw*f32) + (zw*f33)+zAdd;
	   float	xv=(xw*f11)  + (yw*f12) + (zw*f13)+xAdd;
	   zv=1/(zv);
	   float	yv=(xw*f21)  + (yw*f22) + (zw*f23)+yAdd;
	   out->x=(((-xv*320*(DCscale))*zv)+InternalViewOffset);
	   out->y=((yv*320*(DCscale))*zv)+240;
	   out->z=zv * 10;
	   in += stride;
	   out += stride;  
	}
}
//***************************************************************************************************
void	CDCMap::TransformAll()
{
	PROFILE_FN(RMTransform);
	
	SINT	c0;
	FVector WorldPos = FVector(0.0f,0.0f,0.0f);
	FMatrix WorldOri = ID_FMATRIX;
	
	SetupPersTransWrapper(WorldPos, WorldOri);
	
	// first transform the rows
	for (c0 = 0; c0 < mMapHeight - 1; c0 += 4)
	{
		CachedPersTrans(mMapWidth / 2, &Mappy[c0 * mMapWidth], &Trans[c0 * mMapWidth], 2);
	}
	
	// and the other bits 
	CachedPersTrans(mMapWidth  / 2, &Mappy[(mMapHeight - 1) * mMapWidth], &Trans[(mMapHeight - 1) * mMapWidth], 2);
	CachedPersTrans(mMapHeight / 4, &Mappy[(mMapWidth  - 1)            ], &Trans[(mMapWidth  - 1)            ], (mMapWidth * 4));
	
/*		PROFILE_FN(RMTransform);
		
		FVector WorldPos = FVector(0.0f,0.0f,0.0f);
		FMatrix WorldOri = ID_FMATRIX;
		PersTransWrapper(mMapWidth*mMapHeight,(tvector *)&Mappy,(tvector *)&Trans,WorldPos,WorldOri);*/
}

//#pragma optimization_level 0
//***************************************************************************************************
void	CDCMap::TransformSome(SINT sx, SINT sy, SINT nx, SINT ny, SINT stride)
{
	PROFILE_FN(RMTransform);

	FVector WorldPos = FVector(0.0f,0.0f,0.0f);
	FMatrix WorldOri = ID_FMATRIX;
	
	SINT	c0;
	
	for (c0 = sy; c0 < sy + ny * stride; c0 += stride)
		CachedPersTrans(nx, &Mappy[sx + (c0 * mMapWidth)], &Trans[sx + (c0 * mMapWidth)], stride);	

}
//#pragma optimization_level 4

//***************************************************************************************************
void    CDCMap::Render()
{
	{ 
		PROFILE_FN(RMDeltas);
		CalcDeltaColours();
	}
		
	{
		TransformAll();
/*		PROFILE_FN(RMTransform);
		
		FVector WorldPos = FVector(0.0f,0.0f,0.0f);
		FMatrix WorldOri = ID_FMATRIX;
		PersTransWrapper(mMapWidth*mMapHeight,(tvector *)&Mappy,(tvector *)&Trans,WorldPos,WorldOri);
*/	}
	
	for (int a=0;a<(mMapHeight>>2)-1;a++)
		for (int b=0;b<(mMapWidth>>2)-1;b++)
		{
			//if (1/(Trans[2+(b<<2)+(((a<<2)+2)*mMapWidth)].z)>(10.0f))
			{
			PROFILE_FN(RMDraw1);

			FVector l1 = GVectorToFVector(ENGINE.GetCamera()->mLookAt);
			FVector l2 = FVector(float(b) * 4.f, float(a) * 4.f, 0);
			
			float dist = (l1 - l2).MagnitudeSq();

//			if (dist < 1600.f)
			if (dist < 900.f)
				DcRender4x4(b, a);


//			if ((dist > 1179.5f) && (dist < 4900.f))
			if ((dist > 592.5f) && (dist < 4900.f))
//			if (dist < 4900.f)
				DcRender2x2(b, a);

			if (!GAME.IsMultiPlayer())
				if (dist > 4140.f)	
				  if (dist < 16384.f)			
					DcRender1x1(b, a);
			}
		}
	
	for (int b=0;b<(mMapWidth>>2)-1;b++)
	{
		PROFILE_FN(RMDraw2);
		FVector l1 = GVectorToFVector(ENGINE.GetCamera()->mLookAt);
		FVector l2 = FVector(float(b) * 4.f, float((mMapHeight/4)-1) * 4.f, 0);
		
		float dist = (l1 - l2).MagnitudeSq();
		if (dist < 16384.f)			
			DcRender4x3(b,(mMapHeight/4)-1);
	}
	
	for (int b=0;b<(mMapHeight>>2)-1;b++)
	{
		PROFILE_FN(RMDraw2);
		FVector l1 = GVectorToFVector(ENGINE.GetCamera()->mLookAt);
		FVector l2 = FVector(float((mMapWidth/4)-1) * 4.f, float(b) * 4.f, 0);
		
		float dist = (l1 - l2).MagnitudeSq();
		if (dist < 16384.f)			
			DcRender3x4((mMapWidth/4)-1,b);
	}  
	
	DcRender3x3((mMapWidth/4)-1, (mMapHeight/4)-1);

	{
		PROFILE_FN(RMDrawStadium);
		MapDrawBottomEdge();
		MapDrawTopEdge();
		MapDrawLeftEdge();
		MapDrawRightEdge();
	}

	{
		PROFILE_FN(RMDrawParticles);
		CDCParticleSystem::Render(&mParticleSystem);
		CDCParticleSystem::Render(&mEffectParticleSystem);
	}



#ifndef	JCLDCNOPSX	// do this!
	CPlayer *pl = PLAYER(0);
    DrawPickupBeam(pl);
#endif
	
if (GToF( GAME.GetLevelData()->mGVars[WATER_HEIGHT])==0) return;

	{
	PROFILE_FN(RMDrawWater);

	RenderWater();
//	return;
	
#if 1
	
	//***************************************
	// SDS - Render the water
	tvector WaterVerts[4];
	
	WaterVerts[0].x=0;
	WaterVerts[0].y=GToF( GAME.GetLevelData()->mGVars[WATER_HEIGHT]) + 0.02f;
	WaterVerts[0].z=0;
	
	WaterVerts[1].x=mMapWidth;
	WaterVerts[1].y=GToF( GAME.GetLevelData()->mGVars[WATER_HEIGHT]) + 0.02f;
	WaterVerts[1].z=0;
	
	WaterVerts[2].x=mMapWidth;
	WaterVerts[2].y=GToF( GAME.GetLevelData()->mGVars[WATER_HEIGHT]) + 0.02f; 
	WaterVerts[2].z=mMapHeight;
	
	WaterVerts[3].x=0;
	WaterVerts[3].y=GToF( GAME.GetLevelData()->mGVars[WATER_HEIGHT]) + 0.02f; 
	WaterVerts[3].z=mMapHeight; 
	
	STextureRefOther *wt=ENGINE.GetNumberedTexture(TN_WATER);
	
	FVector WorldPos = FVector(0.0f,0.0f,0.0f);
	FMatrix WorldOri = ID_FMATRIX;	
	
	PersTransWrapper2(4,(tvector *)&WaterVerts,(tvector *)&Trans,WorldPos,WorldOri);
	
	kmChangeStripListType			( &TextureHead[wt->mTPage+1], KM_TRANS_POLYGON );    
	kmChangeStripBlendingMode       ( &TextureHead[wt->mTPage+1], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
	kmChangeStripTextureShadingMode ( &TextureHead[wt->mTPage+1], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);
 	kmChangeStripUseAlpha			( &TextureHead[wt->mTPage+1], KM_IMAGE_PARAM1, KM_TRUE );
 	kmChangeStripClampUV			( &TextureHead[wt->mTPage+1], KM_IMAGE_PARAM1, KM_NOCLAMP);
	
	//*************************************************************
	//********************** 4x1 - 1 ******************************
	//*************************************************************
	
	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	TextureS[0].fX                   = Trans[0].x;;
	TextureS[0].fY                   =     Trans[0].y;
	TextureS[0].u.fZ                 =    Trans[0].z;
	//TextureS[0].uBaseRGB.dwPacked           =   0xff00ffff;        
	TextureS[0].fU                   = 0.0f;
	TextureS[0].fV                   = 0.0f;        
	
	
	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	TextureS[1].fX                   = Trans[1].x;
	TextureS[1].fY                   =     Trans[1].y;
	TextureS[1].u.fZ                 =    Trans[1].z;
	//TextureS[1].uBaseRGB.dwPacked           =   0xffff00ff;        
	TextureS[1].fU                   = 1.f * (mMapWidth / 4);
	TextureS[1].fV                   = 0.0f;         
	
	
	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	TextureS[2].fX                   = Trans[2].x;
	TextureS[2].fY                   =     Trans[2].y;
	TextureS[2].u.fZ                 =    Trans[2].z;
	//TextureS[2].uBaseRGB.dwPacked           =   0x4fffff00;        
	TextureS[2].fU                   = 1.f * (mMapWidth / 4);
	TextureS[2].fV                   = 1.f * (mMapHeight / 4);
	
	
	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
	TextureS[3].fX                   = Trans[3].x;;
	TextureS[3].fY                   =     Trans[3].y;
	TextureS[3].u.fZ                 =    Trans[3].z;
	//TextureS[3].uBaseRGB.dwPacked           =   0x4fffffff;        
	TextureS[3].fU                   = 0.0f;
	TextureS[3].fV                   = 1.f * (mMapHeight / 4); 

	/*
		InitCZTs(wt->mTPage+1);		
		TextureS[3].ParamControlWord =KM_VERTEXPARAM_ENDOFSTRIP;

	    ExitCZTs(4);
	    return;
	    */
		//**************
		// SDS Clip to Z
		//**************
//#pragma optimization_level 4		

		// SDS Clip Away
		
		KMVERTEX_03 TempArray[10];
		
		int lc=4;
		int nvc=0;
		float ZV = -0.1f;
		for (int lc=0;lc<4;lc++)
		 {
		 
 		  KMVERTEX_03 V1=TextureS[(lc)]; 
		  KMVERTEX_03 V2=TextureS[(lc+1) % 4];		 
		  
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
		kmStartStrip (&VertexBufferDesc, &TextureHead[wt->mTPage+1]);
		
		
		TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		TextureS[0].fX                   = TempArray[0].fX;
		TextureS[0].fY                   = TempArray[0].fY;
		TextureS[0].u.fZ                 = -TempArray[0].u.fZ;
		TextureS[0].uBaseRGB.dwPacked           =   0x1fffffff;        
		TextureS[0].fU                   = TempArray[0].fU;
		TextureS[0].fV                   = TempArray[0].fV;        
		
		
		TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		TextureS[1].fX                   = TempArray[a].fX;
		TextureS[1].fY                   = TempArray[a].fY;
		TextureS[1].u.fZ                 =  -TempArray[a].u.fZ;
		TextureS[1].uBaseRGB.dwPacked           =   0x1fffffff;        
		TextureS[1].fU                   = TempArray[a].fU;
		TextureS[1].fV                   = TempArray[a].fV;         
		
		
		TextureS[2].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
		TextureS[2].fX                   = TempArray[a+1].fX;
		TextureS[2].fY                   = TempArray[a+1].fY;
		TextureS[2].u.fZ                 =  -TempArray[a+1].u.fZ;
		TextureS[2].uBaseRGB.dwPacked           =   0x1fffffff;        
		TextureS[2].fU                   = TempArray[a+1].fU;
		TextureS[2].fV                   = TempArray[a+1].fV;         
		
		int y=sizeof(KMVERTEX_03);
		for (int i=0; i<=2; i++)
			kmSetVertex (&VertexBufferDesc, &TextureS[i], KM_VERTEXTYPE_03, sizeof (KMVERTEX_03));
		
		}

	    }
  
	kmChangeStripTextureShadingMode ( &TextureHead[wt->mTPage+1], KM_IMAGE_PARAM1, KM_MODULATE);
	}
return;

#endif
}

//******************************************************************************************
#define SKYSIZE	512.f

#define	WATER_SPLITSY_SP 	 4
#define	WATER_SPLITY_SIZE_SP (0.1f)
#define	WATER_SPLITSX_SP 	 4
#define	WATER_SPLITX_SIZE_SP (0.25f)

#define	WATER_SPLITSY_MP 	 2
#define	WATER_SPLITY_SIZE_MP (0.2f)
#define	WATER_SPLITSX_MP 	 2
#define	WATER_SPLITX_SIZE_MP (0.5f)

void	CDCMap::RenderWater()
{
	// JCL render the sky cube upside down... then bodge the z's
	float				water_height = GToF( GAME.GetLevelData()->mGVars[WATER_HEIGHT]);

	if (ENGINE.GetCamera()->mPos.Z > water_height)
		return; // don't draw!

	SINT	WATER_SPLITSY;
	float	WATER_SPLITY_SIZE;
	SINT	WATER_SPLITSX;
	float	WATER_SPLITX_SIZE;
	
	if (GAME.IsMultiPlayer())
	{
		WATER_SPLITSY		= WATER_SPLITSY_MP;
		WATER_SPLITY_SIZE	= WATER_SPLITY_SIZE_MP;
		WATER_SPLITSX		= WATER_SPLITSX_MP;
		WATER_SPLITX_SIZE	= WATER_SPLITX_SIZE_MP;
	}
	else
	{
		WATER_SPLITSY		= WATER_SPLITSY_SP;
		WATER_SPLITY_SIZE	= WATER_SPLITY_SIZE_SP;
		WATER_SPLITSX		= WATER_SPLITSX_SP;
		WATER_SPLITX_SIZE	= WATER_SPLITX_SIZE_SP;
	}

	
	SINT 	c1;
	for (c1 = 0; c1 < (WATER_SPLITSY * WATER_SPLITSX * 4) + 1; c1 ++)
	{
		FVector				x, y, o, cpos;
		STextureRefOther	*tr;
		
		SINT		spl = (c1 % (WATER_SPLITSY * WATER_SPLITSX));
		SINT		splitx = spl % WATER_SPLITSX;
		SINT		splity = spl / WATER_SPLITSX;
		
		SINT	pris;
		if (GAME.IsMultiPlayer())
			pris = CLevelStructure::GetPrisonForMultiplayerLevel(MULTIPLAYER_CAREER.mCurrentTournament, MULTIPLAYER_CAREER.mCurrentArena);
		else
			pris = CAREER.mCurrentPrison;
			
		BOOL		rot = (!(pris == 7));
			
		switch (c1 / (WATER_SPLITSY * WATER_SPLITSX))
		{
		case 0:
			tr = ENGINE.GetNumberedTexture(rot ? TN_SKY_UP : TN_SKY_DOWN);
			x = FVector(SKYSIZE, 0, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(0, SKYSIZE, 0);
			break;
		case 1:
			tr = ENGINE.GetNumberedTexture(rot ? TN_SKY_DOWN: TN_SKY_UP);
			x = FVector(-SKYSIZE, 0, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(0, -SKYSIZE, 0);
			break;
		case 2:
			tr = ENGINE.GetNumberedTexture(rot ? TN_SKY_RIGHT: TN_SKY_LEFT);
			x = FVector(0, -SKYSIZE, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(SKYSIZE, 0, 0);
			break;
		case 3:
			tr = ENGINE.GetNumberedTexture(rot ? TN_SKY_LEFT : TN_SKY_RIGHT);
			x = FVector(0, SKYSIZE, 0);
			y = FVector(0, 0, -SKYSIZE);
			o = FVector(-SKYSIZE, 0, 0);
			break;
		case 4:
			tr = ENGINE.GetNumberedTexture(TN_SKY_CENT);
			x = FVector(-SKYSIZE, 0, 0);
			y = FVector(0, SKYSIZE, 0);
			o = FVector(0, 0,  SKYSIZE);
			
			if (!rot)
			{
				x = -x; y = -y;
			}
			break;
		};
	
		cpos = GVectorToFVector(ENGINE.GetCamera()->mPos);
		
		if (c1 == (WATER_SPLITSY * WATER_SPLITSX * 4))
		{		
			WaterVerts[0] = -x - y + o + cpos;
			WaterVerts[1] =  x - y + o + cpos;
			WaterVerts[2] =  x + y + o + cpos;
			WaterVerts[3] = -x + y + o + cpos;
		}
		else
		{
			WaterVerts[0] = - x + (x * float(splitx    ) * WATER_SPLITX_SIZE * 2)
			                - y + (y * float(splity    ) * WATER_SPLITY_SIZE * 2)
			                + o + cpos;
			WaterVerts[1] = - x + (x * float(splitx + 1) * WATER_SPLITX_SIZE * 2)
						    - y + (y * float(splity    ) * WATER_SPLITY_SIZE * 2)
						    + o + cpos;
			WaterVerts[2] = - x + (x * float(splitx + 1) * WATER_SPLITX_SIZE * 2)
			  				- y + (y * float(splity + 1) * WATER_SPLITY_SIZE * 2)
			  				+ o + cpos;
			WaterVerts[3] = - x + (x * float(splitx    ) * WATER_SPLITX_SIZE * 2)
							- y + (y * float(splity + 1) * WATER_SPLITY_SIZE * 2)
							+ o + cpos;
		}
		
		FVector WorldPos = FVector(0.0f,0.0f,0.0f);
		FMatrix WorldOri = ID_FMATRIX;	
		
		SINT 	c0;
		BOOL	failed = FALSE;
		
		for (c0 = 0; c0 < 4; c0 ++)
		{
			FVector	wv  = WaterVerts[c0];
			
			FVector	d  = wv - cpos;
		
			float	l = d.Z;
			float	r = d.Z / (water_height - cpos.Z);
			
			if (r <= 0)
			{
				failed = TRUE;
				break;
			}
			
			FVector	nv = cpos + (d / r);
	
			WaterVerts[c0] = nv;		
		}
		if (failed)
			continue;
		
		ENGINE.SwapYZ(WaterVerts, 4);
		
		PersTransWrapper2(4,(tvector *)&WaterVerts,(tvector *)&Trans,ZERO_FVECTOR,ID_FMATRIX);
		
		
		SINT	TN = tr->mTPage + 1;
		
		kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
		kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
		kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE_ALPHA);
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
		
		float	u0, u1, v0, v1;
		
		if (c1 == (WATER_SPLITSY * WATER_SPLITSX * 4))
		{
/*			if (rot)
			{
				u1 = tr->mX + 0.004f;
				u0 = tr->mX + tr->mW - 0.004f;
				v1 = tr->mY + 0.004f;
				v0 = tr->mY + tr->mH - 0.004f;
			}
			else*/
			{
				u0 = tr->mX + 0.004f;
				u1 = tr->mX + tr->mW - 0.004f;
				v0 = tr->mY + 0.004f;
				v1 = tr->mY + tr->mH - 0.004f;
			}
		}
		else
		{
			u0 = tr->mX + (tr->mW * float(splitx    ) * WATER_SPLITX_SIZE) + 0.004f;
			u1 = tr->mX + (tr->mW * float(splitx + 1) * WATER_SPLITX_SIZE) + 0.004f;
			v1 = tr->mY + (tr->mH * float(splity    ) * WATER_SPLITY_SIZE) + 0.004f;
			v0 = tr->mY + (tr->mH * float(splity + 1) * WATER_SPLITY_SIZE) + 0.004f;
		}
		
		TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		TextureS[0].fX                   = Trans[0].x;;
		TextureS[0].fY                   =     Trans[0].y;
		TextureS[0].u.fZ                 =    Trans[0].z;
		//TextureS[0].uBaseRGB.dwPacked           =   0xff00ffff;        
		TextureS[0].fU                   = u0;
		TextureS[0].fV                   = v1;
		
		
		TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		TextureS[1].fX                   = Trans[1].x;
		TextureS[1].fY                   =     Trans[1].y;
		TextureS[1].u.fZ                 =    Trans[1].z;
		//TextureS[1].uBaseRGB.dwPacked           =   0xffff00ff;        
		TextureS[1].fU                   = u1;
		TextureS[1].fV                   = v1;
		
		
		TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
		TextureS[2].fX                   = Trans[2].x;
		TextureS[2].fY                   =     Trans[2].y;
		TextureS[2].u.fZ                 =    Trans[2].z;
		//TextureS[2].uBaseRGB.dwPacked           =   0x4fffff00;        
		TextureS[2].fU                   = u1;
		TextureS[2].fV                   = v0;
		
		
		TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
		TextureS[3].fX                   = Trans[3].x;;
		TextureS[3].fY                   =     Trans[3].y;
		TextureS[3].u.fZ                 =    Trans[3].z;
		//TextureS[3].uBaseRGB.dwPacked           =   0x4fffffff;        
		TextureS[3].fU                   = u0;
		TextureS[3].fV                   = v0;
	
		// SDS Clip Away
			
		KMVERTEX_03 TempArray[10];
		
		int lc=4;
		int nvc=0;
		float ZV = -0.1f;
		for (int lc=0;lc<4;lc++)
		{
			KMVERTEX_03 V1=TextureS[(lc)]; 
			KMVERTEX_03 V2=TextureS[(lc+1) % 4];		 
			  
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
	/*		// JCL - now hack the zs!
			// work out where the line from the camera to the vertex hits the water
			FVector c  = GVectorToFVector(ENGINE.GetCamera()->mPos);
			FVector	wv = FVector(TempArray[lc].fX, TempArray[lc].u.fZ, -TempArray[lc].fY);
			
			// convert back from camera space
	//		FMatrix	cmi = GMatrixToFMatrix(ENGINE.GetCamera()->mOrientation.Transpose());
			FMatrix	cmi = GMatrixToFMatrix(ENGINE.GetCamera()->mOrientation);
			wv  = cmi * wv;
			wv -= c;
			
			FVector	d  = wv - c;
	
			float	l = d.Z;
			float	r = d.Z / (water_height - c.Z);
			
			if (r <= 0)
				return; // aargh!
			
			FVector	nv = c + (d / r);
			
			float	foo  = nv.Z;
			nv.Z = nv.Y;
			nv.Y = foo;
			
			PersTransWrapper(1,(tvector *)&nv,(tvector *)&nv,WorldPos,WorldOri);
	*/	
			// now do the original perspective transform.	
			TempArray[lc].u.fZ=(1/TempArray[lc].u.fZ);
			TempArray[lc].fX= (((-TempArray[lc].fX*320*(2))*TempArray[lc].u.fZ)+InternalViewOffset);
			TempArray[lc].fY=((TempArray[lc].fY*320*(2))*TempArray[lc].u.fZ)+240;		 
			TempArray[lc].u.fZ=(TempArray[lc].u.fZ)*10;	
			
			// and overwrite with the new z.
	//		TempArray[lc].u.fZ = nv.Z;	 
		}
					
			// SDS Fan the bastard!!!!
		if (nvc>=3)
		{
			for (int a=1;a<=nvc-2;a++)
			{
				kmStartStrip (&VertexBufferDesc, &TextureHead[TN]);
				
				
				TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
				TextureS[0].fX                   = TempArray[0].fX;
				TextureS[0].fY                   = TempArray[0].fY;
				TextureS[0].u.fZ                 = -TempArray[0].u.fZ;
				TextureS[0].uBaseRGB.dwPacked           =   0x1fffffff;        
				TextureS[0].fU                   = TempArray[0].fU;
				TextureS[0].fV                   = TempArray[0].fV;        
				
				
				TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
				TextureS[1].fX                   = TempArray[a].fX;
				TextureS[1].fY                   = TempArray[a].fY;
				TextureS[1].u.fZ                 =  -TempArray[a].u.fZ;
				TextureS[1].uBaseRGB.dwPacked           =   0x1fffffff;        
				TextureS[1].fU                   = TempArray[a].fU;
				TextureS[1].fV                   = TempArray[a].fV;         
				
				
				TextureS[2].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
				TextureS[2].fX                   = TempArray[a+1].fX;
				TextureS[2].fY                   = TempArray[a+1].fY;
				TextureS[2].u.fZ                 =  -TempArray[a+1].u.fZ;
				TextureS[2].uBaseRGB.dwPacked           =   0x1fffffff;        
				TextureS[2].fU                   = TempArray[a+1].fU;
				TextureS[2].fV                   = TempArray[a+1].fV;         
				
				int y=sizeof(KMVERTEX_03);
				for (int i=0; i<=2; i++)
					kmSetVertex (&VertexBufferDesc, &TextureS[i], KM_VERTEXTYPE_03, sizeof (KMVERTEX_03));
			
			}
	
		}
	  
		kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);
	}
}

//******************************************************************************************
//GINT V1,S1;

BOOL    CDCMap::Read(CMEMBUFFER &mem)
{
//	ResetForVersionCompatibility();

//#if TARGET == PSX
//    printf("About to read map data\n");
//#endif

	//MakeMixers();
	// get version number
	UINT	ver;
	mem.Read(&ver, 4);

	// and load, based on the verson

    if (ver == MKID("1.70"))
	{
        //printf("Got version 1.50 Map!\n");
        SINT cx, cy;//,cx2,cy2;
		SWORD w,h;
		SINT basemix=0;
		mem.Read(&w, 2);
		mem.Read(&h, 2);

		// quick bodge for size
		SINT xs = 0, ys = 0;
		while ((w & 1) == 0)
		{
			w >>= 1;
			xs ++;
		}
		while ((h & 1) == 0)
		{
			h >>= 1;
			ys ++;
		}

		if (!(MAP.Resize(xs, ys)))
			return FALSE;


        //mem.Skip(256*MAX_MAP_TEXTURES);

        for (cy = 0; cy < MAP_HEIGHT; cy++)
		{
            for (cx = 0; cx < MAP_WIDTH; cx++)
			{
                CCellRef    c = CCellRef(cx, cy);
                //unsigned short mymix=0;
                //unsigned char curmix=0;
                SWORD    h;
				mem.Read(&h, 2);
				GetCell(c)->mHeight=h; //<<(HEIGHT_SHIFT-8);
				UBYTE t;
				mem.Read(&t, 1);
				GetCell(c)->mType=t;
				//t = (SINT *)(&GetCell(c)->mTextureNo);
				//mem.Read(t, 4);

//				mem.Read(&GetCell(c)->mColour, 4);

				SINT 	col;
				mem.Read(&col, 4);

//				GetCell(c)->mColour = col & 0x00ffffff;
				//swap b & r
				SINT	r = (col & 0x00ff0000) >> 16;
				SINT	b =  col & 0x000000ff;
				col &= 0x0000ff00;
				col += b << 16;
				col += r;

				GetCell(c)->mColour = col;

				//GetCell(c)->mColour = 0x7f7f7f7f;

// Here just read one byte for 1.50 and fiddle about with the 4-bit values for the two extra texture channels.
                mem.Read(&GetCell(c)->mMix, 1);  // Copy the mixer value into the structure
                mem.Read(&GetCell(c)->mRamp, 1);

                // and read friction
                // note that this is encoded in bits 0 and 8 of mColour..!!!
                UBYTE	fric;
	            mem.Read(&fric, 1);
                GetCell(c)->SetFriction(EFrictionType(fric));

			}
		}
		for (cy = 0; cy < (MAP_HEIGHT>>2); cy++)
		{
            for (cx = 0; cx < (MAP_WIDTH>>2); cx++)
			{
				//SDS Just in case of alignment problems!!
				mem.Read(&Texnums[(cy*(MAP_WIDTH>>2))+cx].tex1,1);
				mem.Read(&Texnums[(cy*(MAP_WIDTH>>2))+cx].tex2,1);
				mem.Read(&Texnums[(cy*(MAP_WIDTH>>2))+cx].tex3,1);				
		//printf("0: %03d  1: %03d  2: %03d\n",Texnums[(cy*(MAP_WIDTH>>2))+cx].tex1,Texnums[(cy*(MAP_WIDTH>>2))+cx].tex2,Texnums[(cy*(MAP_WIDTH>>2))+cx].tex3);
			}
		}
		PrecalcCells();
		return TRUE;
	}
	return FALSE;
}

//******************************************************************************************
#define separatemix(mix,v1,v2,v3) {(v1)=(mix&0x0f);(v2)=(mix>>4)&0x0f;(v3)=(15-((v1)+(v2)));}
#define swapchannels(a,b) \
{\
	unsigned char tmp,me,me2;\
	for(me=0;me<5;me++)\
		for(me2=0;me2<5;me2++)\
		{\
			tmp=m[me][me2][a-1];\
			m[me][me2][a-1]=m[me][me2][b-1];\
			m[me][me2][a-1]=tmp;\
			tmp=t->tex##a;\
			t->tex##a=t->tex##b;\
			t->tex##b=tmp;\
		}\
}

// This combines the mixers of channels a and b, removing channel b...
#define CombineLayers(a,b)\
{\
	unsigned char tmp,me,me2;\
	LayersCombined++;\
	for(me=0;me<5;me++)\
		for(me2=0;me2<5;me2++)\
		{\
			m[me][me2][a-1]+=m[me][me2][b-1];\
			m[me][me2][b-1]=0;\
		}\
	lu##a+=lu##b;\
	lu##b=0;\
}





void	CDCMap::PostLoadProcess()
{
	// analyse map heights for water check

	SINT	c0, c1, c2, c3, c4;
	GINT	wh = GAME.GetLevelData()->mGVars[WATER_HEIGHT];
	S4x4	*t = Texnums;

	for (c0 = 0; c0 < mMapHeight / 4; c0 ++)
	{
		for (c1 = 0; c1 < mMapWidth / 4; c1 ++)
		{
			GINT min = GINT_MAX;
			GINT max = GINT_MIN;

			for (c2 = 0; c2 < 4; c2 ++)
			{
				for (c3 = 0; c3 < 4; c3 ++)
				{
					GINT h = GetCell(CCellRef((c1 * 4) + c3, (c0 * 4) + c2))->GetUnderlyingHeight();

					if (h < min)
						min = h;

					if (h > max)
						max = h;
				}

			}





			//if(wh.GetVal()!=0)
				//if(min>wh)min=wh;
			// Now set up the mMinHeight and mMaxHeight values in the S4x4 structure...
			t->mMinHeight=min.GetVal()>>(GINT_SHIFT-PSXENGINE_SCALE);
			t->mMaxHeight=max.GetVal()>>(GINT_SHIFT-PSXENGINE_SCALE);

			t->mFlags &= ~WC_MASK;

			if ((wh == G0) || (max < wh - G(1)))
			{
				t->mFlags |= WC_NEVER;
			}
			else if (min.mVal > (wh.mVal + 0x00008000))
				t->mFlags |= WC_ALWAYS;
			else t->mFlags |= WC_CHECK;

/*			if((t->mFlags&WC_MASK)==WC_ALWAYS)
			{
				printf("Always: %03d,%03d\n",c1*4,c0*4);

			}
*/


			t++;
		}
	}

#if 0
// Now sort out the 25 mix values for each 4x4...
	t = Texnums;
	int savedcount=0;
	int LayersCombined=0;
	for (c0 = 0; c0 < mMapHeight ; c0 +=4)
	{
		for (c1 = 0; c1 < mMapWidth ; c1 +=4)
		{
			CMapCell *cell=&mCells[(c0 << mMapWidthShift) + c1];

			unsigned char allmix=0;
			unsigned char thismix=0;
			unsigned char l1,l2,l3;
			unsigned char lu1=0,lu2=0,lu3=0;
			unsigned char m[5][5][3];
			for(c3=0;c3<5;c3++)
			{
				for(c4=0;c4<5;c4++)
				{
					thismix=(cell+(c3<<mMapWidthShift)+c4)->mMix;
					separatemix(thismix,m[c3][c4][0],m[c3][c4][1],m[c3][c4][2]);
					if(m[c3][c4][0])lu1++;
					if(m[c3][c4][1])lu2++;
					if(m[c3][c4][2])lu3++;
				}
			}
			//if(t->tex2==t->tex3)CombineLayers(2,3);
			//if(t->tex1==t->tex2)CombineLayers(1,2);
			//if(t->tex1==t->tex3)CombineLayers(1,3);

			// This checks for 3-way blending...
			//if((lu1)&&(lu2)&&(lu3))
			//{
			//	if((t->tex1!=t->tex2)&&(t->tex1!=t->tex3)&&(t->tex2!=t->tex3))
			//	{
			//		t->tex1=t->tex2=t->tex3=0;
			//	}
			//}

			if((!lu1)||(!lu2)||(!lu3))
			{
				if((!lu1)&&(lu2))savedcount++;
				if((!lu1)&&(!lu2)&&(lu3))savedcount++;
				if((lu1)&&(!lu2)&&(lu3))savedcount++;
				// Sort the three channels into 'most used'
				//for(int cnt=0;cnt<2;cnt++)
				//{
				//	if(l2u>l1u)swapchannels(1,2);
				//	if(l3u>l2u)swapchannels(2,3);
				//}
			}
			for(c3=0;c3<5;c3++)
			{
				for(c4=0;c4<5;c4++)
				{
					t->mMix[c3][c4]=m[c3][c4][0]|(m[c3][c4][1]<<4);
					//(cell+(c3*mMapWidth)+c4)->mMix=m[c3][c4][0]|(m[c3][c4][1]<<4);
				}
			}
		t++;
		}
	}
//	printf("Layers combined: %d\n",LayersCombined);
//	printf("Layers saved: %d\n",savedcount);
#endif

	SINT	o1 = 0, o2 = 0, o3 = 0;
	SINT	s1 = 0, s2 = 0, s3 = 0;

	//JCL - get rid of 16ths and 8ths...
	for (c0 = 0; c0 < mMapHeight-1 ; c0 ++)
	{
		for (c1 = 0; c1 < mMapWidth-1 ; c1 ++)
		{
			CMapCell *cell=&mCells[(c0 << mMapWidthShift) + c1];
			SINT	mix = cell->mMix;
			SINT	mix1 = mix & 0xf;
			SINT	mix2 = (mix & 0xf0) >> 4;

			// debug
			if ((mix1 == 15) || (mix2 == 15) || (mix == 0))
				o1 ++;
			else if ((mix1 == 0) || (mix2 == 0) || (mix1 + mix2 == 15))
				o2 ++;
			else
				o3 ++;

#define CUT1	4
#define CUT2	11

			if (mix1 > CUT2)
				cell->mMix = 15;
			else if (mix2 > CUT2)
				cell->mMix = 0xf0;
			else if ((mix1 < CUT1) || (mix2 < CUT1))
			{
				if ((mix1 < CUT1) && (mix2 < CUT1))
					cell->mMix = 0;
				else if (mix1 < CUT1)
					cell->mMix = (mix1 + mix2) << 4;
				else
					cell->mMix = (mix1 + mix2);
			}

			// debug
			mix = cell->mMix;
			mix1 = mix & 0xf;
			mix2 = (mix & 0xf0) >> 4;

			if ((mix1 == 15) || (mix2 == 15) || (mix == 0))
				s1 ++;
			else if ((mix1 == 0) || (mix2 == 0) || (mix1 + mix2 == 15))
				s2 ++;
			else
				s3 ++;
		}
	}
//	printf("Before compression: 1 pass: %d,   2 pass: %d,  3 pass: %d\n", o1, o2, o3);
//	printf("After  compression: 1 pass: %d,   2 pass: %d,  3 pass: %d\n", s1, s2, s3);


	int tlmix,trmix,blmix,brmix;
	for (c0 = 0; c0 < mMapHeight-1 ; c0 ++)
	{
		for (c1 = 0; c1 < mMapWidth-1 ; c1 ++)
		{
			CMapCell *cell=&mCells[(c0 << mMapWidthShift) + c1];
			int flags=0;
			if(cell->mType)
				flags++;
			else
			{
				tlmix=(cell)->mMix;
				trmix=(cell+1)->mMix;
				blmix=(cell+mMapWidth)->mMix;
				brmix=(cell+mMapWidth+1)->mMix;
				int mix=tlmix|trmix|blmix|brmix;
				if((mix&0x0f)>0x01)flags|=2;
				if((mix&0xf0)>0x01)flags|=4;
				tlmix=15-((tlmix&0x0f)+(tlmix>>4));
            	trmix=15-((trmix&0x0f)+(trmix>>4));
            	blmix=15-((blmix&0x0f)+(blmix>>4));
            	brmix=15-((brmix&0x0f)+(brmix>>4));
				mix=tlmix|trmix|blmix|brmix;
				if((mix&0x0f)>0x01)flags|=8;

//				flags = 2;
			}
			cell->mFlags=flags;
		}
	}




#if 0
	int tlmix,trmix,blmix,brmix;
	for (c0 = 0; c0 < mMapHeight ; c0 +=4)
	{
		for (c1 = 0; c1 < mMapWidth ; c1 +=4)
		{
			t=&Texnums[(c0<<(mMapWidthShift-2))+(c1-2)];

			for (c2 = 0; c2 < 4; c2 ++)
			{
				for (c3 = 0; c3 < 4; c3 ++)
				{
					CMapCell *cell=&mCells[(((c0)+c2) << mMapWidthShift) + (c1)+c3];
					int flags=0;
					if(cell->mType)
						flags++;
					else
					{
						tlmix=t->mMix[c2][c3];
						trmix=t->mMix[c2][c3+1];
						blmix=t->mMix[c2+1][c3];
						brmix=t->mMix[c2+1][c3+1];
						int mix=tlmix|trmix|blmix|brmix;
						if((mix&0x0f)>0x02)flags|=2;
						if((mix&0xf0)>0x02)flags|=4;
						tlmix=15-((tlmix&0x0f)+(tlmix>>4));
            			trmix=15-((trmix&0x0f)+(trmix>>4));
            			blmix=15-((blmix&0x0f)+(blmix>>4));
            			brmix=15-((brmix&0x0f)+(brmix>>4));
						mix=tlmix|trmix|blmix|brmix;
						if((mix&0x0f)>0x02)flags|=8;
					}
					cell->mFlags=flags;
				}
			}
		}
	}
#endif
	SINT wht=GAME.GetLevelData()->mGVars[WATER_HEIGHT].GetVal();
	int h1,h2,h3,h4,mask,all;
	for (c0 = 0; c0 < mMapHeight-1 ; c0 ++)
	{
		for (c1 = 0; c1 < mMapWidth-1 ; c1 ++)
		{

			//cell->ClearReverseSplitFlag();

#if 1
			CMapCell *cell=&mCells[(c0 << mMapWidthShift) + c1];
			h3=(cell)->GetUnderlyingHeight().GetVal();
			h4=(cell+1)->GetUnderlyingHeight().GetVal();
			h1=(cell+mMapWidth)->GetUnderlyingHeight().GetVal();
			h2=(cell+mMapWidth+1)->GetUnderlyingHeight().GetVal();
			if(ABS(h1-h4)<ABS(h2-h3))cell->SetReverseSplitFlag();
			//else cell->ClearReverseSplitFlag();
			// Now do the water split as well.....
			if(wht!=0)
			{
				if(h1>wht)h1=wht;
				if(h2>wht)h2=wht;
				if(h3>wht)h3=wht;
				if(h4>wht)h4=wht;
				if(ABS(h1-h4)<ABS(h2-h3))cell->SetReverseSplitFlagWater();
				//else cell->ClearReverseSplitFlagWater();
			}
			//else cell->ClearReverseSplitFlagWater();






			h3=(cell)->mMix;
			h4=(cell+1)->mMix;
			h1=(cell+mMapWidth)->mMix;
			h2=(cell+mMapWidth+1)->mMix;
			all=h1|h2|h3|h4;
			mask=0x0f;
			if(all&mask)
			{
					if((h1&mask)&&(h4&mask)&&(!(h3&mask)))
						cell->SetReverseSplitFlag();
					else if((h1&mask)&&(h4&mask)&&(!(h2&mask)))
						cell->SetReverseSplitFlag();
			}
			mask=0xf0;
			if(all&mask)
			{
					if((h1&mask)&&(h4&mask)&&(!(h3&mask)))
						cell->SetReverseSplitFlag();
					else if((h1&mask)&&(h4&mask)&&(!(h2&mask)))
						cell->SetReverseSplitFlag();
			}
			h1=15-((h1&0x0f)+(h1>>4));
			h2=15-((h2&0x0f)+(h2>>4));
			h3=15-((h3&0x0f)+(h3>>4));
			h4=15-((h4&0x0f)+(h4>>4));
			mask=0x0f;
			if(all&mask)
			{
					if((h1&mask)&&(h4&mask)&&(!(h3&mask)))
						cell->SetReverseSplitFlag();
					else if((h1&mask)&&(h4&mask)&&(!(h2&mask)))
						cell->SetReverseSplitFlag();
			}



#endif
		}
	}
	
// SDS - Huge AMOUNT OF CODE HERE  to speed up rendering	
	
	
	
	
	
	

}


//******************************************************************************************
#define	BEAM_LONG_VERTS			30
#define BEAM_ROTATE_SPEED		0.1f
#define	BEAM_RADIUS				0.4f
#define BEAM_LENGTH				20.f
#define	BEAM_GROUND_PENETRATION	3.f

void CDCMap::DrawPickupBeam(CPlayer *pl)
{
	PROFILE_FN(RMDrawPickupBeam);

	FVector pos;

	//! Hmmmm
	GVector ac = pl->mAnalogCursor;
	ClipPosition(ac);

	switch (CONTROLS.GetControllerMode())
	{
	case CM_KEY_ALT1:
	case CM_JOY_ALT1:
	case CM_JOY_REL:
	case CM_JOY_GOLF:
		pos = FVector(GToF(ac.X), GToF(ac.Y) , GToF(GetCell(CCellRef(ac))->GetUnderlyingHeight()) + BEAM_GROUND_PENETRATION);
		break;
	case CM_JOY_SHUTTLE:
	case CM_JOY_RACER:
	case CM_JOY_DIRECT:
		return; // no beam in shuttle mode.

	default:
		pos = FVector(float(pl->mPickup.X), float(pl->mPickup.Y), GToF(GetCell(pl->mPickup)->GetUnderlyingHeight()) + BEAM_GROUND_PENETRATION);
		break;
	};


	if (pl->mPickupBeamState != NO_BEAM)
	{
		// draw central cylinder.

		ASSERT(pl->mPickup.X != -1);
	
		FMatrix ori;
		ori.MakeRotationYawF(float(ENGINE.GetRenderGameTurn() - pl->mPickupBeamStartTime) * BEAM_ROTATE_SPEED);
	
		float	beam_radius = BEAM_RADIUS * GToF(pl->mPickupBeamSize) * 0.1f;
		SINT	intensity = pl->mPickupBeamSize.Whole() * (0xff / 20);
		DWORD	col = 0xff000000 + (intensity << 16) + (intensity << 8) +intensity;
	
		FVector	verts[BEAM_LONG_VERTS * 2];
		float	vertu[BEAM_LONG_VERTS * 2];
		float   vertv[BEAM_LONG_VERTS * 2];
	
	    STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_BEAM1);
	
		SINT	c0, c1, cv = 0;
	
		for (c0 = 0; c0 < 2; c0 ++)
		{
			for (c1 = 0; c1 < BEAM_LONG_VERTS; c1 ++)
			{
				verts[c1 + c0 * BEAM_LONG_VERTS].X = float(sin((c1 / float(BEAM_LONG_VERTS)) * (2 * PI))) * beam_radius;
				verts[c1 + c0 * BEAM_LONG_VERTS].Y = float(cos((c1 / float(BEAM_LONG_VERTS)) * (2 * PI))) * beam_radius;
				verts[c1 + c0 * BEAM_LONG_VERTS].Z = (float(c0 - 1)) * BEAM_LENGTH;
	
				vertu[c1 + c0 * BEAM_LONG_VERTS] = tr->mX + 0.002f + ((tr->mW - 0.002f) * (float(c1) / float(BEAM_LONG_VERTS))); // arse... wrapping problems
				vertv[c1 + c0 * BEAM_LONG_VERTS] = tr->mY + 0.002f + ((tr->mH - 0.002f) * (float(c0)));
	
				cv ++;
			}
		}
		SINT	TN = tr->mTPage + 1;
		
		kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
	    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
	 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
	 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
		
		ENGINE.SwapYZ((FVector *)verts, BEAM_LONG_VERTS * 2);	
		PersTransWrapper(BEAM_LONG_VERTS * 2,(tvector *)verts, (tvector *)verts, pos , ori);
	
	
		// and draw the polys
		SINT	ind[4];
	
		for (c1 = 0; c1 < BEAM_LONG_VERTS; c1++)
		{
			if (c1 != BEAM_LONG_VERTS - 1)
			{
				ind[0] = ((0    ) * BEAM_LONG_VERTS) + c1;
				ind[1] = ((0    ) * BEAM_LONG_VERTS) + c1 + 1;
				ind[2] = ((0 + 1) * BEAM_LONG_VERTS) + c1;
				ind[3] = ((0 + 1) * BEAM_LONG_VERTS) + c1 + 1;
			}
			else
			{
				ind[0] = ((0    ) * BEAM_LONG_VERTS) + c1;
				ind[1] = ((0    ) * BEAM_LONG_VERTS) + c1 - BEAM_LONG_VERTS + 1;
				ind[2] = ((0 + 1) * BEAM_LONG_VERTS) + c1;
				ind[3] = ((0 + 1) * BEAM_LONG_VERTS) + c1 - BEAM_LONG_VERTS + 1;
			}
	
			InitCZTs(TN);
	
	       	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[0].fX                   =  verts[ind[0]].X;
	       	TextureS[0].fY                   =  verts[ind[0]].Y;
	       	TextureS[0].u.fZ                 = -verts[ind[0]].Z;
	       	TextureS[0].uBaseRGB.dwPacked    = col;
	       	TextureS[0].fU                   = vertu[ind[0]];
	       	TextureS[0].fV                   = vertv[ind[0]];
		
	       	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[1].fX                   =  verts[ind[1]].X;
	       	TextureS[1].fY                   =  verts[ind[1]].Y;
	       	TextureS[1].u.fZ                 = -verts[ind[1]].Z;
	       	TextureS[1].uBaseRGB.dwPacked    = col;
	       	TextureS[1].fU                   = vertu[ind[1]];
	       	TextureS[1].fV                   = vertv[ind[1]];
		
	       	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[2].fX                   =  verts[ind[2]].X;
	       	TextureS[2].fY                   =  verts[ind[2]].Y;
	       	TextureS[2].u.fZ                 = -verts[ind[2]].Z;
	       	TextureS[2].uBaseRGB.dwPacked    = col;
	       	TextureS[2].fU                   = vertu[ind[2]];
	       	TextureS[2].fV                   = vertv[ind[2]];
		
	       	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
	       	TextureS[3].fX                   =  verts[ind[3]].X;
	       	TextureS[3].fY                   =  verts[ind[3]].Y;
	       	TextureS[3].u.fZ                 = -verts[ind[3]].Z;
	       	TextureS[3].uBaseRGB.dwPacked    = col;
	       	TextureS[3].fU                   = vertu[ind[3]];
	       	TextureS[3].fV                   = vertv[ind[3]];
	
			ExitCZTs(4);
		}
		
		// Draw the pulsing circle things..
	
	#define	RING_SEPARATION	5
	#define	RING_COUNT_MAX	80
	
	#define	RING_BRIGHTNESS_PEAK 10
	#define	RING_RADIUS	(BEAM_RADIUS * 2.f)
	#define	RING_START_OFFSET (BEAM_GROUND_PENETRATION + 2.2f)
	
		SINT	count = (ENGINE.GetRenderGameTurn() - pl->mPickupBeamStartTime) % RING_SEPARATION;
	
		while (count < RING_COUNT_MAX)
		{
			// use same geometry as above.
			float	ring_height = -(float(count * count * count * count) * (BEAM_LENGTH / float(RING_COUNT_MAX * RING_COUNT_MAX * RING_COUNT_MAX * RING_COUNT_MAX)) + RING_START_OFFSET);
			float	ring_size = (float(RING_COUNT_MAX * RING_COUNT_MAX) - float((RING_COUNT_MAX - count) * (RING_COUNT_MAX - count))) * (RING_RADIUS / float(RING_COUNT_MAX * RING_COUNT_MAX)) * GToF(pl->mPickupBeamSize) * 0.1f;
	
			SINT	intensity2;
	
			if (count < RING_BRIGHTNESS_PEAK)
				intensity2 = count * (0xff / RING_BRIGHTNESS_PEAK);
			else
				intensity2 = (RING_COUNT_MAX - count) * (0xff / (RING_COUNT_MAX - RING_BRIGHTNESS_PEAK));
	
			intensity2 = (intensity * intensity2) / 0xff;
	
			col = 0xff000000 + (intensity2 << 16) + (intensity2 << 8) +intensity2;
	
			verts[0] = FVector(-ring_size, -ring_size, ring_height);
			verts[1] = FVector( ring_size, -ring_size, ring_height);
			verts[2] = FVector(-ring_size,  ring_size, ring_height);
			verts[3] = FVector( ring_size,  ring_size, ring_height);
			
	        tr = ENGINE.GetNumberedTexture(TN_BEAM2);
	        
	        TN = tr->mTPage + 1;
		
			kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
		    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
		 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
		 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
			
			ENGINE.SwapYZ((FVector *)verts, 4);	
			PersTransWrapper(4,(tvector *)verts, (tvector *)verts, pos , ori);
			
			InitCZTs(TN);
	
			float	u0 = tr->mX + 0.002f;
			float	u1 = tr->mX + tr->mW - 0.002f;
			float	v0 = tr->mY + 0.002f;
			float	v1 = tr->mY + tr->mH - 0.002f;
			
	       	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[0].fX                   = verts[0].X;
	       	TextureS[0].fY                   = verts[0].Y;
	       	TextureS[0].u.fZ                 = -verts[0].Z;
	       	TextureS[0].uBaseRGB.dwPacked    = col;
	       	TextureS[0].fU                   = u0;
	       	TextureS[0].fV                   = v1;
		
	       	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[1].fX                   = verts[1].X;
	       	TextureS[1].fY                   = verts[1].Y;
	       	TextureS[1].u.fZ                 = -verts[1].Z;
	       	TextureS[1].uBaseRGB.dwPacked    = col;
	       	TextureS[1].fU                   = u0;
	       	TextureS[1].fV                   = v0;
		
	       	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
	       	TextureS[2].fX                   = verts[2].X;
	       	TextureS[2].fY                   = verts[2].Y;
	       	TextureS[2].u.fZ                 = -verts[2].Z;
	       	TextureS[2].uBaseRGB.dwPacked    = col;
	       	TextureS[2].fU                   = u1;
	       	TextureS[2].fV                   = v1;
		
	       	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
	       	TextureS[3].fX                   = verts[3].X;
	       	TextureS[3].fY                   = verts[3].Y;
	       	TextureS[3].u.fZ                 = -verts[3].Z;
	       	TextureS[3].uBaseRGB.dwPacked    = col;
	       	TextureS[3].fU                   = u1;
	       	TextureS[3].fV                   = v0;
		
			ExitCZTs(4);
			
			count += RING_SEPARATION;
		}
	}

#define	LASER_RADIUS				0.10f
#define LASER_LENGTH				15.f
#define	LASER_GROUND_PENETRATION	3.f

	if (pl->mCursor.X == -1)
		return; // not on map.
		
	if ((GAME.GetIVar(IV_LEVEL_TYPE) != LT_GOLF) && (GAME.GetIVar(IV_LEVEL_TYPE) != LT_NORMAL))
		return; // don't draw

	// setup transformations
	float	laser_radius = LASER_RADIUS;
	SINT	intensity = 0x7f;
	DWORD	col = 0xff000000 + (intensity << 16);

//	lots of beams here!
	float	v1 = float((sin(float(ENGINE.GetRenderGameTurn()) / 80.f))) * 0.2f + 0.25f;
	float	v2 = float((cos(float(ENGINE.GetRenderGameTurn()) / 80.f))) * 0.2f + 0.25f;

	SINT	c0, c1, c2, cv;
	cv = 0;

	FVector	verts[8];
	float	vertu[8];
	float	vertv[8];

	STextureRefOther    *tr = ENGINE.GetNumberedTexture(TN_LASER);

	for (c2 = 0; c2 < 2; c2 ++)
	{
		for (c0 = 0; c0 < 2; c0 ++)
		{
			for (c1 = 0; c1 < 2; c1 ++)
			{
				verts[cv].X = (laser_radius * float(c1)) - (laser_radius / 2.f);
				verts[cv].Y = 0;
				verts[cv].Z = (float(c0 - 1)) * LASER_LENGTH;

				vertu[cv] = tr->mX + 0.002f + ((tr->mW - 0.002f) * (float(c0)));
				vertv[cv] = tr->mY + 0.002f + float(c1) * 0.0015f + ((c2 == 0) ? v1 : v2); // prevent leakage

				cv ++;
			}
		}
	}
	
  
    SINT TN = tr->mTPage + 1;

	kmChangeStripListType			( &TextureHead[TN], KM_TRANS_POLYGON );    
    kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_ONE,KM_ONE);
 	kmChangeStripTextureShadingMode ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MODULATE);	
 	kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_TRUE );
	
	ENGINE.SwapYZ((FVector *)verts, 8);	
	PersTransWrapper(8,(tvector *)verts, (tvector *)verts, pos , ID_FMATRIX);
	
	InitCZTs(TN);

   	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[0].fX                   = verts[0].X;
   	TextureS[0].fY                   = verts[0].Y;
   	TextureS[0].u.fZ                 = -verts[0].Z;
   	TextureS[0].uBaseRGB.dwPacked    = col;
   	TextureS[0].fU                   = vertu[0];
   	TextureS[0].fV                   = vertv[0];

   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].fX                   = verts[1].X;
   	TextureS[1].fY                   = verts[1].Y;
   	TextureS[1].u.fZ                 = -verts[1].Z;
   	TextureS[1].uBaseRGB.dwPacked    = col;
   	TextureS[1].fU                   = vertu[1];
   	TextureS[1].fV                   = vertv[1];

   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].fX                   = verts[2].X;
   	TextureS[2].fY                   = verts[2].Y;
   	TextureS[2].u.fZ                 = -verts[2].Z;
   	TextureS[2].uBaseRGB.dwPacked    = col;
   	TextureS[2].fU                   = vertu[2];
   	TextureS[2].fV                   = vertv[2];

   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	TextureS[3].fX                   = verts[3].X;
   	TextureS[3].fY                   = verts[3].Y;
   	TextureS[3].u.fZ                 = -verts[3].Z;
   	TextureS[3].uBaseRGB.dwPacked    = col;
   	TextureS[3].fU                   = vertu[3];
   	TextureS[3].fV                   = vertv[3];

	ExitCZTs(4);
	
	InitCZTs(TN);

   	TextureS[0].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[0].fX                   = verts[4].X;
   	TextureS[0].fY                   = verts[4].Y;
   	TextureS[0].u.fZ                 = -verts[4].Z;
   	TextureS[0].uBaseRGB.dwPacked    = col;
   	TextureS[0].fU                   = vertu[4];
   	TextureS[0].fV                   = vertv[4];

   	TextureS[1].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[1].fX                   = verts[5].X;
   	TextureS[1].fY                   = verts[5].Y;
   	TextureS[1].u.fZ                 = -verts[5].Z;
   	TextureS[1].uBaseRGB.dwPacked    = col;
   	TextureS[1].fU                   = vertu[5];
   	TextureS[1].fV                   = vertv[5];

   	TextureS[2].ParamControlWord     = KM_VERTEXPARAM_NORMAL;
   	TextureS[2].fX                   = verts[6].X;
   	TextureS[2].fY                   = verts[6].Y;
   	TextureS[2].u.fZ                 = -verts[6].Z;
   	TextureS[2].uBaseRGB.dwPacked    = col;
   	TextureS[2].fU                   = vertu[6];
   	TextureS[2].fV                   = vertv[6];

   	TextureS[3].ParamControlWord     = KM_VERTEXPARAM_ENDOFSTRIP;
   	TextureS[3].fX                   = verts[7].X;
   	TextureS[3].fY                   = verts[7].Y;
   	TextureS[3].u.fZ                 = -verts[7].Z;
   	TextureS[3].uBaseRGB.dwPacked    = col;
   	TextureS[3].fU                   = vertu[7];
   	TextureS[3].fV                   = vertv[7];

	ExitCZTs(4);
	
}

//*****************************************************************************
//#pragma optimization_level 0
//#pragma dont_inline on

#define FSIGN(a) (((a) >= 0) ? (1.f) : (-1.f))

void	CDCMap::CDSP(STextureRefOther *tr, FVector p1, FVector p2, FVector p3, SVert v1, SVert v2, SVert v3)
{

	SVert	b1[20], b2[20];
	SVert	*in = b1, *out = b2;

	// copy up
	b1[0] = v1;
	b1[1] = v2;
	b1[2] = v3;

	// first push all points onto the plane
	SINT c0;

	FVector normal	= (p2 - p1) ^ (p3 - p1);
	FVector h		= normal ^ FVector(0, 0, 1.f); // a "Horizonal" vector
	if (h.MagnitudeSq() < 0.0001)
		h = FVector(1.f, 0, 0); // oops - nearly horizontal anyway...
	FVector s		= h ^ normal;  // this vector points down the slope
	s = s / s.MagnitudeXY();

	for (c0 = 0; c0 < 3; c0 ++)
	{
		FVector s1 = b1[c0].mPos - p1;
		b1[c0].mPos.Z = p1.Z + s.Z * (s.X * s1.X + s.Y * s1.Y) - 0.02f;
	}

	SVert	*curr_in, *curr_out, *end = in + 3, *prev;


	for (c0 = 0; c0 < 3; c0++)
	{
		FVector	d;
		FVector pa;

		switch (c0)
		{
		case 0:
			d = p2 - p1;
			pa = p1;
			curr_in = in;
			curr_out = out;
			break;
		case 1:
			d = p3 - p2;
			pa = p2;
			curr_in = out;
			curr_out = in;
			break;
		case 2:
			d = p1 - p3;
			pa = p3;
			curr_in = in;
			curr_out = out;
			break;
		};
		prev = end - 1;

		while(curr_in < end)
		{
			FVector d1 = curr_in->mPos - pa;
			FVector d2 = prev   ->mPos - pa;
			float a = d.X * d1.Y - d.Y * d1.X;
			float b = d.X * d2.Y - d.Y * d2.X;

			if (FSIGN(a) != FSIGN(b))
			{
				// different sides - need to do something!
				float	r = FABS(a) + FABS(b);
				float	ra = FABS(a) / r;
				float	rb = FABS(b) / r;
				curr_out->mPos		= prev->mPos + ((curr_in->mPos - prev->mPos) * rb);
				curr_out->mU	= (prev->mU * ra) + (curr_in->mU * rb);
				curr_out->mV	= (prev->mV * ra) + (curr_in->mV * rb);

				curr_out++;
			}

			if (FSIGN(a) == 1.f)
				*curr_out ++ = *curr_in;

			prev = curr_in;
			curr_in ++;
		}

		end = curr_out;
	}

	if (end < out + 3)
		// all clipped - go away
		return;


/*	SINT i[20];

	ASSERT(end < out + 20);

	SINT n = end - out;
	for (c0 = 0; c0 < n; c0++)
		i[c0] = tex->AddVertex(&out[c0]);

	for (c0 = 0; c0 < n - 2; c0++)
		tex->AddTri(i[0], i[c0 + 1], i[c0 + 2]);
*/

//	return;
	
	SINT n = end - out;

	// Transform
	FVector	pverts[20];
	for (c0 = 0; c0 < n; c0 ++)
	{
		pverts[c0].X = out[c0].mPos.X;
		pverts[c0].Z = out[c0].mPos.Y;
		pverts[c0].Y = out[c0].mPos.Z;
	}

	PersTransWrapper(n,(tvector *)&pverts,(tvector *)&pverts, ZERO_FVECTOR, ID_FMATRIX);
	
	// render
	SINT	TN = tr->mTPage + 1;
	
	for (c0 = 0; c0 < n - 2; c0 ++)
	{
		// hmmmm
		if ((pverts[0     ] == pverts[c0 + 1]) ||
		    (pverts[0     ] == pverts[c0 + 2]) ||
		    (pverts[c0 + 1] == pverts[c0 + 2]))
		    continue;
	
		GTriCZT(TN, 
		        pverts[0     ].X, pverts[0     ].Y,
		        pverts[c0 + 1].X, pverts[c0 + 1].Y,
		        pverts[c0 + 2].X, pverts[c0 + 2].Y,
		        pverts[0].Z, pverts[c0 + 1].Z, pverts[c0 + 2].Z,
		        out[0     ].mU, out[0     ].mV,
		        out[c0 + 1].mU, out[c0 + 1].mV,
		        out[c0 + 2].mU, out[c0 + 2].mV);
	}
}

//*****************************************************************************
void	CDCMap::ClipAndDrawShadowPoly(STextureRefOther *tr, SVert v1, SVert v2, SVert v3)
{
	// get footprint

	float	minx = 999999.f, maxx = -9999999.f;
	float	miny = 999999.f, maxy = -9999999.f;

	if (v1.mPos.X < minx) minx = v1.mPos.X;
	if (v2.mPos.X < minx) minx = v2.mPos.X;
	if (v3.mPos.X < minx) minx = v3.mPos.X;
	if (v1.mPos.Y < miny) miny = v1.mPos.Y;
	if (v2.mPos.Y < miny) miny = v2.mPos.Y;
	if (v3.mPos.Y < miny) miny = v3.mPos.Y;

	if (v1.mPos.X > maxx) maxx = v1.mPos.X;
	if (v2.mPos.X > maxx) maxx = v2.mPos.X;
	if (v3.mPos.X > maxx) maxx = v3.mPos.X;
	if (v1.mPos.Y > maxy) maxy = v1.mPos.Y;
	if (v2.mPos.Y > maxy) maxy = v2.mPos.Y;
	if (v3.mPos.Y > maxy) maxy = v3.mPos.Y;

	SINT	startx	= SINT(minx);
	SINT	starty	= SINT(miny);
	SINT	endx	= SINT(maxx);
	SINT	endy	= SINT(maxy);

	if (startx < 0) startx = 0;
	if (starty < 0) starty = 0;
	if (endx > MAP_WIDTH  - 2) endx = MAP_WIDTH  - 2;
	if (endy > MAP_HEIGHT - 2) endy = MAP_HEIGHT - 2;

	SINT c0, c1;
	for (c0 = startx; c0 <= endx; c0++)
	{
		for (c1 = starty; c1 <= endy; c1++)
		{
			FVector p[5];

			p[0] = FVector(float(c0    ), float(c1    ), GToF(MAP.GetCell(CCellRef(c0    , c1    ))->GetHeight()));
			p[1] = FVector(float(c0 + 1), float(c1    ), GToF(MAP.GetCell(CCellRef(c0 + 1, c1    ))->GetHeight()));
			p[2] = FVector(float(c0 + 1), float(c1 + 1), GToF(MAP.GetCell(CCellRef(c0 + 1, c1 + 1))->GetHeight()));
			p[3] = FVector(float(c0    ), float(c1 + 1), GToF(MAP.GetCell(CCellRef(c0    , c1 + 1))->GetHeight()));

			p[4] = (p[0] + p[1] + p[2] + p[3]) / 4.0f;

/*			CDSP(tr, p[4], p[0], p[1], v1, v2, v3);
			CDSP(tr, p[4], p[1], p[2], v1, v2, v3);
			CDSP(tr, p[4], p[2], p[3], v1, v2, v3);
			CDSP(tr, p[4], p[3], p[0], v1, v2, v3);
*/

			// map is split differently
			if (c0 & 1)
			{
				CDSP(tr, p[0], p[1], p[2], v1, v2, v3);
				CDSP(tr, p[0], p[2], p[3], v1, v2, v3);
			}
			else
			{
				CDSP(tr, p[0], p[1], p[3], v1, v2, v3);
				CDSP(tr, p[3], p[1], p[2], v1, v2, v3);
			}
		}
	}
}

//*****************************************************************************
void	CDCMap::CDSQ(STextureRefOther *tr, const FVector &p1, const FVector &p2, const FVector &p3,
					 float sx, float sy, float ex, float ey)
{
	#define Z_FUDGE2 (0.003f)
	// take #2
	// map uvs.
	// !! assumes single texture on page
	
	float	u0, u1, u2, v0, v1, v2;
	
	u0 = (p1.X - sx) / (ex - sx);
	u1 = (p2.X - sx) / (ex - sx);
	u2 = (p3.X - sx) / (ex - sx);

	v0 = (p1.Y - sy) / (ey - sy);
	v1 = (p2.Y - sy) / (ey - sy);
	v2 = (p3.Y - sy) / (ey - sy);	

	FVector	pverts[3];
	pverts[0].X = p1.X;
	pverts[0].Z = p1.Y;
	pverts[0].Y = p1.Z;
	pverts[1].X = p2.X;
	pverts[1].Z = p2.Y;
	pverts[1].Y = p2.Z;
	pverts[2].X = p3.X;
	pverts[2].Z = p3.Y;
	pverts[2].Y = p3.Z;

	PersTransWrapper(3,(tvector *)&pverts,(tvector *)&pverts, ZERO_FVECTOR, ID_FMATRIX);
	
	// render
	SINT	TN = tr->mTPage + 1;

		GTriCZT(TN, 
		        pverts[0].X, pverts[0].Y,
		        pverts[1].X, pverts[1].Y,
		        pverts[2].X, pverts[2].Y,
		        pverts[0].Z - Z_FUDGE2, pverts[1].Z - Z_FUDGE2, pverts[2].Z - Z_FUDGE2,
		        u0, v0, u1, v1, u2, v2);
}

//*****************************************************************************
void	CDCMap::ClipAndDrawShadowQuad(STextureRefOther *tr, float sx, float sy, float ex, float ey)
{
	SINT	startx	= SINT(sx);
	SINT	starty	= SINT(sy);
	SINT	endx	= SINT(ex);
	SINT	endy	= SINT(ey);

	if (startx < 0) startx = 0;
	if (starty < 0) starty = 0;
	if (endx > MAP_WIDTH  - 2) endx = MAP_WIDTH  - 2;
	if (endy > MAP_HEIGHT - 2) endy = MAP_HEIGHT - 2;

	SINT c0, c1;
	for (c0 = startx; c0 <= endx; c0++)
	{
		for (c1 = starty; c1 <= endy; c1++)
		{
			FVector p[4];

			p[0] = FVector(float(c0    ), float(c1    ), GToF(MAP.GetCell(CCellRef(c0    , c1    ))->GetHeight()));
			p[1] = FVector(float(c0 + 1), float(c1    ), GToF(MAP.GetCell(CCellRef(c0 + 1, c1    ))->GetHeight()));
			p[2] = FVector(float(c0 + 1), float(c1 + 1), GToF(MAP.GetCell(CCellRef(c0 + 1, c1 + 1))->GetHeight()));
			p[3] = FVector(float(c0    ), float(c1 + 1), GToF(MAP.GetCell(CCellRef(c0    , c1 + 1))->GetHeight()));

			// map is split differently
			if (c0 & 1)
			{
				CDSQ(tr, p[0], p[1], p[2], sx, sy, ex, ey);
				CDSQ(tr, p[0], p[2], p[3], sx, sy, ex, ey);
			}
			else
			{
				CDSQ(tr, p[0], p[1], p[3], sx, sy, ex, ey);
				CDSQ(tr, p[3], p[1], p[2], sx, sy, ex, ey);
			}
		}
	}
}

//*****************************************************************************
void	CDCMap::DrawShadow(IRenderableThing *t, STextureRefOther *tr)
{
	if ((t) && (tr))
	{
		GVector	pos = t->GetRenderPos();
		if (!(COORDS_INVALID(CCellRef(pos))))
		{
			SVert	verts[4];			

			float dist = - GToF(pos.Z - (MAP.GetMapHeight(pos)));
			if (dist >= -0.5f)
			{
				GINT	gr = t->GetRenderRadius();
				float	r = GToF(gr);
				
				float shadow_scale = 8.f / (dist + 8.f);
				r *= shadow_scale;
				
				SINT	TN = tr->mTPage + 1;
				kmChangeStripListType           ( &TextureHead[TN], KM_TRANS_POLYGON );	
				kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
				kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE ); 	
				kmChangeStripMipmapAdjust		( &TextureHead[TN], KM_IMAGE_PARAM1, KM_MIPMAP_D_ADJUST_0_25 ); 	

				MAP.ClipAndDrawShadowQuad(tr, GToF(pos.X) - r, GToF(pos.Y) - r, GToF(pos.X) + r, GToF(pos.Y) + r);
			}
		}
	}

/*
	if ((t) && (tr))
	{
		GVector	pos = t->GetRenderPos();
		if (!(COORDS_INVALID(CCellRef(pos))))
		{
			SVert	verts[4];			

			float dist = - GToF(pos.Z - (MAP.GetMapHeight(pos)));
			if (dist >= -0.5f)
			{
				GINT	gr = t->GetRenderRadius();
				float	r = GToF(gr);
				
				float shadow_scale = 8.f / (dist + 8.f);
				FVector x = FVector(r * shadow_scale, 0, 0);
				FVector y = FVector(0, r * shadow_scale, 0);

				if (t->GetRenderRotateShadow())
				{
					FMatrix	m;
					float	yaw = GToF(t->GetRenderYaw());
					m.MakeRotationYawF(yaw);
					x = m * x;
					y = m * y;
				}

				float	us = tr->mX + 0.002f;
				float	ue = tr->mX + tr->mW - 0.002f;
				float	vs = tr->mY + 0.002f;
				float	ve = tr->mY + tr->mH - 0.002f;

				for (int c1 = 0;c1 < 4; c1 ++)
				{
					verts[c1].mPos	= GVectorToFVector(pos) + ((c1 & 1) ? x : -x) + ((c1 & 2) ? y : -y);
					verts[c1].mU	= (c1 & 1) ? ue : us;
					verts[c1].mV	= (c1 & 2) ? ve : vs;
				}

				SINT	TN = tr->mTPage + 1;
				kmChangeStripListType           ( &TextureHead[TN], KM_TRANS_POLYGON );	
				kmChangeStripBlendingMode       ( &TextureHead[TN], KM_IMAGE_PARAM1, KM_SRCALPHA, KM_INVSRCALPHA);
				kmChangeStripUseAlpha			( &TextureHead[TN], KM_IMAGE_PARAM1, KM_FALSE ); 	

				MAP.ClipAndDrawShadowPoly(tr, verts[0], verts[2], verts[3]);
				MAP.ClipAndDrawShadowPoly(tr, verts[0], verts[3], verts[1]);
			}
		}
	}
	*/
}

//#pragma optimization_level 4
//#pragma dont_inline off

//******************************************************************************************

BOOL    CDCMap::Init()
{                      // init top level stuff
	if (!(InitCommon()))
		return FALSE;

	//!PSX   hack
    //PSXLoadTimToVRAM("PSXData\\sand.tim", (ULONG *)tempdata);

	return TRUE;
}

//******************************************************************************************
BOOL	CDCMap::Resize(SINT xs, SINT ys)
{
	//!PSX this stuff should be platform specific.
	//!PSX - DEFINITLY!

	// default values.
	mMapWidthShift = xs;
	mMapHeightShift = ys;
	mMapWidth  = 1 << mMapWidthShift;
	mMapHeight = 1 << mMapHeightShift;


	// initialise the heightfield
	mCells = POOL->mMapCells;

	// Allocate memory for MapWhos
	SINT	c0;
	for (c0 = MAP_LAYER_MIN; c0 <= MAP_LAYER_MAX; c0++)
	{
		mMapWhos[c0 - MAP_LAYER_MIN] = (CThing **)POOL->GetMapWhoLayerMemory(c0);
	}

	// clear out data for safety
	jcl_memset(mCells, 0, sizeof(CMapCell) * MAX_MAPCELLS);
	jcl_memset(&Texnums, 0, sizeof(Texnums));

	MAP.InitPlatform();

	Reset(); //! Overkill??

//	printf("Resized map OK\n");

	return TRUE;

}







///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// ** Set flags as water passes through in a pursuit level

void	CDCMap::SetRowWCCheckAndMaxHeight(SINT r, GINT h)
{
	SINT	e = MAP_WIDTH >> 2;
	S4x4	*s4 = Texnums + (r * e);

	SINT	c0;

	SWORD	v = h.GetVal()>>(GINT_SHIFT-PSXENGINE_SCALE);

	for (c0 = 0; c0 < e; c0 ++)
	{
		s4->mFlags = (s4->mFlags & ~WC_MASK)|WC_CHECK;
		s4->mMaxHeight = v;
		s4 ++;
	}
}

void	CDCMap::SetRowWCAlwaysAndMinHeight(SINT r, GINT h)
{
	SINT	e = MAP_WIDTH >> 2;
	S4x4	*s4 = Texnums + (r * e);

	SINT	c0;

	SWORD	v = h.GetVal()>>(GINT_SHIFT-PSXENGINE_SCALE);

	for (c0 = 0; c0 < e; c0 ++)
	{
		s4->mFlags = (s4->mFlags & ~WC_MASK)|WC_ALWAYS;
		s4->mMinHeight = v;
		s4 ++;
	}
}



//***********************************************************************
void	CDCMap::ClearOtherFlags()
{
	S4x4	*t = Texnums;

	SINT	c0, c1;
	for (c0 = 0; c0 < mMapHeight / 4; c0 ++)
	{
		for (c1 = 0; c1 < mMapWidth / 4; c1 ++)
		{
			t->mFlags &= ~F_OTHER_MASK;
			t++;
		}
	}
}

//***********************************************************************
void	CDCMap::SetOtherFlags(const CCellRef &c, SINT flags)
{
	SINT	p = (c.X / 4) + ((c.Y / 4) * (mMapWidth / 4));

	Texnums[p].mFlags |= flags;
}

#endif                                  // TARGET_PSX

