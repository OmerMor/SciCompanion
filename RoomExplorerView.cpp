// FontPreviewView.cpp : implementation of the CRoomExplorerView class
//

#include "stdafx.h"
#include "RoomExplorerDoc.h"
#include "SCIPicEditor.h"
#include "PicResource.h"
#include "PicDrawManager.h"
#include "ViewResource.h"

using namespace sci;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RoomExplorerView.h"

#define COLOR_BG RGB(144, 144, 80)
#define COLOR_SHADOW RGB(100, 100, 60)
#define PEN_COLOR RGB(40, 40, 40)

#define HORZ_SPACE 420
#define VERT_SPACE 280

#define UWM_ROOMBMPREADY      (WM_APP + 0)

#define ROOM_MARGINS        CSize(2, 2)

#define ARROW_SEP           5
#define ARROW_LENGTH 7
#define ARROW_WIDTH 4

#define MOUSEMOVE_TIMER 4567

CRoomExplorerWorkResult *CRoomExplorerWorkResult::CreateFromWorkItem(CRoomExplorerWorkItem *pWorkItem)
{
    CRoomExplorerWorkResult *pResult = NULL;
    BOOL fOk = FALSE;
    PicResource pic;
    if (pWorkItem->blob.GetType() == RS_PIC && SUCCEEDED(pic.InitFromResource(&pWorkItem->blob)))
    {
        PicDrawManager pdm(&pic);
        scoped_array<BYTE> dataDisplay(new BYTE[BMPSIZE]);
        scoped_array<BYTE> dataAux(new BYTE[BMPSIZE]);
        fOk = TRUE;
        BITMAPINFO *pbmi;
        pdm.CopyBitmap(DRAW_ENABLE_VISUAL, dataDisplay.get(), dataAux.get(), &pbmi);

        for (INT_PTR i = 0; i < pWorkItem->_views.GetSize(); i++)
        {
            CRoomView *pRoomView = pWorkItem->_views.GetAt(i);
            ViewResource view;
            if (SUCCEEDED(view.InitFromResource(&pRoomView->blob)))
            {
                DrawViewWithPriority(dataDisplay.get(), pdm.GetPriorityBits(), PriorityFromY(pRoomView->wy),
                    pRoomView->wx, pRoomView->wy,
                    &view, pRoomView->wLoop, pRoomView->wCel);
            }
        }

        // Now make a smushed down bitmap for all this.
        pResult = new CRoomExplorerWorkResult();
        if (pResult)
        {
            pResult->wScript = pWorkItem->wScript;
            // Result takes ownership:
            pResult->pBitmapData.swap(dataDisplay);
        }
    }
    return pResult;
}

CRoomView::CRoomView()
{
    wx = 0;
    wy = 0;
    wLoop = 0;
    wCel = 0;
    hbmp = NULL;
    wView = 0;
}

CRoomView::~CRoomView()
{
    if (hbmp)
    {
        DeleteObject(hbmp);
    }
}

int g_count = 0;

CRoomExplorerNode::CRoomExplorerNode()
{
    North = 0;
    East = 0;
    South = 0;
    West = 0;
    Picture = 0;

    Considered = FALSE;
    BitmapScaled = NULL;
    _fAskedForBitmap = FALSE;
    _pWorkItem = NULL;
    g_count++;
    _sizeScaled.SetSize(0, 0);
}

CRoomExplorerNode::~CRoomExplorerNode()
{
    g_count--;
    if (BitmapScaled)
    {
        DeleteObject(BitmapScaled);
    }
    delete _pWorkItem;
}

void CRoomExplorerNode::_ResizeBitmap(CSize size)
{
    if (BitmapScaled)
    {
        DeleteObject(BitmapScaled);
        BitmapScaled = NULL;
    }

    if (pBitmapData.get())
    {
        // Temporarily make a gdi+ wrapper for it.
        Gdiplus::Bitmap *pimg = NULL;
        SCIBitmapInfo bmi(sPIC_WIDTH, sPIC_HEIGHT);
        pimg = Gdiplus::Bitmap::FromBITMAPINFO(&bmi, pBitmapData.get());
        if (pimg)
        {
            if ((size.cx == sPIC_WIDTH) && (size.cy == sPIC_HEIGHT))
            {
                // Exact size.
                pimg->GetHBITMAP(Color::Black, &BitmapScaled);
            }
            else
            {
                // Make a different sized version
                // Note: this cast to (Bitmap*) is undocumented.
                Gdiplus::Bitmap *pimage = (Gdiplus::Bitmap*)pimg->GetThumbnailImage(size.cx, size.cy, NULL, NULL);
                if (pimage)
                {
                    pimage->GetHBITMAP(Color::Black, &BitmapScaled);
                    delete pimage;
                }
            }
            delete pimg;
        }
    }
    _sizeScaled = size;
}

void CRoomExplorerNode::OnDraw(CDC *pDC, CRect *prc, QueueItems<CRoomExplorerWorkItem, CRoomExplorerWorkResult> *pQueue)
{
    CRect rcClip;
    pDC->GetClipBox(&rcClip);
    CRect rcInner;
    if (rcInner.IntersectRect(&rcClip, prc))
    {
        CDC dcMem;
        if (dcMem.CreateCompatibleDC(pDC))
        {
            if (pBitmapData.get())
            {
                CSize sizeToPaint(prc->Width(), prc->Height());
                if (sizeToPaint != _sizeScaled)
                {
                    // Make a scaled version of the bitmap
                    _ResizeBitmap(sizeToPaint);
                }
                if (BitmapScaled)
                {
                    HGDIOBJ hOld = dcMem.SelectObject(BitmapScaled);
                    pDC->BitBlt(prc->left, prc->top, prc->Width(), prc->Height(), &dcMem, 0, 0, SRCCOPY);
                    dcMem.SelectObject(hOld);
                }
            }
            else
            {
                if (!_fAskedForBitmap)
                {
                    _fAskedForBitmap = TRUE;

                    if (_pWorkItem)
                    {
                        pQueue->GiveWorkItem(_pWorkItem);
                        _pWorkItem = NULL; // Ownership transfered to queue
                    }
                }
                CBrush brush(RGB(128, 128, 128));
                dcMem.FillRect(prc, &brush);
            }
            // Draw number on top.
            TCHAR szNumber[10];
            StringCchPrintf(szNumber, ARRAYSIZE(szNumber), TEXT("%d"), this->ScriptNum);
            COLORREF crOld = pDC->SetTextColor(RGB(0, 0, 0));
            CRect rcShadow = prc;
            rcShadow.OffsetRect(1, 1);
            pDC->DrawText(szNumber, -1, &rcShadow, 0);
            pDC->SetTextColor(RGB(255, 255, 255));
            pDC->DrawText(szNumber, -1, prc, 0);
            pDC->SetTextColor(crOld);
        }
    }
}

//
// Suck out the north, south, east, west rooms, and the pic number.
//
void CRoomExplorerNode::Init(SCIClassBrowser &browser, const CRoomExplorerView::CRoomExplorerGrid *pGrid, WORD wScript, const ClassDefinition *pClass, const ClassVector &classes)
{
    _pGrid = pGrid;

    ASSERT(_pWorkItem == NULL);
    _pWorkItem = new CRoomExplorerWorkItem(wScript);
    if (_pWorkItem)
    {
        // REVIEW: use SCIClassBrowser::ResolveValue for these values
        ScriptNum = wScript;
        const ClassPropertyVector &properties = pClass->GetProperties();
        // STLCLEANUP
        for (size_t i = 0; i < properties.size(); i++)
        {
            std::string strName = properties[i]->GetName();
            if (strName == "north")
            {
                North = properties[i]->GetValue().GetNumberValue();
            }
            else if (strName == "east")
            {
                East = properties[i]->GetValue().GetNumberValue();
            }
            else if (strName == "south")
            {
                South = properties[i]->GetValue().GetNumberValue();
            }
            else if (strName == "west")
            {
                West = properties[i]->GetValue().GetNumberValue();
            }
            else if (strName == "picture")
            {
                if (properties[i]->GetValue().GetStringValue() == "scriptNumber")
                {
                    Picture = wScript;
                }
                else
                {
                    Picture = properties[i]->GetValue().GetNumberValue();
                }
                // Get the ResourceBlob for it.
                ResourceBlob *pData;
                if (_pGrid->GetPics().Lookup(Picture, pData))
                {
                    _pWorkItem->blob = *pData;
                }
            }
        }

        // Look for any subclasses of View
        for (size_t i = 0; i < classes.size(); i++)
        {
            const ClassDefinition *pClass = classes[i];
            if (pClass->IsInstance())
            {
                if (browser.IsSubClassOf(pClass->GetSuperClass().c_str(), "View"))
                {
                    _AddView(pGrid, browser, wScript, pClass);
                }
            }
        }
    }
}

void CRoomExplorerNode::_AddView(const CRoomExplorerView::CRoomExplorerGrid *pGrid, SCIClassBrowser &browser, WORD wScript, const ClassDefinition *pClass)
{
    CRoomView *pView = new CRoomView();
    if (pView)
    {
        const ClassPropertyVector &properties = pClass->GetProperties();
        for (size_t i = 0; i < properties.size(); i++)
        {
            const ClassProperty *pProp = properties[i];
            std::string strPropName = pProp->GetName();
            PropertyValue Value;
            if (strPropName == "view")
            {
                browser.ResolveValue(wScript, pProp->GetValue(), Value);
                pView->wView = Value.GetNumberValue();
            }
            else if (strPropName == "x")
            {
                browser.ResolveValue(wScript, pProp->GetValue(), Value);
                pView->wx = Value.GetNumberValue();
            }
            else if (strPropName == "y")
            {
                browser.ResolveValue(wScript, pProp->GetValue(), Value);
                pView->wy = Value.GetNumberValue();
            }
            else if (strPropName == "loop")
            {
                browser.ResolveValue(wScript, pProp->GetValue(), Value);
                pView->wLoop = Value.GetNumberValue();
            }
            else if (strPropName == "cel")
            {
                browser.ResolveValue(wScript, pProp->GetValue(), Value);
                pView->wCel = Value.GetNumberValue();
            }
        }
        
        // Get the ResourceBlob for it.
        ResourceBlob *pData;
        if (_pGrid->GetViews().Lookup(pView->wView, pData))
        {
            pView->blob = *pData;
        }

        // Add it to the views list of the work item.
        ASSERT(_pWorkItem);
        _pWorkItem->_views.Add(pView);
    }
}




CRoomExplorerView::CRoomExplorerGrid::CRoomExplorerGrid()
{
    _fIsComplete = FALSE;
    _pExplorer = NULL;
    _wHoverScript = 0;
    _wSelectedScript = 0;
}

void CRoomExplorerView::CRoomExplorerGrid::AddRoom(SCIClassBrowser &browser, WORD wScript, const ClassDefinition *pClass, const ClassVector &classes)
{
    // It's a room class!
    CRoomExplorerNode *pNode = new CRoomExplorerNode();
    if (pNode)
    {
        pNode->Init(browser, this, wScript, pClass, classes);
        _nodes[wScript] = pNode;
    }
}

void CRoomExplorerView::CRoomExplorerGrid::LoadResources()
{
    // Get the view and pic resources so we can draw them.
    ResourceEnumerator *pEnum;
    if (SUCCEEDED(theApp.GetResourceMap().CreateEnumerator(NULL, ResourceTypeFlagPic | ResourceTypeFlagView, &pEnum)))
    {
        ResourceBlob *pData;
        while (S_OK == pEnum->Next(&pData))
        {
            ResourceBlob *pDataTest;
            if (pData->GetType() == RS_PIC)
            {
                if (!_pics.Lookup(pData->GetNumber(), pDataTest))
                {
                    _pics.SetAt(pData->GetNumber(), pData);
                }
                else
                {
                    // It was an older version of this resource.
                    delete pData;
                }
            }
            else
            {
                if (!_views.Lookup(pData->GetNumber(), pDataTest))
                {
                    _views.SetAt(pData->GetNumber(), pData);
                }
                else
                {
                    // It was an older version of this resource.
                    delete pData;
                }
            }
        }
        delete pEnum;
    }
}



void _ShiftPoint(CPoint &pt, ROOMDIRECTION iDirectionHint)
{
    BOOL fRet = FALSE;
    switch (iDirectionHint)
    {
    case RD_NORTH:
        pt.y--; // Move it down
        break;
    case RD_WEST:
        pt.x--; // Move it east
        break;
    case RD_EAST:
        pt.x++; // move it west
        break;
    case RD_SOUTH:
    case RD_NONE:
        pt.y++; // move it north
        break;
    }
}


BOOL _AdjustPoint(CPoint ptOrig, ROOMDIRECTION iDirectionHint, CPoint &pt)
{
    BOOL fRet = FALSE;
    switch (iDirectionHint)
    {
    case RD_NORTH:
        if (pt.y > ptOrig.y)
        {
            pt.y++; // Move it down
            fRet = TRUE;
        }
        break;
    case RD_WEST:
        if (pt.x > ptOrig.x)
        {
            pt.x++; // Move it east
            fRet = TRUE;
        }
        break;
    case RD_EAST:
        if (pt.x < ptOrig.x)
        {
            pt.x--; // move it west
            fRet = TRUE;
        }
        break;
    case RD_SOUTH:
    case RD_NONE:
        if (pt.y < ptOrig.y)
        {
            pt.y--; // move it north
            fRet = TRUE;
        }
        break;
    }
    return fRet;
}

// picture number 5 is not getting offset!!!! how come?
void CDyn2DArray::OffsetBy(CPoint pt)
{
TCHAR sz[100];

    POSITION pos = _map.GetStartPosition();
    while (pos)
    {
        CRoomExplorerNode *pNodeCur;
        int index;
        _map.GetNextAssoc(pos, index, pNodeCur);
StringCchPrintf(sz, ARRAYSIZE(sz), TEXT("Offsetting %d\n"), pNodeCur->ScriptNum);
OutputDebugString(sz);

        pNodeCur->Position.Offset(pt);
        // We don't reset it in the array, but that should be ok.
    }    
    _rcBounds.OffsetRect(pt);
}

//
// Tries to put a thing at pt.
// Returns where it actually put it.
//
CPoint CDyn2DArray::SetAt(CPoint pt, CRoomExplorerNode *pNode, ROOMDIRECTION iDirectionHint)
{
TCHAR sz[100];
if (pNode->ScriptNum == 5 || pNode->ScriptNum == 6)
{
StringCchPrintf(sz, ARRAYSIZE(sz), TEXT("%d set to %d, %d\n"), pNode->ScriptNum, pt, pt);
OutputDebugString(sz);
}

    CRoomExplorerNode *pNodeAlreadyThere = GetAt(pt);
    while (pNodeAlreadyThere)
    {
        // Move it a little until we find a free spot

        _ShiftPoint(pt, iDirectionHint);
        pNodeAlreadyThere = GetAt(pt);
    }
    __int32 index = (0xffff & pt.x) + (pt.y << 16);
    _map.SetAt(index, pNode);
    pNode->Position = pt;
    _rcBounds.left = min(_rcBounds.left, pt.x);
    _rcBounds.right = max(_rcBounds.right, pt.x + 1); // Add one, since the bounds needs to include everything.
    _rcBounds.top = min(_rcBounds.top, pt.y);
    _rcBounds.bottom= max(_rcBounds.bottom, pt.y + 1);
    _iNumNodes++;

    CRoomExplorerNode *pNodeThere = GetAt(CPoint(-4, 4));
    if (pNodeThere)
    {
        StringCchPrintf(sz, ARRAYSIZE(sz), TEXT("Node %d is at -4 , 4\n"), pNodeThere->ScriptNum);
        OutputDebugString(sz);
    }
    else
    {
        OutputDebugString(TEXT("nothing is at -4 , 4\n"));
    }

    return pt;
}

CRoomExplorerNode *CDyn2DArray::GetAt(CPoint pt)
{
    CRoomExplorerNode *pNode = NULL;
    __int32 index = (0xffff & pt.x) + (pt.y << 16);
    _map.Lookup(index, pNode);
    return pNode;
}

//
// Do we contain this room.
//
BOOL CDyn2DArray::ContainsRoom(WORD wRoom, CPoint &ptWhere)
{
    BOOL fRet = FALSE;
    POSITION pos = _map.GetStartPosition();
    while (pos && !fRet)
    {
        CRoomExplorerNode *pNodeCur;
        int index;
        _map.GetNextAssoc(pos, index, pNodeCur);
        fRet = (pNodeCur->ScriptNum == wRoom);
        if (fRet)
        {
            ptWhere = CPoint((index << 16) >> 16, index >> 16);
        }
    }    
    return fRet;
}


typedef struct
{
    WORD wNum;
    CPoint pt;
    ROOMDIRECTION iDirection; // direction the connection was made
} ROOMPOS;

//
// Gets the last item in the array and removes it, puts it in *pPos
// Returns FALSE if the array is empty.
//
BOOL _GetLastAndPop(CArray<ROOMPOS, ROOMPOS> &poses, ROOMPOS *pPos)
{
    BOOL fRet = FALSE;
    if (poses.GetSize() > 0)
    {
        fRet = TRUE;
        *pPos = poses.GetAt(poses.GetUpperBound());
        poses.RemoveAt(poses.GetUpperBound());
    }
    return fRet;
}


CDyn2DArray *_CheckForRoomAlreadyConsidered(CRoomExplorerNode *pNode, CArray<CDyn2DArray *, CDyn2DArray *> &array, CPoint &ptStartFrom, ROOMDIRECTION *prd)
{
    *prd = RD_NONE;
    CDyn2DArray *pArray = NULL;
    for (INT_PTR i = 0; (pArray == NULL) && (i < array.GetSize()); i++)
    {
        CDyn2DArray *pArrayThisOne = array.GetAt(i);
        if (pArrayThisOne->ContainsRoom(pNode->North, ptStartFrom))
        {
            pArray = pArrayThisOne;
            ptStartFrom.Offset(CPoint(0, 1));
            *prd = RD_SOUTH;
        }
        else if (pArrayThisOne->ContainsRoom(pNode->South, ptStartFrom))
        {
            pArray = pArrayThisOne;
            ptStartFrom.Offset(CPoint(0, -1));
            *prd = RD_NORTH;
        }
        else if (pArrayThisOne->ContainsRoom(pNode->West, ptStartFrom))
        {
            pArray = pArrayThisOne;
            ptStartFrom.Offset(CPoint(1, 0));
            *prd = RD_EAST;
        }
        else if (pArrayThisOne->ContainsRoom(pNode->East, ptStartFrom))
        {
            pArray = pArrayThisOne;
            ptStartFrom.Offset(CPoint(-1, 0));
            *prd = RD_WEST;
        }
    }
    return pArray;
}

bool CRoomExplorerView::CRoomExplorerGrid::_NodeExistsAndNotConsidered(WORD wNum)
{
    numToNode_t::iterator nodeIt = _nodes.find(wNum);
    return (nodeIt != _nodes.end() && !nodeIt->second->Considered);
}

void CRoomExplorerView::CRoomExplorerGrid::OrganizeRooms()
{
    _fIsComplete = TRUE;
    CArray<ROOMPOS, ROOMPOS> norths;
    CArray<ROOMPOS, ROOMPOS> souths;
    CArray<ROOMPOS, ROOMPOS> easts;
    CArray<ROOMPOS, ROOMPOS> wests;
    // Algorithm:
    //      - for each room: have we already done it?  If so, skip
    //      - for the room, find an empty spot in the grid.
    //      - put it there.  Follow rooms in each direction
    //          - for these rooms, their spots are implied by their direction.
    //              - if those rooms already exist in the map, then leave them alone.
    //          - if not, and that spot is full, then "move back" from the direction you traveled.  Move all
    //                  "appropriate" rooms back too.
    //          - if
    //
    BOOL fStillSomeLeft = TRUE;
    while (fStillSomeLeft)
    {
        fStillSomeLeft = FALSE;
        BOOL fAddedAtLeastOne = FALSE;
        numToNode_t::iterator nodeIt = _nodes.begin();
        while (nodeIt != _nodes.end())
        {
            CDyn2DArray *pArray = NULL;
            ROOMDIRECTION iDirectionHint = RD_NONE;
            //WORD wScript;
            CRoomExplorerNode *pNode = nodeIt->second;
            //_nodes.GetNextAssoc(pos, wScript, pNode);
            if (!pNode->Considered)
            {
                // Does this node have a connection to an already considered room?  If so, use that room's
                // CDyn2DArray, and not a new one.
                BOOL fReuse = FALSE;
                CPoint pt;
                pArray = _CheckForRoomAlreadyConsidered(pNode, _array, pt, &iDirectionHint);
                if (pArray)
                {
                    // TODO: if we're re-using, we need a smart place to start!  A position needs to be suggested.
                    // But reverse it... since iDirectionHint is how we got to this particualr room.  We don't know that
                    // but we know how to get to the already considered room from this one...
                    fReuse = TRUE;
                }
                else
                {
                    // Start from 0,0 with a new array - assuming we have yet added any rooms in this pass
                    if (!fAddedAtLeastOne)
                    {
                        fAddedAtLeastOne = TRUE;
                        pt = CPoint(0, 0);
                        pArray = new CDyn2DArray();
                    }
                    else
                    {
                        // otherwise leave pArray NULL, and skip this guy this time.  The reason is, suppose we have rooms
                        // 45 and 46 that are connected.  46 has a passage to room 8.  Room 8 has already been done. The others
                        // haven't. We encounter 45 here, and it has no connections to existing rooms, so we start anew...
                        // That's no good.  We want to start with 46 instead!  So skip rooms that have no connections - except
                        // we add at least one.
                        fStillSomeLeft = TRUE;
                    }
                }
                if (pArray)
                {
                    // Look at this first room.  Consider it at 0, 0
                    // We'll now add all rooms that are "attached" by direction to this room.
                    // This might not include *all* rooms though, so that's why there is a while
                    // loop above us.
                    //CPoint pt = _FindGoodStartingPoint();
                    while (pNode)
                    {
                        if (!pNode->Considered)
                        {
                            pNode->Considered = TRUE;
                            pt = pArray->SetAt(pt, pNode, iDirectionHint);

                            ROOMPOS roompos;
                            if (_NodeExistsAndNotConsidered(pNode->North))
                            {
                                roompos.pt = CPoint(pt.x, pt.y - 1);
                                roompos.wNum = pNode->North;
                                roompos.iDirection = RD_NORTH;
                                norths.Add(roompos);
                            }
                            if (_NodeExistsAndNotConsidered(pNode->East))
                            {
                                roompos.pt =  CPoint(pt.x + 1, pt.y);
                                roompos.wNum = pNode->East;
                                roompos.iDirection = RD_EAST;
                                easts.Add(roompos);
                            }
                            if (_NodeExistsAndNotConsidered(pNode->South))
                            {
                                roompos.pt = CPoint(pt.x, pt.y + 1);
                                roompos.wNum = pNode->South;
                                roompos.iDirection = RD_SOUTH;
                                souths.Add(roompos);
                            }
                            if (_NodeExistsAndNotConsidered(pNode->West))
                            {
                                roompos.pt = CPoint(pt.x - 1, pt.y);
                                roompos.wNum = pNode->West;
                                roompos.iDirection = RD_WEST;
                                wests.Add(roompos);
                            }
                        }

                        // Pop one off stack.
                        ROOMPOS pos;
                        BOOL fNoMore = FALSE;
                        if (!_GetLastAndPop(norths, &pos))
                        {
                            if (!_GetLastAndPop(easts, &pos))
                            {
                                if (!_GetLastAndPop(souths, &pos))
                                {
                                    if (!_GetLastAndPop(wests, &pos))
                                    {
                                        fNoMore = TRUE;
                                    }
                                }
                            }
                        }

                        if (fNoMore)
                        {
                            pNode = NULL;
                        }
                        else
                        {
                            pNode = _nodes[pos.wNum];
                            pt = pos.pt;
                            iDirectionHint = pos.iDirection;
                        }
                    }

                    // Assuming we made a new pArray, add it.
                    if (!fReuse)
                    {
                        // Add it sorted based on the number of nodes.
                        BOOL fAdded = FALSE;
                        for (INT_PTR i = 0; !fAdded && i < _array.GetSize(); i++)
                        {
                            if (pArray->GetSize() < _array.GetAt(i)->GetSize())
                            {
                                fAdded = TRUE;
                                _array.InsertAt(i, pArray);
                            }
                        }
                        if (!fAdded)
                        {
                            _array.Add(pArray);
                        }
                    }

                }
            }
            ++nodeIt;
            // Done!
        }
    }

    // Now we need to organize the "blocks" we made into something.
    _rcBounds.SetRectEmpty();
    for (INT_PTR i = 0; i < _array.GetSize(); i++)
    {
        CDyn2DArray *pArray = _array.GetAt(i);
        const CRect *prc = pArray->GetBounds();

        // Somehow fit this prc into _rcBounds.
        int xOffset = 0;
        int yOffset = 0;
        if (_rcBounds.Width() > _rcBounds.Height())
        {
            // Add it to the bottom.
            yOffset = _rcBounds.bottom - prc->top;
            // And make sure we don't waste horizontal space - line things up on the left
            xOffset = _rcBounds.left - prc->left;
        }
        else
        {
            // Add it to the right.
            xOffset = _rcBounds.right - prc->left;
            // And make sure we don't waste vertical space - line things up on the top
            yOffset = _rcBounds.top - prc->top;
        }

        // Offset all these ones.
        CPoint ptOffset(xOffset, yOffset);
        pArray->OffsetBy(ptOffset);

        // And grow our boudns rect.
        CRect rcThisOne = *prc;
        //rcThisOne.OffsetRect(ptOffset);
        CRect rcUnion;

        rcUnion.UnionRect(&_rcBounds, &rcThisOne);
        _rcBounds = rcUnion;
    }
}

void CRoomExplorerView::CRoomExplorerGrid::_GetRoomRect(CRoomExplorerNode *pNode, CRect &rect)
{
    CPoint ptDraw = pNode->Position - GetBounds()->TopLeft();
    int iZoom = _pExplorer->GetZoom();
    ptDraw.x *= (HORZ_SPACE / iZoom);
    ptDraw.y *= (VERT_SPACE / iZoom);

    CPoint ptBR = ptDraw + CPoint(sPIC_WIDTH / iZoom, sPIC_HEIGHT / iZoom);
    rect.left = ptDraw.x;
    rect.right = ptDraw.x + sPIC_WIDTH / iZoom;
    rect.top = ptDraw.y;
    rect.bottom = ptDraw.y + sPIC_HEIGHT / iZoom;
    rect.InflateRect(ROOM_MARGINS);

    CPoint pt = _pExplorer->GetOrigin();
    rect.OffsetRect(-pt.x, -pt.y);
}

#define SWAP(x, y) { int temp = x; x = y; y = temp; }

CPoint GetBresen(CPoint ptFrom, CPoint ptTo, int howMuch)
{
    int x0 = ptTo.x;
    int y0 = ptTo.y;
    int x1 = ptFrom.x;
    int y1 = ptFrom.y;
    BOOL fSteep = abs(y1 - y0) > abs(x1 - x0);
    if (fSteep)
    {
        SWAP(x0, y0);
        SWAP(x1, y1);
    }
    if (x0 > x1)
    {
        SWAP(x0, x1);
        SWAP(y0, y1);
        howMuch = (x1 - x0) - howMuch;
    }
    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = 0;
    int y = y0;
    int ystep = (y0 < y1) ? 1 : -1;
    CPoint ptReturn = ptTo;
    for (int x = x0; x < x1; x++)
    {
        if (howMuch)
        {
            error = error + deltay;
            if ((2 * error) >= deltax)
            {
                y = y + ystep;
                error = error - deltax;
            }
        }
        else
        {
            ptReturn = fSteep ? CPoint(y, x) : CPoint(x, y);
        }
        howMuch--;
    }
    return ptReturn;
}

void _DrawArrowLine(CDC *pDC, CPoint ptFrom, CPoint ptTo)
{
    // Draw the line first.
    pDC->MoveTo(ptFrom);
    pDC->LineTo(ptTo);

    // Calc the base of the arrow
    CPoint ptArrowBase = GetBresen(ptFrom, ptTo, ARROW_LENGTH);
    CPoint ptArrowTipsDelta = GetBresen(ptFrom, ptTo, ARROW_WIDTH) - ptArrowBase;

    // Now rotate right 90 degrees to find the right potin
    CPoint ptRight = ptArrowBase + CPoint(ptArrowTipsDelta.y, -ptArrowTipsDelta.x);
    // And left 90 degrees
    CPoint ptLeft = ptArrowBase + CPoint(-ptArrowTipsDelta.y, ptArrowTipsDelta.x);

    // Now draw shape with these 3 points.
    CPoint rgpt[] = { ptRight, ptLeft, ptTo };
    pDC->Polygon(rgpt, ARRAYSIZE(rgpt));
}

void CRoomExplorerView::CRoomExplorerGrid::_DrawLineTo(CDC *pDC, CPoint ptFrom, WORD wRoom)
{
    if (wRoom)
    {
        numToNode_t::iterator nodeIt = _nodes.find(wRoom);
        if (nodeIt != _nodes.end())
        {
            CRoomExplorerNode *pToNode;
            pToNode = nodeIt->second;
            // Figure out where it is in relation to us.
            CRect rectTo;
            _GetRoomRect(pToNode, rectTo);
            rectTo.DeflateRect(ROOM_MARGINS);
            CPoint ptTo = rectTo.CenterPoint();
            if (ptFrom.x < rectTo.left)
            {
                ptTo.x = rectTo.left;
                ptTo.y += ARROW_SEP;
            }
            else if (ptFrom.x > rectTo.right)
            {
                ptTo.x = rectTo.right;
                ptTo.y -= ARROW_SEP;
            }
            else if (ptFrom.y < rectTo.top)
            {
                ptTo.y = rectTo.top;
                ptTo.x += ARROW_SEP;
            }
            else if (ptFrom.y > rectTo.bottom)
            {
                ptTo.y = rectTo.bottom;
                ptTo.x -= ARROW_SEP;
            }
            _DrawArrowLine(pDC, ptFrom, ptTo);
        }
    }
}

const CRect *CRoomExplorerView::CRoomExplorerGrid::GetBounds()
{
    // Either calculate and store the bounds, or you know...
    return &_rcBounds;
}

void CRoomExplorerView::CRoomExplorerGrid::DrawRooms(CDC *pDC, BOOL fHitTestOnly, CPoint pt, WORD *pwRoom)
{
    CPen pen(PS_SOLID, 2, PEN_COLOR);
    HGDIOBJ penOld;
    if (pDC)
    {
        penOld = pDC->SelectObject(&pen);
    }
    if (pwRoom)
    {
        *pwRoom = 0;
    }

    numToNode_t::iterator nodeIt = _nodes.begin();
    while (nodeIt != _nodes.end())
    {
        CRoomExplorerNode *pNode = nodeIt->second;
        WORD wScript = nodeIt->first;
        CRect rect;
        _GetRoomRect(pNode, rect);
        if (fHitTestOnly)
        {
            if (rect.PtInRect(pt))
            {
                *pwRoom = pNode->ScriptNum;
                break;
            }
        }
        else
        {
            // Draw a little shadow
            CSize sizeMargins = ROOM_MARGINS;
            CRect rcShadow(rect.left + sizeMargins.cx * 2, rect.bottom - sizeMargins.cy, rect.right, rect.bottom);
            pDC->FillSolidRect(&rcShadow, COLOR_SHADOW);
            rcShadow.SetRect(rect.right - sizeMargins.cx, rect.top + sizeMargins.cy * 2, rect.right, rect.bottom);
            pDC->FillSolidRect(&rcShadow, COLOR_SHADOW);

            if (_wHoverScript == wScript)
            {
                // Draw a hover rectangle.
                pDC->Rectangle(&rect);
            }
            rect.DeflateRect(ROOM_MARGINS);
            pNode->OnDraw(pDC, &rect, _pExplorer->GetQueue());

            // Draw some lines.
            _DrawLineTo(pDC, CPoint((rect.left + rect.right) / 2 - ARROW_SEP, rect.top), pNode->North);
            _DrawLineTo(pDC, CPoint(rect.right, (rect.top + rect.bottom) / 2 + ARROW_SEP), pNode->East);
            _DrawLineTo(pDC, CPoint((rect.left + rect.right) / 2 + ARROW_SEP, rect.bottom), pNode->South);
            _DrawLineTo(pDC, CPoint(rect.left, (rect.top + rect.bottom) / 2 - ARROW_SEP), pNode->West);
        }
        ++nodeIt;
    }    
    if (pDC)
    {
        pDC->SelectObject(penOld);
    }
}

CRoomExplorerNode *CRoomExplorerView::CRoomExplorerGrid::GetNode(WORD wScript)
{
    numToNode_t::iterator nodeIt = _nodes.find(wScript);
    return (nodeIt != _nodes.end()) ? nodeIt->second : NULL;
}

void CRoomExplorerView::CRoomExplorerGrid::SetHoveredRoom(WORD wRoom)
{
    if (wRoom != _wHoverScript)
    {
        numToNode_t::iterator nodeIt = _nodes.find(_wHoverScript);
        if (nodeIt != _nodes.end())
        {
            CRect rect;
            _GetRoomRect(nodeIt->second, rect);
            _pExplorer->InvalidateGridRect(&rect);
        }
        _wHoverScript = wRoom;
        nodeIt = _nodes.find(wRoom);
        if (nodeIt != _nodes.end())
        {
            CRect rect;
            _GetRoomRect(nodeIt->second, rect);
            _pExplorer->InvalidateGridRect(&rect);
        }
    }
}

void CRoomExplorerView::_OnMouseWheel(UINT nFlags, BOOL fForward, CPoint pt, short nNotches)
{
    int iZoomOld = _iZoom;
    // Shift held down.  Do a zoom:
    if (fForward)
    {
        _iZoom++;
    }
    else
    {
        _iZoom--;
        _iZoom = max(_iZoom, 1);
    }
    
    // Zoom around ptHover.
    int cxHalf = _ptHoverClient.x;
    int cyHalf = _ptHoverClient.y;
    _xOrigin = ((_xOrigin + cxHalf) * iZoomOld / _iZoom) - cxHalf;
    _yOrigin = ((_yOrigin + cyHalf) * iZoomOld / _iZoom) - cyHalf;
    _InvalidateScrollStuff();
    Invalidate(FALSE);
}


CRoomExplorerView::CRoomExplorerGrid::~CRoomExplorerGrid()
{
    for_each(_nodes.begin(), _nodes.end(), delete_map_value());

    POSITION pos = _pics.GetStartPosition();
    while (pos)
    {
        WORD wScript;
        ResourceBlob *pNode;
        _pics.GetNextAssoc(pos, wScript, pNode);
        delete pNode;
    }

    pos = _views.GetStartPosition();
    while (pos)
    {
        WORD wScript;
        ResourceBlob *pNode;
        _views.GetNextAssoc(pos, wScript, pNode);
        delete pNode;
    }

    for (INT_PTR i = 0; i < _array.GetSize(); i++)
    {
        delete _array.GetAt(i);
    }
    _array.RemoveAll();
}



// CRoomExplorerView

IMPLEMENT_DYNCREATE(CRoomExplorerView, CView)


BEGIN_MESSAGE_MAP(CRoomExplorerView, CScrollingThing<CView>)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_MESSAGE(UWM_ROOMBMPREADY, _OnRoomBitmapReady)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDBLCLK()
    ON_MESSAGE(WM_MOUSELEAVE, _OnMouseLeave)
    ON_WM_TIMER()
    ON_WM_KILLFOCUS()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

#define MOVE_NONE 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_RIGHT 3
#define MOVE_LEFT 4

CRoomExplorerView::CRoomExplorerView()
{
    // Scrolling:
    _yOrigin = 0;
    _xOrigin = 0;

    _cx = 1000;
    _cy = 1000;
    _iZoom = 3;

    _fDoubleBuf = FALSE;
    _pbitmapDoubleBuf = NULL;
    _pQueue = NULL;
    _fMoveTimer = FALSE;
    _fInCapture = FALSE;
    _grid.SetRoomExplorer(this);

    _iMoveDirection = MOVE_NONE;
    _fMouseButtonDown = FALSE;
}

CRoomExplorerView::~CRoomExplorerView()
{
    if (_fInCapture)
    {
        ReleaseCapture();
        _fInCapture = FALSE;
    }

    _CleanDoubleBuffer();
    if (_pQueue)
    {
        _pQueue->Abort();
        _pQueue->Release();
    }
}

void CRoomExplorerView::_CleanDoubleBuffer()
{
    // Reset our double buffering, so we regenerate it on the next paint cycle.
    _fDoubleBuf = FALSE;
    delete _pbitmapDoubleBuf;
    _pbitmapDoubleBuf = FALSE;
}


void CRoomExplorerView::_GenerateDoubleBuffer(CDC *pDC)
{
    ASSERT(!_pbitmapDoubleBuf);
    _pbitmapDoubleBuf = new CBitmap();
    if (_pbitmapDoubleBuf)
    {
        CRect rc;
        GetClientRect(&rc);
        //_fDoubleBuf = _pbitmapDoubleBuf->CreateCompatibleBitmap(pDC, _cx * _iZoom, _cy * _iZoom);
        _fDoubleBuf = _pbitmapDoubleBuf->CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
    }
}


void CRoomExplorerView::_OnDraw(CDC *pDC, BOOL fHitTestOnly, CPoint point, WORD *pwRoom)
{
    int iModePrev = pDC->SetBkMode(TRANSPARENT);
    _grid.DrawRooms(pDC, fHitTestOnly, point, pwRoom);
    pDC->SetBkMode(iModePrev);
}

void CRoomExplorerView::OnDraw(CDC *pDC)
{
    if (_pQueue == NULL)
    {
        _pQueue = new QueueItems<CRoomExplorerWorkItem, CRoomExplorerWorkResult>(this->GetSafeHwnd(), UWM_ROOMBMPREADY);
        if (_pQueue)
        {
            if (!_pQueue->Init())
            {
                _pQueue->Release();
                _pQueue = NULL;
            }
        }
    }

    RECT rcClient;
    GetClientRect(&rcClient);
    CDC dcMem;
    if (dcMem.CreateCompatibleDC(pDC))
    {
        if (!_fDoubleBuf)
        {
            _GenerateDoubleBuffer(pDC);
        }

        if (_fDoubleBuf)
        {
            ASSERT(_pbitmapDoubleBuf);
            HGDIOBJ hgdiObj = dcMem.SelectObject(_pbitmapDoubleBuf);

            // Fill background
            dcMem.FillSolidRect(0, 0, _cx, _cy, COLOR_BG);


            // Draw the picture.
            _OnDraw(&dcMem);

            // Now blt back to the real DC.
            pDC->StretchBlt(0, 0, _GetViewWidth(), _GetViewHeight(), &dcMem, 0, 0, _GetViewWidth(), _GetViewHeight(), SRCCOPY); 

            dcMem.SelectObject(hgdiObj);

        }
    }

    // Finish off by drawing a navy background around the font.
    CBrush brush(RGB(128, 128, 64));
    int cyBottom = RECTHEIGHT(rcClient) - _GetViewHeight();
    if (cyBottom > 0)
    {
        CRect rc(CPoint(0, _GetViewHeight()), CSize(RECTWIDTH(rcClient), cyBottom));
        pDC->FillRect(&rc, &brush);
    }
    int cxLeft = RECTWIDTH(rcClient) -  _GetViewWidth();
    if (cxLeft > 0)
    {
        CRect rc(CPoint(_GetViewWidth(), 0), CSize(cxLeft, RECTHEIGHT(rcClient)));
        pDC->FillRect(&rc, &brush);
    }
    __super::OnDraw(pDC);
}

void CRoomExplorerView::OnSize(UINT nType, int cx, int cy)
{
    _RecalcHeight();
    __super::OnSize(nType, cx, cy);
    _CleanDoubleBuffer();
}


void CRoomExplorerView::_UpdateCursor()
{
    switch (_iMoveDirection)
    {
    case MOVE_UP:
        SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSORUP)));
        break;
    case MOVE_DOWN:
        SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSORDOWN)));
        break;
    case MOVE_RIGHT:
        SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSORRIGHT)));
        break;
    case MOVE_LEFT:
        SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSORLEFT)));
        break;
    default:
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
    }
}

#define SENSITIVITY 40

CPoint CRoomExplorerView::_GetMoveDelta(int *piMoveDirection)
{
    int iMoveDirection = MOVE_NONE;
    CPoint ptMove(0, 0);

    if (_ptHoverClient.x && _ptHoverClient.y) // don't move when at 0, 0
    {
        CRect rcClient;
        GetClientRect(&rcClient);

        int iRightDelta = rcClient.right - _ptHoverClient.x;
        if (iRightDelta < SENSITIVITY)
        {
            iMoveDirection = MOVE_RIGHT;
            ptMove.x += (SENSITIVITY - iRightDelta) / 2;
        }
        int iLeftDelta = _ptHoverClient.x - rcClient.left;
        if (iLeftDelta < SENSITIVITY)
        {
            iMoveDirection = MOVE_LEFT;
            ptMove.x -= (SENSITIVITY - iLeftDelta) / 2;
        }
        int iBottomDelta = rcClient.bottom - _ptHoverClient.y;
        if (iBottomDelta < SENSITIVITY)
        {
            iMoveDirection = MOVE_DOWN;
            ptMove.y += (SENSITIVITY - iBottomDelta) / 2;
        }
        int iTopDelta = _ptHoverClient.y - rcClient.top;
        if (iTopDelta < SENSITIVITY)
        {
            iMoveDirection = MOVE_UP;
            ptMove.y -= (SENSITIVITY - iTopDelta) / 2;
        }
    }
    if (piMoveDirection)
    {
        *piMoveDirection = iMoveDirection;
    }
    return ptMove;
}

void CRoomExplorerView::OnMouseMove(UINT nFlags, CPoint point)
{
    CRect rcClient;
    GetClientRect(&rcClient);
    BOOL fMouseWithin = rcClient.PtInRect(point);

    if (fMouseWithin)
    {
        WORD wRoom;
        _ptHoverClient = point;
        CPoint ptView = _MapClientPointToView(point);
        ptView.Offset(CPoint(-_xOrigin, -_yOrigin));
        _grid.DrawRooms(NULL, TRUE, ptView, &wRoom);
        _grid.SetHoveredRoom(wRoom);
    }
    else
    {
        _grid.SetHoveredRoom(0);
    }
    __super::OnMouseMove(nFlags, point);

    int iMoveDirection;
    CPoint ptMove = _GetMoveDelta(&iMoveDirection);
    if (iMoveDirection != _iMoveDirection)
    {
        _iMoveDirection = iMoveDirection;
        _UpdateCursor();
    }
    if (fMouseWithin && (ptMove.x || ptMove.y))
    {
        if (!_fMoveTimer)
        {
            _fMoveTimer = TRUE;
            SetTimer(MOUSEMOVE_TIMER, 30, NULL);
        }
    }
    else
    {
        if (_fMoveTimer)
        {
            _fMoveTimer = FALSE;
            KillTimer(MOUSEMOVE_TIMER);
        }
    }

    if (fMouseWithin)
    {
        if (!_fInCapture)
        {
            // Update the cursor as soon as mouse enters the view
            _UpdateCursor();

            _fInCapture = TRUE;
            SetCapture();

            // Track mouseleaves.
            TRACKMOUSEEVENT eventTrack;
            eventTrack.cbSize = sizeof(eventTrack);
            eventTrack.dwFlags = TME_LEAVE;
            eventTrack.hwndTrack = m_hWnd;
            TrackMouseEvent(&eventTrack);
        }
    }
    else
    {
        if (_fInCapture)
        {
            ReleaseCapture();
            _fInCapture = FALSE;
        }
    }
}

void CRoomExplorerView::OnLButtonDown(UINT nFlags, CPoint point)
{
    _fMouseButtonDown = TRUE;
    __super::OnLButtonDown(nFlags, point);
}

void CRoomExplorerView::OnLButtonUp(UINT nFlags, CPoint point)
{
    _fMouseButtonDown = FALSE;
    __super::OnLButtonUp(nFlags, point);
}




LRESULT CRoomExplorerView::_OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    if (_fInCapture)
    {
        _fMouseButtonDown = FALSE;
        ReleaseCapture();
        _fInCapture = FALSE;
    }
    return 0;
}

void CRoomExplorerView::OnKillFocus(CWnd *pNewWnd)
{
    if (_fInCapture)
    {
        ReleaseCapture();
        _fInCapture = FALSE;
    }
}

void CRoomExplorerView::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == MOUSEMOVE_TIMER)
    {
        if (_fMouseButtonDown)
        {
            CRect rcClient;
            GetClientRect(&rcClient);
            CPoint ptMove = _GetMoveDelta();
            _xOrigin += ptMove.x;
            _xOrigin = min(_xOrigin, max(0, (_GetViewWidth() - rcClient.Width())));
            _xOrigin = max(0, _xOrigin);
            _yOrigin += ptMove.y;
            _yOrigin = min(_yOrigin, max(0, (_GetViewHeight() - rcClient.Height())));
            _yOrigin = max(0, _yOrigin);
            _InvalidateScrollStuff();
            Invalidate(FALSE);
        }
    }
    else
    {
        __super::OnTimer(nIDEvent);
    }
}

void CRoomExplorerView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    WORD wRoom;
    CPoint ptView = _MapClientPointToView(point);
    ptView.Offset(CPoint(-_xOrigin, -_yOrigin));
    _grid.DrawRooms(NULL, TRUE, ptView, &wRoom);
    if (wRoom)
    {
        theApp.OpenScript(wRoom);
    }
}

void CRoomExplorerView::InvalidateGridRect(LPRECT prc)
{
    CPoint ptTL = _MapViewPointToClient(CPoint(prc->left, prc->top));
    ptTL.Offset(CPoint(_xOrigin, _yOrigin));
    CPoint ptBR = _MapViewPointToClient(CPoint(prc->right, prc->bottom));
    ptBR.Offset(CPoint(_xOrigin, _yOrigin));
    CRect rect(ptTL, ptBR);
    InvalidateRect(&rect);
}

void CRoomExplorerView::_RecalcHeight()
{
    CSize sizeOld = CSize(_cx, _cy);
    if (!_grid.IsComplete())
    {
        // Acquire and lock the class browser
        SCIClassBrowser &browser = theApp.GetResourceMap().GetClassBrowser();
        browser.Lock();
        _grid.LoadResources();

        for (WORD wScript = 0; wScript < 1000; wScript++)
        {
            const Script *pScript = browser.GetLKGScript(wScript);

            // Look for a room in this script.
            if (pScript)
            {
                const ClassVector &classes = pScript->GetClasses();
                for (size_t i = 0; i < classes.size(); i++)
                {
                    ClassDefinition *pClass = classes[i];
                    if (pClass && pClass->IsInstance())
                    {
                        const std::string &strSuper = pClass->GetSuperClass();
                        if (strSuper == "Rm")
                        {
                            _grid.AddRoom(browser, wScript, pClass, classes);
                            break;
                        }
                    }
                }
            }
        }

        _grid.OrganizeRooms();

        browser.Unlock();
        // Acquire and lock the class browser
    }

    const CRect *prc = _grid.GetBounds();
    _cx = (prc->Width() + 1) * HORZ_SPACE;
    _cy = (prc->Height() + 1) * VERT_SPACE;
}

void CRoomExplorerView::OnInitialUpdate()
{
    __super::OnInitialUpdate();

    _RecalcHeight();
}

void CRoomExplorerView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
    // We have a new thing.
    __super::OnUpdate(pSender, lHint, pHint);

    // Refresh in all cases.
    if (lHint)
    {
        // Need to redraw the pic
        Invalidate(FALSE);
        _CleanDoubleBuffer(); // size migth have changed.
    }
}

//
// prevent flicker
//
BOOL CRoomExplorerView::OnEraseBkgnd(CDC *pDC)
{
    return TRUE;
}

LRESULT CRoomExplorerView::_OnRoomBitmapReady(WPARAM wParam, LPARAM lParam)
{
    CRoomExplorerWorkResult *pWorkResult;
    while (_pQueue->TakeWorkResult(&pWorkResult))
    {
        CRoomExplorerNode *pNode = _grid.GetNode(pWorkResult->wScript);
        if (pNode)
        {
            // Transfer ownership
            pNode->pBitmapData.swap(pWorkResult->pBitmapData);
            if (pNode->pBitmapData.get())
            {
                Invalidate(FALSE);
            }
        }
        delete pWorkResult;
    }
    return 0;
}


//
// This one takes into account origins
//
CPoint CRoomExplorerView::_MapClientPointToView(CPoint ptScreen)
{
    return CPoint((ptScreen.x + _xOrigin) * 1,
                  (ptScreen.y + _yOrigin) * 1);
}

//
// From pic coordinates to window client coordinates
// (taking into account origins)
//
CPoint CRoomExplorerView::_MapViewPointToClient(CPoint ptPic)
{
    return CPoint((ptPic.x / 1) - _xOrigin,
                  (ptPic.y / 1) - _yOrigin);
}


// CRoomExplorerView diagnostics

#ifdef _DEBUG
void CRoomExplorerView::AssertValid() const
{
    __super::AssertValid();
}

void CRoomExplorerView::Dump(CDumpContext& dc) const
{
    __super::Dump(dc);
}

CRoomExplorerDoc* CRoomExplorerView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRoomExplorerDoc)));
    return (CRoomExplorerDoc*)m_pDocument;
}
#endif //_DEBUG