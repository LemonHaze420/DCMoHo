#include	"Common.h"

#if 0

#include	"Globals.h"
#include	"Drone.h"

#define	DRONE_ACCEL	GINT(0, 1311)

//******************************************************************************************
void	CDrone::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	SitOnGround();
}

//******************************************************************************************
void	CDrone::Process()
{
	// try to head towards person
	GVector d = WORLD.GetPrimaryPosition() - mPos;
	GINT	dist = d.Magnitude();
	d /= dist;

	if (mFlags & TF_ON_GROUND)
	{
		if ((dist < G(2)) &&
//			(mVelocity.Magnitude() < 0.1f))
			(mVelocity * d < G(0)))
			 mVelocity -= d * DRONE_ACCEL; //go away
		else mVelocity += d * DRONE_ACCEL;
	}

	SUPERTYPE::Process();
}


#endif