// AviPlayerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "aviframes.h"
#include "AviPlayerWnd.h"
#include "AviPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAviPlayerWnd
IMPLEMENT_DYNAMIC(CAviPlayerWnd, CWnd)

CAviPlayerWnd::CAviPlayerWnd( CAviPlayer *pAviPlayer )
{
	m_pAviPlayer = pAviPlayer;
}

CAviPlayerWnd::~CAviPlayerWnd()
{
}


BEGIN_MESSAGE_MAP(CAviPlayerWnd, CWnd)
	//{{AFX_MSG_MAP(CAviPlayerWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


bool CAviPlayerWnd::Create(
					  CWnd * pWndParent,
					  UINT nDlgCtrlID, // = AFX_IDW_PANE_FIRST
					  CRect rc, // = CRect( 0,0,0,0 )
					  DWORD dwStyle, // = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN
					  DWORD dwStyleEx // = 0L
					  )
{
	LPCTSTR strClass =
		::AfxRegisterWndClass(
		CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW),
		NULL,
		NULL
		);
	if( strClass == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	if( ! CreateEx(
		dwStyleEx,
		strClass,
		_T(""),
		dwStyle,
		rc.left, rc.top, rc.Width(), rc.Height(),
		pWndParent->GetSafeHwnd(),
		NULL,
		NULL
		)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	SetDlgCtrlID( nDlgCtrlID );
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CAviPlayerWnd message handlers

void CAviPlayerWnd::OnTimer(__EXT_MFC_UINT_PTR nIDEvent) 
{
	if( nIDEvent == 1 )
	{
		if( m_pAviPlayer == NULL )
			return;	

		BOOL bTempPause = m_pAviPlayer->GetTempPause();
		if( bTempPause ){
			m_nTimerStart = GetTickCount();
			return;
		}

		int nTimerSpeed = m_pAviPlayer->GetTimerSpeed();

		DWORD nCurrentTimePlayed = 0;
		DWORD nCurrentTimeNeeded = 0;

		int i = 0;
		do 
		{
			i++;
			m_pAviPlayer->NextFrame();

			DWORD nCurrentFrameNumber = m_pAviPlayer->GetCurrentFrameNumber();
			if( nCurrentFrameNumber == 0){ // first frame
				m_nTimerStart = GetTickCount();
				m_pAviPlayer->m_nFrameSkiped = 0;
			}
			
			nCurrentTimePlayed = nTimerSpeed * m_pAviPlayer->m_nStartFrameNumber;
			nCurrentTimeNeeded = GetTickCount()-m_nTimerStart;
		} 
		while( nCurrentTimePlayed < nCurrentTimeNeeded );

		m_pAviPlayer->m_nFrameSkiped += (i-1);

		Invalidate();
		return;
	} 
	CWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

int CAviPlayerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CAviPlayerWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if( m_pAviPlayer == NULL )
		return;
	m_pAviPlayer->DrawFrame( dc, m_pAviPlayer->m_nFrameNumber );
}

//////////////////////////////////////////////////////////////////////////
