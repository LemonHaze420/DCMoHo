#include	"Common.h"

#include	"Globals.h"
#include	"Cannon.h"

//******************************************************************************************
#define CANNON_GROUND_SPEED_MAX	GINT(0, 52428)
#define CANNON_GROUND_SPEED_MIN	GINT(0, 10000)

void	CCannon::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	mTimer = 0;

	GET_OID;
	GET_TARGET;
}

//******************************************************************************************
#if TARGET == PC
void	CCannon::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_OID;
	WRITE_TARGET;
}	
#endif

//******************************************************************************************
GVector	CCannon::CalcVelocity()
{
	ASSERT(mTarget); // shouldn't ever be NULL

	GVector	v;

	v = (mTarget->GetPos() - mPos);

	GINT	speed	 = v.MagnitudeXY() / 64;
	if (speed > CANNON_GROUND_SPEED_MAX) speed = CANNON_GROUND_SPEED_MAX;
	if (speed < CANNON_GROUND_SPEED_MIN) speed = CANNON_GROUND_SPEED_MIN;

	v.Normalise();
	v *= speed;
	v.Z = -WORLD.Gravity() * (mTarget->GetPos() - mPos).MagnitudeXY() / speed / 2;

	return v;
}

//******************************************************************************************
void	CCannon::Process()
{
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;

	mTimer ++;

	if ((mTimer > 100) && (mTarget))
	{
		SINT	oid;

		if ((mOID != OID_NONE) && (mOID != OID_INVALID))
			oid = mOID;
		else
			oid = OID_CCannonBall;

		CThing *t = SpawnThing(oid);
		if (t)
		{
			CInitThing i;
			i.mPos = mPos + GetRenderThingEmitter(0);
			i.mVelocity = CalcVelocity();
			t->Init(&i);
		}
		mTimer = 0;
	}
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CCannon::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	SINT	oid;

	if ((mOID != OID_NONE) && (mOID != OID_INVALID))
		oid = mOID;
	else
		oid = OID_CCannonBall;

	WORLD.AccumulateOIDResources(oid);
}
#endif

