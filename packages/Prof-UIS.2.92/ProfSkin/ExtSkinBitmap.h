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

#if (!defined __PROF_SKIN_BITMAP_H)
#define __PROF_SKIN_BITMAP_H

#if (!defined __PROF_SKIN_H)
	#include <../ProfSkin/ProfSkin.h>
#endif

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

class __PROF_SKIN_API CExtSkinBitmap : public CExtBitmapCache
{
public:
	CExtSkinBitmap();
	CExtSkinBitmap(
		const CExtBitmap & other
		);
	virtual ~CExtSkinBitmap();
	virtual bool AssignFromOther(
		const CExtBitmap & other,
		bool bEnableThrowException = false
		);

	virtual void Empty();
	virtual bool IsEmpty() const;

	bool LoadImage_File(
		__EXT_MFC_SAFE_LPCTSTR strFilePath,
		bool bMake32 = false,
		bool bEnableThrowException = false
		);

	bool LoadPNG_Resource(
		HINSTANCE hInst,
		HRSRC hRsrc,
		bool bMake32 = false,
		bool bEnableThrowException = false
		);
	bool LoadPNG_File(
		__EXT_MFC_SAFE_LPCTSTR strFilePath,
		bool bMake32 = false,
		bool bEnableThrowException = false
		);
	bool LoadPNG_File(
		CFile & _file,
		bool bMake32 = false,
		bool bEnableThrowException = false
		);
	bool LoadPNG(
		CArchive & ar,
		bool bMake32 = false,
		bool bEnableThrowException = false
		);
	bool SavePNG_File(
		__EXT_MFC_SAFE_LPCTSTR strFilePath,
		bool bMake32 = false,
		bool bEnableThrowException = false
		);
	bool SavePNG_File(
		CFile & _file,
		bool bMake32 = false,
		bool bEnableThrowException = false
		);
	bool SavePNG(
		CArchive & ar,
		bool bMake32 = false,
		bool bEnableThrowException = false
		);
protected:
	static void stat_PngCallbackRead(
		LPVOID pCallbackData,
		LPBYTE pBuffer,
		size_t nSize
		);
	static void stat_PngCallbackWrite(
		LPVOID pCallbackData,
		LPBYTE pBuffer,
		size_t nSize
		);
	static void stat_PngCallbackFlush(
		LPVOID pCallbackData
		);
	static void stat_PngCallbackError(
		LPVOID pCallbackData,
		const char * strErrorDescription
		);
	static void stat_PngCallbackWarning(
		LPVOID pCallbackData,
		const char * strWarningDescription
		);
	static bool stat_PngCheckSwap32(
		LPBYTE pImageBuffer
		);
	static LPBYTE stat_PngLoad(
		CArchive & ar,
		bool bEnableThrowException
		);
	static bool stat_PngSave(
		LPBYTE pImageBuffer,
		CArchive & ar,
		bool bEnableThrowException
		);
}; // class CExtSkinBitmap

#endif // __PROF_SKIN_BITMAP_H

