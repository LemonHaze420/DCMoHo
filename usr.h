#ifndef usr_h
#define usr_h
/*
 *		Users Functions Header
 */
#include <sg_xpt.h>

void *UsrInitVideo(Sint32 *mode, Sint32 *frame, Sint32 *count,Sint32 *latency);


void UsrFinishVideo(void);


void UsrWaitVsync(void);


void UsrSetVsyncFunc(void (*func)(void));


long UsrPrintf(long lx, long ly, char *fmt,...);


void UsrInitSound(void);


void UsrFinishSound(void);

void UsrCheckDoorOpen(void);


void *UsrLoadFile(char *fname, long *len);

void UsrSfcnt2time(long sf, long ncnt, long *hh, long *mm, long *ss, long *ff);
void UsrBeginDraw(void);
void UsrEndDraw(void);
void UsrWaitVBlank(void);

#endif