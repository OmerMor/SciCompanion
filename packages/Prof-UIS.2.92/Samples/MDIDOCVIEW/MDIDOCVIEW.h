// MDIDOCVIEW.h : main header file for the MDIDOCVIEW application
//

#if !defined(AFX_MDIDOCVIEW_H__537FA47C_64CC_4CC0_9045_FD8DC86933A6__INCLUDED_)
#define AFX_MDIDOCVIEW_H__537FA47C_64CC_4CC0_9045_FD8DC86933A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define __PROF_UIS_PROJECT_DLG_PERSIST_REG_KEY _T("MDIDOCVIEW-resizable-dialog-positions")

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWApp:
// See MDIDOCVIEW.cpp for the implementation of this class
//

class CMDIDOCVIEWApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CMDIDOCVIEWApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIDOCVIEWApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//
	// Prof-UIS advanced options
    //
	void SetupUiAdvancedOptions();

	//{{AFX_MSG(CMDIDOCVIEWApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIDOCVIEW_H__537FA47C_64CC_4CC0_9045_FD8DC86933A6__INCLUDED_)
