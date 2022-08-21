/* * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * PCSoundManager.cpp
 * 
 * First Revision 8/9/2000
 *
 * Overview:
 *
 * This file runs all the direct sound interface.
 * The accompyning header file defines all the methods used here.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * */
#include "common.h"
#include "Globals.h"
#include "PCSoundManager.h" //header file for this class
#include "Vfw.h"
#include "pcsetupquery.h"
#include "VolumeOutMaster.h"

extern	char			MoHoCDPath[MAX_PATH];

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

	MCI_OPEN_PARMS		CPCSoundManager::mciOpenParms;
	MCI_SET_PARMS		CPCSoundManager::mciSetParms;
	MCI_PLAY_PARMS		CPCSoundManager::mciPlayParms;
	MCI_SYSINFO_PARMS	CPCSoundManager::mciSysInfoParms;
	MCI_STATUS_PARMS	CPCSoundManager::mciStatusParms;
	BOOL				CPCSoundManager::CDInitialised	= FALSE;
	BOOL 				CPCSoundManager::CDRepeat		= FALSE;
	BOOL 				CPCSoundManager::CDDeviceOpen	= FALSE;
	UINT				CPCSoundManager::CDDeviceID		= 0;
	UINT				CPCSoundManager::CDPlayFrom		= 0;
	UINT				CPCSoundManager::CDPlayTo		= 0;
	int					CPCSoundManager::CDauxdevice	= -1;
	int					CPCSoundManager::CDNumTracks	= 0;
	int					CPCSoundManager::CDCurrentTrack	= 0;
	int					CPCSoundManager::CDTimer		= -1;
	int					CPCSoundManager::num_devs;
	int					CPCSoundManager::SizeOfBuffers	= 32;
	BOOL				CPCSoundManager::IsCDPlaying	= FALSE;

	/********************/
	/* PUBLIC VARIABLES */
	/********************/
	int			theTrack = 0;;
	IVolume*	pCDVolume;
	int			CheckTrackCount = 0;
	int			windowsVolume;
	BOOL		CanISetCDVolumeReturn;
	HANDLE		playThread;
	DWORD		dwPlayThreadId = 0;

/********************************************************/
BOOL	CPCSoundManager::Init(HWND handleToWindow) {	

	LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;
	LPDIRECTSOUNDBUFFER ptemp_DSB_Secondary = NULL;
	IsCDPlaying = FALSE;
	
	if (mySystem)
	{
 		for (int i = mySystem->GetNumberOfSoundDevices(); i--;)
		{
			// Look up selected sound device in enumerated device list. 
			// If match and supports hardware mixing then set to 24 buffers, else 12. (assuming software).
			if (	(mySystem->GetSoundDevice(i)->Guid == SelectedSoundDevice)
				&&	(
						(mySystem->GetSoundDevice(i)->Caps.dwFlags & DSCAPS_SECONDARYMONO)
					||	(mySystem->GetSoundDevice(i)->Caps.dwFlags & DSCAPS_SECONDARYSTEREO)
					)
				)
			{
				SizeOfBuffers = 32;
				break;
			}
			else
			{
				SizeOfBuffers = 24;
			}
		}
	}
	else
	{
		// Hmmm, how did we get this far with no mySystem object? 
		// Assuming code has changed. Assuming no hardware support. 
		// >> Setting 12 sound channels.
		SizeOfBuffers = 12;
	}
	
    // Initialize COM
    if(CoInitialize( NULL ) ) 
	{
    	OutputDebugString("Failure to init Com\n");
		return FALSE;
	}
	
	// We really should init the sound event structure!!

	for( int i = 0; i < SizeOfBuffers; i++)
	{
		mSoundEvent[i].mOwner = NULL;             // or NULL if the current channel has no owner....
		mSoundEvent[i].mChannel = i;           // Channel number.... Funnily enough, the same as the array index...
		mSoundEvent[i].mStatus = SPU_OFF;            // See above....
		mSoundEvent[i].mSample = 0;            // Sample number for this event...
		mSoundEvent[i].mTrack = FALSE;				// Is the sample to track an object across screen?
		mSoundEvent[i].mMasterVolume = 0;		// The Master Volume
		mSoundEvent[i].mSubVolume = 0;			// The Sub Volume
		mSoundEvent[i].mFade= G0;				// The panning of the sound across the speakers
		mSoundEvent[i].mFadeDest = G0;			// Which way the panning is moving.
		mSoundEvent[i].mTime = G0; 		
	}

    // Create IDirectSound using the primary sound device
    if( FAILED(DirectSoundCreate(&SelectedSoundDevice, &g_pDS, NULL ) ) ) {
        
		OutputDebugString("failed to create direct sound with selected device\n");
		return FALSE;
	}

    // Set coop level to DSSCL_PRIORITY 
    if( FAILED(g_pDS->SetCooperativeLevel( handleToWindow, DSSCL_PRIORITY ) ) ) {
        
		return FALSE;
	}
    
    // Get the primary buffer 
    DSBUFFERDESC        dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
    if( FAILED( g_pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) ) {
        
		
		return FALSE;
	}

    // Set primary buffer format to 22kHz and 16-bit output.
    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    if(CAREER.mStereo == TRUE)
	{
		wfx.nChannels       = 2;
	}
	else
	{
		wfx.nChannels       = 1;
	}
    wfx.nSamplesPerSec  = 22050; 
    wfx.wBitsPerSample  = 16; 
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED(pDSBPrimary->SetFormat(&wfx) ) ) {
     
		
		return FALSE;
	}
	
    SAFE_RELEASE( pDSBPrimary );

	//now init the cd audio stuff
	if(InitCDAudio() != TRUE)
	{
		OutputDebugString("Cannot start cd audio!!\n");
	}
	
	return TRUE;

}; // Initialise DirectSound.

/********************************************************/

void	CPCSoundManager::Shutdown() {

//	SAFE_DELETE( g_pWaveSoundRead );
    SAFE_RELEASE( g_pDSBuffer );
    for(int i = 0; i<24; i++) {
		SAFE_RELEASE(DSBuffer[i]);
	}

	for(int x = 0; x <64; x++)
	{
			SAFE_DELETE (WavPointers[x].pbWavData);
			WavPointers[x].pbWavData = NULL;
	}
	StopCDTrack();
	DeInitCDAudio();
	
	SAFE_RELEASE( g_pDS ); 
	
    // Release COM
    CoUninitialize();
}; //close down directx stuff

/********************************************************/
/*
	This method loads all the information about the sound files which the buffers
	need into an array. This is to make the loading of multiple files quicker
	as the information of the wav file is ready to be transferred without
	the process of accessing the straight wave file each time it is needed.
*/
void	CPCSoundManager::AllocSampleBank(char *name)	{
	
	char strFileName[256];

	if(WavPointers[0].pbWavData == NULL)
	{
		
		for(int i = 0; i <64; i++)
		{
			sprintf(strFileName, "data\\sounds\\%d.wav", i);
					
//			g_pWaveSoundRead = new CWaveSoundRead();

			// Load the wave file
			if (!(g_pWaveSoundRead.Open( strFileName )))
			{
				// Reset the wave file to the beginning 
				g_pWaveSoundRead.Reset();
		
				// The size of wave data is in pWaveFileSound->m_ckIn
				INT nWaveFileSize = g_pWaveSoundRead.m_ckIn.cksize;
	
				// Allocate that buffer.
				WavPointers[i].pbWavData = new BYTE[ nWaveFileSize ];
		
				if(FAILED(g_pWaveSoundRead.Read(	nWaveFileSize, 
													WavPointers[i].pbWavData, 
													&WavPointers[i].cbWavSize))) 
				{
					WavPointers[i].pbWavData	= NULL;
					WavPointers[i].cbWavSize	= 0;
				}
				WavPointers[i].cbWavSize = nWaveFileSize;
			
			g_pWaveSoundRead.Close();

			}
		}
	}
		
}; //Setup the sound sample bank

/********************************************************/

void	CPCSoundManager::Reset() {

	for(int i=0;i<SizeOfBuffers;i++)
	{
		mSoundEvent[i].mChannel = i;
		mSoundEvent[i].mOwner = NULL;
		if (DSBuffer[i])
		{
			DSBuffer[i]->Stop();
		}
	}
}; //do something entirely different?

/********************************************************/

//function called to play a sample
SINT	CPCSoundManager::PlaySample(const short id, 
								   GINT volume, 
								   BOOL once, 
								   GINT fade) {
	HRESULT hr;
	
	//if ((mSoundState != 0))
	//	return -1;
	LPDIRECTSOUNDBUFFER ptemp_DSB_Secondary = NULL;

	if (once)
	{
		// check that this sample isn't already being played

		for (SINT c0 = 0; c0 < SizeOfBuffers; c0 ++)
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
		event->mSubVolume = G1; // used to be -10000
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
		
	VOID* pbData  = NULL;
	DWORD dwLength;
	int wavNumber = event->mSample; // this holds the index to the loaded wav sample we want.
	int bufferNum = event->mChannel; //this holds the index to the directsound buffer we want.
	int mChannel = event->mChannel;
	
	if ((WavPointers[wavNumber].cbWavSize) && 
		(WavPointers[wavNumber].pbWavData))
	{
		UINT wavSize  = (WavPointers[wavNumber].cbWavSize);
		
		//lets create a buffer!!!!
		PCMWAVEFORMAT pcmwf; 
		DSBUFFERDESC dsbdesc; 

	   // Set up wave format structure. 
		memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT)); 
		pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM; 
		//set up number of channels ie. 1 for mono. 2 for stereo
		//if(stereo)
		//{
			pcmwf.wf.nChannels       = 1;
		//}
		//else
		//{	
			//pcmwf.wf.nChannels       = 1;
		//}
		
		pcmwf.wf.nSamplesPerSec = 44100; 
		pcmwf.wf.nBlockAlign = 2; 
		pcmwf.wf.nAvgBytesPerSec = 
			pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign; 
		pcmwf.wBitsPerSample = 16; 
 
		// Set up DSBUFFERDESC structure. 
		memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
		dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
		dsbdesc.dwFlags = 
        DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCDEFER; 
 

		dsbdesc.dwBufferBytes = (wavSize); 
		dsbdesc.lpwfxFormat = (LPWAVEFORMATEX)&pcmwf; 
		// Create buffer. 
		
		hr = (g_pDS->CreateSoundBuffer(&dsbdesc, &ptemp_DSB_Secondary, NULL));
		
		if (hr != DS_OK)
		{
			OutputDebugString("Create buffer returned an error code\n");
			return 0;
		}
		
		DSBuffer[mChannel] = ptemp_DSB_Secondary;
		
		DWORD dwStatus;
		DSBuffer[mChannel]->GetStatus( &dwStatus );
		if( dwStatus & DSBSTATUS_BUFFERLOST )
		{
			DSBuffer[mChannel]->Restore();
		}
		
		
		//Firstly lock the corresponding buffer so that we can load data into it.
		
		DSBuffer[mChannel]->Lock(0, 0, &pbData, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER);
		

		//Then set the parameters for that buffer ie.Volume.
		
		DSBuffer[mChannel]->SetVolume(vol);

		//now copy the data from the sample bank to the buffer.
		
		memcpy(pbData, (WavPointers[wavNumber].pbWavData), (WavPointers[wavNumber].cbWavSize));
		
			
		

		//now we can unlock the buffer, we don't need control anymore.
		
		DSBuffer[mChannel]->Unlock( (pbData), 
									(dwLength), 
									NULL,
									0 );
	
		//Now just play the buffer, it gets passed into the the primary buffer and mixed atuomatically.
		
		
		if( IsSampleLooping(wavNumber) != TRUE )
			{
				DSBuffer[mChannel]->Play( 0, 0, NULL );
				event->mStatus=SPU_ON;
			}
			else
			{
				DSBuffer[mChannel]->Play( 0, 0, DSBPLAY_LOOPING );
				event->mStatus=SPU_ON;
			}
				

		
	}
				
	return event->mChannel;
};

/*********************************************************/ 

//called to play a diffdernt type of sample
SINT	CPCSoundManager::PlaySample(const short id, CThing *thing, GINT volume, BOOL track, BOOL once, GINT fade) {
	
	
	LPDIRECTSOUNDBUFFER ptemp_DSB_Secondary = NULL;
	//if (mSoundState != 0)
	//	return -1;

//	if ((id != SOUND_GENER_DIE) && (id != SOUND_GENERATOR))
//		return;

	if (once)
	{
		// check that this sample isn't already being played
		SINT	c0;

		for (c0 = 0; c0 < SizeOfBuffers; c0 ++)
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

		VOID* pbData  = NULL;
		DWORD dwLength;
		int wavNumber = event->mSample; // this holds the index to the loaded wav sample we want.
		int bufferNum = event->mChannel; //this holds the index to the directsound buffer we want.
		int mChannel = event->mChannel;
	
		if ((WavPointers[wavNumber].cbWavSize) && 
			(WavPointers[wavNumber].pbWavData))
			{
			UINT wavSize = (WavPointers[wavNumber].cbWavSize);	
			
			//lets create a buffer!!!!
			PCMWAVEFORMAT pcmwf; 
			DSBUFFERDESC dsbdesc; 

			// Set up wave format structure. 
			memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT)); 
			pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM; 
			//set up the channels. ie 1 for mono. 2 for stereo
			//if(stereo)
			//{
				pcmwf.wf.nChannels = 1;
			//}
			//else
			//{
				//pcmwf.wf.nChannels = 1;
			//}
			pcmwf.wf.nSamplesPerSec = 44100; 
			pcmwf.wf.nBlockAlign = 2; 
			pcmwf.wf.nAvgBytesPerSec = 
				pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign; 
			pcmwf.wBitsPerSample = 16; 
 
			// Set up DSBUFFERDESC structure. 
			memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
			dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
			dsbdesc.dwFlags = 
			DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCDEFER; 
 
		 
			dsbdesc.dwBufferBytes = (wavSize); 
			dsbdesc.lpwfxFormat = (LPWAVEFORMATEX)&pcmwf; 
			// Create buffer. 
			g_pDS->CreateSoundBuffer(&dsbdesc, &ptemp_DSB_Secondary, NULL);
			
			DSBuffer[mChannel] = ptemp_DSB_Secondary;
			//Firstly lock the corresponding buffer so that we can load data into it.
		
			DSBuffer[mChannel]->Lock(0, 0, &pbData, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER);
			//DSBuffer[mChannel]->Lock(0, 0, &pbData, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER); 
 
			//DSBuffer[mChannel]->dwBufferBytes = (WavPointers[wavNumber].pbWavData);		

			//Then set the parameters for that buffer ie.Volume.
			
			DSBuffer[mChannel]->SetVolume(vol);
			//DSBuffer[mChannel]->SetVolume(vol);
	
			//now copy the data from the sample bank to the buffer.
			
			memcpy(pbData, (WavPointers[wavNumber].pbWavData), (WavPointers[wavNumber].cbWavSize));
			
			//now we can unlock the buffer, we don't need control anymore.
			
				DSBuffer[mChannel]->Unlock( (pbData), 
											(dwLength), 
											NULL,
											0 );
			/*
			DSBuffer[mChannel]->Unlock( (WavPointers[wavNumber].pbWavData), 
										(WavPointers[wavNumber].cbWavSize), 
										NULL,
										0 );*/

			//Now just play the buffer, it gets passed into the the primary buffer and mixed atuomatically.
				
				if( IsSampleLooping(wavNumber) != TRUE )
				{
				DSBuffer[mChannel]->Play( 0, 0, NULL );
				event->mStatus=SPU_ON;          // So any other triggered events dont overwrite this channel

				}
				else
				{
				DSBuffer[mChannel]->Play( 0, 0, DSBPLAY_LOOPING );
				event->mStatus=SPU_ON;          // So any other triggered events dont overwrite this channel

				}
			
			}
	
//		printf("Thing for sample: %X\n", event->mOwner);

		return event->mChannel;
	}
	else
 	return -1;
};

/*********************************************************/

//Not called
SINT	CPCSoundManager::OverrideSample(const short id, CThing *thing) {
	KillSamples(thing);
	return PlaySample(id,thing);
};

/*********************************************************/

//stop all samples playing
void	CPCSoundManager::KillSamples(CThing *thing) {
	int i;
	for(i=0;i<SizeOfBuffers;i++)
	{
		if((mSoundEvent[i].mOwner==thing)&&((mSoundEvent[i].mStatus==SPU_ON)))
		{
			DSBuffer[i]->Stop();
			SAFE_RELEASE(DSBuffer[i]);
			DSBuffer[i] = NULL;
			mSoundEvent[i].mStatus = SPU_OFF;
			mSoundEvent[i].mOwner  = NULL;
		}
	}

};

/*********************************************************/
//stop a sample playing
void	CPCSoundManager::KillSample(CThing *thing, const short id) {
	int i;
	for(i=0;i<SizeOfBuffers;i++)
	{
		if((mSoundEvent[i].mOwner  == thing) &&
		   (mSoundEvent[i].mSample == id) &&
		   ((mSoundEvent[i].mStatus==SPU_ON)))
		{
			if(DSBuffer[i] != NULL)
			{
				DSBuffer[i]->Stop();
				SAFE_RELEASE(DSBuffer[i]);
				DSBuffer[i] = NULL;
			}
			
			mSoundEvent[i].mStatus = SPU_OFF;
			mSoundEvent[i].mOwner  = NULL;
		}
	}	

};

/*********************************************************/
	
//sound fade
void	CPCSoundManager::FadeTo(const short id, GINT fadeval, GINT speed, CThing *t) {

	//if (mSoundState != 0) //psx code
	//	return;

//	printf("Fade To: %d\n", id);

	// first find the sample
	SINT	c0;

	for (c0 = 0; c0 < SizeOfBuffers; c0 ++)
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
};

/*********************************************************/
	
//fade and or kill every sample, probably for in game endgame or menu to game fade
void	CPCSoundManager::FadeAllSamples() {
	
	SINT	c0;

	for (c0 = 0; c0 < SizeOfBuffers; c0++)
	{
		mSoundEvent[c0].mFadeDest = G0;
		mSoundEvent[c0].mFade = -G(0, 3000);
	}
	//mSoundState = 1; // fading!
};

/*********************************************************/
//no callers
void	CPCSoundManager::KillAllSamples() {
	int i;
	for(i=0;i<SizeOfBuffers;i++)
	{
			if(DSBuffer[i] != NULL)
			{	
			DSBuffer[i]->Stop();
			SAFE_RELEASE(DSBuffer[i]);
			}
			mSoundEvent[i].mStatus = SPU_OFF;
			mSoundEvent[i].mOwner  = NULL;
	}


};

/*********************************************************/
//called every game turn to update volumes and pans of all sounds
void inline	CPCSoundManager::UpdateStatus() {
	//	printf("Update Status called\n");

	int cnt;
	CCamera *cam = ENGINE.GetCamera();
	int pan;

	
	//char mChannelStatus[24];

//	SpuGetAllKeysStatus(mChannelStatus);

	for(cnt=0;cnt<SizeOfBuffers;cnt++)
	{
		SSoundEvent	*se = &mSoundEvent[cnt];

//		se->mStatus=mChannelStatus[cnt];
		VALIDATE(se->mOwner);  // Make sure any THINGs are still active...
		se->mTime ++;
		
		
		//calculate souund pan
		if(se->mOwner != NULL && CAREER.mStereo == TRUE)
		{
			if(DSBuffer[cnt] != NULL)
			{
				pan = CalculatePan( cam, se->mOwner);
				DSBuffer[cnt]->SetPan(pan);
			}
		}
		
		
		if((se->mTime > 5) && ((se->mStatus==SPU_OFF)))
		{
			se->mOwner=NULL;
		}
		else
		{
			//if this is a sample that should fade
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
					
					if (se->mSubVolume < se->mFadeDest)
					{
						se->mSubVolume = se->mFadeDest;
						se->mFade = G0;

						if (se->mFadeDest == G0)
						{
							// done - stop sample
							
							if(DSBuffer[cnt] != NULL)
							{
								DSBuffer[cnt]->Stop();
								SAFE_RELEASE(DSBuffer[cnt]);
								DSBuffer[cnt] = NULL;
							
								se->mStatus=SPU_OFF;
							}
							
							se->mOwner = NULL;
						}
					}
				}

				if (se->mSubVolume != G0)
				{
					SINT	vol = GetVolumeForThing(se->mOwner);
					vol = Fade(vol, cnt);
					if(DSBuffer[cnt] != NULL)
					{
						DSBuffer[cnt]->SetVolume(vol);
					}
				}
			}

			if (se->mTrack)
			{
				if (!(se->mOwner))
				{
					// he's gone - kill the sample!
					if(DSBuffer[cnt] != NULL)
					{
						DSBuffer[cnt]->Stop();						
						SAFE_RELEASE(DSBuffer[cnt]);
						DSBuffer[cnt] = NULL;
						
						se->mStatus=SPU_OFF;
					}
				}
				else
				{
					// still here - track the volume
					SINT	vol = GetVolumeForThing(se->mOwner);

					if (vol == 0)
					{
						// too far away - kill the sample
						if(DSBuffer[cnt] != NULL)
						{
							DSBuffer[cnt]->Stop();						
							SAFE_RELEASE(DSBuffer[cnt]);
							DSBuffer[cnt] = NULL;
							
							se->mStatus=SPU_OFF;
						}
						
						se->mOwner = NULL;
					}
					else
					{
						vol = Fade(vol, cnt);
						if(DSBuffer[cnt] != NULL)
						{
							DSBuffer[cnt]->SetVolume(vol);
						}
					}
				}
			}
		}
	}
//	SsSeqCalledTbyT();
	DWORD theStatus;
	//SpuGetAllKeysStatus(mChannelStatus);
	for(cnt=0;cnt<SizeOfBuffers;cnt++)
	{
		SSoundEvent	*se = &mSoundEvent[cnt];
		//se->mStatus=mChannelStatus[cnt];
		if(DSBuffer[cnt] !=NULL) 
		{

			DSBuffer[cnt]->GetStatus(&theStatus);
			if(theStatus & (DSBSTATUS_LOOPING || DSBSTATUS_PLAYING ))
			{
				se->mStatus = SPU_ON;
			}
			else
			{
				se->mStatus = SPU_OFF;
				SAFE_RELEASE(DSBuffer[cnt]);
				//DSBuffer[cnt] = NULL;
				
			}
		}
	}

	// now check the cd. If it is playing then leave it.
	// If it isn't playing then leave it.
	// if it is marked as playing but has finished the curent track then
	// advance to the next track.
	// If the track playing is the last one then loop to the start.
	
	CheckTrackCount++;
	if (CheckTrackCount == 30)
	{
	
		BOOL CDWasPlaying = IsCDPlaying;
		CheckCDFunc();
		
		if( (CDWasPlaying == TRUE) && (IsCDPlaying == FALSE) )
		{
			theTrack++;
			if (theTrack < CDNumTracks)
			{
			  PlayTrack(theTrack, 1); 
			}
			else
			{
				theTrack = 2;
				PlayTrack(theTrack, 1);
			}
		
		}
		
		CheckTrackCount = 0;
		
	}
	
};

/*********************************************************/
//**********************************************************************************************
SINT	CPCSoundManager::GetVolumeForThing(CThing *t)
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
//**********************************************************************************************
// Returns the Event number the sample was started on or -1 if Error
SSoundEvent* CPCSoundManager::GetSoundEvent()
{
	int i;
	for(i=0;i<(SizeOfBuffers);i++)
	{
		if((mSoundEvent[i].mStatus != SPU_ON))
		{
			break;
		}
	}
	if(i==(SizeOfBuffers))return NULL;                 // No free channel at the moment....
	return &mSoundEvent[i];
}

//**********************************************************************************************
//**********************************************************************************************
SINT	CPCSoundManager::Fade(SINT v, SINT n)
{
	SINT tv = (G(v) *
	        mSoundEvent[n].mMasterVolume *
	        mSoundEvent[n].mSubVolume *
	        (G(CAREER.mSoundVolume) / G(10))).Whole();

	
	tv = tv*100; //was 350...
	if( tv > 10000)
	{
		tv = 10000;
	}
	tv = ((tv - 10000)/2);
	if(tv < -10000)
	{
		tv = -10000;
	}

	return  tv; 
}

//**********************************************************************************************
float		CPCSoundManager::CalculatePan(CCamera *theCam,CThing *theThing)
{
	if(theCam != NULL && theThing != NULL)
	{
		GVector PlayerPos, CameraPos, ThingPos;
		SINT ClosestPlayer = 0;
		GVector CP, CT;
		FVector CPV, CTV;
		float x, y, ArcTanForP, ArcTanForT, ActualAngle;

		
		if (GAME.IsMultiPlayer())
		{
			GINT	dist = (WORLD.GetPrimaryPosition(0) - theThing->GetPos()).Magnitude();
			// is player 2 closer?
			GINT d2 = (WORLD.GetPrimaryPosition(1) - theThing->GetPos()).Magnitude();
			if (d2 < dist)
				ClosestPlayer = 1;
		}

		PlayerPos = WORLD.GetPrimaryPosition(ClosestPlayer);
		CameraPos = theCam->mPos;
		ThingPos  = theThing->GetPos();

		CP = (PlayerPos - CameraPos);
	
		
		CT =(ThingPos - CameraPos);
		
		GVectorToFVector(&CP, &CPV);
		GVectorToFVector(&CT, &CTV);

		x = CPV.X;
		y = CPV.Y;

		ArcTanForP = atan2(y, x);

		x = CTV.X;
		y = CTV.Y;

		ArcTanForT = atan2(y, x);

		ActualAngle = ArcTanForP - ArcTanForT;

		ActualAngle = sin(ActualAngle);

		ActualAngle = (ActualAngle*500);

		if(ActualAngle < -10000 || ActualAngle > 10000)
		{
			OutputDebugString("too much left or right\n");
			ActualAngle = 0;
		}

		return ActualAngle;
	}
	else
	{
		return 0;
	}
}
//************************************************************************************
int inline CPCSoundManager::FindFreeArrayLocation(LPDIRECTSOUNDBUFFER *theBuffer)
{
	int freeLoc = -1;
	for(int i = 0; i<SizeOfBuffers ;i++)
	{
		if(theBuffer[i] == NULL)
		{
			freeLoc = i;	
		}
	}
	return freeLoc;
}
//**********************************************************************************************
//**********************************************************************************************
/*
 * The following fuctions are all for the playing of cd audio tracks.
 * All have been reused from cd.c and cd.h.
 *
 * 
 * 
 * 
 * The current interface using IVolume interface returns no errors
 * 
 *
 ***********************************************************************************************/

BOOL CPCSoundManager::InitCDAudio()
{
	DWORD aValue = 0;
	int NumOfCDDevs = 0;
	//************
	
	DWORD Drives;
	
	 // Check for MoHo CD - can be found in any [CD] drive.
	Drives = GetLogicalDrives();
	if (Drives)
	{
		for (int i = 0; i < 32; i++)
		{
			if (Drives & (1 << i))
			{
				sprintf(MoHoCDPath, "%c:\\", i + 65);
				if (GetDriveType(MoHoCDPath) == DRIVE_CDROM)
				{
						NumOfCDDevs++;
				}
			}
		}
		  // Ran out of drives to check.
	}
	
	//************
	if( !CDInitialised )
	{
		if(NumOfCDDevs == 1)
		{
			OutputDebugString("only one cd device\n");
			mciOpenParms.lpstrDeviceType = "cdaudio";
			if(mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE, ( DWORD )( LPVOID ) &mciOpenParms ) )
			{
				OutputDebugString( "InitCDAudio: Open CD device failed" );
				return( FALSE );
			}
			
			CDDeviceID = mciOpenParms.wDeviceID;
		}
		else
		{
			CDDeviceID = FindCDDriveToUse(NumOfCDDevs);
		}

		mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;
		mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;
		if(mciSendCommand( CDDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, ( DWORD )( LPVOID ) &mciSetParms ) )
		{
			OutputDebugString( "InitCDAudio: Set time format failed" );
			mciSendCommand( CDDeviceID, MCI_CLOSE, 0, 0 );
			return( FALSE );
		}

		if( !GetCDTOC() )
		{
			mciSendCommand( CDDeviceID, MCI_CLOSE, 0, 0 );
			return( FALSE );
		}
		CDInitialised = TRUE;
		
		/*
		 * Ok this bit is to init an interface for the volume control.
		 */
		
		void CALLBACK MicVolumeChange( DWORD dwCurrentVolume, DWORD dwUserValue );

		
		pCDVolume = (IVolume*)new CVolumeOutMaster();
		if ( !pCDVolume || !pCDVolume->IsAvailable() )
		{
 			CanISetCDVolumeReturn = FALSE;
		}
		else
		{
			CanISetCDVolumeReturn = TRUE;
		}
		pCDVolume->Enable();
		pCDVolume->RegisterNotificationSink(MicVolumeChange, aValue);
		windowsVolume = pCDVolume->GetCurrentVolume();
	  }
	
	return( TRUE );
}
	
//**********************************************************************************************
void CPCSoundManager::DeInitCDAudio()
{
	
	if( CDInitialised )
	{
		if( CDTimer != -1 )
		{
			CDTimer = -1;
		}

		if (pCDVolume)	// JCL
			pCDVolume->SetCurrentVolume(windowsVolume);
		mciSendCommand( CDDeviceID, MCI_CLOSE, 0, 0 );
		CDInitialised = FALSE;
	}
	
}
	
//**********************************************************************************************
void CPCSoundManager::PlayCDTrack( int track )
{

	if( CDInitialised )
	{
		if( ( track < 2 ) )
		{
			track = track + 2;
			
		}

		if( track > CDNumTracks )
		{
			track = 5;
		}

		mciPlayParms.dwFrom = MCI_MAKE_TMSF( track, 0, 0, 0 );
		mciPlayParms.dwTo   = MCI_MAKE_TMSF( track + 1, 0, 0, 0 );
		mciSendCommand(CDDeviceID, MCI_PLAY, MCI_FROM | MCI_TO, (DWORD)(LPVOID) &mciPlayParms);

		CDPlayFrom		= mciPlayParms.dwFrom;
		CDPlayTo		= mciPlayParms.dwTo;
		CDCurrentTrack	= track;
		IsCDPlaying		= TRUE;
	}

}
	
//**********************************************************************************************
void CPCSoundManager::StopCDTrack()
{
	if( CDInitialised )
	{
		mciSendCommand( CDDeviceID, MCI_STOP, 0, 0 );
		
		IsCDPlaying = FALSE;
	}
}
	
//**********************************************************************************************
void CPCSoundManager::PauseCDTrack()
{
	if( CDInitialised )
	{
		mciSendCommand( CDDeviceID, MCI_STOP, 0, 0 );
	
		IsCDPlaying = FALSE;
	}
}
	
//**********************************************************************************************
void CPCSoundManager::UnPauseCDTrack()
{
	MCI_PLAY_PARMS mciPlayParms;

	if( CDInitialised && !IsCDPlaying )
	{
		
		mciPlayParms.dwTo = CDPlayTo;
		if( !mciSendCommand( CDDeviceID, MCI_PLAY, MCI_TO, ( DWORD )&mciPlayParms ) )
		{
			IsCDPlaying = TRUE;
		}
		//MCIWndResume(g_hwndMCIWnd);
		IsCDPlaying = TRUE;
	}
}
	
//**********************************************************************************************
void CPCSoundManager::SetCDVolume( int vol )
{
	DWORD dwVolumeToSet,dwAnyUserValue;
	dwVolumeToSet = vol;
	
	dwAnyUserValue = 0;
	
	if(pCDVolume != NULL)
	{	
		pCDVolume->SetCurrentVolume(dwVolumeToSet);	
	}
}

//**********************************************************************************************
BOOL CPCSoundManager::GetCDTOC()
{
	MCI_STATUS_PARMS	mciStatusParms;

	mciStatusParms.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
	if( mciSendCommand( CDDeviceID, MCI_STATUS, MCI_STATUS_ITEM, ( DWORD )( LPVOID )&mciStatusParms ) )
	{
		OutputDebugString( "GetCDTOC: Query # CD tracks failed" );
		return( FALSE );
	}
	CDNumTracks = mciStatusParms.dwReturn;
	return( TRUE );
}
	
//**********************************************************************************************
void CPCSoundManager::CheckCDFunc()
{
	MCI_STATUS_PARMS	mciStatusParms;

	if( CDInitialised && IsCDPlaying )
	{
		//OutputDebugString( "Checking CD status...\n" );

		mciStatusParms.dwItem = MCI_STATUS_MODE;
		mciSendCommand( CDDeviceID, MCI_STATUS, MCI_STATUS_ITEM, ( DWORD )( LPVOID )&mciStatusParms );
		
		if( MCI_MODE_STOP == mciStatusParms.dwReturn )
		{
			IsCDPlaying = FALSE;
		}
		else
		{
			IsCDPlaying = TRUE;
		}
		//	IsCDPlaying = (MCIWndGetMode(g_hwndMCIWnd, buffer, sizeof(buffer)) == MCI_MODE_PLAY);
	}
}
//***********************************************************************************
void	CPCSoundManager::DeclareVolumeChange()
{
	SINT	vol;
	// music
	vol = (CAREER.mMusicVolume);
	vol = (vol * 6500)/2;
    SetCDVolume(vol);

	// sound
	UpdateStatus();

	// stereo
	
}
//***********************************************************************************
int		CPCSoundManager::PlayTrack(int track, int mode)
{
	 
		if(mode == 2)
		{
			StopTrack();
			theTrack = track; //set current track that is playing.
			PlayCDTrack(theTrack);
			return (0);
		}
		else
		{
			theTrack = track; //set current track that is playing.
			PlayCDTrack(theTrack);
			return (0);
		}

}

//***********************************************************************************
void	CPCSoundManager::StopTrack()
{
	//volume fade
	DWORD Volume, prevVolume;
	if (pCDVolume) // JCL
	{
		Volume = pCDVolume->GetCurrentVolume();
		prevVolume = Volume;
		if ( Volume != windowsVolume )
		{	
			for(Volume; Volume > 100; Volume = Volume-100)
			{
				pCDVolume->SetCurrentVolume(Volume);
			}
		}
	}
	StopCDTrack();
	if (pCDVolume)	// JCL
		pCDVolume->SetCurrentVolume(prevVolume);
}
//***********************************************************************************

//***********************************************************************************
void CALLBACK MicVolumeChange( DWORD dwCurrentVolume, DWORD dwUserValue )
{

}
//***********************************************************************************
BOOL CPCSoundManager::CanISetCDVolume()
{
	return CanISetCDVolumeReturn;
}


//***********************************************************************************
DWORD CPCSoundManager::FindCDDriveToUse(int numOfDrives)
{
    DWORD dwReturn;
  
	UINT tempCDDeviceID;
    char szTimeString[512] = "\0";  // room for 20 tracks
	char cdAudio[256];
	
	for(int x = 0; x < numOfDrives+1; x++)
	{
		if(x == 1)
		{
			mciOpenParms.lpstrDeviceType = "cdaudio";
		}
		else
		{
			sprintf(cdAudio, "cdaudio%d", x - 1);
			mciOpenParms.lpstrDeviceType = cdAudio;
		}

		if(mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE, ( DWORD )( LPVOID ) &mciOpenParms ) )
		{
			OutputDebugString( "InitCDAudio: Open CD device failed" );
			continue;
		}


		tempCDDeviceID = mciOpenParms.wDeviceID;

			// Set the time format to minute/second/frame (MSF) format. 
		mciSetParms.dwTimeFormat = MCI_FORMAT_MSF;
		if ((dwReturn = mciSendCommand(tempCDDeviceID, MCI_SET, 
			MCI_SET_TIME_FORMAT, 
			(DWORD)(LPVOID) &mciSetParms))) 
		{
			mciSendCommand(tempCDDeviceID, MCI_CLOSE, 0, NULL);
			continue;
		}
		
	// For track one and two ( two and three on the cd ) , get and save the starting location

		
		mciStatusParms.dwItem = MCI_STATUS_POSITION;
		mciStatusParms.dwTrack = 2;
		if ((dwReturn = mciSendCommand(tempCDDeviceID, 
			MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, 
			(DWORD)(LPVOID) &mciStatusParms))) 
		{
			mciSendCommand(tempCDDeviceID, MCI_CLOSE, 0, NULL);
			OutputDebugString("track 3 not found!!!!\n");
			continue;
		}

		DWORD trackOneStart = mciStatusParms.dwReturn;

		
		mciStatusParms.dwItem = MCI_STATUS_POSITION;
		mciStatusParms.dwTrack = 3;
		if ((dwReturn = mciSendCommand(tempCDDeviceID, 
			MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, 
			(DWORD)(LPVOID) &mciStatusParms))) 
		{
			mciSendCommand(tempCDDeviceID, MCI_CLOSE, 0, NULL);
			OutputDebugString("track 4 not found!!!!\n");
			continue;
		}

		DWORD trackTwoStart = mciStatusParms.dwReturn;

		DWORD timeBetweenTracks = trackTwoStart - trackOneStart;

		if((timeBetweenTracks < 1373198) && (timeBetweenTracks > 1373178))
		{
		
			return tempCDDeviceID;
		}
	}
	return 0;
}