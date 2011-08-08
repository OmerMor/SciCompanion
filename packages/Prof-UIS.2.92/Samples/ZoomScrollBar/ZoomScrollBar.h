// ZoomScrollBar.h : main header file for the ZoomScrollBar application
//

#if !defined(AFX_ZoomScrollBar_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_)
#define AFX_ZoomScrollBar_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CApp:
// See ZoomScrollBar.cpp for the implementation of this class
//

class CApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZoomScrollBar_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_)
