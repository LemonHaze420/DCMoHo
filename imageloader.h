// $Header$

// $Log$
// Revision 1.2  2000-07-11 11:09:10+01  jjs
// Added Dreamcast defines.
//

// ImageLoader.h: interface for the CImageLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGELOADER_H__19237721_470C_11D2_BC4A_00A0C9A33ADF__INCLUDED_)
#define AFX_IMAGELOADER_H__19237721_470C_11D2_BC4A_00A0C9A33ADF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CImageLoader  
{
public:
	CImageLoader(LPSTR pFilename);
	virtual ~CImageLoader();
	virtual BOOL Load() = 0;
	inline virtual LPSTR GetFilename() {return m_szFilename;}
	inline virtual int Width() {return m_nWidth;}
	inline virtual int Height() {return m_nHeight;}
	inline virtual LPBYTE GetData() {return m_pData;}
	inline virtual LPBYTE GetAlphaData() {return m_pAlphaData;}

	virtual	BOOL	NeedsAlpha() {return FALSE;};

protected:
	virtual void ClearBuffer();
	virtual void ClearAlphaBuffer();
	virtual BOOL AllocBuffer(int nSize);
	virtual BOOL AllocAlphaBuffer(int nSize);
	int m_nWidth;
	int m_nHeight;
	LPBYTE m_pData;
	LPBYTE m_pAlphaData;
	char m_szFilename[256];
};

#endif // !defined(AFX_IMAGELOADER_H__19237721_470C_11D2_BC4A_00A0C9A33ADF__INCLUDED_)
