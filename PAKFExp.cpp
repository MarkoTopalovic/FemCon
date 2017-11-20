// PAKTExp.cpp: Functions for export to PAK's DAT file.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FemCon.h"
#include "ModelData.h"
#include "PAKFExp.h"
#include "BlockIDs.h"
#include "PakExpOpt.h"
#include "math.h"
extern int m_iExFNSTAC;
extern int m_iExPenalty;
extern int m_iExNjrap;
extern int m_iExRest;
extern int m_iExIndf;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


UINT CModelData::ExportPAK_F(CStdioFile* file,CPakExpOpt *peo)
{

	CString str;
	UINT uNGELEM=1;
	UINT i;
	long lGeneralDataPos;


	if((m_NodArray.GetSize())<=0)
	{
		AfxMessageBox(NO_NODES);
		return(0);	
	};
/*	
	if((uNGELEM=m_PropArray.GetSize())<=0)
	{
		AfxMessageBox(NO_GROUPS);
		return(0);	
	};
*/
//Card /2/
	file->WriteString(CardHF2);
	file->WriteString(CardVF2);
	str.Format("%5u\n",INDFOR);
	file->WriteString(str);
//Card /3/
 	file->WriteString(CardHF3);
	file->WriteString(CardVF3);
	lGeneralDataPos=file->GetPosition();
	str.Format("                                                         \n");
	file->WriteString(str);

//Card /4/
 	file->WriteString(CardHF4);
	file->WriteString(CardVF4);
//	str.Format("%5u%5u%5u%5u%10.6f%10.6f%5u%5u\n",INTEB,INDSC,IFORM,MAXIT,EPSTA,EPSTR,NJRAP,MBAND);
	str.Format("%5u%5u%5u%5u%10.6f%10.6f%5u%5u\n",INTEB,INDSC,IFORM,MAXIT,EPSTA,EPSTR,m_iExNjrap,MBAND);
	file->WriteString(str);
//Card /5/
 	file->WriteString(CardHF5);
	file->WriteString(CardVF5);
//	str.Format("%5u\n",IREST);
	str.Format("%5u\n",m_iExRest);
	file->WriteString(str);
//Card /6/
//	if(NPER>0)
	if((UINT)m_PakOptValue.GetNumOfPeriods()>0)
//	{
 	 file->WriteString(CardHF6);
	 file->WriteString(CardVF6);
/*	 for(i=1;i<=NPER;i++)
	 {
		str.Format("%5u%10.6f",NDT,TIME);
		file->WriteString(str);
	 }
	 file->WriteString("\n");
	} */
	for(i=1;i<=(UINT)m_PakOptValue.GetNumOfPeriods();i++)
	{
		str.Format("%5u%10.6f\n",m_PakOptValue.GetNumSteps(i-1),m_PakOptValue.GetStep(i-1));
		file->WriteString(str);
	}
//card 7/
	ExportPAKF_Nodes(file);
	ExportPAKF_Elements(file,&uNGELEM,peo);
	ExportPAKF_MatModels(file);
	ExportPAKF_PrescVals(file);

//Card /11/
 	file->WriteString(CardHF11);
	file->WriteString(CardVF11);
	str.Format("%10.6f%10.6f%10.6f%10.6f%10.6f\n",UINIT,VINIT,WINIT,PINIT,TINIT);
	file->WriteString(str);

	ExportPAK_TimeFunctions(file);

//Card /14/
	ExportPAKF_Press(file);

//Card /14/
 	file->WriteString(CardHF16);
	file->WriteString(CardVF16);

//Upisivanje naknadno

	file->Seek(lGeneralDataPos,CFile::begin);
//	str.Format("%5u%5u%5u%5u%5u%5u%5u%5u",m_NodArray.GetSize(),m_PropArray.GetSize(),m_MaterialsArray.GetSize(),NSTAC,NPER,NPRINT,INDF,IPENAL);
	str.Format("%5u%5u%5u%5u%5u%5u%5u%5u",m_NodArray.GetSize(),m_PropArray.GetSize(),m_MaterialsArray.GetSize(),m_iExFNSTAC,m_PakOptValue.GetNumOfPeriods(),NPRINT,m_iExIndf,IPENAL);
	file->WriteString(str);
	file->Seek(0,CFile::end);

	return(-1);
}

UINT CModelData::ExportPAKF_Elements(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo)
{
	CString str;
	UINT i;
//	std::size_t i;
	HElement el;
	UINT uNETIP,uNP2DMX;
	UINT node_order[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};

	if(m_ElArray[0].m_uTopology==FETO_QUAD4 || m_ElArray[0].m_uTopology==FETO_QUAD8 ||
		m_ElArray[0].m_uTopology==FETO_TRI3 || m_ElArray[0].m_uTopology==FETO_TRI6) uNETIP=2;
	else uNETIP=3;

 	file->WriteString(CardHF8);
	file->WriteString(CardVF8);
	str.Format("%5u%5u%5u\n",uNETIP,m_ElArray.GetSize(),INDAX);
	file->WriteString(str);
    uNP2DMX=0;
	if(m_ElArray[0].m_uTopology==FETO_QUAD8) uNP2DMX=8;
	if(m_ElArray[0].m_uTopology==FETO_QUAD4) uNP2DMX=4;
	if(m_ElArray[0].m_uTopology==FETO_BRICK8) uNP2DMX=8;
	if(m_ElArray[0].m_uTopology==FETO_BRICK20) uNP2DMX=20;

	// stampanje zaglavlja za elemente
	if(uNETIP==2)
		{
			file->WriteString(CardHF8_1);
			file->WriteString(CardVF8_1_a_1);
			file->WriteString(CardVF8_1_a_2);
			str.Format("%5u%5u%5u%13.5f\n",1,1,uNP2DMX,PENALTY);
			file->WriteString(str);
			file->WriteString(CardVF8_1_b_1);
			file->WriteString(CardVF8_1_b_2);
			file->WriteString(CardVF8_1_c_1);
			file->WriteString(CardVF8_1_c_2);
		}
	else if(uNETIP==3)
		{
			file->WriteString(CardHF8_2);
			file->WriteString(CardVF8_2_a_1);
			file->WriteString(CardVF8_2_a_2);
			str.Format("%5u%5u%5u%13.5f\n",1,1,uNP2DMX,PENALTY);
			file->WriteString(str);
			file->WriteString(CardVF8_2_b_1);
			file->WriteString(CardVF8_2_b_2);
			file->WriteString(CardVF8_2_c_1);
			file->WriteString(CardVF8_2_c_2);
		}
	//stampanje elemenata
	for(i=0;i<m_ElArray.GetSize();i++)
	{
		if(uNETIP==2)
		{
			str.Format("%5u%5u%5u%5u%5u\n",m_ElArray[i].m_uID,
				m_ElArray[i].m_uNode[0],m_ElArray[i].m_uNode[1],m_ElArray[i].m_uNode[2],m_ElArray[i].m_uNode[3]);
			file->WriteString(str);
			if(uNP2DMX==8)
			{
				str.Format("%5u%5u%5u%5u\n",
					m_ElArray[i].m_uNode[4],m_ElArray[i].m_uNode[5],m_ElArray[i].m_uNode[6],m_ElArray[i].m_uNode[7]);
				file->WriteString(str);
			}
		}
		else if(uNETIP==3)
		{
			str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u\n",m_ElArray[i].m_uID,
				m_ElArray[i].m_uNode[node_order[0]],m_ElArray[i].m_uNode[node_order[1]],m_ElArray[i].m_uNode[node_order[2]],
				m_ElArray[i].m_uNode[node_order[3]],m_ElArray[i].m_uNode[node_order[4]],m_ElArray[i].m_uNode[node_order[5]],
				m_ElArray[i].m_uNode[node_order[6]],m_ElArray[i].m_uNode[node_order[7]]);
			file->WriteString(str);
			if(uNP2DMX==20)
			{
				str.Format("%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u%5u\n",
					m_ElArray[i].m_uNode[node_order[8]],m_ElArray[i].m_uNode[node_order[9]],m_ElArray[i].m_uNode[node_order[10]],
					m_ElArray[i].m_uNode[node_order[11]],m_ElArray[i].m_uNode[node_order[12]],m_ElArray[i].m_uNode[node_order[13]],
					m_ElArray[i].m_uNode[node_order[14]],m_ElArray[i].m_uNode[node_order[15]],m_ElArray[i].m_uNode[node_order[16]],
					m_ElArray[i].m_uNode[node_order[17]],m_ElArray[i].m_uNode[node_order[18]],m_ElArray[i].m_uNode[node_order[19]]);
				file->WriteString(str);
			}
		}
	}

	return(-1);

}

UINT CModelData::ExportPAKF_Nodes(CStdioFile* file)
{

	CString str;
	UINT i,ii,ci,cj,cjj,cii,uNETIP,uNP2DMX;
	uNP2DMX = 9;
	int Constr[6];
	HNodes nn;
	HElement el;
	HConstraints constr;
	UINT node_order[20]={6,7,4,5,2,3,0,1,18,19,16,17,10,11,8,9,14,15,12,13};
//Card /7/
 	file->WriteString(CardHF7);
	file->WriteString(CardVF7);
// odrdjivanje tipa elemenata
	if(m_ElArray[0].m_uTopology==FETO_QUAD4 || m_ElArray[0].m_uTopology==FETO_QUAD8 ||
		m_ElArray[0].m_uTopology==FETO_TRI3 || m_ElArray[0].m_uTopology==FETO_TRI6) uNETIP=2;
	else uNETIP=3;
	if(m_ElArray[0].m_uTopology==FETO_QUAD8) uNP2DMX=8;
	if(m_ElArray[0].m_uTopology==FETO_BRICK20) uNP2DMX=20;

	cii=(UINT)m_ConsArray[0].m_ConsNodes.GetSize();
// dodavanje ogranicenja za pritisak na medjucvorove u novi constraint set
	HConstraints constraints;
	ConsNode cn;

	for(i=0;i<m_ElArray.GetSize();i++)
	{
		if(uNETIP==2 && uNP2DMX==8)
		{
			for(ii=4;ii<8;ii++)
			{
				cn.m_uNodeID=m_ElArray[i].m_uNode[ii];
				cn.m_uColor=1;
				cn.m_uLayer=1;
				for(cjj=0;cjj<6;cjj++) cn.m_bDOF[cjj]=0;
				cn.m_bDOF[3]=1;
				constraints.m_ConsNodes.Add(cn);
			}
		}
		else if(uNETIP==3 && uNP2DMX==20)
		{
			for(ii=8;ii<20;ii++)
			{
				cn.m_uNodeID=m_ElArray[i].m_uNode[node_order[ii]];
				cn.m_uColor=1;
				cn.m_uLayer=1;
				for(cjj=0;cjj<6;cjj++) cn.m_bDOF[cjj]=0;
				cn.m_bDOF[3]=1;
				constraints.m_ConsNodes.Add(cn);
			}
		}
	}
	m_ConsArray.Add(constraints);
// stampanje cvorova
	for(i=0;i<(UINT)m_NodArray.GetSize();i++)
	{
		nn=m_NodArray[i];

		for(cj=0;cj<6;cj++) Constr[cj]=0;
		
		if(m_ConsArray.GetSize()>0)
		{
			cjj=(UINT)m_ConsArray.GetSize(); // constraint set u kome su dodata ogranicenja za medjucvorove
				for(ci=0;ci<(UINT)m_ConsArray[0].m_ConsNodes.GetSize();ci++)
				{
					if(m_ConsArray[0].m_ConsNodes[ci].m_uNodeID==nn.m_uID)
					{
						for(cj=0;cj<6;cj++) Constr[cj] = (m_ConsArray[0].m_ConsNodes[ci].m_bDOF[cj] ? 1 : 0);
						break;
					}
				}
				if((uNETIP==2 && uNP2DMX==8)||(uNETIP==3 && uNP2DMX==20))
				{
					for(ci=0;ci<(UINT)m_ConsArray[cjj-1].m_ConsNodes.GetSize();ci++)
					{
						if(m_ConsArray[cjj-1].m_ConsNodes[ci].m_uNodeID==nn.m_uID)
						{
							Constr[3] = (m_ConsArray[cjj-1].m_ConsNodes[ci].m_bDOF[3] ? 1 : 0);
							break;
						}
					}
				}
			 //Check prescribed velocity and pressure
				if(m_LoadArray.GetSize()>0)
					for(ci=0;ci<(UINT)m_LoadArray[0].m_StructLoads.GetSize();ci++)
						if(m_LoadArray[0].m_StructLoads[ci].m_uLoadtype==FELT_VELOCITY
							&& m_LoadArray[0].m_StructLoads[ci].m_uSl_funcID>0 && m_LoadArray[0].m_StructLoads[ci].m_uLoadID==nn.m_uID)
						{
						 if(fabs(m_LoadArray[0].m_StructLoads[ci].m_dValue[0])>VELOCITY_TOL) Constr[0]=1;
						 if(fabs(m_LoadArray[0].m_StructLoads[ci].m_dValue[1])>VELOCITY_TOL) Constr[1]=1;
						 if(fabs(m_LoadArray[0].m_StructLoads[ci].m_dValue[2])>VELOCITY_TOL) Constr[2]=1;
						}
						else if(m_LoadArray[0].m_StructLoads[ci].m_uLoadtype==FELT_NODAL_TOTAL_PR
							&& m_LoadArray[0].m_StructLoads[ci].m_uSl_funcID>0 && m_LoadArray[0].m_StructLoads[ci].m_uLoadID==nn.m_uID) 
						{ 
							Constr[3]=1;
						}

		}
		// ako je 2D element ogranicena je brzina u z pravcu
		if(uNETIP==2)Constr[2]=1;
//		
		str.Format("%5u%5u%5u%5u%5u%5u%10.6f%10.6f%10.6f\n",nn.m_uID,Constr[0],Constr[1],Constr[2],Constr[3],Constr[4],nn.m_dX,nn.m_dY,nn.m_dZ);
		file->WriteString(str);
	}

	return(-1);
}

UINT CModelData::ExportPAKF_PrescVals(CStdioFile* file)
{
	CString str;
	UINT i,uNUMZAD=0,uINDPR,uNUMST,uINDEX;

	uNUMZAD=0;
	uNUMST=0;
//Prebrojavanje zadatih vrednosti	
	if(m_LoadArray.GetSize()>0)
		for(i=0;i<m_LoadArray[0].m_StructLoads.GetSize();i++)
		{
			if(m_LoadArray[0].m_StructLoads[i].m_uLoadtype==FELT_VELOCITY || 
				m_LoadArray[0].m_StructLoads[i].m_uLoadtype==FELT_NODAL_TOTAL_PR) uNUMZAD++;
			if(m_LoadArray[0].m_StructLoads[i].m_uLoadtype==FELT_ELEM_PRESS) uNUMST++;

		}
 	file->WriteString(CardHF10);
	file->WriteString(CardVF10);
	str.Format("%5u%5u\n",uNUMZAD,uNUMST);
	file->WriteString(str);

 	file->WriteString(CardHF10_1);
	file->WriteString(CardVF10_1);

	if(m_LoadArray.GetSize()>0)
		for(i=0;i<m_LoadArray[0].m_StructLoads.GetSize();i++)
			if(m_LoadArray[0].m_StructLoads[i].m_uLoadtype==FELT_VELOCITY|| 
				m_LoadArray[0].m_StructLoads[i].m_uLoadtype==FELT_NODAL_TOTAL_PR)
			{
				if(fabs(m_LoadArray[0].m_StructLoads[i].m_dValue[0])>VELOCITY_TOL) uINDPR=1;
				if(fabs(m_LoadArray[0].m_StructLoads[i].m_dValue[1])>VELOCITY_TOL) uINDPR=2;
				if(fabs(m_LoadArray[0].m_StructLoads[i].m_dValue[2])>VELOCITY_TOL) uINDPR=3;
				if(m_LoadArray[0].m_StructLoads[i].m_uLoadtype==FELT_NODAL_TOTAL_PR) uINDPR=4;
				uINDEX=uINDPR-1;
				if(uINDPR==4) uINDEX=0;
				str.Format("%5u%5u%5u%10.6f\n",m_LoadArray[0].m_StructLoads[i].m_uLoadID,
					uINDPR,m_LoadArray[0].m_StructLoads[i].m_uSl_funcID,m_LoadArray[0].m_StructLoads[i].m_dValue[uINDEX]);
					file->WriteString(str);
			}

	return (-1);
}

UINT CModelData::ExportPAKF_Press(CStdioFile* file)
{
	CString str;
//	CPAKString p_str;
//	UINT i,j,uNUMST;
	UINT i,j;
	HElement el;

	UINT surface_nodes_3d[6][8]={   {0,3,2,1,11,10,9,8},
									{4,5,6,7,16,17,18,19},
									{0,1,5,4,8,13,16,12},
									{1,2,6,5,9,14,17,13},
									{2,3,7,6,10,15,18,14},
									{3,0,4,7,11,12,19,15}};
	
	UINT surface_nodes_2d[4][3]={   {0,1,4},
									{1,2,5},
									{2,3,6},
									{3,0,7}};

	UINT surface_nodes_t2d[3][3]={  {0,1,3},
									{1,2,4},
									{2,0,5}};
//Prebrojavanje zadatih vrednosti	
/*	if(m_LoadArray.GetSize()>0)
		for(i=0;i<m_LoadArray[0].m_StructLoads.GetSize();i++)
			if(m_LoadArray[0].m_StructLoads[i].m_uLoadtype==FELT_ELEM_PRESS) uNUMST++; */
	file->WriteString(CardHF13);
	file->WriteString(CardVF13a);
 	file->WriteString(CardHF13_1a);

	if(m_LoadArray.GetSize()>0)
		for(i=0;i<m_LoadArray[0].m_StructLoads.GetSize();i++)
			if(m_LoadArray[0].m_StructLoads[i].m_uLoadtype==FELT_ELEM_PRESS)
			{
				   el=m_ElArray[m_nElemIndex[m_LoadArray[0].m_StructLoads[i].m_uLoadID]];
				   j=m_LoadArray[0].m_StructLoads[i].m_uDof_face[0]-1;
				   if(el.m_uTopology==FETO_QUAD4 || el.m_uTopology==FETO_QUAD8 ||
					  el.m_uTopology==FETO_TRI3 || el.m_uTopology==FETO_TRI6 )
				   {
						str.Format("%5u%5u%5u%5u%5u",el.m_uID,el.m_uNode[surface_nodes_2d[j-2][0]],
											el.m_uNode[surface_nodes_2d[j-2][1]],1,1);
				   }
			  		else if (el.m_uTopology==FETO_BRICK8 || el.m_uTopology==FETO_BRICK20)
					{
						str.Format("%5u%5u%5u%5u%5u%5u%5u%5u",el.m_uID,el.m_uNode[surface_nodes_3d[j][0]],
								el.m_uNode[surface_nodes_3d[j][1]],el.m_uNode[surface_nodes_3d[j][2]],
								el.m_uNode[surface_nodes_3d[j][3]],1,1,1);
					}
					file->WriteString(str);
					file->WriteString("\n");
			 }
	return (-1);
}
UINT CModelData::ExportPAKF_MatModels(CStdioFile* file)
{

	CString str;
//	UINT CONfuncs[MAX_TF],uNTABK=0,uNTABC=0,uMAXTK=0,uMAXTC;
	UINT i,uNTABK=0,uNTABC=0,uMAXTK=0,uMAXTC;
//	UINT i,uNTACV;

	//Card /9/
	file->WriteString(CardHF9);

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
   /* if (m_PakOptValue.GetDynamicAnIsSet())
	{
		uNTABC=(UINT)m_MaterialsArray.GetSize();
		str.Format("%5u%5u%5u%5u%5u%5u\n",NANLK,uNTABK,uMAXTK,NANLC,uNTABC,uMAXTC);
	}
	else 
	{
		uNTABC=0;
		str.Format("%5u%5u%5u%5u\n",NANLK,uNTABK,uMAXTK,NANLC);
	}
	file->WriteString(str); */

	//Card /9-1/
	file->WriteString(CardHF9_1);
	file->WriteString(CardVF9_1);

// PAKF radi samo sa jednim matrijalom
//	for(i=0;i<(UINT)m_MaterialsArray.GetSize();i++)
	i=0;
//	{
		str.Format("%10.6f%5u\n",
			m_MaterialsArray[i].m_dDensity,INDAMI);
		file->WriteString(str);

		if(INDF!=1) //		if (m_PakOptValue.GetDynamicAnIsSet())
		{
			//Card /9-2/
	  		file->WriteString(CardHF9_2);
	  		file->WriteString(CardVF9_2);
			str.Format("%10.6f\n",m_MaterialsArray[i].m_dK[0]);
			file->WriteString(str);

			//Card /9-3/
	  		file->WriteString(CardHF9_3);
	  		file->WriteString(CardVF9_3);
			str.Format("%10.6f\n",m_MaterialsArray[i].m_dThermal_cap);
			file->WriteString(str);
		}
		//Card /9-4/
	  	file->WriteString(CardHF9_4_1);
	  	file->WriteString(CardHF9_4_2);
	  	file->WriteString(CardVF9_4);
		if (m_LoadArray[0].m_bGrav_on)
			{
				str.Format("%10.6f%10.6f%10.6f%10.6f%10.6f%10.6f\n",m_LoadArray[0].m_dGrav[0],m_LoadArray[0].m_dGrav[1],
					m_LoadArray[0].m_dGrav[0],m_MaterialsArray[i].m_dDamping,0.,0.);
			}
		else 
			{
			str.Format("%10.6f%10.6f%10.6f%10.6f%10.6f%10.6f\n",0.,0.,0.,m_MaterialsArray[i].m_dDamping,0.,0.);
			}
		file->WriteString(str);
//	}

	return(-1);
}