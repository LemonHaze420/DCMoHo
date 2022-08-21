#include	"Common.h"

#include	"Switch.h"
#include	"Character.h"
#include	"Globals.h"
#include	"CharBall.h"

void	CSwitch::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

	mFlags |= TF_RENDER_IN_EDITOR_ONLY;

	GET_TARGET;

	mSwitched = FALSE;

	SitOnGround();
}

//******************************************************************************************
#if TARGET == PC
void	CSwitch::WriteData(CInitThing &i)
{
	SUPERTYPE::WriteData(i);

	WRITE_TARGET;
}
#endif

//******************************************************************************************
//******************************************************************************************
void	CSwitchDead::Process()
{
	if (!mSwitched)
	{
		CCharacter	*c;
		if ((!mTarget) ||
			(c = mTarget->IsCharacter(), (c) ? c->GetState() == CS_DEAD : FALSE))
		{
			mSwitched = TRUE;
		}
	}

	SUPERTYPE::Process();
}

//******************************************************************************************
//******************************************************************************************
void	CSwitchAND::Process()
{
	if (mTarget && mChainTrigger)
	{
		CSwitch	*s1, *s2;

		s1 = mTarget->IsSwitch();
		s2 = mChainTrigger->IsSwitch();

		if (s1 && s2)
		{
			mSwitched = s1->IsSwitched() && s2->IsSwitched();
		}
		else
			mSwitched = FALSE;
	}
	else
		mSwitched = FALSE;
}

//******************************************************************************************
//******************************************************************************************
void	CSwitchOR::Process()
{
	if (mTarget && mChainTrigger)
	{
		CSwitch	*s1, *s2;

		s1 = mTarget->IsSwitch();
		s2 = mChainTrigger->IsSwitch();

		if (s1 && s2)
		{
			mSwitched = s1->IsSwitched() || s2->IsSwitched();
		}
		else
			mSwitched = FALSE;
	}
	else
		mSwitched = FALSE;
}

//******************************************************************************************
//******************************************************************************************
void	CPit::Init(CInitThing *i)
{
	SUPERTYPE::Init(i);

#if TARGET == PSX
	MAP.SetOtherFlags(CCellRef(mPos), F_4x4_NEVER);
#endif



}

//******************************************************************************************
void	CPit::Hit(CThing* that)
{
	CCharacter* c = that->IsCharacter() ;
	if (c)
	{
		GVector c_pos = that->GetPos() ;
		if (c_pos.Z > ( mPos.Z+G(1) ) && c->GetState() != CS_DEAD)
		{
			GINT new_dest_yaw = c->GetYaw()+G(0,50000);
			if (new_dest_yaw > G_PI) new_dest_yaw-=(G_PI*2) ;
			((CCharBall*)c)->SetYaw(new_dest_yaw);

			c->TriggerVibration(6);



			if ((GAME.GetGameTurn() & 7) ==0)
			{
				GVector pos = GVector(c_pos.X,c_pos.Y,mPos.Z+G(1)) ;
				c->GetRenderParticleSystem()->MakeHitEffect(&pos, G0 + G_PI,HT_NORMAL);
			}
			GVector v = c->GetVelocity() ;

			if (GAME.GetGameTurn() & 1 )
			{
				c->SetVelocity(GVector(G(0,4000) ,G(0,4000),v.Z*G(0,60000))) ;
			}
			else
			{
				c->SetVelocity(GVector(-G(0,4000) ,-G(0,4000),v.Z*G(0,60000))) ;
			}

		}

		if (c_pos.Z > ( mPos.Z+G(4,32123) ) )
		{
			// ok we've been sliced
			GVector ar = mPos+GVector(G0,G0,G(4,32123)) ;
			c->Move(ar) ;
			c->SetVelocity(ZERO_GVECTOR);
			if (c->GetState() != CS_DEAD)
			{
				c->Damage(100000) ;
				// kill em now..
				GVector ar1 = mPos+GVector(G0,G0,G(1)) ;
				c->Move(ar1) ;
				c->StartDieProcess(TRUE) ;
				c->SetVelocity(ZERO_GVECTOR);
				c->Move(ar) ;
				((CCharBall*)c)->SetInvisible(TRUE);
			}
		}
	}
}


//******************************************************************************************
void	CPit::Process()
{
	static BOOL pre_calc = FALSE ;
	static GMatrix pre_cal_mat[10] ;

	// pre calc all orientions of fan for speed purposes.
	if (pre_calc==FALSE)
	{
		pre_calc=TRUE ;
		GINT yaw = G0;
		for (SINT i=0; i<10; i++)
		{
			pre_cal_mat[i].MakeRotationYaw(yaw) ;
			yaw+=(G_PI*2)/10;
		}
	}


	SINT i = GAME.GetGameTurn()%10 ;
	mOrientation2 = pre_cal_mat[i] ;
	SUPERTYPE::Process() ;
}


