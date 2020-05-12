// CubeShow.h: interface for the CCubeShow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBESHOW_H__24EAA6BE_9D68_4804_931D_8FDEE667BC1F__INCLUDED_)
#define AFX_CUBESHOW_H__24EAA6BE_9D68_4804_931D_8FDEE667BC1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define meshLen			20
#define meshFullLen		60

// 6个面方位( 前、后、左、右、上、下 )和无方位
enum Aspect{ FRONT, BACK, LEFT, RIGHT, UP, DOWN, NOASP };

// 灰色，可见的方位面颜色( 蓝、绿、红、橙、黄、白)
enum Color{ BLUE, GREEN, ORANGE, RED, YELLOW, WHITE, GRAY};

class CCubeShow : public CObject  
{
public:
	CCubeShow();
	virtual ~CCubeShow();

public:
	void ShowAllCube( CDC *pDC );

	void SetCubeColor( int idx, char input[][8] );
	CString GetCubeColor( int idx );
	void rotate( int cube, Aspect asp, int p );

	void InitCube( int cube );
	void CopyCube( int dstCube, int srcCube );

private:
	CPoint org1, org2, org3, org4;
	CBrush brushs[7];				//白、黄、蓝、绿、 红、橙、灰色
	CPoint vertexes[4][6][9][4];	//FRONT,BACK,LEFT,RIGHT,UP,DOWN 初始化魔方顶点
	Color aspectColors[4][6][9];    //初始化个面颜色， 颜色为Color类
};

#endif // !defined(AFX_CUBESHOW_H__24EAA6BE_9D68_4804_931D_8FDEE667BC1F__INCLUDED_)
