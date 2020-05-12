// Cube02View.cpp : implementation of the CCube02View class
//

#include "stdafx.h"
#include "Cube02.h"                                      /* ����CCube02Appͷ�ļ� */

#include "Cube02Doc.h"                                   /* ����CCube02Docͷ�ļ� */
#include "Cube02View.h"                                  /* ����CCube02Viewͷ�ļ� */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int resCount = 0;
CString resValue[128];

/////////////////////////////////////////////////////////////////////////////
// CCube02View

IMPLEMENT_DYNCREATE(CCube02View, CView)                          // CCube02View ��������ʱ�Զ��� CView �����ɡ�CCube02View ���������� //

BEGIN_MESSAGE_MAP(CCube02View, CView)                            //���ڳ�ʼ��
	//{{AFX_MSG_MAP(CCube02View)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

//����ħ�����ԣ���������������ɫ
// U(Up)     ��   ��ɫ
// D(Down)   ��   ��ɫ
// L(Left)   ��   ��ɫ
// R(Right)  ��   ��ɫ
// F(Front)  ǰ   ��ɫ
// B(Back)   ��   ��ɫ

//�ҵ�����
//�Դ��ҵ�ħ�����ֽ�����������İ�����ǰ�����ҶԺú�
//�ٰ���UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR��λ��˳��
//������ҵ�ħ������λ�õ���ɫ����Ӧ�ķ���ÿ��λ�õķ���˳��Ҳ����ɫ˳��
char myInput[][8] = {
    "RU",  "LF",  "UB",  "DR",
    "DL",  "BL",  "UL",  "FU",
    "BD",  "RF",  "BR",  "FD",
    "LDF", "LBD", "FUL", "RFD",
    "UFR", "RDB", "UBL", "RBU"
};

//��λħ������
char aaInput[][8] = {
    "UF",  "UR",  "UB",  "UL",
    "DF",  "DR",  "DB",  "DL",
    "FR",  "FL",  "BR",  "BL", 
    "UFR", "URB", "UBL", "ULF",
    "DRF", "DFL", "DLB", "DBR"
};

// 1������飬ע�⵽ǰ8����������������棬����������Ŀ飬��ud��ud�泯����ȷ�����������û���¿鵱bf���²�Ҳ��ȷ��
//    ��һ����Ⱥ��u��d��r, l, f2, b2������ȷ����
//    �Ժ��ĸ�����ud��fb�泯����ȷ���������
//    ��û���¿鵱bf��bf��Ҳ��ȷ��תһ�£������ס�
// 2���Խǿ飬��5��7��4��6�����֣���������п����bf�濼�ǳ���
//   ����twist������UFR UBL DFL DBRΪ5��7�飬DRF DFL DLB DBRΪ4��6�顣
//    ��5 7   1��������(�ͳ���Ҫ��ת���棩������bf��
//            0����������ȷ
//            2�������ң�Ҫ��ת��������棩��rl�档
//        ud���ڵ�0����ȷ����һ��Ϊ1���ڶ���Ϊ2.
//        ��ôӦ������ud�棬������bf�棬�����rl�档
//    ��4��6��������ud�棬������rl�棬�����bf�档
//    ������ʱ����4��������0���͵�2�����루��1���͵�3������ͬ������ע�⡣

char myResult[30][8] = {
	"D1", "B3", "F1", "U3", "B1",
	"L2", "U3", "B2", "D3", "L2",
	"U1", "R1",	"D1", "F2", "D1",
	"L2", "D2", "B2", "D3", "L2",
	"D1", "B2", "U2", "L2",	"D2",
	"B2", "U2", "L2", "B2", "R2"
};

/////////////////////////////////////////////////////////////////////////////
// CCube02View construction/destruction

CCube02View::CCube02View() :
    v_nCount( 0 ), 
	v_nCountCccc( 0 ), v_nCountNnnn( 5 ), v_nCountError( 0 ),
	v_nTotalCcc( 0 ), v_nTotalNnn( 0 ),
	v_nMaxNnn( 0 )	
{
	//�������ʼ��
	srand( (unsigned)time( NULL ) ); 	
}

CCube02View::~CCube02View()
{
}

BOOL CCube02View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCube02View drawing

void CCube02View::OnDraw(CDC* pDC)
{
	CCube02Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CDC MemDC;
	MemDC.CreateCompatibleDC( NULL );

	CRect rectWnd;
	GetClientRect( rectWnd );	//��ô���Ĵ�С

	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap( pDC, rectWnd.Width(), rectWnd.Height() );
	CBitmap *pOldBmp = MemDC.SelectObject( &MemBitmap );

	MemDC.PatBlt( 0, 0, rectWnd.Width(), rectWnd.Height(), WHITENESS );	     //�����ʾ����

	//////////////
	// ��ʾħ��
	//////////////
	v_CubeShow.ShowAllCube( &MemDC );	

	//
	double avg = ( v_nTotalCcc == 0 ) ? 0 : ( 1.0 * v_nTotalNnn / v_nTotalCcc );
	CString strTemp;
	strTemp.Format( "����ѭ��: %d, �������: %d, �ܹ�λ����: %d, �����: %d, ƽ������: %.1f",
		v_nCountCccc, v_nCountError, v_nTotalCcc, v_nMaxNnn, avg );
	MemDC.TextOut( 10, 19*meshLen, strTemp );

	for( int line = 0; line < resCount; line++ )
		MemDC.TextOut( 10, 19*meshLen+16*line+20, resValue[line] );
	
	//f[f]
	pDC->BitBlt( 0, 0, rectWnd.Width(), rectWnd.Height(), &MemDC, 0, 0, SRCCOPY );

	//
	MemDC.SelectObject( pOldBmp );
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

/////////////////////////////////////////////////////////////////////////////
// CCube02View printing

BOOL CCube02View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCube02View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCube02View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCube02View diagnostics

#ifdef _DEBUG
void CCube02View::AssertValid() const
{
	CView::AssertValid();
}

void CCube02View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCube02Doc* CCube02View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCube02Doc)));
	return (CCube02Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCube02View message handlers

const Aspect asptab[] = { FRONT, BACK, LEFT, RIGHT, UP, DOWN, NOASP };

void CCube02View::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent==1 )
	{
		//����+1
		v_nCount++;

		// ��ʱ����
		CString strTemp;
		
		//���������
		if(      v_nCount % 12 == 2  )  CubeProcStep1();
		else if( v_nCount % 12 == 4  )  CubeProcStep2();
		else if( v_nCount % 12 == 6  )  CubeProcStep3();
		else if( v_nCount % 12 == 8  )  CubeProcStep4();
		else if( v_nCount % 12 == 10 )  CubeProcStep5();
		else   { }
	}

	//ˢ�´���
	Invalidate();
	UpdateWindow();
	
	//���û���OnTimer����
	CView::OnTimer(nIDEvent);
}

int CCube02View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTimer( 1, 200, NULL );

	return 0;
}

BOOL CCube02View::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}

	void CCube02View::CubeProcStep1()
{
	//
	if( v_nCountNnnn != 5 )
		v_nCountCccc++;
	v_nCountNnnn = 0;

	//ԭʼħ��
	v_CubeShow.InitCube( 0 );

	//����ħ��
	int num = 1 + rand() % 1000;			//���ת��1..1000��
	for( int i=0; i<num; i++ )
	{
		Aspect f = asptab[ rand() % 6 ];	//ÿ�����ѡ��1����
		int s = 1 + rand() % 3;				//ÿ�����ѡ��1��ת���Ƕ�

		v_CubeShow.rotate( 0, f,  s );
	}

	//SetCubeColor( 0, myInput );

	//Ϊ��һ���任׼��ħ��1��2��3
	v_CubeShow.CopyCube( 1, 0 );               //��״̬0��ħ����Ϣ������״̬1��ħ����
	v_CubeShow.CopyCube( 2, 0 );
	v_CubeShow.CopyCube( 3, 0 );

	//�����������
	resCount = 0;

	//
	v_nCountNnnn++;
}

void CCube02View::CubeProcStep2()
{
	//ȡ��ħ������
	CString strInput = v_CubeShow.GetCubeColor( 1 );    //GetCubeColor��������ħ��״̬1,���ؼ�¼ħ��������ɫ���ַ���
	//resValue[resCount++].Format( "���� = %s", strInput );
	
	//ȡ�üƻ�����ӡ����
	int nnn = 0;
	v_CubeCalc.GetPlan( strInput, &v_nTotalNnn, &nnn );  //�������ܲ�����nnn�����ݴ��������GetPlan������˴�ӡħ����ԭ����������

	v_nTotalCcc++;                                       //��λ��������
	if( nnn > v_nMaxNnn )
		v_nMaxNnn = nnn;                                 //���������

	//
	//��תħ��			
	//ִ�мƻ�0
	resValue[resCount++] = v_CubeCalc.m_strplan[0];      //
	int n = v_CubeCalc.m_strplan[0].Find( ']' );
	if( n >= 0 )
	{
		int k = n+1;
		while( v_CubeCalc.m_strplan[0][k] == ' ' ) k++;

		CString strTemp;
		strTemp = v_CubeCalc.m_strplan[0].Mid( k );

		while( strTemp.GetLength() >= 2 )
		{
			//TRACE( "\n%c%c", strTemp[0], strTemp[1] );
			
			CString strasp = "FBLRUD";
			int ff = strasp.Find( strTemp[0] );
			if( ff >= 0 )
			{
				// ��1������ħ��1�ϲ���
				v_CubeShow.rotate( 1, asptab[ ff ], ( strTemp[1] - '0') );
			}

			int r = strTemp.Find( ' ' );
			if( r >= 0 )
			{
				k = r+1;
				while( strTemp[k] == ' ' ) k++;
				strTemp = strTemp.Mid( k );
			}
			else
			{
				strTemp.Empty();
			}
		}
	}

	
	//Ϊ��һ���任׼��ħ��2��3
	v_CubeShow.CopyCube( 2, 1 );
	v_CubeShow.CopyCube( 3, 1 );

	//
	v_nCountNnnn++;
}

void CCube02View::CubeProcStep3()
{
	//ִ�мƻ�1
	resValue[resCount++] = v_CubeCalc.m_strplan[1];

	int n = v_CubeCalc.m_strplan[1].Find( ']' );
	if( n >= 0 )
	{
		int k = n+1;
		while( v_CubeCalc.m_strplan[1][k] == ' ' ) k++;

		CString strTemp;
		strTemp = v_CubeCalc.m_strplan[1].Mid( k );

		while( strTemp.GetLength() >= 2 )
		{
			CString strasp = "FBLRUD";
			int ff = strasp.Find( strTemp[0] );
			if( ff >= 0 )
			{
				// ��2������ħ��2�ϲ���
				v_CubeShow.rotate( 2, asptab[ ff ], ( strTemp[1] - '0') );
			}

			int r = strTemp.Find( ' ' );
			if( r >= 0 )
			{
				k = r+1;
				while( strTemp[k] == ' ' ) k++;
				strTemp = strTemp.Mid( k );
			}
			else
			{
				strTemp.Empty();
			}
		}
	}

	//Ϊ��һ���任׼��ħ��3
	v_CubeShow.CopyCube( 3, 2 );
	
	//
	v_nCountNnnn++;
}

void CCube02View::CubeProcStep4()
{
	//ִ�мƻ�2
	resValue[resCount++] = v_CubeCalc.m_strplan[2];

	int n = v_CubeCalc.m_strplan[2].Find( ']' );
	if( n >= 0 )
	{
		int k = n+1;
		while( v_CubeCalc.m_strplan[2][k] == ' ' ) k++;

		CString strTemp;
		strTemp = v_CubeCalc.m_strplan[2].Mid( k );

		while( strTemp.GetLength() >= 2 )
		{					
			CString strasp = "FBLRUD";
			int ff = strasp.Find( strTemp[0] );
			if( ff >= 0 )
			{
				//��3���� ��ħ��3�ϲ���
				v_CubeShow.rotate( 3, asptab[ ff ], ( strTemp[1] - '0') );
			}

			int r = strTemp.Find( ' ' );
			if( r >= 0 )
			{
				k = r+1;
				while( strTemp[k] == ' ' ) k++;
				strTemp = strTemp.Mid( k );
			}
			else
			{
				strTemp.Empty();
			}
		}
	}

	//Ϊ��һ���任׼��ħ��0
	v_CubeShow.CopyCube( 0, 3 );

	//
	v_nCountNnnn++;
}

void CCube02View::CubeProcStep5()
{
	//ִ�мƻ�3
	resValue[resCount++] = v_CubeCalc.m_strplan[3];

	CString strTemp;
	int n = v_CubeCalc.m_strplan[3].Find( ']' );
	if( n >= 0 )
	{
		int k = n+1;
		while( v_CubeCalc.m_strplan[3][k] == ' ' ) k++;

		strTemp = v_CubeCalc.m_strplan[3].Mid( k );

		while( strTemp.GetLength() >= 2 )
		{
			CString strasp = "FBLRUD";
			int ff = strasp.Find( strTemp[0] );
			if( ff >= 0 )
			{
				//��4���� ��ħ��0�ϲ���
				v_CubeShow.rotate( 0, asptab[ ff ], ( strTemp[1] - '0') );
			}

			int r = strTemp.Find( ' ' );
			if( r >= 0 )
			{
				k = r+1;
				while( strTemp[k] == ' ' ) k++;
				strTemp = strTemp.Mid( k );
			}
			else
			{
				strTemp.Empty();
			}
		}
	}

	//
	strTemp = v_CubeShow.GetCubeColor( 0 );
	resValue[resCount++].Format( "��� = %s", v_CubeShow.GetCubeColor( 0 ) );
	
	if( strTemp.Compare( "UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR") == 0 )
		resValue[resCount++].Format( "[Ok!]" );
	else
		v_nCountError++;

	v_nCountNnnn++;
}
