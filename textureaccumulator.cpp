#include	"Common.h"

#ifdef LINK_EDITOR

#include	"TextureAccumulator.h"
#include	"Editors.h"
#include	"TGALoader.h"
#include	"Globals.h"

inline	UWORD	BGRToRGB(UWORD i)
{
	SINT	r = (i >> 10) & 31;
	SINT	g = (i >>  5) & 31;
	SINT	b = (i >>  0) & 31;

	return (b << 10) + (g << 5) + r;
}

//******************************************************************************************
BOOL	CTextureAccumulator::LoadTGA(char *fname, UWORD *data, SINT &sx, SINT &sy, ETASource source)
{
	char	name[256];
	
	// where are we looking?
	if (source == TAS_PSX)
		strcpy(name, "MoHoData\\Textures\\");
	else
		strcpy(name, "Data\\Textures\\");

	ASSERT(fname[0]);
	strcat(name, fname);

	
	SINT dummy;
	CTGALoader loader(name, &dummy);

	if (!loader.Load())
	{
		return FALSE;
	}

	SINT w = loader.Width();
	SINT h = loader.Height();

	sx = w; 
	sy = h;

	SINT	c0, c1;
	
#if TARGET == PC
	UBYTE	*sp = loader.GetData();
#endif
#if TARGET == DC
	UBYTE	*sp = (unsigned char *)loader.GetData();
#endif	

	for (c0 = 0; c0 < sy; c0++)
		for (c1 = 0; c1 < sx; c1++)
		{
			SINT	r = 0, g = 0, b = 0;

			UBYTE	*p = &sp[(((sy - c0 - 1) * w) + c1) * 3]; // TGAs are upside down
			b = p[0];
			g = p[1];
			r = p[2];

			data[(c0 * sx) + c1] = RGBToWord(r, g, b);
		}

	return TRUE;
}

//******************************************************************************************
BOOL	CTextureAccumulator::LoadAndScaleTGA(char *fname, UWORD *data, SINT sx, SINT sy, ETASource source)
{
	char	name[256];
	
	// where are we looking?
	if (source == TAS_PSX)
		strcpy(name, "MoHoData\\Textures\\");
	else
		strcpy(name, "Data\\Textures\\");

	ASSERT(fname[0]);
	strcat(name, fname);

	
	SINT dummy;
	CTGALoader loader(name, &dummy);

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


	// scale with mip-map
	for (c0 = 0; c0 < sy; c0++)
		for (c1 = 0; c1 < sx; c1++)
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

	return TRUE;
}

//******************************************************************************************
void	CTextureAccumulator::AddTexture(char *name, ETATextureType type, SINT number, SINT sx, SINT sy, EBitDepth bd, SINT alpha_type, BOOL all_alpha, BOOL alpha_black, BOOL self_shadow)
{
	if (mNumTextures < TA_MAX_TEXTURES)
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

		mTextures[mNumTextures].mDepth = bd;
		mTextures[mNumTextures].mAlphaType = alpha_type;
		
		mTextures[mNumTextures].mAllAlpha = all_alpha;
		if (type == TATT_MAP)
			mTextures[mNumTextures].mAllAlpha = TRUE;		// map textures always have this


		mTextures[mNumTextures].mAlphaBlack = alpha_black;
		mTextures[mNumTextures].mSelfShadow = self_shadow;

		// set default size based on type  - //! perhaps on other things, like original size!

		if (sx != -1)
		{
			mTextures[mNumTextures].mW = sx;
			mTextures[mNumTextures].mH = sy;
		}
		else
		{
			switch (type)
			{
			case TATT_MAP:
				mTextures[mNumTextures].mW = 64;
				mTextures[mNumTextures].mH = 64;
				break;
			case TATT_MESH:
				mTextures[mNumTextures].mW = 64;
				mTextures[mNumTextures].mH = 64;
				break;
			case TATT_OTHER:
				mTextures[mNumTextures].mW = 64;
				mTextures[mNumTextures].mH = 64;
				break;
			case TATT_NUMBERED:
				mTextures[mNumTextures].mW = 64;
				mTextures[mNumTextures].mH = 64;
				break;
			case TATT_PARTICLE:
				mTextures[mNumTextures].mW = 32;
				mTextures[mNumTextures].mH = 32;
				break;
			case TATT_FONT:
				ASSERT(0); // shouldn't happen
				mTextures[mNumTextures].mW = 64;
				mTextures[mNumTextures].mH = 64;
				break;
			};
		}

		if (type == TATT_OTHER)
		{
//			if (number == -1)   // other textures ignore their numbers
				mTextures[mNumTextures].mNumber = mNumOtherTextures;			
			
			mNumOtherTextures++;
		}

		if (type == TATT_NUMBERED)
			mNumNumberedTextures ++;

		if (type == TATT_PARTICLE)
			mNumParticleTextures ++;

		//! Should assert that this number has not been used already

		mNumTextures ++;
	}
}

//******************************************************************************************
void	CTextureAccumulator::AddTexture(CTexture *t, ETATextureType type, SINT number, SINT sx, SINT sy, EBitDepth bd, SINT alpha_type, BOOL all_alpha, BOOL alpha_black, BOOL self_shadow)
{
	if (t)
		AddTexture(t->GetName(), type, number, sx, sy, bd, alpha_type, all_alpha, alpha_black, self_shadow);
}

//******************************************************************************************
struct	SColourCount
{
	UWORD	mCol;
	SINT	mCount;
};

//******************************************************************************************
// sort functions
int		STAComp(const void *e1, const void *e2)
{
	STATexture *a = (STATexture *)e1;
	STATexture *b = (STATexture *)e2;

	SINT	ad;
	SINT	bd;

	if (a->mDepth == BD_4)
		ad = 1;
	else if (a->mDepth == BD_8)
		ad = 2;
	else if (a->mDepth == BD_16)
		ad = 4;

	if (b->mDepth == BD_4)
		bd = 1;
	else if (b->mDepth == BD_8)
		bd = 2;
	else if (b->mDepth == BD_16)
		bd = 4;


	SINT	sa = a->mW * ad * a->mH;
	SINT	sb = b->mW * bd * b->mH;

	return (sb - sa);
}

int		col_comp(const void *e1, const void *e2)
{
	SColourCount *a = (SColourCount *)e1;
	SColourCount *b = (SColourCount *)e2;

	SINT	sa = a->mCount;
	SINT	sb = b->mCount;

	return (sb - sa);
}

//******************************************************************************************
void	CTextureAccumulator::GetNextClut4Bit(SINT &x, SINT &y)
{
	SINT	c0, c1, c2, c3;

	if (mClut4SubBlockY == -1)
	{
		// got to find a new page
		SINT	bsx = 32  / TA_BLOCK_SIZE;
		SINT	bsy = 32 / TA_BLOCK_SIZE;

		//! simple scan through blocks...
		for (c0 = TA_NUM_BLOCKS_Y - bsy; c0 >= 0 ; c0 --)
			for (c1 = TA_NUM_BLOCKS_X - bsx; c1 >= 0 ; c1 --)
			{
				BOOL	free = TRUE;

				for (c2 = 0; c2 < bsy; c2 ++)
					for (c3 = 0; c3 < bsx; c3 ++)
					{
						if (mBlocksUsed[c0 + c2][c1 + c3])
							free = FALSE;
					}

				if (free)
				{
					mClut4BlockX = c1;
					mClut4BlockY = c0;
					mClut4SubBlockY = 31;

					// mark blocks as used
					for (c2 = 0; c2 < bsy; c2 ++)
						for (c3 = 0; c3 < bsx; c3 ++)
							mBlocksUsed[c0 + c2][c1 + c3] = TRUE;

					goto clut4block_found;
				}
			}

clut4block_found:;
	}

	x = mClut4BlockX * TA_BLOCK_SIZE;
	y = (mClut4BlockY * TA_BLOCK_SIZE) + mClut4SubBlockY;

	mClut4SubBlockY --;
}

//******************************************************************************************
void	CTextureAccumulator::GetNextClut8Bit(SINT &x, SINT &y)
{
	SINT	c0, c1, c2, c3;

	if (mClut8SubBlockY == -1)
	{
		// got to find a new page
		SINT	bsx = 512 / TA_BLOCK_SIZE;
//		SINT	bsy = 32 / TA_BLOCK_SIZE;
		SINT	bsy = 48 / TA_BLOCK_SIZE;

		//! simple scan through blocks...
		for (c0 = TA_NUM_BLOCKS_Y - bsy; c0 >= 0 ; c0 --)
			for (c1 = 0; c1 < TA_NUM_BLOCKS_X - bsx ; c1 ++)
			{
				BOOL	free = TRUE;

				for (c2 = 0; c2 < bsy; c2 ++)
					for (c3 = 0; c3 < bsx; c3 ++)
					{
						if (mBlocksUsed[c0 + c2][c1 + c3])
							free = FALSE;
					}

			 	if (free)
				{
					mClut8BlockX = c1;
					mClut8BlockY = c0;
//					mClut8SubBlockY = 31;
					mClut8SubBlockY = 47;

					// mark blocks as used
					for (c2 = 0; c2 < bsy; c2 ++)
						for (c3 = 0; c3 < bsx; c3 ++)
							mBlocksUsed[c0 + c2][c1 + c3] = TRUE;

					goto clut8block_found;
				}
			}

clut8block_found:;
	}

	x = mClut8BlockX * TA_BLOCK_SIZE;
	y = (mClut8BlockY * TA_BLOCK_SIZE) + mClut8SubBlockY;

	mClut8SubBlockY --;
}

//******************************************************************************************
void	CTextureAccumulator::BuildTPage()
{
	// clear out used flags
	memset(mBlocksUsed, 0, sizeof(mBlocksUsed));

	// clear clut pages
	mClut4SubBlockY = -1;  // invalid state
	mClut8SubBlockY = -1;  // invalid state

	//! this is going to slow it down a bit
	// tits.  go through all the textures, and if the PSX texture exists then use that size,

	// then add them all to the page
	SINT	cnt, c0, c1, c2, c3;
#define SRC_IMAGE_SIZE	512

	UWORD			*src_image		= new UWORD[SRC_IMAGE_SIZE * SRC_IMAGE_SIZE];
	UBYTE			*dest_image		= new UBYTE[1024 * 256];
	UWORD			*clut			= new UWORD[256];

	SColourCount	cc[256 * 256];

	for	(cnt = 0; cnt < mNumTextures; cnt ++)
	{
		STATexture *tex = &mTextures[cnt];
		BOOL	res;
		SINT tx, ty;
		res = LoadTGA(tex->mName, src_image, tx, ty, TAS_PSX);
		
		if (res)
		{
			// use the width and height from the tga
			tex->mW = tx;
			tex->mH = ty;
		}
	}

	// first sort textures in order of size
	qsort(mTextures, mNumTextures, sizeof(STATexture), STAComp);

	// clear stuff out
	memset(dest_image, 0, 512 * 256);
	memset(mData, 0, TA_SIZE_Y * TA_SIZE_X);
	memset(mDump, 0, TA_SIZE_Y * TA_SIZE_X * 2 * 2);
	
	for	(cnt = 0; cnt < mNumTextures; cnt ++)
	{
		STATexture *tex = &mTextures[cnt];
		// Load and scale it to the correct size
		BOOL	res;
				
		// try a PSX texture first
		SINT tx, ty;
		res = LoadTGA(tex->mName, src_image, tx, ty, TAS_PSX);
		
		if (!res) // nope - kick artist and use PC one instead
		{
			res = LoadAndScaleTGA(tex->mName, src_image, tex->mW, tex->mH, TAS_PC);
//			char	text[200];
//			sprintf(text, "No PSX texture for: %s\n", tex->mName);
//			OutputDebugString(text);
		}
		else
		{
			// use the width and height from the tga
			tex->mW = tx;
			tex->mH = ty;
		}

		if (!res)
		{
			// oops
			ASSERT(0);

			char foo[200];
			sprintf(foo, "Level %d: Couldn't add texture: %s\n", GAME.GetCurrentLevelNumber(), tex->mName);
			CONSOLE.AddString(foo);
			#if TARGET == PC
			OutputDebugString(foo);
			#endif

			tex->mX = 0;
			tex->mY = 0;
			tex->mClutX = 0;
			tex->mClutY = 0;

			continue;
		}

		SINT	block_width, block_height;

		switch (tex->mDepth)
		{
		//********************************************
		case BD_4:
			{
				// cheap convert to the right bit depth
				// cheesy.  look for the 14 most common colours and the 2 least common.
				
				// clear table
				memset(dest_image, 0, sizeof(UBYTE) * 512 * 256);

				SINT	num_cc = 0;

				// add colours to list
				SINT	c0;
				UWORD	*ptr = src_image;
				for	(c0 = 0; c0 < tex->mW * tex->mH; c0 ++)
				{
					UWORD	col = (*ptr) & 0x7fff; 

					// do we have this colour?
					for (c1 = 0; c1 < num_cc; c1 ++)
					{
						if (cc[c1].mCol == col)
						{
							cc[c1].mCount ++;
							break;
						}
					}
					if (c1 == num_cc)
					{
						// didn't find it
						cc[num_cc].mCol = col;
						cc[num_cc].mCount = 1;
						num_cc ++;
					}
					ptr ++;
				}

				// pick out colours

				// sort high
				qsort(cc, num_cc, sizeof(SColourCount), col_comp);

				// try to pick 13 of the colours in the top half of popularity.
				SINT	step = ((num_cc) / 2) / 13;
				if (step < 1) step = 1;
				for (c0 = 0; c0 < 13; c0 ++)
				{
					clut[c0] = cc[c0 * step].mCol;
				}
				// try to pick 3 of the colours in the bottom half of popularity.
				SINT	half = num_cc / 2;
				if (half > (num_cc - (13 * step)))
					half = (num_cc - (13 * step));
				step = (half) / 3;
				if (step < 1) step = 1;
				SINT off = (num_cc / 2);
				if (off < 13) off = 13; 
				for (c0 = 0; c0 < 3; c0 ++)
				{
					clut[c0 + 13] = cc[c0 * step + off].mCol;
				}
				
				// now scan through and assign palette indices.

				// attempt at error diffusion
				SINT	errr = 0, errg = 0, errb = 0;

				for	(c0 = 0; c0 < tex->mW * tex->mH; c0 ++)
				{
					if ((c0 / tex->mW) * tex->mW == 0)
					{
						// reset errors at beginning of row
						errr = 0;
						errg = 0;
						errb = 0;
					}


					UWORD	src = src_image[c0];

					SINT	sr = ((src >> 10) & 31) + (errr * 7) / 8;
					SINT	sg = ((src >> 5 ) & 31) + (errg * 7) / 8;
					SINT	sb = ((src      ) & 31) + (errb * 7) / 8;

					// scan through available cluts

					SINT	min_dist = 100000;
					SINT	min_num = 0;

					for (c1 = 0; c1 < 16; c1 ++)
					{
						UWORD	clcol = clut[c1];

						SINT cr = (clcol >> 10) & 31;
						SINT cg = (clcol >> 5 ) & 31;
						SINT cb = (clcol      ) & 31;

						SINT diff = abs(sr - cr) + abs(sg - cg) + abs(sb - cb);

						if (diff < min_dist)
						{
							min_dist = diff;
							min_num = c1;
						}
					}

					// and write out dest pixel
					dest_image[c0 >> 1] |= min_num << ((c0 & 1) ? 4 : 0);		// shift into high nibble?

					// pass on error
					UWORD	clcol = clut[min_num];

					SINT cr = (clcol >> 10) & 31;
					SINT cg = (clcol >> 5 ) & 31;
					SINT cb = (clcol      ) & 31;

					errr = sr - cr;
					errg = sg - cg;
					errb = sb - cb;
	//				errr = cr - sr;
	//				errg = cg - sg;
	//				errb = cb - sb;
				}

				// switch the clut rgb to bgr
				for (c0 = 0; c0 < 16; c0 ++)
				{
					UWORD	clcol = clut[c0];

					SINT cr = (clcol >> 10) & 31;
					SINT cg = (clcol >> 5 ) & 31;
					SINT cb = (clcol      ) & 31;

					// also, check - if 255, 255, 0 go to 0, 0, 0, if 0, 0, 0 go to 0, 0, 1
					if ((!tex->mAllAlpha) || (tex->mAlphaBlack))
					{
						if ((cr == 0) && (cg == 0) && (cb == 0))
							cr = 1;
					}
					if ((cr == 31) && (cg == 0) && (cb == 31))
					{
						cr = cg = cb = 0;
					}

					clut[c0] = (cb << 10) + (cg << 5) + cr;
				}
				
				block_width = tex->mW / 2;
				block_height = tex->mH;
			}
			break;
		//********************************************
#if 1
		case BD_8:
			{
				// cheap convert to the right bit depth
				// cheesy.  look for the 256 most common colours
				
				// clear table
				memset(dest_image, 0, sizeof(UBYTE) * 512 * 256);

				SINT	num_cc = 0;

				// add colours to list
				SINT	c0;
				UWORD	*ptr = src_image;
				for	(c0 = 0; c0 < tex->mW * tex->mH; c0 ++)
				{
					UWORD	col = (*ptr) & 0x7fff; 

					// do we have this colour?
					for (c1 = 0; c1 < num_cc; c1 ++)
					{
						if (cc[c1].mCol == col)
						{
							cc[c1].mCount ++;
							break;
						}
					}
					if (c1 == num_cc)
					{
						// didn't find it
						cc[num_cc].mCol = col;
						cc[num_cc].mCount = 1;
						num_cc ++;
					}
					ptr ++;
				}

				// pick out colours

				// sort high
				qsort(cc, num_cc, sizeof(SColourCount), col_comp);

				// pick the top 256 colours
				for (c0 = 0; c0 < 256; c0 ++)
				{
					clut[c0] = cc[c0].mCol;
				}
				// now scan through and assign palette indices.

				// attempt at error diffusion
				SINT	errr = 0, errg = 0, errb = 0;

				for	(c0 = 0; c0 < tex->mW * tex->mH; c0 ++)
				{
					if ((c0 / tex->mW) * tex->mW == 0)
					{
						// reset errors at beginning of row
						errr = 0;
						errg = 0;
						errb = 0;
					}


					UWORD	src = src_image[c0];

					SINT	sr = ((src >> 10) & 31) + (errr * 7) / 8;
					SINT	sg = ((src >> 5 ) & 31) + (errg * 7) / 8;
					SINT	sb = ((src      ) & 31) + (errb * 7) / 8;

					// scan through available cluts

					SINT	min_dist = 100000;
					SINT	min_num = 0;

					for (c1 = 0; c1 < 256; c1 ++)
					{
						UWORD	clcol = clut[c1];

						SINT cr = (clcol >> 10) & 31;
						SINT cg = (clcol >> 5 ) & 31;
						SINT cb = (clcol      ) & 31;

						SINT diff = abs(sr - cr) + abs(sg - cg) + abs(sb - cb);

						if (diff < min_dist)
						{
							min_dist = diff;
							min_num = c1;
						}
					}

					// and write out dest pixel
					dest_image[c0] = min_num ;

					// pass on error
					UWORD	clcol = clut[min_num];

					SINT cr = (clcol >> 10) & 31;
					SINT cg = (clcol >> 5 ) & 31;
					SINT cb = (clcol      ) & 31;

					errr = sr - cr;
					errg = sg - cg;
					errb = sb - cb;
				}

				// switch the clut rgb to bgr
				for (c0 = 0; c0 < 256; c0 ++)
				{
					UWORD	clcol = clut[c0];

					SINT cr = (clcol >> 10) & 31;
					SINT cg = (clcol >> 5 ) & 31;
					SINT cb = (clcol      ) & 31;

					// also, check - if 255, 255, 0 go to 0, 0, 0, if 0, 0, 0 go to 0, 0, 1
					if ((!tex->mAllAlpha) || (tex->mAlphaBlack))
					{
						if ((cr == 0) && (cg == 0) && (cb == 0))
							cr = 1;
					}
					if ((cr == 31) && (cg == 0) && (cb == 31))
					{
						cr = cg = cb = 0;
					}

					clut[c0] = (cb << 10) + (cg << 5) + cr;
				}
				
				block_width = tex->mW;
				block_height = tex->mH;
			}
			break;
#endif
		//********************************************
		case BD_16:
			{
				// this is slightly easier!  not much to do.
				memset(dest_image, 0, sizeof(UBYTE) * 512 * 256);

				SINT	c0;
				UWORD	*sptr = src_image;
				UBYTE	*dptr = dest_image;
				for	(c0 = 0; c0 < tex->mW * tex->mH; c0 ++)
				{
					// just swap bgr to rgb
					UWORD v = BGRToRGB(*(sptr ++));
					
					// check alphas
					if (v == 0) v = 1;		// black goes up a bit
					if (v == 0x7c1f) v = 0;

					if (tex->mAllAlpha)
						v |= 0x8000;

					*(dptr ++) = v &  0xff;
					*(dptr ++) = v >> 8;
				}
				
				block_width = tex->mW * 2;
				block_height = tex->mH;
			}
			break;
		};

		// look for somewhere to put this!

		SINT	bsx = ((block_width  - 1) / TA_BLOCK_SIZE) + 1; // think about it! (round up)
		SINT	bsy = ((block_height - 1) / TA_BLOCK_SIZE) + 1;

		//! simple scan through blocks...
		for (c0 = 0; c0 < TA_NUM_BLOCKS_Y - bsy + 1; c0 ++)
			for (c1 = 0; c1 < TA_NUM_BLOCKS_X - bsx + 1; c1 ++)
			{
				// 1st, can I put this here, because of texture page limitations?
				if (tex->mDepth == BD_4)
					if (((c1 * TA_BLOCK_SIZE / 2) & 63) + block_width / 2 > 64)
						continue; // no
				if (tex->mDepth == BD_8)
					if (((c1 * TA_BLOCK_SIZE / 2) & 63) + block_width / 2 > 128)
						continue; // no

				BOOL	free = TRUE;

				// does it over half y?
				if ((c0 < TA_NUM_BLOCKS_Y / 2) && (c0 + bsy > TA_NUM_BLOCKS_Y / 2))
					free = FALSE;

				for (c2 = 0; c2 < bsy; c2 ++)
					for (c3 = 0; c3 < bsx; c3 ++)
					{
						if (mBlocksUsed[c0 + c2][c1 + c3])
							free = FALSE;
					}

				if (free)
				{
					// wohoo - put this here!

					// first mark blocks as used
					for (c2 = 0; c2 < bsy; c2 ++)
						for (c3 = 0; c3 < bsx; c3 ++)
							mBlocksUsed[c0 + c2][c1 + c3] = TRUE;

					// write image data
					SINT	sx = c1 * TA_BLOCK_SIZE;
					SINT	sy = c0 * TA_BLOCK_SIZE;

					for (c2 = 0; c2 < block_height; c2 ++)
						for (c3 = 0; c3 < block_width; c3 ++)
						{
							mData[c2 + c0 * TA_BLOCK_SIZE][c3 + c1 * TA_BLOCK_SIZE] = dest_image[(c2 * block_width) + c3];
						}

					// calculate TPage coords
					tex->mX = c1 * TA_BLOCK_SIZE / 2;
					tex->mY = c0 * TA_BLOCK_SIZE;

					SINT	px = c1 * TA_BLOCK_SIZE / 2;
					SINT	py = c0 * TA_BLOCK_SIZE;

					// work out texture page..
					if (py < 256)
							tex->mTPageY = 0;
					else	tex->mTPageY = 256;

					tex->mTPageX = (px & (~63)) + 512;

					tex->mX = px - tex->mTPageX + 512;
					tex->mY = py - tex->mTPageY;

					// write dump
					for (c2 = 0; c2 < block_height; c2 ++)
						for (c3 = 0; c3 < block_width * 2; c3 ++)
						{
							switch (tex->mDepth)
							{
							case BD_4:
								// write 4-bit clut pixel
								{
									UWORD	col;
									// debug
									if ((c2 == 0) || (c3 == 0))
										col = 0xf000;
									else if ((c2 == block_height - 1) || (c3 == block_width * 2 - 1))
										col = 0x001f;
									else
										col = clut[((dest_image[(c2 * block_width) + (c3 / 2)] >> ((c3 & 1) ? 4 : 0)) & 15)];
									mDump[c2 + c0 * TA_BLOCK_SIZE][c3 + (c1 * TA_BLOCK_SIZE * 2)] = col;
								}
								break;
#if 1
							case BD_8:
								// write 8-bit clut pixel
								{
									UWORD	col;
									// debug
									if ((c2 == 0) || (c3 == 0))
										col = 0xf000;
									else if ((c2 == block_height - 1) || (c3 == block_width * 2 - 1))
										col = 0x001f;
									else
										col = clut[dest_image[(c2 * block_width) + (c3 / 2)]];
									mDump[c2 + c0 * TA_BLOCK_SIZE][c3 + (c1 * TA_BLOCK_SIZE * 2)] = col;
								}
								break;
#endif
							case BD_16:
								{
									UWORD	col;
									// debug
									if ((c2 == 0) || (c3 == 0))
										col = 0xf000;
									else if ((c2 == block_height - 1) || (c3 == block_width * 2 - 1))
										col = 0x001f;
									else 
										col = (UWORD(dest_image[(c2 * block_width) + ((c3 >> 2) << 1) + 1]) << 8) + UWORD(dest_image[(c2 * block_width) + ((c3 >> 2) << 1)]) ;
									
									mDump[c2 + c0 * TA_BLOCK_SIZE][c3 + (c1 * TA_BLOCK_SIZE * 2)] = col;
								}
								break;
							}
						}

					// write clut
					switch (tex->mDepth)
					{
					case BD_4:
						{
							SINT tcx, tcy;

							GetNextClut4Bit(tcx, tcy);
							UBYTE	*caddr = &mData[tcy][tcx];

							UWORD	*baddr = (UWORD *)caddr;
							for (c2 = 0; c2 < 16; c2 ++)
							{
								// copy and set high bit if necessary
								if (tex->mSelfShadow)
								{
									if (clut[c2] == (31 << 5))  // all green
										baddr[c2] = 0x8000 + (6 << 10) + (6 << 5) + 6;
									else
										baddr[c2] = clut[c2];
								}
								else if ((tex->mAllAlpha) && clut[c2])
									baddr[c2] = clut[c2] | 0x8000;
								else
									baddr[c2] = clut[c2];
							}
							// write clut dump
							for (c2 = 0; c2 < 64; c2 ++)
							{
								mDump[tcy][tcx * 2 + c2] = clut[c2 >> 2];
							}

							tex->mClutX = (tcx / 2) + 512;
							tex->mClutY = tcy;
						}
						break;
#if 1
					case BD_8:
						{
							SINT tcx, tcy;

							GetNextClut8Bit(tcx, tcy);
							UBYTE	*caddr = &mData[tcy][tcx];

							UWORD	*baddr = (UWORD *)caddr;
							for (c2 = 0; c2 < 256; c2 ++)
							{
								// copy and set high bit if necessary
								if (tex->mSelfShadow)
								{
									if (clut[c2] == (31 << 5))  // all green
										baddr[c2] = 0x8000 + (6 << 10) + (6 << 5) + 6;
									else
										baddr[c2] = clut[c2];
								}
								else if ((tex->mAllAlpha) && clut[c2])
									baddr[c2] = clut[c2] | 0x8000;
								else
									baddr[c2] = clut[c2];
							}
							// write clut dump
							for (c2 = 0; c2 < 256 * 4; c2 ++)
							{
								mDump[tcy][tcx * 2 + c2] = clut[c2 >> 2];
							}

							tex->mClutX = (tcx / 2) + 512;
							tex->mClutY = tcy;

							// now repeat for greyscale
							GetNextClut8Bit(tcx, tcy);  //! Assume that I get one immediately above!!
							caddr = &mData[tcy][tcx];

							baddr = (UWORD *)caddr;
							for (c2 = 0; c2 < 256; c2 ++)
							{
								SINT	val = ((clut[c2] >> 10) & 31) + ((clut[c2] >> 5) & 31) + (clut[c2] & 31);
								val /= 3;
								clut[c2] = (val << 10) + (val << 5) + val;
								// copy and set high bit if necessary
								if (tex->mAllAlpha)
									baddr[c2] = clut[c2] | 0x8000;
								else
									baddr[c2] = clut[c2];
							}
							// write clut dump
							for (c2 = 0; c2 < 256 * 4; c2 ++)
							{
								mDump[tcy][tcx * 2 + c2] = clut[c2 >> 2];
							}

						}
						break;
#endif

					case BD_16:  // no clut...
						break;
					};

					// bugger.
					goto tex_placed;
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
	delete [] clut;
	delete [] dest_image;
	delete [] src_image;
}

//******************************************************************************************
void	CTextureAccumulator::CalculateFontSpecs(UWORD *src_image, STATexture *tex)
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

	SINT	c0, c1, c2, c3, mw, cw = tex->mW / CHARS_PER_FONT_LINE, nc = 0;
	
	for (c0 = 0; c0 < tex->mH / char_height; c0 ++)
	{
		for (c1 = 0; c1 < CHARS_PER_FONT_LINE; c1 ++)
		{
			// for each character, find the rightmost non-zero texel.
			mw = 0;

			for (c3 = 0; c3 < cw; c3 ++)
			{
				for (c2 = 0; c2 < char_height; c2 ++)
				{
					if (src_image[(((c0 * char_height) + c2) * tex->mW) + (c1 * cw) + c3] != 0x7c1f)
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
void	CTextureAccumulator::WriteDump(char *fname)
{
	BOOL	res = CTGALoader::Save16Bit(fname, (UWORD *)mDump, TA_SIZE_X * 2, TA_SIZE_Y);
}

//******************************************************************************************

//******************************************************************************************
STATexture *CTextureAccumulator::GetTextureRef(ETATextureType type, SINT num)
{
	SINT	c1;
	for (c1 = 0; c1 < mNumTextures; c1 ++)
	{
		STATexture *tex = &mTextures[c1];

		if ((tex->mType == type) && (tex->mNumber == num))
			return tex;
	}

	return NULL;
}

//******************************************************************************************
STATexture *CTextureAccumulator::GetTextureRef(ETATextureType type, CTexture *ct)
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
		STATexture *tex = &mTextures[c1];

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
SINT	CTextureAccumulator::GetTextureIndex(ETATextureType type, CTexture *ct)
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
		STATexture *tex = &mTextures[c1];

		if ((tex->mType == type) && (!(stricmp(tex->mName, ct->GetName()))))
			return mTextures[c1].mNumber;
	}

	return 0;
}

//******************************************************************************************
void	CTextureAccumulator::Write(CMEMBUFFER &mem)
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
		STATexture *tex = NULL;
		
		CTexture *mt = MAP.GetMapTexture(c0);
		char	*tname = MAP.GetMapTextureName(c0);
		if ((mt) && (tname[0]))
			tex = GetTextureRef(TATT_MAP, mt);

//		char	*mt = MAP.GetMapTextureName(c0);
//		if (mt[0])
//			tex = GetTextureRef(TATT_MAP, c0);

		if (!tex)
		{
			tex = GetTextureRef(TATT_MAP, 0);  // use default.
		}

		if(tex)
		{
			// here it is
			mem.Write(&tex->mX, 4);
			mem.Write(&tex->mY, 4);
			mem.Write(&tex->mW, 4);
			mem.Write(&tex->mH, 4);
			mem.Write(&tex->mTPageX, 4);
			mem.Write(&tex->mTPageY, 4);
			mem.Write(&tex->mClutX, 4);		 
			mem.Write(&tex->mClutY, 4);
		}
		else
		{
			// no texture - probably trying to write out the front end / level select stuff
			SINT	foo[10];
			mem.Write(&foo, 8 * 4);
		}
	}

	// now other textures
	mem.Write(&mNumOtherTextures, 4);		// write out number of textures

	for (c0 = 0; c0 < mNumOtherTextures; c0 ++)
	{
		// try to find the relevant texture
		STATexture *tex = GetTextureRef(TATT_OTHER, c0);

		if (!tex)
		{
			tex = GetTextureRef(TATT_OTHER, 0);  // use default.
		}
		
		ASSERT(tex);

		if (tex)
		{
			// here it is
			mem.Write(&tex->mX, 4);
			mem.Write(&tex->mY, 4);
			mem.Write(&tex->mW, 4);
			mem.Write(&tex->mH, 4);
			mem.Write(&tex->mTPageX, 4);
			mem.Write(&tex->mTPageY, 4);
			mem.Write(&tex->mClutX, 4);
			mem.Write(&tex->mClutY, 4);
			mem.Write(&tex->mDepth, 4);
			mem.Write(&tex->mAlphaType, 4);
		}
		else
		{
			SINT	foo[10];
			mem.Write(&foo, 10 * 4);
		}
	}

	// now numbered textures
	mem.Write(&mNumNumberedTextures, 4);		// write out number of textures

	for (c0 = 0; c0 < mNumNumberedTextures; c0 ++)
	{
		// try to find the relevant texture
		STATexture *tex = GetTextureRef(TATT_NUMBERED, c0);

		if (!tex)
		{
			tex = GetTextureRef(TATT_NUMBERED, 0);  // use default.
		}
		
		ASSERT(tex);

		if (tex)
		{
			// here it is
			mem.Write(&tex->mX, 4);
			mem.Write(&tex->mY, 4);
			mem.Write(&tex->mW, 4);
			mem.Write(&tex->mH, 4);
			mem.Write(&tex->mTPageX, 4);
			mem.Write(&tex->mTPageY, 4);
			mem.Write(&tex->mClutX, 4);
			mem.Write(&tex->mClutY, 4);
			mem.Write(&tex->mDepth, 4);
			mem.Write(&tex->mAlphaType, 4);
		}
		else
		{
			SINT	foo[10];
			mem.Write(&foo, 10 * 4);
		}
	}

	// now particle textures
	mem.Write(&mNumParticleTextures, 4);		// write out number of textures

	for (c0 = 0; c0 < mNumParticleTextures; c0 ++)
	{
		// try to find the relevant texture
		STATexture *tex = GetTextureRef(TATT_PARTICLE, c0);

		if (!tex)
		{
			tex = GetTextureRef(TATT_PARTICLE, 0);  // use default.
		}
		
		ASSERT(tex);

		if (tex)
		{
			// here it is
			mem.Write(&tex->mX, 4);
			mem.Write(&tex->mY, 4);
			mem.Write(&tex->mW, 4);
			mem.Write(&tex->mH, 4);
			mem.Write(&tex->mTPageX, 4);
			mem.Write(&tex->mTPageY, 4);
			mem.Write(&tex->mClutX, 4);
			mem.Write(&tex->mClutY, 4);
			mem.Write(&tex->mDepth, 4);
			mem.Write(&tex->mAlphaType, 4);
		}
		else
		{
			SINT	foo[10];
			mem.Write(&foo, 10 * 4);
		}
	}

	// write out font data
	STATexture *tex1 = GetTextureRef(TATT_FONT, 0);
	STATexture *tex2 = GetTextureRef(TATT_FONT, 1);
	STATexture *tex3 = GetTextureRef(TATT_FONT, 2);

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
		mem.Write(&tex1->mTPageX, 4);
		mem.Write(&tex1->mTPageY, 4);
		mem.Write(&tex1->mClutX, 4);
		mem.Write(&tex1->mClutY, 4);

		// and write out the font data
		mem.Write(&mFonts[0], sizeof(mFonts[0]));

		if (tex2)
		{
			// write out font position
			mem.Write(&tex2->mX, 4);
			mem.Write(&tex2->mY, 4);
			mem.Write(&tex2->mTPageX, 4);
			mem.Write(&tex2->mTPageY, 4);
			mem.Write(&tex2->mClutX, 4);
			mem.Write(&tex2->mClutY, 4);

			// and write out the font data
			mem.Write(&mFonts[1], sizeof(mFonts[1]));
		}
			if (tex3)
			{
				// write out font position
				mem.Write(&tex3->mX, 4);
				mem.Write(&tex3->mY, 4);
				mem.Write(&tex3->mTPageX, 4);
				mem.Write(&tex3->mTPageY, 4);
				mem.Write(&tex3->mClutX, 4);
				mem.Write(&tex3->mClutY, 4);

				// and write out the font data
				mem.Write(&mFonts[2], sizeof(mFonts[2]));
			}
	}

	// now, lump out the vram data.

	mem.Write(mData, TA_SIZE_Y * TA_SIZE_X);
}

#endif

