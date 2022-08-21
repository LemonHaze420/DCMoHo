#include	"Common.h"

#include	"Camera.h"
#include	"Globals.h"
#include	"Editors.h"
#include	"Primary.h"

#if TARGET == PC
#include	"PCBuildType.h"
#elif TARGET == DC
#include	"DCBuildType.h"
#elif TARGET == PSX
#include	"PSXBuildType.h"
#endif


#define		IDEAL_PITCH			G(0, 51472)
#define		CAMERA_MOMENTUM		GINT(0, 13107)
#define		PITCH_SPEED_DROP	G(0, 6000)
#define		PITCH_SPEED_RISE	G(0, 6000)
#define		MAX_PITCH			G_PI_2

BOOL	first_run_through = TRUE;

//******************************************************************************************
CCamera::CCamera()
{
};

//******************************************************************************************
void	CCamera::Reset(SINT n)
{
	mNumber = n;

#ifdef LINK_EDITOR
	if (EDITOR.IsActive())
		PanOut();
	else
#endif
	{
		// start of level things
		mLookAt = GVector(G(MAP_WIDTH / 2), G0, -G(15));
		mLinearOffset = G(21);
		mPos = GVector(G0, -mLinearOffset, G0);  // well, we've got to start somewhere...

		mOrientation = ID_GMATRIX;
		mMode = LOOKAT;
//		mMode = ORIENTATION;

		mWinner = NULL ;
		mDestLookAt = mLookAt;
		mDestLinearOffset = mLinearOffset;

		mYaw = mDestYaw = G0;
		mFollowToggle = TRUE;

		if (first_run_through)
			mPitch = mDestPitch = -G_PI_2;
		else
			mPitch = mDestPitch = IDEAL_PITCH;
		
		FillOut();

		mLockToggle = FALSE;
		
		// JCL - interpolation reset
		mOldPos = mPos;
		mOldLookAt = mLookAt;
		mOldOrientation = mOrientation;
	}
}
//******************************************************************************************

void	CCamera::PanOut()
{
	mDestLookAt = GVector(G(MAP_WIDTH / 2), G(MAP_HEIGHT / 2), G0);
	mDestLinearOffset = G(63);

	mFollowToggle = FALSE;

	mDestYaw = G0;
}

//******************************************************************************************
void	CCamera::PanIn()
{
	mDestLookAt = WORLD.GetPrimaryPosition(mNumber);
	mDestLinearOffset = G(21);

	mFollowToggle = TRUE;
}

//******************************************************************************************
void	CCamera::Update()
{
	// JCL 18-8 - backup for interpolation first
	mOldPos = mPos;
	mOldLookAt = mLookAt;
	mOldOrientation = mOrientation;

	mMode = CCamera::LOOKAT;

	// Move the camera?
#define	CAMERA_SPEED G1
#define CAMERA_ROT_SPEED  G(0, 8000)

	GINT	dx = G0;
	GINT	dy = G0;
	GINT	dz = G0;

	//**********************************
	//** Manual camera movement

#if TARGET == PC || TARGET == DC
	// rotation
	if (CONTROLS.IsButtonPressed(BUTTON_SPIN_CLOCKWISE, mNumber))
		mDestYaw -= CAMERA_ROT_SPEED;

	if (CONTROLS.IsButtonPressed(BUTTON_SPIN_ANTICLOCKWISE, mNumber))
		mDestYaw += CAMERA_ROT_SPEED;

	if (CONTROLS.IsButtonPressed(BUTTON_SPIN_CENTRE, mNumber))
		mDestYaw = G0;
#endif

#if 0
	if (PLAYER(0)->GetPrimary() && GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER)
	{
		//if (PLAYER(0)->GetPrimary()->GetFlags() & TF_ON_GROUND)
		{

			GINT your_ang = PLAYER(0)->GetPrimary()->GetYaw() ;

			GVector vel = PLAYER(0)->GetPrimary()->GetVelocity() ;
			GINT ang  = -GATAN2(vel.X, vel.Y) ;

			GINT angle_diff = AngleDifference(ang, your_ang) ;

		//	mDestYaw = your_ang + (angle_diff >> 1 )  ;

			mDestYaw = your_ang  ;

			// hmmmm  !!
			if (mDestYaw > G_PI_2 && mYaw < -G_PI_2 )
			{
				mYaw+=G_2PI ;
			}

			if (mDestYaw < -G_PI_2 && mYaw > G_PI_2 )
			{
				mYaw-=G_2PI ;
			}
		}
	}
#endif

#if TARGET == PC || TARGET == DC
	if (CONTROLS.IsButtonPressed(BUTTON_ZOOM_OUT, mNumber))
		mDestLinearOffset += CAMERA_SPEED;
	if (CONTROLS.IsButtonPressed(BUTTON_ZOOM_IN, mNumber))
		mDestLinearOffset -= CAMERA_SPEED;

	if (CONTROLS.IsButtonPressed(BUTTON_WIDE_VIEW, mNumber))
		PanOut();
	if (CONTROLS.IsButtonPressed(BUTTON_TIGHT_VIEW, mNumber))
		PanIn();
#endif

	SINT	movement_mode = -1;


	if ((!(GAME.IsGamePanelActive())) && (!(GAME.IsHelp())))
	{
		// this is set when we want the camera to pan onto the winner ( may be a bad guy ) 
		// e.g. in king hill levels
		if (mWinner!= NULL)
		{
			movement_mode = 6 ;
		}
		//! Think about multiplayer golf
#ifdef FREE_LOOK
		else if (GAME.IsPaused()) // && GAME.IsGameRunning())
			movement_mode = 7;
#endif
		else if ((!(GAME.IsGameRunning()))
#ifdef LINK_EDITOR
		 && (!(EDITOR.IsActive()))
#endif
			)     // sorry!
		{
			movement_mode = 5;
		}
		else if (GAME.IsPaused() || ((GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF) && (CONTROLS.IsButtonPressed(BUTTON_FIRE_PRIMARY))))
			movement_mode = 2;
		else
		{
			switch (CONTROLS.GetControllerMode())
			{
			case CM_MOUSE:
				movement_mode = 0;
				break;

			case CM_JOY_REL:
			case CM_KEY_ALT1:
			case CM_JOY_ALT1:
			case CM_KEYBOARD:
			case CM_JOYPAD:
				movement_mode = 1;
				break;

			case CM_JOY_GOLF:
				movement_mode = 3;
				break;

			case CM_JOY_SHUTTLE:
			case CM_JOY_RACER:
			case CM_JOY_DIRECT:
			case CM_KEY_DIRECT:
				movement_mode = 4;
			}
		}
	}

	switch(movement_mode)
	{
	case 0:
		{
			dx +=   CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CAMERA_SPEED * GCOS(mYaw) -
				    CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CAMERA_SPEED * GSIN(mYaw);
			dy +=   CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CAMERA_SPEED * GSIN(mYaw) +
				    CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CAMERA_SPEED * GCOS(mYaw);

			// or mouse stuff
#ifndef _DEBUG  //! should be if Windowed mode...
#ifdef LINK_EDITOR
			if (!(EDITOR.IsActive()))
#endif
			{
				POINT	p = CONTROLS.GetMousePos(mNumber);

				if (p.x == 0)
					dx -= CAMERA_SPEED;
				if (p.x == PLATFORM.ScreenWidth() - 1)
					dx += CAMERA_SPEED;
				if (p.y == 0)
					dy += CAMERA_SPEED;
				if (p.y == PLATFORM.ScreenHeight() - 1)
					dy -= CAMERA_SPEED;
			}
#endif
			if (CONTROLS.IsButtonPressedDB(BUTTON_FOLLOW_TOGGLE, mNumber))
				mFollowToggle = !mFollowToggle;

			// update dest
			if (mFollowToggle)
			{
				GVector d = WORLD.GetPrimaryPosition(mNumber);

				MAP.ClipPosition(d);

				mDestLookAt = d;
			}
			else
				mDestLookAt += GVector(dx, dy, dz);
			}
		break;

	case 1:
		{
			//! think about this
			GVector pri = WORLD.GetPrimaryPosition(mNumber);
			GVector cur = PLAYER(0)->GetAnalogCursor();
			cur.Z = MAP.GetMapHeight(cur);

			GINT delx = (cur.X - pri.X) / 2;
			GINT dely = (cur.Y - pri.Y) / 2;
			GINT delz = (cur.Z - pri.Z) / 2;

			if (delx >  G(10)) delx =  G(10);
			if (dely >  G(10)) dely =  G(10);
			if (delx < -G(10)) delx = -G(10);
			if (dely < -G(10)) dely = -G(10);

			mDestLookAt = pri + GVector(delx, dely, delz);


		}
		break;

	case 2:
		{
			dx +=   CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CAMERA_SPEED * GCOS(mYaw) -
				    CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CAMERA_SPEED * GSIN(mYaw);
			dy +=   CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CAMERA_SPEED * GSIN(mYaw) +
				    CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CAMERA_SPEED * GCOS(mYaw);

			mDestLookAt += GVector(dx, dy, dz);

			if (mDestLookAt.X < G(3))
				mDestLookAt.X = G(3);
			if (mDestLookAt.Y < G(10))
				mDestLookAt.Y = G(10);
			if (mDestLookAt.X > G(MAP_WIDTH - 3))
				mDestLookAt.X = G(MAP_WIDTH - 3);
			if (mDestLookAt.Y > G(MAP_HEIGHT - 3))
				mDestLookAt.Y = G(MAP_HEIGHT - 3);
		}
		break;

	case 3:
		{
			// Golf

			GVector		v = WORLD.GetPrimaryPosition(mNumber) - PLAYER(0)->GetAnalogCursor();
			v.Normalise();

			mDestLookAt = WORLD.GetPrimaryPosition(mNumber) + v * G(4);
			mDestLinearOffset = G(21) ;
			if (mDestLookAt.Y < G(13))
			{
				if (mDestLookAt.Z > G(4))
				{
					mDestLinearOffset = G(21) + (mDestLookAt.Z - G(4) ) ;
					mDestPitch = G(0,51472) + (G(0,4251) * (G(13) - mDestLookAt.Y)) ;
				}
				else
				{
					mDestPitch = G(0,51472) + (G(0,2551) * (G(13) - mDestLookAt.Y)) ;
				}
			}
		}
		break;

	case 4:
		// shuttle / direct etc
		if (mLockToggle)
			EvaluateLockAngles();
		else
		{
			GVector d = WORLD.GetPrimaryPosition(mNumber);
			d.Z -= G1;

			CPrimary *p = WORLD.GetPrimaryThing(mNumber);
			if (p)
			{
				// suspension?!
				d.Z += p->GetSuspension();

				GVector v = p->GetVelocity();
				v.Z = G0;
				if (GAME.IsMultiPlayer())
				{
					if (v.X > G1)
						v.X = G1;
					if (v.X < -G1)
						v.X = -G1;
					
					d += v * G(10);
				}
				else
					d += v * G(5);
			}

			MAP.ClipPosition(d);

			mDestLookAt = d;

#define CAM_BUGGER_FACTOR	(G(10, 42549) - G(5))

			if (mDestLookAt.Y - mDestLookAt.Z < CAM_BUGGER_FACTOR)
			{
//				mDestLookAt.Y = CAM_BUGGER_FACTOR + mDestLookAt.Z;
				
				// (JCL) - need to tweak this so the rotation at start of level
				// doesn't put the character behind the start lights.				
				
				GINT	desired_dest_y = CAM_BUGGER_FACTOR + mDestLookAt.Z;
				GINT	delta = desired_dest_y - mDestLookAt.Y;
				
				mDestLookAt.Y += delta * ((G_PI - GABS(mYaw)) / G_PI);
			}
				
			//!!
#if 0
			CPrimary *p = WORLD.GetPrimaryThing(mNumber);
			if (p)
			{
				GINT	dyaw;

//				dyaw = p->GetYaw();

				GVector v = p->GetVelocity();
				if (v.MagnitudeSqXY() > G(0, 10))
					dyaw = GATAN2(v.Y, v.X) - G_PI_2;
				else
					dyaw = mDestYaw;


				// stop jerking
				if (dyaw - mDestYaw > G_PI)
					mYaw += G_PI * 2;
				if (dyaw - mDestYaw < -G_PI)
					mYaw -= G_PI * 2;
				mDestYaw = dyaw;
			}
#endif
		}
		break;

	case 5:
		{
			// look at primary
			GVector d = WORLD.GetPrimaryPosition(mNumber);
			d.Z -= G1;
			CPrimary *p = WORLD.GetPrimaryThing(mNumber);
			if (p)
			{
				// suspension?!
				d.Z += p->GetSuspension();

				GVector v = p->GetVelocity();
				v.Z = G0;
				d += v * G(5);
			}
			MAP.ClipPosition(d);

			mDestLookAt = d;

#define CAM_BUGGER_FACTOR	(G(10, 42549) - G(5))

			if (mDestLookAt.Y - mDestLookAt.Z < CAM_BUGGER_FACTOR)
			{
//				mDestLookAt.Y = CAM_BUGGER_FACTOR + mDestLookAt.Z;
				
				// (JCL) - need to tweak this so the rotation at start of level
				// doesn't put the character behind the start lights.				
				
				GINT	desired_dest_y = CAM_BUGGER_FACTOR + mDestLookAt.Z;
				GINT	delta = desired_dest_y - mDestLookAt.Y;
				
				mDestLookAt.Y += delta * ((G_PI - GABS(mYaw)) / G_PI);
			}
				

			// START OF LEVEL SEQUENCE
			GINT	c;
			GINT	gsc;
			
			if (GAME.GetGameState() == GAME_STATE_STARTING)
				gsc = G(GAME.GetGameStateCounter());
			else
				gsc = G(200); // max out

			if ((first_run_through) && (GAME.GetIVar(IV_LEVEL_TYPE) != LT_RACER) && (!(GAME.IsMultiPlayer())))
			{
				c = gsc;
	//			c -= G(50);
				c /= G(80);
				if (c < G0) c = G0;
				if (c > G1) c = G1;
				c = ((G1 - c) * (G1 - c));
	 
				mDestYaw = c * G_PI;
	
				c = gsc;
	//			c -= G(25);
				c /= G(80);
				if (c < G0) c = G0;
				if (c > G1) c = G1;
				c = ((G1 - c) * (G1 - c));
				mPitch = mDestPitch = -(G_PI_2) + (IDEAL_PITCH + G_PI_2) * (G1 - c);		
			}
			else
			{
				c = gsc;
				c -= G(50);
				c /= G(50);
				if (c < G0) c = G0;
				if (c > G1) c = G1;
				c = ((G1 - c) * (G1 - c));
	 
				mDestYaw = c * G_PI;
				mPitch = mDestPitch = IDEAL_PITCH;
			}

			c = gsc;
			c -= G(50);
			c /= G(50);
			if (c < G0) c = G0;
			if (c > G1) c = G1;
			c = ((G1 - c) * (G1 - c));
			mDestLinearOffset = G(21) - (c * G(6));
			
			// not in race..
			if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER)
				mDestYaw = G0;

			mYaw = mDestYaw;
			mLinearOffset = mDestLinearOffset;
			mLookAt = mDestLookAt;
		}
		break;


		case 6:
		// winner
		if (mLockToggle)
			EvaluateLockAngles();
		else
		{
			GVector d = mWinner->GetPos();
			d.Z -= G1;

			if (mWinner)
			{
				// suspension?!
			//	d.Z += p->GetSuspension();

				GVector v = mWinner->GetVelocity();
				v.Z = G0;
				d += v * G(5);
			}

			MAP.ClipPosition(d);

			mDestLookAt = d;

#define CAM_BUGGER_FACTOR	(G(10, 42549) - G(5))

			if (mDestLookAt.Y - mDestLookAt.Z < CAM_BUGGER_FACTOR)
			{
//				mDestLookAt.Y = CAM_BUGGER_FACTOR + mDestLookAt.Z;
				
				// (JCL) - need to tweak this so the rotation at start of level
				// doesn't put the character behind the start lights.				
				
				GINT	desired_dest_y = CAM_BUGGER_FACTOR + mDestLookAt.Z;
				GINT	delta = desired_dest_y - mDestLookAt.Y;
				
				mDestLookAt.Y += delta * ((G_PI - GABS(mYaw)) / G_PI);
			}
				
			//!!
		}
		break;

		case 7:
			{
				// free look
				
				dx +=   CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CAMERA_SPEED * GCOS(mYaw) -
					    CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CAMERA_SPEED * GSIN(mYaw);
				dy +=   CONTROLS.GetAnalogueValue(ANALOGUE_LEFT_RIGHT, mNumber) * CAMERA_SPEED * GSIN(mYaw) +
					    CONTROLS.GetAnalogueValue(ANALOGUE_DOWN_UP   , mNumber) * CAMERA_SPEED * GCOS(mYaw);
	
				mPos += GVector(dx, dy, dz);
				
				if (CONTROLS.IsButtonPressed(BUTTON_SPIN_CLOCKWISE, mNumber))
					mDestYaw -= CAMERA_ROT_SPEED;
				if (CONTROLS.IsButtonPressed(BUTTON_SPIN_ANTICLOCKWISE, mNumber))
					mDestYaw += CAMERA_ROT_SPEED;
					
				if (CONTROLS.IsButtonPressed(BUTTON_FLIPPER_RIGHT, mNumber))
					mDestPitch += CAMERA_ROT_SPEED / 2;
				if (CONTROLS.IsButtonPressed(BUTTON_FLIPPER_LEFT, mNumber))
					mDestPitch -= CAMERA_ROT_SPEED / 2;
				
				if (CONTROLS.IsButtonPressed(BUTTON_SPIN_CENTRE, mNumber))
					mDestYaw = G0;
	
				if (CONTROLS.IsButtonPressed(BUTTON_ZOOM_OUT, mNumber))
					mPos.Z -= G(0, 20000);
				if (CONTROLS.IsButtonPressed(BUTTON_ZOOM_IN, mNumber))
					mPos.Z += G(0, 20000);
					
				// for gamut & things like the stadiums..
				// rotate to build final position
				GVector r = GVector(G0, mLinearOffset, G0);
			
			    GMatrix m;
				m.MakeRotation(mYaw, mPitch, G0);
			    r = m * r;
			
				mLookAt = mPos + r;
				mDestLookAt = mLookAt;
			}
			break;

	default:
		break;
	}
//	if (GAME.GetGameState() == GAME_STATE_FALLEN_OFF_MAP)
//		if (mDestLookAt.Z > G(5))
//			mDestLookAt.Z = G(5);


	if ((movement_mode != 7) && (movement_mode != 5))
	{
		if (movement_mode == 2)
		{
			mDestPitch = IDEAL_PITCH;
		}
		else if (!mLockToggle)
		{
			// try and drop pitch
			mDestPitch -= PITCH_SPEED_DROP;
			if (mDestPitch < IDEAL_PITCH)
				mDestPitch = IDEAL_PITCH;
	
			// make sure we can see what we're looking at
	#ifdef LINK_EDITOR
			if (!(EDITOR.IsActive()))
	#endif
				CheckLineOfSight();
		}
	}

	// update mPos and mLookat with momentum

	// sorry about the bracketting...
/*	if ((GAME.GetGameTurn() < 3)
#ifdef LINK_EDITOR
		 && (!(EDITOR.IsActive()))
#endif
		 )
	{
		mLookAt			= mDestLookAt;
		mLinearOffset	= mDestLinearOffset;
		mYaw			= mDestYaw;
		mPitch			= mDestPitch;
	}
	else*/
	{
		mLookAt			= mLookAt		+ (mDestLookAt       - mLookAt      ) * CAMERA_MOMENTUM;
		mLinearOffset	= mLinearOffset + (mDestLinearOffset - mLinearOffset) * CAMERA_MOMENTUM;
		mYaw			= mYaw			+ (mDestYaw          - mYaw         ) * CAMERA_MOMENTUM;
		mPitch			= mPitch		+ (mDestPitch        - mPitch       ) * CAMERA_MOMENTUM;
	}

	// stop camera turning inside out
	if (mLinearOffset < G(3))
	{
		mDestLinearOffset = G(3);
		mLinearOffset = G(3);
	}


	if (movement_mode != 7)
	{
		if ((movement_mode == 5) && (GAME.GetGameState() == GAME_STATE_STARTING))
			BuildPosition(mYaw, IDEAL_PITCH, mPos);
		else
			BuildPosition(mYaw, mPitch, mPos);
	}

	FillOut();
	

	if ((movement_mode == 5) && (GAME.GetGameState() == GAME_STATE_STARTING))
	{
		GVector r = GVector(G0, mLinearOffset, G0);
	
	    GMatrix m;
		m.MakeRotation(mYaw, mPitch, G0);
	    r = m * r;

		mLookAt = mPos + r;
	}
}

//******************************************************************************************
// fill out all values given the camera mode.
void	CCamera::FillOut()
{
    switch (mMode)
	{
	case LOOKAT:
		// build orientation matrix;
		{
			mOrientation = GMatrix(GVector(G1, G0, G0), GVector(G0, G0, -G1), GVector(G0, G1, G0));

			GMatrix rot_yaw, rot_pit;
			rot_yaw.MakeRotationYaw(-mYaw);
			rot_pit.MakeRotationPitch(-mPitch);

			mOrientation = (mOrientation * rot_pit) * rot_yaw;

			GMatrix tm = mOrientation;
			mOrientation.TransposeInPlace();

			mMode = ORIENTATION;  //! err...
		}
		break;

	case ORIENTATION:
		//


		break;

	default:
		break;
	}

}

//******************************************************************************************
// JCL - 18-8-00  generate interpolated position
void	CCamera::Interpolate()
{
	// Note - this is done in place with a temporary camera
	mPos    = mOldPos    + ((mPos    - mOldPos   ) * GAME.GetFrameRenderFraction());
	mLookAt = mOldLookAt + ((mLookAt - mOldLookAt) * GAME.GetFrameRenderFraction());

	// JCL 19-10-00 Bugger - have to recreate the orientation, because the gamut uses it!
	// this might actually fix some other bugs!
	GVector	l = mLookAt - mPos;
	l.Normalise();
	GVector u  = GVector(G0, G0, G1);
	GVector r  = l ^ u;
	r.Normalise();
	u = l ^ r;
	u.Normalise();

	mOrientation = GMatrix(r, u, l);

	mOrientation.TransposeInPlace();
}

//******************************************************************************************
void	CCamera::BuildPosition(GINT yaw, GINT pitch, GVector &pos)
{

	// rotate to build final position
	GVector r = GVector(G0, -mLinearOffset, G0);

    GMatrix m;
	m.MakeRotation(yaw, pitch, G0);
    r = m * r;

	pos = mLookAt + r;

}

//******************************************************************************************
#define CAM_LOS_GRACE	G(2)

void	CCamera::CheckLineOfSight()
{
	// check to see if we can see what we're looking at (!)

	// for the moment, check to see if we can see the primary ball...

	// for the moment, only change mPitch
	// -> should extend to use mYaw based on flags in the map....

	GVector	target = WORLD.GetPrimaryPosition(mNumber);

	if (COORDS_INVALID(CCellRef(target)))
		return;

	GVector	start, result;

	target.Z = MAP.GetMapHeight(target); // drop to ground.

	// NB! use actual Yaw, but destination Pitch!
	BuildPosition(mYaw, mDestPitch, start);
	result = MAP.FindLineIntersection(start, target);

	while (((result - target).MagnitudeSqXY() > CAM_LOS_GRACE) &&
		   (mDestPitch < MAX_PITCH) &&
		   (result.X != -(G(1))))   //! buggered........
	{
		mDestPitch += PITCH_SPEED_RISE;

		BuildPosition(mYaw, mDestPitch, start);
		result = MAP.FindLineIntersection(start, target);
	}

	if (mDestPitch > MAX_PITCH)
		mDestPitch = MAX_PITCH;
}

//******************************************************************************************
void	CCamera::Lock()
{
	mLockToggle = TRUE;

	// store pos
	mLockPos = mPos;
}

//******************************************************************************************
void	CCamera::Unlock()
{
	if (mLockToggle == TRUE)
	{
		mLockToggle = FALSE;

		// dest pos
		PanIn();

		// dest angles
		mDestYaw = G0;
		mDestPitch = IDEAL_PITCH;
	}
}

//******************************************************************************************
#define LOCK_YAW_CLAMP		G(0, 20000)

//#define LOCK_PITCH_CLAMP	G(0, 20000)

#define LOCK_PITCH_CLAMP_D	(-G(0, 20000))
//#define LOCK_PITCH_CLAMP_D	(G(0, 10000))

#define LOCK_PITCH_CLAMP_U	(IDEAL_PITCH)

#define MIN_DIST			G(10)
#define MAX_DIST			G(30)

void	CCamera::EvaluateLockAngles()
{
	GVector d = WORLD.GetPrimaryPosition(mNumber);
	MAP.ClipPosition(d);
	mDestLookAt = d;

	GVector p = d - mLockPos;

	GINT	l = p.Magnitude();

	if (l > G(0, 100))
	{
		mDestLinearOffset = l;

		mDestYaw = GATAN2(p.Y, p.X) - G_PI_2;

		GINT	v = p.MagnitudeXY();
		if (v > GINT(0, 100))
			mDestPitch = GATAN2(v, -p.Z) - G_PI_2;
			
		//Now we need to see if this is ok?
		if (mDestYaw < -LOCK_YAW_CLAMP)
			mDestYaw = -LOCK_YAW_CLAMP;
		if (mDestYaw >  LOCK_YAW_CLAMP)
			mDestYaw =  LOCK_YAW_CLAMP;
		
		if (mDestPitch < LOCK_PITCH_CLAMP_D)
			mDestPitch = LOCK_PITCH_CLAMP_D;
		if (mDestPitch > LOCK_PITCH_CLAMP_U)
			mDestPitch = LOCK_PITCH_CLAMP_U;
			
		if (mDestLinearOffset < MIN_DIST)
			mDestLinearOffset = MIN_DIST;
			
		if (mDestLinearOffset > MAX_DIST)
			mDestLinearOffset = MAX_DIST;
			
		// and now rebuilt lock pos
		
		GVector r = GVector(G0, -mLinearOffset, G0);
	
	    GMatrix m;
		m.MakeRotation(mDestYaw, mDestPitch, G0);
	    r = m * r;
	
		mLockPos = mDestLookAt + r;
	}
}
