#ifndef TEXTURE_ACCUMULATOR_DC_H
#define TEXTURE_ACCUMULATOR_DC_H

#include	"MemBuffer.h"
#include	"Texture.h"
#include	"TextureCommon.h"

//#define	TADC_SIZE_X	256
//#define TADC_SIZE_Y	256

#define TADC_BLOCK_SIZE	8

//#define TADC_NUM_BLOCKS_X	(TADC_SIZE_X / TADC_BLOCK_SIZE)
//#define TADC_NUM_BLOCKS_Y	(TADC_SIZE_Y / TADC_BLOCK_SIZE)

#define TADC_MAX_TEXTURES		512
#define	TADC_MAX_PAGES			200

enum	ETADCTextureType
{
	TADCTT_MAP,
	TADCTT_MESH, 
	TADCTT_OTHER,
	TADCTT_FONT,
	TADCTT_NUMBERED,
	TADCTT_PARTICLE,
};

struct	STADCTexture
{
	char				mName[256];
	SINT				mPage;
	float				mX, mY;
	float				mW, mH;
	SINT				mWI, mHI;
	ETADCTextureType	mType;
	SINT				mAlphaType;
	SINT				mNumber;
	BOOL				mAllAlpha;
	BOOL				mAlphaBlack;
	BOOL				mSelfShadow;
	SINT				mDesiredPageSize;
	BOOL				mUseVQ;
	BOOL				mMipMap;
};

enum	ETADCSource
{
	TADCS_PC,
	TADCS_DC,
};

class	CTextureAccumulatorDC
{
public:
	void	Init()  {mNumTextures = 0; mNumOtherTextures = 0; mNumNumberedTextures = 0; mNumParticleTextures = 0;};

	void	AddTexture(char *name, ETADCTextureType type, SINT number, SINT sx = -1, SINT sy = -1, SINT alpha_type = 1, BOOL all_alpha = FALSE, BOOL alpha_black = TRUE, BOOL self_shadow = FALSE);
	void	AddTexture(CTexture *t, ETADCTextureType type, SINT number = -1, SINT sx = -1, SINT sy = -1, SINT alpha_type = 1, BOOL all_alpha = FALSE, BOOL alpha_black = TRUE,  BOOL self_shadow = FALSE);

	void	BuildTPage(BOOL	frontend = FALSE);

	void	Write(CMEMBUFFER &mem, BOOL frontend = FALSE);
	void	WriteDump(char *fname);
	void	WriteFontDats(char *base_name);

	STADCTexture *GetTextureRef(ETADCTextureType type, SINT num);
	STADCTexture *GetTextureRef(ETADCTextureType type, CTexture *ct);

	SINT	GetTextureIndex(ETADCTextureType type, CTexture *ct);
protected:
	BOOL	LoadTGA(char *fname, UWORD *data, BOOL &isalpha, SINT &sx, SINT &sy, ETADCSource source);
	BOOL	LoadAndScaleTGA(char *fname, UWORD *data, BOOL &isalpha, SINT sx, SINT sy, ETADCSource source);

	void	CalculateFontSpecs(UWORD *src_image, STADCTexture *tex);

	void	WriteVQPage(SINT n, CMEMBUFFER &mem);
	void	GetCachedVQOrWrite(SINT n, CMEMBUFFER &mem);

	SINT		mNumTextures;

	STADCTexture	mTextures[TADC_MAX_TEXTURES];

	UWORD	mData[TADC_MAX_PAGES][512][512];
	UWORD	mDump[TADC_MAX_PAGES][512][512];

	BOOL	mBlocksUsed[TADC_MAX_PAGES][512 / TADC_BLOCK_SIZE][512 / TADC_BLOCK_SIZE];
	BOOL	mPagesUsed[TADC_MAX_PAGES];
	SINT	mPageSize[TADC_MAX_PAGES];
	char	mPageTexture[TADC_MAX_PAGES][400];

	enum	EPageAlphaType
	{
		EPAT_UNKNOWN = 0,
		EPAT_565,
		EPAT_4444
	};

	EPageAlphaType	mIsPageAlpha[TADC_MAX_PAGES];

	enum	EPageVQType
	{
		EPVQT_UNKNOWN = 0,
		EPVQT_NO,
		EPVQT_YES
	};
	
	EPageVQType		mIsPageVQ[TADC_MAX_PAGES];

	enum	EPageMMType
	{
		EPMMT_UNKNOWN = 0,
		EPMMT_NO,
		EPMMT_YES
	};
	
	EPageMMType		mIsPageMipMapped[TADC_MAX_PAGES];


	SINT	mNumOtherTextures;
	SINT	mNumNumberedTextures;
	SINT	mNumParticleTextures;

	STCFont	mFonts[MAX_INGAME_FONTS];
};


#endif