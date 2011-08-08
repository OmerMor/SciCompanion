// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "HelpNotes.h"
#include "ChildView.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE( CChildView, CFormView );

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
	: CExtWS < CExtWA < CExtAFV < CFormView > > > ( CChildView::IDD, (CWnd*) NULL )
{
}

CChildView::~CChildView()
{
}

void CChildView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView)
	DDX_Control(pDX, IDC_RADIO_SHADOW_1, m_radioShadow1);
	DDX_Control(pDX, IDC_RADIO_SHADOW_2, m_radioShadow2);
	DDX_Control(pDX, IDC_RADIO_SHADOW_3, m_radioShadow3);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_BORDER_RB_COLOR, m_checkCustomBorderRbColor);
	DDX_Control(pDX, IDC_BUTTON_CUSTOM_BORDER_RB_COLOR, m_buttonCustomBorderRbColor);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_BORDER_LT_COLOR, m_checkCustomBorderLtColor);
	DDX_Control(pDX, IDC_BUTTON_CUSTOM_BORDER_LT_COLOR, m_buttonCustomBorderLtColor);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_BACKGROUND_COLOR, m_checkCustomBkColor);
	DDX_Control(pDX, IDC_BUTTON_CUSTOM_BACKGROUND_COLOR, m_buttonCustomBkColor);
	DDX_Control(pDX, IDC_STATIC_GROUP1, m_groupBox1);
	DDX_Control(pDX, IDC_STATIC_GROUP2, m_groupBox2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChildView,CFormView)
	//{{AFX_MSG_MAP(CChildView)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_BACKGROUND_COLOR, OnCheckCustomBackgroundColor)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_BORDER_LT_COLOR, OnCheckCustomBorderLtColor)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_BORDER_RB_COLOR, OnCheckCustomBorderRbColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

#ifndef IS_COMMAND_ID
	#define IS_COMMAND_ID( nID ) ( (nID) & 0x8000 )
#endif

BOOL CChildView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( CWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;
	if(		( nCode != CN_COMMAND && nCode != CN_UPDATE_COMMAND_UI )
		||	(! IS_COMMAND_ID( nID ) )
		||	nID >= 0xF000
		)
		return FALSE;
CWinThread * pThread = ::AfxGetThread();
	if(		pThread != NULL
		&&	pThread->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo )
		)
		return TRUE;
	return FALSE;
}

void CChildView::OnFileNew() 
{
	::AfxMessageBox( _T("CChildView::OnFileNew()") );
}

void CChildView::OnFileOpen() 
{
	::AfxMessageBox( _T("CChildView::OnFileOpen()") );
}

void CChildView::OnFileSave() 
{
	::AfxMessageBox( _T("CChildView::OnFileSave()") );
}

void CChildView::OnFileSendMail() 
{
	::AfxMessageBox( _T("CChildView::OnFileSendMail()") );
}

void CChildView::OnFilePrint() 
{
	::AfxMessageBox( _T("CChildView::OnFilePrint()") );
}

void CChildView::OnFilePrintPreview() 
{
	::AfxMessageBox( _T("CChildView::OnFilePrintPreview()") );
}

void CChildView::OnEditUndo() 
{
	::AfxMessageBox( _T("CChildView::OnEditUndo()") );
}

void CChildView::OnEditCopy() 
{
	::AfxMessageBox( _T("CChildView::OnEditCopy()") );
}

void CChildView::OnEditCut() 
{
	::AfxMessageBox( _T("CChildView::OnEditCut()") );
}

void CChildView::OnEditPaste() 
{
	::AfxMessageBox( _T("CChildView::OnEditPaste()") );
}

void CChildView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ShowSizeGrip( FALSE );
	m_groupBox1.SetStyle( CExtGroupBox::STYLE_CAPTION );
	m_groupBox2.SetStyle( CExtGroupBox::STYLE_CAPTION );
	m_radioShadow2.SetCheck( 1 );
	m_buttonCustomBkColor.SetFlat( TRUE );
	m_buttonCustomBorderLtColor.SetFlat( TRUE );
	m_buttonCustomBorderRbColor.SetFlat( TRUE );
	m_buttonCustomBkColor.m_clrDefault
		= m_buttonCustomBkColor.m_clrSelected
		= RGB(224,248,255);
	m_buttonCustomBorderLtColor.m_clrDefault
		= m_buttonCustomBorderLtColor.m_clrSelected
		= m_buttonCustomBorderRbColor.m_clrDefault
		= m_buttonCustomBorderRbColor.m_clrSelected
		= RGB(0,0,0);
	m_buttonCustomBkColor.m_bEnableBtnColorDefault = false;
	m_buttonCustomBorderLtColor.m_bEnableBtnColorDefault = false;
	m_buttonCustomBorderRbColor.m_bEnableBtnColorDefault = false;
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}


void CChildView::OnCheckCustomBackgroundColor() 
{
	m_buttonCustomBkColor.EnableWindow(
		( m_checkCustomBkColor.GetCheck() != 0 )
			? TRUE : FALSE
		);
}

void CChildView::OnCheckCustomBorderLtColor() 
{
	m_buttonCustomBorderLtColor.EnableWindow(
		( m_checkCustomBorderLtColor.GetCheck() != 0 )
			? TRUE : FALSE
		);
}

void CChildView::OnCheckCustomBorderRbColor() 
{
	m_buttonCustomBorderRbColor.EnableWindow(
		( m_checkCustomBorderRbColor.GetCheck() != 0 )
			? TRUE : FALSE
		);
}
