//
// PicResource implements a runtime version of a pic resource.
// It contains state (current drawing position), in addition to the bitmaps for each of the
// 4 screens.
// It contains the list of drawing commands, and the current index into those commands.
//
// There is no intermediate data object between the generic ResourceBlob and this PicResource.
//

#pragma once

// FWD declaration
class ResourceBlob;

class PicResource : public IResourceBase
{
public:
    PicResource();
    ~PicResource();

    bool IsSame(const PicResource *pPic) const { return pPic && (_id == pPic->_id); }

    // this for existing ones.
    HRESULT InitFromResource(const ResourceBlob *prd);
    // This for new ones
    HRESULT InitNew();

    HBITMAP GetVisualBitmap(int cx, int cy) const;
    HBITMAP GetPriorityBitmap(int cx, int cy) const;
    HBITMAP GetControlBitmap(int cx, int cy) const;
    INT_PTR GetMemorySize() const;
    BITMAPINFO *GetSCIBMI() const; // Caller must delete

    // Insert/remove commands from pic
    void InsertCommands(INT_PTR iStart, INT_PTR cCount, PicCommand *pCommands);
    void RemoveCommand(INT_PTR iCommandIndex);
    void RemoveCommandRange(INT_PTR iStart, INT_PTR iEnd);
    HRESULT CopyRangeToClipboard(INT_PTR iStart, INT_PTR iEnd) const;

    // IResourceBase
    BOOL Serialize(sci::istream *pSerial) const;
    void SetResourceNumber(int iNum) { _iResourceNumber = iNum; }
    void SetPackageNumber(int iNum) { _iFileNumber = iNum; }
    int GetResourceNumber() const { return _iResourceNumber; }
    int GetPackageNumber() const { return _iFileNumber; }
    ResourceType GetType() const { return RS_PIC; }
    BOOL PerformChecks() const;

    std::string GetTitle() const { return _strTitle; }
    void ResetResourceAndFileNumbers() { _iFileNumber = -1; _iResourceNumber = -1; }
    INT_PTR GetCommandCount() const { return _picCommands.size(); }
    const PicCommand &GetCommandAt(INT_PTR iIndex) const;

    // Interesting operations.
    void Draw(PicData &data, SCIPicState &state, INT_PTR iPos) const;
    void UpdatePicState(SCIPicState &state, INT_PTR iDrawPos) const;
    INT_PTR GetLastChangedSpot(PicData &data, SCIPicState &state, int x, int y) const;

private:
    struct PatternInfo
    {
        PatternInfo() : bPatternNR(0), bPatternCode(0), bPatternSize(0) {}
        BYTE bPatternNR;
        BYTE bPatternCode;
        BYTE bPatternSize;
    };

    // Methods for reading pic resource data
    void _ReadRelativePatterns(sci::istream &data, PatternInfo &patterns);
    void _ReadAbsolutePatterns(sci::istream &stream, PatternInfo &patterns);
    void _ReadRelativeMediumPatterns(sci::istream &stream, PatternInfo &patterns);
    void _ReadRelativeMediumLines(sci::istream &data);
    void _ReadRelativeLongLines(sci::istream &stream);
    void _ReadRelativeShortLines(sci::istream &stream);
    void _ReadFill(sci::istream &stream);
    void _ReadSetPalette(sci::istream &data);
    void _ReadSetPaletteEntry(sci::istream &stream);
    /*
    HRESULT _ReadSetVisualSCI1(CPicByteStream &data);       // SCI1
    HRESULT _ReadSetPriorityBars(CPicByteStream &data);     // SCI1
    HRESULT _ReadSetPaletteSCI1(CPicByteStream &data);      // SCI1
    HRESULT _ReadDrawVisualBitmap(CPicByteStream &data);    // SCI1
    */
            
    void _InsertCommands(INT_PTR iStart, INT_PTR cCount, PicCommand *pCommands);

    int _id;
    std::vector<PicCommand> _picCommands;

    // Information about the file itself
    int _iResourceNumber;
    int _iFileNumber;
    std::string _strTitle;
    bool _f256Color;
};

