#include	"Common.h"
#include    "globals.h"
#if TARGET == PSX

#include	"PSXController.h"

#include	"PSXBuildType.h"
#include "profile.h"

#include	"Career.h"

#define PSX_DEMO_VERSION 1

#ifdef RECORD_GAME
static	int record_no = 0;
#endif

#if (DEMO_GAME == 1) || (DEMO_GAME == 2) || (DEMO_GAME == 3) || DEMO_GAME == 4
static	UBYTE	temp_playback_buffer[40000];
#endif

static	UBYTE	*playback_buffer;

//******************************************************************************************

#include 	"PSXCheats.h"

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
void	CController::StartRecording(SINT no)
{
#ifdef RECORD_GAME
	record_no = no; // should really be in the class header

	mPlaybackPtr = (char *)0x80400000;
	mPlaybackSize = 0;

	*((ULONG *)mPlaybackPtr) = PSX_DEMO_VERSION;
	mPlaybackPtr += 4;
	mPlaybackSize += 4;
	*((ULONG *)mPlaybackPtr) = GAME.GetCurrentLevelNumber();
	mPlaybackPtr += 4;
	mPlaybackSize += 4;

	mRecording = ACTIVE;

	printf("Started recording...\n");

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

	STOP_PSX_PROF();
	SOUND.StopTrack();                  // Reinit for double speed loading if I was playing CDA
	MWRedirectIO(__MWIO_CDROM);

	char	str[256];
	sprintf(str, "cdrom:MoHoData\\Demos\\demo%d.dem", record_no);

	SINT	handle = MWopen(str, O_CREAT);
	printf("Returned handle: %d\n",handle);

	if (handle == -1) return ;

	SINT	len;

	MWwrite(handle, (char *)0x80400000, mPlaybackSize);
	MWclose(handle);

	printf("Stopped Recording.\n");
	printf("Recorded: %d\m", mPlaybackSize);
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

	mPlaybackSize = PSXOpenFile(str);
	if (mPlaybackSize == -1)
	{
		printf("Can't open demo file\n");
		return;
	}
//	printf("Playing Back: %d\n", mPlaybackSize);

	mPlaybackPtr = (char *)playback_buffer;
	PSXLoadPartFile((ULONG*)mPlaybackPtr,204800);  // load it all in
	PSXCloseFile();

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

	struct  SPadData *pad_data = PLATFORM.GetPadData(mInputNumber);

	if(!pad_data->comflag)
	{
		// Make sure it's a type we support....
		if((pad_data->type!=4)&&(pad_data->type!=7)&&(pad_data->type!=1))
		{
			pad_data->valid=255;
			pad_data->buttons=0xffff;
			pad_data->lx=
			pad_data->ly=
			pad_data->rx=
			pad_data->ry=128;
		}
		else pad_data->valid=0;
	}
	else
		pad_data->valid=255;

    unsigned long PadValue=(~pad_data->buttons)&0xffff;
    PadValue=((PadValue>>8)&0x00ff)|((PadValue<<8)&0xff00);

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
		if (PadValue & PADstart)
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

	// Right.. Here's the vibration control stuff.....
	// This does all the checks necessary for Dualshock controllers (even in Digital mode)
	// and sends the actuator details when it's required

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

	//!PSX

	//if(!mInputNumber)
	//		printf("Valid: %d  Comflag: %d Type: %d  Size:%d\n",pad_data->valid,pad_data->comflag,pad_data->type,pad_data->count);


	// DAMO!!!
	// This line locks analog controllers in 'analog mode', but mustn't be called every frame...
	// causes the actuators on dualshocks to stop responding... Find somewhere else to put it..
	//if(!pad_data->valid)PadSetMainMode(mInputNumber,1,3);


    SWORD ax=  pad_data->lx - 128;
    SWORD ay=-(pad_data->ly - 128);

    #define DEAD_ZONE	50

    if((ABS(ax) < DEAD_ZONE) || (pad_data->type != 7))	ax=0;
    if((ABS(ay) < DEAD_ZONE) || (pad_data->type != 7))	ay=0;

    mAnalogue[ANALOGUE_LEFT_RIGHT].SetVal(ax<<9);
    mAnalogue[ANALOGUE_DOWN_UP].SetVal(ay<<9);

//    mAnalogue[ANALOGUE_LEFT_RIGHT].SetVal(((PLATFORM.mPadData1.lx-128)&0xf0)<<12);
//    mAnalogue[ANALOGUE_DOWN_UP].SetVal(((PLATFORM.mPadData1.ly-128)&0xf0)<<12);


//#define DIGITAL_PAD_SPEED G(0, 50000)
#define DIGITAL_PAD_SPEED   G(1)
#define DIGITAL_PAD_SPEED2   G(0, 57000)

	if (PadValue & PADselect)
	{
		// "Shift"
		SetButton(BUTTON_PSX_SELECT);

#ifdef	ALLOW_SIMPLE_CHEAT
		if (PadValue & PADR2)
		{
			SetButton(BUTTON_ADVANCE_LEVEL);
			if (PadValue & PADstart)
				SetButton(BUTTON_SHIFT);   //!PSX   this is broken.  should be a separate button for +10 levels...
		}
#endif

#ifdef	ALLOW_DEBUG_CONTROLS

        if (PadValue & PADRdown) 	SetButton(BUTTON_SPIN_CENTRE);
		if (PadValue & PADRup)		SetButton(BUTTON_WIDE_VIEW);
		if (PadValue & PADRleft)	SetButton(BUTTON_TIGHT_VIEW);
		if (PadValue & PADRright)	SetButton(BUTTON_PAUSE);

		if (PadValue & PADL2)
		{
			SetButton(BUTTON_RETREAT_LEVEL);
			if (PadValue & PADstart)
				SetButton(BUTTON_SHIFT);   //!PSX   this is broken.  should be a separate button for +10 levels...
		}


		if (PadValue & PADR1)
		{
			if (PadValue & PADstart)
				SetButton(BUTTON_SCREENSHOT);
			else
				SetButton(BUTTON_ZOOM_OUT);
		}

		if (PadValue & PADL1)		SetButton(BUTTON_ZOOM_IN);
		if (PadValue & PADLright)	SetButton(BUTTON_TOGGLE_WATER);
		if (PadValue & PADLup)		SetButton(BUTTON_PROFILE_NEXT);
		if (PadValue & PADLdown)	SetButton(BUTTON_PROFILE_PREV);
		if (PadValue & PADLleft)	SetButton(BUTTON_TOGGLE_PROFILER);
#endif

	}
	else
	{
		GINT	p = DIGITAL_PAD_SPEED;
				
		// clip diagonals
		if (((PadValue & PADLup  ) || (PadValue & PADLdown )) && 
		    ((PadValue & PADLleft) || (PadValue & PADLright)))
		    p = DIGITAL_PAD_SPEED2;
		    
	
		if (PadValue & PADLup)		mAnalogue[ANALOGUE_DOWN_UP] = p;
		if (PadValue & PADLdown)	mAnalogue[ANALOGUE_DOWN_UP] = -p;
		if (PadValue & PADLleft)	mAnalogue[ANALOGUE_LEFT_RIGHT] = -p;
		if (PadValue & PADLright)	mAnalogue[ANALOGUE_LEFT_RIGHT] = p;

		// different controller settings

		SINT	setting = mInputNumber ? CAREER.mControls2 : CAREER.mControls1;

		// no choice for golf
		if (GAME.GetIVar(IV_LEVEL_TYPE) == LT_GOLF)
			setting = 0;

		switch (setting)
		{
		case 0:
			if (PadValue & PADRright)	SetButton(BUTTON_FIRE_PRIMARY);
			if (PadValue & PADRdown)	SetButton(BUTTON_FIRE_SECONDARY);
			if (PadValue & PADRup)		SetButton(BUTTON_BLOCK);
			if (PadValue & PADRleft)	SetButton(BUTTON_THROW);
			if (PadValue & PADR1)		SetButton(BUTTON_BRAKE);
			break;
		case 1:
			if (PadValue & PADRright)	SetButton(BUTTON_FIRE_SECONDARY);
			if (PadValue & PADRdown)	SetButton(BUTTON_FIRE_PRIMARY);
			if (PadValue & PADRup)		SetButton(BUTTON_THROW);
			if (PadValue & PADRleft)	SetButton(BUTTON_BLOCK);
			if (PadValue & PADR1)		SetButton(BUTTON_BRAKE);
			break;
		case 2:
			if (PadValue & PADRright)	SetButton(BUTTON_BRAKE);
			if (PadValue & PADRdown)	SetButton(BUTTON_FIRE_PRIMARY);
			if (PadValue & PADRup)		SetButton(BUTTON_THROW);
			if (PadValue & PADRleft)	SetButton(BUTTON_BLOCK);
			if (PadValue & PADR1)		SetButton(BUTTON_FIRE_SECONDARY);
			break;
		};

#ifdef	ALLOW_DEBUG_CONTROLS
		if (PadValue & PADL2)		SetButton(BUTTON_SPIN_CLOCKWISE);
		if (PadValue & PADR2)		SetButton(BUTTON_SPIN_ANTICLOCKWISE);
#endif

		if (PadValue & PADstart)	SetButton(BUTTON_GAME_PANEL);

	}
	// predefined keys
	if (PadValue & PADRup)		SetButton(BUTTON_PSX_TRIANGLE);
	if (PadValue & PADRdown)	SetButton(BUTTON_PSX_X);
	if (PadValue & PADRleft)	SetButton(BUTTON_PSX_SQUARE);
	if (PadValue & PADRright)	SetButton(BUTTON_PSX_CIRCLE);
	if (PadValue & PADL1)		SetButton(BUTTON_PSX_L1);
	if (PadValue & PADL2)		SetButton(BUTTON_PSX_L2);
	if (PadValue & PADR1)		SetButton(BUTTON_PSX_R1);
	if (PadValue & PADR2)		SetButton(BUTTON_PSX_R2);


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
	
	if (PadValue & PADselect)
	{
		if (!(mCheatStarted))
		{
			mCheatStarted = TRUE;
			mCheatPos = 0;
		}
		
		if (IDB(BUTTON_PSX_L1))
		{
			if (mCheatPos < 19)
				mCheatKeys[mCheatPos ++] = 1;
		}
	
		if (IDB(BUTTON_PSX_L2))
		{
			if (mCheatPos < 19)
				mCheatKeys[mCheatPos ++] = 2;
		}
	
		if (IDB(BUTTON_PSX_R1))
		{
			if (mCheatPos < 19)
				mCheatKeys[mCheatPos ++] = 3;
		}
	
		if (IDB(BUTTON_PSX_R2))
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
					mCheatAvailable = c0;
					mCheatStarted = FALSE;
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

#endif
