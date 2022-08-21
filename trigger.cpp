#include	"Common.h"

#include	"Trigger.h"
#include	"Globals.h"
#include	"Switch.h"
#include	"Character.h"
#include	"Primary.h"

//******************************************************************************************
void	CTrigger::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	mTimer = 0;

	// read data
	mTarget = NULL;
	mType = 0;
	if (i->mTarget >= 0)
	{
		mType |= TRIG_THING;
		WORLD.GetThing(i->mTarget, &mTarget);
	}

	if ((i->mOID != OID_INVALID) && (i->mOID != OID_NONE))
	{
		mType |= TRIG_OID;
		mOID = i->mOID;
	}

	mSpawnee = NULL;

	SitOnGround();
}

//******************************************************************************************
#if TARGET == PC
void	CTrigger::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	if (mType & TRIG_OID)
		 i.mOID = mOID;
	else i.mOID = OID_NONE;

	WRITE_TARGET;
}
#endif

//******************************************************************************************
void	CTrigger::Process()
{
	SUPERTYPE::Process();
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CTrigger::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	if (mType & TRIG_OID)
		WORLD.AccumulateOIDResources(mOID);

}
#endif

//******************************************************************************************
void	CTrigger::Action()
{
	if (mTimer == 0)
	{
		// triggered!

		mTimer = GetTriggerTime();

		if (mType & TRIG_OID)
		{
			mSpawnee = SpawnThing(mOID);
			if (mSpawnee)
			{
				CInitThing i;
				i.mPos = mTarget->GetPos();
				mSpawnee->Init(&i);
			}
		}

		if (mType & TRIG_THING)
		{
			if (mTarget)
				mTarget->Trigger(mTarget);
		}

		TurnOn();
	}
}

//******************************************************************************************
void	CTrigger::UnAction()
{
	// finished!
	if (mType & TRIG_THING)
		if (mTarget)
			mTarget->UnTrigger(mTarget);

	if (mType & TRIG_OID)
		if (mSpawnee)
			mSpawnee->Die();

	TurnOff();
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

SINT	CTriggerTile::GetTileType(BOOL on)
{
	if (!on)
	{
		switch (mCounter1)
		{
		case 0:	return CT_TRIGGER_GUN;
		case 1: return CT_TRIGGER_BAD_GUN;
		case 2: return CT_TRIGGER_WAVE;
		}
	}
	else
	{
		switch (mCounter1)
		{
		case 0:	return CT_TRIGGER_GUN_ON;
		case 1: return CT_TRIGGER_GUN_ON;
		case 2: return CT_TRIGGER_WAVE_ON;
		}
	}

	return CT_TRIGGER_GUN;
}

//******************************************************************************************
void	CTriggerTile::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	// snap myself to grid.
	GVector p = GVector(G(((mPos.X / 4).Whole() * 4) + 2),
						G(((mPos.Y / 4).Whole() * 4) + 2),
						G0);

	Move(p);
	StickToGround();

	GET_COUNTER(0);		// time active

	GET_COUNTER(1);		// type
	if (mCounter1 < 0)
		mCounter1 = 0;
	if (mCounter1 > 2)
		mCounter1 = 2;

	// set the map cells to the required type
	SINT	c0, c1;
	SINT	tile = GetTileType(FALSE);

	for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
		for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
		{
			MAP.GetCell(CCellRef(c1, c0))->SetType(tile);
		}

	MAP.PrecalcCells(); //! really!!!!

	mFrame = 0;

}

//******************************************************************************************
#if TARGET == PC
void	CTriggerTile::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
	WRITE_COUNTER(1);
}
#endif

//******************************************************************************************
void	CTriggerTile::Shutdown()
{
	SINT c0, c1;

	for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
		for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
		{
			MAP.GetCell(CCellRef(c1, c0))->SetType(CT_NORMAL);
#if 0
			MAP.GetCell(CCellRef(c1, c0))->SetFrame(G0);
#endif
		}

	MAP.PrecalcCells(); //! really!!!!

	SUPERTYPE::Shutdown();
}


//******************************************************************************************
void	CTriggerTile::Hit(CThing *that)
{
	if (that->IsProjectile()) return ;
	Action();
}

//******************************************************************************************







//#define LIGHT_SPACING 0x71c7
#define LIGHT_SPACING 0x7200



void	CTriggerTile::Process()
{
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;

//Draw lights....
	CParticleSystem*ps=MAP.GetEffectParticleSystem();
  	SINT turn=GAME.GetGameTurn();
	SINT x,y;
	SINT col;

#define LIGHT_SPACING 0x7200
	if(GAME.CanIBeSeen(mPos))
	{
		if(mTimer!=0)col=0x008000;
		else
		{
			col=0x000080;
			turn>>=1;
		}


		for(int i=0;i<4;i++)
		{
			CParticle *p=ps->CreateParticle(PART_LIGHT);
			if(p)
			{
				switch ( ((turn>>3)&3))
				{
					case 0:
						x=(mPos.X.mVal-0x20000)+((turn&7)*LIGHT_SPACING);
						y=mPos.Y.mVal+0x20000;
						break;
					case 1:
						x=mPos.X.mVal+0x20000;
						y=(mPos.Y.mVal+0x20000)-((turn&7)*LIGHT_SPACING);
						break;
					case 2:
						x=(mPos.X.mVal+0x20000)-((turn&7)*LIGHT_SPACING);
						y=mPos.Y.mVal-0x20000;
						break;
					case 3:
						x=mPos.X.mVal-0x20000;
						y=(mPos.Y.mVal-0x20000)+((turn&7)*LIGHT_SPACING);
						break;
				}

				p->mPos.X.mVal=x;
				p->mPos.Y.mVal=y;
				p->mPos.Z.mVal=mPos.Z.mVal;
				p->mColour=col;
			}
	 	turn+=8;
		}
	}

//

	if (mTimer > 0)
	{
#if 0
		mFrame += 1;
		if (mFrame > 28)
			mFrame = 0;

		SINT	c0, c1;

		for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
			for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
			{
				MAP.GetCell(CCellRef(c1, c0))->SetFrame(G(mFrame));
			}
#endif
	}

	if (mTimer > 0)
	{
		mTimer --;

		if (mTimer == 0)
		{
			UnAction();
		}
	}


	SUPERTYPE::Process();
}

//******************************************************************************************
void	CTriggerTile::TurnOn()
{
	// set the map cells to the required type
	SINT	c0, c1;
	SINT	tile = GetTileType(TRUE);

	for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
		for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
		{
			MAP.GetCell(CCellRef(c1, c0))->SetType(tile);
		}

	SOUND.PlaySample(SOUND_PICKUP3, this);
}

//******************************************************************************************
void	CTriggerTile::TurnOff()
{
	// set the map cells to the required type
	SINT	c0, c1;
	SINT	tile = GetTileType(FALSE);


	for (c0 = mPos.Y.Whole() - 2; c0 < mPos.Y.Whole() + 2; c0++)
		for (c1 = mPos.X.Whole() - 2; c1 < mPos.X.Whole() + 2; c1++)
		{
			MAP.GetCell(CCellRef(c1, c0))->SetType(tile);
		}

	SOUND.PlaySample(SOUND_PICKUP3, this);
}

//******************************************************************************************
//******************************************************************************************
void	CTriggerDead::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mTriggered = FALSE;
}

//******************************************************************************************

void	CTriggerDead::Process()
{
	if (!mTriggered)
	{
		CCharacter	*c;
		if ((!mChainTrigger) ||
			(c = mChainTrigger->IsCharacter(), (c) ? c->GetState() == CS_DEAD : FALSE))
		{
			Action();
			mTriggered = TRUE;
		}
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
//******************************************************************************************
void	CTriggerSwitch::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mTriggered = FALSE;
}

//******************************************************************************************
void	CTriggerSwitch::Process()
{
	CSwitch		*s;

	if (mChainTrigger)
	{
		s = mChainTrigger->IsSwitch();

		if (s)
		{
			if (s->IsSwitched())
			{
				if (!mTriggered)
				{
					Action();
					mTriggered = TRUE;
				}
			}
			else
			{
				if (mTriggered)
				{
					UnAction();
					mTriggered = FALSE;
				}
			}
		}
	}

	SUPERTYPE::Process();
}



//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// LAP TRIGGER

//******************************************************************************************
void	CTriggerLap::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);
	GET_COUNTER(0);

	// do check
#if TARGET == PC
	if (mCounter0 <=1)
	{
		CONSOLE.AddString("Warning: Lap trigger not correct") ;
	}
#endif

	mTriggered = FALSE;
}

//******************************************************************************************
void	CTriggerLap::Process()
{
	if (!mTriggered)
	{
		SINT players = GAME.GetNumPlayers() ;
		for (SINT i=0;i<players;i++)
		{
			CPrimary	*p = PLAYER(i)->GetPrimary();
			if (p)
				if (p->GetLevelData().GetLapNumber() == (mCounter0 -1))
				{
					Action() ;
					mTriggered = TRUE ;
				}
		}
	}

	SUPERTYPE::Process();
}


//********************************************************************************************************
#if TARGET == PC
void	CTriggerLap::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
}
#endif


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// SCORE TRIGGER

//******************************************************************************************
void	CTriggerScore::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);
	GET_COUNTER(0);

	// do check
#if TARGET == PC
	if (mCounter0 <=0)
	{
		CONSOLE.AddString("Warning: Score trigger not correct") ;
	}
#endif

	mTriggered = FALSE;
}

//******************************************************************************************
void	CTriggerScore::Process()
{
	if (!mTriggered)
	{
		SINT players = GAME.GetNumPlayers() ;
		for (SINT i=0;i<players;i++)
		{
			CPrimary	*p = PLAYER(i)->GetPrimary();
			if (p)
				if (p->GetScore() >= (mCounter0 *100))
				{
					Action() ;
					mTriggered = TRUE ;
				}
		}
	}



	SUPERTYPE::Process();
}


//********************************************************************************************************
#if TARGET == PC
void	CTriggerScore::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
}
#endif


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// ONE MAN ON HILL TRIGGER

//******************************************************************************************
void	CTriggerOnePrimaryOnHill::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);
	mTriggered = FALSE;
	GET_COUNTER(0);		// time active
}

//******************************************************************************************
void	CTriggerOnePrimaryOnHill::Process()
{
	if (!mTriggered)
	{
		KingHillEntryList& chars_on_hill_list = GAME.GetGameLevelData().GetKingHillEntries() ;
		if (GAME.GetGameLevelData().NumCharactersOnHill() == 1 &&
			chars_on_hill_list.First()->GetCharacter()->IsPrimary() )
		{
			CONSOLE.AddString("here !!!!") ;
			Action() ;
			mTriggered = TRUE ;
		}
	}

	else if (mTimer > 0)
	{
		mTimer --;

		if (mTimer == 0)
		{
			UnAction();
			mTriggered = FALSE ;
		}
	}


	SUPERTYPE::Process();
}

//******************************************************************************************
#if TARGET == PC
void	CTriggerOnePrimaryOnHill::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
}
#endif
