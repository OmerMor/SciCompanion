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

#if (!defined __PROF_SKIN_ITEM_H)
#define __PROF_SKIN_ITEM_H

#if (!defined __PROF_SKIN_H)
	#include <../ProfSkin/ProfSkin.h>
#endif

#if (!defined __PROF_SKIN_BITMAP_H)
	#include <../ProfSkin/ExtSkinBitmap.h>
#endif

// Versions
#define __EXT_SKIN_SERIAL_FORMAT_NUMBER 1

// enable XML support in ProfSkin
#define __PROF_SKIN_XML_SUPPORT

#if (defined __PROF_SKIN_XML_SUPPORT)

	#import "msxml.dll"

	// Elements and attributes names for XML documents
	#define __EXT_SKIN_ITEM									L"SkinItem"
	#define __EXT_SKIN_ITEM_GLYPH							L"Glyph"
	#define __EXT_SKIN_ITEM_FONT							L"Font"
	#define __EXT_SKIN_ITEM_COLOR							L"Color"
	#define __EXT_SKIN_ITEM_NAME							L"Name"
	#define __EXT_SKIN_ITEM_COMMENT							L"Comment"
	#define __EXT_SKIN_ITEM_VERSION							L"Version"
	#define __EXT_SKIN_ITEM_ENABLED							L"Enable"
	#define __EXT_SKIN_ITEM_GLYPH_TOP_PADDING				L"Tp"
	#define __EXT_SKIN_ITEM_GLYPH_LEFT_PADDING				L"Lp"
	#define __EXT_SKIN_ITEM_GLYPH_BOTTOM_PADDING			L"Bp"
	#define __EXT_SKIN_ITEM_GLYPH_RIGHT_PADDING				L"Rp"
	#define __EXT_SKIN_ITEM_GLYPH_CONTENT_TOP_PADDING		L"Ctp"
	#define __EXT_SKIN_ITEM_GLYPH_CONTENT_LEFT_PADDING		L"Clp"
	#define __EXT_SKIN_ITEM_GLYPH_CONTENT_BOTTOM_PADDING	L"Cbp"
	#define __EXT_SKIN_ITEM_GLYPH_CONTENT_RIGHT_PADDING		L"Crp"
	#define __EXT_SKIN_ITEM_GLYPH_LEFT_OFFSET				L"Lo"
	#define __EXT_SKIN_ITEM_GLYPH_TOP_OFFSET				L"To"
	#define __EXT_SKIN_ITEM_GLYPH_RIGHT_OFFSET				L"Ro"
	#define __EXT_SKIN_ITEM_GLYPH_BOTTOM_OFFSET				L"Bo"
	#define __EXT_SKIN_ITEM_GLYPH_ICON_INDENT				L"IconIndent"
	#define __EXT_SKIN_ITEM_GLYPH_WIDTH						L"Width"
	#define __EXT_SKIN_ITEM_GLYPH_HEIGHT					L"Height"
	#define __EXT_SKIN_ITEM_GLYPH_SRC_BITMAP				L"Image"
	#define __EXT_SKIN_ITEM_GLYPH_DRAW_METHOD				L"DrawMethod"
	#define __EXT_SKIN_ITEM_GLYPH_DRAW_CENTER				L"DrawCenter"
	#define __EXT_SKIN_ITEM_GLYPH_ENABLED					L"Enable"
	#define __EXT_SKIN_ITEM_FONT_HEIGHT						L"Height"
	#define __EXT_SKIN_ITEM_FONT_WIDTH						L"Width"
	#define __EXT_SKIN_ITEM_FONT_ESCAPEMENT					L"Escapement"
	#define __EXT_SKIN_ITEM_FONT_ORIENTATION				L"Orientation"
	#define __EXT_SKIN_ITEM_FONT_WEIGHT						L"Weight"
	#define __EXT_SKIN_ITEM_FONT_ITALIC						L"Italic"
	#define __EXT_SKIN_ITEM_FONT_UNDERLINE					L"Underline"
	#define __EXT_SKIN_ITEM_FONT_STRIKE_OUT					L"StrikeOut"
	#define __EXT_SKIN_ITEM_FONT_CHAR_SET					L"CharSet"
	#define __EXT_SKIN_ITEM_FONT_OUT_PRECISION				L"OutPrecision"
	#define __EXT_SKIN_ITEM_FONT_CLIP_PRECISION				L"ClipPrecision"
	#define __EXT_SKIN_ITEM_FONT_QUALITY					L"Quality"
	#define __EXT_SKIN_ITEM_FONT_PITCH_AND_FAMILY			L"PitchAndFamily"
	#define __EXT_SKIN_ITEM_FONT_FACE_NAME					L"FaceName"
	//#define __EXT_SKIN_ITEM_FONT_SHADOW_OFFSET_X			L"Sox"
	//#define __EXT_SKIN_ITEM_FONT_SHADOW_OFFSET_Y			L"Soy"
	#define __EXT_SKIN_ITEM_COLOR_COLOR						L"Color"

	#define __EXT_SKIN_ITEM_SCROLL_BAR_ROOT					  L"ScrollBar"
	#define __EXT_SKIN_ITEM_SCROLL_BAR_ROOT_T				_T("ScrollBar")

	#define __EXT_SKIN_ITEM_SCROLL_BAR_TYPE_ZOOM			  L"Zoom"
	#define __EXT_SKIN_ITEM_SCROLL_BAR_TYPE_ZOOM_T			_T("Zoom")
	#define __EXT_SKIN_ITEM_SCROLL_BAR_TYPE_CLASSIC			  L"Classic"
	#define __EXT_SKIN_ITEM_SCROLL_BAR_TYPE_CLASSIC_T		_T("Classic")

	#define __EXT_SKIN_ITEM_SCROLL_BAR_ORIENTATION_H		  L"Horizontal"
	#define __EXT_SKIN_ITEM_SCROLL_BAR_ORIENTATION_H_T		_T("Horizontal")
	#define __EXT_SKIN_ITEM_SCROLL_BAR_ORIENTATION_V		  L"Vertical"
	#define __EXT_SKIN_ITEM_SCROLL_BAR_ORIENTATION_V_T		_T("Vertical")

#endif // (defined __PROF_SKIN_XML_SUPPORT)

#define __EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE 4
#define __EXT_ZLIB_STREAM_ARCHIVE_BUFFER_ALLOC_SIZE 4096
#define __EXT_ZLIB_STREAM_ARCHIVE_BUFFER_GROW_SIZE 4096

class CExtSkinItemBase;
class CExtSkinItem;
class CExtSkin;
class CExtSkinGlyph;
class CExtSkinFont;
class CExtSkinColor;

class __PROF_SKIN_API CExtSkinItemBase : public CObject
{
public:
	DECLARE_DYNAMIC( CExtSkinItemBase );
	class __PROF_SKIN_API NumberFromString
	{
	public:
		static bool stat_GetValue( BSTR bstr, long & lValue );
		static bool stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, long & lValue );
		static bool stat_GetValue( BSTR bstr, unsigned long & ulValue );
		static bool stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, unsigned long & ulValue );
		static bool stat_GetValue( BSTR bstr, short & sValue );
		static bool stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, short & sValue );
		static bool stat_GetValue( BSTR bstr, unsigned short & usValue );
		static bool stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, unsigned short & usValue );
		static bool stat_GetValue( BSTR bstr, char & cValue );
		static bool stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, char & cValue );
		static bool stat_GetValue( BSTR bstr, unsigned char & ucValue );
		static bool stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, unsigned char & ucValue );

	protected:
		static bool stat_GetValue( __EXT_MFC_SAFE_LPCTSTR str, long lMinValue, long lMaxValue, long & lValue );
	};

	CExtSkinItemBase();
	CExtSkinItemBase( const CExtSkinItemBase & other );
	virtual ~CExtSkinItemBase();

	static const TCHAR g_strMessKey[];
	static const BYTE g_arrSignature[ __EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE ];

	virtual bool SerializeCompressedBlock(
		CArchive & ar,
		bool bEnableThrowExceptions = false,
		bool bUseCompression = true,
		__EXT_MFC_SAFE_LPCTSTR strMessKey = NULL
		);
	virtual void Serialize( CArchive & ar );
	bool SerializeFile(
		CFile & _file,
		bool bSave,
		bool bEnableThrowExceptions,
		bool bUseCompression = true,
		__EXT_MFC_SAFE_LPCTSTR strMessKey = NULL
		);
	bool SearchAndLoadSkinFile(
		__EXT_MFC_SAFE_LPCTSTR strFilePath,
		bool bEnableThrowExceptions,
		bool bUseCompression = true,
		__EXT_MFC_SAFE_LPCTSTR strMessKey = NULL
		);
	bool SerializeFile(
		__EXT_MFC_SAFE_LPCTSTR strFilePath,
		bool bSave,
		bool bEnableThrowExceptions,
		bool bUseCompression = true,
		__EXT_MFC_SAFE_LPCTSTR strMessKey = NULL
		);
	
#if (defined __PROF_SKIN_XML_SUPPORT)
	virtual bool Load( IXMLElement * pElement );
#endif // (defined __PROF_SKIN_XML_SUPPORT)

	CExtSkinItemBase & operator = ( const CExtSkinItemBase & other );

	CExtSkinItem * GetParent();
	const CExtSkinItem * GetParent() const;
	virtual CExtSkinItem * GetTopLevel();
	const CExtSkinItem * GetTopLevel() const;
	CExtSkin * GetSkin();
	const CExtSkin * GetSkin() const;
	__EXT_MFC_SAFE_LPCTSTR NameGet() const;
	virtual bool NameSet( __EXT_MFC_SAFE_LPCTSTR strName );
	__EXT_MFC_SAFE_LPCTSTR CommentGet() const;
	void CommentSet( __EXT_MFC_SAFE_LPCTSTR strName );

protected:
	virtual void _Clear();
	
protected:
	CExtSkinItem * m_pParent;
	CExtSafeString m_strName;
	CExtSafeString m_strComment;
}; // class CExtSkinItemBase

class __PROF_SKIN_API CExtSkinGlyph : public CExtSkinItemBase
{
public:
	DECLARE_DYNAMIC( CExtSkinGlyph );
	CExtSkinGlyph();
	CExtSkinGlyph( const CExtSkinGlyph & other );
	virtual ~CExtSkinGlyph();
	virtual void Serialize( CArchive & ar );

	CExtSkinGlyph & operator = ( const CExtSkinGlyph & other );

#if (defined __PROF_SKIN_XML_SUPPORT)
	virtual bool Load( IXMLElement * pElement );
#endif // (defined __PROF_SKIN_XML_SUPPORT)

	virtual bool Draw(
		HDC hDC,
		const RECT & rcDstDraw,
		const RECT & rcSrcDraw,
		bool bForceNoAlpha = false,
		BYTE nSCA = BYTE(0x0FF),
		COLORREF clrColorize = COLORREF(-1L)
		) const;
	bool Draw(
		HDC hDC,
		const RECT & rcDstDraw,
		bool bForceNoAlpha = false,
		BYTE nSCA = BYTE(0x0FF),
		COLORREF clrColorize = COLORREF(-1L)
		) const;
	virtual bool NameSet( __EXT_MFC_SAFE_LPCTSTR strName );

	bool IsEmpty() const;
    CSize GetSize() const;
	long GetIconIndent() const;
	const CRect & GetPadding() const;
	const CRect & GetContentPadding() const;
	const CRect & GetOffset() const;
	const CExtBitmap & GetBitmap() const;
	CExtBitmap & GetBitmap();
		
protected:
	virtual void _Clear();

protected:
	bool m_bEnabled:1;
	bool m_bDrawCenter:1;
	LONG m_nIconIndent;
	CRect m_rcPadding;
	CRect m_rcContentPadding;
	CRect m_rcOffset;
	CSize m_Size;
	CExtSkinBitmap m_Bitmap;
	CExtBitmap::e_DrawMethod_t m_eDM;

	friend class CExtSkinItem;
}; // class CExtSkinGlyph

class __PROF_SKIN_API CExtSkinColor : public CExtSkinItemBase
{
public:
	DECLARE_DYNAMIC( CExtSkinColor );
	CExtSkinColor();
	CExtSkinColor( COLORREF clr );
	virtual ~CExtSkinColor();
	virtual void Serialize( CArchive & ar );
	
#if (defined __PROF_SKIN_XML_SUPPORT)
	virtual bool Load( IXMLElement * pElement );
#endif // (defined __PROF_SKIN_XML_SUPPORT)

	virtual bool NameSet( __EXT_MFC_SAFE_LPCTSTR strName );

	operator COLORREF () const;
	CExtSkinColor & operator = ( const CExtSkinColor & other );
	CExtSkinColor & operator = ( COLORREF clr );
	
	bool IsHollow() const;

protected:
	virtual void _Clear();

protected:
	long m_nColor;

	friend class CExtSkinItem;
}; // class CExtSkinColor

class __PROF_SKIN_API CExtSkinFont
	: public CExtSkinItemBase
	, protected LOGFONT
{
public:
	DECLARE_DYNAMIC( CExtSkinFont );
	CExtSkinFont();
	CExtSkinFont( const LOGFONT & lf );
	CExtSkinFont( const CExtSkinFont & other );
	virtual ~CExtSkinFont();
	virtual void Serialize( CArchive & ar );

#if (defined __PROF_SKIN_XML_SUPPORT)
	virtual bool Load( IXMLElement * pElement );
#endif // (defined __PROF_SKIN_XML_SUPPORT)

	virtual bool NameSet( __EXT_MFC_SAFE_LPCTSTR strName );

	operator LOGFONT & ();
	operator const LOGFONT & ();
	CExtSkinFont & operator = ( const LOGFONT & lf );
	CExtSkinFont & operator = ( const CExtSkinFont & other );
	
	HFONT CreateFont(
		LONG lfEscapement = 0
		) const;
	bool CreateFont(
		CFont & _font,
		LONG lfEscapement = 0
		) const;
	LOGFONT & LogFontGet();
	const LOGFONT & LogFontGet() const;
//	CPoint & ShadowOffsetGet();
//	const CPoint & ShadowOffsetGet() const;

	LPCTSTR GetHashCode() const;

protected:
	virtual void _Clear();
	bool _IsComplete() const;

protected:
	bool m_bHaveHeight:1;
	bool m_bHaveWidth:1;
	bool m_bHaveWeight:1;
	bool m_bHaveItalic:1;
	bool m_bHaveUnderline:1;
	bool m_bHaveStrikeOut:1;
	bool m_bHaveFaceName:1;
//	bool m_bHaveShadowOffset:1;
	bool m_bHavePitchAndFamily:1;
	bool m_bHaveCharSet:1;
	bool m_bComplete:1;
//	CPoint m_ptShadowOffset;
	CExtSafeString m_strHashCode;

	friend class CExtSkinItem;
}; // class CExtSkinFont

class __PROF_SKIN_API CExtSkinItem : public CExtSkinItemBase
{
public:
	DECLARE_DYNAMIC( CExtSkinItem );
	CExtSkinItem();
	virtual ~CExtSkinItem();
	virtual void Serialize( CArchive & ar );

	CExtSkinItem * GetTopLevel();

#if (defined __PROF_SKIN_XML_SUPPORT)
	virtual bool Load( __EXT_MFC_SAFE_LPCTSTR sXMLPath );
	virtual bool Load( IXMLElement * pElement );
#endif // (defined __PROF_SKIN_XML_SUPPORT)

	virtual bool NameSet( __EXT_MFC_SAFE_LPCTSTR strName );

	bool EnabledGet() const;
	void EnabledSet( bool bEnable );

	int NestedGetCount();
	CExtSkinItem * NestedGet( __EXT_MFC_SAFE_LPCTSTR strName );
	const CExtSkinItem * NestedGet( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	const CExtSkinItem & Nested( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	CExtSkinItem & Nested( __EXT_MFC_SAFE_LPCTSTR strName );
	const CExtSkinItem & operator [] ( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	CExtSkinItem & operator [] ( __EXT_MFC_SAFE_LPCTSTR strName );
	CExtSkinItem * NestedGetNext( POSITION & pos );
	bool NestedSet( CExtSkinItem * pItem );
	bool NestedRemove( __EXT_MFC_SAFE_LPCTSTR strName, CExtSkinItem ** ppItem = NULL );
	void NestedRemoveAll();

	int GlyphGetCount();
	CExtSkinGlyph * GlyphGet( __EXT_MFC_SAFE_LPCTSTR strName );
	const CExtSkinGlyph * GlyphGet( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	CExtSkinGlyph * GlyphGetDeep( __EXT_MFC_SAFE_LPCTSTR strName );
	const CExtSkinGlyph * GlyphGetDeep( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	const CExtSkinGlyph & Glyph( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	CExtSkinGlyph & Glyph( __EXT_MFC_SAFE_LPCTSTR strName );
	CExtSkinGlyph * GlyphGetNext( POSITION & pos );
	bool GlyphSet( CExtSkinGlyph * pGlyph );
	bool GlyphRemove( __EXT_MFC_SAFE_LPCTSTR strName, CExtSkinGlyph ** ppGlyph = NULL );
	void GlyphRemoveAll();

	int FontGetCount();
	CExtSkinFont * FontGet( __EXT_MFC_SAFE_LPCTSTR strName );
	const CExtSkinFont * FontGet( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	CExtSkinFont * FontGetDeep( __EXT_MFC_SAFE_LPCTSTR strName );
	const CExtSkinFont * FontGetDeep( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	const CExtSkinFont & Font( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	CExtSkinFont & Font( __EXT_MFC_SAFE_LPCTSTR strName );
	CExtSkinFont * FontGetNext( POSITION & pos );
	bool FontSet( CExtSkinFont * pFont );
	bool FontRemove( __EXT_MFC_SAFE_LPCTSTR strName, CExtSkinFont ** ppFont = NULL );
	void FontRemoveAll();

	int ColorGetCount();
	CExtSkinColor * ColorGet( __EXT_MFC_SAFE_LPCTSTR strName );
	const CExtSkinColor * ColorGet( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	CExtSkinColor * ColorGetDeep( __EXT_MFC_SAFE_LPCTSTR strName );
	const CExtSkinColor * ColorGetDeep( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	const CExtSkinColor & Color( __EXT_MFC_SAFE_LPCTSTR strName ) const;
	CExtSkinColor & Color( __EXT_MFC_SAFE_LPCTSTR strName );
	CExtSkinColor * ColorGetNext( POSITION & pos );
	bool ColorSet( CExtSkinColor * pColor );
	bool ColorRemove( __EXT_MFC_SAFE_LPCTSTR strName, CExtSkinColor ** ppColor = NULL );
	void ColorRemoveAll();

protected:
	virtual void _Clear();

#if (defined __PROF_SKIN_XML_SUPPORT)
	bool _LoadGlyphsCollection( IXMLElement * pElement );
	bool _LoadNestedCollection( IXMLElement * pElement );
	bool _LoadFonts( IXMLElement * pElement );
	bool _LoadColors( IXMLElement * pElement );
#endif // (defined __PROF_SKIN_XML_SUPPORT)

protected:
	bool m_bEnabled:1;
	CExtSafeMapStringToPtr
		m_mapNestedItems,
		m_mapGlyphs,
		m_mapFonts,
		m_mapColors;
}; // class CExtSkinItem

class __PROF_SKIN_API CExtSkin : public CExtSkinItem
{
public:
	DECLARE_DYNAMIC( CExtSkin );
	CExtSkin();
	virtual ~CExtSkin();
	virtual void Serialize( CArchive & ar );
#if (defined __PROF_SKIN_XML_SUPPORT)
	virtual bool Load( __EXT_MFC_SAFE_LPCTSTR sXMLPath );
	virtual bool Load( IXMLElement * pElement );
#endif // (defined __PROF_SKIN_XML_SUPPORT)
	virtual void SynchronizeScrollBarElements(
		__EXT_MFC_SAFE_LPCTSTR strScrollBarTypeBranch,
		__EXT_MFC_SAFE_LPCTSTR strScrollBarOrientationBranch,
		CExtPaintManager::SCROLLBARSKINDATA * pSBSD
		);
	virtual void SynchronizeScrollBars();
	CExtPaintManager::SCROLLBARSKINDATA m_SBSD_ClassicH, m_SBSD_ClassicV, m_SBSD_ZoomH, m_SBSD_ZoomV;
protected:
	virtual void _Clear();
}; // class CExtSkin

class __PROF_SKIN_API CExtZLibStreamCacheArchive : public CArchive
{
protected:
	CExtSafeString m_strMessKey;
	bool m_bUseCompression:1;
	CFile * m_pDataFile;
	LPBYTE m_pDataBuffer;
	static const BYTE g_arrSignatureDefault[ __EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE ];
	mutable BYTE m_arrSignature[ __EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE ];
#pragma pack(push, 1)
	struct __PROF_SKIN_API StreamHeader_t
	{
		BYTE m_arrSignature[ __EXT_ZLIB_STREAM_ARCHIVE_SIGNATURE_SIZE ], m_nMessMode;
		DWORD m_dwDataSize;
		ULONG m_nCRC;
	};
#pragma pack(pop)
	virtual bool OnMess(
		LPBYTE pBuffer,
		ULONG nLength,
		__EXT_MFC_SAFE_LPCTSTR strMessKey = _T("")
		) const;
public:
	CExtZLibStreamCacheArchive(
		CFile * pArchiveFile,
		UINT nArchiveMode,
		bool bUseCompression = true,
		__EXT_MFC_SAFE_LPCTSTR strMessKey = _T(""),
		const LPBYTE arrSignature = NULL,
		INT nArchiveBufferSize = 4096,
		LPVOID pArchiveBuffer = NULL
		);
	virtual ~CExtZLibStreamCacheArchive();
	virtual void Close();
	virtual void Abort();
}; // class CExtZLibStreamCacheArchive

#endif // __PROF_SKIN_ITEM_H

