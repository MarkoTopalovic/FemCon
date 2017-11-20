#pragma once

#include "GeneralDynAnOpt.h"
#include "GeneralOptIterMethod.h"
#include "GeneralOptTimePeriods.h"
#include "DlgImperfection.h"
#include "MaterialDLG.h"
// CGeneralOptionsDlg dialog

class CGeneralOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CGeneralOptionsDlg)

public:
	CGeneralOptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGeneralOptionsDlg();

// Dialog Data
	enum { IDD = IDD_GENERAL_OPTIONS };
	int		m_iNSOPV;
	BOOL	m_bDynamic;
	BOOL	m_bEigenValue;
	BOOL	m_bSuperBeam;
	CString	m_strTitle;
	int		m_iProgramStart;
	CString	m_strAnalysisType;

	int     m_iAnalysisType;
	CGeneralOptTimePeriods* dlgTimePeriods;
	CGeneralOptIterMethod* dlgIterMethod;
	CDlgImperfection *dlgImperfection;
	CMaterialDLG *dlgMaterialDLG;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboAnalysisType();
	afx_msg void OnBnClickedCheckDynamic();
	afx_msg void OnBnClickedButtonDynamicOpt();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckSuperbeam();
	afx_msg void OnBnClickedButtonSuperbeam();
	
	afx_msg void OnBnClickedButtonMaterial();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
