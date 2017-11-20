// PakOptValues.cpp: implementation of the CPakOptValues class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FemCon.h"
#include "PakOptValues.h"
#include "BlockIDs.h"
#include "FemConDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPakOptValues::CPakOptValues()
{

}

CPakOptValues::~CPakOptValues()
{

}

CString CPakOptValues::GetTitle()
{
	return(dlgGeneralOptions->m_strTitle);
}

bool	CPakOptValues::GetDynamicAnIsSet()
{
	return(dlgGeneralOptions->m_bDynamic);	
}

bool    CPakOptValues::GetEigenvalueIsSet()
{
	return(dlgGeneralOptions->m_bEigenValue);
}

int		CPakOptValues::GetNSOPV()
{
	return(dlgGeneralOptions->m_iNSOPV);
}

int		CPakOptValues::GetProgramStart()
{
	return(dlgGeneralOptions->m_iProgramStart);
}

int		CPakOptValues::GetAnalysisType()
{
	return(dlgGeneralOptions->m_iAnalysisType);
}

//TimePeriods

int		CPakOptValues::GetNumOfPeriods()
{
	if (dlgGeneralOptions->dlgTimePeriods->m_iNumOfTStep.GetSize()!=0)
	{
		return(dlgGeneralOptions->dlgTimePeriods->m_iNumOfTStep.GetSize());
	}
	else
	{
		return(1);
	}
}

int		CPakOptValues::GetNumSteps(int i)
{
	if (dlgGeneralOptions->dlgTimePeriods->m_iNumOfTStep.GetSize()==0) return(1);
	return(dlgGeneralOptions->dlgTimePeriods->m_iNumOfTStep.GetAt(i));
}

double	CPakOptValues::GetStep(int i)
{
	if (dlgGeneralOptions->dlgTimePeriods->m_iNumOfTStep.GetSize()==0) return(1.);
	return(dlgGeneralOptions->dlgTimePeriods->m_dTStepIncrement.GetAt(i));
}

int CPakOptValues::GetMaterialModel(int ElementType, int MaterialType)
{
	if (GetAnalysisType()!=0)
	{
		switch (MaterialType)
		{
		case 1:
			return(1);
			break;
		case 3:
			return(3);
			break;
		case 5:
			if (ElementType==1)
			{
				return(5);
			}
			else
			{
//				if (ElementType==PAK_ISO_3D)
//				{
					return(6);
//				}
//				else
//				{
//					return(1);
//				}
			}
			break;
		case 9:
			return(9);
			break;
		case 14:
			return(14);
			break;
		case 42:
			return(42);
			break;
		}
	}
	else
	{
		return(1);
	}
}


//Ither method

int	CPakOptValues::GetIterMethod()
{
	return(dlgGeneralOptions->dlgIterMethod->m_iItherMethod);
}

int CPakOptValues::GetConvCrit()
{
	return((dlgGeneralOptions->dlgIterMethod->m_bConvEnergy*1)+(dlgGeneralOptions->dlgIterMethod->m_bConvForce*2));
}

double CPakOptValues::GetConvTol(int mode)
{
	if (mode==0)
	{
		return(dlgGeneralOptions->dlgIterMethod->m_dTOLE);
	}
	else
	{
		return(dlgGeneralOptions->dlgIterMethod->m_dTOLS);
	}
}

UINT CPakOptValues::GetNodeNumber()
{
	return(dlgGeneralOptions->dlgIterMethod->m_uNodeNumber);
}

int CPakOptValues::GetDirection()
{
	return(dlgGeneralOptions->dlgIterMethod->m_iDirection);
}

double CPakOptValues::GetValue()
{
	return(dlgGeneralOptions->dlgIterMethod->m_dValue);
}

double CPakOptValues::GetAG()
{
	return(dlgGeneralOptions->dlgIterMethod->m_dAG);
}

double CPakOptValues::GetDS()
{
	return(dlgGeneralOptions->dlgIterMethod->m_dDS);
}

int CPakOptValues::GetNMODS()
{
	return(dlgGeneralOptions->dlgImperfection->m_Data.GetSize());
}

Imperfection CPakOptValues::GetImperfection(int i)
{
	return(dlgGeneralOptions->dlgImperfection->m_Data[i]);
}


int CPakOptValues::GetSBTypeOfSegment()
{
	return(dlgSBeamOptions->m_nType);
}

double CPakOptValues::GetSBParamA()
{
	return(dlgSBeamOptions->m_dA);
}

bool CPakOptValues::GetHardeningMaterial()
{
	if (dlgGeneralOptions->dlgMaterialDLG->m_dHS>0 && dlgMaterialDLG->m_dAN>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
