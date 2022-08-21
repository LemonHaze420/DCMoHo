#ifndef TRAIL_H
#define TRAIL_H

#define MAX_TRAIL_POINTS	16
//#define MAX_TRAIL_POINTS	20
//#define MAX_TRAIL_POINTS	10

struct	STrailPoint
{
	GVector	mStart;
	GVector	mEnd;
	BOOL	mActive;
};

class	CTrail
{
public:
	CTrail();

	BOOL	IsActive() {return mLastAdded < MAX_TRAIL_POINTS;};

	void	ProcessNone();
	void	ProcessAdd(const GVector &start, const GVector &end);

	STrailPoint	mPoints[MAX_TRAIL_POINTS];

	SINT	mStartPoint;
	SINT	mLastAdded;

	GVector	mLastStart;
	GVector mLastEnd;

	GVector	mStartVector;
	GVector mEndVector;
};


#endif
