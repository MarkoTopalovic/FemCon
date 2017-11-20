#pragma once

#include <afxtempl.h>

// CGeneralOptTimePeriods dialog

class CGeneralOptTimePeriods : public CDialog
{
	DECLARE_DYNAMIC(CGeneralOptTimePeriods)

public:
	CGeneralOptTimePeriods(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGeneralOptTimePeriods();
	CArray <int, int&> m_iNumOfTStep;
	CArray <double, double&> m_dTStepIncrement;

// Dialog Data
	enum { IDD = IDD_DIALOG_TIME_PERIODS };
	int		m_iNumberOfSteps;
	double	m_dIncrement;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnKillfocusListTimePeriods();
	afx_msg void OnLbnSelchangeListTimePeriods();
	afx_msg void OnBnClickedButtonAddNewStpTimInc();
	afx_msg void OnBnClickedButtonDelStpTimInc();
	afx_msg void OnBnClickedButtonStpTimChange();
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

private:
	int RefreshListBox();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
