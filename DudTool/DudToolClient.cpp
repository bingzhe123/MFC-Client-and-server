
// MFCApplication2.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "DudToolClient.h"
#include "DudToolClientDlg.h"
#include "CLoginDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2App

BEGIN_MESSAGE_MAP(DudToolClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCApplication2App construction

DudToolClientApp::DudToolClientApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
    CString swHostName;
    ::GetPrivateProfileString(L"ServerInfo",
        L"ip",
        L"localhost",
        swHostName.GetBuffer(MAX_PATH),
        MAX_PATH,
        L"./ClientConfig.ini");
    int iPort = 80;
    iPort = ::GetPrivateProfileInt(L"ServerInfo",
        L"Port",
        8080,
        L"./ClientConfig.ini");

    string sHostName = CW2A(swHostName.GetString());



#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    m_spClient = std::make_shared< httplib::SSLClient>(sHostName, iPort);
    auto spClient = std::dynamic_pointer_cast<httplib::SSLClient>(m_spClient);

    spClient->set_ca_cert_path(CA_CERT_FILE);
    spClient->enable_server_certificate_verification(true);
#else
    m_spClient = std::make_shared<Client>(sHostName, iPort);
#endif

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

DudToolClientApp::~DudToolClientApp()
{
    {

        m_spClient->stop();
    }
    std::cout << "~DudToolClientApp" << std::endl;
}


// The one and only CMFCApplication2App object

DudToolClientApp theApp;


// CMFCApplication2App initialization

BOOL DudToolClientApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    m_pszAppName = _tcsdup(L"Arigin Medical");
    CLoginDlg dlgLogin;
    INT_PTR nResponseLogin = dlgLogin.DoModal();
    if (nResponseLogin == IDCANCEL)
    {
        return false;
    }          

	DudToolClientDlg dlg;
	m_pMainWnd = &dlg;                   
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
        return true;
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

