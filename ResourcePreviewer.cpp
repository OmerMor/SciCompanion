// ResourcePreviewer.cpp : implementation file
//
// Contains implementations of all the resource previewers.
//
#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ResourcePreviewer.h"
#include "CompiledScript.h"
#include "Vocab99x.h"
#include "PicDrawManager.h"

#include <vfw.h>

BOOL ResourcePreviewer::OnInitDialog()
{
    BOOL fRet = __super::OnInitDialog();
    ShowSizeGrip(FALSE);
    return fRet;
}

//
// Pic Previewer
//
void PicPreviewer::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_STATICVISUAL, m_wndVisual);
    DDX_Control(pDX, IDC_GROUPVISUAL, m_wndVisualGroup);
    m_wndVisualGroup.SetStyle(CExtGroupBox::e_style_t::STYLE_CAPTION);
    DDX_Control(pDX, IDC_STATICPRIORITY, m_wndPriority);
    DDX_Control(pDX, IDC_GROUPPRIORITY, m_wndPriorityGroup);
    m_wndPriorityGroup.SetStyle(CExtGroupBox::e_style_t::STYLE_CAPTION);
    DDX_Control(pDX, IDC_STATICCONTROL, m_wndControl);
    DDX_Control(pDX, IDC_GROUPCONTROL, m_wndControlGroup);
    m_wndControlGroup.SetStyle(CExtGroupBox::e_style_t::STYLE_CAPTION);

    // Visuals
    DDX_Control(pDX, IDC_BUTTON1, m_wndButton1);
    DDX_Control(pDX, IDC_BUTTON2, m_wndButton2);
    DDX_Control(pDX, IDC_BUTTON3, m_wndButton3);
    DDX_Control(pDX, IDC_BUTTON4, m_wndButton4);
    DDX_Control(pDX, IDC_STATICPALETTE, m_wndStaticPalette);
}

PicPreviewer::~PicPreviewer()
{
}

BEGIN_MESSAGE_MAP(PicPreviewer, ResourcePreviewer)
    ON_COMMAND_EX(IDC_BUTTON1, OnSetPalette)
    ON_COMMAND_EX(IDC_BUTTON2, OnSetPalette)
    ON_COMMAND_EX(IDC_BUTTON3, OnSetPalette)
    ON_COMMAND_EX(IDC_BUTTON4, OnSetPalette)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CNoFlickerStatic message handlers
BOOL PicPreviewer::OnEraseBkgnd(CDC *pDC)
{
    return TRUE;
}

BOOL PicPreviewer::OnSetPalette(UINT nID)
{
    ASSERT((IDC_BUTTON4 - IDC_BUTTON1) == 3);
    _paletteNumber = (BYTE)(nID - IDC_BUTTON1);
    PicDrawManager pdm(_pic.get());
    pdm.SetPalette(_paletteNumber);
    _ResetVisualBitmap(pdm);

    // Update buttons states - ON_UPDATE_COMMAND_UI doesn't work in dialogs.
    for (UINT buttonId = IDC_BUTTON1; buttonId <= IDC_BUTTON4; buttonId++)
    {
        CExtButton *pButton = static_cast<CExtButton*>(GetDlgItem(buttonId));
        pButton->SetCheck(buttonId == nID);
    }
    return TRUE;
}

void PicPreviewer::OnUpdatePaletteButton(CCmdUI *pCmdUI)
{
    BYTE paletteNumber = (BYTE)(pCmdUI->m_nID - IDC_BUTTON1);
    pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck(paletteNumber == _paletteNumber);
}

void PicPreviewer::_ResetVisualBitmap(PicDrawManager &pdm)
{
    CRect rc;
    m_wndVisual.GetClientRect(&rc);
    CBitmap bitmap;
    bitmap.Attach(pdm.CreateVisualBitmap(rc.Width(), rc.Height()));
    m_wndVisual.FromBitmap((HBITMAP)bitmap, rc.Width(), rc.Height());
}

void PicPreviewer::SetResource(const ResourceBlob &blob)
{
    _pic.reset(new PicResource);
    if (SUCCEEDED(_pic.get()->InitFromResource(&blob)))
    {
        PicDrawManager pdm(_pic.get());
        pdm.SetPalette(_paletteNumber);
        pdm.RefreshAllScreens(); // Be efficient - we're going to get all 3 screens.
        _ResetVisualBitmap(pdm);

        // Do the priority and controls too.
        CRect rc;
        m_wndVisual.GetClientRect(&rc);
        CBitmap bitmapP;
        bitmapP.Attach(pdm.CreatePriorityBitmap(rc.Width(), rc.Height()));
        m_wndPriority.FromBitmap((HBITMAP)bitmapP, rc.Width(), rc.Height());
        CBitmap bitmapC;
        bitmapC.Attach(pdm.CreateControlBitmap(rc.Width(), rc.Height()));
        m_wndControl.FromBitmap((HBITMAP)bitmapC, rc.Width(), rc.Height());
    }
}

HBITMAP CreateBitmapFromResource(const IResourceBase *prb, SCIBitmapInfoEx *pbmi, BYTE **ppBitsDest)
{
    HBITMAP hbmpRet = NULL;
    const ICelResource *pcr = NULL;
    prb->QueryCapability(IID_PPV_ARGS_CONST(&pcr));
    const IViewResource *pvr = NULL;
    prb->QueryCapability(IID_PPV_ARGS_CONST(&pvr));
    const IRasterResource *prr = NULL;
    prb->QueryCapability(IID_PPV_ARGS_CONST(&prr));
    // Generate our bitmap.
    // First, figure out the size
    int cLoops = pvr ? pvr->GetLoopCount() : 1;
    int cx = 0;         // Total width
    int cy = 0;         // Total height
    int cBytesMax = 0;  // Max buffer size we need for an individual bitmap.
    std::vector<int> loopHeights;   // Height of each row
    // Flip the order of the loops, since we're drawing the bitmaps upside down, essetially.
    for (int loop = cLoops - 1; loop >= 0; loop--)
    {
        int cxLoop = 0;
        int cyLoop = 0;
        int cCels = pcr ? pcr->GetCelCount(loop) : 1;
        for (int cel = 0; cel < cCels; cel++)
        {
            CSize size = prr->GetSize(MAKE_INDEX(loop, cel));
            cBytesMax = max(cBytesMax, CX_ACTUAL(size.cx) * size.cy);
            cyLoop = max(size.cy, cyLoop);
            cxLoop += size.cx + 1;
        }
        cx = max(cx, cxLoop);
        cy += cyLoop + 1;
        loopHeights.push_back(cyLoop + 1);
    }

    // Subtract one from the border
    cx--;
    cy--;

    // Now we have a bitmap size.
    CDC dc;
    if (dc.CreateCompatibleDC(NULL))
    {
        SCIBitmapInfoEx bmi(cx, cy);
        BYTE *pBitsDest;
        CBitmap bitmap;
        if (bitmap.Attach(CreateDIBSection((HDC)dc, &bmi, DIB_RGB_COLORS, (void**)&pBitsDest, NULL, 0)))
        {
            memset(pBitsDest, 0x0, CX_ACTUAL(cx) * cy); // Fill with black
            scoped_array<BYTE> buffer(new BYTE[cBytesMax]);
            int top = 0;
            for (int loop = cLoops - 1; loop >= 0; loop--)
            {
                int cCels = pcr ? pcr->GetCelCount(loop) : 1;
                int left = 0;
                for (int cel = 0; cel < cCels; cel++)
                {
                    CSize size = prr->GetSize(MAKE_INDEX(loop, cel));
                    // Copy to a temp buffer.
                    prr->CopyBitmapData(MAKE_INDEX(loop, cel), buffer.get(), size);
                    // Now copy to our actual bitmap.
                    int bottom = top + size.cy;
                    for (int y = top; y < bottom; y++)
                    {
                        BYTE *pDest = pBitsDest + (CX_ACTUAL(cx) * y) + left;
                        BYTE *pSrc = buffer.get() + (y - top) * CX_ACTUAL(size.cx);
                        CopyMemory(pDest, pSrc, size.cx);
                        ASSERT((pSrc - buffer.get()) < cBytesMax);
                        ASSERT((pDest - pBitsDest) < (CX_ACTUAL(cx) * cy));
                    }
                    left += size.cx + 1;
                }
                top += loopHeights[cLoops - loop - 1];
            }
            hbmpRet = (HBITMAP)bitmap.Detach();
            *pbmi = bmi;
            *ppBitsDest = pBitsDest;
        }
    }
    return hbmpRet;
}


//
// View previewer
//
BEGIN_MESSAGE_MAP(ViewPreviewer, ResourcePreviewer)
END_MESSAGE_MAP()

void ViewPreviewer::SetResource(const ResourceBlob &blob)
{
    //
    // Generate a large bitmap containing all the views.
    //
    std::auto_ptr<ViewResource> view(new ViewResource);
    if (SUCCEEDED(view.get()->InitFromResource(&blob)))
    {
        _view = view;
        CBitmap bitmap;
        SCIBitmapInfoEx bmi(0, 0);
        BYTE *pBitsDest;
        bitmap.Attach(CreateBitmapFromResource(_view.get(), &bmi, &pBitsDest));
        m_wndView.FromBitmap((HBITMAP)bitmap, bmi.bmiHeader.biWidth, abs(bmi.bmiHeader.biHeight));
    }
}

void ViewPreviewer::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATICVIEW, m_wndView);
    AddAnchor(IDC_STATICVIEW, CPoint(0, 0), CPoint(100, 100));
}

ViewPreviewer::~ViewPreviewer() {}



//
// Script previewer
//
BEGIN_MESSAGE_MAP(ScriptPreviewer, ResourcePreviewer)
END_MESSAGE_MAP()

ScriptPreviewer::~ScriptPreviewer() {}

void ScriptPreviewer::SetResource(const ResourceBlob &blob)
{
    // Try to find a source file.
    std::string scriptFileName = theApp.GetResourceMap().GetScriptFileName(blob.GetName());

    std::ifstream scriptFile(scriptFileName.c_str());
    if (scriptFile.is_open())
    {
        ScriptId scriptId(scriptFileName);
        if (scriptId.Language() == LangSyntaxSCIStudio)
        {
            m_wndHeader.SetWindowText("Language: SCI Studio");
        }
        else
        {
            m_wndHeader.SetWindowText("Language: C syntax");
        }

        std::string scriptText;
        std::string line;
        while (std::getline(scriptFile, line))
        {
            scriptText += line;
            scriptText += "\r\n";

        }
        m_wndEdit.SetWindowText(scriptText.c_str());
    }
    else
    {
        m_wndHeader.SetWindowText("");

        // If that wasn't possible, spew info from the compiled script resource:
        CompiledScript compiledScript(0);
        std::auto_ptr<sci::istream> pStream(blob.ConvertToByteStream());
        if (compiledScript.Load(*pStream))
        {
            // Write some crap.
            std::stringstream out;
            compiledScript.DebugOut(out, true);
            // Now we have a stream.  Put it in the edit box
            m_wndEdit.SetWindowText(out.str().c_str());
        }
        else
        {
            m_wndEdit.SetWindowText("Unable to load script.");
        }
    }
}

void ScriptPreviewer::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_EDITSCRIPT, m_wndEdit);
    DDX_Control(pDX, IDC_STATICHEADER, m_wndHeader);
    AddAnchor(IDC_EDITSCRIPT, CPoint(0, 0), CPoint(100, 100));
}



//
// Text previewer
//
BEGIN_MESSAGE_MAP(TextPreviewer, ResourcePreviewer)
END_MESSAGE_MAP()

TextPreviewer::~TextPreviewer() {}

void TextPreviewer::SetResource(const ResourceBlob &blob)
{
    TextResource text;
    if (SUCCEEDED(text.InitFromResource(&blob)))
    {
        const TextResource::container_type &texts = text.GetTexts();
        TextResource::container_type::const_iterator it = texts.begin();
        std::stringstream ss;
        while (it != texts.end())
        {
            ss << (*it) << "\r\n";
            ++it;
        }
        m_wndEdit.SetWindowText(ss.str().c_str());
    }
    else
    {
        m_wndEdit.SetWindowText("Unable to load text resource.");
    }
}

void TextPreviewer::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDITSCRIPT, m_wndEdit);
    AddAnchor(IDC_EDITSCRIPT, CPoint(0, 0), CPoint(100, 100));
}



//
// Vocab previewer
//
BEGIN_MESSAGE_MAP(VocabPreviewer, ResourcePreviewer)
END_MESSAGE_MAP()

VocabPreviewer::~VocabPreviewer() {}


void VocabPreviewer::_Populate(const std::vector<std::string> &names)
{
    std::vector<std::string>::const_iterator it = names.begin();
    
    std::string text;
    //text.reserve(12000); // Typical size is 12000 characters.
    while (it != names.end())
    {
        text += (*it);
        text += "\r\n";
        ++it;
    }
    m_wndEdit.SetWindowText(text.c_str());
    /*std::stringstream ss;
    while (it != names.end())
    {
        ss << (*it) << "\r\n";
        ++it;
    }
    m_wndEdit.SetWindowText(ss.str().c_str());*/
}

void VocabPreviewer::SetResource(const ResourceBlob &blob)
{
    // It appears that the different vocabs have different formats.  We would need
    // to write previers for each kind.
    // Let's use some heuristics to determine how to load them.
    int iNumber = blob.GetNumber();
    bool fSuccess = false;
    switch (iNumber)
    {
    case 0: // Vocab 000
        {
            Vocab000 vocab(true); // true -> "simple" mode for perf.
            CPrecisionTimer timer;
            timer.Start();
            double initTime = 0.0;
            double popTime = 0.0;
            if (SUCCEEDED(vocab.InitFromResource(&blob)))
            {
                initTime = timer.Stop();

                timer.Start();
                _Populate(vocab.GetWords());
                popTime = timer.Stop();
                fSuccess = true;
            }
        }
        break;
    case 995: // debug info
        {
            CVocabWithNames vocab;
            std::auto_ptr<sci::istream> pStream(blob.ConvertToByteStream());
            if (vocab.Create(pStream.get(), true))
            {
                _Populate(vocab.GetNames());
                fSuccess = true;
            }
        }
        break;
    case 997: // species table
        {
            SpeciesTable species;
            if (species.Load())
            {
                _Populate(species.GetNames());
                fSuccess = true;
            }
        }
        break;
    case 999: // kernel functions
        {
            KernelTable kernels;
            if (kernels.Load())
            {
                _Populate(kernels.GetNames());
                fSuccess = true;
            }
        }
        break;
    default:
        m_wndEdit.SetWindowText("Unsupported vocab resource.");
        fSuccess = true;
        break;
    }

    if (!fSuccess)
    {
        m_wndEdit.SetWindowText("Unable to load vocab resource.");
    }
}

void VocabPreviewer::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDITSCRIPT, m_wndEdit);
    AddAnchor(IDC_EDITSCRIPT, CPoint(0, 0), CPoint(100, 100));
}



//
// Font previewer
//
BEGIN_MESSAGE_MAP(FontPreviewer, ResourcePreviewer)
END_MESSAGE_MAP()

FontPreviewer::~FontPreviewer() {}

void FontPreviewer::SetResource(const ResourceBlob &blob)
{
    std::auto_ptr<CFontResource> pFont(new CFontResource);
    if (SUCCEEDED(pFont.get()->InitFromResource(&blob)))
    {
        _pFont = pFont;
        _pWndFontView->SetFontResource(_pFont.get());
    }
}

BOOL FontPreviewer::OnInitDialog()
{
    BOOL fRet = __super::OnInitDialog();
    CRect rc;
    GetClientRect(&rc);
    CCreateContext context;
    context.m_pNewViewClass = RUNTIME_CLASS(CFontPreviewView);
    _pWndFontView = static_cast<CFontPreviewView*>(context.m_pNewViewClass->CreateObject());
    if (!_pWndFontView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rc, this, 12345, &context))
    {
        return FALSE;
    }
    else
    {
        AddAnchor(_pWndFontView->GetSafeHwnd(), CSize(0, 0), CSize(100, 100));
    }
    return fRet;
}

void FontPreviewer::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}




//
// Sound previewer
//
BEGIN_MESSAGE_MAP(SoundPreviewer, ResourcePreviewer)
    ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, OnSynthChoiceChange)
    ON_CBN_SELCHANGE(IDC_COMBO_MIDIDEVICE, OnMIDIDeviceChange)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, OnPlay)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnStop)
    ON_WM_TIMER()
END_MESSAGE_MAP()

#define SOUND_TIMER 5003

SoundPreviewer::SoundPreviewer()
{
    _fPlaying = false;
}


void SoundPreviewer::SetResource(const ResourceBlob &blob)
{
    _sound.reset(new SoundResource);
    _sound->InitFromResource(&blob);
    g_midiPlayer.SetSound(*_sound.get(), SoundResource::StandardTempo); // We don't have a tempo control
    OnSynthChoiceChange();
    if (m_wndAutoPreview.GetCheck() == BST_CHECKED)
    {
        OnPlay();
    }
    else
    {
        OnStop();
    }
}

void SoundPreviewer::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_COMBO_DEVICE, m_wndSynths);
    DDX_Control(pDX, IDC_EDIT_CHANNELS, m_wndChannels);
    DDX_Control(pDX, IDC_STATIC_SYNTH, m_wndStaticSynth);
    DDX_Control(pDX, IDC_BUTTON_PLAY, m_wndPlay);
    DDX_Control(pDX, IDC_BUTTON_STOP, m_wndStop);
    DDX_Control(pDX, IDC_SLIDER, m_wndSlider);
    DDX_Control(pDX, IDC_CHECK_AUTOPREV, m_wndAutoPreview);

    AddAnchor(IDC_COMBO_DEVICE, CPoint(0, 0), CPoint(100, 0));
    AddAnchor(IDC_SLIDER, CPoint(0, 0), CPoint(100, 0));
}



BOOL SoundPreviewer::OnInitDialog()
{
    BOOL fRet = __super::OnInitDialog();
    CRect rc;
    GetClientRect(&rc);
   
    CDC *pDC = GetDC();
    {
        LOGFONT logFont = { 0 };
        StringCchCopy(logFont.lfFaceName, ARRAYSIZE(logFont.lfFaceName), "Marlett");
        logFont.lfHeight = -MulDiv(10, GetDeviceCaps((HDC)*pDC, LOGPIXELSY), 72);
	    logFont.lfWeight = FW_NORMAL;
	    logFont.lfItalic = FALSE;
	    logFont.lfCharSet = DEFAULT_CHARSET;
	    logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	    logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	    logFont.lfQuality = DEFAULT_QUALITY;
	    logFont.lfPitchAndFamily = FIXED_PITCH;
        _marlettFont.CreateFontIndirect(&logFont);
        m_wndPlay.SetFont(&_marlettFont);
        m_wndStop.SetFont(&_marlettFont);
    }

    // Add the items to the combobox.
    PopulateComboWithDevicesHelper(m_wndSynths);
    m_wndSynths.SetCurSel(0);
    OnSynthChoiceChange();

    m_wndSlider.SetRange(0, 100);

    _UpdatePlayState();

    ReleaseDC(pDC);
    return fRet;
}

std::string SoundPreviewer::_FillChannelString(BYTE bChannel, bool fHeader)
{
    std::string channelString;
    ASSERT(((0x1 << bChannel) & _wChannelMask) || (bChannel == 0));
    WORD wMask = _wChannelMask;
    for (int i = 0; i < 15; i++)
    {
        if (wMask & 0x1)
        {
            // This channel is used.
            if (fHeader)
            {
                char sz[3];
                StringCchPrintf(sz, ARRAYSIZE(sz), "%1x", i);
                channelString += sz;
            }
            else
            {
                if (bChannel == i)
                {
                    // It's used by this event.
                    channelString += "X";
                }
                else
                {
                    channelString += " ";
                }
            }
        }
        wMask >>= 1;
    }
    return channelString;
}

void SoundPreviewer::OnSynthChoiceChange()
{
    // Recalculate the mask.
    _device = GetDeviceFromComboHelper(m_wndSynths);
    if (_sound.get())
    {
        std::string channelText;
        _wChannelMask = _sound->GetChannelMask(_device);
        for (int i = 0; i < 16; i++)
        {
            channelText += ((_wChannelMask >> i) & 0x1)? "1" : "0";
        }
        m_wndChannels.SetWindowText(channelText.c_str());
       
    }
}

void SoundPreviewer::OnMIDIDeviceChange()
{
    // TODO
}
void SoundPreviewer::_UpdatePlayState()
{
    if (_fPlaying)
    {
        SetTimer(SOUND_TIMER, 100, NULL);
    }
    else
    {
        KillTimer(SOUND_TIMER);
    }
    m_wndPlay.EnableWindow(_sound.get() && !_fPlaying);
    m_wndStop.EnableWindow(_sound.get() && _fPlaying);
}
void SoundPreviewer::OnPlay()
{
    g_midiPlayer.Play();
    _fPlaying = true;
    _UpdatePlayState();
}
void SoundPreviewer::OnStop()
{
    g_midiPlayer.Stop();
    _fPlaying = false;
    _UpdatePlayState();
}
void SoundPreviewer::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == SOUND_TIMER)
    {
        if (_fPlaying)
        {
            m_wndSlider.SetPos(g_midiPlayer.QueryPosition(100));
        }
    }
    else
    {
        __super::OnTimer(nIDEvent);
    }
}

//
// Blank previewer
//
BEGIN_MESSAGE_MAP(BlankPreviewer, ResourcePreviewer)
END_MESSAGE_MAP()


