#pragma once

#include "NoFlickerStatic.h"
#include "NonViewClient.h"
#include "ViewResource.h"

// CQuickScripts dialog bar

class CScriptDocument;

class CQuickScripts : public CExtResizableDialog, public INonViewClient
{
public:
	CQuickScripts(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQuickScripts();

    void SetDocument(CDocument *pDoc);

    // INonViewClient
    virtual void UpdateNonView(LPARAM lHint);

// Dialog Data
	enum { IDD = IDD_QUICKSCRIPTS };

    void Initialize();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    BOOL OnInitDialog();
    void OnItemDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
    void OnItemEnter(NMHDR* pNMHDR, LRESULT* pResult);
    BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
private:
    afx_msg void OnGotoPic();
    afx_msg BOOL OnGotoRoom(UINT nID);
    afx_msg void OnGotoView(UINT nID);
    afx_msg void OnUpdateAlwaysOn(CCmdUI *pCmdUI) { pCmdUI->Enable(TRUE); }

    void _InitColumns();
    void _UpdateEntries();
    void _AddFilesOfType(int &iItem, PCTSTR pszWildcard);
    void _OpenItem(int iItem);
    void _ResetUI();
    void _PrepareViewCommands(int iIndex, const ViewResource &view, int nLoop, int nCel);
    void _PrepareViewMenu(int cItems);

    CListCtrl m_wndList;

    CExtNoFlickerStatic m_wndPic;
    WORD _wPic;
    CScriptDocument *_pDoc;
    CExtHyperLinkButton m_wndGotoPic;
    WORD _wNorth;
    CExtHyperLinkButton m_wndNorth;
    WORD _wEast;
    CExtHyperLinkButton m_wndEast;
    WORD _wSouth;
    CExtHyperLinkButton m_wndSouth;
    WORD _wWest;
    CExtHyperLinkButton m_wndWest;

    CExtButton m_wndButtonViews;
    std::vector<WORD> _viewNumbers;

    CExtLabel m_wndScriptNum;

    bool _bFirstTime;
    int _nFirstScript;

    std::string _srcFolder;
};
