

#if !defined(AFX_BJCHENBENSETTING_H__5888D185_36B8_4AA9_A52B_688D8A42B084__INCLUDED_)
#define AFX_BJCHENBENSETTING_H__5888D185_36B8_4AA9_A52B_688D8A42B084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BjChenBenSetting  
{
public:
	BjChenBenSetting();
	virtual ~BjChenBenSetting();
	BjChenBenSetting& operator = (BjChenBenSetting & inObject);

public:
	COLORREF	m_colorLine;
	int			m_nDays;

	;
};

#endif // !defined(AFX_BJCHENBENSETTING_H__5888D185_36B8_4AA9_A52B_688D8A42B084__INCLUDED_)
