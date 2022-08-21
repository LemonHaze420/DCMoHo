// $Header$

// $Log$

#include <windows.h>
#include <stdio.h>
#include "ddraw.h"
#include "mmsystem.h"
#define DWORD_PTR DWORD
#define LONG_PTR long
#include "mmstream.h"
#include "amstream.h"
#include "ddstream.h"


HRESULT OpenMMStream(const char * pszFileName, IDirectDraw7 *pDD, 
				  IMultiMediaStream **ppMMStream);

HRESULT RenderStreamToSurface(IDirectDraw7 *pDD, IDirectDrawSurface7 *pPrimary,
			      IMultiMediaStream *pMMStream, int xcoord, int ycoord);

HRESULT RenderFrameToSurface(IDirectDraw7 *pDD7, IDirectDrawSurface7 *pPrimary,
			      IMultiMediaStream *pMMStream, int xcoord, int ycoord);

void PlayMovie(char *filename, int xpos, int ypos);
void FirstFrame(char *filename, int xpos, int ypos, LPDIRECTDRAWSURFACE7 * surface);
void PlayMovie(char *filename, int xpos, int ypos, int xres, int yres);
void WaitForMovieEnd();
void KillMovieTimer();