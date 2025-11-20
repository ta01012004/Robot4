
// RobotDemoo.h : main header file for the RobotDemoo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRobotDemooApp:
// See RobotDemoo.cpp for the implementation of this class
//

class CRobotDemooApp : public CWinApp
{
public:
	CRobotDemooApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRobotDemooApp theApp;
