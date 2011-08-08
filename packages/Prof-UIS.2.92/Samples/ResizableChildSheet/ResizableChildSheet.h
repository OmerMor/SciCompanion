// ResizableChildSheet.h : main header file for the ResizableChildSheetT application
//

#if !defined(AFX_CHILDSHEET_H__74BA2956_E9E8_4031_BA1C_92A131E0D5C7__INCLUDED_)
#define AFX_CHILDSHEET_H__74BA2956_E9E8_4031_BA1C_92A131E0D5C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

//
// Prof-UIS command manager profile name
//
#define __PROF_UIS_PROJECT_CMD_PROFILE_NAME _T("ResizableChildSheet-command-manager-profile")

#define __PROF_UIS_PROJECT_DLG_PERSIST_REG_KEY _T("ResizableChildSheet-resizable-dialog-positions")

/////////////////////////////////////////////////////////////////////////////
// CDemoApp:
// See ResizableChildSheet.cpp for the implementation of this class
//

class CDemoApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//
	// Prof-UIS advanced options
    //
	void SetupUiAdvancedOptions();

protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	//{{AFX_MSG(CDemoApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDSHEET_H__74BA2956_E9E8_4031_BA1C_92A131E0D5C7__INCLUDED_)
