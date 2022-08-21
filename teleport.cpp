#include	"Common.h"

#include	"Teleport.h"
#include	"Globals.h"
#include	"Character.h"

//******************************************************************************************
void	CTeleport::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	SitOnGround();

	GET_TARGET;

	mEffect = NULL;

	GET_INITIAL_MODE;
	if (mInitialMode == IM_INACTIVE)
		mActivated = FALSE;
	else
	{
		mActivated = TRUE;
		SwitchOnTeleportFlame() ;
	}


	GET_TRIGGER_RESPONSE;
	if (mTriggerResponse == TR_UNUSED)
		mTriggerResponse = TR_ACTIVATE;
	GET_UNTRIGGER_RESPONSE;
	if (mUnTriggerResponse == TR_UNUSED)
		mUnTriggerResponse = TR_NONE;
}

//******************************************************************************************
void	CTeleport::SwitchOnTeleportFlame()
{
	if (mEffect)
		SwitchOffTeleportFlame();

	mEffect = SpawnThing(OID_CFXFlamer3);
	if(mEffect)
	{
		CInitThing i;
		
		i.mPos = mPos + GetRenderThingEmitter(0);
		i.mOrientation.MakeRotationPitch(G_PI_2);
		i.mAngleType = AT_ORI;

		mEffect->Init(&i);
	}
}

//******************************************************************************************
void	CTeleport::SwitchOffTeleportFlame()
{
	if (mEffect)
	{
		mEffect->Die();
		mEffect = NULL;
	}
}



//******************************************************************************************
#ifdef LINK_EDITOR

void	CTeleport::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CFXFlamer3);
}
#endif

//******************************************************************************************
void	CTeleport::Hit(CThing *that)
{
	if (!mActivated) return ;
	CCharacter *c = that->IsCharacter();

	if (c && mTarget)
	{
		CStateInit i;
		i.mDest  = mTarget->GetPos();
		i.mTimer = 50;
		i.mStart = mPos;
		i.mStart.Z = MAP.GetMapHeight(mPos);
		c->ChangeState(CS_TELEPORT, i);
	}
}

//******************************************************************************************
void	CTeleport::Process()
{
	SitOnGround();

	SUPERTYPE::Process();
}

//******************************************************************************************
void	CTeleport::Trigger(CThing *top)
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		{
			if (mActivated == FALSE)
			{
				mActivated = TRUE;
				SwitchOnTeleportFlame() ;
				break;
			}
		}

	case TR_DEACTIVATE:
				if (mActivated == TRUE)
			{
				mActivated = FALSE;
				SwitchOffTeleportFlame() ;
				break;
			}
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				mActivated= TRUE;
				SwitchOnTeleportFlame() ;
			}
			else
			{
				mActivated = FALSE;
				SwitchOffTeleportFlame() ;
			}
			break ;
		}
	};

	SUPERTYPE::Trigger(top);
}



//******************************************************************************************
void	CTeleport::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		{
			if (mActivated == FALSE)
			{
				mActivated = TRUE;
				SwitchOnTeleportFlame() ;
				break;
			}
		}

	case TR_DEACTIVATE:
				if (mActivated == TRUE)
			{
				mActivated = FALSE;
				SwitchOffTeleportFlame() ;
				break;
			}
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				mActivated= TRUE;
				SwitchOnTeleportFlame() ;
			}
			else
			{
				mActivated = FALSE;
				SwitchOffTeleportFlame() ;
			}
			break ;
		}
	};

	SUPERTYPE::UnTrigger(top);
}

//******************************************************************************************
#if TARGET == PC
void	CTeleport::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}
#endif

