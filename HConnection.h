// HCONNECTION.h: interface for the HCProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCONNECTION_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_)
#define AFX_HCONNECTION_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <afxtempl.h>
#include "HDataBlock.h"

class HConnection : public HDataBlock  
{
public:
	HConnection();
	HConnection(const HConnection& rc);
	virtual ~HConnection();

	HConnection& operator =(const HConnection& rp);
	DECLARE_SERIAL(HConnection)
	void Serialize( CArchive& ar );
	UINT m_uID;
	UINT m_uColor;
	UINT m_Pid;
	UINT m_uType;
	UINT m_uLayer;
	UINT m_Disabled;
	CString m_strTitle;
	UINT m_uregionID[2];
	//	CUIntArray m_uregionID;
};

#endif // !defined(AFX_HConnection_H__E4BF19A3_6284_11D2_B069_444553540000__INCLUDED_)
