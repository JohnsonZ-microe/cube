// CubeShow.cpp: implementation of the CCubeShow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cube02.h"
#include "CubeShow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const Color aspectColor_org[ 6 ][ 9 ] = {                                   //定义初始魔方状态
	//front
	{ BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE },
	//back
	{ GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN },
	//left
	{ ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE },
	//right
	{ RED, RED, RED, RED, RED, RED, RED, RED, RED },	
	//up
	{ YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW },
	//down
	{ WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE }
	
};

const char colfaces[] = "FBLRUD";                                          //魔方个面顺序字符串
const Color colTable[6] = { BLUE, GREEN, ORANGE, RED, YELLOW, WHITE };     //颜色顺序字符串

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCubeShow::CCubeShow() :
	org1( meshLen*3 +5, meshLen*2 *3-10 ),
	org2( meshLen*15+5, meshLen*2 *3-10 ),	
	org3( meshLen*15+5, meshLen*5*3-10 ),
	org4( meshLen*3 +5, meshLen*5*3-10 )
{
		// TODO: add construction code here
	brushs[FRONT].CreateSolidBrush( RGB(0,    0,255));	// 蓝色
	brushs[BACK ].CreateSolidBrush( RGB(0,  255,  0));	// 绿色	
	brushs[LEFT ].CreateSolidBrush( RGB(255,132,  0));	// 橙色
	brushs[RIGHT].CreateSolidBrush( RGB(255,  0,  0));	// 红色
	brushs[UP   ].CreateSolidBrush( RGB(255,255,  0));	// 黄色
	brushs[DOWN ].CreateSolidBrush( RGB(255,255,255));	// 白色
	brushs[NOASP].CreateSolidBrush( RGB(153,153,153));	// 灰色

	//
	for( int iCube=0; iCube<4; iCube++ )
		InitCube( iCube );                       //初始化四个魔方，以便绘在窗口中
	
	//
	CPoint *org[] = { (CPoint *)&org1, (CPoint *)&org2, (CPoint *)&org3, (CPoint *)&org4 };

	//vertexes for every aspect
	//绘制魔方图样
	for( iCube=0; iCube<4; iCube++ )
	{
		//front
		vertexes[iCube][FRONT][0][0].x = org[iCube]->x;
		vertexes[iCube][FRONT][0][0].y = org[iCube]->y;
		vertexes[iCube][FRONT][0][1].x = org[iCube]->x;
		vertexes[iCube][FRONT][0][1].y = org[iCube]->y - meshLen;
		vertexes[iCube][FRONT][0][2].x = org[iCube]->x + meshLen;
		vertexes[iCube][FRONT][0][2].y = org[iCube]->y - meshLen;
		vertexes[iCube][FRONT][0][3].x = org[iCube]->x + meshLen;
		vertexes[iCube][FRONT][0][3].y = org[iCube]->y;
	
		for (int i=1;i<=2;i++)
		{
			for (int ind=0;ind<4;ind++)
			{
				vertexes[iCube][FRONT][i][ind].x = vertexes[iCube][FRONT][i-1][ind].x + meshLen;
				vertexes[iCube][FRONT][i][ind].y = vertexes[iCube][FRONT][0  ][ind].y;
			}
		}

		for ( i=3;i<=8;i++)
		{
			for (int ind=0;ind<4;ind++)
			{
				vertexes[iCube][FRONT][i][ind].x = vertexes[iCube][FRONT][i-3][ind].x;
				vertexes[iCube][FRONT][i][ind].y = vertexes[iCube][FRONT][i-3][ind].y - meshLen;
			}
		}

		//back
		float len = meshLen*0.707106f;
		CPoint v( int(org[iCube]->x+meshLen*3+int(len*3)), int(org[iCube]->y-int(len*3)) );
		vertexes[iCube][BACK][2][0]   = v;
		vertexes[iCube][BACK][2][1].x = v.x;
		vertexes[iCube][BACK][2][1].y = v.y - meshLen;
		vertexes[iCube][BACK][2][2].x = v.x + meshLen;
		vertexes[iCube][BACK][2][2].y = v.y - meshLen;
		vertexes[iCube][BACK][2][3].x = v.x + meshLen;
		vertexes[iCube][BACK][2][3].y = v.y;
		
		for ( i=1; i>=0; i--)
		{
			for (int ind=0;ind<4;ind++)
			{
				vertexes[iCube][BACK][i][ind].x = vertexes[iCube][BACK][i+1][ind].x + meshLen;
				vertexes[iCube][BACK][i][ind].y = vertexes[iCube][BACK][2  ][ind].y;
			}
		}
	
		for ( i=3;i<=8;i++)
		{
			for (int ind=0;ind<4;ind++)
			{
				vertexes[iCube][BACK][i][ind].x = vertexes[iCube][BACK][i-3][ind].x;
				vertexes[iCube][BACK][i][ind].y = vertexes[iCube][BACK][i-3][ind].y - meshLen;
			}
		}

		//left
		for( int ind=0; ind<4; ind++ )
		{
			vertexes[iCube][LEFT][0][ind].x = vertexes[iCube][FRONT][0][ind].x - meshLen*3;
			vertexes[iCube][LEFT][0][ind].y = vertexes[iCube][FRONT][0][ind].y;
		}
	
		for( i=1; i<=2; i++ )
		{
			for(int ind=0; ind<4; ind++ )
			{
				vertexes[iCube][LEFT][i][ind].y = vertexes[iCube][LEFT][i-1][ind].y - meshLen;
				vertexes[iCube][LEFT][i][ind].x = vertexes[iCube][LEFT][0  ][ind].x;
			}
		}

		for ( i=3;i<=8;i++)
		{
			for (int ind=0;ind<4;ind++)
			{
				vertexes[iCube][LEFT][i][ind].x = vertexes[iCube][LEFT][i-3][ind].x + meshLen;
				vertexes[iCube][LEFT][i][ind].y = vertexes[iCube][LEFT][i-3][ind].y;
			}
		}

		//right
		vertexes[iCube][RIGHT][0][3]=v;
		vertexes[iCube][RIGHT][0][0].x = v.x - int(len);
		vertexes[iCube][RIGHT][0][0].y = v.y + int(len);
		vertexes[iCube][RIGHT][0][1].x = vertexes[iCube][RIGHT][0][0].x;
		vertexes[iCube][RIGHT][0][1].y = vertexes[iCube][RIGHT][0][0].y - meshLen;
		vertexes[iCube][RIGHT][0][2].x = vertexes[iCube][RIGHT][0][3].x;
		vertexes[iCube][RIGHT][0][2].y = vertexes[iCube][RIGHT][0][3].y - meshLen;
	
		for ( i=1;i<=2;i++)
		{
			for (int ind=0;ind<4;ind++)
			{
				vertexes[iCube][RIGHT][i][ind].y = vertexes[iCube][RIGHT][i-1][ind].y - meshLen;
				vertexes[iCube][RIGHT][i][ind].x = vertexes[iCube][RIGHT][0  ][ind].x;
			}
		}
	
		for ( i=3;i<=8;i++)
		{
			for (int ind=0;ind<4;ind++)
			{
				vertexes[iCube][RIGHT][i][ind].x = vertexes[iCube][RIGHT][i-3][ind].x - int(len);
				vertexes[iCube][RIGHT][i][ind].y = vertexes[iCube][RIGHT][i-3][ind].y + int(len);
			}
		}

		//up
		v.x = org[iCube]->x + int(len*3);
		v.y = org[iCube]->y - int(len*3)-meshLen*3;
		vertexes[iCube][UP][0][0].x = v.x-int(len);
		vertexes[iCube][UP][0][0].y = v.y+int(len);
		vertexes[iCube][UP][0][1]   = v;
		vertexes[iCube][UP][0][2].x = v.x+meshLen;
		vertexes[iCube][UP][0][2].y = v.y;
		vertexes[iCube][UP][0][3].x = vertexes[iCube][UP][0][0].x + meshLen;
		vertexes[iCube][UP][0][3].y = vertexes[iCube][UP][0][0].y;
		
		for ( i=1; i<=2; i++ )
		{		
			for (int ind=0; ind<4; ind++ )
			{
				vertexes[iCube][UP][i][ind].x = vertexes[iCube][UP][i-1][ind].x + meshLen;
				vertexes[iCube][UP][i][ind].y = vertexes[iCube][UP][0  ][ind].y;
			}
		}

		for ( i=3; i<=8; i++ )
		{
			for (int ind=0; ind<4; ind++ )
			{
				vertexes[iCube][UP][i][ind].x = vertexes[iCube][UP][i-3][ind].x - int(len);
				vertexes[iCube][UP][i][ind].y = vertexes[iCube][UP][i-3][ind].y + int(len);
			}
		}

		//down
		for ( i=0; i<9; i++ )
		{
			for (int ind=0; ind<4; ind++ )
			{
				vertexes[iCube][DOWN][i][ind].x = vertexes[iCube][FRONT][i][ind].x;
				vertexes[iCube][DOWN][i][ind].y = vertexes[iCube][FRONT][i][ind].y + meshLen*3;
			}
		}
	}
}

CCubeShow::~CCubeShow()
{

}

void CCubeShow::ShowAllCube( CDC *pDC )
{
	CPoint *org[] = { (CPoint *)&org1, (CPoint *)&org2, (CPoint *)&org3, (CPoint *)&org4 };

	for( int iCube = 0; iCube < 4; iCube++ )
	{
		CPen pen(PS_SOLID,1,BLACK_PEN);                  //CPen类画笔函数，语法：pen（画笔样式，画笔宽度，画笔颜色）
		CPen *pOldPen = pDC->SelectObject( &pen );
		for (int i=0;i<6;i++)
		{
			for (int asp=0;asp<9;asp++)
			{
				pDC->SelectObject( &brushs[ aspectColors[iCube][i][asp]] );
				pDC->Polygon( vertexes[iCube][i][asp], 4 ); //将四副魔方图全部在桌面上绘出

				/*
				strTemp.Format( "%d", asp );
				pDC->TextOut( 
					( vertexes[iCube][i][asp][0].x + vertexes[iCube][i][asp][2].x ) / 2,
					( vertexes[iCube][i][asp][0].y + vertexes[iCube][i][asp][2].y ) / 2,
					strTemp );
				*/
			}
		}

		//strTemp.Format( "Cube %d", iCube );
		//MemDC.TextOut( org[iCube]->x-2*meshLen, org[iCube]->y+meshLen, strTemp );

		pDC->SelectObject( pOldPen );
	}

/*
	//show selected color
	CRect rc( org.x+(meshFullLen<<1), org.y, org.x+(meshFullLen<<1)+meshLen, org.y+meshLen );
	pDC->FillRect( &rc, &brushs[colorSelected] );
	//text
	CFont font;
	VERIFY(font.CreateFont(
		15,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename
	CFont *pOldFont = pDC->SelectObject(&font);
	COLORREF theTextColor = pDC->SetTextColor( RGB(255,255,255) );
	COLORREF theBkColor = pDC->SetBkColor(RGB(0,0,0));
	//
	char* text="Selected Color";
	pDC->TextOut( rc.left-20, rc.bottom+3, text, strlen(text) );
	//
	pDC->SelectObject( pOldFont );
	pDC->SetTextColor( theTextColor );
	pDC->SetBkColor( theBkColor );
*/
}

void CCubeShow::SetCubeColor( int idx, char input[][8] )
{
	//颜色
	//  上下面，从后到前，从左到右，序号0..8
	//  左右面，从后到前，从下到上，序号0..8
	//  前后面，从下到上，从左到右，序号0..8

	//UF位置颜色
	aspectColors[idx][UP   ][7] = colTable[ strchr( colfaces, input[0][0] ) - colfaces ];
	aspectColors[idx][FRONT][7] = colTable[ strchr( colfaces, input[0][1] ) - colfaces ];
	//UR位置颜色
	aspectColors[idx][UP   ][5] = colTable[ strchr( colfaces, input[1][0] ) - colfaces ];
	aspectColors[idx][RIGHT][5] = colTable[ strchr( colfaces, input[1][1] ) - colfaces ];
	//UB位置颜色
	aspectColors[idx][UP   ][1] = colTable[ strchr( colfaces, input[2][0] ) - colfaces ];
	aspectColors[idx][BACK ][7] = colTable[ strchr( colfaces, input[2][1] ) - colfaces ];
	//UL位置颜色
	aspectColors[idx][UP   ][3] = colTable[ strchr( colfaces, input[3][0] ) - colfaces ];
	aspectColors[idx][LEFT ][5] = colTable[ strchr( colfaces, input[3][1] ) - colfaces ];

	//DF位置颜色
	aspectColors[idx][DOWN ][7] = colTable[ strchr( colfaces, input[4][0] ) - colfaces ];
	aspectColors[idx][FRONT][1] = colTable[ strchr( colfaces, input[4][1] ) - colfaces ];
	//DR位置颜色
	aspectColors[idx][DOWN ][5] = colTable[ strchr( colfaces, input[5][0] ) - colfaces ];
	aspectColors[idx][RIGHT][3] = colTable[ strchr( colfaces, input[5][1] ) - colfaces ];
	//DB位置颜色
	aspectColors[idx][DOWN ][1] = colTable[ strchr( colfaces, input[6][0] ) - colfaces ];
	aspectColors[idx][BACK ][1] = colTable[ strchr( colfaces, input[6][1] ) - colfaces ];
	//DL位置颜色
	aspectColors[idx][DOWN ][3] = colTable[ strchr( colfaces, input[7][0] ) - colfaces ];
	aspectColors[idx][LEFT ][3] = colTable[ strchr( colfaces, input[7][1] ) - colfaces ];

	//FR位置颜色
	aspectColors[idx][FRONT][5] = colTable[ strchr( colfaces, input[8][0] ) - colfaces ];
	aspectColors[idx][RIGHT][7] = colTable[ strchr( colfaces, input[8][1] ) - colfaces ];
	//FL位置颜色
	aspectColors[idx][FRONT][3] = colTable[ strchr( colfaces, input[9][0] ) - colfaces ];
	aspectColors[idx][LEFT ][7] = colTable[ strchr( colfaces, input[9][1] ) - colfaces ];
	//BR位置颜色
	aspectColors[idx][BACK ][5] = colTable[ strchr( colfaces, input[10][0] ) - colfaces ];
	aspectColors[idx][RIGHT][1] = colTable[ strchr( colfaces, input[10][1] ) - colfaces ];
	//BL位置颜色
	aspectColors[idx][BACK ][3] = colTable[ strchr( colfaces, input[11][0] ) - colfaces ];
	aspectColors[idx][LEFT ][1] = colTable[ strchr( colfaces, input[11][1] ) - colfaces ];

	//UFR位置颜色
	aspectColors[idx][UP   ][8] = colTable[ strchr( colfaces, input[12][0] ) - colfaces ];
	aspectColors[idx][FRONT][8] = colTable[ strchr( colfaces, input[12][1] ) - colfaces ];
	aspectColors[idx][RIGHT][8] = colTable[ strchr( colfaces, input[12][2] ) - colfaces ];
	//URB位置颜色
	aspectColors[idx][UP   ][2] = colTable[ strchr( colfaces, input[13][0] ) - colfaces ];
	aspectColors[idx][RIGHT][2] = colTable[ strchr( colfaces, input[13][1] ) - colfaces ];
	aspectColors[idx][BACK ][8] = colTable[ strchr( colfaces, input[13][2] ) - colfaces ];
	//UBL位置颜色
	aspectColors[idx][UP   ][0] = colTable[ strchr( colfaces, input[14][0] ) - colfaces ];
	aspectColors[idx][BACK ][6] = colTable[ strchr( colfaces, input[14][1] ) - colfaces ];
	aspectColors[idx][LEFT ][2] = colTable[ strchr( colfaces, input[14][2] ) - colfaces ];
	//ULF位置颜色
	aspectColors[idx][UP   ][6] = colTable[ strchr( colfaces, input[15][0] ) - colfaces ];
	aspectColors[idx][LEFT ][8] = colTable[ strchr( colfaces, input[15][1] ) - colfaces ];
	aspectColors[idx][FRONT][6] = colTable[ strchr( colfaces, input[15][2] ) - colfaces ];

	//DRF位置颜色
	aspectColors[idx][DOWN ][8] = colTable[ strchr( colfaces, input[16][0] ) - colfaces ];
	aspectColors[idx][RIGHT][6] = colTable[ strchr( colfaces, input[16][1] ) - colfaces ];
	aspectColors[idx][FRONT][2] = colTable[ strchr( colfaces, input[16][2] ) - colfaces ];
	//DFL位置颜色
	aspectColors[idx][DOWN ][6] = colTable[ strchr( colfaces, input[17][0] ) - colfaces ];
	aspectColors[idx][FRONT][0] = colTable[ strchr( colfaces, input[17][1] ) - colfaces ];
	aspectColors[idx][LEFT ][6] = colTable[ strchr( colfaces, input[17][2] ) - colfaces ];
	//DLB位置颜色
	aspectColors[idx][DOWN ][0] = colTable[ strchr( colfaces, input[18][0] ) - colfaces ];
	aspectColors[idx][LEFT ][0] = colTable[ strchr( colfaces, input[18][1] ) - colfaces ];
	aspectColors[idx][BACK ][0] = colTable[ strchr( colfaces, input[18][2] ) - colfaces ];
	//DBR位置颜色
	aspectColors[idx][DOWN ][2] = colTable[ strchr( colfaces, input[19][0] ) - colfaces ];
	aspectColors[idx][BACK ][2] = colTable[ strchr( colfaces, input[19][1] ) - colfaces ];
	aspectColors[idx][RIGHT][0] = colTable[ strchr( colfaces, input[19][2] ) - colfaces ];
}

//
CString CCubeShow::GetCubeColor( int idx )
{
	CString strTemp;

	//char *colortab = "BGORYW";
	char *colortab = "FBLRUD";

	//UF位置颜色
	strTemp += colortab[ aspectColors[idx][UP   ][7] ];
	strTemp += colortab[ aspectColors[idx][FRONT][7] ];
	//UR位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][5] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][5] ];
	//UB位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][1] ];
	strTemp += colortab[ aspectColors[idx][BACK ][7] ];
	//UL位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][3] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][5] ];

	//DF位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][7] ];
	strTemp += colortab[ aspectColors[idx][FRONT][1] ];
	//DR位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][5] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][3] ];
	//DB位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][1] ];
	strTemp += colortab[ aspectColors[idx][BACK ][1] ];
	//DL位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][3] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][3] ];

	//FR位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][FRONT][5] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][7] ];
	//FL位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][FRONT][3] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][7] ];
	//BR位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][BACK ][5] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][1] ];
	//BL位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][BACK ][3] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][1] ];
	
	//UFR位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][8] ];
	strTemp += colortab[ aspectColors[idx][FRONT][8] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][8] ];
	//URB位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][2] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][2] ];
	strTemp += colortab[ aspectColors[idx][BACK ][8] ];	
	//UBL位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][0] ];
	strTemp += colortab[ aspectColors[idx][BACK ][6] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][2] ];	
	//ULF位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][6] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][8] ];
	strTemp += colortab[ aspectColors[idx][FRONT][6] ];
	
	//DRF位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][8] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][6] ];
	strTemp += colortab[ aspectColors[idx][FRONT][2] ];
	//DFL位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][6] ];
	strTemp += colortab[ aspectColors[idx][FRONT][0] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][6] ];
	strTemp += " ";
	//DLB位置颜色
	strTemp += colortab[ aspectColors[idx][DOWN ][0] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][0] ];
	strTemp += colortab[ aspectColors[idx][BACK ][0] ];
	
	//DBR位置颜色
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][2] ];
	strTemp += colortab[ aspectColors[idx][BACK ][2] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][0] ];

	//
	return strTemp;
}

//
// 转1，2，3 次，分别对应 正转、对转和反转
// 旋转面 正转 对转 反转  内部棱循环       内部角循环       相关面循环(上下、前后、左右均相反)
//   前    1   2    3      1→3→7→5→1   0→6→8→2→0    右8,7,6→下8,7,6→左6,7,8→上6,7,8→右8,7,6
//   后    1   2    3      1→5→7→3→1   0→2→8→6→0    右2,1,0→上0,1,2→左0,1,2→下2,1,0→右2,1,0
//   左    1   2    3      1→5→7→3→1   0→2→8→6→0    上0,3,6→前6,3,0→下0,3,6→后6,3,0→上0,3,6
//   右    1   2    3      1→3→7→5→1   0→6→8→2→0    上2,5,8→后2,5,8→下8,5,2→前8,5,2→上2,5,8
//   上    1   2    3      1→5→7→3→1   0→2→8→6→0    前6,7,8→左2,5,8→后8,7,6→右8,5,2→前6,7,8
//   下    1   2    3      1→3→7→5→1   0→6→8→2→0    前0,1,2→右0,3,6→后2,1,0→左6,3,0→前0,1,2

//
const struct
{
	Aspect f0;
	int l0, l1, l2, l3;
	int j0, j1, j2, j3;
	Aspect f1;
	int a0, a1, a2;
	Aspect f2;
	int b0, b1, b2;
	Aspect f3;
	int c0, c1, c2;
	Aspect f4;
	int d0, d1, d2;
} xx[] = {
	{ FRONT, 0, 6, 8, 2, 1, 3, 7, 5, RIGHT, 8, 7, 6, DOWN,  8, 7, 6, LEFT, 6, 7, 8, UP,    6, 7, 8 },
	{ BACK,  0, 2, 8, 6, 1, 5, 7, 3, RIGHT, 2, 1, 0, UP,    0, 1, 2, LEFT, 0, 1, 2, DOWN,  2, 1, 0 },
	{ LEFT,  0, 2, 8, 6, 1, 5, 7, 3, UP,    0, 3, 6, FRONT, 6, 3, 0, DOWN, 6, 3, 0, BACK,  0, 3, 6 },
	{ RIGHT, 0, 6, 8, 2, 1, 3, 7, 5, UP,    2, 5, 8, BACK,  2, 5, 8, DOWN, 8, 5, 2, FRONT, 8, 5, 2 },
	{ UP,    0, 2, 8, 6, 1, 5, 7, 3, FRONT, 6, 7, 8, LEFT,  2, 5, 8, BACK, 8, 7, 6, RIGHT, 8, 5, 2 },
	{ DOWN,  0, 6, 8, 2, 1, 3, 7, 5, FRONT, 0, 1, 2, RIGHT, 6, 3, 0, BACK, 2, 1, 0, LEFT,  0, 3, 6 },	
};

//p为旋转次数
void CCubeShow::rotate( int cube, Aspect asp, int p )
{
	Color temp[6][9];
	
	for( int i=0; i<p; i++ )
	{
		memcpy( temp, &aspectColors[cube], 6*9*sizeof(Color) );

		if( asp != xx[asp].f0 )
			continue;		

		//棱
		temp[asp][ xx[asp].l1 ] = aspectColors[cube][asp][ xx[asp].l0 ];
		temp[asp][ xx[asp].l2 ] = aspectColors[cube][asp][ xx[asp].l1 ];
		temp[asp][ xx[asp].l3 ] = aspectColors[cube][asp][ xx[asp].l2 ];
		temp[asp][ xx[asp].l0 ] = aspectColors[cube][asp][ xx[asp].l3 ];
		//角
		temp[asp][ xx[asp].j1 ] = aspectColors[cube][asp][ xx[asp].j0 ];
		temp[asp][ xx[asp].j2 ] = aspectColors[cube][asp][ xx[asp].j1 ];
		temp[asp][ xx[asp].j3 ] = aspectColors[cube][asp][ xx[asp].j2 ];
		temp[asp][ xx[asp].j0 ] = aspectColors[cube][asp][ xx[asp].j3 ];
		//面1到2
		temp[ xx[asp].f2 ][ xx[asp].b0 ] = aspectColors[cube][ xx[asp].f1 ][ xx[asp].a0 ];
		temp[ xx[asp].f2 ][ xx[asp].b1 ] = aspectColors[cube][ xx[asp].f1 ][ xx[asp].a1 ];
		temp[ xx[asp].f2 ][ xx[asp].b2 ] = aspectColors[cube][ xx[asp].f1 ][ xx[asp].a2 ];
		//面2到3
		temp[ xx[asp].f3 ][ xx[asp].c0 ] = aspectColors[cube][ xx[asp].f2 ][ xx[asp].b0 ];
		temp[ xx[asp].f3 ][ xx[asp].c1 ] = aspectColors[cube][ xx[asp].f2 ][ xx[asp].b1 ];
		temp[ xx[asp].f3 ][ xx[asp].c2 ] = aspectColors[cube][ xx[asp].f2 ][ xx[asp].b2 ];
		//面3到4
		temp[ xx[asp].f4 ][ xx[asp].d0 ] = aspectColors[cube][ xx[asp].f3 ][ xx[asp].c0 ];
		temp[ xx[asp].f4 ][ xx[asp].d1 ] = aspectColors[cube][ xx[asp].f3 ][ xx[asp].c1 ];
		temp[ xx[asp].f4 ][ xx[asp].d2 ] = aspectColors[cube][ xx[asp].f3 ][ xx[asp].c2 ];
		//面4到1
		temp[ xx[asp].f1 ][ xx[asp].a0 ] = aspectColors[cube][ xx[asp].f4 ][ xx[asp].d0 ];
		temp[ xx[asp].f1 ][ xx[asp].a1 ] = aspectColors[cube][ xx[asp].f4 ][ xx[asp].d1 ];
		temp[ xx[asp].f1 ][ xx[asp].a2 ] = aspectColors[cube][ xx[asp].f4 ][ xx[asp].d2 ];

		//
		memcpy( &aspectColors[cube], temp, 6*9*sizeof(Color) );
	}
}

void CCubeShow::InitCube( int cube )              //memcpy函数的功能是从源src所指的内存地址的起始位置开始拷贝n个字节到目标dst所指的内存地址的起始位置中。
{
	memcpy( &aspectColors[cube], aspectColor_org, 6*9*sizeof(Color) );
}

void CCubeShow::CopyCube( int dstCube, int srcCube )
{
	memcpy( &aspectColors[dstCube][0][0], &aspectColors[srcCube][0][0], 6*9*sizeof(Color) );
}
