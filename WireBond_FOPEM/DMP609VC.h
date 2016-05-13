// DMP609VC.h : main header file for the DMP609VC application
//

#if !defined(AFX_DMP609VC_H__1FFEB798_2B79_4D5B_AB0B_867FD73D438F__INCLUDED_)
#define AFX_DMP609VC_H__1FFEB798_2B79_4D5B_AB0B_867FD73D438F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDMP609VCApp:
// See DMP609VC.cpp for the implementation of this class
//

class CDMP609VCApp : public CWinApp
{
public:
	CDMP609VCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMP609VCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDMP609VCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMP609VC_H__1FFEB798_2B79_4D5B_AB0B_867FD73D438F__INCLUDED_)
