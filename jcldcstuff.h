#ifndef JCLDCSTUFF_H
#define JCLDCSTUFF_H

// JCL - Misc definitions needed when getting rid of psx libs

typedef struct {		/* short word type 3D vector */	
	short	vx, vy;
	short	vz, pad;
} SVECTOR;
	       
typedef struct {		/* color type vector */	
	u_char	r, g, b, cd;
} CVECTOR;

typedef struct {
	short x, y;		/* offset point on VRAM */
	short w, h;		/* width and height */
} RECT;

typedef struct  {
	short	m[3][3];	/* 3x3 rotation matrix */
        long    t[3];		/* transfer vector */
} MATRIX;

typedef struct {		/* long word type 3D vector */
	long	vx, vy;
	long	vz, pad;
} VECTOR;

inline void	VSync(int n)
{
#ifdef JCLDCNOPSX			// Code needed:
	...
	Code that waits until the next vertical blank
	...
#endif
}	

inline	void	DrawSync(int n)
{
#ifdef JCLDCNOPSX			// Code needed:
	...
	Code that waits until all asynchronous drawing has been completed
	...
#endif
}	

#define limitRange(x, l, h)	((x)=((x)<(l)?(l):(x)>(h)?(h):(x)))


#endif

