#include "common.h"
// VolumeOutMaster.cpp : Module interface implementation.
// Developer : Alex Chmut
// Created : 8/11/98
#define STRICT
#include "PCSoundManager.h"
#include "IVolume.h"
#include "VolumeOutMaster.h"

/////////////////////////////////////////////////////////////////////////////
// 		Defines
#define	BAD_DWORD	(DWORD)-1
#define	WND_CLASS_NAME	"Master Output Volume Msg Wnd Class"
#define	WND_NAME		"Master Output Volume Msg Wnd"

/////////////////////////////////////////////////////////////////////////////
// 		Globals
PCVolumeOutMaster g_pThis = NULL;

////////////////////////////////////////////////////////////

//{{{ Audio specific functions
#define AUDFREQ			22050	// Frequency
#define AUDCHANNELS		1		// Number of channels
#define AUDBITSMPL		16		// Number of bits per sample
inline
void SetDeviceType( WAVEFORMATEX* pwfe )
{
	memset( pwfe, 0, sizeof(WAVEFORMATEX) );
	WORD  nBlockAlign = (AUDCHANNELS*AUDBITSMPL)/8;
	DWORD nSamplesPerSec = AUDFREQ;
	pwfe->wFormatTag = WAVE_FORMAT_PCM;
	pwfe->nChannels = AUDCHANNELS;
	pwfe->nBlockAlign = nBlockAlign;
	pwfe->nSamplesPerSec = nSamplesPerSec;
	pwfe->wBitsPerSample = AUDBITSMPL;
	pwfe->nAvgBytesPerSec = nSamplesPerSec*nBlockAlign;
}
//}}} Audio specific functions

/////////////////////////////////////////////////////////////////////////////
// 		Implementation
//////////////
CVolumeOutMaster::CVolumeOutMaster()
	:	m_bOK(false),
		m_bInitialized(false),
		m_bAvailable(false),

		m_uMixerID(0L),
		m_dwMixerHandle(0L),
		m_hWnd(NULL),

		m_dwMinimalVolume(BAD_DWORD),
		m_dwMaximalVolume(BAD_DWORD),

		m_pfUserSink(NULL),
		m_dwUserValue(0L)
{
	if ( m_bOK = Init() )
	{
		g_pThis = this;
		if ( !Initialize() )
		{
			Done();
			g_pThis = NULL;
		}
	}
}
//////////////
CVolumeOutMaster::~CVolumeOutMaster()
{
	if ( m_bOK )
		Done();
	g_pThis = NULL;
}
//////////////
bool CVolumeOutMaster::Init()
{
	if ( !mixerGetNumDevs() )
		return false;
	// Getting Mixer ID
	HWAVEOUT hwaveOut;
	MMRESULT mmResult;
	WAVEFORMATEX WaveFmt;
	SetDeviceType( &WaveFmt );
	mmResult = waveOutOpen( &hwaveOut, WAVE_MAPPER, &WaveFmt, 0L, 0L, CALLBACK_NULL );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		TRACE(".MasterOutputVolume: FAILURE: Could not open WaveOut Mapper. mmResult=%d\n", mmResult );
		return false;
	} else {
		mmResult = mixerGetID( (HMIXEROBJ)hwaveOut, &m_uMixerID, MIXER_OBJECTF_HWAVEOUT );
		waveOutClose( hwaveOut );
		if ( mmResult != MMSYSERR_NOERROR )
		{
			TRACE(".MasterOutputVolume: FAILURE: WaveOut Mapper in Mixer is not available. mmResult=%d\n", mmResult );
			return false;
		}
	}
	// Exposing Window to Mixer
	WNDCLASSEX wcx;
	memset( &wcx, 0, sizeof(WNDCLASSEX) );	
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.lpszClassName = WND_CLASS_NAME;
	wcx.lpfnWndProc = (WNDPROC)MixerWndProc;
	::RegisterClassEx(&wcx);
	m_hWnd = CreateWindow(	WND_CLASS_NAME,
							WND_NAME,
							WS_POPUP | WS_DISABLED,
							0, 0, 0, 0,
							NULL, NULL, NULL, NULL );
	if ( !m_hWnd )
	{
		TRACE(".MasterOutputVolume: FAILURE: Could not create internal window.\n" );
		return false;
	}
	::ShowWindow(m_hWnd, SW_HIDE);
	mmResult = mixerOpen( (LPHMIXER)&m_dwMixerHandle, m_uMixerID, (DWORD)m_hWnd, 0L, CALLBACK_WINDOW );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		TRACE(".MasterOutputVolume: FAILURE: Could not open Mixer. mmResult=%d\n", mmResult );
		::DestroyWindow( m_hWnd );
		return false;
	}
	return true;
}
//////////////
void CVolumeOutMaster::Done()
{
	if ( mixerClose( (HMIXER)m_dwMixerHandle ) != MMSYSERR_NOERROR )
	{
		TRACE(".MasterOutputVolume: WARNING: Could not close Mixer.\n" );
	}
	::DestroyWindow( m_hWnd );
	m_bInitialized = false;
	m_bOK = false;
}
//////////////
void CVolumeOutMaster::OnControlChanged( DWORD dwControlID )
{
	if ( m_dwVolumeControlID == dwControlID )
	{
		DWORD dwVolume = GetCurrentVolume();
		if ( (dwVolume!=BAD_DWORD) && (m_pfUserSink) )
		{
			(*m_pfUserSink)( dwVolume, m_dwUserValue );
		}
	}
}
//////////////
bool CVolumeOutMaster::Initialize()
{
	MMRESULT mmResult;
	if ( !m_bOK )
		return false;
	TRACE(".MasterOutputVolume: Initializing for Source MasterOut Line ..\n" );
	MIXERLINE MixerLine;
	memset( &MixerLine, 0, sizeof(MIXERLINE) );
	MixerLine.cbStruct = sizeof(MIXERLINE);
	MixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;
	mmResult = mixerGetLineInfo( (HMIXEROBJ)m_dwMixerHandle, &MixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		TRACE(".MasterOutputVolume: FAILURE: Could not get Speakers Destionation Line for the MasterOut Source Line while initilaizing. mmResult=%d\n", mmResult );
		return false;
	}

	MIXERCONTROL Control;
	memset( &Control, 0, sizeof(MIXERCONTROL) );
	Control.cbStruct = sizeof(MIXERCONTROL);

	MIXERLINECONTROLS LineControls;
	memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
	LineControls.cbStruct = sizeof(MIXERLINECONTROLS);

	LineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	LineControls.dwLineID = MixerLine.dwLineID;
	LineControls.cControls = 1;
	LineControls.cbmxctrl = sizeof(MIXERCONTROL);
	LineControls.pamxctrl = &Control;
	mmResult = mixerGetLineControls( (HMIXEROBJ)m_dwMixerHandle, &LineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE );
	if ( mmResult == MMSYSERR_NOERROR )
	{
		if ( !(Control.fdwControl & MIXERCONTROL_CONTROLF_DISABLED) )
		{
			m_bAvailable = true;
			TRACE(".MasterOutputVolume: \"%s\" Volume control for the Speakers Source Line adopted.\n", Control.szShortName );
		} else {
			TRACE(".MasterOutputVolume: WARNING: The Volume Control is disabled.\n" );
		}
	} else {
		TRACE(".MasterOutputVolume: WARNING: Could not get the Speakers Source line Volume Control while initilaizing. mmResult=%d\n", mmResult );
	}

	m_nChannelCount = MixerLine.cChannels;
	m_dwLineID = LineControls.dwLineID;
	m_dwVolumeControlID = Control.dwControlID;
	m_dwMinimalVolume = Control.Bounds.dwMinimum;
	m_dwMaximalVolume = Control.Bounds.dwMaximum;
	m_dwVolumeStep = Control.Metrics.cSteps;

	m_bInitialized = true;
	return true;
}
//////////////
void CVolumeOutMaster::EnableLine( bool bEnable )
{
	if ( !m_bInitialized )
		return;
	bool bAnyEnabled = false;
	MMRESULT mmResult;

	MIXERLINE lineDestination;
	memset( &lineDestination, 0, sizeof(MIXERLINE) );
	lineDestination.cbStruct = sizeof(MIXERLINE);
	lineDestination.dwLineID = m_dwLineID;
	mmResult = mixerGetLineInfo( (HMIXEROBJ)m_dwMixerHandle, &lineDestination, MIXER_GETLINEINFOF_LINEID );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		if ( bEnable )
		{
			TRACE(".MasterOutputVolume: FAILURE: Could not get the Speakers Destination Line while enabling. mmResult=%d\n", mmResult );
		} else {
			TRACE(".MasterOutputVolume: FAILURE: Could not get the Speakers Destination Line while disabling. mmResult=%d\n", mmResult );
		}
		return;
	}
	// Getting all line's controls
	int nControlCount = lineDestination.cControls;
	int nChannelCount = lineDestination.cChannels;
	MIXERLINECONTROLS LineControls;
	memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
	MIXERCONTROL* aControls = (MIXERCONTROL*)malloc( nControlCount * sizeof(MIXERCONTROL) );
	if ( !aControls )
	{
		if ( bEnable )
		{
			TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
		} else {
			TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
		}
		return;
	}
	memset( &aControls[0], 0, sizeof(nControlCount * sizeof(MIXERCONTROL)) );
	for ( int i = 0; i < nControlCount; i++ )
	{
		aControls[i].cbStruct = sizeof(MIXERCONTROL);
	}
	LineControls.cbStruct = sizeof(MIXERLINECONTROLS);
	LineControls.dwLineID = lineDestination.dwLineID;
	LineControls.cControls = nControlCount;
	LineControls.cbmxctrl = sizeof(MIXERCONTROL);
	LineControls.pamxctrl = &aControls[0];
	mmResult = mixerGetLineControls( (HMIXEROBJ)m_dwMixerHandle, &LineControls, MIXER_GETLINECONTROLSF_ALL );
	if ( mmResult == MMSYSERR_NOERROR )
	{
		for ( i = 0; i < nControlCount; i++ )
		{
			LONG lValue;
			bool bReadyToSet = false;
			switch (aControls[i].dwControlType)
			{
			case MIXERCONTROL_CONTROLTYPE_MUTE:
				lValue = (BOOL)!bEnable;
				bReadyToSet = true;
				break;
			case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
				lValue = (BOOL)bEnable;
				bReadyToSet = true;
				break;
			case MIXERCONTROL_CONTROLTYPE_MUX:
				lValue = (BOOL)bEnable;
				bReadyToSet = true;
				break;
			case MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT:
				lValue = (BOOL)bEnable;
				bReadyToSet = true;
				break;
			case MIXERCONTROL_CONTROLTYPE_MIXER:
				lValue = (BOOL)bEnable;
				bReadyToSet = true;
				break;
			}
			if ( bReadyToSet )
			{
				MIXERCONTROLDETAILS_BOOLEAN* aDetails = NULL;
				int nMultipleItems = aControls[i].cMultipleItems;
				int nChannels = nChannelCount;
				// MIXERCONTROLDETAILS
				MIXERCONTROLDETAILS ControlDetails;
				memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
				ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
				ControlDetails.dwControlID = aControls[i].dwControlID;
				if ( aControls[i].fdwControl & MIXERCONTROL_CONTROLF_UNIFORM )
				{
					nChannels = 1;
				}
				if ( aControls[i].fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
				{
					nMultipleItems = aControls[i].cMultipleItems;
					aDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nMultipleItems*nChannels*sizeof(MIXERCONTROLDETAILS_BOOLEAN));
					if ( !aDetails )
					{
						if ( bEnable )
						{
							TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
						} else {
							TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
						}
						continue;
					}
					for ( int nItem = 0; nItem < nMultipleItems; nItem++ )
					{
						/*
						if ( ( aControls[i].dwControlType & MIXERCONTROL_CONTROLTYPE_SINGLESELECT )
						  && ( nItem > 0 ) )
						{
							lValue = (LONG)!((BOOL)lValue);
						}
						*/
						for ( int nChannel = 0; nChannel < nChannels; nChannel++ )
						{
							aDetails[nItem+nChannel].fValue = lValue;
						}
					}
				} else {
					nMultipleItems = 0;
					aDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nChannels*sizeof(MIXERCONTROLDETAILS_BOOLEAN));
					if ( !aDetails )
					{
						if ( bEnable )
						{
							TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
						} else {
							TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
						}
						continue;
					}
					for ( int nChannel = 0; nChannel < nChannels; nChannel++ )
					{
						aDetails[nChannel].fValue = (LONG)lValue;
					}
				}
				ControlDetails.cChannels = nChannels;
				ControlDetails.cMultipleItems = nMultipleItems;
				ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
				ControlDetails.paDetails = &aDetails[0];
				mmResult = mixerSetControlDetails( (HMIXEROBJ)m_dwMixerHandle, &ControlDetails, 0L );
				if ( mmResult == MMSYSERR_NOERROR )
				{
					if ( bEnable )
					{
						TRACE(".MasterOutputVolume: Enabling Line: Speakers Line control \"%s\"(0x%X) has been set to %d.\n", aControls[i].szShortName, aControls[i].dwControlType, lValue );
					} else {
						TRACE(".MasterOutputVolume: Disabling Line: Speakers Line control \"%s\"(0x%X) has been set to %d.\n", aControls[i].szShortName, aControls[i].dwControlType, lValue );
					}
					bAnyEnabled = true;
				}
				free( aDetails );
			}
		}
	} else {
		if ( bEnable )
		{
			TRACE(".MasterOutputVolume: FAILURE: Could not get the line controls while enabling. mmResult=%d\n", mmResult );
		} else {
			TRACE(".MasterOutputVolume: FAILURE: Could not get the line controls while disabling. mmResult=%d\n", mmResult );
		}
	}
	free( aControls );
	if ( !bAnyEnabled )
	{
		if ( bEnable )
		{
			TRACE(".MasterOutputVolume: WARNING: No controls were found for enabling the line.\n" );
		} else {
			TRACE(".MasterOutputVolume: WARNING: No controls were found for disabling the line.\n" );
		}
	}
}
//////////////////////////////////////////////
// IVolume interface
//////////////
bool CVolumeOutMaster::IsAvailable()
{
	return m_bAvailable;
}
//////////////
void CVolumeOutMaster::Enable()
{
	EnableLine( true );
}
void CVolumeOutMaster::Disable()
{
	EnableLine( false );
}
//////////////
DWORD CVolumeOutMaster::GetVolumeMetric()
{
	if ( !m_bAvailable )
		return BAD_DWORD;
	return m_dwVolumeStep;
}
//////////////
DWORD CVolumeOutMaster::GetMinimalVolume()
{
	if ( !m_bAvailable )
		return BAD_DWORD;
	return m_dwMinimalVolume;
}
//////////////
DWORD CVolumeOutMaster::GetMaximalVolume()
{
	if ( !m_bAvailable )
		return BAD_DWORD;
	return m_dwMaximalVolume;
}
//////////////
DWORD CVolumeOutMaster::GetCurrentVolume()
{
	if ( !m_bAvailable )
		return BAD_DWORD;
	MIXERCONTROLDETAILS_UNSIGNED* aDetails = (MIXERCONTROLDETAILS_UNSIGNED*)malloc(m_nChannelCount*sizeof(MIXERCONTROLDETAILS_UNSIGNED));
	if ( !aDetails )
		return BAD_DWORD;
	MIXERCONTROLDETAILS ControlDetails;
	memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
	ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
	ControlDetails.dwControlID = m_dwVolumeControlID;
	ControlDetails.cChannels = m_nChannelCount;
	ControlDetails.cMultipleItems = 0;
	ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	ControlDetails.paDetails = &aDetails[0];
	MMRESULT mmResult = mixerGetControlDetails( (HMIXEROBJ)m_dwMixerHandle, &ControlDetails, MIXER_GETCONTROLDETAILSF_VALUE );
	DWORD dw = aDetails[0].dwValue;
	free( aDetails );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		TRACE(".MasterOutputVolume: FAILURE: Could not get volume. mmResult=%d\n", mmResult );
		return BAD_DWORD;
	}
	return dw;
}
//////////////
void CVolumeOutMaster::SetCurrentVolume( DWORD dwValue )
{
	if ( !m_bAvailable || (dwValue<m_dwMinimalVolume) || (dwValue>m_dwMaximalVolume) )
		return;
	MIXERCONTROLDETAILS_UNSIGNED* aDetails = (MIXERCONTROLDETAILS_UNSIGNED*)malloc(m_nChannelCount*sizeof(MIXERCONTROLDETAILS_UNSIGNED));
	if ( !aDetails )
		return;
	for ( int i = 0; i < m_nChannelCount; i++ )
	{
		aDetails[i].dwValue = dwValue;
	}
	MIXERCONTROLDETAILS ControlDetails;
	memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
	ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
	ControlDetails.dwControlID = m_dwVolumeControlID;
	ControlDetails.cChannels = m_nChannelCount;
	ControlDetails.cMultipleItems = 0;
	ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	ControlDetails.paDetails = &aDetails[0];
	MMRESULT mmResult = mixerSetControlDetails( (HMIXEROBJ)m_dwMixerHandle, &ControlDetails, MIXER_SETCONTROLDETAILSF_VALUE );
	free( aDetails );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		TRACE(".MasterOutputVolume: FAILURE: Could not set volume(%d) mmResult=%d\n", dwValue, mmResult );
	}
}
//////////////
void CVolumeOutMaster::RegisterNotificationSink( PONMICVOULUMECHANGE pfUserSink, DWORD dwUserValue )
{
	m_pfUserSink = pfUserSink;
	m_dwUserValue = dwUserValue;
}
////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK CVolumeOutMaster::MixerWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == MM_MIXM_CONTROL_CHANGE )
	{
		if ( g_pThis )
		{
			g_pThis->OnControlChanged( (DWORD)lParam );
		}
	}
	return ::DefWindowProc( hwnd, uMsg, wParam, lParam);
}
