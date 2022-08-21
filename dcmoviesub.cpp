/*
#include	<shinobi.h>
#include	<sg_mw.h>
#include	<string.h>
#include 	<am.h>
#include 	<sg_chain.h>
*/
#include	"usr.h"
#include <a64file.h>
#include "dcmoviesub.h"
#include "maincode.h"


//SYCHAIN				usrg_chid = (SYCHAIN)NULL;
#define	USR_INT_PRI			(0x90)
AP ap=&ap_obj;
void *vtxbuf;
Sint32 mode, frame, count, latency;
Bool ret;



void ap_mw_err_func(void *obj, char *errmsg)
{
	AP ap=(AP)obj;

	for (;;);
}

void ApInitMw(AP ap,
		Sint32 mod, Sint32 frm, Sint32 cnt, Sint32 ltncy, void *vtxbuf)
{
	MWS_PLY_INIT_SFD	iprm;

	memset(&iprm, 0, sizeof(iprm));	
	iprm.mode		= mod;
	iprm.frame		= frm;
	iprm.count		= cnt;
	iprm.latency	= ltncy;
	mwPlyInitSfdFx(&iprm);
	mwPlySetVertexBuffer(vtxbuf);
	mwPlyEntryErrFunc(ap_mw_err_func, ap);
}

void ApFinishMw(AP ap)
{
	mwPlyFinishSfdFx();
}

void ApStart(AP ap,int x,int y,int xsize,int ysize)
{
	MWS_PLY_CPRM_SFD		*cprm=&ap->cprm;

	memset(cprm, 0, sizeof(cprm));
	cprm->compo_mode = MWD_PLY_COMPO_OPEQ;
	cprm->ftype	= MWD_PLY_FTYPE_SFD;
	cprm->dtype	= MWD_PLY_DTYPE_WND;
	cprm->max_bps = 450*1024*8;
	cprm->max_width = 320;
	cprm->max_height = 240;
	cprm->nfrm_pool_wk = 8;

	cprm->wksize = mwPlyCalcWorkCprmSfd(cprm);

	cprm->work =(signed char*) syMalloc(cprm->wksize);
	ap->ply = mwPlyCreateSofdec(cprm);
	if (ap->ply == NULL) {
		for (;;) ;
	}
	//mwPlySetSrfPntBuf(ap->ply,25*4,cprm->work,sizeof(cprm->work));;
	//mwPlySetSrfPos(ap->ply,25*4,50,50,0);

	mwPlyStartFname(ap->ply, (signed char *)ap->fname);
	mwPlySetDispPos(ap->ply,x,y);		
	mwPlySetDispSize(ap->ply,xsize,ysize);	
	
}


void ApStop(AP ap)
{
	mwPlyStop(ap->ply);
	mwPlyDestroy(ap->ply);
	ap->ply = NULL;
	syFree(ap->cprm.work);
	ap->cprm.work = NULL;
}


void ap_disp_info(AP ap)
{
	Sint32	time, tunit, hh, mm, ss, ff;

	mwPlyGetTime(ap->ply, &time, &tunit);
	UsrSfcnt2time(tunit, time, &hh, &mm, &ss, &ff);
	UsrPrintf(0, 0, "File: %s  Time:%02d:%02d:%02d.%02d", 
							ap->fname, hh, mm, ss, ff);

}


Sint32 ApExec(AP ap, bool Keypress)
{
	MWPLY					ply=ap->ply;
	MWE_PLY_STAT			stat;
	const PDS_PERIPHERAL	*per;

	per = pdGetPeripheral(PDD_PORT_A0);

	*/	
	if (per->press & PDD_DGT_TB && Keypress)
		return FALSE;
	
	stat = mwPlyGetStat(ply);
	if ((stat == MWE_PLY_STAT_PLAYEND) || (stat == MWE_PLY_STAT_ERROR))
		return FALSE;

	return TRUE;
}


void ApGdErrFunc(void *obj, Sint32 errcode)
{
	AP ap=(AP)obj;
	if (errcode == GDD_ERR_TRAYOPEND || errcode == GDD_ERR_UNITATTENT)
		ap->term_flag = ON;	
}

void ApVsyncFunc(void)
{
        UsrCheckDoorOpen();
}


void PlayMovie(char *fname,int x,int y,int xsize,int ysize,BOOL Keypress)
{

	
	ap->fname=fname;
		ApStart(ap,x,y,xsize,ysize);
	
		while (1) 
		{
			mwPlyExecTexSvr();
			UsrBeginDraw();
			mwPlyExecDrawSvr();
			ret = ApExec(ap,Keypress);
			UsrEndDraw();
			if ((ret == FALSE) || ( ap->term_flag == ON))
				break;
			UsrWaitVBlank();
		}
	ApStop(ap);
	
	


}

#define	MW_MAX_CH		(8)
#define	MW_SNDRAM_SIZE	(0x4040*MW_MAX_CH)
/*
void UsrInitSound(void)
{
	KTBOOL ret;
	AM_SOUND	amsnd[8];
	KTU32		port[8], i;
	volatile KTU32	*sndram=KTNULL;

#ifdef USE_MIDIDA
	if ( bpAmSetup(AC_DRIVER_MIDI, KTFALSE, KTNULL)==KTFALSE )
#else
	if ( bpAmSetup(AC_DRIVER_DA, KTFALSE, KTNULL)==KTFALSE )
#endif
		for (;;);

	for (i=0; i<MW_MAX_CH; i++) {
		ret = amSoundAllocateVoiceChannel(&amsnd[i]);
		if (ret == KTFALSE) {
			for (;;);
		}
		port[i] = amsnd[i].voiceChannel;
	}
	ret = amHeapAlloc(&sndram, MW_SNDRAM_SIZE, 32, AM_FIXED_MEMORY, NULL);
	if (ret == KTFALSE) {
		for (;;);
	}
	mwSetupAudio64(sndram, MW_SNDRAM_SIZE, port, MW_MAX_CH);
	acSetTransferMode(AC_TRANSFER_DMA);
}*/
/*
void UsrSetVsyncFunc(void (*func)(void))
{
	if (usrg_chid != (SYCHAIN)NULL) {
		syChainDeleteHandler(usrg_chid);
	}
	usrg_chid = syChainAddHandler(
		(SY_CHAIN_EVENT) SYD_CHAIN_EVENT_IML6_VBLANK,
		(void *)func, (Uint32)USR_INT_PRI, (void *)NULL);
}
*/


void SubInitMovie(void)
{
    mwPlyPreInitSofdec();
	vtxbuf = UsrInitVideo(&mode, &frame, &count, &latency);
	//gdFsEntryErrFuncAll((void *)ApGdErrFunc, ap);
	UsrInitSound();
	//UsrSetVsyncFunc((void (*)(void))ApVsyncFunc);
	ApInitMw(ap, mode, frame, count, latency, vtxbuf);
}
/*
void main(void)
{

#ifdef __GNUC__
	shinobi_workaround();
#endif

	mwPlyPreInitSofdec();
	vtxbuf = UsrInitVideo(&mode, &frame, &count, &latency);
	gdFsEntryErrFuncAll((void *)ApGdErrFunc, ap);
	UsrInitSound();
	UsrSetVsyncFunc((void (*)(void))ApVsyncFunc);
	ApInitMw(ap, mode, frame, count, latency, vtxbuf);
	
	PlayMovie("logo9.sfd",0,0,640,480,FALSE);
	PlayMovie("T2intro.sfd",0,80,640,320,FALSE);
	PlayMovie("intusa1.sfd",0,80,640,320,FALSE);

	ApFinishMw(ap);
	//UsrFinishSound();
	//UsrFinishVideo();
	sbExitSystem();	
	syBtExit();
	
}
*/

