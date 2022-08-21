#ifndef	FRINGE_H
#define	FRINGE_H
      //======================================================-==--=-- --  -
     //
    // LODable 'fringe' drawwer, a fringe is like a disc with N segments
   // and a variable number of concentric thingys, this is supposed to
  // be stateless which is going to be interesting but, we'll see.
 //
//======================================================-==--=-- --  -
class Fringe
{
	VBuffer*		mVB;				  //  Pointer to LT VBuffer
	UWORD*			mTriList;			 //	 Triangle List
	UWORD			mNInds;				//	3* Number of triangles

	SINT			mSegments;
	SINT			mRadii;
	float			mURep;
	CTextureGTEX*	mTexture;

public:
	void	SetTrilist(UWORD* tl, UWORD ninds)		{ mTriList = tl; mNInds = ninds;};
	void	SetSegs(SINT segs)						{ mSegments = segs;};
	void	SetRads(SINT rads)						{ mRadii = rads;};
	void	SetURep(float ur)						{ mURep = ur;};					
	void	SetVBuffer(VBuffer* vb)					{ mVB = vb;};
	void	SetTexture(CTextureGTEX* tex)			{ mTexture = tex;};

	void	CreateMesh(const FMatrix	mat, const FVector pos, const SINT lod);
	void	Draw();
};

#endif