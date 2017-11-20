// HFunctions.h: interface for the HFunctions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HFUNCTIONS_H__342BA6A3_28D7_11D0_84F7_CCED03C10000__INCLUDED_)
#define AFX_HFUNCTIONS_H__342BA6A3_28D7_11D0_84F7_CCED03C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "HDataBlock.h"

typedef struct 
{
	UINT m_uEntryID;//ID of entry (ascending order, must be -1 for last entry)	
	double m_dX;//X value for entry in function	
	double m_dY;//Y value for entry in function
} FunctionEntry;

class HFunctions : public HDataBlock  
{
public:

	HFunctions();
	HFunctions (const HFunctions& rc);

	virtual ~HFunctions();

	DECLARE_SERIAL(HFunctions)      
	void Serialize( CArchive& ar );
	HFunctions& operator =(const HFunctions& rp);

	UINT m_uID;//ID of function
	UINT m_uFunc_type;//Type of function (0=Dimensionless, 1=vs. Time, 2=vs. Temp)	
	CString m_strTitle;//Function title (max 25 char)	
	//nitems records, one per function entry	
	CArray<FunctionEntry,FunctionEntry> m_FunctionEntry;
};

#endif // !defined(AFX_HFUNCTIONS_H__342BA6A3_28D7_11D0_84F7_CCED03C10000__INCLUDED_)
