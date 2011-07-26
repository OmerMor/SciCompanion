
#pragma once
#include "ResourceBase.h"

// fwd decl
class ResourceBlob;

//
// A single event in an SCI sound resource
//
class SoundEvent
{
public:

    enum Command
    {
        NoteOff = 0x80,
        NoteOn = 0x90,
        KeyPressure = 0xA0,
        Control = 0xB0,
        ProgramChange = 0xC0,
        Pressure = 0xD0,
        PitchWheel = 0xE0,
        Special = 0xF0,
    };

    BYTE GetChannel() const  // -> NOTE: we should remove all sound events that specify channel 15!!!! It is used by the interpreter for looping and cuing.
    {
        if (GetCommand() != Special)
        {
            return (_bStatus & 0x0F);
        }
        else
        {
            return 0x0F; // Special applies to all channels
        }
    }
    Command GetCommand()  const { return (Command)(0xF0 & _bStatus); }
    BYTE GetRawStatus() const { return _bStatus; }
    void SetRawStatus(BYTE bStatus) { _bStatus = bStatus; }
    bool IsEffectiveNoteOff() const
    {
        return ((GetCommand() == NoteOff) || ((GetCommand() == NoteOn) && (bParam2 == 0)));
    }

    DWORD wTimeDelta;
    BYTE bParam1;
    BYTE bParam2;

private:
    BYTE _bStatus;
};

class SoundResource : public IResourceBase
{
public:

    class CuePoint
    {
    public:
        enum Type
        {
            Cumulative,
            NonCumulative,
        };
        CuePoint(Type cumulative, DWORD dwTickPos, BYTE value) : _cumulative(cumulative),
            _dwTickPos(dwTickPos), _value(value) {}
        DWORD GetTickPos() const { return _dwTickPos; }
        BYTE GetValue() const { return _value; }
        Type GetType() const { return _cumulative; }
        void SetTickPos(DWORD dwTickPos) { _dwTickPos = dwTickPos; }
        void SetValue(BYTE value);
        void SetType(Type type);
    private:
        DWORD _dwTickPos;
        BYTE _value;
        Type _cumulative;
    };

    enum DeviceType
    {
        RolandMT32 = 0x01,
        YamahaFB01 = 0x02,
        Adlib = 0x04,
        Casio = 0x08,
        Tandy1000 = 0x10,
        PCSpeaker = 0x20,
        DevUnknown = 0x40,
        NewGM = 0x80,
    };

    // For SetChannelMask
    static const WORD AllChannelsMask = 0x7FFF;

    // For loop points
    static const DWORD LoopPointNone = ((DWORD)-1);

    static const WORD StandardTempo = 120;

    SoundResource();

    static std::string GetDeviceName(DeviceType device);

    // this for existing ones.
    HRESULT InitFromResource(const ResourceBlob *prd);
    bool InitializeFromMidi(const std::string &filename);

    void Play(HMIDISTRM handle);
    const std::vector<SoundEvent> &GetEvents() const { return _events; }
    const std::vector<CuePoint> &GetCuePoints() const { return _cues; }
    void AddCuePoint(CuePoint cue);
    void DeleteCue(size_t index);
    void SetCue(size_t index, CuePoint cue);
    DWORD GetLoopPoint() const { return _dwLoopPoint; }
    void SetLoopPoint(DWORD dwTicks);
    WORD GetChannelMask(SoundResource::DeviceType device) const;
    void SetChannelMask(SoundResource::DeviceType device, WORD wChannels);
    WORD GetTimeDivision() const { return _wDivision; }
    void NormalizeToTicks(DWORD dwTicks);
    DWORD GetTotalTicks() const { return _dwTotalTicks; }
    bool CanChangeTempo() const { return _fCanSetTempo; }
    void SetTempo(WORD wTempo) { _wTempoIfChanged = wTempo; }
    WORD GetTempo() const { return _wTempoIfChanged; }

    // IResourceBase
    HRESULT InitNew() { return S_OK; }
    BOOL Serialize(sci::istream *pSerial) const;
    void SetResourceNumber(int i) { _iNumber = i; }
    int GetResourceNumber() const { return _iNumber; }
    void SetPackageNumber(int iNum) { _iPackageNumber = iNum; }
    int GetPackageNumber() const { return _iPackageNumber; }
    ResourceType GetType() const { return RS_SOUND; }

private:
    struct TempoEntry
    {
        DWORD dwTicks;
        DWORD dwTempo;

        bool operator<(const TempoEntry &test) const
        {
            return dwTempo < test.dwTempo;
        }
    };

    void _CombineTracks(const std::vector<std::vector<SoundEvent> > &tracks);
    WORD _ReadMidiFileTrack(size_t nTrack, std::istream &midiFile, std::vector<SoundEvent> &events, std::vector<TempoEntry> &tempoChanges);
    void _RationalizeCuesAndLoops();
    void _NormalizeToSCITempo();
    void _RemoveTempoChanges(const std::vector<TempoEntry> &tempoChanges);

    int _iNumber;
    int _iPackageNumber;
    WORD _channels[15];
    std::vector<SoundEvent> _events;
    WORD _wDivision;
    std::vector<CuePoint> _cues;
    DWORD _dwLoopPoint;       // A single loop-point (tick position)
    DWORD _dwTotalTicks;    // Total length in ticks
    // We don't allow setting the tempo on SCI resources (only MIDI imports)
    // The resolution isn't there.
    bool _fCanSetTempo;     
    bool _fReEvaluateCues;      // Do we need to much around with the cues when saving?
    bool _fReEvaluateLoopPoint; // Do we need to muck around with loop point when saving?
    WORD _wTempoIfChanged;      // The user can change the tempo at which imported midi files
                                // are played.  Since the SCI interpreter always plays at 120bpm,
                                // we need to adjust the actual events when we save.
};


class MidiPlayer
{
public:
    MidiPlayer();
    ~MidiPlayer();
    DWORD SetSound(const SoundResource &sound, WORD wTempo);
    void Play();
    void Pause();
    void Stop();
    bool CanPlay();
    bool CanPause();
    bool CanStop();
    DWORD QueryPosition(DWORD scope);
    DWORD QueryPosition();
    void SetTempo(WORD wTempo) { _wTempo = wTempo; _SetTempoAndDivision(); }
    void SetDevice(SoundResource::DeviceType device) { _device = device; }
    void CueTickPosition(DWORD dwTicks);
    void CuePosition(DWORD dwTicks, DWORD scope);

private:
    bool _Init();
    void _SetTempoAndDivision();
    void _ClearHeaders();
    void static CALLBACK s_MidiOutProc(HMIDIOUT hmo, UINT wMsg, DWORD_PTR dwInstance,  DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    void _OnStreamDone();
    void _CuePosition(DWORD dwEventIndex);

    HMIDISTRM _handle;
    MIDIHDR _midiHdr;
    DWORD _cRemainingStreamEvents; // In case it didn't fit into a 64k chunk
    DWORD _cTotalStreamEvents;
    DWORD *_pRealData; // Full data for the 64k chunk in _midiHdr.
    std::vector<DWORD> _accumulatedStreamTicks;   // Corresponds to _midiHdr.lpData / 3
    DWORD _dwCurrentChunkTickStart;
    DWORD _dwCurrentTickPos;
    SoundResource::DeviceType _device;
    bool _fPlaying;
    bool _fQueuedUp;
    bool _fStoppingStream;
    DWORD _wTotalTime;
    WORD _wTempo;
    WORD _wTimeDivision;
    DWORD _dwLoopPoint;
    DWORD _dwCookie;
};

extern MidiPlayer g_midiPlayer;

void PopulateComboWithDevicesHelper(CComboBox &combo);
SoundResource::DeviceType GetDeviceFromComboHelper(CComboBox &combo);
void SelectDeviceInComboHelper(CComboBox &combo, SoundResource::DeviceType);
