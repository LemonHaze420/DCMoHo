#ifndef PCMAP_H
#define PCMAP_H

#include	"Map.h"
#include	"Texture.h"
#include	"vbuffer.h"

#include	"TextureAccumulator.h"
#include	"MeshAccumulator.h"
#include	"TextureAccumulatorDC.h"
#include	"MeshAccumulatorDC.h"

// mixing levels, this is now locked at 16 because of the compressed format!
#define	MIX_MIXERS		16
// shader levels
#define	MIX_SHADES		64

#define	EFFECT_SHADES	32

class	C4x4
{
	UWORD	mTextureMasks[3];
	UBYTE	mTexture[3];
	UBYTE	mFlags; // bits 0+1: when to turn additive on

public:
	void	SetMin(float x)					{ mMin = x;};
	float	mMax;
	float	mMin;

	void	SetTextureMask(int x, UWORD v)	{ mTextureMasks[x]=v; };
	UWORD	GetTextureMask(int x)			{ return mTextureMasks[x]; };
	void	SetTexture(int x, UBYTE v)		{ mTexture[x]=v; };
	UBYTE	GetTexture(int x)				{ return mTexture[x]; };

// 2 bit flag that tells you which pass to enable additive after
	void	SetAditiveAfter(UBYTE x) {mFlags = (mFlags & 0xfc)|x;};
	UBYTE	GetAditiveAfter() { return mFlags & 0x03;};
// there wil be more flags for stuff like 'pass A and B use the same texture'
};

struct	ExtraPass
{
	CFVF_UVRGB		Verts[5*5+4*4];
	CTextureGTEX*	Texture;
	UWORD*			TriList;
	UWORD			TriCount;
};
#define	NO_EXTRA_PASSES	500

class	CPCMap : public CMap
{
	// NO VIRTUAL FUNCTIONS!!!!!!!
public:
	CPCMap()
	{
		m4x4s = NULL;
	};
	BOOL		Init();
	BOOL		Resize(SINT xs, SINT ys);

	void		ProcessParticles();

	// Rendering
	void		DrawPickupBeam(CPlayer *pl);
	void		DrawLaserBeam(CPlayer *pl);
	void		DrawMapWhoDebug();
	void		Render();

	// Old shadow stuff
	void		CDSP(CTextureGTEX *tex, FVector p1, FVector p2, FVector p3, CFVF_GTEX *v1, CFVF_GTEX *v2, CFVF_GTEX *v3);
	void		ClipAndDrawShadowPoly(CTextureGTEX *tex, CFVF_GTEX *v1, CFVF_GTEX *v2, CFVF_GTEX *v3);
	// new shadow stuff - jcl 16-10-00
	void		CDSQ(CTextureGTEX *tex, const FVector &p1, const FVector &p2, const FVector &p3,
				 	 float sx, float sy, float ex, float ey);
	void		ClipAndDrawShadowQuad(CTextureGTEX *tex, float sx, float sy, float ex, float ey);
	void		DrawShadow(IRenderableThing *t, CTextureGTEX *tex);

	void		GCRender(class C3DGamut* gamut);
	void		GCRender22(class C3DGamut* gamut);

	void		PCRender(class C3DGamut* gamut);
	void		PCRender4x4(SINT x, SINT y, SINT mode = 0); // PCRender4x4 actually renders 4x3, 3x4 and 3x3 too. mode indicates which it is doing. See cpp.
	void		PCRender2x2(SINT x, SINT y);
	void		PCRender1x1(SINT x, SINT y);
	
	void		PCRender4x3(SINT x, SINT y);
	void		PCRender3x4(SINT x, SINT y);
	void		PCRender3x3(SINT x, SINT y);

	// texture stuff
	BOOL		SetCellTexture(CCellRef &c, char *tname);
	int			GetTexNo(char *tname);
	void		Set44Texture(int channel, CCellRef &c, int tnum);
	void		TweakVertex(int channel, CCellRef &c, int addthis);
	void		ClearChannel(int channel, CCellRef &c);
	BOOL		TentativeSetTex(int channel, int tex, const CCellRef &c);
	void		BrutalSetTex(int channel, int tex, CCellRef &c);
	void		Clear44Channel(int channel, const CCellRef &c);
	void		Clear44ChannelByXY(int channel, int x, int y);

	void		SetCellTextureDefault(CCellRef &c);
	void		ReconcileMapTextures();
	CTexture	*GetMapTexture(SINT no) {return mMapTextures[no];};
	char		*GetMapTextureName(SINT no) {return mMapTextureNames[no];};

	void		AddWaterParticles(SINT start_x, SINT start_y);


	// save
	BOOL		Read(CMEMBUFFER &mem);
	BOOL		Write(CMEMBUFFER &mem);
	BOOL		WritePSX(CMEMBUFFER &mem);
	BOOL		WriteDC(CMEMBUFFER &mem);

	void		PostLoadProcess();

	int			GetRandNum();

//	low resolution map stuff.
	C4x4*		m4x4s;

// including the pinmapper

	Pin		mPinMap[129*5]; // cos it's slightly bigger than 9*65 or 17*33
	int		mPinMapXSize;
	int		mPinMapYSize;
	int		mRandomNumbers[250];
	int		RandPos;

// Pinmapper
	void		LoadPinMap(int levelno);

#ifdef LINK_EDITOR
	void		AccumulatePSXResources(CTextureAccumulator &ta, CMeshAccumulator &ma);
	void		AccumulateDCResources(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma);
#endif


	void		InitPlatform(void);				// currently empty
	void		ShutdownPlatform(void);

	void		Create44Masks(const CCellRef &start, const CCellRef &end);
	void		SetupDefaultTexture();
private:
	void		CreateMixTable(void);
	void		CreateEffectTable(void);
	CTextureGTEX			*mTypeTextures[NUM_CELL_TYPES];
	CTextureGTEXAdditive	*mTexPickupBeam1, *mTexPickupBeam2;
	CTextureGTEXAdditive	*mTexLaserBeam;

	CTextureGTEX	*mMapTextures[MAX_MAP_TEXTURES];
	char			mMapTextureNames[MAX_MAP_TEXTURES][256];

// mixers, these are stored reversed so that the first table is 100%
// the second 90% and so on to 10% (given 10 mixers, in fact we will probably
// have 16) the idea is that 10% and 90% can safely be added without causing
// RGB to wrap into each other in D3D. The same should be true on PSX in 16 bit

	SINT		GetMixerShade(GINT a, GINT b);
	ULONG		mMixers[MIX_SHADES][MIX_MIXERS];
	ULONG		mEffects[EFFECT_SHADES][MIX_MIXERS];

	ExtraPass	mExtras[NO_EXTRA_PASSES];
};



#endif

