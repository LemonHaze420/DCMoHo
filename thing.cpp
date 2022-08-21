#include	"Common.h"
#include	"Globals.h"
#include	"Thing.h"
#include	"Editors.h" //!
#include	"Character.h"

#if TARGET == PC
#include	"PCRenderData.h"
#endif

//******************************************************************************************
CInitThing::CInitThing()
{
	// if this is changed - increase version number in load/save
	SINT	c0;

	mPos				= ZERO_GVECTOR;
	mVelocity			= ZERO_GVECTOR;
	mAngleType			= AT_YPR;
	mOrientation		= ID_GMATRIX;
	mYaw				= G0;
	mPitch				= G0;
	mRoll				= G0;
	mOID				= OID_INVALID;
	mTarget				= -2;  // -1 = NONE, -2 = INVALID
	mChainTrigger		= -2;  //   ditto

	mInitialMode		= IM_UNUSED;
	mTriggerResponse	= TR_UNUSED;
	mUnTriggerResponse	= TR_UNUSED;

	for (c0 = 0; c0 < NUM_COUNTERS; c0 ++)
		mCounters[c0]	= -1;  // -1 = INFINITY

	for (c0 = 0; c0 < NUM_BOOLS; c0 ++)
		mBools[c0]	= TRUE; 

	mMeshNo				= -2;
}

//******************************************************************************************
CThing::~CThing()
{
}

//******************************************************************************************
//#if 0  SDS
#if TARGET != PC
void	*CThing::operator	new(size_t size)
{
	return PLATFORM.AllocateThingMemory(size);
}


//******************************************************************************************
void	CThing::operator	delete(void *mem)
{
	PLATFORM.FreeThingMemory(mem);
}
#endif

//******************************************************************************************
void	CThing::Shutdown()
{
	MAP.RemoveFromMapWhoSizeUnknown(this);
	WORLD.RemoveThing(this);

#if TARGET == PC
	delete mRenderThing;
	mRenderThing = NULL;
#elif TARGET == PSX
#endif
}

//******************************************************************************************
void	CThing::Init(CInitThing *i)
{
	mMapWhoNext = NULL;
	mNextThing = NULL;
	mPrevThing = NULL;
#if TARGET == PC
	mRenderThing = NULL;
#elif TARGET == PSX || TARGET == DC
#endif

	ASSERT(GABS(i->mPos.X) < G(1000));
	ASSERT(GABS(i->mPos.Y) < G(1000));
	ASSERT(GABS(i->mPos.Z) < G(1000));

	mPos = i->mPos;

	switch (i->mAngleType)
	{
	case AT_ORI:
		mOrientation = i->mOrientation;
		break;

	case AT_YPR:
		mOrientation.MakeRotation(i->mYaw, i->mPitch, i->mRoll);
		break;
	};

#ifdef LINK_EDITOR
	mEditorYaw = i->mYaw;
	mEditorPitch = i->mPitch;
	mEditorRoll = i->mRoll;
#endif

	mFlags = 0;
	mLife = InitialLife();
	
	WORLD.GetThing(i->mChainTrigger, &mChainTrigger);

	InitRenderThing();

	WORLD.AddThing(this);
	MAP.AddToMapWho(this);
}

//******************************************************************************************
void	CThing::InitRenderThing()
{
#if TARGET == PC
	mRenderThing = CreateAndGetRenderThing(_GetClassID(), this);
#endif
}

//******************************************************************************************
void	CThing::Suicide()
{
	mFlags |= TF_SUICIDE;

#ifdef _DEBUG
	mFlags |= TF_DEBUG_SUICIDE;
#endif
}

//******************************************************************************************
void	CThing::SuicideNext()
{
	// use when not necessarily in Process() function

	mFlags |= TF_SUICIDE_NEXT;
}

//******************************************************************************************
#if TARGET == PC
void	CThing::WriteData(CInitThing &i)
{
	i.mPos = mPos;

#ifdef LINK_EDITOR
	i.mYaw	 = mEditorYaw;
	i.mPitch = mEditorPitch;
	i.mRoll  = mEditorRoll;
#else
	i.mYaw = i.mPitch = i.mRoll = G0;
#endif
	
	if (mChainTrigger)
	{
		i.mChainTrigger = WORLD.GetWriteRef(mChainTrigger);
	}
	else
	{
		i.mChainTrigger = -1;
	}
}
#endif
//******************************************************************************************

void	CThing::Process()
{
	// am I dead?
	if ((mLife <= 0) && (!(mFlags & TF_DEAD)))
	{
		// yes
		mFlags |= TF_DEAD;

		CCharacter* me = IsCharacter() ;
		if (me)
		{
			me->StartDieProcess() ;
		}
		else
		{
			Die(); 
		}
	}

	if (mFlags & TF_SUICIDE_NEXT)
		mFlags |= TF_SUICIDE; // kill me next frame

#ifdef _DEBUG
	mFlags &= ~TF_DEBUG_SUICIDE;
#endif
}

//******************************************************************************************
void	CThing::Render()
{
#if TARGET == PC
	if (mRenderThing)
	{
#ifdef LINK_EDITOR
		if ((EDITOR.IsActive()) || (!(mFlags & TF_RENDER_IN_EDITOR_ONLY)))
#else
		if (!(mFlags & TF_RENDER_IN_EDITOR_ONLY))
#endif
			if  ((!(mFlags & TF_SUICIDE)) &&
				 (!(mFlags & TF_SUICIDE_NEXT)))
			{
				if (((!(mFlags & TF_FLASHING)) || (GAME.GetGameTurn() & 1)) &&
					((!(mFlags & TF_SLOW_FLASHING)) || ((GAME.GetGameTurn() & 2) ==0)))
				{
					SINT	c0;
					SINT	v = WhichRTsToRender();

					for (c0 = 0; c0 < MAX_RENDER_THINGS_PER_THING; c0 ++)
					{
						if (v & (1 << c0))
						{
							// chain down to get correct render thing
							CRenderThing *rt = mRenderThing->GetChainedRenderThing(c0);
							rt->Render();
						}
					}
				}
			}
	}
#endif
}

void	CThing::RenderCollisionVolume(SINT i)
{
#if TARGET == PC
	CRenderThing *rt = mRenderThing;
//	CRenderThing *rt = mRenderThing->GetChainedRenderThing(GetRenderThingNumber());
	rt->RenderCollisionVolume(i);
#endif
}

//******************************************************************************************
GINT	CThing::GetRenderTurn()
{
	return G(GAME.GetGameTurn()); //!!
}

//******************************************************************************************
void	CThing::StickToGround()
{
	mPos.Z = MAP.GetMapHeight(mPos);
}

//******************************************************************************************
void	CThing::SitOnGround()
{
	mPos.Z = MAP.GetMapHeight(mPos) - Radius();
}

//******************************************************************************************
void	CThing::ClipToGround()
{
	GINT h = MAP.GetMapHeight(mPos) - Radius();

	if (h < mPos.Z) mPos.Z = h;
}

//******************************************************************************************
void	CThing::Move(GVector dest)
{
	ASSERT(GABS(dest.X) < G(1000));
	ASSERT(GABS(dest.Y) < G(1000));
	ASSERT(GABS(dest.Z) < G(1000));

	if (mFlags & TF_IN_MAPWHO)
		MAP.MoveMapWho(this, dest);  // changes mpos
	else
		mPos = dest;
}

//******************************************************************************************
CCollisionVolume	*CThing::GetComplexCollisionVolume()
{
#if TARGET == PC
	if (mRenderThing)
		return mRenderThing->GetCollisionVolume();
	else
		return NULL;
#elif TARGET == PSX || TARGET == DC
	return ENGINE.GetCollisionVolume(this);
//	return NULL;
#endif
};

//******************************************************************************************
GINT	CThing::GetRenderThingRadius()
{
#if TARGET == PC
	return mRenderThing ? mRenderThing->GetRTRadius() : CThing::Radius();
#elif TARGET == PSX || TARGET == DC
//	return G1; //!PSX return radius here
	return ENGINE.GetRenderThingRadius(this);
#endif
};

//******************************************************************************************
GVector	CThing::GetRenderThingEmitter(SINT no)
{
#if TARGET == PC
	return mRenderThing ? mRenderThing->GetRTEmitter(no) : ZERO_GVECTOR;
#elif TARGET == PSX || TARGET == DC
	return ENGINE.GetEmitter(this, no);
#endif
};

//******************************************************************************************
GVector	CThing::GetRenderThingEmitterAnimated(SINT no)
{
#if TARGET == PC
	return mRenderThing ? mRenderThing->GetRTEmitterAnimated(no) : ZERO_GVECTOR;
#elif TARGET == PSX || TARGET == DC
	return ENGINE.GetEmitterAnimated(this, no);
//	return ZERO_GVECTOR;
#endif
};

//******************************************************************************************
GINT	CThing::GetRenderThingFrameIncrement(EAnimMode am, SINT* realindex)
{
#if TARGET == PC
	GINT	ret;
	if(mRenderThing) ret = mRenderThing->GetFrameIncrement(am, realindex);
	else ret = G0;
	return ret;
#elif TARGET == PSX || TARGET == DC
	return ENGINE.GetFrameIncrement(_GetClassID(), am, realindex);
#endif
}
