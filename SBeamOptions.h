#pragma once


// CSBeamOptions dialog

class CSBeamOptions : public CDialog
{
	DECLARE_DYNAMIC(CSBeamOptions)

public:
	CSBeamOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSBeamOptions();

// Dialog Data
	enum { IDD = IDD_DIALOG_SBELEM_TRANS };
	double	m_dA;
	double	m_dBA;
	int		m_nType;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio0d();
	afx_msg void OnBnClickedRadio0d2();
	afx_msg void OnBnClickedButtonLoad();
	virtual void OnOK();
	virtual void OnCancel();
	
	afx_msg void OnBnClickedOk();
};
