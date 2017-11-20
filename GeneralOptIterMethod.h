#pragma once


// CGeneralOptIterMethod dialog

class CGeneralOptIterMethod : public CDialog
{
	DECLARE_DYNAMIC(CGeneralOptIterMethod)

public:
	CGeneralOptIterMethod(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGeneralOptIterMethod();
	int m_iItherMethod;
// Dialog Data
	enum { IDD = IDD_DIALOG_ITER_METHOD };
	double	m_dTOLE;
	double	m_dTOLS;
	int		m_iDirection;
	double	m_dValue;
	double	m_dAG;
	double	m_dDS;
	CString	m_strMethod;
	BOOL	m_bConvForce;
	BOOL	m_bConvEnergy;
	UINT	m_uNodeNumber;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboDlgIterMethod();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
