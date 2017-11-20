// FemConDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FemCon.h"
#include "FemConDlg.h"
#include "GeneralOptionsDlg.h"
#include "Shlwapi.h" //used for manipulating CString path
#include "FinishDlg.h"
#include "AnalysedDlg.h"
#include <windows.h>
//used to get process id for paks
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
//
#include "atlsafe.h" 
#include "oleauto.h"
#import "C:\Program Files\Siemens\FEMAPv114\femap.tlb" named_guids
CString m_sNEUPath = "";
int x;
//material extern variables
bool m_bExRamberg = false;
bool m_bExDynamic = false;
bool m_bExGlobal = false;
double m_dExHS = 0;
double m_dExAN = 0;
double m_dExTAUY = 0;
double m_dExCy = 0;
double m_dExEM = 1;
//end of material extern variables
bool m_bCanAnalyse = false; //used to enable/disable analyse button
int m_iSolver = 0;
//load extern variables
CArray <int, int&> m_iExNoOfSteps;
CArray <double, double&> m_dExTimeStepIncrement;
int m_iArreyLenght = 0;
double m_dExTOLE = 0.001;
double m_dExTOLS = 0.001;
double m_dExFEMAPversion = 0.0;
int m_iExItherMethod = 4;
int m_iExNSOPV = 5;
bool	m_bExConvForce = false;
bool	m_bExConvEnergy = true;
//end of load extern variables

//PAKT extern variables
int m_iExTEmpCoeff1 = 0;
//end of PAKT extern variables

//PAKF extern variables
int m_iExFNSTAC = 1;
int m_iExPenalty = 0;
int m_iExNjrap = 2;
int m_iExRest = 1;
int m_iExIndf = 1;
//end of PAKF extern variables

#include <process.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFemConDlg dialog




CFemConDlg::CFemConDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFemConDlg::IDD, pParent), m_sFemapFile(_T("not loaded"))
	, m_sType(_T("Linear"))
	, shownDialog(false)
	, displayFinished(false)
	, m_iSolverText(_T("PAK-S"))

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bGlobal = m_bExGlobal;
}

void CFemConDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FEMAP, m_sFemapFile);
//	DDX_Text(pDX, IDC_STATIC_ANALYSIS_TYPE, m_sType);
	DDX_CBString(pDX, IDC_COMBO_SOLVER, m_iSolverText);
	
	DDX_Check(pDX, IDC_CHECK1, m_bGlobal);

}

BEGIN_MESSAGE_MAP(CFemConDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNOPEN, &CFemConDlg::OnBnClickedBtnopen)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CFemConDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTONOPTIONS, &CFemConDlg::OnBnClickedButtonoptions)
	ON_BN_CLICKED(IDCANCEL, &CFemConDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CFemConDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_FINISH, &CFemConDlg::OnBnClickedButtonFinish)
	ON_CBN_SELCHANGE(IDC_COMBO_SOLVER, &CFemConDlg::OnCbnSelchangeComboSolver)
	ON_BN_CLICKED(IDC_BUTTON_ANALYSE, &CFemConDlg::OnBnClickedButtonAnalyse)
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, &CFemConDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CFemConDlg message handlers

BOOL CFemConDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFemConDlg::OnPaint()
{
	//NO4
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	
	/*if(shownDialog==false)
	{
		ConnectToFemap();
		shownDialog=true;
		CreateNEU();
	}*/
	
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFemConDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFemConDlg::OnBnClickedBtnopen()
{
	ImportFemap();
}

UINT CFemConDlg::ImportFemap() 
{

	CString str, szFilter;
	CStdioFile file;

	szFilter="FEMAP Neutral Files  (*.neu)|*.neu||";

	CFileDialog dlg (TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);

	if (dlg.DoModal() != IDOK)
		return(-1);
	
	if(!file.Open( dlg.GetPathName(),CFile::modeRead | CFile::typeText ))
	{
		AfxMessageBox(FILE_OPEN_ERROR);
		return(0);	
	}
		
	file.ReadString(str);
	file.ReadString(str);
	file.ReadString(str);
	file.ReadString(str);
	m_dExFEMAPversion = atof(str);
	if(m_dExFEMAPversion == 0.0)
	{
		m_dExFEMAPversion = 10.0;
		
	}
		
	file.ReadString(str);
		/*
	DeleteContents();
	OnNewDocument();*/

	while (!feof(file.m_pStream))
	{
		file.ReadString(str);

		if (str.Left(5)=="   -1")
		{
			file.ReadString(str);
	
			int nDataBlockType=atoi (str);

			if ((nDataBlockType==FBID_NODES)||(nDataBlockType==FBID_ELEMENTS)||(nDataBlockType==FBID_PROPERTIES)||(nDataBlockType==FBID_LOADS_OLD)||(nDataBlockType==FBID_LOADS)||
				(nDataBlockType==FBID_CONSTRAINTS_OLD)||(nDataBlockType==FBID_CONSTRAINTS)||(nDataBlockType==FBID_MATERIALS_OLD)||(nDataBlockType==FBID_MATERIALS)||(nDataBlockType==FBID_FUNCTIONS)||
				(nDataBlockType==FBID_CONTACT_SEGMENT)||(nDataBlockType==FBID_CONTACT_PROPERTY)||(nDataBlockType==FBID_CONNECTION)||(nDataBlockType==FBID_SETS))
			{
				m_ModelData.ImportFemap((CStdioFile *) &file, nDataBlockType);
			}
			else
			{
				file.ReadString(str);
	
				while((str.Left(5)!="   -1")&&(!feof(file.m_pStream)))
					file.ReadString(str);
			}
		}
		else
		{
			file.ReadString(str);

			while((str.Left(5)!="   -1")&&(!feof(file.m_pStream)))
				file.ReadString(str);
		}
	}

	m_sFemapFile = _T(file.GetFileName());
	UpdateData( FALSE );
	file.Close();

	return(-1);
}



UINT CFemConDlg::ExportPAK_S(CPakExpOpt *peo) 
{
	CStdioFile file;
	CString str, szFilter;
	szFilter="PAK DAT Files  (*.dat)|*.dat||";
	CFileDialog dlg (FALSE,"dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if (dlg.DoModal() != IDOK)
		return(-1);
	if(!file.Open( dlg.GetPathName(),CFile::modeCreate | CFile::modeWrite | CFile::typeText ))
	{
		AfxMessageBox(FILE_OPEN_ERROR);
		return(0);	
	}
	
//	Card /1/
	file.WriteString(CardH1);
	file.WriteString(CardV1);
	str="  Untitled\n";
	file.WriteString(str);
	m_ModelData.ExportPAK_S((CStdioFile *) &file,peo);
	file.Close();
	return(-1);
}
UINT CFemConDlg::ExportPAK_T(CPakExpOpt *peo) 
{
	CStdioFile file;
	CString str, szFilter;
	szFilter="PAK DAT Files  (*.dat)|*.dat||";
	CFileDialog dlg (FALSE,"dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if (dlg.DoModal() != IDOK)
		return(-1);
	if(!file.Open( dlg.GetPathName(),CFile::modeCreate | CFile::modeWrite | CFile::typeText ))
	{
		AfxMessageBox(FILE_OPEN_ERROR);
		return(0);	
	}
		
//	Card /1/
	file.WriteString(CardH1);
	file.WriteString(CardV1);
	str="  Untitled\n";
	file.WriteString(str);
	m_ModelData.ExportPAK_T((CStdioFile *) &file,peo);
	file.Close();
	return(-1);
}

UINT CFemConDlg::ExportPAK_E(CPakExpOpt *peo) 
{
	CStdioFile file;
	CString str, szFilter;
	szFilter="PAK DAT Files  (*.dat)|*.dat||";
	CFileDialog dlg (FALSE,"dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if (dlg.DoModal() != IDOK)
		return(-1);
	if(!file.Open( dlg.GetPathName(),CFile::modeCreate | CFile::modeWrite | CFile::typeText ))
	{
		AfxMessageBox(FILE_OPEN_ERROR);
		return(0);	
	}
		
//	Card /1/
	file.WriteString(CardH1);
	file.WriteString(CardV1);
	str="  Untitled\n";
	file.WriteString(str);
	m_ModelData.ExportPAK_E((CStdioFile *) &file,peo);
	file.Close();
	return(-1);
}

UINT CFemConDlg::ExportPAK_M(CPakExpOpt *peo) 
{
	CStdioFile file;
	CString str, szFilter;
	szFilter="PAK DAT Files  (*.dat)|*.dat||";
	CFileDialog dlg (FALSE,"dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if (dlg.DoModal() != IDOK)
		return(-1);
	if(!file.Open( dlg.GetPathName(),CFile::modeCreate | CFile::modeWrite | CFile::typeText ))
	{
		AfxMessageBox(FILE_OPEN_ERROR);
		return(0);	
	}
		
//	Card /1/
	file.WriteString(CardH1);
	file.WriteString(CardV1);
	str="  Untitled\n";
	file.WriteString(str);
	m_ModelData.ExportPAK_M((CStdioFile *) &file,peo);
	file.Close();
	return(-1);
}

UINT CFemConDlg::ExportPAK_F(CPakExpOpt *peo) 
{
	CStdioFile file;
	CString str, szFilter;

	szFilter="PAK DAT Files  (*.dat)|*.dat||";

	CFileDialog dlg (FALSE,"dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);

	if (dlg.DoModal() != IDOK)
		return(-1);
	
	if(!file.Open( dlg.GetPathName(),CFile::modeCreate | CFile::modeWrite | CFile::typeText ))
	{
		AfxMessageBox(FILE_OPEN_ERROR);
		return(0);	
	}
		
//	Card /1/
	file.WriteString(CardH1);
	file.WriteString(CardV1);
	str="  Untitled\n";
	file.WriteString(str);

	m_ModelData.ExportPAK_F((CStdioFile *) &file,peo);

	file.Close();

	return(-1);
}

UINT CFemConDlg::ExportPAK_P(CPakExpOpt *peo) 
{
	CStdioFile file;
	CString str, szFilter;

	szFilter="PAK DAT Files  (*.dat)|*.dat||";

	CFileDialog dlg (FALSE,"dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);

	if (dlg.DoModal() != IDOK)
		return(-1);
	
	if(!file.Open( dlg.GetPathName(),CFile::modeCreate | CFile::modeWrite | CFile::typeText ))
	{
		AfxMessageBox(FILE_OPEN_ERROR);
		return(0);	
	}
		
//	Card /1/
	file.WriteString(CardH1);
	file.WriteString(CardV1);
	str="  Untitled\n";
	file.WriteString(str);

	m_ModelData.ExportPAK_P((CStdioFile *) &file,peo);

	file.Close();

	return(-1);
}

UINT CFemConDlg::ExportPAK_PT(CPakExpOpt *peo)
{
	CStdioFile file;
	CString str, szFilter;

	szFilter = "PAKT(P) DAT Files  (*.dat)|*.dat||";

	CFileDialog dlg(FALSE, "dat", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);

	if (dlg.DoModal() != IDOK)
		return(-1);

	if (!file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		AfxMessageBox(FILE_OPEN_ERROR);
		return(0);
	}

	//	Card /1/
	file.WriteString(CardH1);
	file.WriteString(CardV1);
	str = "  Untitled\n";
	file.WriteString(str);

	m_ModelData.ExportPAK_PT((CStdioFile *)&file, peo);

	file.Close();

	return(-1);
}


void CFemConDlg::OnBnClickedButtonExport()
{
	CButton *global;
	BOOL	m_bExGlobal;

	global=(CButton*) GetDlgItem(IDC_CHECK1);
	m_bGlobal = global->GetCheck();
	m_bExGlobal = m_bGlobal;
	switch (m_iSolver)
	{
		case 0: //PAK S
			{
				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				dlgGeneralOptions.dlgMaterialDLG=&dlgMaterialDLG;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;
				CPakExpOpt dlg;
				ExportPAK_S(&dlg);
			}
			break;
		case 1: //PAKT
			{
				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
			//	dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
			//	dlgGeneralOptions.dlgImperfection=&dlgImperfection;
			//	dlgGeneralOptions.dlgMaterialDLG=&dlgMaterialDLG;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;

				CPakExpOpt dlg;
				ExportPAK_T(&dlg);
			}
			break;
		case 2: // PAKF
			{
				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
			//	dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
			//	dlgGeneralOptions.dlgImperfection=&dlgImperfection;
			//	dlgGeneralOptions.dlgMaterialDLG=&dlgMaterialDLG;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;

				CPakExpOpt dlg;
				ExportPAK_F(&dlg);
			}
			break;
		case 3: //PAKP
			{
				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				dlgGeneralOptions.dlgMaterialDLG=&dlgMaterialDLG;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;
				
				CPakExpOpt dlg;
				ExportPAK_P(&dlg);
			}
			break;
		case 4: //PAKPT
		{
					dlgGeneralOptions.dlgTimePeriods = &dlgTimePeriods;
					dlgGeneralOptions.dlgIterMethod = &dlgIterMethod;
					dlgGeneralOptions.dlgImperfection = &dlgImperfection;
					dlgGeneralOptions.dlgMaterialDLG = &dlgMaterialDLG;
					m_ModelData.m_PakOptValue.dlgGeneralOptions = &dlgGeneralOptions;

					CPakExpOpt dlg;
					ExportPAK_PT(&dlg);
		}
			break;
		case 5: //PAKE
			{
				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				dlgGeneralOptions.dlgMaterialDLG=&dlgMaterialDLG;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;

				CPakExpOpt dlg;
				ExportPAK_E(&dlg);
			}
			case 6: //PAKM
			{
				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				dlgGeneralOptions.dlgMaterialDLG=&dlgMaterialDLG;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;

				CPakExpOpt dlg;
				ExportPAK_M(&dlg);
			}
			break;
	}
}

void CFemConDlg::OnBnClickedButtonoptions()
{
	dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
	dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
	dlgGeneralOptions.dlgImperfection=&dlgImperfection;
	m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;

	switch (m_iSolver)
	{
		case 0: //PAK S
			{
				dlgGeneralOptions.DoModal();
			}break;
		case 1:
			{
				dlgPAKTOptions.DoModal();
			}break;
		case 2:
			{
				dlgPAKFOptions.DoModal();
			}break;
	}

	m_sType = _T(dlgGeneralOptions.m_strAnalysisType);
	UpdateData( FALSE );
}

void CFemConDlg::CreateNEU(void)
{
	CLSID       clsid;
	HRESULT hr=CLSIDFromProgID( L"femap.model", &clsid ); 
	IUnknown    FAR *punk;
	if ( hr == S_OK )
	{
		CoInitialize(NULL);
		hr=GetActiveObject(clsid,NULL,&punk);
		if ( hr == S_OK )
		{
			displayFinished = true;
			CComQIPtr<femap::Imodel> pModel;
			pModel=punk;
			CString fullPath = "";
			CString name = pModel->ModelName;
			if(name!=(CString)"")
			{
				// Strip off the path and return just the filename part
				CString tempName = (LPCTSTR) name; // Force CString to make a copy
				PathStripPath(tempName.GetBuffer(0));
				tempName.ReleaseBuffer(-1);
				name=tempName;
				int n = name.Replace((CString)".modfem", (CString)".neu");
				n = name.Replace((CString)".MOD", (CString)".neu");
			}
			else
			{
				name = "Model1.neu";
			}
			//folder location
			CString sAppPath;
			DWORD dwSize = MAX_PATH;
			GetModuleFileName( NULL, sAppPath.GetBuffer( dwSize ), dwSize );
			CString strLocation;
			CString temp = (LPCTSTR) sAppPath; // Force CString to make a copy
			PathRemoveFileSpec(temp.GetBuffer(0));
			temp.ReleaseBuffer(-1);
			strLocation = temp;

			fullPath.Append(strLocation);
			fullPath.Append(_T("\\Work\\"));
			fullPath.Append(name);
			_bstr_t bstrt(fullPath);
			remove(bstrt);
			pModel->feFileWriteNeutral( 0, bstrt, true, true, false, false, false, true, 8, 10.0, 0);
			CString str;
			CString pFileName(fullPath);
			CStdioFile file;
			file.Open(pFileName, CFile::modeRead | CFile::typeText);
			file.ReadString(str);
			file.ReadString(str);
			file.ReadString(str);
			file.ReadString(str);
			m_dExFEMAPversion = atof(str);
	if(m_dExFEMAPversion == 0.0)
	{
		m_dExFEMAPversion = 10.0;
		
	}	
			file.ReadString(str);
			//DeleteContents();
			//OnNewDocument();
			while (!feof(file.m_pStream))
			{
				file.ReadString(str);
				if (str.Left(5)=="   -1")
				{
					file.ReadString(str);
					int nDataBlockType=atoi (str);
					if ((nDataBlockType==FBID_NODES)||(nDataBlockType==FBID_ELEMENTS)||(nDataBlockType==FBID_PROPERTIES)||(nDataBlockType==FBID_LOADS)||
						(nDataBlockType==FBID_CONSTRAINTS) || (nDataBlockType==FBID_MATERIALS) || (nDataBlockType==FBID_FUNCTIONS)||(nDataBlockType==FBID_SETS))
// da li treba dodati ucitavanje kontakt blokova ovde
/*			if ((nDataBlockType==FBID_NODES)||(nDataBlockType==FBID_ELEMENTS)||(nDataBlockType==FBID_PROPERTIES)||(nDataBlockType==FBID_LOADS_OLD)||(nDataBlockType==FBID_LOADS)||
				(nDataBlockType==FBID_CONSTRAINTS_OLD)||(nDataBlockType==FBID_CONSTRAINTS)||(nDataBlockType==FBID_MATERIALS_OLD)||(nDataBlockType==FBID_MATERIALS)||(nDataBlockType==FBID_FUNCTIONS)||
				(nDataBlockType==FBID_CONTACT_SEGMENT)||(nDataBlockType==FBID_CONTACT_PROPERTY)||(nDataBlockType==FBID_CONNECTION)||(nDataBlockType==FBID_SETS))
*/
					{
						m_ModelData.ImportFemap((CStdioFile *) &file, nDataBlockType);
					}
					else
					{
						file.ReadString(str);
						while((str.Left(5)!="   -1")&&(!feof(file.m_pStream)))
						file.ReadString(str);
					}
				}
				else
				{
					file.ReadString(str);
					while((str.Left(5)!="   -1")&&(!feof(file.m_pStream)))
						file.ReadString(str);
				}
			}

			m_sFemapFile = _T(file.GetFileName());
			UpdateData( FALSE );
			file.Close();
			
		}
	}
	
}

void CFemConDlg::ConnectToFemap(void)
{
	CLSID       clsid;
	HRESULT hr=AfxGetClassIDFromString( "femap.model", &clsid);
	IUnknown    FAR *punk;
	if ( hr == S_OK )
	{
		CoInitialize(NULL);
		hr=GetActiveObject(clsid,NULL,&punk);
		if ( hr == S_OK )
		{
			CComQIPtr<femap::Imodel> pModel;
			pModel=punk;
			long rc;
			CWnd* pWnd = FindWindow(NULL, "FemCon");
			HWND hWnd = pWnd->GetSafeHwnd();
			int x;
			x=(int)hWnd;
			rc = pModel->feAppRegisterAddInPane(true, x, x, false, false, 3, 2);
			
			m_bCanAnalyse = true;
			CButton *analysis;
			analysis=(CButton*) GetDlgItem(IDC_BUTTON_ANALYSE);
			analysis->EnableWindow(true);
		}
	} 
}

void CFemConDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CFemConDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CFemConDlg::OnBnClickedButtonFinish()
{
	if (displayFinished==true)
	{
		displayFinished = false;
		FinishDlg finDlg;
		finDlg.DoModal();
	}
	OnCancel();
}

void CFemConDlg::OnCbnSelchangeComboSolver()
{
	CComboBox *box;
	box=(CComboBox*) GetDlgItem(IDC_COMBO_SOLVER);
	CButton *analysis;
	analysis=(CButton*) GetDlgItem(IDC_BUTTON_ANALYSE);
			
	m_iSolver=box->GetCurSel();	
	if (m_bCanAnalyse == true)
	{
		analysis->EnableWindow(true);
	}
	else
	{
		analysis->EnableWindow(false);
	}
}

void CFemConDlg::OnBnClickedButtonAnalyse()
{
	CButton *global;
	BOOL m_bExGlobal;

	global=(CButton*) GetDlgItem(IDC_CHECK1);
	m_bGlobal = global->GetCheck();
	m_bExGlobal = m_bGlobal;
	
	//_spawnl( _P_WAIT, "c:\\PAKNOVI\\paks.bat", "c:\\PAKNOVI\\paks.bat", NULL);
	CString sDatPath;
	DWORD dwSize = MAX_PATH;
	GetModuleFileName( NULL, sDatPath.GetBuffer( dwSize ), dwSize );

switch (m_iSolver)
	{
		case 0: //PAK S
			{

				CString sPAKPath;
				sPAKPath=sDatPath;
				CString tempPAKName = (LPCTSTR) sPAKPath; // Force CString to make a copy
				tempPAKName.ReleaseBuffer(-1);
				sPAKPath=tempPAKName;
				int k = sPAKPath.Replace((CString)"FemCon.exe", (CString)"paks.exe");
				CString pakBatch;
				pakBatch = sPAKPath;
				CString tempBatch = (LPCSTR) pakBatch;
				tempBatch.ReleaseBuffer(-1);
				pakBatch = tempBatch;
				int	t = pakBatch.Replace("paks.exe","paks.bat");
				_spawnl( _P_WAIT, pakBatch, pakBatch, NULL);

				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;
				
				CStdioFile file;
				CString str;

				
				CString fileName;
				fileName=m_sFemapFile;
				CString tempFileName = (LPCTSTR) fileName; // Force CString to make a copy
				tempFileName.ReleaseBuffer(-1);
				fileName=tempFileName;
				int m = fileName.Replace((CString)".neu", (CString)"PakOut.dat"); // PI=PAK Input

				CString tempName = (LPCTSTR) sDatPath; // Force CString to make a copy
				tempName.ReleaseBuffer(-1);
				sDatPath=tempName;
				int n = sDatPath.Replace((CString)"FemCon.exe", fileName);

				CString sDatWorkPath;
				sDatWorkPath = sDatPath;
				CString tempWorkDat = (LPCTSTR) sDatWorkPath;
				tempWorkDat.ReleaseBuffer(-1);
				sDatWorkPath = tempWorkDat;
				int g = sDatWorkPath.Replace(fileName, ("\\Work\\" + fileName));

				file.Open( (LPCSTR)sDatPath ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
			
				CPakExpOpt peo;
			//	Card /1/
				file.WriteString(CardH1);
				file.WriteString(CardV1);
				str="  Untitled\n";
				file.WriteString(str);
				m_ModelData.ExportPAK_S((CStdioFile *) &file,&peo);
				file.Close();

				CString sNEUPath;
				sNEUPath = sDatPath;
				CString tempNEUName = (LPCTSTR) sNEUPath; // Force CString to make a copy
				tempNEUName.ReleaseBuffer(-1);
				sNEUPath=tempNEUName;
				int tt = sNEUPath.Replace((CString)".dat", (CString)".neu");
				//m_sNEUPath=sNEUPath	;
				
				//create input for batch file (Files)
				CString pakResult;
				pakResult = sPAKPath;
				CString tempPakResult = (LPCTSTR) pakResult;
				tempPakResult.ReleaseBuffer(-1);
				pakResult = tempPakResult;
				t = pakResult.Replace((CString)"paks.exe","Files" );
				file.Open( (LPCSTR)pakResult ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString(fileName +"\n");
				CString otherFiles;
				otherFiles = fileName;
				CString tempOthers = (LPCTSTR) otherFiles;
				tempOthers.ReleaseBuffer(-1);
				otherFiles = tempOthers;
				t= otherFiles.Replace(".dat",".lst");
				CString lstFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".lst",".unv");
				CString unvFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".unv",".neu");
				file.WriteString(otherFiles);
				file.Close();
				//created input for batch file (Files)


				
				file.Open( (LPCSTR)pakBatch ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString( "del " + lstFile +"\n");
				file.WriteString( "del " + unvFile +"\n");
				file.WriteString( "paks.exe < Files \n");
				file.WriteString( "rem del " + fileName +"\n");
				file.WriteString( "rem del " + lstFile +"\n");

				CString zsFile;
				zsFile = fileName;
				CString tempZS = (LPCSTR) zsFile;
				tempZS.ReleaseBuffer(-1);
				zsFile = tempZS;
				t= zsFile.Replace(".dat", ".zs");
				file.WriteString( "del " + zsFile +"\n");
				file.WriteString( "del z*.");
				file.Close();

				Sleep(200);
				_spawnl(_P_WAIT, pakBatch, pakBatch, NULL);


				remove(sDatWorkPath);
				if ( rename(sDatPath, sDatWorkPath) )
				perror( NULL );


				CString sLstPath;
				sLstPath = sDatPath;
				CString tempLst = (LPCTSTR) sLstPath;
				tempLst.ReleaseBuffer(-1);
				sLstPath = tempLst;
				int f = sLstPath.Replace(".dat", ".lst");
				CString sLstWorkPath;
				sLstWorkPath = sDatWorkPath;
				CString tempWorkLst = (LPCTSTR) sLstWorkPath;
				tempWorkLst.ReleaseBuffer(-1);
				sLstWorkPath = tempWorkLst;
				int r = sLstWorkPath.Replace(".dat", ".lst");
				remove(sLstWorkPath);
				if ( rename(sLstPath, sLstWorkPath) )
				perror( NULL );

				CString sUnvPath;
				sUnvPath = sDatPath;
				CString tempUnv = (LPCTSTR) sUnvPath;
				tempUnv.ReleaseBuffer(-1);
				sUnvPath = tempUnv;
				int u = sUnvPath.Replace(".dat", ".unv");
				CString sUnvWorkPath;
				sUnvWorkPath = sDatWorkPath;
				CString tempWorkUnv = (LPCTSTR) sUnvWorkPath;
				tempWorkUnv.ReleaseBuffer(-1);
				sUnvWorkPath = tempWorkUnv;
				int v = sUnvWorkPath.Replace(".dat", ".unv");
				remove(sUnvWorkPath);
				if ( rename(sUnvPath, sUnvWorkPath) )
				perror( NULL );

				CString sNeuPath;
				sNeuPath = sDatPath;
				CString tempNeu = (LPCTSTR) sNeuPath;
				tempNeu.ReleaseBuffer(-1);
				sNeuPath = tempNeu;
				int ne = sNeuPath.Replace(".dat", ".neu");
				CString sNeuWorkPath;
				sNeuWorkPath = sDatWorkPath;
				CString tempWorkNeu = (LPCTSTR) sNeuWorkPath;
				tempWorkNeu.ReleaseBuffer(-1);
				sNeuWorkPath = tempWorkNeu;
				int me = sNeuWorkPath.Replace(".dat", ".neu");
				remove(sNeuWorkPath);
				if ( rename(sNeuPath, sNeuWorkPath) )
				perror( NULL );
				m_sNEUPath = sNeuWorkPath;

				AnalysedDlg analysedDlg;
				analysedDlg.DoModal();
			}
			break;
		case 1: //PAK T
			{
				CString tPAKPath;
				tPAKPath=sDatPath;
				CString tempPAKName = (LPCTSTR) tPAKPath; // Force CString to make a copy
				tempPAKName.ReleaseBuffer(-1);
				tPAKPath=tempPAKName;
				int k = tPAKPath.Replace((CString)"FemCon.exe", (CString)"pakt.exe");
				CString pakBatch;
				pakBatch = tPAKPath;
				CString tempBatch = (LPCSTR) pakBatch;
				tempBatch.ReleaseBuffer(-1);
				pakBatch = tempBatch;
				int	t = pakBatch.Replace("pakt.exe","pakt.bat");
				_spawnl( _P_WAIT, pakBatch, pakBatch, NULL);

				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;
				
				CStdioFile file;
				CString str;

				
				CString fileName;
				fileName=m_sFemapFile;
				CString tempFileName = (LPCTSTR) fileName; // Force CString to make a copy
				tempFileName.ReleaseBuffer(-1);
				fileName=tempFileName;
				int m = fileName.Replace((CString)".neu", (CString)"PakOut.dat"); // PI=PAK Input

				CString tempName = (LPCTSTR) sDatPath; // Force CString to make a copy
				tempName.ReleaseBuffer(-1);
				sDatPath=tempName;
				int n = sDatPath.Replace((CString)"FemCon.exe", fileName);

				CString sDatWorkPath;
				sDatWorkPath = sDatPath;
				CString tempWorkDat = (LPCTSTR) sDatWorkPath;
				tempWorkDat.ReleaseBuffer(-1);
				sDatWorkPath = tempWorkDat;
				int g = sDatWorkPath.Replace(fileName, ("\\Work\\" + fileName));

				file.Open( (LPCSTR)sDatPath ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
			
				CPakExpOpt peo;
			//	Card /1/
				file.WriteString(CardH1);
				file.WriteString(CardV1);
				str="  Untitled\n";
				file.WriteString(str);
				m_ModelData.ExportPAK_T((CStdioFile *) &file,&peo);
				file.Close();
//ovde
				CString sNEUPath;
				sNEUPath = sDatPath;
				CString tempNEUName = (LPCTSTR) sNEUPath; // Force CString to make a copy
				tempNEUName.ReleaseBuffer(-1);
				sNEUPath=tempNEUName;
				int ttt = sNEUPath.Replace((CString)".dat", (CString)".NEU");
				//m_sNEUPath=sNEUPath	;
				
				//create input for batch file (Files)
				CString pakResult;
				pakResult = tPAKPath;
				CString tempPakResult = (LPCTSTR) pakResult;
				tempPakResult.ReleaseBuffer(-1);
				pakResult = tempPakResult;
				t = pakResult.Replace((CString)"pakt.exe","Files" );
				file.Open( (LPCSTR)pakResult ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString(fileName +"\n");
				CString otherFiles;
				otherFiles = fileName;
				CString tempOthers = (LPCTSTR) otherFiles;
				tempOthers.ReleaseBuffer(-1);
				otherFiles = tempOthers;
				t= otherFiles.Replace(".dat",".lst");
				CString lstFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".lst",".unv");
				CString unvFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".unv",".neu");
				file.WriteString(otherFiles);
				file.Close();
				//created input for batch file (Files)


				
				file.Open( (LPCSTR)pakBatch ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString( "del " + lstFile +"\n");
				file.WriteString( "del " + unvFile +"\n");
				file.WriteString( "pakt.exe < Files \n");
				file.WriteString( "rem del " + fileName +"\n");
				file.WriteString( "rem del " + lstFile +"\n");

				CString zsFile;
				zsFile = fileName;
				CString tempZS = (LPCSTR) zsFile;
				tempZS.ReleaseBuffer(-1);
				zsFile = tempZS;
				t= zsFile.Replace(".dat", ".zs");
				file.WriteString( "del " + zsFile +"\n");
				file.WriteString( "del z*.");
				file.Close();

				Sleep(200);
				_spawnl(_P_WAIT, pakBatch, pakBatch, NULL);


				remove(sDatWorkPath);
				if ( rename(sDatPath, sDatWorkPath) )
				perror( NULL );


				CString sLstPath;
				sLstPath = sDatPath;
				CString tempLst = (LPCTSTR) sLstPath;
				tempLst.ReleaseBuffer(-1);
				sLstPath = tempLst;
				int f = sLstPath.Replace(".dat", ".lst");
				CString sLstWorkPath;
				sLstWorkPath = sDatWorkPath;
				CString tempWorkLst = (LPCTSTR) sLstWorkPath;
				tempWorkLst.ReleaseBuffer(-1);
				sLstWorkPath = tempWorkLst;
				int r = sLstWorkPath.Replace(".dat", ".lst");
				remove(sLstWorkPath);
				if ( rename(sLstPath, sLstWorkPath) )
				perror( NULL );

				CString sUnvPath;
				sUnvPath = sDatPath;
				CString tempUnv = (LPCTSTR) sUnvPath;
				tempUnv.ReleaseBuffer(-1);
				sUnvPath = tempUnv;
				int u = sUnvPath.Replace(".dat", ".unv");
				CString sUnvWorkPath;
				sUnvWorkPath = sDatWorkPath;
				CString tempWorkUnv = (LPCTSTR) sUnvWorkPath;
				tempWorkUnv.ReleaseBuffer(-1);
				sUnvWorkPath = tempWorkUnv;
				int v = sUnvWorkPath.Replace(".dat", ".unv");
				remove(sUnvWorkPath);
				if ( rename(sUnvPath, sUnvWorkPath) )
				perror( NULL );

				CString sNeuPath;
				sNeuPath = sDatPath;
				CString tempNeu = (LPCTSTR) sNeuPath;
				tempNeu.ReleaseBuffer(-1);
				sNeuPath = tempNeu;
				int ne = sNeuPath.Replace(".dat", ".neu");
				CString sNeuWorkPath;
				sNeuWorkPath = sDatWorkPath;
				CString tempWorkNeu = (LPCTSTR) sNeuWorkPath;
				tempWorkNeu.ReleaseBuffer(-1);
				sNeuWorkPath = tempWorkNeu;
				int me = sNeuWorkPath.Replace(".dat", ".neu");
				remove(sNeuWorkPath);
				if ( rename(sNeuPath, sNeuWorkPath) )
				perror( NULL );
				m_sNEUPath = sNeuWorkPath;

				AnalysedDlg analysedDlg;
				analysedDlg.DoModal();
			}
			break;
		case 2: //PAK F
			{
				CString fPAKPath;
				fPAKPath=sDatPath;
				CString tempPAKName = (LPCTSTR) fPAKPath; // Force CString to make a copy
				tempPAKName.ReleaseBuffer(-1);
				fPAKPath=tempPAKName;
				int k = fPAKPath.Replace((CString)"FemCon.exe", (CString)"pakf.exe");
				CString pakBatch;
				pakBatch = fPAKPath;
				CString tempBatch = (LPCSTR) pakBatch;
				tempBatch.ReleaseBuffer(-1);
				pakBatch = tempBatch;
				int	t = pakBatch.Replace("pakf.exe","pakf.bat");
				_spawnl( _P_WAIT, pakBatch, pakBatch, NULL);

				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;
				
				CStdioFile file;
				CString str;

				
				CString fileName;
				fileName=m_sFemapFile;
				CString tempFileName = (LPCTSTR) fileName; // Force CString to make a copy
				tempFileName.ReleaseBuffer(-1);
				fileName=tempFileName;
				int m = fileName.Replace((CString)".neu", (CString)"PakOut.dat"); // PI=PAK Input

				CString tempName = (LPCTSTR) sDatPath; // Force CString to make a copy
				tempName.ReleaseBuffer(-1);
				sDatPath=tempName;
				int n = sDatPath.Replace((CString)"FemCon.exe", fileName);

				CString sDatWorkPath;
				sDatWorkPath = sDatPath;
				CString tempWorkDat = (LPCTSTR) sDatWorkPath;
				tempWorkDat.ReleaseBuffer(-1);
				sDatWorkPath = tempWorkDat;
				int g = sDatWorkPath.Replace(fileName, ("\\Work\\" + fileName));

				file.Open( (LPCSTR)sDatPath ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
			
				CPakExpOpt peo;
			//	Card /1/
				file.WriteString(CardH1);
				file.WriteString(CardV1);
				str="  Untitled\n";
				file.WriteString(str);
				m_ModelData.ExportPAK_F((CStdioFile *) &file,&peo);
				file.Close();
//ovde
				CString sNEUPath;
				sNEUPath = sDatPath;
				CString tempNEUName = (LPCTSTR) sNEUPath; // Force CString to make a copy
				tempNEUName.ReleaseBuffer(-1);
				sNEUPath=tempNEUName;
				int ttt = sNEUPath.Replace((CString)".dat", (CString)".NEU");
				//m_sNEUPath=sNEUPath	;
				
				//create input for batch file (Files)
				CString pakResult;
				pakResult = fPAKPath;
				CString tempPakResult = (LPCTSTR) pakResult;
				tempPakResult.ReleaseBuffer(-1);
				pakResult = tempPakResult;
				t = pakResult.Replace((CString)"pakf.exe","Files" );
				file.Open( (LPCSTR)pakResult ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString(fileName +"\n");
				CString otherFiles;
				otherFiles = fileName;
				CString tempOthers = (LPCTSTR) otherFiles;
				tempOthers.ReleaseBuffer(-1);
				otherFiles = tempOthers;
				t= otherFiles.Replace(".dat",".lst");
				CString lstFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".lst",".unv");
				CString unvFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".unv",".neu");
				file.WriteString(otherFiles);
				file.Close();
				//created input for batch file (Files)


				
				file.Open( (LPCSTR)pakBatch ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString( "del " + lstFile +"\n");
				file.WriteString( "del " + unvFile +"\n");
				file.WriteString( "pakt.exe < Files \n");
				file.WriteString( "rem del " + fileName +"\n");
				file.WriteString( "rem del " + lstFile +"\n");

				CString zsFile;
				zsFile = fileName;
				CString tempZS = (LPCSTR) zsFile;
				tempZS.ReleaseBuffer(-1);
				zsFile = tempZS;
				t= zsFile.Replace(".dat", ".zs");
				file.WriteString( "del " + zsFile +"\n");
				file.WriteString( "del z*.");
				file.Close();

				Sleep(200);
				_spawnl(_P_WAIT, pakBatch, pakBatch, NULL);


				remove(sDatWorkPath);
				if ( rename(sDatPath, sDatWorkPath) )
				perror( NULL );


				CString sLstPath;
				sLstPath = sDatPath;
				CString tempLst = (LPCTSTR) sLstPath;
				tempLst.ReleaseBuffer(-1);
				sLstPath = tempLst;
				int f = sLstPath.Replace(".dat", ".lst");
				CString sLstWorkPath;
				sLstWorkPath = sDatWorkPath;
				CString tempWorkLst = (LPCTSTR) sLstWorkPath;
				tempWorkLst.ReleaseBuffer(-1);
				sLstWorkPath = tempWorkLst;
				int r = sLstWorkPath.Replace(".dat", ".lst");
				remove(sLstWorkPath);
				if ( rename(sLstPath, sLstWorkPath) )
				perror( NULL );

				CString sUnvPath;
				sUnvPath = sDatPath;
				CString tempUnv = (LPCTSTR) sUnvPath;
				tempUnv.ReleaseBuffer(-1);
				sUnvPath = tempUnv;
				int u = sUnvPath.Replace(".dat", ".unv");
				CString sUnvWorkPath;
				sUnvWorkPath = sDatWorkPath;
				CString tempWorkUnv = (LPCTSTR) sUnvWorkPath;
				tempWorkUnv.ReleaseBuffer(-1);
				sUnvWorkPath = tempWorkUnv;
				int v = sUnvWorkPath.Replace(".dat", ".unv");
				remove(sUnvWorkPath);
				if ( rename(sUnvPath, sUnvWorkPath) )
				perror( NULL );

				CString sNeuPath;
				sNeuPath = sDatPath;
				CString tempNeu = (LPCTSTR) sNeuPath;
				tempNeu.ReleaseBuffer(-1);
				sNeuPath = tempNeu;
				int ne = sNeuPath.Replace(".dat", ".neu");
				CString sNeuWorkPath;
				sNeuWorkPath = sDatWorkPath;
				CString tempWorkNeu = (LPCTSTR) sNeuWorkPath;
				tempWorkNeu.ReleaseBuffer(-1);
				sNeuWorkPath = tempWorkNeu;
				int me = sNeuWorkPath.Replace(".dat", ".neu");
				remove(sNeuWorkPath);
				if ( rename(sNeuPath, sNeuWorkPath) )
				perror( NULL );
				m_sNEUPath = sNeuWorkPath;

				AnalysedDlg analysedDlg;
				analysedDlg.DoModal();
				
				
			}
			break;
		case 3: //PAK P
			{
				CString tPAKPath;
				tPAKPath=sDatPath;
				CString tempPAKName = (LPCTSTR) tPAKPath; // Force CString to make a copy
				tempPAKName.ReleaseBuffer(-1);
				tPAKPath=tempPAKName;
				int k = tPAKPath.Replace((CString)"FemCon.exe", (CString)"pakv.exe");
				CString pakBatch;
				pakBatch = tPAKPath;
				CString tempBatch = (LPCSTR) pakBatch;
				tempBatch.ReleaseBuffer(-1);
				pakBatch = tempBatch;
				int	t = pakBatch.Replace("pakv.exe","pakv.bat");
				_spawnl( _P_WAIT, pakBatch, pakBatch, NULL);

				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;
				
				CStdioFile file;
				CString str;

				
				CString fileName;
				fileName=m_sFemapFile;
				CString tempFileName = (LPCTSTR) fileName; // Force CString to make a copy
				tempFileName.ReleaseBuffer(-1);
				fileName=tempFileName;
				int m = fileName.Replace((CString)".neu", (CString)"PakOut.dat"); // PI=PAK Input

				CString tempName = (LPCTSTR) sDatPath; // Force CString to make a copy
				tempName.ReleaseBuffer(-1);
				sDatPath=tempName;
				int n = sDatPath.Replace((CString)"FemCon.exe", fileName);

				CString sDatWorkPath;
				sDatWorkPath = sDatPath;
				CString tempWorkDat = (LPCTSTR) sDatWorkPath;
				tempWorkDat.ReleaseBuffer(-1);
				sDatWorkPath = tempWorkDat;
				int g = sDatWorkPath.Replace(fileName, ("\\Work\\" + fileName));

				file.Open( (LPCSTR)sDatPath ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
			
				CPakExpOpt peo;
			//	Card /1/
				file.WriteString(CardH1);
				file.WriteString(CardV1);
				str="  Untitled\n";
				file.WriteString(str);
				m_ModelData.ExportPAK_P((CStdioFile *) &file,&peo);
				file.Close();
//ovde
				CString sNEUPath;
				sNEUPath = sDatPath;
				CString tempNEUName = (LPCTSTR) sNEUPath; // Force CString to make a copy
				tempNEUName.ReleaseBuffer(-1);
				sNEUPath=tempNEUName;
				int ttt = sNEUPath.Replace((CString)".dat", (CString)".NEU");
				//m_sNEUPath=sNEUPath	;
				
				//create input for batch file (Files)
				CString pakResult;
				pakResult = tPAKPath;
				CString tempPakResult = (LPCTSTR) pakResult;
				tempPakResult.ReleaseBuffer(-1);
				pakResult = tempPakResult;
				t = pakResult.Replace((CString)"pakv.exe","Files" );
				file.Open( (LPCSTR)pakResult ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString(fileName +"\n");
				CString otherFiles;
				otherFiles = fileName;
				CString tempOthers = (LPCTSTR) otherFiles;
				tempOthers.ReleaseBuffer(-1);
				otherFiles = tempOthers;
				t= otherFiles.Replace(".dat",".lst");
				CString lstFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".lst",".unv");
				CString unvFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".unv",".neu");
				file.WriteString(otherFiles);
				file.Close();
				//created input for batch file (Files)


				
				file.Open( (LPCSTR)pakBatch ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString( "del " + lstFile +"\n");
				file.WriteString( "del " + unvFile +"\n");
				file.WriteString( "pakv.exe < Files \n");
				file.WriteString( "rem del " + fileName +"\n");
				file.WriteString( "rem del " + lstFile +"\n");

				CString zsFile;
				zsFile = fileName;
				CString tempZS = (LPCSTR) zsFile;
				tempZS.ReleaseBuffer(-1);
				zsFile = tempZS;
				t= zsFile.Replace(".dat", ".zs");
				file.WriteString( "del " + zsFile +"\n");
				file.WriteString( "del z*.");
				file.Close();

				Sleep(200);
				_spawnl(_P_WAIT, pakBatch, pakBatch, NULL);


				remove(sDatWorkPath);
				if ( rename(sDatPath, sDatWorkPath) )
				perror( NULL );


				CString sLstPath;
				sLstPath = sDatPath;
				CString tempLst = (LPCTSTR) sLstPath;
				tempLst.ReleaseBuffer(-1);
				sLstPath = tempLst;
				int f = sLstPath.Replace(".dat", ".lst");
				CString sLstWorkPath;
				sLstWorkPath = sDatWorkPath;
				CString tempWorkLst = (LPCTSTR) sLstWorkPath;
				tempWorkLst.ReleaseBuffer(-1);
				sLstWorkPath = tempWorkLst;
				int r = sLstWorkPath.Replace(".dat", ".lst");
				remove(sLstWorkPath);
				if ( rename(sLstPath, sLstWorkPath) )
				perror( NULL );

				CString sUnvPath;
				sUnvPath = sDatPath;
				CString tempUnv = (LPCTSTR) sUnvPath;
				tempUnv.ReleaseBuffer(-1);
				sUnvPath = tempUnv;
				int u = sUnvPath.Replace(".dat", ".unv");
				CString sUnvWorkPath;
				sUnvWorkPath = sDatWorkPath;
				CString tempWorkUnv = (LPCTSTR) sUnvWorkPath;
				tempWorkUnv.ReleaseBuffer(-1);
				sUnvWorkPath = tempWorkUnv;
				int v = sUnvWorkPath.Replace(".dat", ".unv");
				remove(sUnvWorkPath);
				if ( rename(sUnvPath, sUnvWorkPath) )
				perror( NULL );

				CString sNeuPath;
				sNeuPath = sDatPath;
				CString tempNeu = (LPCTSTR) sNeuPath;
				tempNeu.ReleaseBuffer(-1);
				sNeuPath = tempNeu;
				int ne = sNeuPath.Replace(".dat", ".neu");
				CString sNeuWorkPath;
				sNeuWorkPath = sDatWorkPath;
				CString tempWorkNeu = (LPCTSTR) sNeuWorkPath;
				tempWorkNeu.ReleaseBuffer(-1);
				sNeuWorkPath = tempWorkNeu;
				int me = sNeuWorkPath.Replace(".dat", ".neu");
				remove(sNeuWorkPath);
				if ( rename(sNeuPath, sNeuWorkPath) )
				perror( NULL );
				m_sNEUPath = sNeuWorkPath;

				AnalysedDlg analysedDlg;
				analysedDlg.DoModal();
			}
			break;
		case 4: //PAK T(P)
		{
					CString tPAKPath;
					tPAKPath = sDatPath;
					CString tempPAKName = (LPCTSTR)tPAKPath; // Force CString to make a copy
					tempPAKName.ReleaseBuffer(-1);
					tPAKPath = tempPAKName;
					int k = tPAKPath.Replace((CString)"FemCon.exe", (CString)"pakv.exe");
					CString pakBatch;
					pakBatch = tPAKPath;
					CString tempBatch = (LPCSTR)pakBatch;
					tempBatch.ReleaseBuffer(-1);
					pakBatch = tempBatch;
					int	t = pakBatch.Replace("pakv.exe", "pakv.bat");
					_spawnl(_P_WAIT, pakBatch, pakBatch, NULL);

					dlgGeneralOptions.dlgTimePeriods = &dlgTimePeriods;
					dlgGeneralOptions.dlgIterMethod = &dlgIterMethod;
					dlgGeneralOptions.dlgImperfection = &dlgImperfection;
					m_ModelData.m_PakOptValue.dlgGeneralOptions = &dlgGeneralOptions;

					CStdioFile file;
					CString str;


					CString fileName;
					fileName = m_sFemapFile;
					CString tempFileName = (LPCTSTR)fileName; // Force CString to make a copy
					tempFileName.ReleaseBuffer(-1);
					fileName = tempFileName;
					int m = fileName.Replace((CString)".neu", (CString)"PakOut.dat"); // PI=PAK Input

					CString tempName = (LPCTSTR)sDatPath; // Force CString to make a copy
					tempName.ReleaseBuffer(-1);
					sDatPath = tempName;
					int n = sDatPath.Replace((CString)"FemCon.exe", fileName);

					CString sDatWorkPath;
					sDatWorkPath = sDatPath;
					CString tempWorkDat = (LPCTSTR)sDatWorkPath;
					tempWorkDat.ReleaseBuffer(-1);
					sDatWorkPath = tempWorkDat;
					int g = sDatWorkPath.Replace(fileName, ("\\Work\\" + fileName));

					file.Open((LPCSTR)sDatPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);

					CPakExpOpt peo;
					//	Card /1/
					file.WriteString(CardH1);
					file.WriteString(CardV1);
					str = "  Untitled\n";
					file.WriteString(str);
					m_ModelData.ExportPAK_PT((CStdioFile *)&file, &peo);
					file.Close();
					//ovde
					CString sNEUPath;
					sNEUPath = sDatPath;
					CString tempNEUName = (LPCTSTR)sNEUPath; // Force CString to make a copy
					tempNEUName.ReleaseBuffer(-1);
					sNEUPath = tempNEUName;
					int ttt = sNEUPath.Replace((CString)".dat", (CString)".NEU");
					//m_sNEUPath=sNEUPath	;

					//create input for batch file (Files)
					CString pakResult;
					pakResult = tPAKPath;
					CString tempPakResult = (LPCTSTR)pakResult;
					tempPakResult.ReleaseBuffer(-1);
					pakResult = tempPakResult;
					t = pakResult.Replace((CString)"pakv.exe", "Files");
					file.Open((LPCSTR)pakResult, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
					file.WriteString(fileName + "\n");
					CString otherFiles;
					otherFiles = fileName;
					CString tempOthers = (LPCTSTR)otherFiles;
					tempOthers.ReleaseBuffer(-1);
					otherFiles = tempOthers;
					t = otherFiles.Replace(".dat", ".lst");
					CString lstFile = otherFiles;
					file.WriteString(otherFiles + "\n");
					t = otherFiles.Replace(".lst", ".unv");
					CString unvFile = otherFiles;
					file.WriteString(otherFiles + "\n");
					t = otherFiles.Replace(".unv", ".neu");
					file.WriteString(otherFiles);
					file.Close();
					//created input for batch file (Files)



					file.Open((LPCSTR)pakBatch, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
					file.WriteString("del " + lstFile + "\n");
					file.WriteString("del " + unvFile + "\n");
					file.WriteString("pakv.exe < Files \n");
					file.WriteString("rem del " + fileName + "\n");
					file.WriteString("rem del " + lstFile + "\n");

					CString zsFile;
					zsFile = fileName;
					CString tempZS = (LPCSTR)zsFile;
					tempZS.ReleaseBuffer(-1);
					zsFile = tempZS;
					t = zsFile.Replace(".dat", ".zs");
					file.WriteString("del " + zsFile + "\n");
					file.WriteString("del z*.");
					file.Close();

					Sleep(200);
					_spawnl(_P_WAIT, pakBatch, pakBatch, NULL);


					remove(sDatWorkPath);
					if (rename(sDatPath, sDatWorkPath))
						perror(NULL);


					CString sLstPath;
					sLstPath = sDatPath;
					CString tempLst = (LPCTSTR)sLstPath;
					tempLst.ReleaseBuffer(-1);
					sLstPath = tempLst;
					int f = sLstPath.Replace(".dat", ".lst");
					CString sLstWorkPath;
					sLstWorkPath = sDatWorkPath;
					CString tempWorkLst = (LPCTSTR)sLstWorkPath;
					tempWorkLst.ReleaseBuffer(-1);
					sLstWorkPath = tempWorkLst;
					int r = sLstWorkPath.Replace(".dat", ".lst");
					remove(sLstWorkPath);
					if (rename(sLstPath, sLstWorkPath))
						perror(NULL);

					CString sUnvPath;
					sUnvPath = sDatPath;
					CString tempUnv = (LPCTSTR)sUnvPath;
					tempUnv.ReleaseBuffer(-1);
					sUnvPath = tempUnv;
					int u = sUnvPath.Replace(".dat", ".unv");
					CString sUnvWorkPath;
					sUnvWorkPath = sDatWorkPath;
					CString tempWorkUnv = (LPCTSTR)sUnvWorkPath;
					tempWorkUnv.ReleaseBuffer(-1);
					sUnvWorkPath = tempWorkUnv;
					int v = sUnvWorkPath.Replace(".dat", ".unv");
					remove(sUnvWorkPath);
					if (rename(sUnvPath, sUnvWorkPath))
						perror(NULL);

					CString sNeuPath;
					sNeuPath = sDatPath;
					CString tempNeu = (LPCTSTR)sNeuPath;
					tempNeu.ReleaseBuffer(-1);
					sNeuPath = tempNeu;
					int ne = sNeuPath.Replace(".dat", ".neu");
					CString sNeuWorkPath;
					sNeuWorkPath = sDatWorkPath;
					CString tempWorkNeu = (LPCTSTR)sNeuWorkPath;
					tempWorkNeu.ReleaseBuffer(-1);
					sNeuWorkPath = tempWorkNeu;
					int me = sNeuWorkPath.Replace(".dat", ".neu");
					remove(sNeuWorkPath);
					if (rename(sNeuPath, sNeuWorkPath))
						perror(NULL);
					m_sNEUPath = sNeuWorkPath;

					AnalysedDlg analysedDlg;
					analysedDlg.DoModal();
		}
			break;
		case 5: //PAK E
			{
				CString tPAKPath;
				tPAKPath=sDatPath;
				CString tempPAKName = (LPCTSTR) tPAKPath; // Force CString to make a copy
				tempPAKName.ReleaseBuffer(-1);
				tPAKPath=tempPAKName;
				int k = tPAKPath.Replace((CString)"FemCon.exe", (CString)"pake.exe");
				CString pakBatch;
				pakBatch = tPAKPath;
				CString tempBatch = (LPCSTR) pakBatch;
				tempBatch.ReleaseBuffer(-1);
				pakBatch = tempBatch;
				int	t = pakBatch.Replace("pake.exe","pake.bat");
				_spawnl( _P_WAIT, pakBatch, pakBatch, NULL);

				dlgGeneralOptions.dlgTimePeriods=&dlgTimePeriods;
				dlgGeneralOptions.dlgIterMethod=&dlgIterMethod;
				dlgGeneralOptions.dlgImperfection=&dlgImperfection;
				m_ModelData.m_PakOptValue.dlgGeneralOptions=&dlgGeneralOptions;
				
				CStdioFile file;
				CString str;

				
				CString fileName;
				fileName=m_sFemapFile;
				CString tempFileName = (LPCTSTR) fileName; // Force CString to make a copy
				tempFileName.ReleaseBuffer(-1);
				fileName=tempFileName;
				int m = fileName.Replace((CString)".neu", (CString)"PakOut.dat"); // PI=PAK Input

				CString tempName = (LPCTSTR) sDatPath; // Force CString to make a copy
				tempName.ReleaseBuffer(-1);
				sDatPath=tempName;
				int n = sDatPath.Replace((CString)"FemCon.exe", fileName);

				CString sDatWorkPath;
				sDatWorkPath = sDatPath;
				CString tempWorkDat = (LPCTSTR) sDatWorkPath;
				tempWorkDat.ReleaseBuffer(-1);
				sDatWorkPath = tempWorkDat;
				int g = sDatWorkPath.Replace(fileName, ("\\Work\\" + fileName));

				file.Open( (LPCSTR)sDatPath ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
			
				CPakExpOpt peo;
			//	Card /1/
				file.WriteString(CardH1);
				file.WriteString(CardV1);
				str="  Untitled\n";
				file.WriteString(str);
				m_ModelData.ExportPAK_T((CStdioFile *) &file,&peo);
				file.Close();
//ovde
				CString sNEUPath;
				sNEUPath = sDatPath;
				CString tempNEUName = (LPCTSTR) sNEUPath; // Force CString to make a copy
				tempNEUName.ReleaseBuffer(-1);
				sNEUPath=tempNEUName;
				int ttt = sNEUPath.Replace((CString)".dat", (CString)".NEU");
				//m_sNEUPath=sNEUPath	;
				
				//create input for batch file (Files)
				CString pakResult;
				pakResult = tPAKPath;
				CString tempPakResult = (LPCTSTR) pakResult;
				tempPakResult.ReleaseBuffer(-1);
				pakResult = tempPakResult;
				t = pakResult.Replace((CString)"pake.exe","Files" );
				file.Open( (LPCSTR)pakResult ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString(fileName +"\n");
				CString otherFiles;
				otherFiles = fileName;
				CString tempOthers = (LPCTSTR) otherFiles;
				tempOthers.ReleaseBuffer(-1);
				otherFiles = tempOthers;
				t= otherFiles.Replace(".dat",".lst");
				CString lstFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".lst",".unv");
				CString unvFile = otherFiles;
				file.WriteString(otherFiles +"\n");
				t= otherFiles.Replace(".unv",".neu");
				file.WriteString(otherFiles);
				file.Close();
				//created input for batch file (Files)


				
				file.Open( (LPCSTR)pakBatch ,CFile::modeCreate | CFile::modeWrite | CFile::typeText );
				file.WriteString( "del " + lstFile +"\n");
				file.WriteString( "del " + unvFile +"\n");
				file.WriteString( "pake.exe < Files \n");
				file.WriteString( "rem del " + fileName +"\n");
				file.WriteString( "rem del " + lstFile +"\n");

				CString zsFile;
				zsFile = fileName;
				CString tempZS = (LPCSTR) zsFile;
				tempZS.ReleaseBuffer(-1);
				zsFile = tempZS;
				t= zsFile.Replace(".dat", ".zs");
				file.WriteString( "del " + zsFile +"\n");
				file.WriteString( "del z*.");
				file.Close();

				Sleep(200);
				_spawnl(_P_WAIT, pakBatch, pakBatch, NULL);


				remove(sDatWorkPath);
				if ( rename(sDatPath, sDatWorkPath) )
				perror( NULL );


				CString sLstPath;
				sLstPath = sDatPath;
				CString tempLst = (LPCTSTR) sLstPath;
				tempLst.ReleaseBuffer(-1);
				sLstPath = tempLst;
				int f = sLstPath.Replace(".dat", ".lst");
				CString sLstWorkPath;
				sLstWorkPath = sDatWorkPath;
				CString tempWorkLst = (LPCTSTR) sLstWorkPath;
				tempWorkLst.ReleaseBuffer(-1);
				sLstWorkPath = tempWorkLst;
				int r = sLstWorkPath.Replace(".dat", ".lst");
				remove(sLstWorkPath);
				if ( rename(sLstPath, sLstWorkPath) )
				perror( NULL );

				CString sUnvPath;
				sUnvPath = sDatPath;
				CString tempUnv = (LPCTSTR) sUnvPath;
				tempUnv.ReleaseBuffer(-1);
				sUnvPath = tempUnv;
				int u = sUnvPath.Replace(".dat", ".unv");
				CString sUnvWorkPath;
				sUnvWorkPath = sDatWorkPath;
				CString tempWorkUnv = (LPCTSTR) sUnvWorkPath;
				tempWorkUnv.ReleaseBuffer(-1);
				sUnvWorkPath = tempWorkUnv;
				int v = sUnvWorkPath.Replace(".dat", ".unv");
				remove(sUnvWorkPath);
				if ( rename(sUnvPath, sUnvWorkPath) )
				perror( NULL );

				CString sNeuPath;
				sNeuPath = sDatPath;
				CString tempNeu = (LPCTSTR) sNeuPath;
				tempNeu.ReleaseBuffer(-1);
				sNeuPath = tempNeu;
				int ne = sNeuPath.Replace(".dat", ".neu");
				CString sNeuWorkPath;
				sNeuWorkPath = sDatWorkPath;
				CString tempWorkNeu = (LPCTSTR) sNeuWorkPath;
				tempWorkNeu.ReleaseBuffer(-1);
				sNeuWorkPath = tempWorkNeu;
				int me = sNeuWorkPath.Replace(".dat", ".neu");
				remove(sNeuWorkPath);
				if ( rename(sNeuPath, sNeuWorkPath) )
				perror( NULL );
				m_sNEUPath = sNeuWorkPath;

				AnalysedDlg analysedDlg;
				analysedDlg.DoModal();
			}
			break;
}

}
void CFemConDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
//NO2 NO5 NO8
	// TODO: Add your message handler code here
	
}

void CFemConDlg::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	
	CDialog::OnActivateApp(bActive, dwThreadID);
// NO1 NO6 NO7
	// TODO: Add your message handler code here
}

void CFemConDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(shownDialog==false)
	{
		ConnectToFemap();
		shownDialog=true;
		CreateNEU();
	}
	CDialog::OnShowWindow(bShow, nStatus);
//NO3
	// TODO: Add your message handler code here
}

void CFemConDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ExitProcess(0);
	CLSID       clsid;
	HRESULT hr=AfxGetClassIDFromString("femap.model", &clsid);
	IUnknown    FAR *punk;
	if ( hr == S_OK )
	{
		CoInitialize(NULL);
		hr=GetActiveObject(clsid,NULL,&punk);
		if ( hr == S_OK )
		{			
			CComQIPtr<femap::Imodel> pModel;
			pModel=punk;
			long rc;
			rc = pModel->feAppRegisterAddInPane(false, x, x, false, false, 3, 2);			
		}
	}		
	CDialog::OnClose();
}

void CFemConDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	//bool m_bGlobal = true;
}
