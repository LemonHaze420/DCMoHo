#include	"Common.h"

#include	"Globals.h"
#include	"Laser.h"
#include	"Primary.h"

//******************************************************************************************
void	CLaserTurret::Init(CInitThing *i)
{

	SUPERTYPE::Init(i);

	StickToGround();
	
	if (i->mAngleType == AT_YPR)
	{
		mYaw = i->mYaw;
		mPitch = i->mPitch;
		mOrientation.MakeRotation(G0, G0, i->mRoll);
	}		
	else		
	{
		mYaw = G0;
		mPitch = G0;
	}

	mStartBurstNumber = mNumber * 43 ;

	mOrientation2.MakeRotation(mYaw, mPitch, G0);

	GET_COUNTER(0);
	GET_COUNTER(1);

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

	mBeam = NULL;
}

//********************************************************************************************************
#if TARGET == PC
void	CLaserTurret::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
	WRITE_COUNTER(1);
	WRITE_INITIAL_MODE;
	WRITE_TRIGGER_RESPONSE;
	WRITE_UNTRIGGER_RESPONSE;
}
#endif


//******************************************************************************************
void	CLaserTurret::ActivateLaserTurret()
{
	if (mActivated== FALSE)
	{
		mActivated= TRUE ;
		// make the turret fire now !! 
		mStartBurstNumber = mCounter0 - ((GAME.GetGameTurn() /3) % mCounter0);
	}
}

//******************************************************************************************
void	CLaserTurret::Trigger(CThing *top)
{
	switch(mTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateLaserTurret(); break;
	case TR_DEACTIVATE:
		mActivated = FALSE; break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateLaserTurret();
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
void	CLaserTurret::UnTrigger(CThing *top)
{
	switch(mUnTriggerResponse)
	{
	case TR_ACTIVATE:
		ActivateLaserTurret(); break;
	case TR_DEACTIVATE:
		mActivated = FALSE; break;
	case TR_TOGGLE:
		{
			if (mActivated == FALSE)
			{
				ActivateLaserTurret();
			}
			else
			{
				mActivated = FALSE;
			}
			break ;
		}
	};
	mTimer = 0;

	SUPERTYPE::UnTrigger(top);
}


//******************************************************************************************
void	CLaserTurret::Shoot()
{
	if (mBeam)
		return; // already have one!

	mBeam = SpawnThing(OID_CLaserBeam);

	if (mBeam)
	{
		CInitThing i;
		
		i.mAngleType = AT_ORI;
		i.mOrientation.MakeRotation(mYaw, mPitch, G0);
		i.mPos = mPos + GetRenderThingEmitter(0); // + v * G(2);

		mBeam->Init(&i);
	}
}

//******************************************************************************************
void	CLaserTurret::DontShoot()
{
	if (mBeam)
	{
		mBeam->SuicideNext();
		mBeam = NULL;
	}
}

//******************************************************************************************
void	CLaserTurret::Process()
{
	mPos.Z = MAP.GetMapHeightDifference(mPos) ;
	BOOL	on = FALSE;

	if (mActivated)
	{
		SINT	num = (GAME.GetGameTurn() / 3 )+ mStartBurstNumber;

 		if ((mCounter0 == -1) ||
			((num % mCounter0) < mCounter1))
		{
			GVector d = (WORLD.GetPrimaryPosition() - mPos);
			if (d.MagnitudeSq() < G(4000))
				on = TRUE;
		}
	}

	if (on)
		Shoot();
	else
		DontShoot();

	SUPERTYPE::Process();
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CLaserTurret::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CLaserBeam);
}
#endif

//******************************************************************************************


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************


//******************************************************************************************
void	CLaserBeam::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	GVector	try_end = i->mPos + mOrientation * GVector(G0, G(29), G0);
	GVector	end = MAP.FindLineIntersection(mPos, try_end);
	
	if (end.X == -G(1))
	{
		end = try_end;
		
		// clip with the arena walls (assumes at right angles...
		if (end.X < G0)
			end.X = G0;
		if (end.Y < G0)
			end.Y = G0;
		if (end.X > G(MAP_WIDTH))
			end.X = G(MAP_WIDTH);
		if (end.Y > G(MAP_HEIGHT))
			end.Y = G(MAP_HEIGHT);
	}

	// set the Z from the line
	GVector	d1 = try_end - mPos;
	GVector d2 = end - mPos;
	end.Z = mPos.Z + (d1.Z * d2.MagnitudeXY() / d1.MagnitudeXY());

	SetStartAndEnd(i->mPos, end);

	mRadius = (i->mPos - end).Magnitude() / 2;

	// changed the radius!
	MAP.RemoveFromMapWhoSizeUnknown(this);
	MAP.AddToMapWho(this);
	mLastFrameSomeOneHitUs = -2 ;
}

//******************************************************************************************
void	CLaserBeam::SetStartAndEnd(GVector &start, GVector &end)
{
	GVector	avg = (start + end) / 2;

	Move(avg);
	mStart = start;
	mEnd = end;
	mOriginalEnd = end ;

	// precalc normal for collision
	GVector d = mEnd - mStart;
	GINT	l = d.Magnitude();

	if (l < G(0, 100))
		mNormal = GVector(G0, G1, G0);  // erk...

	mNormal = (d ^ GVector(G0, G0, G1)) / l;
}

//******************************************************************************************
void CLaserBeam::Process()
{
	SUPERTYPE::Process() ;

	if (GAME.GetGameTurn() - 4 == mLastFrameSomeOneHitUs)
	{
		mEnd = mOriginalEnd ;
	}
}

//******************************************************************************************
void	CLaserBeam::Hit(CThing *that)
{
	CCharacter *c = that->IsCharacter();

	if (c && c->GetState() != CS_DEAD)
	{
		// find point on beam which hits character	
		GINT dis = (c->GetPos() - mStart).MagnitudeXY() ;
		GVector s_to_e = (mEnd - mStart) ;
		GVector hit_point = s_to_e ;
		hit_point.Normalise() ;
		hit_point*=dis ;
		hit_point+=mStart ;

		GINT bottom_of_that = c->GetPos().Z+c->Radius() ;
		GINT top_of_that ;
		if (that->GetCollideShape() == CSH_SPHERE)
		{
			top_of_that = c->GetPos().Z-c->Radius() ;
		}
		else
		{
			top_of_that = c->GetPos(1).Z-G(2) ;
		}

		// ok are we under or over the beam 
		if (hit_point.Z < bottom_of_that &&
			hit_point.Z > top_of_that)
		{
			// move character to centre of beam

			if (c->DoIGetLasered())
			{
				s_to_e.Z = G0;
				s_to_e.Normalise() ;
				s_to_e *=dis ;
				s_to_e+=mStart ;
				s_to_e.Z = c->GetPos().Z;
				c->Move(s_to_e) ;
			}
			// change state of charater else do sparcle effect if ammune
			c->BeingLasered(hit_point) ;
			mEnd = hit_point ;
			mLastFrameSomeOneHitUs = GAME.GetGameTurn() ;
			//		SOUND.PlaySample(1, this);
		}
	}

}
