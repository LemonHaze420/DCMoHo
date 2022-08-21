#ifndef	ANIMATION_H
#define	ANIMATION_H
   //
  // Rather sadly, changes in this list need to be reflected
 // in RTMesh and amiation.cpp sorry..
//
enum	EAnimMode
{
	AM_INVALID = -1,
	AM_NONE = 0,

	AM_WALK,	
	AM_RUN,		
	AM_STAND,	
	AM_JUMP,	
	AM_MID_JUMP,
	AM_PUNCH,	
	AM_FALLING,	
	AM_LANDING,
	AM_FLINCH,
	AM_DYING,
	AM_WALK2STAND,
	AM_TAILGRAB,
	AM_SHOOT,
	AM_SWITCH_ON,

	AM_CHRISTAIR,
	AM_BORED,
	AM_WAVE,
	AM_PUNCH2,		
	AM_PUNCH3,	
	AM_WEAPON_ATT,	
	AM_JUMP_ATTACK,
	AM_BLOCK,
	AM_FLINCH_BLOCK,
	AM_FLINCH_BEHIND,
	AM_PUNCH_BEHIND,
	AM_BLOCK_UP,
	AM_BLOCK_DOWN,
	AM_GUARD_UP,
	AM_GUARD_DOWN,
	AM_RAGE_ATTACK,
	AM_SPIN_ATTACK,
	AM_RUN_PUNCH_RIGHT,
	AM_RUN_PUNCH_LEFT,
	AM_CHARGE,
	AM_THROW,
	AM_VICTORY,
	
	AM_PUNCH_HIT,
	AM_WEAPON_HIT,
	AM_JUMP_HIT,

	AM_GATE,
	
	AM_FRONT_END1,
	AM_FRONT_END2,
	AM_FRONT_END3,
};

struct	SAnimModeInfo
{
	BOOL		mLooped;
	EAnimMode	mNextMode;
};

EAnimMode	GetNextAnim(EAnimMode am);
BOOL		IsAnimLooped(EAnimMode am);
BOOL		IsAnimJumping(EAnimMode am);
BOOL		IsAnimBusy(EAnimMode am);
BOOL		IsFinalAttackAnim(EAnimMode am);

#endif

