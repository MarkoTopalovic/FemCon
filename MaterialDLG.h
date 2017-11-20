#pragma once


// CMaterialDLG dialog

class CMaterialDLG : public CDialog
{
	DECLARE_DYNAMIC(CMaterialDLG)

public:
	CMaterialDLG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaterialDLG();

// Dialog Data
	enum { IDD = IDD_DIALOG_MATERIAL };
	double m_dTAUY;
	double m_dCy;
	double m_dEM;
	double m_dHS;
	double m_dAN;
	BOOL m_bRamberg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnBnClickedCheckRamberg();
	afx_msg void OnPaint();
};
