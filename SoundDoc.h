#pragma once

#include "ResourceDocument.h"
#include "UndoResource.h"
#include "SoundResource.h"

// CSoundDoc document

//
// Sound view updates
//
// Sound is ready.
#define VIEWUPDATEHINT_SOUNDCHANGED         0x00000100
// Sound synth changed
#define VIEWUPDATEHINT_SOUNDDEVICECHANGED   0x00000200
// The list of cues changed
#define VIEWUPDATEHINT_SOUNDCUECHANGED      0x00000400
// Chosen cue changed
#define VIEWUPDATEHINT_SOUNDACTIVECUECHANGED 0x00000800

class CSoundDoc : public CUndoResource<CResourceDocument, IResourceBase>
{
	DECLARE_DYNCREATE(CSoundDoc)

public:
	CSoundDoc();
	virtual ~CSoundDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o

    // Takes ownership:
    void SetSoundResource(std::auto_ptr<SoundResource> pSound, int id = -1);
    const SoundResource *GetSoundResource() const { return static_cast<const SoundResource*>(GetResource()); }
    SoundResource::DeviceType GetDevice() const { return _device; }
    void SetDevice(SoundResource::DeviceType device, bool fNotify = true);
    void SetChannelMask(SoundResource::DeviceType device, WORD wMask);
    WORD GetChannelMask(SoundResource::DeviceType device) const;
    void AddCuePoint(SoundResource::CuePoint cue);
    void DeleteCue(size_t index);
    void SetCue(size_t index, SoundResource::CuePoint cue);
    void SetLoopPoint(DWORD dwTicks);

    void SetTempo(WORD wTempo);
    WORD GetTempo() const { return _wTempo; }

    void SetActiveCue(int index);
    int GetActiveCue() const { return _cueIndex; }
    void v_OnUndoRedo();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    void afx_msg _OnImportMidi();
    virtual void _OnSuccessfulSave(const IResourceBase *pDoc);
    virtual ResourceType _GetType() const { return RS_SOUND; }

	DECLARE_MESSAGE_MAP()

private:
    // Assumes ownership of this:
    WORD _wTempo;
    int _cueIndex;
    SoundResource::DeviceType _device;

    static SoundResource::DeviceType s_defaultDevice;
};

SoundResource *ImportMidi();

