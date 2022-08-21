#ifndef _OPTIMIZEOT_H_
#define _OPTIMIZEOT_H_

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif /* CPP */

	//extern void CompactOTag(unsigned long int *pOT, int iNumEntries);
	extern void CompactOTagR(unsigned long int *pOT, int iNumEntries);
	//extern void CompactDiv8OTag(unsigned long int *pOT, int iNumEntries);
	extern void CompactDiv8OTagR(unsigned long int *pOT, int iNumEntries);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif /* CPP */
#endif /* _OPTIMIZEOT_H_ */
