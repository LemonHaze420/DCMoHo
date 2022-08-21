#include	"Common.h"

#if TARGET == PSX

#include	"Globals.h"
#include	"Profile.h"
#include    <libmath.h>
#include    <mwinline.h>
#include <gtemac.h>
#include <gtereg_s.h>
#include <inline_s.h>
//#ifdef _MAP_OPTIMISED
//#pragma optimization_level 4
#pragma peephole on
//#endif

#include "psx3dengine.h"
#include	"PSXParticleSystem.h"
void asm MakeColoursCommon(int count);

// Uncomment the next line to remove the SHADOWS, BEAM and LASER, they slooow compiling down!!!
//#define COMPILE_FOR_SPEED

//#define WATERZ (0)
//#define LANDZ	(0)

#define WATERZ (-10)
#define LANDZ	(+10)
#define SHADOWZ (-4)
//					  yoffs--xoffs
//******************************************************************************************
struct  SVertexCache
{
    SWORD   sx;
	SWORD	sy;
	SWORD   sz;
	SWORD	dpq;
	ULONG	mixcols[4];                 // mixcols[0] is FULL colour, then 3 'mixers' for the overlay textures
	SLONG	wcol;
	UBYTE	flags;
	UBYTE	type;
	UBYTE	mix;
	SBYTE	tlind;
};

#define DOTRANSOVERLAYS
#define NUMVERTSCACHEDPERLINE (25)




#define SVertexCachesx	0
#define SVertexCachesy	2
#define SVertexCachesz	4
#define SVertexCachedpq	6
#define SVertexCachemixcols 8
#define SVertexCachewcol 24
#define SVertexCacheflags 28
#define SVertexCachetype 29
#define SVertexCachemix 30
#define SVertexCachetlind 31



#define SIZEOFSVERTEXCACHE 32
#define SIZEOFSVERTEXCACHEP1 32
#define SIZEOFSVERTEXCACHEM1 -32
#define SIZEOFSVERTEXCACHEP2 64
#define SIZEOFSVERTEXCACHEM2 -64
#define SIZEOFCMAPCELL 16
#define SIZEOFCMAPCELLP1 16
#define SIZEOFCMAPCELLM1 -16
#define SIZEOFCMAPCELLP2 32
#define SIZEOFCMAPCELLM2 -32

#define CMapCellmHeight 0
#define CMapCellmOffset 2
#define CMapCellmDisplacement 4
#define CMapCellmOffsetVelocity 6
#define CMapCellmColour         8
#define CMapCellmFlags          12
#define CMapCellmType           13
#define CMapCellmMix            14
#define CMapCellmRamp           15



typedef struct
{
    UWORD vx,vy,vz,pad;
}USVECTOR;



// !!!!!The verts MUST BE FIRST!!!!!
__declspec(scratchpad)	SVertexCache verts[25];
__declspec(scratchpad)	STextureRefMap *tr[4];
__declspec(scratchpad)	CPSXMap::S4x4 *tex;
__declspec(scratchpad)	NTAG *o;
__declspec(scratchpad)	POLY_GT4*o4;
__declspec(scratchpad)	CMapCell*cell;
__declspec(scratchpad)	SINT start_x,start_y;
__declspec(scratchpad)	SINT curx,cury;
__declspec(scratchpad)	SINT lxoffs,lyoffs,lzoffs;
//__declspec(scratchpad)	int SPQCode;
__declspec(scratchpad)	int scratchpadwh;

// This next line pads out the camera matrixes to the end of the scratchpad, leaving 4 bytes for
//		any dummy writes required by tight loops etc....
__declspec(scratchpad)	volatile char SPPadoutforMatrices[0x50];

// These need to be the last things in the scratchpad
__declspec(scratchpad)	CCamera *SPCamera;
__declspec(scratchpad)	MATRIX  SPCameraMATRIX;
__declspec(scratchpad)	GMatrix SPCameraGMatrix;
__declspec(scratchpad)	CameraAlreadyLoaded;
//__declspec(text) int feck;

//int OffScreenMask;
//int	GouraudDepth;

#define gte_SetZSF3(r1)\
{\
	__asm_start();\
	__I_addiu(12,0,r1);\
	__I_ctc2(12,29);\
	__asm_end();\
}

inline  void gte_GetZSF3(int *r1)
{
	__asm_start();
	__evaluate(-1,long(r1));
	__I_cfc2(12,29);
	__I_nop();
	__I_sw(12,0,-1);
	__asm_end();
}

inline  void gte_GetZSF4(int *r1)
{
	__asm_start();
	__evaluate(-1,long(r1));
	__I_cfc2(12,30);
	__I_nop();
	__I_sw(12,0,-1);
	__asm_end();
}

#define gte_SetZSF4(r1)\
{\
	__asm_start();\
	__I_addiu(12,0,r1);\
	__I_ctc2(12,30);\
	__asm_end();\
}
#define gte_RotTransPers_nom(r1)\
{   gte_ldv0(r1);\
    gte_rtps(); }


#define gte_RotTransPersXYP(r1,r2,r3)            \
				{	gte_ldv0(r1);   	\
                    gte_rtps();         \
                    gte_stsxy(r2);      \
                    gte_stszotz(r3);    }

#define gte_NormalClip(r1,r2,r3,r4)				\
				{	gte_ldsxy3(r1,r2,r3);	\
					gte_nclip();		\
					gte_stopz(r4);	}

#define gte_DpqColor_nodp(r1)                  \
				{	gte_ldrgb(r1);		\
                    gte_dpcs();     \
                    gte_strgb(r1);  }

#define gte_stsxy3cSV( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_swc2 (12, 0, __arg0);\
   __I_swc2 (13, 8, __arg0);\
   __I_swc2 (14, 16, __arg0);\
   __asm_end();

#define gte_strgbwater( r0 )  \
{   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_mfc2(12,22);\
   __I_lui(13,0x7f);\
   __I_ori(13,13,0x7f7f);\
   __I_srl(12,12,1);\
   __I_and(12,12,13);\
   __I_sw (12, 0, __arg0);\
   __asm_end();\
}

#define gte_movergb2toinput() \
{	__asm_start();\
	__I_mfc2(12,22);\
	__I_nop();\
	__I_mtc2(12,6);\
	__asm_end();\
}



#define gte_stszh (r0) \
	__asm_start();\
	__evaluate (__arg0, (long)(r0));\
	__I_mfc2(12,19);\
	__I_nop();\
	__I_sh(12,0,__arg0)\
	__asm_end();

#define gte_stsxyz( r0 , r1 )  \
   __evaluate (__arg0, (long)(r0));\
   __evaluate (__arg1, (long)(r1));\
   __asm_start();\
   __I_mfc2 (13  ,19);\
   __I_swc2 (14, 0, __arg0);\
   __I_nop();\
   __I_sh  (13, 0, __arg1);\
   __asm_end();

#define gte_stsxyzadd( r0 , r1 , r2)  \
   __evaluate (__arg0, (long)(r0));\
   __evaluate (__arg1, (long)(r1));\
   __evaluate (__arg2, (long)(r2));\
   __asm_start();\
   __I_mfc2 (12  ,19);\
   __I_swc2 (14, 0, __arg0);\
   __I_addi (12 ,12 ,__arg2);\
   __I_nop();\
   __I_sh  (12, 0, __arg1);\
   __asm_end();




#define gte_setv0(x,y,z) \
	__asm_start();\
	__evaluate (__arg0,(long)((y<<16)|(x)));\
	__evaluate (__arg1,(long)(z));\
	__I_mtc2(__arg0,0);\
	__I_mtc2(__arg1,1);\
	__asm_end();


#define gte_ldv0nop( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_lwc2 (0, 0, __arg0);\
   __I_nop();\
   __I_nop();\
   __I_lwc2 (1, 4, __arg0);\
   __asm_end();

#define gte_ldv0znop( r0 )  \
   __evaluate (__arg0, (long)(r0));\
   __asm_start();\
   __I_lwc2 (1, 4, __arg0);\
   __asm_end();


#if TARGET == PSX
#define gte_SetBackColorRatio( r0 )  \
   __evaluate (__arg0, (long)(r0<<4));\
   __asm_start();\
   __I_mtc2	(0,6);\
   __I_ctc2 (__arg0, 13);\
   __I_ctc2 (__arg0, 14);\
   __I_ctc2 (__arg0, 15);\
   __asm_end();


// These two values define the colours for the BLUE and RED/GREEN values... (256 is standard intensity)
#define WATER_BLUE  0x7fffffff
#define WATER_GREEN 0x7fffffff

#define gte_ldrgbwater(ptr)		\
	__asm_start();						\
	__evaluate(__arg0,(long)ptr); \
	__I_lw(12,0,__arg0);			\
	__I_nop();\
	__I_srl(12,12,8);\
	__I_ori(12,12,0xffa0);\
	__I_sll(12,12,8);\
	__I_mtc2(12,6);	\
	__asm_end();



#if 0
#define gte_BuildHeightNormal(d)\
{\
	__asm_start();\
	__evaluate(__arg0,long(d));\
	__I_andi(13,__arg0,4095);\
	__I_lui(12,1024);\
	__I_or(12,13,__arg0);\
	__I_mtc2(12,0);\
	__I_sub(13,0,__arg0);\
	__I_mtc2(13,1);\
}
#else
//NEW version of CodeWarrior supports this....
inline void gte_BuildHeightNormal(SINT d)
{
	register long tmp1,tmp2;
	asm ("
			andi	%2,%0,4095
			lui		%1,1024
			or		%1,%2,%0
			mtc2	%1,$0
			sub		%2,zero,%0
			mtc2	%2,$1"
			: : "r"(d),"r"(tmp1),"r"(tmp2):);
}
#endif





#endif
inline void maddPrim(unsigned long *ot,POLY_GT3*p)
{
	__evaluate(-1,(long)ot);
	__evaluate(-2,(long)p);
	__asm_start();
	__I_lwl(12,2,-1);
	__I_sw(-2,0,-1);
	//__I_sll(13,-2,8);
	//__I_swl(13,2,-1);
	__I_swl(12,2,-2);
	__asm_end();
}



inline SINT GetDPQRadius(SINT x,SINT y,SINT z,SINT radius)
{
	register int retval;
	asm volatile("	sll		%2,%2,16
					andi	%1,%1,0xffff
					or		%1,%1,%2
					mtc2	%3,$1
					mtc2	%1,$0
					ctc2	%4,$27
					ctc2    $0,$28
					nRTPS
					nop
					mfc2	%0,$8
					nop
					sra		%0,%0,4"
					:"=r"(retval)
					:"r"(x),"r"(y),"r"(z),"r"(radius)
					:);
	return retval;
}


inline SINT GetSXY(void)
{
	register SINT retval;
	asm volatile("	mfc2 %0,$14
					nop"
					:"=r"(retval):
					:
					);
	return retval;
}
inline SINT GetSZ(void)
{
	register SINT retval;
	asm volatile("	mfc2 %0,$19
					nop"
					:"=r"(retval):
					:
					);
	return retval;
}




#define gte_stsxyz( r0 , r1 )  \
   __evaluate (__arg0, (long)(r0));\
   __evaluate (__arg1, (long)(r1));\
   __asm_start();\
   __I_mfc2 (13  ,19);\
   __I_swc2 (14, 0, __arg0);\
   __I_nop();\
   __I_sh  (13, 0, __arg1);\
   __asm_end();











#define map_addPrim(ot,p) NTAG_addPrim((NTAG*)(ot),(long*)(p))

//#define map_addPrim(ot,p) maddPrim((unsigned long*)ot,(POLY_GT3*)p)



//******************************************************************************************





void CPSXMap::Render4x4();



void SetLightMatrix(void)
{
///////////////////////////////////////

    MATRIX lcol,ldir;

/*Col matrix: Components are multipliers. 1.0=full on, 0.0=full off
 *  r1,r2,r3
 *  g1,g2,g3
 *  b1,b2,b3
 */
//    SetMLight(lcol.m,0, 0.34 , 0.05, -0.10);
//    SetMLight(lcol.m,1, -0.15 , -0.15 , 0.15);
//    SetMLight(lcol.m,2, 0.25 , 0.25 , 0.25);

//    SetMLight(lcol.m,0, 1, 1, 0);
//    SetMLight(lcol.m,1, 0, 0, 1);
//    SetMLight(lcol.m,2, -0.5, -0.5, -0.5);

//    SetMLight(lcol.m,0, 0, 0, 0);
//    SetMLight(lcol.m,1, 0, 0, 0);
//    SetMLight(lcol.m,2, 0, 0, 0);

//    SetMLight(lcol.m,0, 0.15 , 0.15, -0.15);
//    SetMLight(lcol.m,1, -0.2 , -0.2 , 0.2);

    SetMLight(lcol.m,0, 0.10 , 0.10, -0.10);
    SetMLight(lcol.m,1, -0.15 , -0.15 , 0.2);

//    SetMLight(lcol.m,0, 0.05 , 0.05, -0.05);
//    SetMLight(lcol.m,1, -0.07 , -0.07 , 0.1);
    SetMLight(lcol.m,2, 0, 0, 0);


/*Dir matrix:
 * x1,y1,z1
 * x2,y2,z2
 * x3,y3,z3
*/
//    ldir.m[0][0]=-0x0650; ldir.m[0][1] =  0x0000; ldir.m[0][2] = -0x0650;  // from right
//    ldir.m[1][0]= 0x0650; ldir.m[1][1] =  0x0000; ldir.m[1][2] = -0x0650;  // from left
//    ldir.m[2][0]= 0x0000; ldir.m[2][1] = -0x0650; ldir.m[2][2] = -0x0650;  // diagonally down & front
//    ldir.m[2][0]= -0x0650; ldir.m[2][1] = 0x0000; ldir.m[2][2] = 0x0650;  // diagonally down & front

    ldir.m[0][0]= -0x1000; ldir.m[0][1] = 0x0000; ldir.m[0][2] = 0x0000;  // from right
    ldir.m[1][0]=  0x1000; ldir.m[1][1] = 0x0000; ldir.m[1][2] = 0x0000;  // from left
//    ldir.m[2][0]= 0x0650; ldir.m[2][1] =-0x1000; ldir.m[2][2] = 0x0650;  // diagonally down & front

//    SetBackColor(100,100,100);
    SetBackColor(0, 0, 0);

	SetLightMatrix(&ldir);
    SetColorMatrix(&lcol);



///////////////////////////////////////

}



#define gte_ZeroTrans() {\
		__asm_start();\
		__I_ctc2(0,5);\
		__I_ctc2(0,6);\
		__I_ctc2(0,7);\
		__asm_end();\
}

void MapDrawBottomEdge(void)
{
	CPSXMesh *meshes[4] = { ENGINE.GetNumberedMesh(NM_STADIUM_WALL1),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL2),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL3),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL4)};

	GVector pos=ZERO_GVECTOR;
	GMatrix ori=ID_GMATRIX;
	int x, pv = 0;
    SINT lookx = SPCamera->mLookAt.X.Whole();
	if(MAP_WIDTH==32)lookx=16;
	if(lookx<=(MAP_WIDTH>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	pv += 7;
	for(x=(MAP_WIDTH>>3) - 1;x>0;x--)
	{
		pos.X.SetVal(pv<<16);
		meshes[(x - 1) & 3]->RenderAtPos(pos, ori);
		pv += 8;
	}
	pos.X.SetVal(pv<<16);
	if(lookx>=(MAP_WIDTH>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_CORNER)->RenderAtPos(pos,ori);
}

void MapDrawTopEdge(void)
{
	CPSXMesh *meshes[4] = { ENGINE.GetNumberedMesh(NM_STADIUM_WALL1),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL2),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL3),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL4)};

	GVector pos=ZERO_GVECTOR;
	GMatrix ori;
	ori.MakeRotationYaw(G_PI);
	int x, pv = MAP_WIDTH - 1;
	pos.Y.SetVal((MAP_HEIGHT - 1)<<16);

	pos.X.SetVal(pv<<16);
    SINT lookx = SPCamera->mLookAt.X.Whole();
	if(MAP_WIDTH==32)lookx=16;
	if(lookx>=(MAP_WIDTH>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	pv -= 7;

	for(x=(MAP_WIDTH>>3) - 1;x>0;x--)
	{
		pos.X.SetVal(pv<<16);
		meshes[(x - 1) & 3]->RenderAtPos(pos, ori);
		pv -= 8;
	}
	pos.X.SetVal(pv<<16);
	if(lookx<=(MAP_WIDTH>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_CORNER)->RenderAtPos(pos,ori);
}

void MapDrawRightEdge(void)
{
	CPSXMesh *meshes[4] = { ENGINE.GetNumberedMesh(NM_STADIUM_WALL1),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL2),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL3),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL4)};

	GVector pos=ZERO_GVECTOR;
	GMatrix ori;
	ori.MakeRotationYaw(G_PI_2);
	int y, pv = 0;
	pos.X.SetVal((MAP_WIDTH - 1)<<16);

    SINT looky = SPCamera->mLookAt.Y.Whole();
	if(MAP_HEIGHT==32)looky=16;
	if(looky<=(MAP_HEIGHT>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	pv += 7;

	for(y=(MAP_HEIGHT>>3) - 1;y>0;y--)
	{
		pos.Y.SetVal(pv<<16);
		meshes[(y - 1) & 3]->RenderAtPos(pos, ori);
		pv += 8;
	}
	pos.Y.SetVal(pv<<16);
	if(looky>=(MAP_HEIGHT>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_CORNER)->RenderAtPos(pos,ori);
}


void MapDrawLeftEdge(void)
{
	CPSXMesh *meshes[4] = { ENGINE.GetNumberedMesh(NM_STADIUM_WALL1),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL2),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL3),
							ENGINE.GetNumberedMesh(NM_STADIUM_WALL4)};

	GVector pos=ZERO_GVECTOR;
	GMatrix ori;
	ori.MakeRotationYaw(-G_PI_2);
	int y, pv = MAP_HEIGHT - 1;

	pos.Y.SetVal(pv<<16);
    SINT looky = SPCamera->mLookAt.Y.Whole();
	if(MAP_HEIGHT==32)looky=16;
	if(looky>=(MAP_HEIGHT>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_WALL60)->RenderAtPos(pos,ori);
	pv -= 7;

	for(y=(MAP_HEIGHT>>3) - 1;y>0;y--)
	{
		pos.Y.SetVal(pv<<16);
		meshes[(y - 1) & 3]->RenderAtPos(pos, ori);
		pv -= 8;
	}
	pos.Y.SetVal(pv<<16);
	if(looky<=(MAP_HEIGHT>>1))
		ENGINE.GetNumberedMesh(NM_STADIUM_CORNER)->RenderAtPos(pos,ori);
}


#if 1
#define OLLIM	110
#define ORLIM   110
#define OTLIM	40
#define OBLIM	100
#else
#define OLLIM	0
#define ORLIM   0
#define OTLIM	0
#define OBLIM	0

#endif


//SINT wxoffs;
//SINT wyoffs;

int	checkmask=F_4x4_1;
int edgeflags;

#define EF_L 1
#define EF_R 2
#define EF_T 4
#define EF_B 8

#define EF_LM ~1
#define EF_RM ~2
#define EF_TM ~4
#define EF_BM ~8

#define EF_ALL (EF_L+EF_R+EF_T+EF_B)

#define IS4x4(x) ( ((x)->mFlags&checkmask) )

#define gte_stszh (r0) \
	__asm_start();\
	__evaluate (__arg0, (long)(r0));\
	__I_mfc2(12,19);\
	__I_nop();\
	__I_sh(12,0,__arg0)\
	__asm_end();


//Check out the 'function' dropdown menu in CodeWarrior (the one with the two braces) for this file....

#pragma mark --ooo==============ooo--
#pragma mark !!!!SUPRISE SURPRISE!!!!
#pragma mark --ooo==============ooo--

extern void	Trigger_Vib(int Vib_Num);

void CPSXMap::DrawVisible4x4s()
{

	//if(!(GAME.GetGameTurn()&127))Trigger_Vib(0);


	// Make sure the checkmask is set to a correct value (for start of a level)
	if(checkmask&(~F_4x4_MASK))checkmask=F_4x4_1;  // Make sure no spurious bits are set...
	if((checkmask&(F_4x4_MASK))==F_4x4_MASK)checkmask=F_4x4_1;  // Make sure only *ONE* bit is set....

	int setmask=checkmask;
	checkmask^=F_4x4_MASK;               // Toggle the bit to check for boundaries...

	USVECTOR cb;
	SINT x,y;
	tex = Texnums;
	lxoffs = SPCamera->mPos.X.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);
    lyoffs = SPCamera->mPos.Y.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);
    lzoffs = SPCamera->mPos.Z.GetVal() >> (GINT_SHIFT - PSXENGINE_SCALE);




		gte_SetBackColorRatio(180);

		SetMyDepthQueue();
	//for(start_y=0;start_y<MAP_HEIGHT;start_y+=4)
	start_y=0;
	do
	{
		//for(start_x=0;start_x<MAP_WIDTH;start_x+=4)
		start_x=0;
		do
		{
			SINT draw=0;
			//verts->sx=0x200+(start_x<<PSXENGINE_SCALE)-lxoffs;
			//verts->sy=0x200+(start_y<<PSXENGINE_SCALE)-lyoffs;
			//verts->sz=tex->mMinHeight-lzoffs;
			//gte_ldv0nop(verts);

			SINT sx=0x200+(start_x<<PSXENGINE_SCALE)-lxoffs;
			SINT sy=0x200+(start_y<<PSXENGINE_SCALE)-lyoffs;
			SINT radius=tex->mMaxHeight-tex->mMinHeight;
			SINT sz=tex->mMinHeight+(radius>>1)-lzoffs;  // Get central plane of 4x4.
			//radius>>=4;
			if(radius<0x400)radius=0x400;  // If it's a near-flat 4x4 we need to take into account the WIDTH of it (5 PSX units)
			//printf("Radius: %d\n",radius);


			radius=GetDPQRadius(sx,sy,sz,radius);  // Returns Pixel radius*2
			sx=GetSXY();
			sy=sx>>16;
			sx=(sx<<16)>>16;
			sz=GetSZ();
			UINT sw=PLATFORM.ScreenWidth();

			if(sz &&
			  ((UINT)(sx+radius)<(UINT)(sw+(radius<<1))) &&
			  ((UINT)(sy+radius)<(UINT)(256+(radius<<1))) &&
			  (!(tex->mFlags & F_4x4_NEVER)))
			{

				// JCL - hack this a bit....
				SINT	wh=GAME.GetLevelData()->mGVars[WATER_HEIGHT].GetVal();

				if (!(GAME.IsGamePanelActive()))
				{
					if((wh)&&((rand()&15)==0))       // Do we want a water decal?
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
				// Render as a 2x2 if we're in 2 player mode...
				if((PLATFORM.ScreenWidth()==256))
				{
					if (tex->mFlags & F_4x4_ALWAYS4)
					{
						edgeflags = EF_ALL;
//						edgeflags = 0;
						Render4x4();
					}
					else
						Render2x2();
				}
				else
				{
					// Render 2x2 or 4x4 depending on the last frames' check

					short f=EF_ALL;
					if(IS4x4(tex))
					{
						if(start_x!=0)
							if(IS4x4(tex-1)) f&=EF_LM;

						if(start_x<(MAP_WIDTH-4))
							if(IS4x4(tex+1)) f&=EF_RM;

						if(start_y!=0)
							if(IS4x4(tex-(MAP_WIDTH>>2))) f&=EF_BM;

						if(start_y<(MAP_HEIGHT-4))
							if(IS4x4(tex+(MAP_WIDTH>>2))) f&=EF_TM;

						edgeflags=f;
						Render4x4();
					}
					else
					{
						Render2x2();
					}


					if(sz>(625<<4))
//					if(sz>(550<<4))
//					if(sz>(300<<4))
					{
						tex->mFlags&=~setmask;
					}
					else
					{
						tex->mFlags|=setmask;
					}
				}
			}
			else tex->mFlags&=~setmask;
			//t->mFlags=flags;
			tex++;
			start_x+=4;
		}while(start_x<MAP_WIDTH);
		start_y+=4;
	}while(start_y<MAP_HEIGHT);

//	printf("Drew %d 4x4s, and %d 2x2s\n", jcl_debug, jcl_debug2);

#if 0
	//printf("MINZ: %d  MAXZ: %d\n",minz,maxz);
	tex=Texnums;
	// Now a second loop to build the strip info...Cheat for now.. just draw the feckers...
	int sxo=200;
	int syo=32+((MAP_HEIGHT>>2)<<1);


	int viscount=0;
	for(y=0,start_y=0;y<MAP_HEIGHT>>2;y++,start_y+=4)
	{
		for(x=0,start_x=0;x<MAP_WIDTH>>2;x++,start_x+=4)
		{
			if(t->mFlags&0x80)          // It's visible...
			{
				//start_x=x<<2;
				//start_y=y<<2;
				tr[1]=ENGINE.GetMapTexture(tex->tex1);
				Render4x4();
				viscount++;
				/*
				{
					POLY_FT4*f4=(POLY_FT4*)o4;
					int sx=sxo+(4*x);
					int sy=syo-(2*y);
					f4->x0=sx+(0*4);
					f4->y0=sy+(0*2);
					f4->x1=sx+(1*4);
					f4->y1=sy+(0*2);
					f4->x2=sx+(0*4);
					f4->y2=sy+(1*2);
					f4->x3=sx+(1*4);
					f4->y3=sy+(1*2);
					cell=&mCells[(y<<2+MAP_WIDTH_SHIFT)+(x<<2)];
					*(ULONG*)&(f4->r0)=0x00808080;
					int val=((tr[1]->mX)>>2)+(((tr[1]->mY)>>2)<<8);
                    f4->clut  =tr[1]->mCLUT;
                    f4->tpage = tr[1]->mTPAGE;
					*((UWORD *)(&(f4->u0)))  = val;
					*((UWORD *)(&(f4->u1)))  = val+0x0100;
					*((UWORD *)(&(f4->u2)))  = val+0x0001;
					*((UWORD *)(&(f4->u3)))  = val+0x0101;
				   	setPolyFT4(f4);
					setSemiTrans(f4,0);
					map_addPrim(&o[1],f4);
					f4++;
					o4=(POLY_GT4*)(f4);
				}
				*/
			}
			t++;
		}
	}
	//printf("Visible 4x4's = %d\n",viscount);
	#endif
	//return o4;


}


void    CPSXMap::Render()
{


	o = PLATFORM.GetOrderTable();
	SPPadoutforMatrices[0x4f]=0&0xff;

	SINT istart_y = ((SPCamera->mLookAt.Y.Whole())-24);
    SINT istart_x = ((SPCamera->mLookAt.X.Whole())-24);

	SINT tsy=istart_y;
	SINT tsx=istart_x;

	SINT dlr=0;
	SINT dtb=0;

	if(MAP_WIDTH==32)dlr=1;
	if(MAP_HEIGHT==32)dtb=1;


	limitRange(istart_y,0,MAP_HEIGHT-48);
    limitRange(istart_x,0,MAP_WIDTH-48);


	if((istart_y==0)||dtb)MapDrawBottomEdge();
	if((istart_y==MAP_HEIGHT-48)||dtb)MapDrawTopEdge();
	if((istart_x==0)||dlr)MapDrawLeftEdge();
	if((istart_x==MAP_WIDTH-48)||dlr)MapDrawRightEdge();


	SetLightMatrix();


	gte_ZeroTrans();

	ENGINE.GMatrixToGTE(&SPCamera->mOrientation);

	o4=PLATFORM.GetMapPolys();

	DrawVisible4x4s();

#if 0

	istart_x&=~3;
	istart_y&=~3;
	SINT c0,c1;
    SINT xstore=istart_x;

	for(c0=0;((c0<8)&&(istart_y<MAP_HEIGHT));c0++)
	S4x4	*t = Texnums;
    {
		for(c1=0;((c1<8)&&(istart_x<MAP_WIDTH));c1++)
        {
            start_x=istart_x;
            start_y=istart_y;
            o4=Render4x4(o4);
			istart_x+=4;
        }
        istart_x=xstore;
        istart_y+=4;
    }

#endif

	CPlayer *pl = PLAYER(0);
    DrawPickupBeam(pl);                 // Tell it where the GT4's are...


	PLATFORM.DeclareUsedMiscPolyGT4s(o4);

	//printf("Min Z: %d  Max Z: %d\n",minz,maxz);

	CPSXParticleSystem::Render(&mParticleSystem);
	CPSXParticleSystem::Render(&mEffectParticleSystem);

}



#define t10 v1
#define t11 v0

#define PSXABS1(a) (((a)^((a) >>31))-((a) >>31))


void inline asm DoThreebyThree(CMapCell *Cell,SINT x,SINT y,SINT MapWidth)
{
	lw	t6,scratchpadwh

	addi	t3,a3,-3                  // t3 is map width-3
	sll		t3,t3,5                     // t3 is (mMapWidth-3)<<5 ((width-3)*2)*sizeof CMapCell
	lui		a3,0x1f80                   // a3 points to vertex cache....
	sll     a2,a2,16                    // Get Y into top half of A2
    andi    a1,a1,0xffff                // Mask off top 16 bits of A1
	or      t7,a1,a2                    // T7 is now combined XY pair
	lw		t9,lzoffs                   // Get Z offset into T9
	addi	at,a3,0x03fc                // A dummy spot for the first NCDS result (makes loop quicker), DISCARDED
	bne		t6,zero,HaveWater
	sra		t6,t6,GINT_SHIFT-PSXENGINE_SCALE
	lui		t6,0x7fff                   // Force water as low as possible....
HaveWater:
	sub		t6,t6,t9                    // Subtract the height offset, T6 is now the compare height for the water
	addi	t5,zero,3                   // Want to do 3 rows at once...
DoNextRow:
	li		t8,3
	lh      t0,CMapCellmHeight(a0)  // T0 = mHeight
	lh      t1,CMapCellmOffset(a0)  // T1 = mOffset
    lh      t2,CMapCellmDisplacement(a0)  // T2 = mDisplacement
	addu     t0,t0,t1                    // T0 = mHeight + mOffset
    addu     t0,t0,t2                    // T0 = mHeight + mOffset + mDisplacement
DoFirstPoint:
	// This line is duplicated at the bottom of the loop
	lh      t10,SIZEOFCMAPCELLP2+CMapCellmHeight(a0)  // T0 = mHeight
	lh      t1,SIZEOFCMAPCELLP2+CMapCellmOffset(a0)  // T1 = mOffset
    lh      t2,SIZEOFCMAPCELLP2+CMapCellmDisplacement(a0)  // T2 = mDisplacement
	addu     t10,t10,t1                    // T0 = mHeight + mOffset
    addu     t10,t10,t2                    // T0 = mHeight + mOffset + mDisplacement

	subu	t1,t0,t10                   // T1 = Next height - This height (for normalcolour)
	sll		t4,t1,3                     // T4 is the height difference. Shift up to get in range
	sra   t0,t0,(HEIGHT_SHIFT-PSXENGINE_SCALE)

	sub		t0,t0,t9                    // Subtract the height offset

//////////////////
	mfc2	t1,C2_RGB2
	mtc2    t7,C2_VXY0                  // Put XY0 into GTE
	sw		t1,0(at)  // place result from last  NCDS into scratchpad
	mtc2    t0,C2_VZ0                   // Put Z0 into GTE

	addiu	a1,a1,(2<<PSXENGINE_SCALE)  // A1 is next X coord along.
    andi    a1,a1,0xffff                // Mask off top 16 bits of A1


	lw		t7,CMapCellmColour(a0)
	sub		t1,t0,t6
	bltz	t1,Abovewater
	sll		t1,t1,4
	blt		t1,4096,nocap
	nop
	addi	t1,zero,4096



nocap:


	mtc2	t1,C2_IR0
	mtc2	t7,C2_RGB
	addi	t1,zero,4095
	addi	t7,zero,0xa00
	ctc2	t1,C2_BFC
	ctc2	t7,C2_GFC
	nop
	DPCS
	nop
	mfc2	t7,C2_RGB2
	ctc2	zero,C2_BFC
	ctc2	zero,C2_GFC

Abovewater:

	RTPS                                // Transorm Single point....

	addi	t8,t8,-1                    // Reduce count by one

//////////////////




	mtc2	t7,C2_RGB                   // Set colour for NCDS

	or      t7,a1,a2                    // T7 is now combined XY pair
	addiu	a0,a0,SIZEOFCMAPCELLP2






	lw		t0,SIZEOFCMAPCELLM2+CMapCellmFlags(a0)  // flags, type and mix
	lbu		t1,SIZEOFCMAPCELLM1+CMapCellmFlags(a0)  // and next mix
	addiu	a3,a3,SIZEOFSVERTEXCACHE
	or		t0,t0,t1                    // mixed mixers

// calculate the height normal from the height difference (T4) into at,t4

	blt		t4,4096,notover4096
	sw		t0,SIZEOFSVERTEXCACHEM1+SVertexCacheflags(a3)  // Use delay slot!
	addi	t4,zero,4095
	b		lim
	nop
notover4096:
	bgt		t4,-4096,lim
	nop
	addi	t4,zero,-4095
lim:
	lui		at,1024
	or		at,at,t4                    // v0 is X/Y components
	sub		t4,zero,t4                  // V1 is Z component

	mfc2	t0,C2_SXY2
	mfc2	t1,C2_SZ3

	mtc2	at,C2_VXY0
	mtc2	t4,C2_VZ0

	addi	at,a3,SVertexCachemixcols-SIZEOFSVERTEXCACHE  // point to where to store the NCDS result


	NCDS
	sw		t0,SIZEOFSVERTEXCACHEM1+SVertexCachesx(a3)
	sh		t1,SIZEOFSVERTEXCACHEM1+SVertexCachesz(a3)

	// Use the Delay slot to initiate the loading of the next point:
	//		same number of instructions (no nop) and shaves off a couple of cycles)
	bgtz	t8,DoFirstPoint
	addu	t0,zero,t10                 // Move NEXT height to T0 for next iteration

	// Reset to next row of the map....
	addi	a1,a1,-6<<PSXENGINE_SCALE
    andi    a1,a1,0xffff                // Mask off top 16 bits of A1


	lui		t2,2<<PSXENGINE_SCALE
	addu	a2,a2,t2
	or      t7,a1,a2                    // T7 is now combined XY pair

	addi	t5,t5,-1

	bgtz	t5,DoNextRow
	add		a0,a0,t3

	mfc2	t1,C2_RGB2
	jr	ra
	sw		t1,0(at)  // place result from last  NCDS into scratchpad
}

/*-----------------01/10/1999 11:09-----------------
 * Copies longwords from src to dst for a BYTE length of length
 * --------------------------------------------------*/
void asm lcopy(long *src,long *dst,int length)
{
Loopy:
		lw		t0,0(a0)
		addi	a2,a2,-4
		addi	a0,a0,4
		sw		t0,0(a1)
		bne		a2,zero,Loopy
		addi	a1,a1,4
		jr	ra
		nop
}


inline void DSetUVWH(POLY_GT4*o4,STextureRefMap*tr,SINT uoffs,SINT voffs,SBYTE split)
{
	int val=tr->mX+uoffs+((tr->mY+voffs)<<8);
	int val2=val+0x000f;
	int val3=val+0x0f0f;
	int val4=val+0x0f00;


	if(split)
	{
		*((UWORD *)(&(o4->u2)))  = val;
		*((UWORD *)(&(o4->u0)))  = val2;//+0x000f;
		*((UWORD *)(&(o4->u1)))  = val3;//+0x0f0f;
		*((UWORD *)(&(o4->u3)))  = val4;//+0x0f00;
	}
	else
	{
		*((UWORD *)(&(o4->u0)))  = val;
		*((UWORD *)(&(o4->u1)))  = val2;//+0x000f;
		*((UWORD *)(&(o4->u3)))  = val3;//+0x0f0f;
		*((UWORD *)(&(o4->u2)))  = val4;//+0x0f00;
	}
}
inline void DSetUVWH2(POLY_GT4*o4,STextureRefMap*tr,SINT uoffs,SINT voffs)
{
	int val=((tr->mX+uoffs))+(((tr->mY+voffs))<<8);
	*((UWORD *)(&(o4->u0)))  = val;
	*((UWORD *)(&(o4->u1)))  = val+0x001f;
	*((UWORD *)(&(o4->u2)))  = val+0x1f00;
	*((UWORD *)(&(o4->u3)))  = val+0x1f1f;
}

#define gte_setdatareg(reg,val)\
{\
	__asm_start();\
	__evaluate(__arg0,(long)(reg));\
	__evaluate(__arg1,(long)(val));\
	__I_addi(12,0,__arg1);\
	__I_nop();\
	__I_mtc2(12,__arg0);\
	__asm_end();\
}
#define gte_setctrlreg(reg,val)\
{\
	__asm_start();\
	__evaluate(__arg0,long(reg));\
	__evaluate(__arg1,long(val));\
	__I_addi(12,0,__arg1);\
	__I_nop();\
	__I_ctc2(12,__arg0);\
	__asm_end();\
}

inline void gte_ldsxy3_fast(SINT x1,SINT x2,SINT x3)
{
	asm volatile("	mtc2	%0,$12
					mtc2	%1,$13
					mtc2	%2,$14"
					:
					:"r"(x1),"r"(x2),"r"(x3)
					:);
}
void CPSXMap::Render2x2()
{
    SINT    c0,c1;

	{
		curx=(start_x<<PSXENGINE_SCALE)-lxoffs;
        cury=(start_y<<PSXENGINE_SCALE)-lyoffs;

		tr[1]=ENGINE.GetMapTexture(tex->tex1);
		tr[2]=ENGINE.GetMapTexture(tex->tex2);
		tr[3]=ENGINE.GetMapTexture(tex->tex3);

		SVertexCache*v=verts;

		SetMyDepthQueue();


		gte_SetBackColorRatio(180);

		cell=&mCells[((start_y) << mMapWidthShift) + start_x];

		DoThreebyThree(cell,(start_x<<PSXENGINE_SCALE)-lxoffs,(start_y<<PSXENGINE_SCALE)-lyoffs,mMapWidth);
		MakeColoursCommon(9);
	}

    cury=start_y<<PSXENGINE_SCALE;
	SVertexCache *cv=verts;

    SINT voffs=32;
    SINT uoffs=0;
	for (c0 = 0; c0 < 2; c0++)
	{

		//curx=start_x<<PSXENGINE_SCALE;

        for (c1 =0; c1 < 2; c1++)
		{

			int p1,p2,p3,p4;
		   	p1=*(ULONG*)(&(cv[0].sx));
			p2=*(ULONG*)(&(cv[1].sx));
			p3=*(ULONG*)(&(cv[3].sx));
			p4=*(ULONG*)(&(cv[4].sx));
			if(!((p1&p2&p3&p4)&OffScreenMask))  // Is it off top or left or bottom or right?
			{
				long    z,z2,z3;
				{

					//gte_ldsxy3(*(long*)&(cv[3].sx),*(long*)&(cv[4].sx),*(long*)&(cv[0].sx));
					gte_ldsxy3_fast(p3,p4,p1);
					//z2=1;
					//z3=-1;
					z=((cv[0].sz+cv[1].sz+cv[3].sz+cv[4].sz)>>6)+LANDZ;
					gte_nclip();
                    if ((z >= 0) && (z <  850))
                    {
						SINT	uoffs_real = uoffs;
						SINT	voffs_real = voffs;

                        // construct the quad
						int len;//=9<<24;
						int	SPQCode;
						z2=gte_getopz();
						//gte_stopz(&z2);
						gte_ldsxy0(*(long*)&(cv[1].sx));
						gte_nclip();
						//POLY_GT4*polys[4]={NULL,NULL,NULL,NULL};
						unsigned char flags=cv[0].flags&F_LAYER_MASK;
						int ind=0;
						cv->tlind=3;
//						if(flags&1)tr[0] = (STextureRefMap*)(ENGINE.GetNumberedTexture(cv[0].type+TN_ICE-1));
						if(flags&1)
						{
							SINT	ctype = cv[0].type;
							tr[0] = (STextureRefMap*)(ENGINE.GetNumberedTexture(ctype + TN_ICE-1));
							if ((ctype >= CT_BOOSTR) && (ctype <= CT_BOOSTD))
							{
								// animate
								SINT turn = GAME.GetGameTurn();

								if ((turn & 2) && (!(turn & 8)))
									uoffs_real |= 32;
								else
									uoffs_real &= ~32;

								if ((turn & 4) && (!(turn & 8)))
									voffs_real |= 32;
								else
									voffs_real &= ~32;
							}
						}
						//cv->wtlswap=0;
						//gte_stopz(&z3);
						z3=gte_getopz();

						if((z2>0)||(z3<0))

						{
							if((z2>0)&&(z3<0))  // Most occuring case, standard quad....
							{
						 		len=12;
								SPQCode=0x3c000000;
							}
							else
							{
								len=9;
								SPQCode=0x34000000;
								if((z2<=0)&&(z3<=0))
									cv->tlind=1;
							}

							POLY_GT4*m4=o4;
							//!JCL - nasty hack
							if (!((flags | cv[3].flags) & 1))
								flags|=(cv[3].flags&F_LAYER_MASK);


							//!JCLONELAYER
//							flags &= 0;

							while(flags)
							{
								if(flags&1)    //Have we got First channel content?
                            	{
									//polys[ind]=o4;
									STextureRefMap* trm=tr[ind];
									int t1,t2;


									if(len==12)
									{
										t1=*(ULONG*)(&(cv[1].sx));
										t2=cv[1].mixcols[ind];//|SPQCode;
										*((ULONG*)(&(m4->x3)))=t1;
										*((long*)(&(m4->r3)))=t2;
									}

									t1=cv[cv->tlind].mixcols[ind]|SPQCode;
									t2=cv[4].mixcols[ind];//|SPQCode;
									*((long*)(&(m4->r0)))=t1;
									*((long*)(&(m4->r1)))=t2;

									t1=cv[0].mixcols[ind];//|SPQCode;
									t2=*(ULONG*)(&(cv[cv->tlind].sx));
									*((long*)(&(m4->r2)))=t1;
									*((ULONG*)(&(m4->x0)))=t2;

									t1=*(ULONG*)(&(cv[4].sx));
									t2=*(ULONG*)(&(cv[0].sx));
									*((ULONG*)(&(m4->x1)))=t1;
									*((ULONG*)(&(m4->x2)))=t2;

									//*(long*)(&o4->tag)=len;  // setlen(o4,len) (len is <<24 already)

									m4->clut  =trm->mCLUT;
                                    m4->tpage = trm->mTPAGE;
									DSetUVWH2(m4,trm,uoffs_real,voffs_real);
									if(cv->tlind!=3)*(UWORD*)(&m4->u0)=*(UWORD*)(&m4->u3);
									if(flags>1)  // Do we have another layer to draw?
									{
										m4++;
										m4[-1].tag=((long)(m4))|(len<<24);
									}

									SPQCode|=0x02000000;
								}
								ind++;
								flags>>=1;
							}

							long *first=o[z].firstprim;
							o[z].firstprim=(long*)o4;
							if(!first)o[z].lastprim=(long*)m4;
							m4->tag=(len<<24)|(long)first;
							o4=++m4;

							//{
							//	while(--ind>=0)
							//	if(polys[ind])map_addPrim(&o[z],polys[ind]);
							//}
						}
					}
				}
			}
			cv++;
			uoffs=(uoffs^32);
		}
		voffs=(voffs^32);
		cv++;
	}
	//return o4;
}

// Create mixers on the fly, saves 1.5K and uses up redundant cycles (saves on reads.)
void asm MakeColoursCommon(int count)
{
	lui		a1,0x1f80                   // A1 points to the Vertex Structure
	lui		v0,0x00ff
	add		t9,zero,a0                  // Get count into t9
	ori		v0,v0,0xffff
	li		a2,4096                     // Always need 4096-the value
	li		a3,15                       // Always need the 15-(x+x) value
	ctc2	zero,C2_RFC                 // Set back colour to zero
	ctc2	zero,C2_GFC                 // Set back colour to zero
	ctc2	zero,C2_BFC                 // Set back colour to zero

	// These 8 instructions are duplicated at the end to take advantage of the final DPCS delay-time
	lbu		t5,SVertexCachemix(a1)   // T5 is tha MIX value
	lw		t6,SVertexCachemixcols(a1)   // T6 is the colour value
	andi	t3,t5,0x0f                  // Mask off mix value 1
	sll		t1,t3,4
	or		t1,t3,t1
	sll		t1,t1,4
	or		t1,t1,t3
	sub     t0,a2,t1                    // T1=4096-((t5&0x0f)*273)
Loop:
	mtc2	t0,C2_IR0                   // Set the first mix going
	mtc2	t6,C2_RGB                   // Set the colour we want to interpolate
	andi	t1,t5,0xf0                  // Mask off mix value 2
	srl		t4,t1,4
	DPCS                                // Set the depth queue going
	or		t1,t4,t1
	sll		t1,t1,4
	or		t1,t1,t4
	sub     t0,a2,t1                    // T0=4096-((t5&0xf0)*273)
	lbu		t5,SVertexCachemix+SIZEOFSVERTEXCACHE(a1)   // T5 is tha MIX value FOR NEXT LOOP
	lw		t6,SVertexCachemixcols+SIZEOFSVERTEXCACHE(a1)   // T6 is the colour value FOR NEXT LOOP
	mfc2	t1,C2_RGB2                  // Get resultant colour from gte
	mtc2	t0,C2_IR0                   // Set the second mix going
	add		t3,t3,t4                    // t3=(mix&0x0f)+((mix>>4)&0x0f)
	and		t1,t1,v0
	DPCS
	sub		t3,a3,t3	                // t3=15-(mix1+mix2)
	sw		t1,SVertexCachemixcols+4(a1)  // Store result of first mix
	sll		t1,t3,4
	or		t1,t3,t1
	sll		t1,t1,4
	or		t1,t1,t3
	sub     t0,a2,t1                    // T0=4096-((t5&0x0f)*273)
	andi	t3,t5,0x0f                  // Mask off mix value 1
	mfc2	t1,C2_RGB2                  // Get resultant colour from gte
	mtc2	t0,C2_IR0                   // Set the third mix going
	and		t1,t1,v0
	DPCS
	sw		t1,SVertexCachemixcols+8(a1)  // Store result of second mix
	addi	a1,a1,SIZEOFSVERTEXCACHE


	// Process mixer for next loop - Thes instructions are virtually free...
	sll		t1,t3,4
	or		t1,t3,t1
	sll		t1,t1,4
	or		t1,t1,t3
	sub     t0,a2,t1                    // T1=4096-((t5&0x0f)*273)
	// end process mixer....

	mfc2	t1,C2_RGB2                  // Get resultant colour from gte
	addi	t9,t9,-1                    // Decrement ocunt
	and		t1,t1,v0
	//nop
	bne		t9,zero,Loop
	sw		t1,SIZEOFSVERTEXCACHEM1+SVertexCachemixcols+12(a1)  // Store result of third mix
	jr	ra
	nop
}



void inline asm DoFivebyFive(CMapCell *Cell,SINT x,SINT y,SINT MapWidth)
{
	lw	t6,scratchpadwh
	//lw	t6,GAME.mLevelData.mGVars+WATER_HEIGHT

	addi	t3,a3,-5                  // t3 is map width-5
	sll		t3,t3,4                     // t3 is (mMapWidth-5)<<4
	lui		a3,0x1f80                   // a3 points to vertex cache....
	sll     a2,a2,16                    // Get Y into top half of A2
    andi    a1,a1,0xffff                // Mask off top 16 bits of A1
	or      t7,a1,a2                    // T7 is now combined XY pair
	lw		t9,lzoffs                   // Get Z offset into T9
	addi	at,a3,0x03fc                // A dummy spot for the first NCDS result (makes loop quicker), DISCARDED
	bne		t6,zero,HaveWater
	sra		t6,t6,GINT_SHIFT-PSXENGINE_SCALE
	lui		t6,0x7fff                   // Force water as low as possible....
HaveWater:
	sub		t6,t6,t9                    // Subtract the height offset, T6 is now the compare height for the water
	addi	t5,zero,5                   // Want to do 5 rows at once...
DoNextRow:
	li		t8,5
	lh      t0,CMapCellmHeight(a0)  // T0 = mHeight
	lh      t1,CMapCellmOffset(a0)  // T1 = mOffset
    lh      t2,CMapCellmDisplacement(a0)  // T2 = mDisplacement
	addu     t0,t0,t1                    // T0 = mHeight + mOffset
    addu     t0,t0,t2                    // T0 = mHeight + mOffset + mDisplacement
DoFirstPoint:
	// This line is duplicated at the bottom of the loop
	lh      t10,SIZEOFCMAPCELLP1+CMapCellmHeight(a0)  // T0 = mHeight
	lh      t1,SIZEOFCMAPCELLP1+CMapCellmOffset(a0)  // T1 = mOffset
    lh      t2,SIZEOFCMAPCELLP1+CMapCellmDisplacement(a0)  // T2 = mDisplacement
	addu     t10,t10,t1                    // T0 = mHeight + mOffset
    addu     t10,t10,t2                    // T0 = mHeight + mOffset + mDisplacement

	subu	t1,t0,t10                   // T1 = Next height - This height (for normalcolour)
	sll		t4,t1,3                     // T4 is the height difference. Shift up to get in range
	sra   t0,t0,(HEIGHT_SHIFT-PSXENGINE_SCALE)

	sub		t0,t0,t9                    // Subtract the height offset

	mfc2	t1,C2_RGB2
	mtc2    t7,C2_VXY0                  // Put XY0 into GTE
	sw		t1,0(at)  // place result from last  NCDS into scratchpad
	mtc2    t0,C2_VZ0                   // Put Z0 into GTE

	addiu	a1,a1,(1<<PSXENGINE_SCALE)  // A1 is next X coord along.
    andi    a1,a1,0xffff                // Mask off top 16 bits of A1


	lw		t7,CMapCellmColour(a0)
	sub		t1,t0,t6
	bltz	t1,Abovewater
	sll		t1,t1,4
	blt		t1,4096,nocap
	nop
	addi	t1,zero,4096



nocap:


	mtc2	t1,C2_IR0
	mtc2	t7,C2_RGB
	addi	t1,zero,4095
	addi	t7,zero,0xa00
	ctc2	t1,C2_BFC
	ctc2	t7,C2_GFC
	nop
	DPCS
	nop
	mfc2	t7,C2_RGB2
	ctc2	zero,C2_BFC
	ctc2	zero,C2_GFC

Abovewater:

	RTPS                                // Transorm Single point....

	addi	t8,t8,-1                    // Reduce count by one

//	blt		t0,t6,Abovewater
//	// We're under water so kill the red component and make sure the water appears 'greeny-blue' (B|=0xff G|=0xc0)
//	srl		t1,t7,8
//	ori		t1,t1,0xffa0
//	sll		t7,t1,8

	addiu	a0,a0,SIZEOFCMAPCELL
	mtc2	t7,C2_RGB                   // Set colour for NCDS

	or      t7,a1,a2                    // T7 is now combined XY pair
	lw		t0,SIZEOFCMAPCELLM1+CMapCellmFlags(a0)  // flags, type and mix
	addiu	a3,a3,SIZEOFSVERTEXCACHE

// calculate the height normal from the height difference (T4) into at,t4

	blt		t4,4096,notover4096
	sw		t0,SIZEOFSVERTEXCACHEM1+SVertexCacheflags(a3)  // Use delay slot!
	addi	t4,zero,4095
	b		lim
	nop
notover4096:
	bgt		t4,-4096,lim
	nop
	addi	t4,zero,-4095
lim:
	lui		at,1024
	or		at,at,t4                    // v0 is X/Y components
	sub		t4,zero,t4                  // V1 is Z component

	mfc2	t0,C2_SXY2
	mfc2	t1,C2_SZ3

	mtc2	at,C2_VXY0
	mtc2	t4,C2_VZ0

	addi	at,a3,SVertexCachemixcols-SIZEOFSVERTEXCACHE  // point to where to store the NCDS result


	NCDS
	sw		t0,SIZEOFSVERTEXCACHEM1+SVertexCachesx(a3)
	sh		t1,SIZEOFSVERTEXCACHEM1+SVertexCachesz(a3)

	// Use the Delay slot to initiate the loading of the next point:
	//		same number of instructions (no nop) and shaves off a couple of cycles)
	bgtz	t8,DoFirstPoint
	addu	t0,zero,t10                 // Move NEXT height to T0 for next iteration

	// Reset to next row of the map....
	addi	a1,a1,-5<<PSXENGINE_SCALE
    andi    a1,a1,0xffff                // Mask off top 16 bits of A1


	lui		t2,1<<PSXENGINE_SCALE
	addu	a2,a2,t2
	or      t7,a1,a2                    // T7 is now combined XY pair

	addi	t5,t5,-1

	bgtz	t5,DoNextRow
	add		a0,a0,t3

	mfc2	t1,C2_RGB2
	jr	ra
	sw		t1,0(at)  // place result from last  NCDS into scratchpad
}


void intplmapvertshoriz(SVertexCache *v)
{

	int sx1=v[0].sx;
	int sx2=v[2].sx;
	int sy1=v[0].sy;
	int sy2=v[2].sy;
	int sz1=v[0].sz;
	int sz2=v[2].sz;
	int	csx=verts[12].sx;
	int csy=verts[12].sy;


	sx1=(sx1+sx2)>>1;
	sy1=(sy1+sy2)>>1;
	v[1].sz=(sz1+sz2)>>1;

	if(csx>sx1)sx1-=2;
	else sx1+=2;
	if(csy>sy1)sy1-=1;
	else sy1+=1;
	v[1].sx=sx1;
	v[1].sy=sy1;

	//v[1].mixcols[0]=((v[0].mixcols[0]>>1)&0x007f7f7f)+((v[2].mixcols[0]>>1)&0x007f7f7f);

	sx1=v[4].sx;
	sy1=v[4].sy;
	sz1=v[4].sz;

	sx1=(sx1+sx2)>>1;
	sy1=(sy1+sy2)>>1;
	v[3].sz=(sz1+sz2)>>1;

	if(csx>sx1)sx1-=2;
	else sx1+=2;
	if(csy>sy1)sy1-=1;
	else sy1+=1;
	v[3].sx=sx1;
	v[3].sy=sy1;

	//v[3].mixcols[0]=((v[2].mixcols[0]>>1)&0x007f7f7f)+((v[4].mixcols[0]>>1)&0x007f7f7f);
}

void intplmapvertsvert(SVertexCache *v)
{

	int sx1=v[0].sx;
	int sx2=v[10].sx;
	int sy1=v[0].sy;
	int sy2=v[10].sy;
	int sz1=v[0].sz;
	int sz2=v[10].sz;
	int	csx=verts[12].sx;
	int csy=verts[12].sy;


	sx1=(sx1+sx2)>>1;
	sy1=(sy1+sy2)>>1;
	v[1].sz=(sz1+sz2)>>1;

	if(csx>sx1)sx1-=2;
	else sx1+=2;
	if(csy>sy1)sy1-=1;
	else sy1+=1;

	v[5].sx=sx1;
	v[5].sy=sy1;

	//v[5].mixcols[0]=((v[0].mixcols[0]>>1)&0x007f7f7f)+((v[2].mixcols[0]>>1)&0x007f7f7f);

	sx1=v[20].sx;
	sy1=v[20].sy;
	sz1=v[20].sz;

	sx1=(sx1+sx2)>>1;
	sy1=(sy1+sy2)>>1;
	v[15].sz=(sz1+sz2)>>1;

	if(csx>sx1)sx1-=2;
	else sx1+=2;
	if(csy>sy1)sy1-=1;
	else sy1+=1;
	v[15].sx=sx1;
	v[15].sy=sy1;

	//v[15].mixcols[0]=((v[2].mixcols[0]>>1)&0x007f7f7f)+((v[4].mixcols[0]>>1)&0x007f7f7f);
}

void CPSXMap::Render4x4()
{
	SINT    c0,c1;
	//SINT wh = GAME.GetLevelData()->mGVars[WATER_HEIGHT].GetVal();

	{
		tr[1]=ENGINE.GetMapTexture(tex->tex1);
		tr[2]=ENGINE.GetMapTexture(tex->tex2);
		tr[3]=ENGINE.GetMapTexture(tex->tex3);

		SVertexCache*v=verts;

		gte_SetBackColorRatio(180);

   		//__asm_start();
		//__I_addiu(12,0,0xDAE3);
		//__I_addiu(13,0,0x251D);
		//__I_sll(13,13,12);
		//__I_ctc2(12,27);
		//__I_ctc2(13,28);
		//__asm_end();
		SetMyDepthQueue();

		cell=&mCells[((start_y) << mMapWidthShift) + start_x];

		DoFivebyFive(cell,(start_x<<PSXENGINE_SCALE)-lxoffs,(start_y<<PSXENGINE_SCALE)-lyoffs,mMapWidth);

		if(edgeflags)
		{

			if(edgeflags&EF_B)
			{
				intplmapvertshoriz((SVertexCache*)0x1f800000);
			}
			if(edgeflags&EF_T)
			{
				intplmapvertshoriz(&verts[20]);
			}
			if(edgeflags&EF_L)
			{
				intplmapvertsvert(&verts[0]);
			}
			if(edgeflags&EF_R)
			{
				intplmapvertsvert(&verts[4]);
			}
		}
		MakeColoursCommon(25);
	}

    cury=start_y<<PSXENGINE_SCALE;
	SVertexCache *cv=verts;
	SINT xc=(start_x+4==mMapWidth)?3:4;
	SINT yc=(start_y+4==mMapHeight)?3:4;

	for (c0 = start_y; c0 < start_y+yc; c0++)
	{
        SINT voffs=48 - (((c0 ) & 3) << 4);
        SINT uoffs=(((start_x ) & 3) << 4);

		curx=start_x<<PSXENGINE_SCALE;

        for (c1 = start_x; c1 < start_x+xc; c1++)
		{
			int p1,p2,p3,p4;
			p1=*(ULONG*)(&(cv[0].sx));
			p2=*(ULONG*)(&(cv[1].sx));
			p3=*(ULONG*)(&(cv[5].sx));
			p4=*(ULONG*)(&(cv[6].sx));
			if(!((p1&p2&p3&p4)&OffScreenMask))  // Is it off top or left or bottom or right?
			{
				long    z,z2,z3;
				//if((cv[0].sz)&&(cv[1].sz)&&(cv[5].sz)&&(cv[6].sz))  // only if all Zs were in screen range...
				{
					register SVertexCache *p0,*p1,*p2,*p3;
					if(cv->flags&F_REVERSE_SPLIT)
					{
						p0=cv+6;
						p1=cv+1;
						p2=cv+5;
						p3=cv;
						//*(long*)&pts[0]=0x00050106;
					}
					else
					{
						p0=cv+5;
						p1=cv+6;
						p2=cv+0;
						p3=cv+1;

						//*(long*)&pts[0]=0x01000605;
					}

					gte_ldsxy3(*(long*)&(p0->sx),*(long*)&(p1->sx),*(long*)&(p2->sx));
					gte_nclip();

					z=((p0->sz+p1->sz+p2->sz+p3->sz)>>6)+LANDZ;
					if ((z >= 0) && (z <  OT_LENGTH))
                    {
						SINT	uoffs_real = uoffs;
						SINT	voffs_real = voffs;

                        // construct the quad
						int len;
						//gte_stopz(&z2);
						z2=gte_getopz();
						int	SPQCode;
						gte_ldsxy0(*(long*)&(p3->sx));
						gte_nclip();
						int flags=cv[0].flags&F_LAYER_MASK;

						int ind=0;
						z3=gte_getopz();
						if(flags&1)
						{
							SINT	ctype = cv[0].type;
							tr[0] = (STextureRefMap*)(ENGINE.GetNumberedTexture(ctype + TN_ICE-1));
							if ((ctype >= CT_BOOSTR) && (ctype <= CT_BOOSTD))
							{
								// animate
								SINT turn = GAME.GetGameTurn();

								if ((turn & 2) && (!(turn & 8)))
									uoffs_real |= 32;
								else
									uoffs_real &= ~32;

								if ((turn & 4) && (!(turn & 8)))
									voffs_real |= 32;
								else
									voffs_real &= ~32;
							}
						}

						//gte_stopz(&z3);
						//int tlind=5;


						if((z2>0)||(z3<0))

						{
							if((z2>0)&&(z3<0))  // Most occuring case, standard quad....
							{
						 		len=12;
								SPQCode=0x3c000000;
							}
							else
							{
								len=9;
								SPQCode=0x34000000;
								if((z2<=0)&&(z3<=0))
									p0=p3;
							}

							POLY_GT4*m4=o4;

							//!JCLONELAYER
//							if (flags & 2) flags = 2;
//							else if (flags & 4) flags = 4;
//							else flags = 8;

//							if (flags & 8) flags = 8;
//							else if (flags & 4) flags = 4;
//							else flags = 2;

//							flags = 8;

							while(flags)
							{
								if(flags&1)//(flags<<31)<0)
								{
									int t1,t2;
									STextureRefMap* trm=tr[ind];


									//Speedup?
									if(len==12)
									{
										t1=*(ULONG*)(&p3->sx);
										t2=p3->mixcols[ind];//|SPQCode;
										*((ULONG*)(&(m4->x3)))=t1;
										*((long*)(&(m4->r3)))=t2;
									}

									t1=*(ULONG*)(&p0->sx);
									t2=*(ULONG*)(&p1->sx);
									*((ULONG*)(&(m4->x0)))=t1;
									*((ULONG*)(&(m4->x1)))=t2;

									t1=*(ULONG*)(&p2->sx);
									t2=p0->mixcols[ind]|SPQCode;
									*((ULONG*)(&(m4->x2)))=t1;
									*((long*)(&(m4->r0)))=t2;

									t1=p1->mixcols[ind];//|SPQCode;
									t2=p2->mixcols[ind];//|SPQCode;
									*((long*)(&(m4->r1)))=t1;
									*((long*)(&(m4->r2)))=t2;

									m4->clut  =trm->mCLUT;
                                	m4->tpage = trm->mTPAGE;
									DSetUVWH(m4,trm,uoffs_real,voffs_real,cv->flags&F_REVERSE_SPLIT);
									if(p0==p3)*(UWORD*)(&m4->u0)=*(UWORD*)(&m4->u3);
									SPQCode|=0x02000000;

									if(flags>1)  // Do we have another layer to draw?
									{
										m4++;
										m4[-1].tag=((long)(m4))|(len<<24);
									}
								}
								ind++;
								flags>>=1;
							}

							long *first=o[z].firstprim;
							o[z].firstprim=(long*)o4;
							if(!first)o[z].lastprim=(long*)m4;
							m4->tag=(len<<24)|(long)first;

							o4=++m4;
						}
					}
				}
			}
			cv++;
			uoffs=(uoffs+16)&0x30;
		}
		cv+=(5-xc);
	}
	//return o4;
}



//******************************************************************************************
//GINT V1,S1;

BOOL    CPSXMap::Read(CMEMBUFFER &mem)
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

//				GetCell(c)->mColour = 0x7f7f7f7f;

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
				mem.Read(&Texnums[(cy*(MAP_WIDTH>>2))+cx].tex1,3);

		//printf("0: %03d  1: %03d  2: %03d\n",Texnums[(cy*(MAP_WIDTH>>2))+cx].tex1,Texnums[(cy*(MAP_WIDTH>>2))+cx].tex2,Texnums[(cy*(MAP_WIDTH>>2))+cx].tex3);



			}
		}
		SetLightMatrix();
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





void	CPSXMap::PostLoadProcess()
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

}


//******************************************************************************************


#define BEAM_LONG_VERTS         32
#define BEAM_ROTATE_SPEED       GINT(0,65536/10)
#define BEAM_RADIUS             GINT(8,0)
#define BEAM_LENGTH             (3072)
#define BEAM_GROUND_PENETRATION GINT(-1,0)
#define RING_SEPARATION 4
#define RING_COUNT_MAX  50
#define RING_BRIGHTNESS_PEAK 10
//#define RING_RADIUS (BEAM_RADIUS * G(1,32768))
#define RING_RADIUS (BEAM_RADIUS * G(0, 30000))	// JCL - shrunk a bit
#define RING_START_OFFSET (BEAM_GROUND_PENETRATION + G(2,13107))
#define LASER_RADIUS                (6553 >> PSXENGINE_SCALE)
#define LASER_LENGTH                (3072)
#define LASER_GROUND_PENETRATION    G(3)

SVECTOR cursor_verts[BEAM_LONG_VERTS*2];

void CPSXMap::DrawPickupBeam(CPlayer *pl)
{
#ifndef COMPILE_FOR_SPEED

	NTAG	*o = PLATFORM.GetOrderTable();
	SVECTOR *vert;

    PROFILE_FN(RMDrawPickupBeam);

	ASSERT(pl->mPickup.X != -1);

	CCamera	*cam = ENGINE.GetCamera();
	GMatrix	foo = SPCamera->mOrientation.Transpose();
    GMatrix ori;
    MATRIX mori,mat;
    ori.MakeRotationYaw(G(ENGINE.GetRenderGameTurn() - pl->mPickupBeamStartTime) * BEAM_ROTATE_SPEED);
    STextureRefOther *tr = ENGINE.GetNumberedTexture(TN_BEAM1);
    GVector pos;

	//! Hmmmm
	GVector ac = pl->mAnalogCursor;
	ClipPosition(ac);

	switch (CONTROLS.GetControllerMode())
	{
	case CM_KEY_ALT1:
	case CM_JOY_ALT1:
	case CM_JOY_REL:
	case CM_JOY_GOLF:
//        pos = GVector((ac.X), (ac.Y) , (GetCell(CCellRef(ac))->GetUnderlyingHeight()));// + BEAM_GROUND_PENETRATION);
        pos = GVector((ac.X), (ac.Y) , (GetCell(CCellRef(ac))->GetHeight()));// + BEAM_GROUND_PENETRATION);
		break;
	case CM_JOY_SHUTTLE:
	case CM_JOY_RACER:
	case CM_JOY_DIRECT:
		return ; // no beam in shuttle mode.

	default:
        pos = GVector(G(pl->mPickup.X), G(pl->mPickup.Y), (GetCell(pl->mPickup)->GetUnderlyingHeight()));// + BEAM_GROUND_PENETRATION);
		break;
	}

    GVector offpos = SPCamera->mPos - pos;
    ENGINE.GMatrixToTransposeMATRIX(&ori,&mori);
	ENGINE.GMatrixToMATRIX(&SPCamera->mOrientation, &mat);

    gte_CompMatrix(&mat,&mori,&mat);


    SINT    tx = (((-foo.Row[0] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    SINT    ty = (((-foo.Row[1] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    SINT    tz = (((-foo.Row[2] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);



    mat.t[0]=tx;
    mat.t[1]=ty;
    mat.t[2]=tz;
    gte_SetTransMatrix(&mat);
    gte_SetRotMatrix(&mat);

    if (pl->mPickupBeamState != NO_BEAM)
    {


        SINT   beam_radius = ((BEAM_RADIUS * (pl->mPickupBeamSize) * GINT(0,6553)).GetVal())>>12;
        //SINT beam_radius=256;
//        SINT    intensity = pl->mPickupBeamSize.Whole() * (0x80 / 10);
        SINT    intensity = pl->mPickupBeamSize.Whole() * (0x80 / 20);  // JCL - tone down the beam
		DWORD   col = 0x3e000000|((intensity*(0x010101)&0x00ffffff));

        vert=cursor_verts;

        SINT    c0, c1, cv = 0;

		long	colour2=((intensity*0x010101)&0x00ffffff)|0x3e000000;
		for (c0 = 0; c0 < 2; c0 ++)
        {

            for (c1 = 0; c1 < BEAM_LONG_VERTS; c1 ++)
            {

                vert->vx=(rsin(c1*(4096/BEAM_LONG_VERTS))*beam_radius)>>12;
                vert->vy=(rcos(c1*(4096/BEAM_LONG_VERTS))*beam_radius)>>12;
                //! JCL quick hack
//                GINT mapheight = MAP.GetMapHeight(GVector(pos.X + GSIN(G(c1) * G_2PI / BEAM_LONG_VERTS) * GINT(beam_radius << 12),
//               										  pos.Y + GCOS(G(c1) * G_2PI / BEAM_LONG_VERTS) * GINT(beam_radius << 12),
//                										  G0));
//                vert->vz = ((c0 - 1)) * BEAM_LENGTH + ((mapheight - pos.Z).mVal >> (GINT_SHIFT-PSXENGINE_SCALE));
                vert->vz = ((c0 - 1)) * BEAM_LENGTH;
                gte_ldv0(vert);
                gte_rtps();

    // Pull Screen Z/X/Y from the GTE registers and check to see if on screen.
                gte_stszotz(&vert->vz);
                gte_stsxy(&vert->vx);

    //            vert->mPos.vz=RotTransPers(&vert->mPos,(long*)(&vert->mPos),&dummy,&dummy);
                //printf("Vert %d    X: %d Y: %d Z: %d\n",c1,vert->mPos.vx,vert->mPos.vy,vert->mPos.vz);
                vert++;
            }
        }

        // and draw the polys
        //! AAAAAAAARRGH

        SINT    ind[6];
        long z;

        int lu = 0;  //
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



            z=NormalClip(*(ULONG*)(&cursor_verts[ind[0]].vx),
                         *(ULONG*)(&cursor_verts[ind[1]].vx),
                         *(ULONG*)(&cursor_verts[ind[2]].vx));
            if(z<0)
            {
                z=cursor_verts[ind[2]].vz>>2;//;printf("Cursor Z: %d\n",z);
                z -= 2; // why not
                if((z >= 0) && (z <  OT_LENGTH))
                {
					setPolyGT4(o4);
                    *( (ULONG *) (&(o4->x0)) )=*(ULONG *)(&(cursor_verts[ind[0]].vx));
                    *( (ULONG *) (&(o4->x1)) )=*(ULONG *)(&(cursor_verts[ind[1]].vx));
                    *( (ULONG *) (&(o4->x2)) )=*(ULONG *)(&(cursor_verts[ind[2]].vx));
                    *( (ULONG *) (&(o4->x3)) )=*(ULONG *)(&(cursor_verts[ind[3]].vx));
                    *(ULONG*)(&o4->r0)=colour2;
                    *(ULONG*)(&o4->r1)=colour2;
                    *(ULONG*)(&o4->r2)=colour2;
                    *(ULONG*)(&o4->r3)=colour2;
                    o4->tpage = tr->mTPAGE_P1P1;
                    o4->clut  =tr->mCLUT;
                    setUVWH(o4,tr->mX + lu, tr->mY ,63 / BEAM_LONG_VERTS, tr->mH - 16);
					map_addPrim(&o[z],o4);
                    o4++;
                }
            }
            lu += 63 / BEAM_LONG_VERTS;
        }


        // Draw the pulsing circle things..

        SINT    count = (ENGINE.GetRenderGameTurn() - pl->mPickupBeamStartTime) % RING_SEPARATION;
        int numbverts=0;
        vert=cursor_verts;
        tr = ENGINE.GetNumberedTexture(TN_BEAM2);

        while (count < RING_COUNT_MAX)
        {


//			SINT ring_size = (((RING_RADIUS * (pl->mPickupBeamSize) * GINT(0,6553)).GetVal())>>20)+(count*16);
			SINT ring_size = (((RING_RADIUS * (pl->mPickupBeamSize) * GINT(0,6553)).GetVal())>>20)+(count*8);
            SINT ring_height=-(count*count*count)>>4;

			//SINT	ring_height = -((count * count * count * count) * (BEAM_LENGTH / (RING_COUNT_MAX * RING_COUNT_MAX * RING_COUNT_MAX * RING_COUNT_MAX)) + RING_START_OFFSET);
			//SINT	ring_size = ((RING_COUNT_MAX * RING_COUNT_MAX) - ((RING_COUNT_MAX - count) * (RING_COUNT_MAX - count))) * (RING_RADIUS / float(RING_COUNT_MAX * RING_COUNT_MAX)) * (pl->mPickupBeamSize) /10;





            SINT    intensity2;

            if (count < RING_BRIGHTNESS_PEAK)
                intensity2 = count * (0x80 / RING_BRIGHTNESS_PEAK);
            else
                intensity2 = (RING_COUNT_MAX - count) * (0x80 / (RING_COUNT_MAX - RING_BRIGHTNESS_PEAK));

            intensity2 = (intensity * intensity2) / 0x80;
    //      intensity = intensity2;

            col = 0x3e000000+((intensity2*0x00010101)&0x00ffffff);

            vert[0].vx = -ring_size;
            vert[0].vy = -ring_size;
            vert[0].vz = ring_height;

            gte_ldv0(vert);
            gte_rtps();

            vert[1].vx = ring_size;
            vert[1].vy = -ring_size;
            vert[1].vz = ring_height;

            gte_stszotz(&vert->vz);
            gte_stsxy(&vert->vx);
            gte_ldv0(&vert[1]);
            gte_rtps();

            vert[2].vx = -ring_size;
            vert[2].vy = ring_size;
            vert[2].vz = ring_height;
            gte_stszotz(&vert[1].vz);
            gte_stsxy(&(vert[1].vx));
            gte_ldv0(&vert[2]);
            gte_rtps();
            vert[3].vx = ring_size;
            vert[3].vy = ring_size;
            vert[3].vz = ring_height;
            gte_stszotz(&vert[2].vz);
            gte_stsxy(&(vert[2].vx));
            gte_ldv0(&vert[3]);
            gte_rtps();
            gte_stszotz(&vert[3].vz);
            gte_stsxy(&(vert[3].vx));

            z=cursor_verts[0].vz>>2;
            if((z >= 0) && (z <  OT_LENGTH))
            {
				setPolyGT4(o4);
                *( (ULONG *) (&(o4->x0)) )=*(ULONG *)(&(cursor_verts[0].vx));
                *( (ULONG *) (&(o4->x1)) )=*(ULONG *)(&(cursor_verts[1].vx));
                *( (ULONG *) (&(o4->x2)) )=*(ULONG *)(&(cursor_verts[2].vx));
                *( (ULONG *) (&(o4->x3)) )=*(ULONG *)(&(cursor_verts[3].vx));
                *(ULONG*)(&o4->r0)=col;
                *(ULONG*)(&o4->r1)=col;
                *(ULONG*)(&o4->r2)=col;
                *(ULONG*)(&o4->r3)=col;
                o4->tpage = tr->mTPAGE_P1P1;
                o4->clut  =tr->mCLUT;
                setUVWH(o4,tr->mX,tr->mY,tr->mW-1,tr->mH-1);
                map_addPrim(&o[z],o4);
                o4++;
            }
            count += RING_SEPARATION;
        }
    }

    SINT   laser_radius = LASER_RADIUS;
	SINT	intensity = 0x7f;
    DWORD   col = (0x3e<<24)|intensity;  // RB-G-R on PSX....

    vert = cursor_verts;
    tr = ENGINE.GetNumberedTexture(TN_LASER);

    ori.MakeRotationYaw(G0);
    ENGINE.GMatrixToTransposeMATRIX(&ori,&mori);
	ENGINE.GMatrixToMATRIX(&SPCamera->mOrientation, &mat);
    gte_CompMatrix(&mat,&mori,&mat);
    gte_SetRotMatrix(&mat);

//	lots of beams here!

	// laser beam

//    float   v1 = float((rsin(float(ENGINE.GetRenderGameTurn()) / 80.f))) * 0.4f + 0.5f;
//    float   v2 = float((rcos(float(ENGINE.GetRenderGameTurn()) / 80.f))) * 0.4f + 0.5f;
    SINT v1=((rsin((ENGINE.GetRenderGameTurn() << 5) & 4095) * 3) >> 3) + (4096 / 2);
    SINT v2=((rcos((ENGINE.GetRenderGameTurn() << 5) & 4095) * 3) >> 3) + (4096 / 2);

	SINT	c2, c0, c1, cv = 0;
    short   uvs[16][2];

	for (c2 = 0; c2 < 2; c2 ++)
	{
		for (c0 = 0; c0 < 2; c0 ++)
		{
			for (c1 = 0; c1 < 2; c1 ++)
			{
                vert->vx = (laser_radius * c1) - (laser_radius / 2);
                vert->vy = 0;
                vert->vz = ((c0 - 1)) * LASER_LENGTH + 1;  // JCL + 1 ...
                gte_ldv0(vert);
                gte_rtps();
                uvs[cv][0] = tr->mX + (c0 * tr->mW);
                uvs[cv][1] = tr->mY + (c1 * 2) + ((((c2 == 0) ? v1 : v2) * tr->mH) >> 12);
                gte_stsxy(&vert->vx);
                gte_stszotz(&vert->vz);
                vert++;
                cv++;
            }
		}
	}

    long z=cursor_verts[2].vz>>2;
    if((z >= 0) && (z <  OT_LENGTH))
    {
		setPolyGT4(o4);
        *( (ULONG *) (&(o4->x0)) )=*(ULONG *)(&(cursor_verts[0].vx));
        *( (ULONG *) (&(o4->x1)) )=*(ULONG *)(&(cursor_verts[1].vx));
        *( (ULONG *) (&(o4->x2)) )=*(ULONG *)(&(cursor_verts[2].vx));
        *( (ULONG *) (&(o4->x3)) )=*(ULONG *)(&(cursor_verts[3].vx));
        *(ULONG*)(&o4->r0)=col;
        *(ULONG*)(&o4->r1)=col;
        *(ULONG*)(&o4->r2)=col;
        *(ULONG*)(&o4->r3)=col;
        o4->tpage = tr->mTPAGE_P1P1;
        o4->clut  =tr->mCLUT;
        o4->u0=uvs[0][0];
        o4->v0=uvs[0][1];
        o4->u1=uvs[1][0];
        o4->v1=uvs[1][1];
        o4->u2=uvs[2][0];
        o4->v2=uvs[2][1];
        o4->u3=uvs[3][0];
        o4->v3=uvs[3][1];
        map_addPrim(&o[z],o4);
        o4++;
    }
    z=cursor_verts[6].vz>>2;
    if((z >= 0) && (z <  OT_LENGTH))
    {
		setPolyGT4(o4);
        *( (ULONG *) (&(o4->x0)) )=*(ULONG *)(&(cursor_verts[4].vx));
        *( (ULONG *) (&(o4->x1)) )=*(ULONG *)(&(cursor_verts[5].vx));
        *( (ULONG *) (&(o4->x2)) )=*(ULONG *)(&(cursor_verts[6].vx));
        *( (ULONG *) (&(o4->x3)) )=*(ULONG *)(&(cursor_verts[7].vx));
        *(ULONG*)(&o4->r0)=col;
        *(ULONG*)(&o4->r1)=col;
        *(ULONG*)(&o4->r2)=col;
        *(ULONG*)(&o4->r3)=col;
        o4->tpage = tr->mTPAGE_P1P1;
        o4->clut  =tr->mCLUT;
        o4->u0=uvs[4][0];
        o4->v0=uvs[4][1];
        o4->u1=uvs[5][0];
        o4->v1=uvs[5][1];
        o4->u2=uvs[6][0];
        o4->v2=uvs[6][1];
        o4->u3=uvs[7][0];
        o4->v3=uvs[7][1];

//        setUVWH(o4,tr->mX,tr->mY,tr->mW-1,tr->mH-1);
        map_addPrim(&o[z],o4);
        o4++;
    }



#undef BEAM_LONG_VERTS
#undef BEAM_ROTATE_SPEED
#undef BEAM_RADIUS
#undef BEAM_LENGTH
#undef BEAM_GROUND_PENETRATION
#undef RING_SEPARATION
#undef RING_COUNT_MAX
#undef RING_BRIGHTNESS_PEAK
#undef RING_RADIUS
#undef RING_START_OFFSET
#undef LASER_RADIUS
#undef LASER_LENGTH
#undef LASER_GROUND_PENETRATION

#endif                                  // COMPILE_FOR_SPEED

}



/*-----------------29/09/1999 15:59-----------------
 * Eventually...
 * --------------------------------------------------
asm addShadowTri(SINT v0,SINT v1,SINT v2,STextureRefMap*tr)
{
}
*/


#define addShadowTri(v0,v1,v2,tr) {                                             \
		setlen(p3,7+1);															\
		*((long *)(&(p3->r0)))  = 0x26202020;	                                \
		/**((long *)(&(p3->r0)))  = 0x26101010;*/	                                \
		*( (ULONG *) (&(p3->x0)) )=*(ULONG *)(&((verts+v0)->sx));               \
        *( (ULONG *) (&(p3->x1)) )=*(ULONG *)(&((verts+v1)->sx));               \
        *( (ULONG *) (&(p3->x2)) )=*(ULONG *)(&((verts+v2)->sx));               \
       p3->tpage = tr->mTPAGE_P1M1;                                             \
      /* p3->tpage = tr->mTPAGE_P1P1;*/                                             \
        p3->clut  = tr->mCLUT;                                                  \
		gte_AverageZ3( ((verts+v0)->sz),((verts+v1)->sz),((verts+v2)->sz),&otz);\
		/*AverageZ3(((verts+v0)->sz,(verts+v1)->sz,(verts+v2)->sz)-16);	*/		\
		otz=(otz>>2)+SHADOWZ;														\
		setpostabr(p3,1);														\
}


////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


extern int ShadowTexNumber;
void	CPSXMap::DrawShadow(IRenderableThing *t)
{
#if 1
#ifndef COMPILE_FOR_SPEED
	STextureRefOther *tr=ENGINE.GetOtherTexture(ShadowTexNumber);

	GINT radius=t->GetRenderRadius();

	CCamera	*cam = ENGINE.GetCamera();
	ENGINE.GMatrixToMATRIX(&SPCamera->mOrientation, &SPCameraMATRIX);
    SetRotMatrix(&SPCameraMATRIX);
//	SINT	z,
	SINT	otz;
//	long dummy;
	GMatrix	foo = SPCamera->mOrientation.Transpose();
    GVector tpos = t->GetRenderPos();
	GVector offpos = SPCamera->mPos - tpos;

	POLY_FT3MODE	*p3 = (POLY_FT3MODE*)PLATFORM.GetMeshPolyGT3s();
	//POLY_GT3		*oldp3 = p3;

	GINT radscale=G(8.0)/(GetMapHeight(tpos)-tpos.Z+G(8,0));  // Change size according to height
	if(GetMapHeight(tpos)>tpos.Z - GINT_HALF) // margin of error.
	{
		radius*=radscale;
		SINT    tx = (((-foo.Row[0] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    	SINT    ty = (((-foo.Row[1] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
    	SINT    tz = (((-foo.Row[2] * (offpos)).GetVal())) >> (GINT_SHIFT - PSXENGINE_SCALE);
		VECTOR	hack = {tx, ty, tz};
    	TransMatrix(&SPCameraMATRIX,&hack);
    	SetTransMatrix(&SPCameraMATRIX);
		NTAG		*o = PLATFORM.GetOrderTable();


		verts[2].sx=0;
		verts[2].sy=0;
		verts[2].sz=(GetMapHeight(tpos)-tpos.Z).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		gte_ldv0nop(&verts[2]);
    	gte_rtps();
    	gte_stsxyzadd(&verts[2].sx,&verts[2].sz,10);

		GVector	ox = GVector(radius, G0, G0);
		GVector	oy = GVector(G0, radius, G0);

		if (t->GetRenderRotateShadow())
		{
			GMatrix	m;
			m.MakeRotationYaw(t->GetRenderYaw());
			ox = m * ox;
			oy = m * oy;
		}

		verts[0].sx=(-ox.X - oy.X).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		verts[0].sy=(-ox.Y - oy.Y).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		verts[0].sz=(GetMapHeight(tpos - ox - oy)-tpos.Z).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
    	gte_ldv0nop(&verts[0]);
    	gte_rtps();
    	gte_stsxyzadd(&verts[0].sx,&verts[0].sz,20);

		verts[1].sx=(ox.X - oy.X).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		verts[1].sy=(ox.Y - oy.Y).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		verts[1].sz=(GetMapHeight(tpos + ox - oy)-tpos.Z).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
    	gte_ldv0nop(&verts[1]);
    	gte_rtps();
    	gte_stsxyzadd(&verts[1].sx,&verts[1].sz,20);

		verts[4].sx=(ox.X + oy.X).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		verts[4].sy=(ox.Y + oy.Y).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		verts[4].sz=(GetMapHeight(tpos + ox + oy)-tpos.Z).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
    	gte_ldv0nop(&verts[4]);
    	gte_rtps();
    	gte_stsxyzadd(&verts[4].sx,&verts[4].sz,20);

		verts[3].sx=(-ox.X + oy.X).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		verts[3].sy=(-ox.Y + oy.Y).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
		verts[3].sz=(GetMapHeight(tpos - ox + oy)-tpos.Z).GetVal()>>(GINT_SHIFT - PSXENGINE_SCALE);
    	gte_ldv0nop(&verts[3]);
    	gte_rtps();
    	gte_stsxyzadd(&verts[3].sx,&verts[3].sz,20);



	/*
 	* Now Draw the shadow using this grid layout....
 	*			v0-----v1
				|\     /|
				| \   / |
				|  \ /  |
				|   v2  |
				|  / \  |
				| /	  \ |
				|/	   \|
				v3-----v4
 	*/


		addShadowTri(0,1,2,tr);
		p3->u0=tr->mX;
		p3->v0=tr->mY+tr->mH-1;
		p3->u1=tr->mX+tr->mW-1;
		p3->v1=tr->mY+tr->mH-1;
		p3->u2=tr->mX+(tr->mW>>1);
		p3->v2=tr->mY+(tr->mH>>1);
		//otz+=10;
		if((otz>=0)&&(otz<OT_LENGTH))
		{
			map_addPrim(&o[otz],p3);
    		p3++;
		}
		addShadowTri(1,4,2,tr);

		//p3->u0=tr->mX+tr->mW-1;
		//p3->v0=tr->mY;
		//p3->u1=tr->mX+tr->mW-1;
		//p3->v1=tr->mY+tr->mH-1;
		p3->u0=tr->mX+tr->mW-1;
		p3->v0=tr->mY+tr->mH-1;
		p3->u1=tr->mX+tr->mW-1;
		p3->v1=tr->mY;
		p3->u2=tr->mX+(tr->mW>>1);
		p3->v2=tr->mY+(tr->mH>>1);
		//otz+=10;
		if((otz>=0)&&(otz<OT_LENGTH))
		{
			map_addPrim(&o[otz],p3);
    		p3++;
		}

		addShadowTri(4,3,2,tr);

		//p3->u0=tr->mX+tr->mW-1;
		//p3->v0=tr->mY+tr->mH-1;
		//p3->u1=tr->mX;
		//p3->v0=tr->mY+tr->mH-1;
		p3->u0=tr->mX+tr->mW-1;
		p3->v0=tr->mY;
		p3->u1=tr->mX;
		p3->v1=tr->mY;
		p3->u2=tr->mX+(tr->mW>>1);
		p3->v2=tr->mY+(tr->mH>>1);
		//otz+=10;
		if((otz>=0)&&(otz<OT_LENGTH))
		{
			map_addPrim(&o[otz],p3);
    		p3++;
		}

		addShadowTri(3,0,2,tr);

		//p3->u0=tr->mX;
		//p3->v0=tr->mY+tr->mH-1;
		//p3->u1=tr->mX;
		//p3->v1=tr->mY;
		p3->u0=tr->mX;
		p3->v0=tr->mY;
		p3->u1=tr->mX;
		p3->v1=tr->mY+tr->mH-1;
		p3->u2=tr->mX+(tr->mW>>1);
		p3->v2=tr->mY+(tr->mH>>1);
		//otz+=10;
		if((otz>=0)&&(otz<OT_LENGTH))
		{
			map_addPrim(&o[otz],p3);
    		p3++;
		}


	}
    PLATFORM.DeclareUsedMeshPolyGT3s((POLY_GT3*)p3);

#endif		// COMPILE_FOR_SPEED
#endif
}


//******************************************************************************************

BOOL    CPSXMap::Init()
{                      // init top level stuff
	if (!(InitCommon()))
		return FALSE;

	//!PSX   hack
    //PSXLoadTimToVRAM("PSXData\\sand.tim", (ULONG *)tempdata);

	return TRUE;
}

//******************************************************************************************
BOOL	CPSXMap::Resize(SINT xs, SINT ys)
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

void	CPSXMap::SetRowWCCheckAndMaxHeight(SINT r, GINT h)
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

void	CPSXMap::SetRowWCAlwaysAndMinHeight(SINT r, GINT h)
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
void	CPSXMap::ClearOtherFlags()
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
void	CPSXMap::SetOtherFlags(const CCellRef &c, SINT flags)
{
	SINT	p = (c.X / 4) + ((c.Y / 4) * (mMapWidth / 4));

	Texnums[p].mFlags |= flags;
}

#endif                                  // TARGET_PSX

