#ifndef PSXCONFIG_H
#define PSXCONFIG_H

class	CPSXConfig
{
public:
	void	Init()
	{
		mSoundVolume = G1;
		mMusicVolume = G1;
	};

protected:
	GINT	mSoundVolume;
	GINT	mMusicVolume;	
};

#endif
