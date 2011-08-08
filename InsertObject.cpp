// InsertObject.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "InsertObject.h"
#include "Compile\ScriptOMAll.h"
#include "ScriptMakerHelper.h"

using namespace sci;
using namespace std;

const TCHAR *g_szObjects[] =
{
    "Prop",   
    "Act",
    "Door",
    "Rgn",
    "Locale",
    "Script",
};

enum Species
{
    SpeciesProp = 0,
    SpeciesAct = 1,
    SpeciesDoor = 2,
    SpeciesRgn = 3,
    SpeciesLocale = 4,
    SpeciesScript = 5,
};

void _AddProperties(ClassDefinition &instance, Species species, WORD wx, WORD wy, WORD wView)
{
    switch(species)
    {
    case SpeciesProp:
    case SpeciesAct:
    case SpeciesDoor:
        instance.AddProperty(new ClassProperty("x", wx));
        instance.AddProperty(new ClassProperty("y", wy));
        instance.AddProperty(new ClassProperty("view", wView));
        if (species == SpeciesDoor)
        {
            // Add a few more
            instance.AddProperty(new ClassProperty("entranceTo", "ROOMNUM"));
            instance.AddProperty(new ClassProperty("locked", "TRUE"));
        }
        break;
    default:
        // For now, other species don't get any...
        break;
    }
}

// CInsertObject dialog
CInsertObject::CInsertObject(LangSyntax lang, CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CInsertObject::IDD, pParent), _lang(lang)
{
}

CInsertObject::~CInsertObject()
{
}

int g_nSelObjectType = 0;

void CInsertObject::_PrepareControls()
{
    // Populate the combo
    for (int i = 0; i < ARRAYSIZE(g_szObjects); i++)
    {
        m_wndComboType.AddString(g_szObjects[i]);
    }

    // Auto-select last used object type.
    m_wndComboType.SetCurSel(g_nSelObjectType);
}

void _MakeTokenStatement(SingleStatement &statement, const string &token)
{
    auto_ptr<ComplexPropertyValue> pValue(new ComplexPropertyValue);
    pValue->SetValue(token, ValueTypeToken);
    statement.SetSyntaxNode(pValue.get());
    pValue.release();
}

void _MakeNumberStatement(SingleStatement &statement, WORD w)
{
    auto_ptr<ComplexPropertyValue> pValue(new ComplexPropertyValue);
    pValue->SetValue(w);
    statement.SetSyntaxNode(pValue.get());
    pValue.release();
}

void _AddAssignment(MethodDefinition &method, const string &lvalueName, const string &assigned)
{
    auto_ptr<Assignment> pEquals(new Assignment);
    pEquals->SetName("=");
    LValue lvalue;
    lvalue.SetName(lvalueName);
    pEquals->SetVariable(lvalue);

    SingleStatement statement;
    _MakeTokenStatement(statement, assigned);
    pEquals->SetStatement1(statement);

    _AddStatement(method, (auto_ptr<SyntaxNode>)pEquals);
}

void _AddBasicSwitch(MethodDefinition &method, const string &switchValue, const string &case0Comments)
{
    auto_ptr<SwitchStatement> pSwitch(new SwitchStatement);

    // Make the switch value and add it to the swtich
    SingleStatement statementSwitchValue;
    _MakeTokenStatement(statementSwitchValue, switchValue);
    pSwitch->SetStatement1(statementSwitchValue);
    
    // Make the case statement and add it to the switch
    auto_ptr<CaseStatement> pCase(new CaseStatement);
    SingleStatement statementZero;
    _MakeNumberStatement(statementZero, 0);
    pCase->SetStatement1(statementZero);
    _AddComment(*pCase, case0Comments);
    pSwitch->AddCase(pCase.get());
    pCase.release();

    // Add the switch to the method
    _AddStatement(method, (auto_ptr<SyntaxNode>)pSwitch);
}



// parameter may be empty.
void _AddSendCall(MethodDefinition &method, const string &objectName, const string &methodName, const string &parameter)
{
    auto_ptr<SendCall> pSend(new SendCall);
    pSend->SetName(objectName);

    // Create the send param to add to the send call
    auto_ptr<SendParam> pParam(new SendParam);
    pParam->SetName(methodName);
    pParam->SetIsMethod(true);

    if (!parameter.empty())
    {
        // Add the parameter to the sendparam.
        auto_ptr<ComplexPropertyValue> pValue(new ComplexPropertyValue);
        pValue->SetValue(parameter, ValueTypeToken);
        _AddStatement(*pParam, (auto_ptr<SyntaxNode>)pValue);
    }

    pSend->AddSendParam(pParam.get());
    pParam.release();
    _AddStatement(method, (auto_ptr<SyntaxNode>)pSend);
}

BOOL CInsertObject::_PrepareBuffer()
{
    // First, some validation.
    int iIndex = m_wndComboType.GetCurSel();
    if (iIndex == CB_ERR)
    {
        AfxMessageBox(TEXT("Please choose an object type."), MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
        return FALSE;
    }
    ASSERT(iIndex < ARRAYSIZE(g_szObjects));

    CString strName;
    m_wndEditName.GetWindowText(strName);
    if (strName.IsEmpty())
    {
        AfxMessageBox(TEXT("Please choose a name for the object."), MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
        return FALSE;
    }

    // The class definition
    ClassDefinition instance;
    instance.SetName((PCTSTR)strName);
    instance.SetSuperClass(g_szObjects[iIndex]);
    instance.SetInstance(true);
    
    // Now the properties.
    _AddProperties(instance, (Species)iIndex, (WORD)theApp._ptFakeEgo.x, (WORD)theApp._ptFakeEgo.y, (WORD)theApp._iView);

    // Before closing off, if they asked for a script, init it on the guy now:
    if (m_wndCheckScript.GetCheck() != 0)
    {
        ClassDefinition instance;
        instance.SetName((PCTSTR)strName);
        instance.SetSuperClass(g_szObjects[iIndex]);
        instance.SetInstance(true);

        // Prepare an init method.
        auto_ptr<MethodDefinition> pMethod(new MethodDefinition);
        pMethod->SetName("init");

        // super.init()
        _AddSendCall(*pMethod, "super", "init", "");

        // self.setScript(***Script)
        string parameter = strName;
        parameter += "Script";
        _AddSendCall(*pMethod, "self", "setScript", parameter);

        // Add the method
        instance.AddMethod(pMethod.get());
        pMethod.release();
    }
    stringstream ss;
    DebugStringOut out(ss, _lang);
    instance.DebugString(out);

    // Did they ask for a script?  Give it now.
    if (m_wndCheckScript.GetCheck() != 0)
    {
        ClassDefinition scriptInstance;
        scriptInstance.SetName((PCTSTR)(strName + "Script"));
        scriptInstance.SetSuperClass("Script");
        scriptInstance.SetInstance(true);

        auto_ptr<MethodDefinition> pMethodDoIt(new MethodDefinition);
        pMethodDoIt->SetName("doit");
        _AddSendCall(*pMethodDoIt, "super", "doit", "");
        _AddComment(*pMethodDoIt, "// code executed each game cycle");
        scriptInstance.AddMethod(pMethodDoIt.get());
        pMethodDoIt.release();

        auto_ptr<MethodDefinition> pMethodHandleEvent(new MethodDefinition);
        pMethodHandleEvent->SetName("handleEvent");
        pMethodHandleEvent->AddParam("pEvent");
        _AddSendCall(*pMethodHandleEvent, "super", "handleEvent", "pEvent");
        _AddComment(*pMethodHandleEvent, "// handle Said's, etc...");
        scriptInstance.AddMethod(pMethodHandleEvent.get());
        pMethodHandleEvent.release();

        auto_ptr<MethodDefinition> pMethodChangeState(new MethodDefinition);
        pMethodChangeState->SetName("changeState");
        pMethodChangeState->AddParam("newState");
        _AddAssignment(*pMethodChangeState, "state", "newState");
        _AddBasicSwitch(*pMethodChangeState, "newState", "// Handle state changes");
        scriptInstance.AddMethod(pMethodChangeState.get());
        pMethodChangeState.release();

        scriptInstance.DebugString(out);
    }
    
    _strBuffer = ss.str().c_str();
    _strBuffer.Replace("\n", "\r\n");

    return TRUE;
}

void CInsertObject::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_EDITNAME, m_wndEditName);
    DDX_Control(pDX, IDC_OBJECTTYPE, m_wndComboType);
    DDX_Control(pDX, IDC_CHECKSCRIPT, m_wndCheckScript);

    // Visuals
    DDX_Control(pDX, IDOK, m_wndOK);
    DDX_Control(pDX, IDCANCEL, m_wndCancel);
    DDX_Control(pDX, IDC_STATIC1, m_wndStatic1);
    DDX_Control(pDX, IDC_STATIC2, m_wndStatic2);

    _PrepareControls();
}

BOOL CInsertObject::OnInitDialog()
{
    __super::OnInitDialog();

    m_wndEditName.SetWindowText(TEXT("<type name here>"));
    m_wndEditName.SetSel(0, -1);
    m_wndEditName.SetFocus();

    ShowSizeGrip(FALSE);

    // the edit control is the first one, so focus will be there.
    return FALSE;
}

void CInsertObject::OnOK()
{
    // Store last used object type index
    g_nSelObjectType = m_wndComboType.GetCurSel();

    if (_PrepareBuffer())
    {
        __super::OnOK(); // Close the dialog
    }
}

BEGIN_MESSAGE_MAP(CInsertObject, CExtResizableDialog)
END_MESSAGE_MAP()


// CInsertObject message handlers
