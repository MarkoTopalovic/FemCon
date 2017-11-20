// SBeamOptions.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "SBeamOptions.h"


// CSBeamOptions dialog

IMPLEMENT_DYNAMIC(CSBeamOptions, CDialog)

CSBeamOptions::CSBeamOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CSBeamOptions::IDD, pParent)
{
	m_dA = 0.0;
	m_dBA = 0.0;
	m_nType = 0;
}

CSBeamOptions::~CSBeamOptions()
{
}

void CSBeamOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_A, m_dA);
	DDV_MinMaxDouble(pDX, m_dA, 1.e-009, 9999999999.);
	DDX_Text(pDX, IDC_EDIT_BA, m_dBA);
	DDV_MinMaxDouble(pDX, m_dBA, 0., 999999999.);
	DDX_Radio(pDX, IDC_RADIO_0D, m_nType);
}


BEGIN_MESSAGE_MAP(CSBeamOptions, CDialog)
	ON_BN_CLICKED(IDC_RADIO_0D, &CSBeamOptions::OnBnClickedRadio0d)
	ON_BN_CLICKED(IDC_RADIO_0D2, &CSBeamOptions::OnBnClickedRadio0d2)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CSBeamOptions::OnBnClickedButtonLoad)
	
	ON_BN_CLICKED(IDOK, &CSBeamOptions::OnBnClickedOk)
END_MESSAGE_MAP()


// CSBeamOptions message handlers

void CSBeamOptions::OnBnClickedRadio0d()
{
	CWnd* b;

	b=GetDlgItem(IDC_EDIT_A);
	b->EnableWindow(true);
	b=GetDlgItem(IDC_EDIT_BA);
	b->EnableWindow(true);

	b=GetDlgItem(IDC_EDIT_FILE);
	b->EnableWindow(false);
	b=GetDlgItem(IDC_BUTTON_LOAD);
	b->EnableWindow(false);
	m_nType = 0;
}

void CSBeamOptions::OnBnClickedRadio0d2()
{
	CWnd* b;

	b=GetDlgItem(IDC_EDIT_A);
	b->EnableWindow(false);
	b=GetDlgItem(IDC_EDIT_BA);
	b->EnableWindow(false);

	b=GetDlgItem(IDC_EDIT_FILE);
	b->EnableWindow(true);
	b=GetDlgItem(IDC_BUTTON_LOAD);
	b->EnableWindow(true);
	m_nType = 1;
}

void CSBeamOptions::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
}

void CSBeamOptions::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CSBeamOptions::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void CSBeamOptions::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString Da = "";
	CString Dba = "";
	CEdit *da, *dba;
	da=(CEdit*) GetDlgItem(IDC_EDIT_A);
	da->GetWindowTextA(Da);
	m_dA = atof(Da);
	dba = (CEdit*) GetDlgItem(IDC_EDIT_BA);
	dba->GetWindowTextA(Dba);
	m_dBA = atof(Dba);
	
	OnOK();
}
