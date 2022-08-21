#include	"Common.h"

#include	"Globals.h"
#include	"Projectile.h"
#include	"Player.h"
#include	"Character.h"
#include	"fence.h"

#include	"PSXBuildType.h"

//******************************************************************************************
void	CProjectile::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

/*	GINT	l = mVelocity.Magnitude();

	if (l > G(0, 100))
	{
		GVector v = mVelocity / l;

		GVector	u = GVector(G0, G0, -G(1));

		GVector	r = v ^ u;
		r.Normalise();
		u = v ^ r;

		mOrientation.Row[0] = r;
		mOrientation.Row[1] = v;
		mOrientation.Row[2] = -u;

		mOrientation.TransposeInPlace();
	}*/

	mGravity = FALSE;
}

//******************************************************************************************
void	CProjectile::Process()
{
	mLife --;
	if (mLife == 0)
		Suicide();

	SUPERTYPE::Process();
}

//******************************************************************************************
void	CProjectile::HitGround(GINT force)
{
	SuicideNext();

	BOOL seen=GAME.CanIBeSeen(mPos);
	switch (GetHitAction())
	{
	case EPJH_NORMAL:
		// $$$ NORMAL BULLET HIT GROUND EFFECT
		CPlayer::PickupSubCell(mPos, G(2, 0));
#ifndef DEMO_GAME
		if(seen)
		{
//damo!!!!
			MAP.GetEffectParticleSystem()->MakeSparks(&mPos);
		}
#endif
//		SOUND.PlaySample(GetHitSample());
		break;

	case EPJH_RAINDROP:
		// $$$ DEADLY PINK TENNIS BALL HIT GROUND EFFECT
//		MAP.Raindrop(CCellRef(mPos), G(0 , 15000));
		MAP.Raindrop(CCellRef(mPos), G(0 , 10000));
#ifndef DEMO_GAME
		if(seen)
		{
/*			GVector pos;
			pos.X=mPos.X;
			pos.Y=mPos.Y;
			pos.Z.mVal=mPos.Z.mVal-0x10000;*/
			MAP.GetEffectParticleSystem()->MakeExplosion1Small(&mPos) ;//MakeHitEffect(&mPos,G(0),HT_BULLETGROUND);
		}
#endif
		break;
	};
}

//******************************************************************************************
void	CProjectile::Hit(CThing *that)
{
	// hmmmmm
	if (CActor::mCurrentBCollideShape == CSH_FENCE)
	{
		// hmmm this is bad
		if (((CFenceSection*)that)->AboveOrBelow(mPos) ) return ;
	}

	BOOL bounceoff=FALSE;
	GINT damage(0,10000) ;


	switch (GetHitAction())
	{

	case EPJH_RAINDROP: damage*=5 ; break ;

	};

	GINT angle_character_to_me;
	if (that->IsCharacter() && ((CCharacter*)that)->IsBlocking() )
	{
		GINT this_ang = GATAN2(mVelocity.X, mVelocity.Y) ;
			 angle_character_to_me = AngleDifference(-((CCharacter*)that)->GetYaw(), this_ang) ;

		if (GABS(angle_character_to_me) > ( G_PI - BLOCKING_DEFFENSE_ANGLE )  )
		{
			// successfull block so projectile will now bounce cos it is not being
			// killed and do half damage to character
			bounceoff=TRUE;
			damage >> 2 ;
		}
		// !DAMO Next line fixes a bug...
		else SuicideNext();

		that->Damage(damage);
		SOUND.PlaySample(SOUND_PUNCH1, this);
//		SOUND.PlaySample(GetHitSample(), this);
	}
	else
	{
		// $$$ BULLET HIT CHARACTER EFFECT
		that->Damage(damage);


		SuicideNext();
		SOUND.PlaySample(SOUND_PUNCH2, this);
//		SOUND.PlaySample(GetHitSample(), this);
	}

			CCharacter* c=that->IsCharacter();
			if(c)
			{
				CParticleSystem *ps=c->GetParticleSystem();
				if(ps)
				{
					if(bounceoff)ps->MakeHitEffect(&mPos,-angle_character_to_me,HT_BLOCK);
					else ps->MakeHitEffect(&mPos,-angle_character_to_me,HT_NORMAL);
				}
			}

	SUPERTYPE::Hit(that);
}

//******************************************************************************************
/*GINT	CProjectile::GetRenderFrame()
{
//	if ((((GAME.GetGameTurn() + mNumber * 43) / 8) & 1) == 0)
	if (((GAME.GetGameTurn() + mNumber * 43) & 1) == 0)
		return G1;
	else
		return G0;

}*/

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

//******************************************************************************************
void	CPJBullet::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

//	mRoll = G0;
	mYaw = G0;
}

//******************************************************************************************
void	CPJBullet::Process()
{
	mYaw += G(0, 30000);

	SUPERTYPE::Process();
}
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

//******************************************************************************************
void	CPJBulletRainDrop::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

//	mRoll = G0;
	mYaw = G0;
}

//******************************************************************************************
void	CPJBulletRainDrop::Process()
{
	mYaw += G(0, 30000);

	SUPERTYPE::Process();
}
