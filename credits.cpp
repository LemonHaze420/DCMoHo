#include	"Common.h"

#include	"Credits.h"
#include	"TextList.h"

//***********************************************************
SINT	CCredits::GetNumCreditLines()
{
	return 1;
}

//***********************************************************
#define COPY1(s) sprintf(l1, "%s", s)
#define COPY2(s) sprintf(l2, "%s", s)
#define COPY3(s) sprintf(l3, "%s", s)
#define COPY4(s) sprintf(l4, "%s", s)
#define COPY5(s) sprintf(l5, "%s", s)

void	CCredits::GetCredit(SINT n, ECreditType *type, SINT *title, char *l1, char *l2, char *l3, char *l4, char *l5)
{
#if TARGET == PSX

	l1[0] = 0;
	l2[0] = 0;
	l3[0] = 0;
	l4[0] = 0;
	l5[0] = 0;
	*type = CRED_NORMAL;
	*title = TEXT_CRED1 + n;

	switch (n)
	{
	case 0:	 *type = CRED_SECTION;		break;
	case 1:  COPY1("Jeremy Longley");	break;
	case 2:  COPY1("Darran Thomas");	break;
	case 3:  COPY1("Alex Trowers");		break;
	case 4:  COPY1("Glenn Corpes");
			 COPY2("Damian Stones");
	         COPY3("Stuart Gillam");	
			 break;
	case 5:  COPY1("Mark Pitcher");
			 COPY2("Jolyon Kemp");
			 break;
	case 6:  COPY1("Chris Hill");
			 COPY2("James Boty");
	         COPY3("Axis Animation");	
			 break;
	case 7:  COPY1("James Hannigan");	break;
	case 8:  COPY1("Richard Joseph");
			 COPY2("Samples courtesy of:");
			 COPY3("Spectrasonic's Distorted Reality 2");
			 break;
	case 9:  COPY1("Les Edgar");
			 COPY2("Audrey Adams");
			 COPY3("John Cook");
			 COPY4("Alison Beasley");
			 break;
	case 10: COPY1("Mark Adami, Ben Board, Mark Feldman, Simon Handby,");
			 COPY2("Fin McGechie, A-D Moore, Morten Pedersen,");
			 COPY3("Richard Reed, Bjarne Rene, Andy Trowers, Mike Man,");
			 COPY4("Larabie Fonts, Shaen Dobson, Mark Lambert,");
			 COPY5("Tim Howarth, Mike Pawley and Imran at KFC");
			 break;
	case 11: break;
	
	case 12: *type = CRED_SECTION;				break;
	case 13: COPY1("Luke Vernon");				break;
	case 14: COPY1("Lee Brown");				break;
	case 15: COPY1("Rachel England-Brassy");	break;
	case 16: COPY1("Sarah Seaby");				break;
	
	case 17: *type = CRED_SECTION;			break;
	case 18: COPY1("Sam Houser");			break;
	case 19: COPY1("Gary J. Foreman");		break;
	case 20: COPY1("Jamie King");			break;
	case 21: COPY1("Neil J. McCaffrey");	break;
	case 22: COPY1("Terry Donovan");
			 COPY2("Jennifer Kolbe");
			 COPY3("Jeff Castaneda");
			 COPY4("Jung Kwak");
			 COPY5("Brandon Rose");
			 break;
	case 23: COPY1("Kevin Gill");		break;
	case 24: COPY1("Jeff Rosa");		break;
	case 25: COPY1("Jeremy Pope");		break;
	case 26: COPY1("Mark Garone");
			 COPY2("Bobby McCarthy");
			 COPY3("Joe Greene");
			 COPY4("Richard Huie");
			 break;
	case 27: COPY1("Rich Rosado");		break;
	
	case 28: *type = CRED_SECTION;		break;
	case 29: COPY1("Mark Lloyd");		break;
	case 30: COPY1("Jim Thompson");
			 COPY2("Denby Grace");
			 COPY3("Matt Hewitt");
			 COPY4("Kevin Hobson");
			 break;
	case 31: COPY1("Kit Brown, Andy Mason, Charlie Kinloch,");
			 COPY2("James Cree, Julian Turner, Lee Johnson,");
			 COPY3("Paul Byers, Rob Dunkin, Tim Bates, Wil Kirton");
			 COPY4("");
			 break;
	};
#elif TARGET == DC

	l1[0] = 0;
	l2[0] = 0;
	l3[0] = 0;
	l4[0] = 0;
	l5[0] = 0;
	*type = CRED_NORMAL;
	*title = TEXT_CRED1 + n;

	switch (n)
	{
	case 0:	 *type = CRED_SECTION;		break;
	case 1:  COPY1("Jeremy Longley");	break;
	case 2:  COPY1("Darran Thomas");	break;
	case 3:  COPY1("Alex Trowers");		break;
	case 4:  COPY1("Glenn Corpes");
			 COPY2("Damian Stones");
	         COPY3("Stuart Gillam");	
			 break;
	case 5:  COPY1("Mark Pitcher");
			 COPY2("Jolyon Kemp");
			 break;
	case 6:  COPY1("Chris Hill");
			 COPY2("James Boty");
	         COPY3("Axis Animation");	
			 break;
	case 7:  COPY1("James Hannigan");	break;
	case 8:  COPY1("Richard Joseph");
			 COPY2("Samples courtesy of:");
			 COPY3("Spectrasonic's Distorted Reality 2");
			 break;
	case 9:  COPY1("Les Edgar");
			 COPY2("Audrey Adams");
			 COPY3("John Cook");
			 COPY4("Alison Beasley");
			 break;
	case 10: COPY1("Mark Adami, Ben Board, Mark Feldman, Simon Handby,");
			 COPY2("Fin McGechie, A-D Moore, Morten Pedersen,");
			 COPY3("Richard Reed, Bjarne Rene, Andy Trowers, Mike Man,");
			 COPY4("Larabie Fonts, Shaen Dobson, Mark Lambert,");
			 COPY5("Tim Howarth, Mike Pawley and Imran at KFC");
			 break;
	case 11: break;
	
	case 12: *type = CRED_SECTION;				break;
	case 13: COPY1("Sean Smith");				break;
	case 14: COPY1("John Jones-Steele, David Rowe"); break;

	
	case 15: *type = CRED_SECTION;				break;
	case 16: COPY1("Luke Vernon");				break;
	case 17: COPY1("Lee Brown");				break;
	case 18: COPY1("Amy Curtin");				break;
	case 19: COPY1("Rachel England-Brassy");	break;
	case 20: COPY1("Sarah Seaby");				break;
	case 21: COPY1("Jon Broadbridge");			break;
	case 22: COPY1("Chris Madgwick");			break;
	
	case 23: *type = CRED_SECTION;		break;
	case 24: COPY1("Mark Lloyd");		break;
	case 25: COPY1("Paul Byers");
			 COPY2("Tim Bates");
			 COPY3("Kevin Hobson");
			 break;
	case 26: COPY1("Charlie Kinloch, Andy Mason, Denby Grace");
			 COPY2("Julian Turner, Chris Brown, Lee Johnson");
			 COPY3("Matt Hewitt, James Cree, William Kirton");
			 COPY4("Robert Dunkin, Phil Alexander");
			 break;
	};


#else

	l1[0] = 0;
	l2[0] = 0;
	l3[0] = 0;
	l4[0] = 0;
	l5[0] = 0;
	*type = CRED_NORMAL;
	*title = TEXT_CRED1 + n;

	switch (n)
	{
	case 0:	 *type = CRED_SECTION;		break;
	case 1:  COPY1("Jeremy Longley");	break;
	case 2:  COPY1("Darran Thomas");	break;
	case 3:  COPY1("Alex Trowers");		break;
	case 4:  COPY1("Glenn Corpes");
			 COPY2("Damian Stones");
	         COPY3("Stuart Gillam");	
			 break;
	case 5:  COPY1("Mark Pitcher");
			 COPY2("Jolyon Kemp");
			 break;
	case 6:  COPY1("Chris Hill");
			 COPY2("James Boty");
	         COPY3("Axis Animation");	
			 break;
	case 7:  COPY1("James Hannigan");	break;
	case 8:  COPY1("Richard Joseph");
			 COPY2("Samples courtesy of:");
			 COPY3("Spectrasonic's Distorted Reality 2");
			 break;
	case 9:  COPY1("Les Edgar");
			 COPY2("Audrey Adams");
			 COPY3("John Cook");
			 COPY4("Alison Beasley");
			 break;
	case 10: COPY1("Mark Adami, Ben Board, Mark Feldman, Simon Handby,");
			 COPY2("Fin McGechie, A-D Moore, Morten Pedersen,");
			 COPY3("Richard Reed, Bjarne Rene, Andy Trowers, Mike Man,");
			 COPY4("Larabie Fonts, Shaen Dobson, Mark Lambert,");
			 COPY5("Tim Howarth, Mike Pawley and Imran at KFC");
			 break;
	case 11: break;
	
	case 12: *type = CRED_SECTION;				break;
	case 13: COPY1("John Jones-Steele, Andrew Evans");		break;
	case 14: COPY1("Anthony Dunn");				break;
	case 15: COPY1("John Jones-Steele, David Rowe"); break;

	
	case 16: *type = CRED_SECTION;				break;
	case 17: COPY1("Luke Vernon");				break;
	case 18: COPY1("Lee Brown");				break;
	case 19: COPY1("Amy Curtin");				break;
	case 20: COPY1("Rachel England-Brassy");	break;
	case 21: COPY1("Sarah Seaby");				break;
	case 22: COPY1("Jon Broadbridge");			break;
	case 23: COPY1("Chris Madgwick");			break;
	case 24: COPY1("Sajjad Majid");				break;
	
	case 25: *type = CRED_SECTION;		break;
	case 26: COPY1("Mark Lloyd");		break;
	case 27: COPY1("Paul Byers");
			 COPY2("Tim Bates");
			 COPY3("Kevin Hobson");
			 break;
	case 28: COPY1("Charlie Kinloch, Andy Mason, Denby Grace");
			 COPY2("Julian Turner, Chris Brown, Lee Johnson");
			 COPY3("Matt Hewitt, James Cree, William Kirton");
			 COPY4("Robert Dunkin, Phil Alexander");

			 break;
	};

#endif
}
