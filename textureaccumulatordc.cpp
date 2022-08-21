#include	"Common.h"

#ifdef LINK_EDITOR

#include	"TextureAccumulatorDC.h"
#include	"Editors.h"
#include	"TGALoader.h"
#include	"Globals.h"

#include	"lzss.h"

inline	UWORD	BGRToRGB(UWORD i)
{
	SINT	r = (i >> 10) & 31;
	SINT	g = (i >>  5) & 31;
	SINT	b = (i >>  0) & 31;

	return (b << 10) + (g << 5) + r;
}

//******************************************************************************************
inline	UWORD	RGBAToWord(SINT r, SINT g, SINT b, SINT a)
{
	return ((a >> 4) << 12) + ((r >> 4) << 8) + ((g >> 4) << 4) + (b >> 4);   //4444
}

//******************************************************************************************
BOOL	CTextureAccumulatorDC::LoadTGA(char *fname, UWORD *data, BOOL &isalpha, SINT &sx, SINT &sy, ETADCSource source)
{
	char	name[256];
	
	// where are we looking?
	if (source == TADCS_DC)
		strcpy(name, "DCData\\Textures\\");
	else
		strcpy(name, "Data\\Textures\\");

	ASSERT(fname[0]);
	strcat(name, fname);
	
	SINT alpha = 0;
	CTGALoader loader(name, &alpha);

	if (!loader.Load())
	{
		return FALSE;
	}

	SINT w = loader.Width();
	SINT h = loader.Height();

	sx = w; 
	sy = h;

	SINT	c0, c1;
	
	UBYTE	*sp = loader.GetData();
	UBYTE	*ap = loader.GetAlphaData();

	for (c0 = 0; c0 < sy; c0++)
		for (c1 = 0; c1 < sx; c1++)
		{
			if (alpha == 0)
			{
				SINT	r = 0, g = 0, b = 0;

				UBYTE	*p = &sp[(((sy - c0 - 1) * w) + c1) * 3]; // TGAs are upside down
				b = p[0];
				g = p[1];
				r = p[2];

				data[(c0 * sx) + c1] = RGBToWord(r, g, b);
			}
			else
			{
				SINT	r = 0, g = 0, b = 0, a = 0;

				UBYTE	*p = &sp[(((sy - c0 - 1) * w) + c1) * 3]; // TGAs are upside down
				b = p[0];
				g = p[1];
				r = p[2];

				p = &ap[(((sy - c0 - 1) * w) + c1)]; // TGAs are upside down
				a = p[0];

				data[(c0 * sx) + c1] = RGBAToWord(r, g, b, a);
			}

		}

	isalpha = (alpha != 0);

	return TRUE;
}

//******************************************************************************************
BOOL	CTextureAccumulatorDC::LoadAndScaleTGA(char *fname, UWORD *data, BOOL &isalpha, SINT sx, SINT sy, ETADCSource source)
{
	char	name[256];
	
	// where are we looking?
	if (source == TADCS_DC)
		strcpy(name, "DCData\\Textures\\");
	else
		strcpy(name, "Data\\Textures\\");

	ASSERT(fname[0]);
	strcat(name, fname);

	
	SINT alpha = 0;
	CTGALoader loader(name, &alpha);

	if (!loader.Load())
	{
		return FALSE;
	}

	SINT w = loader.Width();
	SINT h = loader.Height();

	// is width a multiple of size?
	if (((w / sx) * sx) != w)
	{
		ASSERT(0);
		return FALSE;
	}

	// is height a multiple of sy
	if (((h / sy) * sy) != h)
	{
		ASSERT(0);
		return FALSE;
	}

	SINT	psx = w / sx;
	SINT	psy = h / sy;

	SINT	c0, c1, c2, c3;
	UBYTE	*sp = loader.GetData();
	UBYTE	*ap = loader.GetAlphaData();

	// scale with mip-map
	for (c0 = 0; c0 < sy; c0++)
		for (c1 = 0; c1 < sx; c1++)
		{
			if (alpha == 0)
			{
				SINT	r = 0, g = 0, b = 0;
				for (c2 = 0; c2 < psy; c2 ++)
					for (c3 = 0; c3 < psx; c3 ++)
					{
						UBYTE	*p = &sp[(((sy - c0 - 1) * w * psy) + (c1 * psx) + (c2 * w) + c3) * 3]; // TGAs are upside down
						b += p[0];
						g += p[1];
						r += p[2];
					}

				r /= psx * psy;
				g /= psx * psy;
				b /= psx * psy;

				data[(c0 * sx) + c1] = RGBToWord(r, g, b);
			}
			else
			{
				SINT	r = 0, g = 0, b = 0, a = 0;
				for (c2 = 0; c2 < psy; c2 ++)
					for (c3 = 0; c3 < psx; c3 ++)
					{
						UBYTE	*p = &sp[(((sy - c0 - 1) * w * psy) + (c1 * psx) + (c2 * w) + c3) * 3]; // TGAs are upside down
						b += p[0];
						g += p[1];
						r += p[2];

						p = &ap[(((sy - c0 - 1) * w * psy) + (c1 * psx) + (c2 * w) + c3)]; // TGAs are upside down
						a += p[0];
					}

				r /= psx * psy;
				g /= psx * psy;
				b /= psx * psy;
				a /= psx * psy;

				data[(c0 * sx) + c1] = RGBAToWord(r, g, b, a);
			}
		}

	isalpha = (alpha != 0);

	return TRUE;
}

//******************************************************************************************
void	CTextureAccumulatorDC::AddTexture(char *name, ETADCTextureType type, SINT number, SINT sx, SINT sy, SINT alpha_type, BOOL all_alpha, BOOL alpha_black, BOOL self_shadow)
{
	if (mNumTextures < TADC_MAX_TEXTURES)
	{
		// do we already have this texture?
		SINT	c0;
		for (c0 = 0; c0 < mNumTextures; c0 ++)
		{
			if ((type == mTextures[c0].mType) && (!(stricmp(name, mTextures[c0].mName))))
				return; // yes - so don't add
		}

		strcpy(mTextures[mNumTextures].mName, name);
		mTextures[mNumTextures].mType = type;
		mTextures[mNumTextures].mNumber = number;

		mTextures[mNumTextures].mAlphaType = alpha_type;
		
		mTextures[mNumTextures].mAllAlpha = all_alpha;
		if (type == TATT_MAP)
			mTextures[mNumTextures].mAllAlpha = TRUE;		// map textures always have this


		mTextures[mNumTextures].mAlphaBlack = alpha_black;
		mTextures[mNumTextures].mSelfShadow = self_shadow;

		// set default size based on type  - //! perhaps on other things, like original size!

		mTextures[mNumTextures].mMipMap = TRUE;
		mTextures[mNumTextures].mUseVQ = TRUE;

		if (sx != -1)
		{
			mTextures[mNumTextures].mWI = sx;
			mTextures[mNumTextures].mHI = sy;
		}
		else
		{
			switch (type)
			{
			case TADCTT_MAP:
//				mTextures[mNumTextures].mWI = 64;
//				mTextures[mNumTextures].mHI = 64;
				mTextures[mNumTextures].mWI = -1;
				mTextures[mNumTextures].mHI = -1;
				break;
			case TADCTT_MESH:
//				mTextures[mNumTextures].mWI = 64;
//				mTextures[mNumTextures].mHI = 64;
				mTextures[mNumTextures].mWI = -1;
				mTextures[mNumTextures].mHI = -1;
				break;
			case TADCTT_OTHER:
				mTextures[mNumTextures].mWI = -1;
				mTextures[mNumTextures].mHI = -1;
				break;
			case TADCTT_NUMBERED:
				mTextures[mNumTextures].mWI = -1;
				mTextures[mNumTextures].mHI = -1;
				mTextures[mNumTextures].mMipMap = FALSE;
				break;
			case TADCTT_PARTICLE:
				mTextures[mNumTextures].mWI = -1;
				mTextures[mNumTextures].mHI = -1;
				break;
			case TADCTT_FONT:
				ASSERT(0); // shouldn't happen
				mTextures[mNumTextures].mWI = -1;
				mTextures[mNumTextures].mHI = -1;
				mTextures[mNumTextures].mUseVQ = TRUE;
				mTextures[mNumTextures].mMipMap = FALSE;

				break;
			};
		}

		if (type == TADCTT_OTHER)
		{
//			if (number == -1)   // other textures ignore their numbers
				mTextures[mNumTextures].mNumber = mNumOtherTextures;			
			
			mNumOtherTextures++;
		}

		if ((type == TADCTT_MAP) ||
			(!(strnicmp(name, "water", 5))))
		{
			mTextures[mNumTextures].mDesiredPageSize = 128;
			mTextures[mNumTextures].mUseVQ = TRUE;
		}
		else
		{
			mTextures[mNumTextures].mDesiredPageSize = -1;
//			mTextures[mNumTextures].mUseVQ = FALSE;
		}

		if (type == TADCTT_NUMBERED)
			mNumNumberedTextures ++;

		if (type == TADCTT_PARTICLE)
			mNumParticleTextures ++;

		//! Should assert that this number has not been used already

		mNumTextures ++;
	}
}

//******************************************************************************************
void	CTextureAccumulatorDC::AddTexture(CTexture *t, ETADCTextureType type, SINT number, SINT sx, SINT sy, SINT alpha_type, BOOL all_alpha, BOOL alpha_black, BOOL self_shadow)
{
	if (t)
		AddTexture(t->GetName(), type, number, sx, sy, alpha_type, all_alpha, alpha_black, self_shadow);
}

//******************************************************************************************
// sort functions
int		STADCComp(const void *e1, const void *e2)
{
	STADCTexture *a = (STADCTexture *)e1;
	STADCTexture *b = (STADCTexture *)e2;

	SINT	sa = a->mWI * a->mHI;
	SINT	sb = b->mWI * b->mHI;

	return (sb - sa);
}

//******************************************************************************************
void	CTextureAccumulatorDC::BuildTPage(BOOL	frontend)
{
	// clear out used flags
	memset(mBlocksUsed, 0, sizeof(mBlocksUsed));
	memset(mPagesUsed , 0, sizeof(mPagesUsed ));

	SINT blah = 0;
	for (blah = 0; blah < TADC_MAX_PAGES; blah ++)
	{
		mIsPageAlpha[blah]			= EPAT_UNKNOWN;
		mIsPageVQ[blah]				= EPVQT_UNKNOWN;
		mIsPageMipMapped[blah]		= EPMMT_UNKNOWN;
		mPageTexture[blah][0]		= 0;

		mPageSize[blah] = -2; // undetermined
	}

	//! this is going to slow it down a bit
	// tits.  go through all the textures, and if the DC texture exists then use that size,

	// then add them all to the page
	SINT	cnt, c1, c2, c3;
#define SRC_IMAGE_SIZE	512

	UWORD			*src_image		= new UWORD[SRC_IMAGE_SIZE * SRC_IMAGE_SIZE];
	UWORD			*dest_image		= new UWORD[SRC_IMAGE_SIZE * SRC_IMAGE_SIZE];

	for	(cnt = 0; cnt < mNumTextures; cnt ++)
	{
		PLATFORM.ProcessMessages();

		STADCTexture *tex = &mTextures[cnt];
		BOOL	res, dummy;
		SINT tx, ty;
		res = LoadTGA(tex->mName, src_image, dummy, tx, ty, TADCS_DC);
		
		if (res)
		{
			// use the width and height from the tga
			tex->mWI = tx;
			tex->mHI = ty;
		}
		else if (tex->mWI == -1)
		{
			// force it to use the size of the PC texture
			res = LoadTGA(tex->mName, src_image, dummy, tx, ty, TADCS_PC);
			tex->mWI = tx;
			tex->mHI = ty;
		}

		if (((tex->mWI == 64) || (tex->mWI == 128)) && (tex->mHI == tex->mWI) && (tex->mMipMap))
			tex->mDesiredPageSize = tex->mWI;
		
		// JCL - hack the sky cube textures to 256x256
		if (tex->mWI == 512)
		{
//			tex->mUseVQ = TRUE;
			tex->mUseVQ = FALSE;
			tex->mMipMap = FALSE;
			tex->mDesiredPageSize = 512;
//			tex->mDesiredPageSize = 256;
//			tex->mWI = 256;
//			tex->mHI = 256;
		}

		if (frontend)
		{
			tex->mUseVQ = FALSE;
			tex->mMipMap = FALSE;
			tex->mDesiredPageSize = -1;
		}
	}

	// first sort textures in order of size
	qsort(mTextures, mNumTextures, sizeof(STADCTexture), STADCComp);

	// clear stuff out
	memset(dest_image, 0, 256 * 256 * 2);
//	memset(mData, 0, sizeof(mData));
//	memset(mDump, 0, sizeof(mDump));
	
	for	(cnt = 0; cnt < mNumTextures; cnt ++)
	{
		PLATFORM.ProcessMessages();

		STADCTexture *tex = &mTextures[cnt];
		// Load and scale it to the correct size
		BOOL	res;
				
		// try a DC texture first
		SINT	tx, ty;
		BOOL	is_alpha;

		res = LoadTGA(tex->mName, src_image, is_alpha, tx, ty, TADCS_DC);
		
		if (!res) // nope - kick artist and use PC one instead
		{
			res = LoadAndScaleTGA(tex->mName, src_image, is_alpha, tex->mWI, tex->mHI, TADCS_PC);
//			char	text[200];
//			sprintf(text, "No PSX texture for: %s\n", tex->mName);
//			OutputDebugString(text);
		}
		else
		{
			// use the width and height from the tga
			tex->mWI = tx;
			tex->mHI = ty;
		}

		if (!res)
		{
			// oops
			ASSERT(0);

			char foo[200];
			sprintf(foo, "Level %d: Couldn't add texture: %s\n", GAME.GetCurrentLevelNumber(), tex->mName);
			CONSOLE.AddString(foo);
			OutputDebugString(foo);

			tex->mX = 0;
			tex->mY = 0;

			continue;
		}

		SINT	block_width, block_height;
		BOOL	needs_alpha = FALSE;


		// insert cunning code here
		memset(dest_image, 0, sizeof(UWORD) * 256 * 256);

		SINT	c0;
		UWORD	*sptr = src_image;
		UWORD	*dptr = dest_image;
		for	(c0 = 0; c0 < tex->mWI * tex->mHI; c0 ++)
		{
/*			// just swap bgr to rgb
			UWORD v = BGRToRGB(*(sptr ++));
			
			// check alphas
			if (v == 0) v = 1;		// black goes up a bit
			if (v == 0x7c1f) v = 0;

			if (tex->mAllAlpha)
				v |= 0x8000;

			*(dptr ++) = v &  0xff;
			*(dptr ++) = v >> 8;*/

			if (!is_alpha)
			{
				// convert to 5 6 5
				SINT	r = ((*sptr) >> 10) & 0x1f;
				SINT	g = ((*sptr) >>  5) & 0x1f;
				SINT	b = ((*sptr)      ) & 0x1f;

				*dptr = (r << 11) + (g << 6) + b;
			}
			else
				*dptr = *sptr;

			dptr ++;
			sptr ++;
		}
		
		block_width  = tex->mWI;
		block_height = tex->mHI;

		// more hacking
//		if ((tex->mWI >= 64) && (!is_alpha) && (tex->mWI < 512))
//			tex->mUseVQ = TRUE;


		// look for somewhere to put this!

		SINT	bsx = ((block_width  - 1) / TA_BLOCK_SIZE) + 1; // think about it! (round up)
		SINT	bsy = ((block_height - 1) / TA_BLOCK_SIZE) + 1;

		//! simple scan through blocks..
		SINT	page;
		for (page = 0; page < TADC_MAX_PAGES; page ++)
		{
			if (tex->mDesiredPageSize != -1)
			{
				if (mPageSize[page] != -2)
					continue;
				else
					mPageSize[page] = tex->mDesiredPageSize; // set page size to this
			}
			else
			{
				if (mPageSize[page] == -2)
				{
//					if ((tex->mWI > 128) || (tex->mHI > 128))
						mPageSize[page] = 256; // default
//					else
//						mPageSize[page] = 128; // default
				}
			}

			for (c0 = 0; c0 < (mPageSize[page] / TADC_BLOCK_SIZE) - bsy + 1; c0 ++)
			{
				for (c1 = 0; c1 < (mPageSize[page] / TADC_BLOCK_SIZE) - bsx + 1; c1 ++)
				{
					if ((mIsPageAlpha[page] != EPAT_UNKNOWN) &&
						(  is_alpha  && (mIsPageAlpha[page] == EPAT_565 )) ||
						((!is_alpha) && (mIsPageAlpha[page] == EPAT_4444)))
						continue; // wrong page type

					if ((mIsPageVQ[page] != EPVQT_UNKNOWN) &&
						(  tex->mUseVQ  && (mIsPageVQ[page] == EPVQT_NO )) ||
						((!tex->mUseVQ) && (mIsPageVQ[page] == EPVQT_YES)))
						continue; // wrong page type

					if ((mIsPageMipMapped[page] != EPMMT_UNKNOWN) &&
						(  tex->mMipMap  && (mIsPageMipMapped[page] == EPMMT_NO )) ||
						((!tex->mMipMap) && (mIsPageMipMapped[page] == EPMMT_YES)))
						continue; // wrong page type

					BOOL	free = TRUE;

					for (c2 = 0; c2 < bsy; c2 ++)
						for (c3 = 0; c3 < bsx; c3 ++)
						{
							if (mBlocksUsed[page][c0 + c2][c1 + c3])
								free = FALSE;
						}

					if (free)
					{
						// wohoo - put this here!

						// first mark blocks as used
						for (c2 = 0; c2 < bsy; c2 ++)
							for (c3 = 0; c3 < bsx; c3 ++)
								mBlocksUsed[page][c0 + c2][c1 + c3] = TRUE;
						
						if (!mPagesUsed[page])
						{
							// first time here - clear the page
							memset(mData[page], 0, sizeof(mData[0]));
							memset(mDump[page], 0, sizeof(mDump[0]));
	
						}
						mPagesUsed[page] = TRUE;
						mIsPageAlpha[page] = is_alpha ? EPAT_4444 : EPAT_565;
						mIsPageVQ[page] = tex->mUseVQ ? EPVQT_YES : EPVQT_NO;
						mIsPageMipMapped[page] = tex->mMipMap ? EPMMT_YES : EPMMT_NO;

						if (tex->mDesiredPageSize != -1)
						{
							// has used all the page anyway
							strcpy(mPageTexture[page], tex->mName);
						}

						// write image data
						SINT	sx = c1 * TADC_BLOCK_SIZE;
						SINT	sy = c0 * TADC_BLOCK_SIZE;

						for (c2 = 0; c2 < block_height; c2 ++)
							for (c3 = 0; c3 < block_width; c3 ++)
							{
								mData[page][c2 + c0 * TA_BLOCK_SIZE][c3 + c1 * TA_BLOCK_SIZE] = dest_image[(c2 * block_width) + c3];
							}

						// calculate TPage coords
						tex->mX = float(c1 * TA_BLOCK_SIZE) / float(mPageSize[page]);
						tex->mY = float(c0 * TA_BLOCK_SIZE) / float(mPageSize[page]);
						tex->mW = float(tex->mWI) / float(mPageSize[page]);
						tex->mH = float(tex->mHI) / float(mPageSize[page]);
						tex->mPage = page;

						SINT	px = c1 * TA_BLOCK_SIZE;
						SINT	py = c0 * TA_BLOCK_SIZE;

						// write dump
						for (c2 = 0; c2 < block_height; c2 ++)
							for (c3 = 0; c3 < block_width; c3 ++)
							{
								UWORD	col;
								// debug
								if ((c2 == 0) || (c3 == 0))
								{
									if (is_alpha)
										col = 0xf01f;
									else if (tex->mUseVQ)
										col = 0xffff;
									else
										col = 0xf000;
								}
								else if ((c2 == block_height - 1) || (c3 == block_width - 1))
								{
									if (is_alpha)
										col = 0x070f;
									else if (tex->mUseVQ)
										col = 0xffff;
									else
										col = 0x001f;
								}
								else 
								{
									// convert to 555
									if (is_alpha)
									{
										col = dest_image[(c2 * block_width) + c3];
										SINT	b = (col >> 8) & 0x0f;
										SINT	g = (col >> 4) & 0x0f;
										SINT	r = (col     ) & 0x0f;
										col = (r << 11) + (g << 6) + (b << 1);
									}
									else
									{
										col = dest_image[(c2 * block_width) + c3];
										SINT	b = (col >> 11) & 0x1f;
										SINT	g = (col >>  6) & 0x1f;
										SINT	r = (col      ) & 0x1f;
										col = (r << 10) + (g << 5) + b;
									}
								}
								
								mDump[page][c2 + c0 * TA_BLOCK_SIZE][c3 + (c1 * TA_BLOCK_SIZE)] = col;
							}

						// bugger.
						goto tex_placed;
					}
				}
			}
		}

		char foo[200];
		sprintf(foo, "Level %d: Couldn't find space for texture: %s\n", GAME.GetCurrentLevelNumber(), tex->mName);
		CONSOLE.AddString(foo);
		#if TARGET != DC
		OutputDebugString(foo);
		#endif
tex_placed:;

		// if it's a font, calculate the character widths
		if (tex->mType == TATT_FONT)
			CalculateFontSpecs(src_image, tex);
	}
	delete [] dest_image;
	delete [] src_image;
}

//******************************************************************************************
void	CTextureAccumulatorDC::CalculateFontSpecs(UWORD *src_image, STADCTexture *tex)
{
	// scan through the source image, and calculate the width of each character

#define CHARS_PER_FONT_LINE		16
//#define	CHAR_HEIGHT				16

	SINT	char_height;
	
	switch (tex->mNumber)
	{
	case 1:	 char_height = 8; break;
	default: char_height = 16; break;
	}

	SINT	c0, c1, c2, c3, mw, cw = tex->mWI / CHARS_PER_FONT_LINE, nc = 0;
	
	for (c0 = 0; c0 < tex->mHI / char_height; c0 ++)
	{
		for (c1 = 0; c1 < CHARS_PER_FONT_LINE; c1 ++)
		{
			// for each character, find the rightmost non-zero texel.
			mw = 0;

			for (c3 = 0; c3 < cw; c3 ++)
			{
				for (c2 = 0; c2 < char_height; c2 ++)
				{
					if (src_image[(((c0 * char_height) + c2) * tex->mWI) + (c1 * cw) + c3] & 0xf000)  // something in the alpha channel
						mw = c3 + 1;
				}
			}

			mFonts[tex->mNumber].mWidths[nc ++] = mw;
		}
	}

	mFonts[tex->mNumber].mNumChars = nc;
	mFonts[tex->mNumber].mCharHeight = char_height;
	mFonts[tex->mNumber].mCharWidth = cw;
	mFonts[tex->mNumber].mCharsPerLine = CHARS_PER_FONT_LINE;
}

//******************************************************************************************
#ifdef LINK_EDITOR
UWORD	tempdump[512 * 512];
UWORD	tempdump2[512 * 512];
#endif

void	CTextureAccumulatorDC::WriteDump(char *fname)
{
	SINT	c0;
	for (c0 = 0; c0 < TADC_MAX_PAGES; c0 ++)
	{
		PLATFORM.ProcessMessages();

		if (mPagesUsed[c0])
		{
			char name[500];
			sprintf(name, "%s_%d.tga", fname, c0);

			// have to copy into temp space
			SINT	c1;
			for (c1 = 0; c1 < mPageSize[c0]; c1 ++)
				memcpy(tempdump + (c1 * mPageSize[c0]), &mDump[c0][c1][0], mPageSize[c0] * 2);


			BOOL	res = CTGALoader::Save16Bit(name, tempdump, mPageSize[c0], mPageSize[c0]);
//			BOOL	res = CTGALoader::Save16Bit(name, &mDump[c0][0][0], 512, 512);
		}
	}
}

//******************************************************************************************

//******************************************************************************************
STADCTexture *CTextureAccumulatorDC::GetTextureRef(ETADCTextureType type, SINT num)
{
	SINT	c1;
	for (c1 = 0; c1 < mNumTextures; c1 ++)
	{
		STADCTexture *tex = &mTextures[c1];

		if ((tex->mType == type) && (tex->mNumber == num))
			return tex;
	}

	return NULL;
}

//******************************************************************************************
STADCTexture *CTextureAccumulatorDC::GetTextureRef(ETADCTextureType type, CTexture *ct)
{
	// look up based on name
	if (!ct)
	{
		ASSERT(0);
		return NULL;
	}
	
	SINT	c1;
	for (c1 = 0; c1 < mNumTextures; c1 ++)
	{
		STADCTexture *tex = &mTextures[c1];

		if ((tex->mType == type) && (!(stricmp(tex->mName, ct->GetName()))))
			return tex;
	}

	char foo[200];
	sprintf(foo, "Level %d: Couldn't find texture that should be there...: %s\n", GAME.GetCurrentLevelNumber(), ct->GetName());
	CONSOLE.AddString(foo);
	#if TARGET != DC
	OutputDebugString(foo);
	#endif

	return NULL;
}

//******************************************************************************************
SINT	CTextureAccumulatorDC::GetTextureIndex(ETADCTextureType type, CTexture *ct)
{
	// look up based on name
	if (!ct)
	{
		ASSERT(0);
		return 0;
	}
	
	SINT	c1;
	for (c1 = 0; c1 < mNumTextures; c1 ++)
	{
		STADCTexture *tex = &mTextures[c1];

		if ((tex->mType == type) && (!(stricmp(tex->mName, ct->GetName()))))
			return mTextures[c1].mNumber;
	}

	return 0;
}

//******************************************************************************************
void	CTextureAccumulatorDC::Write(CMEMBUFFER &mem, BOOL frontend)
{
	UINT	CID_TPAG	= MKID("TPAG");
	UINT	TEX_VER	= (MKID("1.00"));

	mem.Write(&CID_TPAG, 4);	// write out chunk header
	mem.Write(&TEX_VER, 4);	// write out verson number

	// write out texture data

	// first, map textures
	SINT	c0;

	for (c0 = 0; c0 < MAX_MAP_TEXTURES; c0 ++)
	{
		// try to find the relevant texture
		STADCTexture *tex = NULL;
		
		CTexture *mt = MAP.GetMapTexture(c0);
		char	*tname = MAP.GetMapTextureName(c0);
		if ((mt) && (tname[0]))
			tex = GetTextureRef(TADCTT_MAP, mt);

//		char	*mt = MAP.GetMapTextureName(c0);
//		if (mt[0])
//			tex = GetTextureRef(TATT_MAP, c0);

		if (!tex)
		{
			tex = GetTextureRef(TADCTT_MAP, 0);  // use default.
		}

		if(tex)
		{
			// here it is
			mem.Write(&tex->mX, 4);
			mem.Write(&tex->mY, 4);
			mem.Write(&tex->mW, 4);
			mem.Write(&tex->mH, 4);
			mem.Write(&tex->mPage, 4);
		}
		else
		{
			// no texture - probably trying to write out the front end / level select stuff
			SINT	foo[5];
			mem.Write(&foo, 5 * 4);
		}
	}

	// now other textures
	mem.Write(&mNumOtherTextures, 4);		// write out number of textures

	for (c0 = 0; c0 < mNumOtherTextures; c0 ++)
	{
		// try to find the relevant texture
		STADCTexture *tex = GetTextureRef(TADCTT_OTHER, c0);

		if (!tex)
		{
			tex = GetTextureRef(TADCTT_OTHER, 0);  // use default.
		}
		
		ASSERT(tex);

		if (tex)
		{
			// here it is
			mem.Write(&tex->mX, 4);
			mem.Write(&tex->mY, 4);
			mem.Write(&tex->mW, 4);
			mem.Write(&tex->mH, 4);
			mem.Write(&tex->mPage, 4);
		}
		else
		{
			SINT	foo[5];
			mem.Write(&foo, 5 * 4);
		}
	}

	// now numbered textures
	mem.Write(&mNumNumberedTextures, 4);		// write out number of textures

	for (c0 = 0; c0 < mNumNumberedTextures; c0 ++)
	{
		// try to find the relevant texture
		STADCTexture *tex = GetTextureRef(TADCTT_NUMBERED, c0);

		if (!tex)
		{
			tex = GetTextureRef(TADCTT_NUMBERED, 0);  // use default.
		}
		
		ASSERT(tex);

		if (tex)
		{
			// here it is
			mem.Write(&tex->mX, 4);
			mem.Write(&tex->mY, 4);
			mem.Write(&tex->mW, 4);
			mem.Write(&tex->mH, 4);
			mem.Write(&tex->mPage, 4);
		}
		else
		{
			SINT	foo[5];
			mem.Write(&foo, 5 * 4);
		}
	}

	// now particle textures
	mem.Write(&mNumParticleTextures, 4);		// write out number of textures

	for (c0 = 0; c0 < mNumParticleTextures; c0 ++)
	{
		// try to find the relevant texture
		STADCTexture *tex = GetTextureRef(TADCTT_PARTICLE, c0);

		if (!tex)
		{
			tex = GetTextureRef(TADCTT_PARTICLE, 0);  // use default.
		}
		
		ASSERT(tex);

		if (tex)
		{
			// here it is
			mem.Write(&tex->mX, 4);
			mem.Write(&tex->mY, 4);
			mem.Write(&tex->mW, 4);
			mem.Write(&tex->mH, 4);
			mem.Write(&tex->mPage, 4);
		}
		else
		{
			SINT	foo[5];
			mem.Write(&foo, 5 * 4);
		}
	}

	// write out font data
	STADCTexture *tex1 = GetTextureRef(TADCTT_FONT, 0);
	STADCTexture *tex2 = GetTextureRef(TADCTT_FONT, 1);
	STADCTexture *tex3 = GetTextureRef(TADCTT_FONT, 2);

	SINT	num_fonts;

	if (!tex1)
		num_fonts = 0;
	else if (!tex2)
		num_fonts = 1;
	else if (!tex3)
		num_fonts = 2;
	else
		num_fonts = 3;

	mem.Write(&num_fonts, 4);

	if (tex1)
	{
		// write out font position
		mem.Write(&tex1->mX, 4);
		mem.Write(&tex1->mY, 4);
		mem.Write(&tex1->mPage, 4);

		// and write out the font data
		mem.Write(&mFonts[0], sizeof(mFonts[0]));

		if (tex2)
		{
			// write out font position
			mem.Write(&tex2->mX, 4);
			mem.Write(&tex2->mY, 4);
			mem.Write(&tex2->mPage, 4);

			// and write out the font data
			mem.Write(&mFonts[1], sizeof(mFonts[1]));
		}
			if (tex3)
			{
				// write out font position
				mem.Write(&tex3->mX, 4);
				mem.Write(&tex3->mY, 4);
				mem.Write(&tex3->mPage, 4);

				// and write out the font data
				mem.Write(&mFonts[2], sizeof(mFonts[2]));
			}
	}

	// now write out the pages

	// count them!
	SINT	count = 0;
	for (c0 = 0; c0 < TADC_MAX_PAGES; c0 ++)
		if (mPagesUsed[c0])
			count ++;

	// write out number of pages
	mem.Write(&count, 4);

	for (c0 = 0; c0 < TADC_MAX_PAGES; c0 ++)
	{
		if (mPagesUsed[c0])
		{
			if (frontend)
			{
				// no mipmapping or VQ
				mIsPageVQ[c0] = EPVQT_NO;
				mIsPageMipMapped[c0] = EPMMT_NO;
			}

			SINT	foo = mPageSize[c0];
			mem.Write(&foo, 4);		// Size X
			mem.Write(&foo, 4);		// Size Y
			foo = 0;
			if (mIsPageAlpha[c0] == EPAT_4444)
				foo |= 1;
			if (mIsPageVQ[c0] == EPVQT_YES)
				foo |= 2;
			if (mIsPageMipMapped[c0] == EPMMT_YES)
				foo |= 4; // mip map

			if (!(mIsPageVQ[c0] == EPVQT_YES))
				foo |= 8; // compress

			mem.Write(&foo, 4);		// Info = bit 0: 0 = 565, bit 1 = 4444, bit 1: 0 = normal, 1 = VQ compressed

			if (mIsPageVQ[c0] == EPVQT_YES)
			{
				if (mPageTexture[c0][0] != 0)
					GetCachedVQOrWrite(c0, mem);
				else
					WriteVQPage(c0, mem);
			}
			else
			{
/*				// have to write out in lines now...
				SINT	c1;
				for (c1 = 0; c1 < mPageSize[c0]; c1 ++)
					mem.Write(&mData[c0][c1][0], mPageSize[c0]* 2);
*/
				// compress
				SINT	x, y;
				for (y = 0; y < mPageSize[c0]; y++)
					for (x = 0; x < mPageSize[c0]; x++)
					{
						tempdump[(y * mPageSize[c0]) + x] = mData[c0][y][x];
					}

				SINT v = lzss_compress((unsigned char*) tempdump2,
									   (unsigned char*) tempdump,
									   mPageSize[c0] * mPageSize[c0] * 2);

				mem.Write(&v, 4);
				mem.Write(&tempdump2, v);
			}
		}
	}
}

//*****************************************************************
void	CTextureAccumulatorDC::WriteFontDats(char *base_name)
{
	// write out dats
	STADCTexture *tex1 = GetTextureRef(TADCTT_FONT, 0);
	STADCTexture *tex2 = GetTextureRef(TADCTT_FONT, 1);
	STADCTexture *tex3 = GetTextureRef(TADCTT_FONT, 2);

	char		name[200];

	if (tex1)
	{
		CMEMBUFFER	mem;
		mem.InitFromMem();
		mem.Write(&mFonts[0], sizeof(mFonts[0]));
		sprintf(name, "%s1.dat", base_name);
		mem.Close(name);
	}
	if (tex2)
	{
		CMEMBUFFER	mem;
		mem.InitFromMem();
		mem.Write(&mFonts[1], sizeof(mFonts[0]));
		sprintf(name, "%s2.dat", base_name);
		mem.Close(name);
	}
	if (tex3)
	{
		CMEMBUFFER	mem;
		mem.InitFromMem();
		mem.Write(&mFonts[2], sizeof(mFonts[0]));
		sprintf(name, "%s3.dat", base_name);
		mem.Close(name);
	}
}

//*****************************************************************
void	CTextureAccumulatorDC::WriteVQPage(SINT n, CMEMBUFFER &mem)
{
	// Write the page out as a TGA
	// convert the TGA to a PVR
	// load in the PVR and write it to the membuffer.

	char name[500];
	sprintf(name, "c:\\dev\\ballz\\in.tga");

	// build the tga 

	// NOTE - PAGE MUST NOT BE ALPHA!

	SINT	c0, c1;

	for (c0 = 0; c0 < mPageSize[n]; c0 ++)
		for (c1 = 0; c1 < mPageSize[n]; c1 ++)
		{
			DWORD	col = mData[n][c0][c1];

			if (mIsPageAlpha[n] == EPAT_565)
			{
				SINT	b = (col >> 11) & 0x1f;
				SINT	g = (col >>  6) & 0x1f;
				SINT	r = (col      ) & 0x1f;
				col = (r << 10) + (g << 5) + b;
			}

			tempdump[c1 + (c0 * mPageSize[n])] = col;
		}

	if (mIsPageAlpha[n] == EPAT_4444)
		CTGALoader::Save16Bit4444(name, tempdump, mPageSize[n], mPageSize[n]);
	else
		CTGALoader::Save16Bit(name, tempdump, mPageSize[n], mPageSize[n]);

	if (mIsPageMipMapped[n] == EPMMT_YES)
		system("command /c c:\\dev\\ballz\\pvrc.bat");
	else
		system("command /c c:\\dev\\ballz\\pvrc2.bat");

	// now load and write
	CMEMBUFFER	mem2;

	mem2.InitFromFile("c:\\dev\\ballz\\in.pvr");

	SINT size = mem2.GetDataSize();

	mem.Write(&size, 4);

	UBYTE	i;

	// shit
	for (c0 = 0; c0 < size; c0 ++)
	{
		mem2.Read(&i, 1);
		mem.Write(&i, 1);
	}

	mem2.Close();
}

//*****************************************************************
void	CTextureAccumulatorDC::GetCachedVQOrWrite(SINT n, CMEMBUFFER &mem)
{
	// texture is whole page - have we cached this?
	CMEMBUFFER	mem2;

	char	foo[400], name[400];
	strcpy(foo, mPageTexture[n]);

	foo[strlen(foo) - 4] = 0;  //strip of the .tga

	sprintf(name, "c:\\dev\\ballz\\data\\textures\\%s.pvr", foo);

	if (!(mem2.InitFromFile(name)))
	{
		// not there - need to create
		char name2[500];
		sprintf(name2, "c:\\dev\\ballz\\in.tga");

		// build the tga 

		// NOTE - PAGE MUST NOT BE ALPHA!

		SINT	c0, c1;

		for (c0 = 0; c0 < mPageSize[n]; c0 ++)
			for (c1 = 0; c1 < mPageSize[n]; c1 ++)
			{
				DWORD	col = mData[n][c0][c1];

				if (mIsPageAlpha[n] == EPAT_565)
				{
					SINT	b = (col >> 11) & 0x1f;
					SINT	g = (col >>  6) & 0x1f;
					SINT	r = (col      ) & 0x1f;
					col = (r << 10) + (g << 5) + b;
				}

				tempdump[c1 + (c0 * mPageSize[n])] = col;
			}

		if (mIsPageAlpha[n] == EPAT_4444)
			CTGALoader::Save16Bit4444(name2, tempdump, mPageSize[n], mPageSize[n]);
		else
			CTGALoader::Save16Bit(name2, tempdump, mPageSize[n], mPageSize[n]);


		char cmd[500];

		if (mIsPageMipMapped[n] == EPMMT_YES)
			sprintf(cmd, "command /c c:\\dev\\ballz\\pvrcr.bat %s", name);
		else
			sprintf(cmd, "command /c c:\\dev\\ballz\\pvrcr2.bat %s", name);

		system(cmd);

		mem2.InitFromFile(name);
	}

	SINT size = mem2.GetDataSize();

	mem.Write(&size, 4);

	UBYTE	i;
	SINT	c0;

	// shit
	for (c0 = 0; c0 < size; c0 ++)
	{
		mem2.Read(&i, 1);
		mem.Write(&i, 1);
	}

	mem2.Close();


}


#endif
