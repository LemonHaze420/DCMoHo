#include	"Common.h"

#include	"Vampire.h"
#include	"Globals.h"

#define	VAMPIRE_ACCEL	GINT(0, 1311)
#define VAMPIRE_STUN_TIME 15

#define VAMPIRE_TILT_DAMP GINT(0, 55000)
#define VAMPIRE_TILT_MAG  GINT(0,  15000)

#define	VAMPIRE_DIE_TILT	 GINT(1, 40000)
#define	VAMPIRE_DIE_ACCEL GINT(0, 500)
#define	VAMPIRE_DIE_DAMP  GINT(0, 26214)

#define VAMPIRE_DAMAGE G(1, 25000)

//******************************************************************************************
void	CVampire::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mTarget = NULL;
}

//******************************************************************************************

#define VAMPIRE_SIGHT	G(12)

void	CVampire::StateNormal()
{
	// has target moved too far away?
//	if (mTarget)
//	{
//		if ((mTarget->GetPos() - mPos).MagnitudeSq() > VAMPIRE_SIGHT * VAMPIRE_SIGHT)
//			mTarget = NULL;		// lost target
//	}

	// is target dead??
	if (mTarget)
	{
		ECharState	cs = mTarget->GetState();
		if ((cs == CS_DEAD) || (cs == CS_FINISHED))
			mTarget = NULL;
	}

	// get a new target
	if (!mTarget)
	{
		SINT		c0, n = WORLD.GetNumNurblings();
		CNurbling	*mint = NULL;
		GINT		mind = VAMPIRE_SIGHT;

		for (c0 = 0; c0 < n; c0 ++)
		{
			CNurbling *nt = WORLD.GetNurbling(c0);

			if (nt)
			{
				ECharState cs = nt->GetState();
				if ((cs != CS_DEAD) && (cs != CS_FINISHED))
				{
					GINT	dist = (mPos - nt->GetPos()).Magnitude();
					if (dist < mind)
					{
						mind = dist;
						mint = nt;
					}
				}
			}
		}
		mTarget = mint;
	}

	GINT	force = G0;
	if (mTarget)
	{
		// try to head towards target
		GVector d = mTarget->GetPos()  - mPos;
		GINT	dist = d.Magnitude();
		d /= dist;
		mDir = d;

		GINT	force = G0;
		if ((dist < G(2)) &&
			(mVelocity * d < G(0)))
			force = G(-1); //go away
		else
			force = G( 1);

		if (mFlags & TF_ON_GROUND)
		{
			Thrust( d * VAMPIRE_ACCEL * force ); 
		}

	}
}

//******************************************************************************************
void	CVampire::Hit(CThing *that)
{
	if (mState != CS_DEAD)
	{
		if (that->IsFriendly())
		{
			that->Damage(VAMPIRE_DAMAGE);
		}
	}
}
