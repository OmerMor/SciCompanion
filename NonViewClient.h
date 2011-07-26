
#pragma once

// fwd deck
class CView;
class CObject;

class __declspec(uuid("4823762c-a9cf-44d1-8258-881c338a7f9e")) INonViewClient
{
public:
    virtual void UpdateNonView(LPARAM lHint) = 0;
};

class __declspec(uuid("a587f365-64c4-4081-ba38-7d4145055e60")) INonViewHost
{
public:
    // The semantics of AddNonViewClient are such that a client is allowed to add itself
    // multiple times (but should only be called once).
    // Also, the client's life-time must exceed that of the host.
    virtual void AddNonViewClient(INonViewClient *pClient) = 0;
    virtual void UpdateAllViewsAndNonViews(CView *pSender, LPARAM lHint, CObject *pObject = NULL) = 0;
};

// stl helper
struct _UpdateNonView : public std::binary_function<INonViewClient*, LPARAM, void>
{
    void operator()(INonViewClient *pClient, LPARAM lHint) const
    {
        pClient->UpdateNonView(lHint);
    }
};

