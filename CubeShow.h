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

// 6���淽λ( ǰ�������ҡ��ϡ��� )���޷�λ
enum Aspect{ FRONT, BACK, LEFT, RIGHT, UP, DOWN, NOASP };

// ��ɫ���ɼ��ķ�λ����ɫ( �����̡��졢�ȡ��ơ���)
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
	CBrush brushs[7];				//�ס��ơ������̡� �졢�ȡ���ɫ
	CPoint vertexes[4][6][9][4];	//FRONT,BACK,LEFT,RIGHT,UP,DOWN ��ʼ��ħ������
	Color aspectColors[4][6][9];    //��ʼ��������ɫ�� ��ɫΪColor��
};

#endif // !defined(AFX_CUBESHOW_H__24EAA6BE_9D68_4804_931D_8FDEE667BC1F__INCLUDED_)
