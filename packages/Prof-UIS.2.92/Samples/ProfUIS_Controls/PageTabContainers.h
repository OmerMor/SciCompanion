#if !defined(AFX_PAGETABCONTAINERS_H__815AD1B0_19DE_497D_9095_E4C754FCE4CD__INCLUDED_)
#define AFX_PAGETABCONTAINERS_H__815AD1B0_19DE_497D_9095_E4C754FCE4CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageTabContainers.h : header file
//
#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageTabContainers dialog

class CPageTabContainers : public CPageBase
{
// Construction
public:
	CPageTabContainers(CWnd* pParent = NULL);   // standard constructor

	class CMyTabPageContainerWndFlat : public CExtTabPageContainerFlatWnd
	{
//		virtual void OnTabWndClickedItemCloseButton( LONG nItemIndex )
//		{
//			CString strMessage;
//			strMessage.Format( _T("Clicked X-button on %d tab item"), nItemIndex );
//			::AfxMessageBox( LPCTSTR(strMessage) );
//		}

		class CMyTabWndFlat : public CExtTabFlatWnd
		{
			int m_nShift;
			int m_nSize;
		public:
			CMyTabWndFlat() : m_nShift( -1 ), m_nSize( -1 ) { }
			int GetShift()
			{
				CRect rc;
				GetWindowRect(rc);
				return _CalcRgnShift( OrientationIsHorizontal(), rc );
			}
			int GetSize() { return OnFlatTabWndGetSize( OrientationIsHorizontal() ); }
			void SetShift( int nShift ) { m_nShift = nShift; }
			void SetSize( int nSize ) { m_nSize = nSize; }
		protected:
			virtual int _CalcRgnShift( bool bHorz, const CRect & rc )
			{
				int nShift = 0;
				if( m_nShift >= 0 )
					nShift = m_nShift;
				else
					nShift = ::MulDiv( bHorz ? rc.Height() : rc.Width(), 1, 4 );
				return nShift;
			}
			virtual int OnFlatTabWndGetSize( bool bHorz )
			{
				int nSize = 0;
				if(bHorz)
					if( m_nSize > 0 )
						nSize = m_nSize;
					else
						nSize = 16;//::GetSystemMetrics( SM_CXHSCROLL );
					else
						if( m_nSize > 0 )
							nSize = m_nSize;
						else
							nSize = 16;//::GetSystemMetrics( SM_CYHSCROLL );
						return nSize;
			}
		}; // class CMyTabWndFlat	
		
		virtual CExtTabWnd * OnTabWndGetTabImpl()
		{
			return new CExtTWPC < CMyTabWndFlat >;
		}
		
		virtual bool OnTabWndClickedButton(
			LONG nHitTest,
			bool bButtonPressed,
			INT nMouseButton, // MK_... values
			UINT nMouseEventFlags)
		{
			ASSERT_VALID( this );
			nMouseButton;
			nMouseEventFlags;
			if( nHitTest==__ETWH_BUTTON_CLOSE && !bButtonPressed ){
				TRACE1("\"Close\" button clicked on page %d\n", PageSelectionGet());
			}
			if( nHitTest==__ETWH_BUTTON_HELP && !bButtonPressed ){
				TRACE1("\"Help\" button clicked on page %d\n", PageSelectionGet());
			}
			return true;
		}
		public:
			void SetShift( int nShift ){
				((CMyTabWndFlat*)m_pWndTab)->SetShift( nShift );
				((CMyTabWndFlat*)m_pWndTab)->UpdateTabWnd();
			}
			void SetSize( int nSize ){
				((CMyTabWndFlat*)m_pWndTab)->SetSize( nSize );
				((CMyTabWndFlat*)m_pWndTab)->UpdateTabWnd();
				_RepositionBarsImpl();
			}
			int GetShift(){
				return ((CMyTabWndFlat*)m_pWndTab)->GetShift();
			}
			int GetSize(){
				return ((CMyTabWndFlat*)m_pWndTab)->GetSize();
			}
	}; // class CMyTabPageContainerWndFlat	

	class CMyTabPageContainerWnd : public CExtTabPageContainerWnd
	{
	}; // class CMyTabPageContainerWnd
	
	CMyTabPageContainerWndFlat m_wndTabPageContainerFlat;
	CMyTabPageContainerWnd m_wndTabPageContainer;
	//CExtWRB<CMyTabPageContainerWnd> m_wndTabPageContainer;

// Dialog Data
	//{{AFX_DATA(CPageTabContainers)
	enum { IDD = IDD_PAGE_TAB_CONTAINERS };
	CExtEdit		m_wndSize;
	CExtEdit		m_wndShift;
	CExtCheckBox	m_CheckShowHome;
	CExtCheckBox	m_CheckShowEnd;
	CExtCheckBox	m_chkItemDragging;
	CExtCheckBox	m_chkShowTabList;
	CExtCheckBox	m_chkEnableTabList;
	CExtLabel		m_StaticSize;
	CExtLabel		m_StaticShift;
	CExtLabel		m_StaticOrientation;
	CExtSpinWnd		m_SpinSize;
	CExtSpinWnd		m_SpinShift;
	CExtComboBox	m_Orientation;
	CExtCheckBox	m_chkShowHelp;
	CExtCheckBox	m_chkShowClose;
	CExtCheckBox	m_chkShowBorders;
	CExtCheckBox	m_chkInvertVertFont;
	CExtCheckBox	m_chkInclineBefore;
	CExtCheckBox	m_chkInclineAfter;
	CExtCheckBox	m_chkFullWidths;
	CExtCheckBox	m_chkFocusOnHover;
	CExtCheckBox	m_chkEqualWidths;
	CExtCheckBox	m_chkEnableHelp;
	CExtCheckBox	m_chkEnableClose;
	CExtCheckBox	m_chkCenterText;
	CExtCheckBox	m_chkBoldSelection;
	CExtCheckBox	m_chkAutoHideScroll;
	int				m_nShift;
	int				m_nSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageTabContainers)
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL InitTreeCtrl(CTreeCtrl *pTreeCtrl, CExtTabPageContainerWnd* pTabsContainer);
	BOOL InitEditCtrl(CEdit *pEditCtrl, CExtTabPageContainerWnd* pTabsContainer);
	BOOL InitListBoxCtrl(CListBox *pListBox, CExtTabPageContainerWnd* pTabsContainer);
	bool m_bInitComplete:1;
	
	CExtColorCtrl m_wndColorCtrl0;
	CExtNCSB < CListBox > m_wndListBox0;
	CExtNCSB < CExtEdit > m_wndEditCtrl0;
	CExtNCSB < CExtWFF< CTreeCtrl > > m_wndTreeCtrl0;
	CExtResizableDialog m_wndTabPageDialog0;
	
	CExtColorCtrl m_wndColorCtrl1;
	CExtNCSB < CListBox >  m_wndListBox1;
	CExtNCSB < CExtEdit > m_wndEditCtrl1;
	CExtNCSB < CExtWFF< CTreeCtrl > > m_wndTreeCtrl1;
	CExtResizableDialog m_wndTabPageDialog1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageTabContainers)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokComboOrientation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // #if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETABCONTAINERS_H__815AD1B0_19DE_497D_9095_E4C754FCE4CD__INCLUDED_)
