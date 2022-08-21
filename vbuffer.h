// $Header$

// $Log$
// Revision 1.1  2000-07-11 10:57:55+01  jjs
// Pre DX7 version.
//

#ifndef	VBUFFER_H
#define	VBUFFER_H

    //======================================================-==--=-- --  -
   //
  //	Vertex buffer creation and deletion 
 //
//======================================================-==--=-- --  -
class	VBuffer
{
public:
	IDirect3DVertexBuffer7*	VB;
	VBuffer()
	{
		VB=0;
	};
	~VBuffer()
	{ 
		if(VB) VB->Release();
	};
	void Release(void)
	{
		if(VB) VB->Release();
	};
	// ### Default Lock as per the MS vbuffer sample app
	
	void* Lock()
	{
		void*	t;
		VB->Lock( DDLOCK_WAIT, &t, NULL );
		return t;
	};
	void Unlock()
	{
		VB->Unlock();
	};
      //======================================================-==--=-- --  -
     //
    // This creates a vertex buffer by first creating a vertex buffer
   // description, the second parameter is the flags for CreateVertexBuffer
  // the last three parameters are as described in D3DVERTEXBUFFERDESC
 //
//======================================================-==--=-- --  -
	int	Init(LPDIRECT3DDEVICE7 d3ddev,	/* D3DDEVICE3					*/
			int dwFlags,				/* flags for CreatVertexBuffer	*/
			int dwCaps,					/* CAPS for D3DVERTEXBUFFERDESC */
			int dwFVF,					/* vertex format for		"	*/
			int dwNumVertices);			/* number of vertices for	"	*/

};

#endif