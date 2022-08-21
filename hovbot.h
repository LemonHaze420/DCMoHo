#ifndef HOVBOT_H
#define HOVBOT_H

#include	"Actor.h"
#include    "strike.h"
class CCharacter ;

enum eHovBotState
{
	HB_HEADING_TO_TARGET,
	HB_DYING,
	HB_STUNNED
} ;

DECLARE_THING_CLASS(CHovBot, CActor)
public:
	virtual	void		Init(CInitThing *i);

	virtual	void		Process();

	virtual	ECollideResponse	GetCollideResponse()	{return CR_BOUNCE;};
	virtual	void		Hit(CThing *that);
	virtual BOOL		IsStrikeable() { return TRUE ; }
	virtual	SINT		StrikeScore() {return 0;};  
	virtual	BOOL		IsPersistent() {return TRUE;};
	virtual	GINT		Radius() {return G(0,45000);};
			void		ChooseTarget() ;
			void		FaceTarget() ;
			void		ProcessHeadToTarget() ;

			void		ProcessDying() ;
			void		ProcessStunned();
			virtual BOOL		Strike(const CStrike& strike) ;
	virtual	BOOL		DoICollideWithMap() {return FALSE;};
#ifdef LINK_EDITOR
	virtual void		DeclareSpawnees() ;
#endif


	virtual	GMatrix			GetRenderOrientation(SINT no)
	{
		switch (no)
		{
		case 1:		return mOrientation2;
		default:	return SUPERTYPE::GetRenderOrientation(no);
		};
	};

protected:
	GINT	mYaw;
	GINT	mRoll;
	GINT	mRollRate ;
	GINT	mHoverHeight ;
	CCharacter*	mCurrentTarget;
	GMatrix mOrientation2 ;
	eHovBotState mState;
	SINT	mStateCounter;
	GVector	mStunnedVelocity ;
	GINT	mLastMapHeight ;
	GVector mPosBeforeMove;
};

#endif
