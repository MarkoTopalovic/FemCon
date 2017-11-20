// AnalysedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "FemConDlg.h"
#include "AnalysedDlg.h"
#include <windows.h>
#include "atlsafe.h" 
#include "oleauto.h"
#import "C:\Program Files\Siemens\FEMAPv114\femap.tlb" named_guids
extern CString m_sNEUPath;

// AnalysedDlg dialog

IMPLEMENT_DYNAMIC(AnalysedDlg, CDialog)

AnalysedDlg::AnalysedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AnalysedDlg::IDD, pParent)
{
	
}

AnalysedDlg::~AnalysedDlg()
{
}

void AnalysedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AnalysedDlg, CDialog)
	ON_BN_CLICKED(IDOK, &AnalysedDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// AnalysedDlg message handlers

void AnalysedDlg::OnBnClickedOk()
{
	
	CLSID       clsid;
	HRESULT hr=CLSIDFromProgID( L"femap.model", &clsid ); 
	IUnknown    FAR *punk;
	if ( hr == S_OK )
	{
		CoInitialize(NULL);
		hr=GetActiveObject(clsid,NULL,&punk);
		if ( hr == S_OK )
		{
			//CString sNEUPath;// = 
			
			_bstr_t bstrt(m_sNEUPath);
			CComQIPtr<femap::Imodel> pModel;
			pModel=punk;
			pModel->feFileReadNeutral2(42, bstrt, false, false, true, false, false, true, 0);
			pModel->feAppManagePanes("FemCon",0);
		}
	}

	OnOK();
}
