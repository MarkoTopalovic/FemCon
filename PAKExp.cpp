// PAKExp.cpp: Functions for export to PAK's DAT file.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FemCon.h"
#include "ModelData.h"
#include "PAKExp.h"
#include "BlockIDs.h"
#include "PakExpOpt.h"
#include "math.h"
#include "PakString.h"
#include "MaterialDLG.h"
extern double m_dExHS;
extern double m_dExAN;
extern bool m_bExRamberg;
extern double m_dExTAUY;
extern double m_dExCy;
extern double m_dExEM;
extern bool m_bExGlobal;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


UINT CModelData::ExportPAK_S(CStdioFile* file,CPakExpOpt *peo)
{

	CString str;
	UINT uNGELEM=0;
	UINT i;
	long lGeneralDataPos;
	int iINPT, iICCGG;

	iINPT = 1;
	iICCGG = -11;

	if((m_NodArray.GetSize())<=0)
	{
		AfxMessageBox(NO_NODES);
		return(0);	
	};
	
//	if((uNGELEM=m_PropArray.GetSize())<=0)
// sneza promenila nema izjednacavanja sa uNgelem broja propertija
	if((m_PropArray.GetSize())<=0)
	{
		AfxMessageBox(NO_GROUPS);
		return(0);	
	};

//Card /2/
	file->WriteString(CardH2);
	file->WriteString(CardV2);
	str.Format("%5u%5u\n", INDFOR, iINPT);
	file->WriteString(str);
//Card /3/
 	file->WriteString(CardH3);
	file->WriteString(CardV3);
	lGeneralDataPos=file->GetPosition();
	str.Format("                              \n");
	file->WriteString(str);

//Card /4/
 	file->WriteString(CardH4);
	file->WriteString(CardV4);
	
	HNodes nn=m_NodArray[0];
	str.Format("%2u%2u%2u%2u%2u%2u   %5u%5u%5u\n",
				nn.m_bPermbc[0],nn.m_bPermbc[1],nn.m_bPermbc[2],nn.m_bPermbc[3],nn.m_bPermbc[4],nn.m_bPermbc[5],
				(int)m_PakOptValue.GetDynamicAnIsSet(),(int)m_PakOptValue.GetEigenvalueIsSet(),ICVEL);
	file->WriteString(str);
//Card /5/
 	file->WriteString(CardH5);
	file->WriteString(CardV5);
	str.Format("%5u%5d%5d%5d%5d%5d%5d%5d%5d%5d%5d\n",NULAZ,NBLPR,NBLGR,ISTKO,NCVPR,ISTEM,ISTVN,ISTSI,ISTDE,ISTNA,IDEAS);
	file->WriteString(str);
//Card /6/
 	file->WriteString(CardH6);
	file->WriteString(CardV6);
	if(NBLPR > 0)
	{	// stampa samo 1 red u kartici 6
		for(i=1;i<=NBLPR;i++)
		{
			str.Format("%5u%5u%5u%5u%5u\n",i,NPRGR1,NPRGR2,NPRGR3,NPRGR4);
			file->WriteString(str);
		}
	}
	for(i=1;i<=NBLGR;i++)
	{
		str.Format("%5u%5u%5u%5u%5u\n",i,NPRGR1,NPRGR2,NPRGR3,NPRGR4);
		file->WriteString(str);
	}
//Card /7/
 	file->WriteString(CardH7);
	file->WriteString(CardV7);
//	str.Format("%5u          %5u%5u%10.6f%10.6f\n",IREST,NMODS,ICCGG,TOLG,ALFAG);
//	str.Format("%5u          %5u%5u%10.6f%10.6f\n",m_PakOptValue.GetProgramStart(),NMODS,ICCGG,TOLG,ALFAG);
	str.Format("%5u          %5u%5d%10.6f%10.6f     %5u\n",m_PakOptValue.GetProgramStart(),m_PakOptValue.GetNMODS(),ICCGG,TOLG,ALFAG,OKORAK);
	file->WriteString(str);
	
	if(m_PakOptValue.GetNMODS()>0)
	{
		Imperfection imp;
	//Card /7-1/
 		file->WriteString(CardH7_1);
		file->WriteString(CardV7_1);
		for(int y=0;y<m_PakOptValue.GetNMODS();y++)
		{
			imp=m_PakOptValue.GetImperfection(y);
			if (iINPT==0)
				{
					str.Format("%5u%5u%5u%10.6f\n",imp.ModeNum, imp.NodeNum, imp.DOF, imp.Scale);
				}
			else 
				{
					str.Format("%5u%10u%5u%10.6f\n",imp.ModeNum, imp.NodeNum, imp.DOF, imp.Scale);
				}
			file->WriteString(str);
		}
	}
//Card /8/
 	file->WriteString(CardH8);
	file->WriteString(CardV8);
	for(i=1;i<=(UINT)m_PakOptValue.GetNumOfPeriods();i++)
	{
		str.Format("%5u%5u%10.6f\n",i,m_PakOptValue.GetNumSteps(i-1),m_PakOptValue.GetStep(i-1));
		file->WriteString(str);
	}
    
	
	if (m_PakOptValue.GetDynamicAnIsSet())
	{
//Card /8-1/
 		file->WriteString(CardH8_1);
		file->WriteString(CardV8_1);
		str.Format("%5u\n",1);
		file->WriteString (str);
	}

	if (m_PakOptValue.GetEigenvalueIsSet())
	{
//Card /8-3/
 		file->WriteString(CardH8_3);
		file->WriteString(CardV8_3);
		str.Format("%5u%5u\n",1,m_PakOptValue.GetNSOPV());
		file->WriteString (str);
	}
	else
	{
		if (m_PakOptValue.GetDynamicAnIsSet())
		{
//Card /8-2/
	 		file->WriteString(CardH8_2);
			file->WriteString(CardV8_2);
			str.Format("\n");
			file->WriteString (str);
		}
	}
	
//Card /9/
 	file->WriteString(CardH9);
	file->WriteString(CardV9);
	
	switch(m_PakOptValue.GetConvCrit())
	{
		case 0:
			str.Format("%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f%10.6f%5u\n",m_PakOptValue.GetIterMethod(),MAXIT,1,0,0,TOLE,TOLS,TOLM,TOLA,NBRCR);
			break;
		case 1:
			str.Format("%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f%10.7f%5u\n",m_PakOptValue.GetIterMethod(),MAXIT,1,0,0,m_PakOptValue.GetConvTol(0),TOLS,TOLM,TOLA,NBRCR);
			break;
		case 2:
			str.Format("%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f%10.6f%5u\n",m_PakOptValue.GetIterMethod(),MAXIT,1,0,0,TOLE,m_PakOptValue.GetConvTol(1),TOLM,TOLA,NBRCR);
			break;
		case 3:
			str.Format("%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f%10.6f%5u\n",m_PakOptValue.GetIterMethod(),MAXIT,1,0,0,m_PakOptValue.GetConvTol(0),m_PakOptValue.GetConvTol(1),TOLM,TOLA,NBRCR);
			break;
	}

	file->WriteString(str);

	if (m_PakOptValue.GetIterMethod()>=61)
	{
	 	file->WriteString(CardH9_1);
		file->WriteString(CardV9_1);
		
		str.Format("%5u%5u%5u%10.6f%10.6f%10.6f\n",5,m_PakOptValue.GetNodeNumber(),m_PakOptValue.GetDirection(),
					m_PakOptValue.GetValue(),m_PakOptValue.GetAG(),m_PakOptValue.GetDS());
		file->WriteString(str);		
	}

	ExportPAK_Nodes(file, iINPT);

	if (m_PakOptValue.GetDynamicAnIsSet() && !(m_PakOptValue.GetEigenvalueIsSet()))
	{
//Card /10.1/
		str.Format("\n");
		file->WriteString (str);
	}

	if(m_ConsArray.GetSize() && m_ConsArray[0].m_ConsEqs.GetSize()>0)
	{
//Card /10-02/
		ExportPAK_Equations(file, iINPT);
	}

	if (ExportPAK_MatModels(file)==999) return(0);
//Card /13/
	ExportPAK_Elements(file, &uNGELEM, peo, iINPT);
	if ((UINT)m_ConnArray.GetSize()>0) ExportPAK_ContactElements(file, iINPT);
//Card /14/
	ExportPAK_TimeFunctions(file);
//Card /15/
	ExportPAK_Loads(file, iINPT);

//Card /16/
 	file->WriteString(CardH16);
	file->WriteString(CardV16);

//Upisivanje naknadno

	file->Seek(lGeneralDataPos,CFile::begin);

	CMap<UINT,UINT,UINT,UINT> TypesOfEl;
	UINT matID, matType, elType;
	HProperties pr;
	TypesOfEl.RemoveAll();

	for(i=0;i<(UINT)m_ElArray.GetSize();i++)
	{
// odredjivanje grupa elemenata. razliciti tipovi elemenata su razlicite grupe
		TypesOfEl[m_PropArray[m_nPropIndex[m_ElArray[i].m_uPropID]].m_uType]++;
	}
// ako ima kontaktnih elemenata dodaje se jos jedna grupa elemenata
	if((UINT)m_ConnArray.GetSize()>0) TypesOfEl[m_CPropArray[m_nCPropIndex[0]].m_uType]++;
//
// odredjivanje broja razlicitih materijala
	CMap<UINT,UINT,UINT,UINT> TypesOfMat;

	TypesOfMat.RemoveAll();

	for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	{
// sneza promenila kad je dodato citanje Cam-clay-a i ostalih materijala type = 6 General
		if(m_MaterialsArray[i].m_uType != 6)
		{
			TypesOfMat[m_MaterialsArray[i].m_uType]++;
		}
		else //if(m_MaterialsArray[i].m_uType != 6)
		{
			TypesOfMat[m_MaterialsArray[i].m_uSubType]++;
		}
//		TypesOfMat[m_MaterialsArray[i].m_uType]++;
//		TypesOfMat[m_MaterialsArray[i].m_iNonlin_type]++;
	}
//	P R I V R E M E N O
//	str.Format("%5u%5u%5u%5u",m_NodArray.GetSize(),TypesOfEl.GetCount(),m_MaterialsArray.GetSize(),m_PakOptValue.GetNumOfPeriods(),NKRT);
	if (iINPT == 0)
		{
			str.Format("%5u%5u%5u%5u",m_NodArray.GetSize(),TypesOfEl.GetCount()+uNGELEM,TypesOfMat.GetCount(),m_PakOptValue.GetNumOfPeriods(),NKRT);
//			str.Format("%5u%5u%5u%5u",m_NodArray.GetSize(),TypesOfEl.GetCount(),1,m_PakOptValue.GetNumOfPeriods(),NKRT);
		}
	else 
		{
			str.Format("%10u%5u%5u%5u",m_NodArray.GetSize(),TypesOfEl.GetCount(),TypesOfMat.GetCount(),m_PakOptValue.GetNumOfPeriods(),NKRT);
//			str.Format("%10u%5u%5u%5u",m_NodArray.GetSize(),TypesOfEl.GetCount(),1,m_PakOptValue.GetNumOfPeriods(),NKRT);
		}

	file->WriteString(str);
	file->Seek(0,CFile::end);

	return(-1);
}

UINT CModelData::ExportPAK_Elements(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo, int iINPT)
{
	CString str;
	UINT i,j,k;
	int NMODM;
// Top maj 2012
	int intBTH = BTH;
	int intDTH = DTH;
	int intINDBTH=INDBTH;
	int intINDDTH=INDDTH;
// Top
	HProperties pr;
	CArray <UINT,UINT&> ElNum;
	CMap<UINT,UINT,UINT,UINT> PropIndex; // relation between Property ID and it's position in ElNum;
	HElement el;
	UINT uIETYP,uNETIP;
	CMap<UINT,UINT,UINT,UINT> TypesOfEl;
	CPAKString pstr;
	UINT uMODEL1;

	//FEMAT Topology
	//Value          {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
	  UINT FEM_Top[]={2,3,3,6,4,8,4,6,8,1,10,15,20, 0, 0, 0};

	TypesOfEl.RemoveAll();

	for(i=0;i<(UINT)m_ElArray.GetSize();i++)
	{
		TypesOfEl[m_PropArray[m_nPropIndex[m_ElArray[i].m_uPropID]].m_uType]++;
	}

	POSITION pos = TypesOfEl.GetStartPosition();
	
	while( pos != NULL )
	{
		UINT key, num;

		TypesOfEl.GetNextAssoc( pos, key, num);
		
		switch (key)
		{
//****************************************************************************
// ****************************** 1D Elementi *********************************
//****************************************************************************
		case FET_ROD: case FET_BAR: case FET_BEAM: case FET_GAP:
			{
				
				HElement *ell;
				HProperties pr1;

				for(i=0;i<(UINT)m_ElArray.GetSize();i++)
				{
					if ((m_PropArray[m_nPropIndex[m_ElArray[i].m_uPropID]].m_uType)==key)
					{
						ell=&m_ElArray[i];
						pr1=m_PropArray[m_nPropIndex[ell->m_uPropID]];
						break;
					}
				}

				UINT uNE2=0;

				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				{
					if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						if(m_ElArray[j].m_uTopology==FETO_LINE) uNE2++;
					}
				}

				if(uNE2>0)	//1D element
				{
				 //Card /13/

				 if(key==FET_ROD)
				 {
					 uNETIP=PAK_TRUSS;

					 file->WriteString(CardH13);
					 file->WriteString(CardV13);
 			 		 
					 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));
 
					// Top maj 2012
				    intINDBTH=INDBTH;
					intINDDTH=INDDTH;
					 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					 {
						el=m_ElArray[j];
						if( el.m_uColor>1024 && el.m_uColor<2048)
							{
									intINDBTH=1;
							}
						else if(el.m_uColor>2048)
							{
									intINDDTH=1;
							}
					 }
					// Top
					 if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE2,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE2,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					 file->WriteString(str);

					 file->WriteString(CardH13_1);
					 file->WriteString(CardH13_1_a_1);
					 file->WriteString(CardV13_1_a_2);
					 file->WriteString(CardH13_1_b_1);
					 file->WriteString(CardV13_1_b_2);

				/*	 bool first;
					 first=true; */

					 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					 if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
						{
							el=m_ElArray[j];
							pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

							if(el.m_uTopology==FETO_LINE)
							{
								// Top maj 2012						
								intBTH = BTH;
								intDTH = DTH;
								if( el.m_uColor>1024 && el.m_uColor<2048)
								{
									intBTH=el.m_uColor-1023;
									// ankeri kasne 5 koraka za remenatama
									if (pr.m_uID==21 || pr.m_uID==22|| pr.m_uID==23)
									{
										intBTH=el.m_uColor-1018;
									}
								}
								else if(el.m_uColor>2048)
								{
									intDTH=el.m_uColor-2048;
								}
								// Top
							if (iINPT == 0)
								{
								str.Format("%5u%5u%5u%10.6f%5u%10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,ISNA,pr.m_dValue[0],KORC,float(intBTH),float(intDTH));
								}
							else 
								{
								str.Format("%10u%5u%5u%10.6f%5u%10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,ISNA,pr.m_dValue[0],KORC,float(intBTH),float(intDTH));
								}
								file->WriteString(str);

						   /*	if (first) 
								{
								 file->WriteString(CardH13_1_b_1);
								 file->WriteString(CardV13_1_b_2);
							
								 first=false;
								} */

								for(k=0;k<FEM_Top[el.m_uTopology];k++) 
								{
									if (iINPT == 0)
										{
										str.Format("%5u",el.m_uNode[k]);
										}
									else 
										{
										str.Format("%10u",el.m_uNode[k]);
										}
									file->WriteString(str);
								}
								file->WriteString("\n");
							}
						}
				 }
				 else if(key==FET_BEAM)
				 {
					 uNETIP=PAK_THINWALED_B;

					 file->WriteString(CardH13);
					 file->WriteString(CardV13);

					 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

					 // Top maj 2012
				    intINDBTH=INDBTH;
					intINDDTH=INDDTH;
					 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					 {
						el=m_ElArray[j];
						if( el.m_uColor>1024 && el.m_uColor<2048)
							{
									intINDBTH=1;
							}
						else if(el.m_uColor>2048)
							{
									intINDDTH=1;
							}
					 }

					 if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE2,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE2,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					 file->WriteString(str);

					 file->WriteString(CardH13_6);
					 file->WriteString(CardH13_6_a_1);
					 file->WriteString(CardV13_6_a_2);
  					 str.Format("%5u%5u%5u%5u%10.6f%10.6f%10.6f%10.6f%10.6f%5u\n",
									NT,NELM,NTIP,NKAR,OY,OZ,YM,ZM,ALFAU,INDOF);
					 file->WriteString(str);

 					 pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

					 file->WriteString(CardH13_6_b_1);
					 file->WriteString(CardV13_6_b_2);
  					 pstr.Format("%10.5.1e%10.5.1e%10.5.1e%10.5.1e",
									pr1.m_dValue[0], pr1.m_dValue[2], pr1.m_dValue[1], pr1.m_dValue[4]);
					 file->WriteString(pstr);
  					 str.Format("%10.6f%10.6f%10.6f\n", ALFA,CAPAY,CAPAZ);
					 file->WriteString(str);

 					 file->WriteString(CardH13_6_h_1);
					 file->WriteString(CardV13_6_h_2);

					 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					 if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
						{
							el=m_ElArray[j];
							pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

							if(el.m_uTopology==FETO_LINE || el.m_uTopology==FETO_LINE3)
							{
							// Top maj 2012						
							intBTH = BTH;
							intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
								if (iINPT == 0)
									{
									pstr.Format("%5u%5u%5u%5u%5u%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.6f%10.6f\n",
										el.m_uID,el.m_uNode[0],el.m_uNode[1],
										NTIP_13_6,pr.m_uMatIID,NAP_13_6,0,el.m_dOrient[0],el.m_dOrient[1],el.m_dOrient[2],float(intBTH),float(intDTH));
									}
								else 
									{
									pstr.Format("%10u%10u%10u%5u%5u%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.6f%10.6f\n",
										el.m_uID,el.m_uNode[0],el.m_uNode[1],
										NTIP_13_6,pr.m_uMatIID,NAP_13_6,0,el.m_dOrient[0],el.m_dOrient[1],el.m_dOrient[2],float(intBTH),float(intDTH));
									}
								file->WriteString(pstr);
							}
						}
				 }
				else if(key==FET_BAR)
				{
					 uNETIP=PAK_BEAM_SUPER;

					 file->WriteString(CardH13);
					 file->WriteString(CardV13);

					 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

					 // Top maj 2012
				    intINDBTH=INDBTH;
					intINDDTH=INDDTH;
					 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					 {
						el=m_ElArray[j];
						if( el.m_uColor>1024 && el.m_uColor<2048)
							{
									intINDBTH=1;
							}
						else if(el.m_uColor>2048)
							{
									intINDDTH=1;
							}
					 }
					 //Top
					 if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE2,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE2,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					 file->WriteString(str);

					 file->WriteString(CardH13_9);
					 file->WriteString(CardH13_9_a_1);
					 file->WriteString(CardH13_9_a_2);
				     str.Format("%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f%10.6f%5d\n",
					   NGAUSX2,NGAUSY2,NGAUSZ2,MSET,BETA,MSLOJ,CPP1,CPP2,CPP3,IALFA);
					 file->WriteString(str);

 					 pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

					 file->WriteString(CardH13_9_b_1);
					 file->WriteString(CardH13_9_b_2);
					 
					 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
										m_PakOptValue.GetSBTypeOfSegment(),NS,NCVP,NPRES,IND3D,NCFL,NPRL,IPOMT,CPP1,CPP2,CPP3);
					 file->WriteString(str);

					 file->WriteString(CardH13_9_d1_1);
					 file->WriteString(CardH13_9_d1_2);
					 file->WriteString(CardH13_9_d2_1);
					 file->WriteString(CardH13_9_d2_2);
					 
					 str.Format("%5u%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f\n",
						1,1,0,0,0,m_PakOptValue.GetSBParamA(),0,.0,.0);
					 file->WriteString(str);
					 str.Format("%5u\n", 1);
					 file->WriteString(str);

					 file->WriteString(CardH13_9_e1_1);
					 file->WriteString(CardH13_9_e1_2);					 

					 file->WriteString(" 1 1 1 1 1 1\n");

					 file->WriteString(CardH13_9_e2_1);
					 file->WriteString(CardH13_9_e2_2);
					 
					 file->WriteString("    1  0 0 0 1 1 1          .0        .0\n");

					 file->WriteString(CardH13_9_f1_1);
					 file->WriteString(CardH13_9_f1_2);
					 file->WriteString(CardH13_9_f2_1);
					 file->WriteString(CardH13_9_f2_2);
					 file->WriteString(CardH13_9_f3_1);
					 file->WriteString(CardH13_9_f3_2);

					 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					 if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
						{
							el=m_ElArray[j];
							pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

							if(el.m_uTopology==FETO_LINE)
							{
								if (iINPT == 0)
									{
										str.Format("%5u%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f\n",
											el.m_uID,1,0,0,0,1.,0,.0,.0);
									}
								else 
									{
										str.Format("%10u%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f\n",
											el.m_uID,1,0,0,0,1.,0,.0,.0);
									}
								file->WriteString(str);
								if (iINPT == 0)
									{
										str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u\n",
											el.m_uNode[0],el.m_uNode[1],el.m_uNode[2],
											1,1,1,0,0,0);
									}
								else 
									{
										str.Format("%10u%10u%10u%5u%5u%5u%5u%5u%5u\n",
											el.m_uNode[0],el.m_uNode[1],el.m_uNode[2],
											1,1,1,0,0,0);
									}
								file->WriteString(str);
								str.Format("%10.6f%10.6f%10.6f%10.6f%10.6f%10.6f\n",
									el.m_dOrient[0],el.m_dOrient[0],el.m_dOrient[0],
									1.,1.,1.);
								file->WriteString(str);
							}
						}
				}
				else if(key==FET_GAP)
				 {
					 uNETIP=PAK_GAP;

					 file->WriteString(CardH13);
					 file->WriteString(CardV13);
 			 		 
					 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));
 
					 if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE2,GAP_IETYP,NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE2,GAP_IETYP,NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					 file->WriteString(str);

					 file->WriteString(CardH13_12);
					 file->WriteString(CardH13_12_a);
					 file->WriteString(CardV13_12_a_3);

					 pstr.Format("%10.3.1e%10.3.2e%10.3.2e%10.3.1e%10.3.1e%10.3.1e%10.3.1e\n",
						 pr1.m_dValue[0],pr1.m_dValue[1],pr1.m_dValue[2],pr1.m_dValue[3],pr1.m_dValue[4],
						 pr1.m_dValue[5],pr1.m_dValue[6]);
					 file->WriteString(pstr);

					 file->WriteString(CardH13_12_b);
					 file->WriteString(CardV13_12_b_2);
					 file->WriteString(CardH13_12_c);
					 file->WriteString(CardV13_12_c_2);

				/*	 bool first;
					 first=true; */

					 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					 if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
						{
							el=m_ElArray[j];
							pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

							if(el.m_uTopology==FETO_LINE)
							{
								if (iINPT == 0)
									{
									str.Format("%5u%5u%5u%5u%10.4f%10.4f%10.4f%5u\n",
										el.m_uID,pr.m_uMatIID,ISNA,IPG,el.m_dOrient[0],el.m_dOrient[1],el.m_dOrient[2],KORC);
									}
								else 
									{
									str.Format("%10u%5u%5u%5u%10.4f%10.4f%10.4f%5u\n",
										el.m_uID,pr.m_uMatIID,ISNA,IPG,el.m_dOrient[0],el.m_dOrient[1],el.m_dOrient[2],KORC);
									}
								file->WriteString(str);

							/*	if (first) 
								{
								 file->WriteString(CardH13_1_b_1);
								 file->WriteString(CardV13_1_b_2);
							
								 first=false;
								} */

								for(k=0;k<FEM_Top[el.m_uTopology];k++) 
								{
								if (iINPT == 0)
									{
									str.Format("%5u",el.m_uNode[k]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[k]);
									}
									file->WriteString(str);
								}
								file->WriteString("\n");
							}
						}
				 }

				}

			}
			break;


//****************************************************************************
//****************************** 2D Elementi *********************************
//****************************************************************************
		case FET_SHEAR_LIN:case FET_SHEAR_PAR:
		case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR:
		case FET_BENDING_LIN:case FET_BENDING_PAR:
		case FET_PLATE_LIN:case FET_PLATE_PAR:
		case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR:
		case FET_LAMINATE_LIN:case FET_LAMINATE_PAR:
		case FET_AXISYM_LIN:case FET_AXISYM_PAR:
			{
				HElement *ell;

				for(i=0;i<(UINT)m_ElArray.GetSize();i++)
				{
					if ((m_PropArray[m_nPropIndex[m_ElArray[i].m_uPropID]].m_uType)==key)
					{
						ell=&m_ElArray[i];
						break;
					}
				}

				UINT uNE4=0,uNE3=0;

				switch (key)
				{
					case FET_SHEAR_LIN:case FET_SHEAR_PAR: uIETYP=4;
						break;
					case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR: uIETYP=0;
						break;
					case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR: uIETYP=2;
						break;
					case FET_AXISYM_LIN:case FET_AXISYM_PAR: uIETYP=1;
						break;
				}

				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						if(m_ElArray[j].m_uTopology==FETO_QUAD4 || m_ElArray[j].m_uTopology==FETO_QUAD8) uNE4++;
						else uNE3++;
					}

				if(uNE4>0)	//Cetvorougaoni 2D Element
				{
				 //Card /13/
				 if(key==FET_BENDING_LIN || key==FET_BENDING_PAR ||
					key==FET_PLATE_LIN || key==FET_PLATE_PAR ||
					key==FET_LAMINATE_LIN || key==FET_LAMINATE_PAR) uNETIP=(peo->m_b4N2BD ? PAK_SHELL_BD : PAK_ISO_SHELL); 
				 else uNETIP=PAK_ISO_2D;

				 file->WriteString(CardH13);
				 file->WriteString(CardV13);

					 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

					// Top maj 2012
				    intINDBTH=INDBTH;
					intINDDTH=INDDTH;
					 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					 {
						el=m_ElArray[j];
						if( el.m_uColor>1024 && el.m_uColor<2048)
							{
									intINDBTH=1;
							}
						else if(el.m_uColor>2048)
							{
									intINDDTH=1;
							}
					 }
					// Top

					 if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE4,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE4,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
				 file->WriteString(str);

				 file->WriteString(CardH13_2);
				 file->WriteString(CardV13_2_a_1);
				 if(uNETIP==PAK_ISO_2D)
				 {
				  file->WriteString(CardV13_2_a_2);
				  str.Format("%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f%10.6f%5d\n",
					uIETYP,NGAUSX2,NGAUSY2,MSET,BETA,MSLOJ,CPP1,CPP2,CPP3,IALFA);
				 }
				 else
				 {
				  file->WriteString(CardV13_8_a_2);
				  str.Format("%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f%10.6f%5d\n",
					NGAUSX2,NGAUSY2,NGAUSZ2,MSET,BETA,MSLOJ,CPP1,CPP2,CPP3,IALFA);
				 }
				 file->WriteString(str);

				 file->WriteString(CardV13_2_b_1);
				 file->WriteString(CardV13_2_b_2);
				 file->WriteString(CardV13_2_c_1);
				 file->WriteString(CardV13_2_c_2);

				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
						{
							// Top maj 2012						
							intBTH = BTH;
							intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
							if (iINPT == 0)
								{
								str.Format("%5u%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA,IPGS,pr.m_dValue[0],KORC,float(intBTH),float(intDTH));
								}
							else 
								{
								str.Format("%10u%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA,IPGS,pr.m_dValue[0],KORC,float(intBTH),float(intDTH));
								}
							file->WriteString(str);
							for(k=0;k<FEM_Top[el.m_uTopology];k++) 
							{
							if (iINPT == 0)
								{
									str.Format("%5u",el.m_uNode[k]);
								}
							else 
								{
									str.Format("%10u",el.m_uNode[k]);
								}
							file->WriteString(str);
						}
						file->WriteString("\n");
						}
					}
				}

				if(uNE3>0)	// Trougaoni 2D Element
				{
				 if(uNE4>0) (*uNGELEM)++;
				 //Card /13/
				 if(key==FET_BENDING_LIN || key==FET_BENDING_PAR ||
					key==FET_PLATE_LIN || key==FET_PLATE_PAR ||
					key==FET_LAMINATE_LIN || key==FET_LAMINATE_PAR) uNETIP=PAK_ISO_TRI_SHELL; 
				 else uNETIP=PAK_ISO_TRI;

				 file->WriteString(CardH13);
				 file->WriteString(CardV13);

				 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

				 //Top maj 2012
				 intINDBTH=INDBTH;
				 intINDDTH=INDDTH;
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					el=m_ElArray[j];
					if( el.m_uColor>1024 && el.m_uColor<2048)
						{
								intINDBTH=1;
						}
					else if(el.m_uColor>2048)
						{
								intINDDTH=1;
						}
				 }
				// Top

					if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE3,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 uNETIP,uNE3,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
				 file->WriteString(str);

				 file->WriteString(CardH13_2);
				 file->WriteString(CardV13_2_a_1);

				 if(uNETIP==PAK_ISO_TRI)
				 {
				  file->WriteString(CardV13_2_a_2);
				  str.Format("%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f%10.6f%5d\n",
					uIETYP,NGAUSX2,NGAUSY2,MSET,BETA,MSLOJ,CPP1,CPP2,CPP3,IALFA);
				 }
				 {
				  file->WriteString(CardV13_8_a_2);
				  str.Format("%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f%10.6f%5d\n",
					NGAUSX2,NGAUSY2,NGAUSZ2,MSET,BETA,MSLOJ,CPP1,CPP2,CPP3,IALFA);
				 }
				 file->WriteString(str);

				 file->WriteString(CardV13_2_b_1);
				 file->WriteString(CardV13_2_b_2);
				 file->WriteString(CardV13_2_c_1);
				 file->WriteString(CardV13_2_c_2);

				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

						if(el.m_uTopology==FETO_TRI3 || el.m_uTopology==FETO_TRI6)
						{
							// Top maj 2012
                            intBTH = BTH;
	                        intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
							if (iINPT == 0)
								{
									str.Format("%5u%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f\n",
										el.m_uID,pr.m_uMatIID,IPRCO,ISNA,IPGS,pr.m_dValue[0],KORC,float(intBTH),float(intDTH));
								}
							else 
								{
									str.Format("%10u%5u%5u%5u%5u%10.6f%5u%10.6f%10.6f\n",
										el.m_uID,pr.m_uMatIID,IPRCO,ISNA,IPGS,pr.m_dValue[0],KORC,float(intBTH),float(intDTH));
								}
							file->WriteString(str);
							for(k=0;k<FEM_Top[el.m_uTopology];k++) 
							{
							if (iINPT == 0)
								{
									str.Format("%5u",el.m_uNode[k]);
								}
							else 
								{
									str.Format("%10u",el.m_uNode[k]);
								}
								file->WriteString(str);
							//}
//								file->WriteString(str);
							}
							file->WriteString("\n");
						}
					}
				}				

			}
			break;
//****************************************************************************
// ****************************** 3D Elementi ********************************
//****************************************************************************
		case FET_3D_LIN:case FET_3D_PAR:
			{
				HElement *ell;
				int ISNA1=2;

				for(i=0;i<(UINT)m_ElArray.GetSize();i++)
				{
					if ((m_PropArray[m_nPropIndex[m_ElArray[i].m_uPropID]].m_uType)==key)
					{
						ell=&m_ElArray[i];
						break;
					}
				}

				UINT uNE_3D=0,uNE1_3D=0,uNE6_3D=0,uNE9_3D=0,uNE42_3D=0,uNE_PRISM=0,uNE_TETRA=0;
				UINT uel_type, uMat;

				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

						switch (el.m_uTopology)
						{
//							case FETO_BRICK8:case FETO_BRICK20: uNE_3D++;
							case FETO_BRICK8:case FETO_BRICK20: 
								{ uel_type=GetModelTypeForElement(el.m_uID);
									switch(uel_type)
									{
										case 1:	uNE1_3D++;
											break;
										case 6:	uNE6_3D++;
											break;
										case 9:	uNE9_3D++;
											break;
										case 42: uNE42_3D++;
											break;
									}
								}
								break;
							case FETO_WEDGE6:case FETO_WEDGE15: uNE_PRISM++;
								break;
							case FETO_TETRA4:case FETO_TETRA10: uNE_TETRA++;
								break;
						}
					}

				uNETIP = PAK_ISO_3D;
//				if(uNE_3D>0)	// 3D Element
				if(uNE1_3D>0)	// 3D Element materijal izoparametarski
				{
				 UINT node_order[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};

				 //Card /13/
				 file->WriteString(CardH13);
				 file->WriteString(CardV13);

				 NMODM=1;
//				 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

				// Top maj 2012
                 intINDBTH=INDBTH;
				 intINDDTH=INDDTH;
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					el=m_ElArray[j];
					if( el.m_uColor>1024 && el.m_uColor<2048)
						{
								intINDBTH=1;
						}
					else if(el.m_uColor>2048)
						{
								intINDDTH=1;
						}
				 }
				 // Top
					if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_3D,uNE1_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
//						 PAK_ISO_3D,uNE_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_3D,uNE1_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
//						 PAK_ISO_3D,uNE_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
				 file->WriteString(str);
				 
				 file->WriteString(CardH13_3);
				 file->WriteString(CardV13_3_a_1);
				 file->WriteString(CardV13_3_a_2);
// dodala Sneza ovaj uslov za broj gausovih tacaka, da bi radio pr. 7 iz linearne - izbaceno. Treba dodati mogucnost izbora gausovih tacaka
//				 if(el.m_uTopology==FETO_BRICK20)
//					{
//					 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
//						3,3,3,BETA,IALFA);
//					}
//				else // if (el.m_uTopology==FETO_BRICK8)
//					{
					 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
						NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
//					}
//				 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
//					NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
				 file->WriteString(str);

				 file->WriteString(CardV13_3_b_1);
				 file->WriteString(CardV13_3_b_2);
				 file->WriteString(CardV13_3_c_1);
				 file->WriteString(CardV13_3_c_2);
				
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
				    if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];
						// dodato da stampa razlicite grupe za razlicite materijale
						uel_type=GetModelTypeForElement(el.m_uID);
						if((el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20)&&(uel_type==1))
						{
							// Top maj 2012
							intBTH = BTH;
	                        intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
							if (iINPT == 0)
								{
								str.Format("%5u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							else 
								{
								str.Format("%10u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							file->WriteString(str);
							for(k=0;k<8;k++) 
							{
								if (iINPT == 0)
									{
										str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							}
							file->WriteString("\n");
							if(el.m_uTopology==FETO_BRICK20)
							 for(k=8;k<20;k++) 
							 {
								 if (iINPT == 0)
									{
										str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							 }
							file->WriteString("\n");
						}
					}
				 }
				}

				if(uNE6_3D>0)	// 3D Element materijal elasto plastican
				{
				 UINT node_order[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};
				
				 if(uNE1_3D>0) (*uNGELEM)++;

				 //Card /13/
				 file->WriteString(CardH13);
				 file->WriteString(CardV13);

				 NMODM=6;
//				 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

				// Top maj 2012
                 intINDBTH=INDBTH;
				 intINDDTH=INDDTH;
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					el=m_ElArray[j];
					if( el.m_uColor>1024 && el.m_uColor<2048)
						{
								intINDBTH=1;
						}
					else if(el.m_uColor>2048)
						{
								intINDDTH=1;
						}
				 }
				 // Top
					if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_3D,uNE6_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
//						 PAK_ISO_3D,uNE_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_3D,uNE6_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
//						 PAK_ISO_3D,uNE_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
				 file->WriteString(str);
				 
				 file->WriteString(CardH13_3);
				 file->WriteString(CardV13_3_a_1);
				 file->WriteString(CardV13_3_a_2);
// dodala Sneza ovaj uslov za broj gausovih tacaka, da bi radio pr. 7 iz linearne
/*				 if(el.m_uTopology==FETO_BRICK20)
					{
					 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
						3,3,3,BETA,IALFA);
					}
				else // if (el.m_uTopology==FETO_BRICK8)
					{
					 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
						NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
					} */
				 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
					NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
				 file->WriteString(str);

				 file->WriteString(CardV13_3_b_1);
				 file->WriteString(CardV13_3_b_2);
				 file->WriteString(CardV13_3_c_1);
				 file->WriteString(CardV13_3_c_2);
				
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
				    if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];
						// dodato da stampa razlicite grupe za razlicite materijale
						uel_type=GetModelTypeForElement(el.m_uID);
						if((el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20)&&(uel_type==6))
						{
							// Top maj 2012
							intBTH = BTH;
	                        intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
							if (iINPT == 0)
								{
								str.Format("%5u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							else 
								{
								str.Format("%10u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							file->WriteString(str);
							for(k=0;k<8;k++) 
							{
								if (iINPT == 0)
									{
										str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							}
							file->WriteString("\n");
							if(el.m_uTopology==FETO_BRICK20)
							 for(k=8;k<20;k++) 
							 {
								 if (iINPT == 0)
									{
										str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							 }
							file->WriteString("\n");
						}
					}
				 }
				}
				
				if(uNE9_3D>0)	// 3D Element materijal elasto plastican
				{
				 UINT node_order[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};
				
				 if(uNE1_3D>0 || uNE6_3D>0) (*uNGELEM)++;

				 //Card /13/
				 file->WriteString(CardH13);
				 file->WriteString(CardV13);

				 NMODM=9;
//				 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

				// Top maj 2012
                 intINDBTH=INDBTH;
				 intINDDTH=INDDTH;
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					el=m_ElArray[j];
					if( el.m_uColor>1024 && el.m_uColor<2048)
						{
								intINDBTH=1;
						}
					else if(el.m_uColor>2048)
						{
								intINDDTH=1;
						}
				 }
				 // Top
					if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_3D,uNE9_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
//						 PAK_ISO_3D,uNE_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_3D,uNE9_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
//						 PAK_ISO_3D,uNE_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
				 file->WriteString(str);
				 
				 file->WriteString(CardH13_3);
				 file->WriteString(CardV13_3_a_1);
				 file->WriteString(CardV13_3_a_2);
// dodala Sneza ovaj uslov za broj gausovih tacaka, da bi radio pr. 7 iz linearne
/*				 if(el.m_uTopology==FETO_BRICK20)
					{
					 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
						3,3,3,BETA,IALFA);
					}
				else // if (el.m_uTopology==FETO_BRICK8)
					{
					 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
						NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
					} */
				 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
					NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
				 file->WriteString(str);

				 file->WriteString(CardV13_3_b_1);
				 file->WriteString(CardV13_3_b_2);
				 file->WriteString(CardV13_3_c_1);
				 file->WriteString(CardV13_3_c_2);
				
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
				    if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];
						// dodato da stampa razlicite grupe za razlicite materijale
						uel_type=GetModelTypeForElement(el.m_uID);
						if((el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20)&&(uel_type==9))
						{
							// Top maj 2012
							intBTH = BTH;
	                        intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
							if (iINPT == 0)
								{
								str.Format("%5u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							else 
								{
								str.Format("%10u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							file->WriteString(str);
							for(k=0;k<8;k++) 
							{
								if (iINPT == 0)
									{
										str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							}
							file->WriteString("\n");
							if(el.m_uTopology==FETO_BRICK20)
							 for(k=8;k<20;k++) 
							 {
								 if (iINPT == 0)
									{
										str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							 }
							file->WriteString("\n");
						}
					}
				 }
				}

				if(uNE42_3D>0)	// 3D Element materijal Mohr-Coloumb
				{
				 UINT node_order[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};

				 if(uNE1_3D>0 || uNE6_3D>0 || uNE9_3D>0) (*uNGELEM)++;
				 //Card /13/
				 file->WriteString(CardH13);
				 file->WriteString(CardV13);

				 NMODM=42;
//				 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

				// Top maj 2012
                 intINDBTH=INDBTH;
				 intINDDTH=INDDTH;
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					el=m_ElArray[j];
					if( el.m_uColor>1024 && el.m_uColor<2048)
						{
								intINDBTH=1;
						}
					else if(el.m_uColor>2048)
						{
								intINDDTH=1;
						}
				 }
				 // Top
					if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_3D,uNE42_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
//						 PAK_ISO_3D,uNE_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_3D,uNE42_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
//						 PAK_ISO_3D,uNE_3D,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
				 file->WriteString(str);
				 
				 file->WriteString(CardH13_3);
				 file->WriteString(CardV13_3_a_1);
				 file->WriteString(CardV13_3_a_2);
// dodala Sneza ovaj uslov za broj gausovih tacaka, da bi radio pr. 7 iz linearne
/*				 if(el.m_uTopology==FETO_BRICK20)
					{
					 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
						3,3,3,BETA,IALFA);
					}
				else // if (el.m_uTopology==FETO_BRICK8)
					{
					 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
						NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
					} */
				 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
					NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
				 file->WriteString(str);

				 file->WriteString(CardV13_3_b_1);
				 file->WriteString(CardV13_3_b_2);
				 file->WriteString(CardV13_3_c_1);
				 file->WriteString(CardV13_3_c_2);
				
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
				    if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];
						// dodato da stampa razlicite grupe za razlicite materijale
						uel_type=GetModelTypeForElement(el.m_uID);
						if((el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20)&&(uel_type==42))
						{
							// Top maj 2012
							intBTH = BTH;
	                        intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
							if (iINPT == 0)
								{
								str.Format("%5u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							else 
								{
								str.Format("%10u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							file->WriteString(str);
							for(k=0;k<8;k++) 
							{
								if (iINPT == 0)
									{
										str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							}
							file->WriteString("\n");
							if(el.m_uTopology==FETO_BRICK20)
							 for(k=8;k<20;k++) 
							 {
								if(iINPT==0)
									{
										str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
										str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							 }
							file->WriteString("\n");
						}
					}
				 }
				} 

				if(uNE_PRISM>0)	// 3D Prismatic Element
				{
				 UINT node_order[15]={5,6,4,1,2,0,17,18,16,9,10,8,13,14,12};

				 if(uNE_3D>0) uNGELEM++;
				 //Card /13/
				 file->WriteString(CardH13);
				 file->WriteString(CardV13);
				 // Top maj 2012
				 intINDBTH=INDBTH;
				 intINDDTH=INDDTH;
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					if( el.m_uColor>1024 && el.m_uColor<2048)
						{
								intINDBTH=1;
						}
					else if(el.m_uColor>2048)
						{
								intINDDTH=1;
						}
				 }
				 // Top
					if (iINPT == 0)
						{
						 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_PRISM,uNE_PRISM,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
					else 
						{
						 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
						 PAK_ISO_PRISM,uNE_PRISM,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
						}
				 file->WriteString(str);
				 
				 file->WriteString(CardH13_3);
				 file->WriteString(CardV13_3_a_1);
				 file->WriteString(CardV13_3_a_2);
				 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
					NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
				 file->WriteString(str);

				 file->WriteString(CardV13_3_b_1);
				 file->WriteString(CardV13_3_b_2);
				 file->WriteString(CardV13_3_c_1);
				 file->WriteString(CardV13_3_c_2);
				
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

						if(el.m_uTopology==FETO_WEDGE6 || el.m_uTopology==FETO_WEDGE15)
						{
							// Top maj 2012
							intBTH = BTH;
	                        intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
							if (iINPT == 0)
								{
								str.Format("%5u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							else 
								{
								str.Format("%10u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							file->WriteString(str);
// Sneza 27.06.2014. promenjeno da stampa sestocvorni kao osmocvorni element. ponavlja se 3ci cvor kao 4ti i 6sti kao 8mi
//							for(k=0;k<6;k++) 
//							{
								if (iINPT == 0)
									{
	str.Format("%5u%5u%5u%5u%5u%5u%5u%5u",el.m_uNode[node_order[0]],el.m_uNode[node_order[1]],el.m_uNode[node_order[2]],el.m_uNode[node_order[2]],
		el.m_uNode[node_order[3]],el.m_uNode[node_order[4]],el.m_uNode[node_order[5]],el.m_uNode[node_order[5]]);
									}
								else 
									{
//									str.Format("%10u",el.m_uNode[node_order[k]]);
	str.Format("%10u%10u%10u%10u%10u%10u%10u%10u",el.m_uNode[node_order[0]],el.m_uNode[node_order[1]],el.m_uNode[node_order[2]],el.m_uNode[node_order[2]],
		el.m_uNode[node_order[3]],el.m_uNode[node_order[4]],el.m_uNode[node_order[5]],el.m_uNode[node_order[5]]);
									}
								file->WriteString(str);
//							}
							file->WriteString("\n");
							if(el.m_uTopology==FETO_WEDGE15)
							 for(k=6;k<15;k++) 
							 {
								 if (iINPT == 0)
									{
									str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
									str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							 }
							file->WriteString("\n");
						}
					}
				 }
				}
			
				if(uNE_TETRA>0)	// 3D Tetra Element
				{
//				 UINT node_order[10]={4,1,2,0,9,10,8,13,14,12};
			//  stampanje tetra kao u adini
//				 UINT node_order[10]={4,1,0,2,13,8,12,9,10,14};
					// stampanje "redom"
				 UINT node_order[10]={0,1,2,4,8,9,10,12,13,14};

				 if(uNE_3D>0 || uNE_PRISM>0) uNGELEM++;
				 //Card /13/
				 file->WriteString(CardH13);
				 file->WriteString(CardV13);


				 NMODM=m_PakOptValue.GetMaterialModel(uNETIP, GetModelTypeForElement(ell->m_uID));

				 // Top maj 2012
				 intINDBTH=INDBTH;
				 intINDDTH=INDDTH;
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					if( el.m_uColor>1024 && el.m_uColor<2048)
						{
								intINDBTH=1;
						}
					else if(el.m_uColor>2048)
						{
								intINDDTH=1;
						}
				 }
				 //
				 if (iINPT == 0)
					{
					 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
					 PAK_ISO_TETRA,uNE_TETRA,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
					}
				else 
					{
					 str.Format("%5u%10u%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",
					 PAK_ISO_TETRA,uNE_TETRA,m_PakOptValue.GetAnalysisType(),NMODM,intINDBTH,intINDDTH,INDKOV,0,COEF1,COEF2,COEF3);
					}
				 file->WriteString(str);
				 
				 file->WriteString(CardH13_3);
				 file->WriteString(CardV13_3_a_1);
				 file->WriteString(CardV13_3_a_2);

				 str.Format("%5u%5u%5u     %10.6f                                   %5d\n",
						4,1,1,BETA,-1);
//				 str.Format("%5u%5u%5u   %10.6f                                   %5d\n",
//					NGAUSX3,NGAUSY3,NGAUSZ3,BETA,IALFA);
				 file->WriteString(str);

				 file->WriteString(CardV13_3_b_1);
				 file->WriteString(CardV13_3_b_2);
				 file->WriteString(CardV13_3_c_1);
				 file->WriteString(CardV13_3_c_2);
				
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
				    if(m_PropArray[m_nPropIndex[m_ElArray[j].m_uPropID]].m_uType==key)
					{
						el=m_ElArray[j];
						pr = m_PropArray[m_nPropIndex[el.m_uPropID]];

						if(el.m_uTopology==FETO_TETRA4 || el.m_uTopology==FETO_TETRA10)
						{
							// Top maj 2012
							intBTH = BTH;
	                        intDTH = DTH;
							if( el.m_uColor>1024 && el.m_uColor<2048)
							{
								intBTH=el.m_uColor-1023;
							}
							else if(el.m_uColor>2048)
							{
								intDTH=el.m_uColor-2048;
							}
							// Top
							if (iINPT == 0)
								{
								str.Format("%5u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							else 
								{
								str.Format("%10u%5u%5u%5u%5u%5u          %10.6f%10.6f\n",
									el.m_uID,pr.m_uMatIID,IPRCO,ISNA1,IPGS,KORC,float(intBTH),float(intDTH));
								}
							file->WriteString(str);
							for(k=0;k<4;k++) 
							{
								if (iINPT == 0)
									{
									str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
									str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							}
							file->WriteString("\n");
							if(el.m_uTopology==FETO_TETRA10)
							 for(k=4;k<10;k++) 
							 {
								 if (iINPT == 0)
									{
									str.Format("%5u",el.m_uNode[node_order[k]]);
									}
								else 
									{
									str.Format("%10u",el.m_uNode[node_order[k]]);
									}
								file->WriteString(str);
							 }
							file->WriteString("\n");
						}
					}
				 }
				}
			}
			break;
		}
	}

	return(-1);
}

UINT CModelData::ExportPAK_Nodes(CStdioFile* file, int iINPT)
{

	CString str;
	CPAKString pstr;
	UINT i;
	HNodes nn;
	UINT ci,cj;
	int Constr[6];
	EqCoeff eqc0;

//Card /10/
 	file->WriteString(CardH10);
	file->WriteString(CardV10);
	for(i=0;i<(UINT)m_NodArray.GetSize();i++)
	{
		nn=m_NodArray[i];

		for(cj=0;cj<6;cj++) Constr[cj]=0;
		
		if(m_ConsArray.GetSize()>0)
		{
			for(ci=0;ci<(UINT)m_ConsArray[0].m_ConsNodes.GetSize();ci++)
				if(m_ConsArray[0].m_ConsNodes[ci].m_uNodeID==nn.m_uID)
				{
				 for(cj=0;cj<6;cj++) Constr[cj] = (m_ConsArray[0].m_ConsNodes[ci].m_bDOF[cj] ? 1 : 0);
				 break;
				}

			 //Check equations
			for(ci=0;ci<(UINT)m_ConsArray[0].m_ConsEqs.GetSize();ci++)
			{
				 for(cj=0;cj<6;cj++)
				 {
					eqc0=m_ConsArray[0].m_ConsEqs[ci].m_EqCoefs[0];
					if(eqc0.m_uEqn_nodeID==nn.m_uID && eqc0.m_uEqn_dof==cj+1) Constr[cj]=-1;
				 }
			}
		}

		if (iINPT == 0)
			{
			str.Format("%5u%c%2d%2d%2d%2d%2d%2d  ",
				nn.m_uID,CH,Constr[0],Constr[1],Constr[2],
						  Constr[3],Constr[4],Constr[5]);
			}
		else 
			{
			str.Format("%10u%c%2d%2d%2d%2d%2d%2d  ",
				nn.m_uID,CH,Constr[0],Constr[1],Constr[2],
						  Constr[3],Constr[4],Constr[5]);
			}
			file->WriteString(str);

		if (iINPT == 0)
			{
			if ((abs(nn.m_dX)>=1000.0)||(abs(nn.m_dY)>=1000.0)||(abs(nn.m_dZ)>=1000.0))
				{
					pstr.Format("%10.4.1e%10.4.1e%10.4.1e%5u%5u\n",
						nn.m_dX,nn.m_dY,nn.m_dZ,KORC,LJ); 
				}
			else
				{
					pstr.Format("%10.5f%10.5f%10.5f%5u%5u\n",
						nn.m_dX,nn.m_dY,nn.m_dZ,KORC,LJ);
				}
			}
		else 
			{
			if ((abs(nn.m_dX)>=1000.0)||(abs(nn.m_dY)>=1000.0)||(abs(nn.m_dZ)>=1000.0))
				{
					pstr.Format("%15.9.1e%15.9.1e%15.9.1e%5u%5u\n",
						nn.m_dX,nn.m_dY,nn.m_dZ,KORC,LJ); 
				}
			else
				{
					pstr.Format("%15.9f%15.9f%15.9f%5u%5u\n",
						nn.m_dX,nn.m_dY,nn.m_dZ,KORC,LJ);
				}
			}

//		pstr.Format("%5u%c%2d%2d%2d%2d%2d%2d  %10.4.1e%10.4.1e%10.4.1e%5u%5u\n",
//			nn.m_uID,CH,Constr[0],Constr[1],Constr[2],
//					  Constr[3],Constr[4],Constr[5],
//			nn.m_dX,nn.m_dY,nn.m_dZ,KORC,LJ);
		file->WriteString(pstr);

//		pstr.Format("%5u%c%2d%2d%2d%2d%2d%2d  %10.5.1e%10.5.1e%10.5.1e%5u%5u\n",
//			nn.m_uID,CH,nn.m_bPermbc[0],nn.m_bPermbc[1],nn.m_bPermbc[2],
//					  nn.m_bPermbc[3],nn.m_bPermbc[4],nn.m_bPermbc[5],
//			nn.m_dX,nn.m_dY,nn.m_dZ,KORC,LJ);
//		file->WriteString(pstr);
	}


	return(-1);
}

UINT CModelData::ExportPAK_MatModels(CStdioFile* file)
{

	CString str;
	CPAKString p_str;
	UINT i,k,j=0,jj=0,br_mat,jk=1,id_mat,ii;
	UINT uMODEL1,uModel1old;
	UINT imat[50];

	HMaterial mat;

	CMap<UINT,UINT,UINT,UINT> TypesOfMat, TypesOfMatPAK;

	TypesOfMat.RemoveAll();
	TypesOfMatPAK.RemoveAll();


	for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	{
// sneza promenila kad je dodato citanje Cam-clay-a i ostalih materijala type = 6 General
		if(m_MaterialsArray[i].m_uType != 6)
		{
			TypesOfMat[m_MaterialsArray[i].m_uType]++;
		}
		else //if(m_MaterialsArray[i].m_uType != 6)
		{
			TypesOfMat[m_MaterialsArray[i].m_uSubType]++;
		}
//		TypesOfMat[m_MaterialsArray[i].m_uType]++;
//		TypesOfMat[m_MaterialsArray[i].m_iNonlin_type]++;
	}


//Card /11/

	 	file->WriteString(CardH11);
		file->WriteString(CardV11);

		POSITION pos = TypesOfMat.GetStartPosition();
	
		while( pos != NULL )
		{
			UINT key, num;

			TypesOfMat.GetNextAssoc( pos, key, num);
		
			switch (key)
			{
			case FEMT_ISO: uMODEL1=PAKM_ELASTIC_ISO;
				break;
			case FEMT_2D_ORTHO: case FEMT_3D_ORTHO: uMODEL1=PAKM_ELASTIC_ORTHO;
				break;
//			case 6: 
// dodato da stampa Cam-Clay i ostale materijale definisane u femapu Tiye=6 General ima subType
//				{	switch (m_MaterialsArray[num].m_uSubType)
//					{
					case PAKM_CAM_CLAY: uMODEL1=9;
						break;
					case PAKM_MOHR_COULOMB: uMODEL1=42;
//					}
//				}
			} 
//			if (m_PakOptValue.GetAnalysisType()!=0) uMODEL1=3;
			if (key < 8) 
			{
				if (m_MaterialsArray[key].m_iNonlin_type==3) uMODEL1=PAKM_MISES_PLASTIC_ROD;

				if (m_MaterialsArray[key].m_dAlpha[0]!=0) uMODEL1=3;

				if ((uMODEL1>2)&&(m_PakOptValue.GetAnalysisType()==0))
				{
					AfxMessageBox(" Analysis type does not match model!");
					return(999);
				}
				
				if ((uMODEL1==5)&&(!m_bIsThereRod)) uMODEL1=6;

				if ( m_bExRamberg == true) uMODEL1=6;
			}

			str.Format("%5u%5u%5u\n",uMODEL1,num+jj,MODEL3);
			file->WriteString(str);

// pamti broj razlicitih materijala za materijalni model		
			jj=jj+1;
			imat[jj] = num;
		}	
		
	for(k=0;k<(UINT)m_MaterialsArray.GetSize();k++)
	{
		mat=m_MaterialsArray[k];
/*		switch (mat.m_uType)
		{
		case FEMT_ISO: uMODEL1=PAKM_ELASTIC_ISO;
			break;
		case FEMT_2D_ORTHO: case FEMT_3D_ORTHO: uMODEL1=PAKM_ELASTIC_ORTHO;
		}

	 	file->WriteString(CardH11);
		file->WriteString(CardV11);
		
		if (m_PakOptValue.GetAnalysisType()!=0) uMODEL1=5;
		
		str.Format("%5u%5u%5u\n",uMODEL1,MODEL2,MODEL3);

		file->WriteString(str);*/
	//Card /12/
//		for(i=1;i<=(UINT)m_MaterialsArray.GetSize();i++)
// dodato da stampa Cam-Clay i ostale materijale 
		switch (mat.m_uType)
		{
		case FEMT_ISO: uMODEL1=PAKM_ELASTIC_ISO;
			break;
		case FEMT_2D_ORTHO: case FEMT_3D_ORTHO: uMODEL1=PAKM_ELASTIC_ORTHO;
			break;
		case 6:
			switch (mat.m_uSubType)
			{
			case PAKM_CAM_CLAY:
				{
					uMODEL1=9;
					mat.m_dDensity=mat.m_dE[2];
				}
				break;
			case PAKM_MOHR_COULOMB:
				{
					uMODEL1=42;
					mat.m_dDensity=mat.m_dE[2];
				}
			}
		}
			if (mat.m_uType != 6) 
			{
				if (mat.m_iNonlin_type==3) uMODEL1=PAKM_MISES_PLASTIC_ROD;

				if (mat.m_dAlpha[0]!=0) uMODEL1=3;

				if ((uMODEL1==5)&&(!m_bIsThereRod)) uMODEL1=6;

				if ( m_bExRamberg == true) uMODEL1=6;
			}
		//		{
	 		file->WriteString(CardH12);
			file->WriteString(CardV12);

			if (k==0)
			{	uModel1old=uMODEL1;
//				jk=jk+1;
//				br_mat=imat[jk];
				j=1;
				id_mat=k+1;
				br_mat=0;
				}
// dodato da stampa vise istih materijala ako ima vise razlicitih tipova materijala
			else if((k>0)&&(uModel1old!=uMODEL1))
				{  
					jk=jk+1;
					br_mat=br_mat+imat[jk-1]; // broj razlicitih materijala prethodno odstampanog materijalnog modela
					uModel1old=uMODEL1;
					j=k+1; //-br_mat;  //razlika tekuceg materijala i br. materijala u prethodnim materijalnim modelima
					id_mat=1;
				} 
			else if((k>0)&&(uModel1old==uMODEL1))
				{  
					br_mat=br_mat+imat[jk-1]; // broj razlicitih materijala prethodno odstampanog materijalnog modela
					j=1;  //razlika tekuceg materijala i br. materijala u prethodnim materijalnim modelima
					id_mat=k+1;
				} 

				// stampa isti materijal dok ne
				for (ii=0;ii<j;ii++)
				{

				p_str.Format("%5u%5u%10.4.2e\n",uMODEL1,id_mat+ii,mat.m_dDensity);
				file->WriteString(p_str);
	/*			if (m_PakOptValue.GetAnalysisType()!=0)
				{
					str.Format("%5u\n",1);
					file->WriteString(str);			
				}
	*/
		//Card /12-1/
			
				if ( m_bExRamberg == true) //if checked option on Material Dialog
				{
					file->WriteString(CardH12_6);
					file->WriteString(CardH12_6_a1);
					file->WriteString(CardV12_6_a1);
					str.Format("    1\n");
					file->WriteString(str);
					file->WriteString(CardH12_6_a2);
					file->WriteString(CardV12_6_a2);
					str.Format("%10.3e%10.3e\n",mat.m_dE[0],mat.m_dNu[0]);
					file->WriteString(str);
					file->WriteString(CardH12_6_a3);
					file->WriteString("C  TAUY,   Cy,		HS\n");
					str.Format("%10.3e%10.3e  %10.5f\n",m_dExTAUY,m_dExCy,m_dExHS);
					file->WriteString(str);
					file->WriteString(CardH12_6_a4);
					file->WriteString(CardV12_6_a4);
					str.Format("%10.6f  %2.1f\n",m_dExAN,m_dExEM);
					file->WriteString(str);
				}
				else // old routine
				{

				switch (uMODEL1)
				{
				case  PAKM_ELASTIC_ISO: //1
					{
	label_elastic_iso:
						if (mat.m_dAlpha[0]!=0) goto label_thermo_elastic_iso;
						file->WriteString(CardH12_1);
						file->WriteString(CardV12_1_1);
						file->WriteString(CardV12_1_2);
						p_str.Format("%10.4.2e\n",mat.m_dE[0]);
						file->WriteString(p_str);
						file->WriteString(CardV12_1_3);
						file->WriteString(CardV12_1_4);
						p_str.Format("%10.4.2e\n",mat.m_dNu[0]);
						file->WriteString(p_str);
					}
					break;
				case  PAKM_THERMO_ELASTIC_ISO: //3
					{
	label_thermo_elastic_iso:
						file->WriteString(CardH12_14);
						file->WriteString(CardH12_14_a1);
						file->WriteString(CardV12_14_a1);
						str.Format("%5d\n",1);
						file->WriteString(str);
						file->WriteString(CardH12_14_a2);
						file->WriteString(CardV12_14_a2);
						p_str.Format("%10.2f%10.2.2e\n",2000.,mat.m_dE[0]);
						file->WriteString(p_str);
						file->WriteString(CardH12_14_b1);
						file->WriteString(CardV12_14_b1);
						str.Format("%5d\n",1);
						p_str.Format("%10.2f%10.2.2e\n",2000.,mat.m_dNu[0]);
						file->WriteString(str);
						file->WriteString(CardH12_14_b2);
						file->WriteString(CardV12_14_b2);
						file->WriteString(p_str);
						str.Format("%5d\n",1);
						p_str.Format("%10.2f%10.2.2e\n",2000.,mat.m_dAlpha[0]);
						file->WriteString(CardH12_14_c1);
						file->WriteString(CardV12_14_c1);
						file->WriteString(str);
						file->WriteString(CardH12_14_c2);
						file->WriteString(CardV12_14_c2);
						file->WriteString(p_str);
						file->WriteString(CardH12_14_e);
						file->WriteString(CardV12_14_e);
						str.Format("%10f\n",mat.m_dTemperature);
						file->WriteString(str);
					}
					break;
				case PAKM_ELASTIC_ORTHO: //2
					{
			 			file->WriteString(CardH12_2);
						file->WriteString(CardV12_2_1);
						file->WriteString(CardV12_2_2);
						str.Format("%10.3e%10.3e%10.3e\n",mat.m_dE[0],mat.m_dE[1],mat.m_dE[2]);
						file->WriteString(str);
						file->WriteString(CardV12_2_3);
						file->WriteString(CardV12_2_4);
						str.Format("%10.3e%10.3e%10.3e\n",mat.m_dNu[0],mat.m_dNu[1],mat.m_dNu[2]);
						file->WriteString(str);
						file->WriteString(CardV12_2_5);
						file->WriteString(CardV12_2_6);
						str.Format("%10.3e%10.3e%10.3e\n",mat.m_dG[0],mat.m_dG[1],mat.m_dG[2]);
						file->WriteString(str);
					}
					break;
				case  PAKM_MISES_PLASTIC_ROD: //5
					{
			 			if (mat.m_iNonlin_type==0) goto label_elastic_iso;
						file->WriteString(CardH12_5);
						file->WriteString(CardH12_5_a1);
						file->WriteString(CardV12_5_a1);
						str.Format("    1\n");
						file->WriteString(str);
						file->WriteString(CardH12_5_a2);
						file->WriteString(CardV12_5_a2);
						str.Format("%10.3e%10.3e\n",mat.m_dE[0],mat.m_dNu[0]);
						file->WriteString(str);
						file->WriteString(CardH12_5_a3);
						file->WriteString(CardV12_5_a3);
						str.Format("%10.3e%10.3e\n",mat.m_dPlastYieldLim[0],mat.m_dPlastHardSlope);
						file->WriteString(str);
					}
					break;
				case  PAKM_MISES_PLASTIC: //6 
					{
			 			if (mat.m_iNonlin_type==0) goto label_elastic_iso;
						file->WriteString(CardH12_6);
						file->WriteString(CardH12_6_a1);
						file->WriteString(CardV12_6_a1);
						str.Format("    1\n");
						file->WriteString(str);
						file->WriteString(CardH12_6_a2);
						file->WriteString(CardV12_6_a2);
						str.Format("%10.3e%10.3e\n",mat.m_dE[0],mat.m_dNu[0]);
						file->WriteString(str);
						file->WriteString(CardH12_6_a3);
						file->WriteString(CardV12_6_a3);
						str.Format("%10.3e%10.3e\n",mat.m_dPlastYieldLim[0],mat.m_dPlastHardSlope);
						file->WriteString(str);
						file->WriteString(CardH12_6_a4);
						file->WriteString(CardV12_6_a4);
						str.Format("%10.3e%10.3e\n",1.,1.);
						file->WriteString(str);
					}
					break;
				case 9: //PAKM_CAM_CLAY
					{
						file->WriteString(CardH12_9);
						file->WriteString(CardV12_9_1);
						str.Format("%10.3e%10.3e%10.3e%5u\n",mat.m_dE[0],mat.m_dE[1],mat.m_dG[0],mat.m_dG[1]);
						file->WriteString(str);
						file->WriteString(CardV12_9_2);
						file->WriteString(CardV12_9_3);
						str.Format("%10.3e%10.3e%10.3e%10.3e%10.3e\n",mat.m_dG[2],mat.m_dNu[0],mat.m_dNu[1],mat.m_dNu[2],mat.m_dGMatrix_3D[0]);
						file->WriteString(str);
					}
					break;
				case 42: //PAKM_MOHR_COULOMB
					{
						file->WriteString(CardH12_42);
						file->WriteString(CardV12_42_1);
						str.Format("%10.3e%10.3e\n",mat.m_dE[0],mat.m_dE[1]);
						file->WriteString(str);
						file->WriteString(CardV12_42_2);
						file->WriteString(CardV12_42_3);
						str.Format("%10.3e%10.3e%10.3e\n",mat.m_dGMatrix_3D[4],mat.m_dGMatrix_3D[5],mat.m_dGMatrix_3D[6]);
						file->WriteString(str);
					}
				}
				}
			}
//		}
	}

	return(-1);
}

UINT CModelData::ExportPAK_TimeFunctions(CStdioFile* file)
{
	CString str;
	CPAKString pstr;
	UINT i,j,uMAXTFT=0;

	//Ako nema nijedne vremenske funkcije ili ako ima gravitaciju dodaje
	// jednu konstantnu vremensku funkciju
	if(m_FunctionsArray.GetSize()==0 || m_LoadArray[0].m_bGrav_on)
	{
		HFunctions new_function;
		FunctionEntry fun_entry;

		fun_entry.m_dX=0.;
		fun_entry.m_dY=1.;
		new_function.m_FunctionEntry.Add(fun_entry);
		fun_entry.m_dX=10000.;
		fun_entry.m_dY=1.;
		new_function.m_FunctionEntry.Add(fun_entry);
		new_function.m_uID=m_FunctionsArray.GetSize()+1;
		m_FunctionsArray.Add(new_function);
	}

	for(i=0;i<(UINT)m_FunctionsArray.GetSize();i++)
	if((UINT)m_FunctionsArray[i].m_FunctionEntry.GetSize()>uMAXTFT)
		uMAXTFT=m_FunctionsArray[i].m_FunctionEntry.GetSize();


	file->WriteString(CardH14);
	file->WriteString(CardV14);
	str.Format("%5u%5u\n",m_FunctionsArray.GetSize(),MAXTFT);
	file->WriteString(str);
	for(i=0;i<(UINT)m_FunctionsArray.GetSize();i++)
	{
		file->WriteString(CardV14_1_a_1);
		file->WriteString(CardV14_1_a_2);
		str.Format("%5u%5u\n",m_FunctionsArray[i].m_uID,m_FunctionsArray[i].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV14_1_b_1);
		file->WriteString(CardV14_1_b_2);
		for(j=0;j<(UINT)m_FunctionsArray[i].m_FunctionEntry.GetSize();j++)
		{
			pstr.Format("%10.4.2e%10.4.1e\n",m_FunctionsArray[i].m_FunctionEntry[j].m_dX,
							 m_FunctionsArray[i].m_FunctionEntry[j].m_dY);
			file->WriteString(pstr);
		}
	}
	return (-1);
}

// stampanje kontaktnih elemenata
UINT CModelData::ExportPAK_ContactElements(CStdioFile* file, int iINPT)
{
	CString str;
	CPAKString p_str;
	UINT uNTSF,uNETIP=93,uNE2=0,NMODM=1,uType=2,uMAXCE=4,i,j,jj,k,uRegion,inode,temp,temp1;
	UINT IPENALTY=1;
	float AKN=1.e5,AKSP=0.e0;
	HCProperties cp;
	HConnection lo;
	HContactSegment cs;
	Entity en;
	HElement el;
	int i_nodNum[1000][2];
	
	UINT surface_nodes_3d[6][8]={   {0,3,2,1,11,10,9,8},
									{4,5,6,7,16,17,18,19},
									{0,1,5,4,8,13,16,12},
									{1,2,6,5,9,14,17,13},
									{2,3,7,6,10,15,18,14},
									{3,0,4,7,11,12,19,15}};

	UINT surface_nodes_tetra[4][8]={{0,2,1,1,10,9,1,8},
									{0,1,4,4,8,13,4,12},
									{1,2,4,4,9,14,4,13},
									{2,0,4,4,10,12,4,14}};
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

	UINT surface_nodes_tshell[2][8]={{0,2,1,1,5,4,1,3},
									{0,1,2,2,3,4,2,5}};
/*
Ovo su pritisci na linijama 2d elementa i elemenata ljuski i oni se pisu u
/15-2/ i ima ih NPP2. Kada su zadati pritisci na povrsinama ciji je broj veci 
od 2 onda su to linijski pritisci i treba ih staviti u ovu grupu za povrsine od 3 do 6.
Ovo je prva kartica: za ITIPE treba da je isti kao IETYP za 2d element.
						NFUN je broj funkcije a ako nema staviti 1
						IPRAV za sada je 0
						FAKP pritisci u cvoru 1 i 2
						THICV debljina koja je zadata u property kod elementa
						KORC = 0
*/
	UINT surface_nodes_2d[4][3]={   {0,1,4},
									{1,2,5},
									{2,3,6},
									{3,0,7}};

	UINT surface_nodes_t2d[3][3]={  {0,1,3},
									{1,2,4},
									{2,0,5}};

		// odredjivanje broja kontaktor cvorova - uNTSF
		inode=0;
		for(i=0;i<(UINT)m_ConnArray.GetSize();i++)
			{
				lo=m_ConnArray[i];
				uRegion=lo.m_uregionID[1];
				uRegion--;
				cs=m_ConnSegArray[uRegion];
				//ciljni segment
		//		uRegion=lo.m_uregionID[0];

				for(jj=0;jj<(UINT)cs.m_Entity.GetSize();jj++)
					{
					el=m_ElArray[m_nElemIndex[cs.m_Entity[jj].m_uEntityID]];
					j=cs.m_Entity[jj].m_uFaceID-1;
						
						switch(el.m_uTopology)
						{
						case FETO_BRICK8: case FETO_BRICK20:
						{
							for(k=0;k<(UINT)(el.m_uTopology==FETO_BRICK8 ? 4:8);k++)
							{
								i_nodNum[inode][0]=el.m_uNode[surface_nodes_3d[j][k]];
								i_nodNum[inode][1]=i+1;
								inode++;
							}
						}
							break;
						case FETO_TETRA4: case FETO_TETRA10:
						{
							for(k=0;k<(UINT)(el.m_uTopology==FETO_TETRA4 ? 4:8);k++)
							{
								i_nodNum[inode][0]=el.m_uNode[surface_nodes_3d[j][k]];
								i_nodNum[inode][1]=i+1;
								inode++;
							}
						}
							break;
						case FETO_QUAD4: case FETO_QUAD8: case FETO_TRI3: case FETO_TRI6:
							if(j<2)
							{
								for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 4:8);k++)
								{
			 						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
										i_nodNum[inode][0]=el.m_uNode[surface_nodes_shell[j][k]];
									else
										i_nodNum[inode][0]=el.m_uNode[surface_nodes_tshell[j][k]];
									i_nodNum[inode][1]=i+1;
									inode++;
								}
							}
							break;
						}
				}
			}
	// sortiranje niza i_nodeNum - bubbleSort
	for(i = inode - 1; i > 0; i--)
	{
		for(j = 0; j < i; j++)
		{
		  if(i_nodNum[j][0] > i_nodNum[j+1][0]) /* poredjenje susednih elmenata */
		  {
			/* zamena array[j] i array[j+1] */
			temp = i_nodNum[j][0];
			temp1 = i_nodNum[j][1];
			i_nodNum[j][0] = i_nodNum[j+1][0];
			i_nodNum[j+1][0] = temp;
			i_nodNum[j][1] = i_nodNum[j+1][1];
			i_nodNum[j+1][1] = temp1;
		  }
		}
	}
 // prepisivanje niza i_nodeNum u niz bez ponavljanja elemenata
	uNE2=inode;
		jj=0;
	for(i = 0; i< inode; i++)
	{
		/* brojanje istih cvorova na istom ciljnom segmentu */   
		if ((i_nodNum[i][0] == i_nodNum[i+1][0])&&(i_nodNum[i][1] == i_nodNum[i+1][1])) 
		  {
			  jj++;
		  }
	}
	uNE2=inode-jj;
	// zaglavlje za blok kontaktnih elemenata 
		file->WriteString(CardH13);
		file->WriteString(CardV13);
		str.Format("%5u%5u%5u%5u\n",uNETIP,uNE2,uType,NMODM);
		file->WriteString(str);

		file->WriteString(CardH13_93);
		file->WriteString(CardV13_93_a_1);
		file->WriteString(CardV13_93_a_2);

	//Card 13-93 a)
	 uNTSF=m_ConnArray.GetSize();
		p_str.Format("%5u     %10.6f%10.6f%10.4.1e%5u     %10.4.1e%10.4.1e\n",uNTSF,FMSTAT,FMDIN,EPSIL,IPENALTY,AKN,AKSP);
		file->WriteString(p_str);

	//Card 13-93 b)
		for(i=0;i<(UINT)m_ConnArray.GetSize();i++)
			{
				file->WriteString(CardV13_93_b1_1);
				file->WriteString(CardV13_93_b1_2);

				lo=m_ConnArray[i];
				uRegion=lo.m_uregionID[0];
				uRegion--;
				cs=m_ConnSegArray[uRegion];
				p_str.Format("%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",i+1,cs.m_Entity.GetSize(),ITSRF,uMAXCE,IORT,KORC,XP1,XP2,XP3);
				file->WriteString(p_str);

				file->WriteString(CardV13_93_b2_1);
				file->WriteString(CardV13_93_b2_2);
				
				for(jj=0;jj<(UINT)cs.m_Entity.GetSize();jj++)
					{
					el=m_ElArray[m_nElemIndex[cs.m_Entity[jj].m_uEntityID]];
					j=cs.m_Entity[jj].m_uFaceID-1;
						
						switch(el.m_uTopology)
						{
						case FETO_BRICK8: case FETO_BRICK20:
						{
							for(k=0;k<(UINT)(el.m_uTopology==FETO_BRICK8 ? 4:8);k++)
							{
								str.Format("%5u",el.m_uNode[surface_nodes_3d[j][k]]);
								file->WriteString(str);
							}
							file->WriteString("\n");
						}
							break;
						case FETO_TETRA4: case FETO_TETRA10:
						{
							for(k=0;k<(UINT)(el.m_uTopology==FETO_TETRA4 ? 4:8);k++)
							{
								str.Format("%5u",el.m_uNode[surface_nodes_tetra[j][k]]);
								file->WriteString(str);
							}
							file->WriteString("\n");
						}
							break;
						case FETO_QUAD4: case FETO_QUAD8: case FETO_TRI3: case FETO_TRI6:
							if(j<2)
							{
								for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 4:8);k++)
								{
			 						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
										str.Format("%5u",el.m_uNode[surface_nodes_shell[j][k]]);
									else
										str.Format("%5u",el.m_uNode[surface_nodes_tshell[j][k]]);
									file->WriteString(str);
								}
								file->WriteString("\n");
							}
							break;
						}
					}
			}
	//Card 13-93 c)
			file->WriteString(CardV13_93_c_1);
			file->WriteString(CardV13_93_c_2);

			for(i=0; i<inode; i++)
				{
//	 			   if ((i_nodNum[i][0] != i_nodNum[i+1][0])&&(i_nodNum[i][1] == i_nodNum[i+1][1]))
	 			   if (i_nodNum[i][0] != i_nodNum[i+1][0])
					{
						str.Format("%5u%5u%10.6f%10.6f\n",i_nodNum[i][0],i_nodNum[i][1],FSFD,FSFD);
						file->WriteString(str);
					  }

				}

/*		for(i=0;i<(UINT)m_ConnArray.GetSize();i++)
			{
				file->WriteString(CardV13_93_c_1);
				file->WriteString(CardV13_93_c_2);

				lo=m_ConnArray[i];
				uRegion=lo.m_uregionID[1];
				uRegion--;
				cs=m_ConnSegArray[uRegion];
				//ciljni segment
				uRegion=lo.m_uregionID[0];

				for(jj=0;jj<(UINT)cs.m_Entity.GetSize();jj++)
					{
					el=m_ElArray[m_nElemIndex[cs.m_Entity[jj].m_uEntityID]];
					j=cs.m_Entity[jj].m_uFaceID-1;
						
						switch(el.m_uTopology)
						{
						case FETO_BRICK8: case FETO_BRICK20:
						{
							for(k=0;k<(UINT)(el.m_uTopology==FETO_BRICK8 ? 4:8);k++)
							{
								str.Format("%5u%5u%10.6f%10.6f\n",el.m_uNode[surface_nodes_3d[j][k]],uRegion,FSFD,FSFD);
								file->WriteString(str);
							}
						}
							break;
						case FETO_TETRA4: case FETO_TETRA10:
						{
							for(k=0;k<(UINT)(el.m_uTopology==FETO_TETRA4 ? 4:8);k++)
							{
								str.Format("%5u%5u%10.6f%10.6f\n",el.m_uNode[surface_nodes_tetra[j][k]],uRegion,FSFD,FSFD);
								file->WriteString(str);
							}
						}
							break;
						}
				}
			} */
		
	return (-1);
}

UINT CModelData::ExportPAK_Loads(CStdioFile* file, int iINPT)
{
	CString str;
	CPAKString p_str;
	UINT uNCF=0,uNPP2=0,uNPP3=0,uNZADP=0,i,j,k;
	HLoads lo;
	StructLoad sl;
	HElement el;
	
	// za Card 15-10 absorbujuci granicni uslovi - 1.3.2014. (zadaje sa kao toplotni fluks)
	UINT uNPPF2=0,uNPPF3=0;
	bool ima;
	//
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
// bilo ranije promenjeno April 2015.
/*		UINT surface_nodes_tetra[4][8]={{0,2,1,1,10,9,1,8},
									{0,1,4,4,8,13,4,12},
									{1,2,4,4,9,14,4,13},
									{2,0,4,4,10,12,4,14}};
*/
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

	UINT surface_nodes_tshell[2][8]={{0,2,1,1,5,4,1,3},
									{0,1,2,2,3,4,2,5}};
/*
Ovo su pritisci na linijama 2d elementa i elemenata ljuski i oni se pisu u
/15-2/ i ima ih NPP2. Kada su zadati pritisci na povrsinama ciji je broj veci 
od 2 onda su to linijski pritisci i treba ih staviti u ovu grupu za povrsine od 3 do 6.
Ovo je prva kartica: za ITIPE treba da je isti kao IETYP za 2d element.
						NFUN je broj funkcije a ako nema staviti 1
						IPRAV za sada je 0
						FAKP pritisci u cvoru 1 i 2
						THICV debljina koja je zadata u property kod elementa
						KORC = 0

(1)(2)   1-5   	ITIPE(I)    Type of 2D element loaded by line loading (I)
                               						EQ. 0; PLANE STRESS
                               						EQ. 1; AXISYMMETRIC
                               						EQ. 2; PLANE STRAIN
(3)	     6-10   NFUN(I)   	Time function for line loading (I)
(7)	    11-15   IPRAV(I)  	Indicator for loading direction
                             						EQ. 0; Total line loading 
                                      						 in X-Y plane
                               						EQ. 1; X-direction
                               						EQ. 2; Y-direction
                               						EQ. 3; Z-direction
(3)    	16-25   	FAKP(I,1)   		Proportionality factor for node 1
(3)		26-35   	FAKP(I,2)   		Proportionality factor for node 2
(4)    	36-45   	THICV(I)    		Element thickness  for  line  (A-B) 
                            					subjected to line load (I) 
(5)    	46-50   	KORC        		Increment for data generation

Ovo je druga kartica: samo cvorovi po datom redosledu dole
(6)     1-5    	NODPR(I,1)  		First node of line (A-B)
(6)	    6-10   	NODPR(I,2)  		Second node of line (A-B)
(6)	   11-15  	NODPR(I,3)  		Third (middle) node of line (A-B)

								 
*/
	UINT surface_nodes_2d[4][3]={   {0,1,4},
									{1,2,5},
									{2,3,6},
									{3,0,7}};

	UINT surface_nodes_t2d[3][3]={  {0,1,3},
									{1,2,4},
									{2,0,5}};
	

	if(m_LoadArray.GetSize()==0) 
	{
//		AfxMessageBox(NO_LOADS);
		file->WriteString(CardH15);
		file->WriteString(CardV15);
		str.Format("\n");
		file->WriteString (str);

		return(0);
	}
	lo=m_LoadArray[0];	//Za sada samo prvi set
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
		sl=lo.m_StructLoads[i];
		switch(sl.m_uLoadtype)
		{
		case FELT_NODAL_FORCE:
			for(j=0;j<6;j++) if(sl.m_uDof_face[j] && (fabs(sl.m_dValue[j])>=LOAD_FORCE_TOL)) uNCF++;
		break;
		case FELT_ELEM_PRESS:
			{
			    j=sl.m_uDof_face[0]-1;
				if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
				{
			     if(j<2) uNPP3++;
				 else
				 {
					el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					if(	el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20 ||
						el.m_uTopology==FETO_TETRA4 || el.m_uTopology==FETO_TETRA10	) uNPP3++;
					else uNPP2++;
				 } 
				}
			}
		break;
		case FELT_NODAL_DISPL:
			for(j=0;j<6;j++) if(sl.m_uDof_face[j] && (fabs(sl.m_dValue[j])>=LOAD_NDISP_TOL)) uNZADP++;
		break;
 // odredjivanje broja elemenata na kojima je zadat fluks
		case FELT_ELEM_HEAT_FL:
			{
				j=sl.m_uDof_face[0]-1;
				if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
					{
						 if(j<2) uNPPF3++;
					 else
						 {
							el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
							if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20) uNPPF3++;
							else uNPPF2++;
						 } 
					}
			}
		break;
		}
	}
	//Card 15
		file->WriteString(CardH15);
		file->WriteString(CardV15);
	if (iINPT == 0)
		{
			str.Format("%5u%5u%5u%5u     %5u%5u%5u%5u%5d%5u\n",uNCF,uNPP2,uNPP3,NPGR,NPLJ,lo.m_NodalTemps.GetSize(),uNZADP,lo.m_bGrav_on,ICERNE,uNPPF3);
		}
	else
		{
			str.Format("%10u%10u%10u%10u          %10u%10u%10u%10u%10d%10u\n",uNCF,uNPP2,uNPP3,NPGR,NPLJ,lo.m_NodalTemps.GetSize(),uNZADP,lo.m_bGrav_on,ICERNE,uNPPF3);
		}
		file->WriteString(str);
	
	//Card 15-1
		if(uNCF>0)
		{
			file->WriteString(CardH15_1);
			file->WriteString(CardV15_1);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_NODAL_FORCE)
					for(j=0;j<6;j++) 
					 if(sl.m_uDof_face[j] && fabs(sl.m_dValue[j])>=LOAD_FORCE_TOL)
					 {
						UINT nc;
						nc=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
						if (iINPT == 0)
							{
								p_str.Format("%5u%5u%5u%10.4.1e%5u%10.6f\n",sl.m_uLoadID,j+1,nc,sl.m_dValue[j],KORC,(double)(FPOM));
							}
						else
							{
								p_str.Format("%10u%5u%5u%10.4.1e%5u%10.6f\n",sl.m_uLoadID,j+1,nc,sl.m_dValue[j],KORC,(double)(FPOM));
							}
						file->WriteString(p_str);
					 }
			}
		}

	//Card 15-2
	//Line pressure
		if(uNPP2>0)
		{
			UINT uITIPE;
			file->WriteString(CardH15_2);
			file->WriteString(CardV15_2_a_1);
			file->WriteString(CardV15_2_a_2);
			file->WriteString(CardV15_2_b_1);
			file->WriteString(CardV15_2_b_2);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_ELEM_PRESS)
				{
				   el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
				   if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8 ||
					  el.m_uTopology==FETO_TRI3 || el.m_uTopology==FETO_TRI6 )
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
						
						UINT nfun;
						nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
						p_str.Format("%5u%5u%5u%10.4.1e%10.4.1e%10.4.1e%5u%10u\n",uITIPE,nfun,IPRAV,
							sl.m_dValue[0], sl.m_dValue[0], pr.m_dValue[0], KORC,el.m_uID);
						file->WriteString(p_str);
						for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 2:3);k++)
						{
							if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
								str.Format("%5u",el.m_uNode[surface_nodes_2d[j-2][k]]);
							else
								str.Format("%5u",el.m_uNode[surface_nodes_t2d[j-2][k]]);
							file->WriteString(str);
						}
						file->WriteString("\n");
					}
				   }
				}
			}
		}
		
	//Card 15-3
	//Surface pressure
		if(uNPP3>0)
		{
			file->WriteString(CardH15_3);
			file->WriteString(CardV15_3_a_1);
			file->WriteString(CardV15_3_a_2);
			file->WriteString(CardV15_3_b_1);
			file->WriteString(CardV15_3_b_2);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_ELEM_PRESS)
				{
					el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					j=sl.m_uDof_face[0]-1;
					 if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
					 {
						UINT nfun;
						nfun=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
						
						switch(el.m_uTopology)
						{
						case FETO_BRICK8: case FETO_BRICK20:
						{
							p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u%10u\n",nfun,IPRAV,
						 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC,el.m_uID);
							file->WriteString(p_str);
							for(k=0;k<(UINT)(el.m_uTopology==FETO_BRICK8 ? 4:8);k++)
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
						case FETO_TETRA4: case FETO_TETRA10:
						{
							//int IPRAV1=sl.m_dDirection[1]+1;
							p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u%10u\n",nfun,IPRAV,
						 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC,el.m_uID);
							file->WriteString(p_str);
							for(k=0;k<(UINT)(el.m_uTopology==FETO_TETRA4 ? 4:8);k++)
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
								p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u%10u\n",nfun,IPRAV,
							 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC,el.m_uID);
								file->WriteString(p_str);
								for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 4:8);k++)
								{
								if(iINPT==0)
									{
			 						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
										str.Format("%5u",el.m_uNode[surface_nodes_shell[j][k]]);
									else
										str.Format("%5u",el.m_uNode[surface_nodes_tshell[j][k]]);
									}
								else
									{
			 						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
										str.Format("%10u",el.m_uNode[surface_nodes_shell[j][k]]);
									else
										str.Format("%10u",el.m_uNode[surface_nodes_tshell[j][k]]);
									}
									file->WriteString(str);
								}
								file->WriteString("\n");
							}
							break;
						}
					 }
				}
			}
		}
	
	//Card 15-7
		if(lo.m_NodalTemps.GetSize()>0)
		{
			file->WriteString(CardH15_7);
			file->WriteString(CardV15_7);
		 	for(i=0;i<(UINT)lo.m_NodalTemps.GetSize();i++)
			{
				UINT nc;
				//if (lo.m_NodalTemps[i].m_uNdt_funcID<1) nc=1;
				
	
                if(iINPT==0)
				{
				nc=(lo.m_NodalTemps[i].m_uNdt_funcID ? lo.m_NodalTemps[i].m_uNdt_funcID : 1);
					str.Format("%5d%5d%10.6f%5d\n",lo.m_NodalTemps[i].m_uNdtempID,nc,
						lo.m_NodalTemps[i].m_dNdtemp,KORC);
				}
				else
				{
				nc=(lo.m_NodalTemps[i].m_uNdt_funcID ? lo.m_NodalTemps[i].m_uNdt_funcID : 1);
					str.Format("%10d%5d%10.6f%5d\n",lo.m_NodalTemps[i].m_uNdtempID,nc,
						lo.m_NodalTemps[i].m_dNdtemp,KORC);
				}
				file->WriteString(str);
			}
		}
	
	//Card 15-8
		if(uNZADP>0)
		{
			file->WriteString(CardH15_8);
			file->WriteString(CardV15_8);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_NODAL_DISPL)
					for(j=0;j<6;j++) 
					 if(sl.m_uDof_face[j] && fabs(sl.m_dValue[j])>=LOAD_NDISP_TOL)
					 {
						UINT nc;
						nc=(sl.m_uSl_funcID ? sl.m_uSl_funcID : 1);
//						str.Format("%5u%5u%5u%10.4f%5u\n",sl.m_uLoadID,j+1,nc,sl.m_dValue[j],KORC);
						p_str.Format("%5u%5u%5u%10.4.1e%5u\n",sl.m_uLoadID,j+1,nc,sl.m_dValue[j],KORC);
						file->WriteString(p_str);
					 }
			}
	}

	//Card 15-9
	if(lo.m_bGrav_on)
	{
		UINT fun;
		file->WriteString(CardH15_9);
		file->WriteString(CardV15_9);
		if (lo.m_dGrav[0] != 0.0) fun=lo.m_dGrav_fun[0];
		if (lo.m_dGrav[1] != 0.0) fun=lo.m_dGrav_fun[1];
		if (lo.m_dGrav[2] != 0.0) fun=lo.m_dGrav_fun[2];
		if(fun == 0) fun=m_FunctionsArray.GetSize();
		str.Format("%5u%10.4f%10.4f%10.4f\n",fun,lo.m_dGrav[0],lo.m_dGrav[1],lo.m_dGrav[2]);
		file->WriteString(str);
	}
  
	//Card 15-10 absorbujuci granicni uslovi - 1.3.2014. (zadaje sa kao toplotni fluks)

	/*	if(uNPPF2>0) //  Line pressure
						if(uNPPF2>0)
						{
							UINT uITIPE;
							file->WriteString(CardH13_2);
							file->WriteString(CardV13_2);
							for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
							{
								sl=lo.m_StructLoads[i];
								if(sl.m_uLoadtype==FELT_ELEM_HEAT_FL || sl.m_uLoadtype==FELT_ELEM_CONV || sl.m_uLoadtype==FELT_ELEM_RADIATION)
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
										str.Format("%5d",el.m_uID);
										file->WriteString(str);
										for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 2:3);k++)
										{
											if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
												str.Format("%5u",el.m_uNode[surface_nodes_2d[j-2][k]]);
											else
												str.Format("%5u",el.m_uNode[surface_nodes_t2d[j-2][k]]);
											file->WriteString(str);
										}
									   }
									}
								}
							} */
							
						// Surface pressure
							if(uNPPF3>0)
							{
								file->WriteString(CardH15_10);
								file->WriteString(CardV15_10);
		 						for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
								{
									sl=lo.m_StructLoads[i];
									if(sl.m_uLoadtype==FELT_ELEM_HEAT_FL)
									{
										el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
										j=sl.m_uDof_face[0]-1;
										 if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
										 {
											switch(el.m_uTopology)
											{
											case FETO_BRICK8: case FETO_BRICK20:
											{
												str.Format("%5d",el.m_uID);
												file->WriteString(str);
												for(k=0;k<(UINT)(el.m_uTopology==FETO_BRICK8 ? 4:8);k++)
												{
													str.Format("%5u\n",el.m_uNode[surface_nodes_3d[j][k]]);
													file->WriteString(str);
												}
											}
												break;
											case FETO_QUAD4: case FETO_QUAD8: case FETO_TRI3: case FETO_TRI6:
												if(j<2)
												{
													str.Format("%5d",el.m_uID);
													file->WriteString(str);
													for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 4:8);k++)
													{
			 											if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
															str.Format("%5u\n",el.m_uNode[surface_nodes_shell[j][k]]);
														else
															str.Format("%5u\n",el.m_uNode[surface_nodes_tshell[j][k]]);
														file->WriteString(str);
													}
											
												}
												break;
											}
										 }
									}
								}
							}

	// kraj absorbujucih granicnih uslova - 1.3.2014

	return (-1);
}

UINT CModelData::ExportPAK_Loads_FindType(HMaterial* mat, int uMODEL1)
{
	if (m_PakOptValue.GetAnalysisType()!=0) uMODEL1=5;

			switch (uMODEL1)
			{
			case  PAKM_ELASTIC_ISO:
				{
_label_elastic_iso:
					if (mat->m_dAlpha[0]!=0) goto _label_thermo_elastic_iso;
					uMODEL1=1;
				}
				break;
			case  PAKM_THERMO_ELASTIC_ISO:
				{
_label_thermo_elastic_iso:
					uMODEL1=3;
				}
				break;
			case PAKM_ELASTIC_ORTHO:
				{
					uMODEL1=2;
				}
			case  PAKM_MISES_PLASTIC:
				{
			 		if (mat->m_iNonlin_type==0) goto _label_elastic_iso;
				}
			}

	return uMODEL1;
}

UINT CModelData::ExportPAK_Equations(CStdioFile* file, int iINPT)
{
	CString str;
	UINT i,j;
	
	//Za sada samo prvi set
	HConstraints &co=m_ConsArray[0];;
	

	//Card /10-02/
	file->WriteString(CardH10_02);


	//Card /10-02_a/
	file->WriteString(CardH10_02_a);
	file->WriteString(CardV10_02_a);
	str.Format("%5u%5u\n",co.m_ConsEqs.GetSize(),6);
	file->WriteString(str);

	//Card /10-02_b/
	file->WriteString(CardH10_02_b);
	file->WriteString(CardV10_02_b);

	for(i=0;i<(UINT)co.m_ConsEqs.GetSize();i++)
	{
		ConsEq &eq=co.m_ConsEqs[i];

		str.Format("%5u",eq.m_uEqnID);
		file->WriteString(str);

		j=1;
		while(j<(UINT)eq.m_EqCoefs.GetSize() && eq.m_EqCoefs[j].m_uEqn_nodeID>0)
		{
			str.Format("%10.4f",-eq.m_EqCoefs[j].m_dCoeff);
			file->WriteString(str);
			j++;
		}
		file->WriteString("\n");
	}

	//Card /10-02_c/
	file->WriteString(CardH10_02_c);
	file->WriteString(CardV10_02_c);

	for(i=0;i<(UINT)co.m_ConsEqs.GetSize();i++)
	{
		ConsEq &eq=co.m_ConsEqs[i];

		str.Format("%5u",eq.m_uEqnID);
		file->WriteString(str);

		j=0;
		while(j<(UINT)eq.m_EqCoefs.GetSize() && eq.m_EqCoefs[j].m_uEqn_nodeID>0)
		{
			if(iINPT==0)
				{
					str.Format("%5u%5u",eq.m_EqCoefs[j].m_uEqn_nodeID,eq.m_EqCoefs[j].m_uEqn_dof);
				}
			else
				{
					str.Format("%10u%5u",eq.m_EqCoefs[j].m_uEqn_nodeID,eq.m_EqCoefs[j].m_uEqn_dof);
				}
			file->WriteString(str);
			j++;
		}
		file->WriteString("\n");
	}

	return(-1);
}
