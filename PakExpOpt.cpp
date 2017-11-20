// PakExpOpt.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "PakExpOpt.h"


// CPakExpOpt dialog

IMPLEMENT_DYNAMIC(CPakExpOpt, CDialog)

CPakExpOpt::CPakExpOpt(CWnd* pParent /*=NULL*/)
	: CDialog(CPakExpOpt::IDD, pParent)
{
	m_b2D2Sh = FALSE;
	m_b4N2BD = TRUE;
}

CPakExpOpt::~CPakExpOpt()
{
}

void CPakExpOpt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_4N2BD, m_c4N2BD);
	DDX_Check(pDX, IDC_CHECK_2D2SH, m_b2D2Sh);
	DDX_Check(pDX, IDC_CHECK_4N2BD, m_b4N2BD);
}


BEGIN_MESSAGE_MAP(CPakExpOpt, CDialog)
	ON_BN_CLICKED(IDC_CHECK_2D2SH, &CPakExpOpt::OnBnClickedCheck2d2sh)
END_MESSAGE_MAP()


// CPakExpOpt message handlers

void CPakExpOpt::OnBnClickedCheck2d2sh()
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_b2D2Sh) m_c4N2BD.EnableWindow(1);else m_c4N2BD.EnableWindow(0);
}
