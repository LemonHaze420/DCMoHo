// Strike.h: interface for the Strike class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STRIKE_H 
#define STRIKE_H

class CCharacter ;

enum eStrikeImplement
{
	SI_SMALL_PUNCH=1,
	SI_BIG_PUNCH=2,
	SI_WEAPON=4,
	SI_SPIN_PUNCH=8,
	SI_CHARGE_PUNCH = 16
};


class CStrike  
{
public:
	CStrike() : mOverwriteSpin(FALSE) {} ;
	~CStrike() {};


	CCharacter*			mOriginator;
	SINT				mStrength;
	BOOL				mWillKnockOut;
	GVector				mAttackVelocity;
	eStrikeImplement	mStrikedWith;	
	GINT				mShoulderToFistDist;
	BOOL				mOverwriteSpin;

};

#endif

