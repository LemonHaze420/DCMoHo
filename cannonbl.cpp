#include	"Common.h"

#include	"Globals.h"
#include	"CannonBl.h"

//******************************************************************************************
void	CCannonBall::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mLanded = FALSE;
}

//******************************************************************************************
void	CCannonBall::Process()
{
	GINT	l = mVelocity.Magnitude();

	if (l > G(0, 100))
	{
		GVector v = mVelocity / l;

		GVector	u = GVector(G0, G0, -G(1));

		GVector	r = v ^ u;
		r.Normalise();
		u = v ^ r;

		mOrientation.Row[0] = r;
		mOrientation.Row[1] = v;
		mOrientation.Row[2] = -u;

		mOrientation.TransposeInPlace();
	}

	if (!mLanded)
	{
		if (mVelocity.Z > G0) // going down!
		{
			if (!(COORDS_INVALID(CCellRef(mPos))))
			{
				GINT map_height = MAP.GetMapHeight(mPos);

				if (mPos.Z + Radius() > map_height)
				{
					mLanded = TRUE;
					MAP.Raindrop(CCellRef(mPos + GVector(GINT_HALF, GINT_HALF, G0)),
						         (G1 - G(0, 40000))); //er... the explosion does this as well!!
					SuicideNext();

					CThing	*t = SpawnThing(OID_CFXExplosion2);
					if (t)
					{
						CInitThing i;
						i.mPos = mPos;
						i.mPos.Z += G(2); // Down a bit...
						t->Init(&i);
					}
				}
			}
		}
	}

	SUPERTYPE::Process();
}


//******************************************************************************************
#ifdef LINK_EDITOR

void	CCannonBall::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CFXExplosion2);
}
#endif
