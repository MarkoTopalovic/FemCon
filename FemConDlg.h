// FemConDlg.h : header file
//

#pragma once
#include "ModelData.h"
#include "FileIO.h"
#include "PakExpOpt.h"
#include "SendKeys.h"
#include "PAKFOptions.h"
#include "PAKTOptions.h"

// CFemConDlg dialog
class CFemConDlg : public CDialog
{
// Construction
public:
	CFemConDlg(CWnd* pParent = NULL);	// standard constructor
	UINT ImportFemap();
	UINT ExportPAK_S(CPakExpOpt *peo);
	UINT ExportPAK_T(CPakExpOpt *peo);
	UINT ExportPAK_E(CPakExpOpt *peo);
	UINT ExportPAK_F(CPakExpOpt *peo);
	UINT ExportPAK_P(CPakExpOpt *peo);
	UINT ExportPAK_PT(CPakExpOpt *peo);
	UINT ExportPAK_M(CPakExpOpt *peo);
	CModelData m_ModelData;
	CGeneralOptionsDlg dlgGeneralOptions;

	CPAKFOptions dlgPAKFOptions;
	CPAKTOptions dlgPAKTOptions;

	CGeneralDynAnOpt dlgGeneralDynAnOpt;
	CGeneralOptIterMethod dlgIterMethod;
	CGeneralOptTimePeriods dlgTimePeriods;
	CDlgImperfection dlgImperfection;
	CSBeamOptions dlgSBeamOptions;
	CMaterialDLG dlgMaterialDLG;
// Dialog Data
	enum { IDD = IDD_FEMCON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	BOOL	m_bGlobal;

	afx_msg void OnBnClickedBtnopen();
	afx_msg void OnBnClickedButtonExport();
	CString m_sFemapFile;
	afx_msg void OnBnClickedButtonoptions();
	CString m_sType;
	void CreateNEU(void);
	void ConnectToFemap(void);
	bool shownDialog;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonFinish();
	bool displayFinished;
	
	afx_msg void OnCbnSelchangeComboSolver();
	CString m_iSolverText;
	afx_msg void OnBnClickedButtonAnalyse();
private:
	CSendKeys m_sk;
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheck1();
};
