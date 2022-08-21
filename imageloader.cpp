// ImageLoader.cpp: implementation of the CImageLoader class.
//
//////////////////////////////////////////////////////////////////////
#include "Common.h"

#if TARGET == PC

#include "ImageLoader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageLoader::CImageLoader(LPSTR pFilename) : m_pData(NULL), m_nWidth(0), m_nHeight(0), m_pAlphaData(NULL)
{
	strcpy(m_szFilename, pFilename);
}

CImageLoader::~CImageLoader()
{
	ClearBuffer();
	ClearAlphaBuffer();
}

void CImageLoader::ClearBuffer()
{
	if (m_pData)
	{
		delete []m_pData;
		m_pData = NULL;
	}
}

void CImageLoader::ClearAlphaBuffer()
{
	if (m_pAlphaData)
	{
		delete []m_pAlphaData;
		m_pAlphaData = NULL;
	}
}

BOOL CImageLoader::AllocBuffer(int nSize)
{
	ClearBuffer();
	m_pData = new BYTE[nSize];
	return (m_pData != NULL);
}

BOOL CImageLoader::AllocAlphaBuffer(int nSize)
{
	ClearAlphaBuffer();
	m_pAlphaData = new BYTE[nSize];
	return (m_pAlphaData != NULL);
}

#endif
