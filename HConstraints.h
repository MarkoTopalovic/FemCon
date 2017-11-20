// HConstraints.h: interface for the HConstraints class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCONSTRAINTS_H__7D2CD282_6362_11D2_8266_004F4900B9E0__INCLUDED_)
#define AFX_HCONSTRAINTS_H__7D2CD282_6362_11D2_8266_004F4900B9E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "HDataBlock.h"


class ConsNode : public CObject
{
public:
	ConsNode& operator =(const ConsNode& rc);
	 ConsNode(const ConsNode& rc);
	ConsNode();
	~ConsNode();
	UINT m_uNodeID;
	UINT m_uColor;
	UINT m_uLayer;
	bool m_bDOF[9];
};

	struct EqCoeff
	{
		UINT m_uEqn_nodeID;
		UINT m_uEqn_dof;
		double m_dCoeff;
	};

class ConsEq:public CObject
{
public:
	ConsEq ();
	ConsEq (const ConsEq& rc);
	~ConsEq ();
	ConsEq& operator = (const ConsEq& rc);
	UINT m_uEqnID;
	UINT m_uColor;
	UINT m_uLayer;
	CArray<EqCoeff,EqCoeff> m_EqCoefs;	
};

class HConstraints : public HDataBlock  
{
public:
	HConstraints();
	HConstraints (const HConstraints& rc);

	virtual ~HConstraints();

	DECLARE_SERIAL(HConstraints)
	void Serialize( CArchive& ar );
	HConstraints& operator =(const HConstraints& rp);

	UINT m_uSetID;
	CString m_strTitle;

	CArray<ConsNode,ConsNode&> m_ConsNodes;
	CArray<ConsEq,ConsEq&> m_ConsEqs;

};

#endif // !defined(AFX_HCONSTRAINTS_H__7D2CD282_6362_11D2_8266_004F4900B9E0__INCLUDED_)
