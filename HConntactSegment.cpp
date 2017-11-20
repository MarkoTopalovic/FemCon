// HContactSegment.cpp: implementation of the HLoads class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Femcon.h"
#include "HContactSegment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( HContactSegment,HDataBlock, 1)

HContactSegment::HContactSegment()
{
	m_Entity.SetSize(0,1);

}

HContactSegment::HContactSegment(const HContactSegment & rc)
{
	(*this)=rc;
}


HContactSegment::~HContactSegment()
{
	m_Entity.RemoveAll();
}

HContactSegment& HContactSegment::operator =(const HContactSegment & rp)
{
	int i;

	m_uDataBlockID=rp.m_uDataBlockID;
	m_uID=rp.m_uID;
	m_uColor=rp.m_uColor;
	m_uLayer=rp.m_uLayer;
	m_uOutput=rp.m_uOutput;
	m_uRefnode=rp.m_uRefnode;
	m_uRigid=rp.m_uRigid;
	m_uRef_sys=rp.m_uRef_sys;
	m_uOffset=rp.m_uOffset;
	m_strTitle=rp.m_strTitle;

	m_Entity.SetSize(rp.m_Entity.GetSize());
	for (i=0;i<m_Entity.GetSize();i++)
	{
		Entity et;
		et.m_uEntityID=rp.m_Entity[i].m_uEntityID;
		et.m_uFaceID=rp.m_Entity[i].m_uFaceID;
		et.m_uType=rp.m_Entity[i].m_uType;
		et.m_uExpanded=rp.m_Entity[i].m_uExpanded;
		m_Entity.SetAt(i,et);
	}
	return *this;
}


void HContactSegment::Serialize(CArchive& ar)
{
	int i;
	if (ar.IsStoring())
	{
		ar << m_uDataBlockID;
		ar << m_uID;
		ar << m_uColor;
		ar << m_uLayer;
		ar << m_uOutput;
		ar << m_uRefnode;
		ar << m_uRigid;
		ar << m_uRef_sys;
		ar << m_uOffset;
		ar << m_strTitle;

		ar << m_Entity.GetSize();
		for (i=0;i<m_Entity.GetSize();i++)
		{
			ar << m_Entity[i].m_uEntityID;
			ar << m_Entity[i].m_uFaceID;
			ar << m_Entity[i].m_uType;
			ar << m_Entity[i].m_uExpanded;
		}

	}
	else
	{
		WORD w;
		ar >> m_uDataBlockID;
		ar >> m_uID;
		ar >> m_uColor;
		ar >> m_uLayer;
		ar >> m_uOutput;
		ar >> m_uRefnode;
		ar >> m_uRigid;
		ar >> m_uRef_sys;
		ar >> m_uOffset;
		ar >> m_strTitle;
		ar >> m_strTitle;

		ar >> w;
		m_Entity.SetSize(w);
		for (i=0;i<m_Entity.GetSize();i++)
		{
			Entity et;
			ar >> et.m_uEntityID;
			ar >> et.m_uFaceID;
			ar >> et.m_uType;
			ar >> et.m_uExpanded;
			m_Entity.SetAt(i,et);
		}
	}
}


Entity& Entity::operator =(const Entity & rp)
{
	m_uEntityID=rp.m_uEntityID;
	m_uFaceID=rp.m_uFaceID;
	m_uType=rp.m_uType;
	m_uExpanded=rp.m_uExpanded;
	return *this;
}

Entity::Entity(const Entity & rc)
{
	(*this)=rc;
}

Entity::Entity()
{

}
