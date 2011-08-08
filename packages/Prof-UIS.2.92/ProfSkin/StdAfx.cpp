// stdafx.cpp : source file that includes just the standard includes
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

	#ifdef _DEBUG

		#pragma message("   Prof-Skin is automatically linking with LibPNGDLLCRT-d.lib")
		#pragma message("      (PNG image format support)")
		#pragma comment(lib,"LibPNGDLLCRT-d.lib")

		#pragma message("   Prof-Skin is automatically linking with ZLibDLLCRT-d.lib")
		#pragma message("      (ZIP compression support required by LibPNG)")
		#pragma comment(lib,"ZLibDLLCRT-d.lib")

	#else

		#pragma message("   Prof-Skin is automatically linking with LibPNGDLLCRT.lib")
		#pragma message("      (PNG image format support)")
		#pragma comment(lib,"LibPNGDLLCRT.lib")

		#pragma message("   Prof-Skin is automatically linking with ZLibDLLCRT.lib")
		#pragma message("      (ZIP compression support used by LibPNG)")
		#pragma comment(lib,"ZLibDLLCRT.lib")

	#endif // _DEBUG

#endif // (defined _AFXDLL) && (! defined __STATPROFUIS_WITH_DLLMFC__)

