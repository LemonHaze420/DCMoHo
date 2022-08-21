// RawLoader.cpp: implementation of the CRawLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "Common.h"

#if TARGET == PC

#include "RawLoader.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRawLoader::CRawLoader(LPSTR pFilename, SINT alpha_type) : CImageLoader(pFilename), mAlphaType(alpha_type)
{
}

CRawLoader::~CRawLoader()
{
}

//******************************************************************************************
BOOL	CRawLoader::NeedsAlpha()
{
	return mAlphaType;
}

//******************************************************************************************
int CRawLoader::Load()
{
	fpos_t size;

	// Open the file
	FILE * datafile = fopen(GetFilename(), "rb");
	if (!datafile)
		return FALSE;

	// Let's see how large it is
	fseek(datafile, 0, SEEK_END);
	fgetpos(datafile, &size);
	m_nWidth = m_nHeight = (int)sqrt((double)size/3);

	// Load in the image
	if (!AllocBuffer(m_nWidth*m_nHeight*3))
		return FALSE;
	fseek(datafile, 0, SEEK_SET);
	fread(GetData(), m_nWidth*m_nHeight*3, 1, datafile);
	fclose(datafile);

	switch(mAlphaType)
	{
	case 1:
		{
			// auto alpha on black!
			ClearAlphaBuffer();
			AllocAlphaBuffer(m_nWidth*m_nHeight);
			
			BYTE	*pd = m_pData;
			BYTE	*pa = m_pAlphaData;

			SINT c0;
			for (c0 = 0; c0 < m_nWidth*m_nHeight; c0++)
			{
				if ((*pd == 0) && (*(pd + 1) == 0 ) && (*(pd + 2) == 0))
					*pa = 0;
				else
					*pa = 255;

				pa++;
				pd+=3;
			}		
		}
		break;
	case 2:
		{
			// gradient alpha on red!
			ClearAlphaBuffer();
			AllocAlphaBuffer(m_nWidth*m_nHeight);
			
			BYTE	*pd = m_pData;
			BYTE	*pa = m_pAlphaData;

			SINT c0;
			for (c0 = 0; c0 < m_nWidth*m_nHeight; c0++)
			{
				if ((*pd == 0) && (*(pd + 1) == 0 ) && (*(pd + 2) < 85))
				{
					if (*(pd + 2) < 55)
						*pa = 0;
					else
						*pa = ((*(pd + 2)) - 55) << 3; // alpha based on red component
					*(pd + 2) = 0;
				}
				else
					*pa = 255;

				pa++;
				pd+=3;
			}		
		}
		break;
	case 3:
		{
			// gradient alpha on rgb
			ClearAlphaBuffer();
			AllocAlphaBuffer(m_nWidth*m_nHeight);
			
			BYTE	*pd = m_pData;
			BYTE	*pa = m_pAlphaData;

			SINT c0;
			for (c0 = 0; c0 < m_nWidth*m_nHeight; c0++)
			{
				if ((*pd < 64) && (*(pd + 1) < 64 ) && (*(pd + 2) < 64))
				{
					SINT t = *pd + *(pd + 1) + *(pd + 2);

					*pa = BYTE((t * 4) / 3);
				}
				else
					*pa = 255;

				pa++;
				pd+=3;
			}		
		}
		break;
	}

	return TRUE;
}

#endif
