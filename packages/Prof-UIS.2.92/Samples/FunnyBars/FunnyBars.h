// FunnyBars.h : main header file for the FUNNYBARS application
//

#if !defined(AFX_FUNNYBARS_H__B7370753_7B61_4AC4_94A9_034ED77E3327__INCLUDED_)
#define AFX_FUNNYBARS_H__B7370753_7B61_4AC4_94A9_034ED77E3327__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFunnyBarsApp:
// See FunnyBars.cpp for the implementation of this class
//

class CFunnyBarsApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CFunnyBarsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFunnyBarsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CFunnyBarsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNNYBARS_H__B7370753_7B61_4AC4_94A9_034ED77E3327__INCLUDED_)
