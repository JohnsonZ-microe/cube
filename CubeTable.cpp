// CubeTable.cpp: implementation of the CCubeTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CubeTable.h"

#define CHAROFFSET	65

//各个位置所含方块朝向总数 角3 棱2
//前12个为棱, 后8个为角
const int val[ 20 ] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3 };

//Each move consists of two 4-cycles. This string contains these in FBRLUD order.
//以八个为一组分别代表FBRLUD面, 以此来为旋转面上各位置的方向、所含块提供准确变化方式
const char *CCubeTable::perm = "AIBJTMROCLDKSNQPEKFIMSPRGJHLNTOQAGCEMTNSBFDHORPQ";

// number 65='A' is often subtracted to convert char ABC... to number 0,1,2,...
const char *CCubeTable::corn = "QRSTQRTSQSRTQTRSQSTRQTSR";

const int numLayor[8] = { 2, 9, 9, 7, 6, 13, 13, 9 };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCubeTable::CCubeTable()
{
	tables[0] = table0; tablesize[0] = 1;
	tables[1] = table1; tablesize[1] = 4096;
	tables[2] = table2; tablesize[2] = 6561;
	tables[3] = table3; tablesize[3] = 4096;
	tables[4] = table4; tablesize[4] = 256;
	tables[5] = table5; tablesize[5] = 1536;
	tables[6] = table6; tablesize[6] = 13824;
	tables[7] = table7; tablesize[7] = 576;

	//取得执行程序EXE文件所在路径
	CString strPath;
	LPTSTR pFileName = strPath.GetBuffer( MAX_PATH+4 );
	GetModuleFileName( 0, pFileName, MAX_PATH );
	strPath.ReleaseBuffer();
	strPath = strPath.Left( strPath.ReverseFind( '\\' ) );

	//生成、输出各个表
	for( int iTab=0; iTab<8; iTab++ )
	{
		filltable( iTab );

		CString strFileName;		
		strFileName.Format( "%s\\table%d.txt", strPath, iTab );

		CStdioFile file;
		if( file.Open( strFileName, CFile::modeCreate | CFile::modeWrite ) )
		{
			CString strTemp;
			strTemp.Format( "// table%d[ %d ]", iTab, tablesize[iTab] / 8 + ((tablesize[iTab]%8==0) ? 0: 1) );
			file.WriteString( strTemp );

			strTemp.Format( "\nconst unsigned long table%d[] = {", iTab );
			file.WriteString( strTemp );

			unsigned long data = 0, temp = 0;
			int iCount = 0;

			for( int idx=0; idx<tablesize[iTab]; idx++ )
			{
				if( ( idx%8 == 0) && (iCount%100 == 0) )
				{
					strTemp.Format( "\n    // table%d -- %d(00..99)", iTab, iCount/100 );
					file.WriteString( strTemp );
				}

				temp = tables[iTab][idx] % 16;
				data = ( idx % 8 == 0 ) ? temp : (data + (temp << (idx%8*4)));

				if( idx % 8 == 7 )
				{
					

					file.WriteString( ( iCount%10==0 ) ? "\n    " : " " );
					
					strTemp.Format( "0X%08X,", data );
					file.WriteString( strTemp );

					iCount++;
				}
			}

			strTemp.Format( "%s0X%08X,\n};", (iCount%10==0) ? "\n    " : " ", data );
			file.WriteString( strTemp );
			
			file.Close();
		}
	}
}

CCubeTable::~CCubeTable()
{

}

///////////////////////////////
//交换数组中两个变量的值
#define SWAP( ppp, aaa, bbb )	{ temp = ppp[aaa]; ppp[aaa] = ppp[bbb]; ppp[bbb] = temp; }

/////////////////////////////////////
// set cube to solved position
void CCubeTable::reset()
{
	for( int i=0; i<20; i++ )
		v_pos[i] = i, v_ori[i] = 0;
}

// Cycles 4 pieces in array p, the piece indices given by a[0..3].
// 旋转*p中的4个块, 4个块位置由a【0，1，2，3】决定
void CCubeTable::cycle( int *p, char *a )
{	
	SWAP( p, a[0] - CHAROFFSET, a[1] - CHAROFFSET );
	SWAP( p, a[0] - CHAROFFSET, a[2] - CHAROFFSET );
	SWAP( p, a[0] - CHAROFFSET, a[3] - CHAROFFSET );
}

// twists i-th piece a+1 times.
void CCubeTable::twist( int i, int a )
{
	int j = i - CHAROFFSET;
	v_ori[j] = (v_ori[j]+a+1) % val[j];
}

////////////////////////////////////////////
//转动
//m从0到5 分别代表fbrlud面的旋转 顺时针
//do a clockwise quarter turn cube move
//输入：m表示面，FBRLUD序，0F 1B 2R 3L 4U 5D
void CCubeTable::domove( int m )
{    
    int i = 8;
	char *p = (char *)perm + 8*m;
    
    //旋转“棱”
    cycle( v_pos, p );  //旋转一面上四个棱块, 当前块得到前一块的方块值
    cycle( v_ori, p );  //旋转一面上四个棱块, 当前块得到前一方块的朝向

    //旋转“角”
    cycle( v_pos, p+4 );  //旋转一面上四个角块, 当前块得到前一块的方块值
    cycle( v_ori, p+4 );  //旋转一面上四个角块, 当前块得到前一方块的朝向

    //twist corners if RLFB
    //旋转角块朝向如果是RLFB面转动
    if( m<4 )
    {
		//  twist(p[7],1)
		//  twist(p[6],0)
		//  twist(p[5],1)
		//  twist(p[4],0)
		//for(;--i>3;) 
		for( i=7; i>=4; i-- )
		{
			//document.write( "<br> i = " + i );
			twist( p[i], i&1 );
		}

		//注意到*perm中前四组即（32个代表fbrl旋转的字符）每组的后四个角块都是按由该面左上角逆时针的顺序
		//对P【5】和p【7】 该块方向值=(该块方向值（即前一块刚才的方向值）+2）%3
		//  对5 7   1 代表正向(就朝向要旋转的面） 0代表上下 2代表左右（要旋转面的左右面）
		//  现在	之后
		//  0	2 原来前一块的上下面变为现在面的左右面
		//  1	0 原来前一块的左右面变为现在面的上下面
		//  2	1 原来前一块的正面仍是正面
		//  对4 6        该块方向值=(该块方向值（即前一块刚才的方向值）+1）%3
		//  现在	之后
		//  0	1
		//  1	2
		//  2	0 一样的分析
		//  有 0代表朝上下只有1，2在5，7和4，6间不同这是因为对当前面的4，6为魔方整体向左向右旋转90度面的5，7位
		//   同样的方向值有不同的意义, 即 对每一面旋转近考虑当前面 旋转后对从另一面看仍然符合旋转后的结果
    }

    //flip edges if FB
    //旋转棱块朝向如果是FB面转动
    if( m<2 )
    {
		//  twist(p[3],0)
		//  twist(p[2],0)
		//  twist(p[1],0)
		//  twist(p[0],0)
		//for(i=4;i--;) 
		for( i=3; i>=0; i-- )
		{
			//document.write( "<br> i = " + i );
			twist( perm[8*m+i], 0 );
		}
		//这个好理解写由0到1，或由1变0
    }
}

////////////////////////////////
// convert number in range 0..23 to permutation of 4 chars.
void CCubeTable::numtoperm( int *p, int n, int ofp )
{
	p[ofp+3] = ofp;

    //for( var a=3; a--; )
    for( int a=2; a>=0; a-- )
    {
		//document.write( "<br> a = " + a );

		p[ofp+a] = n % (4-a) + ofp;  // n%（4-a)只能提供0～3还要加上所处位置  此时为比后面大的数
   
		n /= (4-a);

		//for( var b=a; ++b<4; )
		for( int b=a+1; b<4; b++ )
		{
			//document.write( "<br> a = " + a + ", b = " + b );
			if( p[ofp+b] >= p[ofp+a] )
				p[ofp+b]++;  //比前面大加几个再加上等于自己多一个
		}
	}
}

//////////////////////////////////////////////////////////////////
// convert permutation of 4 chars to a number in range 0..23
int CCubeTable::permtonum( int *p, int ofp )
{
	int n = 0;
	
	for( int a=0; a<4; a++ )
    {
		n *= (4-a);

		for( int b=a+1; b<4; b++ )
		{
			//document.write( "<br> a = " + a + ", b = " + b );
			if( p[ofp+b]  < p[ofp+a] )
				n++;
		}
    }

    return n;

    // 举个例子 3 2 4 1 （2*3+1）*2+1=15
    // 用字符（a*3+b)*2+c=6*a+2b+c 符合排列知识
    // 也可联系进制 或从前往后、从后往前 每个数都会一直乘到（后面数个数）的阶乘
}

///////////////////////////////////////////////////////////
// sets cube to any position which has index n in table t
void CCubeTable::setposition( int t, int n )
{
    int i=0;
    reset();

    switch( t )
    {
    // case 0 does nothing so leaves cube solved
    case 1: //edgeflip
		for( i=0; i<12; i++ )
		{
			v_ori[i] = n & 1;
			n >>= 1;
		}
		break;

    case 2: //cornertwist
		for( i=12; i<20; i++ )
		{
			v_ori[i] = n % 3;
			n /= 3;
		}
		break;

    case 3: //middle edge choice
		for( i=0; i<12; i++ )
		{
			v_pos[i] = 8 * n & 8;
			n >>= 1;
		}
		break;

    case 4: //ud slice choice
		for( i=0; i<8; i++ )
		{
			v_pos[i] = 4 * n & 4;
			n >>= 1;
		}
		break;

    case 5: { //tetrad choice,parity,twist
		int k = n%6 * 4, j = 12;
		n /= 6;
		for( i=0; i<8; i++ )
		{
			v_pos[i+12] = (n&1) ? corn[k++]-CHAROFFSET : j++;
			n >>= 1;
		}
		}break;

    case 6: //slice permutations
      numtoperm( v_pos, n%24, 12 ); n /= 24;
      numtoperm( v_pos, n%24, 4  ); n /= 24;
      numtoperm( v_pos, n,    0  );
      break;

    case 7://corner permutations
	  numtoperm( v_pos, n/24, 8  );
	  numtoperm( v_pos, n%24, 16 );
      break;
    }
}

//////////////////////////////
// 得到当前魔方状态在当前阶段表格中的序列
// get index of cube position from table t
int CCubeTable::getposition( int t )
{
    int i=-1, n=0;

    switch( t )
    {
    // case 0 does nothing so returns 0
    //当t=0 返回0

    // 对棱块i朝向如果朝向不对，就加上1右移i格 0则相当于没效果 即用12个二进制位来表示棱块朝向
    case 1: //edgeflip
		// 12 bits, set bit if edge is flipped      
		//for(;++i<12;)
		for( i=0; i<12; i++ )
		{
			//document.write( "<br> i = " + i );
			n += ( v_ori[i] << i );
		}
		break;

    // 对角块朝向，可认为是8个三进制位
    case 2://cornertwist
		// get base 3 number of 8 digits - each digit is corner twist
		//for(i=20;--i>11;)
		for( i=19; i>=12; i-- )
		{
			//document.write( "<br> i = " + i );
			n = n*3 + v_ori[i];
		}
		break;

    case 3://middle edge choice
		// 12 bits, set bit if edge belongs in Um middle slice
		// 对0～11块当v_pos【i】为8，9，10，11时移动i位 即 用12个二进制位来表示中层棱块位置
		//for(;++i<12;)
		for( i=0; i<12; i++ )
		{
			//document.write( "<br> i = " + i );
			n += (v_pos[i] & 8) ? ( 1<<i ) : 0;
		}
		break;

    case 4://ud slice choice
		// 8 bits, set bit if UD edge belongs in Fm middle slice
		// 层下层切片情况，以上已把中层安好，现在只管uf，df，ub，db与ru，lu，rd，ld各形成两个圈。
		// 这里对ru，rd，lu，ld的块进行标记。
		for( i=0; i<8; i++ )
			n += (v_pos[i] & 4) ? ( 1<<i ) : 0;
		break;

    //四分体情况 仅由各面的180度旋转，每个角块能处的位置形成一个环
    // case4、5完成后魔方的状态仅有各面180度形成，所以各情况间能通过180度转换形成的本质上是一样的
    case 5: { //tetrad choice, twist and parity
		// 该阶段分两部分来得到序列 第一阶段是一个环上各个块所在的位置 第二阶段是对应角块所形成的顺序。
		int j, k, cl, corn1[8] = { 0 }, corn2[4] = { 0 };
		// 8 bits, set bit if corner belongs in second tetrad.
		// also separate pieces for twist/parity determination
		k = j = 0;
		for( i=0; i<8; i++ )
		{
			if( (cl=(v_pos[i+12]-12)) & 4 )
			{
				//corn（4～7）分别表示16~19角块的出现顺序
				corn1[cl] = k++;
				n += 1<<i;
			}
			else
			{
				//corn（0~3) corn（0）表示12～15第一个数-12，corn（1）表示12～15第二个数-12，...
				corn1[j++] = cl;
			}
		}
      
		//Find permutation of second tetrad after solving first
		for( i=0; i<4; i++)
		{
			//document.write( "<br> i = " + i );
			//corn[i]的值是多少，corn2【i】=该值加4在16～19出现的顺序
			corn2[i] = corn1[4+corn1[i]];
		}
        
		//Solve one piece of second tetrad
		//for(;--i;)
		for( i=3; i>0; i--)      
		{
			//document.write( "<br> i = " + i );
			// 转换以得到相对位置 把corn2【0】对应的数顺序当作0（我们自己觉得他最后变为0但并未赋值，看底下分析），对后面重新排序
			corn2[i] ^= corn2[0];
		}

		// encode parity/tetrad twist
		// 对于每一种16～19出现位置的排列，若固定任意值剩下有6种排列
		n = n*6 + corn2[1]*2 - 2;

		//对最后两个数进行排列
		if( ( corn2[3] + 0.5 ) < corn2[2] )  //浮点数表示的整数，如果是"<"则至少差1
			n++;

		// 先分析一下 异或产生后的效果
		//    ^	0	1	2	3
		//    0	0	1	2	3
		//    1	1	0	3	2
		//    2	2	3	0	1
		//    3	3	2	1	0
		//    对任意数与其他数异或运算 0不变 对不是0的数则该数与0互换值，剩下两个互换值也在该数与剩余3个数进行运算后互换值
		//    对corn前四  15 13 12 14
		//    对应值      19 17 16 18
		//    顺序        	1  3  0  2
		//    结果        0  2  1  3
		//    即互换值
		//    下面说明这么换得合理性
		//      1，先说明对corn前四个这一步不需其顺序必须按照12，13，14，15.
		//         从魔方中心看这8块（4组）是一样的，等价的，12～15我们自己定义的。
		//         且这步的目的是使角分成两部分。
		//         我们只管各个(4个）的对应情况是否满足，顺序无影响。即使不是按12到顺序，我们就把他当作是。
		//      2，就对特殊的情况分析     初始状态	     UFR UBL DFL DBR
		//           按v_pos（12～19）顺序  初始对应状态  DLB DRF URB ULF
		//           当旋转任意面180比如r转后           dbr ubl dfl ufr
		//                                             dlb urb drf ulf
		//           由180度旋转而来是等价的 若我们仍按初始12～15的顺序排列变为
		//                                             UFR UBL DFL DBR
		//                                             ulf urb drf dlb 只有中间两个和外边两个换

		} break;

	case 6://two edge and one corner orbit, permutation
		// 对各个圈中顺序的情况，可同样当作24进制
		n = permtonum(v_pos,0)*576 + permtonum(v_pos,4)*24 + permtonum(v_pos,12);
		break;

	case 7://one edge and one corner orbit, permutation
		// 对各个圈中顺序的情况，可同样当作24进制
		n = permtonum(v_pos,8)*24 + permtonum(v_pos,16);
		break;
    }

    //document.write( "<br>ti=" + t + ", n=" + n + ", len=" + tables[t].length );

    return n;
}

extern int resCount;
extern CString resValue[128];

/////////////////////////////////////
// 产生一个表格供寻找得到解决途径
// 输入: ti--表索引
// calculate a pruning table
void CCubeTable::filltable( int ti )
{
    int i;
    //int tl = tables[ti].length;
    int tl = tablesize[ti];
	char *tb = tables[ ti ];
    //document.write( "<br>tables: " + ti + ", len=" + tl );

    // alocate table memory
    //var tb = tables[ti] = new char[tl];

    //document.write( "<br><<<<----------" );

    //clear table
    //memset( tb, 0, tl);
    //for( i=0; i<tl; i++ )
	//	tables[ti][i] = 0;
	memset( tb, 0, tl );
    
    //回到复原状态
    reset();

    //标记当前深度为1
    tb[ getposition(ti) ] = 1;

    // while there are positions of depth l
    int n = 1, layor = 1;
	int max_layor = 0;

    while( n > 0 )
    {
		//当有魔方在lll层 若没有的话经过下面n=0 却不能用到下面的n++而退出
		//过程先lll一直加 在增加到a后 对a层所有情况处理有继续到a+1层的
		// a层处理完后到a+1，但a+1层所有情况都不能使某一情况对应a+2层，
		// 且不能使n增大，lll变为a+2，n=0退出
		n = 0;

		// find each position of depth lll
		for( i=0; i<tl; i++ )
		{
			//if( ( ti==6 ) && ( lll==9 ) )
			//  break;

			//遍历来找lll层所有情况
			if( tb[i] == layor )
			{
				//document.write( "<br>i: " + i + ", lll=" + lll );
				//construct that cube position
				//使魔方到table【ti】的序列i情况
				setposition( ti, i );          
				//document.write( "<br>v_ori = " + v_ori );
     
				// try each face any amount
				// 尝试每个面任何次
				for( int f=0; f<6; f++ )
				{
					// q = 1, 转动90度---正转
					// q = 2，转动180度--对转
					// q = 3，转动270度--反转
					for( int q=1; q<4; q++ )
					{
						domove( f );

						// get resulting position
						// 得到旋转后的序列
						int r = getposition( ti );

						// if move as allowed in that phase, and position is a new one
						// 如果魔方1:允许当前转动
						// a：任何情况下都允许每面转两次
						// b：ti=1，1&6=0, 任何面都可转动；
						//    ti=2，3      不能转fb面；
						//    ti=4，5      只能转ud面
						//    ti=6，7      只能每面转180度。
						//  注意，面序为FBRLUD
						if( ( (q==2) || (f>=(ti&6)) ) && (tables[ti][r] == 0) )
						{
							// mark that position as depth l+1
							tb[r] = layor+1;  // 使达到的最大深度加一
							n++;
						}
					}

					//使回原状态
					domove( f );
				}
			}
		}

		//if( ti >= 4 )
        //TRACE( "\ntable[%d], layor[%d], n = %d", ti, layor, n );

		// 深度加一
		layor++;

		if( layor > max_layor )
			max_layor = layor;
		
		//if( layor > 8 )
		//	break;

	} //当ti=0时 getposition得0 通过!tb[r]退出


	int k[12] = { 0 };
	int min = tb[ 0 ], max = tb[ 0 ];
	for( i=1; i<tl; i++ )
	{
		if( tb[ i ] < min ) min = tb[i];
		if( tb[ i ] > max ) max = tb[i];

		k[ tb[ i ] - 1 ]++;
	}

    //TRACE( "table[%d], [%d, %d], {%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d}, maxlayor = %d  %s", ti, min, max, 
	//	k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8], k[9], k[10], k[11], max_layor,
	//	( max_layor == numLayor[ti] ) ? "√" : "★" );
}
