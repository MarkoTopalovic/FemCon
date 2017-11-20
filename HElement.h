// HElement.h: interface for the HElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELEMENT_H__ABFFBE40_6355_11D2_B069_444553540000__INCLUDED_)
#define AFX_HELEMENT_H__ABFFBE40_6355_11D2_B069_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HDataBlock.h"

class HElement : public HDataBlock  
{
public:
	HElement();
	HElement(HElement& rc);
	virtual ~HElement();

	HElement& operator = (const HElement& rp);

	DECLARE_SERIAL(HElement)
	void Serialize( CArchive& ar );

	UINT m_uID;
	UINT m_uColor;
	UINT m_uPropID;
	UINT m_uType;
	UINT m_uTopology;
	UINT m_uLayer;
	UINT m_uOrientID; 
	bool m_bMatl_orflag;// Material orientation flag
	UINT m_uNode[20];
	double m_dOrient[3];
	double m_dOffset1[3];
	double m_dOffset2[3];
	bool m_bRelease1[6];
	bool m_bRelease2[6];

};

#endif // !defined(AFX_HELEMENT_H__ABFFBE40_6355_11D2_B069_444553540000__INCLUDED_)
