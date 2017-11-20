// GeneralOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "FemConDlg.h"
#include "GeneralOptionsDlg.h"
#include "GeneralDynAnOpt.h"
#include "GeneralOptTimePeriods.h"
#include "SBeamOptions.h"
#include "MaterialDLG.h"
extern bool m_bExDynamic;
extern int m_iExNSOPV;
// CGeneralOptionsDlg dialog

IMPLEMENT_DYNAMIC(CGeneralOptionsDlg, CDialog)

CGeneralOptionsDlg::CGeneralOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralOptionsDlg::IDD, pParent)
	//, m_bSuperBeam(FALSE)
{
	m_iNSOPV = m_iExNSOPV;
	m_bDynamic = FALSE;
	m_bEigenValue = FALSE;
	m_bSuperBeam = FALSE;
	m_strTitle = _T("PAK-S example");
	m_strAnalysisType = _T("Linear");
	m_iAnalysisType=0;
	m_iProgramStart=0;
}

CGeneralOptionsDlg::~CGeneralOptionsDlg()
{
}

void CGeneralOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NSOPV, m_iNSOPV);
	DDV_MinMaxInt(pDX, m_iNSOPV, 1, 1000);
	DDX_Check(pDX, IDC_CHECK_DYNAMIC, m_bDynamic);
	DDX_Check(pDX, IDC_CHECK_EIGEN, m_bEigenValue);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDV_MaxChars(pDX, m_strTitle, 100);
	DDX_CBString(pDX, IDC_COMBO_ANALYSIS_TYPE, m_strAnalysisType);
	DDX_Check(pDX, IDC_CHECK_SUPERBEAM, m_bSuperBeam);
}


BEGIN_MESSAGE_MAP(CGeneralOptionsDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ANALYSIS_TYPE, &CGeneralOptionsDlg::OnCbnSelchangeComboAnalysisType)
	ON_BN_CLICKED(IDC_CHECK_DYNAMIC, &CGeneralOptionsDlg::OnBnClickedCheckDynamic)
	ON_BN_CLICKED(IDC_BUTTON_DYNAMIC_OPT, &CGeneralOptionsDlg::OnBnClickedButtonDynamicOpt)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CGeneralOptionsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_SUPERBEAM, &CGeneralOptionsDlg::OnBnClickedCheckSuperbeam)
	ON_BN_CLICKED(IDC_BUTTON_SUPERBEAM, &CGeneralOptionsDlg::OnBnClickedButtonSuperbeam)
	ON_BN_CLICKED(IDC_BUTTON_MATERIAL, &CGeneralOptionsDlg::OnBnClickedButtonMaterial)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CGeneralOptionsDlg message handlers

int CGeneralOptionsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	m_bDynamic = 0;
	m_bEigenValue = 0;
	m_iNSOPV = 5;
	m_strTitle = "PAK-S example";
	m_iProgramStart = 0;
	m_iAnalysisType=0;
	m_strAnalysisType = "Linear";
*/
	return 0;
}

void CGeneralOptionsDlg::OnCbnSelchangeComboAnalysisType()
{
	CComboBox *box;
	box=(CComboBox*) GetDlgItem(IDC_COMBO_ANALYSIS_TYPE);
	m_iAnalysisType=box->GetCurSel();
}

void CGeneralOptionsDlg::OnBnClickedCheckDynamic()
{
	CButton *opt, *dyn;
	opt=(CButton*) GetDlgItem(IDC_BUTTON_DYNAMIC_OPT);
	dyn=(CButton*) GetDlgItem(IDC_CHECK_DYNAMIC);
	opt->EnableWindow( (dyn->GetState() & 0x0003));
}

void CGeneralOptionsDlg::OnBnClickedButtonDynamicOpt()
{
	CGeneralDynAnOpt dynDlg;
	dynDlg.DoModal();
}

void CGeneralOptionsDlg::OnBnClickedOk()
{
	CString Nsopv = "";
	CString Title = "";
	CButton *eigen, *dynamic, *superbeam;
	CEdit *nsopv, *title;
	eigen=(CButton*) GetDlgItem(IDC_CHECK_EIGEN);
	dynamic=(CButton*) GetDlgItem(IDC_CHECK_DYNAMIC);
	superbeam=(CButton*) GetDlgItem(IDC_CHECK_SUPERBEAM);
	nsopv = (CEdit*) GetDlgItem(IDC_EDIT_NSOPV);
	title = (CEdit*) GetDlgItem(IDC_EDIT_TITLE);
	
	nsopv->GetWindowTextA(Nsopv);
	title->GetWindowTextA(Title);
	m_bEigenValue = eigen->GetCheck();
	m_bDynamic = dynamic->GetCheck();
	m_bSuperBeam = superbeam->GetCheck();
	m_iNSOPV = atoi(Nsopv);
	m_strTitle = Title;
	OnOK();
	//CGeneralOptTimePeriods timeDlg;
	//timeDlg.DoModal();
	//CModelData m_ModelData;
	//m_ModelData.m_PakOptValue.dlgIterMethod.DoModal();
	//dlgTOPIterMethod.DoModal();
	//dlgTimePeriods.DoModal();
	//dlgTOPIterMethod->DoModal();
	//dlgIterMethod->DoModal();
	dlgTimePeriods->DoModal();
	dlgIterMethod->DoModal();
	dlgImperfection->DoModal();
}

void CGeneralOptionsDlg::OnBnClickedCheckSuperbeam()
{
	CButton *optSB, *sBeam;
	optSB=(CButton*) GetDlgItem(IDC_BUTTON_SUPERBEAM);
	sBeam=(CButton*) GetDlgItem(IDC_CHECK_SUPERBEAM);
	optSB->EnableWindow( (sBeam->GetState() & 0x0003));
}

void CGeneralOptionsDlg::OnBnClickedButtonSuperbeam()
{
	CSBeamOptions sbDlg;
	sbDlg.DoModal();
}

void CGeneralOptionsDlg::OnBnClickedButtonMaterial()
{
	CMaterialDLG dlgMaterialDLG;
	
	dlgMaterialDLG.DoModal();
}

void CGeneralOptionsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (m_bExDynamic == true)
	{
		m_iNSOPV = m_iExNSOPV;
UpdateData(FALSE);

		
		CButton *opt, *dyn, *eigen;
		opt=(CButton*) GetDlgItem(IDC_BUTTON_DYNAMIC_OPT);
		dyn=(CButton*) GetDlgItem(IDC_CHECK_DYNAMIC);
		eigen=(CButton*) GetDlgItem(IDC_CHECK_EIGEN);
		dyn->SetCheck(1);
		eigen->SetCheck(1);
		opt->EnableWindow(1);

		
	/*CEdit *nsopv;
	nsopv = (CEdit*) GetDlgItem(IDC_EDIT_NSOPV);
nsopv->SetWindowTextA(*/

	}
}
