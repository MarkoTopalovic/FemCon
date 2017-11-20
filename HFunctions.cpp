// HFunctions.cpp: implementation of the HFunctions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PAKExp.h"
#include "HFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( HFunctions,HDataBlock, 1)

HFunctions::HFunctions()
{
	m_FunctionEntry.SetSize(0,5);
}

HFunctions::HFunctions (const HFunctions& rc)
{
	(*this)=rc;
}

HFunctions& HFunctions::operator =(const HFunctions& rc)
{
	m_uDataBlockID=rc.m_uDataBlockID;
	m_uID=rc.m_uID;
	m_uFunc_type=rc.m_uFunc_type;
	m_strTitle=rc.m_strTitle;

	m_FunctionEntry.SetSize(rc.m_FunctionEntry.GetSize());

	for (int i=0;i<rc.m_FunctionEntry.GetSize();i++)
	{
		FunctionEntry* obj= new FunctionEntry;

		obj->m_uEntryID=rc.m_FunctionEntry[i].m_uEntryID;
		obj->m_dX=rc.m_FunctionEntry[i].m_dX;
		obj->m_dY= rc.m_FunctionEntry[i].m_dY;
		m_FunctionEntry[i]=*obj;
	}

	return *this;
}

HFunctions::~HFunctions()
{
	m_FunctionEntry.RemoveAll();
}

void HFunctions::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_uID;//ID of function
		ar << m_uFunc_type;//Type of function (0=Dimensionless, 1=vs. Time, 2=vs. Temp)
		ar << m_strTitle;//Function title (max 25 char)
		ar << m_FunctionEntry.GetSize();
		for (int i=0;i<m_FunctionEntry.GetSize();i++)
		{
			ar << m_FunctionEntry[i].m_uEntryID;
			ar << m_FunctionEntry[i].m_dX;
			ar << m_FunctionEntry[i].m_dY;
		}
	}
	else
	{
		UINT u;
		ar >> m_uID;//ID of function
		ar >> m_uFunc_type;//Type of function (0=Dimensionless, 1=vs. Time, 2=vs. Temp)
		ar >> m_strTitle;//Function title (max 25 char)
		ar >>u;
		m_FunctionEntry.SetSize(u);
		for (int i=0;i<m_FunctionEntry.GetSize();i++)
		{
			FunctionEntry* fe = new FunctionEntry;
			ar >> fe->m_uEntryID;
			ar >> fe->m_dX;
			ar >> fe->m_dY;
			m_FunctionEntry.SetAt(i,*fe);
		}
	}
}
