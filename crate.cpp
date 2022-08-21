#include	"Common.h"

#include	"Globals.h"
#include	"Crate.h"
#include	"Effect.h"

//******************************************************************************************
void	CCrate::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	mZBase = mPos.Z;
	mZVel = G0;

	mSmashed = FALSE;

	mOID = i->mOID;

	if (mOID == OID_INVALID)
		mOID = OID_NONE;

	GET_TARGET;
}

//******************************************************************************************
#if TARGET == PC || TARGET == DC
//******************************************************************************************
void	CCrate::WriteData(CInitThing &i)
{
#if TARGET == PC
	SUPERTYPE::WriteData(i);

	 i.mOID = mOID;
	 WRITE_TARGET;
#endif
}

#if TARGET == PC && defined(LINK_EDITOR)
//******************************************************************************************
void	CCrate::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(mOID);
//	WORLD.AccumulateOIDResources(OID_CPUPHealth);
}
#endif
//******************************************************************************************
#endif
//******************************************************************************************
void	CCrate::Process()
{
//	StickToGround();

	SUPERTYPE::Process();

	// move in Z
	mPos.Z += mZVel;

	if ((mZBase != mPos.Z) || (mZVel != G0))
	{
		mZVel += WORLD.Gravity();

		if (mPos.Z >= mZBase)
		{
			mPos.Z = mZBase;
			if (mZVel > WORLD.Gravity() * 3)
			{
				mZVel = -mZVel * G(0, 30000);
			}
			else
				mZVel = G0;
		}
	}
}

//******************************************************************************************
void	CCrate::Hit(CThing *that)
{
//	that->Stun(G1);
}


//******************************************************************************************
void	CCrate::Smash()
{
	if (mSmashed)
		return;

	mSmashed = TRUE;

	Die();

	// Spawn bonus thing?
	CThing	*t = NULL;

	if (mOID != OID_NONE)
		t = SpawnThing(mOID);

	if (t)
	{
		CInitThing	i;
		i.mPos = mPos;
		i.mPos.Z -= GINT_HALF;
		i.mVelocity = GVector(G0, G0, -G(0, 20000));
		t->Init(&i);

		// stop you spawning thing twice if you can smash crate again fast enough
		mOID=OID_NONE;
	}

	// kill something??
	if (mTarget)
		mTarget->Die();
		
	// sample
	SOUND.PlaySample(SmashSample(), this);
	
	
}

//******************************************************************************************
void	CCrate::Rock()
{
	if (DoIRock())
	{
		// rock a bit
		mZVel = -WORLD.Gravity() * 5;
	}
}

//******************************************************************************************
BOOL	CCrate::Strike(const CStrike& strike)
{
	SUPERTYPE::Strike(strike);

	// stop the effect of the damage function
	mLife +=HitDamage() /4;
	mLife -=HitDamage();

	// if hit with a weapon then smash straight away

	if (strike.mStrikedWith == SI_WEAPON) mLife = 0 ;

	if (mLife <= 0)
		Smash();

	return TRUE ;
}

//******************************************************************************************
void	CCrate::Damage(GINT amount)
{
	mLife -= (HitDamage() /4) ;
	if (mLife <= 0)
		Smash();
	else
	{
		if (mZVel == G0) Rock() ;
	}
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CGenerator::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mEffect = SpawnThing(OID_CFXFlamer2);
	if (mEffect)
	{
		CInitThing i;
		
		i.mPos = mPos + GetRenderThingEmitter(0);
		i.mOrientation.MakeRotationPitch(-G_PI_2);
		i.mAngleType = AT_ORI;

		mEffect->Init(&i);
	}

	mDeathCount = 0;
	
	SOUND.PlaySample(SOUND_GENERATOR, this, G(0, 20000), TRUE, TRUE);
}

//******************************************************************************************
#ifdef LINK_EDITOR

void	CGenerator::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();

	WORLD.AccumulateOIDResources(OID_CFXFlamer2);
	WORLD.AccumulateOIDResources(OID_CFXExplosion1);
	WORLD.AccumulateOIDResources(OID_CFXExplosion2);
}
#endif

//******************************************************************************************
void	CGenerator::Smash()
{
	if (mEffect)
		mEffect->Die();

	// spawn explosion part 1
	CThing	*t = SpawnThing(OID_CFXExplosion1);
	if (t)
	{
		CInitThing i;
		i.mPos = mPos;
		t->Init(&i);
	}
	
	//sound
	SOUND.KillSamples(this);
	SOUND.PlaySample(SOUND_GENER_DIE, this, G(0, 60000), FALSE, TRUE);

	SUPERTYPE::Smash();
}

//******************************************************************************************
void	CGenerator::Die()
{
	mDeathCount = 1;
}

//******************************************************************************************
void	CGenerator::Process()
{
	if (mDeathCount)
	{
		mDeathCount ++;
		if (mDeathCount == 16)
		{
			SuicideNext();

			// spawn explosion part 2
			CThing	*t = SpawnThing(OID_CFXExplosion2);
			if (t)
			{
				CInitThing i;
				i.mPos = mPos;
				t->Init(&i);
			}
		}
	}
	else
	{
		// regenerate sample
		if (((GAME.GetGameTurn() + mNumber) & 15) == 0)
			SOUND.PlaySample(SOUND_GENERATOR, this, G(0, 20000), TRUE, TRUE);
	}

	SUPERTYPE::Process();
}
