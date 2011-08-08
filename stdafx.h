// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components

#include <afxext.h>         // MFC extensions

#include <afxcview.h>       // CListView

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

#include <afxpriv.h> // For WM_IDLEUPDATECMDUI, WM_SIZEPARENT

#include <afxole.h> // For crystal edit


#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
#include <shlwapi.h>

#include <gdiplus.h>
using namespace Gdiplus;
#include <Gdipluspixelformats.h>
#include <afxdlgs.h>

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <iostream>
#include <functional>
#include <fstream>
#include <iomanip>

#include <hash_map>

// Deprecated warnings caused by shlwapi
#pragma warning(disable: 4995)


// for deleting pointers in some containers:
struct delete_object
{
    template<typename T>
    void operator()(const T* ptr) const
    {
        delete ptr;
    }
};

// for deleting pointers to arrays in some containers:
struct delete_object_array
{
    template<typename T>
    void operator()(const T* ptr) const
    {
        delete [] ptr;
    }
};

// for deleting values in a map:
struct delete_map_value
{
    template<typename TKEY, typename TVALUE>
    void operator()(const std::pair<TKEY, TVALUE> &ptr) const
    {
        delete ptr.second;
    }
};




//
// Some crap we need - scoped_ptr from boost
//
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:  // emphasize the following members are private
    noncopyable( const noncopyable& );
    const noncopyable& operator=( const noncopyable& );
};

template<class T> class scoped_ptr : noncopyable
{
private:
    T* _ptr;
    scoped_ptr(scoped_ptr const &);
    scoped_ptr & operator=(scoped_ptr const &);
    typedef scoped_ptr<T> this_type;

public:
    typedef T element_type;

    explicit scoped_ptr(T * p = 0) : _ptr(p) {} // never throws
    ~scoped_ptr() { delete _ptr; } // never throws

    void reset(T * p = 0) // never throws
    {
        this_type(p).swap(*this);
    }

    T & operator*() const // never throws
    {
        return *_ptr;
    }
    T * operator->() const // never throws
    {
        return _ptr;
    }
    T * get() const // never throws
    {
        return _ptr;
    }
    
    void swap(scoped_ptr & b) // never throws
    {
        T *tmp = b._ptr;
        b._ptr = _ptr;
        _ptr = tmp;
    }
};

template<class T> void swap(scoped_ptr<T> & a, scoped_ptr<T> & b) // never throws
{
    a.swap(b);
}

template<class T> inline void checked_array_delete(T * x)
{
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete [] x;
}
//  boost's scope_array
//  scoped_array extends scoped_ptr to arrays. Deletion of the array pointed to
//  is guaranteed, either on destruction of the scoped_array or via an explicit
//  reset(). Use shared_array or std::vector if your needs are more complex.
//
template<class T> class scoped_array // noncopyable
{
private:

    T * ptr;

    scoped_array(scoped_array const &);
    scoped_array & operator=(scoped_array const &);

    typedef scoped_array<T> this_type;

public:

    typedef T element_type;

    explicit scoped_array(T * p = 0) : ptr(p) // never throws
    {
    }

    ~scoped_array() // never throws
    {
        checked_array_delete(ptr);
    }

    void reset(T * p = 0) // never throws
    {
        ASSERT(p == 0 || p != ptr); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T & operator[](std::ptrdiff_t i) const // never throws
    {
        ASSERT(ptr != 0);
        ASSERT(i >= 0);
        return ptr[i];
    }

    T * get() const // never throws
    {
        return ptr;
    }

    // implicit conversion to "bool"

    typedef T * this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const // never throws
    {
        return ptr == 0? 0: &this_type::ptr;
    }

    bool operator! () const // never throws
    {
        return ptr == 0;
    }

    void swap(scoped_array & b) // never throws
    {
        T * tmp = b.ptr;
        b.ptr = ptr;
        ptr = tmp;
    }

};

template<class T> inline void swap(scoped_array<T> & a, scoped_array<T> & b) // never throws
{
    a.swap(b);
}




#include <afxtempl.h>

//#ifdef _AFXDLL
//fwef
#define BEGIN_TPL_MESSAGE_MAP(theTemplArgs, theClass, baseClass) \
	template <theTemplArgs> \
	const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
{ return &theClass::messageMap; }  \
	template <theTemplArgs> \
	const AFX_MSGMAP* theClass::GetMessageMap() const \
{ return &theClass::messageMap; } \
	template <theTemplArgs> \
	const AFX_MSGMAP theClass::messageMap =  \
{ &baseClass::GetThisMessageMap, &theClass::_messageEntries[0] }; \
	typedef theClass ThisClass; \
	typedef baseClass TheBaseClass; \
	template <theTemplArgs>  \
	const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
{  
//#else
//#define BEGIN_TEMPLATE_MESSAGE_MAP(theTemplArgs, theClass, baseClass)              template <theTemplArgs>                                                        const AFX_MSGMAP* theClass::GetMessageMap() const                                      { return &theClass::messageMap; }                                              template <theTemplArgs>                                                        AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap =             { &baseClass::messageMap, &theClass::_messageEntries[0] };                     template <theTemplArgs>                                                        AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] =                {                                                                      
//#endif

#define BEGIN_TPL_MESSAGE_MAP3(theTemplArgs, theClass, baseClass) \
PTM_WARNING_DISABLE \
template <theTemplArgs> \
const AFX_MSGMAP* theClass::GetMessageMap() const \
{ return GetThisMessageMap(); } \
template <theTemplArgs> \
const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
{ \
typedef theClass ThisClass; \
typedef baseClass TheBaseClass; \
static const AFX_MSGMAP_ENTRY _messageEntries[] = \
{

// Additional defines so we can use multi-value templates with BEGIN_TEMPLATE_MESSAGE_MAP
#define TEMPLATE_1(t1)                   t1
#define TEMPLATE_2(t1, t2)               t1, t2
#define TEMPLATE_3(t1 ,t2 ,t3)           t1, t2, t3
#define TCLASS_1(theClass, t1)           theClass<t1>
#define TCLASS_2(theClass, t1, t2)       theClass<t1, t2>
#define TCLASS_3(theClass, t1, t2, t3)   theClass<t1, t2, t3>


#include <Prof-UIS.h>

#ifdef DEBUG
#define DOCSUPPORT
#endif

// Commonly included or large header files:
#include "sci.h"
#include "ResourceData.h"
#include "StlUtil.h"
#include "PicCommands.h"


//#if !defined(AFX_TEMPLDEF_H__742F3281_055B_11D4_B261_00104BB13A66__INCLUDED_)
//#define AFX_TEMPLDEF_H__742F3281_055B_11D4_B261_00104BB13A66__INCLUDED_
// 
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
// 
/////////////////////////////////////////////////////////////////////////////////
//// TemplDef.h: helper macroses for using templates with MFC
////
//// Author: Yury Goltsman
//// email: ygprg@go.to
//// page: http://go.to/ygprg
//// Copyright © 2000, Yury Goltsman
////
//// This code provided "AS IS," without any kind of warranty.
//// You may freely use or modify this code provided this
//// Copyright is included in all derived versions.
////
//// version : 1.1 (18.02.2004)
//// Added support for VS2003(VC++ 7.1)
////
//// version : 1.0
////
// 
/////////////////////////////////////////////////////////////////////////////////
//// common definitions for any map:
// 
//// use it to specify list of arguments for class and as theTemplArgs
//// e.g. BEGIN_TEMPLATE_MESSAGE_MAP(ARGS2(class base_class, int max),
//// CMyTClass,
//// base_class)
// 
#define ARGS2(arg1, arg2) arg1, arg2
#define ARGS3(arg1, arg2, arg3) arg1, arg2, arg3
#define ARGS4(arg1, arg2, arg3, arg4) arg1, arg2, arg3, arg4
#define ARGS5(arg1, arg2, arg3, arg4, arg5) arg1, arg2, arg3, arg4, arg5
// 
/////////////////////////////////////////////////////////////////////////////////
//// definition for MESSAGE_MAP:
// 
//#define DECLARE_TEMPLATE_MESSAGE_MAP() DECLARE_MESSAGE_MAP()
// 
//
//#if ( _MSC_VER >= 1400) // VC++ 8.0 (.net)
// 
////#ifdef _AFXDLL
#define BEGIN_TPL_MESSAGE_MAP2(theTemplArgs, theClass, baseClass) \
PTM_WARNING_DISABLE \
template \
const AFX_MSGMAP* theClass::GetMessageMap() const \
{ return GetThisMessageMap(); } \
template \
const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
{ \
typedef theClass ThisClass; \
typedef baseClass TheBaseClass; \
static const AFX_MSGMAP_ENTRY _messageEntries[] = \
{\
 
//#else
// 
//#if ( _MSC_VER >= 1300) // VC++ 7.0 (.net) 
// 
//
//#ifdef _AFXDLL
//#define BEGIN_TEMPLATE_MESSAGE_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
//{ return &theClass::messageMap; } \
//template \
//const AFX_MSGMAP* theClass::GetMessageMap() const \
//{ return &theClass::messageMap; } \
//template \
//const AFX_MSGMAP theClass::messageMap = \
//{ &baseClass::GetThisMessageMap, &theClass::_messageEntries[0] }; \
//template \
//const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
//{ \
// 
//#else
//#define BEGIN_TEMPLATE_MESSAGE_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_MSGMAP* theClass::GetMessageMap() const \
//{ return &theClass::messageMap; } \
//template \
//AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
//{ &baseClass::messageMap, &theClass::_messageEntries[0] }; \
//template \
//AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
//{ \
// 
//#endif
// 
//#else
// 
//
//// Old VC6.0
// 
//#ifdef _AFXDLL
//#define BEGIN_TEMPLATE_MESSAGE_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_MSGMAP* PASCAL theClass::_GetBaseMessageMap() \
//{ return &baseClass::messageMap; } \
//template \
//const AFX_MSGMAP* theClass::GetMessageMap() const \
//{ return &theClass::messageMap; } \
//template \
//AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
//{ &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
//template \
//AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
//{ \
// 
//#else
//#define BEGIN_TEMPLATE_MESSAGE_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_MSGMAP* theClass::GetMessageMap() const \
//{ return &theClass::messageMap; } \
//template \
//AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
//{ &baseClass::messageMap, &theClass::_messageEntries[0] }; \
//template \
//AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
//{ \
// 
//#endif
// 
//#endif
// 
//#endif
//#define END_TEMPLATE_MESSAGE_MAP() END_MESSAGE_MAP()
// 
//
/////////////////////////////////////////////////////////////////////////////////
//// definition for OLECMD_MAP:
// 
//#define DECLARE_TEMPLATE_OLECMD_MAP() DECLARE_OLECMD_MAP()
// 
//#ifdef _AFXDLL
//#define BEGIN_TEMPLATE_OLECMD_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_OLECMDMAP* PASCAL theClass::_GetBaseCommandMap() \
//{ return &baseClass::commandMap; } \
//template \
//const AFX_OLECMDMAP* theClass::GetCommandMap() const \
//{ return &theClass::commandMap; } \
//template \
//AFX_COMDAT AFX_DATADEF const AFX_OLECMDMAP theClass::commandMap = \
//{ &theClass::_GetBaseCommandMap, &theClass::_commandEntries[0] }; \
//template \
//AFX_COMDAT const AFX_OLECMDMAP_ENTRY theClass::_commandEntries[] = \
//{ \
// 
//#else
//#define BEGIN_TEMPLATE_OLECMD_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_OLECMDMAP* theClass::GetCommandMap() const \
//{ return &theClass::commandMap; } \
//template \
//AFX_COMDAT AFX_DATADEF const AFX_OLECMDMAP theClass::commandMap = \
//{ &baseClass::commandMap, &theClass::_commandEntries[0] }; \
//template \
//AFX_COMDAT const AFX_OLECMDMAP_ENTRY theClass::_commandEntries[] = \
//{ \
// 
//#endif
// 
//#define END_TEMPLATE_OLECMD_MAP() END_OLECMD_MAP()
// 
/////////////////////////////////////////////////////////////////////////////////
//// definition for INTERFACE_MAP:
// 
//#define DECLARE_TEMPLATE_INTERFACE_MAP() DECLARE_INTERFACE_MAP()
// 
//#ifdef _AFXDLL
//#define BEGIN_TEMPLATE_INTERFACE_MAP(theTemplArgs, theClass, theBase) \
//template \
//const AFX_INTERFACEMAP* PASCAL theClass::_GetBaseInterfaceMap() \
//{ return &theBase::interfaceMap; } \
//template \
//const AFX_INTERFACEMAP* theClass::GetInterfaceMap() const \
//{ return &theClass::interfaceMap; } \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_INTERFACEMAP theClass::interfaceMap = \
//{ &theClass::_GetBaseInterfaceMap, &theClass::_interfaceEntries[0], }; \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_INTERFACEMAP_ENTRY theClass::_interfaceEntries[] = \
//{ \
// 
//#else
//#define BEGIN_TEMPLATE_INTERFACE_MAP(theTemplArgs, theClass, theBase) \
//template \
//const AFX_INTERFACEMAP* theClass::GetInterfaceMap() const \
//{ return &theClass::interfaceMap; } \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_INTERFACEMAP theClass::interfaceMap = \
//{ &theBase::interfaceMap, &theClass::_interfaceEntries[0], }; \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_INTERFACEMAP_ENTRY theClass::_interfaceEntries[] = \
//{ \
// 
//#endif
// 
//#define END_TEMPLATE_INTERFACE_MAP() END_INTERFACE_MAP()
// 
/////////////////////////////////////////////////////////////////////////////////
//// definition for DISPATCH_MAP:
// 
//#define DECLARE_TEMPLATE_DISPATCH_MAP() DECLARE_DISPATCH_MAP()
// 
//#ifdef _AFXDLL
//#define BEGIN_TEMPLATE_DISPATCH_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_DISPMAP* PASCAL theClass::_GetBaseDispatchMap() \
//{ return &baseClass::dispatchMap; } \
//template \
//const AFX_DISPMAP* theClass::GetDispatchMap() const \
//{ return &theClass::dispatchMap; } \
//template \
//AFX_COMDAT const AFX_DISPMAP theClass::dispatchMap = \
//{ &theClass::_GetBaseDispatchMap, &theClass::_dispatchEntries[0], \
//&theClass::_dispatchEntryCount, &theClass::_dwStockPropMask }; \
//template \
//AFX_COMDAT UINT theClass::_dispatchEntryCount = (UINT)-1; \
//template \
//AFX_COMDAT DWORD theClass::_dwStockPropMask = (DWORD)-1; \
//template \
//AFX_COMDAT const AFX_DISPMAP_ENTRY theClass::_dispatchEntries[] = \
//{ \
// 
//#else
//#define BEGIN_TEMPLATE_DISPATCH_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_DISPMAP* theClass::GetDispatchMap() const \
//{ return &theClass::dispatchMap; } \
//template \
//AFX_COMDAT const AFX_DISPMAP theClass::dispatchMap = \
//{ &baseClass::dispatchMap, &theClass::_dispatchEntries[0], \
//&theClass::_dispatchEntryCount, &theClass::_dwStockPropMask }; \
//template \
//AFX_COMDAT UINT theClass::_dispatchEntryCount = (UINT)-1; \
//template \
//AFX_COMDAT DWORD theClass::_dwStockPropMask = (DWORD)-1; \
//template \
//AFX_COMDAT const AFX_DISPMAP_ENTRY theClass::_dispatchEntries[] = \
//{ \
// 
//#endif
// 
//#define END_TEMPLATE_DISPATCH_MAP() END_DISPATCH_MAP()
// 
/////////////////////////////////////////////////////////////////////////////////
//// definition for CONNECTION_MAP:
// 
//#define DECLARE_TEMPLATE_CONNECTION_MAP() DECLARE_CONNECTION_MAP()
// 
//#ifdef _AFXDLL
//#define BEGIN_TEMPLATE_CONNECTION_MAP(theTemplArgs, theClass, theBase) \
//template \
//const AFX_CONNECTIONMAP* PASCAL theClass::_GetBaseConnectionMap() \
//{ return &theBase::connectionMap; } \
//template \
//const AFX_CONNECTIONMAP* theClass::GetConnectionMap() const \
//{ return &theClass::connectionMap; } \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_CONNECTIONMAP theClass::connectionMap = \
//{ &theClass::_GetBaseConnectionMap, &theClass::_connectionEntries[0], }; \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_CONNECTIONMAP_ENTRY theClass::_connectionEntries[] = \
//{ \
// 
//#else
//#define BEGIN_TEMPLATE_CONNECTION_MAP(theTemplArgs, theClass, theBase) \
//template \
//const AFX_CONNECTIONMAP* theClass::GetConnectionMap() const \
//{ return &theClass::connectionMap; } \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_CONNECTIONMAP theClass::connectionMap = \
//{ &(theBase::connectionMap), &theClass::_connectionEntries[0], }; \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_CONNECTIONMAP_ENTRY theClass::_connectionEntries[] = \
//{ \
// 
//#endif
// 
//#define END_TEMPLATE_CONNECTION_MAP() END_CONNECTION_MAP()
// 
/////////////////////////////////////////////////////////////////////////////////
//// definition for EVENTSINK_MAP:
// 
//#define DECLARE_TEMPLATE_EVENTSINK_MAP() DECLARE_EVENTSINK_MAP()
// 
//#ifdef _AFXDLL
//#define BEGIN_TEMPLATE_EVENTSINK_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_EVENTSINKMAP* PASCAL theClass::_GetBaseEventSinkMap() \
//{ return &baseClass::eventsinkMap; } \
//template \
//const AFX_EVENTSINKMAP* theClass::GetEventSinkMap() const \
//{ return &theClass::eventsinkMap; } \
//template \
//const AFX_EVENTSINKMAP theClass::eventsinkMap = \
//{ &theClass::_GetBaseEventSinkMap, &theClass::_eventsinkEntries[0], \
//&theClass::_eventsinkEntryCount }; \
//template \
//UINT theClass::_eventsinkEntryCount = (UINT)-1; \
//template \
//const AFX_EVENTSINKMAP_ENTRY theClass::_eventsinkEntries[] = \
//{ \
// 
//#else
//#define BEGIN_TEMPLATE_EVENTSINK_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_EVENTSINKMAP* theClass::GetEventSinkMap() const \
//{ return &theClass::eventsinkMap; } \
//template \
//const AFX_EVENTSINKMAP theClass::eventsinkMap = \
//{ &baseClass::eventsinkMap, &theClass::_eventsinkEntries[0], \
//&theClass::_eventsinkEntryCount }; \
//template \
//UINT theClass::_eventsinkEntryCount = (UINT)-1; \
//template \
//const AFX_EVENTSINKMAP_ENTRY theClass::_eventsinkEntries[] = \
//{ \
// 
//#endif
// 
//#define END_TEMPLATE_EVENTSINK_MAP() END_EVENTSINK_MAP()
// 
/////////////////////////////////////////////////////////////////////////////////
//// definition for EVENT_MAP:
// 
//#define DECLARE_TEMPLATE_EVENT_MAP() DECLARE_EVENT_MAP()
// 
//#define BEGIN_TEMPLATE_EVENT_MAP(theTemplArgs, theClass, baseClass) \
//template \
//const AFX_EVENTMAP* theClass::GetEventMap() const \
//{ return &eventMap; } \
//template \
//const AFX_DATADEF AFX_EVENTMAP theClass::eventMap = \
//{ &(baseClass::eventMap), theClass::_eventEntries, \
//&theClass::_dwStockEventMask }; \
//template \
//AFX_COMDAT DWORD theClass::_dwStockEventMask = (DWORD)-1; \
//template \
//AFX_COMDAT const AFX_DATADEF AFX_EVENTMAP_ENTRY theClass::_eventEntries[] = \
//{
// 
//#define END_TEMPLATE_EVENT_MAP() END_EVENT_MAP()
// 
//
//#endif // !defined(AFX_TEMPLDEF_H__742F3281_055B_11D4_B261_00104BB13A66__INCLUDED_) 