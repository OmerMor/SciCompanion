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

#if (!defined __PROF_SKIN_H)
#define __PROF_SKIN_H

// __PROF_SKIN_VERSION_DWORD is required for static library builds
#define __PROF_SKIN_VERSION_DWORD     __PROF_UIS_VERSION_DWORD

// Prof-Skin library

#if (defined _AFXDLL)
	#if (defined __STATPROFUIS_WITH_DLLMFC__)

		#if (defined __PROF_UIS_FOR_REGULAR_DLL)
			#define __PROF_SKIN_PERFORMS_STATE_MANAGING
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292yud-RDE.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292ymd-RDE.lib"
				#else
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292ynd-RDE.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292yu-RDE.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292ym-RDE.lib"
				#else
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292yn-RDE.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#else
			#define __PROF_SKIN_MANAGE_STATE AFX_MANAGE_STATE( ::AfxGetAppModuleState() )
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292yud.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292ymd.lib"
				#else
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292ynd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292yu.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292ym.lib"
				#else
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292yn.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#endif // else from (defined __PROF_UIS_FOR_REGULAR_DLL)

	#else

		#define __PROF_SKIN_PERFORMS_STATE_MANAGING

#if (defined __PROF_SKIN_IMPL__)
		#define __PROF_SKIN_STATE_MANAGER_DECL _declspec(dllexport)
#else
		#define __PROF_SKIN_STATE_MANAGER_DECL _declspec(dllimport)
#endif // (defined __PROF_SKIN_IMPL__)

		#if (defined __PROF_UIS_FOR_REGULAR_DLL)
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292ud-RDE.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292ud-RDE.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292md-RDE.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292md-RDE.lib"
				#else
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292nd-RDE.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292nd-RDE.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292u-RDE.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292u-RDE.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292m-RDE.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292m-RDE.lib"
				#else
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292n-RDE.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292n-RDE.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#else
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292ud.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292ud.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292md.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292md.lib"
				#else
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292nd.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292nd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292u.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292u.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292m.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292m.lib"
				#else
					#define __PROF_SKIN_MODULE_NAME	_T("ProfSkin292n.dll")
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292n.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#endif // else from (defined __PROF_UIS_FOR_REGULAR_DLL)
	#endif // __STATPROFUIS_WITH_DLLMFC__
#else
		#if (defined __PROF_UIS_FOR_REGULAR_DLL)
			#define __PROF_SKIN_PERFORMS_STATE_MANAGING
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292sud-RDE.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292smd-RDE.lib"
				#else
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292snd-RDE.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292su-RDE.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292sm-RDE.lib"
				#else
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292sn-RDE.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#else
			#define __PROF_SKIN_MANAGE_STATE AFX_MANAGE_STATE( ::AfxGetAppModuleState() )
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292sud.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292smd.lib"
				#else
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292snd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292su.lib"
				#elif (defined _MBCS)
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292sm.lib"
				#else
					#define __PROF_SKIN_LIB_NAME		   "ProfSkin292sn.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#endif // else from (defined __PROF_UIS_FOR_REGULAR_DLL)
#endif // _AFXDLL

#if (defined __PROF_SKIN_IMPL__)
	#if (!defined __EXT_PROFUIS_STATIC_LINK)
		#define __PROF_SKIN_API _declspec(dllexport)
	#else
		#define __PROF_SKIN_API
	#endif // _AFXDLL
#else 
	#if (!defined __EXT_PROFUIS_STATIC_LINK)
		#define __PROF_SKIN_API _declspec(dllimport)
	#else
		#define __PROF_SKIN_API
	#endif // _AFXDLL

	#pragma message("   Automatically linking with Prof-SKIN library: " __PROF_SKIN_LIB_NAME )
	#pragma message("      (Skin support module for Prof-UIS)")
	#pragma comment( lib, __PROF_SKIN_LIB_NAME ) 
#endif

#if (defined __PROF_SKIN_PERFORMS_STATE_MANAGING)

	#if (! defined __PROF_SKIN_STATE_MANAGER_DECL)
		#define __PROF_SKIN_STATE_MANAGER_DECL
	#endif

	struct AFX_MAINTAIN_STATE2;
	class AFX_MODULE_STATE;

	class __PROF_SKIN_STATE_MANAGER_DECL CExt_ProfSkin_ModuleState
	{
		static AFX_MODULE_STATE * g_pExternalModuleState;
		AFX_MAINTAIN_STATE2 * m_pAfxState;
	public:
		CExt_ProfSkin_ModuleState();
		virtual ~CExt_ProfSkin_ModuleState();
		static void InitExtension(
			AFX_MODULE_STATE * pExternalModuleState = NULL
			);
	}; // class CExt_ProfSkin_ModuleState
	#define __PROF_SKIN_MANAGE_STATE CExt_ProfSkin_ModuleState the_ProfSkin_ModuleState

#endif // __PROF_SKIN_PERFORMS_STATE_MANAGING

#if (!defined __PROF_UIS_H)
	#include <Prof-UIS.h>
#endif

// Prof-UIS native unicode character type support for VC++ .NET 2002 or later
#if (!defined __ExtMfcSafeNativeTCHAR_H)
	#include <ExtMfcSafeNativeTCHAR.h>
#endif

#if (!defined __PROF_SKIN_BITMAP_H)
	#include <../ProfSkin/ExtSkinBitmap.h>
#endif

#if (!defined __PROF_SKIN_PAINT_MANAGER_H)
	#include <../ProfSkin/ExtSkinPaintManager.h>
#endif

#if (!defined __PROF_SKIN_ITEM_H)
	#include <../ProfSkin/ExtSkinItem.h>
#endif

#if (! defined __PROF_SKIN_IMPL__)

	#if (defined _AFXDLL)

		#if (defined __STATPROFUIS_WITH_DLLMFC__)

			#ifdef _DEBUG

				#pragma message("   Automatically linking with LibPNGDLLCRT-d.lib")
				#pragma message("      (PNG image format support)")
				#pragma comment(lib,"LibPNGDLLCRT-d.lib")

				#pragma message("   Automatically linking with ZLibDLLCRT-d.lib")
				#pragma message("      (ZIP compression support required by LibPNG)")
				#pragma comment(lib,"ZLibDLLCRT-d.lib")

			#else

				#pragma message("   Automatically linking with LibPNGDLLCRT.lib")
				#pragma message("      (PNG image format support)")
				#pragma comment(lib,"LibPNGDLLCRT.lib")

				#pragma message("   Automatically linking with ZLibDLLCRT.lib")
				#pragma message("      (ZIP compression support used by LibPNG)")
				#pragma comment(lib,"ZLibDLLCRT.lib")

			#endif

		#endif //(defined __STATPROFUIS_WITH_DLLMFC__)

	#else // (defined _AFXDLL)

			#ifdef _DEBUG

				#pragma message("   Automatically linking with LibPNGLIBCRT-d.lib")
				#pragma message("      (PNG image format support)")
				#pragma comment(lib,"LibPNGLIBCRT-d.lib")

				#pragma message("   Automatically linking with ZLibLIBCRT-d.lib")
				#pragma message("      (ZIP compression support required by LibPNG)")
				#pragma comment(lib,"ZLibLIBCRT-d.lib")

			#else

				#pragma message("   Automatically linking with LibPNGLIBCRT.lib")
				#pragma message("      (PNG image format support)")
				#pragma comment(lib,"LibPNGLIBCRT.lib")

				#pragma message("   Automatically linking with ZLibLIBCRT.lib")
				#pragma message("      (ZIP compression support used by LibPNG)")
				#pragma comment(lib,"ZLibLIBCRT.lib")

			#endif

	#endif // else from (defined _AFXDLL)

#endif // (! defined __PROF_SKIN_IMPL__)

#endif /// __PROF_SKIN_H

