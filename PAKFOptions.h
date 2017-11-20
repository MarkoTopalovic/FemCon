#pragma once
#include "afxwin.h"
#include "GeneralOptTimePeriods.h"

// CPAKFOptions dialog

class CPAKFOptions : public CDialog
{
	DECLARE_DYNAMIC(CPAKFOptions)

public:
	CPAKFOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPAKFOptions();

// Dialog Data
	enum { IDD = IDD_DIALOG_PAKF };
	int		m_iSteady;
	int		m_iNewton;
	int		m_iPenalty;
	int		m_iStart;
	int		m_iAnalysis;
	CGeneralOptTimePeriods* dlgTimePeriods;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioSteady();
	afx_msg void OnBnClickedRadioUnsteady();
	afx_msg void OnBnClickedRadiomnewton();
	afx_msg void OnBnClickedRadiofnewton();
	afx_msg void OnBnClickedRadioStart();
	afx_msg void OnBnClickedRadioCheck();
	afx_msg void OnBnClickedRadioFfHt();
	afx_msg void OnBnClickedRadioFf();
	afx_msg void OnBnClickedRadioHt();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedTime();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
