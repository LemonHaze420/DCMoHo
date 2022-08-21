#ifndef	GAMUT_H
#define	GAMUT_H

// These two are now in worldspace/2 coordinates.
//#define MAXDISTANCE		(20.f)
#define MAXDISTANCE		(10.f)
#define SHAGDISTANCE	(1.5f)	// distance to pull the cone back from the view to give a generous gamut

#define MAX_GAMUT_LINES 514

#define GROUNDZ 0.02f

class CCamera;

struct	GamutLine
{
	SINT		Min;
	SINT		Max;
};
class	CGamut
{
public:
	GamutLine	mGamutLines[MAX_GAMUT_LINES];
	SINT		mFirstY;
	SINT		mLastY;

	SINT		mNumP;
	FVector		mP[32];
	SINT		mNumO;
	FVector		mOut[32];	// convex hull of mP
	

	CGamut()  {};
	~CGamut() {};

	void		SetSize(SINT minx, SINT maxx, SINT miny, SINT maxy);

	void		Calculate(const CCamera *c, SINT scale);
	void		AddLine(SINT x, SINT y, SINT x2);
#ifdef DEBUG
//	void		DrawDebug();
#endif

private:
	SINT		MinX;
	SINT		MaxX;
	SINT		MinY;
	SINT		MaxY;

};
#endif