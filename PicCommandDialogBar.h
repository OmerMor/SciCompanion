#pragma once

#include "NonViewClient.h"
#include "ExtDialogFwdCmd.h"

class PicResource;
class CPicDoc;

class PicCommandListBox : public CListBox
{
protected:
    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

// PicCommandDialogBar dialog

class PicCommandDialogBar : public CExtDialogFwdCmd, public INonViewClient
{
public:
	PicCommandDialogBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~PicCommandDialogBar();

// Dialog Data
	enum { IDD = IDD_PICCOMMANDS };

    void OnDeleteCommands();

    void SetDocument(CDocument *pDoc);
    CPicDoc *GetDocument() { return _pDoc; }
    int GetCurrentPosition() { return _iCurrentPicPos; }

    // INonViewClient
    void UpdateNonView(LPARAM lHint);

    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

    LRESULT OnOcmDrawItem(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSelChange();
    afx_msg void OnGotoScript();
    void OnCropCommands();
    void OnCopyCommands();
    void OnCutCommands();
    void _OnUpdateCommands();

private:
    void _OnDelete(BOOL fCut, BOOL fCopy);
    void _UpdateItemCount();
    const PicResource *_GetEditPic();
    void _InvalidateCurrentCaretPos();
    
    CPicDoc *_pDoc;
    int _iUserSelectedPos;
    int _iCurrentPicPos;        // Current position in CommandView (equal to pic position)
    bool _fAttached;

    CFont m_font;

    PicCommandListBox m_wndList;

    CExtHyperLinkButton m_wndGotoScript;

    // Visual
    CExtButton m_wndDelete;
    CExtButton m_wndCopy;
    CExtButton m_wndCrop;
    HACCEL _hAccel;
};
