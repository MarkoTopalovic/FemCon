// HLoads.cpp: implementation of the HLoads class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Femcon.h"
#include "HLoads.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( HLoads,HDataBlock, 1)

HLoads::HLoads()
{
	m_StructLoads.SetSize(0,10);
	m_NodalTemps.SetSize(0,1);
	m_ElTemps.SetSize(0,1);

}

HLoads::HLoads(const HLoads & rc)
{
	(*this)=rc;
}


HLoads::~HLoads()
{
	m_StructLoads.RemoveAll();

	m_NodalTemps.RemoveAll();

	m_ElTemps.RemoveAll();
}

HLoads& HLoads::operator =(const HLoads & rp)
{
	int i;

	m_uDataBlockID=rp.m_uDataBlockID;
	m_uSetID=rp.m_uSetID;
	m_strTitle=rp.m_strTitle;
	m_uCSys=rp.m_uCSys;
	m_dDef_temp=rp.m_dDef_temp;
	m_bTemp_on=rp.m_bTemp_on;
	m_bGrav_on=rp.m_bGrav_on;
	m_bOmega_on=rp.m_bOmega_on;
	for (i=0;i<6;i++)
		{
			m_dGrav[i]=rp.m_dGrav[i];
			m_dGrav_fun[i]=rp.m_dGrav_fun[i];
		}
	for (i=0;i<3;i++)
		m_dOrigin[i]=rp.m_dOrigin[i];
	for (i=0;i<3;i++)
		m_dOmega[i]=rp.m_dOmega[i];
	m_dStef_boltz=rp.m_dStef_boltz;
	m_dAbs_temp=rp.m_dAbs_temp;
	m_dFree_cnv_exp=rp.m_dFree_cnv_exp;
	m_dFc_flu_cond=rp.m_dFc_flu_cond;
	m_dFc_flu_cp=rp.m_dFc_flu_cp;
	m_dFc_flu_vis=rp.m_dFc_flu_vis;
	m_dFc_flu_dens=rp.m_dFc_flu_dens;
	m_dFc_cons_coef=rp.m_dFc_cons_coef;
	m_dFc_reynolds=rp.m_dFc_reynolds;
	m_dFc_pran_in=rp.m_dFc_pran_in;
	m_dFc_pran_out=rp.m_dFc_pran_out;
	m_uTfc_flu_cond=rp.m_uTfc_flu_cond;
	m_uTfc_flu_cp=rp.m_uTfc_flu_cp;
	m_uTfc_flu_vis=rp.m_uTfc_flu_vis;
	m_bAlt_free_conv=rp.m_bAlt_free_conv;
	m_bFc_flu_flag=rp.m_bFc_flu_flag;
	m_bFc_conv_flow=rp.m_bFc_conv_flow;

/*	m_StructLoads.SetSize(rp.m_StructLoads.GetSize());
	for (i=0;i<m_StructLoads.GetSize();i++)
	{
		int j;
		StructLoad sl;
		sl.m_uLoadID=rp.m_StructLoads[i].m_uLoadID;
		sl.m_uLoadtype=rp.m_StructLoads[i].m_uLoadtype;
		sl.m_uColor=rp.m_StructLoads[i].m_uColor;
		sl.m_uLayer=rp.m_StructLoads[i].m_uLayer;
		sl.m_uDefine_sys=rp.m_StructLoads[i].m_uDefine_sys;
		sl.m_uSl_funcID=rp.m_StructLoads[i].m_uSl_funcID;
		sl.m_dPhase=rp.m_StructLoads[i].m_dPhase;
		sl.m_dCoefficient=rp.m_StructLoads[i].m_dCoefficient;
		for (j=0;j<6;j++)
			sl.m_uDof_face[j]=rp.m_StructLoads[i].m_uDof_face[j];
		for (j=0;j<6;j++)
			sl.m_dValue[j]=rp.m_StructLoads[i].m_dValue[j];
		sl.m_dAddI_coeff=rp.m_StructLoads[i].m_dAddI_coeff;
		for (j=0;j<3;j++)
		sl.m_uAddI_fnc[j]=rp.m_StructLoads[i].m_uAddI_fnc[j];
		sl.m_bCan_shade=rp.m_StructLoads[i].m_bCan_shade;
		sl.m_bCan_be_shade=rp.m_StructLoads[i].m_bCan_be_shade;
		sl.m_uSubtype=rp.m_StructLoads[i].m_uSubtype;
		for (j=0;j<3;j++)
			sl.m_uDir_func[j]=rp.m_StructLoads[i].m_uDir_func[j];
		for (j=0;j<3;j++)
			sl.m_dDirection[j]=rp.m_StructLoads[i].m_dDirection[j];
		m_StructLoads.SetAt(i,sl);
	}*/
	m_StructLoads.Copy(rp.m_StructLoads);

	m_NodalTemps.SetSize(rp.m_NodalTemps.GetSize());
	for (i=0;i<m_NodalTemps.GetSize();i++)
	{
		NodalTemp nt;
		nt.m_uNdtempID=rp.m_NodalTemps[i].m_uNdtempID;
		nt.m_uColor=rp.m_NodalTemps[i].m_uColor;
		nt.m_uLayer=rp.m_NodalTemps[i].m_uLayer;
		nt.m_dNdtemp=rp.m_NodalTemps[i].m_dNdtemp;
		nt.m_uNdt_funcID=rp.m_NodalTemps[i].m_uNdt_funcID;
		m_NodalTemps.SetAt(i,nt);
	}	

	m_ElTemps.SetSize(rp.m_ElTemps.GetSize());
	for (i=0;i<m_ElTemps.GetSize();i++)
	{
		ElTemp et;
		et.m_uEltempID=rp.m_ElTemps[i].m_uEltempID;
		et.m_uColor=rp.m_ElTemps[i].m_uColor;
		et.m_uLayer=rp.m_ElTemps[i].m_uLayer;
		et.m_dEltemp=rp.m_ElTemps[i].m_dEltemp;
		et.m_uElf_funcID=rp.m_ElTemps[i].m_uElf_funcID;
		m_ElTemps.SetAt(i,et);
	}
	return *this;
}


void HLoads::Serialize(CArchive& ar)
{
	int i;
	if (ar.IsStoring())
	{
		ar << m_uDataBlockID;
		ar << m_uSetID;
		ar << m_strTitle;
		ar << m_uCSys;
		ar << m_dDef_temp;
		ar << m_bTemp_on;
		ar << m_bGrav_on;
		ar << m_bOmega_on;
		for (i=0;i<6;i++)
		{
			ar << m_dGrav[i];
			ar << m_dGrav_fun[i];
		}
		for (i=0;i<3;i++)
			ar << m_dOrigin[i];
		for (i=0;i<3;i++)
			ar << m_dOmega[i];
		ar << m_dStef_boltz;
		ar << m_dAbs_temp;
		ar << m_dFree_cnv_exp;
		ar << m_dFc_flu_cond;
		ar << m_dFc_flu_cp;
		ar << m_dFc_flu_vis;
		ar << m_dFc_flu_dens;
		ar << m_dFc_cons_coef;
		ar << m_dFc_reynolds;
		ar << m_dFc_pran_in;
		ar << m_dFc_pran_out;
		ar << m_uTfc_flu_cond;
		ar << m_uTfc_flu_cp;
		ar << m_uTfc_flu_vis;
		ar << m_bAlt_free_conv;
		ar << m_bFc_flu_flag;
		ar << m_bFc_conv_flow;
		ar << m_StructLoads.GetSize();
		for (i=0;i<m_StructLoads.GetSize();i++)
		{
			int j;

			ar << m_StructLoads[i].m_uLoadID;
			ar << m_StructLoads[i].m_uLoadtype;
			ar << m_StructLoads[i].m_uColor;
			ar << m_StructLoads[i].m_uLayer;
			ar << m_StructLoads[i].m_uDefine_sys;
			ar << m_StructLoads[i].m_uSl_funcID;
			ar << m_StructLoads[i].m_dPhase;
			ar << m_StructLoads[i].m_dCoefficient;
			for (j=0;j<6;j++)
				ar << m_StructLoads[i].m_uDof_face[j];
			for (j=0;j<6;j++)
				ar << m_StructLoads[i].m_dValue[j];
			ar << m_StructLoads[i].m_dAddI_coeff;
			for (j=0;j<3;j++)
			ar << m_StructLoads[i].m_uAddI_fnc[j];
			ar << m_StructLoads[i].m_bCan_shade;
			ar << m_StructLoads[i].m_bCan_be_shade;
			ar << m_StructLoads[i].m_uSubtype;
			for (j=0;j<3;j++)
				ar << m_StructLoads[i].m_uDir_func[j];
			for (j=0;j<3;j++)
				ar << m_StructLoads[i].m_dDirection[j];

		}

		ar << m_NodalTemps.GetSize();
		for (i=0;i<m_NodalTemps.GetSize();i++)
		{

			ar << m_NodalTemps[i].m_uNdtempID;
			ar << m_NodalTemps[i].m_uColor;
			ar << m_NodalTemps[i].m_uLayer;
			ar << m_NodalTemps[i].m_dNdtemp;
			ar << m_NodalTemps[i].m_uNdt_funcID;
		}	

		ar << m_ElTemps.GetSize();
		for (i=0;i<m_ElTemps.GetSize();i++)
		{

			ar << m_ElTemps[i].m_uEltempID;
			ar << m_ElTemps[i].m_uColor;
			ar << m_ElTemps[i].m_uLayer;
			ar << m_ElTemps[i].m_dEltemp;
			ar << m_ElTemps[i].m_uElf_funcID;
		}

	}
	else
	{
		WORD w;
		ar >> m_uDataBlockID;
		ar >> m_uSetID;
		ar >> m_strTitle;
		ar >> m_uCSys;
		ar >> m_dDef_temp;
		ar >> w;
		m_bTemp_on=(bool)w;
		ar >> w;
		m_bGrav_on=(bool)w;
		ar >> w;
		m_bOmega_on=(bool)w;
		for (i=0;i<6;i++)
		{
			ar >> m_dGrav[i];
			ar >> m_dGrav_fun[i];
		}
		for (i=0;i<3;i++)
			ar >> m_dOrigin[i];
		for (i=0;i<3;i++)
			ar >> m_dOmega[i];
		ar >> m_dStef_boltz;
		ar >> m_dAbs_temp;
		ar >> m_dFree_cnv_exp;
		ar >> m_dFc_flu_cond;
		ar >> m_dFc_flu_cp;
		ar >> m_dFc_flu_vis;
		ar >> m_dFc_flu_dens;
		ar >> m_dFc_cons_coef;
		ar >> m_dFc_reynolds;
		ar >> m_dFc_pran_in;
		ar >> m_dFc_pran_out;
		ar >> m_uTfc_flu_cond;
		ar >> m_uTfc_flu_cp;
		ar >> m_uTfc_flu_vis;
		ar >> w;
		m_bAlt_free_conv=(bool)w;
		ar >> w;
		m_bFc_flu_flag=(bool)w;
		ar >> w;
		m_bFc_conv_flow=(bool)w;

		ar >> w;
		m_StructLoads.SetSize(w);
		for (i=0;i<m_StructLoads.GetSize();i++)
		{

			int j;
			StructLoad sl;
			ar << sl.m_uLoadID;	  
			ar << sl.m_uLoadtype;
			ar << sl.m_uColor;
			ar << sl.m_uLayer;
			ar << sl.m_uDefine_sys;
			ar << sl.m_uSl_funcID;
			ar << sl.m_dPhase;
			ar << sl.m_dCoefficient;
			for (j=0;j<6;j++)
				ar << sl.m_uDof_face[j];
			for (j=0;j<6;j++)
				ar << sl.m_dValue[j];
			ar << sl.m_dAddI_coeff;
			for (j=0;j<3;j++)
			ar << sl.m_uAddI_fnc[j];
			ar << sl.m_bCan_shade;
			ar << sl.m_bCan_be_shade;
			ar << sl.m_uSubtype;
			for (j=0;j<3;j++)
				ar << sl.m_uDir_func[j];
			for (j=0;j<3;j++)
				ar << sl.m_dDirection[j];
			m_StructLoads.SetAt(i,sl);			
		}

		ar >> w;
		m_NodalTemps.SetSize(w);
		for (i=0;i<m_NodalTemps.GetSize();i++)
		{

			NodalTemp nt;
			ar >> nt.m_uNdtempID;
			ar >> nt.m_uColor;
			ar >> nt.m_uLayer;
			ar >> nt.m_dNdtemp;
			ar >> nt.m_uNdt_funcID;
			m_NodalTemps.SetAt(i,nt);
		}	

		ar >> w;
		m_ElTemps.SetSize(w);
		for (i=0;i<m_ElTemps.GetSize();i++)
		{

			ElTemp et;
			ar >> et.m_uEltempID;
			ar >> et.m_uColor;
			ar >> et.m_uLayer;
			ar >> et.m_dEltemp;
			ar >> et.m_uElf_funcID;
			m_ElTemps.SetAt(i,et);
		}
	}
}

StructLoad::StructLoad()
{
	;
}

StructLoad::~StructLoad()
{
	;
}

StructLoad::StructLoad(const StructLoad & rc)
{
	(*this)=rc;

}

StructLoad& StructLoad::operator =(const StructLoad & rp)
{
	int j;

	m_uLoadID=rp.m_uLoadID;
	m_uLoadtype=rp.m_uLoadtype;
	m_uColor=rp.m_uColor;
	m_uLayer=rp.m_uLayer;
	m_uDefine_sys=rp.m_uDefine_sys;
	m_uSl_funcID=rp.m_uSl_funcID;
	m_dPhase=rp.m_dPhase;
	m_dCoefficient=rp.m_dCoefficient;
	for (j=0;j<6;j++)
		m_uDof_face[j]=rp.m_uDof_face[j];
	for (j=0;j<6;j++)
		m_dValue[j]=rp.m_dValue[j];
	m_dAddI_coeff=rp.m_dAddI_coeff;
	for (j=0;j<3;j++)
	m_uAddI_fnc[j]=rp.m_uAddI_fnc[j];
	m_bCan_shade=rp.m_bCan_shade;
	m_bCan_be_shade=rp.m_bCan_be_shade;
	m_uSubtype=rp.m_uSubtype;
	for (j=0;j<3;j++)
		m_uDir_func[j]=rp.m_uDir_func[j];
	for (j=0;j<3;j++)
		m_dDirection[j]=rp.m_dDirection[j];

	return *this;
}

NodalTemp::NodalTemp(const NodalTemp & rc)
{
	(*this)=rc;
}

NodalTemp& NodalTemp::operator =(const NodalTemp & rp)
{
	m_uNdtempID=rp.m_uNdtempID;
	m_uColor=rp.m_uColor;
	m_uLayer=rp.m_uLayer;
	m_dNdtemp=rp.m_dNdtemp;
	m_uNdt_funcID=rp.m_uNdt_funcID;
	return *this;
}

NodalTemp::NodalTemp()
{

}

ElTemp& ElTemp::operator =(const ElTemp & rp)
{
	m_uEltempID=rp.m_uEltempID;
	m_uColor=rp.m_uColor;
	m_uLayer=rp.m_uLayer;
	m_dEltemp=rp.m_dEltemp;
	m_uElf_funcID=rp.m_uElf_funcID;
	return *this;
}

ElTemp::ElTemp(const ElTemp & rc)
{
	(*this)=rc;
}

ElTemp::ElTemp()
{

}
