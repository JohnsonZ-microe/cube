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

const Color aspectColor_org[ 6 ][ 9 ] = {                                   //�����ʼħ��״̬
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

const char colfaces[] = "FBLRUD";                                          //ħ������˳���ַ���
const Color colTable[6] = { BLUE, GREEN, ORANGE, RED, YELLOW, WHITE };     //��ɫ˳���ַ���

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
	brushs[FRONT].CreateSolidBrush( RGB(0,    0,255));	// ��ɫ
	brushs[BACK ].CreateSolidBrush( RGB(0,  255,  0));	// ��ɫ	
	brushs[LEFT ].CreateSolidBrush( RGB(255,132,  0));	// ��ɫ
	brushs[RIGHT].CreateSolidBrush( RGB(255,  0,  0));	// ��ɫ
	brushs[UP   ].CreateSolidBrush( RGB(255,255,  0));	// ��ɫ
	brushs[DOWN ].CreateSolidBrush( RGB(255,255,255));	// ��ɫ
	brushs[NOASP].CreateSolidBrush( RGB(153,153,153));	// ��ɫ

	//
	for( int iCube=0; iCube<4; iCube++ )
		InitCube( iCube );                       //��ʼ���ĸ�ħ�����Ա���ڴ�����
	
	//
	CPoint *org[] = { (CPoint *)&org1, (CPoint *)&org2, (CPoint *)&org3, (CPoint *)&org4 };

	//vertexes for every aspect
	//����ħ��ͼ��
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
		CPen pen(PS_SOLID,1,BLACK_PEN);                  //CPen�໭�ʺ������﷨��pen��������ʽ�����ʿ�ȣ�������ɫ��
		CPen *pOldPen = pDC->SelectObject( &pen );
		for (int i=0;i<6;i++)
		{
			for (int asp=0;asp<9;asp++)
			{
				pDC->SelectObject( &brushs[ aspectColors[iCube][i][asp]] );
				pDC->Polygon( vertexes[iCube][i][asp], 4 ); //���ĸ�ħ��ͼȫ���������ϻ��

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
	//��ɫ
	//  �����棬�Ӻ�ǰ�������ң����0..8
	//  �����棬�Ӻ�ǰ�����µ��ϣ����0..8
	//  ǰ���棬���µ��ϣ������ң����0..8

	//UFλ����ɫ
	aspectColors[idx][UP   ][7] = colTable[ strchr( colfaces, input[0][0] ) - colfaces ];
	aspectColors[idx][FRONT][7] = colTable[ strchr( colfaces, input[0][1] ) - colfaces ];
	//URλ����ɫ
	aspectColors[idx][UP   ][5] = colTable[ strchr( colfaces, input[1][0] ) - colfaces ];
	aspectColors[idx][RIGHT][5] = colTable[ strchr( colfaces, input[1][1] ) - colfaces ];
	//UBλ����ɫ
	aspectColors[idx][UP   ][1] = colTable[ strchr( colfaces, input[2][0] ) - colfaces ];
	aspectColors[idx][BACK ][7] = colTable[ strchr( colfaces, input[2][1] ) - colfaces ];
	//ULλ����ɫ
	aspectColors[idx][UP   ][3] = colTable[ strchr( colfaces, input[3][0] ) - colfaces ];
	aspectColors[idx][LEFT ][5] = colTable[ strchr( colfaces, input[3][1] ) - colfaces ];

	//DFλ����ɫ
	aspectColors[idx][DOWN ][7] = colTable[ strchr( colfaces, input[4][0] ) - colfaces ];
	aspectColors[idx][FRONT][1] = colTable[ strchr( colfaces, input[4][1] ) - colfaces ];
	//DRλ����ɫ
	aspectColors[idx][DOWN ][5] = colTable[ strchr( colfaces, input[5][0] ) - colfaces ];
	aspectColors[idx][RIGHT][3] = colTable[ strchr( colfaces, input[5][1] ) - colfaces ];
	//DBλ����ɫ
	aspectColors[idx][DOWN ][1] = colTable[ strchr( colfaces, input[6][0] ) - colfaces ];
	aspectColors[idx][BACK ][1] = colTable[ strchr( colfaces, input[6][1] ) - colfaces ];
	//DLλ����ɫ
	aspectColors[idx][DOWN ][3] = colTable[ strchr( colfaces, input[7][0] ) - colfaces ];
	aspectColors[idx][LEFT ][3] = colTable[ strchr( colfaces, input[7][1] ) - colfaces ];

	//FRλ����ɫ
	aspectColors[idx][FRONT][5] = colTable[ strchr( colfaces, input[8][0] ) - colfaces ];
	aspectColors[idx][RIGHT][7] = colTable[ strchr( colfaces, input[8][1] ) - colfaces ];
	//FLλ����ɫ
	aspectColors[idx][FRONT][3] = colTable[ strchr( colfaces, input[9][0] ) - colfaces ];
	aspectColors[idx][LEFT ][7] = colTable[ strchr( colfaces, input[9][1] ) - colfaces ];
	//BRλ����ɫ
	aspectColors[idx][BACK ][5] = colTable[ strchr( colfaces, input[10][0] ) - colfaces ];
	aspectColors[idx][RIGHT][1] = colTable[ strchr( colfaces, input[10][1] ) - colfaces ];
	//BLλ����ɫ
	aspectColors[idx][BACK ][3] = colTable[ strchr( colfaces, input[11][0] ) - colfaces ];
	aspectColors[idx][LEFT ][1] = colTable[ strchr( colfaces, input[11][1] ) - colfaces ];

	//UFRλ����ɫ
	aspectColors[idx][UP   ][8] = colTable[ strchr( colfaces, input[12][0] ) - colfaces ];
	aspectColors[idx][FRONT][8] = colTable[ strchr( colfaces, input[12][1] ) - colfaces ];
	aspectColors[idx][RIGHT][8] = colTable[ strchr( colfaces, input[12][2] ) - colfaces ];
	//URBλ����ɫ
	aspectColors[idx][UP   ][2] = colTable[ strchr( colfaces, input[13][0] ) - colfaces ];
	aspectColors[idx][RIGHT][2] = colTable[ strchr( colfaces, input[13][1] ) - colfaces ];
	aspectColors[idx][BACK ][8] = colTable[ strchr( colfaces, input[13][2] ) - colfaces ];
	//UBLλ����ɫ
	aspectColors[idx][UP   ][0] = colTable[ strchr( colfaces, input[14][0] ) - colfaces ];
	aspectColors[idx][BACK ][6] = colTable[ strchr( colfaces, input[14][1] ) - colfaces ];
	aspectColors[idx][LEFT ][2] = colTable[ strchr( colfaces, input[14][2] ) - colfaces ];
	//ULFλ����ɫ
	aspectColors[idx][UP   ][6] = colTable[ strchr( colfaces, input[15][0] ) - colfaces ];
	aspectColors[idx][LEFT ][8] = colTable[ strchr( colfaces, input[15][1] ) - colfaces ];
	aspectColors[idx][FRONT][6] = colTable[ strchr( colfaces, input[15][2] ) - colfaces ];

	//DRFλ����ɫ
	aspectColors[idx][DOWN ][8] = colTable[ strchr( colfaces, input[16][0] ) - colfaces ];
	aspectColors[idx][RIGHT][6] = colTable[ strchr( colfaces, input[16][1] ) - colfaces ];
	aspectColors[idx][FRONT][2] = colTable[ strchr( colfaces, input[16][2] ) - colfaces ];
	//DFLλ����ɫ
	aspectColors[idx][DOWN ][6] = colTable[ strchr( colfaces, input[17][0] ) - colfaces ];
	aspectColors[idx][FRONT][0] = colTable[ strchr( colfaces, input[17][1] ) - colfaces ];
	aspectColors[idx][LEFT ][6] = colTable[ strchr( colfaces, input[17][2] ) - colfaces ];
	//DLBλ����ɫ
	aspectColors[idx][DOWN ][0] = colTable[ strchr( colfaces, input[18][0] ) - colfaces ];
	aspectColors[idx][LEFT ][0] = colTable[ strchr( colfaces, input[18][1] ) - colfaces ];
	aspectColors[idx][BACK ][0] = colTable[ strchr( colfaces, input[18][2] ) - colfaces ];
	//DBRλ����ɫ
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

	//UFλ����ɫ
	strTemp += colortab[ aspectColors[idx][UP   ][7] ];
	strTemp += colortab[ aspectColors[idx][FRONT][7] ];
	//URλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][5] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][5] ];
	//UBλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][1] ];
	strTemp += colortab[ aspectColors[idx][BACK ][7] ];
	//ULλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][3] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][5] ];

	//DFλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][7] ];
	strTemp += colortab[ aspectColors[idx][FRONT][1] ];
	//DRλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][5] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][3] ];
	//DBλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][1] ];
	strTemp += colortab[ aspectColors[idx][BACK ][1] ];
	//DLλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][3] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][3] ];

	//FRλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][FRONT][5] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][7] ];
	//FLλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][FRONT][3] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][7] ];
	//BRλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][BACK ][5] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][1] ];
	//BLλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][BACK ][3] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][1] ];
	
	//UFRλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][8] ];
	strTemp += colortab[ aspectColors[idx][FRONT][8] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][8] ];
	//URBλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][2] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][2] ];
	strTemp += colortab[ aspectColors[idx][BACK ][8] ];	
	//UBLλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][0] ];
	strTemp += colortab[ aspectColors[idx][BACK ][6] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][2] ];	
	//ULFλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][UP   ][6] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][8] ];
	strTemp += colortab[ aspectColors[idx][FRONT][6] ];
	
	//DRFλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][8] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][6] ];
	strTemp += colortab[ aspectColors[idx][FRONT][2] ];
	//DFLλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][6] ];
	strTemp += colortab[ aspectColors[idx][FRONT][0] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][6] ];
	strTemp += " ";
	//DLBλ����ɫ
	strTemp += colortab[ aspectColors[idx][DOWN ][0] ];
	strTemp += colortab[ aspectColors[idx][LEFT ][0] ];
	strTemp += colortab[ aspectColors[idx][BACK ][0] ];
	
	//DBRλ����ɫ
	strTemp += " ";
	strTemp += colortab[ aspectColors[idx][DOWN ][2] ];
	strTemp += colortab[ aspectColors[idx][BACK ][2] ];
	strTemp += colortab[ aspectColors[idx][RIGHT][0] ];

	//
	return strTemp;
}

//
// ת1��2��3 �Σ��ֱ��Ӧ ��ת����ת�ͷ�ת
// ��ת�� ��ת ��ת ��ת  �ڲ���ѭ��       �ڲ���ѭ��       �����ѭ��(���¡�ǰ�����Ҿ��෴)
//   ǰ    1   2    3      1��3��7��5��1   0��6��8��2��0    ��8,7,6����8,7,6����6,7,8����6,7,8����8,7,6
//   ��    1   2    3      1��5��7��3��1   0��2��8��6��0    ��2,1,0����0,1,2����0,1,2����2,1,0����2,1,0
//   ��    1   2    3      1��5��7��3��1   0��2��8��6��0    ��0,3,6��ǰ6,3,0����0,3,6����6,3,0����0,3,6
//   ��    1   2    3      1��3��7��5��1   0��6��8��2��0    ��2,5,8����2,5,8����8,5,2��ǰ8,5,2����2,5,8
//   ��    1   2    3      1��5��7��3��1   0��2��8��6��0    ǰ6,7,8����2,5,8����8,7,6����8,5,2��ǰ6,7,8
//   ��    1   2    3      1��3��7��5��1   0��6��8��2��0    ǰ0,1,2����0,3,6����2,1,0����6,3,0��ǰ0,1,2

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

//pΪ��ת����
void CCubeShow::rotate( int cube, Aspect asp, int p )
{
	Color temp[6][9];
	
	for( int i=0; i<p; i++ )
	{
		memcpy( temp, &aspectColors[cube], 6*9*sizeof(Color) );

		if( asp != xx[asp].f0 )
			continue;		

		//��
		temp[asp][ xx[asp].l1 ] = aspectColors[cube][asp][ xx[asp].l0 ];
		temp[asp][ xx[asp].l2 ] = aspectColors[cube][asp][ xx[asp].l1 ];
		temp[asp][ xx[asp].l3 ] = aspectColors[cube][asp][ xx[asp].l2 ];
		temp[asp][ xx[asp].l0 ] = aspectColors[cube][asp][ xx[asp].l3 ];
		//��
		temp[asp][ xx[asp].j1 ] = aspectColors[cube][asp][ xx[asp].j0 ];
		temp[asp][ xx[asp].j2 ] = aspectColors[cube][asp][ xx[asp].j1 ];
		temp[asp][ xx[asp].j3 ] = aspectColors[cube][asp][ xx[asp].j2 ];
		temp[asp][ xx[asp].j0 ] = aspectColors[cube][asp][ xx[asp].j3 ];
		//��1��2
		temp[ xx[asp].f2 ][ xx[asp].b0 ] = aspectColors[cube][ xx[asp].f1 ][ xx[asp].a0 ];
		temp[ xx[asp].f2 ][ xx[asp].b1 ] = aspectColors[cube][ xx[asp].f1 ][ xx[asp].a1 ];
		temp[ xx[asp].f2 ][ xx[asp].b2 ] = aspectColors[cube][ xx[asp].f1 ][ xx[asp].a2 ];
		//��2��3
		temp[ xx[asp].f3 ][ xx[asp].c0 ] = aspectColors[cube][ xx[asp].f2 ][ xx[asp].b0 ];
		temp[ xx[asp].f3 ][ xx[asp].c1 ] = aspectColors[cube][ xx[asp].f2 ][ xx[asp].b1 ];
		temp[ xx[asp].f3 ][ xx[asp].c2 ] = aspectColors[cube][ xx[asp].f2 ][ xx[asp].b2 ];
		//��3��4
		temp[ xx[asp].f4 ][ xx[asp].d0 ] = aspectColors[cube][ xx[asp].f3 ][ xx[asp].c0 ];
		temp[ xx[asp].f4 ][ xx[asp].d1 ] = aspectColors[cube][ xx[asp].f3 ][ xx[asp].c1 ];
		temp[ xx[asp].f4 ][ xx[asp].d2 ] = aspectColors[cube][ xx[asp].f3 ][ xx[asp].c2 ];
		//��4��1
		temp[ xx[asp].f1 ][ xx[asp].a0 ] = aspectColors[cube][ xx[asp].f4 ][ xx[asp].d0 ];
		temp[ xx[asp].f1 ][ xx[asp].a1 ] = aspectColors[cube][ xx[asp].f4 ][ xx[asp].d1 ];
		temp[ xx[asp].f1 ][ xx[asp].a2 ] = aspectColors[cube][ xx[asp].f4 ][ xx[asp].d2 ];

		//
		memcpy( &aspectColors[cube], temp, 6*9*sizeof(Color) );
	}
}

void CCubeShow::InitCube( int cube )              //memcpy�����Ĺ����Ǵ�Դsrc��ָ���ڴ��ַ����ʼλ�ÿ�ʼ����n���ֽڵ�Ŀ��dst��ָ���ڴ��ַ����ʼλ���С�
{
	memcpy( &aspectColors[cube], aspectColor_org, 6*9*sizeof(Color) );
}

void CCubeShow::CopyCube( int dstCube, int srcCube )
{
	memcpy( &aspectColors[dstCube][0][0], &aspectColors[srcCube][0][0], 6*9*sizeof(Color) );
}
