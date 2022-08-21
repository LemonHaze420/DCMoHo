#include	"Common.h"

#define LZ_N		 4096	/* size of ring buffer */
#define LZ_F_SS       18	/* upper limit for match_length */
#define LZ_F_ARI      60
#define LZ_THRESHOLD	2   /* encode string into position and length
						   if match_length is greater than this */
#define LZ_NIL			LZ_N	/* index for root of binary search trees */

#define	EOLZF		-1

unsigned char* from_buffer;
long from_pointer;
long from_max;

unsigned char* to_buffer;
long to_pointer;

unsigned char  lztext_buf[LZ_N + LZ_F_ARI - 1 + 5];	/* ring buffer of size N,
			with extra F_ARI-1 bytes to facilitate string comparison */

void lz_init(void)
{
  to_pointer = from_pointer = 0;
  return;
}

int getc_buffer(void)
{
  if (from_pointer == from_max)
	return(EOLZF);
  return(from_buffer[from_pointer++]);
}

int putc_buffer(int invoer)
{
  to_buffer[to_pointer++] = (unsigned char) invoer;
  return(1);
}


void Decode(void)	/* Just the reverse of Encode(). */
{
	int  i, j, k, r, c;
	unsigned int  flags;

	for (i = 0; i < LZ_N - LZ_F_SS; i++) lztext_buf[i] = ' ';

#if 1

	r = LZ_N - LZ_F_SS;  flags = 0;
	for ( ; ; )
	{
		if (((flags >>= 1) & 256) == 0) {
			if ((c = getc_buffer()) == EOLZF) break;
			flags = c | 0xff00;		/* uses higher byte cleverly */
		}							/* to count eight */
		if (flags & 1) {
			if ((c = getc_buffer()) == EOLZF) break;
			putc_buffer(c);  lztext_buf[r++] = c;  r &= (LZ_N - 1);
		} else {
			if ((i = getc_buffer()) == EOLZF) break;
			if ((j = getc_buffer()) == EOLZF) break;
			i |= ((j & 0xf0) << 4);  j = (j & 0x0f) + LZ_THRESHOLD;
			for (k = 0; k <= j; k++) {
				c = lztext_buf[(i + k) & (LZ_N - 1)];
				putc_buffer(c);  lztext_buf[r++] = c;  r &= (LZ_N - 1);
			}
		}
	}
#endif
}

void lzss_decompress(unsigned char* fbuffer, unsigned char* tbuffer, long comp_size)
{
  lz_init();
  from_buffer = fbuffer;
  from_max = comp_size;
  to_buffer = tbuffer;
  Decode();
  return;
}