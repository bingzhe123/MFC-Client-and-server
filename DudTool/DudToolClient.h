
// MFCApplication2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "httplib/httplib.h"
#include "CommonDef.h"
using namespace httplib;

// CMFCApplication2App:
// See MFCApplication2.cpp for the implementation of this class
//

class DudToolClientApp : public CWinApp
{
public:
	DudToolClientApp();
    ~DudToolClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
public:
    std::shared_ptr<Client> m_spClient;
    bool m_bIsManager = true;
    CString m_sCurrentUserName;
    UserAuthoriy m_userAuthority;
    string m_sUserID = "1000023";
};

extern DudToolClientApp theApp;
