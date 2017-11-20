// HDataBlock.h: interface for the HDataBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDATABLOCK_H__C9812550_60EF_11D2_9943_8BCA2B38BBFE__INCLUDED_)
#define AFX_HDATABLOCK_H__C9812550_60EF_11D2_9943_8BCA2B38BBFE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class HDataBlock : public CObject  
{
public:
	HDataBlock();
	virtual ~HDataBlock();
	DECLARE_SERIAL(HDataBlock)
	void Serialize( CArchive& ar );

	UINT m_uDataBlockID;

};

#endif // !defined(AFX_HDATABLOCK_H__C9812550_60EF_11D2_9943_8BCA2B38BBFE__INCLUDED_)
