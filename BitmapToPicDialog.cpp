// BitmapToPicDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "BitmapToPicDialog.h"
#include "PicResource.h"
#include "BitmapToPicDialog.h"
#include "PicDrawManager.h"
#include "PicDoc.h"

#define FROMCLIPBOARD_TIMER 2345

int g_iAlgorithm = 1;   // RGB euclidian
int g_iPalette = 3;     // Smooth half-tone
int g_iScaleImage = 1;
int g_iIgnoreWhite = 0;
int g_nColors = 10;

struct THREADINFO
{
    HWND hwndDlg;
    int iAlgorithm;
    int iPalette;
    BOOL fIgnoreWhite;
    int nColors;
    COLORREF *pCRBitmap;
    SIZE size;
    HANDLE hEvent;
    EGACOLOR picPalette[40];
};

struct THREADRESPONSE
{
    CArray<PicCommand, PicCommand> *pcommands;
    HBITMAP hbm;
    BOOL fTooBig;

    THREADRESPONSE() { hbm = NULL; }
    ~THREADRESPONSE() { DeleteObject(hbm); }
};

void FreeThreadInfo(THREADINFO *pInfo)
{
    if (pInfo->pCRBitmap)
    {
        delete [] pInfo->pCRBitmap;
        pInfo->pCRBitmap = NULL;
    }
    if (pInfo->hEvent)
    {
        CloseHandle(pInfo->hEvent);
        pInfo->hEvent = NULL;
    }
}

BOOL InitThreadInfo(THREADINFO *pInfo, HWND hwnd, BOOL fIgnoreWhite, int iAlgorithm, int iPalette, int nColors, CSize &size, const COLORREF *pCRBitmap, EGACOLOR *pPicPalette)
{
    BOOL fRet = FALSE;
    ZeroMemory(pInfo, sizeof(*pInfo));
    pInfo->hwndDlg = hwnd;
    pInfo->iAlgorithm = iAlgorithm;
    pInfo->iPalette = iPalette;
    pInfo->nColors = nColors;
    pInfo->pCRBitmap = new COLORREF[size.cx * size.cy];
    pInfo->size = size;
    pInfo->fIgnoreWhite = fIgnoreWhite;
    if (pInfo->pCRBitmap)
    {
        // Bitmap data
        CopyMemory(pInfo->pCRBitmap, pCRBitmap, size.cx * size.cy * sizeof(*pCRBitmap));
        // Optional palette.
        CopyMemory(pInfo->picPalette, pPicPalette, sizeof(pInfo->picPalette));
        pInfo->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (pInfo->hEvent)
        {
            fRet = TRUE;
        }
        else
        {
            FreeThreadInfo(pInfo);
        }
    }
    return fRet;
}



// CBitmapToPicDialog dialog

#define UWM_PICREADY        (WM_APP + 0)
#define UWM_CONVERTPROGRESS (WM_APP + 1) // LPARAM is int from 0 to 100
#define UWM_CONVERTSTATUS   (WM_APP + 2) // LPARAM is text

CBitmapToPicDialog::CBitmapToPicDialog(CWnd* pParent /*=NULL*/)
	: CExtNCW<CExtResizableDialog>(CBitmapToPicDialog::IDD, pParent)
{
    _pCRBitmap = NULL;
    _nColors = g_nColors;
    _iAlgorithm = g_iAlgorithm;
    _iPalette = g_iPalette;
    _iScaleImage = g_iScaleImage;
    _iIgnoreWhite = g_iIgnoreWhite;
    _size.SetSize(0, 0);

    _pThread = NULL;
    _hEvent = NULL;
    _fConverting = FALSE;
    _pcommands = NULL;
    _fTooBig = FALSE;

    _pbmpNormal = NULL;
    _pbmpScaled = NULL;
    _pbmpCurrent = NULL;
    _bContrastCenterNormal = 0x80;
    _bContrastCenterScaled = 0x80;

    _nBrightness = 50;
    _nContrast = 50;
    _nSaturation = 50;

    _fInitializedControls = false;
}

CBitmapToPicDialog::~CBitmapToPicDialog()
{
    delete [] _pCRBitmap;
    _DeleteCommands();
    delete _pbmpNormal;
    delete _pbmpScaled;
    delete _pbmpCurrent;

    g_nColors = _nColors;
    g_iAlgorithm = _iAlgorithm;
    g_iPalette = _iPalette;
    g_iScaleImage = _iScaleImage;
    g_iIgnoreWhite = _iIgnoreWhite;
}

void CBitmapToPicDialog::_SetBitmap(HBITMAP hbmp)
{
    HBITMAP hOld = m_wndPic.SetBitmap(hbmp);
    if (hOld)
    {
        DeleteObject(hOld);
    }
}

void CBitmapToPicDialog::DoDataExchange(CDataExchange* pDX)
{
    int nBrightness = _nBrightness;
    int nContrast = _nContrast;
    int nSaturation = _nSaturation;
    int iScale = _iScaleImage;

	CExtNCW<CExtResizableDialog>::DoDataExchange(pDX);

    if( m_wndSlider1.GetSafeHwnd() != NULL )
    {
        m_wndSlider1.UpdateSliderWnd();
    }
    if (!_fInitializedControls)
    {
        // Necessary for prof-uis
        DDX_Control(pDX, IDC_SLIDERNUMCOLORS, m_wndSlider1);
        m_wndSlider1.SetStyle(CExtSliderWnd::e_style_t::ES_PROFUIS);
        DDX_Control(pDX, IDC_SLIDERBRIGHTNESS, m_wndSlider2);
        m_wndSlider2.SetStyle(CExtSliderWnd::e_style_t::ES_PROFUIS);
        DDX_Control(pDX, IDC_SLIDERCONTRAST, m_wndSlider3);
        m_wndSlider3.SetStyle(CExtSliderWnd::e_style_t::ES_PROFUIS);
        DDX_Control(pDX, IDC_SLIDERSATURATION, m_wndSlider4);
        m_wndSlider4.SetStyle(CExtSliderWnd::e_style_t::ES_PROFUIS);
        DDX_Control(pDX, IDC_BUTTONCONVERT, m_wndButton1);
        DDX_Control(pDX, IDC_BUTTONBROWSE, m_wndButton2);
        DDX_Control(pDX, IDOK, m_wndButton3);
        DDX_Control(pDX, IDCANCEL, m_wndButton4);
        DDX_Control(pDX, IDC_RADIO1, m_wndRadio1);
        DDX_Control(pDX, IDC_RADIO2, m_wndRadio2);
        DDX_Control(pDX, IDC_RADIO3, m_wndRadio3);
        DDX_Control(pDX, IDC_RADIO4, m_wndRadio4);
        DDX_Control(pDX, IDC_RADIO5, m_wndRadio5);
        DDX_Control(pDX, IDC_RADIOHALFTONE, m_wndRadio6);
        DDX_Control(pDX, IDC_RADIOSOLID, m_wndRadio7);
        DDX_Control(pDX, IDC_RADIOHALFTONESMOOTH, m_wndRadio8);
        DDX_Control(pDX, IDC_RADIOPICPALETTE, m_wndRadio9);
        DDX_Control(pDX, IDC_STATICGROUP1, m_wndGroup1);
        DDX_Control(pDX, IDC_STATICGROUP2, m_wndGroup2);
        DDX_Control(pDX, IDC_STATICGROUP3, m_wndGroup3);
        DDX_Control(pDX, IDC_STATICGROUP4, m_wndGroup4);
        DDX_Control(pDX, IDC_CHECKSCALE, m_wndCheck1);
        DDX_Control(pDX, IDC_CHECKIGNOREWHITE, m_wndCheck2);
        DDX_Control(pDX, IDC_STATIC1, m_wndLabel1);
        DDX_Control(pDX, IDC_STATIC2, m_wndLabel2);
        DDX_Control(pDX, IDC_STATIC3, m_wndLabel3);
        _fInitializedControls = true;
    }

    DDX_Control(pDX, IDC_EDITSTATUS, m_wndEditStatus);
    m_wndEditStatus.FmtLines(TRUE);
    DDX_Control(pDX, IDC_STATICPIC, m_wndPic);
    DDX_Control(pDX, IDC_STATICORIG, m_wndOrig);
    DDX_Control(pDX, IDC_BUTTONCLIPBOARD, m_wndFromClipboard);
    m_wndFromClipboard.EnableWindow(IsClipboardFormatAvailable(CF_BITMAP));
    DDX_Control(pDX, IDC_PROGRESSCONVERT, m_wndProgress);
    DDX_Check(pDX, IDC_CHECKSCALE, _iScaleImage);

    DDX_Check(pDX, IDC_CHECKIGNOREWHITE, _iIgnoreWhite);

    // MinMaxSlider busted in MFC 7.0
    // DDV_MinMaxSlider(pDX, _nColors, 1, 40); 
    GetDlgItem(IDC_SLIDERNUMCOLORS)->SendMessage(TBM_SETRANGEMIN, FALSE, (LPARAM) 1);
    GetDlgItem(IDC_SLIDERNUMCOLORS)->SendMessage(TBM_SETRANGEMAX, TRUE, (LPARAM) PALETTE_SIZE);
    GetDlgItem(IDC_SLIDERNUMCOLORS)->SendMessage(TBM_SETTICFREQ, 10, 0);
    DDX_Slider(pDX, IDC_SLIDERNUMCOLORS, _nColors);

    GetDlgItem(IDC_SLIDERBRIGHTNESS)->SendMessage(TBM_SETRANGEMIN, FALSE, (LPARAM) 0);
    GetDlgItem(IDC_SLIDERBRIGHTNESS)->SendMessage(TBM_SETRANGEMAX, TRUE, (LPARAM) 100);
    GetDlgItem(IDC_SLIDERBRIGHTNESS)->SendMessage(TBM_SETTICFREQ, 10, 0);
    DDX_Slider(pDX, IDC_SLIDERBRIGHTNESS, _nBrightness);

    GetDlgItem(IDC_SLIDERCONTRAST)->SendMessage(TBM_SETRANGEMIN, FALSE, (LPARAM) 0);
    GetDlgItem(IDC_SLIDERCONTRAST)->SendMessage(TBM_SETRANGEMAX, TRUE, (LPARAM) 100);
    GetDlgItem(IDC_SLIDERCONTRAST)->SendMessage(TBM_SETTICFREQ, 10, 0);
    DDX_Slider(pDX, IDC_SLIDERCONTRAST, _nContrast);

    GetDlgItem(IDC_SLIDERSATURATION)->SendMessage(TBM_SETRANGEMIN, FALSE, (LPARAM) 0);
    GetDlgItem(IDC_SLIDERSATURATION)->SendMessage(TBM_SETRANGEMAX, TRUE, (LPARAM) 100);
    GetDlgItem(IDC_SLIDERSATURATION)->SendMessage(TBM_SETTICFREQ, 10, 0);
    DDX_Slider(pDX, IDC_SLIDERSATURATION, _nSaturation);

    // Enable the convert button if we have a bitmap
    ((CButton*)GetDlgItem(IDC_BUTTONCONVERT))->EnableWindow(_pbmpCurrent != NULL);

    int nID;
    switch(_iAlgorithm)
    {
    case 1:
        nID = IDC_RADIO1;
        break;
    case 2:
        nID = IDC_RADIO2;
        break;
    case 3:
        nID = IDC_RADIO3;
        break;
    case 4:
        nID = IDC_RADIO4;
        break;
    case 5:
        nID = IDC_RADIO5;
        break;
    default:
        ASSERT(FALSE);
    }
    ((CButton*)GetDlgItem(nID))->SetCheck(1);

    switch(_iPalette)
    {
    case 1:
        nID = IDC_RADIOHALFTONE;
        break;
    case 2:
        nID = IDC_RADIOSOLID;
        break;
    case 3:
        nID = IDC_RADIOHALFTONESMOOTH;
        break;
    case 4:
        nID = IDC_RADIOPICPALETTE;
        break;
    default:
        ASSERT(FALSE);
    }
    ((CButton*)GetDlgItem(nID))->SetCheck(1);

    m_wndProgress.SetRange(0, 100);

    if ((_nBrightness != nBrightness) ||
        (_nContrast != nContrast) ||
        (_nSaturation != nSaturation) ||
        (_iScaleImage != iScale))
    {
        _UpdateOrigBitmap();
    }

    SetTimer(FROMCLIPBOARD_TIMER, 1000, NULL);
}

void CBitmapToPicDialog::OnCancel()
{
    if (_fConverting)
    {
        if (_hEvent)
        {
            SetEvent(_hEvent);
            _hEvent = NULL;
        }
    }
    else
    {
        __super::OnCancel();
    }
}

void CBitmapToPicDialog::OnOK()
{
    ASSERT(!_fConverting);
    BOOL fClose = TRUE;
    if (_fTooBig)
    {
        if (IDNO == AfxMessageBox("The pic is too big to be saved as an SCI resource.  Accept it anyway?", MB_APPLMODAL | MB_ICONQUESTION | MB_YESNO))
        {
            fClose = FALSE;
        }
    }
    if (fClose)
    {
        __super::OnOK();
    }
}

BEGIN_MESSAGE_MAP(CBitmapToPicDialog, CExtNCW<CExtResizableDialog>)
    ON_BN_CLICKED(IDC_BUTTONCONVERT, OnConvert)
    ON_MESSAGE(UWM_PICREADY, _OnConvertDone)
    ON_MESSAGE(UWM_CONVERTSTATUS, _OnConvertStatus)
    ON_MESSAGE(UWM_CONVERTPROGRESS, _OnConvertProgress)
    ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
    ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
    ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
    ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadio5)
    ON_BN_CLICKED(IDC_RADIOHALFTONE, OnBnClickedRadiohalftone)
    ON_BN_CLICKED(IDC_RADIOSOLID, OnBnClickedRadiosolid)
    ON_BN_CLICKED(IDC_RADIOHALFTONESMOOTH, OnBnClickedRadiohalftonesmooth)
    ON_BN_CLICKED(IDC_CHECKSCALE, OnScaleClicked)
    ON_BN_CLICKED(IDC_BUTTONBROWSE, OnBrowse)
    ON_BN_CLICKED(IDC_BUTTONCLIPBOARD, OnPasteFromClipboard)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_RADIOPICPALETTE, OnBnClickedRadiopicpalette)
    ON_BN_CLICKED(IDC_CHECKIGNOREWHITE, OnBnClickedCheckignorewhite)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CBitmapToPicDialog message handlers

BOOL IsOneOf(EGACOLOR b, EGACOLOR *rgBytes, int cBytes)
{
    for (int i = 0; i < cBytes; i++)
    {
        if ((rgBytes[i].color1 == b.color1) && (rgBytes[i].color2 == b.color2))
        {
            return TRUE;
        }
    }
    return FALSE;
}

void _SendStatus(HWND hwnd, PCTSTR pszText)
{
    SendMessage(hwnd, UWM_CONVERTSTATUS, NULL, (LPARAM)pszText);
}

//
// Send progress, and bail out if the handle is set.
//
BOOL _SendProgressAndCheckAbort(HWND hwnd, HANDLE hEvent, int nCurrent, int nMax)
{
    if (WAIT_OBJECT_0 != WaitForSingleObject(hEvent, 0))
    {
        SendMessage(hwnd, UWM_CONVERTPROGRESS, NULL, (LPARAM) (100 * nCurrent / nMax));
        return FALSE;
    }
    else
    {
        return TRUE; // Abort!
    }
}

UINT CBitmapToPicDialog::s_ThreadWorker(void *pParam)
{
    THREADINFO *pInfo = (THREADINFO*)pParam;
    CSize size = pInfo->size;
    int cPixels = pInfo->size.cx * pInfo->size.cy;
    int iAlgorithm = pInfo->iAlgorithm;
    int iPalette = pInfo->iPalette;
    BOOL fIgnoreWhite = pInfo->fIgnoreWhite;
    EGACOLOR *pPicPalette = pInfo->picPalette;
    int cPicPalette = sizeof(pInfo->picPalette);
    COLORREF *pCRBitmap = pInfo->pCRBitmap;
    CArray<PicCommand, PicCommand> *pcommands = new CArray<PicCommand, PicCommand>;
    HWND hwnd = pInfo->hwndDlg;
    int nColors = pInfo->nColors;
    BOOL fAbort = FALSE;
    TCHAR szMsg[MAX_PATH];

    BOOL fSuccess = FALSE;
    if (pcommands)
    {
        EGACOLOR *pegaTemp = new EGACOLOR[cPixels];
        if (pegaTemp)
        {
            int rgColorCounts[256]; // Indexed by EGACOLOR_TO_BYTE
            ZeroMemory(rgColorCounts, sizeof(rgColorCounts));

            _SendStatus(hwnd, TEXT("Mapping image colors to SCI colors"));

            // Figure out which EGACOLOR each pixel maps to.
            int iInterval = cPixels / 100;
            for (int i = 0; !fAbort && (i < cPixels); i++)
            {
                EGACOLOR curColor;
                if (iPalette == 4)
                {
                    // iPalette = 4 means that we use the current pic's palette.
                    curColor = GetClosestEGAColorFromSet(iAlgorithm, *(pCRBitmap + i), pPicPalette, cPicPalette);
                }
                else
                {
                    curColor = GetClosestEGAColor(iAlgorithm, iPalette, *(pCRBitmap + i));
                }

                *(pegaTemp + i) = curColor;
                // Keep a count of the number of times each colour appears.
                rgColorCounts[EGACOLOR_TO_BYTE(curColor)]++;

                if ((i % iInterval) == 0)
                {
                    fAbort = _SendProgressAndCheckAbort(hwnd, pInfo->hEvent, i, cPixels);
                }
            }

            int cMostCommonColors = 0;
            EGACOLOR rgMostCommonColors[256];
            if (!fAbort)
            {
                if (iPalette == 4)
                {
                    // This is easy, just copy the pic's palette.
                    CopyMemory(rgMostCommonColors, pPicPalette, cPicPalette);
                    // We have 40 "most common colours"
                    cMostCommonColors = cPicPalette;
                }
                else
                {
                    // Count unique colors for status purposes
                    int nUniqueColors = 0;
                    int i = 0;
                    for (; i < ARRAYSIZE(rgColorCounts); i++)
                    {
                        if (rgColorCounts[i])
                        {
                            nUniqueColors++;
                        }
                    }
                    StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("Finding most common of %d colors"), nUniqueColors);
                    _SendStatus(hwnd, szMsg);

                    // Then figure out the most common MAXCONVERTEDCOLORS
                    for (int i = 0; !fAbort && (i < nColors); i++)
                    {
                        int cCount = 0;
                        int bBest = -1;
                        for (int j = 0; j < ARRAYSIZE(rgColorCounts); j++)
                        {
                            if (rgColorCounts[j] > cCount)
                            {
                                cCount = rgColorCounts[j];
                                bBest = j;
                            }
                        }
                        if (bBest != -1)
                        {
                            rgMostCommonColors[cMostCommonColors] = EGAColorFromByte(bBest);
                            rgColorCounts[bBest] = 0; // Set it to zero so it won't be included in future counts
                            cMostCommonColors++;
                        }
                        else
                        {
                            // We're done.
                            break;
                        }
                        fAbort = _SendProgressAndCheckAbort(hwnd, pInfo->hEvent, i, nColors);
                    }
                }
            }

            if (!fAbort)
            {
                StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("Remapping to most common"), cMostCommonColors);
                _SendStatus(hwnd, szMsg);


                // Now figure out which of the most common MAXCONVERTEDCOLORS each pixel belongs to.
                EGACOLOR *pegaTempI = pegaTemp;
                COLORREF *pcrTempI = pCRBitmap;
                for (int i = 0; !fAbort && (i < cPixels); pegaTempI++, pcrTempI++, i++)
                {
                    if (!IsOneOf(*pegaTempI, rgMostCommonColors, cMostCommonColors))
                    {
                        // It wasn't one of our most common colors, so re-evaluate it.
                        *pegaTempI = GetClosestEGAColorFromSet(iAlgorithm, *pcrTempI, rgMostCommonColors, cMostCommonColors);
                    }

                    if ((i % iInterval) == 0)
                    {
                        fAbort = _SendProgressAndCheckAbort(hwnd, pInfo->hEvent, i, cPixels);
                    }
                }
            }

            if (!fAbort)
            {
                // 4 == use pic's palette
                fAbort = s_ConvertToPic(hwnd, pInfo->hEvent, pcommands, pegaTemp, size, fIgnoreWhite, rgMostCommonColors, cMostCommonColors, (iPalette == 4));
            }

            if (!fAbort)
            {
                // Put the pic in the static, and give stats on it.
                PicResource *pepic = new PicResource();
                if (pepic)
                {
                    if (SUCCEEDED(pepic->InitNew()))
                    {
                        pepic->InsertCommands(-1, pcommands->GetCount(), pcommands->GetData());

                        // Make sure the pic is small enough.
                        BOOL fTooBig = FALSE;
                        sci::istream serial;
                        if (pepic->Serialize(&serial))
                        {
                            DWORD cMax = 0xff00; // Provide safe boundary, since we have other pic commands in pic
                            if (serial.GetDataSize() > cMax)
                            {
                                fTooBig = TRUE;
                                StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("Warning! Pic size: %d bytes (%d too many)"), serial.GetDataSize(), (serial.GetDataSize() - cMax));
                            }
                            else
                            {
                                StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("Success! Pic size: %d bytes"), serial.GetDataSize());
                            }
                            _SendStatus(hwnd, szMsg);
                        }

                        // We're good.
                        PicDrawManager pdm(pepic);
                        HBITMAP hbm = pdm.CreateVisualBitmap(320, 190);
                        if (hbm)
                        {
                            THREADRESPONSE *pResponse = new THREADRESPONSE;
                            if (pResponse)
                            {
                                pResponse->hbm = hbm;
                                pResponse->pcommands = pcommands;
                                pResponse->fTooBig = fTooBig;
                                fSuccess = TRUE;
                                if (::SendMessage(hwnd, UWM_PICREADY, 0, (LPARAM)pResponse))
                                {
                                    // The information has been transfered.
                                    hbm = NULL;
                                    pcommands = NULL;
                                }
                            }
                            if (hbm)
                            {
                                DeleteObject(hbm);
                            }
                        }

                    }
                    delete pepic;
                }
            }

            delete [] pegaTemp;
        }

        delete pcommands;
    }

    if (!fSuccess)
    {
        // Send null response.
        ::SendMessage(hwnd, UWM_PICREADY, 0, 0);
    }

    // Clean up the struct that was passed to us.
    FreeThreadInfo(pInfo);
    delete pInfo;

    return 0;
}


LRESULT CBitmapToPicDialog::_OnConvertDone(WPARAM wParam, LPARAM lParam)
{
    _fConverting = FALSE;
    _hEvent = NULL;
    THREADRESPONSE *pResponse = (THREADRESPONSE*)lParam;
    if (pResponse)
    {
        CRect rectBitmap;
        m_wndPic.GetClientRect(&rectBitmap);
        m_wndPic.FromBitmap(pResponse->hbm, rectBitmap.Width(), rectBitmap.Height());

        ASSERT(_pcommands == NULL);
        _pcommands = pResponse->pcommands;
        pResponse->pcommands = NULL;

        // We got a response, re-enable Ok.
        GetDlgItem(IDOK)->EnableWindow();

        _fTooBig = pResponse->fTooBig;

        delete pResponse;
    }
    else
    {
        // It failed for some other reason
    }

    // Re-enable the convert button, now that we're done.
    GetDlgItem(IDC_BUTTONCONVERT)->EnableWindow();

    // Reset progress
    m_wndProgress.SetPos(0);

    return TRUE; // Important, so the caller knows we handled it.
}

LRESULT CBitmapToPicDialog::_OnConvertStatus(WPARAM wParam, LPARAM lParam)
{
    _AddToEdit((PCTSTR)lParam);
    return TRUE;
}

LRESULT CBitmapToPicDialog::_OnConvertProgress(WPARAM wParam, LPARAM lParam)
{
    m_wndProgress.SetPos((int)lParam);
    return TRUE;
}

void CBitmapToPicDialog::_DeleteCommands()
{
    if (_pcommands)
    {
        delete _pcommands;
        _pcommands = NULL;
    }
}

void CBitmapToPicDialog::OnConvert()
{
    // Delete any old commands
    _DeleteCommands();

    // Get the most recent values.
    UpdateData(TRUE);

    // Do some validation
    if ((_nColors < 1) || (_nColors > PALETTE_SIZE))
    {
        AfxMessageBox(TEXT("Number of colours must be between 1 and 40"), MB_ERRORFLAGS);
        return;
    }

    // Disable the Convert and Ok buttons
    GetDlgItem(IDC_BUTTONCONVERT)->EnableWindow(FALSE);
    GetDlgItem(IDOK)->EnableWindow(FALSE);

    // Clear out the status box
    m_wndEditStatus.SetWindowText(TEXT(""));

    _PrepareBitmapForConversion();

    CSize size = _size;
    COLORREF *pCRBitmap = _pCRBitmap;

    THREADINFO *pInfo = new THREADINFO; // Will be deleted by thread.
    if (InitThreadInfo(pInfo, GetSafeHwnd(), (_iIgnoreWhite != 0), _iAlgorithm, _iPalette, _nColors, size, _pCRBitmap, _picPalette))
    {
        _fConverting = TRUE;
        _hEvent = pInfo->hEvent;
        _pThread = AfxBeginThread(s_ThreadWorker, pInfo, 0, 0, 0, NULL);
        if (_pThread)
        {
            pInfo = NULL;
        }
        else
        {
            FreeThreadInfo(pInfo);
            _fConverting = FALSE;
        }
        if (pInfo)
        {
            delete pInfo;
        }
    }
}


void CBitmapToPicDialog::OnBrowse()
{
    CFileDialog dialog(TRUE, NULL, NULL, NULL, g_szGdiplusFilter);
    if (IDOK == dialog.DoModal())
    {
        CString strFileName = dialog.GetFileName();
#ifdef UNICODE
        Gdiplus::Image *pImage = Bitmap::FromFile(strFileName);
#else
        // GDI+ only deals with unicode.
        int a = lstrlenA(strFileName);
        BSTR unicodestr = SysAllocStringLen(NULL, a);
        MultiByteToWideChar(CP_ACP, 0, strFileName, a, unicodestr, a);
        Gdiplus::Image *pImage = Bitmap::FromFile(unicodestr);
        //... when done, free the BSTR
        SysFreeString(unicodestr);
#endif    
        if (pImage)
        {
            if (_Init(pImage))
            {
                _UpdateOrigBitmap();
            }
            delete pImage;
        }
    }
}

void CBitmapToPicDialog::InsertIntoPic(CPicDoc *pDoc)
{
    ASSERT(_pcommands);
    pDoc->InsertCommands(_pcommands->GetCount(), _pcommands->GetData());
}

void CBitmapToPicDialog::_AddToEdit(PCTSTR pszText)
{
    m_wndEditStatus.SetWindowText(pszText);
}

BOOL CanConnect(EGACOLOR colorScreen, const EGACOLOR *rgBestColors,  int iColorIndex, BOOL *pfExact)
{
    *pfExact = FALSE;
    // The colorScreen
    BYTE bScreen = EGACOLOR_TO_BYTE(colorScreen);
    /*if (bScreen == 0xff)
    {
        return FALSE; // pure white
    }*/
    // REVIEW: possible optimization: do a fill color at the beginning for the most popular color.
    // And then remove that from the list of sorted colours we do.  And we'll need *include* it above.

    if (EGACOLOR_EQUAL(rgBestColors[iColorIndex], colorScreen))
    {
        *pfExact = TRUE;
        return TRUE; // Yes, this is our color.
    }

    // We can still connect, as long as it isn't one of the colors we're already used.
    for (int i = 0; i < iColorIndex; i++)
    {
        if (EGACOLOR_EQUAL(colorScreen, rgBestColors[i]))
        {
            return FALSE;
        }
    }
    return TRUE;
}


BYTE GetPaletteIndexOf(EGACOLOR color, EGACOLOR *rgColor, int cColors)
{
    for (int i = 0; i < cColors; i++)
    {
        if (EGACOLOR_EQUAL(color, rgColor[i]))
        {
            return (BYTE)i;
        }
    }
    ASSERT(FALSE);
    return 0;
}

#define IS_WHITE(color) (EGACOLOR_TO_BYTE(color) == 0xff)

//
// Does the meat of the conversion
// Returns TRUE if it was aborted.
//
BOOL CBitmapToPicDialog::s_ConvertToPic(HWND hwnd, HANDLE hEvent, CArray<PicCommand, PicCommand> *pcommands, EGACOLOR *pegaTemp, CSize &size, BOOL fIgnoreWhite, EGACOLOR *rgColors, int cColors, BOOL fDontSetPalette)
{
    BOOL fAbort = FALSE;
    // Clean out the commands array.
    pcommands->RemoveAll();

    if (!fDontSetPalette)
    {
        // Prepare our palette
        EGACOLOR palette[40];
        ZeroMemory(palette, sizeof(palette));
        ASSERT(cColors <= PALETTE_SIZE);
        for (int paletteColor = 0; paletteColor < cColors; paletteColor++)
        {
            palette[paletteColor] = rgColors[paletteColor];
        }
        pcommands->Add(PicCommand::CreateSetPalette(0, palette));
    }

    // An array of EGACOLOR arrays, each of which is cColors long
    EGACOLOR *rgOrderedColorsPerLine[190];
    int rgNumberOfColorsPerLine[190]; // From 1 to cColors
    ZeroMemory(rgOrderedColorsPerLine, sizeof(rgOrderedColorsPerLine));
    // Allocate one big blob for all the sub-arrays.  We'll point the members of
    // rgOrderedColorsPerLine in the next for loop.
    ASSERT(size.cy <= ARRAYSIZE(rgOrderedColorsPerLine));
    ASSERT(size.cy <= ARRAYSIZE(rgNumberOfColorsPerLine));

    _SendStatus(hwnd, TEXT("Analyzing lines"));

    EGACOLOR *rgAllColors = new EGACOLOR[cColors * ARRAYSIZE(rgOrderedColorsPerLine)];
    if (rgAllColors)
    {
        for (int line = 0; !fAbort && (line < size.cy); line++)
        {
            
            // Assign the array:
            rgOrderedColorsPerLine[line] = rgAllColors + (cColors * line);

            // An array to count how many strips there are of each on this line.
            int fragmentsPerColor[256]; // Uses real EGA numbers
            ZeroMemory(fragmentsPerColor, sizeof(fragmentsPerColor));

            {
                const EGACOLOR *pLine = pegaTemp + size.cx * line;
                EGACOLOR curColor = *pLine;

                int cFragments = 0;
                int startFragment = 0;
                int endFragment = 0;

                for (int x = 1; x < size.cx; x++)
                {
                    EGACOLOR color = *(pLine + x);
                    if (!EGACOLOR_EQUAL(color, curColor))
                    {
                        if (startFragment == endFragment)
                        {
                            // It was just a single dot.  That doesn't counts as much as a regular
                            // fragment - since we care less about optimizing it into a big line, since
                            // a bunch of little dots are cheaper than a bunch of little lines.
                            // I have found that the best thing is just to not count it at all.
                            // However, since we must count this color at least once (or else it will be
                            // excluded from the line completely), then count the first one.
                            if (fragmentsPerColor[EGACOLOR_TO_BYTE(curColor)] == 0)
                            {
                                fragmentsPerColor[EGACOLOR_TO_BYTE(curColor)]++;
                            }
                        }
                        else
                        {
                            fragmentsPerColor[EGACOLOR_TO_BYTE(curColor)]++;
                        }

                        // Start a new fragment.
                        cFragments++;
                        startFragment = x;
                        endFragment = x;

                        curColor = color;
                    }
                    else
                    {
                        endFragment = x;
                    }
                }
                fragmentsPerColor[EGACOLOR_TO_BYTE(curColor)]++;
            }

            if (fIgnoreWhite)
            {
                // If we make white the "most common" on every line, then it will be drawn first,
                // which is essential if we're "ignoring white"
                fragmentsPerColor[0xff] = sPIC_WIDTH + 1;
            }

            // Now we have a count of the most popular colours on this line, and how many fragments there are of each.
            EGACOLOR *rgOrderedColorsForThisLine = rgOrderedColorsPerLine[line];
            if (rgOrderedColorsForThisLine)
            {
                int i = 0;
                for (; i < cColors; i++)
                {
                    int cFragCount = 0;
                    BYTE bestColorIndex = 0;
                    for (WORD j = 0; j < cColors; j++)
                    {
                        BYTE indexIntoFPC = EGACOLOR_TO_BYTE(rgColors[j]);
                        if (fragmentsPerColor[indexIntoFPC] > cFragCount)
                        {
                            cFragCount = fragmentsPerColor[indexIntoFPC];
                            bestColorIndex = (BYTE)j;
                        }
                    }
                    if (cFragCount == 0)
                    {
                        break;
                    }
                    else
                    {
                        rgOrderedColorsForThisLine[i] = rgColors[bestColorIndex];
                        // zero out, so we don't count this again.
                        fragmentsPerColor[EGACOLOR_TO_BYTE(rgColors[bestColorIndex])] = 0;
                    }
                }
                rgNumberOfColorsPerLine[line] = i;
                ASSERT(i > 0);
            }

            fAbort = _SendProgressAndCheckAbort(hwnd, hEvent, line, size.cy);
        }
    }

    if (rgAllColors && !fAbort)
    {
        _SendStatus(hwnd, (TEXT("Generating commands...")));

        // Algorithm:
        // 1) Have a number for each line, that represents where we are in the ordered array of colors for that line:
        int rgCurrentColorPosPerLine[190];
        ZeroMemory(rgCurrentColorPosPerLine, sizeof(rgCurrentColorPosPerLine));
        // 2) Have a number that indicates, in general, what are we drawing right now.
        int currentColorPosWereDrawing = 0;
        // 3) for each color pos, 0 to cColors, we do:
        for ( ; !fAbort && (currentColorPosWereDrawing < cColors); currentColorPosWereDrawing++)
        {
            fAbort = _SendProgressAndCheckAbort(hwnd, hEvent, currentColorPosWereDrawing, cColors);

            // 4) for each line, we do:
            for (int line = 0; line < size.cy; line++)
            {
                if ((currentColorPosWereDrawing < rgNumberOfColorsPerLine[line]) &&  // Verify we don't start looking at garbage colours in this line
                    (rgCurrentColorPosPerLine[line] == currentColorPosWereDrawing))  // Is this line current at the same "color pos" that we're drawing?
                {
                    // This is the colour we'll be using:
                    EGACOLOR currentColor = rgOrderedColorsPerLine[line][currentColorPosWereDrawing];

                    // Make this the current color - set it as color 0, and set the visual to taht.
                    // TODO: PERF: improve size, by setting a palette @ the beginning.
                    pcommands->Add(PicCommand::CreateSetVisual(0, GetPaletteIndexOf(currentColor, rgColors, cColors)));

                    for (int realLine = line; realLine < size.cy; realLine++)
                    {
                        // Decide to draw this line if it is the same color as THE line
                        EGACOLOR *rgOrderedColorsForThisLine = rgOrderedColorsPerLine[realLine];
                        if (EGACOLOR_EQUAL(currentColor, rgOrderedColorsForThisLine[currentColorPosWereDrawing]))
                        {
                            const EGACOLOR *pLine = pegaTemp + size.cx * realLine;
                            for (int x = 0; x < size.cx; x++)
                            {
                                if (EGACOLOR_EQUAL(currentColor, *(pLine + x)))
                                {
                                    // We've hit one of our colours.  Now continue until we can't anymore.
                                    int xStart = x;
                                    int xLastExact = xStart;
                                    BOOL fExact;
                                    while ((x < size.cx) && CanConnect(*(pLine + x), rgOrderedColorsForThisLine, currentColorPosWereDrawing, &fExact))
                                    {
                                        if (fExact)
                                        {
                                            xLastExact = x; // This is the last colour that was an exact match.
                                        }
                                        x++;
                                    }
                                    ASSERT(xStart < x); // We must always hit the above loop once.
                                    //int xEnd = x - 1;
                                    int xEnd = xLastExact;

                                    if (fIgnoreWhite && IS_WHITE(currentColor) && ((xStart == 0) || (xEnd == (size.cx - 1))))
                                    {
                                        // Don't do anything.
                                    }
                                    else
                                    {
                                        // Draw line to xStart to xEnd;
                                        if (xStart == xEnd)
                                        {
                                            // Just a dot
                                            pcommands->Add(PicCommand::CreatePattern((WORD)xStart, (WORD)realLine, 0, 0, FALSE, FALSE));
                                        }
                                        else
                                        {
                                            // A line
                                            pcommands->Add(PicCommand::CreateLine((WORD)xStart, (WORD)realLine, (WORD)xEnd, (WORD)realLine));
                                        }
                                    }
                                }
                            }                    

                            // Increment the colorpos for this line.
                            rgCurrentColorPosPerLine[realLine]++;
                        }
                    }
                }
            }
        }
    }

    if (rgAllColors)
    {
        delete [] rgAllColors;
    }

    return fAbort;
}

//
// Ensures we have a _pCRBitmap: the array of COLORREF values to convert
//
BOOL CBitmapToPicDialog::_ReallocateCRBitmap(CSize size)
{
    if ((size != _size) || (_pCRBitmap == NULL))
    {
        delete [] _pCRBitmap;
        _size = size;
        _pCRBitmap = new COLORREF[_size.cx * _size.cy];
    }
    return (_pCRBitmap != NULL);
}

//
// Prepares _pbmpCurrent for conversion.
//
void CBitmapToPicDialog::_PrepareBitmapForConversion()
{
    if (_pbmpCurrent)
    {
        CSize size((int)_pbmpCurrent->GetWidth(), (int)_pbmpCurrent->GetHeight());
        if (_ReallocateCRBitmap(size))
        {
            for (int y = 0; y < size.cy; y++)
            {
                COLORREF *pLineCR = _pCRBitmap + size.cx * y;
                for (int x = 0; x < size.cx; x++)                                
                {
                    Gdiplus::Color color;
                    if (Ok == _pbmpCurrent->GetPixel(x, y, &color))
                    {
                        *(pLineCR + x) = color.ToCOLORREF();
                    }
                }
            }
        }
    }
}



void CBitmapToPicDialog::_ApplySettingsToCurrentBitmap()
{
    int nBrightness = (_nBrightness - 50) * 255 / 50; // 0 - 100  --> -255 to 255

    int nContrast = (100 - _nContrast);
    nContrast = max(1, nContrast); // avoid divide by zero
    nContrast = 255 * 50 / nContrast;

    int nSat = (100 - _nSaturation);
    nSat = max(1, nSat); // avoid divide by zero
    nSat = 255 * 50 / nSat;

    UINT cx = _pbmpCurrent->GetWidth();
    UINT cy = _pbmpCurrent->GetHeight();

    Gdiplus::Rect rect(0, 0, cx, cy);
    Gdiplus::BitmapData bitmapData;
    if (Ok == _pbmpCurrent->LockBits(&rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData))
    {
        int nCCGreen = 0;
        int nCCRed = 0;
        int nCCBlue = 0;
        for (UINT y = 0; y < cy; y++)
        {
            ARGB *pLine = ((COLORREF*)bitmapData.Scan0) + y * bitmapData.Width;
            for (UINT x = 0; x < cx; x++, pLine++)
            {
                Gdiplus::Color color;
                color.SetValue(*pLine);

                int red = (int)color.GetR();
                int blue = (int)color.GetB();
                int green = (int)color.GetG();
                if (nBrightness != 0)
                {
                    red += nBrightness;
                    blue += nBrightness;
                    green += nBrightness;
                }

                red = min(255, max(red, 0));
                blue = min(255, max(blue, 0));
                green = min(255, max(green, 0));
                nCCRed += red;
                nCCGreen += green;
                nCCBlue += blue;

                *pLine = Gdiplus::Color::MakeARGB(255, red, green, blue);
            }
        }
        nCCGreen /= (cx * cy);
        nCCRed /= (cx * cy);
        nCCBlue /= (cx * cy);

        for (UINT y = 0; y < cy; y++)
        {
            ARGB *pLine = ((COLORREF*)bitmapData.Scan0) + y * bitmapData.Width;
            for (UINT x = 0; x < cx; x++, pLine++)
            {
                Gdiplus::Color color;
                color.SetValue(*pLine);

                int red = (int)color.GetR();
                int blue = (int)color.GetB();
                int green = (int)color.GetG();

                if (nContrast != 255)
                {
                    red = ((red - nCCRed) * nContrast / 255) + nCCRed;
                    green = ((green - nCCGreen) * nContrast / 255) + nCCGreen;
                    blue = ((blue - nCCBlue) * nContrast / 255) + nCCBlue;
                }

                if (nSat != 255)
                {
                    // Take the avg of the colours
                    int avg = (red + green + blue) / 3;

                    // Everything will tend towards or away from this colour.
                    red = ((red - avg) * nSat / 255) + avg;
                    blue = ((blue - avg) * nSat / 255) + avg;
                    green = ((green - avg) * nSat / 255) + avg;
                }

                red = min(255, max(red, 0));
                blue = min(255, max(blue, 0));
                green = min(255, max(green, 0));
                *pLine = Gdiplus::Color::MakeARGB(255, red, green, blue);
            }
        }


        _pbmpCurrent->UnlockBits(&bitmapData);
    }
}

//
// Updates the original bitmap.  This does the following:
// 1) Copies either _pbmpNormal or _pbmpScaled to _pbmpCurrent
//
void CBitmapToPicDialog::_UpdateOrigBitmap()
{
    // Clear out the old Gdiplus Bitmap
    if (_pbmpCurrent)
    {
        delete _pbmpCurrent;
        _pbmpCurrent = NULL;
    }

    // Get a clone of the appropriate bitmap
    if (_iScaleImage)
    {
        if (_pbmpScaled)
        {
            _pbmpCurrent = (Gdiplus::Bitmap*)((Gdiplus::Image*)_pbmpScaled)->Clone();
            _nContrastCenter = _bContrastCenterScaled;
        }
    }
    else
    {
        if (_pbmpNormal)
        {
            _pbmpCurrent = (Gdiplus::Bitmap*)((Gdiplus::Image*)_pbmpNormal)->Clone();
            _nContrastCenter = _bContrastCenterNormal;
        }
    }

    if (_pbmpCurrent)
    {
        _ApplySettingsToCurrentBitmap();
    
        HBITMAP hbmpOrig = NULL;
        if (Ok == _pbmpCurrent->GetHBITMAP(Color::Black, &hbmpOrig))
        {
            ASSERT(hbmpOrig);
            CBitmap bitmapOrig;
            bitmapOrig.Attach(hbmpOrig);
            if (m_wndOrig.GetSafeHwnd())
            {
                m_wndOrig.FromBitmap(hbmpOrig, (int)_pbmpCurrent->GetWidth(), (int)_pbmpCurrent->GetHeight());
                // Redraw right away
                m_wndOrig.RedrawWindow();
            }
            DeleteObject(hbmpOrig);
        }
    }
}

void CBitmapToPicDialog::_CalculateContrastCenter(Gdiplus::Bitmap *pBitmap, BYTE *pbContrastCenter)
{
    UINT cx = pBitmap->GetWidth();
    UINT cy = pBitmap->GetHeight();

    Gdiplus::Rect rect(0, 0, cx, cy);
    Gdiplus::BitmapData bitmapData;
    int iTotal = 0;
    if (Ok == pBitmap->LockBits(&rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppRGB, &bitmapData))
    {
        for (UINT y = 0; y < cy; y++)
        {
            ARGB *pLine = ((COLORREF*)bitmapData.Scan0) + y * bitmapData.Width;
            for (UINT x = 0; x < cx; x++, pLine++)
            {
                Gdiplus::Color color;
                color.SetValue(*pLine);
                iTotal += (int)color.GetR() + (int)color.GetB() + (int)color.GetG();
            }
        }
        iTotal /= (cx * cy * 3);
        *pbContrastCenter = (BYTE)iTotal;
        pBitmap->UnlockBits(&bitmapData);
    }
}

BOOL CBitmapToPicDialog::_Init(Gdiplus::Image *pImage)
{
    CSize size(min(sPIC_WIDTH, pImage->GetWidth()), min(sPIC_HEIGHT, pImage->GetHeight()));
    BOOL fRet = FALSE;

    // Clear out any old ones.
    delete _pbmpNormal;
    delete _pbmpScaled;
    _pbmpNormal = NULL;
    _pbmpScaled = NULL;

    // Make a scaled and normal copy of the original.
    CDC dcTemp;
    if (dcTemp.CreateCompatibleDC(NULL))
    {
        _pbmpNormal = new Gdiplus::Bitmap(size.cx, size.cy, PixelFormat32bppRGB);
        if (_pbmpNormal)
        {
            Graphics graphics(_pbmpNormal);
            fRet = (Ok == graphics.DrawImage(pImage, 0, 0, 0, 0, pImage->GetWidth(), pImage->GetHeight(), UnitPixel));
            if (fRet)
            {
                _CalculateContrastCenter(_pbmpNormal, &_bContrastCenterNormal);
            }
        }

        if (fRet)
        {
            _pbmpScaled = new Gdiplus::Bitmap(sPIC_WIDTH, sPIC_HEIGHT, PixelFormat32bppRGB);
            if (_pbmpScaled)
            {
                Graphics graphics(_pbmpScaled);
                fRet = (Ok == graphics.DrawImage(pImage, 0, 0, sPIC_WIDTH, sPIC_HEIGHT));
                if (fRet)
                {
                    _CalculateContrastCenter(_pbmpScaled, &_bContrastCenterScaled);
                }
            }
        }
    }

    // Enable the convert button upon success
    ((CButton*)GetDlgItem(IDC_BUTTONCONVERT))->EnableWindow(fRet);

    return fRet;
}




void CBitmapToPicDialog::OnBnClickedRadio1()
{
    _iAlgorithm = 1;
}

void CBitmapToPicDialog::OnBnClickedRadio2()
{
    _iAlgorithm = 2;
}

void CBitmapToPicDialog::OnBnClickedRadio3()
{
    _iAlgorithm = 3;
}

void CBitmapToPicDialog::OnBnClickedRadio4()
{
    _iAlgorithm = 4;
}

void CBitmapToPicDialog::OnBnClickedRadio5()
{
    _iAlgorithm = 5;
}

void CBitmapToPicDialog::OnBnClickedRadiohalftone()
{
    _iPalette = 1;
}

void CBitmapToPicDialog::OnBnClickedRadiosolid()
{
    _iPalette = 2;
}

void CBitmapToPicDialog::OnBnClickedRadiohalftonesmooth()
{
    _iPalette = 3;
}

void CBitmapToPicDialog::OnBnClickedRadiopicpalette()
{
    _iPalette = 4;
}

void CBitmapToPicDialog::OnScaleClicked()
{
    // Get the most recent values
    UpdateData(TRUE);
}

void CBitmapToPicDialog::OnBnClickedCheckignorewhite()
{
    UpdateData(TRUE);
}


void CBitmapToPicDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pWnd)
{
    UpdateData(TRUE);
}

void CBitmapToPicDialog::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == FROMCLIPBOARD_TIMER)
    {
        m_wndFromClipboard.EnableWindow(IsClipboardFormatAvailable(CF_BITMAP));
    }
    else
    {
        __super::OnTimer(nIDEvent);
    }
}



void CBitmapToPicDialog::OnPasteFromClipboard()
{
    if (IsClipboardFormatAvailable(CF_BITMAP))
    {
        if (OpenClipboard())
        {
            HBITMAP hBmp = (HBITMAP)GetClipboardData(CF_BITMAP);

            Gdiplus::Bitmap *pBitmap = Gdiplus::Bitmap::FromHBITMAP(hBmp, NULL);
            if (pBitmap)
            {
                if (_Init(pBitmap))
                {
                    _UpdateOrigBitmap();
                }
                delete pBitmap;
            }

            CloseClipboard();
        }
    }
}



BOOL CBitmapToPicDialog::OnInitDialog()
{
    __super::OnInitDialog();

    // Disable the Ok button until we have a converted pic.
    GetDlgItem(IDOK)->EnableWindow(FALSE);

    // Put a little help text
    _AddToEdit(TEXT("Choose options, then click Convert"));

    OnPasteFromClipboard();

    return TRUE;
}

