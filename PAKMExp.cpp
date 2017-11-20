// PAKTExp.cpp: Functions for export to PAK's DAT file.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FemCon.h"
#include "ModelData.h"
#include "PAKMExp.h"
#include "BlockIDs.h"
#include "PakExpOpt.h"
#include "math.h"
#include "PakString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


UINT CModelData::ExportPAK_M(CStdioFile* file,CPakExpOpt *peo)
{

	CString str;
	int uNPER;
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
//Card /2/
	file->WriteString(CardH2);
	file->WriteString(CardV2);
	str.Format("%5u\n",INDFOR);
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
	str.Format("%5u%5u%5u%5u%10.6f%10.6f%5u\n",INTEB,INDSC,IFORM,MAXIT,EPSTA,EPSTR,NJRAP);
	file->WriteString(str);
//Card /5/
 	file->WriteString(CardH5);
	file->WriteString(CardV5);
	str.Format("%5u%5u%5u%15.8lf\n",IREST,NCZK,NCZR,VREMO);
	file->WriteString(str);
//Card /6/
    if (m_PakOptValue.GetDynamicAnIsSet())
	{
		if(uNPER>0)
		{
 		 file->WriteString(CardH6);
		 file->WriteString(CardV6);
		 file->WriteString(CardV6_a_1);
		 file->WriteString(CardV6_a_2);
		 for(i=1;i<=uNPER;i++)
		 {
			str.Format("%5u", m_PakOptValue.GetNumSteps(0));
			file->WriteString(str);
		 }
		 file->WriteString("\n");
		 file->WriteString(CardV6_b_1);
		 file->WriteString(CardV6_b_2);
		 for(i=1;i<=uNPER;i++)
		 {
			str.Format("%10.6f",m_PakOptValue.GetStep(0));
			file->WriteString(str);
		 }
		 file->WriteString("\n");
		}
	}

	ExportPAKM_Nodes(file);
//	ExportPAKM_Elements(file,&uNGELEM,peo);
// Na silu stampa sve elemente u jednu grupu
	ExportPAKM_Elements_Without_Groups(file,&uNGELEM,peo);

// Card /9/
	ExportPAKM_MatModels(file);
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
	
//Card /10/
	if(m_LoadArray[0].m_NodalTemps.GetSize()>0) ExportPAKM_TimeFunctions(file);
//Card /11/
	ExportPAKM_PrescTemp(file);

//Card /12/
	ExportPAKM_Loads(file);
/*
//Card /12-6/
	file->WriteString (CardH12_6);
	file->WriteString (CardV12_6);
    str.Format ("%10.6f\n",m_MaterialsArray[0].m_dTemperature);
    file->WriteString (str);
*/
//Card /16/
 	file->WriteString(CardH16);
	file->WriteString(CardV16);

//Upisivanje naknadno

	file->Seek(lGeneralDataPos,CFile::begin);
//	str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u",m_NodArray.GetSize(),m_PropArray.GetSize(),m_MaterialsArray.GetSize(),m_PakOptValue.GetDynamicAnIsSet(),uNPER,NPRINT,IANIZ,NULAZ,ITOR,IGRAF);
// Na silu Brj grupa elemenata je uvek 1
	str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u",m_NodArray.GetSize(),1,m_MaterialsArray.GetSize(),m_PakOptValue.GetDynamicAnIsSet(),uNPER,NPRINT,IANIZ,NULAZ,ITOR,IGRAF);
	file->WriteString(str);
	file->Seek(0,CFile::end);

	return(-1);
}

UINT CModelData::ExportPAKM_Elements(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo)
{
	CString str;
	UINT i,j,k,z;
	HProperties pr;
	CArray <UINT,UINT&> ElNum;
	CMap<UINT,UINT,UINT,UINT> PropIndex; // relation between Property ID and it's position in ElNum;
	HElement el;
	UINT uINDAX,uNETIP;
	UINT fn;
	HLoads lo;
	UINT HGfuncs[MAX_TF],uNQE=0;
	bool ima;

	//FEMAT Topology
	//Value          {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
	  UINT FEM_Top[]={2,0,3,6,4,8,4,6,8,1,10,15,20, 0, 0, 0};



  //Odredjivanje NQE
	if(m_LoadArray.GetSize()>0) 
	{
		lo=m_LoadArray[0];	//Za sada samo prvi set
	
		for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
		{
		 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_HEAT_GEN || !lo.m_StructLoads[i].m_uSl_funcID) continue;
		 ima=false;
		 for(j=0;j<uNQE;j++) if(lo.m_StructLoads[i].m_uSl_funcID==HGfuncs[j]) {ima=true;break;}
		 if(!ima) {HGfuncs[uNQE]=lo.m_StructLoads[i].m_uSl_funcID;uNQE++;}
		}
	}

	for(i=0;i<*uNGELEM;i++)
	{
		pr=m_PropArray[i];

		switch (pr.m_uType)
		{

//****************************************************************************
// ****************************** 2D Elementi *********************************
//****************************************************************************
		case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR:
		case FET_PLATE_LIN:case FET_PLATE_PAR:
		case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR:
		case FET_AXISYM_LIN:case FET_AXISYM_PAR:

			{
				UINT uNE_2D=0;
				UINT uNP2DMX;

				uNETIP=PAKT_ISO_2D;
				uINDAX=0;
				if(pr.m_uType==FET_AXISYM_LIN || pr.m_uType==FET_AXISYM_PAR) uINDAX=1;

				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						if(m_ElArray[j].m_uTopology==FETO_QUAD4 || m_ElArray[j].m_uTopology==FETO_QUAD8 ||
							m_ElArray[j].m_uTopology==FETO_TRI3 || m_ElArray[j].m_uTopology==FETO_TRI6) uNE_2D++;
					}

				if(uNE_2D>0)	//Cetvorougaoni 2D Element
				{
				 UINT node_order_quad[8]={0,1,2,3,4,5,6,7};
				 UINT node_order_tri[20]={0,1,2,2,4,5,2,6};
				 UINT *node_order;

				 //Card /8/

				 file->WriteString(CardH8);
				 file->WriteString(CardV8);
				 str.Format("%5u%5u%5u%5u\n",uNETIP,uNE_2D,uINDAX,IZIP);
				 file->WriteString(str);

				 file->WriteString(CardH8_2);
				 file->WriteString(CardV8_2_a_1);
				 file->WriteString(CardV8_2_a_2);
				 if(pr.m_uType==FET_MEMBRANE_LIN || pr.m_uType==FET_PLATE_LIN ||
					 pr.m_uType==FET_PLANESTRAIN_LIN || pr.m_uType==FET_AXISYM_LIN) uNP2DMX=4; else uNP2DMX=8;
				 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f\n",
					NMAT2D,MAT2D,uNP2DMX,IPR2DC,IPTG2,NGAU2X,NGAU2Y,NTHIC,pr.m_dValue[0]);
				 
				 file->WriteString(str);

				 file->WriteString(CardV8_2_b_1);
				 file->WriteString(CardV8_2_b_2);
				 file->WriteString(CardV8_2_c_1);
				 file->WriteString(CardV8_2_c_2);

				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8 ||
							el.m_uTopology==FETO_TRI3 || el.m_uTopology==FETO_TRI6)
						{
						   switch(el.m_uTopology)
							{
							 case FETO_QUAD4: case FETO_QUAD8: 
							  node_order=node_order_quad; break;
							 case FETO_TRI3: case FETO_TRI6: 
							  node_order=node_order_tri; break;
							}
						   
							str.Format("%5u",el.m_uID);
							file->WriteString(str);

							for(k=0;k<4;k++) 
							{
								str.Format("%5u",el.m_uNode[k]);
								file->WriteString(str);
							}


	//Odredjivanje NQE
	fn=0;
	for(k=0;k<(UINT)lo.m_StructLoads.GetSize();k++)
	{
	 if(lo.m_StructLoads[k].m_uLoadID==el.m_uID && lo.m_StructLoads[k].m_uLoadtype==FELT_ELEM_HEAT_GEN && lo.m_StructLoads[k].m_uSl_funcID) 
	 {
		for(z=0;z<uNQE;z++)
			if(HGfuncs[z]==lo.m_StructLoads[k].m_uSl_funcID)
			{
				fn=z+1;
				break;
			}
	 }
	}


							str.Format("%5u%5u%5u%5u%5u%5u%10.6f\n",
								pr.m_uMatIID,fn,KORE,NBEG,IPRCO,IPGS,pr.m_dValue[0]);
							file->WriteString(str);

							if(el.m_uTopology==FETO_QUAD8 || el.m_uTopology==FETO_TRI6)
							{
							 for(k=4;k<8;k++) 
							 {
								str.Format("%5u",el.m_uNode[k]);
								file->WriteString(str);
							 }
							 file->WriteString("\n");\
							}

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
				UINT uNE_3D=0,uNE_PRISM=0,uNE_TETRA=0;;
				UINT uNP3DMX;

				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20 ||
						   el.m_uTopology==FETO_WEDGE6 || el.m_uTopology==FETO_WEDGE15 ||
						   el.m_uTopology==FETO_TETRA4 || el.m_uTopology==FETO_TETRA10)	uNE_3D++;
					}

				if(uNE_3D>0)	// 3D Element
				{
				 UINT node_order_3d[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};
				 UINT node_order_prism[20]={5,6,4,4,1,2,0,0,17,18,4,16,9,10,0,8,13,14,12,12};
				 UINT node_order_tetra[20]={4,4,4,4,1,2,0,0,4,4,4,4,9,10,0,8,13,14,12,12};
				 UINT *node_order;

				 //Card /8/
				 file->WriteString(CardH8);
				 file->WriteString(CardV8);
				 str.Format("%5u%5u%5u%5u\n",
				 PAKT_ISO_3D,uNE_3D,INDAX,IZIP);
				 file->WriteString(str);
				 
				 file->WriteString(CardH8_3);
				 file->WriteString(CardV8_3_a_1);
				 file->WriteString(CardV8_3_a_2);
				 if(pr.m_uType==FET_3D_LIN) uNP3DMX=8; else uNP3DMX=20;
				 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u\n",
					NMAT3D,MAT3D,uNP3DMX,IPR3DC,IPRTG3,NGAUSX,NGAUSY,NGAUSZ);
				 file->WriteString(str);

				 file->WriteString(CardV8_3_b_1);
				 file->WriteString(CardV8_3_b_2);
				 file->WriteString(CardV8_3_c_1);
				 file->WriteString(CardV8_3_c_2);
				
				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20 ||
						   el.m_uTopology==FETO_WEDGE6 || el.m_uTopology==FETO_WEDGE15 ||
						   el.m_uTopology==FETO_TETRA4 || el.m_uTopology==FETO_TETRA10)
						{
							switch(el.m_uTopology)
							{
							 case FETO_BRICK8: case FETO_BRICK20: 
							  node_order=node_order_3d; break;
							 case FETO_WEDGE6: case FETO_WEDGE15: 
							  node_order=node_order_prism; break;
							 case FETO_TETRA4: case FETO_TETRA10: 
							  node_order=node_order_tetra; break;
							}
							
							str.Format("%5u",el.m_uID);
							file->WriteString(str);
							for(k=0;k<8;k++) 
							{
								str.Format("%5u",el.m_uNode[node_order[k]]);
								file->WriteString(str);
							}


	//Odredjivanje NQE
	fn=0;
	for(k=0;k<(UINT)lo.m_StructLoads.GetSize();k++)
	{
	 if(lo.m_StructLoads[k].m_uLoadID==el.m_uID && lo.m_StructLoads[k].m_uLoadtype==FELT_ELEM_HEAT_GEN && lo.m_StructLoads[k].m_uSl_funcID) 
	 {
		for(z=0;z<uNQE;z++)
			if(HGfuncs[z]==lo.m_StructLoads[k].m_uSl_funcID)
			{
				fn=z+1;
				break;
			}
	 }
	}

							str.Format("%5u%5u%5u%5u%5u%5u\n",pr.m_uMatIID,fn,KORE,NBEG,IPRCO,IPGS);
							file->WriteString(str);
							
							if(el.m_uTopology==FETO_BRICK20 || el.m_uTopology==FETO_WEDGE15 || el.m_uTopology==FETO_TETRA10)
							{
							 for(k=8;k<20;k++) 
							 {
								str.Format("%5u",el.m_uNode[node_order[k]]);
								file->WriteString(str);
							 }
							 file->WriteString("\n");
							}
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

//**************************************************************************
//**************************************************************************
// Ova funkcija eksportuje sve elemente u jednu grupu i to, naravno, nema pojma.
// Znaci, sve sto je u njoj uradjeno je na silu.
//**************************************************************************
UINT CModelData::ExportPAKM_Elements_Without_Groups(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo)
{
	CString str;
	UINT i,j,k,z;
	HProperties pr;
	CArray <UINT,UINT&> ElNum;
	CMap<UINT,UINT,UINT,UINT> PropIndex; // relation between Property ID and it's position in ElNum;
	HElement el;
	UINT uINDAX,uNETIP;
	UINT fn;
	HLoads lo;
	UINT HGfuncs[MAX_TF],uNQE=0;
	bool ima;

	//FEMAT Topology
	//Value          {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
	  UINT FEM_Top[]={2,0,3,6,4,8,4,6,8,1,10,15,20, 0, 0, 0};



  //Odredjivanje NQE
	if(m_LoadArray.GetSize()>0) 
	{
		lo=m_LoadArray[0];	//Za sada samo prvi set
	
		for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
		{
		 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_HEAT_GEN || !lo.m_StructLoads[i].m_uSl_funcID) continue;
		 ima=false;
		 for(j=0;j<uNQE;j++) if(lo.m_StructLoads[i].m_uSl_funcID==HGfuncs[j]) {ima=true;break;}
		 if(!ima) {HGfuncs[uNQE]=lo.m_StructLoads[i].m_uSl_funcID;uNQE++;}
		}
	}

	BOOL first_group=TRUE;

	for(i=0;i<*uNGELEM;i++)
	{
		pr=m_PropArray[i];

		switch (pr.m_uType)
		{

//****************************************************************************
// ****************************** 2D Elementi *********************************
//****************************************************************************
		case FET_MEMBRANE_LIN:case FET_MEMBRANE_PAR:
		case FET_PLATE_LIN:case FET_PLATE_PAR:
		case FET_PLANESTRAIN_LIN:case FET_PLANESTRAIN_PAR:
		case FET_AXISYM_LIN:case FET_AXISYM_PAR:

			{
				UINT uNE_2D=0;
				UINT uNP2DMX;

				uNETIP=PAKT_ISO_2D;
				uINDAX=0;
				if(pr.m_uType==FET_AXISYM_LIN || pr.m_uType==FET_AXISYM_PAR) uINDAX=1;

				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						if(m_ElArray[j].m_uTopology==FETO_QUAD4 || m_ElArray[j].m_uTopology==FETO_QUAD8 ||
							m_ElArray[j].m_uTopology==FETO_TRI3 || m_ElArray[j].m_uTopology==FETO_TRI6) uNE_2D++;
					}

				if(uNE_2D>0)	//Cetvorougaoni 2D Element
				{
				 UINT node_order_quad[8]={0,1,2,3,4,5,6,7};
				 UINT node_order_tri[20]={0,1,2,2,4,5,2,6};
				 UINT *node_order;

// Na silu stampa zaglavlje samo za prvu grupu
		if(first_group)
		{
				first_group=FALSE;
				 //Card /8/
				 file->WriteString(CardH8);
				 file->WriteString(CardV8);

		//		 str.Format("%5u%5u%5u%5u\n",uNETIP,uNE_2D,uINDAX,IZIP);
		//	Na silu stampa da je broj elemenata u grupi jednak ukupnom broju elemenata
				 str.Format("%5u%5u%5u%5u\n",uNETIP,m_ElArray.GetSize(),uINDAX,IZIP);
				 file->WriteString(str);

				 file->WriteString(CardH8_2);
				 file->WriteString(CardV8_2_a_1);
				 file->WriteString(CardV8_2_a_2);
				 if(pr.m_uType==FET_MEMBRANE_LIN || pr.m_uType==FET_PLATE_LIN ||
					 pr.m_uType==FET_PLANESTRAIN_LIN || pr.m_uType==FET_AXISYM_LIN) uNP2DMX=4; else uNP2DMX=8;
//				 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f\n",
//					NMAT2D,MAT2D,uNP2DMX,IPR2DC,IPTG2,NGAU2X,NGAU2Y,NTHIC,pr.m_dValue[0]);
				 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%10.6f\n",
					m_MaterialsArray.GetSize(),MAT2D,uNP2DMX,IPR2DC,IPTG2,NGAU2X,NGAU2Y,NTHIC,pr.m_dValue[0]);
				 
				 file->WriteString(str);

				 file->WriteString(CardV8_2_b_1);
				 file->WriteString(CardV8_2_b_2);
				 file->WriteString(CardV8_2_c_1);
				 file->WriteString(CardV8_2_c_2);
		}
// kraj stampanja zaglavlja samo za prvu grupu

				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8 ||
							el.m_uTopology==FETO_TRI3 || el.m_uTopology==FETO_TRI6)
						{
						   switch(el.m_uTopology)
							{
							 case FETO_QUAD4: case FETO_QUAD8: 
							  node_order=node_order_quad; break;
							 case FETO_TRI3: case FETO_TRI6: 
							  node_order=node_order_tri; break;
							}
						   
							str.Format("%5u",el.m_uID);
							file->WriteString(str);

							for(k=0;k<4;k++) 
							{
								str.Format("%5u",el.m_uNode[k]);
								file->WriteString(str);
							}


	//Odredjivanje NQE
	fn=0;
	for(k=0;k<(UINT)lo.m_StructLoads.GetSize();k++)
	{
	 if(lo.m_StructLoads[k].m_uLoadID==el.m_uID && lo.m_StructLoads[k].m_uLoadtype==FELT_ELEM_HEAT_GEN && lo.m_StructLoads[k].m_uSl_funcID) 
	 {
		for(z=0;z<uNQE;z++)
			if(HGfuncs[z]==lo.m_StructLoads[k].m_uSl_funcID)
			{
				fn=z+1;
				break;
			}
	 }
	}


							str.Format("%5u%5u%5u%5u%5u%5u%10.6f\n",
								pr.m_uMatIID,fn,KORE,NBEG,IPRCO,IPGS,pr.m_dValue[0]);
							file->WriteString(str);

							if(el.m_uTopology==FETO_QUAD8 || el.m_uTopology==FETO_TRI6)
							{
							 for(k=4;k<8;k++) 
							 {
								str.Format("%5u",el.m_uNode[k]);
								file->WriteString(str);
							 }
							 file->WriteString("\n");\
							}

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
				UINT uNE_3D=0,uNE_PRISM=0,uNE_TETRA=0;;
				UINT uNP3DMX;

				for(j=0;j<(UINT)m_ElArray.GetSize();j++)
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20 ||
						   el.m_uTopology==FETO_WEDGE6 || el.m_uTopology==FETO_WEDGE15 ||
						   el.m_uTopology==FETO_TETRA4 || el.m_uTopology==FETO_TETRA10)	uNE_3D++;
					}

				if(uNE_3D>0)	// 3D Element
				{
				 UINT node_order_3d[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};
				 UINT node_order_prism[20]={5,6,4,4,1,2,0,0,17,18,4,16,9,10,0,8,13,14,12,12};
				 UINT node_order_tetra[20]={4,4,4,4,1,2,0,0,4,4,4,4,9,10,0,8,13,14,12,12};
				 UINT *node_order;

// Na silu stampa zaglavlje samo za prvu grupu
		if(first_group)
		{
				first_group=FALSE;
				 //Card /8/
				 file->WriteString(CardH8);
				 file->WriteString(CardV8);
		//		 str.Format("%5u%5u%5u%5u\n",PAKT_ISO_3D,uNE_3D,INDAX,IZIP);
		//	Na silu stampa da je broj elemenata u grupi jednak ukupnom broju elemenata
				 str.Format("%5u%5u%5u%5u\n",PAKT_ISO_3D,m_ElArray.GetSize(),INDAX,IZIP);
				 file->WriteString(str);
				 
				 file->WriteString(CardH8_3);
				 file->WriteString(CardV8_3_a_1);
				 file->WriteString(CardV8_3_a_2);
				 if(pr.m_uType==FET_3D_LIN) uNP3DMX=8; else uNP3DMX=20;
//				 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u\n",
//					NMAT3D,MAT3D,uNP3DMX,IPR3DC,IPRTG3,NGAUSX,NGAUSY,NGAUSZ);
				 str.Format("%5u%5u%5u%5u%5u%5u%5u%5u\n",
					m_MaterialsArray.GetSize(),MAT3D,uNP3DMX,IPR3DC,IPRTG3,NGAUSX,NGAUSY,NGAUSZ);

				 file->WriteString(str);

				 file->WriteString(CardV8_3_b_1);
				 file->WriteString(CardV8_3_b_2);
				 file->WriteString(CardV8_3_c_1);
				 file->WriteString(CardV8_3_c_2);
		}
// kraj stampanja zaglavlja samo za prvu grupu

				 for(j=0;j<(UINT)m_ElArray.GetSize();j++)
				 {
					if(m_ElArray[j].m_uPropID==pr.m_uID)
					{
						el=m_ElArray[j];
						if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20 ||
						   el.m_uTopology==FETO_WEDGE6 || el.m_uTopology==FETO_WEDGE15 ||
						   el.m_uTopology==FETO_TETRA4 || el.m_uTopology==FETO_TETRA10)
						{
							switch(el.m_uTopology)
							{
							 case FETO_BRICK8: case FETO_BRICK20: 
							  node_order=node_order_3d; break;
							 case FETO_WEDGE6: case FETO_WEDGE15: 
							  node_order=node_order_prism; break;
							 case FETO_TETRA4: case FETO_TETRA10: 
							  node_order=node_order_tetra; break;
							}
							
							str.Format("%5u",el.m_uID);
							file->WriteString(str);
							for(k=0;k<8;k++) 
							{
								str.Format("%5u",el.m_uNode[node_order[k]]);
								file->WriteString(str);
							}


	//Odredjivanje NQE
	fn=0;
	for(k=0;k<(UINT)lo.m_StructLoads.GetSize();k++)
	{
	 if(lo.m_StructLoads[k].m_uLoadID==el.m_uID && lo.m_StructLoads[k].m_uLoadtype==FELT_ELEM_HEAT_GEN && lo.m_StructLoads[k].m_uSl_funcID) 
	 {
		for(z=0;z<uNQE;z++)
			if(HGfuncs[z]==lo.m_StructLoads[k].m_uSl_funcID)
			{
				fn=z+1;
				break;
			}
	 }
	}

							str.Format("%5u%5u%5u%5u%5u%5u\n",pr.m_uMatIID,fn,KORE,NBEG,IPRCO,IPGS);
							file->WriteString(str);
							
							if(el.m_uTopology==FETO_BRICK20 || el.m_uTopology==FETO_WEDGE15 || el.m_uTopology==FETO_TETRA10)
							{
							 for(k=8;k<20;k++) 
							 {
								str.Format("%5u",el.m_uNode[node_order[k]]);
								file->WriteString(str);
							 }
							 file->WriteString("\n");
							}
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


UINT CModelData::ExportPAKM_Nodes(CStdioFile* file)
{

	CString str;
	UINT i,ci;
	HNodes nn;
	INT idt;

//Card /7/
 	file->WriteString(CardH7);
	file->WriteString(CardV7);
	for(i=0;i<(UINT)m_NodArray.GetSize();i++)
	{
		nn=m_NodArray[i];
		idt=0;
		if(m_LoadArray.GetSize()>0)
			for(ci=0;ci<(UINT)m_LoadArray[0].m_NodalTemps.GetSize();ci++)
				if(m_LoadArray[0].m_NodalTemps[ci].m_uNdtempID==nn.m_uID)
				{
				 if(m_LoadArray[0].m_NodalTemps[ci].m_dNdtemp>0) idt=-1; else idt=1;
				 break;
				}

		str.Format("%5u   %2d%10.5f%10.5f%10.5f          %5u\n",
			nn.m_uID,idt,nn.m_dX,nn.m_dY,nn.m_dZ,KORC);
		file->WriteString(str);
	}


	return(-1);
}

UINT CModelData::ExportPAKM_MatModels(CStdioFile* file)
{

	CString str;
	UINT CONfuncs[MAX_TF],TCfuncs[MAX_TF],uNTABK=0,uNTABC=0,uMAXTK=0,uMAXTC;
	UINT i,j,uNTAKV,uNTACV;

//	bool ima; // ne koristi se. koristilo se u delu pod komentarom


	//Card /9/
	file->WriteString(CardH9);
	file->WriteString(CardV9);

	for(i=0;i<(UINT)m_FunctionsArray.GetSize();i++)
		if((UINT)m_FunctionsArray[i].m_FunctionEntry.GetSize()>uMAXTK)
			uMAXTK=m_FunctionsArray[i].m_FunctionEntry.GetSize();
/*
//Odredjivanje NTABK
	for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	{
	 if(!m_MaterialsArray[i].m_uFK[0]) continue;
	 ima=false;
	 for(j=0;j<uNTABK;j++) if(m_MaterialsArray[i].m_uFK[0]==CONfuncs[j]) {ima=true;break;}
	 if(!ima) {CONfuncs[uNTABK]=m_MaterialsArray[i].m_uFK[0];uNTABK++;}
	}
//Odredjivanje NTABC
	for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	{
	 if(!m_MaterialsArray[i].m_uFThermal_cap) continue;
	 ima=false;
	 for(j=0;j<uNTABC;j++) if(m_MaterialsArray[i].m_uFThermal_cap==TCfuncs[j]) {ima=true;break;}
	 if(!ima) {TCfuncs[uNTABC]=m_MaterialsArray[i].m_uFThermal_cap;uNTABC++;}
	}
*/
// NTABK i NTABC bi trebali da budu jednaki broju materijala
	uNTABK=(UINT)m_MaterialsArray.GetSize();

	uMAXTC=uMAXTK;
    if (m_PakOptValue.GetDynamicAnIsSet())
	{
		uNTABC=(UINT)m_MaterialsArray.GetSize();
		str.Format("%5u%5u%5u%5u%5u%5u\n",NANLK,uNTABK,uMAXTK,NANLC,uNTABC,uMAXTC);
	}
	else 
	{
		uNTABC=0;
		str.Format("%5u%5u%5u%5u\n",NANLK,uNTABK,uMAXTK,NANLC);
	}

	file->WriteString(str);

	//Card /9-1/
	file->WriteString(CardH9_1);
	file->WriteString(CardV9_1);

	for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	{
//		str.Format("%5u%5u%5u%10.6f%10.6f%10.6f%10.6f%10.6f%10.6f\n",
//			m_MaterialsArray[i].m_uFK[0],IBFK,IBFK,FAKP,FAKP,FAKP,m_MaterialsArray[i].m_dDensity,TOPM,TMNM);
		str.Format("%5u%5u%5u%10.6f%10.6f%10.6f%10.6f%10.4f%10.6f\n",
			m_MaterialsArray[i].m_uID,IBFK,IBFK,FAKP,FAKP,FAKP,m_MaterialsArray[i].m_dDensity,TOPM,TMNM);
		file->WriteString(str);
	}

	//Card /9-2/
	for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	{
	  	file->WriteString(CardH9_2);
	  	file->WriteString(CardV9_2_a_1);
		file->WriteString(CardV9_2_a_2);
		if(m_MaterialsArray[i].m_uFK[0])
			uNTAKV=m_FunctionsArray[m_nFuncIndex[m_MaterialsArray[i].m_uFK[0]]].m_FunctionEntry.GetSize();
		else uNTAKV=1;

		str.Format("%5u%5u\n",m_MaterialsArray[i].m_uID,uNTAKV);
		file->WriteString(str);

	  	file->WriteString(CardV9_2_b_1);
		file->WriteString(CardV9_2_b_2);
		if(m_MaterialsArray[i].m_uFK[0])
		{
			for(j=0;j<uNTAKV;j++)
			{
				str.Format("%10.6f%10.6f\n",m_FunctionsArray[m_nFuncIndex[m_MaterialsArray[i].m_uFK[0]]].m_FunctionEntry[i].m_dX,
					m_FunctionsArray[m_nFuncIndex[m_MaterialsArray[i].m_uFK[0]]].m_FunctionEntry[i].m_dY);
			}
			file->WriteString(str);
		} 
		else
		{
			str.Format("%10.6f%10.6f\n",0.,m_MaterialsArray[i].m_dK[0]);
			file->WriteString(str);
		}
	}

	if (m_PakOptValue.GetDynamicAnIsSet())
	{
		//Card /9-3/
		for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
		{
	  		file->WriteString(CardH9_3);
	  		file->WriteString(CardV9_3_a_1);
			file->WriteString(CardV9_3_a_2);
			if(m_MaterialsArray[i].m_uFK[0])
				uNTACV=m_FunctionsArray[m_nFuncIndex[m_MaterialsArray[i].m_uFThermal_cap]].m_FunctionEntry.GetSize();
			else uNTACV=1;

			str.Format("%5u%5u\n",m_MaterialsArray[i].m_uID,uNTACV);
			file->WriteString(str);

	  		file->WriteString(CardV9_3_b_1);
			file->WriteString(CardV9_3_b_2);
			if(m_MaterialsArray[i].m_uFK[0])
			{
				for(j=0;j<uNTACV;j++)
				{
					str.Format("%10.6f%10.6f\n",m_FunctionsArray[m_nFuncIndex[m_MaterialsArray[i].m_uFThermal_cap]].m_FunctionEntry[i].m_dX,
						m_FunctionsArray[m_nFuncIndex[m_MaterialsArray[i].m_uFThermal_cap]].m_FunctionEntry[i].m_dY);
				}
				file->WriteString(str);
			}
			else
			{
				str.Format("%10.6f%10.6f\n",0.,m_MaterialsArray[i].m_dThermal_cap);
				file->WriteString(str);
			}
		}
	}

	return(-1);
}


UINT CModelData::ExportPAKM_TimeFunctions(CStdioFile* file)
{
	CString str;
	CPAKString pstr;
	UINT i,j,uMAXTFT=0;

	if(m_FunctionsArray.GetSize()==0)
	{
		HFunctions new_function;
		FunctionEntry fun_entry;

		fun_entry.m_dX=0.;
		fun_entry.m_dY=1.;
		new_function.m_FunctionEntry.Add(fun_entry);
		fun_entry.m_dX=10000.;
		fun_entry.m_dY=1.;
		new_function.m_FunctionEntry.Add(fun_entry);
		new_function.m_uID=1;
		m_FunctionsArray.Add(new_function);
	}

	for(i=0;i<(UINT)m_FunctionsArray.GetSize();i++)
	if((UINT)m_FunctionsArray[i].m_FunctionEntry.GetSize()>uMAXTFT)
		uMAXTFT=m_FunctionsArray[i].m_FunctionEntry.GetSize();


	file->WriteString(CardH10);
	file->WriteString(CardV10);
	str.Format("%5u%5u\n",m_FunctionsArray.GetSize(),MAXTFT);
	file->WriteString(str);
	for(i=0;i<(UINT)m_FunctionsArray.GetSize();i++)
	{
		file->WriteString(CardV10_1_a_1);
		file->WriteString(CardV10_1_a_2);
		str.Format("%5u%5u\n",m_FunctionsArray[i].m_uID,m_FunctionsArray[i].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV10_1_b_1);
		file->WriteString(CardV10_1_b_2);
		for(j=0;j<(UINT)m_FunctionsArray[i].m_FunctionEntry.GetSize();j++)
		{
			pstr.Format("%10.4.1e%10.4.1e\n",m_FunctionsArray[i].m_FunctionEntry[j].m_dX,
										     m_FunctionsArray[i].m_FunctionEntry[j].m_dY);
			file->WriteString(pstr);
		}
	}
	return (-1);
}


UINT CModelData::ExportPAKM_PrescTemp(CStdioFile* file)
{
	HLoads lo;
	UINT i;
	CString str;

	if(m_LoadArray.GetSize()>0 && m_LoadArray[0].m_NodalTemps.GetSize())
	{
		lo=m_LoadArray[0];

		file->WriteString(CardH11);
		file->WriteString(CardV11);

		for(i=0;i<(UINT)lo.m_NodalTemps.GetSize();i++)
		{
			if(lo.m_NodalTemps[i].m_dNdtemp>0)
			{
				UINT fn=lo.m_NodalTemps[i].m_uNdt_funcID;

				//if(lo.m_NodalTemps[i].m_uNdt_funcID<1) fn=1;
				str.Format("%5u%5u%10.4lf%5u\n",lo.m_NodalTemps[i].m_uNdtempID,fn,
					lo.m_NodalTemps[i].m_dNdtemp,KORC);
				file->WriteString(str);
			}
		}
	}
	return (-1);
}

/*
UINT CModelData::ExportPAKM_Loads(CStdioFile* file)
{
	CString str;
	UINT i,j,uMAXTFT=0;
	HLoads lo;
	StructLoad sl;
	UINT uNQP=0,uNHP=0,uNQE=0;
	UINT SFfuncs[MAX_TF],CONVfuncs[MAX_TF],HSfuncs[MAX_TF];
	bool ima;
	UINT uIFL,uIHP;
	HElement el;

	UINT surface_nodes_3d[6][8]={   {0,1,2,3,8,9,10,11},
									{4,5,6,7,16,17,18,19},
									{0,1,5,4,8,13,16,12},
									{1,2,6,5,9,14,17,13},
									{2,3,7,6,10,15,18,14},
									{3,0,4,7,11,12,19,15}};


	if(m_LoadArray.GetSize()==0) 
	{
//		AfxMessageBox(NO_LOADS);
		return(0);
	}

	lo=m_LoadArray[0];	//Za sada samo prvi set
	
	for(i=0;i<(UINT)m_FunctionsArray.GetSize();i++)
		if((UINT)m_FunctionsArray[i].m_FunctionEntry.GetSize()>uMAXTFT)
			uMAXTFT=m_FunctionsArray[i].m_FunctionEntry.GetSize();

	
//Odredjivanje NQP
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_HEAT_FL || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 ima=false;
	 for(j=0;j<uNQP;j++) if(lo.m_StructLoads[i].m_uSl_funcID==SFfuncs[j]) {ima=true;break;}
	 if(!ima) {SFfuncs[uNQP]=lo.m_StructLoads[i].m_uSl_funcID;uNQP++;}
	}
//Odredjivanje NHP
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_CONV || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 ima=false;
	 for(j=0;j<uNHP;j++) if(lo.m_StructLoads[i].m_uSl_funcID==CONVfuncs[j]) {ima=true;break;}
	 if(!ima) {CONVfuncs[uNHP]=lo.m_StructLoads[i].m_uSl_funcID;uNHP++;}
	}
//Odredjivanje NQE
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_HEAT_GEN || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 ima=false;
	 for(j=0;j<uNQE;j++) if(lo.m_StructLoads[i].m_uSl_funcID==HSfuncs[j]) {ima=true;break;}
	 if(!ima) {HSfuncs[uNQE]=lo.m_StructLoads[i].m_uSl_funcID;uNQE++;}
	}

	file->WriteString(CardH12);
	file->WriteString(CardV12);
	str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u\n",NPOC,uNQP,uMAXTFT,uNHP,uMAXTFT,
		NTOK,uMAXTFT,uNQE,uMAXTFT,NHR,uMAXTFT);
	file->WriteString(str);


//Card /12-1/ DATA ABOUT INTERNAL HEAT GENERATION (SOURCE)	
	if(uNQE)
	for(i=0;i<lo.m_StructLoads.GetSize();i++)
	  if(lo.m_StructLoads[i].m_uLoadtype==FELT_ELEM_HEAT_GEN)
	  {
		file->WriteString(CardH12_1);
		file->WriteString(CardV12_1_a_1);
		file->WriteString(CardV12_1_a_2);
		str.Format("%5u%5u\n",lo.m_StructLoads[i].m_uSl_funcID,m_FunctionsArray[m_nFuncIndex[lo.m_StructLoads[i].m_uSl_funcID]].m_FunctionEntry.GetSize());
		file->WriteString(CardV12_1_b_1);
		file->WriteString(CardV12_1_b_2);
		for(j=0;j<m_FunctionsArray[m_nFuncIndex[lo.m_StructLoads[i].m_uSl_funcID]].m_FunctionEntry.GetSize();j++)
		{
			str.Format("%10.6f%10.6f\n",m_FunctionsArray[m_nFuncIndex[lo.m_StructLoads[i].m_uSl_funcID]].m_FunctionEntry[j].m_dX,
				m_FunctionsArray[m_nFuncIndex[lo.m_StructLoads[i].m_uSl_funcID]].m_FunctionEntry[j].m_dY);
			file->WriteString(str);
		}
	
	}
	
//Card /12-6/ INITIAL TEMPERATURE
	if(NPOC)
	{
	 file->WriteString(CardH12_6);
	 file->WriteString(CardV12_6);
//	 str.Format("%5f\n",lo.m_dDef_temp);
     str.Format ("%10.6f\n",m_MaterialsArray[0].m_dTemperature);
	 file->WriteString(str);
	}


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
	file->WriteString(CardV13_3_e);
	
	return (-1);
}

*/

//Privremeno za vezbe

UINT CModelData::ExportPAKM_Loads(CStdioFile* file)
{
	CString str;
	UINT i,j,k,uMAXTFT=0;
	HLoads lo;
	StructLoad sl;
	UINT uNQP=0,uNHP=0,uNTOK=0,uNQE=0,uNHR=0;
	UINT HFfuncs[MAX_TF],CONVfuncs[MAX_TF],HGfuncs[MAX_TF],RADfuncs[MAX_TF],TOKfuncs[MAX_TF];
	bool ima;
	UINT uIFL,uIHP;
	HElement el;


	UINT surface_nodes_3d[6][8]={   {0,3,2,1,11,10,9,8},
									{4,5,6,7,16,17,18,19},
									{0,1,5,4,8,13,16,12},
									{1,2,6,5,9,14,17,13},
									{2,3,7,6,10,15,18,14},
									{3,0,4,7,11,12,19,15}};

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
		return(0);
	}

	lo=m_LoadArray[0];	//Za sada samo prvi set
	
	for(i=0;i<(UINT)m_FunctionsArray.GetSize();i++)
		if((UINT)m_FunctionsArray[i].m_FunctionEntry.GetSize()>uMAXTFT)
			uMAXTFT=m_FunctionsArray[i].m_FunctionEntry.GetSize();


//Odredjivanje NQE
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_HEAT_GEN || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 ima=false;
	 for(j=0;j<uNQE;j++) if(lo.m_StructLoads[i].m_uSl_funcID==HGfuncs[j]) {ima=true;break;}
	 if(!ima) {HGfuncs[uNQE]=lo.m_StructLoads[i].m_uSl_funcID;uNQE++;}
	}
//Odredjivanje NQP
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_HEAT_FL || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 ima=false;
	 for(j=0;j<uNQP;j++) if(lo.m_StructLoads[i].m_uSl_funcID==HFfuncs[j]) {ima=true;break;}
	 if(!ima) {HFfuncs[uNQP]=lo.m_StructLoads[i].m_uSl_funcID;uNQP++;}
	}
//Odredjivanje NHP
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	int test=0;
	test = lo.m_StructLoads[i].m_uLoadtype;
	test = FELT_ELEM_CONV;
	test = lo.m_StructLoads[i].m_uSl_funcID;



	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_CONV || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 ima=false;
	 for(j=0;j<uNHP;j++) if(lo.m_StructLoads[i].m_uSl_funcID==CONVfuncs[j]) {ima=true;break;}
	 if(!ima) {CONVfuncs[uNHP]=lo.m_StructLoads[i].m_uSl_funcID;uNHP++;}

	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_CONV || !lo.m_StructLoads[i].m_uAddI_fnc[0]) continue;
	 ima=false;
	 for(j=0;j<uNTOK;j++) if(lo.m_StructLoads[i].m_uAddI_fnc[0]==TOKfuncs[j]) {ima=true;break;}
	 if(!ima) {TOKfuncs[uNTOK]=lo.m_StructLoads[i].m_uAddI_fnc[0];uNTOK++;}
	}
//Odredjivanje NHR
	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_RADIATION || !lo.m_StructLoads[i].m_uSl_funcID) continue;
	 ima=false;
	 for(j=0;j<uNHR;j++) if(lo.m_StructLoads[i].m_uSl_funcID==RADfuncs[j]) {ima=true;break;}
	 if(!ima) {RADfuncs[uNHR]=lo.m_StructLoads[i].m_uSl_funcID;uNHR++;}

	 if(lo.m_StructLoads[i].m_uLoadtype!=FELT_ELEM_RADIATION || !lo.m_StructLoads[i].m_uAddI_fnc[0]) continue;
	 ima=false;
	 for(j=0;j<uNTOK;j++) if(lo.m_StructLoads[i].m_uAddI_fnc[0]==TOKfuncs[j]) {ima=true;break;}
	 if(!ima) {TOKfuncs[uNTOK]=lo.m_StructLoads[i].m_uAddI_fnc[0];uNTOK++;}
	}

    file->WriteString(CardH12);
	file->WriteString(CardV12);
	str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u\n",m_PakOptValue.GetDynamicAnIsSet(),uNQP,uMAXTFT,uNHP,uMAXTFT,
		uNTOK,uMAXTFT,uNQE,uMAXTFT,uNHR,uMAXTFT);
	file->WriteString(str);


//Card /12-1/ DATA ABOUT INTERNAL HEAT GENERATION (SOURCE)	
	if(uNQE>0)
	{
	 file->WriteString(CardH12_1);
	 for(i=0;i<uNQE;i++)
	 {
		file->WriteString(CardV12_1_a_1);
		file->WriteString(CardV12_1_a_2);
		str.Format("%5u%5u\n",i+1,m_FunctionsArray[m_nFuncIndex[HGfuncs[i]]].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV12_1_b_1);
		file->WriteString(CardV12_1_b_2);
		for(j=0;j<m_FunctionsArray[m_nFuncIndex[HGfuncs[i]]].m_FunctionEntry.GetSize();j++)
		{
			str.Format("%10.4f%10.4f\n",m_FunctionsArray[m_nFuncIndex[HGfuncs[i]]].m_FunctionEntry[j].m_dX,
				m_FunctionsArray[m_nFuncIndex[HGfuncs[i]]].m_FunctionEntry[j].m_dY);
			file->WriteString(str);
		}
	 }
	}

//Card /12-2/ DATA ABOUT SURFACE FLUXES
	if(uNQP>0)
	{
	 file->WriteString(CardH12_2);
	 for(i=0;i<uNQP;i++)
	 {
		file->WriteString(CardV12_2_a_1);
		file->WriteString(CardV12_2_a_2);
		str.Format("%5u%5u\n",i+1,m_FunctionsArray[m_nFuncIndex[HFfuncs[i]]].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV12_2_b_1);
		file->WriteString(CardV12_2_b_2);
		for(j=0;j<m_FunctionsArray[m_nFuncIndex[HFfuncs[i]]].m_FunctionEntry.GetSize();j++)
		{
			str.Format("%10.4f%10.4f\n",m_FunctionsArray[m_nFuncIndex[HFfuncs[i]]].m_FunctionEntry[j].m_dX,
				m_FunctionsArray[m_nFuncIndex[HFfuncs[i]]].m_FunctionEntry[j].m_dY);
			file->WriteString(str);
		}
	 }
	}

//Card /12-3/ DATA ABOUT ENVIRONMENTAL TEMPERATURE
	if(uNTOK>0)
	{
	 file->WriteString(CardH12_3);
	 for(i=0;i<uNTOK;i++)
	 {
		file->WriteString(CardV12_3_a_1);
		file->WriteString(CardV12_3_a_2);
		str.Format("%5u%5u\n",i+1,m_FunctionsArray[m_nFuncIndex[TOKfuncs[i]]].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV12_3_b_1);
		file->WriteString(CardV12_3_b_2);
		for(j=0;j<m_FunctionsArray[m_nFuncIndex[TOKfuncs[i]]].m_FunctionEntry.GetSize();j++)
		{
			str.Format("%10.4f%10.4f\n",m_FunctionsArray[m_nFuncIndex[TOKfuncs[i]]].m_FunctionEntry[j].m_dX,
				m_FunctionsArray[m_nFuncIndex[TOKfuncs[i]]].m_FunctionEntry[j].m_dY);
			file->WriteString(str);
		}
	 }
	}

//Card /12-4/ DATA ABOUT CONVECTION COEFFICIENT
	if(uNHP>0)
	{
	 file->WriteString(CardH12_4);
	 for(i=0;i<uNHP;i++)
	 {
		file->WriteString(CardV12_4_a_1);
		file->WriteString(CardV12_4_a_2);
		str.Format("%5u%5u\n",i+1,m_FunctionsArray[m_nFuncIndex[CONVfuncs[i]]].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV12_4_b_1);
		file->WriteString(CardV12_4_b_2);
		for(j=0;j<m_FunctionsArray[m_nFuncIndex[CONVfuncs[i]]].m_FunctionEntry.GetSize();j++)
		{
			str.Format("%10.4f%10.4f\n",m_FunctionsArray[m_nFuncIndex[CONVfuncs[i]]].m_FunctionEntry[j].m_dX,
				m_FunctionsArray[m_nFuncIndex[CONVfuncs[i]]].m_FunctionEntry[j].m_dY);
			file->WriteString(str);
		}
	 }
	}

//Card /12-5/ DATA ABOUT EMISSIVITY COEFFICIENT
	if(uNHR>0)
	{
	 file->WriteString(CardH12_5);
	 for(i=0;i<uNHR;i++)
	 {
		file->WriteString(CardV12_5_a_1);
		file->WriteString(CardV12_5_a_2);
		str.Format("%5u%5u\n",i+1,m_FunctionsArray[m_nFuncIndex[RADfuncs[i]]].m_FunctionEntry.GetSize());
		file->WriteString(str);
		file->WriteString(CardV12_5_b_1);
		file->WriteString(CardV12_5_b_2);
		for(j=0;j<m_FunctionsArray[m_nFuncIndex[RADfuncs[i]]].m_FunctionEntry.GetSize();j++)
		{
			str.Format("%10.4f%10.4f\n",m_FunctionsArray[m_nFuncIndex[RADfuncs[i]]].m_FunctionEntry[j].m_dX,
				m_FunctionsArray[m_nFuncIndex[RADfuncs[i]]].m_FunctionEntry[j].m_dY);
			file->WriteString(str);
		}
	 }
	 file->WriteString("\n");
	}


//Card /12-6/ INITIAL TEMPERATURE
	if(m_PakOptValue.GetDynamicAnIsSet())
	{
	 file->WriteString(CardH12_6);
	 file->WriteString(CardV12_6);
	 str.Format("%5f\n",lo.m_dDef_temp);
//     str.Format ("%10.6f\n",m_MaterialsArray[0].m_dTemperature);
	 file->WriteString(str);
	}






//Card /13/
	UINT uNPP2=0,uNPP3=0;

	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
	{
		sl=lo.m_StructLoads[i];
		switch(sl.m_uLoadtype)
		{
		 case FELT_ELEM_HEAT_FL:case FELT_ELEM_CONV:case FELT_ELEM_RADIATION:
			{
			    j=sl.m_uDof_face[0]-1;
				if(fabs(sl.m_dValue[0])>=LOAD_PRESS_TOL)
				{
			     if(j<2) uNPP3++;
				 else
				 {
					el=m_ElArray[m_nElemIndex[sl.m_uLoadID]];
					if(el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20) uNPP3++;
					else uNPP2++;
				 } 
				}
			}
		 break;
		}
	}

	
	//Card 13-2
	//Line pressure
		if(uNPP2>0)
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
						UINT q=0,h=0,Tok=0,e=0;
						switch(sl.m_uLoadtype)
						{
						case FELT_ELEM_HEAT_FL:
							{
								for(k=0;k<uNQP;k++)
									if(HFfuncs[k]==sl.m_uSl_funcID) break;
								q=k+1;
							}
							break;
						case FELT_ELEM_CONV:
							{
								for(k=0;k<uNHP;k++)
									if(CONVfuncs[k]==sl.m_uSl_funcID) break;
								h=k+1;
								for(k=0;k<uNTOK;k++)
									if(TOKfuncs[k]==sl.m_uAddI_fnc[0]) break;
								Tok=k+1;

							}
							break;
						case FELT_ELEM_RADIATION:
							{
								for(k=0;k<uNHR;k++)
									if(RADfuncs[k]==sl.m_uSl_funcID) break;
								e=k+1;
								for(k=0;k<uNTOK;k++)
									if(TOKfuncs[k]==sl.m_uAddI_fnc[0]) break;
								Tok=k+1;
							}
							break;
						}
						str.Format("%5d%5d%5d%5d%5d\n",q,h,Tok,e,0);
						file->WriteString(str);
					}
				   }
				}
			}
		}
		
	//Card 13-3
	//Surface pressure
		if(uNPP3>0)
		{
			file->WriteString(CardH13_3);
			file->WriteString(CardV13_3);
		 	for(i=0;i<(UINT)lo.m_StructLoads.GetSize();i++)
			{
				sl=lo.m_StructLoads[i];
				if(sl.m_uLoadtype==FELT_ELEM_HEAT_FL || sl.m_uLoadtype==FELT_ELEM_CONV || sl.m_uLoadtype==FELT_ELEM_RADIATION)
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
							str.Format("%5d",el.m_uID);
							file->WriteString(str);
							for(k=0;k<(UINT)(el.m_uTopology==FETO_BRICK8 ? 4:8);k++)
							{
								str.Format("%5u",el.m_uNode[surface_nodes_3d[j][k]]);
								file->WriteString(str);
							}


						UINT q=0,h=0,Tok=0,e=0;
						switch(sl.m_uLoadtype)
						{
						case FELT_ELEM_HEAT_FL:
							{
								for(k=0;k<uNQP;k++)
									if(HFfuncs[k]==sl.m_uSl_funcID) break;
								q=k+1;
							}
							break;
						case FELT_ELEM_CONV:
							{
								for(k=0;k<uNHP;k++)
									if(CONVfuncs[k]==sl.m_uSl_funcID) break;
								h=k+1;
								for(k=0;k<uNTOK;k++)
									if(TOKfuncs[k]==sl.m_uAddI_fnc[0]) break;
								Tok=k+1;

							}
							break;
						case FELT_ELEM_RADIATION:
							{
								for(k=0;k<uNHR;k++)
									if(RADfuncs[k]==sl.m_uSl_funcID) break;
								e=k+1;
								for(k=0;k<uNTOK;k++)
									if(TOKfuncs[k]==sl.m_uAddI_fnc[0]) break;
								Tok=k+1;
							}
							break;
						}
						str.Format("%5d%5d%5d%5d%5d\n",q,h,Tok,e,0);
						file->WriteString(str);


						}
							break;
						case FETO_QUAD4: case FETO_QUAD8: case FETO_TRI3: case FETO_TRI6:
							if(j<2)
							{
				//				p_str.Format("%5u%5u%10.4.1e%10.4.1e%10.4.1e%10.4.1e%5u\n",nfun,IPRAV,
				//			 		sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],sl.m_dValue[0],KORC);
								str.Format("%5d",el.m_uID);
								file->WriteString(str);
								for(k=0;k<(UINT)((el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_TRI3)? 4:8);k++)
								{
			 						if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8)
										str.Format("%5u",el.m_uNode[surface_nodes_shell[j][k]]);
									else
										str.Format("%5u",el.m_uNode[surface_nodes_tshell[j][k]]);
									file->WriteString(str);
								}
						
							
						UINT q=0,h=0,Tok=0,e=0;
						switch(sl.m_uLoadtype)
						{
						case FELT_ELEM_HEAT_FL:
							{
								for(k=0;k<uNQP;k++)
									if(HFfuncs[k]==sl.m_uSl_funcID) break;
								q=k+1;
							}
							break;
						case FELT_ELEM_CONV:
							{
								for(k=0;k<uNHP;k++)
									if(CONVfuncs[k]==sl.m_uSl_funcID) break;
								h=k+1;
								for(k=0;k<uNTOK;k++)
									if(TOKfuncs[k]==sl.m_uAddI_fnc[0]) break;
								Tok=k+1;

							}
							break;
						case FELT_ELEM_RADIATION:
							{
								for(k=0;k<uNHR;k++)
									if(RADfuncs[k]==sl.m_uSl_funcID) break;
								e=k+1;
								for(k=0;k<uNTOK;k++)
									if(TOKfuncs[k]==sl.m_uAddI_fnc[0]) break;
								Tok=k+1;
							}
							break;
						}
						str.Format("%5d%5d%5d%5d%5d\n",q,h,Tok,e,0);
						file->WriteString(str);
							
							
							}
							break;
						}
					 }
				}
			}
		}





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
	file->WriteString(CardV13_3_e);	
	return (-1);
}



