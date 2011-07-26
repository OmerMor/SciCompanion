#pragma once

#include "NonViewClient.h"

// CNonViewHostDocument document

class CNonViewHostDocument : public CDocument, public INonViewHost
{
public:
    // INonViewHost
    void AddNonViewClient(INonViewClient *pClient);
    void UpdateAllViewsAndNonViews(CView *pSender, LPARAM lHint, CObject *pObject = NULL);

private:
    // Use a set, to ensure we don't add the same client twice.
    std::set<INonViewClient*> _nonViewClients;
};
