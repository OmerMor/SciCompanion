// MainDlg.h : header file
//

#if !defined(AFX_MAINDLG_H__6F25FADE_3D19_4371_A550_B74561E7B764__INCLUDED_)
#define AFX_MAINDLG_H__6F25FADE_3D19_4371_A550_B74561E7B764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageNotAvailable.h"
#include "PageTabWindow.h"
#include "PageTabContainers.h"
#include "PageButtons.h"
#include "PageHyperLinks.h"
#include "PagePopupMenus.h"
#include "PageStatusBar.h"
#include "PageToolbars.h"
#include "PageCalendar.h"
#include "PageDateTimeDuration.h"
#include "PageGrid.h"
#include "PageMaskedEdit.h"
#include "PageTreeCtrl.h"
#include "PageListCtrl.h"
#include "PageShellTreeCtrl.h"
#include "PageShellListCtrl.h"
#include "PageShellBrowser.h"
#include "PageShellDialogFile.h"
#include "PageShellDialogBrowseFor.h"

#include "IconListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd

class CSplashWnd : public CWnd
{
protected:
	CWnd m_wndInvisibleParent;
	CBitmap m_bitmap;
	CSize m_sizeBitmap;
	CStringArray m_arrTextLines;
// Construction
public:

	CSplashWnd(
		CWnd * pWndParent = CWnd::GetDesktopWindow(),
		UINT nBitmapID = IDB_BITMAP_SPLASH
		);

// Attributes
public:
	CFont m_font;
	INT m_nMaxTextLines, m_nLineHeight;
	COLORREF m_clrText;
	CRect m_rcText;

// Operations
public:
	bool Create(
		CWnd * pWndParent,
		UINT nBitmapID
		);

	static bool RegisterSplashWndClass();

	void AddTextLine(
		LPCTSTR sText = NULL
		);
	void ReplaceLastLine(
		LPCTSTR sText = NULL
		);
	void ClearLines();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplashWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplashWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define __SIMPLE_SPLASH_WINDOW_CLASS_NAME__ _T("__SIMPLE_SPLASH_WINDOW_CLASS_NAME__")

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CExtNCW < CExtResizableDialog >
{
	bool m_bDialogInitComplete;

	// Construction
public:
	DECLARE_DYNAMIC(CMainDlg);
	CMainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_PROFUIS_CONTROLS_DIALOG };
	//}}AFX_DATA
	CListBoxMenuLike m_wndList;
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	CExtMenuControlBar m_wndMenuBar;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;

// Implementation
public:
	virtual ~CMainDlg();

protected:
	HICON m_hIcon;
	void _PrepareFavoritesList( IShellFolder * pFolder, UINT * pnMenuID, CExtPopupMenuWnd * pPopup, INT * pnPos );
	STDMETHOD(StrRetToStr)( STRRET StrRet, LPTSTR * str, LPITEMIDLIST pidl );
	STDMETHOD(ResolveChannel)( IShellFolder * pFolder, LPCITEMIDLIST pidl, LPTSTR * lpszURL );
	STDMETHOD(ResolveInternetShortcut)( LPCTSTR lpszLinkFile, LPTSTR * lpszURL );
	STDMETHOD(ResolveLink)( LPCTSTR lpszLinkFile, LPTSTR * lpszPath );
	STDMETHOD(IDLFromPath)( CExtSafeString strPath, LPITEMIDLIST * ppidl );
	IMalloc * m_pMalloc;

	typedef CMap < UINT, UINT, CString, CString > ID_to_URL_t;
	ID_to_URL_t m_mapURLs;

	CPageNotAvailable * m_pPageNotAvailable;
#if (!defined __EXT_MFC_NO_TAB_CTRL)
	CPageTabWindow * m_pPageTabWindows;
#endif
#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )
	CPageTabContainers * m_pPageTabContainers;
#endif
	CPageButtons * m_pPageButtons;
	CPageHyperLinks * m_pPageHyperLinks;
	CPageAviLabel * m_pPageAviLabel;
	CPagePopupMenus * m_pPagePopupMenus;
	CPageStatusBar * m_pPageStatusBar;
	CPageToolbars * m_pPageToolbars;
#if (!defined __EXT_MFC_NO_DATE_PICKER)
	CPageCalendar * m_pPageCalendar;
#endif
#if (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )
	CPageDateTimeDuration * m_pPageDateTimeDuration;
#endif
#if (!defined __EXT_MFC_NO_DATE_BROWSER )
	CPageDateBrowser * m_pPageDateBrowser;
#endif
#if (!defined __EXT_MFC_NO_GRIDWND)
	CPageGrid * m_pPageGrid;
	CPageMaskedEdit * m_pPageMaskedEdit;
#endif
	CPageSystemNumberCurrencyEditors * m_pPageSystemNumberCurrencyEdit;
#if ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )
	CPageListCtrl * m_pPageListCtrl;
#endif
#if ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )
	CPageTreeCtrl * m_pPageTreeCtrl;
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )
	CPageShellListCtrl * m_pPageShellListCtrl;
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )
	CPageShellTreeCtrl * m_pPageShellTreeCtrl;
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )
	CPageShellBrowser * m_pPageShellBrowser;
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )
	CPageShellDialogFile * m_pPageShellDialogFile;
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )
	CPageShellDialogBrowseFor * m_pPageShellDialogBrowseFor;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAppAbout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeList();
	afx_msg void OnBack();
	afx_msg void OnNext();
	afx_msg void OnFavoritesAdd();
	afx_msg void OnFavoritesOrganize();
	//}}AFX_MSG

	afx_msg LRESULT OnExtMenuPrepareLevel( WPARAM wParam, LPARAM lParam );
	afx_msg void OnFavoritesURL( UINT nID );

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__6F25FADE_3D19_4371_A550_B74561E7B764__INCLUDED_)
