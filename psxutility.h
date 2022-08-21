#ifndef PSXUTILITY_H
#define PSXUTILITY_H

//inline	int sprintf(char *, const char *, ...) {return 0;}; //!PSX write sprintf function here!

inline	void *jcl_memset(void *p, int v, int n)
{
	//!PSX write sensible memset here
	int		c0;
	unsigned char	*c = (unsigned char *)p, u = (unsigned char)v;
	void *foo = p;

	for (c0 = 0; c0 < n; c0 ++)
	{
		*(c ++) = u;
	}

	return foo;
}

//SINT	PSXLoadFile(char *fname, ULONG *buffer);
SINT	PSXLoadPartFile(ULONG *buffer,SINT len);
//BOOL	PSXLoadTimToVRAM(char *fname, ULONG *tempmem);
SINT	PSXOpenFile(char *fname);
SINT	PSXCloseFile(void);


#endif
