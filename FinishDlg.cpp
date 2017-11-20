// FinishDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "FinishDlg.h"
#include "atlsafe.h" 
#include "oleauto.h"
#import "C:\Program Files\Siemens\FEMAPv114\femap.tlb" named_guids

// FinishDlg dialog

IMPLEMENT_DYNAMIC(FinishDlg, CDialog)

FinishDlg::FinishDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FinishDlg::IDD, pParent)
{

}

FinishDlg::~FinishDlg()
{
}

void FinishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FinishDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// FinishDlg message handlers

void FinishDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CLSID       clsid;
		HRESULT hr=AfxGetClassIDFromString( "femap.model", &clsid);
		IUnknown    FAR *punk;
		if ( hr == S_OK )
		{
			CoInitialize(NULL);
			hr=GetActiveObject(clsid,NULL,&punk);
			if ( hr == S_OK )
			{
				CComQIPtr<femap::Imodel> pModel;
				pModel=punk;
				pModel->feAppManagePanes("FemCon",0);
			}
		}
	// Do not call CDialog::OnPaint() for painting messages
}

void FinishDlg::OnClose()
{
	CDialog::OnClose();
	//EndDialog(3);
}

int FinishDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	//EndDialog(3);
	return 0;
}

