// DMP609VC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DMP609VC.h"
#include "DMP609VCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMP609VCApp

BEGIN_MESSAGE_MAP(CDMP609VCApp, CWinApp)
	//{{AFX_MSG_MAP(CDMP609VCApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMP609VCApp construction

CDMP609VCApp::CDMP609VCApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDMP609VCApp object

CDMP609VCApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDMP609VCApp initialization
#include "DlgForcePowerMonitor.h"
#include "EFS_DisplayCfg.h"
DlgForcePowerMonitor *pDlgForcePowerMonitor;
DlgForcePowerMonitor DlgForcePowerMonitor;
HANDLE	m_hDevice = INVALID_HANDLE_VALUE;//定义设备句柄
extern CDMP609VCDlg *pDlgTestDMP609;

#include "LimitSingleInstance.H"
CLimitSingleInstance gSingleInstanceObj(TEXT("Global\\{MACTRONIX FPMD}"));

BOOL CDMP609VCApp::InitInstance()
{
    if (gSingleInstanceObj.IsAnotherInstanceRunning())
    {
       return FALSE;
    }

	AfxOleInit();
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#ifdef __START_FPDM__
//	UpdateData(FALSE);	

	m_pMainWnd = &DlgForcePowerMonitor;


	DlgForcePowerMonitor.DoModal();
	//OnBnClickedButtonStartForcePowerMonitor();
	//pDlgForcePowerMonitor->ShowWindow(1);

#else
	CDMP609VCDlg dlg;
	m_pMainWnd = &dlg;
	pDlgTestDMP609 = &dlg;

	int nResponse = dlg.DoModal();
//	dlg.ShowWindow(0);
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
#endif //__START_FPDM__

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
