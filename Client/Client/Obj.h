#pragma once
#include "SceneMgr.h"

class CObj
{
public:
	CObj();
	CObj(const INFO & rInfo);
	virtual ~CObj();

public:
	const INFO& GetInfo() const { return m_tInfo; }
	const RECT& GetRect() const { return m_tRect; }
	const COLLINFO&  GetColInfo() const { return m_tColInfo; }
	const RECT&		 GetColRect() const { return m_tColRect; }
	float GetAttackXPos() { return m_fAttackX; }
	float GetAttackYPos() { return m_fAttackY; }
	Layer::ID GetLayer() { return m_eLayer; }
	const FRAME& GetFrame() { return m_tFrame; }

public:
	void SetInfo(const INFO& rInfo) 
	{ 
		//if(rInfo.szName.data() != nullptr)
			m_tInfo = rInfo;
	}
	void SetDmg(int _dmg) { m_tInfo.iAtt = _dmg; }
	void SetSize(float _cx, float _cy) { m_tInfo.fCX = _cx, m_tInfo.fCY = _cy; }
	void SetPos(float _x, float _y) { m_tInfo.fX = _x, m_tInfo.fY = _y; }
	void SetColPos(float _x, float _y) { m_tColInfo.fX = _x, m_tColInfo.fY = _y; }
	void SetAngle(float _angle) { m_fAngle = _angle; }
	void SetHp(int _dmg) { m_tInfo.iHp -= _dmg; }
	void SetExp(int _exp) { m_tInfo.iExp += _exp; }
	void SetMoney(int _money) { m_tInfo.iGold += _money; }
	void SetMaxHp(int _maxhp) { m_tInfo.iMaxHp += _maxhp; }
	void SetMaxMp(int _maxmp) { m_tInfo.iMaxMp += _maxmp; }
	void SetMaxSp(int _maxsp) { m_tInfo.iMaxSp += _maxsp; }
	void SetAtt(int _att)  { m_tInfo.iAtt += _att; }
	void SetName(char* pName) { strcpy_s(m_szName, pName); }
	void SetCurScene(const CSceneMgr::SCENE& eID) { m_eCurScene = eID; }
	void SetIsDead(const bool& bIsDead) { m_bIsDead = bIsDead; }
	void SetHpInfo(const int& hp) { m_tInfo.iHp = hp; }

	//////////////////////////////////////////////////////////////////
	void SetPlayerHp(const int& hp)		{ m_tInfo.iMaxHp = hp; m_tInfo.iHp = hp; };
	void SetPlayerMp(const int& mp)		{ m_tInfo.iMaxMp = mp; m_tInfo.iMp = mp; };
	void SetPlayerSp(const int& sp)		{ m_tInfo.iMaxSp = sp; m_tInfo.iSp = sp; };
	void SetPlayerAtt(const int& att)	{ m_tInfo.iAtt = att; };

	const CSceneMgr::SCENE& GetSceneID() { return m_eCurScene; }

	/*충돌 밀어내기 좌표세팅*/
	void AddSetXpos(float _x) { m_tInfo.fX += _x; }
	void AddSetYpos(float _y) { m_tInfo.fY += _y; }
	void AddSetColInfoX(float _x) { m_tColInfo.fX += _x; }
	void AddSetColInfoY(float _y) { m_tColInfo.fY += _y; }

public:
	virtual void Initialize() = 0;
	virtual void LateInit();
	virtual int  Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;

public:
	void UpdateRect();
	void UpdateColRect();
	void UpdateColXYPos();

	void bIsDead() { m_bIsDead = true; }

	void MoveFrame();
protected:
	// Server Data Receive.
	int ReceiveData(SOCKET s, char* buf, int len, int flags);

protected:
	/* ____________________________[서버 관련 함수]____________________________ */
	/* CLIENT MOVE PACKET */
	void send_move_packet(unsigned char dir);
	/* SEND 함수*/
	void send_packet(void* packet);
	/* RECV 함수 */
	void Recv_DataFromServer();
	/* 패킷 처리(모든 컨텐츠를 처리해야 한다) */
	void ProcessPacket(char* ptr);

	void process_data(char* net_buf, size_t io_byte);

protected:
	INFO m_tInfo;
	RECT m_tRect;	//이미지 박스

	/*충돌 박스*/
	COLLINFO m_tColInfo;
	RECT	 m_tColRect;

	/*이미지 프레임 정보*/
	FRAME m_tFrame;

	/*이동 속도*/
	float m_fSpeed;

	/*공격 방향*/
	float m_fAngle;
	/*공격 Object 생성 위치.*/
	float m_fAttackX;
	float m_fAttackY;

	bool m_bLateInit;
	bool m_bIsDead;

	
	Layer::ID m_eLayer;

	/* server */
	char m_szName[MAX_ID_LEN] = "";
	char m_recv_buf[PROTOCOL_TEST::MAX_BUF_SIZE] = "";


	CSceneMgr::SCENE	m_eCurScene = CSceneMgr::SC_END;
	ObjDir::DIR			m_eCurDir = ObjDir::DOWN;	// 현재 방향.
	ObjDir::DIR			m_ePreDir = ObjDir::DOWN;	// 이전 방향.
};

