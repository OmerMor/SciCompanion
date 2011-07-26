// ScriptFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ScriptFrame.h"
#include "SCIPicEditor.h"
#include "VocabAutoCompleteDialog.h"
#include "crysedit/editcmd.h"

// CVocabChildFrame

IMPLEMENT_DYNCREATE(CScriptFrame, CMDITabChildWnd)

CScriptFrame::CScriptFrame()
{
    theApp.SetScriptFrame(this);
}

CScriptFrame::~CScriptFrame()
{
    theApp.SetScriptFrame(NULL);
}


BEGIN_MESSAGE_MAP(CScriptFrame, CMDITabChildWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()



// CVocabChildFrame message handlers


int CScriptFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    EnableDocking(CBRS_ALIGN_ANY);

    //m_wndStatusBar.SetPane0Icon(IDI_EXCLAMATION);

    return 0;
}

