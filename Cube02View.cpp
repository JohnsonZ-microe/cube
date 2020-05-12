// Cube02View.cpp : implementation of the CCube02View class
//

#include "stdafx.h"
#include "Cube02.h"                                      /* 载入CCube02App头文件 */

#include "Cube02Doc.h"                                   /* 载入CCube02Doc头文件 */
#include "Cube02View.h"                                  /* 载入CCube02View头文件 */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int resCount = 0;
CString resValue[128];

/////////////////////////////////////////////////////////////////////////////
// CCube02View

IMPLEMENT_DYNCREATE(CCube02View, CView)                          // CCube02View 类在运行时自动由 CView 类生成。CCube02View 类派生而来 //

BEGIN_MESSAGE_MAP(CCube02View, CView)                            //窗口初始化
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

//对于魔方而言，六个面有六种颜色
// U(Up)     上   黄色
// D(Down)   下   白色
// L(Left)   左   橙色
// R(Right)  右   红色
// F(Front)  前   蓝色
// B(Back)   后   绿色

//我的输入
//对打乱的魔方，现将六个面的中心按上下前后左右对好后
//再按“UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR”位置顺序
//输入打乱的魔方所在位置的颜色所对应的方向，每个位置的方向顺序也是颜色顺序
char myInput[][8] = {
    "RU",  "LF",  "UB",  "DR",
    "DL",  "BL",  "UL",  "FU",
    "BD",  "RF",  "BR",  "FD",
    "LDF", "LBD", "FUL", "RFD",
    "UFR", "RDB", "UBL", "RBU"
};

//归位魔方输入
char aaInput[][8] = {
    "UF",  "UR",  "UB",  "UL",
    "DF",  "DR",  "DB",  "DL",
    "FR",  "FL",  "BR",  "BL", 
    "UFR", "URB", "UBL", "ULF",
    "DRF", "DFL", "DLB", "DBR"
};

// 1：对棱块，注意到前8个先输的是上下两面，对有上下面的块，当ud在ud面朝向正确，不在则错。对没上下块当bf在下层也正确。
//    第一步到群组u，d，r, l, f2, b2这是正确朝向。
//    对后四个，当ud在fb面朝向正确，不在则错。
//    对没上下块当bf在bf面也正确。转一下，很容易。
// 2：对角块，有5，7和4，6的区分，这里对所有块仅就bf面考虑朝向。
//   （看twist（））UFR UBL DFL DBR为5，7块，DRF DFL DLB DBR为4，6块。
//    对5 7   1代表正向(就朝向要旋转的面）这里是bf面
//            0代表上下正确
//            2代表左右（要旋转面的左右面）即rl面。
//        ud块在第0个正确，第一个为1，第二个为2.
//        那么应先输入ud面，再输入bf面，最后输rl面。
//    对4，6块先输入ud面，再输入rl面，最后输bf面。
//    看输入时连续4个，（第0个和第2个）与（第1个和第3）个不同。输入注意。

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
	//随机数初始化
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
	GetClientRect( rectWnd );	//获得窗体的大小

	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap( pDC, rectWnd.Width(), rectWnd.Height() );
	CBitmap *pOldBmp = MemDC.SelectObject( &MemBitmap );

	MemDC.PatBlt( 0, 0, rectWnd.Width(), rectWnd.Height(), WHITENESS );	     //获得显示窗口

	//////////////
	// 显示魔方
	//////////////
	v_CubeShow.ShowAllCube( &MemDC );	

	//
	double avg = ( v_nTotalCcc == 0 ) ? 0 : ( 1.0 * v_nTotalNnn / v_nTotalCcc );
	CString strTemp;
	strTemp.Format( "错误循环: %d, 错误计数: %d, 总归位次数: %d, 最大步数: %d, 平均步数: %.1f",
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
		//计数+1
		v_nCount++;

		// 临时变量
		CString strTemp;
		
		//处理各步骤
		if(      v_nCount % 12 == 2  )  CubeProcStep1();
		else if( v_nCount % 12 == 4  )  CubeProcStep2();
		else if( v_nCount % 12 == 6  )  CubeProcStep3();
		else if( v_nCount % 12 == 8  )  CubeProcStep4();
		else if( v_nCount % 12 == 10 )  CubeProcStep5();
		else   { }
	}

	//刷新窗口
	Invalidate();
	UpdateWindow();
	
	//调用基类OnTimer函数
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

	//原始魔方
	v_CubeShow.InitCube( 0 );

	//打乱魔方
	int num = 1 + rand() % 1000;			//随机转动1..1000次
	for( int i=0; i<num; i++ )
	{
		Aspect f = asptab[ rand() % 6 ];	//每次随机选择1个面
		int s = 1 + rand() % 3;				//每次随机选择1个转动角度

		v_CubeShow.rotate( 0, f,  s );
	}

	//SetCubeColor( 0, myInput );

	//为下一步变换准备魔方1，2，3
	v_CubeShow.CopyCube( 1, 0 );               //将状态0的魔方信息拷贝到状态1的魔方中
	v_CubeShow.CopyCube( 2, 0 );
	v_CubeShow.CopyCube( 3, 0 );

	//清输出缓冲区
	resCount = 0;

	//
	v_nCountNnnn++;
}

void CCube02View::CubeProcStep2()
{
	//取得魔方输入
	CString strInput = v_CubeShow.GetCubeColor( 1 );    //GetCubeColor函数输入魔方状态1,返回记录魔方个面颜色的字符串
	//resValue[resCount++].Format( "输入 = %s", strInput );
	
	//取得计划并打印出来
	int nnn = 0;
	v_CubeCalc.GetPlan( strInput, &v_nTotalNnn, &nnn );  //存两份总步数，nnn用于暂存最大步数，GetPlan还完成了打印魔方还原方案的任务

	v_nTotalCcc++;                                       //归位次数自增
	if( nnn > v_nMaxNnn )
		v_nMaxNnn = nnn;                                 //检验最大步数

	//
	//旋转魔方			
	//执行计划0
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
				// 第1步，在魔方1上操作
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

	
	//为下一步变换准备魔方2，3
	v_CubeShow.CopyCube( 2, 1 );
	v_CubeShow.CopyCube( 3, 1 );

	//
	v_nCountNnnn++;
}

void CCube02View::CubeProcStep3()
{
	//执行计划1
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
				// 第2步，在魔方2上操作
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

	//为下一步变换准备魔方3
	v_CubeShow.CopyCube( 3, 2 );
	
	//
	v_nCountNnnn++;
}

void CCube02View::CubeProcStep4()
{
	//执行计划2
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
				//第3步， 在魔方3上操作
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

	//为下一步变换准备魔方0
	v_CubeShow.CopyCube( 0, 3 );

	//
	v_nCountNnnn++;
}

void CCube02View::CubeProcStep5()
{
	//执行计划3
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
				//第4步， 在魔方0上操作
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
	resValue[resCount++].Format( "输出 = %s", v_CubeShow.GetCubeColor( 0 ) );
	
	if( strTemp.Compare( "UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR") == 0 )
		resValue[resCount++].Format( "[Ok!]" );
	else
		v_nCountError++;

	v_nCountNnnn++;
}
