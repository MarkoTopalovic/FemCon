// HMaterial.h: interface for the HMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMATERIAL_H__E4BF19A0_6284_11D2_B069_444553540000__INCLUDED_)
#define AFX_HMATERIAL_H__E4BF19A0_6284_11D2_B069_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HDataBlock.h"

class HMaterial : public HDataBlock  
{
public:
	HMaterial();
	HMaterial (const HMaterial& rc);
	virtual ~HMaterial();

	HMaterial& operator = (const HMaterial& rp);
	DECLARE_SERIAL(HMaterial)
	void Serialize( CArchive& ar );
	UINT m_uID;
	UINT m_uColor;
	UINT m_uType;
	UINT m_uSubType;
	UINT m_uLayer;
	bool m_bHas_functions;
	CString m_strTitle;
	double m_dE[3]; // Young's modulus
	double m_dG[3]; // Shear modulus
	double m_dNu[3]; // Poisson's ratio
	double m_dGMatrix_3D[22];// Upper triangle of 6x6 3D anisotropic elastic matrix
	double m_dGMatrix_2D[6];// Upper triangle of 6x6 3D anisotropic elastic matrix
	double m_dAlpha[6];// Thermal expansion coefficients
	double m_dK[6];// Thermal conductivity coefficients
	double m_dThermal_cap;
	double m_dDensity;
	double m_dDamping;
	double m_dTemperature;
	double m_dTensionLimit[2];
	double m_dCompLimit[2];
	double m_dShearLimit;
// Naredne promenljive se odnose na funkcije
	UINT m_uFE[3];
	UINT m_uFG[3];
	UINT m_uNu[3];
	UINT m_uFGMatrix3D[22];
	UINT m_uFGMatrix2D[6];
	UINT m_uFAlpha[6];
	UINT m_uFK[6];
	UINT m_uFThermal_cap;
	UINT m_uFDensity;
	UINT m_uFDamping;
	UINT m_uFTemperature;
	UINT m_uFTensionLimit[2];
	UINT m_uCompLimit[2];
	UINT m_uShearLimit;

	double m_dPlastHardSlope;
	double m_dPlastYieldLim[2];
	double m_dHypPolyord[2];
	int	   m_iNonlin_type;
	UINT   m_iNonlin_func;
	int	   m_iHard_type;
	int	   m_iYield_type;
};

#endif // !defined(AFX_HMATERIAL_H__E4BF19A0_6284_11D2_B069_444553540000__INCLUDED_)
