#include	"Common.h"

#include	"Globals.h"
#include	"Action.h"
#include	"Editors.h"
#include	"Profile.h"
#include	"Primary.h"

//******************************************************************************************
BOOL	CControls::Init()
{
	SINT	c0;

	for (c0 = 0; c0 < NUM_CONTROLLERS; c0 ++)
		mControllers[c0].Init();

	mControllers[0].Activate(); //!CONTROLS shouldn't be here
	mControllers[1].Activate();
	mControllers[0].SetInputNumber(0);
	mControllers[1].SetInputNumber(1);
	return TRUE;
}

//******************************************************************************************
POINT	CControls::GetMousePos(SINT cn)
{
	POINT	p;

	p.x = mControllers[cn].mMouseX;
	p.y = mControllers[cn].mMouseY;

	return p;
}

//******************************************************************************************
BOOL	CControls::IsButtonPressed(SINT button, SINT cn)
{
	return (mControllers[cn].IsButtonSet(button));
}

//******************************************************************************************
BOOL	CControls::IsButtonPressedDB(SINT button, SINT cn) // debounced version
{
	return (  (mControllers[cn].IsButtonSet   (button)) &&
		   ((!(mControllers[cn].IsButtonOldSet(button)))));
}

//******************************************************************************************
BOOL	CControls::IsButtonReleasedDB(SINT button, SINT cn) // debounced version
{
	return ((!(mControllers[cn].IsButtonSet   (button)) &&
		   (  (mControllers[cn].IsButtonOldSet(button)))));
}

//******************************************************************************************
GINT	CControls::GetAnalogueValue(SINT value, SINT cn)
{
	return mControllers[cn].mAnalogue[value];
}

//******************************************************************************************
void	CControls::Process()
{
	SINT	c0,cnt;

	// Only process other controllers if we're in multi-player mode...
	if(GAME.IsMultiPlayer()) cnt=NUM_CONTROLLERS;
	else cnt=1;
	
	#if TARGET == DC
	for (c0 = 0; c0 < cnt ; c0 ++)
		mControllers[c0].Update();
	#endif	

	//******************************
	// debug things

	if (IsButtonPressedDB(BUTTON_TOGGLE_MAPDRAW))
		ENGINE.ChangeMapDrawMode();

	if (IsButtonPressedDB(BUTTON_CHANGE_GAMUT))
		ENGINE.ChangeGamut();

#if ENABLE_PROFILER==1
	if (IsButtonPressedDB(BUTTON_TOGGLE_PROFILER))
		GAME.ToggleProfiler();

	if (IsButtonPressedDB(BUTTON_PROFILE_NEXT))
		CProfiler::NextPage();

	if (IsButtonPressedDB(BUTTON_PROFILE_PREV))
		CProfiler::PrevPage();
#endif //ENABLE_PROFILER

	if (IsButtonPressedDB(BUTTON_TOGGLE_DEBUG_TEXT))
		GAME.ToggleDebugText();

	if (IsButtonPressedDB(BUTTON_TOGGLE_2D))
		GAME.Toggle2D();

	if (IsButtonPressedDB(BUTTON_TOGGLE_COLLISION_INFO))
		GAME.ToggleCollisionInfo();

		if (IsButtonPressedDB(BUTTON_TOGGLE_CHARACTER_INFO))
		GAME.ToggleCharacterInfo();

	if (IsButtonPressedDB(BUTTON_TOGGLE_MAPWHO_INFO))
		GAME.ToggleMapWhoInfo();

	if (IsButtonPressedDB(BUTTON_TOGGLE_WATER))
		GAME.GetLevelData()->ToggleWater();

	if (IsButtonPressedDB(BUTTON_SCREENSHOT))
		GAME.TakeScreenShot();

#if TARGET == PC
	if (IsButtonPressedDB(BUTTON_RELIGHT_LEVEL))
		MAP.RelightLevel();
	if (IsButtonPressedDB(BUTTON_UNLIGHT_LEVEL))
		MAP.UnlightLevel();
#endif


#ifdef LINK_EDITOR
	if (EDITOR.IsActive())
		return; // nothing to do here...
#endif

	//************************
	// main controls

	// race boost
	if (GAME.IsGameStarting() && GAME.GetIVar(IV_LEVEL_TYPE) == LT_RACER && !(GAME.IsHelp()))
	{
		for (c0 = 0; c0 < NUM_CONTROLLERS; c0 ++)
		{
			CPrimary* p = PLAYER(c0)->GetPrimary() ;
			if (mControllers[c0].IsActive() && p)
			{
				// re-calculate contoller movement rotated into camera space
				GINT 	cx, cy;
			//	GINT	camy = GAME.GetCamera(c0)->mYaw;
			//	GINT camy = p->GetYaw() ;  // ????
				GINT camy = G0 ;

				cx =   GetAnalogueValue(ANALOGUE_LEFT_RIGHT, c0) *  GCOS(camy) -
						   GetAnalogueValue(ANALOGUE_DOWN_UP   , c0) *  GSIN(camy);
				cy =   GetAnalogueValue(ANALOGUE_LEFT_RIGHT, c0) *  GSIN(camy) +
						   GetAnalogueValue(ANALOGUE_DOWN_UP   , c0) *  GCOS(camy);

				GAME.PlayerAction(c0, A_DIRECT_START_RACER_BOOST, cx, cy);
			}
		}
	}

	if ((GAME.IsGameRunning()) && (!(GAME.IsGamePanelActive())))
	{
		if (!(GAME.IsHelp()))
		{
			for (c0 = 0; c0 < NUM_CONTROLLERS; c0 ++)
			{
				if (mControllers[c0].IsActive())
				{
					// pre-calculate contoller movement rotated into camera space
					GINT 	cx, cy;
					GINT	camy = GAME.GetCamera(c0)->mYaw;

					cx =   GetAnalogueValue(ANALOGUE_LEFT_RIGHT, c0) *  GCOS(camy) -
						   GetAnalogueValue(ANALOGUE_DOWN_UP   , c0) *  GSIN(camy);
					cy =   GetAnalogueValue(ANALOGUE_LEFT_RIGHT, c0) *  GSIN(camy) +
						   GetAnalogueValue(ANALOGUE_DOWN_UP   , c0) *  GCOS(camy);

					switch (GetControllerMode())
					{
					case CM_JOY_SHUTTLE:
						// Do what comes naturally
						GAME.PlayerAction(c0, A_THRUST_DEFAULT);
						// thrust up
						if (IsButtonPressed(BUTTON_FIRE_PRIMARY, c0))
							GAME.PlayerAction(c0, A_THRUST_UP);

						// thrust along (always do this)
						GAME.PlayerAction(c0, A_THRUST_ALONG, cx, cy);

						break;

					case CM_JOY_RACER:
/*						// Boost
						if (IsButtonPressedDB(BUTTON_FIRE_PRIMARY, c0))
							GAME.PlayerAction(c0, A_RACER_BOOST_START);

						if (IsButtonReleasedDB(BUTTON_FIRE_PRIMARY, c0))
							GAME.PlayerAction(c0, A_RACER_BOOST_END);

						// thrust along (always do this)
						GAME.PlayerAction(c0, A_RACER_ALONG, cx, cy);

						break;
*/
					case CM_JOY_DIRECT:
					case CM_KEY_DIRECT:


						if (IsButtonPressed(BUTTON_FIRE_PRIMARY, c0) &&
							IsButtonPressed(BUTTON_BLOCK, c0) )
						{
							GAME.PlayerAction(c0, A_DIRECT_SPIN_ATTACK) ;
						}
						else
						{

							if (IsButtonPressedDB(BUTTON_FIRE_PRIMARY, c0))
								GAME.PlayerAction(c0, A_DIRECT_ATTACK0);

							// Block
							if (IsButtonPressed(BUTTON_BLOCK, c0))
								GAME.PlayerAction(c0, A_DIRECT_START_BLOCK) ;
						}

						// jump?
						if (IsButtonPressedDB(BUTTON_FIRE_SECONDARY, c0))
							GAME.PlayerAction(c0, A_DIRECT_JUMP);



						// Pick up
						if (IsButtonPressedDB(BUTTON_THROW, c0))
							GAME.PlayerAction(c0, A_DIRECT_THROW) ;


												// Block
						if (IsButtonReleasedDB(BUTTON_BLOCK, c0))
							GAME.PlayerAction(c0, A_DIRECT_END_BLOCK) ;


						// trick?
//						if (IsButtonPressedDB(BUTTON_TRICK, c0))
//							GAME.PlayerAction(c0, A_DIRECT_TRICK0);

						// thrust along (always do this)
						GAME.PlayerAction(c0, A_RACER_ALONG, cx, cy);


						break;

					default:
						// fire buttons
						if (IsButtonPressedDB(BUTTON_FIRE_SECONDARY, c0))
							PLAYER(c0)->ActivateCursor();
						else if (IsButtonReleasedDB(BUTTON_FIRE_SECONDARY, c0))
							PLAYER(c0)->DeActivateCursor();

						if (IsButtonPressedDB(BUTTON_FIRE_PRIMARY, c0))
						{
							if (!(IsButtonPressed(BUTTON_FIRE_SECONDARY, c0)))
								GAME.PlayerAction(c0, SECONDARY_FIRE);
						}
					};

					// change weapon
					if (IsButtonPressedDB(BUTTON_CHANGE_WEAPON, c0))
						GAME.PlayerAction(c0, CHANGE_WEAPON);
				}
			}
		}

		// help
		if (IsButtonPressedDB(BUTTON_HELP))
			GAME.ToggleHelp();
	}
	// pause
	if (IsButtonPressedDB(BUTTON_PAUSE))
		GAME.TogglePause();

	// frame inc
	if (IsButtonPressedDB(BUTTON_NEXT_FRAME))
		GAME.FrameIncrease();

	if (!(GAME.IsGamePanelActive()))
	{
		if (IsButtonPressedDB(BUTTON_GAME_PANEL))
			GAME.ToggleGamePanel(0);
		else if (GAME.IsMultiPlayer())
			if (IsButtonPressedDB(BUTTON_GAME_PANEL, 1))
				GAME.ToggleGamePanel(1);
	}

	// next primary
	if (IsButtonPressed(BUTTON_NEXT_PRIMARY))
		WORLD.NextPrimary();

	//******************************
	// debug things

	if (IsButtonPressedDB(BUTTON_EDITOR))
		GAME.ToggleEditor();

	//******************************
	// Change level

#if TARGET == PC
	SINT	new_level = GAME.GetCurrentLevelNumber();

	if (IsButtonPressedDB(BUTTON_ADVANCE_LEVEL))
	{
		if (IsButtonPressed(BUTTON_SHIFT))
			new_level += 10;
		else
			new_level ++;
	}

	if (IsButtonPressedDB(BUTTON_RETREAT_LEVEL))
	{
		if (IsButtonPressed(BUTTON_SHIFT))
			new_level -= 10;
		else
			new_level --;
	}

	if (new_level != GAME.GetCurrentLevelNumber())
	{
		if (new_level > 99)
			new_level -= 1000;
		if (new_level < 0 )
			new_level += 1000;

		BOOL res = GAME.LoadLevel(new_level);
		if (!(res))
		{
			char text[200];
			sprintf(text, "Level %03d failed to load.", new_level);
			CONSOLE.AddString(text);
		}
	}
#endif

	//******************************
	// Teleport ball
	if (IsButtonPressed(BUTTON_TELEPORT))
	{
		CPrimary *t = WORLD.GetPrimaryThing();

		if (t)
		{
			SINT x = PLAYER(0)->GetCursor().X;
			SINT y = PLAYER(0)->GetCursor().Y;

			if (x != -1)
			{
				t->Move(GVector(G(x), G(y), MAP.GetMapHeight(GVector(G(x), G(y), G0))));
			}
		}
	}

}

//******************************************************************************************
void	CControls::SetControllerMode(EControllerMode mode, SINT n)
{
	if (n == -1)
	{
		SINT	c0;

		for (c0 = 0; c0 < NUM_CONTROLLERS; c0 ++)
			mControllers[c0].SetControllerMode(mode);
	}
	else
	{
		if ((n < NUM_CONTROLLERS) && (n >= 0))
			mControllers[n].SetControllerMode(mode);
	}
}

//******************************************************************************************
EControllerMode	CControls::GetControllerMode()
{
	// return the state of the first controller - they should all be the same, I hope...
	return mControllers[0].GetControllerMode();
}

//******************************************************************************************
void	CControls::Update()
{
	SINT	c0;

	for (c0 = 0; c0 < NUM_CONTROLLERS; c0 ++)
		mControllers[c0].Update();
}

//******************************************************************************************
BOOL	CControls::AreAnyButtonsPressed()
{
#if TARGET == DC
	SINT	c0;
	BOOL	ret = FALSE;

	for (c0 = 0; c0 < NUM_CONTROLLERS; c0 ++)
		ret |= mControllers[c0].AreAnyButtonsPressed();
		
	return ret;
	
#else
	return FALSE;
#endif
}

