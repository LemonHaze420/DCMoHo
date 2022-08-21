#ifndef	SECURE
#define	SECURE

//****************************************************************************
// #INCLUDE THIS HEADER FILE IN THE MODULE THAT CONTAINS MAIN()
// MAIN() SHOULD BEGIN THUS:
//
//		void main()
//		{
//		#ifdef SECURE
//			Security();			 		// returns only if security check is OK
//		#endif
//		....
//		}
//
//****************************************************************************

// EDIT THIS LINE TO SHOW THE SECURITY NUMBER PROVIDED BY
// THE PSYGNOSIS SECURITY CARD UTILITY

#define	SECURITYID	0x21bca613

unsigned long gdwSid = SECURITYID;	// make ID available to Security()
extern void Security();
#endif



