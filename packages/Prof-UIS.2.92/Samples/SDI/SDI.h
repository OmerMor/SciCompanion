// SDI.h : main header file for the SDI application
//

#if !defined(AFX_SDI_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_)
#define AFX_SDI_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSDIApp:
// See SDI.cpp for the implementation of this class
//

class CSDIApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CSDIApp();
	virtual ~CSDIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDIApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CSDIApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDI_H__BFA61E0F_5638_435C_AEBA_3DBC3601639F__INCLUDED_)
