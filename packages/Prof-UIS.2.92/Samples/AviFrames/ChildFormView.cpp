// ChildFormView.cpp : implementation file
//

#include "stdafx.h"
#include "AviFrames.h"
#include "ChildFormView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GetMainFrame() (DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd())) 

/////////////////////////////////////////////////////////////////////////////
// CAviView

IMPLEMENT_DYNAMIC( CAviView, CExtScrollWnd )

CAviView::CAviView()
{
	//{{AFX_DATA_INIT(CAviView)
	//}}AFX_DATA_INIT
	m_bInitComplete = false;
}

CAviView::~CAviView()
{
}

BEGIN_MESSAGE_MAP( CAviView, CExtScrollWnd )
	//{{AFX_MSG_MAP(CAviView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void CAviView::PostNcDestroy() 
{
	CExtNSB < CExtScrollWnd > :: PostNcDestroy();
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CAviView diagnostics

#ifdef _DEBUG
void CAviView::AssertValid() const
{
	CExtNSB < CExtScrollWnd > :: AssertValid();
}

void CAviView::Dump(CDumpContext& dc) const
{
	CExtNSB < CExtScrollWnd > :: Dump(dc);
}
#endif //_DEBUG

void CAviView::_AdjustCornerBetweenScrollBars()
{
	ASSERT_VALID( this );
	if( m_wndLabelCorner.GetSafeHwnd() == NULL )
		return;
CRect rcCorner( 0, 0, 0, 0 );
CScrollBar * pSBH = GetScrollBarCtrl( SB_HORZ );
	if( pSBH->GetSafeHwnd() != NULL )
	{
		CScrollBar * pSBV = GetScrollBarCtrl( SB_VERT );
		if( pSBV->GetSafeHwnd() != NULL )
		{
			if(		( pSBH->GetStyle() & WS_VISIBLE ) != 0
				&&	( pSBV->GetStyle() & WS_VISIBLE ) != 0
				)
			{
				CRect rcSBH, rcSBV;
				pSBH->GetWindowRect( &rcSBH );
				pSBV->GetWindowRect( &rcSBV );
				rcCorner.left = rcSBV.left;
				rcCorner.right = rcSBV.right;
				rcCorner.top = rcSBH.top;
				rcCorner.bottom = rcSBH.bottom;
				ScreenToClient( &rcCorner );
			}
		}
	}
UINT nSWPF = SWP_NOOWNERZORDER | ( rcCorner.IsRectEmpty() ? SWP_HIDEWINDOW : SWP_SHOWWINDOW ) ;
	m_wndLabelCorner.SetWindowPos( pSBH, rcCorner.left, rcCorner.top, rcCorner.Width(), rcCorner.Height(), nSWPF );
}

bool CAviView::Create(
			CWnd * pWndParent,
			UINT nDlgCtrlID, // = AFX_IDW_PANE_FIRST
			CRect rc, // = CRect( 0,0,0,0 )
			DWORD dwStyle, // = WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
			DWORD dwStyleEx // = 0L
			)
{
//	dwStyleEx |= WS_EX_CLIENTEDGE;

LPCTSTR strClass =
		::AfxRegisterWndClass(
			CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
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

//////////////////////////////////////////////////////////////////////////

int CAviView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CExtNSB < CExtScrollWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;
	if( ! m_wndLabelCorner.Create( _T(""), WS_CHILD|WS_CLIPSIBLINGS, CRect(0,0,0,0), this ) )
		return -1;
	m_bInitComplete = true;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CAviView message handlers
//////////////////////////////////////////////////////////////////////////

CSize CAviView::OnSwGetTotalSize() const
{
	if( GetMainFrame()->m_pAviPlayer != NULL )
		return GetMainFrame()->m_pAviPlayer->GetAviRenderSize();
	return CSize(0,0);
}

//////////////////////////////////////////////////////////////////////////

bool CAviView::OnSwDoScrollBy(
	CSize sizeScroll,
	bool bDoScroll // = true
	)
{
	CExtNSB < CExtScrollWnd > :: OnSwDoScrollBy( sizeScroll, bDoScroll );
	
	if(!GetMainFrame()->m_pAviPlayer)
		return true;
	
	LONG lPosHorz = ScrollPos32Get( SB_HORZ );
	LONG lPosVert = ScrollPos32Get( SB_VERT );
	
	CSize szAvi = GetMainFrame()->m_pAviPlayer->GetAviRenderSize();
	GetMainFrame()->m_pAviPlayer->SetPlayerWindowPos(
		-lPosHorz,
		-lPosVert,
		szAvi.cx,
		szAvi.cy
		);
	_AdjustCornerBetweenScrollBars();

	return true;
}

//////////////////////////////////////////////////////////////////////////

CRect CAviView::OnSwRecalcLayout(
					   bool bDoLayout,
					   LPCRECT pRectClientSrc )
{
CRect rc = CExtNSB < CExtScrollWnd > :: OnSwRecalcLayout( bDoLayout, pRectClientSrc ); 
	if( ! bDoLayout )
		return rc;
	
	if( ! GetMainFrame()->m_pAviPlayer )
		return rc;
	
LONG lPosHorz = ScrollPos32Get( SB_HORZ );
LONG lPosVert = ScrollPos32Get( SB_VERT );
	
	CSize szAvi = GetMainFrame()->m_pAviPlayer->GetAviRenderSize();
	GetMainFrame()->m_pAviPlayer->SetPlayerWindowPos(
		-lPosHorz,
		-lPosVert,
		szAvi.cx,
		szAvi.cy
		);	
	_AdjustCornerBetweenScrollBars();

	return rc;
}

