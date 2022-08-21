#include	"Common.h"

#include	"Globals.h"
#include	"FlameThrower.h"
#include	"Effect.h"

//******************************************************************************************
void	CFlameThrower::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();
	mFlamer	= NULL;
	
	if (i->mAngleType == AT_YPR)
		mYaw = i->mYaw;
	else		
		mYaw = G0;

	mOrientation2.MakeRotationYaw(mYaw);

	mTimer	= 0;
	mStartBurstNumber = mNumber * 43 ;
	mOrientation = ID_GMATRIX;
	mMoving = FALSE ;

	//!
//	CreateFlamer();

	GET_COUNTER(0);
	GET_COUNTER(1);
	GET_COUNTER(2);
	GET_BOOL(0);
	GET_BOOL(1);
	GET_BOOL(2);
	
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
}

//********************************************************************************************************
#if TARGET == PC
void	CFlameThrower::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
	WRITE_COUNTER(1);
	WRITE_COUNTER(2);
	WRITE_BOOL(0);
	WRITE_BOOL(1);
	WRITE_BOOL(2);
	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}
#endif


//******************************************************************************************
void	CFlameThrower::ActivateFlameThrower() 
{
	if (mActivated == FALSE)
	{
		mActivated= TRUE;
		// make the turret fire now !! 
		mStartBurstNumber = mCounter0 - ((GAME.GetGameTurn() /3) % mCounter0);
		if (!mFlamer)
		{
			CreateFlamer() ;
		}
	}
}


//******************************************************************************************
void	CFlameThrower::DeActivateFlameThrower() 
{
	if (mActivated == TRUE)
	{
		mActivated= FALSE;
		if (mFlamer)
		{
			mFlamer->Die();
			mFlamer = NULL;
		}
	}
}


//******************************************************************************************
void	CFlameThrower::Trigger(CThing *top)
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateFlameThrower(); break;
	case TR_DEACTIVATE:
		DeActivateFlameThrower(); break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateFlameThrower();
			}
			else
			{
				DeActivateFlameThrower();
			}
			break ;
		}
	};

	SUPERTYPE::Trigger(top);
}



//******************************************************************************************
void	CFlameThrower::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateFlameThrower(); break;
	case TR_DEACTIVATE:
		DeActivateFlameThrower(); break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateFlameThrower();
			}
			else
			{
				DeActivateFlameThrower();
			}
			break ;
		}
	};

	SUPERTYPE::UnTrigger(top);
}
//****************************************************************************
void	CFlameThrower::CreateFlamer()
{
	if (mFlamer)
		return;

	mFlamer = SpawnThing(OID_CFXFlamer);

	if (mFlamer)
	{
		CInitThing	i;

//		i.mPos = mPos + mOrientation * GetRenderThingEmitter(0);
		i.mPos = mPos + GetRenderThingEmitter(0);
		i.mOrientation = mOrientation2;
		i.mAngleType = AT_ORI;
		mFlamer->Init(&i);
	}
}

//******************************************************************************************
void	CFlameThrower::Aim()
{
	GVector d = (WORLD.GetPrimaryPosition() - mPos);
	GINT	v = d.MagnitudeXY();

	// do yaw ;

	GINT RequiredYaw = GATAN2(d.Y, d.X) - G_PI_2;

	if (GABS(AngleDifference(mYaw, RequiredYaw)) < G(0,3000) )
	{
		mYaw = RequiredYaw;
	}
	else
	{
		// go left or right ?
		GVector otho1 = mOrientation2 * GVector(G1,G0,G0)  ;
		GINT dot = d * otho1 ;

		if (dot >=0)
		{
			mYaw-=G(0,3000) ;
			if (mYaw < -G_PI) mYaw+=(G_PI*2); 
		}
		else
		{
			mYaw+=G(0,3000) ;
			if (mYaw > G_PI) mYaw-=(G_PI*2); 
		}
	}

	mOrientation2.MakeRotation(mYaw, G0, G0);

}


//******************************************************************************************
void	CFlameThrower::Process()
{
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;
	if ((mActivated) && (mCounter0 != -1))
	{
		// Bursty
		SINT	num = (GAME.GetGameTurn() /3)+ mStartBurstNumber;

		if ((mCounter0 == -1) ||
			((num % mCounter0) < mCounter1))
		{
			GVector d = (WORLD.GetPrimaryPosition() - mPos);
			if (d.MagnitudeSq() < G(4000))
				CreateFlamer();
			else
			{
				if (mFlamer)
				{
					mFlamer->Die();
					mFlamer = NULL;
				}
			}
		}
		else
		{
			if (mFlamer)
			{
				mFlamer->Die();
				mFlamer = NULL;
			}
		}

/*		mTimer ++;
		if (mTimer == mCounter0)
		{
			if (mFlamer)
			{
				mFlamer->Die();
				mFlamer = NULL;
			}
			else
				CreateFlamer();

			mTimer = 0;
		}*/
	}

	if (!mBool2)
	{
		if (mBool0) // spin?
		{
			mYaw += mBool1 ? -G(0, 3000) : G(0, 3000);   // direction
			mOrientation2.MakeRotationYaw(mYaw);
			mMoving = TRUE ;
		}
	}
	else
	{
		Aim() ;
	}


	// move flamer?
	if (mFlamer)
	{
//		mFlamer->Move(mPos + mOrientation2 * GetRenderThingEmitter(0) + (mOrientation2 * GVector(G0, G(2), -G(2)))); //!!!BODGE
		mFlamer->Move(mPos + GetRenderThingEmitter(0));
		mFlamer->SetOrientation(mOrientation2);
	}

	SUPERTYPE::Process();
}


//******************************************************************************************
#ifdef LINK_EDITOR

void	CFlameThrower::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CFXFlamer);
}

#endif

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************


void	CFlameThrowerGround::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();
	mFlamer	= NULL;
	
	mTimer	= 0;
	//!
	mFlags |= TF_RENDER_IN_EDITOR_ONLY;
	mStartBurstNumber = mNumber * 43 ;
	GET_COUNTER(0);
	GET_COUNTER(1);
		
	GET_INITIAL_MODE;
	if (mInitialMode == IM_INACTIVE)
		mActivated = FALSE;
	else
	{
		mActivated = TRUE;
		ActivateFlameThrowerGround() ;
	}

	GET_TRIGGER_RESPONSE;
	if (mTriggerResponse == TR_UNUSED)
		mTriggerResponse = TR_ACTIVATE;
	GET_UNTRIGGER_RESPONSE;
	if (mUnTriggerResponse == TR_UNUSED)
		mUnTriggerResponse = TR_NONE;
}

//********************************************************************************************************
#if TARGET == PC
void	CFlameThrowerGround::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
	WRITE_COUNTER(1);
	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}
#endif

//****************************************************************************
void	CFlameThrowerGround::CreateFlamer()
{
	mFlamer = SpawnThing(OID_CFXFlamer);

	if (mFlamer)
	{
		CInitThing	i;

		i.mPos = mPos ;//+ GVector((G(0, RANDOM() % 32768) - G(0, 16384)) * G(4),
						//	    (G(0, RANDOM() % 32768) - G(0, 16384)) * G(4),
						//		G0);
		i.mOrientation.MakeRotationPitch(-G_PI_2);
		i.mAngleType = AT_ORI;
		mFlamer->Init(&i);
		((CFXFlamer*)mFlamer)->SetXRandomStartOffset(G1) ;
		((CFXFlamer*)mFlamer)->SetYRandomStartOffset(G1) ;
	}
}

//******************************************************************************************
void	CFlameThrowerGround::ActivateFlameThrowerGround() 
{
	if (mActivated == FALSE)
	{
		mActivated= TRUE;
		// make the turret fire now !! 
		mStartBurstNumber = mCounter0 - ((GAME.GetGameTurn() /3) % mCounter0);
		if (!mFlamer)
		{
			CreateFlamer() ;
		}
	}
}


//******************************************************************************************
void	CFlameThrowerGround::DeActivateFlameThrowerGround() 
{
	if (mActivated == TRUE)
	{
		mActivated= FALSE;
		if (mFlamer)
		{
			mFlamer->Die();
			mFlamer = NULL;
		}
	}
}


//******************************************************************************************
void	CFlameThrowerGround::Trigger(CThing *top)
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateFlameThrowerGround(); break;
	case TR_DEACTIVATE:
		DeActivateFlameThrowerGround(); break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateFlameThrowerGround();
			}
			else
			{
				DeActivateFlameThrowerGround();
			}
			break ;
		}
	};


	SUPERTYPE::Trigger(top);
}



//******************************************************************************************
void	CFlameThrowerGround::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateFlameThrowerGround(); break;
	case TR_DEACTIVATE:
		DeActivateFlameThrowerGround(); break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateFlameThrowerGround();
			}
			else
			{
				DeActivateFlameThrowerGround();
			}
			break ;
		}
	};
	

	SUPERTYPE::UnTrigger(top);
}
//******************************************************************************************
void	CFlameThrowerGround::Process()
{
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;
	if ((mActivated) && (mCounter0 != -1))
	{
		// Bursty
		SINT	num = (GAME.GetGameTurn() /3)+ mStartBurstNumber;

		if ((num % mCounter0) == 0 && !mFlamer)
		{
			CreateFlamer();
		}
		else
		{
			if (num% mCounter0 > mCounter1 )
			{
				
				if (mFlamer)
				{
					mFlamer->Die();
					mFlamer = NULL;
				}
			}
		}
	}
	


	// move flamer?
//	if (mFlamer)
//		mFlamer->Move(mPos);

	SUPERTYPE::Process();
}



//******************************************************************************************
#ifdef LINK_EDITOR

void	CFlameThrowerGround::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CFXFlamer);
}

#endif

