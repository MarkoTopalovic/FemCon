// HCProperties.h: interface for the HCProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCPROPERTIES_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_)
#define AFX_HCPROPERTIES_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <afxtempl.h>
#include "HDataBlock.h"

class HCProperties : public HDataBlock  
{
public:
	HCProperties();
	HCProperties(const HCProperties& rc);
	virtual ~HCProperties();

	HCProperties& operator =(const HCProperties& rp);
	DECLARE_SERIAL(HCProperties)
	void Serialize( CArchive& ar );
	UINT m_uID;
	UINT m_uColor;
	UINT m_Zero1;
	UINT m_uType;
	UINT m_uLayer;
	UINT m_Zero2;
	CString m_strTitle;
	UINT m_uFlag[4];
	UINT m_uMaxExtr_Mat;
	CUIntArray m_uExtra_Matl;
	UINT m_uMax_val;
	CArray <double,double> m_dValue;
};

#endif // !defined(AFX_HCPROPERTIES_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_)
