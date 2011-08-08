// ProfUIS_Controls.h : main header file for the PROFUIS_CONTROLS application
//

#if !defined(AFX_PROFUIS_CONTROLS_H__4ABF6D1C_5BEA_4FEB_B891_B762A236AE89__INCLUDED_)
#define AFX_PROFUIS_CONTROLS_H__4ABF6D1C_5BEA_4FEB_B891_B762A236AE89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define __CANCEL_BUTTON_PRESSED_EVENT_RESULT_DESCRIPTION _T("***Cancel***")

/////////////////////////////////////////////////////////////////////////////
// CMainApp:
// See ProfUIS_Controls.cpp for the implementation of this class
//

class CMainApp : public CWinApp
{
public:
	IMPLEMENT_CWinAPP_DoMessageBox;
	CMainApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMainApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
// CExtFlatEdit
//////////////////////////////////////////////////////////////////////////

class CExtFlatEdit : public CExtEditBase 
{
public:
    CExtFlatEdit::CExtFlatEdit()
        : CExtEditBase()
    {
    }
    
protected:
    
    virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
    {
        switch( message )
        {
        case WM_NCPAINT:
            {
                DefWindowProc(
                    WM_NCPAINT,
                    wParam,
                    lParam
                    );

                CRect rcInBarWnd, rcInBarClient;
                GetWindowRect( &rcInBarWnd );
                GetClientRect( &rcInBarClient );
                ClientToScreen( &rcInBarClient );
                if( rcInBarWnd == rcInBarClient )
                    return 0;

                CPoint ptDevOffset = -rcInBarWnd.TopLeft();
                rcInBarWnd.OffsetRect( ptDevOffset );
                rcInBarClient.OffsetRect( ptDevOffset );
                
                CWindowDC dc( this );
                ASSERT( dc.GetSafeHdc() != NULL );

                const int cx = ::GetSystemMetrics(SM_CXVSCROLL);
                const int cy = ::GetSystemMetrics(SM_CYHSCROLL);
            
                bool bHasVerticalSB = 
                    ( rcInBarWnd.Width() - rcInBarClient.Width() ) >= cx;
                bool bHasHorizontalSB = 
                    ( rcInBarWnd.Height() - rcInBarClient.Height() ) >= cy;
            
                if( bHasVerticalSB && bHasHorizontalSB )
                {
                    dc.FillSolidRect(
                        rcInBarWnd.right - cx - 4,
                        rcInBarWnd.bottom - cy - 4,
                        cx,
                        cy,
                        g_PaintManager->GetColor(COLOR_WINDOW)
                        );
                }

                CRect rcExclude( rcInBarClient );
                if( bHasVerticalSB )
                    rcExclude.right += cx;
                if( bHasHorizontalSB )
                    rcExclude.bottom += cy;
                dc.ExcludeClipRect( &rcExclude );
                
                static const TCHAR szEditBox[] = _T("EDIT");
                TCHAR szCompare[sizeof(szEditBox)/sizeof(szEditBox[0])+1];
                ::GetClassName( 
                    GetSafeHwnd(), 
                    szCompare, 
                    sizeof(szCompare)/sizeof(szCompare[0]) 
                );
                if( !lstrcmpi( szCompare, szEditBox ) )
				{
                    bool bReadOnly = 
                        ( (GetStyle() & ES_READONLY) != 0 ) ? true : false;
					COLORREF clrSysBk =
						g_PaintManager->GetColor( 
							bReadOnly 
							? COLOR_3DFACE
							: COLOR_WINDOW 
						);
					dc.FillSolidRect(
						&rcInBarWnd, 
						clrSysBk
						);
                }
                
                g_PaintManager->PaintResizableBarChildNcAreaRect(
                    dc,
                    rcInBarWnd,
                    this
                    );
    
                return 0;
            } // case WM_NCPAINT
        }; // switch( message )
        return CExtEditBase::WindowProc( message, wParam, lParam );
    };
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFUIS_CONTROLS_H__4ABF6D1C_5BEA_4FEB_B891_B762A236AE89__INCLUDED_)
