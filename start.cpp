#include	"Common.h"

#include	"Start.h"
#include	"Globals.h"
#include	"Primary.h"

//******************************************************************************************
void	CStartP1::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	SitOnGround();
	mFlags |= TF_RENDER_IN_EDITOR_ONLY;
	mStarted = FALSE;

	mYaw = i->mYaw;
}

//******************************************************************************************
void	CStartP1::Process()
{
	if (!mStarted)
	{
		SINT	ch = GAME.GetPlayerCharacter(0);
		SINT	oid = OID_CPrimaryAngel;

		switch (ch)
		{
		case 0:	oid = OID_CPrimaryAngel; break;
		case 1:	oid = OID_CPrimaryApostle; break;
		case 2:	oid = OID_CPrimaryBenny; break;
		case 3:	oid = OID_CPrimaryLockdown; break;
		case 4:	oid = OID_CPrimarySophie; break;
		case 5:	oid = OID_CPrimaryDoctor; break;
		};

		CThing	*t = SpawnThing(oid);

		if (t)
		{
			CInitThing	i;

			i.mPos = mPos;
			i.mAngleType = AT_YPR;
			i.mYaw = mYaw;
 
			t->Init(&i);
			((CPrimary*)t)->SpawnedBy(this) ;
		}	

		mStarted = TRUE;
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
//******************************************************************************************
void	CStartP2::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	SitOnGround();
	mFlags |= TF_RENDER_IN_EDITOR_ONLY;
	mStarted = FALSE;

	mYaw = i->mYaw;
}

//******************************************************************************************
void	CStartP2::Process()
{
	if (!mStarted)
	{
		SINT	ch = GAME.GetPlayerCharacter(1);
		SINT	oid = OID_CPrimaryAngel;

		switch (ch)
		{
		case 0:	oid = OID_CPrimaryAngel; break;
		case 1:	oid = OID_CPrimaryApostle; break;
		case 2:	oid = OID_CPrimaryBenny; break;
		case 3:	oid = OID_CPrimaryLockdown; break;
		case 4:	oid = OID_CPrimarySophie; break;
		case 5:	oid = OID_CPrimaryDoctor; break;
		};

		CThing	*t = SpawnThing(oid);

		if (t)
		{
			CInitThing	i;

			i.mPos = mPos;
			i.mAngleType = AT_YPR;
			i.mYaw = mYaw;

			t->Init(&i);
			((CPrimary*)t)->SpawnedBy(this) ;
		};

		mStarted = TRUE;
	}

	SUPERTYPE::Process();
}
