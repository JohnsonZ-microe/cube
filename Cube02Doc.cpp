// Cube02Doc.cpp : implementation of the CCube02Doc class
//

#include "stdafx.h"
#include "Cube02.h"

#include "Cube02Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCube02Doc

IMPLEMENT_DYNCREATE(CCube02Doc, CDocument)

BEGIN_MESSAGE_MAP(CCube02Doc, CDocument)
	//{{AFX_MSG_MAP(CCube02Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCube02Doc construction/destruction

CCube02Doc::CCube02Doc()
{
	// TODO: add one-time construction code here

}

CCube02Doc::~CCube02Doc()
{
}

BOOL CCube02Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCube02Doc serialization

void CCube02Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCube02Doc diagnostics

#ifdef _DEBUG
void CCube02Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCube02Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCube02Doc commands
