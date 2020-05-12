// Cube02.h : main header file for the CUBE02 application
//

#if !defined(AFX_CUBE02_H__D1938710_F9F0_4416_8C44_224FE10EE86E__INCLUDED_)
#define AFX_CUBE02_H__D1938710_F9F0_4416_8C44_224FE10EE86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCube02App:
// See Cube02.cpp for the implementation of this class
//

class CCube02App : public CWinApp
{
public:
	CCube02App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCube02App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCube02App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUBE02_H__D1938710_F9F0_4416_8C44_224FE10EE86E__INCLUDED_)
