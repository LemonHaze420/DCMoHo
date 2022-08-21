#include	"Common.h"

#if TARGET == PC

#include	"Globals.h"
#include	"PCFMV.h"

#include	"pcmovie.h"
#include	"movie.h"
#include	"rad.h"
#include	"bink.h"

//*****************************************************************************
#define STR_VOL	64

StrInfo STR_ROCKSTAR =
{
		"DATA\\VIDEO\\ROCKSTAR.BIK",
		0,32,
		320,192
};

StrInfo STR_BROADSWORD =
{
		"DATA\\VIDEO\\LOGO9.BIK",
		0,0,
		640,480
};

StrInfo STR_TAKE2 =
{
		"DATA\\VIDEO\\T2INTRO.BIK",
		0,32,
		320,192,
};

StrInfo STR_LOSTTOYS =
{
		"DATA\\VIDEO\\LOSTLOGO.BIK",
		0,32,
		320,192,
};

StrInfo STR_INTRO_ENG =
{
		"DATA\\VIDEO\\INTENG.BIK",
		0,32,
		320,192,
};

StrInfo STR_INTRO_FRE =
{
		"DATA\\VIDEO\\INTFREN.BIK",
		0,32,
		320,192,
};

StrInfo STR_INTRO_GER=
{
		"DATA\\VIDEO\\INTGERM.BIK",
		0,32,
		320,192,
};

StrInfo STR_INTRO_SPA =
{
		"DATA\\VIDEO\\INTSPAN.BIK",
		0,32,
		320,192,
};

StrInfo STR_INTRO_ITA =
{
		"DATA\\VIDEO\\INTITA.BIK",
		0,32,
		320,192,
};

StrInfo STR_ANGEL =
{
		"DATA\\VIDEO\\ANGELS.BIK",
		0,32,
		320,192,
};

StrInfo STR_APOSTLE =
{
		"DATA\\VIDEO\\APOSTS.BIK",
		0,32,
		320,192,
};

StrInfo STR_BENNY =
{
		"DATA\\VIDEO\\BENNS.BIK",
		0,32,
		320,192,
};

StrInfo STR_LOCKDOWN =
{
		"DATA\\VIDEO\\LOCKS.BIK",
		0,32,
		320,192,
};

StrInfo STR_SOPHIE =
{
		"DATA\\VIDEO\\SOPHS.BIK",
		0,32,
		320,192,
};

StrInfo STR_DOCTOR =
{
		"DATA\\VIDEO\\DOCS.BIK",
		0,32,
		320,192,
};

StrInfo STR_ALPHASTAR_IN =
{
		"DATA\\VIDEO\\ALPHINT.BIK",
		0,32,
		320,192,
};

StrInfo STR_ALPHASTAR_ANGEL =
{
		"DATA\\VIDEO\\ANGELOS.BIK",
		0,32,
		320,192,
};

StrInfo STR_ALPHASTAR_APOSTLE =
{
		"DATA\\VIDEO\\APOSTOS.BIK",
		0,32,
		320,192,
};

StrInfo STR_ALPHASTAR_BENNY =
{
		"DATA\\VIDEO\\BENNOS.BIK",
		0,32,
		320,192,
};

StrInfo STR_ALPHASTAR_LOCKDOWN =
{
		"DATA\\VIDEO\\LOCKOS.BIK",
		0,32,
		320,192,
};

StrInfo STR_ALPHASTAR_SOPHIE =
{
		"DATA\\VIDEO\\SOPHOS.BIK",
		0,32,
		320,192,
};

StrInfo STR_ALPHASTAR_DOCTOR =
{
		"DATA\\VIDEO\\DOCOS.BIK",
		0,32,
		320,192,
};

// advance to the next Bink Frame
static bool NextBinkFrame(HBINK bink, HBINKBUFFER binkBuffer)
{
	DDSURFACEDESC2 DDSdesc;
	
	// decompress a frame
	BinkDoFrame(bink);
	DDSdesc.dwSize = sizeof(DDSURFACEDESC2);
	
	// lock the primary surface
	if (BinkBufferLock(binkBuffer))
	{
		// copy the data onto the screen
		if (BinkCopyToBuffer(bink, binkBuffer->Buffer, binkBuffer->BufferPitch, binkBuffer->Height,0,0,binkBuffer->SurfaceType) == 1)
		{
			//MessageBox(NULL, "DEBUG: Failed to blit frame.", GAME_NAME, MB_OK);
			//exit(1);
		}
	
		// unlock the surface
		BinkBufferUnlock(binkBuffer);
	}
	
	BinkBufferBlit(binkBuffer, bink->FrameRects, BinkGetRects(bink, binkBuffer->SurfaceType));
	// advance or close the window
	if (bink->FrameNum==bink->Frames) // goto the next if not on the last
		return false;

	BinkNextFrame(bink);
	return true;
}

bool NeedToResetRes = false;

// ----------------------------------------------------------------------------
// Attempts to obtain the resolution 640, 480, 32 if _Mode is set to true
// Setting _Mode to false returns to the default resolution.
void	FMVResChange(bool _Mode)
{
	SINT d;

	if (_Mode)
		d = 32;
	else
		d = 16;

	if (NeedToResetRes || _Mode)
	{
		for (int i = mySystem->GetNumberOfVideoDevices(); i--;)
		{
			if (mySystem->GetVideoDevice(i)->Guid == SelectedDisplayDevice)
			{
				for (int j = mySystem->GetVideoDevice(i)->GetNumberOfResolutions(); j--;)
				{
					if (	(mySystem->GetVideoDevice(i)->GetResolution(j)->dwWidth == 640) 
						&&	(mySystem->GetVideoDevice(i)->GetResolution(j)->dwHeight == 480) 
						&&	(mySystem->GetVideoDevice(i)->GetResolution(j)->ddpfPixelFormat.dwRGBBitCount == d)
						)
					{
	#ifndef _DEBUG
						PLATFORM.GetDPWin()->DeclareResolutionChange(640, 480, d);
						NeedToResetRes = _Mode;
	#endif
					}
				}
			}
		}
	}
}

//*****************************************************************************
SINT	PlayFMV(EMovie movie)
{
	StrInfo	*stream				= NULL;
	SINT	ret					= PLAYSTR_ERROR;
	bool	ForceMovie			= false;
	bool	ContinueWithMovie	= true;	

	switch(movie)
	{
//	case FMV_ROCKSTAR:				stream = &STR_ROCKSTAR;				break;
	case FMV_ROCKSTAR:				stream = &STR_TAKE2;				break;
	case FMV_LOSTTOYS:				stream = &STR_LOSTTOYS;				break;
	case FMV_BROADSWORD:			stream = &STR_BROADSWORD;			break;
	case FMV_INTRO_ENG:				stream = &STR_INTRO_ENG;			break; 
	case FMV_INTRO_FRE:				stream = &STR_INTRO_FRE;			break;
	case FMV_INTRO_GER:				stream = &STR_INTRO_GER;			break;
	case FMV_INTRO_SPA:				stream = &STR_INTRO_SPA;			break;
    case FMV_INTRO_ITA:				stream = &STR_INTRO_ITA;			break;
	case FMV_ANGEL:					stream = &STR_ANGEL;				break;
	case FMV_APOSTLE:				stream = &STR_APOSTLE;				break;
	case FMV_BENNY:					stream = &STR_BENNY;				break;
	case FMV_LOCKDOWN:				stream = &STR_LOCKDOWN;				break;
	case FMV_SOPHIE:				stream = &STR_SOPHIE;				break;
	case FMV_DOCTOR:				stream = &STR_DOCTOR;				break;
	case FMV_ALPHASTAR_IN:			stream = &STR_ALPHASTAR_IN;			break;
	case FMV_ALPHASTAR_ANGEL:		stream = &STR_ALPHASTAR_ANGEL;		break;
	case FMV_ALPHASTAR_APOSTLE:		stream = &STR_ALPHASTAR_APOSTLE;	break;
	case FMV_ALPHASTAR_BENNY:		stream = &STR_ALPHASTAR_BENNY;		break;
	case FMV_ALPHASTAR_LOCKDOWN:	stream = &STR_ALPHASTAR_LOCKDOWN;	break;
	case FMV_ALPHASTAR_SOPHIE:		stream = &STR_ALPHASTAR_SOPHIE;		break;
	case FMV_ALPHASTAR_DOCTOR:		stream = &STR_ALPHASTAR_DOCTOR;		break;
	};

	if (stream)
	{
		SOUND.StopTrack();

		// - are ------------------------------------------
		// Verify that the primary surface is valid and 
		// then clear to black.
		
		DDBLTFX					clearEffect;
		LPDIRECTDRAWSURFACE7	refSurface;
		ZeroMemory(&clearEffect, sizeof(DDBLTFX));
		clearEffect.dwSize = sizeof(DDBLTFX);
		clearEffect.dwFillColor = 0x00000000;		// Black

		// May help!
		PLATFORM.GetDPWin()->GetDirectDraw7()->WaitForVerticalBlank(DDWAITVB_BLOCKEND,NULL);
		PLATFORM.GetDPWin()->GetDirectDraw7()->FlipToGDISurface();

		// Obtain reference to the primary surface. (Can return NULL).
//		PLATFORM.GetDPWin()->GetDirectDraw7()->GetGDISurface(&refSurface);
//		refSurface = PLATFORM.GetDPWin()->GetPrimarySurface();
		if (	SUCCEEDED(PLATFORM.GetDPWin()->GetDirectDraw7()->GetGDISurface(&refSurface))
			&&	refSurface)
		{
			if (FAILED(refSurface->IsLost()))
			{
				// Surface memory lost... attempt to restore memory.
				if (PLATFORM.GetDPWin()->PrimaryRestore() != DD_OK)
				{
					// Failed to restore, not safe to blit. 
					// Can't do anything more here.
					return 0;
				}
			}

			// Surface is valid, clear to black.
			refSurface->Blt(NULL, 
							NULL, 
							NULL, 
							DDBLT_WAIT | DDBLT_COLORFILL, 
							&clearEffect);
		}
		else
		{
			// No primary surface object!!! 
			// Can't do anything... return 0!
			return 0;
		}

		// Finished verifying, restoring and clearing 
		// display, now it's okay to continue with render
		// the FMV, etc.
		// - End are --------------------------------------


		// In certain cases want to assign key callback
		// that quits the rest of the movie.
		switch(movie)
		{
		case FMV_ALPHASTAR_IN:
		case FMV_ALPHASTAR_ANGEL:
		case FMV_ALPHASTAR_APOSTLE:
		case FMV_ALPHASTAR_BENNY:
		case FMV_ALPHASTAR_LOCKDOWN:
		case FMV_ALPHASTAR_SOPHIE:
		case FMV_ALPHASTAR_DOCTOR:
			ForceMovie = true;
			break;
		default:
			ForceMovie = false;
			break;
		}


		HBINK bink;
		HBINKBUFFER binkBuffer;
		
		BinkSoundUseDirectSound(SOUND.GetDirectSound());
	
		// Locate cd for video
		DWORD	Drives;
		FILE*	FH;
		char	VideoPath[MAX_PATH];
		
		// Check for MoHo CD - can be found in any [CD] drive.
		Drives = GetLogicalDrives();
		if (Drives)
		{
			for (int i = 0; i < 32; i++)
			{
				if (Drives & (1 << i))
				{
					sprintf(VideoPath, "%c:\\", i + 65);
					if (GetDriveType(VideoPath) == DRIVE_CDROM)
					{
						strcat(VideoPath, stream->strName);
						
						// Check for a file on the disk.
						FH = fopen(VideoPath, "r");
						if (FH)
						{
							fclose(FH);
							
							// Open the Bink file
							bink=BinkOpen(VideoPath, 0);
							
							if(bink)
							{
								PLATFORM.GetDPWin()->GetDirectDraw7()->GetGDISurface(&refSurface);
								BinkBufferSetDirectDraw(PLATFORM.GetDPWin()->GetDirectDraw7(), refSurface);	// are: Already have a handle to the primary surface, from above.
								
								if(stream->width == 320)
								{
									binkBuffer = BinkBufferOpen(PLATFORM.GetDPWin()->GetHwnd(), stream->width,stream->height, BINKBUFFERSTRETCHYINT | BINKBUFFERSTRETCHXINT | BINKBUFFERAUTO);
									
									BinkBufferSetScale(binkBuffer, 640, 384);
									
									BinkBufferSetOffset(binkBuffer, 0, 48);
								}
								else
									binkBuffer = BinkBufferOpen(PLATFORM.GetDPWin()->GetHwnd(), stream->width,stream->height, BINKBUFFERAUTO);
								
								ContinueWithMovie = true;
								while (ContinueWithMovie)
								{
									PLATFORM.ProcessMessages();
									if (	(CONTROLS.IsButtonPressed(BUTTON_PSX_X))
										|| 	(CONTROLS.IsButtonPressed(BUTTON_FIRE_PRIMARY))
										||	((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_SPACE]) & 0x80)
										|| 	((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_RETURN]) & 0x80)
										|| 	((PLATFORM.GetDPWin()->GetDInput()->GetKeyState()[DIK_ESCAPE]) & 0x80)
										)
									{
										ContinueWithMovie = ForceMovie;
									}
									
									if (!BinkWait(bink))
									{
										if(!NextBinkFrame(bink,binkBuffer))
										{
											ContinueWithMovie = false;
										}
									}
								}
								
								refSurface->Blt(NULL, 
									NULL, 
									NULL, 
									DDBLT_WAIT | DDBLT_COLORFILL, 
									&clearEffect);
								
								BinkBufferClose(binkBuffer);
								
								if (bink)
									BinkClose(bink);

								// Wait for a while. So that the keys will clear.
								Sleep(200);
							}
							
							break;
						}
					}
				}
			}
		}

		

	//	TTWindow* refTTWindow = PLATFORM.GetDPWin();
	//	if (refTTWindow)
	//	{
	//		OutputDebugString("Restoring D3D and buffers.\n");
	//		refTTWindow->RestoreD3D();
	//	}
	}

	// Un assign key callback.
	PLATFORM.SetKeyChangeCallback(NULL);

	return PLAYSTR_END;
}
//*****************************************************************************
#endif

