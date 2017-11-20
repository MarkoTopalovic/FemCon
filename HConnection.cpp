// HConnection.cpp: implementation of the HConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Femcon.h"
#include "HConnection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(HConnection,HDataBlock, 1)

HConnection::HConnection()
{

}

HConnection::HConnection(const HConnection & rc)
{
	(*this)=rc;
}


HConnection::~HConnection()
{

}

HConnection& HConnection::operator =(const HConnection & rp)
{
	int i;

	m_uDataBlockID=rp.m_uDataBlockID;
	m_uID=rp.m_uID;
	m_uColor=rp.m_uColor;	
	m_Pid=rp.m_Pid;
	m_uType=rp.m_uType;
	m_uLayer=rp.m_uLayer;
	m_Disabled=rp.m_Disabled;
	m_strTitle=rp.m_strTitle;
	for (i=0;i<2;i++)
		m_uregionID[i]=rp.m_uregionID[i];
	return *this;
}


void HConnection::Serialize(CArchive& ar)
{

	UINT i;
	if (ar.IsStoring())
	{
		ar << m_uDataBlockID;
		ar << m_uID;
		ar << m_uColor;	
		ar << m_Pid;
		ar << m_uType;
		ar << m_uLayer;
		ar << m_Disabled;
		ar << m_strTitle;
		for (i=0;i< 2;i++)
			ar << m_uregionID[i];
	}
	else
	{
		ar >> m_uDataBlockID;
		ar >> m_uID;
		ar >> m_uColor;	
		ar >> m_Pid;
		ar >> m_uType;
		ar >> m_uLayer;
		ar >> m_Disabled;
		ar >> m_strTitle;
//		m_uregionID.SetSize(2);
		for (i=0;i< 2;i++)
			ar >> m_uregionID[i];
	}
}


