// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2011 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// mailto:support@prof-uis.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#include "stdafx.h"

#if (defined _AFXDLL) && (! defined __STATPROFUIS_WITH_DLLMFC__)

	#include <afxdllx.h>

	#ifndef __AFXSTATE_H__
		#include <AfxStat_.h>
	#endif

#endif // (defined _AFXDLL) && (! defined __STATPROFUIS_WITH_DLLMFC__)

#include "ProfSkin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (defined _AFXDLL) && (! defined __STATPROFUIS_WITH_DLLMFC__)

static AFX_EXTENSION_MODULE the_ProfSkin_ExtensionModule =
{
	NULL,
	NULL
};

extern "C" int APIENTRY DllMain(
	HINSTANCE hInstance,
	DWORD dwReason,
	LPVOID lpReserved
	)
{
	lpReserved;
	if( dwReason == DLL_PROCESS_ATTACH )
	{
		TRACE3(
			"   * * * INITIALIZING DYNAMIC LIBRARY: ProfSkin version %d.%d%d * * * \n",
			((__PROF_SKIN_VERSION_DWORD >> 24) & 0XFF),
			((__PROF_SKIN_VERSION_DWORD >> 16) & 0XFF),
			((__PROF_SKIN_VERSION_DWORD >>  8) & 0XFF)
			);
		if( ! AfxInitExtensionModule(
				the_ProfSkin_ExtensionModule,
				hInstance
				)
			)
			return 0;
#if (!defined __PROF_UIS_FOR_REGULAR_DLL)
//		new CDynLinkLibrary( the_ProfSkin_ExtensionModule );
		CExt_ProfSkin_ModuleState::InitExtension();
#endif // (!defined __PROF_UIS_FOR_REGULAR_DLL)
	} // if( dwReason == DLL_PROCESS_ATTACH )
	else if( dwReason == DLL_PROCESS_DETACH )
	{
		TRACE3(
			"   * * * TERMINATING DYNAMIC LIBRARY: ProfSkin version %d.%d%d * * * \n",
			((__PROF_SKIN_VERSION_DWORD >> 24) & 0XFF),
			((__PROF_SKIN_VERSION_DWORD >> 16) & 0XFF),
			((__PROF_SKIN_VERSION_DWORD >>  8) & 0XFF)
			);
		AfxTermExtensionModule( the_ProfSkin_ExtensionModule );
	} // else if( dwReason == DLL_PROCESS_DETACH )
	return 1;   // ok
}
#endif // (defined _AFXDLL) && (! defined __STATPROFUIS_WITH_DLLMFC__)

#if (defined __PROF_SKIN_PERFORMS_STATE_MANAGING)

#if _MFC_VER < 0x800
	#ifndef _AFXDLL

		class _AFX_THREAD_STATE;

		struct AFX_MAINTAIN_STATE2
		{
			AFX_MAINTAIN_STATE2(AFX_MODULE_STATE* pModuleState);
			~AFX_MAINTAIN_STATE2();

		protected:
			AFX_MODULE_STATE* m_pPrevModuleState;
			_AFX_THREAD_STATE* m_pThreadState;
		};

		AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2(AFX_MODULE_STATE* pNewState)
		{
			m_pThreadState = _afxThreadState;
			m_pPrevModuleState = m_pThreadState->m_pModuleState;
			m_pThreadState->m_pModuleState = pNewState;
		}

		AFX_MAINTAIN_STATE2::~AFX_MAINTAIN_STATE2()
		{
			m_pThreadState->m_pModuleState = m_pPrevModuleState;
		}

	#endif //_AFXDLL
#endif // _MFC_VER < 0x800

AFX_MODULE_STATE * CExt_ProfSkin_ModuleState::g_pExternalModuleState = NULL;

CExt_ProfSkin_ModuleState::CExt_ProfSkin_ModuleState()
{
	m_pAfxState = NULL;
	if( g_pExternalModuleState != NULL )
		m_pAfxState = new AFX_MAINTAIN_STATE2( g_pExternalModuleState );

}

CExt_ProfSkin_ModuleState::~CExt_ProfSkin_ModuleState()
{
	if( m_pAfxState != NULL )
		delete m_pAfxState;
}

void CExt_ProfSkin_ModuleState::InitExtension(
	AFX_MODULE_STATE * pExternalModuleState // = NULL
	)
{
#if (defined _AFXDLL) && (! defined __STATPROFUIS_WITH_DLLMFC__)
//	#if (defined __PROF_UIS_FOR_REGULAR_DLL)
		new CDynLinkLibrary( the_ProfSkin_ExtensionModule );
//	#endif
#endif
	g_pExternalModuleState = pExternalModuleState;
}

#endif // (defined __PROF_SKIN_PERFORMS_STATE_MANAGING)


