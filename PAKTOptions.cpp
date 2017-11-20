// PAKTOptions.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "PAKTOptions.h"

extern int m_iExTEmpCoeff1;
// CPAKTOptions dialog

IMPLEMENT_DYNAMIC(CPAKTOptions, CDialog)

CPAKTOptions::CPAKTOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CPAKTOptions::IDD, pParent)
{
		m_iTEmpCoeff1 = m_iExTEmpCoeff1;
}

CPAKTOptions::~CPAKTOptions()
{
}

void CPAKTOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEMP_COEFF1, m_iTEmpCoeff1);
}


BEGIN_MESSAGE_MAP(CPAKTOptions, CDialog)
	ON_BN_CLICKED(IDOK, &CPAKTOptions::OnBnClickedOk)
END_MESSAGE_MAP()


// CPAKTOptions message handlers

void CPAKTOptions::OnBnClickedOk()
{
	CString TempCoeff1 = "";
	CEdit *coeff1;
	coeff1 = (CEdit*) GetDlgItem(IDC_TEMP_COEFF1);
	coeff1->GetWindowTextA(TempCoeff1);
	m_iTEmpCoeff1 = atoi(TempCoeff1);
	m_iExTEmpCoeff1 = m_iTEmpCoeff1;
	// TODO: Add your control notification handler code here
	OnOK();
}
