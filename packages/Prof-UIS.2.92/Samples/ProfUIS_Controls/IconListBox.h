#if !defined(AFX_ICONLISTBOX_H__05529A1B_C832_44F4_9CA3_7194AC7EA611__INCLUDED_)
#define AFX_ICONLISTBOX_H__05529A1B_C832_44F4_9CA3_7194AC7EA611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListBoxMenuLike window

#if !defined(__EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxCustomPainted window

class CListBoxCustomPainted : public CListBox, public CExtPmBridge
{
public:
	DECLARE_DYNCREATE( CListBoxCustomPainted );
	IMPLEMENT_CExtPmBridge_MEMBERS( CListBoxCustomPainted );
	bool m_bShowSelAlways:1, m_bCaseSensitiveSort:1, m_bAutoDestroyListBox:1;
	CListBoxCustomPainted();
	virtual ~CListBoxCustomPainted();
	//{{AFX_VIRTUAL(CListBoxCustomPainted)
	public:
	virtual void DrawItem( LPDRAWITEMSTRUCT pDIS );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT pMIS );
	virtual int CompareItem( LPCOMPAREITEMSTRUCT pCIS );
	virtual void DeleteItem( LPDELETEITEMSTRUCT pDIS );
	virtual int VKeyToItem( UINT nKey, UINT nIndex );
	virtual int CharToItem( UINT nKey, UINT nIndex );
	protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CListBoxCustomPainted)
	//}}AFX_MSG
	afx_msg void OnLbnReflectSelCancel();
	afx_msg void OnLbnReflectKillFocus();
	afx_msg void OnLbnReflectSetFocus();
	DECLARE_MESSAGE_MAP()
protected:
	virtual void _RedrawLB();
public:
	virtual void PmBridge_OnPaintManagerChanged( CExtPaintManager * pGlobalPM );
	virtual HFONT OnLbQueryItemFont( INT nItemIndex );
	virtual CSize OnLbQueryItemExtraSize( INT nItemIndex );
	virtual bool OnLbQueryWindowFocusedState();
	virtual bool OnLbEraseEntire( CDC & dc, CRect rcClient );
};

/////////////////////////////////////////////////////////////////////////////
// CListBoxMenuLike window

class CListBoxMenuLike : public CExtNCSB < CListBoxCustomPainted >
{
// Construction
public:
	CListBoxMenuLike();

// Attributes
public:

private:
	void _Invalidate()
	{
		ASSERT_VALID( this );
		Invalidate();
		UpdateWindow();
	}

public:
	class ITEM_DATA
	{
	public:
		CString m_strText;
		CExtCmdIcon m_icon;
		HWND m_hWndPage;
		ITEM_DATA(
			CExtCmdIcon & icon,
			LPCTSTR strText,
			HWND hWnd
			)
			: m_icon( icon )
			, m_strText( strText )
			, m_hWndPage( hWnd )
		{
		}
		~ITEM_DATA()
		{
		}
		CSize GetSize() const
		{
			ASSERT( m_icon.GetSize().cx > 0 );
			ASSERT( m_icon.GetSize().cy > 0 );
			return m_icon.GetSize();
		}
		CExtCmdIcon * GetIconPtr()
		{
			ASSERT( !m_icon.IsEmpty() );
			return (&m_icon);
		}
	private:
		static CRect CalcFillItemRect( const RECT & rcItem, const SIZE & sizeIcon )
		{
			CRect rcFillItem( rcItem );
			rcFillItem.left += sizeIcon.cx + 8;
			rcFillItem.DeflateRect( 0, 2, 2, 2 );
			return rcFillItem;
		}
		static CRect CalcTextRect( const RECT & rcItem, const SIZE & sizeIcon )
		{
			CRect rcText = CalcFillItemRect( rcItem, sizeIcon );
			rcText.DeflateRect( 4, 2, 2, 2 );
			return rcText;
		}
		static CSize CalcColorCellSize()
		{
			return CSize( 6, 6 );
		}
		static CRect CalcEraseRect(
			const RECT & rcItem,
			HWND hWnd,
			INT nItemID
			)
		{
			ASSERT( hWnd != NULL && ::IsWindow(hWnd) );
			ASSERT( nItemID >= 0 );
			CListBox * pLB = (CListBox *)
				CWnd::FromHandlePermanent( hWnd );
			ASSERT_VALID( pLB );
			INT nCount = pLB->GetCount();
			ASSERT( nCount > 0 );
			ASSERT( nItemID < nCount );
			CRect rcErase = rcItem;
			if( nItemID == (nCount-1) )
			{
				CRect rcClient;
				pLB->GetClientRect( &rcClient );
				if( rcErase.bottom < rcClient.bottom )
					rcErase.bottom = rcClient.bottom;
			} // if( nItemID == (nCount-1) )
			return rcErase;
		}
	public:
		void DrawItem(
			INT nIconAreaWidth,
			LPDRAWITEMSTRUCT pDIS
			);
		void MeasureItem(
			LPMEASUREITEMSTRUCT pMIS,
			CSize _sizeClientArea
			);
	}; // class ITEM_DATA
	
	CArray < ITEM_DATA*, ITEM_DATA* > m_arrItems;

// Operations
public:
	void AddIcon(
		CExtCmdIcon & icon,
		LPCTSTR strComment,
		HWND hWnd
		)
	{
		ASSERT_VALID( this );
		ASSERT( !icon.IsEmpty() );
		m_arrItems.Add(
			new ITEM_DATA(
				icon,
				strComment,
				hWnd
				)
			);
		CExtNCSB < CListBoxCustomPainted > :: AddString(
			LPCTSTR(strComment) // _T("")
			);
	}
	void RemoveAllIcons()
	{
		ASSERT_VALID( this );
		if( GetSafeHwnd() != NULL )
			CExtNCSB < CListBoxCustomPainted > :: ResetContent();
		for( INT i = 0; i < m_arrItems.GetSize(); i++ )
			delete m_arrItems[i];
		m_arrItems.RemoveAll();
	}
	CExtCmdIcon * GetIconPtr( INT nPos )
	{
		ASSERT_VALID( this );
		ASSERT( 0 <= nPos && nPos < m_arrItems.GetSize() );
		ITEM_DATA * pData = m_arrItems[nPos];
		ASSERT( pData != NULL );
		return pData->GetIconPtr();
	}

	virtual bool OnLbEraseEntire( CDC & dc, CRect rcClient );
protected:
	INT GetIconAreaWidth()
	{
		INT nIconAreaWidth = 0;
		for( INT nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ ){
			ITEM_DATA *pItemData = m_arrItems[ nIndex ];
			if( nIconAreaWidth < pItemData->GetSize().cx ){
				nIconAreaWidth = pItemData->GetSize().cx;
			}
		}
		nIconAreaWidth += (5+5);
		return nIconAreaWidth;
	}
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxMenuLike)
	public:
	virtual void DrawItem( LPDRAWITEMSTRUCT pDIS );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT pMIS );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListBoxMenuLike();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxMenuLike)
	afx_msg void OnSelcancel();
	afx_msg void OnKillfocus();
	afx_msg void OnSetfocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONLISTBOX_H__05529A1B_C832_44F4_9CA3_7194AC7EA611__INCLUDED_)
