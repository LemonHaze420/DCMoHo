#include	"Common.h"

#include	"Animation.h"

EAnimMode	GetNextAnim(EAnimMode am)
{
	switch (am)
	{
	case AM_JUMP:		
	case AM_CHRISTAIR:
	case AM_TAILGRAB:
	case AM_JUMP_HIT:
		return AM_MID_JUMP;

	case AM_MID_JUMP:	return AM_LANDING;
//	case AM_FALLING:	return AM_LANDING;

	case AM_WEAPON_ATT:
		return AM_WEAPON_HIT;

	case AM_PUNCH:
		return AM_PUNCH_HIT;

	case AM_JUMP_ATTACK:
		return AM_JUMP_HIT;

	case AM_RUN_PUNCH_LEFT:
	case AM_RUN_PUNCH_RIGHT:
	case AM_LANDING:	
	case AM_SHOOT:
	case AM_SWITCH_ON:
	case AM_BORED:
	case AM_BLOCK_DOWN:
	case AM_GUARD_DOWN:
	case AM_FLINCH_BEHIND:
	case AM_PUNCH_BEHIND:
	case AM_FLINCH:		
	case AM_PUNCH_HIT:
	case AM_WEAPON_HIT:
	case AM_RAGE_ATTACK:
	case AM_SPIN_ATTACK:
	case AM_THROW:
	case AM_VICTORY:
	case AM_WAVE:
		return AM_WALK;


	case AM_WALK2STAND: return AM_STAND;

	case AM_FLINCH_BLOCK:
	case AM_BLOCK_UP: return AM_BLOCK;
	case AM_GUARD_UP: return AM_PUNCH3;

	case AM_PUNCH2:
	case AM_CHARGE:
	case AM_PUNCH3: return AM_GUARD_DOWN;	

	case AM_DYING:		return AM_INVALID; //  stop

	default:			return AM_NONE;
	};
}

BOOL	IsAnimLooped(EAnimMode am)
{
	switch (am)
	{
	case AM_WALK:
	case AM_RUN:
	case AM_STAND:
	case AM_MID_JUMP:
	case AM_BLOCK:
	case AM_FRONT_END1:
		return TRUE;
	default:
		return FALSE;
	};
}

BOOL	IsAnimJumping(EAnimMode am)
{
	switch (am)
	{
	case AM_JUMP:
	case AM_MID_JUMP:
	case AM_FALLING:
	case AM_LANDING:
	case AM_TAILGRAB:
	case AM_CHRISTAIR:
	case AM_JUMP_ATTACK:
	case AM_JUMP_HIT:
		return TRUE;
	default:
		return FALSE;
	};
}


BOOL	IsFinalAttackAnim(EAnimMode am)
{
	switch (am)
	{
	case AM_WEAPON_HIT:
	case AM_PUNCH_HIT:
	case AM_JUMP_HIT:
	case AM_CHARGE:
	case AM_PUNCH2:
	case AM_PUNCH3:
	case AM_RUN_PUNCH_LEFT:
	case AM_RUN_PUNCH_RIGHT:
		return TRUE;
	default:
		return FALSE;
	};
}


BOOL	IsAnimBusy(EAnimMode am)
{
	switch (am)
	{
	case AM_PUNCH:
	case AM_FLINCH:
	case AM_DYING:
	case AM_TAILGRAB:
	case AM_SWITCH_ON:
	case AM_CHRISTAIR:
	case AM_WAVE:
	case AM_PUNCH2:	
	case AM_GUARD_UP:		
	case AM_PUNCH3:
	case AM_WEAPON_ATT:
	case AM_JUMP_ATTACK:
	case AM_BLOCK:
	case AM_RAGE_ATTACK:
	case AM_SPIN_ATTACK:
	case AM_FLINCH_BLOCK:
	case AM_FLINCH_BEHIND:
	case AM_PUNCH_BEHIND:
	case AM_BLOCK_UP:
	case AM_VICTORY:
	case AM_RUN_PUNCH_LEFT:
	case AM_RUN_PUNCH_RIGHT:
	case AM_CHARGE:
	case AM_THROW:
	case AM_SHOOT:
	case AM_PUNCH_HIT:
	case AM_WEAPON_HIT:
	case AM_JUMP_HIT:
		return TRUE;

	default:
		return FALSE;
	}

}


