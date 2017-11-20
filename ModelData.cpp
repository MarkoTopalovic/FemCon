// ModelData.cpp: implementation of the CModelData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FemCon.h"
#include "ModelData.h"
#include "BlockIDs.h"
extern CArray <int, int&> m_iExNoOfSteps;
extern CArray <double, double&> m_dExTimeStepIncrement;
extern int m_iArreyLenght;
extern int m_iExNSOPV;
extern double m_dExTOLE;
extern double m_dExTOLS;
extern double m_dExFEMAPversion;

extern double m_dExTAUY;
extern double m_dExCy;
extern bool m_bExRamberg;

extern int m_iExItherMethod;
extern bool m_bExConvForce;
extern bool m_bExConvEnergy;
extern bool m_bExDynamic;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModelData::CModelData()
: m_bArcLenght(false)
, m_bLineSearch(false)
{
	m_bIsThereRod=false;
}

CModelData::~CModelData()
{

}

UINT CModelData::ExportFemap(CStdioFile* file)
{
	ExportFemap_Nodes(file);
	ExportFemap_Properties(file);
	ExportFemap_Elements(file);
	ExportFemap_Constraints(file);
	ExportFemap_Loads(file);
	ExportFemap_Materials(file);

	return(-1);
}

UINT CModelData::ExportFemap_Constraints(CStdioFile* file)
{

	CString str;

	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	str.Format("%d\n",FBID_CONSTRAINTS);
	file->WriteString(str);

	for (int i=0;i<m_ConsArray.GetSize();i++)
	{
		int j;

		HConstraints& con=m_ConsArray[i];

		str.Format("%u,\n",con.m_uSetID);
		file->WriteString(str);
		
		str=con.m_strTitle+"\n";
		file->WriteString(str);
		
		for (j=0;j<con.m_ConsNodes.GetSize();j++)
		{
			const ConsNode* cn=&con.m_ConsNodes[j];
			str.Format("%u,%u,%u,%d,%d,%d,%d,%d,%d,\n",
						cn->m_uNodeID,cn->m_uColor,cn->m_uLayer,
						cn->m_bDOF[0],cn->m_bDOF[1],cn->m_bDOF[2],
						cn->m_bDOF[3],cn->m_bDOF[4],cn->m_bDOF[5]);
			file->WriteString(str);
		}
		
		str="-1,-1,-1,0,0,0,0,0,0,\n";
		file->WriteString(str);
		
		for (j=0;j<con.m_ConsEqs.GetSize();j++)
		{
			const ConsEq* ce=&con.m_ConsEqs[j];
			str.Format("%u,%u,%u,%u,\n",
					ce->m_uEqnID,ce->m_uColor,ce->m_uLayer);
			file->WriteString(str);
			str.Format("%d,\n",ce->m_EqCoefs.GetSize());

			for (int k=0;k<ce->m_EqCoefs.GetSize();k++)
			{
				const EqCoeff* ec=&ce->m_EqCoefs[k];
				str.Format("%u,%u,%lf,\n",
						ec->m_uEqn_nodeID,ec->m_uEqn_dof,
						ec->m_dCoeff);
				file->WriteString(str);
			}
		}
		
		str="-1,-1,-1,\n";
		file->WriteString(str);					
	}

	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	return(-1);
}

UINT CModelData::ExportFemap_Elements(CStdioFile* file)
{
	
	CString str;

	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	str.Format("%d\n",FBID_ELEMENTS);
	file->WriteString(str);
	
	for (int i=0;i<m_ElArray.GetSize();i++)
	{
		HElement& element=m_ElArray[i];

		str.Format("%u,%u,%u,%u,%u,%u,%u,%d,\n",
				element.m_uID,element.m_uColor,element.m_uPropID,element.m_uType,
				element.m_uTopology,element.m_uLayer,element.m_uOrientID,element.m_bMatl_orflag);
		file->WriteString(str);

		str.Format("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n",
				element.m_uNode[0],element.m_uNode[1],element.m_uNode[2],element.m_uNode[3],
				element.m_uNode[4],element.m_uNode[5],element.m_uNode[6],element.m_uNode[7],
				element.m_uNode[8],element.m_uNode[9]);
		file->WriteString(str);

		str.Format("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\n",
			element.m_uNode[10],element.m_uNode[11],element.m_uNode[12],element.m_uNode[13],
			element.m_uNode[14],element.m_uNode[15],element.m_uNode[16],element.m_uNode[17],
			element.m_uNode[18],element.m_uNode[19]);
		file->WriteString(str);
		
		str.Format("%lf,%lf,%lf,\n",element.m_dOrient[0],
				element.m_dOrient[1],element.m_dOrient[2]);
		file->WriteString(str);

		str.Format("%lf,%lf,%lf,\n",element.m_dOffset1[0],
			element.m_dOffset1[1],element.m_dOffset1[2]);
		file->WriteString(str);

		str.Format("%lf,%lf,%lf,\n",element.m_dOffset2[0],
			element.m_dOffset2[1],element.m_dOffset2[2]);
		file->WriteString(str);

		str.Format("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n");
		file->WriteString(str);
		}

	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	return(-1);
}

UINT CModelData::ExportFemap_Loads(CStdioFile* file)
{
	CString str;
	
	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	str.Format("%d\n",FBID_LOADS);
	file->WriteString(str);


	for (int i=0;i<m_LoadArray.GetSize();i++)
	{
		int j;

		HLoads& ld=m_LoadArray[i];

		str.Format("%u,\n",ld.m_uSetID);
		file->WriteString(str);
		
		str=ld.m_strTitle + "\n";
		file->WriteString(str);
		
		str.Format("%u,%lf,%d,%d,%d,\n",
				ld.m_uCSys,ld.m_dDef_temp,
				ld.m_bTemp_on,ld.m_bGrav_on,
				ld.m_bOmega_on);
		file->WriteString(str);
		
		str.Format("%lf,%lf,%lf,%u,%u,%u,\n",
				ld.m_dGrav[0],ld.m_dGrav[1],ld.m_dGrav[2],ld.m_dGrav_fun[0],ld.m_dGrav_fun[1],ld.m_dGrav_fun[2]);
		file->WriteString(str);
		
		str.Format("%lf,%lf,%lf,%u,%u,%u,\n",
				ld.m_dGrav[3],ld.m_dGrav[4],ld.m_dGrav[5],ld.m_dGrav_fun[3],ld.m_dGrav_fun[4],ld.m_dGrav_fun[5]);
		file->WriteString(str);
		
		str.Format("%lf,%lf,%lf,\n",
				ld.m_dOrigin[0],ld.m_dOrigin[1],ld.m_dOrigin[2]);
		file->WriteString(str);
		
		str.Format("%lf,%lf,%lf,\n",
				ld.m_dOmega[0],ld.m_dOmega[1],ld.m_dOmega[2]);
		file->WriteString(str);
		
		str.Format("%lf,%lf,%lf,\n",
				ld.m_dStef_boltz,ld.m_dAbs_temp,ld.m_dFree_cnv_exp);
		file->WriteString(str);
		
		str.Format("%lf,%lf,%lf,%lf,\n",
				ld.m_dFc_flu_cond,ld.m_dFc_flu_cp,
				ld.m_dFc_flu_vis,ld.m_dFc_flu_dens);
		file->WriteString(str);
		
		str.Format("%lf,%lf,%lf,%lf,\n",
				ld.m_dFc_cons_coef,ld.m_dFc_reynolds,
				ld.m_dFc_pran_in,ld.m_dFc_pran_out);
		file->WriteString(str);
		
		str.Format("%u,%u,%u,\n",
				ld.m_uTfc_flu_cond,ld.m_uTfc_flu_cp,ld.m_uTfc_flu_vis);
		file->WriteString(str);
		
		str="0,0,0,\n"; // To je onaj red sto pravi zbrku kod ucitavanja 
		file->WriteString(str);
			// Devet nepoznatih redova
			str="0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0,0,0,0,0,0,0,0,0,\n";
			file->WriteString(str);
			str="0,0,0,0,0,0,0,0,\n";
			file->WriteString(str);
			str="0,0,0,0,0,\n";
			file->WriteString(str);
			str="0,0,0,0,0,0,0,\n";
			file->WriteString(str);
			str="0,0,0,0,0,\n";
			file->WriteString(str);
		
		for (j=0;j<ld.m_StructLoads.GetSize();j++)
		{
			StructLoad& sl=ld.m_StructLoads[j];
		
			str.Format("%u,%u,%u,%u,%u,%u,\n",
					sl.m_uLoadID,sl.m_uLoadtype,
					sl.m_uColor,sl.m_uLayer,
					sl.m_uDefine_sys,sl.m_uSl_funcID);
			file->WriteString(str);
			
			str.Format("%lf,%lf,\n",
					sl.m_dPhase,sl.m_dCoefficient);
			file->WriteString(str);
			
			for (int k=0;k<6;k++)
			{
				str.Format("%u,%lf,\n",
						sl.m_uDof_face[k],sl.m_dValue[k]);
				file->WriteString(str);
			}
			
			str.Format("%lf,%u,%u,%u,\n",
					sl.m_dAddI_coeff,sl.m_uAddI_fnc[0],
					sl.m_uAddI_fnc[1],sl.m_uAddI_fnc[2]);
			file->WriteString(str);
			
			str="0,0,0,0,0,0,0,\n";
			file->WriteString(str);
			
			str.Format("%u,%u,%u\n",
					sl.m_uDir_func[0],sl.m_uDir_func[1],
					sl.m_uDir_func[2]);
			file->WriteString(str);
			
			str.Format("%lf,%lf,%lf,\n",
					sl.m_dDirection[0],sl.m_dDirection[1],
					sl.m_dDirection[2]);
			file->WriteString(str);
		}

		str="-1,-1,-1,-1,-1,\n";
		file->WriteString(str);
		
		for (j=0;j<ld.m_NodalTemps.GetSize();j++)
		{
			NodalTemp& nt=ld.m_NodalTemps[j];
		
			str.Format("%u,%u,%u,%lf,%u,\n",
					nt.m_uNdtempID,nt.m_uColor,
					nt.m_uLayer,nt.m_dNdtemp,
					nt.m_uNdt_funcID);
			file->WriteString(str);
		}
		
		str="-1,-1,-1,0.,0.,\n";

		file->WriteString(str);
		
		for (j=0;j<ld.m_ElTemps.GetSize();j++)
		{
			ElTemp& nt=ld.m_ElTemps[j];
		
			str.Format("%u,%u,%u,%lf,%u,\n",
					nt.m_uEltempID,nt.m_uColor,
					nt.m_uLayer,nt.m_dEltemp,
					nt.m_uElf_funcID);
			file->WriteString(str);
		}

		str="-1,-1,-1,0.,0.,\n";
		file->WriteString(str);
	}

	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	return(-1);
}

UINT CModelData::ExportFemap_Nodes(CStdioFile* file)
{

	CString str;
	
	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	str.Format("%d\n",FBID_NODES);
	file->WriteString(str);
	
	for (int i=0;i<m_NodArray.GetSize();i++)
	{
		HNodes& node=m_NodArray[i];

		str.Format("%u,%u,%u,%u,%u,%d,%d,%d,%d,%d,%d,%lf,%lf,%lf,\n",
					node.m_uID,node.m_uDefine_sys,node.m_uOutput_sys,
					node.m_uLayer,node.m_uColor,node.m_bPermbc[0],node.m_bPermbc[1],
					node.m_bPermbc[2],node.m_bPermbc[3],node.m_bPermbc[4],node.m_bPermbc[5],
					node.m_dX,node.m_dY,node.m_dZ);
		file->WriteString(str);
	}

	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	return(-1);
}

UINT CModelData::ExportFemap_Properties(CStdioFile* file)
{

	CString str;

	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	str.Format("%d\n",FBID_PROPERTIES);
	file->WriteString(str);
	
	for (int i=0;i<m_PropArray.GetSize();i++)
	{
		int j,k;

		HProperties& prop=m_PropArray[i];

		str.Format("%u,%u,%u,%u,%u,%u,\n",
				prop.m_uID,prop.m_uColor,prop.m_uMatIID,
				prop.m_uType,prop.m_uLayer,prop.m_uRefCS);
		file->WriteString(str);
		
		str=prop.m_strTitle+"\n";
		file->WriteString(str);
		
		str.Format("%u,%u,%u,%u\n",
				prop.m_uFlag[0],prop.m_uFlag[1],
				prop.m_uFlag[2],prop.m_uFlag[3]);
		file->WriteString(str);
		
		str.Format("%u\n",prop.m_uNum_lam);
		file->WriteString(str);
		
		for (j=0;j<(int)prop.m_uNum_lam/8;j++)
		{
			str.Format("%u,%u,%u,%u,%u,%u,%u,%u,\n",
				prop.m_uLam_MID[j*8],prop.m_uLam_MID[j*8+1],
				prop.m_uLam_MID[j*8+2],prop.m_uLam_MID[j*8+3],
				prop.m_uLam_MID[j*8+4],prop.m_uLam_MID[j*8+5],
				prop.m_uLam_MID[j*8+6],prop.m_uLam_MID[j*8+7]);
			file->WriteString(str);
		}
		
		for (k=j*8;k<(int)prop.m_uNum_lam;k++)
			fprintf(file->m_pStream,"%u,",prop.m_uLam_MID[k]);
		
		fprintf(file->m_pStream,"\n");
		
		str.Format("%u\n",prop.m_uNum_val);
		file->WriteString(str);
		
		for (j=0;j<(int)prop.m_uNum_val/5;j++)
		{
			str.Format("%lf,%lf,%lf,%lf,%lf,\n",
				prop.m_dValue[j*5],prop.m_dValue[j*5+1],
				prop.m_dValue[j*5+2],prop.m_dValue[j*5+3],
				prop.m_dValue[j*5+4]);
			file->WriteString(str);
		}
		
		for (k=j*5;k<(int)prop.m_uNum_val;k++)
			fprintf(file->m_pStream,"%lf,",prop.m_dValue[k]);
		
		fprintf(file->m_pStream,"\n");

	}

	str.Format("%s\n",FB_SEPARATOR);
	file->WriteString(str);

	return(-1);
}

UINT CModelData::ExportFemap_Materials(CStdioFile* file)
{

	CString str;

	for (int i=0;i<m_MaterialsArray.GetSize();i++)
		{
			int j;

			HMaterial& mat=m_MaterialsArray[i];
			str.Format("%u,%u,%u,%u,%u\n",
				mat.m_uID,mat.m_uColor,mat.m_uType,
				mat.m_uLayer,mat.m_bHas_functions);
			file->WriteString(str);
			str=mat.m_strTitle+"\n";
			file->WriteString(str);
			str.Format("%lf,%lf,%lf\n",
				mat.m_dE[0],mat.m_dE[1],mat.m_dE[2]);
			file->WriteString(str);
			str.Format("%lf,%lf,%lf\n",
				mat.m_dG[0],mat.m_dG[1],mat.m_dG[2]);
			file->WriteString(str);
			str.Format("%lf,%lf,%lf\n",
				mat.m_dNu[0],mat.m_dNu[1],mat.m_dNu[2]);
			file->WriteString(str);
			for (j=0;j<4;j++)
			{
				str.Format("%lf,%lf,%lf,%lf,%lf\n",
					mat.m_dGMatrix_3D[j*5],mat.m_dGMatrix_3D[j*5+1],
					mat.m_dGMatrix_3D[j*5+2],mat.m_dGMatrix_3D[j*5+3],
					mat.m_dGMatrix_3D[j*5+4]);
				file->WriteString(str);
			}
			str.Format("%lf\n",mat.m_dGMatrix_3D[20]);
			file->WriteString(str);
			str.Format("%lf,%lf,%lf,%lf,%lf\n",
				mat.m_dGMatrix_2D[0],mat.m_dGMatrix_2D[1],
				mat.m_dGMatrix_2D[2],mat.m_dGMatrix_2D[3],
				mat.m_dGMatrix_2D[4]);
			file->WriteString(str);
			str.Format("%lf\n",mat.m_dGMatrix_2D[5]);
			file->WriteString(str);
			str.Format("%lf,%lf,%lf,%lf,%lf\n",
				mat.m_dAlpha[0],mat.m_dAlpha[1],
				mat.m_dAlpha[2],mat.m_dAlpha[3],
				mat.m_dAlpha[4]);
			file->WriteString(str);
			str.Format("%lf\n",mat.m_dAlpha[5]);
			file->WriteString(str);
			str.Format("%lf,%lf,%lf,%lf,%lf\n",
				mat.m_dK[0],mat.m_dK[1],
				mat.m_dK[2],mat.m_dK[3],
				mat.m_dK[4]);
			file->WriteString(str);
			str.Format("%lf\n",mat.m_dK[5]);
			file->WriteString(str);
			str.Format("%lf,%lf,%lf,%lf\n",
				mat.m_dThermal_cap,mat.m_dDensity,
				mat.m_dDamping,mat.m_dTemperature);
			file->WriteString(str);
			str.Format("%lf,%lf,%lf,%lf,%lf\n",
				mat.m_dTensionLimit[0],mat.m_dTensionLimit[1],
				mat.m_dCompLimit[0],mat.m_dCompLimit[1],
				mat.m_dShearLimit);
			file->WriteString(str);
			if (mat.m_bHas_functions)
			{
				str.Format("%u,%u,%u\n",
					mat.m_uFE[0],mat.m_uFE[1],mat.m_uFE[2]);
				file->WriteString(str);
				str.Format("%u,%u,%u\n",
					mat.m_uFG[0],mat.m_uFG[1],mat.m_uFG[2]);
				file->WriteString(str);
				str.Format("%u,%u,%u\n",
					mat.m_uNu[0],mat.m_uNu[1],mat.m_uNu[2]);
				file->WriteString(str);
				for (j=0;j<4;i++)
				{
					str.Format("%u,%u,%u,%u,%u\n",
						mat.m_uFGMatrix3D[j*5],mat.m_uFGMatrix3D[j*5+1],
						mat.m_uFGMatrix3D[j*5+2],mat.m_uFGMatrix3D[j*5+3],
						mat.m_uFGMatrix3D[j*5+4]);
					file->WriteString(str);
				}
				str.Format("%u\n",mat.m_uFGMatrix3D[20]);
				file->WriteString(str);
				str.Format("%u,%u,%u,%u,%u\n",
					mat.m_uFGMatrix2D[0],mat.m_uFGMatrix2D[1],
					mat.m_uFGMatrix2D[2],mat.m_uFGMatrix2D[3],
					mat.m_uFGMatrix2D[4]);
				file->WriteString(str);				
				str.Format("%u\n",mat.m_uFGMatrix2D[5]);
				file->WriteString(str);
				sscanf(str,"%u,%u,%u,%u,%u\n",
					mat.m_uFAlpha[0],mat.m_uFAlpha[1],
					mat.m_uFAlpha[2],mat.m_uFAlpha[3],
					mat.m_uFAlpha[4]);
				file->WriteString(str);
				str.Format("%u\n",mat.m_uFAlpha[5]);
				file->WriteString(str);
				str.Format("%u,%u,%u,%u,%u\n",
					mat.m_uFK[0],mat.m_uFK[1],
					mat.m_uFK[2],mat.m_uFK[3],
					mat.m_uFK[4]);
				file->WriteString(str);
				str.Format("%u\n",mat.m_uFK[5]);
				file->WriteString(str);
				str.Format("%u,%u,%u,%u\n",
					mat.m_uFThermal_cap,mat.m_uFDensity,
					mat.m_uFDamping,mat.m_uFTemperature);
				file->WriteString(str);
				str.Format("%u,%u,%u,%u,%u\n",
					mat.m_uFTensionLimit[0],mat.m_uFTensionLimit[1],
					mat.m_uCompLimit[0],mat.m_uCompLimit[1],
					mat.m_uShearLimit);
				file->WriteString(str);
			}
			// Nepoznati redovi:
			str="0.,0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,\n";
			file->WriteString(str);
			str="0,0,0,0,0,\n";
			file->WriteString(str);
			str="0,0,1,0,0,0,\n";
			file->WriteString(str);
			str="0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,0.,0.,0.,\n";
			file->WriteString(str);
			str="0.,0.,\n";
			file->WriteString(str);
			str="0,0,0,0,\n";
			file->WriteString(str);
			str="0,0,0,0,\n";
			file->WriteString(str);
	}

	return(-1);
}

UINT CModelData::ImportFemap(CStdioFile* file, int nDataBlockType)
{
	
	CString str="0";

	switch (nDataBlockType)
	{
	case FBID_NODES:
		{
			ImportFemap_Nodes(file);
			
			FillNodeIndex();
		}
	break;

	case FBID_ELEMENTS:
		{
			ImportFemap_Elements(file);
			
			FillElemIndex();
		}
	break;

	case FBID_PROPERTIES:
		{
			ImportFemap_Properties(file);

			FillPropIndex();
		}
	break;
	case FBID_LOADS_OLD:
		{
			ImportFemap_Loads_OLD(file);
		}
	break;
	case FBID_LOADS:
		{
			ImportFemap_Loads(file);
		}
	break;

	case FBID_CONSTRAINTS_OLD:
		{
			ImportFemap_Constraints_OLD(file);
		}
	break;
	case FBID_CONSTRAINTS:
		{
			ImportFemap_Constraints(file);
		}
	break;

	case FBID_MATERIALS_OLD:
		{
			ImportFemap_Materials_OLD(file);
		}
	break;
	case FBID_MATERIALS:
		{
			ImportFemap_Materials(file);
		}
	break;

	case FBID_CONTACT_SEGMENT:
		{
			ImportFemap_ContactSegment(file);
//			FillPropIndex();
		}
	break;

	case FBID_CONTACT_PROPERTY:
		{
			ImportFemap_ConnectionProperty(file);
			FillCPropIndex();
		}
	break;

	case FBID_CONNECTION:
		{
			ImportFemap_Connection(file);
			FillConnectIndex();
		}
	break;

	case FBID_FUNCTIONS:
		{
			ImportFemap_Functions(file);
			FillFuncIndex();
		}
	break;

	case FBID_SETS:
		{
			ImportFemap_AnalysisSets(file);
		}
	break;

	default:
		while((str.Left(5)!="   -1")||(feof(file->m_pStream)))
		{file->ReadString(str);}
	break;
	}	//switch (nDataBlockType)


	return(-1);
}


UINT CModelData::ImportFemap_Constraints_OLD(CStdioFile* file)
{
	CString str;
	m_ConsArray.RemoveAll();
	file->ReadString(str);
	while (str.Left(5)!="   -1")
	{
		HConstraints constraints;
		constraints.m_uDataBlockID=406;
		constraints.m_uSetID=atoi(str);
		file->ReadString(str);
		constraints.m_strTitle=str;
		file->ReadString(str);
		while (str.Left(3)!="-1,")
		{
			ConsNode cn;
		
			sscanf(str,"%u,%u,%u,%d,%d,%d,%d,%d,%d",
					&cn.m_uNodeID,&cn.m_uColor,&cn.m_uLayer,
					&cn.m_bDOF[0],&cn.m_bDOF[1],&cn.m_bDOF[2],
					&cn.m_bDOF[3],&cn.m_bDOF[4],&cn.m_bDOF[5]);
			
			constraints.m_ConsNodes.Add(cn);
			
			file->ReadString(str);
			
		}//while (str.Left(3)!="-1,")
		
		file->ReadString(str);

		while (str.Left(3)!="-1,")
		{
			ConsEq conseq;
			sscanf(str,"%u,%u,%u,%u",&conseq.m_uEqnID,&conseq.m_uColor,&conseq.m_uLayer);
			file->ReadString(str);
			int nSize=atoi(str);
			conseq.m_EqCoefs.SetSize(nSize);
			for (int i=0;i<nSize;i++)
			{
				EqCoeff& eqcoeff=conseq.m_EqCoefs[i];
				file->ReadString(str);
				sscanf(str,"%u,%u,%lf",&eqcoeff.m_uEqn_nodeID,&eqcoeff.m_uEqn_dof,&eqcoeff.m_dCoeff);
			}
			constraints.m_ConsEqs.Add(conseq);
			file->ReadString(str);
		}//while (str.Left(3)!="-1,")
		m_ConsArray.Add(constraints);

		file->ReadString(str);
	}//	while (str.Left(5)!="   -1")
	return(-1);
}



UINT CModelData::ImportFemap_Constraints(CStdioFile* file)
{

	CString str;
	m_ConsArray.RemoveAll();
	file->ReadString(str);
	
	while (str.Left(5)!="   -1")
	{
		HConstraints constraints;

		constraints.m_uDataBlockID=406;
		constraints.m_uSetID=atoi(str);

		file->ReadString(str);
		
		constraints.m_strTitle=str;
		
		file->ReadString(str);
		
		while (str.Left(3)!="-1,")
		{
			ConsNode cn;
		
			sscanf(str,"%u,%u,%u,%d,%d,%d,%d,%d,%d",
					&cn.m_uNodeID,&cn.m_uColor,&cn.m_uLayer,
					&cn.m_bDOF[0],&cn.m_bDOF[1],&cn.m_bDOF[2],
					&cn.m_bDOF[3],&cn.m_bDOF[4],&cn.m_bDOF[5]);
			
			constraints.m_ConsNodes.Add(cn);
			
			file->ReadString(str);
			
		}//while (str.Left(3)!="-1,")
		
		file->ReadString(str);


		while (str.Left(3)!="-1,")
		{		
			file->ReadString(str);
		}//while (str.Left(3)!="-1,")
		file->ReadString(str);
		while (str.Left(3)!="-1,")
		{		
			file->ReadString(str);
		}//while (str.Left(3)!="-1,")
		file->ReadString(str);
		while (str.Left(3)!="-1,")
		{		
			file->ReadString(str);
		}//while (str.Left(3)!="-1,")
		file->ReadString(str);


		while (str.Left(3)!="-1,")
		{
			ConsEq conseq;
			sscanf(str,"%u,%u,%u",&conseq.m_uEqnID,&conseq.m_uColor,&conseq.m_uLayer);
		
			file->ReadString(str);
			
			int nSize=atoi(str);
			
			conseq.m_EqCoefs.SetSize(nSize);
			
			for (int i=0;i<nSize;i++)
			{
				//EqCoeff e;
			
				//conseq.m_EqCoefs.Add(e);
				
				EqCoeff& eqcoeff=conseq.m_EqCoefs[i];

				file->ReadString(str);
				
				sscanf(str,"%u,%u,%lf",&eqcoeff.m_uEqn_nodeID,&eqcoeff.m_uEqn_dof,&eqcoeff.m_dCoeff);
				
			}
			
			constraints.m_ConsEqs.Add(conseq);
			
			file->ReadString(str);
			
		}//while (str.Left(3)!="-1,")

		m_ConsArray.Add(constraints);
		// at this time str should be -1,-1,-1,0, which means all of the eqations are loaded
		//and what remains are few rows that were added in version 9.3 which are not implemented in PAK
		file->ReadString(str);
		if (m_dExFEMAPversion >=9.3)
		{
			while (str != "-1,0,0,") //-1,0,0, is the final ot these new rows 
				file->ReadString(str);
			file->ReadString(str);
		}
	}//	while (str.Left(5)!="   -1")
	
	return(-1);
}

		

UINT CModelData::ImportFemap_Elements(CStdioFile* file)
{
	m_bIsThereRod=false;
	m_ElArray.RemoveAll();
	CString str;
	file->ReadString(str);
	while (str.Left(5)!="   -1")
	{
		HElement element;
		element.m_uDataBlockID=404;
		sscanf(str,"%u,%u,%u,%u,%u,%u,%u,%d",
				&element.m_uID,&element.m_uColor,&element.m_uPropID,&element.m_uType,
				&element.m_uTopology,&element.m_uLayer,&element.m_uOrientID,&element.m_bMatl_orflag);

		if (element.m_uType==FET_ROD) m_bIsThereRod=true;

		file->ReadString(str);
		
		sscanf(str,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
				&element.m_uNode[0],&element.m_uNode[1],&element.m_uNode[2],&element.m_uNode[3],
				&element.m_uNode[4],&element.m_uNode[5],&element.m_uNode[6],&element.m_uNode[7],
				&element.m_uNode[8],&element.m_uNode[9]);
		
		file->ReadString(str);
		
		sscanf(str,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
				&element.m_uNode[10],&element.m_uNode[11],&element.m_uNode[12],&element.m_uNode[13],
				&element.m_uNode[14],&element.m_uNode[15],&element.m_uNode[16],&element.m_uNode[17],
				&element.m_uNode[18],&element.m_uNode[19]);
		
		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf",&element.m_dOrient[0],
				&element.m_dOrient[1],&element.m_dOrient[2]);
		
		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf",&element.m_dOffset1[0],
				&element.m_dOffset1[1],&element.m_dOffset1[2]);
		
		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf",&element.m_dOffset2[0],
				&element.m_dOffset2[1],&element.m_dOffset2[2]);
		
		file->ReadString(str);

		//Nejasno ssta se ccita (previsse brojki)
		
		file->ReadString(str);
		
		m_ElArray.Add(element);

	}//while (str.Left(5)!="   -1")

	return(-1);
}
UINT CModelData::ImportFemap_Loads_OLD(CStdioFile* file)
{
	CString str;
	m_LoadArray.RemoveAll();
	file->ReadString(str);
	while (str.Left(5)!="   -1")
	{
		HLoads load;
		load.m_uDataBlockID=407;
		load.m_uSetID=atoi(str);
		file->ReadString(str);
		load.m_strTitle=str;
		file->ReadString(str);
		sscanf(str,"%u,%lf,%d,%d,%d",&load.m_uCSys,&load.m_dDef_temp,&load.m_bTemp_on,&load.m_bGrav_on,&load.m_bOmega_on);
		file->ReadString(str);
		sscanf(str,"%lf,%lf,%lf,%u,%u,%u",&load.m_dGrav[0],&load.m_dGrav[1],&load.m_dGrav[2],&load.m_dGrav_fun[0],&load.m_dGrav_fun[1],&load.m_dGrav_fun[2]);
		file->ReadString(str);
		sscanf(str,"%lf,%lf,%lf,%u,%u",&load.m_dGrav[3],&load.m_dGrav[4],&load.m_dGrav[5],&load.m_dGrav_fun[3],&load.m_dGrav_fun[4],&load.m_dGrav_fun[5]);
		file->ReadString(str);
		sscanf(str,"%lf,%lf,%lf",&load.m_dOrigin[0],&load.m_dOrigin[1],&load.m_dOrigin[2]);
		file->ReadString(str);
		sscanf(str,"%lf,%lf,%lf",&load.m_dOmega[0],&load.m_dOmega[1],&load.m_dOmega[2]);
		file->ReadString(str);
		sscanf(str,"%lf,%lf,%lf",&load.m_dStef_boltz,&load.m_dAbs_temp,&load.m_dFree_cnv_exp);
		file->ReadString(str);
		sscanf(str,"%lf,%lf,%lf,%lf",&load.m_dFc_flu_cond,&load.m_dFc_flu_cp,&load.m_dFc_flu_vis,&load.m_dFc_flu_dens);
		file->ReadString(str);
		sscanf(str,"%lf,%lf,%lf,%lf",&load.m_dFc_cons_coef,&load.m_dFc_reynolds,&load.m_dFc_pran_in,&load.m_dFc_pran_out);
		file->ReadString(str);
		sscanf(str,"%u,%u,%u",&load.m_uTfc_flu_cond,&load.m_uTfc_flu_cp,&load.m_uTfc_flu_vis);
		file->ReadString(str);
		int a,ff,cf;
		sscanf(str,"%d,%d,%d",&a,&ff,&cf);
		load.m_bAlt_free_conv=(bool)a;
		load.m_bFc_flu_flag=(bool)ff;
		load.m_bFc_conv_flow=(bool)cf;
//11 to 16
		// Nepoznati redovi
		int i;
		//for (i=0;i<9;i++) file->ReadString(str);
					//TODO
//int m_iExNoOfSteps = 0;
//double m_dExTimeStepIncrement = 0;

		file->ReadString(str);
		file->ReadString(str);

		double t1,t2,t3;
		sscanf(str,"%lf,%lf,%lf",&t1,&t2,&t3);
		
		file->ReadString(str);
		
		double d1,d2,d3,d4,d5,d6;
		sscanf(str,"%lf,%lf,%lf,%lf,%lf,%lf",&d1,&d2,&d3,&d4,&d5,&d6);
		//m_dExTimeStepIncrement = d6; 
		file->ReadString(str); 
		file->ReadString(str);
		
		int v1,v2,v3,v4,v5,v6;
		sscanf(str,"%d,%d,%d,%d,%d,%d",&v1,&v2,&v3,&v4,&v5,&v6);
		//m_iExNoOfSteps = v6;
		file->ReadString(str);
		int m1,m2,m3,m4,m5,m6,m7,m8;
		sscanf(str,"%d,%d,%d,%d,%d,%d,%d,%d",&m1,&m2,&m3,&m4,&m5,&m6,&m7,&m8);
		m_bArcLenght = (bool)m5;
		//edit here
		
		file->ReadString(str);
		file->ReadString(str);
		int cr1,cr2,cr3,cr4,cr5,cr6,cr7;
		sscanf(str,"%d,%d,%d,%d,%d,%d,%d",&cr1,&cr2,&cr3,&cr4,&cr5,&cr6,&cr7);
		m_bExConvForce = (bool)cr3;
		if (m_bExConvForce == true & t2!=0)
		{
			m_dExTOLS = t2;
		}
		//m_bExConvEnergy = (bool)cr4;
		if (m_bExConvEnergy == true & t3!=0)
		{
			m_dExTOLE = t3;
		}
		m_bLineSearch =!(bool)cr5;

		if(m8==2 && m_bArcLenght == false && m_bLineSearch == true)//BFGS
		{
			m_iExItherMethod = 5;
		}
		else if(m8==2 && m_bArcLenght == false && m_bLineSearch == false)//Modified Newton
		{
			m_iExItherMethod = 1;
		}
		else if(m8==2 && m_bArcLenght == true)//Arc Lenght Modified Newton
		{
			m_iExItherMethod = 6;
		}
		else if (m8==1 && m_bArcLenght == false) //Full Newton
		{
			m_iExItherMethod = 3;
		}
		else if (m8==1 && m_bArcLenght == true) //Arc Lenght Full Newton
		{
			m_iExItherMethod = 7;
		}
		file->ReadString(str);
		file->ReadString(str);
		while (str.Left(3)!="-1,")
		{
			StructLoad structload;
			sscanf(str,"%u,%u,%u,%u,%u,%u",&structload.m_uLoadID,&structload.m_uLoadtype,&structload.m_uColor,&structload.m_uLayer,&structload.m_uDefine_sys,&structload.m_uSl_funcID);
			file->ReadString(str);
			sscanf(str,"%lf,%lf",&structload.m_dPhase,&structload.m_dCoefficient);
			for (i=0;i<6;i++)
			{
				file->ReadString(str);	
				sscanf(str,"%u,%lf",&structload.m_uDof_face[i],&structload.m_dValue[i]);
			}
			file->ReadString(str);
			sscanf(str,"%lf,%u,%u,%u", &structload.m_dAddI_coeff,&structload.m_uAddI_fnc[0],&structload.m_uAddI_fnc[1],&structload.m_uAddI_fnc[2]);
			file->ReadString(str); // Nejasno
			file->ReadString(str);
			sscanf(str,"%u,%u,%u",&structload.m_uDir_func[0],&structload.m_uDir_func[1],&structload.m_uDir_func[2]);
			file->ReadString(str);
			sscanf(str,"%lf,%lf,%lf",&structload.m_dDirection[0],&structload.m_dDirection[1],&structload.m_dDirection[2]);
			load.m_StructLoads.Add(structload);
			file->ReadString(str);
		}//while (str.Left(3)!="-1,")
		file->ReadString(str);
		while (str.Left(3)!="-1,")
		{
			NodalTemp nodaltemp;
			double tmp;
			sscanf(str,"%u,%u,%u,%lf,%lf,%u",&nodaltemp.m_uNdtempID,&nodaltemp.m_uColor,&nodaltemp.m_uLayer,&nodaltemp.m_dNdtemp,&tmp,&nodaltemp.m_uNdt_funcID);
			load.m_NodalTemps.Add(nodaltemp);
			file->ReadString(str);	
		}//while (str.Left(3)!="-1,")
		file->ReadString(str);
		while (str.Left(3)!="-1,")
		{
			ElTemp eltemp;
			sscanf(str,"%u,%u,%u,%lf,%u",&eltemp.m_uEltempID,&eltemp.m_uColor,&eltemp.m_uLayer,&eltemp.m_dEltemp,&eltemp.m_uElf_funcID);
			load.m_ElTemps.Add(eltemp);
			file->ReadString(str);
		}//while (str.Left(3)!="-1,")
		m_LoadArray.Add(load);					
		file->ReadString(str);
	}//	while (str.Left(5)!="   -1")

	return(-1);
}


UINT CModelData::ImportFemap_Loads(CStdioFile* file)
{

	CString str;
	m_LoadArray.RemoveAll();
	file->ReadString(str); // str=1

	while (str.Left(5)!="   -1")
	{
	
		HLoads load;
	
		load.m_uDataBlockID=407;
		load.m_uSetID=atoi(str);
		
		file->ReadString(str); //Opterecenje
		
		load.m_strTitle=str;
		
		file->ReadString(str);
		
		sscanf(str,"%u,%lf,%d,%d,%d",&load.m_uCSys,&load.m_dDef_temp,
				&load.m_bTemp_on,&load.m_bGrav_on,&load.m_bOmega_on);

		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf,%u,%u,%u",&load.m_dGrav[0],&load.m_dGrav[1],&load.m_dGrav[2],&load.m_dGrav_fun[0],&load.m_dGrav_fun[1],&load.m_dGrav_fun[2]);

		file->ReadString(str);

		sscanf(str,"%lf,%lf,%lf,%u,%u,%u",&load.m_dGrav[3],&load.m_dGrav[4],&load.m_dGrav[5],&load.m_dGrav_fun[3],&load.m_dGrav_fun[4],&load.m_dGrav_fun[5]);

		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf",&load.m_dOrigin[0],&load.m_dOrigin[1],&load.m_dOrigin[2]);

		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf",&load.m_dOmega[0],&load.m_dOmega[1],&load.m_dOmega[2]);

		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf",&load.m_dStef_boltz,&load.m_dAbs_temp,&load.m_dFree_cnv_exp);

		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf,%lf",&load.m_dFc_flu_cond,&load.m_dFc_flu_cp,
				&load.m_dFc_flu_vis,&load.m_dFc_flu_dens);

		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf,%lf",&load.m_dFc_cons_coef,&load.m_dFc_reynolds,
				&load.m_dFc_pran_in,&load.m_dFc_pran_out);

		file->ReadString(str);
		
		sscanf(str,"%u,%u,%u",&load.m_uTfc_flu_cond,&load.m_uTfc_flu_cp,&load.m_uTfc_flu_vis);

		file->ReadString(str);
		
		int a,ff,cf;
		
		sscanf(str,"%d,%d,%d",&a,&ff,&cf);

		load.m_bAlt_free_conv=(bool)a;
		load.m_bFc_flu_flag=(bool)ff;
		load.m_bFc_conv_flow=(bool)cf;
//11 to 16
		// Nepoznati redovi
		int i;
		//for (i=0;i<9;i++) file->ReadString(str);
					//TODO
//int m_iExNoOfSteps = 0;
//double m_dExTimeStepIncrement = 0;

		file->ReadString(str);
		file->ReadString(str);

		double t1,t2,t3;
		sscanf(str,"%lf,%lf,%lf",&t1,&t2,&t3);
		
		file->ReadString(str);
		
		double d1,d2,d3,d4,d5,d6;
		sscanf(str,"%lf,%lf,%lf,%lf,%lf,%lf",&d1,&d2,&d3,&d4,&d5,&d6);
	//	m_dExTimeStepIncrement = d6; //time step increment 6th value
		file->ReadString(str); 
		file->ReadString(str);
		
		int v1,v2,v3,v4,v5,v6;
		sscanf(str,"%d,%d,%d,%d,%d,%d",&v1,&v2,&v3,&v4,&v5,&v6); //number of steps 6th value
	//	m_iExNoOfSteps = v6;
		file->ReadString(str);
		int m1,m2,m3,m4,m5,m6,m7,m8;
		sscanf(str,"%d,%d,%d,%d,%d,%d,%d,%d",&m1,&m2,&m3,&m4,&m5,&m6,&m7,&m8);
		m_bArcLenght = (bool)m5;
		//edit here
		
		file->ReadString(str);
		file->ReadString(str);
		int cr1,cr2,cr3,cr4,cr5,cr6,cr7;
		sscanf(str,"%d,%d,%d,%d,%d,%d,%d",&cr1,&cr2,&cr3,&cr4,&cr5,&cr6,&cr7);
		m_bExConvForce = (bool)cr3;
		if (m_bExConvForce == true & t2!=0)
		{
			m_dExTOLS = t2;
		}
		//m_bExConvEnergy = (bool)cr4;
		if (m_bExConvEnergy == true & t3!=0)
		{
			m_dExTOLE = t3;
		}
		m_bLineSearch =!(bool)cr5;

		if(m8==2 && m_bArcLenght == false && m_bLineSearch == true)//BFGS
		{
			m_iExItherMethod = 5;

		}
		else if(m8==2 && m_bArcLenght == false && m_bLineSearch == false)//Modified Newton
		{
			m_iExItherMethod = 1;

		}
		else if(m8==2 && m_bArcLenght == true)//Arc Lenght Modified Newton
		{
			m_iExItherMethod = 6;

		}
		else if (m8==1 && m_bArcLenght == false) //Full Newton
		{
			m_iExItherMethod = 3;
		}
		else if (m8==1 && m_bArcLenght == true) //Arc Lenght Full Newton
		{
			m_iExItherMethod = 7;
		}
		file->ReadString(str);
		



		file->ReadString(str);
		//so far so good
		while (str.Left(3)!="-1,")
		{

			StructLoad structload;
			int newLoadType = 0;
			sscanf(str,"%u,%u,%u,%u,%u",&structload.m_uLoadID,&newLoadType, //,%u
					&structload.m_uColor,&structload.m_uLayer,&structload.m_uDefine_sys);//,&structload.m_uSl_funcID);
//sscanf(str,"%u,%u,%u,%u,%u,%u",&structload.m_uLoadID,&structload.m_uLoadtype,&structload.m_uColor,&structload.m_uLayer,&structload.m_uDefine_sys,&structload.m_uSl_funcID);
			//conversion table from version 4.4 to 10 or higher
			/*
										4.4			 10
			nodal force					1			1
			nodal displacement			2			3
			nodal acceleration			3			7
			nodal heat generation		5			11
			nodal heat flux				6			10
			velocity					7			5
			nonlinear transient			8			12
			distributed line load		10			41
			element face pressure		11			42
			element heat generation		13			47
			element heat flux			14			44
			element convection			15			45
			element radiation			16			46
			*/
			switch(newLoadType)
			{
				case 1:
				{
					structload.m_uLoadtype = 1;
				}
				break;
				case 3:
				{
					structload.m_uLoadtype = 2;
				}
				break;
				case 7:
				{
					structload.m_uLoadtype = 3;
				}
				break;
				case 11:
				{
					structload.m_uLoadtype = 5;
				}
				break;
				case 10:
				{
					structload.m_uLoadtype = 6;
				}
				break;
				case 5:
				{
					structload.m_uLoadtype = 7;
				}
				break;
				case 12:
				{
					structload.m_uLoadtype = 8;
				}
				break;
				case 14:
				{
					structload.m_uLoadtype = 14;
				}
				break;
				case 41:
				{
					structload.m_uLoadtype = 10;
				}
				break;
				case 42:
				{
					structload.m_uLoadtype = 11;
					
				}
				break;
				case 47:
				{
					structload.m_uLoadtype = 13;
				}
				break;
				case 44:
				{
					structload.m_uLoadtype = 14;
				}
				break;
				case 45:
				{
					structload.m_uLoadtype = 15;
				}
				break;
				case 46:
				{
					structload.m_uLoadtype = 16;
				}
				break;
			}


			file->ReadString(str);
			
			//sscanf(str,"%lf,%lf",&structload.m_dPhase,&structload.m_dCoefficient); this was in 4.4
			structload.m_dPhase = 0.0;
			structload.m_dCoefficient = 0.0;

				
			sscanf(str,"%u,%u,%u",&structload.m_uDof_face[0],&structload.m_uDof_face[1],&structload.m_uDof_face[2]);
			structload.m_uDof_face[3] = 0;
			structload.m_uDof_face[4] = 0;
			structload.m_uDof_face[5] = 0;
					
			file->ReadString(str);
			sscanf(str,"%lf,%lf,%lf",&structload.m_dValue[0],&structload.m_dValue[1],&structload.m_dValue[2]);
			structload.m_dValue[3] = 0.0;
			structload.m_dValue[4] = 0.0;
			structload.m_dValue[5] = 0.0;

			file->ReadString(str);
// proveriti sa Snezom u vezi pakt
			sscanf(str,"%u,%u,%u,%u",&structload.m_uSl_funcID,&structload.m_uAddI_fnc[1],&structload.m_uAddI_fnc[0],&structload.m_uAddI_fnc[2]);
			
//problem
			file->ReadString(str); // shade
			
			

			file->ReadString(str);
			sscanf(str,"%u,%u,%u",&structload.m_uDir_func[0],&structload.m_uDir_func[1],&structload.m_uDir_func[2]);	
			file->ReadString(str);
			sscanf(str,"%lf,%lf,%lf",&structload.m_dDirection[0],&structload.m_dDirection[1],&structload.m_dDirection[2]);
					
			load.m_StructLoads.Add(structload);

			file->ReadString(str);

		}//while (str.Left(3)!="-1,")


		file->ReadString(str);
		
		while (str.Left(3)!="-1,")
		{
			file->ReadString(str);	

		}//while (str.Left(3)!="-1,")

		file->ReadString(str);
		
		while (str.Left(3)!="-1,")
		{
			NodalTemp nodaltemp;
			double tmp;

			sscanf(str,"%u,%u,%u,%lf,%lf,%u",&nodaltemp.m_uNdtempID,&nodaltemp.m_uColor,
					&nodaltemp.m_uLayer,&nodaltemp.m_dNdtemp,&tmp,&nodaltemp.m_uNdt_funcID);
					
			load.m_NodalTemps.Add(nodaltemp);
			
			file->ReadString(str);	

		}//while (str.Left(3)!="-1,")


		file->ReadString(str);
		
		while (str.Left(3)!="-1,")
		{
			ElTemp eltemp;

			sscanf(str,"%u,%u,%u,%lf,%u",&eltemp.m_uEltempID,&eltemp.m_uColor,
					&eltemp.m_uLayer,&eltemp.m_dEltemp,&eltemp.m_uElf_funcID);

			load.m_ElTemps.Add(eltemp);

			file->ReadString(str);

		}//while (str.Left(3)!="-1,")
			
		m_LoadArray.Add(load);					
		
		file->ReadString(str);
		if (m_dExFEMAPversion >=9.3)
		{
			while (str.Left(3)!="-1,")
			{
				file->ReadString(str);	

			}//while (str.Left(3)!="-1,")
		
			while (str != "-1,0,0,") //-1,0,0, is the final ot these new rows 
				file->ReadString(str);
			file->ReadString(str);
		}
		
	}//	while (str.Left(5)!="   -1")

	return(-1);
}

UINT CModelData::ImportFemap_Nodes(CStdioFile* file)
{

	m_NodArray.RemoveAll();
	CString str;
	
	file->ReadString(str);
	
	while (str.Left(5)!="   -1")
	{
		HNodes node;
		
		node.m_uDataBlockID=FBID_NODES;
		
		sscanf(str,"%u,%u,%u,%u,%u,%d,%d,%d,%d,%d,%d,%lf,%lf,%lf",
				&node.m_uID,&node.m_uDefine_sys,&node.m_uOutput_sys,
				&node.m_uLayer,&node.m_uColor,&node.m_bPermbc[0],&node.m_bPermbc[1],
				&node.m_bPermbc[2],&node.m_bPermbc[3],&node.m_bPermbc[4],&node.m_bPermbc[5],
				&node.m_dX,&node.m_dY,&node.m_dZ);
		
		m_NodArray.Add(node);
		
		file->ReadString(str);

	}//while (str.Left(5)!="   -1")
	
	return(-1);
}

UINT CModelData::ImportFemap_Properties(CStdioFile* file)
{
	//todo
	CString str;
	m_PropArray.RemoveAll();
	file->ReadString(str);

	while (str.Left(5)!="   -1")
	{
		HProperties prop;

		prop.m_uDataBlockID=402;
		
		sscanf(str,"%u,%u,%u,%u,%u,%u",&prop.m_uID,&prop.m_uColor,&prop.m_uMatIID,&prop.m_uType,&prop.m_uLayer,&prop.m_uRefCS);
				
		file->ReadString(str);
		
		prop.m_strTitle=str;
		
		file->ReadString(str);
		
		sscanf(str,"%u,%u,%u,%u",&prop.m_uFlag[0],&prop.m_uFlag[1],&prop.m_uFlag[2],&prop.m_uFlag[3]); //so far so good
				
		file->ReadString(str);
		
		prop.m_uNum_lam=(UINT)atoi(str);
		prop.m_uLam_MID.SetSize(prop.m_uNum_lam);

		int i;
		for (i=0;i<(int)prop.m_uNum_lam/8;i++)
		{
			file->ReadString(str);
		
			sscanf(str,"%u,%u,%u,%u,%u,%u,%u,%u",
					&prop.m_uLam_MID[i*8],&prop.m_uLam_MID[i*8+1],
					&prop.m_uLam_MID[i*8+2],&prop.m_uLam_MID[i*8+3],
					&prop.m_uLam_MID[i*8+4],&prop.m_uLam_MID[i*8+5],
					&prop.m_uLam_MID[i*8+6],&prop.m_uLam_MID[i*8]+7);
		}
		
		int j;
		for (j=i*8;j<(int)prop.m_uNum_lam;j++)
			fscanf(file->m_pStream,"%u,",&prop.m_uLam_MID[j]);
		
		file->ReadString(str);
		file->ReadString(str);
					
		prop.m_uNum_val=(UINT)atoi(str);
		prop.m_dValue.SetSize(prop.m_uNum_val);

		for (i=0;i<(int)prop.m_uNum_val/5;i++)
		{
			file->ReadString(str);
			
			sscanf(str,"%lf,%lf,%lf,%lf,%lf",
					&prop.m_dValue[i*5],&prop.m_dValue[i*5+1],
					&prop.m_dValue[i*5+2],&prop.m_dValue[i*5+3],
					&prop.m_dValue[i*5+4]);	
		}

		for (j=i*5;j<(int)prop.m_uNum_val;j++)
			fscanf(file->m_pStream,"%lf,",&prop.m_dValue[j]);
		
		file->ReadString(str); 
		//added 2 for loops here
		
		if (str=="")
			file->ReadString(str);


		if (m_dExFEMAPversion >=6.0)
		{
			int m_dFirstLoop;
			m_dFirstLoop=(UINT)atoi(str);
			int k;
			for (k=0;k<m_dFirstLoop;k++)
			file->ReadString(str);
		
			if (m_dExFEMAPversion >=8.1)
			{
				file->ReadString(str); 
				int m_dSecondLoop;
				m_dSecondLoop=(UINT)atoi(str);
				int l;
				for (l=0;l<m_dSecondLoop;l++)
				file->ReadString(str);
			}

			file->ReadString(str); //str = -1 end of the big loop
		}

		m_PropArray.Add(prop);
		
		
	}//while (str.Left(5)!="   -1")

	return(-1);
}

UINT CModelData::ImportFemap_Materials_OLD(CStdioFile* file)
{
	CString str;
	m_MaterialsArray.RemoveAll();
	file->ReadString(str); //1

	while (str.Left(5)!="   -1")
	{
		HMaterial mat;
		mat.m_uDataBlockID=401;
		sscanf(str,"%u,%u,%u,%u,%u",
			&mat.m_uID,&mat.m_uColor,&mat.m_uType,
			&mat.m_uLayer,&mat.m_bHas_functions);
		file->ReadString(str); //2
		mat.m_strTitle=str;
		file->ReadString(str); //3
		sscanf(str,"%lf,%lf,%lf",
			&mat.m_dE[0],&mat.m_dE[1],&mat.m_dE[2]);
		file->ReadString(str); //4
		sscanf(str,"%lf,%lf,%lf",
			&mat.m_dG[0],&mat.m_dG[1],&mat.m_dG[2]);
		file->ReadString(str); //5
		sscanf(str,"%lf,%lf,%lf",
			&mat.m_dNu[0],&mat.m_dNu[1],&mat.m_dNu[2]);
		int i;
		for (i=0;i<4;i++)
		{
			file->ReadString(str); //6,7,8,9
			sscanf(str,"%lf,%lf,%lf,%lf,%lf",
				&mat.m_dGMatrix_3D[i*5],&mat.m_dGMatrix_3D[i*5+1],
				&mat.m_dGMatrix_3D[i*5+2],&mat.m_dGMatrix_3D[i*5+3],
				&mat.m_dGMatrix_3D[i*5+4]);
		}
		file->ReadString(str); //10
		mat.m_dGMatrix_3D[20]=atof(str);
		file->ReadString(str); //11
		sscanf(str,"%lf,%lf,%lf,%lf,%lf",
			&mat.m_dGMatrix_2D[0],&mat.m_dGMatrix_2D[1],
			&mat.m_dGMatrix_2D[2],&mat.m_dGMatrix_2D[3],
			&mat.m_dGMatrix_2D[4]);
		file->ReadString(str); //12
		mat.m_dGMatrix_2D[5]=atof(str);
		file->ReadString(str); //13
		sscanf(str,"%lf,%lf,%lf,%lf,%lf",
			&mat.m_dAlpha[0],&mat.m_dAlpha[1],
			&mat.m_dAlpha[2],&mat.m_dAlpha[3],
			&mat.m_dAlpha[4]);
		file->ReadString(str); //14
		mat.m_dAlpha[5]=atof(str);
		file->ReadString(str); //15
		sscanf(str,"%lf,%lf,%lf,%lf,%lf",
			&mat.m_dK[0],&mat.m_dK[1],
			&mat.m_dK[2],&mat.m_dK[3],
			&mat.m_dK[4]);
		file->ReadString(str); //16
		mat.m_dK[5]=atof(str);
		file->ReadString(str); //17
		sscanf(str,"%lf,%lf,%lf,%lf",
			&mat.m_dThermal_cap,&mat.m_dDensity,
			&mat.m_dDamping,&mat.m_dTemperature);
		file->ReadString(str); //18
		sscanf(str,"%lf,%lf,%lf,%lf,%lf",
			&mat.m_dTensionLimit[0],&mat.m_dTensionLimit[1],
			&mat.m_dCompLimit[0],&mat.m_dCompLimit[1],
			&mat.m_dShearLimit);
		
		if (mat.m_bHas_functions)
		{
			file->ReadString(str);
			sscanf(str,"%u,%u,%u",
				&mat.m_uFE[0],&mat.m_uFE[1],&mat.m_uFE[2]);
			file->ReadString(str);
			sscanf(str,"%u,%u,%u",
				&mat.m_uFG[0],&mat.m_uFG[1],&mat.m_uFG[2]);
			file->ReadString(str);
			sscanf(str,"%u,%u,%u",
				&mat.m_uNu[0],&mat.m_uNu[1],&mat.m_uNu[2]);
			for (i=0;i<4;i++)
			{
				file->ReadString(str);
				sscanf(str,"%u,%u,%u,%u,%u",
					&mat.m_uFGMatrix3D[i*5],&mat.m_uFGMatrix3D[i*5+1],
					&mat.m_uFGMatrix3D[i*5+2],&mat.m_uFGMatrix3D[i*5+3],
					&mat.m_uFGMatrix3D[i*5+4]);
			}
			file->ReadString(str);
			mat.m_uFGMatrix3D[20]=atoi(str);
			file->ReadString(str);
			sscanf(str,"%u,%u,%u,%u,%u",
				&mat.m_uFGMatrix2D[0],&mat.m_uFGMatrix2D[1],
				&mat.m_uFGMatrix2D[2],&mat.m_uFGMatrix2D[3],
				&mat.m_uFGMatrix2D[4]);
			file->ReadString(str);
			mat.m_uFGMatrix2D[5]=atoi(str);
			file->ReadString(str);
			sscanf(str,"%u,%u,%u,%u,%u",
				&mat.m_uFAlpha[0],&mat.m_uFAlpha[1],
				&mat.m_uFAlpha[2],&mat.m_uFAlpha[3],
				&mat.m_uFAlpha[4]);
			file->ReadString(str);
			mat.m_uFAlpha[5]=atoi(str);
			file->ReadString(str);
			sscanf(str,"%u,%u,%u,%u,%u",
				&mat.m_uFK[0],&mat.m_uFK[1],
				&mat.m_uFK[2],&mat.m_uFK[3],
				&mat.m_uFK[4]);
			file->ReadString(str);
			mat.m_uFK[5]=atoi(str);
			file->ReadString(str);
			sscanf(str,"%u,%u,%u,%u",
				&mat.m_uFThermal_cap,&mat.m_uFDensity,
				&mat.m_uFDamping,&mat.m_uFTemperature);
			file->ReadString(str);
			sscanf(str,"%u,%u,%u,%u,%u",
				&mat.m_uFTensionLimit[0],&mat.m_uFTensionLimit[1],
				&mat.m_uCompLimit[0],&mat.m_uCompLimit[1],
				&mat.m_uShearLimit);
		}

	for (i=0;i<7;i++)
		file->ReadString(str); //25
		sscanf(str,"%lf,%lf,%lf",
				&mat.m_dPlastHardSlope,&mat.m_dPlastYieldLim[0],&mat.m_dPlastYieldLim[1]);

		if (m_dExFEMAPversion>=4.5)
		{
			file->ReadString(str); //26
		}
	for (i=0;i<2;i++)
		file->ReadString(str);
		sscanf(str,"%lf,%lf,%d,%u,%d,%d",
				&mat.m_dHypPolyord[0], &mat.m_dHypPolyord[1], &mat.m_iNonlin_type, &mat.m_iNonlin_func,
				&mat.m_iHard_type, &mat.m_iYield_type);

	for (i=0;i<5;i++)
		file->ReadString(str);

	m_MaterialsArray.Add(mat);
	file->ReadString(str);
	}

	return(-1);
}

UINT CModelData::ImportFemap_Materials(CStdioFile* file)
{
	CString str;
	m_MaterialsArray.RemoveAll();
	file->ReadString(str);
	while (str.Left(5)!="   -1")
	{
		HMaterial mat;
		mat.m_uDataBlockID=601;	
		UINT constFormat,subType;
//		UINT constFormat;
		constFormat = 0;
		subType = 0;
		sscanf(str,"%u,%u,%u,%u,%u,%u,%u",&mat.m_uID,&constFormat,&mat.m_uColor,&mat.m_uType,&subType,&mat.m_uLayer,&mat.m_bHas_functions);
		file->ReadString(str);
		mat.m_strTitle=str;
		mat.m_uSubType=subType;
		file->ReadString(str); //new to version 10. this ia always 10 number of boolean flags
		file->ReadString(str); //new to version 10. boolean flags

		int bval[10]; 
		//these should be cenverted into bool (if they are needed which i doubt)
		sscanf(str,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&bval[0],&bval[1],&bval[2],&bval[3],&bval[4],&bval[5],&bval[6],&bval[7],&bval[8],&bval[9]);
		
		file->ReadString(str); //new to version 10. number of integer values always 25

		int ival[25];
		int ivalnum = 0;
		file->ReadString(str); //new to version 10. integer values 1-10
		sscanf(str,"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",&ival[0],&ival[1],&ival[2],&ival[3],&ival[4],&ival[5],&ival[6],&ival[7],&ival[8],&ival[9]);
		file->ReadString(str); //new to version 10. integer values 11-20
		sscanf(str,"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",&ival[10],&ival[11],&ival[12],&ival[13],&ival[14],&ival[15],&ival[16],&ival[17],&ival[18],&ival[19]);
		file->ReadString(str); //new to version 10. integer values 21-25
		sscanf(str,"%u,%u,%u,%u,%u,",&ival[20],&ival[21],&ival[22],&ival[23],&ival[24]);
		
		mat.m_dHypPolyord[0] = ival[0];
		mat.m_dHypPolyord[1] = ival[1];
		mat.m_iNonlin_type = ival[2];
		mat.m_iHard_type = ival[3];
		mat.m_iYield_type = ival[4];

		file->ReadString(str); //new to version 10. number of real values always 200
		double mval[200];
		
		int realRowCounter = 0;
		for(realRowCounter; realRowCounter<20; realRowCounter++)
		{
			file->ReadString(str);
			sscanf(str,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",&mval[realRowCounter*10],&mval[realRowCounter*10+1],&mval[realRowCounter*10+2],&mval[realRowCounter*10+3],&mval[realRowCounter*10+4],&mval[realRowCounter*10+5],&mval[realRowCounter*10+6],&mval[realRowCounter*10+7],&mval[realRowCounter*10+8],&mval[realRowCounter*10+9]);
		}

		//Young's modulus
		mat.m_dE[0] = mval[0];
		mat.m_dE[1] = mval[1];
		mat.m_dE[2] = mval[2];
		//Shear modulus
		mat.m_dG[0] = mval[3];
		mat.m_dG[1] = mval[4];
		mat.m_dG[2] = mval[5];
		//Poisson.s ratio
		mat.m_dNu[0] = mval[6];
		mat.m_dNu[1] = mval[7];
		mat.m_dNu[2] = mval[8];
		//GMatrix_3D
		int imat = 0;
		for(imat;imat<=20;imat++)
		{
			mat.m_dGMatrix_3D[imat] = mval[9+imat];
		}
		mat.m_dGMatrix_2D[0] = mval[30];
		mat.m_dGMatrix_2D[1] = mval[31];
		mat.m_dGMatrix_2D[2] = mval[32];
		mat.m_dGMatrix_2D[3] = mval[33];
		mat.m_dGMatrix_2D[4] = mval[34];
		mat.m_dGMatrix_2D[5] = mval[35];
		//Thermal expansion coefficients
		mat.m_dAlpha[0] = mval[36];
		mat.m_dAlpha[1] = mval[37];
		mat.m_dAlpha[2] = mval[38];
		mat.m_dAlpha[3] = mval[39];
		mat.m_dAlpha[4] = mval[40];
		mat.m_dAlpha[5] = mval[41];
		//Thermal conductivity coefficients
		mat.m_dK[0] = mval[42];
		mat.m_dK[1] = mval[43];
		mat.m_dK[2] = mval[44];
		mat.m_dK[3] = mval[45];
		mat.m_dK[4] = mval[46];
		mat.m_dK[5] = mval[47];
		mat.m_dThermal_cap = mval[48];
		mat.m_dDensity = mval[49];
		mat.m_dDamping = mval[50];
		mat.m_dTemperature = mval[51];
		mat.m_dTensionLimit[0] = mval[52];
		mat.m_dTensionLimit[1] = mval[53];
		mat.m_dCompLimit[0] = mval[54];
		mat.m_dCompLimit[1] = mval[55];
		mat.m_dShearLimit = mval[56];
		mat.m_dPlastHardSlope  = mval[84]; //Cy
		mat.m_dPlastYieldLim[0]  = mval[85]; //TauY
		mat.m_dPlastYieldLim[1] = mval[86];
		
		m_dExTAUY = mval[85];
		m_dExCy = mval[84];


		file->ReadString(str); // number of function IDs always 50

		int fval[50];
		int functionRowCounter = 0;
		for(functionRowCounter; functionRowCounter<5; functionRowCounter++)
		{
			file->ReadString(str);
			sscanf(str,"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",&fval[functionRowCounter*10],&fval[functionRowCounter*10+1],&fval[functionRowCounter*10+2],&fval[functionRowCounter*10+3],&fval[functionRowCounter*10+4],&fval[functionRowCounter*10+5],&fval[functionRowCounter*10+6],&fval[functionRowCounter*10+7],&fval[functionRowCounter*10+8],&fval[functionRowCounter*10+9]);
		}
		mat.m_iNonlin_func = fval[5];

		file->ReadString(str); // number of  Addl function IDs always 70

		int rc = atoi(str)/10;
		int g;
		for(g=0; g<rc; g++)
		{
			file->ReadString(str);
			
		}

		file->ReadString(str);  //should be -1

	m_MaterialsArray.Add(mat);
	//file->ReadString(str);
	}

	return(-1);
}

// ucitavanje kontaktnih karakteristika
UINT CModelData::ImportFemap_ConnectionProperty(CStdioFile* file)
{
	CString str;
	m_CPropArray.RemoveAll();
	file->ReadString(str);

	while (str.Left(5)!="   -1")
	{
		HCProperties prop;

		prop.m_uDataBlockID=918;
		
		sscanf(str,"%u,%u,%u,%u,%u,%u",&prop.m_uID,&prop.m_uColor,&prop.m_Zero1,&prop.m_uType,&prop.m_uLayer,&prop.m_Zero2);
				
		file->ReadString(str);
		
		prop.m_strTitle=str;
		
		file->ReadString(str);
		
		sscanf(str,"%u,%u,%u,%u",&prop.m_uFlag[0],&prop.m_uFlag[1],&prop.m_uFlag[2],&prop.m_uFlag[3]); //so far so good
				
		file->ReadString(str);
		
		prop.m_uMaxExtr_Mat=(UINT)atoi(str);
		prop.m_uExtra_Matl.SetSize(prop.m_uMaxExtr_Mat);

		int i;
		for (i=0;i<(int)prop.m_uMaxExtr_Mat/8;i++)
		{
			file->ReadString(str);
		
			sscanf(str,"%u,%u,%u,%u,%u,%u,%u,%u",
					&prop.m_uExtra_Matl[i*8],&prop.m_uExtra_Matl[i*8+1],
					&prop.m_uExtra_Matl[i*8+2],&prop.m_uExtra_Matl[i*8+3],
					&prop.m_uExtra_Matl[i*8+4],&prop.m_uExtra_Matl[i*8+5],
					&prop.m_uExtra_Matl[i*8+6],&prop.m_uExtra_Matl[i*8]+7);
		}
		
		int j;
		for (j=i*8;j<(int)prop.m_uMaxExtr_Mat;j++)
			fscanf(file->m_pStream,"%u,",&prop.m_uExtra_Matl[j]);
		
		file->ReadString(str);
		file->ReadString(str);
					
		prop.m_uMax_val=(UINT)atoi(str);
		prop.m_dValue.SetSize(prop.m_uMax_val);

		for (i=0;i<(int)prop.m_uMax_val/5;i++)
		{
			file->ReadString(str);
			
			sscanf(str,"%lf,%lf,%lf,%lf,%lf",
					&prop.m_dValue[i*5],&prop.m_dValue[i*5+1],
					&prop.m_dValue[i*5+2],&prop.m_dValue[i*5+3],
					&prop.m_dValue[i*5+4]);	
		}

		for (j=i*5;j<(int)prop.m_uMax_val;j++)
			fscanf(file->m_pStream,"%lf,",&prop.m_dValue[j]);
		
		file->ReadString(str); 
		//added 2 for loops here
		
		if (str=="")
			file->ReadString(str);


		if (m_dExFEMAPversion >=6.0)
		{
			int m_dFirstLoop;
			m_dFirstLoop=(UINT)atoi(str);
			int k;
			for (k=0;k<m_dFirstLoop;k++)
			file->ReadString(str);
		
			if (m_dExFEMAPversion >=8.1)
			{
				file->ReadString(str); 
				int m_dSecondLoop;
				m_dSecondLoop=(UINT)atoi(str);
				int l;
				for (l=0;l<m_dSecondLoop;l++)
				file->ReadString(str);
			}

			file->ReadString(str); //str = -1 end of the big loop
		}

		m_CPropArray.Add(prop);
		
		
	}//while (str.Left(5)!="   -1")

	return(-1);
}

// ucitavanje kontaktnih segmenata
UINT CModelData::ImportFemap_ContactSegment(CStdioFile* file)
{
	//todo
	CString str;
	m_ConnSegArray.RemoveAll();
	file->ReadString(str);

	while (str.Left(5)!="   -1")
	{
		HContactSegment prop;

		prop.m_uDataBlockID=615;
		
		sscanf(str,"%u,%u,%u,%u,%u,%u,%u,%u",&prop.m_uID,&prop.m_uColor,&prop.m_uLayer,&prop.m_uOutput,&prop.m_uRefnode,&prop.m_uRigid,&prop.m_uRef_sys,&prop.m_uOffset);
				
// ucitavanje 3 linije  Seg Type
		file->ReadString(str);
		file->ReadString(str);
		file->ReadString(str);
// ucitavanje naziva
		file->ReadString(str);
		
		prop.m_strTitle=str;
// ucitavanje linije 3
		file->ReadString(str);
//ucitavanje entiteta 
		file->ReadString(str);
		while (str.Left(3)!="-1,")
		{
			Entity eltemp;
			sscanf(str,"%u,%u,%u,%u",&eltemp.m_uEntityID,&eltemp.m_uFaceID,&eltemp.m_uType,&eltemp.m_uExpanded);
			prop.m_Entity.Add(eltemp);
			file->ReadString(str);
		}//while (str.Left(3)!="-1,")
		m_ConnSegArray.Add(prop);
		file->ReadString(str);
	}//while (str.Left(5)!="   -1")

	return(-1);
}

//ucitavanje kontaktnih parova CONNECTION
UINT CModelData::ImportFemap_Connection(CStdioFile* file)
{
	CString str;
	m_ConnArray.RemoveAll();
	file->ReadString(str);

	while (str.Left(5)!="   -1")
	{
		HConnection prop;

		prop.m_uDataBlockID=919;
		
		sscanf(str,"%u,%u,%u,%u,%u,%u",&prop.m_uID,&prop.m_uColor,&prop.m_Pid,&prop.m_uType,&prop.m_uLayer,&prop.m_Disabled);
				
		file->ReadString(str);
		
		prop.m_strTitle=str;
		
		file->ReadString(str);
		
		sscanf(str,"%u,%u",&prop.m_uregionID[0],&prop.m_uregionID[1]);
		
		file->ReadString(str); //str = -1 end of the big loop

		m_ConnArray.Add(prop);
		
	}//while (str.Left(5)!="   -1")

	return(-1);
}

//edit here
UINT CModelData::ImportFemap_AnalysisSets(CStdioFile* file)
{
	//todo
	CString str;

	m_iExNoOfSteps.RemoveAll();
	m_dExTimeStepIncrement.RemoveAll();
	m_iArreyLenght=0;


	file->ReadString(str);
	while (str.Left(5)!="   -1")
	{	
		
		int dyn[5];
		sscanf(str,"%u,%u,%u,%u,%u",&dyn[0],&dyn[1],&dyn[2],&dyn[3],&dyn[4]);
		
		if (dyn[3]==2)
		{
			m_bExDynamic = true;
		}		 
		file->ReadString(str); //title
		int i;
		for (i=0; i<=13; i++)
		{
			file->ReadString(str);
		}
		file->ReadString(str);
		file->ReadString(str);
		for (i=0; i<=20; i++)
		{
			file->ReadString(str);
		}
		file->ReadString(str); //0
		file->ReadString(str); //0
		for (i=0; i<=28; i++)
		{
			file->ReadString(str);
		}
		file->ReadString(str); //NSOPV
		int nsopv[6];
		sscanf(str,"%u,%u,%u,%u,%u,%u",&nsopv[0],&nsopv[1],&nsopv[2],&nsopv[3],&nsopv[4],&nsopv[5]);
		m_iExNSOPV = nsopv[1];
		for (i=0; i<=36; i++)
		{
			file->ReadString(str);
		}
		file->ReadString(str); //number of time cases
		int timeNum = atoi(str);
		file->ReadString(str); // time steps time increments

//work here
		int numOfTimeSteps[10];
		double timeIncrement[10];
		int skipFactor[10];
		sscanf(str,"%d,%lf,%d,%d,%lf,%d,%d,%lf,%d,%d,%lf,%d,%d,%lf,%d,%d,%lf,%d,%d,%lf,%d,%d,%lf,%d,%d,%lf,%d,%d,%lf,%d",
				&numOfTimeSteps[0],&timeIncrement[0],&skipFactor[0],
				&numOfTimeSteps[1],&timeIncrement[1],&skipFactor[1],
				&numOfTimeSteps[2],&timeIncrement[2],&skipFactor[2],
				&numOfTimeSteps[3],&timeIncrement[3],&skipFactor[3],
				&numOfTimeSteps[4],&timeIncrement[4],&skipFactor[4],
				&numOfTimeSteps[5],&timeIncrement[5],&skipFactor[5],
				&numOfTimeSteps[6],&timeIncrement[6],&skipFactor[6],
				&numOfTimeSteps[7],&timeIncrement[7],&skipFactor[7],
				&numOfTimeSteps[8],&timeIncrement[8],&skipFactor[8],
				&numOfTimeSteps[9],&timeIncrement[9],&skipFactor[9]);
		
		int t;
		for (t=0; t<10; t++)
		{
			if ((numOfTimeSteps[t]>0) && (timeIncrement[t]>0.0) && dyn[3]==22)
			{
				m_iExNoOfSteps.Add(numOfTimeSteps[t]);
				m_dExTimeStepIncrement.Add(timeIncrement[t]);
				m_iArreyLenght++;
			}
		}
		
		for (i=0; i<=18; i++)
		{
			file->ReadString(str);
		}
		file->ReadString(str); //-1

	}//while (str.Left(5)!="   -1")

	return(-1);
}

UINT CModelData::ImportFemap_Functions(CStdioFile* file)
{
	CString str;
	m_FunctionsArray.RemoveAll();
	file->ReadString(str);

	while (str.Left(5)!="   -1")
	{
		HFunctions fn;

		fn.m_uDataBlockID=420;
		
		sscanf(str,"%u,%u",
					&fn.m_uID,&fn.m_uFunc_type);
	
		file->ReadString(str);
		fn.m_strTitle = str;
		
		file->ReadString(str);
		while (str.Left(3)!="-1,")
		{
			FunctionEntry fe;
			sscanf(str,"%u,%lf,%lf",
						&fe.m_uEntryID,&fe.m_dX,&fe.m_dY);
			
			fn.m_FunctionEntry.Add(fe);
			file->ReadString(str);
		}
		
		file->ReadString(str);
		
		m_FunctionsArray.Add(fn);
	}

	return(-1);
}

UINT CModelData::ExportFemap_Functions(CStdioFile* file)
{
	return(-1);
}

UINT CModelData::FillNodeIndex()
{
	m_nNodeIndex.RemoveAll();
	m_nNodeIndex.InitHashTable(NODE_ARRAY_GROW_BY,TRUE);

	for (int i=0;i<m_NodArray.GetSize();i++)
	{
		m_nNodeIndex.SetAt(m_NodArray[i].m_uID,i);
	}

	return(-1);
}

UINT CModelData::FillElemIndex()
{
	m_nElemIndex.RemoveAll();
	m_nElemIndex.InitHashTable(ELEMENT_ARRAY_GROW_BY,TRUE);

	for (int i=0;i<m_ElArray.GetSize();i++)
	{
		m_nElemIndex.SetAt(m_ElArray[i].m_uID,i);
	}

	return(-1);
}

UINT CModelData::FillPropIndex()
{
	m_nPropIndex.RemoveAll();

	for (int i=0;i<m_PropArray.GetSize();i++)
	{
		m_nPropIndex.SetAt(m_PropArray[i].m_uID,i);
	}

	return(-1);
}
UINT CModelData::FillCPropIndex()
{
	m_nCPropIndex.RemoveAll();

	for (int i=0;i<m_CPropArray.GetSize();i++)
	{
		m_nCPropIndex.SetAt(m_CPropArray[i].m_uID,i);
	}

	return(-1);
}
UINT CModelData::FillConnectIndex()
{
	m_nConectIndex.RemoveAll();

	for (int i=0;i<m_ConnArray.GetSize();i++)
	{
		m_nConectIndex.SetAt(m_ConnArray[i].m_uID,i);
	}

	return(-1);
}

UINT CModelData::FillFuncIndex()
{
	m_nFuncIndex.RemoveAll();

	for (int i=0;i<m_FunctionsArray.GetSize();i++)
	{
		m_nFuncIndex.SetAt(m_FunctionsArray[i].m_uID,i);
	}

	return(-1);
}


UINT CModelData::DeleteContents()
{

	m_NodArray.RemoveAll();
	m_ElArray.RemoveAll();
	m_PropArray.RemoveAll();
	m_ConsArray.RemoveAll();
	m_LoadArray.RemoveAll();
	m_nNodeIndex.RemoveAll();
	m_nElemIndex.RemoveAll();

	return(-1);
}

UINT CModelData::OnNewDocument()
{

	m_NodArray.SetSize(NODE_ARRAY_NEW_SIZE, NODE_ARRAY_GROW_BY);
	m_ElArray.SetSize(ELEMENT_ARRAY_NEW_SIZE, ELEMENT_ARRAY_GROW_BY);
	m_PropArray.SetSize(PROPERTIES_ARRAY_NEW_SIZE, PROPERTIES_ARRAY_GROW_BY);
	m_ConsArray.SetSize(LOADS_ARRAY_NEW_SIZE, LOADS_ARRAY_GROW_BY);
	m_LoadArray.SetSize(CONSTRAINTS_ARRAY_NEW_SIZE, CONSTRAINTS_ARRAY_GROW_BY);
	m_MaterialsArray.SetSize(MATERIAL_ARRAY_NEW_SIZE, CONSTRAINTS_ARRAY_GROW_BY);

	return(-1);
}

int CModelData::GetModelTypeForElement(UINT ElementID)
{
	HElement *elem;
	HProperties *prop;
	UINT type, subType;
	UINT uMODEL1;
	int i;

	elem=&m_ElArray[m_nElemIndex[ElementID]];
	prop=&m_PropArray[m_nPropIndex[elem->m_uPropID]];
	
	for(i=0;i<m_MaterialsArray.GetSize();i++)
	{	
		if (m_MaterialsArray[i].m_uID==prop->m_uMatIID)
		{
			type=m_MaterialsArray[i].m_uType;
			subType=m_MaterialsArray[i].m_uSubType;
			break;
		}
	}
// sneza promenila zbog citanja General tipa materijala (Cam-clay...)
		switch (type)
		{
		case FEMT_ISO: uMODEL1=PAKM_ELASTIC_ISO;
			break;
		case FEMT_2D_ORTHO: case FEMT_3D_ORTHO: uMODEL1=PAKM_ELASTIC_ORTHO;
			break;
		case 6:
			switch (subType)
			{
				case PAKM_CAM_CLAY:	uMODEL1=9;
					break;
				case PAKM_MOHR_COULOMB:	uMODEL1=42;
			}
		}
		if (type != 6) 
		{
			if (m_MaterialsArray[i].m_iNonlin_type==3) uMODEL1=PAKM_MISES_PLASTIC_ROD;

			if (m_MaterialsArray[i].m_dAlpha[0]!=0) uMODEL1=3;

			if ((uMODEL1==5)&&(!m_bIsThereRod)) uMODEL1=6;

			if ( m_bExRamberg == true) uMODEL1=6;
		}
/*			switch (type)
			{
			case FEMT_ISO: 
				uMODEL1=PAKM_ELASTIC_ISO;
				break;
			case FEMT_2D_ORTHO: 
			case FEMT_3D_ORTHO: 
				uMODEL1=PAKM_ELASTIC_ORTHO;
				break;
			}
			
//			if (m_PakOptValue.GetAnalysisType()!=0) uMODEL1=3;
			
			if (m_MaterialsArray[i].m_iNonlin_type==3) uMODEL1=PAKM_MISES_PLASTIC_ROD;

			if (m_MaterialsArray[i].m_dAlpha[0]!=0) uMODEL1=3;

//			if ((uMODEL1==5)&&(!m_bIsThereRod)) uMODEL1=6; */
			
	return(uMODEL1);
}