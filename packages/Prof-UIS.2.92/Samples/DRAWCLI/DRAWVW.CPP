// drawvw.cpp : implementation of the CDrawView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include <afxpriv.h>

#include "drawcli.h"

#include "drawdoc.h"
#include "drawobj.h"
#include "cntritem.h"
#include "drawvw.h"

#include "drawobj.h"
#include "drawtool.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// private clipboard format (list of Draw objects)
CLIPFORMAT CDrawView::m_cfDraw = (CLIPFORMAT)
	::RegisterClipboardFormat(_T("MFC Draw Sample"));
CLIPFORMAT CDrawView::m_cfObjectDescriptor = NULL;

/////////////////////////////////////////////////////////////////////////////
// CDrawView

IMPLEMENT_DYNCREATE (CDrawView, CScrollView )

BEGIN_MESSAGE_MAP( CDrawView, CScrollView )
	//{{AFX_MSG_MAP(CDrawView)
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT, OnCancelEdit)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_DRAW_SELECT, OnDrawSelect)
	ON_COMMAND(ID_DRAW_ROUNDRECT, OnDrawRoundRect)
	ON_COMMAND(ID_DRAW_RECT, OnDrawRect)
	ON_COMMAND(ID_DRAW_LINE, OnDrawLine)
	ON_COMMAND(ID_DRAW_ELLIPSE, OnDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, OnUpdateDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, OnUpdateDrawLine)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, OnUpdateDrawRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDRECT, OnUpdateDrawRoundRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SELECT, OnUpdateDrawSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEBACK, OnUpdateSingleSelect)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateAnySelect)
	ON_COMMAND(ID_DRAW_POLYGON, OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, OnUpdateDrawPolygon)
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_OBJECT_FILLCOLOR, OnObjectFillColor)
	ON_COMMAND(ID_OBJECT_LINECOLOR, OnObjectLineColor)
	ON_COMMAND(ID_OBJECT_MOVEBACK, OnObjectMoveBack)
	ON_COMMAND(ID_OBJECT_MOVEFORWARD, OnObjectMoveForward)
	ON_COMMAND(ID_OBJECT_MOVETOBACK, OnObjectMoveToBack)
	ON_COMMAND(ID_OBJECT_MOVETOFRONT, OnObjectMoveToFront)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_SHOWOBJECTS, OnViewShowObjects)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWOBJECTS, OnUpdateViewShowObjects)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTIES, OnUpdateEditProperties)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEFORWARD, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOBACK, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOFRONT, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_DOC_LINE_WIDTH, OnUpdateDocLineWidth)
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPrepareOneMenuLevel,
		OnExtMenuPrepare
		)

// 	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEBACK, OnUpdateSelectionSingle)
// 	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEFORWARD, OnUpdateSelectionSingle)
// 	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOBACK, OnUpdateSelectionSingle)
// 	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOFRONT, OnUpdateSelectionSingle)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_FILLCOLOR, OnUpdateSelectionNonEmpty)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_LINECOLOR, OnUpdateSelectionNonEmpty)

	// color popup menu
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
		OnColorSelectCustom
		)

	ON_COMMAND_RANGE(
		ID_LINE_WIDTH_0,
		ID_LINE_WIDTH_OTHER,
		OnLineWidth
		)
	ON_UPDATE_COMMAND_UI_RANGE(
		ID_LINE_WIDTH_0,
		ID_LINE_WIDTH_OTHER,
		OnUpdateLineWidth
		)

	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPopupDrawItem,
		OnDrawPopupMenuItem
		)

#if (defined __EXT_MFC_NO_PRINT)
	ON_COMMAND( ID_FILE_PRINT, OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, OnFilePrintPreview )
#endif

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawView construction/destruction

CDrawView::CDrawView()
{
	m_bGrid = TRUE;
	m_gridColor = RGB(0, 0, 128);
	m_bActive = FALSE;
// new
	if( m_cfObjectDescriptor == NULL )
		m_cfObjectDescriptor = (CLIPFORMAT)::RegisterClipboardFormat(_T("Object Descriptor") );
	m_prevDropEffect = DROPEFFECT_NONE;
// end new
}

CDrawView::~CDrawView()
{
}

BOOL CDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT(cs.style & WS_CHILD);
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);

	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE|WS_EX_STATICEDGE);
	return TRUE;
}

void CDrawView::OnActivateView(BOOL bActivate, CView* pActiveView,
	CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActiveView, pDeactiveView);

	// invalidate selections when active status changes
	if (m_bActive != bActivate)
	{
		if( bActivate )  // if becoming active update as if active
		{
			m_bActive = bActivate;
			SyncToolbarObjButtons();
		}
		if (!m_selection.IsEmpty())
			OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_bActive = bActivate;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView drawing

void CDrawView::InvalObj(CDrawObj* pObj)
{
	CRect rect = pObj->m_position;
	DocToClient(rect);
	if (m_bActive && IsSelected(pObj))
	{
		rect.left -= 4;
		rect.top -= 5;
		rect.right += 5;
		rect.bottom += 4;
	}
	rect.InflateRect(1, 1); // handles CDrawOleObj objects

	InvalidateRect(rect, FALSE);
}

void CDrawView::OnUpdate(CView* , LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		InvalObj((CDrawObj*)pHint);
		break;

	case HINT_UPDATE_SELECTION: // an entire selection has changed
		{
			CDrawObjList* pList = pHint != NULL ?
				(CDrawObjList*)pHint : &m_selection;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
				InvalObj(pList->GetNext(pos));
		}
		break;

	case HINT_DELETE_SELECTION: // an entire selection has been removed
		if (pHint != &m_selection)
		{
			CDrawObjList* pList = (CDrawObjList*)pHint;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
			{
				CDrawObj* pObj = pList->GetNext(pos);
				InvalObj(pObj);
				Remove(pObj);   // remove it from this view's selection
			}
		}
		break;

	case HINT_UPDATE_OLE_ITEMS:
		{
			CDrawDoc* pDoc = GetDocument();
			POSITION pos = pDoc->GetObjects()->GetHeadPosition();
			while (pos != NULL)
			{
				CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
				if (pObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
					InvalObj(pObj);
			}
		}
		break;

	case HINT_UPDATE_TOOLBAR_COLOR_BUTTONS:
		{
			CWnd * pWndFocus = GetFocus();
			if( pWndFocus != NULL
				&& (pWndFocus == this || IsChild(pWndFocus) )
				)
				SyncToolbarObjButtons();
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CDrawView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView :: OnPrepareDC( pDC, pInfo );

	// mapping mode is MM_ANISOTROPIC
	// these extents setup a mode similar to MM_LOENGLISH
	// MM_LOENGLISH is in .01 physical inches
	// these extents provide .01 logical inches

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(
		::GetDeviceCaps( pDC->m_hDC, LOGPIXELSX ),
		::GetDeviceCaps( pDC->m_hDC, LOGPIXELSY )
		);
	pDC->SetWindowExt(100, -100);

	// set the origin of the coordinate system to the center of the page
	CPoint ptOrg;
	ptOrg.x = GetDocument()->GetSize().cx / 2;
	ptOrg.y = GetDocument()->GetSize().cy / 2;

	// ptOrg is in logical coordinates
	pDC->OffsetWindowOrg(-ptOrg.x,ptOrg.y);
}

BOOL CDrawView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	// do the scroll
	if( ! __BASE_OF_CDrawView__ :: OnScrollBy( sizeScroll, bDoScroll ) )
		return FALSE;

	// update the position of any in-place active item
	if( bDoScroll )
	{
		UpdateActiveItem();
		UpdateWindow();
	}
	return TRUE;
}

void CDrawView::OnDraw(CDC* pDC)
{
CDrawDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
CDC dc;
CDC * pDrawDC = pDC;
CBitmap bitmap;
CBitmap * pOldBitmap = NULL;
	// only paint the rect that needs repainting
CRect client;
	pDC->GetClipBox(client);
CRect rect = client;
	DocToClient(rect);
	if( ! pDC->IsPrinting() )
	{

		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////
		// BEGIN: FIRST VERSION OF THE "TRASH-LINES" PROBLEM FIX
		GetClientRect( &rect );
		client = rect;
		ClientToDoc( client );
		pDC->SelectClipRgn( NULL );
		// END: FIRST VERSION OF THE "TRASH-LINES" PROBLEM FIX
		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////

		// draw to offscreen bitmap for fast looking repaints
		if( dc.CreateCompatibleDC( pDC ) )
		{
			if(	bitmap.CreateCompatibleBitmap(
					pDC,
					rect.Width(),
					rect.Height()
					)
				)
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;
				// offset origin more because bitmap is just piece
				// of the whole drawing
				dc.OffsetViewportOrg( -rect.left, -rect.top );
				pOldBitmap = dc.SelectObject( &bitmap );
				dc.SetBrushOrg( rect.left % 8, rect.top % 8 );
				// might as well clip to the same rectangle
				dc.IntersectClipRect( client );
			}
		}
	}
	// paint background
	if( ! pDC->IsPrinting() )
	{
		CBrush brush;
		if( ! brush.CreateSolidBrush( pDoc->GetPaperColor() ) )
			return;
		brush.UnrealizeObject();
		pDrawDC->FillRect( client, &brush );
		if( m_bGrid )
			DrawGrid( pDrawDC );
	}
	pDoc->Draw( pDrawDC, this );
	if( pDrawDC != pDC )
	{
		pDC->SetViewportOrg( 0, 0 );
		pDC->SetWindowOrg( 0, 0 );
		pDC->SetMapMode( MM_TEXT );
		dc.SetViewportOrg( 0, 0 );
		dc.SetWindowOrg( 0, 0 );
		dc.SetMapMode( MM_TEXT );
		pDC->BitBlt(
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			&dc,
			0,
			0,
			SRCCOPY
			);
		dc.SelectObject( pOldBitmap );
	}
}

void CDrawView::Remove(CDrawObj* pObj)
{
	POSITION pos = m_selection.Find(pObj);
	if (pos != NULL)
		m_selection.RemoveAt(pos);
}

void CDrawView::PasteNative(COleDataObject& dataObject)
{
	// get file refering to clipboard data
	CFile* pFile = dataObject.GetFileData(m_cfDraw);
	if (pFile == NULL)
		return;

	// connect the file to the archive
	CArchive ar(pFile, CArchive::load);
	TRY
	{
		ar.m_pDocument = GetDocument(); // set back-pointer in archive

		// read the selection
		m_selection.Serialize(ar);
	}
	CATCH_ALL(e)
	{
		ar.Close();
		delete pFile;
		THROW_LAST();
	}
	END_CATCH_ALL

	ar.Close();
	delete pFile;
}

void CDrawView::PasteEmbedded(COleDataObject& dataObject, CPoint point )
{
	BeginWaitCursor();

	// paste embedded
	CDrawOleObj* pObj = new CDrawOleObj(GetInitialPosition());
	ASSERT_VALID(pObj);
	CDrawItem* pItem = new CDrawItem(GetDocument(), pObj);
	ASSERT_VALID(pItem);
	pObj->m_pClientItem = pItem;

	TRY
	{
		if (!pItem->CreateFromData(&dataObject) &&
			!pItem->CreateStaticFromData(&dataObject))
		{
			AfxThrowMemoryException();      // any exception will do
		}

		// add the object to the document
		GetDocument()->Add(pObj);
		m_selection.AddTail(pObj);
		ClientToDoc( point );
		pObj->MoveTo( CRect( point, pObj->m_extent ), this );

		// try to get initial presentation data
		pItem->UpdateLink();
		pItem->UpdateExtent();
	}
	CATCH_ALL(e)
	{
		// clean up item
		pItem->Delete();
		pObj->m_pClientItem = NULL;
		GetDocument()->Remove(pObj);
		pObj->Remove();

		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH_ALL

	EndWaitCursor();
}

void CDrawView::DrawGrid(CDC* pDC)
{
	CDrawDoc* pDoc = GetDocument();

	COLORREF oldBkColor = pDC->SetBkColor(pDoc->GetPaperColor());

	CRect rect;
	rect.left = -pDoc->GetSize().cx / 2;
	rect.top = -pDoc->GetSize().cy / 2;
	rect.right = rect.left + pDoc->GetSize().cx;
	rect.bottom = rect.top + pDoc->GetSize().cy;

	// Center lines
	CPen penDash;
	penDash.CreatePen(PS_DASH, 1, m_gridColor);
	CPen* pOldPen = pDC->SelectObject(&penDash);

	pDC->MoveTo(0, rect.top);
	pDC->LineTo(0, rect.bottom);
	pDC->MoveTo(rect.left, 0);
	pDC->LineTo(rect.right, 0);

	// Major unit lines
	CPen penDot;
	penDot.CreatePen(PS_DOT, 1, m_gridColor);
	pDC->SelectObject(&penDot);

	for (int x = rect.left / 100 * 100; x < rect.right; x += 100)
	{
		if (x != 0)
		{
			pDC->MoveTo(x, rect.top);
			pDC->LineTo(x, rect.bottom);
		}
	}

	for (int y = rect.top / 100 * 100; y < rect.bottom; y += 100)
	{
		if (y != 0)
		{
			pDC->MoveTo(rect.left, y);
			pDC->LineTo(rect.right, y);
		}
	}

	// Outlines
	CPen penSolid;
	penSolid.CreatePen(PS_SOLID, 1, m_gridColor);
	pDC->SelectObject(&penSolid);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.left, rect.top);

	pDC->SelectObject(pOldPen);
	pDC->SetBkColor(oldBkColor);
}

void CDrawView::OnInitialUpdate()
{
	CSize size = GetDocument()->GetSize();
	CClientDC dc(NULL);
	size.cx =
		MulDiv(
			size.cx,
			::GetDeviceCaps( dc.m_hDC, LOGPIXELSX ),
			100
			);
	size.cy =
		MulDiv(
			size.cy,
			::GetDeviceCaps( dc.m_hDC, LOGPIXELSY ),
			100
			);
	SetScrollSizes( MM_TEXT, size, sizeDefault, CSize( 1, 1 ) );
}

void CDrawView::SetPageSize(CSize size)
{
	CClientDC dc(NULL);
	size.cx =
		MulDiv(
			size.cx,
			::GetDeviceCaps( dc.m_hDC, LOGPIXELSX ),
			100
			);
	size.cy =
		MulDiv(
			size.cy,
			::GetDeviceCaps( dc.m_hDC, LOGPIXELSY ),
			100
			);
	SetScrollSizes(MM_TEXT, size);
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_WINDOW, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView printing

BOOL CDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if( ! DoPreparePrinting( pInfo ) )
		return FALSE;
WORD nPageNoMin = 1, nPageNoMax = 1;
	pInfo->m_nNumPreviewPages    = nPageNoMax;
	pInfo->m_pPD->m_pd.nMinPage  = nPageNoMin;
	pInfo->m_pPD->m_pd.nFromPage = nPageNoMin;
	pInfo->m_pPD->m_pd.nMaxPage  = nPageNoMax;
	pInfo->m_pPD->m_pd.nToPage   = nPageNoMax;
	return TRUE;
}

void CDrawView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView :: OnBeginPrinting( pDC, pInfo );

	// check page size -- user could have gone into print setup
	// from print dialog and changed paper or orientation
	GetDocument()->ComputePageSize();
}

void CDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CDrawView::IsSelected(const CObject* pDocItem) const
{
	CDrawObj* pDrawObj = (CDrawObj*)pDocItem;
	if (pDocItem->IsKindOf(RUNTIME_CLASS(CDrawItem)))
		pDrawObj = ((CDrawItem*)pDocItem)->m_pDrawObj;
	return m_selection.Find(pDrawObj) != NULL;
}

void CDrawView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CDrawItem object.
COleInsertDialog dlgOleInsert;
	if( dlgOleInsert.DoModal() != IDOK )
		return;

	BeginWaitCursor();

	// First create the C++ object
	CDrawOleObj* pObj = new CDrawOleObj(GetInitialPosition());
	ASSERT_VALID(pObj);
	CDrawItem* pItem = new CDrawItem(GetDocument(), pObj);
	ASSERT_VALID(pItem);
	pObj->m_pClientItem = pItem;

	// Now create the OLE object/item
	TRY
	{
		if( ! dlgOleInsert.CreateItem(pObj->m_pClientItem ) )
			AfxThrowMemoryException();

		// add the object to the document
		GetDocument()->Add(pObj);

		// try to get initial presentation data
		pItem->UpdateLink();
		pItem->UpdateExtent();

		// if insert new object -- initially show the object
		if (dlgOleInsert.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);
	}
	CATCH_ALL(e)
	{
		// clean up item
		pItem->Delete();
		pObj->m_pClientItem = NULL;
		GetDocument()->Remove(pObj);
		pObj->Remove();

		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH_ALL

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.
void CDrawView::OnCancelEdit()
{
	// deactivate any in-place active item on this view!
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		// if we found one, deactivate it
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	// escape also brings us back into select mode
	ReleaseCapture();
CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnCancel();
	CDrawTool::c_drawShape = selection;
}

void CDrawView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();
			return;
		}
	}

	__BASE_OF_CDrawView__ :: OnSetFocus( pOldWnd );
}

CRect CDrawView::GetInitialPosition()
{
	CRect rect(10, 10, 10, 10);
	ClientToDoc(rect);
	return rect;
}

void CDrawView::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CDrawView::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom <= rect.top);
}

void CDrawView::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CDrawView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

void CDrawView::Select(CDrawObj* pObj, BOOL bAdd)
{
	if (!bAdd)
	{
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	}

	if (pObj == NULL || IsSelected(pObj))
		return;

	m_selection.AddTail(pObj);
	InvalObj(pObj);
}

// rect is in device coordinates
void CDrawView::SelectWithinRect(CRect rect, BOOL bAdd)
{
	if (!bAdd)
		Select(NULL);

	ClientToDoc(rect);

	CDrawObjList* pObList = GetDocument()->GetObjects();
	POSITION posObj = pObList->GetHeadPosition();
	while (posObj != NULL)
	{
		CDrawObj* pObj = pObList->GetNext(posObj);
		if (pObj->Intersects(rect))
			Select(pObj, TRUE);
	}
}

void CDrawView::Deselect(CDrawObj* pObj)
{
	POSITION pos = m_selection.Find(pObj);
	if (pos != NULL)
	{
		InvalObj(pObj);
		m_selection.RemoveAt(pos);
	}
}

void CDrawView::CloneSelection()
{
	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		pObj->Clone(pObj->m_pDocument);
			// copies object and adds it to the document
	}
}

void CDrawView::UpdateActiveItem()
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// this will update the item rectangles by calling
		//  OnGetPosRect & OnGetClipRect.
		pActiveItem->SetItemRects();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView message handlers

void CDrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonDown(this, nFlags, point);
	SyncToolbarObjButtons();
}

void CDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonUp(this, nFlags, point);
	SyncToolbarObjButtons();
}

void CDrawView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnMouseMove(this, nFlags, point);
}

void CDrawView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!m_bActive)
		return;
	CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonDblClk(this, nFlags, point);
}

void CDrawView::OnDestroy()
{
	__BASE_OF_CDrawView__ :: OnDestroy();

	// deactivate the inplace active item on this view
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
	{
		pActiveItem->Deactivate();
		ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	}
}

void CDrawView::OnDrawSelect()
{
	CDrawTool::c_drawShape = selection;
}

void CDrawView::OnDrawRoundRect()
{
	CDrawTool::c_drawShape = roundRect;
}

void CDrawView::OnDrawRect()
{
	CDrawTool::c_drawShape = rect;
}

void CDrawView::OnDrawLine()
{
	CDrawTool::c_drawShape = line;
}

void CDrawView::OnDrawEllipse()
{
	CDrawTool::c_drawShape = ellipse;
}

void CDrawView::OnDrawPolygon()
{
	CDrawTool::c_drawShape = poly;
}

void CDrawView::OnUpdateDrawEllipse(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetRadio();
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->SetRadio(CDrawTool::c_drawShape == ellipse);
}

void CDrawView::OnUpdateDrawLine(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetRadio();
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->SetRadio(CDrawTool::c_drawShape == line);
}

void CDrawView::OnUpdateDrawRect(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetRadio();
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->SetRadio(CDrawTool::c_drawShape == rect);
}

void CDrawView::OnUpdateDrawRoundRect(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetRadio();
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->SetRadio(CDrawTool::c_drawShape == roundRect);
}

void CDrawView::OnUpdateDrawSelect(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetRadio();
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable();
	pCmdUI->SetRadio(CDrawTool::c_drawShape == selection);
}

void CDrawView::OnUpdateSingleSelect(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetCheck( 0 );
		return;
	}
	pCmdUI->Enable( m_selection.GetCount() == 1 && GetDocument()->GetObjects()->GetCount() > 1 );
}

void CDrawView::OnEditSelectAll()
{
	CDrawObjList* pObList = GetDocument()->GetObjects();
	POSITION pos = pObList->GetHeadPosition();
	while (pos != NULL)
		Select(pObList->GetNext(pos), TRUE);
}

void CDrawView::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable(GetDocument()->GetObjects()->GetCount() != 0);
}

void CDrawView::OnEditClear()
{
	// update all the views before the selection goes away
	GetDocument()->UpdateAllViews(NULL, HINT_DELETE_SELECTION, &m_selection);
	OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);

	// now remove the selection from the document
	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawObj* pObj = m_selection.GetNext(pos);
		GetDocument()->Remove(pObj);
		pObj->Remove();
	}
	m_selection.RemoveAll();
}

void CDrawView::OnUpdateAnySelect(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable(!m_selection.IsEmpty());
}

void CDrawView::OnUpdateDrawPolygon(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetRadio();
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->SetRadio(CDrawTool::c_drawShape == poly);
}

void CDrawView::OnSize(UINT nType, int cx, int cy)
{
	__BASE_OF_CDrawView__ :: OnSize( nType, cx, cy );
	UpdateActiveItem();
//	if(		m_wndScrollBarH.GetSafeHwnd() != NULL
//		||	m_wndScrollBarV.GetSafeHwnd() != NULL
//		)
//		RepositionBars( 0, 0xFFFF, 0 );
}

void CDrawView::OnViewGrid()
{
	m_bGrid = !m_bGrid;
	Invalidate(FALSE);
}

void CDrawView::OnUpdateViewGrid(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_bGrid);

}

BOOL CDrawView::OnEraseBkgnd( CDC * pDC )
{
	pDC;
	return TRUE;
}

void CDrawView::OnObjectFillColor()
{
int nSelCount = int( m_selection.GetCount() );
	if( nSelCount == 0 )
	{
		ASSERT( FALSE ); // should be disabled
		return;
	}
POSITION pos = m_selection.GetHeadPosition();
	ASSERT( pos != NULL );
CDrawObj * pObj = m_selection.GetNext( pos );
	ASSERT( pObj != NULL );
CExtColorDlg dlgColor;
	dlgColor.m_clrInit
		= dlgColor.m_clrNew
		= pObj->GetFillColor();
	VERIFY(
		dlgColor.m_strCaption.LoadString( ID_OBJECT_FILLCOLOR )
		);
	if( dlgColor.DoModal() != IDOK )
		return;
	pos = m_selection.GetHeadPosition();
	ASSERT( pos != NULL );
	while( pos != NULL )
	{
		pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
		pObj->SetNoFill( FALSE );
		pObj->SetFillColor( dlgColor.m_clrNew );
	}
	SyncToolbarObjButtons();
}

void CDrawView::OnObjectLineColor()
{
int nSelCount = int( m_selection.GetCount() );
	if( nSelCount == 0 )
	{
		ASSERT( FALSE ); // should be disabled
		return;
	}
POSITION pos = m_selection.GetHeadPosition();
	ASSERT( pos != NULL );
CDrawObj * pObj = m_selection.GetNext( pos );
	ASSERT( pObj != NULL );
CExtColorDlg dlgColor;
	dlgColor.m_clrInit
		= dlgColor.m_clrNew
		= pObj->GetLineColor();
	VERIFY(
		dlgColor.m_strCaption.LoadString( ID_OBJECT_LINECOLOR )
		);
	if( dlgColor.DoModal() != IDOK )
		return;
	pos = m_selection.GetHeadPosition();
	ASSERT( pos != NULL );
	while( pos != NULL )
	{
		pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
		pObj->SetNoOutline( FALSE );
		pObj->SetLineColor( dlgColor.m_clrNew );
	}
	SyncToolbarObjButtons();
}

void CDrawView::OnObjectMoveBack()
{
	CDrawDoc* pDoc = GetDocument();
	CDrawObj* pObj = m_selection.GetHead();
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(pObj);
	ASSERT(pos != NULL);
	if (pos != pObjects->GetHeadPosition())
	{
		POSITION posPrev = pos;
		pObjects->GetPrev(posPrev);
		pObjects->RemoveAt(pos);
		pObjects->InsertBefore(posPrev, pObj);
		InvalObj(pObj);
	}
}

void CDrawView::OnObjectMoveForward()
{
	CDrawDoc* pDoc = GetDocument();
	CDrawObj* pObj = m_selection.GetHead();
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(pObj);
	ASSERT(pos != NULL);
	if (pos != pObjects->GetTailPosition())
	{
		POSITION posNext = pos;
		pObjects->GetNext(posNext);
		pObjects->RemoveAt(pos);
		pObjects->InsertAfter(posNext, pObj);
		InvalObj(pObj);
	}
}

void CDrawView::OnObjectMoveToBack()
{
	CDrawDoc* pDoc = GetDocument();
	CDrawObj* pObj = m_selection.GetHead();
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(pObj);
	ASSERT(pos != NULL);
	pObjects->RemoveAt(pos);
	pObjects->AddHead(pObj);
	InvalObj(pObj);
}

void CDrawView::OnObjectMoveToFront()
{
	CDrawDoc* pDoc = GetDocument();
	CDrawObj* pObj = m_selection.GetHead();
	CDrawObjList* pObjects = pDoc->GetObjects();
	POSITION pos = pObjects->Find(pObj);
	ASSERT(pos != NULL);
	pObjects->RemoveAt(pos);
	pObjects->AddTail(pObj);
	InvalObj(pObj);
}

void CDrawView::OnEditCopy()
{
	ASSERT_VALID(this);
	ASSERT(m_cfDraw != NULL);

	// Create a shared file and associate a CArchive with it
	CSharedFile file;
	CArchive ar(&file, CArchive::store);

	// Serialize selected objects to the archive
	m_selection.Serialize(ar);
	ar.Close();

	COleDataSource* pDataSource = NULL;
	TRY
	{
		pDataSource = new COleDataSource;
		// put on local format instead of or in addation to
		pDataSource->CacheGlobalData(m_cfDraw, file.Detach());

		// if only one item and it is a COleClientItem then also
		// paste in that format
		CDrawObj* pDrawObj = m_selection.GetHead();
		if (m_selection.GetCount() == 1 &&
			pDrawObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
		{
			CDrawOleObj* pDrawOle = (CDrawOleObj*)pDrawObj;
			pDrawOle->m_pClientItem->GetClipboardData(pDataSource, FALSE);
		}
		pDataSource->SetClipboard();
	}
	CATCH_ALL(e)
	{
		delete pDataSource;
		THROW_LAST();
	}
	END_CATCH_ALL
}

void CDrawView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable(!m_selection.IsEmpty());
}

void CDrawView::OnEditCut()
{
	OnEditCopy();
	OnEditClear();
	OnDrawSelect();
}

void CDrawView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable(!m_selection.IsEmpty());
}

void CDrawView::OnEditPaste()
{
	COleDataObject dataObject;
	dataObject.AttachClipboard();

	// invalidate current selection since it will be deselected
	OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	m_selection.RemoveAll();
	if (dataObject.IsDataAvailable(m_cfDraw))
	{
		PasteNative(dataObject);

		// now add all items in m_selection to document
		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
			GetDocument()->Add(m_selection.GetNext(pos));
	}
	else
		PasteEmbedded(dataObject, GetInitialPosition().TopLeft() );

	GetDocument()->SetModifiedFlag();

	// invalidate new pasted stuff
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_SELECTION, &m_selection);
}

void CDrawView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}

	// determine if private or standard OLE formats are on the clipboard
	COleDataObject dataObject;
	BOOL bEnable = dataObject.AttachClipboard() &&
		(dataObject.IsDataAvailable(m_cfDraw) ||
		 COleClientItem::CanCreateFromData(&dataObject));

	// enable command based on availability
	pCmdUI->Enable(bEnable);
}

void CDrawView::OnViewShowObjects()
{
	CDrawOleObj::c_bShowItems = !CDrawOleObj::c_bShowItems;
	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_OLE_ITEMS, NULL);
}

void CDrawView::OnUpdateViewShowObjects(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetCheck( 0 );
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable();
	pCmdUI->SetCheck(CDrawOleObj::c_bShowItems);
}

void CDrawView::OnEditProperties()
{
	if (m_selection.GetCount() == 1 && CDrawTool::c_drawShape == selection)
	{
		CDrawTool* pTool =
			CDrawTool::FindTool(
				CDrawTool::c_drawShape
				);
		ASSERT(pTool != NULL);
		pTool->OnEditProperties(this);
	}
}

void CDrawView::OnUpdateEditProperties(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable( m_selection.GetCount() == 1 && CDrawTool::c_drawShape == selection );
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView diagnostics

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	__BASE_OF_CDrawView__ :: AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	__BASE_OF_CDrawView__ :: Dump( dc );
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// new
// support for drag/drop

int CDrawView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( __BASE_OF_CDrawView__ :: OnCreate( lpCreateStruct ) == -1 )
		return -1;

//CFrameWnd * pWndFrame = GetParentFrame();
//	if(		pWndFrame == NULL
//		||	(! pWndFrame->IsKindOf( RUNTIME_CLASS(CMDIChildWnd) ) )
//		||	pWndFrame->IsKindOf( RUNTIME_CLASS(CDrawInPlaceFrame) )
//		)
//	{
//		m_wndScrollBarH.m_eSO = CExtScrollBar::__ESO_BOTTOM;
//		m_wndScrollBarV.m_eSO = CExtScrollBar::__ESO_RIGHT;
//		if( ! m_wndScrollBarV.Create(
//				WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_RIGHTALIGN,
//				CRect(0,0,0,0),
//				this,
//				1
//				)
//			)
//		{
//			ASSERT( FALSE );
//			return -1;
//		}
//		if( ! m_wndScrollBarH.Create(
//				WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_BOTTOMALIGN,
//				CRect(0,0,0,0),
//				this,
//				2
//				)
//			)
//		{
//			ASSERT( FALSE );
//			return -1;
//		}
//		m_wndScrollBarH.SyncReservedSpace( &m_wndScrollBarV );
//		m_wndScrollBarV.SyncReservedSpace( &m_wndScrollBarH );
//		ModifyStyle( WS_HSCROLL|WS_VSCROLL, 0, SWP_FRAMECHANGED );
//	}

	// register drop target
	if( m_dropTarget.Register( this ) )
		return 0;
	else
		return -1;
}

CScrollBar* CDrawView::GetScrollBarCtrl(int nBar) const
{
	ASSERT_VALID( this );
	if( m_hWnd == NULL || (! ::IsWindow(m_hWnd) ) )
		return NULL;
//	ASSERT( nBar == SB_HORZ || nBar == SB_VERT );
//	if( nBar == SB_HORZ )
//	{
//		if( m_wndScrollBarH.GetSafeHwnd() != NULL )
//			return ( const_cast < CExtScrollBar * > ( &m_wndScrollBarH ) );
//	} // if( nBar == SB_HORZ )
//	else if( nBar == SB_VERT )
//	{
//		if( m_wndScrollBarV.GetSafeHwnd() != NULL )
//			return ( const_cast < CExtScrollBar * > ( &m_wndScrollBarV ) );
//	} // else from if( nBar == SB_HORZ )
	return __BASE_OF_CDrawView__ :: GetScrollBarCtrl( nBar );
}

BOOL CDrawView::GetObjectInfo(COleDataObject* pDataObject,
	CSize* pSize, CSize* pOffset)
{
	ASSERT(pSize != NULL);

	// get object descriptor data
	HGLOBAL hObjDesc = pDataObject->GetGlobalData(m_cfObjectDescriptor);
	if (hObjDesc == NULL)
	{
		if (pOffset != NULL)
			*pOffset = CSize(0, 0); // fill in defaults instead
		*pSize = CSize(0, 0);
		return FALSE;
	}
	ASSERT(hObjDesc != NULL);

	// otherwise, got CF_OBJECTDESCRIPTOR ok.  Lock it down and extract size.
	LPOBJECTDESCRIPTOR pObjDesc = (LPOBJECTDESCRIPTOR)GlobalLock(hObjDesc);
	ASSERT(pObjDesc != NULL);
	pSize->cx = (int)pObjDesc->sizel.cx;
	pSize->cy = (int)pObjDesc->sizel.cy;
	if (pOffset != NULL)
	{
		pOffset->cx = (int)pObjDesc->pointl.x;
		pOffset->cy = (int)pObjDesc->pointl.y;
	}
	GlobalUnlock(hObjDesc);
	GlobalFree(hObjDesc);

	// successfully retrieved pSize & pOffset info
	return TRUE;
}

DROPEFFECT CDrawView::OnDragEnter(COleDataObject* pDataObject,
	DWORD grfKeyState, CPoint point)
{
	ASSERT(m_prevDropEffect == DROPEFFECT_NONE);
	m_bDragDataAcceptable = FALSE;
	if (!COleClientItem::CanCreateFromData(pDataObject))
		return DROPEFFECT_NONE;

	m_bDragDataAcceptable = TRUE;
	GetObjectInfo(pDataObject, &m_dragSize, &m_dragOffset);
	CClientDC dc(NULL);
	dc.HIMETRICtoDP(&m_dragSize);
	dc.HIMETRICtoDP(&m_dragOffset);

	return OnDragOver(pDataObject, grfKeyState, point);
}

DROPEFFECT CDrawView::OnDragOver(COleDataObject*,
	DWORD grfKeyState, CPoint point)
{
	if(m_bDragDataAcceptable == FALSE)
		return DROPEFFECT_NONE;

	point -= m_dragOffset;  // adjust target rect by original cursor offset

	// check for point outside logical area -- i.e. in hatched region
	// GetTotalSize() returns the size passed to SetScrollSizes
	CRect rectScroll(CPoint(0, 0), GetTotalSize());

	CRect rectItem(point,m_dragSize);
	rectItem.OffsetRect(GetDeviceScrollPosition());

	DROPEFFECT de = DROPEFFECT_NONE;
	CRect rectTemp;
	if (rectTemp.IntersectRect(rectScroll, rectItem))
	{
		// check for force link
		if ((grfKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
			de = DROPEFFECT_NONE; // DRAWCLI isn't a linking container
		// check for force copy
		else if ((grfKeyState & MK_CONTROL) == MK_CONTROL)
			de = DROPEFFECT_COPY;
		// check for force move
		else if ((grfKeyState & MK_ALT) == MK_ALT)
			de = DROPEFFECT_MOVE;
		// default -- recommended action is move
		else
			de = DROPEFFECT_MOVE;
	}

	if (point == m_dragPoint)
		return de;

	// otherwise, cursor has moved -- need to update the drag feedback
	CClientDC dc(this);
	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		// erase previous focus rect
		dc.DrawFocusRect(CRect(m_dragPoint, m_dragSize));
	}
	m_prevDropEffect = de;
	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		m_dragPoint = point;
		dc.DrawFocusRect(CRect(point, m_dragSize));
	}
	return de;
}

BOOL CDrawView::OnDrop(COleDataObject* pDataObject,
	DROPEFFECT /*dropEffect*/, CPoint point)
{
	ASSERT_VALID(this);

	// clean up focus rect
	OnDragLeave();

	// offset point as appropriate for dragging
	GetObjectInfo(pDataObject, &m_dragSize, &m_dragOffset);
	CClientDC dc(NULL);
	dc.HIMETRICtoDP(&m_dragSize);
	dc.HIMETRICtoDP(&m_dragOffset);
	point -= m_dragOffset;

	// invalidate current selection since it will be deselected
	OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
	m_selection.RemoveAll();
	if (m_bDragDataAcceptable)
		PasteEmbedded(*pDataObject, point);

	// update the document and views
	GetDocument()->SetModifiedFlag();
	GetDocument()->UpdateAllViews(NULL, 0, NULL);      // including this view

	return TRUE;
}

void CDrawView::OnDragLeave()
{
	CClientDC dc(this);
	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		dc.DrawFocusRect(CRect(m_dragPoint,m_dragSize)); // erase previous focus rect
		m_prevDropEffect = DROPEFFECT_NONE;
	}
}


void CDrawView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	if( IsPrintPreviewMode() )
		return;
CDrawDoc * pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	// make sure window is active
	GetParentFrame()->ActivateFrame();

CPoint ptLocal = point;
	ScreenToClient( &ptLocal );
	ClientToDoc( ptLocal );

bool bTryProcessItemFocus = !( (point.x < 0) || (point.y < 0) );
CDrawObj * pObj = NULL;
	if( bTryProcessItemFocus )
	{
		pObj = GetDocument()->ObjectAt( ptLocal );
		if( pObj == NULL )
			return;
		if( !IsSelected(pObj) )
			Select( pObj, FALSE ); // reselect item if appropriate
		Invalidate();
		UpdateWindow();
	} // if( bTryProcessItemFocus )
	else
	{
		POSITION pos = m_selection.GetHeadPosition();
		if( pos == NULL )
			return;
		pObj = m_selection.GetNext( pos );
	} // else from if( bTryProcessItemFocus )

	ASSERT( pObj != NULL );

	SyncToolbarObjButtons();

CMenu menu;
	if( !menu.LoadMenu(ID_POPUP_MENU) )
	{
		ASSERT( FALSE );
		return;
	}

HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
	ASSERT( hWnd != NULL );
	ASSERT( ::IsWindow( hWnd ) );

CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	VERIFY( pPopup->UpdateFromMenu( hWnd, &menu ) );

	VERIFY(
		pPopup->TrackPopupMenu(
			TPMX_OWNERDRAW_FIXED|TPMX_HIDE_KEYBOARD_ACCELERATORS,
			point.x,
			point.y
			)
		);
}

void CDrawView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if (pInfo->m_bPreview == FALSE)
		((CDrawDoc*)GetDocument())->m_pSummInfo->RecordPrintDate();
	OnDraw(pDC);
}

LRESULT CDrawView::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
{
	lParam;
CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
		reinterpret_cast
			< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
			( wParam );
	ASSERT( pData != NULL );
CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );

	if( ( pPopup->TrackFlagsGet() & TPMX_NO_CMD_UI ) != 0 )
		return 0;

CDrawDoc * pDoc = GetDocument();
	ASSERT_VALID( pDoc );

CDrawObj * pObj = NULL;
POSITION pos = m_selection.GetHeadPosition();
	if( pos != NULL )
	{
		pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
	}

INT nDefaultItemPos =
		pPopup->ItemFindPosForCmdID(ID_EDIT_PROPERTIES);
	if( nDefaultItemPos >= 0 )
		pPopup->ItemDefaultSet(nDefaultItemPos);

INT nNextPos, nReplacePos =
		pPopup->ItemFindPosForCmdID( ID_OLE_VERB_FIRST, -1 );
	for(	nNextPos = nReplacePos;
			nReplacePos >= 0;
			nNextPos = nReplacePos =
				pPopup->ItemFindPosForCmdID( ID_OLE_VERB_FIRST, nNextPos + 1 )
		)
	{
		CDrawOleObj * pDrawOleObj =
			DYNAMIC_DOWNCAST(CDrawOleObj,pObj);
		if( pDrawOleObj != NULL )
		{ // if menu for OLE object
			// delete OLE verb entry
			VERIFY( pPopup->ItemRemove( nReplacePos ) );
			CMenu menu;
			VERIFY( menu.CreatePopupMenu() );
			ASSERT( menu.GetSafeHmenu() != NULL );
			CMenu * pMenuPopup = &menu; // menu.GetSubMenu(0);
			//ASSERT( pMenuPopup->GetSafeHmenu() != NULL );
			VERIFY( pMenuPopup->AppendMenu( 0, ID_OLE_VERB_FIRST ) );
			nNextPos ++;
			UINT nConvertID = ID_OLE_EDIT_CONVERT;
			AFX_CMDHANDLERINFO info;
			if( ! pDoc->OnCmdMsg( ID_OLE_EDIT_CONVERT, CN_COMMAND, NULL, &info ) )
				nConvertID = 0;
			ASSERT( pDrawOleObj->m_pClientItem != NULL );
			ASSERT( ::IsMenu( pMenuPopup->GetSafeHmenu() ) );
			AfxOleSetEditMenu(
				pDrawOleObj->m_pClientItem,
				pMenuPopup, // &menu,
				pMenuPopup->GetMenuItemCount() - 1, // First OLE cmd
				ID_OLE_VERB_FIRST,
				ID_OLE_VERB_LAST,
				nConvertID
				);
			const CExtCmdIcon & _iconOLE =
				g_CmdManager->CmdGetIcon(
					g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
					ID_OLE_INSERT_NEW
					);
			CString sOlePopupText;
			pMenuPopup->GetMenuString(0,sOlePopupText,MF_BYPOSITION);
			ASSERT( !sOlePopupText.IsEmpty() );
			VERIFY(
				pPopup->ItemInsert(
					(UINT)CExtPopupMenuWnd::TYPE_POPUP,
					nReplacePos,
					(LPCTSTR)sOlePopupText,
					_iconOLE
					)
				);
			nNextPos ++;
			pPopup->ItemSetDisplayed( nReplacePos, true );
			CExtPopupMenuWnd * pOlePopup = pPopup->ItemGetPopup( nReplacePos );
			ASSERT( pOlePopup != NULL );
			if( pOlePopup != NULL )
			{
				if( ! pOlePopup->UpdateFromMenu(
						AfxGetMainWnd()->GetSafeHwnd(),
						&menu,
						true,
						false,
						true
						)
					)
				{
					pPopup->ItemRemove( nReplacePos );
					nNextPos ++;
				}
				else
					pOlePopup->TrackFlagsSet( pOlePopup->TrackFlagsGet() | TPMX_NO_CMD_UI );
			} // if( pOlePopup != NULL )
		} // if menu for OLE object
		else
		{ // if menu for internal object
			// delete OLE verb entry
			VERIFY( pPopup->ItemRemove( nReplacePos ) );
			nNextPos --;
			if(		nReplacePos > 0
				&&	pPopup->ItemGetCmdID( nReplacePos - 1 ) == ID_SEPARATOR
				)
			{	// delete separator before OLE verb entry
				VERIFY( pPopup->ItemRemove( nReplacePos - 1 ) );
				nNextPos --;
			}	// delete separator before OLE verb entry
		} // if menu for internal object
	}

	if( pObj != NULL )
	{
		CExtPopupColorMenuWnd * pColorPopup;
		CString sNoColor;

		nReplacePos =
			pPopup->ItemFindPosForCmdID(ID_OBJECT_FILLCOLOR);
		if( nReplacePos >= 0 )
		{
			VERIFY( sNoColor.LoadString(IDS_STRING_NO_COLOR_FILL) );
			ASSERT( !sNoColor.IsEmpty() );
			pColorPopup = new CExtPopupColorMenuWnd;
			pColorPopup->m_sBtnTextColorDefault = sNoColor;
			pColorPopup->m_lParamCookie = LPARAM(ID_OBJECT_FILLCOLOR);
			pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
			pColorPopup->m_clrDefault = COLORREF(-1);
			pColorPopup->m_clrInitial =
				pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor();
			VERIFY(
				pPopup->ItemInsertSpecPopup(
					pColorPopup,
					nReplacePos + 1,
					pPopup->ItemGetText(nReplacePos),
					pPopup->ItemGetIcon(nReplacePos)
					)
				);
			pPopup->ItemSetDisplayed( nReplacePos + 1, true );
			VERIFY( pPopup->ItemRemove(nReplacePos) );
		} // if( nReplacePos >= 0 )

		nReplacePos =
			pPopup->ItemFindPosForCmdID(ID_OBJECT_LINECOLOR);
		if( nReplacePos >= 0 )
		{
			VERIFY( sNoColor.LoadString(IDS_STRING_NO_COLOR_OUTLINE) );
			ASSERT( !sNoColor.IsEmpty() );
			pColorPopup = new CExtPopupColorMenuWnd;
			pColorPopup->m_sBtnTextColorDefault = sNoColor;
			//pColorPopup->m_bEnableBtnColorDefault = false;
			pColorPopup->m_lParamCookie = LPARAM(ID_OBJECT_LINECOLOR);
			pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
			pColorPopup->m_clrDefault = COLORREF(-1);
			pColorPopup->m_clrInitial =
				pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor();
			VERIFY(
				pPopup->ItemInsertSpecPopup(
					pColorPopup,
					nReplacePos + 1,
					pPopup->ItemGetText(nReplacePos),
					pPopup->ItemGetIcon(nReplacePos)
					)
				);
			pPopup->ItemSetDisplayed( nReplacePos + 1, true );
			VERIFY( pPopup->ItemRemove(nReplacePos) );
		} // if( nReplacePos >= 0 )

	} // if( pObj != NULL )

	nReplacePos = pPopup->ItemFindByText( _T("Line &Width...") );
	if( nReplacePos >= 0 )
	{
		const CExtCmdIcon & _icon =
			g_CmdManager->CmdGetIcon(
				g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
				ID_DOC_LINE_WIDTH
				);
		pPopup->ItemSetPopupIcon( nReplacePos, _icon );
	}
	

	return TRUE;
}

LRESULT CDrawView::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
POSITION pos = m_selection.GetHeadPosition();
	ASSERT( pos != NULL );
	while( pos != NULL )
	{
		CDrawObj * pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
		switch( lParam )
		{
		case ID_OBJECT_FILLCOLOR:
			if( COLORREF(wParam) == COLORREF(-1) )
				pObj->SetNoFill();
			else
			{
				pObj->SetNoFill( FALSE );
				pObj->SetFillColor( COLORREF(wParam) );
			}
		break;
		case ID_OBJECT_LINECOLOR:
			if( COLORREF(wParam) == COLORREF(-1) )
				pObj->SetNoOutline();
			else
			{
				pObj->SetNoOutline( FALSE );
				pObj->SetLineColor( COLORREF(wParam) );
			}
		break;
		} // switch( lParam )
	}
	Invalidate();
	UpdateWindow();
	SyncToolbarObjButtons();
	return 0;
}

LRESULT CDrawView::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
{
	wParam;
	switch( lParam )
	{
	case ID_OBJECT_FILLCOLOR:
	case ID_OBJECT_LINECOLOR:
		PostMessage(WM_COMMAND,lParam,0);
	break;
	} // switch( lParam )
	return 0;
}

void CDrawView::OnUpdateSelectionSingle(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
int nSelCount = int( m_selection.GetCount() );
	pCmdUI->Enable( nSelCount == 1 );
}

void CDrawView::OnUpdateSelectionNonEmpty(CCmdUI* pCmdUI)
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
int nSelCount = int( m_selection.GetCount() );
	pCmdUI->Enable( nSelCount > 0 );
}

void CDrawView::OnLineWidth( UINT nCmdID )
{
	ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );
#ifdef _DEBUG
int nSelCount = int( m_selection.GetCount() );
	ASSERT( nSelCount > 0 );
#endif // _DEBUG

POSITION pos = m_selection.GetHeadPosition();
	ASSERT( pos != NULL );

	while( pos != NULL )
	{
		CDrawObj * pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
	
		switch( nCmdID )
		{
		case ID_LINE_WIDTH_OTHER:
				ASSERT( nSelCount == 1 );
				if( CDrawTool::c_drawShape == selection )
					PostMessage( WM_COMMAND, ID_EDIT_PROPERTIES );
			return;
		case ID_LINE_WIDTH_0:
				pObj->SetNoOutline( TRUE );
			break;
		default:
			{
				ASSERT( ID_LINE_WIDTH_1 <= nCmdID && nCmdID <= ID_LINE_WIDTH_10 );
				UINT nLineWidth = nCmdID - ID_LINE_WIDTH_0;
				ASSERT( 1 <= nLineWidth && nLineWidth <= 10 );
				pObj->SetNoOutline( FALSE );
				pObj->m_logpen.lopnWidth.x = nLineWidth;
				pObj->m_logpen.lopnWidth.y = nLineWidth;
			}
			break;
		} // switch( nCmdID )

	} // while( pos != NULL )

	Invalidate();
}

void CDrawView::OnUpdateLineWidth( CCmdUI * pCmdUI )
{
	ASSERT( pCmdUI != NULL );
	if( IsPrintPreviewMode() )
	{
		pCmdUI->SetRadio( 0 );
		pCmdUI->Enable( FALSE );
		return;
	}
UINT nCmdID = pCmdUI->m_nID;
	ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );
int nSelCount = int( m_selection.GetCount() );

CDrawObj * pObj = NULL;
	if( nSelCount > 0 )
	{
		POSITION pos = m_selection.GetHeadPosition();
		ASSERT( pos != NULL );
		pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
	}
	else
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetRadio( FALSE );
	}

	if( nCmdID == ID_LINE_WIDTH_OTHER )
	{
		pCmdUI->Enable( nSelCount == 1 && m_selection.GetCount() == 1 && CDrawTool::c_drawShape == selection );
		if( pObj != NULL )
			pCmdUI->SetRadio( pObj->m_logpen.lopnWidth.x > 10 );
		return;
	}

	pCmdUI->Enable( nSelCount > 0 );
	
	if( nCmdID == ID_LINE_WIDTH_0 )
	{
		if( pObj != NULL )
			pCmdUI->SetRadio( pObj->IsNoOutline() );
		return;
	}

	ASSERT( ID_LINE_WIDTH_1 <= nCmdID && nCmdID <= ID_LINE_WIDTH_10 );
UINT nLineWidth = nCmdID - ID_LINE_WIDTH_0;
	ASSERT( 1 <= nLineWidth && nLineWidth <= 10 );
	pCmdUI->Enable( nSelCount > 0 );
	if( pObj != NULL )
		pCmdUI->SetRadio( UINT(pObj->m_logpen.lopnWidth.x) == nLineWidth );

}

LRESULT CDrawView::OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam)
{
	wParam;

CExtPopupMenuWnd::DRAWITEMDATA * pDrawItemData =
		reinterpret_cast < CExtPopupMenuWnd::DRAWITEMDATA * > ( lParam );
	ASSERT( pDrawItemData != NULL );
UINT nCmdID = pDrawItemData->GetCmdID();
	if( ! ( ID_LINE_WIDTH_1 <= nCmdID && nCmdID <= ID_LINE_WIDTH_10 ) )
		return FALSE; // default painting

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
const CExtCustomizeCmdTreeNode * pNode =
		pDrawItemData->m_pItemData->GetCmdNode();
	if( pNode != NULL )
	{
		ASSERT_VALID( pNode );
		if( (pNode->GetFlags() & __ECTN_TBB_SEPARATED_DROPDOWN) != 0 )
			return FALSE;
	} // if( pNode != NULL )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)


	// paint default menu item background
	pDrawItemData->PaintDefault(
		false,
		true,
		false,
		false,
		false
		);


UINT nLineWidth = nCmdID - ID_LINE_WIDTH_0;
	ASSERT( 1 <= nLineWidth && nLineWidth <= 10 );

INT nIconAreaWidth = g_PaintManager->GetMenuIconAreaWidth();
	if( nIconAreaWidth < 0 )
	{
		nIconAreaWidth = __EXT_MENU_ICON_CX + __EXT_MENU_GAP * 2;
		nIconAreaWidth = g_PaintManager->UiScalingDo( nIconAreaWidth, CExtPaintManager::__EUIST_X );
	} 

bool bRTL = pDrawItemData->m_pPopup->OnQueryLayoutRTL();
CRect rcItem = LPCRECT(*pDrawItemData);
	if( bRTL )
		rcItem.DeflateRect( 2, 0, nIconAreaWidth + 4, 0 );
	else
		rcItem.DeflateRect( nIconAreaWidth + 4, 0, 2, 0 );
	rcItem.top += rcItem.Height()/2;
	rcItem.top -= nLineWidth/2;
	rcItem.bottom = rcItem.top + nLineWidth;
	
	// get draw DC
CDC & dc = *( (CDC *) *pDrawItemData );

	dc.FillSolidRect(
		&rcItem,
		g_PaintManager->GetColor(
			(m_selection.GetCount() > 0)
				? COLOR_3DDKSHADOW
				: COLOR_3DSHADOW
				,
			this
			)
		);

	return TRUE;
}

void CDrawView::OnUpdateDocLineWidth(CCmdUI* pCmdUI) 
{
	if( IsPrintPreviewMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable( m_selection.GetCount() > 0 );
}

void CDrawView::SyncToolbarObjButtons()
{
	if( IsPrintPreviewMode() )
		return;
CFrameWnd * pFrame = GetParentFrame();
	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf( RUNTIME_CLASS(CMDIChildWnd) ) )
	{
		CMainFrame * pMainFrame = (CMainFrame *)pFrame->GetParentFrame();
		ASSERT_VALID( pMainFrame );
		ASSERT_KINDOF( CMainFrame, pMainFrame );
		int nSelCount = int( m_selection.GetCount() );
		CDrawObj * pObj = NULL;
		if( nSelCount > 0 )
		{
			POSITION pos = m_selection.GetHeadPosition();
			ASSERT( pos != NULL );
			pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
		} // if( nSelCount > 0 )
#if (defined __EXT_MFC_NO_CUSTOMIZE)
		if( pObj != NULL )
		{
			if(		pMainFrame->m_pBtnColorFillS != NULL
				&&	pMainFrame->m_pBtnColorFillS->ColorGet(true)
						!= pObj->GetFillColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorFillS );
				ASSERT( pMainFrame->m_pBtnColorFillP != NULL );
				ASSERT_VALID( pMainFrame->m_pBtnColorFillS );
				pMainFrame->m_pBtnColorFillS->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFillP->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFillS->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFillP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFillS->RedrawButton();
				pMainFrame->m_pBtnColorFillP->RedrawButton();
			}
			if(		pMainFrame->m_pBtnColorOutlineS != NULL
				&&	pMainFrame->m_pBtnColorOutlineS->ColorGet(false)
						!= pObj->GetLineColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorOutlineS );
				ASSERT( pMainFrame->m_pBtnColorOutlineP != NULL );
				ASSERT_VALID( pMainFrame->m_pBtnColorOutlineP );
				pMainFrame->m_pBtnColorOutlineS->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutlineS->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutlineS->RedrawButton();
				pMainFrame->m_pBtnColorOutlineP->RedrawButton();
			}

			CExtBarButton * pTBB =
				pMainFrame->m_wndToolBarStandard.GetButton(
					pMainFrame->m_wndToolBarStandard.CommandToIndex(
						ID_DOC_LINE_WIDTH
						)
					);
			ASSERT_VALID( pTBB );
			UINT nCmdID = pTBB->GetCmdID( true );
			ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );

			UINT nAdjustCmdID = ID_LINE_WIDTH_0;
			if( !pObj->IsNoOutline() )
			{
				UINT xw = (UINT)pObj->m_logpen.lopnWidth.x;
				if( xw <= 10 )
				{
					ASSERT( xw > 0 );
					nAdjustCmdID = ID_LINE_WIDTH_0 + xw;
					ASSERT( ID_LINE_WIDTH_1 <= nAdjustCmdID && nAdjustCmdID <= ID_LINE_WIDTH_10 );
				}
				else
					nAdjustCmdID = ID_LINE_WIDTH_OTHER;
			}

			if( nCmdID != nAdjustCmdID )
				pTBB->SetCmdID( nAdjustCmdID, true, true );
		} // if( pObj != NULL )

#else // (defined __EXT_MFC_NO_CUSTOMIZE)

		if( pObj != NULL )
		{
			if(		pMainFrame->m_pBtnColorFillP != NULL
				&&	pMainFrame->m_pBtnColorFillP->ColorGet(true)
						!= pObj->GetFillColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorFillP );
				pMainFrame->m_pBtnColorFillP->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFillP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFillP->RedrawButton();
			}
			if(		pMainFrame->m_pBtnColorOutlineP != NULL
				&&	pMainFrame->m_pBtnColorOutlineP->ColorGet(false)
						!= pObj->GetLineColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorOutlineP );
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutlineP->RedrawButton();
			}
		} // if( pObj != NULL )

		pMainFrame->RedrawCommandItems( ID_OBJECT_FILLCOLOR ); 
		pMainFrame->RedrawCommandItems( ID_OBJECT_LINECOLOR ); 
		pMainFrame->RedrawCommandItems( ID_LINE_WIDTH_OTHER ); 

#endif // not - (defined __EXT_MFC_NO_CUSTOMIZE)
		return;
	} // if( pFrame->IsKindOf( RUNTIME_CLASS(CMDIChildWnd) ) );
	else if( pFrame->IsKindOf( RUNTIME_CLASS(CDrawInPlaceFrame) ) )
	{
		CDrawInPlaceFrame * pMainFrame = (CDrawInPlaceFrame*)pFrame;
		ASSERT_VALID( pMainFrame );
		ASSERT_KINDOF( CDrawInPlaceFrame, pMainFrame );
		int nSelCount = int( m_selection.GetCount() );
		CDrawObj * pObj = NULL;
		if( nSelCount > 0 )
		{
			POSITION pos = m_selection.GetHeadPosition();
			ASSERT( pos != NULL );
			pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
		} // if( nSelCount > 0 )
		if( pObj != NULL )
		{
			if(		pMainFrame->m_pBtnColorFillS != NULL
				&&	pMainFrame->m_pBtnColorFillS->ColorGet(true)
						!= pObj->GetFillColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorFillS );
				ASSERT( pMainFrame->m_pBtnColorFillP != NULL );
				ASSERT_VALID( pMainFrame->m_pBtnColorFillS );
				pMainFrame->m_pBtnColorFillS->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFillP->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFillS->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFillP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFillS->RedrawButton();
				pMainFrame->m_pBtnColorFillP->RedrawButton();
			}
			if(		pMainFrame->m_pBtnColorOutlineS != NULL
				&&	pMainFrame->m_pBtnColorOutlineS->ColorGet(false)
						!= pObj->GetLineColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorOutlineS );
				ASSERT( pMainFrame->m_pBtnColorOutlineP != NULL );
				ASSERT_VALID( pMainFrame->m_pBtnColorOutlineP );
				pMainFrame->m_pBtnColorOutlineS->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutlineS->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutlineS->RedrawButton();
				pMainFrame->m_pBtnColorOutlineP->RedrawButton();
			}

			CExtBarButton * pTBB =
				pMainFrame->m_wndToolBarStandard.GetButton(
					pMainFrame->m_wndToolBarStandard.CommandToIndex(
						ID_DOC_LINE_WIDTH
						)
					);
			ASSERT_VALID( pTBB );
			UINT nCmdID = pTBB->GetCmdID( true );
			ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );

			UINT nAdjustCmdID = ID_LINE_WIDTH_0;
			if( !pObj->IsNoOutline() )
			{
				UINT xw = (UINT)pObj->m_logpen.lopnWidth.x;
				if( xw <= 10 )
				{
					ASSERT( xw > 0 );
					nAdjustCmdID = ID_LINE_WIDTH_0 + xw;
					ASSERT( ID_LINE_WIDTH_1 <= nAdjustCmdID && nAdjustCmdID <= ID_LINE_WIDTH_10 );
				}
				else
					nAdjustCmdID = ID_LINE_WIDTH_OTHER;
			}

			if( nCmdID != nAdjustCmdID )
				pTBB->SetCmdID( nAdjustCmdID, true, true );
		} // if( pObj != NULL )

		return;
	} // else if( pFrame->IsKindOf( RUNTIME_CLASS(CDrawInPlaceFrame) ) )


/*
	#if (defined __EXT_MFC_NO_CUSTOMIZE)
		if( pObj != NULL )
		{
			if(		pMainFrame->m_pBtnColorFillS != NULL
				&&	pMainFrame->m_pBtnColorFillS->ColorGet(true)
						!= pObj->GetFillColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorFillS );
				ASSERT( pMainFrame->m_pBtnColorFillP != NULL );
				ASSERT_VALID( pMainFrame->m_pBtnColorFillS );
				pMainFrame->m_pBtnColorFillS->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFillP->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFillS->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFillP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFillS->RedrawButton();
				pMainFrame->m_pBtnColorFillP->RedrawButton();
			}
			if(		pMainFrame->m_pBtnColorOutlineS != NULL
				&&	pMainFrame->m_pBtnColorOutlineS->ColorGet(false)
						!= pObj->GetLineColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorOutlineS );
				ASSERT( pMainFrame->m_pBtnColorOutlineP != NULL );
				ASSERT_VALID( pMainFrame->m_pBtnColorOutlineP );
				pMainFrame->m_pBtnColorOutlineS->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutlineS->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutlineS->RedrawButton();
				pMainFrame->m_pBtnColorOutlineP->RedrawButton();
			}

			CExtBarButton * pTBB =
				pMainFrame->m_wndToolBarStandard.GetButton(
					pMainFrame->m_wndToolBarStandard.CommandToIndex(
						ID_DOC_LINE_WIDTH
						)
					);
			ASSERT_VALID( pTBB );
			UINT nCmdID = pTBB->GetCmdID( true );
			ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );

			UINT nAdjustCmdID = ID_LINE_WIDTH_0;
			if( !pObj->IsNoOutline() )
			{
				UINT xw = (UINT)pObj->m_logpen.lopnWidth.x;
				if( xw <= 10 )
				{
					ASSERT( xw > 0 );
					nAdjustCmdID = ID_LINE_WIDTH_0 + xw;
					ASSERT( ID_LINE_WIDTH_1 <= nAdjustCmdID && nAdjustCmdID <= ID_LINE_WIDTH_10 );
				}
				else
					nAdjustCmdID = ID_LINE_WIDTH_OTHER;
			}

			if( nCmdID != nAdjustCmdID )
				pTBB->SetCmdID( nAdjustCmdID, true, true );
		} // if( pObj != NULL )

	#else // (defined __EXT_MFC_NO_CUSTOMIZE)

		if( pObj != NULL )
		{
			if(		pMainFrame->m_pBtnColorFillP != NULL
				&&	pMainFrame->m_pBtnColorFillP->ColorGet(true)
						!= pObj->GetFillColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorFillP );
				pMainFrame->m_pBtnColorFillP->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFillP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFillP->RedrawButton();
			}
			if(		pMainFrame->m_pBtnColorOutlineP != NULL
				&&	pMainFrame->m_pBtnColorOutlineP->ColorGet(false)
						!= pObj->GetLineColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorOutlineP );
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutlineP->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutlineP->RedrawButton();
			}
		} // if( pObj != NULL )

		pMainFrame->RedrawCommandItems( ID_OBJECT_FILLCOLOR ); 
		pMainFrame->RedrawCommandItems( ID_OBJECT_LINECOLOR ); 
		pMainFrame->RedrawCommandItems( ID_LINE_WIDTH_OTHER ); 

	#endif // not - (defined __EXT_MFC_NO_CUSTOMIZE)
*/
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
void CDrawView::OnCancelEditCntr()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CDrawView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

