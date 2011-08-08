// AviPlayer.h: interface for the CAviPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIPLAYER_H__B07E8928_6A64_4106_97EA_21B8C4089F45__INCLUDED_)
#define AFX_AVIPLAYER_H__B07E8928_6A64_4106_97EA_21B8C4089F45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Vfw.h>

#include "AviPlayerWnd.h"

class CAviPlayer : public CObject
{
	DECLARE_DYNCREATE( CAviPlayer );
public:
	CAviPlayer(){
		ASSERT(FALSE);
	}
	CAviPlayer(CWnd *pWndParent);
	virtual ~CAviPlayer();
	
protected:
	PAVISTREAM m_pAviStream;
	AVISTREAMINFO m_AviInfo;
	PGETFRAME m_pGetFrame;
	int m_nAviWidth,	m_nAviHeight,
		m_nRenderWidth, m_nRenderHeight;
	HDRAWDIB m_hDrawDib;
	BOOL m_bIsPlaying;
	BOOL m_bTempPause;

	CAviPlayerWnd* m_pAviPlayerWnd;

	void Clean();
	BOOL Prepare();

public:
	DWORD m_nFrameNumber;
	DWORD m_nFrameCount;
	DWORD m_nFrameSkiped;
	BOOL m_nStartFrameNumber;
	
	void SetPlayerWindowPos( int x, int y, int nWidth, int nHeight )
	{
		if( m_pAviPlayerWnd->GetSafeHwnd() != NULL )
			m_pAviPlayerWnd->MoveWindow( x, y, nWidth, nHeight );
	}

	CSize GetAviRenderSize()
	{
		return CSize( m_nRenderWidth, m_nRenderHeight );
	}

	DWORD GetCurrentFrameNumber()
	{
		return m_nFrameNumber;
	}

	DWORD GetFrameCount()
	{
		return m_nFrameCount;
	}
	
	DWORD GetFrameSkipped()
	{
		return m_nFrameSkiped;
	}

	void SetCurrentFrameNumber( DWORD nFrameNumber )
	{
		m_nFrameNumber = nFrameNumber;
	}
	
	void NextFrame()
	{
		if( m_nFrameCount == m_nFrameNumber+1)
		{
			m_nStartFrameNumber = m_nFrameNumber = AVIStreamStart( m_pAviStream );
		}
		else
		{		
			m_nFrameNumber++;
			m_nStartFrameNumber++;
		}
	}
	
	void Play()
	{
		ASSERT(m_pAviPlayerWnd && m_hDrawDib);
		if(m_pAviPlayerWnd && m_hDrawDib){
			m_pAviPlayerWnd->KillTimer(1);
			m_pAviPlayerWnd->SetTimer(1, GetTimerSpeed());
			m_bIsPlaying = TRUE;
			m_nStartFrameNumber = 0;
		}
	}
	
	void Stop()
	{
		ASSERT(m_pAviPlayerWnd && m_hDrawDib);
		if(m_pAviPlayerWnd && m_hDrawDib){
			m_nFrameSkiped = 0;
			m_pAviPlayerWnd->KillTimer(1);
			m_nFrameNumber = AVIStreamStart( m_pAviStream );
			m_bIsPlaying = FALSE;
			m_pAviPlayerWnd->Invalidate();
		}
	}

	void Pause()
	{
		ASSERT(m_pAviPlayerWnd && m_hDrawDib);
		if(m_pAviPlayerWnd){
			m_pAviPlayerWnd->KillTimer(1);
			m_bIsPlaying = FALSE;
		}
	}
	
	BOOL IsOpen()
	{ 
		return (m_hDrawDib!=NULL);
	}

	BOOL IsPlaying()
	{ 
		return m_bIsPlaying;
	}

	int GetTimerSpeed()
	{
		/* dwRate / dwScale == samples/second */
		return MulDiv(1000,m_AviInfo.dwScale, m_AviInfo.dwRate);
	}

	void SetZoom( int nZoomPercent )
	{
		m_nRenderWidth = MulDiv(m_nAviWidth, nZoomPercent, 100);
		m_nRenderHeight = MulDiv(m_nAviHeight, nZoomPercent, 100);
		SetPlayerWindowPos(0, 0, m_nRenderWidth, m_nRenderHeight);
	}
	
	void SetTempPause(BOOL bSet)
	{
		m_bTempPause = bSet;
	}
	
	BOOL GetTempPause()
	{
		return m_bTempPause;
	}
	
	BOOL OpenFromFile( LPCTSTR sAviFilePath );
	
	BOOL DrawFrame(
		HDC hDC,
		int nFrameNumber
		);
};

#endif // !defined(AFX_AVIPLAYER_H__B07E8928_6A64_4106_97EA_21B8C4089F45__INCLUDED_)
