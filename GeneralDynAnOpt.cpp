// GeneralDynAnOpt.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "GeneralDynAnOpt.h"


// CGeneralDynAnOpt dialog

IMPLEMENT_DYNAMIC(CGeneralDynAnOpt, CDialog)

CGeneralDynAnOpt::CGeneralDynAnOpt(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralDynAnOpt::IDD, pParent)
{
	m_dFirstIntParam = 0.0;
	m_dSecIntParam = 0.0;
	m_dIntegrationMethod = 1;
	m_iIDAMP = 0;
	m_iIMASS = 0;
}

CGeneralDynAnOpt::~CGeneralDynAnOpt()
{
}

void CGeneralDynAnOpt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIRST_INT_PAR, m_dFirstIntParam);
	DDV_MinMaxDouble(pDX, m_dFirstIntParam, 0., 1000.);
	DDX_Text(pDX, IDC_EDIT_SEC_INT_PAR, m_dSecIntParam);
	DDV_MinMaxDouble(pDX, m_dSecIntParam, 0., 1000.);
	DDX_Radio(pDX, IDC_RADIO_WHILSON, m_dIntegrationMethod);
	DDX_Text(pDX, IDC_EDIT_IDAMP, m_iIDAMP);
	DDV_MinMaxInt(pDX, m_iIDAMP, 0, 10);
	DDX_Text(pDX, IDC_EDIT_IMASS, m_iIMASS);
	DDV_MinMaxInt(pDX, m_iIMASS, 0, 10);
}


BEGIN_MESSAGE_MAP(CGeneralDynAnOpt, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RADIO_WHILSON, &CGeneralDynAnOpt::OnBnClickedRadioWhilson)
	ON_BN_CLICKED(IDC_RADIO_NEWMARK, &CGeneralDynAnOpt::OnBnClickedRadioNewmark)
	ON_BN_CLICKED(IDC_RADIO_CENTRAL, &CGeneralDynAnOpt::OnBnClickedRadioCentral)
	ON_BN_CLICKED(IDOK, &CGeneralDynAnOpt::OnBnClickedOk)
END_MESSAGE_MAP()


// CGeneralDynAnOpt message handlers

int CGeneralDynAnOpt::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	m_dIntegrationMethod = 1;
	m_dFirstIntParam = 0;
	m_dSecIntParam = 0;
	m_iIDAMP = 0;
	m_iIMASS = 0;
*/	
	return 0;
}

void CGeneralDynAnOpt::OnBnClickedRadioWhilson()
{
	m_dIntegrationMethod = 0;
	// TODO: Add your control notification handler code here
}

void CGeneralDynAnOpt::OnBnClickedRadioNewmark()
{
	m_dIntegrationMethod = 1;
	// TODO: Add your control notification handler code here
}

void CGeneralDynAnOpt::OnBnClickedRadioCentral()
{
	m_dIntegrationMethod = 2;
	// TODO: Add your control notification handler code here
}

void CGeneralDynAnOpt::OnBnClickedOk()
{
	CString Firstintparam = "";
	CString Secondintparam = "";
	CString Idamp = "";
	CString Imass = "";

	CEdit *firstip, *secondip, *idamp, *imass;

	firstip=(CEdit*) GetDlgItem(IDC_EDIT_FIRST_INT_PAR);
	secondip=(CEdit*) GetDlgItem(IDC_EDIT_SEC_INT_PAR);
	idamp=(CEdit*) GetDlgItem(IDC_EDIT_IDAMP);
	imass=(CEdit*) GetDlgItem(IDC_EDIT_IMASS);
	firstip->GetWindowTextA(Firstintparam);
	secondip->GetWindowTextA(Secondintparam);
	idamp->GetWindowTextA(Idamp);
	imass->GetWindowTextA(Imass);

	m_dFirstIntParam = atof(Firstintparam);
	m_dSecIntParam = atof(Secondintparam);
	m_iIDAMP = atoi(Idamp);
	m_iIMASS = atoi(Imass);
	OnOK();
}
