// HElement.cpp: implementation of the HElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Femcon.h"
#include "HElement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(HElement,HDataBlock, 1)

HElement::HElement()
{
	int i;

	m_uID=0;
	m_uLayer=0;
	m_uOrientID=0;
	m_uPropID=0;
	m_uTopology=0;
	m_uType=0;
	m_uColor=0;
	for (i=0;i<20;i++)
		m_uNode[i]=0;
	for (i=0;i<3;i++)
	{
		m_dOrient[i]=0.;
		m_dOffset1[i]=0.;
		m_dOffset2[i]=0.;
	}
	for (i=0;i<6;i++)
	{
		m_bRelease1[i]=false;
		m_bRelease2[i]=false;
	}

}

HElement::HElement(HElement& rc)
{
	(*this)=rc;
}


HElement::~HElement()
{

}

HElement& HElement::operator = (const HElement& rp)
{
	int i;

	m_uDataBlockID=rp.m_uDataBlockID;	
	m_uID=rp.m_uID;
	m_uColor=rp.m_uColor;
	m_uPropID=rp.m_uPropID;
	m_uType=rp.m_uType;
	m_uTopology=rp.m_uTopology;
	m_uLayer=rp.m_uLayer;
	m_uOrientID=rp.m_uOrientID; 
	m_bMatl_orflag=rp.m_bMatl_orflag;// Material orientation flag
	for (i=0;i<20;i++)
		m_uNode[i]=rp.m_uNode[i];
	for (i=0;i<3;i++)
		m_dOrient[i]=rp.m_dOrient[i];
	for (i=0;i<3;i++)
		m_dOffset1[i]=rp.m_dOffset1[i];
	for (i=0;i<3;i++)
		m_dOffset2[i]=rp.m_dOffset2[i];
	for (i=0;i<6;i++)
		m_bRelease1[i]=rp.m_bRelease1[i];
	for (i=0;i<6;i++)
		m_bRelease2[i]=rp.m_bRelease2[i];
	return *this;
}

void HElement::Serialize(CArchive& ar)
{
	int i;
	if (ar.IsStoring())
	{
		ar << m_uDataBlockID;	
		ar << m_uID;
		ar << m_uColor;
		ar << m_uPropID;
		ar << m_uType;
		ar << m_uTopology;
		ar << m_uLayer;
		ar << m_uOrientID; 
		ar << m_bMatl_orflag;// Material orientation flag
		for (i=0;i<20;i++)
			ar << m_uNode[i];
		for (i=0;i<3;i++)
			ar << m_dOrient[i];
		for (i=0;i<3;i++)
			ar << m_dOffset1[i];
		for (i=0;i<3;i++)
			ar << m_dOffset2[i];
		for (i=0;i<6;i++)
			ar << m_bRelease1[i];
		for (i=0;i<6;i++)
			ar << m_bRelease2[i];
	}
	else
	{
		WORD w;
		ar >> m_uDataBlockID;
		ar >> m_uID;
		ar >> m_uColor;
		ar >> m_uPropID;
		ar >> m_uType;
		ar >> m_uTopology;
		ar >> m_uLayer;
		ar >> m_uOrientID; 
		ar >> w;
		m_bMatl_orflag=(bool)w;
		for (i=0;i<20;i++)
			ar >> m_uNode[i];
		for (i=0;i<3;i++)
			ar >> m_dOrient[i];
		for (i=0;i<3;i++)
			ar >> m_dOffset1[i];
		for (i=0;i<3;i++)
			ar >> m_dOffset2[i];
		for (i=0;i<6;i++)
		{
			ar >> w;
			m_bRelease1[i]=(bool)w;
		}
		for (i=0;i<6;i++)
		{
			ar >> w;
			m_bRelease2[i]=(bool)w;
		}

	}
}
/*
void HElement::DrawElement(int * flags)
{
	HNodes node;
	int j,k;

	switch(*(flags))
	{
	case 0:
		for(j=0;j<Elements[m_uTopology];j++)
		{

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		glBegin(GL_TRIANGLES);

		for(k=0;k<3;k++)
		{	
			node=m_nNodArray[m_nNodeIndex[m_uNode[Geometry[m_uTopology][j][k]]]];
		
			glIndexi(*(flags+2));
			glEdgeFlag(Edge[m_uTopology][j][k]);
			glVertex3d(node.m_dX, node.m_dY, node.m_dZ);
		}
	
		glEnd();

		glEnable( GL_POLYGON_OFFSET_LINE );
		glPolygonOffset( -0.15, -0.1 );

		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		glBegin(GL_TRIANGLES);

		glIndexi(6);

		for(k=0;k<3;k++)
		{	
			node=m_nNodArray[m_nNodeIndex[m_uNode[Geometry[m_uTopology][j][k]]]];
		
			glEdgeFlag(Edge[m_uTopology][j][k]);
			glVertex3d(node.m_dX, node.m_dY, node.m_dZ);
		}
	
		glEnd();

		glDisable( GL_POLYGON_OFFSET_FILL );
		}
		break;

	case 1:
		glBegin(GL_LINES);

		glIndexi(6);
		for(j=0;j<Lines[m_uTopology];j++)
		{
			for(k=0;k<2;k++)
			{	
				node=m_nNodArray[m_nNodeIndex[m_uNode[WireTopology[m_uTopology][j][k]]]];
			
				glIndexi(*(flags+2));
				glVertex3d(node.m_dX, node.m_dY, node.m_dZ);
			}
		}
		glEnd();
		break;

	default:
		break;
	}

	bool f=false;
	char s[20];

	switch(*(flags+3))
	{
		case '1': 
			sprintf(s,"%i",m_uID);
			f=true;
			break;
		case '2': 
			sprintf(s,"%i",m_uPropID);
			f=true;
			break;
		case '3': 
			sprintf(s,"%i",m_uType);
			f=true;
			break;

		default: 
			break;
	}

	if (f)
	{
		double dX, dY, dZ;

		dX=dY=dZ=0;

		int l;

		l=0;

		for(j=0;j<10;j++)
		{
			if (m_uNode[j]!=0)
			{
				node=m_nNodArray[m_nNodeIndex[m_uNode[j]]];

				dX=dX+node.m_dX;
				dY=dY+node.m_dY;
				dZ=dZ+node.m_dZ;
				
				l=l+1;
			}
		}

		dX=dX/l;
		dY=dY/l;
		dZ=dZ/l;

		glRasterPos3d(dX, dY, dZ);

		glPushAttrib(GL_LIST_BIT);
		glListBase(base-31);
		glCallLists(strlen(s), GL_UNSIGNED_BYTE, &s[0]);
		glPopAttrib();
	}
}
*/