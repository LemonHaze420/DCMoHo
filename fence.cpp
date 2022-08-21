#include	"Common.h"

#include	"Fence.h"
#include	"Globals.h"
#include	"Character.h"
#include	"Primary.h"

//******************************************************************************************
void	CFencePost::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	GET_TARGET;

	mFenceSection = NULL;
	mFenceSectionC = NULL;
}

//******************************************************************************************
#if TARGET == PC
void	CFencePost::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
}	
#endif

//******************************************************************************************
void	CFencePost::Die()
{
	if (mFenceSection)
	{
		mFenceSection->Die();
		mFenceSection = NULL;
	}

	if (mFenceSectionC)
	{
		mFenceSectionC->Die();
		mFenceSectionC = NULL;
	}

	SUPERTYPE::Die();
}

//******************************************************************************************
void	CFencePost::Process()
{
	GINT b4Z = mPos.Z ;
	mPos.Z = MAP.GetMapHeightDifference(mPos);
	if (mPos.Z != b4Z && mFenceSection)
	{
		((CFenceSection*)mFenceSection)->MoveStartZVal(mPos.Z - b4Z) ;
	}
		
	if (IsValid())
	{
		if ((!mFenceSection) && mTarget)
		{
			mFenceSection = SpawnThing(OID_CFenceSection);

			if (mFenceSection)
			{
				CInitThing	i;

				i.mPos = mPos;
				i.mTarget = mTarget->GetNumber();

				mFenceSection->Init(&i);
			}
		}

		if ((!mFenceSectionC) && mChainTrigger)
		{
			mFenceSectionC = SpawnThing(OID_CFenceSection);

			if (mFenceSectionC)
			{
				CInitThing	i;

				i.mPos = mPos;
				i.mTarget = mChainTrigger->GetNumber();

				mFenceSectionC->Init(&i);
			}
		}

		if ((!mTarget) && (mFenceSection))
		{
			mFenceSection->Die();
			mFenceSection = NULL;
		}

		if ((!mChainTrigger) && (mFenceSectionC))
		{
			mFenceSectionC->Die();
			mFenceSectionC = NULL;
		}
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CFencePost::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CFenceSection);
}
#endif

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

#define FENCE_HEIGHT G(2,16123) 

//******************************************************************************************
void	CFenceSection::Init(CInitThing *i)
{
	mRadius = G1; // gotta do something....

	SUPERTYPE::Init(i);

	ASSERT(!(WORLD.IsReading()));  // we're buggered if it is....
	GVector	end;

	if (WORLD.IsReading())
		end = ZERO_GVECTOR;
	else
	{
		CThing	*t;
		WORLD.GetThing(i->mTarget, &t);
		if (t)
			end = t->GetPos() ;//+ t->GetRenderThingEmitter(0);
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
void	CFenceSection::SetStartAndEnd(GVector &start, GVector &end)
{
	GVector	avg = (start + end) / 2;

	Move(avg);
	mStart = start;
	mEnd = end;

	// clip to ground
	mStart.Z = MAP.GetMapHeight(mStart);
	mEnd.Z   = MAP.GetMapHeight(mEnd);

	mCollideShape = CSH_FENCE ;

	if (GABS(mStart.X - mEnd.X) < G(0,100) )
	{
		mCollideShape = CSH_FENCE_Y_ALLIGN ;
	}
	
	if (GABS(mStart.Y - mEnd.Y) < G(0,100) )
	{
		mCollideShape = CSH_FENCE_X_ALLIGN ;
	}

	// precalc normal for collision
	GVector d = mEnd - mStart;
	GINT	l = d.Magnitude();

	if (l < G(0, 100))
		mNormal = GVector(G0, G1, G0);  // erk...

	mNormal = (d ^ GVector(G0, G0, G1)) / l;

}



//******************************************************************************************
void	CFenceSection::Process()
{	
	mEnd.Z =  MAP.GetMapHeightDifference(mEnd);
	SUPERTYPE::Process() ;
}


//******************************************************************************************
void	CFenceSection::Hit(CThing *that)
{
	CPrimary *p = that->IsPrimary();
	
	if (p && p->GetState() != CS_KNOCKED_OUT)
	{
		that->Damage(G(0,2000)) ;
//		SOUND.PlaySample(1, this);
	}

}


//******************************************************************************************
BOOL	CFenceSection::AboveOrBelow(GVector& point)
{
	// hmmm this only works if fence is level 

	if ( point.Z > mStart.Z &&
		 point.Z > mEnd.Z ) return TRUE ;

	if ( point.Z < (mStart.Z - FENCE_HEIGHT) &&
		 point.Z < (mStart.Z - FENCE_HEIGHT) ) return TRUE ;

	return FALSE;

}