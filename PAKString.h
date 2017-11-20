// PAKString.h: interface for the CPAKString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAKSTRING_H__249CAF80_E73C_11D5_93CB_5254AB509BD5__INCLUDED_)
#define AFX_PAKSTRING_H__249CAF80_E73C_11D5_93CB_5254AB509BD5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

class CPAKString : public CString  
{
public:
	CPAKString();
	virtual ~CPAKString();

	void FormatV(LPCTSTR lpszFormat, va_list argList);
	void AFX_CDECL Format(LPCTSTR lpszFormat, ...);
    void RemoveExponents(LPCTSTR lpszFormat, va_list argList);
};

#endif // !defined(AFX_PAKSTRING_H__249CAF80_E73C_11D5_93CB_5254AB509BD5__INCLUDED_)
