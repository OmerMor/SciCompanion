// TextDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "SoundDoc.h"
#include "SoundResource.h"

SoundResource::DeviceType CSoundDoc::s_defaultDevice = SoundResource::RolandMT32;

// CSoundDoc

IMPLEMENT_DYNCREATE(CSoundDoc, CResourceDocument)

CSoundDoc::CSoundDoc()
{
    _cueIndex = -1;
    _wTempo = SoundResource::StandardTempo;
    _device = s_defaultDevice;
}


CSoundDoc::~CSoundDoc()
{
}

void CSoundDoc::v_OnUndoRedo()
{
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDCHANGED);
}

void CSoundDoc::SetSoundResource(std::auto_ptr<SoundResource> pSound, int id)
{
    _id = id;
    AddFirstResource(pSound.get());
    _wTempo = pSound->GetTempo();
    pSound.release();
    _UpdateTitle();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDCHANGED);
}

void CSoundDoc::SetActiveCue(int index)
{
    if (_cueIndex != index)
    {
        _cueIndex = index;
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDACTIVECUECHANGED);
    }
}

void CSoundDoc::SetDevice(SoundResource::DeviceType device, bool fNotify)
{
    if (_device != device)
    {
        _device = device;
        s_defaultDevice = _device;
        if (fNotify)
        {
            UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDDEVICECHANGED);
        }
    }
}

void CSoundDoc::SetChannelMask(SoundResource::DeviceType device, WORD wMask)
{
    WORD wMaskOld = GetSoundResource()->GetChannelMask(device);
    if (wMaskOld != wMask)
    {
        SoundResource *pNewSound = new SoundResource(*GetSoundResource());
        pNewSound->SetChannelMask(device, wMask);
        AddNewResourceToUndo(pNewSound);
        SetModifiedFlag(TRUE);
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDCHANGED);
    }
}

WORD CSoundDoc::GetChannelMask(SoundResource::DeviceType device) const
{
    return GetSoundResource()->GetChannelMask(device);
}

void CSoundDoc::AddCuePoint(SoundResource::CuePoint cue)
{
    SoundResource *pNewSound = new SoundResource(*GetSoundResource());
    pNewSound->AddCuePoint(cue);
    AddNewResourceToUndo(pNewSound);
    SetModifiedFlag(TRUE);
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDCUECHANGED);
}

void CSoundDoc::DeleteCue(size_t index)
{
    SoundResource *pNewSound = new SoundResource(*GetSoundResource());
    pNewSound->DeleteCue(index);
    AddNewResourceToUndo(pNewSound);
    SetModifiedFlag(TRUE);
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDCUECHANGED);
}

void CSoundDoc::SetCue(size_t index, SoundResource::CuePoint cue)
{
    SoundResource *pNewSound = new SoundResource(*GetSoundResource());
    pNewSound->SetCue(index, cue);
    AddNewResourceToUndo(pNewSound);
    SetModifiedFlag(TRUE);
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDCUECHANGED);
}

void CSoundDoc::SetLoopPoint(DWORD dwTicks)
{
    SoundResource *pNewSound = new SoundResource(*GetSoundResource());
    pNewSound->SetLoopPoint(dwTicks);
    AddNewResourceToUndo(pNewSound);
    SetModifiedFlag(TRUE);
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDCUECHANGED);
}


BEGIN_MESSAGE_MAP(CSoundDoc, TCLASS_2(CUndoResource, CResourceDocument, IResourceBase))
    ON_COMMAND(ID_IMPORT_MIDI, _OnImportMidi)
END_MESSAGE_MAP()


void CSoundDoc::_OnSuccessfulSave(const IResourceBase *pDoc)
{
    SetLastSaved(pDoc);
}

void CSoundDoc::_OnImportMidi()
{
    std::auto_ptr<SoundResource> pSound(ImportMidi());
    if (pSound.get())
    {
        AddNewResourceToUndo(pSound.get());
        pSound.release();
        SetModifiedFlag(TRUE);
        // Also set the device to GM, since that is what has channels enabled by default...
        _device = SoundResource::NewGM;
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SOUNDCHANGED | VIEWUPDATEHINT_SOUNDDEVICECHANGED);
    }
}

void CSoundDoc::SetTempo(WORD wTempo)
{
    if (_wTempo != wTempo)
    {
        _wTempo = wTempo;
        // Special case for tempo - don't make a new sound.  The tempo will change many times a second
        // when the user is sliding the tempo slider.
        SoundResource *pSound = const_cast<SoundResource*>(GetSoundResource());
        pSound->SetTempo(wTempo);
    }
}


// CSoundDoc diagnostics

#ifdef _DEBUG
void CSoundDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSoundDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSoundDoc serialization

void CSoundDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


const char c_szMidiFilter[] = "Midi Files|*.mid;*.midi|All Files|*.*|";
SoundResource *ImportMidi()
{
    // Create a file dialog.
    CFileDialog fileDialog(TRUE, NULL, NULL, OFN_ENABLESIZING | OFN_EXPLORER | OFN_HIDEREADONLY, c_szMidiFilter);
    fileDialog.m_ofn.lpstrTitle = TEXT("Import midi file");
    if (IDOK == fileDialog.DoModal())
    {
        // Tell EditPic about the image.
        CString fileName = fileDialog.GetFileName();
        std::auto_ptr<SoundResource> pSound(new SoundResource);
        if (pSound->InitializeFromMidi((PCSTR)fileName))
        {
            return pSound.release();
        }
    }
    return NULL;
}
