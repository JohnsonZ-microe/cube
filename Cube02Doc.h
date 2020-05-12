// Cube02Doc.h : interface of the CCube02Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBE02DOC_H__953632D3_1FB2_449E_931B_85E23E7AC25F__INCLUDED_)
#define AFX_CUBE02DOC_H__953632D3_1FB2_449E_931B_85E23E7AC25F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCube02Doc : public CDocument
{
protected: // create from serialization only
	CCube02Doc();
	DECLARE_DYNCREATE(CCube02Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCube02Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCube02Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCube02Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUBE02DOC_H__953632D3_1FB2_449E_931B_85E23E7AC25F__INCLUDED_)
