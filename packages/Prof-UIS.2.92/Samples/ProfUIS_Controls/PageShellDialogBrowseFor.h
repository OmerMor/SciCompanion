#if !defined(__PAGE_SHELL_DIALOG_BROWSE_FOR_H)
#define __PAGE_SHELL_DIALOG_BROWSE_FOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageShellDialogBrowseFor.h : header file
//

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageShellDialogBrowseFor dialog

class CPageShellDialogBrowseFor : public CPageBase 
{
// Construction
public:
	CPageShellDialogBrowseFor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageShellDialogBrowseFor)
	enum { IDD = IDD_PAGE_SHELL_DIALOG_BROWSE_FOR };
	CExtCheckBox	m_checkJumpToCurrentDirectoryInitially;
	CExtCheckBox	m_checkAllowCurrentDirectoryChanging;
 	CExtCheckBox	m_checkCompareWithWindowsDialogs;
 	CExtCheckBox	m_checkEnableShellContextMenusBFF;
 	CExtCheckBox	m_checkShowHiddenItemsBFF;
 	CExtEdit		m_editCustomCaptionForNewFolderButton;
 	CExtEdit		m_editCustomCaptionForEditLabel;
 	CExtEdit		m_editCustomCaptionForCancelButton1;
 	CExtEdit		m_labelEditRootInBrowseForDialog;
 	CExtEdit		m_editCustomCaptionForOkButton;
 	CExtEdit		m_editCustomCaptionForBrowseForDialog;
 	CExtCheckBox	m_checkShowNewFolderButton;
 	CExtCheckBox	m_checkShowEdit;
 	CExtEdit		m_editTitleText;
 	CExtEdit		m_editInitialFolderForBrowseDialog;
	CExtCheckBox	m_checkSaveRestoreShellLocation;
	CExtCheckBox	m_checkSaveRestoreWindowPosition;
	//}}AFX_DATA
	CExtEdit				m_editResultOfBrowseDialog;
	CString m_strRootCurrent;
	CExtPIDL m_pidlCustomRoot;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageShellDialogBrowseFor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageShellDialogBrowseFor)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonShowWindowsDialog();
	afx_msg void OnButtonShowProfUisDialog();
	afx_msg void OnButtonSetCustomRoot();
	afx_msg void OnButtonResetCustomRoot();
	afx_msg void OnCheckCompareWithWindowsDialogs();
	afx_msg void OnCheckSrSl();
	afx_msg void OnCheckJumpToCurrentDirectoryInitially();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT BrowseCallbackProc( WPARAM wParam, LPARAM lParam );
};

#endif // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PAGE_SHELL_DIALOG_BROWSE_FOR_H)
