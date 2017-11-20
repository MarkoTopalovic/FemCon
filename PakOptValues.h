// PakOptValues.h: interface for the CPakOptValues class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "GeneralOptionsDlg.h"
#include "GeneralDynAnOpt.h"
#include "GeneralOptIterMethod.h"
#include "GeneralOptTimePeriods.h"
#include "DlgImperfection.h"
#include "SBeamOptions.h"
#include "MaterialDLG.h"
class CPakOptValues  
{
public:
	double GetSBParamA();
	int GetSBTypeOfSegment();
	CPakOptValues();
	virtual ~CPakOptValues();
	
	CGeneralOptionsDlg *dlgGeneralOptions;
	CGeneralOptTimePeriods *dlgTimePeriods;
	CGeneralOptIterMethod  *dlgIterMethod;
	CSBeamOptions *dlgSBeamOptions;
	CMaterialDLG *dlgMaterialDLG;
	CString GetTitle();
	bool	GetDynamicAnIsSet();
	bool    GetEigenvalueIsSet();
	int		GetNSOPV();
	int		GetProgramStart();
	int		GetAnalysisType();

//Time periods
	int		GetNumOfPeriods();
	int		GetNumSteps(int i);
	double	GetStep(int i);

//Ither method
	int		GetIterMethod();
	int 	GetMaterialModel(int ElementType, int MaterialType);

	int		GetConvCrit();
	double	GetConvTol(int mode);
	UINT	GetNodeNumber();
	int		GetDirection();
	double	GetValue();
	double	GetAG();
	double	GetDS();

//Imperfection
	int GetNMODS();
	Imperfection GetImperfection(int i);
	bool GetHardeningMaterial();
};
