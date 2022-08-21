#include	"Common.h"

#include	"CheckPoint.h"
#include	"Globals.h"
#include	"Character.h"

//******************************************************************************************
void	CCheckPoint::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	SitOnGround();

	GET_TARGET;

	// get checkpoint number
	GET_COUNTER(0);
	GET_BOOL(0) ;

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

	mCheckPointSection= NULL;
	WORLD.GetCheckPointNB().Add(this) ;
}

//******************************************************************************************
#if TARGET == PC
void	CCheckPoint::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
	WRITE_COUNTER(0);
	WRITE_BOOL(0);
	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}	
#endif


//******************************************************************************************
void	CCheckPoint::Shutdown() 
{
	SUPERTYPE::Shutdown() ;
	WORLD.GetCheckPointNB().Remove(this) ;
}


//******************************************************************************************
void	CCheckPoint::Process()
{
	if (IsValid())
	{
		if ((!mCheckPointSection) && mTarget)
		{

			mCheckPointSection = SpawnThing(OID_CCheckPointSection);

#if TARGET==PC
			if (!mChainTrigger && GAME.IsMultiPlayer())
			{
				char c1[20] ;
				sprintf(c1, "CheckPoint %d has no Respawn spot !!", mCounter0) ;
				CONSOLE.AddString(c1) ;
			}
#endif
			if (mCheckPointSection)
			{
				CInitThing	i;

				i.mPos = mPos;
				i.mTarget = mTarget->GetNumber();

				// do check for easy mistakes to make in editor
#if TARGET == PC

				char temp[50] ;

				CCheckPoint* cp = mTarget->IsCheckPoint() ;
				if (!cp)
				{
					sprintf(temp,"WARNING: Checkpoint %d target is not a checkpoint", mCounter0) ;	
					CONSOLE.AddString(temp);
				}
				else
				{
					if (cp->GetCheckPointNumber() != mCounter0)
					{
						sprintf(temp,"WARNING: Checkpoint %d target is different number", mCounter0) ;	
						CONSOLE.AddString(temp);
					}
				}
#endif

				mCheckPointSection->Init(&i);
				((CCheckPointSection*)mCheckPointSection)->SetCheckPointNumber(mCounter0) ;
				((CCheckPointSection*)mCheckPointSection)->SetJump(!mBool0) ;
			}
		}
	}

	SUPERTYPE::Process();
}


//******************************************************************************************
void	CCheckPoint::Trigger(CThing *top)
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		mActivated = TRUE; break;
	case TR_DEACTIVATE:
		mActivated = FALSE; break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				mActivated = TRUE;
			}
			else
			{
				mActivated = FALSE;
			}
			break ;
		}
	};

	SUPERTYPE::Trigger(top);
}



//******************************************************************************************
void	CCheckPoint::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		mActivated = TRUE; break;
	case TR_DEACTIVATE:
		mActivated = FALSE; break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				mActivated = TRUE;
			}
			else
			{
				mActivated = FALSE;
			}
			break ;
		}
	};

	SUPERTYPE::UnTrigger(top);
}

//******************************************************************************************

CCheckPoint* CCheckPoint::GetACheckPointFromNumber(SINT num) 
{
	const CheckPointList& items = WORLD.GetCheckPointNB();
	CCheckPoint *nt ;

	// loop through all checkpoints in world
	for (nt = items.First() ;
		 nt != NULL ;
		 nt = items.Next() )
	{
		// is the checkpont one we are looking for ?
		if (nt->GetCheckPointNumber() == num && nt->GetTarget())
		{
			return nt ;
		}
	}
	ASSERT(0) ;
	return NULL;
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CCheckPoint::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CCheckPointSection);
}
#endif

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************


//******************************************************************************************
void	CCheckPointSection::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	ASSERT(!(WORLD.IsReading()));  // we're buggered if it is....
	GVector	end;

	if (WORLD.IsReading())
		end = ZERO_GVECTOR;
	else
	{
		CThing	*t;
		WORLD.GetThing(i->mTarget, &t);
		if (t)
			end = t->GetPos() ;
		else
			end = GVector(G(32), G(32), G0);
	}

	SetStartAndEnd(i->mPos, end);

	mRadius = (i->mPos - end).Magnitude() / 2;

	// changed the radius!
	MAP.RemoveFromMapWhoSizeUnknown(this);
	MAP.AddToMapWho(this);
}

//******************************************************************************************
void	CCheckPointSection::SetStartAndEnd(GVector &start, GVector &end)
{
	GVector	avg = (start + end) / 2;

	Move(avg);
	mStart = start;
	mEnd = end;

	// clip to ground
	mStart.Z = MAP.GetMapHeight(mStart);
	mEnd.Z   = MAP.GetMapHeight(mEnd);

	// precalc normal for collision
	GVector d = mEnd - mStart;
	GINT	l = d.Magnitude();

	if (l < G(0, 100))
		mNormal = GVector(G0, G1, G0);  // erk...

	mNormal = (d ^ GVector(G0, G0, G1)) / l;
}

//******************************************************************************************
void	CCheckPointSection::Hit(CThing *that)
{
	CCharacter *c = that->IsCharacter();
	
	if (c)
	{
		c->ReachedCheckPoint(mCheckPointNum) ;

		// make racer jump

		if (mJump && (c->IsRacer() || c->IsBadGuy() ) ) 
		{
			c->Jump() ;
		}
	}
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CRacerReSpawner::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;
	mYaw = i->mYaw;
	SitOnGround();
}