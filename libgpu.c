#include"libgte.h"
#include"libgpu.h"
#include<graph.h>

DISPENV *PutDispEnv(DISPENV *env)
{
}



DRAWENV *PutDrawEnv(DRAWENV *env)
{
}



DISPENV *GetDispEnv(DISPENV *env)
{
}



DRAWENV *GetDrawEnv(DRAWENV *env)
{
}



DISPENV *SetDefDispEnv(DISPENV *disp, int x, int y, int w, int h)
{
}



DRAWENV *SetDefDrawEnv(DRAWENV *draw, int x, int y, int w, int h)
{
}




TIM_IMAGE *ReadTIM(TIM_IMAGE *timimg)
{
}



TMD_PRIM *ReadTMD(TMD_PRIM *tmdprim)
{
}



int CheckPrim(char *s, u_long *p)
{
}



int ClearImage(RECT *rect, u_char r, u_char g, u_char b)
{
}



int DrawSync(int mode)
{
}



int FntOpen(int x, int y, int w, int h, int isbg, int n)
{
}



int GetDispMode(int width, int height, int isinter)
{
}



int GetGraphType(void)
{
}



int LoadImage(RECT *rect, u_long *p)
{
}



int MargePrim()
{
}



int MoveImage(RECT *rect, int x, int y)
{
}



int OpenTIM(u_long *addr)
{
}



int OpenTMD(u_long *tmd, int obj_no)
{
}



int ResetGraph(int mode)
{
	_setvideomode( _MRES256COLOR );
}



int SetGraphDebug(int level)
{
}



int StoreImage(RECT *rect, u_long *p)
{
}



int VSyncCC(int mode)
{
	// Can't really implement this function in C
}



u_long *ClearOTag(u_long *ot, int n)
{
	// clear object table in forwards direction
	u_long *i, *j;
	int count;

	for (i=(ot), j=(ot+1), count = 1; count < n; i++, j++, count++)
	  {
		*i =  ((u_long) j) & 0x00ffffff;
	  }
	  *i = 0x00ffffff;
}



u_long *ClearOTagR(u_long *ot, int n)
{
	u_long * i, * j;

	// clear object table in reverse direction
	for( i=(ot+n-1), j=(ot+n-2); 	j>=ot; 	i--, j-- )
		*i = (u_long) j;

	*ot = 0x00FFFFFF;
}



u_long *FntFlush(int id)
{
}



u_long *NextPrim(u_long *p)
{
}



u_short GetClut(int x, int y)
{
}



u_short GetTPage(int tp, int abr, int x, int y)
{
}



u_short LoadClut(u_long *clut, int x, int y)
{
}



u_short LoadTPage(u_long *pix, int tp, int abr, int x, int y, int w, int h)
{
}



u_short LoadTPageA(u_long *pix, int tp, int abr, int x, int y, int w, int h)
{
}



void AddPrim()
{
}



void AddPrims()
{
}



void CatPrim()
{
}



void DrawOTag( u_long * ot)
{
	SVECTOR			*scrCoorPtr;
	u_long  			*terminate;
	long 				outerProd;
	unsigned char  GPUcode;
	unsigned char  GPUlen;
	u_long 			*primPtr;

	primPtr = ot;

	//_clearscreen( _GCLEARSCREEN );
	// memset( 0xA0000, 0, 64000 );
	SetLongMem( 0, (long *)0xA0000, 16000 );

	while( primPtr != 0x00FFFFFF )
	{
		GPUcode = *(((unsigned char *)primPtr) + 7);
		GPUlen = *(((unsigned char *)primPtr) + 3);

		switch( GPUcode )
		{
			case 0x29:	/* flat plain quads */
			case 0x28:
				break;

			case 0x21:	/* flat plain traingle */
			case 0x20:
				break;

			case 0x39:	/* gouraud plane quad */
			case 0x38:
				_setcolor( 1 );
				_moveto( (*(primPtr+2)) & 0x0000FFFF, (*(primPtr+2)) >> 16 );
				_lineto( (*(primPtr+4)) & 0x0000FFFF, (*(primPtr+4)) >> 16 );
				_lineto( (*(primPtr+8)) & 0x0000FFFF, (*(primPtr+8)) >> 16 );
				_lineto( (*(primPtr+6)) & 0x0000FFFF, (*(primPtr+6)) >> 16 );
				_lineto( (*(primPtr+2)) & 0x0000FFFF, (*(primPtr+2)) >> 16 );
				break;

			case 0x31:	/* gouraud plane triangle */
			case 0x30:
				printf("GP3");
				break;

			case 0x2D:	/* flat textured quad */
			case 0x2C:
				break;

			case 0x25:	/* flat textured tri */
			case 0x24:
				break;

			case 0x3D:	/* gouraud textured quad */
			case 0x3C:
				break;

			case 0x35:	/* gouraud textured tri */
			case 0x34:
				break;

			case 0x40:	/* normal and gouraud lines */
			case 0x48:
			case 0x4C:
			case 0x50:
			case 0x58:
			case 0x5C:
			case 0x00:
				break;

			default:
				printf("Warning Unknown Code...%02x\n",GPUcode);
				PadPause();
				break;
		}

		primPtr = (u_long *)(((*primPtr) & 0x00ffffffL));
	}

}



void DrawOTagIO()
{
}



void DrawPrim()
{
}



void DumpClut(u_short clut)
{
}



void DumpOTag()
{
}



void DumpTPage(u_short tpage)
{
}



void DumpDispEnv(DISPENV *env)
{
}



void DumpDrawEnv(DRAWENV *env)
{
}



void FntLoad(int tx, int ty)
{
}



void SetDumpFnt(int id)
{
}


	       
//void SetDrawOffset()
//{
//}



void SetSemiTrans()
{
}



void SetShadeTex()
{
}



void SetDispMask(int mask)
{
}



void SetPolyF3(POLY_F3  *p)
{
}



void SetPolyFT3(POLY_FT3 *p)
{
}



void SetPolyG3(POLY_G3  *p)
{
}



void SetPolyGT(POLY_GT3 *p)
{
}



void SetPolyF4(POLY_F4  *p)
{
}



void SetPolyFT4(POLY_FT4 *p)
{
}



void SetPolyG4(POLY_G4  *p)
{
}



void SetPolyGT4(POLY_GT4 *p)
{
}



void SetLineF2(LINE_F2 *p)
{
}



void SetLineG2(LINE_G2 *p)
{
}



void SetSprt8(SPRT_8  *p)
{
}



void SetSprt16(SPRT_16 *p)
{
}



void SetSprt(SPRT    *p)
{
}



void SetTile1(TILE_1  *p)
{
}



void SetTile8(TILE_8  *p)
{
}



void SetTile16(TILE_16 *p)
{
}



void SetTile(TILE    *p)
{
}



void SetDrawEnv(DR_ENV *dr_env, DRAWENV *env)
{
}



void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, RECT *tw)
{
}



void SetTexWindow(DR_TWIN *p, RECT *tw)
{
}



void SetDrawOffset(DR_OFFSET *p, u_short *ofs)
{
}



void SetDrawArea(DR_AREA *p, RECT *r)
{
}



void TermPrim()
{
}







