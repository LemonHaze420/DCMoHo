#include	"Common.h"
#include	"Globals.h"
#include    "HovBot.h"
#include    "Character.h"

#define AVERAGE_HOVER_BOT_HEIGHT  -(G(2,12000))

#define STOP_FOLLOWING_HEIGHT_DIFF -G(2)

//******************************************************************************************
void	CHovBot::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	StickToGround();

	mYaw = G(RANDOM() % 300) / 10;
	mRoll = G0 ;
	mOrientation2.MakeRotationYaw(mYaw);
	mCurrentTarget = NULL ;
	mState = HB_HEADING_TO_TARGET ;
	GINT height_diff = G(0,RANDOM()%30000) - G(0,15000) ;
	mHoverHeight = AVERAGE_HOVER_BOT_HEIGHT + height_diff;
	mStateCounter = 0 ;
	mRollRate = G0 ;
	mLastMapHeight = -G(20) ;
	mPosBeforeMove = GVector(G0,G0,G0) ;

//	SitOnGround();
}


//******************************************************************************************
void	CHovBot::ChooseTarget()
{
	// get a new target

	CListArray<CCharacter, MAX_CHARACTERS_IN_WORLD>& items = WORLD.GetCharacterNB();
	CCharacter	*current = NULL;
	GINT		mind = G(20)*G(20);

	CCharacter* nt;
	for (nt = items.First();
		 nt != NULL ;
		 nt = items.Next() )
	{
		if (nt && nt->IsPrimary() )
		{
			ECharState cs = nt->GetState();
			// cant select targets above us
			GINT height_diff = ( nt->GetPos().Z - mPos.Z ) ;
			if (cs != CS_DEAD && (height_diff > STOP_FOLLOWING_HEIGHT_DIFF ))
			{
				GINT	dist = (mPos - nt->GetPos()).MagnitudeSq();
				if (dist < mind)
				{
					mind = dist;
					current = nt;
				}
			}
		}

	}
	mCurrentTarget = current;
}

//******************************************************************************************
void	CHovBot::FaceTarget()
{

	GVector u_to_t =  (mCurrentTarget->GetPos() - mPos ) ;
	GINT angle_you_to_target = GATAN2(u_to_t.X, u_to_t.Y);
	mOrientation.MakeRotationYaw(-angle_you_to_target);

}



//******************************************************************************************
void	CHovBot::ProcessHeadToTarget()
{
	// rotate a bit
	mYaw += G(0, 13277);

	mOrientation2.MakeRotationYaw(mYaw);

	if (mCurrentTarget == NULL ||
		(mCurrentTarget && mCurrentTarget->GetState() == CS_DEAD) )
	{
		ChooseTarget() ;
	}
	else
	{
		if (RANDOM()%50 == 23)
		{
			ChooseTarget() ;
		}
	}

	GINT map_height =  MAP.GetMapHeight(mPos) ;
	GINT h_diff = map_height - mPos.Z ;
	GINT hd = mHoverHeight + h_diff;
	mVelocity = mOrientation * GVector(G0, G0, hd * G(0, 20000));
	if (mCurrentTarget == NULL || (!GAME.IsGameRunning())) return ;

	GINT map_diff = mLastMapHeight - map_height ;
	mLastMapHeight = map_height ;
	if (map_diff > G(0,4000) )
	{
		Move(mPosBeforeMove);
		mVelocity.X = G0 ;
		mVelocity.Y = G0 ;
		return ;
	}

	GVector u_to_target = mCurrentTarget->GetPos() - mPos ;
	if ( u_to_target.Z < STOP_FOLLOWING_HEIGHT_DIFF )
	{
		mCurrentTarget=NULL;
		return;
	}


	if (GAME.GetGameState() >= GAME_STATE_GAME_FINISHED) return ;

	GINT dist = u_to_target.MagnitudeXY() ;
	u_to_target.Z = G0;
	u_to_target.Normalise() ;

	// ok make things speedup up closer they are to you
	GINT speedup= ((G(10)-dist)*G(0,361)) ;
	speedup*=3;
	if (speedup < G0) speedup = G0;

	// speed up blade going round
	mYaw+=speedup*G(1,62121);

	// make hov bot move towards target
	u_to_target*=(G(0,3611) + speedup)  ;
	AddVelocity(u_to_target);
	FaceTarget() ;
}

//******************************************************************************************
void	CHovBot::ProcessDying()
{
	mYaw += G(0, 24831);
	mRoll += mRollRate ;
	mRollRate*=G(0,63000) ;

	mOrientation.MakeRotationRoll(mRoll);
	mOrientation2.MakeRotationYaw(mYaw);
	GINT mh = MAP.GetMapHeight(mPos);
	if (mPos.Z > mh)
	{
		CThing	*t = SpawnThing(OID_CFXExplosion1);
		if (t)
		{
			CInitThing i;
			i.mPos = mPos;
			t->Init(&i);
		}
		MAP.Raindrop(CCellRef(mPos), G(0 , 4000));
		Die() ;
		return ;
	}
}

#define HOVBOT_STUNNED_TIME 20
//******************************************************************************************
void	CHovBot::ProcessStunned()
{
	mYaw += G(0, 23277);
	mOrientation2.MakeRotationYaw(mYaw);
	GINT map_height =  MAP.GetMapHeight(mPos);
	GINT hd = mHoverHeight + (map_height - mPos.Z);
	mVelocity = mOrientation * GVector(G0, G0, hd * G(0, 20000));
	GINT map_diff = mLastMapHeight - map_height ;
	mLastMapHeight = map_height ;
	if (map_diff > G(0,16000) )
	{
		Move(mPosBeforeMove);
		mVelocity.X = G0 ;
		mVelocity.Y = G0 ;
		return ;
	}
	AddVelocity(mStunnedVelocity) ;
	mStateCounter++ ;
	if (mStateCounter > HOVBOT_STUNNED_TIME)
	mState = HB_HEADING_TO_TARGET ;
	mStunnedVelocity*=G(0,55000) ;
}


//******************************************************************************************
void	CHovBot::Process()
{
	mPosBeforeMove = mPos ;
	SUPERTYPE::Process();
	switch (mState)
	{
	case HB_HEADING_TO_TARGET:
		{
			ProcessHeadToTarget() ;
			break ;
		}
	case HB_DYING:
		{
			ProcessDying() ;
			break ;
		}
	case HB_STUNNED:
		{
			ProcessStunned() ;
			break ;
		}
	default:
		{
			ASSERT(0) ;
		}
	}
}

//******************************************************************************************
BOOL	CHovBot::Strike(const CStrike& strike)
{
	if (mState == HB_DYING) return FALSE ;

	mState = HB_DYING ;
	mRoll = G(0,32222) + G(0,RANDOM()%10000) ;
	GVector	dir = mPos - strike.mOriginator->GetPos();
	dir.Normalise();
	GVector attack_velocity;
	if (strike.mStrikedWith == SI_SMALL_PUNCH)
	{
		attack_velocity = dir * G(0, 32000);
		mRollRate = G(0,38277) ;
	}
	else
	{
		attack_velocity = dir * G(0, 48000);
		mRollRate = G(0,18277) ;
	}
	AddVelocity(attack_velocity);
	return TRUE ;
}


//******************************************************************************************
void	CHovBot::Hit(CThing *that)
{
	CCharacter* c = that->IsCharacter() ;

	if (c && !c->IsStriking())
	{
		GVector u_to_target = that->GetPos() - mPos ;
		u_to_target.Normalise() ;
		GINT angle_that_to_you = -GATAN2(-u_to_target.X, -u_to_target.Y);
		GVector hit_velocity = u_to_target*G(0,16123) ;

		GINT damage = G(0,16123) ;

		if (c->IsBlocking() )
		{
	     	GINT this_ang = GATAN2(u_to_target.X, u_to_target.Y) ;
			GINT angle_character_to_me = AngleDifference(-c->GetYaw(), this_ang) ;

			if (GABS(angle_character_to_me) > ( G_PI - BLOCKING_DEFFENSE_ANGLE )  )
			{
				// successfull block so projectile will now bounce cos it is not being
				// killed and do half damage to character
				damage = damage >> 2  ;
			}
			else
			{
				c->SetAnimMode(AM_FLINCH);
			}
		}
		else
		{
			c->SetAnimMode(AM_FLINCH);
		}
		c->	AddStrikeVelocity(hit_velocity,BIG_PUNCH_COMBAT_MAX_VELOCITY) ;
		c->Damage(damage) ;

		if (mState == HB_DYING) return ;

		GVector right_of_spinner = mOrientation*GVector(-G(0,16124),G0,G0) ;
		GVector stunned_velocity = -(u_to_target+right_of_spinner) ;
		stunned_velocity.Z = G0 ;
		mStunnedVelocity = (stunned_velocity*G(0,64000));
		mStateCounter = 0 ;
		mState = HB_STUNNED ;
		GVector hit_point = that->GetPos();
		hit_point.Z = mPos.Z;
		c->GetRenderParticleSystem()->MakeHitEffect(&hit_point, angle_that_to_you,HT_NORMAL) ;

	}
}


//******************************************************************************************
#ifdef LINK_EDITOR

void	CHovBot::DeclareSpawnees()
{
	SUPERTYPE::DeclareSpawnees();
	WORLD.AccumulateOIDResources(OID_CFXExplosion1);

}
#endif
