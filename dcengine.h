#ifndef DCENGINE_H
#define DCENGINE_H

#include	"Camera.h"
#include	"CollisionVolume.h"

#include	"Map.h"
#include	"MemBuffer.h"
#include	"DCMesh.h"
#include	"TextureCommon.h"

// scaling parameters to make life simpler.....
#define 	DCMULTX	(1.25f)
#define 	DCMULTY (1.875f)


#define		MAX_PSX_MESHES	256
#define		MAX_OTHER_TEXTURES		256
#define		MAX_NUMBERED_TEXTURES	256
#define		MAX_PARTICLE_TEXTURES	64

//extern __declspec(scratchpad) int OffScreenMask;
extern __declspec(text) int GouraudDepth;
extern __declspec(text) int OffScreenMask;


// These are setup into the scratchpad at the start of the render loop....
#if defined(ORIGVERS)
extern __declspec(scratchpad)	int scratchpadwh;
extern __declspec(scratchpad)	CCamera *SPCamera;
extern __declspec(scratchpad)	MATRIX  SPCameraMATRIX;
extern __declspec(scratchpad)	GMatrix SPCameraGMatrix;
extern __declspec(scratchpad)	SINT	CameraAlreadyLoaded;
#else
extern __declspec(text)	int scratchpadwh;
extern __declspec(text)	CCamera *SPCamera;
//extern __declspec(text)	MATRIX  SPCameraMATRIX;
extern __declspec(text)	GMatrix SPCameraGMatrix;
extern __declspec(text)	SINT	CameraAlreadyLoaded;
#endif
// x,y,z are GINT.mVals ....... (pass Gvextor.X.mVal etc...)
extern void CommonOffsetCoord(int x,int y,int z);
#define CommonSetTrans gte_MACtoTRANS
/******************************************************************************************/

// CHANGED FOR NEW FORMAT
struct	STextureRefMap
{
	//UWORD	mX, mY;
	//UWORD	mW, mH;
    //UWORD   mCLUT;
    //UWORD   mTPAGE;
   
   	float	mX, mY;
	float	mW, mH;
    SINT   mTPage;
    
    
};

struct	STextureRefOther
{
	/*
	UWORD	mX, mY;
	UWORD	mW, mH;
    UWORD   mCLUT;
    UWORD   mTPAGE_P1P1;                // Default mode 1+1
    UWORD   mTPAGE_P05P05;              // 0.5 + 0.5
    UWORD   mTPAGE_P1M1;                // 1 - 1
    UWORD   mTPAGE_P1P025;              // 1 + 0.25
    */
    
   	float	mX, mY;
	float	mW, mH;
    SINT   mTPage;    
};

struct	SDCFont
{
	STCFont	f;
	float	mX, mY;
	SINT	mTPage;
};

class CDCEngine 
{
public:

	CDCEngine() {};
	~CDCEngine() {};

	BOOL	Init()
	{
		mNumMeshes = 0;
		mNumRenderData = 0;
		return TRUE;
	};
	void	Shutdown() {};

	void	Render(CCamera *cam , CCamera *cam2);
	//void	Flip();

	SINT	GetRenderGameTurn();

	void	ChangeGamut()	{};
	void	ToggleMapDraw() {};
	void	ChangeMapDrawMode() {};

	CCamera		*GetCamera()  {return mCurrentCamera; };
	void		SetCamera(CCamera *cam) {mCurrentCamera = cam;};

//	void	GMatrixToMATRIX(const GMatrix *gmat, MATRIX *mat);
//  void    GMatrixToTransposeMATRIX(const GMatrix *gmat, MATRIX *mat);

	void	GMatrixToGTE(const GMatrix *gmat);
	void	GMatrixTransposeToGTE(const GMatrix *gte);

	void	ReadPSXTextures(CMEMBUFFER &mem);
	void	ReadPSXMeshes(CMEMBUFFER &mem);
	void	ReadPSXRTInfo(CMEMBUFFER &mem);

	STextureRefMap		*GetMapTexture(SINT no) {return &mMapTextures[no];};
	STextureRefOther	*GetOtherTexture(SINT no) {return &mOtherTextures[no];};
	STextureRefOther	*GetNumberedTexture(SINT no) {return &mNumberedTextures[no];};
	STextureRefOther	*GetParticleTexture(SINT no) {return &mParticleTextures[no];};

	CDCMesh		*GetMesh(SINT no) {return &mMeshes[no];};
	SINT			GetNumMeshes()	{return mNumMeshes;};

	void	DrawSprite(SINT n, float px, float py, SINT sx, SINT sy, float otdepth, SINT r = 127, SINT g = 127, SINT b = 127, SINT semitrans = -1, BOOL grey = FALSE, GINT scale = G1, SINT flip = 0);
	void	DrawBar(float x, float y, SINT num_bits, SINT colr, SINT colg, SINT colb, BOOL centre = FALSE, float ot = 2.f);
	// font stuff - code in PSXEnginePanel.cpp
	SINT	DrawChar(SINT font, UINT c, float x, float y, DWORD col, float otdepth = 1);  // returns width of char
	void	DrawString(SINT font, char *s, float x, float y, DWORD col, float otdepth = 1,BOOL HasBorder=FALSE);
	SINT	GetStringSize(SINT font, char *s);

	CDCMesh	*GetDefaultMesh()			{return &mMeshes[mDefaultMesh];};
	CDCMesh	*GetGlobalMesh(SINT n)		{return &mMeshes[mGlobalMeshes[n]];};
	CDCMesh	*GetNumberedMesh(SINT n)	{return &mMeshes[mNumberedMeshes[n]];};

	GVector	GetEmitter(CThing *t, SINT no);
	GVector	GetEmitterAnimated(CThing *t, SINT no);
	GINT	GetFrameIncrement(SINT id, EAnimMode a, SINT* realindex);
	CCollisionVolume 	*GetCollisionVolume(CThing *t);
	GINT	GetRenderThingRadius(CThing *t);

	SINT	GetNumFonts() {return mNumFonts;};

	void	SwapYZ(FVector *v, SINT num);	// (JCL) sniff....

protected:
	void	RenderThings();
	void	RenderSkyCube();

	void	RenderPlayerState(CPlayer *for_player, SINT left_pixels_offset, SINT screen_width);
	void	RenderPanel(); //  code in PSXEnginePanel.cpp

	CCamera	*mCurrentCamera;
	CCamera *mViewportCameras[2];
	// debug reporting
	SINT	mFrameCount;
	SINT	mTickCount;
	SINT	mFPS;
	SINT	mPolyCount;

	// texture references
	STextureRefMap		mMapTextures[MAX_MAP_TEXTURES];
	STextureRefOther	mOtherTextures[MAX_OTHER_TEXTURES];
	STextureRefOther	mNumberedTextures[MAX_NUMBERED_TEXTURES];
	STextureRefOther	mParticleTextures[MAX_PARTICLE_TEXTURES];
	
	// fonts
	SINT			mNumFonts;
	SDCFont		mFonts[MAX_INGAME_FONTS];

	// meshes
#define	MAX_GLOBAL_MESHES 	256
#define	MAX_NUMBERED_MESHES	128

	CDCMesh		mMeshes[MAX_PSX_MESHES];
	SINT			mNumMeshes;

	SINT			mDefaultMesh;						// references to mMeshes
	SINT			mGlobalMeshes[MAX_GLOBAL_MESHES];   // for global meshes
	SINT			mNumberedMeshes[MAX_NUMBERED_MESHES];
	SWORD			mNumGlobalMeshes;
	SWORD			mNumNumberedMeshes;

	// thing references
	SWORD			mThingRef[MAX_OIDS];
	SDCRenderData	*mRenderData;
	SINT			mNumRenderData;

	// panel and fades stuff
	// referenced in PSXEnginePanel.cpp
#define FADE_X	16
#define FADE_Y	16

	UBYTE	mFadeData[FADE_Y][FADE_X];
};

//******************************************************************************************
#endif























































