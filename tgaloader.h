#ifndef TGALOADER_H
#define TGALOADER_H

#include "ImageLoader.h"

class CTGALoader : public CImageLoader  
{
public:
	CTGALoader(LPSTR pFilename, SINT *alpha_type);
	virtual ~CTGALoader();
	virtual BOOL Load();

	virtual	BOOL	NeedsAlpha();

	static	BOOL	Save16Bit(char *name, UWORD *data, SINT sx, SINT sy); // why not!
	static	BOOL	Save16Bit4444(char *name, UWORD *data, SINT sx, SINT sy); // why not!
protected:
	SINT	*mAlphaType;
};

#endif 
