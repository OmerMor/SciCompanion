// PicDoc.h : interface of the CPicDoc class
//


#pragma once

#include "PicResource.h"
#include "DialogFactory.h"
#include "ResourceDocument.h"
#include "PicDrawManager.h"
#include "UndoResource.h"

// FWD declaration.
class PicResource;
class IResourceBase;

extern const char *c_rgControlColourNames[];

//
// Pic view updates
//
// The selected/edited pic changed
#define VIEWUPDATEHINT_NEWPIC            0x00000001
// Something about the content of the edited pic was invalidated.
#define VIEWUPDATEHINT_EDITPICINVALID       0x00000002
// The current position of the edited pic was changed
#define VIEWUPDATEHINT_EDITPICPOSCHANGE     0x00000004
// The pen style changed
#define VIEWUPDATEHINT_PENSTYLECHANGE       0x00000008
// Some preferences changed (such as drawgridlines, or fake ego size)
#define VIEWUPDATEHINT_PREFERENCESCHANGED   0x00000010
// The palette with which the picture is drawn just changed.
#define VIEWUPDATEHINT_PALETTECHANGED       0x00000020
// Zoom level of the pic changed.
#define VIEWUPDATEHINT_ZOOMLEVELCHANGED     0x00000800

class CPicDoc : public CUndoResource<CResourceDocument, IResourceBase>, public IDialogFactory
{
protected: // create from serialization only
    CPicDoc();
    DECLARE_DYNCREATE(CPicDoc)

// Attributes
public:
    // IDialogFacotry
    void InvokeDialog(UINT nID, RECT *prcTarget);

    void OnToolbarDropDown(NMTOOLBAR *pnmtb);
    void GetPenStyle(PenStyle *pPenStyle) { *pPenStyle = _penStyle; }
    int GetZoom() { return _iZoom; }
    void SetZoom(int iZoom);
    void SetPalette(BYTE b);

    void SeekToPos(INT_PTR iPos, bool fNotify = true);
    void InsertCommand(PicCommand *pCommand);
    void InsertCommands(INT_PTR iStart, INT_PTR cCount, PicCommand *pCommands);
    void InsertCommands(INT_PTR cCount, PicCommand *pCommands);
    void RemoveCommand(INT_PTR iCommandIndex);
    void RemoveCommandRange(INT_PTR iStart, INT_PTR iEnd);

    void v_OnUndoRedo();

// Operations
public:
    PicResource *CloneResource();

// Overrides
public:
    virtual BOOL OnNewDocument();

// Implementation
public:
    virtual ~CPicDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    const PicResource *GetPicResource() const;
    PicDrawManager &GetDrawManager() { _pdm.SetPic(GetPicResource()); return _pdm; };

    // This transfers ownership of PicResource to this class.
    void SetEditPic(PicResource *pEditPic, int id = -1);

protected:
    virtual ResourceType _GetType() const { return RS_PIC; }

    PicDrawManager _pdm;

// Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()

private:

    // Pattern state.  We do not use the pattern state
    // from the current position in the pic.  Instead, we store our
    // own pattern state in the document.
    PenStyle _penStyle;

    // Zoom level of pic.
    int _iZoom;
};

template<class _T>
BOOL InsertPaletteCommands(typename _T *pepic, INT_PTR iPos, const EGACOLOR *pPaletteOrig, EGACOLOR *pPaletteNew, BOOL fWriteEntire)
{
    BOOL fSomethingChanged = FALSE;
    if (fWriteEntire)
    {
        // Write the entire palette, for any palette that changed.
        for (int i = 0; i < 4; i++)
        {
            if (0 != memcmp(&pPaletteOrig[40 * i], &pPaletteNew[40 * i], sizeof(EGACOLOR) * 40))
            {
                // This palette changed.  Write it.
                PicCommand command = PicCommand::CreateSetPalette(i, &pPaletteNew[40 * i]);
                pepic->InsertCommands(iPos, 1, &command);
                fSomethingChanged = TRUE;
            }
        }
    }
    else
    {
        // Cycle through each color, and see if it changed.
        CArray<PicCommand, PicCommand> commands;
        for (int i = 0; i < 160; i++)
        {
            EGACOLOR oldColor = pPaletteOrig[i];
            EGACOLOR newColor = pPaletteNew[i];
            if ((oldColor.color1 != newColor.color1) || (oldColor.color2 != newColor.color2))
            {
                // This entry changed. Write it.
                commands.Add(PicCommand::CreateSetPaletteEntry(i / 40, i % 40, newColor));
            }
        }
        if (commands.GetCount() > 0)
        {
            pepic->InsertCommands(iPos, commands.GetCount(), commands.GetData());
            fSomethingChanged = TRUE;
        }
    }
    return fSomethingChanged;
}

