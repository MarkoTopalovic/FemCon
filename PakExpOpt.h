#pragma once


// CPakExpOpt dialog

class CPakExpOpt : public CDialog
{
	DECLARE_DYNAMIC(CPakExpOpt)

public:
	CPakExpOpt(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPakExpOpt();

// Dialog Data
	enum { IDD = IDD_DIALOG_PAKEXPOPT };
	CButton	m_c4N2BD;
	BOOL	m_b2D2Sh;
	BOOL	m_b4N2BD;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck2d2sh();
};
