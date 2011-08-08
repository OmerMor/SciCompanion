// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "GLViews.h"
#include "ChildView.h"
#include "MainFrm.h"

#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DObject

IMPLEMENT_DYNCREATE( C3DObject, CObject );

C3DObject::C3DObject(
	LPCTSTR sName, // = GL_VIEWS_NONAME
	bool bEnabledParentDependency, //  = true
	bool bEnabledScale, //  = false
	bool bEnabledTranslation, //  = true
	bool bEnabledOrientation //  = true
	)
	: m_sName( sName )
	, m_pParent( NULL )
	, m_bEnabledParentDependency( bEnabledParentDependency )
	, m_bEnabledScale( bEnabledScale )
	, m_bEnabledTranslation( bEnabledTranslation )
	, m_bEnabledOrientation( bEnabledOrientation )
	, m_hti( NULL )
{
	ASSERT( sName != NULL );
	ASSERT( !m_sName.IsEmpty() );
	LocalLoadIdentity();
}

C3DObject::~C3DObject()
{
	ASSERT( m_pParent == NULL );
	for(	POSITION pos = m_listChilds.GetHeadPosition();
			pos != NULL;
			)
	{ // walk all childs
		C3DObject * pChild = m_listChilds.GetNext( pos );
		ASSERT_VALID( pChild );
		ASSERT( pChild->m_pParent == this );
		pChild->m_pParent = NULL;
		delete pChild;
	} // walk all childs
	m_listChilds.RemoveAll();
}

void C3DObject::SerializeState(
	CArchive & ar
	)
{
	ASSERT_VALID( this );
	if( ar.IsStoring() )
	{
		ar << m_v3LocalScale.x;
		ar << m_v3LocalScale.y;
		ar << m_v3LocalScale.z;
		
		ar << m_v3LocalTranslation.x;
		ar << m_v3LocalTranslation.y;
		ar << m_v3LocalTranslation.z;

		ar << m_quatLocalOrientation.x;
		ar << m_quatLocalOrientation.y;
		ar << m_quatLocalOrientation.z;
		ar << m_quatLocalOrientation.w;

//		ar << m_mtxLastTransformation.m11;
//		ar << m_mtxLastTransformation.m21;
//		ar << m_mtxLastTransformation.m31;
//		ar << m_mtxLastTransformation.m41;
//		ar << m_mtxLastTransformation.m12;
//		ar << m_mtxLastTransformation.m22;
//		ar << m_mtxLastTransformation.m32;
//		ar << m_mtxLastTransformation.m42;
//		ar << m_mtxLastTransformation.m13;
//		ar << m_mtxLastTransformation.m23;
//		ar << m_mtxLastTransformation.m33;
//		ar << m_mtxLastTransformation.m43;
//		ar << m_mtxLastTransformation.m14;
//		ar << m_mtxLastTransformation.m24;
//		ar << m_mtxLastTransformation.m34;
//		ar << m_mtxLastTransformation.m44;

		ar << m_sName;

		DWORD dwEnabledFlags = 0L;
		if( m_bEnabledParentDependency )
			dwEnabledFlags |= 0x00000001L;
		if( m_bEnabledScale )
			dwEnabledFlags |= 0x00000002L;
		if( m_bEnabledTranslation )
			dwEnabledFlags |= 0x00000004L;
		if( m_bEnabledOrientation )
			dwEnabledFlags |= 0x00000008L;

		ar << dwEnabledFlags;
	} // if( ar.IsStoring() )
	else
	{
		LocalLoadIdentity();

		ar >> m_v3LocalScale.x;
		ar >> m_v3LocalScale.y;
		ar >> m_v3LocalScale.z;
		
		ar >> m_v3LocalTranslation.x;
		ar >> m_v3LocalTranslation.y;
		ar >> m_v3LocalTranslation.z;

		ar >> m_quatLocalOrientation.x;
		ar >> m_quatLocalOrientation.y;
		ar >> m_quatLocalOrientation.z;
		ar >> m_quatLocalOrientation.w;

//		ar >> m_mtxLastTransformation.m11;
//		ar >> m_mtxLastTransformation.m21;
//		ar >> m_mtxLastTransformation.m31;
//		ar >> m_mtxLastTransformation.m41;
//		ar >> m_mtxLastTransformation.m12;
//		ar >> m_mtxLastTransformation.m22;
//		ar >> m_mtxLastTransformation.m32;
//		ar >> m_mtxLastTransformation.m42;
//		ar >> m_mtxLastTransformation.m13;
//		ar >> m_mtxLastTransformation.m23;
//		ar >> m_mtxLastTransformation.m33;
//		ar >> m_mtxLastTransformation.m43;
//		ar >> m_mtxLastTransformation.m14;
//		ar >> m_mtxLastTransformation.m24;
//		ar >> m_mtxLastTransformation.m34;
//		ar >> m_mtxLastTransformation.m44;

		ar >> m_sName;
		ASSERT( !m_sName.IsEmpty() );

		m_bEnabledParentDependency = false;
		m_bEnabledScale = false;
		m_bEnabledTranslation = false;
		m_bEnabledOrientation = false;
		DWORD dwEnabledFlags = 0L;
		ar >> dwEnabledFlags;
		if( (dwEnabledFlags&0x00000001L) != 0 )
			m_bEnabledParentDependency = true;
		if( (dwEnabledFlags&0x00000002L) != 0 )
			m_bEnabledScale = true;
		if( (dwEnabledFlags&0x00000004L) != 0 )
			m_bEnabledTranslation = true;
		if( (dwEnabledFlags&0x00000008L) != 0 )
			m_bEnabledOrientation = true;

	} // else from if( ar.IsStoring() )
}

void C3DObject::WalkTree(
	C3DObject::eWalkTreeQuery walkTreeQuery,
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror // = NULL
	)
{
	ASSERT_VALID( this );
	if(	!OnWalkTree(
			walkTreeQuery,
			pCam,
			pView3D,
			lpvCookie,
			pObjMirror
			)
		)
		return;
	for(	POSITION pos = m_listChilds.GetHeadPosition();
			pos != NULL;
			)
	{ // walk all childs
		C3DObject * pChild = m_listChilds.GetNext( pos );
		ASSERT_VALID( pChild );
		ASSERT( pChild->m_pParent == this );
		if(		walkTreeQuery == EWTQ_RENDER
			&&	( !IsRenderSubtreeItem(pChild) )
			)
			continue;
		pChild->WalkTree(
			walkTreeQuery,
			pCam,
			pView3D,
			lpvCookie,
			pObjMirror
			);
	} // walk all childs
}

bool C3DObject::OnWalkTree(
	C3DObject::eWalkTreeQuery walkTreeQuery,
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror
	)
{
bool bRetVal = true;
	ASSERT_VALID( this );
	switch( walkTreeQuery )
	{
	case EWTQ_THREAD_INIT:
		OnThreadInit( lpvCookie );
	break;
	case EWTQ_THREAD_DONE:
		OnThreadDone( lpvCookie );
	break;
	case EWTQ_PLAY:
		OnPlay( lpvCookie );
	break;
	case EWTQ_TRANSFORM:
		ASSERT_VALID( pCam );
		ASSERT_VALID( pView3D );
		OnTransform( pCam, pView3D, lpvCookie );
	break;
	case EWTQ_RENDER:
		ASSERT_VALID( pCam );
		ASSERT_VALID( pView3D );
		OnRender( pCam, pView3D, lpvCookie, pObjMirror );
		if( !IsRenderSubtree() )
			bRetVal = false;
	break;
	case EWTQ_ADD_TO_TREE:
		OnAddToTree( (CObjectHierarchyTreeCtrl*)lpvCookie );
	break;
	} // switch( walkTreeQuery )
	return bRetVal;
}

void C3DObject::OnThreadInit( LPVOID lpvCookie )
{
	ASSERT_VALID( this );
	lpvCookie;
}

void C3DObject::OnThreadDone( LPVOID lpvCookie )
{
	ASSERT_VALID( this );
	lpvCookie;
}

void C3DObject::OnPlay(
	LPVOID lpvCookie
	)
{
	ASSERT_VALID( this );
	lpvCookie;
}

void C3DObject::CalcTransformation(
	_mrct mtxParentTransformation
	)
{
	ASSERT_VALID( this );
	if( m_bEnabledParentDependency )
		m_mtxLastTransformation = mtxParentTransformation;
	else
		m_mtxLastTransformation.load_identity();
	if( m_bEnabledScale )
		m_mtxLastTransformation *= m_v3LocalScale.get_as_scale();
	if( m_bEnabledTranslation )
		m_mtxLastTransformation *= m_v3LocalTranslation.get_as_translation();
	if( m_bEnabledOrientation )
		m_mtxLastTransformation *= m_quatLocalOrientation;
}

void C3DObject::OnTransform(
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pCam );
	ASSERT_VALID( pView3D );
	pCam;
	pView3D;
	lpvCookie;
C3DObject * pParent = GetParent();
	if( pParent != NULL )
		CalcTransformation( pParent->m_mtxLastTransformation );
	else
	{
		_mt mtxIdentity;
		CalcTransformation( mtxIdentity );
	}
}

void C3DObject::OnRender(
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pCam );
	ASSERT_VALID( pView3D );
	pCam;
	pView3D;
	lpvCookie;
	pObjMirror;
}

bool C3DObject::IsRenderSubtree()
{
	ASSERT_VALID( this );
	return true;
}

bool C3DObject::IsRenderSubtreeItem( C3DObject * pObjChild )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pObjChild );
	ASSERT( pObjChild->GetParent() == this );
	pObjChild;
	return true;
}

void C3DObject::OnAddToTree(
	CObjectHierarchyTreeCtrl * pTreeCtrl
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pTreeCtrl );
	ASSERT( pTreeCtrl->GetSafeHwnd() != NULL );
	ASSERT( m_hti == NULL );
HTREEITEM htiParent = TVI_ROOT;
C3DObject * pObjParent = GetParent();
	if( pObjParent != NULL )
	{
		ASSERT_VALID( pObjParent );
		htiParent = pObjParent->m_hti;
		ASSERT( htiParent != NULL );
	}
CString strTreeItemText;
int nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_GENERIC_OBJ;
	OnQueryTreeDisplayParms(
		strTreeItemText,
		nTreeImageIndex
		);
	ASSERT( !strTreeItemText.IsEmpty() );
	m_hti =
		pTreeCtrl->InsertItem(
			(LPCTSTR)strTreeItemText,
			nTreeImageIndex,
			nTreeImageIndex,
			htiParent,
			TVI_LAST
			);
	ASSERT( m_hti != NULL );
	VERIFY( pTreeCtrl->SetItemData( m_hti, (DWORD)this ) );
}

void C3DObject::OnTreeItemDblClick(
	CObjectHierarchyTreeCtrl * pTreeCtrl
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pTreeCtrl );
	ASSERT( pTreeCtrl->GetSafeHwnd() != NULL );
	ASSERT( m_hti != NULL );
	pTreeCtrl;
}

void C3DObject::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	strTreeItemText = GetName();
	ASSERT( !strTreeItemText.IsEmpty() );
	if( m_pParent == NULL )
		nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_ROOT;
}

void C3DObject::GenerateSphere(
	int nTextureIndex,
	_v3rct ptCenter,
	GLfloat fRadius,
	int nStacksCount,
	GLfloat fAngleTheta0,
	GLfloat fAngleTheta1,
	GLfloat fAngleTheta1Phi0,
	GLfloat fAngleTheta1Phi1,
	bool bNoDepth // = false
	)
{
	ASSERT( fRadius >= 0 && nStacksCount > 3 );
	for( int nStackIdx0 = 0; nStackIdx0 < (nStacksCount / 2); nStackIdx0++ )
	{
		GLfloat fAngleStack0 =
			fAngleTheta1Phi0 +
				( fAngleTheta1Phi1 - fAngleTheta1Phi0 ) /
					( nStacksCount / 2 ) * nStackIdx0 ;
		GLfloat fAngleStack1 =
			fAngleTheta1Phi0 +
				( fAngleTheta1Phi1 - fAngleTheta1Phi0 ) /
					( nStacksCount / 2 ) * ( nStackIdx0 + 1 );
		_v3t
			vNormalPrev0, vNormalCurr0, vNormalPrev1, vNormalCurr1,
			vVertexPrev0, vVertexCurr0, vVertexPrev1, vVertexCurr1;
		GLfloat
			fTextureCoordPrevX0 = 0.0f, fTextureCoordPrevY0 = 0.0f,
			fTextureCoordCurrX0 = 0.0f, fTextureCoordCurrY0 = 0.0f,
			fTextureCoordPrevX1 = 0.0f, fTextureCoordPrevY1 = 0.0f,
			fTextureCoordCurrX1 = 0.0f, fTextureCoordCurrY1 = 0.0f;
		for( int nStackIdx1 = 0; nStackIdx1 <= nStacksCount; nStackIdx1++ )
		{
			if( nStackIdx1 > 0 )
			{
				vNormalPrev0 = vNormalCurr0;
				vNormalPrev1 = vNormalCurr1;
				vVertexPrev0 = vVertexCurr0;
				vVertexPrev1 = vVertexCurr1;
				fTextureCoordPrevX0 = fTextureCoordCurrX0;
				fTextureCoordPrevX1 = fTextureCoordCurrX1;
				fTextureCoordPrevY0 = fTextureCoordCurrY0;
				fTextureCoordPrevY1 = fTextureCoordCurrY1;
			} // if( nStackIdx1 > 0 )
			GLfloat fAngleStack2 =
				fAngleTheta0 +
					nStackIdx1 * (fAngleTheta1 - fAngleTheta0)
						/ nStacksCount;
			vNormalCurr0.load_vector(
				_ntr::cos(fAngleStack0) * _ntr::cos(fAngleStack2),
				_ntr::sin(fAngleStack0),
				_ntr::cos(fAngleStack0) * _ntr::sin(fAngleStack2)
				);
			vVertexCurr0.load_vector(
				ptCenter.x + fRadius * vNormalCurr0.x,
				ptCenter.y + fRadius * vNormalCurr0.y,
				ptCenter.z + fRadius * vNormalCurr0.z
				);
			fTextureCoordCurrX0 = ((GLfloat)nStackIdx1) / ((GLfloat)nStacksCount);
			fTextureCoordCurrY0 = (2.0f * nStackIdx0) / ((GLfloat)nStacksCount);
			vNormalCurr1.load_vector(
				_ntr::cos(fAngleStack1) * _ntr::cos(fAngleStack2),
				_ntr::sin(fAngleStack1),
				_ntr::cos(fAngleStack1) * _ntr::sin(fAngleStack2)
				);
			vVertexCurr1.load_vector(
				ptCenter.x + fRadius * vNormalCurr1.x,
				ptCenter.y + fRadius * vNormalCurr1.y,
				ptCenter.z + fRadius * vNormalCurr1.z
				);
			fTextureCoordCurrX1 = ((GLfloat)nStackIdx1) / ((GLfloat)nStacksCount);
			fTextureCoordCurrY1 = (2 * (nStackIdx0 + 1)) / (GLfloat)nStacksCount;
			if( nStackIdx1 > 0 )
			{
				C3DSquare * pObjSquare = new C3DSquare;
				pObjSquare->m_nTextureIndex = nTextureIndex;
				pObjSquare->m_bNoDepth = bNoDepth;
				pObjSquare->m_bUseNormal = true;
				pObjSquare->m_vecNormal = vNormalCurr1;
				pObjSquare->m_arrTextureCoords[0].m_fX = fTextureCoordPrevX0;
				pObjSquare->m_arrTextureCoords[0].m_fY = fTextureCoordPrevY0;
				pObjSquare->m_arrPoints[0] = vVertexPrev0;
				pObjSquare->m_arrTextureCoords[1].m_fX = fTextureCoordPrevX1;
				pObjSquare->m_arrTextureCoords[1].m_fY = fTextureCoordPrevY1;
				pObjSquare->m_arrPoints[1] = vVertexPrev1;
				pObjSquare->m_arrTextureCoords[2].m_fX = fTextureCoordCurrX1;
				pObjSquare->m_arrTextureCoords[2].m_fY = fTextureCoordCurrY1;
				pObjSquare->m_arrPoints[2] = vVertexCurr1;
				pObjSquare->m_arrTextureCoords[3].m_fX = fTextureCoordCurrX0;
				pObjSquare->m_arrTextureCoords[3].m_fY = fTextureCoordCurrY0;
				pObjSquare->m_arrPoints[3] = vVertexCurr0;
				AddChild( pObjSquare );
			} // if( nStackIdx1 > 0 )
		} // for( int nStackIdx1 = 0; nStackIdx1 <= nStacksCount; nStackIdx1++ )
	} // for( int nStackIdx0 = 0; nStackIdx0 < (nStacksCount / 2); nStackIdx0++ )
}

bool C3DObject::stat_LoadResourceToMemory(
	LPCTSTR pszResId,
	LPCTSTR pszRsType,
	LPVOID * pLpvOutBuffer,
	DWORD * pDwOutSize
	)
{
	*pLpvOutBuffer = NULL;
	*pDwOutSize = 0L;
HINSTANCE hInst =
		::AfxFindResourceHandle( pszResId, pszRsType );
	if( hInst == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
HRSRC hRsrc = FindResource( hInst, pszResId, pszRsType );
	if( hRsrc == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
HGLOBAL hGlobal = LoadResource( hInst, hRsrc );
	if( hGlobal == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
LPVOID lpvData = LockResource( hGlobal );
DWORD dwSize = SizeofResource( hInst, hRsrc );
	if( lpvData == NULL )
	{
		ASSERT( FALSE );
		::UnlockResource( hGlobal );
		::FreeResource( hGlobal );
		return false;
	}
	if( dwSize == 0L )
	{
		ASSERT( FALSE );
		::UnlockResource( hGlobal );
		::FreeResource( hGlobal );
		return false;
	}
	*pLpvOutBuffer = malloc( dwSize );
	if( (*pLpvOutBuffer) == NULL )
	{
		ASSERT( FALSE );
		::UnlockResource( hGlobal );
		::FreeResource( hGlobal );
		return false;
	}
	memcpy( *pLpvOutBuffer, lpvData, dwSize );
	*pDwOutSize = dwSize;
	::UnlockResource( hGlobal );
	::FreeResource( hGlobal );
	return true;
}

bool C3DObject::stat_IsFileExists(
	LPCTSTR sFilePath
	)
{
	ASSERT( sFilePath != NULL );
	if( sFilePath == NULL )
		return false;
#if (defined _UNICODE)
		struct _wfinddata_t fd;
#else
		struct _finddata_t fd;
#endif
	long hNextFile = (long)
#if (defined _UNICODE)
			_wfindfirst(
#else
			_findfirst(
#endif
				(LPTSTR)sFilePath,
				&fd
				);
bool bExists = true;
	if( hNextFile < 0 )
		bExists = false;
	else
	{
		if( (fd.attrib&_A_SUBDIR) != 0 )
			bExists = false;
	} // else from if( hNextFile < 0 )
	_findclose( hNextFile );
	return bExists;
}

/////////////////////////////////////////////////////////////////////////////
// C3DModifier

IMPLEMENT_DYNCREATE( C3DModifier, C3DObject );

C3DModifier::C3DModifier(
	LPCTSTR sName, // = GL_VIEWS_NONAME
	GLfloat fAnglePlayStepPitch, //  = 0.0f
	GLfloat fAnglePlayStepYaw, //  = 0.0f
	GLfloat fAnglePlayStepRoll //  = 0.0f
	)
	: C3DObject( sName )
	, m_fAnglePlayStepPitch( fAnglePlayStepPitch )
	, m_fAnglePlayStepYaw( fAnglePlayStepYaw )
	, m_fAnglePlayStepRoll( fAnglePlayStepRoll )
{
}

C3DModifier::~C3DModifier()
{
}

void C3DModifier::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	ASSERT( ! m_sName.IsEmpty() );
int nCount = int( m_listChilds.GetCount() );
	ASSERT( nCount > 0 );
	if( nCount > 1 )
	{
		strTreeItemText = _T("Group modifier \"");
		strTreeItemText += m_sName;
		strTreeItemText += _T("\"");
		nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_MODIFIER_GRP;
		return;
	} // if( nCount > 1 )
C3DObject * pObjHead = m_listChilds.GetHead();
	ASSERT_VALID( pObjHead );
CString strChildDisplayText;
	pObjHead->OnQueryTreeDisplayParms(
		strChildDisplayText,
		nTreeImageIndex
		);
	ASSERT( !strChildDisplayText.IsEmpty() );
	strTreeItemText = _T("Modifier for: ");
	strTreeItemText += strChildDisplayText;
	nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_MODIFIER_ONE;
}

void C3DModifier::OnPlay(
	LPVOID lpvCookie
	)
{
	ASSERT_VALID( this );
	lpvCookie;
#ifdef GL_VIEWS_DRAW_ANIMS
	if(		m_fAnglePlayStepPitch != 0.0f
		&&	m_fAnglePlayStepYaw != 0.0f
		&&	m_fAnglePlayStepRoll != 0.0f
		)
		LocalAdjustOrientation(
			m_fAnglePlayStepPitch,
			m_fAnglePlayStepYaw,
			m_fAnglePlayStepRoll
			);
#endif // GL_VIEWS_DRAW_ANIMS
}

/////////////////////////////////////////////////////////////////////////////
// C3DSquare

IMPLEMENT_DYNCREATE( C3DSquare, C3DObject );

C3DSquare::C3DSquare()
	: C3DObject( GL_VIEWS_SQUARE_NAME )
	, m_nTextureIndex( -1 )
	, m_bUseNormal( false )
	, m_bNoDepth( false )
	, m_bNoCullFace( false )
	, m_bAdjustAlphaFunc( false )
	, m_bAdjustBlendFunc( false )
	, m_gleAlphaFunc( GL_NOTEQUAL )
	, m_glcAlphaRef( 0.0f )
	, m_gleBlendFactorS( GL_SRC_ALPHA )
	, m_gleBlendFactorD( GL_ONE )
{
}

C3DSquare::~C3DSquare()
{
}

void C3DSquare::OnRender(
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pCam );
	ASSERT_VALID( pView3D );
	pCam;
	pView3D;
	lpvCookie;
	pObjMirror;
	glPushMatrix();
	glMultMatrixf( m_mtxLastTransformation.arr );
		if( m_bNoDepth )
		{
			glDisable( GL_DEPTH_TEST );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_bNoDepth )
		if( m_bNoCullFace )
		{
			glDisable( GL_CULL_FACE );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_bNoCullFace )
		if( m_bAdjustAlphaFunc )
		{
			glEnable( GL_ALPHA_TEST );
			GL_VIEWS_CHECK_OPENGL_ERROR
			glAlphaFunc( m_gleAlphaFunc, m_glcAlphaRef );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_bAdjustAlphaFunc )
		if( m_bAdjustBlendFunc )
		{
			glEnable( GL_BLEND );
			GL_VIEWS_CHECK_OPENGL_ERROR
			glBlendFunc( m_gleBlendFactorS, m_gleBlendFactorD );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_bAdjustBlendFunc )
		if( m_nTextureIndex >= 0 )
		{
			ASSERT( m_nTextureIndex < GL_VIEWS_TEXTURE_COUNT );
			glEnable( GL_TEXTURE_2D );
			GL_VIEWS_CHECK_OPENGL_ERROR
			glBindTexture(
				GL_TEXTURE_2D,
				pView3D->m_TextureIds[m_nTextureIndex]
				);
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_nTextureIndex >= 0 )
		glBegin(GL_QUADS);
			for( int nVertexIdx = 0; nVertexIdx < 4; nVertexIdx++ )
			{
				glTexCoord2f(
					m_arrTextureCoords[nVertexIdx].m_fX,
					m_arrTextureCoords[nVertexIdx].m_fY
					);
				glVertex3fv(
					m_arrPoints[nVertexIdx].arr
					);
			}
		glEnd();
		if( m_nTextureIndex >= 0 )
		{
			ASSERT( m_nTextureIndex < GL_VIEWS_TEXTURE_COUNT );
			glDisable( GL_TEXTURE_2D );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_nTextureIndex >= 0 )
		if( m_bAdjustBlendFunc )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			GL_VIEWS_CHECK_OPENGL_ERROR
			glDisable( GL_BLEND );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_bAdjustBlendFunc )
		if( m_bAdjustAlphaFunc )
		{
			glDisable( GL_ALPHA_TEST );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_bAdjustAlphaFunc )
		if( m_bNoCullFace )
		{
			glEnable( GL_CULL_FACE );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_bNoCullFace )
		if( m_bNoDepth )
		{
			glEnable( GL_DEPTH_TEST );
			GL_VIEWS_CHECK_OPENGL_ERROR
		} // if( m_bNoDepth )
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// C3DOuterScene

IMPLEMENT_DYNCREATE( C3DOuterScene, C3DObject );

volatile bool C3DOuterScene::g_bRenderOuterScene = true;
GLfloat C3DOuterScene::g_fBottomPlaneValX = 0.7f;
GLfloat C3DOuterScene::g_fBottomPlaneValY = 0.4f;
GLfloat C3DOuterScene::g_fBottomPlaneValZ = 0.7f;
GLfloat C3DOuterScene::g_fRingRadius = 0.80f;
GLint C3DOuterScene::g_nSphereValStacks = 12;

C3DOuterScene::C3DOuterScene()
	: C3DObject( GL_VIEWS_OUTER_SCENE_NAME )
{
}

C3DOuterScene::~C3DOuterScene()
{
}

void C3DOuterScene::OnThreadInit( LPVOID lpvCookie )
{
	ASSERT_VALID( this );
	lpvCookie;

C3DSquare * pObjSquare = new C3DSquare;
	pObjSquare->m_nTextureIndex = GL_VIEWS_TEXTURE_IDX_BOTTOM_PLANE;
	pObjSquare->m_bNoDepth = true;
	pObjSquare->m_arrTextureCoords[3].m_fX = 2.0f;
	pObjSquare->m_arrTextureCoords[3].m_fY = 2.0f;
	pObjSquare->m_arrPoints[3] = _v3t( -g_fBottomPlaneValX, -g_fBottomPlaneValY, -g_fBottomPlaneValZ );
	pObjSquare->m_arrTextureCoords[2].m_fX = 0.0f;
	pObjSquare->m_arrTextureCoords[2].m_fY = 2.0f;
	pObjSquare->m_arrPoints[2] = _v3t(  g_fBottomPlaneValX, -g_fBottomPlaneValY, -g_fBottomPlaneValZ );
	pObjSquare->m_arrTextureCoords[1].m_fX = 0.0f;
	pObjSquare->m_arrTextureCoords[1].m_fY = 0.0f;
	pObjSquare->m_arrPoints[1] = _v3t(  g_fBottomPlaneValX, -g_fBottomPlaneValY,  g_fBottomPlaneValZ );
	pObjSquare->m_arrTextureCoords[0].m_fX = 2.0f;
	pObjSquare->m_arrTextureCoords[0].m_fY = 0.0f;
	pObjSquare->m_arrPoints[0] = _v3t( -g_fBottomPlaneValX, -g_fBottomPlaneValY,  g_fBottomPlaneValZ );
	pObjSquare->m_bUseNormal = true;
_v3t pt0(
	pObjSquare->m_arrPoints[1].x - pObjSquare->m_arrPoints[0].x,
	pObjSquare->m_arrPoints[1].y - pObjSquare->m_arrPoints[0].y,
	pObjSquare->m_arrPoints[1].z - pObjSquare->m_arrPoints[0].z
	);
_v3t pt1(
	pObjSquare->m_arrPoints[2].x - pObjSquare->m_arrPoints[0].x,
	pObjSquare->m_arrPoints[2].y - pObjSquare->m_arrPoints[0].y,
	pObjSquare->m_arrPoints[2].z - pObjSquare->m_arrPoints[0].z
	);
_v3t vNormal = pt0 ^ pt1;
	vNormal.normalize();
	pObjSquare->m_vecNormal = vNormal;
	AddChild( pObjSquare );

	GenerateSphere(
		GL_VIEWS_TEXTURE_IDX_RING,
		_v3t( 0.0f, 0.0f, 0.0f ),
		g_fRingRadius,
		g_nSphereValStacks,
		_ntr::get_pi()*2.0f,
		0.0f,
		- 0.3f,
		+ 0.7f,
		true
		);
}

bool C3DOuterScene::IsRenderSubtree()
{
	ASSERT_VALID( this );
	return g_bRenderOuterScene;
}

void C3DOuterScene::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	C3DObject::OnQueryTreeDisplayParms(
		strTreeItemText,
		nTreeImageIndex
		);
	nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_OUTER_SCENE;
}

/////////////////////////////////////////////////////////////////////////////
// C3DCamera

IMPLEMENT_DYNCREATE( C3DCamera, C3DObject );

const GLfloat C3DCamera::g_arrFovValues[GL_VIEWS_FOV_COUNT] =
{
	(( 10.0f*3.1415926535897932384626433832795f)/180.0f ), //  0  10 degrees
	(( 15.0f*3.1415926535897932384626433832795f)/180.0f ), //  1  15 degrees
	(( 20.0f*3.1415926535897932384626433832795f)/180.0f ), //  2  20 degrees
	(( 25.0f*3.1415926535897932384626433832795f)/180.0f ), //  3  25 degrees
	(( 30.0f*3.1415926535897932384626433832795f)/180.0f ), //  4  30 degrees
	(( 35.0f*3.1415926535897932384626433832795f)/180.0f ), //  5  35 degrees
	(( 40.0f*3.1415926535897932384626433832795f)/180.0f ), //  6  40 degrees
	(( 45.0f*3.1415926535897932384626433832795f)/180.0f ), //  7  45 degrees
	(( 50.0f*3.1415926535897932384626433832795f)/180.0f ), //  8  50 degrees
	(( 55.0f*3.1415926535897932384626433832795f)/180.0f ), //  9  55 degrees
	(( 60.0f*3.1415926535897932384626433832795f)/180.0f ), // 10  60 degrees
	(( 65.0f*3.1415926535897932384626433832795f)/180.0f ), // 11  65 degrees
	(( 70.0f*3.1415926535897932384626433832795f)/180.0f ), // 12  70 degrees
	(( 75.0f*3.1415926535897932384626433832795f)/180.0f ), // 13  75 degrees
	(( 80.0f*3.1415926535897932384626433832795f)/180.0f ), // 14  80 degrees
	(( 85.0f*3.1415926535897932384626433832795f)/180.0f ), // 15  85 degrees
	(( 90.0f*3.1415926535897932384626433832795f)/180.0f ), // 16  90 degrees
	(( 95.0f*3.1415926535897932384626433832795f)/180.0f ), // 17  95 degrees
	((100.0f*3.1415926535897932384626433832795f)/180.0f ), // 18 100 degrees
	((110.0f*3.1415926535897932384626433832795f)/180.0f ), // 19 110 degrees
	((120.0f*3.1415926535897932384626433832795f)/180.0f ), // 20 120 degrees
	((130.0f*3.1415926535897932384626433832795f)/180.0f ), // 21 130 degrees
	((140.0f*3.1415926535897932384626433832795f)/180.0f ), // 22 140 degrees
	((150.0f*3.1415926535897932384626433832795f)/180.0f ), // 23 150 degrees
	((160.0f*3.1415926535897932384626433832795f)/180.0f ), // 24 160 degrees
	((170.0f*3.1415926535897932384626433832795f)/180.0f ), // 25 170 degrees
};

volatile bool C3DCamera::g_bRenderCameraAxes = true;
GLfloat C3DCamera::g_fAxisLineLength = 0.025f;
GLfloat C3DCamera::g_fAxisLineWidth = 2.0f;

C3DCamera::C3DCamera(
	UINT nTreeDblClickCmdID, // = 0L
	LPCTSTR sName, // = GL_VIEWS_NONAME
	int nFovIndex, // = GL_VIEWS_FOV_DEF_INDEX
	GLfloat fNearPlane, // = 1.0f
	GLfloat fFarPlane // = 5.0f
	)
	: C3DObject( sName )
	, m_nTreeDblClickCmdID( nTreeDblClickCmdID )
	, m_fAspect( 1.0f )
	, m_nFovIndex( nFovIndex )
	, m_fNearPlane( fNearPlane )
	, m_fFarPlane( fFarPlane )
{
	ASSERT( 0 <= m_nFovIndex && m_nFovIndex < GL_VIEWS_FOV_COUNT );
}

C3DCamera::~C3DCamera()
{
}

void C3DCamera::SerializeState(
	CArchive & ar
	)
{
	ASSERT_VALID( this );
	ASSERT( 0 <= m_nFovIndex && m_nFovIndex < GL_VIEWS_FOV_COUNT );
	C3DObject::SerializeState( ar );
	if( ar.IsStoring() )
	{
		ar << m_fAspect;
		ar << DWORD(m_nFovIndex);
		ar << m_fNearPlane;
		ar << m_fFarPlane;
	} // if( ar.IsStoring() )
	else
	{
		ar >> m_fAspect;
		DWORD dwTmp;
		ar >> dwTmp; m_nFovIndex = int(dwTmp);
		ASSERT( 0 <= m_nFovIndex && m_nFovIndex < GL_VIEWS_FOV_COUNT );
		if( !(0 <= m_nFovIndex && m_nFovIndex < GL_VIEWS_FOV_COUNT) )
			m_nFovIndex = GL_VIEWS_FOV_DEF_INDEX;
		ar >> m_fNearPlane;
		ar >> m_fFarPlane;
	} // else from if( ar.IsStoring() )
}

void C3DCamera::OnRender(
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pCam );
	ASSERT_VALID( pView3D );
	pCam;
	pView3D;
	lpvCookie;
	if( (!g_bRenderCameraAxes) )
		return;
	if( pCam == this && pObjMirror == NULL )
		return;
	glPushMatrix();
	glMultMatrixf( m_mtxLastTransformation.arr );
		glLineWidth( g_fAxisLineWidth );
		GL_VIEWS_CHECK_OPENGL_ERROR
		glBegin( GL_LINES );
			glColor3f( 1.0f, 0.0f, 0.0f );	glVertex3f( 0.0f, 0.0f, 0.0f );	glVertex3f( g_fAxisLineLength,              0.0f,              0.0f );
			glColor3f( 0.0f, 1.0f, 0.0f );	glVertex3f( 0.0f, 0.0f, 0.0f );	glVertex3f(              0.0f, g_fAxisLineLength,              0.0f );
			glColor3f( 0.0f, 0.0f, 1.0f );	glVertex3f( 0.0f, 0.0f, 0.0f );	glVertex3f(              0.0f,              0.0f, g_fAxisLineLength );
			glColor3f( 1.0f, 1.0f, 1.0f ); 
		glEnd();
	glPopMatrix();
}

void C3DCamera::OnTreeItemDblClick(
	CObjectHierarchyTreeCtrl * pTreeCtrl
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pTreeCtrl );
	ASSERT( pTreeCtrl->GetSafeHwnd() != NULL );
	ASSERT( m_hti != NULL );
	if( m_nTreeDblClickCmdID == 0L )
		return;
CFrameWnd * pFrame = pTreeCtrl->GetParentFrame();
	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		ASSERT_VALID( pFrame );
		ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) );
	}
	pFrame->SendMessage( WM_COMMAND, m_nTreeDblClickCmdID );
}

void C3DCamera::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	C3DObject::OnQueryTreeDisplayParms(
		strTreeItemText,
		nTreeImageIndex
		);
	nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_CAMERA_OBJ;
}

/////////////////////////////////////////////////////////////////////////////
// C3DCube

IMPLEMENT_DYNCREATE( C3DCube, C3DObject );

volatile bool C3DCube::g_bRenderCubeObjects = true;
GLfloat C3DCube::g_fCubeVal = 0.03f;
GLfloat C3DCube::g_fTranslateCubeVal = C3DCube::g_fCubeVal * 2.2f;

C3DCube::C3DCube(
	LPCTSTR sName, // = GL_VIEWS_NONAME
	bool bCenterCube // = false
	)
	: C3DObject( sName )
	, m_bCenterCube( bCenterCube )
{
}

C3DCube::~C3DCube()
{
}

GLfloat C3DCube::g_fCenterCubePlayPitch = _ntr::d2r(0.4f);
GLfloat C3DCube::g_fCenterCubePlayYaw = _ntr::d2r(0.3f);
GLfloat C3DCube::g_fCenterCubePlayRoll = _ntr::d2r(0.2f);

void C3DCube::OnThreadInit( LPVOID lpvCookie )
{
	ASSERT_VALID( this );
	lpvCookie;

static const struct
{
	int m_nTextureIndex;
	struct {
		GLfloat m_fTextureX, m_fTextureY,
			m_fPosX, m_fPosY, m_fPosZ;
	} m_arrVertexData[4];
} g_arrSquaresInitData[] = 
{
	{	GL_VIEWS_TEXTURE_IDX_CUBE_WHITE,
		{
			{ 0.0f, 0.0f, -g_fCubeVal, -g_fCubeVal,  g_fCubeVal },
			{ 1.0f, 0.0f,  g_fCubeVal, -g_fCubeVal,  g_fCubeVal },
			{ 1.0f, 1.0f,  g_fCubeVal,  g_fCubeVal,  g_fCubeVal },
			{ 0.0f, 1.0f, -g_fCubeVal,  g_fCubeVal,  g_fCubeVal },
		},
	},	
	{	GL_VIEWS_TEXTURE_IDX_CUBE_WHITE,
		{
			{ 1.0f, 0.0f, -g_fCubeVal, -g_fCubeVal, -g_fCubeVal },
			{ 1.0f, 1.0f, -g_fCubeVal,  g_fCubeVal, -g_fCubeVal },
			{ 0.0f, 1.0f,  g_fCubeVal,  g_fCubeVal, -g_fCubeVal },
			{ 0.0f, 0.0f,  g_fCubeVal, -g_fCubeVal, -g_fCubeVal },
		},
	},	
	{	GL_VIEWS_TEXTURE_IDX_CUBE_ORANGE,
		{
			{ 1.0f, 0.0f,  g_fCubeVal, -g_fCubeVal, -g_fCubeVal },
			{ 1.0f, 1.0f,  g_fCubeVal,  g_fCubeVal, -g_fCubeVal },
			{ 0.0f, 1.0f,  g_fCubeVal,  g_fCubeVal,  g_fCubeVal },
			{ 0.0f, 0.0f,  g_fCubeVal, -g_fCubeVal,  g_fCubeVal },
		},
	},	
	{	GL_VIEWS_TEXTURE_IDX_CUBE_ORANGE,
		{
			{ 0.0f, 0.0f, -g_fCubeVal, -g_fCubeVal, -g_fCubeVal },
			{ 1.0f, 0.0f, -g_fCubeVal, -g_fCubeVal,  g_fCubeVal },
			{ 1.0f, 1.0f, -g_fCubeVal,  g_fCubeVal,  g_fCubeVal },
			{ 0.0f, 1.0f, -g_fCubeVal,  g_fCubeVal, -g_fCubeVal },
		},
	},	
};

	for(	int nSquareIdx = 0;
			nSquareIdx < (sizeof(g_arrSquaresInitData)/sizeof(g_arrSquaresInitData[0]));
			nSquareIdx++
		)
	{
		C3DSquare * pObjSquare = new C3DSquare;
		pObjSquare->m_nTextureIndex =
			g_arrSquaresInitData[nSquareIdx].m_nTextureIndex;
		for( int nVertexIdx = 0; nVertexIdx < 4; nVertexIdx++ )
		{
			pObjSquare->m_arrTextureCoords[nVertexIdx].m_fX =
				g_arrSquaresInitData[nSquareIdx].m_arrVertexData[nVertexIdx].m_fTextureX;
			pObjSquare->m_arrTextureCoords[nVertexIdx].m_fY =
				g_arrSquaresInitData[nSquareIdx].m_arrVertexData[nVertexIdx].m_fTextureY;
			pObjSquare->m_arrPoints[nVertexIdx].x =
				g_arrSquaresInitData[nSquareIdx].m_arrVertexData[nVertexIdx].m_fPosX;
			pObjSquare->m_arrPoints[nVertexIdx].y =
				g_arrSquaresInitData[nSquareIdx].m_arrVertexData[nVertexIdx].m_fPosY;
			pObjSquare->m_arrPoints[nVertexIdx].z =
				g_arrSquaresInitData[nSquareIdx].m_arrVertexData[nVertexIdx].m_fPosZ;
		}
		pObjSquare->m_bNoCullFace = true;
		pObjSquare->m_bAdjustAlphaFunc = true;
		pObjSquare->m_bAdjustBlendFunc = true;
		AddChild( pObjSquare );
	}
}

void C3DCube::OnPlay(
	LPVOID lpvCookie
	)
{
	ASSERT_VALID( this );
	lpvCookie;
#ifdef GL_VIEWS_DRAW_ANIMS
	if( m_bCenterCube )
	{
		ASSERT_VALID( m_pParent );
		ASSERT_KINDOF( C3DModifier, m_pParent );
		m_pParent->LocalAdjustOrientation(
			g_fCenterCubePlayPitch,
			g_fCenterCubePlayYaw,
			g_fCenterCubePlayRoll
			);
	}
#endif // GL_VIEWS_DRAW_ANIMS
}

bool C3DCube::IsRenderSubtreeItem( C3DObject * pObjChild )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pObjChild );
	ASSERT( pObjChild->GetParent() == this );
	if( g_bRenderCubeObjects )
		return true;
	if( pObjChild->IsKindOf(RUNTIME_CLASS(C3DSquare)) )
		return false;
	return true;
}

void C3DCube::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	strTreeItemText =
		m_bCenterCube
			? _T("center cube")
			: _T("leaf cube")
			;
	nTreeImageIndex =
		m_bCenterCube
			? GL_VIEWS_TREE_IMG_IDX_CUBE_CENTER
			: GL_VIEWS_TREE_IMG_IDX_CUBE_LEAF
			;
}

/////////////////////////////////////////////////////////////////////////////
// C3DPlanet

IMPLEMENT_DYNCREATE( C3DPlanet, C3DObject );

volatile bool C3DPlanet::g_bRenderPlanetObjects = true;
GLfloat C3DPlanet::g_fPlanetRadiusEarth = 0.04f;
GLfloat C3DPlanet::g_fPlanetRadiusMoon = 0.01f;
GLint C3DPlanet::g_nPlanetSphereStacks = 12;
GLfloat C3DPlanet::g_fAnglePlayStepPitchEarth = _ntr::d2r( 2.5f );
GLfloat C3DPlanet::g_fAnglePlayStepPitchMoon = _ntr::d2r( 4.5f );
GLfloat C3DPlanet::g_fAnglePlayStepYawEarth = _ntr::d2r( 1.5f );
GLfloat C3DPlanet::g_fAnglePlayStepYawMoon = _ntr::d2r( 2.5f );
GLfloat C3DPlanet::g_fAnglePlayStepRollEarth = _ntr::d2r( 4.0f );
GLfloat C3DPlanet::g_fAnglePlayStepRollMoon = _ntr::d2r( 6.0f );

C3DPlanet::C3DPlanet(
	LPCTSTR sName, // = GL_VIEWS_NONAME
	int nPlanetTextureIndex, // = GL_VIEWS_TEXTURE_IDX_EARTH
	GLint nPlanetStacks, // = C3DPlanet::g_nPlanetSphereStacks
	GLfloat fPlanetRadius, // = C3DPlanet::g_fPlanetRadiusEarth
	GLfloat fAnglePlayStepPitch, // = C3DPlanet::g_fAnglePlayStepPitchEarth
	GLfloat fAnglePlayStepYaw, // = C3DPlanet::g_fAnglePlayStepYawEarth
	GLfloat fAnglePlayStepRoll // = C3DPlanet::g_fAnglePlayStepRollMoon
	)
	: C3DObject( sName )
	, m_nPlanetTextureIndex( nPlanetTextureIndex )
	, m_nPlanetStacks( nPlanetStacks )
	, m_fPlanetRadius( fPlanetRadius )
	, m_fAnglePlayStepPitch( fAnglePlayStepPitch )
	, m_fAnglePlayStepYaw( fAnglePlayStepYaw )
	, m_fAnglePlayStepRoll( fAnglePlayStepRoll )
{
}

C3DPlanet::~C3DPlanet()
{
}

void C3DPlanet::OnThreadInit( LPVOID lpvCookie )
{
	ASSERT_VALID( this );
	lpvCookie;
	GenerateSphere(
		m_nPlanetTextureIndex,
		_v3t( 0.0f, 0.0f, 0.0f ),
		m_fPlanetRadius,
		m_nPlanetStacks,
		_ntr::get_pi() * 2.0f,
		0.0f,
		- _ntr::get_pi() / 2.0f,
		+ _ntr::get_pi() / 2.0f
		);
}

bool C3DPlanet::IsRenderSubtreeItem( C3DObject * pObjChild )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pObjChild );
	ASSERT( pObjChild->GetParent() == this );
	if( g_bRenderPlanetObjects )
		return true;
	if( pObjChild->IsKindOf(RUNTIME_CLASS(C3DSquare)) )
		return false;
	return true;
}

void C3DPlanet::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	C3DObject::OnQueryTreeDisplayParms(
		strTreeItemText,
		nTreeImageIndex
		);
	//nTreeImageIndex = ....;
}

void C3DPlanet::OnPlay(
	LPVOID lpvCookie
	)
{
	ASSERT_VALID( this );
	lpvCookie;
	if( !g_bRenderPlanetObjects )
		return;
#ifdef GL_VIEWS_DRAW_ANIMS
	ASSERT_VALID( m_pParent );
	ASSERT_KINDOF( C3DModifier, m_pParent );
	m_pParent->LocalAdjustOrientation(
		m_fAnglePlayStepPitch,
		m_fAnglePlayStepYaw,
		m_fAnglePlayStepRoll
		);
#endif // GL_VIEWS_DRAW_ANIMS
}

/////////////////////////////////////////////////////////////////////////////
// C3DText

IMPLEMENT_DYNCREATE( C3DText, C3DObject );

volatile bool C3DText::g_bRenderTextObjects = true;
GLfloat C3DText::g_fScaleModifier = 0.03f;
GLfloat C3DText::g_fAnglePlayStepPitchText = 0.0f;
GLfloat C3DText::g_fAnglePlayStepYawText = 0.0f;
GLfloat C3DText::g_fAnglePlayStepRollText = _ntr::d2r( 2.0f );

C3DText::C3DText(
	LPCTSTR sName, // = GL_VIEWS_NONAME // should be used to set displayed text or one letter
	COLORREF clrText, // = RGB(255,255,255)
	GLfloat fAnglePlayStepPitch, // = C3DText::g_fAnglePlayStepPitchText
	GLfloat fAnglePlayStepYaw,   // = C3DText::g_fAnglePlayStepYawText
	GLfloat fAnglePlayStepRoll   // = C3DText::g_fAnglePlayStepRollText
	)
	: C3DObject( sName )
	, m_fRed(	GLfloat(GetRValue(clrText)) / 255.0f )
	, m_fGreen(	GLfloat(GetGValue(clrText)) / 255.0f )
	, m_fBlue(	GLfloat(GetBValue(clrText)) / 255.0f )
	, m_fAnglePlayStepPitch( fAnglePlayStepPitch )
	, m_fAnglePlayStepYaw( fAnglePlayStepYaw )
	, m_fAnglePlayStepRoll( fAnglePlayStepRoll )
{
}

C3DText::~C3DText()
{
}

void C3DText::OnPlay(
	LPVOID lpvCookie
	)
{
	ASSERT_VALID( this );
	lpvCookie;
	if( !g_bRenderTextObjects )
		return;
#ifdef GL_VIEWS_DRAW_ANIMS
	ASSERT_VALID( m_pParent );
	ASSERT_KINDOF( C3DModifier, m_pParent );
	m_pParent->LocalAdjustOrientation(
		m_fAnglePlayStepPitch,
		m_fAnglePlayStepYaw,
		m_fAnglePlayStepRoll
		);
#endif // GL_VIEWS_DRAW_ANIMS
}

void C3DText::OnRender(
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pCam );
	ASSERT_VALID( pView3D );
	pCam;
	lpvCookie;
	pObjMirror;
	if( !g_bRenderTextObjects )
		return;
	if( !pView3D->m_Font3D.IsFontCreated() )
		return;
LPCTSTR sText = GetName();
	ASSERT( sText != NULL );
	if( _tcslen(sText) == 0 )
		return;
	glEnable( GL_ALPHA_TEST );
	GL_VIEWS_CHECK_OPENGL_ERROR
	glAlphaFunc( GL_NOTEQUAL, 0.0 );
	GL_VIEWS_CHECK_OPENGL_ERROR
		glDisable( GL_CULL_FACE );
		GL_VIEWS_CHECK_OPENGL_ERROR
			glPushMatrix();
			glMultMatrixf( m_mtxLastTransformation.arr );
				glColor3f( m_fRed, m_fGreen, m_fBlue );
					glEnable( GL_BLEND );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glBlendFunc( GL_SRC_ALPHA, GL_ONE );
					GL_VIEWS_CHECK_OPENGL_ERROR
						glPushMatrix();
							glScalef(
								g_fScaleModifier,
								g_fScaleModifier,
								g_fScaleModifier
								);
							pView3D->m_Font3D.TextOut( sText );
						glPopMatrix();
					glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glDisable( GL_BLEND );
					GL_VIEWS_CHECK_OPENGL_ERROR
				glColor3f( 1.0f, 1.0f, 1.0f ); 
			glPopMatrix();
		glDisable( GL_ALPHA_TEST );
		GL_VIEWS_CHECK_OPENGL_ERROR
	glEnable( GL_CULL_FACE );
	GL_VIEWS_CHECK_OPENGL_ERROR
}

void C3DText::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	strTreeItemText.Format(
		_T("text \"%s\""),
		GetName()
		);
	ASSERT( !strTreeItemText.IsEmpty() );
	nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_TEXT_OBJ;
}

/////////////////////////////////////////////////////////////////////////////
// C3DWnd

IMPLEMENT_DYNCREATE( C3DWnd, C3DObject );

_v3t C3DWnd::g_arrDefPointsWndAviPlayer[4] =
{
	_v3t( -0.12f, -0.11f, -0.35f ),
	_v3t(  0.12f, -0.11f, -0.35f ),
	_v3t(  0.12f,  0.11f, -0.35f ),
	_v3t( -0.12f,  0.11f, -0.35f ),
};

_v3t C3DWnd::g_arrDefPointsWndMirror[4] =
{
	_v3t(  0.12f, -0.11f,  0.35f ),
	_v3t( -0.12f, -0.11f,  0.35f ),
	_v3t( -0.12f,  0.11f,  0.35f ),
	_v3t(  0.12f,  0.11f,  0.35f ),
};

C3DWnd::C3DWnd(
	LPCTSTR sName, // = GL_VIEWS_WND_AVI_PLAYER
	int nPlaneTextureIndex, // = GL_VIEWS_TEXTURE_IDX_WND_AVI_PLAYER
	_v3rct pt0, // = C3DWnd::g_arrDefPointsWndAviPlayer[0]
	_v3rct pt1, // = C3DWnd::g_arrDefPointsWndAviPlayer[1]
	_v3rct pt2, // = C3DWnd::g_arrDefPointsWndAviPlayer[2]
	_v3rct pt3  // = C3DWnd::g_arrDefPointsWndAviPlayer[3]
	)
	: C3DObject( sName )
	, m_nPlaneTextureIndex( nPlaneTextureIndex )
{
	m_arrWndPoints[0] = pt0;
	m_arrWndPoints[1] = pt1;
	m_arrWndPoints[2] = pt2;
	m_arrWndPoints[3] = pt3;
	_RecalcNormalAndContentArea();
}

C3DWnd::~C3DWnd()
{
}

void C3DWnd::_RecalcNormalAndContentArea()
{
	ASSERT_VALID( this );
_v3t pt0(
	m_arrWndPoints[1].x - m_arrWndPoints[0].x,
	m_arrWndPoints[1].y - m_arrWndPoints[0].y,
	m_arrWndPoints[1].z - m_arrWndPoints[0].z
	);
_v3t pt1(
	m_arrWndPoints[2].x - m_arrWndPoints[0].x,
	m_arrWndPoints[2].y - m_arrWndPoints[0].y,
	m_arrWndPoints[2].z - m_arrWndPoints[0].z
	);
	m_lastNormal = pt0 ^ pt1;
	m_lastNormal.normalize();

	stat_CalcWindowContentPlane(
		m_arrWndPoints,
		m_arrContentPoints
		);
}

void C3DWnd::stat_CalcWindowContentPlane(
	_v3t * arrWndPoints,
	_v3t * arrContentPoints
	)
{
	ASSERT( arrWndPoints != NULL );
	ASSERT( arrContentPoints != NULL );
	for( int i = 0; i< 4; i++ )
	{
//		arrContentPoints[i].x = arrWndPoints[i].x * 0.9700f;
//		arrContentPoints[i].y = arrWndPoints[i].y * 0.8000f;
//		arrContentPoints[i].z = arrWndPoints[i].z - arrWndPoints[i].z * 0.0001f;
		arrContentPoints[i].x = arrWndPoints[i].x * 0.9650f;
		arrContentPoints[i].y = arrWndPoints[i].y * ( (i < 2 ) ? 0.7850f : 0.7600f );
		arrContentPoints[i].z = arrWndPoints[i].z - arrWndPoints[i].z * 0.0001f;
	}
}

void C3DWnd::OnRender(
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pCam );
	ASSERT_VALID( pView3D );
	pCam;
	pView3D;
	lpvCookie;
	pObjMirror;
	glPushMatrix();
	glMultMatrixf( m_mtxLastTransformation.arr );
		glEnable( GL_ALPHA_TEST );
		GL_VIEWS_CHECK_OPENGL_ERROR
		glAlphaFunc( GL_NOTEQUAL, 0.0f );
		GL_VIEWS_CHECK_OPENGL_ERROR
			glEnable( GL_BLEND );
			GL_VIEWS_CHECK_OPENGL_ERROR
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR );
			GL_VIEWS_CHECK_OPENGL_ERROR
				glEnable( GL_TEXTURE_2D );
				GL_VIEWS_CHECK_OPENGL_ERROR
					glBindTexture(
						GL_TEXTURE_2D,
						pView3D->m_TextureIds[m_nPlaneTextureIndex]
						);
					GL_VIEWS_CHECK_OPENGL_ERROR
					glDisable( GL_DEPTH_TEST );
					GL_VIEWS_CHECK_OPENGL_ERROR
						glBegin(GL_QUADS);
							glNormal3fv( m_lastNormal.arr );
							glTexCoord2f(0.0f, 0.0f); glVertex3fv( m_arrWndPoints[0].arr );
							glTexCoord2f(1.0f, 0.0f); glVertex3fv( m_arrWndPoints[1].arr );
							glTexCoord2f(1.0f, 1.0f); glVertex3fv( m_arrWndPoints[2].arr );
							glTexCoord2f(0.0f, 1.0f); glVertex3fv( m_arrWndPoints[3].arr );
						glEnd();
					glEnable( GL_DEPTH_TEST );
					GL_VIEWS_CHECK_OPENGL_ERROR
				glDisable( GL_TEXTURE_2D );
				GL_VIEWS_CHECK_OPENGL_ERROR
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			GL_VIEWS_CHECK_OPENGL_ERROR
			glDisable( GL_BLEND );
			GL_VIEWS_CHECK_OPENGL_ERROR
		glDisable( GL_ALPHA_TEST );
		GL_VIEWS_CHECK_OPENGL_ERROR
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// C3DMirror

IMPLEMENT_DYNCREATE( C3DMirror, C3DWnd );

volatile bool C3DMirror::g_bRenderMirrors = true;

C3DMirror::C3DMirror(
	LPCTSTR sName, // = GL_VIEWS_WND_MIRROR
	int nPlaneTextureIndex, // = GL_VIEWS_TEXTURE_IDX_WND_MIRROR
	_v3rct pt0, // = C3DWnd::g_arrDefPointsWndMirror[0]
	_v3rct pt1, // = C3DWnd::g_arrDefPointsWndMirror[1]
	_v3rct pt2, // = C3DWnd::g_arrDefPointsWndMirror[2]
	_v3rct pt3  // = C3DWnd::g_arrDefPointsWndMirror[3]
	)
	: C3DWnd( sName, nPlaneTextureIndex, pt0, pt1, pt2, pt3 )
	, m_bRenderingThisMirror( false )
{
}

C3DMirror::~C3DMirror()
{
}

void C3DMirror::OnRender(
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pCam );
	ASSERT_VALID( pView3D );
	pCam;
	pView3D;
	lpvCookie;
	if( m_bRenderingThisMirror )
		return;
	C3DWnd::OnRender( pCam, pView3D, lpvCookie, pObjMirror );
	if( !g_bRenderMirrors )
		return;
	if( pObjMirror != NULL )
		return; // multiple mirrors currently not supported by this sample
	m_bRenderingThisMirror = true;
		// clear mirror (make it black)
		glDisable( GL_DEPTH_TEST );
		GL_VIEWS_CHECK_OPENGL_ERROR
			glPushMatrix();
			glMultMatrixf( m_mtxLastTransformation.arr );
				glBegin( GL_QUADS );
					glColor3f( 0.0f, 0.0f, 0.0f );
						glVertex3fv( m_arrContentPoints[0].arr );
						glVertex3fv( m_arrContentPoints[1].arr );
						glVertex3fv( m_arrContentPoints[2].arr );
						glVertex3fv( m_arrContentPoints[3].arr );
					glColor3f( 1.0f, 1.0f, 1.0f );
				glEnd();
			glPopMatrix();
		glEnable( GL_DEPTH_TEST );
		GL_VIEWS_CHECK_OPENGL_ERROR
		// create content mask in the stencil buffer
		glEnable( GL_STENCIL_TEST );
		GL_VIEWS_CHECK_OPENGL_ERROR
		glStencilFunc( GL_ALWAYS, 1, 1 );
		GL_VIEWS_CHECK_OPENGL_ERROR
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		GL_VIEWS_CHECK_OPENGL_ERROR
			glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
			GL_VIEWS_CHECK_OPENGL_ERROR
			glDisable( GL_DEPTH_TEST );
			GL_VIEWS_CHECK_OPENGL_ERROR
				glPushMatrix();
				glMultMatrixf( m_mtxLastTransformation.arr );
					glBegin( GL_QUADS );
						glVertex3fv( m_arrContentPoints[0].arr );
						glVertex3fv( m_arrContentPoints[1].arr );
						glVertex3fv( m_arrContentPoints[2].arr );
						glVertex3fv( m_arrContentPoints[3].arr );
					glEnd();
				glPopMatrix();
			glEnable( GL_DEPTH_TEST );
			GL_VIEWS_CHECK_OPENGL_ERROR
			glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
			GL_VIEWS_CHECK_OPENGL_ERROR
		// compute reflection matrix and render mirror content
		glStencilFunc( GL_EQUAL, 1, 1 );
		GL_VIEWS_CHECK_OPENGL_ERROR
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		GL_VIEWS_CHECK_OPENGL_ERROR
			glPushMatrix();
				_mt _mtxReflection;
// TODO: calc dynamically m_arrContentPoints[0].z for _ptEq (plane equation)
_v4t _ptEq( m_lastNormal.x, m_lastNormal.y, m_lastNormal.z, m_arrContentPoints[0].z );
//_v4t _ptEq( 0.0f, 0.0f, -1.0f, 0.35f );
				_mtxReflection.load_reflection( _ptEq );
				glMultMatrixf( _mtxReflection.arr );
				double _ptEqDouble[4] = { _ptEq.a, _ptEq.b, _ptEq.c, _ptEq.d };
				glClipPlane( GL_CLIP_PLANE0, _ptEqDouble );
				GL_VIEWS_CHECK_OPENGL_ERROR
				glEnable( GL_CLIP_PLANE0 );
				GL_VIEWS_CHECK_OPENGL_ERROR
					glCullFace( GL_BACK );
					GL_VIEWS_CHECK_OPENGL_ERROR
						the3DPipe.GetRoot()->WalkTree(
							C3DObject::EWTQ_RENDER,
							pCam,
							pView3D,
							lpvCookie,
							this
							);
					glCullFace( GL_FRONT );
					GL_VIEWS_CHECK_OPENGL_ERROR
				glDisable( GL_CLIP_PLANE0 );
				GL_VIEWS_CHECK_OPENGL_ERROR
			glPopMatrix();
		glDisable( GL_STENCIL_TEST );
		GL_VIEWS_CHECK_OPENGL_ERROR
	m_bRenderingThisMirror = false;
}

void C3DMirror::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	C3DObject::OnQueryTreeDisplayParms(
		strTreeItemText,
		nTreeImageIndex
		);
	nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_WND_MIRROR;
}

/////////////////////////////////////////////////////////////////////////////
// C3DAviPlayer

IMPLEMENT_DYNCREATE( C3DAviPlayer, C3DWnd );

volatile bool C3DAviPlayer::g_bRenderAviPlayers = true;

static LPCTSTR g_sAviPlayerTempFilePath = _T(".\\AVITEMP.AVI");

C3DAviPlayer::C3DAviPlayer(
	LPCTSTR sName, // = GL_VIEWS_WND_AVI_PLAYER
	int nPlaneTextureIndex, // = GL_VIEWS_TEXTURE_IDX_WND_AVI_PLAYER
	int nSurfaceTextureIndex, // = GL_VIEWS_TEXTURE_IDX_AVI_SURFACE
	_v3rct pt0, // = C3DWnd::g_arrDefPointsWndAviPlayer[0]
	_v3rct pt1, // = C3DWnd::g_arrDefPointsWndAviPlayer[1]
	_v3rct pt2, // = C3DWnd::g_arrDefPointsWndAviPlayer[2]
	_v3rct pt3, // = C3DWnd::g_arrDefPointsWndAviPlayer[3]
	int nRenderWidth, // = 256
	int nRenderHeight // = 256
	)
	: C3DWnd( sName, nPlaneTextureIndex, pt0, pt1, pt2, pt3 )
	, m_bInitComplete( false )
	, m_pAviStream( NULL )
	, m_pGetFrame( NULL )
	, m_nAviWidth( 0 )
	, m_nAviHeight( 0 )
	, m_nRenderWidth( nRenderWidth )
	, m_nRenderHeight( nRenderHeight )
	, m_nRenderSize( nRenderWidth*nRenderHeight )
	, m_hDrawDib( NULL )
	, m_nFrameNumber( 0 )
	, m_nFrameCount( 0 )
	, m_hDC( NULL )
	, m_hBitmap( NULL )
	, m_pDibRawData( NULL )
	, m_nSurfaceTextureIndex( nSurfaceTextureIndex )
{
	ATLASSERT( m_nRenderWidth > 0 );
	ATLASSERT( m_nRenderHeight > 0 );
	ATLASSERT( m_nRenderSize > 0 );
}

C3DAviPlayer::~C3DAviPlayer()
{
	ASSERT( !m_bInitComplete );
}

void C3DAviPlayer::stat_AlertDisplay( LPCTSTR sText )
{
	ASSERT( sText != NULL );
	::AfxMessageBox( sText, MB_OK|MB_ICONERROR );
}

void C3DAviPlayer::OnThreadInit( LPVOID lpvCookie )
{
	ASSERT_VALID( this );
	C3DObject::OnThreadInit( lpvCookie );
	ATLASSERT( m_nRenderWidth > 0 );
	ATLASSERT( m_nRenderHeight > 0 );
	ATLASSERT( m_nRenderSize > 0 );
	
	if( !stat_IsFileExists(g_sAviPlayerTempFilePath) )
	{
		LPVOID lpvBuffer = NULL;
		DWORD dwSize = 0;
		if(		!stat_LoadResourceToMemory(
					MAKEINTRESOURCE(IDR_AVIFILE_FOR_PLAYER),
					_T("AVI"),
					&lpvBuffer,
					&dwSize
					)
			||	lpvBuffer == NULL
			||	dwSize == 0
			)
		{
			ASSERT( FALSE );
			stat_AlertDisplay( _T("Failed to load AVI resource") );
			return;
		}
	
		bool bFileSaved = false;
		try
		{
			CFile _file(
				g_sAviPlayerTempFilePath,
				CFile::modeCreate
					|CFile::modeWrite
					|CFile::typeBinary
					|CFile::shareExclusive
				);
			_file.Seek( 0, CFile::begin );
			_file.Write( lpvBuffer, dwSize );
			_file.Close();
			bFileSaved = true;
		} // try
		catch( CException * pXept )
		{
			pXept->Delete();
			ASSERT( FALSE );
		} // catch( CException * pXept )
		catch( ... )
		{
			ASSERT( FALSE );
		} // catch( ... )

		::free( lpvBuffer );

		if( !bFileSaved )
		{
			ASSERT( FALSE );
			stat_AlertDisplay( _T("Failed to extract AVI resource") );
			return;
		}
	} // if( !statIsFileExists(sFilePath) )
	
	::AVIFileInit();
	if( ::AVIStreamOpenFromFile(
			&m_pAviStream,
			g_sAviPlayerTempFilePath,
			streamtypeVIDEO,
			0,
			OF_READ,
			NULL
			) != 0
		)
	{
		ASSERT( FALSE );
		stat_AlertDisplay( _T("AVI stream initialization failed") );
		return;
	}
	::AVIStreamInfo(
		m_pAviStream,
		&m_pAviInfo,
		sizeof(m_pAviInfo)
		);
	m_nAviWidth =
		m_pAviInfo.rcFrame.right - m_pAviInfo.rcFrame.left;
	m_nAviHeight =
		m_pAviInfo.rcFrame.bottom - m_pAviInfo.rcFrame.top;
	m_nFrameCount =
		::AVIStreamLength( m_pAviStream );
	m_hDC =
		::CreateCompatibleDC( NULL );
	if( m_hDC == NULL )
	{
		ASSERT( FALSE );
		stat_AlertDisplay( _T("Failed to alloc HDC for AVI stream") );
		return;
	}
	::memset( &m_dataBmpInfoHdr, 0, sizeof(BITMAPINFOHEADER) );
	m_dataBmpInfoHdr.biSize = sizeof(BITMAPINFOHEADER);
	m_dataBmpInfoHdr.biPlanes = 1;
	m_dataBmpInfoHdr.biBitCount = 32; //24;
	m_dataBmpInfoHdr.biWidth = m_nRenderWidth;
	m_dataBmpInfoHdr.biHeight = m_nRenderHeight;
	m_dataBmpInfoHdr.biCompression = BI_RGB;
	m_hBitmap =
		::CreateDIBSection(
			m_hDC,
			(BITMAPINFO*)( &m_dataBmpInfoHdr ),
			DIB_RGB_COLORS,
			(void**)(&m_pDibRawData),
			NULL,
			NULL
			);
	if( m_hBitmap == NULL )
	{
		ASSERT( FALSE );
		stat_AlertDisplay( _T("Failed to create DIB section") );
		return;
	}
	ATLASSERT( m_pDibRawData != NULL );
	::SelectObject( m_hDC, m_hBitmap );
	m_pGetFrame =
		::AVIStreamGetFrameOpen(
			m_pAviStream,
			NULL
			);
	if( m_pGetFrame == NULL )
	{
//		ASSERT( FALSE );
		stat_AlertDisplay( _T("AVI frame initialization failed") );
		return;
	}
	m_hDrawDib = ::DrawDibOpen();
	if( m_hDrawDib == NULL )
	{
		ASSERT( FALSE );
		stat_AlertDisplay( _T("DrawDibOpen() failed") );
		return;
	}
	m_bInitComplete = true;
}

void C3DAviPlayer::OnThreadDone( LPVOID lpvCookie )
{
	ASSERT_VALID( this );
	C3DObject::OnThreadDone( lpvCookie );
	if( m_hBitmap != NULL )
		::DeleteObject( m_hBitmap );
	if( m_hDrawDib != NULL )
		::DrawDibClose( m_hDrawDib );
	if( m_pGetFrame != NULL )
		::AVIStreamGetFrameClose( m_pGetFrame );
	if( m_pAviStream != NULL )
		::AVIStreamRelease( m_pAviStream );
	::AVIFileExit();
	try
	{
		CFile::Remove( g_sAviPlayerTempFilePath );
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	m_bInitComplete = false;
}

void C3DAviPlayer::OnRender(
	C3DCamera * pCam,
	C3DView * pView3D,
	LPVOID lpvCookie,
	C3DMirror * pObjMirror
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pCam );
	ASSERT_VALID( pView3D );
	pCam;
	pView3D;
	lpvCookie;
	C3DWnd::OnRender( pCam, pView3D, lpvCookie, pObjMirror );
	if( (!g_bRenderAviPlayers) || (!m_bInitComplete) )
		return;
	glPushMatrix();
	glMultMatrixf( m_mtxLastTransformation.arr );
		glEnable( GL_TEXTURE_2D );
		GL_VIEWS_CHECK_OPENGL_ERROR
			glBindTexture(
				GL_TEXTURE_2D,
				pView3D->m_TextureIds[m_nSurfaceTextureIndex]
				);
			GL_VIEWS_CHECK_OPENGL_ERROR
			////////// begin get AVI frame //////////
				LPBITMAPINFOHEADER lpbi;
				lpbi = (LPBITMAPINFOHEADER)
					::AVIStreamGetFrame(
						m_pGetFrame,
						m_nFrameNumber
						);
				unsigned char * pAviFrameRawData = (unsigned char *)
					lpbi
					+ lpbi->biSize
					+ lpbi->biClrUsed * sizeof(RGBQUAD);
				::DrawDibDraw(
					m_hDrawDib,
					m_hDC,
					0, 0, m_nRenderWidth, m_nRenderHeight,
					lpbi,
					pAviFrameRawData,
					0, 0, m_nAviWidth, m_nAviHeight,
					0
					);
				unsigned char * pBufferRGBA = m_pDibRawData;
				for( int i = 0; i < m_nRenderSize; i++ )
				{
					unsigned char _byte = *pBufferRGBA;
					*pBufferRGBA = *(pBufferRGBA+2);
					pBufferRGBA ++;
					pBufferRGBA ++;
					*pBufferRGBA = _byte;
					pBufferRGBA ++;
					*pBufferRGBA = 255;
					pBufferRGBA ++;
				}
				glTexSubImage2D(
					GL_TEXTURE_2D, 0,
					0, 0, m_nRenderWidth, m_nRenderHeight,
					GL_RGBA, GL_UNSIGNED_BYTE,
					m_pDibRawData
					);
				GL_VIEWS_CHECK_OPENGL_ERROR
			////////// end get AVI frame //////////
			glBegin( GL_QUADS );
				glTexCoord2f( 0.0f, 0.0f ); glVertex3fv( m_arrContentPoints[0].arr );
				glTexCoord2f( 1.0f, 0.0f ); glVertex3fv( m_arrContentPoints[1].arr );
				glTexCoord2f( 1.0f, 1.0f ); glVertex3fv( m_arrContentPoints[2].arr );
				glTexCoord2f( 0.0f, 1.0f ); glVertex3fv( m_arrContentPoints[3].arr );
			glEnd();
		glDisable( GL_TEXTURE_2D );
		GL_VIEWS_CHECK_OPENGL_ERROR
	glPopMatrix();
}

void C3DAviPlayer::OnQueryTreeDisplayParms(
	CString & strTreeItemText,
	int & nTreeImageIndex
	)
{
	ASSERT_VALID( this );
	C3DObject::OnQueryTreeDisplayParms(
		strTreeItemText,
		nTreeImageIndex
		);
	nTreeImageIndex = GL_VIEWS_TREE_IMG_IDX_WND_AVI;
}

void C3DAviPlayer::OnPlay(
	LPVOID lpvCookie
	)
{
	ASSERT_VALID( this );
	lpvCookie;
#ifdef GL_VIEWS_DRAW_ANIMS
	if( m_bInitComplete && g_bRenderAviPlayers )
	{
		m_nFrameNumber ++;
		if( m_nFrameNumber >= m_nFrameCount )
			m_nFrameNumber = 0;
	} // if( m_bInitComplete && g_bRenderAviPlayers )
#endif // GL_VIEWS_DRAW_ANIMS
}

/////////////////////////////////////////////////////////////////////////////
// C3DTexture

void C3DTexture::_Destroy()
{
	if( m_sizeTexture.cx == 0 )
	{
		ASSERT( m_sizeTexture.cy == 0 );
		ASSERT( m_pData == NULL );
		return;
	}
	ASSERT( m_sizeTexture.cy != 0 );
	ASSERT( m_pData != NULL );
	free( m_pData );
	m_sizeTexture.cx = m_sizeTexture.cy = 0;
	m_pData = NULL;
	m_bAlphaLayerExist = false;
}

C3DTexture::C3DTexture()
	: m_sizeTexture( 0, 0 )
	, m_pData( NULL )
	, m_bAlphaLayerExist( false )
{
}

C3DTexture::~C3DTexture()
{
	_Destroy();
}

bool C3DTexture::LoadResourceBitmapAs32Bit(
	LPCTSTR lpszResource,
	unsigned char nAlphaClearVal // = 255
	)
{
	Empty();
CBitmap _bmp, _dib, *pBmpOldSrc = NULL, *pBmpOldDst = NULL;
	if( !_bmp.LoadBitmap(lpszResource) )
	{
		ASSERT( FALSE );
		return false;
	}
BITMAP _bmp_info;
	_bmp.GetBitmap( &_bmp_info );
	if(		_bmp_info.bmPlanes != 1
		||	_bmp_info.bmWidth <= 0
		||	_bmp_info.bmHeight <= 0
		)
	{
		ASSERT( FALSE );
		return false;
	}
const int nBytesPerPixel = 4;
int nPixelCount = _bmp_info.bmWidth * _bmp_info.bmHeight;
int nBytesCount = nPixelCount * nBytesPerPixel;

CWindowDC dcDesktop( NULL );
CDC dcSrc, dcDst;
	if(		!dcSrc.CreateCompatibleDC(&dcDesktop)
		||	!dcDst.CreateCompatibleDC(&dcDesktop)
		)
	{
		ASSERT( FALSE );
		return false;
	}
BITMAPINFOHEADER bih;
	::memset( (LPVOID)&bih, 0, sizeof(BITMAPINFOHEADER) );
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = _bmp_info.bmWidth;
	bih.biHeight = _bmp_info.bmHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 32; /*_bmp_info.bmBitsPixel*/;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = nPixelCount;
LPVOID pColorSurface = NULL;
HBITMAP	hDIB =
		::CreateDIBSection(
			dcDesktop.GetSafeHdc(),
			(LPBITMAPINFO)&bih,
			DIB_RGB_COLORS,
			&pColorSurface,
			NULL,
			NULL
			);
	if( hDIB == NULL || pColorSurface == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	_dib.Attach( hDIB );
	pBmpOldSrc = dcSrc.SelectObject( &_bmp );
	pBmpOldDst = dcDst.SelectObject( &_dib );
	dcDst.BitBlt(
		0, 0, _bmp_info.bmWidth, _bmp_info.bmHeight,
		&dcSrc, 
		0, 0,
		SRCCOPY
		);
	dcDst.SelectObject( pBmpOldDst );
	dcSrc.SelectObject( pBmpOldSrc );
	m_pData = ::malloc( nBytesCount );
	if( m_pData == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	::memcpy( m_pData, pColorSurface, nBytesCount );
	m_sizeTexture.cx = _bmp_info.bmWidth;
	m_sizeTexture.cy = _bmp_info.bmHeight;
	ASSERT( !m_bAlphaLayerExist );
	m_bAlphaLayerExist = true;
unsigned char
		_uc0, _uc1, _uc2,
		*pData = (unsigned char *)m_pData;
	for( int i = 0; i < nPixelCount; i++ )
	{
		_uc0 = pData[ 4*i + 0 ];
		_uc1 = pData[ 4*i + 1 ];
		_uc2 = pData[ 4*i + 2 ];
		pData[ 4*i + 0 ] = _uc2;
		pData[ 4*i + 1 ] = _uc1;
		pData[ 4*i + 2 ] = _uc0;
		pData[ 4*i + 3 ] = nAlphaClearVal;
	}
	return true;
}

bool C3DTexture::AddAlphaLayer(
	unsigned char nAlphaClearVal // = 255
	)
{
	if( IsEmpty() || m_bAlphaLayerExist )
		return false;
unsigned char * pDataOld = (unsigned char *)m_pData;
	ASSERT( pDataOld != NULL );
	ASSERT( m_sizeTexture.cx > 0 && m_sizeTexture.cy > 0 );
int nPixelCount = m_sizeTexture.cx * m_sizeTexture.cy;
unsigned char * pDataNew = new unsigned char[4*nPixelCount];
	for( int i = 0; i < nPixelCount; i++ )
	{
		pDataNew[ 4*i + 0 ] = pDataOld[ 3*i + 0 ];
		pDataNew[ 4*i + 1 ] = pDataOld[ 3*i + 1 ];
		pDataNew[ 4*i + 2 ] = pDataOld[ 3*i + 2 ];
		pDataNew[ 4*i + 3 ] = nAlphaClearVal;
	}
	delete [] pDataOld;
	pDataOld = pDataNew;
	return true;
}


bool C3DTexture::SetAlphaLayer(
	unsigned char nAlphaSetVal // = 255
	)
{
	if( IsEmpty() || (!m_bAlphaLayerExist) )
		return false;
unsigned char * pData = (unsigned char *)m_pData;
	ASSERT( pData != NULL );
	ASSERT( m_sizeTexture.cx > 0 && m_sizeTexture.cy > 0 );
int nPixelCount = m_sizeTexture.cx * m_sizeTexture.cy;
	for( int i = 0; i < nPixelCount; i++ )
		pData[ 4*i + 3 ] = nAlphaSetVal;
	return true;
}

bool C3DTexture::SetAlphaLayerIf(
	unsigned char nAlphaSetVal,
	COLORREF clr
	)
{
	if( IsEmpty() || (!m_bAlphaLayerExist) )
		return false;
unsigned char * pData = (unsigned char *)m_pData;
	ASSERT( pData != NULL );
	ASSERT( m_sizeTexture.cx > 0 && m_sizeTexture.cy > 0 );
int nPixelCount = m_sizeTexture.cx * m_sizeTexture.cy;
unsigned char byteR = GetRValue(clr);
unsigned char byteG = GetGValue(clr);
unsigned char byteB = GetBValue(clr);
	for( int i = 0; i < nPixelCount; i++ )
	{
		if(		byteR == pData[ 4*i + 0 ]
			&&	byteG == pData[ 4*i + 1 ]
			&&	byteB == pData[ 4*i + 2 ]
			)
			pData[ 4*i + 3 ] = nAlphaSetVal;
	}
	return true;
}

bool C3DTexture::SetAlphaLayerNB()
{
	if( IsEmpty() || (!m_bAlphaLayerExist) )
		return false;
unsigned char * pData = (unsigned char *)m_pData;
	ASSERT( pData != NULL );
	ASSERT( m_sizeTexture.cx > 0 && m_sizeTexture.cy > 0 );
int nPixelCount = m_sizeTexture.cx * m_sizeTexture.cy;
	for( int i = 0; i < nPixelCount; i++ )
	{
		unsigned char byteR = pData[ 4*i + 0 ];
		pData[ 4*i + 3 ] = byteR;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// C3DView

IMPLEMENT_DYNCREATE( C3DView, CObject );

C3DView::C3DView(
	HWND hWndOutput // = NULL
	)
	: m_hWndOutput( hWndOutput )
	, m_sizeView( GL_VIEWS_MIN_VIEW_DX, GL_VIEWS_MIN_VIEW_DY )
	, m_sizeViewNE( GL_VIEWS_MIN_VIEW_DX, GL_VIEWS_MIN_VIEW_DY )
	, m_hOpenGlContext( NULL )
	, m_nCameraIndex( 0L )
	, m_strViewName( _T("noname") )
{
	ASSERT( m_hWndOutput != NULL );
	ASSERT( ::IsWindow(m_hWndOutput) );
	_Init();
}

C3DView::~C3DView()
{
	_Done();
}

bool C3DView::IsViewVisible() const
{
	ASSERT_VALID( this );
	ASSERT( m_hWndOutput != NULL );
	ASSERT( ::IsWindow( ((HWND)m_hWndOutput) ) );
	if( ::IsWindowVisible( ((HWND)m_hWndOutput) ) )
	{
		return (
			::SendMessage(
				m_hWndOutput,
				GL_VIEWS_WM_QUERY_VIEW_VISIBILITY,
				0L,
				0L
				) != 0
			) ? true : false;
//		HWND hWndParent = ::GetParent( m_hWndOutput );
//		ASSERT( hWndParent != NULL );
//		ASSERT( ::IsWindow( hWndParent ) );
//		if( ::IsWindowVisible( hWndParent ) )
//			return true;
	}
	return false;
}

void C3DView::Lock()
{
	ASSERT_VALID( this );
	m_csGDI.Lock();
}

void C3DView::Unlock()
{
	ASSERT_VALID( this );
	m_csGDI.Unlock();
}

void C3DView::_Init()
{
	m_dc.SetViewSize( m_sizeView, false );
	m_dc.GetInternalBitmap();
	ASSERT( m_dc.GetSafeHdc() != NULL );
	if( m_hOpenGlContext != NULL )
		return;
static PIXELFORMATDESCRIPTOR pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,									
	PFD_SUPPORT_OPENGL|PFD_DRAW_TO_BITMAP,
	PFD_TYPE_RGBA,
	OnGlGetBufferBits(__EBB_COLOR),
	0, 0, 0, 0, 0, 0,
	OnGlGetBufferBits(__EBB_ALPHA),
	0,
	OnGlGetBufferBits(__EBB_ACCUM),
	0, 0, 0, 0,
	OnGlGetBufferBits(__EBB_Z),
	OnGlGetBufferBits(__EBB_STENCIL),
	OnGlGetBufferBits(__EBB_AUXILIARY),
	PFD_MAIN_PLANE,
	0,
	0, 0, 0
};
GLuint PixelFormat = ChoosePixelFormat( m_dc.GetSafeHdc(), &pfd );
	if(	!PixelFormat )
	{
		OnGlAlertDisplay( _T("Failed to choose the pixel format") );
		ASSERT( FALSE );
		return;
	}
	if( !SetPixelFormat( m_dc.GetSafeHdc(), PixelFormat, &pfd ) )
	{
		OnGlAlertDisplay( _T("Failed to set the pixel format") );
		ASSERT( FALSE );
		return;
	}
	m_hOpenGlContext = wglCreateContext( m_dc.GetSafeHdc() );
	if( m_hOpenGlContext == NULL )
	{
		OnGlAlertDisplay( _T("Failed to create OpenGL rendering context") );
		ASSERT( FALSE );
		return;
	}

	if( !wglMakeCurrent( m_dc.GetSafeHdc(), m_hOpenGlContext ) )
	{
		OnGlAlertDisplay( _T("Failed to activate OpenGL rendering context") );
		ASSERT( FALSE );
		return;
	}
struct
{
	LPCTSTR m_lpszResource;
} _texture_init_data[GL_VIEWS_TEXTURE_COUNT] =
{
	{ MAKEINTRESOURCE(IDB_BITMAP_CUBE_ORANGE) },
	{ MAKEINTRESOURCE(IDB_BITMAP_CUBE_WHITE) },
	{ MAKEINTRESOURCE(IDB_BITMAP_RING) },
	{ MAKEINTRESOURCE(IDB_BITMAP_BOTTOM_PLANE) },
	{ MAKEINTRESOURCE(IDB_BITMAP_WND_AVI_PLAYER) },
	{ MAKEINTRESOURCE(IDB_BITMAP_WND_AVI_PLAYER) }, // for surface
	{ MAKEINTRESOURCE(IDB_BITMAP_WND_MIRROR) },
	{ MAKEINTRESOURCE(IDB_BITMAP_EARTH) },
	{ MAKEINTRESOURCE(IDB_BITMAP_MOON) },
};
	
	glEnable( GL_TEXTURE_2D );
	GL_VIEWS_CHECK_OPENGL_ERROR

	for( int nTextureIdx = 0; nTextureIdx < GL_VIEWS_TEXTURE_COUNT; nTextureIdx++ )
	{
		C3DTexture objTexture;
		if( !objTexture.LoadResourceBitmapAs32Bit(
				_texture_init_data[nTextureIdx].m_lpszResource
				)
			)
		{
			ASSERT( FALSE );
			return;
		}
		ASSERT(	objTexture.IsAlphaLayerExist() );
		objTexture.SetAlphaLayerIf( 180, RGB(245,21,3) );
		objTexture.SetAlphaLayerIf( 0, RGB(0,0,0) );
		glGenTextures( 1, &m_TextureIds[nTextureIdx] );
		GL_VIEWS_CHECK_OPENGL_ERROR
		glBindTexture( GL_TEXTURE_2D, m_TextureIds[nTextureIdx] );
		GL_VIEWS_CHECK_OPENGL_ERROR
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			objTexture.GetWidth(),
			objTexture.GetHeight(),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			objTexture.GetData()
			);
		GL_VIEWS_CHECK_OPENGL_ERROR
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		GL_VIEWS_CHECK_OPENGL_ERROR
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		GL_VIEWS_CHECK_OPENGL_ERROR
	}

	glDisable( GL_TEXTURE_2D );
	GL_VIEWS_CHECK_OPENGL_ERROR

	ASSERT( !m_Font3D.IsFontCreated() );
	VERIFY( m_Font3D.CreateFont( m_dc.GetSafeHdc() ) );
	
	if( !wglMakeCurrent( NULL, NULL ) )
	{
		OnGlAlertDisplay( _T("Failed to release contexts") );
		ASSERT( FALSE );
		return;
	}
}

void C3DView::_Done()
{
	m_Font3D.DeleteFont();
	if( m_hOpenGlContext != NULL )
	{
		if( !wglDeleteContext( m_hOpenGlContext ) )
		{
			OnGlAlertDisplay( _T("Failed to release rendering context") );
			ASSERT( FALSE );
		}
		m_hOpenGlContext = NULL;
	} // if( m_hOpenGlContext != NULL )
	m_dc.DeleteDC();
}

void C3DView::SetViewSize( CSize sizeView )
{
	ASSERT_VALID( this );
	ASSERT( m_sizeView.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeView.cy >= GL_VIEWS_MIN_VIEW_DY );
	ASSERT( m_sizeViewNE.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeViewNE.cy >= GL_VIEWS_MIN_VIEW_DY );
	Lock();
	m_sizeViewNE.cx = max( GL_VIEWS_MIN_VIEW_DX, sizeView.cx );
	m_sizeViewNE.cy = max( GL_VIEWS_MIN_VIEW_DY, sizeView.cy );
	Unlock();
}

void C3DView::AdjustViewSize(
	bool bLockView // = true
	)
{
	ASSERT_VALID( this );
	if(	m_sizeView == m_sizeViewNE )
		return;
	if( bLockView )
		Lock();
	ASSERT( m_sizeView.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeView.cy >= GL_VIEWS_MIN_VIEW_DY );
	ASSERT( m_sizeViewNE.cx >= GL_VIEWS_MIN_VIEW_DX && m_sizeViewNE.cy >= GL_VIEWS_MIN_VIEW_DY );
	_Done();
	m_sizeView = m_sizeViewNE;
	_Init();
	if( bLockView )
		Unlock();
}

void C3DView::DoStepLeft( GLfloat fStepSize )
{
	ASSERT_VALID( this );
	Lock();
C3DCamera * pCam = the3DPipe.GetCamera( m_nCameraIndex );
	ASSERT_VALID( pCam );
	pCam->DoStepLeft( fStepSize );
	Unlock();
}

void C3DView::DoStepUp( GLfloat fStepSize )
{
	ASSERT_VALID( this );
	Lock();
C3DCamera * pCam = the3DPipe.GetCamera( m_nCameraIndex );
	ASSERT_VALID( pCam );
	pCam->DoStepUp( fStepSize );
	Unlock();
}

void C3DView::DoStepForward( GLfloat fStepSize )
{
	ASSERT_VALID( this );
	Lock();
C3DCamera * pCam = the3DPipe.GetCamera( m_nCameraIndex );
	ASSERT_VALID( pCam );
	pCam->DoStepForward( fStepSize );
	Unlock();
}

void C3DView::DoLookLeft( GLfloat fAngleDegrees )
{
	ASSERT_VALID( this );
	Lock();
C3DCamera * pCam = the3DPipe.GetCamera( m_nCameraIndex );
	ASSERT_VALID( pCam );
	pCam->DoLookLeft( fAngleDegrees );
	Unlock();
}

void C3DView::DoLookUp( GLfloat fAngleDegrees )
{
	ASSERT_VALID( this );
	Lock();
C3DCamera * pCam = the3DPipe.GetCamera( m_nCameraIndex );
	ASSERT_VALID( pCam );
	pCam->DoLookUp( fAngleDegrees );
	Unlock();
}

void C3DView::DoLookOwnAxis( GLfloat fAngleDegrees )
{
	ASSERT_VALID( this );
	Lock();
C3DCamera * pCam = the3DPipe.GetCamera( m_nCameraIndex );
	ASSERT_VALID( pCam );
	pCam->DoLookOwnAxis( fAngleDegrees );
	Unlock();
}

/////////////////////////////////////////////////////////////////////////////
// C3DPipeThread

C3DPipeThread the3DPipe;

C3DPipeThread::C3DPipeThread()
	: m_bInitComplete( false )
	, m_eventShutdownStart( FALSE, FALSE, NULL, NULL )
	, m_eventShutdownComplete( FALSE, FALSE, NULL, NULL )
	, m_eventRenderViews( FALSE, FALSE, NULL, NULL )
	, m_pRoot( new C3DObject(GL_VIEWS_ROOTNAME) )
	, m_bTimerAnimationEnabled( false )
{
	m_bAutoDelete = FALSE;
}

C3DPipeThread::~C3DPipeThread()
{
	ASSERT( !m_bAutoDelete );
	_ResourcesFree();
	m_arrCams.RemoveAll();
	delete m_pRoot;
}

bool C3DPipeThread::_ResourcesInit()
{
	return true;
}

void C3DPipeThread::_ResourcesFree()
{
int nViewCount = int( m_arrViews.GetSize() );
	for( int nViewIdx = 0; nViewIdx < nViewCount; nViewIdx++ )
	{
		C3DView * pView3D = m_arrViews[nViewIdx];
		ASSERT_VALID( pView3D );
		delete pView3D;
	}
	m_arrViews.RemoveAll();

	m_bInitComplete = false;
}

C3DObject * C3DPipeThread::GetRoot()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pRoot );
	return m_pRoot;
}

const C3DObject * C3DPipeThread::GetRoot() const
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pRoot );
	return m_pRoot;
}

void C3DPipeThread::AddView( C3DView * pView3D )
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pRoot );
	ASSERT_VALID( pView3D );
	m_arrViews.Add( pView3D );
}

int C3DPipeThread::GetViewCount() const
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pRoot );
	return int( m_arrViews.GetSize() );
}

C3DView * C3DPipeThread::GetView( int nIndex )
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pRoot );
	ASSERT( 0 <= nIndex && nIndex < m_arrViews.GetSize() );
C3DView * pView3D = m_arrViews[nIndex];
	ASSERT_VALID( pView3D );
	return pView3D;
}


void C3DPipeThread::AddCamera( C3DCamera * pCam )
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pRoot );
	ASSERT_VALID( pCam );
	ASSERT( !m_bInitComplete );
	m_pRoot->AddChild( pCam );
	m_arrCams.Add( pCam );
}

int C3DPipeThread::GetCameraCount() const
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pRoot );
	return int( m_arrCams.GetSize() );
}

C3DCamera * C3DPipeThread::GetCamera( int nIndex )
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pRoot );
	ASSERT( 0 <= nIndex && nIndex < m_arrCams.GetSize() );
C3DCamera * pCam = m_arrCams[nIndex];
	ASSERT_VALID( pCam );
	return pCam;
}

void C3DPipeThread::Render()
{
	m_eventRenderViews.SetEvent();
}

void C3DPipeThread::ShutdownAndWaitFor()
{
	if( !m_bInitComplete )
		return;
	m_eventShutdownStart.SetEvent();
	for(	CExtPopupMenuWnd::PassMsgLoop(false);
			!m_eventShutdownComplete.Lock(500);
			CExtPopupMenuWnd::PassMsgLoop(false)
		);
}

BOOL C3DPipeThread::InitInstance()
{
	if( !_ResourcesInit() )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( m_arrViews.GetSize() > 0 );
	m_bInitComplete = true;

	return TRUE;
}

void C3DPipeThread::Delete()
{
	ASSERT( !m_bAutoDelete );
	_ResourcesFree();
	CWinThread::Delete();
}

int C3DPipeThread::Run()
{
	ASSERT( m_bInitComplete );

	ASSERT_VALID( m_pRoot );
	ObjectWriteAccessSet( true );
		m_pRoot->WalkTree(
			C3DObject::EWTQ_THREAD_INIT,
			NULL,
			NULL,
			NULL,
			NULL
			);
	ObjectWriteAccessSet( false );

int nViewCount = int( m_arrViews.GetSize() );
	ASSERT( nViewCount > 0 );

CSyncObject * arrToWait[2] =
{
	&m_eventShutdownStart,
	&m_eventRenderViews
};
CMultiLock _ml( arrToWait, 2, FALSE );
	for( ; true; )
	{
		DWORD dwWaitResult = _ml.Lock( INFINITE, FALSE, 0 );
		if( dwWaitResult != (WAIT_OBJECT_0 + 1) )
			break;
		bool bResetWriteAccess = false;
		for( int nViewIdx = 0; nViewIdx < nViewCount; nViewIdx++ )
		{
			C3DView * pView3D = m_arrViews[nViewIdx];
			ASSERT_VALID( pView3D );
			if( !pView3D->IsViewVisible() )
				continue;
			bool bSendMsgComplete = false;
			pView3D->Lock();
			ASSERT( !pView3D->m_strViewName.IsEmpty() );
			pView3D->AdjustViewSize( false );
			ASSERT( pView3D->Get3DDC().GetSafeHdc() != NULL );
			if( pView3D->Get3DDC().GetSafeHdc() != NULL )
			{
				if( !bResetWriteAccess )
				{
					ObjectWriteAccessSet( true );
					bResetWriteAccess = true;
				}
				ASSERT( GetCameraCount() > 0 );
				C3DCamera * pCam = GetCamera( pView3D->GetCameraIndex() );
				ASSERT_VALID( pCam );
				m_pRoot->WalkTree(
					C3DObject::EWTQ_TRANSFORM,
					pCam,
					pView3D,
					NULL,
					NULL
					);
				if( !wglMakeCurrent( pView3D->Get3DDC().GetSafeHdc(), pView3D->GetOpenGlContext() ) )
				{
					pView3D->OnGlAlertDisplay( _T("Failed to activate OpenGL rendering context") );
					ASSERT( FALSE );
					continue;
				} // if( !wglMakeCurrent( pView3D->Get3DDC().GetSafeHdc(), pView3D->GetOpenGlContext() ) )
				else
				{
#ifdef _DEBUG
					static int g_nRenderCounter = 0;
					TRACE2( "    >>> 3D-PIPE: rendering %s images (counter=%d)\n", (LPCTSTR)pView3D->m_strViewName, g_nRenderCounter );
					g_nRenderCounter++;
#endif // _DEBUG
					glShadeModel( GL_SMOOTH );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glClearDepth( 1.0f );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glEnable( GL_DEPTH_TEST );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glDepthFunc( GL_LEQUAL );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glDepthMask( GL_TRUE );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glClearStencil( 0 );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glEnable( GL_CULL_FACE );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glCullFace( GL_FRONT );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glFrontFace( GL_CW );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glClear(
						GL_COLOR_BUFFER_BIT
						|GL_DEPTH_BUFFER_BIT
						|GL_STENCIL_BUFFER_BIT
						);
					GL_VIEWS_CHECK_OPENGL_ERROR
					glColorMask( 1, 1, 1, 1 );
					GL_VIEWS_CHECK_OPENGL_ERROR
					CSize _sizeView3D = pView3D->GetViewSize();
					pCam->m_fAspect =
						((GLfloat)_sizeView3D.cx)
						/ ((GLfloat)_sizeView3D.cy)
						;
					glViewport(
						0,
						0,
						_sizeView3D.cx,
						_sizeView3D.cy
						);
					GL_VIEWS_CHECK_OPENGL_ERROR
					glMatrixMode( GL_PROJECTION );
					GL_VIEWS_CHECK_OPENGL_ERROR
					_mt mtxPerspective;
					mtxPerspective.load_perspective(
						pCam->GetFov(),
						pCam->m_fAspect,
						pCam->m_fNearPlane,
						pCam->m_fFarPlane
						);
					glLoadMatrixf( mtxPerspective.arr );
					GL_VIEWS_CHECK_OPENGL_ERROR
					glMatrixMode( GL_MODELVIEW );
					GL_VIEWS_CHECK_OPENGL_ERROR
					_mt _mtxCameraInversion( pCam->m_mtxLastTransformation );
					_mtxCameraInversion.load_inversion();
					glLoadMatrixf(_mtxCameraInversion.arr);
					GL_VIEWS_CHECK_OPENGL_ERROR
					m_pRoot->WalkTree(
						C3DObject::EWTQ_RENDER,
						pCam,
						pView3D,
						NULL,
						NULL
						);
					glFinish();
					GL_VIEWS_CHECK_OPENGL_ERROR
					if( !wglMakeCurrent( NULL, NULL ) )
					{
						pView3D->OnGlAlertDisplay( _T("Failed to release contexts") );
						ASSERT( FALSE );
					}
					bSendMsgComplete = true;
				} // else from if( !wglMakeCurrent( pView3D->Get3DDC().GetSafeHdc(), pView3D->GetOpenGlContext() ) )
			} // if( pView3D->Get3DDC().GetSafeHdc() != NULL )
			pView3D->Unlock();
			if( bSendMsgComplete )
			{
				if( bResetWriteAccess )
				{
					ObjectWriteAccessSet( false );
					bResetWriteAccess = false;
				}
				::SendMessage(
					pView3D->GetOutputHWND(),
					GL_VIEWS_WM_RENDER_FRAME_COMPLETE,
					0L,
					0L
					);
			}
		} // for( int nViewIdx = 0; nViewIdx < nViewCount; nViewIdx++ )
		if( m_bTimerAnimationEnabled )
		{
#ifdef _DEBUG
			static int g_nRecalcPosCounter = 0;
			TRACE1( "    >>> 3D-PIPE: recalculating object positions (counter=%d)\n", g_nRecalcPosCounter );
			g_nRecalcPosCounter++;
#endif // _DEBUG
			if( !bResetWriteAccess )
			{
				ObjectWriteAccessSet( true );
				bResetWriteAccess = true;
			}
			m_pRoot->WalkTree(
				C3DObject::EWTQ_PLAY,
				NULL,
				NULL,
				NULL,
				NULL
				);
		} // if( m_bTimerAnimationEnabled )
		if( bResetWriteAccess )
		{
			ObjectWriteAccessSet( false );
			bResetWriteAccess = false;
		}
	} // for( ; true; )
	
	ObjectWriteAccessSet( true );
		m_pRoot->WalkTree(
			C3DObject::EWTQ_THREAD_DONE,
			NULL,
			NULL,
			NULL,
			NULL
			);
	ObjectWriteAccessSet( false );
	
	_ResourcesFree();
	m_eventShutdownComplete.SetEvent();
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// CCameraSelectionComboBox

IMPLEMENT_DYNAMIC( CCameraSelectionComboBox, CExtComboBox )

CCameraSelectionComboBox::CCameraSelectionComboBox()
{
}

CCameraSelectionComboBox::~CCameraSelectionComboBox()
{
}

BEGIN_MESSAGE_MAP( CCameraSelectionComboBox, CExtComboBox )
	//{{AFX_MSG_MAP(CCameraSelectionComboBox)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(CBN_SELENDOK,OnReflectCbnSelEndOK)
END_MESSAGE_MAP()

void CCameraSelectionComboBox::SyncCameraWithSelectedItem(
	bool bSetFocusToView // = false
	)
{
	ASSERT_VALID( this );
CWnd * pWnd = GetParent();
	ASSERT_VALID( pWnd );
	ASSERT_KINDOF( CExtToolControlBar, pWnd );
CChildView * pChildView = STATIC_DOWNCAST( CChildView, pWnd->GetParent() );
	ASSERT_VALID( pChildView );
C3DView * pView3D = pChildView->m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
int nCurSel = GetCurSel();
	ASSERT( 0 <= nCurSel && nCurSel < GL_VIEWS_CAMERA_COUNT );
int nCamIdxOld = pView3D->GetCameraIndex();
	ASSERT( 0 <= nCamIdxOld && nCamIdxOld < GL_VIEWS_CAMERA_COUNT );
	if( nCamIdxOld != nCurSel )
	{
		pView3D->SetCameraIndex( nCurSel );
	} // if( nCamIdxOld != nCurSel )
	if( bSetFocusToView )
		pChildView->SetFocus();
	if( nCamIdxOld != nCurSel )
	{
		CFrameWnd * pFrame = GetParentFrame();
		ASSERT_VALID( pFrame );
		CMainFrame * pMainFrame =
			DYNAMIC_DOWNCAST( CMainFrame, pFrame );
		if( pMainFrame == NULL )
		{
			pMainFrame =
				DYNAMIC_DOWNCAST( CMainFrame, pFrame->GetParentFrame() );
			ASSERT_VALID( pMainFrame );
		}
		pMainFrame->SyncCameraFovValue();
	} // if( nCamIdxOld != nCurSel )
	the3DPipe.Render();
}

int CCameraSelectionComboBox::SetCurSel(
	int nSelect,
	bool bSetFocusToView // = false
	)
{
	ASSERT( 0 <= nSelect && nSelect < GL_VIEWS_CAMERA_COUNT );
int nRetVal = CExtComboBox::SetCurSel( nSelect );
	SyncCameraWithSelectedItem( bSetFocusToView );
	return nRetVal;
}

void CCameraSelectionComboBox::OnReflectCbnSelEndOK()
{
	ASSERT_VALID( this );
bool bSetFocusToView = false;
HWND hWndFocus = ::GetFocus();
	if( hWndFocus == m_hWnd )
		bSetFocusToView = true;
	SyncCameraWithSelectedItem( bSetFocusToView );
}


/////////////////////////////////////////////////////////////////////////////
// CCameraFovComboBox

IMPLEMENT_DYNAMIC( CCameraFovComboBox, CExtComboBox )

CCameraFovComboBox::CCameraFovComboBox()
{
}

CCameraFovComboBox::~CCameraFovComboBox()
{
}

BEGIN_MESSAGE_MAP( CCameraFovComboBox, CExtComboBox )
	//{{AFX_MSG_MAP(CCameraFovComboBox)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(CBN_SELENDOK,OnReflectCbnSelEndOK)
END_MESSAGE_MAP()

void CCameraFovComboBox::SyncCameraWithSelectedItem(
	bool bSetFocusToView // = false
	)
{
	ASSERT_VALID( this );
CWnd * pWnd = GetParent();
	ASSERT_VALID( pWnd );
	ASSERT_KINDOF( CExtToolControlBar, pWnd );
CChildView * pChildView = STATIC_DOWNCAST( CChildView, pWnd->GetParent() );
	ASSERT_VALID( pChildView );
C3DView * pView3D = pChildView->m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
int nCamIdx = pView3D->GetCameraIndex();
	ASSERT( 0 <= nCamIdx && nCamIdx < GL_VIEWS_CAMERA_COUNT );
C3DCamera * pCam = the3DPipe.GetCamera( nCamIdx );
	ASSERT_VALID( pCam );

int nFovIdx = GetCurSel();
	ASSERT( 0 <= nFovIdx && nFovIdx < GL_VIEWS_FOV_COUNT );

bool bRender = false;
	the3DPipe.ObjectWriteAccessSet( true );
		if( pCam->m_nFovIndex != nFovIdx ) 
		{
			pCam->m_nFovIndex = nFovIdx;
			bRender = true;
		}
	the3DPipe.ObjectWriteAccessSet( false );

	if( bSetFocusToView )
		pChildView->SetFocus();
	if( bRender )
	{
		CFrameWnd * pFrame = GetParentFrame();
		ASSERT_VALID( pFrame );
		CMainFrame * pMainFrame =
			DYNAMIC_DOWNCAST( CMainFrame, pFrame );
		if( pMainFrame == NULL )
		{
			pMainFrame =
				DYNAMIC_DOWNCAST( CMainFrame, pFrame->GetParentFrame() );
			ASSERT_VALID( pMainFrame );
		}
		pMainFrame->SyncCameraFovValue( nCamIdx, nFovIdx );
		the3DPipe.Render();
	}
}

int CCameraFovComboBox::SetCurSel(
	int nSelect,
	bool bSetFocusToView // = false
	)
{
	ASSERT( 0 <= nSelect && nSelect < GL_VIEWS_FOV_COUNT );
int nRetVal = CExtComboBox::SetCurSel( nSelect );
	SyncCameraWithSelectedItem( bSetFocusToView );
	return nRetVal;
}

void CCameraFovComboBox::OnReflectCbnSelEndOK()
{
	ASSERT_VALID( this );
bool bSetFocusToView = false;
HWND hWndFocus = ::GetFocus();
	if( hWndFocus == m_hWnd )
		bSetFocusToView = true;
	SyncCameraWithSelectedItem( bSetFocusToView );
}


/////////////////////////////////////////////////////////////////////////////
// CObjectHierarchyTreeCtrl

IMPLEMENT_DYNAMIC( CObjectHierarchyTreeCtrl, CTreeCtrl )

CObjectHierarchyTreeCtrl::CObjectHierarchyTreeCtrl()
	: CExtNCSB < CTreeCtrl > ( true, true )
{
}

CObjectHierarchyTreeCtrl::~CObjectHierarchyTreeCtrl()
{
}

BEGIN_MESSAGE_MAP( CObjectHierarchyTreeCtrl, CTreeCtrl )
	//{{AFX_MSG_MAP(CObjectHierarchyTreeCtrl)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectHierarchyTreeCtrl message handlers

void CObjectHierarchyTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CExtNCSB < CTreeCtrl > :: OnLButtonDblClk( nFlags, point );
UINT htFlags = 0L;
HTREEITEM _hti = CExtNCSB < CTreeCtrl > :: HitTest( point, &htFlags );
	if(		_hti != NULL
		&&	(htFlags&(TVHT_ONITEM)) != 0
		)
	{
		C3DObject * pObj = (C3DObject *)
			CExtNCSB < CTreeCtrl > :: GetItemData( _hti );
		ASSERT( pObj != NULL );
		ASSERT_VALID( pObj );
		ASSERT_KINDOF( C3DObject, pObj );
		pObj->OnTreeItemDblClick( this );
	}

}

/////////////////////////////////////////////////////////////////////////////
// CChildView

IMPLEMENT_DYNAMIC( CChildView, CWnd )

GLfloat CChildView::g_fStepRotationAngle = 2.0f;
GLfloat CChildView::g_fStepWalkSize = 0.02f;

CChildView::CChildView(
	UINT nIdResourceCursor
	)
	: m_wndGlPanel( nIdResourceCursor )
{
	ASSERT( (ID_SELCAM9) == (ID_SELCAM8 + 1) );
	ASSERT( (ID_SELCAM8) == (ID_SELCAM7 + 1) );
	ASSERT( (ID_SELCAM7) == (ID_SELCAM6 + 1) );
	ASSERT( (ID_SELCAM6) == (ID_SELCAM5 + 1) );
	ASSERT( (ID_SELCAM5) == (ID_SELCAM4 + 1) );
	ASSERT( (ID_SELCAM4) == (ID_SELCAM3 + 1) );
	ASSERT( (ID_SELCAM3) == (ID_SELCAM2 + 1) );
	ASSERT( (ID_SELCAM2) == (ID_SELCAM1 + 1) );
	ASSERT( (ID_SELCAM1) == (ID_SELCAM0 + 1) );
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP( CChildView, CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_ROTATE_ABOUT_X0, OnRotateAboutX0)
	ON_COMMAND(ID_ROTATE_ABOUT_X1, OnRotateAboutX1)
	ON_COMMAND(ID_ROTATE_ABOUT_Y0, OnRotateAboutY0)
	ON_COMMAND(ID_ROTATE_ABOUT_Y1, OnRotateAboutY1)
	ON_COMMAND(ID_ROTATE_ABOUT_Z0, OnRotateAboutZ0)
	ON_COMMAND(ID_ROTATE_ABOUT_Z1, OnRotateAboutZ1)
	ON_COMMAND(ID_TRANSLATE_X0, OnTranslateX0)
	ON_COMMAND(ID_TRANSLATE_X1, OnTranslateX1)
	ON_COMMAND(ID_TRANSLATE_Y0, OnTranslateY0)
	ON_COMMAND(ID_TRANSLATE_Y1, OnTranslateY1)
	ON_COMMAND(ID_TRANSLATE_Z0, OnTranslateZ0)
	ON_COMMAND(ID_TRANSLATE_Z1, OnTranslateZ1)
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_BTN_MENU_ROTATION, OnUpdateEnabledBtnInBar)
	ON_COMMAND(ID_CAM_FOV_INC, OnCamFovInc)
	ON_UPDATE_COMMAND_UI(ID_CAM_FOV_INC, OnUpdateCamFovInc)
	ON_COMMAND(ID_CAM_FOV_DEC, OnCamFovDec)
	ON_UPDATE_COMMAND_UI(ID_CAM_FOV_DEC, OnUpdateCamFovDec)
	//}}AFX_MSG_MAP
	
	ON_COMMAND_RANGE( ID_SELCAM0, ID_SELCAM9, OnSelectCamera )
	ON_UPDATE_COMMAND_UI_RANGE( ID_SELCAM0, ID_SELCAM9, OnUpdateSelectCamera )
	
	ON_UPDATE_COMMAND_UI( ID_CAMERA_SELECTION_COMBO, OnUpdateEnabledBtnInBar )
	ON_UPDATE_COMMAND_UI( ID_CAMERA_FOV_COMBO, OnUpdateEnabledBtnInBar )
	ON_UPDATE_COMMAND_UI( ID_BTN_MENU_ROTATION, OnUpdateEnabledBtnInBar )
	ON_UPDATE_COMMAND_UI( ID_BTN_MENU_TRANSLATION, OnUpdateEnabledBtnInBar )
	
	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPrepareMenu,
		OnExtMenuPrepare
		)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

LRESULT CChildView::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
{
	lParam;
CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
		reinterpret_cast
			< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
			( wParam );
	ASSERT( pData != NULL );
CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );

	// remove camera selection combo command
INT nPos = pPopup->ItemFindPosForCmdID(ID_CAMERA_SELECTION_COMBO);
	if( nPos >= 0 )
		pPopup->ItemRemove( nPos );
	if(		pPopup->ItemGetCount() > 0
		&&	pPopup->ItemGetInfo(0).IsSeparator()
		)
		pPopup->ItemRemove( 0 );

	// replace camera FOV combo command
	nPos = pPopup->ItemFindPosForCmdID(ID_CAMERA_FOV_COMBO);
	if( nPos >= 0 )
	{
		pPopup->ItemRemove( nPos );
		if( pPopup->ItemInsert(
				(UINT)CExtPopupMenuWnd::TYPE_POPUP,
				nPos,
				_T("Camera FOV"),
				NULL,
				m_hWnd
				)
			)
		{
			CExtPopupMenuWnd * pSubMenu =
				pPopup->ItemGetPopup( nPos );
			ASSERT_VALID( pSubMenu );
			VERIFY(
				pSubMenu->ItemInsert(
					ID_CAM_FOV_INC,
					-1,
					NULL,
					NULL,
					m_hWnd
					)
				);
			VERIFY(
				pSubMenu->ItemInsert(
					ID_CAM_FOV_DEC,
					-1,
					NULL,
					NULL,
					m_hWnd
					)
				);
			UpdateDialogControls( this, TRUE );
		}
#ifdef _DEBUG
		else
		{
			ASSERT( FALSE );
		}
#endif // _DEBUG
	}
	
	// remove leading separator if exist
	if(		pPopup->ItemGetCount() > 0
		&&	pPopup->ItemGetInfo(0).IsSeparator()
		)
		pPopup->ItemRemove( 0 );

	return TRUE;
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ! (CExtWRB < CWnd > ::PreCreateWindow(cs) ) )
		return FALSE;

	cs.dwExStyle &= ~(WS_EX_STATICEDGE|WS_EX_CLIENTEDGE);
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPSIBLINGS;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
CPaintDC dcPaint( this );
	CExtPaintManager::stat_ExcludeChildAreas(
		dcPaint.GetSafeHdc(),
		GetSafeHwnd()
		);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CExtWRB < CWnd >::OnCreate(lpCreateStruct) == -1 )
	{
		ASSERT( FALSE );
		return -1;
	}

	m_wndToolbar.m_bPresubclassDialogMode = true;
	if( !m_wndToolbar.Create(
			_T(""),
			this,
			AFX_IDW_DIALOGBAR,
			WS_CHILD|WS_VISIBLE
				|CBRS_ALIGN_TOP|CBRS_TOOLTIPS
				|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
			)
		)
	{
		ASSERT( FALSE );
		return -1;
	}
	if( !m_wndToolbar.LoadToolBar(IDR_TOOLBAR_INVIEW) )
	{
		ASSERT( FALSE );
		return -1;
	}
	if( !m_wndToolbar.InitNavigationBar() )
		return -1;
	
	if(	!m_wndGlPanel.Create(
			this,
			GL_VIEWS_ID_VIEW_DLG_CTRL_ID
			)
		)
	{
		ASSERT( FALSE );
		return -1;
	}
	m_wndGlPanel.SetFont( CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)) );

	CWnd::RepositionBars( 0, 0xFFFF, GL_VIEWS_ID_VIEW_DLG_CTRL_ID );
	return 0;
}


void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CExtWRB < CWnd > ::OnSize(nType, cx, cy);
	if( nType != SIZE_MINIMIZED )
		CWnd::RepositionBars( 0, 0xFFFF, GL_VIEWS_ID_VIEW_DLG_CTRL_ID );
}

void CChildView::OnUpdateEnabledBtnInBar(CCmdUI* pCmdUI) 
{
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
}

void CChildView::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd ::OnSetFocus(pOldWnd);
	m_wndGlPanel.SetFocus();
}

void CChildView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	pWnd;
	point;
}

void CChildView::OnRotateAboutX0() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoLookUp( g_fStepRotationAngle );
	the3DPipe.Render();
}
void CChildView::OnRotateAboutX1() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoLookUp( -g_fStepRotationAngle );
	the3DPipe.Render();
}

void CChildView::OnRotateAboutY0() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoLookLeft( g_fStepRotationAngle );
	the3DPipe.Render();
}
void CChildView::OnRotateAboutY1() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoLookLeft( -g_fStepRotationAngle );
	the3DPipe.Render();
}

void CChildView::OnRotateAboutZ0() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoLookOwnAxis( g_fStepRotationAngle );
	the3DPipe.Render();
}
void CChildView::OnRotateAboutZ1() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoLookOwnAxis( -g_fStepRotationAngle );
	the3DPipe.Render();
}

void CChildView::OnTranslateX0() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoStepLeft( g_fStepWalkSize );
	the3DPipe.Render();
}
void CChildView::OnTranslateX1() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoStepLeft( -g_fStepWalkSize );
	the3DPipe.Render();
}

void CChildView::OnTranslateY0() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoStepUp( g_fStepWalkSize );
	the3DPipe.Render();
}
void CChildView::OnTranslateY1() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoStepUp( -g_fStepWalkSize );
	the3DPipe.Render();
}

void CChildView::OnTranslateZ0() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoStepForward( g_fStepWalkSize );
	the3DPipe.Render();
}
void CChildView::OnTranslateZ1() 
{
	ASSERT_VALID( this );
C3DView * pView3D = m_wndGlPanel.GetView3D();
	if( pView3D == NULL )
		return;
	ASSERT_VALID( pView3D );
	pView3D->DoStepForward( -g_fStepWalkSize );
	the3DPipe.Render();
}

void CChildView::OnSelectCamera( UINT nCmdID )
{
	ASSERT_VALID( this );
	ASSERT( ID_SELCAM0 <= nCmdID && nCmdID <= ID_SELCAM9 );
int nCameraIndex = nCmdID - ID_SELCAM0;
	ASSERT( 0 <= nCameraIndex && nCameraIndex < GL_VIEWS_CAMERA_COUNT );
	m_wndToolbar.m_wndCameraSelCombo.SetCurSel( nCameraIndex );
}
void CChildView::OnUpdateSelectCamera(CCmdUI* pCmdUI)
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	ASSERT( ID_SELCAM0 <= pCmdUI->m_nID && pCmdUI->m_nID <= ID_SELCAM9 );
int nCameraIndexReal = m_wndToolbar.m_wndCameraSelCombo.GetCurSel();
	if( nCameraIndexReal < 0 )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	ASSERT( 0 <= nCameraIndexReal && nCameraIndexReal < GL_VIEWS_CAMERA_COUNT );
int nCameraIndexToCmp = pCmdUI->m_nID - ID_SELCAM0;
	ASSERT( 0 <= nCameraIndexToCmp && nCameraIndexToCmp < GL_VIEWS_CAMERA_COUNT );
	pCmdUI->Enable( TRUE );
	pCmdUI->SetRadio(
		(nCameraIndexReal == nCameraIndexToCmp)
			? TRUE
			: FALSE
		);
}


void CChildView::OnCamFovInc() 
{
	ASSERT_VALID( this );
int nCameraIndexReal = m_wndToolbar.m_wndCameraSelCombo.GetCurSel();
	ASSERT(
			( 0 <= nCameraIndexReal && nCameraIndexReal < GL_VIEWS_CAMERA_COUNT )
		||	nCameraIndexReal < 0
		);
	if( nCameraIndexReal < 0 )
		return;
C3DCamera * pCam = the3DPipe.GetCamera( nCameraIndexReal );
	ASSERT_VALID( pCam );
int nFovIdx = pCam->m_nFovIndex;
	ASSERT( 0 <= nFovIdx && nFovIdx < GL_VIEWS_FOV_COUNT );
	if( nFovIdx == (GL_VIEWS_FOV_COUNT-1) )
		return;
	m_wndToolbar.m_wndCameraFovCombo.SetCurSel( nFovIdx+1, false );
}
void CChildView::OnUpdateCamFovInc(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
int nCameraIndexReal = m_wndToolbar.m_wndCameraSelCombo.GetCurSel();
	ASSERT(
			( 0 <= nCameraIndexReal && nCameraIndexReal < GL_VIEWS_CAMERA_COUNT )
		||	nCameraIndexReal < 0
		);
	if( nCameraIndexReal < 0 )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
C3DCamera * pCam = the3DPipe.GetCamera( nCameraIndexReal );
	ASSERT_VALID( pCam );
int nFovIdx = pCam->m_nFovIndex;
	ASSERT( 0 <= nFovIdx && nFovIdx < GL_VIEWS_FOV_COUNT );
	pCmdUI->Enable(
		(nFovIdx < (GL_VIEWS_FOV_COUNT-1)) ? TRUE : FALSE
		);
}

void CChildView::OnCamFovDec() 
{
	ASSERT_VALID( this );
int nCameraIndexReal = m_wndToolbar.m_wndCameraSelCombo.GetCurSel();
	ASSERT(
			( 0 <= nCameraIndexReal && nCameraIndexReal < GL_VIEWS_CAMERA_COUNT )
		||	nCameraIndexReal < 0
		);
	if( nCameraIndexReal < 0 )
		return;
C3DCamera * pCam = the3DPipe.GetCamera( nCameraIndexReal );
	ASSERT_VALID( pCam );
int nFovIdx = pCam->m_nFovIndex;
	ASSERT( 0 <= nFovIdx && nFovIdx < GL_VIEWS_FOV_COUNT );
	if( nFovIdx == 0 )
		return;
	m_wndToolbar.m_wndCameraFovCombo.SetCurSel( nFovIdx-1, false );
}
void CChildView::OnUpdateCamFovDec(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
int nCameraIndexReal = m_wndToolbar.m_wndCameraSelCombo.GetCurSel();
	ASSERT(
			( 0 <= nCameraIndexReal && nCameraIndexReal < GL_VIEWS_CAMERA_COUNT )
		||	nCameraIndexReal < 0
		);
	if( nCameraIndexReal < 0 )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
C3DCamera * pCam = the3DPipe.GetCamera( nCameraIndexReal );
	ASSERT_VALID( pCam );
int nFovIdx = pCam->m_nFovIndex;
	ASSERT( 0 <= nFovIdx && nFovIdx < GL_VIEWS_FOV_COUNT );
	pCmdUI->Enable(
		(nFovIdx > 0) ? TRUE : FALSE
		);
}
