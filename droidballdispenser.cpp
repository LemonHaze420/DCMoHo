#include "common.h"
#include "DroidBallDispenser.h"
#include "globals.h"
#include "DroidBall.h"
#include "Grenade.h"

//******************************************************************************************
void	CBallDispenser::Init(CInitThing *i)
{

	SUPERTYPE::Init(i) ;
	//StickToGround();
	GET_COUNTER(0);
	mNextTurnToDepense = 50 ;
	mAnimMode = AM_NONE;
	mRealAnimIndex = 0;
	mFrame = 0;
	mFrameInc = 0;
}

//********************************************************************************************************
#if TARGET == PC
void	CBallDispenser::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
}
#endif



//******************************************************************************************
void		CBallDispenser::StartNextBallTimer()
{
	// this function is called when the last despensed ball gets picked up

	mNextTurnToDepense = GAME.GetGameTurn() +( mCounter0*25) ;
}



//******************************************************************************************
BOOL	CBallDispenser::SetAnimMode(EAnimMode am, BOOL reset_frame)
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
void	CBallDispenser::ProcessAnimation()
{

	if (mAnimMode == AM_NONE) return ;

	GINT	old_frame = mFrame;

	mFrame+=mFrameInc;

	if ((old_frame < GINT_HALF) && (mFrame >= GINT_HALF))
	{
		AnimTrigger();
	}


	if(mFrame>=G1)
	{
		mAnimMode= AM_NONE ;	
	}
}



//******************************************************************************************
void	CBallDispenser::Process()
{
	ProcessAnimation() ;

	// is it time to dispense another ball ?
	if (GAME.GetGameTurn() == mNextTurnToDepense )
	{
		SetAnimMode(AM_GATE, TRUE) ;
		mNextTurnToDepense = -2 ;
	}

	SUPERTYPE::Process();
}


//******************************************************************************************
void	CBallDispenser::StartPrematureDispense()
{
	// has it already dispensed a ball which has'nt been picked up yet.
	if (mNextTurnToDepense != -2) 
	{
		SetAnimMode(AM_GATE, TRUE) ;
		mNextTurnToDepense = -2 ;
	}
}


//******************************************************************************************
void	CBallDispenser::AnimTrigger()
{
	Dispense() ;
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

//******************************************************************************************
#ifdef LINK_EDITOR

void	CDroidBallDispenser::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CDroidBall);
}
#endif

//******************************************************************************************
void	CDroidBallDispenser::Dispense()
{
	GVector e1, e2;

	e1 = GetRenderThingEmitter(0) + mPos;
	e2 = GetRenderThingEmitter(1) + mPos;
	GVector vel = e2-e1 ;
	vel.Normalise() ;
	vel = vel / 5 ;

	CThing *t = SpawnThing(OID_CDroidBall);
	if (t)
	{
		CInitThing i;
		i.mPos = e2;
		i.mYaw = G_PI_2 ;
		i.mPitch = G(0) ;
		i.mRoll = G(0) ;

		i.mVelocity = vel ;
		t->Init(&i);
		((CDroidBall*)t)->SetDispensedBy(this) ;
	}
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************


#if 0


//******************************************************************************************
#ifdef LINK_EDITOR

void	CGrenadeDispenser::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CGrenade);
}
#endif

//******************************************************************************************
void	CGrenadeDispenser::Dispense()
{
	GVector e1, e2;

	e1 = GetRenderThingEmitter(0) + mPos;
	e2 = GetRenderThingEmitter(1) + mPos;
	GVector vel = e2-e1 ;
	vel.Normalise() ;
	vel = vel / 5 ;

	CThing *t = SpawnThing(OID_CGrenade);
	if (t)
	{
		CInitThing i;
		i.mPos = e2;
		i.mYaw = G_PI_2 ;
		i.mPitch = G(0) ;
		i.mRoll = G(0) ;

		i.mVelocity = vel ;
		t->Init(&i);
		((CGrenade*)t)->SetDispensedBy(this) ;
	}
}


#endif