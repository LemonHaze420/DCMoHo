#include <stdio.h>

#define N		 4096	/* size of ring buffer */
#define F_SS       18	/* upper limit for match_length */
#define F_ARI      60
#define THRESHOLD	2   /* encode string into position and length
						   if match_length is greater than this */
#define NIL			N	/* index for root of binary search trees */

unsigned char* from_buffer;
long from_pointer;
long from_max;

unsigned char* to_buffer;
long to_pointer;
long to_max;

unsigned long int
		textsize = 0,	/* text size counter */
		codesize = 0,	/* code size counter */
		printcount = 0;	/* counter for reporting progress every 1K bytes */

unsigned char  text_buf[N + F_ARI - 1];	/* ring buffer of size N,
			with extra F_ARI-1 bytes to facilitate string comparison */
int		match_position, match_length,  /* of longest match.  These are
			set by the InsertNode() procedure. */
		lson[N + 1], rson[N + 257], dad[N + 1];  /* left & right children &
			parents -- These constitute binary search trees. */

void lz_init(void)
{
  to_pointer = from_pointer = printcount = codesize = textsize = 0;
  return;
}

int getc_buffer(void)
{
  if (from_pointer == from_max)
	return(EOF);
  return(from_buffer[from_pointer++]);
}

int putc_buffer(int invoer)
{
  to_buffer[to_pointer++] = (unsigned char) invoer;
  return(1);
}
