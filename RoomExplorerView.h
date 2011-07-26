
#pragma once

#include "ScrollingThing.h"
#include "ClassBrowser.h"
#include "QueueItems.h"

class CRoomExplorerDoc;

class CRoomView
{
public:
    CRoomView();
    ~CRoomView();

    WORD wView;
    WORD wx;
    WORD wy;
    WORD wLoop;
    WORD wCel;
    CSize size;
    HBITMAP hbmp;
    ResourceBlob blob;
};




class CRoomExplorerWorkItem
{
public:
    CRoomExplorerWorkItem(WORD wScript) { this->wScript = wScript; }
    ~CRoomExplorerWorkItem()
    {
        for (INT_PTR i = 0; i < _views.GetSize(); i++)
        {
            delete _views.GetAt(i);
        }
    }

    WORD wScript;
    ResourceBlob blob;
    CArray<CRoomView*, CRoomView*> _views;
};


class CRoomExplorerWorkResult
{
public:
    CRoomExplorerWorkResult() { wScript= 0; }
    scoped_array<BYTE> pBitmapData;
    WORD wScript;

    static CRoomExplorerWorkResult *CreateFromWorkItem(CRoomExplorerWorkItem *pWorkItem);
};

// fwd decl
class CRoomExplorerNode;


enum ROOMDIRECTION
{
    RD_NONE,
    RD_NORTH,
    RD_EAST,
    RD_SOUTH,
    RD_WEST,
};

//
// Maps x,y coords to a scriptnumber.
//
class CDyn2DArray
{
public:
    CDyn2DArray() { _rcBounds.SetRectEmpty(); _iNumNodes = 0; }
    CRoomExplorerNode *GetAt(CPoint pt);
    CPoint SetAt(CPoint pt, CRoomExplorerNode *pNode, ROOMDIRECTION iDirectionHint);
    const CRect *GetBounds() { return &_rcBounds; }
    void OffsetBy(CPoint pt);
    BOOL ContainsRoom(WORD wRoom, CPoint &ptWhere);
    int GetSize() { return _iNumNodes; }

private:
    CMap<__int32, __int32, CRoomExplorerNode *, CRoomExplorerNode *> _map;
    CRect _rcBounds;
    int _iNumNodes;
};


class CRoomExplorerView : public CScrollingThing<CView>
{
protected: // create from serialization only
    CRoomExplorerView();
    DECLARE_DYNCREATE(CRoomExplorerView)

// Attributes
public:
    CRoomExplorerDoc* GetDocument() const;

// Overrides
    public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
protected:
    virtual BOOL OnEraseBkgnd(CDC *pDC);

// Implementation
public:
    virtual ~CRoomExplorerView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
    virtual void OnInitialUpdate();
    virtual void OnSize(UINT nType, int cx, int cy);
    virtual void OnMouseMove(UINT nFlags, CPoint point);
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual void OnKillFocus(CWnd *pNewWnd);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnLButtonUp(UINT nFlags, CPoint point);

    QueueItems<CRoomExplorerWorkItem, CRoomExplorerWorkResult> *GetQueue() { return _pQueue; }

    void InvalidateGridRect(LPRECT prc);
    int GetZoom() { return _iZoom; }
    CPoint GetOrigin() { return CPoint(_xOrigin, _yOrigin); }

protected:
    LRESULT _OnMouseLeave(WPARAM wParam, LPARAM lParam);
    void _GenerateDoubleBuffer(CDC *pDC);
    void _CleanDoubleBuffer();
    void _OnDraw(CDC *pDC, BOOL fHitTestOnly = FALSE, CPoint point = CPoint(0, 0), WORD *pwRoom = NULL);
    void _RecalcHeight();
    LRESULT _OnRoomBitmapReady(WPARAM wParam, LPARAM lParam);
    void _OnMouseWheel(UINT nFlags, BOOL fForward, CPoint pt, short nNotches);
    CPoint _GetMoveDelta(int *piMoveDirection = NULL);
    void _UpdateCursor();

    // Scrolling
    virtual int _GetViewWidth() { return _cx / _iZoom; }
    virtual int _GetViewHeight() { return _cy / _iZoom; }

    CPoint _MapClientPointToView(CPoint ptScreen);
    CPoint _MapViewPointToClient(CPoint ptPic);

// Generated message map functions
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

public:
    class CRoomExplorerGrid
    {
    public:
        CRoomExplorerGrid();
        void SetRoomExplorer(CRoomExplorerView *pExplorer) { _pExplorer = pExplorer; }
        void AddRoom(SCIClassBrowser &browser, WORD wScript, const sci::ClassDefinition *pClass, const sci::ClassVector &classes);
        void OrganizeRooms();
        void LoadResources();
        BOOL IsComplete() { return _fIsComplete; }
        ~CRoomExplorerGrid();
        const CRect *GetBounds();
        void DrawRooms(CDC *pDC, BOOL fHitTestOnly, CPoint pt, WORD *pwRoom);
        const CMap<WORD, WORD, ResourceBlob*, ResourceBlob*> &GetPics() const { return _pics; }
        const CMap<WORD, WORD, ResourceBlob*, ResourceBlob*> &GetViews() const { return _views; }
        CRoomExplorerNode *GetNode(WORD wScript);
        void SetHoveredRoom(WORD wRoom);

    private:
        void _DrawLineTo(CDC *pDC, CPoint pt, WORD wRoom);
        void _GetRoomRect(CRoomExplorerNode *pNode, CRect &rect);
        const CRoomExplorerNode *_GetRoomAt(int x, int y);
        bool _NodeExistsAndNotConsidered(WORD wNum);
        //CPoint _FindGoodStartingPoint();

        typedef std::map<WORD, CRoomExplorerNode*> numToNode_t;
        numToNode_t _nodes;

        CMap<WORD, WORD, ResourceBlob*, ResourceBlob*> _pics;
        CMap<WORD, WORD, ResourceBlob*, ResourceBlob*> _views;

        BOOL _fIsComplete;
        CRoomExplorerView *_pExplorer;

        WORD _wHoverScript;
        WORD _wSelectedScript;

        CArray<CDyn2DArray *, CDyn2DArray *> _array;
        CRect _rcBounds;
    };

private:
    int _cy;
    int _cx;
    int _iZoom;

    BOOL _fDoubleBuf;
    CBitmap *_pbitmapDoubleBuf;
    CPoint _ptHoverClient;
    BOOL _fMoveTimer;
    BOOL _fInCapture;
    BOOL _fMouseButtonDown;

    CRoomExplorerGrid _grid;
    QueueItems<CRoomExplorerWorkItem, CRoomExplorerWorkResult> *_pQueue;

    int _iMoveDirection;
};

class CRoomExplorerNode
{
public:
    CRoomExplorerNode();
    void OnDraw(CDC *pDC, CRect *prc, QueueItems<CRoomExplorerWorkItem, CRoomExplorerWorkResult> *pQueue);
    void Init(SCIClassBrowser &browser, const CRoomExplorerView::CRoomExplorerGrid *pGrid, WORD wScript, const sci::ClassDefinition *pClass, const sci::ClassVector &classes);
    void _AddView(const CRoomExplorerView::CRoomExplorerGrid *pGrid, SCIClassBrowser &browser, WORD wScript, const sci::ClassDefinition *pClass);
    ~CRoomExplorerNode();

    WORD North;
    WORD East;
    WORD South;
    WORD West;
    WORD Picture;
    WORD ScriptNum;

    BOOL Considered;
    CPoint Position;

    scoped_array<BYTE> pBitmapData;

    HBITMAP BitmapScaled;

private:
    void _ResizeBitmap(CSize size);

    BOOL _fAskedForBitmap;
    CSize _sizeScaled; // of BitmapScaled
    CArray <CRoomView*, CRoomView*> _views;
    const CRoomExplorerView::CRoomExplorerGrid *_pGrid;

    CRoomExplorerWorkItem *_pWorkItem;
};


#ifndef _DEBUG  // debug version in FontPreviewView.cpp
inline CRoomExplorerDoc* CRoomExplorerView::GetDocument() const
   { return reinterpret_cast<CRoomExplorerDoc*>(m_pDocument); }
#endif

