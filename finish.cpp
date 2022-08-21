#include	"Common.h"

#include	"Globals.h"
#include	"Finish.h"
#include	"Primary.h"

#define		FINISH_PROXIMITY_A	G(5)
#define		FINISH_PROXIMITY_B	G(3)

//******************************************************************************************
void	CFinish::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();
	
	if (GAME.GetIVar(IV_LEVEL_TYPE) != LT_GOLF)
	{
		mPos.Z -= G(3); // float above ground
		mSize = FINISH_PROXIMITY_A;
	}
	else
	{
		mSize = FINISH_PROXIMITY_B;
	}

	WORLD.RegisterFinishPos(mPos);

	mInitialZ = mPos.Z;
	
}

//******************************************************************************************
void	CFinish::Process()
{
	// bounce!  (Should this be an actor??) 
	// ->probably for interpolated engine...

	if (GAME.GetIVar(IV_LEVEL_TYPE) != LT_GOLF)
		mPos.Z = mInitialZ + GSIN(G(GAME.GetGameTurn()) / 10) * GINT_HALF;

	// need different conditions here for multiple balls

	for (SINT i=0;i<GAME.GetNumPlayers();i++)
	{

		CPrimary	*pr = PLAYER(i)->GetPrimary();

		// check if player not already finished
		if (pr && !pr->GetLevelData().GetFinishedLevel() )
		{ 
			GVector p = pr->GetPos();;

			if ((p - mPos).Manhattan() < mSize)
			{
				if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF)
				{
					if (pr->GetVelocity().Magnitude() < G(0, 3000))
					{
						pr->FinishedLevel() ;
					}
				}
				else
				{
					pr->FinishedLevel() ;
				}
			}
		}
	}
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CFinishPin::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	SetAnimMode(AM_WALK, TRUE);
}

//******************************************************************************************
BOOL	CFinishPin::SetAnimMode(EAnimMode am, BOOL reset_frame)
{
	GINT	fi;

	fi = GetRenderThingFrameIncrement(am, &mRealAnimIndex);

	if (reset_frame)
		mFrame = G0;

	if(fi != G0)
	{
		mFrameInc = fi;
		mAnimMode = am;
		return		TRUE;
	}

	return FALSE;
}

//******************************************************************************************
void	CFinishPin::ProcessAnimation()
{

	if (mAnimMode == AM_NONE) return ;

	GINT	old_frame = mFrame;

	mFrame+=mFrameInc;

/*	if ((old_frame < GINT_HALF) && (mFrame >= GINT_HALF))
	{
		AnimTrigger();
	}*/


	if(mFrame>=G1)
	{
		mFrame -= G1;
	}
}
//******************************************************************************************
void	CFinishPin::Process()
{
	ProcessAnimation();

	SUPERTYPE::Process();
}

//******************************************************************************************
//******************************************************************************************
void	CFinishTrigger::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;
}


//******************************************************************************************
void	CFinishTrigger::Trigger(CThing *top)
{
	CPrimary	*pr = PLAYER(0)->GetPrimary();

	// check if player not already finished
	if (pr && !pr->GetLevelData().GetFinishedLevel() )
	{ 
		pr->FinishedLevel() ;
	}

	SUPERTYPE::Trigger(top);
}
