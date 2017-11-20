#pragma once


// FinishDlg dialog

class FinishDlg : public CDialog
{
	DECLARE_DYNAMIC(FinishDlg)

public:
	FinishDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~FinishDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FINISH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
