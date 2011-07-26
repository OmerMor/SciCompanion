// ChoosePriConDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ChoosePriConDialog.h"



// CChoosePriConDialog dialog

CChoosePriConDialog::CChoosePriConDialog(CWnd* pParent /*=NULL*/)
	: CBaseColorDialog(CChoosePriConDialog::IDD, pParent)
{
    // 16 things here.
    m_wndStatic.SetRowsAndColumns(2, 8);
    m_wndStatic.ShowSelection(TRUE);
    m_wndStatic.ShowIndices(TRUE);
    m_wndStatic.SetPalette(g_egaColorChooserPalette);// just provide a basic palette
    _rgStatusText = NULL;
}

CChoosePriConDialog::~CChoosePriConDialog()
{
}

void CChoosePriConDialog::DoDataExchange(CDataExchange* pDX)
{
	CBaseColorDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHOOSECOLORSTATIC, m_wndStatic);
    m_wndStatic.SetCallback(this);
    DDX_Control(pDX, IDC_DIALOGDESCRIPTION, m_wndStaticCaption);

    DDX_Control(pDX, IDC_STATICSTATUS, m_wndStaticStatus);
    if (_rgStatusText == NULL)
    {
        m_wndStaticStatus.ShowWindow(SW_HIDE);
    }

    GetDlgItem(IDC_CHECK_OPTION)->ShowWindow(SW_HIDE);

    // Set the caption.
    TCHAR szBuf[MAX_PATH];
    LoadString(AfxGetResourceHandle(), _nID, szBuf, ARRAYSIZE(szBuf));
    m_wndStaticCaption.SetWindowText(szBuf);

    // Visuals
    DDX_Control(pDX, IDCANCEL, m_wndCancel);
}


BEGIN_MESSAGE_MAP(CChoosePriConDialog, CBaseColorDialog)
END_MESSAGE_MAP()


// CChoosePriConDialog message handlers

void CChoosePriConDialog::OnColorClick(BYTE bIndex, int nID, BOOL fLeftClick)
{
    if (fLeftClick)
    {
        // We only have one child - it must have been it.
        SetColor(bIndex);
        _fEnded = TRUE;
        UpdateData(TRUE);
        EndDialog(IDOK);
    }
}

void CChoosePriConDialog::OnColorHover(BYTE bIndex)
{
    if (_rgStatusText)
    {
        if (bIndex < 16)
        {
            m_wndStaticStatus.SetWindowText(_rgStatusText[bIndex]);
        }
        else
        {
            m_wndStaticStatus.SetWindowText("");
        }
    }
}