// MthOutput.h : main header file for the MthOutput application
//

#if !defined(AFX_MDI_H__1E66F334_D3A5_44C6_A3FF_41A22AB16A0E__INCLUDED_)
#define AFX_MDI_H__1E66F334_D3A5_44C6_A3FF_41A22AB16A0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemoApp:
// See MthOutput.cpp for the implementation of this class
//

class CDemoApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;

public:
	//{{AFX_MSG(CDemoApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDI_H__1E66F334_D3A5_44C6_A3FF_41A22AB16A0E__INCLUDED_)
