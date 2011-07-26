#pragma once

//
// This contains definitions for classes and functions related to resources that have cels
// (e.g. views and fonts)
//

class __declspec(uuid("0fc8b1be-e8fb-4ab6-bbbb-4034b10731cf")) ICelResource
{
public:
    virtual int GetCelCount(int nLoop = 0) const = 0;
};


class __declspec(uuid("2146e69b-55a6-4d09-82a3-cfaa309c130d")) ICelResourceDoc
{
public:
    virtual int GetSelectedCel() const = 0;
    virtual void SetSelectedCel(int nCel) = 0;
    virtual void InsertCel(int nLoop, int nCel, BOOL fBefore) = 0;
    virtual void GetLabelString(PTSTR pszLabel, size_t cch, int nCel) const = 0;
};


class CCelDescription : public CObject
{
public:
    CCelDescription(int nLoop, int nCel) { _nLoop = nLoop; _nCel = nCel; }
    CCelDescription(DWORD dwIndex)
    {
        _nLoop = DWORD_TO_LOOP(dwIndex);
        _nCel = DWORD_TO_CEL(dwIndex);
    }

    int GetCel() { return _nCel; }
    int GetLoop() { return _nLoop; }
    BOOL Equals(int nLoop, int nCel) { return (nLoop == _nLoop) && (nCel == _nCel);}

private:
    int _nLoop;
    int _nCel;
};


