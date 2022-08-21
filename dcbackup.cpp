/********************************************************************
 *  Shinobi Library Sample
 *  Copyright (c) 1998 SEGA
 *
 *  Library : Backup Library
 *  Module  : Library Sample
 *  File    : backup.c
 *  Date    : 1999-01-14
 *  Version : 1.00
 *
 ********************************************************************/

#include <shinobi.h>
#include "dcbackup.h"
#include <string.h>
/*===============================================================*/
/* �Ή����郁�����[�J�[�h�̍ő�e��                              */
/* Mamimum volume to use.                                        */
/*===============================================================*/

#define MAX_CAPS BUD_CAPACITY_1MB


/*===============================================================*/
/* �Ή����郁�����[�J�[�h�̃h���C�u��                            */
/* Number of memory card to use.                                 */
/*===============================================================*/

#define MAX_DRIVES 8
#define USE_DRIVES BUD_USE_DRIVE_ALL


/*===============================================================*/
/* �������[�J�[�h�̏�Ԃ��i�[���Ă����\����                      */
/* (backup.h ���Q��)                                             */
/* Structure to store the information of memory card.            */
/* (See backup.h)                                                */
/*===============================================================*/

BACKUPINFO gBupInfo[8];


/*===============================================================*/
/* �o�b�N�A�b�v���C�u�������g�p���郏�[�N                        */
/* �S�o�C�g���E�Ŋm�ۂ���                                        */
/* Work buffer that backup library uses.                         */
/* (Must be 4-bytes aligned.)                                    */
/*===============================================================*/

Uint32 gBupWork[BUM_WORK_SIZE(MAX_CAPS, MAX_DRIVES) / sizeof(Uint32)];


/*===============================================================*/
/*      �X�^�e�B�b�N�֐��̃v���g�^�C�v�錾                       */
/*      Prototypes of static functions.                          */
/*===============================================================*/

static Sint32 BupComplete(Sint32 drive, Sint32 op, Sint32 stat, Uint32 param);
static Sint32 BupProgress(Sint32 drive, Sint32 op, Sint32 count, Sint32 max);
static void BupInitCallback(void);


/*===============================================================*/
/*      �o�b�N�A�b�v���C�u����������                             */
/*      Initialize backup library.                               */
/*===============================================================*/

void BupInit(void)
{
	memset(gBupInfo, 0, sizeof(gBupInfo));
	buInit(MAX_CAPS, USE_DRIVES, gBupWork, BupInitCallback);
}


/*===============================================================*/
/*      �o�b�N�A�b�v���C�u�����I��                               */
/*      Finalize backup library.                                 */
/*===============================================================*/

void BupExit(void)
{
	do {} while (buExit() != BUD_ERR_OK);
}


/*===============================================================*/
/*      �������[�J�[�h�̏�Ԃ��擾                               */
/*      Get information of memory card.                          */
/*===============================================================*/

const BACKUPINFO* BupGetInfo(Sint32 drive)
{
	return (const BACKUPINFO*)&gBupInfo[drive];
}


/*===============================================================*/
/*      �t�@�C���̃��[�h                                         */
/*      Load a file.                                             */
/*===============================================================*/

Sint32 BupLoad(Sint32 drive, const char* fname, void* buf)
{
	return buLoadFile(drive, fname, buf, 0);
}


/*===============================================================*/
/*      �t�@�C���̃Z�[�u                                         */
/*      Save a file.                                             */
/*===============================================================*/

Sint32 BupSave(Sint32 drive, const char* fname, void* buf, Sint32 nblock)
{
	BUS_TIME time;
	SYS_RTC_DATE rtc;
	
	syRtcGetDate( &rtc );
	
	time.year 		= rtc.year;
	time.month 		= rtc.month;
	time.day 		= rtc.day;
	time.hour 		= rtc.hour;
	time.minute 	= rtc.minute;
	time.second 	= rtc.second;
	time.dayofweek 	= rtc.dayofweek;
	
	return buSaveFile(drive, fname, buf, nblock, &time,
							BUD_FLAG_VERIFY | BUD_FLAG_COPY(0));
}


/*===============================================================*/
/*      �t�@�C���̍폜                                           */
/*      Delete a file.                                           */
/*===============================================================*/

Sint32 BupDelete(Sint32 drive, const char* fname)
{
	return buDeleteFile(drive, fname);
}


/*===============================================================*/
/*      �G���[�R�[�h�𕶎���ɕϊ�                               */
/*      Convert error code into character string.                */
/*===============================================================*/

const char* BupGetErrorString(Sint32 err)
{
	switch (err) {
		case BUD_ERR_OK:				return "OK\0";
		case BUD_ERR_BUSY:				return "BUSY\0";
		case BUD_ERR_INVALID_PARAM:		return "INVALID PARAMETER\0";
		case BUD_ERR_ILLEGAL_DISK:		return "ILLEGAL DISK\0";
		case BUD_ERR_UNKNOWN_DISK:		return "UNKNOWN DISK\0";
		case BUD_ERR_NO_DISK:			return "NO DISK\0";
		case BUD_ERR_UNFORMAT:			return "UNFORMAT\0";
		case BUD_ERR_DISK_FULL:			return "DISK FULL\0";
		case BUD_ERR_FILE_NOT_FOUND:	return "FILE NOT FOUND\0";
		case BUD_ERR_FILE_EXIST:		return "FILE EXIST\0";
		case BUD_ERR_CANNOT_OPEN:		return "CANNOT OPEN\0";
		case BUD_ERR_CANNOT_CREATE:		return "CANNOT CREATE\0";
		case BUD_ERR_EXECFILE_EXIST:	return "EXECUTABLE FILE EXIST\0";
		case BUD_ERR_ACCESS_DENIED:		return "ACCESS DENIED\0";
		case BUD_ERR_VERIFY:			return "VERIFY ERROR\0";
		default:						return "GENERIC ERROR\0";
	}
}


/*===============================================================*/
/*      �I�y���[�V�����R�[�h�𕶎���ɕϊ�                       */
/*      Convert operation code into character string.            */
/*===============================================================*/

const char* BupGetOperationString(Sint32 op)
{
	switch (op) {
		case BUD_OP_MOUNT:			return "CONNECTED\0";
		case BUD_OP_UNMOUNT:		return "DISCONNECTED\0";
		case BUD_OP_FORMATDISK:		return "FORMATDISK\0";
		case BUD_OP_DELETEFILE:		return "DELETEFILE\0";
		case BUD_OP_LOADFILE:		return "LOADFILE\0";
		case BUD_OP_SAVEFILE:		return "SAVEFILE\0";
		default:					return "UNKNOWN OPERATION\0";
	}
}


/*===============================================================*/
/*      �R�[���o�b�N�֐�                                         */
/*      Callback functions.                                      */
/*===============================================================*/

/*
** �������I���R�[���o�b�N�֐�
** Callback function when initialization of library was finished.
*/

static void BupInitCallback(void)
{
	Sint32 i;

	/* �����A�o�߃R�[���o�b�N�֐���ݒ肷�� */
	/* Register callback functions.         */
	buSetCompleteCallback(BupComplete);
	buSetProgressCallback(BupProgress);
}


/*
** �����R�[���o�b�N�֐�
** Callback function when operation was finished.
*/

static Sint32 BupComplete(Sint32 drive, Sint32 op, Sint32 stat, Uint32 param)
{
	BACKUPINFO* info;
	Sint32 ret;

	info = &gBupInfo[drive];

	switch (op) {
		/* �������[�J�[�h���ڑ����ꂽ */
		/* Memory card was connected.  */
		case BUD_OP_MOUNT:
			if (stat == BUD_ERR_OK) {
				info->Ready = TRUE;
				ret = buGetDiskInfo(drive, &info->DiskInfo);
				if (ret == BUD_ERR_OK) {
					info->IsFormat = TRUE;
				}
				/* �G���[��Ԃ��N���A */
				/* Clear error status */
				info->LastError = BUD_ERR_OK;
			}
			break;
		/* �������[�J�[�h�����O���ꂽ */
		/* Memory card was removed.     */
		case BUD_OP_UNMOUNT:
			info->Ready = FALSE;
			info->IsFormat = FALSE;
			info->ProgressCount = 0;
			info->ProgressMax = 0;
			info->Operation = 0;
			memset(&info->DiskInfo, 0, sizeof(BUS_DISKINFO));
			break;
		default:
			/* �G���[��Ԃ��i�[ */
			/* Store error code. */
			info->LastError = stat;

			/* �����X�V���� */
			/* Update information of memory card. */
			buGetDiskInfo(drive, &info->DiskInfo);
	}

	/* �I�y���[�V�����R�[�h���N���A */
	/* Clear operation code.        */
	info->Operation = 0;

	return BUD_CBRET_OK;
}


/*
** �o�߃R�[���o�b�N�֐�
** Callback function through operation progress.
*/

static Sint32 BupProgress(Sint32 drive, Sint32 op, Sint32 count, Sint32 max)
{
	BACKUPINFO* info;

	info = &gBupInfo[drive];

	/* �o�ߏ�ԁA�I�y���[�V�����R�[�h���i�[ */
	/* Store operation code.                */
	info->ProgressCount = count;
	info->ProgressMax = max;
	info->Operation = op;

	return BUD_CBRET_OK;
}



/******************************* end of file *******************************/
