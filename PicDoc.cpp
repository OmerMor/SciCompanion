// EditPicDoc.cpp : implementation of the CPicDoc class
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "PicDoc.h"
#include "PicResource.h"
#include "ChooseColorDialog.h"
#include "ChoosePriConDialog.h"
#include "ChoosePenStyleDialog.h"
#include "ResourceMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


__int64 g_Ticks = 0;
__int64 g_TicksIncAdd = 0;

// CPicDoc

IMPLEMENT_DYNCREATE(CPicDoc, CResourceDocument)

BEGIN_MESSAGE_MAP(CPicDoc, TCLASS_2(CUndoResource, CResourceDocument, IResourceBase))
END_MESSAGE_MAP()

// CPicDoc construction/destruction

CPicDoc::CPicDoc()
{
    InitPenStyle(&_penStyle);
}

CPicDoc::~CPicDoc()
{
}

const char *c_rgControlColourNames[] =
{
    "ctlBLACK",
    "ctlNAVY",
    "ctlGREEN",
    "ctlTEAL",
    "ctlMAROON",
    "ctlPURPLE",
    "ctlBROWN",
    "ctlSILVER",
    "ctlGREY",
    "ctlBLUE",
    "ctlLIME",
    "ctlCYAN",
    "ctlRED",
    "ctlFUCHSIA",
    "ctlYELLOW",
    "ctlWHITE",
};

void CPicDoc::InvokeDialog(UINT nID, RECT *prcButton)
{
    const PicResource *pPic = GetPicResource();
    if (pPic)
    {
        _pdm.SetPic(pPic);
        switch (nID)
        {
        case IDC_SETVISUAL:
            {
                CChooseColorDialog dialog;
                const SCIPicState *ppicState = _pdm.GetPicState();;
                BYTE bPalette = _pdm.GetPalette();
                ASSERT((bPalette >= 0) && (bPalette < 4));
                dialog.Init(&ppicState->pPalettes[bPalette * 40]);
                dialog.SetColor(ppicState->bPaletteOffset);
                dialog.SetTrackRect(prcButton);
                if (IDOK == dialog.DoModal())
                {
                    BYTE bIndex = dialog.GetColor();

                    // Time to create a command.
                    PicCommand command = PicCommand::CreateSetVisual(0, bIndex);
                    InsertCommand(&command);
                }
            }
            break;

        case IDC_SETPRIORITY:
            {
                CChoosePriConDialog dialog;
                const SCIPicState *ppicState = _pdm.GetPicState();
                dialog.SetCaptionID(IDS_CHOOSEAPRIORITY);
                dialog.SetColor(ppicState->bPriorityValue);
                dialog.SetTrackRect(prcButton);
                if (IDOK == dialog.DoModal())
                {
                    BYTE bIndex = dialog.GetColor();
                    // Time to create a command.
                    PicCommand command = PicCommand::CreateSetPriority(bIndex);
                    InsertCommand(&command);
                }
            }
            break;

        case IDC_SETCONTROL:
            {
                CChoosePriConDialog dialog;
                const SCIPicState *ppicState = _pdm.GetPicState();
                dialog.SetCaptionID(IDS_CHOOSEACONTROL);
                dialog.SetColor(ppicState->bControlValue);
                dialog.SetTrackRect(prcButton);
                dialog.SetStatusText(c_rgControlColourNames);
                if (IDOK == dialog.DoModal())
                {
                    BYTE bIndex = dialog.GetColor();
                    // Time to create a command.
                    PicCommand command = PicCommand::CreateSetControl(bIndex);
                    InsertCommand(&command);
                }
            }
            break;

        case IDC_SETPENSTYLE:
            {
                CChoosePenStyleDialog dialog;
                dialog.SetPenStyle(&_penStyle);
                dialog.SetTrackRect(prcButton);
                if (IDOK == dialog.DoModal())
                {
                    dialog.GetPenStyle(&_penStyle);
                    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_PENSTYLECHANGE);
                    // Just set our state, issue no commands.
                }
            }
            break;
            break;
        }
    }           
}


void CPicDoc::OnToolbarDropDown(NMTOOLBAR *pnmtb)
{
    if (pnmtb->iItem == ID_SHOWTRACEIMAGE)
    {
        // Bring up the alpha slider
        // Need to figure out the screen location of the button.
        RECT rcButton;
        // Get its rect in the toolbar:
        ::SendMessage(pnmtb->hdr.hwndFrom, TB_GETRECT, pnmtb->iItem, (LPARAM)&rcButton);

        RECT rcToolbar;
        // Then the toolbar's rect:
        ::GetWindowRect(pnmtb->hdr.hwndFrom, &rcToolbar);

        // Adjust, and invoke the dialog.
        OffsetRect(&rcButton, rcToolbar.left, rcToolbar.top);
        InvokeDialog(ID_SHOWTRACEIMAGE, &rcButton);
    }
}

BOOL CPicDoc::OnNewDocument()
{
    if (!__super::OnNewDocument())
        return FALSE;

    return TRUE;
}


// This transfers ownership of PicResource to this class.
void CPicDoc::SetEditPic(PicResource *pEditPic, int id)
{
    _id = id;
    AddFirstResource(pEditPic);
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_NEWPIC);
    _UpdateTitle();
}

const PicResource *CPicDoc::GetPicResource() const
{
    return static_cast<const PicResource*>(GetResource());
}

void CPicDoc::SetZoom(int iZoom)
{
    iZoom = max(1, iZoom);
    iZoom = min(15, iZoom);
    if (iZoom != _iZoom)
    {
        _iZoom = iZoom;
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_ZOOMLEVELCHANGED);
    }
}

void CPicDoc::SetPalette(BYTE b)
{
    _pdm.SetPalette(b);
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_PALETTECHANGED);
}

void CPicDoc::SeekToPos(INT_PTR iPos, bool fNotify)
{
    if (_pdm.SeekToPos(iPos))
    {
        if (fNotify)
        {
            UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_EDITPICPOSCHANGE);
        }
    }
}

INT_PTR _NextPicPos(INT_PTR iPos, INT_PTR delta)
{
    if (iPos != -1)
    {
        iPos += delta;
    }
    return iPos;
}

INT_PTR _PrevPicPos(INT_PTR iPos, INT_PTR delta)
{
    if (iPos != -1)
    {
        iPos -= delta;
        iPos = max(0, iPos);
    }
    return iPos;
}

void CPicDoc::InsertCommand(PicCommand *pCommand)
{
    PicResource *pNewPic = CloneResource();
    pNewPic->InsertCommands(_pdm.GetPos(), 1, pCommand);
    SeekToPos(_NextPicPos(_pdm.GetPos(), 1), false);
    AddNewResourceToUndo(pNewPic);
    _pdm.Invalidate();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_EDITPICINVALID | VIEWUPDATEHINT_EDITPICPOSCHANGE);
    SetModifiedFlag(TRUE);
}
void CPicDoc::InsertCommands(INT_PTR iStart, INT_PTR cCount, PicCommand *pCommands)
{
    PicResource *pNewPic = CloneResource();
    pNewPic->InsertCommands(iStart, cCount, pCommands);
    INT_PTR iPos = _pdm.GetPos();
    if (iStart <= iPos)
    {
        SeekToPos(_NextPicPos(iPos, cCount), false);
    }
    AddNewResourceToUndo(pNewPic);
    _pdm.Invalidate();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_EDITPICINVALID | VIEWUPDATEHINT_EDITPICPOSCHANGE);
    SetModifiedFlag(TRUE);
}
void CPicDoc::InsertCommands(INT_PTR cCount, PicCommand *pCommands)
{
    PicResource *pNewPic = CloneResource();
    pNewPic->InsertCommands(_pdm.GetPos(), cCount, pCommands);
    SeekToPos(_NextPicPos(_pdm.GetPos(), cCount), false);
    AddNewResourceToUndo(pNewPic);
    _pdm.Invalidate();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_EDITPICINVALID | VIEWUPDATEHINT_EDITPICPOSCHANGE);
    SetModifiedFlag(TRUE);
}
void CPicDoc::RemoveCommand(INT_PTR iCommandIndex)
{
    PicResource *pNewPic = CloneResource();
    pNewPic->RemoveCommand(iCommandIndex);
    INT_PTR iPos = _pdm.GetPos();
    if (iCommandIndex < iPos)
    {
        SeekToPos(_PrevPicPos(iPos, 1), false);
    }
    AddNewResourceToUndo(pNewPic);
    _pdm.Invalidate();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_EDITPICINVALID | VIEWUPDATEHINT_EDITPICPOSCHANGE);
    SetModifiedFlag(TRUE);
}
void CPicDoc::RemoveCommandRange(INT_PTR iStart, INT_PTR iEnd)
{
    PicResource *pNewPic = CloneResource();
    pNewPic->RemoveCommandRange(iStart, iEnd);
    INT_PTR iPos = _pdm.GetPos();
    if (iStart < iPos)
    {
        SeekToPos(_PrevPicPos(iPos, (iEnd - iStart)), false);
    }
    AddNewResourceToUndo(pNewPic);
    _pdm.Invalidate();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_EDITPICINVALID | VIEWUPDATEHINT_EDITPICPOSCHANGE);
    SetModifiedFlag(TRUE);
}

PicResource *CPicDoc::CloneResource()
{
    const PicResource *pResource = static_cast<const PicResource *>(GetResource());
    return new PicResource(*pResource);
}

void CPicDoc::v_OnUndoRedo()
{
    _pdm.Invalidate();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_EDITPICINVALID);
}

// CPicDoc diagnostics

#ifdef _DEBUG
void CPicDoc::AssertValid() const
{
    __super::AssertValid();
}

void CPicDoc::Dump(CDumpContext& dc) const
{
    __super::Dump(dc);
}
#endif //_DEBUG


// CPicDoc commands
