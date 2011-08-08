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

#if (!defined __PROF_SKIN_PAINT_MANAGER_H)
#define __PROF_SKIN_PAINT_MANAGER_H

#if (!defined __PROF_SKIN_H)
	#include <../ProfSkin/ProfSkin.h>
#endif

#if (!defined __PROF_SKIN_ITEM_H)
	#include <../ProfSkin/ExtSkinItem.h>
#endif

class CExtControlBar;
class CExtToolControlBar;
class CExtMenuControlBar;
class CExtStatusControlBar;
class CExtPopupMenuWnd;
class CExtTabWnd;

class __PROF_SKIN_API CExtPaintManagerSkin 
	: public CExtPaintManager
{
public:
	DECLARE_SERIAL( CExtPaintManagerSkin )

	CExtPaintManagerSkin(void);
	virtual ~CExtPaintManagerSkin(void);

	virtual void SerializeSynchronizationData( CArchive & ar );

	virtual e_paint_manager_name_t OnQueryPaintManagerName()
	{
		ASSERT_VALID( this );
		return ProfSkinPainter;
	}

	virtual bool OnStateSerialize(
		CArchive & ar,
		bool bEnableThrowExceptions = false
		);

	virtual bool NcFrame_IsSupported(
		const CWnd * pWnd,
		LPARAM lParam = 0L
		) const;
	virtual HRGN NcFrame_GenerateSkinFrameRGN(
		const RECT & rcWnd,
		const CWnd * pWnd,
		LPARAM lParam = 0L
		) const;
	virtual INT NcFrame_GetCaptionHeight(
		bool bActive,
		const CWnd * pWnd,
		LPARAM lParam = 0L
		) const;
	virtual void NcFrame_GetMetrics(
		RECT & rcNcBorders,
		RECT & rcThemePadding,
		const CWnd * pWnd,
		LPARAM lParam = 0L
		) const;
	virtual CRect NcFrame_GetHtRect(
		UINT nHT,
		bool bScreenMapping,
		bool bLayoutBordersH,
		bool bLayoutBordersV,
		const CExtCmdIcon * pIcon,
		const CWnd * pWnd,
		LPMINMAXINFO pMinMaxInfo = NULL,
		LPARAM lParam = 0L
		) const;
	static __EXT_MFC_SAFE_LPCTSTR stat_NcFrameButton_NameFromState(
		e_nc_button_state_t eState
		);
	virtual void NcFrame_Paint(
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
		e_nc_button_state_t eStateButtonHelp,
		e_nc_button_state_t eStateButtonMinimize,
		e_nc_button_state_t eStateButtonMaximizeRestore,
		e_nc_button_state_t eStateButtonClose,
		const CWnd * pWnd,
		LPARAM lParam = 0L
		) const;
	virtual void NcFrame_GetRects(
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
		LPMINMAXINFO pMinMaxInfo = NULL,
		LPARAM lParam = 0L
		) const;

	virtual bool Bar_MiniDockFrameGetNcMetrics(
		INT & nResizingFrameDX,
		INT & nResizingFrameDY,
		INT & nCaptionDY,
		CExtMiniDockFrameWnd * pMiniFrameWnd,
		CExtControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual bool Bar_MiniDockFrameNcCalcSize(
		RECT & rcAdjust,
		CExtMiniDockFrameWnd * pMiniFrameWnd,
		CExtControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual bool Bar_MiniDockFrameCaptionCalcRect(
		RECT & rcCaption,
		CExtMiniDockFrameWnd * pMiniFrameWnd,
		CExtControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual void PaintResizableBarSeparator(
		CDC & dc,
		const RECT & rc,
		bool bHorz,
		CExtControlBar * pBar,
		LPARAM lParam = 0
		);
	virtual bool Bar_LayoutNcAreaButton(
		RECT & rcButton,
		const RECT & rcCaption,
		CExtBarNcAreaButton * pLayoutBtn,
		CExtBarNcAreaButton * pPrevBtn,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		) const;
	virtual CRect Bar_GetCaptionBordersForText(
		CExtControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual INT Bar_SeparatorWidthGet(
		CExtControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual INT Bar_SeparatorHeightGet(
		CExtControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual INT Bar_GripperWidthAtLeftGet(
		CExtControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual INT Bar_GripperHeightAtTopGet(
		CExtControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual HFONT Bar_GetCaptionFont(
		bool bHorz,
		CExtToolControlBar * pBar,
		LPARAM lParam = 0
		);
	virtual HFONT CreateDefaultFont();
	virtual CSize Toolbar_GetMinButtonSize(
		CObject * pHelperSrc,
		LPARAM lParam = 0
		);
	virtual void Toolbar_AdjustButtonSize(
		CExtBarButton * pTBB,
		CSize & sizePreCalc
		);
	virtual HFONT Toolbar_GetFont(
		bool bHorz,
		CExtControlBar * pBar,
		LPARAM lParam = 0
		);
	virtual bool Toolbar_GetBGInfo(
		RECT & rcBorders,
		INT & nGripWidthAtLeft,
		INT & nGripHeightAtTop,
		CExtToolControlBar * pBar,
		LPARAM lParam = 0
		) const;
	virtual bool Toolbar_GetSizeOfCEB(
		SIZE & sizeReal,
		const SIZE & sizePreCalc,
		CDC & dc,
		BOOL bHorz,
		CExtBarContentExpandButton * pTBB,
		LPARAM lParam = 0
		);
	virtual void PaintToolbarExpandButton(
		CDC & dc,
		const RECT & rcButtonArea,
		bool bHorz, // if false - down
		bool bBarIsCompletelyVisible,
		bool bEnabled,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L,
		bool bTransparentBackground = false
		);
	virtual void FixedBar_AdjustClientRect(
		const CExtControlBar * pBar,
		CRect & rcPreCalc
		)
	{
		ASSERT_VALID( this );
		pBar;
		rcPreCalc;
		return;
	}
	virtual INT FixedBar_GetRowDistance(
		const CExtToolControlBar * pBar
		)
	{
		ASSERT_VALID( this );
		pBar;
		return 4;
	}
	virtual bool FixedBar_IsPaintRowBkMode(
		const CExtToolControlBar * pBar
		)
	{
		ASSERT_VALID( this );
		pBar;
		return false;
	}
	virtual CSize GetPushedOffset()
	{
		ASSERT_VALID( this );
		return CSize( 0, 0 );
	}
	virtual CSize GetToolBarButtonOffset( BOOL bHorz )
	{
		ASSERT_VALID( this );
		bHorz;
		return CSize( 0, 0 );
	}

	void CExtPaintManagerSkin::PaintToolbarTextField(
		CDC & dc,
		PAINTTOOLBARTEXTFIELDDATA & _ptbtfd
		);
	virtual void PaintComboFrame(
		CDC & dc,
		CExtPaintManager::PAINTCOMBOFRAMEDATA & _pcfd
		);
	virtual void PaintPushButton(
		CDC & dc,
		CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
		);
	virtual bool GetCb2DbTransparentMode(
		CObject * pObjHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool PaintDockerBkgnd(
		bool bClientMapping,
		CDC & dc,
		CWnd * pWnd,
		LPARAM lParam = NULL
		);
	virtual void PaintControlBarBorders(
		CDC & dc,
		CExtPaintManager::PAINTCONTROLBARBORDERSDATA & _pcbbd
		);
	virtual void PaintDockBarClientArea(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void PaintControlBarClientArea(
		CDC & dc,
		const RECT & rcClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void PaintDockingFrame(
		CDC & dc,
		CExtPaintManager::PAINTDOCKINGFRAMEDATA & _pdfd
		);
	virtual void PaintMenuItem(
		CDC & dc,
		CExtPaintManager::PAINTMENUITEMDATA & _pmid
		);
	virtual void PaintMenuSeparator(
		CDC & dc,
		const RECT & rectItem,
		int nIconAreaWidth,
		bool bRarelyUsed,
		bool bIsForceNoLeftGradient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual COLORREF GetMenuFrameFillColor(
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		)
	{
		ASSERT_VALID( this );
		pHelperSrc;
		lParam;
		return COLORREF( -1L );
	}
	virtual CRect GetMenuBorderMetrics(
		CWnd * pWnd,
		LPARAM lParam = 0
		) const;
	virtual void PaintMenuBorder(
		CDC & dc,
		const RECT & rectClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void PaintMenuExpandButton(
		CDC & dc,
		const RECT & rectButton,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void PaintMenuScrollButton(
		CDC & dc,
		bool bHover,
		bool bPushed,
		bool bTopButton,
		const RECT & rcScrollButton,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void PaintMenuTearOffButton(
		CDC & dc,
		const RECT & rectButton,
		bool bPushed,
		bool bHover,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void PaintGripper(
		CDC & dc,
		CExtPaintManager::PAINTGRIPPERDATA & _pgd
		);
	virtual void PaintDockingCaptionButton(
		CDC & dc,
		CExtPaintManager::PAINTDOCKINGCAPTIONBUTTONDATA & _pdcbd
		);
	virtual INT GetSeparatorHeight();

	virtual CSize GetResizingGriperSize(
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		) const;

	virtual void PaintResizingGripper(
		CDC & dc,
		const RECT & rcGrip,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

protected:
	CExtSkinGlyph & CExtPaintManagerSkin::_GetDockedToolBarGlyph(
		CExtToolControlBar * pBar,
		CExtSkinGlyph ** ppGlyphGripper = NULL
		);
	const CExtSkinGlyph & CExtPaintManagerSkin::_GetDockedToolBarGlyph(
		CExtToolControlBar * pBar,
		const CExtSkinGlyph ** ppGlyphGripper = NULL
		) const;

public:
#ifndef __EXT_MFC_NO_STATUSBAR
	virtual bool StatusBar_QuerySkinSupport(
		const CExtStatusControlBar * pStatusBar,
		LPARAM lParam = 0
		) const;
	virtual bool StatusBar_ErasePaneBackground(
		CDC & dc,
		int nPaneIdx,
		const RECT & rcPane,
		const CExtStatusControlBar * pStatusBar,
		LPARAM lParam = 0
		) const;
	virtual bool StatusBar_PaintPane(
		CDC & dc,
		int nPaneIdx,
		const RECT & rcPane,
		DWORD dwPaneStyle,
		HICON hIcon,
		__EXT_MFC_SAFE_LPCTSTR sPaneText,
		UINT nDrawTextFlags,
		COLORREF clrText,
		const CExtStatusControlBar * pStatusBar,
		LPARAM lParam = 0
		) const;
	virtual bool StatusBar_PaintSeparator(
		CDC & dc,
		int nPaneIdxAfterSep,
		const RECT & rcPaneBefore,
		const RECT & rcPaneAfter,
		const CExtStatusControlBar * pStatusBar,
		LPARAM lParam = 0
		) const;
#endif // ifndef __EXT_MFC_NO_STATUSBAR

	virtual void PaintSeparator(
		CDC & dc,
		const RECT & rectItem,
		bool bHorz,
		bool bTransparentBk,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

	virtual void PaintIcon(
		CDC & dc,
		bool bHorz,
		CExtCmdIcon * pIcon,
		const CRect & rcClient,
		bool bPushed,
		bool bEnabled,
		bool bHover = false,
		INT nIconAlignment = 0,
		bool bRTL = false,
		CRect * prcIcon = NULL,
		CRect * prcText = NULL,
		CRect * prcIconMargins = NULL,
		COLORREF clrCustomAccentEffectForIcon = COLORREF(-1L)
		);

#if (!defined __EXT_MFC_NO_TAB_CTRL)
	virtual bool QueryTabWndHoverChangingRedraw(
		const CExtTabWnd * pWndTab,
		LPARAM lParam = 0L
		);
	virtual void PaintTabbedTabClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void PaintTabClientArea(
		CDC & dc,
		CRect & rcClient,
		CRect & rcTabItemsArea,
		CRect & rcTabNearBorderArea,
		DWORD dwOrientation,
		bool bGroupedMode,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void PaintTabItem(
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
		LPARAM lParam = 0L,
		COLORREF clrForceText = COLORREF(-1L),
		COLORREF clrForceTabBk = COLORREF(-1L),
		COLORREF clrForceTabBorderLT = COLORREF(-1L),
		COLORREF clrForceTabBorderRB = COLORREF(-1L),
		COLORREF clrForceTabSeparator = COLORREF(-1L),
		bool bDwmMode = false
		);
	virtual void PaintTabButton(
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
		LPARAM lParam = 0L,
		bool bFlat = false
		);
	virtual void PaintTabNcAreaRect(
		CDC & dc,
		const RECT & rc,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void TabWnd_MeasureItemAreaMargins(
		CExtTabWnd * pTabWnd,
		LONG & nSpaceBefore,
		LONG & nSpaceAfter,
		LONG & nSpaceOver
		);	
	virtual void TabWnd_UpdateItemMeasure(
		CExtTabWnd * pTabWnd,
		CExtTabWnd::TAB_ITEM_INFO * pTii,
		CDC & dcMeasure,
		CSize & sizePreCalc
		);	
	virtual void GetThemeAccentTabColors(
		COLORREF * pClrSetectedTabItemText,
		COLORREF * pClrSetectedTabItemFace = NULL,
		COLORREF * pClrSetectedTabItemBorderLight = NULL,
		COLORREF * pClrSetectedTabItemBorderDark = NULL,
		CObject * pHelperSrc = NULL,
		LPARAM lParam = 0L
		);
	virtual __EXT_MFC_SAFE_LPCTSTR GetSkinnedTabControlType(
		const CExtTabWnd * pWndTab
		);
	virtual __EXT_MFC_SAFE_LPCTSTR GetSkinnedTabControlOrientation(
		const CExtTabWnd * pWndTab
		);
#endif // (!defined __EXT_MFC_NO_TAB_CTRL)

	virtual COLORREF GetIconAlphaColor() const;
	virtual double GetIconAlphaAdjustPercent() const;

	virtual bool AdjustIcon( 
		CExtCmdIcon * pIcon
		);

	virtual COLORREF QueryObjectTextColor(
		CDC & dc,
		bool bEnabled,
		bool bFocused,
		bool bHovered,
		bool bPressed,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual CSize GetCheckButtonBoxSize(
		CDC & dc,
		CExtPaintManager::PAINTCHECKRADIOBUTTONDATA & _pcbd
		) const;
	virtual CSize GetRadioButtonBoxSize(
		CDC & dc,
		CExtPaintManager::PAINTCHECKRADIOBUTTONDATA & _pcbd
		) const;

	virtual void PaintCheckButtonBox(
		CDC & dc,
		CExtPaintManager::PAINTCHECKRADIOBUTTONDATA & _pcbd
		);
	virtual void PaintRadioButtonBox(
		CDC & dc,
		CExtPaintManager::PAINTCHECKRADIOBUTTONDATA & _pcbd
		);

	virtual bool PaintGroupBoxFrame(
		CDC & dc,
		const RECT & rcBorder,
		COLORREF clrFrame = COLORREF(-1L),
		CObject * pHelperSrc = NULL,
		LPARAM lParam = 0L
		);
	virtual bool PaintGroupBoxLabel(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR strText,
		COLORREF clrText,
		HFONT hFont,
		DWORD dwDrawTextFlags,
		bool bEnabled,
		const RECT & rcText,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);

#if( !defined __EXT_MFC_NO_SPIN || !defined __EXT_MFC_NO_DURATIONWND )
	virtual void PaintSpinButtonArrow(
		CDC & dc,
		const CRect & rcButton,
		bool bUp,		
		bool bHorz,
		bool bEnabled,
		bool bPushed,
		bool bHover,
		bool bHotTrack,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);	
	virtual void PaintSpinButton(
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
		LPARAM lParam = 0L
		);	
#endif // #if( !defined __EXT_MFC_NO_SPIN || !defined __EXT_MFC_NO_DURATIONWND )

	virtual void PaintControlFrame(
		CDC & dc,
		CExtPaintManager::PAINTCONTROLFRAMEDATA & _pcfd
		);

	virtual bool AdjustControlFont(
		HFONT & hFont,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual DLGTEMPLATE * AdjustDialogTemplate(
		const DLGTEMPLATE * lpDialogTemplate,
		CObject * pHelperSrc,
		bool bWizard = false,
		LPARAM lParam = 0L
		);

#if (!defined __EXT_MFC_NO_DATE_PICKER)
	virtual void PaintDatePickerPushButton(
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
		LPARAM lParam = 0L
		);
	virtual void PaintDatePickerClientArea(
		CDC & dc,
		const RECT & rcTotalClient,
		const RECT & rcUsedClient,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
#endif // (!defined __EXT_MFC_NO_DATE_PICKER)

#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
	virtual void PaintShortcutListItem(
		CDC & dc,
		CExtPaintManager::PAINTSHORTCUTLISTITEMSDATA & _pslid
		);
#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)

	virtual SCROLLBARSKINDATA * ScrollBar_GetSkinData(
		bool bHorzBar,
		CObject * pHelperSrc,
		LPARAM lParam = 0L,
		bool bLightAccent = true
		);
	virtual bool ScrollBar_GetMetrics(
		INT * p_nHorzBarHeight = NULL,
		INT * p_nVertBarWidth = NULL,
		INT * p_nHorzButtonWidth = NULL,
		INT * p_nVertButtonHeight = NULL,
		INT * p_nHorzThumbMinWidth = NULL,
		INT * p_nVertThumbMinHeight = NULL,
		CObject * pHelperSrc = NULL,
		LPARAM lParam = 0L
		) const;

	virtual bool PaintDocumentClientAreaBkgnd(
		CDC & dc,
		CWnd * pWnd,
		LPARAM lParam = NULL
		);

#ifndef __EXT_MFC_NO_GRIDWND
	virtual bool Grid_PaintButton(
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
		LPARAM lParam = 0L
		);
	virtual bool Grid_PaintHeaderSortArrow(
		CDC & dc,
		const RECT & rcSortArrow,
		bool bHorz,
		bool bAscending,
		COLORREF * pColorValues,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool Grid_PaintHeaderBackground(
		CDC & dc,
		const RECT & rcArea,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual COLORREF Grid_OnQueryTextColor(
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
		);
#endif // #ifndef __EXT_MFC_NO_GRIDWND

#if (!defined __EXT_MFC_NO_REPORTGRIDWND)
	virtual bool ReportGrid_PaintHeaderRowBackground(
		CDC & dc,
		const RECT & rcArea,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool ReportGrid_PaintHeaderSortArrow(
		CDC & dc,
		const RECT & rcSortArrow,
		bool bAscending,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool ReportGrid_PaintGroupAreaBackground(
		CDC & dc,
		const RECT & rcArea,
		__EXT_MFC_SAFE_LPCTSTR strEmptyAreaCaption, // not NULL if area is empty
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual COLORREF ReportGrid_GetHeaderTextColor(
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual COLORREF ReportGrid_GetGroupConnectorColor(
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool ReportGrid_PaintGroupAreaItemBackground(
		CDC & dc,
		const RECT & rcItem,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
#endif // (!defined __EXT_MFC_NO_REPORTGRIDWND)

	virtual CFont * MenuCaptionGetFont(
		CExtPopupMenuWnd * pPopup,
		LPARAM lParam = 0L
		) const;
	virtual CSize MenuCaption_Measure(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR sMenuCaptionText,
		CExtPopupMenuWnd * pPopup,
		LPARAM lParam = 0L
		) const;
	virtual void MenuCaption_Paint(
		CDC & dc,
		const RECT & rcMenuCaption,
		__EXT_MFC_SAFE_LPCTSTR sMenuCaptionText,
		CExtPopupMenuWnd * pPopup,
		LPARAM lParam = 0L
		) const;

#if (!defined __EXT_MFC_NO_RIBBON_BAR)

	virtual bool Ribbon_OptionsPageBackgroundIsDefault() const;

	virtual CFont * RibbonGallery_GetItemFont(
		CExtRibbonGalleryWnd * pGalleryWnd,
		LPVOID pGalleryItemData,
		LPARAM lParam = 0L
		);
	virtual CSize RibbonGallery_MeasureItem(
		CDC & dc,
		CExtRibbonGalleryWnd * pGalleryWnd,
		LPVOID pGalleryItemData,
		LPARAM lParam = 0L
		);
	virtual bool RibbonGallery_DrawItem(
		CDC & dc,
		CExtRibbonGalleryWnd * pGalleryWnd,
		const RECT & rcItem,
		LPVOID pGalleryItemData,
		LPARAM lParam = 0L
		);
	virtual bool RibbonGallery_EraseEntireBk(
		CDC & dc,
		const CRect & rcClient,
		const CRect & rcActiveChildArea,
		CRgn & rgnActiveChildArea,
		const CRect & rcActiveChildArea2,
		CRgn & rgnActiveChildArea2,
		CExtRibbonGalleryWnd * pGalleryWnd,
		LPARAM lParam = 0L
		);
	virtual void RibbonGallery_PaintScrollArea(
		CDC & dc,
		CExtRibbonGalleryInplaceScrollBar * pSB,
		LPARAM lParam = 0L
		);

	virtual bool Ribbon_GroupCaptionIsAtTop(
		const CExtRibbonButtonGroup * pGroupTBB,
		LPARAM lParam = 0L
		) const;
	virtual void Ribbon_PaintGroupBk(
		CDC & dc,
		const RECT & rcGroupBk,
		const CExtRibbonButtonGroup * pGroupTBB,
		LPARAM lParam = 0L
		);
	virtual void Ribbon_PaintPageBk(
		CDC & dc,
		const RECT & rcPageBk,
		const CExtRibbonPage * pRibbonPage,
		LPARAM lParam = 0L
		);
	virtual void Ribbon_PaintPushButton(
		CDC & dc,
		CExtPaintManager::PAINTPUSHBUTTONDATA & _ppbd
		);
	virtual INT Ribbon_GetTabIntersectionHeight(
		const CExtRibbonPage * pRibbonPage,
		LPARAM lParam = 0L
		) const;
	virtual void Ribbon_PaintTabItem(
		CDC & dc,
		CRect & rcTabItemsArea,
		bool bSelected,
		const CRect & rcEntireItem,
		CFont * pFont,
		__EXT_MFC_SAFE_LPCTSTR sText,
		CExtCmdIcon * pIcon,
		CObject * pHelperSrc,
		LPARAM lParam = 0L,
		COLORREF clrForceText = COLORREF(-1L),
		COLORREF clrForceTabBk = COLORREF(-1L),
		COLORREF clrForceTabBorderLT = COLORREF(-1L),
		COLORREF clrForceTabBorderRB = COLORREF(-1L),
		COLORREF clrForceTabSeparator = COLORREF(-1L)
		);
	virtual void Ribbon_PaintText(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR strText,
		const RECT & rcText,
		UINT nDrawTextFlags,
		bool bDrawDropDownArrow,
		CExtBitmap * pBmpAlternativeDropDownArrow,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual bool Ribbon_FileButtonIsItegrationSupported() const;
	virtual void Ribbon_NcOverPaint(
		CDC & dc,
		bool bFrameActive,
		INT nWidthFromLeft,
		bool bDwmMode,
		LPCRECT pRectDwmText,
		__EXT_MFC_SAFE_LPCTSTR strDwmCaptionTitle,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void Ribbon_EmbeddedCaptionPaintText(
		CDC & dc,
		__EXT_MFC_SAFE_LPCTSTR strCaption,
		CRect rcDrawText,
		__EXT_MFC_SAFE_LPCTSTR strCaptionDelimiter,
		const CExtRibbonBar * pRibbonBar,
		LPARAM lParam = 0L
		);
	virtual UINT Ribbon_EmbeddedCaptionGetTextAlignmentFlags(
		const CExtRibbonBar * pRibbonBar,
		LPARAM lParam = 0L
		);
	virtual void Ribbon_EmbeddedCaptionAdjustTextRect(
		CRect & rcAdjust,
		const CExtRibbonBar * pRibbonBar,
		LPARAM lParam = 0L
		);

#endif // (!defined __EXT_MFC_NO_RIBBON_BAR)

	virtual INT GetDropDownButtonWidth(
		CObject * pHelperSrc = NULL,
		LPARAM lParam = 0L
		) const;

	// header control
	virtual void Header_PaintBackground(
		CDC & dc,
		CRect rc,
		CObject * pHelperSrc,
		LPARAM lParam = 0L
		);
	virtual void Header_PaintItem(
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
		LPARAM lParam = 0L
		);
protected:
	bool _PaintHeaderPartImpl(
		CDC & dc,
		CRect rc,
		CObject * pHelperSrc,
		LPARAM lParam,
		bool bDrawItemLikeBackground = false,
		bool bHover = false,
		bool bPressed = false
		);
public:

	CExtSkin m_Skin;
}; // class CExtPaintManagerSkin

#endif // __PROF_SKIN_PAINT_MANAGER_H

