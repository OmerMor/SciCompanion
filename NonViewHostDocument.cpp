// NonViewHostDocument.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "NonViewHostDocument.h"


// CNonViewHostDocument
void CNonViewHostDocument::UpdateAllViewsAndNonViews(CView *pSender, LPARAM lHint, CObject *pObject)
{
    std::for_each(_nonViewClients.begin(), _nonViewClients.end(), std::bind2nd(_UpdateNonView(), lHint));
    __super::UpdateAllViews(pSender, lHint, pObject);
}

void CNonViewHostDocument::AddNonViewClient(INonViewClient *pClient)
{
    _nonViewClients.insert(pClient);
}

