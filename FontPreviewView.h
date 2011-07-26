// FontPreviewView.h : interface of the CFontPreviewView class
//

#pragma once

#include "ScrollingThing.h"
#include "FontDoc.h"
#include "FontResource.h"

class CFontPreviewView : public CScrollingThing<CView>
{
protected: // create from serialization only
    CFontPreviewView();
    DECLARE_DYNCREATE(CFontPreviewView)

// Attributes
public:
    CFontDoc* GetDocument() const;

    void SetFontResource(CFontResource *pfr) { _pfr = pfr; _RecalcHeight(); Invalidate(FALSE); }

// Overrides
    public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
protected:
    virtual BOOL OnEraseBkgnd(CDC *pDC);

// Implementation
public:
    virtual ~CFontPreviewView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
    virtual void OnInitialUpdate();
    virtual void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);

protected:
    const CFontResource *_GetFont();
    void _GenerateDoubleBuffer(CDC *pDC);
    void _CleanDoubleBuffer();
    void _OnDraw(CDC *pDC, BOOL fHitTestOnly = FALSE, CPoint point = CPoint(0, 0), int *pnChar = NULL);
    void _RecalcHeight();
    void _SyncLetters();
    void OnLButtonDown(UINT nFlags, CPoint point);

    // Scrolling
    virtual int _GetViewWidth() { return _cxPreview; }
    virtual int _GetViewHeight() { return _cyFont * _iZoom; }

// Generated message map functions
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

private:

    // For scrolling (font coordinates)
    int _cyFont;
    int _cxFont;
    int _cxPreview;

    int _iZoom;

    BOOL _fDoubleBuf;
    CBitmap *_pbitmapDoubleBuf;

    CFontResource *_pfr;

    CString _strLetters;
};

#ifndef _DEBUG  // debug version in FontPreviewView.cpp
inline CFontDoc* CFontPreviewView::GetDocument() const
   { return reinterpret_cast<CFontDoc*>(m_pDocument); }
#endif


