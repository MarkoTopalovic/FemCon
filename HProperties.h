// HProperties.h: interface for the HProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HPROPERTIES_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_)
#define AFX_HPROPERTIES_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <afxtempl.h>
#include "HDataBlock.h"

class HProperties : public HDataBlock  
{
public:
	HProperties();
	HProperties(const HProperties& rc);
	virtual ~HProperties();

	HProperties& operator =(const HProperties& rp);
	DECLARE_SERIAL(HProperties)
	void Serialize( CArchive& ar );
	UINT m_uID;
	UINT m_uColor;
	UINT m_uMatIID;
	UINT m_uType;
	UINT m_uLayer;
	UINT m_uRefCS;// Reference coordinate system
	CString m_strTitle;
	UINT m_uFlag[4];
	UINT m_uNum_lam;
	CUIntArray m_uLam_MID;
	UINT m_uNum_val;
	CArray <double,double> m_dValue;
};

#endif // !defined(AFX_HPROPERTIES_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_)
