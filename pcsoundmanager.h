#include "common.h"


#ifndef PCSOUNDMANAGER_H
#define PCSOUNDMANAGER_H

#define STRICT
#include <windows.h>
#include "Thing.h"
#include <commdlg.h>
#include "mmsystem.h"
#include "WavRead.h"
#include <Dsound.h>//Direct Sound interface file
#include "resource.h" 
#include "SampleList.h"
#include "math.h"
#include "Career.h"
#include "Camera.h"


typedef struct
{
			CThing *mOwner;             // or NULL if the current channel has no owner....
			long	mChannel;           // Channel number.... Funnily enough, the same as the array index...
			long	mStatus;            // See above....
			long	mSample;            // Sample number for this event...
			BOOL	mTrack;				// Is the sample to track an object across screen?
			GINT	mMasterVolume;		// The Master Volume
			GINT	mSubVolume;			// The Sub Volume
			GINT	mFade;				// The Fade of the sound
			GINT	mFadeDest;			// .
			GINT	mTime; 				// bugfix...
}SSoundEvent;


#define NUMEVENTS 2
#define SPU_ON 1
#define SPU_OFF 0
#define SPU_OFF_ENV_OFF 0
#define SPU_OFF_ENV_ON 0
#define SPU_ON_ENV_OFF 0

#define DEFAULT_SOUND_VOLUME (G(0, 40000))
#define WANT_CDMUSIC		1		//If 1, use CD music
#define	MUSIC_RATIO	25

class CPCSoundManager
{
public:
	
	/*******************/
	/* LOCAL VARIABLES */
	/*******************/
	static MCI_OPEN_PARMS		mciOpenParms;
	static MCI_SET_PARMS		mciSetParms;
	static MCI_PLAY_PARMS		mciPlayParms;
	static MCI_SYSINFO_PARMS	mciSysInfoParms;
	static MCI_STATUS_PARMS		mciStatusParms;
	static BOOL					CDInitialised;	
	static BOOL 				CDRepeat;		
	static BOOL 				CDDeviceOpen;	
	static UINT					CDDeviceID;		
	static UINT					CDPlayFrom;		
	static UINT					CDPlayTo;		
	static int					CDauxdevice;	
	static int					CDNumTracks;		

	static int					CDCurrentTrack;	
	static int					CDTimer;
	static int					num_devs;
	static int					SizeOfBuffers;
	
	CPCSoundManager()
	{
		g_pDS								= NULL;
		g_pDSBuffer							= NULL;
		
		for (int i = 0;i < SizeOfBuffers; i++)
		{
			DSBuffer[i]						= NULL;
			memset((void*)&mSoundEvent[i], 0, sizeof(mSoundEvent[i]));
		}

		for (i = 0; i < 64; i ++)
		{
			WavPointers[i].pbWavData		= NULL;
			WavPointers[i].cbWavSize		= NULL;
		}
	}

	BOOL Init(HWND handleToWindow); //Start up directx stuff
	void Shutdown(); //close down directx stuff
	void AllocSampleBank(char *name); //Setup the sound sample bank
	void Reset(); //so something entirely different?

	//function called to play a sample
	SINT PlaySample(const short id, GINT volume = DEFAULT_SOUND_VOLUME, BOOL once = FALSE, GINT fade = G0); 
	//called to play a diffdernt type of sample
	SINT PlaySample(const short id, CThing *thing, GINT volume = DEFAULT_SOUND_VOLUME, BOOL track = FALSE, BOOL once = FALSE, GINT fade = G0);
	//two things happening at once?
	SINT OverrideSample(const short id, CThing *thing);
	//stop all samples playing
	void KillSamples(CThing *thing);
	//stop a sample playing
	void KillSample(CThing *thing, const short id);
	
	//music fade I assume
	void FadeTo(const short id, GINT fadeval, GINT speed = G(0, 10000), CThing *t = NULL);
	
	//fade and or kill every sample, probably for in game endgame or menu to game fade
	void FadeAllSamples();
	
	//Stop all samples playing
	void KillAllSamples();
	
	//Returns a pointer to a free sound buffer
	SSoundEvent* GetSoundEvent();
	
	//Obtains a value for the volume a sound should be at.
	SINT GetVolumeForThing(CThing *t);

	LPDIRECTSOUND GetDirectSound() {return g_pDS;}

	int inline FindFreeArrayLocation(LPDIRECTSOUNDBUFFER*);

	void inline UpdateStatus();

	float CalculatePan(CCamera *theCam, CThing *theThing); 
	
	/* *
	 * 
	 * The following fuctions are defined for cd playing.
	 *
	 * */
	
	BOOL GetCDTOC();
	
	void CheckCDFunc();
	
	BOOL InitCDAudio();
	
	void DeInitCDAudio();
	
	static void PlayCDTrack( int track );
	
	void StopCDTrack();
	
	void PauseCDTrack();
	
	void UnPauseCDTrack();
	
	void SetCDVolume( int vol );
	
	void DeclareVolumeChange();

	int PlayTrack(int track, int mode);

	void StopTrack();

	BOOL CanISetCDVolume();

	DWORD FindCDDriveToUse(int numOfDrives);

protected:


	SINT Fade(SINT v, SINT n);

	struct filePointer{
	BYTE*   pbWavData; // Pointer to actual wav data 
    UINT    cbWavSize; // Size of data
	};

	filePointer			WavPointers[64];
	LPDIRECTSOUND       g_pDS;
	LPDIRECTSOUNDBUFFER g_pDSBuffer;
	LPDIRECTSOUNDBUFFER	DSBuffer[32];
	SSoundEvent			mSoundEvent[32];

	CWaveSoundRead		g_pWaveSoundRead;
	static BOOL			IsCDPlaying;
	
};
#endif


