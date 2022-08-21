#ifndef LZSS_H
#define LZSS_H

long lzss_compress(unsigned char* tbuffer, unsigned char* fbuffer, long real_size);
void lzss_decompress(unsigned char* fbuffer, unsigned char* tbuffer, long comp_size);

#endif
