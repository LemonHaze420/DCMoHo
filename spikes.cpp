#include	"Common.h"
#include	"Spikes.h"
#include	"Map.h"
#include	"Editors.h"
#include	"globals.h"
#include	"Character.h"
#include	"Charball.h"

#include	"PSXBuildType.h"

//******************************************************************************************
void	CSpikes::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	GINT	yaw1 = G(RANDOM()%(1 << (GINT_SHIFT - 1)));
	GMatrix m1 ;
	m1.MakeRotationYaw(yaw1) ;
	GINT	pitch,yaw ;
	GVector map_normal = MAP.GetMapNormal(mPos) ;
	GINT	v = map_normal.MagnitudeXY();
	yaw = GATAN2(map_normal.Y, map_normal.X) - G_PI_2;
	pitch = GATAN2(v, -map_normal.Z) ;
	GMatrix m2 ;
	m2.MakeRotation(yaw, pitch, G0);
	mOrientation = m2 * m1 ;
}

//******************************************************************************************
void	CSpikes::Process()
{
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;
	SUPERTYPE::Process();
}

//******************************************************************************************
void	CSpikes::Hit(CThing *that)
{
	CCharacter* c = that->IsCharacter() ;
	if (c)
	{
		if ((c->GetLastSpikeHitTime() < (GAME.GetGameTurn() - 25 )  ) ) // SRG can't remember why this is here &&(((CCharBall*)c)->GetCollideShape()==CSH_DOUBLE_SPHERE))
		{
			c->Damage(G1);
			c->SetLastSpikeHitTime(GAME.GetGameTurn() ) ;
			c->Stun(4) ;

			// sorry for this hack ;(
			((CCharBall*)c)->SetSpinVelocity(G0) ;

			// Now show lightning effect on character that hit the spikes.... DAMO!
#ifndef DEMO_GAME
			if (c->GetCollideShape() == CSH_DOUBLE_SPHERE)
			{
				CParticleSystem*ps;
				if((ps=c->GetParticleSystem())!=NULL)
				{
					CParticle *p=ps->CreateParticle(PART_LIGHTNING);
					if(p)
					{
						p->mLife=20;
						p->uPRing.mBrightness=16384;
						p->mColour=0xffffff;
						p->mPos.X.mVal=
						p->mPos.Y.mVal=
						p->mPos.Z.mVal=0;
						p->mIsRelative=1;
					}
				}
			}
#endif
			// Sample
			SOUND.PlaySample(SOUND_BUZZ, this, GINT_HALF, TRUE, TRUE);
			SOUND.FadeTo(SOUND_BUZZ, G0, G(0, 3000), this);  // fade away straight away
		}
	}
	else
	{
		that->Damage(G1) ;
	}
}

//******************************************************************************************
//******************************************************************************************

#if 0

void	CSpikyDeath::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();
}

//******************************************************************************************
void	CSpikyDeath::Process()
{
	StickToGround();

	SUPERTYPE::Process();
}

//******************************************************************************************
void	CSpikyDeath::Hit(CThing *that)
{
	that->Damage(G(10));
}

#endif

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

//******************************************************************************************
void	CStinger::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	GINT	pitch,yaw ;
	GVector map_normal = MAP.GetMapNormal(mPos) ;
	GINT	v = map_normal.MagnitudeXY();

	yaw = GATAN2(map_normal.Y, map_normal.X) - G_PI_2;
	pitch = GATAN2(v, -map_normal.Z) ;
	mOrientation.MakeRotationYRP(yaw, G0, pitch);

	mStartBurstNumber = 0 ;
	GET_COUNTER(0);
	GET_COUNTER(1);
	GET_COUNTER(2);
	if (mCounter2 == -1 )
	{
		mStartBurstNumber = 0 ;
	}
	else
	{
		mStartBurstNumber = mCounter2 ;
	}

	GET_INITIAL_MODE;
	if (mInitialMode == IM_INACTIVE)
		mActivated = FALSE ;
	else
	{
		mActivated = TRUE ;
	}

	GET_TRIGGER_RESPONSE;
	if (mTriggerResponse == TR_UNUSED)
		mTriggerResponse = TR_ACTIVATE;
	GET_UNTRIGGER_RESPONSE;
	if (mUnTriggerResponse == TR_UNUSED)
		mUnTriggerResponse = TR_NONE;
	mSpikeAnim = 0 ;
	mSpikeAnimCount = 0 ;
	mStingerMovement = STINGER_NOT_MOVING ;

#if TARGET == PSX
	MAP.SetOtherFlags(CCellRef(mPos), F_4x4_ALWAYS4);
#endif
}

//********************************************************************************************************
#if TARGET == PC
void	CStinger::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
	WRITE_COUNTER(1);
	WRITE_COUNTER(2);
	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}
#endif

//******************************************************************************************
void	CStinger::Hit(CThing *that)
{
	if (mSpikeAnim != 4) return ;
	CCharacter* c = that->IsCharacter() ;
	if (c)
	{
		if ((c->GetLastSpikeHitTime() < (GAME.GetGameTurn() - 25 )  ))
		{
			c->Damage(G1);
			c->SetLastSpikeHitTime(GAME.GetGameTurn() ) ;
			c->Stun(4) ;
			// sorry for this hack ;(
			((CCharBall*)c)->SetSpinVelocity(G0) ;
			// Now show lightning effect on character that hit the spikes.... DAMO!
#ifndef DEMO_GAME
			if (c->GetCollideShape() == CSH_DOUBLE_SPHERE)
			{
				CParticleSystem*ps;
				if((ps=c->GetParticleSystem())!=NULL)
				{
					CParticle *p=ps->CreateParticle(PART_LIGHTNING);
					if(p)
					{
						p->mLife=20;
						p->uPRing.mBrightness=16384;
						p->mColour=0xffffff;
						p->mPos.X.mVal=
						p->mPos.Y.mVal=
						p->mPos.Z.mVal=0;
						p->mIsRelative=1;
					}
				}
			}
#endif
			// Sample
			SOUND.PlaySample(SOUND_BUZZ, this, GINT_HALF, TRUE, TRUE);
			SOUND.FadeTo(SOUND_BUZZ, G0, G(0, 3000), this);  // fade away straight away
		}
	}
	else
	{
		that->Damage(G1) ;
	}
}


//******************************************************************************************
void	CStinger::MoveStingerUp()
{
	if (mSpikeAnim != 0) return ;
	mSpikeAnim = 1 ;
	mSpikeAnimCount = 0 ;
	mStingerMovement = STINGER_MOVING_UP ;

}


//******************************************************************************************
void	CStinger::MoveStingerDown()
{
	if (mSpikeAnim != 4) return ;
	mSpikeAnim = 2 ;
	mSpikeAnimCount = 0 ;
	mStingerMovement = STINGER_MOVING_DOWN ;

}


//******************************************************************************************
void	CStinger::ActivateStinger()
{
	if (mActivated == FALSE)
	{
		mActivated= TRUE;
		mStartBurstNumber = mCounter0 - ((GAME.GetGameTurn() /3) % mCounter0);
	}
}


//******************************************************************************************
void	CStinger::DeActivateStinger()
{
	if (mActivated == TRUE)
	{
		mActivated= FALSE;
	}
}


//******************************************************************************************
void	CStinger::Trigger(CThing *top)
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateStinger(); break;
	case TR_DEACTIVATE:
		DeActivateStinger(); break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateStinger() ;
			}
			else
			{
				DeActivateStinger();
			}
			break ;
		}
	};


	SUPERTYPE::Trigger(top);
}



//******************************************************************************************
void	CStinger::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateStinger(); break;
	case TR_DEACTIVATE:
		DeActivateStinger(); break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateStinger();
			}
			else
			{
				DeActivateStinger();
			}
			break ;
		}
	};


	SUPERTYPE::UnTrigger(top);
}
//******************************************************************************************
void	CStinger::Process()
{
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;
	SUPERTYPE::Process();

	if ((mActivated) && (mCounter0 != -1))
	{

		// Bursty
		SINT	num = (GAME.GetGameTurn() /3)+ mStartBurstNumber;

		if ((num % mCounter0) == 0  &&  mSpikeAnim == 0 && mStingerMovement == STINGER_NOT_MOVING)
		{
			//CONSOLE.AddString("move spike up") ;
			MoveStingerUp();
			return ;
		}
		else
		{
			if (num% mCounter0 > mCounter1 && mSpikeAnim == 4 && mStingerMovement == STINGER_NOT_MOVING)
			{
				//CONSOLE.AddString("move spike down") ;
				MoveStingerDown() ;
				return ;
			}
		}

		if (mStingerMovement == STINGER_NOT_MOVING) return ;
		mSpikeAnimCount++ ;
		if (mSpikeAnimCount == 1)
		{
			if (mStingerMovement == STINGER_MOVING_UP)
			{
				if (mSpikeAnim == 0)
				{
					mSpikeAnim = 1 ;
				}
				else
				{
					mSpikeAnim = mSpikeAnim << 1 ;
				}
				if (mSpikeAnim == 4) mStingerMovement = STINGER_NOT_MOVING ;
			}
			else
			{
				mSpikeAnim = mSpikeAnim >> 1 ;
				if (mSpikeAnim == 0) mStingerMovement = STINGER_NOT_MOVING ;
			}
			mSpikeAnimCount = 0 ;
		}
	}

}


//******************************************************************************************
SINT	CStinger::WhichRTsToRender()
{
	#ifdef LINK_EDITOR
		if (EDITOR.IsActive()) return 4 ;
	#endif

	return mSpikeAnim ;
}
