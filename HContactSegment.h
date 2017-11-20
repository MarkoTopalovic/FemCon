// HLoads.h: interface for the HLoads class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCONNTACTSEGMENT_H__ADDF1161_651D_11D2_8266_004F4900B9E0__INCLUDED_)
#define AFX_HCONNTACTSEGMENT_H__ADDF1161_651D_11D2_8266_004F4900B9E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "HDataBlock.h"

class Entity : public CObject
{
public:
	 Entity();
	Entity& operator = (const Entity& rc);
	Entity(const Entity& rc);
	UINT m_uEntityID;
	UINT m_uFaceID;
	UINT m_uType;
	UINT m_uExpanded;
};

class HContactSegment : public HDataBlock  
{
public:	
	HContactSegment();
	HContactSegment(const HContactSegment& rc);
	virtual ~HContactSegment();
	HContactSegment& operator = (const HContactSegment& rp);
	DECLARE_SERIAL(HContactSegment)
	void Serialize( CArchive& ar );

	UINT m_uID;
	UINT m_uColor;
	UINT m_uLayer;
	UINT m_uOutput;
	UINT m_uRefnode;
	UINT m_uRigid;
	UINT m_uRef_sys;
	UINT m_uOffset;
	CString m_strTitle;

	CArray<Entity,Entity&> m_Entity;

};

#endif // !defined(AFX_HLOADS_H__ADDF1161_651D_11D2_8266_004F4900B9E0__INCLUDED_)
