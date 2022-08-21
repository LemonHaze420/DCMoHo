#include	"Common.h"
#include    "globals.h"
#if TARGET == DC

#include	"DCController.h"
#include	"DCBuildType.h"
#include 	"profile.h"
#include	"Career.h"
#include 	"maincode.h"

#define PSX_DEMO_VERSION 1

//#define	CONTROLLER_DEBUG1		// uncomment for debug controls

//#define CONTROLLER_DEBUG_KEYBOARD

#ifdef RECORD_GAME
static	int record_no = 0;
#endif

#if (DEMO_GAME == 1) || (DEMO_GAME == 2) || (DEMO_GAME == 3) || DEMO_GAME == 4
static	UBYTE	temp_playback_buffer[40000];
#endif

static	UBYTE	temp_record_buffer1[50000];
static	UBYTE	temp_record_buffer2[50000];
static	SINT	temp_record_size;

static	UBYTE	*playback_buffer;

#define DEADZONE	(G(0, 13107))

//******************************************************************************************

#include 	"DCCheats.h"

struct	SCheat
{
	SINT	mNumKeys;
	SINT	mKeys[20];
}	CHEATS[] = 
	{
		{ 15, {3, 4, 3, 1, 3, 2, 3, 3, 4, 3, 1, 1, 3, 2, 3}},		// all prisons  (TTnP)
		{ 11, {3, 4, 1, 1, 3, 3, 4, 1, 2, 4, 4}},					// Bouncy   (Boing)
		{ 13, {3, 3, 3, 4, 3, 1, 1, 1, 4, 3, 1, 1, 2}},				// Bonus   	(Bruce)
		{ 12, {3, 3, 4, 2, 1, 4, 2, 3, 1, 4, 4, 1}},				// Doctor  	(temp)
		{ 10, {3, 3, 3, 4, 2, 3, 3, 3, 2, 1}},						// version	(LtL)
		{ 16, {4, 3, 4, 3, 4, 3, 1, 2, 1, 3, 1, 3, 4, 3, 4, 2}},	// slippery (6:00)
	};


//******************************************************************************************
BOOL	is_key_pressed(PDS_KEYBOARD *keys, SINT key)
{
	SINT	c0;
	
	for (c0 = 0; c0 < 6; c0 ++)
		if (keys->key[c0] == key)
			return TRUE;
			
	return FALSE;
}

//******************************************************************************************

void	CController::StartRecording(SINT no)
{
#ifdef RECORD_GAME
	record_no = no; // should really be in the class header

	mPlaybackPtr = (char *)temp_record_buffer1;
	mPlaybackSize = 0;

	*((ULONG *)mPlaybackPtr) = PSX_DEMO_VERSION;
	mPlaybackPtr += 4;
	mPlaybackSize += 4;
	*((ULONG *)mPlaybackPtr) = GAME.GetCurrentLevelNumber();
	mPlaybackPtr += 4;
	mPlaybackSize += 4;

	mRecording = ACTIVE;

//	printf("Started recording...\n");

#endif
}

//******************************************************************************************
void	CController::RecordState()
{
#ifdef RECORD_GAME
	*((ULONG *)mPlaybackPtr) = mButtons1;
	mPlaybackPtr += 4;
	mPlaybackSize += 4;
	*((ULONG *)mPlaybackPtr) = mButtons2;
	mPlaybackPtr += 4;
	mPlaybackSize += 4;
	*((GINT *)mPlaybackPtr) = mAnalogue[0];
	mPlaybackPtr += 4;
	mPlaybackSize += 4;
	*((GINT *)mPlaybackPtr) = mAnalogue[1];
	mPlaybackPtr += 4;
	mPlaybackSize += 4;

#endif
}

//******************************************************************************************
void	CController::StopRecording()
{
#ifdef RECORD_GAME
	mRecording = NOT_ACTIVE;

	// copy into second buffer
	SINT 	c0;
	UBYTE	*p1 = temp_record_buffer1;
	UBYTE	*p2 = temp_record_buffer2;
	for (c0 = 0; c0 < mPlaybackSize; c0 ++)
	{
		*(p2 ++) = *(p1 ++);
	}
//	memcpy(temp_record_buffer2, temp_record_buffer1, mPlaybackSize);
	temp_record_size = mPlaybackSize;

//	printf("Stopped Recording.\n");
//	printf("Recorded: %d\m", mPlaybackSize);
#endif
}

//******************************************************************************************
void	CController::StartPlayBack(SINT no)
{
#if (DEMO_GAME == 1) || (DEMO_GAME == 2) || (DEMO_GAME == 3) || DEMO_GAME == 4
	playback_buffer = temp_playback_buffer;
#else
	// make damn sure this doesn't overflow...
	playback_buffer = (POOL->mLevelPool) + LEVEL_POOL_SIZE - (64 * 1024);
#endif


	char	str[256];
	sprintf(str, "DEMOS\\DEMO%d.DEM", no);

	mPlaybackSize = DCOpenFile(str);
	if (mPlaybackSize == -1)
	{
//		printf("Can't open demo file\n");
		return;
	}
//	printf("Playing Back: %d\n", mPlaybackSize);

	mPlaybackPtr = (char *)playback_buffer;
	DCLoadPartFile((ULONG*)mPlaybackPtr,204800);  // load it all in
	DCCloseFile();

//	printf("Demo Version : %d\n", *((ULONG *)mPlaybackPtr));
	mPlaybackPtr += 4;
	mPlaybackSize -= 4;
//	printf("Demo Wants to run on level: %d\n", *((ULONG *)mPlaybackPtr));
	mPlaybackPtr += 4;
	mPlaybackSize -= 4;

	mPlayingBack = ACTIVE;
}

//******************************************************************************************
void	CController::PlayBackState()
{
	ASSERT(mPlayingBack == ACTIVE);

	if (mPlaybackSize <= 0)
	{
		StopPlayBack();
		return;
	}

	mButtons1 = *((ULONG *)mPlaybackPtr);
	mPlaybackPtr += 4;
	mPlaybackSize -= 4;
	mButtons2 = *((ULONG *)mPlaybackPtr);
	mPlaybackPtr += 4;
	mPlaybackSize -= 4;
	mAnalogue[0] = *((GINT *)mPlaybackPtr);
	mPlaybackPtr += 4;
	mPlaybackSize -= 4;
	mAnalogue[1] = *((GINT *)mPlaybackPtr);
	mPlaybackPtr += 4;
	mPlaybackSize -= 4;


	// Very Cheeky!
//	GAME.TakeScreenShot();
}

//******************************************************************************************
void	CController::StopPlayBack()
{
	ASSERT(mPlayingBack == ACTIVE);

	mPlayingBack = NOT_ACTIVE;

//	printf("Stopped PlayBack.\n");

//#if (DEMO_GAME == 1) || (DEMO_GAME == 2)
	GAME.SetQuitFlag(GQ_FRONTEND);
//#endif
}

static unsigned char align[6]={0,1,0xFF,0xFF,0xFF,0xFF};

#define VIB_END	0
#define VIB_ON	1
#define VIB_OFF 2
#define VIB_VAR 3
#define VIB_DELAY 4
#define VIB_ENDTICK 5




UBYTE vibTest[]={VIB_ON,VIB_DELAY,20,VIB_OFF,VIB_VAR,0,VIB_END};
UBYTE vibHitGround[]={VIB_VAR,150,VIB_DELAY,8,VIB_VAR,0,VIB_END};
UBYTE vibBeenHit[]={VIB_VAR,120,VIB_DELAY,8,VIB_VAR,0,VIB_END};
UBYTE vibShocked[]={VIB_VAR,64,VIB_ON,VIB_DELAY,10,VIB_OFF,VIB_VAR,0,VIB_END};
UBYTE vibBeenHitHard[]={VIB_VAR,255,VIB_DELAY,32,VIB_VAR,0,VIB_END};
UBYTE vibStop[]={VIB_VAR,0,VIB_OFF,VIB_VAR,0,VIB_OFF,VIB_VAR,0,VIB_OFF,VIB_END};
UBYTE vibChurn[]={VIB_VAR,100,VIB_ON,VIB_DELAY,10,VIB_OFF,VIB_VAR,0,VIB_END};
UBYTE *vib_list[]=
{
	(UBYTE*)&vibTest,
	(UBYTE*)&vibHitGround,
	(UBYTE*)&vibBeenHit,
	(UBYTE*)&vibShocked,
	(UBYTE*)&vibBeenHitHard,
	(UBYTE*)&vibStop,
	(UBYTE*)&vibChurn
};




void	CController::TriggerVibration(int Vib_Num)
{
	if((!mPlayingBack)&&(CAREER.mForceFeedback))
	{
		struct  SPadData *pad_data = PLATFORM.GetPadData(mInputNumber);
		//printf("Triggered Vib\n");
		pad_data->vib_profile=vib_list[Vib_Num];
		pad_data->ticker=1;
	}
}

void StopVibration()
{
	struct SPadData *pad0 = PLATFORM.GetPadData(0);
	struct SPadData *pad1 = PLATFORM.GetPadData(1);
	pad0->Motor0=0;
	pad0->Motor1=0;
	pad0->vib_profile=NULL;
	pad1->Motor0=0;
	pad1->Motor1=0;
	pad1->vib_profile=NULL;
	CONTROLS.Process();
	CONTROLS.Process();
}




void	CController::ProcessVibration()
{
	struct SPadData *pad = PLATFORM.GetPadData(mInputNumber);
	//printf("Processing vib\n");
	UBYTE *v=pad->vib_profile;
	if(v)
	{
		if(!--pad->ticker)
		{
			UBYTE next;
			int done=0;
			while(!done)
			{
				next=*v++;
				switch(next)
				{
					case VIB_END :
						//printf("VIB_END\n");
						v=NULL;
						pad->Motor0=0;
						pad->Motor1=0;
						done=1;
						break;
					case VIB_ON :
						//printf("VIB_ON\n");
						pad->Motor0=1;
						break;
					case VIB_OFF :
						//printf("VIB_OFF\n");
						pad->Motor0=0;
						break;
					case VIB_VAR :
						//printf("VIB_VAR\n");
						pad->Motor1=*v++;
						break;
					case VIB_DELAY :
						//printf("VIB_DELAY\n");
						pad->ticker=*v++;
						done=1;
						break;
					case VIB_ENDTICK :
						done=1;
						break;
					default:
						//printf("Illegal code in Vibration Profile: 0x%02x\n",next);
						done=1;
						break;
				}
			}
			pad->vib_profile=v;
		}
	}


}







// ****************************************************************************
void	CController::Update()
{
	// backup state
	mButtons1Old = mButtons1;
	mButtons2Old = mButtons2;

	// Buttons
	mButtons1 = 0;
	mButtons2 = 0;

#if DEMO_GAME == 1 || DEMO_GAME == 3
	if (PadValue & PADselect)
	{
		GAME.SetQuitFlag(GQ_QUIT);
//		return;
	}
#endif

#ifndef DEMO_GAME
	if (mPlayingBack == ACTIVE)
	{
		PDS_PERIPHERAL *per = GetSubPad(mInputNumber);
		if (per->on & PDD_DGT_ST)
		{
			StopPlayBack();
			return;
		}
	}
#endif

	// DEMO PLAYBACK
	if (mPlayingBack == ACTIVE)
	{
		PlayBackState();
		return;
	}

#if (DEMO_GAME == 1) || (DEMO_GAME == 2)
	// safety!
	return;
#endif


#ifdef JCLDCNOPSX		// force feedback??

	if(!mPlayingBack)
	{
		ProcessVibration();
		SPadData *padi = pad_data;
		PadSetAct(mInputNumber<<4, &padi->Motor0, 2);  // Point the library to the motor info...
		if(!padi->valid)                    // valid is ZERO if pad is active
		{
			int state = PadGetState(mInputNumber<<4);   // See if the controller is connected, and is ready for data
			//printf("State: %d\n",state);
			if (state==PadStateDiscon)
			{
				padi->Motor0 = 0;
				padi->Motor1 = 0;
			}
			else
			{
				if (PadInfoMode(mInputNumber<<4,InfoModeCurExID,0))  // Have we got actuators?
				{
					//printf("Have actuators\n");
					//if (state == PadStateFindPad)
					//	padi->Send = 0;
					//if ( padi->Send==0 )
					//{
					//	if (state == PadStateFindCTP1)
					//		padi->Send = 1;
						if (state == PadStateStable)
						{
							//printf("Sending...\n");
							if (PadSetActAlign(mInputNumber<<4,align))  // Ready to receive update....
								padi->Send = 1;
						}
					//}
				}
			}
		}
	}
#endif

	// is this  a valid pad?
	if (!(PLATFORM.IsControllerValid(mInputNumber)))
		return;		


#define DIGITAL_PAD_SPEED   G(1)
#define DIGITAL_PAD_SPEED2   G(0, 57000)

	PDS_PERIPHERAL *per = GetSubPad(mInputNumber);
	
//	if ((GAME.mInFrontEnd) && (per->on & PDD_DGT_TL))
	if (0)
	{
		// do nothing so cheats work.
	}
	else
	{
		GINT	p = DIGITAL_PAD_SPEED;
		
   	 	mAnalogue[ANALOGUE_LEFT_RIGHT]= ((float) per->x1*512);
    	mAnalogue[ANALOGUE_DOWN_UP]   = ((float)-per->y1*512);
    	
    	if ((mAnalogue[ANALOGUE_LEFT_RIGHT] <  DEADZONE) &&
	      	(mAnalogue[ANALOGUE_LEFT_RIGHT] > -DEADZONE))
	      	mAnalogue[ANALOGUE_LEFT_RIGHT] = G0;

    	if ((mAnalogue[ANALOGUE_DOWN_UP] <  DEADZONE) &&
	      	(mAnalogue[ANALOGUE_DOWN_UP] > -DEADZONE))
	      	mAnalogue[ANALOGUE_DOWN_UP] = G0;

		if (per->on & PDD_DGT_ST) {SetButton(BUTTON_GAME_PANEL);}		

		SINT	setting = mInputNumber ? CAREER.mControls2 : CAREER.mControls1;

		// no choice for golf
		if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF)
			setting = 0;

		switch (setting)
		{
		case 0:
			if (per->on & PDD_DGT_TA) {SetButton(BUTTON_FIRE_SECONDARY);}
			if (per->on & PDD_DGT_TB) {SetButton(BUTTON_FIRE_PRIMARY);}
			if (per->on & PDD_DGT_TY) {SetButton(BUTTON_BLOCK);}
			if (per->on & PDD_DGT_TX) {SetButton(BUTTON_THROW);}		
			if (per->on & PDD_DGT_TR) {SetButton(BUTTON_BRAKE);}
			break;
		case 1:
			if (per->on & PDD_DGT_TB) {SetButton(BUTTON_FIRE_SECONDARY);}
			if (per->on & PDD_DGT_TA) {SetButton(BUTTON_FIRE_PRIMARY);}
			if (per->on & PDD_DGT_TX) {SetButton(BUTTON_BLOCK);}
			if (per->on & PDD_DGT_TY) {SetButton(BUTTON_THROW);}		
			if (per->on & PDD_DGT_TR) {SetButton(BUTTON_BRAKE);}
			break;
		case 2:
			if (per->on & PDD_DGT_TR) {SetButton(BUTTON_FIRE_SECONDARY);}
			if (per->on & PDD_DGT_TA) {SetButton(BUTTON_FIRE_PRIMARY);}
			if (per->on & PDD_DGT_TX) {SetButton(BUTTON_BLOCK);}
			if (per->on & PDD_DGT_TY) {SetButton(BUTTON_THROW);}		
			if (per->on & PDD_DGT_TB) {SetButton(BUTTON_BRAKE);}
			break;
		};


		if (((per->on & PDD_DGT_KU) || (per->on & PDD_DGT_KD)) && 
		    ((per->on & PDD_DGT_KL) || (per->on & PDD_DGT_KR)))
		p = DIGITAL_PAD_SPEED2;


		if (per->on & PDD_DGT_KU) {mAnalogue[ANALOGUE_DOWN_UP] = p;}
		if (per->on & PDD_DGT_KD) {mAnalogue[ANALOGUE_DOWN_UP] = -p;}
		if (per->on & PDD_DGT_KL) {mAnalogue[ANALOGUE_LEFT_RIGHT] = -p;}
		if (per->on & PDD_DGT_KR) {mAnalogue[ANALOGUE_LEFT_RIGHT] = p;}


#ifdef	CONTROLLER_DEBUG1
		if (per->on & PDD_DGT_ST) {showem();}		
		if (per->on & PDD_DGT_TL) {SetButton(BUTTON_SPIN_CLOCKWISE);}
		if (per->on & PDD_DGT_TR) {SetButton(BUTTON_SPIN_ANTICLOCKWISE); } 
#endif

		// Keyboard Stuff
#ifdef 	CONTROLLER_DEBUG_KEYBOARD
		PDS_KEYBOARD *keys = GetKeys();
		
		if (is_key_pressed(keys, PDD_KEYUS_PGDN))		{SetButton(BUTTON_SPIN_ANTICLOCKWISE);};
		if (is_key_pressed(keys, PDD_KEYUS_DEL))		{SetButton(BUTTON_SPIN_CLOCKWISE);};
		if (is_key_pressed(keys, PDD_KEYUS_HOME))		{SetButton(BUTTON_ZOOM_IN);};
		if (is_key_pressed(keys, PDD_KEYUS_END))		{SetButton(BUTTON_ZOOM_OUT);};

		if (is_key_pressed(keys, PDD_KEYUS_PAD_END))	{SetButton(BUTTON_TIGHT_VIEW);};
		if (is_key_pressed(keys, PDD_KEYUS_PAD_DOWN))	{SetButton(BUTTON_WIDE_VIEW);};
		if (is_key_pressed(keys, PDD_KEYUS_PAD_INSERT))	{SetButton(BUTTON_PAUSE);};

		if (is_key_pressed(keys, PDD_KEYUS_INSERT))		{SetButton(BUTTON_FLIPPER_LEFT);};
		if (is_key_pressed(keys, PDD_KEYUS_PGUP))		{SetButton(BUTTON_FLIPPER_RIGHT);};

		if (is_key_pressed(keys, PDD_KEYUS_PAD_HOME))	{SetButton(BUTTON_TOGGLE_PROFILER);};
		if (is_key_pressed(keys, PDD_KEYUS_PAD_UP))		{SetButton(BUTTON_PROFILE_PREV);};
		if (is_key_pressed(keys, PDD_KEYUS_PAD_PGUP))	{SetButton(BUTTON_PROFILE_NEXT);};
#endif

		// DC Specific buttons
		if (per->on & PDD_DGT_TA) {SetButton(BUTTON_DC_A);}
		if (per->on & PDD_DGT_TB) {SetButton(BUTTON_DC_B);}
		if (per->on & PDD_DGT_TX) {SetButton(BUTTON_DC_X);}		
		if (per->on & PDD_DGT_TY) {SetButton(BUTTON_DC_Y);}
		
		if (per->on & PDD_DGT_TL) {SetButton(BUTTON_DC_L);}		
		if (per->on & PDD_DGT_TR) {SetButton(BUTTON_DC_R);}	
		if (per->on & PDD_DGT_ST) {SetButton(BUTTON_DC_START);}

		if (per->on & PDD_DGT_TC) {SetButton(BUTTON_DC_C);}

		
		// reset?
		if ((IsButtonSet(BUTTON_DC_A)) &&
			(IsButtonSet(BUTTON_DC_B)) &&
			(IsButtonSet(BUTTON_DC_X)) &&
			(IsButtonSet(BUTTON_DC_Y)) &&
			(IsButtonSet(BUTTON_DC_START)))
			SetButton(BUTTON_DC_RESET);

	}

#ifdef AGEING
	if ((GAME.GetGameTurn() & (128 + 256)) == 0)
		mAnalogue[ANALOGUE_DOWN_UP] = G1;
	if ((GAME.GetGameTurn() & (128 + 256)) == 128)
		mAnalogue[ANALOGUE_LEFT_RIGHT] = G1;
	if ((GAME.GetGameTurn() & (128 + 256)) == 256)
		mAnalogue[ANALOGUE_DOWN_UP] = -G1;
	if ((GAME.GetGameTurn() & (128 + 256)) == 128 + 256)
		mAnalogue[ANALOGUE_LEFT_RIGHT] = -G1;

	if ((GAME.GetGameTurn() & 31) == 0)
		SetButton(BUTTON_FIRE_PRIMARY);
	if ((GAME.GetGameTurn() & 63) == 48)
		SetButton(BUTTON_FIRE_SECONDARY);
#endif

#ifdef RECORD_GAME

	// DEMO Recording
	if (mRecording == ACTIVE)
	{
		if (IsButtonSet(BUTTON_GAME_PANEL))
			StopRecording();
		else
			RecordState();
	}
#endif

//	printf("%06d %06d\n", mAnalogue[ANALOGUE_LEFT_RIGHT].mVal, mAnalogue[ANALOGUE_DOWN_UP]. mVal);


	// Cheats
	
#define	IDB(button) ((IsButtonSet(button)) && (!(IsButtonOldSet(button))))
	
	mCheatAvailable = -1; // only available for a frame...
	
	if (per->on & PDD_DGT_TL)
	{
		if (!(mCheatStarted))
		{
			mCheatStarted = TRUE;
			mCheatPos = 0;
		}
		
		if (IDB(BUTTON_DC_X))
		{
			if (mCheatPos < 19)
				mCheatKeys[mCheatPos ++] = 1;
		}
	
		if (IDB(BUTTON_DC_Y))
		{
			if (mCheatPos < 19)
				mCheatKeys[mCheatPos ++] = 2;
		}
	
		if (IDB(BUTTON_DC_A))
		{
			if (mCheatPos < 19)
				mCheatKeys[mCheatPos ++] = 3;
		}
	
		if (IDB(BUTTON_DC_B))
		{
			if (mCheatPos < 19)
				mCheatKeys[mCheatPos ++] = 4;
		}
		
		SINT c0;
		
		for (c0 = 0; c0 < NUM_CHEATS; c0 ++)
		{
			SINT n = CHEATS[c0].mNumKeys;
			
			if (n == mCheatPos)
			{
				SINT	c1;
				SINT	ok = TRUE;
				
				for (c1 = 0; c1 < n; c1 ++)
				{
					if (mCheatKeys[c1] != CHEATS[c0].mKeys[c1])
						ok = FALSE;
				}
				if (ok)
				{
					if ((c0 != 1) && (c0 != 5))		// no slippery or bouncy.
					{
						mCheatAvailable = c0;
						mCheatStarted = FALSE;
					}
				}
			}
		}
	}
	else
	{
		mCheatStarted = FALSE;
	}	
}

//*******************************************************************
SINT	CController::GetCheat()
{
	return mCheatAvailable;
}


//*******************************************************************
BOOL	CController::AreAnyButtonsPressed()
{
//	if (!mIsActive)
//		return FALSE;

	if (mInputNumber > 1)
		return FALSE;
		
	if (!(PLATFORM.IsControllerValid(mInputNumber)))
		return FALSE;

	BOOL	ret = FALSE;

	ret |= IsButtonSet(BUTTON_DC_A);
	ret |= IsButtonSet(BUTTON_DC_B);
	ret |= IsButtonSet(BUTTON_DC_C);
	ret |= IsButtonSet(BUTTON_DC_X);
	ret |= IsButtonSet(BUTTON_DC_Y);
	ret |= IsButtonSet(BUTTON_DC_Z);
	ret |= IsButtonSet(BUTTON_DC_L);
	ret |= IsButtonSet(BUTTON_DC_R);
	ret |= IsButtonSet(BUTTON_DC_START);

	if ((mAnalogue[ANALOGUE_DOWN_UP] > DEADZONE) || (mAnalogue[ANALOGUE_DOWN_UP] < -DEADZONE))
		ret = TRUE;
	if ((mAnalogue[ANALOGUE_LEFT_RIGHT] > DEADZONE) || (mAnalogue[ANALOGUE_LEFT_RIGHT] < -DEADZONE))
		ret = TRUE;

	return ret;	
}

#endif
