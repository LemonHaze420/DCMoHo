#include	"Common.h"
#include	"Globals.h"
#include	"Effector.h"

//******************************************************************************************
void	CEffector::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	mYaw = G(RANDOM() % 300) / 10;
	mOrientation.MakeRotationYaw(mYaw);
}

//******************************************************************************************
void	CEffector::Process()
{
	StickToGround();

	// rotate a bit
//	mYaw += G(0, 3276);
	//! don't

	mOrientation.MakeRotationYaw(mYaw);

	SUPERTYPE::Process();
}

//******************************************************************************************
BOOL	CEffector::GetRenderSelected()
{
	//! Don't like this....

	CCellRef m = CCellRef(mPos);
	CCellRef c = PLAYER(0)->GetCursor();

	return (((m.X == c.X) || (m.X == c.X - 1)) &&
			((m.Y == c.Y) || (m.Y == c.Y - 1)));
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CEFRainDrop::Activate()
{
	MAP.Raindrop(CCellRef(mPos), G1);
}

//******************************************************************************************
//******************************************************************************************

#define LINEWAVE_SIZE	GINT(0, 48000)
#define LINEWAVE_COUNT	20
//#define LINEWAVE_SIZE	GINT(0, 32768)
//#define LINEWAVE_COUNT	15
#define LINEWAVE_DELAY	3

void	CEFLineWavePair::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mActive = 0;
	mType	= LWP_NOT_INITIALISED;

	GET_TARGET;
}

//******************************************************************************************
#if TARGET == PC
void	CEFLineWavePair::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	if (mType != LWP_SECONDARY)
		WRITE_TARGET;
}
#endif

//******************************************************************************************
void	CEFLineWavePair::Activate()
{
	if (mTarget)
	{
		if (mType == LWP_SECONDARY)
		{
			mTarget->Activate();
		}
		else
		{
			mDistance = 0;
			mActive = LINEWAVE_COUNT * 3;
			mDelayCounter = LINEWAVE_DELAY;
		}
	}
}

//******************************************************************************************

void	CEFLineWavePair::Process()
{
	// initialise?
	if (mType == LWP_NOT_INITIALISED)
	{
		if (mTarget)
		{
			mTarget->SetTarget(this);
			mType = LWP_PRIMARY;
		}
		else
			mType = LWP_SECONDARY;
	}

#if TARGET == PSX
	if (!(GAME.IsMultiPlayer()))
#endif
	if (mTarget)
	{
		// do the line
		if (mActive > 0)
		{
			mDelayCounter --;
//			if (mDelayCounter == 0)
			{
				mDelayCounter = LINEWAVE_DELAY;
				SINT c0;

				GINT force = LINEWAVE_SIZE / 3;

				if (mActive < 5)
					force *= mActive / 5;

				// do the line
				GVector	d = mTarget->GetPos() - mPos;

				GINT	dx, dy;
				SINT	count;

				if (GABS(d.X) > GABS(d.Y))
				{
					dx = d.X > G0 ?  G1 : -G1;
					dy = d.Y / GABS(d.X);
					count = GABS(d.X).Whole();
				}
				else
				{
					dy = d.Y > G0 ?  G1 : -G1;
					dx = d.X / GABS(d.Y);
					count = GABS(d.Y).Whole();
				}

				GINT	l = d.MagnitudeXY();

				GVector	pos		= mPos + (GVector(d.Y / l, -d.X / l, G0) * mDistance) / 3;

				for (c0 = 0; c0 < count; c0 ++)
				{
					MAP.GetCell(CCellRef(pos - GVector(GINT_HALF, GINT_HALF, G0)))->AddOffsetVelocity(-force);
					pos.X += dx;
					pos.Y += dy;
				}

				mDistance ++;

				mActive --;
			}
		}

		// maybe spit out some lightning!
		if ((mType == LWP_PRIMARY) &&
			(((GAME.GetGameTurn()) & 0x3f) == ((mNumber << 2) & 0x3f)))
		{
			CThing *t = SpawnThing(OID_CFXLightning);
			if (t)
			{
				CInitThing i;
				i.mPos = mPos + GetRenderThingEmitter(0);
				i.mTarget = mTarget->GetNumber();
				t->Init(&i);
			}
		}
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CEFLineWavePair::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CFXLightning);
}
#endif

//******************************************************************************************
//******************************************************************************************

void	CEFLineWavePair2::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mActive = 0;
	mType	= LWP_NOT_INITIALISED;

	GET_TARGET;

	GET_INITIAL_MODE;
	if (mInitialMode == IM_INACTIVE)
		mActivated = FALSE;
	else
		mActivated = TRUE;

	GET_TRIGGER_RESPONSE;
	if (mTriggerResponse == TR_UNUSED)
		mTriggerResponse = TR_ACTIVATE;
	GET_UNTRIGGER_RESPONSE;
	if (mUnTriggerResponse == TR_UNUSED)
		mUnTriggerResponse = TR_NONE;

	mTriggerOnFrame = 0;
}

//******************************************************************************************
#if TARGET == PC
void	CEFLineWavePair2::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}
#endif

//******************************************************************************************
void	CEFLineWavePair2::Activate()
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		{
		mActivated = TRUE;
		mTriggerOnFrame = (GAME.GetGameTurn()+1 ) %0x40;
		break;
		}
	case TR_DEACTIVATE:
		mActivated = FALSE; break;
	};
}

//******************************************************************************************
void	CEFLineWavePair2::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		{
		mActivated = TRUE; 	
		mTriggerOnFrame = (GAME.GetGameTurn()+1) %0x40;
		break;
		}
	case TR_DEACTIVATE:
		mActivated = FALSE; break;
	};

	SUPERTYPE::UnTrigger(top);
}

//******************************************************************************************

void	CEFLineWavePair2::Process()
{
	// initialise?
	if (mType == LWP_NOT_INITIALISED)
	{
		if (mTarget)
		{
			mTarget->SetTarget(this);
			mType = LWP_PRIMARY;
		}
		else
			mType = LWP_SECONDARY;
	}

	if (mActivated)
		if (((GAME.GetGameTurn()) & 0x3f) == mTriggerOnFrame)
			if (mTarget)
			{
				if (mType == LWP_SECONDARY)
				{
//					mTarget->Activate();
				}
				else
				{
					mDistance = 0;
					mActive = LINEWAVE_COUNT * 3;
					mDelayCounter = LINEWAVE_DELAY;

					// maybe spit out some lightning!
					CThing *t = SpawnThing(OID_CFXLightning);
					if (t)
					{
						CInitThing i;
						i.mPos = mPos + GetRenderThingEmitter(0);
						i.mTarget = mTarget->GetNumber();
						t->Init(&i);
					}
				}
			}

#if TARGET == PSX
	if (!(GAME.IsMultiPlayer()))
#endif
	if (mTarget)
	{
		// do the line
		if (mActive > 0)
		{
			mDelayCounter --;
//			if (mDelayCounter == 0)
			{
				mDelayCounter = 2;
				SINT c0;

				GINT force = LINEWAVE_SIZE / 5 ;

				if (mActive < 5)
					force *= mActive / 5;

				// do the line
				GVector	d = mTarget->GetPos() - mPos;

				GINT	dx, dy;
				SINT	count;

				if (GABS(d.X) > GABS(d.Y))
				{
					dx = d.X > G0 ?  G1 : -G1;
					dy = d.Y / GABS(d.X);
					count = GABS(d.X).Whole();
				}
				else
				{
					dy = d.Y > G0 ?  G1 : -G1;
					dx = d.X / GABS(d.Y);
					count = GABS(d.Y).Whole();
				}

				GINT	l = d.MagnitudeXY();
//				GVector	pos		= mPos + GVector(d.Y / l, -d.X / l, G0) * mDistance;
				GVector	pos		= mPos + (GVector(d.Y / l, -d.X / l, G0) * mDistance) / 3;

				for (c0 = 0; c0 < count; c0 ++)
				{
//					MAP.GetCell(CCellRef(pos - GVector(GINT_HALF, GINT_HALF, G0)))->AddOffsetVelocity(-force * (G1 - (GVector(G(0, (pos.X - GINT_HALF).Frac()), G(0, (pos.Y - GINT_HALF).Frac()), G0).MagnitudeXY())));
					if ((pos.X >= G1) && (pos.Y >= G1) &&
						(pos.X < G(MAP_WIDTH - 1)) &&
						(pos.Y < G(MAP_HEIGHT - 1)))
					{
						MAP.GetCell(CCellRef(pos - GVector(GINT_HALF, GINT_HALF, G0)))->AddOffsetVelocity(-force);
					}
					pos.X += dx;
					pos.Y += dy;
				}

				mDistance ++;

				mActive --;
			}
		}
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CEFLineWavePair2::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CFXLightning);
}
#endif

