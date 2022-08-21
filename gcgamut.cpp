#include "common.h"
#include "gcgamut.h"
    //======================================================-==--=-- --  -
   //
  // Gamut
 //
//======================================================-==--=-- --  -
C3DGamut::C3DGamut(int radius)
{
	int			nolines = radius*2+1;
	FRadius = (float)radius;
	C3DGamutLine*	gline;
	int			i;
	Diameter = nolines;
	Lines = new C3DGamutLine[nolines];					// alocate surrounding C3DGamutLines 
	for(i=nolines, gline = Lines; i; gline++, i--)
	{
		gline->Cells = new GamutCell[nolines];		// and the cells within them
	}
}
//======================================================-==--=-- --  -
C3DGamut::~C3DGamut()
{
	int			nolines = Diameter;
	int			i;
	C3DGamutLine*	gline;
	for(i=nolines, gline = Lines; i; gline++, i--)
	{
		delete[] gline->Cells;
	}
	delete[] Lines;
}
//======================================================-==--=-- --  -
void C3DGamut::Clear(void)
{
	int			nolines = Diameter;
	int			i;
	C3DGamutLine*	gline;
	GamutCell*	cell;
	for(i=nolines, gline = Lines; i; gline++, i--)
	{
		int		ii;
		for(ii = nolines, cell = gline->Cells; ii; cell++, ii--)
		{
			cell->Max = 10000000.0;
			cell->Min = -10000000.0; 
		}		
	}
}
    //======================================================-==--=-- --  -
   //
  // Add a 'simple span buffered', z-buffered triangle
 //
//======================================================-==--=-- --  -
void C3DGamut::AddTri(FVector* pa,FVector* pb,FVector* pc)
{
	float side = (pb->X-pa->X)*(pc->Y-pa->Y)-(pc->X-pa->X)*(pb->Y-pa->Y);

	if(side==0)
		return;			// totally edge on, forget it ### sort of

	float xdz = ((pb->Z-pa->Z)*(pc->Y-pa->Y)-(pc->Z-pa->Z)*(pb->Y-pa->Y))/side;
// so that was (dz*(pb->Y-pa->Y))/(dx*(pb->Y-pa->Y)) which is dz/dx
	float ydz = ((pc->Z-pa->Z)*(pb->X-pa->X)-(pb->Z-pa->Z)*(pc->X-pa->X))/side;
// I don't really understand why that is dz/dy but it is!
	float bz = pa->Z-xdz*(pa->X)-ydz*(pa->Y); // move to top left of gamut

	float z;

	int			nolines = Diameter;
	int			i;
	C3DGamutLine*	gline;
	GamutCell*	cell;

	if(side>0)
	{
		for(i=nolines, gline = Lines; i; gline++, bz+= ydz, i--)
		{
			int		ii;
			for(z = bz, ii = nolines, cell = gline->Cells; ii; cell++, z+=xdz, ii--)
			{
				if(z>cell->Min) cell->Min=z;
			}
		}
	}
	else
	{
		for(i=nolines, gline = Lines; i; gline++, bz+= ydz, i--)
		{
			int		ii;
			for(z = bz, ii = nolines, cell = gline->Cells; ii; cell++, z+=xdz, ii--)
			{
				if(z<cell->Max) cell->Max=z;
			}
		}
	}
}
//======================================================-==--=-- --  -
void C3DGamut::MakeIt(FMatrix* mat, FVector* ipos, float depth, float fovx, float fovy)
{
	float	xscale = depth*fovx;
	float	yscale = depth*fovy;
	float	munge = depth+0.0001f;	// small intentional error to avoid 'perfect' plane bug
	FVector	pos[5];

	pos[0].X = 0;
	pos[0].Y = 0;
	pos[0].Z = ipos->Z;

	
	BaseX = (int)floorf(ipos->X-FRadius-pos[0].X);					// rewind to (0,0)
	BaseY = (int)floorf(ipos->Y-FRadius-pos[0].Y);					// rewind to (0,0)


	pos[0].X+=FRadius;
	pos[0].Y+=FRadius;

	pos[0]-=mat->Row[2];				// munge back one thingy

	pos[1] = pos[0]+mat->Row[2]*depth+mat->Row[0]*xscale+mat->Row[1]*yscale;
	pos[2] = pos[0]+mat->Row[2]*munge+mat->Row[0]*xscale-mat->Row[1]*yscale;
	pos[3] = pos[0]+mat->Row[2]*depth-mat->Row[0]*xscale-mat->Row[1]*yscale;
	pos[4] = pos[0]+mat->Row[2]*munge-mat->Row[0]*xscale+mat->Row[1]*yscale;

	Clear();
	AddTri(&pos[0],&pos[2],&pos[1]); // right 
	AddTri(&pos[0],&pos[3],&pos[2]); // base 
	AddTri(&pos[0],&pos[4],&pos[3]); // top
	AddTri(&pos[0],&pos[1],&pos[4]); // left
	AddTri(&pos[1],&pos[2],&pos[3]); // far

	XMin = YMin = 12340;
	YMax = XMax = -12340;

	int t;
	for(int i=0; i<5; i++)
	{
		t = (int)pos[i].X;
		if(t>XMax) XMax=t;
		if(t<XMin) XMin=t;
		t = (int)pos[i].Y;
		if(t>YMax) YMax=t;
		if(t<YMin) YMin=t;
	}
	XMax++;
	if(XMax>=Diameter) XMax=Diameter;
	XMin--;
	if(XMin<0) XMin=0;
	YMax++;
	if(YMax>=Diameter) YMax=Diameter;
	YMin--;
	if(YMin<0) YMin=0;
	XMax+=BaseX;
	XMin+=BaseX;
	YMax+=BaseY;
	YMin+=BaseY;
}
//======================================================-==--=-- --  -
BOOL C3DGamut::InGamut2d(int x, int y)
{
	if((x<XMin)||(x>=XMax)) return FALSE;
	if((y<YMin)||(y>=YMax)) return FALSE;
	x-=BaseX;
	y-=BaseY;
	GamutCell*	gc = &Lines[y].Cells[x];
	return gc->Max>=gc->Min;
}
//======================================================-==--=-- --  -
BOOL C3DGamut::InGamut3d(int x, int y, float min, float max)
{
	if((x<XMin)||(x>=XMax)) return FALSE;
	if((y<YMin)||(y>=YMax)) return FALSE;
	x-=BaseX;
	y-=BaseY;
	GamutCell*	gc = &Lines[y].Cells[x];
	return (gc->Max>min)&&(gc->Min<max);
}//======================================================-==--=-- --  -
