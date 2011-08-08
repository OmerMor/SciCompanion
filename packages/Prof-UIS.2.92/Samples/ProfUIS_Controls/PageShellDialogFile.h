#if !defined(__PAGE_SHELL_DIALOG_FILE_H)
#define __PAGE_SHELL_DIALOG_FILE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageShellDialogFile.h : header file
//

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageShellDialogFile dialog

class CPageShellDialogFile : public CPageBase 
{
// Construction
public:
	CPageShellDialogFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageShellDialogFile)
	enum { IDD = IDD_PAGE_SHELL_DIALOG_FILE };
	CExtCheckBox	m_checkJumpToCurrentDirectoryInitially;
	CExtCheckBox	m_checkAllowCurrentDirectoryChanging;
 	CExtCheckBox	m_checkCompareWithWindowsDialogs;
 	CExtCheckBox	m_checkShowTree;
 	CExtCheckBox	m_checkUseSplitter;
 	CExtCheckBox	m_checkCollapsingAnimation;
	CExtCheckBox	m_checkShowRootFolder;
	CExtCheckBox	m_checkShowParentFolder;
 	CExtCheckBox	m_checkEnableShellContextMenusFD;
 	CExtCheckBox	m_checkInitiallyCollapsed;
 	CExtCheckBox	m_checkEnableCollapsing;
 	CExtCheckBox	m_checkShowHiddenItemsFD;
 	CExtCheckBox	m_checkResolveLinks;
 	CExtCheckBox	m_checkWriteAccess;
 	CExtCheckBox	m_checkShowFTCombo;
 	CExtCheckBox	m_checkReadAccess;
 	CExtCheckBox	m_checkPromptOverwrite;
 	CExtCheckBox	m_checkFNEShow;
 	CExtCheckBox	m_checkFNEReadOnly;
 	CExtCheckBox	m_checkFilesMustExist;
 	CExtCheckBox	m_checkPathMustExist;
 	CExtEdit		m_editCustomCaptionForFileTypeLabel;
 	CExtEdit		m_editCustomCaptionForFileNameLabel;
 	CExtEdit		m_editCustomCaptionForCancelButton2;
 	CExtEdit		m_editCustomCaptionForOpenSaveButton;
 	CExtEdit		m_editCustomCaptionForFileDialog;
 	CExtComboBox	m_comboFileDialogType;
 	CExtEdit		m_editInitialFolderForFileDialog;
 	CExtEdit		m_editInitialFileTypeIndex;
 	CExtSpinWnd		m_spinInitialFileTypeIndex;
 	CExtEdit		m_editInitialFileNames;
	CExtCheckBox	m_checkSaveRestoreShellLocation;
	CExtCheckBox	m_checkSaveRestoreWindowPosition;
	//}}AFX_DATA
	CExtNCSB < CExtEdit >	m_editResultOfFileDialog;
	CExtEdit				m_editFileDialogFilter;
protected:
	CString m_strRootCurrent;
	CExtPIDL m_pidlCustomRoot;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageShellDialogFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageShellDialogFile)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonShowWindowsDialog();
	afx_msg void OnButtonShowProfUisDialog();
	afx_msg void OnCheckCompareWithWindowsDialogs();
	afx_msg void OnCheckSrSl();
	afx_msg void OnCheckJumpToCurrentDirectoryInitially();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PAGE_SHELL_DIALOG_FILE_H)
