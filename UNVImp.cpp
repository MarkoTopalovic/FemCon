// UNVImp.cpp: Functions for import PAK's UNV file.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FemCon.h"
#include "ModelData.h"
#include "BlockIDs.h"

UINT CModelData::ImportUNV(CStdioFile* file)
{
	
	CString str;

	while (!feof(file->m_pStream))
	{
		file->ReadString(str);

		if (str.Left(5)!="   -1")
			break;

		file->ReadString(str);
		int nDataBlockType=atoi (str);

		switch (nDataBlockType)
		{
		case UBID_NODES:
			{
				ImportUNV_Nodes(file);
				
				FillNodeIndex();
			}
		break;

		case UBID_ELEMENTS:
			{
				ImportUNV_Elements(file);
				
				FillElemIndex();
			}
		break;

		case UBID_LOADS:
			{
				ImportUNV_Loads(file);
			}
		break;

		case UBID_CONSTRAINTS:
			{
				ImportUNV_Constraints(file);
			}

		default:
			while(str.Left(5)!="   -1")
			{file->ReadString(str);}
		break;
		}	//switch (nDataBlockType)

	}

	return(-1);
}

UINT CModelData::ImportUNV_Constraints(CStdioFile* file)
{

	CString str;

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
			sscanf(str,"%u,%u,%u,%u",
					&conseq.m_uEqnID,&conseq.m_uColor,&conseq.m_uLayer);
		
			file->ReadString(str);
			
			int nSize=atoi(str);
			
			conseq.m_EqCoefs.SetSize(nSize);
			
			for (int i=0;i<nSize;i++)
			{
				//EqCoeff e;
			
				//conseq.m_EqCoefs.Add(e);
				
				EqCoeff& eqcoeff=conseq.m_EqCoefs[i];

				file->ReadString(str);
				
				sscanf(str,"%u,%u,%lf",
						&eqcoeff.m_uEqn_nodeID,&eqcoeff.m_uEqn_dof,
						&eqcoeff.m_dCoeff);
				
			}
			
			constraints.m_ConsEqs.Add(conseq);
			
			file->ReadString(str);
			
		}//while (str.Left(3)!="-1,")

		m_ConsArray.Add(constraints);

		file->ReadString(str);
		
	}//	while (str.Left(5)!="   -1")
	
	return(-1);
}

UINT CModelData::ImportUNV_Elements(CStdioFile* file)
{

	CString str;

	file->ReadString(str);

	while (str.Left(5)!="   -1")
	{

		HElement element;

		element.m_uDataBlockID=UBID_ELEMENTS;
		
		sscanf(str,"%u,%u,%u,%u,%u,%u,%u",
				&element.m_uID,
				&element.m_uPropID, //Privremeno
				&element.m_uType,&element.m_uTopology,
				&element.m_uLayer,&element.m_uOrientID,&element.m_bMatl_orflag); //Privremeno

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
		
		m_ElArray.Add(element);

	}//while (str.Left(5)!="   -1")

	return(-1);
}

UINT CModelData::ImportUNV_Loads(CStdioFile* file)
{

	CString str;

	file->ReadString(str);

	while (str.Left(5)!="   -1")
	{
	
		HLoads load;
	
		load.m_uDataBlockID=407;
		load.m_uSetID=atoi(str);
		
		file->ReadString(str);
		
		load.m_strTitle=str;
		
		file->ReadString(str);
		
		sscanf(str,"%u,%lf,%d,%d,%d",&load.m_uCSys,&load.m_dDef_temp,
				&load.m_bTemp_on,&load.m_bGrav_on,&load.m_bOmega_on);

		file->ReadString(str);
		
		sscanf(str,"%lf,%lf,%lf",&load.m_dGrav[0],&load.m_dGrav[1],&load.m_dGrav[2]);

		file->ReadString(str);

		sscanf(str,"%lf,%lf,%lf",&load.m_dGrav[3],&load.m_dGrav[4],&load.m_dGrav[5]);

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

		// Nepoznati redovi
		int i;
		for (i=0;i<9;i++) file->ReadString(str);
					
		file->ReadString(str);

		while (str.Left(3)!="-1,")
		{

			StructLoad structload;
			
			sscanf(str,"%u,%u,%u,%u,%u,%u",&structload.m_uLoadID,&structload.m_uLoadtype,
					&structload.m_uColor,&structload.m_uLayer,&structload.m_uDefine_sys,&structload.m_uSl_funcID);

			file->ReadString(str);
			
			sscanf(str,"%lf,%lf",&structload.m_dPhase,&structload.m_dCoefficient);

			for (i=0;i<6;i++)
			{
				file->ReadString(str);
				
				sscanf(str,"%u,%lf",&structload.m_uDof_face[i],&structload.m_dValue[i]);
			}
			
			file->ReadString(str);
			
			sscanf(str,"%lf,%u,%u,%u", &structload.m_dAddI_coeff,&structload.m_uAddI_fnc[0],
					&structload.m_uAddI_fnc[1],&structload.m_uAddI_fnc[2]);

			file->ReadString(str); // Nejasno
			
			file->ReadString(str);
			
			sscanf(str,"%u,%u,%u",&structload.m_uDir_func[0],&structload.m_uDir_func[1],
					&structload.m_uDir_func[2]);
					
			file->ReadString(str);
			
			sscanf(str,"%lf,%lf,%lf",&structload.m_dDirection[0],&structload.m_dDirection[1],
					&structload.m_dDirection[2]);
					
			load.m_StructLoads.Add(structload);

			file->ReadString(str);

		}//while (str.Left(3)!="-1,")

		file->ReadString(str);
		
		while (str.Left(3)!="-1,")
		{
			NodalTemp nodaltemp;

			sscanf(str,"%u,%u,%u,%lf,%u",&nodaltemp.m_uNdtempID,&nodaltemp.m_uColor,
					&nodaltemp.m_uLayer,&nodaltemp.m_dNdtemp,&nodaltemp.m_uNdt_funcID);
					
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

	}//	while (str.Left(5)!="   -1")

	return(-1);
}

UINT CModelData::ImportUNV_Nodes(CStdioFile* file)
{

	CString str;
	
	file->ReadString(str);
	
	while (str.Left(5)!="   -1")
	{
		HNodes node;
		
		node.m_uDataBlockID=UBID_NODES;
		
		sscanf(str,"%u,%d,%d,%d,%lf,%lf,%lf",
				&node.m_uID,
				&node.m_uDefine_sys,&node.m_uOutput_sys, //Privremeno
				&node.m_uColor,
				&node.m_dX,&node.m_dY,&node.m_dZ);
		
		m_NodArray.Add(node);
		
		file->ReadString(str);

	}//while (str.Left(5)!="   -1")
	
	return(-1);
}

