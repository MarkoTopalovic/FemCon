#pragma once


// AnalysedDlg dialog

class AnalysedDlg : public CDialog
{
	DECLARE_DYNAMIC(AnalysedDlg)

public:
	AnalysedDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~AnalysedDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ANALYSED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
