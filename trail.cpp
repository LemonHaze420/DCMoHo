#include	"Common.h"

#include	"Trail.h"

#define TRAIL_INTERP	4

//********************************************************************************
CTrail::CTrail()
{
	mStartPoint = 0;
	mLastAdded = MAX_TRAIL_POINTS;

	SINT	c0;

	for (c0 = 0; c0 < MAX_TRAIL_POINTS; c0 ++)
	{
		mPoints[c0].mActive = FALSE;
	}
}

/*
//********************************************************************************
void	CTrail::ProcessNone()
{
	mPoints[mStartPoint].mActive	= FALSE;

	mLastAdded ++;
	mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;
}

//********************************************************************************
void	CTrail::ProcessAdd(const GVector &start, const GVector &end)
{
	mPoints[mStartPoint].mStart		= start;
	mPoints[mStartPoint].mEnd		= end;
	mPoints[mStartPoint].mActive	= TRUE;

	mLastAdded = 0;
	mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;
}*/

//********************************************************************************
void	CTrail::ProcessNone()
{
	SINT	c0;

	for (c0 = 0; c0 < TRAIL_INTERP; c0 ++)
	{
		mPoints[mStartPoint].mActive	= FALSE;
		mLastAdded ++;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;
	}
}

//********************************************************************************

#define MAGIC1	G(0, 7000)
#define MAGIC2	G(0, 15000)
#define MAGIC3	G(0, 3000)

void	CTrail::ProcessAdd(const GVector &start, const GVector &end)
{
	SINT	c0;

	if (mLastAdded != 0)
	{
		// clear intermediary points & create new point
		for (c0 = 0; c0 < TRAIL_INTERP - 1; c0 ++)
		{
			mPoints[mStartPoint].mActive	= FALSE;
			mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;
		}

		mPoints[mStartPoint].mStart		= start;
		mPoints[mStartPoint].mEnd		= end;
		mPoints[mStartPoint].mActive	= TRUE;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;

		mStartVector = ZERO_GVECTOR;
		mEndVector = ZERO_GVECTOR;
	}
	else
	{
		// interpolate for intermediary points.

		GVector	psv, pev;

/*		if (mLastAdded == 1)
		{
			// no vector - use current.
			psv = start - mLastStart;
			pev = end - mLastEnd;
//			psv = ZERO_GVECTOR;
//			pev = ZERO_GVECTOR;
		}
		else*/
		{
			psv = mStartVector;
			pev = mEndVector  ;
//			psv = mStartVector - (start - mLastStart);
//			pev = mEndVector   - (end   - mLastEnd  );
		}
		
		mStartVector = start - mLastStart;
		mEndVector = end - mLastEnd;
#if 0
/*		// 1
		mPoints[mStartPoint].mStart		= mLastStart + (mStartVector * G(0, 16384)) + (psv * MAGIC1);
		mPoints[mStartPoint].mEnd		= mLastEnd   + (mEndVector   * G(0, 16384)) + (pev * MAGIC1);
		mPoints[mStartPoint].mActive	= TRUE;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;

		// 2
		mPoints[mStartPoint].mStart		= mLastStart + (mStartVector * G(0, 32786)) + (psv * MAGIC2);
		mPoints[mStartPoint].mEnd		= mLastEnd   + (mEndVector   * G(0, 32786)) + (pev * MAGIC2);
		mPoints[mStartPoint].mActive	= TRUE;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;

		// 3
		mPoints[mStartPoint].mStart		= mLastStart + (mStartVector * G(0, 49152)) + (psv * MAGIC3);
		mPoints[mStartPoint].mEnd		= mLastEnd   + (mEndVector   * G(0, 49152)) + (pev * MAGIC3);
		mPoints[mStartPoint].mActive	= TRUE;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;*/
#else
		// 1
		mPoints[mStartPoint].mStart		= mLastStart + ((psv * 3) + mStartVector) / 16;
		mPoints[mStartPoint].mEnd		= mLastEnd + ((pev * 3) + mEndVector) / 16;
		mPoints[mStartPoint].mActive	= TRUE;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;

		// 2
		mPoints[mStartPoint].mStart		= mLastStart + (psv + mStartVector) / 4;
		mPoints[mStartPoint].mEnd		= mLastEnd + (pev + mEndVector) / 4;
		mPoints[mStartPoint].mActive	= TRUE;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;

		// 3
		mPoints[mStartPoint].mStart		= mLastStart + ((psv * 3) + (mStartVector * 3 * 3)) / 16;
		mPoints[mStartPoint].mEnd		= mLastEnd + ((pev * 3) + (mEndVector * 3 * 3)) / 16;
		mPoints[mStartPoint].mActive	= TRUE;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;

		// 4
		mPoints[mStartPoint].mStart		= start;
		mPoints[mStartPoint].mEnd		= end;
		mPoints[mStartPoint].mActive	= TRUE;
		mStartPoint = (mStartPoint + 1) % MAX_TRAIL_POINTS;
	}
#endif
	mLastAdded = 0;

	mLastStart = start;
	mLastEnd = end;
}

//********************************************************************************
//********************************************************************************
