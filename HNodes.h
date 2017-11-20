// HNodes.h: interface for the HNodes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HNODES_H__A0ABEC00_6290_11D2_B069_444553540000__INCLUDED_)
#define AFX_HNODES_H__A0ABEC00_6290_11D2_B069_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HDataBlock.h"

class HNodes : public HDataBlock  
{
public:
	HNodes();
	HNodes (const HNodes& rc);
	virtual ~HNodes();

	HNodes& operator =(const HNodes& rp);
	DECLARE_SERIAL(HNodes)
	void Serialize( CArchive& ar );

	UINT m_uID;
	UINT m_uDefine_sys;// ID of definition coordinate system
	UINT m_uOutput_sys;// ID of output coordinate system
	UINT m_uLayer;
	UINT m_uColor;
	bool m_bPermbc[6];// the six permanent constaints
// Coordinates in Global Rectangular coordinate system:
	double m_dX;
	double m_dY;
	double m_dZ;

};

#endif // !defined(AFX_HNODES_H__A0ABEC00_6290_11D2_B069_444553540000__INCLUDED_)
