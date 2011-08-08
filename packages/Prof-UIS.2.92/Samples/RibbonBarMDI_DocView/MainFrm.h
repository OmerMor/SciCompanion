// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ID_RIBBON_PAGE_CLIPBOARD                                      9000
#define ID_RIBBON_PAGE_FONT                                           9001
#define ID_RIBBON_PAGE_PARAGRAPH                                      9002
#define ID_MAILINGS_RULES                                             9003
#define ID_MAILINGS_GOTO_RECORD                                       9004
#define ID_TRANSLATION_TOOL_TIP                                       9005
#define ID_SHOW_HIDE_RULER                                            9006
#define ID_SHOW_HIDE_GRIDLINES                                        9007
#define ID_SHOW_HIDE_PROPERTIES                                       9008
#define ID_SHOW_HIDE_DOCUMENT_MAP                                     9009
#define ID_SHOW_HIDE_THUMBNAILS                                       9010
#define ID_SHOW_HIDE_DOCUMENT_ACTION_BAR                              9011
#define ID_RIBBON_PAGE_STYLE                                          9012
#define ID_RIBBON_PAGE_INSERT_PAGES                                   9013
#define ID_RIBBON_PAGE_INSERT_TABLE                                   9014
#define ID_RIBBON_PAGE_INSERT_ILLUSTRATIONS                           9015
#define ID_RIBBON_PAGE_INSERT_LINKS                                   9016
#define ID_RIBBON_PAGE_INSERT_HEADER_AND_FOOTER                       9017
#define ID_RIBBON_PAGE_INSERT_TEXT                                    9018
#define ID_RIBBON_PAGE_INSERT_SYMBOLS                                 9019
#define ID_RIBBON_PAGE_PL_THEMES                                      9020
#define ID_RIBBON_PAGE_PL_PAGE_SETUP                                  9021
#define ID_RIBBON_PAGE_PL_PAGE_BACKGROUND                             9022
#define ID_RIBBON_PAGE_PL_PARAGRAPH                                   9023
#define ID_RIBBON_PAGE_PL_ARRANGE                                     9024
#define ID_RIBBON_PAGE_REF_TOC                                        9025
#define ID_RIBBON_PAGE_REF_FOOTNOTES                                  9026
#define ID_RIBBON_PAGE_REF_CB                                         9027
#define ID_RIBBON_PAGE_REF_CAPTIONS                                   9028
#define ID_RIBBON_PAGE_REF_INDEX                                      9029
#define ID_RIBBON_PAGE_REF_TOA                                        9030
#define ID_RIBBON_PAGE_MAIL_CREATE                                    9031
#define ID_RIBBON_PAGE_MAIL_SMM                                       9032
#define ID_RIBBON_PAGE_MAIL_WIF                                       9033
#define ID_RIBBON_PAGE_MAIL_PREVIEW_RESULTS                           9034
#define ID_RIBBON_PAGE_MAIL_FINISH                                    9035
#define ID_RIBBON_PAGE_REVIEW_PROOFING                                9036
#define ID_RIBBON_PAGE_REVIEW_COMMENTS                                9037
#define ID_RIBBON_PAGE_REVIEW_TRACKING                                9038
#define ID_RIBBON_PAGE_REVIEW_CHANGES                                 9039
#define ID_RIBBON_PAGE_REVIEW_COMPARE                                 9040
#define ID_RIBBON_PAGE_REVIEW_PROTECT                                 9041
#define ID_RIBBON_PAGE_VIEW_DV                                        9042
#define ID_RIBBON_PAGE_VIEW_SHOW_HIDE                                 9043
#define ID_RIBBON_PAGE_VIEW_ZOOM                                      9044
#define ID_RIBBON_PAGE_VIEW_WINDOW                                    9044
#define ID_FB_FINISH_RESTRICT_PERMISSION_UNRESTRICTED_ACCESS          9044
#define ID_FB_FINISH_RESTRICT_PERMISSION_DO_NOT_DISTRIBUTE            9045
#define ID_FB_FINISH_RESTRICT_PERMISSION_AS                           9046
#define ID_SAVE_SELECTION_AS_NEW_QUICK_STYLE                          9047
#define ID_CLEAR_FORMATTING                                           9048
#define ID_APPLY_STYLES                                               9049
#define ID_FONT_MORE_UNDERLINES                                       9050
#define ID_FONT_CASE_SENTENCE                                         9051
#define ID_FONT_CASE_LOWER                                            9052
#define ID_FONT_CASE_UPPPER                                           9053
#define ID_FONT_CASE_CAPITALIZE_EACH_WORD                             9054
#define ID_FONT_CASE_TOGGLE                                           9055
#define ID_EDIT_PASTE_AS_HYPERLINK                                    9056
#define ID_PARAGRAPH_LINE_SPACING_10                                  9057
#define ID_PARAGRAPH_LINE_SPACING_15                                  9058
#define ID_PARAGRAPH_LINE_SPACING_20                                  9059
#define ID_PARAGRAPH_LINE_SPACING_25                                  9060
#define ID_PARAGRAPH_LINE_SPACING_30                                  9061
#define ID_PARAGRAPH_LINE_SPACING_MORE                                9062
#define ID_PARAGRAPH_LINE_SPACING_SPACE_BEFORE                        9063
#define ID_PARAGRAPH_LINE_SPACING_NO_SPACE_AFTER                      9064
#define ID_DEFINE_NEW_BULLET                                          9065
#define ID_DEFINE_NEW_MULTILEVEL_LIST                                 9066
#define ID_DEFINE_NEW_LIST_STYLE                                      9067
#define ID_SE_FONT_LIST                                               9068
#define ID_SE_SIZE                                                    9069
#define ID_RTL                                                        9070

#include "ChildView.h"

#if !defined(__EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif

#define ID_WINDOWS_LIST ID_WINDOW_SWITCH_LARGE

/////////////////////////////////////////////////////////////////////////////
// CMyUserDefinedRibbonOptionsPage

class CMyUserDefinedRibbonOptionsPage : public CExtRibbonOptionsPage
{
public:
	DECLARE_DYNCREATE( CMyUserDefinedRibbonOptionsPage );

	CMyUserDefinedRibbonOptionsPage();
	virtual ~CMyUserDefinedRibbonOptionsPage();
	virtual UINT RibbonOptionsPage_GetID() const;
	virtual bool RibbonOptionsPage_InitContent();
	virtual void RibbonOptionsPage_Apply();
	virtual void RibbonOptionsPage_Cancel();

// Attributes
public:
// Dialog Data
	//{{AFX_DATA(CMyUserDefinedRibbonOptionsPage)
	CExtLabel m_label;
	//}}AFX_DATA

// Implementation
protected:
    //{{AFX_VIRTUAL(CExtRibbonBar)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CMyUserDefinedRibbonOptionsPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CMyUserDefinedRibbonOptionsPage

// comment this line to remove backstage view
#define __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__

#if (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)

class CMyBackstageViewWnd : public CExtRibbonBackstageViewWnd
{
public:
	CMyBackstageViewWnd();
	virtual ~CMyBackstageViewWnd();
	virtual bool OnBsvInitContent();
	void _InitBackStageView_Resources();
	void _InitBackStageView_Tree();
	virtual void OnBsvItemInvokeDropDownMenu( HTREEITEM hti, e_item_invoke_type eIIT );
}; // class CMyBackstageViewWnd

#endif // (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)

/////////////////////////////////////////////////////////////////////////////
// CMyRibbonBar

#define __SO_FONT_SIZE_MIN	4.0f
#define __SO_FONT_SIZE_MAX	200.f

class CMyRibbonBar : public CExtRibbonBar
{
public:
#if (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)
	CMyBackstageViewWnd m_BSV;
#endif // (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)

	HICON m_hIconPara, m_hIconFontTT, m_hIconFontPix;
	CString m_sDecimalSeparator, m_sCurrentFontFaceName;
	float m_lfCurrentFontSize;

	COLORREF m_clrFontColor;
	COLORREF m_clrTextHighlightColor;

	CMyRibbonBar();
	virtual ~CMyRibbonBar();

	virtual void OnRibbonGalleryInitContent(
		CExtRibbonGalleryWnd & wndRG,
		CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
		CExtRibbonButtonGallery * pRibbonGalleryTBB
		);
	virtual void OnRibbonOptionsDialogInitPageRTCs(
		CExtRibbonOptionsDialog & dlgRibbonOptions
		);

	virtual bool OnPopupListBoxGetStyles(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		DWORD & dwListBoxStyles
		) const;
	virtual bool OnPopupListBoxInitContent(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox
		);
	virtual CSize OnPopupListBoxMeasureTrackSize(
		const CExtBarButton * pTBB,
		const CExtCustomizeCmdTreeNode * pNode
		) const;
	virtual bool OnPopupListBoxItemDraw(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox,
		LPDRAWITEMSTRUCT pDIS
		);
	virtual bool OnPopupListBoxItemMeasure(
		CExtBarButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CListBox & wndListBox,
		LPMEASUREITEMSTRUCT pMIS
		);
	static int CALLBACK stat_FillFontListBoxProc(
		ENUMLOGFONTEX * pEnumLogFontEx,
		NEWTEXTMETRICEX * pNewTextMetriCEx, 
		int nFontType,
		CListBox * pListBox
		);
	virtual bool OnTextFieldVerify(
		CExtBarTextFieldButton * pTextFieldTBB,
		CExtCustomizeCmdTreeNode * pNode,
		__EXT_MFC_SAFE_LPCTSTR sTextOld,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);
	virtual void OnTextFieldInplaceTextGet(
		const CExtBarTextFieldButton * pTextFieldTBB,
		const CExtCustomizeCmdTreeNode * pNode,
		CExtSafeString & sTextFieldBuffer
		);
	virtual void OnTextFieldInplaceTextSet(
		CExtBarTextFieldButton * pTextFieldTBB,
		CExtCustomizeCmdTreeNode * pNode,
		CExtSafeString & sTextFieldBuffer,
		__EXT_MFC_SAFE_LPCTSTR sTextNew
		);

	virtual bool OnColorItemValueGet(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF & clr,
		bool bSelected
		);
	virtual bool OnColorItemValueSet(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF & clr,
		bool bSelected
		);
	virtual bool OnColorItemCustom(
		CExtCustomizeCmdTreeNode * pNode
		);
	virtual CExtBarButton * OnCreateToolbarButton(
		CExtToolControlBar * pBar,
		CExtCustomizeCmdTreeNode * pNodeI,
		CExtCustomizeCmdTreeNode * pNodeC
		);
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

class CMainFrame : public CExtNCW < CMDIFrameWnd >
{
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

#ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
	
	class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg
	{
	public:
		CMyMdiWindowsListDlg(
				CMDIFrameWnd * pMDIFrameWnd,
				CWnd * pParent = NULL
				)
			: CExtMdiWindowsListDlg(
				pMDIFrameWnd,
				pParent
				)
		{
		}
		CMyMdiWindowsListDlg(
				HWND hMDIFrameWnd,
				CWnd * pParent = NULL
				)
			: CExtMdiWindowsListDlg(
				hMDIFrameWnd,
				pParent
				)
		{
		}
	protected:
		virtual void OnUpdateButtons()
		{
			CExtMdiWindowsListDlg::OnUpdateButtons();
			m_btnSave.ShowWindow(SW_HIDE);
		}
	}; // class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg
	
#endif // #ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	

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
	virtual void RecalcLayout(BOOL bNotify = TRUE);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	class CMDITabs : public CExtTabMdiWnd
	{
	protected:
		bool OnHookWndMsg( LRESULT & lResult, HWND hWndHooked, UINT nMessage, WPARAM & wParam, LPARAM & lParam )
		{
			__PROF_UIS_MANAGE_STATE;
			if( hWndHooked == _GetHwndMdiArea() )
			{
				switch( nMessage )
				{
				case WM_ERASEBKGND:
					lResult = 1L;
					return true;
				case WM_PAINT:
					{
						CWnd * pWnd = CWnd::FromHandle( hWndHooked );
						CPaintDC dc( pWnd );
						CRect rcClient;
						::GetClientRect( hWndHooked, &rcClient );
						if( ! g_PaintManager->PaintDockerBkgnd(
								true,
								dc,
								pWnd
								)
							)
							dc.FillSolidRect(
								&rcClient,
								g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT )
								);
					}
					lResult = 1L;
					return true;
				case WM_NCCALCSIZE:
					lResult = 0L;
					return true;
				} // switch( nMessage )
			} // if( hWndHooked == _GetHwndMdiArea() )
			return CExtTabMdiWnd::OnHookWndMsg( lResult, hWndHooked, nMessage, wParam, lParam );
		}
	};

protected:  // control bar embedded members
	CMyRibbonBar                      m_wndRibbonBar;
	CExtStatusControlBar              m_wndStatusBar;
//	CExtMenuControlBar                m_wndMenuBar;
	CExtToolControlBar                m_wndToolBar;
	CExtThemeSwitcherToolControlBar   m_wndToolBarUiLook;
	CExtZoomScrollBar                 m_wndZoomScrollBar;
	CMDITabs                          m_wndMdiTabs;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()

	void _AdjustRTL();

	CExtRibbonNode * m_pRibbonNode;
	void _InitRibbonBar();
	CExtRibbonNodeRightButtonsCollection * _InitRibbon_RightButtonsCollection();
	CExtRibbonNodeQuickAccessButtonsCollection * _InitRibbon_QuickAccessButtonsCollection();
	virtual void NcFrameImpl_SyncThemeSettings( CExtPaintManager * pPM, bool bPostNotification );
	CExtRibbonNodeFile * _InitRibbon_FileMenu();

	CExtRibbonNode * _InitRibbonPage_Home();
	CExtRibbonNode * _InitRibbonNode_Home_Clipboard();
	CExtRibbonNode * _InitRibbonNode_Home_Font();
	CExtRibbonNode * _InitRibbonNode_Home_Paragraph();
	CExtRibbonNode * _InitRibbonNode_Home_Style();
	CExtRibbonNode * _InitRibbonNode_Home_Find();

	CExtRibbonNode * _InitRibbonPage_Insert();
	CExtRibbonNode * _InitRibbonNode_Insert_Pages();
	CExtRibbonNode * _InitRibbonNode_Insert_Table();
	CExtRibbonNode * _InitRibbonNode_Insert_Illustrations();
	CExtRibbonNode * _InitRibbonNode_Insert_Links();
	CExtRibbonNode * _InitRibbonNode_Insert_HeaderAndFooter();
	CExtRibbonNode * _InitRibbonNode_Insert_Text();
	CExtRibbonNode * _InitRibbonNode_Insert_Symbols();

	CExtRibbonNode * _InitRibbonPage_PageLayout();
	CExtRibbonNode * _InitRibbonNode_PageLayout_Themes();
	CExtRibbonNode * _InitRibbonNode_PageLayout_PageSetup();
	CExtRibbonNode * _InitRibbonNode_PageLayout_PageBackground();
	CExtRibbonNode * _InitRibbonNode_PageLayout_Paragraph();
	CExtRibbonNode * _InitRibbonNode_PageLayout_Arrange();

	CExtRibbonNode * _InitRibbonPage_References();
	CExtRibbonNode * _InitRibbonNode_References_TableOfContents();
	CExtRibbonNode * _InitRibbonNode_References_FootNotes();
	CExtRibbonNode * _InitRibbonNode_References_CitationsAndBibliography();
	CExtRibbonNode * _InitRibbonNode_References_Captions();
	CExtRibbonNode * _InitRibbonNode_References_Index();
	CExtRibbonNode * _InitRibbonNode_References_TableOfAuthorities();

	CExtRibbonNode * _InitRibbonPage_Mailings();
	CExtRibbonNode * _InitRibbonNode_Mailings_Create();
	CExtRibbonNode * _InitRibbonNode_Mailings_StartMailMerge();
	CExtRibbonNode * _InitRibbonNode_Mailings_WriteAndInsertFields();
	CExtRibbonNode * _InitRibbonNode_Mailings_PreviewResults();
	CExtRibbonNode * _InitRibbonNode_Mailings_Finish();

	CExtRibbonNode * _InitRibbonPage_Review();
	CExtRibbonNode * _InitRibbonNode_Review_Proofing();
	CExtRibbonNode * _InitRibbonNode_Review_Comments();
	CExtRibbonNode * _InitRibbonNode_Review_Tracking();
	CExtRibbonNode * _InitRibbonNode_Review_Changes();
	CExtRibbonNode * _InitRibbonNode_Review_Compare();
	CExtRibbonNode * _InitRibbonNode_Review_Protect();

	CExtRibbonNode * _InitRibbonPage_View();
	CExtRibbonNode * _InitRibbonNode_View_DocumentViews();
	CExtRibbonNode * _InitRibbonNode_View_ShowHide();
	CExtRibbonNode * _InitRibbonNode_View_Zoom();
	CExtRibbonNode * _InitRibbonNode_View_Window();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
