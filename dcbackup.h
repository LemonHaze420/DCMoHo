/********************************************************************
 *  Shinobi Library Sample
 *  Copyright (c) 1998 SEGA
 *
 *  Library : Backup Library
 *  Module  : Library Sample Header
 *  File    : backup.h
 *  Date    : 1999-01-14
 *  Version : 1.00
 *
 ********************************************************************/

#ifndef _BACKUP_H_
#define _BACKUP_H_

#include <sg_bup.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******** �������J�[�h���\���� *******************************************/
/******** Memory card information structure ********************************/
typedef struct {
	Uint16 Ready;               /* �������J�[�h���ڑ�����Ă��邩          */
	                            /* Is memory card connected ?              */
	Uint16 IsFormat;            /* �t�H�[�}�b�g����Ă��邩�ǂ���          */
	                            /* Is memory card formatted ?              */
	Uint32 LastError;           /* �Ō�ɔ��������G���[�R�[�h              */
	                            /* Error code which occured last.          */
	Uint32 ProgressCount;       /* �����o�߃J�E���g                        */
	                            /* Operation progress count.               */
	Uint32 ProgressMax;         /* �����o�߃J�E���g�ő�                    */
	                            /* Operattion progress count maximum.      */
	Uint32 Operation;           /* �������̃I�y���[�V�����R�[�h            */
	                            /* Operation code in handling now.         */
	BUS_DISKINFO DiskInfo;      /* �h���C�u���                            */
	                            /* Drive information.                      */
} BACKUPINFO;


void BupInit(void);
void BupExit(void);
const BACKUPINFO* BupGetInfo(Sint32 drive);
const char* BupGetErrorString(Sint32 err);
const char* BupGetOperationString(Sint32 op);
Sint32 BupLoad(Sint32 drive, const char* fname, void* buf);
Sint32 BupSave(Sint32 drive, const char* fname, void* buf, Sint32 nblock);
Sint32 BupDelete(Sint32 drive, const char* fname);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BACKUP_H_ */

/******************************* end of file *******************************/
