// DlgImperfection.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "DlgImperfection.h"
#include "GeneralOptIterMethod.h"

// CDlgImperfection dialog

IMPLEMENT_DYNAMIC(CDlgImperfection, CDialog)

CDlgImperfection::CDlgImperfection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImperfection::IDD, pParent)
{
	m_iModeNum = 0;
	m_iNodeNumber = 0;
	m_iDof = 0;
	m_dImperfection = 0.0;
}

CDlgImperfection::~CDlgImperfection()
{
}

void CDlgImperfection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ModeNumber, m_iModeNum);
	DDV_MinMaxInt(pDX, m_iModeNum, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_NodeNumber, m_iNodeNumber);
	DDV_MinMaxInt(pDX, m_iNodeNumber, 1, 999999);
	DDX_Text(pDX, IDC_EDIT_Dof, m_iDof);
	DDV_MinMaxInt(pDX, m_iDof, 1, 6);
	DDX_Text(pDX, IDC_EDIT_Imperfection, m_dImperfection);
}


BEGIN_MESSAGE_MAP(CDlgImperfection, CDialog)
	ON_BN_CLICKED(IDAPPLY, &CDlgImperfection::OnBnClickedApply)
	ON_BN_CLICKED(IDCANCEL, &CDlgImperfection::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgImperfection::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgImperfection message handlers

void CDlgImperfection::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDlgImperfection::OnBnClickedApply()
{
	CString Modenum = "";
	CString Nodenumber = "";
	CString Dof = "";
	CString Imp = "";
	CEdit *modenum, *nodenumber, *dof, *imperfection;
	modenum=(CEdit*) GetDlgItem(IDC_EDIT_ModeNumber);
	modenum->GetWindowTextA(Modenum);
	m_iModeNum = atoi(Modenum);
	nodenumber=(CEdit*) GetDlgItem(IDC_EDIT_NodeNumber);
	nodenumber->GetWindowTextA(Nodenumber);
	m_iNodeNumber = atoi(Nodenumber);
	dof = (CEdit*) GetDlgItem(IDC_EDIT_Dof);
	dof->GetWindowTextA(Dof);
	m_iDof = atoi(Dof);
	imperfection=(CEdit*) GetDlgItem(IDC_EDIT_Imperfection);
	imperfection->GetWindowTextA(Imp);
	m_dImperfection = atof(Imp);
	
	Imperfection data;
	
	if (UpdateData(true))
	{
		data.NodeNum = m_iNodeNumber;
		data.ModeNum = this->m_iModeNum;
		data.DOF = m_iDof;
		data.Scale = m_dImperfection;
		
		m_Data.Add(data);
	}
}

void CDlgImperfection::OnBnClickedCancel()
{
	OnCancel();
	CGeneralOptIterMethod iterDlg;
	iterDlg.DoModal();
}

void CDlgImperfection::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	EndDialog(3);
}
