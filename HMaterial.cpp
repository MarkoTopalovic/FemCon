// HMaterial.cpp: implementation of the HMaterial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FemCon.h"
#include "HMaterial.h"
extern double m_dExTAUY;
extern double m_dExCy;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( HMaterial,HDataBlock, 1)

HMaterial::HMaterial()
{

}

HMaterial::HMaterial(const HMaterial &rc)
{
	(*this)=rc;
}


HMaterial::~HMaterial()
{

}

HMaterial& HMaterial::operator =(const HMaterial & rp)
{
	int i;

	m_uDataBlockID=rp.m_uDataBlockID;
	m_uID=rp.m_uID;
	m_uColor=rp.m_uColor;
	m_uType=rp.m_uType;
	m_uSubType=rp.m_uSubType;
	m_uLayer=rp.m_uLayer;
	m_bHas_functions=rp.m_bHas_functions;
	m_strTitle=rp.m_strTitle;
	for (i=0;i<3;i++)
	{
		m_dE[i]=rp.m_dE[i]; // Young's modulus
		m_dG[i]=rp.m_dG[i]; // Shear modulus
		m_dNu[i]=rp.m_dNu[i]; // Poisson's ratio
	}
	for (i=0;i<22;i++)
		m_dGMatrix_3D[i]=rp.m_dGMatrix_3D[i];// Upper triangle of 6x6 3D anisotropic elastic matrix
	for (i=0;i<6;i++)
		m_dGMatrix_2D[i]=rp.m_dGMatrix_2D[i];// Upper triangle of 6x6 3D anisotropic elastic matrix
	for (i=0;i<6;i++)
		m_dAlpha[i]=rp.m_dAlpha[i];// Thermal expansion coefficients
	for (i=0;i<6;i++)
		m_dK[i]=rp.m_dK[i];// Thermal conductivity coefficients
	m_dThermal_cap=rp.m_dThermal_cap;
	m_dDensity=rp.m_dDensity;
	m_dDamping=rp.m_dDamping;
	m_dTemperature=rp.m_dTemperature;
	for (i=0;i<2;i++)
		m_dTensionLimit[i]=rp.m_dTensionLimit[i];
	for (i=0;i<2;i++)
		m_dCompLimit[i]=rp.m_dCompLimit[i];
	m_dShearLimit=rp.m_dShearLimit;
	for (i=0;i<3;i++)
		m_uFE[i]=rp.m_uFE[i];
	for (i=0;i<6;i++)
		m_uFG[i]=rp.m_uFG[i];
	for (i=0;i<6;i++)
		m_uNu[i]=rp.m_uNu[i];
	for (i=0;i<22;i++)
		m_uFGMatrix3D[i]=rp.m_uFGMatrix3D[i];
	for (i=0;i<6;i++)
		m_uFGMatrix2D[i]=rp.m_uFGMatrix2D[i];
	for (i=0;i<6;i++)
		m_uFAlpha[i]=rp.m_uFAlpha[i];
	for (i=0;i<6;i++)
		m_uFK[i]=rp.m_uFK[i];
	m_uFThermal_cap=rp.m_uFThermal_cap;
	m_uFDensity=rp.m_uFDensity;
	m_uFDamping=rp.m_uFDamping;
	m_uFTemperature=rp.m_uFTemperature;
	for (i=0;i<2;i++)
		m_uFTensionLimit[i]=rp.m_uFTensionLimit[i];
	for (i=0;i<2;i++)
		m_uCompLimit[i]=rp.m_uCompLimit[i];
	m_uShearLimit=rp.m_uShearLimit;

	m_dPlastHardSlope=rp.m_dPlastHardSlope;
	m_dExCy = m_dPlastHardSlope;
	m_dPlastYieldLim[0]=rp.m_dPlastYieldLim[0];
	m_dExTAUY= m_dPlastYieldLim[0];
	m_dPlastYieldLim[1]=rp.m_dPlastYieldLim[1];

	m_iNonlin_type=rp.m_iNonlin_type;
	m_iNonlin_func=rp.m_iNonlin_func;


	return *this;	
}


void HMaterial::Serialize(CArchive& ar)
{

	int i;
	if (ar.IsStoring())
	{
		ar << m_uDataBlockID;
		ar << m_uID;
		ar << m_uColor;
		ar << m_uType;
		ar << m_uLayer;
		ar << m_bHas_functions;
		ar << m_strTitle;
		for (i=0;i<3;i++)
		{
			ar << m_dE[i]; // Young's modulus
			ar << m_dG[i]; // Shear modulus
			ar << m_dNu[i]; // Poisson's ratio
		}
		for (i=0;i<22;i++)
			ar << m_dGMatrix_3D[i];// Upper triangle of 6x6 3D anisotropic elastic matrix
		for (i=0;i<6;i++)
			ar << m_dGMatrix_2D[i];// Upper triangle of 6x6 3D anisotropic elastic matrix
		for (i=0;i<6;i++)
			ar << m_dAlpha[i];// Thermal expansion coefficients
		for (i=0;i<6;i++)
			ar << m_dK[i];// Thermal conductivity coefficients
		ar << m_dThermal_cap;
		ar << m_dDensity;
		ar << m_dDamping;
		ar << m_dTemperature;
		for (i=0;i<2;i++)
			ar << m_dTensionLimit[i];
		for (i=0;i<2;i++)
			ar << m_dCompLimit[i];
		ar << m_dShearLimit;
		for (i=0;i<3;i++)
			ar << m_uFE[i];
		for (i=0;i<6;i++)
			ar << m_uFG[i];
		for (i=0;i<6;i++)
			ar << m_uNu[i];
		for (i=0;i<22;i++)
			ar << m_uFGMatrix3D[i];
		for (i=0;i<6;i++)
			ar << m_uFGMatrix2D[i];
		for (i=0;i<6;i++)
			ar << m_uFAlpha[i];
		for (i=0;i<6;i++)
			ar << m_uFK[i];
		ar << m_uFThermal_cap;
		ar << m_uFDensity;
		ar << m_uFDamping;
		ar << m_uFTemperature;
		for (i=0;i<2;i++)
			ar << m_uFTensionLimit[i];
		for (i=0;i<2;i++)
			ar << m_uCompLimit[i];
		ar << m_uShearLimit;

		ar << m_dPlastHardSlope;
		ar << m_dPlastYieldLim[0];
		ar << m_dPlastYieldLim[1];

		ar << m_iNonlin_type;
		ar << m_iNonlin_func;
	}
	else
	{
		WORD w;
		ar >> m_uDataBlockID;
		ar >> m_uID;
		ar >> m_uColor;
		ar >> m_uType;
		ar >> m_uLayer;
		ar >> w;
		m_bHas_functions=(bool)w;
		ar >> m_strTitle;
		for (i=0;i<3;i++)
		{
			ar >> m_dE[i]; // Young's modulus
			ar >> m_dG[i]; // Shear modulus
			ar >> m_dNu[i]; // Poisson's ratio
		}
		for (i=0;i<22;i++)
			ar >> m_dGMatrix_3D[i];// Upper triangle of 6x6 3D anisotropic elastic matrix
		for (i=0;i<2;i++)
			ar >> m_dGMatrix_2D[i];// Upper triangle of 6x6 3D anisotropic elastic matrix
		for (i=0;i<2;i++)
			ar >> m_dAlpha[i];// Thermal expansion coefficients
		for (i=0;i<2;i++)
			ar >> m_dK[i];// Thermal conductivity coefficients
		ar >> m_dThermal_cap;
		ar >> m_dDensity;
		ar >> m_dDamping;
		ar >> m_dTemperature;
		for (i=0;i<2;i++)
			ar >> m_dTensionLimit[i];
		for (i=0;i<2;i++)
			ar >> m_dCompLimit[i];
		ar >> m_dShearLimit;
		for (i=0;i<3;i++)
			ar >> m_uFE[i];
		for (i=0;i<6;i++)
			ar >> m_uFG[i];
		for (i=0;i<6;i++)
			ar >> m_uNu[i];
		for (i=0;i<22;i++)
			ar >> m_uFGMatrix3D[i];
		for (i=0;i<6;i++)
			ar >> m_uFGMatrix2D[i];
		for (i=0;i<6;i++)
			ar >> m_uFAlpha[i];
		for (i=0;i<6;i++)
			ar >> m_uFK[i];
		ar >> m_uFThermal_cap;
		ar >> m_uFDensity;
		ar >> m_uFDamping;
		ar >> m_uFTemperature;
		for (i=0;i<2;i++)
			ar >> m_uFTensionLimit[i];
		for (i=0;i<2;i++)
			ar >> m_uCompLimit[i];
		ar >> m_uShearLimit;

		ar >> m_dPlastHardSlope;
		ar >> m_dPlastYieldLim[0];
		ar >> m_dPlastYieldLim[1];

		ar >> m_iNonlin_type;
		ar >> m_iNonlin_func;
	}
}

