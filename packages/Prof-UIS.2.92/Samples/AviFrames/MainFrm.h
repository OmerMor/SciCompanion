// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildFormView.h"

//#define ID_MYEXTBTN_SLIDER 5005
#define ID_MYEXTBTN_SCROLLER 5006

#define GetMainFrame() (DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd())) 

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

class CMainFrame
	: public CExtNCW < CFrameWnd >
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	, public CExtCustomizeSite
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
{
public:
	CMainFrame();
	DECLARE_DYNAMIC(CMainFrame)
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtBarButton *  OnCreateToolbarButton(
		CExtToolControlBar * pBar,
		CExtCustomizeCmdTreeNode * pNodeI,
		CExtCustomizeCmdTreeNode * pNodeC
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

// Attributes
public:
	CAviPlayer *m_pAviPlayer;
	CAviView  *m_pWndView;
	
private:
	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CExtMenuControlBar    m_wndMenuBar;
	CExtToolControlBar    m_wndToolBar;
	CExtThemeSwitcherToolControlBar    m_wndToolBarUiLook;
	CExtToolControlBar    m_wndToolBarPlayer;
	CExtToolControlBar    m_wndToolBarSeek;
	CExtToolControlBar    m_wndToolBarZoom;

public:

	CExtStatusControlBar  m_wndStatusBar;
	
	class CZoomBarSliderButton: public CExtBarSliderButton
	{
		static BOOL g_bSynchronizing; 
	public:
		static CTypedPtrArray< CPtrArray, CZoomBarSliderButton* > g_arrButtons;
		CZoomBarSliderButton(
			CExtToolControlBar * pBar = NULL,
			UINT nCmdID = ID_SEPARATOR,
			UINT nStyle = 0,
			ULONG nScrollTotalRange = 0L,
			ULONG nScrollPos = 0L,
			ULONG nScrollPageSize = 0L,
			INT nScrollButtonExtentH = 0,
			INT nScrollButtonExtentV = 0,
			INT nScrollControlExtentH = 50,
			INT nScrollControlExtentV = 50
			)
			: CExtBarSliderButton(
			pBar,
			nCmdID,
			nStyle,
			nScrollTotalRange,
			nScrollPos,
			nScrollPageSize,
			nScrollButtonExtentH,
			nScrollButtonExtentV,
			nScrollControlExtentH,
			nScrollControlExtentV
			)
		{
			g_arrButtons.Add(this);
		}
		~CZoomBarSliderButton()
		{
			for(int i=0; i< g_arrButtons.GetSize(); i++){
				if( this == g_arrButtons[i] ){
					g_arrButtons.RemoveAt(i);
					break;
				}
			}
		}
			
		ULONG ScrollPosSet( ULONG nScrollPos )
		{
			ULONG ulRet = CExtBarSliderButton::ScrollPosSet( nScrollPos );
			RedrawButton();

			if( g_bSynchronizing )
				return ulRet;
			
			g_bSynchronizing = TRUE;
			
			for(int i=0; i< g_arrButtons.GetSize(); i++){
				CZoomBarSliderButton *pZoomSliderTBB = g_arrButtons[i];
				ASSERT_VALID(pZoomSliderTBB);
				pZoomSliderTBB->ScrollPosSet(nScrollPos);
			}
			
			g_bSynchronizing = FALSE;

			CMainFrame *pFrame = GetMainFrame();
			if(pFrame->m_pAviPlayer){
				int nZoomPercent = 100;
				switch(nScrollPos) {
				case 0:
					nZoomPercent = 10;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("10 %"));
					break;
				case 1:
					nZoomPercent = 25;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("25 %"));
					break;
				case 2:
					nZoomPercent = 50;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("50 %"));
					break;
				case 3:
					nZoomPercent = 75;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("75 %"));
					break;
				case 4:
					nZoomPercent = 100;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("100 %"));
					break;
				case 5:
					nZoomPercent = 150;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("150 %"));
					break;
				case 6:
					nZoomPercent = 200;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("200 %"));
					break;
				case 7:
					nZoomPercent = 250;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("250 %"));
					break;
				case 8:
					nZoomPercent = 300;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("300 %"));
					break;
				case 9:
					nZoomPercent = 350;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("350 %"));
					break;
				case 10:
					nZoomPercent = 400;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("400 %"));
					break;
				case 11:
					nZoomPercent = 450;
					pFrame->m_wndStatusBar.SetPaneText(1,_T("450 %"));
					break;
				}
				pFrame->m_pAviPlayer->SetZoom( nZoomPercent );
				if( !pFrame->m_pAviPlayer->IsPlaying() )
					pFrame->m_pWndView->Invalidate();
				pFrame->m_pWndView->OnSwUpdateScrollBars();
			}		
			return ulRet;
		}
	}; // class CZoomBarSliderButton
	
	
	class CSeekBarSliderButton: public CExtBarSliderButton
	{
		static BOOL g_bSynchronizing; 
	public:
		static CTypedPtrArray< CPtrArray, CSeekBarSliderButton* > g_arrButtons;
		CSeekBarSliderButton(
			CExtToolControlBar * pBar = NULL,
			UINT nCmdID = ID_SEPARATOR,
			UINT nStyle = 0,
			ULONG nScrollTotalRange = 0L,
			ULONG nScrollPos = 0L,
			ULONG nScrollPageSize = 0L,
			INT nScrollButtonExtentH = 0,
			INT nScrollButtonExtentV = 0,
			INT nScrollControlExtentH = 50,
			INT nScrollControlExtentV = 50
			)
			: CExtBarSliderButton(
			pBar,
			nCmdID,
			nStyle,
			nScrollTotalRange,
			nScrollPos,
			nScrollPageSize,
			nScrollButtonExtentH,
			nScrollButtonExtentV,
			nScrollControlExtentH,
			nScrollControlExtentV
			)
		{
			g_bSynchronizing = FALSE;
			g_arrButtons.Add(this);
		}
		~CSeekBarSliderButton()
		{
			for(int i=0; i< g_arrButtons.GetSize(); i++){
				if( this == g_arrButtons[i] ){
					g_arrButtons.RemoveAt(i);
					break;
				}
			}
		}
		
		ULONG ScrollPosSet( ULONG nScrollPos)
		{
			ULONG ulRet = CExtBarSliderButton::ScrollPosSet( nScrollPos );
			RedrawButton();
			
			if( g_bSynchronizing )
				return ulRet;

			g_bSynchronizing = TRUE;
			
			for(int i=0; i < g_arrButtons.GetSize(); i++){
				CSeekBarSliderButton *pSeekScrollerTBB = g_arrButtons[i];
				ASSERT_VALID(pSeekScrollerTBB);
				pSeekScrollerTBB->ScrollPosSet( nScrollPos );
			}

			g_bSynchronizing = FALSE;

			CMainFrame *pFrame = GetMainFrame();
			if(pFrame->m_pAviPlayer){
				pFrame->m_pAviPlayer->SetCurrentFrameNumber( nScrollPos );
				if( !pFrame->m_pAviPlayer->IsPlaying() && !pFrame->m_pAviPlayer->GetTempPause())
					pFrame->m_pWndView->Invalidate();
			}

			return ulRet;
		}

		virtual void OnClick(
			CPoint point,
			bool bDown
			)
		{
			CMainFrame *pFrame = GetMainFrame();
			pFrame->m_pAviPlayer->SetTempPause(TRUE);
			CExtBarSliderButton::OnClick(point,bDown);
			pFrame->m_pAviPlayer->SetTempPause(FALSE);
			pFrame->m_pAviPlayer->m_nStartFrameNumber = 0;
			pFrame->m_pWndView->Invalidate();
		}
		
	};// class CSeekBarSliderButton

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnPlay();
	afx_msg void OnUpdatePlay(CCmdUI* pCmdUI);
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI* pCmdUI);
	afx_msg void OnPause();
	afx_msg void OnUpdatePause(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnSlider();
	afx_msg void OnScroller();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
