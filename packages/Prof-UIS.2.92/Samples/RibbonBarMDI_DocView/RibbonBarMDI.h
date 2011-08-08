// RibbonBarMDI.h : main header file for the RibbonBarMDI application
//

#if !defined(AFX_RibbonBarMDI_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_)
#define AFX_RibbonBarMDI_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRibbonBarMDIApp:
// See RibbonBarMDI.cpp for the implementation of this class
//

class CRibbonBarMDIApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CRibbonBarMDIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRibbonBarMDIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CRibbonBarMDIApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RibbonBarMDI_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_)
