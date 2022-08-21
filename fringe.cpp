// $Header$

// $Log$
// Revision 1.3  2000-07-11 11:11:38+01  jjs
// Changed DrawIndexedprimitive to DX7 spec.
//
// Revision 1.2  2000-07-11 10:08:53+01  jjs
// Commented out Drawprim for DC.
//

#include	"common.h"
#include	"globals.h"

#include	"vbuffer.h"

#include	"Fringe.h"
           //======================================================-==--=-- --  -
          //
         //   precalculated trilists to allow simplified vertex buffers  
        //
       //     0-1  0-1-2  0-1-2-3
      //      |\|  |\|\|  |\|\|\|
     //       2-3  3-4-5  4-5-6-. 
    //        |\|  |\|\.  |\|\|..
   //         4-.  6-7..  8-9-...
  //          |..  |\...  |\|....
 // 
//======================================================-==--=-- --  -
#define DEFINEQUAD_SPLITTL(X, MOD) (X)+(MOD), (X), (X)+(MOD)+1, (X)+(MOD)+1, (X), (X)+1
#define	M1W(X) DEFINEQUAD_SPLITTL(X, 2)
#define	M2W(X) DEFINEQUAD_SPLITTL(X, 3), DEFINEQUAD_SPLITTL((X+1), 3)
#define	M3W(X) DEFINEQUAD_SPLITTL(X, 4), DEFINEQUAD_SPLITTL((X+1), 3), DEFINEQUAD_SPLITTL((X+2), 3)
 // These Are good for up to 16 high meshes, note that I don't support 'wrapping'
// as it's largely screwed on most drivers i've tried it on
UWORD onewidemesh[]	  =	{	
							M1W(0), M1W(2), M1W(4), M1W(6), M1W(8), M1W(10),M1W(12),M1W(14),
							M1W(16),M1W(18),M1W(20),M1W(22),M1W(24),M1W(26),M1W(28),M1W(30)
						};
UWORD twowidemesh[]	  =	{	
							M2W(0), M2W(3), M2W(6), M2W(9), M2W(12),M2W(15),M2W(18),M2W(21),
							M2W(24),M2W(27),M2W(30),M2W(33),M2W(36),M2W(39),M2W(42),M2W(45)
						};
UWORD threewidemesh[] =	{	
							M3W(0), M3W(4), M3W(8), M3W(12),M3W(16),M3W(20),M3W(24),M3W(28),
							M3W(32),M3W(36),M3W(40),M3W(44),M3W(48),M3W(52),M3W(56),M3W(60)
						};
//======================================================-==--=-- --  -
void Fringe::CreateMesh(const FMatrix	mat, const FVector pos, const SINT lod)
{
	CFVF_UVRGB*	vbp = (CFVF_UVRGB*)mVB->Lock();	// lock the vertex buffer
	FMatrix		spinner = mat;
	FMatrix		rot;
	rot.MakeRotationF( (PI*2)/(float)mSegments,0,0);

	CFVF_UVRGB* vp = vbp;
	int	yc;
	float u = 0;
	float uinc = mURep/(float)mSegments;

	for(yc=0; yc<=mSegments; u+=uinc, yc++)
	{
		FVector	v = spinner*FVector(0,0,1);
		spinner=spinner*rot;
		
		vp[0].mDiffuse	= 0xffffffff;
		vp[0].mPos		= pos+v*0.1f;
		vp[0].mU		= u;
		vp[0].mV		= 0;

		vp[1].mDiffuse	= 0xffffffff;
		vp[1].mPos		= pos+v*1.0f;
		vp[1].mU		= u;
		vp[1].mV		= 1;
		
		vp+=2;
	}
	mTriList	= onewidemesh;
	mNInds		= 6*mSegments;
	mVB->Unlock();
}
//======================================================-==--=-- --  -
void Fringe::Draw()
{
	mTexture->Activate();
#if TARGET != DC
	PLATFORM.GetD3DDevice()->DrawIndexedPrimitiveVB(	D3DPT_TRIANGLELIST, mVB->VB, 0, (mSegments+1)*2, mTriList, mNInds, 0);
#endif
}
//======================================================-==--=-- --  -
