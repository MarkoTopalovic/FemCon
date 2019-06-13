// PAKTExp.cpp: Functions for export to PAK's DAT file.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FemCon.h"
#include "ModelData.h"
#include "PAKPExp.h"
#include "BlockIDs.h"
#include "PakExpOpt.h"
#include "math.h"
#include "PakString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


UINT CModelData::ExportPAK_P(CStdioFile* file,CPakExpOpt *peo)
{

	CString str;
	int uNPER,iINPT,iICCGG;
	UINT uNGELEM;
	UINT i;
	long lGeneralDataPos;
	

	if((m_NodArray.GetSize())<=0)
	{
		AfxMessageBox(NO_NODES);
		return(0);	
	};
	
	if((uNGELEM=m_PropArray.GetSize())<=0)
	{
		AfxMessageBox(NO_GROUPS);
		return(0);	
	};
	iINPT = 1;
	iICCGG = -11;
/*	if ((m_NodArray.GetSize())>99999)
		{
			iINPT=1;
			iICCGG=-11;
		}
	else
		{
			iINPT=0;
			iICCGG=0;
		}  */
//Card /2/
	file->WriteString(CardH2);
	file->WriteString(CardV2);
	str.Format("%5u%5u\n",INDFOR,iINPT);
	file->WriteString(str);
//Card /3/
	uNPER=m_PakOptValue.GetNumOfPeriods();
	if (uNPER==-1) uNPER=0;
 	file->WriteString(CardH3);
	file->WriteString(CardV3);
	lGeneralDataPos=file->GetPosition();
	str.Format("                                                         \n");
	file->WriteString(str);

//Card /4/
 	file->WriteString(CardH4);
	file->WriteString(CardV4);
	str.Format("               %5u%10.6f%10.6f%5u\n",MAXIT,EPSTA,EPSTR,NJRAP);
	file->WriteString(str);
//Card /5/
 	file->WriteString(CardH5);
	file->WriteString(CardV5);
	str.Format("%5u%5d\n",IREST,iICCGG);
	file->WriteString(str);
//Card /6/
 	file->WriteString(CardH6);
	file->WriteString(CardV6);
	for(i=1;i<=(UINT)m_PakOptValue.GetNumOfPeriods();i++)
	{
		str.Format("%5u%10.6f\n",m_PakOptValue.GetNumSteps(i-1),m_PakOptValue.GetStep(i-1));
		file->WriteString(str);
	}

//Card /7/
	ExportPAKP_Nodes(file, iINPT);

//Card /8/
//	ExportPAKT_Elements(file,&uNGELEM,peo);
// Stampa sve elemente u jednu grupu. Od juna 2015. svi elementi u PAKVu se stampaju u jednu grupu
	ExportPAKP_Elements_Without_Groups(file, &uNGELEM, peo, iINPT);

// Card /9/
	ExportPAKP_PrescTemp(file, iINPT);

//Card /10/
 	file->WriteString(CardH10p);
	file->WriteString(CardV10p);
    str.Format ("                              %10.6f%5d%5d%5d%10.6f\n",GAMA,IOSA,ISIL,IFIL,ZASIC);
	file->WriteString(str);

/*
    file->WriteString (CardH9);
    file->WriteString (CardV9);
    if (m_PakOptValue.GetDynamicAnIsSet())
	{
		str.Format("%5d%5d%5d%5d%5d%5d\n",0,1,1,0,1,1);
	}
	else
	{
		str.Format("%5d%5d%5d%5d\n",0,1,1,0);
	}

	file->WriteString(str);
	
    file->WriteString (CardH9_1);
    file->WriteString (CardV9_1);
    str.Format ("%5d%5d%5d%10.6f%10.6f%10.6f%10.6f%10.6f%10.6f\n",1,0,0,0.,0.,0.,m_MaterialsArray[0].m_dDensity,2000.,0);
	file->WriteString(str);
	
    file->WriteString (CardH9_2);
    file->WriteString (CardV9_2_a_1);
    file->WriteString (CardV9_2_a_2);
    str.Format ("%5d%5d\n",1,1);
	file->WriteString(str);

    file->WriteString (CardV9_2_b_1);
    file->WriteString (CardV9_2_b_2);
    str.Format ("%10.6f%10.6f\n",0.,m_MaterialsArray[0].m_dK[0]);
	file->WriteString(str);

	if (m_PakOptValue.GetDynamicAnIsSet())
    {
		file->WriteString (CardH9_3);
		file->WriteString (CardV9_3_a_1);
		file->WriteString (CardV9_3_a_2);
		str.Format ("%5d%5d\n",1,1);
		file->WriteString(str);

		file->WriteString (CardV9_3_b_1);
		file->WriteString (CardV9_3_b_2);
		str.Format ("%10.6f%10.6f\n",0.,m_MaterialsArray[0].m_dThermal_cap);
		file->WriteString(str);
	}
*/
	
//Card /11/
	ExportPAKP_Loads(file,iINPT);

	/*
//Card /12-6/
	file->WriteString (CardH12_6);
	file->WriteString (CardV12_6);
    str.Format ("%10.6f\n",m_MaterialsArray[0].m_dTemperature);
    file->WriteString (str);
*/

//Card /13/
	ExportPAKP_MatModels(file);

//Card /14/
//	ExportPAKT_TimeFunctions(file);
	ExportPAK_TimeFunctions(file);

//Card /15/
 	file->WriteString(CardH15);
	file->WriteString(CardV15);

//Upisivanje naknadno

	file->Seek(lGeneralDataPos,CFile::begin);
//	str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u",m_NodArray.GetSize(),m_PropArray.GetSize(),m_MaterialsArray.GetSize(),m_PakOptValue.GetDynamicAnIsSet(),uNPER,NPRINT,IANIZ,NULAZ,ITOR,IGRAF);
// Na silu Brj grupa elemenata je uvek 1 
//	if((m_NodArray.GetSize())<=99999)
	if (iINPT == 0)
	{
		str.Format("%5u               %5u%5u%5u%5u%5u", m_NodArray.GetSize(), uNPER, NPRINT, (m_PakOptValue.GetDynamicAnIsSet() ? 0 : 1), IVDP, ISNUMBER);
	}
	else
	{
		str.Format("%10u               %5u%5u%5u%5u%5u", m_NodArray.GetSize(), uNPER, NPRINT, (m_PakOptValue.GetDynamicAnIsSet() ? 0 : 1), IVDP, ISNUMBER);
	}

	file->WriteString(str);
	file->Seek(0,CFile::end);

	return(-1);
}

UINT CModelData::ExportPAKP_Nodes(CStdioFile* file, int iINPT)
{

	CString str;
	UINT i;
	HNodes nn;

//Card /7/
 	file->WriteString(CardH7);
	file->WriteString(CardV7);
	if (iINPT == 0)
		{
			for(i=0;i<(UINT)m_NodArray.GetSize();i++)
			{
				nn=m_NodArray[i];

				str.Format("%5u   %2d%10.5f%10.5f%10.5f          %5u\n",
					nn.m_uID,0,nn.m_dX,nn.m_dY,nn.m_dZ,KORC);
				file->WriteString(str);
			}
		}
	else 
		{ 
			for(i=0;i<(UINT)m_NodArray.GetSize();i++)
			{
				nn=m_NodArray[i];

				str.Format("%10u   %2d%10.5f%10.5f%10.5f          %5u\n",
					nn.m_uID,0,nn.m_dX,nn.m_dY,nn.m_dZ,KORC);
				file->WriteString(str);
			}
		}
//	for(i=0;i<(UINT)m_NodArray.GetSize();i++)
//	{
//		nn=m_NodArray[i];
//
//		str.Format("%5u   %2d%10.5f%10.5f%10.5f          %5u\n",
//			nn.m_uID,0,nn.m_dX,nn.m_dY,nn.m_dZ,KORC);
//		file->WriteString(str);
//	}


	return(-1);
}

//**************************************************************************
//**************************************************************************
// Ova funkcija eksportuje sve elemente u jednu grupu. Od juna 2015. u PAKPu svi elementi se zapisiju u jednu grupu
//**************************************************************************
UINT CModelData::ExportPAKP_Elements_Without_Groups(CStdioFile* file, UINT *uNGELEM, CPakExpOpt *peo, int iINPT)
{
	CString str;
	UINT i,j,k;
	HProperties pr;
	CArray <UINT,UINT&> ElNum;
	CMap<UINT,UINT,UINT,UINT> PropIndex; // relation between Property ID and it's position in ElNum;
	HElement el;
	UINT uINDAX,uNETIP;
	UINT uNP3DMX;
	HLoads lo;
	HNodes nn;
	int tipEl;

	//FEMAT Topology
	//Value          {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
	  UINT FEM_Top[]={2,3,3,6,4,8,4,6,8,1,10,15,20, 0, 0, 0};

// Stampaju se svi elementi u kartici /8/, nema grupa elemenata u PAKV od juna 2015.
				 //Card /8/
				 file->WriteString(CardH8);
				 file->WriteString(CardV8);
		//		 str.Format("%5u%5u%5u%5u\n",PAKT_ISO_3D,uNE_3D,INDAX,IZIP);
// Sneza dodala kad ima vise od 100000 elemenata
				if(iINPT<1)
					{
					 str.Format("%5u%5u%5u%10.6f%10.6f\n",PAKT_ISO_3D,m_ElArray.GetSize(),INDAX,HEIGHT,RADIUS);
					}
				else
					{
					 str.Format("%5u%10u%5u%10.6f%10.6f\n",PAKT_ISO_3D,m_ElArray.GetSize(),INDAX,HEIGHT,RADIUS);
					}
				 file->WriteString(str);
				 
				 file->WriteString(CardH8_3);
				 file->WriteString(CardV8_3_a_1);
				 file->WriteString(CardV8_3_a_2);
//				 if(pr.m_uType==FET_3D_LIN) uNP3DMX=20; else uNP3DMX=20;
				 uNP3DMX=20;
//				 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u\n",
//					NMAT3D,MAT3D,uNP3DMX,IPR3DC,IPRTG3,NGAUSX,NGAUSY,NGAUSZ);
				 str.Format("%5u     %5u\n",
					m_MaterialsArray.GetSize(),uNP3DMX);
//
				 file->WriteString(str);
//
				 file->WriteString(CardV8_3_b_1);
				 file->WriteString(CardV8_3_b_2);
				 file->WriteString(CardV8_3_c_1);
				 file->WriteString(CardV8_3_c_2);
// kraj stampanja zaglavlja kartice /8/

//	for(i=0;i<*uNGELEM;i++) // nema grupa
	for(j=0;j<(UINT)m_ElArray.GetSize();j++)
	{
		i=m_ElArray[j].m_uPropID-1;
		pr=m_PropArray[i];

		switch (pr.m_uType)
		{
//
//****************************************************************************
// ****************************** 1D Elementi *********************************
//****************************************************************************
		case FET_ROD: case FET_BEAM: case FET_BEAM37:
			{
				
//				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
//					if(m_ElArray[j].m_uPropID==pr.m_uID)
//					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_LINE || el.m_uTopology==FETO_LINE3)
						{
						    tipEl=FEM_Top[el.m_uTopology]+10;
							if (iINPT<1)
								{
									str.Format("%5u",el.m_uID);
								}
							else
								{
									str.Format("%10u",el.m_uID);
								}
							file->WriteString(str);
							// nov format. stampa sve elemente u istoj grupi, u jednoj liniji stampa "20" cvorova za sve elemente (nule kad nema cvorova)
							for(k=0;k<20;k++) 
							{
								if (iINPT<1)
								{
									str.Format("%5u",el.m_uNode[k]);
								}
								else
								{
									str.Format("%10u",el.m_uNode[k]);
								}
								file->WriteString(str);
							}
							str.Format("%5u     %10.6f%5u\n",pr.m_uMatIID,pr.m_dValue[0],tipEl);
							file->WriteString(str);
						}
//					}
			}
			break;
//****************************************************************************
// ****************************** 2D Elementi *********************************
//****************************************************************************
		case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR:
		case FET_PLATE_LIN:case FET_PLATE_PAR:
		case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR:
		case FET_AXISYM_LIN:case FET_AXISYM_PAR:

			{
//				UINT uNE_2D=0;
//				UINT uNP2DMX;

//				uNETIP=PAKT_ISO_2D;
//				uINDAX=0;
//				if(pr.m_uType==FET_AXISYM_LIN || pr.m_uType==FET_AXISYM_PAR) uINDAX=1;

/*				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						if(m_ElArray[j].m_uTopology==FETO_QUAD4 || m_ElArray[j].m_uTopology==FETO_QUAD8 ||
							m_ElArray[j].m_uTopology==FETO_TRI3 || m_ElArray[j].m_uTopology==FETO_TRI6) uNE_2D++;
					}

				if(uNE_2D>0)	//Cetvorougaoni 2D Element
				{ */
				 UINT node_order_quad[20]={0,1,2,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,8};
				// UINT node_order_tri[20]={0,1,2,2,4,5,2,6,8,8,8,8,8,8,8,8,8,8,8,8}; // degenerisani 2D element
				 UINT node_order_tri[20]={0,1,2,4,5,6,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
				 UINT *node_order;

// Na silu stampa zaglavlje samo za prvu grupu  - Svi elementi u jednoj grupi - Sneza avgust 2015
/*		if(first_group)
		{
				first_group=FALSE;
				 //Card /8/
				 file->WriteString(CardH8);
				 file->WriteString(CardV8);

		//		 str.Format("%5u%5u%5u%5u\n",uNETIP,uNE_2D,uINDAX,IZIP);
		//	Na silu stampa da je broj elemenata u grupi jednak ukupnom broju elemenata
				 str.Format("%5u%5u%5u%10.6f%10.6f\n",uNETIP,m_ElArray.GetSize(),uINDAX,HEIGHT,RADIUS);
				 file->WriteString(str);

				 file->WriteString(CardH8_2);
				 file->WriteString(CardV8_2_a_1);
				 file->WriteString(CardV8_2_a_2);
				 if(pr.m_uType==FET_MEMBRANE_LIN || pr.m_uType==FET_PLATE_LIN ||
					 pr.m_uType==FET_PLANESTRAIN_LIN || pr.m_uType==FET_AXISYM_LIN) uNP2DMX=4; else uNP2DMX=20;
//				 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f\n",
//					NMAT2D,MAT2D,uNP2DMX,IPR2DC,IPTG2,NGAU2X,NGAU2Y,NTHIC,pr.m_dValue[0]);
				 str.Format("%5u     %5u\n",
					m_MaterialsArray.GetSize(),uNP2DMX);
				 
				 file->WriteString(str);

				 file->WriteString(CardV8_2_b_1);
				 file->WriteString(CardV8_2_b_2);
				 file->WriteString(CardV8_2_c_1);
				 file->WriteString(CardV8_2_c_2);
		} */
// kraj stampanja zaglavlja samo za prvu grupu

//				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
//					if(m_ElArray[j].m_uPropID==pr.m_uID)
//					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8 ||
							el.m_uTopology==FETO_TRI3 || el.m_uTopology==FETO_TRI6)
						{
						   switch(el.m_uTopology)
							{
							 case FETO_QUAD4: 
							  node_order=node_order_quad; tipEl=24; break;
							 case FETO_QUAD8: 
							  node_order=node_order_quad; tipEl=28; break;
							 case FETO_TRI3: 
							  node_order=node_order_tri; tipEl=23; break;
							 case FETO_TRI6: 
							  node_order=node_order_tri; tipEl=26; break;
							}
						   
						   if (iINPT<1)
								{
									str.Format("%5u",el.m_uID);
								}
							else
								{
									str.Format("%10u",el.m_uID);
								}
							file->WriteString(str);
//							for(k=0;k<8;k++) 
							// nov format. stampa sve elemente u istoj grupi, u jednoj liniji stampa "20" cvorova za sve elemente (nule kad nema cvorova)
							for(k=0;k<20;k++) 
							{
								if (iINPT<1)
								{
									str.Format("%5u",el.m_uNode[node_order[k]]);
								}
								else
								{
									str.Format("%10u",el.m_uNode[node_order[k]]);
								}
								file->WriteString(str);
							}

							str.Format("%5u     %10.6f%5u\n",pr.m_uMatIID,pr.m_dValue[0],tipEl);
							file->WriteString(str);

						}
//					}
				//}
			}
			break;
//****************************************************************************
// ****************************** 3D Elementi ********************************
//****************************************************************************
		case FET_3D_LIN:case FET_3D_PAR:
			{
				UINT uNE_3D=0,uNE_PRISM=0,uNE_TETRA=0;

// stampa elemente redom, ne treba da broji elemente odredjenog tipa
/*				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20 ||
						   el.m_uTopology==FETO_WEDGE6 || el.m_uTopology==FETO_WEDGE15 ||
						   el.m_uTopology==FETO_TETRA4 || el.m_uTopology==FETO_TETRA10)	uNE_3D++;
					}

				if(uNE_3D>0)	// 3D Element
				{ */
				 UINT node_order_3d[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};
				 UINT node_order_prism[20]={5,6,4,4,1,2,0,0,17,18,4,16,9,10,0,8,13,14,12,12};
//				 UINT node_order_tetra[20]={4,4,4,4,1,2,0,0,4,4,4,4,9,10,0,8,13,14,12,12};
				 UINT node_order_tetra[20]={0,1,2,4,8,9,10,12,13,14,15,15,15,15,15,15,15,15,15,15};
				 UINT *node_order;

//				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
//				 {
//					if(m_ElArray[j].m_uPropID==pr.m_uID)
//					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20 ||
						   el.m_uTopology==FETO_WEDGE6 || el.m_uTopology==FETO_WEDGE15 ||
						   el.m_uTopology==FETO_TETRA4 || el.m_uTopology==FETO_TETRA10)
						{
	/*						switch(el.m_uTopology)
							{
							 case FETO_BRICK8: case FETO_BRICK20: 
							  node_order=node_order_3d; break;
							 case FETO_WEDGE6: case FETO_WEDGE15: 
							  node_order=node_order_prism; break;
							 case FETO_TETRA4: case FETO_TETRA10: 
							  node_order=node_order_tetra; break;
							} */
							switch(el.m_uTopology)
								{
									case FETO_BRICK8:
									  node_order=node_order_3d; tipEl=38; break;
									case FETO_BRICK20: 
									  node_order=node_order_3d; tipEl=320; break;
									case FETO_WEDGE6:
									  node_order=node_order_prism; tipEl=36; break;
									case FETO_WEDGE15: 
									  node_order=node_order_prism; tipEl=315; break;
									case FETO_TETRA4:
									  node_order=node_order_tetra; tipEl=34; break;
									case FETO_TETRA10: 
									  node_order=node_order_tetra; tipEl=310; break;
								}
							   if (iINPT<1)
									{
										str.Format("%5u",el.m_uID);
									}
								else
									{
										str.Format("%10u",el.m_uID);
									}
							file->WriteString(str);
//							for(k=0;k<8;k++) 
							// nov format. stampa sve elemente u istoj grupi, u jednoj liniji stampa "20" cvorova za sve elemente (nule kad nema cvorova)
							for(k=0;k<20;k++) 
							{
								if (iINPT<1)
								{
									str.Format("%5u",el.m_uNode[node_order[k]]);
								}
								else
								{
									str.Format("%10u",el.m_uNode[node_order[k]]);
								}
								file->WriteString(str);
							}

							str.Format("%5u               %5u\n",pr.m_uMatIID,tipEl);
							file->WriteString(str);

// stampa 0 ako nema medjucvorova, svi cvorovi se stampaju u jednom redu
/*							if(el.m_uTopology==FETO_BRICK20 || el.m_uTopology==FETO_WEDGE15 || el.m_uTopology==FETO_TETRA10)
							{
							 for(k=8;k<20;k++) 
							 {
								str.Format("%5u",el.m_uNode[node_order[k]]);
								file->WriteString(str);
							 }
							 file->WriteString("\n");
							}  */
						}
//					}
//				 }
				//}
			}
			break;
		}
	}

	return(-1);
}

UINT CModelData::ExportPAKP_PrescTemp(CStdioFile* file, int iINPT)
{
	HLoads lo;
	UINT i;
	CString str;
	HNodes nn;


	if(m_LoadArray.GetSize()>0 && m_LoadArray[0].m_NodalTemps.GetSize())
	{
		lo=m_LoadArray[0];

		file->WriteString(CardH9);

		file->WriteString(CardH9_a);
		file->WriteString(CardV9_a);
		if (iINPT == 0)
			{
				str.Format("%5d\n",lo.m_NodalTemps.GetSize());
			}
		else
			{
				str.Format("%10d\n",lo.m_NodalTemps.GetSize());
			}
		file->WriteString(str);

		file->WriteString(CardH9_b);
		file->WriteString(CardV9_b);
		for(i=0;i<(UINT)lo.m_NodalTemps.GetSize();i++)
		{
//			if(lo.m_NodalTemps[i].m_dNdtemp>=0)
//			{
				UINT fn=lo.m_NodalTemps[i].m_uNdt_funcID;

				//if(lo.m_NodalTemps[i].m_uNdt_funcID<1) fn=1;
		/*		UINT ccvor;
				ccvor = lo.m_NodalTemps[i].m_uNdtempID;
				nn = m_NodArray[ccvor-1]; */
				
				if (iINPT == 0)
				{
					str.Format("%5u%5u%5u%10.4lf\n",lo.m_NodalTemps[i].m_uNdtempID,INDPR,fn,
						lo.m_NodalTemps[i].m_dNdtemp);
				}
				else
				{
					str.Format("%10u%5u%5u%10.4lf%10u\n",lo.m_NodalTemps[i].m_uNdtempID,INDPR,fn,
						lo.m_NodalTemps[i].m_dNdtemp,i+1);
				}
				file->WriteString(str);
//			}
		}
/*		for(i=0;i<(UINT)lo.;i++)
		{
			if(lo.m_NodalTemps[i].m_dNdtemp>=0)
			{
				UINT fn=lo.m_NodalTemps[i].m_uNdt_funcID;

				//if(lo.m_NodalTemps[i].m_uNdt_funcID<1) fn=1;
				if((m_NodArray.GetSize())<=99999)
				{
					str.Format("%5u%5u%5u%10.4lf\n",lo.m_NodalTemps[i].m_uNdtempID,INDPR,fn,
						lo.m_NodalTemps[i].m_dNdtemp);
				}
				else
				{
					str.Format("%10u%5u%5u%10.4lf\n",lo.m_NodalTemps[i].m_uNdtempID,INDPR,fn,
						lo.m_NodalTemps[i].m_dNdtemp);
				}
				file->WriteString(str);
			}
		}*/
	}
	else   // ako nema zadatog potencijala stampa 0 u kartici 9
	{
		file->WriteString(CardH9);

		file->WriteString(CardH9_a);
		file->WriteString(CardV9_a);
		if (iINPT == 0)
			{
				str.Format("%5d\n",0);
			}
		else
			{
				str.Format("%10d\n",0);
			}
		file->WriteString(str);
	}
	return (-1);
}

UINT CModelData::ExportPAKP_Loads(CStdioFile* file, int iINPT)
{
	CString str;
	UINT i,j,k,uMAXTFT=0;
	HLoads lo;
	StructLoad sl;
	UINT uNQP = 0, uNHP = 0, uNHPMAX=0;
	UINT HFfuncs[MAX_TF],CONVfuncs[MAX_TF];
	bool ima;
	HElement el;
	long lGeneralDataPosQ;

	UINT surface_nodes_3d[6][8]={   {0,3,2,1,11,10,9,8},
									{4,5,6,7,16,17,18,19},
									{0,1,5,4,8,13,16,12},
									{1,2,6,5,9,14,17,13},
									{2,3,7,6,10,15,18,14},
									{3,0,4,7,11,12,19,15}};

// promenjeno April 2015.
	UINT surface_nodes_tetra[4][8]={{0,2,1,21,10,9,8,21},
									{0,1,4,21,8,13,12,21},
									{1,2,4,21,9,14,13,21},
									{2,0,4,21,10,12,14,21}};
/*
	UINT surface_nodes_prism[5][8]={{0,2,1,1,8,7,1,6},
									{3,4,5,5,12,13,5,14},
									{0,1,4,3,6,10,12,9},
									{1,2,5,4,7,11,13,10},
									{2,0,3,5,8,9,14,11}};

	UINT surface_nodes_tetra[4][8]={{0,2,1,1,6,5,1,4},
									{0,1,3,3,4,8,3,7},
									{1,2,3,3,5,9,3,8},
									{2,0,3,3,6,7,3,9}};
*/
	UINT surface_nodes_shell[2][8]={{0,3,2,1,7,6,5,4},
									{0,1,2,3,4,5,6,7}};

	UINT surface_nodes_tshell[2][8]={{0,2,1,3,6,5,4,7},
									{0,1,2,3,4,5,6,7}};

	UINT surface_nodes_2d[4][3]={   {0,1,4},
									{1,2,5},
									{2,3,6},
									{3,0,7}};

	UINT surface_nodes_t2d[3][3]={  {0,1,4},
									{1,2,5},
									{2,0,6}};



	if(m_LoadArray.GetSize()==0) 
	{
//		AfxMessageBox(NO_LOADS);
		return(0);
	}

	lo=m_LoadArray[0];	//Za sada samo prvi set

	for(i=0;i<(UINT)m_FunctionsArray.GetSize();i++)
		if((UINT)m_FunctionsArray[i].m_FunctionEntry.GetSize()>uMAXTFT)
			uMAXTFT=m_FunctionsArray[i].m_FunctionEntry.GetSize();
//	UINT HFfuncs[uMAXTFT], CONVfuncs[uMAXTFT];
//Odredjivanje NQP
	UINT uNQP_Loads=0;
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_HEAT_FL || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 uNQP_Loads++;
	 ima=false;
	 for(j=0;j<uNQP;j++) if(lo.m_StructLoads[i].m_uSl_funcID==HFfuncs[j]) {ima=true;break;}
	 if(!ima) {HFfuncs[uNQP]=lo.m_StructLoads[i].m_uSl_funcID;uNQP++;}
	}
//Odredjivanje NHP
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_CONV || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 ima=false;
	 for(j=0;j<uNHP;j++) if(lo.m_StructLoads[i].m_uSl_funcID==CONVfuncs[j]) {ima=true;break;}
	 if(!ima) {CONVfuncs[uNHP]=lo.m_StructLoads[i].m_uSl_funcID;uNHP++;}
  }

    file->WriteString(CardH11);
	file->WriteString(CardV11);
//	if((m_NodArray.GetSize())<=99999)
//	if(iINPT==0)
//		{
//			str.Format("%5u%5u%5u\n",uNQP_Loads,INDFS,INFILT); // TODO INDEFS treba da se ucitava iz kartice u translatoru
//		}
	//	else 
	//	{
			str.Format("%10u%5u%5u\n",uNQP_Loads,INDFS,INFILT);
//			str.Format("%5u%5u%5u\n",uNQP_Loads,INDFS,INFILT);
			//	}
	file->WriteString(str);

/*
//Card /12-2/ DATA ABOUT SURFACE FLUXES
	if(uNQP>0)
	{
	 file->WriteString(CardH11_2);
	 for(i=0;i<uNQP;i++)
	 {
		file->WriteString(CardV11_2_a_1);
		file->WriteString(CardV11_2_a_2);
		str.Format("%5u%5u\n",i+1,m_FunctionsArray[m_nFuncIndex[HFfuncs[i]]].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV11_2_b_1);
		file->WriteString(CardV11_2_b_2);
		for(j=0;j<m_FunctionsArray[m_nFuncIndex[HFfuncs[i]]].m_FunctionEntry.GetSize();j++)
		{
			str.Format("%10.4f%10.4f\n",m_FunctionsArray[m_nFuncIndex[HFfuncs[i]]].m_FunctionEntry[j].m_dX,
				m_FunctionsArray[m_nFuncIndex[HFfuncs[i]]].m_FunctionEntry[j].m_dY);
			file->WriteString(str);
		}
	 }
	}


//Card /12-4/ DATA ABOUT CONVECTION COEFFICIENT
	if(uNHP>0)
	{
	 file->WriteString(CardH11_4);
	 for(i=0;i<uNHP;i++)
	 {
		file->WriteString(CardV11_4_a_1);
		file->WriteString(CardV11_4_a_2);
		str.Format("%5u%5u\n",i+1,m_FunctionsArray[m_nFuncIndex[CONVfuncs[i]]].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV11_4_b_1);
		file->WriteString(CardV11_4_b_2);
		for(j=0;j<m_FunctionsArray[m_nFuncIndex[CONVfuncs[i]]].m_FunctionEntry.GetSize();j++)
		{
			str.Format("%10.4f%10.4f\n",m_FunctionsArray[m_nFuncIndex[CONVfuncs[i]]].m_FunctionEntry[j].m_dX,
				m_FunctionsArray[m_nFuncIndex[CONVfuncs[i]]].m_FunctionEntry[j].m_dY);
			file->WriteString(str);
		}
	 }
	}
*/


  if(uNQP_Loads>0)
  {
//Card /11-a i 11-b/
	UINT uNPP2=0,uNPP3=0;

	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
		sl=lo.m_StructLoads[i];
		if(sl.m_uLoadtype==FELT_ELEM_HEAT_FL)
		{
			j=sl.m_uDof_face[0]-1;
			if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
			{
			 if(j<2) uNPP3++;
			 else
			 {
				el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
				if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20|| el.m_uTopology==FETO_TETRA10) uNPP3++;
				else uNPP2++;
			 } 
			}
		}
	}

	//Card 11-a
	//Line pressure
		if(uNPP2>0)
		{
			UINT uITIPE;
			file->WriteString(CardH11_a);
			file->WriteString(CardV11_a);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_ELEM_HEAT_FL)
				{
				   el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
				   if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8 ||
					  el.m_uTopology==FETO_TRI3 || el.m_uTopology==FETO_TRI6)
				   {
					j=sl.m_uDof_face[0]-1;
					if(j>1 && fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
					{
						HProperties pr = m_PropArray[m_nPropIndex[el.m_uPropID]];
					
						switch (el.m_uType)
						{
							case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR:
							case FET_PLATE_LIN:case FET_PLATE_PAR: uITIPE=0;
							break;
							case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR: uITIPE=2;
							break;
							case FET_AXISYM_LIN:case FET_AXISYM_PAR: uITIPE=1;
							break;
						}
						
						//UINT nfun;
						//nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
						//p_str.Format("%5u%5u%5u%10.4.1e%10.4.1e%10.4.1e%5u\n",uITIPE,nfun,IPRAV,
						//	sl.m_dValue[0],sl.m_dValue[0],pr.m_dValue[0],KORC);
//   						if((m_NodArray.GetSize())<=99999)
						if(iINPT==0)
							{
								str.Format("%5d",el.m_uID);
							}
						else 
							{
								str.Format("%10d",el.m_uID);
							}
						file->WriteString(str);
						for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 2:3);k++)
						{
							if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
 //  								if((m_NodArray.GetSize())<=99999)
								if(iINPT==0)
									{
										str.Format("%5u",el.m_uNode[surface_nodes_2d[j-2][k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[surface_nodes_2d[j-2][k]]);
									}
							else
							if (iINPT == 0)
									{
										str.Format("%5u",el.m_uNode[surface_nodes_t2d[j-2][k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[surface_nodes_t2d[j-2][k]]);
									}
							file->WriteString(str);
						}
/*						
						UINT q=0;
						for(k=0;k<uNQP;k++)
							if(HFfuncs[k]==sl.m_uSl_funcID) break;
						q=k+1;

						str.Format("%5d%10.6f%10.6f\n",q,sl.m_dValue[0],sl.m_dValue[0]);
*/
//   								if((m_NodArray.GetSize())<=99999)
								if(iINPT==0)
									{
										str.Format("%5d%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0]);
									}
								else 
									{
										str.Format("%10d%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0]);
									}
						file->WriteString(str);
					}
				   }
				}
			}
		}
		
	//Card 11-b
	//Surface pressure
		if(uNPP3>0)
		{
			file->WriteString(CardH11_b);
			file->WriteString(CardV11_b);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_ELEM_HEAT_FL)
				{
					el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					j=sl.m_uDof_face[0]-1;
					 if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
					 {
				//		UINT nfun;
				//		nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
						
						switch(el.m_uTopology)
						{
						case FETO_BRICK8: case FETO_BRICK20:
						{
				//			p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
				//		 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
//							if((m_NodArray.GetSize())<=99999)
							if(iINPT==0)
								{
									str.Format("%5d",el.m_uID);
								}
							else 
								{
									str.Format("%10d",el.m_uID);
								}
							file->WriteString(str);
							for(k=0;k<(UINT)(el.m_uTopology==FETO_BRICK8 ? 4:8);k++)
							{
//								if((m_NodArray.GetSize())<=99999)
								if(iINPT==0)
									{
										str.Format("%5u",el.m_uNode[surface_nodes_3d[j][k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[surface_nodes_3d[j][k]]);
									}
								file->WriteString(str);
							}
/*
							UINT q=0;
							for(k=0;k<uNQP;k++)
								if(HFfuncs[k]==sl.m_uSl_funcID) break;
							q=k+1;
							str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n",q,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0]);
*/
//							if((m_NodArray.GetSize())<=99999)
							// promenjen format stampa 8 cvorova za fluks kod osmocvornih elemenata
							if(el.m_uTopology==FETO_BRICK8)
								{
									int node48=0;
									if(iINPT==0)
										{
										str.Format("%5d%5d%5d%5d",node48,node48,node48,node48);
										}
									else 
										{
										str.Format("%10d%10d%10d%10d",node48,node48,node48,node48);
										}
								}
								file->WriteString(str);
							if(iINPT==0)
								{
								str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0]);
								}
							else 
								{
								str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0]);
								}
							file->WriteString(str);
							}
							break;
							case FETO_TETRA4: case FETO_TETRA10:
							{
//								if((m_NodArray.GetSize())<=99999)
								if(iINPT==0)
									{
										str.Format("%5d",el.m_uID);
									}
								else 
									{
										str.Format("%10d",el.m_uID);
									}
								file->WriteString(str);
								for(k=0;k<(UINT)(el.m_uTopology==FETO_TETRA4 ? 4:8);k++)
								{
//									if((m_NodArray.GetSize())<=99999)
									if(iINPT==0)
										{
										str.Format("%5u",el.m_uNode[surface_nodes_tetra[j][k]]);
										}
									else
										{
										str.Format("%10u",el.m_uNode[surface_nodes_tetra[j][k]]);
										}
									file->WriteString(str);
								}
				//				file->WriteString("\n");
								double press4=0.0;
//								if((m_NodArray.GetSize())<=99999)
								if(iINPT==0)
									{
									str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],press4);
									}
								else 
									{
									str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],press4);
									}
								file->WriteString(str);
							}
								break;
						case FETO_QUAD4: case FETO_QUAD8: case FETO_TRI3: case FETO_TRI6:
							if(j<2)
							{
				//				p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
				//			 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
//							if((m_NodArray.GetSize())<=99999)
							if(iINPT==0)
								{
									str.Format("%5d",el.m_uID);
								}
							else 
								{
									str.Format("%10d",el.m_uID);
								}
								file->WriteString(str);
								for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 4:8);k++)
								{
			 						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
//										if((m_NodArray.GetSize())<=99999)
										if(iINPT==0)
											{
												str.Format("%5u",el.m_uNode[surface_nodes_shell[j][k]]);
											}
										else 
											{
												str.Format("%10u",el.m_uNode[surface_nodes_shell[j][k]]);
											}
									else
	//									if((m_NodArray.GetSize())<=99999)
										if(iINPT==0)
										{
												str.Format("%5u",el.m_uNode[surface_nodes_tshell[j][k]]);
											}
										else 
											{
												str.Format("%10u",el.m_uNode[surface_nodes_tshell[j][k]]);
											}
									file->WriteString(str);
								}
/*							
								UINT q=0;
								for(k=0;k<uNQP;k++)
									if(HFfuncs[k]==sl.m_uSl_funcID) break;
								q=k+1;
								str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n",q,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0]);
*/
//								if((m_NodArray.GetSize())<=99999)
								if(iINPT==0)
									{
										str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0]);
									}
								else 
									{
										str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0]);
									}
								file->WriteString(str);
							}
							break;
						}
					 }
				}
			}
		}
	}




//Card /12/
    file->WriteString(CardH12);
	file->WriteString(CardV12);
//		if((m_NodArray.GetSize())<=99999)

/*	if(iINPT==0)
			{
				str.Format("%5u%5u\n",uNHP,m_ElArray.GetSize());
			}
		else 
			{
				str.Format("%5u%10u\n",uNHP,m_ElArray.GetSize());
			}
		if(	uNHP==0) str.Format("%5u%5u\n",uNHP,0);
	file->WriteString(str); */
// naknadno upisivanje maksimalnog broja elemenata u konturama
	lGeneralDataPosQ = file->GetPosition();
	str.Format("               \n");
	file->WriteString(str);

  if(uNHP>0)
  {
    file->WriteString(CardH12_1);
//Petlja po konturama. Konture se razlikuju po funkcijama
	UINT ci;
	for(ci=0;ci<uNHP;ci++)
	{
	    file->WriteString(CardH12_1_a);
	    file->WriteString(CardV12_1_a);
		//Odredjivanje broja elemenata u konturi i koliko ima 2D i 3D
		UINT ContFun=CONVfuncs[ci];
		UINT uNMAX=0;
		UINT uNPP2=0,uNPP3=0;
		for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
		{
			sl=lo.m_StructLoads[i];
			if(sl.m_uLoadtype==FELT_ELEM_CONV && sl.m_uSl_funcID==ContFun)
			{
				uNMAX++;
				j=sl.m_uDof_face[0]-1;
				if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
				{
				 if(j<2) uNPP3++;
				 else
				 {
					el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20 || el.m_uTopology==FETO_TETRA10) uNPP3++;
					else uNPP2++;
				 } 
				}
			}
		}
		if (uNHPMAX<uNPP3) uNHPMAX = uNPP3;
		if (uNHPMAX<uNPP2) uNHPMAX = uNPP2;
// za ID konture stampa ID funkcije za h iz Femapa
		str.Format("%5u%5u\n", ContFun, uNMAX);
//		str.Format("%5u%5u\n", ci + 1, uNMAX);
		file->WriteString(str);

	
		//Card 12-b
	//Line pressure
		if(uNPP2>0)
		{
			UINT uITIPE;
			file->WriteString(CardH12_1_b);
			file->WriteString(CardV12_1_b);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_ELEM_CONV && sl.m_uSl_funcID==ContFun)
				{
				   el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
				   if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8 ||
					  el.m_uTopology==FETO_TRI3 || el.m_uTopology==FETO_TRI6)
				   {
					j=sl.m_uDof_face[0]-1;
					if(j>1 && fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
					{
						HProperties pr = m_PropArray[m_nPropIndex[el.m_uPropID]];
					
						switch (el.m_uType)
						{
							case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR:
							case FET_PLATE_LIN:case FET_PLATE_PAR: uITIPE=0;
							break;
							case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR: uITIPE=2;
							break;
							case FET_AXISYM_LIN:case FET_AXISYM_PAR: uITIPE=1;
							break;
						}
						
						//UINT nfun;
						//nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
						//p_str.Format("%5u%5u%5u%10.4.1e%10.4.1e%10.4.1e%5u\n",uITIPE,nfun,IPRAV,
						//	sl.m_dValue[0],sl.m_dValue[0],pr.m_dValue[0],KORC);
						if (iINPT == 0)
						{
							str.Format("%5d",el.m_uID);
						}
						else
						{
							str.Format("%10d",el.m_uID);
						}
						file->WriteString(str);
						for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 2:3);k++)
						{
							if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
							{
								if (iINPT == 0)
								{
									str.Format("%5u",el.m_uNode[surface_nodes_2d[j-2][k]]);
								}
								else
								{
									str.Format("%10u",el.m_uNode[surface_nodes_2d[j-2][k]]);
								}
							}	
							else
							{
								if (iINPT == 0)
								{
									str.Format("%5u",el.m_uNode[surface_nodes_t2d[j-2][k]]);
								}
								else
								{
									str.Format("%10u",el.m_uNode[surface_nodes_t2d[j-2][k]]);
								}
							}
							file->WriteString(str);
						}
						file->WriteString("\n");
					}
				   }
				}
			}
		}
		
	//Card 12-c
	//Surface pressure
		if(uNPP3>0)
		{
			file->WriteString(CardH12_1_c);
			file->WriteString(CardV12_1_c);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_ELEM_CONV && sl.m_uSl_funcID==ContFun)
				{
					el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					j=sl.m_uDof_face[0]-1;
					 if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
					 {
				//		UINT nfun;
				//		nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
						
						switch(el.m_uTopology)
						{
						case FETO_BRICK8: case FETO_BRICK20:
						{
				//			p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
				//		 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
							if (iINPT == 0)
							{
								str.Format("%5d",el.m_uID);
							}
							else
							{
								str.Format("%10d",el.m_uID);
							}
							file->WriteString(str);
//							for(k=0;k<(UINT)(el.m_uTopology==FETO_BRICK8 ? 4:8);k++)
							for(k=0;k<8;k++)
							{
								if (iINPT == 0)
								{
									str.Format("%5u",el.m_uNode[surface_nodes_3d[j][k]]);
								}
								else
								{
									str.Format("%10u",el.m_uNode[surface_nodes_3d[j][k]]);
								}
								file->WriteString(str);
							}

							file->WriteString("\n");

							}
							break;
						case FETO_TETRA10:
						{
							if (iINPT == 0)
							{
								str.Format("%5d",el.m_uID);
							}
							else
							{
								str.Format("%10d",el.m_uID);
							}
							file->WriteString(str);
							for(k=0;k<8;k++)
							{
								if (iINPT == 0)
								{
									str.Format("%5u",el.m_uNode[surface_nodes_tetra[j][k]]);
								}
								else
								{
									str.Format("%10u",el.m_uNode[surface_nodes_tetra[j][k]]);
								}
								file->WriteString(str);
							}

							file->WriteString("\n");
							}
							break;
						case FETO_QUAD4: case FETO_QUAD8: case FETO_TRI3: case FETO_TRI6:
							if(j<2)
							{
				//				p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
				//			 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
								if (iINPT == 0)
								{
									str.Format("%5d",el.m_uID);
								}
								else
								{
									str.Format("%10d",el.m_uID);
								}
								file->WriteString(str);
//								for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 4:8);k++)
								for(k=0;k<8;k++)
								{
			 						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
									{
										if (iINPT == 0)
										{
											str.Format("%5u",el.m_uNode[surface_nodes_shell[j][k]]);
										}
										else
										{
											str.Format("%10u",el.m_uNode[surface_nodes_shell[j][k]]);
										}
										file->WriteString(str);
									}
									else
									{
										if (iINPT == 0)
										{
											str.Format("%5u",el.m_uNode[surface_nodes_tshell[j][k]]);
										}
										else
										{
											str.Format("%10u",el.m_uNode[surface_nodes_tshell[j][k]]);
										}
									file->WriteString(str);
									}
								}
								file->WriteString("\n");
							}
							break;
						}
					 }
				}
			}
		}

	}

  }
  //Upisivanje naknadno max. broja kontura i max. broja elemenata na konturi

  file->Seek(lGeneralDataPosQ, CFile::begin);
  if (iINPT == 0)
  {
	  str.Format("%5u%5u     \n", uNHP, uNHPMAX);
  }
  else
  {
	  str.Format("%5u%10u\n", uNHP, uNHPMAX);
  }
//  if (uNHP == 0) str.Format("%5u%5u\n", uNHP, 0);
  file->WriteString(str); 
  file->Seek(0, CFile::end);



/*
//Card /13-3/

	file->WriteString(CardH13_3);
	file->WriteString(CardV13_3);

	for(i=0;i<lo.m_StructLoads.GetSize();i++)
	{
		sl=lo.m_StructLoads[i];
		if(sl.m_uLoadtype==FELT_ELEM_HEAT_FL ||
			sl.m_uLoadtype==FELT_ELEM_CONV)
		{
			el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
			for(j=0;j<6;j++)
			 if(sl.m_uDof_face[j] && fabs(sl.m_dValue[j])>=LOAD_FORCE_TOL)
			 {
			  str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u\n",sl.m_uLoadID,
				el.m_uNode[surface_nodes_3d[sl.m_uDof_face[j]][0]],
				el.m_uNode[surface_nodes_3d[sl.m_uDof_face[j]][1]],
				el.m_uNode[surface_nodes_3d[sl.m_uDof_face[j]][2]],
				el.m_uNode[surface_nodes_3d[sl.m_uDof_face[j]][3]],
				(sl.m_uLoadtype==FELT_ELEM_HEAT_FL ? sl.m_uSl_funcID:0),
				(sl.m_uLoadtype==FELT_ELEM_CONV ? sl.m_uSl_funcID:0),
				ITO,IHR,FO);
			  file->WriteString(str);
			  file->WriteString(str);
			 }
		}
	}

*/
	return (-1);
}

UINT CModelData::ExportPAKP_MatModels(CStdioFile* file)
{

	CString str;
	CPAKString pstr;
	UINT i, INDPK=0;


	//Card /13/
	file->WriteString(CardH13);
	file->WriteString(CardV13);

	str.Format("%5u\n",m_MaterialsArray.GetSize());
	file->WriteString(str);

	//Card /13-1/
	file->WriteString(CardH13_1);

	file->WriteString(CardV13_1_1);
	file->WriteString(CardV13_1_2);
///////////   2D //////////////////////
	for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	{
//		pstr.Format("%10.4.1e%10.4.1e%10.4.1e%10.4.1e%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u%10.4.1e\n",
		pstr.Format("%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e%5u%10.4.2e%5u\n",
			m_MaterialsArray[i].m_dK[0],m_MaterialsArray[i].m_dK[0],m_MaterialsArray[i].m_dK[0],
			m_MaterialsArray[i].m_dThermal_cap,m_MaterialsArray[i].m_dK[1],m_MaterialsArray[i].m_dK[1],
			m_MaterialsArray[i].m_dK[1],m_MaterialsArray[i].m_dThermal_cap,
			INDPK,m_MaterialsArray[i].m_dTemperature,i+1);
		file->WriteString(pstr);
	}
////////     3D /////////////////// TODO uradi za sada je sve ovo harcode-ovano
	///      Parametar NETIP iz kartice 8 koji odredjuje da li je 2d ili 3d je hardcode-ovan
	/// tako da ako bih i napravio uslov koji proverava koliko je NETIP to bi i dalje bilo hardcode-ovano
	/// dakle prvo treba NETIP da ucitam iz FEMAPa

	/*for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	{
		pstr.Format("%10.4.1e%10.4.1e%10.4.1e%10.4.1e%10.4.1e\n",
			m_MaterialsArray[i].m_dK[0],m_MaterialsArray[i].m_dK[0],m_MaterialsArray[i].m_dK[0],
			m_MaterialsArray[i].m_dThermal_cap,m_MaterialsArray[i].m_dDensity);
		file->WriteString(pstr);
	}*/


	return(-1);
}
