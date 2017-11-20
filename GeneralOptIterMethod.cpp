// GeneralOptIterMethod.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "GeneralOptIterMethod.h"
#include "DlgImperfection.h"
#include "GeneralOptTimePeriods.h"
extern double m_dExTOLE;
extern double m_dExTOLS;
extern int m_iExItherMethod;
extern bool m_bExConvForce;
extern bool m_bExConvEnergy;
// CGeneralOptIterMethod dialog

IMPLEMENT_DYNAMIC(CGeneralOptIterMethod, CDialog)

CGeneralOptIterMethod::CGeneralOptIterMethod(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralOptIterMethod::IDD, pParent)
{
	
	m_dTOLE = m_dExTOLE;
	m_dTOLS = m_dExTOLS;
	m_iDirection = 1;
	m_dValue = 1.0;
	m_dAG = 1.0;
	m_dDS = 1.0;
	m_strMethod = _T("Full Newton + line search");
	m_bConvForce = m_bExConvForce;
	m_bConvEnergy = m_bExConvEnergy;
	m_uNodeNumber = 1;
	m_iItherMethod = m_iExItherMethod;
}

CGeneralOptIterMethod::~CGeneralOptIterMethod()
{
}

void CGeneralOptIterMethod::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOLE, m_dTOLE);
	DDV_MinMaxDouble(pDX, m_dTOLE, 1.e-012, 1.);
	DDX_Text(pDX, IDC_EDIT_TOLS, m_dTOLS);
	DDV_MinMaxDouble(pDX, m_dTOLS, 1.e-012, 1.);
	DDX_Text(pDX, IDC_EDIT_DIRECTION, m_iDirection);
	DDV_MinMaxInt(pDX, m_iDirection, 1, 6);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_dValue);
	DDV_MinMaxDouble(pDX, m_dValue, -10000000., 10000000.);
	DDX_Text(pDX, IDC_EDIT_AG, m_dAG);
	DDV_MinMaxDouble(pDX, m_dAG, -10000000., 10000000.);
	DDX_Text(pDX, IDC_EDIT_DS, m_dDS);
	DDV_MinMaxDouble(pDX, m_dDS, -10000000., 10000000.);
	DDX_CBString(pDX, IDC_COMBO_DLG_ITER_METHOD, m_strMethod);
	DDX_Check(pDX, IDC_CHECK_CONCRIT_FORCE, m_bConvForce);
	DDX_Check(pDX, IDC_CHECK_CONCRIT_ENERGY, m_bConvEnergy);
	DDX_Text(pDX, IDC_EDIT_NOD_NUM, m_uNodeNumber);
	DDV_MinMaxUInt(pDX, m_uNodeNumber, 1, 4294967295);
}


BEGIN_MESSAGE_MAP(CGeneralOptIterMethod, CDialog)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_DLG_ITER_METHOD, &CGeneralOptIterMethod::OnCbnSelchangeComboDlgIterMethod)
	ON_BN_CLICKED(IDOK, &CGeneralOptIterMethod::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGeneralOptIterMethod::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CGeneralOptIterMethod message handlers

int CGeneralOptIterMethod::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

//	m_strMethod="Full Newton + line search";
//	m_bConvEnergy = true;
//	m_bConvForce = false;
//	m_dTOLE = 1.e-03;
//	m_dTOLS = 1.e-03;
//	m_uNodeNumber = 1;
//	m_dAG = 1.;
//	m_dDS = 1.;
//	m_dValue = 1.;
//	m_iDirection = 1;
//	m_iItherMethod = 4;

	return 0;
}

void CGeneralOptIterMethod::OnCbnSelchangeComboDlgIterMethod()
{
	CComboBox* box;
	CEdit *nn, *dir, *val, *ag, *ds;
	
	box=(CComboBox*) GetDlgItem(IDC_COMBO_DLG_ITER_METHOD);
	nn=(CEdit*) GetDlgItem(IDC_EDIT_NOD_NUM);
	dir=(CEdit*) GetDlgItem(IDC_EDIT_DIRECTION);
	val=(CEdit*) GetDlgItem(IDC_EDIT_VALUE);
	ag=(CEdit*) GetDlgItem(IDC_EDIT_AG);
	ds=(CEdit*) GetDlgItem(IDC_EDIT_DS);

	if (box->GetCurSel()>5)
	{
		if (!nn->IsWindowEnabled())  nn->EnableWindow(true);
		if (!dir->IsWindowEnabled()) dir->EnableWindow(true);
		if (!val->IsWindowEnabled()) val->EnableWindow(true);
		if (!ag->IsWindowEnabled())  ag->EnableWindow(true);
		if (!ds->IsWindowEnabled())  ds->EnableWindow(true);
	}
	else
	{
		if (nn->IsWindowEnabled())  nn->EnableWindow(false);
		if (dir->IsWindowEnabled()) dir->EnableWindow(false);
		if (val->IsWindowEnabled()) val->EnableWindow(false);
		if (ag->IsWindowEnabled())  ag->EnableWindow(false);
		if (ds->IsWindowEnabled())  ds->EnableWindow(false);
	}

	m_iItherMethod = box->GetCurSel();

	switch (m_iItherMethod)
	{
	case  0 : m_iItherMethod=-1;
			  break;
	case  1 : m_iItherMethod= 1;
			  break;
	case  2 : m_iItherMethod= 2;
			  break;
	case  3 : m_iItherMethod= 3;
			  break;
	case  4 : m_iItherMethod= 4;
			  break;
	case  5 : m_iItherMethod= 5;
			  break;
	case  6 : m_iItherMethod=61;
			  break;
	case  7 : m_iItherMethod=63;
			  break;
	case  8 : m_iItherMethod=71;
			  break;
	case  9 : m_iItherMethod=73;
			  break;
	}
}

void CGeneralOptIterMethod::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CGeneralOptIterMethod::OnBnClickedOk()
{	
	CString Energy = "";
	CString Force = "";
	CString NodeNumber = "";
	CString DS = "";
	CString AG = "";
	CString Value = "";
	CString Direction = "";
	CString Tole = "";
	CString Tols = "";
	CEdit *nn, *dir, *val, *ag, *ds, *tole, *tols;
	CButton *energy, *force;
	energy=(CButton*) GetDlgItem(IDC_CHECK_CONCRIT_ENERGY);
	force=(CButton*) GetDlgItem(IDC_CHECK_CONCRIT_FORCE);
	tole=(CEdit*) GetDlgItem(IDC_EDIT_TOLE);
	tols=(CEdit*) GetDlgItem(IDC_EDIT_TOLS);
	nn=(CEdit*) GetDlgItem(IDC_EDIT_NOD_NUM);
	dir=(CEdit*) GetDlgItem(IDC_EDIT_DIRECTION);
	val=(CEdit*) GetDlgItem(IDC_EDIT_VALUE);
	ag=(CEdit*) GetDlgItem(IDC_EDIT_AG);
	ds=(CEdit*) GetDlgItem(IDC_EDIT_DS);
	bool m_bExConvForce, m_bExConvEnergy;

	tole->GetWindowTextA(Tole);
	tols->GetWindowTextA(Tols);
	nn->GetWindowText(NodeNumber);
	dir->GetWindowTextA(Direction);
	val->GetWindowTextA(Value);
	ag->GetWindowTextA(AG);
	ds->GetWindowTextA(DS);
	m_bConvForce = force->GetCheck();
	m_bConvEnergy = energy->GetCheck();

	m_bExConvForce = m_bConvForce;
	m_bExConvEnergy = m_bConvEnergy;
	
	m_dTOLE = atof(Tole);
	m_dTOLS = atof(Tols);
	
	m_uNodeNumber = atoi(NodeNumber);
	m_iDirection = atoi(Direction);
	m_dValue = atof(Value);
	m_dAG = atof(AG);
	m_dDS =	atof(DS);

	//OnOK();
	EndDialog(3);
	//CDlgImperfection imperfectDlg;
	//imperfectDlg.DoModal();
}

void CGeneralOptIterMethod::OnBnClickedCancel()
{
	OnCancel();
	//CGeneralOptTimePeriods timeDlg;
	//timeDlg.DoModal();
}


void CGeneralOptIterMethod::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	UpdateData();
	m_dTOLE = m_dExTOLE;
	m_dTOLS = m_dExTOLS;
	m_iItherMethod = m_iExItherMethod;
	CComboBox* box;
	box=(CComboBox*) GetDlgItem(IDC_COMBO_DLG_ITER_METHOD);
	if (m_iExItherMethod==5)
	{
		box->SetCurSel(6);
		m_strMethod="BFGS";
	}	
	else if (m_iExItherMethod==3)
	{
		box->SetCurSel(4);
		m_strMethod="Full Newton";
	}
	else if (m_iExItherMethod==1)
	{
		box->SetCurSel(2);
		m_strMethod="Modified Newton";
	}	
	else if (m_iExItherMethod==6)
	{
		box->SetCurSel(7);
		m_strMethod="Arc lenght + modified Newton";
	}	
	else if (m_iExItherMethod==7)
	{
		box->SetCurSel(8);
		m_strMethod="Arc lenght + full Newton";
	}	


	CEdit *nn, *dir, *val, *ag, *ds;
	
	nn=(CEdit*) GetDlgItem(IDC_EDIT_NOD_NUM);
	dir=(CEdit*) GetDlgItem(IDC_EDIT_DIRECTION);
	val=(CEdit*) GetDlgItem(IDC_EDIT_VALUE);
	ag=(CEdit*) GetDlgItem(IDC_EDIT_AG);
	ds=(CEdit*) GetDlgItem(IDC_EDIT_DS);

	if (box->GetCurSel()>5)
	{
		if (!nn->IsWindowEnabled())  nn->EnableWindow(true);
		if (!dir->IsWindowEnabled()) dir->EnableWindow(true);
		if (!val->IsWindowEnabled()) val->EnableWindow(true);
		if (!ag->IsWindowEnabled())  ag->EnableWindow(true);
		if (!ds->IsWindowEnabled())  ds->EnableWindow(true);
	}
	else
	{
		if (nn->IsWindowEnabled())  nn->EnableWindow(false);
		if (dir->IsWindowEnabled()) dir->EnableWindow(false);
		if (val->IsWindowEnabled()) val->EnableWindow(false);
		if (ag->IsWindowEnabled())  ag->EnableWindow(false);
		if (ds->IsWindowEnabled())  ds->EnableWindow(false);
	}

	m_bConvForce = m_bExConvForce;
	m_bConvEnergy = m_bExConvEnergy;
	UpdateData(FALSE);	
}
