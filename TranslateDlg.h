#pragma once


// CTranslateDlg dialog

class CTranslateDlg : public CDialog
{
	DECLARE_DYNAMIC(CTranslateDlg)

public:
	CTranslateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTranslateDlg();

// Dialog Data
	enum { IDD = IDD_TRANSLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
};
