// Cube02View.h : interface of the CCube02View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBE02VIEW_H__329F9047_917B_4E1E_9DCC_CD8FC7A83E03__INCLUDED_)
#define AFX_CUBE02VIEW_H__329F9047_917B_4E1E_9DCC_CD8FC7A83E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CubeCalc.h"
#include "CubeShow.h"

/***********
* 魔方坐标 *
***********/

/*
// 旋转轴
enum Axis{ X, Y, Z };

// description of chunk position
enum ChunkIndex{ NEGATIVE, MIDDLE, POSITIVE };

// 旋转方向( 顺时针、逆时针、不旋转 ) 
enum RotateDirection{ CLOCKWISE, COUNTERCLOCKWISE, NOROTATE};
*/

/*
// 面
class Facelet{
public:
	Aspect asp;
	int index;
	Color color;
	Facelet() : asp(NOASP), index(-1), color(GRAY) {}
};

// 棱，包括两个面
class Edge
{
public:
	Facelet faceletMain;
	Facelet faceletSide;
};

// 角, 包括三个面
class Corner
{
public:
	Facelet faceletMain;
	Facelet facelet1;
	Facelet facelet2;
};
*/

class CCube02View : public CView
{
protected: // create from serialization only
	CCube02View();
	DECLARE_DYNCREATE(CCube02View)

private:
	
// Attributes
public:
	CCube02Doc* GetDocument();

private:                      //DWORD  32位4字节无符号整数
	DWORD v_nCount;           
	DWORD v_nCountCccc;
	DWORD v_nCountNnnn;
	DWORD v_nCountError;
	DWORD v_nTotalCcc;	//总次数
	DWORD v_nTotalNnn;	//总步数
	int v_nMaxNnn;

	CCubeCalc v_CubeCalc;
	CCubeShow v_CubeShow;

private:
	void CubeProcStep1();
	void CubeProcStep2();
	void CubeProcStep3();
	void CubeProcStep4();
	void CubeProcStep5();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCube02View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCube02View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCube02View)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Cube02View.cpp
inline CCube02Doc* CCube02View::GetDocument()
   { return (CCube02Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUBE02VIEW_H__329F9047_917B_4E1E_9DCC_CD8FC7A83E03__INCLUDED_)
