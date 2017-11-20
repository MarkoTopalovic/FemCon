// FemCon.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


#include "atlsafe.h" 
#include "oleauto.h"
//#include <cstringt.h> //needed for damn CString
#include <fstream>
#import "C:\Program Files\Siemens\FEMAPv114\femap.tlb" named_guids


// CFemConApp:
// See FemCon.cpp for the implementation of this class
//

class CFemConApp : public CWinApp
{
public:
	CFemConApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFemConApp theApp;