#include	"Common.h"

#include	"Pickup.h"
#include	"Globals.h"
#include    "Character.h"


//******************************************************************************************
void	CPickup::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mYaw = G(RANDOM() % 300) / 10;
	mOrientation.MakeRotationYaw(mYaw);

//	SitOnGround();
}

//******************************************************************************************
void	CPickup::Process()
{
	// rotate a bit
	mYaw += G(0, 3277);

	mOrientation.MakeRotationYaw(mYaw);

	if (AmILimited())
		mLife --;

	SUPERTYPE::Process();

	if (mLife < 40)
		mFlags |= TF_FLASHING;
}

//******************************************************************************************
void	CPickup::Hit(CThing *that)
{
	if (that->DoIAcceptPickups())
	{
		DoPickup(that);
		SOUND.PlaySample(GetSample(), this);
		
		GAME.CrowdEvent(G(0, 20000), G(0, 20000));
	}
}

//******************************************************************************************
void	CPickup::DoPickup(CThing *that)
{
	that->ApplyPickup(PickupType(), G1);
	SuicideNext();
}


//******************************************************************************************
SINT	CPickup::GetSample()
{
	return SOUND_PICKUP1;
};


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// PICKUP TOKEN for tricks and that

SINT CPUPToken::mTokensInWorld = 0 ;
//******************************************************************************************

void	CPUPToken::Init(CInitThing *i)
{ 
	mTokensInWorld++ ;
	mInvisible = FALSE ;
	SUPERTYPE::Init(i) ; 
	GET_COUNTER(0);
	mReGenTime = -2 ;
	mListedInStatic = TRUE;

} 


//******************************************************************************************
void	CPUPToken::Process()
{
	SUPERTYPE::Process() ;
	if (mReGenTime == GAME.GetGameTurn() && mInvisible )
	{
		mInvisible = FALSE ;
	}
}


//******************************************************************************************
void	CPUPToken::DoPickup(CThing *that)
{
	if (!GAME.IsMultiPlayer())
	{
		SUPERTYPE::DoPickup(that) ;
		return ;
	}

	that->ApplyPickup(PickupType(), G1);
	mInvisible = TRUE ;
	mReGenTime = GAME.GetGameTurn() + (mCounter0 *25);
}


//********************************************************************************************************
#if TARGET == PC
void	CPUPToken::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_COUNTER(0);
}
#endif

//******************************************************************************************
SINT	CPUPToken::GetSample()
{
	return SOUND_PICKUP3;
};
