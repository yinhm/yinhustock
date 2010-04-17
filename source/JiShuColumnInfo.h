#if !defined(AFX_JISHUCOLUMNINFO_H__F1784DC1_1466_11D4_8829_00400552E583__INCLUDED_)
#define AFX_JISHUCOLUMNINFO_H__F1784DC1_1466_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// TechniqueColumnInfo 

class TechniqueColumnInfo 
{
// Construction
public:
	TechniqueColumnInfo(CTaiShanDoc *pDoc);
	CTaiShanDoc *m_pDoc;

// Attributes
public:

// Operations
public:
//   TECHNIQUE_COLUMN  m_Technique[MAXJISHUCOUNT];
   UINT FindSolt(PTECHNIQUE_COLUMN& pIndexIndex);
   BOOL GetTechniqueFromId(UINT nId,PTECHNIQUE_COLUMN& pTechniqueIndex);
   BOOL GetTechniqueFromIndexName(CString IndexName,PTECHNIQUE_COLUMN& pTechniqueIndex );
   void SetColumnNotUse(UINT nId);
   void SetAllColumnNotUse();
   
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TechniqueColumnInfo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TechniqueColumnInfo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JISHUCOLUMNINFO_H__F1784DC1_1466_11D4_8829_00400552E583__INCLUDED_)
