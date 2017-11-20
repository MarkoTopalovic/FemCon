#pragma once


// CPAKTOptions dialog

class CPAKTOptions : public CDialog
{
	DECLARE_DYNAMIC(CPAKTOptions)

public:
	CPAKTOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPAKTOptions();

// Dialog Data
	enum { IDD = IDD_DIALOG_PAKT };
	int m_iTEmpCoeff1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
