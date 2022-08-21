// $Header$

// $Log$
// Revision 1.22  2000-10-10 12:20:18+01  are
// Removed all code for sorting. Transformations for GTEX based triangles are no longer stored.
//
// Revision 1.21  2000-10-06 16:02:37+01  asd
// took out flag for vertex lighting
//
// Revision 1.20  2000-10-05 15:07:44+01  asd
// added flag to processverticies to add lighting and fix the textures on my screen
//
// Revision 1.19  2000-10-03 10:42:21+01  jjs
// Fixes transform memory leak.
//
// Revision 1.18  2000-10-02 15:26:11+01  jjs
// Removes LINK_EDITOR if NO_LINK_EDITOR defined in settings.
//
// Revision 1.17  2000-09-29 11:20:54+01  jcl
// <>
//
// Revision 1.16  2000-09-28 13:55:40+01  jcl
// <>
//
// Revision 1.15  2000-09-19 10:01:56+01  jcl
// <>
//
// Revision 1.14  2000-09-15 11:03:15+01  are
// Removed sorting of GTEXAdditive vertices.
//
// Revision 1.13  2000-09-14 16:57:54+01  are
// <>
//
// Revision 1.12  2000-09-14 15:36:17+01  are
// <>
//
// Revision 1.11  2000-09-14 10:37:44+01  are
// <>
//
// Revision 1.10  2000-09-11 15:08:04+01  are
// Faster sorting of alphas. Converts from GTEX to TL vertices. (WORKS!)
//
// Revision 1.9  2000-09-07 17:07:27+01  are
// Attempt to sort GTEX verticies. (Not there yet).
//
// Revision 1.8  2000-09-04 17:32:55+01  are
// Better sorting of triangles
//
// Revision 1.7  2000-09-01 17:14:55+01  are
// Bit of optimisation. (more to follow)
//
// Revision 1.6  2000-08-31 11:48:34+01  are
// DrawAllTexture method now sorts all triangles for all textures, sorting them into decending or of z-depth (a sum of the three vertex z values is used as a sort key).
//
// Revision 1.5  2000-08-23 11:58:55+01  are
// Changes the program title to reflect the tga that is loading. This is for debug, and can be removed later.
//
// Revision 1.4  2000-08-11 17:26:36+01  are
// Detects and restores surfaces / data.
//
// Revision 1.3  2000-08-05 10:33:40+01  jjs
// First PC milestone.
//
// Revision 1.2  2000-07-11 10:43:52+01  jjs
// DirectX7 and first Dreamcast version.
//

#include	"Common.h"

#if TARGET == PC 
#include	"Texture.h"
#include	"Globals.h"
#include	"RawLoader.h"
#include	"TGALoader.h"
#include	"d3derr.h"
#include	"Profile.h"

//******************************************************************************************
CTexture	*CTexture::mFirstTexture	= NULL;
SINT		CTexture::mTextureCount		= 0;

//******************************************************************************************
CTexture::~CTexture()
{
	Shutdown();
}

//******************************************************************************************
//** texture formal evaluation

static DDPIXELFORMAT pixel_format_rgb ;
static DDPIXELFORMAT pixel_format_rgba;

//******************************************************************************************
static int countbits(unsigned int a)
{
	int c = 0;
	while (a)
	{
		if (a & 1) c++;
		a >>= 1;
	}
	return c;
}

//******************************************************************************************
static HRESULT CALLBACK etfcb(DDPIXELFORMAT *pf, VOID *)
{
	int c = countbits(pf->dwRBitMask) +
			countbits(pf->dwGBitMask) +
			countbits(pf->dwBBitMask) +
			countbits(pf->dwRGBAlphaBitMask);

	char	str[200];
	sprintf(str, "bpp: %d (%d %d %d %d)\n", c, countbits(pf->dwRBitMask), countbits(pf->dwGBitMask), countbits(pf->dwBBitMask), countbits(pf->dwRGBAlphaBitMask));
	TRACE(str);

	if (c != 16) return DDENUMRET_OK; // I want a 16 bit pixel format.

	// now, load it into either of the two pixel formats if it's a better choice than the one we've got so far
	if (pixel_format_rgb .dwSize == 0) pixel_format_rgb  = *pf;
	else
	if (countbits(pf->dwRGBAlphaBitMask) < countbits(pixel_format_rgb .dwRGBAlphaBitMask)) pixel_format_rgb  = *pf;

	if (pixel_format_rgba.dwSize == 0) pixel_format_rgba = *pf;
	else
	if (countbits(pf->dwRGBAlphaBitMask) > countbits(pixel_format_rgba.dwRGBAlphaBitMask) &&
		countbits(pf->dwRGBAlphaBitMask) <= 4) pixel_format_rgba = *pf;

	return DDENUMRET_OK;
}

//******************************************************************************************
BOOL CTexture::EvaluatePixelFormats()
{
	// mark the two pixel formats we want to fill in as uninitialised.
	pixel_format_rgba.dwSize = 0;
	pixel_format_rgb .dwSize = 0;

	// then ask the enumeration function to have a go at filling them in.
	LPDIRECT3DDEVICE7  device = PLATFORM.GetD3DDevice();
	
	ASSERT(device);

	RET_ERR(device->EnumTextureFormats(etfcb, NULL));

	// and return whether it's found any or not.
	if (pixel_format_rgba.dwSize != 0 && pixel_format_rgb.dwSize != 0) return TRUE ;
	else															   return FALSE;
}

//******************************************************************************************
BOOL	CTexture::InitAll()
{
	ASSERT(mFirstTexture == NULL);

	// enumerate texture formats
	BOOL	retval = EvaluatePixelFormats();
	ASSERT(retval);

	return TRUE;
}

//******************************************************************************************
BOOL	CTexture::Init(char *fname)
{
	char	name[256] = "data\\Textures\\";

	ASSERT(fname[0]);
	strcat(name, fname);

	_strlwr(name);
	
	DDPIXELFORMAT	*p  = &pixel_format_rgb;
	mAlphaType = 0;

	// try to predetermine alpha type
	if (name[strlen(name) - 6] == '_')
	{
		switch (name[strlen(name) - 5])
		{
		case '1':
			p = &pixel_format_rgba;
			mAlphaType = 1;
			break;

		case '2':
			p = &pixel_format_rgba;
			mAlphaType = 2;
			break;

		case '3':
			p = &pixel_format_rgba;
			mAlphaType = 3;
			break;

		default:
//			ASSERT(0);
			break;
		}
	}

	// sort out animation stuff
	//! should be able to have both..

	BOOL	r = TRUE;
	SINT	l = strlen(name);
	if ((l > 8) && (name[l -  8] == '0') && (name[l - 7] == '0') && (name[l - 6] == '0') && (name[l - 5] == '1'))
	{
		// aha!

		// load the frames...

		SINT	c0 = 0;
		BOOL	t = TRUE;

		while (TRUE)
		{
			mTexSurface[c0]	= new CTextureSurface;
			ASSERT(mTexSurface[c0]);

			sprintf(&(name[l - 8]), "%04d", c0 + 1);
			name[l - 4] = '.';	// repair the '.' !

			r = (mTexSurface[c0]->Create(PLATFORM.GetDPWin()->GetDirectDraw7(),
				 &IID_IDirect3DHALDevice,
				 CRawLoader(name, mAlphaType),
				 &pixel_format_rgb, &pixel_format_rgba,
				 &texw,&texh) == DD_OK);
			if (!r)
				break;
			c0 ++;
			if (c0 == MAX_ANIM_FRAMES)
				break;
		}

		delete mTexSurface[c0]; // last one didn't load
		mNumAnimationFrames = c0;
		r = (c0 != 0); // did we get at least one frame?
	}
	else
	{
		mTexSurface[0]	= new CTextureSurface;
		ASSERT(mTexSurface[0]);

		// what filetype do we have?

		if ((name[l-4] == '.') && (name[l-3] == 't') && (name[l-2] == 'g') && (name[l-1] == 'a'))
		{
			// .tga! 
			r = (mTexSurface[0]->Create(PLATFORM.GetDPWin()->GetDirectDraw7(),
				 &IID_IDirect3DHALDevice,
				 CTGALoader(name, &mAlphaType),  // note - loader fills in alphatype...
				 &pixel_format_rgb, &pixel_format_rgba,
				 &texw,&texh
				 ) == DD_OK);
		}
		else
		{
			// try the .raw loader

			r = (mTexSurface[0]->Create(PLATFORM.GetDPWin()->GetDirectDraw7(),
				 &IID_IDirect3DHALDevice,
				 CRawLoader(name, mAlphaType),
				 &pixel_format_rgb, &pixel_format_rgba,
				 &texw,&texh
				 ) == DD_OK);
		}

		if (!r)
		{
//			MessageBox(NULL, "Unable to load required texture.\nPlease try running the program again.\nIf the problem persists then reinstall the game.", GAME_NAME, MB_OK);
			return r;
		}
//		ASSERT(r);

		mNumAnimationFrames = 1;
	}

	strcpy(mName, fname);
	mCurrentFrame = 0;

	return r;
}

//******************************************************************************************
void	CTexture::Shutdown()
{
	if (mVerts)
	{
		delete [] mVerts;
		mVerts = NULL;
	}

	if (mTris)
	{
		delete [] mTris;
		mTris = NULL;
	}

	SINT c0;
	
	for (c0 = 0; c0 < mNumAnimationFrames; c0++)
	{
		mTexSurface[c0]->Destroy();
		delete mTexSurface[c0];
		mTexSurface[c0] = NULL;
	}
}

//******************************************************************************************
void	CTexture::ReleaseAll()
{
	CTexture *t = mFirstTexture;

	while (t)
	{
		CTexture *s = t->mNextTexture;
		delete t;
		t = s;
	}
	mFirstTexture = NULL;
}

//******************************************************************************************
void	CTexture::SetFrame(float frame)
{
	ASSERT(SINT(frame) < mNumAnimationFrames);

	if (SINT(frame) >= mNumAnimationFrames)
		frame = float(mNumAnimationFrames - 1);

	if (mCurrentFrame != frame)
		DrawAll();

	mCurrentFrame = frame;
}
//******************************************************************************************
void	CTexture::DrawAllTextures()
{
	PROFILE_FN(RenderTextures);

	// Local variables.
	CTexture				*t;
	
	// ----
	// Render non alphas
	ENGINE.DisableAlpha();
	t = mFirstTexture;
	while (t)
	{
		t->DrawAll();
		t = t->mNextTexture;
	}
}

//******************************************************************************************
void	CTexture::DrawAllTexturesAdditive()
{
	PROFILE_FN(RenderTextures);
	CTexture *t = mFirstTexture;
	
	while (t)
	{
		t->DrawAllAdditive();
		t = t->mNextTexture;
	}
}

//******************************************************************************************
void	CTexture::AddTri(SINT a, SINT b, SINT c)
{
	if (mNumTris == mTriBuffSize)
	{
		// need a larger buffer
		IncreaseTriBuffer(mTriBuffSize * 2);
	}

	mTris[mNumTris].mIndex[0] = WORD(a);
	mTris[mNumTris].mIndex[1] = WORD(b);
	mTris[mNumTris].mIndex[2] = WORD(c);

	mNumTris++;
}

//******************************************************************************************
void	CTexture::DrawAll()
{
	if (mNumTris == 0)
		return; // nothing to do!

	// Hopefully this should restore textures if they are lost.
	CTextureSurface*		workingTextureFrame = this->mTexSurface[SINT(mCurrentFrame)];
	LPDIRECTDRAWSURFACE7	workingSurface;
	if (workingTextureFrame)
	{
		workingSurface = workingTextureFrame->GetSurface();
		if (workingSurface)
		{
			if (FAILED(workingSurface->IsLost()))
			{
				// Surface for texture has been lost.
				// Assume that all surfaces for the texture have been lost.

				// Delete all of the DirectDraw surfaces. 
				for (int i = mNumAnimationFrames; i--;)
				{
					delete this->mTexSurface[i];
					this->mTexSurface[i] = NULL;
				}
				this->mNumAnimationFrames = 0;

				// Re-initialise the texture frames.
				if (!this->Init(this->mName))
				{
					OutputDebugString("Failed to realocate texture\n");
				}
				return;
			}
		}
		else
		{
			OutputDebugString("Ahhh #1\n");
		}
	}
	else
	{
		// Right, so there is no working surface. 
		// Assuming that the last time around the texture 
		// was freed (above), BUT failed to reload.

		// Just to be on the safe size free any existing 
		// surfaces and attempt to reload again.
		for (int i = mNumAnimationFrames; i--;)
		{
			if (mTexSurface[i])
			{
				delete this->mTexSurface[i];
				this->mTexSurface[i] = NULL;
			}
			this->mNumAnimationFrames = 0;
		}
		
		// Re-initialise the texture frames.
		this->Init(this->mName);
		return;
	}
	

	ENGINE.ChangeTexture(workingTextureFrame);

	// set render states.
	//! HMMMM

	switch(mAlphaType)
	{
	case 1:
	case 2:
	case 3:
	case 4:  // 4444 texture
		ENGINE.EnableMultiplicativeAlpha();
		break;
	}

	PutTris();

	switch(mAlphaType)
	{
	case 1:
	case 2:
	case 3:
	case 4:

		ENGINE.DisableAlpha();
		break;
	}

	ENGINE.AddToPolyCount(mNumTris);

	mNumVerts = 0;
	mNumTris = 0;
}

//******************************************************************************************
void	CTexture::IncreaseVertBuffer(SINT size)
{
	if (!mVerts)
	{
		// well, start with 128 
		mVerts = new UBYTE[128 * GetVertexSize()];
		ASSERT(mVerts);
		mVertBuffSize = 128;
	}
	else
	{
		UBYTE *t = new UBYTE[size * GetVertexSize()];
		ASSERT(t);
		memcpy(t, mVerts, mVertBuffSize * GetVertexSize());
		delete [] mVerts;
		mVerts = t;
		mVertBuffSize = size;
	}
}

//******************************************************************************************
void	CTexture::IncreaseTriBuffer(SINT size)
{
	if (!mTris)
	{
		// well, start with 128 
		mTris = new CTriIndex[128];
		ASSERT(mTris);
		mTriBuffSize = 128;
	}
	else
	{
		CTriIndex *t = new CTriIndex[size];
		ASSERT(t);
		memcpy(t, mTris, mTriBuffSize * sizeof(CTriIndex));
		delete [] mTris;
		mTris = t;
		mTriBuffSize = size;
	}
}
//******************************************************************************************
void CTexture::Activate(void)
{
	ENGINE.ChangeTexture(mTexSurface[SINT(mCurrentFrame)]);
}
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************


// --------------------------------------------------------
SINT	CTextureGTEX::AddVertex(CFVF_GTEX *v)
{
	if (mNumVerts == mVertBuffSize)
	{
		// need a larger buffer
		IncreaseVertBuffer(mVertBuffSize * 2);
	}

	((CFVF_GTEX *)mVerts)[mNumVerts] = *v;

	return mNumVerts++;
}

//******************************************************************************************
void	CTextureGTEX::PutTris()
{
	PROFILE_FN(RenderDIP);

	HRESULT	r = ENGINE.GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_GTEX_FLAGS,
															 mVerts, mNumVerts, (WORD *)mTris, mNumTris * 3, NULL );

	ASSERT(r == D3D_OK);
}

//******************************************************************************************
CTextureGTEX	*CTextureGTEX::GetTexture(char *fname)
{
	// do we have this texture already??
	CTexture *t = mFirstTexture;

	while (t)
	{
		if (!(stricmp(fname, t->GetName())))
		{
			if (t->GetType() != TT_GTEX)
			{	// is this the same format that it was loaded in?????
				ASSERT(0);  // actually - this is probably ok. - just create a new CTexture object using the same surface ...
//				return NULL;
			}
			else return (CTextureGTEX *)t; // yes! 
		}

		t = t->mNextTexture;
	};

	// nope - better create one!
	CTextureGTEX *n = new CTextureGTEX;

	if (n)
	{
		if (n->Init(fname))
		{
			n->mNextTexture = mFirstTexture;
			mFirstTexture = n;
			return n;
		}
		
		// oops
		delete n;

		return ENGINE.GetDefaultTextureGTEX(); // return default texture...
	}
	return FALSE;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

SINT	CTextureTL::AddVertex(D3DTLVERTEX *v)
{
	if (mNumVerts == mVertBuffSize)
	{
		// need a larger buffer
		IncreaseVertBuffer(mVertBuffSize * 2);
	}

	((D3DTLVERTEX *)mVerts)[mNumVerts] = *v;

	return mNumVerts++;
}

//******************************************************************************************
void	CTextureTL::PutTris()
{
	PROFILE_FN(RenderDIP);
	
	ENGINE.GetDevice()->SetTextureStageState(0, D3DTSS_ADDRESS , D3DTADDRESS_CLAMP);
	
	HRESULT	r = ENGINE.GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX,
															 mVerts, mNumVerts, (WORD *)mTris, mNumTris * 3, NULL );
	
	ENGINE.GetDevice()->SetTextureStageState(0, D3DTSS_ADDRESS , D3DTADDRESS_WRAP);

	ASSERT(r == D3D_OK);
}

//******************************************************************************************
CTextureTL	*CTextureTL::GetTexture(char *fname)
{
	// do we have this texture already??
	CTexture *t = mFirstTexture;

	//char buffer[256];
	//sprintf(buffer, "Moho: Loading %s.", fname);
	//SetWindowText(PLATFORM.GetDPWin()->GetHwnd(), buffer);
 	
	while (t)
	{
		if (!(stricmp(fname, t->GetName())))
		{
			if (t->GetType() != TT_TL)
			{	// is this the same format that it was loaded in?????
				ASSERT(0);
//				return NULL;
			}
			else return (CTextureTL *)t; // yes! 
		}

		t = t->mNextTexture;
	};

	// nope - better create one!

	CTextureTL *n = new CTextureTL;

	if (n)
	{
		if (n->Init(fname))
		{
			n->mNextTexture = mFirstTexture;
			mFirstTexture = n;
			return n;
		}
		
		delete n;
	}
	
	return FALSE;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

BOOL	CTextureGTEXAdditive::Init(char *fname)
{
	BOOL res = CTexture::Init(fname); //! careful if we decide to derive this elsewhere...

	mTransforms = NULL;
	mNumTransforms = 0;
	mTransformBuffSize = 0;
	mLastEngineTransformTri  = -1;
	mLastEngineTransformVert = -1;
	mFirstUntransformedVertex = 0;
	return res;
}

//******************************************************************************************
void	CTextureGTEXAdditive::ShutdownTransforms()
{
	if (mTransforms)
	{
		delete [] mTransforms;
		mTransforms = NULL;
	}
}

//******************************************************************************************

SINT	CTextureGTEXAdditive::AddVertex(CFVF_GTEX *v)
{
	if (ENGINE.GetTransformCount() != mLastEngineTransformVert)
	{
		// the transformation has changed - record this vertex number for use later
		mFirstUntransformedVertex = mNumVerts;
		mLastEngineTransformVert = ENGINE.GetTransformCount();
	}

	if (mNumVerts == mVertBuffSize)
	{
		// need a larger buffer
		IncreaseVertBuffer(mVertBuffSize * 2);
	}

	((CFVF_GTEX *)mVerts)[mNumVerts] = *v;

	return mNumVerts++;
}

//******************************************************************************************
void	CTextureGTEXAdditive::IncreaseTransformBuffer(SINT size)
{
	if (!mTransforms)
	{
		// well, start with 16
		mTransforms = new CTransform[16];
		ASSERT(mTransforms);
		mTransformBuffSize = 16;
	}
	else
	{
		CTransform *t = new CTransform[size];
		ASSERT(t);
		memcpy(t, mTransforms, mTransformBuffSize * sizeof(CTransform));
		delete [] mTransforms;
		mTransforms = t;
		mTransformBuffSize = size;
	}
}

//******************************************************************************************
void	CTextureGTEXAdditive::AddTri(SINT a, SINT b, SINT c)
{
	if (ENGINE.GetTransformCount() != mLastEngineTransformTri)
	{
		// the transformation has changed - we need to store a new transform
		if (mNumTransforms == mTransformBuffSize)
		{
			// need a larger buffer
			IncreaseTransformBuffer(mTransformBuffSize * 2);
		}
		
		ENGINE.GetTransforms(&mTransforms[mNumTransforms].mPos, &mTransforms[mNumTransforms].mOrientation);
		mTransforms[mNumTransforms].mStartTri  = mNumTris;
		mTransforms[mNumTransforms].mStartVert = mFirstUntransformedVertex;
		mLastEngineTransformTri = ENGINE.GetTransformCount();
		mNumTransforms ++;
	}

	ASSERT(a >= mFirstUntransformedVertex); // can't access verts outside of current transform scope...
	ASSERT(b >= mFirstUntransformedVertex); 
	ASSERT(c >= mFirstUntransformedVertex); 

	if (mNumTris == mTriBuffSize)
	{
		// need a larger buffer
		IncreaseTriBuffer(mTriBuffSize * 2);
	}

	mTris[mNumTris].mIndex[0] = WORD(a - mFirstUntransformedVertex);  // remember - the indices are relative later...
	mTris[mNumTris].mIndex[1] = WORD(b - mFirstUntransformedVertex);
	mTris[mNumTris].mIndex[2] = WORD(c - mFirstUntransformedVertex);

	mNumTris++;
}

//******************************************************************************************
void	CTextureGTEXAdditive::DrawAllAdditive()
{
	PROFILE_FN(RenderDIP);

	if (mNumTris == 0)
		return; // nothing to do!

	ENGINE.ChangeTexture(mTexSurface[SINT(mCurrentFrame)]);
	ENGINE.EnableAdditiveAlpha();

	// ok - draw in batches of transformation groups
	SINT start_tri  = 0, end_tri;
	SINT start_vert = 0, end_vert;

	SINT c0;
	for (c0 = 0; c0 < mNumTransforms; c0 ++)
	{
		if (c0 == mNumTransforms - 1)
		{
			end_tri  = mNumTris  - 1;
			end_vert = mNumVerts - 1;
		}
		else
		{
			end_tri  = mTransforms[c0 + 1].mStartTri -  1;
			end_vert = mTransforms[c0 + 1].mStartVert - 1;
		}

		ENGINE.SetupTransforms(mTransforms[c0].mPos, mTransforms[c0].mOrientation);

		HRESULT	r = ENGINE.GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_GTEX_FLAGS,
										((CFVF_GTEX *)mVerts) + start_vert, end_vert - start_vert + 1,
										(WORD *)(mTris + start_tri), (end_tri - start_tri + 1) * 3, NULL );
		ASSERT(r == D3D_OK);

		start_tri  = end_tri  + 1;
		start_vert = end_vert + 1;
	}
	ENGINE.DisableAlpha();
	
	mNumTransforms = 0;
	mLastEngineTransformTri  = -1;
	mLastEngineTransformVert = -1;
	mFirstUntransformedVertex = 0;

	ENGINE.AddToPolyCount(mNumTris);
	mNumVerts = 0;
	mNumTris = 0;
}

//******************************************************************************************
CTextureGTEXAdditive	*CTextureGTEXAdditive::GetTexture(char *fname)
{
	// do we have this texture already??
	CTexture *t = mFirstTexture;

	while (t)
	{
		if (!(stricmp(fname, t->GetName())))
		{
			if (t->GetType() != TT_GTEX_ADD)
			{	// is this the same format that it was loaded in?????
				ASSERT(0);  // actually - this is probably ok. - just create a new CTexture object using the same surface ...
//				return NULL;
			}
			else
				return (CTextureGTEXAdditive *)t; // yes! 
		}

		t = t->mNextTexture;
	};

	// nope - better create one!
	CTextureGTEXAdditive *n = new CTextureGTEXAdditive;

	if (n)
	{
		if (n->Init(fname))
		{
			n->mNextTexture = mFirstTexture;
			mFirstTexture = n;
			return n;
		}

		delete n;
	}
	return FALSE;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

// NOTE  This texture type does no inherent sorting.

SINT	CTextureTLAdditive::AddVertex(D3DTLVERTEX *v)
{
	if (mNumVerts == mVertBuffSize)
	{
		// need a larger buffer
		IncreaseVertBuffer(mVertBuffSize * 2);
	}

	((D3DTLVERTEX *)mVerts)[mNumVerts] = *v;

	return mNumVerts++;
}


//******************************************************************************************
void	CTextureTLAdditive::PutTris()
{
	PROFILE_FN(RenderDIP);
	
	HRESULT	r = ENGINE.GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX,
															 mVerts, mNumVerts, (WORD *)mTris, mNumTris * 3, NULL );
	ASSERT(r == D3D_OK);
}

//******************************************************************************************
CTextureTLAdditive	*CTextureTLAdditive::GetTexture(char *fname)
{
	// do we have this texture already??
	CTexture *t = mFirstTexture;

	while (t)
	{
		if (!(stricmp(fname, t->GetName())))
		{
			if (t->GetType() != TT_TLADD)
			{	// is this the same format that it was loaded in?????
				ASSERT(0);
				return NULL;
			}
			return (CTextureTLAdditive *)t; // yes! 
		}

		t = t->mNextTexture;
	};

	// nope - better create one!

	CTextureTLAdditive *n = new CTextureTLAdditive;

	if (n)
	{
		if (n->Init(fname))
		{
			n->mNextTexture = mFirstTexture;
			mFirstTexture = n;
			return n;
		}
		
		delete n;
	}
	
	return FALSE;
}

//******************************************************************************************
void	CTextureTLAdditive::DrawAllAdditive()
{
	if (mNumTris == 0)
		return; // nothing to do!

	ENGINE.ChangeTexture(mTexSurface[SINT(mCurrentFrame)]);

	// set render states.
	ENGINE.EnableAdditiveAlpha();;

	PutTris();

	ENGINE.DisableAlpha();

	ENGINE.AddToPolyCount(mNumTris);

	mNumVerts = 0;
	mNumTris = 0;
}

#endif

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//<<<<<<<<<<<<<<D R E A M C A S T      T  E  X  T  U  R  E      R O U T I N E S>>>>>>>>>>>>>
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************


#if TARGET == DC

#include	"Texture.h"
#include	"Globals.h"
#include	"RawLoader.h"
#include	"TGALoader.h"
#include	"d3derr.h"
#include	"Profile.h"

//******************************************************************************************
CTexture	*CTexture::mFirstTexture = NULL;

//******************************************************************************************
CTexture::~CTexture()
{
	Shutdown();
}

//******************************************************************************************
//** texture formal evaluation

static DDPIXELFORMAT pixel_format_rgb ;
static DDPIXELFORMAT pixel_format_rgba;

//******************************************************************************************
static int countbits(unsigned int a)
{
	int c = 0;
	while (a)
	{
		if (a & 1) c++;
		a >>= 1;
	}
	return c;
}

//******************************************************************************************
static HRESULT CALLBACK etfcb(DDPIXELFORMAT *pf, VOID *)
{


	return 1;
}

//******************************************************************************************
BOOL CTexture::EvaluatePixelFormats()
{

													   return FALSE;
}

//******************************************************************************************
BOOL	CTexture::InitAll()
{


	return TRUE;
}

//******************************************************************************************
BOOL	CTexture::Init(char *fname)
{
	char	name[256] = "data\\Textures\\";

	return TRUE;
}

//******************************************************************************************
void	CTexture::Shutdown()
{

}

//******************************************************************************************
void	CTexture::ReleaseAll()
{

}

//******************************************************************************************
void	CTexture::SetFrame(float frame)
{

}

//******************************************************************************************
void	CTexture::DrawAllTextures()
{

}

//******************************************************************************************
void	CTexture::DrawAllTexturesAdditive()
{
	
}

//******************************************************************************************
void	CTexture::AddTri(SINT a, SINT b, SINT c)
{
 	

}

//******************************************************************************************
void	CTexture::DrawAll()
{

}

//******************************************************************************************
void	CTexture::IncreaseVertBuffer(SINT size)
{

}

//******************************************************************************************
void	CTexture::IncreaseTriBuffer(SINT size)
{

}
//******************************************************************************************
void CTexture::Activate(void)
{
	ENGINE.ChangeTexture(mTexSurface[SINT(mCurrentFrame)]);
}
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

SINT	CTextureGTEX::AddVertex(CFVF_GTEX *v)
{

}

//******************************************************************************************
void	CTextureGTEX::PutTris()
{

}

//******************************************************************************************
CTextureGTEX	*CTextureGTEX::GetTexture(char *fname)
{
	CTextureGTEX *n = new CTextureGTEX;
	return n;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

SINT	CTextureTL::AddVertex(D3DTLVERTEX *v)
{


// SDS - add vertices to a strip header
// ************************************

// 	kmSetVertex (&VertexBufferDesc, &CurrentStrip_05[i], KM_VERTEXTYPE_05, sizeof (KMVERTEX_05));


}

//******************************************************************************************
void	CTextureTL::PutTris()
{

}

//******************************************************************************************
CTextureTL	*CTextureTL::GetTexture(char *fname)
{
	CTextureTL *n = new CTextureTL;
	return n;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

BOOL	CTextureGTEXAdditive::Init(char *fname)
{

}

//******************************************************************************************
void	CTextureGTEXAdditive::ShutdownTransforms()
{

}

//******************************************************************************************

SINT	CTextureGTEXAdditive::AddVertex(CFVF_GTEX *v)
{

}

//******************************************************************************************
void	CTextureGTEXAdditive::IncreaseTransformBuffer(SINT size)
{

}

//******************************************************************************************
void	CTextureGTEXAdditive::AddTri(SINT a, SINT b, SINT c)
{

}

//******************************************************************************************
void	CTextureGTEXAdditive::DrawAllAdditive()
{

}

//******************************************************************************************
CTextureGTEXAdditive	*CTextureGTEXAdditive::GetTexture(char *fname)
{
	CTextureGTEXAdditive *n = new CTextureGTEXAdditive;

}

//******************************************************************************************
void	CTextureTLAdditive::PutTris()
{

}

//******************************************************************************************
CTextureTLAdditive	*CTextureTLAdditive::GetTexture(char *fname)
{
	CTextureTLAdditive *n = new CTextureTLAdditive;
	
	
}

//******************************************************************************************
void	CTextureTLAdditive::DrawAllAdditive()
{

}

#endif
