// CubeTable.cpp: implementation of the CCubeTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CubeTable.h"

#define CHAROFFSET	65

//����λ���������鳯������ ��3 ��2
//ǰ12��Ϊ��, ��8��Ϊ��
const int val[ 20 ] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3 };

//Each move consists of two 4-cycles. This string contains these in FBRLUD order.
//�԰˸�Ϊһ��ֱ����FBRLUD��, �Դ���Ϊ��ת���ϸ�λ�õķ����������ṩ׼ȷ�仯��ʽ
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

	//ȡ��ִ�г���EXE�ļ�����·��
	CString strPath;
	LPTSTR pFileName = strPath.GetBuffer( MAX_PATH+4 );
	GetModuleFileName( 0, pFileName, MAX_PATH );
	strPath.ReleaseBuffer();
	strPath = strPath.Left( strPath.ReverseFind( '\\' ) );

	//���ɡ����������
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
//��������������������ֵ
#define SWAP( ppp, aaa, bbb )	{ temp = ppp[aaa]; ppp[aaa] = ppp[bbb]; ppp[bbb] = temp; }

/////////////////////////////////////
// set cube to solved position
void CCubeTable::reset()
{
	for( int i=0; i<20; i++ )
		v_pos[i] = i, v_ori[i] = 0;
}

// Cycles 4 pieces in array p, the piece indices given by a[0..3].
// ��ת*p�е�4����, 4����λ����a��0��1��2��3������
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
//ת��
//m��0��5 �ֱ����fbrlud�����ת ˳ʱ��
//do a clockwise quarter turn cube move
//���룺m��ʾ�棬FBRLUD��0F 1B 2R 3L 4U 5D
void CCubeTable::domove( int m )
{    
    int i = 8;
	char *p = (char *)perm + 8*m;
    
    //��ת���⡱
    cycle( v_pos, p );  //��תһ�����ĸ����, ��ǰ��õ�ǰһ��ķ���ֵ
    cycle( v_ori, p );  //��תһ�����ĸ����, ��ǰ��õ�ǰһ����ĳ���

    //��ת���ǡ�
    cycle( v_pos, p+4 );  //��תһ�����ĸ��ǿ�, ��ǰ��õ�ǰһ��ķ���ֵ
    cycle( v_ori, p+4 );  //��תһ�����ĸ��ǿ�, ��ǰ��õ�ǰһ����ĳ���

    //twist corners if RLFB
    //��ת�ǿ鳯�������RLFB��ת��
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

		//ע�⵽*perm��ǰ���鼴��32������fbrl��ת���ַ���ÿ��ĺ��ĸ��ǿ鶼�ǰ��ɸ������Ͻ���ʱ���˳��
		//��P��5����p��7�� �ÿ鷽��ֵ=(�ÿ鷽��ֵ����ǰһ��ղŵķ���ֵ��+2��%3
		//  ��5 7   1 ��������(�ͳ���Ҫ��ת���棩 0�������� 2�������ң�Ҫ��ת��������棩
		//  ����	֮��
		//  0	2 ԭ��ǰһ����������Ϊ�������������
		//  1	0 ԭ��ǰһ����������Ϊ�������������
		//  2	1 ԭ��ǰһ���������������
		//  ��4 6        �ÿ鷽��ֵ=(�ÿ鷽��ֵ����ǰһ��ղŵķ���ֵ��+1��%3
		//  ����	֮��
		//  0	1
		//  1	2
		//  2	0 һ���ķ���
		//  �� 0��������ֻ��1��2��5��7��4��6�䲻ͬ������Ϊ�Ե�ǰ���4��6Ϊħ����������������ת90�����5��7λ
		//   ͬ���ķ���ֵ�в�ͬ������, �� ��ÿһ����ת�����ǵ�ǰ�� ��ת��Դ���һ�濴��Ȼ������ת��Ľ��
    }

    //flip edges if FB
    //��ת��鳯�������FB��ת��
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
		//��������д��0��1������1��0
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

		p[ofp+a] = n % (4-a) + ofp;  // n%��4-a)ֻ���ṩ0��3��Ҫ��������λ��  ��ʱΪ�Ⱥ�������
   
		n /= (4-a);

		//for( var b=a; ++b<4; )
		for( int b=a+1; b<4; b++ )
		{
			//document.write( "<br> a = " + a + ", b = " + b );
			if( p[ofp+b] >= p[ofp+a] )
				p[ofp+b]++;  //��ǰ���Ӽ����ټ��ϵ����Լ���һ��
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

    // �ٸ����� 3 2 4 1 ��2*3+1��*2+1=15
    // ���ַ���a*3+b)*2+c=6*a+2b+c ��������֪ʶ
    // Ҳ����ϵ���� ���ǰ���󡢴Ӻ���ǰ ÿ��������һֱ�˵����������������Ľ׳�
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
// �õ���ǰħ��״̬�ڵ�ǰ�׶α���е�����
// get index of cube position from table t
int CCubeTable::getposition( int t )
{
    int i=-1, n=0;

    switch( t )
    {
    // case 0 does nothing so returns 0
    //��t=0 ����0

    // �����i����������򲻶ԣ��ͼ���1����i�� 0���൱��ûЧ�� ����12��������λ����ʾ��鳯��
    case 1: //edgeflip
		// 12 bits, set bit if edge is flipped      
		//for(;++i<12;)
		for( i=0; i<12; i++ )
		{
			//document.write( "<br> i = " + i );
			n += ( v_ori[i] << i );
		}
		break;

    // �Խǿ鳯�򣬿���Ϊ��8��������λ
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
		// ��0��11�鵱v_pos��i��Ϊ8��9��10��11ʱ�ƶ�iλ �� ��12��������λ����ʾ�в����λ��
		//for(;++i<12;)
		for( i=0; i<12; i++ )
		{
			//document.write( "<br> i = " + i );
			n += (v_pos[i] & 8) ? ( 1<<i ) : 0;
		}
		break;

    case 4://ud slice choice
		// 8 bits, set bit if UD edge belongs in Fm middle slice
		// ���²���Ƭ����������Ѱ��в㰲�ã�����ֻ��uf��df��ub��db��ru��lu��rd��ld���γ�����Ȧ��
		// �����ru��rd��lu��ld�Ŀ���б�ǡ�
		for( i=0; i<8; i++ )
			n += (v_pos[i] & 4) ? ( 1<<i ) : 0;
		break;

    //�ķ������ ���ɸ����180����ת��ÿ���ǿ��ܴ���λ���γ�һ����
    // case4��5��ɺ�ħ����״̬���и���180���γɣ����Ը��������ͨ��180��ת���γɵı�������һ����
    case 5: { //tetrad choice, twist and parity
		// �ý׶η����������õ����� ��һ�׶���һ�����ϸ��������ڵ�λ�� �ڶ��׶��Ƕ�Ӧ�ǿ����γɵ�˳��
		int j, k, cl, corn1[8] = { 0 }, corn2[4] = { 0 };
		// 8 bits, set bit if corner belongs in second tetrad.
		// also separate pieces for twist/parity determination
		k = j = 0;
		for( i=0; i<8; i++ )
		{
			if( (cl=(v_pos[i+12]-12)) & 4 )
			{
				//corn��4��7���ֱ��ʾ16~19�ǿ�ĳ���˳��
				corn1[cl] = k++;
				n += 1<<i;
			}
			else
			{
				//corn��0~3) corn��0����ʾ12��15��һ����-12��corn��1����ʾ12��15�ڶ�����-12��...
				corn1[j++] = cl;
			}
		}
      
		//Find permutation of second tetrad after solving first
		for( i=0; i<4; i++)
		{
			//document.write( "<br> i = " + i );
			//corn[i]��ֵ�Ƕ��٣�corn2��i��=��ֵ��4��16��19���ֵ�˳��
			corn2[i] = corn1[4+corn1[i]];
		}
        
		//Solve one piece of second tetrad
		//for(;--i;)
		for( i=3; i>0; i--)      
		{
			//document.write( "<br> i = " + i );
			// ת���Եõ����λ�� ��corn2��0����Ӧ����˳����0�������Լ�����������Ϊ0����δ��ֵ�������·��������Ժ�����������
			corn2[i] ^= corn2[0];
		}

		// encode parity/tetrad twist
		// ����ÿһ��16��19����λ�õ����У����̶�����ֵʣ����6������
		n = n*6 + corn2[1]*2 - 2;

		//�������������������
		if( ( corn2[3] + 0.5 ) < corn2[2] )  //��������ʾ�������������"<"�����ٲ�1
			n++;

		// �ȷ���һ�� ���������Ч��
		//    ^	0	1	2	3
		//    0	0	1	2	3
		//    1	1	0	3	2
		//    2	2	3	0	1
		//    3	3	2	1	0
		//    ����������������������� 0���� �Բ���0�����������0����ֵ��ʣ����������ֵҲ�ڸ�����ʣ��3������������󻥻�ֵ
		//    ��cornǰ��  15 13 12 14
		//    ��Ӧֵ      19 17 16 18
		//    ˳��        	1  3  0  2
		//    ���        0  2  1  3
		//    ������ֵ
		//    ����˵����ô���ú�����
		//      1����˵����cornǰ�ĸ���һ��������˳����밴��12��13��14��15.
		//         ��ħ�����Ŀ���8�飨4�飩��һ���ģ��ȼ۵ģ�12��15�����Լ�����ġ�
		//         ���ⲽ��Ŀ����ʹ�Ƿֳ������֡�
		//         ����ֻ�ܸ���(4�����Ķ�Ӧ����Ƿ����㣬˳����Ӱ�졣��ʹ���ǰ�12��˳�����ǾͰ��������ǡ�
		//      2���Ͷ�������������     ��ʼ״̬	     UFR UBL DFL DBR
		//           ��v_pos��12��19��˳��  ��ʼ��Ӧ״̬  DLB DRF URB ULF
		//           ����ת������180����rת��           dbr ubl dfl ufr
		//                                             dlb urb drf ulf
		//           ��180����ת�����ǵȼ۵� �������԰���ʼ12��15��˳�����б�Ϊ
		//                                             UFR UBL DFL DBR
		//                                             ulf urb drf dlb ֻ���м����������������

		} break;

	case 6://two edge and one corner orbit, permutation
		// �Ը���Ȧ��˳����������ͬ������24����
		n = permtonum(v_pos,0)*576 + permtonum(v_pos,4)*24 + permtonum(v_pos,12);
		break;

	case 7://one edge and one corner orbit, permutation
		// �Ը���Ȧ��˳����������ͬ������24����
		n = permtonum(v_pos,8)*24 + permtonum(v_pos,16);
		break;
    }

    //document.write( "<br>ti=" + t + ", n=" + n + ", len=" + tables[t].length );

    return n;
}

extern int resCount;
extern CString resValue[128];

/////////////////////////////////////
// ����һ�����Ѱ�ҵõ����;��
// ����: ti--������
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
    
    //�ص���ԭ״̬
    reset();

    //��ǵ�ǰ���Ϊ1
    tb[ getposition(ti) ] = 1;

    // while there are positions of depth l
    int n = 1, layor = 1;
	int max_layor = 0;

    while( n > 0 )
    {
		//����ħ����lll�� ��û�еĻ���������n=0 ȴ�����õ������n++���˳�
		//������lllһֱ�� �����ӵ�a�� ��a��������������м�����a+1���
		// a�㴦�����a+1����a+1���������������ʹĳһ�����Ӧa+2�㣬
		// �Ҳ���ʹn����lll��Ϊa+2��n=0�˳�
		n = 0;

		// find each position of depth lll
		for( i=0; i<tl; i++ )
		{
			//if( ( ti==6 ) && ( lll==9 ) )
			//  break;

			//��������lll���������
			if( tb[i] == layor )
			{
				//document.write( "<br>i: " + i + ", lll=" + lll );
				//construct that cube position
				//ʹħ����table��ti��������i���
				setposition( ti, i );          
				//document.write( "<br>v_ori = " + v_ori );
     
				// try each face any amount
				// ����ÿ�����κδ�
				for( int f=0; f<6; f++ )
				{
					// q = 1, ת��90��---��ת
					// q = 2��ת��180��--��ת
					// q = 3��ת��270��--��ת
					for( int q=1; q<4; q++ )
					{
						domove( f );

						// get resulting position
						// �õ���ת�������
						int r = getposition( ti );

						// if move as allowed in that phase, and position is a new one
						// ���ħ��1:����ǰת��
						// a���κ�����¶�����ÿ��ת����
						// b��ti=1��1&6=0, �κ��涼��ת����
						//    ti=2��3      ����תfb�棻
						//    ti=4��5      ֻ��תud��
						//    ti=6��7      ֻ��ÿ��ת180�ȡ�
						//  ע�⣬����ΪFBRLUD
						if( ( (q==2) || (f>=(ti&6)) ) && (tables[ti][r] == 0) )
						{
							// mark that position as depth l+1
							tb[r] = layor+1;  // ʹ�ﵽ�������ȼ�һ
							n++;
						}
					}

					//ʹ��ԭ״̬
					domove( f );
				}
			}
		}

		//if( ti >= 4 )
        //TRACE( "\ntable[%d], layor[%d], n = %d", ti, layor, n );

		// ��ȼ�һ
		layor++;

		if( layor > max_layor )
			max_layor = layor;
		
		//if( layor > 8 )
		//	break;

	} //��ti=0ʱ getposition��0 ͨ��!tb[r]�˳�


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
	//	( max_layor == numLayor[ti] ) ? "��" : "��" );
}
