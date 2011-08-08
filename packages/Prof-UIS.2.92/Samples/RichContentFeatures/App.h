#if !defined(__GEO_CONTROLS_APP_H)
#define __GEO_CONTROLS_APP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"


class CApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;

	struct CMyResizablePropertySheet : public CExtResizablePropertySheet
	{
		CMyResizablePropertySheet() { }
		CMyResizablePropertySheet( UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0 ) : CExtResizablePropertySheet( nIDCaption, pParentWnd, iSelectPage ) { }
		CMyResizablePropertySheet( __EXT_MFC_SAFE_LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0 ) : CExtResizablePropertySheet( pszCaption, pParentWnd, iSelectPage ) { }
	protected:
		virtual void PreSubclassWindow() { CExtResizablePropertySheet::PreSubclassWindow(); g_CmdManager->ProfileWndAdd( NULL, m_hWnd ); }
		virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
		{
			switch( message )
			{
			case WM_INITDIALOG:
				{
					GetDlgItem(IDHELP)->ShowWindow( SW_HIDE );
					GetDlgItem(IDOK)->ShowWindow( SW_HIDE );
				}
			break;
			case WM_DESTROY : g_CmdManager->ProfileWndRemove( m_hWnd ); break;
			}
			return CExtResizablePropertySheet::WindowProc( message, wParam, lParam );
		}
		void OnResizablePropertySheetInitialized()
		{
			CExtResizablePropertySheet::OnResizablePropertySheetInitialized();
			CExtButton * pBtnClose = STATIC_DOWNCAST( CExtButton, GetDlgItem(IDCANCEL) );
			pBtnClose->SetWindowText( _T("<html><meta txt=\"&C\"><i><u><b>C</b></u>lose</i><sup>(Alt+C)</sup></html>") );
			pBtnClose->SetTooltipText( _T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>This <i><u><b>C</b></u>lose</i><sup>(Alt+C)</sup> button uses <b>HTML</b> <img src=smile.bmp> both in caption and tooltip.</p></html>") );
		}
	}; /// struct CMyResizablePropertySheet

	CApp();
	virtual ~CApp();

	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(__GEO_CONTROLS_APP_H)
