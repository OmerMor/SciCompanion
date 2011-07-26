//
// Pic Command classes
//
#pragma once


struct PenStyle
{
    PenStyle() : bPatternSize(0), bPatternNR(0), fRandomNR(0), fRectangle(0), fPattern(0) {}
    BOOL fPattern;
    BOOL fRectangle;
    BOOL fRandomNR;
    BYTE bPatternSize;
    BYTE bPatternNR;
};

void InitPenStyle(PenStyle *pPenStyle);
BOOL IsPenStyleEqual(const PenStyle *pPenStyle1, const PenStyle *pPenStyle2);
void PatternInfoFromIndex(BYTE bIndex, PenStyle *pPenStyle);
BYTE IndexFromPatternInfo(const PenStyle *pPenStyle);

