// GeneralOptTimePeriods.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "GeneralOptTimePeriods.h"
#include "GeneralOptIterMethod.h"
#include "GeneralOptionsDlg.h"
extern CArray <int, int&> m_iExNoOfSteps;
extern CArray <double, double&> m_dExTimeStepIncrement;
extern int m_iArreyLenght;

// CGeneralOptTimePeriods dialog

IMPLEMENT_DYNAMIC(CGeneralOptTimePeriods, CDialog)

CGeneralOptTimePeriods::CGeneralOptTimePeriods(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralOptTimePeriods::IDD, pParent)
{
	m_iNumberOfSteps = 1;
	m_dIncrement = 1.0;
}

CGeneralOptTimePeriods::~CGeneralOptTimePeriods()
{
}

void CGeneralOptTimePeriods::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_OF_STEPS, m_iNumberOfSteps);
	DDV_MinMaxInt(pDX, m_iNumberOfSteps, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_TIME_STP_INC, m_dIncrement);
	DDV_MinMaxDouble(pDX, m_dIncrement, 1.e-006, 10000000000.);
}


BEGIN_MESSAGE_MAP(CGeneralOptTimePeriods, CDialog)
	ON_LBN_KILLFOCUS(IDC_LIST_TIME_PERIODS, &CGeneralOptTimePeriods::OnLbnKillfocusListTimePeriods)
	ON_LBN_SELCHANGE(IDC_LIST_TIME_PERIODS, &CGeneralOptTimePeriods::OnLbnSelchangeListTimePeriods)
	ON_BN_CLICKED(IDC_BUTTON_ADD_NEW_STP_TIM_INC, &CGeneralOptTimePeriods::OnBnClickedButtonAddNewStpTimInc)
	ON_BN_CLICKED(IDC_BUTTON_DEL_STP_TIM_INC, &CGeneralOptTimePeriods::OnBnClickedButtonDelStpTimInc)
	ON_BN_CLICKED(IDC_BUTTON_STP_TIM_CHANGE, &CGeneralOptTimePeriods::OnBnClickedButtonStpTimChange)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CGeneralOptTimePeriods::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGeneralOptTimePeriods::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGeneralOptTimePeriods message handlers

void CGeneralOptTimePeriods::OnLbnKillfocusListTimePeriods()
{
	// TODO: Add your control notification handler code here
}

void CGeneralOptTimePeriods::OnLbnSelchangeListTimePeriods()
{
	CListBox* list;
	CEdit* NumOfSteps, *Increment;
	CString item, strNum, strInc;
	int num;

	list=(CListBox*) GetDlgItem(IDC_LIST_TIME_PERIODS);
	NumOfSteps=(CEdit*) GetDlgItem(IDC_EDIT_NUM_OF_STEPS);
	Increment=(CEdit*) GetDlgItem(IDC_EDIT_TIME_STP_INC);
	
	list->GetText( list->GetCurSel(), item );
	strNum=item.Left(5);
	num=atoi(strNum);
	strNum.Format("%d",num);
	strInc=item.Right(15);
	
	NumOfSteps->SetWindowText(strNum);
	Increment->SetWindowText(strInc);

	CButton *del, *change;

	del=(CButton*) GetDlgItem(IDC_BUTTON_DEL_STP_TIM_INC);
	change=(CButton*) GetDlgItem(IDC_BUTTON_STP_TIM_CHANGE);

	if (!del->IsWindowEnabled())     del->EnableWindow(true);
	if (!change->IsWindowEnabled())  change->EnableWindow(true);
}

void CGeneralOptTimePeriods::OnBnClickedButtonAddNewStpTimInc()
{
	int num;
	double inc;
	CString sNum, sInc, item;
	CEdit* NumOfSteps, *Increment;
	
	NumOfSteps=(CEdit*) GetDlgItem(IDC_EDIT_NUM_OF_STEPS);
	Increment=(CEdit*) GetDlgItem(IDC_EDIT_TIME_STP_INC);

	NumOfSteps->GetWindowText(sNum);
	Increment->GetWindowText(sInc);
	
	num=atoi(sNum);
	inc=atof(sInc);

	if ((num>0) && (inc>0))
	{
		m_iNumOfTStep.Add(num);
		m_dTStepIncrement.Add(inc);

		RefreshListBox();
	}
}

void CGeneralOptTimePeriods::OnBnClickedButtonDelStpTimInc()
{
	CListBox* list;
	CButton* but, *change;
	int num;

	list=(CListBox*) GetDlgItem(IDC_LIST_TIME_PERIODS);
	but=(CButton*) GetDlgItem(IDC_BUTTON_DEL_STP_TIM_INC);
	change=(CButton*) GetDlgItem(IDC_BUTTON_STP_TIM_CHANGE);
	

	if (list->GetCount()==0) 
	{
		but->EnableWindow(false);
		change->EnableWindow(false);
		return;
	}
	
	num=list->GetCurSel();
	m_iNumOfTStep.RemoveAt(num);
	m_dTStepIncrement.RemoveAt(num);

	RefreshListBox();

	but->EnableWindow(false);
	change->EnableWindow(false);
}

void CGeneralOptTimePeriods::OnBnClickedButtonStpTimChange()
{
	int num, i;
	double inc;
	CString sNum, sInc, item;
	CEdit* NumOfSteps, *Increment;
	CListBox* list;

	list=(CListBox*) GetDlgItem(IDC_LIST_TIME_PERIODS);
	NumOfSteps=(CEdit*) GetDlgItem(IDC_EDIT_NUM_OF_STEPS);
	Increment=(CEdit*) GetDlgItem(IDC_EDIT_TIME_STP_INC);

	NumOfSteps->GetWindowText(sNum);
	Increment->GetWindowText(sInc);
	
	num=atoi(sNum);
	inc=atof(sInc);

	i=list->GetCurSel();
	
	if ((num>0) && (inc>0))
	{
		m_iNumOfTStep[i]=num;
		m_dTStepIncrement[i]=inc;

		RefreshListBox();
	}
}


int CGeneralOptTimePeriods::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

//	int temp;
//	double dtemp;

//	m_iNumOfTStep.RemoveAll();
//	m_dTStepIncrement.RemoveAll();
	m_iNumberOfSteps=1;
	m_dIncrement=1.;
	
	return 0;
}

int CGeneralOptTimePeriods::RefreshListBox() 
{
	CListBox* list;
	int i;
	CString item;

	list=(CListBox*) GetDlgItem(IDC_LIST_TIME_PERIODS);
	list->ResetContent();

	for(i=0; i<m_iNumOfTStep.GetSize(); i++)
	{
		item.Format("%5i  ->  %13.5e",m_iNumOfTStep[i], m_dTStepIncrement[i]);
		list->AddString(item);
	}
	
	return 0;
}

void CGeneralOptTimePeriods::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
		int i;
		if(m_iArreyLenght>0)
		{
			for(i=0; i<m_iArreyLenght; i++)
			{
				m_iNumOfTStep.Add(m_iExNoOfSteps[i]);
				m_dTStepIncrement.Add(m_dExTimeStepIncrement[i]);
			}
		}
	RefreshListBox();
}
void CGeneralOptTimePeriods::OnBnClickedOk()
{
	//OnOK();
	EndDialog(3);
	/*CGeneralOptIterMethod iterDlg;
	iterDlg.DoModal();*/
}

void CGeneralOptTimePeriods::OnBnClickedCancel()
{
	OnCancel();
	//CGeneralOptionsDlg generalDlg;
	//generalDlg.DoModal();
}
