// HNodes.cpp: implementation of the HNodes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Femcon.h"
#include "HNodes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( HNodes,HDataBlock, 1)

HNodes::HNodes()
{
	m_uID=0;
	m_uDefine_sys=0;
	m_uOutput_sys=0;
	m_uLayer=0;
	m_uColor=0;
	m_dX=m_dY=m_dZ=0.;
	for (int i=0;i<6;i++)
		m_bPermbc[i]=false;
}

HNodes::HNodes(const HNodes & rc)
{
	(*this)=rc;
}


HNodes::~HNodes()
{

}

HNodes& HNodes::operator =(const HNodes & rp)
{
	m_uDataBlockID=rp.m_uDataBlockID;
	m_uID=rp.m_uID;
	m_uDefine_sys=rp.m_uDefine_sys;// ID of definition coordinate system
	m_uOutput_sys=rp.m_uOutput_sys;// ID of output coordinate system
	m_uLayer=rp.m_uLayer;
	m_uColor=rp.m_uColor;
	for (int i=0;i<6;i++)
		m_bPermbc[i]=rp.m_bPermbc[i];// the six permanent constaints
	m_dX=rp.m_dX;
	m_dY=rp.m_dY;
	m_dZ=rp.m_dZ;

	return *this;

}

void HNodes::Serialize(CArchive& ar)
{
	int i;
	if (ar.IsStoring())
	{	
		ar << m_uDataBlockID;
		ar << m_uID;
		ar << m_uDefine_sys;// ID of definition coordinate system
		ar << m_uOutput_sys;// ID of output coordinate system
		ar << m_uLayer;
		ar << m_uColor;
		for (i=0;i<6;i++)
			ar << m_bPermbc[6];// the six permanent constaints
// Coordinates in Global Rectangular coordinate system:
		ar << m_dX;
		ar << m_dY;
		ar << m_dZ;
	}
	else
	{
		WORD w;
		ar >> m_uDataBlockID;
		ar >> m_uID;
		ar >> m_uDefine_sys;// ID of definition coordinate system
		ar >> m_uOutput_sys;// ID of output coordinate system
		ar >> m_uLayer;
		ar >> m_uColor;
		for (i=0;i<6;i++)
		{
			ar >> w;
			m_bPermbc[6]=(bool)w;
		}
// Coordinates in Global Rectangular coordinate system:
		ar >> m_dX;
		ar >> m_dY;
		ar >> m_dZ;
	}
}

