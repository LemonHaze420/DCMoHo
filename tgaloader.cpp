#include "Common.h"

#if TARGET == PC

#include "TGALoader.h"


struct TGAHeader
{
	UBYTE	mIDLength;
	UBYTE	mColourMapType;
	UBYTE	mImageType;
	UBYTE	mColourMapSpec[5];
	UWORD	mOriginX;
	UWORD	mOriginY;
	UWORD	mSizeX;
	UWORD	mSizeY;
	UBYTE	mPixelDepth;
	UBYTE	mImageDescriptor;
};

//******************************************************************************************
CTGALoader::CTGALoader(LPSTR pFilename, SINT *alpha_type) : CImageLoader(pFilename), mAlphaType(alpha_type)
{
}

//******************************************************************************************
CTGALoader::~CTGALoader()
{
}

//******************************************************************************************
BOOL	CTGALoader::NeedsAlpha()
{
	return ((mAlphaType) && (*mAlphaType));  // hehe!!
}

//******************************************************************************************
int CTGALoader::Load()
{
	// Open the file
	FILE *datafile = fopen(GetFilename(), "rb");
	if (!datafile)
		return FALSE;

	// read header
	TGAHeader	header;
	fread(&header, sizeof(header), 1, datafile);

	// check it
	if ((header.mImageType != 2) ||
		((header.mPixelDepth != 24) && (header.mPixelDepth != 32)))
	{
		return FALSE;
	}
	
	m_nWidth = header.mSizeX;
	m_nHeight = header.mSizeY;
	
	// Load in the image
	if (!AllocBuffer(m_nWidth*m_nHeight*3))
		return FALSE;

	if (header.mPixelDepth == 24)
	{
		// just load it in!
		fread(m_pData, m_nWidth * m_nHeight * 3, 1, datafile);

/*		if (header.mOriginY != 0)
		{
			// oh dear.  got to flip...
			SINT	c0, c1;
			UBYTE	*pa = m_pData;
			UBYTE	*pb = m_pData + ((m_nHeight - 1) * m_nWidth) * 3;

			for (c0 = 0; c0 < m_nHeight / 2; c0++)
			{
				for (c1 = 0; c1 < m_nWidth * 3; c1++)
				{
					UBYTE t = *pa;
					*pa = *pb;
					*pb = t;
					pa ++;
					pb ++;
				}
				pb -= m_nWidth * 6;
			}
		}*/
	}
	else
	{
		// arse - legacy loader format....
		UBYTE	*buffer = new UBYTE[m_nWidth * m_nHeight * 4];
		fread(buffer, m_nWidth * m_nHeight * 4, 1, datafile);

		ClearAlphaBuffer();
		AllocAlphaBuffer(m_nWidth*m_nHeight);

		SINT	c0, c1;
		UBYTE	*pd;
		UBYTE	*pa; 

		if (header.mOriginY == 0)
		{
			pd = m_pData;
			pa = m_pAlphaData;
		}
		else
		{
			pd = m_pData + ((m_nHeight - 1) * m_nWidth) * 3;
			pa = m_pAlphaData + ((m_nHeight - 1) * m_nWidth);
		}
		UBYTE	*ps = buffer;

		for (c0 = 0; c0 < m_nHeight; c0++)
		{
			for (c1 = 0; c1 < m_nWidth ; c1++)
			{
				*(pd + 0) = *(ps + 0);
				*(pd + 1) = *(ps + 1);
				*(pd + 2) = *(ps + 2);
				*pa		  = *(ps + 3);

				pd += 3;
				pa ++;
				ps += 4;
			}
			if (header.mOriginY != 0)
			{
				pa -= m_nWidth * 2;
				pd -= m_nWidth * 6;
			}
		}
		*mAlphaType = 4;  // write back into the loading texture.

		delete [] buffer;
	}

	fclose(datafile);

	return TRUE;
}

//******************************************************************************************
BOOL	CTGALoader::Save16Bit(char *name, UWORD *data, SINT sx, SINT sy)
{
	TGAHeader	h;

	h.mIDLength = 0;
	h.mColourMapType = 0;
	h.mImageType = 2;
	h.mColourMapSpec[0] = 0;
	h.mColourMapSpec[1] = 0;
	h.mColourMapSpec[2] = 0;
	h.mColourMapSpec[3] = 0;
	h.mColourMapSpec[4] = 0;
	h.mOriginX = 0;
	h.mOriginY = 0;
	h.mSizeX = sx;
	h.mSizeY = sy;
	h.mPixelDepth = 24;
	h.mImageDescriptor = 0;

	FILE *fp = fopen(name, "wb");

	if (!fp)
		return FALSE;

	fwrite(&h, sizeof(h), 1, fp);

	SINT	c0, c1;

	for (c0 = 0; c0 < sy; c0 ++)
		for (c1 = 0; c1 < sx; c1 ++)
		{
			UWORD	*ptr = &data[c1 + ((sy - 1 - c0) * sx)];
			UBYTE r = (((*ptr) >> 10) & 31) << 3;
			UBYTE g = (((*ptr) >> 5 ) & 31) << 3;
			UBYTE b = (((*ptr)      ) & 31) << 3;

			fwrite(&r, sizeof(UBYTE), 1, fp);
			fwrite(&g, sizeof(UBYTE), 1, fp);
			fwrite(&b, sizeof(UBYTE), 1, fp);

			ptr ++;
		}

	fclose(fp);

	return TRUE;
}

//******************************************************************************************
BOOL	CTGALoader::Save16Bit4444(char *name, UWORD *data, SINT sx, SINT sy)
{
	TGAHeader	h;

	h.mIDLength = 0;
	h.mColourMapType = 0;
	h.mImageType = 2;
	h.mColourMapSpec[0] = 0;
	h.mColourMapSpec[1] = 0;
	h.mColourMapSpec[2] = 0;
	h.mColourMapSpec[3] = 0;
	h.mColourMapSpec[4] = 0;
	h.mOriginX = 0;
	h.mOriginY = 0;
	h.mSizeX = sx;
	h.mSizeY = sy;
	h.mPixelDepth = 32;
	h.mImageDescriptor = 0;

	FILE *fp = fopen(name, "wb");

	if (!fp)
		return FALSE;

	fwrite(&h, sizeof(h), 1, fp);

	SINT	c0, c1;

	for (c0 = 0; c0 < sy; c0 ++)
		for (c1 = 0; c1 < sx; c1 ++)
		{
			UWORD	*ptr = &data[c1 + ((sy - 1 - c0) * sx)];
			UBYTE a = (((*ptr) >> 12) & 15) << 4;
			UBYTE r = (((*ptr) >>  8) & 15) << 4;
			UBYTE g = (((*ptr) >>  4) & 15) << 4;
			UBYTE b = (((*ptr)      ) & 15) << 4;

			fwrite(&b, sizeof(UBYTE), 1, fp);
			fwrite(&g, sizeof(UBYTE), 1, fp);
			fwrite(&r, sizeof(UBYTE), 1, fp);
			fwrite(&a, sizeof(UBYTE), 1, fp);

			ptr ++;
		}

	fclose(fp);

	return TRUE;
}

#endif
