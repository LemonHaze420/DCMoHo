#include "Common.h"
#include "globals.h"
#include "BadGuyPrimary.h"


//******************************************************************************************
void	CBadGuyPrimary::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mOddsOfDoingSpinAttack = -1 ;
	if (mCounter0 >=4)
	{
		mOddsOfDoingSpinAttack = (mCounter0 >> 1) +10;
	}
	mCanBlockBF |= SI_SPIN_PUNCH ;

}

//******************************************************************************************
BOOL	CBadGuyPrimary::Attack(SINT no)
{
	BOOL res = CCharBall::Attack(no) ;
	if (res == FALSE) return FALSE ;

	if (MainCharacterAttack(no))
	{
		return TRUE ;
	}
	return FALSE ;
}

//******************************************************************************************
BOOL CBadGuyPrimary::CounterAttack(BOOL facing_target) 
{
	if (!IsStriking())
	{
		Attack(1) ;
	}

	if (mCurrentAction != ATTACK_TARGET)
	{
		StartAttackingTarget() ;
			mStateTimer = 0;
	}
	return TRUE ;
}

#define BAD_GUY_FRAMES_END_COMBO 45


//******************************************************************************************
BOOL CBadGuyPrimary::DoAnAttack() 
{
	// spin attack
	SINT pc = RANDOM()%100 ;

	SINT diff = GAME.GetGameTurn() - mLastPunchFrameNum ;
	if (mCurrentPunchSequenceNum ==0 || diff >= BAD_GUY_FRAMES_END_COMBO ) 
	{
		SINT pc = RANDOM()%100 ;
		if (mOddsOfDoingSpinAttack != -1 && pc <= mOddsOfDoingSpinAttack)
		{
			return Attack(1) ;
		}
	}

	return Attack(0) ;
}

//******************************************************************************************
GINT	CBadGuyPrimary::GroundFriction(EFrictionType friction_type)
{
	GINT friction = SUPERTYPE::GroundFriction(friction_type);
	if (mAnimMode == AM_CHARGE) friction*=G(0,62000) ;
	return friction ;
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CBadGuyBenny::Process() 
{
	if (mAnimMode == AM_PUNCH || mAnimMode == AM_PUNCH_HIT)
	{ 
		mDFlags&= (~DFC_DrawTrail) ; 
	}
	else
	{
		mDFlags|= DFC_DrawTrail ; 
	}
	SUPERTYPE::Process() ; 
}


//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void	CBadGuyDoctor::Process() 
{
	if (mAnimMode == AM_PUNCH || mAnimMode == AM_PUNCH_HIT)
	{ 
		mDFlags&= (~DFC_DrawTrail) ; 
	}
	else
	{
		mDFlags|= DFC_DrawTrail ; 
	}
	SUPERTYPE::Process() ; 
}

/*
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
void	CKingHillBadGuyPrimary::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mOddsOfDoingSpinAttack = -1 ;
	if (mCounter0 >=4)
	{
		mOddsOfDoingSpinAttack = (mCounter0 >> 1) +30;
	}
}

//******************************************************************************************
BOOL	CKingHillBadGuyPrimary::Attack(SINT no)
{
	BOOL res = CCharBall::Attack(no) ;
	if (res == FALSE) return FALSE ;

	if (MainCharacterAttack(no))
	{
		return TRUE ;
	}
	return FALSE ;
}


#define BAD_GUY_FRAMES_END_COMBO 45


//******************************************************************************************
BOOL CKingHillBadGuyPrimary::DoAnAttack() 
{
	// spin attack
	SINT pc = RANDOM()%100 ;

	SINT diff = GAME.GetGameTurn() - mLastPunchFrameNum ;
	if (mCurrentPunchSequenceNum ==0 || diff >= BAD_GUY_FRAMES_END_COMBO ) 
	{
		SINT pc = RANDOM()%100 ;

		// less likely to do a spin attack if have a weapon
		if (mHoldingThing) pc=pc << 1 ;

		if (mOddsOfDoingSpinAttack != -1 && pc <= mOddsOfDoingSpinAttack)
		{
			return Attack(1) ;
		}
	}

	return Attack(0) ;
}

//******************************************************************************************
GINT	CKingHillBadGuyPrimary::GroundFriction(EFrictionType friction_type)
{
	GINT friction = SUPERTYPE::GroundFriction(friction_type);
	if (mAnimMode == AM_CHARGE) friction*=G(0,62000) ;
	return friction ;
}
*/