#pragma once


// CResourceListChildFrame frame

#include "MDITabChildWnd.h"

class CResourceManagerFrame : public CMDITabChildWnd
{
	DECLARE_DYNCREATE(CResourceManagerFrame)
protected:
	CResourceManagerFrame();           // protected constructor used by dynamic creation
	virtual ~CResourceManagerFrame();

protected:
	DECLARE_MESSAGE_MAP()
};


