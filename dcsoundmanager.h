#ifndef DCSOUNDMANAGER_H
#define DCSOUNDMANAGER_H
#define MaxChannels 64


/* mStatus is one of the following:
 * 	SPU_ON				Channel is active
 *  SPU_ON_ENV_OFF		Channel is active, but sound has ended.
 *  SPU_OFF_ENV_ON		Channel is being released (envelope playing out)
 *  SPU_OFF				Channel is free
 */

#include	"SampleList.h"

enum	SoundState
{
SPU_ON,
SPU_OFF
};

typedef struct
{
			CThing *mOwner;             // or NULL if the current channel has no owner....
			long	mChannel;           // Channel number.... Funnily enough, the same as the array index...
			SoundState	mStatus;            // See above....
			long	mSample;            // Sample number for this event...
			BOOL	mTrack;				// track thing?
			GINT	mMasterVolume;
			GINT	mSubVolume;
			GINT	mFade;			
			GINT	mFadeDest;
			GINT	mTime; 				// bugfix...
}SSoundEvent;

#define DEFAULT_SOUND_VOLUME (G(0, 45000))

class CDCSoundManager
{
	public:
		BOOL	Init();
		void	Reset();
		int		Shutdown();
		void	AllocSampleBank(char *name);
// Play a sample that's not connected to a THING....
		SINT	PlaySample(const short id, GINT volume = DEFAULT_SOUND_VOLUME, BOOL once = FALSE, GINT fade = G0);
// Play a sample 'owned' by THING
		SINT	PlaySample(const short id, CThing *thing, GINT volume = DEFAULT_SOUND_VOLUME, BOOL track = FALSE, BOOL once = FALSE, GINT fade = G0);
// Kill all samples owned by THING and trigger this instead...
		SINT	OverrideSample(const short id, CThing *thing);
// Kill all samples owned by THING (do this when a THING has looping samples, otherwise it'll last forever!)
		void	KillSamples(CThing *thing);
		void	KillSample(CThing *thing, const short id);  // kill a specific sample
// Call once a frame to validate any active THINGS and disable any channels that have completed playing...
		void 	UpdateStatus();
		
		void	FadeTo(const short id, GINT fadeval, GINT speed = G(0, 10000), CThing *t = NULL);
		
		void	FadeAllSamples();
		void	KillAllSamples();

		// how loud should a sample play?
		SINT	GetVolumeForThing(CThing *t);
		SINT	Fade(SINT v, SINT n);

		SINT	mSoundState;

		int PlayTrack(int track,int mode);
		void StopTrack(void);

		void	DeclareVolumeChange();
	protected:


		SSoundEvent *GetSoundEvent();
//		UBYTE	mVabHeaderBuff[16384];   // Space for header info
//		UBYTE	mVabHeaderBuff[32768];   // Space for header info
		UBYTE	mVabHeaderBuff[35000];   // Space for header info
		SSoundEvent mSoundEvent[MaxChannels];

		SWORD	mVab;                   // Bank identifier returned by LIBSND


};

#endif
