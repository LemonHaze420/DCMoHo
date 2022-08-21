#include	"Common.h"

#include	"Player.h"
#include	"Globals.h"
#include	"Primary.h"


//******************************************************************************************
BOOL	CPlayer::Init()
{
	// initialise state
	mActive = FALSE;		// activate later

	// initialise weapons
//	mCurrentWeapon = OID_CMPAverage;
	mCurrentWeapon = OID_CMPBomb;

	// initialise cursor
	mCursorState					= CS_NOT_ACTIVE;
	mAnalogCursor					= GVector(G(5), G(5), G0);
	mAnalogCursorVelocity			= ZERO_GVECTOR;
	mHasAnalogCursorFoundPrimary	= FALSE;
	mCursorYaw						= G0;
	mCursorDist						= G1;
	mLevelResult					= LR_NONE;
	mResultTime						= -1;

	// initialise beam
	mPickupBeamState = NO_BEAM;
	mPickupBeamSize = G0;
	mPickupBeamStartTime = 0;

	// relative joystick mode..
	mRelX = G0;
	mRelY = G0;

	// Racer Stuff
	mRacerBoost = -1;
	mReleasedBlock = TRUE ;

	mPrimary = NULL ;
	return TRUE;
};

//******************************************************************************************
void	CPlayer::SetLevelResult(ELevelResult result)
{
	mLevelResult = result;

	if (mResultTime == -1)
		mResultTime = GAME.GetGameTurn();

	// crowd?
	if (result == LR_WIN)
		GAME.CrowdEvent(G(2), G1);

}

//******************************************************************************************
void	CPlayer::ProcessAction(EAction action, GINT v1, GINT v2)
{
	CPrimary	*p = WORLD.GetPrimaryThing(mNumber);

	// if knocked out make pushing buttons recover faster
	if (p && p->GetState() == CS_KNOCKED_OUT)
	{
		// pushed any of the direct buttons
		if (action >=A_DIRECT_JUMP)
		{
			p->ReduceKnockoutTime(2) ;
		}
		return ;
	}


	if (p && p->GetState() != CS_DEAD && p->GetRenderAnimation() != AM_CHARGE && p->GetState() != CS_LASERED )
	{
		switch(action)
		{
		case CHANGE_WEAPON:
			switch(mCurrentWeapon)
			{
			case OID_CMPAverage:
				mCurrentWeapon = OID_CMPLower;
//				CONSOLE.AddString("Lower Selected");
				break;
			case OID_CMPLower:
				mCurrentWeapon = OID_CMPBomb;
//				CONSOLE.AddString("Bomb Selected");
				break;
			case OID_CMPBomb:
				mCurrentWeapon = OID_CMPAverage;
//				CONSOLE.AddString("Average Selected");
				break;
			};
			break;

		case SECONDARY_FIRE:
/*			{
				SINT	x = GetCursor().X;
				SINT	y = GetCursor().Y;

				if (x != -1)
				{
					CThing *t = SpawnThing(mCurrentWeapon);
					if (t)
					{
						CInitThing i;
						i.mPos = GVector(G(x), G(y), MAP.GetMapHeight(GVector(G(x), G(y), G0)));
						t->Init(&i);
					}
				}
			}*/
			break;

		// Shuttle stuff
	#define THRUST_UP_POWER			(-G(0, 2000))
	#define THRUST_ALONG_POWER		(G(0, 300))
	#define THRUST_CEILING			(-G(5))
	#define THRUST_ANTI_GRAV		(-G(0, 1000))

		case A_THRUST_DEFAULT:
			p->AddVelocity(GVector(G0, G0, THRUST_ANTI_GRAV));
			break;

		case A_THRUST_UP:
			{
				GINT h = G1 + (THRUST_CEILING - p->GetPos().Z) / THRUST_CEILING;
				if (h < G0)
					h = G0;
				if (h > G1)
					h = G1;
				h *= h; // square for stronger ceiling
//				h *= h; // and again

				p->AddVelocity(GVector(G0, G0, h * THRUST_UP_POWER));
				break;
			}

		case A_THRUST_ALONG:
			p->AddVelocity(GVector(v1 * THRUST_ALONG_POWER, v2 * THRUST_ALONG_POWER, G0));
			break;


		// ****************************8
		// ** Racer stuff

//#define RACER_ALONG_POWER		(G(0, 900))
//#define RACER_ALONG_POWER2		(G(0, 300))
#define RACER_ALONG_POWER		(G(0, 1200))
#define RACER_ALONG_POWER2		(G(0, 800))

#define RACER_BOOST_POWER		(G(0, 40000))
#define RACER_BOOST_MAX			70
#define RACER_BOOST_DELAY		20

		case A_RACER_BOOST_START:
			ASSERT(mRacerBoost < 0);
			if (mRacerBoost == -1)
				mRacerBoost = 0;
			break;

		case A_RACER_BOOST_END:
			if (mRacerBoost > -1)
			{
				GMatrix m;
				m.MakeRotationYaw(p->GetYaw());
				GVector v = GVector(G0, (G(mRacerBoost) / RACER_BOOST_MAX) * RACER_BOOST_POWER, G0);
				v = m * v;

				p->SetVelocity(p->GetVelocity() + v);
				mRacerBoost = -RACER_BOOST_DELAY;
			}
			break;

		case A_RACER_ALONG:
			if (p->mFlags & TF_ON_GROUND)
				p->Thrust(GVector(v1 * RACER_ALONG_POWER, v2 * RACER_ALONG_POWER, G0));
			else
				p->Thrust(GVector(v1 * RACER_ALONG_POWER2, v2 * RACER_ALONG_POWER2, G0));

			// also process boost here
			if (mRacerBoost != -1)
				mRacerBoost ++;
			if (mRacerBoost > RACER_BOOST_MAX)
			{
				ProcessAction(A_RACER_BOOST_END, v1, v2);
			}
			break;

// *****************************
// Direct Stuff
		case A_DIRECT_JUMP:
			p->Jump();
			break;

		case A_DIRECT_ATTACK0:
			p->Attack(0);
			break;

		case A_DIRECT_SPIN_ATTACK:
		{
			if (mReleasedBlock == TRUE)
			{
				if (p->Attack(1) )
				{
					mReleasedBlock = FALSE ;
				}
			}
			break ;
		}

		case A_DIRECT_START_BLOCK:
			if (p->GetBrakePower() != G0 && !p->IsBlocking() ) p->StartBlocking();
			break;

		case A_DIRECT_END_BLOCK:
		{
			mReleasedBlock=TRUE;
			p->StopBlocking();
			break;
		}

		case A_DIRECT_THROW:
			p->StartThrowingAnimation() ;
			break ;

		case A_DIRECT_TRICK0:
			p->Trick(0);
			break;

		case A_DIRECT_START_RACER_BOOST:
			p->RacerBoost(GVector(v1 * RACER_ALONG_POWER, v2 * RACER_ALONG_POWER, G0)) ;
			break ;

		default:
			ASSERT(0);
			CONSOLE.AddString("Unknown Action!");
		};
	}
	else if (p && p->GetState() != CS_DEAD && p->GetRenderAnimation() == AM_CHARGE  && action == A_DIRECT_JUMP)
	{
		// quick hack to allow you to jump out of charge attacks
		p->Jump();
	}
}
//******************************************************************************************
GINT	CPlayer::GetBoostVal()
{
	if (mRacerBoost >= 0)
		return G(mRacerBoost) / RACER_BOOST_MAX;
	else
		return -(G(mRacerBoost + 1) / RACER_BOOST_DELAY);
}

 //******************************************************************************************
//******************************************************************************************
#ifdef LINK_EDITOR

void	CPlayer::DeclareSpawnees()
{
	WORLD.AccumulateOIDResources(OID_CMPLower);
	WORLD.AccumulateOIDResources(OID_CMPBomb);
	WORLD.AccumulateOIDResources(OID_CMPAverage);
}
#endif

//******************************************************************************************
void	CPlayer::Process()
{
	if (!mActive)
		return;

	// kill the beam after level victory?
	if (!(CONTROLS.mControllers[mNumber].IsActive()))
	{
		DeActivateCursor();
	}

	ProcessCursor();

}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//***
//*** Cursor stuff
//***
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

#define	CURSOR_SPEED	G(0, 20000)
#define CURSOR_DAMPING  G(0, 30000)

//******************************************************************************************
// vithout velocity
//#define	PICKUP_TOOL_STRENGTH  G(-12)
//#define PICKUP_TOOL_DAMPING GINT(0, 13107)

#define	PICKUP_TOOL_STRENGTH  G(-5)
#define	PICKUP_TOOL_STRENGTH_ALT  G(-3)
#define PICKUP_TOOL_DAMPING GINT(0, 13107)

#define	PICKUP_TOOL_STRENGTH_SINT  (-5<<(HEIGHT_SHIFT))
#define	PICKUP_TOOL_STRENGTH_ALT_SINT  (-3<<HEIGHT_SHIFT)
#define PICKUP_TOOL_DAMPING_SINT (13107>>(GINT_SHIFT-HEIGHT_SHIFT))



// with velocity
//#define	PICKUP_TOOL_STRENGTH  G(-4)
//#define PICKUP_TOOL_DAMPING GINT(0, 13107)

//******************************************************************************************
void	CPlayer::ProcessCursor()
{
	// move cursor if in manual control
	switch (CONTROLS.GetControllerMode())
	{
	case CM_JOY_ALT1:
	case CM_KEY_ALT1:
	case CM_KEYBOARD:
	case CM_JOYPAD:
		if (!mHasAnalogCursorFoundPrimary)
		{
			// try to find the Primary Ball, so we can start near it!
			CPrimary *t;

			if ((t = WORLD.GetPrimaryThing(mNumber)) != NULL)
			{
				mAnalogCursor = t->GetPos(); // + GVECTOR_HALF;
				mHasAnalogCursorFoundPrimary = TRUE;
			}
		}

		mAnalogCursorVelocity *= CURSOR_DAMPING;
		mAnalogCursorVelocity.Y += CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CURSOR_SPEED;
		mAnalogCursorVelocity.X += CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CURSOR_SPEED;

		mAnalogCursor += mAnalogCursorVelocity;

		if (mAnalogCursor.X < G0) mAnalogCursor.X = G0;
		if (mAnalogCursor.Y < G0) mAnalogCursor.Y = G0;
		if (mAnalogCursor.X > G(MAP_WIDTH  - 1)) mAnalogCursor.X = G(MAP_WIDTH  - 1);
		if (mAnalogCursor.Y > G(MAP_HEIGHT - 1)) mAnalogCursor.Y = G(MAP_HEIGHT - 1);
		
		// ok - now we need to clamp the cursor near the ball.
		{
			CPrimary *t;
	
			if ((t = WORLD.GetPrimaryThing(mNumber)) != NULL)
			{
				GVector bpos = t->GetPos();
				GVector delta = mAnalogCursor - bpos;
				delta.Z = G0;
				
				GINT	mag = delta.Magnitude();
				
				if (mag > G(4))
				{
					mAnalogCursor = bpos + (delta * (G(4) / mag));
				}
			}
		}

		break;
	case CM_JOY_REL:
		CPrimary *t;

		if ((t = WORLD.GetPrimaryThing(mNumber)) != NULL)
		{
			mAnalogCursor = t->GetPos(); // + GVECTOR_HALF;
			mHasAnalogCursorFoundPrimary = TRUE;
		}
		mRelX += CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CURSOR_SPEED;
		mRelY += CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CURSOR_SPEED;
		mAnalogCursor.X+=mRelX;
		mAnalogCursor.Y+=mRelY;

		if (mAnalogCursor.X < G0) mAnalogCursor.X = G0;
		if (mAnalogCursor.Y < G0) mAnalogCursor.Y = G0;
		if (mAnalogCursor.X > G(MAP_WIDTH  - 1)) mAnalogCursor.X = G(MAP_WIDTH  - 1);
		if (mAnalogCursor.Y > G(MAP_HEIGHT - 1)) mAnalogCursor.Y = G(MAP_HEIGHT - 1);

		break;
	case CM_JOY_GOLF:
		{
#define	CURSOR_YAW_SPEED	G(0, 3000)
#define	CURSOR_DIST_SPEED	G(0, 10000)
	
			if (!(CONTROLS.IsButtonPressed(BUTTON_FIRE_PRIMARY, mNumber)))
			{
				mCursorYaw  -= CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CURSOR_YAW_SPEED;
				mCursorDist -= CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CURSOR_DIST_SPEED;

				if (mCursorDist < G(1, 10000)) mCursorDist = G(1, 10000);
				if (mCursorDist > G(4)) mCursorDist = G(4);

				GVector p = WORLD.GetPrimaryPosition();
				mAnalogCursor.X = p.X - mCursorDist * GSIN(mCursorYaw);
				mAnalogCursor.Y = p.Y - mCursorDist * GCOS(mCursorYaw);
				mAnalogCursor.Z = p.Z;
			}
		}
		break;

	default:
		break;

	};

	ProcessPickupTool();

#define BEAM_SIZE	G(10)

	// Beam Effect
	if (CONTROLS.GetControllerMode() == CM_JOY_GOLF)
	{
		if (mCursorGolfMode == CGM_TOPPED)
			mPickupBeamSize = BEAM_SIZE;
		else
			mPickupBeamSize = (G(mCursorStateTime) * BEAM_SIZE) / 25;
	}
	else
	{
		if (mPickupBeamState == GROWING)
		{
			mPickupBeamSize += G1;
			if (mPickupBeamSize >= BEAM_SIZE)
				mPickupBeamState = FULL_BEAM;
		}
		if (mPickupBeamState == SHRINKING)
		{
			mPickupBeamSize -= G1;
			if (mPickupBeamSize <= G0)
				mPickupBeamState = NO_BEAM;
		}
	}
}


void	CPlayer::ProcessPickupTool()
{
	EvaluateCursorPosition();

	switch(CONTROLS.GetControllerMode())
	{
		// various control mechanisms

	case CM_JOY_ALT1:
	case CM_KEY_ALT1:
	case CM_JOY_REL:
		{
			// ALT 1 - pickup 4 points on a sub square basis.
			GINT power = G0;
			if (mCursorState == CS_ACTIVE_TOOL)
			{
				power = PICKUP_TOOL_STRENGTH_ALT;
			}
			else if (mCursorState == CS_SHRINKING_TOOL)
			{
				if ((-- mCursorStateTime) > 0)
				{
					power = (PICKUP_TOOL_STRENGTH_ALT * mCursorStateTime) / 10;
				}
				else
					mCursorState = CS_NOT_ACTIVE;
			}
			if (power != G0)
			{
				PickupSubCell(mAnalogCursor, power);

/*				// get line
				GVector l = mAnalogCursor - WORLD.GetPrimaryPosition();
				GVector r;
				if (l.MagnitudeSq() > G(0, 100))
				{
					r = l ^ GVector(G0, G0, -G1);
					r.Normalise();
				}
				else
					r = GVector(G1, G0, G0);

//				GVector r2 = r * 2;
				GVector r2 = r;
				l.Normalise();
				l *= 2;

//				PickupSubCell(mAnalogCursor + r, power);
//				PickupSubCell(mAnalogCursor - r, power);
//				PickupSubCell(mAnalogCursor + r2, power);
//				PickupSubCell(mAnalogCursor - r2, power);
//				PickupSubCell(mAnalogCursor + r2 + l, -(power / 3));
//				PickupSubCell(mAnalogCursor      + l, -(power / 3));
//				PickupSubCell(mAnalogCursor - r2 + l, -(power / 3));
//				PickupSubCell(mAnalogCursor + r2 - l, -power / 3);
//				PickupSubCell(mAnalogCursor      - l, -power / 3);
//				PickupSubCell(mAnalogCursor - r2 - l, -power / 3);
*/			}
		}
	case CM_JOY_GOLF:
		if (mCursorState == CS_ACTIVE_TOOL)
		{
			if (mCursorGolfMode == CGM_UP)
			{
				mCursorStateTime ++;
				if (mCursorStateTime > 25)
				{
					mCursorStateTime = 4;
					mCursorGolfMode = CGM_TOPPED;
				}
			}
			else if (mCursorGolfMode == CGM_TOPPED)
			{
				mCursorStateTime --;
				if (mCursorStateTime < 0)
				{
					mCursorStateTime = 25;
					mCursorGolfMode = CGM_DOWN;
				}
			}
			else
			{
				mCursorStateTime -= 3;
				if (mCursorStateTime < 1)
				{
					mCursorStateTime = 1;
					mCursorGolfMode = CGM_UP;
				}
			}
		}

		break;

	default:
		if (mCursorState == CS_ACTIVE_TOOL)
		{
			// pick up landscape
			CMapCell *c = MAP.GetCell(mPickup);

			if (c->CanIMove())
			{

				//SINT	o = c->mOffset;
				//c->mOffset += (((PICKUP_TOOL_STRENGTH_SINT - o) * PICKUP_TOOL_DAMPING_SINT)>>HEIGHT_SHIFT);
				//c->mOffsetVelocity = 0;

				c->AddOffset((PICKUP_TOOL_STRENGTH -c->GetOffset())*PICKUP_TOOL_DAMPING);
				c->mOffsetVelocity = 0;

//				c->mOffsetVelocity = c->mOffset - o;
			}
		}
		else if (mCursorState == CS_SHRINKING_TOOL)
		{
			if ((-- mCursorStateTime) > 0)
			{
				CMapCell *c = MAP.GetCell(mPickup);

				if (c->CanIMove())
				{
					//SINT	o = c->mOffset;
					c->AddOffset ((((PICKUP_TOOL_STRENGTH *mCursorStateTime)/10)-c->GetOffset())*PICKUP_TOOL_DAMPING);
					//c->mOffset += (((((PICKUP_TOOL_STRENGTH_SINT * mCursorStateTime)) / 10 ) - c->mOffset) * PICKUP_TOOL_DAMPING_SINT)>>(HEIGHT_SHIFT);
					c->mOffsetVelocity = 0;
				}
			}
			else
				mCursorState = CS_NOT_ACTIVE;
		}
	}
}

//******************************************************************************************
void	CPlayer::ActivateCursor()
{
	switch (CONTROLS.GetControllerMode())
	{

	case CM_JOY_GOLF:
		// GOLFNEW
		// can I play a shot?
		if (GAME.GolfCanIPlay(mNumber))
		{
			mCursorState = CS_ACTIVE_TOOL;
			mCursorStateTime = 0;
			mCursorGolfMode = CGM_UP;

			// enable beam effect
			mPickupBeamState = GROWING;
			mPickupBeamSize = G0;
			mPickupBeamStartTime = GAME.GetGameTurn();
		}
		break;

	default:
		{
			ASSERT((mCursorState == CS_NOT_ACTIVE) || (mCursorState == CS_SHRINKING_TOOL));

			// look to see if there is something nearby to be pressed
			//! should be mapwhos...
//			CThing	*t;
//			GVector pos = mCursor.ToGVector(); // + GVector(GINT_HALF, GINT_HALF, G0);
//			t = FindThingUnderCursor();
//			if (t)
//			{
//				t->Activate();
//				mCursorState = CS_ACTIVE_OTHER;
//				return;
//			}

			// didn't find anything - so pick up landscape
			if (mCursor.X != -1)
			{
				mCursorState = CS_ACTIVE_TOOL;
				mPickup = mCursor;
				mAnalogPickup = mAnalogCursor;

				// enable beam effect
				mPickupBeamState = GROWING;
				mPickupBeamSize = G0;
				mPickupBeamStartTime = GAME.GetGameTurn();
			}
		}
	}
}

//******************************************************************************************
void	CPlayer::PlayShot()
{
	if (!mPrimary)
		return;

	GVector	v = mPrimary->GetPos() - mAnalogCursor;
	GINT	l = v.Magnitude();
	v.Normalise();

	GVector	d = GVector(v.X, v.Y, -(G(2) / (l - G(0, 30000))));
	d.Normalise();

	GINT	f;
	if (mCursorGolfMode == CGM_TOPPED)
		f = G(25);
	else
		f = G(mCursorStateTime);
	d *= f / 24;

	mPrimary->SetVelocity(d);
	mPrimary->mFlags &= ~TF_ON_GROUND;

	// set spin
	GVector spax = d ^ GVector(G0, G0, -G1);

	spax /= -G(5);

	mPrimary->SetSpin(spax);

	MAP.Raindrop(CCellRef(mAnalogCursor), -(G((mCursorGolfMode == CGM_TOPPED) ? 25 : mCursorStateTime)) / 70);

	GAME.GolfDeclarePlay(mNumber);
	GAME.GolfDeclareLastShot(mNumber, mPrimary->GetPos());
}

//******************************************************************************************
void	CPlayer::DeActivateCursor()
{
	if (CONTROLS.GetControllerMode() == CM_JOY_GOLF)
	{
		if (mCursorState == CS_ACTIVE_TOOL)   // otherwise, fire wasn't allowed.
		{
			PlayShot();
			mCursorStateTime = 0;
			mPickupBeamState = NO_BEAM;
		}
	}

	if (mCursorState == CS_ACTIVE_TOOL)
	{
		mCursorState = CS_SHRINKING_TOOL;
		mCursorStateTime = 10;
	}
	else
		mCursorState = CS_NOT_ACTIVE;

	// deactivate beam if necessary.
	if (mPickupBeamState != NO_BEAM)
	{
		mPickupBeamState = SHRINKING;
	}
}


//******************************************************************************************
void	CPlayer::RecentreCursor()
{
	mHasAnalogCursorFoundPrimary = FALSE;
}

//******************************************************************************************
//** Process the mouse pickup tool

//******************************************************************************************
void	CPlayer::EvaluateCursorPosition()
{
	CCamera	*cam = GAME.GetCamera(mNumber);

	switch (CONTROLS.GetControllerMode())
	{
#if TARGET == PC
		//!PSX - maybe move this into PCMap??
	case CM_MOUSE:
		{
			if (mCursorState == CS_ACTIVE_TOOL)
				return; // don't bother

			POINT	p = CONTROLS.GetMousePos(mNumber);

			GVector d = ENGINE.GetCursorOffsetMatrix(p.x, p.y) * GVector(G0, G0, G1);
			d = cam->mOrientation * d;

			GVector v = MAP.FindLineIntersection(cam->mPos, cam->mPos + d);

			if (v.X != G(-1))
			{
				//!hmm
				mCursor = CCellRef(v);
			}
			else
				mCursor = CCellRef(-1, -1); //  no cursor available.
		}
		break;
#endif

	case CM_JOY_ALT1:
	case CM_KEY_ALT1:
	case CM_KEYBOARD:
	case CM_JOYPAD:
	case CM_JOY_REL:
		mCursor = CCellRef(mAnalogCursor + GVECTOR_HALF);

		//!
		mPickup = mCursor;
		mAnalogPickup = mAnalogCursor;
		break;
	case CM_JOY_GOLF:
		break;
	default:
		mCursor.X = mCursor.Y = 32;
		break;
	};

}

//******************************************************************************************
CThing *CPlayer::FindThingUnderCursor()
{
	// look to see if there is something nearby to be pressed
	CThing	*t;
	GVector pos = mCursor.ToGVector(); // + GVector(GINT_HALF, GINT_HALF, G0);

	t = MAP.GetFirstThing(pos, G1);

	while (t)
	{
		// compare to pos...
		GVector dist = pos - t->mPos;
		GINT r = t->Radius();

		if (r < G1) r = G1; // otherwise you'll never be able to select small things...

		if (dist.MagnitudeSqXY() < r * r)
		{
			if (t->CanIBeActivated())
			{
				return t;
			}
		}

		t = MAP.GetNextThing();
	};

	return t; // == NULL
}

//******************************************************************************************

// NB - this is a static function.
void	CPlayer::PickupSubCell(const GVector &v, GINT power)
{
#if TARGET == PSX
	if (GAME.IsMultiPlayer())
		return;
#endif

	GINT	f1 = v.X.GFrac();
	GINT	f2 = v.Y.GFrac();

	CCellRef	cr = CCellRef(v);

	CMapCell *c;


	if ((cr.X > 0) && (cr.Y > 0))
	{
		c = MAP.GetCell(cr);
		if (c->CanIMove())
		{
			GINT	o = c->GetOffset();
			c->AddOffset((power - o) * PICKUP_TOOL_DAMPING * GROOT((G1 - f1) * (G1 - f2)));
			c->SetOffsetVelocity(c->GetOffset() - o);
		}
	}
	if ((cr.X < MAP_WIDTH - 2) && (cr.Y > 0))
	{
		c = MAP.GetCell(CCellRef(cr.X + 1, cr.Y));
		if (c->CanIMove())
		{
			GINT	o = c->GetOffset();
			c->AddOffset((power - o) * PICKUP_TOOL_DAMPING * GROOT((f1) * (G1 - f2)));
			c->SetOffsetVelocity(c->GetOffset() - o);
		}
	}
	if ((cr.X > 0) && (cr.Y < MAP_HEIGHT - 2))
	{
		c = MAP.GetCell(CCellRef(cr.X, cr.Y + 1));
		if (c->CanIMove())
		{
			GINT	o = c->GetOffset();
			c->AddOffset((power - o) * PICKUP_TOOL_DAMPING * GROOT((G1 - f1) * (f2)));
			c->SetOffsetVelocity(c->GetOffset() - o);
		}
	}
	if ((cr.Y < MAP_HEIGHT - 2) && (cr.X < MAP_WIDTH - 2))
	{
		c = MAP.GetCell(CCellRef(cr.X + 1, cr.Y + 1));
		if (c->CanIMove())
		{
			GINT	o = c->GetOffset();
			c->AddOffset((power - o) * PICKUP_TOOL_DAMPING * GROOT((f1) * (f2)));
			c->SetOffsetVelocity(c->GetOffset() - o);
		}
	}
}






