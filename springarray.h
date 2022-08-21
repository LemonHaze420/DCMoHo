#ifndef SPRING_ARRAY_H
#define SPRING_ARRAY_H

class	CSpring
{
public:
	GVector	mPos;
	GVector	mVelocity;
	GVector	mForce;

	BOOL	mLocked;
};

class	CSpringArray
{
public:
	CSpringArray(SINT x, SINT y, SINT Z);
	~CSpringArray();

	void	Process();
	void	AddForce(SINT x, SINT y, SINT Z, GVector force);

	CSpring	*mSprings;

	SINT	mXSize, mYSize, mZSize;

	SINT	GetSpring(SINT x, SINT y, SINT z)
	{
		return ((z * mYSize) + y) * mXSize + x; 
	};

protected:
	void	ClearForces();
	void	ProcessPair(SINT s, SINT d, GVector p, GINT dist);
};



#endif