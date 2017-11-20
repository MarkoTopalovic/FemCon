// HLoads.h: interface for the HLoads class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLOADS_H__ADDF1161_651D_11D2_8266_004F4900B9E0__INCLUDED_)
#define AFX_HLOADS_H__ADDF1161_651D_11D2_8266_004F4900B9E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "HDataBlock.h"

class StructLoad : public CObject
{
public:
	StructLoad();
	~StructLoad();
	StructLoad& operator=(const StructLoad& rc);
	StructLoad(const StructLoad&);
	UINT m_uLoadID;
	UINT m_uLoadtype;
	UINT m_uColor;
	UINT m_uLayer;
	UINT m_uDefine_sys;
	UINT m_uSl_funcID;
	double m_dPhase;
	double m_dCoefficient;
	UINT m_uDof_face[6];
	double m_dValue[6];
	double m_dAddI_coeff;
	UINT m_uAddI_fnc[3];
	bool m_bCan_shade;
	bool m_bCan_be_shade;
	UINT m_uSubtype;
	UINT m_uDir_func[3];
	double m_dDirection[3];
};

class NodalTemp : public CObject
{
public:
	 NodalTemp();
	NodalTemp& operator =(const NodalTemp& rc);
	NodalTemp(const NodalTemp& rc);
	UINT m_uNdtempID;
	UINT m_uColor;
	UINT m_uLayer;
	double m_dNdtemp;
	UINT m_uNdt_funcID;
};

class ElTemp : public CObject
{
public:
	 ElTemp();
	 ElTemp(const ElTemp& rc);
	ElTemp& operator = (const ElTemp& rc);
	UINT m_uEltempID;
	UINT m_uColor;
	UINT m_uLayer;
	double m_dEltemp;
	UINT m_uElf_funcID;
};

class HLoads : public HDataBlock  
{
public:	
	HLoads();
	HLoads(const HLoads& rc);
	virtual ~HLoads();
	HLoads& operator = (const HLoads& rp);
	DECLARE_SERIAL(HLoads)
	void Serialize( CArchive& ar );

	UINT m_uSetID;
	CString m_strTitle;
	UINT m_uCSys;
	double m_dDef_temp;
	bool m_bTemp_on;
	bool m_bGrav_on;
	bool m_bOmega_on;
	double m_dGrav[6];
	UINT m_dGrav_fun[6];
	double m_dOrigin[3];
	double m_dOmega[3];
	double m_dStef_boltz;
	double m_dAbs_temp;
	double m_dFree_cnv_exp;
	double m_dFc_flu_cond;
	double m_dFc_flu_cp;
	double m_dFc_flu_vis;
	double m_dFc_flu_dens;
	double m_dFc_cons_coef;
	double m_dFc_reynolds;
	double m_dFc_pran_in;
	double m_dFc_pran_out;
	UINT m_uTfc_flu_cond;
	UINT m_uTfc_flu_cp;
	UINT m_uTfc_flu_vis;
	bool m_bAlt_free_conv;
	bool m_bFc_flu_flag;
	bool m_bFc_conv_flow;

	CArray<StructLoad,StructLoad&> m_StructLoads;
	CArray<NodalTemp,NodalTemp&> m_NodalTemps;
	CArray<ElTemp,ElTemp&> m_ElTemps;

};

#endif // !defined(AFX_HLOADS_H__ADDF1161_651D_11D2_8266_004F4900B9E0__INCLUDED_)
