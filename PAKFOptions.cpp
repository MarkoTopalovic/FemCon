// PAKFOptions.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "PAKFOptions.h"
#include "GeneralOptTimePeriods.h"
#include "GeneralOptionsDlg.h"
extern int m_iExFNSTAC;
extern int m_iExPenalty;
extern int m_iExNjrap;
extern int m_iExRest;
extern int m_iExIndf;


// CPAKFOptions dialog

IMPLEMENT_DYNAMIC(CPAKFOptions, CDialog)

CPAKFOptions::CPAKFOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CPAKFOptions::IDD, pParent)
	, m_iSteady(FALSE)
	, m_iNewton(TRUE)
	, m_iStart(FALSE)
//	, m_iAnalysis(FALSE)
{
	m_iAnalysis = m_iExIndf;
	m_iPenalty = 0;
}

CPAKFOptions::~CPAKFOptions()
{
}

void CPAKFOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_Steady, m_iSteady);
	DDX_Radio(pDX, IDC_RADIO_mNewton, m_iNewton);
	DDX_Radio(pDX, IDC_RADIO_Start, m_iStart);
	DDX_Radio(pDX, IDC_RADIO_FF_HT, m_iAnalysis);
}


BEGIN_MESSAGE_MAP(CPAKFOptions, CDialog)
//	ON_BN_CLICKED(IDOK, &CPAKFOptions::OnBnClickedOk)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CPAKFOptions::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_Steady, &CPAKFOptions::OnBnClickedRadioSteady)
	ON_BN_CLICKED(IDC_RADIO_Unsteady, &CPAKFOptions::OnBnClickedRadioUnsteady)
	ON_BN_CLICKED(IDC_RADIO_mNewton, &CPAKFOptions::OnBnClickedRadiomnewton)
	ON_BN_CLICKED(IDC_RADIO_fNewton, &CPAKFOptions::OnBnClickedRadiofnewton)
	ON_BN_CLICKED(IDC_RADIO_Start, &CPAKFOptions::OnBnClickedRadioStart)
	ON_BN_CLICKED(IDC_RADIO_Check, &CPAKFOptions::OnBnClickedRadioCheck)
	ON_BN_CLICKED(IDC_RADIO_FF_HT, &CPAKFOptions::OnBnClickedRadioFfHt)
	ON_BN_CLICKED(IDC_RADIO_FF, &CPAKFOptions::OnBnClickedRadioFf)
	ON_BN_CLICKED(IDC_RADIO_HT, &CPAKFOptions::OnBnClickedRadioHt)
	ON_BN_CLICKED(IDC_CHECK3, &CPAKFOptions::OnBnClickedCheck3)
	ON_BN_CLICKED(IDTime, &CPAKFOptions::OnBnClickedTime)
	END_MESSAGE_MAP()

// CPAKFOptions message handlers

int CPAKFOptions::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
void CPAKFOptions::OnBnClickedOk()
{
	if(m_iSteady==0)	m_iExFNSTAC = 1;
	if(m_iSteady==1)	m_iExFNSTAC = 0;
	m_iExPenalty = m_iPenalty;
	if(m_iNewton = 0)  m_iExNjrap = 2;
	if(m_iNewton = 1)  m_iExNjrap = 1;
	if(m_iStart==0)	m_iExRest = 1;
	if(m_iStart==1)	m_iExRest = 0;
//	m_iExRest = m_iStart;
	m_iExIndf = m_iAnalysis;

	OnOK();
}

void CPAKFOptions::OnBnClickedRadioSteady()
{
	m_iSteady = 0;
}

void CPAKFOptions::OnBnClickedRadioUnsteady()
{
	m_iSteady = 1;
}

void CPAKFOptions::OnBnClickedRadiomnewton()
{
	m_iNewton = 1;
}

void CPAKFOptions::OnBnClickedRadiofnewton()
{
	m_iNewton = 0;
}

void CPAKFOptions::OnBnClickedRadioStart()
{
	m_iStart = 1;
}

void CPAKFOptions::OnBnClickedRadioCheck()
{
	m_iStart = 0;
}

void CPAKFOptions::OnBnClickedRadioFfHt()
{
	m_iAnalysis = 0;
}

void CPAKFOptions::OnBnClickedRadioFf()
{
	m_iAnalysis = 1;
}

void CPAKFOptions::OnBnClickedRadioHt()
{
	m_iAnalysis = 2;
}

void CPAKFOptions::OnBnClickedCheck3()
{
	m_iPenalty = 1;
}

void CPAKFOptions::OnBnClickedTime()
{
//	OnOK();
	CGeneralOptTimePeriods dlgTimePeriods;
	
	dlgTimePeriods.DoModal();
}
