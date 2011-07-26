// PreferencesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "PreferencesDialog.h"
#include ".\preferencesdialog.h"


// CPreferencesDialog dialog

CPreferencesDialog::CPreferencesDialog(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CPreferencesDialog::IDD, pParent)
{
    _fBrowseInfoStart = theApp._fBrowseInfo;
}

CPreferencesDialog::~CPreferencesDialog()
{
}

BOOL CPreferencesDialog::OnInitDialog()
{
    BOOL fRet = __super::OnInitDialog();
    // Add the MIDI devices
    UINT cDevices = midiOutGetNumDevs();
    for (UINT i = 0; i < cDevices; i++)
    {
        MIDIOUTCAPS outcaps = { 0 };
        MMRESULT result = midiOutGetDevCaps(i, &outcaps, sizeof(outcaps));
        if (result == MMSYSERR_NOERROR)
        {
            m_wndMIDIDevices.InsertString(-1, outcaps.szPname);
        }
    }
    if (cDevices)
    {
        m_wndMIDIDevices.SetCurSel(0);
    }
    return fRet;
}

void CPreferencesDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
    ShowSizeGrip(FALSE);
    DDX_Check(pDX, IDC_DRAWGRIDLINES, theApp._fGridLines);
    DDX_Check(pDX, IDC_SCALETRACINGIMAGES, theApp._fScaleTracingImages);
    DDX_Check(pDX, IDC_CHECKAUTOSUGGEST, theApp._fUseAutoSuggest);
    DDX_Check(pDX, IDC_CHECKAUTOLOADGAME, theApp._fAutoLoadGame);
    DDX_Check(pDX, IDC_CHECKDUPENEWCELS, theApp._fDupeNewCels);
    DDX_Check(pDX, IDC_CHECKUSEBOXEGO, theApp._fUseBoxEgo);
    DDX_Check(pDX, IDC_CHECKSCI01, theApp._fSCI01);

    DDX_Check(pDX, IDC_BROWSEINFO, theApp._fBrowseInfo);
    DDX_Check(pDX, IDC_PARAMINFO, theApp._fParamInfo);
    DDX_Check(pDX, IDC_CODECOMPLETION, theApp._fCodeCompletion);
    DDX_Check(pDX, IDC_HOVERTIPS, theApp._fHoverTips);
    DDX_Check(pDX, IDC_COMPILEERRORSOUND, theApp._fPlayCompileErrorSound);

    DDX_Text(pDX, IDC_FAKEEGOX, theApp._cxFakeEgo);
    DDV_MinMaxInt(pDX, theApp._cxFakeEgo, 10, 80);
    DDX_Text(pDX, IDC_FAKEEGOY, theApp._cyFakeEgo);
    DDV_MinMaxInt(pDX, theApp._cyFakeEgo, 10, 80);

    DDX_Control(pDX, IDC_COMBO_MIDIDEVICE, m_wndMIDIDevices);

    // Visuals
    DDX_Control(pDX, IDC_BROWSEINFO, m_wndBrowserInfo);
    DDX_Control(pDX, IDC_CODECOMPLETION, m_wndCodeCompletion);
    DDX_Control(pDX, IDC_PARAMINFO, m_wndParamInfo);
    DDX_Control(pDX, IDC_HOVERTIPS, m_wndHoverTips);
    DDX_Control(pDX, IDC_GROUP1, m_wndGroup1);
    DDX_Control(pDX, IDC_SCALETRACINGIMAGES, m_wndCheck1);
    DDX_Control(pDX, IDC_DRAWGRIDLINES, m_wndCheck2);
    DDX_Control(pDX, IDC_CHECKAUTOSUGGEST, m_wndCheck3);
    DDX_Control(pDX, IDC_CHECKAUTOLOADGAME, m_wndCheck4);
    DDX_Control(pDX, IDC_CHECKDUPENEWCELS, m_wndCheck5);
    DDX_Control(pDX, IDC_CHECKUSEBOXEGO, m_wndCheck6);
    DDX_Control(pDX, IDC_CHECKSCI01, m_wndCheck7);
    DDX_Control(pDX, IDC_COMPILEERRORSOUND, m_wndCheck8);
    DDX_Control(pDX, IDOK, m_wndOk);
    DDX_Control(pDX, IDCANCEL, m_wndCancel);
    DDX_Control(pDX, IDC_STATIC1, m_wndStatic1);

    _SyncBrowseInfo();
}

void CPreferencesDialog::_SyncBrowseInfo()
{
    CButton *pCheck = static_cast<CButton*>(GetDlgItem(IDC_BROWSEINFO));
    int iChecked = pCheck->GetCheck();
    GetDlgItem(IDC_PARAMINFO)->EnableWindow(iChecked);
    GetDlgItem(IDC_CODECOMPLETION)->EnableWindow(iChecked);
    GetDlgItem(IDC_HOVERTIPS)->EnableWindow(iChecked);
}


BEGIN_MESSAGE_MAP(CPreferencesDialog, CExtResizableDialog)
    ON_BN_CLICKED(IDC_BROWSEINFO, OnBnClickedBrowseinfo)
END_MESSAGE_MAP()


// CPreferencesDialog message handlers

void CPreferencesDialog::OnBnClickedBrowseinfo()
{
    _SyncBrowseInfo();
}

void CPreferencesDialog::OnOK()
{
    __super::OnOK();
    if (_fBrowseInfoStart != theApp._fBrowseInfo)
    {
        if (theApp._fBrowseInfo)
        {
            theApp.GenerateBrowseInfo();
        }
    }
}

