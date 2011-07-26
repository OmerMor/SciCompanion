// CVPCDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "VPCDialogBar.h"
#include "PicResource.h"
#include "PicDoc.h"

// CVPCDialogBar
CVPCDialogBar::CVPCDialogBar(CWnd* pParent /*=NULL*/)
	: CExtDialogFwdCmd(IDD_VPC, pParent)
{
}

CVPCDialogBar::~CVPCDialogBar()
{
}


BEGIN_MESSAGE_MAP(CVPCDialogBar, CExtDialogFwdCmd)
END_MESSAGE_MAP()

void CVPCDialogBar::SetDocument(CDocument *pDoc)
{
    _pDoc = (CPicDoc*)pDoc;
    if (_pDoc)
    {
        _pDoc->AddNonViewClient(this);

        m_wndVSet.SetDialogFactory(_pDoc);
        m_wndPSet.SetDialogFactory(_pDoc);
        m_wndCSet.SetDialogFactory(_pDoc);
        m_wndPenSet.SetDialogFactory(_pDoc);
    }
}


void CVPCDialogBar::UpdateNonView(LPARAM lHint)
{
    if (_pDoc)
    {
        PicDrawManager &pdm = _pDoc->GetDrawManager();

        if (lHint & (VIEWUPDATEHINT_EDITPICPOSCHANGE | VIEWUPDATEHINT_NEWPIC | VIEWUPDATEHINT_EDITPICINVALID))
        {
            // Update our buttons.
            // Get the information we need from the CPicDoc, and give it to the buttons.

            const SCIPicState *pstate = pdm.GetPicState();

            m_wndVSet.SetColorAndState(pstate->color, pstate->dwDrawEnable & DRAW_ENABLE_VISUAL);
            m_wndVToggle.SetCheck((pstate->dwDrawEnable & DRAW_ENABLE_VISUAL) != 0);
            
            EGACOLOR colorP = { pstate->bPriorityValue, pstate->bPriorityValue };
            m_wndPSet.SetColorAndState(colorP, pstate->dwDrawEnable & DRAW_ENABLE_PRIORITY);
            m_wndPToggle.SetCheck((pstate->dwDrawEnable & DRAW_ENABLE_PRIORITY) != 0);

            EGACOLOR colorC = { pstate->bControlValue, pstate->bControlValue };
            m_wndCSet.SetColorAndState(colorC, pstate->dwDrawEnable & DRAW_ENABLE_CONTROL);
            m_wndCToggle.SetCheck((pstate->dwDrawEnable & DRAW_ENABLE_CONTROL) != 0);

            // If the position changed, or we have a new pic, etc...
            // then the pen could have changed.
            lHint |= VIEWUPDATEHINT_PENSTYLECHANGE;
        }

        if (lHint & VIEWUPDATEHINT_PENSTYLECHANGE)
        {
            PenStyle penStyle;
            _pDoc->GetPenStyle(&penStyle);
            m_wndPenSet.SetPenStyle(&penStyle);
        }

        if (lHint & VIEWUPDATEHINT_PALETTECHANGED)
        {
            const SCIPicState *pstate = pdm.GetPicState();
            m_wndVSet.SetColorAndState(pstate->color, pstate->dwDrawEnable & DRAW_ENABLE_VISUAL);
        }
    }
}

// CVPCDialogBar message handlers

void CVPCDialogBar::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	__super::DoDataExchange(pDX);
    ShowSizeGrip(FALSE);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time

	DDX_Control(pDX, IDC_TOGGLEVISUAL, m_wndVToggle);
    DDX_Control(pDX, IDC_TOGGLEPRIORITY, m_wndPToggle);
    DDX_Control(pDX, IDC_TOGGLECONTROL, m_wndCToggle);
	DDX_Control(pDX, IDC_SETVISUAL, m_wndVSet);
    DDX_Control(pDX, IDC_SETPRIORITY, m_wndPSet);
    DDX_Control(pDX, IDC_SETCONTROL, m_wndCSet);

    DDX_Control(pDX, IDC_SETPENSTYLE, m_wndPenSet);

    // Visuals
    DDX_Control(pDX, IDC_STATIC1, m_wndLabel);
}

