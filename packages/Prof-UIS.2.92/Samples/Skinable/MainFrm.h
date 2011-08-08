// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__205C43B5_15CF_429F_8CA0_07C395066CE5__INCLUDED_)
#define AFX_MAINFRM_H__205C43B5_15CF_429F_8CA0_07C395066CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(__EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogThemes

class CDialogThemes
	: public CExtResizableDialog
{
public:
	CDialogThemes();
	virtual ~CDialogThemes();
	bool CreateInsideBar(
		CExtControlBar * pBar
		);
	void InvokeMainFrameCommand( UINT nCmdID );

	//{{AFX_DATA(CDialogThemes)
	enum { IDD = IDD_DIALOG_THEMES };
	CExtHyperLinkButton	m_wndButtonInstallTheme2005;
	CExtHyperLinkButton	m_wndButtonInstallTheme2003;
	CExtHyperLinkButton	m_wndButtonInstallThemeXP;
	CExtHyperLinkButton	m_wndButtonInstallTheme2000;
	CExtHyperLinkButton	m_wndButtonLoadThemeBIN;
	CExtHyperLinkButton	m_wndButtonLoadThemeXML;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDialogThemes)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDialogThemes)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLoadThemeXml();
	afx_msg void OnButtonLoadThemeBin();
	afx_msg void OnButtonTheme2005();
	afx_msg void OnButtonTheme2003();
	afx_msg void OnButtonThemeXp();
	afx_msg void OnButtonTheme2000();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDialogInvokeModalDialogs

class CDialogInvokeModalDialogs
	: public CExtResizableDialog
{
public:
	CDialogInvokeModalDialogs();
	virtual ~CDialogInvokeModalDialogs();
	bool CreateInsideBar(
		CExtControlBar * pBar
		);

	//{{AFX_DATA(CDialogInvokeModalDialogs)
	enum { IDD = IDD_DIALOG_INVOKE_MODAL_DIALOGS };
	CExtHyperLinkButton		m_wndButtonInvokeDialogIconEditor;
	CExtHyperLinkButton		m_wndButtonInvokeDialogColor;
	CExtHyperLinkButton		m_wndButtonInvokeDialogAbout;
	CExtHyperLinkButton		m_wndButtonInvokeFileDialog;
	CExtHyperLinkButton		m_wndButtonInvokeBffDialog;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDialogInvokeModalDialogs)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDialogInvokeModalDialogs)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInvokeColorDialog();
	afx_msg void OnButtonInvokeIconEditorDialog();
	afx_msg void OnButtonInvokeAboutDialog();
	afx_msg void OnButtonInvokeFileDialog();
	afx_msg void OnButtonInvokeBffDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDialogInputFields

class CDialogInputFields
	: public CExtResizableDialog
{
public:
	CDialogInputFields();
	virtual ~CDialogInputFields();
	bool CreateInsideBar(
		CExtControlBar * pBar
		);

	//{{AFX_DATA(CDialogInputFields)
	enum { IDD = IDD_DIALOG_INPUT_FIELDS };
	CExtIPAddressCtrl	m_wndIPAddress2;
	CExtIPAddressCtrl	m_wndIPAddress1;
	CExtSpinWnd		m_wndSpin6;
	CExtSpinWnd		m_wndSpin5;
	CExtSpinWnd		m_wndSpin4;
	CExtSpinWnd		m_wndSpin3;
	CExtEdit		m_wndEdit6;
	CExtEdit		m_wndEdit5;
	CExtEdit		m_wndEdit4;
	CExtEdit		m_wndEdit3;
	CExtEdit		m_wndEdit2;
	CExtEdit		m_wndEdit1;
	CExtComboBox	m_wndCombo4;
	CExtComboBox	m_wndCombo3;
	CExtComboBox	m_wndCombo2;
	CExtComboBox	m_wndCombo1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDialogInputFields)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDialogInputFields)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#if (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)

/////////////////////////////////////////////////////////////////////////////
// CDialogDateTimeDuration dialog

class CDialogDateTimeDuration : public CExtResizableDialog
{
// Construction
public:
	CDialogDateTimeDuration(CWnd* pParent = NULL);   // standard constructor
	bool CreateInsideBar(
		CExtControlBar * pBar
		);

// Dialog Data
	//{{AFX_DATA(CDialogDateTimeDuration)
	enum { IDD = IDD_DIALOG_DATE_TIME_DURATION };
	CExtCheckBox	m_wndDisabledDuration;
	CExtCheckBox	m_wndDisabledDateTime;
	CExtCheckBox	m_wndModeSeconds;
	CExtCheckBox	m_wndModeMinutes;
	CExtCheckBox	m_wndModeHours;
	CExtCheckBox	m_wndModeDays;
	CExtLabel		m_wndLabelNotifications;
	CExtCheckBox	m_wndContinuousDateTime;
	CExtCheckBox	m_wndContinuousDuration;
	CExtGroupBox	m_wndBoxTimeFormat;
	CExtGroupBox	m_wndBoxModeDateTime;
	CExtGroupBox	m_wndBoxModeDuration;
	CExtCheckBox	m_wndBtnSpinDateTime;
	CExtCheckBox	m_wndBtnSpinDuration;
	CExtCheckBox	m_wndBtnDropDown;
	CEdit			m_wndNotifications;
	CExtColorButton m_wndColorTextDateTime;
	CExtColorButton	m_wndColorBackDateTime;
	CExtButton		m_wndAdjustSizeDateTime;
	CExtColorButton m_wndColorTextDuration;
	CExtColorButton	m_wndColorBackDuration;
	CExtButton		m_wndAdjustSizeDuration;
	CExtGroupBox	m_wndBoxDuration;
	CExtGroupBox	m_wndBoxDateTime;
	CExtDurationWnd m_wndDuration;
	CExtDateTimeWnd m_wndDateTime;
	CExtRadioButton m_wndTimeFormatAuto;
	CExtRadioButton m_wndTimeFormatH12;
	CExtRadioButton m_wndTimeFormatH24;
	CExtRadioButton m_wndModeTime;
	CExtRadioButton m_wndModeDate;
	CExtRadioButton m_wndModeDateTime;
	int		m_nTimeFormat;
	int		m_nMode;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDateTimeDuration)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;

	// Generated message map functions
	//{{AFX_MSG(CDialogDateTimeDuration)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	LRESULT OnSelChangedDuration( WPARAM wParam, LPARAM lParam );
	LRESULT OnSelChangedDateTime( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)

/////////////////////////////////////////////////////////////////////////////
// CDialogButtons

class CDialogButtons
	: public CExtResizableDialog
{
	static HICON GetSmallIconForButton()
	{
		HICON hIcon = (HICON)
			::LoadImage(
				::AfxGetInstanceHandle(),
				MAKEINTRESOURCE( IDR_MAINFRAME ),
				IMAGE_ICON,
				16,
				16,
				0
				);
		ASSERT( hIcon != NULL );
		return hIcon;
	}
public:
	CDialogButtons();
	virtual ~CDialogButtons();
	bool CreateInsideBar(
		CExtControlBar * pBar
		);

	//{{AFX_DATA(CDialogButtons)
	enum { IDD = IDD_DIALOG_BUTTONS };
	CExtButton			m_wndButtonDisabled1;
	CExtButton			m_wndButtonNormal1;
	CExtButton			m_wndButtonDefault1;
	CExtButton			m_wndButtonDisabled2;
	CExtButton			m_wndButtonNormal2;
	CExtButton			m_wndButtonDefault2;
	CExtButton			m_wndButtonDisabled3;
	CExtButton				m_wndButtonNormal3;
	CExtButton			m_wndButtonDefault3;
	CExtHyperLinkButton	m_wndButtonNormal4;
	CExtHyperLinkButton	m_wndButtonDefault4;
	CExtColorButton		m_wndButtonDisabled5;
	CExtColorButton		m_wndButtonNormal5;
	CExtColorButton		m_wndButtonDefault5;
	CExtColorButton		m_wndButtonDisabled6;
	CExtColorButton		m_wndButtonNormal6;
	CExtColorButton		m_wndButtonDefault6;
	CExtGroupBox		m_wndGroupBox1;
	CExtGroupBox		m_wndGroupBox2;
	CExtGroupBox		m_wndGroupBox3;
	CExtGroupBox		m_wndGroupBox4;
	CExtGroupBox		m_wndGroupBox5;
	CExtGroupBox		m_wndGroupBox6;
	CExtGroupBox		m_wndGroupBox7;
	CExtGroupBox		m_wndGroupBox8;
	CExtGroupBox		m_wndGroupBox9;
	CExtRadioButton		m_wndRadio1;
	CExtRadioButton		m_wndRadio2;
	CExtRadioButton		m_wndRadioDisabled;
	CExtCheckBox		m_wndCheckTwoState;
	CExtCheckBox		m_wndCheckThreeState;
	CExtCheckBox		m_wndCheckDisabled;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDialogButtons)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDialogButtons)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CColorControlsView window

class CColorControlsView : public CWnd
{
// Construction
public:
	CColorControlsView();
	bool CreateInsideBar(
		CExtControlBar * pBar
		);

// Attributes
public:

protected:
	CExtColorCtrl
		m_wndColorCtrlRoller,
		m_wndColorCtrlCubeRGB,
		m_wndColorCtrlCubeCMY,
		m_wndColorCtrlMixerRGB,
		m_wndColorCtrlMixerCMY,
		m_wndColorCtrlHSL;
	bool m_bSyncingValues;
	bool m_bColorControlsVisible;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorControlsView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorControlsView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorControlsView)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	LRESULT OnColorChanged(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

class CMainFrame
	: public CExtNCW < CMDIFrameWnd >
	, public CExtDynamicBarSite
{
public:
	DECLARE_DYNAMIC( CMainFrame );

	CMainFrame();

// Attributes
protected:
	WINDOWPLACEMENT m_dataFrameWP;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CExtMenuControlBar			m_wndMenuBar;
	CExtStatusControlBar		m_wndStatusBar;
	CExtComboBox				m_wndComboInToolBar;
	CExtToolControlBar			m_wndToolBarStandard;
	CExtToolControlBar			m_wndToolBarThemes;
	CExtToolControlBar			m_wndToolBarSkins;
	CExtDynamicControlBar		* m_pBarButtons;
	CDialogButtons				m_dlgButtons;
	CExtDynamicControlBar		* m_pBarInputFields;
	CDialogInputFields			m_dlgInputFields;
#if (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)
	CExtDynamicControlBar		* m_pBarDateTimeDuration;
	CDialogDateTimeDuration		m_dlgDateTimeDuration;
#endif // (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)
	CExtDynamicControlBar		* m_pBarColorControls;
	CColorControlsView			m_viewColorControls;
	CExtDynamicControlBar		* m_pBarInvokeModalDialogs;
	CDialogInvokeModalDialogs	m_dlgInvokeModalDialogs;
	CExtDynamicControlBar		* m_pBarThemes;
	CDialogThemes				m_dlgThemes;
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	CExtTMDBS < CExtTabMdiWnd >	m_wndMdiTabs;
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTheme2000();
	afx_msg void OnUpdateTheme2000(CCmdUI* pCmdUI);
	afx_msg void OnThemeXp();
	afx_msg void OnUpdateThemeXp(CCmdUI* pCmdUI);
	afx_msg void OnTheme2003();
	afx_msg void OnUpdateTheme2003(CCmdUI* pCmdUI);
	afx_msg void OnTheme2005();
	afx_msg void OnUpdateTheme2005(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnThemeLoadSkinXml();
	afx_msg void OnThemeLoadSkinBin();
	afx_msg void OnComboBoxInToolbar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__205C43B5_15CF_429F_8CA0_07C395066CE5__INCLUDED_)
