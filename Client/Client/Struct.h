#pragma once

typedef struct tagInfo
{
	float	fCX;
	float	fCY;
	float	fX;
	float	fY;

	wstring	szName;
	int		iLevel;

	int		iMaxHp;
	int		iHp;

	int		iMaxMp;
	int		iMp;

	int		iAtt;

	int		iMaxExp;
	int		iExp;

	int		iGold;

	int		iMaxSp;
	int		iSp;

}INFO;

typedef struct tagCollInfo
{
	float	fCX;
	float	fCY;
	float	fX;
	float	fY;
}COLLINFO;


typedef struct tagFrame
{
	int iFrameStart;	//�̹��� x�� ������
	int iFrameEnd;		//�̹��� y�� ������
	int iScene;			//�̹��� y�� ������

	DWORD dwOldTime;
	DWORD dwFrameSpd;	//�̹��� ����ӵ�
}FRAME;


typedef struct tagTextRectInfo
{
	float	fCX;
	float	fCY;
	float	fX;
	float	fY;
}TEXTINFO;

typedef struct tagPlayerInfo
{
	int iStap;
	int iInt;
	int iHack;
	int iDef;
	int iMr;
	int iDex;
	int iAgl;
}PLAYERINFO;

struct Point
{
	int x = 0;
	int y = 0;
};



/////////////////////////////////////////////////

typedef struct tagClientRecvPacket
{
	bool	is_connected;
	float	cx;
	float	cy;
	float	x;
	float	y;
	wstring	name;
	int		level;
	int		hp;
	int		mp;
	int		sp;
	int		att;
	float	speed;
	int		exp;

} cl_recv;

typedef struct tagClientSendPacket
{
	bool is_connected;
	char input;

} cl_send;