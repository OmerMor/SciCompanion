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

#if (!defined __PROF_SKIN_PAINT_MANAGER_H)
	#include <../ProfSkin/ExtSkinPaintManager.h>
#endif

#if (!defined __EXT_RICH_CONTENT_H)
	#include <ExtRichContent.h>
#endif // (!defined __EXT_RICH_CONTENT_H)

#if (! defined __EXTMINIDOCKFRAMEWND_H)
	#include <../Src/ExtMiniDockFrameWnd.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include <../Src/ExtDockBar.h>
#endif

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	#if (!defined __EXT_CONTROLBAR_TABBED_FEATURES_H)
		#include <../Src/ExtControlBarTabbedFeatures.h>
	#endif // __EXT_CONTROLBAR_TABBED_FEATURES_H
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

#if (! defined __VSSYM32_H__)
	#include <vssym32/vssym32.h>
#endif // (! defined __VSSYM32_H__)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( CExtPaintManagerSkin, CExtPaintManager, VERSIONABLE_SCHEMA|1 );

CExtPaintManagerSkin::CExtPaintManagerSkin(void)
{
	m_bUseTahomaFont = false;
#if (!defined __EXT_MFC_NO_PAGECONTAINER_CTRL)
	m_bPageContainerUseButtonStyle = true;
	m_bPageContainerUseGroupBoxStyle = true;
	m_bPageContainerNoEmbossDisabledText = true;
#endif // (!defined __EXT_MFC_NO_PAGECONTAINER_CTRL)
#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
	m_bShortcutListUseButtonStyleInScrollButtons = true;
	m_bShortcutListUseDockerBkgnd = true;
	m_bShortcutListItemUseButtonStyle = true;
#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
}

CExtPaintManagerSkin::~CExtPaintManagerSkin(void)
{
}

void CExtPaintManagerSkin::SerializeSynchronizationData( CArchive & ar )
{
	ASSERT_VALID( this );
	m_Skin.Serialize( ar );
}
	
bool CExtPaintManagerSkin::OnStateSerialize(
	CArchive & ar,
	bool bEnableThrowExceptions // = false
	)
{
	try
	{
		m_Skin.SerializeCompressedBlock( ar, bEnableThrowExceptions );
		return true;
	}
	catch( CException * pXept )
	{
		if( bEnableThrowExceptions )
			throw;
		pXept->Delete();
	}
	catch( ... )
	{
		if( bEnableThrowExceptions )
			throw;
	}
	return false;
}

bool CExtPaintManagerSkin::NcFrame_IsSupported(
	const CWnd * pWnd,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	lParam;
	if( pWnd == NULL )
		return false;
	//ASSERT_VALID( pWnd );
	if( pWnd->GetSafeHwnd() == NULL )
		return false;
//DWORD dwStyle = pWnd->GetStyle();
//	if( (dwStyle&WS_CHILD) != 0 )
//		return false;
	return true;
}

HRGN CExtPaintManagerSkin::NcFrame_GenerateSkinFrameRGN(
	const RECT & rcWnd,
	const CWnd * pWnd,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pWnd );
	rcWnd;
	lParam;
const CExtSkinGlyph & glyphFrame = m_Skin[_T("NcFrame")][_T("NcArea")].Glyph(_T("Inactive"));
//CRect rcNcBorders = glyphFrame.GetContentPadding();
CRect rcThemePadding = glyphFrame.GetPadding();
bool bRTL = ( (pWnd->GetExStyle()&WS_EX_LAYOUTRTL) != 0 ) ? true : false;
HRGN hRgn =
		glyphFrame.GetBitmap().GenerateSkinFrameAlphaRGN(
			bRTL,
			rcWnd,
			rcThemePadding
			);
	return hRgn;
}

INT CExtPaintManagerSkin::NcFrame_GetCaptionHeight(
	bool bActive,
	const CWnd * pWnd,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	bActive;
	pWnd;
	lParam;
CRect rcCaption =
		NcFrame_GetHtRect(
			HTCAPTION,
			false,
			true,
			true,
			NULL,
			pWnd,
			NULL
			);
	return rcCaption.Height();
}

void CExtPaintManagerSkin::NcFrame_GetMetrics(
	RECT & rcNcBorders,
	RECT & rcThemePadding,
	const CWnd * pWnd,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	ASSERT( NcFrame_IsSupported( pWnd ) );
	rcNcBorders;
	rcThemePadding;
	pWnd;
	lParam;
const CExtSkinGlyph & glyphFrame = m_Skin[_T("NcFrame")][_T("NcArea")].Glyph(_T("Inactive"));
	rcNcBorders = glyphFrame.GetContentPadding();
	rcThemePadding = glyphFrame.GetPadding();
}

CRect CExtPaintManagerSkin::NcFrame_GetHtRect(
	UINT nHT,
	bool bScreenMapping,
	bool bLayoutBordersH,
	bool bLayoutBordersV,
	const CExtCmdIcon * pIcon,
	const CWnd * pWnd,
	LPMINMAXINFO pMinMaxInfo, // = NULL
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	ASSERT( NcFrame_IsSupported( pWnd ) );
CRect rcNcBorders, rcThemePadding;
	NcFrame_GetMetrics(
		rcNcBorders,
		rcThemePadding,
		pWnd,
		lParam
		);
CRect rcWnd;
	pWnd->GetWindowRect( &rcWnd );
	if( ! bScreenMapping )
		rcWnd.OffsetRect( -rcWnd.TopLeft() );
bool bZoomed = pWnd->IsZoomed()? true : false;
bool bSupportsResizingH = bLayoutBordersH;
bool bSupportsResizingV = bLayoutBordersV;
DWORD dwWndStyle = pWnd->GetStyle();
	if(		(dwWndStyle&WS_THICKFRAME) != 0
		&&	(	(! bSupportsResizingH )
			||	(! bSupportsResizingV )
			)
		)
	{
		bool bHaveMinMaxInfo = false;
		MINMAXINFO _mmi;
		if( pMinMaxInfo != NULL )
		{
			bHaveMinMaxInfo = true;
			__EXT_MFC_MEMCPY( 
				&_mmi, 
				sizeof(MINMAXINFO),
				pMinMaxInfo, 
				sizeof(MINMAXINFO) 
				);
		} // if( pMinMaxInfo != NULL )
		else
		{
			::memset( &_mmi, 0, sizeof(MINMAXINFO) );
			monitor_parms_t _mp;
			stat_GetMonitorParms( _mp, (CWnd*)pWnd );
			_mmi.ptMaxPosition.x = _mp.m_rcWorkArea.left;
			_mmi.ptMaxPosition.y = _mp.m_rcWorkArea.top;
			_mmi.ptMaxTrackSize.x = _mp.m_rcWorkArea.Width(); // ::GetSystemMetrics( SM_CXMAXTRACK );
			_mmi.ptMaxTrackSize.y = _mp.m_rcWorkArea.Height(); // ::GetSystemMetrics( SM_CYMAXTRACK );
			_mmi.ptMinTrackSize.x = ::GetSystemMetrics( SM_CXMINTRACK );
			_mmi.ptMinTrackSize.y = ::GetSystemMetrics( SM_CYMINTRACK );
			_mmi.ptMaxSize.x = _mmi.ptMaxTrackSize.x;
			_mmi.ptMaxSize.y = _mmi.ptMaxTrackSize.y;
			if( ((CWnd*)pWnd)->SendMessage( WM_GETMINMAXINFO, 0, LPARAM(&_mmi) ) == 0 )
				bHaveMinMaxInfo = true;
		} // else from if( pMinMaxInfo != NULL )
		if( bHaveMinMaxInfo )
		{
			if( _mmi.ptMinTrackSize.x < _mmi.ptMaxTrackSize.x )
				bSupportsResizingH = true;
			if( _mmi.ptMinTrackSize.y < _mmi.ptMaxTrackSize.y )
				bSupportsResizingV = true;
		} // if( bHaveMinMaxInfo )
	} // if( (dwWndStyle&WS_THICKFRAME) != 0 ...
	switch( nHT )
	{
	case HTSYSMENU:
		{
		if( (dwWndStyle&WS_SYSMENU) == 0 )
			break;
		int nIconAreaWidth = 0;
		if( pIcon != NULL && (! pIcon->IsEmpty() ) )
			nIconAreaWidth = pIcon->GetSize().cx + 2;
		return
			CRect(
				rcWnd.left + ( bSupportsResizingH ? rcNcBorders.left : 0 ),
				rcWnd.top + ( bSupportsResizingV ? rcNcBorders.top : 0 ),
				rcWnd.left + ( bSupportsResizingH ? rcNcBorders.left : 0 )
					+ nIconAreaWidth,
				rcWnd.top + rcThemePadding.top
				);
		}
	case HTCAPTION:
//		if( (dwWndStyle&WS_CAPTION) == 0 )
//			break;
		return
			CRect(
				rcWnd.left + (bZoomed ? 0 : ( bSupportsResizingH ? rcNcBorders.left : 0 ) ),
				rcWnd.top + (bZoomed ? 0 : ( bSupportsResizingV ? rcNcBorders.top : 0 ) ),
				rcWnd.right - (bZoomed ? 0 : ( bSupportsResizingH ? rcNcBorders.right : 0 ) ),
				rcWnd.top + rcThemePadding.top
				);
	case HTTOP:
		if( bZoomed || (! bSupportsResizingV ) )
			break;
		return
			CRect(
				rcWnd.left + ( bSupportsResizingH ? rcNcBorders.left : 0 ),
				rcWnd.top,
				rcWnd.right - ( bSupportsResizingH ? rcNcBorders.right : 0 ),
				rcWnd.top + rcNcBorders.top
				);
	case HTBOTTOM:
		if( bZoomed || (! bSupportsResizingV ) )
			break;
		return
			CRect(
				rcWnd.left + ( bSupportsResizingH ? rcNcBorders.left : 0 ),
				rcWnd.bottom - rcNcBorders.bottom,
				rcWnd.right - ( bSupportsResizingH ? rcNcBorders.right : 0 ),
				rcWnd.bottom
				);
	case HTLEFT:
		if( bZoomed || (! bSupportsResizingH ) )
			break;
		return
			CRect(
				rcWnd.left,
				rcWnd.top + ( bSupportsResizingV ? rcNcBorders.top : 0 ),
				rcWnd.left + rcNcBorders.left,
				rcWnd.bottom - ( bSupportsResizingV ? rcNcBorders.bottom : 0 )
				);
	case HTRIGHT:
		if( bZoomed || (! bSupportsResizingH ) )
			break;
		return
			CRect(
				rcWnd.right - rcNcBorders.right,
				rcWnd.top + ( bSupportsResizingV ? rcNcBorders.top : 0 ),
				rcWnd.right,
				rcWnd.bottom - ( bSupportsResizingV ? rcNcBorders.bottom : 0 )
				);
	case HTTOPLEFT:
		if( bZoomed || (! bSupportsResizingH ) || (! bSupportsResizingV ) )
			break;
		return
			CRect(
				rcWnd.left,
				rcWnd.top,
				rcWnd.left + rcNcBorders.left,
				rcWnd.top + rcNcBorders.top
				);
	case HTTOPRIGHT:
		if( bZoomed || (! bSupportsResizingH ) || (! bSupportsResizingV ) )
			break;
		return
			CRect(
				rcWnd.right - rcNcBorders.right,
				rcWnd.top,
				rcWnd.right,
				rcWnd.top + rcNcBorders.top
				);
	case HTBOTTOMLEFT:
		if( bZoomed || (! bSupportsResizingH ) || (! bSupportsResizingV ) )
			break;
		return
			CRect(
				rcWnd.left,
				rcWnd.bottom - rcNcBorders.bottom,
				rcWnd.left + rcNcBorders.left,
				rcWnd.bottom
				);
	case HTBOTTOMRIGHT:
		if( bZoomed || (! bSupportsResizingH ) || (! bSupportsResizingV ) )
			break;
		return
			CRect(
				rcWnd.right - rcNcBorders.right,
				rcWnd.bottom - rcNcBorders.bottom,
				rcWnd.right,
				rcWnd.bottom
				);
	} // switch( nHT )
	return CRect( 0, 0, 0, 0 );
}

__EXT_MFC_SAFE_LPCTSTR CExtPaintManagerSkin::stat_NcFrameButton_NameFromState(
	CExtPaintManager::e_nc_button_state_t eState
	)
{
	if( eState == __ENCBS_DISABLED )
		return _T("Disabled");
	if( eState == __ENCBS_HOVER )
		return _T("Hover");
	if( eState == __ENCBS_PRESSED )
		return _T("Pressed");
	ASSERT( eState == __ENCBS_NORMAL );
	return _T("Normal");
}

void CExtPaintManagerSkin::NcFrame_Paint(
	CDC & dc,
	const CExtCmdIcon * pIcon,
	__EXT_MFC_SAFE_LPCTSTR strCaption,
	UINT nDrawTextAlignFlags,
	const RECT & rcFrame,
	const RECT & rcClient,
	const RECT & rcIcon,
	const RECT & rcText,
	const RECT & rcHelp,
	const RECT & rcMinimize,
	const RECT & rcMaximizeRestore,
	const RECT & rcClose,
	bool bFrameActive,
	bool bFrameEnabled,
	bool bFrameMaximized,
	CExtPaintManager::e_nc_button_state_t eStateButtonHelp,
	CExtPaintManager::e_nc_button_state_t eStateButtonMinimize,
	CExtPaintManager::e_nc_button_state_t eStateButtonMaximizeRestore,
	CExtPaintManager::e_nc_button_state_t eStateButtonClose,
	const CWnd * pWnd,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
//	pIcon;
//	strCaption;
//	nDrawTextAlignFlags;
//	rcFrame;
	rcClient;
//	rcIcon;
//	rcText;
//	rcHelp;
//	rcMinimize;
//	rcMaximizeRestore;
//	rcClose;
//	bFrameActive;
	bFrameEnabled;
//	bFrameMaximized;
//	eStateButtonHelp;
//	eStateButtonMinimize;
//	eStateButtonMaximizeRestore;
//	eStateButtonClose;
	pWnd;
	lParam;
LPCTSTR strFrameActive =
		bFrameActive ? _T("Active") : _T("Inactive");
const CExtSkinGlyph & glyphFrame =
		m_Skin[_T("NcFrame")][_T("NcArea")].Glyph(strFrameActive);
const CExtSkinColor & colorFrameText =
		m_Skin[_T("NcFrame")][_T("NcArea")].Color(strFrameActive);
	glyphFrame.Draw( dc.m_hDC, rcFrame, true );
	if(		pIcon != NULL
		&&	(! pIcon->IsEmpty() )
		&&	( ! ::IsRectEmpty( &rcIcon ) )
		&&	dc.RectVisible( &rcIcon )
		)
		pIcon->Paint(
			const_cast < CExtPaintManagerSkin * > ( this ),
			dc.m_hDC,
			rcIcon.left,
			rcIcon.top,
			-1,
			-1
			);
	if( ! colorFrameText.IsHollow() )
	{
		int nLenCaption =
			int( ( strCaption == NULL ) ? 0 : _tcslen( strCaption ) );
		if(		nLenCaption > 0
			&&	( ! ::IsRectEmpty( &rcText ) )
			&&	dc.RectVisible( &rcText )
			)
		{
			const CExtSkinFont & fontFrameText =
				m_Skin[_T("NcFrame")][_T("NcArea")].Font(strFrameActive);
			CFont _font;
			if( fontFrameText.CreateFont( _font ) )
			{
				COLORREF clrText =
					//RGB( 255, 255, 255 );
					COLORREF( colorFrameText );
				CFont * pOldFont = dc.SelectObject(
					//(CFont*)&m_FontBold
					&_font
					);
				COLORREF clrOldText = dc.SetTextColor( clrText );
				int nOldBkMode = dc.SetBkMode( TRANSPARENT );
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					strCaption, nLenCaption,
					(LPRECT)&rcText,
					nDrawTextAlignFlags|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX, 0
					);
				dc.SetBkMode( nOldBkMode );
				dc.SetTextColor( clrOldText );
				dc.SelectObject( pOldFont );
			} // if( fontFrameText.CreateFont( _font ) )
#ifdef _DEBUG
			else
			{
				ASSERT( FALSE );
			} // else from if( fontFrameText.CreateFont( _font ) )
#endif // _DEBUG
		}
	} // if( ! colorFrameText.IsHollow() )
	if(		( ! ::IsRectEmpty( &rcHelp ) )
		&&	dc.RectVisible( &rcHelp )
		)
	{
		LPCTSTR strGlyph = stat_NcFrameButton_NameFromState( eStateButtonHelp );
		const CExtSkinGlyph & glyph = m_Skin[_T("Button")][_T("FrameNcButton")][_T("Help")][strFrameActive].Glyph(strGlyph);
		glyph.Draw( dc.m_hDC, rcHelp );
	}
	if(		( ! ::IsRectEmpty( &rcMinimize ) )
		&&	dc.RectVisible( &rcMinimize )
		)
	{
		LPCTSTR strGlyph = stat_NcFrameButton_NameFromState( eStateButtonMinimize );
		const CExtSkinGlyph & glyph = m_Skin[_T("Button")][_T("FrameNcButton")]
			[
				( pWnd != NULL && pWnd->IsIconic() )
					? _T("Restore")
					: _T("Minimize")
			]
			[strFrameActive].Glyph(strGlyph);
		glyph.Draw( dc.m_hDC, rcMinimize );
	}
	if(		( ! ::IsRectEmpty( &rcMaximizeRestore ) )
		&&	dc.RectVisible( &rcMaximizeRestore )
		)
	{
		LPCTSTR strGlyph = stat_NcFrameButton_NameFromState( eStateButtonMaximizeRestore );
		const CExtSkinGlyph & glyph = m_Skin[_T("Button")][_T("FrameNcButton")][ bFrameMaximized ? _T("Restore") : _T("Maximize") ][strFrameActive].Glyph(strGlyph);
		glyph.Draw( dc.m_hDC, rcMaximizeRestore );
	}
	if(		( ! ::IsRectEmpty( &rcClose ) )
		&&	dc.RectVisible( &rcClose )
		)
	{
		LPCTSTR strGlyph = stat_NcFrameButton_NameFromState( eStateButtonClose );
		const CExtSkinGlyph & glyph = m_Skin[_T("Button")][_T("FrameNcButton")][_T("Close")][strFrameActive].Glyph(strGlyph);
		glyph.Draw( dc.m_hDC, rcClose );
	}
}

#if (!defined __EXT_MFC_NO_RIBBON_BAR)

void CExtPaintManagerSkin::Ribbon_EmbeddedCaptionPaintText(
	CDC & dc,
	__EXT_MFC_SAFE_LPCTSTR strCaption,
	CRect rcDrawText,
	__EXT_MFC_SAFE_LPCTSTR strCaptionDelimiter,
	const CExtRibbonBar * pRibbonBar,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pRibbonBar );
	if(		strCaption == NULL
		||	(! dc.RectVisible(&rcDrawText) )
		)
		return;

	if( IsHighContrast() )
	{
		CExtPaintManager::Ribbon_EmbeddedCaptionPaintText(
			dc,
			strCaption,
			rcDrawText,
			strCaptionDelimiter,
			pRibbonBar,
			lParam
			);
		return;
	}

INT nLenCaption = INT( _tcslen( strCaption ) );
	if( nLenCaption == 0 )
		return;
bool bFrameActive = true;
	if( pRibbonBar->m_pExtNcFrameImpl != NULL )
		bFrameActive = pRibbonBar->m_pExtNcFrameImpl->NcFrameImpl_IsActive();
LPCTSTR strFrameActive =
		bFrameActive ? _T("Active") : _T("Inactive");
const CExtSkinColor & colorFrameText =
		m_Skin[_T("NcFrame")][_T("NcArea")].Color(strFrameActive);
	if( colorFrameText.IsHollow() )
		return;

bool bRTL = ( (pRibbonBar->GetExStyle()&WS_EX_LAYOUTRTL) != 0 ) ? true : false;;
UINT nBaseDT = Ribbon_EmbeddedCaptionGetTextAlignmentFlags( pRibbonBar );
//UINT nAlignDT = nBaseDT & (DT_LEFT|DT_CENTER|DT_RIGHT);
UINT nDT = nBaseDT & (~(DT_CALCRECT));
	if( bRTL )
	{
		if( (nDT&3) == DT_RIGHT )
		{
			nDT &= ~3;
			nDT |= DT_LEFT;
		}
		else if( (nDT&3) == DT_LEFT )
		{
			nDT &= ~3;
			nDT |= DT_RIGHT;
		}
	} // if( bRTL )

CExtSafeString strLeft = strCaption, strMiddle, strRight;
INT nLenDelimiter = 0;
	if( strCaptionDelimiter != NULL )
	{
		nLenDelimiter = INT( _tcslen( strCaptionDelimiter ) );
		if( nLenDelimiter > 0 )
		{
			INT nPos = strLeft.Find( strCaptionDelimiter );
			if( nPos >= 0 )
			{
				strRight = strLeft.Right( nLenCaption - nPos - nLenDelimiter );
				strLeft = strLeft.Left( nPos );
				if( ! strRight.IsEmpty() )
					strMiddle = strCaptionDelimiter;
			} // if( nPos >= 0 )
		} // if( nLenDelimiter > 0 )
	} // if( strCaptionDelimiter != NULL )
CFont * pOldFont = dc.SelectObject( &m_FontCaption );
INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
COLORREF clrOldText = dc.GetTextColor();
	nDT &= ~(DT_END_ELLIPSIS);
INT nWidthLeft = stat_CalcTextDimension( dc, m_FontCaption, strLeft, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_CALCRECT ).Width();
INT nWidthMiddle = 0, nWidthRight = 0, nWidthAvailable = rcDrawText.Width();
	if( ! strRight.IsEmpty() )
	{
		nWidthMiddle = stat_CalcTextDimension( dc, m_FontCaption, strMiddle, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_CALCRECT ).Width();
		nWidthRight  = stat_CalcTextDimension( dc, m_FontCaption, strRight, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_CALCRECT ).Width();
	}

COLORREF clrRibbon_EmbeddedCaptionTextLeft = colorFrameText;
double lfHue = 0.0, lfSat = 0.0, lfLum = 0.0, lfAdjustmedLum = 0.25;
	stat_RGBtoHSL( clrRibbon_EmbeddedCaptionTextLeft, &lfHue, &lfSat, &lfLum );
	if( lfLum > 0.50 )
	{
		lfAdjustmedLum = -lfAdjustmedLum;
		if( lfLum > 0.75 )
			lfAdjustmedLum *= 0.25;
	}
	else
	{
		if( lfLum < 0.25 )
			lfAdjustmedLum *= 0.25;
	}
COLORREF clrRibbon_EmbeddedCaptionTextRight = stat_HLS_Adjust( clrRibbon_EmbeddedCaptionTextLeft, 0.0, lfAdjustmedLum, 0.0 );
	dc.SetTextColor( clrRibbon_EmbeddedCaptionTextLeft );
	if( nWidthAvailable > ( nWidthLeft + nWidthMiddle + nWidthRight ) && (! strRight.IsEmpty() ) )
	{
		if( (nDT&DT_RIGHT) != 0 )
			rcDrawText.left = rcDrawText.right - ( nWidthLeft + nWidthMiddle + nWidthRight );
		else if( (nDT&DT_CENTER) != 0 )
			rcDrawText.left += ( rcDrawText.Width() - ( nWidthLeft + nWidthMiddle + nWidthRight ) ) / 2; 
		nDT &= ~(DT_RIGHT|DT_CENTER);
		nDT |= DT_LEFT;
		CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(strLeft), strLeft.GetLength(), rcDrawText, nDT, 0 );
		rcDrawText.left += nWidthLeft;
		CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(strMiddle), strMiddle.GetLength(), rcDrawText, nDT, 0 );
		dc.SetTextColor( clrRibbon_EmbeddedCaptionTextRight );
		rcDrawText.left += nWidthMiddle;
		nDT |= DT_END_ELLIPSIS;
		CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(strRight), strRight.GetLength(), rcDrawText, nDT, 0 );
	} // if( nWidthAvailable > ( nWidthLeft + nWidthMiddle ) && (! strRight.IsEmpty() ) )
	else
	{
		nDT &= ~(DT_LEFT|DT_CENTER|DT_RIGHT);
		nDT |= DT_LEFT; // nDT |= nAlignDT;
		nDT |= DT_END_ELLIPSIS;
		if( bRTL )
			nDT |= DT_RTLREADING;
		CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(strLeft), strLeft.GetLength(), rcDrawText, nDT, 0 );
		if(! strRight.IsEmpty() )
		{
			rcDrawText.left += nWidthLeft;
			if( ( rcDrawText.Width() + nWidthMiddle ) > 0 )
			{
				CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(strMiddle), strMiddle.GetLength(), rcDrawText, nDT, 0 );
				dc.SetTextColor( clrRibbon_EmbeddedCaptionTextRight );
				rcDrawText.left += nWidthMiddle;
				CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(strRight), strRight.GetLength(), rcDrawText, nDT, 0 );
			}
		}
	} // else from if( nWidthAvailable > ( nWidthLeft + nWidthMiddle ) && (! strRight.IsEmpty() ) )
	dc.SetTextColor( clrOldText );
	dc.SetBkMode( nOldBkMode );
	dc.SelectObject( &pOldFont );
}

UINT CExtPaintManagerSkin::Ribbon_EmbeddedCaptionGetTextAlignmentFlags(
	const CExtRibbonBar * pRibbonBar,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pRibbonBar );
	if( IsHighContrast() )
		return 
			CExtPaintManager::Ribbon_EmbeddedCaptionGetTextAlignmentFlags(
				pRibbonBar,
				lParam
				);	
	return DT_SINGLELINE|DT_CENTER|DT_VCENTER;
}

void CExtPaintManagerSkin::Ribbon_EmbeddedCaptionAdjustTextRect(
	CRect & rcAdjust,
	const CExtRibbonBar * pRibbonBar,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pRibbonBar );

	if( IsHighContrast() )
	{
		CExtPaintManager::Ribbon_EmbeddedCaptionAdjustTextRect(
			rcAdjust,
			pRibbonBar,
			lParam
			);
		return;
	}

bool bDwmMode = false;
	if(		pRibbonBar->m_pExtNcFrameImpl != NULL
		&&	pRibbonBar->m_pExtNcFrameImpl->NcFrameImpl_IsDwmCaptionReplacement()
		)
		bDwmMode = true;
	if(		( ! bDwmMode )
		&&	pRibbonBar->GetParent()->IsZoomed()
		)
	{
		rcAdjust.OffsetRect( 0, 3 );
	}
}

#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

#define __EXT_MFC_SKINNED_CAPTION_TEXT_GAP 4

void CExtPaintManagerSkin::NcFrame_GetRects(
	CExtCmdIcon * pIcon,
	__EXT_MFC_SAFE_LPCTSTR strCaption,
	UINT nDrawTextAlignFlags,
	RECT & rcIcon,
	RECT & rcText,
	RECT & rcHelp,
	RECT & rcMinimize,
	RECT & rcMaximizeRestore,
	RECT & rcClose,
	const CWnd * pWnd,
	LPMINMAXINFO pMinMaxInfo, // = NULL
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	strCaption;
	nDrawTextAlignFlags;
	::SetRectEmpty( &rcIcon );
	::SetRectEmpty( &rcText );
	::SetRectEmpty( &rcHelp );
	::SetRectEmpty( &rcMinimize );
	::SetRectEmpty( &rcMaximizeRestore );
	::SetRectEmpty( &rcClose );
	ASSERT( NcFrame_IsSupported( pWnd ) );
DWORD dwWndStyle = pWnd->GetStyle();
//	if( (dwWndStyle&WS_CAPTION) == 0 )
//		return;
DWORD dwWndStyleEx = pWnd->GetExStyle();
CRect rcNcBorders, rcThemePadding;
	NcFrame_GetMetrics(
		rcNcBorders,
		rcThemePadding,
		pWnd,
		lParam
		);
CRect rcCaption =
		NcFrame_GetHtRect(
			HTCAPTION,
			false,
			true,
			true,
			pIcon,
			pWnd,
			pMinMaxInfo
			);
CRect rc = rcCaption;
	::CopyRect( &rcText, &rcCaption );
bool bZoomed = pWnd->IsZoomed()? true : false;
	if( (dwWndStyle&WS_SYSMENU) != 0 )
	{
		LPCTSTR strFrameActive = _T("Active");
		const CExtSkinGlyph & glyphClose = m_Skin[_T("Button")][_T("FrameNcButton")][_T("Close")][strFrameActive].Glyph(_T("Normal"));
		CSize _sizeScClose = glyphClose.GetSize();
		CRect _rcOffsetScClose = glyphClose.GetOffset();
		rc.right -= bZoomed ? rcThemePadding.right : 1;
		rc.right -= _rcOffsetScClose.right;
		rc.left = rc.right - _sizeScClose.cx;
		rc.top = rcCaption.top + _rcOffsetScClose.top + ( bZoomed ? _rcOffsetScClose.bottom : 0 );
		rc.bottom = rc.top + _sizeScClose.cy;
		::CopyRect( &rcClose, &rc );
		rc.OffsetRect( -_sizeScClose.cx, 0 );
		rcText.right = rcCaption.right - __EXT_MFC_SKINNED_CAPTION_TEXT_GAP;
		if( (dwWndStyle&WS_MAXIMIZEBOX) != 0 )
		{
			const CExtSkinGlyph & glyphMaximize = m_Skin[_T("Button")][_T("FrameNcButton")][_T("Maximize")][strFrameActive].Glyph(_T("Normal"));
			CSize _sizeScMaximize = glyphMaximize.GetSize();
			CRect _rcOffsetScMaximize = glyphMaximize.GetOffset();
			rc.right -= _rcOffsetScMaximize.right;
			rc.top = rcCaption.top + _rcOffsetScMaximize.top + ( bZoomed ? _rcOffsetScMaximize.bottom : 0 );
			rc.left = rc.right - _sizeScMaximize.cx;
			rc.bottom = rc.top + _sizeScMaximize.cy;
			::CopyRect( &rcMaximizeRestore, &rc );
			rc.OffsetRect( -_sizeScMaximize.cx, 0 );
			rcText.right = rcMaximizeRestore.left - __EXT_MFC_SKINNED_CAPTION_TEXT_GAP;
		} // if( (dwWndStyle&WS_MAXIMIZEBOX) != 0 )
		if( (dwWndStyle&WS_MINIMIZEBOX) != 0 )
		{
			const CExtSkinGlyph & glyphMinimize = m_Skin[_T("Button")][_T("FrameNcButton")][_T("Minimize")][strFrameActive].Glyph(_T("Normal"));
			CSize _sizeScMinimize = glyphMinimize.GetSize();
			CRect _rcOffsetScMinimize = glyphMinimize.GetOffset();
			rc.right -= _rcOffsetScMinimize.right;
			rc.top = rcCaption.top + _rcOffsetScMinimize.top + ( bZoomed ? _rcOffsetScMinimize.bottom : 0 );
			rc.left = rc.right - _sizeScMinimize.cx;
			rc.bottom = rc.top + _sizeScMinimize.cy;
			::CopyRect( &rcMinimize, &rc );
			rc.OffsetRect( -_sizeScMinimize.cx, 0 );
			rcText.right = rcMinimize.left - __EXT_MFC_SKINNED_CAPTION_TEXT_GAP;
		} // if( (dwWndStyle&WS_MINIMIZEBOX) != 0 )
		if( (dwWndStyleEx&WS_EX_CONTEXTHELP) != 0 )
		{
			const CExtSkinGlyph & glyphHelp = m_Skin[_T("Button")][_T("FrameNcButton")][_T("Help")][strFrameActive].Glyph(_T("Normal"));
			CSize _sizeScHelp = glyphHelp.GetSize();
			CRect _rcOffsetScHelp = glyphHelp.GetOffset();
			rc.right -= _rcOffsetScHelp.right;
			rc.top = rcCaption.top + _rcOffsetScHelp.top + ( bZoomed ? _rcOffsetScHelp.bottom : 0 );
			rc.left = rc.right - _sizeScHelp.cx;
			rc.bottom = rc.top + _sizeScHelp.cy;
			::CopyRect( &rcHelp, &rc );
			rcText.right = rcHelp.left - __EXT_MFC_SKINNED_CAPTION_TEXT_GAP;
		} // if( (dwWndStyleEx&WS_EX_CONTEXTHELP) != 0 )
		if(		pIcon != NULL
			&&	(! pIcon->IsEmpty() )
			)
		{
			CSize _sizeIcon = pIcon->GetSize();
			if( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 )
			{
				int x = rcCaption.left + (bZoomed ? rcNcBorders.left : 0 );
				int y = rcCaption.top + ( rcCaption.Height() - _sizeIcon.cy ) / 2;
				::SetRect(
					&rcIcon,
					x,
					y,
					x + _sizeIcon.cx,
					y + _sizeIcon.cy
					);
				rcText.left = rcIcon.right + __EXT_MFC_SKINNED_CAPTION_TEXT_GAP;
			} // if( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 )
			else
				rcText.left += __EXT_MFC_SKINNED_CAPTION_TEXT_GAP;
		} // if( pIcon != NULL ...
		else
			rcText.left += __EXT_MFC_SKINNED_CAPTION_TEXT_GAP + (bZoomed ? rcNcBorders.left : 0 );
	} // if( (dwWndStyle&WS_SYSMENU) != 0 )
	else
	{
		rcText.left += __EXT_MFC_SKINNED_CAPTION_TEXT_GAP + (bZoomed ? rcNcBorders.left : 0 );
		rcText.right -= __EXT_MFC_SKINNED_CAPTION_TEXT_GAP;
	}
}

bool CExtPaintManagerSkin::Bar_MiniDockFrameGetNcMetrics(
	INT & nResizingFrameDX,
	INT & nResizingFrameDY,
	INT & nCaptionDY,
	CExtMiniDockFrameWnd * pMiniFrameWnd,
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pMiniFrameWnd );
	pMiniFrameWnd;
	lParam;

LPCTSTR strBarType = _T("ResizableBar");
CExtToolControlBar * pToolCB = DYNAMIC_DOWNCAST( CExtToolControlBar, pBar );
	if( pToolCB )
	{
		strBarType = _T("ToolBar");
		if( pToolCB->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
			strBarType = _T("MenuBar");
	}

const CExtSkinItem & Frame = 
		m_Skin[_T("Frame")][_T("MiniFrame")][_T("ControlBar")][strBarType][_T("Active")];

const CExtSkinItem & Caption =
		m_Skin[_T("Caption")][_T("Frame")][_T("MiniFrame")][_T("ControlBar")][strBarType][_T("Active")];

const CExtSkinGlyph & FrameGlyph = Frame.Glyph( _T("Frame") );
const CExtSkinGlyph & CaptionGlyph = Caption.Glyph( _T("Caption") );

CRect rcFramePadding = FrameGlyph.GetPadding();
CSize szCaption = CaptionGlyph.GetSize();

	nResizingFrameDX = rcFramePadding.left;
	nResizingFrameDY = rcFramePadding.top;
	nCaptionDY = szCaption.cy;

	return true;
}

bool CExtPaintManagerSkin::Bar_MiniDockFrameNcCalcSize(
	RECT & rcAdjust,
	CExtMiniDockFrameWnd * pMiniFrameWnd,
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pMiniFrameWnd );
	rcAdjust;
	pMiniFrameWnd;
	pBar;
	lParam;

LPCTSTR strBarType = _T("ResizableBar");
CExtToolControlBar * pToolCB = DYNAMIC_DOWNCAST( CExtToolControlBar, pBar );
	if( pToolCB )
	{
		strBarType = _T("ToolBar");
		if( pToolCB->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
			strBarType = _T("MenuBar");
	}

const CExtSkinItem & Frame = 
		m_Skin[_T("Frame")][_T("MiniFrame")][_T("ControlBar")][strBarType][_T("Active")];

const CExtSkinItem & Caption =
		m_Skin[_T("Caption")][_T("Frame")][_T("MiniFrame")][_T("ControlBar")][strBarType][_T("Active")];

const CExtSkinGlyph & FrameGlyph = Frame.Glyph( _T("Frame") );
const CExtSkinGlyph & CaptionGlyph = Caption.Glyph( _T("Caption") );

CRect rcFramePadding = FrameGlyph.GetPadding();
CSize szCaption = CaptionGlyph.GetSize();

	rcAdjust.left += rcFramePadding.left;
	rcAdjust.top += rcFramePadding.top + szCaption.cy;
	rcAdjust.right -= rcFramePadding.right;
	rcAdjust.bottom -= rcFramePadding.bottom;

	return true;
}

bool CExtPaintManagerSkin::Bar_MiniDockFrameCaptionCalcRect(
	RECT & rcCaption,
	CExtMiniDockFrameWnd * pMiniFrameWnd,
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pMiniFrameWnd );
	pMiniFrameWnd;
	pBar;
	lParam;

	if( pMiniFrameWnd == NULL )
		return false;

LPCTSTR strBarType = _T("ResizableBar");
	CExtToolControlBar * pToolCB = DYNAMIC_DOWNCAST( CExtToolControlBar, pBar );
	if( pToolCB )
	{
		strBarType = _T("ToolBar");
		if( pToolCB->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
			strBarType = _T("MenuBar");
	}

	const CExtSkinItem & Frame = 
		m_Skin[_T("Frame")][_T("MiniFrame")][_T("ControlBar")][strBarType][_T("Active")];

	const CExtSkinItem & Caption =
		m_Skin[_T("Caption")][_T("Frame")][_T("MiniFrame")][_T("ControlBar")][strBarType][_T("Active")];

	const CExtSkinGlyph & FrameGlyph = Frame.Glyph( _T("Frame") );
	const CExtSkinGlyph & CaptionGlyph = Caption.Glyph( _T("Caption") );

	CRect rcFramePadding = FrameGlyph.GetPadding();
	CSize szCaption = CaptionGlyph.GetSize();

	CRect rcFrameCaption;
	::GetWindowRect( pMiniFrameWnd->GetSafeHwnd(), &rcFrameCaption );
	rcFrameCaption -= rcFrameCaption.TopLeft();

	rcFrameCaption.left += rcFramePadding.left;
	rcFrameCaption.top += rcFramePadding.top;
	rcFrameCaption.right -= rcFramePadding.right;
	rcFrameCaption.bottom = rcFrameCaption.top + szCaption.cy;

	::CopyRect( &rcCaption, &rcFrameCaption );

	return true;
}

void CExtPaintManagerSkin::PaintResizableBarSeparator(
	CDC & dc,
	const RECT & rc,
	bool bHorz,
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	ASSERT( dc.GetSafeHdc() != NULL );
	dc;
	rc;
	bHorz;
	pBar;
	lParam;
	
LPCTSTR strSeparatorType = _T("SeparatorV");
	if( bHorz )
		strSeparatorType = _T("SeparatorH");
	
	CExtSkinGlyph & SeparatorGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("ResizableBar")].Glyph( strSeparatorType );

	SeparatorGlyph.Draw( dc.m_hDC, rc );
}

bool CExtPaintManagerSkin::Bar_LayoutNcAreaButton(
	RECT & rcButton,
	const RECT & rcCaption,
	CExtBarNcAreaButton * pLayoutBtn,
	CExtBarNcAreaButton * pPrevBtn,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pLayoutBtn );
	pHelperSrc;
	lParam;

LPCTSTR strButtonType = _T("");
	if( pLayoutBtn->IsKindOf( RUNTIME_CLASS( CExtBarNcAreaButtonClose ) ) )
		strButtonType = _T("Close");
	else
	if(		pLayoutBtn->IsKindOf( RUNTIME_CLASS( CExtBarNcAreaButtonExpand ) )
		||	pLayoutBtn->IsKindOf( RUNTIME_CLASS( CExtBarNcAreaButtonMenu ) )
		)
		strButtonType = _T("Options");
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	else
	if( pLayoutBtn->IsKindOf( RUNTIME_CLASS( CExtBarNcAreaButtonAutoHide ) ) )
		strButtonType = _T("PinOff");
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	else
		return false;

LPCTSTR strBarActiveState =
		pLayoutBtn->IsBarWndActive() ? _T("Active") : _T("Inactive");

	const CExtSkinGlyph & ButtonFrameGlyph = 
		m_Skin[_T("Caption")][_T("CaptionButton")][_T("Normal")]
			[strButtonType][strBarActiveState].Glyph( _T("Frame") );
	if( ButtonFrameGlyph.IsEmpty() )
		return false;

CSize _size = ButtonFrameGlyph.GetSize();
INT x = rcCaption.right - _size.cx - ButtonFrameGlyph.GetPadding().right;
INT y = rcCaption.top;

CRect rc( x, y, x + _size.cx, y + _size.cy );
	if( pPrevBtn != NULL )
	{
		const CRect & rcPrev = *pPrevBtn;
		rc.OffsetRect(
			rcPrev.left - rc.right,
			0
			);
	}

	rc.OffsetRect( 
		-ButtonFrameGlyph.GetOffset().right,
		ButtonFrameGlyph.GetOffset().top 
		);

	rcButton = rc;

	return true;
}

CRect CExtPaintManagerSkin::Bar_GetCaptionBordersForText(
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	pBar;
	lParam;
// EG - TO DO - RETURN RESIZABLE BAR CAPTION PADDING
	return CRect( 2, 1, 2, 1 );
}

INT CExtPaintManagerSkin::Bar_SeparatorWidthGet(
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	pBar;
	lParam;

	const CExtSkinGlyph & SeparatorGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("ResizableBar")].Glyph( _T("SeparatorV") );

	return SeparatorGlyph.GetSize().cx;
}

INT CExtPaintManagerSkin::Bar_SeparatorHeightGet(
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	pBar;
	lParam;

	const CExtSkinGlyph & SeparatorGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("ResizableBar")].Glyph( _T("SeparatorH") );

	return SeparatorGlyph.GetSize().cy;
}

INT CExtPaintManagerSkin::Bar_GripperWidthAtLeftGet(
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	pBar;
	lParam;
	return 0;
}

INT CExtPaintManagerSkin::Bar_GripperHeightAtTopGet(
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	lParam;

	bool bFloating = true;

LPCTSTR strBarType = _T("ResizableBar");
	if( pBar )	
	{
		if( pBar->IsKindOf( RUNTIME_CLASS( CExtToolControlBar ) ) )
		{
			if( pBar->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
				strBarType = _T("MenuBar");
			else
				strBarType = _T("ToolBar");
		}

		bFloating = pBar->IsFloating();
	}
	
	const CExtSkinItem & Caption = bFloating 
		? m_Skin[_T("Caption")][_T("ControlBar")][strBarType][_T("Floating")][_T("Active")]
		: m_Skin[_T("Caption")][_T("ControlBar")][strBarType][_T("Active")];

	const CExtSkinGlyph & CaptionGlyph = Caption.Glyph( _T("Caption") );
		
	return CaptionGlyph.GetSize().cy;
}

HFONT CExtPaintManagerSkin::Bar_GetCaptionFont(
	bool bHorz,
	CExtToolControlBar * pBar,
	LPARAM lParam // = 0
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	bHorz;
	pBar;
	lParam;
	return NULL;
}

HFONT CExtPaintManagerSkin::CreateDefaultFont()
{
const CExtSkinFont & skinFont = m_Skin[_T("Button")].Font(_T("Measure"));
CFont _font;
	if( ! skinFont.CreateFont( _font ) )
		return CExtPaintManager::CreateDefaultFont();
	return (HFONT)_font.Detach();
}

CSize CExtPaintManagerSkin::Toolbar_GetMinButtonSize(
	CObject * pHelperSrc,
	LPARAM lParam // = 0
	)
{
	ASSERT_VALID( this );
	if( pHelperSrc != NULL )
	{
		if(		pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtPopupMenuWnd ) )
			||	(	pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtBarButton ) )
				&&	(	(!( ((CExtBarButton*)pHelperSrc)->GetBar() ) -> IsDockedVertically() )
					||	( ((CExtBarButton*)pHelperSrc)->GetNoRotateVerticalLayout() )
					)
				)
			)
		{
			CExtSkinGlyph & g =
				m_Skin[_T("Button")][_T("Simple")]
					[_T("PushButton")][_T("ControlBar")][_T("Horizontal")]
					[_T("ToolBar")][_T("Normal")].Glyph( _T("PushButton") )
					;
			CRect rcPaddingImage = g.GetPadding();
			CRect rcPaddingContent = g.GetContentPadding();
			CSize _size(
				rcPaddingContent.left + rcPaddingContent.right,
				rcPaddingImage.top + rcPaddingImage.bottom
				);
			return _size;
		}
		else if(
				pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtBarButton ) )
			)
		{
			CExtSkinGlyph & g =
				m_Skin[_T("Button")][_T("Simple")]
					[_T("PushButton")][_T("ControlBar")][_T("Vertical")]
					[_T("ToolBar")][_T("Normal")].Glyph( _T("PushButton") )
					;
			CRect rcPaddingImage = g.GetContentPadding();
			CRect rcPaddingContent = g.GetPadding();
			CSize _size(
				rcPaddingImage.left + rcPaddingImage.right,
				rcPaddingContent.top + rcPaddingContent.bottom
				);
		}
	} // if( pHelperSrc != NULL )
	return
		CExtPaintManager::Toolbar_GetMinButtonSize(
			pHelperSrc,
			lParam
			);
}

void CExtPaintManagerSkin::Toolbar_AdjustButtonSize(
	CExtBarButton * pTBB,
	CSize & sizePreCalc
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pTBB );
	if( pTBB->IsKindOf( RUNTIME_CLASS( CExtBarContentExpandButton ) ) )
		return;
CExtToolControlBar * pBar = pTBB->GetBar();
	if( pBar == NULL )
	{
		CExtPaintManager::Toolbar_AdjustButtonSize(
			pTBB,
			sizePreCalc
			);
		return;
	}

bool bHorizontal = ( (! pTBB->GetNoRotateVerticalLayout() ) && pBar->IsDockedVertically() ) ? false : true;
LPCTSTR strDockingType = bHorizontal ? _T("Horizontal") : _T("Vertical");
bool bDockSiteCustomizeMode =
		pBar->_IsDockSiteCustomizeMode();
bool bPushed =
		( pTBB->IsPressed() && (!bDockSiteCustomizeMode) )
			? true : false;
bool bEnabled =
		( pTBB->IsDisabled() && (!bDockSiteCustomizeMode) )
			? false : true;
bool bHover =
		( pTBB->IsHover() && (!bDockSiteCustomizeMode) )
			? true : false;
	if( ! bDockSiteCustomizeMode )
	{
		if(		CExtToolControlBar::g_bMenuTracking
			||	CExtPopupMenuWnd::IsMenuTracking()
			)
			bHover = false;
		else if( !bHover )
			bHover = pTBB->IsPressedTracking();
		//if( bPushed && bHover && IsPressedTracking() )
			//bHover = false;
	} // if( ! bDockSiteCustomizeMode )

CExtSkinItem * pButton = NULL; 
	if( pTBB->GetSeparatedDropDown() )
		pButton = (&(m_Skin[_T("Button")][_T("SeparatedDropDown")]));
	else if( pTBB->IsAbleToTrackMenu() )
		pButton = (&(m_Skin[_T("Button")][_T("DropDown")]));
	else
		pButton = (&(m_Skin[_T("Button")][_T("Simple")]));
LPCTSTR strState = _T("Normal");
	if( ! bEnabled )
		strState = _T("Disabled");
	else if( bPushed )
		strState = _T("Pressed");
	else if( bHover /*|| _ppbd.m_bDrawFocusRect*/ )
		strState = _T("Hover");
//	else if( _ppbd.m_bDefaultPushButton )
//		strState = _T("Default");
	pButton =
		(&((*pButton)
			[_T("PushButton")][_T("ControlBar")][strDockingType]
			[_T("ToolBar")][strState]
		));
CExtSkinGlyph & PushButtonGlyph =
		pButton->Glyph( _T("PushButton") );
CSize sizeSkin = PushButtonGlyph.GetSize();
	if( sizePreCalc.cx < sizeSkin.cx )
		sizePreCalc.cx = sizeSkin.cx;
	if( sizePreCalc.cy < sizeSkin.cy )
		sizePreCalc.cy = sizeSkin.cy;
}

HFONT CExtPaintManagerSkin::Toolbar_GetFont(
	bool bHorz,
	CExtControlBar * pBar,
	LPARAM lParam // = 0
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	pBar; lParam; bHorz;
	return ( (HFONT) m_FontNormal.GetSafeHandle() );
}

bool CExtPaintManagerSkin::Toolbar_GetBGInfo(
	RECT & rcBorders,
	INT & nGripWidthAtLeft,
	INT & nGripHeightAtTop,
	CExtToolControlBar * pBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	lParam;

DWORD dwBarStyle = pBar->GetBarStyle();
	nGripWidthAtLeft = 0;
	nGripHeightAtTop = 0;
	if(		(dwBarStyle&CBRS_FLOATING) != 0
		||	pBar->IsFloating()
		)
	{
		rcBorders.left = 0;
		rcBorders.top = 0;
		rcBorders.right = 0;
		rcBorders.bottom = 0;
		return true;
	}

const CExtSkinGlyph * pGripperGlyph = NULL;
const CExtSkinGlyph & BkGlyph = _GetDockedToolBarGlyph( pBar, &pGripperGlyph );
	ASSERT( pGripperGlyph != NULL );
	rcBorders = BkGlyph.GetPadding();
	if( (dwBarStyle&CBRS_ORIENT_HORZ) != 0 )
	{
		if( (dwBarStyle&CBRS_GRIPPER) != 0 )
			nGripWidthAtLeft = pGripperGlyph->GetSize().cx;
		rcBorders.top = 0;
		rcBorders.bottom = 0;
	} // if( (dwBarStyle&CBRS_ORIENT_HORZ) != 0 )
	else
	{
		if( (dwBarStyle&CBRS_GRIPPER) != 0 )
			nGripHeightAtTop = pGripperGlyph->GetSize().cy;
		rcBorders.left = 0;
		rcBorders.right = 0;
	} // else from if( (dwBarStyle&CBRS_ORIENT_HORZ) != 0 )
	return true;
}

const CExtSkinGlyph & CExtPaintManagerSkin::_GetDockedToolBarGlyph(
	CExtToolControlBar * pBar,
	const CExtSkinGlyph ** ppGlyphGripper // = NULL
	) const
{
	return
		(
			( const_cast < CExtPaintManagerSkin * > ( this ) ) ->
				_GetDockedToolBarGlyph(
				pBar,
				const_cast < CExtSkinGlyph ** > ( ppGlyphGripper )
				)
		);
}

CExtSkinGlyph & CExtPaintManagerSkin::_GetDockedToolBarGlyph(
	CExtToolControlBar * pBar,
	CExtSkinGlyph ** ppGlyphGripper // = NULL
	)
{
	ASSERT_VALID( pBar );
	ASSERT( ! pBar->IsFloating() );
LPCTSTR strBarType = _T("ToolBar");
	if( pBar->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
		strBarType = _T("MenuBar");

LPCTSTR strDockingType = _T("Horizontal");
	if( pBar->IsDockedVertically() )
		strDockingType = _T("Vertical");

LPCTSTR strDockingAbility = _T("Dockable");
	if( pBar->m_pDockSite == NULL )
		strDockingAbility = _T("Static");

CExtSkinGlyph & ToolBarBk =
		m_Skin[_T("Background")][_T("ControlBar")][strBarType][strDockingType][strDockingAbility]
		.Glyph( _T("Background") );

	if( ppGlyphGripper != NULL )
	{
		LPCTSTR strGlyphType = _T("GripperV");
		if( pBar->IsDockedVertically() )
			strGlyphType = _T("GripperH");

		CExtSkinGlyph & GripperGlyph =
			m_Skin[_T("ImageCollection")][_T("ControlBar")][strBarType][strDockingType]
			.Glyph( strGlyphType );
		(*ppGlyphGripper) = &GripperGlyph;
	}

	return ToolBarBk;
}

bool CExtPaintManagerSkin::Toolbar_GetSizeOfCEB(
	SIZE & sizeReal,
	const SIZE & sizePreCalc,
	CDC & dc,
	BOOL bHorz,
	CExtBarContentExpandButton * pTBB,
	LPARAM lParam // = 0
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pTBB );
	sizeReal;
	sizePreCalc;
	dc;
	bHorz;
	pTBB;
	lParam;

	LPCTSTR strToolBarType = bHorz ? _T("Horizontal") : _T("Vertical");
	LPCTSTR strDockingAbility = _T("Dockable");
	CExtToolControlBar * pToolBar = pTBB->GetBar();
	if( pToolBar && pToolBar->m_pDockSite == NULL )
		strDockingAbility = _T("Static");

	CExtSkinGlyph & ChevronGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("ToolBar")][strToolBarType][strDockingAbility]
		.Glyph( _T("ChevronPartial") );
		
	if( bHorz )
	{
		sizeReal.cx = ChevronGlyph.GetSize().cx;
		sizeReal.cy = sizePreCalc.cy;
	} // if( bHorz )
	else
	{
		sizeReal.cx = sizePreCalc.cx;
		sizeReal.cy = ChevronGlyph.GetSize().cy;
	} // else from if( bHorz )

	return true;
}

void CExtPaintManagerSkin::PaintToolbarExpandButton(
	CDC & dc,
	const RECT & rcButtonArea,
	bool bHorz, // if false - down
	bool bBarIsCompletelyVisible,
	bool bEnabled,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam, // = 0L
	bool bTransparentBackground // = false
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	dc;
	rcButtonArea;
	bHorz;
	bBarIsCompletelyVisible;
	bEnabled;
	bPushed;
	bHover;
	pHelperSrc;
	lParam;
	bTransparentBackground;

CRect rect(rcButtonArea);
	rect.NormalizeRect();
	if( !dc.RectVisible( &rect ) )
		return;

CRect rectButton( rect );
	PAINTPUSHBUTTONDATA _ppbd(
		pHelperSrc, bHorz, rectButton, NULL, 
		NULL, false, bHover, bPushed, false, 
		bEnabled, false, false, false, 0
		);

	PaintPushButton( dc, _ppbd );

	LPCTSTR strToolBarType = bHorz ? _T("Horizontal") : _T("Vertical");
	LPCTSTR strChevronType = bBarIsCompletelyVisible ? _T("ChevronAll") : _T("ChevronPartial");
	LPCTSTR strDockingAbility = _T("Dockable");
	CExtToolControlBar * pToolBar = DYNAMIC_DOWNCAST( CExtToolControlBar, pHelperSrc );
	if( pToolBar && pToolBar->m_pDockSite == NULL )
		strDockingAbility = _T("Static");

	CExtSkinGlyph & ChevronGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("ToolBar")][strToolBarType][strDockingAbility]
		.Glyph( strChevronType );
	ChevronGlyph.Draw( dc.m_hDC, rcButtonArea );
}

void CExtPaintManagerSkin::PaintToolbarTextField(
	CDC & dc,
	PAINTTOOLBARTEXTFIELDDATA & _ptbtfd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

CRect rcTextField( _ptbtfd.m_rcButton );
	if(	! _ptbtfd.m_bEnabled )  
		rcTextField.DeflateRect( 1, 2 );
	if( _ptbtfd.m_bComboField )
	{
		PAINTCOMBOFRAMEDATA _pcfd(
			_ptbtfd.m_pHelperSrc,
				rcTextField,
			_ptbtfd.m_bHover,
			_ptbtfd.m_bPushed,
			_ptbtfd.m_bEnabled
			);
		_pcfd.m_bNoTopOuterBorder = true;
		PaintComboFrame( dc, _pcfd );
		dc.SelectClipRgn( NULL );
	} // if( _ptbtfd.m_bComboField )
	else
	{
	} // else from if( _ptbtfd.m_bComboField )

CRect rcBk( _ptbtfd.m_rcTextField );
	if( ! _ptbtfd.m_bEnabled )
		rcBk.DeflateRect( 0, 1 );

//	dc.FillSolidRect(
//		&rcBk,
//		GetColor( _ptbtfd.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE, _ptbtfd.m_pHelperSrc, _ptbtfd.m_lParam )
//		);

	if( ! _ptbtfd.m_sText.IsEmpty() )
	{
		COLORREF clrTextOld =
			dc.SetTextColor( 
				::GetSysColor( _ptbtfd.m_bEnabled ? COLOR_WINDOWTEXT : COLOR_GRAYTEXT )
				);
		CFont * pFontOld =
			dc.SelectObject( &m_FontNormal );
		int nBkModeOld = dc.SetBkMode( TRANSPARENT );
		CRect rcText( _ptbtfd.m_rcTextField );
		rcText.left += 5;
		rcText.top += 2;
		bool bRTL = ( (g_ResourceManager->OnQueryLangLayout()&LAYOUT_RTL) != 0 ) ? true : false;
		DWORD dwDtAlignFlags = bRTL ? (DT_RTLREADING|DT_RIGHT) : 0;
		CExtRichContentLayout::stat_DrawText(
			dc.m_hDC,
			LPCTSTR(_ptbtfd.m_sText), _ptbtfd.m_sText.GetLength(),
			&rcText,
			_ptbtfd.m_nDrawTextFlags|dwDtAlignFlags, 0
			);
		dc.SetBkMode( nBkModeOld );
		dc.SelectObject( pFontOld );
		dc.SetTextColor( clrTextOld );
	} // if( !_ptbtfd.m_sText.IsEmpty() )
}

void CExtPaintManagerSkin::PaintComboFrame(
	CDC & dc,
	CExtPaintManager::PAINTCOMBOFRAMEDATA & _pcfd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
INT nDD = GetSystemMetrics(SM_CXHTHUMB);
CRect rectClient( _pcfd.m_rcClient );
	if(		_pcfd.m_pHelperSrc != NULL
		&&	_pcfd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CComboBox) )
		)
	{
		CRect rcExclude( rectClient );
		rcExclude.DeflateRect( 2, 2 );
		if( _pcfd.m_bRightText )
			rcExclude.left += nDD;
		else
			rcExclude.right -= nDD;
		dc.ExcludeClipRect( &rcExclude );
		PaintDockerBkgnd(
			true,
			dc,
			(CWnd*)_pcfd.m_pHelperSrc,
			_pcfd.m_lParam
			);
	}
	
LPCTSTR strGlyphName = _T("Disabled");
	if( _pcfd.m_bEnabled )
	{
		if( _pcfd.m_bPushed )
			strGlyphName = _T("Pressed");
		else if( _pcfd.m_bHover )
			strGlyphName = _T("Hover");
		else
			strGlyphName = _T("Normal");
	} // if( _pcfd.m_bEnabled )

CRect rectFrame = rectClient;
CRect rcDDButton( rectClient );
	rcDDButton.left = rcDDButton.right - nDD;
	rectFrame.right = rcDDButton.left;

CExtSkinGlyph & glyphFrame = 
		m_Skin[_T("ComboBox")][_T("ComboBoxFrame")].Glyph( strGlyphName );
CExtSkinGlyph & glyphDropDownButton = 
		m_Skin[_T("ComboBox")][_T("ComboBoxDropDownButton")].Glyph( strGlyphName );
CExtSkinGlyph & glyphDropDownArrow = 
		m_Skin[_T("ComboBox")][_T("ComboBoxDropDownArrow")].Glyph( strGlyphName );

	if( _pcfd.m_bRightText )
	{
		CDC dcX;
		VERIFY( dcX.CreateCompatibleDC( NULL ) );

		CExtBitmap _bmpBk;
		VERIFY( _bmpBk.FromSurface( dc.m_hDC, rectClient ) );
  		COLORREF * pClrSurface = NULL;
		HBITMAP hBitmap = _bmpBk.CreateBitmap( &pClrSurface );
		ASSERT( hBitmap != NULL && pClrSurface != NULL );
		CBitmap _bmp;
		_bmp.Attach( hBitmap );
		
		CBitmap * pOldBmp = dcX.SelectObject( CBitmap::FromHandle( hBitmap ) );
	
		glyphFrame.Draw( dcX.GetSafeHdc(), rectFrame );
		glyphDropDownButton.Draw( dcX.GetSafeHdc(), rcDDButton );
 		glyphDropDownArrow.Draw( dcX.GetSafeHdc(), rcDDButton );

		dcX.SelectObject( pOldBmp );

		CExtBitmap _bmpData;
		VERIFY( _bmpData.FromBitmap( hBitmap ) );
		_bmpData.FlipHorizontal();
 		_bmpData.Draw( dc.m_hDC, rectClient );
	}
	else
	{
		glyphFrame.Draw( dc.m_hDC, rectFrame );
		glyphDropDownButton.Draw( dc.m_hDC, rcDDButton );
		glyphDropDownArrow.Draw( dc.m_hDC, rcDDButton );
	}
}

void CExtPaintManagerSkin::PaintPushButton(
	CDC & dc,
	CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( _ppbd.m_rcClient.IsRectEmpty() )
		return;
	
#if (!defined __EXT_MFC_NO_RIBBON_BAR)
	if( _ppbd.m_pHelperSrc != NULL )
	{
		CExtBarButton * pTBB = DYNAMIC_DOWNCAST( CExtBarButton, _ppbd.m_pHelperSrc );
		if( pTBB != NULL )
		{
			if(		pTBB->IsRibbonPaintingMode()
				||	pTBB->IsKindOf( RUNTIME_CLASS( CExtRibbonButtonDialogLauncher ) )
				)
			{
				Ribbon_PaintPushButton( dc, _ppbd );
				return;
			}
			CExtToolControlBar * pBar = pTBB->GetBar();
			if(		pBar != NULL
				&&	pBar->IsKindOf( RUNTIME_CLASS( CExtRibbonPage ) )
				&&	(! pTBB->IsNoRibbonLayout() )
				&&	(! pTBB->IsKindOf( RUNTIME_CLASS( CExtBarMdiRightButton ) ) )
				)
			{
				Ribbon_PaintPushButton( dc, _ppbd );
				return;
			}
		} // if( pTBB != NULL )
	} // if( _ppbd.m_pHelperSrc != NULL )
#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

	if(		_ppbd.m_pHelperSrc != NULL
		&&	(	(	_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPopupColorMenuWnd) )
				&&	_ppbd.m_bHelperColorCellButton
				)
#if (!defined __EXT_MFC_NO_DATE_PICKER)
			||	_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtDatePickerWnd) )
#endif
			)
		)
	{
		CExtPaintManager::PaintPushButton( dc, _ppbd );
		return;
	}

enum ButtonDrawingMode
{
	eModeControl,
	eModeToolBar,
	eModeMenuBar,
	eModePopupMenu,
};

bool bHorizontal = true;
ButtonDrawingMode aDrawingMode = eModeControl;
	if( _ppbd.m_pHelperSrc != NULL  )
	{
		if( _ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtBarButton ) ) )
		{
			CExtBarButton * pBarButton = (CExtBarButton*)_ppbd.m_pHelperSrc;
			CExtToolControlBar * pToolBar = DYNAMIC_DOWNCAST( CExtToolControlBar, pBarButton->GetBar() );
			ASSERT( pToolBar != NULL );
			
			if( pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
				aDrawingMode = eModeMenuBar;
			else
				aDrawingMode = eModeToolBar;

			if( ! pBarButton->GetNoRotateVerticalLayout() )
			{
				if( pToolBar->IsDockedVertically() )
					bHorizontal = false;
			}
		}
		else if( _ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtPopupMenuWnd ) ) )
		{
			aDrawingMode = eModePopupMenu;
		}
		else if( _ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CWnd ) ) )
		{
			PaintDockerBkgnd( true, dc, (CWnd*)_ppbd.m_pHelperSrc );
		}
	}

CExtSkinItem 
		* pButton = NULL, 
		* pPushButton = NULL,
		* pDropDownButton = NULL,
		* pDropDownArrow = NULL;

	if( _ppbd.m_bSeparatedDropDown )
		pButton = (&(m_Skin[_T("Button")][_T("SeparatedDropDown")]));
	else if( _ppbd.m_bDropDown )
		pButton = (&(m_Skin[_T("Button")][_T("DropDown")]));
	else
		pButton = (&(m_Skin[_T("Button")][_T("Simple")]));

LPCTSTR strDockingType = bHorizontal ? _T("Horizontal") : _T("Vertical");

	if( ! _ppbd.m_bEnabled )
	{
		_ppbd.m_bHover = false;
		_ppbd.m_bDrawFocusRect = false;
		_ppbd.m_bIndeterminate = false;
	}

	if( _ppbd.m_bSeparatedDropDown && (_ppbd.m_bPushed || _ppbd.m_bPushedDropDown) )
		_ppbd.m_bHover = false;

LPCTSTR strState = _T("Normal");
	if( ! _ppbd.m_bEnabled )
		strState = _T("Disabled");
//	else if( _ppbd.m_bPushed )
	else if( _ppbd.m_bPushed || _ppbd.m_bIndeterminate )
		strState = _T("Pressed");
	else if( _ppbd.m_bHover || _ppbd.m_bDrawFocusRect )
		strState = _T("Hover");
//	else if( _ppbd.m_bIndeterminate )
//		strState = _T("Indeterminate");
	else if( _ppbd.m_bDefaultPushButton )
		strState = _T("Default");

	switch( aDrawingMode )
	{
	case eModeControl:
		{
			pPushButton = &(*pButton)[_T("PushButton")][_T("Control")][strState];
			if( _ppbd.m_bDropDown )
			{
                pDropDownButton = &(*pButton)[_T("DropDownButton")][_T("Control")][strState];
				pDropDownArrow = &(*pButton)[_T("DropDownArrow")][_T("Control")][strState];
			}

			break;
		}
	case eModePopupMenu:
	case eModeToolBar:
		{
				pPushButton =
					(&((*pButton)
						[_T("PushButton")][_T("ControlBar")][strDockingType]
						[_T("ToolBar")][strState]
					));
			if( aDrawingMode == eModePopupMenu )
				pPushButton =
					(&((*pPushButton)[_T("PaletteMenu")]));
			if( _ppbd.m_bDropDown )
			{
				pDropDownButton = 
					(&((*pButton)
						[_T("DropDownButton")][_T("ControlBar")][strDockingType]
						[_T("ToolBar")][strState]
					));
				pDropDownArrow = 
					(&((*pButton)
						[_T("DropDownArrow")][_T("ControlBar")][strDockingType]
						[_T("ToolBar")][strState]
					));
				if( aDrawingMode == eModePopupMenu )
				{
					pDropDownButton =
						(&((*pDropDownButton)[_T("PaletteMenu")]));
					pDropDownArrow =
						(&((*pDropDownArrow)[_T("PaletteMenu")]));
				}
			} // if( _ppbd.m_bDropDown )
			else
			{
				if(		_ppbd.m_pHelperSrc != NULL
					&&	_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtBarContentExpandButton ) )
					)
					pPushButton =
						(&((*pPushButton)[_T("Chevron")]));
			} // else from if( _ppbd.m_bDropDown )
			break;
		}
	case eModeMenuBar:
		{
				pPushButton =
					(&((*pButton)
						[_T("PushButton")][_T("ControlBar")][strDockingType]
						[_T("ToolBar")][_T("MenuBar")][strState]
					));
			if( _ppbd.m_bDropDown )
			{
				pDropDownButton = 
					(&((*pButton)
						[_T("DropDownButton")][_T("ControlBar")][strDockingType]
						[_T("ToolBar")][_T("MenuBar")][strState]
					));
				pDropDownArrow = 
					(&((*pButton)
						[_T("DropDownArrow")][_T("ControlBar")][strDockingType]
						[_T("ToolBar")][_T("MenuBar")][strState]
					));
			}

			break;
		}
	}

	ASSERT( pPushButton != NULL );
	ASSERT( _ppbd.m_bDropDown ? ( pDropDownButton != NULL ) : true );
	ASSERT( _ppbd.m_bDropDown ? ( pDropDownArrow != NULL ) : true );

	if(		(! _ppbd.m_bPushed )
		&&	pDropDownButton
		&&	_ppbd.m_bPushedDropDown
		)
		pDropDownButton =
			(&( pDropDownButton->GetParent()->Nested( _T("Pressed") ) ));

BYTE nScaPushButton = BYTE(0x0FF), nScaDropDownButton = BYTE(0x0FF), nScaDropDownArrow = BYTE(0x0FF);
	if( _ppbd.m_bIndeterminate )
	{
		if( pPushButton != NULL )
		{
			CExtSkinItem * pIndeterminate = pPushButton->NestedGet( _T("Indeterminate") );
			if( pIndeterminate != NULL )
				pPushButton = pIndeterminate;
			else
				nScaPushButton = BYTE(0x080);
		}
		if( pDropDownButton != NULL )
		{
			CExtSkinItem * pIndeterminate = pDropDownButton->NestedGet( _T("Indeterminate") );
			if( pIndeterminate != NULL )
				pDropDownButton = pIndeterminate;
			else
				nScaDropDownButton = BYTE(0x080);
		}
		if( pDropDownArrow != NULL )
		{
			CExtSkinItem * pIndeterminate = pDropDownArrow->NestedGet( _T("Indeterminate") );
			if( pIndeterminate != NULL )
				pDropDownArrow = pIndeterminate;
			else
				nScaDropDownArrow = BYTE(0x080);
		}
	}

bool bRTL = _ppbd.IsRTL();
CRect rectClient( _ppbd.m_rcClient );
//	rectClient.DeflateRect( _ppbd.m_rcBorderSizes );
CRect rectCaption( &rectClient );

	if( _ppbd.m_bDropDown )
	{
		CRect rectDeflateSimpleDropDown( 0, 0, 0, 0 );
		if( ! _ppbd.m_bSeparatedDropDown )
		{
			CExtSkinGlyph & PushButtonGlyph =
				pPushButton->Glyph( _T("PushButton") );
			PushButtonGlyph.Draw( dc.m_hDC, rectClient );
			rectDeflateSimpleDropDown =
				PushButtonGlyph.GetContentPadding();
			rectClient.DeflateRect( &rectDeflateSimpleDropDown );
		} // if( ! _ppbd.m_bSeparatedDropDown )
		CRect rectDropDown( rectClient );
		if( _ppbd.m_bHorz )
		{
			if( bRTL )
			{
				rectCaption.left += _CalcSplitButtonExtentImpl( _ppbd.m_pHelperSrc, _ppbd.m_bHorz );
				rectDropDown.right = rectCaption.left;
			} // if( bRTL )
			else
			{
				rectCaption.right -= _CalcSplitButtonExtentImpl( _ppbd.m_pHelperSrc, _ppbd.m_bHorz );
				rectDropDown.left = rectCaption.right;
			} // else from if( bRTL )
		} // if( _ppbd.m_bHorz )
		else
		{
			rectCaption.bottom -= _CalcSplitButtonExtentImpl( _ppbd.m_pHelperSrc, _ppbd.m_bHorz );
			rectDropDown.top = rectCaption.bottom;
		} // if( _ppbd.m_bHorz )

		rectClient = rectCaption;

		if( _ppbd.m_bSeparatedDropDown )
		{
			CExtSkinGlyph & PushButtonGlyph =
				pPushButton->Glyph( _T("PushButton") );
			PushButtonGlyph.Draw( dc.m_hDC, rectClient, false, nScaPushButton );
			rectClient.DeflateRect( 
				&PushButtonGlyph.GetContentPadding()
				);

			CExtSkinGlyph & DropDownButtonGlyph =
				pDropDownButton->Glyph( _T("DropDownButton") );
			DropDownButtonGlyph.Draw( dc.m_hDC, rectDropDown, false, nScaDropDownButton );

			CRect rectDropDownArrow( rectDropDown );
			rectDropDownArrow.DeflateRect(
				DropDownButtonGlyph.GetContentPadding()
				);

			CExtSkinGlyph & DropDownArrowGlyph =
				pDropDownArrow->Glyph( _T("DropDownArrow") );
			if( ! _ppbd.m_bNoDropDownArrow )
				DropDownArrowGlyph.Draw( dc.m_hDC, rectDropDownArrow, false, nScaDropDownArrow );
		}
		else
		{
			CExtSkinGlyph & DropDownArrowGlyph =
				pDropDownArrow->Glyph( _T("DropDownArrow") );
			if( ! _ppbd.m_bNoDropDownArrow )
				DropDownArrowGlyph.Draw( dc.m_hDC, rectDropDown, false, nScaDropDownArrow );
			rectClient.DeflateRect( &rectDeflateSimpleDropDown );
			rectCaption = rectClient;
		}
	} // if( _ppbd.m_bDropDown )
	else
	{
		CExtSkinGlyph & PushButtonGlyph =
			pPushButton->Glyph( _T("PushButton") );
		PushButtonGlyph.Draw( dc.m_hDC, rectClient, false, nScaPushButton );
		rectClient.DeflateRect( 
			&PushButtonGlyph.GetContentPadding()
			);
		rectCaption = rectClient;
	} // else from if( _ppbd.m_bDropDown )

CRect rcFocus( rectCaption );

CExtSafeString sTitle = _T("");
	if( _ppbd.m_sText != NULL )
		sTitle = _ppbd.m_sText;

COLORREF clrOldText = dc.GetTextColor();
COLORREF clrOldBk = dc.GetBkColor();
int nOldBkMode = dc.SetBkMode( TRANSPARENT );

COLORREF bgColor = 0;
	if( _ppbd.m_clrForceBk != ((COLORREF)-1L) )
		bgColor = _ppbd.m_clrForceBk;
	else if( _ppbd.m_bHover || _ppbd.m_bPushed )
		bgColor = GetColor( CLR_3DFACE_IN, _ppbd.m_pHelperSrc, _ppbd.m_lParam );
	else
		bgColor = GetColor( CLR_3DFACE_OUT, _ppbd.m_pHelperSrc, _ppbd.m_lParam );

	// Draw the icon
	if(		rectClient.bottom > rectClient.top
		&&	rectClient.right > rectClient.left
		)
	{
		if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		{
			PaintIcon(
				dc,
				_ppbd.m_bHorz,
				_ppbd.m_pIcon,
				rectClient,
				_ppbd.m_bPushed,
				_ppbd.m_bEnabled,
				_ppbd.m_bHover,
				_ppbd.m_nIconAlignment,
				_ppbd.m_bForceRtlLayout && _ppbd.m_bHorz,
				NULL,
				&rectCaption,
				&_ppbd.m_rcIconMargins,
				_ppbd.m_clrCustomAccentEffectForIcon
				);
			
			if( _ppbd.m_nFocusMode == __EBFM_CAPTION_AREA )
				rcFocus = rectCaption;

			if(		_ppbd.m_bHorz
				&&	_ppbd.m_pHelperSrc != NULL
				&&	_ppbd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtPopupMenuWnd))
				)
			{ 
				// if painting tool button with icon in popup menu
				if( bRTL )
					rectCaption.right -= 3;
				else
					rectCaption.left += 3;
			}

			if( ! rectCaption.IsRectEmpty() )
			{
				if( _ppbd.m_bHorz )
				{
					rectCaption.DeflateRect( 
						_ppbd.m_rcTextMargins.left,
						_ppbd.m_rcTextMargins.top,
						_ppbd.m_rcTextMargins.right,
						_ppbd.m_rcTextMargins.bottom
						);
				}
				else
				{
					rectCaption.DeflateRect( 
						_ppbd.m_rcTextMargins.top,
						_ppbd.m_rcTextMargins.right,
						_ppbd.m_rcTextMargins.bottom,
						_ppbd.m_rcTextMargins.left
						);			
				}
			}
		} // if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
		else
		{
// -2.62
// 			if( _ppbd.m_bHorz )
// 				rectCaption.DeflateRect( 2, 0 );
// 			else
// 				rectCaption.DeflateRect( 0, 2 );
			PaintSysCmdGlyph(
				dc,
				_ppbd.m_nHelperSysCmdID,
				rectClient,
				false,
				_ppbd.m_bPushed,
				_ppbd.m_bEnabled
				);

		} // else from if( _ppbd.m_pIcon != NULL && (! _ppbd.m_pIcon->IsEmpty()) )
	}

	if(		rectCaption.bottom > rectCaption.top
		&&	rectCaption.right > rectCaption.left
		)
	{ // if we have valid area for text
		if( ! sTitle.IsEmpty() )
		{
			if( _ppbd.m_bPushed )
			{
				CSize sizePushedOffset = GetPushedOffset();
				rectCaption.OffsetRect(sizePushedOffset);
			}
			dc.SetBkColor( bgColor );

			COLORREF clrText = COLORREF(-1L);
			if( _ppbd.m_bEnabled )
			{
				if( _ppbd.m_bPushed )
				{
					if( _ppbd.m_clrForceTextPressed == COLORREF(-1L) )
					{
						const CExtSkinColor & skinColor =
							pPushButton->Color( _T("Pressed") );
						if( ! skinColor.IsHollow() )
							clrText = COLORREF( skinColor );
					}
					else
						clrText = _ppbd.m_clrForceTextPressed;
				}
				else if( _ppbd.m_bHover )
				{
					if( _ppbd.m_clrForceTextHover == COLORREF(-1L) )
					{
						const CExtSkinColor & skinColor =
							pPushButton->Color( _T("Hover") );
						if( ! skinColor.IsHollow() )
							clrText = COLORREF( skinColor );
					}
					else
						clrText = _ppbd.m_clrForceTextHover;
				}
				else
				{
					if( _ppbd.m_clrForceTextNormal == COLORREF(-1L) )
					{
						const CExtSkinColor & skinColor =
							pPushButton->Color( _T("Normal") );
						if( ! skinColor.IsHollow() )
							clrText = COLORREF( skinColor );
					}
					else
						clrText = _ppbd.m_clrForceTextNormal;
				}
			} // if( _ppbd.m_bEnabled )
			else
			{
					if( _ppbd.m_clrForceTextDisabled == COLORREF(-1L) )
					{
						const CExtSkinColor & skinColor =
							pPushButton->Color( _T("Disabled") );
						if( ! skinColor.IsHollow() )
							clrText = COLORREF( skinColor );
					}
					else
						clrText = _ppbd.m_clrForceTextDisabled;
			} // else from if( _ppbd.m_bEnabled )
			if( clrText != COLORREF(-1L) )
			{ // if skinned color is not hollow
				dc.SetTextColor( clrText );

				CFont _font, * pCurrFont = NULL;
				if( _ppbd.m_hFont != NULL )
					pCurrFont = CFont::FromHandle(_ppbd.m_hFont);
				else
				{
					if( _ppbd.m_bEnabled )
					{
						if( _ppbd.m_bPushed )
						{
							const CExtSkinFont & skinFont =
								pPushButton->Font(
									_ppbd.m_bDefaultPushButton
										? _T("PressedDefault")
										: _T("Pressed")
									);
							if(	skinFont.CreateFont( _font ) )
								pCurrFont = &_font;
#ifdef _DEBUG
							else
							{
								ASSERT( FALSE );
							}
#endif // _DEBUG
						}
						else if( _ppbd.m_bHover )
						{
							const CExtSkinFont & skinFont =
								pPushButton->Font(
									_ppbd.m_bDefaultPushButton
										? _T("HoverDefault")
										: _T("Hover")
									);
							if(	skinFont.CreateFont( _font ) )
								pCurrFont = &_font;
#ifdef _DEBUG
							else
							{
								ASSERT( FALSE );
							}
#endif // _DEBUG
						}
						else
						{
							const CExtSkinFont & skinFont =
								pPushButton->Font(
									_ppbd.m_bDefaultPushButton
										? _T("NormalDefault")
										: _T("Normal")
									);
							if(	skinFont.CreateFont( _font ) )
								pCurrFont = &_font;
#ifdef _DEBUG
							else
							{
								ASSERT( FALSE );
							}
#endif // _DEBUG
						}
					} // if( _ppbd.m_bEnabled )
					else
					{
							const CExtSkinFont & skinFont =
								pPushButton->Font(
									_ppbd.m_bDefaultPushButton
										? _T("DisabledDefault")
										: _T("Disabled")
									);
							if(	skinFont.CreateFont( _font ) )
								pCurrFont = &_font;
#ifdef _DEBUG
							else
							{
								ASSERT( FALSE );
							}
#endif // _DEBUG
					} // else from if( _ppbd.m_bEnabled )
				} // else from if( _ppbd.m_hFont != NULL )
				if( pCurrFont->GetSafeHandle() != NULL )
				{
					CFont * pOldFont = dc.SelectObject( pCurrFont );
					CRect rcTextLocation( 0, 0, 0, 0);
					CExtRichContentLayout::e_layout_orientation_t eLO = CExtRichContentLayout::__ELOT_NORMAL;
					if( ! _ppbd.m_bHorz )
						eLO = CExtRichContentLayout::__ELOT_90_CW;
					UINT nDtMeasureFlags = DT_LEFT | DT_TOP | DT_CALCRECT | ((bRTL || _ppbd.m_bForceRtlText) ? DT_RTLREADING : 0);
					if( _ppbd.m_bWordBreak )
					{
						rcTextLocation = rectCaption;
						rcTextLocation.OffsetRect( -rcTextLocation.TopLeft() );
						rcTextLocation.bottom = rcTextLocation.top;
						nDtMeasureFlags |= DT_WORDBREAK;
					}
					else
						nDtMeasureFlags |= DT_SINGLELINE;
					CExtRichContentLayout::stat_DrawText(
						CExtRichContentLayout::__ELFMT_AUTO_DETECT, eLO,
						dc.m_hDC, LPCTSTR(sTitle), rcTextLocation, nDtMeasureFlags, 0
						);
					rcTextLocation.OffsetRect( rectCaption.TopLeft() - rcTextLocation.TopLeft() );
						
					UINT nDtDrawFlags = 0;
					if( (_ppbd.m_nTextAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_RIGHT )
					{
						nDtDrawFlags |= DT_RIGHT;
						rcTextLocation.OffsetRect(
							rectCaption.Width() - rcTextLocation.Width(),
							0
							);
					}
					else if( (_ppbd.m_nTextAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
					{
						nDtDrawFlags |= DT_CENTER;
						rcTextLocation.OffsetRect(
							( rectCaption.Width() - rcTextLocation.Width() ) / 2,
							0
							);
					}
					else
						nDtDrawFlags |= DT_LEFT;
						
					if( (_ppbd.m_nTextAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_BOTTOM )
					{
						nDtDrawFlags |= DT_BOTTOM;
						rcTextLocation.OffsetRect(
							0,
							rectCaption.Height() - rcTextLocation.Height()
							);
					}
					else if( (_ppbd.m_nTextAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_TOP )
					{
						nDtDrawFlags |= DT_TOP;
					}
					else
					{
						nDtDrawFlags |= DT_VCENTER;
						rcTextLocation.OffsetRect(
							0,
							( rectCaption.Height() - rcTextLocation.Height() ) / 2
							);
					}

					if( _ppbd.m_bWordBreak )
						nDtDrawFlags |= DT_WORDBREAK;
					else
						nDtDrawFlags |= DT_SINGLELINE;

					if( _ppbd.m_bPushed )
					{
						CSize szPushedOffset = GetPushedOffset();
						rcTextLocation.OffsetRect( szPushedOffset );
					}

					if( _ppbd.m_bForceUseDrawTextApiH )
					{
						CExtRichContentLayout::stat_DrawText(
							CExtRichContentLayout::__ELFMT_AUTO_DETECT, eLO,
							dc.m_hDC, LPCTSTR(sTitle), &rcTextLocation, nDtDrawFlags, 0
							);
					} // if( _ppbd.m_bForceUseDrawTextApiH )
					else
					{
						if(		_ppbd.m_bEnabled 
							||	_ppbd.m_clrForceTextDisabled != ((COLORREF)-1L)
							)
							CExtRichContentLayout::stat_DrawText(
								CExtRichContentLayout::__ELFMT_AUTO_DETECT, eLO,
								dc.m_hDC, LPCTSTR(sTitle), &rcTextLocation, nDtDrawFlags, 0
								);
						else
						{
							CRect rcDrawTextOffs = rcTextLocation;
							rcDrawTextOffs.OffsetRect( 1, 1 );
							COLORREF clrTextRestore = dc.SetTextColor( GetColor( COLOR_3DHILIGHT, _ppbd.m_pHelperSrc, _ppbd.m_lParam ) );
							CExtRichContentLayout::stat_DrawText(
								CExtRichContentLayout::__ELFMT_AUTO_DETECT, eLO,
								dc.m_hDC, LPCTSTR(sTitle), &rcDrawTextOffs, nDtDrawFlags, 0
								);
							dc.SetTextColor( GetColor( COLOR_3DSHADOW, _ppbd.m_pHelperSrc, _ppbd.m_lParam ) );
							CExtRichContentLayout::stat_DrawText(
								CExtRichContentLayout::__ELFMT_AUTO_DETECT, eLO,
								dc.m_hDC, LPCTSTR(sTitle), rcTextLocation, nDtDrawFlags, 0
								);
							dc.SetTextColor( clrTextRestore );
						} // else from if( _ppbd.m_bEnabled )
					} // else from if( _ppbd.m_bForceUseDrawTextApiH )
					dc.SelectObject( pOldFont );
				} // if( pCurrFont->GetSafeHandle() != NULL )
			} // if skinned color is not hollow
		} // if( !sTitle.IsEmpty() )
	
	} // if we have valid area for text

	if(		_ppbd.m_bDrawFocusRect 
		&&	( ( !sTitle.IsEmpty() ) || _ppbd.m_nFocusMode == __EBFM_CLIENT_AREA )
		&&	rcFocus.bottom > rcFocus.top 
		&&	rcFocus.right > rcFocus.left
		)
	{
		rcFocus.DeflateRect( 2, 2 );
		dc.DrawFocusRect( &rcFocus );
	}

	dc.SetBkMode( nOldBkMode );
	dc.SetBkColor( clrOldBk );
	dc.SetTextColor( clrOldText );
}

bool CExtPaintManagerSkin::GetCb2DbTransparentMode(
	CObject * pObjHelperSrc,
	LPARAM lParam /*= 0L*/
	)
{
	pObjHelperSrc;
	lParam;
	return true;
}

bool CExtPaintManagerSkin::PaintDockerBkgnd(
	bool bClientMapping,
	CDC & dc,
	CWnd * pWnd,
	LPARAM lParam // = NULL
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pWnd->GetSafeHwnd() != NULL );
	
	if( RenderCustomBackground(
			bClientMapping,
			dc,
			pWnd,
			lParam
			)
		)
		return true;
	
	if(	! GetCb2DbTransparentMode( pWnd ) )
		return false;

CRect rcBarSpecificShade( 0, 0, 0, 0 );
CExtToolControlBar * pToolBar = NULL;

#ifndef __EXT_MFC_NO_STATUSBAR
CExtStatusControlBar * pStatusBar = NULL;
#endif // ifndef __EXT_MFC_NO_STATUSBAR

CWnd * pFrame = pWnd;
	for(	;
		pFrame != NULL;
		pFrame = pFrame->GetParent()
		)
	{
		if( (pFrame->GetStyle() & WS_CHILD) == 0 )
		{
			// (- 2.25)
			//	if(! pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
			//		return false;
			break;
		}
		else // frame features - design mode
			if( pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
			{
				CMDIChildWnd * pWndMdiChild =
					DYNAMIC_DOWNCAST( CMDIChildWnd, pFrame );
				if( pWndMdiChild != NULL )
				{
					CMDIFrameWnd * pWndMdiFrame =
						STATIC_DOWNCAST( CMDIFrameWnd, pFrame->GetParentFrame() );
					BOOL bMax = FALSE;
					pWndMdiFrame->MDIGetActive( &bMax );
					if( bMax )
						pFrame = pWndMdiFrame;
				} // if( pWndMdiChild != NULL )
				break;
			}
		if( pFrame->IsKindOf( RUNTIME_CLASS(CExtToolControlBar) ) )
		{
			if( ((CExtToolControlBar*)pFrame)->m_pDockSite != NULL )
			{
//				if(		(!(	pFrame->IsKindOf( RUNTIME_CLASS(CExtPanelControlBar) )
//					||	pFrame->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) )
//					) )
//					&&	(! ((CExtToolControlBar*)pFrame)->m_bForceNoBalloonWhenRedockable)
//					)
				{
					if( ((CExtToolControlBar*)pFrame)->m_pDockBar == NULL )
						continue;
					if( pToolBar == NULL )
					{
						pToolBar = (CExtToolControlBar*)pFrame;
						pFrame->GetWindowRect( &rcBarSpecificShade );
					}
				}
			}
			//break;
		}
#ifndef __EXT_MFC_NO_STATUSBAR
		else
		if( pFrame->IsKindOf( RUNTIME_CLASS(CExtStatusControlBar) ) )
		{
			if( pStatusBar == NULL )
			{
				pStatusBar = (CExtStatusControlBar*)pFrame;
				pFrame->GetWindowRect( &rcBarSpecificShade );
			}
			//break;
		}
#endif // ifndef __EXT_MFC_NO_STATUSBAR
	}
	if( pFrame == NULL )
		return false;

//	CExtToolControlBar::FtLayoutQuery _FtLayoutQuery;
//	if( _FtLayoutQuery.Query( pWnd->m_hWnd ) )
//	{
//		if( bClientMapping )
//			_FtLayoutQuery.ReMapRectsToTargetClient();
//		else
//			_FtLayoutQuery.ReMapRectsToTargetWindow();
//		_FtLayoutQuery.DrawRects( dc );
//		return true;
//	} // if( _FtLayoutQuery.Query( pWnd->m_hWnd ) )

CRect rcPaintGradient;
//	pFrame->GetClientRect( &rcPaintGradient );
//	pFrame->ClientToScreen( &rcPaintGradient );
	pFrame->GetWindowRect( &rcPaintGradient );

	if( bClientMapping )
	{
		pWnd->ScreenToClient( &rcPaintGradient );
		pWnd->ScreenToClient( &rcBarSpecificShade );
	} // if( bClientMapping )
	else
	{
		CRect rc;
		pWnd->GetWindowRect( &rc );
		rcPaintGradient.OffsetRect( -rc.left, -rc.top );
		rcBarSpecificShade.OffsetRect( -rc.left, -rc.top );
	} // else from if( bClientMapping )

	if(		::IsRectEmpty( &rcPaintGradient )
		||	( ! dc.RectVisible( &rcPaintGradient ) )
		)
		return true;

//	if( clrSolidShade0 != ((COLORREF)(-1L)) )
//	{
//		if( clrSolidShade1 != ((COLORREF)(-1L)) )
//		{
//			bool bRTL = ( (dc.GetLayout()&LAYOUT_RTL) != 0 ) ? true : false;
//			if( bRTL )
//			{
//				COLORREF clrTmp = clrSolidShade0;
//				clrSolidShade0 = clrSolidShade1;
//				clrSolidShade1 = clrTmp;
//			} // if( bRTL )
//			stat_PaintGradientRect(
//				dc,
//				rcPaintGradient,
//				clrSolidShade0,
//				clrSolidShade1,
//				true
//				);
//		} // if( clrSolidShade1 != ((COLORREF)(-1L)) )
//		else
//			dc.FillSolidRect( rcPaintGradient, clrSolidShade0 );
//		return true;
//	} // if( clrSolidShade0 != ((COLORREF)(-1L)) )

	CExtSkinGlyph & BkColorGlyph = m_Skin[_T("Background")].Glyph( _T("Background") );
	BkColorGlyph.Draw( dc.m_hDC, rcPaintGradient );

	if( pToolBar != NULL )
	{
//		if( OnQueryUseThemeColors() )
//		{
//			// if use WinXP themed colors
//			COLORREF clrFillHint;
//			OnQueryThemeColors( &clrFillHint );
//			COLORREF clrLeft	= stat_HLS_Adjust( clrFillHint,  0.00, 0.77, 0.15 );
//			COLORREF clrMiddle	= stat_HLS_Adjust( clrFillHint,	 0.00, 0.58, 0.15 );
//			COLORREF clrRight	= stat_HLS_Adjust( clrFillHint,	 0.00,-0.25,-0.25 );
//			if( OnQuerySystemTheme() == ThemeLunaSilver )
//				clrRight = stat_HLS_Adjust( clrRight, 0.05, 0.00, 0.05 );
//			stat_PaintGradientRect2steps( 
//				dc, 
//				rcPaintGradient, 
//				clrLeft, 
//				clrMiddle, 
//				clrRight,
//				bToolbarHorz 
// 				);
//		} // if( OnQueryUseThemeColors() )
//		else
//		{
//			COLORREF clrLeft = 
//				bToolbarHorz 
//					? GetColor( _2003CLR_MLA_NORM_RIGHT, pWnd, lParam )
//					: GetColor( _2003CLR_MLA_NORM_LEFT, pWnd, lParam )
//					;
//			COLORREF clrRight = 
//				bToolbarHorz 
//					? GetColor( _2003CLR_MLA_NORM_LEFT, pWnd, lParam )
//					: GetColor( _2003CLR_MLA_NORM_RIGHT, pWnd, lParam )
//					;
//			stat_PaintGradientRect( 
//				dc,
//				rcPaintGradient,
//				clrLeft,
//				clrRight,
//				bToolbarHorz 
//				);
//		} // else from if( OnQueryUseThemeColors() )
		ASSERT_VALID( pToolBar );
		if( pToolBar->IsFloating() )
		{
			// EG - TO DO - PAINT FLOATING TOOLBAR'S CLIENT AREA
		}
		else
		{
//			LPCTSTR strBarType = _T("ToolBar");
//			if( pToolBar->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
//				strBarType = _T("MenuBar");
//
//			LPCTSTR strDockingType;
//			if( pToolBar->IsDockedVertically() )
//				strDockingType = _T("Vertical");
//			else
//				strDockingType = _T("Horizontal");
//			
//			LPCTSTR strDockingAbility;
//			if( pToolBar->m_pDockSite == NULL )
//				strDockingAbility = _T("Static");
//			else
//				strDockingAbility = _T("Dockable");
//			
//			CExtSkinGlyph & ToolBarBk =
//				m_Skin[_T("Background")][_T("ControlBar")][strBarType][strDockingType][strDockingAbility]
//					.Glyph( _T("Background") );
//
//			ToolBarBk.Draw( dc.m_hDC, rcBarSpecificShade );
			_GetDockedToolBarGlyph( pToolBar ).Draw( dc.m_hDC, rcBarSpecificShade ); 
		} // if( pToolBar->IsFloating() )
	} // if( pToolBar != NULL )
#ifndef __EXT_MFC_NO_STATUSBAR
	else if( pStatusBar != NULL )
	{
		// EG - TO DO - PAINT STATUS BAR ENTIRE BACKGROUND !!! ALREADY IMPLEMENTED
		CExtSkinGlyph & StatusBarBk =
			m_Skin[_T("Background")][_T("ControlBar")][_T("StatusBar")][_T("EntireBackground")]
				.Glyph( _T("Background") );
		StatusBarBk.Draw( dc.m_hDC, rcBarSpecificShade );
	} // if( pStatusBar != NULL )
#endif // ifndef __EXT_MFC_NO_STATUSBAR
	else
	{
//		CRect rcWnd;
//		pWnd->GetClientRect( &rcWnd );
//		pFrame->ClientToScreen( &rcWnd );
//		PaintDockerBkgnd( 
//			bClientMapping,
//			dc,  
//			rcPaintGradient,
//			rcWnd
//			);

	} // else from ...

	return true;
}

void CExtPaintManagerSkin::PaintControlBarBorders(
	CDC & dc,
	CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	_pcbbd;
	dc;

//	if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )
//	{
//		COLORREF clr = GetColor( _2003CLR_STATUSBAR_ITEM, _pcbbd.m_pHelperSrc, _pcbbd.m_lParam );
//		dc.Draw3dRect( &_pcbbd.m_rc, clr, clr );
//		return;
//	} // if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )
//	CExtPaintManagerXP::PaintControlBarBorders( dc, _pcbbd );
}

void CExtPaintManagerSkin::PaintDockBarClientArea(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if(		::IsRectEmpty( &rcClient )
		|| ( ! dc.RectVisible( &rcClient ) )
		)
		return;
	CRect rcPaintGradient( 0, 0, 0, 0 );
	CExtDockOuterBar * pOuterDocker = NULL;
	if( pHelperSrc != NULL )
	{
		pOuterDocker =
			DYNAMIC_DOWNCAST(
			CExtDockOuterBar,
			pHelperSrc
			);
		if( pOuterDocker != NULL )
		{
			CFrameWnd * pFrame =
				pOuterDocker->GetParentFrame();
			ASSERT_VALID( pFrame );
			pFrame->GetClientRect( &rcPaintGradient );
			pFrame->ClientToScreen( &rcPaintGradient );
			pOuterDocker->ScreenToClient( &rcPaintGradient );
			if(		::IsRectEmpty( &rcPaintGradient )
				|| ( ! dc.RectVisible( &rcPaintGradient ) )
				)
				return;
		}
	}
//	if(		pOuterDocker == NULL
//		||	dc.GetDeviceCaps( BITSPIXEL ) <= 8
//		)
//	{
//		CExtPaintManagerXP::PaintDockBarClientArea(
//			dc,
//			rcClient,
//			pHelperSrc,
//			lParam
//			);
//		return;
//	}
	if( RenderCustomBackground(
			true,
			dc,
			pHelperSrc,
			lParam
			)
		)
		return;
	if( pHelperSrc != NULL )
	{
		CWnd * pWnd = DYNAMIC_DOWNCAST( CWnd, pHelperSrc );
		if(		pWnd != NULL
			&&	pWnd->GetSafeHwnd() != NULL
			&&	PaintDockerBkgnd(
					true,
					dc,
					pWnd,
					lParam
					)
			)
			return;
	}
	CExtPaintManager::PaintDockBarClientArea(
		dc,
		rcClient,
		pHelperSrc,
		lParam
		);
}

void CExtPaintManagerSkin::PaintDockingFrame(
	CDC & dc,
	CExtPaintManager::PAINTDOCKINGFRAMEDATA & _pdfd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

CExtMiniDockFrameWnd * pMiniFrame =
		DYNAMIC_DOWNCAST(
			CExtMiniDockFrameWnd,
			_pdfd.m_pHelperSrc
			);

	if( pMiniFrame != NULL )
	{
		CExtSkinGlyph & BkColorGlyph = 
			m_Skin[_T("Background")][_T("Frame")][_T("MiniFrame")].Glyph( _T("Background") );
		BkColorGlyph.Draw( dc.m_hDC, _pdfd.m_rcWindow );
 
		LPCTSTR strBarType = _T("ResizableBar");
	
		CExtControlBar *pBar = NULL;
		CWnd * pWnd = pMiniFrame->GetWindow( GW_CHILD );
		if( pWnd )
		{
			pWnd = pWnd->GetWindow( GW_CHILD );
			pBar = DYNAMIC_DOWNCAST( CExtControlBar, pWnd );
			if( pWnd && pWnd->IsKindOf( RUNTIME_CLASS( CExtToolControlBar ) ) )
			{
				if( pWnd->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
					strBarType = _T("MenuBar");
				else
					strBarType = _T("ToolBar");
			}
		}
		
		LPCTSTR strActiveBarState = _T("Active");
		if( pBar && !pBar->IsBarWindowActive() )
			strActiveBarState = _T("Inactive");
		
		CExtSkinItem & Frame = 
			m_Skin[_T("Frame")][_T("MiniFrame")][_T("ControlBar")][strBarType][strActiveBarState];

		CExtSkinItem & Caption =
			m_Skin[_T("Caption")][_T("Frame")][_T("MiniFrame")][_T("ControlBar")][strBarType][strActiveBarState];

		CExtSkinGlyph & FrameGlyph = Frame.Glyph( _T("Frame") );
		CExtSkinGlyph & CaptionGlyph = Caption.Glyph( _T("Caption") );

		FrameGlyph.Draw( dc.m_hDC, _pdfd.m_rcWindow );

		CRect rcCaption;
		VERIFY(
			Bar_MiniDockFrameCaptionCalcRect(
				rcCaption,
				pMiniFrame,
				pBar
				)
			);
		
		COLORREF clrColorize = COLORREF(-1L);
		bool bFlashCaptionHighlightedState = false;
		if(		_pdfd.m_pHelperSrc != NULL
			&&	_pdfd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtMiniDockFrameWnd) )
			)
		{
			CExtControlBar * pBar = ((CExtMiniDockFrameWnd*)_pdfd.m_pHelperSrc)->FindSingleVisbleFlatingBar();
			if(		pBar != NULL
				&&	pBar->FlashCaptionIsInProgress( &bFlashCaptionHighlightedState )
				)
			{
				if( bFlashCaptionHighlightedState )
					clrColorize = pBar->m_clrFlashCaptionBackground;
			}
		}
		
		if( ! CaptionGlyph.Draw( dc.m_hDC, rcCaption, false, BYTE(0xFF), clrColorize ) )
		{
			if( clrColorize != COLORREF(-1L) )
				dc.FillSolidRect( &rcCaption, clrColorize );
		}

		return;
	} // if( pMiniFrame != NULL )

CExtToolControlBar * pToolBar =
		DYNAMIC_DOWNCAST(
			CExtToolControlBar,
			_pdfd.m_pHelperSrc
			);
	if( pToolBar != NULL )
	{
		LPCTSTR strBarType = _T("ToolBar");
		if( pToolBar->IsKindOf( RUNTIME_CLASS(CExtMenuControlBar) ) )
			strBarType = _T("MenuBar");

		CExtSkinItem & Frame = 
			m_Skin[_T("Frame")][_T("ToolControlBar")][strBarType];

		CExtSkinGlyph & FrameGlyph = Frame.Glyph( _T("Frame") );
		FrameGlyph.Draw( dc.m_hDC, _pdfd.m_rcWindow );
	} // if( pToolBar != NULL )
}

void CExtPaintManagerSkin::PaintControlBarClientArea(
	CDC & dc,
	const RECT & rcClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if(		::IsRectEmpty( &rcClient )
		||	( ! dc.RectVisible( &rcClient ) )
		)
		return;
	if( pHelperSrc != NULL )
	{
		ASSERT_VALID( pHelperSrc );
		CExtToolControlBar * pToolBar =
			DYNAMIC_DOWNCAST(
				CExtToolControlBar,
				pHelperSrc
				);
		if( pToolBar != NULL )
		{
			if(		( ((CExtToolControlBar*)pHelperSrc)->m_bForceNoBalloonWhenRedockable )
				&&	RenderCustomBackground(
						true,
						dc,
						pHelperSrc,
						lParam
						)
				)
				return;
			
			PaintDockerBkgnd(
				true,
				dc,
				pToolBar
				);
			
			LPCTSTR strBarType = _T("");
			if( pToolBar->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
				strBarType = _T("MenuBar");
			else
			if( pToolBar->IsKindOf( RUNTIME_CLASS( CExtStatusControlBar ) ) )
				strBarType = _T("SatatusBar");
			else
				strBarType = _T("ToolBar");

			CExtSkinItem & BkBase = 
				m_Skin[_T("Background")][_T("ClientArea")][_T("ControlBar")][strBarType];

			if( pToolBar->IsFloating() )
			{
				CExtSkinGlyph & BackgroundGlyph = BkBase[_T("Floating")].Glyph( _T("Background") );
				BackgroundGlyph.Draw( dc.m_hDC, rcClient );
			}
			else
			{
				LPCTSTR strDockingType = _T("Horizontal");
				if( pToolBar->IsDockedVertically() )
					strDockingType = _T("Vertical");

				LPCTSTR strDockingAbility = _T("Dockable");
				if( pToolBar->m_pDockSite == NULL )
					strDockingAbility = _T("Static");

				CExtSkinGlyph & BackgroundGlyph 
					= BkBase[strDockingType][strDockingAbility].Glyph( _T("Background") );
				BackgroundGlyph.Draw( dc.m_hDC, rcClient );
			}
			
			return;
		} // if( pToolBar != NULL )

		CWnd * pWnd = DYNAMIC_DOWNCAST( CWnd, pHelperSrc );
		if(		pWnd != NULL
			&&	pWnd->GetSafeHwnd() != NULL
			&&	PaintDockerBkgnd(
				true,
				dc,
				pWnd,
				lParam
				)
			)
			return;
	} // if( pHelperSrc != NULL )
	CExtPaintManager::PaintControlBarClientArea(
		dc,
		rcClient,
		pHelperSrc,
		lParam
		);
}

void CExtPaintManagerSkin::PaintMenuItem(
		CDC & dc,
		CExtPaintManager::PAINTMENUITEMDATA & _pmid
		)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( _pmid.m_rcItem.IsRectEmpty() )
		return;
	if( IsHighContrast() )
	{
		CExtPaintManager::PaintMenuItem(
			dc,
			_pmid
			);
		return;
	}

bool bRTL = false;
	if( _pmid.m_pHelperSrc != NULL )
	{
		ASSERT_VALID( _pmid.m_pHelperSrc );
		CExtPopupMenuWnd * pPopup =
			DYNAMIC_DOWNCAST( CExtPopupMenuWnd, _pmid.m_pHelperSrc );
		if( pPopup != NULL )
			bRTL = pPopup->OnQueryLayoutRTL();
	} // if( _pmid.m_pHelperSrc != NULL )
CExtSafeString _sText =
		( _pmid.m_sText == NULL ) ? _T("") : _pmid.m_sText;
CExtSafeString _sAccelText =
		( _pmid.m_sAccelText == NULL ) ? _T("") : _pmid.m_sAccelText;

//COLORREF clr3dFace = GetColor( CLR_3DFACE_OUT, _pmid.m_pHelperSrc, _pmid.m_lParam );
//COLORREF clrText = GetColor( CLR_TEXT_OUT, _pmid.m_pHelperSrc, _pmid.m_lParam );
//COLORREF clrBk = clr3dFace;
	
CRect rcBk( _pmid.m_rcItemWithIndents );
CRect rcBkWithIndents( _pmid.m_rcItemWithIndents );
CRect rcDDArea( _pmid.m_rcItemWithIndents );
	
CRect rcIconArea( _pmid.m_rcItemWithIndents );
	if( bRTL )
		rcIconArea.left = rcIconArea.right - _pmid.m_nIconAreaWidth;
	else
		rcIconArea.right = rcIconArea.left + _pmid.m_nIconAreaWidth;

LPCTSTR strMenuItemType = _T("MenuItem");
	if( _pmid.m_bSeparatedDropDown )
		strMenuItemType = _T("SeparatedDropDownMenuItem");
	else if( _pmid.m_bPopup )
		strMenuItemType = _T("PopupMenuItem");

CExtSkinItem & SkinItemMenuItem = m_Skin[_T("MenuItem")][strMenuItemType];

LPCTSTR strSkinItemType = _T("");
	if( !_pmid.m_bEnabled )
		strSkinItemType = _T("Disabled");
	else if( _pmid.m_bSelected )
		strSkinItemType = _T("Selected");
	else
		strSkinItemType = _T("Normal");

CExtSkinItem & SkinItem = 
		_pmid.m_bRarelyUsed 
			? SkinItemMenuItem[strSkinItemType][_T("RarelyUsed")]
			: SkinItemMenuItem[strSkinItemType];

CExtSkinGlyph & IconAreaGlyph = SkinItem.Glyph( _T("IconArea") );
	IconAreaGlyph.Draw( dc.m_hDC, rcIconArea );

	if( bRTL )
		rcBk.right -= _pmid.m_nIconAreaWidth;
	else
		rcBk.left += _pmid.m_nIconAreaWidth;

CRect rcMenuItemRect( rcBk );
	if( _pmid.m_bPopup )
	{
		if( bRTL )
		{
			rcDDArea.right = rcDDArea.left + __EXT_MENU_POPUP_ARROW_AREA_DX;
			rcDDArea.right += __EXT_MENU_GAP;
			if( _pmid.m_bSeparatedDropDown )
				rcMenuItemRect.left = rcDDArea.right;
		}
		else
		{
            rcDDArea.left = rcDDArea.right - __EXT_MENU_POPUP_ARROW_AREA_DX;
			rcDDArea.left -= __EXT_MENU_GAP;
			if( _pmid.m_bSeparatedDropDown )
				rcMenuItemRect.right = rcDDArea.left;
		}
	}

CExtSkinGlyph & MenuItemGlyph = SkinItem.Glyph( _T("MenuItem") );
	MenuItemGlyph.Draw( dc.m_hDC, rcMenuItemRect );

//	if( _pmid.m_bSelected )
//	{
//		ASSERT( _pmid.m_bEnabled );
//		clrBk = GetColor( COLOR_HIGHLIGHT, _pmid.m_pHelperSrc, _pmid.m_lParam );
//		clrText = GetColor( COLOR_MENUTEXT, _pmid.m_pHelperSrc, _pmid.m_lParam  );
//	}

COLORREF clrText = COLORREF( SkinItem.Color( _T("TextColor") ) );

	rcBk.DeflateRect( __EXT_MENU_GAP, __EXT_MENU_GAP );
COLORREF clrOldText = dc.SetTextColor( clrText );
int nOldBkMode = dc.SetBkMode( TRANSPARENT );

	rcBk.DeflateRect( __EXT_MENU_GAP, 0 );
CFont * pRenderFont =
		( _pmid.m_hCustomFont == NULL )
			? (&m_FontNormal)
			: CFont::FromHandle(_pmid.m_hCustomFont)
			;
CFont * pOldFont = dc.SelectObject( pRenderFont );
	ASSERT( pOldFont != NULL );

DWORD dwDtAlignFlags = bRTL ? (DT_RIGHT|DT_RTLREADING) : DT_LEFT;
CRect rcMeasureText( 0, 0, 0, 0 );
	CExtRichContentLayout::stat_DrawText(
		dc.m_hDC,
		LPCTSTR(_sText), _sText.GetLength(),
		&rcMeasureText,
		DT_CALCRECT|DT_SINGLELINE|dwDtAlignFlags, 0
		);
CRect rcDrawText( rcBk ), rcDrawTextExtended( rcBk );
INT nLenTextExtended = ( _pmid.m_sExtendedText != NULL ) ? INT( _tcslen( _pmid.m_sExtendedText ) ) : 0;
	if( nLenTextExtended > 0 )
	{
		rcDrawText.bottom
			= rcDrawTextExtended.top
			= rcDrawText.top + rcMeasureText.Height();
	}
	else
	{
		rcDrawText.OffsetRect(
			0,
			( rcDrawText.Height() - rcMeasureText.Height() ) / 2
			);
	}

	if( bRTL )
		rcDrawText.left = rcDrawText.right - rcMeasureText.Width();
	CExtRichContentLayout::stat_DrawText(
		dc.m_hDC,
		LPCTSTR(_sText), _sText.GetLength(),
		&rcDrawText,
		DT_SINGLELINE|dwDtAlignFlags, 0
		);
	if( nLenTextExtended > 0 )
	{
		CFont * pOldFont = dc.SelectObject( &m_FontNormal );
		CExtRichContentLayout::stat_DrawText(
			dc.m_hDC,
			_pmid.m_sExtendedText, nLenTextExtended,
			&rcDrawTextExtended,
			dwDtAlignFlags, 0
			);
		dc.SelectObject( pOldFont );
	}

	if( ! _sAccelText.IsEmpty() )
	{
		rcMeasureText.SetRect( 0, 0, 0, 0 );
		CExtRichContentLayout::stat_DrawText(
			dc.m_hDC,
			LPCTSTR(_sAccelText), _sAccelText.GetLength(),
			&rcMeasureText,
			DT_CALCRECT|DT_SINGLELINE|dwDtAlignFlags, 0
			);
		rcDrawText = rcBk;
		rcDrawText.OffsetRect(
			0,
			( rcDrawText.Height() - rcMeasureText.Height() ) / 2
			);
		if( bRTL )
		{
			rcDrawText.right =
				rcDrawText.left
				+ rcMeasureText.Width()
				+ __EXT_MENU_POPUP_ARROW_AREA_DX
				;
			rcBk.right = rcDrawText.right;
		} // if( bRTL )
		else
		{
			rcDrawText.left =
				rcDrawText.right
				- rcMeasureText.Width()
				- __EXT_MENU_POPUP_ARROW_AREA_DX
				;
			rcBk.left = rcDrawText.left;
		} // else from if( bRTL )
		if( bRTL )
		{
			rcDrawText.left = rcDrawText.right - rcMeasureText.Width();
			if( _pmid.m_bEnabled )
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					LPCTSTR(_sAccelText), _sAccelText.GetLength(),
					&rcDrawText,
					DT_SINGLELINE|dwDtAlignFlags, 0
					);
			else
			{
				CRect rcDrawTextOffs = rcDrawText;
				rcDrawTextOffs.OffsetRect( 1, 1 );
				COLORREF clrTextRestore =
					dc.SetTextColor(
						GetColor(
							COLOR_3DHILIGHT,
							_pmid.m_pHelperSrc,
							_pmid.m_lParam
							)
						);
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					LPCTSTR(_sAccelText), _sAccelText.GetLength(),
					&rcDrawTextOffs,
					DT_SINGLELINE|dwDtAlignFlags, 0
					);
				dc.SetTextColor(
					GetColor(
					COLOR_3DSHADOW,
					_pmid.m_pHelperSrc,
					_pmid.m_lParam
					)
					);
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					LPCTSTR(_sAccelText), _sAccelText.GetLength(),
					&rcDrawText,
					DT_SINGLELINE|dwDtAlignFlags, 0
					);
				dc.SetTextColor( clrTextRestore );
			} // else from if( _pmid.m_bEnabled )
		} // if( bRTL )
		else
			dc.DrawState(
			rcDrawText.TopLeft(),
			rcDrawText.Size(),
			_sAccelText,
			(_pmid.m_bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
			TRUE,
			0,
			(CBrush*)NULL
			);
	} // if( ! _sAccelText.IsEmpty() )

	if( _pmid.m_bPopup )
	{
		CRect rcPopupArrow( _pmid.m_rcItem );
		rcPopupArrow.DeflateRect( __EXT_MENU_GAP, __EXT_MENU_GAP );
		if( bRTL )
			rcPopupArrow.right =
			rcPopupArrow.left + __EXT_MENU_POPUP_ARROW_AREA_DX;
		else
			rcPopupArrow.left =
			rcPopupArrow.right - __EXT_MENU_POPUP_ARROW_AREA_DX;

		if( _pmid.m_bCustomizeListMode )
		{
			COLORREF ColorValues[2] =
			{
				RGB(0,0,0),
					GetColor(
					_pmid.m_bSelected ? CLR_MENUTEXT_IN : CLR_MENUTEXT_OUT,
					_pmid.m_pHelperSrc,
					_pmid.m_lParam 
					)
			};
			PaintGlyphCentered(
				dc,
				rcPopupArrow,
				_pmid.m_bSeparatedDropDown
				? g_glyph_customize_dropdown_src
				: g_glyph_customize_popup_src,
				ColorValues
				);
		} // if( _pmid.m_bCustomizeListMode )
		else
		{
			if( _pmid.m_bSeparatedDropDown )
			{
				CExtSkinGlyph & DropDownArea = SkinItem.Glyph( _T("DropDownArea") );
				DropDownArea.Draw( dc.m_hDC, rcDDArea );
			} 

			CExtSkinGlyph & DropDownArrow = SkinItem.Glyph( _T("DropDownArrow") );
			DropDownArrow.Draw( dc.m_hDC, rcDDArea );
		} // else from if( _pmid.m_bCustomizeListMode )
	} // if( _pmid.m_bPopup )

	dc.SelectObject( pOldFont );
	dc.SetBkMode( nOldBkMode );
	dc.SetTextColor( clrOldText );

CSize sizeIcon( __EXT_MENU_ICON_CX, __EXT_MENU_ICON_CY );
	if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	{
		sizeIcon = _pmid.m_pIcon->GetSize();
		if(	sizeIcon.cx <= 18 && sizeIcon.cy < sizeIcon.cx )
			sizeIcon.cy = sizeIcon.cx;
	}
	
CRect rcIcon( _pmid.m_rcItem.TopLeft(), sizeIcon );
	if( bRTL )
		rcIcon.OffsetRect( _pmid.m_rcItem.Width() - sizeIcon.cx, 0 );
	
	if( _pmid.m_bXtraMark )
	{
		rcIcon.OffsetRect(
			( ( _pmid.m_nIconAreaWidth/2 - rcIcon.Width() ) / 2 + _pmid.m_nIconAreaWidth/2 - 2 )
			* (bRTL ? (-1) : 1),
			( _pmid.m_rcItem.Height() - rcIcon.Height() ) / 2
			);

		if( _pmid.m_bXtraChecked )
		{
			CRect rcXtraMark(
				bRTL ? (_pmid.m_rcItem.right - _pmid.m_rcItem.Height() - 1) : (_pmid.m_rcItem.left + 1),
				_pmid.m_rcItem.top + 1,
				bRTL ? (_pmid.m_rcItem.right - 1) : (_pmid.m_rcItem.left + _pmid.m_rcItem.Height() - 1),
				_pmid.m_rcItem.bottom - 1
				);
			
			CExtSkinGlyph & XtraMarkFrameGlyph = 
				m_Skin[_T("MenuItem")][_T("CheckMark")][strSkinItemType].Glyph( _T("Frame") );
			XtraMarkFrameGlyph.Draw( dc.m_hDC, rcXtraMark );
			
			CExtSkinGlyph & XtraMarkGlyph = 
				m_Skin[_T("MenuItem")][_T("CheckMark")][strSkinItemType].Glyph( _T("CheckMark") );
			XtraMarkGlyph.Draw( dc.m_hDC, rcXtraMark );
		} 
	} // if( _pmid.m_bXtraMark )
	else
	{
		rcIcon.OffsetRect(
			( ( _pmid.m_nIconAreaWidth - rcIcon.Width() ) / 2 - 1) * ( bRTL ? (-1) : 1 ),
			( _pmid.m_rcItem.Height() - rcIcon.Height() ) / 2
			);
	} // else from if( _pmid.m_bXtraMark )

CRect rcIconFrame( rcIcon );
	rcIconFrame.InflateRect(
		__EXT_MENU_IMG_RECT_OUT_GAP,
		__EXT_MENU_IMG_RECT_OUT_GAP
		);

	if( _pmid.m_bPushedCheck || _pmid.m_bIndeterminate || _pmid.m_bPushedRadio )
	{
		if( _pmid.m_bPushedCheck || _pmid.m_bIndeterminate )
		{
			BYTE nScaCheck = BYTE(0x0FF);
			CExtSkinItem * pItem = &(m_Skin[_T("MenuItem")][_T("CheckMark")][strSkinItemType]);
			if( _pmid.m_bIndeterminate )
			{
				CExtSkinItem * pIndeterminate = pItem->NestedGet( _T("Indeterminate") );
				if( pIndeterminate != NULL )
					pItem = pIndeterminate;
				else
					nScaCheck = BYTE(0x080);
			}
			CExtSkinGlyph & MarkFrameGlyph = 
				pItem->Glyph( _T("Frame") );
			MarkFrameGlyph.Draw( dc.m_hDC, rcIconFrame, false, nScaCheck );
		}
		else
		{
			CExtSkinGlyph & MarkFrameGlyph = 
				m_Skin[_T("MenuItem")][_T("RadioDot")][strSkinItemType].Glyph( _T("Frame") );
			MarkFrameGlyph.Draw( dc.m_hDC, rcIconFrame );
		}

		if( _pmid.m_pIcon == NULL || _pmid.m_pIcon->IsEmpty() )
		{
			if( _pmid.m_bPushedCheck || _pmid.m_bIndeterminate )
			{
//				CExtSkinGlyph & MarkGlyph = 
//					m_Skin[_T("MenuItem")][_T("CheckMark")][strSkinItemType].Glyph( _T("CheckMark") );
				BYTE nScaCheck = BYTE(0x0FF);
				CExtSkinItem * pItem = &(m_Skin[_T("MenuItem")][_T("CheckMark")][strSkinItemType]);
				if( _pmid.m_bIndeterminate )
				{
					CExtSkinItem * pIndeterminate = pItem->NestedGet( _T("Indeterminate") );
					if( pIndeterminate != NULL )
						pItem = pIndeterminate;
					else
						nScaCheck = BYTE(0x080);
				}
				CExtSkinGlyph & MarkGlyph = 
					pItem->Glyph( _T("CheckMark") );

				MarkGlyph.Draw( dc.m_hDC, rcIconFrame, false, nScaCheck );
			}
			else
			{
				CExtSkinGlyph & MarkGlyph = 
					m_Skin[_T("MenuItem")][_T("RadioDot")][strSkinItemType].Glyph( _T("RadioDot") );
				MarkGlyph.Draw( dc.m_hDC, rcIconFrame );
			}
		} // if( _pmid.m_pIcon == NULL || _pmid.m_pIcon->IsEmpty() )
	}

	if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	{
		_pmid.m_pIcon->Paint(
			this,
			dc,
			rcIcon.left,
			rcIcon.top,
			-1,
			-1,
			_pmid.m_bEnabled
				?	(	_pmid.m_bSelected
						?	CExtCmdIcon::__PAINT_HOVER
						:	CExtCmdIcon::__PAINT_NORMAL
					)
				:			CExtCmdIcon::__PAINT_DISABLED
			);
	} // if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	else
	{
		PaintSysCmdGlyph(
			dc,
			_pmid.m_nHelperSysCmdID,
			rcIcon,
			_pmid.m_bSelected,
			false,
			_pmid.m_bEnabled
			);
	} // else from if( _pmid.m_pIcon != NULL && (! _pmid.m_pIcon->IsEmpty()) )
	
	if( ! _pmid.m_rcInPlaceWnd.IsRectEmpty() )
	{
		CRect rcInPlaceWndOuterBorder( _pmid.m_rcInPlaceWnd );
		rcInPlaceWndOuterBorder.InflateRect( 2, 2, 2, 3 );
		INT nDD = (INT)::GetSystemMetrics( SM_CXHTHUMB );
		CRect rcBk( rcInPlaceWndOuterBorder );
		if( _pmid.m_bPopup )
			rcBk.right += nDD;
		dc.FillSolidRect(
			&rcBk,
			GetColor(
			_pmid.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE,
			_pmid.m_pHelperSrc,
			_pmid.m_lParam
			)
			);
		if( _pmid.m_bPopup )
		{
			rcInPlaceWndOuterBorder.right += nDD;
			if( _pmid.m_bEnabled )
			{
				PAINTCOMBOFRAMEDATA _pcfd(
					_pmid.m_pHelperSrc,
					rcInPlaceWndOuterBorder,
					_pmid.m_bSelected, // hover
					_pmid.m_bSelected && _pmid.m_bHelperPopupDropped, // pushed
					_pmid.m_bEnabled
					);
				_pcfd.m_bNoTopOuterBorder = true;
				PaintComboFrame( dc, _pcfd );
			} // if( _pmid.m_bEnabled )
			else
			{
				CRect rcBorder( rcInPlaceWndOuterBorder );
				rcBorder.DeflateRect( 2, 2 );
				COLORREF clrBorder =
					GetColor( COLOR_WINDOW, _pmid.m_pHelperSrc, _pmid.m_lParam );
				dc.Draw3dRect(
					&rcBorder,
					clrBorder,
					clrBorder
					);
				// paint arrow down
				CRect rcBtn( rcBorder );
				rcBtn.left = rcBtn.right - nDD;
				glyph_t * pGlyph = g_DockingCaptionGlyphs[ __DCBT_ARROW_SMALL_DOWN ];
				ASSERT( pGlyph != NULL );
				COLORREF ColorValues[2] =
				{
					RGB(0,0,0),
					GetColor( COLOR_WINDOWTEXT, _pmid.m_pHelperSrc, _pmid.m_lParam )
				};
				PaintGlyphCentered(
					dc,
					rcBtn,
					*pGlyph,
					ColorValues
					);
			} // else from if( _pmid.m_bEnabled )
			rcInPlaceWndOuterBorder.DeflateRect( 1, 1 );
			rcInPlaceWndOuterBorder.right -= nDD;
		} // if( _pmid.m_bPopup )
		else
		{
			if( _pmid.m_bEnabled )
			{
				COLORREF clrFace = GetColor( COLOR_3DFACE, _pmid.m_pHelperSrc, _pmid.m_lParam );
				dc.Draw3dRect(
					rcInPlaceWndOuterBorder,
					clrFace,
					clrFace
					);
				rcInPlaceWndOuterBorder.DeflateRect( 1, 1 );
				if( _pmid.m_bSelected )
					dc.Draw3dRect(
					rcInPlaceWndOuterBorder,
					GetColor( COLOR_3DSHADOW, _pmid.m_pHelperSrc, _pmid.m_lParam ),
					GetColor( COLOR_3DHIGHLIGHT, _pmid.m_pHelperSrc, _pmid.m_lParam )
					);
				else
					dc.Draw3dRect(
					rcInPlaceWndOuterBorder,
					clrFace,
					clrFace
					);
			} // if( _pmid.m_bEnabled )
			else
			{
				CRect rcBorder( rcInPlaceWndOuterBorder );
				rcBorder.DeflateRect( 2, 2 );
				COLORREF clrBorder = GetColor( COLOR_WINDOW, _pmid.m_pHelperSrc, _pmid.m_lParam );
				dc.Draw3dRect(
					&rcBorder,
					clrBorder,
					clrBorder
					);
			} // else from if( _pmid.m_bEnabled )
		} // else from if( _pmid.m_bPopup )
		if( ! _pmid.m_strInPlaceText.IsEmpty() )
		{
			rcInPlaceWndOuterBorder.DeflateRect( 5, 2, 2, 2 );
			COLORREF clrOldText =
				dc.SetTextColor(
				::GetSysColor( _pmid.m_bEnabled ? COLOR_WINDOWTEXT : COLOR_GRAYTEXT )
				);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			CFont * pOldFont = dc.SelectObject( pRenderFont );
			ASSERT( pOldFont != NULL );
			CExtRichContentLayout::stat_DrawText(
				dc.m_hDC,
				LPCTSTR(_pmid.m_strInPlaceText), _pmid.m_strInPlaceText.GetLength(),
				&rcInPlaceWndOuterBorder,
				DT_SINGLELINE|DT_NOPREFIX|DT_TOP|dwDtAlignFlags, 0
				);
			dc.SelectObject( pOldFont );
			dc.SetBkMode( nOldBkMode );
			dc.SetTextColor( clrOldText );
		} // if( _pmid.m_bEnabled )
	} // if( ! _pmid.m_rcInPlaceWnd.IsRectEmpty() )
}

void CExtPaintManagerSkin::PaintMenuSeparator(
	CDC & dc,
	const RECT & rectItem,
	int nIconAreaWidth,
	bool bRarelyUsed,
	bool bIsForceNoLeftGradient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	lParam;
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	nIconAreaWidth;
	bIsForceNoLeftGradient;
	if( ::IsRectEmpty(&rectItem) )
		return;
	
	bool bRTL = false;
	if( pHelperSrc != NULL )
	{
		ASSERT_VALID( pHelperSrc );
		CExtPopupMenuWnd * pPopup =
			DYNAMIC_DOWNCAST( CExtPopupMenuWnd, pHelperSrc );
		if( pPopup != NULL )
			bRTL = pPopup->OnQueryLayoutRTL();
	} // if( pHelperSrc != NULL )

	CRect rcIconArea( rectItem );
	if( bRTL )
		rcIconArea.left = rcIconArea.right - nIconAreaWidth;
	else
		rcIconArea.right = rcIconArea.left + nIconAreaWidth;

	CExtSkinItem & SeparatorItem = 
		bRarelyUsed
		? m_Skin[_T("MenuItem")][_T("Separator")][_T("RarelyUsed")]
		: m_Skin[_T("MenuItem")][_T("Separator")];

	CExtSkinGlyph & IconAreaGlyph = SeparatorItem.Glyph( _T("IconArea") );
	IconAreaGlyph.Draw( dc.m_hDC, rcIconArea );
	
	CRect rectSeparator( rectItem );
	rectSeparator.DeflateRect( 
		rcIconArea.Width(),
		0,
		0,
		1  // conflict with default outer margins
		);

	CExtSkinGlyph & SeparatorGlyph = SeparatorItem.Glyph( _T("Separator") );
	SeparatorGlyph.Draw( dc.m_hDC, rectSeparator );
}

CRect CExtPaintManagerSkin::GetMenuBorderMetrics(
	CWnd * pWnd,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	pWnd;
	lParam;
const CExtSkinGlyph & FrameGlyph = m_Skin[_T("Frame")][_T("Menu")].Glyph( _T("Frame") );
CRect rc = FrameGlyph.GetPadding();
	if(		pWnd != NULL
		&&	pWnd->IsKindOf( RUNTIME_CLASS( CExtPopupMenuWnd ) )
		&&	((CExtPopupMenuWnd*)pWnd)->_IsRibbonMode()
		&&	( ((CExtPopupMenuWnd*)pWnd)->TrackFlagsGet() & TPMX_RIBBON_FILE_MENU ) != 0
		)
		rc.bottom += 28;
	return rc;
}

void CExtPaintManagerSkin::PaintMenuBorder(
		CDC & dc,
		const RECT & rectClient,
		CObject * pHelperSrc,
		LPARAM lParam // = 0L
		)
{
	pHelperSrc;lParam;

	CRect rcClient( rectClient );
CExtSkinGlyph & MenuFrameGlyph = m_Skin[_T("Frame")][_T("Menu")].Glyph( _T("Frame") );
	MenuFrameGlyph.Draw( dc.m_hDC, rcClient );

	rcClient.DeflateRect( 
		MenuFrameGlyph.GetPadding()
		);
CExtSkinGlyph & MenuBkGlyph = m_Skin[_T("Background")][_T("Menu")].Glyph( _T("Background") );
	MenuBkGlyph.Draw( dc.m_hDC, rcClient );
}

void CExtPaintManagerSkin::PaintMenuExpandButton(
	CDC & dc,
	const RECT & rectButton,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bPushed;
	lParam;
	
	bool bRTL = false;
	if( pHelperSrc != NULL )
	{
		ASSERT_VALID( pHelperSrc );
		CExtPopupMenuWnd * pPopup =
			DYNAMIC_DOWNCAST( CExtPopupMenuWnd, pHelperSrc );
		if( pPopup != NULL )
			bRTL = pPopup->OnQueryLayoutRTL();
	}
	
	int nIconAreaWidth = 0;
	if(		pHelperSrc != NULL // (+ 2.23)
		&&	pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) )
		&&	((CExtPopupMenuWnd*)pHelperSrc) ->
		ItemGetCount() > 0
		)
	{
		CExtPopupMenuWnd::MENUITEMDATA & _mii =
			((CExtPopupMenuWnd*)pHelperSrc) ->
			ItemGetInfo(
			((CExtPopupMenuWnd*)pHelperSrc) ->
			ItemGetCount()
			- 1
			);
		if( ! _mii.IsToolButton() )
			nIconAreaWidth = _mii.GetIconAreaWidth();
	}

	CRect rcButton( rectButton );

LPCTSTR strExpandButtonType = _T("");
	if( bHover )
		strExpandButtonType = _T("Selected");
	else	
		strExpandButtonType = _T("Normal");

	CExtSkinItem & ExpandButtonSkinItem = m_Skin[_T("MenuItem")][_T("ExpandButton")][strExpandButtonType];

	CRect rcIconArea( rcButton );
	if( bRTL )
	{
		rcIconArea.left = rcIconArea.right - nIconAreaWidth;
		rcButton.right -= nIconAreaWidth;
	}
	else
	{
		rcIconArea.right = rcIconArea.left + nIconAreaWidth;
		rcButton.left += nIconAreaWidth;
	}

	CExtSkinGlyph & IconAreaGlyph = ExpandButtonSkinItem.Glyph( _T("IconArea") );
	IconAreaGlyph.Draw( dc.m_hDC, rcIconArea );
	
	CExtSkinGlyph & MenuItemGlyph = ExpandButtonSkinItem.Glyph( _T("MenuItem") );
	MenuItemGlyph.Draw( dc.m_hDC, rcButton );

	CExtSkinGlyph & ExpandButtonGlyph = ExpandButtonSkinItem.Glyph( _T("ExpandButton") );
	ExpandButtonGlyph.Draw( dc.m_hDC, rectButton );
}

void CExtPaintManagerSkin::PaintMenuScrollButton(
	CDC & dc,
	bool bHover,
	bool bPushed,
	bool bTopButton,
	const RECT & rcScrollButton,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bPushed;
	lParam;

	bool bRTL = false;
	if( pHelperSrc != NULL )
	{
		ASSERT_VALID( pHelperSrc );
		CExtPopupMenuWnd * pPopup =
			DYNAMIC_DOWNCAST( CExtPopupMenuWnd, pHelperSrc );
		if( pPopup != NULL )
			bRTL = pPopup->OnQueryLayoutRTL();
	}

	int nIconAreaWidth = 0;
	if(		pHelperSrc != NULL // (+ 2.23)
		&&	pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) )
		&&	((CExtPopupMenuWnd*)pHelperSrc) ->
		ItemGetCount() > 0
		)
	{
		CExtPopupMenuWnd::MENUITEMDATA & _mii =
			((CExtPopupMenuWnd*)pHelperSrc) ->
			ItemGetInfo(
			((CExtPopupMenuWnd*)pHelperSrc) ->
			ItemGetCount()
			- 1
			);
		if( ! _mii.IsToolButton() )
			nIconAreaWidth = _mii.GetIconAreaWidth();
	}

	CRect rcButton( rcScrollButton );
	
// Temporary fix
	if( bTopButton )
		rcButton.bottom += 1;
	else
		rcButton.top -= 1;
	rcButton.left -= 1;
	rcButton.right += 1;
// Temporary fix

LPCTSTR strScrollButtonType = _T("");
	if( bHover )
		strScrollButtonType = _T("Selected");
	else	
		strScrollButtonType = _T("Normal");

	CExtSkinItem & ScrollButtonSkinItem = m_Skin[_T("MenuItem")][_T("ScrollButton")][strScrollButtonType];

	CRect rcIconArea( rcButton );
	if( bRTL )
	{
		rcIconArea.left = rcIconArea.right - nIconAreaWidth;
		rcButton.right -= nIconAreaWidth;
	}
	else
	{
		rcIconArea.right = rcIconArea.left + nIconAreaWidth;
		rcButton.left += nIconAreaWidth;
	}

	CExtSkinGlyph & IconAreaGlyph = ScrollButtonSkinItem.Glyph( _T("IconArea") );
	IconAreaGlyph.Draw( dc.m_hDC, rcIconArea );

	CExtSkinGlyph & MenuItemGlyph = ScrollButtonSkinItem.Glyph( _T("MenuItem") );
	MenuItemGlyph.Draw( dc.m_hDC, rcButton );

	CExtSkinGlyph & ScrollButtonGlyph = 
		ScrollButtonSkinItem.Glyph( 
			bTopButton ? _T("ScrollButtonTop") : _T("ScrollButtonBottom") 
		);
	ScrollButtonGlyph.Draw( dc.m_hDC, rcScrollButton );
}

void CExtPaintManagerSkin::PaintMenuTearOffButton(
	CDC & dc,
	const RECT & rectButton,
	bool bPushed,
	bool bHover,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bPushed;
	lParam;

	bool bRTL = false;
	if( pHelperSrc != NULL )
	{
		ASSERT_VALID( pHelperSrc );
		CExtPopupMenuWnd * pPopup =
			DYNAMIC_DOWNCAST( CExtPopupMenuWnd, pHelperSrc );
		if( pPopup != NULL )
			bRTL = pPopup->OnQueryLayoutRTL();
	}

	int nIconAreaWidth = 0;
	if(		pHelperSrc != NULL // (+ 2.23)
		&&	pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) )
		&&	((CExtPopupMenuWnd*)pHelperSrc) ->
		ItemGetCount() > 0
		)
	{
		CExtPopupMenuWnd::MENUITEMDATA & _mii =
			((CExtPopupMenuWnd*)pHelperSrc) ->
			ItemGetInfo(
			((CExtPopupMenuWnd*)pHelperSrc) ->
			ItemGetCount()
			- 1
			);
		if( ! _mii.IsToolButton() )
			nIconAreaWidth = _mii.GetIconAreaWidth();
	}

	CRect rcButton( rectButton );
	rcButton.left -= 1;	// Temporary fix
	rcButton.right += 1;// Temporary fix

LPCTSTR strTearOffButtonType = _T("");
	if( bHover )
		strTearOffButtonType = _T("Selected");
	else	
		strTearOffButtonType = _T("Normal");

	CExtSkinItem & TearOffButtonSkinItem = m_Skin[_T("MenuItem")][_T("TearOffButton")][strTearOffButtonType];

	CRect rcIconArea( rcButton );
	if( bRTL )
	{
		rcIconArea.left = rcIconArea.right - nIconAreaWidth;
		rcButton.right -= nIconAreaWidth;
	}
	else
	{
		rcIconArea.right = rcIconArea.left + nIconAreaWidth;
		rcButton.left += nIconAreaWidth;
	}

	CExtSkinGlyph & IconAreaGlyph = TearOffButtonSkinItem.Glyph( _T("IconArea") );
	IconAreaGlyph.Draw( dc.m_hDC, rcIconArea );

	CExtSkinGlyph & MenuItemGlyph = TearOffButtonSkinItem.Glyph( _T("MenuItem") );
	MenuItemGlyph.Draw( dc.m_hDC, rcButton );

	CExtSkinGlyph & TearOffButtonGlyph = TearOffButtonSkinItem.Glyph( _T("TearOffButton") );
	TearOffButtonGlyph.Draw( dc.m_hDC, rectButton );
}

void CExtPaintManagerSkin::PaintGripper(
		CDC & dc,
		CExtPaintManager::PAINTGRIPPERDATA & _pgd
		)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
CRect rectGripper2( _pgd.m_rcGripper );
	if( _pgd.m_bSideBar || _pgd.m_bFloating )
	{
		if( ! _pgd.m_bFloating )
			rectGripper2.DeflateRect( 1, 1 );

#if (defined COLOR_GRADIENTACTIVECAPTION && defined COLOR_GRADIENTACTIVECAPTION)
		ASSERT( COLOR_GRADIENTACTIVECAPTION == 27 );
		ASSERT( COLOR_GRADIENTINACTIVECAPTION == 28 );
#endif

		LPCTSTR strBarType = _T("");
		if( _pgd.m_bSideBar )
			strBarType = _T("ResizableBar");
		else
		{
			strBarType = _T("ToolBar");
			if( _pgd.m_pHelperSrc )
			{
				CWnd * pWnd = DYNAMIC_DOWNCAST( CWnd, _pgd.m_pHelperSrc );
				if( pWnd )
				{
					pWnd = pWnd->GetWindow( GW_CHILD );
					if( pWnd )
					{
						pWnd = pWnd->GetWindow( GW_CHILD );
						if( pWnd && pWnd->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
							strBarType = _T("MenuBar");
					}
				}
			}
		}

		LPCTSTR strBarState = _T("");
		if( _pgd.m_bActive )
			strBarState = _T("Active");
		else
			strBarState = _T("Inactive");

		CExtSkinItem & Caption = _pgd.m_bFloating 
			? m_Skin[_T("Caption")][_T("ControlBar")][strBarType][_T("Floating")][strBarState]
			: m_Skin[_T("Caption")][_T("ControlBar")][strBarType][strBarState];

		CExtSkinGlyph & CaptionGlyph = Caption.Glyph( _T("Caption") );
		COLORREF clrColorize = COLORREF(-1L);
		if( _pgd.m_bFlashCaptionHighlightedState )
		{
			ASSERT( _pgd.m_clrFlashCaptionBackground != COLORREF(-1L) );
			ASSERT( _pgd.m_clrFlashCaptionText != COLORREF(-1L) );
			clrColorize = _pgd.m_clrFlashCaptionBackground;
		} // if( _pgd.m_bFlashCaptionHighlightedState )
		CaptionGlyph.Draw( dc.m_hDC, _pgd.m_rcGripper, false, BYTE(0xFF), clrColorize );

//		INT nTextColorIndex = COLOR_HIGHLIGHTTEXT;

		int nTextLen = 0;
		if(		_pgd.m_sCaption != NULL
			&&	( nTextLen = int(_tcslen(_pgd.m_sCaption)) ) > 0
			&&	(! _pgd.m_rcText.IsRectEmpty() )
			&&	_pgd.m_rcText.right > _pgd.m_rcText.left
			&&	_pgd.m_rcText.bottom > _pgd.m_rcText.top
			)
		{
//			COLORREF clrText =
//				GetColor(
//					nTextColorIndex,
//					_pgd.m_pHelperSrc,
//					_pgd.m_lParam 
//					);
			CExtSkinColor & CaptionTextColor = Caption.Color( _T("Text") );
			if( ! CaptionTextColor.IsHollow() )
			{
//				COLORREF clrText = CaptionTextColor;
			COLORREF clrText =
				_pgd.m_bFlashCaptionHighlightedState
					? _pgd.m_clrFlashCaptionText
					: CaptionTextColor
					;
				COLORREF clrOldText = dc.SetTextColor( clrText );
				int nOldBkMode = dc.SetBkMode(TRANSPARENT);
				CFont * pCurrFont = ( _pgd.m_bSideBar && (!_pgd.m_bFloating) ) ? ( &m_FontNormalBC ) : ( &m_FontBoldBC   );
				CFont * pOldFont = dc.SelectObject( pCurrFont );
				ASSERT( pOldFont != NULL );
				CRect rcDrawText( _pgd.m_rcText );
				CExtRichContentLayout::e_layout_orientation_t eLO = CExtRichContentLayout::__ELOT_NORMAL;
				if( _pgd.m_bHorz )
					eLO = CExtRichContentLayout::__ELOT_270_CW;
				UINT nDtAlign = _pgd.m_bForceRTL ? DT_RIGHT : DT_LEFT;
				CExtRichContentLayout::stat_DrawText(
					CExtRichContentLayout::__ELFMT_AUTO_DETECT, eLO,
					dc.m_hDC, _pgd.m_sCaption, nTextLen, rcDrawText, nDtAlign|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS, 0
					);
				dc.SelectObject( pOldFont );
				dc.SetBkMode(nOldBkMode);
				dc.SetTextColor(clrOldText);
			} // if( ! CaptionTextColor.IsHollow() )
		}
	} // if( _pgd.m_bSideBar || _pgd.m_bFloating )
	else
	{
		CExtSkinGlyph * pGripperGlyph = NULL;
		_GetDockedToolBarGlyph(
			STATIC_DOWNCAST( CExtToolControlBar, _pgd.m_pHelperSrc ),
			&pGripperGlyph
			);
		ASSERT( pGripperGlyph != NULL );
		pGripperGlyph->Draw( dc.m_hDC, rectGripper2 );
	} // else from if( _pgd.m_bSideBar || _pgd.m_bFloating )
}

void CExtPaintManagerSkin::PaintDockingCaptionButton(
		CDC & dc,
		CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA & _pdcbd
		)
{
LPCTSTR strButtonType = _T("");
	switch( _pdcbd.m_eType )
	{
	case CExtPaintManager::__DCBT_CLOSE:
		strButtonType = _T("Close");
		break;
	case CExtPaintManager::__DCBT_ARROW_DOWN:
		strButtonType = _T("Options");
		break;
	case CExtPaintManager::__DCBT_WND_KEEP:
		strButtonType = _T("PinOn");
		break;
	case CExtPaintManager::__DCBT_WND_UNKEEP:
		strButtonType = _T("PinOff");
		break;
	default:
		CExtPaintManager::PaintDockingCaptionButton( dc, _pdcbd );
		return;
	}

LPCTSTR strButtonState = _T("");
	if( !_pdcbd.m_bEnabled )
		strButtonState = _T("Disabled");
	else if( _pdcbd.m_bPushed )
		strButtonState = _T("Pressed");
	else if( _pdcbd.m_bHover )
		strButtonState = _T("Hover");
	else
		strButtonState = _T("Normal");

LPCTSTR strBarActiveState =
		_pdcbd.m_bBarWndActive ? _T("Active") : _T("Inactive");

	CExtSkinGlyph & ButtonFrameGlyph = 
		m_Skin[_T("Caption")][_T("CaptionButton")]
			[strButtonState][strButtonType][strBarActiveState].Glyph( _T("Frame") );
	ButtonFrameGlyph.Draw( dc.m_hDC, _pdcbd.m_rcClient );

	CExtSkinGlyph & ButtonGlyph = 
		m_Skin[_T("Caption")][_T("CaptionButton")]
		[strButtonState][strButtonType][strBarActiveState].Glyph( strButtonType );
	ButtonGlyph.Draw( dc.m_hDC, _pdcbd.m_rcClient );
}

INT CExtPaintManagerSkin::GetSeparatorHeight()
{
	ASSERT_VALID( this );
CExtSkinGlyph & SeparatorGlyph = m_Skin[_T("MenuItem")][_T("Separator")].Glyph( _T("Separator") );
	return SeparatorGlyph.GetSize().cy;
}


CSize CExtPaintManagerSkin::GetResizingGriperSize(
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	pHelperSrc;
	lParam;
	return CSize( 18, 18 );	
}

void CExtPaintManagerSkin::PaintResizingGripper(
	CDC & dc,
	const RECT & rcGrip,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if(		rcGrip.left > rcGrip.right
		||	rcGrip.top > rcGrip.bottom
		||	( ! dc.RectVisible(&rcGrip) )
		)
		return;
	lParam;

//COLORREF clrLineShadow = GetColor( COLOR_WINDOW, pHelperSrc, lParam );
//COLORREF clrLineFace = GetColor( COLOR_3DSHADOW, pHelperSrc, lParam );
//CPen penLineShadow(PS_SOLID, 1, clrLineShadow);
//CPen penLineFace(PS_SOLID, 1, clrLineFace);
//CPen * pOldPen = NULL;
//
//CPoint ptTop = CPoint( rcGrip.right, rcGrip.bottom - 3 );
//CPoint ptBottom = CPoint( rcGrip.right - 2, rcGrip.bottom - 1 );
//
//	for( int nLine = 0; nLine < 3; nLine++ )
//	{
//		pOldPen = dc.SelectObject( &penLineFace );
//		dc.MoveTo( ptBottom );
//		dc.LineTo( ptTop );
//		ptBottom.Offset( -1, 0 );
//		ptTop.Offset( 0, -1 );
//		dc.MoveTo( ptBottom );
//		dc.LineTo( ptTop );
//		dc.SelectObject( pOldPen );
//
//		pOldPen = dc.SelectObject( &penLineShadow );
//		ptBottom.Offset( -1, 0 );
//		ptTop.Offset( 0, -1 );
//		dc.MoveTo( ptBottom );
//		dc.LineTo( ptTop );
//		dc.SelectObject( pOldPen );
//		
//		ptBottom.Offset( -2, 0 );
//		ptTop.Offset( 0, -2 );
//	}

LPCTSTR strWindowType = _T("Generic");
	if( pHelperSrc != NULL )
	{
		ASSERT_VALID( pHelperSrc );
#ifndef __EXT_MFC_NO_STATUSBAR
		if( pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar)) )
			strWindowType = _T("StatusBar");
		else
#endif // ifndef __EXT_MFC_NO_STATUSBAR
		if( pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtResizableDialog)) )
			strWindowType = _T("Dialog");
	} // if( pHelperSrc != NULL )
	CExtSkinGlyph & ResizingGripperGlyph = 
		m_Skin[_T("ResizingGripper")][strWindowType]
		.Glyph( _T("ResizingGripper") );
	ResizingGripperGlyph.Draw( dc.m_hDC, rcGrip );
}

#ifndef __EXT_MFC_NO_STATUSBAR
bool CExtPaintManagerSkin::StatusBar_QuerySkinSupport(
	const CExtStatusControlBar * pStatusBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pStatusBar );
	pStatusBar;
	lParam;
	return true;
}

bool CExtPaintManagerSkin::StatusBar_ErasePaneBackground(
	CDC & dc,
	int nPaneIdx,
	const RECT & rcPane,
	const CExtStatusControlBar * pStatusBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pStatusBar );
	dc;
	nPaneIdx;
	rcPane;
	pStatusBar;
	lParam;
int iWidth = 0;
UINT nIndex = 0, nPaneStyle = 0;
	pStatusBar->GetPaneInfo( nPaneIdx, nIndex, nPaneStyle, iWidth );
bool bDisabled = ( ( nPaneStyle & SBPS_DISABLED ) != 0 ) ? true : false;
LPCTSTR strPaneState = bDisabled ? _T("Disabled") : _T("Enabled");
const CExtSkinGlyph & PaneGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("StatusBar")][strPaneState].Glyph( _T("Pane") );
CExtSkinGlyph * pPaneGlyph = const_cast<CExtSkinGlyph*>( &PaneGlyph );
	pPaneGlyph->Draw( dc.m_hDC, rcPane );
	return false;
}

bool CExtPaintManagerSkin::StatusBar_PaintPane(
	CDC & dc,
	int nPaneIdx,
	const RECT & rcPane,
	DWORD dwPaneStyle,
	HICON hIcon,
	__EXT_MFC_SAFE_LPCTSTR sPaneText,
	UINT nDrawTextFlags,
	COLORREF clrText,
	const CExtStatusControlBar * pStatusBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pStatusBar );
	nPaneIdx;
	clrText;
	pStatusBar;
	lParam;

bool bDisabled = ( ( dwPaneStyle & SBPS_DISABLED ) != 0 ) ? true : false;
LPCTSTR strPaneState = bDisabled ? _T("Disabled") : _T("Enabled");
const CExtSkinGlyph & PaneGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("StatusBar")][strPaneState].Glyph( _T("Pane") );
CRect rectPane( rcPane );
	rectPane.DeflateRect(
		&PaneGlyph.GetContentPadding()
		);

	CRect rcText( rectPane );
	if( hIcon != NULL )
	{
		CExtCmdIcon _icon;
		_icon.AssignFromHICON( hIcon, true );
		CSize sizeIcon = _icon.GetSize();
		ASSERT( sizeIcon.cx > 0 && sizeIcon.cy > 0 );
		rcText.left += sizeIcon.cx + PaneGlyph.GetIconIndent();
		_icon.Paint(
			const_cast < CExtPaintManagerSkin * > ( this ),
			dc,
			rectPane.left,
			rectPane.top + ( rectPane.Height() - sizeIcon.cy ) / 2,
			-1,
			-1
			);
	} // if( hIcon != NULL )
	
int nPaneTextLen =
		(sPaneText == NULL)
		? int(0)
		: int(_tcslen(sPaneText))
		;
	if(		( !bDisabled )
		||  ( bDisabled && ( !pStatusBar->m_bHideTextOnDisabledPanes ) )
		&&	nPaneTextLen > 0
		)
	{ // if text painting is allowed
		if( rcText.right > rcText.left )
		{
			const CExtSkinColor & PaneTextColor = 
				m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("StatusBar")][strPaneState].Color( _T("Text") );
			if( ! PaneTextColor.IsHollow() )
			{
				COLORREF clrTextCustom = // RGB( 212, 220, 220 );
					PaneTextColor;
				COLORREF clrTextRestore = dc.SetTextColor( clrTextCustom );
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					LPCTSTR(sPaneText), nPaneTextLen,
					&rcText,
					nDrawTextFlags, 0
					);
				if( clrTextRestore != COLORREF(-1L) )
					dc.SetTextColor( clrTextRestore );
			} // if( ! PaneTextColor.IsHollow() )
		} // if( rcText.right > rcText.left )
	} // if text painting is allowed

	return true;
}

bool CExtPaintManagerSkin::StatusBar_PaintSeparator(
	CDC & dc,
	int nPaneIdxAfterSep,
	const RECT & rcPaneBefore,
	const RECT & rcPaneAfter,
	const CExtStatusControlBar * pStatusBar,
	LPARAM lParam // = 0
	) const
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pStatusBar );
	nPaneIdxAfterSep;
	rcPaneBefore;
	rcPaneAfter;
	pStatusBar;
	lParam;

int iWidth = 0;
UINT nIndex = 0, nPaneStyle = 0;
	pStatusBar->GetPaneInfo( nPaneIdxAfterSep, nIndex, nPaneStyle, iWidth );
bool bDisabled = ( ( nPaneStyle & SBPS_DISABLED ) != 0 ) ? true : false;
	LPCTSTR strPaneState = bDisabled ? _T("Disabled") : _T("Enabled");

CRect rcSeparator(
		rcPaneBefore.right,
		rcPaneAfter.top,
		rcPaneAfter.left,
		rcPaneAfter.bottom
		);
	
	const CExtSkinGlyph & SeparatorGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][_T("StatusBar")][strPaneState].Glyph( _T("Separator") );

CExtSkinGlyph * pGlyph = const_cast<CExtSkinGlyph*>( &SeparatorGlyph );
CSize _sizeGlyph = pGlyph->GetSize();
int nWidth = rcSeparator.Width();
	if( nWidth != _sizeGlyph.cx )
	{
		rcSeparator.left += ( nWidth - _sizeGlyph.cx ) / 2;
		rcSeparator.right = rcSeparator.left + _sizeGlyph.cx;
	}
	pGlyph->Draw( dc.m_hDC, rcSeparator );
	
	return true;
}
#endif // ifndef __EXT_MFC_NO_STATUSBAR

void CExtPaintManagerSkin::PaintIcon(
	CDC & dc,
	bool bHorz,
	CExtCmdIcon * pIcon,
	const CRect & rcClient,
	bool bPushed,
	bool bEnabled,
	bool bHover, // = false
	INT nIconAlignment, // = 0
	bool bRTL, // = false
	CRect * prcIcon, // = NULL
	CRect * prcText, // = NULL
	CRect * prcIconMargins, // = NULL
	COLORREF clrCustomAccentEffectForIcon // = COLORREF(-1L)
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bHorz;
	bHover;
	bRTL;
	
	if( pIcon == NULL || pIcon->IsEmpty() )
	{
		if( prcIcon != NULL )
			*prcIcon = CRect( 0, 0, 0, 0 );
		if( prcText != NULL )
			*prcText = rcClient;
		return;
	}

CSize _sizeIcon = pIcon->GetSize();
	ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );

CRect rcIconMargins( 0, 0, 0, 0 );
	if( prcIconMargins != NULL )
		rcIconMargins = *prcIconMargins;

CRect rcTextLocation( rcClient );
CRect rcIconLocation( 0, 0, _sizeIcon.cx, _sizeIcon.cy );

	if( bHorz )
	{
		rcIconLocation.OffsetRect( 
			rcClient.left, 
			rcClient.top 
			);

		if(	( _sizeIcon.cx + rcIconMargins.left + rcIconMargins.right ) > rcClient.Width() )
		{
			nIconAlignment &= ~__ALIGN_HORIZ_MASK;
			nIconAlignment |= __ALIGN_HORIZ_CENTER;
		}

		if(	( _sizeIcon.cy + rcIconMargins.top + rcIconMargins.bottom ) > rcClient.Height() )
		{
			nIconAlignment &= ~__ALIGN_VERT_MASK;
			nIconAlignment |= __ALIGN_VERT_CENTER;
		}

		if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_RIGHT )
		{
			rcIconLocation.OffsetRect( rcClient.right - rcIconLocation.right - rcIconMargins.right, 0 );
			rcTextLocation.right = rcIconLocation.left - rcIconMargins.left;
		}
		else if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
		{
			rcIconLocation.OffsetRect( ( rcClient.right - rcIconLocation.right ) / 2 + rcIconMargins.left - rcIconMargins.right, 0 );
		}
		else if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_LEFT )
		{
			rcIconLocation.OffsetRect( rcIconMargins.left, 0 );
			rcTextLocation.left = rcIconLocation.right + rcIconMargins.right;
		}

		if( (nIconAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_TOP )
		{
			rcIconLocation.OffsetRect( 0, rcIconMargins.top );
			if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
				rcTextLocation.top = rcIconLocation.bottom + rcIconMargins.bottom;
		}
		else if( (nIconAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_CENTER )
		{
			rcIconLocation.OffsetRect( 0, ( rcClient.bottom - rcIconLocation.bottom ) / 2 + rcIconMargins.top - rcIconMargins.bottom );
		}
		else if( (nIconAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_BOTTOM )
		{
			rcIconLocation.OffsetRect( 0, rcClient.bottom - rcIconLocation.bottom - rcIconMargins.bottom );
			if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
				rcTextLocation.bottom = rcIconLocation.top - rcIconMargins.top;
		}
	}
	else
	{
		rcIconLocation.OffsetRect( 
			rcClient.right - rcIconLocation.Width(), 
			rcClient.top 
			);

		if(	( _sizeIcon.cy + rcIconMargins.top + rcIconMargins.bottom ) > rcClient.Height() )
		{
			nIconAlignment &= ~__ALIGN_HORIZ_MASK;
			nIconAlignment |= __ALIGN_HORIZ_CENTER;
		}

		if(	( _sizeIcon.cx + rcIconMargins.left + rcIconMargins.right ) > rcClient.Width() )
		{
			nIconAlignment &= ~__ALIGN_VERT_MASK;
			nIconAlignment |= __ALIGN_VERT_CENTER;
		}

		if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_RIGHT )
		{
			rcIconLocation.OffsetRect( 0, rcClient.bottom - rcIconLocation.bottom - rcIconMargins.bottom );
			rcTextLocation.bottom = rcIconLocation.top - rcIconMargins.top;
		}
		else if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
		{
			rcIconLocation.OffsetRect( 0, ( rcClient.bottom - rcIconLocation.bottom ) / 2 + rcIconMargins.top - rcIconMargins.bottom );
		}
		else if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_LEFT )
		{
			rcIconLocation.OffsetRect( 0, rcIconMargins.top );
			rcTextLocation.top = rcIconLocation.bottom;
		}
		
		if( (nIconAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_TOP )
		{
			rcIconLocation.OffsetRect( rcIconMargins.right, 0 );
			if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
				rcTextLocation.right = rcIconLocation.left;
		}
		else if( (nIconAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_CENTER )
		{
			rcIconLocation.OffsetRect( ( rcClient.left - rcIconLocation.left ) / 2 + rcIconMargins.right - rcIconMargins.left, 0 );
		}
		else if( (nIconAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_BOTTOM )
		{
			rcIconLocation.OffsetRect( rcClient.left - rcIconLocation.left - rcIconMargins.left, 0 );
			if( (nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER )
				rcTextLocation.left = rcIconLocation.right;
		}
	}

	if(		(nIconAlignment&__ALIGN_HORIZ_MASK) == __ALIGN_HORIZ_CENTER 
		&&	(nIconAlignment&__ALIGN_VERT_MASK) == __ALIGN_VERT_CENTER
		)
		rcTextLocation.SetRectEmpty();

	if(		rcTextLocation.top >= rcTextLocation.bottom
		||	rcTextLocation.left >= rcTextLocation.right
		)
		rcTextLocation.SetRectEmpty();
    
	// if button is pressed then press the icon also
	if( bPushed )
	{
		CSize sizePushedOffset = GetPushedOffset();
		rcIconLocation.OffsetRect( sizePushedOffset.cx, sizePushedOffset.cy );
	}

	if(		( ! bEnabled )
		&&	clrCustomAccentEffectForIcon != COLORREF(-1L)
		)
	{
		pIcon->PaintAccentColorized(
			this,
			dc.GetSafeHdc(),
			clrCustomAccentEffectForIcon,
			rcIconLocation.left,
			rcIconLocation.top
			);
	}
	else
	{
		pIcon->Paint(
			this,
			dc.GetSafeHdc(),
			rcIconLocation.left,
			rcIconLocation.top,
			-1,
			-1,
			(! bEnabled )
				? CExtCmdIcon::__PAINT_DISABLED
				: bPushed
					? CExtCmdIcon::__PAINT_PRESSED
					: bHover
						? CExtCmdIcon::__PAINT_HOVER
						: CExtCmdIcon::__PAINT_NORMAL
			);
	}

	if( prcIcon != NULL )
		*prcIcon = rcIconLocation;
	if( prcText != NULL )
		*prcText = rcTextLocation;
}

#if (!defined __EXT_MFC_NO_TAB_CTRL)

bool CExtPaintManagerSkin::QueryTabWndHoverChangingRedraw(
	const CExtTabWnd * pWndTab,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pWndTab );
	pWndTab;
	lParam;
	return true;
}

void CExtPaintManagerSkin::PaintTabbedTabClientArea(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this);
	ASSERT( dc.GetSafeHdc() != NULL );
	if(		pHelperSrc == NULL
		||	(! pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtTabWnd ) ) )
		)
		CExtPaintManager::PaintTabbedTabClientArea(
			dc,
			rcClient,
			rcTabItemsArea,
			rcTabNearBorderArea,
			dwOrientation,
			bGroupedMode,
			pHelperSrc,
			lParam
			);
	PaintTabClientArea(
		dc,
		rcClient,
		rcTabItemsArea,
		rcTabNearBorderArea,
		dwOrientation,
		bGroupedMode,
		pHelperSrc,
		lParam
		);
}

__EXT_MFC_SAFE_LPCTSTR CExtPaintManagerSkin::GetSkinnedTabControlType(
	const CExtTabWnd * pWndTab
	)
{
	ASSERT_VALID( this );
	if( pWndTab == NULL )
		return _T("Control");
	ASSERT_VALID( pWndTab );
	if( pWndTab->IsKindOf( RUNTIME_CLASS( CExtDynAutoHideArea ) ) )
		return _T("AutoHideArea");
	if( pWndTab->IsKindOf( RUNTIME_CLASS( CExtDynTabWnd ) ) )
		return _T("ResizableBarGroup");
	if( pWndTab->_IsMdiTabCtrl() )
		return _T("MDI");
	if( pWndTab->GetSafeHwnd() == NULL )
		return _T("Control");
CWnd * pWndParent = pWndTab->GetParent();
	if(		pWndParent != NULL
		&&	pWndParent->IsKindOf( RUNTIME_CLASS( CExtTabPageContainerWnd ) )
		)
		return _T("PageContainer");
	return _T("Control");
}

__EXT_MFC_SAFE_LPCTSTR CExtPaintManagerSkin::GetSkinnedTabControlOrientation(
	const CExtTabWnd * pWndTab
	)
{
	ASSERT_VALID( this );
	if( pWndTab == NULL )
		return _T("Top");
	ASSERT_VALID( pWndTab );
DWORD dwOrientation = pWndTab->OrientationGet();
	if( dwOrientation == __ETWS_ORIENT_BOTTOM )
		return _T("Bottom");
	if( dwOrientation == __ETWS_ORIENT_LEFT )
		return _T("Left");
	if( dwOrientation == __ETWS_ORIENT_RIGHT )
		return _T("Right");
	return _T("Top");
}

void CExtPaintManagerSkin::PaintTabClientArea(
	CDC & dc,
	CRect & rcClient,
	CRect & rcTabItemsArea,
	CRect & rcTabNearBorderArea,
	DWORD dwOrientation,
	bool bGroupedMode,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this);
	ASSERT( dc.GetSafeHdc() != NULL );
	if(		pHelperSrc == NULL
		||	(! pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtTabWnd ) ) )
		||	(! PaintDockerBkgnd( true, dc, (CExtTabWnd*)pHelperSrc, lParam ) )
		)
	{
		CExtPaintManager::PaintTabClientArea(
			dc,
			rcClient,
			rcTabItemsArea,
			rcTabNearBorderArea,
			dwOrientation,
			bGroupedMode,
			pHelperSrc,
			lParam
			);
		return;
	}
CWnd * pGen = STATIC_DOWNCAST( CWnd, pHelperSrc );
CExtTabWnd * pExtTabWnd = DYNAMIC_DOWNCAST( CExtTabWnd, pGen );
LPCTSTR strSkinnedTabControlType = GetSkinnedTabControlType( pExtTabWnd );
LPCTSTR strSkinnedTabControlOrientation = GetSkinnedTabControlOrientation( pExtTabWnd );
CExtSkinGlyph & tabAreaEntire = 
	m_Skin[_T("Background")][_T("Tab")][_T("EntireArea")]
		[strSkinnedTabControlOrientation][strSkinnedTabControlType]
		.Glyph( _T("Area") );
	tabAreaEntire.Draw( dc.m_hDC, rcClient );
CExtSkinGlyph & tabAreaItems = 
	m_Skin[_T("Background")][_T("Tab")][_T("ItemsArea")]
		[strSkinnedTabControlOrientation][strSkinnedTabControlType]
		.Glyph( _T("Area") );
	tabAreaItems.Draw( dc.m_hDC, rcTabItemsArea );
CExtSkinGlyph & tabAreaNearBorder = 
	m_Skin[_T("Background")][_T("Tab")][_T("NearBorder")]
		[strSkinnedTabControlOrientation][strSkinnedTabControlType]
		.Glyph( _T("Area") );
CRect rcTabNearBorderArea2 = rcTabNearBorderArea;
CRect rcOffset = tabAreaNearBorder.GetOffset();
	rcTabNearBorderArea2.OffsetRect( rcOffset.left, rcOffset.top );
	tabAreaNearBorder.Draw( dc.m_hDC, rcTabNearBorderArea2 );
}

void CExtPaintManagerSkin::PaintTabItem(
	CDC & dc,
	CRect & rcTabItemsArea,
	bool bTopLeft,
	bool bHorz,
	bool bSelected,
	bool bCenteredText,
	bool bGroupedMode,
	bool bInGroupActive,
	bool bInvertedVerticalMode,
	const CRect & rcEntireItem,
	CSize sizeTextMeasured,
	CFont * pFont,
	__EXT_MFC_SAFE_LPCTSTR sText,
	CExtCmdIcon * pIcon,
	CObject * pHelperSrc,
	LPARAM lParam, // = 0L
	COLORREF clrForceText, // = COLORREF(-1L)
	COLORREF clrForceTabBk, // = COLORREF(-1L)
	COLORREF clrForceTabBorderLT, // = COLORREF(-1L)
	COLORREF clrForceTabBorderRB, // = COLORREF(-1L)
	COLORREF clrForceTabSeparator, // = COLORREF(-1L)
	bool bDwmMode // = false
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bDwmMode;
	if(		pHelperSrc == NULL
//		||	(	(! pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtTabWnd ) ) )
//			&&	(! pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtBarButton ) ) )
//			)
		||	IsHighContrast()
		)
	{
		CExtPaintManager::PaintTabItem(
			dc,
			rcTabItemsArea,
			bTopLeft,
			bHorz,
			bSelected,
			bCenteredText,
			bGroupedMode,
			bInGroupActive,
			bInvertedVerticalMode,
			rcEntireItem,
			sizeTextMeasured,
			pFont,
			sText,
			pIcon,
			pHelperSrc,
			lParam, // = 0L
			clrForceText, // = COLORREF(-1L)
			clrForceTabBk, // = COLORREF(-1L)
			clrForceTabBorderLT, // = COLORREF(-1L)
			clrForceTabBorderRB, // = COLORREF(-1L)
			clrForceTabSeparator // = COLORREF(-1L)
			);
		return;
	}
LPCTSTR _sText = LPCTSTR(sText);
CExtTabWnd * pWndTab = DYNAMIC_DOWNCAST( CExtTabWnd, pHelperSrc );
CExtBarButton * pTBB = DYNAMIC_DOWNCAST( CExtBarButton, pHelperSrc );
bool bDrawIcon =
		(	pWndTab != NULL		
		&&	pIcon != NULL 
		&&	(! pIcon->IsEmpty() ) 
		&&	(pWndTab->GetTabWndStyle()&__ETWS_HIDE_ICONS) == 0 
		)
		? true : false;

LPCTSTR strGroupped = _T("Simple"); // bGroupedMode ? _T("Groupped") : _T("Simple");
if( bGroupedMode )
	bSelected = false;

LPCTSTR strItemType = _T("Disabled");
bool bHover = false, bEnabled = false;
CExtTabWnd::TAB_ITEM_INFO * pTII = NULL;
bool bNoPrefix = false;
	if( pWndTab != NULL )
	{
		ASSERT_VALID( pWndTab );
		pTII = pWndTab->ItemGet( LONG( lParam ) );
		ASSERT( pTII != NULL );
		ASSERT_VALID( pTII );
		if( pTII->EnabledGet() )
		{
			bEnabled = true;
			strItemType = bSelected ? _T("Selected") : _T("Default");
			bHover = ( pWndTab->GetHoverTrackingItem() == lParam ) ? true : false;
		}
		bNoPrefix = ( ( pWndTab->GetTabWndStyleEx() & __ETWS_EX_NO_PREFIX ) != 0 ) ? true : false;
	}
	else if( pTBB != NULL )
	{
		if( pTBB->IsEnabled() )
		{
			bEnabled = true;
			strItemType = bSelected ? _T("Selected") : _T("Default");
			bHover = pTBB->IsHover();
		}
	}
LPCTSTR strHover = bHover ? _T("Hover") : _T("Normal");
LPCTSTR strSkinnedTabControlType = GetSkinnedTabControlType( pWndTab );
LPCTSTR strSkinnedTabControlOrientation = GetSkinnedTabControlOrientation( pWndTab );
CExtSkinGlyph & tabItem = 
	m_Skin[_T("TabItem")][strGroupped][strItemType][strHover]
		[strSkinnedTabControlOrientation][strSkinnedTabControlType]
		.Glyph( _T("Area") );
CExtSkinColor & tabText = 
	m_Skin[_T("TabItem")][strGroupped][strItemType][strHover]
		[strSkinnedTabControlOrientation][strSkinnedTabControlType]
		.Color( _T("Text") );
COLORREF clrText = RGB(0,0,0);
	if( tabText.IsHollow() )
		_sText = _T("");
	else
		clrText = tabText;
CRect rcEntireItemA = rcEntireItem;
	rcEntireItemA.top += tabItem.GetOffset().top;
	rcEntireItemA.bottom += tabItem.GetOffset().bottom;
	rcEntireItemA.left += tabItem.GetOffset().left;
	rcEntireItemA.right += tabItem.GetOffset().right;

#if (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE)
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
bool bDetectedDynTPC = false;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( pTII != NULL )
	{
		CObject * pObject = pTII->EventProviderGet();
		if( pObject != NULL )
		{
			CExtDynamicControlBar * pBar = DYNAMIC_DOWNCAST( CExtDynamicControlBar, pObject );
			if( pBar != NULL )
			{
				bool bFlashCaptionHighlightedState = false;
				if( pBar->FlashCaptionIsInProgress( &bFlashCaptionHighlightedState ) )
				{
					if( bFlashCaptionHighlightedState )
					{
						clrForceText = pBar->m_clrFlashCaptionText;
						clrForceTabBk = pBar->m_clrFlashCaptionBackground;
						bSelected = true;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
						bDetectedDynTPC = true;
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
					}
				}
			}
		}
	}
#endif// (!defined __EXT_MFC_NO_DYNAMIC_BAR_SITE)

COLORREF clrColorizeTabShape = COLORREF(-1L);
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if(		pHelperSrc != NULL
		&&	clrForceTabBk != COLORREF(-1L)
		&&	(	bDetectedDynTPC
			||	pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtDynTabWnd ) )
			||	pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtDynAutoHideArea ) )
			)
		)
	{
		if( clrForceText != COLORREF(-1L) )
			clrText = clrForceText;
		clrColorizeTabShape = clrForceTabBk;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	if(		pHelperSrc != NULL
		&&	clrForceTabBk != COLORREF(-1L)
		&&	pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtTabWnd ) )
		&&	((CExtTabWnd*)pHelperSrc)->_IsMdiTabCtrl()
		)
	{
		if( clrForceText != COLORREF(-1L) )
			clrText = clrForceText;
		clrColorizeTabShape = clrForceTabBk;
	}
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

	tabItem.Draw( dc.m_hDC, rcEntireItemA, false, BYTE(0xFF), clrColorizeTabShape );

CRect rcEntireItemB = rcEntireItem;
CRect rcDeflate = tabItem.GetContentPadding();
	if( bHorz )
		rcEntireItemB.OffsetRect( rcDeflate.left, 0 );
	else
		rcEntireItemB.OffsetRect( 0, rcDeflate.top );
CExtCmdIcon * pIconTabItemCloseButton = NULL;
CExtTabWnd * pTabWnd = DYNAMIC_DOWNCAST( CExtTabWnd, pHelperSrc );
CRect rcTabItemCloseButton( 0, 0, 0, 0 );
CExtCmdIcon::e_paint_type_t ePaintStateITICB = CExtCmdIcon::__PAINT_NORMAL;
	if( pTabWnd != NULL && ::IsWindow(pTabWnd->m_hWnd) )
	{
		ASSERT_VALID( pTabWnd );
		CExtTabWnd::TAB_ITEM_INFO * pTii = pTabWnd->ItemGet( LONG( lParam ) );
		ASSERT( pTii != NULL );
		ASSERT_VALID( pTii );
		pIconTabItemCloseButton =
			pTabWnd->OnTabWndQueryItemCloseButtonShape( pTii );
		ePaintStateITICB =
			(CExtCmdIcon::e_paint_type_t)
				pTabWnd->OnTabWndQueryItemCloseButtonPaintState( pTii );
		if( pIconTabItemCloseButton != NULL )
			rcTabItemCloseButton = pTii->CloseButtonRectGet();
	}
	CExtPaintManager::stat_PaintTabItemImpl(
		dc,
		rcTabItemsArea,
		bTopLeft,
		bHorz,
		bSelected,
		bEnabled,
		bCenteredText,
		bGroupedMode,
		bInGroupActive,
		bInvertedVerticalMode,
		bDrawIcon,
		rcEntireItemB, //rcEntireItem,
		sizeTextMeasured,
		pFont,
		_sText,
		bNoPrefix,
		pIcon,
		pIconTabItemCloseButton,
		INT(ePaintStateITICB),
		rcTabItemCloseButton,
		clrText,
		COLORREF(-1L), // clrTabBk,
		COLORREF(-1L), // clrTabBorderLT,
		COLORREF(-1L), // clrTabBorderRB,
		COLORREF(-1L), // clrTabSeparator
		true,
		pHelperSrc
		);
}

void CExtPaintManagerSkin::PaintTabButton(
	CDC & dc,
	CRect & rcButton,
	LONG nHitTest,
	bool bTopLeft,
	bool bHorz,
	bool bEnabled,
	bool bHover,
	bool bPushed,
	bool bGroupedMode,
	CObject * pHelperSrc,
	LPARAM lParam, // = 0L
	bool bFlat // = false
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bTopLeft;
	bHorz;
	lParam;
	if(		pHelperSrc == NULL
		||	(! pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtTabWnd ) ) )
		)
	{
		CExtPaintManager::PaintTabButton(
			dc,
			rcButton,
			nHitTest,
			bTopLeft,
			bHorz,
			bEnabled,
			bHover,
			bPushed,
			bGroupedMode,
			pHelperSrc,
			lParam,
			bFlat
			);
		return;
	}
LPCTSTR strButtonType = _T("Close");
	switch( nHitTest )
	{
	case __ETWH_BUTTON_LEFTUP:
		strButtonType = _T("Up");
	break;
	case __ETWH_BUTTON_RIGHTDOWN:
		strButtonType = _T("Down");
	break;
	case __ETWH_BUTTON_HELP:
		strButtonType = _T("Help");
	break;
	case __ETWH_BUTTON_CLOSE:
		strButtonType = _T("Close");
	break;
	case __ETWH_BUTTON_TAB_LIST:
		strButtonType = _T("TabList");
	break;
	case __ETWH_BUTTON_SCROLL_HOME:
		strButtonType = _T("Home");
	break;
	case __ETWH_BUTTON_SCROLL_END:
		strButtonType = _T("End");
	break;
	default:
		CExtPaintManager::PaintTabButton(
			dc,
			rcButton,
			nHitTest,
			bTopLeft,
			bHorz,
			bEnabled,
			bHover,
			bPushed,
			bGroupedMode,
			pHelperSrc,
			lParam,
			bFlat
			);
	return;
	} // switch( nHitTest )
CExtTabWnd * pWndTab = STATIC_DOWNCAST( CExtTabWnd, pHelperSrc );

LPCTSTR strGroupped = _T("Simple"); // bGroupedMode ? _T("Groupped") : _T("Simple");

LPCTSTR strState =
		bEnabled
		?	(	bPushed
					? _T("Pressed")
					: bHover ? _T("Hover") : _T("Normal")
			)
		: _T("Disabled")
		;
//LPCTSTR strSkinnedTabControlType = GetSkinnedTabControlType( pWndTab );
LPCTSTR strSkinnedTabControlOrientation = GetSkinnedTabControlOrientation( pWndTab );
CExtSkinGlyph & tabItem = 
	m_Skin[_T("TabButton")][strGroupped][strButtonType][strState]
		[strSkinnedTabControlOrientation] //[strSkinnedTabControlType]
		.Glyph( _T("Area") );
	tabItem.Draw( dc.m_hDC, rcButton );
}

void CExtPaintManagerSkin::PaintTabNcAreaRect(
	CDC & dc,
	const RECT & rc,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if(		pHelperSrc == NULL
		||	(! pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtTabWnd ) ) )
		)
	{
		CExtPaintManager::PaintTabNcAreaRect(
			dc,
			rc,
			pHelperSrc,
			lParam
			);
		return;
	}
	PaintDockerBkgnd( false, dc, (CWnd*)pHelperSrc, lParam );
}

void CExtPaintManagerSkin::TabWnd_MeasureItemAreaMargins(
	CExtTabWnd * pTabWnd,
	LONG & nSpaceBefore,
	LONG & nSpaceAfter,
	LONG & nSpaceOver
	)
{
	ASSERT_VALID( this );
	ASSERT( pTabWnd != NULL && ::IsWindow(pTabWnd->m_hWnd) );
	ASSERT_VALID( pTabWnd );
	CExtPaintManager::TabWnd_MeasureItemAreaMargins(
		pTabWnd,
		nSpaceBefore,
		nSpaceAfter,
		nSpaceOver
		);
}

void CExtPaintManagerSkin::TabWnd_UpdateItemMeasure(
	CExtTabWnd * pTabWnd,
	CExtTabWnd::TAB_ITEM_INFO * pTii,
	CDC & dcMeasure,
	CSize & sizePreCalc
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pTii );
	ASSERT( dcMeasure.GetSafeHdc() != NULL );
	ASSERT( pTii->GetTabWnd() == pTabWnd );
	CExtPaintManager::TabWnd_UpdateItemMeasure(
		pTabWnd,
		pTii,
		dcMeasure,
		sizePreCalc
		);
}

void CExtPaintManagerSkin::GetThemeAccentTabColors(
	COLORREF * pClrSetectedTabItemText,
	COLORREF * pClrSetectedTabItemFace, // = NULL
	COLORREF * pClrSetectedTabItemBorderLight, // = NULL
	COLORREF * pClrSetectedTabItemBorderDark, // = NULL
	CObject * pHelperSrc, // = NULL
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	CExtPaintManager::GetThemeAccentTabColors(
		pClrSetectedTabItemText,
		pClrSetectedTabItemFace,
		pClrSetectedTabItemBorderLight,
		pClrSetectedTabItemBorderDark,
		pHelperSrc,
		lParam
		);
}

#endif // (!defined __EXT_MFC_NO_TAB_CTRL)


void CExtPaintManagerSkin::PaintSeparator(
	CDC & dc,
	const RECT & rectItem,
	bool bHorz,
	bool bTransparentBk,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

CExtToolControlBar * pToolBar = DYNAMIC_DOWNCAST( CExtToolControlBar, pHelperSrc );
	if( pToolBar == NULL )
	{
		CExtPaintManager::PaintSeparator(  
			dc, 
			rectItem, 
			bHorz, 
			bTransparentBk, 
			pHelperSrc, 
			lParam
			);
		return;
	}

LPCTSTR strBarType = _T("ToolBar");
	if( pToolBar->IsKindOf( RUNTIME_CLASS( CExtMenuControlBar ) ) )
		strBarType = _T("MenuBar");
LPCTSTR strSeparatorType = bHorz ? _T("SeparatorV") : _T("SeparatorH");
CExtSkinGlyph & SeparatorGlyph = 
		m_Skin[_T("ImageCollection")][_T("ControlBar")][strBarType].Glyph( strSeparatorType );
CSize _size = SeparatorGlyph.GetSize();
	if( _size.cx == 0 || _size.cy == 0 )
		return;
CRect rectItemA = rectItem;
	if( bHorz )
	{
		rectItemA.left += ( rectItemA.Width() - _size.cx ) / 2;
		rectItemA.right = rectItemA.left + _size.cx;
	}
	else
	{
		rectItemA.top += ( rectItemA.Height() - _size.cy ) / 2;
		rectItemA.bottom = rectItemA.top + _size.cy;
	}
	SeparatorGlyph.Draw( dc.m_hDC, rectItemA );
}

COLORREF CExtPaintManagerSkin::GetIconAlphaColor() const
{
	ASSERT_VALID( this );
COLORREF clrIconAlpha = m_Skin[ _T("General") ].Color( _T("IconAlphaColor") );
	return clrIconAlpha;
}

double CExtPaintManagerSkin::GetIconAlphaAdjustPercent() const
{
	ASSERT_VALID( this );
//	return 0.70; // 70% enlighten
	return -0.50;
}


bool CExtPaintManagerSkin::AdjustIcon( 
	CExtCmdIcon * pIcon
	)
{
	ASSERT_VALID( this );
	if( pIcon == NULL )
		return false;

	if( (pIcon->m_dwFlags&__EXT_ICON_PERSISTENT_BITMAP_DISABLED) == 0 )
	{
		pIcon->m_bmpDisabled.Empty();
		pIcon->m_bmpDisabled = pIcon->m_bmpNormal;
		pIcon->m_bmpDisabled.Make32();

		COLORREF clrAlpha = GetIconAlphaColor();
		if( clrAlpha == COLORREF(-1L) )
		{
			pIcon->m_bmpDisabled.MakeMono( GetColor( COLOR_3DFACE ) );
			pIcon->m_bmpDisabled.AdjustAlpha( -0.70 ); // -0.50
		}
		else
		{
			double H = 0.0, S = 0.0, L = 0.0;
			CExtBitmap::stat_RGBtoHSL( clrAlpha, &H, &S, &L );
			S = 0.3;
			clrAlpha = CExtBitmap::stat_HLStoRGB( H, L, S );
			pIcon->m_bmpDisabled.MakeMono( clrAlpha );
			double lfAlphaAdjustPercent = GetIconAlphaAdjustPercent();
			pIcon->m_bmpDisabled.AdjustAlpha( lfAlphaAdjustPercent );
		}

		return true;
	}

	return false;
}

CSize CExtPaintManagerSkin::GetCheckButtonBoxSize(
	CDC & dc,
	CExtPaintManager::PAINTCHECKRADIOBUTTONDATA & _pcbd
	) const
{
	ASSERT_VALID( this );
	dc;
	_pcbd;
const CExtSkinGlyph & _glyph = m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Check")][_T("Unchecked")].Glyph(_T("Normal"));
CSize _size = _glyph.GetSize();
	return _size;
}

CSize CExtPaintManagerSkin::GetRadioButtonBoxSize(
	CDC & dc,
	CExtPaintManager::PAINTCHECKRADIOBUTTONDATA & _pcbd
	) const
{
	ASSERT_VALID( this );
	dc;
	_pcbd;
const CExtSkinGlyph & _glyph = m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Radio")][_T("Unchecked")].Glyph(_T("Normal"));
CSize _size = _glyph.GetSize();
	return _size;
}

COLORREF CExtPaintManagerSkin::QueryObjectTextColor(
	CDC & dc,
	bool bEnabled,
	bool bFocused,
	bool bHovered,
	bool bPressed,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( pHelperSrc == NULL )
		return
			CExtPaintManager::QueryObjectTextColor(
				dc,
				bEnabled,
				bFocused,
				bHovered,
				bPressed,
				pHelperSrc,
				lParam
				);
	ASSERT_VALID( pHelperSrc );
	if( pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtHyperLinkButton) ) )
	{ // hyper-link
		if( ! bEnabled )
			return COLORREF(m_Skin[_T("Button")][_T("HyperLink")].Color(_T("Disabled")));
		if( bPressed )
			return COLORREF(m_Skin[_T("Button")][_T("HyperLink")].Color(_T("Pressed")));
		if( bHovered )
			return COLORREF(m_Skin[_T("Button")][_T("HyperLink")].Color(_T("Hover")));
		if( bFocused )
			return COLORREF(m_Skin[_T("Button")][_T("HyperLink")].Color(_T("Focused")));
		return COLORREF(m_Skin[_T("Button")][_T("HyperLink")].Color(_T("Normal")));
	} // hyper-link
	if( pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtCheckBox) ) )
	{ // check box button
		if( ! bEnabled )
			return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Check")][_T("Unchecked")].Color(_T("Disabled")));
		if( bPressed )
			return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Check")][_T("Unchecked")].Color(_T("Pressed")));
		if( bHovered )
			return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Check")][_T("Unchecked")].Color(_T("Hover")));
		if( bFocused )
			return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Check")][_T("Unchecked")].Color(_T("Focused")));
		return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Check")][_T("Unchecked")].Color(_T("Normal")));
	} // check box button
	if( pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtRadioButton) ) )
	{ // check box button
		if( ! bEnabled )
			return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Radio")][_T("Unchecked")].Color(_T("Disabled")));
		if( bPressed )
			return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Radio")][_T("Unchecked")].Color(_T("Pressed")));
		if( bHovered )
			return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Radio")][_T("Unchecked")].Color(_T("Hover")));
		if( bFocused )
			return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Radio")][_T("Unchecked")].Color(_T("Focused")));
		return COLORREF(m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Radio")][_T("Unchecked")].Color(_T("Normal")));
	} // check box button
	if( pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtLabel) ) )
	{ // static
		if( ! bEnabled )
			return COLORREF(m_Skin[_T("Label")].Color(_T("Disabled")));
		return COLORREF(m_Skin[_T("Label")].Color(_T("Normal")));
	} // static
	return
		CExtPaintManager::QueryObjectTextColor(
			dc,
			bEnabled,
			bFocused,
			bHovered,
			bPressed,
			pHelperSrc,
			lParam
			);
}

void CExtPaintManagerSkin::PaintCheckButtonBox(
	CDC & dc,
	CExtPaintManager::PAINTCHECKRADIOBUTTONDATA & _pcbd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
LPCTSTR strState = _T("Normal");
	if( ! _pcbd.m_bEnabled )
		strState = _T("Disabled");
	else if( _pcbd.m_bPushed )
		strState = _T("Pressed");
	else if( _pcbd.m_bHover )
		strState = _T("Hover");
//	else if( _pcbd.m_bFocus )
//		strState = _T("Focused");
LPCTSTR strChecked = _T("Unchecked");
	switch( _pcbd.m_eState )
	{
	case CExtPaintManager::BOX_UNCHECKED:
	case CExtPaintManager::BOX_LDOWN_UNCHECKED:
	case CExtPaintManager::BOX_DISABLED_UNCHECKED:
	case CExtPaintManager::BOX_MOUSE_HOVER_UNCHECKED:
		break;
	case CExtPaintManager::BOX_CHECKED:
	case CExtPaintManager::BOX_LDOWN_CHECKED:
	case CExtPaintManager::BOX_MOUSE_HOVER_CHECKED:
	case CExtPaintManager::BOX_DISABLED_CHECKED:
		strChecked = _T("Checked");
		break;
	case CExtPaintManager::BOX_INDETERMINATE:
	case CExtPaintManager::BOX_LDOWN_INDETERMINATE:
	case CExtPaintManager::BOX_MOUSE_HOVER_INDETERMINATE:
	case CExtPaintManager::BOX_DISABLED_INDETERMINATE:
		strChecked = _T("Indeterminate");
		break;
	} // switch( _pcrbd.m_eState )
const CExtSkinGlyph & _glyph = m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Check")][strChecked].Glyph(strState);
	_glyph.Draw( dc.m_hDC, _pcbd.m_rcBox );
}
	
void CExtPaintManagerSkin::PaintRadioButtonBox(
	CDC & dc,
	CExtPaintManager::PAINTCHECKRADIOBUTTONDATA & _pcbd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
LPCTSTR strState = _T("Normal");
	if( ! _pcbd.m_bEnabled )
		strState = _T("Disabled");
	else if( _pcbd.m_bPushed )
		strState = _T("Pressed");
	else if( _pcbd.m_bHover )
		strState = _T("Hover");
//	else if( _pcbd.m_bFocus )
//		strState = _T("Focused");
LPCTSTR strChecked = _T("Unchecked");
	switch( _pcbd.m_eState )
	{
	case CExtPaintManager::BOX_UNCHECKED:
	case CExtPaintManager::BOX_LDOWN_UNCHECKED:
	case CExtPaintManager::BOX_DISABLED_UNCHECKED:
	case CExtPaintManager::BOX_MOUSE_HOVER_UNCHECKED:
		break;
	case CExtPaintManager::BOX_CHECKED:
	case CExtPaintManager::BOX_LDOWN_CHECKED:
	case CExtPaintManager::BOX_MOUSE_HOVER_CHECKED:
	case CExtPaintManager::BOX_DISABLED_CHECKED:
		strChecked = _T("Checked");
		break;
	case CExtPaintManager::BOX_INDETERMINATE:
	case CExtPaintManager::BOX_LDOWN_INDETERMINATE:
	case CExtPaintManager::BOX_MOUSE_HOVER_INDETERMINATE:
	case CExtPaintManager::BOX_DISABLED_INDETERMINATE:
		strChecked = _T("Checked");
		break;
	} // switch( _pcrbd.m_eState )
const CExtSkinGlyph & _glyph = m_Skin[_T("Button")][_T("CheckAndRadio")][_T("Radio")][strChecked].Glyph(strState);
	_glyph.Draw( dc.m_hDC, _pcbd.m_rcBox );
}

bool CExtPaintManagerSkin::PaintGroupBoxFrame(
	CDC & dc,
	const RECT & rcBorder,
	COLORREF clrFrame, // = COLORREF(-1L)
	CObject * pHelperSrc, // = NULL
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	clrFrame;
	pHelperSrc;
	lParam;
	if(		rcBorder.left >= rcBorder.right
		||	rcBorder.top >= rcBorder.bottom
		||	(! dc.RectVisible( &rcBorder ) )
		)
		return true;
const CExtSkinGlyph & _glyph = m_Skin[_T("GroupBox")].Glyph(_T("Border"));
	_glyph.Draw( dc.m_hDC, rcBorder );
	return true;
}

bool CExtPaintManagerSkin::PaintGroupBoxLabel(
	CDC & dc,
	__EXT_MFC_SAFE_LPCTSTR strText,
	COLORREF clrText,
	HFONT hFont,
	DWORD dwDrawTextFlags,
	bool bEnabled,
	const RECT & rcText,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
int nLenText = int( ( strText != NULL ) ? _tcslen(strText) : 0 );
	if(		nLenText == 0
		||	rcText.left >= rcText.right
		||	rcText.top >= rcText.bottom
		||	(! dc.RectVisible( &rcText ) )
		)
		return true;
	if( clrText == COLORREF(-1L) )
	{
		clrText =
			QueryObjectTextColor(
				dc,
				bEnabled,
				false,
				false,
				false,
				pHelperSrc
				);
		if( clrText == COLORREF(-1L) )
			clrText =
				GetColor(
					bEnabled
						? COLOR_BTNTEXT
						: CExtPaintManager::CLR_TEXT_DISABLED,
					pHelperSrc
					);
	}		
//	if( clrText == COLORREF(-1L) )
//	{
		CExtSkinColor & skinColor =
			m_Skin[_T("GroupBox")].Color(
				bEnabled ? _T("Enabled") : _T("Disabled") );
		if( skinColor.IsHollow() )
			return true;
		clrText = COLORREF( skinColor );
//	} // if( clrText == COLORREF(-1L) )
HFONT hDrawFont = ( hFont != NULL ) ? hFont : ( (HFONT) m_FontNormal.GetSafeHandle() );
HGDIOBJ hOldFont = dc.SelectObject( hDrawFont );
INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
COLORREF clrOldText = dc.SetTextColor( clrText );
CRect rc( rcText );
	CExtRichContentLayout::stat_DrawText(
		dc.m_hDC,
		LPCTSTR(strText), int(_tcslen(strText)),
		rc,
		dwDrawTextFlags, 0
		);
	dc.SetBkMode( nOldBkMode );
	dc.SetTextColor( clrOldText );
	dc.SelectObject( hOldFont );
	return true;
}

#if( !defined __EXT_MFC_NO_SPIN || !defined __EXT_MFC_NO_DURATIONWND )

void CExtPaintManagerSkin::PaintSpinButtonArrow(
	CDC & dc,
	const CRect & rcButton,
	bool bUp,		
	bool bHorz,
	bool bEnabled,
	bool bPushed,
	bool bHover,
	bool bHotTrack,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bHotTrack;
	pHelperSrc;
	lParam;

LPCTSTR strState = _T("Disabled");
	if( bEnabled )
	{
		if( bPushed )
			strState = _T("Pressed");
		else if( bHover )
			strState = _T("Hover");
		else
			strState = _T("Normal");
	} // if( bEnabled )

LPCTSTR strHorzVert = NULL, strUpDown = NULL;
	if( bHorz )
	{
		strHorzVert = _T("Horizontal");
		strUpDown = bUp ? _T("Left") : _T("Right");
	} // if( bHorz )
	else
	{
		strHorzVert = _T("Vertical");
		strUpDown = bUp ? _T("Up") : _T("Down");
	} // else from if( bHorz )

const CExtSkinGlyph & _glyph =
	m_Skin[_T("Button")][_T("Spin")][strHorzVert][strUpDown][_T("Arrow")]
		.Glyph(strState);
	_glyph.Draw( dc.m_hDC, rcButton );
}

void CExtPaintManagerSkin::PaintSpinButton(
	CDC & dc,
	const CRect & rcButton,
	bool bUp,		
	bool bHorz,
	bool bEnabled,
	bool bFlat,
	bool bDrawBorder,
	bool bPushed,
	bool bHover,
	bool bHotTrack,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	bFlat;
	bDrawBorder;

LPCTSTR strState = _T("Disabled");
	if( bEnabled )
	{
		if( bPushed )
			strState = _T("Pressed");
		else if( bHover )
			strState = _T("Hover");
		else
			strState = _T("Normal");
	} // if( bEnabled )

LPCTSTR strHorzVert = NULL, strUpDown = NULL;
	if( bHorz )
	{
		strHorzVert = _T("Horizontal");
		strUpDown = bUp ? _T("Left") : _T("Right");
	} // if( bHorz )
	else
	{
		strHorzVert = _T("Vertical");
		strUpDown = bUp ? _T("Up") : _T("Down");
	} // else from if( bHorz )

const CExtSkinGlyph & _glyph =
	m_Skin[_T("Button")][_T("Spin")][strHorzVert][strUpDown][_T("Button")]
		.Glyph(strState);
	_glyph.Draw( dc.m_hDC, rcButton );

CRect rcContentPadding = _glyph.GetContentPadding();
CRect rcArrow = rcButton;
	rcArrow.DeflateRect(
		rcContentPadding.left,
		rcContentPadding.top,
		rcContentPadding.right,
		rcContentPadding.bottom
		);

	PaintSpinButtonArrow(
		dc,
		rcArrow, // rcButton,
		bUp,		
		bHorz,
		bEnabled,
		bPushed,
		bHover,
		bHotTrack,
		pHelperSrc,
		lParam
		);
}

#endif // #if( !defined __EXT_MFC_NO_SPIN || !defined __EXT_MFC_NO_DURATIONWND )

void CExtPaintManagerSkin::PaintControlFrame(
	CDC & dc,
	CExtPaintManager::PAINTCONTROLFRAMEDATA & _pcfd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
CRect rcItem( _pcfd.m_rcClient );
LPCTSTR strControlType = _T("Generic");
	if( _pcfd.m_pHelperSrc != NULL )
	{
		if( _pcfd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtEdit)) )
			strControlType = _T("Edit");
	}
LPCTSTR strState = _T("Disabled");
LPCTSTR strActive = _T("Inactive");
LPCTSTR strHover = _T("Normal");
	if( _pcfd.m_bEnabled )
	{
		strState = _T("Normal");
		if( _pcfd.m_bFocus )
			strActive = _T("Active");
		if( _pcfd.m_bHover )
			strHover = _T("Hover");
	}
const CExtSkinGlyph & _glyph =
		m_Skin[_T("ControlFrame")][strActive][strControlType].Glyph(strState);
	_glyph.Draw( dc.m_hDC, rcItem );

COLORREF clrInner = 
		GetColor( 
			( ! _pcfd.m_bEnabled ) 
				? COLOR_3DFACE 
				: COLOR_WINDOW,
			_pcfd.m_pHelperSrc, 
			_pcfd.m_lParam 
			);
	rcItem.DeflateRect(2,2);
	dc.Draw3dRect( rcItem, clrInner, clrInner );
}

bool CExtPaintManagerSkin::AdjustControlFont(
	HFONT & hFont,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( hFont != NULL );
	hFont;
	pHelperSrc;
	lParam;
//LOGFONT _lf;
//	::memset( &_lf, 0, sizeof(LOGFONT) );
//	::GetObject( (HGDIOBJ)hFont, sizeof(LOGFONT), (LPVOID) &_lf );
//	hFont = ::CreateFontIndirect( &_lf );
//	ASSERT( hFont != NULL );
//	return true;
	return false;
}

DLGTEMPLATE * CExtPaintManagerSkin::AdjustDialogTemplate(
	const DLGTEMPLATE * lpDialogTemplate,
	CObject * pHelperSrc,
	bool bWizard, // = false
	LPARAM lParam // = 0L
	)
{
	ASSERT( lpDialogTemplate != NULL );
	bWizard;
	lParam;
LPCTSTR strSkinnedFontType = NULL;
	if( pHelperSrc != NULL )
	{
		if( pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtResDlg) ) )
		{
			strSkinnedFontType =
				((CExtResDlg *)pHelperSrc) ->
					OnQuerySkinnedFontParmName();
		}
		else if( pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtResPP) ) )
		{
			strSkinnedFontType =
				((CExtResPP *)pHelperSrc) ->
					OnQuerySkinnedFontParmName();
		}
		else if( pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtResPS) ) )
		{
			strSkinnedFontType =
				((CExtResPS *)pHelperSrc) ->
					OnQuerySkinnedFontParmName();
		}
	} // if( pHelperSrc != NULL )
	if( strSkinnedFontType == NULL )
		return NULL;
	ASSERT( _tcslen(strSkinnedFontType) > 0 );
CExtSkinFont & skinFont =
		m_Skin[_T("DialogTemplate")].Font(strSkinnedFontType);
const LOGFONT & lf = skinFont.LogFontGet();
	if( _tcslen(lf.lfFaceName) == 0 )
		return NULL;
	if( lf.lfHeight == 0 )
		return NULL;
CDialogTemplate dlgTemplate( lpDialogTemplate );
	dlgTemplate.SetFont(
		lf.lfFaceName,
		WORD( abs( lf.lfHeight ) )
		);
	return (DLGTEMPLATE*)dlgTemplate.Detach();
}

#if (!defined __EXT_MFC_NO_DATE_PICKER)

void CExtPaintManagerSkin::PaintDatePickerPushButton(
	CDC & dc,
	const CRect & rcButton,
	LONG nButtonType, // __EDPWH_BTN_NONE or __EDPWH_BTN_TODAY
	__EXT_MFC_SAFE_LPCTSTR strCaption,
	bool bFlat,
	bool bDrawBorder,
	bool bPushed,
	bool bHover,
	HFONT hFont,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
CExtMemoryDC dcMem( &dc, &rcButton );
	CExtPaintManager::PaintDatePickerPushButton(
		dcMem,
		rcButton,
		nButtonType,
		strCaption,
		bFlat,
		bDrawBorder,
		bPushed,
		bHover,
		hFont,
		pHelperSrc,
		lParam
		);
}

void CExtPaintManagerSkin::PaintDatePickerClientArea(
	CDC & dc,
	const RECT & rcTotalClient,
	const RECT & rcUsedClient,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( ! dc.RectVisible(&rcTotalClient) )
		return;
	dc.FillSolidRect(
		&rcTotalClient,
		GetColor( CExtPaintManager::CLR_3DFACE_OUT, pHelperSrc, lParam )
		);
	dc.FillSolidRect(
		&rcUsedClient,
		GetColor( COLOR_WINDOW, pHelperSrc, lParam )
		);
}

#endif // (!defined __EXT_MFC_NO_DATE_PICKER)

#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

void CExtPaintManagerSkin::PaintShortcutListItem(
	CDC & dc,
	CExtPaintManager::PAINTSHORTCUTLISTITEMSDATA & _pslid
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
CExtSkinItem * pButton = (&(m_Skin[_T("Button")][_T("Simple")]));
CExtSkinItem * pSkinItem =
		(&((*pButton)
			[_T("PushButton")][_T("ControlBar")][_T("Horizontal")]
			[_T("ToolBar")][_T("MenuBar")][_T("Normal")]
		));
const CExtSkinColor & skinColor =
		pSkinItem->Color( _T("Normal") );
	if( skinColor.IsHollow() )
		m_clrShortcutListItemAlternativeText = COLORREF( -1L );
	else
		m_clrShortcutListItemAlternativeText = COLORREF( skinColor );
	CExtPaintManager::PaintShortcutListItem( dc, _pslid );
}

#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

CExtPaintManager::SCROLLBARSKINDATA * CExtPaintManagerSkin::ScrollBar_GetSkinData(
	bool bHorzBar,
	CObject * pHelperSrc,
	LPARAM lParam, // = 0L
	bool bLightAccent // = true
	)
{
	ASSERT_VALID( this );
	bLightAccent;
	if( IsHighContrast() )
		return
			CExtPaintManager::ScrollBar_GetSkinData(
				bHorzBar,
				pHelperSrc,
				lParam,
				bLightAccent
				);
	if( pHelperSrc != NULL )
	{
		ASSERT_VALID( pHelperSrc );
		if( pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtZoomScrollBar ) ) )
		{
			switch( ((CExtZoomScrollBar*)pHelperSrc)->m_eSO )
			{
			case CExtScrollBar::__ESO_TOP:
			case CExtScrollBar::__ESO_BOTTOM:
				return (&m_Skin.m_SBSD_ZoomH);
			case CExtScrollBar::__ESO_LEFT:
			case CExtScrollBar::__ESO_RIGHT:
				return (&m_Skin.m_SBSD_ZoomV);
			default:
				return bHorzBar ? (&m_Skin.m_SBSD_ZoomH) : (&m_Skin.m_SBSD_ZoomV);
			} // switch( ((CExtZoomScrollBar*)pHelperSrc)->m_eSO )
		} // if( pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtZoomScrollBar ) ) )
	} // if( pHelperSrc != NULL )
	return bHorzBar ? (&m_Skin.m_SBSD_ClassicH) : (&m_Skin.m_SBSD_ClassicV);
}

bool CExtPaintManagerSkin::ScrollBar_GetMetrics(
	INT * p_nHorzBarHeight, // = NULL
	INT * p_nVertBarWidth, // = NULL
	INT * p_nHorzButtonWidth, // = NULL
	INT * p_nVertButtonHeight, // = NULL
	INT * p_nHorzThumbMinWidth, // = NULL
	INT * p_nVertThumbMinHeight, // = NULL
	CObject * pHelperSrc, // = NULL
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
bool bDefaultMetricsRetrived =
		CExtPaintManager::ScrollBar_GetMetrics(
			p_nHorzBarHeight,
			p_nVertBarWidth,
			p_nHorzButtonWidth,
			p_nVertButtonHeight,
			p_nHorzThumbMinWidth,
			p_nVertThumbMinHeight,
			pHelperSrc,
			lParam
			);
	if( IsHighContrast() )
		return bDefaultMetricsRetrived;
	if( ! bDefaultMetricsRetrived )
	{
		if( p_nHorzBarHeight != NULL )
			(*p_nHorzBarHeight) = ::GetSystemMetrics( SM_CYHSCROLL );
		if( p_nVertBarWidth != NULL )
			(*p_nVertBarWidth) = ::GetSystemMetrics( SM_CXVSCROLL );
		if( p_nHorzButtonWidth != NULL )
			(*p_nHorzButtonWidth) = ::GetSystemMetrics( SM_CXHSCROLL );
		if( p_nVertButtonHeight != NULL )
			(*p_nVertButtonHeight) = ::GetSystemMetrics( SM_CYVSCROLL );
	} // if( ! bDefaultMetricsRetrived )
	if( p_nHorzThumbMinWidth != NULL )
	{
		CExtPaintManager::SCROLLBARSKINDATA * pSBSD =
			( const_cast < CExtPaintManagerSkin * > ( this ) ) ->
				ScrollBar_GetSkinData( true, pHelperSrc, lParam, true );
		if( pSBSD != NULL )
		{
			CRect rcPaddingOfThumbButton =
				pSBSD->m_arrPartPadding[INT(__ESBSPT_COLD)][INT(__ESBMHT_THUMB)];
			INT nMinExtent = rcPaddingOfThumbButton.left + rcPaddingOfThumbButton.right;
			if( (*p_nHorzThumbMinWidth) < nMinExtent )
				(*p_nHorzThumbMinWidth) = nMinExtent;
		} // if( pSBSD != NULL )
	} // if( p_nHorzThumbMinWidth != NULL )
	if( p_nVertThumbMinHeight != NULL )
	{
		CExtPaintManager::SCROLLBARSKINDATA * pSBSD =
			( const_cast < CExtPaintManagerSkin * > ( this ) ) ->
				ScrollBar_GetSkinData( false, pHelperSrc, lParam, true );
		if( pSBSD != NULL )
		{
			CRect rcPaddingOfThumbButton =
				pSBSD->m_arrPartPadding[INT(__ESBSPT_COLD)][INT(__ESBMHT_THUMB)];
			INT nMinExtent = rcPaddingOfThumbButton.top + rcPaddingOfThumbButton.bottom;
			if( (*p_nVertThumbMinHeight) < nMinExtent )
				(*p_nVertThumbMinHeight) = nMinExtent;
		} // if( pSBSD != NULL )
	} // if( p_nVertThumbMinHeight != NULL )
	return true;
}

bool CExtPaintManagerSkin::PaintDocumentClientAreaBkgnd(
	CDC & dc,
	CWnd * pWnd,
	LPARAM lParam // = NULL
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pWnd->GetSafeHwnd() != NULL );
	return PaintDockerBkgnd( true, dc, pWnd, lParam );
}

#ifndef __EXT_MFC_NO_GRIDWND

bool CExtPaintManagerSkin::Grid_PaintButton(
	CDC & dc,
	const RECT & rcButton,
	INT nButtonType,
	bool bPressed,
	bool bHovered,
	bool bFocused,
	bool bEnabled,
	COLORREF clrFace,
	COLORREF clrLight,
	COLORREF clrShadow,
	COLORREF clrWindow,
	COLORREF clrText,
	DWORD dwHelperPaintFlags,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( pHelperSrc == NULL )
		return false;
	ASSERT_VALID( pHelperSrc );

	if( IsHighContrast() )
		return 
			CExtPaintManager::Grid_PaintButton(
				dc,
				rcButton,
				nButtonType,
				bPressed,
				bHovered,
				bFocused,
				bEnabled,
				clrFace,
				clrLight,
				clrShadow,
				clrWindow,
				clrText,
				dwHelperPaintFlags,
				pHelperSrc,
				lParam
				);

CRect rc( rcButton );
//	dc.FillSolidRect( &rc, clrWindow );

LPCTSTR strState = _T("Disabled");
	if( bEnabled )
	{
		if( bPressed )
			strState = _T("Pressed");
		else if( bHovered )
			strState = _T("Hover");
		else
			strState = _T("Normal");
	} // if( bEnabled )
CExtSafeString strGlyphBackground = strState;
	strGlyphBackground += _T("-Background");
CExtSafeString strGlyphArrow = strState;
	strGlyphArrow += _T("-Arrow");
LPCTSTR strButtonType = NULL, strArrow = NULL;
CExtBitmap * pBitmap = NULL;
	switch( nButtonType ) 
	{
	case INT(CExtGridCell::__EBTT_DROPDOWN):
		strButtonType = _T("DropDown");
		if( (dwHelperPaintFlags&__EGCPF_NO_BUTTON_DROPDOWN_ARROW) == 0 )
			strArrow = LPCTSTR(strGlyphArrow);
	break;
	case INT(CExtGridCell::__EBTT_ELLIPSIS):
		strButtonType = _T("Ellipsis");
		if( (dwHelperPaintFlags&__EGCPF_NO_BUTTON_ELLIPSIS_DOTS) == 0 )
			strArrow = LPCTSTR(strGlyphArrow);
	break;
	case INT(CExtGridCell::__EBTT_UPDOWN_UP):
		strButtonType = _T("Up");
		strArrow = LPCTSTR(strGlyphArrow);
	break;
	case INT(CExtGridCell::__EBTT_UPDOWN_DOWN):
		strButtonType = _T("Down");
		strArrow = LPCTSTR(strGlyphArrow);
	break;
	case INT(CExtGridCell::__EBTT_FILTER_APPLIED):
		{
			//strButtonType = _T("DropDown");
			CExtBitmap & aBmp = _GetBmpCacheFilter( true );
			pBitmap = &aBmp;
		}
	break;
	case INT(CExtGridCell::__EBTT_FILTER_UNAPPLIED):
		{
			//strButtonType = _T("DropDown");
			CExtBitmap & aBmp = _GetBmpCacheFilter( false );
			pBitmap = &aBmp;
		}
	break;
	} // switch( nButtonType ) 
	if( strButtonType != NULL )
	{
		const CExtSkinGlyph & _glyph =
			m_Skin[_T("Button")][_T("Grid")][strButtonType]
				.Glyph( LPCTSTR(strGlyphBackground) );
		_glyph.Draw( dc.m_hDC, rcButton );
	} // if( strButtonType != NULL )
	if( strArrow != NULL )
	{
		const CExtSkinGlyph & _glyph =
			m_Skin[_T("Button")][_T("Grid")][strButtonType]
				.Glyph( LPCTSTR(strArrow) );
		_glyph.Draw( dc.m_hDC, rcButton );
	} // if( strArrow != NULL )
	if(		pBitmap != NULL
		&&	(! pBitmap->IsEmpty() )
		)
	{
		CRect rcDst( rc.TopLeft(), pBitmap->GetSize() );
		rcDst.OffsetRect(
			( rc.Width() - rcDst.Width() ) / 2,
			( rc.Height() - rcDst.Height() ) / 2
			);
		pBitmap->AlphaBlend( dc.m_hDC, rcDst );
	} // if( pBitmap != NULL ...
	return true;
}

bool CExtPaintManagerSkin::Grid_PaintHeaderSortArrow(
	CDC & dc,
	const RECT & rcSortArrow,
	bool bHorz,
	bool bAscending,
	COLORREF * pColorValues,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( IsHighContrast() )
		return 
			CExtPaintManager::Grid_PaintHeaderSortArrow(
				dc,
				rcSortArrow,
				bHorz,
				bAscending,
				pColorValues,
				pHelperSrc,
				lParam
				);
	if( pColorValues == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
const CExtSkinGlyph & _glyph =
		m_Skin[_T("Grid")][_T("SortArrow")]
			.Glyph(
				bHorz
					? ( bAscending ? _T("AscendingLeftRight") : _T("DescendingLeftRight") )
					: ( bAscending ? _T("AscendingTopBottom") : _T("DescendingTopBottom") )
				);
	_glyph.Draw( dc.m_hDC, rcSortArrow );
	return true;
}

bool CExtPaintManagerSkin::Grid_PaintHeaderBackground(
	CDC & dc,
	const RECT & rcArea,
	LONG nColNo,
	LONG nRowNo,
	INT nColType,
	INT nRowType,
	DWORD dwAreaFlags,
	DWORD dwHelperPaintFlags,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

	if( IsHighContrast() )
		return 
			CExtPaintManager::Grid_PaintHeaderBackground(
				dc,
				rcArea,
				nColNo,
				nRowNo,
				nColType,
				nRowType,
				dwAreaFlags,
				dwHelperPaintFlags,
				pHelperSrc,
				lParam
				);

CExtGridWnd * pGridWnd = ( pHelperSrc != NULL ) ? ( DYNAMIC_DOWNCAST( CExtGridWnd, pHelperSrc ) ) : NULL;
	if( pGridWnd == NULL )
		return false;

bool bHoverByColumn = ( (dwHelperPaintFlags&__EGCPF_HOVER_BY_COLUMN) != 0 ) ? true : false;
bool bHoverByRow = ( (dwHelperPaintFlags&__EGCPF_HOVER_BY_ROW) != 0 ) ? true : false;
bool bHighlightedBySelectedColumn = ( (dwHelperPaintFlags&__EGCPF_HIGHLIGHTED_BY_SELECTED_COLUMN) != 0 ) ? true : false;
bool bHighlightedBySelectedRow = ( (dwHelperPaintFlags&__EGCPF_HIGHLIGHTED_BY_SELECTED_ROW) != 0 ) ? true : false;
bool bHighlightedByFocusedColumn = ( (dwHelperPaintFlags&__EGCPF_HIGHLIGHTED_BY_FOCUSED_COLUMN) != 0 ) ? true : false;
bool bHighlightedByFocusedRow = ( (dwHelperPaintFlags&__EGCPF_HIGHLIGHTED_BY_FOCUSED_ROW) != 0 ) ? true : false;
// bool bFocusedControl = ( (dwHelperPaintFlags&__EGCPF_FOCUSED_CONTROL) != 0 ) ? true : false;
bool bHighlightPressing = ( (dwHelperPaintFlags&(__EGCPF_HIGHLIGHTED_BY_PRESSED_COLUMN|__EGCPF_HIGHLIGHTED_BY_PRESSED_ROW)) != 0 ) ? true : false;

LPCTSTR strOuterAreaLocation = NULL;
	if( nColType == 0 && nRowType == 0 )
	{
		if( nColNo >= 0 && nRowNo >= 0 )
			return false;
		strOuterAreaLocation = _T("GeneralOuterBackground");
	}
	else if( nColType ==  0 && nRowType == -1 )
		strOuterAreaLocation = _T("Top");
	else if( nColType ==  0 && nRowType ==  1 )
		strOuterAreaLocation = _T("Bottom");
	else if( nColType == -1 && nRowType ==  0 )
		strOuterAreaLocation = _T("Left");
	else if( nColType ==  1 && nRowType ==  0 )
		strOuterAreaLocation = _T("Right");
	else if( nColType == -1 && nRowType == -1 )
		strOuterAreaLocation = _T("TopLeft");
	else if( nColType ==  1 && nRowType == -1 )
		strOuterAreaLocation = _T("TopRight");
	else if( nColType == -1 && nRowType ==  1 )
		strOuterAreaLocation = _T("BottomLeft");
	else if( nColType ==  1 && nRowType ==  1 )
		strOuterAreaLocation = _T("BottomRight");
	if( strOuterAreaLocation == NULL )
		return false;
	if( nColNo >= 0L && nRowNo >= 0L ) 
	{
		LPCTSTR strHeaderCellState = _T("Normal");
		if( bHighlightPressing )
			strHeaderCellState = _T("Pressed");
		else if( bHoverByColumn && bHoverByRow )
			strHeaderCellState = _T("Hover");
		else if( bHighlightedByFocusedColumn || bHighlightedByFocusedRow )
			strHeaderCellState = _T("HighlightedByFocus");
		else if( bHighlightedBySelectedColumn || bHighlightedBySelectedRow )
			strHeaderCellState = _T("HighlightedBySelection");
		else if( bHoverByColumn || bHoverByRow )
		{
			if( nColType == 0 || nRowType == 0 )
				strHeaderCellState = _T("HighlightedByHover");
		}
		const CExtSkinGlyph & _glyph =
			m_Skin[_T("Grid")][_T("HeaderCell")][strOuterAreaLocation]
				.Glyph( strHeaderCellState );
		_glyph.Draw( dc.m_hDC, rcArea );
	} // if( nColNo >= 0L && nRowNo >= 0L ) 
	else
	{
		const CExtSkinGlyph & _glyph =
			m_Skin[_T("Grid")][_T("OuterBackground")][strOuterAreaLocation]
				.Glyph( _T("Background") );
		_glyph.Draw( dc.m_hDC, rcArea );
	} // else from if( nColNo >= 0L && nRowNo >= 0L ) 
	return true;
}

COLORREF CExtPaintManagerSkin::Grid_OnQueryTextColor(
	const CExtGridCell & _cell,
	CDC & dc,
	LONG nVisibleColNo,
	LONG nVisibleRowNo,
	LONG nColNo,
	LONG nRowNo,
	INT nColType,
	INT nRowType,
	DWORD dwAreaFlags,
	DWORD dwHelperPaintFlags
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( (&_cell) );
	if( (dwHelperPaintFlags&__EGCPF_PRINTING_TARGET_MASK) != 0 )
		return COLORREF(-1L);
	if( IsHighContrast() )
		return
			CExtPaintManager::Grid_OnQueryTextColor(
				_cell,
				dc,
				nVisibleColNo,
				nVisibleRowNo,
				nColNo,
				nRowNo,
				nColType,
				nRowType,
				dwAreaFlags,
				dwHelperPaintFlags
				);
LPCTSTR strOuterAreaLocation = NULL;
	if( nColType == 0 && nRowType == 0 )
		return COLORREF(-1L);
	else if( nColType ==  0 && nRowType == -1 )
		strOuterAreaLocation = _T("Top");
	else if( nColType ==  0 && nRowType ==  1 )
		strOuterAreaLocation = _T("Bottom");
	else if( nColType == -1 && nRowType ==  0 )
		strOuterAreaLocation = _T("Left");
	else if( nColType ==  1 && nRowType ==  0 )
		strOuterAreaLocation = _T("Right");
	else if( nColType == -1 && nRowType == -1 )
		strOuterAreaLocation = _T("TopLeft");
	else if( nColType ==  1 && nRowType == -1 )
		strOuterAreaLocation = _T("TopRight");
	else if( nColType == -1 && nRowType ==  1 )
		strOuterAreaLocation = _T("BottomLeft");
	else if( nColType ==  1 && nRowType ==  1 )
		strOuterAreaLocation = _T("BottomRight");
	if( strOuterAreaLocation == NULL )
		return COLORREF(-1L);
bool bHoverByColumn = ( (dwHelperPaintFlags&__EGCPF_HOVER_BY_COLUMN) != 0 ) ? true : false;
bool bHoverByRow = ( (dwHelperPaintFlags&__EGCPF_HOVER_BY_ROW) != 0 ) ? true : false;
bool bHighlightedBySelectedColumn = ( (dwHelperPaintFlags&__EGCPF_HIGHLIGHTED_BY_SELECTED_COLUMN) != 0 ) ? true : false;
bool bHighlightedBySelectedRow = ( (dwHelperPaintFlags&__EGCPF_HIGHLIGHTED_BY_SELECTED_ROW) != 0 ) ? true : false;
bool bHighlightedByFocusedColumn = ( (dwHelperPaintFlags&__EGCPF_HIGHLIGHTED_BY_FOCUSED_COLUMN) != 0 ) ? true : false;
bool bHighlightedByFocusedRow = ( (dwHelperPaintFlags&__EGCPF_HIGHLIGHTED_BY_FOCUSED_ROW) != 0 ) ? true : false;
// bool bFocusedControl = ( (dwHelperPaintFlags&__EGCPF_FOCUSED_CONTROL) != 0 ) ? true : false;
bool bHighlightPressing = ( (dwHelperPaintFlags&(__EGCPF_HIGHLIGHTED_BY_PRESSED_COLUMN|__EGCPF_HIGHLIGHTED_BY_PRESSED_ROW)) != 0 ) ? true : false;
LPCTSTR strHeaderCellState = _T("Normal");
	if( bHighlightPressing )
		strHeaderCellState = _T("Pressed");
	else if( bHoverByColumn && bHoverByRow )
		strHeaderCellState = _T("Hover");
	else if( bHighlightedByFocusedColumn || bHighlightedByFocusedRow )
		strHeaderCellState = _T("HighlightedByFocus");
	else if( bHighlightedBySelectedColumn || bHighlightedBySelectedRow )
		strHeaderCellState = _T("HighlightedBySelection");
	else if( bHoverByColumn || bHoverByRow )
		strHeaderCellState = _T("HighlightedByHover");
const CExtSkinColor & colorText =
			m_Skin[_T("Grid")][_T("HeaderCell")][strOuterAreaLocation]
				.Color( strHeaderCellState );
	if( colorText.IsHollow() )
		return COLORREF(-1L);
COLORREF clr = COLORREF( colorText );
	return clr;
}

#endif // #ifndef __EXT_MFC_NO_GRIDWND

#if (!defined __EXT_MFC_NO_REPORTGRIDWND)

bool CExtPaintManagerSkin::ReportGrid_PaintHeaderRowBackground(
	CDC & dc,
	const RECT & rcArea,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
//	dc;
//	rcArea;
//	pHelperSrc;
//	lParam;
//	return false;
	return
		CExtPaintManagerSkin::Grid_PaintHeaderBackground(
			dc,
			rcArea,
			-1L,
			-1L,
			0,
			0,
			0,
			0,
			pHelperSrc,
			lParam
			);
}

bool CExtPaintManagerSkin::ReportGrid_PaintHeaderSortArrow(
	CDC & dc,
	const RECT & rcSortArrow,
	bool bAscending,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

	if( IsHighContrast() )
		return 
			CExtPaintManager::ReportGrid_PaintHeaderSortArrow(
				dc,
				rcSortArrow,
				bAscending,
				pHelperSrc,
				lParam
				);
static COLORREF ColorValues[4] =
{
	RGB(0,0,0),
	RGB(0,0,0),
	RGB(0,0,0),
	RGB(0,0,0),
};
	return Grid_PaintHeaderSortArrow( dc, rcSortArrow, false, bAscending, ColorValues, pHelperSrc, lParam );
}

bool CExtPaintManagerSkin::ReportGrid_PaintGroupAreaBackground(
	CDC & dc,
	const RECT & rcArea,
	__EXT_MFC_SAFE_LPCTSTR strEmptyAreaCaption, // not NULL if area is empty
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

	if( IsHighContrast() )
		return 
			CExtPaintManager::ReportGrid_PaintGroupAreaBackground(
				dc,
				rcArea,
				strEmptyAreaCaption,
				pHelperSrc,
				lParam
				);
bool bSomeThemedBackground = false;
CExtSkinGlyph & glyphBack = m_Skin[_T("Background")][_T("ReportGroupArea")].Glyph( _T("Background") );
	if( glyphBack.IsEmpty() )
	{
		CWnd * pWnd = DYNAMIC_DOWNCAST( CWnd, pHelperSrc );
		if(		pWnd != NULL
			&&	PaintDockerBkgnd( true, dc, pWnd, lParam )
			)
			bSomeThemedBackground = true;
	}
	else
	{
		glyphBack.Draw( dc.m_hDC, rcArea );
		bSomeThemedBackground = true;
	}
	if( bSomeThemedBackground )
	{
		CExtSkinColor & skinColor = m_Skin[_T("Grid")].Color( _T("ReportEmptyMessageColor") );
		if( ! skinColor.IsHollow() )
		{
			INT nTextLen = 0;
			if(		strEmptyAreaCaption != NULL
				&&	( nTextLen = INT( _tcslen( strEmptyAreaCaption ) ) ) > 0
				)
			{
				COLORREF clrEmptyMessageText = COLORREF( skinColor );
				CRect rcCaption = rcArea;
				rcCaption.DeflateRect( 5, 5, 0, 5 );
				int nOldBkMode = dc.SetBkMode( TRANSPARENT );
				COLORREF clrOldTextColor = dc.SetTextColor( clrEmptyMessageText );
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					LPCTSTR(strEmptyAreaCaption), nTextLen,
					rcCaption,
					DT_SINGLELINE|DT_NOCLIP, 0
					);
				dc.SetTextColor( clrOldTextColor );
				dc.SetBkMode( nOldBkMode );
			}
		}
		return true;
	}
	return false;
}

COLORREF CExtPaintManagerSkin::ReportGrid_GetHeaderTextColor(
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	pHelperSrc;
	lParam;
	return COLORREF(-1L);
}

COLORREF CExtPaintManagerSkin::ReportGrid_GetGroupConnectorColor(
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	pHelperSrc;
	lParam;
	CExtSkinColor & skinColor = m_Skin[_T("Grid")].Color( _T("ReportGroupConnector") );
	if( ! skinColor.IsHollow() )
	{
		COLORREF clr = COLORREF( skinColor );
		return clr;
	}
	return COLORREF(-1L);
}

bool CExtPaintManagerSkin::ReportGrid_PaintGroupAreaItemBackground(
	CDC & dc,
	const RECT & rcItem,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	pHelperSrc;
	lParam;
const CExtSkinGlyph & _glyph =
		m_Skin[_T("Grid")][_T("OuterBackground")][_T("ReportGroupAreaItem")]
			.Glyph( _T("Background") );
	_glyph.Draw( dc.m_hDC, rcItem );
	return true;
}

#endif // (!defined __EXT_MFC_NO_REPORTGRIDWND)

CFont * CExtPaintManagerSkin::MenuCaptionGetFont(
	CExtPopupMenuWnd * pPopup,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	ASSERT_VALID( pPopup );
	if( IsHighContrast() )
		return CExtPaintManager::MenuCaptionGetFont( pPopup, lParam );
	return (CFont *)(&m_FontBold);
}

CSize CExtPaintManagerSkin::MenuCaption_Measure(
	CDC & dc,
	__EXT_MFC_SAFE_LPCTSTR sMenuCaptionText,
	CExtPopupMenuWnd * pPopup,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pPopup );
	if( sMenuCaptionText == NULL )
		return CSize( 0, 0 );
CExtSafeString _sText = sMenuCaptionText;
	if( _sText.GetLength() == 0 )
		return CSize( 0, 0 );
	if( IsHighContrast() )
		return
			CExtPaintManager::MenuCaption_Measure(
				dc,
				sMenuCaptionText,
				pPopup,
				lParam
				);
CFont * pFont = MenuCaptionGetFont(	pPopup, lParam );
	ASSERT( pFont->GetSafeHandle() != NULL );
CSize _size =
		stat_CalcTextDimension(
			dc,
			*pFont,
			_sText,
			DT_SINGLELINE|DT_TOP|DT_LEFT
			).Size();
const CExtSkinGlyph & _glyph = m_Skin[_T("MenuCaption")].Glyph( _T("Background") );
const CRect & rcContentPadding = _glyph.GetContentPadding();
	_size.cx += rcContentPadding.left + rcContentPadding.right;
	_size.cy += rcContentPadding.top + rcContentPadding.bottom;
	return _size;
}

void CExtPaintManagerSkin::MenuCaption_Paint(
	CDC & dc,
	const RECT & rcMenuCaption,
	__EXT_MFC_SAFE_LPCTSTR sMenuCaptionText,
	CExtPopupMenuWnd * pPopup,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
//	ASSERT_VALID( pPopup );
	if(		( ! dc.RectVisible( &rcMenuCaption ) )
		//||	sMenuCaptionText == NULL
		)
		return;

	if( IsHighContrast() )
	{
		CExtPaintManager::MenuCaption_Paint(
			dc,
			rcMenuCaption,
			sMenuCaptionText,
			pPopup,
			lParam
			);
		return;
	}

INT nTextLen = ( sMenuCaptionText != NULL ) ? INT( _tcslen( sMenuCaptionText ) ) : 0;
//	if( nTextLen == 0 )
//		return;
CRect _rcMenuCaption = rcMenuCaption;
	if(		_rcMenuCaption.IsRectEmpty()
		||	( ! dc.RectVisible( &_rcMenuCaption ) )
		)
		return;
CRect _rcMenuCaptionBk = _rcMenuCaption;
	//_rcMenuCaptionBk.InflateRect( 2, 2, 2, 0 );
const CExtSkinGlyph & _glyph = m_Skin[_T("MenuCaption")].Glyph( _T("Background") );
	_glyph.Draw( dc.m_hDC, _rcMenuCaptionBk );

const CExtSkinColor & _color = m_Skin[_T("MenuCaption")].Color( _T("TextColor") );
	if( ! _color.IsHollow() )
	{
		COLORREF clrMenuCaptionText = _color;
		const CRect & rcContentPadding = _glyph.GetContentPadding();
		_rcMenuCaption.DeflateRect( &rcContentPadding );
		if( nTextLen != 0 )
		{
			bool bRTL = ( pPopup != NULL ) ? pPopup->OnQueryLayoutRTL() : false;
			UINT nDT = DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_NOCLIP
					| ( bRTL ? DT_RIGHT : DT_LEFT );
			CFont * pFont = MenuCaptionGetFont(	pPopup, lParam );
			ASSERT( pFont->GetSafeHandle() != NULL );
			CFont * pOldFont = dc.SelectObject( pFont );
			COLORREF clrOldText = dc.SetTextColor( clrMenuCaptionText );
			INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
			CExtRichContentLayout::stat_DrawText(
				dc.m_hDC,
				LPCTSTR( sMenuCaptionText ), nTextLen,
				&_rcMenuCaption,
				nDT, 0
				);
			dc.SetBkMode( nOldBkMode );
			dc.SetTextColor( clrOldText );
			dc.SelectObject( pOldFont );
		} // if( nTextLen != 0 )
	} // if( ! _color.IsHollow() )
}

#if (!defined __EXT_MFC_NO_RIBBON_BAR)

bool CExtPaintManagerSkin::Ribbon_OptionsPageBackgroundIsDefault() const
{
	ASSERT_VALID( this );
	return true;
}

CFont * CExtPaintManagerSkin::RibbonGallery_GetItemFont(
	CExtRibbonGalleryWnd * pGalleryWnd,
	LPVOID pGalleryItemData,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pGalleryWnd );

	if( IsHighContrast() )
		return 
			CExtPaintManager::RibbonGallery_GetItemFont(
				pGalleryWnd,
				pGalleryItemData,
				lParam
				);	

CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI =
		(CExtToolBoxWnd::TOOLBOX_ITEM_DATA *)pGalleryItemData;
	if( pTBCI->ItemGetNext( __TBCGN_PARENT ) == pGalleryWnd->ItemGetRoot() )
		return (&m_FontBold);
	else
		return (&m_FontNormal);
}

CSize CExtPaintManagerSkin::RibbonGallery_MeasureItem(
	CDC & dc,
	CExtRibbonGalleryWnd * pGalleryWnd,
	LPVOID pGalleryItemData,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pGalleryWnd );
	return 
		CExtPaintManager::RibbonGallery_MeasureItem(
			dc,
			pGalleryWnd,
			pGalleryItemData,
			lParam
			);	
}

bool CExtPaintManagerSkin::RibbonGallery_DrawItem(
	CDC & dc,
	CExtRibbonGalleryWnd * pGalleryWnd,
	const RECT & rcItem,
	LPVOID pGalleryItemData,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pGalleryWnd );
	if( IsHighContrast() )
		return 
			CExtPaintManager::RibbonGallery_DrawItem(
				dc,
				pGalleryWnd,
				rcItem,
				pGalleryItemData,
				lParam
				);
CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI =
		(CExtToolBoxWnd::TOOLBOX_ITEM_DATA *)pGalleryItemData;
DWORD dwItemStyle = pTBCI->GetItemStyle();
CRect _rcItem = rcItem;
CSize sizeItem = _rcItem.Size();
	if(		(dwItemStyle & __TBWI_VISIBLE) == 0
		||	_rcItem.IsRectEmpty()
		||	(! dc.RectVisible( &_rcItem ) )
		)
		return true;
__EXT_MFC_SAFE_LPCTSTR sText = pTBCI->TextGet();
DWORD dwToolBoxWndStyles = pGalleryWnd->GetToolBoxWndStyle();
bool bTreeBoxes = ( (dwToolBoxWndStyles&__TBWS_TREE_BOXES) != 0 ) ? true : false;
bool bListViewMode = false;
CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Parent = pTBCI->ItemGetNext( __TBCGN_PARENT );
CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_Root = pGalleryWnd->ItemGetRoot();
	ASSERT( pTBCI_Parent != NULL );
	if( pTBCI_Parent != pTBCI_Root )
	{
		bTreeBoxes = false;
		ASSERT( pTBCI_Parent->ItemGetNext( __TBCGN_PARENT ) == pTBCI_Root );
		if( pTBCI_Parent->GetItemStyle() & __TBWI_LISTBOX_VIEW )
			bListViewMode = true;
		LPCTSTR strItemState = _T("Disabled");
		if( (dwItemStyle & __TBWI_ENABLED) != 0 )
		{
			strItemState = _T("Normal");
			if( (dwItemStyle & (__TBWI_PRESSED|__TBWI_SELECTED) ) != 0 )
			{
				if( (dwItemStyle & __TBWI_HOVER) != 0 )
					strItemState = _T("Hover");
				else
					strItemState = _T("Pressed");;
			}
			else if( (dwItemStyle & __TBWI_HOVER) != 0 )
					strItemState = _T("Hover");
		} // else if( _ppbd.m_bEnabled )
		CExtSkinGlyph & _glyph = m_Skin[_T("Button")][_T("RibbonGallery")][strItemState].Glyph( _T("Background") );
		_glyph.Draw( dc.m_hDC, rcItem );
	} // if( pTBCI_Parent != pTBCI_Root )
	else
	{
		bListViewMode = true;
		MenuCaption_Paint(
			dc,
			rcItem,
			sText,
			( pGalleryWnd->GetSafeHwnd() != NULL )
				? ( DYNAMIC_DOWNCAST( CExtPopupMenuWnd, pGalleryWnd->GetParent() ) )
				: NULL
			);
		return true;
	} // else from if( pTBCI_Parent != pTBCI_Root )
bool bMultipleExp = ( (dwToolBoxWndStyles&__TBWS_MULTIPLE_EXPANDED_GROUPS) != 0 ) ? true : false;
bool bAllowSelectionDrawing = true;
	if(		bMultipleExp
		&&	pTBCI->ItemGetNext( __TBCGN_PARENT ) != pGalleryWnd->ItemGetActive()
		)
		bAllowSelectionDrawing = false;

bool bEnabled = (dwItemStyle & __TBWI_ENABLED) ? true : false;
CSize sizeIcon( 0, 0 );
CExtCmdIcon & _icon = pTBCI->IconGet();
	sizeIcon = _icon.GetSize();
	sizeIcon.cx = UiScalingDo( sizeIcon.cx, __EUIST_X );
	sizeIcon.cy = UiScalingDo( sizeIcon.cy, __EUIST_Y );
	if( bListViewMode )
	{
		if( (dwToolBoxWndStyles&(__TBWS_FLAT_GROUPS_2005|__TBWS_PM_BUTTONS)) != 0 )
			_rcItem.DeflateRect( __EXT_TOOLBOXWND_2005_MARGIN_DX, 0 );
		if( bTreeBoxes )
		{
			int nTreeBoxAreaDX = max( sizeItem.cy, __EXT_TOOLBOXWND_TREE_BOX_AREA_DY );
			CRect rcTreeBox = _rcItem;
			rcTreeBox.right = rcTreeBox.left + nTreeBoxAreaDX;
			bool bPlus = true;
			if( bMultipleExp )
			{
				if( (dwItemStyle&__TBWI_EXPANDED) != 0 )
					bPlus = false;
			} // if( bMultipleExp )
			else
			{
				if( (dwItemStyle&__TBWI_ACTIVE) != 0 )
					bPlus = false;
			} // else from if( bMultipleExp )
			pGalleryWnd->OnToolBoxWndDrawTreeBox(
				pTBCI,
				_rcItem,
				rcTreeBox,
				(! bPlus ),
				dc
				);
			_rcItem.left += nTreeBoxAreaDX;
		} // if( bTreeBoxes )
		CRect rcText = _rcItem;
		rcText.DeflateRect(
			__EXT_TOOLBOXWND_ITEM_MARGIN_DX,
			__EXT_TOOLBOXWND_ITEM_MARGIN_DY
			);
		if( ( ! _icon.IsEmpty() ) && sizeIcon.cx > 0 && sizeIcon.cy > 0 )
		{
			CRect rcIcon = rcText;
			rcIcon.right = rcIcon.left + sizeIcon.cx;
			rcText.left += sizeIcon.cx + __EXT_TOOLBOXWND_ICON2TEXT_DX;
			rcIcon.top += (rcIcon.Height() - sizeIcon.cy) / 2;
			_icon.Paint(
				this,
				dc.GetSafeHdc(),
				rcIcon,
				bEnabled
					? CExtCmdIcon::__PAINT_NORMAL
					: CExtCmdIcon::__PAINT_DISABLED
				);
		}
		if(		(rcText.right - rcText.left > 6)
			&&	(rcText.bottom - rcText.top > 6)
			)
		{
			INT nTextLen = 0;
			if(		sText != NULL
				&&	( nTextLen = INT(_tcslen(sText)) ) > 0
				)
			{
				CFont * pOldFont = dc.SelectObject( pTBCI->ItemGetFont() );
				int nOldBkMode = dc.SetBkMode( TRANSPARENT );

				CExtSkinItem & _item =  m_Skin[_T("MenuItem")][_T("MenuItem")][_T("Normal")];
				COLORREF clrText = COLORREF( _item.Color( _T("TextColor") ) );
				COLORREF clrTextOld = dc.SetTextColor( clrText );
				CRect _rcText = rcText;
				_rcText.OffsetRect( 0, -1 );
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					sText, nTextLen,
					_rcText,
					DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS, 0
					);
				dc.SetTextColor( clrTextOld );

				dc.SetBkMode( nOldBkMode );
				dc.SelectObject( pOldFont );
			}
		}
	} // if( bListViewMode )
	else
	{
		if( ( ! _icon.IsEmpty() ) && sizeIcon.cx > 0 && sizeIcon.cy > 0 )
		{
			CRect rcIcon = _rcItem;
			rcIcon.DeflateRect(
				(rcIcon.Width() - sizeIcon.cx) / 2,
				(rcIcon.Height() - sizeIcon.cy) / 2
				);
			rcIcon.right = rcIcon.left + sizeIcon.cx;
			rcIcon.bottom = rcIcon.top + sizeIcon.cy;
			_icon.Paint(
				this,
				dc.GetSafeHdc(),
				rcIcon,
				bEnabled
					? CExtCmdIcon::__PAINT_NORMAL
					: CExtCmdIcon::__PAINT_DISABLED 
				);
		}
	} // else from if( bListViewMode )
CWnd * pWnd = pGalleryWnd->GetParent();
	if(		pWnd != NULL
		&&	pWnd->IsKindOf( RUNTIME_CLASS( CExtRibbonGalleryPopupMenuWnd ) )
		&&	(((CExtRibbonGalleryPopupMenuWnd*)pWnd)->TrackFlagsGet()&TPMX_RIBBON_FILE_MENU)
		)
	{
		CRect rcClient;
		pGalleryWnd->GetClientRect( &rcClient );
		dc.FillSolidRect( rcClient.left, rcClient.top, 1, rcClient.Height(), RGB(197,197,197) );
		dc.FillSolidRect( rcClient.left+1, rcClient.top, 1, rcClient.Height(), RGB(245,245,245) );
	}
	return true;
}

bool CExtPaintManagerSkin::RibbonGallery_EraseEntireBk(
	CDC & dc,
	const CRect & rcClient,
	const CRect & rcActiveChildArea,
	CRgn & rgnActiveChildArea,
	const CRect & rcActiveChildArea2,
	CRgn & rgnActiveChildArea2,
	CExtRibbonGalleryWnd * pGalleryWnd,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pGalleryWnd );
	return 
		CExtPaintManager::RibbonGallery_EraseEntireBk(
			dc,
			rcClient,
			rcActiveChildArea,
			rgnActiveChildArea,
			rcActiveChildArea2,
			rgnActiveChildArea2,
			pGalleryWnd,
			lParam
			);	
}

void CExtPaintManagerSkin::RibbonGallery_PaintScrollArea(
	CDC & dc,
	CExtRibbonGalleryInplaceScrollBar * pSB,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pSB );
	ASSERT( pSB->GetSafeHwnd() != NULL );

	if( IsHighContrast() )
	{
		CExtPaintManager::RibbonGallery_PaintScrollArea(
			dc,
			pSB,
			lParam
			);
		return;
	}

	PaintDockerBkgnd( true, dc, pSB, lParam );

CRect rcClient;
	pSB->GetClientRect( &rcClient );

bool bKeyFocusMode = false;
CExtRibbonGalleryWnd * pRibbonGalleryWnd =
		STATIC_DOWNCAST( CExtRibbonGalleryWnd, pSB->GetParent() );
	if(		pRibbonGalleryWnd->m_bInPlaceGallery
		&&	pRibbonGalleryWnd->_IsKeyFocusGallery()
		)
		bKeyFocusMode = true;
INT nIndex, nCount = sizeof(pSB->m_arrButtons) / sizeof(pSB->m_arrButtons[0]);
INT nNextTop = 0;
	for( nIndex = 0; nIndex < nCount; nIndex ++ )
	{
		bool bForceHover = false;
		CRect rc = pSB->RgBtnGetRect( (CExtRibbonGalleryInplaceScrollBar::e_ButtonType_t)nIndex );
		INT nBtnHeightSrc = -1;
		INT nBtnHeightDst = -1; // UiScalingDo( nBtnHeightSrc, __EUIST_X );
		LPCTSTR strSubPath = NULL;
		switch( nIndex )
		{
		case INT(CExtRibbonGalleryInplaceScrollBar::__BTT_UP):
			strSubPath = _T("ButtonUp");
			if( nBtnHeightSrc < 0 )
			{
				nBtnHeightSrc = 21;
				nBtnHeightDst = UiScalingDo( nBtnHeightSrc, __EUIST_X );
			}
		break;
		case INT(CExtRibbonGalleryInplaceScrollBar::__BTT_DOWN):
			strSubPath = _T("ButtonDown");
			if( nBtnHeightSrc < 0 )
			{
				nBtnHeightSrc = 19;
				nBtnHeightDst = UiScalingDo( nBtnHeightSrc, __EUIST_X );
			}
		break;
		case INT(CExtRibbonGalleryInplaceScrollBar::__BTT_MENU):
			strSubPath = _T("ButtonMenu");
			if( nBtnHeightSrc < 0 )
			{
				nBtnHeightSrc = 20;
				nBtnHeightDst = UiScalingDo( nBtnHeightSrc, __EUIST_X );
			}
			if( bKeyFocusMode )
				bForceHover = true;
		break;
		} // switch( nIndex )
		if( strSubPath == NULL )
			continue;
		ASSERT( nBtnHeightSrc > 0 );
		ASSERT( nBtnHeightDst > 0 );
		LPCTSTR strState = _T("Normal");
		rc.top = nNextTop;
		rc.bottom = rc.top + nBtnHeightDst;
		CExtRibbonGalleryInplaceScrollBar::e_ButtonState_t eBST = pSB->m_arrButtons[ nIndex ];
		if( bForceHover )
			eBST = CExtRibbonGalleryInplaceScrollBar::__BST_HOVER;
		switch( eBST )
		{
		case CExtRibbonGalleryInplaceScrollBar::__BST_DISABLED:
			strState = _T("Disabled");
		break;
		case CExtRibbonGalleryInplaceScrollBar::__BST_NORMAL:
		break;
		case CExtRibbonGalleryInplaceScrollBar::__BST_HOVER:
			strState = _T("Hover");
		break;
		case CExtRibbonGalleryInplaceScrollBar::__BST_PRESSED:
			strState = _T("Pressed");
		break;
		} // switch( eBST )
		CExtSkinGlyph & _glyphButtonBackground = m_Skin[_T("RibbonScrollBar")][strSubPath][strState].Glyph(_T("Background"));
		CExtSkinGlyph & _glyphButtonArrow = m_Skin[_T("RibbonScrollBar")][strSubPath][strState].Glyph(_T("Arrow"));
		if( rc.bottom > rcClient.bottom )
			rc.bottom = rcClient.bottom;
		_glyphButtonBackground.Draw( dc.m_hDC, rc );
		_glyphButtonArrow.Draw( dc.m_hDC, rc );
		nNextTop = rc.bottom;
	}
}

bool CExtPaintManagerSkin::Ribbon_GroupCaptionIsAtTop(
	const CExtRibbonButtonGroup * pGroupTBB,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	pGroupTBB;
	lParam;
	return false;
}

void CExtPaintManagerSkin::Ribbon_PaintGroupBk(
	CDC & dc,
	const RECT & rcGroupBk,
	const CExtRibbonButtonGroup * pGroupTBB,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	if( IsHighContrast() )
	{
		CExtPaintManager::Ribbon_PaintGroupBk( dc, rcGroupBk, pGroupTBB, lParam );
		return;
	}
// 	if( pGroupTBB->ParentButtonGet() != NULL )
// 		return;
// 	if( pGroupTBB->GetBar()->GetParent()->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) ) )
// 		return;
// CExtSkinGlyph & _glyph = m_Skin[_T("Ribbon")][_T("Group")].Glyph(_T("Background"));
// 	_glyph.Draw( dc.m_hDC, rcGroupBk );

	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT_VALID( pGroupTBB );
	if( pGroupTBB->ParentButtonGet() != NULL )
		return;
	if( ::IsRectEmpty( &rcGroupBk ) )
		return;
	if( ! dc.RectVisible( &rcGroupBk ) )
		return;
CRect _rcGroupBk = rcGroupBk;
	_rcGroupBk.InflateRect( 0, 0, 1, 1 );
CExtSafeString strCaptionText = pGroupTBB->GetText();
INT nCaptionTextLength = strCaptionText.GetLength();
bool bEnabled = pGroupTBB->IsEnabled();
bool bHover = ( bEnabled && pGroupTBB->IsHover() ) ? true : false;
	if( bHover && pGroupTBB->GetBar()->GetMenuTrackingButton() >= 0 )
		bHover = false;
bool bCollapsed = pGroupTBB->TopCollapsedStateGet();
	if( bCollapsed )
	{
		bool bPressed = false;
		if( bEnabled )
			bPressed = pGroupTBB->IsPressed();
		PAINTPUSHBUTTONDATA _ppbd;
		_ppbd.m_rcClient = _rcGroupBk;
		_ppbd.m_bFlat = false;
		_ppbd.m_bEnabled = bEnabled;
		_ppbd.m_bHover = bHover;
		_ppbd.m_bPushed = bPressed;
		_ppbd.m_bHorz = true;
		PaintPushButton( dc, _ppbd );
		CExtCmdIcon * pCmdIcon = Ribbon_GetGroupCollapsedIcon( pGroupTBB, lParam );
		//CExtCmdIcon * pCmdIcon = ((CExtBarButton*)pGroupTBB)->GetIconPtr();
		CRect rcIconArea = _rcGroupBk, rcTextArea = _rcGroupBk;
		if( pCmdIcon != NULL && (! pCmdIcon->IsEmpty() ) )
		{
			CSize _sizeIcon = pCmdIcon->GetSize();
			rcIconArea.left += ( rcIconArea.Width() - _sizeIcon.cx ) / 2;
			rcIconArea.top += ( rcIconArea.Height() - _sizeIcon.cy ) / 2 - 6;
			rcIconArea.right = rcIconArea.left + _sizeIcon.cx;
			rcIconArea.bottom = rcIconArea.top + _sizeIcon.cy;
			rcTextArea.top = rcIconArea.bottom + 7;
		} // if( pCmdIcon != NULL && (! pCmdIcon->IsEmpty() ) )
		else
			rcTextArea.top += 16;
		rcIconArea.OffsetRect( 0, -10 );
		if( pCmdIcon != NULL && (! pCmdIcon->IsEmpty() ) )
		{
			pCmdIcon->Paint(
				this,
				dc,
				rcIconArea.left,
				rcIconArea.top,
				-1,
				-1
				);
		} // if( pCmdIcon != NULL && (! pCmdIcon->IsEmpty() ) )
		if( nCaptionTextLength > 0 )
		{
			LPCTSTR strState = _T("Normal");
			if( bPressed )
				strState = _T("Pressed");
			else if( bHover )
				strState = _T("Hover");
 			const CExtSkinColor & skinTextColor = m_Skin[_T("Ribbon")][_T("Group")][_T("Collapsed")][strState].Color( _T("RibbonGroupCaptionTextColor") );
			if( ! skinTextColor.IsHollow() )
			{
				CExtRibbonPage * pRibbonPage = const_cast < CExtRibbonPage * > ( pGroupTBB->GetRibbonPage() );
				ASSERT_VALID( pRibbonPage );
				ASSERT( pRibbonPage->GetSafeHwnd() != NULL );
				CFont * pFont = pRibbonPage->OnGetToolbarFont( false, false, const_cast < CExtRibbonButtonGroup * > ( pGroupTBB ) );
				ASSERT( pFont->GetSafeHandle() != NULL );
				//CFont * pOldFont = dc.SelectObject( pFont );
				CFont * pOldFont = dc.SelectObject( &m_FontRibbonGroupCaption );
				INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
				COLORREF clrOldTextColor = dc.SetTextColor( COLORREF(skinTextColor) );
				CRect rcCaption = rcTextArea;
	rcCaption.OffsetRect( 0, -5 );
				Ribbon_PaintText(
					dc,
					LPCTSTR( strCaptionText ),
					rcCaption,
					DT_CENTER|DT_TOP,
					true,
					NULL,
					(CObject*)pGroupTBB,
					lParam
					);
				dc.SetTextColor( clrOldTextColor );
				dc.SetBkMode( nOldBkMode );
				dc.SelectObject( pOldFont );
			} // if( ! skinTextColor.IsHollow() )
		} // if( nCaptionTextLength > 0 )
	} // if( bCollapsed )
	else
	{
		const CExtRibbonPage * pRibbonPage = pGroupTBB->GetRibbonPage();
		ASSERT_VALID( pRibbonPage );
		CWnd * pWndParent = pRibbonPage->GetParent();
		bool bDrawCaption = true, bPopupMode =
				(	pWndParent != NULL
				&&	pWndParent->IsKindOf( RUNTIME_CLASS( CExtRibbonPopupMenuWnd ) )
				&&	(! ((CExtRibbonPopupMenuWnd*)pWndParent)->m_wndRibbonPage.m_bHelperAutoHideMode )
				) ? true : false;
		INT nCaptionHeight = 0;
		if( bPopupMode && (! Ribbon_IsPopupGroupWithCaption( (CObject*) pRibbonPage ) ) )
		{
			bDrawCaption = false;
			strCaptionText.Empty();
			nCaptionTextLength = 0;
		}
		else
		{
			strCaptionText.Replace( _T("\r"), _T(" ") );
			strCaptionText.Replace( _T("\n"), _T(" ") );
			strCaptionText.Replace( _T("\t"), _T(" ") );
			strCaptionText.Replace( _T("  "), _T(" ") );
			strCaptionText.TrimLeft( _T(" ") );
			strCaptionText.TrimRight( _T(" ") );
			nCaptionHeight = Ribbon_GroupCaptionGetHeight( pGroupTBB, lParam );
			ASSERT( nCaptionHeight >= 0 );
		}
// 		COLORREF clrBorder = RGB(0,0,0);
// 		if( bEnabled )
// 		{
// 			if( bHover )
// 			{
// 				clrBorder = GetColor( COLOR_3DDKSHADOW );
// 				dc.FillRect( &_rcGroupBk, &m_brushLighterDefault );
// 			} // if( bHover )
// 			else
// 				clrBorder = GetColor( COLOR_3DSHADOW );
// 		} // if( bEnabled )
// 		else
// 			clrBorder = GetColor( COLOR_3DSHADOW );
		if( nCaptionHeight > 0 && nCaptionTextLength > 0 )
		{
			CRect rcCaption = _rcGroupBk;
			bool bTopCaption = Ribbon_GroupCaptionIsAtTop( pGroupTBB );
			if( bTopCaption )
				rcCaption.bottom = rcCaption.top + nCaptionHeight;
			else
				rcCaption.top = rcCaption.bottom - nCaptionHeight;
// 			dc.FillSolidRect( &rcCaption, GetColor( COLOR_3DSHADOW ) );
// 			dc.Draw3dRect( &rcCaption, clrBorder, clrBorder );

			if( pGroupTBB->ParentButtonGet() != NULL )
				return;
			if( pGroupTBB->GetBar()->GetParent()->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) ) )
				return;
			CExtSkinGlyph & _glyph = m_Skin[_T("Ribbon")][_T("Group")].Glyph(_T("Background"));
			_glyph.Draw( dc.m_hDC, rcGroupBk );

			CExtRibbonPage * pRibbonPage = const_cast < CExtRibbonPage * > ( pGroupTBB->GetRibbonPage() );
			ASSERT_VALID( pRibbonPage );
			ASSERT( pRibbonPage->GetSafeHwnd() != NULL );
			CFont * pFont = pRibbonPage->OnGetToolbarFont( false, false, const_cast < CExtRibbonButtonGroup * > ( pGroupTBB ) );
			ASSERT( pFont->GetSafeHandle() != NULL );

			CRect rcCaptionTextAlignmentDLB =
				pGroupTBB->GetCaptionTextAlignmentRect();
			if( ! rcCaptionTextAlignmentDLB.IsRectEmpty() )
			{
				rcCaption.left =
					max(
						rcCaption.left,
						rcCaptionTextAlignmentDLB.left
						);
				rcCaption.right =
					min(
						rcCaption.right,
						rcCaptionTextAlignmentDLB.right
						);
			} // if( ! rcCaptionTextAlignmentDLB.IsRectEmpty() )

///			CExtSkinColor & skinTextColor = m_Skin[_T("Button")][_T("Simple")][_T("Control")].Color( _T("Normal") );
			LPCTSTR strState = _T("Normal");
			if( bHover )
				strState = _T("Hover");
			const CExtSkinColor & skinTextColor = m_Skin[_T("Ribbon")][_T("Group")][_T("Expanded")][strState].Color( _T("RibbonGroupCaptionTextColor") );
			if( ! skinTextColor.IsHollow() )
			{
				CFont * pOldFont = dc.SelectObject( &m_FontRibbonGroupCaption );
				INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
				COLORREF clrOldTextColor = dc.SetTextColor( COLORREF(skinTextColor) );
				UINT nRibbonGroupCaptionTextShadowDF = DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS;
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					LPCTSTR( strCaptionText ), nCaptionTextLength,
					&rcCaption,
					nRibbonGroupCaptionTextShadowDF, 0
					);
				dc.SetTextColor( clrOldTextColor );
				dc.SetBkMode( nOldBkMode );
				dc.SelectObject( pOldFont );
			}
		} // if( nCaptionHeight > 0 && nCaptionTextLength > 0 )
	} // else from if( bCollapsed )
}

void CExtPaintManagerSkin::Ribbon_PaintPageBk(
	CDC & dc,
	const RECT & rcPageBk,
	const CExtRibbonPage * pRibbonPage,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	if( IsHighContrast() )
	{
		CExtPaintManager::Ribbon_PaintPageBk( dc, rcPageBk, pRibbonPage, lParam );
		return;
	}
	if( pRibbonPage->GetParent()->IsKindOf( RUNTIME_CLASS(CExtPopupMenuWnd) ) )
		return;
CExtSkinGlyph & _glyph = m_Skin[_T("Ribbon")][_T("Page")].Glyph(_T("Background"));
	_glyph.Draw( dc.m_hDC, rcPageBk );
}

void CExtPaintManagerSkin::Ribbon_PaintPushButton(
	CDC & dc,
	CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( _ppbd.m_rcClient.IsRectEmpty() )
		return;
	if( ! dc.RectVisible( &_ppbd.m_rcClient ) )
		return;
	if( IsHighContrast() )
	{
		CExtPaintManager::Ribbon_PaintPushButton( dc, _ppbd );
		return;
	}
	ASSERT_VALID( _ppbd.m_pHelperSrc );
CExtBarButton * pTBB = DYNAMIC_DOWNCAST( CExtBarButton, _ppbd.m_pHelperSrc );
	ASSERT( pTBB != NULL );
CExtBarButton * pParentTBB = pTBB->ParentButtonGet();
CExtRibbonButton * pRibbonTBB = DYNAMIC_DOWNCAST( CExtRibbonButton, _ppbd.m_pHelperSrc );
CExtSafeString strText = pTBB->GetText();
INT nTextLength = strText.GetLength();
bool bDLB = false;
	if(		nTextLength == 0
		&&	_ppbd.m_pHelperSrc != NULL
		&&	_ppbd.m_pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtRibbonButtonDialogLauncher ) )
		)
	{
		bDLB = true;
		strText = _T("...");
		nTextLength = strText.GetLength();
	}
enum e_layout_case_t
{
	__ELC_TOOL,
	__ELC_SMALL,
	__ELC_LARGE,
};
e_layout_case_t eLC = __ELC_TOOL;
	if( bDLB )
		eLC = __ELC_SMALL;
	else if( pRibbonTBB != NULL )
	{
		if(		pParentTBB == NULL
			||	pParentTBB->IsKindOf( RUNTIME_CLASS( CExtRibbonButtonToolGroup ) )
			)
		{
			if( pParentTBB == NULL )
			{
				const CExtRibbonNode * pRibbonNode = pTBB->Ribbon_GetNode();
				if( pRibbonNode != NULL )
				{
					const CExtCustomizeCmdTreeNode * pParentNode = pRibbonNode->GetParentNode();
					if(		pParentNode != NULL
						&&	(	pParentNode->IsKindOf( RUNTIME_CLASS( CExtRibbonNodeRightButtonsCollection ) )
							||	pParentNode->IsKindOf( RUNTIME_CLASS( CExtRibbonNodeQuickAccessButtonsCollection ) )
							)
						)
					eLC = __ELC_SMALL;
				} // if( pRibbonNode != NULL )
			} // if( pParentTBB == NULL )
		}
		else
		{
			if( pRibbonTBB->RibbonILV_Get() == __EXT_RIBBON_ILV_SIMPLE_LARGE )
				eLC = __ELC_LARGE;
			else
				eLC = __ELC_SMALL;
		}
	} // else if( pRibbonTBB != NULL )
	else
	{
		if(		pParentTBB == NULL
			||	( ! pParentTBB->IsKindOf( RUNTIME_CLASS( CExtRibbonButtonToolGroup ) ) )
			)
			eLC = __ELC_SMALL;
	} // if( pRibbonTBB != NULL )

//_ppbd.m_bDropDown = true;
//_ppbd.m_bSeparatedDropDown = true;
//_ppbd.m_bPushedDropDown = true;

INT nILV = pTBB->RibbonILV_Get();
CRect rcCP = pTBB->OnRibbonGetContentPadding();
CRect rcIcon = _ppbd.m_rcClient;
	rcIcon.DeflateRect(
		rcCP.left,
		rcCP.top,
		rcCP.right,
		rcCP.bottom
		);
INT nTextToIconDistance = 0;
CRect rcText =  rcIcon;
CSize _sizeIcon( 0, 0 );
CExtCmdIcon * pCmdIcon = pTBB->GetIconPtr();
	if( pCmdIcon != NULL && (! pCmdIcon->IsEmpty() ) )
	{
		_sizeIcon = Ribbon_GetIconSize( pTBB, nILV, _ppbd.m_lParam );
		if( nTextLength >= 0 )
			nTextToIconDistance = Ribbon_GetTextToIconDistance( dc, nILV, pTBB, _ppbd.m_lParam );
		if( eLC == __ELC_LARGE )
		{
			rcIcon.bottom = rcIcon.top + _sizeIcon.cy;
			rcIcon.OffsetRect( ( rcIcon.Width() - _sizeIcon.cx ) / 2, 4 );
			if( nTextLength > 0 )
			{
				rcText.top = rcIcon.bottom;
				rcText.top += nTextToIconDistance;
			};
		} // if( eLC == __ELC_LARGE )
		else
		{
			if( nTextLength > 0 )
			{
				rcIcon.right = rcText.left = rcIcon.left + _sizeIcon.cx;
				rcText.left += nTextToIconDistance;
			}
			else
			{
				rcIcon = _ppbd.m_rcClient;
				if(		pTBB->IsAbleToTrackMenu()
					//&&	_ppbd.m_bSeparatedDropDown
					&&	(! pTBB->IsKindOf( RUNTIME_CLASS( CExtRibbonButtonGroup ) ) )
					)
					rcIcon.right -= GetDropDownButtonWidth( _ppbd.m_pHelperSrc, _ppbd.m_lParam );
				rcIcon.OffsetRect( ( rcIcon.Width() - _sizeIcon.cx ) / 2 + 1, 0 );
			}
			rcIcon.OffsetRect( 0, ( rcIcon.Height() - _sizeIcon.cy ) / 2 );
		} // else from if( eLC == __ELC_LARGE )
	} // if( pCmdIcon != NULL && (! pCmdIcon->IsEmpty() ) )
	else
		pCmdIcon = NULL;

const CExtSkinColor * pSkinTextColor = NULL;
bool bDrawDropDownWithText = _ppbd.m_bDropDown;
	switch( eLC )
	{
	case __ELC_LARGE:
		ASSERT_VALID( pTBB );
		rcText = Ribbon_CalcLargeDropDownRect( pTBB );
		if( ! pTBB->IsKindOf( RUNTIME_CLASS( CExtRibbonButton ) ) )
			bDrawDropDownWithText = false;
		if( _ppbd.m_bSeparatedDropDown )
		{
			LPCTSTR strState1 = _T("Normal");
						if( ! _ppbd.m_bEnabled )
							strState1 = _T("Disabled");
						else if( ( _ppbd.m_bPushed && (!_ppbd.m_bPushedDropDown) ) || _ppbd.m_bIndeterminate )
							strState1 = _T("Pressed");
						else if( _ppbd.m_bPushed && _ppbd.m_bPushedDropDown )
							strState1 = _T("Hover");
						else if( _ppbd.m_bHover && (!_ppbd.m_bHoverDropDown) )
							strState1 = _T("Hover");
						else if( _ppbd.m_bHover && _ppbd.m_bHoverDropDown )
							strState1 = _T("HoverNP");
			LPCTSTR strState2 = _T("Normal");
						if( ! _ppbd.m_bEnabled )
							strState2 = _T("Disabled");
						else if( _ppbd.m_bPushed || _ppbd.m_bIndeterminate )
							strState2 = _T("Pressed");
						else if( _ppbd.m_bHover && _ppbd.m_bHoverDropDown )
							strState2 = _T("Hover");
						else if( _ppbd.m_bHover && (!_ppbd.m_bHoverDropDown) )
							strState2 = _T("HoverNP");
			CRect rcB = _ppbd.m_rcClient;
			CRect rcD = _ppbd.m_rcClient;
			rcB.bottom = rcD.top = rcText.top;
			const CExtSkinGlyph & _glyphB = m_Skin[_T("Button")][_T("RibbonButton")][_T("Large")][_T("SplitButton")][_T("PushButton")][strState1].Glyph( _T("Background") );
			_glyphB.Draw( dc.m_hDC, rcB );
			const CExtSkinGlyph & _glyphD = m_Skin[_T("Button")][_T("RibbonButton")][_T("Large")][_T("SplitButton")][_T("DropDownButton")][strState2].Glyph( _T("Background") );
			_glyphD.Draw( dc.m_hDC, rcD );
			const CExtSkinColor & skinTextColor = m_Skin[_T("Button")][_T("RibbonButton")][_T("Large")][_T("SplitButton")][_T("DropDownButton")][strState2].Color( _T("RibbonButtonTextColor") );
			pSkinTextColor = &skinTextColor;
		}
		else
		{
			LPCTSTR strState = _T("Normal");
			if( ! _ppbd.m_bEnabled )
				strState = _T("Disabled");
			else if( _ppbd.m_bPushed || _ppbd.m_bIndeterminate )
				strState = _T("Pressed");
			else if( _ppbd.m_bHover )
				strState = _T("Hover");
			const CExtSkinGlyph & _glyph = m_Skin[_T("Button")][_T("RibbonButton")][_T("Large")][ _ppbd.m_bDropDown ? _T("DropDownButton") : _T("PushButton") ][strState].Glyph( _T("Background") );
			_glyph.Draw( dc.m_hDC, _ppbd.m_rcClient );
			const CExtSkinColor & skinTextColor = m_Skin[_T("Button")][_T("RibbonButton")][_T("Large")][ _ppbd.m_bDropDown ? _T("DropDownButton") : _T("PushButton") ][strState].Color( _T("RibbonButtonTextColor") );
			pSkinTextColor = &skinTextColor;
		}
	break; // case __ELC_LARGE
	case __ELC_TOOL:
	case __ELC_SMALL:
	{
		LPCTSTR strBT = ( eLC == __ELC_TOOL ) ? _T("Tool") : _T("Small");
		if( _ppbd.m_bSeparatedDropDown )
		{
			LPCTSTR strState1 = _T("Normal");
						if( ! _ppbd.m_bEnabled )
							strState1 = _T("Disabled");
						else if( ( _ppbd.m_bPushed && (!_ppbd.m_bPushedDropDown) ) || _ppbd.m_bIndeterminate )
							strState1 = _T("Pressed");
						else if( _ppbd.m_bPushed && _ppbd.m_bPushedDropDown )
							strState1 = _T("Hover");
						else if( _ppbd.m_bHover && (!_ppbd.m_bHoverDropDown) )
							strState1 = _T("Hover");
						else if( _ppbd.m_bHover && _ppbd.m_bHoverDropDown )
							strState1 = _T("HoverNP");
			LPCTSTR strState2 = _T("Normal");
						if( ! _ppbd.m_bEnabled )
							strState2 = _T("Disabled");
						else if( _ppbd.m_bPushed || _ppbd.m_bIndeterminate )
							strState2 = _T("Pressed");
						else if( _ppbd.m_bHover && _ppbd.m_bHoverDropDown )
							strState2 = _T("Hover");
						else if( _ppbd.m_bHover && (!_ppbd.m_bHoverDropDown) )
							strState2 = _T("HoverNP");
			INT nDropDownButtonWidth = GetDropDownButtonWidth( _ppbd.m_pHelperSrc, _ppbd.m_lParam );
			CRect rcB = _ppbd.m_rcClient;
			CRect rcD = _ppbd.m_rcClient;
			rcB.right -= nDropDownButtonWidth;
			rcD.left = rcB.right;
			const CExtSkinGlyph & _glyphB = m_Skin[_T("Button")][_T("RibbonButton")][strBT][_T("SplitButton")][_T("PushButton")][strState1].Glyph( _T("Background") );
			_glyphB.Draw( dc.m_hDC, rcB );
			const CExtSkinGlyph & _glyphD = m_Skin[_T("Button")][_T("RibbonButton")][strBT][_T("SplitButton")][_T("DropDownButton")][strState2].Glyph( _T("Background") );
			_glyphD.Draw( dc.m_hDC, rcD );
			const CExtSkinColor & skinTextColor = m_Skin[_T("Button")][_T("RibbonButton")][strBT][_T("SplitButton")][_T("PushButton")][strState1].Color( _T("RibbonButtonTextColor") );
			pSkinTextColor = &skinTextColor;
		}
		else
		{
			LPCTSTR strState = _T("Normal");
			if( ! _ppbd.m_bEnabled )
				strState = _T("Disabled");
			else if( _ppbd.m_bPushed || _ppbd.m_bIndeterminate )
				strState = _T("Pressed");
			else if( _ppbd.m_bHover )
				strState = _T("Hover");
			const CExtSkinGlyph & _glyph = m_Skin[_T("Button")][_T("RibbonButton")][strBT][ _ppbd.m_bDropDown ? _T("DropDownButton") : _T("PushButton") ][strState].Glyph( _T("Background") );
			_glyph.Draw( dc.m_hDC, _ppbd.m_rcClient );
			const CExtSkinColor & skinTextColor = m_Skin[_T("Button")][_T("RibbonButton")][strBT][ _ppbd.m_bDropDown ? _T("DropDownButton") : _T("PushButton") ][strState].Color( _T("RibbonButtonTextColor") );
			pSkinTextColor = &skinTextColor;
		}
	}
	break; // cases __ELC_TOOL and __ELC_SMALL
	} // switch( eLC )

	if( pCmdIcon != NULL )
	{
		ASSERT( ! pCmdIcon->IsEmpty() );
		CSize _sizeRealIcon = pCmdIcon->GetSize();
		if( _sizeRealIcon != _sizeIcon )
			rcIcon.OffsetRect(
				( _sizeIcon.cx - _sizeRealIcon.cx ) / 2,
				( _sizeIcon.cy - _sizeRealIcon.cy) / 2
				);
		rcIcon.right = rcIcon.left + _sizeRealIcon.cx;
		rcIcon.bottom = rcIcon.top + _sizeRealIcon.cy;
		bool bCenterBigIconVerticallyMode = ( eLC == __ELC_LARGE && nTextLength == 0 && ( ! _ppbd.m_bDropDown ) ) ? true : false;
		if( bCenterBigIconVerticallyMode)
			rcIcon.OffsetRect(
				0,
				_ppbd.m_rcClient.top - rcIcon.top + ( _ppbd.m_rcClient.Height() - rcIcon.Height() ) / 2
				);

		CExtCmdIcon::e_paint_type_t ePT = CExtCmdIcon::__PAINT_DISABLED;
		if( _ppbd.m_bEnabled )
		{
			if( _ppbd.m_bPushed )
				ePT = CExtCmdIcon::__PAINT_PRESSED;
			else if( _ppbd.m_bHover )
				ePT = CExtCmdIcon::__PAINT_HOVER;
			else
				ePT = CExtCmdIcon::__PAINT_NORMAL;
		} // if( _ppbd.m_bEnabled )
		pCmdIcon->Paint(
			this,
			dc.m_hDC,
			rcIcon,
			ePT
			);
	} // if( pCmdIcon != NULL )

	if( nTextLength > 0 || _ppbd.m_bDropDown )
	{
		UINT nDT = 0;
		if( eLC != __ELC_LARGE )
		{
			strText.Replace( _T("\r"), _T(" ") );
			strText.Replace( _T("\n"), _T(" ") );
			strText.Replace( _T("\t"), _T(" ") );
			strText.Replace( _T("  "), _T(" ") );
			strText.TrimLeft( _T(" ") );
			strText.TrimRight( _T(" ") );
			nTextLength = INT( strText.GetLength() );
			nDT = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		} // if( eLC != __ELC_LARGE )
		else
			nDT = DT_CENTER|DT_TOP;
		if( nTextLength > 0 || _ppbd.m_bDropDown )
		{
			CFont * pFont = pTBB->GetBar()->OnGetToolbarFont( false, false, _ppbd.m_pHelperSrc );
			ASSERT( pFont->GetSafeHandle() != NULL );
			CFont * pOldFont = dc.SelectObject( pFont );
			int nOldBkMode = dc.SetBkMode( TRANSPARENT );
			COLORREF clrText =
				( pSkinTextColor != NULL && (! pSkinTextColor->IsHollow() ) )
					? COLORREF( (*pSkinTextColor) )
					: GetColor( _ppbd.m_bEnabled ? COLOR_BTNTEXT : CLR_TEXT_DISABLED )
					;
			COLORREF clrOldText = dc.SetTextColor( clrText );
			Ribbon_PaintText(
				dc,
				( nTextLength > 0 ) ? LPCTSTR(strText) : _T(""),
				rcText,
				nDT,
				bDrawDropDownWithText,
				NULL,
				_ppbd.m_pHelperSrc,
				_ppbd.m_lParam
				);
			dc.SetTextColor( clrOldText );
			dc.SetBkMode( nOldBkMode );
			dc.SelectObject( pOldFont );
		} // if( nTextLength > 0 || _ppbd.m_bDropDown )
	} // if( nTextLength > 0 || _ppbd.m_bDropDown )
}

INT CExtPaintManagerSkin::Ribbon_GetTabIntersectionHeight(
	const CExtRibbonPage * pRibbonPage,
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
	if( IsHighContrast() )
		return CExtPaintManager::Ribbon_GetTabIntersectionHeight( pRibbonPage, lParam );
	return 0;
}

void CExtPaintManagerSkin::Ribbon_PaintTabItem(
	CDC & dc,
	CRect & rcTabItemsArea,
	bool bSelected,
	const CRect & rcEntireItem,
	CFont * pFont,
	__EXT_MFC_SAFE_LPCTSTR sText,
	CExtCmdIcon * pIcon,
	CObject * pHelperSrc,
	LPARAM lParam, // = 0L
	COLORREF clrForceText, // = COLORREF(-1L)
	COLORREF clrForceTabBk, // = COLORREF(-1L)
	COLORREF clrForceTabBorderLT, // = COLORREF(-1L)
	COLORREF clrForceTabBorderRB, // = COLORREF(-1L)
	COLORREF clrForceTabSeparator // = COLORREF(-1L)
	)
{
	ASSERT_VALID( this );
CRect _rcEntireItem = rcEntireItem;
	if( ! IsHighContrast() )
	{
		if( bSelected )
			_rcEntireItem.bottom += 1;
		else
			_rcEntireItem.bottom += 3;
	}
	CExtPaintManager::Ribbon_PaintTabItem(
		dc,
		rcTabItemsArea,
		bSelected,
		_rcEntireItem,
		pFont,
		sText,
		pIcon,
		pHelperSrc,
		lParam,
		clrForceText,
		clrForceTabBk,
		clrForceTabBorderLT,
		clrForceTabBorderRB,
		clrForceTabSeparator
		);
}

void CExtPaintManagerSkin::Ribbon_PaintText(
	CDC & dc,
	__EXT_MFC_SAFE_LPCTSTR strText,
	const RECT & rcText,
	UINT nDrawTextFlags,
	bool bDrawDropDownArrow,
	CExtBitmap * pBmpAlternativeDropDownArrow,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	CExtPaintManager::Ribbon_PaintText(
		dc,
		strText,
		rcText,
		nDrawTextFlags,
		bDrawDropDownArrow,
		pBmpAlternativeDropDownArrow,
		pHelperSrc,
		lParam
		);
}

bool CExtPaintManagerSkin::Ribbon_FileButtonIsItegrationSupported() const
{
	ASSERT_VALID( this );
	if( IsHighContrast() )
		return CExtPaintManager::Ribbon_FileButtonIsItegrationSupported();	
	return false;
}

void CExtPaintManagerSkin::Ribbon_NcOverPaint(
	CDC & dc,
	bool bFrameActive,
	INT nWidthFromLeft,
	bool bDwmMode,
	LPCRECT pRectDwmText,
	__EXT_MFC_SAFE_LPCTSTR strDwmCaptionTitle,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );

	if( IsHighContrast() )
	{
		CExtPaintManager::Ribbon_NcOverPaint(
			dc,
			bFrameActive,
			nWidthFromLeft,
			bDwmMode,
			pRectDwmText,
			strDwmCaptionTitle,
			pHelperSrc,
			lParam
			);
		return;
	}

bool bZoomedMode = false;
	if( pHelperSrc != NULL )
	{
		CExtRibbonBar * pRibbonBar = DYNAMIC_DOWNCAST( CExtRibbonBar, pHelperSrc );
		if( pRibbonBar != NULL )
		{
			if( pRibbonBar->GetParent()->IsZoomed() )
				bZoomedMode = true;
		} // if( pRibbonBar != NULL )
	} // if( pHelperSrc != NULL )
	nWidthFromLeft += 15;

INT nTextLen = 0;
	if(		pHelperSrc != NULL
		&&	pHelperSrc->IsKindOf( RUNTIME_CLASS(CExtRibbonBar) )
		)
	{
		CExtRibbonBar * pRibbonBar = STATIC_DOWNCAST( CExtRibbonBar, pHelperSrc );
		CRect rcIcon( 0, 0, 0, 0 );
		if(		bDwmMode
			&&	pRectDwmText != NULL
			&&	LPCTSTR(strDwmCaptionTitle) != NULL
			&&	( nTextLen = INT(_tcslen(LPCTSTR(strDwmCaptionTitle))) ) > 0
// 			&&	pRectDwmText->left < pRectDwmText->right
// 			&&	pRectDwmText->top < pRectDwmText->bottom
// 			&&	dc.RectVisible( pRectDwmText )
			)
		{
			CWnd * pWnd = (CWnd*)pHelperSrc;
			bool bRTL = ( (pWnd->GetExStyle()&WS_EX_LAYOUTRTL) != 0 ) ? true : false;
			for( ; (pWnd->GetStyle()&WS_CHILD) != 0; pWnd = pWnd->GetParent() );
			if( pWnd != NULL )
			{
				CRect rcText = *pRectDwmText;
				if( pRibbonBar->m_pExtNcFrameImpl != NULL )
				{
					CRect rcIcon( 0, 0, 0, 0 );
					CExtCmdIcon _icon;
					pRibbonBar->m_pExtNcFrameImpl->NcFrameImpl_GetIcon( _icon );
					if( ! _icon.IsEmpty() )
					{
						CSize _sizeIcon = _icon.GetSize();
						ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );
						INT nIconTop = rcText.top + ( rcText.Height() - _sizeIcon.cy ) / 2;
						rcIcon.SetRect(
							rcText.left,
							nIconTop,
							rcText.left + _sizeIcon.cx,
							nIconTop + _sizeIcon.cy
							);
						INT nV = ::GetSystemMetrics( SM_CXFRAME );
						rcIcon.OffsetRect( nV, 0 );
						rcText.left = rcIcon.right + nV;
						if( dc.RectVisible( &rcIcon ) )
						{
							DWORD dwOldRTF = 0;
							CExtCmdIcon::e_paint_type_t ePT = CExtCmdIcon::__PAINT_NORMAL;
							CWnd * pFrameWnd = pRibbonBar->m_pExtNcFrameImpl->NcFrameImpl_GetFrameWindow();
							if(		pFrameWnd != NULL
								&&	(! pFrameWnd->IsWindowEnabled() )
								)
								ePT = CExtCmdIcon::__PAINT_DISABLED;
							CExtBitmap & _bmp = _icon.GetBitmap( ePT, this );
							if( bDwmMode && bRTL )
							{
								dwOldRTF = _bmp.RunTimeFlagsGet();
								_bmp.RunTimeFlagsSet( dwOldRTF | __EXT_BMP_FLAG_NO_RTL_DETECTION );
							}
							if( bRTL )
							{
								CExtBitmap _bmp2 = _bmp;
								_bmp2.FlipHorizontal();
								if( ! _bmp2.IsEmpty() )
								{
									CSize _size = _bmp2.GetSize();
									_bmp2.AlphaBlend(
										dc.m_hDC,
										rcIcon.left,
										rcIcon.top,
										rcIcon.Width(),
										rcIcon.Height(),
										0,
										0,
										rcIcon.Width(),
										rcIcon.Height()
										);
								}
							}
							else
							{
								_icon.Paint(
									this,
									dc,
									rcIcon,
									pWnd->IsWindowEnabled()
										? CExtCmdIcon::__PAINT_NORMAL
										: CExtCmdIcon::__PAINT_DISABLED
									);
							}
							if( bDwmMode && bRTL )
							{
								_bmp.RunTimeFlagsSet( dwOldRTF );
							}
						}
					} // if( ! _icon.IsEmpty() )
			}

				if(		pRectDwmText->left < pRectDwmText->right
					&&	pRectDwmText->top < pRectDwmText->bottom
					&&	dc.RectVisible( pRectDwmText )
					)
				{
/*
					HWND hWndOpenThemeData = pWnd->m_hWnd;
					if(	g_PaintManager.m_UxTheme.OpenThemeDataEx(
							hWndOpenThemeData,
							VSCLASS_WINDOW,
							__EXT_UX_OTD_NONCLIENT
							) != NULL
						)
					{ // if extended theme data was open successfully
						static const int g_nDwmGlowSize2007 = 10;
						CExtSafeString strWindowText = LPCTSTR(strDwmCaptionTitle);
						CFont * pOldFont = dc.SelectObject( &m_FontCaption );
						INT nTextWidthRequredPX = rcText.Width() - g_nDwmGlowSize2007 * 2;
						CExtUxTheme::__EXT_UX_DTTOPTS dtoMeasure = { sizeof(CExtUxTheme::__EXT_UX_DTTOPTS) };
						dtoMeasure.dwFlags = __EXT_UX_DTT_COMPOSITED | __EXT_UX_DTT_GLOWSIZE;
						dtoMeasure.iGlowSize = g_nDwmGlowSize2007;
						CRect rcTextMeasure;
						rcTextMeasure.SetRect( 0, 0, 0, 0 );
						g_PaintManager.m_UxTheme.GetThemeTextExtent(
							dc.m_hDC,
							0,
							0,
							strWindowText,
							-1,
							DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX|DT_CALCRECT,
							NULL,
							&rcTextMeasure
							);
						INT nTextWidthCurrentPX = rcTextMeasure.Width();
 						bool bDrawText = true;
 						if( nTextWidthCurrentPX > nTextWidthRequredPX )
						{
 							bDrawText = false;
							INT nLenCalc = strWindowText.GetLength() - 1;
							for( ; nLenCalc > 0; nLenCalc -- )
							{
								CExtSafeString strCalc = strWindowText.Left( nLenCalc );
		// 						if( bRTL )
		// 						{
		// 							CString strTmp = strCalc;
		// 							strCalc = _T("...");
		// 							strCalc += strTmp;
		// 						} // if( bRTL )
		// 						else
									strCalc += _T("...");
								rcTextMeasure.SetRect( 0, 0, 0, 0 );
								g_PaintManager.m_UxTheme.GetThemeTextExtent(
									dc.m_hDC,
									0,
									0,
									strCalc,
									-1,
									DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX|DT_CALCRECT,
									NULL,
									&rcTextMeasure
									);
								INT nTextWidthCurrentPX = rcTextMeasure.Width();
								if( nTextWidthCurrentPX <= nTextWidthRequredPX )
								{
									strWindowText = strCalc;
									bDrawText = true;
									break;
								} // if( nTextWidthCurrentPX <= nTextWidthRequredPX )
							} // for( ; nLenCalc > 0; nLenCalc -- )
						} // if( nTextWidthCurrentPX > nTextWidthRequredPX )
						dc.SelectObject( &pOldFont );
						if( bDrawText )
						{
							DWORD dwOldLayout1 = LAYOUT_LTR, dwOldLayout2 = LAYOUT_LTR;
							if( bRTL )
								dwOldLayout1 = dc.SetLayout( LAYOUT_RTL );
							CExtMemoryDC dc32(
								&dc,
								&rcText,
								CExtMemoryDC::MDCOPT_TO_MEMORY
									| CExtMemoryDC::MDCOPT_FORCE_DIB
									| CExtMemoryDC::MDCOPT_FILL_BITS
									| CExtMemoryDC::MDCOPT_DIB_NEGATIVE_HEIGHT
								);
							ASSERT( dc32.GetSafeHdc() != NULL );
							dc.FillSolidRect( &rcText, RGB(0,0,0) );
							if( bRTL )
								dwOldLayout2 = dc32.SetLayout( LAYOUT_RTL );
							pOldFont = dc32.SelectObject( &m_FontCaption );
							CExtUxTheme::__EXT_UX_DTTOPTS dto = { sizeof(CExtUxTheme::__EXT_UX_DTTOPTS) };
							dto.dwFlags = __EXT_UX_DTT_TEXTCOLOR;
							dto.iGlowSize = g_nDwmGlowSize2007;
							if( ( ! g_PaintManager.m_bIsWin7OrLater ) && pWnd->IsZoomed() )
								dto.crText = RGB(0x0FF,0x0FF,0x0FF);
							else
							{
								dto.dwFlags |= __EXT_UX_DTT_COMPOSITED | __EXT_UX_DTT_GLOWSIZE;
								dto.crText = bFrameActive ? RGB(0,0,0) : RGB(0x060,0x060,0x060);
							}
							g_PaintManager.m_UxTheme.DrawThemeTextEx(
								dc32.m_hDC,
								0,
								0,
								strWindowText,
								-1,
								DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX,
								rcText,
								&dto
								);
							dc32.SelectObject( pOldFont );
							if( bRTL )
							{
								dc.SetLayout( dwOldLayout1 );
								dc32.SetLayout( dwOldLayout2 );
							} // if( bRTL )
						} // if( bDrawText )
						g_PaintManager.m_UxTheme.CloseThemeData();
					} // if extended theme data was open successfully
*/

					rcText.DeflateRect( 5, 0 );
					if( rcText.left < rcText.right && rcText.top < rcText.bottom )
					{
						static const int g_nDwmGlowSize2007 = 10;
						CFont * pOldFont = dc.SelectObject( &m_FontCaption );
						CExtUxTheme::__EXT_UX_DTTOPTS dto = { sizeof(CExtUxTheme::__EXT_UX_DTTOPTS) };
						dto.dwFlags = __EXT_UX_DTT_COMPOSITED | __EXT_UX_DTT_GLOWSIZE | __EXT_UX_DTT_TEXTCOLOR;
						dto.iGlowSize = g_nDwmGlowSize2007;
						if( ( ! g_PaintManager.m_bIsWin7OrLater ) && pWnd->IsZoomed() )
							dto.crText = RGB(0x0FF,0x0FF,0x0FF);
						else
						{
							dto.dwFlags |= __EXT_UX_DTT_COMPOSITED | __EXT_UX_DTT_GLOWSIZE;
							dto.crText = bFrameActive ? RGB(0,0,0) : RGB(0x060,0x060,0x060);
						}
						CExtRichContentLayout::stat_DrawText(
							dc.m_hDC, LPCTSTR(strDwmCaptionTitle), &rcText,
							DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_END_ELLIPSIS| ( bRTL ? DT_RTLREADING : 0 ), 0,
							pWnd->m_hWnd, VSCLASS_WINDOW, /*__EXT_UX_OTD_NONCLIENT*/ 0, 0, 0, &dto
							);
						dc.SelectObject( pOldFont );
					}


				}
			} // if( pWnd != NULL )
		} // if( bDwmMode ...
		
// 							if(		pRibbonBar->m_pExtNcFrameImpl != NULL
// 								&&	pRibbonBar->RibbonLayout_IsFrameIntegrationEnabled()
// 								&&	pRibbonBar->RibbonQuickAccessBar_AboveTheRibbonGet()
// 								&&	Ribbon_IsSystemMenuIconPresent()
// 								)
// 							{
// 								CWnd * pWndFrameImpl = pRibbonBar->m_pExtNcFrameImpl->NcFrameImpl_GetFrameWindow();
// 								if( pWndFrameImpl->GetSafeHwnd() != NULL )
// 								{
// 									CRect rcSeparator( 0, 0, 0, 0 );
// 									if( bDwmMode )
// 										rcSeparator.SetRect( rcIcon.right, rcIcon.top, rcIcon.right + 2, rcIcon.bottom );
// 									else
// 									{
// 										CRect rcWnd;
// 										pWndFrameImpl->GetWindowRect( &rcWnd );
// 										rcIcon = pRibbonBar->m_pExtNcFrameImpl->NcFrameImpl_GetNcHtRect(HTSYSMENU,false,false,false,NULL,&rcWnd);
// 										rcSeparator.SetRect( rcIcon.right + 0, rcIcon.top, rcIcon.right + 2, rcIcon.bottom - 3 );
// 										if( pWndFrameImpl->IsZoomed() )
// 											rcSeparator.OffsetRect( m_rcNcFrameBordersActive.left, m_rcNcFrameBordersActive.top );
// 									}
// 									Ribbon_PaintSeparator( dc, rcSeparator, NULL );
// 					//rcSeparator.InflateRect( 2, 2 ); dc.FillSolidRect( rcSeparator, 255 );
// 								}
// 
// 							}
	}

}

#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

bool CExtPaintManagerSkin::_PaintHeaderPartImpl(
	CDC & dc,
	CRect rc,
	CObject * pHelperSrc,
	LPARAM lParam,
	bool bDrawItemLikeBackground, // = false
	bool bHover, // = false
	bool bPressed // = false
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	pHelperSrc;
	lParam;
LPCTSTR strOuterAreaLocation = bDrawItemLikeBackground ? _T("Top") : _T("GeneralOuterBackground");
	if( bDrawItemLikeBackground ) 
	{
		LPCTSTR strHeaderCellState = _T("Normal");
		if( bPressed )
			strHeaderCellState = _T("Pressed");
		else if( bHover )
			strHeaderCellState = _T("Hover");
		const CExtSkinGlyph & _glyph =
			m_Skin[_T("Grid")][_T("HeaderCell")][strOuterAreaLocation]
				.Glyph( strHeaderCellState );
		_glyph.Draw( dc.m_hDC, rc );
	} // if( bDrawItemLikeBackground ) 
	else
	{
		const CExtSkinGlyph & _glyph =
			m_Skin[_T("Grid")][_T("OuterBackground")][strOuterAreaLocation]
				.Glyph( _T("Background") );
		_glyph.Draw( dc.m_hDC, rc );
	} // else from if( bDrawItemLikeBackground ) 
	return true;
}

INT CExtPaintManagerSkin::GetDropDownButtonWidth(
	CObject * pHelperSrc, // = NULL
	LPARAM lParam // = 0L
	) const
{
	ASSERT_VALID( this );
#ifdef _DEBUG
	if( pHelperSrc != NULL )
	{
		ASSERT_VALID( pHelperSrc );
	}
#endif // _DEBUG
	if( IsHighContrast() )
		return CExtPaintManager::GetDropDownButtonWidth( pHelperSrc, lParam );
#if (!defined __EXT_MFC_NO_RIBBON_BAR)
bool bRibbonMode = false;
	if( pHelperSrc != NULL )
	{
		if( pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtRibbonButton ) ) )
			bRibbonMode = true;
		else if( pHelperSrc->IsKindOf( RUNTIME_CLASS( CExtBarButton ) ) )
		{
			CExtToolControlBar * pToolBar = ((CExtBarButton*)pHelperSrc)->GetSafeBar();
			if(		pToolBar != NULL
				&&	pToolBar->IsKindOf( RUNTIME_CLASS( CExtRibbonPage ) )
				)
				bRibbonMode = true;
		}
	}
	if( bRibbonMode )
	{
		INT nDD = 9;
		nDD = UiScalingDo( nDD, __EUIST_X );
		return nDD;
	}
#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)
	return CExtPaintManager::GetDropDownButtonWidth( pHelperSrc, lParam );
}


void CExtPaintManagerSkin::Header_PaintBackground(
	CDC & dc,
	CRect rc,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( IsHighContrast() )
	{
		CExtPaintManager::Header_PaintBackground( dc, rc, pHelperSrc, lParam );
		return;
	}
	if( ! _PaintHeaderPartImpl( dc, rc, pHelperSrc, lParam ) )
		CExtPaintManager::Header_PaintBackground( dc, rc, pHelperSrc, lParam );
}

void CExtPaintManagerSkin::Header_PaintItem(
	CDC & dc,
	CRect rcItemEntire,
	CRect rcItemData,
	CRect rcIcon,
	CRect rcText,
	CRect rcSortArrow,
	CRect rcButton,
	CRect rcButtonIcon,
	bool bSorted,
	bool bSortedAscending,
	INT nColNo,
	INT nColCount,
	const CExtCmdIcon & iconItem,
	const CExtCmdIcon & iconButton,
	bool bHover,
	bool bPressed,
	bool bButtonEvent,
	__EXT_MFC_SAFE_LPCTSTR strItemText,
	CObject * pHelperSrc,
	LPARAM lParam // = 0L
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( IsHighContrast() )
	{
		CExtPaintManager::Header_PaintItem(
			dc, rcItemEntire, rcItemData, rcIcon, rcText, rcSortArrow, rcButton, rcButtonIcon,
			bSorted, bSortedAscending, nColNo, nColCount, iconItem, iconButton, bHover, bPressed, bButtonEvent,
			strItemText, pHelperSrc, lParam
			);
		return;
	}
COLORREF clrLT1 = GetColor( COLOR_3DHIGHLIGHT, pHelperSrc, lParam );
COLORREF clrRB1 = GetColor( COLOR_3DSHADOW,  pHelperSrc, lParam );
CRect rcDraw = rcItemEntire;
	if( ! rcButton.IsRectEmpty() )
		rcDraw.right = rcButton.left;
	if( ! _PaintHeaderPartImpl(
			dc, rcDraw, pHelperSrc, lParam, true,
			bHover && (!bButtonEvent),
			bPressed && (!bButtonEvent) )
			)
	{
		bool bDrawBorder = true;
		if( (!bButtonEvent) && ( bHover || bPressed ) )
		{
			bDrawBorder = !bPressed;
			if( bPressed && bHover )
				dc.FillRect( &rcDraw, &m_brushDarkerDefault );
			else if( bPressed )
				dc.FillRect( &rcDraw, &m_brushDarkDefault );
			else if( bHover )
				dc.FillRect( &rcDraw, &m_brushLighterDefault );
		}
		if( bDrawBorder )
			dc.Draw3dRect( &rcDraw, clrLT1, clrRB1 );
	}
	if(		(! rcButton.IsRectEmpty() )
		&&	dc.RectVisible( &rcButton )
		)
	{
		rcDraw = rcButton;
		if( ! _PaintHeaderPartImpl(
				dc, rcDraw, pHelperSrc, lParam, true,
				bHover && bButtonEvent,
				bPressed && bButtonEvent
				)
			)
		{
			bool bDrawBorder = true;
			if( bButtonEvent && ( bHover || bPressed ) )
			{
				bDrawBorder = !bPressed;
				if( bPressed && bHover )
					dc.FillRect( &rcDraw, &m_brushDarkerDefault );
				else if( bPressed )
					dc.FillRect( &rcDraw, &m_brushDarkDefault );
				else if( bHover )
					dc.FillRect( &rcDraw, &m_brushLighterDefault );
			}
			if( bDrawBorder )
				dc.Draw3dRect( &rcDraw, clrLT1, clrRB1 );
		}
		if(		(! iconButton.IsEmpty() )
			&&	(! rcButtonIcon.IsRectEmpty() )
			&&	dc.RectVisible( &rcButtonIcon )
			)
			iconButton.Paint( this, dc, rcButtonIcon );
	}
	if(		(! iconItem.IsEmpty() )
		&&	(! rcIcon.IsRectEmpty() )
		&&	dc.RectVisible( &rcIcon )
		)
		iconItem.Paint( this, dc, rcIcon );
	if(		bSorted
		&&	(! rcSortArrow.IsRectEmpty() )
		&&	dc.RectVisible( &rcSortArrow )
		)
	{
		const CExtSkinGlyph & _glyph =
			m_Skin[_T("Grid")][_T("SortArrow")]
				.Glyph( bSortedAscending ? _T("AscendingTopBottom") : _T("DescendingTopBottom") );
		_glyph.Draw( dc.m_hDC, rcSortArrow );
	}
INT nTextLen = ( LPCTSTR(strItemText) != NULL ) ? INT( _tcslen(LPCTSTR(strItemText)) ) : 0;
	if(		nTextLen > 0
		&&	(! rcText.IsRectEmpty() )
		&&	dc.RectVisible( &rcText )
		)
	{
		LPCTSTR strOuterAreaLocation = _T("Top");
		LPCTSTR strHeaderCellState = _T("Normal");
		if( ! bButtonEvent )
		{
			if( bPressed )
				strHeaderCellState = _T("Pressed");
			else if( bHover )
				strHeaderCellState = _T("Hover");
		}
		const CExtSkinColor & colorText =
			m_Skin[_T("Grid")][_T("HeaderCell")][strOuterAreaLocation]
				.Color( strHeaderCellState );
		if( ! colorText.IsHollow() )
		{
			COLORREF clrTextColor = COLORREF( colorText );
			CFont * pItemFont = & m_FontNormal;
			int nOldBkMode = dc.SetBkMode( TRANSPARENT );
			COLORREF clrOldText = dc.SetTextColor( clrTextColor );
			CFont * pOldFont = dc.SelectObject( pItemFont );
			CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(strItemText), nTextLen, (LPRECT)&rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS, 0 );
			dc.SelectObject( pOldFont );
			dc.SetTextColor( clrOldText );
			dc.SetBkMode( nOldBkMode );
		}
	}
}

