#ifndef TEXTURE_H
#define TEXTURE_H

#include	"TextureSurface.h"
#include	"VertexTypes.h"

enum	ETextureType
{
	TT_BASE = 0,
	TT_GTEX,
	TT_TL,
	TT_GTEX_ADD,
	TT_TLADD,
};

class	CTriIndex
{
public:
	WORD	mIndex[3];
};

#define	MAX_ANIM_FRAMES 35

class	CTexture
{
public:
	friend class CJCLEngine;
	friend class CPCFEPrisonSelect;
	friend class CPCGame;

	virtual	void	AddTri(SINT a, SINT b, SINT c);

	virtual	void	DrawAll();
	virtual	void	DrawAllAdditive() {};

	static	BOOL	InitAll();
	static	void	ReleaseAll();
	static	void	DrawAllTextures();
	static	void	DrawAllTexturesAdditive();

	char				*GetName() {return mName;};
	virtual	ETextureType GetType() {return TT_BASE;};
	SINT				 GetTextureID() {return mTextureID;}

	
	static	CTexture	*mFirstTexture;
	CTexture			*mNextTexture;

	virtual	BOOL	Init(char *fname);
	void	Shutdown();

	void			SetFrame(float frame);
	void			Activate();
protected:
	void			IncreaseVertBuffer(SINT size);
	void			IncreaseTriBuffer(SINT size);

	virtual	void	PutTris() = 0;
	
	static	BOOL	EvaluatePixelFormats();

	virtual	SINT	GetVertexSize() = 0;

	CTextureSurface	*mTexSurface[MAX_ANIM_FRAMES];

	// animation stuff;
	SINT			mNumAnimationFrames;
	float			mCurrentFrame;

	void		*mVerts;
	CTriIndex	*mTris;
	
	SINT		mNumVerts;
	SINT		mNumTris;

	SINT		mVertBuffSize;
	SINT		mTriBuffSize;

	SINT		mAlphaType;

	char		mName[200];

	
	static SINT mTextureCount;	// Class variable.
	SINT		mTextureID;		// A number (not unique) for aiding the identification of the texture - using to speed the sorting and thus rendering of the textures.

public:
	SINT		texw,texh;
protected:

	CTexture()
	{
		mTextureID = CTexture::mTextureCount % 100;
		CTexture::mTextureCount++;
		mVerts = NULL;
		mTris = NULL;

		mNumVerts = 0;
		mNumTris = 0;
		mVertBuffSize = 0;
		mTriBuffSize = 0;
		mNextTexture = NULL;
		mName[0] = 0;
	};

	virtual	~CTexture() = 0;

};

//******************************************************************************************
//******************************************************************************************

class	CTextureTL : public CTexture
{
public:
	SINT	AddVertex(D3DTLVERTEX *v);

	static	CTextureTL	*GetTexture(char *fname);
	virtual	ETextureType GetType() {return TT_TL;};

protected:
	virtual	SINT	GetVertexSize() {return sizeof(D3DTLVERTEX);};
	virtual	void	PutTris();

};

//******************************************************************************************
//******************************************************************************************
// Additive alpha texture - delayed drawing till end of frame
// has to batch up the transforms so we can remember what they were.

class	CTransform
{
public:
	FVector mPos;
	FMatrix mOrientation;
	SINT	mStartTri;
	SINT	mStartVert;
};
//******************************************************************************************
//******************************************************************************************
// this is where it gets fun

class	CTextureGTEX : public CTexture
{
public:
	SINT	AddVertex(CFVF_GTEX *v);

	static	CTextureGTEX	*GetTexture(char *fname);
	virtual	ETextureType GetType() {return TT_GTEX;};

protected:
	virtual	SINT	GetVertexSize() {return sizeof(CFVF_GTEX);};
	virtual	void	PutTris();
};

//******************************************************************************************
//******************************************************************************************
//! Maybe this should be derived from CTextureGTEX so the texture can decide what type it is and the
//  drawer doesn't know...

class	CTextureGTEXAdditive : public CTexture
{
public:
	~CTextureGTEXAdditive() {ShutdownTransforms();};

	virtual	BOOL	Init(char *fname);
	virtual void	ShutdownTransforms();
	
	SINT	AddVertex(CFVF_GTEX *v);
	virtual	void	AddTri(SINT a, SINT b, SINT c);
	virtual void	IncreaseTransformBuffer(SINT size);

	static	CTextureGTEXAdditive	*GetTexture(char *fname);
	virtual	ETextureType GetType() {return TT_GTEX_ADD;};


	virtual	void	DrawAll() {};
	virtual	void	DrawAllAdditive();

protected:
	virtual	SINT	GetVertexSize() {return sizeof(CFVF_GTEX);};
	virtual	void	PutTris() {};

	CTransform	*mTransforms;
	SINT		mNumTransforms;
	SINT		mTransformBuffSize;

	SINT		mLastEngineTransformTri;
	SINT		mLastEngineTransformVert;

	SINT		mFirstUntransformedVertex;
};

//******************************************************************************************
//******************************************************************************************

// this class does no inherent sorting.

class	CTextureTLAdditive : public CTexture
{
public:
	SINT	AddVertex(D3DTLVERTEX *v);

	static	CTextureTLAdditive	*GetTexture(char *fname);
	virtual	ETextureType GetType() {return TT_TLADD;};

	virtual	void	DrawAll() {};
	virtual	void	DrawAllAdditive();

protected:
	virtual	SINT	GetVertexSize() {return sizeof(D3DTLVERTEX);};
	virtual	void	PutTris();

};


#endif