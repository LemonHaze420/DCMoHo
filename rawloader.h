// RawLoader.h: interface for the CRawLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAWLOADER_H__19237722_470C_11D2_BC4A_00A0C9A33ADF__INCLUDED_)
#define AFX_RAWLOADER_H__19237722_470C_11D2_BC4A_00A0C9A33ADF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ImageLoader.h"

class CRawLoader : public CImageLoader  
{
public:
	CRawLoader(LPSTR pFilename, SINT alpha_type = 0);
	virtual ~CRawLoader();
	virtual BOOL Load();

	virtual	BOOL	NeedsAlpha();

protected:
	SINT	mAlphaType;
};

#endif // !defined(AFX_RAWLOADER_H__19237722_470C_11D2_BC4A_00A0C9A33ADF__INCLUDED_)
