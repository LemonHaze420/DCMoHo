#include	"Common.h"

#include	"SpringArray.h"


//******************************************************************************************
CSpringArray::CSpringArray(SINT x, SINT y, SINT z)
{
/*	mXSize = x;
	mYSize = y;
	mZSize = z;

	// Allocate cells
	mSprings = new CSpring[x * y * z];
	ASSERT(mSprings);

	// Clear Cells
	SINT	c = 0;

	for (z = 0; z < mZSize; z++)
		for (y = 0; y < mYSize; y++)
			for (x = 0; x < mXSize; x++)
			{
				mSprings[c].mPos = GVector(G(x), G(y), G(-z));
				mSprings[c].mVelocity = ZERO_GVECTOR;
				mSprings[c].mForce = ZERO_GVECTOR;

//				if (x == 0)
				if ((x == 0) || (x == mXSize - 1))
					mSprings[c].mLocked = TRUE;
				else
					mSprings[c].mLocked = FALSE;

				c ++;
			}*/
}

//******************************************************************************************
CSpringArray::~CSpringArray()
{
/*	if (mSprings)
		delete [] mSprings;*/
}

//******************************************************************************************
void	CSpringArray::ClearForces()
{
/*	SINT c0, n;

	n = mXSize * mYSize * mZSize;

	for (c0 = 0; c0 < n; c0 ++)
		mSprings[c0].mForce = ZERO_GVECTOR;*/
}

//******************************************************************************************
void	CSpringArray::AddForce(SINT x, SINT y, SINT z, GVector force)
{
/*	mSprings[GetSpring(x, y, z)].mForce += force;*/
}

//******************************************************************************************
#define SPRING_FORCE_MOD2	GINT(0, 13107)
#define SPRING_FORCE_MOD1	GINT(0, 16384)
#define	SPRING_VEL_DAMP		GINT(0, 55705)

//******************************************************************************************
void	CSpringArray::ProcessPair(SINT s, SINT d, GVector p, GINT dist)
{
/*	if (GABS(dist) < GINT(0, 65))
		return;

	GVector force1 = ZERO_GVECTOR, force2 = ZERO_GVECTOR;

	// erm... I've got no idea what this function should be...
	GVector	v = mSprings[s].mPos - mSprings[d].mPos;
	GINT	f = v.Magnitude();

	v /= f;
	f -= dist;
	force1 = -(v * f * SPRING_FORCE_MOD1);
	
	mSprings[s].mForce += (force1 + force2) / (dist * dist);*/

}

//******************************************************************************************
void	CSpringArray::Process()
{
/*	// 2 passes

	// first - evaluate forces
	SINT	x, y, z, c = 0;

	for (z = 0; z < mZSize; z++)
		for (y = 0; y < mYSize; y++)
			for (x = 0; x < mXSize; x++)
			{
				// scan the whole bloody thing...

				SINT tx, ty, tz, tc = 0;
				for (tz = 0; tz < mZSize; tz++)
					for (ty = 0; ty < mYSize; ty++)
						for (tx = 0; tx < mXSize; tx++)
						{
							GINT z1 = G(ABS(tx - x));
							GINT z2 = G(ABS(ty - y));
							GINT z3 = G(ABS(tz - z));

							GINT dist = GROOT(z1*z1 + z2*z2 + z3*z3);

							ProcessPair(c, tc, ZERO_GVECTOR, dist);
							tc ++;
						}

				c ++;
			}

	
	// now process velocity & things		
	CSpring	*s = mSprings;

	for (z = 0; z < mZSize; z++)
		for (y = 0; y < mYSize; y++)
			for (x = 0; x < mXSize; x++)
			{
				if (!(s->mLocked))
				{
					s->mVelocity	= (s->mVelocity * SPRING_VEL_DAMP) + s->mForce;
					s->mPos			+= s->mVelocity;
				}

				s->mForce = ZERO_GVECTOR;

				s ++;
			}*/
}

//******************************************************************************************
//******************************************************************************************
