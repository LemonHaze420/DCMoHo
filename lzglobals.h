
#define N		 4096	/* size of ring buffer */
#define F_SS       18	/* upper limit for match_length */
#define F_ARI      60
#define THRESHOLD	2   /* encode string into position and length
						   if match_length is greater than this */
#define NIL			N	/* index for root of binary search trees */

extern unsigned char* from_buffer;
extern long from_pointer;
extern long from_max;

extern unsigned char* to_buffer;
extern long to_pointer;
extern long to_max;

extern unsigned long int textsize;	/* text size counter */
extern unsigned long int codesize;	/* code size counter */
extern unsigned long int printcount; /* counter for reporting progress every 1K bytes */

extern unsigned char  text_buf[N + F_ARI - 1];
extern int match_position;
extern int match_length;
extern int lson[N + 1];
extern int rson[N + 257];
extern int dad[N + 1];

extern void lz_init(void);
extern int getc_buffer(void);
extern int putc_buffer(int invoer);

