// GLViews.h : main header file for the GLViews application
//

#if !defined(AFX_GLViews_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_)
#define AFX_GLViews_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGLViewsApp:
// See GLViews.cpp for the implementation of this class
//

class CGLViewsApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CGLViewsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLViewsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CGLViewsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLViews_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_)
