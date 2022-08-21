#include	"Common.h"

#if 0

#include	"Clown.h"
#include	"Globals.h"

//******************************************************************************************
void	CClown::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	ChangeState(CS_ASLEEP, CStateInit());
}

//******************************************************************************************
void	CClown::Process()
{
	SUPERTYPE::Process();
}

//******************************************************************************************
#define CLOWN_BULLET_SPEED G(1)

void	CClown::StateNormal()
{
	SINT	 num = GAME.GetGameTurn() + mNumber * 43;

	SINT	n3 = num / 3;

	if ((n3 * 3) == num)
	{
		if ((n3 % 16) < 3)
		{
			CThing	*t = SpawnThing(OID_CPJBullet);
			if (t)
			{
				CInitThing i;
				
				GVector start = mPos + GVector(G0, G0, -G(0, 30000));

				GVector v = (WORLD.GetPrimaryPosition() /*+ GVector(G0, G0, -G(0, 20000))*/ - start);
				GINT	l = v.Magnitude();

				if (l > G(0, 1000))
				{
					v = v / l;
					v = v * CLOWN_BULLET_SPEED;

					i.mVelocity = v;

					i.mVelocity += GVector(G(0, (RANDOM() % 4000)) - G(0, 2000),
										   G(0, (RANDOM() % 4000)) - G(0, 2000),
										   G(0, (RANDOM() % 1000)) - G(0,  500));

					i.mVelocity += mVelocity / 2;

					i.mPos = start + GetRenderThingEmitter(0);

					t->Init(&i);

					SOUND.PlaySample(SOUND_SHOT2, this);
				}
			}
			SetAnimMode(AM_SHOOT);
		}
	}

	SUPERTYPE::StateNormal();
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CClown::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CPJBullet);
}
#endif


#endif