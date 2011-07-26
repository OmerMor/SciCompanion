
#pragma once

//
// CMDITabsDialogBar
//
// Dialog bar that hosts a CMDITabs control
//

class CScriptDocument;
class CResourceDocument;
class CMDITabChildWnd;

class CMDITabsDialogBar : public CTabCtrl
{
public:
    void AddTab(CFrameWnd *pFrame, MDITabType iType);
    void RemoveTab(CFrameWnd *pFrame);
    CScriptDocument *ActivateScript(ScriptId script);
    CResourceDocument *ActivateResourceDocument(ResourceType type, WORD wNum);
    void OnActivateTab(CFrameWnd *pFrame);
    bool CanGoBack();
    bool CanGoForward();
    void GoBack();
    void GoForward();
    void OnUpdateTitles();

protected:
	DECLARE_MESSAGE_MAP()

private:
    CDocument *_GetIfKindOf(int i, const CRuntimeClass* pClass, CMDITabChildWnd **pActive) const;
    CFrameWnd *_GetNextPrev(bool fNext);
    void _GoBackForward(bool fForward);
    LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnSelChange(NMHDR *pnmhdr, LRESULT *lResult);
    void DrawItem(LPDRAWITEMSTRUCT);
    void DrawItemBorder(LPDRAWITEMSTRUCT);
    void DrawMainBorder(LPDRAWITEMSTRUCT);

    void OnPaint();
    BOOL OnEraseBkgnd(CDC *pDC);

    CFont m_font;
    CImageList m_imagelist;

    CExtBitmap _tabBitmap[11];
    CExtBitmap _tabBitmapNS[11];

    typedef std::list<CFrameWnd*> travellog_list;
    travellog_list _travelLog;
    CFrameWnd *_pActiveTab;
    bool _fNavigatingViaTravelLog;
};

