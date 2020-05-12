// CubeTable.h: interface for the CCubeTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBETABLE_H__4D71CE01_C2C3_4FD2_9485_667C93AB5E2A__INCLUDED_)
#define AFX_CUBETABLE_H__4D71CE01_C2C3_4FD2_9485_667C93AB5E2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCubeTable : public CObject  
{
public:
	CCubeTable();
	virtual ~CCubeTable();

private:
	// 魔方当前位置
	int v_pos[ 20 ];	//各个位置所含的方块， 0..19
	int v_ori[ 20 ];	//各个位置所含方块的朝向, 为0, 1, 2 

	// 临时变量
	int temp;

	static const char *perm;
	static const char *corn;

public:
	// pruning tables, 2 for each phase
	char table0[ 1     ];
	char table1[ 4096  ];  //4096 = 2^12 12个棱的v_ori
	char table2[ 6561  ];  //6561 = 3^8   8个角的v_ori
	char table3[ 4096  ];  
	char table4[ 256   ];  
	char table5[ 1536  ];  // 1536 = 24*64
	char table6[ 13824 ];  //13824 = 24*24*24
	char table7[ 576   ];  //  576 = 24*24

	char *tables [8];
	int tablesize[8];

public:
	void filltable( int ti );

private:
	void reset();
	void cycle( int *p, char *a );
	void twist( int i, int a );
	void domove( int m );
	void numtoperm( int *p, int n, int ofp );
	int permtonum( int *p, int ofp );
	void setposition( int t, int n );
	int getposition( int t );
};

CCubeTable theTable;

#endif // !defined(AFX_CUBETABLE_H__4D71CE01_C2C3_4FD2_9485_667C93AB5E2A__INCLUDED_)
