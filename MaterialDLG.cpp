// MaterialDLG.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "MaterialDLG.h"
extern double m_dExHS;
extern double m_dExAN;
extern bool m_bExRamberg;
extern double m_dExTAUY;
extern double m_dExCy;
extern double m_dExEM;
// CMaterialDLG dialog

IMPLEMENT_DYNAMIC(CMaterialDLG, CDialog)

CMaterialDLG::CMaterialDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialDLG::IDD, pParent)
{
	m_dHS = m_dExHS;
	m_dAN = m_dExAN;
	m_bRamberg = FALSE;
	m_dTAUY = m_dExTAUY;
	m_dCy = m_dExCy;
	m_dEM = m_dExEM;
}

CMaterialDLG::~CMaterialDLG()
{
}

void CMaterialDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HS, m_dHS);
	DDX_Text(pDX, IDC_EDIT_AN, m_dAN);
	DDX_Check(pDX, IDC_CHECK_RAMBERG, m_bRamberg);
	DDX_Text(pDX, IDC_EDIT_TAUY, m_dTAUY);
	DDX_Text(pDX, IDC_EDIT_Cy, m_dCy);
	DDX_Text(pDX, IDC_EDIT_EM, m_dEM);
}


BEGIN_MESSAGE_MAP(CMaterialDLG, CDialog)
	ON_BN_CLICKED(IDOK, &CMaterialDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMaterialDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_RAMBERG, &CMaterialDLG::OnBnClickedCheckRamberg)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMaterialDLG message handlers

void CMaterialDLG::OnBnClickedOk()
{
	CString HS = "";
	CString AN = "";
	CString TAUY = "";
	CString CY = "";
	CString EM = "";
	CEdit *hs, *an, *tauy, *cy, *em;
	hs = (CEdit*) GetDlgItem(IDC_EDIT_HS);
	an = (CEdit*) GetDlgItem(IDC_EDIT_AN);
	tauy = (CEdit*) GetDlgItem(IDC_EDIT_TAUY);
	cy = (CEdit*) GetDlgItem(IDC_EDIT_Cy);
	em = (CEdit*) GetDlgItem(IDC_EDIT_EM);
	hs->GetWindowTextA(HS);
	an->GetWindowTextA(AN);
	tauy->GetWindowTextA(TAUY);
	cy->GetWindowTextA(CY);
	em->GetWindowTextA(EM);

	m_dHS = atof(HS);
	m_dExHS = m_dHS;
	m_dAN = atof(AN);
	m_dExAN = m_dAN;
	m_dTAUY = atof(TAUY);
	m_dExTAUY = m_dTAUY;
	m_dCy = atof(CY); 
	m_dExCy = m_dCy;
	m_dEM = atof(EM);
	m_dExEM = m_dEM;

	OnOK();
}

void CMaterialDLG::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CMaterialDLG::OnBnClickedCheckRamberg()
{
	CButton *ramberg;
	
	ramberg=(CButton*) GetDlgItem(IDC_CHECK_RAMBERG);
	m_bRamberg = ramberg->GetState();
	m_bExRamberg = IsDlgButtonChecked (IDC_CHECK_RAMBERG) == BST_CHECKED;

	CEdit *hs, *an, *tauy, *cy, *em;
	hs = (CEdit*) GetDlgItem(IDC_EDIT_HS);
	an = (CEdit*) GetDlgItem(IDC_EDIT_AN);
	tauy = (CEdit*) GetDlgItem(IDC_EDIT_TAUY);
	cy = (CEdit*) GetDlgItem(IDC_EDIT_Cy);
	em = (CEdit*) GetDlgItem(IDC_EDIT_EM);
	hs->EnableWindow(m_bExRamberg);
	an->EnableWindow(m_bExRamberg);
	tauy->EnableWindow(m_bExRamberg);
	cy->EnableWindow(m_bExRamberg);
	em->EnableWindow(m_bExRamberg);
}

void CMaterialDLG::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CButton *ramberg;
	ramberg=(CButton*) GetDlgItem(IDC_CHECK_RAMBERG);
	if (m_bExRamberg == true)
	{
		ramberg->SetCheck(BST_CHECKED);
	}
	else
	{
		ramberg->SetCheck(BST_UNCHECKED);
	}
	CEdit *hs, *an, *tauy, *cy, *em;
	hs = (CEdit*) GetDlgItem(IDC_EDIT_HS);
	an = (CEdit*) GetDlgItem(IDC_EDIT_AN);
	tauy = (CEdit*) GetDlgItem(IDC_EDIT_TAUY);
	cy = (CEdit*) GetDlgItem(IDC_EDIT_Cy);
	em = (CEdit*) GetDlgItem(IDC_EDIT_EM);
	hs->EnableWindow(m_bExRamberg);
	an->EnableWindow(m_bExRamberg);
	tauy->EnableWindow(m_bExRamberg);
	cy->EnableWindow(m_bExRamberg);
	em->EnableWindow(m_bExRamberg);
	// Do not call CDialog::OnPaint() for painting messages
}
