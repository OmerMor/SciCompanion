// ResizablePropertySheet.h : main header file for the ResizablePropertySheet application
//

#if !defined(AFX_ResizablePropertySheet_H__066D34B2_489A_4403_8AC5_15B139F45240__INCLUDED_)
#define AFX_ResizablePropertySheet_H__066D34B2_489A_4403_8AC5_15B139F45240__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyApp:
// See ResizablePropertySheet.cpp for the implementation of this class
//

class CMyApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CMyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ResizablePropertySheet_H__066D34B2_489A_4403_8AC5_15B139F45240__INCLUDED_)
