// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_)
#define AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window

#define __WGS84_MAP_LEFT              15.68
#define __WGS84_MAP_RIGHT              5.3
#define __WGS84_MAP_WIDTH            ( __WGS84_MAP_LEFT - __WGS84_MAP_RIGHT )
#define __WGS84_MAP_TOP               55.46
#define __WGS84_MAP_BOTTOM            46.68
#define __WGS84_MAP_HEIGHT           ( __WGS84_MAP_TOP - __WGS84_MAP_BOTTOM )
#define __ZOOM_EXTENT_PX             180
#define __ZOOM_FACTOR_MULTIPLIER     160.0

class CMainFrame;

class CChildView : public CExtWRB < CExtScrollWnd >
{
// Construction
public:
	DECLARE_DYNAMIC( CChildView );
	CChildView();

	static HBRUSH stat_GetHalftoneLightBrush(
		int nLighterSpec
		);

	inline static INT GeodesicToClientX(
		double lfLongitude,
		double lfZoomFactor,
		INT nBitmapWidth
		)
	{
		double lfX = ( ( lfLongitude - __WGS84_MAP_RIGHT ) * double( nBitmapWidth ) * lfZoomFactor ) / __WGS84_MAP_WIDTH;
		return INT(lfX);
	}
	inline static double ClientToGeodesicX(
		INT nClientX,
		double lfZoomFactor,
		INT nBitmapWidth
		)
	{
		if( lfZoomFactor == 0.0 )
			return +10e+10;
		double lfLongitude = ( double(nClientX) * __WGS84_MAP_WIDTH ) / ( double( nBitmapWidth ) * lfZoomFactor ) + __WGS84_MAP_RIGHT;
		return lfLongitude;
	}
	double ClientToGeodesicX(
		INT nClientX
		) const
	{
		ASSERT_VALID( this );
		ASSERT( ! m_bmp.IsEmpty() );
		return
			ClientToGeodesicX(
				nClientX,
				m_lfZoomFactor,
				m_bmp.GetSize().cx
				);
	}
	inline static INT GeodesicToClientY(
		double lfLatitude,
		double lfZoomFactor,
		INT nBitmapHeight
		)
	{
		double nTmp = double( nBitmapHeight ) * lfZoomFactor;
		double lfY = double( nTmp ) - ( ( lfLatitude - __WGS84_MAP_BOTTOM ) * nTmp ) / __WGS84_MAP_HEIGHT;
		return INT(lfY);
	}
	inline static double ClientToGeodesicY(
		INT nClientY,
		double lfZoomFactor,
		INT nBitmapHeight
		)
	{
		if( lfZoomFactor == 0.0 )
			return +10e+10;
		double nTmp = double( nBitmapHeight ) * lfZoomFactor;
		double lfLatitude =  ( ( double( nTmp ) - double(nClientY) ) * __WGS84_MAP_HEIGHT ) / nTmp + __WGS84_MAP_BOTTOM;
		return lfLatitude;
	}
	double ClientToGeodesicY(
		INT nClientY
		) const
	{
		ASSERT_VALID( this );
		ASSERT( ! m_bmp.IsEmpty() );
		return
			ClientToGeodesicY(
				nClientY,
				m_lfZoomFactor,
				m_bmp.GetSize().cy
				);
	}
	inline static CPoint GeodesicToClient(
		double lfLongitude,
		double lfLatitude,
		double lfZoomFactor,
		CSize sizeBitmap
		)
	{
		return
			CPoint(
				GeodesicToClientX( lfLongitude, lfZoomFactor, sizeBitmap.cx ),
				GeodesicToClientY( lfLatitude, lfZoomFactor, sizeBitmap.cy )
				);
	}
	struct GEODESIC_POINT
	{
		double lfLongitude, lfLatitude;
		GEODESIC_POINT()
		{
		}
		GEODESIC_POINT(
			double _lfLongitude,
			double _lfLatitude
			)
			: lfLongitude( _lfLongitude )
			, lfLatitude( _lfLatitude )
		{
		}
		GEODESIC_POINT( const GEODESIC_POINT & gp )
		{
			lfLongitude = gp.lfLongitude;
			lfLatitude = gp.lfLatitude;
		}
		GEODESIC_POINT & operator = ( const GEODESIC_POINT & gp )
		{
			lfLongitude = gp.lfLongitude;
			lfLatitude = gp.lfLatitude;
			return (*this);
		}
		GEODESIC_POINT operator - () const
		{
			return GEODESIC_POINT( -lfLongitude, -lfLatitude );
		}
	};
	inline static CPoint GeodesicToClient(
		const GEODESIC_POINT & gp,
		double lfZoomFactor,
		CSize sizeBitmap
		)
	{
		return
			CPoint(
				GeodesicToClientX( gp.lfLongitude, lfZoomFactor, sizeBitmap.cx ),
				GeodesicToClientY( gp.lfLatitude, lfZoomFactor, sizeBitmap.cy )
				);
	}
	CPoint GeodesicToClient(
		const GEODESIC_POINT & gp
		) const
	{
		ASSERT_VALID( this );
		ASSERT( ! m_bmp.IsEmpty() );
		return
			GeodesicToClient(
				gp,
				m_lfZoomFactor,
				m_bmp.GetSize()
				);
	}
	inline static GEODESIC_POINT ClientToGeodesic(
		const POINT & ptClient,
		double lfZoomFactor,
		CSize sizeBitmap
		)
	{
		return
			GEODESIC_POINT(
				ClientToGeodesicX( ptClient.x, lfZoomFactor, sizeBitmap.cx ),
				ClientToGeodesicY( ptClient.y, lfZoomFactor, sizeBitmap.cy )
				);
	}
	GEODESIC_POINT ClientToGeodesic(
		const POINT & ptClient
		) const
	{
		ASSERT_VALID( this );
		ASSERT( ! m_bmp.IsEmpty() );
		return
			ClientToGeodesic(
				ptClient,
				m_lfZoomFactor,
				m_bmp.GetSize()
				);
	}
	struct GEODESIC_RECT
	{
		double lfLongitudeLeft, lfLatitudeTop, lfLongitudeRight, lfLatitudeBottom;
		GEODESIC_RECT()
		{
		}
		GEODESIC_RECT(
			double _lfLongitudeLeft,
			double _lfLatitudeTop,
			double _lfLongitudeRight,
			double _lfLatitudeBottom
			)
			: lfLongitudeLeft( _lfLongitudeLeft )
			, lfLatitudeTop( _lfLatitudeTop )
			, lfLongitudeRight( _lfLongitudeRight )
			, lfLatitudeBottom( _lfLatitudeBottom )
		{
		}
		GEODESIC_RECT( const GEODESIC_RECT & gr )
		{
			lfLongitudeLeft = gr.lfLongitudeLeft;
			lfLatitudeTop = gr.lfLatitudeTop;
			lfLongitudeRight = gr.lfLongitudeRight;
			lfLatitudeBottom = gr.lfLatitudeBottom;
		}
		GEODESIC_RECT & operator = ( const GEODESIC_RECT & gr )
		{
			lfLongitudeLeft = gr.lfLongitudeLeft;
			lfLatitudeTop = gr.lfLatitudeTop;
			lfLongitudeRight = gr.lfLongitudeRight;
			lfLatitudeBottom = gr.lfLatitudeBottom;
			return (*this);
		}
		bool PtInRect(
			double _lfLongitude,
			double _lfLatitude
			) const
		{
			if(		lfLongitudeLeft <= _lfLongitude && _lfLongitude <= lfLongitudeRight
				&&	lfLatitudeBottom <= _lfLatitude && _lfLatitude <= lfLatitudeTop
				)
				return true;
			return false;
		}
		bool PtInRect( const GEODESIC_POINT & gp ) const
		{
			return PtInRect( gp.lfLongitude, gp.lfLatitude );
		}
		void OffsetRect(
			double _lfLongitude,
			double _lfLatitude
			)
		{
			lfLongitudeLeft += _lfLongitude;
			lfLatitudeTop += _lfLatitude;
			lfLongitudeRight += _lfLongitude;
			lfLatitudeBottom += _lfLatitude;
		}
		bool EnsureLineCompletelyOutsideRect(
			const GEODESIC_POINT & gp1,
			const GEODESIC_POINT & gp2
			) const
		{
			if( PtInRect( gp1 ) )
				return false;
			if( PtInRect( gp2 ) )
				return false;
			if(		( gp1.lfLongitude < lfLongitudeLeft && gp2.lfLongitude < lfLongitudeLeft )
				||	( gp1.lfLongitude > lfLongitudeRight && gp2.lfLongitude > lfLongitudeRight )
				||	( gp1.lfLatitude < lfLatitudeBottom && gp2.lfLatitude < lfLatitudeBottom )
				||	( gp1.lfLatitude > lfLatitudeTop && gp2.lfLatitude > lfLatitudeTop )
				)
				return true;
			return false;
		}
		void OffsetRect(
			const GEODESIC_POINT & gp
			)
		{
			OffsetRect( gp.lfLongitude, gp.lfLatitude );
		}
	};
	inline static CRect GeodesicToClient(
		const GEODESIC_RECT & gr,
		double lfZoomFactor,
		CSize sizeBitmap
		)
	{
		return
			CRect(
				GeodesicToClientX( gr.lfLongitudeLeft, lfZoomFactor, sizeBitmap.cx ),
				GeodesicToClientY( gr.lfLatitudeTop, lfZoomFactor, sizeBitmap.cy ),
				GeodesicToClientX( gr.lfLongitudeRight, lfZoomFactor, sizeBitmap.cx ),
				GeodesicToClientY( gr.lfLatitudeBottom, lfZoomFactor, sizeBitmap.cy )
				);
	}
	CRect GeodesicToClient(
		const GEODESIC_RECT & gr
		) const
	{
		ASSERT_VALID( this );
		ASSERT( ! m_bmp.IsEmpty() );
		return
			GeodesicToClient(
				gr,
				m_lfZoomFactor,
				m_bmp.GetSize()
				);
	}
	inline static GEODESIC_RECT ClientToGeodesic(
		const RECT & rcClient,
		double lfZoomFactor,
		CSize sizeBitmap
		)
	{
		return
			GEODESIC_RECT(
				ClientToGeodesicX( rcClient.left, lfZoomFactor, sizeBitmap.cx ),
				ClientToGeodesicY( rcClient.top, lfZoomFactor, sizeBitmap.cy ),
				ClientToGeodesicX( rcClient.right, lfZoomFactor, sizeBitmap.cx ),
				ClientToGeodesicY( rcClient.bottom, lfZoomFactor, sizeBitmap.cy )
				);
	}
	GEODESIC_RECT ClientToGeodesic(
		const RECT & rcClient
		) const
	{
		ASSERT_VALID( this );
		ASSERT( ! m_bmp.IsEmpty() );
		return
			ClientToGeodesic(
				rcClient,
				m_lfZoomFactor,
				m_bmp.GetSize()
				);
	}
	struct GEODESIC_OBJECT
	{
		CExtSafeString     m_strName, m_strURL;
		GEODESIC_POINT     m_gp;
		LONG               m_nLevel;
		CExtBitmap         m_bmp;
		GEODESIC_OBJECT()
			: m_gp( 0.0, 0.0 )
			, m_nLevel( 3 )
		{
		}
		GEODESIC_OBJECT(
			LPCTSTR _strName,
			LPCTSTR _strURL,
			const GEODESIC_POINT & _gp,
			LONG _nLevel
			)
			: m_strName( ( _strName == NULL ) ? _T("") : _strName )
			, m_strURL( ( _strURL == NULL ) ? _T("") : _strURL )
			, m_gp( _gp )
			, m_nLevel( _nLevel )
		{
			ASSERT( 0 <= m_nLevel && m_nLevel <= 3 );
		}
		GEODESIC_OBJECT(
			LPCTSTR _strName,
			LPCTSTR _strURL,
			double _lfLongitude,
			double _lfLatitude,
			LONG _nLevel
			)
			: m_strName( ( _strName == NULL ) ? _T("") : _strName )
			, m_strURL( ( _strURL == NULL ) ? _T("") : _strURL )
			, m_gp( _lfLongitude, _lfLatitude )
			, m_nLevel( _nLevel )
		{
			ASSERT( 0 <= m_nLevel && m_nLevel <= 3 );
		}
		GEODESIC_OBJECT( const GEODESIC_OBJECT & _other )
		{
			m_strName   = _other.m_strName;
			m_strURL    = _other.m_strURL;
			m_gp        = _other.m_gp;
			m_nLevel    = _other.m_nLevel;
			m_bmp       = _other.m_bmp;
			ASSERT( 0 <= m_nLevel && m_nLevel <= 3 );
		}
		GEODESIC_OBJECT & operator = ( const GEODESIC_OBJECT & _other )
		{
			m_strName   = _other.m_strName;
			m_strURL    = _other.m_strURL;
			m_gp        = _other.m_gp;
			m_nLevel    = _other.m_nLevel;
			m_bmp       = _other.m_bmp;
			ASSERT( 0 <= m_nLevel && m_nLevel <= 3 );
			return (*this);
		}
		bool ScanLine( CString & s )
		{
			int nPos1 = s.Find( _T(";") );
			if( nPos1 < 0 )
				return false;
			int nPos2 = s.Find( _T(";"), nPos1 + 1 );
			if( nPos2 < 0 )
				return false;
			int nPos3 = s.Find( _T(";"), nPos2 + 1 );
			if( nPos3 < 0 )
				return false;
			int nPos4 = s.Find( _T(";"), nPos3 + 1 );

			CString _s = s.Mid( 0, nPos1 );
			_s.TrimLeft( _T("\" \r\n\t,;") );
			_s.TrimRight( _T("\" \r\n\t,;") );
			if( _s.IsEmpty() )
				return false;
			m_strName = _s;

			_s = s.Mid( nPos1 + 1, nPos2 - nPos1 - 1 );
			_s.TrimLeft( _T("\" \r\n\t,;") );
			_s.TrimRight( _T("\" \r\n\t,;") );
			if( _s.IsEmpty() )
				return false;
			if( __EXT_MFC_STSCANF( LPCTSTR(_s), _T("%lf"), &m_gp.lfLatitude ) != 1 )
				return false;

			_s = s.Mid( nPos2 + 1, nPos3 - nPos2 - 1 );
			_s.TrimLeft( _T("\" \r\n\t,;") );
			_s.TrimRight( _T("\" \r\n\t,;") );
			if( _s.IsEmpty() )
				return false;
			if( __EXT_MFC_STSCANF( LPCTSTR(_s), _T("%lf"), &m_gp.lfLongitude ) != 1 )
				return false;

			if( nPos4 >= 0 )
				_s = s.Mid( nPos3 + 1, nPos4 - nPos3 - 1 );
			else
				_s = s.Mid( nPos3 + 1, s.GetLength() - nPos3 - 1 );
			_s.TrimLeft( _T("\" \r\n\t,;") );
			_s.TrimRight( _T("\" \r\n\t,;") );
			if( _s.IsEmpty() )
				return false;
			_s.MakeLower();
			if( _s == _T("hbf-cap") )
				m_nLevel = 3;
			else if( _s == _T("hbf") )
				m_nLevel = 2;
			else if( _s == _T("hpu") )
				m_nLevel = 1;
			else if( _s == _T("bf") )
				m_nLevel = 0;
			else
				return false;

			if( nPos4 >= 0 )
			{
				int nPos5 = s.Find( _T(";"), nPos4 + 1 );
				if( nPos5 >= 0 )
					_s = s.Mid( nPos4 + 1, nPos5 - nPos4 - 1 );
				else
					_s = s.Mid( nPos4 + 1, s.GetLength() - nPos4 - 1 );
				_s.TrimLeft( _T("\" \r\n\t,;") );
				_s.TrimRight( _T("\" \r\n\t,;") );
				m_strURL = _s;
				if( nPos5 >= 0 )
				{
					_s = s.Mid( nPos5 + 1, s.GetLength() - nPos5 - 1 );
					int d;
					if( __EXT_MFC_STSCANF( LPCTSTR(_s), _T("%d"), &d ) == 1 )
					{
						VERIFY( m_bmp.LoadBMP_Resource( MAKEINTRESOURCE(d) ) );
						CSize _sizeBmp = m_bmp.GetSize();
						static const int nMaxIconWidth = 70;
						if( _sizeBmp.cx > nMaxIconWidth )
						{
							_sizeBmp.cy = ::MulDiv( _sizeBmp.cy, nMaxIconWidth, _sizeBmp.cx );
							_sizeBmp.cx = nMaxIconWidth;
							CExtBitmap::Filter _f( CExtBitmap::Filter::bilinear );
							m_bmp.Scale( _sizeBmp.cx, _sizeBmp.cy, _f );
						}
					}
				}
			}
			return true;
		}
	};

// Attributes
public:
	CExtBitmap m_bmp;
protected:
	CExtBitmap m_bmpCache, m_bmpHelperZoomRectHighlight;
	CExtScrollBar m_wndScrollBarH, m_wndScrollBarV;
	CExtZoomScrollBar m_wndZoomer;
	CRect m_rcHelperZoomRectPadding, m_rcHelperDrawZoomRect;
	enum e_show_text_labels_t
	{
		__ESTLT_HIDE_ALL           = 0,
		__ESTLT_BY_ZOOM            = 1,
		__ESTLT_SHOW_ALL           = 2,
	};
	e_show_text_labels_t m_eSTLT;
	enum e_show_objects_t
	{
		__ESTOT_HIDE_ALL           = 0,
		__ESTOT_ELLIPSE_FLAT       = 1,
		__ESTOT_ELLIPSE_CIRCLE     = 2,
		__ESTOT_ELLIPSE_SOLID      = 3,
		__ESTOT_ALPHA_BMP          = 4,
	};
	e_show_objects_t m_eSTOT;
	INT m_nHT;
	CExtPopupMenuTipWnd m_wndCoolTip;
	double m_lfZoomFactor;
	CArray < GEODESIC_OBJECT, GEODESIC_OBJECT & > m_arrObjects;
	CPoint m_ptLastStatusLL;
	CExtSafeString m_strEmptyLocationStatusText;
	static const COLORREF g_arrClrBk[ 4 ];
	static const double g_arrVZF[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
	static const INT g_arrRadiuses[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
	static const INT g_arrDwmGlowSizes[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
	CExtBitmapCache m_arrObjectBmps[ 4 ];
	CSize m_arrObjectBmpSize[ 4 ];
	CPoint m_arrObjectBmpOffsets[ 4 ];
	bool m_bEnableVistaTextRendering:1, m_bEnableHalftoning:1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();
	INT GetObjectCount() const;
	GEODESIC_OBJECT & GetObjectAt( INT nObjectIndex );
	const GEODESIC_OBJECT & GetObjectAt( INT nObjectIndex ) const;
	virtual CSize OnSwGetTotalSize() const;
	virtual CSize OnSwGetPageSize( int nDirection ) const
	{
		nDirection;
		return CSize( 50, 50 );
	}
	virtual CSize OnSwGetLineSize( int nDirection ) const
	{
		nDirection;
		return CSize( 10, 10 );
	}
	virtual void OnSwSetScrollPos( POINT pt );
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
		);
	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc  = NULL
		);
	virtual void OnSwPaintAreaBetweenScrollBarWindows(
		CDC & dc,
		const RECT & rcAreaBetweenScrollBarWindows
		);
	virtual void OnSwPaint( CDC & dc );
protected:
	CMainFrame * _GetMainFrame() const;
	void _AdjustHT( CPoint point );
	CRect _AvailableRectTrackSpaceGet() const;
	CRect _TrackZoomRect(
		CPoint ptStartTracking,
		CRect rcAvailableRectTrackSpace,
		bool bUseHalftone = false
		);
	INT _HitTestObject(
		CPoint ptClient,
		RECT * pRectObject = NULL
		) const;
	void _StatusLL_Clear();
	bool _StatusLL_Set(
		const GEODESIC_OBJECT & _object
		);
	bool _StatusLL_Set(
		LPCTSTR strName,
		double lfLongitude,
		double lfLatitude
		);
	bool _StatusLL_Set(
		LPCTSTR strName,
		CPoint ptClient
		);
	void _UpdateZoomerTT();
	void _SetZoomFromZoomer(
		bool bTrackPos
		);

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnHideAllTextLabels();
	afx_msg void OnUpdateHideAllTextLabels( CCmdUI * pCmdUI );
	afx_msg void OnShowTextLabelsWithZooming();
	afx_msg void OnUpdateShowTextLabelsZooming( CCmdUI * pCmdUI );
	afx_msg void OnShowAllTextLabels();
	afx_msg void OnUpdateShowAllTextLabels( CCmdUI * pCmdUI );
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnVistaTextRendering();
	afx_msg void OnUpdateVistaTextRendering(CCmdUI* pCmdUI);
	afx_msg void OnHalftoning();
	afx_msg void OnUpdateHalftoning(CCmdUI* pCmdUI);
	afx_msg void OnObjectHideAll();
	afx_msg void OnUpdateObjectHideAll(CCmdUI* pCmdUI);
	afx_msg void OnObjectEllipseFlat();
	afx_msg void OnUpdateObjectEllipseFlat(CCmdUI* pCmdUI);
	afx_msg void OnObjectEllipseCircle();
	afx_msg void OnUpdateObjectEllipseCircle(CCmdUI* pCmdUI);
	afx_msg void OnObjectEllipseSolid();
	afx_msg void OnUpdateObjectEllipseSolid(CCmdUI* pCmdUI);
	afx_msg void OnObjectAlphaBmp();
	afx_msg void OnUpdateObjectAlphaBmp(CCmdUI* pCmdUI);
	afx_msg void OnShowTrackingTooltipOnZoomScrollbar();
	afx_msg void OnUpdateShowTrackingTooltipOnZoomScrollbar(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_)
