// HCProperties.cpp: implementation of the HCProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Femcon.h"
#include "HCProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(HCProperties,HDataBlock, 1)

HCProperties::HCProperties()
{

}

HCProperties::HCProperties(const HCProperties & rc)
{
	(*this)=rc;
}


HCProperties::~HCProperties()
{

}

HCProperties& HCProperties::operator =(const HCProperties & rp)
{
	int i;

	m_uDataBlockID=rp.m_uDataBlockID;
	m_uID=rp.m_uID;
	m_uColor=rp.m_uColor;	
	m_Zero1=rp.m_Zero1;
	m_uType=rp.m_uType;
	m_uLayer=rp.m_uLayer;
	m_Zero2=rp.m_Zero2;
	m_strTitle=rp.m_strTitle;
	for (i=0;i<4;i++)
		m_uFlag[i]=rp.m_uFlag[i];
	m_uMaxExtr_Mat=rp.m_uMaxExtr_Mat;
	m_uExtra_Matl.SetSize(m_uMaxExtr_Mat);
	for (i=0;i<(int)m_uMaxExtr_Mat;i++)
		m_uExtra_Matl[i]=rp.m_uExtra_Matl[i];
	m_uMax_val=rp.m_uMax_val;
	m_dValue.SetSize(m_uMax_val);
	for (i=0;i<(int)m_uMax_val;i++)
		m_dValue[i]=rp.m_dValue[i];
	return *this;
}


void HCProperties::Serialize(CArchive& ar)
{

	UINT i;
	if (ar.IsStoring())
	{
		ar << m_uDataBlockID;
		ar << m_uID;
		ar << m_uColor;	
		ar << m_Zero1;
		ar << m_uType;
		ar << m_uLayer;
		ar << m_Zero2;
		ar << m_strTitle;
		for (i=0;i<4;i++)
			ar << m_uFlag[i];
		ar << m_uMaxExtr_Mat;
		for (i=0;i< m_uMaxExtr_Mat;i++)
			ar << m_uExtra_Matl[i];
		ar << m_uMax_val;
		for (i=0;i< m_uMax_val;i++)
			ar << m_dValue[i];
	}
	else
	{
		ar >> m_uDataBlockID;
		ar >> m_uID;
		ar >> m_uColor;	
		ar >> m_Zero1;
		ar >> m_uType;
		ar >> m_uLayer;
		ar >> m_Zero2;
		ar >> m_strTitle;
		for (i=0;i<4;i++)
			ar >> m_uFlag[i];
		ar >> m_uMaxExtr_Mat;
		m_uExtra_Matl.SetSize(m_uMaxExtr_Mat);
		for (i=0;i< m_uMaxExtr_Mat;i++)
			ar >> m_uExtra_Matl[i];
		ar >> m_uMax_val;
		m_dValue.SetSize(m_uMax_val);
		for (i=0;i< m_uMax_val;i++)
			ar >> m_dValue[i];
	}
}


