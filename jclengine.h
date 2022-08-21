// $Header$

// $Log$
// Revision 1.24  2000-10-17 16:24:32+01  are
// New method
//
// Revision 1.23  2000-10-11 16:00:15+01  are
// Camera interpolation.
//
// Revision 1.22  2000-10-05 13:45:31+01  are
// Water texture is now additive.
//
// Revision 1.21  2000-10-04 17:15:34+01  are
// Added 'DisableFogEffect' method definition.
//
// Revision 1.20  2000-10-04 14:35:55+01  asd
// added fog code
//
// Revision 1.19  2000-10-02 15:04:59+01  are
// <>
//
// Revision 1.18  2000-10-02 13:49:00+01  are
// Method 'RenderSkyCube' definition added.
//
// Revision 1.17  2000-09-28 13:55:31+01  jcl
// <>
//
// Revision 1.16  2000-09-27 11:39:13+01  jcl
// <>
//
// Revision 1.15  2000-09-27 09:05:41+01  jjs
// Added GetSysTimeFloat method.
//
// Revision 1.14  2000-09-25 10:22:16+01  jjs
// Further EnginePanel changes.
//
// Revision 1.13  2000-09-23 12:35:40+01  jjs
// More EnginePanel changes.
//
// Revision 1.12  2000-09-22 12:51:17+01  jjs
// Added mLevelType.
//
// Revision 1.11  2000-09-21 15:10:09+01  jjs
// More EnginePanel changes.
//
// Revision 1.10  2000-09-20 16:27:53+01  jjs
// More EnginePanel changes.
//
// Revision 1.9  2000-09-14 16:33:34+01  are
// <>
//
// Revision 1.8  2000-09-13 15:49:34+01  are
// <>
//
// Revision 1.7  2000-09-12 17:20:39+01  sds
// Big Merge II - all of jeremy's link editor stuff and andrews TARGET == DC stuff
//
// Revision 1.6  2000-09-11 14:52:33+01  are
// Water is now an alpha based texture.
//
// Revision 1.5  2000-08-31 11:53:40+01  are
// Method definition. DrawShadowedString is designed for use when the caller wishes to draw a string with a drop shadow. The user can not specify the shadow offset, however, the colour of the two strings can be specified. The shadow offset is implmentation dependent.
//
// Revision 1.4  2000-08-05 10:33:03+01  jjs
// First PC milestone.
//
// Revision 1.3  2000-07-11 10:20:19+01  jjs
// DirectX7 version.
//
// Revision 1.2  2000-07-11 10:19:42+01  jjs
// Added Dreamcast changes.
//

#ifndef JCLENGINE_H
#define JCLENGINE_H
#if TARGET == PC
#include	"Camera.h"
#include	"TextureSurface.h"
#include	"Texture.h"
#include	"CollisionVolume.h"
#include	"Gamut.h"
#include	"Mesh.h"

#include	"TextureAccumulator.h"

#define PCMULTX	(1.25f)
#define PCMULTY (1.875f)
	
enum EGamutType
{
	GAMUT_NORMAL,
	GAMUT_LOWERED,
	GAMUT_NONE
};

struct	SPCFont
{
	STCFont	f;
	SINT	mX, mY;
	CTextureTL *font;
};

class CPlayer ;

//******************************************************************************************
class CJCLEngine
{
public:
	CJCLEngine() {};
	~CJCLEngine() {};

	BOOL	Init();
	void	Shutdown();

	void	Reset();

	BOOL	LoadLevelTextures();

	void	ChangeTexture(CTextureSurface *t);

	void	SetViewMatrixFromCamera(D3DMATRIX &matView);
	void	FMatToD3DMat(FMatrix *f, D3DMATRIX *d);
	void	SetupInitialRenderstates();
	void	SetupTransforms(FVector pos, FMatrix ori);

	void	Render2D();
	void	RenderPlayers2D(CPlayer* for_player, SINT left_pixels_offset, SINT screen_width  );

	void	RenderSky();
	void	RenderSkyCube();
	void	RenderWater();
	void	RenderCursor();
	void	RenderGameState();
	void	RenderPlayerState(CPlayer* for_player, SINT left_pixels_offset, SINT screen_width  ) ;
	void	RenderThings();
	void	RenderStadium();
	void	Render(CCamera *cam1, CCamera *cam2);
	void	Flip();
	
	void	DrawSpritePC(CTextureTL* n, float px, float py, float z, float width, float height, D3DCOLOR col=0xffffffff, float scalex=1.0f, float scaley=1.0f, SINT flip = 0);
	void	DrawSpritePC2(CTextureTL* n, float px, float py, float z, float width, float height, D3DCOLOR col=0xffffffff, float scalex=1.0f, float scaley=1.0f, SINT flip = 0);
	void	DrawBar(float x, float y, SINT num_bits, SINT colr, SINT colg, SINT colb, BOOL centre = FALSE, SINT ot = 2);

	// font stuff - code in PSXEnginePanel.cpp
	SINT	DrawChar(SINT font, UINT c, float x, float y, DWORD col, float z = 0.11f);  // returns width of char
	void	DrawString(SINT font, char *s, float x, float y, DWORD col, float z = 0.11f,BOOL HasBorder=FALSE);
	void	DrawShadowedString(SINT font, char *s, float x, float y, DWORD col, float z = 0.11f,BOOL HasBorder=FALSE);
	SINT	GetStringSize(SINT font, char *s);

	SINT			mNumFonts;
	SPCFont		mFonts[MAX_INGAME_FONTS];

	void	AddToPolyCount(SINT i) {mPolyCount += i;};

	DWORD	CalcLight(FVector v);

	GMatrix GetCursorOffsetMatrix(SINT x, SINT y);

	CCamera				*GetCamera()  {return mCamera; };
	void		SetCamera(CCamera *cam) {mCamera = cam;};
	LPDIRECT3DDEVICE7	 GetDevice()  {return mDevice; };

	CTextureGTEX		*GetSkyTexture() {return mSkyTexture;};

	// renderstate controllers for renderthings
	BOOL	AddFogEffect(DWORD dwColour, DWORD dwMode,  float fStart, float fEnd, float fDensity, BOOL fUseRange);
	void	DisableFogEffect();

	void	EnableAdditiveAlpha();
	void	EnableMultiplicativeAlpha();
	void	DisableAlpha();
	void	EnableUWrapping();
	void	DisableUWrapping();


	void	DrawIconSprite(CTextureTL *t, float x, float y, SINT number, DWORD col);
	void	DrawNumber(float x, float y, SINT number, DWORD col = 0xffffffff);
	void	DrawIcon(float x, float y, SINT number, DWORD col = 0xffffffff);

	void	DrawSplashScreen(SINT t);
	void	DrawLoadingScreen();

	float	GetSysTimeFloat(void);

	void	DrawCollisionSphere(float radius, SINT hit_flags);
	void	DrawCollisionVolume(CCollisionVolume *cv, SINT hit_flags);
	void	DrawDebugCuboid(DWORD col, FVector &axes, FVector &pos, FMatrix ori = ID_FMATRIX);
	void	RenderArrow(FVector v1, FVector v2, DWORD col = 0xffffffff);

	SINT	GetTransformCount() {return mTransformCount;};
	bool	auto_shadow(CTextureTL *tex);
	void	GetTransforms(FVector *v, FMatrix *o) 
	{
		*v = mTransformPosition;
		*o = mTransformOrientation;
	};

	SINT	GetRenderGameTurn();

	CGamut	*GetGamut() {return &mGamut;};
	EGamutType	GetGamutType() {return mGamutType;};

	void	ChangeGamut();
// evil, nasty shit code warning
	void	ChangeMapDrawMode() { mAltMapDraw++;if(mAltMapDraw>2) mAltMapDraw=0;};
	int		GetMapDrawMode() { return mAltMapDraw;};

#ifdef LINK_EDITOR
	void		AccumulatePSXResources(CTextureAccumulator &ta, CMeshAccumulator &ma);
	void		AccumulateDCResources(CTextureAccumulatorDC &ta, CMeshAccumulatorDC &ma);
#endif

	// default stuff
	CMesh			*GetDefaultMesh()			{return mDefaultMesh;};
	CTextureGTEX	*GetDefaultTextureGTEX()	{return mDefaultTextureGTEX;};


	void	PrepareLevel();

protected:
	CTextureTL		*mTexCursor;
	CTextureTL		*mTexLevelComplete;
	CTextureTL		*mTexDied;
	CTextureTL		*mTexGolfShot;

	CTextureTL		*mTexNumbers;
	CTextureTL		*mTexIcons;

	CTextureGTEX	*mSkyTexture;
	CTextureGTEXAdditive	*mTexWater;

	CTextureGTEX*	mTexSkyCentre;
	CTextureGTEX*	mTexSkyUp;
	CTextureGTEX*	mTexSkyDown;
	CTextureGTEX*	mTexSkyLeft;
	CTextureGTEX*	mTexSkyRight;

	CTextureTL		*mTexSplash;
	CTextureTL		*mTexLoading;

	CTextureGTEX	*mDefaultTextureGTEX;

	// bar stuff
	CTextureTL		*mTexBarLeft;
	CTextureTL		*mTexBarMiddle;
	CTextureTL		*mTexBarRight;

	// panel stuff
	CTextureTL			*mTexPanelBar;
	CTextureTL			*mTexPanelBar2;
	CTextureTLAdditive	*mTexPanelBackground;

	CTextureTL			*mTexStart1;
	CTextureTL			*mTexStart2;
	CTextureTL			*mTexStart3;

	CTextureTL			*mTexHealth0;
	CTextureTL			*mTexHealth1;
	CTextureTL			*mTexHealth2;
	CTextureTL			*mTexHealth3;

	CTextureTL			*mClock;
	CTextureTL			*mPanel;
	CTextureTL			*mLevelType[8];
	CTextureTL			*mStartArm;

#ifdef LINK_EDITOR
	CTextureGTEXAdditive	*mTexEdArrow;
#endif

	// debug
	CTextureGTEXAdditive	*mTexOutline;
	int					mAltMapDraw;
	// scene globals
	LPDIRECT3DDEVICE7 mDevice;

	CCamera	*mCamera;
	CCamera *mViewportCameras[2];

	// debug reporting
	SINT	mFrameCount;
	SINT	mTickCount;
	SINT	mFPS;
	SINT	mPolyCount;

	LARGE_INTEGER	frequency;
// stored transforms
	FVector	mTransformPosition;
	FMatrix mTransformOrientation;
	SINT	mTransformCount;		// for checking if the transforms have changed

	CTextureSurface		*mLastTexture;

	CGamut		mGamut;
	EGamutType	mGamutType;

	class		C3DGamut*	m3DGamut;

	// global mesh stuff
#define	MAX_GLOBAL_MESHES 256
#define	MAX_NUMBERED_MESHES 256

	CMesh		*mDefaultMesh;

	CMesh		*mGlobalMeshes[MAX_GLOBAL_MESHES];
	SINT		mGMRelocates[MAX_GLOBAL_MESHES];
	SINT		mNumGlobalMeshes;
	SINT		mNumRelocatedMeshes;

	CMesh		*mNumberedMeshes[MAX_NUMBERED_MESHES];

public:
	SINT		GetGlobalMeshNumber(char *name);
	CMesh		*GetGlobalMesh(SINT num) {ASSERT((num >= 0) && (num < mNumGlobalMeshes)); return mGlobalMeshes[num];};
	
	void		ClearRelocatedMeshes()		{mNumRelocatedMeshes = 0;};
	void		AddRelocatedMesh(SINT num);
	SINT		GetRelocatedMesh(SINT num);

	BOOL		Read(CMEMBUFFER &mem);
	BOOL		Write(CMEMBUFFER &mem);

#ifdef LINK_EDITOR
	void		WritePSXMeshReferences(CMEMBUFFER &mem, CMeshAccumulator &ma);
	void		WriteDCMeshReferences(CMEMBUFFER &mem, CMeshAccumulatorDC &ma);
#endif

	SINT		mNumDisplays;

	SINT		RenderingView() {return mRenderingView;}
	SINT		mRenderingView;
};

//******************************************************************************************
#endif
#endif
