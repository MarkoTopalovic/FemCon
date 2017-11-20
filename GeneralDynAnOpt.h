#pragma once


// CGeneralDynAnOpt dialog

class CGeneralDynAnOpt : public CDialog
{
	DECLARE_DYNAMIC(CGeneralDynAnOpt)

public:
	CGeneralDynAnOpt(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGeneralDynAnOpt();

// Dialog Data
	enum { IDD = IDD_DIALOG_DYN_OPTIONS };
	double	m_dFirstIntParam;
	double	m_dSecIntParam;
	int		m_dIntegrationMethod;
	int		m_iIDAMP;
	int		m_iIMASS;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioWhilson();
	afx_msg void OnBnClickedRadioNewmark();
	afx_msg void OnBnClickedRadioCentral();
	afx_msg void OnBnClickedOk();
};
