// ThemeColorizer.h : main header file for the ThemeColorizer application
//

#if !defined(__THEMECOLORIZER_H)
#define __THEMECOLORIZER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CThemeColorizerApp:
// See ThemeColorizer.cpp for the implementation of this class
//

class CThemeColorizerApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CThemeColorizerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeColorizerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CThemeColorizerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__THEMECOLORIZER_H)
