#ifndef CREDITS_H
#define CREDITS_H

enum	ECreditType
{
	CRED_NORMAL = 0,
	CRED_SECTION = 1,
};

class	CCredits
{
public:
	static	SINT	GetNumCreditLines();
	
	static	void	GetCredit(SINT n, ECreditType *type, SINT *title, char *l1, char *l2, char *l3, char *l4, char *l5);

};

#endif