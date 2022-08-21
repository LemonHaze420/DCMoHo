#ifndef GAMUT3_H
#define GAMUT3_H
           //======================================================-==--=-- --  -
          //
         // 3D gamut stuff
        // This is going to 'render' the five planes of the frustrum with depth
       // into a 2D mapwho indexing array so that each gamut cell can be 
      // compared with the min/max of each mapwho's landscape and maybe even
     // each mapwho cell's active members, we are going to try to keep everything
    // in a single mapwho cell, or rather 2*2 of them. note that CLOCKWISE planes
   // set minimums and ANTICLOCKWISE set maximums, therefore the frustrum is an	
  // _inverted_ pyramid.
 //
//======================================================-==--=-- --  -
struct GamutCell
{
	float		Max;
	float		Min;
};
//======================================================-==--=-- --  -
struct C3DGamutLine
{
	GamutCell*	Cells;
};
//======================================================-==--=-- --  -
class C3DGamut
{
	int				BaseX; // subtract these from the test 
	int				BaseY; // pos before 'sampling'
	float			FRadius;
	int				Diameter;
	C3DGamutLine*	Lines;				// storage for the gamut
public:	
	int				XMin;
	int				YMin;
	int				XMax;				// not the last valid but one more
	int				YMax;				// for loop couters < compares etc...

					C3DGamut(int radius);
					~C3DGamut();

	void			AddTri(FVector* pa,FVector* pb,FVector* pc);
	void			MakeIt(FMatrix* mat, FVector* ipos, float depth, float fovx, float fovy);
	void			Clear(void);
	BOOL			InGamut2d(int x, int y);
	BOOL			InGamut3d(int x, int y, float min, float max);

// Don't ever pass this invalid coords, sounds dodgy but if you are getting your x and y ranges
// from the gamut, why worry?

	BOOL			NoClipInGamut3d(int x, int y, float min, float max)
	{
		GamutCell*	gc = &Lines[y-BaseY].Cells[x-BaseX];
		return (gc->Max>min)&&(gc->Min<max);
	};
};
#endif
 