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

#include "StdAfx.h"

#if (!defined __PROF_SKIN_ITEM_H)
	#include <../ProfSkin/ExtSkinItem.h>
#endif

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#include <../src/mfc/AfxImpl.h>
#endif

#if (!defined ZLIB_H)
	#include <zlib.h>
#endif

#define __ReadHuge__ Read
#define __WriteHuge__ Write

IMPLEMENT_DYNAMIC( CExtSkinItemBase, CObject );
IMPLEMENT_DYNAMIC( CExtSkinGlyph, CExtSkinItemBase );
IMPLEMENT_DYNAMIC( CExtSkinColor, CExtSkinItemBase );
IMPLEMENT_DYNAMIC( CExtSkinFont, CExtSkinItemBase );
IMPLEMENT_DYNAMIC( CExtSkinItem, CExtSkinItemBase );
IMPLEMENT_DYNAMIC( CExtSkin, CExtSkinItem );

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool CExtSkinItemBase::NumberFromString::stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, long lMinValue, long lMaxValue, long & lValue )
{
	if( str == NULL || _tcslen(str) == 0 )
		return false;
CString strSource( str );
	ASSERT( ! strSource.IsEmpty() );
long lResult = LONG_MAX;
	if(		strSource.GetLength() > 2
		&&	strSource[0] == _T('0')
		&&	(	strSource[1] == _T('x')
			||	strSource[1] == _T('X')
			)
		)
		lResult = _tcstol( strSource, NULL, 16 );
	else
		lResult = _tcstol( strSource, NULL, 10 );
	if(		lResult <= lMinValue
		||	long(lResult) >= long(lMaxValue)
		)
		return false;
	lValue = lResult;
	return true;
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( BSTR bstr, long & lValue )
{
	return stat_GetValue( __EXT_MFC_SAFE_LPCTSTR( CExtSafeString( bstr ) ), lValue );
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, long & lValue )
{
	if( ! stat_GetValue( str, LONG_MIN, LONG_MAX, lValue ) )
		return false;
	return true;
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( BSTR bstr, unsigned long & ulValue )
{
	return stat_GetValue( __EXT_MFC_SAFE_LPCTSTR( CExtSafeString( bstr ) ), ulValue );
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, unsigned long & ulValue )
{
long lValue;
	if( ! stat_GetValue( str, -1, ULONG_MAX, lValue ) )
		return false;
	ulValue = (unsigned long)lValue;
	return true;
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( BSTR bstr, short & sValue )
{
	return stat_GetValue( __EXT_MFC_SAFE_LPCTSTR( CExtSafeString( bstr ) ), sValue );
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, short & sValue )
{
long lValue;
	if( ! stat_GetValue( str, SHRT_MIN, SHRT_MAX, lValue ) )
		return false;
	sValue = (short)lValue;
	return true;
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( BSTR bstr, unsigned short & usValue )
{
	return stat_GetValue( __EXT_MFC_SAFE_LPCTSTR( CExtSafeString( bstr ) ), usValue );
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, unsigned short & usValue )
{
long lValue;
	if( ! stat_GetValue( str, -1, USHRT_MAX, lValue ) )
		return false;
	usValue = (unsigned short)lValue;
	return true;
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( BSTR bstr, char & cValue )
{
	return stat_GetValue( __EXT_MFC_SAFE_LPCTSTR( CExtSafeString( bstr ) ), cValue );
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, char & cValue )
{
long lValue;
	if( ! stat_GetValue( str, CHAR_MIN, CHAR_MAX, lValue ) )
		return false;
	cValue = (char)lValue;
	return true;
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( BSTR bstr, unsigned char & ucValue )
{
	return stat_GetValue( __EXT_MFC_SAFE_LPCTSTR( CExtSafeString( bstr ) ), ucValue );
}

bool CExtSkinItemBase::NumberFromString::stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, unsigned char & ucValue )
{
long lValue;
	if( ! stat_GetValue( str, -1, UCHAR_MAX, lValue ) )
		return false;
	ucValue = (unsigned char)lValue;
	return true;
}

CExtSkinItemBase::CExtSkinItemBase()
	: m_pParent( NULL )
{
	_Clear();
}

CExtSkinItemBase::CExtSkinItemBase( const CExtSkinItemBase & other )
	: m_pParent( NULL )
{
	ASSERT_VALID( (&other) );
	(*this) = other;
}

CExtSkinItemBase::~CExtSkinItemBase()
{
	_Clear();
}

const TCHAR CExtSkinItemBase::g_strMessKey[] =
	_T("CExtSkinItemBase::SerializeFile");
const BYTE CExtSkinItemBase::g_arrSignature[ __EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE ] = 
	{ 'S', 'K', 'I', 'N' };

bool CExtSkinItemBase::SerializeCompressedBlock(
	CArchive & ar,
	bool bEnableThrowExceptions, // = false
	bool bUseCompression, // = true
	__EXT_MFC_SAFE_LPCTSTR strMessKey // = NULL
	)
{
	ASSERT_VALID( this );
	if( strMessKey == NULL || _tcslen( strMessKey ) == 0 )
		strMessKey = g_strMessKey;
	try
	{
		BYTE arrBuffer[ 1024*16 ];
		DWORD dwDataIndex, dwDataSize;
		CMemFile _file;
		if( ar.IsStoring() )
		{
			CExtZLibStreamCacheArchive arBuffer(
				&_file,
				CArchive::store,
				bUseCompression,
				bUseCompression ? strMessKey : _T(""),
				bUseCompression ? (LPBYTE)g_arrSignature : NULL
				);
			Serialize( arBuffer );
			arBuffer.Flush();
			arBuffer.Close();
			ASSERT_VALID( this );
			dwDataSize = (DWORD)_file.GetPosition();
			_file.Seek( 0L, CFile::begin );
			ar << dwDataSize;
			for( dwDataIndex = 0; dwDataIndex < dwDataSize; )
			{
				DWORD dwPortionSize = dwDataSize - dwDataIndex;
				if( dwPortionSize > (sizeof(arrBuffer)/sizeof(arrBuffer[0])) )
					dwPortionSize = (sizeof(arrBuffer)/sizeof(arrBuffer[0]));
				_file.Read( arrBuffer, UINT(dwPortionSize) );
				ar.Write( arrBuffer, UINT(dwPortionSize) );
				dwDataIndex += dwPortionSize;
			} // for( dwDataIndex = 0; dwDataIndex < dwDataSize; )
		} // if( ar.IsStoring() )
		else
		{
			ar >> dwDataSize;
			for( dwDataIndex = 0; dwDataIndex < dwDataSize; )
			{
				DWORD dwPortionSize = dwDataSize - dwDataIndex;
				if( dwPortionSize > (sizeof(arrBuffer)/sizeof(arrBuffer[0])) )
					dwPortionSize = (sizeof(arrBuffer)/sizeof(arrBuffer[0]));
				ar.Read( arrBuffer, UINT(dwPortionSize) );
				_file.Write( arrBuffer, UINT(dwPortionSize) );
				dwDataIndex += dwPortionSize;
			} // for( dwDataIndex = 0; dwDataIndex < dwDataSize; )
			_file.Seek( 0L, CFile::begin );
			bUseCompression = true; // auto detect format
			CExtZLibStreamCacheArchive arBuffer(
				&_file,
				CArchive::load,
				bUseCompression,
				bUseCompression ? strMessKey : _T(""),
				bUseCompression ? (LPBYTE)g_arrSignature : NULL
				);
			Serialize( arBuffer );
			ASSERT_VALID( this );
		} // else from if( ar.IsStoring() )
		return true;
	} // try
	catch( CException * pException )
	{
		if( bEnableThrowExceptions )
			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
		if( bEnableThrowExceptions )
			throw;
	} // catch( ... )
	return false;
}

void CExtSkinItemBase::Serialize( CArchive & ar )
{
	ASSERT_VALID( this );
	if( ar.IsStoring() )
	{
		ar << m_strName;
		ar << m_strComment;
	} // if( ar.IsStoring() )
	else
	{
		_Clear();
		ar >> m_strName;
		ar >> m_strComment;
	} // else from if( ar.IsStoring() )
}

bool CExtSkinItemBase::SerializeFile(
	CFile & _file,
	bool bSave,
	bool bEnableThrowExceptions,
	bool bUseCompression, // = true
	__EXT_MFC_SAFE_LPCTSTR strMessKey // = NULL
	)
{
	ASSERT_VALID( this );
	try
	{
		if( ! bSave )
			bUseCompression = true; // auto detect format
		if( strMessKey == NULL || _tcslen( strMessKey ) == 0 )
			strMessKey = g_strMessKey;
		CExtZLibStreamCacheArchive ar(
			&_file,
			bSave ? CArchive::store : CArchive::load,
			bUseCompression,
			bUseCompression ? strMessKey : _T(""),
			bUseCompression ? (LPBYTE)g_arrSignature : NULL
			);
		Serialize( ar );
		ASSERT_VALID( this );
		return true;
	} // try
	catch( CException * pException )
	{
		if( bEnableThrowExceptions )
			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
		if( bEnableThrowExceptions )
			throw;
	} // catch( ... )
	return false;
}

bool CExtSkinItemBase::SearchAndLoadSkinFile(
	__EXT_MFC_SAFE_LPCTSTR strFilePath,
	bool bEnableThrowExceptions,
	bool bUseCompression, // = true
	__EXT_MFC_SAFE_LPCTSTR strMessKey // = NULL
	)
{
	try
	{
		if( SerializeFile(
				strFilePath,
				false,
				bEnableThrowExceptions,
				bUseCompression,
				strMessKey
				)
			)
			return true;
	} // try
	catch( CException * pException )
	{
//		if( bEnableThrowExceptions )
//			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
//		if( bEnableThrowExceptions )
//			throw;
	} // catch( ... )

TCHAR strSpecifiedDrive[_MAX_DRIVE];
TCHAR strSpecifiedDir[_MAX_DIR];
TCHAR strSpecifiedFileName[_MAX_FNAME];
TCHAR strSpecifiedFileExtension[_MAX_EXT];
	::memset( strSpecifiedDrive, 0, sizeof(strSpecifiedDrive) );
	::memset( strSpecifiedDir, 0, sizeof(strSpecifiedDir) );
	::memset( strSpecifiedFileName, 0, sizeof(strSpecifiedFileName) );
	::memset( strSpecifiedFileExtension, 0, sizeof(strSpecifiedFileExtension) );
	
	__EXT_MFC_SPLITPATH( 
		strFilePath, 
		strSpecifiedDrive, 
		_MAX_DRIVE,
		strSpecifiedDir, 
		_MAX_DIR,
		strSpecifiedFileName, 
		_MAX_FNAME,
		strSpecifiedFileExtension,
		_MAX_EXT
		);

TCHAR strModulePathBuffer[ _MAX_PATH + 1 ];
TCHAR strModuleDrive[_MAX_DRIVE];
TCHAR strModuleDir[_MAX_DIR];
TCHAR strModuleFileName[_MAX_FNAME];
TCHAR strModuleFileExtension[_MAX_EXT];
	::memset( strModulePathBuffer, 0, sizeof(strModulePathBuffer) );
	::memset( strModuleDrive, 0, sizeof(strModuleDrive) );
	::memset( strModuleDir, 0, sizeof(strModuleDir) );
	::memset( strModuleFileName, 0, sizeof(strModuleFileName) );
	::memset( strModuleFileExtension, 0, sizeof(strModuleFileExtension) );
	::GetModuleFileName(
		::GetModuleHandle( NULL ),
		strModulePathBuffer,
		_MAX_PATH
		);
	__EXT_MFC_SPLITPATH( 
		strModulePathBuffer, 
		strModuleDrive, 
		_MAX_DRIVE,
		strModuleDir,
		_MAX_DIR,
		strModuleFileName, 
		_MAX_FNAME,
		strModuleFileExtension,
		_MAX_EXT
		);

TCHAR strTryPath[ _MAX_PATH + 1 ];
	::memset( strTryPath, 0, sizeof(strTryPath) );
	__EXT_MFC_SPRINTF( 
		__EXT_MFC_SPRINTF_PARAM( strTryPath, _MAX_PATH + 1 ), 
		_T(".\\%s%s"), 
		strSpecifiedFileName, 
		strSpecifiedFileExtension 
		);
	try
	{
		if( SerializeFile(
				strTryPath,
				false,
				bEnableThrowExceptions,
				bUseCompression,
				strMessKey
				)
			)
			return true;
	} // try
	catch( CException * pException )
	{
//		if( bEnableThrowExceptions )
//			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
//		if( bEnableThrowExceptions )
//			throw;
	} // catch( ... )
	__EXT_MFC_SPRINTF( 
		__EXT_MFC_SPRINTF_PARAM( strTryPath, _MAX_PATH + 1 ), 
		_T("%s%s%s%s"), 
		strModuleDrive, 
		strModuleDir, 
		strSpecifiedFileName, 
		strSpecifiedFileExtension 
		);
	try
	{
		if( SerializeFile(
				strTryPath,
				false,
				bEnableThrowExceptions,
				bUseCompression,
				strMessKey
				)
			)
			return true;
	} // try
	catch( CException * pException )
	{
		if( bEnableThrowExceptions )
			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
		if( bEnableThrowExceptions )
			throw;
	} // catch( ... )
	return false;
}

bool CExtSkinItemBase::SerializeFile(
	__EXT_MFC_SAFE_LPCTSTR strFilePath,
	bool bSave,
	bool bEnableThrowExceptions,
	bool bUseCompression, // = true
	__EXT_MFC_SAFE_LPCTSTR strMessKey // = NULL
	)
{
	ASSERT_VALID( this );
	if( strFilePath == NULL )
	{
		if( bEnableThrowExceptions )
	#if _MFC_VER >= 0x0800
		::AfxThrowArchiveException( CArchiveException::genericException );
	#else
		::AfxThrowArchiveException( CArchiveException::generic );
	#endif
		return false;
	}
	try
	{
		CFile _file(
			LPCTSTR( strFilePath ),
			bSave
				?	( CFile::modeWrite | CFile::modeCreate | CFile::typeBinary )
				:	( CFile::modeRead | CFile::typeBinary )
			);
		bool bRetVal =
			SerializeFile(
				_file,
				bSave,
				bEnableThrowExceptions,
				bUseCompression,
				strMessKey
				);
		ASSERT_VALID( this );
		ASSERT( bRetVal || (! bEnableThrowExceptions ) );
		return bRetVal;
	} // try
	catch( CException * pException )
	{
		if( bEnableThrowExceptions )
			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
		if( bEnableThrowExceptions )
			throw;
	} // catch( ... )
	return false;
}

#if (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinItemBase::Load( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	ASSERT( m_pParent == NULL );
	if( m_pParent != NULL )
		return false;
	ASSERT( pElement != NULL );
	_Clear();
	if( ! pElement )
		return false;
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
_variant_t varValue;
	try
	{
		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_NAME );
		if( varValue.vt != VT_BSTR )
			throw 0;
		m_strName = (BSTR)(_bstr_t)varValue;
		if( m_strName.GetLength() == 0 )
			throw 0;
		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_COMMENT );
		if( varValue.vt == VT_BSTR )
			m_strComment = (BSTR)(_bstr_t)varValue;
	}
	catch( int )
	{
		ASSERT( FALSE );
		_Clear();
		return false;
	}
	return true;
}

#endif // (defined __PROF_SKIN_XML_SUPPORT)

CExtSkinItemBase & CExtSkinItemBase :: operator = ( const CExtSkinItemBase & other )
{
	ASSERT_VALID( this );
	ASSERT_VALID( (&other) );
	NameSet( other.m_strName );
	CommentSet( other.m_strComment );
	return (*this);
}

CExtSkinItem * CExtSkinItemBase::GetParent()
{
	ASSERT_VALID( this );
	return m_pParent;
}

const CExtSkinItem * CExtSkinItemBase::GetParent() const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItemBase * > ( this ) -> GetParent();
}

CExtSkinItem * CExtSkinItemBase::GetTopLevel()
{
	ASSERT_VALID( this );
CExtSkinItem * pParent = GetParent();
	if( pParent == NULL )
		return NULL;
	ASSERT_VALID( pParent );
	return pParent->GetTopLevel();
}

const CExtSkinItem * CExtSkinItemBase::GetTopLevel() const
{
	ASSERT_VALID( this );
	return const_cast<CExtSkinItemBase*>( this )->GetTopLevel();
}

CExtSkin * CExtSkinItemBase::GetSkin()
{
	ASSERT_VALID( this );
CExtSkinItem * pItem = GetTopLevel();
	if( pItem == NULL )
		return NULL;
	ASSERT_VALID( pItem );
CExtSkin * pSkin = DYNAMIC_DOWNCAST( CExtSkin, pItem );
	return pSkin;
}

const CExtSkin * CExtSkinItemBase::GetSkin() const
{
	ASSERT_VALID( this );
	return const_cast<CExtSkinItemBase*>( this )->GetSkin();
}

__EXT_MFC_SAFE_LPCTSTR CExtSkinItemBase::NameGet() const
{
	ASSERT_VALID( this );
	return m_strName;
}

bool CExtSkinItemBase::NameSet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
	if( strName == NULL )
		m_strName.Empty();
	else
		m_strName = strName;
	return true;
}

__EXT_MFC_SAFE_LPCTSTR CExtSkinItemBase::CommentGet() const
{
	ASSERT_VALID( this );
	return m_strComment;
}

void CExtSkinItemBase::CommentSet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
	m_strComment = strName;
}

void CExtSkinItemBase::_Clear()
{
	ASSERT_VALID( this );
	m_strName.Empty();
	m_strComment.Empty();
}

CExtSkinGlyph::CExtSkinGlyph()
{
	_Clear();
}

CExtSkinGlyph::CExtSkinGlyph( const CExtSkinGlyph & other )
{
	ASSERT_VALID( (&other) );
	(*this) = other;
}

CExtSkinGlyph::~CExtSkinGlyph()
{
	_Clear();
}

void CExtSkinGlyph::Serialize( CArchive & ar )
{
	ASSERT_VALID( this );
	CExtSkinItemBase::Serialize( ar );
	m_Bitmap.SerializeUnpacked( ar );
	if( ar.IsStoring() )
	{
		DWORD dwFlags = 0;
		if( m_bEnabled )
			dwFlags |= 0x00000001;
		if( m_bDrawCenter )
			dwFlags |= 0x00000002;
		ar << dwFlags;
		ar << DWORD(m_nIconIndent);
		ar << m_rcPadding;
		ar << m_rcContentPadding;
		ar << m_rcOffset;
		ar << m_Size;
		DWORD dwDM = (DWORD) m_eDM;
		ar << dwDM;
	} // if( ar.IsStoring() )
	else
	{
		DWORD dwFlags, dwTmp;
		ar >> dwFlags;
		if( ( dwFlags & 0x00000001 ) != 0 )
			m_bEnabled = true;
		else
			m_bEnabled = false;
		if( ( dwFlags & 0x00000002 ) != 0 )
			m_bDrawCenter = true;
		else
			m_bDrawCenter = false;
		ar >> dwTmp;
		m_nIconIndent = INT(dwTmp);
		ar >> m_rcPadding;
		ar >> m_rcContentPadding;
		ar >> m_rcOffset;
		ar >> m_Size;
		DWORD dwDM = 0;
		ar >> dwDM;
		m_eDM = (CExtBitmap::e_DrawMethod_t) dwDM;
	} // else from if( ar.IsStoring() )
	ASSERT_VALID( this );
}

CExtSkinGlyph & CExtSkinGlyph :: operator = ( const CExtSkinGlyph & other )
{
	ASSERT_VALID( this );
	ASSERT_VALID( (&other) );
	CExtSkinItemBase :: operator = ( other );
	m_bEnabled = other.m_bEnabled;
	m_bDrawCenter = other.m_bDrawCenter;
	m_nIconIndent = other.m_nIconIndent;
	m_rcPadding = other.m_rcPadding;
	m_rcContentPadding = other.m_rcContentPadding;
	m_rcOffset = other.m_rcOffset;
	m_Size = other.m_Size;
	m_eDM = other.m_eDM;
	m_Bitmap.AssignFromOther( other.m_Bitmap );
	ASSERT_VALID( this );
	return (*this);
}

#if (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinGlyph::Load( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	ASSERT( pElement != NULL );
	if( pElement == NULL ) 
		return false;
	if( ! CExtSkinItemBase::Load( pElement ) )
		return false;
	_Clear();
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
_variant_t varValue;
	try
	{
		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_TOP_PADDING );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcPadding.top ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_LEFT_PADDING );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcPadding.left ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_BOTTOM_PADDING );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcPadding.bottom ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_RIGHT_PADDING );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcPadding.right ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_CONTENT_TOP_PADDING );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcContentPadding.top ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_CONTENT_LEFT_PADDING );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcContentPadding.left ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_CONTENT_BOTTOM_PADDING );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcContentPadding.bottom ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_CONTENT_RIGHT_PADDING );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcContentPadding.right ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_LEFT_OFFSET );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcOffset.left ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_TOP_OFFSET );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcOffset.top ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_RIGHT_OFFSET );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcOffset.right ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_BOTTOM_OFFSET );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_rcOffset.bottom ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_ICON_INDENT );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_nIconIndent ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_WIDTH );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_Size.cx ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_HEIGHT );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_Size.cy ) )
				throw 0;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_DRAW_METHOD );
		if( varValue.vt == VT_BSTR )
			m_eDM =
			CExtBitmap::stat_GetDrawMethodFromString(
				CExtSafeString( (BSTR)(_bstr_t)varValue )
				);

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_SRC_BITMAP );
		if( varValue.vt == VT_BSTR )
		{
			if( ! m_Bitmap.LoadImage_File(
					__EXT_MFC_SAFE_LPCTSTR( CString( (BSTR)(_bstr_t)varValue ) )
					)
				)
				throw 0;
		}
		else if( m_eDM != CExtBitmap::__EDM_HOLLOW )
			throw 0;

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_DRAW_CENTER );
		if( varValue.vt == VT_BSTR )
		{
			CString strDrawCenter( (BSTR)(_bstr_t)varValue );
			strDrawCenter.MakeLower();
			if( strDrawCenter == _T( "false" ) )
				m_bEnabled = false;
		}

		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_GLYPH_ENABLED );
		if( varValue.vt == VT_BSTR )
		{
			CString strEnabled( (BSTR)(_bstr_t)varValue );
			strEnabled.MakeLower();
			if( strEnabled == _T( "false" ) )
				m_bEnabled = false;
		}
	}
	catch( CException * pExcep )
	{
		delete pExcep;
		ASSERT( FALSE );
		_Clear();
		return false;
	}
	catch( int )
	{
		ASSERT( FALSE );
		_Clear();
		return false;
	}
	ASSERT_VALID( this );
	return true;
}

#endif // (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinGlyph::Draw(
	HDC hDC,
	const RECT & rcDstDraw,
	const RECT & rcSrcDraw,
	bool bForceNoAlpha, // = false
	BYTE nSCA, // = BYTE(0x0FF)
	COLORREF clrColorize // = COLORREF(-1L)
	) const
{
	ASSERT_VALID( this );
	ASSERT( hDC != NULL );
	if(		hDC == NULL
		||	m_Bitmap.IsEmpty()
		)
		return false;
CRect _rcDstDraw = rcDstDraw;
CSize _sizeDraw(
		rcDstDraw.right - rcDstDraw.left,
		rcDstDraw.bottom - rcDstDraw.top
		);
CSize _sizeMinDraw(
		m_rcPadding.right + m_rcPadding.left,
		m_rcPadding.bottom + m_rcPadding.top
		);
	if( _sizeDraw.cx < _sizeMinDraw.cx )
	{
		LONG nSize = ( _sizeMinDraw.cx - _sizeDraw.cx ) / 2 - 2;
		_rcDstDraw.left -= nSize;
		_rcDstDraw.right += nSize;
	}
	if( _sizeDraw.cy < _sizeMinDraw.cy )
	{
		LONG nSize = ( _sizeMinDraw.cy - _sizeDraw.cy ) / 2;
		_rcDstDraw.top -= nSize;
		_rcDstDraw.bottom += nSize;
	}

CExtBitmap _bitmapColorized;
const CExtBitmap * pBitmapForPainting = &m_Bitmap;
	if( clrColorize != COLORREF(-1L) )
	{
		_bitmapColorized = m_Bitmap;
		_bitmapColorized.Make32();
		_bitmapColorized.MakeMono( clrColorize );
		pBitmapForPainting = &_bitmapColorized;
	}

	if( bForceNoAlpha )
		return
			pBitmapForPainting->DrawSkinParts( 
				hDC,
				_rcDstDraw,
				rcSrcDraw,
				m_rcPadding,
				m_eDM,
				m_bDrawCenter
				);
	else
		return
			pBitmapForPainting->AlphaBlendSkinParts( 
				hDC,
				_rcDstDraw,
				rcSrcDraw,
				m_rcPadding, 
				m_eDM,
				m_bDrawCenter,
				true,
				nSCA
				);
}

bool CExtSkinGlyph::Draw(
	HDC hDC,
	const RECT & rcDstDraw,
	bool bForceNoAlpha, // = false
	BYTE nSCA, // = BYTE(0x0FF)
	COLORREF clrColorize // = COLORREF(-1L)
	) const
{
	ASSERT_VALID( this );
	ASSERT( hDC != NULL );
	if(		hDC == NULL
		||	m_Bitmap.IsEmpty()
		)
		return false;
CSize _size = m_Bitmap.GetSize();
RECT rcSrcDraw = { 0, 0, _size.cx, _size.cy };
	return
		Draw(
			hDC,
			rcDstDraw,
			rcSrcDraw,
			bForceNoAlpha,
			nSCA,
			clrColorize
			);
}

bool CExtSkinGlyph::IsEmpty() const
{
	ASSERT_VALID( this );
	return m_Bitmap.IsEmpty();
}

CSize CExtSkinGlyph::GetSize() const
{
	ASSERT_VALID( this );
	if( m_Size.cx <= 0 && m_Size.cy <= 0 )
		return m_Bitmap.GetSize();
	return m_Size;
}

long CExtSkinGlyph::GetIconIndent() const
{
	ASSERT_VALID( this );
	return m_nIconIndent;
}

const CRect & CExtSkinGlyph::GetPadding() const
{
	ASSERT_VALID( this );
	return m_rcPadding;
}

const CRect & CExtSkinGlyph::GetContentPadding() const
{
	ASSERT_VALID( this );
	return m_rcContentPadding;
}

const CRect & CExtSkinGlyph::GetOffset() const
{
	ASSERT_VALID( this );
	return m_rcOffset;
}

const CExtBitmap & CExtSkinGlyph::GetBitmap() const
{
	ASSERT_VALID( this );
	return m_Bitmap;
}

CExtBitmap & CExtSkinGlyph::GetBitmap()
{
	ASSERT_VALID( this );
	return m_Bitmap;
}

bool CExtSkinGlyph::NameSet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
	if( _tcscmp( m_strName, strName ) == 0 )
		return true;
	if( m_pParent == NULL )
		return CExtSkinItemBase::NameSet( strName );
	if( m_pParent->GlyphGet( strName ) )
		return false;
	return CExtSkinItemBase::NameSet( strName );
}

void CExtSkinGlyph::_Clear()
{
	ASSERT_VALID( this );
	m_rcPadding = CRect( 0, 0, 0, 0 );
	m_rcContentPadding = CRect( 0, 0, 0, 0 );
	m_rcOffset = CRect( 0, 0, 0, 0 );
	m_Size = CSize( 0, 0 );
	m_Bitmap.Empty();
	m_eDM = CExtBitmap::__EDM_DEFAULT;
	m_bEnabled = true;
	m_bDrawCenter = true;
	m_nIconIndent = 0;
}

CExtSkinColor::CExtSkinColor()
{
	_Clear();
}

CExtSkinColor::CExtSkinColor( COLORREF clr )
{
	(*this) = clr;
}

CExtSkinColor::~CExtSkinColor()
{
	_Clear();
}

void CExtSkinColor::Serialize( CArchive & ar )
{
	ASSERT_VALID( this );
	CExtSkinItemBase::Serialize( ar );
	if( ar.IsStoring() )
	{
		ar << DWORD(m_nColor);
	} // if( ar.IsStoring() )
	else
	{
		DWORD dwTmp;
		ar >> dwTmp;
		m_nColor = COLORREF(dwTmp);
	} // else from if( ar.IsStoring() )
	ASSERT_VALID( this );
}

#if (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinColor::Load( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	ASSERT( pElement != NULL );
	if( pElement == NULL )
		return false;
	if( ! CExtSkinItemBase::Load( pElement ) )
		return false;
	_Clear();
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
_variant_t varValue;
	try
	{
		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_COLOR_COLOR );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_nColor ) )
				throw 0;
		}
	}
	catch( int )
	{
		ASSERT( FALSE );
		_Clear();
		return false;
	}
	return true;	
}

#endif // (defined __PROF_SKIN_XML_SUPPORT)

CExtSkinColor :: operator COLORREF () const
{
	ASSERT_VALID( this );
	return (COLORREF)m_nColor;
}

CExtSkinColor & CExtSkinColor :: operator = ( const CExtSkinColor & other )
{
	ASSERT_VALID( this );
	ASSERT_VALID( (&other) );
	CExtSkinItemBase :: operator = ( other );
	m_nColor = other.m_nColor;
	return (*this);
}

CExtSkinColor& CExtSkinColor :: operator = ( COLORREF clr )
{
	ASSERT_VALID( this );
	m_nColor = (long)clr;
	return (*this);
}

bool CExtSkinColor::IsHollow() const
{
	ASSERT_VALID( this );
	return ( m_nColor >= 0  ) ? false : true;
}

bool CExtSkinColor::NameSet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
	if( _tcscmp( m_strName, strName ) == 0 )
		return true;
	if( m_pParent == NULL )
		return CExtSkinItemBase::NameSet( strName );
	if( m_pParent->ColorGet( strName ) )
		return false;
	return CExtSkinItemBase::NameSet( strName );
}

void CExtSkinColor::_Clear()
{
	ASSERT_VALID( this );
	m_nColor = -1L;
}

CExtSkinFont::CExtSkinFont()
{
	_Clear();
}

CExtSkinFont::CExtSkinFont( const LOGFONT & lf )
{
	_Clear();
	(*this) = lf;
}

CExtSkinFont::CExtSkinFont( const CExtSkinFont & other )
{
	_Clear();
	ASSERT_VALID( (&other) );
	(*this) = other;
}

CExtSkinFont::~CExtSkinFont()
{
	_Clear();
}

void CExtSkinFont::Serialize( CArchive & ar )
{
	ASSERT_VALID( this );
	CExtSkinItemBase::Serialize( ar );
	if( ar.IsStoring() )
	{
		LOGFONT & _lfT = LogFontGet();
		LOGFONTA _lf;
		__EXT_MFC_MEMCPY(
			&_lf, 
			sizeof(LOGFONTA),
			&_lfT, 
			sizeof(LOGFONTA)
			);
#ifdef _UNICODE
		::memset( &_lf.lfFaceName, 0, sizeof(_lf.lfFaceName) );
		USES_CONVERSION;
	#if _MFC_VER >= 0x0800
		::strcpy_s( _lf.lfFaceName, LF_FACESIZE, T2CA(_lfT.lfFaceName) );
	#else
		::strcpy( _lf.lfFaceName, T2CA(_lfT.lfFaceName) );
	#endif
#endif
		ar.Write( &_lf, sizeof(LOGFONTA) );
		ar << m_strHashCode;
		DWORD dwFlags = 0;
		if( m_bHaveHeight )
			dwFlags |= 0x00000001;
		if( m_bHaveWidth )
			dwFlags |= 0x00000002;
		if( m_bHaveWeight )
			dwFlags |= 0x00000004;
		if( m_bHaveItalic )
			dwFlags |= 0x00000008;
		if( m_bHaveUnderline )
			dwFlags |= 0x00000010;
		if( m_bHaveStrikeOut )
			dwFlags |= 0x00000020;
		if( m_bHaveFaceName )
			dwFlags |= 0x00000040;
		if( m_bComplete )
			dwFlags |= 0x00000080;
		if( m_bHavePitchAndFamily )
			dwFlags |= 0x00000100;
		if( m_bHaveCharSet )
			dwFlags |= 0x00000200;
		ar << dwFlags;
	} // if( ar.IsStoring() )
	else
	{
		_Clear();
		LOGFONTA _lf;
		ar.Read( &_lf, sizeof(LOGFONTA) );
		LOGFONT & _lfT = LogFontGet();
		BYTE lfCharSetSaved = _lfT.lfCharSet;
		__EXT_MFC_MEMCPY(
			(LPVOID)&_lfT, 
			sizeof(LOGFONTA),
			(LPVOID)&_lf, 
			sizeof(LOGFONTA) 
			);
		_lfT.lfCharSet = lfCharSetSaved;
#ifdef _UNICODE
		::memset( &_lfT.lfFaceName, 0, sizeof(_lfT.lfFaceName) );
		USES_CONVERSION;
		__EXT_MFC_STRCPY( 
			_lfT.lfFaceName, 
			LF_FACESIZE, 
			A2CT(_lf.lfFaceName) 
			);
#endif
		ar >> m_strHashCode;
		DWORD dwFlags = 0;
		ar >> dwFlags;
		if( ( dwFlags & 0x00000001 ) != 0 )
			m_bHaveHeight = true;
		else
			m_bHaveHeight = false;
		if( ( dwFlags & 0x00000002 ) != 0 )
			m_bHaveWidth = true;
		else
			m_bHaveWidth = false;
		if( ( dwFlags & 0x00000004 ) != 0 )
			m_bHaveWeight = true;
		else
			m_bHaveWeight = false;
		if( ( dwFlags & 0x00000008 ) != 0 )
			m_bHaveItalic = true;
		else
			m_bHaveItalic = false;
		if( ( dwFlags & 0x00000010 ) != 0 )
			m_bHaveUnderline = true;
		else
			m_bHaveUnderline = false;
		if( ( dwFlags & 0x00000020 ) != 0 )
			m_bHaveStrikeOut = false;
		if( ( dwFlags & 0x00000040 ) != 0 )
			m_bHaveFaceName = true;
		else
			m_bHaveFaceName = false;
		if( ( dwFlags & 0x00000080 ) != 0 )
			m_bComplete = true;
		else
			m_bComplete = false;
		if( ( dwFlags & 0x00000100 ) != 0 )
			m_bHavePitchAndFamily = true;
		else
			m_bHavePitchAndFamily = false;
		if( ( dwFlags & 0x00000200 ) != 0 )
			m_bHaveCharSet = true;
		else
			m_bHaveCharSet = false;
	} // else from if( ar.IsStoring() )
	ASSERT_VALID( this );
}

#if (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinFont::Load( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	ASSERT( pElement != NULL );
	if( pElement == NULL )
		return false;
	if( ! CExtSkinItemBase::Load( pElement ) )
		return false;
	_Clear();
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
_variant_t varValue;
	try
	{
		LOGFONT logFont = { 0 };

		varValue = pElementPtr->getAttribute( 
			_bstr_t( __EXT_SKIN_ITEM_FONT_HEIGHT ) );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, logFont.lfHeight ) )
				throw 0;
				
			m_bHaveHeight = true;
		}
				
		varValue = pElementPtr->getAttribute( 
			_bstr_t( __EXT_SKIN_ITEM_FONT_WIDTH ) );
		if( varValue.vt == VT_BSTR )
		{	
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, logFont.lfWidth ) )
        		throw 0;

			m_bHaveWidth = true;
		}

		varValue = pElementPtr->getAttribute( 
			_bstr_t( __EXT_SKIN_ITEM_FONT_WEIGHT ) );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, logFont.lfWeight ) )
				throw 0;

			m_bHaveWeight = true;
		}

		varValue = pElementPtr->getAttribute( 
			_bstr_t( __EXT_SKIN_ITEM_FONT_ITALIC ) );
		if( varValue.vt == VT_BSTR )
		{	
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, logFont.lfItalic ) )
				throw 0;

			m_bHaveItalic = true;
		}

		varValue = pElementPtr->getAttribute( 
			_bstr_t( __EXT_SKIN_ITEM_FONT_UNDERLINE ) );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, logFont.lfUnderline ) )
				throw 0;

			m_bHaveUnderline = true;
		}

		varValue = pElementPtr->getAttribute( 
			_bstr_t( __EXT_SKIN_ITEM_FONT_STRIKE_OUT ) );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, logFont.lfStrikeOut ) )
				throw 0;

			m_bHaveStrikeOut = true;
		}

		varValue = pElementPtr->getAttribute( 
			_bstr_t( __EXT_SKIN_ITEM_FONT_FACE_NAME ) );
		if( varValue.vt == VT_BSTR )
		{
			CExtSafeString strFaceName( (BSTR)(_bstr_t)varValue );
			if( strFaceName.GetLength() > LF_FACESIZE - 1 )
				throw 0;

			__EXT_MFC_STRNCPY( 
				logFont.lfFaceName, 
				LF_FACESIZE, 
				strFaceName, 
				LF_FACESIZE 
				);

			__EXT_MFC_MEMCPY( 
				static_cast<LOGFONT*>( this ), 
				sizeof( LOGFONT ),
				&logFont, 
				sizeof( LOGFONT ) 
				);

			m_bHaveFaceName = true;
		}

//		varValue = pElementPtr->getAttribute( 
//			_bstr_t( __EXT_SKIN_ITEM_FONT_SHADOW_OFFSET_X ) );
//		if( varValue.vt == VT_BSTR )
//		{
//			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_ptShadowOffset.x ) )
//				throw 0;
//
//			m_bHaveShadowOffset = true;
//		}

//		varValue = pElementPtr->getAttribute( 
//			_bstr_t( __EXT_SKIN_ITEM_FONT_SHADOW_OFFSET_Y ) );
//		if( varValue.vt == VT_BSTR )
//		{
//			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, m_ptShadowOffset.y ) )
//				throw 0;
//
//			m_bHaveShadowOffset = true;
//		}

		varValue = pElementPtr->getAttribute( _bstr_t( __EXT_SKIN_ITEM_FONT_PITCH_AND_FAMILY) );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, logFont.lfPitchAndFamily ) )
				throw 0;
			m_bHavePitchAndFamily =  true;
		}

		varValue = pElementPtr->getAttribute( _bstr_t(__EXT_SKIN_ITEM_FONT_CHAR_SET) );
		if( varValue.vt == VT_BSTR )
		{
			if( ! CExtSkinItemBase::NumberFromString::stat_GetValue( (BSTR)(_bstr_t)varValue, logFont.lfCharSet ) )
				throw 0;
			m_bHaveCharSet =  true;
		}

	}
	catch( int )
	{
		ASSERT( FALSE );
		_Clear();
		return false;
	}
	ASSERT_VALID( this );
	return true;
}

#endif // (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinFont::NameSet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
	if( _tcscmp( m_strName, strName ) == 0 )
		return true;
	if( m_pParent == NULL )
		return CExtSkinItemBase::NameSet( strName );
	if( m_pParent->FontGet( strName ) )
		return false;
	return CExtSkinItemBase::NameSet( strName );
}

bool CExtSkinFont::_IsComplete() const
{
	ASSERT_VALID( this );
	if( m_bComplete )
		return true;
const_cast < CExtSkinFont * > ( this ) ->
		m_bComplete =  
			   m_bHaveHeight
			&& m_bHaveWidth
			&& m_bHaveWeight
			&& m_bHaveItalic
			&& m_bHaveUnderline
			&& m_bHaveStrikeOut
			&& m_bHaveFaceName
//			&& m_bHaveShadowOffset
			&& m_bHavePitchAndFamily
			&& m_bHaveCharSet
			;
	return m_bComplete;
}

CExtSkinFont :: operator LOGFONT & ()
{
	ASSERT_VALID( this );
	return LogFontGet();
}

CExtSkinFont :: operator const LOGFONT & ()
{
	ASSERT_VALID( this );
	return ( LOGFONT & ) ( * const_cast < CExtSkinFont * > ( this ) );
}

CExtSkinFont & CExtSkinFont :: operator = ( const LOGFONT & lf )
{
	ASSERT_VALID( this );
	__EXT_MFC_MEMCPY(
		static_cast < LOGFONT * > ( this ), 
		sizeof(LOGFONT),
		&lf, 
		sizeof(LOGFONT) 
		);
	return (*this);
}

CExtSkinFont & CExtSkinFont :: operator = ( const CExtSkinFont & other )
{
	ASSERT_VALID( this );
	ASSERT_VALID( (&other) );
	CExtSkinItemBase :: operator = ( other );
	(*this) = ( LOGFONT & ) other;
	m_bHaveWidth = other.m_bHaveWidth;
	m_bHaveWeight = other.m_bHaveWeight;
	m_bHaveItalic = other.m_bHaveItalic;
	m_bHaveUnderline = other.m_bHaveUnderline;
	m_bHaveStrikeOut = other.m_bHaveStrikeOut;
//	m_ptShadowOffset = other.m_ptShadowOffset;
	m_strHashCode = other.m_strHashCode;
	ASSERT_VALID( this );
	return (*this);
}

HFONT CExtSkinFont::CreateFont(
	LONG lfEscapement // = 0
	) const
{
	ASSERT_VALID( this );
LOGFONT _lfDst;
const LOGFONT & _lfSrc = LogFontGet();
	__EXT_MFC_MEMCPY(
		&_lfDst,
		sizeof(LOGFONT),
		&_lfSrc,
		sizeof(LOGFONT) 
		);
	_lfDst.lfEscapement = lfEscapement;
HFONT hFont = ::CreateFontIndirect( &_lfDst );
	return hFont;
}

bool CExtSkinFont::CreateFont(
	CFont & _font,
	LONG lfEscapement // = 0
	) const
{
	ASSERT_VALID( this );
	if( _font.GetSafeHandle() != NULL )
		_font.DeleteObject();
HFONT hFont = CreateFont( lfEscapement );
	if( hFont == NULL )
		return false;
	_font.Attach( hFont );
	return true;
}

LOGFONT & CExtSkinFont::LogFontGet()
{
	ASSERT_VALID( this );
	if( _IsComplete() )
		return (*this);
CExtSkinItem * pItem = NULL;
	if( m_pParent != NULL )
		pItem = m_pParent->GetParent();
CExtSkinFont * pFont = NULL;
	if( pItem != NULL )
		pFont = pItem->FontGetDeep( m_strName );
	if( pItem == NULL || pFont == NULL )
		return (*this);
	if( (! m_bHaveHeight) && pFont->m_bHaveHeight )
	{
		lfHeight = pFont->lfHeight;
		m_bHaveHeight = true;
	}
	if( (! m_bHaveWidth) && pFont->m_bHaveWidth )
	{
		lfWidth = pFont->lfWidth;
		m_bHaveWidth = true;
	}
	if( (! m_bHaveWeight) && pFont->m_bHaveWeight )
	{
		lfWeight = pFont->lfWeight;
		m_bHaveWeight = true;
	}
	if( (! m_bHaveItalic) && pFont->m_bHaveItalic )
	{
		lfItalic = pFont->lfItalic;
		m_bHaveItalic = true;
	}
	if( (! m_bHaveUnderline) && pFont->m_bHaveUnderline )
	{
		lfUnderline = pFont->lfUnderline;
		m_bHaveUnderline = true;
	}
	if( (! m_bHaveStrikeOut) && pFont->m_bHaveStrikeOut )
	{
		lfStrikeOut = pFont->lfStrikeOut;
		m_bHaveStrikeOut = true;
	}
	if( (! m_bHaveFaceName) && pFont->m_bHaveFaceName )
	{
		__EXT_MFC_MEMCPY(
			lfFaceName, 
			LF_FACESIZE,
			pFont->lfFaceName, 
			LF_FACESIZE 
			);
		m_bHaveFaceName = true;
	}
//	if( (! m_bHaveShadowOffset) && pFont->m_bHaveShadowOffset )
//	{
//		m_ptShadowOffset = pFont->m_ptShadowOffset;
//		m_bHaveShadowOffset = true;
//	}
	if( (! m_bHavePitchAndFamily) && pFont->m_bHavePitchAndFamily )
	{
		lfPitchAndFamily = pFont->lfPitchAndFamily;
		m_bHavePitchAndFamily = true;
	}
	if( (! m_bHaveCharSet) && pFont->m_bHaveCharSet )
	{
		lfCharSet = pFont->lfCharSet;
		m_bHaveCharSet = true;
	}

	return (*this);
}

const LOGFONT & CExtSkinFont::LogFontGet() const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinFont * > ( this )->LogFontGet();
}

//CPoint & CExtSkinFont::ShadowOffsetGet()
//{
//	ASSERT_VALID( this );
//	return m_ptShadowOffset;
//}

//const CPoint & CExtSkinFont::ShadowOffsetGet() const
//{
//	ASSERT_VALID( this );
//	return const_cast<CExtSkinFont*>( this )->ShadowOffsetGet();
//}

LPCTSTR CExtSkinFont::GetHashCode() const
{
	ASSERT_VALID( this );
	if(		(! m_strHashCode.IsEmpty() )
		&&	_IsComplete()
		)
		return m_strHashCode;
CExtSafeString & strHashCode = const_cast<CExtSkinFont*>( this )->m_strHashCode;
	strHashCode.Format( 
		_T("%d%d%d%d%d%c%c%c%c%c%c%c%c%s%d%d"),
		lfHeight,
		lfWidth,
		lfEscapement,
		lfOrientation,
		lfWeight,
		lfItalic,
		lfUnderline,
		lfStrikeOut,
		lfCharSet,
		lfOutPrecision,
		lfClipPrecision,
		lfQuality,
		lfPitchAndFamily,
		lfFaceName,
		0, // m_ptShadowOffset.x,
		0  // m_ptShadowOffset.y
		);
	return m_strHashCode;
}

void CExtSkinFont::_Clear()
{
	ASSERT_VALID( this );
	::memset( static_cast < LOGFONT * > ( this ), 0, sizeof( LOGFONT ) );

HFONT hFont = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	if( hFont != NULL )
		::GetObject( hFont, sizeof( LOGFONT ), static_cast < LOGFONT * > ( this ) );
	else
		lfCharSet = DEFAULT_CHARSET;

//	__EXT_MFC_STRCPY( lfFaceName, LF_FACESIZE, _T("MS Sans Serif") );
//	lfWeight = FW_NORMAL;
//	lfHeight = -11;
//	lfCharSet = DEFAULT_CHARSET;
//	lfQuality = PROOF_QUALITY;
	m_bHaveHeight = m_bHaveWidth = m_bHaveWeight = m_bHaveItalic
		= m_bHaveUnderline = m_bHaveStrikeOut = m_bHaveFaceName
//		= m_bHaveShadowOffset
		= m_bHavePitchAndFamily = m_bHaveCharSet
		= false;
	m_bComplete = false;
//	m_ptShadowOffset.x = m_ptShadowOffset.y = 0;
	m_strHashCode.Empty();
}


CExtSkinItem::CExtSkinItem()
{
	_Clear();
}

CExtSkinItem::~CExtSkinItem()
{
	_Clear();
}

void CExtSkinItem::Serialize( CArchive & ar )
{
	ASSERT_VALID( this );
	CExtSkinItemBase::Serialize( ar );
	if( ar.IsStoring() )
	{
		POSITION pos;
		DWORD dwFlags = 0, dwCount = 0;
		if( m_bEnabled )
			dwFlags |= 0x00000001;
		ar << dwFlags;

		dwCount = (DWORD)m_mapNestedItems.GetCount();
		ar << dwCount;
		for( pos = m_mapNestedItems.GetStartPosition(); pos != NULL; )
		{
			CExtSafeString strKey;
			CExtSkinItemBase * pValue;
			m_mapNestedItems.GetNextAssoc( pos, strKey, (void*&)pValue );
			ASSERT( pValue != NULL );
			ar << strKey;
			pValue->Serialize( ar );
		} // for( pos = m_mapNestedItems.GetStartPosition(); pos != NULL; )

		dwCount = (DWORD)m_mapGlyphs.GetCount();
		ar << dwCount;
		for( pos = m_mapGlyphs.GetStartPosition(); pos != NULL; )
		{
			CExtSafeString strKey;
			CExtSkinItemBase * pValue;
			m_mapGlyphs.GetNextAssoc( pos, strKey, (void*&)pValue );
			ASSERT( pValue != NULL );
			ar << strKey;
			pValue->Serialize( ar );
		} // for( pos = m_mapGlyphs.GetStartPosition(); pos != NULL; )

		dwCount = (DWORD)m_mapFonts.GetCount();
		ar << dwCount;
		for( pos = m_mapFonts.GetStartPosition(); pos != NULL; )
		{
			CExtSafeString strKey;
			CExtSkinItemBase * pValue;
			m_mapFonts.GetNextAssoc( pos, strKey, (void*&)pValue );
			ASSERT( pValue != NULL );
			ar << strKey;
			pValue->Serialize( ar );
		} // for( pos = m_mapFonts.GetStartPosition(); pos != NULL; )

		dwCount = (DWORD)m_mapColors.GetCount();
		ar << dwCount;
		for( pos = m_mapColors.GetStartPosition(); pos != NULL; )
		{
			CExtSafeString strKey;
			CExtSkinItemBase * pValue;
			m_mapColors.GetNextAssoc( pos, strKey, (void*&)pValue );
			ASSERT( pValue != NULL );
			ar << strKey;
			pValue->Serialize( ar );
		} // for( pos = m_mapColors.GetStartPosition(); pos != NULL; )

	} // if( ar.IsStoring() )
	else
	{
		_Clear();
		DWORD dwFlags = 0, dwIndex, dwCount = 0;
		ar >> dwFlags;
		if( ( dwFlags & 0x00000001 ) != 0 )
			m_bEnabled = true;
		else
			m_bEnabled = false;

		ar >> dwCount;
		for( dwIndex = 0; dwIndex < dwCount; dwIndex ++ )
		{
			CExtSafeString strKey;
			ar >> strKey;
			Nested( LPCTSTR(strKey) ).Serialize( ar );
		} // for( dwIndex = 0; dwIndex < dwCount; dwIndex ++ )

		ar >> dwCount;
		for( dwIndex = 0; dwIndex < dwCount; dwIndex ++ )
		{
			CExtSafeString strKey;
			ar >> strKey;
			CExtSkinGlyph * pValue = new CExtSkinGlyph;
			pValue->Serialize( ar );
			if( ! GlyphSet( pValue ) )
				delete pValue;
		} // for( dwIndex = 0; dwIndex < dwCount; dwIndex ++ )

		ar >> dwCount;
		for( dwIndex = 0; dwIndex < dwCount; dwIndex ++ )
		{
			CExtSafeString strKey;
			ar >> strKey;
			CExtSkinFont * pValue = new CExtSkinFont;
			pValue->Serialize( ar );
			if( ! FontSet( pValue ) )
				delete pValue;
		} // for( dwIndex = 0; dwIndex < dwCount; dwIndex ++ 

		ar >> dwCount;
		for( dwIndex = 0; dwIndex < dwCount; dwIndex ++ )
		{
			CExtSafeString strKey;
			ar >> strKey;
			CExtSkinColor * pValue = new CExtSkinColor;
			pValue->Serialize( ar );
			if( ! ColorSet( pValue ) )
				delete pValue;
		} // for( dwIndex = 0; dwIndex < dwCount; dwIndex ++ 
	} // else from if( ar.IsStoring() )
	ASSERT_VALID( this );
}

CExtSkinItem * CExtSkinItem::GetTopLevel()
{
	ASSERT_VALID( this );
	if( m_pParent == NULL )
		return this;
	return CExtSkinItemBase::GetTopLevel();
}

#if (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinItem::Load( __EXT_MFC_SAFE_LPCTSTR sXMLPath )
{
	ASSERT_VALID( this );
	ASSERT( sXMLPath != NULL );
	_Clear();
	if(		sXMLPath == NULL
		||	_tcslen( sXMLPath ) == 0
		)
		return false;
TCHAR sFullPath[1024], sSavedCurDir[1024], sNewCurDir[1024];
	::memset( sFullPath, 0, sizeof(sFullPath) );
	::memset( sSavedCurDir, 0, sizeof(sSavedCurDir) );
	::memset( sNewCurDir, 0, sizeof(sNewCurDir) );
	if( ! ::AfxFullPath( sFullPath, sXMLPath ) )
		return false;
	if( ::GetCurrentDirectory(
			sizeof(sSavedCurDir)/sizeof(TCHAR) - 1,
			sSavedCurDir
			) == 0
		)
		return false;

TCHAR sDrive[_MAX_DRIVE];
TCHAR sDir[_MAX_DIR];
TCHAR sFileName[_MAX_FNAME];
TCHAR sFileExt[_MAX_EXT];
	::memset( sDrive, 0, sizeof(sDrive) );
	::memset( sDir, 0, sizeof(sDir) );
	::memset( sFileName, 0, sizeof(sFileName) );
	::memset( sFileExt, 0, sizeof(sFileExt) );
	
	__EXT_MFC_SPLITPATH( 
		sFullPath, 
		sDrive, 
		_MAX_DRIVE,
		sDir, 
		_MAX_DIR,
		sFileName, 
		_MAX_FNAME,
		sFileExt,
		_MAX_EXT
		);
	__EXT_MFC_STRCPY( sNewCurDir, 1024, sDrive );
	__EXT_MFC_STRCAT( sNewCurDir, 1024, sDir );

	if( ! ::SetCurrentDirectory( sNewCurDir ) )
		return false;
MSXML::IXMLDOMDocumentPtr pDocumentPtr;
HRESULT hRes = pDocumentPtr.CreateInstance( __uuidof(MSXML::DOMDocument) );
	if( hRes != S_OK )
		return false;
	if( pDocumentPtr->load( _bstr_t( sFullPath ) ) == VARIANT_FALSE )
		return false;
bool bRetVal =
		Load( (IXMLElement*)pDocumentPtr->documentElement.GetInterfacePtr() );
	VERIFY( ::SetCurrentDirectory( sSavedCurDir ) );
	return bRetVal;
}

bool CExtSkinItem::Load( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	ASSERT( pElement != NULL );
	if( pElement == NULL )
		return false;
	if( ! CExtSkinItemBase::Load( pElement ) )
		return false;
	_Clear();
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
_variant_t varValue;
	try
	{
		if( ! _LoadGlyphsCollection( pElement ) )
			throw 0;
		if( ! _LoadNestedCollection( pElement ) )
			throw 0;
		if( ! _LoadFonts( pElement ) )
			throw 0;
		if( ! _LoadColors( pElement ) )
			throw 0;
		varValue = pElementPtr->getAttribute( __EXT_SKIN_ITEM_ENABLED );
		if( varValue.vt == VT_BSTR )
		{
			CString strEnabled( (BSTR)(_bstr_t)varValue );
			strEnabled.MakeLower();
			if( strEnabled == _T( "false" ) )
				m_bEnabled = false;
		}
	}
	catch( int )
	{
		ASSERT( FALSE );
		_Clear();
		return false;
	}
	ASSERT_VALID( this );
	return true;
}

#endif // (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinItem::EnabledGet() const
{
	ASSERT_VALID( this );
	return m_bEnabled;
}

void CExtSkinItem::EnabledSet( bool bEnable )
{
	ASSERT_VALID( this );
	m_bEnabled = bEnable;
}

bool CExtSkinItem::NameSet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
	if( _tcscmp( m_strName, strName ) == 0 )
		return true;
	if( m_pParent == NULL )
		return CExtSkinItemBase::NameSet( strName );
	if( m_pParent->NestedGet( strName ) )
		return false;
	return CExtSkinItemBase::NameSet( strName );
}

int CExtSkinItem::NestedGetCount()
{
	ASSERT_VALID( this );
	return (int)m_mapNestedItems.GetCount();
}

CExtSkinItem * CExtSkinItem::NestedGetNext( POSITION & pos )
{
	ASSERT_VALID( this );
	if( pos == NULL )
		pos = m_mapNestedItems.GetStartPosition();
	if( pos == NULL )
		return NULL;
void * pvObject = NULL;
CExtSafeString strKey;
	m_mapNestedItems.GetNextAssoc( pos, strKey, pvObject );
	return (CExtSkinItem*)pvObject;
}

CExtSkinItem * CExtSkinItem::NestedGet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
void * pvObject = NULL;
	if( ! m_mapNestedItems.Lookup( strName, pvObject ) )
		return NULL;
	ASSERT_VALID( ((CExtSkinItem*)pvObject) );
	return (CExtSkinItem*)pvObject;
}

const CExtSkinItem * CExtSkinItem::NestedGet( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this )->NestedGet( strName );
}

const CExtSkinItem & CExtSkinItem::Nested( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this )->Nested( strName );
}

CExtSkinItem & CExtSkinItem::Nested( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
CExtSkinItem * pItem = NestedGet( strName );
	if( pItem != NULL )
		return (*pItem);
CExtSkinItem * pNewItem = new CExtSkinItem;
	pNewItem->m_strName = strName;
	VERIFY( NestedSet( pNewItem ) );
	return (*pNewItem);
}

const CExtSkinItem & CExtSkinItem :: operator [] ( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> operator [] ( strName );
}

CExtSkinItem & CExtSkinItem :: operator [] ( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
	return Nested( strName );
}

bool CExtSkinItem::NestedSet( CExtSkinItem * pItem )
{
	ASSERT_VALID( this );
	if( pItem == NULL )
		return false;
	ASSERT_VALID( pItem );
__EXT_MFC_SAFE_LPCTSTR _strName = pItem->NameGet();
CExtSafeString strName;
	if( _strName == NULL || _tcslen( _strName ) == 0 )
	{
		static int g_nCounter = 0;
		strName.Format(
			_T("%d-%p"),
			g_nCounter,
			pItem
			);
		g_nCounter++;
	}
	else
		strName = LPCTSTR(_strName);
CExtSkinItem * pExistingItem = NestedGet( LPCTSTR(strName) );
	if( pExistingItem && pExistingItem != pItem )
		delete pExistingItem;
	pItem->m_pParent = this;
	m_mapNestedItems.SetAt( strName, pItem );
	return true;
}

bool CExtSkinItem::NestedRemove(
	__EXT_MFC_SAFE_LPCTSTR strName,
	CExtSkinItem ** ppItem // = NULL
	)
{
	ASSERT_VALID( this );
CExtSkinItem * pExistingItem = NestedGet( strName );
	if( pExistingItem == NULL )
		return false;
	if( ppItem != NULL )
		*ppItem = pExistingItem;
	else
		delete pExistingItem;
	m_mapNestedItems.RemoveKey( strName );
	return true;
}

void CExtSkinItem::NestedRemoveAll()
{
	ASSERT_VALID( this );
void * pvObject = NULL;
CExtSafeString strKey;
	for( POSITION pos = m_mapNestedItems.GetStartPosition(); pos; )
	{
		m_mapNestedItems.GetNextAssoc( pos, strKey, pvObject );
		CExtSkinItem * pSkinItem = (CExtSkinItem*)pvObject;
		ASSERT( pSkinItem );
		delete pSkinItem;
	}
	m_mapNestedItems.RemoveAll();
}

int CExtSkinItem::GlyphGetCount()
{
	ASSERT_VALID( this );
	return (int)m_mapGlyphs.GetCount();
}

CExtSkinGlyph * CExtSkinItem::GlyphGet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
void * pvObject = NULL;
	if( ! m_mapGlyphs.Lookup( strName, pvObject ) )
		return NULL;
	ASSERT_VALID( ((CExtSkinGlyph*)pvObject) );
	return (CExtSkinGlyph*)pvObject;
}

const CExtSkinGlyph * CExtSkinItem::GlyphGet( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> GlyphGet( strName );
}

const CExtSkinGlyph & CExtSkinItem::Glyph( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> Glyph( strName );
}

CExtSkinGlyph & CExtSkinItem::Glyph( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
CExtSkinGlyph * pGlyph = GlyphGetDeep( strName );
	if( pGlyph == NULL )
		::AfxThrowUserException();
	return (*pGlyph);
}

CExtSkinGlyph * CExtSkinItem::GlyphGetDeep( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
CExtSkinGlyph * pGlyph = GlyphGet( strName );
	if( pGlyph != NULL )
		return pGlyph;
CExtSkinItem * pParent = GetParent();
	if( pParent != NULL )
		return pParent->GlyphGetDeep( strName );
	return NULL;
}

const CExtSkinGlyph * CExtSkinItem::GlyphGetDeep( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> GlyphGetDeep( strName );
}

CExtSkinGlyph * CExtSkinItem::GlyphGetNext( POSITION & pos )
{
	ASSERT_VALID( this );
	if( pos == NULL )
		pos = m_mapGlyphs.GetStartPosition();
	if( pos == NULL )
		return NULL;
void * pvObject = NULL;
CExtSafeString strKey;
	m_mapGlyphs.GetNextAssoc( pos, strKey, pvObject );
	return (CExtSkinGlyph*)pvObject;
}

bool CExtSkinItem::GlyphSet( CExtSkinGlyph * pGlyph )
{
	ASSERT_VALID( this );
	if( pGlyph == NULL )
		return false;
	ASSERT_VALID( pGlyph );
__EXT_MFC_SAFE_LPCTSTR strName = pGlyph->NameGet();
	if( strName == NULL || _tcslen( strName ) == 0 )
		return false;
CExtSkinGlyph * pExistingGlyph = GlyphGet( strName );
	if( pExistingGlyph && pExistingGlyph != pGlyph )
		delete pExistingGlyph;
	pGlyph->m_pParent = this;
	m_mapGlyphs.SetAt( strName, pGlyph );
	return true;
}

bool CExtSkinItem::GlyphRemove(
	__EXT_MFC_SAFE_LPCTSTR strName,
	CExtSkinGlyph ** ppGlyph // = NULL
	)
{
	ASSERT_VALID( this );
CExtSkinGlyph * pExistingGlyph = GlyphGet( strName );
	if( pExistingGlyph == NULL )
		return false;
	if( ppGlyph != NULL )
		(*ppGlyph) = pExistingGlyph;
	else
        delete pExistingGlyph;
	m_mapGlyphs.RemoveKey( strName );
	return true;
}

void CExtSkinItem::GlyphRemoveAll()
{
	ASSERT_VALID( this );
void * pvObject = NULL;
CExtSafeString strKey;
	for( POSITION pos = m_mapGlyphs.GetStartPosition(); pos; )
	{
		m_mapGlyphs.GetNextAssoc( pos, strKey, pvObject );
		CExtSkinGlyph * pSkinGlyph = (CExtSkinGlyph*)pvObject;
		ASSERT( pSkinGlyph );
		delete pSkinGlyph;
	}
	m_mapGlyphs.RemoveAll();
}

int CExtSkinItem::FontGetCount()
{
	ASSERT_VALID( this );
	return (int)m_mapFonts.GetCount();
}

CExtSkinFont * CExtSkinItem::FontGet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
void * pvObject = NULL;
	if( ! m_mapFonts.Lookup( strName, pvObject ) )
		return NULL;
	ASSERT_VALID( ((CExtSkinFont*)pvObject) );
	return (CExtSkinFont*)pvObject;
}

const CExtSkinFont * CExtSkinItem::FontGet( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> FontGet( strName );
}

const CExtSkinFont & CExtSkinItem::Font( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> Font( strName );
}

CExtSkinFont & CExtSkinItem::Font( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
CExtSkinFont * pFont = FontGetDeep( strName );
	if( pFont == NULL )
		::AfxThrowUserException();
	return *pFont;
}

CExtSkinFont * CExtSkinItem::FontGetDeep( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
CExtSkinFont * pFont = FontGet( strName );
	if( pFont != NULL )
		return pFont;
CExtSkinItem * pParent = GetParent();
	if( pParent != NULL )
		return pParent->FontGetDeep( strName );
	return NULL;
}

const CExtSkinFont * CExtSkinItem::FontGetDeep( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> FontGetDeep( strName );
}

CExtSkinFont * CExtSkinItem::FontGetNext( POSITION & pos )
{
	ASSERT_VALID( this );
	if( pos == NULL )
		pos = m_mapFonts.GetStartPosition();
	if( pos == NULL )
		return NULL;
void * pvObject = NULL;
CExtSafeString strKey;
	m_mapFonts.GetNextAssoc( pos, strKey, pvObject );
	return (CExtSkinFont*)pvObject;
}

bool CExtSkinItem::FontSet( CExtSkinFont * pFont )
{
	ASSERT_VALID( this );
	if( pFont == NULL )
		return false;
	ASSERT_VALID( pFont );
__EXT_MFC_SAFE_LPCTSTR strName = pFont->NameGet();
	if( strName == NULL || _tcslen( strName ) == 0 )
		return false;
CExtSkinFont * pExistingFont = FontGet( strName );
	if( pExistingFont && pExistingFont != pFont )
		delete pExistingFont;
	pFont->m_pParent = this;
	m_mapFonts.SetAt( strName, pFont );
	return true;
}

bool CExtSkinItem::FontRemove(
	__EXT_MFC_SAFE_LPCTSTR strName,
	CExtSkinFont ** ppFont //= NULL
	)
{
	ASSERT_VALID( this );
CExtSkinFont * pExistingFont = FontGet( strName );
	if( pExistingFont == NULL )
		return false;
	if( ppFont != NULL )
		(*ppFont) = pExistingFont;
	else
		delete pExistingFont;
	m_mapFonts.RemoveKey( strName );
	return true;
}

void CExtSkinItem::FontRemoveAll()
{
	ASSERT_VALID( this );
void * pvObject = NULL;
CExtSafeString strKey;
	for( POSITION pos = m_mapFonts.GetStartPosition(); pos; )
	{
		m_mapFonts.GetNextAssoc( pos, strKey, pvObject );
		CExtSkinFont * pSkinFont = (CExtSkinFont*)pvObject;
		ASSERT( pSkinFont );
		delete pSkinFont;
	}
	m_mapFonts.RemoveAll();
}

int CExtSkinItem::ColorGetCount()
{
	ASSERT_VALID( this );
	return (int)m_mapColors.GetCount();
}

CExtSkinColor * CExtSkinItem::ColorGet( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
void * pvObject = NULL;
	if( ! m_mapColors.Lookup( strName, pvObject ) )
		return NULL;
	return (CExtSkinColor*)pvObject;
}

const CExtSkinColor * CExtSkinItem::ColorGet( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> ColorGet( strName );
}

const CExtSkinColor & CExtSkinItem::Color( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> Color( strName );
}

CExtSkinColor & CExtSkinItem::Color( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
CExtSkinColor * pColor = ColorGetDeep( strName );
	if( pColor == NULL )
		::AfxThrowUserException();
	return *pColor;
}

CExtSkinColor * CExtSkinItem::ColorGetDeep( __EXT_MFC_SAFE_LPCTSTR strName )
{
	ASSERT_VALID( this );
CExtSkinColor * pColor = ColorGet( strName );
	if( pColor != NULL )
		return pColor;
CExtSkinItem * pParent = GetParent();
	if( pParent != NULL )
		return pParent->ColorGetDeep( strName );
	return NULL;
}

const CExtSkinColor * CExtSkinItem::ColorGetDeep( __EXT_MFC_SAFE_LPCTSTR strName ) const
{
	ASSERT_VALID( this );
	return const_cast < CExtSkinItem * > ( this ) -> ColorGetDeep( strName );
}

CExtSkinColor * CExtSkinItem::ColorGetNext( POSITION & pos )
{
	ASSERT_VALID( this );
	if( pos == NULL )
		pos = m_mapColors.GetStartPosition();
	if( pos == NULL )
		return NULL;
void * pvObject = NULL;
CExtSafeString strKey;
	m_mapColors.GetNextAssoc( pos, strKey, pvObject );
	return (CExtSkinColor*)pvObject;
}

bool CExtSkinItem::ColorSet( CExtSkinColor * pColor )
{
	ASSERT_VALID( this );
	if( pColor == NULL )
		return false;
	ASSERT_VALID( pColor );
__EXT_MFC_SAFE_LPCTSTR strName = pColor->NameGet();
	if( strName == NULL || _tcslen( strName ) == 0 )
		return false;
CExtSkinColor * pExistingColor = ColorGet( strName );
	if( pExistingColor && pExistingColor != pColor )
		delete pExistingColor;
	pColor->m_pParent = this;
	m_mapColors.SetAt( strName, pColor );
	return true;
}

bool CExtSkinItem::ColorRemove(
	__EXT_MFC_SAFE_LPCTSTR strName,
	CExtSkinColor ** ppColor //= NULL
	)
{
	ASSERT_VALID( this );
CExtSkinColor * pExistingColor = ColorGet( strName );
	if( pExistingColor == NULL )
		return false;
	if( ppColor != NULL )
		*ppColor = pExistingColor;
	else
		delete pExistingColor;
	m_mapColors.RemoveKey( strName );
	return true;
}

void CExtSkinItem::ColorRemoveAll()
{
	ASSERT_VALID( this );
	void * pvObject = NULL;
	CExtSafeString strKey;
	for( POSITION pos = m_mapColors.GetStartPosition(); pos; )
	{
		m_mapColors.GetNextAssoc( pos, strKey, pvObject );
		CExtSkinColor * pSkinColor = (CExtSkinColor*)pvObject;
		ASSERT( pSkinColor );
		delete pSkinColor;
	}
	m_mapColors.RemoveAll();
}

void CExtSkinItem::_Clear()
{
	ASSERT_VALID( this );
	m_bEnabled = true;
	NestedRemoveAll();
	GlyphRemoveAll();
	FontRemoveAll();
	ColorRemoveAll();
}

#if (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkinItem::_LoadGlyphsCollection( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	ASSERT( pElement != NULL );
	if( pElement == NULL )
		return false;
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
MSXML::IXMLDOMNodeListPtr pListPtr = 
		pElementPtr->selectNodes( _bstr_t( __EXT_SKIN_ITEM_GLYPH ) );
	ASSERT( pListPtr != NULL );
	if( pListPtr == NULL || pListPtr->length <= 0 )
		return true;
	for( long lItem = 0; lItem < pListPtr->length; ++lItem )
	{
		MSXML::IXMLDOMElementPtr pGlyphPtr = pListPtr->item[lItem];
		ASSERT( pGlyphPtr != NULL );
		if( pGlyphPtr == NULL )
			continue;

		CExtSkinGlyph * pGlyph = new CExtSkinGlyph;
		if( !pGlyph->Load( (IXMLElement*)pGlyphPtr.GetInterfacePtr() ) || !GlyphSet( pGlyph ) )
		{
			ASSERT( FALSE );
			delete pGlyph;
			continue;
		}
	}
	ASSERT_VALID( this );
	return true;
}

bool CExtSkinItem::_LoadNestedCollection( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	ASSERT( pElement != NULL );
	if( pElement == NULL )
		return false;
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
MSXML::IXMLDOMNodeListPtr pListPtr = 
		pElementPtr->selectNodes( _bstr_t( __EXT_SKIN_ITEM ) );
	ASSERT( pListPtr != NULL );
	if( pListPtr == NULL || pListPtr->length <= 0 )
		return true;
	for( long lItem = 0; lItem < pListPtr->length; ++lItem )
	{
		MSXML::IXMLDOMElementPtr pItemPtr = pListPtr->item[lItem];
		ASSERT( pItemPtr != NULL );
		if( pItemPtr == NULL )
			continue;

		CExtSkinItem * pItem = new CExtSkinItem;
		if( !pItem->Load( (IXMLElement*)pItemPtr.GetInterfacePtr() ) || !NestedSet( pItem ) )
		{
			ASSERT( FALSE );
			delete pItem;
			continue;
		}
	}
	ASSERT_VALID( this );
	return true;
}

bool CExtSkinItem::_LoadFonts( IXMLElement * pElement )
{
	ASSERT( pElement );
	ASSERT_VALID( this );
	if( pElement == NULL )
		return false;
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
MSXML::IXMLDOMNodeListPtr pListPtr = 
		pElementPtr->selectNodes( _bstr_t( __EXT_SKIN_ITEM_FONT ) );
	ASSERT( pListPtr != NULL );
	if( pListPtr == NULL || pListPtr->length <= 0 )
		return true;
	for( long lItem = 0; lItem < pListPtr->length; ++lItem )
	{
		MSXML::IXMLDOMElementPtr pItemPtr = pListPtr->item[lItem];
		ASSERT( pItemPtr != NULL );
		if( pItemPtr == NULL )
			continue;

		CExtSkinFont * pFont = new CExtSkinFont;
		if( !pFont->Load( (IXMLElement*)pItemPtr.GetInterfacePtr() ) || !FontSet( pFont ) )
		{
			ASSERT( FALSE );
			delete pFont;
			continue;
		}
	}
	ASSERT_VALID( this );
	return true;
}

bool CExtSkinItem::_LoadColors( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	ASSERT( pElement != NULL );
	if( pElement == NULL )
		return false;
MSXML::IXMLDOMElementPtr pElementPtr = pElement;
MSXML::IXMLDOMNodeListPtr pListPtr = 
		pElementPtr->selectNodes( _bstr_t( __EXT_SKIN_ITEM_COLOR ) );
	ASSERT( pListPtr != NULL );
	if( pListPtr == NULL || pListPtr->length <= 0 )
		return true;
	for( long lItem = 0; lItem < pListPtr->length; ++lItem )
	{
		MSXML::IXMLDOMElementPtr pItemPtr = pListPtr->item[lItem];
		ASSERT( pItemPtr != NULL );
		if( pItemPtr == NULL )
			continue;

		CExtSkinColor * pColor = new CExtSkinColor;
		if( !pColor->Load( (IXMLElement*)pItemPtr.GetInterfacePtr() ) || !ColorSet( pColor ) )
		{
			ASSERT( FALSE );
			delete pColor;
			continue;
		}
	}
	ASSERT_VALID( this );
	return true;
}

#endif // (defined __PROF_SKIN_XML_SUPPORT)

CExtSkin::CExtSkin()
{
}

CExtSkin::~CExtSkin()
{
}

void CExtSkin::_Clear()
{
	ASSERT_VALID( this );
	CExtSkinItem::_Clear();
	m_SBSD_ClassicH.Empty();
	m_SBSD_ClassicV.Empty();
	m_SBSD_ZoomH.Empty();
	m_SBSD_ZoomV.Empty();
}

void CExtSkin::SynchronizeScrollBarElements(
	__EXT_MFC_SAFE_LPCTSTR strScrollBarTypeBranch,
	__EXT_MFC_SAFE_LPCTSTR strScrollBarOrientationBranch,
	CExtPaintManager::SCROLLBARSKINDATA * pSBSD
	)
{
	ASSERT_VALID( this );
	ASSERT( LPCTSTR(strScrollBarTypeBranch) != NULL );
	ASSERT( _tcslen( LPCTSTR(strScrollBarTypeBranch) ) > 0 );
	ASSERT( LPCTSTR(strScrollBarOrientationBranch) != NULL );
	ASSERT( _tcslen( LPCTSTR(strScrollBarOrientationBranch) ) > 0 );
	ASSERT( pSBSD != NULL );
	pSBSD->Empty();
CExtSkinItem * pItemWalkTypeAndOrientation = NestedGet( __EXT_SKIN_ITEM_SCROLL_BAR_ROOT_T );
	if( pItemWalkTypeAndOrientation == NULL )
		return;
	pItemWalkTypeAndOrientation = pItemWalkTypeAndOrientation->NestedGet( strScrollBarTypeBranch );
	if( pItemWalkTypeAndOrientation == NULL )
		return;
	pItemWalkTypeAndOrientation = pItemWalkTypeAndOrientation->NestedGet( strScrollBarOrientationBranch );
	if( pItemWalkTypeAndOrientation == NULL )
		return;
LPCTSTR arr1[INT(CExtPaintManager::__ESBSPT_PART_COUNT)] =
{
	_T("Disabled"),
	_T("Cold"),
	_T("Normal"),
	_T("Hot"),
	_T("Pressed"),
};
LPCTSTR arr2[INT(CExtPaintManager::__ESBMHT_PART_COUNT)] =
{
	_T("SolidScrollableArea"),
	_T("ButtonUp"),
	_T("ButtonDown"),
	_T("ButtonThumb"),
	_T("PageUp"),
	_T("PageDown"),
	_T("ArrowUp"),
	_T("ArrowDown"),
	_T("ThumbGripper"),
};
INT nIndex1, nIndex2;
	for( nIndex1 = 0; nIndex1 < INT(CExtPaintManager::__ESBSPT_PART_COUNT); nIndex1++ )
	{
		CExtSkinItem * pItem1 = pItemWalkTypeAndOrientation->NestedGet( (arr1[nIndex1]) );
		if( pItem1 == NULL )
			continue;
		for( nIndex2 = 0; nIndex2 < INT(CExtPaintManager::__ESBMHT_PART_COUNT); nIndex2++ )
		{
			CExtSkinGlyph * pGlyph = pItem1->GlyphGet( arr2[nIndex2] );
			if( pGlyph == NULL )
				continue;
			CExtBitmap & _bmp = pGlyph->GetBitmap();
			if( _bmp.IsEmpty() )
				continue;
			pSBSD->m_arrPartPadding[nIndex1][nIndex2] = pGlyph->GetPadding();
			pSBSD->m_arrPartBmp[nIndex1][nIndex2] = _bmp;
		} // for( nIndex2 = 0; nIndex2 < INT(CExtPaintManager::__ESBMHT_PART_COUNT); nIndex2++ )
	} // for( nIndex1 = 0; nIndex1 < INT(CExtPaintManager::__ESBSPT_PART_COUNT); nIndex1++ )
}

void CExtSkin::SynchronizeScrollBars()
{
	ASSERT_VALID( this );
	SynchronizeScrollBarElements( __EXT_SKIN_ITEM_SCROLL_BAR_TYPE_ZOOM_T, __EXT_SKIN_ITEM_SCROLL_BAR_ORIENTATION_H_T, &m_SBSD_ZoomH );
	SynchronizeScrollBarElements( __EXT_SKIN_ITEM_SCROLL_BAR_TYPE_ZOOM_T, __EXT_SKIN_ITEM_SCROLL_BAR_ORIENTATION_V_T, &m_SBSD_ZoomV );
	SynchronizeScrollBarElements( __EXT_SKIN_ITEM_SCROLL_BAR_TYPE_CLASSIC_T, __EXT_SKIN_ITEM_SCROLL_BAR_ORIENTATION_H_T, &m_SBSD_ClassicH );
	SynchronizeScrollBarElements( __EXT_SKIN_ITEM_SCROLL_BAR_TYPE_CLASSIC_T, __EXT_SKIN_ITEM_SCROLL_BAR_ORIENTATION_V_T, &m_SBSD_ClassicV );
}

#if (defined __PROF_SKIN_XML_SUPPORT)

bool CExtSkin::Load( __EXT_MFC_SAFE_LPCTSTR sXMLPath )
{
	ASSERT_VALID( this );
bool bRetVal = CExtSkinItem::Load( sXMLPath );
	return bRetVal;
}

bool CExtSkin::Load( IXMLElement * pElement )
{
	ASSERT_VALID( this );
	m_SBSD_ClassicH.Empty();
	m_SBSD_ClassicV.Empty();
	m_SBSD_ZoomH.Empty();
	m_SBSD_ZoomV.Empty();
bool bRetVal = CExtSkinItem::Load( pElement );
	SynchronizeScrollBars();
	return bRetVal;
}

#endif // (defined __PROF_SKIN_XML_SUPPORT)

void CExtSkin::Serialize( CArchive & ar )
{
	ASSERT_VALID( this );
	if( ar.IsStoring() )
	{
		DWORD dwSerialFormatNumer = __EXT_SKIN_SERIAL_FORMAT_NUMBER;
		ar << dwSerialFormatNumer;
	} // if( ar.IsStoring() )
	else
	{
		_Clear();
		m_SBSD_ClassicH.Empty();
		m_SBSD_ClassicV.Empty();
		m_SBSD_ZoomH.Empty();
		m_SBSD_ZoomV.Empty();
		DWORD dwSerialFormatNumer = 0;
		ar >> dwSerialFormatNumer;
	} // else from if( ar.IsStoring() )
	CExtSkinItem::Serialize( ar );
	ASSERT_VALID( this );
	if( ar.IsLoading() )
		SynchronizeScrollBars();
}

const BYTE CExtZLibStreamCacheArchive::g_arrSignatureDefault[
		__EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE
		] = 
	{ 'P', 'A', 'C', 'K' };

CExtZLibStreamCacheArchive::CExtZLibStreamCacheArchive(
	CFile * pArchiveFile,
	UINT nArchiveMode,
	bool bUseCompression, // = true
	__EXT_MFC_SAFE_LPCTSTR strMessKey, // = _T("")
	const LPBYTE arrSignature, // = NULL
	INT nArchiveBufferSize, // = 4096
	LPVOID pArchiveBuffer // = NULL
	)
	: CArchive( pArchiveFile, nArchiveMode, nArchiveBufferSize, pArchiveBuffer )
	, m_strMessKey( strMessKey != NULL ? strMessKey : _T("") )
	, m_bUseCompression( bUseCompression )
	, m_pDataFile( m_pFile )
{
	__EXT_MFC_MEMCPY(
		m_arrSignature,
		__EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE,
		arrSignature == NULL ? g_arrSignatureDefault : arrSignature,
		__EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE
		);
	if( ! m_strMessKey.IsEmpty() )
		m_bUseCompression = true;
	if( m_bUseCompression )
	{
		if( IsStoring() )
		{
			m_pDataBuffer =
				new BYTE[ __EXT_ZLIB_STREAM_ARCHIVE_BUFFER_ALLOC_SIZE ];
			m_pFile =
				new CMemFile(
					m_pDataBuffer,
					__EXT_ZLIB_STREAM_ARCHIVE_BUFFER_ALLOC_SIZE,
					__EXT_ZLIB_STREAM_ARCHIVE_BUFFER_GROW_SIZE
					);
		} // if( IsStoring() )
		else
		{
			CExtSafeString bufferSrc, bufferPacked;
			DWORD dwDataSize = (DWORD) m_pDataFile->GetLength();
			LPBYTE pBuffer = (LPBYTE)bufferSrc.GetBuffer( dwDataSize );
			m_pDataBuffer = new BYTE[ dwDataSize ];
			m_pFile =
				new CMemFile(
					m_pDataBuffer,
					dwDataSize,
					__EXT_ZLIB_STREAM_ARCHIVE_BUFFER_GROW_SIZE
					);
			m_pDataFile->Seek( 0, CFile::begin );
			StreamHeader_t _StreamHeader;
			m_pDataFile->Read( &_StreamHeader, sizeof(_StreamHeader) );
			bool bSignatureFound =
				( ::memcmp(
					m_arrSignature,
					_StreamHeader.m_arrSignature,
					__EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE
					) == 0
				) ? true : false;
			if( bSignatureFound )
			{
				dwDataSize = m_pDataFile -> __ReadHuge__ ( pBuffer, dwDataSize );
				if( _StreamHeader.m_nMessMode == 3 )
				{
					ULONG nCRC = adler32( 0L, Z_NULL, 0 );
					OnMess( pBuffer, dwDataSize, m_strMessKey );
					nCRC = adler32( nCRC, pBuffer, dwDataSize );
					if( nCRC != _StreamHeader.m_nCRC )
					{
						// password incorrect 
						Abort();
					#if _MFC_VER >= 0x0800
						::AfxThrowArchiveException( CArchiveException::genericException );
					#else
						::AfxThrowArchiveException( CArchiveException::generic );
					#endif
					} // if( nCRC != _StreamHeader.m_nCRC )
				} // if( _StreamHeader.m_nMessMode == 3 )
				DWORD dwSizeCompressed = _StreamHeader.m_dwDataSize;
				LPBYTE pBufferPacked = (LPBYTE)
					bufferPacked.GetBuffer( dwSizeCompressed );
				int nCompressionRetVal =
					uncompress(
						pBufferPacked,
						&dwSizeCompressed,
						(LPBYTE)pBuffer,
						dwDataSize
						);
				ASSERT( nCompressionRetVal == Z_OK );
				if( nCompressionRetVal != Z_OK )
				#if _MFC_VER >= 0x0800
					::AfxThrowArchiveException( CArchiveException::genericException );
				#else
					::AfxThrowArchiveException( CArchiveException::generic );
				#endif
				m_pFile -> __WriteHuge__ ( pBufferPacked, dwSizeCompressed );
			} // if( bSignatureFound )
			else
			{
				m_pDataFile->Seek( 0, CFile::begin );
				dwDataSize = m_pDataFile -> __ReadHuge__ ( pBuffer, dwDataSize );
				m_pFile -> __WriteHuge__ ( pBuffer, dwDataSize );
			} // else from if( bSignatureFound )
			m_pFile->Seek( 0, CFile::begin );
		} // else from if( IsStoring() )
	} // if( m_bUseCompression )
}

CExtZLibStreamCacheArchive::~CExtZLibStreamCacheArchive()
{
	if(		m_pFile != NULL
		&&	( m_nMode & bNoFlushOnDelete ) == 0
		)
		Close();
	Abort();
}

bool CExtZLibStreamCacheArchive::OnMess(
	LPBYTE pBuffer,
	ULONG nLength,
	__EXT_MFC_SAFE_LPCTSTR strMessKey
	) const
{
	if( pBuffer == NULL )
		return false;
int nKeyLength = int( _tcslen( strMessKey ) );
	if( nKeyLength == 0 )
		return false;
	for( ULONG i = 0; i < nLength; i++ )
	{
		for( int j = 0; j < nKeyLength; j++ )
			pBuffer[ i ] ^= BYTE( strMessKey[ j ] );
	}
	return true;
}

void CExtZLibStreamCacheArchive::Abort()
{
	if(		m_bUseCompression
		&&	m_pFile != NULL
		)
	{
		m_pDataBuffer = LPBYTE(((CMemFile*)m_pFile)->Detach());
		delete [] m_pDataBuffer;
		m_pFile->Close();
		delete m_pFile;
	}
	CArchive::Abort();
}

void CExtZLibStreamCacheArchive::Close()
{
	ASSERT_VALID( m_pFile );
	Flush();
	if( m_bUseCompression )
	{
		if( IsStoring() )
		{
			CExtSafeString bufferPacked;
			DWORD dwDataSize = (DWORD) m_pFile->GetLength();
			m_pDataBuffer = LPBYTE(((CMemFile*)m_pFile)->Detach());
			DWORD dwSizeCompressed = (DWORD)( dwDataSize * 1.1 ) + 12;
			LPBYTE pBufferPacked = (LPBYTE)
				bufferPacked.GetBuffer( dwSizeCompressed );
			INT nCompressionRetVal =
				compress(
					pBufferPacked,
					&dwSizeCompressed,
					m_pDataBuffer,
					dwDataSize
					);
			ASSERT( nCompressionRetVal == Z_OK );
			if( nCompressionRetVal != Z_OK )
			#if _MFC_VER >= 0x0800
				::AfxThrowArchiveException( CArchiveException::genericException );
			#else
				::AfxThrowArchiveException( CArchiveException::generic );
			#endif
			ULONG nCRC = adler32( 0L, Z_NULL, 0 );
			nCRC = adler32( nCRC, pBufferPacked, dwSizeCompressed );
			OnMess( pBufferPacked, dwSizeCompressed, m_strMessKey );
			StreamHeader_t _StreamHeader;
			__EXT_MFC_MEMCPY(
				_StreamHeader.m_arrSignature, 
				__EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE,
				m_arrSignature, 
				__EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE
				);
			_StreamHeader.m_dwDataSize = dwDataSize;
			_StreamHeader.m_nMessMode = BYTE( m_strMessKey.IsEmpty() ? 1 : 3 );
			_StreamHeader.m_nCRC = nCRC;
			m_pDataFile->Write( &_StreamHeader, sizeof(_StreamHeader) );
			m_pDataFile -> __WriteHuge__ ( pBufferPacked, dwSizeCompressed );
		} // if( IsStoring() )
		else
		{
			m_pDataBuffer = LPBYTE(((CMemFile*)m_pFile)->Detach());
		} // else from if( IsStoring() )
		delete [] m_pDataBuffer;
		m_pFile->Close();
		delete m_pFile;
	} // if( m_bUseCompression )
	m_pFile = NULL;
}


