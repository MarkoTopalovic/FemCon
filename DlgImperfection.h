#pragma once


// CDlgImperfection dialog

#include <afxtempl.h>

class Imperfection
{
public:
	int ModeNum;
	int NodeNum;
	int DOF;
	double Scale;
};

class CDlgImperfection : public CDialog
{
	DECLARE_DYNAMIC(CDlgImperfection)

public:
	CDlgImperfection(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgImperfection();
	CArray <Imperfection, Imperfection&> m_Data;

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPERFECTION };
	int		m_iModeNum;
	int		m_iNodeNumber;
	int		m_iDof;
	double	m_dImperfection;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
