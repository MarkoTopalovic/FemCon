// ModelData.h: interface for the CModelData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELDATA_H__F0632FFA_A3E0_4978_8FEC_1F152CF300E8__INCLUDED_)
#define AFX_MODELDATA_H__F0632FFA_A3E0_4978_8FEC_1F152CF300E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HNodes.h"
#include "HElement.h"
#include "HProperties.h"
#include "HConstraints.h"
#include "HLoads.h"
#include "HMaterial.h"
#include "HFunctions.h"
#include "HCProperties.h"
#include "HConnection.h"
#include "HContactSegment.h"

#include "FileIO.h"
#include "PAKExp.h"
#include "PakExpOpt.h"
#include "PakOptValues.h"

class CModelData: public CObject
{
public:
	CModelData();
	virtual ~CModelData();

	CArray <HNodes,HNodes&> m_NodArray;
	CArray <HElement,HElement&> m_ElArray;
	CArray <HProperties,HProperties&> m_PropArray;
	CArray <HConstraints,HConstraints&> m_ConsArray;
	CArray <HLoads,HLoads&> m_LoadArray;
	CArray <HMaterial,HMaterial&> m_MaterialsArray;
	CArray <HFunctions,HFunctions&> m_FunctionsArray;
	CArray <HCProperties,HCProperties&> m_CPropArray;
	CArray <HConnection,HConnection&> m_ConnArray;
	CArray <HContactSegment,HContactSegment&> m_ConnSegArray;


	CMap<UINT,UINT,UINT,UINT> m_nNodeIndex; // relation between Node ID and it's position in NodArray
	CMap<UINT,UINT,UINT,UINT> m_nElemIndex; // relation between Element ID and it's position in ElArray
	CMap<UINT,UINT,UINT,UINT> m_nPropIndex; // relation between Property ID and it's position in PropArray
	CMap<UINT,UINT,UINT,UINT> m_nCPropIndex; // relation between CProperty ID and it's position in CPropArray
	CMap<UINT,UINT,UINT,UINT> m_nConectIndex; // relation between Connection ID and it's position in ConnArray
	CMap<UINT,UINT,UINT,UINT> m_nFuncIndex; // relation between Function ID and it's position in FuncArray

	UINT ImportFemap(CStdioFile* file, int nDataBlockType);
	UINT ImportFemap_Nodes(CStdioFile* file);
	UINT ImportFemap_Elements(CStdioFile* file);
	UINT ImportFemap_Loads_OLD(CStdioFile* file);
	UINT ImportFemap_Loads(CStdioFile* file);
	UINT ImportFemap_Constraints_OLD(CStdioFile* file);
	UINT ImportFemap_Constraints(CStdioFile* file);
	UINT ImportFemap_Properties(CStdioFile* file);
	UINT ImportFemap_Materials_OLD(CStdioFile* file);
	UINT ImportFemap_Materials(CStdioFile* file);
	UINT ImportFemap_Functions(CStdioFile* file);
	UINT CModelData::ImportFemap_ContactSegment(CStdioFile* file);
	UINT CModelData::ImportFemap_ConnectionProperty(CStdioFile* file);
	UINT CModelData::ImportFemap_Connection(CStdioFile* file);
	UINT ImportFemap_AnalysisSets(CStdioFile* file);

	UINT ExportFemap(CStdioFile* file);
	UINT ExportFemap_Nodes(CStdioFile* file);
	UINT ExportFemap_Elements(CStdioFile* file);
	UINT ExportFemap_Loads(CStdioFile* file);
	UINT ExportFemap_Constraints(CStdioFile* file);
	UINT ExportFemap_Properties(CStdioFile* file);
	UINT ExportFemap_Materials(CStdioFile* file);
	UINT ExportFemap_Functions(CStdioFile* file);

	UINT FillNodeIndex();
	UINT FillElemIndex();
	UINT FillPropIndex();
	UINT FillCPropIndex();
	UINT FillConnectIndex();
	UINT FillFuncIndex();

	UINT DeleteContents();
	UINT OnNewDocument();

	UINT ImportUNV(CStdioFile* file);
	
	UINT ExportPAK_S(CStdioFile* file,CPakExpOpt *peo);
	UINT ExportPAK_T(CStdioFile* file,CPakExpOpt *peo);
	UINT ExportPAK_E(CStdioFile* file,CPakExpOpt *peo);
	UINT ExportPAK_F(CStdioFile* file,CPakExpOpt *peo);
	UINT ExportPAK_P(CStdioFile* file,CPakExpOpt *peo);
	UINT ExportPAK_PT(CStdioFile* file, CPakExpOpt *peo);
	UINT ExportPAK_M(CStdioFile* file, CPakExpOpt *peo);
	CPakOptValues m_PakOptValue;

	int GetModelTypeForElement(UINT ElementID);

	bool m_bIsThereRod;

private:
	UINT ImportUNV_Nodes(CStdioFile* file);
	UINT ImportUNV_Elements(CStdioFile* file);
	UINT ImportUNV_Loads(CStdioFile* file);
	UINT ImportUNV_Constraints(CStdioFile* file);

	UINT ExportPAK_Nodes(CStdioFile* file, int iINPT);
	UINT ExportPAK_Elements(CStdioFile* file, UINT *uNGELEM, CPakExpOpt *peo, int iINPT);
	UINT ExportPAK_MatModels(CStdioFile* file);
	UINT ExportPAK_TimeFunctions(CStdioFile* file);
	UINT ExportPAK_Loads(CStdioFile* file, int iINPT);
	UINT ExportPAK_ContactElements(CStdioFile* file, int iINPT);
	UINT ExportPAK_Loads_FindType(HMaterial* mat, int uMODEL1);
	UINT ExportPAK_Equations(CStdioFile* file, int iINPT);

	UINT ExportPAKT_Nodes(CStdioFile* file);
	UINT ExportPAKT_Elements(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo);
	UINT ExportPAKT_Elements_Without_Groups(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo);
	UINT ExportPAKT_MatModels(CStdioFile* file);
	UINT ExportPAKT_Loads(CStdioFile* file);
	UINT ExportPAKT_PrescTemp(CStdioFile* file);
	UINT ExportPAKT_TimeFunctions(CStdioFile* file);


	UINT ExportPAKE_Nodes(CStdioFile* file);
	UINT ExportPAKE_Elements(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo);
	UINT ExportPAKE_Elements_Without_Groups(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo);
	UINT ExportPAKE_MatModels(CStdioFile* file);
	UINT ExportPAKE_Loads(CStdioFile* file);
	UINT ExportPAKE_PrescTemp(CStdioFile* file);
	UINT ExportPAKE_TimeFunctions(CStdioFile* file);


	UINT ExportPAKF_Nodes(CStdioFile* file);
	UINT ExportPAKF_Elements(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo);
	UINT ExportPAKF_MatModels(CStdioFile* file);
	UINT ExportPAKF_PrescVals(CStdioFile* file);
	UINT ExportPAKF_Press(CStdioFile* file);

	UINT ExportPAKP_Nodes(CStdioFile* file, int iINPT);
	UINT ExportPAKP_Elements_Without_Groups(CStdioFile* file, UINT *uNGELEM, CPakExpOpt *peo, int iINPT);
	UINT ExportPAKP_PrescTemp(CStdioFile* file, int iINPT);
	UINT ExportPAKP_Loads(CStdioFile* file, int iINPT);
	UINT ExportPAKP_MatModels(CStdioFile* file);

	UINT ExportPAKPT_PrescTemp(CStdioFile* file, int iINPT);
	UINT ExportPAKPT_Loads(CStdioFile* file, int iINPT);
	UINT ExportPAKPT_MatModels(CStdioFile* file);

	UINT ExportPAKM_Nodes(CStdioFile* file);
	UINT ExportPAKM_Elements(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo);
	UINT ExportPAKM_Elements_Without_Groups(CStdioFile* file,UINT *uNGELEM,CPakExpOpt *peo);
	UINT ExportPAKM_MatModels(CStdioFile* file);
	UINT ExportPAKM_Loads(CStdioFile* file);
	UINT ExportPAKM_PrescTemp(CStdioFile* file);
	UINT ExportPAKM_TimeFunctions(CStdioFile* file);

public:
	bool m_bArcLenght;
	bool m_bLineSearch;
};

#endif // !defined(AFX_MODELDATA_H__F0632FFA_A3E0_4978_8FEC_1F152CF300E8__INCLUDED_)
