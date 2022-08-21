#include	"Common.h"

#include	"MemBuffer.h"  //(sic)
#include	"Globals.h"

#include <libspu.h>
#include <libsnd.h>
#include <libcd.h>


#include 	"PSXBuildType.h"
#include	"Career.h"

//**********************************************************************************************
#define StopAllSounds() for(int i=0;i<24;i++) {SsUtKeyOffV(i); mSoundEvent[i].mOwner = NULL;}

//**********************************************************************************************
BOOL CPSXSoundManager::Init()
{
	/*-----------------09/05/00 14:33-------------------
	 * THIS is why the movie player fecks up...
	 *  Need to do this *BEFORE* CdInit();
	 * Bladdy library interaction!!! :o(
	 * --------------------------------------------------*/
//	SsInit();		/* reset sound */
	SsSetTableSize (NULL,0,0);          // Only used by the SEQ player
	SsSetTickMode(SS_NOTICK);           // Call internal manager at 50Hz
	SsSetMono();                        // No stereo sound for now..
	SsSetMute(SS_MUTE_OFF);             // Turn off the mute function
	SsSetMVol(127, 127);                // Set master volume to full

	mVab=-1;                            // Make sure the bank allocator knows there's nothing allocated..

	Reset();

	return TRUE;
}

//**********************************************************************************************
void	CPSXSoundManager::Reset()
{
	for(int i=0;i<24;i++)
	{
		mSoundEvent[i].mChannel = i;
		mSoundEvent[i].mOwner = NULL;
	}
	mSoundState = 0; // ok!
}

//**********************************************************************************************
int CPSXSoundManager::Shutdown()
{
	/*-----------------09/05/00 14:33-------------------
	 * See the Init function above...
	 * --------------------------------------------------*/

//	SsEnd();
//	SsQuit();

	return 1;
}


//**********************************************************************************************
void CPSXSoundManager::AllocSampleBank(char *name)
{
	ULONG vabsize;
	CPSXMemBuffer mem;
	// Use the OrderTable as a temporary storage space for loading...
	char *data=(char*)(PLATFORM.GetOrderTable());
	char *headname=data;
	char *bodyname=data+128;
	unsigned char *mVabBodyBuff=(unsigned char*)data+256;


	sprintf(headname,"SOUNDS\\%s.VH",name);
	sprintf(bodyname,"SOUNDS\\%s.VB",name);

	if(mVab!=-1)
	{
		StopAllSounds();
		SsVabClose(mVab);       // Force the close of any currently open bank...
	}


	mem.InitFromFile(headname);
	mem.Read(mVabHeaderBuff,-1);
	mem.Close();

	VabHdr*vabhdr=((VabHdr*)(&mVabHeaderBuff[0]));
	vabsize=vabhdr->fsize-(2080+((vabhdr->ps+1)*512));

	mVab = SsVabOpenHeadSticky (mVabHeaderBuff, -1, 0x1100);
	if (mVab < 0)
	{
		printf ("SsVabOpenHead : failed!\n");
		while(1);
	}

	//SsVabTransCompleted(SS_WAIT_COMPLETED);  // Block until last transfer (if any) has ended
	mem.InitFromFile(bodyname);
	while(vabsize>2048)
	{
		mem.Read(mVabBodyBuff,2048);       // Read 64 bytes to membuffer
		if(SsVabTransBodyPartly(mVabBodyBuff,2048,mVab)==-1)  // Transfer 64 bytes to SndRam
		{
			printf("SSVabTransBodyPartly failed!\n");
			while(1);
		}
		SsVabTransCompleted(SS_WAIT_COMPLETED);  // Block until last transfer (if any) has ended
		vabsize-=2048;
	}
	if(vabsize)
	{
//		printf("Fraction size: %ed\n",vabsize);
		mem.Read(mVabBodyBuff,vabsize);
		SsVabTransBodyPartly(mVabBodyBuff,vabsize,mVab);
		SsVabTransCompleted(SS_WAIT_COMPLETED);  // Block until last transfer (if any) has ended
	}
	mem.Close();

	Reset();
}

//**********************************************************************************************
SINT	CPSXSoundManager::GetVolumeForThing(CThing *t)
{
	if (!t)
		return 127;

	GINT	dist = (WORLD.GetPrimaryPosition(0) - t->GetPos()).Magnitude();
	if (GAME.IsMultiPlayer())
	{
		// is player 2 closer?
		GINT d2 = (WORLD.GetPrimaryPosition(1) - t->GetPos()).Magnitude();
		if (d2 < dist)
			dist = d2;
	}

	SINT	vol = 25 - dist.Whole();
	if (vol > 20) vol = 20;
	if (vol < 0 ) vol = 0;
	vol *= 6;

	return vol;
}

//**********************************************************************************************
SINT	CPSXSoundManager::Fade(SINT v, SINT n)
{
	return (G(v) *
	        mSoundEvent[n].mMasterVolume *
	        mSoundEvent[n].mSubVolume *
	        (G(CAREER.mSoundVolume) / G(10))).Whole();
}

//**********************************************************************************************
#pragma optimization_level 0

void	CPSXSoundManager::UpdateStatus()
{
//	printf("Update Status called\n");

	int cnt;
	char mChannelStatus[24];
	SsSeqCalledTbyT();

//	SpuGetAllKeysStatus(mChannelStatus);

	for(cnt=0;cnt<24;cnt++)
	{
		SSoundEvent	*se = &mSoundEvent[cnt];

//		se->mStatus=mChannelStatus[cnt];
		VALIDATE(se->mOwner);  // Make sure any THINGs are still active...
		se->mTime ++;

		if((se->mTime > 5) && ((se->mStatus==SPU_OFF)||(se->mStatus==SPU_ON_ENV_OFF)))
			se->mOwner=NULL;
		else
		{
			if (se->mFade != G0)
			{
				se->mSubVolume += se->mFade;
				if (se->mFade > G0)
				{
					if (se->mSubVolume > se->mFadeDest)
					{
						se->mSubVolume = se->mFadeDest;
						se->mFade = G0;
					}
				}
				else
				{
//					printf("Fading down channel %d, id %d : sub vol %d, fade dest %d, fade %d\n", cnt, se->mSample, se->mSubVolume.mVal, se->mFadeDest.mVal, se->mFade.mVal);
					if (se->mSubVolume < se->mFadeDest)
					{
						se->mSubVolume = se->mFadeDest;
						se->mFade = G0;

						if (se->mFadeDest == G0)
						{
							// done - stop sample
							SsUtKeyOffV(cnt);
							se->mOwner = NULL;
						}
					}
				}

				if (se->mSubVolume != G0)
				{
					SINT	vol = GetVolumeForThing(se->mOwner);
					vol = Fade(vol, cnt);
					SsUtSetVVol(cnt, vol, vol);
				}
			}

			if (se->mTrack)
			{
				if (!(se->mOwner))
				{
					// he's gone - kill the sample!
					SsUtKeyOffV(cnt);
				}
				else
				{
					// still here - track the volume
					SINT	vol = GetVolumeForThing(se->mOwner);

					if (vol == 0)
					{
						// too far away - kill the sample
						SsUtKeyOffV(cnt);
						se->mOwner = NULL;
					}
					else
					{
						vol = Fade(vol, cnt);
						SsUtSetVVol(cnt, vol, vol);
					}
				}
			}
		}
	}
//	SsSeqCalledTbyT();

	SpuGetAllKeysStatus(mChannelStatus);
	for(cnt=0;cnt<24;cnt++)
	{
		SSoundEvent	*se = &mSoundEvent[cnt];
		se->mStatus=mChannelStatus[cnt];
	}
}
#pragma optimization_level 4

//**********************************************************************************************
// Returns the Event number the sample was started on or -1 if Error
SSoundEvent* CPSXSoundManager::GetSoundEvent()
{
	int i;
	for(i=4;i<24;i++)if((mSoundEvent[i].mStatus==SPU_OFF)||(mSoundEvent[i].mStatus==SPU_ON_ENV_OFF))break;
	if(i==24)return NULL;                 // No free channel at the moment....
	return &mSoundEvent[i];
}

//**********************************************************************************************
BOOL	NOT_CURSOR(SINT id)
{
	return ((id != SOUND_CANCEL) && (id != SOUND_MOVE) && (id != SOUND_SELECT));
}

SINT 	CPSXSoundManager::PlaySample(const short id, GINT volume, BOOL once, GINT fade)
{
	if ((mSoundState != 0) && (NOT_CURSOR(id)))
		return -1;

	if (once)
	{
		// check that this sample isn't already being played
		SINT	c0;

		for (c0 = 0; c0 < 24; c0 ++)
		{
//			printf("Status of %d : %d, sample %d\n", c0, mSoundEvent[c0].mStatus, mSoundEvent[c0].mSample);

			if ((mSoundEvent[c0].mStatus != SPU_OFF) &&
			    (mSoundEvent[c0].mStatus != SPU_ON_ENV_OFF) &&
			    (mSoundEvent[c0].mSample == id))
			    return -1; // ah...
		}
	}

	SSoundEvent*event=GetSoundEvent();
	if(!event)return-1;
	// channel,bank,prog,tone,pitch,fine,lvol,rvol

	event->mOwner=NULL;
	event->mStatus=SPU_ON;          // So any other triggered events dont overwrite this channel
	event->mMasterVolume = volume;
	event->mSample 	= id;
	event->mTrack = FALSE;
	event->mTime = 0;

	if (fade == G0)
	{
		event->mFade = G0;
		event->mSubVolume = G1;
	}
	else
	{
		if (fade < G0)
		{
			event->mSubVolume = G1;
			event->mFade = fade;
			event->mFadeDest = G0;
		}
		else
		{
			event->mSubVolume = G0;
			event->mFade = fade;
			event->mFadeDest = G1;
		}
	}

	SINT	vol = Fade(127, event->mChannel);
	SsUtKeyOnV(event->mChannel,mVab,id,0,60,0, vol, vol);

	SsSeqCalledTbyT();

	return event->mChannel;
}

//**********************************************************************************************
SINT 	CPSXSoundManager::PlaySample(const short id, CThing *thing, GINT volume, BOOL track, BOOL once, GINT fade)
{
	if (mSoundState != 0)
		return -1;

//	if ((id != SOUND_GENER_DIE) && (id != SOUND_GENERATOR))
//		return;

	if (once)
	{
		// check that this sample isn't already being played
		SINT	c0;

		for (c0 = 0; c0 < 24; c0 ++)
		{
			if ((mSoundEvent[c0].mStatus != SPU_OFF) &&
			    (mSoundEvent[c0].mStatus != SPU_ON_ENV_OFF) &&
			    (mSoundEvent[c0].mSample == id) &&
			    (mSoundEvent[c0].mOwner == thing))
			    return -1; // ah...
		}
	}

	SINT v = GetVolumeForThing(thing);
	if (v == 0)
		return -1; // too far away anyway...

	SSoundEvent *event=GetSoundEvent();
	if(event)
	{
		event->mOwner 	= thing;            // Set owner info
		event->mSample 	= id;
		event->mTrack 	= track;
		event->mMasterVolume  = volume;
		event->mTime = 0;

		if (fade == G0)
		{
			event->mFade = G0;
			event->mSubVolume = G1;
		}
		else
		{
			if (fade < G0)
			{
				event->mSubVolume = G1;
				event->mFade = fade;
				event->mFadeDest = G0;
			}
			else
			{
				event->mSubVolume = G0;
				event->mFade = fade;
				event->mFadeDest = G1;
			}
		}
		// channel,bank,prog,tone,pitch,fine,lvol,rvol

		SINT 	vol = Fade(GetVolumeForThing(thing), event->mChannel);

		SsUtKeyOnV(event->mChannel,mVab,id,0,60,0,vol, vol);
		event->mStatus=SPU_ON;          // So any other triggered events dont overwrite this channel

		SsSeqCalledTbyT();

//		printf("Thing for sample: %X\n", event->mOwner);

		return event->mChannel;
	}
	else
	return -1;


}

//**********************************************************************************************
void CPSXSoundManager::KillSamples(CThing *thing)
{
	int i;
	for(i=0;i<24;i++)
	{
		if((mSoundEvent[i].mOwner==thing)&&((mSoundEvent[i].mStatus==SPU_ON)||(mSoundEvent[i].mStatus==SPU_ON_ENV_OFF)))
		{
			SsUtKeyOffV(i);
			mSoundEvent[i].mStatus = SPU_OFF;
			mSoundEvent[i].mOwner  = NULL;
		}
	}
}

//**********************************************************************************************
void	CPSXSoundManager::KillSample(CThing *thing, const short id)  // kill a specific sample
{
	int i;
	for(i=0;i<24;i++)
	{
		if((mSoundEvent[i].mOwner  == thing) &&
		   (mSoundEvent[i].mSample == id) &&
		   ((mSoundEvent[i].mStatus==SPU_ON)||(mSoundEvent[i].mStatus==SPU_ON_ENV_OFF)))
		{
			SsUtKeyOffV(i);
			mSoundEvent[i].mStatus = SPU_OFF;
			mSoundEvent[i].mOwner  = NULL;
		}
	}
}

//**********************************************************************************************
SINT CPSXSoundManager::OverrideSample(const short id, CThing *thing)
{
	KillSamples(thing);
	return PlaySample(id,thing);
}

//**********************************************************************************************
void	CPSXSoundManager::FadeTo(const short id, GINT fadeval, GINT speed, CThing *t)
{
	if (mSoundState != 0)
		return;

//	printf("Fade To: %d\n", id);

	// first find the sample
	SINT	c0;

	for (c0 = 0; c0 < 24; c0 ++)
	{
		if ((mSoundEvent[c0].mSample == id) && (mSoundEvent[c0].mOwner == t))
		{
			mSoundEvent[c0].mFadeDest = fadeval;
			if (mSoundEvent[c0].mFadeDest < mSoundEvent[c0].mSubVolume)
				mSoundEvent[c0].mFade = -speed;
			else
				mSoundEvent[c0].mFade = speed;

			return;
		}
	}
//	printf("Couldn't find sample to fade:%d \n", id);
}

//**********************************************************************************************
void	CPSXSoundManager::FadeAllSamples()
{
//	printf("Fade All called\n");

	SINT	c0;

	for (c0 = 0; c0 < 24; c0 ++)
	{
		mSoundEvent[c0].mFadeDest = G0;
		mSoundEvent[c0].mFade = -G(0, 3000);
	}
	mSoundState = 1; // fading!
}

//**********************************************************************************************
void	CPSXSoundManager::KillAllSamples()
{
	StopAllSounds();
	SsSeqCalledTbyT();
}


//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************




#define XA_STOPPED 0
#define XA_PLAYING_SINGLE 1
#define XA_PLAYING_CIRCULAR 2


// .XA file structure
#define NUMCHANNELS 6

typedef struct {
	char*	filename;
	int		startpos;
	int		endpos;
	int		curtrack;
	int		playing;
	CdlCB	OldCallBack;

} XAFILE;

XAFILE theXAFile[1] =
{
	"\\MOHODATA\\MUSICALL.XA;1",		// File name
	0,									// File start position, filled in by CdSearchFile
	0,									// File end position filled in by CdSearchFile
	0,									// Keep any old CdCallback, filled in by SOUND.PlayTrack
	0,                                  // Currently playing track
	XA_STOPPED                                  // Playing status.
};

// CD buffer, don't really need this as we are not interested in the data
// itself coming in
u_char buffer[16];
u_long *chanAddress=(u_long *)&buffer[0];
// Function Prototypes
void	PlayXAChannel(int channel, int startPos, int endpos);
void	cbready(int intr, u_char *result);
CdlCB	PrepareXA(void);
void	UnprepareXA(void);
int		main(void);

int currentPos=0;       // current position of CD
int fileNo=0;           // file number we wish to play

int currentChannel;		// Channel number of video header
int ID;					// ID of sector, 352 for video
int gChannel;			// Channel number we are currently playing


//int tlist[8]={1,2,3,0};
//int tlist[8]={2, 3, 4, 2, 2, 2, 0};

int CPSXSoundManager::PlayTrack(int track,int mode)
{
#if ENABLE_PROFILER==1
	return 0;
#endif

	CdlFILE fp;				// CD file details


// Set up for XA playing
		if(theXAFile[0].playing==XA_STOPPED)
		{
			if(CdSearchFile(&fp,theXAFile[0].filename) == 0)
			{
				printf("%s: not found\n", theXAFile[0].filename);
				return 1;
			}
        	// get XA file start position
			theXAFile[0].startpos = CdPosToInt(&fp.pos);

        	// get XA file end position, start pos + number of sectors -1
        	theXAFile[0].endpos = theXAFile[0].startpos + (fp.size/2048) -1;
 			theXAFile[0].OldCallBack = PrepareXA();
		}
		PlayXAChannel(track,theXAFile[0].startpos,theXAFile[0].endpos);
		theXAFile[0].playing=XA_PLAYING_CIRCULAR;

	return(0);

}

/*
*
*       NAME            void PlayXAChannel(int startPos, int endpos)
*
*       FUNCTION        Plays channel zero of an .XA file
*
*       NOTES
*
*       CHANGED         PROGRAMMER      REASON
*       -------         ----------      ------
*       05/06/98        Mike Kav        Created
*
*/

#define	MUSIC_RATIO	25 	// * 10

void PlayXAChannel(int channelNum, int startPos, int endpos)
{

	CdlLOC  loc;
	CdlFILTER theFilter;

	theXAFile[0].curtrack=channelNum;

	// set volume
	SINT	vol = (CAREER.mMusicVolume * MUSIC_RATIO) / 10;
    SsSetSerialVol(SS_SERIAL_A, vol, vol);


	// set up .XA filter
	theFilter.file=1;
	theFilter.chan=channelNum;
	gChannel=channelNum;
	CdControlF(CdlSetfilter, (u_char *)&theFilter);

	// Starting position on CD
	CdIntToPos(startPos, &loc);
	currentPos=startPos;

	// begin playing
	CdControlF(CdlReadS, (u_char *)&loc);

	return;
}


/*
*
*       NAME            CdlCB PrepareXA(void)
*
*       FUNCTION        Set CD mode and hook in callback for XA playing
*
*       NOTES           Returns the current callback if required
*
*       CHANGED         PROGRAMMER      REASON
*       -------         ----------      ------
*       05/06/98        Mike Kav        Created
*
*/
CdlCB PrepareXA(void)
{
#if 1
	SpuCommonAttr attr;

// setup for CDA playback
	CdControlF(CdlPause,0);

	attr.mvol.left=0x4000;
	attr.mvol.right=0x4000;
	attr.cd.volume.left=0x4000;
	attr.cd.volume.right=0x4000;
	attr.cd.mix=SPU_ON;
	attr.cd.reverb=SPU_ON;
	attr.mask=SPU_COMMON_CDVOLL|SPU_COMMON_CDVOLR|SPU_COMMON_CDMIX|SPU_COMMON_CDREV|SPU_COMMON_MVOLR|SPU_COMMON_MVOLL;

	//CdControl(CdlDemute,NULL,NULL);
	SpuSetCommonAttr(&attr);

//#else

	u_char param[4];

// setup for XA playback
	param[0] = CdlModeSpeed|CdlModeRT|CdlModeSF;
	CdControlB(CdlSetmode, param, 0);
	CdControlF(CdlPause,0);

	return CdReadyCallback((CdlCB)cbready);
#endif
}

/*
*
*       NAME            void UnprepareXA(CdlCB oldCallback)
*
*       NOTES           Returns the current callback if required
*
*       CHANGED         PROGRAMMER      REASON
*       -------         ----------      ------
*       05/06/98        Mike Kav        Created
*
*/

void UnprepareXA(void)
{
	u_char param[4];

// Reset any Callback that we replaced
	if(theXAFile[0].playing!=XA_STOPPED)
	{
		// Stop CDA playing....
		CdControlF(CdlPause,0);
		CdReadyCallback(theXAFile[0].OldCallBack);

		theXAFile[0].playing=XA_STOPPED;
	}
	// clear XA mode
	param[0] = CdlModeSpeed;
	CdControlB(CdlSetmode, param, 0);
	return;
}

void CPSXSoundManager::StopTrack(void)
{
#if ENABLE_PROFILER==1
	return;
#endif
	UnprepareXA();
}





void cbready(int intr, u_char *result)
{
	if (intr == CdlDataReady)
	{
		unsigned char buf[8];
		CdGetSector((unsigned long*)buf,2);



		//printf("Bytes from header: %02x %02x %02x %02x %02x %02x %02x %02x\n",
		 //		buffer[0],
		 //		buffer[1],
		 //		buffer[2],
		 //		buffer[3],
		 //		buffer[4],
		 //		buffer[5],
		 // 		buffer[6],
		 //		buffer[7]);


		#if 1

		ID = *(unsigned short *)(buf);

		currentChannel = *((unsigned short *)(buf+2));
		currentChannel = (currentChannel&31744)>>10;

	// If this is a video sector then check that this is the channel
 	// you want then stop playing the .XA sample
		if( (ID == 352) && (currentChannel == gChannel) )
		{
//			printf("Callback! (chan %d)\n",currentChannel);
			if(theXAFile[0].playing==XA_PLAYING_SINGLE)
			{
//				printf("Song finished... Stopping\n");
				CdControlF(CdlPause,0);
		    	SsSetSerialVol(SS_SERIAL_A,0,0);
				UnprepareXA();
			}
			else
			{
				theXAFile[0].curtrack=(theXAFile[0].curtrack+1)%NUMCHANNELS;
//				printf("Skipping to track: %d\n",theXAFile[0].curtrack);
				SOUND.PlayTrack(theXAFile[0].curtrack,XA_PLAYING_CIRCULAR);

			}
		}

		#endif


	}
	//else
	//	printf("UnHandled Callback Occured (%d)\n",intr);
}


//***********************************************************************************
void	CPSXSoundManager::DeclareVolumeChange()
{
	SINT	vol;

	// music
	vol = (CAREER.mMusicVolume * MUSIC_RATIO) / 10;
    SsSetSerialVol(SS_SERIAL_A, vol, vol);

	// sound
	UpdateStatus();

	// stereo
	if (CAREER.mStereo)
		SsSetStereo();
	else
		SsSetMono();

}
