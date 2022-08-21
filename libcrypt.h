/*
**
** PlayStation Anti Piracy library Header
**
** Version 3.0 (20th November 1998)
**
** Author: Colin Hughes @ SCEE
**
** (c) Sony Computer Entertainment Europe 1999
**
*/

#ifndef _LIBCD_H_
#include <libcd.h>
#endif


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern void InitCrypt();
extern void ReadCrypt();
extern __declspec(data) volatile CdlCB CD_cbsync;			/* complete callback */
extern __declspec(data) volatile CdlCB CD_cbready;			/* data ready callback */


#define GOTKEY ( ( ((long)CD_cbsync)|((long)CD_cbready) )==0 )


#if 0
#define Decrypt( r0 ) __asm__ volatile (	\
	"mfc0 $12,$3;"							\
	"or $13,$0,%0;"							\
	"sll $12,$12,16;"						\
	"srl $14,$12,16;"						\
	"or $12,$12,$14;"						\
	"or $14,$0,512;"						\
	"lw $15,0($13);"						\
	"sub $14,$14,1;"						\
	"xor $15,$15,$12;"						\
	"sw $15,0($13);"						\
	"add $13,$13,4;"						\
	"bnez $14,.-20;"						\
	:										\
	: "r"( r0 )								\
	: "$12","$13","$14","$15" )
#else
extern Decrypt(unsigned char*buf);

#endif

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif
