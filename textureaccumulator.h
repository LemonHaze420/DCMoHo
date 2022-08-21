#ifndef TEXTURE_ACCUMULATOR_H
#define TEXTURE_ACCUMULATOR_H

#include	"MemBuffer.h"
#include	"Texture.h"
#include	"TextureCommon.h"

#define	TA_SIZE_X	1024
#define TA_SIZE_Y	512

#define TA_BLOCK_SIZE	8

#define TA_NUM_BLOCKS_X	(TA_SIZE_X / TA_BLOCK_SIZE)
#define TA_NUM_BLOCKS_Y	(TA_SIZE_Y / TA_BLOCK_SIZE)

#define TA_MAX_TEXTURES		512

enum	EBitDepth
{
	BD_4	= 0,
	BD_8	= 1,
	BD_16	= 2
};

enum	ETATextureType
{
	TATT_MAP,
	TATT_MESH, 
	TATT_OTHER,
	TATT_FONT,
	TATT_NUMBERED,
	TATT_PARTICLE,
};

struct	STATexture
{
	char			mName[256];
	SINT			mX, mY;
	SINT			mW, mH;
	SINT			mTPageX, mTPageY;
	SINT			mClutX, mClutY;
	EBitDepth		mDepth;
	ETATextureType	mType;
	SINT			mAlphaType;
	SINT			mNumber;
	BOOL			mAllAlpha;
	BOOL			mAlphaBlack;
	BOOL			mSelfShadow;
};

enum	ETASource
{
	TAS_PC,
	TAS_PSX,
};

class	CTextureAccumulator
{
public:
	void	Init()  {mNumTextures = 0; mNumOtherTextures = 0; mNumNumberedTextures = 0; mNumParticleTextures = 0;};

	void	AddTexture(char *name, ETATextureType type, SINT number, SINT sx = -1, SINT sy = -1, EBitDepth bd = BD_4, SINT alpha_type = 1, BOOL all_alpha = FALSE, BOOL alpha_black = TRUE, BOOL self_shadow = FALSE);
	void	AddTexture(CTexture *t, ETATextureType type, SINT number = -1, SINT sx = -1, SINT sy = -1, EBitDepth bd = BD_4, SINT alpha_type = 1, BOOL all_alpha = FALSE, BOOL alpha_black = TRUE,  BOOL self_shadow = FALSE);

	void	BuildTPage();

	void	Write(CMEMBUFFER &mem);
	void	WriteDump(char *fname);

	void	GetNextClut4Bit(SINT &x, SINT &y);
	void	GetNextClut8Bit(SINT &x, SINT &y);

	STATexture *GetTextureRef(ETATextureType type, SINT num);
	STATexture *GetTextureRef(ETATextureType type, CTexture *ct);

	SINT	GetTextureIndex(ETATextureType type, CTexture *ct);
protected:
	BOOL	LoadTGA(char *fname, UWORD *data, SINT &sx, SINT &sy, ETASource source);
	BOOL	LoadAndScaleTGA(char *fname, UWORD *data, SINT sx, SINT sy, ETASource source);

	void	CalculateFontSpecs(UWORD *src_image, STATexture *tex);

	SINT		mNumTextures;

	STATexture	mTextures[TA_MAX_TEXTURES];

	unsigned	char	mData[TA_SIZE_Y][TA_SIZE_X    ];
	WORD				mDump[TA_SIZE_Y][TA_SIZE_X * 2];

	BOOL	mBlocksUsed[TA_NUM_BLOCKS_Y][TA_NUM_BLOCKS_X];

	SINT	mClut4BlockX;
	SINT	mClut4BlockY;
	SINT	mClut4SubBlockY;

	SINT	mClut8BlockX;
	SINT	mClut8BlockY;
	SINT	mClut8SubBlockY;

	SINT	mNumOtherTextures;
	SINT	mNumNumberedTextures;
	SINT	mNumParticleTextures;

	STCFont	mFonts[MAX_INGAME_FONTS];
};

#endif