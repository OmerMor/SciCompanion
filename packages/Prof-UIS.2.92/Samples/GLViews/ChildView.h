// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_)
#define AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	#if (!defined __EXT_CONTROLBAR_TABBED_FEATURES_H)
		#include <../Src/ExtControlBarTabbedFeatures.h>
	#endif // __EXT_CONTROLBAR_TABBED_FEATURES_H
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

#define GL_VIEWS_CAMERA_STATE _T("CameraState")

#define GL_VIEWS_CAMERA_COUNT 10
#define GL_VIEWS_CAMERA_NAME_FMT _T("Camera %d")

#define GL_VIEWS_FOV_COUNT 26
#define GL_VIEWS_FOV_DEF_INDEX 7 // 45 degrees
#define GL_VIEWS_CAMERA_FOV_FMT _T("FOV %2d")
#define GL_VIEWS_CAMERA_DEF_NEAR_PLANE 0.01f
#define GL_VIEWS_CAMERA_DEF_FAR_PLANE 10.0f

#define GL_VIEWS_MIN_VIEW_DX 20
#define GL_VIEWS_MIN_VIEW_DY 20

#define GL_VIEWS_NONAME _T("noname")
#define GL_VIEWS_ROOTNAME _T("root")
#define GL_VIEWS_SQUARE_NAME _T("square")
#define GL_VIEWS_OUTER_SCENE_NAME _T("outer scene")
#define GL_VIEWS_OUTER_SCENE_NAME _T("outer scene")
#define GL_VIEWS_PLANET_EARTH _T("earth")
#define GL_VIEWS_PLANET_MOON _T("moon")
#define GL_VIEWS_WND_AVI_PLAYER _T("avi player window")
#define GL_VIEWS_WND_MIRROR _T("mirror window")

#define GL_VIEWS_WM_RENDER_FRAME_COMPLETE (WM_USER+123)
#define GL_VIEWS_WM_QUERY_VIEW_VISIBILITY (WM_USER+124)
#define GL_VIEWS_WM_RENDER_INITIAL (WM_USER+125)
#define GL_VIEWS_ID_VIEW_DLG_CTRL_ID 0x1234

#define GL_VIEWS_TIMER_ID 0x654
#define GL_VIEWS_TIMER_ELAPSE (1000/30)

#define GL_VIEWS_DRAW_ANIMS

#define GL_VIEWS_TEXTURE_COUNT 9
#define GL_VIEWS_TEXTURE_IDX_CUBE_ORANGE 0
#define GL_VIEWS_TEXTURE_IDX_CUBE_WHITE 1
#define GL_VIEWS_TEXTURE_IDX_RING 2
#define GL_VIEWS_TEXTURE_IDX_BOTTOM_PLANE 3
#define GL_VIEWS_TEXTURE_IDX_WND_AVI_PLAYER 4
#define GL_VIEWS_TEXTURE_IDX_AVI_SURFACE 5
#define GL_VIEWS_TEXTURE_IDX_WND_MIRROR 6
#define GL_VIEWS_TEXTURE_IDX_EARTH 7
#define GL_VIEWS_TEXTURE_IDX_MOON 8

#define GL_VIEWS_TREE_IMG_IDX_GENERIC_OBJ 0
#define GL_VIEWS_TREE_IMG_IDX_ROOT 1
#define GL_VIEWS_TREE_IMG_IDX_MODIFIER_GRP 2
#define GL_VIEWS_TREE_IMG_IDX_MODIFIER_ONE 3
#define GL_VIEWS_TREE_IMG_IDX_CUBE_CENTER 4
#define GL_VIEWS_TREE_IMG_IDX_CUBE_LEAF 5
#define GL_VIEWS_TREE_IMG_IDX_TEXT_OBJ 6
#define GL_VIEWS_TREE_IMG_IDX_WND_MIRROR 7
#define GL_VIEWS_TREE_IMG_IDX_WND_AVI 8
#define GL_VIEWS_TREE_IMG_IDX_OUTER_SCENE 9
#define GL_VIEWS_TREE_IMG_IDX_CAMERA_OBJ 10

#ifdef _DEBUG
	#define GL_VIEWS_CHECK_OPENGL_ERROR \
		{ \
			GLenum _gl_errror = glGetError(); \
			ASSERT( _gl_errror == GL_NO_ERROR ); \
		}
#else
	#define GL_VIEWS_CHECK_OPENGL_ERROR \
		{ \
		}
#endif // _DEBUG

#define GL_VIEWS_CAMERA_DEFIDX_MAIN 8
#define GL_VIEWS_CAMERA_DEFIDX_0 9
#define GL_VIEWS_CAMERA_DEFIDX_1 5

typedef gltl::ntraits<GLfloat> _ntr;

typedef gltl::vector3<GLfloat,_ntr> _v3t;
typedef gltl::vector3<GLfloat,_ntr> & _v3rt;
typedef const gltl::vector3<GLfloat,_ntr> & _v3rct;

typedef gltl::vector4<GLfloat,_ntr> _v4t;
typedef gltl::vector4<GLfloat,_ntr> & _v4rt;
typedef const gltl::vector4<GLfloat,_ntr> & _v4rct;

typedef gltl::quaternion<GLfloat,_ntr> _q4t;
typedef gltl::quaternion<GLfloat,_ntr> & _q4rt;
typedef const gltl::quaternion<GLfloat,_ntr> & _q4rct;

typedef gltl::matrix<GLfloat,_ntr> _mt;
typedef gltl::matrix<GLfloat,_ntr> & _mrt;
typedef const gltl::matrix<GLfloat,_ntr> & _mrct;

/////////////////////////////////////////////////////////////////////////////
// C3DFont

class C3DFont
{
	bool m_bCreated:1;
	GLYPHMETRICSFLOAT m_GMF[256];
	GLuint m_nGlListBase;
public:
	
	C3DFont()
		: m_bCreated( false )
	{
	}
	
	C3DFont(
		HDC hDC,                    // dc for font creation
		int nHeight =               // height of font
			-12,
		int nWidth =                // average character width
			0,
		int nEscapement =           // angle of escapement
			0,
		int nOrientation =          // base-line orientation angle
			0,
		int fnWeight =              // font weight
			FW_BOLD,
		DWORD fdwItalic =           // italic attribute option
			FALSE,
		DWORD fdwUnderline =        // underline attribute option
			FALSE,
		DWORD fdwStrikeOut =        // strikeout attribute option
			FALSE,
		DWORD fdwCharSet =          // character set identifier
			ANSI_CHARSET,
		DWORD fdwOutputPrecision =  // output precision
			OUT_TT_PRECIS,
		DWORD fdwClipPrecision =    // clipping precision
			CLIP_DEFAULT_PRECIS,
		DWORD fdwQuality =          // output quality
			ANTIALIASED_QUALITY,
		DWORD fdwPitchAndFamily =   // pitch and family
			FF_DONTCARE|DEFAULT_PITCH,
		LPCTSTR lpszFace =          // typeface name
			_T("Times New Roman"),
		float deviation =           // specifies the maximum chordal deviation from the true outlines
			0.0f,
		float extrusion =           // extrusion value in the negative z direction
			0.02f,
		int format =                // specifies line segments or polygons in display lists
			WGL_FONT_POLYGONS
		)
		: m_bCreated( false )
	{
		CreateFont(
			hDC,                    // dc for font creation
			nHeight,                // height of font
			nWidth,                 // average character width
			nEscapement,            // angle of escapement
			nOrientation,           // base-line orientation angle
			fnWeight,               // font weight
			fdwItalic,              // italic attribute option
			fdwUnderline,           // underline attribute option
			fdwStrikeOut,           // strikeout attribute option
			fdwCharSet,             // character set identifier
			fdwOutputPrecision,     // output precision
			fdwClipPrecision,       // clipping precision
			fdwQuality,             // output quality
			fdwPitchAndFamily,      // pitch and family
			lpszFace,               // typeface name
			deviation,              // specifies the maximum chordal deviation from the true outlines
			extrusion,              // extrusion value in the negative z direction
			format                  // specifies line segments or polygons in display lists
			);
	}
	
	~C3DFont()
	{
		DeleteFont();
	}
	
	bool CreateFont(
		HDC hDC,                    // dc for font creation
		int nHeight =               // height of font
			-12,
		int nWidth =                // average character width
			0,
		int nEscapement =           // angle of escapement
			0,
		int nOrientation =          // base-line orientation angle
			0,
		int fnWeight =              // font weight
			FW_BOLD,
		DWORD fdwItalic =           // italic attribute option
			FALSE,
		DWORD fdwUnderline =        // underline attribute option
			FALSE,
		DWORD fdwStrikeOut =        // strikeout attribute option
			FALSE,
		DWORD fdwCharSet =          // character set identifier
			ANSI_CHARSET,
		DWORD fdwOutputPrecision =  // output precision
			OUT_TT_PRECIS,
		DWORD fdwClipPrecision =    // clipping precision
			CLIP_DEFAULT_PRECIS,
		DWORD fdwQuality =          // output quality
			ANTIALIASED_QUALITY,
		DWORD fdwPitchAndFamily =   // pitch and family
			FF_DONTCARE|DEFAULT_PITCH,
		LPCTSTR lpszFace =          // typeface name
			_T("Times New Roman"),
		float deviation =           // specifies the maximum chordal deviation from the true outlines
			0.0f,
		float extrusion =           // extrusion value in the negative z direction
			0.02f,
		int format =                // specifies line segments or polygons in display lists
			WGL_FONT_POLYGONS
		)
	{
		DeleteFont();
		m_nGlListBase = glGenLists( 256 );
		HFONT hFont =
			::CreateFont(
				nHeight,            // height of font
				nWidth,             // average character width
				nEscapement,        // angle of escapement
				nOrientation,       // base-line orientation angle
				fnWeight,           // font weight
				fdwItalic,          // italic attribute option
				fdwUnderline,       // underline attribute option
				fdwStrikeOut,       // strikeout attribute option
				fdwCharSet,         // character set identifier
				fdwOutputPrecision, // output precision
				fdwClipPrecision,   // clipping precision
				fdwQuality,         // output quality
				fdwPitchAndFamily,  // pitch and family
				lpszFace            // typeface name
				);
		if( hFont == NULL )
			return false;
		HFONT hPrevFont = (HFONT)::SelectObject( hDC, hFont );
		BOOL bCreated =
			::wglUseFontOutlines(
				hDC,                // device context of the outline font
				0,                  // first glyph to be turned into a display list
				255,                // number of glyphs to be turned into display lists
				m_nGlListBase,      // specifies the starting display list
				deviation,          // specifies the maximum chordal deviation from the true outlines
				extrusion,          // extrusion value in the negative z direction
				format,             // specifies line segments or polygons in display lists
				m_GMF               // address of buffer to receive glyph metric data
				);
		::SelectObject( hDC, hPrevFont );
		::DeleteObject( hFont );
		m_bCreated = bCreated ? true : false;
		return m_bCreated;
	}
	
	void DeleteFont()
	{
		if( !m_bCreated )
			return;
		::glDeleteLists( m_nGlListBase, 256 );
		m_bCreated = false;
	}
	
	bool IsFontCreated() const
	{
		return m_bCreated;
	}

	float TextOut(
		LPCTSTR sText,
		bool bMeasureOnly = false,
		bool bCenter = true
		)
	{
		if( !IsFontCreated() )
			return 0.0f;
		if( sText == NULL || (*sText) == _T('\0') )
			return 0.0f;
		float fLength = 0.0f;  // text length
		unsigned int nTextChars = (unsigned int) _tcslen( sText );
		if( bMeasureOnly || bCenter )
		{
			for(	unsigned int nIndex = 0;
					nIndex < nTextChars;
					nIndex++
					)
				fLength += m_GMF[ sText[nIndex] ].gmfCellIncX;
			if( fLength == 0.0f )
				return 0.0f;
		}
		if( bMeasureOnly )
			return fLength;
		if( bCenter )
			::glTranslatef( -fLength / 2.0f, 0.0f, 0.0f ); // center text
		::glPushAttrib( GL_LIST_BIT );
			::glListBase( m_nGlListBase );
			::glCallLists(
				nTextChars,
#ifdef _UNICODE
				GL_UNSIGNED_SHORT
#else
				GL_UNSIGNED_BYTE
#endif
				,
				sText
				);
		::glPopAttrib();
		return fLength;
	}

	float CharOut(
		TCHAR tChar,
		bool bMeasureOnly = false,
		bool bCenter = true
		)
	{
		if( !IsFontCreated() )
			return 0.0f;
		if( tChar == _T('\0') )
			return 0.0f;
		TCHAR sTextBuffer[2] = { tChar, _T('\0') };
		return TextOut( sTextBuffer, bMeasureOnly, bCenter );
	}

	float FormatOut(
		LPCTSTR sFormatSpec,
		...
		)
	{
		if( !IsFontCreated() )
			return 0.0f;
		if( sFormatSpec == NULL || (*sFormatSpec) == _T('\0') )
			return 0.0f;
		TCHAR sTextBuffer[1024];
		va_list _va_p;
		va_start( _va_p, sFormatSpec );
		__EXT_MFC_VSPRINTF( 
			__EXT_MFC_VSPRINTF_PARAM( sTextBuffer, 1024 ),
			sFormatSpec, 
			_va_p 
			);
		va_end( _va_p );
		return TextOut( sTextBuffer, false );
	}

}; // class C3DFont

/////////////////////////////////////////////////////////////////////////////
// C3DObject

class C3DCamera;
class C3DView;
class C3DMirror;
class CObjectHierarchyTreeCtrl;

class C3DObject : public CObject
{
protected:
	CString m_sName;
	C3DObject * m_pParent;
	CList < C3DObject *, C3DObject * > m_listChilds;
	HTREEITEM m_hti;
public:
	_v3t m_v3LocalScale, m_v3LocalTranslation;
	_q4t m_quatLocalOrientation;
	_mt m_mtxLastTransformation;
	bool m_bEnabledParentDependency:1;
	bool m_bEnabledScale:1;
	bool m_bEnabledTranslation:1;
	bool m_bEnabledOrientation:1;

	DECLARE_DYNCREATE( C3DObject );
	C3DObject(
		LPCTSTR sName = GL_VIEWS_NONAME,
		bool bEnabledParentDependency = true,
		bool bEnabledScale = false,
		bool bEnabledTranslation = true,
		bool bEnabledOrientation = true
		);
	virtual ~C3DObject();
	virtual void SerializeState(
		CArchive & ar
		);
	void LocalLoadIdentity()
	{
		ASSERT_VALID( this );
		m_v3LocalTranslation.load_vector();
		m_quatLocalOrientation.load_quaternion();
		m_v3LocalScale.load_vector( 1.0f, 1.0f, 1.0f );
	}
	void LocalTrail(
		GLfloat fTrailBy,
		bool bDoTrailLocalX,
		bool bDoTrailLocalY,
		bool bDoTrailLocalZ
		)
	{
		ASSERT_VALID( this );
		_q4t trailer( m_quatLocalOrientation );
		if( bDoTrailLocalZ )
		{
		}
		else if( bDoTrailLocalX )
		{
			trailer *=
				_q4t(
					0.0f,
					- _ntr::get_pi() / 2.0f,
					0.0f
					);
		}
		else if( bDoTrailLocalY )
		{
			trailer *=
				_q4t(
					_ntr::get_pi() / 2.0f,
					0.0f,
					0.0f
					);
		}
		GLfloat _x, _y, _z;
		trailer.get_direction_vector( _x, _y, _z );
		m_v3LocalTranslation.x += _x * fTrailBy;
		m_v3LocalTranslation.y += _y * fTrailBy;
		m_v3LocalTranslation.z += _z * fTrailBy;
	}

	void LocalAdjustPitch( GLfloat fAngleRads )
	{
		ASSERT_VALID( this );
		m_quatLocalOrientation *= _q4t( fAngleRads, 0.0f, 0.0f );
	}
	void LocalAdjustYaw( GLfloat fAngleRads )
	{
		ASSERT_VALID( this );
		m_quatLocalOrientation *= _q4t( 0.0f, fAngleRads, 0.0f );
	}
	void LocalAdjustRoll( GLfloat fAngleRads )
	{
		ASSERT_VALID( this );
		m_quatLocalOrientation *= _q4t( 0.0f, 0.0f, fAngleRads );
	}
	void LocalAdjustOrientation(
		GLfloat fAngleRadsPitch,
		GLfloat fAngleRadsYaw,
		GLfloat fAngleRadsRoll
		)
	{
		ASSERT_VALID( this );
		m_quatLocalOrientation *=
			_q4t( fAngleRadsPitch, fAngleRadsYaw, fAngleRadsRoll );
	}
	LPCTSTR GetName() const
	{
		ASSERT_VALID( this );
		return m_sName;
	}
	void AddChild( C3DObject * pChild )
	{
		ASSERT_VALID( this );
		ASSERT_VALID( pChild );
		m_listChilds.AddTail( pChild );
		pChild->m_pParent = this;
	}
	C3DObject * GetParent()
	{
		ASSERT_VALID( this );
		return m_pParent;
	}
	const C3DObject * GetParent() const
	{
		ASSERT_VALID( this );
		return m_pParent;
	}
	HTREEITEM GetHTREEITEM()
	{
		ASSERT_VALID( this );
		ASSERT( m_hti != NULL );
		return m_hti;
	}

	enum eWalkTreeQuery
	{
		EWTQ_THREAD_INIT	= 0L,
		EWTQ_THREAD_DONE	= 1L,
		EWTQ_PLAY			= 2L,
		EWTQ_TRANSFORM		= 3L,
		EWTQ_RENDER			= 4L,
		EWTQ_ADD_TO_TREE	= 5L,
	};
	virtual void WalkTree(
		eWalkTreeQuery walkTreeQuery,
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror = NULL
		);
	virtual bool OnWalkTree(
		eWalkTreeQuery walkTreeQuery,
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror
		);
	virtual void OnThreadInit( LPVOID lpvCookie );
	virtual void OnThreadDone( LPVOID lpvCookie );
	virtual void OnPlay(
		LPVOID lpvCookie
		);
	void CalcTransformation(
		_mrct mtxParentTransformation
		);
	virtual void OnTransform(
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie
		);
	virtual void OnRender(
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror
		);
	virtual bool IsRenderSubtree();
	virtual bool IsRenderSubtreeItem( C3DObject * pObjChild );
	virtual void OnAddToTree(
		CObjectHierarchyTreeCtrl * pTreeCtrl
		);
	virtual void OnTreeItemDblClick(
		CObjectHierarchyTreeCtrl * pTreeCtrl
		);
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);

	void GenerateSphere(
		int nTextureIndex,
		_v3rct ptCenter,
		GLfloat fRadius,
		int nStacksCount,
		GLfloat fAngleTheta0,
		GLfloat fAngleTheta1,
		GLfloat fAngleTheta1Phi0,
		GLfloat fAngleTheta1Phi1,
		bool bNoDepth = false
		);
	
	static bool stat_LoadResourceToMemory(
		LPCTSTR pszResId,
		LPCTSTR pszRsType,
		LPVOID * pLpvOutBuffer,
		DWORD * pDwOutSize
		);
	static bool stat_IsFileExists(
		LPCTSTR sFilePath
		);

}; // class C3DObject

/////////////////////////////////////////////////////////////////////////////
// C3DModifier

class C3DModifier : public C3DObject
{
protected:
	GLfloat
		m_fAnglePlayStepPitch, m_fAnglePlayStepYaw, m_fAnglePlayStepRoll;
public:
	DECLARE_DYNCREATE( C3DModifier );
	C3DModifier(
		LPCTSTR sName = GL_VIEWS_NONAME,
		GLfloat fAnglePlayStepPitch = 0.0f,
		GLfloat fAnglePlayStepYaw = 0.0f,
		GLfloat fAnglePlayStepRoll = 0.0f
		);
	~C3DModifier();
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);
	virtual void OnPlay(
		LPVOID lpvCookie
		);
}; // class C3DModifier

/////////////////////////////////////////////////////////////////////////////
// C3DOuterScene

class C3DSquare : public C3DObject
{
public:
	DECLARE_DYNCREATE( C3DSquare );

	int m_nTextureIndex;
	struct
	{
		GLfloat m_fX, m_fY;
	} m_arrTextureCoords[4];
	_v3t m_arrPoints[4];
	_v3t m_vecNormal;
	bool m_bAdjustAlphaFunc:1, m_bAdjustBlendFunc:1,
		m_bNoCullFace:1, m_bNoDepth:1, m_bUseNormal:1;
	GLenum m_gleBlendFactorS, m_gleBlendFactorD, m_gleAlphaFunc;
	GLclampf m_glcAlphaRef;
	
	C3DSquare();
	~C3DSquare();
	virtual void OnRender(
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror
		);
}; // class C3DSquare

/////////////////////////////////////////////////////////////////////////////
// C3DOuterScene

class C3DOuterScene : public C3DObject
{
public:
	DECLARE_DYNCREATE( C3DOuterScene );
	static volatile bool g_bRenderOuterScene;
	static GLfloat g_fBottomPlaneValX;
	static GLfloat g_fBottomPlaneValY;
	static GLfloat g_fBottomPlaneValZ;
	static GLfloat g_fRingRadius;
	static GLint g_nSphereValStacks;
	C3DOuterScene();
	~C3DOuterScene();
	virtual void OnThreadInit( LPVOID lpvCookie );
	bool IsRenderSubtree();
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);
}; // class C3DOuterScene

/////////////////////////////////////////////////////////////////////////////
// C3DCamera

class C3DCamera : public C3DObject
{
public:
	static const GLfloat g_arrFovValues[GL_VIEWS_FOV_COUNT];

	UINT m_nTreeDblClickCmdID;
	int m_nFovIndex;
	GLfloat m_fAspect, m_fNearPlane, m_fFarPlane;
	static volatile bool g_bRenderCameraAxes;
	static GLfloat g_fAxisLineLength;
	static GLfloat g_fAxisLineWidth;
	DECLARE_DYNCREATE( C3DCamera );
	C3DCamera(
		UINT nTreeDblClickCmdID = 0L,
		LPCTSTR sName = GL_VIEWS_NONAME,
		int nFovIndex = GL_VIEWS_FOV_DEF_INDEX,
		GLfloat fNearPlane = 1.0f,
		GLfloat fFarPlane = 5.0f
		);
	~C3DCamera();

	GLfloat GetFov() const
	{
		ASSERT_VALID( this );
		ASSERT( 0 <= m_nFovIndex && m_nFovIndex < GL_VIEWS_FOV_COUNT );
		return g_arrFovValues[m_nFovIndex];
	}
	void DoStepLeft( GLfloat fStepSize )
	{
		ASSERT_VALID( this );
		LocalTrail( fStepSize, true, false, false );
	}
	void DoStepUp( GLfloat fStepSize )
	{
		ASSERT_VALID( this );
		LocalTrail( fStepSize, false, true, false );
	}
	void DoStepForward( GLfloat fStepSize )
	{
		ASSERT_VALID( this );
		LocalTrail( fStepSize, false, false, true );
	}
	void DoLookLeft( GLfloat fAngleDegrees )
	{
		ASSERT_VALID( this );
		LocalAdjustYaw( _ntr::d2r(fAngleDegrees) );
	}
	void DoLookUp( GLfloat fAngleDegrees )
	{
		ASSERT_VALID( this );
		LocalAdjustPitch( _ntr::d2r(fAngleDegrees) );
	}
	void DoLookOwnAxis( GLfloat fAngleDegrees )
	{
		ASSERT_VALID( this );
		LocalAdjustRoll( _ntr::d2r(fAngleDegrees) );
	}

	virtual void SerializeState(
		CArchive & ar
		);

	virtual void OnRender(
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror
		);
	virtual void OnTreeItemDblClick(
		CObjectHierarchyTreeCtrl * pTreeCtrl
		);
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);
}; // class C3DCamera

/////////////////////////////////////////////////////////////////////////////
// C3DCube

class C3DCube : public C3DObject
{
protected:
	bool m_bCenterCube:1;
public:
	static volatile bool g_bRenderCubeObjects;
	static GLfloat g_fCubeVal;
	static GLfloat g_fTranslateCubeVal;
	static GLfloat g_fCenterCubePlayPitch;
	static GLfloat g_fCenterCubePlayYaw;
	static GLfloat g_fCenterCubePlayRoll;
	DECLARE_DYNCREATE( C3DCube );
	C3DCube(
		LPCTSTR sName = GL_VIEWS_NONAME,
		bool bCenterCube = false
		);
	~C3DCube();

	virtual void OnThreadInit( LPVOID lpvCookie );
	virtual void OnPlay(
		LPVOID lpvCookie
		);
	virtual bool IsRenderSubtreeItem( C3DObject * pObjChild );
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);
}; // class C3DCube

/////////////////////////////////////////////////////////////////////////////
// C3DPlanet

class C3DPlanet : public C3DObject
{
protected:
	int m_nPlanetTextureIndex;
	GLint m_nPlanetStacks;
	GLfloat m_fPlanetRadius,
		m_fAnglePlayStepPitch, m_fAnglePlayStepYaw, m_fAnglePlayStepRoll;
public:
	static volatile bool g_bRenderPlanetObjects;
	static GLfloat g_fPlanetRadiusEarth;
	static GLfloat g_fPlanetRadiusMoon;
	static GLint g_nPlanetSphereStacks;
	static GLfloat g_fAnglePlayStepPitchEarth;
	static GLfloat g_fAnglePlayStepPitchMoon;
	static GLfloat g_fAnglePlayStepYawEarth;
	static GLfloat g_fAnglePlayStepYawMoon;
	static GLfloat g_fAnglePlayStepRollEarth;
	static GLfloat g_fAnglePlayStepRollMoon;
	DECLARE_DYNCREATE( C3DPlanet );
	C3DPlanet(
		LPCTSTR sName = GL_VIEWS_NONAME,
		int nPlanetTextureIndex = GL_VIEWS_TEXTURE_IDX_EARTH,
		GLint nPlanetStacks = C3DPlanet::g_nPlanetSphereStacks,
		GLfloat fPlanetRadius = C3DPlanet::g_fPlanetRadiusEarth,
		GLfloat fAnglePlayStepPitch = C3DPlanet::g_fAnglePlayStepPitchEarth,
		GLfloat fAnglePlayStepYaw = C3DPlanet::g_fAnglePlayStepYawEarth,
		GLfloat fAnglePlayStepRoll = C3DPlanet::g_fAnglePlayStepRollEarth
		);
	~C3DPlanet();

	virtual void OnThreadInit( LPVOID lpvCookie );
	virtual bool IsRenderSubtreeItem( C3DObject * pObjChild );
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);
	virtual void OnPlay(
		LPVOID lpvCookie
		);
}; // class C3DPlanet

/////////////////////////////////////////////////////////////////////////////
// C3DText

class C3DText : public C3DObject
{
protected:
	GLfloat m_fRed, m_fGreen, m_fBlue;
	GLfloat m_fAnglePlayStepPitch, m_fAnglePlayStepYaw, m_fAnglePlayStepRoll;
public:
	static volatile bool g_bRenderTextObjects;
	static GLfloat g_fScaleModifier;
	static GLfloat g_fAnglePlayStepPitchText;
	static GLfloat g_fAnglePlayStepYawText;
	static GLfloat g_fAnglePlayStepRollText;

	DECLARE_DYNCREATE( C3DText );
	C3DText(
		LPCTSTR sName = GL_VIEWS_NONAME, // should be used to set displayed text or one letter
		COLORREF clrText = RGB(255,255,255),
		GLfloat fAnglePlayStepPitch = C3DText::g_fAnglePlayStepPitchText,
		GLfloat fAnglePlayStepYaw = C3DText::g_fAnglePlayStepYawText,
		GLfloat fAnglePlayStepRoll = C3DText::g_fAnglePlayStepRollText
		);
	~C3DText();

	virtual void OnPlay(
		LPVOID lpvCookie
		);
	virtual void OnRender(
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror
		);
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);
}; // class C3DText

/////////////////////////////////////////////////////////////////////////////
// C3DWnd

class C3DWnd : public C3DObject
{
protected:
	int m_nPlaneTextureIndex;
	_v3t m_arrWndPoints[4], m_arrContentPoints[4];
	_v3t m_lastNormal;
	void _RecalcNormalAndContentArea();
	static void stat_CalcWindowContentPlane(
		_v3t * arrWndPoints,
		_v3t * arrContentPoints
		);
public:
	DECLARE_DYNCREATE( C3DWnd );
	static _v3t g_arrDefPointsWndAviPlayer[4];
	static _v3t g_arrDefPointsWndMirror[4];
	C3DWnd(
		LPCTSTR sName = GL_VIEWS_WND_AVI_PLAYER,
		int nPlaneTextureIndex = GL_VIEWS_TEXTURE_IDX_WND_AVI_PLAYER,
		_v3rct pt0 = C3DWnd::g_arrDefPointsWndAviPlayer[0],
		_v3rct pt1 = C3DWnd::g_arrDefPointsWndAviPlayer[1],
		_v3rct pt2 = C3DWnd::g_arrDefPointsWndAviPlayer[2],
		_v3rct pt3 = C3DWnd::g_arrDefPointsWndAviPlayer[3]
		);
	~C3DWnd();

	virtual void OnRender(
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror
		);
}; // class C3DWnd

/////////////////////////////////////////////////////////////////////////////
// C3DMirror

class C3DMirror : public C3DWnd
{
protected:
	bool m_bRenderingThisMirror:1;
public:
	static volatile bool C3DMirror::g_bRenderMirrors;

	DECLARE_DYNCREATE( C3DMirror );
	C3DMirror(
		LPCTSTR sName = GL_VIEWS_WND_MIRROR,
		int nPlaneTextureIndex = GL_VIEWS_TEXTURE_IDX_WND_MIRROR,
		_v3rct pt0 = C3DWnd::g_arrDefPointsWndMirror[0],
		_v3rct pt1 = C3DWnd::g_arrDefPointsWndMirror[1],
		_v3rct pt2 = C3DWnd::g_arrDefPointsWndMirror[2],
		_v3rct pt3 = C3DWnd::g_arrDefPointsWndMirror[3]
		);
	~C3DMirror();

	virtual void OnRender(
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror
		);
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);
}; // class C3DMirror

/////////////////////////////////////////////////////////////////////////////
// C3DAviPlayer

class C3DAviPlayer : public C3DWnd
{
protected:
	bool m_bInitComplete:1;
	PAVISTREAM m_pAviStream;
	AVISTREAMINFO m_pAviInfo;
	PGETFRAME m_pGetFrame;
	BITMAPINFOHEADER m_dataBmpInfoHdr;
	int m_nAviWidth, m_nAviHeight,
		m_nRenderWidth, m_nRenderHeight, m_nRenderSize;
	HDRAWDIB m_hDrawDib;
	long m_nFrameNumber, m_nFrameCount;
	HDC m_hDC;
	HBITMAP m_hBitmap;
	unsigned char * m_pDibRawData;
	int m_nSurfaceTextureIndex;

	static void stat_AlertDisplay( LPCTSTR sText );

public:
	static volatile bool g_bRenderAviPlayers;

	DECLARE_DYNCREATE( C3DAviPlayer );
	C3DAviPlayer(
		LPCTSTR sName = GL_VIEWS_WND_AVI_PLAYER,
		int nPlaneTextureIndex = GL_VIEWS_TEXTURE_IDX_WND_AVI_PLAYER,
		int nSurfaceTextureIndex = GL_VIEWS_TEXTURE_IDX_AVI_SURFACE,
		_v3rct pt0 = C3DWnd::g_arrDefPointsWndAviPlayer[0],
		_v3rct pt1 = C3DWnd::g_arrDefPointsWndAviPlayer[1],
		_v3rct pt2 = C3DWnd::g_arrDefPointsWndAviPlayer[2],
		_v3rct pt3 = C3DWnd::g_arrDefPointsWndAviPlayer[3],
		int nRenderWidth = 256,
		int nRenderHeight = 256
		);
	~C3DAviPlayer();

	virtual void OnThreadInit( LPVOID lpvCookie );
	virtual void OnThreadDone( LPVOID lpvCookie );
	virtual void OnRender(
		C3DCamera * pCam,
		C3DView * pView3D,
		LPVOID lpvCookie,
		C3DMirror * pObjMirror
		);
	virtual void OnPlay(
		LPVOID lpvCookie
		);
	virtual void OnQueryTreeDisplayParms(
		CString & strTreeItemText,
		int & nTreeImageIndex
		);
}; // class C3DAviPlayer

/////////////////////////////////////////////////////////////////////////////
// C3DDC

class C3DDC : public CDC
{
protected:
	CBitmap m_bmp;
	HBITMAP m_hBmpOld;
	COLORREF * m_pColorSurface;
	CSize m_sizeView;
	void _Init()
	{
		if( GetSafeHdc() != NULL )
		{
			ASSERT( m_bmp.GetSafeHandle() != NULL );
			return;
		}
		ASSERT( m_bmp.GetSafeHandle() == NULL );
		CWindowDC dcDesktop( NULL );
		if( !CreateCompatibleDC(&dcDesktop) )
		{
			ASSERT( FALSE );
			return;
		}
		ASSERT( m_sizeView.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeView.cy >= GL_VIEWS_MIN_VIEW_DY );

		BITMAPINFOHEADER bih;
		::memset( (LPVOID)&bih, 0, sizeof(BITMAPINFOHEADER) );
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biWidth = m_sizeView.cx;
		bih.biHeight = m_sizeView.cy;
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = m_sizeView.cx * m_sizeView.cy;
		HBITMAP	hDIB =
			::CreateDIBSection(
				dcDesktop.GetSafeHdc(),
				(LPBITMAPINFO)&bih,
				DIB_RGB_COLORS,
				(LPVOID*)&m_pColorSurface,
				NULL,
				NULL
				);
		if( hDIB == NULL || m_pColorSurface == NULL )
		{
			CDC::DeleteDC();
			ASSERT( FALSE );
			return;
		}
		m_bmp.Attach( hDIB );

		m_hBmpOld = (HBITMAP)::SelectObject( GetSafeHdc(), m_bmp.GetSafeHandle() );
	}
	void _Done()
	{
		if( GetSafeHdc() != NULL )
		{
			::SelectObject( GetSafeHdc(), m_hBmpOld );
			m_hBmpOld = NULL;
			CDC::DeleteDC();
			m_pColorSurface = NULL;
		}
		if( m_bmp.GetSafeHandle() != NULL )
			m_bmp.DeleteObject();
	}
public:
	C3DDC()
		: m_hBmpOld( NULL )
		, m_pColorSurface( NULL )
		, m_sizeView( GL_VIEWS_MIN_VIEW_DX, GL_VIEWS_MIN_VIEW_DY )
	{
	}
	~C3DDC()
	{
		_Done();
	}
	BOOL DeleteDC()
	{
		_Done();
		return TRUE;
	}
	CBitmap & GetInternalBitmap()
	{
		ASSERT_VALID( this );
		_Init();
		ASSERT( GetSafeHdc() != NULL );
		ASSERT( m_bmp.GetSafeHandle() != NULL );
		ASSERT( m_pColorSurface != NULL );
		return m_bmp;
	}
	COLORREF * GetColorSurface()
	{
		ASSERT_VALID( this );
		ASSERT( GetSafeHdc() != NULL );
		ASSERT( m_bmp.GetSafeHandle() != NULL );
		ASSERT( m_pColorSurface != NULL );
		return m_pColorSurface;
	}
	LONG GetColorSurfaceSize()
	{
		ASSERT_VALID( this );
		ASSERT( m_sizeView.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeView.cy >= GL_VIEWS_MIN_VIEW_DY );
		return m_sizeView.cx*m_sizeView.cy*4;
	}
	CSize GetViewSize() const
	{
		ASSERT_VALID( this );
		ASSERT( m_sizeView.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeView.cy >= GL_VIEWS_MIN_VIEW_DY );
		return m_sizeView;
	}
	void SetViewSize( CSize sizeView, bool bForceReInit )
	{
		ASSERT_VALID( this );
		ASSERT( m_sizeView.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeView.cy >= GL_VIEWS_MIN_VIEW_DY );
		sizeView.cx = max( GL_VIEWS_MIN_VIEW_DX, sizeView.cx );
		sizeView.cy = max( GL_VIEWS_MIN_VIEW_DY, sizeView.cy );
		if( m_sizeView == sizeView && (!bForceReInit) )
			return;
		_Done();
		m_sizeView = sizeView;
		_Init();
		ASSERT( GetSafeHdc() != NULL );
	}
};

/////////////////////////////////////////////////////////////////////////////
// C3DTexture

class C3DTexture
{
	CSize m_sizeTexture;
	LPVOID m_pData;
	bool m_bAlphaLayerExist:1;
	void _Destroy();
public:
	C3DTexture();
	~C3DTexture();
	CSize GetSize() const
	{
		return m_sizeTexture;
	}
	int GetWidth() const
	{
		return m_sizeTexture.cx;
	}
	int GetHeight() const
	{
		return m_sizeTexture.cy;
	}
	LPVOID GetData()
	{
		return m_pData;
	}
	LPCVOID GetData() const
	{
		return m_pData;
	}
	bool IsAlphaLayerExist() const
	{
		return m_bAlphaLayerExist;
	}
	bool IsEmpty() const
	{
		if( m_sizeTexture.cx == 0 )
		{
			ASSERT( m_sizeTexture.cy == 0 );
			ASSERT( m_pData == NULL );
			return true;
		}
		ASSERT( m_sizeTexture.cy != 0 );
		ASSERT( m_pData != NULL );
		return false;
	}
	void Empty()
	{
		_Destroy();
	}
	bool LoadResourceBitmapAs32Bit(
		LPCTSTR lpszResource,
		unsigned char nAlphaClearVal = 255
		);
	bool AddAlphaLayer( unsigned char nAlphaClearVal = 255 );
	bool SetAlphaLayer( unsigned char nAlphaSetVal = 255 );
	bool SetAlphaLayerIf(
		unsigned char nAlphaSetVal,
		COLORREF clr
		);
	bool SetAlphaLayerNB();
}; // class C3DTexture

/////////////////////////////////////////////////////////////////////////////
// C3DView

class C3DView : public CObject
{
protected:
	HWND m_hWndOutput;
	CSize m_sizeView, m_sizeViewNE;
	C3DDC m_dc;
	HGLRC m_hOpenGlContext;
	int m_nCameraIndex;
	void _Init();
	void _Done();
	CCriticalSection m_csGDI;
public:
	GLuint m_TextureIds[GL_VIEWS_TEXTURE_COUNT];
	C3DFont m_Font3D;
	CString m_strViewName;
	
	DECLARE_DYNCREATE( C3DView );
	C3DView(
		HWND hWndOutput = NULL
		);
	virtual ~C3DView();
	virtual void Lock();
	virtual void Unlock();
	bool IsViewVisible() const;
	CSize GetViewSize() const
	{
		ASSERT_VALID( this );
		ASSERT( m_sizeView.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeView.cy >= GL_VIEWS_MIN_VIEW_DY );
		ASSERT( m_sizeViewNE.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeViewNE.cy >= GL_VIEWS_MIN_VIEW_DY );
		return m_sizeView;
	}
	void SetViewSize( CSize sizeView );
	C3DDC & Get3DDC()
	{
		ASSERT_VALID( this );
		ASSERT( m_dc.GetSafeHdc() != NULL );
		return m_dc;
	}
	HGLRC GetOpenGlContext()
	{
		ASSERT_VALID( this );
		return m_hOpenGlContext;
	}
	HWND GetOutputHWND()
	{
		ASSERT_VALID( this );
		ASSERT( m_hWndOutput != NULL );
		ASSERT( ::IsWindow(m_hWndOutput) );
		return m_hWndOutput;
	}
	enum eBufferBitsType
	{
		__EBB_COLOR,
		__EBB_Z,
		__EBB_STENCIL,
		__EBB_ALPHA,
		__EBB_ACCUM,
		__EBB_AUXILIARY,
	};
	virtual BYTE OnGlGetBufferBits( eBufferBitsType _ebb )
	{
		if( _ebb == __EBB_COLOR )
			return 32;
		if( _ebb == __EBB_Z )
			return 16;
		if( _ebb == __EBB_STENCIL )
			return 8;
		return 0;
	}
	virtual HWND OnGlAlertGetParentHWND()
	{
		return ::GetDesktopWindow();
	}
	virtual void OnGlAlertDisplay(
		LPCTSTR sErrorText,
		LPCTSTR sErrorCaption = _T("Error")
		)
	{
		ASSERT( sErrorText != NULL );
		ASSERT( sErrorCaption != NULL );
		::ProfUISMsgBox( OnGlAlertGetParentHWND(), sErrorText, sErrorCaption, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL );
	}

	void AdjustViewSize(
		bool bLockView = true
		);

	int GetCameraIndex()
	{
		ASSERT_VALID( this );
		Lock();
		int nCameraIndex = m_nCameraIndex;
		ASSERT( 0 <= nCameraIndex && nCameraIndex < GL_VIEWS_CAMERA_COUNT );
		Unlock();
		return nCameraIndex;
	}
	void SetCameraIndex( int nCameraIndex )
	{
		ASSERT_VALID( this );
		Lock();
		ASSERT( 0 <= nCameraIndex && nCameraIndex < GL_VIEWS_CAMERA_COUNT );
		m_nCameraIndex = nCameraIndex;
		Unlock();
	}

	void DoStepLeft( GLfloat fStepSize );
	void DoStepUp( GLfloat fStepSize );
	void DoStepForward( GLfloat fStepSize );
	void DoLookLeft( GLfloat fAngleDegrees );
	void DoLookUp( GLfloat fAngleDegrees );
	void DoLookOwnAxis( GLfloat fAngleDegrees );

}; // class C3DView

/////////////////////////////////////////////////////////////////////////////
// C3DPipeThread

class C3DPipeThread : public CWinThread
{
	friend class C3DView;
protected:
	bool _ResourcesInit();
	void _ResourcesFree();
	
	C3DObject * m_pRoot;
	CArray < C3DView *, C3DView * > m_arrViews;
	CArray < C3DCamera *, C3DCamera * > m_arrCams;
	
	bool m_bInitComplete:1;

	CEvent
		m_eventShutdownStart,
		m_eventShutdownComplete,
		m_eventRenderViews;
	CCriticalSection m_csObjectWriteAccess;
public:
	volatile bool m_bTimerAnimationEnabled;

	C3DPipeThread();
	virtual ~C3DPipeThread();

	void ObjectWriteAccessSet( bool bLock )
	{
		if( bLock )
			m_csObjectWriteAccess.Lock();
		else
			m_csObjectWriteAccess.Unlock();
	}

	// thread initialization
	virtual BOOL InitInstance();
	// thread deletion
	virtual void Delete();
	// running and idle processing
	virtual int Run();

	// must be called from other thread to signal render event
	void Render();

	// must be called from other thread to signal shutdown
	void ShutdownAndWaitFor();

	// get root
	C3DObject * GetRoot();
	const C3DObject * GetRoot() const;

	// view operation
	void AddView( C3DView * pView3D );
	int GetViewCount() const;
	C3DView * GetView( int nIndex );

	// camera operations
	void AddCamera( C3DCamera * pCam );
	int GetCameraCount() const;
	C3DCamera * GetCamera( int nIndex );

}; // class C3DPipeThread

extern C3DPipeThread the3DPipe;

/////////////////////////////////////////////////////////////////////////////
// CCameraSelectionComboBox window

class CCameraSelectionComboBox : public CExtComboBox
{

// Construction
public:
	DECLARE_DYNAMIC( CCameraSelectionComboBox )
	CCameraSelectionComboBox();

// Attributes
public:

// Operations
public:
	void SyncCameraWithSelectedItem(
		bool bSetFocusToView = false
		);
	int SetCurSel(
		int nSelect,
		bool bSetFocusToView = false
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraSelectionComboBox)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCameraSelectionComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCameraSelectionComboBox)
	//}}AFX_MSG
	afx_msg void OnReflectCbnSelEndOK();
	DECLARE_MESSAGE_MAP()
}; // class CCameraSelectionComboBox

/////////////////////////////////////////////////////////////////////////////
// CCameraFovComboBox window

class CCameraFovComboBox : public CExtComboBox
{

// Construction
public:
	DECLARE_DYNAMIC( CCameraFovComboBox )
	CCameraFovComboBox();

// Attributes
public:

// Operations
public:
	void SyncCameraWithSelectedItem(
		bool bSetFocusToView = false
		);
	int SetCurSel(
		int nSelect,
		bool bSetFocusToView = false
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraFovComboBox)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCameraFovComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCameraFovComboBox)
	//}}AFX_MSG
	afx_msg void OnReflectCbnSelEndOK();
	DECLARE_MESSAGE_MAP()
}; // class CCameraFovComboBox


/////////////////////////////////////////////////////////////////////////////
// CObjectHierarchyTreeCtrl window

class CObjectHierarchyTreeCtrl : public CExtNCSB < CTreeCtrl >
{
// Construction
public:
	DECLARE_DYNAMIC( CObjectHierarchyTreeCtrl )
	CObjectHierarchyTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectHierarchyTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CObjectHierarchyTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObjectHierarchyTreeCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CObjectHierarchyTreeCtrl

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CExtWRB < CWnd >
{
// Construction
public:
	DECLARE_DYNAMIC( CChildView )
	CChildView(
		UINT nIdResourceCursor
		);

// Attributes
public:
	
	class CNavigationBar : public CExtToolControlBar
	{
	public:
		CCameraSelectionComboBox m_wndCameraSelCombo;
		CCameraFovComboBox m_wndCameraFovCombo;

		CNavigationBar()
		{
		}
		~CNavigationBar()
		{
		}
//		CExtBarContentExpandButton * OnCreateBarRightBtn()
//		{
//			return NULL;
//		}

		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			if( message == WM_CONTEXTMENU )
				return 0L;
			if(		message == WM_LBUTTONDOWN
				||	message == WM_LBUTTONUP
				||	message == WM_MBUTTONDOWN
				||	message == WM_MBUTTONUP
				||	message == WM_RBUTTONDOWN
				||	message == WM_RBUTTONUP
				)
			{
				HWND hWndThis = GetSafeHwnd();
				ASSERT( hWndThis != NULL );
				ASSERT( ::IsWindow( hWndThis ) );
				HWND hWndParent = ::GetParent( hWndThis );
				ASSERT( hWndParent != NULL );
				ASSERT( ::IsWindow( hWndParent ) );
				HWND hWndFocus = ::GetFocus();
				if(		hWndFocus == NULL
					||	hWndFocus == hWndThis
					||	::IsChild( hWndThis, hWndFocus )
					)
					::SetFocus( hWndParent );
				else if( ! ::IsChild( hWndParent, hWndFocus ) )
					::SetFocus( hWndParent );
			}
			return CExtToolControlBar::WindowProc(message,wParam,lParam);
		}

		bool InitNavigationBar()
		{
			m_bPresubclassDialogMode = true;

			if(	!m_wndCameraSelCombo.Create(
					WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,
					CRect( 0, 0, 100, 400 ),
					this,
					ID_CAMERA_SELECTION_COMBO
					)
				)
			{
				ASSERT( FALSE );
				return  false;
			}
			VERIFY(
				SetButtonCtrl(
					CommandToIndex(ID_CAMERA_SELECTION_COMBO),
					&m_wndCameraSelCombo
					)
				);
			for( int nCamIdx = 0; nCamIdx < GL_VIEWS_CAMERA_COUNT; nCamIdx++ )
			{
				CString sCamName;
				sCamName.Format( GL_VIEWS_CAMERA_NAME_FMT, nCamIdx );
				m_wndCameraSelCombo.InsertString( nCamIdx, sCamName );
			}
			m_wndCameraSelCombo.SetCurSel( 0 );
			m_wndCameraSelCombo.SetFont( CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)) );

			if(	!m_wndCameraFovCombo.Create(
					WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,
					CRect( 0, 0, 100, 400 ),
					this,
					ID_CAMERA_FOV_COMBO
					)
				)
			{
				ASSERT( FALSE );
				return  false;
			}
			VERIFY(
				SetButtonCtrl(
					CommandToIndex(ID_CAMERA_FOV_COMBO),
					&m_wndCameraFovCombo
					)
				);
			for( int nFovIdx = 0; nFovIdx < GL_VIEWS_FOV_COUNT; nFovIdx++ )
			{
				CString sFovName;
				int nFovDegrees = (int)
					(
						(
							( C3DCamera::g_arrFovValues[nFovIdx] * 180.0f )
								/ 3.1415926535897932384626433832795f
						) + 0.4f
					);
				ASSERT( nFovDegrees >= 10 && nFovDegrees <= 180 );
				sFovName.Format( GL_VIEWS_CAMERA_FOV_FMT, nFovDegrees );
				m_wndCameraFovCombo.InsertString( nFovIdx, sFovName );
			}
			m_wndCameraFovCombo.SetCurSel( GL_VIEWS_FOV_DEF_INDEX );
			m_wndCameraFovCombo.SetFont( CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)) );

			CMenu menuRotate, menuTranslate;
			if(		!menuRotate.LoadMenu(IDR_MENU_CAM_ROTATE)
				||	!menuTranslate.LoadMenu(IDR_MENU_CAM_TRANSLATE)
				)
			{
				ASSERT( FALSE );
				return  false;
			}

			int nIdxBtnRotate =
				CommandToIndex(ID_BTN_MENU_ROTATION);
			VERIFY(
				SetButtonMenu(
					nIdxBtnRotate,
					menuRotate.Detach()
					)
				);
			int nIdxBtnTranslate =
				CommandToIndex(ID_BTN_MENU_TRANSLATION);
			VERIFY(
				SetButtonMenu(
					nIdxBtnTranslate,
					menuTranslate.Detach()
					)
				);
			
			CExtBarButton * pTBB;

			pTBB = GetButton( nIdxBtnRotate );
			ASSERT_VALID( pTBB );
			pTBB->SetSeparatedDropDown();
			pTBB->SetAutoChangeID();
			pTBB->SetCmdID( ID_ROTATE_ABOUT_X0, true );

			pTBB = GetButton( nIdxBtnTranslate );
			ASSERT_VALID( pTBB );
			pTBB->SetSeparatedDropDown();
			pTBB->SetAutoChangeID();
			pTBB->SetCmdID( ID_TRANSLATE_X0, true );

			return true;
		}

	}; // class CNavigationBar

	class COutputWnd : public CWnd
	{
		C3DView * m_pView3D;
		C3DDC m_dc;
		HCURSOR m_hCursor;
		void _SafeResize3D( bool bForceReInit )
		{
			if( m_pView3D == NULL )
				return;
			ASSERT_VALID( m_pView3D );
			CRect rcClient;
			GetClientRect( &rcClient );
			m_pView3D->SetViewSize( rcClient.Size() );
			m_dc.SetViewSize( rcClient.Size(), bForceReInit );
			the3DPipe.Render();
		}
		bool _CanUpdate()
		{
			if( CExtControlBar::FindHwndNotSafeForMutualPaint(this) != NULL )
				return false;
			if(		CExtPopupMenuWnd::g_bMenuWithShadows
				&&	(! g_PaintManager.m_bIsWin2000orLater )
				)
			{
				if( CExtToolControlBar::g_bMenuTracking )
					return false;
				if( CExtPopupMenuWnd::IsMenuTracking() )
					return false;
			} // if( CExtPopupMenuWnd::g_bMenuWithShadows ...
			return true;
		}

		enum eMouseMotionHook
		{
			EMMH_NONE = 0,
			EMMH_LOOK_UP = 1,
			EMMH_LOOK_LEFT = 2,
			EMMH_LOOK_OWN_AXIS = 3,
			EMMH_WALK_FORWARD = 4,
			EMMH_WALK_LEFT = 5,
			EMMH_WALK_UP = 6,
		};
		eMouseMotionHook m_eMMH;
		CPoint m_ptScreenMMH;
	public:
		COutputWnd( UINT nIdResourceCursor )
			: m_pView3D( NULL )
			, m_eMMH( EMMH_NONE )
			, m_ptScreenMMH( -1, -1 )
		{
			HINSTANCE hInst = ::AfxGetInstanceHandle();
			m_hCursor = (HCURSOR)
				::LoadImage(
					hInst,
					MAKEINTRESOURCE(nIdResourceCursor),
					IMAGE_CURSOR,
					0,
					0,
					0
					);
			ASSERT( m_hCursor != NULL );
		}
		~COutputWnd()
		{
			if( m_hCursor != NULL )
				::DestroyCursor( m_hCursor );
		}
		bool Create( CWnd * pWndParent, UINT nDlgCtrlID )
		{
			ASSERT_VALID( this );
			ASSERT_VALID( pWndParent );
			ASSERT( m_hWnd == NULL );
			ASSERT( pWndParent->m_hWnd != NULL );
			ASSERT( ::IsWindow(pWndParent->m_hWnd) );
			if(	!CWnd::Create(
					::AfxRegisterWndClass(0),
					NULL,
					WS_CHILD|WS_VISIBLE,
					CRect( 0, 0, 0, 0 ),
					pWndParent,
					nDlgCtrlID,
					NULL
					)
				)
			{
				ASSERT( FALSE );
				return false;
			}
			m_pView3D = new C3DView( m_hWnd );
			the3DPipe.AddView( m_pView3D );
			return true;
		}
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			if( message == GL_VIEWS_WM_QUERY_VIEW_VISIBILITY )
			{
				if( !IsWindowVisible() )
					return 0L;
				CWnd * pWndParent = GetParent();
				ASSERT_VALID( pWndParent );
				if( !pWndParent->IsWindowVisible() )
					return 0L;
				CExtControlBar * pBar =
					DYNAMIC_DOWNCAST(
						CExtControlBar,
						pWndParent
						);
				if( pBar == NULL )
					return 1L;
				if( !pBar->IsVisible() )
					return 0L;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
				if( !pBar->AutoHideModeGet() )
				{
					pWndParent = pBar->GetParent();
					ASSERT_VALID( pWndParent );
					CExtDockDynTabBar * pDockBar =
						DYNAMIC_DOWNCAST(
							CExtDockDynTabBar,
							pWndParent
							);
					if( pDockBar == NULL )
						return 1L;
					pWndParent = pDockBar->GetParent();
					ASSERT_VALID( pWndParent );
					CExtDynTabControlBar * pTabDocker =
						STATIC_DOWNCAST(
							CExtDynTabControlBar,
							pWndParent
							);
					LONG nOwnIndex =
						pTabDocker->FindControlBar( pBar );
					ASSERT( nOwnIndex >= 0 );
					LONG nSelIndex =
						pTabDocker->GetSwitcherSelection();
					if( nOwnIndex != nSelIndex )
						return 0L;
				}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
				return 1L;
			} // if( message == GL_VIEWS_WM_QUERY_VIEW_VISIBILITY )
			if( message == GL_VIEWS_WM_RENDER_FRAME_COMPLETE )
			{
				ASSERT_VALID( m_pView3D );
				if( !_CanUpdate() )
				{
					_SafeResize3D( false );
					CExtPopupMenuWnd::PassMsgLoop( false );
					return 1L;
				}
				C3DDC & dcSrc = m_pView3D->Get3DDC();
				LONG nSurfaceSizeDst = m_dc.GetColorSurfaceSize();
				LONG nSurfaceSizeSrc = dcSrc.GetColorSurfaceSize();
				if( nSurfaceSizeDst != nSurfaceSizeSrc )
				{
					_SafeResize3D( true );
					CExtPopupMenuWnd::PassMsgLoop( false );
					return 1L;
				}
				COLORREF * pSurfDst = m_dc.GetColorSurface();
				COLORREF * pSurfSrc = m_pView3D->Get3DDC().GetColorSurface();
				::memmove( (LPVOID)pSurfDst, (LPVOID)pSurfSrc, nSurfaceSizeDst );
				Invalidate();
				UpdateWindow();
				CExtPopupMenuWnd::PassMsgLoop( false );
				return 1L;
			} // if( message == GL_VIEWS_WM_RENDER_FRAME_COMPLETE )
			if(		message == WM_TIMER
				&&	wParam == GL_VIEWS_TIMER_ID
				)
			{
				the3DPipe.Render();
				return 0L;
			}
			if( message == WM_ERASEBKGND )
				return 1L;
			if( message == WM_PAINT )
			{
// #ifdef _DEBUG
// static int g_nPaintCounter = 0;
// TRACE1( ">>> output painting %d\r\n", g_nPaintCounter++ );
// if( CExtPopupMenuWnd::IsKeyPressed( VK_CONTROL ) )
// {
// 	__asm int 3;
// }
// #endif // _DEBUG
				CRect rcClient;
				GetClientRect( &rcClient );
				ASSERT( rcClient.left == 0 && rcClient.top == 0 );
				CPaintDC dcPaint( this );
				if( rcClient.right <= 0 || rcClient.bottom <= 0 )
					return 0L;
				
				if(		m_pView3D == NULL
					||	m_dc.GetSafeHdc() == NULL
					)
				{
					dcPaint.FillSolidRect(
						&rcClient,
						RGB(0,0,0)
						);
					return 0L;
				} // if( m_pView3D == NULL )
				CSize sizeBuffer = m_dc.GetViewSize();
				if(		sizeBuffer.cx == rcClient.right
					&&	sizeBuffer.cy == rcClient.bottom
					)
					dcPaint.BitBlt(
						0, 0, rcClient.right, rcClient.bottom,
						&m_dc,
						0, 0,
						SRCCOPY
						);
				else
					dcPaint.StretchBlt(
						0, 0, rcClient.right, rcClient.bottom,
						&m_dc,
						0, 0, sizeBuffer.cx, sizeBuffer.cy,
						SRCCOPY
						);
				return 0L;
			} // if( message == WM_PAINT )
			if( message == WM_NCCALCSIZE )
			{
				NCCALCSIZE_PARAMS * pNCCSP =
					reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
				ASSERT( pNCCSP != NULL );
				CRect rcInBarWnd( pNCCSP->rgrc[0] );
				CSize sizeDeflate(
					min( 4, ::GetSystemMetrics(SM_CXSIZEFRAME) ),
					min( 4, ::GetSystemMetrics(SM_CYSIZEFRAME) )
					);
				rcInBarWnd.DeflateRect( sizeDeflate );
				::CopyRect( &(pNCCSP->rgrc[0]), rcInBarWnd );
				return 0;
			} // if( message == WM_NCCALCSIZE )
			if( message == WM_NCPAINT )
			{
				CRect rcInBarWnd, rcInBarClient;
				GetWindowRect( &rcInBarWnd );
				GetClientRect( &rcInBarClient );
				ClientToScreen( &rcInBarClient );
				if( rcInBarWnd == rcInBarClient )
					return 0;
				CPoint ptDevOffset = -rcInBarWnd.TopLeft();
				rcInBarWnd.OffsetRect( ptDevOffset );
				rcInBarClient.OffsetRect( ptDevOffset );
				CWindowDC dc( this );
				ASSERT( dc.GetSafeHdc() != NULL );
				dc.ExcludeClipRect( &rcInBarClient );
				dc.FillSolidRect(
					&rcInBarWnd,
					g_PaintManager->GetColor(
						CExtPaintManager::CLR_3DFACE_OUT,
						this
						)
					);
				return 0;
			} // if( message == WM_NCPAINT )
			if( message == WM_CONTEXTMENU )
				return 0L;
			if(		message == WM_LBUTTONDOWN
				||	message == WM_LBUTTONUP
				||	message == WM_MBUTTONDOWN
				||	message == WM_MBUTTONUP
				||	message == WM_RBUTTONDOWN
				||	message == WM_RBUTTONUP
				)
			{ // if mouse clicks
				HWND hWndThis = GetSafeHwnd();
				ASSERT( hWndThis != NULL );
				ASSERT( ::IsWindow( hWndThis ) );
				HWND hWndFocus = ::GetFocus();
				if( hWndFocus != hWndThis )
					::SetFocus( hWndThis );
				if( message == WM_LBUTTONDOWN )
				{
					if( CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) )
						m_eMMH = EMMH_WALK_LEFT;
					else if( CExtPopupMenuWnd::IsKeyPressed(VK_SHIFT) )
						m_eMMH = EMMH_WALK_UP;
					else
						m_eMMH = EMMH_WALK_FORWARD;
					VERIFY( ::GetCursorPos(&m_ptScreenMMH) );
					SetCapture();
				}
				else if( message == WM_RBUTTONDOWN )
				{
					if( CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) )
						m_eMMH = EMMH_LOOK_OWN_AXIS;
					else if( CExtPopupMenuWnd::IsKeyPressed(VK_SHIFT) )
						m_eMMH = EMMH_LOOK_UP;
					else
						m_eMMH = EMMH_LOOK_LEFT;
					VERIFY( ::GetCursorPos(&m_ptScreenMMH) );
					SetCapture();
				}
				else if( message == WM_LBUTTONUP || message == WM_RBUTTONUP )
				{
					if( m_eMMH != EMMH_NONE )
					{
						m_eMMH = EMMH_NONE;
						m_ptScreenMMH.x = m_ptScreenMMH.y = -1;
						ReleaseCapture();
					}
				}
				return 0;
			} // if mouse clicks
			if( message == WM_MOUSEMOVE )
			{
				if( m_pView3D == NULL )
					return 0;
				ASSERT_VALID( m_pView3D );
				CPoint ptScreen;
				VERIFY( ::GetCursorPos(&ptScreen) );
				CPoint ptDiff(
					m_ptScreenMMH.x - ptScreen.x,
					m_ptScreenMMH.y - ptScreen.y
					);
				const GLfloat fDivAngle = 4.0f;
				const GLfloat fDivStep = 200.0f;
				switch( m_eMMH )
				{
				case EMMH_LOOK_UP:
					m_pView3D->DoLookUp(
						- ((GLfloat)ptDiff.y) / fDivAngle
						);
					m_ptScreenMMH = ptScreen;
					the3DPipe.Render();
					break;
				case EMMH_LOOK_LEFT:
					m_pView3D->DoLookLeft(
						-( (GLfloat)ptDiff.x ) / fDivAngle
						);
					m_ptScreenMMH = ptScreen;
					the3DPipe.Render();
					break;
				case EMMH_LOOK_OWN_AXIS:
					m_pView3D->DoLookOwnAxis(
						( (GLfloat)ptDiff.x ) / fDivAngle
						);
					m_ptScreenMMH = ptScreen;
					the3DPipe.Render();
					break;
				case EMMH_WALK_FORWARD:
					m_pView3D->DoStepForward(
						- ( (GLfloat)ptDiff.y ) / fDivStep
						);
					m_ptScreenMMH = ptScreen;
					the3DPipe.Render();
					break;
				case EMMH_WALK_LEFT:
					m_pView3D->DoStepLeft(
						- ( (GLfloat)ptDiff.x ) / fDivStep
						);
					m_ptScreenMMH = ptScreen;
					the3DPipe.Render();
					break;
				case EMMH_WALK_UP:
					m_pView3D->DoStepUp(
						( (GLfloat)ptDiff.y ) / fDivStep
						);
					m_ptScreenMMH = ptScreen;
					the3DPipe.Render();
					break;
				}
				return 0;
			} // if( message == WM_MOUSEMOVE )
			if( message == WM_CAPTURECHANGED )
			{
				if( m_hWnd != ((HWND)lParam) )
				{
					m_eMMH = EMMH_NONE;
					m_ptScreenMMH.x = m_ptScreenMMH.y = -1;
				}
				return 0L;
			} // if( message == WM_CAPTURECHANGED )
			if( message == WM_MOUSEWHEEL )
			{
				if( m_pView3D == NULL )
					return 0;
				ASSERT_VALID( m_pView3D );
				int nWheelDelta = (int)((short)(HIWORD(wParam)));
				if( nWheelDelta == 0 )
					return 0;
				const GLfloat fAngle = 5.0f;
				if( CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) )
				{
					if( CExtPopupMenuWnd::IsKeyPressed(VK_SHIFT) )
					{
						CWnd * pViewWnd = GetParent();
						ASSERT_VALID( pViewWnd );
						pViewWnd->SendMessage(
							WM_COMMAND,
							( nWheelDelta < 0 )
								? ID_CAM_FOV_DEC
								: ID_CAM_FOV_INC
							);
						return 0;
					} // if( CExtPopupMenuWnd::IsKeyPressed(VK_SHIFT) )
					else
						m_pView3D->DoLookOwnAxis(
							(nWheelDelta < 0) ? (fAngle) : (-fAngle)
							);
				} // if( CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) )
				else if( CExtPopupMenuWnd::IsKeyPressed(VK_SHIFT) )
					m_pView3D->DoLookLeft(
						(nWheelDelta < 0) ? (fAngle) : (-fAngle)
						);
				else
					m_pView3D->DoLookUp(
						(nWheelDelta < 0) ? (-fAngle) : (fAngle)
						);
				the3DPipe.Render();
				return 0;
			} // if( message == WM_MOUSEWHEEL )
			if(		message == WM_SIZE
				||	message == WM_WINDOWPOSCHANGED
				||	message == WM_DISPLAYCHANGE
				)
			{
				_SafeResize3D( true );
				return CWnd::WindowProc(message,wParam,lParam);
			}
			if( message == WM_DESTROY )
			{
				TimerAnimationEnable( FALSE );
				return CWnd::WindowProc(message,wParam,lParam);
			}
			if( message == WM_SETCURSOR )
			{
				if( m_hCursor == NULL )
					return 0;
				SetCursor( m_hCursor );
				return (!0);
			} // if( message == WM_SETCURSOR )
			return CWnd::WindowProc(message,wParam,lParam);
		}

		void TimerAnimationEnable( bool bEnable )
		{
			ASSERT_VALID( this );
			the3DPipe.m_bTimerAnimationEnabled = bEnable;
			if( bEnable )
				SetTimer( GL_VIEWS_TIMER_ID, GL_VIEWS_TIMER_ELAPSE, NULL );
			else
				KillTimer( GL_VIEWS_TIMER_ID );
		}

		C3DView * GetView3D()
		{
			ASSERT_VALID( this );
			return m_pView3D;
		}
		
	}; // class COutputWnd

	CNavigationBar m_wndToolbar;
	COutputWnd m_wndGlPanel;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

static GLfloat g_fStepRotationAngle;
static GLfloat g_fStepWalkSize;

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRotateAboutX0();
	afx_msg void OnRotateAboutX1();
	afx_msg void OnRotateAboutY0();
	afx_msg void OnRotateAboutY1();
	afx_msg void OnRotateAboutZ0();
	afx_msg void OnRotateAboutZ1();
	afx_msg void OnTranslateX0();
	afx_msg void OnTranslateX1();
	afx_msg void OnTranslateY0();
	afx_msg void OnTranslateY1();
	afx_msg void OnTranslateZ0();
	afx_msg void OnTranslateZ1();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCamFovInc();
	afx_msg void OnUpdateCamFovInc(CCmdUI* pCmdUI);
	afx_msg void OnCamFovDec();
	afx_msg void OnUpdateCamFovDec(CCmdUI* pCmdUI);
	//}}AFX_MSG
	
	afx_msg void OnSelectCamera( UINT nCmdID );
	afx_msg void OnUpdateSelectCamera(CCmdUI* pCmdUI);
	
	afx_msg void OnUpdateEnabledBtnInBar(CCmdUI* pCmdUI);
	
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_)
