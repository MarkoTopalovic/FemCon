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


UINT CModelData::ExportPAK_PT(CStdioFile* file,CPakExpOpt *peo)
{

	CString str;
	int uNPER,iINPT,iICCGG,iPAKT;
	UINT uNGELEM;
	UINT i;
	long lGeneralDataPos;

	iINPT = 0;
	iICCGG = 0;
	iPAKT = 0;

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
// stavljeno za kalibraciju lamela da je format I10
//		if((m_NodArray.GetSize())>99999)
//		{
			iINPT=1;
			iICCGG=-11;
//		}
//Card /2/
	file->WriteString(CardH2);
	file->WriteString(CardV2);
	str.Format("%5u%5u     %5u\n",INDFOR,iINPT,iPAKT);
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
	ExportPAKPT_PrescTemp(file, iINPT);

//Card /10/ stampa pocetnu temperaturu - dodato stampanje u ExportPAKPT_Loads
/*	file->WriteString(CardH10tp);
	file->WriteString(CardV10tp);
	str.Format("                              %10.6f%5d%5d%5d%10.6f\n", GAMA, IOSA, ISIL, IFIL, ZASIC);
	file->WriteString(str); */

//Card /11/
	ExportPAKPT_Loads(file, iINPT);

/*
    file->WriteString (CardH9);
    file->WriteString (CardV9);

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
//Card /12-6/
	file->WriteString (CardH12_6);
	file->WriteString (CardV12_6);
    str.Format ("%10.6f\n",m_MaterialsArray[0].m_dTemperature);
    file->WriteString (str);
*/

//Card /13/
	ExportPAKPT_MatModels(file);

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
	if(iINPT==0)
		{
			str.Format("%5u               %5u%5u%5u%5u",m_NodArray.GetSize(),uNPER,NPRINT,(m_PakOptValue.GetDynamicAnIsSet() ? 0:1),IVDP);
		}
	else 
		{
			str.Format("%10u               %5u%5u%5u%5u",m_NodArray.GetSize(),uNPER,NPRINT,(m_PakOptValue.GetDynamicAnIsSet() ? 0:1),IVDP);
		}

	file->WriteString(str);
	file->Seek(0,CFile::end);

	return(-1);
}

//**************************************************************************
//**************************************************************************
// Ova funkcija eksportuje sve elemente u jednu grupu. Od juna 2015. u PAKPu svi elementi se zapisiju u jednu grupu
//**************************************************************************

UINT CModelData::ExportPAKPT_PrescTemp(CStdioFile* file, int iINPT)
{
	HLoads lo;
	UINT i;
	CString str;

	if(m_LoadArray.GetSize()>0 && m_LoadArray[0].m_NodalTemps.GetSize())
	{
		lo=m_LoadArray[0];
		file->WriteString(CardH9t);

		file->WriteString(CardH9_at);
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
				if (iINPT == 0)
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

UINT CModelData::ExportPAKPT_Loads(CStdioFile* file, int iINPT)
{
	CString str;
	UINT i,j,k,uMAXTFT=0;
	HLoads lo;
	StructLoad sl;
	UINT uNQP = 0, uNHP = 0, uNTOK = 0, uNQE = 0, uNHR = 0;
	UINT HFfuncs[MAX_TF], CONVfuncs[MAX_TF], HGfuncs[MAX_TF], RADfuncs[MAX_TF], TOKfuncs[MAX_TF];
	bool ima;
	HElement el;
	HNodes nn;
	double tez[3];

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

// pakt
		UINT uNQP_Loads = 0, uNHP_Loads = 0, uNQE_Loads = 0, uNHR_Loads = 0;
//Odredjivanje NQE - toplotni izvor u elementu
		for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
		{
			if (lo.m_StructLoads[i].m_uLoadtype != FELT_ELEM_HEAT_GEN || !lo.m_StructLoads[i].m_uSl_funcID) continue;
			uNQE_Loads++;
			ima = false;
			for (j = 0; j<uNQE; j++) if (lo.m_StructLoads[i].m_uSl_funcID == HGfuncs[j]) { ima = true; break; }
			if (!ima) { HGfuncs[uNQE] = lo.m_StructLoads[i].m_uSl_funcID; uNQE++; }
		}
//Odredjivanje NQP - fluks
		for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
		{
			if (lo.m_StructLoads[i].m_uLoadtype != FELT_ELEM_HEAT_FL || !lo.m_StructLoads[i].m_uSl_funcID) continue;
			uNQP_Loads++;
			ima = false;
			for (j = 0; j<uNQP; j++) if (lo.m_StructLoads[i].m_uSl_funcID == HFfuncs[j]) { ima = true; break; }
			if (!ima) { HFfuncs[uNQP] = lo.m_StructLoads[i].m_uSl_funcID; uNQP++; }
		}
//Odredjivanje NHP - prelaznost
		for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
		{
//			int test = 0;
//			test = lo.m_StructLoads[i].m_uLoadtype;
//			test = FELT_ELEM_CONV;
//			test = lo.m_StructLoads[i].m_uSl_funcID;

			if (lo.m_StructLoads[i].m_uLoadtype != FELT_ELEM_CONV || !lo.m_StructLoads[i].m_uSl_funcID) continue;
			uNHP_Loads++;
			ima = false;
			for (j = 0; j<uNHP; j++) if (lo.m_StructLoads[i].m_uSl_funcID == CONVfuncs[j]) { ima = true; break; }
			if (!ima) { CONVfuncs[uNHP] = lo.m_StructLoads[i].m_uSl_funcID; uNHP++; }

			if (lo.m_StructLoads[i].m_uLoadtype != FELT_ELEM_CONV || !lo.m_StructLoads[i].m_uAddI_fnc[0]) continue;
			ima = false;
			for (j = 0; j<uNTOK; j++) if (lo.m_StructLoads[i].m_uAddI_fnc[0] == TOKfuncs[j]) { ima = true; break; }
			if (!ima) { TOKfuncs[uNTOK] = lo.m_StructLoads[i].m_uAddI_fnc[0]; uNTOK++; }
		}
//Odredjivanje NHR - zracenje
		for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
		{
			if (lo.m_StructLoads[i].m_uLoadtype != FELT_ELEM_RADIATION || !lo.m_StructLoads[i].m_uSl_funcID) continue;
			uNHR_Loads++;
			ima = false;
			for (j = 0; j<uNHR; j++) if (lo.m_StructLoads[i].m_uSl_funcID == RADfuncs[j]) { ima = true; break; }
			if (!ima) { RADfuncs[uNHR] = lo.m_StructLoads[i].m_uSl_funcID; uNHR++; }

	/*		if (lo.m_StructLoads[i].m_uLoadtype != FELT_ELEM_RADIATION || !lo.m_StructLoads[i].m_uAddI_fnc[0]) continue;
			ima = false;
			for (j = 0; j<uNTOK; j++) if (lo.m_StructLoads[i].m_uAddI_fnc[0] == TOKfuncs[j]) { ima = true; break; }
			if (!ima) { TOKfuncs[uNTOK] = lo.m_StructLoads[i].m_uAddI_fnc[0]; uNTOK++; } */
		}

// kraj dela iz PAKT-a
	//Card /10/ INITIAL TEMPERATURE
//		if (m_PakOptValue.GetDynamicAnIsSet())
//		{
			file->WriteString(CardH10tp);
			file->WriteString(CardV10tp);
			str.Format("%10.6f\n", lo.m_dDef_temp);
			//     str.Format ("%10.6f\n",m_MaterialsArray[0].m_dTemperature);
			file->WriteString(str);
//		}

	file->WriteString(CardH11);
	file->WriteString(CardV11t);
//	if((m_NodArray.GetSize())<=99999)
	if(iINPT==0)
		{
		str.Format("%5u%5u%5u\n", uNQP_Loads, uNHP_Loads, uNHR_Loads); // TODO INDEFS treba da se ucitava iz kartice u translatoru
		}
	else 
		{
//			str.Format("%10u%5u%5u\n",uNQP_Loads,INDFS,INFILT);
		str.Format("%10u%10u%10u\n", uNQP_Loads, uNHP_Loads, uNHR_Loads);
		}
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

// elementi na kojima je zadat fluks
  if(uNQP_Loads>0)
  {
//Card /11-a i 11-b/
	UINT uNPP2=0,uNPP3=0;
	file->WriteString(CardH11_a1);

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
//								if(iINPT==0)  //format funkcije ne treba da zavisi od broja cvorova
									//{
										str.Format("%5d%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0]);
//									}
//								else 
//									{
//										str.Format("%10d%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0]);
//									}
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
/*							if(iINPT==0)
								{*/
								str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0]);
/*								}
							else 
								{
								str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n",sl.m_uSl_funcID,sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0]);
								} */
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
// ******************************************************
// elementi na kojima je zadata prelaznost
// ******************************************************
	if (uNHP_Loads>0)
	{
		//Card /11-a i 11-b/
		UINT uNPP2 = 0, uNPP3 = 0;
		file->WriteString(CardH11_ac1);

		for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
		{
			sl = lo.m_StructLoads[i];
			if (sl.m_uLoadtype == FELT_ELEM_CONV)
			{
				j = sl.m_uDof_face[0] - 1;
				if (fabs(sl.m_dValue[0]) >= LOAD_PRESS_TOL)
				{
					if (j<2) uNPP3++;
					else
					{
						el = m_ElArray[m_nElemIndex[sl.m_uLoadID]];
						if (el.m_uTopology == FETO_BRICK8 || el.m_uTopology == FETO_BRICK20 || el.m_uTopology == FETO_TETRA10) uNPP3++;
						else uNPP2++;
					}
				}
			}
		}
	//Card 11-a
	//Line pressure
		if (uNPP2>0)
		{
			UINT uITIPE;
			file->WriteString(CardH11_a);
			file->WriteString(CardV11_a);
			for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
			{
				sl = lo.m_StructLoads[i];
				if (sl.m_uLoadtype == FELT_ELEM_CONV)
				{
					el = m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					if (el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_QUAD8 ||
						el.m_uTopology == FETO_TRI3 || el.m_uTopology == FETO_TRI6)
					{
						j = sl.m_uDof_face[0] - 1;
						if (j>1 && fabs(sl.m_dValue[0]) >= LOAD_PRESS_TOL)
						{
							HProperties pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

							switch (el.m_uType)
							{
							case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR:
							case FET_PLATE_LIN:case FET_PLATE_PAR: uITIPE = 0;
								break;
							case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR: uITIPE = 2;
								break;
							case FET_AXISYM_LIN:case FET_AXISYM_PAR: uITIPE = 1;
								break;
							}

							//UINT nfun;
							//nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
							//p_str.Format("%5u%5u%5u%10.4.1e%10.4.1e%10.4.1e%5u\n",uITIPE,nfun,IPRAV,
							//	sl.m_dValue[0],sl.m_dValue[0],pr.m_dValue[0],KORC);
							//   						if((m_NodArray.GetSize())<=99999)
							if (iINPT == 0)
							{
								str.Format("%5d", el.m_uID);
							}
							else
							{
								str.Format("%10d", el.m_uID);
							}
							file->WriteString(str);
							for (k = 0; k<(UINT)((el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_TRI3) ? 2 : 3); k++)
							{
								if (el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_QUAD8)
									if (iINPT == 0)
									{
										str.Format("%5u", el.m_uNode[surface_nodes_2d[j - 2][k]]);
									}
									else
									{
										str.Format("%10u", el.m_uNode[surface_nodes_2d[j - 2][k]]);
									}
								else
									if (iINPT == 0)
									{
										str.Format("%5u", el.m_uNode[surface_nodes_t2d[j - 2][k]]);
									}
									else
									{
										str.Format("%10u", el.m_uNode[surface_nodes_t2d[j - 2][k]]);
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
					/*		if (iINPT == 0)
							{*/
								str.Format("%5d%5d\n", sl.m_uSl_funcID, sl.m_uAddI_fnc[0]);
						/*	}
							else
							{
								str.Format("%10d%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0]);
							} */
							file->WriteString(str);
						}
					}
				}
			}
		}

		//Card 11-b
		//Surface pressure
		if (uNPP3>0)
		{
			file->WriteString(CardH11_b);
			file->WriteString(CardV11_bc);
			for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
			{
				sl = lo.m_StructLoads[i];
				if (sl.m_uLoadtype == FELT_ELEM_CONV)
				{
					el = m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					j = sl.m_uDof_face[0] - 1;
					if (fabs(sl.m_dValue[0]) >= LOAD_PRESS_TOL)
					{
						//		UINT nfun;
						//		nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);

						switch (el.m_uTopology)
						{
						case FETO_BRICK8: case FETO_BRICK20:
						{
											  //			p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
											  //		 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
											  if (iINPT == 0)
											  {
												  str.Format("%5d", el.m_uID);
											  }
											  else
											  {
												  str.Format("%10d", el.m_uID);
											  }
											  file->WriteString(str);
											  for (k = 0; k<(UINT)(el.m_uTopology == FETO_BRICK8 ? 4 : 8); k++)
											  {
												  if (iINPT == 0)
												  {
													  str.Format("%5u", el.m_uNode[surface_nodes_3d[j][k]]);
												  }
												  else
												  {
													  str.Format("%10u", el.m_uNode[surface_nodes_3d[j][k]]);
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
											  // promenjen format stampa 8 cvorova za fluks kod osmocvornih elemenata
											  if (el.m_uTopology == FETO_BRICK8)
											  {
												  int node48 = 0;
												  if (iINPT == 0)
												  {
													  str.Format("%5d%5d%5d%5d", node48, node48, node48, node48);
												  }
												  else
												  {
													  str.Format("%10d%10d%10d%10d", node48, node48, node48, node48);
												  }
											  }
											  file->WriteString(str);
											  /*if (iINPT == 0)
											  { */
												  str.Format("%5d%5d\n", sl.m_uSl_funcID, sl.m_uAddI_fnc[0]);
											  /*}
											  else
											  {
												  str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0]);
											  }*/
											  file->WriteString(str);
						}
							break;
						case FETO_TETRA4: case FETO_TETRA10:
						{
											  if (iINPT == 0)
											  {
												  str.Format("%5d", el.m_uID);
											  }
											  else
											  {
												  str.Format("%10d", el.m_uID);
											  }
											  file->WriteString(str);
											  // odredjivanje težišta stranice; stampa se vertikalna koordinata težišta
											  UINT cvor;
											  tez[0] = 0.0;
											  tez[1] = 0.0;
											  tez[2] = 0.0;
											  for (k = 0; k<(UINT)(el.m_uTopology == FETO_TETRA4 ? 4 : 8); k++)
											  {
												  if (iINPT == 0)
												  {
													  str.Format("%5u", el.m_uNode[surface_nodes_tetra[j][k]]);
												  }
												  else
												  {
													  str.Format("%10u", el.m_uNode[surface_nodes_tetra[j][k]]);
												  }
												  file->WriteString(str);
												  cvor = el.m_uNode[surface_nodes_tetra[j][k]];
												  if (cvor != 0)
												  {
	//												  nn = m_NodArray[el.m_uNode[surface_nodes_tetra[j][k]]];
													  nn = m_NodArray[cvor-1];
//													  str.Format("%10u%10u%10.4f\n", cvor, nn, nn.m_dZ);
													  //			  tez[0] = tez[0] + nn.m_dX;
													  //			  tez[1] = tez[1] + nn.m_dY;
 													  tez[2] = tez[2] + nn.m_dZ;
												  }
											  }
	//										  file->WriteString(str);
									  //				file->WriteString("\n");
										//	  tez[0] = tez[0] / k;
										//	  tez[1] = tez[1] / k;
		//									  str.Format("%10.4f%10u\n", tez[2]);
			//								  file->WriteString(str);
											  tez[2] = tez[2] / 6;
											  int IPROM,IFH,IFTOK;
											  IPROM = 0;
											  IFH = sl.m_uSl_funcID;
											  IFTOK = sl.m_uAddI_fnc[0];
											  if (sl.m_uAddI_fnc[0] == 2)
											  {
												  IPROM = 2;
												  IFH = 1000001;
												  IFTOK = 3;
											  }
											  else if (sl.m_uAddI_fnc[0] == 4)
											  {
												  IPROM = 3;
												  IFTOK = 3;
												  IFH = 1002001;
											  }
											  else if (sl.m_uAddI_fnc[0] == 6)
											  {
												  IPROM = 2;
												  IFTOK = 3;
												  IFH = 2000001;
											  }
											  else if (sl.m_uAddI_fnc[0] == 8)
											  {
												  IPROM = 3;
												  IFH = 2002001;
												  IFTOK = 3;
											  }
											  else if (sl.m_uAddI_fnc[0] == 10)
											  {
												  IFH = 2;
												  IFTOK = 3;
											  }  
											  if (iINPT == 0)
											  {
											     str.Format("%10u%5u%5d%10.4f\n", IFH, IFTOK, IPROM, tez[2]);
											//	  str.Format("%5u%5u%5u%10.4f\n", sl.m_uSl_funcID, sl.m_uAddI_fnc[0], IPROM, tez[2]);
											   }
											  else
											  {
												  str.Format("%10u%10u%5d%10.4f\n", IFH, IFTOK, IPROM, tez[2]);
											  }
											  file->WriteString(str);
						}
							break;
						case FETO_QUAD4: case FETO_QUAD8: case FETO_TRI3: case FETO_TRI6:
							if (j<2)
							{
								//				p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
								//			 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
								if (iINPT == 0)
								{
									str.Format("%5d", el.m_uID);
								}
								else
								{
									str.Format("%10d", el.m_uID);
								}
								file->WriteString(str);
								for (k = 0; k<(UINT)((el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_TRI3) ? 4 : 8); k++)
								{
									if (el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_QUAD8)
										if (iINPT == 0)
										{
											str.Format("%5u", el.m_uNode[surface_nodes_shell[j][k]]);
										}
										else
										{
											str.Format("%10u", el.m_uNode[surface_nodes_shell[j][k]]);
										}
									else
										if (iINPT == 0)
										{
											str.Format("%5u", el.m_uNode[surface_nodes_tshell[j][k]]);
										}
										else
										{
											str.Format("%10u", el.m_uNode[surface_nodes_tshell[j][k]]);
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
								/*if (iINPT == 0)
								{*/
									str.Format("%5d%5d\n", sl.m_uSl_funcID, sl.m_uAddI_fnc[0]);
								/*}
								else
								{
									str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0]);
								} */
								file->WriteString(str);
							}
							break;
						}
					}
				}
			}
		}
	}

// elementi na kojima je zadato zracenje
	if (uNHR_Loads>0)
	{
		//Card /11-a i 11-b/
		UINT uNPP2 = 0, uNPP3 = 0;
		file->WriteString(CardH11_ar1);

		for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
		{
			sl = lo.m_StructLoads[i];
			if (sl.m_uLoadtype == FELT_ELEM_RADIATION)
			{
				j = sl.m_uDof_face[0] - 1;
				if (fabs(sl.m_dValue[0]) >= LOAD_PRESS_TOL)
				{
					if (j<2) uNPP3++;
					else
					{
						el = m_ElArray[m_nElemIndex[sl.m_uLoadID]];
						if (el.m_uTopology == FETO_BRICK8 || el.m_uTopology == FETO_BRICK20 || el.m_uTopology == FETO_TETRA10) uNPP3++;
						else uNPP2++;
					}
				}
			}
		}


		//Card 11-a
		//Line pressure
		if (uNPP2>0)
		{
			UINT uITIPE;
			file->WriteString(CardH11_a);
			file->WriteString(CardV11_a);
			for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
			{
				sl = lo.m_StructLoads[i];
				if (sl.m_uLoadtype == FELT_ELEM_RADIATION)
				{
					el = m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					if (el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_QUAD8 ||
						el.m_uTopology == FETO_TRI3 || el.m_uTopology == FETO_TRI6)
					{
						j = sl.m_uDof_face[0] - 1;
						if (j>1 && fabs(sl.m_dValue[0]) >= LOAD_PRESS_TOL)
						{
							HProperties pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

							switch (el.m_uType)
							{
							case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR:
							case FET_PLATE_LIN:case FET_PLATE_PAR: uITIPE = 0;
								break;
							case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR: uITIPE = 2;
								break;
							case FET_AXISYM_LIN:case FET_AXISYM_PAR: uITIPE = 1;
								break;
							}

							if (iINPT == 0)
							{
								str.Format("%5d", el.m_uID);
							}
							else
							{
								str.Format("%10d", el.m_uID);
							}
							file->WriteString(str);
							for (k = 0; k<(UINT)((el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_TRI3) ? 2 : 3); k++)
							{
								if (el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_QUAD8)
									if (iINPT == 0)
									{
										str.Format("%5u", el.m_uNode[surface_nodes_2d[j - 2][k]]);
									}
									else
									{
										str.Format("%10u", el.m_uNode[surface_nodes_2d[j - 2][k]]);
									}
								else
									if ((m_NodArray.GetSize()) <= 99999)
									{
										str.Format("%5u", el.m_uNode[surface_nodes_t2d[j - 2][k]]);
									}
									else
									{
										str.Format("%10u", el.m_uNode[surface_nodes_t2d[j - 2][k]]);
									}
								file->WriteString(str);
							}
							if (iINPT == 0)
							{
								str.Format("%5d%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0]);
							}
							else
							{
								str.Format("%10d%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0]);
							}
							file->WriteString(str);
						}
					}
				}
			}
		}

		//Card 11-b
		//Surface pressure
		if (uNPP3>0)
		{
			file->WriteString(CardH11_b);
			file->WriteString(CardV11_br);
			for (i = 0; i<(UINT)lo.m_StructLoads.GetSize(); i++)
			{
				sl = lo.m_StructLoads[i];
				if (sl.m_uLoadtype == FELT_ELEM_RADIATION)
				{
					el = m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					j = sl.m_uDof_face[0] - 1;
					if (fabs(sl.m_dValue[0]) >= LOAD_PRESS_TOL)
					{
						//		UINT nfun;
						//		nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);

						switch (el.m_uTopology)
						{
						case FETO_BRICK8: case FETO_BRICK20:
						{
											  //			p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
											  //		 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
											  if (iINPT == 0)
											  {
												  str.Format("%5d", el.m_uID);
											  }
											  else
											  {
												  str.Format("%10d", el.m_uID);
											  }
											  file->WriteString(str);
											  for (k = 0; k<(UINT)(el.m_uTopology == FETO_BRICK8 ? 4 : 8); k++)
											  {
												  if (iINPT == 0)
												  {
													  str.Format("%5u", el.m_uNode[surface_nodes_3d[j][k]]);
												  }
												  else
												  {
													  str.Format("%10u", el.m_uNode[surface_nodes_3d[j][k]]);
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
											  // promenjen format stampa 8 cvorova za fluks kod osmocvornih elemenata
											  if (el.m_uTopology == FETO_BRICK8)
											  {
												  int node48 = 0;
												  if (iINPT == 0)
												  {
													  str.Format("%5d%5d%5d%5d", node48, node48, node48, node48);
												  }
												  else
												  {
													  str.Format("%10d%10d%10d%10d", node48, node48, node48, node48);
												  }
											  }
											  file->WriteString(str);
											  if (iINPT == 0)
											  {
												  str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0]);
											  }
											  else
											  {
												  str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0]);
											  }
											  file->WriteString(str);
						}
							break;
						case FETO_TETRA4: case FETO_TETRA10:
						{
											  if (iINPT == 0)
											  {
												  str.Format("%5d", el.m_uID);
											  }
											  else
											  {
												  str.Format("%10d", el.m_uID);
											  }
											  file->WriteString(str);
											  for (k = 0; k<(UINT)(el.m_uTopology == FETO_TETRA4 ? 4 : 8); k++)
											  {
												  if (iINPT == 0)
												  {
													  str.Format("%5u", el.m_uNode[surface_nodes_tetra[j][k]]);
												  }
												  else
												  {
													  str.Format("%10u", el.m_uNode[surface_nodes_tetra[j][k]]);
												  }
												  file->WriteString(str);
											  }
											  //				file->WriteString("\n");
											  double press4 = 0.0;
											  if (iINPT == 0)
											  {
												  str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0], press4);
											  }
											  else
											  {
												  str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0], press4);
											  }
											  file->WriteString(str);
						}
							break;
						case FETO_QUAD4: case FETO_QUAD8: case FETO_TRI3: case FETO_TRI6:
							if (j<2)
							{
								//				p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
								//			 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
								if (iINPT == 0)
								{
									str.Format("%5d", el.m_uID);
								}
								else
								{
									str.Format("%10d", el.m_uID);
								}
								file->WriteString(str);
								for (k = 0; k<(UINT)((el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_TRI3) ? 4 : 8); k++)
								{
									if (el.m_uTopology == FETO_QUAD4 || el.m_uTopology == FETO_QUAD8)
										if (iINPT == 0)
										{
											str.Format("%5u", el.m_uNode[surface_nodes_shell[j][k]]);
										}
										else
										{
											str.Format("%10u", el.m_uNode[surface_nodes_shell[j][k]]);
										}
									else
										if (iINPT == 0)
										{
											str.Format("%5u", el.m_uNode[surface_nodes_tshell[j][k]]);
										}
										else
										{
											str.Format("%10u", el.m_uNode[surface_nodes_tshell[j][k]]);
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
								if (iINPT == 0)
								{
									str.Format("%5d%10.6f%10.6f%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0]);
								}
								else
								{
									str.Format("%10d%10.6f%10.6f%10.6f%10.6f\n", sl.m_uSl_funcID, sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0], sl.m_dValue[0]);
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



/*  ne stampa se protoko za pakt

//Card /12/
    file->WriteString(CardH12);
	file->WriteString(CardV12);
		if(iINPT==0)
			{
				str.Format("%5u%5u\n",uNHP,m_ElArray.GetSize());
			}
		else 
			{
				str.Format("%5u%10u\n",uNHP,m_ElArray.GetSize());
			}
		if(	uNHP==0) str.Format("%5u%5u\n",uNHP,0);
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

		str.Format("%5u%5u\n",ci+1,uNMAX);
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
								if((m_NodArray.GetSize())<=99999)
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
								if((m_NodArray.GetSize())<=99999)
								{
									str.Format("%5u",el.m_uNode[surface_nodes_t2d[j-2][k]]);
								}
								else
								{
									str.Format("%10u",el.m_uNode[surface_nodes_t2d[j-2][k]]);
								}
							}
						}
						file->WriteString(str);
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
							if((m_NodArray.GetSize())<=99999)
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
								if((m_NodArray.GetSize())<=99999)
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
							if((m_NodArray.GetSize())<=99999)
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
								if((m_NodArray.GetSize())<=99999)
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
								if((m_NodArray.GetSize())<=99999)
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
										if((m_NodArray.GetSize())<=99999)
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
										if((m_NodArray.GetSize())<=99999)
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

  */

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

UINT CModelData::ExportPAKPT_MatModels(CStdioFile* file)
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
		pstr.Format("%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e\n",
			m_MaterialsArray[i].m_dK[0],m_MaterialsArray[i].m_dK[0],m_MaterialsArray[i].m_dK[0],
			m_MaterialsArray[i].m_dThermal_cap, m_MaterialsArray[i].m_dDensity);
		file->WriteString(pstr);
/*		pstr.Format("%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e%10.4.2e%5u%10.4.2e\n",
			m_MaterialsArray[i].m_dK[0], m_MaterialsArray[i].m_dK[0], m_MaterialsArray[i].m_dK[0],
			m_MaterialsArray[i].m_dThermal_cap, m_MaterialsArray[i].m_dK[1], m_MaterialsArray[i].m_dK[1],
			m_MaterialsArray[i].m_dK[1], m_MaterialsArray[i].m_dThermal_cap,
			INDPK, m_MaterialsArray[i].m_dTemperature);
		file->WriteString(pstr); */
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
