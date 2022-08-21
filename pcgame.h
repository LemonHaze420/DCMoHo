#ifndef PCGAME_H
#define PCGAME_H

#include	"Game.h"
#include	"MemBuffer.h"

#include	"TextureAccumulator.h"
#include	"MeshAccumulator.h"
#include	"ThingAccumulator.h"

#include	"TextureAccumulatorDC.h"
#include	"MeshAccumulatorDC.h"
#include	"ThingAccumulatorDC.h"

class	CPCGame : public CGame
{
public:
	void		ShowSplashScreen();

	EGQuitType	RunLevel(SINT levelno);
	SINT		Run();

	void		ShowLoadingScreenWhilstWaitingForUserToPressSpace(SINT n);
	void		ShowLoadingScreen(	SINT n, 
									bool ShowLoading	= true, 
									bool ShowPress		= false);

	void		TakeScreenShot() {};

	BOOL		LoadLevel(SINT levelno);
	void		RestartLevel();
	BOOL		SaveLevel(SINT levelno = -1);

	CMesh		*GetNumberedMesh(SINT n);
	SINT		GetNumNumberedMeshes() {return NM_NUMBER;};

	GINT        GetFrameRenderFraction(void);

	bool		mInFrontEnd;
	float		mFrameTime, mBaseTime;

#ifdef LINK_EDITOR
	BOOL		SavePSXLevel(SINT levelno = -1);

	BOOL		SavePSXFrontEndData();

	BOOL		SavePSXPrisonSelectData();

	BOOL		SaveDCLevel(SINT levelno = -1);
	BOOL		SaveDCFrontEndData();
	BOOL		SaveDCPrisonSelectData(SINT n);
	BOOL		SaveDCScreen(SINT n);

protected:

	CTextureAccumulator	mTextureAccumulator;
	CMeshAccumulator	mMeshAccumulator;
	CThingAccumulator	mThingAccumulator;

	CTextureAccumulatorDC	mTextureAccumulatorDC;
	CMeshAccumulatorDC		mMeshAccumulatorDC;
	CThingAccumulatorDC		mThingAccumulatorDC;

	void		RelocateGlobalMeshes();

	void		AddPSXGlobalTextures();
	void		AddPSXNumberedMeshes();
	void		AddDCGlobalTextures();
	void		AddDCNumberedMeshes();

	void		WritePCFontDats();

public:
	void		WritePSXNumberedMeshReferences(CMEMBUFFER &mem, CMeshAccumulator &ma);
	void		WriteDCNumberedMeshReferences(CMEMBUFFER &mem, CMeshAccumulatorDC &ma);

#endif

};


#endif