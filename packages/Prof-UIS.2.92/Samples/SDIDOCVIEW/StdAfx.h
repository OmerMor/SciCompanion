// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CC6465E0_E460_4D2A_BD3E_402D69655378__INCLUDED_)
#define AFX_STDAFX_H__CC6465E0_E460_4D2A_BD3E_402D69655378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _SECURE_ATL 1

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


/////////////////////////////////////////////////////////////////
//
// Prof-UIS API
//
/////////////////////////////////////////////////////////////////

#if (!defined __PROF_UIS_H)
	#include <Prof-UIS.h>
#endif // (!defined __PROF_UIS_H)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CC6465E0_E460_4D2A_BD3E_402D69655378__INCLUDED_)
