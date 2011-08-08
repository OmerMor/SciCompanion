// FixedSizePanels.h : main header file for the FixedSizePanels application
//

#if !defined(__FIXEDSIZEPANELS_H)
#define __FIXEDSIZEPANELS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFixedSizePanelsApp:
// See FixedSizePanels.cpp for the implementation of this class
//

class CFixedSizePanelsApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CFixedSizePanelsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFixedSizePanelsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CFixedSizePanelsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__FIXEDSIZEPANELS_H)
