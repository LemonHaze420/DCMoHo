// DroidBallPot.cpp: implementation of the CDroidBallPot class.
//
//////////////////////////////////////////////////////////////////////
#include "common.h"
#include "globals.h"
#include "DroidBallPot.h"
#include "DroidBall.h"
#include	"Editors.h"
#include	"Character.h"

//******************************************************************************************
void	CDroidBallPot::Init(CInitThing *i)
{
	SUPERTYPE::Init(i) ;

	GET_COUNTER(0);
	
	if (mCounter0==-1)
	{
		mMagneticRadius = BALL_POT_POLE_CHECK_RADIUS ;
		mCounter0 = (BALL_POT_POLE_CHECK_RADIUS * 100).Whole();
	}
	else
	{
		mMagneticRadius =G(mCounter0) / G(100) ;
	}

	WORLD.GetDroidBallPotNB().Add(this) ;
}    

//******************************************************************************************
void	CDroidBallPot::Shutdown() 
{
	SUPERTYPE::Shutdown() ;
	WORLD.GetDroidBallPotNB().Remove(this) ;
}

//********************************************************************************************************
#if TARGET == PC
void	CDroidBallPot::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
}
#endif

//******************************************************************************************
void	CDroidBallPot::Process() 
{
	// if game not running then pole magnet is switched off..
	if (GAME.GetGameState() != GAME_STATE_RUNNING) return ;

	static GINT distsq = G0;

	const DroidBallInWorldList& items = WORLD.GetDroidBallNB();

	// loop through all checkpoints in world
	for (CDroidBall* nt = items.First() ;
					 nt != NULL ;
					 nt = items.Next() )
	{
		if (distsq == G0)
		{
			distsq = mMagneticRadius+nt->Radius() ;
			distsq = distsq * distsq ;
		}

		if (!(nt->GetFlags() & TF_FLASHING) && (!nt->IsStuckToPole()))
		{
			GVector ntp = nt->GetPos() ;
			GINT currentdistsq = (mPos - ntp).MagnitudeSqXY() ;

			if (currentdistsq <= distsq  ) // && ((mPos.Z - G(8)) < ntp.Z ))
			{	
				// ok hack so when we throw the ball close to the droid ball pot if the 
				// droid ball hits the pole during the animation then make it stick there
				if ( nt->IsBeingCarried() != NULL)
				{  
					GINT r1 = nt->Radius() + Radius() ;
					r1*=r1 ;
					if ((currentdistsq <= r1) &&
						nt->BeingCarriedBy()->GetRenderAnimation() == AM_THROW &&
						nt->BeingCarriedBy()->GetRenderFrame() > G(0,16234))
					{
						nt->BeingCarriedBy()->Drop(GOT_SUCKED_IN_BY_DROID_BALL_POST) ;
						nt->HitPole(this) ;
					}
				}
				else
				{
					nt->HitPole(this) ;
				}
			}
		}
	}
}


//******************************************************************************************
BOOL	CDroidBallPot::CloseToDroidBallPot(CCharacter* for_character, GINT* return_angle) 
{
	BOOL result = FALSE ;
	const DroidBallPotsInWorldList& items = WORLD.GetDroidBallPotNB();

	// loop through all checkpoints in world
	for (CDroidBallPot*	 nt = items.First() ;
						 nt != NULL ;
						 nt = items.Next() )
	{
		GVector vec_char_to_pot =nt->GetPos() - for_character->GetPos() ;
		GINT dist = vec_char_to_pot.MagnitudeSqXY() ;
		if (dist < G(25) )
		{
			*return_angle = -GATAN2(vec_char_to_pot.X, vec_char_to_pot.Y) ;
			return TRUE ;
		}
	}

	return result ;
}
