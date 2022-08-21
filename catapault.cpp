#include	"Common.h"

#include	"Globals.h"
#include	"Catapault.h"
#include	"Effect.h"

//******************************************************************************************
void	CCatapault::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mRoll = G_PI / 2;
	mRollVelocity = G0;
	mUp = TRUE;
	mCounter = 60;
	mOrientation.MakeRotationRoll(mRoll);
	StickToGround();
	mPos.Z -= G(0, 4000);
}

//******************************************************************************************
GMatrix	CCatapault::GetAngularVelocity(SINT no)
{
	// build angular velocity matrix
	GMatrix m;
	m.MakeRotationRoll(mRollVelocity);

	return m;  // see - easy wasn't it!
}

//******************************************************************************************
void	CCatapault::Process()
{
	mRollVelocity += GINT(0, 393) * ((mUp) ? -1 : 1);
	mRoll += mRollVelocity;

	if (mUp)
	{
		if (mRoll < (G_PI / 2))
		{
			mRoll =  (G_PI / 2);
			mRollVelocity = -mRollVelocity * GINT(0, 26214);
		}
	}
	else
	{
		if (mRoll >  (G_PI))
		{
			mRoll =  (G_PI);
			mRollVelocity = -mRollVelocity * GINT(0, 26214);
		}
	}

	mCounter --;

	if (mCounter == 0)
	{
		mCounter = 100;
		mUp = !mUp;
	}

	mOrientation.MakeRotationRoll(mRoll);

	SUPERTYPE::Process();
}

//******************************************************************************************
//******************************************************************************************
void	CMechCatapault::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mPitch = G0;
	mPitchVelocity = G0;
	mUp = TRUE;
	mCounter = 60;
	mOrientation2.MakeRotationPitch(mPitch);
	StickToGround();
	mPos.Z -= G(0, 4000);
}

//******************************************************************************************
void	CMechCatapault::Process()
{
	mPitchVelocity += GINT(0, 393) * ((mUp) ? -1 : 1);
	mPitch += mPitchVelocity;

	if (mUp)
	{
		if (mPitch < G0)
		{
			mPitch =  G0;
			mPitchVelocity = -mPitchVelocity * GINT(0, 26214);
		}
	}
	else
	{
		if (mPitch > G_PI_2)
		{
			mPitch = G_PI_2;
			mPitchVelocity = -mPitchVelocity * GINT(0, 26214);
		}
	}

	mCounter --;

	if (mCounter == 0)
	{
		mCounter = 100;
		mUp = !mUp;
	}

	mOrientation2.MakeRotationPitch(mPitch);

	// spawn lightning????
	if ((GAME.GetGameTurn() & 0x3f) == ((mNumber << 2) & 0x3f))
	{
		CFXLightning *t = (CFXLightning *)SpawnThing(OID_CFXLightning);
		if (t)
		{
			// dummy init

			CInitThing i;
			t->Init(&i);

			// here's the interesting stuff
			GVector e1, e2;

			e1 = GetRenderThingEmitter(0) + mPos;
			e2 = GetRenderThingEmitter(1) + mPos;
			t->SetStartAndEnd(e1, e2);
		}
	}




	SUPERTYPE::Process();
}

//******************************************************************************************
GMatrix	CMechCatapault::GetAngularVelocity(SINT no)
{
	if (no == 1)
	{
		// build angular velocity matrix
		GMatrix m;
		m.MakeRotationPitch(mPitchVelocity);

		return m;
	}
	else return SUPERTYPE::GetAngularVelocity(no);
}

//******************************************************************************************
//******************************************************************************************
void	CSteamHammer::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mPitch = G0;
	mPitchVelocity = G0;
	mUp = TRUE;
	mCounter = 60;
	mOrientation2.MakeRotationPitch(mPitch);
	StickToGround();
	mPos.Z -= G(0, 4000);
}

//******************************************************************************************
void	CSteamHammer::Process()
{
	mPitchVelocity += GINT(0, 393) * ((mUp) ? -1 : 1);
	mPitch += mPitchVelocity;

	if (mUp)
	{
		if (mPitch < G0)
		{
			mPitch =  G0;
			mPitchVelocity = -mPitchVelocity * GINT(0, 26214);
		}
	}
	else
	{
		if (mPitch > G_PI_2)
		{
			mPitch = G_PI_2;
			mPitchVelocity = -mPitchVelocity * GINT(0, 26214);
		}
	}

	mCounter --;

	if (mCounter == 0)
	{
		mCounter = 100;
		mUp = !mUp;
	}

	mOrientation2.MakeRotationPitch(mPitch);

	SUPERTYPE::Process();
}

//******************************************************************************************
GMatrix	CSteamHammer::GetAngularVelocity(SINT no)
{
	if (no == 1)
	{
		// build angular velocity matrix
		GMatrix m;
		m.MakeRotationPitch(mPitchVelocity);

		return m;
	}
	else return SUPERTYPE::GetAngularVelocity(no);
}
