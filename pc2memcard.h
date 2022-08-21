
#define BLOCK_MAX	(15)

extern struct DIRENTRY McFileList[BLOCK_MAX];

// Here follows my extra error codes (additions to LIBMCRD.H)

#define McBusy 			64
#define McFileCorrupt 	65
#define	McWrongVersion	66		// (JCL) and one of mine - for a different version of the career


// Call MCAcquireCard() to make sure there's a valid card inserted. If there isn't, it'll return an error
//	(defined in LIBMCRD.H)....
//	Do this *BEFORE* any of the other functions... (load/save/format)

// e.g:
#if 0

{
	while ((!exit_key_pressed)&&(MCAcquireCard()!=McErrNone))
	{
		// Do some stuph... Anim maybe?
	}
	MCSaveCareer("feck",career,1);
	.. or
	MCLoadCareer("feck",career,1);
	.. or
	MCFormat();
}

#endif






extern int MCGetState(int check);


extern int MCAcquireCard(void);


// Call this function to save a file. If the user is overwriting a game that exists, pass in TRUE for overwrite,
//	otherwise it'll return with an error....

class	CCareer;
extern int MCSaveCareer(char *name,CCareer*career,int overwrite);


// Same as SaveCareer except no 'overwrite' flag required. Returns an error code if summat goes wrong....
//		(including McFileCorrupt if the data is damaged.....)
extern int MCLoadCareer(char *name,CCareer*career);


extern int MCFormat(void);


// Returns the number of MOHO saves on the current cartridge.
//	Doesn't return errors.... will return 0 if there's no card/unformatted etc...
extern int MCNumFiles(void);


// HOUSEKEEPING FUNCTIONS. oNLY USED FOR DEBUGGING....
extern void MCEchoFiles(void);

