// HConstraints.cpp: implementation of the HConstraints class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Femcon.h"
#include "HConstraints.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(HConstraints,HDataBlock, 1)

HConstraints::HConstraints()
{

	m_ConsNodes.SetSize(0,10);
	m_ConsEqs.SetSize(0,10);

}

HConstraints::HConstraints (const HConstraints& rc)
{
	(*this)=rc;
}

HConstraints& HConstraints::operator =(const HConstraints& rc)
{
	int i;

	m_uDataBlockID=rc.m_uDataBlockID;
	m_uSetID=rc.m_uSetID;
	m_strTitle=rc.m_strTitle;

	m_ConsNodes.SetSize(rc.m_ConsNodes.GetSize());
	for (i=0;i<m_ConsNodes.GetSize();i++)
	{
		ConsNode& node=m_ConsNodes[i];
		node.m_uNodeID=rc.m_ConsNodes[i].m_uNodeID;
		node.m_uColor=rc.m_ConsNodes[i].m_uColor;
		node.m_uLayer=rc.m_ConsNodes[i].m_uLayer;
		for (int j=0;j<6;j++)
			node.m_bDOF[j]=rc.m_ConsNodes[i].m_bDOF[j];

	}

	m_ConsEqs.SetSize(rc.m_ConsEqs.GetSize());
	for (i=0;i<m_ConsEqs.GetSize();i++)
	{
		ConsEq& eq=m_ConsEqs[i];
		eq.m_uEqnID=rc.m_ConsEqs[i].m_uEqnID;
		eq.m_uColor=rc.m_ConsEqs[i].m_uColor;
		eq.m_uLayer=rc.m_ConsEqs[i].m_uLayer;
		eq.m_EqCoefs.SetSize(rc.m_ConsEqs[i].m_EqCoefs.GetSize());
		for (int j=0;j<eq.m_EqCoefs.GetSize();j++)
		{
			EqCoeff& ec=eq.m_EqCoefs[j];
			ec.m_uEqn_nodeID=rc.m_ConsEqs[i].m_EqCoefs[j].m_uEqn_nodeID;
			ec.m_uEqn_dof=rc.m_ConsEqs[i].m_EqCoefs[j].m_uEqn_dof;
			ec.m_dCoeff=rc.m_ConsEqs[i].m_EqCoefs[j].m_dCoeff;
		}
	}

	return *this;
}

HConstraints::~HConstraints()
{
	m_ConsNodes.RemoveAll();

//	for (int i=0;i<m_ConsEqs.GetSize();i++)
//		m_ConsEqs[i].m_EqCoefs.RemoveAll();
	m_ConsEqs.RemoveAll();
}

void HConstraints::Serialize(CArchive& ar)
{
	int i;
	if (ar.IsStoring())
	{
		ar << m_uDataBlockID;
		ar << m_uSetID;
		ar << m_strTitle;
		ar << m_ConsNodes.GetSize();
		for (i=0;i<m_ConsNodes.GetSize();i++)
		{
			ar << m_ConsNodes[i].m_uNodeID;
			ar << m_ConsNodes[i].m_uColor;
			ar << m_ConsNodes[i].m_uLayer;
			for (int j=0;j<6;j++)
				ar << m_ConsNodes[i].m_bDOF[j];
		}
		
		ar << m_ConsEqs.GetSize();
		for (i=0;i<m_ConsEqs.GetSize();i++)
		{
			ar << m_ConsEqs[i].m_uEqnID;
			ar << m_ConsEqs[i].m_uColor;
			ar << m_ConsEqs[i].m_uLayer;
			ar << m_ConsEqs[i].m_EqCoefs.GetSize();
			for (int j=0;j<m_ConsEqs[i].m_EqCoefs.GetSize();j++)
			{
				ar << m_ConsEqs[i].m_EqCoefs[j].m_uEqn_nodeID;
				ar << m_ConsEqs[i].m_EqCoefs[j].m_uEqn_dof;
				ar << m_ConsEqs[i].m_EqCoefs[j].m_dCoeff;
			}
		}

	}
	else
	{
		int nSize;
		ar >> m_uDataBlockID;
		ar >> m_uSetID;
		ar >> m_strTitle;
		ar >> nSize;
		m_ConsNodes.SetSize(nSize);
		for (i=0;i<nSize;i++)
		{
//			ConsNode& node=m_ConsNodes[i];
			ar >> m_ConsNodes[i].m_uNodeID;
			ar >> m_ConsNodes[i].m_uColor;
			ar >> m_ConsNodes[i].m_uLayer;
			for (int j=0;j<6;j++)
			{
				WORD w;
				ar >> w;
				m_ConsNodes[i].m_bDOF[j]=(bool)w;
			}
		}
		ar >> nSize;
		m_ConsEqs.SetSize(nSize);
		for (i=0;i<nSize;i++)
		{
			ConsEq& eq=m_ConsEqs[i];
			ar >> eq.m_uEqnID;
			ar >> eq.m_uColor;
			ar >> eq.m_uLayer;
			ar >> nSize;
			eq.m_EqCoefs.SetSize(nSize);
			for (int j=0;j<eq.m_EqCoefs.GetSize();j++)
			{
				EqCoeff& ec=eq.m_EqCoefs[j];
				ar >> ec.m_uEqn_nodeID;
				ar >> ec.m_uEqn_dof;
				ar >> ec.m_dCoeff;
				eq.m_EqCoefs.SetAt(j,ec);
			}
		}
	}
}

ConsNode::ConsNode()
{
	;
}

ConsNode::ConsNode(const ConsNode & rc)
{
	(*this)=rc;
}

ConsNode::~ConsNode()
{
	;
}

ConsNode& ConsNode::operator =(const ConsNode & rc)
{
	m_uNodeID=rc.m_uNodeID;
	m_uColor=rc.m_uColor;
	m_uLayer=rc.m_uLayer;
	for (int i=0;i<6;i++)
		m_bDOF[i]=rc.m_bDOF[i];
	return *this;
}


ConsEq::ConsEq ()
{
	m_EqCoefs.SetSize(0,1);
}
ConsEq::~ConsEq ()
{
	m_EqCoefs.RemoveAll();
}

ConsEq::ConsEq(const ConsEq & rc)
{
	(*this)=rc;
}

ConsEq& ConsEq::operator =(const ConsEq & rc)
{
	m_uColor=rc.m_uColor;
	m_uEqnID=rc.m_uEqnID;
	m_uLayer=rc.m_uLayer;
	m_EqCoefs.Copy(rc.m_EqCoefs);

	return *this;
}

