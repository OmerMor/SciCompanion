#include "stdafx.h"
#include "SCIPicEditor.h"
#include "Compile\ScriptOM.h"
#include "ClassBrowser.h"
#include "ClassBrowserInfo.h"

using namespace sci;

SCIClassBrowserNode::~SCIClassBrowserNode()
{
    // Don't delete all our children - they are not owned by us.
}

//
// Scan this guy, and all its superclasses, for methods.
// Put the methods in map.
//
int SCIClassBrowserNode::ComputeAllMethods(MethodVector &array) const
{
    int iIndexSubclassBegin = 0;
    if (_pClass)
    {
        const MethodVector &methods = _pClass->GetMethods();
        for (size_t i = 0; i < methods.size(); i++)
        {
            MethodDefinition *pMethod = methods[i];
            ASSERT(pMethod);
            
            // Is this method already in the map?
            if (!matches_name(array.begin(), array.end(), pMethod->GetName()))
            {
                // Nope.  Add it.
                array.push_back(pMethod);
            }
            // else yes.  Subclasses win, so don't do anything here
        }
        iIndexSubclassBegin = (int)array.size();
        // Now go to our parent.
        if (_pSuperClass)
        {
            _pSuperClass->ComputeAllMethods(array);
        }
    }
    return iIndexSubclassBegin;
}

int SCIClassBrowserNode::ComputeAllProperties(ClassPropertyVector &array) const
{
    int iIndexSubclassBegin = 0;
    if (_pClass)
    {
        // STLCLEANUP
        const ClassPropertyVector &props = _pClass->GetProperties();
        for (size_t i = 0; i < props.size(); i++)
        {
            ClassProperty *pProp = props[i];
            ASSERT(pProp);
            
            // Is this method already in the map?
            if (!matches_name(array.begin(), array.end(), pProp->GetName()))
            {
                // Nope.  Add it.
                array.push_back(pProp);
            }
            // else yes.  Subclasses win, so don't do anything here
        }
        iIndexSubclassBegin = (int)array.size();
        // Now go to our parent.
        if (_pSuperClass)
        {
            _pSuperClass->ComputeAllProperties(array);
        }
    }
    return iIndexSubclassBegin;
}


void SCIClassBrowserNode::RemoveSubClass(SCIClassBrowserNode* pSubClass)
{
    // We should always find it.
    // Also, assume there is only one.
    _subClasses.erase(find(_subClasses.begin(), _subClasses.end(), pSubClass));
}
