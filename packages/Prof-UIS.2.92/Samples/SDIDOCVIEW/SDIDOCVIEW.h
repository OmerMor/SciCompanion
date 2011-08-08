// SDIDOCVIEW.h : main header file for the SDIDOCVIEW application
//

#if !defined(AFX_SDIDOCVIEW_H__FA425BE3_3357_4A60_9520_7DA3860A13FA__INCLUDED_)
#define AFX_SDIDOCVIEW_H__FA425BE3_3357_4A60_9520_7DA3860A13FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define __PROF_UIS_PROJECT_DLG_PERSIST_REG_KEY _T("SDIDOCVIEW-resizable-dialog-positions")

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWApp:
// See SDIDOCVIEW.cpp for the implementation of this class
//

class CSDIDOCVIEWApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CSDIDOCVIEWApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDIDOCVIEWApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//
	// Prof-UIS advanced options
    //
	void SetupUiAdvancedOptions();

	//{{AFX_MSG(CSDIDOCVIEWApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDIDOCVIEW_H__FA425BE3_3357_4A60_9520_7DA3860A13FA__INCLUDED_)
