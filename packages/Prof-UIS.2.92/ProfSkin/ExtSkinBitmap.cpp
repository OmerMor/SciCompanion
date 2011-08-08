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

#if (!defined __PROF_SKIN_BITMAP_H)
	#include <../ProfSkin/ExtSkinBitmap.h>
#endif

#include <PNG.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExtSkinBitmap::CExtSkinBitmap()
{
}

CExtSkinBitmap::CExtSkinBitmap(
	const CExtBitmap & other
	)
	: CExtBitmapCache( other )
{
}

CExtSkinBitmap::~CExtSkinBitmap()
{
	Empty();
}

bool CExtSkinBitmap::AssignFromOther(
	const CExtBitmap & other,
	bool bEnableThrowException // = false
	)
{
bool bRetVal =
		CExtBitmapCache::AssignFromOther(
			other,
			bEnableThrowException
			);
	return bRetVal;
}

void CExtSkinBitmap::Empty()
{
	CExtBitmapCache::Empty();
}

bool CExtSkinBitmap::IsEmpty() const
{
bool bRetVal = CExtBitmapCache::IsEmpty();
	return bRetVal;
}

bool CExtSkinBitmap::LoadImage_File(
		__EXT_MFC_SAFE_LPCTSTR strFilePath,
		bool bMake32, //= false
		bool bEnableThrowException //= false
		)
{
	if( !strFilePath || _tcslen( strFilePath ) == 0 )
		return false;

	TCHAR sExt[_MAX_EXT] = { 0 };
	__EXT_MFC_SPLITPATH( strFilePath, NULL, 0, NULL, 0, NULL, 0, sExt, _MAX_EXT );
	
	if( _tcsicmp( sExt, _T( ".png" ) ) == 0 )
	{
		if( ! LoadPNG_File( strFilePath, bMake32, bEnableThrowException ) )
			return false;
	
		return true;
	}
	else if( _tcsicmp( sExt, _T( ".bmp" ) ) == 0 )
		return LoadBMP_File( strFilePath, bMake32, bEnableThrowException );

	if( LoadPNG_File( strFilePath, bMake32, false ) )
		return true;

	if( LoadBMP_File( strFilePath, bMake32, false ) )
		return true;

	if( bEnableThrowException )
		AfxThrowUserException();

	return false;
}

bool CExtSkinBitmap::LoadPNG_Resource(
	HINSTANCE hInst,
	HRSRC hRsrc,
	bool bMake32, // = true
	bool bEnableThrowException // = false
	)
{
HGLOBAL hGlobal = ::LoadResource( hInst, hRsrc );
	if( hGlobal == NULL )
	{
		if( bEnableThrowException )
			::AfxThrowUserException();
		return false;
	} // if( hGlobal == NULL )
LPBYTE pData = (LPBYTE)::LockResource( hGlobal );
DWORD dwResourceSize = ::SizeofResource( hInst, hRsrc );
	ASSERT( dwResourceSize > 0 );
	if( pData == NULL )
	{
		::FreeResource( hGlobal );
		if( bEnableThrowException )
			::AfxThrowUserException();
		return false;
	} // if( pData == NULL )
	try
	{
		CMemFile _file;
		_file.Attach( pData, dwResourceSize );
		_file.Seek( 0, CFile::begin );
		CArchive ar( &_file, CArchive::load );
		bool bRetVal =
			LoadPNG(
				ar,
				bMake32,
				bEnableThrowException
				);
		::UnlockResource( hGlobal );
		::FreeResource( hGlobal );
		return bRetVal;
	} // try
	catch( CException * pException )
	{
		::UnlockResource( hGlobal );
		::FreeResource( hGlobal );
		if( bEnableThrowException )
			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
		::UnlockResource( hGlobal );
		::FreeResource( hGlobal );
		if( bEnableThrowException )
			throw;
	} // catch( ... )
	return false;
}

bool CExtSkinBitmap::LoadPNG_File(
	__EXT_MFC_SAFE_LPCTSTR strFilePath,
	bool bMake32, // = true
	bool bEnableThrowException // = false
	)
{
	try
	{
		CFile _file(
			strFilePath,
			CFile::modeRead
			| CFile::typeBinary
			);
		return
			LoadPNG_File(
				_file,
				bMake32,
				bEnableThrowException
				);
	} // try
	catch( ... )
	{
		if( bEnableThrowException )
			throw;
	} // catch( ... )
	return false;
}

bool CExtSkinBitmap::LoadPNG_File(
	CFile & _file,
	bool bMake32, // = true
	bool bEnableThrowException // = false
	)
{
CArchive ar(
		&_file,
		CArchive::load
		);
	return
		LoadPNG(
			ar,
			bMake32,
			bEnableThrowException
			);
}

bool CExtSkinBitmap::LoadPNG(
	CArchive & ar,
	bool bMake32, // = true
	bool bEnableThrowException // = false
	)
{
	ASSERT( ar.IsLoading() );
	Empty();
	m_pImageBuffer =
		stat_PngLoad(
			ar,
			bEnableThrowException
			);
	if( m_pImageBuffer == NULL )
		return false;
	OnLoad();
	if( bMake32 )
		return Make32( bEnableThrowException );
	return true;
}

bool CExtSkinBitmap::SavePNG_File(
	__EXT_MFC_SAFE_LPCTSTR strFilePath,
	bool bMake32, // = true
	bool bEnableThrowException // = false
	)
{
	try
	{
		CFile _file(
			strFilePath,
			CFile::modeCreate
				| CFile::modeWrite
				| CFile::typeBinary
			);
		return
			SavePNG_File(
				_file,
				bMake32,
				bEnableThrowException
				);
	} // try
	catch( ... )
	{
		if( bEnableThrowException )
			throw;
	} // catch( ... )
	return false;
}

bool CExtSkinBitmap::SavePNG_File(
	CFile & _file,
	bool bMake32, // = true
	bool bEnableThrowException // = false
	)
{
CArchive ar(
		&_file,
		CArchive::store
		);
	return
		SavePNG(
			ar,
			bMake32,
			bEnableThrowException
			);
}

bool CExtSkinBitmap::SavePNG(
	CArchive & ar,
	bool bMake32, // = true
	bool bEnableThrowException // = false
	)
{
	ASSERT( ar.IsStoring() );
	if( m_pImageBuffer == NULL )
	{
		ASSERT( FALSE );
		if( bEnableThrowException )
			::AfxThrowUserException();
		return false;
	} // if( m_pImageBuffer == NULL )
	if( bMake32 )
	{
		if( ! Make32( bEnableThrowException ) )
			return false;
	} // if( bMake32 )
	return
		stat_PngSave(
			m_pImageBuffer,
			ar,
			bEnableThrowException
			);
}

void CExtSkinBitmap::stat_PngCallbackRead(
	LPVOID pCallbackData,
	LPBYTE pBuffer,
	size_t nSize
	)
{
	ASSERT( pCallbackData != NULL );
CArchive * pAr = (CArchive *)
		::png_get_io_ptr(
			(png_structp) pCallbackData
			);
	ASSERT( pAr != NULL );
	pAr->Read( pBuffer, UINT(nSize) );
}

void CExtSkinBitmap::stat_PngCallbackWrite(
	LPVOID pCallbackData,
	LPBYTE pBuffer,
	size_t nSize
	)
{
	ASSERT( pCallbackData != NULL );
CArchive * pAr = (CArchive *)
		::png_get_io_ptr(
			(png_structp) pCallbackData
			);
	ASSERT( pAr != NULL );
	pAr->Write( pBuffer, UINT(nSize) );
}

void CExtSkinBitmap::stat_PngCallbackFlush(
	LPVOID pCallbackData
	)
{
	ASSERT( pCallbackData != NULL );
CArchive * pAr = (CArchive *)
		::png_get_io_ptr(
			(png_structp) pCallbackData
			);
	ASSERT( pAr != NULL );
	pAr->Flush();
}

void CExtSkinBitmap::stat_PngCallbackError(
	LPVOID pCallbackData,
	const char * strErrorDescription
	)
{
	pCallbackData;
	strErrorDescription;
	::AfxThrowUserException();
}

void CExtSkinBitmap::stat_PngCallbackWarning(
	LPVOID pCallbackData,
	const char * strWarningDescription
	)
{
	pCallbackData;
	strWarningDescription;
}

bool CExtSkinBitmap::stat_PngCheckSwap32(
	LPBYTE pImageBuffer
	)
{
	ASSERT( pImageBuffer != NULL );
LPBITMAPINFOHEADER pBIH = stat_RcInfoHeader( pImageBuffer );
	if( pBIH->biBitCount == 32 || pBIH->biBitCount == 24 )
	{
		DWORD r = stat_MaskRed( pImageBuffer );
		DWORD g = stat_MaskGreen( pImageBuffer );
		DWORD b = stat_MaskBlue( pImageBuffer );
		if(		r == 0x00ff0000
			&&	g == 0x0000ff00
			&&	b == 0x000000ff
			)
		{
			ULONG nScanLineSize = stat_RcScanLineSize( pBIH->biWidth, pBIH->biBitCount );
			INT x, y;
			for( y = 0; y < pBIH->biHeight; y++ )
			{
				LPBYTE pScanLine =
					stat_RcScanLine(
						pImageBuffer,
						ULONG( pBIH->biHeight - y - 1 ),
						nScanLineSize
						);
				COLORREF * ptrSrc = (COLORREF *)pScanLine;
				for( x = 0; x < pBIH->biWidth; x++ )
				{
					if( pBIH->biBitCount == 32 )
					{
						BYTE nSrcR = GetRValue( (*ptrSrc) );
						BYTE nSrcG = GetGValue( (*ptrSrc) );
						BYTE nSrcB = GetBValue( (*ptrSrc) );
#define GetAValue__(rgb) ((BYTE)((rgb)>>24))
						BYTE nSrcA = GetAValue__( (*ptrSrc) );
						(*ptrSrc) = ( RGB( nSrcB, nSrcG, nSrcR ) | ( COLORREF(nSrcA) << 24 ) );
						ptrSrc++;
					} // pBIH->biBitCount == 32
					else
					{
						ASSERT( pBIH->biBitCount == 24 );
						LPBYTE ptr = (LPBYTE)ptrSrc;
						BYTE nTmp = ptr[0];
						ptr[0] = ptr[2];
						ptr[2] = nTmp;
						ptr += 3;
						ptrSrc = (COLORREF *)ptr;
					} // else from if( pBIH->biBitCount == 32 )
				} // for( x = 0; x < pBIH->biWidth; x++ )
			} // for( y = 0; y < pBIH->biHeight; y++ )
			return true;
		}
	} // if( pBIH->biBitCount == 32 || pBIH->biBitCount == 24 )
	return false;
}

LPBYTE CExtSkinBitmap::stat_PngLoad(
	CArchive & ar,
	bool bEnableThrowException
	)
{
	ASSERT( ar.IsLoading() );
png_structp pCallbackData = NULL;
png_infop pInfoPNG = NULL;
png_uint_32 nWidth, nHeight;
png_colorp pPalettePNG;
INT nColorType, nPaletteSize;
INT nBitDepth, nPixelDepth;
LPBYTE pImageBuffer = NULL;
RGBQUAD * pPalette = NULL;
png_bytepp pRows = NULL;
	try
	{		
		BYTE png_check[8];
		ar.Read( png_check, 8 );
		if(	::png_sig_cmp(
				png_check,
				(png_size_t)0,
				8
				) != 0
			)
			::AfxThrowUserException();
		pCallbackData =
			::png_create_read_struct(
				PNG_LIBPNG_VER_STRING,
				(png_voidp)NULL,
				(png_error_ptr)stat_PngCallbackError,
				(png_error_ptr)stat_PngCallbackWarning
				);
		if( ! pCallbackData )
			::AfxThrowUserException();
		pInfoPNG = ::png_create_info_struct( pCallbackData );
		if( ! pInfoPNG )
		{
			::png_destroy_read_struct(
				&pCallbackData,
				(png_infopp)NULL,
				(png_infopp)NULL
				);
			::AfxThrowUserException();
		}
		::png_set_read_fn(
			pCallbackData,
			&ar,
			(png_rw_ptr)stat_PngCallbackRead
			);
// disable warning 4611
#pragma warning( push )
#pragma warning ( disable : 4611 )
        if(	setjmp( png_jmpbuf( pCallbackData ) ) )
		{
			::png_destroy_read_struct(
				&pCallbackData,
				&pInfoPNG,
				NULL
				);
			::AfxThrowUserException();
		}
// rollback warning 4611
#pragma warning( pop )
		::png_set_sig_bytes( pCallbackData, 8 );
		::png_read_info( pCallbackData, pInfoPNG );
		::png_get_IHDR(
			pCallbackData,
			pInfoPNG,
			&nWidth,
			&nHeight,
			&nBitDepth,
			&nColorType,
			NULL,
			NULL,
			NULL
			);
		nPixelDepth = pInfoPNG->pixel_depth;
		bool bAllocRGB = true;
		if( nBitDepth == 16 )
		{
			if(			nPixelDepth == 16
					&&	nColorType == PNG_COLOR_TYPE_GRAY
				)
				bAllocRGB = false;
			else if(	nPixelDepth == 48
					&&	nColorType == PNG_COLOR_TYPE_RGB
				)
				bAllocRGB = false;
			else
			{
				::png_set_strip_16( pCallbackData );
				nBitDepth = 8;
			}
		} // if( nBitDepth == 16 )
		switch( nColorType )
		{
		case PNG_COLOR_TYPE_RGB:
		case PNG_COLOR_TYPE_RGB_ALPHA:
		break;
		case PNG_COLOR_TYPE_PALETTE:
			if( nPixelDepth == 2 )
			{
				::png_set_packing( pCallbackData );
				nPixelDepth = 8;
			}					
		break;
		case PNG_COLOR_TYPE_GRAY:
			if( nPixelDepth == 2 )
			{
				::png_set_expand( pCallbackData );
				nPixelDepth = 8;
			}
		break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			::png_set_gray_to_rgb( pCallbackData );
			nPixelDepth = 32;
		break;
		default:
			::AfxThrowUserException();
		} // switch( nColorType )
		::png_read_update_info( pCallbackData, pInfoPNG );
		nColorType = ::png_get_color_type( pCallbackData, pInfoPNG );
		switch( nColorType )
		{
		case PNG_COLOR_TYPE_RGB:
			::png_set_invert_alpha( pCallbackData );
			if( bAllocRGB )
				pImageBuffer =
					stat_AllocHL(
						nWidth,
						nHeight,
						24,
						0x00FF0000,
						0x0000FF00,
						0x000000FF,
						0,
						0
						);
			else
				pImageBuffer =
					stat_AllocHL(
						nWidth,
						nHeight,
						nPixelDepth,
						0,
						0,
						0,
						0,
						0
						);
		break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			pImageBuffer =
				stat_AllocHL(
					nWidth,
					nHeight,
					32,
					0x00FF0000,
					0x0000FF00,
					0x000000FF,
					0xFF000000,
					0
					);
		break;
		case PNG_COLOR_TYPE_PALETTE:
		{
			pImageBuffer =
				stat_AllocHL(
					nWidth,
					nHeight,
					nPixelDepth,
					0,
					0,
					0,
					0,
					0
					);
			::png_get_PLTE(
				pCallbackData,
				pInfoPNG,
				&pPalettePNG,
				&nPaletteSize
				);
			pPalette = ( RGBQUAD * )
				( pImageBuffer + sizeof( BITMAPINFOHEADER ) );
			INT nPaletteIndex;
			for( nPaletteIndex = 0; nPaletteIndex < nPaletteSize; nPaletteIndex++ )
			{
				pPalette[nPaletteIndex].rgbRed   = pPalettePNG[nPaletteIndex].red;
				pPalette[nPaletteIndex].rgbGreen = pPalettePNG[nPaletteIndex].green;
				pPalette[nPaletteIndex].rgbBlue  = pPalettePNG[nPaletteIndex].blue;
			} // for( nPaletteIndex = 0; nPaletteIndex < nPaletteSize; nPaletteIndex++ )
		}
		break;
		case PNG_COLOR_TYPE_GRAY:
			pImageBuffer =
				stat_AllocHL(
					nWidth,
					nHeight,
					nPixelDepth,
					0,
					0,
					0,
					0,
					0
					);
			if( nPixelDepth <= 8 )
			{
				pPalette = ( RGBQUAD * )
					( pImageBuffer + sizeof( BITMAPINFOHEADER ) );
				nPaletteSize = 1 << nPixelDepth;
				INT nPaletteIndex;
				for( nPaletteIndex = 0; nPaletteIndex < nPaletteSize; nPaletteIndex++ )
				{
					pPalette[nPaletteIndex].rgbRed
						= pPalette[nPaletteIndex].rgbGreen
						= pPalette[nPaletteIndex].rgbBlue
						= BYTE( ( nPaletteIndex * 255 ) / ( nPaletteSize - 1 ) );
				} // for( nPaletteIndex = 0; nPaletteIndex < nPaletteSize; nPaletteIndex++ )
			} // if( nPixelDepth <= 8 )
		break;
		default:
			::AfxThrowUserException();
		} // switch( nColorType )
		LPBITMAPINFOHEADER pBIH = stat_RcInfoHeader( pImageBuffer );
		if( ::png_get_valid( pCallbackData, pInfoPNG, PNG_INFO_pHYs ) )
		{
			png_uint_32 nWidthPNG, nHeightPNG;
			int res_unit_type = 0;
			::png_get_pHYs(
				pCallbackData,
				pInfoPNG,
				&nWidthPNG,
				&nHeightPNG,
				&res_unit_type
				);
			if( res_unit_type == 1 )
			{
				pBIH->biXPelsPerMeter = nWidthPNG;
				pBIH->biYPelsPerMeter = nHeightPNG;
			}
		} // if( ::png_get_valid( pCallbackData, pInfoPNG, PNG_INFO_pHYs ) )
		pRows = ( png_bytepp ) ::malloc( nHeight * sizeof(png_bytep) );
		if( ! pRows )
		{
			if( pPalette != NULL )
				::png_free( pCallbackData, pPalette );				
			::png_destroy_read_struct(
				&pCallbackData,
				&pInfoPNG,
				NULL
				);
			stat_FreeHL( pImageBuffer );
			pImageBuffer = NULL;
			::AfxThrowUserException();
		} // if( ! pRows )
		png_uint_32 nRow;
		for( nRow = 0; nRow < nHeight; nRow++)				
			pRows[nHeight - 1 - nRow] =
				stat_RcScanLine(
					pImageBuffer,
					stat_RcScanLineSize(
						nWidth,
						pBIH->biBitCount
						),
					nRow
					);

		::png_read_image( pCallbackData, pRows );
		if( pRows )
		{
			::free( pRows );
			pRows = NULL;
		}
		::png_read_end( pCallbackData, pInfoPNG );
		if( pCallbackData )
			::png_destroy_read_struct(
				&pCallbackData,
				&pInfoPNG,
				(png_infopp)NULL
				);
		stat_PngCheckSwap32( pImageBuffer );
		return pImageBuffer;
	} // try
	catch( CException * pException )
	{
		if( pCallbackData )
			::png_destroy_read_struct(
				&pCallbackData,
				&pInfoPNG,
				(png_infopp)NULL
				);
		if( pRows )
			::free( pRows );
		if( pImageBuffer )
			stat_FreeHL( pImageBuffer );
		if( bEnableThrowException )
			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
		if( pCallbackData )
			::png_destroy_read_struct(
				&pCallbackData,
				&pInfoPNG,
				(png_infopp)NULL
				);
		if( pRows )
			::free( pRows );
		if( pImageBuffer )
			stat_FreeHL( pImageBuffer );
		if( bEnableThrowException )
			::AfxThrowUserException();
	} // catch( ... )
	return NULL;
}

bool CExtSkinBitmap::stat_PngSave(
	LPBYTE pImageBuffer,
	CArchive & ar,
	bool bEnableThrowException
	)
{
	ASSERT( pImageBuffer != NULL );
	ASSERT( ar.IsStoring() );
png_structp pCallbackData = NULL;
png_infop pInfoPNG = NULL;
bool bCheckSwap32 = false;
	try
	{
		LPBITMAPINFOHEADER pBIH = stat_RcInfoHeader( pImageBuffer );
		if(		pBIH->biBitCount != 32
			||	stat_MaskAlpha( pImageBuffer ) == 0
			)
			::AfxThrowUserException(); // not supported
		bCheckSwap32 = stat_PngCheckSwap32( pImageBuffer );
		INT nBitDepth = (pBIH->biBitCount > 8) ? 8 : pBIH->biBitCount;
		png_uint_32 nWidth = pBIH->biWidth;
		png_uint_32 nHeight = pBIH->biHeight;
		pCallbackData =
			::png_create_write_struct(
				PNG_LIBPNG_VER_STRING,
				(png_voidp)NULL,
				(png_error_ptr)stat_PngCallbackError,
				(png_error_ptr)stat_PngCallbackWarning
				);
		if( ! pCallbackData )
			::AfxThrowUserException();
		pInfoPNG = ::png_create_info_struct( pCallbackData );
		if( ! pInfoPNG )
		{
			::png_destroy_write_struct(&pCallbackData,  (png_infopp)NULL);
			::AfxThrowUserException();
		}
// disable warning 4611
#pragma warning( push )
#pragma warning ( disable : 4611 )
		if( setjmp( png_jmpbuf( pCallbackData ) ) )
		{
			::png_destroy_write_struct( &pCallbackData, &pInfoPNG );
			::AfxThrowUserException();
		}
// rollback warning 4611
#pragma warning( pop )
		::png_set_write_fn(
			pCallbackData,
			&ar,
			(png_rw_ptr)stat_PngCallbackWrite,
			(png_flush_ptr)stat_PngCallbackFlush
			);
		if(		pBIH->biXPelsPerMeter > 0
			&&	pBIH->biXPelsPerMeter > 0
			)
			::png_set_pHYs(
				pCallbackData,
				pInfoPNG,
				png_uint_32(pBIH->biXPelsPerMeter),
				png_uint_32(pBIH->biYPelsPerMeter),
				1
				);
		::png_set_IHDR(
			pCallbackData,
			pInfoPNG,
			nWidth,
			nHeight,
			nBitDepth, 
			PNG_COLOR_TYPE_RGBA,
			PNG_INTERLACE_NONE, 
			PNG_COMPRESSION_TYPE_BASE,
			PNG_FILTER_TYPE_BASE
			);
		::png_write_info( pCallbackData, pInfoPNG );
		png_uint_32 nRow;
		for( nRow = 0; nRow < nHeight; nRow++ )
		{
			::png_write_row(
				pCallbackData,
				stat_RcScanLine(
					pImageBuffer,
					stat_RcScanLineSize(
						nWidth,
						pBIH->biBitCount
						),
					nHeight - nRow - 1
					)
				);
		}
		::png_write_end( pCallbackData, pInfoPNG );
		::png_destroy_write_struct( &pCallbackData, &pInfoPNG );
		if( bCheckSwap32 )
			stat_PngCheckSwap32( pImageBuffer );
		return true;
	} // try
	catch( CException * pException )
	{
		if( bCheckSwap32 )
			stat_PngCheckSwap32( pImageBuffer );
		if( pCallbackData )
			::png_destroy_write_struct( &pCallbackData, &pInfoPNG );
		if( bEnableThrowException )
			throw;
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
		if( bCheckSwap32 )
			stat_PngCheckSwap32( pImageBuffer );
		if( pCallbackData )
			::png_destroy_write_struct( &pCallbackData, &pInfoPNG );
		if( bEnableThrowException )
			::AfxThrowUserException();
	} // catch( ... )
	return false;
}


