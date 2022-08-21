#include	"Common.h"

#if TARGET == DC

#include	"MemBuffer.h"  //(sic)
#include	"Globals.h"

#include 	"DCBuildType.h"
#include	"Career.h"
#include 	"maincode.h"
#include    "samplelist.h"
//**********************************************************************************************
#define StopAllSounds() for(int i=0;i<MaxChannels;i++) {SDSKillSample(i); mSoundEvent[i].mOwner = NULL;}

//**********************************************************************************************



BOOL CDCSoundManager::Init()
{
#ifdef JCLDCNOPSX		// Rewrite for DC

	/*-----------------09/05/00 :33-------------------
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
#endif
	mVab=-1;                            // Make sure the bank allocator knows there's nothing allocated..

	Reset();

	return TRUE;
}

//**********************************************************************************************
void	CDCSoundManager::Reset()
{
	for(int i=0;i<MaxChannels;i++)
	{
		mSoundEvent[i].mChannel = i;
		mSoundEvent[i].mOwner = NULL;
		mSoundEvent[i].mStatus= SPU_OFF;
	}
	mSoundState = 0; // ok!
}

//**********************************************************************************************
int CDCSoundManager::Shutdown()
{
	/*-----------------09/05/00 14:33-------------------
	 * See the Init function above...
	 * --------------------------------------------------*/

//	SsEnd();
//	SsQuit();

	return 1;
}


//**********************************************************************************************
void CDCSoundManager::AllocSampleBank(char *name)
{

}

//**********************************************************************************************
SINT	CDCSoundManager::GetVolumeForThing(CThing *t)
{
	if (!t)
		return 127;

	float	dist = (GVectorToFVector(WORLD.GetPrimaryPosition(0) - t->GetPos())).Magnitude();
	if (GAME.IsMultiPlayer())
	{
		// is player 2 closer?
		float d2 = (GVectorToFVector(WORLD.GetPrimaryPosition(1) - t->GetPos())).Magnitude();
		if (d2 < dist)
			dist = d2;
	}

	SINT	vol = 25 - SINT(dist);
	if (vol > 20) vol = 20;
	if (vol < 0 ) vol = 0;
	vol *= 6;

	return vol;
}

//**********************************************************************************************
SINT	CDCSoundManager::Fade(SINT v, SINT n)
{
	return (G(v) *
	        mSoundEvent[n].mMasterVolume *
	        mSoundEvent[n].mSubVolume *
	        (G(CAREER.mSoundVolume) / G(10))).Whole();
}

//**********************************************************************************************
void	CDCSoundManager::UpdateStatus()
{
//#ifdef JCLDCNOPSX		// Rewrite for DC
//	printf("Update Status called\n");

	int cnt;

	//SsSeqCalledTbyT();

//	SpuGetAllKeysStatus(mChannelStatus);

	for(cnt=0;cnt<MaxChannels;cnt++)
	{
		SSoundEvent	*se = &mSoundEvent[cnt];

//		se->mStatus=mChannelStatus[cnt];
		VALIDATE(se->mOwner);  // Make sure any THINGs are still active...
		se->mTime ++;

		if((se->mTime > 5) && ((se->mStatus==SPU_OFF)))
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
							//SDS SsUtKeyOffV(cnt);
							SDSKillSample(cnt);
							se->mOwner = NULL;
						}
					}
				}

				if (se->mSubVolume != G0)
				{
					SINT	vol = GetVolumeForThing(se->mOwner);
					vol = Fade(vol, cnt);
					//SDS SsUtSetVVol(cnt, vol, vol);
					SDSVolume(cnt,vol);
				}
			}

			if (se->mTrack)
			{
				if (!(se->mOwner))
				{
					// he's gone - kill the sample!
					//SsUtKeyOffV(cnt);
					SDSKillSample(cnt);
				}
				else
				{
					// still here - track the volume
					SINT	vol = GetVolumeForThing(se->mOwner);

					if (vol == 0)
					{
						// too far away - kill the sample
						//SsUtKeyOffV(cnt);
						SDSKillSample(cnt);
						se->mOwner = NULL;
					}
					else
					{
						vol = Fade(vol, cnt);
						//SsUtSetVVol(cnt, vol, vol);
						SDSVolume(cnt,vol);
					}
				}
			}
		}
	}
//	SsSeqCalledTbyT();

	//SpuGetAllKeysStatus(mChannelStatus);

	for(cnt=0;cnt<MaxChannels;cnt++)
	{
		SSoundEvent	*se = &mSoundEvent[cnt];
		se->mStatus=SDSIsChannelPlaying((int)cnt) ? SPU_ON:SPU_OFF;
	}

}

//**********************************************************************************************
// Returns the Event number the sample was started on or -1 if Error
SSoundEvent* CDCSoundManager::GetSoundEvent()
{
	// SDS - thid loop started from 4 instead of 1
	int i;
	for(i=0;i<MaxChannels;i++)if((mSoundEvent[i].mStatus==SPU_OFF))break;
	 if ((i>=56) && (i<=63)) return NULL;
	if(i==MaxChannels)return NULL;                 // No free channel at the moment....
	return &mSoundEvent[i];

}

//**********************************************************************************************
BOOL	NOT_CURSOR(SINT id)
{
	return ((id != SOUND_CANCEL) && (id != SOUND_MOVE) && (id != SOUND_SELECT));
}

SINT 	CDCSoundManager::PlaySample(const short id, GINT volume, BOOL once, GINT fade)
{

	if ((mSoundState != 0) && (NOT_CURSOR(id)))
		return -1;

	if (once)
	{
		// check that this sample isn't already being played
		SINT	c0;

		for (c0 = 0; c0 < MaxChannels; c0 ++)
		{
//			printf("Status of %d : %d, sample %d\n", c0, mSoundEvent[c0].mStatus, mSoundEvent[c0].mSample);

			if ((mSoundEvent[c0].mStatus != SPU_OFF) &&
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
	//SDS SsUtKeyOnV(event->mChannel,mVab,id,0,60,0, vol, vol);
	SDSPlaySample(event->mChannel,id,vol,IsSampleLooping(id));
	
	//SDS SsSeqCalledTbyT();

	return event->mChannel;

}

//**********************************************************************************************
SINT 	CDCSoundManager::PlaySample(const short id, CThing *thing, GINT volume, BOOL track, BOOL once, GINT fade)
{

	if (mSoundState != 0)
		return -1;

//	if ((id != SOUND_GENER_DIE) && (id != SOUND_GENERATOR))
//		return;

	if (once)
	{
		// check that this sample isn't already being played
		SINT	c0;

		for (c0 = 0; c0 < MaxChannels; c0 ++)
		{
			if ((mSoundEvent[c0].mStatus != SPU_OFF) &&
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
		SDSPlaySample(event->mChannel,id,vol,IsSampleLooping(id));
		//SDS SsUtKeyOnV(event->mChannel,mVab,id,0,60,0,vol, vol);
		event->mStatus=SPU_ON;          // So any other triggered events dont overwrite this channel

		//SsSeqCalledTbyT();

//		printf("Thing for sample: %X\n", event->mOwner);

		return event->mChannel;
	}
	else
		return -1;

}

//**********************************************************************************************
void CDCSoundManager::KillSamples(CThing *thing)
{

	int i;
	for(i=0;i<MaxChannels;i++)
	{
		if((mSoundEvent[i].mOwner==thing)&&((mSoundEvent[i].mStatus==SPU_ON)))
		{
			//SDS SsUtKeyOffV(i);
			SDSKillSample(i);
			mSoundEvent[i].mStatus = SPU_OFF;
			mSoundEvent[i].mOwner  = NULL;
		}
	}

}

//**********************************************************************************************
void	CDCSoundManager::KillSample(CThing *thing, const short id)  // kill a specific sample
{

	int i;
	for(i=0;i<MaxChannels;i++)
	{
		if((mSoundEvent[i].mOwner  == thing) &&
		   (mSoundEvent[i].mSample == id) &&
		   ((mSoundEvent[i].mStatus==SPU_ON)))
		{
			//SDS SsUtKeyOffV(i);
			SDSKillSample(i);
			mSoundEvent[i].mStatus = SPU_OFF;
			mSoundEvent[i].mOwner  = NULL;
		}
	}

}

//**********************************************************************************************
SINT CDCSoundManager::OverrideSample(const short id, CThing *thing)
{
	KillSamples(thing);
	return PlaySample(id,thing);
}

//**********************************************************************************************
void	CDCSoundManager::FadeTo(const short id, GINT fadeval, GINT speed, CThing *t)
{
	if (mSoundState != 0)
		return;

//	printf("Fade To: %d\n", id);

	// first find the sample
	SINT	c0;

	for (c0 = 0; c0 < MaxChannels; c0 ++)
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
void	CDCSoundManager::FadeAllSamples()
{
//	printf("Fade All called\n");

	SINT	c0;

	for (c0 = 0; c0 < MaxChannels; c0 ++)
	{
		mSoundEvent[c0].mFadeDest = G0;
		mSoundEvent[c0].mFade = -G(0, 3000);
	}
	mSoundState = 1; // fading!
}

//**********************************************************************************************
void	CDCSoundManager::KillAllSamples()
{
StopAllSounds();
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

#ifdef JCLDCNOPSX		// PSX Structures...

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

#endif

int CDCSoundManager::PlayTrack(int track,int mode)
{
PlayStream(track+1);

#ifdef JCLDCNOPSX		// Rewrite for DC   (Essentially play DC Audio Track #n)

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

#endif
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

#define	MUSIC_RATIO	18 	// * 10
//#define	MUSIC_RATIO	25 	// * 10

#ifdef JCLDCNOPSX		// Not needed (called from above)
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
#endif

void CDCSoundManager::StopTrack(void)
{
StopStream();
#ifdef JCLDCNOPSX		// Rewrite for DC - stop playing current track

#if ENABLE_PROFILER==1
	return;
#endif
	UnprepareXA();
	
#endif
}



#ifdef JCLDCNOPSX		// not needed ???


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
#endif

//***********************************************************************************
void	CDCSoundManager::DeclareVolumeChange()
{
	SINT vol = (CAREER.mMusicVolume * MUSIC_RATIO) / 10;
	SetStreamVolume(vol);
	
	if (CAREER.mStereo)
		SDSStereoMono(0); // Stereo
	else
		SDSStereoMono(1); // Mono !!!!
		
			
#ifdef JCLDCNOPSX		// Rewrite for DC - set overall volumes for music. sfx, and change between mono & stereo

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

#endif
}


#endif