#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H

class CFVF_GTEX
{
	// Flexible vertex structure

public:
	CFVF_GTEX() {mSpecular = 0x00000000;}; // AAAAAAAARGGH a constructor! be careful how you use this!!

	FVector	mPos;
	ULONG	mDiffuse;
	ULONG	mSpecular;
	float	mU, mV;
};
#define FVF_GTEX_FLAGS (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1) 

struct CFVF_GNOTEX
{
	// Flexible vertex structure

	FVector	mPos;
	ULONG	mDiffuse;
};
#define FVF_GNOTEX_FLAGS (D3DFVF_XYZ | D3DFVF_DIFFUSE ) 
//====================================-==--=- --  -
struct	CFVF_UVRGB
{
	FVector	mPos;
	ULONG	mDiffuse;
	float	mU, mV;
};
#define FVF_UVRGB_FLAGS (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) 
//====================================-==--=- --  -
struct	CFVF_TLUV
{
	FVector	mPos;
	float	mRHW;
	float	mU, mV;
};
#define FVF_TLUV_FLAGS ( D3DFVF_XYZRHW | D3DFVF_TEX1 )
//====================================-==--=- --  -
struct	CFVF_TLDSUV
{
	FVector	mPos;
	float	mRHW;
	ULONG	mDiffuse;
	ULONG	mSpecular;
	float	mU, mV;
};
#define FVF_TLDSUV_FLAGS ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1 )
//====================================-==--=- --  -


#endif