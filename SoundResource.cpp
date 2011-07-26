#include "stdafx.h"
#include "SoundResource.h"
#include "SCIPicEditor.h"

bool _GetDeltaTime(sci::istream &stream, DWORD *pw)
{
    *pw = 0;
    BYTE b;
    stream >> b;
    while (stream.good())
    {
        if (b == 0xf8) // special marker
        {
            *pw += 240;
            stream >> b;
        }
        else
        {
            *pw += b;
            break;
        }
        ASSERT(b != 0xfc); // TODO: we don't handle this yet
    }
    return stream.good();
}

bool _IsStatusByte(BYTE b)
{
    return ((b & 0x80) == 0x80); // Status byte if most significant bit is set.
}


SoundEvent::Command _GetSoundCommand(BYTE bStatus)
{
    return (SoundEvent::Command)(bStatus & 0xF0);
}

BYTE _GetChannel(BYTE bStatus)
{
    return bStatus & 0x0F; // Lower nibble
}

// When played at 120bpm (our default), the time division for SCI games is 30.
// (60 ticks per second, and a quarter note lasts half a second for 120 bpm)
#define SCI_PPQN 30

SoundResource::SoundResource()
{
    ZeroMemory(_channels, sizeof(_channels));
    _wDivision = SCI_PPQN; // by default
    _dwLoopPoint = LoopPointNone;
    _dwTotalTicks = 0;
    _fCanSetTempo = false;
    _fReEvaluateCues = false;
    _fReEvaluateLoopPoint = false;
    _wTempoIfChanged = StandardTempo;
    _iNumber = -1;
    _iPackageNumber = -1;
}

void SoundResource::CuePoint::SetValue(BYTE value)
{
    _value = value;
    if (_cumulative == NonCumulative)
    {
        // 126 is the max value for a non-cumulative cue.
        _value = min(126, _value);
    }
}
void SoundResource::CuePoint::SetType(Type type)
{
    _cumulative = type;
    if (_cumulative == NonCumulative)
    {
        // 126 is the max value for a non-cumulative cue.
        _value = min(126, _value);
    }
}


WORD SoundResource::GetChannelMask(DeviceType device) const
{
    WORD wMask = 0;
    for (int i = 0; i < ARRAYSIZE(_channels); ++i)
    {
        if (((BYTE)(_channels[i] >> 8)) & device)
        {
            // this channel is used
            wMask |= (0x0001 << i);
        }
    }

    // Special cases:
    if (device == RolandMT32)
    {
        // Always include channel nine
        wMask |= 0x0200;
    }
    return wMask;
}

void SoundResource::SetChannelMask(SoundResource::DeviceType device, WORD wChannels)
{
    WORD wDeviceMask = ((WORD)device) << 8;
    for (size_t i = 0; i < ARRAYSIZE(_channels); ++i)
    {
        if (wChannels & (0x0001 << i))
        {
            // This channel is used
            _channels[i] |= wDeviceMask;
        }
        else
        {
            // This one is not
            _channels[i] &= ~wDeviceMask;
        }
    }
}

HRESULT SoundResource::InitFromResource(const ResourceBlob *prd)
{
    _iNumber = prd->GetNumber();
    _iPackageNumber = prd->GetPackageHint();

    sci::istream stream = prd->GetReadStream();
    BYTE b;
    stream >> b; // Digital sample flag - we don't care about this.
    for (int i = 0; i < ARRAYSIZE(_channels); ++i)
    {
        stream >> _channels[i];
    }
    WORD wSoundEffectOffset;
    stream >> wSoundEffectOffset;

    // Get delta time.
    bool fDone = false;
    _dwTotalTicks = 0;
    while (!fDone && stream.good())
    {
        SoundEvent event;
        // Get the delta time
        _GetDeltaTime(stream, &event.wTimeDelta);
        
        BYTE bStatus;
        if (stream.good())
        {
            _dwTotalTicks += event.wTimeDelta;
            stream >> bStatus;
        }
        if (stream.good())
        {
            if (_IsStatusByte(bStatus))
            {
                event.SetRawStatus(bStatus);
            }
            else
            {
                // Repeat last status.
                if (!_events.empty())
                {
                    event.SetRawStatus(_events[_events.size() - 1].GetRawStatus());
                }
                // else TODO: error conditions

                // And back up one byte
                stream.seekg(stream.tellg() - 1);
            }

            switch (event.GetCommand())
            {
            case SoundEvent::NoteOff:
            case SoundEvent::NoteOn:
            case SoundEvent::KeyPressure:
            case SoundEvent::Control:
            case SoundEvent::PitchWheel:
                stream >> event.bParam1;
                stream >> event.bParam2;
                if ((event.GetCommand() == SoundEvent::Control) && (event.bParam1 == 0x60))
                {
                    CuePoint cp(CuePoint::Cumulative, _dwTotalTicks, event.bParam2);
                    _cues.push_back(cp);
                }
                break;

            case SoundEvent::ProgramChange:
            case SoundEvent::Pressure:
                stream >> event.bParam1;
                if (event.GetCommand() == SoundEvent::ProgramChange)
                {
                    if (event.GetChannel() == 15)
                    {
                        if (event.bParam1 < 127)
                        {
                            CuePoint cp(CuePoint::NonCumulative, _dwTotalTicks, event.bParam1);
                            _cues.push_back(cp);
                        }
                        else if (event.bParam1 == 127)
                        {
                            _dwLoopPoint = _dwTotalTicks;
                        }
                        // else nothign
                    }
                    // fall through
                }
                break;

            case SoundEvent::Special:
                switch(bStatus)
                {
                case 0xF0:
                    {
                        // Eat bytes until 0xF7
                        bool fNotEnd = true;
                        while (stream.good() && fNotEnd)
                        {
                            BYTE bByte;
                            stream >> bByte;
                            fNotEnd = (bByte != 0xF7);
                        }
                        if (stream.good())
                        {
                            stream.seekg(stream.tellg() - 1); // Go back one.
                        }
                    }
                    break;
                case 0xFC:
                    fDone = true;
                    break;
                default:
                    ASSERT(FALSE); // TODO: deal with this: improper sound
                    break;
                }
                break;
            }
            _events.push_back(event);
        }
    }
    if (!stream.good())
    {
        theApp.LogInfo("Corrupt sound resource.\n");
    }
    return S_OK;
}


BYTE _GetC(std::istream &midiFile)
{
    char c;
    midiFile.read(&c, 1);
    return c;
}

WORD _ReadBEWORD(std::istream &stream)
{
    WORD w = _GetC(stream);
    w = (w << 8) | _GetC(stream);
    return w;
}
DWORD _ReadBEDWORD(std::istream &stream)
{
    DWORD dw = _GetC(stream);
    dw = (dw << 8) | _GetC(stream);
    dw = (dw << 8) | _GetC(stream);
    dw = (dw << 8) | _GetC(stream);
    return dw;
}



DWORD ReadVarLen(std::istream &midiFile)
{
    DWORD value;
    BYTE c;
    if ((value = _GetC(midiFile)) & 0x80)
    {
        value &= 0x7F;
        do
        {
            value = (value << 7) + ((c = _GetC(midiFile)) & 0x7F);
        }
        while (c & 0x80);
    }
    return value;
}


//
// Reads a midi file into the sound resource, and returns a WORD that is the channel mask
// of used channels.
//
WORD SoundResource::_ReadMidiFileTrack(size_t nTrack, std::istream &midiFile, std::vector<SoundEvent> &events, std::vector<TempoEntry> &tempoChanges)
{
    WORD wChannelMask = 0;
    DWORD dw = _ReadBEDWORD(midiFile);
    DWORD dwTotalTicks = 0;
    while (dw != 0x4D54726B && midiFile.good())
    {
        // Skip this random chunk
        dw = _ReadBEDWORD(midiFile); // size of the chunk
        midiFile.seekg(dw, std::ios_base::cur);
        dw = _ReadBEDWORD(midiFile); // Next chunk id
    }
    if (midiFile.good())
    {
        // How long is it?
        DWORD cbBytes = _ReadBEDWORD(midiFile);

        // Read our track.
        BYTE bLastStatus = 0;
        bool fSkip = false;
        DWORD dwMissingTime = 0;
        std::istream::pos_type curPosDebug = midiFile.tellg();
        std::istream::pos_type finalPos = midiFile.tellg() + (std::istream::pos_type)cbBytes;
        while (midiFile.good() && (midiFile.tellg() < finalPos))
        {
            SoundEvent event;
            // We start with time delta
            event.wTimeDelta = ReadVarLen(midiFile);
            dwTotalTicks += event.wTimeDelta;
            event.wTimeDelta += dwMissingTime;
            fSkip = false;

            // Now the status byte
            BYTE bStatus = _GetC(midiFile);
            if (bStatus & 0x80)
            {
                // High bit is set.  A new status.
                bLastStatus = bStatus;
            }
            else
            {
                // Re-use the last one...
                bStatus = bLastStatus;
                ASSERT((bLastStatus & 0xF0) != 0xF0);
                // Seek backwards one byte:
                midiFile.seekg(-1, std::ios_base::cur);
            }
            // Use last status
            ASSERT(bStatus);
            event.SetRawStatus(bStatus);

            // Read one or two bytes
            switch (bStatus & 0xF0)
            {
            case SoundEvent::NoteOff:
            case SoundEvent::NoteOn:
            case SoundEvent::KeyPressure:
            case SoundEvent::Control:
            case SoundEvent::PitchWheel:
                event.bParam1 = _GetC(midiFile);
                event.bParam2 = _GetC(midiFile);
                break;
            case SoundEvent::ProgramChange:
            case SoundEvent::Pressure:
                event.bParam1 = _GetC(midiFile);
                break;
            case SoundEvent::Special:
                if (bStatus == 0xFF)
                {
                    // Non-midi event (instrument, tempo, time sig, etc..)
                    BYTE bType = _GetC(midiFile);
                    DWORD cbBytes = ReadVarLen(midiFile);
                    ASSERT(cbBytes < 0x0000ffff); // debug sanity check
                    // Skip it... (we may eventually use it, not sure yet)
                    if ((bType == 0x51) && (cbBytes == 3))
                    {
                        if (nTrack != 0)
                        {
                            theApp.LogInfo("Found tempo event in track %d in midi file - ignoring\n", nTrack);
                        }
                        else
                        {
                            // This is a tempo change event.  
                            DWORD dwMSPerQN = (static_cast<DWORD>(_GetC(midiFile))) << 16;
                            dwMSPerQN |= (static_cast<DWORD>(_GetC(midiFile))) << 8;
                            dwMSPerQN |= _GetC(midiFile);
                            TempoEntry tempoEntry = { (dwTotalTicks + event.wTimeDelta), ((60000000 / dwMSPerQN)) };
                            if (tempoChanges.empty() && (tempoEntry.dwTicks != 0))
                            {
                                // Stick a 120bpm at position 0 if this is the first entry and its not a pos 0
                                TempoEntry tempo120 = { 0, 120 };
                                tempoChanges.push_back(tempo120);
                            }
                            tempoChanges.push_back(tempoEntry);
                        }
                        fSkip = true;
                    }
                    else
                    {
                        midiFile.seekg(cbBytes, std::ios_base::cur);
                    }
                }
                else
                {
                    ASSERT((bStatus == 0xF0) || (bStatus == 0xF7));
                    DWORD cbBytes = ReadVarLen(midiFile);
                    ASSERT(cbBytes < 0x0000ffff); // debug sanity check
                    // Skip the data.  We don't handle these (other than using the timedelta)
                    midiFile.seekg(cbBytes, std::ios_base::cur);
                }
                break;
            default:
                ASSERT(FALSE);
                break;
            }
            if (event.GetChannel() != 15) // We don't support 15.  SCI uses it for cues and loops.
            {
                // Ensure this channel is in the mask.
                wChannelMask |= 0x1 << event.GetChannel();
            }
            if (fSkip)
            {
                dwMissingTime += event.wTimeDelta;
            }
            else
            {
                events.push_back(event);
                dwMissingTime = 0;
            }
        }
        // Assuming the midi file didn't lie...
        std::istream::pos_type curPos = midiFile.tellg();
        if (curPos != finalPos)
        {
            OutputDebugString("Corrupt MIDI file? Track length is wrong.\n");
            // Handle this by resetting it for reading the next track.
            midiFile.seekg(finalPos, std::ios_base::beg);
        }
    }
    return wChannelMask;
}   

void SoundResource::_CombineTracks(const std::vector<std::vector<SoundEvent> > &tracks)
{
    // We'll need to store a position for each track.  Let's initialize them to zero.
    std::vector<size_t> _trackPos;
    _trackPos.insert(_trackPos.begin(), tracks.size(), 0);
    // And keep a running count of the time delta for each track.
    std::vector<DWORD> _trackTimeDelta;
    _trackTimeDelta.insert(_trackTimeDelta.begin(), tracks.size(), 0);

    // And here's our overall time counter.
    DWORD dwTimeDelta = 0;
    DWORD dwLastTimeDelta = 0;

    while (true)
    {
        // Find the next event, time-wise
        DWORD dwClosest = (DWORD)(-1);
        size_t bestTrack = tracks.size(); // out of bounds
        for (size_t i = 0; i < tracks.size(); i++)
        {
            const std::vector<SoundEvent> &track = tracks[i];
            if (_trackPos[i] < track.size())
            {
                // Still events remaining in this track, so it's under consideration.
                ASSERT((_trackTimeDelta[i] + track[_trackPos[i]].wTimeDelta) >= dwTimeDelta);
                DWORD dwTimeToNextEvent = (_trackTimeDelta[i] + track[_trackPos[i]].wTimeDelta) - dwTimeDelta;
                if (dwTimeToNextEvent < dwClosest)
                {
                    bestTrack = i; // This is the best track so far.
                    dwClosest = dwTimeToNextEvent;
                }
            }
        }
        if (bestTrack < tracks.size())
        {
            // Add this event.
            size_t posInTrack = _trackPos[bestTrack];
            SoundEvent event = tracks[bestTrack][posInTrack];
            // Now update things.
            _trackTimeDelta[bestTrack] += event.wTimeDelta;
            // This is now our new time delta
            dwTimeDelta = _trackTimeDelta[bestTrack];
            _trackPos[bestTrack]++; // We took one event from this track.

            // Fixup the event time before adding it.
            event.wTimeDelta = dwTimeDelta - dwLastTimeDelta;
            _events.push_back(event);
            dwLastTimeDelta = dwTimeDelta;
        }
        else
        {
            break; // No more events left.
        }
    }
    _dwTotalTicks = dwLastTimeDelta;
}

//
// Adjust the timing of each event so that it sounds the same as it currently does,
// but using a 30 PPQN time division and 120bpm (this is what the SCI interpreter uses by default)
void SoundResource::_NormalizeToSCITempo()
{
    DWORD dwTimeOrig = 0;
    DWORD dwLastTimeOrig = 0;
    DWORD dwLastTimeNew = 0;
    for (size_t i = 0; i < _events.size(); i++)
    {
        SoundEvent &event = _events[i];
        dwTimeOrig = dwLastTimeOrig + event.wTimeDelta;
        event.wTimeDelta = (DWORD)MulDiv(MulDiv(dwTimeOrig, SCI_PPQN, (int)_wDivision), StandardTempo, _wTempoIfChanged) - dwLastTimeNew;
        dwLastTimeNew += event.wTimeDelta;
        dwLastTimeOrig = dwTimeOrig;
    }
    _wDivision = SCI_PPQN;
    _wTempoIfChanged = StandardTempo;
    _dwTotalTicks = dwLastTimeNew;
}

// Adjust the timing of each event so that the whole resource ends up being a certain amount of ticks
void SoundResource::NormalizeToTicks(DWORD dwTicks)
{
    DWORD dwLastTimeOrig = 0;
    DWORD dwLastTimeNew = 0;
    size_t i = 0;

    DWORD totalticksTest = 0;
    for (; i < _events.size(); i++)
    {
        SoundEvent &event = _events[i];

        totalticksTest +=event.wTimeDelta;

        DWORD dwTimeTotalOrig = dwLastTimeOrig + event.wTimeDelta;
        //ASSERT((DWORD)_dwTotalTicks > dwLastTimeNew);
        ASSERT(event.wTimeDelta < 65000); // Just some sanity check for overflows.
        DWORD dwTimeTotalNew = MulDiv(dwTimeTotalOrig, dwTicks, _dwTotalTicks);
        event.wTimeDelta = dwTimeTotalNew - dwLastTimeNew;        

        dwLastTimeNew = dwTimeTotalNew;
        dwLastTimeOrig = dwTimeTotalOrig;
    }
    ASSERT(totalticksTest == _dwTotalTicks);
    _dwTotalTicks = dwLastTimeNew;
}

void SoundResource::_RemoveTempoChanges(const std::vector<TempoEntry> &tempoChanges)
{
    // Find the fastest tempo, and set that as our tempo.
    TempoEntry entry = *max_element(tempoChanges.begin(), tempoChanges.end());
    _wTempoIfChanged = static_cast<WORD>(entry.dwTempo);
    // Now adjust events
    if (tempoChanges.size() > 1)
    {
        _dwTotalTicks = 0; // We're going to recalculate this.

        // Grab the first tempo
        DWORD dwOriginalTicks = 0;
        DWORD dwOldTempo = tempoChanges[0].dwTempo;
        DWORD dwNewTempo = _wTempoIfChanged;
        size_t tempoIndex = 0;
        DWORD dwTotalTicksAtOldTempo = 0;
        DWORD dwTotalTicksAtNewTempo = 0;
        for (size_t i = 0; i < _events.size(); i++)
        {
            dwOriginalTicks += _events[i].wTimeDelta;
            if ((tempoIndex < tempoChanges.size()) &&
                (dwOriginalTicks >= tempoChanges[tempoIndex].dwTicks))
            {
                // We have a new tempo starting here.
                dwOldTempo = tempoChanges[tempoIndex].dwTempo;
                tempoIndex++;
                dwTotalTicksAtOldTempo = 0;
                dwTotalTicksAtNewTempo = 0;
            }
            DWORD dwTotalTicksAtNewTempoTemp = (dwTotalTicksAtOldTempo + _events[i].wTimeDelta) * dwNewTempo / dwOldTempo;
            ASSERT(dwTotalTicksAtNewTempoTemp >= dwTotalTicksAtOldTempo);
            _events[i].wTimeDelta = dwTotalTicksAtNewTempoTemp - dwTotalTicksAtOldTempo;
            _dwTotalTicks += _events[i].wTimeDelta;
            ASSERT(_events[i].wTimeDelta < 65000); // Just some sanity check for overflows.
            dwTotalTicksAtNewTempo = dwTotalTicksAtNewTempoTemp;
        }
    }
}

bool SoundResource::InitializeFromMidi(const std::string &filename)
{
    std::ifstream midiFile(filename.c_str(), std::ios::in | std::ios::binary);
    if (midiFile.is_open())
    {
        DWORD dw = _ReadBEDWORD(midiFile);
        if (dw == 0x4D546864)
        {
            dw = _ReadBEDWORD(midiFile);
            if (dw == 6)
            {
                WORD wFormat = _ReadBEWORD(midiFile);
                if ((wFormat == 0) || (wFormat == 1))
                {
                    WORD wNumTracks = _ReadBEWORD(midiFile);;
                    _wDivision = _ReadBEWORD(midiFile);
                    ASSERT((_wDivision & 0x8000) == 0); // TODO: support SMPTE

                    // Now we expect tracks.  Each track will consist of an event vector.
                    std::vector<std::vector<SoundEvent> > tracks;
                    // Put in wNumTracks empty tracks:
                    std::vector<SoundEvent> emptyTrack;
                    tracks.insert(tracks.begin(), wNumTracks, emptyTrack);
                    // Read each track
                    std::vector<TempoEntry> tempoChanges;
                    WORD wChannelMask = 0;
                    for (WORD i = 0; i < wNumTracks && midiFile.good(); i++)
                    {
                        wChannelMask |= _ReadMidiFileTrack(i, midiFile, tracks[i], tempoChanges);
                    }

                    _CombineTracks(tracks);
                    _RemoveTempoChanges(tempoChanges);

                    // By default, turn off all channels for all synths except GM
                    // GM is the "new standard" for fan-made games
                    FillMemory(_channels, sizeof(_channels), 0);
                    SetChannelMask(NewGM, wChannelMask);
                }
                else
                {
                    AfxMessageBox("Only type 0 and type 1 midi files are supported.", MB_ERRORFLAGS);
                }
            }
            else
            {
                AfxMessageBox("Wrong chunk size", MB_ERRORFLAGS);
            }
        }
        else
        {
            AfxMessageBox("Not a midi file", MB_ERRORFLAGS);
        }
    }
    _fCanSetTempo = true;
    return true;
}

void _RemoveEvent(size_t index, std::vector<SoundEvent> &events)
{
    if (index + 1 < events.size())
    {
        // First add this event's time to the next:
        events[index + 1].wTimeDelta += events[index].wTimeDelta;
    }
    events.erase(&events[index]);
}

bool predTicks(SoundResource::CuePoint &cue1, SoundResource::CuePoint &cue2)
{
    return (cue1.GetTickPos() <= cue2.GetTickPos());
}

SoundEvent _MakeCueEvent(SoundResource::CuePoint cue, DWORD dwTicksPrior)
{
    SoundEvent event;
    if (cue.GetType() == SoundResource::CuePoint::Cumulative)
    {
        event.SetRawStatus(SoundEvent::Control | 15); // REVIEW: should we OR in 15?
        ASSERT(cue.GetTickPos() >= dwTicksPrior);
        event.bParam1 = 0x60;
        event.bParam2 = cue.GetValue(); // Unclear if there is a limit here...
    }
    else
    {
        event.SetRawStatus(SoundEvent::ProgramChange | 15);
        event.bParam1 = cue.GetValue();
        ASSERT(event.bParam1 < 127);
    }
    event.wTimeDelta = cue.GetTickPos() - dwTicksPrior;
    return event;
}

void SoundResource::_RationalizeCuesAndLoops()
{
    if (_fReEvaluateLoopPoint)
    {
        // First remove any loop points
        for (size_t i = 0; i < _events.size();)
        {
            SoundEvent &event = _events[i];
            if ((event.GetCommand() == SoundEvent::ProgramChange) &&
                (event.GetChannel() == 15) &&
                (event.bParam1 == 127))
            {
                // This is a loop point.  Remove it. Don't increment the loop counter,
                // since the vector is one smaller now.
                _RemoveEvent(i, _events);
            }
            else
            {
                i++;
            }
        }
        if (_dwLoopPoint != SoundResource::LoopPointNone)
        {
            // Now add it back in.
            DWORD dwTotalTicks = 0;
            bool fInserted = false;
            for (size_t i = 0; i < _events.size(); i++)
            {
                SoundEvent &event = _events[i];
                if ((dwTotalTicks + event.wTimeDelta) >= _dwLoopPoint)
                {
                    // The loop point is in between this and the last event.
                    SoundEvent loopEvent;
                    loopEvent.SetRawStatus(SoundEvent::ProgramChange | 15);
                    loopEvent.bParam1 = 127;
                    // Time delta is the looppoint, minus the total ticks to the previous event.
                    loopEvent.wTimeDelta = _dwLoopPoint - dwTotalTicks;
                    if (loopEvent.wTimeDelta)
                    {
                        // Adjust the current event's time delta
                        event.wTimeDelta -= loopEvent.wTimeDelta;
                    }
                    _events.insert(&_events[i], loopEvent);
                    fInserted = true;
                    break; // Break instead of using a loop variable, since insert invalidated 'event'
                }
                dwTotalTicks += event.wTimeDelta; // ... and so otherwise we could crash here.
            }
            ASSERT(fInserted);
        }
    }

    if (_fReEvaluateCues)
    {
        // First remove any cue points
        for (size_t i = 0; i < _events.size();)
        {
            SoundEvent &event = _events[i];
            if (((event.GetCommand() == SoundEvent::ProgramChange) &&
                 (event.GetChannel() == 15) &&
                 (event.bParam1 < 127)) 
                 ||
                ((event.GetCommand() == SoundEvent::Control) &&
                 (event.bParam1 == 0x60)))
            {
                // This is a cue point.  Remove it. Don't increment the loop counter,
                // since the vector is one smaller now.
                _RemoveEvent(i, _events);
            }
            else
            {
                i++;
            }
        }
        // Now add cue points back in.
        // However, first we need to sort our cue point array.
        sort(_cues.begin(), _cues.end(), predTicks);

        DWORD dwTotalTicks = 0;
        std::vector<CuePoint>::iterator cueIt = _cues.begin();
        for (size_t i = 0; (cueIt != _cues.end()) && (i < _events.size()); i++)
        {
            bool fDoneWithCuesHere = false;
            while (!fDoneWithCuesHere)
            {
                fDoneWithCuesHere = true;
                SoundEvent &event = _events[i];
                DWORD wTimeDelta = event.wTimeDelta; // stash this, because event might become invalid.
                if ((dwTotalTicks + event.wTimeDelta) >= cueIt->GetTickPos())
                {
                    // The cue is in between this and the last event.
                    SoundEvent cueEvent = _MakeCueEvent(*cueIt, dwTotalTicks);
                    if (cueEvent.wTimeDelta)
                    {
                        // Adjust the current event's time delta
                        event.wTimeDelta -= cueEvent.wTimeDelta;
                    }
                    _events.insert(&_events[i], cueEvent); // we may have invalidated SoundEvent &event now.
                    ++cueIt;    // Go to the next cue.
                    ++i;        // We just added one to the events, so double-increment this.
                    if (cueIt != _cues.end())
                    {
                        // Perhaps there is another cue point to be inserted in this spot?
                        fDoneWithCuesHere = false;
                    }
                }
                else
                {
                    fDoneWithCuesHere = true;
                }
                dwTotalTicks += wTimeDelta;
            }
        }
        ASSERT(cueIt == _cues.end()); // Otherwise we missed one.  Cues can't be past the end of the ticks, so we should
                                      // have used all of them.
    }
    _fReEvaluateCues = false;
    _fReEvaluateLoopPoint = false;

    if ((_wTempoIfChanged != StandardTempo) || (_wDivision != SCI_PPQN))
    {
        _NormalizeToSCITempo();
        _fCanSetTempo = false;
    }
}

BOOL SoundResource::Serialize(sci::istream *pSerial) const
{
    BOOL fRet = TRUE;

    // Special crazy case for sound resources.
    if (_fReEvaluateLoopPoint || _fReEvaluateCues || ((_wTempoIfChanged != StandardTempo) || (_wDivision != SCI_PPQN)))
    {
        // Make a copy of ourselves.
        SoundResource sound = *this;
        sound._RationalizeCuesAndLoops();
        // And serialize that instead.
        fRet = sound.Serialize(pSerial);
    }
    else
    {
        pSerial->WriteByte(0); // Digital sample -> no

        // Channel information:
        for (size_t i = 0; i < ARRAYSIZE(_channels); i++)
        {
            pSerial->WriteWord(_channels[i]);
        }
        ASSERT(ARRAYSIZE(_channels) == 15);
        pSerial->WriteWord(0); // Digital sample offset - we don't care about this for SCI0.
        
        // Now the events.
        BYTE bLastStatus = 0;
        DWORD dwAddToLastTimeDelta = 0;
        // Don't allow more than about 64K
        for (size_t i = 0; i < _events.size() && (pSerial->GetDataSize() < 0x0000fe00); i++)
        {
            const SoundEvent &event = _events[i];
            if ((event.GetCommand() == SoundEvent::Special) && (event.GetRawStatus() != 0xFC))  // 0xFC = stop bit.
            {
                // Ignore this event except for the time.
                dwAddToLastTimeDelta += event.wTimeDelta;
            }
            else
            {
                DWORD dwTimeDelta = event.wTimeDelta + dwAddToLastTimeDelta;
                // First the time delta...
                while (dwTimeDelta >= 240)
                {
                    pSerial->WriteByte(0xf8);
                    dwTimeDelta -= 240;
                }
                pSerial->WriteByte((BYTE)dwTimeDelta);

                // Then the status byte if it's different from the previous one.
                BYTE bCurrentStatus = event.GetRawStatus();
                if (bLastStatus != bCurrentStatus)
                {
                    pSerial->WriteByte(bCurrentStatus);
                    bLastStatus = bCurrentStatus;
                }

                switch (event.GetCommand())
                {
                case SoundEvent::NoteOff:
                case SoundEvent::NoteOn:
                case SoundEvent::KeyPressure:
                case SoundEvent::Control:
                case SoundEvent::PitchWheel:
                    pSerial->WriteByte(event.bParam1);
                    pSerial->WriteByte(event.bParam2);
                    break;
                case SoundEvent::ProgramChange:
                case SoundEvent::Pressure:
                    pSerial->WriteByte(event.bParam1);
                    break;
                case SoundEvent::Special:
                    ASSERT(event.GetRawStatus() == 0xFC);
                    // no params
                    break;
                default:
                    ASSERT(FALSE);
                    break;
                }
                dwAddToLastTimeDelta = 0;
            }
        }
        if (bLastStatus != 0xFC)
        {
            // Stick a stop status byte in if there wasn't one already.
            pSerial->WriteByte(0); // time delta
            pSerial->WriteByte(0xFC);
        }
    }
    return fRet;
}

const struct
{
    SoundResource::DeviceType type;
    const char *pszName;
}
c_rgDeviceNames[] = 
{
    { SoundResource::RolandMT32, "Roland MT32 ($01)" },
    { SoundResource::YamahaFB01, "Yamaha FB-01 ($02)" },
    { SoundResource::Adlib, "Adlib ($04)" },
    { SoundResource::Casio, "Casio  ($08)" },
    { SoundResource::Tandy1000, "Tandy 1000 ($10)" },
    { SoundResource::PCSpeaker, "PC Speaker ($20)" },
    { SoundResource::DevUnknown, "Unknown ($40)" },
    { SoundResource::NewGM, "GM ($80)" },
};

void PopulateComboWithDevicesHelper(CComboBox &combo)
{
    for (int i = 0; i < ARRAYSIZE(c_rgDeviceNames); i++)
    {
        combo.InsertString(-1, c_rgDeviceNames[i].pszName);
    }
}

SoundResource::DeviceType GetDeviceFromComboHelper(CComboBox &combo)
{
    return c_rgDeviceNames[combo.GetCurSel()].type;
}

void SelectDeviceInComboHelper(CComboBox &combo, SoundResource::DeviceType type)
{
    for (int i = 0; i < ARRAYSIZE(c_rgDeviceNames); i++)
    {
        if (c_rgDeviceNames[i].type == type)
        {
            combo.SetCurSel(i);
            return;
        }
    }
}

void SoundResource::AddCuePoint(CuePoint cue)
{
    _cues.push_back(cue);
}

void SoundResource::DeleteCue(size_t index)
{
    if (index < _cues.size())
    {
        _cues.erase(&_cues[index]);
        _fReEvaluateCues = true;
    }
}

void SoundResource::SetCue(size_t index, CuePoint cue)
{
    if (index < _cues.size())
    {
        _cues[index] = cue;
        _fReEvaluateCues = true;
    }
}

void SoundResource::SetLoopPoint(DWORD dwTicks)
{
    if (_dwLoopPoint != dwTicks)
    {
        _dwLoopPoint = dwTicks ? dwTicks : LoopPointNone;
        _fReEvaluateLoopPoint = true;
    }
}


//
// Midi helper
//
MidiPlayer::MidiPlayer()
{
    _handle = NULL;
    ZeroMemory(&_midiHdr, sizeof(_midiHdr));
    _pRealData = NULL;
    _device = SoundResource::RolandMT32;
    _fPlaying = false;
    _wTotalTime = 0;
    _wTimeDivision = SCI_PPQN;
    _wTempo = 120;
    _fStoppingStream = false;
    _dwCookie = 0;
}

MidiPlayer::~MidiPlayer()
{
    _ClearHeaders();
    if (_handle)
    {
        midiStreamClose(_handle);
    }
}

void PrintMidiOutErrorMsg(unsigned long err)
{
#define BUFFERSIZE 120
	char	buffer[BUFFERSIZE];
	char msg[200];
	if (!(err = midiOutGetErrorText(err, &buffer[0], BUFFERSIZE)))
	{
		StringCchPrintf(msg, ARRAYSIZE(msg), "%s\r\n", &buffer[0]);
        OutputDebugString(msg);
	}
	else if (err == MMSYSERR_BADERRNUM)
	{
		OutputDebugString("Strange error number returned!\r\n");
	}
	else
	{
		OutputDebugString("Specified pointer is invalid!\r\n");
	}
}

void MidiPlayer::_SetTempoAndDivision()
{
    if (_handle)
    {
        if (!_fPlaying)
        {
            MIDIPROPTIMEDIV prop;
	        /* Set the stream device's Time Division */
	        prop.cbStruct = sizeof(MIDIPROPTIMEDIV);
            prop.dwTimeDiv = _wTimeDivision;
	        unsigned long err = midiStreamProperty(_handle, (LPBYTE)&prop, MIDIPROP_SET|MIDIPROP_TIMEDIV);
            if (err)
            {
                // We failed
                OutputDebugString("An error setting time division!\n");
            }
        } // We can't set this while we're playing.

        // But tempo is no problem...
        MIDIPROPTEMPO   tempo;
        tempo.cbStruct = sizeof(MIDIPROPTEMPO);
        tempo.dwTempo = 60000000 / _wTempo; // 60 million microseconds / bpm
        unsigned long err = midiStreamProperty(_handle, (LPBYTE)&tempo, MIDIPROP_SET|MIDIPROP_TEMPO);
        if (err)
        {
            OutputDebugString("An error setting the tempo!\n");
        }
    }
}

bool MidiPlayer::_Init()
{
    if (_handle == NULL)
    {
        UINT streamno = 0;
        if (midiStreamOpen(&_handle, &streamno, 1, reinterpret_cast<DWORD_PTR>(s_MidiOutProc), reinterpret_cast<DWORD_PTR>(this), CALLBACK_FUNCTION))
        {
            OutputDebugString("Failed to open midi stream\n");
            ASSERT(_handle == NULL);
        }
    }
    return (_handle != NULL);
}

DWORD MidiPlayer::QueryPosition(DWORD scope)
{
    DWORD pos = 0;
    if (_handle)
    {
        if (_fPlaying)
        {
            MMTIME time;
            time.wType = TIME_TICKS;
            if (!midiStreamPosition(_handle, &time, sizeof(time)))
            {
                // ticks ellapsed on current 64k chunk
                // plus the tick count when we started
                _dwCurrentTickPos = time.u.ticks + _dwCurrentChunkTickStart;
                pos = _dwCurrentTickPos * scope / _wTotalTime;   
            }
        }
        else
        {
            pos = _dwCurrentTickPos * scope / _wTotalTime;
        }
    }
    return pos;
}

DWORD MidiPlayer::QueryPosition()
{
    return QueryPosition(_wTotalTime);
}

// 3 DWORDs per event, approx 64k (a little less)
#define MAX_STREAM_EVENTS (65532 / (3 * sizeof(DWORD)))

DWORD MidiPlayer::SetSound(const SoundResource &sound, WORD wInitialTempo)
{
    _ClearHeaders();
    _wTimeDivision = sound.GetTimeDivision();
    _dwLoopPoint = sound.GetLoopPoint();
    if (_Init())
    {
        // Set up the stream.
        // Put new data in.
        const std::vector<SoundEvent> &events = sound.GetEvents();
        // Accumulated ticks will need to be at most this big:
        _accumulatedStreamTicks.resize(events.size());
        DWORD cbSize = (DWORD)(events.size() * 3) * sizeof(DWORD);
        unsigned long *stuff = new unsigned long[cbSize / sizeof(DWORD)];
        _pRealData = stuff;

        DWORD cbEventsUsed = 0;
        DWORD wExtraTime = 0;
        _wTotalTime = 0;
        WORD wChannelMask = sound.GetChannelMask(_device);
        DWORD dwAccumulatedTicks = 0;
        for (size_t i = 0; i < events.size(); ++i)
        {
            // Only add the event if it is for a channel we're using.
            if ((0x1 << events[i].GetChannel()) & wChannelMask)
            {
                cbEventsUsed++;
                DWORD dwThisTimeDelta = events[i].wTimeDelta + wExtraTime;
                *stuff++ = dwThisTimeDelta;
                dwAccumulatedTicks += dwThisTimeDelta;
                _accumulatedStreamTicks[cbEventsUsed] = dwAccumulatedTicks;
                *stuff++ = 0;
                *stuff++ = (MEVT_SHORTMSG << 24) | events[i].GetRawStatus() | (((DWORD)events[i].bParam1) << 8) | (((DWORD)events[i].bParam2) << 16);
                wExtraTime = 0;
            }
            else
            {
                // But take into account the time delta:
                wExtraTime += events[i].wTimeDelta;
            }
            _wTotalTime += events[i].wTimeDelta; // Not quite right - there could be empty space at the end.
        }
        
        SetTempo(wInitialTempo);
        _cTotalStreamEvents = cbEventsUsed;
        _CuePosition(0);
    }
    return ++_dwCookie;
}

void MidiPlayer::Play()
{
    unsigned int err = midiStreamRestart(_handle);
    if (!err)
    {
        _fPlaying = true;
    }
    else
    {
        PrintMidiOutErrorMsg(err);
    }
}

void MidiPlayer::Pause()
{
    unsigned int err = midiStreamPause(_handle);
    if (!err)
    {
        _fPlaying = false;
    }
    else
    {
        PrintMidiOutErrorMsg(err);
    }
}

void MidiPlayer::Stop()
{
    Pause();
    // Return to the beginning.
    _CuePosition(0);
}

bool MidiPlayer::CanPlay()
{
    return (!_fPlaying);
}
bool MidiPlayer::CanPause()
{
    return (_fPlaying);
}
bool MidiPlayer::CanStop()
{
    return QueryPosition(3000) != 0;
}


void MidiPlayer::_ClearHeaders()
{
    if (_handle)
    {
        // Stop and unhook the old data
        if (_pRealData)
        {
            _fStoppingStream = true;
            midiOutReset((HMIDIOUT)_handle);
            _fStoppingStream = false;

            // Unprepare it
            if (_fQueuedUp)
            {
                midiOutUnprepareHeader((HMIDIOUT)_handle, &_midiHdr, sizeof(_midiHdr));
                _fQueuedUp = false;
            }

            delete [] _pRealData;
            _midiHdr.lpData = NULL;
            _pRealData = NULL;
        }
    }
}

//
// Queues the player to a particular "percentage"
//
void MidiPlayer::CuePosition(DWORD dwTicks, DWORD scope)
{
    dwTicks = min(scope, dwTicks);
    dwTicks = _wTotalTime * dwTicks / scope;
    //dwTicks = _accumulatedStreamTicks[_cTotalStreamEvents - 1] * dwTicks / scope;
    // Find the spot to cue.
    size_t i = 0;
    for (; i < _accumulatedStreamTicks.size(); i++)
    {
        if (_accumulatedStreamTicks[i] >= dwTicks)
        {
            break;
        }
    }
    if (i >= _accumulatedStreamTicks.size())
    {
        i = 0;
    }
    _CuePosition((DWORD)i);
}
void MidiPlayer::CueTickPosition(DWORD dwTicks)
{
    // Find the spot to cue.
    size_t i = 0;
    for (; i < _accumulatedStreamTicks.size(); i++)
    {
        if (_accumulatedStreamTicks[i] >= dwTicks)
        {
            break;
        }
    }
    if (i >= _accumulatedStreamTicks.size())
    {
        i = 0;
    }
    _CuePosition((DWORD)i);
}

//
// Queues the player to a particular index in the events.
//
void MidiPlayer::_CuePosition(DWORD dwEventIndex)
{
    if (_pRealData)
    {
        if (_fQueuedUp)
        {
            _fStoppingStream = true;
            midiOutReset((HMIDIOUT)_handle);
            _fStoppingStream = false;
            midiOutUnprepareHeader((HMIDIOUT)_handle, &_midiHdr, sizeof(_midiHdr));
            _fQueuedUp = false;
        }

        ZeroMemory(&_midiHdr, sizeof(_midiHdr));
        _midiHdr.lpData = (LPSTR)(&_pRealData[dwEventIndex * 3]);
        // How long is it?
        DWORD cEvents;
        if (_cTotalStreamEvents > (dwEventIndex + MAX_STREAM_EVENTS))
        {
            // We can't play all
            _cRemainingStreamEvents = _cTotalStreamEvents - (dwEventIndex + MAX_STREAM_EVENTS);
            cEvents = MAX_STREAM_EVENTS;
        }
        else
        {
            _cRemainingStreamEvents = 0;
            cEvents = _cTotalStreamEvents - dwEventIndex;
        }
        _dwCurrentChunkTickStart = _accumulatedStreamTicks[dwEventIndex];
        _dwCurrentTickPos = _dwCurrentChunkTickStart;
        _midiHdr.dwBufferLength = _midiHdr.dwBytesRecorded = (cEvents * 3 * sizeof(DWORD));
        _midiHdr.dwFlags = 0;
		unsigned long err = midiOutPrepareHeader((HMIDIOUT)_handle,  &_midiHdr, sizeof(MIDIHDR));
	    if (!err)
		{
            _fQueuedUp = true; // assuming midiStreamOut works
	        /* Queue the Stream of messages. Output doesn't actually start
			until we later call midiStreamRestart().
			*/
			err = midiStreamOut(_handle, &_midiHdr, sizeof(MIDIHDR));
            if (!err)
            {
            }
            else
            {
                PrintMidiOutErrorMsg(err);
            }
        }
        else
        {
            PrintMidiOutErrorMsg(err);
        }
        if (!err && _fPlaying)
        {
            // We're good, so play it now
            err = midiStreamRestart(_handle);
            if (err)
            {
                PrintMidiOutErrorMsg(err);
            }
        }
    }
}

void MidiPlayer::_OnStreamDone()
{
    if (!_fStoppingStream)
    {
        // There is more to this stream... keep going.
        if (_cRemainingStreamEvents)
        {
            _CuePosition(_cTotalStreamEvents - _cRemainingStreamEvents);
        }
        else
        {
            if (_dwLoopPoint != SoundResource::LoopPointNone)
            {
                // Cue the loop point.
                CueTickPosition(_dwLoopPoint);
            }
        }
    }
}

void CALLBACK MidiPlayer::s_MidiOutProc(HMIDIOUT hmo, UINT wMsg, DWORD_PTR dwInstance,  DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    if (wMsg == MM_MOM_DONE)
    {
        (reinterpret_cast<MidiPlayer*>(dwInstance))->_OnStreamDone();
    }
}

MidiPlayer g_midiPlayer;
