// CubeCalc.h: interface for the CCubeCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBECALC_H__35EF9A3D_70FF_400D_B782_A194DAC9E4B2__INCLUDED_)
#define AFX_CUBECALC_H__35EF9A3D_70FF_400D_B782_A194DAC9E4B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// number 65='A' is often subtracted to convert char ABC... to number 0,1,2,...
#define CHAROFFSET	65

//
class CCubeCalc : public CObject  
{
public:
	CCubeCalc();
	virtual ~CCubeCalc();

	void GetPlan( CString strInput, DWORD *TotalNnn, int *nnn );

public:
	CString m_strplan[4];

private:
	// 魔方当前位置
	int v_pos[ 20 ];	//各个位置所含的方块， 0..19
	int v_ori[ 20 ];	//各个位置所含方块的朝向, 为0, 1, 2 
	int v_err[ 20 ];	//各个位置所含方块的颜色顺序是否正确

	// current phase being searched (0,2,4,6 for phases 1 to 4)
	int phase;

	// 临时变量
	int temp;

	// current phase solution
	int moveface[20];
	int moveamount[20];

	int resLen;
	char resVal[256][8];

	static const char *faces;
	static const char *perm;
	static const char *order;
	static const char *bithash;
	static const char bithashtoinput[][4];
	static const char *moveFace;

	static const unsigned long  table0[];
	static const unsigned long  table1[];
	static const unsigned long  table2[];
	static const unsigned long  table3[];
	static const unsigned long  table4[];
	static const unsigned long  table5[];
	static const unsigned long  table6[];
	static const unsigned long  table7[];
	static const unsigned long *tables[8];

private:
	void cycle( int *p, char *a );
	void twist( int i, int a );
	void domove( int m );
	int permtonum( int *p, int ofp );
	int getposition( int t );
	bool searchphase( int movesleft, int movesdone, int lastmove );
};

#endif // !defined(AFX_CUBECALC_H__35EF9A3D_70FF_400D_B782_A194DAC9E4B2__INCLUDED_)
