#pragma once

#include "NonViewClient.h"
#include "ExtDialogFwdCmd.h"

class SoundResource;
class CSoundDoc;

class MidiCommandListBox : public CListBox
{
protected:
    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

// PicCommandDialogBar dialog

class SoundToolboxDialogBar : public CExtDialogFwdCmd, public INonViewClient
{
public:
	SoundToolboxDialogBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~SoundToolboxDialogBar();

// Dialog Data
	enum { IDD = IDD_SOUNDTOOLBOX };

    void OnDeleteCommands();

    void SetDocument(CDocument *pDoc);
    CSoundDoc *GetDocument() { return _pDoc; }

    // INonViewClient
    void UpdateNonView(LPARAM lHint);

    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

    LRESULT OnOcmDrawItem(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSelChange();
    afx_msg void OnCueChange();
    afx_msg void OnAllChannels();
    afx_msg void OnAddCue();
    afx_msg void OnDeleteCue();
    afx_msg void OnEditTime();
    afx_msg void OnEditValue();
    afx_msg void OnCheckCumulative();

private:
    void _UpdateItemCount();
    void _UpdateCues();
    void _UpdateOnSelectedCue();
    const SoundResource *_GetSoundResource();
    void _InvalidateCurrentCaretPos();
    
    CSoundDoc *_pDoc;
    int _iUserSelectedPos;
    int _iCurrentPicPos;        // Current position in CommandView (equal to pic position)
    bool _fAttached;

    CFont m_font;

    CExtComboBox m_wndDevices;
    CExtCheckBox m_wndCheckAllChannels;
    MidiCommandListBox m_wndList;
    CListBox m_wndCues;
    CExtEdit m_wndEditTime;
    CExtEdit m_wndEditValue;
    CExtButton m_wndAdd;
    CExtButton m_wndDelete;
    CExtCheckBox m_wndCumulative;

    // Visuals
    CExtLabel m_wndStatic1;
    CExtLabel m_wndStatic2;
    CExtLabel m_wndStatic3;
    CExtGroupBox m_wndStatic4;
    CExtLabel m_wndStatic5;

    HACCEL _hAccel;
};
