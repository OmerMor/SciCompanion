#pragma once

#include "interfaces.h"

//
// This can represent any vocab resource with names
//
class CVocabWithNames : public ILookupNames
{
public:
    CVocabWithNames() { _fDirty = false; }
    virtual std::string Lookup(WORD wName) const;
    virtual bool ReverseLookup(std::string name, WORD &wIndex) const;
    const std::vector<std::string> &GetNames() const { return _names; }
    WORD Add(const std::string &str);
    bool Create(sci::istream *pStream, bool fTruncationOk = false) { return _Create(*pStream, fTruncationOk); }

protected:
    bool _Create(sci::istream &byteStream, bool fTruncationOk = false);
    bool _IsDirty() { return _fDirty; }
    virtual std::string _GetMissingName(WORD wName) const { return ""; }

private:
    std::vector<std::string> _names;
    bool _fDirty;
};

//
// Selector names
//
class SelectorTable : public CVocabWithNames
{
public:
    bool Load();
    void Save();
};

//
// Kernel names
//
class KernelTable : public CVocabWithNames
{
public:
    bool Load();
    virtual std::string Lookup(WORD wName) const;
    virtual bool ReverseLookup(std::string name, WORD &wIndex) const;

protected:
    std::string _GetMissingName(WORD wName) const;
};

// fwd decl
class CompiledScript;

//
// Given a "class #", returns the script that it is in.
// The global class table is used for disassembling scripts.
// It is based off the same resource as the species table,
// except that it also groks the script resources and pulls
// in the class names.
//
class GlobalClassTable : public ILookupNames
{
public:
    bool Load();
    WORD GetScriptNum(WORD wClass) { return _scriptNums[wClass]; }
    const std::vector<WORD> &GetScriptNums() { return _scriptNums; } // REVIEW: remove this

    // ILookupNames
    std::string Lookup(WORD wIndex) const;
    bool GetSpeciesPropertySelector(WORD wSpeciesIndex, std::vector<WORD> &props, std::vector<WORD> &values)
    {
        size_t numSpecies = _speciesSelectors.size();
        bool fRet = (wSpeciesIndex < _speciesSelectors.size());
        if (fRet)
        {
            props = _speciesSelectors[wSpeciesIndex];
            values = _speciesSelectorPropertyValues[wSpeciesIndex];
        }
        return fRet;
    } 

    ~GlobalClassTable();

private:
    bool _Create(sci::istream &byteStream);

    std::vector<WORD> _scriptNums;
    std::vector<std::string> _classNames;
    std::vector<std::vector<WORD> > _speciesSelectors;
    std::vector<std::vector<WORD> > _speciesSelectorPropertyValues;
    std::vector<CompiledScript*> _scripts;
};

//
// Maps species index to a particular script (and index within that script)
//
class SpeciesTable
{
public:
    SpeciesTable() { _wNewSpeciesIndex = 0; _fDirty = false; }
    bool Load();
    void Save();
    bool GetSpeciesIndex(WORD wScript, WORD wClassIndexInScript, SpeciesIndex &wSpeciesIndex) const;
    bool GetSpeciesLocation(SpeciesIndex wSpeciesIndex, WORD &wScript, WORD &wClassIndexInScript) const;
    SpeciesIndex MaybeAddSpeciesIndex(WORD wScript, WORD wClassIndexInScript);
    std::vector<std::string> GetNames() const;

private:
    bool _Create(sci::istream &byteStream);

    typedef std::map<WORD, std::vector<WORD> > species_map;

    // Fast lookup of wScript + wScriptIndex -> wSpeciesIndex
    species_map _map;
    // Data in its basic form, as stored (e.g. a list of scripts)
    std::vector<WORD> _direct;
    WORD _wNewSpeciesIndex;
    bool _fDirty;
};
