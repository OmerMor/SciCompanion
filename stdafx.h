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
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
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

#ifdef _AFXDLL
fwef
#define BEGIN_TEMPLATE_MESSAGE_MAP(theTemplArgs, theClass, baseClass)              template <theTemplArgs>                                                        const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap()                             { return &theClass::messageMap; }                                              template <theTemplArgs>                                                        const AFX_MSGMAP* theClass::GetMessageMap() const                              { return &theClass::messageMap; }                                              template <theTemplArgs>                                                        const AFX_MSGMAP theClass::messageMap =                                        { &baseClass::GetThisMessageMap, &theClass::_messageEntries[0] };              template <theTemplArgs>                                                        const AFX_MSGMAP_ENTRY theClass::_messageEntries[] =                           {                                                                      
#else
#define BEGIN_TEMPLATE_MESSAGE_MAP(theTemplArgs, theClass, baseClass)              template <theTemplArgs>                                                        const AFX_MSGMAP* theClass::GetMessageMap() const                                      { return &theClass::messageMap; }                                              template <theTemplArgs>                                                        AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap =             { &baseClass::messageMap, &theClass::_messageEntries[0] };                     template <theTemplArgs>                                                        AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] =                {                                                                      
#endif

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


