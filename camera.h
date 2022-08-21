#ifndef CAMERA_H
#define	CAMERA_H

class CCharacter ;

class	CCamera
{
public:
	CCamera();

	void	Update();
	void	FillOut();

	void	PanOut();
	void	PanIn();

	void	Reset(SINT n);

	void	Lock();
	void	Unlock();
	void	EvaluateLockAngles();
	void	SetWinner(CCharacter* c) { mWinner = c ; }

	void	Interpolate();  // JCL - 18-8-00  generate interpolated position

	enum ECameraMode
	{
		ORIENTATION = 0,
		LOOKAT,
	};


	CCharacter* mWinner ;
	GVector	mPos;
	GMatrix	mOrientation;

	GINT	mLinearOffset;
	GVector mLookAt;
	ECameraMode	mMode;

	GVector	mDestLookAt;
	GINT	mDestLinearOffset;

	GINT	mYaw;
	GINT	mDestYaw;

	GINT	mPitch;
	GINT	mDestPitch;

	BOOL	mFollowToggle;

	BOOL	mLockToggle;
	GVector	mLockPos;

	// JCL 18-8  backup stuff for interpolation
	GVector	mOldPos, mOldLookAt;
	GMatrix mOldOrientation;
	

#if TARGET == PSX

    MATRIX  mMOrientation;                      // Contains m[0][0]->m[2][2] (4.12) and t[0] to t[3] (32.0)

#endif

protected:
	void	BuildPosition(GINT yaw, GINT pitch, GVector &pos);
	void	CheckLineOfSight();

	SINT	mNumber;
};

#endif
