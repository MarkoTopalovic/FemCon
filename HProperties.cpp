// HProperties.cpp: implementation of the HProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Femcon.h"
#include "HProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(HProperties,HDataBlock, 1)

HProperties::HProperties()
{

}

HProperties::HProperties(const HProperties & rc)
{
	(*this)=rc;
}


HProperties::~HProperties()
{

}

HProperties& HProperties::operator =(const HProperties & rp)
{
	int i;

	m_uDataBlockID=rp.m_uDataBlockID;
	m_uID=rp.m_uID;
	m_uColor=rp.m_uColor;	
	m_uMatIID=rp.m_uMatIID;
	m_uType=rp.m_uType;
	m_uLayer=rp.m_uLayer;
	m_uRefCS=rp.m_uRefCS;// Reference coordinate system
	m_strTitle=rp.m_strTitle;
	for (i=0;i<4;i++)
		m_uFlag[i]=rp.m_uFlag[i];
	m_uNum_lam=rp.m_uNum_lam;
	m_uLam_MID.SetSize(m_uNum_lam);
	for (i=0;i<(int)m_uNum_lam;i++)
		m_uLam_MID[i]=rp.m_uLam_MID[i];
	m_uNum_val=rp.m_uNum_val;
	m_dValue.SetSize(m_uNum_val);
	for (i=0;i<(int)m_uNum_val;i++)
		m_dValue[i]=rp.m_dValue[i];
	return *this;
}


void HProperties::Serialize(CArchive& ar)
{

	UINT i;
	if (ar.IsStoring())
	{
		ar << m_uDataBlockID;
		ar << m_uID;
		ar << m_uColor;	
		ar << m_uMatIID;
		ar << m_uType;
		ar << m_uLayer;
		ar << m_uRefCS;// Reference coordinate system
		ar << m_strTitle;
		for (i=0;i<4;i++)
			ar << m_uFlag[i];
		ar << m_uNum_lam;
		for (i=0;i< m_uNum_lam;i++)
			ar << m_uLam_MID[i];
		ar << m_uNum_val;
		for (i=0;i< m_uNum_val;i++)
			ar << m_dValue[i];
	}
	else
	{
		ar >> m_uDataBlockID;
		ar >> m_uID;
		ar >> m_uColor;	
		ar >> m_uMatIID;
		ar >> m_uType;
		ar >> m_uLayer;
		ar >> m_uRefCS;// Reference coordinate system
		ar >> m_strTitle;
		for (i=0;i<4;i++)
			ar >> m_uFlag[i];
		ar >> m_uNum_lam;
		m_uLam_MID.SetSize(m_uNum_lam);
		for (i=0;i< m_uNum_lam;i++)
			ar >> m_uLam_MID[i];
		ar >> m_uNum_val;
		m_dValue.SetSize(m_uNum_val);
		for (i=0;i< m_uNum_val;i++)
			ar >> m_dValue[i];
	}
}


