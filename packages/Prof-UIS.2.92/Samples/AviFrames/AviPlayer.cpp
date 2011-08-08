// AviPlayer.cpp: implementation of the CAviPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aviframes.h"
#include "AviPlayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( CAviPlayer, CObject );

//////////////////////////////////////////////////////////////////////////

CAviPlayer::CAviPlayer(CWnd *pWndParent)
	: m_pAviStream( NULL )
	, m_pGetFrame( NULL )
	, m_nAviWidth( 0 )
	, m_nAviHeight( 0 )
	, m_nRenderWidth( 0 )
	, m_nRenderHeight( 0 )
	, m_hDrawDib( NULL )
	, m_nFrameNumber( 0 )
	, m_nFrameCount( 0 )
	, m_nFrameSkiped(0)
	, m_pAviPlayerWnd(NULL)
	, m_bIsPlaying( FALSE )
	, m_bTempPause( FALSE )
{
	AVIFileInit();

	m_pAviPlayerWnd = new CAviPlayerWnd(this);
	m_pAviPlayerWnd->Create( pWndParent	);
	
}

//////////////////////////////////////////////////////////////////////////

CAviPlayer::~CAviPlayer()
{
	if( m_pAviPlayerWnd ){
		m_pAviPlayerWnd->DestroyWindow();
		delete m_pAviPlayerWnd;
		m_pAviPlayerWnd = NULL;
	}
	Clean();
	AVIFileExit();
}

//////////////////////////////////////////////////////////////////////////

void CAviPlayer::Clean()
{
	if( m_hDrawDib != NULL ){
		::DrawDibClose( m_hDrawDib );
		m_hDrawDib = NULL;
	}
	if( m_pGetFrame != NULL ){
		::AVIStreamGetFrameClose( m_pGetFrame );
		m_pGetFrame = NULL; 
	}
	if( m_pAviStream != NULL ){
		::AVIStreamRelease( m_pAviStream );
		m_pAviStream = NULL;
	}
	::DrawDibEnd(m_hDrawDib);
	::DrawDibClose(m_hDrawDib);
	m_hDrawDib = NULL;
}
	
//////////////////////////////////////////////////////////////////////////

BOOL CAviPlayer::Prepare()
{
	ASSERT(m_pAviPlayerWnd);
	m_pAviPlayerWnd->ShowWindow( SW_HIDE );
	AVIStreamInfo(
		m_pAviStream,
		&m_AviInfo,
		sizeof(m_AviInfo)
		);

	m_nRenderWidth = m_nAviWidth = m_AviInfo.rcFrame.right - m_AviInfo.rcFrame.left;
	m_nRenderHeight = m_nAviHeight = m_AviInfo.rcFrame.bottom - m_AviInfo.rcFrame.top;
	
	SetPlayerWindowPos( 0, 0, m_nRenderWidth, m_nRenderHeight );

	//get start position and count of frames
	m_nFrameNumber = AVIStreamStart( m_pAviStream );
	m_nFrameCount  = AVIStreamLength( m_pAviStream );
	
	m_pGetFrame =
		::AVIStreamGetFrameOpen(
		m_pAviStream,
		NULL
		);
	if( m_pGetFrame == NULL )
	{
//		ASSERT( FALSE );
		AfxMessageBox( _T("AVI frame initialization failed"), MB_OK|MB_ICONERROR );
		return FALSE;
	}

	m_hDrawDib = ::DrawDibOpen();
	if( m_hDrawDib == NULL )
	{
//		ASSERT( FALSE );
		AfxMessageBox( _T("DrawDibOpen() failed"), MB_OK|MB_ICONERROR );
		return FALSE;
	}	
	
	m_pAviPlayerWnd->ShowWindow( SW_SHOWNOACTIVATE );
	return TRUE;
}
	
//////////////////////////////////////////////////////////////////////////

BOOL CAviPlayer::OpenFromFile( LPCTSTR sAviFilePath )
{
	Clean();
	
	if( ::AVIStreamOpenFromFile(
		&m_pAviStream,
		sAviFilePath,
		streamtypeVIDEO,
		0,
		OF_READ,
		NULL
		) != 0
		)
	{
		Clean();
		ASSERT( FALSE );
		AfxMessageBox( _T("AVI stream initialization failed"), MB_OK|MB_ICONERROR );
		return FALSE;
	}
	BOOL bRet = Prepare();
	return bRet;
}

//////////////////////////////////////////////////////////////////////////

BOOL CAviPlayer::DrawFrame(
	HDC hDC,
	int nFrameNumber
	)
{
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)
		::AVIStreamGetFrame(
		m_pGetFrame,
		nFrameNumber
		);

	if(!lpbi)
		return FALSE;

	unsigned char * pAviFrameRawData = (unsigned char *)
		lpbi
		+ lpbi->biSize
		+ lpbi->biClrUsed * sizeof(RGBQUAD);

	unsigned char * pDibRawData = NULL;
		
	HBITMAP hBitmap =
		::CreateDIBSection(
		hDC,
		(BITMAPINFO*)(lpbi),
		DIB_RGB_COLORS,
		(void**)(&pDibRawData),
		NULL,
		NULL
		);

	if( hBitmap == NULL )
	{
		ASSERT( FALSE );
		AfxMessageBox( _T("Failed to create DIB section"), MB_OK|MB_ICONERROR );
		return FALSE;
	}

	ASSERT( pDibRawData != NULL );

	::DrawDibDraw(
		m_hDrawDib,
		hDC,
		0, 0, m_nRenderWidth, m_nRenderHeight,
		lpbi,
		pAviFrameRawData,
		0, 0, m_nAviWidth, m_nAviHeight,
		0
		);

	DeleteObject(hBitmap);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
